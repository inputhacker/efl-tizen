#include "evas_common_private.h"
#include "evas_gl_thread.h"

#ifdef GL_GLES

#ifdef EVAS_GL_RENDER_THREAD_IS_GENERIC



typedef struct
{
   EGLint return_value;
} Evas_Thread_Command_eglGetError;

static void
_gl_thread_eglGetError(void *data)
{
   Evas_Thread_Command_eglGetError *thread_param = data;
   thread_param->return_value = eglGetError();
}

EAPI EGLint
eglGetError_thread_cmd(void)
{
   if (!evas_gl_thread_enabled())
     return eglGetError();

   Evas_Thread_Command_eglGetError thread_param;
   evas_gl_thread_cmd_enqueue(EVAS_GL_THREAD_TYPE_GL, _gl_thread_eglGetError, &thread_param, EVAS_GL_THREAD_MODE_FINISH);
   return thread_param.return_value;
}



typedef struct
{
   EGLBoolean return_value;
   EGLenum api;
} Evas_Thread_Command_eglBindAPI;

static void
_gl_thread_eglBindAPI(void *data)
{
   Evas_Thread_Command_eglBindAPI *thread_param = data;
   thread_param->return_value = eglBindAPI(thread_param->api);
}

EAPI EGLBoolean
eglBindAPI_thread_cmd(EGLenum api)
{
   if (!evas_gl_thread_enabled())
     return eglBindAPI(api);

   Evas_Thread_Command_eglBindAPI thread_param;
   thread_param.api = api;
   evas_gl_thread_cmd_enqueue(EVAS_GL_THREAD_TYPE_GL, _gl_thread_eglBindAPI, &thread_param, EVAS_GL_THREAD_MODE_FINISH);
   return thread_param.return_value;
}



typedef struct
{
   EGLenum return_value;
} Evas_Thread_Command_eglQueryAPI;

static void
_gl_thread_eglQueryAPI(void *data)
{
   Evas_Thread_Command_eglQueryAPI *thread_param = data;
   thread_param->return_value = eglQueryAPI();
}

EAPI EGLenum
eglQueryAPI_thread_cmd(void)
{
   if (!evas_gl_thread_enabled())
     return eglQueryAPI();

   Evas_Thread_Command_eglQueryAPI thread_param;
   evas_gl_thread_cmd_enqueue(EVAS_GL_THREAD_TYPE_GL, _gl_thread_eglQueryAPI, &thread_param, EVAS_GL_THREAD_MODE_FINISH);
   return thread_param.return_value;
}



typedef struct
{
   EGLBoolean return_value;
   EGLDisplay dpy;
   EGLSurface draw;
   EGLSurface read;
   EGLContext ctx;
} Evas_Thread_Command_eglMakeCurrent;

EGLContext current_thread_ctx = EGL_NO_CONTEXT;

static void
_gl_thread_eglMakeCurrent(void *data)
{
   Evas_Thread_Command_eglMakeCurrent *thread_param = data;
   ERR("THREAD >> OTHER THREAD MAKECURRENT : (%p, %p, %p, %p)\n", thread_param->dpy, thread_param->draw, thread_param->read, thread_param->ctx);
   thread_param->return_value = eglMakeCurrent(thread_param->dpy, thread_param->draw, thread_param->read, thread_param->ctx);
   if (thread_param->return_value == EGL_TRUE)
     current_thread_ctx = thread_param->ctx;
}

EAPI EGLBoolean
eglMakeCurrent_thread_cmd(EGLDisplay dpy, EGLSurface draw, EGLSurface read, EGLContext ctx)
{
   if (!evas_gl_thread_enabled())
     return eglMakeCurrent(dpy, draw, read, ctx);

   Evas_Thread_Command_eglMakeCurrent thread_param;
   thread_param.dpy = dpy;
   thread_param.draw = draw;
   thread_param.read = read;
   thread_param.ctx = ctx;
   evas_gl_thread_cmd_enqueue(EVAS_GL_THREAD_TYPE_GL, _gl_thread_eglMakeCurrent, &thread_param, EVAS_GL_THREAD_MODE_FINISH);
   return thread_param.return_value;
}



EAPI EGLContext
eglGetCurrentContext_thread_cmd(void)
{
   if (!evas_gl_thread_enabled())
     return eglGetCurrentContext();

   /* Current thread is one per thread */
   return current_thread_ctx;
}



typedef struct
{
   EGLDisplay return_value;
   EGLint     readdraw;
} Evas_Thread_Command_eglGetCurrentSurface;

static void
_gl_thread_eglGetCurrentSurface(void *data)
{
   Evas_Thread_Command_eglGetCurrentSurface *thread_param = data;
   thread_param->return_value = eglGetCurrentSurface(thread_param->readdraw);
}

EAPI EGLSurface
eglGetCurrentSurface_thread_cmd(EGLint readdraw)
{
   if (!evas_gl_thread_enabled())
     return eglGetCurrentSurface(readdraw);

   Evas_Thread_Command_eglGetCurrentSurface thread_param;
   thread_param.readdraw = readdraw;
   evas_gl_thread_cmd_enqueue(EVAS_GL_THREAD_TYPE_GL, _gl_thread_eglGetCurrentSurface, &thread_param, EVAS_GL_THREAD_MODE_FINISH);
   return thread_param.return_value;
}



typedef struct
{
   EGLDisplay return_value;
} Evas_Thread_Command_eglGetCurrentDisplay;

static void
_gl_thread_eglGetCurrentDisplay(void *data)
{
   Evas_Thread_Command_eglGetCurrentDisplay *thread_param = data;
   thread_param->return_value = eglGetCurrentDisplay();
}

EAPI EGLDisplay
eglGetCurrentDisplay_thread_cmd(void)
{
   if (!evas_gl_thread_enabled())
     return eglGetCurrentDisplay();

   Evas_Thread_Command_eglGetCurrentDisplay thread_param;
   evas_gl_thread_cmd_enqueue(EVAS_GL_THREAD_TYPE_GL, _gl_thread_eglGetCurrentDisplay, &thread_param, EVAS_GL_THREAD_MODE_FINISH);
   return thread_param.return_value;
}



typedef struct
{
   EGLBoolean return_value;
} Evas_Thread_Command_eglReleaseThread;

static void
_gl_thread_eglReleaseThread(void *data)
{
   Evas_Thread_Command_eglReleaseThread *thread_param = data;
   thread_param->return_value = eglReleaseThread();
}

EAPI EGLBoolean
eglReleaseThread_thread_cmd(void)
{
   if (!evas_gl_thread_enabled())
     return eglReleaseThread();

   Evas_Thread_Command_eglReleaseThread thread_param;
   evas_gl_thread_cmd_enqueue(EVAS_GL_THREAD_TYPE_GL, _gl_thread_eglReleaseThread, &thread_param, EVAS_GL_THREAD_MODE_FINISH);
   return thread_param.return_value;
}



typedef struct
{
   EGLBoolean return_value;
   EGLDisplay dpy;
   EGLSurface surface;
   EGLint attribute;
   EGLint *value;
} Evas_Thread_Command_eglQuerySurface;

static void
_gl_thread_eglQuerySurface(void *data)
{
   Evas_Thread_Command_eglQuerySurface *thread_param = data;
   thread_param->return_value = eglQuerySurface(thread_param->dpy, thread_param->surface, thread_param->attribute, thread_param->value);
}

EAPI EGLBoolean
eglQuerySurface_thread_cmd(EGLDisplay dpy, EGLSurface surface, EGLint attribute, EGLint *value)
{
   if (!evas_gl_thread_enabled())
     return eglQuerySurface(dpy, surface, attribute, value);

   Evas_Thread_Command_eglQuerySurface thread_param;
   thread_param.dpy = dpy;
   thread_param.surface = surface;
   thread_param.attribute = attribute;
   thread_param.value = value;
   evas_gl_thread_cmd_enqueue(EVAS_GL_THREAD_TYPE_GL, _gl_thread_eglQuerySurface, &thread_param, EVAS_GL_THREAD_MODE_FINISH);
   return thread_param.return_value;
}



typedef struct
{
   EGLBoolean return_value;
   EGLDisplay dpy;
   EGLint interval;
} Evas_Thread_Command_eglSwapInterval;

static void
_gl_thread_eglSwapInterval(void *data)
{
   Evas_Thread_Command_eglSwapInterval *thread_param = data;
   thread_param->return_value = eglSwapInterval(thread_param->dpy, thread_param->interval);
}

EAPI EGLBoolean
eglSwapInterval_thread_cmd(EGLDisplay dpy, EGLint interval)
{
   if (!evas_gl_thread_enabled())
     return eglSwapInterval(dpy, interval);

   Evas_Thread_Command_eglSwapInterval thread_param;
   thread_param.dpy = dpy;
   thread_param.interval = interval;
   evas_gl_thread_cmd_enqueue(EVAS_GL_THREAD_TYPE_GL, _gl_thread_eglSwapInterval, &thread_param, EVAS_GL_THREAD_MODE_FINISH);
   return thread_param.return_value;
}



typedef struct
{
   EGLBoolean return_value;
} Evas_Thread_Command_eglWaitGL;

static void
_gl_thread_eglWaitGL(void *data)
{
   Evas_Thread_Command_eglWaitGL *thread_param = data;
   thread_param->return_value = eglWaitGL();
}

EAPI EGLBoolean
eglWaitGL_thread_cmd(void)
{
   if (!evas_gl_thread_enabled())
     return eglWaitGL();

   Evas_Thread_Command_eglWaitGL thread_param;
   evas_gl_thread_cmd_enqueue(EVAS_GL_THREAD_TYPE_GL, _gl_thread_eglWaitGL, &thread_param, EVAS_GL_THREAD_MODE_FINISH);
   return thread_param.return_value;
}



typedef struct
{
   EGLBoolean return_value;
   EGLDisplay dpy;
   EGLSurface surface;
} Evas_Thread_Command_eglSwapBuffers;

static void
_gl_thread_eglSwapBuffers(void *data)
{
   Evas_Thread_Command_eglSwapBuffers *thread_param = data;
   thread_param->return_value = eglSwapBuffers(thread_param->dpy, thread_param->surface);
}

EAPI EGLBoolean
eglSwapBuffers_thread_cmd(EGLDisplay dpy, EGLSurface surface)
{
   if (!evas_gl_thread_enabled())
     return eglSwapBuffers(dpy, surface);

   Evas_Thread_Command_eglSwapBuffers thread_param;
   thread_param.dpy = dpy;
   thread_param.surface = surface;
   evas_gl_thread_cmd_enqueue(EVAS_GL_THREAD_TYPE_GL, _gl_thread_eglSwapBuffers, &thread_param, EVAS_GL_THREAD_MODE_FINISH);
   return thread_param.return_value;
}



typedef struct
{
   EGLBoolean return_value;
   EGLDisplay dpy;
   EGLSurface surface;
   EGLint *rects;
   EGLint n_rects;
} Evas_Thread_Command_eglSwapBuffersWithDamage;

EGLBoolean (*orig_evas_eglSwapBuffersWithDamage)(EGLDisplay dpy, EGLSurface surface, EGLint *rects, EGLint n_rects);

EAPI void
eglSwapBuffersWithDamage_orig_evas_set(void *func)
{
   orig_evas_eglSwapBuffersWithDamage = func;
}

EAPI void *
eglSwapBuffersWithDamage_orig_evas_get()
{
   return orig_evas_eglSwapBuffersWithDamage;
}

static void
_gl_thread_eglSwapBuffersWithDamage(void *data)
{
   Evas_Thread_Command_eglSwapBuffersWithDamage *thread_param = data;
   thread_param->return_value = orig_evas_eglSwapBuffersWithDamage(thread_param->dpy, thread_param->surface, thread_param->rects, thread_param->n_rects);
}

EAPI EGLBoolean
eglSwapBuffersWithDamage_thread_cmd(EGLDisplay dpy, EGLSurface surface, EGLint *rects, EGLint n_rects)
{
   if (!evas_gl_thread_enabled())
     return orig_evas_eglSwapBuffersWithDamage(dpy, surface, rects, n_rects);

   Evas_Thread_Command_eglSwapBuffersWithDamage thread_param;
   thread_param.dpy = dpy;
   thread_param.surface = surface;
   thread_param.rects = rects;
   thread_param.n_rects = n_rects;
   evas_gl_thread_cmd_enqueue(EVAS_GL_THREAD_TYPE_GL, _gl_thread_eglSwapBuffersWithDamage, &thread_param, EVAS_GL_THREAD_MODE_FINISH);
   return thread_param.return_value;
}



typedef struct
{
   EGLBoolean return_value;
   EGLDisplay dpy;
   EGLSurface surface;
   EGLint *rects;
   EGLint n_rects;
} Evas_Thread_Command_eglSetDamageRegion;

EGLBoolean (*orig_evas_eglSetDamageRegion)(EGLDisplay dpy, EGLSurface surface, EGLint *rects, EGLint n_rects);

EAPI void
eglSetDamageRegion_orig_evas_set(void *func)
{
   orig_evas_eglSetDamageRegion = func;
}

EAPI void *
eglSetDamageRegion_orig_evas_get()
{
   return orig_evas_eglSetDamageRegion;
}

static void
_gl_thread_eglSetDamageRegion(void *data)
{
   Evas_Thread_Command_eglSetDamageRegion *thread_param = data;
   thread_param->return_value = orig_evas_eglSetDamageRegion(thread_param->dpy, thread_param->surface, thread_param->rects, thread_param->n_rects);
}

EAPI EGLBoolean
eglSetDamageRegion_thread_cmd(EGLDisplay dpy, EGLSurface surface, EGLint *rects, EGLint n_rects)
{
   if (!evas_gl_thread_enabled())
     return orig_evas_eglSetDamageRegion(dpy, surface, rects, n_rects);

   Evas_Thread_Command_eglSetDamageRegion thread_param;
   thread_param.dpy = dpy;
   thread_param.surface = surface;
   thread_param.rects = rects;
   thread_param.n_rects = n_rects;
   evas_gl_thread_cmd_enqueue(EVAS_GL_THREAD_TYPE_GL, _gl_thread_eglSetDamageRegion, &thread_param, EVAS_GL_THREAD_MODE_FINISH);
   return thread_param.return_value;
}



typedef struct
{
   EGLBoolean return_value;
   EGLDisplay dpy;
   void *buffer;
   EGLint attribute;
   EGLint *value;
} Evas_Thread_Command_eglQueryWaylandBuffer;

EGLBoolean (*orig_evas_eglQueryWaylandBuffer)(EGLDisplay dpy, void *buffer, EGLint attribute, EGLint *value);

EAPI void
eglQueryWaylandBuffer_orig_evas_set(void *func)
{
   orig_evas_eglQueryWaylandBuffer = func;
}

EAPI void *
eglQueryWaylandBuffer_orig_evas_get()
{
   return orig_evas_eglQueryWaylandBuffer;
}

static void
_gl_thread_eglQueryWaylandBuffer(void *data)
{
   Evas_Thread_Command_eglQueryWaylandBuffer *thread_param = data;
   thread_param->return_value = orig_evas_eglQueryWaylandBuffer(thread_param->dpy, thread_param->buffer, thread_param->attribute, thread_param->value);
}

EAPI EGLBoolean
eglQueryWaylandBuffer_thread_cmd(EGLDisplay dpy, void *buffer, EGLint attribute, EGLint *value)
{
   if (!evas_gl_thread_enabled())
     return orig_evas_eglQueryWaylandBuffer(dpy, buffer, attribute, value);

   Evas_Thread_Command_eglQueryWaylandBuffer thread_param;
   thread_param.dpy = dpy;
   thread_param.buffer = buffer;
   thread_param.attribute = attribute;
   thread_param.value = value;
   evas_gl_thread_cmd_enqueue(EVAS_GL_THREAD_TYPE_GL, _gl_thread_eglQueryWaylandBuffer, &thread_param, EVAS_GL_THREAD_MODE_FINISH);
   return thread_param.return_value;
}



/***** EVAS GL *****/

typedef struct
{
   EGLint return_value;
} Evas_GL_Thread_Command_eglGetError;

static void
_evgl_thread_eglGetError(void *data)
{
   Evas_GL_Thread_Command_eglGetError *thread_param = data;
   thread_param->return_value = eglGetError();
}

EAPI EGLint
eglGetError_evgl_thread_cmd(void)
{
   if (!evas_evgl_thread_enabled())
     return eglGetError();

   Evas_GL_Thread_Command_eglGetError thread_param;
   evas_gl_thread_cmd_enqueue(EVAS_GL_THREAD_TYPE_EVGL, _evgl_thread_eglGetError, &thread_param, EVAS_GL_THREAD_MODE_FINISH);
   return thread_param.return_value;
}



EGLContext current_evgl_thread_context = EGL_NO_CONTEXT;


typedef struct
{
   EGLBoolean return_value;
   EGLDisplay dpy;
   EGLSurface draw;
   EGLSurface read;
   EGLContext ctx;
} Evas_GL_Thread_Command_eglMakeCurrent;

static void
_evgl_thread_eglMakeCurrent(void *data)
{
   Evas_GL_Thread_Command_eglMakeCurrent *thread_param = data;
   ERR("THREAD >> EVGL OTHER THREAD MAKECURRENT : (%p, %p, %p, %p)\n", thread_param->dpy, thread_param->draw, thread_param->read, thread_param->ctx);
   thread_param->return_value = eglMakeCurrent(thread_param->dpy, thread_param->draw, thread_param->read, thread_param->ctx);
   if (thread_param->return_value == EGL_TRUE)
	   current_evgl_thread_context = thread_param->ctx;
}

EAPI EGLBoolean
eglMakeCurrent_evgl_thread_cmd(EGLDisplay dpy, EGLSurface draw, EGLSurface read, EGLContext ctx)
{
   if (!evas_evgl_thread_enabled())
     {
        if (eina_thread_self() == evas_gl_thread_get(EVAS_GL_THREAD_TYPE_EVGL))
           ERR("THREAD >> EVGL OTHER THREAD MAKECURRENT(INNER) : (%p, %p, %p, %p)\n", dpy, draw, read, ctx);
        return eglMakeCurrent(dpy, draw, read, ctx);
     }

   Evas_GL_Thread_Command_eglMakeCurrent thread_param;
   thread_param.dpy = dpy;
   thread_param.draw = draw;
   thread_param.read = read;
   thread_param.ctx = ctx;
   evas_gl_thread_cmd_enqueue(EVAS_GL_THREAD_TYPE_EVGL, _evgl_thread_eglMakeCurrent, &thread_param, EVAS_GL_THREAD_MODE_FINISH);
   return thread_param.return_value;
}



typedef struct
{
   EGLContext return_value;
} Evas_GL_Thread_Command_eglGetCurrentContext;

static void
_evgl_thread_eglGetCurrentContext(void *data)
{
   Evas_GL_Thread_Command_eglGetCurrentContext *thread_param = data;
   thread_param->return_value = eglGetCurrentContext();
}

EAPI EGLContext
eglGetCurrentContext_evgl_thread_cmd(void)
{
   if (!evas_evgl_thread_enabled())
     return eglGetCurrentContext();

   return current_evgl_thread_context;

   Evas_GL_Thread_Command_eglGetCurrentContext thread_param;
   evas_gl_thread_cmd_enqueue(EVAS_GL_THREAD_TYPE_EVGL, _evgl_thread_eglGetCurrentContext, &thread_param, EVAS_GL_THREAD_MODE_FINISH);
   return thread_param.return_value;
}



typedef struct
{
   EGLDisplay return_value;
   EGLint     readdraw;
} Evas_GL_Thread_Command_eglGetCurrentSurface;

static void
_evgl_thread_eglGetCurrentSurface(void *data)
{
   Evas_GL_Thread_Command_eglGetCurrentSurface *thread_param = data;
   thread_param->return_value = eglGetCurrentSurface(thread_param->readdraw);
}

EAPI EGLSurface
eglGetCurrentSurface_evgl_thread_cmd(EGLint readdraw)
{
   if (!evas_evgl_thread_enabled())
     return eglGetCurrentSurface(readdraw);

   Evas_GL_Thread_Command_eglGetCurrentSurface thread_param;
   thread_param.readdraw = readdraw;
   evas_gl_thread_cmd_enqueue(EVAS_GL_THREAD_TYPE_EVGL, _evgl_thread_eglGetCurrentSurface, &thread_param, EVAS_GL_THREAD_MODE_FINISH);
   return thread_param.return_value;
}



typedef struct
{
   EGLDisplay return_value;
} Evas_GL_Thread_Command_eglGetCurrentDisplay;

static void
_evgl_thread_eglGetCurrentDisplay(void *data)
{
   Evas_GL_Thread_Command_eglGetCurrentDisplay *thread_param = data;
   thread_param->return_value = eglGetCurrentDisplay();
}

EAPI EGLDisplay
eglGetCurrentDisplay_evgl_thread_cmd(void)
{
   if (!evas_evgl_thread_enabled())
     return eglGetCurrentDisplay();

   Evas_GL_Thread_Command_eglGetCurrentDisplay thread_param;
   evas_gl_thread_cmd_enqueue(EVAS_GL_THREAD_TYPE_EVGL, _evgl_thread_eglGetCurrentDisplay, &thread_param, EVAS_GL_THREAD_MODE_FINISH);
   return thread_param.return_value;
}



#else /* EVAS_GL_RENDER_THREAD_IS_GENERIC */


#include <dlfcn.h>

EGLint (*eglGetError_thread_cmd)();
EGLBoolean (*eglBindAPI_thread_cmd)(EGLenum api);
EGLenum (*eglQueryAPI_thread_cmd)();
EGLBoolean (*eglMakeCurrent_thread_cmd)(EGLDisplay dpy, EGLSurface draw, EGLSurface read, EGLContext ctx);
EGLContext (*eglGetCurrentContext_thread_cmd)(void);
EGLSurface (*eglGetCurrentSurface_thread_cmd)(EGLint readdraw);
EGLDisplay (*eglGetCurrentDisplay_thread_cmd)(void);
EGLBoolean (*eglReleaseThread_thread_cmd)();

EGLBoolean (*eglQuerySurface_thread_cmd)(EGLDisplay dpy, EGLSurface surface, EGLint attribute, EGLint *value);
EGLBoolean (*eglSwapInterval_thread_cmd)(EGLDisplay dpy, EGLint interval);
EGLBoolean (*eglWaitGL_thread_cmd)(void);
EGLBoolean (*eglSwapBuffers_thread_cmd)(EGLDisplay dpy, EGLSurface surface);

void (*eglSwapBuffersWithDamage_orig_evas_set)(void *func);
void *(*eglSwapBuffersWithDamage_orig_evas_get)();
EGLBoolean (*eglSwapBuffersWithDamage_thread_cmd)(EGLDisplay dpy, EGLSurface surface, EGLint *rects, EGLint n_rects);

void (*eglSetDamageRegion_orig_evas_set)(void *func);
void *(*eglSetDamageRegion_orig_evas_get)();
EGLBoolean (*eglSetDamageRegion_thread_cmd)(EGLDisplay dpy, EGLSurface surface, EGLint *rects, EGLint n_rects);

void (*eglQueryWaylandBuffer_orig_evas_set)(void *func);
void *(*eglQueryWaylandBuffer_orig_evas_get)();
EGLBoolean (*eglQueryWaylandBuffer_thread_cmd)(EGLDisplay dpy, void *buffer, EGLint attribute, EGLint *value);


/***** EVAS GL *****/

EGLint (*eglGetError_evgl_thread_cmd)();
EGLBoolean (*eglMakeCurrent_evgl_thread_cmd)(EGLDisplay dpy, EGLSurface draw, EGLSurface read, EGLContext ctx);
EGLContext (*eglGetCurrentContext_evgl_thread_cmd)(void);
EGLSurface (*eglGetCurrentSurface_evgl_thread_cmd)(EGLint readdraw);
EGLDisplay (*eglGetCurrentDisplay_evgl_thread_cmd)(void);


void _egl_thread_link_init()
{
#define LINK2GENERIC(sym) \
   sym = dlsym(RTLD_DEFAULT, #sym); \
   if (!sym) ERR("Could not find function '%s'", #sym);

   // Get function pointer to evas_gl_common that is now provided through the link of GL_Generic.
   LINK2GENERIC(eglGetError_thread_cmd);
   LINK2GENERIC(eglBindAPI_thread_cmd);
   LINK2GENERIC(eglQueryAPI_thread_cmd);
   LINK2GENERIC(eglMakeCurrent_thread_cmd);
   LINK2GENERIC(eglGetCurrentContext_thread_cmd);
   LINK2GENERIC(eglGetCurrentSurface_thread_cmd);
   LINK2GENERIC(eglGetCurrentDisplay_thread_cmd);
   LINK2GENERIC(eglReleaseThread_thread_cmd);

   LINK2GENERIC(eglQuerySurface_thread_cmd);
   LINK2GENERIC(eglSwapInterval_thread_cmd);
   LINK2GENERIC(eglWaitGL_thread_cmd);
   LINK2GENERIC(eglSwapBuffers_thread_cmd);

   LINK2GENERIC(eglSwapBuffersWithDamage_orig_evas_set);
   LINK2GENERIC(eglSwapBuffersWithDamage_orig_evas_get);
   LINK2GENERIC(eglSwapBuffersWithDamage_thread_cmd);

   LINK2GENERIC(eglSetDamageRegion_orig_evas_set);
   LINK2GENERIC(eglSetDamageRegion_orig_evas_get);
   LINK2GENERIC(eglSetDamageRegion_thread_cmd);

   LINK2GENERIC(eglQueryWaylandBuffer_orig_evas_set);
   LINK2GENERIC(eglQueryWaylandBuffer_orig_evas_get);
   LINK2GENERIC(eglQueryWaylandBuffer_thread_cmd);

   /***** EVAS GL *****/

   LINK2GENERIC(eglGetError_evgl_thread_cmd);
   LINK2GENERIC(eglMakeCurrent_evgl_thread_cmd);
   LINK2GENERIC(eglGetCurrentContext_evgl_thread_cmd);
   LINK2GENERIC(eglGetCurrentSurface_evgl_thread_cmd);
   LINK2GENERIC(eglGetCurrentDisplay_evgl_thread_cmd);
}

#endif /* EVAS_GL_RENDER_THREAD_IS_GENERIC */

#endif
