#include "evas_gl_thread.h"


#ifdef GL_GLES

#define EVAS_TH_EGL_FN(ret, name, ...) \
    ret (*GL_TH_FN(name))(GL_TH_DP,  ##__VA_ARGS__);
#define EVAS_TH_EGL_FN_ASYNC(ret, name, ...) \
    void *(*GL_TH_FN(name##_begin))(GL_TH_DP,  ##__VA_ARGS__); \
    ret (*GL_TH_FN(name##_end))(void *ref);

typedef struct
{
  EVAS_TH_EGL_FN_LIST
} Evas_GL_Thread_EGL_Func;

#undef EVAS_TH_EGL_FN_ASYNC
#undef EVAS_TH_EGL_FN

#define EVAS_TH_EGL_FN(ret, name, ...) \
    typedef ret (*GL_TH_FNTYPE(name))(__VA_ARGS__);
#define EVAS_TH_EGL_FN_ASYNC(ret, name, ...) \
    typedef void *(*GL_TH_FNTYPE(name##_begin))(__VA_ARGS__); \
    typedef ret (*GL_TH_FNTYPE(name##_end))(void *ref);

EVAS_TH_EGL_FN_LIST

#undef EVAS_TH_EGL_FN_ASYNC
#undef EVAS_TH_EGL_FN


#ifdef EVAS_GL_RENDER_THREAD_COMPILE_FOR_GL_GENERIC


typedef struct
{
  GL_TH_FNTYPE(eglGetError) orig_func;
  EGLint return_value;
} GL_TH_ST(eglGetError);

static void
GL_TH_CB(eglGetError)(void *data)
{
  GL_TH_ST(eglGetError) *thread_data = (*(void **)data);

  thread_data->return_value =
      thread_data->orig_func();
}

EGLint
GL_TH_FN(eglGetError)(GL_TH_DP)
{
  GL_TH_ST(eglGetError) thread_data_local, *thread_data = &thread_data_local, **thread_data_ptr;
  void *thcmd_ref;

  if (!evas_gl_thread_enabled(thread_type))
    return ((GL_TH_FNTYPE(eglGetError))orig_func)();

  thread_data_ptr =
      evas_gl_thread_cmd_create(thread_type, sizeof(GL_TH_ST(eglGetError) *), &thcmd_ref);
  *thread_data_ptr = thread_data;

  thread_data->orig_func = orig_func;

  evas_gl_thread_cmd_enqueue(thcmd_ref,
                             GL_TH_CB(eglGetError),
                             EVAS_GL_THREAD_MODE_FINISH);

  return thread_data->return_value;
}



typedef struct
{
  GL_TH_FNTYPE(eglBindAPI) orig_func;
  EGLBoolean return_value;
  EGLenum api;
} GL_TH_ST(eglBindAPI);

static void
GL_TH_CB(eglBindAPI)(void *data)
{
  GL_TH_ST(eglBindAPI) *thread_data = *(void **)data;

  thread_data->return_value =
      thread_data->orig_func(thread_data->api);
}

EGLBoolean
GL_TH_FN(eglBindAPI)(GL_TH_DP, EGLenum api)
{
  GL_TH_ST(eglBindAPI) thread_data_local, *thread_data = &thread_data_local, **thread_data_ptr;
  void *thcmd_ref;

  if (!evas_gl_thread_enabled(thread_type))
    return ((GL_TH_FNTYPE(eglBindAPI))orig_func)(api);

  thread_data_ptr =
      evas_gl_thread_cmd_create(thread_type, sizeof(GL_TH_ST(eglBindAPI) *), &thcmd_ref);
  *thread_data_ptr = thread_data;

  thread_data->api = api;
  thread_data->orig_func = orig_func;

  evas_gl_thread_cmd_enqueue(thcmd_ref,
                             GL_TH_CB(eglBindAPI),
                             EVAS_GL_THREAD_MODE_FINISH);

  return thread_data->return_value;
}



typedef struct
{
  GL_TH_FNTYPE(eglQueryAPI) orig_func;
  EGLenum return_value;
} GL_TH_ST(eglQueryAPI);

static void
GL_TH_CB(eglQueryAPI)(void *data)
{
  GL_TH_ST(eglQueryAPI) *thread_data = *(void **)data;

  thread_data->return_value =
      thread_data->orig_func();
}

EGLenum
GL_TH_FN(eglQueryAPI)(GL_TH_DP)
{
  GL_TH_ST(eglQueryAPI) thread_data_local, *thread_data = &thread_data_local, **thread_data_ptr;
  void *thcmd_ref;

  if (!evas_gl_thread_enabled(thread_type))
    return ((GL_TH_FNTYPE(eglQueryAPI))orig_func)();

  thread_data_ptr =
      evas_gl_thread_cmd_create(thread_type, sizeof(GL_TH_ST(eglQueryAPI) *), &thcmd_ref);
  *thread_data_ptr = thread_data;

  thread_data->orig_func = orig_func;

  evas_gl_thread_cmd_enqueue(thcmd_ref,
                             GL_TH_CB(eglQueryAPI),
                             EVAS_GL_THREAD_MODE_FINISH);

  return thread_data->return_value;
}



typedef struct
{
  GL_TH_FNTYPE(eglMakeCurrent) orig_func;
  EGLBoolean return_value;
  EGLDisplay dpy;
  EGLSurface draw;
  EGLSurface read;
  EGLContext ctx;
} GL_TH_ST(eglMakeCurrent);

EGLDisplay current_thread_dpy  = EGL_NO_DISPLAY;
EGLSurface current_thread_draw = EGL_NO_SURFACE;
EGLSurface current_thread_read = EGL_NO_SURFACE;
EGLContext current_thread_ctx  = EGL_NO_CONTEXT;

static void
GL_TH_CB(eglMakeCurrent)(void *data)
{
  GL_TH_ST(eglMakeCurrent) *thread_data = *(void **)data;
  DBG("THREAD >> OTHER THREAD MAKECURRENT : (%p, %p, %p, %p)\n", thread_data->dpy,
      thread_data->draw, thread_data->read, thread_data->ctx);

  thread_data->return_value =
      thread_data->orig_func(thread_data->dpy,
                             thread_data->draw,
                             thread_data->read,
                             thread_data->ctx);

  if (thread_data->return_value == EGL_TRUE)
    {
      current_thread_dpy = thread_data->dpy;
      current_thread_draw = thread_data->draw;
      current_thread_read = thread_data->read;
      current_thread_ctx = thread_data->ctx;
    }
}

EGLBoolean
GL_TH_FN(eglMakeCurrent)(GL_TH_DP, EGLDisplay dpy, EGLSurface draw, EGLSurface read, EGLContext ctx)
{
  GL_TH_ST(eglMakeCurrent) thread_data_local, *thread_data = &thread_data_local, **thread_data_ptr;
  void *thcmd_ref;

  if (!evas_gl_thread_enabled(thread_type))
    return ((GL_TH_FNTYPE(eglMakeCurrent))orig_func)(dpy, draw, read, ctx);

  /* Skip for noop make-current */
  if (current_thread_dpy == dpy &&
      current_thread_draw == draw &&
      current_thread_read == read &&
      current_thread_ctx == ctx)
    return EGL_TRUE;

  thread_data_ptr =
      evas_gl_thread_cmd_create(thread_type, sizeof(GL_TH_ST(eglMakeCurrent) *), &thcmd_ref);
  *thread_data_ptr = thread_data;

  thread_data->dpy = dpy;
  thread_data->draw = draw;
  thread_data->read = read;
  thread_data->ctx = ctx;
  thread_data->orig_func = orig_func;

  evas_gl_thread_cmd_enqueue(thcmd_ref,
                             GL_TH_CB(eglMakeCurrent),
                             EVAS_GL_THREAD_MODE_FINISH);

  return thread_data->return_value;
}



EGLContext
GL_TH_FN(eglGetCurrentContext)(GL_TH_DP)
{
  if (!evas_gl_thread_enabled(thread_type))
    return ((GL_TH_FNTYPE(eglGetCurrentContext))orig_func)();

  /* Current context is completely tracked by this variable */
  return current_thread_ctx;
}


EGLSurface
GL_TH_FN(eglGetCurrentSurface)(GL_TH_DP, EGLint readdraw)
{
  if (!evas_gl_thread_enabled(thread_type))
    return ((GL_TH_FNTYPE(eglGetCurrentSurface))orig_func)(readdraw);

  /* Current surfaces are completely tracked by this variable */
  if (readdraw == EGL_READ)
    return current_thread_read;
  else if (readdraw == EGL_DRAW)
    return current_thread_draw;
  else
    return NULL;
}


EGLDisplay
GL_TH_FN(eglGetCurrentDisplay)(GL_TH_DP)
{
  if (!evas_gl_thread_enabled(thread_type))
    return ((GL_TH_FNTYPE(eglGetCurrentDisplay))orig_func)();

  /* Current display is completely tracked by this variable */
  return current_thread_dpy;
}

typedef struct
{
  GL_TH_FNTYPE(eglReleaseThread) orig_func;
  EGLBoolean return_value;
} GL_TH_ST(eglReleaseThread);

static void
GL_TH_CB(eglReleaseThread)(void *data)
{
  GL_TH_ST(eglReleaseThread) *thread_data = *(void **)data;

  thread_data->return_value =
      thread_data->orig_func();
}

EGLBoolean
GL_TH_FN(eglReleaseThread)(GL_TH_DP)
{
  GL_TH_ST(eglReleaseThread) thread_data_local, *thread_data = &thread_data_local, **thread_data_ptr;
  void *thcmd_ref;

  if (!evas_gl_thread_enabled(thread_type))
    return ((GL_TH_FNTYPE(eglReleaseThread))orig_func)();

  thread_data_ptr =
      evas_gl_thread_cmd_create(thread_type, sizeof(GL_TH_ST(eglReleaseThread) *), &thcmd_ref);
  *thread_data_ptr = thread_data;

  thread_data->orig_func = orig_func;

  evas_gl_thread_cmd_enqueue(thcmd_ref,
                             GL_TH_CB(eglReleaseThread),
                             EVAS_GL_THREAD_MODE_FINISH);

  return thread_data->return_value;
}



typedef struct
{
  GL_TH_FNTYPE(eglQuerySurface) orig_func;
  EGLBoolean return_value;
  EGLDisplay dpy;
  EGLSurface surface;
  EGLint attribute;
  EGLint *value;
} GL_TH_ST(eglQuerySurface);

static void
GL_TH_CB(eglQuerySurface)(void *data)
{
  GL_TH_ST(eglQuerySurface) *thread_data = *(void **)data;

  thread_data->return_value =
      thread_data->orig_func(thread_data->dpy,
                             thread_data->surface,
                             thread_data->attribute,
                             thread_data->value);
}

EGLBoolean
GL_TH_FN(eglQuerySurface)(GL_TH_DP, EGLDisplay dpy, EGLSurface surface, EGLint attribute, EGLint *value)
{
  GL_TH_ST(eglQuerySurface) thread_data_local, *thread_data = &thread_data_local, **thread_data_ptr;
  void *thcmd_ref;

  if (!evas_gl_thread_enabled(thread_type))
    return ((GL_TH_FNTYPE(eglQuerySurface))orig_func)(dpy, surface, attribute, value);

  thread_data_ptr =
      evas_gl_thread_cmd_create(thread_type, sizeof(GL_TH_ST(eglQuerySurface) *), &thcmd_ref);
  *thread_data_ptr = thread_data;

  thread_data->dpy = dpy;
  thread_data->surface = surface;
  thread_data->attribute = attribute;
  thread_data->value =value;
  thread_data->orig_func = orig_func;

  evas_gl_thread_cmd_enqueue(thcmd_ref,
                             GL_TH_CB(eglQuerySurface),
                             EVAS_GL_THREAD_MODE_FINISH);

  return thread_data->return_value;
}



typedef struct
{
  GL_TH_FNTYPE(eglSwapInterval) orig_func;
  EGLBoolean return_value;
  EGLDisplay dpy;
  EGLint interval;
} GL_TH_ST(eglSwapInterval);

static void
GL_TH_CB(eglSwapInterval)(void *data)
{
  GL_TH_ST(eglSwapInterval) *thread_data = *(void **)data;

  thread_data->return_value =
      thread_data->orig_func(thread_data->dpy,
                             thread_data->interval);
}

EGLBoolean
GL_TH_FN(eglSwapInterval)(GL_TH_DP, EGLDisplay dpy, EGLint interval)
{
  GL_TH_ST(eglSwapInterval) thread_data_local, *thread_data = &thread_data_local, **thread_data_ptr;
  void *thcmd_ref;

  if (!evas_gl_thread_enabled(thread_type))
    return ((GL_TH_FNTYPE(eglSwapInterval))orig_func)(dpy, interval);

  thread_data_ptr =
      evas_gl_thread_cmd_create(thread_type, sizeof(GL_TH_ST(eglSwapInterval) *), &thcmd_ref);
  *thread_data_ptr = thread_data;

  thread_data->dpy = dpy;
  thread_data->interval = interval;
  thread_data->orig_func = orig_func;

  evas_gl_thread_cmd_enqueue(thcmd_ref,
                             GL_TH_CB(eglSwapInterval),
                             EVAS_GL_THREAD_MODE_FINISH);

  return thread_data->return_value;
}



typedef struct
{
  GL_TH_FNTYPE(eglWaitGL) orig_func;
  EGLBoolean return_value;
} GL_TH_ST(eglWaitGL);

static void
GL_TH_CB(eglWaitGL)(void *data)
{
  GL_TH_ST(eglWaitGL) *thread_data = *(void **)data;

  thread_data->return_value =
      thread_data->orig_func();
}

EGLBoolean
GL_TH_FN(eglWaitGL)(GL_TH_DP)
{
  GL_TH_ST(eglWaitGL) thread_data_local, *thread_data = &thread_data_local, **thread_data_ptr;
  void *thcmd_ref;

  if (!evas_gl_thread_enabled(thread_type))
    return ((GL_TH_FNTYPE(eglWaitGL))orig_func)();

  thread_data_ptr =
      evas_gl_thread_cmd_create(thread_type, sizeof(GL_TH_ST(eglWaitGL) *), &thcmd_ref);
  *thread_data_ptr = thread_data;

  thread_data->orig_func = orig_func;

  evas_gl_thread_cmd_enqueue(thcmd_ref,
                             GL_TH_CB(eglWaitGL),
                             EVAS_GL_THREAD_MODE_FINISH);

  return thread_data->return_value;
}



typedef struct
{
  GL_TH_FNTYPE(eglSwapBuffers) orig_func;
  EGLBoolean return_value;
  EGLDisplay dpy;
  EGLSurface surface;
} GL_TH_ST(eglSwapBuffers);

static void
GL_TH_CB(eglSwapBuffers)(void *data)
{
  GL_TH_ST(eglSwapBuffers) *thread_data = *(void **)data;

  thread_data->return_value =
      thread_data->orig_func(thread_data->dpy,
                             thread_data->surface);
}

EGLBoolean
GL_TH_FN(eglSwapBuffers)(GL_TH_DP, EGLDisplay dpy, EGLSurface surface)
{
  GL_TH_ST(eglSwapBuffers) thread_data_local, *thread_data = &thread_data_local, **thread_data_ptr;
  void *thcmd_ref;

  if (!evas_gl_thread_enabled(thread_type))
    return ((GL_TH_FNTYPE(eglSwapBuffers))orig_func)(dpy, surface);

  thread_data_ptr =
      evas_gl_thread_cmd_create(thread_type, sizeof(GL_TH_ST(eglSwapBuffers) *) + sizeof(GL_TH_ST(eglSwapBuffers)), &thcmd_ref);
  *thread_data_ptr = (void *)((char *)thread_data_ptr + sizeof(GL_TH_ST(eglSwapBuffers) *));
  thread_data = *thread_data_ptr;

  int thread_mode = EVAS_GL_THREAD_MODE_FINISH;
  if (!evas_gl_thread_force_finish())
    thread_mode = EVAS_GL_THREAD_MODE_FLUSH;

  thread_data->dpy = dpy;
  thread_data->surface = surface;
  thread_data->orig_func = orig_func;

  evas_gl_thread_cmd_enqueue(thcmd_ref,
                             GL_TH_CB(eglSwapBuffers),
                             thread_mode);

  return thread_data->return_value;
}



typedef struct
{
  GL_TH_FNTYPE(eglSwapBuffersWithDamage) orig_func;
  EGLBoolean return_value;
  EGLDisplay dpy;
  EGLSurface surface;
  EGLint *rects;
  EGLint n_rects;
} GL_TH_ST(eglSwapBuffersWithDamage);

static void
GL_TH_CB(eglSwapBuffersWithDamage)(void *data)
{
  GL_TH_ST(eglSwapBuffersWithDamage) *thread_data = *(void **)data;

  thread_data->return_value =
      thread_data->orig_func(thread_data->dpy,
                             thread_data->surface,
                             thread_data->rects,
                             thread_data->n_rects);
}


EGLBoolean
GL_TH_FN(eglSwapBuffersWithDamage)(GL_TH_DP, EGLDisplay dpy, EGLSurface surface, EGLint *rects, EGLint n_rects)
{
  GL_TH_ST(eglSwapBuffersWithDamage) thread_data_local, *thread_data = &thread_data_local, **thread_data_ptr;
  void *thcmd_ref;

  if (!evas_gl_thread_enabled(thread_type))
    return ((GL_TH_FNTYPE(eglSwapBuffersWithDamage))orig_func)(dpy, surface, rects, n_rects);

  thread_data_ptr =
      evas_gl_thread_cmd_create(thread_type, sizeof(GL_TH_ST(eglSwapBuffersWithDamage) *) + sizeof(GL_TH_ST(eglSwapBuffersWithDamage)), &thcmd_ref);
  *thread_data_ptr = (void *)((char *)thread_data_ptr + sizeof(GL_TH_ST(eglSwapBuffersWithDamage) *));
  thread_data = *thread_data_ptr;

  int thread_mode = EVAS_GL_THREAD_MODE_FINISH;
  if (!evas_gl_thread_force_finish())
    thread_mode = EVAS_GL_THREAD_MODE_FLUSH;

  thread_data->dpy = dpy;
  thread_data->surface = surface;
  thread_data->rects = rects;
  thread_data->n_rects = n_rects;
  thread_data->orig_func = orig_func;

  evas_gl_thread_cmd_enqueue(thcmd_ref,
                             GL_TH_CB(eglSwapBuffersWithDamage),
                             thread_mode);

  return thread_data->return_value;
}



typedef struct
{
  GL_TH_FNTYPE(eglSetDamageRegion) orig_func;
  EGLBoolean return_value;
  EGLDisplay dpy;
  EGLSurface surface;
  EGLint *rects;
  EGLint n_rects;
} GL_TH_ST(eglSetDamageRegion);

static void
GL_TH_CB(eglSetDamageRegion)(void *data)
{
  GL_TH_ST(eglSetDamageRegion) *thread_data = *(void **)data;

  thread_data->return_value =
      thread_data->orig_func(thread_data->dpy,
                             thread_data->surface,
                             thread_data->rects,
                             thread_data->n_rects);
}

EGLBoolean
GL_TH_FN(eglSetDamageRegion)(GL_TH_DP, EGLDisplay dpy, EGLSurface surface, EGLint *rects, EGLint n_rects)
{
  GL_TH_ST(eglSetDamageRegion) thread_data_local, *thread_data = &thread_data_local, **thread_data_ptr;
  void *thcmd_ref;

  if (!evas_gl_thread_enabled(thread_type))
    return ((GL_TH_FNTYPE(eglSetDamageRegion))orig_func)(dpy, surface, rects, n_rects);

  thread_data_ptr =
      evas_gl_thread_cmd_create(thread_type, sizeof(GL_TH_ST(eglSetDamageRegion) *), &thcmd_ref);
  *thread_data_ptr = thread_data;

  thread_data->dpy = dpy;
  thread_data->surface = surface;
  thread_data->rects = rects;
  thread_data->n_rects = n_rects;
  thread_data->orig_func = orig_func;

  evas_gl_thread_cmd_enqueue(thcmd_ref,
                             GL_TH_CB(eglSetDamageRegion),
                             EVAS_GL_THREAD_MODE_FINISH);

  return thread_data->return_value;
}



typedef struct
{
  GL_TH_FNTYPE(eglQueryWaylandBufferWL) orig_func;
  EGLBoolean return_value;
  EGLDisplay dpy;
  void *buffer;
  EGLint attribute;
  EGLint *value;
} GL_TH_ST(eglQueryWaylandBufferWL);

static void
GL_TH_CB(eglQueryWaylandBufferWL)(void *data)
{
  GL_TH_ST(eglQueryWaylandBufferWL) *thread_data = *(void **)data;

  thread_data->return_value =
      thread_data->orig_func(thread_data->dpy,
                             thread_data->buffer,
                             thread_data->attribute,
                             thread_data->value);
}

EGLBoolean
GL_TH_FN(eglQueryWaylandBufferWL)(GL_TH_DP, EGLDisplay dpy, void *buffer, EGLint attribute, EGLint *value)
{
  GL_TH_ST(eglQueryWaylandBufferWL) thread_data_local, *thread_data = &thread_data_local, **thread_data_ptr;
  void *thcmd_ref;

  if (!evas_gl_thread_enabled(thread_type))
    return ((GL_TH_FNTYPE(eglQueryWaylandBufferWL))orig_func)(dpy, buffer, attribute, value);

  thread_data_ptr =
      evas_gl_thread_cmd_create(thread_type, sizeof(GL_TH_ST(eglQueryWaylandBufferWL) *), &thcmd_ref);
  *thread_data_ptr = thread_data;

  thread_data->dpy = dpy;
  thread_data->buffer = buffer;
  thread_data->attribute = attribute;
  thread_data->value =value;
  thread_data->orig_func = orig_func;

  evas_gl_thread_cmd_enqueue(thcmd_ref,
                             GL_TH_CB(eglQueryWaylandBufferWL),
                             EVAS_GL_THREAD_MODE_FINISH);

  return thread_data->return_value;
}

typedef struct
{
  GL_TH_FNTYPE(eglGetProcAddress) orig_func;
  void *return_value;
  char const * procname;
} GL_TH_ST(eglGetProcAddress);



static void
GL_TH_CB(eglGetProcAddress)(void *data)
{
  GL_TH_ST(eglGetProcAddress) *thread_data = *(void **)data;

   thread_data->return_value = thread_data->orig_func(thread_data->procname);
}

void *
GL_TH_FN(eglGetProcAddress)(GL_TH_DP, char const * procname)
{
  GL_TH_ST(eglGetProcAddress) thread_data_local, *thread_data = &thread_data_local, **thread_data_ptr;
  void *thcmd_ref;

  if (!evas_gl_thread_enabled(thread_type))
    return ((GL_TH_FNTYPE(eglGetProcAddress))orig_func)(procname);

  thread_data_ptr =
      evas_gl_thread_cmd_create(thread_type, sizeof(GL_TH_ST(eglGetProcAddress) *), &thcmd_ref);
  *thread_data_ptr = thread_data;

  thread_data->procname = procname;
  thread_data->orig_func = orig_func;

  evas_gl_thread_cmd_enqueue(thcmd_ref,
                             GL_TH_CB(eglGetProcAddress),
                             EVAS_GL_THREAD_MODE_FINISH);

   return thread_data->return_value;
}



typedef struct
{
  GL_TH_FNTYPE(eglCreateWindowSurface) orig_func;
  EGLSurface return_value;
  EGLDisplay egl_disp;
  EGLConfig egl_config;
  EGLNativeWindowType egl_win;
  EGLint const * attrib_list;
} GL_TH_ST(eglCreateWindowSurface);

static void
GL_TH_CB(eglCreateWindowSurface)(void *data)
{
  GL_TH_ST(eglCreateWindowSurface) *thread_data = *(void **)data;

  thread_data->return_value = thread_data->orig_func(thread_data->egl_disp,
                                                     thread_data->egl_config,
                                                     thread_data->egl_win,
                                                     thread_data->attrib_list);
}

EGLSurface
GL_TH_FN(eglCreateWindowSurface)(GL_TH_DP, EGLDisplay egl_disp, EGLConfig egl_config,
    EGLNativeWindowType egl_win, EGLint const * attrib_list)
    {
  GL_TH_ST(eglCreateWindowSurface) thread_data_local, *thread_data = &thread_data_local, **thread_data_ptr;
  void *thcmd_ref;

  if (!evas_gl_thread_enabled(thread_type))
    return ((GL_TH_FNTYPE(eglCreateWindowSurface))orig_func)(egl_disp, egl_config,
        egl_win, attrib_list);


  thread_data_ptr =
      evas_gl_thread_cmd_create(thread_type, sizeof(GL_TH_ST(eglCreateWindowSurface) *), &thcmd_ref);
  *thread_data_ptr = thread_data;

  thread_data->egl_disp = egl_disp;
  thread_data->egl_config = egl_config;
  thread_data->egl_win = egl_win;
  thread_data->attrib_list = attrib_list;
  thread_data->orig_func = orig_func;

  evas_gl_thread_cmd_enqueue(thcmd_ref,
                             GL_TH_CB(eglCreateWindowSurface),
                             EVAS_GL_THREAD_MODE_FINISH);

  return thread_data->return_value;
    }

typedef struct
{
  GL_TH_FNTYPE(eglDestroySurface) orig_func;
  EGLBoolean return_value;
  EGLDisplay egl_disp;
  EGLSurface egl_surf;
} GL_TH_ST(eglDestroySurface);

static void
GL_TH_CB(eglDestroySurface)(void *data)
{
  GL_TH_ST(eglDestroySurface) *thread_data = *(void **)data;

  thread_data->return_value = thread_data->orig_func(thread_data->egl_disp,
                                                     thread_data->egl_surf);
}

EGLBoolean
GL_TH_FN(eglDestroySurface)(GL_TH_DP, EGLDisplay egl_disp, EGLSurface egl_surf)
{
  GL_TH_ST(eglDestroySurface) thread_data_local, *thread_data = &thread_data_local, **thread_data_ptr;
  void *thcmd_ref;

  if (!evas_gl_thread_enabled(thread_type))
    return ((GL_TH_FNTYPE(eglDestroySurface))orig_func)(egl_disp, egl_surf);


  thread_data_ptr =
      evas_gl_thread_cmd_create(thread_type, sizeof(GL_TH_ST(eglDestroySurface) *), &thcmd_ref);
  *thread_data_ptr = thread_data;

  thread_data->egl_disp = egl_disp;
  thread_data->egl_surf = egl_surf;
  thread_data->orig_func = orig_func;

  evas_gl_thread_cmd_enqueue(thcmd_ref,
                             GL_TH_CB(eglDestroySurface),
                             EVAS_GL_THREAD_MODE_FINISH);


  return thread_data->return_value;
}


typedef struct
{
  GL_TH_FNTYPE(eglCreateContext) orig_func;
  EGLContext return_value;
  EGLDisplay display;
  EGLConfig config;
  EGLContext share_context;
  EGLint const * attrib_list;
} GL_TH_ST(eglCreateContext);

static void
GL_TH_CB(eglCreateContext)(void *data)
{
  GL_TH_ST(eglCreateContext) *thread_data = *(void **)data;

  thread_data->return_value = thread_data->orig_func(thread_data->display,
                                               thread_data->config,
                                               thread_data->share_context,
                                               thread_data->attrib_list);
}

EGLContext
GL_TH_FN(eglCreateContext)(GL_TH_DP, EGLDisplay display,
                                 EGLConfig config,
                                 EGLContext share_context,
                                 EGLint const * attrib_list)
{

  GL_TH_ST(eglCreateContext) thread_data_local, *thread_data = &thread_data_local, **thread_data_ptr;
  void *thcmd_ref;

  if (!evas_gl_thread_enabled(thread_type))
    return ((GL_TH_FNTYPE(eglCreateContext))orig_func)(display, config, share_context, attrib_list);

  thread_data_ptr =
        evas_gl_thread_cmd_create(thread_type, sizeof(GL_TH_ST(eglCreateContext) *), &thcmd_ref);
    *thread_data_ptr = thread_data;

    thread_data->display = display;
    thread_data->config = config;
    thread_data->share_context = share_context;
    thread_data->attrib_list = attrib_list;
    thread_data->orig_func = orig_func;

    evas_gl_thread_cmd_enqueue(thcmd_ref,
                               GL_TH_CB(eglCreateContext),
                               EVAS_GL_THREAD_MODE_FINISH);

  return thread_data->return_value;
}

typedef struct
{
  GL_TH_FNTYPE(eglDestroyContext) orig_func;
  EGLBoolean return_value;
  EGLDisplay display;
  EGLContext context;
} GL_TH_ST(eglDestroyContext);

static void
GL_TH_CB(eglDestroyContext)(void *data)
{
  GL_TH_ST(eglDestroyContext) *thread_data = *(void **)data;

  thread_data->return_value = thread_data->orig_func(thread_data->display,
                                                thread_data->context);
}

EGLBoolean
GL_TH_FN(eglDestroyContext)(GL_TH_DP,EGLDisplay display,
                                  EGLContext context)
{

  GL_TH_ST(eglDestroyContext) thread_data_local, *thread_data = &thread_data_local, **thread_data_ptr;
  void *thcmd_ref;

  if (!evas_gl_thread_enabled(thread_type))
    return ((GL_TH_FNTYPE(eglDestroyContext))orig_func)(display, context);


  thread_data_ptr =
        evas_gl_thread_cmd_create(thread_type, sizeof(GL_TH_ST(eglDestroyContext) *), &thcmd_ref);
    *thread_data_ptr = thread_data;


  thread_data->display = display;
  thread_data->context = context;
  thread_data->orig_func = orig_func;

  evas_gl_thread_cmd_enqueue(thcmd_ref,
                             GL_TH_CB(eglDestroyContext),
                             EVAS_GL_THREAD_MODE_FINISH);

  return thread_data->return_value;
}


typedef struct
{
  GL_TH_FNTYPE(eglQueryString) orig_func;
  char const * return_value;
  EGLDisplay display;
  EGLint name;
} GL_TH_ST(eglQueryString);

static void
GL_TH_CB(eglQueryString)(void *data)
{
  GL_TH_ST(eglQueryString) *thread_data = *(void **)data;

  thread_data->return_value = thread_data->orig_func(thread_data->display,
                                                thread_data->name);
}

char const *
GL_TH_FN(eglQueryString)(GL_TH_DP,EGLDisplay display,  EGLint name)
{

  GL_TH_ST(eglQueryString) thread_data_local, *thread_data = &thread_data_local, **thread_data_ptr;
  void *thcmd_ref;

  if (!evas_gl_thread_enabled(thread_type))
    return ((GL_TH_FNTYPE(eglQueryString))orig_func)(display, name);


  thread_data_ptr =
        evas_gl_thread_cmd_create(thread_type, sizeof(GL_TH_ST(eglQueryString) *), &thcmd_ref);
    *thread_data_ptr = thread_data;


  thread_data->display = display;
  thread_data->name = name;
  thread_data->orig_func = orig_func;

  evas_gl_thread_cmd_enqueue(thcmd_ref,
                             GL_TH_CB(eglQueryString),
                             EVAS_GL_THREAD_MODE_FINISH);

  return thread_data->return_value;
}

typedef struct
{
  GL_TH_FNTYPE(eglCreateImage) orig_func;
  void * return_value;
  EGLDisplay dpy;
  EGLContext ctx;
  int target;
  void* buffer;
  int *attribs;
} GL_TH_ST(eglCreateImage);

static void
GL_TH_CB(eglCreateImage)(void *data)
{
  GL_TH_ST(eglCreateImage) *thread_data = *(void **)data;

  thread_data->return_value = thread_data->orig_func(thread_data->dpy, thread_data->ctx, thread_data->target,
                                                     thread_data->buffer, thread_data->attribs);
}

void *
GL_TH_FN(eglCreateImage)(GL_TH_DP, EGLDisplay dpy, EGLContext ctx, int target, void* buffer, int *attribs)
{

  GL_TH_ST(eglCreateImage) thread_data_local, *thread_data = &thread_data_local, **thread_data_ptr;
  void *thcmd_ref;

  if (!evas_gl_thread_enabled(thread_type))
    return ((GL_TH_FNTYPE(eglCreateImage))orig_func)(dpy, ctx, target, buffer, attribs);


  thread_data_ptr =
        evas_gl_thread_cmd_create(thread_type, sizeof(GL_TH_ST(eglCreateImage) *), &thcmd_ref);
    *thread_data_ptr = thread_data;


  thread_data->dpy = dpy;
  thread_data->ctx = ctx;
  thread_data->target = target;
  thread_data->buffer = buffer;
  thread_data->attribs = attribs;
  thread_data->orig_func = orig_func;

  evas_gl_thread_cmd_enqueue(thcmd_ref,
                             GL_TH_CB(eglCreateImage),
                             EVAS_GL_THREAD_MODE_FINISH);

  return thread_data->return_value;
}


typedef struct
{
  GL_TH_FNTYPE(eglDestroyImage) orig_func;
  EGLBoolean return_value;
  EGLDisplay  dpy;
  EGLImageKHR img;
} GL_TH_ST(eglDestroyImage);

static void
GL_TH_CB(eglDestroyImage)(void *data)
{
  GL_TH_ST(eglDestroyImage) *thread_data = *(void **)data;

  thread_data->return_value = thread_data->orig_func(thread_data->dpy, thread_data->img);
}

EGLBoolean
GL_TH_FN(eglDestroyImage)(GL_TH_DP,  EGLDisplay  dpy, EGLImageKHR img)
{

  GL_TH_ST(eglDestroyImage) thread_data_local, *thread_data = &thread_data_local, **thread_data_ptr;
  void *thcmd_ref;

  if (!evas_gl_thread_enabled(thread_type))
    return ((GL_TH_FNTYPE(eglDestroyImage))orig_func)(dpy, img);


  thread_data_ptr =
        evas_gl_thread_cmd_create(thread_type, sizeof(GL_TH_ST(eglDestroyImage) *), &thcmd_ref);
    *thread_data_ptr = thread_data;


  thread_data->dpy = dpy;
  thread_data->img = img;
  thread_data->orig_func = orig_func;

  evas_gl_thread_cmd_enqueue(thcmd_ref,
                             GL_TH_CB(eglDestroyImage),
                             EVAS_GL_THREAD_MODE_FINISH);

  return thread_data->return_value;
}


typedef struct
{
  GL_TH_FNTYPE(eglCreateSyncKHR) orig_func;
  void * return_value;
  EGLDisplay dpy;
  unsigned int type;
  const int *attrib_list;
} GL_TH_ST(eglCreateSyncKHR);

static void
GL_TH_CB(eglCreateSyncKHR)(void *data)
{
  GL_TH_ST(eglCreateSyncKHR) *thread_data = *(void **)data;

  thread_data->return_value = thread_data->orig_func(thread_data->dpy, thread_data->type, thread_data->attrib_list);
}

void *
GL_TH_FN(eglCreateSyncKHR)(GL_TH_DP, EGLDisplay dpy, unsigned int type, const int *attrib_list)
{

  GL_TH_ST(eglCreateSyncKHR) thread_data_local, *thread_data = &thread_data_local, **thread_data_ptr;
  void *thcmd_ref;

  if (!evas_gl_thread_enabled(thread_type))
    return ((GL_TH_FNTYPE(eglCreateSyncKHR))orig_func)(dpy, type, attrib_list);


  thread_data_ptr =
        evas_gl_thread_cmd_create(thread_type, sizeof(GL_TH_ST(eglCreateSyncKHR) *), &thcmd_ref);
    *thread_data_ptr = thread_data;


  thread_data->dpy = dpy;
  thread_data->type = type;
  thread_data->attrib_list = attrib_list;
  thread_data->orig_func = orig_func;

  evas_gl_thread_cmd_enqueue(thcmd_ref,
                             GL_TH_CB(eglCreateSyncKHR),
                             EVAS_GL_THREAD_MODE_FINISH);

  return thread_data->return_value;
}

typedef struct
{
  GL_TH_FNTYPE(eglDestroySyncKHR) orig_func;
  EGLBoolean return_value;
  EGLDisplay  dpy;
  void * sync;
} GL_TH_ST(eglDestroySyncKHR);

static void
GL_TH_CB(eglDestroySyncKHR)(void *data)
{
  GL_TH_ST(eglDestroySyncKHR) *thread_data = *(void **)data;

  thread_data->return_value = thread_data->orig_func(thread_data->dpy, thread_data->sync);
}

EGLBoolean
GL_TH_FN(eglDestroySyncKHR)(GL_TH_DP,  EGLDisplay  dpy, void * sync)
{

  GL_TH_ST(eglDestroySyncKHR) thread_data_local, *thread_data = &thread_data_local, **thread_data_ptr;
  void *thcmd_ref;

  if (!evas_gl_thread_enabled(thread_type))
    return ((GL_TH_FNTYPE(eglDestroySyncKHR))orig_func)(dpy, sync);


  thread_data_ptr =
        evas_gl_thread_cmd_create(thread_type, sizeof(GL_TH_ST(eglDestroySyncKHR) *), &thcmd_ref);
    *thread_data_ptr = thread_data;


  thread_data->dpy = dpy;
  thread_data->sync = sync;
  thread_data->orig_func = orig_func;

  evas_gl_thread_cmd_enqueue(thcmd_ref,
                             GL_TH_CB(eglDestroySyncKHR),
                             EVAS_GL_THREAD_MODE_FINISH);

  return thread_data->return_value;
}


typedef struct
{
  GL_TH_FNTYPE(eglClientWaitSyncKHR) orig_func;
  int return_value;
  EGLDisplay  dpy;
  void * sync;
  int flags;
  unsigned long long timeout;
} GL_TH_ST(eglClientWaitSyncKHR);

static void
GL_TH_CB(eglClientWaitSyncKHR)(void *data)
{
  GL_TH_ST(eglClientWaitSyncKHR) *thread_data = *(void **)data;

  thread_data->return_value = thread_data->orig_func(thread_data->dpy, thread_data->sync, thread_data->flags, thread_data->timeout);
}

int
GL_TH_FN(eglClientWaitSyncKHR)(GL_TH_DP,  EGLDisplay  dpy,  void * sync, int flags, unsigned long long timeout)
{

  GL_TH_ST(eglClientWaitSyncKHR) thread_data_local, *thread_data = &thread_data_local, **thread_data_ptr;
  void *thcmd_ref;

  if (!evas_gl_thread_enabled(thread_type))
    return ((GL_TH_FNTYPE(eglClientWaitSyncKHR))orig_func)(dpy, sync, flags, timeout);


  thread_data_ptr =
        evas_gl_thread_cmd_create(thread_type, sizeof(GL_TH_ST(eglClientWaitSyncKHR) *), &thcmd_ref);
    *thread_data_ptr = thread_data;


  thread_data->dpy = dpy;
  thread_data->sync = sync;
  thread_data->flags = flags;
  thread_data->timeout = timeout;
  thread_data->orig_func = orig_func;

  evas_gl_thread_cmd_enqueue(thcmd_ref,
                             GL_TH_CB(eglClientWaitSyncKHR),
                             EVAS_GL_THREAD_MODE_FINISH);

  return thread_data->return_value;
}

typedef struct
{
  GL_TH_FNTYPE(eglSignalSyncKHR) orig_func;
  EGLBoolean return_value;
  EGLDisplay  dpy;
  void * sync;
  unsigned mode;
} GL_TH_ST(eglSignalSyncKHR);

static void
GL_TH_CB(eglSignalSyncKHR)(void *data)
{
  GL_TH_ST(eglSignalSyncKHR) *thread_data = *(void **)data;

  thread_data->return_value = thread_data->orig_func(thread_data->dpy, thread_data->sync, thread_data->mode);
}

EGLBoolean
GL_TH_FN(eglSignalSyncKHR)(GL_TH_DP,  EGLDisplay  dpy,  void * sync, unsigned mode)
{

  GL_TH_ST(eglSignalSyncKHR) thread_data_local, *thread_data = &thread_data_local, **thread_data_ptr;
  void *thcmd_ref;

  if (!evas_gl_thread_enabled(thread_type))
    return ((GL_TH_FNTYPE(eglSignalSyncKHR))orig_func)(dpy, sync, mode);


  thread_data_ptr =
        evas_gl_thread_cmd_create(thread_type, sizeof(GL_TH_ST(eglSignalSyncKHR) *), &thcmd_ref);
    *thread_data_ptr = thread_data;


  thread_data->dpy = dpy;
  thread_data->sync = sync;
  thread_data->mode = mode;
  thread_data->orig_func = orig_func;

  evas_gl_thread_cmd_enqueue(thcmd_ref,
                             GL_TH_CB(eglSignalSyncKHR),
                             EVAS_GL_THREAD_MODE_FINISH);

  return thread_data->return_value;
}

typedef struct
{
  GL_TH_FNTYPE(eglGetSyncAttribKHR) orig_func;
  EGLBoolean return_value;
  EGLDisplay  dpy;
  void * sync;
  int attribute;
  int *value;
} GL_TH_ST(eglGetSyncAttribKHR);

static void
GL_TH_CB(eglGetSyncAttribKHR)(void *data)
{
  GL_TH_ST(eglGetSyncAttribKHR) *thread_data = *(void **)data;

  thread_data->return_value = thread_data->orig_func(thread_data->dpy, thread_data->sync, thread_data->attribute, thread_data->value);
}

EGLBoolean
GL_TH_FN(eglGetSyncAttribKHR)(GL_TH_DP,  EGLDisplay  dpy,  void * sync, int attribute, int *value)
{

  GL_TH_ST(eglGetSyncAttribKHR) thread_data_local, *thread_data = &thread_data_local, **thread_data_ptr;
  void *thcmd_ref;

  if (!evas_gl_thread_enabled(thread_type))
    return ((GL_TH_FNTYPE(eglGetSyncAttribKHR))orig_func)(dpy, sync, attribute, value);


  thread_data_ptr =
        evas_gl_thread_cmd_create(thread_type, sizeof(GL_TH_ST(eglGetSyncAttribKHR) *), &thcmd_ref);
    *thread_data_ptr = thread_data;


  thread_data->dpy = dpy;
  thread_data->sync = sync;
  thread_data->attribute = attribute;
  thread_data->value = value;
  thread_data->orig_func = orig_func;

  evas_gl_thread_cmd_enqueue(thcmd_ref,
                             GL_TH_CB(eglGetSyncAttribKHR),
                             EVAS_GL_THREAD_MODE_FINISH);

  return thread_data->return_value;
}


typedef struct
{
  GL_TH_FNTYPE(eglWaitSyncKHR) orig_func;
  int return_value;
  EGLDisplay  dpy;
  void * sync;
  int flags;
} GL_TH_ST(eglWaitSyncKHR);

static void
GL_TH_CB(eglWaitSyncKHR)(void *data)
{
  GL_TH_ST(eglWaitSyncKHR) *thread_data = *(void **)data;

  thread_data->return_value = thread_data->orig_func(thread_data->dpy, thread_data->sync, thread_data->flags);
}

int
GL_TH_FN(eglWaitSyncKHR)(GL_TH_DP,  EGLDisplay  dpy,  void * sync, int flags)
{

  GL_TH_ST(eglWaitSyncKHR) thread_data_local, *thread_data = &thread_data_local, **thread_data_ptr;
  void *thcmd_ref;

  if (!evas_gl_thread_enabled(thread_type))
    return ((GL_TH_FNTYPE(eglWaitSyncKHR))orig_func)(dpy, sync, flags);


  thread_data_ptr =
        evas_gl_thread_cmd_create(thread_type, sizeof(GL_TH_ST(eglWaitSyncKHR) *), &thcmd_ref);
    *thread_data_ptr = thread_data;


  thread_data->dpy = dpy;
  thread_data->sync = sync;
  thread_data->flags = flags;
  thread_data->orig_func = orig_func;

  evas_gl_thread_cmd_enqueue(thcmd_ref,
                             GL_TH_CB(eglWaitSyncKHR),
                             EVAS_GL_THREAD_MODE_FINISH);

  return thread_data->return_value;
}


typedef struct
{
  GL_TH_FNTYPE(eglBindWaylandDisplayWL) orig_func;
  EGLBoolean return_value;
  EGLDisplay  dpy;
  void * wl_display;
} GL_TH_ST(eglBindWaylandDisplayWL);

static void
GL_TH_CB(eglBindWaylandDisplayWL)(void *data)
{
  GL_TH_ST(eglBindWaylandDisplayWL) *thread_data = *(void **)data;

  thread_data->return_value = thread_data->orig_func(thread_data->dpy, thread_data->wl_display);
}

EGLBoolean
GL_TH_FN(eglBindWaylandDisplayWL)(GL_TH_DP,  EGLDisplay  dpy,  void *wl_display)
{

  GL_TH_ST(eglBindWaylandDisplayWL) thread_data_local, *thread_data = &thread_data_local, **thread_data_ptr;
  void *thcmd_ref;

  if (!evas_gl_thread_enabled(thread_type))
    return ((GL_TH_FNTYPE(eglBindWaylandDisplayWL))orig_func)(dpy, wl_display);


  thread_data_ptr =
        evas_gl_thread_cmd_create(thread_type, sizeof(GL_TH_ST(eglBindWaylandDisplayWL) *), &thcmd_ref);
    *thread_data_ptr = thread_data;


  thread_data->dpy = dpy;
  thread_data->wl_display = wl_display;
  thread_data->orig_func = orig_func;

  evas_gl_thread_cmd_enqueue(thcmd_ref,
                             GL_TH_CB(eglBindWaylandDisplayWL),
                             EVAS_GL_THREAD_MODE_FINISH);

  return thread_data->return_value;
}


typedef struct
{
  GL_TH_FNTYPE(eglUnbindWaylandDisplayWL) orig_func;
  EGLBoolean return_value;
  EGLDisplay  dpy;
  void * wl_display;
} GL_TH_ST(eglUnbindWaylandDisplayWL);

static void
GL_TH_CB(eglUnbindWaylandDisplayWL)(void *data)
{
  GL_TH_ST(eglUnbindWaylandDisplayWL) *thread_data = *(void **)data;

  thread_data->return_value = thread_data->orig_func(thread_data->dpy, thread_data->wl_display);
}

EGLBoolean
GL_TH_FN(eglUnbindWaylandDisplayWL)(GL_TH_DP,  EGLDisplay  dpy,  void *wl_display)
{

  GL_TH_ST(eglUnbindWaylandDisplayWL) thread_data_local, *thread_data = &thread_data_local, **thread_data_ptr;
  void *thcmd_ref;

  if (!evas_gl_thread_enabled(thread_type))
    return ((GL_TH_FNTYPE(eglUnbindWaylandDisplayWL))orig_func)(dpy, wl_display);


  thread_data_ptr =
        evas_gl_thread_cmd_create(thread_type, sizeof(GL_TH_ST(eglUnbindWaylandDisplayWL) *), &thcmd_ref);
    *thread_data_ptr = thread_data;


  thread_data->dpy = dpy;
  thread_data->wl_display = wl_display;
  thread_data->orig_func = orig_func;

  evas_gl_thread_cmd_enqueue(thcmd_ref,
                             GL_TH_CB(eglUnbindWaylandDisplayWL),
                             EVAS_GL_THREAD_MODE_FINISH);

  return thread_data->return_value;
}



typedef struct
{
  GL_TH_FNTYPE(wl_egl_window_tizen_set_buffer_transform) orig_func;
  void *egl_window;
  int wl_output_transform;
} GL_TH_ST(wl_egl_window_tizen_set_buffer_transform);

static void
GL_TH_CB(wl_egl_window_tizen_set_buffer_transform)(void *data)
{
  GL_TH_ST(wl_egl_window_tizen_set_buffer_transform) *thread_data = *(void **)data;

  thread_data->orig_func(thread_data->egl_window, thread_data->wl_output_transform);
}

void
GL_TH_FN(wl_egl_window_tizen_set_buffer_transform)(GL_TH_DP, void *egl_window, int wl_output_transform)
{

  GL_TH_ST(wl_egl_window_tizen_set_buffer_transform) thread_data_local, *thread_data = &thread_data_local, **thread_data_ptr;
  void *thcmd_ref;

  if (!evas_gl_thread_enabled(thread_type))
    return ((GL_TH_FNTYPE(wl_egl_window_tizen_set_buffer_transform))orig_func)(egl_window, wl_output_transform);

  thread_data_ptr =
        evas_gl_thread_cmd_create(thread_type, sizeof(GL_TH_ST(wl_egl_window_tizen_set_buffer_transform) *), &thcmd_ref);
  *thread_data_ptr = thread_data;

  thread_data->egl_window = egl_window;
  thread_data->wl_output_transform = wl_output_transform;
  thread_data->orig_func = orig_func;

  evas_gl_thread_cmd_enqueue(thcmd_ref,
                             GL_TH_CB(wl_egl_window_tizen_set_buffer_transform),
                             EVAS_GL_THREAD_MODE_FINISH);
}

typedef struct
{
  GL_TH_FNTYPE(wl_egl_window_tizen_set_window_transform) orig_func;
  void *egl_window;
  int wl_output_transform;
} GL_TH_ST(wl_egl_window_tizen_set_window_transform);

static void
GL_TH_CB(wl_egl_window_tizen_set_window_transform)(void *data)
{
  GL_TH_ST(wl_egl_window_tizen_set_window_transform) *thread_data = *(void **)data;

  thread_data->orig_func(thread_data->egl_window, thread_data->wl_output_transform);
}

void
GL_TH_FN(wl_egl_window_tizen_set_window_transform)(GL_TH_DP,  void *egl_window, int wl_output_transform)
{

  GL_TH_ST(wl_egl_window_tizen_set_window_transform) thread_data_local, *thread_data = &thread_data_local, **thread_data_ptr;
  void *thcmd_ref;

  if (!evas_gl_thread_enabled(thread_type))
    return ((GL_TH_FNTYPE(wl_egl_window_tizen_set_window_transform))orig_func)(egl_window, wl_output_transform);

  thread_data_ptr =
        evas_gl_thread_cmd_create(thread_type, sizeof(GL_TH_ST(wl_egl_window_tizen_set_window_transform) *), &thcmd_ref);
  *thread_data_ptr = thread_data;

  thread_data->egl_window = egl_window;
  thread_data->wl_output_transform = wl_output_transform;
  thread_data->orig_func = orig_func;

  evas_gl_thread_cmd_enqueue(thcmd_ref,
                             GL_TH_CB(wl_egl_window_tizen_set_window_transform),
                             EVAS_GL_THREAD_MODE_FINISH);
}


/*****************************************************************************/


static Evas_GL_Thread_EGL_Func th_egl_func;
Eina_Bool th_egl_func_initialized = EINA_FALSE;

void *
evas_gl_thread_egl_func_get()
{
  if (!th_egl_func_initialized)
    {
#define THREAD_FUNCTION_ASSIGN(func) th_egl_func.func = func;

#define EVAS_TH_EGL_FN(ret, name, ...) \
    THREAD_FUNCTION_ASSIGN(GL_TH_FN(name));
#define EVAS_TH_EGL_FN_ASYNC(ret, name, ...) \
    THREAD_FUNCTION_ASSIGN(GL_TH_FN(name##_begin)); \
    THREAD_FUNCTION_ASSIGN(GL_TH_FN(name##_end));

      EVAS_TH_EGL_FN_LIST

#undef EVAS_TH_EGL_FN_ASYNC
#undef EVAS_TH_EGL_FN

#undef THREAD_FUNCTION_ASSIGN

      th_egl_func_initialized = EINA_TRUE;
    }

  return &th_egl_func;
}


#else /* ! EVAS_GL_RENDER_THREAD_COMPILE_FOR_GL_GENERIC */
/* compiled for GL backend */


#include <dlfcn.h>

#define EVAS_TH_EGL_FN(ret, name, ...) \
    ret (*GL_TH_FN(name))(GL_TH_DP,  ##__VA_ARGS__);
#define EVAS_TH_EGL_FN_ASYNC(ret, name, ...) \
    void *(*GL_TH_FN(name##_begin))(GL_TH_DP,  ##__VA_ARGS__); \
    ret (*GL_TH_FN(name##_end))(void *ref);

EVAS_TH_EGL_FN_LIST

#undef EVAS_TH_EGL_FN_ASYNC
#undef EVAS_TH_EGL_FN

void
_egl_thread_link_init(void *func_ptr)
{
  const Evas_GL_Thread_EGL_Func *th_egl_func = func_ptr;

  if (!th_egl_func)
    {
      ERR("Thread functions (EGL BASE) are not exist");
      return;
    }

#define THREAD_FUNCTION_ASSIGN(func) func = th_egl_func->func;

#define EVAS_TH_EGL_FN(ret, name, ...) \
    THREAD_FUNCTION_ASSIGN(GL_TH_FN(name));
#define EVAS_TH_EGL_FN_ASYNC(ret, name, ...) \
    THREAD_FUNCTION_ASSIGN(GL_TH_FN(name##_begin)); \
    THREAD_FUNCTION_ASSIGN(GL_TH_FN(name##_end));

  EVAS_TH_EGL_FN_LIST

#undef EVAS_TH_EGL_FN_ASYNC
#undef EVAS_TH_EGL_FN

#undef THREAD_FUNCTION_ASSIGN
}

#endif /* EVAS_GL_RENDER_THREAD_COMPILE_FOR_GL_GENERIC */

#endif /* ! GL_GLES */
