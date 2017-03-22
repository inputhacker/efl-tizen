#include "evas_common_private.h"
#include "evas_private.h"
#ifdef EVAS_CSERVE2
#include "evas_cs2_private.h"
#endif
#include "evas_engine.h"
#include "evas_native_common.h"
#include <tbm_surface.h>

#ifdef HAVE_DLSYM
# include <dlfcn.h>
#endif

/* logging domain variable */
int _evas_engine_software_tbm_log_dom = -1;

/* evas function tables - filled in later (func and parent func) */
static Evas_Func func, pfunc;

/* For wl_buffer's native set */
static void *tbm_server_lib = NULL;
static tbm_surface_h (*glsym_wayland_tbm_server_get_surface) (struct wayland_tbm_server *tbm_srv, struct wl_resource *wl_buffer) = NULL;

/* engine structure data */
typedef struct _Render_Engine Render_Engine;
struct _Render_Engine
{
   Render_Engine_Software_Generic generic;

   void (*outbuf_reconfigure)(Outbuf *ob, int x, int y, int w, int h, int rot, Outbuf_Depth depth, Eina_Bool alpha, Eina_Bool resize);
};

/* LOCAL FUNCTIONS */
static Render_Engine *
_render_engine_ouput_setup(Evas_Engine_Info_Software_Tbm *info, int w, int h)
{
   Render_Engine *re = NULL;
   Outbuf *ob;

   /* try to allocate space for our render engine structure */
   if (!(re = calloc(1, sizeof(Render_Engine))))
     goto on_error;

   /* try to create new outbuf */
   if (!(ob = evas_outbuf_setup(info, w, h)))
     goto on_error;

   if (!evas_render_engine_software_generic_init(&re->generic, ob,
                                                 evas_outbuf_swap_mode_get,
                                                 evas_outbuf_rot_get,
                                                 evas_outbuf_reconfigure,
                                                 evas_outbuf_update_region_first_rect,
                                                 evas_outbuf_update_region_new,
                                                 evas_outbuf_update_region_push,
                                                 evas_outbuf_update_region_free,
                                                 evas_output_idle_flush,
                                                 evas_outbuf_flush,
                                                 evas_outbuf_free, 
                                                 ob->w, ob->h))
     goto on_error;

   /* return the allocated render_engine structure */
   return re;

 on_error:
   if (re) evas_render_engine_software_generic_clean(&re->generic);

   free(re);
   return NULL;
}

static void
_symbols(void)
{
   static int done = 0;
   int fail = 0;
   const char *wayland_tbm_server_lib = "libwayland-tbm-server.so.0";

   if (done) return;

#define LINK2GENERIC(sym) \
   glsym_##sym = dlsym(RTLD_DEFAULT, #sym);

   tbm_server_lib = dlopen(wayland_tbm_server_lib, RTLD_LOCAL | RTLD_LAZY);
   if (tbm_server_lib)
     {
        LINK2GENERIC(wayland_tbm_server_get_surface);
        if (fail == 1)
          {
             ERR("fail to dlsym about wayland_tbm_server_get_surface symbol");
             dlclose(tbm_server_lib);
             tbm_server_lib = NULL;
             return;
          }
     }
   else
     return;

   done = 1;
}

/* ENGINE API FUNCTIONS WE PROVIDE */
static void *
eng_info(Evas *eo_evas EINA_UNUSED)
{
   Evas_Engine_Info_Software_Tbm *einfo;

   LOGFN(__FILE__, __LINE__, __FUNCTION__);

   /* try to allocate space for new engine info */
   if (!(einfo = calloc(1, sizeof(Evas_Engine_Info_Software_Tbm))))
     return NULL;

   /* fill in engine info */
   einfo->magic.magic = rand();
   einfo->render_mode = EVAS_RENDER_MODE_BLOCKING;

   /* return allocated engine info */
   return einfo;
}

static void
eng_info_free(Evas *eo_evas EINA_UNUSED, void *info)
{
   Evas_Engine_Info_Software_Tbm *einfo;

   LOGFN(__FILE__, __LINE__, __FUNCTION__);

   /* try to free previously allocated engine info */
   if ((einfo = (Evas_Engine_Info_Software_Tbm *)info))
     free(einfo);
}

static int
eng_setup(Evas *eo_evas, void *info)
{
   Evas_Engine_Info_Software_Tbm *einfo;
   Evas_Public_Data *epd;
   Render_Engine *re = NULL;

   LOGFN(__FILE__, __LINE__, __FUNCTION__);
   /* try to cast to our engine info */
   if (!(einfo = (Evas_Engine_Info_Software_Tbm *)info))
     return 0;

   /* try to get evas public data */
   if (!(epd = eo_data_scope_get(eo_evas, EVAS_CANVAS_CLASS)))
     return 0;

   /* test for valid engine output */
   if (!(re = epd->engine.data.output))
     {
        /* if we have no engine data, assume we have not initialized yet */
        evas_common_init();

        re = _render_engine_ouput_setup(einfo, epd->output.w, epd->output.h);

        if (re)
          re->generic.ob->info = einfo;
        else
          goto err;
     }
   else
     {
        Outbuf *ob;

        ob = evas_outbuf_setup(einfo, epd->output.w, epd->output.h);
        if (!ob)
          {
             ERR("Failed to evas_outbuf_setup");
             return 0;
          }

        evas_render_engine_software_generic_update(&re->generic, ob,
                                                   epd->output.w,
                                                   epd->output.h);
     }

   epd->engine.data.output = re;
   if (!epd->engine.data.output)
     {
        ERR("Failed to create Render Engine");
        goto err;
     }

   if (!epd->engine.data.context)
     {
        epd->engine.data.context =
          epd->engine.func->context_new(epd->engine.data.output);
     }

   return 1;

err:
   evas_common_shutdown();
   return 0;
}

static void
eng_output_free(void *data)
{
   Render_Engine *re;

   if ((re = (Render_Engine *)data))
     {
        evas_render_engine_software_generic_clean(&re->generic);
        free(re);
     }

   if (tbm_server_lib)
     {
       dlclose(tbm_server_lib);
       tbm_server_lib = NULL;
     }

   evas_common_shutdown();
}

static void *
eng_image_native_set(void *data EINA_UNUSED, void *image, void *native)
{
   Evas_Native_Surface *ns = native;
   Image_Entry *ie = image;
   RGBA_Image *im = image, *im2;

   if (!im) return im;

   if (ns->type == EVAS_NATIVE_SURFACE_TBM)
     {
        if (im->native.data)
          {
             //image have native surface already
             Evas_Native_Surface *ens = im->native.data;

             if ((ens->type == ns->type) &&
                 (ens->data.tbm.buffer == ns->data.tbm.buffer))
                return im;
          }
      }
   else if (ns->type == EVAS_NATIVE_SURFACE_WL)
     {
        if (im->native.data)
          {
             Evas_Native_Surface *ens;

             ens = im->native.data;
             if (ens->data.wl.legacy_buffer == ns->data.wl.legacy_buffer)
               return im;
          }
     }

   im2 = (RGBA_Image *)evas_cache_image_data(evas_common_image_cache_get(),
                                 ie->w, ie->h,
                                 NULL, 1,
                                 EVAS_COLORSPACE_ARGB8888);

   if (im->native.data)
      {
         if (im->native.func.free)
            im->native.func.free(im->native.func.data, im);
      }

   if (!ns) return im;

#ifdef EVAS_CSERVE2
   if (evas_cserve2_use_get() && evas_cache2_image_cached(ie))
     evas_cache2_image_close(ie);
   else
#endif
     evas_cache_image_drop(ie);

   im = im2;

   if (ns->type == EVAS_NATIVE_SURFACE_TBM)
     {
          return evas_native_tbm_surface_image_set(NULL, im, ns);
     }
   else if (ns->type == EVAS_NATIVE_SURFACE_WL)
     {
       // TODO  : need the code for all wl_buffer type
       // For TBM surface
       if (glsym_wayland_tbm_server_get_surface)
         {
            tbm_surface_h _tbm_surface;
            tbm_surface_info_s info;

            _tbm_surface = glsym_wayland_tbm_server_get_surface(NULL,ns->data.wl.legacy_buffer);

            tbm_surface_get_info(_tbm_surface, &info);

            return evas_native_tbm_surface_image_set(_tbm_surface, im, ns);
         }
       else
         {
            return NULL;
         }
     }

   return im;
}

static void *
eng_image_native_get(void *data EINA_UNUSED, void *image)
{
   RGBA_Image *im = image;
   Native *n;
   if (!im) return NULL;
   n = im->native.data;
   if (!n) return NULL;
   return &(n->ns);
}

/* EVAS MODULE FUNCTIONS */
static int
module_open(Evas_Module *em)
{
   LOGFN(__FILE__, __LINE__, __FUNCTION__);

   /* check for valid evas module */
   if (!em) return 0;

   /* try to get functions from whatever engine module we inherit from */
   if (!_evas_module_engine_inherit(&pfunc, "software_generic")) return 0;

   /* try to create our logging domain */
   _evas_engine_software_tbm_log_dom =
     eina_log_domain_register("evas-software_tbm", EVAS_DEFAULT_LOG_COLOR);
   if (_evas_engine_software_tbm_log_dom < 0)
     {
        EINA_LOG_ERR("Cannot create a module logging domain");
        return 0;
     }

   /* copy parent functions */
   func = pfunc;
   /* override engine specific functions */
#define ORD(f) EVAS_API_OVERRIDE(f, &func, eng_)
   ORD(info);
   ORD(info_free);
   ORD(setup);
   ORD(output_free);
   ORD(image_native_set);
   ORD(image_native_get);

   _symbols();
   /* advertise our own engine functions */
   em->functions = (void *)(&func);

   return 1;
}

static void
module_close(Evas_Module *em EINA_UNUSED)
{
   LOGFN(__FILE__, __LINE__, __FUNCTION__);

   /* unregister logging domain */
   if (_evas_engine_software_tbm_log_dom > -1)
     eina_log_domain_unregister(_evas_engine_software_tbm_log_dom);

   /* reset logging domain variable */
   _evas_engine_software_tbm_log_dom = -1;
}

static Evas_Module_Api evas_modapi =
{
   EVAS_MODULE_API_VERSION, "software_tbm", "none", {module_open, module_close}
};

EVAS_MODULE_DEFINE(EVAS_MODULE_TYPE_ENGINE, engine, software_tbm);

#ifndef EVAS_STATIC_BUILD_SOFTWARE_TBM
EVAS_EINA_MODULE_DEFINE(engine, software_tbm);
#endif
