#include "evas_common_private.h"
#include "evas_engine.h"
#include "../gl_common/evas_gl_define.h"
#include "../software_generic/evas_native_common.h"

#ifdef HAVE_DLSYM
# include <dlfcn.h>
#endif

#ifdef EVAS_CSERVE2
# include "evas_cs2_private.h"
#endif

#define EVAS_GL_NO_GL_H_CHECK 1
#include "Evas_GL.h"

#define EVAS_GL_UPDATE_TILE_SIZE 16

#ifndef EGL_NATIVE_PIXMAP_KHR
# define EGL_NATIVE_PIXMAP_KHR 0x30b0
#endif

#ifndef EGL_Y_INVERTED_NOK
# define EGL_Y_INVERTED_NOK 0x307F
#endif

/* local structures */
typedef struct _Render_Engine Render_Engine;
struct _Render_Engine
{
   Render_Engine_GL_Generic generic;
};

/* Evas GL wl_surface & wl_egl_window */
typedef struct _Evgl_wl_Surface Evgl_wl_Surface;
struct _Evgl_wl_Surface
{
   struct wl_surface *wl_surf;
   struct wl_egl_window *egl_win;
};

/* local function prototypes */
typedef void (*_eng_fn) (void);
typedef _eng_fn (*glsym_func_eng_fn) ();
typedef void (*glsym_func_void) ();
typedef void *(*glsym_func_void_ptr) ();
typedef int (*glsym_func_int) ();
typedef unsigned int (*glsym_func_uint) ();
typedef const char *(*glsym_func_const_char_ptr) ();

Evas_GL_Common_Image_Call glsym_evas_gl_common_image_ref = NULL;
Evas_GL_Common_Image_Call glsym_evas_gl_common_image_unref = NULL;
Evas_GL_Common_Image_Call glsym_evas_gl_common_image_free = NULL;
Evas_GL_Common_Image_Call glsym_evas_gl_common_image_native_disable = NULL;
Evas_GL_Common_Image_Call glsym_evas_gl_common_image_native_enable = NULL;
Evas_GL_Common_Image_New_From_Data glsym_evas_gl_common_image_new_from_data = NULL;
Evas_GL_Common_Context_Call glsym_evas_gl_common_image_all_unload = NULL;
Evas_GL_Preload glsym_evas_gl_preload_init = NULL;
Evas_GL_Preload glsym_evas_gl_preload_shutdown = NULL;
EVGL_Engine_Call glsym_evgl_engine_shutdown = NULL;
EVGL_Native_Surface_Call glsym_evgl_native_surface_buffer_get = NULL;
EVGL_Native_Surface_Yinvert_Call glsym_evgl_native_surface_yinvert_get = NULL;
Evas_Gl_Symbols glsym_evas_gl_symbols = NULL;

Evas_GL_Common_Context_New glsym_evas_gl_common_context_new = NULL;
Evas_GL_Common_Context_Call glsym_evas_gl_common_context_flush = NULL;
Evas_GL_Common_Context_Call glsym_evas_gl_common_context_free = NULL;
Evas_GL_Common_Context_Call glsym_evas_gl_common_context_use = NULL;
Evas_GL_Common_Context_Call glsym_evas_gl_common_context_newframe = NULL;
Evas_GL_Common_Context_Call glsym_evas_gl_common_context_done = NULL;
Evas_GL_Common_Context_Resize_Call glsym_evas_gl_common_context_resize = NULL;
Evas_GL_Common_Buffer_Dump_Call glsym_evas_gl_common_buffer_dump = NULL;
Evas_GL_Preload_Render_Call glsym_evas_gl_preload_render_lock = NULL;
Evas_GL_Preload_Render_Call glsym_evas_gl_preload_render_unlock = NULL;
Evas_GL_Preload_Render_Call glsym_evas_gl_preload_render_relax = NULL;

glsym_func_void     glsym_evas_gl_common_error_set = NULL;
glsym_func_int      glsym_evas_gl_common_error_get = NULL;
glsym_func_void_ptr glsym_evas_gl_common_current_context_get = NULL;
// TIZEN_ONLY(20160425): Fix linking to 'context_restore_set'
//void (*glsym_evas_gl_context_restore_set) (Eina_Bool enable) = NULL;
//

_eng_fn (*glsym_eglGetProcAddress) (const char *a) = NULL;
void *(*glsym_eglCreateImage) (EGLDisplay a, EGLContext b, EGLenum c, EGLClientBuffer d, const int *e) = NULL;
void (*glsym_eglDestroyImage) (EGLDisplay a, void *b) = NULL;
void (*glsym_glEGLImageTargetTexture2DOES) (int a, void *b)  = NULL;
unsigned int (*glsym_eglSwapBuffersWithDamage) (EGLDisplay a, void *b, const EGLint *d, EGLint c) = NULL;
unsigned int (*glsym_eglSetDamageRegion) (EGLDisplay a, EGLSurface b, EGLint *c, EGLint d) = NULL;
unsigned int (*glsym_eglQueryWaylandBuffer)(EGLDisplay a, struct wl_resource *b, EGLint c, EGLint *d) = NULL;

void (*glsym_evas_gl_common_surface_cache_dump)(void) = NULL;

#define REPLACE_THREAD(prefix, dst, typ) \
   if (prefix##dst && (typ)prefix##dst != (typ)dst##_thread_cmd) \
     { \
        dst##_orig_evas_set(prefix##dst); \
        prefix##dst = (typ)dst##_thread_cmd; \
     }

//TIZEN_ONLY(20161121) : Support PreRotation
////////////////////////////////////
//libwayland-client.so.0
static void *wl_client_lib_handle = NULL;
wl_egl_win_get_capabilities glsym_wl_egl_win_get_capabilities = NULL;
wl_egl_win_set_rotation glsym_wl_egl_win_set_rotation = NULL;
//

/* local variables */
static Eina_Bool initted = EINA_FALSE;
static int gl_wins = 0;
static Evas_Func func, pfunc;

/* external variables */
int _evas_engine_wl_egl_log_dom = -1;
Eina_Bool extn_have_buffer_age = EINA_TRUE;
Eina_Bool extn_have_y_inverted = EINA_TRUE;
Eina_Bool prev_extn_have_buffer_age = EINA_TRUE;

/* local functions */
static inline Outbuf *
eng_get_ob(Render_Engine *re)
{
   return re->generic.software.ob;
}

static void
gl_symbols(void)
{
   static Eina_Bool done = EINA_FALSE;

   if (done) return;

   /* FIXME: Remove this line as soon as eglGetDisplay() autodetection
    * gets fixed. Currently it is incorrectly detecting wl_display and
    * returning _EGL_PLATFORM_X11 instead of _EGL_PLATFORM_WAYLAND.
    *
    * See ticket #1972 for more info.
    */
   setenv("EGL_PLATFORM", "wayland", 1);

#define LINK2GENERIC(sym) \
   glsym_##sym = dlsym(RTLD_DEFAULT, #sym);

   // Get function pointer to evas_gl_common now provided through GL_Generic.
   LINK2GENERIC(evas_gl_common_surface_cache_dump);
   LINK2GENERIC(evas_gl_common_image_all_unload);
   LINK2GENERIC(evas_gl_common_image_ref);
   LINK2GENERIC(evas_gl_common_image_unref);
   LINK2GENERIC(evas_gl_common_image_new_from_data);
   LINK2GENERIC(evas_gl_common_image_native_disable);
   LINK2GENERIC(evas_gl_common_image_free);
   LINK2GENERIC(evas_gl_common_image_native_enable);
   LINK2GENERIC(evas_gl_common_context_new);
   LINK2GENERIC(evas_gl_common_context_flush);
   LINK2GENERIC(evas_gl_common_context_free);
   LINK2GENERIC(evas_gl_common_context_use);
   LINK2GENERIC(evas_gl_common_context_newframe);
   LINK2GENERIC(evas_gl_common_context_done);
   LINK2GENERIC(evas_gl_common_context_resize);
   LINK2GENERIC(evas_gl_common_buffer_dump);
   LINK2GENERIC(evas_gl_preload_render_lock);
   LINK2GENERIC(evas_gl_preload_render_unlock);
   LINK2GENERIC(evas_gl_preload_render_relax);
   LINK2GENERIC(evas_gl_preload_init);
   LINK2GENERIC(evas_gl_preload_shutdown);
   LINK2GENERIC(evgl_native_surface_buffer_get);
   LINK2GENERIC(evgl_native_surface_yinvert_get);
   LINK2GENERIC(evgl_engine_shutdown);
   LINK2GENERIC(evas_gl_symbols);
   LINK2GENERIC(evas_gl_common_error_get);
   LINK2GENERIC(evas_gl_common_error_set);
   LINK2GENERIC(evas_gl_common_current_context_get);
// TIZEN_ONLY(20160425): Fix linking to 'context_restore_set'
// LINK2GENERIC(evas_gl_context_restore_set);
//

#define FINDSYM(dst, sym, typ) \
   if (glsym_eglGetProcAddress) { \
      if (!dst) dst = (typ)glsym_eglGetProcAddress(sym); \
   } else { \
      if (!dst) dst = (typ)dlsym(RTLD_DEFAULT, sym); \
   }

   FINDSYM(glsym_eglGetProcAddress, "eglGetProcAddressKHR", glsym_func_eng_fn);
   FINDSYM(glsym_eglGetProcAddress, "eglGetProcAddressEXT", glsym_func_eng_fn);
   FINDSYM(glsym_eglGetProcAddress, "eglGetProcAddressARB", glsym_func_eng_fn);
   FINDSYM(glsym_eglGetProcAddress, "eglGetProcAddress", glsym_func_eng_fn);
   REPLACE_THREAD(glsym_, eglGetProcAddress, glsym_func_eng_fn);

   glsym_evas_gl_symbols((void*)glsym_eglGetProcAddress);

   FINDSYM(glsym_eglCreateImage, "eglCreateImageKHR", glsym_func_void_ptr);
   FINDSYM(glsym_eglCreateImage, "eglCreateImageEXT", glsym_func_void_ptr);
   FINDSYM(glsym_eglCreateImage, "eglCreateImageARB", glsym_func_void_ptr);
   FINDSYM(glsym_eglCreateImage, "eglCreateImage", glsym_func_void_ptr);

   FINDSYM(glsym_eglDestroyImage, "eglDestroyImageKHR", glsym_func_void);
   FINDSYM(glsym_eglDestroyImage, "eglDestroyImageEXT", glsym_func_void);
   FINDSYM(glsym_eglDestroyImage, "eglDestroyImageARB", glsym_func_void);
   FINDSYM(glsym_eglDestroyImage, "eglDestroyImage", glsym_func_void);

   FINDSYM(glsym_glEGLImageTargetTexture2DOES, "glEGLImageTargetTexture2DOES",
           glsym_func_void);
   REPLACE_THREAD(glsym_, glEGLImageTargetTexture2DOES, glsym_func_void);

   FINDSYM(glsym_eglSwapBuffersWithDamage, "eglSwapBuffersWithDamageEXT",
           glsym_func_uint);
   FINDSYM(glsym_eglSwapBuffersWithDamage, "eglSwapBuffersWithDamageINTEL",
           glsym_func_uint);
   FINDSYM(glsym_eglSwapBuffersWithDamage, "eglSwapBuffersWithDamage",
           glsym_func_uint);
   REPLACE_THREAD(glsym_, eglSwapBuffersWithDamage, glsym_func_uint);

   FINDSYM(glsym_eglSetDamageRegion, "eglSetDamageRegionKHR",
           glsym_func_uint);
   REPLACE_THREAD(glsym_, eglSetDamageRegion, glsym_func_uint);

   FINDSYM(glsym_eglQueryWaylandBuffer, "eglQueryWaylandBufferWL",
           glsym_func_uint);
   REPLACE_THREAD(glsym_, eglQueryWaylandBuffer, glsym_func_uint);

   done = EINA_TRUE;
}

//TIZEN_ONLY(20161121) : Support PreRotation
static void
pre_rotation_symbols(void)
{
   static Eina_Bool init_pre_rotation_syms = EINA_FALSE;
   if (init_pre_rotation_syms) return;
   init_pre_rotation_syms = EINA_TRUE;

#ifdef GL_GLES
   const char *wayland_egl_lib = LIBDIR"/libwayland-egl.so";
   wl_client_lib_handle = dlopen(wayland_egl_lib, RTLD_NOW);
   if (!wl_client_lib_handle)
     {
        ERR("Unable to open libtbm: %s", dlerror());
        return;
     }

#define FIND_EGL_WL_SYM(dst, sym, typ) \
   if (!dst) dst = (typ)dlsym(wl_client_lib_handle, sym); \
   if (!dst)  \
     { \
        ERR("Symbol not found %s\n", sym); \
        return; \
     }

   FIND_EGL_WL_SYM(glsym_wl_egl_win_set_rotation, "wl_egl_window_set_rotation", glsym_func_void);
   FIND_EGL_WL_SYM(glsym_wl_egl_win_get_capabilities, "wl_egl_window_get_capabilities", glsym_func_int);

#undef FIND_EGL_WL_SYM
#endif
}

static void
gl_extn_veto(Render_Engine *re)
{
   const char *str = NULL;

   str = eglQueryString(eng_get_ob(re)->egl_disp, EGL_EXTENSIONS);
   if (str)
     {
        const char *s;
        if (getenv("EVAS_GL_INFO"))
          printf("EGL EXTN:\n%s\n", str);
        // Disable Partial Rendering
        if ((s = getenv("EVAS_GL_PARTIAL_DISABLE")) && atoi(s))
          {
             extn_have_buffer_age = EINA_FALSE;
             glsym_eglSwapBuffersWithDamage = NULL;
             glsym_eglSetDamageRegion = NULL;
          }
        if (!strstr(str, "EGL_EXT_buffer_age"))
          {
             if (!strstr(str, "EGL_KHR_partial_update"))
               extn_have_buffer_age = EINA_FALSE;
          }
        if (!strstr(str, "EGL_KHR_partial_update"))
          {
             glsym_eglSetDamageRegion = NULL;
          }
        if (!strstr(str, "EGL_NOK_texture_from_pixmap"))
          {
             extn_have_y_inverted = EINA_FALSE;
          }
        else
          {
             const GLubyte *vendor, *renderer;

             vendor = glGetString_thread_cmd(GL_VENDOR);
             renderer = glGetString_thread_cmd(GL_RENDERER);
             // XXX: workaround mesa bug!
             // looking for mesa and intel build which is known to
             // advertise the EGL_NOK_texture_from_pixmap extension
             // but not set it correctly. guessing vendor/renderer
             // strings will be like the following:
             // OpenGL vendor string: Intel Open Source Technology Center
             // OpenGL renderer string: Mesa DRI Intel(R) Sandybridge Desktop
             if (((vendor) && (strstr((const char *)vendor, "Intel"))) &&
                 ((renderer) && (strstr((const char *)renderer, "Mesa"))) &&
                 ((renderer) && (strstr((const char *)renderer, "Intel")))
                )
               extn_have_y_inverted = EINA_FALSE;
          }
        if (!strstr(str, "EGL_EXT_swap_buffers_with_damage"))
          {
             glsym_eglSwapBuffersWithDamage = NULL;
          }
        if (strstr(str, "EGL_TIZEN_image_native_surface"))
          {
             eng_get_ob(re)->gl_context->shared->info.egl_tbm_ext = 1;
          }
     }
   else
     {
        if (getenv("EVAS_GL_INFO"))
          printf("NO EGL EXTN!\n");
        extn_have_buffer_age = EINA_FALSE;
     }
}

static void
_re_winfree(Render_Engine *re)
{
   Outbuf *ob;

   if (!(ob = eng_get_ob(re))) return;
   if (!ob->surf) return;
   glsym_evas_gl_preload_render_relax(eng_preload_make_current, ob);
   eng_window_unsurf(ob);
}

static void
_evas_native_win_pre_rotation_set(struct wl_egl_window *win, int angle)
{
    evas_wl_egl_window_rotation rot;
    if (!win) return;
    if (!glsym_wl_egl_win_set_rotation)
      {
         ERR("Not supported PreRotation");
         return;
      }

    switch (angle)
      {
         case 270:
            rot = EVAS_ROTATION_90;
            break;
         case 180:
            rot = EVAS_ROTATION_180;
            break;
         case 90:
            rot = EVAS_ROTATION_270;
            break;
         case 0:
            rot = EVAS_ROTATION_0;
            break;
         default:
            rot = EVAS_ROTATION_0;
            break;
      }

    if (glsym_wl_egl_win_set_rotation)
      {
         glsym_wl_egl_win_set_rotation(win, rot);
      }
}

static void *
evgl_eng_display_get(void *data)
{
   Render_Engine *re;
   Outbuf *ob;

   if (!(re = (Render_Engine *)data)) return NULL;
   if (!(ob = eng_get_ob(re))) return NULL;
   return (void *)ob->egl_disp;
}

static void *
evgl_eng_evas_surface_get(void *data)
{
   Render_Engine *re;
   Outbuf *ob;

   if (!(re = (Render_Engine *)data)) return NULL;
   if (!(ob = eng_get_ob(re))) return NULL;
   return (void *)ob->egl_surface[0];
}

static void *
evgl_eng_native_window_create(void *data)
{
   Evgl_wl_Surface* surface;
   Render_Engine *re;
   Outbuf *ob;

   if (!(re = (Render_Engine *)data))
     {
        ERR("Invalid Render Engine Data!");
        glsym_evas_gl_common_error_set(data, EVAS_GL_NOT_INITIALIZED);
        return NULL;
     }
   if (!(ob = eng_get_ob(re))) return NULL;

   if (!(surface = calloc(1, sizeof(Evgl_wl_Surface))))
     return NULL;
   surface->wl_surf = wl_compositor_create_surface(ob->compositor);
   if (!surface->wl_surf)
     {
        ERR("Could not create wl_surface: %m");
        glsym_evas_gl_common_error_set(data, EVAS_GL_BAD_DISPLAY);
        return NULL;
     }
   surface->egl_win = wl_egl_window_create(surface->wl_surf, 1, 1);
   if (!surface->egl_win)
     {
        ERR("Could not create wl_egl window: %m");
        glsym_evas_gl_common_error_set(data, EVAS_GL_BAD_DISPLAY);
        return NULL;
     }

   return (void *)surface;
}

static int
evgl_eng_native_window_destroy(void *data, void *win)
{
   Evgl_wl_Surface* surface;

   if (!win)
     {
        ERR("Inavlid native window.");
        glsym_evas_gl_common_error_set(data, EVAS_GL_BAD_NATIVE_WINDOW);
        return 0;
     }

   surface = (Evgl_wl_Surface*)win;
   if (surface->egl_win)
     wl_egl_window_destroy((struct wl_egl_window *)surface->egl_win);
   if (surface->wl_surf)
      wl_surface_destroy(surface->wl_surf);

   free(surface);
   return 1;
}

static void *
evgl_eng_window_surface_create(void *data, void *win)
{
   Render_Engine *re;
   Outbuf *ob;
   EGLSurface surface = EGL_NO_SURFACE;
   Evgl_wl_Surface* evgl_surface;

   if (!(re = (Render_Engine *)data))
     {
        ERR("Invalid Render Engine Data!");
        glsym_evas_gl_common_error_set(data, EVAS_GL_NOT_INITIALIZED);
        return NULL;
     }
   if (!(ob = eng_get_ob(re))) return NULL;
   if (!(evgl_surface = (Evgl_wl_Surface *)win)) return NULL;
   if (!(evgl_surface->egl_win)) return NULL;

   surface = eglCreateWindowSurface(ob->egl_disp, ob->egl_config,
                                    (EGLNativeWindowType)evgl_surface->egl_win, NULL);
   if (!surface)
     {
        ERR("Could not create egl window surface: %#x", eglGetError());
        return NULL;
     }

   return (void *)surface;
}

static int
evgl_eng_window_surface_destroy(void *data, void *surface)
{
   Render_Engine *re;
   Outbuf *ob;

   if (!(re = (Render_Engine *)data))
     {
        ERR("Invalid Render Engine Data!");
        glsym_evas_gl_common_error_set(data, EVAS_GL_NOT_INITIALIZED);
        return 0;
     }
   if (!(ob = eng_get_ob(re))) return 0;

   if (!surface)
     {
        ERR("Invalid surface.");
        glsym_evas_gl_common_error_set(data, EVAS_GL_BAD_SURFACE);
        return 0;
     }
   eglDestroySurface(ob->egl_disp, (EGLSurface)surface);
   return 1;
}

static void *
evgl_eng_context_create(void *data, void *ctxt, Evas_GL_Context_Version version, int ctx_flag)
{
   Render_Engine *re;
   Outbuf *ob;
   EGLContext context = EGL_NO_CONTEXT;
   int attrs[5] = {0};

   if (!(re = (Render_Engine *)data))
     {
        ERR("Invalid Render Engine Data!");
        glsym_evas_gl_common_error_set(data, EVAS_GL_NOT_INITIALIZED);
        return NULL;
     }

   if (!(ob = eng_get_ob(re))) return NULL;

   if ((version < EVAS_GL_GLES_1_X) || (version > EVAS_GL_GLES_3_X))
     {
        ERR("Invalid context version number %d", version);
        glsym_evas_gl_common_error_set(data, EVAS_GL_BAD_PARAMETER);
        return NULL;
     }

   if ((version == EVAS_GL_GLES_3_X) &&
       ((!ob->gl_context) || (ob->gl_context->gles_version != EVAS_GL_GLES_3_X)))
     {
        EGLint num_config=0;
        EGLint version_config_attrs[3];
        version_config_attrs[0] = EGL_RENDERABLE_TYPE;
        version_config_attrs[1] = EGL_OPENGL_ES3_BIT;
        version_config_attrs[2] = EGL_NONE;

        if ((!eglChooseConfig(ob->egl_disp, version_config_attrs, NULL, 0, &num_config))
           || (num_config < 1))
          {
             ERR("GLES 3 version not supported!");
             glsym_evas_gl_common_error_set(data, EVAS_GL_BAD_ATTRIBUTE);
             return NULL;
          }
        else if (ob->gl_context)
          {
             ob->gl_context->gles_version = EVAS_GL_GLES_3_X;
          }
        else
          {
             ERR("Evas GL Context not initialised!");
             glsym_evas_gl_common_error_set(data, EVAS_GL_NOT_INITIALIZED);
             return NULL;
          }
     }

   attrs[0] = EGL_CONTEXT_CLIENT_VERSION;
   attrs[1] = version;

   if (ctx_flag & EVAS_GL_DEBUG)
     {
        attrs[2] = EGL_CONTEXT_FLAGS_KHR;
        attrs[3] |= EGL_CONTEXT_OPENGL_DEBUG_BIT_KHR;
        attrs[4] = EGL_NONE;
     }
   else
     attrs[2] = EGL_NONE;

   if (ctxt)
     {
        context =
          eglCreateContext(ob->egl_disp, ob->egl_config,
                           (EGLContext)ctxt, attrs);
     }

   else if (version == EVAS_GL_GLES_1_X || version == EVAS_GL_GLES_3_X)
     {
        context =
          eglCreateContext(ob->egl_disp, ob->egl_config,
                           NULL, attrs);
     }
   else
     {
        context =
          eglCreateContext(ob->egl_disp, ob->egl_config,
                           ob->egl_context[0], attrs);
     }

   if (!context)
     {
        int err = eglGetError();
        ERR("Failed to create egl context: %#x", err);
        glsym_evas_gl_common_error_set(data, err - EGL_SUCCESS);
        return NULL;
     }

   return (void *)context;
}

static int
evgl_eng_context_destroy(void *data, void *ctxt)
{
   Render_Engine *re;
   Outbuf *ob;

   if (!(re = (Render_Engine *)data))
     {
        ERR("Invalid Render Input Data. Engine: %p", data);
        glsym_evas_gl_common_error_set(data, EVAS_GL_NOT_INITIALIZED);
        return 0;
     }
   if (!(ob = eng_get_ob(re))) return 0;
   if (!ctxt)
     {
        ERR("Invalid Render Input Data. Context: %p", ctxt);
        glsym_evas_gl_common_error_set(data, EVAS_GL_BAD_CONTEXT);
        return 0;
     }

   eglDestroyContext(ob->egl_disp, (EGLContext)ctxt);
   return 1;
}

static int
evgl_eng_make_current(void *data, void *surface, void *ctxt, int flush)
{
   Render_Engine *re;
   Outbuf *ob;
   EGLContext ctx;
   EGLSurface surf;
   int ret = 0;

   if (!(re = (Render_Engine *)data))
     {
        ERR("Invalid Render Engine Data!");
        glsym_evas_gl_common_error_set(data, EVAS_GL_NOT_INITIALIZED);
        return 0;
     }
   if (!(ob = eng_get_ob(re))) return 0;

   ctx = (EGLContext)ctxt;
   surf = (EGLSurface)surface;

   if ((!ctxt) && (!surface))
     {
        ret =
          eglMakeCurrent_thread_cmd(ob->egl_disp, EGL_NO_SURFACE,
                         EGL_NO_SURFACE, EGL_NO_CONTEXT);
        if (!ret)
          {
             int err = eglGetError_thread_cmd();
             glsym_evas_gl_common_error_set(err - EGL_SUCCESS);
             ERR("eglMakeCurrent() failed! Error Code=%#x", err);
             return 0;
          }
        return 1;
     }

   if ((eglGetCurrentContext_thread_cmd() != ctx) ||
       (eglGetCurrentSurface_thread_cmd(EGL_READ) != surf) ||
       (eglGetCurrentSurface_thread_cmd(EGL_DRAW) != surf))
     {
        if (flush) eng_window_use(NULL);

        ret = eglMakeCurrent_thread_cmd(ob->egl_disp, surf, surf, ctx);
        if (!ret)
          {
             int err = eglGetError_thread_cmd();
             glsym_evas_gl_common_error_set(err - EGL_SUCCESS);
             ERR("eglMakeCurrent() failed! Error Code=%#x", err);
             return 0;
          }
     }
   return 1;
}

static void *
evgl_eng_proc_address_get(const char *name)
{
   if (glsym_eglGetProcAddress) return glsym_eglGetProcAddress(name);
   return dlsym(RTLD_DEFAULT, name);
}

static const char *
evgl_eng_string_get(void *data)
{
   Render_Engine *re;
   Outbuf *ob;

   if (!(re = (Render_Engine *)data))
     {
        ERR("Invalid Render Engine Data!");
        glsym_evas_gl_common_error_set(data, EVAS_GL_NOT_INITIALIZED);
        return NULL;
     }
   if (!(ob = eng_get_ob(re))) return NULL;

   return eglQueryString(ob->egl_disp, EGL_EXTENSIONS);
}

static int
evgl_eng_rotation_angle_get(void *data)
{
   Render_Engine *re;
   Outbuf *ob;

   if (!(re = (Render_Engine *)data))
     {
        ERR("Invalid Render Engine Data!");
        glsym_evas_gl_common_error_set(data, EVAS_GL_NOT_INITIALIZED);
        return 0;
     }
   if (!(ob = eng_get_ob(re))) return 0;

   if (ob->gl_context)
     return ob->gl_context->rot;
   else
     {
        ERR("Unable to retrieve rotation angle.");
        glsym_evas_gl_common_error_set(data, EVAS_GL_BAD_CONTEXT);
        return 0;
     }
}

static void *
evgl_eng_pbuffer_surface_create(void *data, EVGL_Surface *sfc,
                                const int *attrib_list)
{
   Render_Engine_GL_Generic *re = data;

   // TODO: Add support for surfaceless pbuffers (EGL_NO_TEXTURE)
   // TODO: Add support for EGL_MIPMAP_TEXTURE??? (GLX doesn't support them)

   if (attrib_list)
     WRN("This PBuffer implementation does not support extra attributes yet");

#ifdef GL_GLES
   Evas_Engine_GL_Context *evasglctx;
   int config_attrs[20];
   int surface_attrs[20];
   EGLSurface egl_sfc;
   EGLConfig egl_cfg;
   int num_config, i = 0;
   EGLDisplay disp;
   EGLContext ctx;

   disp = re->window_egl_display_get(re->software.ob);
   evasglctx = re->window_gl_context_get(re->software.ob);
   ctx = evasglctx->eglctxt;

   // It looks like eglMakeCurrent might fail if we use a different config from
   // the actual display surface. This is weird.
   i = 0;
   config_attrs[i++] = EGL_CONFIG_ID;
   config_attrs[i++] = 0;
   config_attrs[i++] = EGL_NONE;
   eglQueryContext(disp, ctx, EGL_CONFIG_ID, &config_attrs[1]);

   num_config =0;
   if (!eglChooseConfig(disp, config_attrs, &egl_cfg, 1, &num_config)
       || (num_config < 1))
     {
        int err = eglGetError();
        glsym_evas_gl_common_error_set(data, err - EGL_SUCCESS);
        ERR("eglChooseConfig failed with error %x", err);
        return NULL;
     }

   // Now, choose the config for the PBuffer
   i = 0;
   surface_attrs[i++] = EGL_WIDTH;
   surface_attrs[i++] = sfc->w;
   surface_attrs[i++] = EGL_HEIGHT;
   surface_attrs[i++] = sfc->h;

   surface_attrs[i++] = EGL_NONE;

   egl_sfc = eglCreatePbufferSurface(disp, egl_cfg, surface_attrs);
   if (!egl_sfc)
     {
        int err = eglGetError();
        glsym_evas_gl_common_error_set(data, err - EGL_SUCCESS);
        ERR("eglCreatePbufferSurface failed with error %x", err);
        return NULL;
     }

   return egl_sfc;
#else
   GLXPbuffer pbuf;
   GLXFBConfig *cfgs;
   int config_attrs[20];
   int surface_attrs[20];
   int ncfg = 0, i;

   // TODO: Check all required config attributes
   // TODO: Should be tested when EFL runs with wayland on Desktop

#ifndef GLX_VISUAL_ID
# define GLX_VISUAL_ID 0x800b
#endif

   i = 0;
   if (sfc->pbuffer.color_fmt != EVAS_GL_NO_FBO)
     {
        config_attrs[i++] = GLX_BUFFER_SIZE;
        if (sfc->pbuffer.color_fmt == EVAS_GL_RGBA_8888)
          {
             config_attrs[i++] = 32;
             //config_attrs[i++] = GLX_BIND_TO_TEXTURE_RGBA_EXT;
             //config_attrs[i++] = 1;
          }
        else
          {
             config_attrs[i++] = 24;
             //config_attrs[i++] = GLX_BIND_TO_TEXTURE_RGB_EXT;
             //config_attrs[i++] = 1;
          }
     }
   if (sfc->depth_fmt)
     {
        config_attrs[i++] = GLX_DEPTH_SIZE;
        config_attrs[i++] = 24; // FIXME: This should depend on the requested bits
     }
   if (sfc->stencil_fmt)
     {
        config_attrs[i++] = GLX_STENCIL_SIZE;
        config_attrs[i++] = 8; // FIXME: This should depend on the requested bits
     }
   //config_attrs[i++] = GLX_VISUAL_ID;
   //config_attrs[i++] = XVisualIDFromVisual(vis);
   config_attrs[i++] = 0;

   cfgs = glXChooseFBConfig(re->software.ob->disp, re->software.ob->screen,
                            config_attrs, &ncfg);
   if (!cfgs || !ncfg)
     {
        ERR("GLX failed to find a valid config for the pbuffer");
        if (cfgs) XFree(cfgs);
        return NULL;
     }

   i = 0;
   surface_attrs[i++] = GLX_LARGEST_PBUFFER;
   surface_attrs[i++] = 0;
   surface_attrs[i++] = GLX_PBUFFER_WIDTH;
   surface_attrs[i++] = sfc->w;
   surface_attrs[i++] = GLX_PBUFFER_HEIGHT;
   surface_attrs[i++] = sfc->h;
   surface_attrs[i++] = 0;
   pbuf = glXCreatePbuffer(re->software.ob->disp, cfgs[0], surface_attrs);
   if (cfgs) XFree(cfgs);

   if (!pbuf)
     {
        ERR("GLX failed to create a pbuffer");
        return NULL;
     }

   return (void*)(intptr_t)pbuf;
#endif
}

static int
evgl_eng_pbuffer_surface_destroy(void *data, void *surface)
{
   /* EVGLINIT(re, 0); */
   if (!data)
     {
        ERR("Invalid Render Engine Data!");
        glsym_evas_gl_common_error_set(NULL, EVAS_GL_NOT_INITIALIZED);
        return 0;
     }

   if (!surface)
     {
        ERR("Invalid surface.");
        glsym_evas_gl_common_error_set(data, EVAS_GL_BAD_SURFACE);
        return 0;
     }

#ifdef GL_GLES
   Render_Engine *re = data;

   eglDestroySurface(eng_get_ob(re)->egl_disp, (EGLSurface)surface);
#else
   // TODO: Should be tested when EFL runs with wayland on Desktop
   Render_Engine_GL_Generic *re = data;
   GLXPbuffer pbuf = (GLXPbuffer)(intptr_t) surface;

   glXDestroyPbuffer(re->software.ob->disp, pbuf);
#endif

   return 1;
}

static void
evgl_eng_native_win_surface_config_get(void *data, int *win_depth,
                                         int *win_stencil, int *win_msaa)
{
   Render_Engine *re = data;
   if (!re) return;

   if (win_depth)
     *win_depth = eng_get_ob(re)->detected.depth_buffer_size;
   if (win_stencil)
     *win_stencil = eng_get_ob(re)->detected.stencil_buffer_size;
   if (win_msaa)
     *win_msaa = eng_get_ob(re)->detected.msaa;

   DBG("Window config(depth %d, stencil %d, msaa %d)",
       eng_get_ob(re)->detected.depth_buffer_size,
       eng_get_ob(re)->detected.stencil_buffer_size,
       eng_get_ob(re)->detected.msaa);
}

static int
evgl_eng_native_win_prerotation_set(void *data)
{
   Render_Engine *re = data;
   if (!re) return 0;
   if (!(eng_get_ob(re)->support_pre_rotation)) return 0;
   if (eng_get_ob(re)->gl_context->pre_rotated) return 0;

   _evas_native_win_pre_rotation_set(eng_get_ob(re)->info->info.win, eng_get_ob(re)->info->info.rotation);

   // re->win's h & w are not modified
   eng_get_ob(re)->rot = 0;

   /* There maybe bad frame due to mismatch of surface and
    * window size if orientation changes in the middle of
    * rendering pipeline, therefore recreate the surface.
    */
   eng_get_ob(re)->gl_context->pre_rotated = EINA_TRUE;

   return 1;
}

static void
evgl_eng_partial_rendering_enable()
{
   extn_have_buffer_age = prev_extn_have_buffer_age;
   prev_extn_have_buffer_age = EINA_FALSE;
}

// TIZEN_ONLY
static void
evgl_eng_partial_rendering_disable()
{
   prev_extn_have_buffer_age = extn_have_buffer_age;
   extn_have_buffer_age = EINA_FALSE;
}


static const EVGL_Interface evgl_funcs =
{
   evgl_eng_display_get,
   evgl_eng_evas_surface_get,
   evgl_eng_native_window_create,
   evgl_eng_native_window_destroy,
   evgl_eng_window_surface_create,
   evgl_eng_window_surface_destroy,
   evgl_eng_context_create,
   evgl_eng_context_destroy,
   evgl_eng_make_current,
   evgl_eng_proc_address_get,
   evgl_eng_string_get,
   evgl_eng_rotation_angle_get,
   evgl_eng_pbuffer_surface_create,
   evgl_eng_pbuffer_surface_destroy,
   NULL, // OpenGL-ES 1
   NULL, // OpenGL-ES 1
   NULL, // OpenGL-ES 1
   evgl_eng_native_win_surface_config_get,
   evgl_eng_native_win_prerotation_set,
   evgl_eng_partial_rendering_enable,
   evgl_eng_partial_rendering_disable,
};

/* engine functions */
static void *
eng_info(Evas *evas EINA_UNUSED)
{
   Evas_Engine_Info_Wayland_Egl *info;

   /* try to allocate space for our engine info */
   if (!(info = calloc(1, sizeof(Evas_Engine_Info_Wayland_Egl))))
     return NULL;

   info->magic.magic = rand();
   info->render_mode = EVAS_RENDER_MODE_BLOCKING;

   return info;
}

static void
eng_info_free(Evas *evas EINA_UNUSED, void *info)
{
   Evas_Engine_Info_Wayland_Egl *inf;

   if ((inf = (Evas_Engine_Info_Wayland_Egl *)info))
     free(inf);
}

static int
eng_setup(Evas *evas, void *info)
{
   Render_Engine_Swap_Mode swap_mode = MODE_FULL;
   Evas_Engine_Info_Wayland_Egl *inf;
   Evas_Public_Data *epd;
   Render_Engine *re;
   Outbuf *ob;
   const char *s;

   inf = (Evas_Engine_Info_Wayland_Egl *)info;
   epd = eo_data_scope_get(evas, EVAS_CANVAS_CLASS);

   if ((s = getenv("EVAS_GL_SWAP_MODE")))
     {
        if ((!strcasecmp(s, "full")) ||
            (!strcasecmp(s, "f")))
          swap_mode = MODE_FULL;
        else if ((!strcasecmp(s, "copy")) ||
                 (!strcasecmp(s, "c")))
          swap_mode = MODE_COPY;
        else if ((!strcasecmp(s, "double")) ||
                 (!strcasecmp(s, "d")) ||
                 (!strcasecmp(s, "2")))
          swap_mode = MODE_DOUBLE;
        else if ((!strcasecmp(s, "triple")) ||
                 (!strcasecmp(s, "t")) ||
                 (!strcasecmp(s, "3")))
          swap_mode = MODE_TRIPLE;
        else if ((!strcasecmp(s, "quadruple")) ||
                 (!strcasecmp(s, "q")) ||
                 (!strcasecmp(s, "4")))
          swap_mode = MODE_QUADRUPLE;
     }
   else
     {
// in most gl implementations - egl and glx here that we care about the TEND
// to either swap or copy backbuffer and front buffer, but strictly that is
// not true. technically backbuffer content is totally undefined after a swap
// and thus you MUST re-render all of it, thus MODE_FULL
        swap_mode = MODE_FULL;
// BUT... reality is that lmost every implementation copies or swaps so
// triple buffer mode can be used as it is a superset of double buffer and
// copy (though using those explicitly is more efficient). so let's play with
// triple buffer mdoe as a default and see.
//        re->mode = MODE_TRIPLE;
// XXX: note - the above seems to break on some older intel chipsets and
// drivers. it seems we CANT depend on backbuffer staying around. bugger!
        switch (inf->swap_mode)
          {
           case EVAS_ENGINE_WAYLAND_EGL_SWAP_MODE_FULL:
             swap_mode = MODE_FULL;
             break;
           case EVAS_ENGINE_WAYLAND_EGL_SWAP_MODE_COPY:
             swap_mode = MODE_COPY;
             break;
           case EVAS_ENGINE_WAYLAND_EGL_SWAP_MODE_DOUBLE:
             swap_mode = MODE_DOUBLE;
             break;
           case EVAS_ENGINE_WAYLAND_EGL_SWAP_MODE_TRIPLE:
             swap_mode = MODE_TRIPLE;
             break;
           case EVAS_ENGINE_WAYLAND_EGL_SWAP_MODE_QUADRUPLE:
             swap_mode = MODE_QUADRUPLE;
             break;
           default:
             swap_mode = MODE_AUTO;
             break;
          }
     }

   if (!(re = epd->engine.data.output))
     {
        Render_Engine_Merge_Mode merge = MERGE_BOUNDING;

        /* FIXME: Remove this line as soon as eglGetDisplay() autodetection
         * gets fixed. Currently it is incorrectly detecting wl_display and
         * returning _EGL_PLATFORM_X11 instead of _EGL_PLATFORM_WAYLAND.
         *
         * See ticket #1972 for more info.
         */
        setenv("EGL_PLATFORM", "wayland", 1);

        /* try to allocate space for a new render engine */
        if (!(re = calloc(1, sizeof(Render_Engine))))
          return 0;

        /* if we have not initialize gl & evas, do it */
        if (!initted)
          {
             evas_common_init();
             glsym_evas_gl_preload_init();
          }

        ob = eng_window_new(evas, inf, epd->output.w, epd->output.h, swap_mode,
                            inf->depth_bits, inf->stencil_bits, inf->msaa_bits);
        if (!ob) goto ob_err;

        if (!evas_render_engine_gl_generic_init(&re->generic, ob,
                                                eng_outbuf_swap_mode_get,
                                                eng_outbuf_rotation_get,
                                                eng_outbuf_reconfigure,
                                                eng_outbuf_region_first_rect,
                                                eng_outbuf_update_region_new,
                                                eng_outbuf_update_region_push,
                                                eng_outbuf_update_region_free,
                                                NULL,
                                                eng_outbuf_flush,
                                                eng_window_free,
                                                eng_window_use,
                                                eng_outbuf_gl_context_get,
                                                eng_outbuf_egl_display_get,
                                                eng_gl_context_new,
                                                eng_gl_context_use,
                                                &evgl_funcs,
                                                epd->output.w, epd->output.h))
          {
             eng_window_free(ob);
             goto ob_err;
          }

        epd->engine.data.output = re;
        gl_wins++;

        if ((s = getenv("EVAS_GL_PARTIAL_MERGE")))
          {
             if ((!strcmp(s, "bounding")) || (!strcmp(s, "b")))
               merge = MERGE_BOUNDING;
             else if ((!strcmp(s, "full")) || (!strcmp(s, "f")))
               merge = MERGE_FULL;
             else if ((!strcmp(s, "smart")) || (!strcmp(s, "s")))
               merge = MERGE_SMART;
          }

        evas_render_engine_software_generic_merge_mode_set(&re->generic.software, merge);

        if (!initted)
          {
             gl_extn_veto(re);
             initted = EINA_TRUE;
          }
     }
   else
     {
        re = epd->engine.data.output;
        ob = eng_get_ob(re);
        if ((ob) && (_re_wincheck(ob)))
          {
             ob->info = inf;
             if ((ob->info->info.display != ob->disp) ||
                 (ob->info->info.surface != ob->surface) ||
                 /* FIXME: comment out below line.
                  * since there is no place set the info->info.win for now,
                  * it causes renew the window unnecessarily.
                  */
                 /* (ob->info->info.win != ob->win) || */
                 (ob->info->info.depth != ob->depth) ||
                 (ob->info->info.screen != ob->screen) ||
                 (ob->info->info.destination_alpha != ob->alpha))
               {
                  ob->gl_context->references++;
                  gl_wins--;

                  ob = eng_window_new(evas, inf, epd->output.w, epd->output.h, swap_mode,
                                      inf->depth_bits, inf->stencil_bits, inf->msaa_bits);
                  if (!ob) goto ob_err;

                  eng_window_use(ob);

                  evas_render_engine_software_generic_update(&re->generic.software, ob,
                                                             epd->output.w, epd->output.h);
                  gl_wins++;
                  eng_get_ob(re)->gl_context->references--;
               }
             else if ((ob->w != epd->output.w) || (ob->h != epd->output.h) ||
                      (ob->info->info.rotation != ob->rot) ||
                      (ob->gl_context->pre_rotated))
               {
                 //TIZEN_ONLY(20161121) : Support PreRotation
                 if (ob->support_pre_rotation && ob->gl_context->pre_rotated)
                   {
                      ob->gl_context->pre_rotated = EINA_FALSE;
                      evgl_eng_native_win_prerotation_set(re);
                   }

                 eng_outbuf_reconfigure(ob, epd->output.w, epd->output.h,
                                         ob->info->info.rotation, 0);
               }
          }
     }

   if (!eng_get_ob(re)) goto ob_err;

   if (!epd->engine.data.output)
     {
        if (eng_get_ob(re))
          {
             eng_window_free(eng_get_ob(re));
             gl_wins--;
          }
        goto ob_err;
     }

   if (re->generic.software.tb)
     evas_common_tilebuf_free(re->generic.software.tb);
   re->generic.software.tb =
     evas_common_tilebuf_new(epd->output.w, epd->output.h);

   if (re->generic.software.tb)
     {
        evas_common_tilebuf_set_tile_size(re->generic.software.tb,
                                          TILESIZE, TILESIZE);
        evas_render_engine_software_generic_tile_strict_set
          (&re->generic.software, EINA_TRUE);
     }

   if (!epd->engine.data.context)
     {
        epd->engine.data.context =
          epd->engine.func->context_new(epd->engine.data.output);
     }

   eng_window_use(eng_get_ob(re));

   return 1;

ob_err:
   free(re);
   return 0;
}

static Eina_Bool
eng_canvas_alpha_get(void *data, void *info EINA_UNUSED)
{
   Render_Engine *re;

   if ((re = (Render_Engine *)data))
     return re->generic.software.ob->alpha;

   return EINA_FALSE;
}

static void
eng_output_free(void *data)
{
   Render_Engine *re;

   if ((re = (Render_Engine *)data))
     {
        glsym_evas_gl_preload_render_relax(eng_preload_make_current, eng_get_ob(re));

        if (gl_wins == 1) glsym_evgl_engine_shutdown(re);

        evas_render_engine_software_generic_clean(&re->generic.software);

        gl_wins--;

        free(re);
     }

   if ((initted == EINA_TRUE) && (gl_wins == 0))
     {
        glsym_evas_gl_preload_shutdown();
        evas_common_shutdown();
        initted = EINA_FALSE;
     }
}

static void
eng_output_dump(void *data)
{
   Render_Engine *re;

   if (!(re = (Render_Engine *)data)) return;
   glsym_evas_gl_common_surface_cache_dump();
   evas_common_image_image_all_unload();
   evas_common_font_font_all_unload();
   glsym_evas_gl_common_image_all_unload(eng_get_ob(re)->gl_context);
   _re_winfree(re);
}

static void *
eng_gl_current_context_get(void *data EINA_UNUSED)
{
   EVGL_Context *ctx;

   ctx = glsym_evas_gl_common_current_context_get();
   if (!ctx)
     return NULL;

#ifdef GL_GLES
   if (eglGetCurrentContext_thread_cmd() == (ctx->context))
     return ctx;
   else
     return NULL;
#else
   if (glXGetCurrentContext() == (ctx->context))
     return ctx;
   else
     return NULL;
#endif
}

static int
eng_gl_error_get(void *data)
{
   int err;

   if ((err = glsym_evas_gl_common_error_get(data)) != EVAS_GL_SUCCESS)
     goto end;

   err = eglGetError() - EGL_SUCCESS;

end:
   glsym_evas_gl_common_error_set(data, EVAS_GL_SUCCESS);
   return err;
}

static void
_native_cb_bind(void *data EINA_UNUSED, void *image)
{
   Evas_GL_Image *img;
   Native *n;

   if (!(img = image)) return;
   if (!(n = img->native.data)) return;

   if (n->ns.type == EVAS_NATIVE_SURFACE_WL)
     {
        if (n->ns_data.wl_surface.surface)
          {
             if (glsym_glEGLImageTargetTexture2DOES)
               {
                  glsym_glEGLImageTargetTexture2DOES(img->native.target, n->ns_data.wl_surface.surface);
                  GLERRV("glsym_glEGLImageTargetTexture2DOES");
               }
             else
               ERR("Try glEGLImageTargetTexture2DOES on EGL with no support");
          }
     }
   else if (n->ns.type == EVAS_NATIVE_SURFACE_OPENGL)
     {
        glBindTexture_thread_cmd(GL_TEXTURE_2D, n->ns.data.opengl.texture_id);
     }
   else if (n->ns.type == EVAS_NATIVE_SURFACE_EVASGL)
     {
        if (n->ns_data.evasgl.surface)
          {
             Eina_Bool is_egl_image = EINA_FALSE;
             void *surface = NULL;

             if (glsym_evgl_native_surface_buffer_get)
               surface = glsym_evgl_native_surface_buffer_get(n->ns_data.evasgl.surface, &is_egl_image);

             if (is_egl_image)
               {
                  if (glsym_glEGLImageTargetTexture2DOES)
                    {
                       glsym_glEGLImageTargetTexture2DOES(GL_TEXTURE_2D, surface);
                       if (eglGetError() != EGL_SUCCESS)
                         ERR("glEGLImageTargetTexture2DOES() failed.");
                    }
                  else
                    ERR("Try glEGLImageTargetTexture2DOES on EGL with no support");
               }
             else
               {
                  glBindTexture_thread_cmd(GL_TEXTURE_2D, (GLuint)(uintptr_t)surface);
               }
          }
    }
  else if (n->ns.type == EVAS_NATIVE_SURFACE_TBM)
    {
#ifdef GL_GLES
      if (n->ns_data.tbm.surface)
        {
           if (glsym_glEGLImageTargetTexture2DOES)
              {
                glsym_glEGLImageTargetTexture2DOES(GL_TEXTURE_EXTERNAL_OES, n->ns_data.tbm.surface);
                if (eglGetError() != EGL_SUCCESS)
                  ERR("glEGLImageTargetTexture2DOES() failed.");
              }
            else
              ERR("Try glEGLImageTargetTexture2DOES on EGL with no support");
        }
#endif
    }
}

static void
_native_cb_unbind(void *data EINA_UNUSED, void *image)
{
   Evas_GL_Image *img;
   Native *n;

   if (!(img = image)) return;
   if (!(n = img->native.data)) return;

   if (n->ns.type == EVAS_NATIVE_SURFACE_WL)
     {
        //glBindTexture(GL_TEXTURE_2D, 0); //really need?
     }
   else if (n->ns.type == EVAS_NATIVE_SURFACE_OPENGL)
     {
        glBindTexture_thread_cmd(GL_TEXTURE_2D, 0);
     }
  else if (n->ns.type == EVAS_NATIVE_SURFACE_EVASGL)
    {
      // nothing
    }
  else if (n->ns.type == EVAS_NATIVE_SURFACE_TBM)
    {
      // nothing
    }
}

static void
_native_cb_free(void *data, void *image)
{
   Render_Engine *re;
   Outbuf *ob;
   Evas_GL_Image *img;
   Native *n;
   uint32_t texid;
   void *wlid;

   if (!(re = (Render_Engine *)data)) return;
   if (!(img = image)) return;
   if (!(n = img->native.data)) return;
   if (!(ob = eng_get_ob(re))) return;

   if (n->ns.type == EVAS_NATIVE_SURFACE_WL)
     {
        wlid = (void*)n->ns_data.wl_surface.wl_buf;
        eina_hash_del(ob->gl_context->shared->native_wl_hash, &wlid, img);
        if (n->ns_data.wl_surface.surface)
          {
             int err;
             if (glsym_eglDestroyImage)
               {
                  glsym_eglDestroyImage(ob->egl_disp, n->ns_data.wl_surface.surface);
                  if ((err = eglGetError()) != EGL_SUCCESS)
                    {
                       ERR("eglDestroyImage() failed.");
                       glsym_evas_gl_common_error_set(err - EGL_SUCCESS);
                    }
               }
             else
               ERR("Try eglDestroyImage on EGL with  no support");
          }
     }
   else if (n->ns.type == EVAS_NATIVE_SURFACE_OPENGL)
     {
        texid = n->ns.data.opengl.texture_id;
        eina_hash_del(ob->gl_context->shared->native_tex_hash, &texid, img);
     }
  else if (n->ns.type == EVAS_NATIVE_SURFACE_EVASGL)
    {
      eina_hash_del(eng_get_ob(re)->gl_context->shared->native_evasgl_hash, &n->ns_data.evasgl.surface, img);
    }
  else if (n->ns.type == EVAS_NATIVE_SURFACE_TBM)
    {
       eina_hash_del(ob->gl_context->shared->native_tbm_hash, &n->ns_data.tbm.buffer, img);
#ifdef GL_GLES
      if (n->ns_data.tbm.surface)
        {
           int err;
           if (glsym_eglDestroyImage)
             {
                glsym_eglDestroyImage(ob->egl_disp,
                                      n->ns_data.tbm.surface);
                if ((err = eglGetError()) != EGL_SUCCESS)
                  {
                     ERR("eglDestroyImage() failed.");
                  }
             }
           else
              ERR("Try eglDestroyImage on EGL with no support");
        }
#endif
    }
   img->native.data = NULL;
   img->native.func.data = NULL;
   img->native.func.bind = NULL;
   img->native.func.unbind = NULL;
   img->native.func.free = NULL;

   free(n);
}

static int
_native_cb_yinvert(void *data EINA_UNUSED, void *image)
{
   Evas_GL_Image *im = image;
   Native *n = im->native.data;
   int yinvert = 0;

   // Yinvert callback should only be used for EVAS_NATIVE_SURFACE_EVASGL type now,
   // as yinvert value is not changed for other types.
   if (n->ns.type == EVAS_NATIVE_SURFACE_WL)
     {
     }
   else if (n->ns.type == EVAS_NATIVE_SURFACE_OPENGL)
     {
        yinvert = 0;
     }
   else if (n->ns.type == EVAS_NATIVE_SURFACE_TBM)
     {
        yinvert = 1;
     }
   else if (n->ns.type == EVAS_NATIVE_SURFACE_EVASGL)
     {
        if (glsym_evgl_native_surface_yinvert_get)
          yinvert = glsym_evgl_native_surface_yinvert_get(n->ns_data.evasgl.surface);
     }

   return yinvert;
}

static void *
eng_image_native_set(void *data, void *image, void *native)
{
   Render_Engine *re;
   Outbuf *ob;
   Native *n;
   Evas_Native_Surface *ns;
   Evas_GL_Image *img, *img2;
   unsigned int tex = 0, fbo = 0;
   uint32_t texid;
   void *buffer = NULL;
   void *wlid, *wl_buf = NULL;

   if (!(re = (Render_Engine *)data)) return NULL;
   if (!(ob = eng_get_ob(re))) return NULL;

   ns = native;

   if (!(img = image))
     {
        if ((ns) && (ns->type == EVAS_NATIVE_SURFACE_OPENGL))
          {
             img =
               glsym_evas_gl_common_image_new_from_data(ob->gl_context,
                                                        ns->data.opengl.w,
                                                        ns->data.opengl.h,
                                                        NULL, 1,
                                                        EVAS_COLORSPACE_ARGB8888);
          }
        else
          return NULL;
     }

   if (ns)
     {
        if (ns->type == EVAS_NATIVE_SURFACE_WL)
          {
             wl_buf = ns->data.wl.legacy_buffer;
             if (img->native.data)
               {
                  Evas_Native_Surface *ens;

                  ens = img->native.data;
                  if (ens->data.wl.legacy_buffer == wl_buf)
                    return img;
               }
          }
        else if (ns->type == EVAS_NATIVE_SURFACE_OPENGL)
          {
             tex = ns->data.opengl.texture_id;
             fbo = ns->data.opengl.framebuffer_id;
             if (img->native.data)
               {
                  Evas_Native_Surface *ens;

                  ens = img->native.data;
                  if ((ens->data.opengl.texture_id == tex) &&
                      (ens->data.opengl.framebuffer_id == fbo))
                     return img;
               }
          }
        else if (ns->type == EVAS_NATIVE_SURFACE_EVASGL)
         {
            buffer = ns->data.evasgl.surface;
            if (img->native.data)
              {
                 Evas_Native_Surface *ens = img->native.data;
                 if (ens->data.evasgl.surface == buffer)
                   return img;
              }
         }
        else if (ns->type == EVAS_NATIVE_SURFACE_TBM)
         {
            buffer = ns->data.tbm.buffer;
            if (img->native.data)
              {
                 Evas_Native_Surface *ens = img->native.data;
                 if (ens->data.tbm.buffer == buffer)
                   return img;
              }
         }
     }

   if ((!ns) && (!img->native.data)) return img;

   eng_window_use(ob);

   if (img->native.data)
     {
        if (img->native.func.free)
          img->native.func.free(img->native.func.data, img);
        glsym_evas_gl_common_image_native_disable(img);
     }

   if (!ns) return img;

   if (ns->type == EVAS_NATIVE_SURFACE_WL)
     {
        wlid = wl_buf;
        img2 = eina_hash_find(ob->gl_context->shared->native_wl_hash, &wlid);
        if (img2 == img) return img;
        if (img2)
          {
             if((n = img2->native.data))
               {
                  glsym_evas_gl_common_image_ref(img2);
                  glsym_evas_gl_common_image_free(img);
                  return img2;
               }
          }
     }
   else if (ns->type == EVAS_NATIVE_SURFACE_OPENGL)
     {
        texid = tex;
        img2 = eina_hash_find(ob->gl_context->shared->native_tex_hash, &texid);
        if (img2 == img) return img;
        if (img2)
          {
             if ((n = img2->native.data))
               {
                  glsym_evas_gl_common_image_ref(img2);
                  glsym_evas_gl_common_image_free(img);
                  return img2;
               }
          }
     }
   else if (ns->type == EVAS_NATIVE_SURFACE_EVASGL)
     {
         img2 = eina_hash_find(eng_get_ob(re)->gl_context->shared->native_evasgl_hash, &buffer);
        if (img2 == img) return img;
        if (img2)
          {
             n = img2->native.data;
             if (n)
              {
                 glsym_evas_gl_common_image_ref(img2);
                 glsym_evas_gl_common_image_free(img);
                 return img2;
              }
         }
     }
  else if (ns->type == EVAS_NATIVE_SURFACE_TBM)
    {
       img2 = eina_hash_find(ob->gl_context->shared->native_tbm_hash, &buffer);
       if (img2 == img) return img;
       if (img2)
         {
            n = img2->native.data;
            if (n)
             {
                glsym_evas_gl_common_image_ref(img2);
                glsym_evas_gl_common_image_free(img);
                return img2;
             }
        }
    }

   img2 = glsym_evas_gl_common_image_new_from_data(ob->gl_context, img->w,
                                                   img->h, NULL, img->alpha,
                                                   EVAS_COLORSPACE_ARGB8888);
   glsym_evas_gl_common_image_free(img);

   if (!(img = img2)) return NULL;

   if (ns->type == EVAS_NATIVE_SURFACE_WL)
     {
        if (native)
          {
             if ((n = calloc(1, sizeof(Native))))
               {
                  EGLint attribs[3];
                  int format, yinvert = 1;

                  glsym_eglQueryWaylandBuffer(ob->egl_disp, wl_buf,
                                                EGL_TEXTURE_FORMAT, &format);
                  if ((format != EGL_TEXTURE_RGB) &&
                      (format != EGL_TEXTURE_RGBA)&&
                      (format != EGL_TEXTURE_Y_U_V_WL) &&
                      (format != EGL_TEXTURE_Y_UV_WL))
                    {
                       ERR("eglQueryWaylandBuffer() %d format is not supported ", format);
                       glsym_evas_gl_common_image_free(img);
                       free(n);
                       return NULL;
                    }

                  attribs[0] = EVAS_GL_WAYLAND_PLANE_WL;
                  attribs[1] = 0; //if plane is 1 then 0, if plane is 2 then 1
                  attribs[2] = EGL_NONE;

                  memcpy(&(n->ns), ns, sizeof(Evas_Native_Surface));
                  if (glsym_eglQueryWaylandBuffer(ob->egl_disp, wl_buf,
                                                    EVAS_GL_WAYLAND_Y_INVERTED_WL,
                                                    &yinvert) == EGL_FALSE)
                    yinvert = 1;
                  eina_hash_add(ob->gl_context->shared->native_wl_hash,
                                &wlid, img);

                  n->ns_data.wl_surface.wl_buf = wl_buf;
                  if (glsym_eglCreateImage)
                    n->ns_data.wl_surface.surface = glsym_eglCreateImage(ob->egl_disp,
                                                          NULL,
                                                          EVAS_GL_WAYLAND_BUFFER_WL,
                                                          wl_buf, attribs);
                  else
                    {
                       ERR("Try eglCreateImage on EGL with no support");
                       eina_hash_del(ob->gl_context->shared->native_wl_hash,
                                     &wlid, img);
                       glsym_evas_gl_common_image_free(img);
                       free(n);
                       return NULL;
                    }

                  if (!n->ns_data.wl_surface.surface)
                    {
                       ERR("eglCreatePixmapSurface() for %p failed", wl_buf);
                       eina_hash_del(ob->gl_context->shared->native_wl_hash,
                                     &wlid, img);
                       glsym_evas_gl_common_image_free(img);
                       free(n);
                       return NULL;
                    }

                  //XXX: workaround for mesa-10.2.8
                  // mesa's eglQueryWaylandBufferWL() with EGL_WAYLAND_Y_INVERTED_WL works incorrect.
                  img->native.yinvert = yinvert;
//                  img->native.yinvert = 1;
                  img->native.loose = 0;
                  img->native.data = n;
                  img->native.func.data = re;
                  img->native.func.bind = _native_cb_bind;
                  img->native.func.unbind = _native_cb_unbind;
                  img->native.func.free = _native_cb_free;
                  if ( (format == EGL_TEXTURE_Y_U_V_WL)||
                       (format == EGL_TEXTURE_Y_UV_WL))
                     img->native.target = GL_TEXTURE_EXTERNAL_OES;
                  else
                     img->native.target = GL_TEXTURE_2D;
                  img->native.mipmap = 0;

                  glsym_evas_gl_common_image_native_enable(img);
               }
          }
     }
   else if (ns->type == EVAS_NATIVE_SURFACE_OPENGL)
     {
        if (native)
          {
             if ((n = calloc(1, sizeof(Native))))
               {
                  memcpy(&(n->ns), ns, sizeof(Evas_Native_Surface));
                  eina_hash_add(ob->gl_context->shared->native_tex_hash, &texid, img);

                  n->ns_data.opengl.surface = 0;

                  img->native.yinvert = 0;
                  img->native.loose = 0;
                  img->native.data = n;
                  img->native.func.data = re;
                  img->native.func.bind = _native_cb_bind;
                  img->native.func.unbind = _native_cb_unbind;
                  img->native.func.free = _native_cb_free;
                  img->native.func.yinvert = _native_cb_yinvert;
                  img->native.target = GL_TEXTURE_2D;
                  img->native.mipmap = 0;

                  glsym_evas_gl_common_image_native_enable(img);
               }
          }
     }
   else if (ns->type == EVAS_NATIVE_SURFACE_EVASGL)
     {
       if (native)
         {
           n = calloc(1, sizeof(Native));
           if (n)
             {
               memcpy(&(n->ns), ns, sizeof(Evas_Native_Surface));

               eina_hash_add(eng_get_ob(re)->gl_context->shared->native_evasgl_hash, &buffer, img);

               n->ns_data.evasgl.surface = ns->data.evasgl.surface;
               img->native.yinvert     = 0;
               img->native.loose       = 0;
               img->native.data        = n;
               img->native.func.data   = re;
               img->native.func.bind   = _native_cb_bind;
               img->native.func.unbind = _native_cb_unbind;
               img->native.func.free   = _native_cb_free;
               img->native.target      = GL_TEXTURE_2D;
               img->native.mipmap      = 0;

               glsym_evas_gl_common_image_native_enable(img);
             }
         }
     }
  else if (ns->type == EVAS_NATIVE_SURFACE_TBM)
    {
#ifdef GL_GLES
       if (native)
         {
           n = calloc(1, sizeof(Native));
           if (n)
             {
               eina_hash_add(eng_get_ob(re)->gl_context->shared->native_tbm_hash, &buffer, img);

               memcpy(&(n->ns), ns, sizeof(Evas_Native_Surface));
               n->ns_data.tbm.buffer = buffer;

               if (glsym_eglCreateImage)
                 n->ns_data.tbm.surface = glsym_eglCreateImage(eng_get_ob(re)->egl_disp,
                                                               EGL_NO_CONTEXT,
                                                               EGL_NATIVE_SURFACE_TIZEN,
                                                               (void *)buffer,
                                                               NULL);
               else
                 ERR("Try eglCreateImage on EGL with no support");
               if (!n->ns_data.tbm.surface)
                 ERR("eglCreateImage() for %p failed", buffer);
               img->native.yinvert     = 1;
               img->native.loose       = 0;
               img->native.data        = n;
               img->native.func.data   = re;
               img->native.func.bind   = _native_cb_bind;
               img->native.func.unbind = _native_cb_unbind;
               img->native.func.free   = _native_cb_free;
               img->native.target      = GL_TEXTURE_EXTERNAL_OES;
               img->native.mipmap      = 0;
               img->native.rot         = ns->data.tbm.rot;
               img->native.ratio       = ns->data.tbm.ratio;
               img->native.flip        = ns->data.tbm.flip;

               glsym_evas_gl_common_image_native_enable(img);
             }
         }
#endif
    }

   return img;
}

static void *
eng_image_native_get(void *data EINA_UNUSED, void *image)
{
   Evas_GL_Image *im = image;
   Evas_Native_Surface *n;

   if (!im) return NULL;
   n = im->native.data;
   if (!n) return NULL;
   return n;
}

Eina_Bool
eng_preload_make_current(void *data, void *doit)
{
   Outbuf *ob;

   if (!(ob = data)) return EINA_FALSE;

   if (doit)
     {
        if (!eglMakeCurrent_thread_cmd(ob->egl_disp, ob->egl_surface[0],
                            ob->egl_surface[0], ob->egl_context[0]))
          return EINA_FALSE;
     }
   else
     {
        if (!eglMakeCurrent_thread_cmd(ob->egl_disp, EGL_NO_SURFACE, EGL_NO_SURFACE,
                            EGL_NO_CONTEXT))
          return EINA_FALSE;
     }

   return EINA_TRUE;
}

/* evas module functions */
static int
module_open(Evas_Module *em)
{
   /* check for valid module */
   if (!em) return 0;

   /* get whatever engine module we inherit from */
   if (!_evas_module_engine_inherit(&pfunc, "gl_generic")) return 0;

   /* setup logging domain */
   if (_evas_engine_wl_egl_log_dom < 0)
     {
        _evas_engine_wl_egl_log_dom =
          eina_log_domain_register("evas-wayland_egl", EVAS_DEFAULT_LOG_COLOR);
     }

   if (_evas_engine_wl_egl_log_dom < 0)
     {
        EINA_LOG_ERR("Can not create a module log domain.");
        return 0;
     }

   /* store functions for later use */
   func = pfunc;
#define ORD(f) EVAS_API_OVERRIDE(f, &func, eng_)

   ORD(info);
   ORD(info_free);
   ORD(setup);
   ORD(canvas_alpha_get);

   ORD(output_free);
   ORD(output_dump);

   ORD(image_native_set);

   ORD(gl_current_context_get);
   ORD(gl_error_get);

   evas_gl_thread_link_init();
   gl_symbols();

   //TIZEN_ONLY(20161121) : Support PreRotation
   pre_rotation_symbols();
   //

   /* advertise out which functions we support */
   em->functions = (void *)(&func);

   return 1;
}

static void
module_close(Evas_Module *em EINA_UNUSED)
{
   eina_log_domain_unregister(_evas_engine_wl_egl_log_dom);
}

static Evas_Module_Api evas_modapi =
{
   EVAS_MODULE_API_VERSION, "wayland_egl", "none", {module_open, module_close}
};

EVAS_MODULE_DEFINE(EVAS_MODULE_TYPE_ENGINE, engine, wayland_egl);

#ifndef EVAS_STATIC_BUILD_WAYLAND_EGL
EVAS_EINA_MODULE_DEFINE(engine, wayland_egl);
#endif
