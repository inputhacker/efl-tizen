#ifndef EVAS_GL_THREAD_GLX_H
#define EVAS_GL_THREAD_GLX_H

#ifndef GL_GLES

# include <GL/glx.h>

#ifdef EVAS_GL_RENDER_THREAD_IS_GENERIC

#define GL_GLEXT_PROTOTYPES

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


EAPI void glXBindTexImage_orig_evas_set(void *func);
EAPI void *glXBindTexImage_orig_evas_get(void);
EAPI void glXBindTexImage_thread_cmd(Display *dpy, GLXDrawable drawable, int buffer, const int *attrib_list);
EAPI GLXFBConfig * glXChooseFBConfig_thread_cmd(Display *dpy, int screen, const int *attribList, int *nitems);
EAPI GLXContext glXCreateContext_thread_cmd(Display *dpy, XVisualInfo *vis, GLXContext shareList, Bool direct);
EAPI GLXContext glXCreateNewContext_thread_cmd(Display *dpy, GLXFBConfig config, int renderType, GLXContext shareList, Bool direct);
EAPI GLXPbuffer glXCreatePbuffer_thread_cmd(Display *dpy, GLXFBConfig config, const int *attribList);
EAPI void glXCreatePixmap_orig_evas_set(void *func);
EAPI void *glXCreatePixmap_orig_evas_get(void);
EAPI GLXPixmap glXCreatePixmap_thread_cmd(Display *dpy, GLXFBConfig config, Pixmap pixmap, const int *attribList);
EAPI GLXWindow glXCreateWindow_thread_cmd(Display *dpy, GLXFBConfig config, Window win, const int *attribList);
EAPI void glXDestroyContext_thread_cmd(Display *dpy, GLXContext ctx);
EAPI void glXDestroyPbuffer_thread_cmd(Display *dpy, GLXPbuffer pbuf);
EAPI void glXDestoyPixmap_orig_evas_set(void *func);
EAPI void *glXDestoyPixmap_orig_evas_get(void);
EAPI void glXDestroyPixmap_thread_cmd(Display *dpy, GLXPixmap pixmap);
EAPI void glXDestroyWindow_thread_cmd(Display *dpy, GLXWindow window);
EAPI int glXGetConfig_thread_cmd(Display *dpy, XVisualInfo *visual, int attrib, int *value);
EAPI GLXContext glXGetCurrentContext_thread_cmd(void);
EAPI int glXGetFBConfigAttrib_thread_cmd(Display *dpy, GLXFBConfig config, int attribute, int *value);

EAPI void glXGetVideoSync_orig_evas_set(void *func);
EAPI void *glXGetVideoSync_orig_evas_get(void);
EAPI int glXGetVideoSync_thread_cmd(unsigned int *count);
EAPI XVisualInfo * glXGetVisualFromFBConfig_thread_cmd(Display *dpy, GLXFBConfig config);
EAPI Bool glXMakeContextCurrent_thread_cmd(Display *dpy, GLXDrawable draw, GLXDrawable read, GLXContext ctx);
EAPI void glXQueryDrawable_orig_evas_set(void *func);
EAPI void *glXQueryDrawable_orig_evas_get(void);
EAPI void glXQueryDrawable_thread_cmd(Display *dpy, GLXDrawable draw, int attribute, unsigned int *value);
EAPI Bool glXQueryExtension_thread_cmd(Display *dpy, int *errorb, int *event);
EAPI const char * glXQueryExtensionsString_thread_cmd(Display *dpy, int screen);

EAPI void glXReleaseBuffersMESA_orig_evas_set(void *func);
EAPI void *glXReleaseBuffersMESA_orig_evas_get(void);
EAPI Bool glXReleaseBuffersMESA_thread_cmd(Display *dpy, GLXDrawable drawable);

EAPI void glXReleaseTexImageEXT_orig_evas_set(void *func);
EAPI void *glXReleaseTexImageEXT_orig_evas_get(void);
EAPI void glXReleaseTexImageEXT_thread_cmd(Display *dpy, GLXDrawable drawable, int buffer);
EAPI void glXSwapBuffers_thread_cmd(Display *dpy, GLXDrawable drawable);

EAPI void glXSwapIntervalEXT_orig_evas_set(void *func);
EAPI void *glXSwapIntervalEXT_orig_evas_get(void);
EAPI void glXSwapIntervalEXT_thread_cmd(Display *dpy, GLXDrawable drawable, int interval);

EAPI void glXSwapIntervalSGI_orig_evas_set(void *func);
EAPI void *glXSwapIntervalSGI_orig_evas_get(void);
EAPI int glXSwapIntervalSGI_thread_cmd(int interval);

EAPI void glXWaitVideoSync_orig_evas_set(void *func);
EAPI void *glXWaitVideoSync_orig_evas_get(void);
EAPI int glXWaitVideoSync_thread_cmd(int divisor, int remainder, unsigned int *count);

/****** EVAS GL ******/

EAPI void glXBindTexImage_orig_evgl_set(void *func);
EAPI void *glXBindTexImage_orig_evgl_get(void);
EAPI void glXBindTexImage_evgl_thread_cmd(Display *dpy, GLXDrawable drawable, int buffer, const int *attrib_list);
EAPI GLXFBConfig * glXChooseFBConfig_evgl_thread_cmd(Display *dpy, int screen, const int *attribList, int *nitems);
EAPI GLXContext glXCreateContext_evgl_thread_cmd(Display *dpy, XVisualInfo *vis, GLXContext shareList, Bool direct);
EAPI GLXContext glXCreateNewContext_evgl_thread_cmd(Display *dpy, GLXFBConfig config, int renderType, GLXContext shareList, Bool direct);
EAPI GLXPbuffer glXCreatePbuffer_evgl_thread_cmd(Display *dpy, GLXFBConfig config, const int *attribList);

EAPI void glXCreatePixmap_orig_evgl_set(void *func);
EAPI void *glXCreatePixmap_orig_evgl_get(void);
EAPI GLXPixmap glXCreatePixmap_evgl_thread_cmd(Display *dpy, GLXFBConfig config, Pixmap pixmap, const int *attribList);
EAPI GLXWindow glXCreateWindow_evgl_thread_cmd(Display *dpy, GLXFBConfig config, Window win, const int *attribList);
EAPI void glXDestroyContext_evgl_thread_cmd(Display *dpy, GLXContext ctx);
EAPI void glXDestroyPbuffer_evgl_thread_cmd(Display *dpy, GLXPbuffer pbuf);

EAPI void glXDestroyPixmap_orig_evgl_set(void *func);
EAPI void *glXDestroyPixmap_orig_evgl_get(void);
EAPI void glXDestroyPixmap_evgl_thread_cmd(Display *dpy, GLXPixmap pixmap);
EAPI void glXDestroyWindow_evgl_thread_cmd(Display *dpy, GLXWindow window);
EAPI int glXGetConfig_evgl_thread_cmd(Display *dpy, XVisualInfo *visual, int attrib, int *value);
EAPI GLXContext glXGetCurrentContext_evgl_thread_cmd(void);
EAPI int glXGetFBConfigAttrib_evgl_thread_cmd(Display *dpy, GLXFBConfig config, int attribute, int *value);

EAPI void glXGetVideoSync_orig_evgl_set(void *func);
EAPI void *glXGetVideoSync_orig_evgl_get(void);
EAPI int glXGetVideoSync_evgl_thread_cmd(unsigned int *count);
EAPI XVisualInfo * glXGetVisualFromFBConfig_evgl_thread_cmd(Display *dpy, GLXFBConfig config);
EAPI Bool glXMakeContextCurrent_evgl_thread_cmd(Display *dpy, GLXDrawable draw, GLXDrawable read, GLXContext ctx);

EAPI void glXQueryDrawable_orig_evgl_set(void *func);
EAPI void *glXQueryDrawable_orig_evgl_get(void);
EAPI void glXQueryDrawable_evgl_thread_cmd(Display *dpy, GLXDrawable draw, int attribute, unsigned int *value);
EAPI Bool glXQueryExtension_evgl_thread_cmd(Display *dpy, int *errorb, int *event);
EAPI const char * glXQueryExtensionsString_evgl_thread_cmd(Display *dpy, int screen);

EAPI void glXReleaseBuffersMESA_orig_evgl_set(void *func);
EAPI void *glXReleaseBuffersMESA_orig_evgl_get(void);
EAPI Bool glXReleaseBuffersMESA_evgl_thread_cmd(Display *dpy, GLXDrawable drawable);

EAPI void glXReleaseTexImage_orig_evgl_set(void *func);
EAPI void *glXReleaseTexImage_orig_evgl_get(void);
EAPI void glXReleaseTexImage_evgl_thread_cmd(Display *dpy, GLXDrawable drawable, int buffer);
EAPI void glXSwapBuffers_evgl_thread_cmd(Display *dpy, GLXDrawable drawable);

EAPI void glXSwapIntervalEXT_orig_evgl_set(void *func);
EAPI void *glXSwapIntervalEXT_orig_evgl_get(void);
EAPI void glXSwapIntervalEXT_evgl_thread_cmd(Display *dpy, GLXDrawable drawable, int interval);

EAPI void glXSwapIntervalSGI_orig_evgl_set(void *func);
EAPI void *glXSwapIntervalSGI_orig_evgl_get(void);
EAPI int glXSwapIntervalSGI_evgl_thread_cmd(int interval);

EAPI void glXWaitVideoSync_orig_evgl_set(void *func);
EAPI void *glXWaitVideoSync_orig_evgl_get(void);
EAPI int glXWaitVideoSync_evgl_thread_cmd(int divisor, int remainder, unsigned int *count);

#else /* EVAS_GL_RENDER_THREAD_IS_GENERIC */


extern void (*glXBindTexImage_orig_evas_set)(void *func);
extern void *(*glXBindTexImage_orig_evas_get)(void);
extern void (*glXBindTexImage_thread_cmd)(Display *dpy, GLXDrawable drawable, int buffer, const int *attrib_list);
extern GLXFBConfig * (*glXChooseFBConfig_thread_cmd)(Display *dpy, int screen, const int *attribList, int *nitems);
extern GLXContext (*glXCreateContext_thread_cmd)(Display *dpy, XVisualInfo *vis, GLXContext shareList, Bool direct);
extern GLXContext (*glXCreateNewContext_thread_cmd)(Display *dpy, GLXFBConfig config, int renderType, GLXContext shareList, Bool direct);
extern GLXPbuffer (*glXCreatePbuffer_thread_cmd)(Display *dpy, GLXFBConfig config, const int *attribList);
extern void (*glXCreatePixmap_orig_evas_set)(void *func);
extern void  *(*glXCreatePixmap_orig_evas_get)(void);
extern GLXPixmap (*glXCreatePixmap_thread_cmd)(Display *dpy, GLXFBConfig config, Pixmap pixmap, const int *attribList);
extern GLXWindow (*glXCreateWindow_thread_cmd)(Display *dpy, GLXFBConfig config, Window win, const int *attribList);
extern void (*glXDestroyContext_thread_cmd)(Display *dpy, GLXContext ctx);
extern void (*glXDestroyPbuffer_thread_cmd)(Display *dpy, GLXPbuffer pbuf);
extern void (*glXDestroyPixmap_orig_evas_set)(void *func);
extern void *(*glXDestroyPixmap_orig_evas_get)(void);
extern void (*glXDestroyPixmap_thread_cmd)(Display *dpy, GLXPixmap pixmap);
extern void (*glXDestroyWindow_thread_cmd)(Display *dpy, GLXWindow window);
extern int (*glXGetConfig_thread_cmd)(Display *dpy, XVisualInfo *visual, int attrib, int *value);
extern GLXContext (*glXGetCurrentContext_thread_cmd)(void);
extern int (*glXGetFBConfigAttrib_thread_cmd)(Display *dpy, GLXFBConfig config, int attribute, int *value);
extern void (*glXGetVideoSync_orig_evas_set)(void *func);
extern void *(*glXGetVideoSync_orig_evas_get)(void);
extern int (*glXGetVideoSync_thread_cmd)(unsigned int *count);
extern XVisualInfo * (*glXGetVisualFromFBConfig_thread_cmd)(Display *dpy, GLXFBConfig config);
extern Bool (*glXMakeContextCurrent_thread_cmd)(Display *dpy, GLXDrawable draw, GLXDrawable read, GLXContext ctx);
extern void (*glXQueryDrawable_orig_evas_set)(void *func);
extern void *(*glXQueryDrawable_orig_evas_get)(void);
extern void (*glXQueryDrawable_thread_cmd)(Display *dpy, GLXDrawable draw, int attribute, unsigned int *value);
extern Bool (*glXQueryExtension_thread_cmd)(Display *dpy, int *errorb, int *event);
extern const char * (*glXQueryExtensionsString_thread_cmd)(Display *dpy, int screen);
extern void (*glXReleaseBuffersMESA_orig_evas_set)(void *func);
extern void *(*glXReleaseBuffersMESA_orig_evas_get)(void);
extern Bool (*glXReleaseBuffersMESA_thread_cmd)(Display *dpy, GLXDrawable drawable);
extern void (*glXReleaseTexImage_orig_evas_set)(void *func);
extern void *(*glXReleaseTexImage_orig_evas_get)(void);
extern void (*glXReleaseTexImage_thread_cmd)(Display *dpy, GLXDrawable drawable, int buffer);
extern void (*glXSwapBuffers_thread_cmd)(Display *dpy, GLXDrawable drawable);
extern void (*glXSwapIntervalEXT_orig_evas_set)(void *func);
extern void *(*glXSwapIntervalEXT_orig_evas_get)(void);
extern void (*glXSwapIntervalEXT_thread_cmd)(Display *dpy, GLXDrawable drawable, int interval);
extern void (*glXSwapIntervalSGI_orig_evas_set)(void *func);
extern void *(*glXSwapIntervalSGI_orig_evas_get)(void);
extern int (*glXSwapIntervalSGI_thread_cmd)(int interval);
extern void (*glXWaitVideoSync_orig_evas_set)(void *func);
extern void *(*glXWaitVideoSync_orig_evas_get)(void);
extern int (*glXWaitVideoSync_thread_cmd)(int divisor, int remainder, unsigned int *count);

/****** EVAS GL ******/

extern void (*glXBindTexImage_orig_evgl_set)(void *func);
extern void *(*glXBindTexImage_orig_evgl_get)(void);
extern void (*glXBindTexImage_evgl_thread_cmd)(Display *dpy, GLXDrawable drawable, int buffer, const int *attrib_list);
extern GLXFBConfig * (*glXChooseFBConfig_evgl_thread_cmd)(Display *dpy, int screen, const int *attribList, int *nitems);
extern GLXContext (*glXCreateContext_evgl_thread_cmd)(Display *dpy, XVisualInfo *vis, GLXContext shareList, Bool direct);
extern GLXContext (*glXCreateNewContext_evgl_thread_cmd)(Display *dpy, GLXFBConfig config, int renderType, GLXContext shareList, Bool direct);
extern GLXPbuffer (*glXCreatePbuffer_evgl_thread_cmd)(Display *dpy, GLXFBConfig config, const int *attribList);
extern void (*glXCreatePixmap_orig_evgl_set)(void *func);
extern void *(*glXCreatePixmap_orig_evgl_get)(void);
extern GLXPixmap (*glXCreatePixmap_evgl_thread_cmd)(Display *dpy, GLXFBConfig config, Pixmap pixmap, const int *attribList);
extern GLXWindow (*glXCreateWindow_evgl_thread_cmd)(Display *dpy, GLXFBConfig config, Window win, const int *attribList);
extern void (*glXDestroyContext_evgl_thread_cmd)(Display *dpy, GLXContext ctx);
extern void (*glXDestroyPbuffer_evgl_thread_cmd)(Display *dpy, GLXPbuffer pbuf);
extern void (*glXDestroyPixmap_orig_evgl_set)(void *func);
extern void *(*glXDestroyPixmap_orig_evgl_get)(void);
extern void (*glXDestroyPixmap_evgl_thread_cmd)(Display *dpy, GLXPixmap pixmap);
extern void (*glXDestroyWindow_evgl_thread_cmd)(Display *dpy, GLXWindow window);
extern int (*glXGetConfig_evgl_thread_cmd)(Display *dpy, XVisualInfo *visual, int attrib, int *value);
extern GLXContext (*glXGetCurrentContext_evgl_thread_cmd)(void);
extern int (*glXGetFBConfigAttrib_evgl_thread_cmd)(Display *dpy, GLXFBConfig config, int attribute, int *value);
extern void (*glXGetVideoSync_orig_evgl_set)(void *func);
extern void *(*glXGetVideoSync_orig_evgl_get)(void);
extern int (*glXGetVideoSync_evgl_thread_cmd)(unsigned int *count);
extern XVisualInfo * (*glXGetVisualFromFBConfig_evgl_thread_cmd)(Display *dpy, GLXFBConfig config);
extern Bool (*glXMakeContextCurrent_evgl_thread_cmd)(Display *dpy, GLXDrawable draw, GLXDrawable read, GLXContext ctx);
extern void (*glXQueryDrawable_orig_evgl_set)(void *func);
extern void *(*glXQueryDrawable_orig_evgl_get)(void);
extern void (*glXQueryDrawable_evgl_thread_cmd)(Display *dpy, GLXDrawable draw, int attribute, unsigned int *value);
extern Bool (*glXQueryExtension_evgl_thread_cmd)(Display *dpy, int *errorb, int *event);
extern const char * (*glXQueryExtensionsString_evgl_thread_cmd)(Display *dpy, int screen);
extern void (*glXReleaseBuffersMESA_orig_evgl_set)(void *func);
extern void *(*glXReleaseBuffersMESA_orig_evgl_get)(void);
extern Bool (*glXReleaseBuffersMESA_evgl_thread_cmd)(Display *dpy, GLXDrawable drawable);
extern void (*glXReleaseTexImage_orig_evgl_set)(void *func);
extern void *(*glXReleaseTexImage_orig_evgl_get)(void);
extern void (*glXReleaseTexImage_evgl_thread_cmd)(Display *dpy, GLXDrawable drawable, int buffer);
extern void (*glXSwapBuffers_evgl_thread_cmd)(Display *dpy, GLXDrawable drawable);
extern void (*glXSwapIntervalEXT_orig_evgl_set)(void *func);
extern void *(*glXSwapIntervalEXT_orig_evgl_get)(void);
extern void (*glXSwapIntervalEXT_evgl_thread_cmd)(Display *dpy, GLXDrawable drawable, int interval);
extern void (*glXSwapIntervalSGI_orig_evgl_set)(void *func);
extern void *(*glXSwapIntervalSGI_orig_evgl_get)(void);
extern int (*glXSwapIntervalSGI_evgl_thread_cmd)(int interval);
extern void (*glXWaitVideoSync_orig_evgl_set)(void *func);
extern void *(*glXWaitVideoSync_orig_evgl_get)(void);
extern int (*glXWaitVideoSync_evgl_thread_cmd)(int divisor, int remainder, unsigned int *count);


extern void _glx_thread_link_init();


#endif /* EVAS_GL_RENDER_THREAD_IS_GENERIC */

#endif /* ! GL_GLES */

#endif /* EVAS_GL_THREAD_GLX_H */
