#include "evas_common_private.h"
#include "evas_private.h"
#ifdef EVAS_CSERVE2
# include "evas_cs2_private.h"
#endif

#include "evas_engine.h"
#include "../software_generic/evas_native_common.h"

#ifdef HAVE_DLSYM
# include <dlfcn.h>
#endif

/* logging domain variable */
int _evas_engine_way_shm_log_dom = -1;

/* evas function tables - filled in later (func and parent func) */
static Evas_Func func, pfunc;

Evas_Native_Tbm_Surface_Image_Set_Call  glsym_evas_native_tbm_surface_image_set = NULL;

/* For wl_buffer's native set */
static void *tbm_server_lib = NULL;
typedef struct _tbm_surface * tbm_surface_h;
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
_render_engine_swapbuf_setup(int w, int h, Evas_Engine_Info_Wayland_Shm *einfo)
{
   Render_Engine *re;
   Outbuf *ob;
   Render_Engine_Merge_Mode merge_mode = MERGE_SMART;
   const char *s;

   LOGFN(__FILE__, __LINE__, __FUNCTION__);

   /* try to allocate space for new render engine */
   if (!(re = calloc(1, sizeof(Render_Engine)))) return NULL;

   ob = _evas_outbuf_setup(w, h, einfo);
   if (!ob) goto err;

   if (!evas_render_engine_software_generic_init(&re->generic, ob,
                                                 _evas_outbuf_swap_mode_get,
                                                 _evas_outbuf_rotation_get,
                                                 NULL,
                                                 NULL,
                                                 _evas_outbuf_update_region_new,
                                                 _evas_outbuf_update_region_push,
                                                 _evas_outbuf_update_region_free,
                                                 _evas_outbuf_idle_flush,
                                                 _evas_outbuf_flush,
                                                 _evas_outbuf_free,
                                                 w, h))
     goto err;

   re->outbuf_reconfigure = _evas_outbuf_reconfigure;

   s = getenv("EVAS_WAYLAND_PARTIAL_MERGE");
   if (s)
     {
        if ((!strcmp(s, "bounding")) || (!strcmp(s, "b")))
          merge_mode = MERGE_BOUNDING;
        else if ((!strcmp(s, "full")) || (!strcmp(s, "f")))
          merge_mode = MERGE_FULL;
        else if ((!strcmp(s, "smart")) || (!strcmp(s, "s")))
          merge_mode = MERGE_SMART;
     }

   evas_render_engine_software_generic_merge_mode_set(&re->generic, merge_mode);

   /* return allocated render engine */
   return re;

err:
   if (ob) _evas_outbuf_free(ob);
   free(re);
   return NULL;
}

static void
_symbols(void)
{
   static int done = 0;
   int fail = 0;
   const char *wayland_tbm_server_lib = LIBDIR"/libwayland-tbm-server.so.0";

   if (done) return;

#define LINK2GENERIC(lib, sym)                     \
   glsym_##sym = dlsym(lib, #sym);   \
   if (!(glsym_##sym))                        \
     {                                        \
        ERR("%s", dlerror());                 \
        fail = 1;                             \
     }

   // Get function pointer to native_common that is now provided through the link of SW_Generic.
   LINK2GENERIC(RTLD_DEFAULT, evas_native_tbm_surface_image_set);
   if (fail == 1)
     {
        ERR("fail to dlsym about evas_native_tbm_surface_image_set symbol");
        return;
     }
   tbm_server_lib = dlopen(wayland_tbm_server_lib, RTLD_LOCAL | RTLD_LAZY);
   if (tbm_server_lib)
     {
        LINK2GENERIC(tbm_server_lib, wayland_tbm_server_get_surface);
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
eng_info(Evas *eo_evas)
{
   Evas_Engine_Info_Wayland_Shm *einfo;

   LOGFN(__FILE__, __LINE__, __FUNCTION__);

   /* try to allocate space for new engine info */
   if (!(einfo = calloc(1, sizeof(Evas_Engine_Info_Wayland_Shm))))
     return NULL;

   /* fill in engine info */
   einfo->magic.magic = rand();
   einfo->render_mode = EVAS_RENDER_MODE_BLOCKING;
   einfo->evas = eo_evas;

   /* return allocated engine info */
   return einfo;
}

static void
eng_info_free(Evas *eo_evas EINA_UNUSED, void *info)
{
   Evas_Engine_Info_Wayland_Shm *einfo;

   LOGFN(__FILE__, __LINE__, __FUNCTION__);

   /* try to free previously allocated engine info */
   if ((einfo = (Evas_Engine_Info_Wayland_Shm *)info))
     free(einfo);
}

static int
eng_setup(Evas *eo_evas, void *info)
{
   Evas_Engine_Info_Wayland_Shm *einfo;
   Evas_Public_Data *epd;
   Render_Engine *re = NULL;

   LOGFN(__FILE__, __LINE__, __FUNCTION__);
   /* try to cast to our engine info */
   if (!(einfo = (Evas_Engine_Info_Wayland_Shm *)info))
     return 0;

   /* try to get evas public data */
   if (!(epd = eo_data_scope_get(eo_evas, EVAS_CANVAS_CLASS)))
     return 0;

   /* test for valid engine output */
   if (!(re = epd->engine.data.output))
     {
        /* if we have no engine data, assume we have not initialized yet */
        evas_common_init();

        re = _render_engine_swapbuf_setup(epd->output.w, epd->output.h, einfo);

        if (re)
          re->generic.ob->info = einfo;
        else
          goto err;
     }
   else
     {
        Outbuf *ob;

        ob = _evas_outbuf_setup(epd->output.w, epd->output.h, einfo);

        if (ob)
          {
             ob->info = einfo;
             evas_render_engine_software_generic_update(&re->generic, ob,
                                                        epd->output.w,
                                                        epd->output.h);
          }
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

static void
eng_output_resize(void *data, int w, int h)
{
   Render_Engine *re;
   Evas_Engine_Info_Wayland_Shm *einfo;
   int dx = 0, dy = 0;
   Eina_Bool resize = EINA_FALSE;

   LOGFN(__FILE__, __LINE__, __FUNCTION__);

   if (!(re = (Render_Engine *)data)) return;
   if (!(einfo = re->generic.ob->info)) return;

   if (einfo->info.edges & 4) // resize from left
     {
        if ((einfo->info.rotation == 90) || (einfo->info.rotation == 270))
          dx = re->generic.ob->h - h;
        else
          dx = re->generic.ob->w - w;
     }

   if (einfo->info.edges & 1) // resize from top
     {
        if ((einfo->info.rotation == 90) || (einfo->info.rotation == 270))
          dy = re->generic.ob->w - w;
        else
          dy = re->generic.ob->h - h;
     }

   if (einfo->info.edges) resize = EINA_TRUE;

   re->outbuf_reconfigure(re->generic.ob, dx, dy, w, h,
                          einfo->info.rotation, einfo->info.depth,
                          einfo->info.destination_alpha, resize);

   evas_common_tilebuf_free(re->generic.tb);
   if ((re->generic.tb = evas_common_tilebuf_new(w, h)))
     evas_common_tilebuf_set_tile_size(re->generic.tb, TILESIZE, TILESIZE);

   re->generic.w = w;
   re->generic.h = h;
}

static void *
eng_image_native_set(void *data EINA_UNUSED, void *image, void *native)
{
   Evas_Native_Surface *ns = native;
   Image_Entry *ie = image;
   RGBA_Image *im = image, *im2;
   void *wl_buf = NULL;

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
             wl_buf = ns->data.wl.legacy_buffer;
             if (im->native.data)
               {
                  Evas_Native_Surface *ens;

                  ens = im->native.data;
                  if (ens->data.wl.legacy_buffer == wl_buf)
                    return im;
               }
           }

        if ((ns->type == EVAS_NATIVE_SURFACE_OPENGL) &&
            (ns->version == EVAS_NATIVE_SURFACE_VERSION))
          im2 = evas_cache_image_data(evas_common_image_cache_get(),
                                      ie->w, ie->h,
                                      ns->data.x11.visual, 1,
                                      EVAS_COLORSPACE_ARGB8888);
        else
          im2 = evas_cache_image_data(evas_common_image_cache_get(),
                                      ie->w, ie->h,
                                      NULL, 1,
                                      EVAS_COLORSPACE_ARGB8888);
     }

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
        if (glsym_evas_native_tbm_surface_image_set)
          return glsym_evas_native_tbm_surface_image_set(NULL, im, ns);
        else
          return NULL;
     }
   else if (ns->type == EVAS_NATIVE_SURFACE_WL)
     {
       // TODO  : need the code for all wl_buffer type
       // First of all, for TBM surface
       if (glsym_wayland_tbm_server_get_surface && glsym_evas_native_tbm_surface_image_set)
         {
            tbm_surface_h _tbm_surface;

            _tbm_surface = glsym_wayland_tbm_server_get_surface(NULL,ns->data.wl.legacy_buffer);
            return glsym_evas_native_tbm_surface_image_set(_tbm_surface, im, ns);
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
   _evas_engine_way_shm_log_dom =
     eina_log_domain_register("evas-wayland_shm", EVAS_DEFAULT_LOG_COLOR);
   if (_evas_engine_way_shm_log_dom < 0)
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
   ORD(output_resize);
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
   if (_evas_engine_way_shm_log_dom > -1)
     eina_log_domain_unregister(_evas_engine_way_shm_log_dom);

   /* reset logging domain variable */
   _evas_engine_way_shm_log_dom = -1;
}

static Evas_Module_Api evas_modapi =
{
   EVAS_MODULE_API_VERSION, "wayland_shm", "none", {module_open, module_close}
};

EVAS_MODULE_DEFINE(EVAS_MODULE_TYPE_ENGINE, engine, wayland_shm);

#ifndef EVAS_STATIC_BUILD_WAYLAND_SHM
EVAS_EINA_MODULE_DEFINE(engine, wayland_shm);
#endif
