#include "evas_common_private.h"
#include "evas_private.h"
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
static tbm_surface_h (*glsym_wayland_tbm_server_get_surface) (void *tbm_srv, void *wl_buffer) = NULL;

/* engine structure data */
typedef struct _Render_Engine Render_Engine;
struct _Render_Engine
{
   Render_Output_Software_Generic generic;

   void (*outbuf_reconfigure)(Outbuf *ob, int x, int y, int w, int h, int rot, Outbuf_Depth depth, Eina_Bool alpha, Eina_Bool resize);
};

/* LOCAL FUNCTIONS */
static Render_Engine *
_render_engine_ouput_setup(void* engine, Evas_Engine_Info_Software_Tbm *info, int w, int h)
{
   Render_Engine *re = NULL;
   Outbuf *ob;

   /* try to allocate space for our render engine structure */
   if (!(re = calloc(1, sizeof(Render_Engine))))
     goto on_error;

   /* try to create new outbuf */
   if (!(ob = evas_outbuf_setup(info, w, h)))
     goto on_error;

   if (!evas_render_engine_software_generic_init(engine, &re->generic, ob,
                                                 evas_outbuf_swap_mode_get,
                                                 evas_outbuf_rot_get,
                                                 evas_outbuf_reconfigure,
                                                 evas_outbuf_update_region_first_rect,
                                                 NULL,
                                                 evas_outbuf_update_region_new,
                                                 evas_outbuf_update_region_push,
                                                 evas_outbuf_update_region_free,
                                                 evas_output_idle_flush,
                                                 evas_outbuf_flush,
                                                 NULL,
                                                 evas_outbuf_free, 
                                                 w, h))
     goto on_error;

   /* return the allocated render_engine structure */
   return re;

 on_error:
   if (re) evas_render_engine_software_generic_clean(engine, &re->generic);

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
static void
eng_output_info_setup(void *info)
{
   Evas_Engine_Info_Software_Tbm *einfo = info;

   LOGFN(__FILE__, __LINE__, __FUNCTION__);

   einfo->render_mode = EVAS_RENDER_MODE_BLOCKING;
}

static void *
eng_output_setup(void *engine, void *info, unsigned int w, unsigned int h)
{
   Evas_Engine_Info_Software_Tbm *einfo;
   Render_Engine *re = NULL;

   LOGFN(__FILE__, __LINE__, __FUNCTION__);
   /* try to cast to our engine info */
   if (!(einfo = (Evas_Engine_Info_Software_Tbm *)info))
     return NULL;

   /* if we have no engine data, assume we have not initialized yet */
   evas_common_init();

   re = _render_engine_ouput_setup(engine, einfo, w, h);

   if (re)
     {
        re->generic.ob->info = einfo;
        /* init tbm native surface lib */
        _evas_native_tbm_init();
     }
   else
     goto err;

   return re;

err:
   evas_common_shutdown();
   return NULL;
}

static void
eng_output_resize(void *engine EINA_UNUSED, void *data, int w, int h)
{
   Render_Engine *re;
   Evas_Engine_Info_Software_Tbm *einfo;

   LOGFN(__FILE__, __LINE__, __FUNCTION__);

   if (!(re = (Render_Engine *)data)) return;
   if (!(einfo = re->generic.ob->info)) return;

   evas_outbuf_reconfigure(re->generic.ob, w, h,einfo->info.rotation, einfo->info.depth);

   evas_common_tilebuf_free(re->generic.tb);
   if ((re->generic.tb = evas_common_tilebuf_new(w, h)))
     evas_common_tilebuf_set_tile_size(re->generic.tb, TILESIZE, TILESIZE);

   re->generic.w = w;
   re->generic.h = h;
}

static int
eng_output_update(void *engine EINA_UNUSED, void *data, void *info, unsigned int w, unsigned int h)
{
   Evas_Engine_Info_Software_Tbm *einfo = info;
   Render_Engine *re = data;

   Outbuf *ob;

   ob = evas_outbuf_setup(einfo, w, h);
   if (!ob)
     {
        ERR("Failed to evas_outbuf_setup");
        return 0;
     }

   evas_render_engine_software_generic_update(&re->generic, re->generic.ob,
                                              w, h);

   return 1;
}

static void
eng_output_free(void *engine, void *data)
{
   Render_Engine *re;

   if ((re = (Render_Engine *)data))
     {
        evas_render_engine_software_generic_clean(engine, &re->generic);
        free(re);
     }

   /* shutdown tbm native surface lib */
   _evas_native_tbm_shutdown();

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
   RGBA_Image *im = image, *im2 = NULL;

   if (!im) return im;

   if (ns)
     {
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
     }

   if (im->native.data)
      {
         if (im->native.func.free)
            im->native.func.free(im);
      }

   if (!ns) return im;

   if (ns->type == EVAS_NATIVE_SURFACE_TBM)
     {
        Evas_Colorspace cs = _evas_native_tbm_surface_colorspace_get(NULL, ns);
        if (cs == EVAS_COLORSPACE_ARGB8888)
          {
             im2 = (RGBA_Image *)evas_cache_image_data(evas_common_image_cache_get(),
                                                       ie->w, ie->h, NULL, ie->flags.alpha,
                                                       EVAS_COLORSPACE_ARGB8888);
          }
        else
          {
             int stride = _evas_native_tbm_surface_stride_get(NULL, ns);
             if (stride > -1)
               {
                   /**
                    * To support various color format in Native TBM Surface,
                    * Cache Image should have both im->image.data and cs.data memory.
                    * In default, evas_cache_image_copied_data is callled with his colorspace.
                    * In the case, cs.data is allocated and free, then re-allocated.
                    * To optimize, we have two options.
                    * One of them, evas_cache_image_copied_data is called with EVAS_COLORSPACE_ARGB8888
                    * The other option, evas_cache_image_data is called with his colorspace
                    * and evas_cache_image_surface_alloc should be called.
                    * Then, new Cache Image's cs should be set with EVAS_COLORSPACE_ARGB8888.
                    * Because of allocation cs.data in _evas_native_tbm_surface_image_set()
                    * In current, first option is used.
                    **/
                   im2 = (RGBA_Image *)evas_cache_image_copied_data(evas_common_image_cache_get(),
                                                                    stride, ie->h, NULL, ie->flags.alpha,
                                                                    EVAS_COLORSPACE_ARGB8888);
               }
             else
               {
                  ERR("Fail to get stride");
                  return im;
               }
          }
     }
   else
     {
        im2 = (RGBA_Image *)evas_cache_image_data(evas_common_image_cache_get(),
                                                  ie->w, ie->h,
                                                  NULL, 1,
                                                  EVAS_COLORSPACE_ARGB8888);
     }

     evas_cache_image_drop(ie);

   im = im2;

   if (ns->type == EVAS_NATIVE_SURFACE_TBM)
     {
          return _evas_native_tbm_surface_image_set(NULL, im, ns);
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

            return _evas_native_tbm_surface_image_set(_tbm_surface, im, ns);
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

static int
eng_image_native_init(void *engine EINA_UNUSED, Evas_Native_Surface_Type type)
{
   switch (type)
     {
      case EVAS_NATIVE_SURFACE_TBM:
        return _evas_native_tbm_init();
      case EVAS_NATIVE_SURFACE_EVASGL:
        return 1;
      default:
        ERR("Native surface type %d not supported!", type);
        return 0;
     }
}

static void
eng_image_native_shutdown(void *engine EINA_UNUSED, Evas_Native_Surface_Type type)
{
   switch (type)
     {
      case EVAS_NATIVE_SURFACE_TBM:
        _evas_native_tbm_shutdown();
        return;
      default:
        ERR("Native surface type %d not supported!", type);
        return;
     }
}

/* EVAS MODULE FUNCTIONS */
static int
module_open(Evas_Module *em)
{
   LOGFN(__FILE__, __LINE__, __FUNCTION__);

   /* check for valid evas module */
   if (!em) return 0;

   /* try to get functions from whatever engine module we inherit from */
   if (!_evas_module_engine_inherit(&pfunc, "software_generic", sizeof (Evas_Engine_Info_Software_Tbm))) return 0;

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
   ORD(output_info_setup);
   ORD(output_setup);
   ORD(output_update);
   ORD(output_resize);
   ORD(output_free);
   ORD(image_native_set);
   ORD(image_native_get);
   ORD(image_native_init);
   ORD(image_native_shutdown);

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
