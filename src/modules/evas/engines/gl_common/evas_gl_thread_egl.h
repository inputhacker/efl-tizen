#ifndef EVAS_GL_THREAD_EGL_H
#define EVAS_GL_THREAD_EGL_H

#ifdef GL_GLES

#include <EGL/egl.h>
#include <EGL/eglext.h>

extern void *evas_gl_thread_egl_func_get(void);


#define EVAS_TH_EGL_FN_LIST \
/* EGL 1.4 Referencing to Thread Local Storage */ \
EVAS_TH_EGL_FN      (EGLint     , eglGetError) \
EVAS_TH_EGL_FN      (EGLBoolean , eglBindAPI, EGLenum api) \
EVAS_TH_EGL_FN      (EGLenum    , eglQueryAPI) \
EVAS_TH_EGL_FN      (EGLBoolean , eglMakeCurrent, EGLDisplay dpy, EGLSurface draw, EGLSurface read, EGLContext ctx) \
EVAS_TH_EGL_FN      (EGLContext , eglGetCurrentContext) \
EVAS_TH_EGL_FN      (EGLSurface , eglGetCurrentSurface, EGLint readdraw) \
EVAS_TH_EGL_FN      (EGLDisplay , eglGetCurrentDisplay) \
EVAS_TH_EGL_FN      (EGLBoolean , eglReleaseThread) \
 \
/* EGL 1.4 Sequential Operations */ \
EVAS_TH_EGL_FN      (EGLBoolean , eglQuerySurface, EGLDisplay dpy, EGLSurface surface, EGLint attribute, EGLint *value) \
EVAS_TH_EGL_FN      (EGLBoolean , eglSwapInterval, EGLDisplay dpy, EGLint interval) \
EVAS_TH_EGL_FN      (EGLBoolean , eglWaitGL) \
EVAS_TH_EGL_FN      (EGLBoolean , eglSwapBuffers, EGLDisplay dpy, EGLSurface surface) \
EVAS_TH_EGL_FN      (EGLBoolean , eglSwapBuffersWithDamage, EGLDisplay dpy, EGLSurface surface, EGLint *rects, EGLint n_rects) \
EVAS_TH_EGL_FN      (EGLBoolean , eglSetDamageRegion, EGLDisplay dpy, EGLSurface surface, EGLint *rects, EGLint n_rects) \
EVAS_TH_EGL_FN      (EGLBoolean , eglQueryWaylandBufferWL, EGLDisplay dpy, void *buffer, EGLint attribute, EGLint *value) \
EVAS_TH_EGL_FN      (EGLSurface , eglCreateWindowSurface, EGLDisplay egl_disp, EGLConfig egl_config, EGLNativeWindowType egl_win, EGLint const * attrib_list) \
EVAS_TH_EGL_FN      (EGLBoolean , eglDestroySurface, EGLDisplay egl_disp, EGLSurface egl_surf) \
EVAS_TH_EGL_FN      (EGLContext , eglCreateContext, EGLDisplay display, EGLConfig config, EGLContext share_context, EGLint const * attrib_list) \
EVAS_TH_EGL_FN      (EGLBoolean , eglDestroyContext, EGLDisplay display, EGLContext context) \
EVAS_TH_EGL_FN      (char const *, eglQueryString, EGLDisplay display,  EGLint name) \
EVAS_TH_EGL_FN      (void *     , eglCreateImage, EGLDisplay dpy, EGLContext ctx, int target, void* buffer, int *attribs) \
EVAS_TH_EGL_FN      (EGLBoolean , eglDestroyImage,  EGLDisplay  dpy, EGLImageKHR img) \
EVAS_TH_EGL_FN      (void *     , eglCreateSyncKHR, EGLDisplay dpy, unsigned int type, const int *attrib_list) \
EVAS_TH_EGL_FN      (EGLBoolean , eglDestroySyncKHR,  EGLDisplay  dpy, void * sync) \
EVAS_TH_EGL_FN      (int        , eglClientWaitSyncKHR,  EGLDisplay  dpy,  void * sync, int flags, unsigned long long timeout) \
EVAS_TH_EGL_FN      (EGLBoolean , eglSignalSyncKHR,  EGLDisplay  dpy,  void * sync, unsigned mode) \
EVAS_TH_EGL_FN      (EGLBoolean , eglGetSyncAttribKHR,  EGLDisplay  dpy,  void * sync, int attribute, int *value) \
EVAS_TH_EGL_FN      (int        , eglWaitSyncKHR,  EGLDisplay  dpy,  void * sync, int flags) \
EVAS_TH_EGL_FN      (EGLBoolean , eglBindWaylandDisplayWL,  EGLDisplay  dpy,  void *wl_display) \
EVAS_TH_EGL_FN      (EGLBoolean , eglUnbindWaylandDisplayWL,  EGLDisplay  dpy,  void *wl_display) \
EVAS_TH_EGL_FN      (void *     , eglGetProcAddress, char const * procname) \
EVAS_TH_EGL_FN      (void       , wl_egl_window_tizen_set_buffer_transform, void *egl_window, int wl_output_transform) \
EVAS_TH_EGL_FN      (void       , wl_egl_window_tizen_set_window_transform, void *egl_window, int wl_output_transform)





#ifdef EVAS_GL_RENDER_THREAD_COMPILE_FOR_GL_GENERIC


#define EVAS_TH_EGL_FN(ret, name, ...) \
 extern ret GL_TH_FN(name)(GL_TH_DP, ##__VA_ARGS__);
#define EVAS_TH_EGL_FN_ASYNC(ret, name, ...) \
 extern void *GL_TH_FN(name##_begin)(GL_TH_DP, ##__VA_ARGS__); \
 extern ret GL_TH_FN(name##_end)(void *ref);

EVAS_TH_EGL_FN_LIST

#undef EVAS_TH_EGL_FN_ASYNC
#undef EVAS_TH_EGL_FN


#else /* ! EVAS_GL_RENDER_THREAD_COMPILE_FOR_GL_GENERIC */


#define EVAS_TH_EGL_FN(ret, name, ...) \
 extern ret (*GL_TH_FN(name))(GL_TH_DP, ##__VA_ARGS__);
#define EVAS_TH_EGL_FN_ASYNC(ret, name, ...) \
 extern void *(*GL_TH_FN(name##_begin))(GL_TH_DP, ##__VA_ARGS__); \
 extern ret (*GL_TH_FN(name##_end))(void *ref);

EVAS_TH_EGL_FN_LIST

#undef EVAS_TH_EGL_FN_ASYNC
#undef EVAS_TH_EGL_FN

extern void _egl_thread_link_init(void *func_ptr);


#endif /* EVAS_GL_RENDER_THREAD_COMPILE_FOR_GL_GENERIC */


#endif /* GL_GLES */

#endif /* EVAS_GL_THREAD_EGL_H */
