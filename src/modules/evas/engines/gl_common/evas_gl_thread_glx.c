#include "evas_common_private.h"
#include "evas_gl_thread.h"


#ifndef GL_GLES


#ifdef EVAS_GL_RENDER_THREAD_IS_GENERIC


/*
   void
   glXBindTexImage(Display *dpy, GLXDrawable drawable, int buffer, const int *attrib_list);
 */

typedef struct
{
   Display *dpy;
   GLXDrawable drawable;
   int buffer;
   const int *attrib_list;

} Thread_Command_glXBindTexImage;

void (*orig_evas_glXBindTexImage)(Display *dpy, GLXDrawable drawable, int buffer, const int *attrib_list);

EAPI void
glXBindTexImage_orig_evas_set(void *func)
{
   orig_evas_glXBindTexImage = func;
}

EAPI void *
glXBindTexImage_orig_evas_get(void)
{
   return orig_evas_glXBindTexImage;
}

static void
_gl_thread_glXBindTexImage(void *data)
{
   Thread_Command_glXBindTexImage *thread_param =
      (Thread_Command_glXBindTexImage *)data;

   orig_evas_glXBindTexImage(thread_param->dpy,
                             thread_param->drawable,
                             thread_param->buffer,
                             thread_param->attrib_list);

}

EAPI void
glXBindTexImage_thread_cmd(Display *dpy, GLXDrawable drawable, int buffer, const int *attrib_list)
{
   if (!evas_gl_thread_enabled())
     {
        orig_evas_glXBindTexImage(dpy, drawable, buffer, attrib_list);
        return;
     }

   int thread_mode = EVAS_GL_THREAD_MODE_FINISH;

   Thread_Command_glXBindTexImage thread_param_local;
   Thread_Command_glXBindTexImage *thread_param = &thread_param_local;

   thread_param->dpy = dpy;
   thread_param->drawable = drawable;
   thread_param->buffer = buffer;
   thread_param->attrib_list = attrib_list;

   evas_gl_thread_cmd_enqueue(EVAS_GL_THREAD_TYPE_GL,
                              _gl_thread_glXBindTexImage,
                              thread_param,
                              thread_mode);
}

/*
   GLXFBConfig *
   glXChooseFBConfig(Display *dpy, int screen, const int *attribList, int *nitems);
 */

typedef struct
{
   GLXFBConfig * return_value;
   Display *dpy;
   int screen;
   const int *attribList;
   int *nitems;

} Thread_Command_glXChooseFBConfig;

static void
_gl_thread_glXChooseFBConfig(void *data)
{
   Thread_Command_glXChooseFBConfig *thread_param =
      (Thread_Command_glXChooseFBConfig *)data;

   thread_param->return_value = glXChooseFBConfig(thread_param->dpy,
                                                  thread_param->screen,
                                                  thread_param->attribList,
                                                  thread_param->nitems);

}

EAPI GLXFBConfig *
glXChooseFBConfig_thread_cmd(Display *dpy, int screen, const int *attribList, int *nitems)
{
   if (!evas_gl_thread_enabled())
     {
        return glXChooseFBConfig(dpy, screen, attribList, nitems);
     }

   int thread_mode = EVAS_GL_THREAD_MODE_FINISH;

   Thread_Command_glXChooseFBConfig thread_param_local;
   Thread_Command_glXChooseFBConfig *thread_param = &thread_param_local;

   thread_param->dpy = dpy;
   thread_param->screen = screen;
   thread_param->attribList = attribList;
   thread_param->nitems = nitems;

   evas_gl_thread_cmd_enqueue(EVAS_GL_THREAD_TYPE_GL,
                              _gl_thread_glXChooseFBConfig,
                              thread_param,
                              thread_mode);

   return thread_param->return_value;
}

/*
   GLXContext
   glXCreateContext(Display *dpy, XVisualInfo *vis, GLXContext shareList, Bool direct);
 */

typedef struct
{
   GLXContext return_value;
   Display *dpy;
   XVisualInfo *vis;
   GLXContext shareList;
   Bool direct;

} Thread_Command_glXCreateContext;

static void
_gl_thread_glXCreateContext(void *data)
{
   Thread_Command_glXCreateContext *thread_param =
      (Thread_Command_glXCreateContext *)data;

   thread_param->return_value = glXCreateContext(thread_param->dpy,
                                                 thread_param->vis,
                                                 thread_param->shareList,
                                                 thread_param->direct);

}

EAPI GLXContext
glXCreateContext_thread_cmd(Display *dpy, XVisualInfo *vis, GLXContext shareList, Bool direct)
{
   if (!evas_gl_thread_enabled())
     {
        return glXCreateContext(dpy, vis, shareList, direct);
     }

   int thread_mode = EVAS_GL_THREAD_MODE_FINISH;

   Thread_Command_glXCreateContext thread_param_local;
   Thread_Command_glXCreateContext *thread_param = &thread_param_local;

   thread_param->dpy = dpy;
   thread_param->vis = vis;
   thread_param->shareList = shareList;
   thread_param->direct = direct;

   evas_gl_thread_cmd_enqueue(EVAS_GL_THREAD_TYPE_GL,
                              _gl_thread_glXCreateContext,
                              thread_param,
                              thread_mode);

   return thread_param->return_value;
}

/*
   GLXContext
   glXCreateNewContext(Display *dpy, GLXFBConfig config, int renderType, GLXContext shareList, Bool direct);
 */

typedef struct
{
   GLXContext return_value;
   Display *dpy;
   GLXFBConfig config;
   int renderType;
   GLXContext shareList;
   Bool direct;

} Thread_Command_glXCreateNewContext;

static void
_gl_thread_glXCreateNewContext(void *data)
{
   Thread_Command_glXCreateNewContext *thread_param =
      (Thread_Command_glXCreateNewContext *)data;

   thread_param->return_value = glXCreateNewContext(thread_param->dpy,
                                                    thread_param->config,
                                                    thread_param->renderType,
                                                    thread_param->shareList,
                                                    thread_param->direct);

}

EAPI GLXContext
glXCreateNewContext_thread_cmd(Display *dpy, GLXFBConfig config, int renderType, GLXContext shareList, Bool direct)
{
   if (!evas_gl_thread_enabled())
     {
        return glXCreateNewContext(dpy, config, renderType, shareList, direct);
     }

   int thread_mode = EVAS_GL_THREAD_MODE_FINISH;

   Thread_Command_glXCreateNewContext thread_param_local;
   Thread_Command_glXCreateNewContext *thread_param = &thread_param_local;

   thread_param->dpy = dpy;
   thread_param->config = config;
   thread_param->renderType = renderType;
   thread_param->shareList = shareList;
   thread_param->direct = direct;

   evas_gl_thread_cmd_enqueue(EVAS_GL_THREAD_TYPE_GL,
                              _gl_thread_glXCreateNewContext,
                              thread_param,
                              thread_mode);

   return thread_param->return_value;
}

/*
   GLXPbuffer
   glXCreatePbuffer(Display *dpy, GLXFBConfig config, const int *attribList);
 */

typedef struct
{
   GLXPbuffer return_value;
   Display *dpy;
   GLXFBConfig config;
   const int *attribList;

} Thread_Command_glXCreatePbuffer;

static void
_gl_thread_glXCreatePbuffer(void *data)
{
   Thread_Command_glXCreatePbuffer *thread_param =
      (Thread_Command_glXCreatePbuffer *)data;

   thread_param->return_value = glXCreatePbuffer(thread_param->dpy,
                                                 thread_param->config,
                                                 thread_param->attribList);

}

EAPI GLXPbuffer
glXCreatePbuffer_thread_cmd(Display *dpy, GLXFBConfig config, const int *attribList)
{
   if (!evas_gl_thread_enabled())
     {
        return glXCreatePbuffer(dpy, config, attribList);
     }

   int thread_mode = EVAS_GL_THREAD_MODE_FINISH;

   Thread_Command_glXCreatePbuffer thread_param_local;
   Thread_Command_glXCreatePbuffer *thread_param = &thread_param_local;

   thread_param->dpy = dpy;
   thread_param->config = config;
   thread_param->attribList = attribList;

   evas_gl_thread_cmd_enqueue(EVAS_GL_THREAD_TYPE_GL,
                              _gl_thread_glXCreatePbuffer,
                              thread_param,
                              thread_mode);

   return thread_param->return_value;
}

/*
   GLXPixmap
   glXCreatePixmap(Display *dpy, GLXFBConfig config, Pixmap pixmap, const int *attribList);
 */

typedef struct
{
   GLXPixmap return_value;
   Display *dpy;
   GLXFBConfig config;
   Pixmap pixmap;
   const int *attribList;

} Thread_Command_glXCreatePixmap;

GLXPixmap (*orig_evas_glXCreatePixmap)(Display *dpy, GLXFBConfig config, Pixmap pixmap, const int *attribList);

EAPI void
glXCreatePixmap_orig_evas_set(void *func)
{
   orig_evas_glXCreatePixmap = func;
}

EAPI void *
glXCreatePixmap_orig_evas_get(void)
{
   return orig_evas_glXCreatePixmap;
}


static void
_gl_thread_glXCreatePixmap(void *data)
{
   Thread_Command_glXCreatePixmap *thread_param =
      (Thread_Command_glXCreatePixmap *)data;

   thread_param->return_value = orig_evas_glXCreatePixmap(thread_param->dpy,
                                                          thread_param->config,
                                                          thread_param->pixmap,
                                                          thread_param->attribList);

}

EAPI GLXPixmap
glXCreatePixmap_thread_cmd(Display *dpy, GLXFBConfig config, Pixmap pixmap, const int *attribList)
{
   if (!evas_gl_thread_enabled())
     {
        return orig_evas_glXCreatePixmap(dpy, config, pixmap, attribList);
     }

   int thread_mode = EVAS_GL_THREAD_MODE_FINISH;

   Thread_Command_glXCreatePixmap thread_param_local;
   Thread_Command_glXCreatePixmap *thread_param = &thread_param_local;

   thread_param->dpy = dpy;
   thread_param->config = config;
   thread_param->pixmap = pixmap;
   thread_param->attribList = attribList;

   evas_gl_thread_cmd_enqueue(EVAS_GL_THREAD_TYPE_GL,
                              _gl_thread_glXCreatePixmap,
                              thread_param,
                              thread_mode);

   return thread_param->return_value;
}

/*
   GLXWindow
   glXCreateWindow(Display *dpy, GLXFBConfig config, Window win, const int *attribList);
 */

typedef struct
{
   GLXWindow return_value;
   Display *dpy;
   GLXFBConfig config;
   Window win;
   const int *attribList;

} Thread_Command_glXCreateWindow;

static void
_gl_thread_glXCreateWindow(void *data)
{
   Thread_Command_glXCreateWindow *thread_param =
      (Thread_Command_glXCreateWindow *)data;

   thread_param->return_value = glXCreateWindow(thread_param->dpy,
                                                thread_param->config,
                                                thread_param->win,
                                                thread_param->attribList);

}

EAPI GLXWindow
glXCreateWindow_thread_cmd(Display *dpy, GLXFBConfig config, Window win, const int *attribList)
{
   if (!evas_gl_thread_enabled())
     {
        return glXCreateWindow(dpy, config, win, attribList);
     }

   int thread_mode = EVAS_GL_THREAD_MODE_FINISH;

   Thread_Command_glXCreateWindow thread_param_local;
   Thread_Command_glXCreateWindow *thread_param = &thread_param_local;

   thread_param->dpy = dpy;
   thread_param->config = config;
   thread_param->win = win;
   thread_param->attribList = attribList;

   evas_gl_thread_cmd_enqueue(EVAS_GL_THREAD_TYPE_GL,
                              _gl_thread_glXCreateWindow,
                              thread_param,
                              thread_mode);

   return thread_param->return_value;
}

/*
   void
   glXDestroyPbuffer(Display *dpy, GLXPbuffer pbuf);
 */

typedef struct
{
   Display *dpy;
   GLXPbuffer pbuf;

} Thread_Command_glXDestroyPbuffer;

static void
_gl_thread_glXDestroyPbuffer(void *data)
{
   Thread_Command_glXDestroyPbuffer *thread_param =
      (Thread_Command_glXDestroyPbuffer *)data;

   glXDestroyPbuffer(thread_param->dpy,
                     thread_param->pbuf);

}

EAPI void
glXDestroyPbuffer_thread_cmd(Display *dpy, GLXPbuffer pbuf)
{
   if (!evas_gl_thread_enabled())
     {
        glXDestroyPbuffer(dpy, pbuf);
        return;
     }

   int thread_mode = EVAS_GL_THREAD_MODE_FINISH;

   Thread_Command_glXDestroyPbuffer thread_param_local;
   Thread_Command_glXDestroyPbuffer *thread_param = &thread_param_local;

   thread_param->dpy = dpy;
   thread_param->pbuf = pbuf;

   evas_gl_thread_cmd_enqueue(EVAS_GL_THREAD_TYPE_GL,
                              _gl_thread_glXDestroyPbuffer,
                              thread_param,
                              thread_mode);
}

/*
   void
   glXDestroyPixmap(Display *dpy, GLXPixmap pixmap);
 */

typedef struct
{
   Display *dpy;
   GLXPixmap pixmap;

} Thread_Command_glXDestroyPixmap;

void (*orig_evas_glXDestroyPixmap)(Display *dpy, GLXPixmap pixmap);

EAPI void
glXDestroyPixmap_orig_evas_set(void *func)
{
   orig_evas_glXDestroyPixmap = func;
}

EAPI void *
glXDestroyPixmap_orig_evas_get(void)
{
   return orig_evas_glXDestroyPixmap;
}

static void
_gl_thread_glXDestroyPixmap(void *data)
{
   Thread_Command_glXDestroyPixmap *thread_param =
      (Thread_Command_glXDestroyPixmap *)data;

   orig_evas_glXDestroyPixmap(thread_param->dpy,
                              thread_param->pixmap);

}

EAPI void
glXDestroyPixmap_thread_cmd(Display *dpy, GLXPixmap pixmap)
{
   if (!evas_gl_thread_enabled())
     {
        orig_evas_glXDestroyPixmap(dpy, pixmap);
        return;
     }

   int thread_mode = EVAS_GL_THREAD_MODE_FINISH;

   Thread_Command_glXDestroyPixmap thread_param_local;
   Thread_Command_glXDestroyPixmap *thread_param = &thread_param_local;

   thread_param->dpy = dpy;
   thread_param->pixmap = pixmap;

   evas_gl_thread_cmd_enqueue(EVAS_GL_THREAD_TYPE_GL,
                              _gl_thread_glXDestroyPixmap,
                              thread_param,
                              thread_mode);
}

/*
   void
   glXDestroyWindow(Display *dpy, GLXWindow window);
 */

typedef struct
{
   Display *dpy;
   GLXWindow window;

} Thread_Command_glXDestroyWindow;

static void
_gl_thread_glXDestroyWindow(void *data)
{
   Thread_Command_glXDestroyWindow *thread_param =
      (Thread_Command_glXDestroyWindow *)data;

   glXDestroyWindow(thread_param->dpy,
                    thread_param->window);

}

EAPI void
glXDestroyWindow_thread_cmd(Display *dpy, GLXWindow window)
{
   if (!evas_gl_thread_enabled())
     {
        glXDestroyWindow(dpy, window);
        return;
     }

   int thread_mode = EVAS_GL_THREAD_MODE_FINISH;

   Thread_Command_glXDestroyWindow thread_param_local;
   Thread_Command_glXDestroyWindow *thread_param = &thread_param_local;

   thread_param->dpy = dpy;
   thread_param->window = window;

   evas_gl_thread_cmd_enqueue(EVAS_GL_THREAD_TYPE_GL,
                              _gl_thread_glXDestroyWindow,
                              thread_param,
                              thread_mode);
}

/*
   int
   glXGetConfig(Display *dpy, XVisualInfo *visual, int attrib, int *value);
 */

typedef struct
{
   int return_value;
   Display *dpy;
   XVisualInfo *visual;
   int attrib;
   int *value;

} Thread_Command_glXGetConfig;

static void
_gl_thread_glXGetConfig(void *data)
{
   Thread_Command_glXGetConfig *thread_param =
      (Thread_Command_glXGetConfig *)data;

   thread_param->return_value = glXGetConfig(thread_param->dpy,
                                             thread_param->visual,
                                             thread_param->attrib,
                                             thread_param->value);

}

EAPI int
glXGetConfig_thread_cmd(Display *dpy, XVisualInfo *visual, int attrib, int *value)
{
   if (!evas_gl_thread_enabled())
     {
        return glXGetConfig(dpy, visual, attrib, value);
     }

   int thread_mode = EVAS_GL_THREAD_MODE_FINISH;

   Thread_Command_glXGetConfig thread_param_local;
   Thread_Command_glXGetConfig *thread_param = &thread_param_local;

   thread_param->dpy = dpy;
   thread_param->visual = visual;
   thread_param->attrib = attrib;
   thread_param->value = value;

   evas_gl_thread_cmd_enqueue(EVAS_GL_THREAD_TYPE_GL,
                              _gl_thread_glXGetConfig,
                              thread_param,
                              thread_mode);

   return thread_param->return_value;
}

/*
   int
   glXGetFBConfigAttrib(Display *dpy, GLXFBConfig config, int attribute, int *value);
 */

typedef struct
{
   int return_value;
   Display *dpy;
   GLXFBConfig config;
   int attribute;
   int *value;

} Thread_Command_glXGetFBConfigAttrib;

static void
_gl_thread_glXGetFBConfigAttrib(void *data)
{
   Thread_Command_glXGetFBConfigAttrib *thread_param =
      (Thread_Command_glXGetFBConfigAttrib *)data;

   thread_param->return_value = glXGetFBConfigAttrib(thread_param->dpy,
                                                     thread_param->config,
                                                     thread_param->attribute,
                                                     thread_param->value);

}

EAPI int
glXGetFBConfigAttrib_thread_cmd(Display *dpy, GLXFBConfig config, int attribute, int *value)
{
   if (!evas_gl_thread_enabled())
     {
        return glXGetFBConfigAttrib(dpy, config, attribute, value);
     }

   int thread_mode = EVAS_GL_THREAD_MODE_FINISH;

   Thread_Command_glXGetFBConfigAttrib thread_param_local;
   Thread_Command_glXGetFBConfigAttrib *thread_param = &thread_param_local;

   thread_param->dpy = dpy;
   thread_param->config = config;
   thread_param->attribute = attribute;
   thread_param->value = value;

   evas_gl_thread_cmd_enqueue(EVAS_GL_THREAD_TYPE_GL,
                              _gl_thread_glXGetFBConfigAttrib,
                              thread_param,
                              thread_mode);

   return thread_param->return_value;
}

/*
   int
   glXGetVideoSync(unsigned int *count);
 */

typedef struct
{
   int return_value;
   unsigned int *count;

} Thread_Command_glXGetVideoSync;

int (*orig_evas_glXGetVideoSync)(unsigned int *count);

EAPI void
glXGetVideoSync_orig_evas_set(void *func)
{
   orig_evas_glXGetVideoSync = func;
}

EAPI void *
glXGetVideoSync_orig_evas_get(void)
{
   return orig_evas_glXGetVideoSync;
}

static void
_gl_thread_glXGetVideoSync(void *data)
{
   Thread_Command_glXGetVideoSync *thread_param =
      (Thread_Command_glXGetVideoSync *)data;

   thread_param->return_value = orig_evas_glXGetVideoSync(thread_param->count);

}

EAPI int
glXGetVideoSync_thread_cmd(unsigned int *count)
{
   if (!evas_gl_thread_enabled())
     {
        return orig_evas_glXGetVideoSync(count);
     }

   int thread_mode = EVAS_GL_THREAD_MODE_FINISH;

   Thread_Command_glXGetVideoSync thread_param_local;
   Thread_Command_glXGetVideoSync *thread_param = &thread_param_local;

   thread_param->count = count;

   evas_gl_thread_cmd_enqueue(EVAS_GL_THREAD_TYPE_GL,
                              _gl_thread_glXGetVideoSync,
                              thread_param,
                              thread_mode);

   return thread_param->return_value;
}

/*
   XVisualInfo *
   glXGetVisualFromFBConfig(Display *dpy, GLXFBConfig config);
 */

typedef struct
{
   XVisualInfo * return_value;
   Display *dpy;
   GLXFBConfig config;

} Thread_Command_glXGetVisualFromFBConfig;

static void
_gl_thread_glXGetVisualFromFBConfig(void *data)
{
   Thread_Command_glXGetVisualFromFBConfig *thread_param =
      (Thread_Command_glXGetVisualFromFBConfig *)data;

   thread_param->return_value = glXGetVisualFromFBConfig(thread_param->dpy,
                                                         thread_param->config);

}

EAPI XVisualInfo *
glXGetVisualFromFBConfig_thread_cmd(Display *dpy, GLXFBConfig config)
{
   if (!evas_gl_thread_enabled())
     {
        return glXGetVisualFromFBConfig(dpy, config);
     }

   int thread_mode = EVAS_GL_THREAD_MODE_FINISH;

   Thread_Command_glXGetVisualFromFBConfig thread_param_local;
   Thread_Command_glXGetVisualFromFBConfig *thread_param = &thread_param_local;

   thread_param->dpy = dpy;
   thread_param->config = config;

   evas_gl_thread_cmd_enqueue(EVAS_GL_THREAD_TYPE_GL,
                              _gl_thread_glXGetVisualFromFBConfig,
                              thread_param,
                              thread_mode);

   return thread_param->return_value;
}

/*
   Bool
   glXMakeContextCurrent(Display *dpy, GLXDrawable draw, GLXDrawable read, GLXContext ctx);
 */

typedef struct
{
   Bool return_value;
   Display *dpy;
   GLXDrawable draw;
   GLXDrawable read;
   GLXContext ctx;

} Thread_Command_glXMakeContextCurrent;


GLXContext current_thread_ctx = NULL;
GLXDrawable current_thread_draw = NULL;
GLXDrawable current_thread_read = NULL;

static void
_gl_thread_glXMakeContextCurrent(void *data)
{
   Thread_Command_glXMakeContextCurrent *thread_param =
      (Thread_Command_glXMakeContextCurrent *)data;

   fprintf(stderr,"THREAD >> OTHER THREAD MAKECONTEXTCURRENT : (%p, %p, %p, %p)\n",
                                 thread_param->dpy, (void *)thread_param->draw,
                                 (void* )thread_param->read, thread_param->ctx);

   thread_param->return_value = glXMakeContextCurrent(thread_param->dpy,
                                                      thread_param->draw,
                                                      thread_param->read,
                                                      thread_param->ctx);

   if (thread_param->return_value)
     {
        current_thread_ctx = thread_param->ctx;
        current_thread_draw = thread_param->draw;
        current_thread_read = thread_param->read;
     }

}

EAPI Bool
glXMakeContextCurrent_thread_cmd(Display *dpy, GLXDrawable draw, GLXDrawable read, GLXContext ctx)
{
   if (!evas_gl_thread_enabled())
     {
        return glXMakeContextCurrent(dpy, draw, read, ctx);
     }

   if (current_thread_ctx == ctx &&
       current_thread_draw == draw &&
       current_thread_read == read)
      return True;

   int thread_mode = EVAS_GL_THREAD_MODE_FINISH;

   Thread_Command_glXMakeContextCurrent thread_param_local;
   Thread_Command_glXMakeContextCurrent *thread_param = &thread_param_local;

   thread_param->dpy = dpy;
   thread_param->draw = draw;
   thread_param->read = read;
   thread_param->ctx = ctx;

   evas_gl_thread_cmd_enqueue(EVAS_GL_THREAD_TYPE_GL,
                              _gl_thread_glXMakeContextCurrent,
                              thread_param,
                              thread_mode);

   return thread_param->return_value;
}

/*
   GLXContext
   glXGetCurrentContext(void);
 */

EAPI GLXContext
glXGetCurrentContext_thread_cmd(void)
{
   if (!evas_gl_thread_enabled())
     {
        return glXGetCurrentContext();
     }

   return current_thread_ctx;
}

/*
   void
   glXDestroyContext(Display *dpy, GLXContext ctx);
 */

typedef struct
{
   Display *dpy;
   GLXContext ctx;

} Thread_Command_glXDestroyContext;

static void
_gl_thread_glXDestroyContext(void *data)
{
   Thread_Command_glXDestroyContext *thread_param =
      (Thread_Command_glXDestroyContext *)data;

   glXDestroyContext(thread_param->dpy,
                     thread_param->ctx);

}

EAPI void
glXDestroyContext_thread_cmd(Display *dpy, GLXContext ctx)
{
   if (!evas_gl_thread_enabled())
     {
        glXDestroyContext(dpy, ctx);
        return;
     }

   int thread_mode = EVAS_GL_THREAD_MODE_FINISH;

   Thread_Command_glXDestroyContext thread_param_local;
   Thread_Command_glXDestroyContext *thread_param = &thread_param_local;

   thread_param->dpy = dpy;
   thread_param->ctx = ctx;

   evas_gl_thread_cmd_enqueue(EVAS_GL_THREAD_TYPE_GL,
                              _gl_thread_glXDestroyContext,
                              thread_param,
                              thread_mode);
}


/*
   void
   glXQueryDrawable(Display *dpy, GLXDrawable draw, int attribute, unsigned int *value);
 */

typedef struct
{
   Display *dpy;
   GLXDrawable draw;
   int attribute;
   unsigned int *value;

} Thread_Command_glXQueryDrawable;

void (*orig_evas_glXQueryDrawable)(Display *dpy, GLXDrawable draw, int attribute, unsigned int *value);

EAPI void
glXQueryDrawable_orig_evas_set(void *func)
{
   orig_evas_glXQueryDrawable = func;
}

EAPI void *
glXQueryDrawable_orig_evas_get(void)
{
   return orig_evas_glXQueryDrawable;
}

static void
_gl_thread_glXQueryDrawable(void *data)
{
   Thread_Command_glXQueryDrawable *thread_param =
      (Thread_Command_glXQueryDrawable *)data;

   orig_evas_glXQueryDrawable(thread_param->dpy,
                              thread_param->draw,
                              thread_param->attribute,
                              thread_param->value);

}

EAPI void
glXQueryDrawable_thread_cmd(Display *dpy, GLXDrawable draw, int attribute, unsigned int *value)
{
   if (!evas_gl_thread_enabled())
     {
        orig_evas_glXQueryDrawable(dpy, draw, attribute, value);
        return;
     }

   int thread_mode = EVAS_GL_THREAD_MODE_FINISH;

   Thread_Command_glXQueryDrawable thread_param_local;
   Thread_Command_glXQueryDrawable *thread_param = &thread_param_local;

   thread_param->dpy = dpy;
   thread_param->draw = draw;
   thread_param->attribute = attribute;
   thread_param->value = value;

   evas_gl_thread_cmd_enqueue(EVAS_GL_THREAD_TYPE_GL,
                              _gl_thread_glXQueryDrawable,
                              thread_param,
                              thread_mode);
}

/*
   Bool
   glXQueryExtension(Display *dpy, int *errorb, int *event);
 */

typedef struct
{
   Bool return_value;
   Display *dpy;
   int *errorb;
   int *event;

} Thread_Command_glXQueryExtension;

static void
_gl_thread_glXQueryExtension(void *data)
{
   Thread_Command_glXQueryExtension *thread_param =
      (Thread_Command_glXQueryExtension *)data;

   thread_param->return_value = glXQueryExtension(thread_param->dpy,
                                                  thread_param->errorb,
                                                  thread_param->event);

}

EAPI Bool
glXQueryExtension_thread_cmd(Display *dpy, int *errorb, int *event)
{
   if (!evas_gl_thread_enabled())
     {
        return glXQueryExtension(dpy, errorb, event);
     }

   int thread_mode = EVAS_GL_THREAD_MODE_FINISH;

   Thread_Command_glXQueryExtension thread_param_local;
   Thread_Command_glXQueryExtension *thread_param = &thread_param_local;

   thread_param->dpy = dpy;
   thread_param->errorb = errorb;
   thread_param->event = event;

   evas_gl_thread_cmd_enqueue(EVAS_GL_THREAD_TYPE_GL,
                              _gl_thread_glXQueryExtension,
                              thread_param,
                              thread_mode);

   return thread_param->return_value;
}

/*
   const char *
   glXQueryExtensionsString(Display *dpy, int screen);
 */

typedef struct
{
   const char * return_value;
   Display *dpy;
   int screen;

} Thread_Command_glXQueryExtensionsString;

static void
_gl_thread_glXQueryExtensionsString(void *data)
{
   Thread_Command_glXQueryExtensionsString *thread_param =
      (Thread_Command_glXQueryExtensionsString *)data;

   thread_param->return_value = glXQueryExtensionsString(thread_param->dpy,
                                                         thread_param->screen);

}

EAPI const char *
glXQueryExtensionsString_thread_cmd(Display *dpy, int screen)
{
   if (!evas_gl_thread_enabled())
     {
        return glXQueryExtensionsString(dpy, screen);
     }

   int thread_mode = EVAS_GL_THREAD_MODE_FINISH;

   Thread_Command_glXQueryExtensionsString thread_param_local;
   Thread_Command_glXQueryExtensionsString *thread_param = &thread_param_local;

   thread_param->dpy = dpy;
   thread_param->screen = screen;

   evas_gl_thread_cmd_enqueue(EVAS_GL_THREAD_TYPE_GL,
                              _gl_thread_glXQueryExtensionsString,
                              thread_param,
                              thread_mode);

   return thread_param->return_value;
}

/*
   Bool
   glXReleaseBuffersMESA(Display *dpy, GLXDrawable drawable);
 */

typedef struct
{
   Bool return_value;
   Display *dpy;
   GLXDrawable drawable;

} Thread_Command_glXReleaseBuffersMESA;

Bool (*orig_evas_glXReleaseBuffersMESA)(Display *dpy, GLXDrawable drawable);

EAPI void
glXReleaseBuffersMESA_orig_evas_set(void *func)
{
   orig_evas_glXReleaseBuffersMESA = func;
}

EAPI void *
glXReleaseBuffersMESA_orig_evas_get(void)
{
   return orig_evas_glXReleaseBuffersMESA;
}

static void
_gl_thread_glXReleaseBuffersMESA(void *data)
{
   Thread_Command_glXReleaseBuffersMESA *thread_param =
      (Thread_Command_glXReleaseBuffersMESA *)data;

   thread_param->return_value = orig_evas_glXReleaseBuffersMESA(thread_param->dpy,
                                                                thread_param->drawable);

}

EAPI Bool
glXReleaseBuffersMESA_thread_cmd(Display *dpy, GLXDrawable drawable)
{
   if (!evas_gl_thread_enabled())
     {
        return orig_evas_glXReleaseBuffersMESA(dpy, drawable);
     }

   int thread_mode = EVAS_GL_THREAD_MODE_FINISH;

   Thread_Command_glXReleaseBuffersMESA thread_param_local;
   Thread_Command_glXReleaseBuffersMESA *thread_param = &thread_param_local;

   thread_param->dpy = dpy;
   thread_param->drawable = drawable;

   evas_gl_thread_cmd_enqueue(EVAS_GL_THREAD_TYPE_GL,
                              _gl_thread_glXReleaseBuffersMESA,
                              thread_param,
                              thread_mode);

   return thread_param->return_value;
}

/*
   void
   glXReleaseTexImage(Display *dpy, GLXDrawable drawable, int buffer);
 */

typedef struct
{
   Display *dpy;
   GLXDrawable drawable;
   int buffer;

} Thread_Command_glXReleaseTexImage;

void (*orig_evas_glXReleaseTexImage)(Display *dpy, GLXDrawable drawable, int buffer);

EAPI void
glXReleaseTexImage_orig_evas_set(void *func)
{
   orig_evas_glXReleaseTexImage = func;
}

EAPI void *
glXReleaseTexImage_orig_evas_get(void)
{
   return orig_evas_glXReleaseTexImage;
}

static void
_gl_thread_glXReleaseTexImage(void *data)
{
   Thread_Command_glXReleaseTexImage *thread_param =
      (Thread_Command_glXReleaseTexImage *)data;

   orig_evas_glXReleaseTexImage(thread_param->dpy,
                                thread_param->drawable,
                                thread_param->buffer);

}

EAPI void
glXReleaseTexImage_thread_cmd(Display *dpy, GLXDrawable drawable, int buffer)
{
   if (!evas_gl_thread_enabled())
     {
        orig_evas_glXReleaseTexImage(dpy, drawable, buffer);
        return;
     }

   int thread_mode = EVAS_GL_THREAD_MODE_FINISH;

   Thread_Command_glXReleaseTexImage thread_param_local;
   Thread_Command_glXReleaseTexImage *thread_param = &thread_param_local;

   thread_param->dpy = dpy;
   thread_param->drawable = drawable;
   thread_param->buffer = buffer;

   evas_gl_thread_cmd_enqueue(EVAS_GL_THREAD_TYPE_GL,
                              _gl_thread_glXReleaseTexImage,
                              thread_param,
                              thread_mode);
}

/*
   void
   glXSwapBuffers(Display *dpy, GLXDrawable drawable);
 */

typedef struct
{
   Display *dpy;
   GLXDrawable drawable;

} Thread_Command_glXSwapBuffers;

static void
_gl_thread_glXSwapBuffers(void *data)
{
   Thread_Command_glXSwapBuffers *thread_param =
      (Thread_Command_glXSwapBuffers *)data;

   glXSwapBuffers(thread_param->dpy,
                  thread_param->drawable);

}

EAPI void
glXSwapBuffers_thread_cmd(Display *dpy, GLXDrawable drawable)
{
   if (!evas_gl_thread_enabled())
     {
        glXSwapBuffers(dpy, drawable);
        return;
     }

   int thread_mode = EVAS_GL_THREAD_MODE_FINISH;

   Thread_Command_glXSwapBuffers thread_param_local;
   Thread_Command_glXSwapBuffers *thread_param = &thread_param_local;

   thread_param->dpy = dpy;
   thread_param->drawable = drawable;

   evas_gl_thread_cmd_enqueue(EVAS_GL_THREAD_TYPE_GL,
                              _gl_thread_glXSwapBuffers,
                              thread_param,
                              thread_mode);
}

/*
   void
   glXSwapIntervalEXT(Display *dpy, GLXDrawable drawable, int interval);
 */

typedef struct
{
   Display *dpy;
   GLXDrawable drawable;
   int interval;

} Thread_Command_glXSwapIntervalEXT;

void (*orig_evas_glXSwapIntervalEXT)(Display *dpy, GLXDrawable drawable, int interval);

EAPI void
glXSwapIntervalEXT_orig_evas_set(void *func)
{
   orig_evas_glXSwapIntervalEXT = func;
}

EAPI void *
glXSwapIntervalEXT_orig_evas_get(void)
{
   return orig_evas_glXSwapIntervalEXT;
}

static void
_gl_thread_glXSwapIntervalEXT(void *data)
{
   Thread_Command_glXSwapIntervalEXT *thread_param =
      (Thread_Command_glXSwapIntervalEXT *)data;

   orig_evas_glXSwapIntervalEXT(thread_param->dpy,
                                thread_param->drawable,
                                thread_param->interval);

}

EAPI void
glXSwapIntervalEXT_thread_cmd(Display *dpy, GLXDrawable drawable, int interval)
{
   if (!evas_gl_thread_enabled())
     {
        orig_evas_glXSwapIntervalEXT(dpy, drawable, interval);
        return;
     }

   int thread_mode = EVAS_GL_THREAD_MODE_FINISH;

   Thread_Command_glXSwapIntervalEXT thread_param_local;
   Thread_Command_glXSwapIntervalEXT *thread_param = &thread_param_local;

   thread_param->dpy = dpy;
   thread_param->drawable = drawable;
   thread_param->interval = interval;

   evas_gl_thread_cmd_enqueue(EVAS_GL_THREAD_TYPE_GL,
                              _gl_thread_glXSwapIntervalEXT,
                              thread_param,
                              thread_mode);
}

/*
   int
   glXSwapIntervalSGI(int interval);
 */

typedef struct
{
   int return_value;
   int interval;

} Thread_Command_glXSwapIntervalSGI;

int (*orig_evas_glXSwapIntervalSGI)(int interval);

EAPI void
glXSwapIntervalSGI_orig_evas_set(void *func)
{
   orig_evas_glXSwapIntervalSGI = func;
}

EAPI void *
glXSwapIntervalSGI_orig_evas_get(void)
{
   return orig_evas_glXSwapIntervalSGI;
}

static void
_gl_thread_glXSwapIntervalSGI(void *data)
{
   Thread_Command_glXSwapIntervalSGI *thread_param =
      (Thread_Command_glXSwapIntervalSGI *)data;

   thread_param->return_value = orig_evas_glXSwapIntervalSGI(thread_param->interval);

}

EAPI int
glXSwapIntervalSGI_thread_cmd(int interval)
{
   if (!evas_gl_thread_enabled())
     {
        return orig_evas_glXSwapIntervalSGI(interval);
     }

   int thread_mode = EVAS_GL_THREAD_MODE_FINISH;

   Thread_Command_glXSwapIntervalSGI thread_param_local;
   Thread_Command_glXSwapIntervalSGI *thread_param = &thread_param_local;

   thread_param->interval = interval;

   evas_gl_thread_cmd_enqueue(EVAS_GL_THREAD_TYPE_GL,
                              _gl_thread_glXSwapIntervalSGI,
                              thread_param,
                              thread_mode);

   return thread_param->return_value;
}

/*
   int
   glXWaitVideoSync(int divisor, int remainder, unsigned int *count);
 */

typedef struct
{
   int return_value;
   int divisor;
   int remainder;
   unsigned int *count;

} Thread_Command_glXWaitVideoSync;

int (*orig_evas_glXWaitVideoSync)(int divisor, int remainder, unsigned int *count);

EAPI void
glXWaitVideoSync_orig_evas_set(void *func)
{
   orig_evas_glXWaitVideoSync = func;
}

EAPI void *
glXWaitVideoSync_orig_evas_get(void)
{
   return orig_evas_glXWaitVideoSync;
}

static void
_gl_thread_glXWaitVideoSync(void *data)
{
   Thread_Command_glXWaitVideoSync *thread_param =
      (Thread_Command_glXWaitVideoSync *)data;

   thread_param->return_value = orig_evas_glXWaitVideoSync(thread_param->divisor,
                                                           thread_param->remainder,
                                                           thread_param->count);

}

EAPI int
glXWaitVideoSync_thread_cmd(int divisor, int remainder, unsigned int *count)
{
   if (!evas_gl_thread_enabled())
     {
        return orig_evas_glXWaitVideoSync(divisor, remainder, count);
     }

   int thread_mode = EVAS_GL_THREAD_MODE_FINISH;

   Thread_Command_glXWaitVideoSync thread_param_local;
   Thread_Command_glXWaitVideoSync *thread_param = &thread_param_local;

   thread_param->divisor = divisor;
   thread_param->remainder = remainder;
   thread_param->count = count;

   evas_gl_thread_cmd_enqueue(EVAS_GL_THREAD_TYPE_GL,
                              _gl_thread_glXWaitVideoSync,
                              thread_param,
                              thread_mode);

   return thread_param->return_value;
}

/****** EVAS GL ******/
/*
   void
   glXBindTexImage(Display *dpy, GLXDrawable drawable, int buffer, const int *attrib_list);
 */

typedef struct
{
   Display *dpy;
   GLXDrawable drawable;
   int buffer;
   const int *attrib_list;

} EVGL_Thread_Command_glXBindTexImage;

void (*orig_evgl_glXBindTexImage)(Display *dpy, GLXDrawable drawable, int buffer, const int *attrib_list);

void
glXBindTexImage_orig_evgl_set(void *func)
{
   orig_evgl_glXBindTexImage = func;
}

void *
glXBindTexImage_orig_evgl_get(void)
{
   return orig_evgl_glXBindTexImage;
}

static void
_evgl_thread_glXBindTexImage(void *data)
{
   EVGL_Thread_Command_glXBindTexImage *thread_param =
      (EVGL_Thread_Command_glXBindTexImage *)data;

   orig_evgl_glXBindTexImage(thread_param->dpy,
                             thread_param->drawable,
                             thread_param->buffer,
                             thread_param->attrib_list);

}

EAPI void
glXBindTexImage_evgl_thread_cmd(Display *dpy, GLXDrawable drawable, int buffer, const int *attrib_list)
{
   if (!evas_evgl_thread_enabled())
     {
        orig_evgl_glXBindTexImage(dpy, drawable, buffer, attrib_list);
        return;
     }

   int thread_mode = EVAS_GL_THREAD_MODE_FINISH;

   EVGL_Thread_Command_glXBindTexImage thread_param_local;
   EVGL_Thread_Command_glXBindTexImage *thread_param = &thread_param_local;

   thread_param->dpy = dpy;
   thread_param->drawable = drawable;
   thread_param->buffer = buffer;
   thread_param->attrib_list = attrib_list;

   evas_gl_thread_cmd_enqueue(EVAS_GL_THREAD_TYPE_EVGL,
                              _evgl_thread_glXBindTexImage,
                              thread_param,
                              thread_mode);
}

/*
   GLXFBConfig *
   glXChooseFBConfig(Display *dpy, int screen, const int *attribList, int *nitems);
 */

typedef struct
{
   GLXFBConfig * return_value;
   Display *dpy;
   int screen;
   const int *attribList;
   int *nitems;

} EVGL_Thread_Command_glXChooseFBConfig;

static void
_evgl_thread_glXChooseFBConfig(void *data)
{
   EVGL_Thread_Command_glXChooseFBConfig *thread_param =
      (EVGL_Thread_Command_glXChooseFBConfig *)data;

   thread_param->return_value = glXChooseFBConfig(thread_param->dpy,
                                                  thread_param->screen,
                                                  thread_param->attribList,
                                                  thread_param->nitems);

}

EAPI GLXFBConfig *
glXChooseFBConfig_evgl_thread_cmd(Display *dpy, int screen, const int *attribList, int *nitems)
{
   if (!evas_evgl_thread_enabled())
     {
        return glXChooseFBConfig(dpy, screen, attribList, nitems);
     }

   int thread_mode = EVAS_GL_THREAD_MODE_FINISH;

   EVGL_Thread_Command_glXChooseFBConfig thread_param_local;
   EVGL_Thread_Command_glXChooseFBConfig *thread_param = &thread_param_local;

   thread_param->dpy = dpy;
   thread_param->screen = screen;
   thread_param->attribList = attribList;
   thread_param->nitems = nitems;

   evas_gl_thread_cmd_enqueue(EVAS_GL_THREAD_TYPE_EVGL,
                              _evgl_thread_glXChooseFBConfig,
                              thread_param,
                              thread_mode);

   return thread_param->return_value;
}

/*
   GLXContext
   glXCreateContext(Display *dpy, XVisualInfo *vis, GLXContext shareList, Bool direct);
 */

typedef struct
{
   GLXContext return_value;
   Display *dpy;
   XVisualInfo *vis;
   GLXContext shareList;
   Bool direct;

} EVGL_Thread_Command_glXCreateContext;

static void
_evgl_thread_glXCreateContext(void *data)
{
   EVGL_Thread_Command_glXCreateContext *thread_param =
      (EVGL_Thread_Command_glXCreateContext *)data;

   thread_param->return_value = glXCreateContext(thread_param->dpy,
                                                 thread_param->vis,
                                                 thread_param->shareList,
                                                 thread_param->direct);

}

EAPI GLXContext
glXCreateContext_evgl_thread_cmd(Display *dpy, XVisualInfo *vis, GLXContext shareList, Bool direct)
{
   if (!evas_evgl_thread_enabled())
     {
        return glXCreateContext(dpy, vis, shareList, direct);
     }

   int thread_mode = EVAS_GL_THREAD_MODE_FINISH;

   EVGL_Thread_Command_glXCreateContext thread_param_local;
   EVGL_Thread_Command_glXCreateContext *thread_param = &thread_param_local;

   thread_param->dpy = dpy;
   thread_param->vis = vis;
   thread_param->shareList = shareList;
   thread_param->direct = direct;

   evas_gl_thread_cmd_enqueue(EVAS_GL_THREAD_TYPE_EVGL,
                              _evgl_thread_glXCreateContext,
                              thread_param,
                              thread_mode);

   return thread_param->return_value;
}

/*
   GLXContext
   glXCreateNewContext(Display *dpy, GLXFBConfig config, int renderType, GLXContext shareList, Bool direct);
 */

typedef struct
{
   GLXContext return_value;
   Display *dpy;
   GLXFBConfig config;
   int renderType;
   GLXContext shareList;
   Bool direct;

} EVGL_Thread_Command_glXCreateNewContext;

static void
_evgl_thread_glXCreateNewContext(void *data)
{
   EVGL_Thread_Command_glXCreateNewContext *thread_param =
      (EVGL_Thread_Command_glXCreateNewContext *)data;

   thread_param->return_value = glXCreateNewContext(thread_param->dpy,
                                                    thread_param->config,
                                                    thread_param->renderType,
                                                    thread_param->shareList,
                                                    thread_param->direct);

}

EAPI GLXContext
glXCreateNewContext_evgl_thread_cmd(Display *dpy, GLXFBConfig config, int renderType, GLXContext shareList, Bool direct)
{
   if (!evas_evgl_thread_enabled())
     {
        return glXCreateNewContext(dpy, config, renderType, shareList, direct);
     }

   int thread_mode = EVAS_GL_THREAD_MODE_FINISH;

   EVGL_Thread_Command_glXCreateNewContext thread_param_local;
   EVGL_Thread_Command_glXCreateNewContext *thread_param = &thread_param_local;

   thread_param->dpy = dpy;
   thread_param->config = config;
   thread_param->renderType = renderType;
   thread_param->shareList = shareList;
   thread_param->direct = direct;

   evas_gl_thread_cmd_enqueue(EVAS_GL_THREAD_TYPE_EVGL,
                              _evgl_thread_glXCreateNewContext,
                              thread_param,
                              thread_mode);

   return thread_param->return_value;
}

/*
   GLXPbuffer
   glXCreatePbuffer(Display *dpy, GLXFBConfig config, const int *attribList);
 */

typedef struct
{
   GLXPbuffer return_value;
   Display *dpy;
   GLXFBConfig config;
   const int *attribList;

} EVGL_Thread_Command_glXCreatePbuffer;

static void
_evgl_thread_glXCreatePbuffer(void *data)
{
   EVGL_Thread_Command_glXCreatePbuffer *thread_param =
      (EVGL_Thread_Command_glXCreatePbuffer *)data;

   thread_param->return_value = glXCreatePbuffer(thread_param->dpy,
                                                 thread_param->config,
                                                 thread_param->attribList);

}

EAPI GLXPbuffer
glXCreatePbuffer_evgl_thread_cmd(Display *dpy, GLXFBConfig config, const int *attribList)
{
   if (!evas_evgl_thread_enabled())
     {
        return glXCreatePbuffer(dpy, config, attribList);
     }

   int thread_mode = EVAS_GL_THREAD_MODE_FINISH;

   EVGL_Thread_Command_glXCreatePbuffer thread_param_local;
   EVGL_Thread_Command_glXCreatePbuffer *thread_param = &thread_param_local;

   thread_param->dpy = dpy;
   thread_param->config = config;
   thread_param->attribList = attribList;

   evas_gl_thread_cmd_enqueue(EVAS_GL_THREAD_TYPE_EVGL,
                              _evgl_thread_glXCreatePbuffer,
                              thread_param,
                              thread_mode);

   return thread_param->return_value;
}

/*
   GLXPixmap
   glXCreatePixmap(Display *dpy, GLXFBConfig config, Pixmap pixmap, const int *attribList);
 */

typedef struct
{
   GLXPixmap return_value;
   Display *dpy;
   GLXFBConfig config;
   Pixmap pixmap;
   const int *attribList;

} EVGL_Thread_Command_glXCreatePixmap;

GLXPixmap (*orig_evgl_glXCreatePixmap)(Display *dpy, GLXFBConfig config, Pixmap pixmap, const int *attribList);

void
glXCreatePixmap_orig_evgl_set(void *func)
{
   orig_evgl_glXCreatePixmap = func;
}

void *
glXCreatePixmap_orig_evgl_get(void)
{
   return orig_evgl_glXCreatePixmap;
}

static void
_evgl_thread_glXCreatePixmap(void *data)
{
   EVGL_Thread_Command_glXCreatePixmap *thread_param =
      (EVGL_Thread_Command_glXCreatePixmap *)data;

   thread_param->return_value = orig_evgl_glXCreatePixmap(thread_param->dpy,
                                                          thread_param->config,
                                                          thread_param->pixmap,
                                                          thread_param->attribList);

}

EAPI GLXPixmap
glXCreatePixmap_evgl_thread_cmd(Display *dpy, GLXFBConfig config, Pixmap pixmap, const int *attribList)
{
   if (!evas_evgl_thread_enabled())
     {
        return orig_evgl_glXCreatePixmap(dpy, config, pixmap, attribList);
     }

   int thread_mode = EVAS_GL_THREAD_MODE_FINISH;

   EVGL_Thread_Command_glXCreatePixmap thread_param_local;
   EVGL_Thread_Command_glXCreatePixmap *thread_param = &thread_param_local;

   thread_param->dpy = dpy;
   thread_param->config = config;
   thread_param->pixmap = pixmap;
   thread_param->attribList = attribList;

   evas_gl_thread_cmd_enqueue(EVAS_GL_THREAD_TYPE_EVGL,
                              _evgl_thread_glXCreatePixmap,
                              thread_param,
                              thread_mode);

   return thread_param->return_value;
}

/*
   GLXWindow
   glXCreateWindow(Display *dpy, GLXFBConfig config, Window win, const int *attribList);
 */

typedef struct
{
   GLXWindow return_value;
   Display *dpy;
   GLXFBConfig config;
   Window win;
   const int *attribList;

} EVGL_Thread_Command_glXCreateWindow;

static void
_evgl_thread_glXCreateWindow(void *data)
{
   EVGL_Thread_Command_glXCreateWindow *thread_param =
      (EVGL_Thread_Command_glXCreateWindow *)data;

   thread_param->return_value = glXCreateWindow(thread_param->dpy,
                                                thread_param->config,
                                                thread_param->win,
                                                thread_param->attribList);

}

EAPI GLXWindow
glXCreateWindow_evgl_thread_cmd(Display *dpy, GLXFBConfig config, Window win, const int *attribList)
{
   if (!evas_evgl_thread_enabled())
     {
        return glXCreateWindow(dpy, config, win, attribList);
     }

   int thread_mode = EVAS_GL_THREAD_MODE_FINISH;

   EVGL_Thread_Command_glXCreateWindow thread_param_local;
   EVGL_Thread_Command_glXCreateWindow *thread_param = &thread_param_local;

   thread_param->dpy = dpy;
   thread_param->config = config;
   thread_param->win = win;
   thread_param->attribList = attribList;

   evas_gl_thread_cmd_enqueue(EVAS_GL_THREAD_TYPE_EVGL,
                              _evgl_thread_glXCreateWindow,
                              thread_param,
                              thread_mode);

   return thread_param->return_value;
}

/*
   void
   glXDestroyContext(Display *dpy, GLXContext ctx);
 */

typedef struct
{
   Display *dpy;
   GLXContext ctx;

} EVGL_Thread_Command_glXDestroyContext;

static void
_evgl_thread_glXDestroyContext(void *data)
{
   EVGL_Thread_Command_glXDestroyContext *thread_param =
      (EVGL_Thread_Command_glXDestroyContext *)data;

   glXDestroyContext(thread_param->dpy,
                     thread_param->ctx);

}

EAPI void
glXDestroyContext_evgl_thread_cmd(Display *dpy, GLXContext ctx)
{
   if (!evas_evgl_thread_enabled())
     {
        glXDestroyContext(dpy, ctx);
        return;
     }

   int thread_mode = EVAS_GL_THREAD_MODE_FINISH;

   EVGL_Thread_Command_glXDestroyContext thread_param_local;
   EVGL_Thread_Command_glXDestroyContext *thread_param = &thread_param_local;

   thread_param->dpy = dpy;
   thread_param->ctx = ctx;

   evas_gl_thread_cmd_enqueue(EVAS_GL_THREAD_TYPE_EVGL,
                              _evgl_thread_glXDestroyContext,
                              thread_param,
                              thread_mode);
}

/*
   void
   glXDestroyPbuffer(Display *dpy, GLXPbuffer pbuf);
 */

typedef struct
{
   Display *dpy;
   GLXPbuffer pbuf;

} EVGL_Thread_Command_glXDestroyPbuffer;

static void
_evgl_thread_glXDestroyPbuffer(void *data)
{
   EVGL_Thread_Command_glXDestroyPbuffer *thread_param =
      (EVGL_Thread_Command_glXDestroyPbuffer *)data;

   glXDestroyPbuffer(thread_param->dpy,
                     thread_param->pbuf);

}

EAPI void
glXDestroyPbuffer_evgl_thread_cmd(Display *dpy, GLXPbuffer pbuf)
{
   if (!evas_evgl_thread_enabled())
     {
        glXDestroyPbuffer(dpy, pbuf);
        return;
     }

   int thread_mode = EVAS_GL_THREAD_MODE_FINISH;

   EVGL_Thread_Command_glXDestroyPbuffer thread_param_local;
   EVGL_Thread_Command_glXDestroyPbuffer *thread_param = &thread_param_local;

   thread_param->dpy = dpy;
   thread_param->pbuf = pbuf;

   evas_gl_thread_cmd_enqueue(EVAS_GL_THREAD_TYPE_EVGL,
                              _evgl_thread_glXDestroyPbuffer,
                              thread_param,
                              thread_mode);
}

/*
   void
   glXDestroyPixmap(Display *dpy, GLXPixmap pixmap);
 */

typedef struct
{
   Display *dpy;
   GLXPixmap pixmap;

} EVGL_Thread_Command_glXDestroyPixmap;

void (*orig_evgl_glXDestroyPixmap)(Display *dpy, GLXPixmap pixmap);

void
glXDestroyPixmap_orig_evgl_set(void *func)
{
   orig_evgl_glXDestroyPixmap = func;
}

void *
glXDestroyPixmap_orig_evgl_get(void)
{
   return orig_evgl_glXDestroyPixmap;
}

static void
_evgl_thread_glXDestroyPixmap(void *data)
{
   EVGL_Thread_Command_glXDestroyPixmap *thread_param =
      (EVGL_Thread_Command_glXDestroyPixmap *)data;

   orig_evgl_glXDestroyPixmap(thread_param->dpy,
                              thread_param->pixmap);

}

EAPI void
glXDestroyPixmap_evgl_thread_cmd(Display *dpy, GLXPixmap pixmap)
{
   if (!evas_evgl_thread_enabled())
     {
        orig_evgl_glXDestroyPixmap(dpy, pixmap);
        return;
     }

   int thread_mode = EVAS_GL_THREAD_MODE_FINISH;

   EVGL_Thread_Command_glXDestroyPixmap thread_param_local;
   EVGL_Thread_Command_glXDestroyPixmap *thread_param = &thread_param_local;

   thread_param->dpy = dpy;
   thread_param->pixmap = pixmap;

   evas_gl_thread_cmd_enqueue(EVAS_GL_THREAD_TYPE_EVGL,
                              _evgl_thread_glXDestroyPixmap,
                              thread_param,
                              thread_mode);
}

/*
   void
   glXDestroyWindow(Display *dpy, GLXWindow window);
 */

typedef struct
{
   Display *dpy;
   GLXWindow window;

} EVGL_Thread_Command_glXDestroyWindow;

static void
_evgl_thread_glXDestroyWindow(void *data)
{
   EVGL_Thread_Command_glXDestroyWindow *thread_param =
      (EVGL_Thread_Command_glXDestroyWindow *)data;

   glXDestroyWindow(thread_param->dpy,
                    thread_param->window);

}

EAPI void
glXDestroyWindow_evgl_thread_cmd(Display *dpy, GLXWindow window)
{
   if (!evas_evgl_thread_enabled())
     {
        glXDestroyWindow(dpy, window);
        return;
     }

   int thread_mode = EVAS_GL_THREAD_MODE_FINISH;

   EVGL_Thread_Command_glXDestroyWindow thread_param_local;
   EVGL_Thread_Command_glXDestroyWindow *thread_param = &thread_param_local;

   thread_param->dpy = dpy;
   thread_param->window = window;

   evas_gl_thread_cmd_enqueue(EVAS_GL_THREAD_TYPE_EVGL,
                              _evgl_thread_glXDestroyWindow,
                              thread_param,
                              thread_mode);
}

/*
   int
   glXGetConfig(Display *dpy, XVisualInfo *visual, int attrib, int *value);
 */

typedef struct
{
   int return_value;
   Display *dpy;
   XVisualInfo *visual;
   int attrib;
   int *value;

} EVGL_Thread_Command_glXGetConfig;

static void
_evgl_thread_glXGetConfig(void *data)
{
   EVGL_Thread_Command_glXGetConfig *thread_param =
      (EVGL_Thread_Command_glXGetConfig *)data;

   thread_param->return_value = glXGetConfig(thread_param->dpy,
                                             thread_param->visual,
                                             thread_param->attrib,
                                             thread_param->value);

}

EAPI int
glXGetConfig_evgl_thread_cmd(Display *dpy, XVisualInfo *visual, int attrib, int *value)
{
   if (!evas_evgl_thread_enabled())
     {
        return glXGetConfig(dpy, visual, attrib, value);
     }

   int thread_mode = EVAS_GL_THREAD_MODE_FINISH;

   EVGL_Thread_Command_glXGetConfig thread_param_local;
   EVGL_Thread_Command_glXGetConfig *thread_param = &thread_param_local;

   thread_param->dpy = dpy;
   thread_param->visual = visual;
   thread_param->attrib = attrib;
   thread_param->value = value;

   evas_gl_thread_cmd_enqueue(EVAS_GL_THREAD_TYPE_EVGL,
                              _evgl_thread_glXGetConfig,
                              thread_param,
                              thread_mode);

   return thread_param->return_value;
}

/*
   int
   glXGetFBConfigAttrib(Display *dpy, GLXFBConfig config, int attribute, int *value);
 */

typedef struct
{
   int return_value;
   Display *dpy;
   GLXFBConfig config;
   int attribute;
   int *value;

} EVGL_Thread_Command_glXGetFBConfigAttrib;

static void
_evgl_thread_glXGetFBConfigAttrib(void *data)
{
   EVGL_Thread_Command_glXGetFBConfigAttrib *thread_param =
      (EVGL_Thread_Command_glXGetFBConfigAttrib *)data;

   thread_param->return_value = glXGetFBConfigAttrib(thread_param->dpy,
                                                     thread_param->config,
                                                     thread_param->attribute,
                                                     thread_param->value);

}

EAPI int
glXGetFBConfigAttrib_evgl_thread_cmd(Display *dpy, GLXFBConfig config, int attribute, int *value)
{
   if (!evas_evgl_thread_enabled())
     {
        return glXGetFBConfigAttrib(dpy, config, attribute, value);
     }

   int thread_mode = EVAS_GL_THREAD_MODE_FINISH;

   EVGL_Thread_Command_glXGetFBConfigAttrib thread_param_local;
   EVGL_Thread_Command_glXGetFBConfigAttrib *thread_param = &thread_param_local;

   thread_param->dpy = dpy;
   thread_param->config = config;
   thread_param->attribute = attribute;
   thread_param->value = value;

   evas_gl_thread_cmd_enqueue(EVAS_GL_THREAD_TYPE_EVGL,
                              _evgl_thread_glXGetFBConfigAttrib,
                              thread_param,
                              thread_mode);

   return thread_param->return_value;
}

/*
   int
   glXGetVideoSync(unsigned int *count);
 */

typedef struct
{
   int return_value;
   unsigned int *count;

} EVGL_Thread_Command_glXGetVideoSync;

int (*orig_evgl_glXGetVideoSync)(unsigned int *count);

void
glXGetVideoSync_orig_evgl_set(void *func)
{
   orig_evgl_glXGetVideoSync = func;
}

void *
glXGetVideoSync_orig_evgl_get(void)
{
   return orig_evgl_glXGetVideoSync;
}

static void
_evgl_thread_glXGetVideoSync(void *data)
{
   EVGL_Thread_Command_glXGetVideoSync *thread_param =
      (EVGL_Thread_Command_glXGetVideoSync *)data;

   thread_param->return_value = orig_evgl_glXGetVideoSync(thread_param->count);

}

EAPI int
glXGetVideoSync_evgl_thread_cmd(unsigned int *count)
{
   if (!evas_evgl_thread_enabled())
     {
        return orig_evgl_glXGetVideoSync(count);
     }

   int thread_mode = EVAS_GL_THREAD_MODE_FINISH;

   EVGL_Thread_Command_glXGetVideoSync thread_param_local;
   EVGL_Thread_Command_glXGetVideoSync *thread_param = &thread_param_local;

   thread_param->count = count;

   evas_gl_thread_cmd_enqueue(EVAS_GL_THREAD_TYPE_EVGL,
                              _evgl_thread_glXGetVideoSync,
                              thread_param,
                              thread_mode);

   return thread_param->return_value;
}

/*
   XVisualInfo *
   glXGetVisualFromFBConfig(Display *dpy, GLXFBConfig config);
 */

typedef struct
{
   XVisualInfo * return_value;
   Display *dpy;
   GLXFBConfig config;

} EVGL_Thread_Command_glXGetVisualFromFBConfig;

static void
_evgl_thread_glXGetVisualFromFBConfig(void *data)
{
   EVGL_Thread_Command_glXGetVisualFromFBConfig *thread_param =
      (EVGL_Thread_Command_glXGetVisualFromFBConfig *)data;

   thread_param->return_value = glXGetVisualFromFBConfig(thread_param->dpy,
                                                         thread_param->config);

}

EAPI XVisualInfo *
glXGetVisualFromFBConfig_evgl_thread_cmd(Display *dpy, GLXFBConfig config)
{
   if (!evas_evgl_thread_enabled())
     {
        return glXGetVisualFromFBConfig(dpy, config);
     }

   int thread_mode = EVAS_GL_THREAD_MODE_FINISH;

   EVGL_Thread_Command_glXGetVisualFromFBConfig thread_param_local;
   EVGL_Thread_Command_glXGetVisualFromFBConfig *thread_param = &thread_param_local;

   thread_param->dpy = dpy;
   thread_param->config = config;

   evas_gl_thread_cmd_enqueue(EVAS_GL_THREAD_TYPE_EVGL,
                              _evgl_thread_glXGetVisualFromFBConfig,
                              thread_param,
                              thread_mode);

   return thread_param->return_value;
}

/*
   Bool
   glXMakeContextCurrent(Display *dpy, GLXDrawable draw, GLXDrawable read, GLXContext ctx);
 */

typedef struct
{
   Bool return_value;
   Display *dpy;
   GLXDrawable draw;
   GLXDrawable read;
   GLXContext ctx;

} EVGL_Thread_Command_glXMakeContextCurrent;

GLXContext current_evgl_thread_ctx = NULL;
GLXDrawable current_evgl_thread_draw = NULL;
GLXDrawable current_evgl_thread_read = NULL;

static void
_evgl_thread_glXMakeContextCurrent(void *data)
{
   EVGL_Thread_Command_glXMakeContextCurrent *thread_param =
      (EVGL_Thread_Command_glXMakeContextCurrent *)data;

   fprintf(stderr,"EVGL THREAD >> OTHER THREAD MAKECONTEXTCURRENT : (%p, %p, %p, %p)\n",
                                 thread_param->dpy, (void *)thread_param->draw,
                                 (void* )thread_param->read, thread_param->ctx);

   thread_param->return_value = glXMakeContextCurrent(thread_param->dpy,
                                                      thread_param->draw,
                                                      thread_param->read,
                                                      thread_param->ctx);

   if (thread_param->return_value)
     {
        current_evgl_thread_ctx = thread_param->ctx;
        current_evgl_thread_draw = thread_param->draw;
        current_evgl_thread_read = thread_param->read;
     }

}

EAPI Bool
glXMakeContextCurrent_evgl_thread_cmd(Display *dpy, GLXDrawable draw, GLXDrawable read, GLXContext ctx)
{
   if (!evas_evgl_thread_enabled())
     {
        return glXMakeContextCurrent(dpy, draw, read, ctx);
     }

   if (current_evgl_thread_ctx == ctx &&
       current_evgl_thread_draw == draw &&
       current_evgl_thread_read == read)
      return True;

   int thread_mode = EVAS_GL_THREAD_MODE_FINISH;

   EVGL_Thread_Command_glXMakeContextCurrent thread_param_local;
   EVGL_Thread_Command_glXMakeContextCurrent *thread_param = &thread_param_local;

   thread_param->dpy = dpy;
   thread_param->draw = draw;
   thread_param->read = read;
   thread_param->ctx = ctx;

   evas_gl_thread_cmd_enqueue(EVAS_GL_THREAD_TYPE_EVGL,
                              _evgl_thread_glXMakeContextCurrent,
                              thread_param,
                              thread_mode);

   return thread_param->return_value;
}

/*
   GLXContext
   glXGetCurrentContext(void);
 */

EAPI GLXContext
glXGetCurrentContext_evgl_thread_cmd(void)
{
   if (!evas_evgl_thread_enabled())
     {
        return glXGetCurrentContext();
     }

   return current_evgl_thread_ctx;
}


/*
   void
   glXQueryDrawable(Display *dpy, GLXDrawable draw, int attribute, unsigned int *value);
 */

typedef struct
{
   Display *dpy;
   GLXDrawable draw;
   int attribute;
   unsigned int *value;

} EVGL_Thread_Command_glXQueryDrawable;

void (*orig_evgl_glXQueryDrawable)(Display *dpy, GLXDrawable draw, int attribute, unsigned int *value);

void
glXQueryDrawable_orig_evgl_set(void *func)
{
   orig_evgl_glXQueryDrawable = func;
}

void *
glXQueryDrawable_orig_evgl_get(void)
{
   return orig_evgl_glXQueryDrawable;
}

static void
_evgl_thread_glXQueryDrawable(void *data)
{
   EVGL_Thread_Command_glXQueryDrawable *thread_param =
      (EVGL_Thread_Command_glXQueryDrawable *)data;

   orig_evgl_glXQueryDrawable(thread_param->dpy,
                              thread_param->draw,
                              thread_param->attribute,
                              thread_param->value);

}

EAPI void
glXQueryDrawable_evgl_thread_cmd(Display *dpy, GLXDrawable draw, int attribute, unsigned int *value)
{
   if (!evas_evgl_thread_enabled())
     {
        orig_evgl_glXQueryDrawable(dpy, draw, attribute, value);
        return;
     }

   int thread_mode = EVAS_GL_THREAD_MODE_FINISH;

   EVGL_Thread_Command_glXQueryDrawable thread_param_local;
   EVGL_Thread_Command_glXQueryDrawable *thread_param = &thread_param_local;

   thread_param->dpy = dpy;
   thread_param->draw = draw;
   thread_param->attribute = attribute;
   thread_param->value = value;

   evas_gl_thread_cmd_enqueue(EVAS_GL_THREAD_TYPE_EVGL,
                              _evgl_thread_glXQueryDrawable,
                              thread_param,
                              thread_mode);
}

/*
   Bool
   glXQueryExtension(Display *dpy, int *errorb, int *event);
 */

typedef struct
{
   Bool return_value;
   Display *dpy;
   int *errorb;
   int *event;

} EVGL_Thread_Command_glXQueryExtension;

static void
_evgl_thread_glXQueryExtension(void *data)
{
   EVGL_Thread_Command_glXQueryExtension *thread_param =
      (EVGL_Thread_Command_glXQueryExtension *)data;

   thread_param->return_value = glXQueryExtension(thread_param->dpy,
                                                  thread_param->errorb,
                                                  thread_param->event);

}

EAPI Bool
glXQueryExtension_evgl_thread_cmd(Display *dpy, int *errorb, int *event)
{
   if (!evas_evgl_thread_enabled())
     {
        return glXQueryExtension(dpy, errorb, event);
     }

   int thread_mode = EVAS_GL_THREAD_MODE_FINISH;

   EVGL_Thread_Command_glXQueryExtension thread_param_local;
   EVGL_Thread_Command_glXQueryExtension *thread_param = &thread_param_local;

   thread_param->dpy = dpy;
   thread_param->errorb = errorb;
   thread_param->event = event;

   evas_gl_thread_cmd_enqueue(EVAS_GL_THREAD_TYPE_EVGL,
                              _evgl_thread_glXQueryExtension,
                              thread_param,
                              thread_mode);

   return thread_param->return_value;
}

/*
   const char *
   glXQueryExtensionsString(Display *dpy, int screen);
 */

typedef struct
{
   const char * return_value;
   Display *dpy;
   int screen;

} EVGL_Thread_Command_glXQueryExtensionsString;

static void
_evgl_thread_glXQueryExtensionsString(void *data)
{
   EVGL_Thread_Command_glXQueryExtensionsString *thread_param =
      (EVGL_Thread_Command_glXQueryExtensionsString *)data;

   thread_param->return_value = glXQueryExtensionsString(thread_param->dpy,
                                                         thread_param->screen);

}

EAPI const char *
glXQueryExtensionsString_evgl_thread_cmd(Display *dpy, int screen)
{
   if (!evas_evgl_thread_enabled())
     {
        return glXQueryExtensionsString(dpy, screen);
     }

   int thread_mode = EVAS_GL_THREAD_MODE_FINISH;

   EVGL_Thread_Command_glXQueryExtensionsString thread_param_local;
   EVGL_Thread_Command_glXQueryExtensionsString *thread_param = &thread_param_local;

   thread_param->dpy = dpy;
   thread_param->screen = screen;

   evas_gl_thread_cmd_enqueue(EVAS_GL_THREAD_TYPE_EVGL,
                              _evgl_thread_glXQueryExtensionsString,
                              thread_param,
                              thread_mode);

   return thread_param->return_value;
}

/*
   Bool
   glXReleaseBuffersMESA(Display *dpy, GLXDrawable drawable);
 */

typedef struct
{
   Bool return_value;
   Display *dpy;
   GLXDrawable drawable;

} EVGL_Thread_Command_glXReleaseBuffersMESA;

Bool (*orig_evgl_glXReleaseBuffersMESA)(Display *dpy, GLXDrawable drawable);

void
glXReleaseBuffersMESA_orig_evgl_set(void *func)
{
   orig_evgl_glXReleaseBuffersMESA = func;
}

void *
glXReleaseBuffersMESA_orig_evgl_get(void)
{
   return orig_evgl_glXReleaseBuffersMESA;
}

static void
_evgl_thread_glXReleaseBuffersMESA(void *data)
{
   EVGL_Thread_Command_glXReleaseBuffersMESA *thread_param =
      (EVGL_Thread_Command_glXReleaseBuffersMESA *)data;

   thread_param->return_value = orig_evgl_glXReleaseBuffersMESA(thread_param->dpy,
                                                                thread_param->drawable);

}

EAPI Bool
glXReleaseBuffersMESA_evgl_thread_cmd(Display *dpy, GLXDrawable drawable)
{
   if (!evas_evgl_thread_enabled())
     {
        return orig_evgl_glXReleaseBuffersMESA(dpy, drawable);
     }

   int thread_mode = EVAS_GL_THREAD_MODE_FINISH;

   EVGL_Thread_Command_glXReleaseBuffersMESA thread_param_local;
   EVGL_Thread_Command_glXReleaseBuffersMESA *thread_param = &thread_param_local;

   thread_param->dpy = dpy;
   thread_param->drawable = drawable;

   evas_gl_thread_cmd_enqueue(EVAS_GL_THREAD_TYPE_EVGL,
                              _evgl_thread_glXReleaseBuffersMESA,
                              thread_param,
                              thread_mode);

   return thread_param->return_value;
}

/*
   void
   glXReleaseTexImage(Display *dpy, GLXDrawable drawable, int buffer);
 */

typedef struct
{
   Display *dpy;
   GLXDrawable drawable;
   int buffer;

} EVGL_Thread_Command_glXReleaseTexImage;

void (*orig_evgl_glXReleaseTexImage)(Display *dpy, GLXDrawable drawable, int buffer);

void
glXReleaseTexImage_orig_evgl_set(void *func)
{
   orig_evgl_glXReleaseTexImage = func;
}

void *
glXReleaseTexImage_orig_evgl_get(void)
{
   return orig_evgl_glXReleaseTexImage;
}

static void
_evgl_thread_glXReleaseTexImage(void *data)
{
   EVGL_Thread_Command_glXReleaseTexImage *thread_param =
      (EVGL_Thread_Command_glXReleaseTexImage *)data;

   orig_evgl_glXReleaseTexImage(thread_param->dpy,
                                thread_param->drawable,
                                thread_param->buffer);

}

EAPI void
glXReleaseTexImage_evgl_thread_cmd(Display *dpy, GLXDrawable drawable, int buffer)
{
   if (!evas_evgl_thread_enabled())
     {
        orig_evgl_glXReleaseTexImage(dpy, drawable, buffer);
        return;
     }

   int thread_mode = EVAS_GL_THREAD_MODE_FINISH;

   EVGL_Thread_Command_glXReleaseTexImage thread_param_local;
   EVGL_Thread_Command_glXReleaseTexImage *thread_param = &thread_param_local;

   thread_param->dpy = dpy;
   thread_param->drawable = drawable;
   thread_param->buffer = buffer;

   evas_gl_thread_cmd_enqueue(EVAS_GL_THREAD_TYPE_EVGL,
                              _evgl_thread_glXReleaseTexImage,
                              thread_param,
                              thread_mode);
}

/*
   void
   glXSwapBuffers(Display *dpy, GLXDrawable drawable);
 */

typedef struct
{
   Display *dpy;
   GLXDrawable drawable;

} EVGL_Thread_Command_glXSwapBuffers;

static void
_evgl_thread_glXSwapBuffers(void *data)
{
   EVGL_Thread_Command_glXSwapBuffers *thread_param =
      (EVGL_Thread_Command_glXSwapBuffers *)data;

   glXSwapBuffers(thread_param->dpy,
                  thread_param->drawable);

}

EAPI void
glXSwapBuffers_evgl_thread_cmd(Display *dpy, GLXDrawable drawable)
{
   if (!evas_evgl_thread_enabled())
     {
        glXSwapBuffers(dpy, drawable);
        return;
     }

   int thread_mode = EVAS_GL_THREAD_MODE_FINISH;

   EVGL_Thread_Command_glXSwapBuffers thread_param_local;
   EVGL_Thread_Command_glXSwapBuffers *thread_param = &thread_param_local;

   thread_param->dpy = dpy;
   thread_param->drawable = drawable;

   evas_gl_thread_cmd_enqueue(EVAS_GL_THREAD_TYPE_EVGL,
                              _evgl_thread_glXSwapBuffers,
                              thread_param,
                              thread_mode);
}

/*
   void
   glXSwapIntervalEXT(Display *dpy, GLXDrawable drawable, int interval);
 */

typedef struct
{
   Display *dpy;
   GLXDrawable drawable;
   int interval;

} EVGL_Thread_Command_glXSwapIntervalEXT;

void (*orig_evgl_glXSwapIntervalEXT)(Display *dpy, GLXDrawable drawable, int interval);

void
glXSwapIntervalEXT_orig_evgl_set(void *func)
{
   orig_evgl_glXSwapIntervalEXT = func;
}

void *
glXSwapIntervalEXT_orig_evgl_get(void)
{
   return orig_evgl_glXSwapIntervalEXT;
}

static void
_evgl_thread_glXSwapIntervalEXT(void *data)
{
   EVGL_Thread_Command_glXSwapIntervalEXT *thread_param =
      (EVGL_Thread_Command_glXSwapIntervalEXT *)data;

   orig_evgl_glXSwapIntervalEXT(thread_param->dpy,
                                thread_param->drawable,
                                thread_param->interval);

}

EAPI void
glXSwapIntervalEXT_evgl_thread_cmd(Display *dpy, GLXDrawable drawable, int interval)
{
   if (!evas_evgl_thread_enabled())
     {
        orig_evgl_glXSwapIntervalEXT(dpy, drawable, interval);
        return;
     }

   int thread_mode = EVAS_GL_THREAD_MODE_FINISH;

   EVGL_Thread_Command_glXSwapIntervalEXT thread_param_local;
   EVGL_Thread_Command_glXSwapIntervalEXT *thread_param = &thread_param_local;

   thread_param->dpy = dpy;
   thread_param->drawable = drawable;
   thread_param->interval = interval;

   evas_gl_thread_cmd_enqueue(EVAS_GL_THREAD_TYPE_EVGL,
                              _evgl_thread_glXSwapIntervalEXT,
                              thread_param,
                              thread_mode);
}

/*
   int
   glXSwapIntervalSGI(int interval);
 */

typedef struct
{
   int return_value;
   int interval;

} EVGL_Thread_Command_glXSwapIntervalSGI;

int (*orig_evgl_glXSwapIntervalSGI)(int interval);

void
glXSwapIntervalSGI_orig_evgl_set(void *func)
{
   orig_evgl_glXSwapIntervalSGI = func;
}

void *
glXSwapIntervalSGI_orig_evgl_get(void)
{
   return orig_evgl_glXSwapIntervalSGI;
}

static void
_evgl_thread_glXSwapIntervalSGI(void *data)
{
   EVGL_Thread_Command_glXSwapIntervalSGI *thread_param =
      (EVGL_Thread_Command_glXSwapIntervalSGI *)data;

   thread_param->return_value = orig_evgl_glXSwapIntervalSGI(thread_param->interval);

}

EAPI int
glXSwapIntervalSGI_evgl_thread_cmd(int interval)
{
   if (!evas_evgl_thread_enabled())
     {
        return orig_evgl_glXSwapIntervalSGI(interval);
     }

   int thread_mode = EVAS_GL_THREAD_MODE_FINISH;

   EVGL_Thread_Command_glXSwapIntervalSGI thread_param_local;
   EVGL_Thread_Command_glXSwapIntervalSGI *thread_param = &thread_param_local;

   thread_param->interval = interval;

   evas_gl_thread_cmd_enqueue(EVAS_GL_THREAD_TYPE_EVGL,
                              _evgl_thread_glXSwapIntervalSGI,
                              thread_param,
                              thread_mode);

   return thread_param->return_value;
}

/*
   int
   glXWaitVideoSync(int divisor, int remainder, unsigned int *count);
 */

typedef struct
{
   int return_value;
   int divisor;
   int remainder;
   unsigned int *count;

} EVGL_Thread_Command_glXWaitVideoSync;

int (*orig_evgl_glXWaitVideoSync)(int divisor, int remainder, unsigned int *count);

void
glXWaitVideoSync_orig_evgl_set(void *func)
{
   orig_evgl_glXWaitVideoSync = func;
}

void *
glXWaitVideoSync_orig_evgl_get(void)
{
   return orig_evgl_glXWaitVideoSync;
}

static void
_evgl_thread_glXWaitVideoSync(void *data)
{
   EVGL_Thread_Command_glXWaitVideoSync *thread_param =
      (EVGL_Thread_Command_glXWaitVideoSync *)data;

   thread_param->return_value = orig_evgl_glXWaitVideoSync(thread_param->divisor,
                                                           thread_param->remainder,
                                                           thread_param->count);

}

EAPI int
glXWaitVideoSync_evgl_thread_cmd(int divisor, int remainder, unsigned int *count)
{
   if (!evas_evgl_thread_enabled())
     {
        return orig_evgl_glXWaitVideoSync(divisor, remainder, count);
     }

   int thread_mode = EVAS_GL_THREAD_MODE_FINISH;

   EVGL_Thread_Command_glXWaitVideoSync thread_param_local;
   EVGL_Thread_Command_glXWaitVideoSync *thread_param = &thread_param_local;

   thread_param->divisor = divisor;
   thread_param->remainder = remainder;
   thread_param->count = count;

   evas_gl_thread_cmd_enqueue(EVAS_GL_THREAD_TYPE_EVGL,
                              _evgl_thread_glXWaitVideoSync,
                              thread_param,
                              thread_mode);

   return thread_param->return_value;
}


#else /* EVAS_GL_RENDER_THREAD_IS_GENERIC */


#include <dlfcn.h>


void (*glXBindTexImage_orig_evas_set)(void *func) = NULL;
void *(*glXBindTexImage_orig_evas_get)(void) = NULL;
void (*glXBindTexImage_thread_cmd)(Display *dpy, GLXDrawable drawable, int buffer, const int *attrib_list) = NULL;
GLXFBConfig * (*glXChooseFBConfig_thread_cmd)(Display *dpy, int screen, const int *attribList, int *nitems) = NULL;
GLXContext (*glXCreateContext_thread_cmd)(Display *dpy, XVisualInfo *vis, GLXContext shareList, Bool direct) = NULL;
GLXContext (*glXCreateNewContext_thread_cmd)(Display *dpy, GLXFBConfig config, int renderType, GLXContext shareList, Bool direct) = NULL;
GLXPbuffer (*glXCreatePbuffer_thread_cmd)(Display *dpy, GLXFBConfig config, const int *attribList) = NULL;
void (*glXCreatePixmap_orig_evas_set)(void *func) = NULL;
void *(*glXCreatePixmap_orig_evas_get)(void) = NULL;
GLXPixmap (*glXCreatePixmap_thread_cmd)(Display *dpy, GLXFBConfig config, Pixmap pixmap, const int *attribList) = NULL;
GLXWindow (*glXCreateWindow_thread_cmd)(Display *dpy, GLXFBConfig config, Window win, const int *attribList) = NULL;
void (*glXDestroyContext_thread_cmd)(Display *dpy, GLXContext ctx) = NULL;
void (*glXDestroyPbuffer_thread_cmd)(Display *dpy, GLXPbuffer pbuf) = NULL;
void (*glXDestroyPixmap_orig_evas_set)(void *func) = NULL;
void *(*glXDestroyPixmap_orig_evas_get)(void) = NULL;
void (*glXDestroyPixmap_thread_cmd)(Display *dpy, GLXPixmap pixmap) = NULL;
void (*glXDestroyWindow_thread_cmd)(Display *dpy, GLXWindow window) = NULL;
int (*glXGetConfig_thread_cmd)(Display *dpy, XVisualInfo *visual, int attrib, int *value) = NULL;
GLXContext (*glXGetCurrentContext_thread_cmd)(void) = NULL;
int (*glXGetFBConfigAttrib_thread_cmd)(Display *dpy, GLXFBConfig config, int attribute, int *value) = NULL;
void (*glXGetVideoSync_orig_evas_set)(void *func) = NULL;
void *(*glXGetVideoSync_orig_evas_get)(void) = NULL;
int (*glXGetVideoSync_thread_cmd)(unsigned int *count) = NULL;
XVisualInfo * (*glXGetVisualFromFBConfig_thread_cmd)(Display *dpy, GLXFBConfig config) = NULL;
Bool (*glXMakeContextCurrent_thread_cmd)(Display *dpy, GLXDrawable draw, GLXDrawable read, GLXContext ctx) = NULL;
void (*glXQueryDrawable_orig_evas_set)(void *func) = NULL;
void *(*glXQueryDrawable_orig_evas_get)(void) = NULL;
void (*glXQueryDrawable_thread_cmd)(Display *dpy, GLXDrawable draw, int attribute, unsigned int *value) = NULL;
Bool (*glXQueryExtension_thread_cmd)(Display *dpy, int *errorb, int *event) = NULL;
const char * (*glXQueryExtensionsString_thread_cmd)(Display *dpy, int screen) = NULL;
void (*glXReleaseBuffersMESA_orig_evas_set)(void *func) = NULL;
void *(*glXReleaseBuffersMESA_orig_evas_get)(void) = NULL;
Bool (*glXReleaseBuffersMESA_thread_cmd)(Display *dpy, GLXDrawable drawable) = NULL;
void (*glXReleaseTexImage_orig_evas_set)(void *func) = NULL;
void *(*glXReleaseTexImage_orig_evas_get)(void) = NULL;
void (*glXReleaseTexImage_thread_cmd)(Display *dpy, GLXDrawable drawable, int buffer) = NULL;
void (*glXSwapBuffers_thread_cmd)(Display *dpy, GLXDrawable drawable) = NULL;
void (*glXSwapIntervalEXT_orig_evas_set)(void *func) = NULL;
void *(*glXSwapIntervalEXT_orig_evas_get)(void) = NULL;
void (*glXSwapIntervalEXT_thread_cmd)(Display *dpy, GLXDrawable drawable, int interval) = NULL;
void (*glXSwapIntervalSGI_orig_evas_set)(void *func) = NULL;
void *(*glXSwapIntervalSGI_orig_evas_get)(void) = NULL;
int (*glXSwapIntervalSGI_thread_cmd)(int interval) = NULL;
void (*glXWaitVideoSync_orig_evas_set)(void *func) = NULL;
void *(*glXWaitVideoSync_orig_evas_get)(void) = NULL;
int (*glXWaitVideoSync_thread_cmd)(int divisor, int remainder, unsigned int *count) = NULL;

/****** EVAS GL ******/

void (*glXBindTexImage_orig_evgl_set)(void *func) = NULL;
void *(*glXBindTexImage_orig_evgl_get)(void) = NULL;
void (*glXBindTexImage_evgl_thread_cmd)(Display *dpy, GLXDrawable drawable, int buffer, const int *attrib_list) = NULL;
GLXFBConfig * (*glXChooseFBConfig_evgl_thread_cmd)(Display *dpy, int screen, const int *attribList, int *nitems) = NULL;
GLXContext (*glXCreateContext_evgl_thread_cmd)(Display *dpy, XVisualInfo *vis, GLXContext shareList, Bool direct) = NULL;
GLXContext (*glXCreateNewContext_evgl_thread_cmd)(Display *dpy, GLXFBConfig config, int renderType, GLXContext shareList, Bool direct) = NULL;
GLXPbuffer (*glXCreatePbuffer_evgl_thread_cmd)(Display *dpy, GLXFBConfig config, const int *attribList) = NULL;
void (*glXCreatePixmap_orig_evgl_set)(void *func) = NULL;
void *(*glXCreatePixmap_orig_evgl_get)(void) = NULL;
GLXPixmap (*glXCreatePixmap_evgl_thread_cmd)(Display *dpy, GLXFBConfig config, Pixmap pixmap, const int *attribList) = NULL;
GLXWindow (*glXCreateWindow_evgl_thread_cmd)(Display *dpy, GLXFBConfig config, Window win, const int *attribList) = NULL;
void (*glXDestroyContext_evgl_thread_cmd)(Display *dpy, GLXContext ctx) = NULL;
void (*glXDestroyPbuffer_evgl_thread_cmd)(Display *dpy, GLXPbuffer pbuf) = NULL;
void (*glXDestroyPixmap_orig_evgl_set)(void *func) = NULL;
void *(*glXDestroyPixmap_orig_evgl_get)(void) = NULL;
void (*glXDestroyPixmap_evgl_thread_cmd)(Display *dpy, GLXPixmap pixmap) = NULL;
void (*glXDestroyWindow_evgl_thread_cmd)(Display *dpy, GLXWindow window) = NULL;
int (*glXGetConfig_evgl_thread_cmd)(Display *dpy, XVisualInfo *visual, int attrib, int *value) = NULL;
GLXContext (*glXGetCurrentContext_evgl_thread_cmd)(void) = NULL;
int (*glXGetFBConfigAttrib_evgl_thread_cmd)(Display *dpy, GLXFBConfig config, int attribute, int *value) = NULL;
void (*glXGetVideoSync_orig_evgl_set)(void *func) = NULL;
void *(*glXGetVideoSync_orig_evgl_get)(void) = NULL;
int (*glXGetVideoSync_evgl_thread_cmd)(unsigned int *count) = NULL;
XVisualInfo * (*glXGetVisualFromFBConfig_evgl_thread_cmd)(Display *dpy, GLXFBConfig config) = NULL;
Bool (*glXMakeContextCurrent_evgl_thread_cmd)(Display *dpy, GLXDrawable draw, GLXDrawable read, GLXContext ctx) = NULL;
void (*glXQueryDrawable_orig_evgl_set)(void *func) = NULL;
void *(*glXQueryDrawable_orig_evgl_get)(void) = NULL;
void (*glXQueryDrawable_evgl_thread_cmd)(Display *dpy, GLXDrawable draw, int attribute, unsigned int *value) = NULL;
Bool (*glXQueryExtension_evgl_thread_cmd)(Display *dpy, int *errorb, int *event) = NULL;
const char * (*glXQueryExtensionsString_evgl_thread_cmd)(Display *dpy, int screen) = NULL;
void (*glXReleaseBuffersMESA_orig_evgl_set)(void *func) = NULL;
void *(*glXReleaseBuffersMESA_orig_evgl_get)(void) = NULL;
Bool (*glXReleaseBuffersMESA_evgl_thread_cmd)(Display *dpy, GLXDrawable drawable) = NULL;
void (*glXReleaseTexImage_orig_evgl_set)(void *func) = NULL;
void *(*glXReleaseTexImage_orig_evgl_get)(void) = NULL;
void (*glXReleaseTexImage_evgl_thread_cmd)(Display *dpy, GLXDrawable drawable, int buffer) = NULL;
void (*glXSwapBuffers_evgl_thread_cmd)(Display *dpy, GLXDrawable drawable) = NULL;
void (*glXSwapIntervalEXT_orig_evgl_set)(void *func) = NULL;
void *(*glXSwapIntervalEXT_orig_evgl_get)(void) = NULL;
void (*glXSwapIntervalEXT_evgl_thread_cmd)(Display *dpy, GLXDrawable drawable, int interval) = NULL;
void (*glXSwapIntervalSGI_orig_evgl_set)(void *func) = NULL;
void *(*glXSwapIntervalSGI_orig_evgl_get)(void) = NULL;
int (*glXSwapIntervalSGI_evgl_thread_cmd)(int interval) = NULL;
void (*glXWaitVideoSync_orig_evgl_set)(void *func) = NULL;
void *(*glXWaitVideoSync_orig_evgl_get)(void) = NULL;
int (*glXWaitVideoSync_evgl_thread_cmd)(int divisor, int remainder, unsigned int *count) = NULL;



void _glx_thread_link_init()
{
#define LINK2GENERIC(sym) \
   sym = dlsym(RTLD_DEFAULT, #sym); \
   if (!sym) ERR("Could not find function '%s'", #sym);

   LINK2GENERIC(glXBindTexImage_orig_evas_set);
   LINK2GENERIC(glXBindTexImage_orig_evas_get);
   LINK2GENERIC(glXBindTexImage_thread_cmd);
   LINK2GENERIC(glXChooseFBConfig_thread_cmd);
   LINK2GENERIC(glXCreateContext_thread_cmd);
   LINK2GENERIC(glXCreateNewContext_thread_cmd);
   LINK2GENERIC(glXCreatePbuffer_thread_cmd);
   LINK2GENERIC(glXCreatePixmap_orig_evas_set);
   LINK2GENERIC(glXCreatePixmap_orig_evas_get);
   LINK2GENERIC(glXCreatePixmap_thread_cmd);
   LINK2GENERIC(glXCreateWindow_thread_cmd);
   LINK2GENERIC(glXDestroyContext_thread_cmd);
   LINK2GENERIC(glXDestroyPbuffer_thread_cmd);
   LINK2GENERIC(glXDestroyPixmap_orig_evas_set);
   LINK2GENERIC(glXDestroyPixmap_orig_evas_get);
   LINK2GENERIC(glXDestroyPixmap_thread_cmd);
   LINK2GENERIC(glXDestroyWindow_thread_cmd);
   LINK2GENERIC(glXGetConfig_thread_cmd);
   LINK2GENERIC(glXGetCurrentContext_thread_cmd);
   LINK2GENERIC(glXGetFBConfigAttrib_thread_cmd);
   LINK2GENERIC(glXGetVideoSync_orig_evas_set);
   LINK2GENERIC(glXGetVideoSync_orig_evas_get);
   LINK2GENERIC(glXGetVideoSync_thread_cmd);
   LINK2GENERIC(glXGetVisualFromFBConfig_thread_cmd);
   LINK2GENERIC(glXMakeContextCurrent_thread_cmd);
   LINK2GENERIC(glXQueryDrawable_orig_evas_set);
   LINK2GENERIC(glXQueryDrawable_orig_evas_get);
   LINK2GENERIC(glXQueryDrawable_thread_cmd);
   LINK2GENERIC(glXQueryExtension_thread_cmd);
   LINK2GENERIC(glXQueryExtensionsString_thread_cmd);
   LINK2GENERIC(glXReleaseBuffersMESA_orig_evas_set);
   LINK2GENERIC(glXReleaseBuffersMESA_orig_evas_get);
   LINK2GENERIC(glXReleaseBuffersMESA_thread_cmd);
   LINK2GENERIC(glXReleaseTexImage_orig_evas_set);
   LINK2GENERIC(glXReleaseTexImage_orig_evas_get);
   LINK2GENERIC(glXReleaseTexImage_thread_cmd);
   LINK2GENERIC(glXSwapBuffers_thread_cmd);
   LINK2GENERIC(glXSwapIntervalEXT_orig_evas_set);
   LINK2GENERIC(glXSwapIntervalEXT_orig_evas_get);
   LINK2GENERIC(glXSwapIntervalEXT_thread_cmd);
   LINK2GENERIC(glXSwapIntervalSGI_orig_evas_set);
   LINK2GENERIC(glXSwapIntervalSGI_orig_evas_get);
   LINK2GENERIC(glXSwapIntervalSGI_thread_cmd);
   LINK2GENERIC(glXWaitVideoSync_orig_evas_set);
   LINK2GENERIC(glXWaitVideoSync_orig_evas_get);
   LINK2GENERIC(glXWaitVideoSync_thread_cmd);

   /****** EVAS GL ******/

   LINK2GENERIC(glXBindTexImage_orig_evgl_set);
   LINK2GENERIC(glXBindTexImage_orig_evgl_get);
   LINK2GENERIC(glXBindTexImage_evgl_thread_cmd);
   LINK2GENERIC(glXChooseFBConfig_evgl_thread_cmd);
   LINK2GENERIC(glXCreateContext_evgl_thread_cmd);
   LINK2GENERIC(glXCreateNewContext_evgl_thread_cmd);
   LINK2GENERIC(glXCreatePbuffer_evgl_thread_cmd);
   LINK2GENERIC(glXCreatePixmap_orig_evgl_set);
   LINK2GENERIC(glXCreatePixmap_orig_evgl_get);
   LINK2GENERIC(glXCreatePixmap_evgl_thread_cmd);
   LINK2GENERIC(glXCreateWindow_evgl_thread_cmd);
   LINK2GENERIC(glXDestroyContext_evgl_thread_cmd);
   LINK2GENERIC(glXDestroyPbuffer_evgl_thread_cmd);
   LINK2GENERIC(glXDestroyPixmap_orig_evgl_set);
   LINK2GENERIC(glXDestroyPixmap_orig_evgl_get);
   LINK2GENERIC(glXDestroyPixmap_evgl_thread_cmd);
   LINK2GENERIC(glXDestroyWindow_evgl_thread_cmd);
   LINK2GENERIC(glXGetConfig_evgl_thread_cmd);
   LINK2GENERIC(glXGetCurrentContext_evgl_thread_cmd);
   LINK2GENERIC(glXGetFBConfigAttrib_evgl_thread_cmd);
   LINK2GENERIC(glXGetVideoSync_orig_evgl_set);
   LINK2GENERIC(glXGetVideoSync_orig_evgl_get);
   LINK2GENERIC(glXGetVideoSync_evgl_thread_cmd);
   LINK2GENERIC(glXGetVisualFromFBConfig_evgl_thread_cmd);
   LINK2GENERIC(glXMakeContextCurrent_evgl_thread_cmd);
   LINK2GENERIC(glXQueryDrawable_orig_evgl_set);
   LINK2GENERIC(glXQueryDrawable_orig_evgl_get);
   LINK2GENERIC(glXQueryDrawable_evgl_thread_cmd);
   LINK2GENERIC(glXQueryExtension_evgl_thread_cmd);
   LINK2GENERIC(glXQueryExtensionsString_evgl_thread_cmd);
   LINK2GENERIC(glXReleaseBuffersMESA_orig_evgl_set);
   LINK2GENERIC(glXReleaseBuffersMESA_orig_evgl_get);
   LINK2GENERIC(glXReleaseBuffersMESA_evgl_thread_cmd);
   LINK2GENERIC(glXReleaseTexImage_orig_evgl_set);
   LINK2GENERIC(glXReleaseTexImage_orig_evgl_get);
   LINK2GENERIC(glXReleaseTexImage_evgl_thread_cmd);
   LINK2GENERIC(glXSwapBuffers_evgl_thread_cmd);
   LINK2GENERIC(glXSwapIntervalEXT_orig_evgl_set);
   LINK2GENERIC(glXSwapIntervalEXT_orig_evgl_get);
   LINK2GENERIC(glXSwapIntervalEXT_evgl_thread_cmd);
   LINK2GENERIC(glXSwapIntervalSGI_orig_evgl_set);
   LINK2GENERIC(glXSwapIntervalSGI_orig_evgl_get);
   LINK2GENERIC(glXSwapIntervalSGI_evgl_thread_cmd);
   LINK2GENERIC(glXWaitVideoSync_orig_evgl_set);
   LINK2GENERIC(glXWaitVideoSync_orig_evgl_get);
   LINK2GENERIC(glXWaitVideoSync_evgl_thread_cmd);
}

#endif /* EVAS_GL_RENDER_THREAD_IS_GENERIC */


#endif /* ! GL_GLES */
