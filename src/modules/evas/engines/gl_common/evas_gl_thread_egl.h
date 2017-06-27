#ifndef EVAS_GL_THREAD_EGL_H
#define EVAS_GL_THREAD_EGL_H

#ifdef GL_GLES

#include <EGL/egl.h>


typedef void (*WL_EGL_RECONFIG)(void *win, int w, int h, int rot, int info_rot, int window_rotation, int info_edges);

#ifdef EVAS_GL_RENDER_THREAD_COMPILE_FOR_GL_GENERIC


#ifdef EAPI
# undef EAPI
#endif

#ifdef _WIN32
# ifdef EFL_EVAS_BUILD
#  ifdef DLL_EXPORT
#   define EAPI __declspec(dllexport)
#  else
#   define EAPI
#  endif /* ! DLL_EXPORT */
# else
#  define EAPI __declspec(dllimport)
# endif /* ! EFL_EVAS_BUILD */
#else
# ifdef __GNUC__
#  if __GNUC__ >= 4
#   define EAPI __attribute__ ((visibility("default")))
#  else
#   define EAPI
#  endif
# else
#  define EAPI
# endif
#endif /* ! _WIN32 */

/* EGL 1.4 Referencing to Thread Local Storage */
EAPI EGLint     eglGetError_thread_cmd();
EAPI EGLBoolean eglBindAPI_thread_cmd(EGLenum api);
EAPI EGLenum    eglQueryAPI_thread_cmd();
EAPI EGLBoolean eglMakeCurrent_thread_cmd(EGLDisplay dpy, EGLSurface draw, EGLSurface read, EGLContext ctx);
EAPI EGLContext eglGetCurrentContext_thread_cmd(void);
EAPI EGLSurface eglGetCurrentSurface_thread_cmd(EGLint readdraw);
EAPI EGLDisplay eglGetCurrentDisplay_thread_cmd(void);
EAPI EGLBoolean eglReleaseThread_thread_cmd();


/* EGL 1.4 Sequential Operations */
EAPI EGLBoolean eglQuerySurface_thread_cmd(EGLDisplay dpy, EGLSurface surface, EGLint attribute, EGLint *value);
EAPI EGLBoolean eglSwapInterval_thread_cmd(EGLDisplay dpy, EGLint interval);
EAPI EGLBoolean eglWaitGL_thread_cmd(void);
EAPI EGLBoolean eglSwapBuffers_thread_cmd(EGLDisplay dpy, EGLSurface surface);

EAPI void       eglSwapBuffersWithDamage_orig_evas_set(void *func);
EAPI void      *eglSwapBuffersWithDamage_orig_evas_get();
EAPI EGLBoolean eglSwapBuffersWithDamage_thread_cmd(EGLDisplay dpy, EGLSurface surface, EGLint *rects, EGLint n_rects);

EAPI void       eglSetDamageRegion_orig_evas_set(void *func);
EAPI void      *eglSetDamageRegion_orig_evas_get();
EAPI EGLBoolean eglSetDamageRegion_thread_cmd(EGLDisplay dpy, EGLSurface surface, EGLint *rects, EGLint n_rects);

EAPI void       eglQueryWaylandBuffer_orig_evas_set(void *func);
EAPI void      *eglQueryWaylandBuffer_orig_evas_get();
EAPI EGLBoolean eglQueryWaylandBuffer_thread_cmd(EGLDisplay dpy, void *buffer, EGLint attribute, EGLint *value);

EAPI void       eglGetProcAddress_orig_evas_set(void *func);
EAPI void      *eglGetProcAddress_orig_evas_get();
EAPI void      *eglGetProcAddress_thread_cmd(char const * procname);

EAPI void       eglWindowReconfig_thread_cmd(void *win, int w, int h, int rot, int info_rot, int window_rotation, int info_edges, WL_EGL_RECONFIG fn_wl_egl_reconfig);

/***** EVAS GL *****/

/* EGL 1.4 Referencing to Thread Local Storage */
EAPI EGLint     eglGetError_evgl_thread_cmd();
EAPI EGLBoolean eglMakeCurrent_evgl_thread_cmd(EGLDisplay dpy, EGLSurface draw, EGLSurface read, EGLContext ctx);
EAPI EGLContext eglGetCurrentContext_evgl_thread_cmd(void);
EAPI EGLSurface eglGetCurrentSurface_evgl_thread_cmd(EGLint readdraw);
EAPI EGLDisplay eglGetCurrentDisplay_evgl_thread_cmd(void);
EAPI EGLSurface eglCreateWindowSurface_evgl_thread_cmd(EGLDisplay egl_disp, EGLConfig egl_config, EGLNativeWindowType egl_win, EGLint const * attrib_list);
EAPI EGLSurface eglDestroySurface_evgl_thread_cmd(EGLDisplay egl_disp, EGLSurface egl_surf);
EAPI EGLContext eglCreateContext_evgl_thread_cmd(EGLDisplay display, EGLConfig config, EGLContext share_context, EGLint const * attrib_list);
EAPI EGLBoolean eglDestroyContext_evgl_thread_cmd(EGLDisplay display, EGLContext context);
EAPI char const *eglQueryString_evgl_thread_cmd(EGLDisplay display,  EGLint name);


#else /* ! EVAS_GL_RENDER_THREAD_COMPILE_FOR_GL_GENERIC */


/* EGL 1.4 Referencing to Thread Local Storage */
extern EGLint     (*eglGetError_thread_cmd)();
extern EGLBoolean (*eglBindAPI_thread_cmd)(EGLenum api);
extern EGLenum    (*eglQueryAPI_thread_cmd)();
extern EGLBoolean (*eglMakeCurrent_thread_cmd)(EGLDisplay dpy, EGLSurface draw, EGLSurface read, EGLContext ctx);
extern EGLContext (*eglGetCurrentContext_thread_cmd)(void);
extern EGLSurface (*eglGetCurrentSurface_thread_cmd)(EGLint readdraw);
extern EGLDisplay (*eglGetCurrentDisplay_thread_cmd)(void);
extern EGLBoolean (*eglReleaseThread_thread_cmd)();
extern void       (*eglGetProcAddress_orig_evas_set)(void *func);
extern void      *(*eglGetProcAddress_orig_evas_get)();
extern void       *(*eglGetProcAddress_thread_cmd)(char const * procname);


/* EGL 1.4 Sequential Operations */
extern EGLBoolean (*eglQuerySurface_thread_cmd)(EGLDisplay dpy, EGLSurface surface, EGLint attribute, EGLint *value);
extern EGLBoolean (*eglSwapInterval_thread_cmd)(EGLDisplay dpy, EGLint interval);
extern EGLBoolean (*eglWaitGL_thread_cmd)(void);
extern EGLBoolean (*eglSwapBuffers_thread_cmd)(EGLDisplay dpy, EGLSurface surface);
extern void       (*eglSwapBuffersWithDamage_orig_evas_set)(void *func);
extern void      *(*eglSwapBuffersWithDamage_orig_evas_get)();
extern EGLBoolean (*eglSwapBuffersWithDamage_thread_cmd)(EGLDisplay dpy, EGLSurface surface, EGLint *rects, EGLint n_rects);
extern void       (*eglSetDamageRegion_orig_evas_set)(void *func);
extern void      *(*eglSetDamageRegion_orig_evas_get)();
extern EGLBoolean (*eglSetDamageRegion_thread_cmd)(EGLDisplay dpy, EGLSurface surface, EGLint *rects, EGLint n_rects);
extern void       (*eglQueryWaylandBuffer_orig_evas_set)(void *func);
extern void      *(*eglQueryWaylandBuffer_orig_evas_get)();
extern EGLBoolean (*eglQueryWaylandBuffer_thread_cmd)(EGLDisplay dpy, void *buffer, EGLint attribute, EGLint *value);
extern void       (*eglWindowReconfig_thread_cmd)(void *win, int w, int h, int rot, int info_rot, int window_rotation, int info_edges, WL_EGL_RECONFIG fn_wl_egl_reconfig);


/***** EVAS GL *****/

/* EGL 1.4 Referencing to Thread Local Storage */
extern EGLint     (*eglGetError_evgl_thread_cmd)();
extern EGLBoolean (*eglMakeCurrent_evgl_thread_cmd)(EGLDisplay dpy, EGLSurface draw, EGLSurface read, EGLContext ctx);
extern EGLContext (*eglGetCurrentContext_evgl_thread_cmd)(void);
extern EGLSurface (*eglGetCurrentSurface_evgl_thread_cmd)(EGLint readdraw);
extern EGLDisplay (*eglGetCurrentDisplay_evgl_thread_cmd)(void);
extern EGLSurface (*eglCreateWindowSurface_evgl_thread_cmd)(EGLDisplay egl_disp, EGLConfig egl_config, EGLNativeWindowType egl_win, EGLint const * attrib_list);
extern EGLSurface (*eglDestroySurface_evgl_thread_cmd)(EGLDisplay egl_disp, EGLSurface egl_surf);
extern EGLContext (*eglCreateContext_evgl_thread_cmd)(EGLDisplay display, EGLConfig config, EGLContext share_context, EGLint const * attrib_list);
extern EGLBoolean (*eglDestroyContext_evgl_thread_cmd)(EGLDisplay display, EGLContext context);
extern char const *(*eglQueryString_evgl_thread_cmd)(EGLDisplay display,  EGLint name);



extern void _egl_thread_link_init();


#endif /* EVAS_GL_RENDER_THREAD_COMPILE_FOR_GL_GENERIC */

#endif /* GL_GLES */

#endif
