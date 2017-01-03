/*
 * This is an automatically generated file using a python script.
 * ($EFL_HOME/src/utils/evas/generate_gl_thread_api.py)
 * Recommend that you modify data files ($EFL_HOME/src/utils/evas/gl_api_def.txt)
 * and make use of scripts if you need to fix them.
 */

/*
 * void
 * glActiveTexture(GLenum texture);
 */

typedef struct
{
   GLenum texture;

} EVGL_API_Thread_Command_glActiveTexture;

void (*orig_evgl_api_glActiveTexture)(GLenum texture);

static void
_evgl_api_thread_glActiveTexture(void *data)
{
   EVGL_API_Thread_Command_glActiveTexture *thread_data =
      (EVGL_API_Thread_Command_glActiveTexture *)data;

   orig_evgl_api_glActiveTexture(thread_data->texture);

}

EAPI void
glActiveTexture_evgl_api_thread_cmd(GLenum texture)
{
   if (!evas_evgl_thread_enabled() || (_main_thread_id != eina_thread_self()))
     {
        orig_evgl_api_glActiveTexture(texture);
        return;
     }

   int thread_mode = EVAS_GL_THREAD_MODE_FINISH;

   EVGL_API_Thread_Command_glActiveTexture thread_data_local;
   EVGL_API_Thread_Command_glActiveTexture *thread_data = &thread_data_local;

   thread_data->texture = texture;

   evas_gl_thread_cmd_enqueue(EVAS_GL_THREAD_TYPE_EVGL,
                              _evgl_api_thread_glActiveTexture,
                              thread_data,
                              thread_mode);
}

/*
 * void
 * glAttachShader(GLuint program, GLuint shader);
 */

typedef struct
{
   GLuint program;
   GLuint shader;
   int command_allocated;

} EVGL_API_Thread_Command_glAttachShader;

void (*orig_evgl_api_glAttachShader)(GLuint program, GLuint shader);

static void
_evgl_api_thread_glAttachShader(void *data)
{
   EVGL_API_Thread_Command_glAttachShader *thread_data =
      (EVGL_API_Thread_Command_glAttachShader *)data;

   orig_evgl_api_glAttachShader(thread_data->program,
                                thread_data->shader);

   if (thread_data->command_allocated)
     eina_mempool_free(_mp_command, thread_data);
}

EAPI void
glAttachShader_evgl_api_thread_cmd(GLuint program, GLuint shader)
{
   if (!evas_evgl_thread_enabled() || (_main_thread_id != eina_thread_self()))
     {
        orig_evgl_api_glAttachShader(program, shader);
        return;
     }

   int thread_mode = EVAS_GL_THREAD_MODE_FINISH;

   EVGL_API_Thread_Command_glAttachShader thread_data_local;
   EVGL_API_Thread_Command_glAttachShader *thread_data = &thread_data_local;

   /* command_allocated flag init. */
   thread_data->command_allocated = 0;

   if (!evas_gl_thread_force_finish())
     { /* _flush */
        EVGL_API_Thread_Command_glAttachShader *thread_data_new;
        thread_data_new = eina_mempool_malloc(_mp_command,
                                              sizeof(EVGL_API_Thread_Command_glAttachShader));
        if (thread_data_new)
          {
             thread_data = thread_data_new;
             thread_data->command_allocated = 1;
             thread_mode = EVAS_GL_THREAD_MODE_FLUSH;
          }
     }

   thread_data->program = program;
   thread_data->shader = shader;

   evas_gl_thread_cmd_enqueue(EVAS_GL_THREAD_TYPE_EVGL,
                              _evgl_api_thread_glAttachShader,
                              thread_data,
                              thread_mode);
}

/*
 * void
 * glBindAttribLocation(GLuint program, GLuint index, const char* name);
 */

typedef struct
{
   GLuint program;
   GLuint index;
   const char* name;

} EVGL_API_Thread_Command_glBindAttribLocation;

void (*orig_evgl_api_glBindAttribLocation)(GLuint program, GLuint index, const char* name);

static void
_evgl_api_thread_glBindAttribLocation(void *data)
{
   EVGL_API_Thread_Command_glBindAttribLocation *thread_data =
      (EVGL_API_Thread_Command_glBindAttribLocation *)data;

   orig_evgl_api_glBindAttribLocation(thread_data->program,
                                      thread_data->index,
                                      thread_data->name);

}

EAPI void
glBindAttribLocation_evgl_api_thread_cmd(GLuint program, GLuint index, const char* name)
{
   if (!evas_evgl_thread_enabled() || (_main_thread_id != eina_thread_self()))
     {
        orig_evgl_api_glBindAttribLocation(program, index, name);
        return;
     }

   int thread_mode = EVAS_GL_THREAD_MODE_FINISH;

   EVGL_API_Thread_Command_glBindAttribLocation thread_data_local;
   EVGL_API_Thread_Command_glBindAttribLocation *thread_data = &thread_data_local;

   thread_data->program = program;
   thread_data->index = index;
   thread_data->name = name;

   evas_gl_thread_cmd_enqueue(EVAS_GL_THREAD_TYPE_EVGL,
                              _evgl_api_thread_glBindAttribLocation,
                              thread_data,
                              thread_mode);
}

/*
 * void
 * glBindBuffer(GLenum target, GLuint buffer);
 */

typedef struct
{
   GLenum target;
   GLuint buffer;
   int command_allocated;

} EVGL_API_Thread_Command_glBindBuffer;

void (*orig_evgl_api_glBindBuffer)(GLenum target, GLuint buffer);

static void
_evgl_api_thread_glBindBuffer(void *data)
{
   EVGL_API_Thread_Command_glBindBuffer *thread_data =
      (EVGL_API_Thread_Command_glBindBuffer *)data;

   orig_evgl_api_glBindBuffer(thread_data->target,
                              thread_data->buffer);

   if (thread_data->command_allocated)
     eina_mempool_free(_mp_command, thread_data);
}

EAPI void
glBindBuffer_evgl_api_thread_cmd(GLenum target, GLuint buffer)
{
   if (!evas_evgl_thread_enabled() || (_main_thread_id != eina_thread_self()))
     {
        orig_evgl_api_glBindBuffer(target, buffer);
        return;
     }

   int thread_mode = EVAS_GL_THREAD_MODE_FINISH;

   EVGL_API_Thread_Command_glBindBuffer thread_data_local;
   EVGL_API_Thread_Command_glBindBuffer *thread_data = &thread_data_local;

   /* command_allocated flag init. */
   thread_data->command_allocated = 0;

   if (!evas_gl_thread_force_finish())
     { /* _flush */
        EVGL_API_Thread_Command_glBindBuffer *thread_data_new;
        thread_data_new = eina_mempool_malloc(_mp_command,
                                              sizeof(EVGL_API_Thread_Command_glBindBuffer));
        if (thread_data_new)
          {
             thread_data = thread_data_new;
             thread_data->command_allocated = 1;
             thread_mode = EVAS_GL_THREAD_MODE_FLUSH;
          }
     }

   thread_data->target = target;
   thread_data->buffer = buffer;

   evas_gl_thread_cmd_enqueue(EVAS_GL_THREAD_TYPE_EVGL,
                              _evgl_api_thread_glBindBuffer,
                              thread_data,
                              thread_mode);
}

/*
 * void
 * glBindFramebuffer(GLenum target, GLuint framebuffer);
 */

typedef struct
{
   GLenum target;
   GLuint framebuffer;
   int command_allocated;

} EVGL_API_Thread_Command_glBindFramebuffer;

void (*orig_evgl_api_glBindFramebuffer)(GLenum target, GLuint framebuffer);

static void
_evgl_api_thread_glBindFramebuffer(void *data)
{
   EVGL_API_Thread_Command_glBindFramebuffer *thread_data =
      (EVGL_API_Thread_Command_glBindFramebuffer *)data;

   orig_evgl_api_glBindFramebuffer(thread_data->target,
                                   thread_data->framebuffer);

   if (thread_data->command_allocated)
     eina_mempool_free(_mp_command, thread_data);
}

EAPI void
glBindFramebuffer_evgl_api_thread_cmd(GLenum target, GLuint framebuffer)
{
   if (!evas_evgl_thread_enabled() || (_main_thread_id != eina_thread_self()))
     {
        orig_evgl_api_glBindFramebuffer(target, framebuffer);
        return;
     }

   int thread_mode = EVAS_GL_THREAD_MODE_FINISH;

   EVGL_API_Thread_Command_glBindFramebuffer thread_data_local;
   EVGL_API_Thread_Command_glBindFramebuffer *thread_data = &thread_data_local;

   /* command_allocated flag init. */
   thread_data->command_allocated = 0;

   if (!evas_gl_thread_force_finish())
     { /* _flush */
        EVGL_API_Thread_Command_glBindFramebuffer *thread_data_new;
        thread_data_new = eina_mempool_malloc(_mp_command,
                                              sizeof(EVGL_API_Thread_Command_glBindFramebuffer));
        if (thread_data_new)
          {
             thread_data = thread_data_new;
             thread_data->command_allocated = 1;
             thread_mode = EVAS_GL_THREAD_MODE_FLUSH;
          }
     }

   thread_data->target = target;
   thread_data->framebuffer = framebuffer;

   evas_gl_thread_cmd_enqueue(EVAS_GL_THREAD_TYPE_EVGL,
                              _evgl_api_thread_glBindFramebuffer,
                              thread_data,
                              thread_mode);
}

/*
 * void
 * glBindRenderbuffer(GLenum target, GLuint renderbuffer);
 */

typedef struct
{
   GLenum target;
   GLuint renderbuffer;
   int command_allocated;

} EVGL_API_Thread_Command_glBindRenderbuffer;

void (*orig_evgl_api_glBindRenderbuffer)(GLenum target, GLuint renderbuffer);

static void
_evgl_api_thread_glBindRenderbuffer(void *data)
{
   EVGL_API_Thread_Command_glBindRenderbuffer *thread_data =
      (EVGL_API_Thread_Command_glBindRenderbuffer *)data;

   orig_evgl_api_glBindRenderbuffer(thread_data->target,
                                    thread_data->renderbuffer);

   if (thread_data->command_allocated)
     eina_mempool_free(_mp_command, thread_data);
}

EAPI void
glBindRenderbuffer_evgl_api_thread_cmd(GLenum target, GLuint renderbuffer)
{
   if (!evas_evgl_thread_enabled() || (_main_thread_id != eina_thread_self()))
     {
        orig_evgl_api_glBindRenderbuffer(target, renderbuffer);
        return;
     }

   int thread_mode = EVAS_GL_THREAD_MODE_FINISH;

   EVGL_API_Thread_Command_glBindRenderbuffer thread_data_local;
   EVGL_API_Thread_Command_glBindRenderbuffer *thread_data = &thread_data_local;

   /* command_allocated flag init. */
   thread_data->command_allocated = 0;

   if (!evas_gl_thread_force_finish())
     { /* _flush */
        EVGL_API_Thread_Command_glBindRenderbuffer *thread_data_new;
        thread_data_new = eina_mempool_malloc(_mp_command,
                                              sizeof(EVGL_API_Thread_Command_glBindRenderbuffer));
        if (thread_data_new)
          {
             thread_data = thread_data_new;
             thread_data->command_allocated = 1;
             thread_mode = EVAS_GL_THREAD_MODE_FLUSH;
          }
     }

   thread_data->target = target;
   thread_data->renderbuffer = renderbuffer;

   evas_gl_thread_cmd_enqueue(EVAS_GL_THREAD_TYPE_EVGL,
                              _evgl_api_thread_glBindRenderbuffer,
                              thread_data,
                              thread_mode);
}

/*
 * void
 * glBindTexture(GLenum target, GLuint texture);
 */

typedef struct
{
   GLenum target;
   GLuint texture;
   int command_allocated;

} EVGL_API_Thread_Command_glBindTexture;

void (*orig_evgl_api_glBindTexture)(GLenum target, GLuint texture);

static void
_evgl_api_thread_glBindTexture(void *data)
{
   EVGL_API_Thread_Command_glBindTexture *thread_data =
      (EVGL_API_Thread_Command_glBindTexture *)data;

   orig_evgl_api_glBindTexture(thread_data->target,
                               thread_data->texture);

   if (thread_data->command_allocated)
     eina_mempool_free(_mp_command, thread_data);
}

EAPI void
glBindTexture_evgl_api_thread_cmd(GLenum target, GLuint texture)
{
   if (!evas_evgl_thread_enabled() || (_main_thread_id != eina_thread_self()))
     {
        orig_evgl_api_glBindTexture(target, texture);
        return;
     }

   int thread_mode = EVAS_GL_THREAD_MODE_FINISH;

   EVGL_API_Thread_Command_glBindTexture thread_data_local;
   EVGL_API_Thread_Command_glBindTexture *thread_data = &thread_data_local;

   /* command_allocated flag init. */
   thread_data->command_allocated = 0;

   if (!evas_gl_thread_force_finish())
     { /* _flush */
        EVGL_API_Thread_Command_glBindTexture *thread_data_new;
        thread_data_new = eina_mempool_malloc(_mp_command,
                                              sizeof(EVGL_API_Thread_Command_glBindTexture));
        if (thread_data_new)
          {
             thread_data = thread_data_new;
             thread_data->command_allocated = 1;
             thread_mode = EVAS_GL_THREAD_MODE_FLUSH;
          }
     }

   thread_data->target = target;
   thread_data->texture = texture;

   evas_gl_thread_cmd_enqueue(EVAS_GL_THREAD_TYPE_EVGL,
                              _evgl_api_thread_glBindTexture,
                              thread_data,
                              thread_mode);
}

/*
 * void
 * glBlendColor(GLclampf red, GLclampf green, GLclampf blue, GLclampf alpha);
 */

typedef struct
{
   GLclampf red;
   GLclampf green;
   GLclampf blue;
   GLclampf alpha;
   int command_allocated;

} EVGL_API_Thread_Command_glBlendColor;

void (*orig_evgl_api_glBlendColor)(GLclampf red, GLclampf green, GLclampf blue, GLclampf alpha);

static void
_evgl_api_thread_glBlendColor(void *data)
{
   EVGL_API_Thread_Command_glBlendColor *thread_data =
      (EVGL_API_Thread_Command_glBlendColor *)data;

   orig_evgl_api_glBlendColor(thread_data->red,
                              thread_data->green,
                              thread_data->blue,
                              thread_data->alpha);

   if (thread_data->command_allocated)
     eina_mempool_free(_mp_command, thread_data);
}

EAPI void
glBlendColor_evgl_api_thread_cmd(GLclampf red, GLclampf green, GLclampf blue, GLclampf alpha)
{
   if (!evas_evgl_thread_enabled() || (_main_thread_id != eina_thread_self()))
     {
        orig_evgl_api_glBlendColor(red, green, blue, alpha);
        return;
     }

   int thread_mode = EVAS_GL_THREAD_MODE_FINISH;

   EVGL_API_Thread_Command_glBlendColor thread_data_local;
   EVGL_API_Thread_Command_glBlendColor *thread_data = &thread_data_local;

   /* command_allocated flag init. */
   thread_data->command_allocated = 0;

   if (!evas_gl_thread_force_finish())
     { /* _flush */
        EVGL_API_Thread_Command_glBlendColor *thread_data_new;
        thread_data_new = eina_mempool_malloc(_mp_command,
                                              sizeof(EVGL_API_Thread_Command_glBlendColor));
        if (thread_data_new)
          {
             thread_data = thread_data_new;
             thread_data->command_allocated = 1;
             thread_mode = EVAS_GL_THREAD_MODE_FLUSH;
          }
     }

   thread_data->red = red;
   thread_data->green = green;
   thread_data->blue = blue;
   thread_data->alpha = alpha;

   evas_gl_thread_cmd_enqueue(EVAS_GL_THREAD_TYPE_EVGL,
                              _evgl_api_thread_glBlendColor,
                              thread_data,
                              thread_mode);
}

/*
 * void
 * glBlendEquation(GLenum mode);
 */

typedef struct
{
   GLenum mode;
   int command_allocated;

} EVGL_API_Thread_Command_glBlendEquation;

void (*orig_evgl_api_glBlendEquation)(GLenum mode);

static void
_evgl_api_thread_glBlendEquation(void *data)
{
   EVGL_API_Thread_Command_glBlendEquation *thread_data =
      (EVGL_API_Thread_Command_glBlendEquation *)data;

   orig_evgl_api_glBlendEquation(thread_data->mode);

   if (thread_data->command_allocated)
     eina_mempool_free(_mp_command, thread_data);
}

EAPI void
glBlendEquation_evgl_api_thread_cmd(GLenum mode)
{
   if (!evas_evgl_thread_enabled() || (_main_thread_id != eina_thread_self()))
     {
        orig_evgl_api_glBlendEquation(mode);
        return;
     }

   int thread_mode = EVAS_GL_THREAD_MODE_FINISH;

   EVGL_API_Thread_Command_glBlendEquation thread_data_local;
   EVGL_API_Thread_Command_glBlendEquation *thread_data = &thread_data_local;

   /* command_allocated flag init. */
   thread_data->command_allocated = 0;

   if (!evas_gl_thread_force_finish())
     { /* _flush */
        EVGL_API_Thread_Command_glBlendEquation *thread_data_new;
        thread_data_new = eina_mempool_malloc(_mp_command,
                                              sizeof(EVGL_API_Thread_Command_glBlendEquation));
        if (thread_data_new)
          {
             thread_data = thread_data_new;
             thread_data->command_allocated = 1;
             thread_mode = EVAS_GL_THREAD_MODE_FLUSH;
          }
     }

   thread_data->mode = mode;

   evas_gl_thread_cmd_enqueue(EVAS_GL_THREAD_TYPE_EVGL,
                              _evgl_api_thread_glBlendEquation,
                              thread_data,
                              thread_mode);
}

/*
 * void
 * glBlendEquationSeparate(GLenum modeRGB, GLenum modeAlpha);
 */

typedef struct
{
   GLenum modeRGB;
   GLenum modeAlpha;
   int command_allocated;

} EVGL_API_Thread_Command_glBlendEquationSeparate;

void (*orig_evgl_api_glBlendEquationSeparate)(GLenum modeRGB, GLenum modeAlpha);

static void
_evgl_api_thread_glBlendEquationSeparate(void *data)
{
   EVGL_API_Thread_Command_glBlendEquationSeparate *thread_data =
      (EVGL_API_Thread_Command_glBlendEquationSeparate *)data;

   orig_evgl_api_glBlendEquationSeparate(thread_data->modeRGB,
                                         thread_data->modeAlpha);

   if (thread_data->command_allocated)
     eina_mempool_free(_mp_command, thread_data);
}

EAPI void
glBlendEquationSeparate_evgl_api_thread_cmd(GLenum modeRGB, GLenum modeAlpha)
{
   if (!evas_evgl_thread_enabled() || (_main_thread_id != eina_thread_self()))
     {
        orig_evgl_api_glBlendEquationSeparate(modeRGB, modeAlpha);
        return;
     }

   int thread_mode = EVAS_GL_THREAD_MODE_FINISH;

   EVGL_API_Thread_Command_glBlendEquationSeparate thread_data_local;
   EVGL_API_Thread_Command_glBlendEquationSeparate *thread_data = &thread_data_local;

   /* command_allocated flag init. */
   thread_data->command_allocated = 0;

   if (!evas_gl_thread_force_finish())
     { /* _flush */
        EVGL_API_Thread_Command_glBlendEquationSeparate *thread_data_new;
        thread_data_new = eina_mempool_malloc(_mp_command,
                                              sizeof(EVGL_API_Thread_Command_glBlendEquationSeparate));
        if (thread_data_new)
          {
             thread_data = thread_data_new;
             thread_data->command_allocated = 1;
             thread_mode = EVAS_GL_THREAD_MODE_FLUSH;
          }
     }

   thread_data->modeRGB = modeRGB;
   thread_data->modeAlpha = modeAlpha;

   evas_gl_thread_cmd_enqueue(EVAS_GL_THREAD_TYPE_EVGL,
                              _evgl_api_thread_glBlendEquationSeparate,
                              thread_data,
                              thread_mode);
}

/*
 * void
 * glBlendFunc(GLenum sfactor, GLenum dfactor);
 */

typedef struct
{
   GLenum sfactor;
   GLenum dfactor;
   int command_allocated;

} EVGL_API_Thread_Command_glBlendFunc;

void (*orig_evgl_api_glBlendFunc)(GLenum sfactor, GLenum dfactor);

static void
_evgl_api_thread_glBlendFunc(void *data)
{
   EVGL_API_Thread_Command_glBlendFunc *thread_data =
      (EVGL_API_Thread_Command_glBlendFunc *)data;

   orig_evgl_api_glBlendFunc(thread_data->sfactor,
                             thread_data->dfactor);

   if (thread_data->command_allocated)
     eina_mempool_free(_mp_command, thread_data);
}

EAPI void
glBlendFunc_evgl_api_thread_cmd(GLenum sfactor, GLenum dfactor)
{
   if (!evas_evgl_thread_enabled() || (_main_thread_id != eina_thread_self()))
     {
        orig_evgl_api_glBlendFunc(sfactor, dfactor);
        return;
     }

   int thread_mode = EVAS_GL_THREAD_MODE_FINISH;

   EVGL_API_Thread_Command_glBlendFunc thread_data_local;
   EVGL_API_Thread_Command_glBlendFunc *thread_data = &thread_data_local;

   /* command_allocated flag init. */
   thread_data->command_allocated = 0;

   if (!evas_gl_thread_force_finish())
     { /* _flush */
        EVGL_API_Thread_Command_glBlendFunc *thread_data_new;
        thread_data_new = eina_mempool_malloc(_mp_command,
                                              sizeof(EVGL_API_Thread_Command_glBlendFunc));
        if (thread_data_new)
          {
             thread_data = thread_data_new;
             thread_data->command_allocated = 1;
             thread_mode = EVAS_GL_THREAD_MODE_FLUSH;
          }
     }

   thread_data->sfactor = sfactor;
   thread_data->dfactor = dfactor;

   evas_gl_thread_cmd_enqueue(EVAS_GL_THREAD_TYPE_EVGL,
                              _evgl_api_thread_glBlendFunc,
                              thread_data,
                              thread_mode);
}

/*
 * void
 * glBlendFuncSeparate(GLenum srcRGB, GLenum dstRGB, GLenum srcAlpha, GLenum dstAlpha);
 */

typedef struct
{
   GLenum srcRGB;
   GLenum dstRGB;
   GLenum srcAlpha;
   GLenum dstAlpha;
   int command_allocated;

} EVGL_API_Thread_Command_glBlendFuncSeparate;

void (*orig_evgl_api_glBlendFuncSeparate)(GLenum srcRGB, GLenum dstRGB, GLenum srcAlpha, GLenum dstAlpha);

static void
_evgl_api_thread_glBlendFuncSeparate(void *data)
{
   EVGL_API_Thread_Command_glBlendFuncSeparate *thread_data =
      (EVGL_API_Thread_Command_glBlendFuncSeparate *)data;

   orig_evgl_api_glBlendFuncSeparate(thread_data->srcRGB,
                                     thread_data->dstRGB,
                                     thread_data->srcAlpha,
                                     thread_data->dstAlpha);

   if (thread_data->command_allocated)
     eina_mempool_free(_mp_command, thread_data);
}

EAPI void
glBlendFuncSeparate_evgl_api_thread_cmd(GLenum srcRGB, GLenum dstRGB, GLenum srcAlpha, GLenum dstAlpha)
{
   if (!evas_evgl_thread_enabled() || (_main_thread_id != eina_thread_self()))
     {
        orig_evgl_api_glBlendFuncSeparate(srcRGB, dstRGB, srcAlpha, dstAlpha);
        return;
     }

   int thread_mode = EVAS_GL_THREAD_MODE_FINISH;

   EVGL_API_Thread_Command_glBlendFuncSeparate thread_data_local;
   EVGL_API_Thread_Command_glBlendFuncSeparate *thread_data = &thread_data_local;

   /* command_allocated flag init. */
   thread_data->command_allocated = 0;

   if (!evas_gl_thread_force_finish())
     { /* _flush */
        EVGL_API_Thread_Command_glBlendFuncSeparate *thread_data_new;
        thread_data_new = eina_mempool_malloc(_mp_command,
                                              sizeof(EVGL_API_Thread_Command_glBlendFuncSeparate));
        if (thread_data_new)
          {
             thread_data = thread_data_new;
             thread_data->command_allocated = 1;
             thread_mode = EVAS_GL_THREAD_MODE_FLUSH;
          }
     }

   thread_data->srcRGB = srcRGB;
   thread_data->dstRGB = dstRGB;
   thread_data->srcAlpha = srcAlpha;
   thread_data->dstAlpha = dstAlpha;

   evas_gl_thread_cmd_enqueue(EVAS_GL_THREAD_TYPE_EVGL,
                              _evgl_api_thread_glBlendFuncSeparate,
                              thread_data,
                              thread_mode);
}

/*
 * void
 * glBufferData(GLenum target, GLsizeiptr size, const void* data, GLenum usage);
 */

typedef struct
{
   GLenum target;
   GLsizeiptr size;
   const void* data;
   GLenum usage;

} EVGL_API_Thread_Command_glBufferData;

void (*orig_evgl_api_glBufferData)(GLenum target, GLsizeiptr size, const void* data, GLenum usage);

static void
_evgl_api_thread_glBufferData(void *data)
{
   EVGL_API_Thread_Command_glBufferData *thread_data =
      (EVGL_API_Thread_Command_glBufferData *)data;

   orig_evgl_api_glBufferData(thread_data->target,
                              thread_data->size,
                              thread_data->data,
                              thread_data->usage);

}

EAPI void
glBufferData_evgl_api_thread_cmd(GLenum target, GLsizeiptr size, const void* data, GLenum usage)
{
   if (!evas_evgl_thread_enabled() || (_main_thread_id != eina_thread_self()))
     {
        orig_evgl_api_glBufferData(target, size, data, usage);
        return;
     }

   int thread_mode = EVAS_GL_THREAD_MODE_FINISH;

   EVGL_API_Thread_Command_glBufferData thread_data_local;
   EVGL_API_Thread_Command_glBufferData *thread_data = &thread_data_local;

   thread_data->target = target;
   thread_data->size = size;
   thread_data->data = data;
   thread_data->usage = usage;

   evas_gl_thread_cmd_enqueue(EVAS_GL_THREAD_TYPE_EVGL,
                              _evgl_api_thread_glBufferData,
                              thread_data,
                              thread_mode);
}

/*
 * void
 * glBufferSubData(GLenum target, GLintptr offset, GLsizeiptr size, const void* data);
 */

typedef struct
{
   GLenum target;
   GLintptr offset;
   GLsizeiptr size;
   const void* data;

} EVGL_API_Thread_Command_glBufferSubData;

void (*orig_evgl_api_glBufferSubData)(GLenum target, GLintptr offset, GLsizeiptr size, const void* data);

static void
_evgl_api_thread_glBufferSubData(void *data)
{
   EVGL_API_Thread_Command_glBufferSubData *thread_data =
      (EVGL_API_Thread_Command_glBufferSubData *)data;

   orig_evgl_api_glBufferSubData(thread_data->target,
                                 thread_data->offset,
                                 thread_data->size,
                                 thread_data->data);

}

EAPI void
glBufferSubData_evgl_api_thread_cmd(GLenum target, GLintptr offset, GLsizeiptr size, const void* data)
{
   if (!evas_evgl_thread_enabled() || (_main_thread_id != eina_thread_self()))
     {
        orig_evgl_api_glBufferSubData(target, offset, size, data);
        return;
     }

   int thread_mode = EVAS_GL_THREAD_MODE_FINISH;

   EVGL_API_Thread_Command_glBufferSubData thread_data_local;
   EVGL_API_Thread_Command_glBufferSubData *thread_data = &thread_data_local;

   thread_data->target = target;
   thread_data->offset = offset;
   thread_data->size = size;
   thread_data->data = data;

   evas_gl_thread_cmd_enqueue(EVAS_GL_THREAD_TYPE_EVGL,
                              _evgl_api_thread_glBufferSubData,
                              thread_data,
                              thread_mode);
}

/*
 * GLenum
 * glCheckFramebufferStatus(GLenum target);
 */

typedef struct
{
   GLenum return_value;
   GLenum target;

} EVGL_API_Thread_Command_glCheckFramebufferStatus;

GLenum (*orig_evgl_api_glCheckFramebufferStatus)(GLenum target);

static void
_evgl_api_thread_glCheckFramebufferStatus(void *data)
{
   EVGL_API_Thread_Command_glCheckFramebufferStatus *thread_data =
      (EVGL_API_Thread_Command_glCheckFramebufferStatus *)data;

   thread_data->return_value = orig_evgl_api_glCheckFramebufferStatus(thread_data->target);

}

EAPI GLenum
glCheckFramebufferStatus_evgl_api_thread_cmd(GLenum target)
{
   if (!evas_evgl_thread_enabled() || (_main_thread_id != eina_thread_self()))
     {
        return orig_evgl_api_glCheckFramebufferStatus(target);
     }

   int thread_mode = EVAS_GL_THREAD_MODE_FINISH;

   EVGL_API_Thread_Command_glCheckFramebufferStatus thread_data_local;
   EVGL_API_Thread_Command_glCheckFramebufferStatus *thread_data = &thread_data_local;

   thread_data->target = target;

   evas_gl_thread_cmd_enqueue(EVAS_GL_THREAD_TYPE_EVGL,
                              _evgl_api_thread_glCheckFramebufferStatus,
                              thread_data,
                              thread_mode);

   return thread_data->return_value;
}

/*
 * void
 * glClear(GLbitfield mask);
 */

typedef struct
{
   GLbitfield mask;

} EVGL_API_Thread_Command_glClear;

void (*orig_evgl_api_glClear)(GLbitfield mask);

static void
_evgl_api_thread_glClear(void *data)
{
   EVGL_API_Thread_Command_glClear *thread_data =
      (EVGL_API_Thread_Command_glClear *)data;

   orig_evgl_api_glClear(thread_data->mask);

}

EAPI void
glClear_evgl_api_thread_cmd(GLbitfield mask)
{
   if (!evas_evgl_thread_enabled() || (_main_thread_id != eina_thread_self()))
     {
        orig_evgl_api_glClear(mask);
        return;
     }

   int thread_mode = EVAS_GL_THREAD_MODE_FINISH;

   EVGL_API_Thread_Command_glClear thread_data_local;
   EVGL_API_Thread_Command_glClear *thread_data = &thread_data_local;

   thread_data->mask = mask;

   evas_gl_thread_cmd_enqueue(EVAS_GL_THREAD_TYPE_EVGL,
                              _evgl_api_thread_glClear,
                              thread_data,
                              thread_mode);
}

/*
 * void
 * glClearColor(GLclampf red, GLclampf green, GLclampf blue, GLclampf alpha);
 */

typedef struct
{
   GLclampf red;
   GLclampf green;
   GLclampf blue;
   GLclampf alpha;

} EVGL_API_Thread_Command_glClearColor;

void (*orig_evgl_api_glClearColor)(GLclampf red, GLclampf green, GLclampf blue, GLclampf alpha);

static void
_evgl_api_thread_glClearColor(void *data)
{
   EVGL_API_Thread_Command_glClearColor *thread_data =
      (EVGL_API_Thread_Command_glClearColor *)data;

   orig_evgl_api_glClearColor(thread_data->red,
                              thread_data->green,
                              thread_data->blue,
                              thread_data->alpha);

}

EAPI void
glClearColor_evgl_api_thread_cmd(GLclampf red, GLclampf green, GLclampf blue, GLclampf alpha)
{
   if (!evas_evgl_thread_enabled() || (_main_thread_id != eina_thread_self()))
     {
        orig_evgl_api_glClearColor(red, green, blue, alpha);
        return;
     }

   int thread_mode = EVAS_GL_THREAD_MODE_FINISH;

   EVGL_API_Thread_Command_glClearColor thread_data_local;
   EVGL_API_Thread_Command_glClearColor *thread_data = &thread_data_local;

   thread_data->red = red;
   thread_data->green = green;
   thread_data->blue = blue;
   thread_data->alpha = alpha;

   evas_gl_thread_cmd_enqueue(EVAS_GL_THREAD_TYPE_EVGL,
                              _evgl_api_thread_glClearColor,
                              thread_data,
                              thread_mode);
}

/*
 * void
 * glClearDepthf(GLclampf depth);
 */

typedef struct
{
   GLclampf depth;

} EVGL_API_Thread_Command_glClearDepthf;

void (*orig_evgl_api_glClearDepthf)(GLclampf depth);

static void
_evgl_api_thread_glClearDepthf(void *data)
{
   EVGL_API_Thread_Command_glClearDepthf *thread_data =
      (EVGL_API_Thread_Command_glClearDepthf *)data;

   orig_evgl_api_glClearDepthf(thread_data->depth);

}

EAPI void
glClearDepthf_evgl_api_thread_cmd(GLclampf depth)
{
   if (!evas_evgl_thread_enabled() || (_main_thread_id != eina_thread_self()))
     {
        orig_evgl_api_glClearDepthf(depth);
        return;
     }

   int thread_mode = EVAS_GL_THREAD_MODE_FINISH;

   EVGL_API_Thread_Command_glClearDepthf thread_data_local;
   EVGL_API_Thread_Command_glClearDepthf *thread_data = &thread_data_local;

   thread_data->depth = depth;

   evas_gl_thread_cmd_enqueue(EVAS_GL_THREAD_TYPE_EVGL,
                              _evgl_api_thread_glClearDepthf,
                              thread_data,
                              thread_mode);
}

/*
 * void
 * glClearStencil(GLint s);
 */

typedef struct
{
   GLint s;

} EVGL_API_Thread_Command_glClearStencil;

void (*orig_evgl_api_glClearStencil)(GLint s);

static void
_evgl_api_thread_glClearStencil(void *data)
{
   EVGL_API_Thread_Command_glClearStencil *thread_data =
      (EVGL_API_Thread_Command_glClearStencil *)data;

   orig_evgl_api_glClearStencil(thread_data->s);

}

EAPI void
glClearStencil_evgl_api_thread_cmd(GLint s)
{
   if (!evas_evgl_thread_enabled() || (_main_thread_id != eina_thread_self()))
     {
        orig_evgl_api_glClearStencil(s);
        return;
     }

   int thread_mode = EVAS_GL_THREAD_MODE_FINISH;

   EVGL_API_Thread_Command_glClearStencil thread_data_local;
   EVGL_API_Thread_Command_glClearStencil *thread_data = &thread_data_local;

   thread_data->s = s;

   evas_gl_thread_cmd_enqueue(EVAS_GL_THREAD_TYPE_EVGL,
                              _evgl_api_thread_glClearStencil,
                              thread_data,
                              thread_mode);
}

/*
 * void
 * glColorMask(GLboolean red, GLboolean green, GLboolean blue, GLboolean alpha);
 */

typedef struct
{
   GLboolean red;
   GLboolean green;
   GLboolean blue;
   GLboolean alpha;

} EVGL_API_Thread_Command_glColorMask;

void (*orig_evgl_api_glColorMask)(GLboolean red, GLboolean green, GLboolean blue, GLboolean alpha);

static void
_evgl_api_thread_glColorMask(void *data)
{
   EVGL_API_Thread_Command_glColorMask *thread_data =
      (EVGL_API_Thread_Command_glColorMask *)data;

   orig_evgl_api_glColorMask(thread_data->red,
                             thread_data->green,
                             thread_data->blue,
                             thread_data->alpha);

}

EAPI void
glColorMask_evgl_api_thread_cmd(GLboolean red, GLboolean green, GLboolean blue, GLboolean alpha)
{
   if (!evas_evgl_thread_enabled() || (_main_thread_id != eina_thread_self()))
     {
        orig_evgl_api_glColorMask(red, green, blue, alpha);
        return;
     }

   int thread_mode = EVAS_GL_THREAD_MODE_FINISH;

   EVGL_API_Thread_Command_glColorMask thread_data_local;
   EVGL_API_Thread_Command_glColorMask *thread_data = &thread_data_local;

   thread_data->red = red;
   thread_data->green = green;
   thread_data->blue = blue;
   thread_data->alpha = alpha;

   evas_gl_thread_cmd_enqueue(EVAS_GL_THREAD_TYPE_EVGL,
                              _evgl_api_thread_glColorMask,
                              thread_data,
                              thread_mode);
}

/*
 * void
 * glCompileShader(GLuint shader);
 */

typedef struct
{
   GLuint shader;
   int command_allocated;

} EVGL_API_Thread_Command_glCompileShader;

void (*orig_evgl_api_glCompileShader)(GLuint shader);

static void
_evgl_api_thread_glCompileShader(void *data)
{
   EVGL_API_Thread_Command_glCompileShader *thread_data =
      (EVGL_API_Thread_Command_glCompileShader *)data;

   orig_evgl_api_glCompileShader(thread_data->shader);

   if (thread_data->command_allocated)
     eina_mempool_free(_mp_command, thread_data);
}

EAPI void
glCompileShader_evgl_api_thread_cmd(GLuint shader)
{
   if (!evas_evgl_thread_enabled() || (_main_thread_id != eina_thread_self()))
     {
        orig_evgl_api_glCompileShader(shader);
        return;
     }

   int thread_mode = EVAS_GL_THREAD_MODE_FINISH;

   EVGL_API_Thread_Command_glCompileShader thread_data_local;
   EVGL_API_Thread_Command_glCompileShader *thread_data = &thread_data_local;

   /* command_allocated flag init. */
   thread_data->command_allocated = 0;

   if (!evas_gl_thread_force_finish())
     { /* _flush */
        EVGL_API_Thread_Command_glCompileShader *thread_data_new;
        thread_data_new = eina_mempool_malloc(_mp_command,
                                              sizeof(EVGL_API_Thread_Command_glCompileShader));
        if (thread_data_new)
          {
             thread_data = thread_data_new;
             thread_data->command_allocated = 1;
             thread_mode = EVAS_GL_THREAD_MODE_FLUSH;
          }
     }

   thread_data->shader = shader;

   evas_gl_thread_cmd_enqueue(EVAS_GL_THREAD_TYPE_EVGL,
                              _evgl_api_thread_glCompileShader,
                              thread_data,
                              thread_mode);
}

/*
 * void
 * glCompressedTexImage2D(GLenum target, GLint level, GLenum internalformat, GLsizei width, GLsizei height, GLint border, GLsizei imageSize, const void* data);
 */

typedef struct
{
   GLenum target;
   GLint level;
   GLenum internalformat;
   GLsizei width;
   GLsizei height;
   GLint border;
   GLsizei imageSize;
   const void* data;
   int command_allocated;
   GLCOMPRESSEDTEXIMAGE2D_COPY_VARIABLE; /* TODO */

} EVGL_API_Thread_Command_glCompressedTexImage2D;

void (*orig_evgl_api_glCompressedTexImage2D)(GLenum target, GLint level, GLenum internalformat, GLsizei width, GLsizei height, GLint border, GLsizei imageSize, const void* data);

static void
_evgl_api_thread_glCompressedTexImage2D(void *data)
{
   EVGL_API_Thread_Command_glCompressedTexImage2D *thread_data =
      (EVGL_API_Thread_Command_glCompressedTexImage2D *)data;

   orig_evgl_api_glCompressedTexImage2D(thread_data->target,
                                        thread_data->level,
                                        thread_data->internalformat,
                                        thread_data->width,
                                        thread_data->height,
                                        thread_data->border,
                                        thread_data->imageSize,
                                        thread_data->data);

   GLCOMPRESSEDTEXIMAGE2D_COPY_VARIABLE_FREE; /* TODO */

   if (thread_data->command_allocated)
     eina_mempool_free(_mp_command, thread_data);
}

EAPI void
glCompressedTexImage2D_evgl_api_thread_cmd(GLenum target, GLint level, GLenum internalformat, GLsizei width, GLsizei height, GLint border, GLsizei imageSize, const void* data)
{
   if (!evas_evgl_thread_enabled() || (_main_thread_id != eina_thread_self()))
     {
        orig_evgl_api_glCompressedTexImage2D(target, level, internalformat, width, height, border, imageSize, data);
        return;
     }

   int thread_mode = EVAS_GL_THREAD_MODE_FINISH;

   EVGL_API_Thread_Command_glCompressedTexImage2D thread_data_local;
   EVGL_API_Thread_Command_glCompressedTexImage2D *thread_data = &thread_data_local;

   /* command_allocated flag init. */
   thread_data->command_allocated = 0;

   if (!evas_gl_thread_force_finish())
     { /* _flush */
        EVGL_API_Thread_Command_glCompressedTexImage2D *thread_data_new;
        thread_data_new = eina_mempool_malloc(_mp_command,
                                              sizeof(EVGL_API_Thread_Command_glCompressedTexImage2D));
        if (thread_data_new)
          {
             thread_data = thread_data_new;
             thread_data->command_allocated = 1;
             thread_mode = EVAS_GL_THREAD_MODE_FLUSH;
          }
     }

   thread_data->target = target;
   thread_data->level = level;
   thread_data->internalformat = internalformat;
   thread_data->width = width;
   thread_data->height = height;
   thread_data->border = border;
   thread_data->imageSize = imageSize;
   thread_data->data = data;

   GLCOMPRESSEDTEXIMAGE2D_COPY_VARIABLE_INIT; /* TODO */

   if (thread_mode == EVAS_GL_THREAD_MODE_FINISH)
     goto finish;

   GLCOMPRESSEDTEXIMAGE2D_COPY_TO_MEMPOOL; /* TODO */

finish:
   evas_gl_thread_cmd_enqueue(EVAS_GL_THREAD_TYPE_EVGL,
                              _evgl_api_thread_glCompressedTexImage2D,
                              thread_data,
                              thread_mode);
}

/*
 * void
 * glCompressedTexSubImage2D(GLenum target, GLint level, GLint xoffset, GLint yoffset, GLsizei width, GLsizei height, GLenum format, GLsizei imageSize, const void* data);
 */

typedef struct
{
   GLenum target;
   GLint level;
   GLint xoffset;
   GLint yoffset;
   GLsizei width;
   GLsizei height;
   GLenum format;
   GLsizei imageSize;
   const void* data;
   int command_allocated;
   GLCOMPRESSEDTEXSUBIMAGE2D_COPY_VARIABLE; /* TODO */

} EVGL_API_Thread_Command_glCompressedTexSubImage2D;

void (*orig_evgl_api_glCompressedTexSubImage2D)(GLenum target, GLint level, GLint xoffset, GLint yoffset, GLsizei width, GLsizei height, GLenum format, GLsizei imageSize, const void* data);

static void
_evgl_api_thread_glCompressedTexSubImage2D(void *data)
{
   EVGL_API_Thread_Command_glCompressedTexSubImage2D *thread_data =
      (EVGL_API_Thread_Command_glCompressedTexSubImage2D *)data;

   orig_evgl_api_glCompressedTexSubImage2D(thread_data->target,
                                           thread_data->level,
                                           thread_data->xoffset,
                                           thread_data->yoffset,
                                           thread_data->width,
                                           thread_data->height,
                                           thread_data->format,
                                           thread_data->imageSize,
                                           thread_data->data);

   GLCOMPRESSEDTEXSUBIMAGE2D_COPY_VARIABLE_FREE; /* TODO */

   if (thread_data->command_allocated)
     eina_mempool_free(_mp_command, thread_data);
}

EAPI void
glCompressedTexSubImage2D_evgl_api_thread_cmd(GLenum target, GLint level, GLint xoffset, GLint yoffset, GLsizei width, GLsizei height, GLenum format, GLsizei imageSize, const void* data)
{
   if (!evas_evgl_thread_enabled() || (_main_thread_id != eina_thread_self()))
     {
        orig_evgl_api_glCompressedTexSubImage2D(target, level, xoffset, yoffset, width, height, format, imageSize, data);
        return;
     }

   int thread_mode = EVAS_GL_THREAD_MODE_FINISH;

   EVGL_API_Thread_Command_glCompressedTexSubImage2D thread_data_local;
   EVGL_API_Thread_Command_glCompressedTexSubImage2D *thread_data = &thread_data_local;

   /* command_allocated flag init. */
   thread_data->command_allocated = 0;

   if (!evas_gl_thread_force_finish())
     { /* _flush */
        EVGL_API_Thread_Command_glCompressedTexSubImage2D *thread_data_new;
        thread_data_new = eina_mempool_malloc(_mp_command,
                                              sizeof(EVGL_API_Thread_Command_glCompressedTexSubImage2D));
        if (thread_data_new)
          {
             thread_data = thread_data_new;
             thread_data->command_allocated = 1;
             thread_mode = EVAS_GL_THREAD_MODE_FLUSH;
          }
     }

   thread_data->target = target;
   thread_data->level = level;
   thread_data->xoffset = xoffset;
   thread_data->yoffset = yoffset;
   thread_data->width = width;
   thread_data->height = height;
   thread_data->format = format;
   thread_data->imageSize = imageSize;
   thread_data->data = data;

   GLCOMPRESSEDTEXSUBIMAGE2D_COPY_VARIABLE_INIT; /* TODO */

   if (thread_mode == EVAS_GL_THREAD_MODE_FINISH)
     goto finish;

   GLCOMPRESSEDTEXSUBIMAGE2D_COPY_TO_MEMPOOL; /* TODO */

finish:
   evas_gl_thread_cmd_enqueue(EVAS_GL_THREAD_TYPE_EVGL,
                              _evgl_api_thread_glCompressedTexSubImage2D,
                              thread_data,
                              thread_mode);
}

/*
 * void
 * glCopyTexImage2D(GLenum target, GLint level, GLenum internalformat, GLint x, GLint y, GLsizei width, GLsizei height, GLint border);
 */

typedef struct
{
   GLenum target;
   GLint level;
   GLenum internalformat;
   GLint x;
   GLint y;
   GLsizei width;
   GLsizei height;
   GLint border;

} EVGL_API_Thread_Command_glCopyTexImage2D;

void (*orig_evgl_api_glCopyTexImage2D)(GLenum target, GLint level, GLenum internalformat, GLint x, GLint y, GLsizei width, GLsizei height, GLint border);

static void
_evgl_api_thread_glCopyTexImage2D(void *data)
{
   EVGL_API_Thread_Command_glCopyTexImage2D *thread_data =
      (EVGL_API_Thread_Command_glCopyTexImage2D *)data;

   orig_evgl_api_glCopyTexImage2D(thread_data->target,
                                  thread_data->level,
                                  thread_data->internalformat,
                                  thread_data->x,
                                  thread_data->y,
                                  thread_data->width,
                                  thread_data->height,
                                  thread_data->border);

}

EAPI void
glCopyTexImage2D_evgl_api_thread_cmd(GLenum target, GLint level, GLenum internalformat, GLint x, GLint y, GLsizei width, GLsizei height, GLint border)
{
   if (!evas_evgl_thread_enabled() || (_main_thread_id != eina_thread_self()))
     {
        orig_evgl_api_glCopyTexImage2D(target, level, internalformat, x, y, width, height, border);
        return;
     }

   int thread_mode = EVAS_GL_THREAD_MODE_FINISH;

   EVGL_API_Thread_Command_glCopyTexImage2D thread_data_local;
   EVGL_API_Thread_Command_glCopyTexImage2D *thread_data = &thread_data_local;

   thread_data->target = target;
   thread_data->level = level;
   thread_data->internalformat = internalformat;
   thread_data->x = x;
   thread_data->y = y;
   thread_data->width = width;
   thread_data->height = height;
   thread_data->border = border;

   evas_gl_thread_cmd_enqueue(EVAS_GL_THREAD_TYPE_EVGL,
                              _evgl_api_thread_glCopyTexImage2D,
                              thread_data,
                              thread_mode);
}

/*
 * void
 * glCopyTexSubImage2D(GLenum target, GLint level, GLint xoffset, GLint yoffset, GLint x, GLint y, GLsizei width, GLsizei height);
 */

typedef struct
{
   GLenum target;
   GLint level;
   GLint xoffset;
   GLint yoffset;
   GLint x;
   GLint y;
   GLsizei width;
   GLsizei height;

} EVGL_API_Thread_Command_glCopyTexSubImage2D;

void (*orig_evgl_api_glCopyTexSubImage2D)(GLenum target, GLint level, GLint xoffset, GLint yoffset, GLint x, GLint y, GLsizei width, GLsizei height);

static void
_evgl_api_thread_glCopyTexSubImage2D(void *data)
{
   EVGL_API_Thread_Command_glCopyTexSubImage2D *thread_data =
      (EVGL_API_Thread_Command_glCopyTexSubImage2D *)data;

   orig_evgl_api_glCopyTexSubImage2D(thread_data->target,
                                     thread_data->level,
                                     thread_data->xoffset,
                                     thread_data->yoffset,
                                     thread_data->x,
                                     thread_data->y,
                                     thread_data->width,
                                     thread_data->height);

}

EAPI void
glCopyTexSubImage2D_evgl_api_thread_cmd(GLenum target, GLint level, GLint xoffset, GLint yoffset, GLint x, GLint y, GLsizei width, GLsizei height)
{
   if (!evas_evgl_thread_enabled() || (_main_thread_id != eina_thread_self()))
     {
        orig_evgl_api_glCopyTexSubImage2D(target, level, xoffset, yoffset, x, y, width, height);
        return;
     }

   int thread_mode = EVAS_GL_THREAD_MODE_FINISH;

   EVGL_API_Thread_Command_glCopyTexSubImage2D thread_data_local;
   EVGL_API_Thread_Command_glCopyTexSubImage2D *thread_data = &thread_data_local;

   thread_data->target = target;
   thread_data->level = level;
   thread_data->xoffset = xoffset;
   thread_data->yoffset = yoffset;
   thread_data->x = x;
   thread_data->y = y;
   thread_data->width = width;
   thread_data->height = height;

   evas_gl_thread_cmd_enqueue(EVAS_GL_THREAD_TYPE_EVGL,
                              _evgl_api_thread_glCopyTexSubImage2D,
                              thread_data,
                              thread_mode);
}

/*
 * GLuint
 * glCreateProgram(void);
 */

typedef struct
{
   GLuint return_value;

} EVGL_API_Thread_Command_glCreateProgram;

GLuint (*orig_evgl_api_glCreateProgram)(void);

static void
_evgl_api_thread_glCreateProgram(void *data)
{
   EVGL_API_Thread_Command_glCreateProgram *thread_data =
      (EVGL_API_Thread_Command_glCreateProgram *)data;

   thread_data->return_value = orig_evgl_api_glCreateProgram();

}

EAPI GLuint
glCreateProgram_evgl_api_thread_cmd(void)
{
   if (!evas_evgl_thread_enabled() || (_main_thread_id != eina_thread_self()))
     {
        return orig_evgl_api_glCreateProgram();
     }

   int thread_mode = EVAS_GL_THREAD_MODE_FINISH;

   EVGL_API_Thread_Command_glCreateProgram thread_data_local;
   EVGL_API_Thread_Command_glCreateProgram *thread_data = &thread_data_local;


   evas_gl_thread_cmd_enqueue(EVAS_GL_THREAD_TYPE_EVGL,
                              _evgl_api_thread_glCreateProgram,
                              thread_data,
                              thread_mode);

   return thread_data->return_value;
}

/*
 * GLuint
 * glCreateShader(GLenum type);
 */

typedef struct
{
   GLuint return_value;
   GLenum type;

} EVGL_API_Thread_Command_glCreateShader;

GLuint (*orig_evgl_api_glCreateShader)(GLenum type);

static void
_evgl_api_thread_glCreateShader(void *data)
{
   EVGL_API_Thread_Command_glCreateShader *thread_data =
      (EVGL_API_Thread_Command_glCreateShader *)data;

   thread_data->return_value = orig_evgl_api_glCreateShader(thread_data->type);

}

EAPI GLuint
glCreateShader_evgl_api_thread_cmd(GLenum type)
{
   if (!evas_evgl_thread_enabled() || (_main_thread_id != eina_thread_self()))
     {
        return orig_evgl_api_glCreateShader(type);
     }

   int thread_mode = EVAS_GL_THREAD_MODE_FINISH;

   EVGL_API_Thread_Command_glCreateShader thread_data_local;
   EVGL_API_Thread_Command_glCreateShader *thread_data = &thread_data_local;

   thread_data->type = type;

   evas_gl_thread_cmd_enqueue(EVAS_GL_THREAD_TYPE_EVGL,
                              _evgl_api_thread_glCreateShader,
                              thread_data,
                              thread_mode);

   return thread_data->return_value;
}

/*
 * void
 * glCullFace(GLenum mode);
 */

typedef struct
{
   GLenum mode;

} EVGL_API_Thread_Command_glCullFace;

void (*orig_evgl_api_glCullFace)(GLenum mode);

static void
_evgl_api_thread_glCullFace(void *data)
{
   EVGL_API_Thread_Command_glCullFace *thread_data =
      (EVGL_API_Thread_Command_glCullFace *)data;

   orig_evgl_api_glCullFace(thread_data->mode);

}

EAPI void
glCullFace_evgl_api_thread_cmd(GLenum mode)
{
   if (!evas_evgl_thread_enabled() || (_main_thread_id != eina_thread_self()))
     {
        orig_evgl_api_glCullFace(mode);
        return;
     }

   int thread_mode = EVAS_GL_THREAD_MODE_FINISH;

   EVGL_API_Thread_Command_glCullFace thread_data_local;
   EVGL_API_Thread_Command_glCullFace *thread_data = &thread_data_local;

   thread_data->mode = mode;

   evas_gl_thread_cmd_enqueue(EVAS_GL_THREAD_TYPE_EVGL,
                              _evgl_api_thread_glCullFace,
                              thread_data,
                              thread_mode);
}

/*
 * void
 * glDeleteBuffers(GLsizei n, const GLuint* buffers);
 */

typedef struct
{
   GLsizei n;
   const GLuint* buffers;
   void *buffers_copied; /* COPIED */
   int command_allocated;

} EVGL_API_Thread_Command_glDeleteBuffers;

void (*orig_evgl_api_glDeleteBuffers)(GLsizei n, const GLuint* buffers);

static void
_evgl_api_thread_glDeleteBuffers(void *data)
{
   EVGL_API_Thread_Command_glDeleteBuffers *thread_data =
      (EVGL_API_Thread_Command_glDeleteBuffers *)data;

   orig_evgl_api_glDeleteBuffers(thread_data->n,
                                 thread_data->buffers);


   if (thread_data->buffers_copied)
     eina_mempool_free(_mp_delete_object, thread_data->buffers_copied);

   if (thread_data->command_allocated)
     eina_mempool_free(_mp_command, thread_data);
}

EAPI void
glDeleteBuffers_evgl_api_thread_cmd(GLsizei n, const GLuint* buffers)
{
   if (!evas_evgl_thread_enabled() || (_main_thread_id != eina_thread_self()))
     {
        orig_evgl_api_glDeleteBuffers(n, buffers);
        return;
     }

   int thread_mode = EVAS_GL_THREAD_MODE_FINISH;

   EVGL_API_Thread_Command_glDeleteBuffers thread_data_local;
   EVGL_API_Thread_Command_glDeleteBuffers *thread_data = &thread_data_local;

   /* command_allocated flag init. */
   thread_data->command_allocated = 0;

   if (!evas_gl_thread_force_finish())
     { /* _flush */
        EVGL_API_Thread_Command_glDeleteBuffers *thread_data_new;
        thread_data_new = eina_mempool_malloc(_mp_command,
                                              sizeof(EVGL_API_Thread_Command_glDeleteBuffers));
        if (thread_data_new)
          {
             thread_data = thread_data_new;
             thread_data->command_allocated = 1;
             thread_mode = EVAS_GL_THREAD_MODE_FLUSH;
          }
     }

   thread_data->n = n;
   thread_data->buffers = buffers;

   thread_data->buffers_copied = NULL;
   if (thread_mode == EVAS_GL_THREAD_MODE_FINISH)
     goto finish;

   /* copy variable */
   if (buffers)
     {
        /* 1. check memory size */
        unsigned int copy_size = n * sizeof(GLuint);
        if (copy_size > _mp_delete_object_memory_size)
          {
             thread_mode = EVAS_GL_THREAD_MODE_FINISH;
             goto finish;
          }
        /* 2. malloc & copy */
        thread_data->buffers_copied = eina_mempool_malloc(_mp_delete_object, copy_size);
        if (thread_data->buffers_copied)
          {
             memcpy(thread_data->buffers_copied, buffers, copy_size);
          }
        else
          {
             thread_mode = EVAS_GL_THREAD_MODE_FINISH;
             goto finish;
          }
        /* 3. replace */
        thread_data->buffers = (const GLuint *)thread_data->buffers_copied;
     }
   /* end of copy variable */

finish:
   evas_gl_thread_cmd_enqueue(EVAS_GL_THREAD_TYPE_EVGL,
                              _evgl_api_thread_glDeleteBuffers,
                              thread_data,
                              thread_mode);
}

/*
 * void
 * glDeleteFramebuffers(GLsizei n, const GLuint* framebuffers);
 */

typedef struct
{
   GLsizei n;
   const GLuint* framebuffers;
   void *framebuffers_copied; /* COPIED */
   int command_allocated;

} EVGL_API_Thread_Command_glDeleteFramebuffers;

void (*orig_evgl_api_glDeleteFramebuffers)(GLsizei n, const GLuint* framebuffers);

static void
_evgl_api_thread_glDeleteFramebuffers(void *data)
{
   EVGL_API_Thread_Command_glDeleteFramebuffers *thread_data =
      (EVGL_API_Thread_Command_glDeleteFramebuffers *)data;

   orig_evgl_api_glDeleteFramebuffers(thread_data->n,
                                      thread_data->framebuffers);


   if (thread_data->framebuffers_copied)
     eina_mempool_free(_mp_delete_object, thread_data->framebuffers_copied);

   if (thread_data->command_allocated)
     eina_mempool_free(_mp_command, thread_data);
}

EAPI void
glDeleteFramebuffers_evgl_api_thread_cmd(GLsizei n, const GLuint* framebuffers)
{
   if (!evas_evgl_thread_enabled() || (_main_thread_id != eina_thread_self()))
     {
        orig_evgl_api_glDeleteFramebuffers(n, framebuffers);
        return;
     }

   int thread_mode = EVAS_GL_THREAD_MODE_FINISH;

   EVGL_API_Thread_Command_glDeleteFramebuffers thread_data_local;
   EVGL_API_Thread_Command_glDeleteFramebuffers *thread_data = &thread_data_local;

   /* command_allocated flag init. */
   thread_data->command_allocated = 0;

   if (!evas_gl_thread_force_finish())
     { /* _flush */
        EVGL_API_Thread_Command_glDeleteFramebuffers *thread_data_new;
        thread_data_new = eina_mempool_malloc(_mp_command,
                                              sizeof(EVGL_API_Thread_Command_glDeleteFramebuffers));
        if (thread_data_new)
          {
             thread_data = thread_data_new;
             thread_data->command_allocated = 1;
             thread_mode = EVAS_GL_THREAD_MODE_FLUSH;
          }
     }

   thread_data->n = n;
   thread_data->framebuffers = framebuffers;

   thread_data->framebuffers_copied = NULL;
   if (thread_mode == EVAS_GL_THREAD_MODE_FINISH)
     goto finish;

   /* copy variable */
   if (framebuffers)
     {
        /* 1. check memory size */
        unsigned int copy_size = n * sizeof(GLuint);
        if (copy_size > _mp_delete_object_memory_size)
          {
             thread_mode = EVAS_GL_THREAD_MODE_FINISH;
             goto finish;
          }
        /* 2. malloc & copy */
        thread_data->framebuffers_copied = eina_mempool_malloc(_mp_delete_object, copy_size);
        if (thread_data->framebuffers_copied)
          {
             memcpy(thread_data->framebuffers_copied, framebuffers, copy_size);
          }
        else
          {
             thread_mode = EVAS_GL_THREAD_MODE_FINISH;
             goto finish;
          }
        /* 3. replace */
        thread_data->framebuffers = (const GLuint *)thread_data->framebuffers_copied;
     }
   /* end of copy variable */

finish:
   evas_gl_thread_cmd_enqueue(EVAS_GL_THREAD_TYPE_EVGL,
                              _evgl_api_thread_glDeleteFramebuffers,
                              thread_data,
                              thread_mode);
}

/*
 * void
 * glDeleteProgram(GLuint program);
 */

typedef struct
{
   GLuint program;

} EVGL_API_Thread_Command_glDeleteProgram;

void (*orig_evgl_api_glDeleteProgram)(GLuint program);

static void
_evgl_api_thread_glDeleteProgram(void *data)
{
   EVGL_API_Thread_Command_glDeleteProgram *thread_data =
      (EVGL_API_Thread_Command_glDeleteProgram *)data;

   orig_evgl_api_glDeleteProgram(thread_data->program);

}

EAPI void
glDeleteProgram_evgl_api_thread_cmd(GLuint program)
{
   if (!evas_evgl_thread_enabled() || (_main_thread_id != eina_thread_self()))
     {
        orig_evgl_api_glDeleteProgram(program);
        return;
     }

   int thread_mode = EVAS_GL_THREAD_MODE_FINISH;

   EVGL_API_Thread_Command_glDeleteProgram thread_data_local;
   EVGL_API_Thread_Command_glDeleteProgram *thread_data = &thread_data_local;

   thread_data->program = program;

   evas_gl_thread_cmd_enqueue(EVAS_GL_THREAD_TYPE_EVGL,
                              _evgl_api_thread_glDeleteProgram,
                              thread_data,
                              thread_mode);
}

/*
 * void
 * glDeleteRenderbuffers(GLsizei n, const GLuint* renderbuffers);
 */

typedef struct
{
   GLsizei n;
   const GLuint* renderbuffers;
   void *renderbuffers_copied; /* COPIED */
   int command_allocated;

} EVGL_API_Thread_Command_glDeleteRenderbuffers;

void (*orig_evgl_api_glDeleteRenderbuffers)(GLsizei n, const GLuint* renderbuffers);

static void
_evgl_api_thread_glDeleteRenderbuffers(void *data)
{
   EVGL_API_Thread_Command_glDeleteRenderbuffers *thread_data =
      (EVGL_API_Thread_Command_glDeleteRenderbuffers *)data;

   orig_evgl_api_glDeleteRenderbuffers(thread_data->n,
                                       thread_data->renderbuffers);


   if (thread_data->renderbuffers_copied)
     eina_mempool_free(_mp_delete_object, thread_data->renderbuffers_copied);

   if (thread_data->command_allocated)
     eina_mempool_free(_mp_command, thread_data);
}

EAPI void
glDeleteRenderbuffers_evgl_api_thread_cmd(GLsizei n, const GLuint* renderbuffers)
{
   if (!evas_evgl_thread_enabled() || (_main_thread_id != eina_thread_self()))
     {
        orig_evgl_api_glDeleteRenderbuffers(n, renderbuffers);
        return;
     }

   int thread_mode = EVAS_GL_THREAD_MODE_FINISH;

   EVGL_API_Thread_Command_glDeleteRenderbuffers thread_data_local;
   EVGL_API_Thread_Command_glDeleteRenderbuffers *thread_data = &thread_data_local;

   /* command_allocated flag init. */
   thread_data->command_allocated = 0;

   if (!evas_gl_thread_force_finish())
     { /* _flush */
        EVGL_API_Thread_Command_glDeleteRenderbuffers *thread_data_new;
        thread_data_new = eina_mempool_malloc(_mp_command,
                                              sizeof(EVGL_API_Thread_Command_glDeleteRenderbuffers));
        if (thread_data_new)
          {
             thread_data = thread_data_new;
             thread_data->command_allocated = 1;
             thread_mode = EVAS_GL_THREAD_MODE_FLUSH;
          }
     }

   thread_data->n = n;
   thread_data->renderbuffers = renderbuffers;

   thread_data->renderbuffers_copied = NULL;
   if (thread_mode == EVAS_GL_THREAD_MODE_FINISH)
     goto finish;

   /* copy variable */
   if (renderbuffers)
     {
        /* 1. check memory size */
        unsigned int copy_size = n * sizeof(GLuint);
        if (copy_size > _mp_delete_object_memory_size)
          {
             thread_mode = EVAS_GL_THREAD_MODE_FINISH;
             goto finish;
          }
        /* 2. malloc & copy */
        thread_data->renderbuffers_copied = eina_mempool_malloc(_mp_delete_object, copy_size);
        if (thread_data->renderbuffers_copied)
          {
             memcpy(thread_data->renderbuffers_copied, renderbuffers, copy_size);
          }
        else
          {
             thread_mode = EVAS_GL_THREAD_MODE_FINISH;
             goto finish;
          }
        /* 3. replace */
        thread_data->renderbuffers = (const GLuint *)thread_data->renderbuffers_copied;
     }
   /* end of copy variable */

finish:
   evas_gl_thread_cmd_enqueue(EVAS_GL_THREAD_TYPE_EVGL,
                              _evgl_api_thread_glDeleteRenderbuffers,
                              thread_data,
                              thread_mode);
}

/*
 * void
 * glDeleteShader(GLuint shader);
 */

typedef struct
{
   GLuint shader;
   int command_allocated;

} EVGL_API_Thread_Command_glDeleteShader;

void (*orig_evgl_api_glDeleteShader)(GLuint shader);

static void
_evgl_api_thread_glDeleteShader(void *data)
{
   EVGL_API_Thread_Command_glDeleteShader *thread_data =
      (EVGL_API_Thread_Command_glDeleteShader *)data;

   orig_evgl_api_glDeleteShader(thread_data->shader);

   if (thread_data->command_allocated)
     eina_mempool_free(_mp_command, thread_data);
}

EAPI void
glDeleteShader_evgl_api_thread_cmd(GLuint shader)
{
   if (!evas_evgl_thread_enabled() || (_main_thread_id != eina_thread_self()))
     {
        orig_evgl_api_glDeleteShader(shader);
        return;
     }

   int thread_mode = EVAS_GL_THREAD_MODE_FINISH;

   EVGL_API_Thread_Command_glDeleteShader thread_data_local;
   EVGL_API_Thread_Command_glDeleteShader *thread_data = &thread_data_local;

   /* command_allocated flag init. */
   thread_data->command_allocated = 0;

   if (!evas_gl_thread_force_finish())
     { /* _flush */
        EVGL_API_Thread_Command_glDeleteShader *thread_data_new;
        thread_data_new = eina_mempool_malloc(_mp_command,
                                              sizeof(EVGL_API_Thread_Command_glDeleteShader));
        if (thread_data_new)
          {
             thread_data = thread_data_new;
             thread_data->command_allocated = 1;
             thread_mode = EVAS_GL_THREAD_MODE_FLUSH;
          }
     }

   thread_data->shader = shader;

   evas_gl_thread_cmd_enqueue(EVAS_GL_THREAD_TYPE_EVGL,
                              _evgl_api_thread_glDeleteShader,
                              thread_data,
                              thread_mode);
}

/*
 * void
 * glDeleteTextures(GLsizei n, const GLuint* textures);
 */

typedef struct
{
   GLsizei n;
   const GLuint* textures;
   void *textures_copied; /* COPIED */
   int command_allocated;

} EVGL_API_Thread_Command_glDeleteTextures;

void (*orig_evgl_api_glDeleteTextures)(GLsizei n, const GLuint* textures);

static void
_evgl_api_thread_glDeleteTextures(void *data)
{
   EVGL_API_Thread_Command_glDeleteTextures *thread_data =
      (EVGL_API_Thread_Command_glDeleteTextures *)data;

   orig_evgl_api_glDeleteTextures(thread_data->n,
                                  thread_data->textures);


   if (thread_data->textures_copied)
     eina_mempool_free(_mp_delete_object, thread_data->textures_copied);

   if (thread_data->command_allocated)
     eina_mempool_free(_mp_command, thread_data);
}

EAPI void
glDeleteTextures_evgl_api_thread_cmd(GLsizei n, const GLuint* textures)
{
   if (!evas_evgl_thread_enabled() || (_main_thread_id != eina_thread_self()))
     {
        orig_evgl_api_glDeleteTextures(n, textures);
        return;
     }

   int thread_mode = EVAS_GL_THREAD_MODE_FINISH;

   EVGL_API_Thread_Command_glDeleteTextures thread_data_local;
   EVGL_API_Thread_Command_glDeleteTextures *thread_data = &thread_data_local;

   /* command_allocated flag init. */
   thread_data->command_allocated = 0;

   if (!evas_gl_thread_force_finish())
     { /* _flush */
        EVGL_API_Thread_Command_glDeleteTextures *thread_data_new;
        thread_data_new = eina_mempool_malloc(_mp_command,
                                              sizeof(EVGL_API_Thread_Command_glDeleteTextures));
        if (thread_data_new)
          {
             thread_data = thread_data_new;
             thread_data->command_allocated = 1;
             thread_mode = EVAS_GL_THREAD_MODE_FLUSH;
          }
     }

   thread_data->n = n;
   thread_data->textures = textures;

   thread_data->textures_copied = NULL;
   if (thread_mode == EVAS_GL_THREAD_MODE_FINISH)
     goto finish;

   /* copy variable */
   if (textures)
     {
        /* 1. check memory size */
        unsigned int copy_size = n * sizeof(GLuint);
        if (copy_size > _mp_delete_object_memory_size)
          {
             thread_mode = EVAS_GL_THREAD_MODE_FINISH;
             goto finish;
          }
        /* 2. malloc & copy */
        thread_data->textures_copied = eina_mempool_malloc(_mp_delete_object, copy_size);
        if (thread_data->textures_copied)
          {
             memcpy(thread_data->textures_copied, textures, copy_size);
          }
        else
          {
             thread_mode = EVAS_GL_THREAD_MODE_FINISH;
             goto finish;
          }
        /* 3. replace */
        thread_data->textures = (const GLuint *)thread_data->textures_copied;
     }
   /* end of copy variable */

finish:
   evas_gl_thread_cmd_enqueue(EVAS_GL_THREAD_TYPE_EVGL,
                              _evgl_api_thread_glDeleteTextures,
                              thread_data,
                              thread_mode);
}

/*
 * void
 * glDepthFunc(GLenum func);
 */

typedef struct
{
   GLenum func;

} EVGL_API_Thread_Command_glDepthFunc;

void (*orig_evgl_api_glDepthFunc)(GLenum func);

static void
_evgl_api_thread_glDepthFunc(void *data)
{
   EVGL_API_Thread_Command_glDepthFunc *thread_data =
      (EVGL_API_Thread_Command_glDepthFunc *)data;

   orig_evgl_api_glDepthFunc(thread_data->func);

}

EAPI void
glDepthFunc_evgl_api_thread_cmd(GLenum func)
{
   if (!evas_evgl_thread_enabled() || (_main_thread_id != eina_thread_self()))
     {
        orig_evgl_api_glDepthFunc(func);
        return;
     }

   int thread_mode = EVAS_GL_THREAD_MODE_FINISH;

   EVGL_API_Thread_Command_glDepthFunc thread_data_local;
   EVGL_API_Thread_Command_glDepthFunc *thread_data = &thread_data_local;

   thread_data->func = func;

   evas_gl_thread_cmd_enqueue(EVAS_GL_THREAD_TYPE_EVGL,
                              _evgl_api_thread_glDepthFunc,
                              thread_data,
                              thread_mode);
}

/*
 * void
 * glDepthMask(GLboolean flag);
 */

typedef struct
{
   GLboolean flag;
   int command_allocated;

} EVGL_API_Thread_Command_glDepthMask;

void (*orig_evgl_api_glDepthMask)(GLboolean flag);

static void
_evgl_api_thread_glDepthMask(void *data)
{
   EVGL_API_Thread_Command_glDepthMask *thread_data =
      (EVGL_API_Thread_Command_glDepthMask *)data;

   orig_evgl_api_glDepthMask(thread_data->flag);

   if (thread_data->command_allocated)
     eina_mempool_free(_mp_command, thread_data);
}

EAPI void
glDepthMask_evgl_api_thread_cmd(GLboolean flag)
{
   if (!evas_evgl_thread_enabled() || (_main_thread_id != eina_thread_self()))
     {
        orig_evgl_api_glDepthMask(flag);
        return;
     }

   int thread_mode = EVAS_GL_THREAD_MODE_FINISH;

   EVGL_API_Thread_Command_glDepthMask thread_data_local;
   EVGL_API_Thread_Command_glDepthMask *thread_data = &thread_data_local;

   /* command_allocated flag init. */
   thread_data->command_allocated = 0;

   if (!evas_gl_thread_force_finish())
     { /* _flush */
        EVGL_API_Thread_Command_glDepthMask *thread_data_new;
        thread_data_new = eina_mempool_malloc(_mp_command,
                                              sizeof(EVGL_API_Thread_Command_glDepthMask));
        if (thread_data_new)
          {
             thread_data = thread_data_new;
             thread_data->command_allocated = 1;
             thread_mode = EVAS_GL_THREAD_MODE_FLUSH;
          }
     }

   thread_data->flag = flag;

   evas_gl_thread_cmd_enqueue(EVAS_GL_THREAD_TYPE_EVGL,
                              _evgl_api_thread_glDepthMask,
                              thread_data,
                              thread_mode);
}

/*
 * void
 * glDepthRangef(GLclampf zNear, GLclampf zFar);
 */

typedef struct
{
   GLclampf zNear;
   GLclampf zFar;

} EVGL_API_Thread_Command_glDepthRangef;

void (*orig_evgl_api_glDepthRangef)(GLclampf zNear, GLclampf zFar);

static void
_evgl_api_thread_glDepthRangef(void *data)
{
   EVGL_API_Thread_Command_glDepthRangef *thread_data =
      (EVGL_API_Thread_Command_glDepthRangef *)data;

   orig_evgl_api_glDepthRangef(thread_data->zNear,
                               thread_data->zFar);

}

EAPI void
glDepthRangef_evgl_api_thread_cmd(GLclampf zNear, GLclampf zFar)
{
   if (!evas_evgl_thread_enabled() || (_main_thread_id != eina_thread_self()))
     {
        orig_evgl_api_glDepthRangef(zNear, zFar);
        return;
     }

   int thread_mode = EVAS_GL_THREAD_MODE_FINISH;

   EVGL_API_Thread_Command_glDepthRangef thread_data_local;
   EVGL_API_Thread_Command_glDepthRangef *thread_data = &thread_data_local;

   thread_data->zNear = zNear;
   thread_data->zFar = zFar;

   evas_gl_thread_cmd_enqueue(EVAS_GL_THREAD_TYPE_EVGL,
                              _evgl_api_thread_glDepthRangef,
                              thread_data,
                              thread_mode);
}

/*
 * void
 * glDetachShader(GLuint program, GLuint shader);
 */

typedef struct
{
   GLuint program;
   GLuint shader;
   int command_allocated;

} EVGL_API_Thread_Command_glDetachShader;

void (*orig_evgl_api_glDetachShader)(GLuint program, GLuint shader);

static void
_evgl_api_thread_glDetachShader(void *data)
{
   EVGL_API_Thread_Command_glDetachShader *thread_data =
      (EVGL_API_Thread_Command_glDetachShader *)data;

   orig_evgl_api_glDetachShader(thread_data->program,
                                thread_data->shader);

   if (thread_data->command_allocated)
     eina_mempool_free(_mp_command, thread_data);
}

EAPI void
glDetachShader_evgl_api_thread_cmd(GLuint program, GLuint shader)
{
   if (!evas_evgl_thread_enabled() || (_main_thread_id != eina_thread_self()))
     {
        orig_evgl_api_glDetachShader(program, shader);
        return;
     }

   int thread_mode = EVAS_GL_THREAD_MODE_FINISH;

   EVGL_API_Thread_Command_glDetachShader thread_data_local;
   EVGL_API_Thread_Command_glDetachShader *thread_data = &thread_data_local;

   /* command_allocated flag init. */
   thread_data->command_allocated = 0;

   if (!evas_gl_thread_force_finish())
     { /* _flush */
        EVGL_API_Thread_Command_glDetachShader *thread_data_new;
        thread_data_new = eina_mempool_malloc(_mp_command,
                                              sizeof(EVGL_API_Thread_Command_glDetachShader));
        if (thread_data_new)
          {
             thread_data = thread_data_new;
             thread_data->command_allocated = 1;
             thread_mode = EVAS_GL_THREAD_MODE_FLUSH;
          }
     }

   thread_data->program = program;
   thread_data->shader = shader;

   evas_gl_thread_cmd_enqueue(EVAS_GL_THREAD_TYPE_EVGL,
                              _evgl_api_thread_glDetachShader,
                              thread_data,
                              thread_mode);
}

/*
 * void
 * glDisable(GLenum cap);
 */

typedef struct
{
   GLenum cap;
   int command_allocated;

} EVGL_API_Thread_Command_glDisable;

void (*orig_evgl_api_glDisable)(GLenum cap);

static void
_evgl_api_thread_glDisable(void *data)
{
   EVGL_API_Thread_Command_glDisable *thread_data =
      (EVGL_API_Thread_Command_glDisable *)data;

   orig_evgl_api_glDisable(thread_data->cap);

   if (thread_data->command_allocated)
     eina_mempool_free(_mp_command, thread_data);
}

EAPI void
glDisable_evgl_api_thread_cmd(GLenum cap)
{
   if (!evas_evgl_thread_enabled() || (_main_thread_id != eina_thread_self()))
     {
        orig_evgl_api_glDisable(cap);
        return;
     }

   int thread_mode = EVAS_GL_THREAD_MODE_FINISH;

   EVGL_API_Thread_Command_glDisable thread_data_local;
   EVGL_API_Thread_Command_glDisable *thread_data = &thread_data_local;

   /* command_allocated flag init. */
   thread_data->command_allocated = 0;

   if (!evas_gl_thread_force_finish())
     { /* _flush */
        EVGL_API_Thread_Command_glDisable *thread_data_new;
        thread_data_new = eina_mempool_malloc(_mp_command,
                                              sizeof(EVGL_API_Thread_Command_glDisable));
        if (thread_data_new)
          {
             thread_data = thread_data_new;
             thread_data->command_allocated = 1;
             thread_mode = EVAS_GL_THREAD_MODE_FLUSH;
          }
     }

   thread_data->cap = cap;

   evas_gl_thread_cmd_enqueue(EVAS_GL_THREAD_TYPE_EVGL,
                              _evgl_api_thread_glDisable,
                              thread_data,
                              thread_mode);
}

/*
 * void
 * glDisableVertexAttribArray(GLuint index);
 */

typedef struct
{
   GLuint index;
   int command_allocated;

} EVGL_API_Thread_Command_glDisableVertexAttribArray;

void (*orig_evgl_api_glDisableVertexAttribArray)(GLuint index);

static void
_evgl_api_thread_glDisableVertexAttribArray(void *data)
{
   EVGL_API_Thread_Command_glDisableVertexAttribArray *thread_data =
      (EVGL_API_Thread_Command_glDisableVertexAttribArray *)data;

   orig_evgl_api_glDisableVertexAttribArray(thread_data->index);

   if (thread_data->command_allocated)
     eina_mempool_free(_mp_command, thread_data);
}

EAPI void
glDisableVertexAttribArray_evgl_api_thread_cmd(GLuint index)
{
   if (!evas_evgl_thread_enabled() || (_main_thread_id != eina_thread_self()))
     {
        orig_evgl_api_glDisableVertexAttribArray(index);
        return;
     }

   int thread_mode = EVAS_GL_THREAD_MODE_FINISH;

   EVGL_API_Thread_Command_glDisableVertexAttribArray thread_data_local;
   EVGL_API_Thread_Command_glDisableVertexAttribArray *thread_data = &thread_data_local;

   /* command_allocated flag init. */
   thread_data->command_allocated = 0;

   if (!evas_gl_thread_force_finish())
     { /* _flush */
        EVGL_API_Thread_Command_glDisableVertexAttribArray *thread_data_new;
        thread_data_new = eina_mempool_malloc(_mp_command,
                                              sizeof(EVGL_API_Thread_Command_glDisableVertexAttribArray));
        if (thread_data_new)
          {
             thread_data = thread_data_new;
             thread_data->command_allocated = 1;
             thread_mode = EVAS_GL_THREAD_MODE_FLUSH;
          }
     }

   thread_data->index = index;

   evas_gl_thread_cmd_enqueue(EVAS_GL_THREAD_TYPE_EVGL,
                              _evgl_api_thread_glDisableVertexAttribArray,
                              thread_data,
                              thread_mode);
}

/*
 * void
 * glDrawArrays(GLenum mode, GLint first, GLsizei count);
 */

typedef struct
{
   GLenum mode;
   GLint first;
   GLsizei count;

} EVGL_API_Thread_Command_glDrawArrays;

void (*orig_evgl_api_glDrawArrays)(GLenum mode, GLint first, GLsizei count);

static void
_evgl_api_thread_glDrawArrays(void *data)
{
   EVGL_API_Thread_Command_glDrawArrays *thread_data =
      (EVGL_API_Thread_Command_glDrawArrays *)data;

   orig_evgl_api_glDrawArrays(thread_data->mode,
                              thread_data->first,
                              thread_data->count);

}

EAPI void
glDrawArrays_evgl_api_thread_cmd(GLenum mode, GLint first, GLsizei count)
{
   if (!evas_evgl_thread_enabled() || (_main_thread_id != eina_thread_self()))
     {
        orig_evgl_api_glDrawArrays(mode, first, count);
        return;
     }

   int thread_mode = EVAS_GL_THREAD_MODE_FINISH;

   EVGL_API_Thread_Command_glDrawArrays thread_data_local;
   EVGL_API_Thread_Command_glDrawArrays *thread_data = &thread_data_local;

   thread_data->mode = mode;
   thread_data->first = first;
   thread_data->count = count;

   evas_gl_thread_cmd_enqueue(EVAS_GL_THREAD_TYPE_EVGL,
                              _evgl_api_thread_glDrawArrays,
                              thread_data,
                              thread_mode);
}

/*
 * void
 * glDrawElements(GLenum mode, GLsizei count, GLenum type, const void* indices);
 */

typedef struct
{
   GLenum mode;
   GLsizei count;
   GLenum type;
   const void* indices;

} EVGL_API_Thread_Command_glDrawElements;

void (*orig_evgl_api_glDrawElements)(GLenum mode, GLsizei count, GLenum type, const void* indices);

static void
_evgl_api_thread_glDrawElements(void *data)
{
   EVGL_API_Thread_Command_glDrawElements *thread_data =
      (EVGL_API_Thread_Command_glDrawElements *)data;

   orig_evgl_api_glDrawElements(thread_data->mode,
                                thread_data->count,
                                thread_data->type,
                                thread_data->indices);

}

EAPI void
glDrawElements_evgl_api_thread_cmd(GLenum mode, GLsizei count, GLenum type, const void* indices)
{
   if (!evas_evgl_thread_enabled() || (_main_thread_id != eina_thread_self()))
     {
        orig_evgl_api_glDrawElements(mode, count, type, indices);
        return;
     }

   int thread_mode = EVAS_GL_THREAD_MODE_FINISH;

   EVGL_API_Thread_Command_glDrawElements thread_data_local;
   EVGL_API_Thread_Command_glDrawElements *thread_data = &thread_data_local;

   thread_data->mode = mode;
   thread_data->count = count;
   thread_data->type = type;
   thread_data->indices = indices;

   evas_gl_thread_cmd_enqueue(EVAS_GL_THREAD_TYPE_EVGL,
                              _evgl_api_thread_glDrawElements,
                              thread_data,
                              thread_mode);
}

/*
 * void
 * glEnable(GLenum cap);
 */

typedef struct
{
   GLenum cap;
   int command_allocated;

} EVGL_API_Thread_Command_glEnable;

void (*orig_evgl_api_glEnable)(GLenum cap);

static void
_evgl_api_thread_glEnable(void *data)
{
   EVGL_API_Thread_Command_glEnable *thread_data =
      (EVGL_API_Thread_Command_glEnable *)data;

   orig_evgl_api_glEnable(thread_data->cap);

   if (thread_data->command_allocated)
     eina_mempool_free(_mp_command, thread_data);
}

EAPI void
glEnable_evgl_api_thread_cmd(GLenum cap)
{
   if (!evas_evgl_thread_enabled() || (_main_thread_id != eina_thread_self()))
     {
        orig_evgl_api_glEnable(cap);
        return;
     }

   int thread_mode = EVAS_GL_THREAD_MODE_FINISH;

   EVGL_API_Thread_Command_glEnable thread_data_local;
   EVGL_API_Thread_Command_glEnable *thread_data = &thread_data_local;

   /* command_allocated flag init. */
   thread_data->command_allocated = 0;

   if (!evas_gl_thread_force_finish())
     { /* _flush */
        EVGL_API_Thread_Command_glEnable *thread_data_new;
        thread_data_new = eina_mempool_malloc(_mp_command,
                                              sizeof(EVGL_API_Thread_Command_glEnable));
        if (thread_data_new)
          {
             thread_data = thread_data_new;
             thread_data->command_allocated = 1;
             thread_mode = EVAS_GL_THREAD_MODE_FLUSH;
          }
     }

   thread_data->cap = cap;

   evas_gl_thread_cmd_enqueue(EVAS_GL_THREAD_TYPE_EVGL,
                              _evgl_api_thread_glEnable,
                              thread_data,
                              thread_mode);
}

/*
 * void
 * glEnableVertexAttribArray(GLuint index);
 */

typedef struct
{
   GLuint index;
   int command_allocated;

} EVGL_API_Thread_Command_glEnableVertexAttribArray;

void (*orig_evgl_api_glEnableVertexAttribArray)(GLuint index);

static void
_evgl_api_thread_glEnableVertexAttribArray(void *data)
{
   EVGL_API_Thread_Command_glEnableVertexAttribArray *thread_data =
      (EVGL_API_Thread_Command_glEnableVertexAttribArray *)data;

   orig_evgl_api_glEnableVertexAttribArray(thread_data->index);

   if (thread_data->command_allocated)
     eina_mempool_free(_mp_command, thread_data);
}

EAPI void
glEnableVertexAttribArray_evgl_api_thread_cmd(GLuint index)
{
   if (!evas_evgl_thread_enabled() || (_main_thread_id != eina_thread_self()))
     {
        orig_evgl_api_glEnableVertexAttribArray(index);
        return;
     }

   int thread_mode = EVAS_GL_THREAD_MODE_FINISH;

   EVGL_API_Thread_Command_glEnableVertexAttribArray thread_data_local;
   EVGL_API_Thread_Command_glEnableVertexAttribArray *thread_data = &thread_data_local;

   /* command_allocated flag init. */
   thread_data->command_allocated = 0;

   if (!evas_gl_thread_force_finish())
     { /* _flush */
        EVGL_API_Thread_Command_glEnableVertexAttribArray *thread_data_new;
        thread_data_new = eina_mempool_malloc(_mp_command,
                                              sizeof(EVGL_API_Thread_Command_glEnableVertexAttribArray));
        if (thread_data_new)
          {
             thread_data = thread_data_new;
             thread_data->command_allocated = 1;
             thread_mode = EVAS_GL_THREAD_MODE_FLUSH;
          }
     }

   thread_data->index = index;

   evas_gl_thread_cmd_enqueue(EVAS_GL_THREAD_TYPE_EVGL,
                              _evgl_api_thread_glEnableVertexAttribArray,
                              thread_data,
                              thread_mode);
}

/*
 * void
 * glFinish(void);
 */

void (*orig_evgl_api_glFinish)(void);

static void
_evgl_api_thread_glFinish(void *data EINA_UNUSED)
{
   orig_evgl_api_glFinish();

}

EAPI void
glFinish_evgl_api_thread_cmd(void)
{
   if (!evas_evgl_thread_enabled() || (_main_thread_id != eina_thread_self()))
     {
        orig_evgl_api_glFinish();
        return;
     }

   int thread_mode = EVAS_GL_THREAD_MODE_FINISH;

   evas_gl_thread_cmd_enqueue(EVAS_GL_THREAD_TYPE_EVGL,
                              _evgl_api_thread_glFinish,
                              NULL,
                              thread_mode);
}

/*
 * void
 * glFlush(void);
 */

void (*orig_evgl_api_glFlush)(void);

static void
_evgl_api_thread_glFlush(void *data EINA_UNUSED)
{
   orig_evgl_api_glFlush();

}

EAPI void
glFlush_evgl_api_thread_cmd(void)
{
   if (!evas_evgl_thread_enabled() || (_main_thread_id != eina_thread_self()))
     {
        orig_evgl_api_glFlush();
        return;
     }

   int thread_mode = EVAS_GL_THREAD_MODE_FINISH;

   evas_gl_thread_cmd_enqueue(EVAS_GL_THREAD_TYPE_EVGL,
                              _evgl_api_thread_glFlush,
                              NULL,
                              thread_mode);
}

/*
 * void
 * glFramebufferRenderbuffer(GLenum target, GLenum attachment, GLenum renderbuffertarget, GLuint renderbuffer);
 */

typedef struct
{
   GLenum target;
   GLenum attachment;
   GLenum renderbuffertarget;
   GLuint renderbuffer;
   int command_allocated;

} EVGL_API_Thread_Command_glFramebufferRenderbuffer;

void (*orig_evgl_api_glFramebufferRenderbuffer)(GLenum target, GLenum attachment, GLenum renderbuffertarget, GLuint renderbuffer);

static void
_evgl_api_thread_glFramebufferRenderbuffer(void *data)
{
   EVGL_API_Thread_Command_glFramebufferRenderbuffer *thread_data =
      (EVGL_API_Thread_Command_glFramebufferRenderbuffer *)data;

   orig_evgl_api_glFramebufferRenderbuffer(thread_data->target,
                                           thread_data->attachment,
                                           thread_data->renderbuffertarget,
                                           thread_data->renderbuffer);

   if (thread_data->command_allocated)
     eina_mempool_free(_mp_command, thread_data);
}

EAPI void
glFramebufferRenderbuffer_evgl_api_thread_cmd(GLenum target, GLenum attachment, GLenum renderbuffertarget, GLuint renderbuffer)
{
   if (!evas_evgl_thread_enabled() || (_main_thread_id != eina_thread_self()))
     {
        orig_evgl_api_glFramebufferRenderbuffer(target, attachment, renderbuffertarget, renderbuffer);
        return;
     }

   int thread_mode = EVAS_GL_THREAD_MODE_FINISH;

   EVGL_API_Thread_Command_glFramebufferRenderbuffer thread_data_local;
   EVGL_API_Thread_Command_glFramebufferRenderbuffer *thread_data = &thread_data_local;

   /* command_allocated flag init. */
   thread_data->command_allocated = 0;

   if (!evas_gl_thread_force_finish())
     { /* _flush */
        EVGL_API_Thread_Command_glFramebufferRenderbuffer *thread_data_new;
        thread_data_new = eina_mempool_malloc(_mp_command,
                                              sizeof(EVGL_API_Thread_Command_glFramebufferRenderbuffer));
        if (thread_data_new)
          {
             thread_data = thread_data_new;
             thread_data->command_allocated = 1;
             thread_mode = EVAS_GL_THREAD_MODE_FLUSH;
          }
     }

   thread_data->target = target;
   thread_data->attachment = attachment;
   thread_data->renderbuffertarget = renderbuffertarget;
   thread_data->renderbuffer = renderbuffer;

   evas_gl_thread_cmd_enqueue(EVAS_GL_THREAD_TYPE_EVGL,
                              _evgl_api_thread_glFramebufferRenderbuffer,
                              thread_data,
                              thread_mode);
}

/*
 * void
 * glFramebufferTexture2D(GLenum target, GLenum attachment, GLenum textarget, GLuint texture, GLint level);
 */

typedef struct
{
   GLenum target;
   GLenum attachment;
   GLenum textarget;
   GLuint texture;
   GLint level;
   int command_allocated;

} EVGL_API_Thread_Command_glFramebufferTexture2D;

void (*orig_evgl_api_glFramebufferTexture2D)(GLenum target, GLenum attachment, GLenum textarget, GLuint texture, GLint level);

static void
_evgl_api_thread_glFramebufferTexture2D(void *data)
{
   EVGL_API_Thread_Command_glFramebufferTexture2D *thread_data =
      (EVGL_API_Thread_Command_glFramebufferTexture2D *)data;

   orig_evgl_api_glFramebufferTexture2D(thread_data->target,
                                        thread_data->attachment,
                                        thread_data->textarget,
                                        thread_data->texture,
                                        thread_data->level);

   if (thread_data->command_allocated)
     eina_mempool_free(_mp_command, thread_data);
}

EAPI void
glFramebufferTexture2D_evgl_api_thread_cmd(GLenum target, GLenum attachment, GLenum textarget, GLuint texture, GLint level)
{
   if (!evas_evgl_thread_enabled() || (_main_thread_id != eina_thread_self()))
     {
        orig_evgl_api_glFramebufferTexture2D(target, attachment, textarget, texture, level);
        return;
     }

   int thread_mode = EVAS_GL_THREAD_MODE_FINISH;

   EVGL_API_Thread_Command_glFramebufferTexture2D thread_data_local;
   EVGL_API_Thread_Command_glFramebufferTexture2D *thread_data = &thread_data_local;

   /* command_allocated flag init. */
   thread_data->command_allocated = 0;

   if (!evas_gl_thread_force_finish())
     { /* _flush */
        EVGL_API_Thread_Command_glFramebufferTexture2D *thread_data_new;
        thread_data_new = eina_mempool_malloc(_mp_command,
                                              sizeof(EVGL_API_Thread_Command_glFramebufferTexture2D));
        if (thread_data_new)
          {
             thread_data = thread_data_new;
             thread_data->command_allocated = 1;
             thread_mode = EVAS_GL_THREAD_MODE_FLUSH;
          }
     }

   thread_data->target = target;
   thread_data->attachment = attachment;
   thread_data->textarget = textarget;
   thread_data->texture = texture;
   thread_data->level = level;

   evas_gl_thread_cmd_enqueue(EVAS_GL_THREAD_TYPE_EVGL,
                              _evgl_api_thread_glFramebufferTexture2D,
                              thread_data,
                              thread_mode);
}

/*
 * void
 * glFrontFace(GLenum mode);
 */

typedef struct
{
   GLenum mode;

} EVGL_API_Thread_Command_glFrontFace;

void (*orig_evgl_api_glFrontFace)(GLenum mode);

static void
_evgl_api_thread_glFrontFace(void *data)
{
   EVGL_API_Thread_Command_glFrontFace *thread_data =
      (EVGL_API_Thread_Command_glFrontFace *)data;

   orig_evgl_api_glFrontFace(thread_data->mode);

}

EAPI void
glFrontFace_evgl_api_thread_cmd(GLenum mode)
{
   if (!evas_evgl_thread_enabled() || (_main_thread_id != eina_thread_self()))
     {
        orig_evgl_api_glFrontFace(mode);
        return;
     }

   int thread_mode = EVAS_GL_THREAD_MODE_FINISH;

   EVGL_API_Thread_Command_glFrontFace thread_data_local;
   EVGL_API_Thread_Command_glFrontFace *thread_data = &thread_data_local;

   thread_data->mode = mode;

   evas_gl_thread_cmd_enqueue(EVAS_GL_THREAD_TYPE_EVGL,
                              _evgl_api_thread_glFrontFace,
                              thread_data,
                              thread_mode);
}

/*
 * void
 * glGenBuffers(GLsizei n, GLuint* buffers);
 */

typedef struct
{
   GLsizei n;
   GLuint* buffers;

} EVGL_API_Thread_Command_glGenBuffers;

void (*orig_evgl_api_glGenBuffers)(GLsizei n, GLuint* buffers);

static void
_evgl_api_thread_glGenBuffers(void *data)
{
   EVGL_API_Thread_Command_glGenBuffers *thread_data =
      (EVGL_API_Thread_Command_glGenBuffers *)data;

   orig_evgl_api_glGenBuffers(thread_data->n,
                              thread_data->buffers);

}

EAPI void
glGenBuffers_evgl_api_thread_cmd(GLsizei n, GLuint* buffers)
{
   if (!evas_evgl_thread_enabled() || (_main_thread_id != eina_thread_self()))
     {
        orig_evgl_api_glGenBuffers(n, buffers);
        return;
     }

   int thread_mode = EVAS_GL_THREAD_MODE_FINISH;

   EVGL_API_Thread_Command_glGenBuffers thread_data_local;
   EVGL_API_Thread_Command_glGenBuffers *thread_data = &thread_data_local;

   thread_data->n = n;
   thread_data->buffers = buffers;

   evas_gl_thread_cmd_enqueue(EVAS_GL_THREAD_TYPE_EVGL,
                              _evgl_api_thread_glGenBuffers,
                              thread_data,
                              thread_mode);
}

/*
 * void
 * glGenerateMipmap(GLenum target);
 */

typedef struct
{
   GLenum target;

} EVGL_API_Thread_Command_glGenerateMipmap;

void (*orig_evgl_api_glGenerateMipmap)(GLenum target);

static void
_evgl_api_thread_glGenerateMipmap(void *data)
{
   EVGL_API_Thread_Command_glGenerateMipmap *thread_data =
      (EVGL_API_Thread_Command_glGenerateMipmap *)data;

   orig_evgl_api_glGenerateMipmap(thread_data->target);

}

EAPI void
glGenerateMipmap_evgl_api_thread_cmd(GLenum target)
{
   if (!evas_evgl_thread_enabled() || (_main_thread_id != eina_thread_self()))
     {
        orig_evgl_api_glGenerateMipmap(target);
        return;
     }

   int thread_mode = EVAS_GL_THREAD_MODE_FINISH;

   EVGL_API_Thread_Command_glGenerateMipmap thread_data_local;
   EVGL_API_Thread_Command_glGenerateMipmap *thread_data = &thread_data_local;

   thread_data->target = target;

   evas_gl_thread_cmd_enqueue(EVAS_GL_THREAD_TYPE_EVGL,
                              _evgl_api_thread_glGenerateMipmap,
                              thread_data,
                              thread_mode);
}

/*
 * void
 * glGenFramebuffers(GLsizei n, GLuint* framebuffers);
 */

typedef struct
{
   GLsizei n;
   GLuint* framebuffers;

} EVGL_API_Thread_Command_glGenFramebuffers;

void (*orig_evgl_api_glGenFramebuffers)(GLsizei n, GLuint* framebuffers);

static void
_evgl_api_thread_glGenFramebuffers(void *data)
{
   EVGL_API_Thread_Command_glGenFramebuffers *thread_data =
      (EVGL_API_Thread_Command_glGenFramebuffers *)data;

   orig_evgl_api_glGenFramebuffers(thread_data->n,
                                   thread_data->framebuffers);

}

EAPI void
glGenFramebuffers_evgl_api_thread_cmd(GLsizei n, GLuint* framebuffers)
{
   if (!evas_evgl_thread_enabled() || (_main_thread_id != eina_thread_self()))
     {
        orig_evgl_api_glGenFramebuffers(n, framebuffers);
        return;
     }

   int thread_mode = EVAS_GL_THREAD_MODE_FINISH;

   EVGL_API_Thread_Command_glGenFramebuffers thread_data_local;
   EVGL_API_Thread_Command_glGenFramebuffers *thread_data = &thread_data_local;

   thread_data->n = n;
   thread_data->framebuffers = framebuffers;

   evas_gl_thread_cmd_enqueue(EVAS_GL_THREAD_TYPE_EVGL,
                              _evgl_api_thread_glGenFramebuffers,
                              thread_data,
                              thread_mode);
}

/*
 * void
 * glGenRenderbuffers(GLsizei n, GLuint* renderbuffers);
 */

typedef struct
{
   GLsizei n;
   GLuint* renderbuffers;

} EVGL_API_Thread_Command_glGenRenderbuffers;

void (*orig_evgl_api_glGenRenderbuffers)(GLsizei n, GLuint* renderbuffers);

static void
_evgl_api_thread_glGenRenderbuffers(void *data)
{
   EVGL_API_Thread_Command_glGenRenderbuffers *thread_data =
      (EVGL_API_Thread_Command_glGenRenderbuffers *)data;

   orig_evgl_api_glGenRenderbuffers(thread_data->n,
                                    thread_data->renderbuffers);

}

EAPI void
glGenRenderbuffers_evgl_api_thread_cmd(GLsizei n, GLuint* renderbuffers)
{
   if (!evas_evgl_thread_enabled() || (_main_thread_id != eina_thread_self()))
     {
        orig_evgl_api_glGenRenderbuffers(n, renderbuffers);
        return;
     }

   int thread_mode = EVAS_GL_THREAD_MODE_FINISH;

   EVGL_API_Thread_Command_glGenRenderbuffers thread_data_local;
   EVGL_API_Thread_Command_glGenRenderbuffers *thread_data = &thread_data_local;

   thread_data->n = n;
   thread_data->renderbuffers = renderbuffers;

   evas_gl_thread_cmd_enqueue(EVAS_GL_THREAD_TYPE_EVGL,
                              _evgl_api_thread_glGenRenderbuffers,
                              thread_data,
                              thread_mode);
}

/*
 * void
 * glGenTextures(GLsizei n, GLuint* textures);
 */

typedef struct
{
   GLsizei n;
   GLuint* textures;

} EVGL_API_Thread_Command_glGenTextures;

void (*orig_evgl_api_glGenTextures)(GLsizei n, GLuint* textures);

static void
_evgl_api_thread_glGenTextures(void *data)
{
   EVGL_API_Thread_Command_glGenTextures *thread_data =
      (EVGL_API_Thread_Command_glGenTextures *)data;

   orig_evgl_api_glGenTextures(thread_data->n,
                               thread_data->textures);

}

EAPI void
glGenTextures_evgl_api_thread_cmd(GLsizei n, GLuint* textures)
{
   if (!evas_evgl_thread_enabled() || (_main_thread_id != eina_thread_self()))
     {
        orig_evgl_api_glGenTextures(n, textures);
        return;
     }

   int thread_mode = EVAS_GL_THREAD_MODE_FINISH;

   EVGL_API_Thread_Command_glGenTextures thread_data_local;
   EVGL_API_Thread_Command_glGenTextures *thread_data = &thread_data_local;

   thread_data->n = n;
   thread_data->textures = textures;

   evas_gl_thread_cmd_enqueue(EVAS_GL_THREAD_TYPE_EVGL,
                              _evgl_api_thread_glGenTextures,
                              thread_data,
                              thread_mode);
}

/*
 * void
 * glGetActiveAttrib(GLuint program, GLuint index, GLsizei bufsize, GLsizei* length, GLint* size, GLenum* type, char* name);
 */

typedef struct
{
   GLuint program;
   GLuint index;
   GLsizei bufsize;
   GLsizei* length;
   GLint* size;
   GLenum* type;
   char* name;

} EVGL_API_Thread_Command_glGetActiveAttrib;

void (*orig_evgl_api_glGetActiveAttrib)(GLuint program, GLuint index, GLsizei bufsize, GLsizei* length, GLint* size, GLenum* type, char* name);

static void
_evgl_api_thread_glGetActiveAttrib(void *data)
{
   EVGL_API_Thread_Command_glGetActiveAttrib *thread_data =
      (EVGL_API_Thread_Command_glGetActiveAttrib *)data;

   orig_evgl_api_glGetActiveAttrib(thread_data->program,
                                   thread_data->index,
                                   thread_data->bufsize,
                                   thread_data->length,
                                   thread_data->size,
                                   thread_data->type,
                                   thread_data->name);

}

EAPI void
glGetActiveAttrib_evgl_api_thread_cmd(GLuint program, GLuint index, GLsizei bufsize, GLsizei* length, GLint* size, GLenum* type, char* name)
{
   if (!evas_evgl_thread_enabled() || (_main_thread_id != eina_thread_self()))
     {
        orig_evgl_api_glGetActiveAttrib(program, index, bufsize, length, size, type, name);
        return;
     }

   int thread_mode = EVAS_GL_THREAD_MODE_FINISH;

   EVGL_API_Thread_Command_glGetActiveAttrib thread_data_local;
   EVGL_API_Thread_Command_glGetActiveAttrib *thread_data = &thread_data_local;

   thread_data->program = program;
   thread_data->index = index;
   thread_data->bufsize = bufsize;
   thread_data->length = length;
   thread_data->size = size;
   thread_data->type = type;
   thread_data->name = name;

   evas_gl_thread_cmd_enqueue(EVAS_GL_THREAD_TYPE_EVGL,
                              _evgl_api_thread_glGetActiveAttrib,
                              thread_data,
                              thread_mode);
}

/*
 * void
 * glGetActiveUniform(GLuint program, GLuint index, GLsizei bufsize, GLsizei* length, GLint* size, GLenum* type, char* name);
 */

typedef struct
{
   GLuint program;
   GLuint index;
   GLsizei bufsize;
   GLsizei* length;
   GLint* size;
   GLenum* type;
   char* name;

} EVGL_API_Thread_Command_glGetActiveUniform;

void (*orig_evgl_api_glGetActiveUniform)(GLuint program, GLuint index, GLsizei bufsize, GLsizei* length, GLint* size, GLenum* type, char* name);

static void
_evgl_api_thread_glGetActiveUniform(void *data)
{
   EVGL_API_Thread_Command_glGetActiveUniform *thread_data =
      (EVGL_API_Thread_Command_glGetActiveUniform *)data;

   orig_evgl_api_glGetActiveUniform(thread_data->program,
                                    thread_data->index,
                                    thread_data->bufsize,
                                    thread_data->length,
                                    thread_data->size,
                                    thread_data->type,
                                    thread_data->name);

}

EAPI void
glGetActiveUniform_evgl_api_thread_cmd(GLuint program, GLuint index, GLsizei bufsize, GLsizei* length, GLint* size, GLenum* type, char* name)
{
   if (!evas_evgl_thread_enabled() || (_main_thread_id != eina_thread_self()))
     {
        orig_evgl_api_glGetActiveUniform(program, index, bufsize, length, size, type, name);
        return;
     }

   int thread_mode = EVAS_GL_THREAD_MODE_FINISH;

   EVGL_API_Thread_Command_glGetActiveUniform thread_data_local;
   EVGL_API_Thread_Command_glGetActiveUniform *thread_data = &thread_data_local;

   thread_data->program = program;
   thread_data->index = index;
   thread_data->bufsize = bufsize;
   thread_data->length = length;
   thread_data->size = size;
   thread_data->type = type;
   thread_data->name = name;

   evas_gl_thread_cmd_enqueue(EVAS_GL_THREAD_TYPE_EVGL,
                              _evgl_api_thread_glGetActiveUniform,
                              thread_data,
                              thread_mode);
}

/*
 * void
 * glGetAttachedShaders(GLuint program, GLsizei maxcount, GLsizei* count, GLuint* shaders);
 */

typedef struct
{
   GLuint program;
   GLsizei maxcount;
   GLsizei* count;
   GLuint* shaders;

} EVGL_API_Thread_Command_glGetAttachedShaders;

void (*orig_evgl_api_glGetAttachedShaders)(GLuint program, GLsizei maxcount, GLsizei* count, GLuint* shaders);

static void
_evgl_api_thread_glGetAttachedShaders(void *data)
{
   EVGL_API_Thread_Command_glGetAttachedShaders *thread_data =
      (EVGL_API_Thread_Command_glGetAttachedShaders *)data;

   orig_evgl_api_glGetAttachedShaders(thread_data->program,
                                      thread_data->maxcount,
                                      thread_data->count,
                                      thread_data->shaders);

}

EAPI void
glGetAttachedShaders_evgl_api_thread_cmd(GLuint program, GLsizei maxcount, GLsizei* count, GLuint* shaders)
{
   if (!evas_evgl_thread_enabled() || (_main_thread_id != eina_thread_self()))
     {
        orig_evgl_api_glGetAttachedShaders(program, maxcount, count, shaders);
        return;
     }

   int thread_mode = EVAS_GL_THREAD_MODE_FINISH;

   EVGL_API_Thread_Command_glGetAttachedShaders thread_data_local;
   EVGL_API_Thread_Command_glGetAttachedShaders *thread_data = &thread_data_local;

   thread_data->program = program;
   thread_data->maxcount = maxcount;
   thread_data->count = count;
   thread_data->shaders = shaders;

   evas_gl_thread_cmd_enqueue(EVAS_GL_THREAD_TYPE_EVGL,
                              _evgl_api_thread_glGetAttachedShaders,
                              thread_data,
                              thread_mode);
}

/*
 * int
 * glGetAttribLocation(GLuint program, const char* name);
 */

typedef struct
{
   int return_value;
   GLuint program;
   const char* name;

} EVGL_API_Thread_Command_glGetAttribLocation;

int (*orig_evgl_api_glGetAttribLocation)(GLuint program, const char* name);

static void
_evgl_api_thread_glGetAttribLocation(void *data)
{
   EVGL_API_Thread_Command_glGetAttribLocation *thread_data =
      (EVGL_API_Thread_Command_glGetAttribLocation *)data;

   thread_data->return_value = orig_evgl_api_glGetAttribLocation(thread_data->program,
                                                                 thread_data->name);

}

EAPI int
glGetAttribLocation_evgl_api_thread_cmd(GLuint program, const char* name)
{
   if (!evas_evgl_thread_enabled() || (_main_thread_id != eina_thread_self()))
     {
        return orig_evgl_api_glGetAttribLocation(program, name);
     }

   int thread_mode = EVAS_GL_THREAD_MODE_FINISH;

   EVGL_API_Thread_Command_glGetAttribLocation thread_data_local;
   EVGL_API_Thread_Command_glGetAttribLocation *thread_data = &thread_data_local;

   thread_data->program = program;
   thread_data->name = name;

   evas_gl_thread_cmd_enqueue(EVAS_GL_THREAD_TYPE_EVGL,
                              _evgl_api_thread_glGetAttribLocation,
                              thread_data,
                              thread_mode);

   return thread_data->return_value;
}

/*
 * void
 * glGetBooleanv(GLenum pname, GLboolean* params);
 */

typedef struct
{
   GLenum pname;
   GLboolean* params;

} EVGL_API_Thread_Command_glGetBooleanv;

void (*orig_evgl_api_glGetBooleanv)(GLenum pname, GLboolean* params);

static void
_evgl_api_thread_glGetBooleanv(void *data)
{
   EVGL_API_Thread_Command_glGetBooleanv *thread_data =
      (EVGL_API_Thread_Command_glGetBooleanv *)data;

   orig_evgl_api_glGetBooleanv(thread_data->pname,
                               thread_data->params);

}

EAPI void
glGetBooleanv_evgl_api_thread_cmd(GLenum pname, GLboolean* params)
{
   if (!evas_evgl_thread_enabled() || (_main_thread_id != eina_thread_self()))
     {
        orig_evgl_api_glGetBooleanv(pname, params);
        return;
     }

   int thread_mode = EVAS_GL_THREAD_MODE_FINISH;

   EVGL_API_Thread_Command_glGetBooleanv thread_data_local;
   EVGL_API_Thread_Command_glGetBooleanv *thread_data = &thread_data_local;

   thread_data->pname = pname;
   thread_data->params = params;

   evas_gl_thread_cmd_enqueue(EVAS_GL_THREAD_TYPE_EVGL,
                              _evgl_api_thread_glGetBooleanv,
                              thread_data,
                              thread_mode);
}

/*
 * void
 * glGetBufferParameteriv(GLenum target, GLenum pname, GLint* params);
 */

typedef struct
{
   GLenum target;
   GLenum pname;
   GLint* params;

} EVGL_API_Thread_Command_glGetBufferParameteriv;

void (*orig_evgl_api_glGetBufferParameteriv)(GLenum target, GLenum pname, GLint* params);

static void
_evgl_api_thread_glGetBufferParameteriv(void *data)
{
   EVGL_API_Thread_Command_glGetBufferParameteriv *thread_data =
      (EVGL_API_Thread_Command_glGetBufferParameteriv *)data;

   orig_evgl_api_glGetBufferParameteriv(thread_data->target,
                                        thread_data->pname,
                                        thread_data->params);

}

EAPI void
glGetBufferParameteriv_evgl_api_thread_cmd(GLenum target, GLenum pname, GLint* params)
{
   if (!evas_evgl_thread_enabled() || (_main_thread_id != eina_thread_self()))
     {
        orig_evgl_api_glGetBufferParameteriv(target, pname, params);
        return;
     }

   int thread_mode = EVAS_GL_THREAD_MODE_FINISH;

   EVGL_API_Thread_Command_glGetBufferParameteriv thread_data_local;
   EVGL_API_Thread_Command_glGetBufferParameteriv *thread_data = &thread_data_local;

   thread_data->target = target;
   thread_data->pname = pname;
   thread_data->params = params;

   evas_gl_thread_cmd_enqueue(EVAS_GL_THREAD_TYPE_EVGL,
                              _evgl_api_thread_glGetBufferParameteriv,
                              thread_data,
                              thread_mode);
}

/*
 * GLenum
 * glGetError(void);
 */

typedef struct
{
   GLenum return_value;

} EVGL_API_Thread_Command_glGetError;

GLenum (*orig_evgl_api_glGetError)(void);

static void
_evgl_api_thread_glGetError(void *data)
{
   EVGL_API_Thread_Command_glGetError *thread_data =
      (EVGL_API_Thread_Command_glGetError *)data;

   thread_data->return_value = orig_evgl_api_glGetError();

}

EAPI GLenum
glGetError_evgl_api_thread_cmd(void)
{
   if (!evas_evgl_thread_enabled() || (_main_thread_id != eina_thread_self()))
     {
        return orig_evgl_api_glGetError();
     }

   int thread_mode = EVAS_GL_THREAD_MODE_FINISH;

   EVGL_API_Thread_Command_glGetError thread_data_local;
   EVGL_API_Thread_Command_glGetError *thread_data = &thread_data_local;


   evas_gl_thread_cmd_enqueue(EVAS_GL_THREAD_TYPE_EVGL,
                              _evgl_api_thread_glGetError,
                              thread_data,
                              thread_mode);

   return thread_data->return_value;
}

/*
 * void
 * glGetFloatv(GLenum pname, GLfloat* params);
 */

typedef struct
{
   GLenum pname;
   GLfloat* params;

} EVGL_API_Thread_Command_glGetFloatv;

void (*orig_evgl_api_glGetFloatv)(GLenum pname, GLfloat* params);

static void
_evgl_api_thread_glGetFloatv(void *data)
{
   EVGL_API_Thread_Command_glGetFloatv *thread_data =
      (EVGL_API_Thread_Command_glGetFloatv *)data;

   orig_evgl_api_glGetFloatv(thread_data->pname,
                             thread_data->params);

}

EAPI void
glGetFloatv_evgl_api_thread_cmd(GLenum pname, GLfloat* params)
{
   if (!evas_evgl_thread_enabled() || (_main_thread_id != eina_thread_self()))
     {
        orig_evgl_api_glGetFloatv(pname, params);
        return;
     }

   int thread_mode = EVAS_GL_THREAD_MODE_FINISH;

   EVGL_API_Thread_Command_glGetFloatv thread_data_local;
   EVGL_API_Thread_Command_glGetFloatv *thread_data = &thread_data_local;

   thread_data->pname = pname;
   thread_data->params = params;

   evas_gl_thread_cmd_enqueue(EVAS_GL_THREAD_TYPE_EVGL,
                              _evgl_api_thread_glGetFloatv,
                              thread_data,
                              thread_mode);
}

/*
 * void
 * glGetFramebufferAttachmentParameteriv(GLenum target, GLenum attachment, GLenum pname, GLint* params);
 */

typedef struct
{
   GLenum target;
   GLenum attachment;
   GLenum pname;
   GLint* params;

} EVGL_API_Thread_Command_glGetFramebufferAttachmentParameteriv;

void (*orig_evgl_api_glGetFramebufferAttachmentParameteriv)(GLenum target, GLenum attachment, GLenum pname, GLint* params);

static void
_evgl_api_thread_glGetFramebufferAttachmentParameteriv(void *data)
{
   EVGL_API_Thread_Command_glGetFramebufferAttachmentParameteriv *thread_data =
      (EVGL_API_Thread_Command_glGetFramebufferAttachmentParameteriv *)data;

   orig_evgl_api_glGetFramebufferAttachmentParameteriv(thread_data->target,
                                                       thread_data->attachment,
                                                       thread_data->pname,
                                                       thread_data->params);

}

EAPI void
glGetFramebufferAttachmentParameteriv_evgl_api_thread_cmd(GLenum target, GLenum attachment, GLenum pname, GLint* params)
{
   if (!evas_evgl_thread_enabled() || (_main_thread_id != eina_thread_self()))
     {
        orig_evgl_api_glGetFramebufferAttachmentParameteriv(target, attachment, pname, params);
        return;
     }

   int thread_mode = EVAS_GL_THREAD_MODE_FINISH;

   EVGL_API_Thread_Command_glGetFramebufferAttachmentParameteriv thread_data_local;
   EVGL_API_Thread_Command_glGetFramebufferAttachmentParameteriv *thread_data = &thread_data_local;

   thread_data->target = target;
   thread_data->attachment = attachment;
   thread_data->pname = pname;
   thread_data->params = params;

   evas_gl_thread_cmd_enqueue(EVAS_GL_THREAD_TYPE_EVGL,
                              _evgl_api_thread_glGetFramebufferAttachmentParameteriv,
                              thread_data,
                              thread_mode);
}

/*
 * void
 * glGetIntegerv(GLenum pname, GLint* params);
 */

typedef struct
{
   GLenum pname;
   GLint* params;

} EVGL_API_Thread_Command_glGetIntegerv;

void (*orig_evgl_api_glGetIntegerv)(GLenum pname, GLint* params);

static void
_evgl_api_thread_glGetIntegerv(void *data)
{
   EVGL_API_Thread_Command_glGetIntegerv *thread_data =
      (EVGL_API_Thread_Command_glGetIntegerv *)data;

   orig_evgl_api_glGetIntegerv(thread_data->pname,
                               thread_data->params);

}

EAPI void
glGetIntegerv_evgl_api_thread_cmd(GLenum pname, GLint* params)
{
   if (!evas_evgl_thread_enabled() || (_main_thread_id != eina_thread_self()))
     {
        orig_evgl_api_glGetIntegerv(pname, params);
        return;
     }

   int thread_mode = EVAS_GL_THREAD_MODE_FINISH;

   EVGL_API_Thread_Command_glGetIntegerv thread_data_local;
   EVGL_API_Thread_Command_glGetIntegerv *thread_data = &thread_data_local;

   thread_data->pname = pname;
   thread_data->params = params;

   evas_gl_thread_cmd_enqueue(EVAS_GL_THREAD_TYPE_EVGL,
                              _evgl_api_thread_glGetIntegerv,
                              thread_data,
                              thread_mode);
}

/*
 * void
 * glGetProgramiv(GLuint program, GLenum pname, GLint* params);
 */

typedef struct
{
   GLuint program;
   GLenum pname;
   GLint* params;

} EVGL_API_Thread_Command_glGetProgramiv;

void (*orig_evgl_api_glGetProgramiv)(GLuint program, GLenum pname, GLint* params);

static void
_evgl_api_thread_glGetProgramiv(void *data)
{
   EVGL_API_Thread_Command_glGetProgramiv *thread_data =
      (EVGL_API_Thread_Command_glGetProgramiv *)data;

   orig_evgl_api_glGetProgramiv(thread_data->program,
                                thread_data->pname,
                                thread_data->params);

}

EAPI void
glGetProgramiv_evgl_api_thread_cmd(GLuint program, GLenum pname, GLint* params)
{
   if (!evas_evgl_thread_enabled() || (_main_thread_id != eina_thread_self()))
     {
        orig_evgl_api_glGetProgramiv(program, pname, params);
        return;
     }

   int thread_mode = EVAS_GL_THREAD_MODE_FINISH;

   EVGL_API_Thread_Command_glGetProgramiv thread_data_local;
   EVGL_API_Thread_Command_glGetProgramiv *thread_data = &thread_data_local;

   thread_data->program = program;
   thread_data->pname = pname;
   thread_data->params = params;

   evas_gl_thread_cmd_enqueue(EVAS_GL_THREAD_TYPE_EVGL,
                              _evgl_api_thread_glGetProgramiv,
                              thread_data,
                              thread_mode);
}

/*
 * void
 * glGetProgramInfoLog(GLuint program, GLsizei bufsize, GLsizei* length, char* infolog);
 */

typedef struct
{
   GLuint program;
   GLsizei bufsize;
   GLsizei* length;
   char* infolog;

} EVGL_API_Thread_Command_glGetProgramInfoLog;

void (*orig_evgl_api_glGetProgramInfoLog)(GLuint program, GLsizei bufsize, GLsizei* length, char* infolog);

static void
_evgl_api_thread_glGetProgramInfoLog(void *data)
{
   EVGL_API_Thread_Command_glGetProgramInfoLog *thread_data =
      (EVGL_API_Thread_Command_glGetProgramInfoLog *)data;

   orig_evgl_api_glGetProgramInfoLog(thread_data->program,
                                     thread_data->bufsize,
                                     thread_data->length,
                                     thread_data->infolog);

}

EAPI void
glGetProgramInfoLog_evgl_api_thread_cmd(GLuint program, GLsizei bufsize, GLsizei* length, char* infolog)
{
   if (!evas_evgl_thread_enabled() || (_main_thread_id != eina_thread_self()))
     {
        orig_evgl_api_glGetProgramInfoLog(program, bufsize, length, infolog);
        return;
     }

   int thread_mode = EVAS_GL_THREAD_MODE_FINISH;

   EVGL_API_Thread_Command_glGetProgramInfoLog thread_data_local;
   EVGL_API_Thread_Command_glGetProgramInfoLog *thread_data = &thread_data_local;

   thread_data->program = program;
   thread_data->bufsize = bufsize;
   thread_data->length = length;
   thread_data->infolog = infolog;

   evas_gl_thread_cmd_enqueue(EVAS_GL_THREAD_TYPE_EVGL,
                              _evgl_api_thread_glGetProgramInfoLog,
                              thread_data,
                              thread_mode);
}

/*
 * void
 * glGetRenderbufferParameteriv(GLenum target, GLenum pname, GLint* params);
 */

typedef struct
{
   GLenum target;
   GLenum pname;
   GLint* params;

} EVGL_API_Thread_Command_glGetRenderbufferParameteriv;

void (*orig_evgl_api_glGetRenderbufferParameteriv)(GLenum target, GLenum pname, GLint* params);

static void
_evgl_api_thread_glGetRenderbufferParameteriv(void *data)
{
   EVGL_API_Thread_Command_glGetRenderbufferParameteriv *thread_data =
      (EVGL_API_Thread_Command_glGetRenderbufferParameteriv *)data;

   orig_evgl_api_glGetRenderbufferParameteriv(thread_data->target,
                                              thread_data->pname,
                                              thread_data->params);

}

EAPI void
glGetRenderbufferParameteriv_evgl_api_thread_cmd(GLenum target, GLenum pname, GLint* params)
{
   if (!evas_evgl_thread_enabled() || (_main_thread_id != eina_thread_self()))
     {
        orig_evgl_api_glGetRenderbufferParameteriv(target, pname, params);
        return;
     }

   int thread_mode = EVAS_GL_THREAD_MODE_FINISH;

   EVGL_API_Thread_Command_glGetRenderbufferParameteriv thread_data_local;
   EVGL_API_Thread_Command_glGetRenderbufferParameteriv *thread_data = &thread_data_local;

   thread_data->target = target;
   thread_data->pname = pname;
   thread_data->params = params;

   evas_gl_thread_cmd_enqueue(EVAS_GL_THREAD_TYPE_EVGL,
                              _evgl_api_thread_glGetRenderbufferParameteriv,
                              thread_data,
                              thread_mode);
}

/*
 * void
 * glGetShaderiv(GLuint shader, GLenum pname, GLint* params);
 */

typedef struct
{
   GLuint shader;
   GLenum pname;
   GLint* params;

} EVGL_API_Thread_Command_glGetShaderiv;

void (*orig_evgl_api_glGetShaderiv)(GLuint shader, GLenum pname, GLint* params);

static void
_evgl_api_thread_glGetShaderiv(void *data)
{
   EVGL_API_Thread_Command_glGetShaderiv *thread_data =
      (EVGL_API_Thread_Command_glGetShaderiv *)data;

   orig_evgl_api_glGetShaderiv(thread_data->shader,
                               thread_data->pname,
                               thread_data->params);

}

EAPI void
glGetShaderiv_evgl_api_thread_cmd(GLuint shader, GLenum pname, GLint* params)
{
   if (!evas_evgl_thread_enabled() || (_main_thread_id != eina_thread_self()))
     {
        orig_evgl_api_glGetShaderiv(shader, pname, params);
        return;
     }

   int thread_mode = EVAS_GL_THREAD_MODE_FINISH;

   EVGL_API_Thread_Command_glGetShaderiv thread_data_local;
   EVGL_API_Thread_Command_glGetShaderiv *thread_data = &thread_data_local;

   thread_data->shader = shader;
   thread_data->pname = pname;
   thread_data->params = params;

   evas_gl_thread_cmd_enqueue(EVAS_GL_THREAD_TYPE_EVGL,
                              _evgl_api_thread_glGetShaderiv,
                              thread_data,
                              thread_mode);
}

/*
 * void
 * glGetShaderInfoLog(GLuint shader, GLsizei bufsize, GLsizei* length, char* infolog);
 */

typedef struct
{
   GLuint shader;
   GLsizei bufsize;
   GLsizei* length;
   char* infolog;

} EVGL_API_Thread_Command_glGetShaderInfoLog;

void (*orig_evgl_api_glGetShaderInfoLog)(GLuint shader, GLsizei bufsize, GLsizei* length, char* infolog);

static void
_evgl_api_thread_glGetShaderInfoLog(void *data)
{
   EVGL_API_Thread_Command_glGetShaderInfoLog *thread_data =
      (EVGL_API_Thread_Command_glGetShaderInfoLog *)data;

   orig_evgl_api_glGetShaderInfoLog(thread_data->shader,
                                    thread_data->bufsize,
                                    thread_data->length,
                                    thread_data->infolog);

}

EAPI void
glGetShaderInfoLog_evgl_api_thread_cmd(GLuint shader, GLsizei bufsize, GLsizei* length, char* infolog)
{
   if (!evas_evgl_thread_enabled() || (_main_thread_id != eina_thread_self()))
     {
        orig_evgl_api_glGetShaderInfoLog(shader, bufsize, length, infolog);
        return;
     }

   int thread_mode = EVAS_GL_THREAD_MODE_FINISH;

   EVGL_API_Thread_Command_glGetShaderInfoLog thread_data_local;
   EVGL_API_Thread_Command_glGetShaderInfoLog *thread_data = &thread_data_local;

   thread_data->shader = shader;
   thread_data->bufsize = bufsize;
   thread_data->length = length;
   thread_data->infolog = infolog;

   evas_gl_thread_cmd_enqueue(EVAS_GL_THREAD_TYPE_EVGL,
                              _evgl_api_thread_glGetShaderInfoLog,
                              thread_data,
                              thread_mode);
}

/*
 * void
 * glGetShaderPrecisionFormat(GLenum shadertype, GLenum precisiontype, GLint* range, GLint* precision);
 */

typedef struct
{
   GLenum shadertype;
   GLenum precisiontype;
   GLint* range;
   GLint* precision;

} EVGL_API_Thread_Command_glGetShaderPrecisionFormat;

void (*orig_evgl_api_glGetShaderPrecisionFormat)(GLenum shadertype, GLenum precisiontype, GLint* range, GLint* precision);

static void
_evgl_api_thread_glGetShaderPrecisionFormat(void *data)
{
   EVGL_API_Thread_Command_glGetShaderPrecisionFormat *thread_data =
      (EVGL_API_Thread_Command_glGetShaderPrecisionFormat *)data;

   orig_evgl_api_glGetShaderPrecisionFormat(thread_data->shadertype,
                                            thread_data->precisiontype,
                                            thread_data->range,
                                            thread_data->precision);

}

EAPI void
glGetShaderPrecisionFormat_evgl_api_thread_cmd(GLenum shadertype, GLenum precisiontype, GLint* range, GLint* precision)
{
   if (!evas_evgl_thread_enabled() || (_main_thread_id != eina_thread_self()))
     {
        orig_evgl_api_glGetShaderPrecisionFormat(shadertype, precisiontype, range, precision);
        return;
     }

   int thread_mode = EVAS_GL_THREAD_MODE_FINISH;

   EVGL_API_Thread_Command_glGetShaderPrecisionFormat thread_data_local;
   EVGL_API_Thread_Command_glGetShaderPrecisionFormat *thread_data = &thread_data_local;

   thread_data->shadertype = shadertype;
   thread_data->precisiontype = precisiontype;
   thread_data->range = range;
   thread_data->precision = precision;

   evas_gl_thread_cmd_enqueue(EVAS_GL_THREAD_TYPE_EVGL,
                              _evgl_api_thread_glGetShaderPrecisionFormat,
                              thread_data,
                              thread_mode);
}

/*
 * void
 * glGetShaderSource(GLuint shader, GLsizei bufsize, GLsizei* length, char* source);
 */

typedef struct
{
   GLuint shader;
   GLsizei bufsize;
   GLsizei* length;
   char* source;

} EVGL_API_Thread_Command_glGetShaderSource;

void (*orig_evgl_api_glGetShaderSource)(GLuint shader, GLsizei bufsize, GLsizei* length, char* source);

static void
_evgl_api_thread_glGetShaderSource(void *data)
{
   EVGL_API_Thread_Command_glGetShaderSource *thread_data =
      (EVGL_API_Thread_Command_glGetShaderSource *)data;

   orig_evgl_api_glGetShaderSource(thread_data->shader,
                                   thread_data->bufsize,
                                   thread_data->length,
                                   thread_data->source);

}

EAPI void
glGetShaderSource_evgl_api_thread_cmd(GLuint shader, GLsizei bufsize, GLsizei* length, char* source)
{
   if (!evas_evgl_thread_enabled() || (_main_thread_id != eina_thread_self()))
     {
        orig_evgl_api_glGetShaderSource(shader, bufsize, length, source);
        return;
     }

   int thread_mode = EVAS_GL_THREAD_MODE_FINISH;

   EVGL_API_Thread_Command_glGetShaderSource thread_data_local;
   EVGL_API_Thread_Command_glGetShaderSource *thread_data = &thread_data_local;

   thread_data->shader = shader;
   thread_data->bufsize = bufsize;
   thread_data->length = length;
   thread_data->source = source;

   evas_gl_thread_cmd_enqueue(EVAS_GL_THREAD_TYPE_EVGL,
                              _evgl_api_thread_glGetShaderSource,
                              thread_data,
                              thread_mode);
}

/*
 * const GLubyte*
 * glGetString(GLenum name);
 */

typedef struct
{
   const GLubyte* return_value;
   GLenum name;

} EVGL_API_Thread_Command_glGetString;

const GLubyte* (*orig_evgl_api_glGetString)(GLenum name);

static void
_evgl_api_thread_glGetString(void *data)
{
   EVGL_API_Thread_Command_glGetString *thread_data =
      (EVGL_API_Thread_Command_glGetString *)data;

   thread_data->return_value = orig_evgl_api_glGetString(thread_data->name);

}

EAPI const GLubyte*
glGetString_evgl_api_thread_cmd(GLenum name)
{
   if (!evas_evgl_thread_enabled() || (_main_thread_id != eina_thread_self()))
     {
        return orig_evgl_api_glGetString(name);
     }

   int thread_mode = EVAS_GL_THREAD_MODE_FINISH;

   EVGL_API_Thread_Command_glGetString thread_data_local;
   EVGL_API_Thread_Command_glGetString *thread_data = &thread_data_local;

   thread_data->name = name;

   evas_gl_thread_cmd_enqueue(EVAS_GL_THREAD_TYPE_EVGL,
                              _evgl_api_thread_glGetString,
                              thread_data,
                              thread_mode);

   return thread_data->return_value;
}

/*
 * void
 * glGetTexParameterfv(GLenum target, GLenum pname, GLfloat* params);
 */

typedef struct
{
   GLenum target;
   GLenum pname;
   GLfloat* params;

} EVGL_API_Thread_Command_glGetTexParameterfv;

void (*orig_evgl_api_glGetTexParameterfv)(GLenum target, GLenum pname, GLfloat* params);

static void
_evgl_api_thread_glGetTexParameterfv(void *data)
{
   EVGL_API_Thread_Command_glGetTexParameterfv *thread_data =
      (EVGL_API_Thread_Command_glGetTexParameterfv *)data;

   orig_evgl_api_glGetTexParameterfv(thread_data->target,
                                     thread_data->pname,
                                     thread_data->params);

}

EAPI void
glGetTexParameterfv_evgl_api_thread_cmd(GLenum target, GLenum pname, GLfloat* params)
{
   if (!evas_evgl_thread_enabled() || (_main_thread_id != eina_thread_self()))
     {
        orig_evgl_api_glGetTexParameterfv(target, pname, params);
        return;
     }

   int thread_mode = EVAS_GL_THREAD_MODE_FINISH;

   EVGL_API_Thread_Command_glGetTexParameterfv thread_data_local;
   EVGL_API_Thread_Command_glGetTexParameterfv *thread_data = &thread_data_local;

   thread_data->target = target;
   thread_data->pname = pname;
   thread_data->params = params;

   evas_gl_thread_cmd_enqueue(EVAS_GL_THREAD_TYPE_EVGL,
                              _evgl_api_thread_glGetTexParameterfv,
                              thread_data,
                              thread_mode);
}

/*
 * void
 * glGetTexParameteriv(GLenum target, GLenum pname, GLint* params);
 */

typedef struct
{
   GLenum target;
   GLenum pname;
   GLint* params;

} EVGL_API_Thread_Command_glGetTexParameteriv;

void (*orig_evgl_api_glGetTexParameteriv)(GLenum target, GLenum pname, GLint* params);

static void
_evgl_api_thread_glGetTexParameteriv(void *data)
{
   EVGL_API_Thread_Command_glGetTexParameteriv *thread_data =
      (EVGL_API_Thread_Command_glGetTexParameteriv *)data;

   orig_evgl_api_glGetTexParameteriv(thread_data->target,
                                     thread_data->pname,
                                     thread_data->params);

}

EAPI void
glGetTexParameteriv_evgl_api_thread_cmd(GLenum target, GLenum pname, GLint* params)
{
   if (!evas_evgl_thread_enabled() || (_main_thread_id != eina_thread_self()))
     {
        orig_evgl_api_glGetTexParameteriv(target, pname, params);
        return;
     }

   int thread_mode = EVAS_GL_THREAD_MODE_FINISH;

   EVGL_API_Thread_Command_glGetTexParameteriv thread_data_local;
   EVGL_API_Thread_Command_glGetTexParameteriv *thread_data = &thread_data_local;

   thread_data->target = target;
   thread_data->pname = pname;
   thread_data->params = params;

   evas_gl_thread_cmd_enqueue(EVAS_GL_THREAD_TYPE_EVGL,
                              _evgl_api_thread_glGetTexParameteriv,
                              thread_data,
                              thread_mode);
}

/*
 * void
 * glGetUniformfv(GLuint program, GLint location, GLfloat* params);
 */

typedef struct
{
   GLuint program;
   GLint location;
   GLfloat* params;

} EVGL_API_Thread_Command_glGetUniformfv;

void (*orig_evgl_api_glGetUniformfv)(GLuint program, GLint location, GLfloat* params);

static void
_evgl_api_thread_glGetUniformfv(void *data)
{
   EVGL_API_Thread_Command_glGetUniformfv *thread_data =
      (EVGL_API_Thread_Command_glGetUniformfv *)data;

   orig_evgl_api_glGetUniformfv(thread_data->program,
                                thread_data->location,
                                thread_data->params);

}

EAPI void
glGetUniformfv_evgl_api_thread_cmd(GLuint program, GLint location, GLfloat* params)
{
   if (!evas_evgl_thread_enabled() || (_main_thread_id != eina_thread_self()))
     {
        orig_evgl_api_glGetUniformfv(program, location, params);
        return;
     }

   int thread_mode = EVAS_GL_THREAD_MODE_FINISH;

   EVGL_API_Thread_Command_glGetUniformfv thread_data_local;
   EVGL_API_Thread_Command_glGetUniformfv *thread_data = &thread_data_local;

   thread_data->program = program;
   thread_data->location = location;
   thread_data->params = params;

   evas_gl_thread_cmd_enqueue(EVAS_GL_THREAD_TYPE_EVGL,
                              _evgl_api_thread_glGetUniformfv,
                              thread_data,
                              thread_mode);
}

/*
 * void
 * glGetUniformiv(GLuint program, GLint location, GLint* params);
 */

typedef struct
{
   GLuint program;
   GLint location;
   GLint* params;

} EVGL_API_Thread_Command_glGetUniformiv;

void (*orig_evgl_api_glGetUniformiv)(GLuint program, GLint location, GLint* params);

static void
_evgl_api_thread_glGetUniformiv(void *data)
{
   EVGL_API_Thread_Command_glGetUniformiv *thread_data =
      (EVGL_API_Thread_Command_glGetUniformiv *)data;

   orig_evgl_api_glGetUniformiv(thread_data->program,
                                thread_data->location,
                                thread_data->params);

}

EAPI void
glGetUniformiv_evgl_api_thread_cmd(GLuint program, GLint location, GLint* params)
{
   if (!evas_evgl_thread_enabled() || (_main_thread_id != eina_thread_self()))
     {
        orig_evgl_api_glGetUniformiv(program, location, params);
        return;
     }

   int thread_mode = EVAS_GL_THREAD_MODE_FINISH;

   EVGL_API_Thread_Command_glGetUniformiv thread_data_local;
   EVGL_API_Thread_Command_glGetUniformiv *thread_data = &thread_data_local;

   thread_data->program = program;
   thread_data->location = location;
   thread_data->params = params;

   evas_gl_thread_cmd_enqueue(EVAS_GL_THREAD_TYPE_EVGL,
                              _evgl_api_thread_glGetUniformiv,
                              thread_data,
                              thread_mode);
}

/*
 * int
 * glGetUniformLocation(GLuint program, const char* name);
 */

typedef struct
{
   int return_value;
   GLuint program;
   const char* name;

} EVGL_API_Thread_Command_glGetUniformLocation;

int (*orig_evgl_api_glGetUniformLocation)(GLuint program, const char* name);

static void
_evgl_api_thread_glGetUniformLocation(void *data)
{
   EVGL_API_Thread_Command_glGetUniformLocation *thread_data =
      (EVGL_API_Thread_Command_glGetUniformLocation *)data;

   thread_data->return_value = orig_evgl_api_glGetUniformLocation(thread_data->program,
                                                                  thread_data->name);

}

EAPI int
glGetUniformLocation_evgl_api_thread_cmd(GLuint program, const char* name)
{
   if (!evas_evgl_thread_enabled() || (_main_thread_id != eina_thread_self()))
     {
        return orig_evgl_api_glGetUniformLocation(program, name);
     }

   int thread_mode = EVAS_GL_THREAD_MODE_FINISH;

   EVGL_API_Thread_Command_glGetUniformLocation thread_data_local;
   EVGL_API_Thread_Command_glGetUniformLocation *thread_data = &thread_data_local;

   thread_data->program = program;
   thread_data->name = name;

   evas_gl_thread_cmd_enqueue(EVAS_GL_THREAD_TYPE_EVGL,
                              _evgl_api_thread_glGetUniformLocation,
                              thread_data,
                              thread_mode);

   return thread_data->return_value;
}

/*
 * void
 * glGetVertexAttribfv(GLuint index, GLenum pname, GLfloat* params);
 */

typedef struct
{
   GLuint index;
   GLenum pname;
   GLfloat* params;

} EVGL_API_Thread_Command_glGetVertexAttribfv;

void (*orig_evgl_api_glGetVertexAttribfv)(GLuint index, GLenum pname, GLfloat* params);

static void
_evgl_api_thread_glGetVertexAttribfv(void *data)
{
   EVGL_API_Thread_Command_glGetVertexAttribfv *thread_data =
      (EVGL_API_Thread_Command_glGetVertexAttribfv *)data;

   orig_evgl_api_glGetVertexAttribfv(thread_data->index,
                                     thread_data->pname,
                                     thread_data->params);

}

EAPI void
glGetVertexAttribfv_evgl_api_thread_cmd(GLuint index, GLenum pname, GLfloat* params)
{
   if (!evas_evgl_thread_enabled() || (_main_thread_id != eina_thread_self()))
     {
        orig_evgl_api_glGetVertexAttribfv(index, pname, params);
        return;
     }

   int thread_mode = EVAS_GL_THREAD_MODE_FINISH;

   EVGL_API_Thread_Command_glGetVertexAttribfv thread_data_local;
   EVGL_API_Thread_Command_glGetVertexAttribfv *thread_data = &thread_data_local;

   thread_data->index = index;
   thread_data->pname = pname;
   thread_data->params = params;

   evas_gl_thread_cmd_enqueue(EVAS_GL_THREAD_TYPE_EVGL,
                              _evgl_api_thread_glGetVertexAttribfv,
                              thread_data,
                              thread_mode);
}

/*
 * void
 * glGetVertexAttribiv(GLuint index, GLenum pname, GLint* params);
 */

typedef struct
{
   GLuint index;
   GLenum pname;
   GLint* params;

} EVGL_API_Thread_Command_glGetVertexAttribiv;

void (*orig_evgl_api_glGetVertexAttribiv)(GLuint index, GLenum pname, GLint* params);

static void
_evgl_api_thread_glGetVertexAttribiv(void *data)
{
   EVGL_API_Thread_Command_glGetVertexAttribiv *thread_data =
      (EVGL_API_Thread_Command_glGetVertexAttribiv *)data;

   orig_evgl_api_glGetVertexAttribiv(thread_data->index,
                                     thread_data->pname,
                                     thread_data->params);

}

EAPI void
glGetVertexAttribiv_evgl_api_thread_cmd(GLuint index, GLenum pname, GLint* params)
{
   if (!evas_evgl_thread_enabled() || (_main_thread_id != eina_thread_self()))
     {
        orig_evgl_api_glGetVertexAttribiv(index, pname, params);
        return;
     }

   int thread_mode = EVAS_GL_THREAD_MODE_FINISH;

   EVGL_API_Thread_Command_glGetVertexAttribiv thread_data_local;
   EVGL_API_Thread_Command_glGetVertexAttribiv *thread_data = &thread_data_local;

   thread_data->index = index;
   thread_data->pname = pname;
   thread_data->params = params;

   evas_gl_thread_cmd_enqueue(EVAS_GL_THREAD_TYPE_EVGL,
                              _evgl_api_thread_glGetVertexAttribiv,
                              thread_data,
                              thread_mode);
}

/*
 * void
 * glGetVertexAttribPointerv(GLuint index, GLenum pname, void** pointer);
 */

typedef struct
{
   GLuint index;
   GLenum pname;
   void** pointer;

} EVGL_API_Thread_Command_glGetVertexAttribPointerv;

void (*orig_evgl_api_glGetVertexAttribPointerv)(GLuint index, GLenum pname, void** pointer);

static void
_evgl_api_thread_glGetVertexAttribPointerv(void *data)
{
   EVGL_API_Thread_Command_glGetVertexAttribPointerv *thread_data =
      (EVGL_API_Thread_Command_glGetVertexAttribPointerv *)data;

   orig_evgl_api_glGetVertexAttribPointerv(thread_data->index,
                                           thread_data->pname,
                                           thread_data->pointer);

}

EAPI void
glGetVertexAttribPointerv_evgl_api_thread_cmd(GLuint index, GLenum pname, void** pointer)
{
   if (!evas_evgl_thread_enabled() || (_main_thread_id != eina_thread_self()))
     {
        orig_evgl_api_glGetVertexAttribPointerv(index, pname, pointer);
        return;
     }

   int thread_mode = EVAS_GL_THREAD_MODE_FINISH;

   EVGL_API_Thread_Command_glGetVertexAttribPointerv thread_data_local;
   EVGL_API_Thread_Command_glGetVertexAttribPointerv *thread_data = &thread_data_local;

   thread_data->index = index;
   thread_data->pname = pname;
   thread_data->pointer = pointer;

   evas_gl_thread_cmd_enqueue(EVAS_GL_THREAD_TYPE_EVGL,
                              _evgl_api_thread_glGetVertexAttribPointerv,
                              thread_data,
                              thread_mode);
}

/*
 * void
 * glHint(GLenum target, GLenum mode);
 */

typedef struct
{
   GLenum target;
   GLenum mode;
   int command_allocated;

} EVGL_API_Thread_Command_glHint;

void (*orig_evgl_api_glHint)(GLenum target, GLenum mode);

static void
_evgl_api_thread_glHint(void *data)
{
   EVGL_API_Thread_Command_glHint *thread_data =
      (EVGL_API_Thread_Command_glHint *)data;

   orig_evgl_api_glHint(thread_data->target,
                        thread_data->mode);

   if (thread_data->command_allocated)
     eina_mempool_free(_mp_command, thread_data);
}

EAPI void
glHint_evgl_api_thread_cmd(GLenum target, GLenum mode)
{
   if (!evas_evgl_thread_enabled() || (_main_thread_id != eina_thread_self()))
     {
        orig_evgl_api_glHint(target, mode);
        return;
     }

   int thread_mode = EVAS_GL_THREAD_MODE_FINISH;

   EVGL_API_Thread_Command_glHint thread_data_local;
   EVGL_API_Thread_Command_glHint *thread_data = &thread_data_local;

   /* command_allocated flag init. */
   thread_data->command_allocated = 0;

   if (!evas_gl_thread_force_finish())
     { /* _flush */
        EVGL_API_Thread_Command_glHint *thread_data_new;
        thread_data_new = eina_mempool_malloc(_mp_command,
                                              sizeof(EVGL_API_Thread_Command_glHint));
        if (thread_data_new)
          {
             thread_data = thread_data_new;
             thread_data->command_allocated = 1;
             thread_mode = EVAS_GL_THREAD_MODE_FLUSH;
          }
     }

   thread_data->target = target;
   thread_data->mode = mode;

   evas_gl_thread_cmd_enqueue(EVAS_GL_THREAD_TYPE_EVGL,
                              _evgl_api_thread_glHint,
                              thread_data,
                              thread_mode);
}

/*
 * GLboolean
 * glIsBuffer(GLuint buffer);
 */

typedef struct
{
   GLboolean return_value;
   GLuint buffer;

} EVGL_API_Thread_Command_glIsBuffer;

GLboolean (*orig_evgl_api_glIsBuffer)(GLuint buffer);

static void
_evgl_api_thread_glIsBuffer(void *data)
{
   EVGL_API_Thread_Command_glIsBuffer *thread_data =
      (EVGL_API_Thread_Command_glIsBuffer *)data;

   thread_data->return_value = orig_evgl_api_glIsBuffer(thread_data->buffer);

}

EAPI GLboolean
glIsBuffer_evgl_api_thread_cmd(GLuint buffer)
{
   if (!evas_evgl_thread_enabled() || (_main_thread_id != eina_thread_self()))
     {
        return orig_evgl_api_glIsBuffer(buffer);
     }

   int thread_mode = EVAS_GL_THREAD_MODE_FINISH;

   EVGL_API_Thread_Command_glIsBuffer thread_data_local;
   EVGL_API_Thread_Command_glIsBuffer *thread_data = &thread_data_local;

   thread_data->buffer = buffer;

   evas_gl_thread_cmd_enqueue(EVAS_GL_THREAD_TYPE_EVGL,
                              _evgl_api_thread_glIsBuffer,
                              thread_data,
                              thread_mode);

   return thread_data->return_value;
}

/*
 * GLboolean
 * glIsEnabled(GLenum cap);
 */

typedef struct
{
   GLboolean return_value;
   GLenum cap;

} EVGL_API_Thread_Command_glIsEnabled;

GLboolean (*orig_evgl_api_glIsEnabled)(GLenum cap);

static void
_evgl_api_thread_glIsEnabled(void *data)
{
   EVGL_API_Thread_Command_glIsEnabled *thread_data =
      (EVGL_API_Thread_Command_glIsEnabled *)data;

   thread_data->return_value = orig_evgl_api_glIsEnabled(thread_data->cap);

}

EAPI GLboolean
glIsEnabled_evgl_api_thread_cmd(GLenum cap)
{
   if (!evas_evgl_thread_enabled() || (_main_thread_id != eina_thread_self()))
     {
        return orig_evgl_api_glIsEnabled(cap);
     }

   int thread_mode = EVAS_GL_THREAD_MODE_FINISH;

   EVGL_API_Thread_Command_glIsEnabled thread_data_local;
   EVGL_API_Thread_Command_glIsEnabled *thread_data = &thread_data_local;

   thread_data->cap = cap;

   evas_gl_thread_cmd_enqueue(EVAS_GL_THREAD_TYPE_EVGL,
                              _evgl_api_thread_glIsEnabled,
                              thread_data,
                              thread_mode);

   return thread_data->return_value;
}

/*
 * GLboolean
 * glIsFramebuffer(GLuint framebuffer);
 */

typedef struct
{
   GLboolean return_value;
   GLuint framebuffer;

} EVGL_API_Thread_Command_glIsFramebuffer;

GLboolean (*orig_evgl_api_glIsFramebuffer)(GLuint framebuffer);

static void
_evgl_api_thread_glIsFramebuffer(void *data)
{
   EVGL_API_Thread_Command_glIsFramebuffer *thread_data =
      (EVGL_API_Thread_Command_glIsFramebuffer *)data;

   thread_data->return_value = orig_evgl_api_glIsFramebuffer(thread_data->framebuffer);

}

EAPI GLboolean
glIsFramebuffer_evgl_api_thread_cmd(GLuint framebuffer)
{
   if (!evas_evgl_thread_enabled() || (_main_thread_id != eina_thread_self()))
     {
        return orig_evgl_api_glIsFramebuffer(framebuffer);
     }

   int thread_mode = EVAS_GL_THREAD_MODE_FINISH;

   EVGL_API_Thread_Command_glIsFramebuffer thread_data_local;
   EVGL_API_Thread_Command_glIsFramebuffer *thread_data = &thread_data_local;

   thread_data->framebuffer = framebuffer;

   evas_gl_thread_cmd_enqueue(EVAS_GL_THREAD_TYPE_EVGL,
                              _evgl_api_thread_glIsFramebuffer,
                              thread_data,
                              thread_mode);

   return thread_data->return_value;
}

/*
 * GLboolean
 * glIsProgram(GLuint program);
 */

typedef struct
{
   GLboolean return_value;
   GLuint program;

} EVGL_API_Thread_Command_glIsProgram;

GLboolean (*orig_evgl_api_glIsProgram)(GLuint program);

static void
_evgl_api_thread_glIsProgram(void *data)
{
   EVGL_API_Thread_Command_glIsProgram *thread_data =
      (EVGL_API_Thread_Command_glIsProgram *)data;

   thread_data->return_value = orig_evgl_api_glIsProgram(thread_data->program);

}

EAPI GLboolean
glIsProgram_evgl_api_thread_cmd(GLuint program)
{
   if (!evas_evgl_thread_enabled() || (_main_thread_id != eina_thread_self()))
     {
        return orig_evgl_api_glIsProgram(program);
     }

   int thread_mode = EVAS_GL_THREAD_MODE_FINISH;

   EVGL_API_Thread_Command_glIsProgram thread_data_local;
   EVGL_API_Thread_Command_glIsProgram *thread_data = &thread_data_local;

   thread_data->program = program;

   evas_gl_thread_cmd_enqueue(EVAS_GL_THREAD_TYPE_EVGL,
                              _evgl_api_thread_glIsProgram,
                              thread_data,
                              thread_mode);

   return thread_data->return_value;
}

/*
 * GLboolean
 * glIsRenderbuffer(GLuint renderbuffer);
 */

typedef struct
{
   GLboolean return_value;
   GLuint renderbuffer;

} EVGL_API_Thread_Command_glIsRenderbuffer;

GLboolean (*orig_evgl_api_glIsRenderbuffer)(GLuint renderbuffer);

static void
_evgl_api_thread_glIsRenderbuffer(void *data)
{
   EVGL_API_Thread_Command_glIsRenderbuffer *thread_data =
      (EVGL_API_Thread_Command_glIsRenderbuffer *)data;

   thread_data->return_value = orig_evgl_api_glIsRenderbuffer(thread_data->renderbuffer);

}

EAPI GLboolean
glIsRenderbuffer_evgl_api_thread_cmd(GLuint renderbuffer)
{
   if (!evas_evgl_thread_enabled() || (_main_thread_id != eina_thread_self()))
     {
        return orig_evgl_api_glIsRenderbuffer(renderbuffer);
     }

   int thread_mode = EVAS_GL_THREAD_MODE_FINISH;

   EVGL_API_Thread_Command_glIsRenderbuffer thread_data_local;
   EVGL_API_Thread_Command_glIsRenderbuffer *thread_data = &thread_data_local;

   thread_data->renderbuffer = renderbuffer;

   evas_gl_thread_cmd_enqueue(EVAS_GL_THREAD_TYPE_EVGL,
                              _evgl_api_thread_glIsRenderbuffer,
                              thread_data,
                              thread_mode);

   return thread_data->return_value;
}

/*
 * GLboolean
 * glIsShader(GLuint shader);
 */

typedef struct
{
   GLboolean return_value;
   GLuint shader;

} EVGL_API_Thread_Command_glIsShader;

GLboolean (*orig_evgl_api_glIsShader)(GLuint shader);

static void
_evgl_api_thread_glIsShader(void *data)
{
   EVGL_API_Thread_Command_glIsShader *thread_data =
      (EVGL_API_Thread_Command_glIsShader *)data;

   thread_data->return_value = orig_evgl_api_glIsShader(thread_data->shader);

}

EAPI GLboolean
glIsShader_evgl_api_thread_cmd(GLuint shader)
{
   if (!evas_evgl_thread_enabled() || (_main_thread_id != eina_thread_self()))
     {
        return orig_evgl_api_glIsShader(shader);
     }

   int thread_mode = EVAS_GL_THREAD_MODE_FINISH;

   EVGL_API_Thread_Command_glIsShader thread_data_local;
   EVGL_API_Thread_Command_glIsShader *thread_data = &thread_data_local;

   thread_data->shader = shader;

   evas_gl_thread_cmd_enqueue(EVAS_GL_THREAD_TYPE_EVGL,
                              _evgl_api_thread_glIsShader,
                              thread_data,
                              thread_mode);

   return thread_data->return_value;
}

/*
 * GLboolean
 * glIsTexture(GLuint texture);
 */

typedef struct
{
   GLboolean return_value;
   GLuint texture;

} EVGL_API_Thread_Command_glIsTexture;

GLboolean (*orig_evgl_api_glIsTexture)(GLuint texture);

static void
_evgl_api_thread_glIsTexture(void *data)
{
   EVGL_API_Thread_Command_glIsTexture *thread_data =
      (EVGL_API_Thread_Command_glIsTexture *)data;

   thread_data->return_value = orig_evgl_api_glIsTexture(thread_data->texture);

}

EAPI GLboolean
glIsTexture_evgl_api_thread_cmd(GLuint texture)
{
   if (!evas_evgl_thread_enabled() || (_main_thread_id != eina_thread_self()))
     {
        return orig_evgl_api_glIsTexture(texture);
     }

   int thread_mode = EVAS_GL_THREAD_MODE_FINISH;

   EVGL_API_Thread_Command_glIsTexture thread_data_local;
   EVGL_API_Thread_Command_glIsTexture *thread_data = &thread_data_local;

   thread_data->texture = texture;

   evas_gl_thread_cmd_enqueue(EVAS_GL_THREAD_TYPE_EVGL,
                              _evgl_api_thread_glIsTexture,
                              thread_data,
                              thread_mode);

   return thread_data->return_value;
}

/*
 * void
 * glLineWidth(GLfloat width);
 */

typedef struct
{
   GLfloat width;
   int command_allocated;

} EVGL_API_Thread_Command_glLineWidth;

void (*orig_evgl_api_glLineWidth)(GLfloat width);

static void
_evgl_api_thread_glLineWidth(void *data)
{
   EVGL_API_Thread_Command_glLineWidth *thread_data =
      (EVGL_API_Thread_Command_glLineWidth *)data;

   orig_evgl_api_glLineWidth(thread_data->width);

   if (thread_data->command_allocated)
     eina_mempool_free(_mp_command, thread_data);
}

EAPI void
glLineWidth_evgl_api_thread_cmd(GLfloat width)
{
   if (!evas_evgl_thread_enabled() || (_main_thread_id != eina_thread_self()))
     {
        orig_evgl_api_glLineWidth(width);
        return;
     }

   int thread_mode = EVAS_GL_THREAD_MODE_FINISH;

   EVGL_API_Thread_Command_glLineWidth thread_data_local;
   EVGL_API_Thread_Command_glLineWidth *thread_data = &thread_data_local;

   /* command_allocated flag init. */
   thread_data->command_allocated = 0;

   if (!evas_gl_thread_force_finish())
     { /* _flush */
        EVGL_API_Thread_Command_glLineWidth *thread_data_new;
        thread_data_new = eina_mempool_malloc(_mp_command,
                                              sizeof(EVGL_API_Thread_Command_glLineWidth));
        if (thread_data_new)
          {
             thread_data = thread_data_new;
             thread_data->command_allocated = 1;
             thread_mode = EVAS_GL_THREAD_MODE_FLUSH;
          }
     }

   thread_data->width = width;

   evas_gl_thread_cmd_enqueue(EVAS_GL_THREAD_TYPE_EVGL,
                              _evgl_api_thread_glLineWidth,
                              thread_data,
                              thread_mode);
}

/*
 * void
 * glLinkProgram(GLuint program);
 */

typedef struct
{
   GLuint program;
   int command_allocated;

} EVGL_API_Thread_Command_glLinkProgram;

void (*orig_evgl_api_glLinkProgram)(GLuint program);

static void
_evgl_api_thread_glLinkProgram(void *data)
{
   EVGL_API_Thread_Command_glLinkProgram *thread_data =
      (EVGL_API_Thread_Command_glLinkProgram *)data;

   orig_evgl_api_glLinkProgram(thread_data->program);

   if (thread_data->command_allocated)
     eina_mempool_free(_mp_command, thread_data);
}

EAPI void
glLinkProgram_evgl_api_thread_cmd(GLuint program)
{
   if (!evas_evgl_thread_enabled() || (_main_thread_id != eina_thread_self()))
     {
        orig_evgl_api_glLinkProgram(program);
        return;
     }

   int thread_mode = EVAS_GL_THREAD_MODE_FINISH;

   EVGL_API_Thread_Command_glLinkProgram thread_data_local;
   EVGL_API_Thread_Command_glLinkProgram *thread_data = &thread_data_local;

   /* command_allocated flag init. */
   thread_data->command_allocated = 0;

   if (!evas_gl_thread_force_finish())
     { /* _flush */
        EVGL_API_Thread_Command_glLinkProgram *thread_data_new;
        thread_data_new = eina_mempool_malloc(_mp_command,
                                              sizeof(EVGL_API_Thread_Command_glLinkProgram));
        if (thread_data_new)
          {
             thread_data = thread_data_new;
             thread_data->command_allocated = 1;
             thread_mode = EVAS_GL_THREAD_MODE_FLUSH;
          }
     }

   thread_data->program = program;

   evas_gl_thread_cmd_enqueue(EVAS_GL_THREAD_TYPE_EVGL,
                              _evgl_api_thread_glLinkProgram,
                              thread_data,
                              thread_mode);
}

/*
 * void
 * glPixelStorei(GLenum pname, GLint param);
 */

typedef struct
{
   GLenum pname;
   GLint param;
   int command_allocated;

} EVGL_API_Thread_Command_glPixelStorei;

void (*orig_evgl_api_glPixelStorei)(GLenum pname, GLint param);

static void
_evgl_api_thread_glPixelStorei(void *data)
{
   EVGL_API_Thread_Command_glPixelStorei *thread_data =
      (EVGL_API_Thread_Command_glPixelStorei *)data;

   orig_evgl_api_glPixelStorei(thread_data->pname,
                               thread_data->param);

   if (thread_data->command_allocated)
     eina_mempool_free(_mp_command, thread_data);
}

EAPI void
glPixelStorei_evgl_api_thread_cmd(GLenum pname, GLint param)
{
   if (!evas_evgl_thread_enabled() || (_main_thread_id != eina_thread_self()))
     {
        orig_evgl_api_glPixelStorei(pname, param);
        return;
     }

   int thread_mode = EVAS_GL_THREAD_MODE_FINISH;

   EVGL_API_Thread_Command_glPixelStorei thread_data_local;
   EVGL_API_Thread_Command_glPixelStorei *thread_data = &thread_data_local;

   /* command_allocated flag init. */
   thread_data->command_allocated = 0;

   if (!evas_gl_thread_force_finish())
     { /* _flush */
        EVGL_API_Thread_Command_glPixelStorei *thread_data_new;
        thread_data_new = eina_mempool_malloc(_mp_command,
                                              sizeof(EVGL_API_Thread_Command_glPixelStorei));
        if (thread_data_new)
          {
             thread_data = thread_data_new;
             thread_data->command_allocated = 1;
             thread_mode = EVAS_GL_THREAD_MODE_FLUSH;
          }
     }

   thread_data->pname = pname;
   thread_data->param = param;

   evas_gl_thread_cmd_enqueue(EVAS_GL_THREAD_TYPE_EVGL,
                              _evgl_api_thread_glPixelStorei,
                              thread_data,
                              thread_mode);
}

/*
 * void
 * glPolygonOffset(GLfloat factor, GLfloat units);
 */

typedef struct
{
   GLfloat factor;
   GLfloat units;
   int command_allocated;

} EVGL_API_Thread_Command_glPolygonOffset;

void (*orig_evgl_api_glPolygonOffset)(GLfloat factor, GLfloat units);

static void
_evgl_api_thread_glPolygonOffset(void *data)
{
   EVGL_API_Thread_Command_glPolygonOffset *thread_data =
      (EVGL_API_Thread_Command_glPolygonOffset *)data;

   orig_evgl_api_glPolygonOffset(thread_data->factor,
                                 thread_data->units);

   if (thread_data->command_allocated)
     eina_mempool_free(_mp_command, thread_data);
}

EAPI void
glPolygonOffset_evgl_api_thread_cmd(GLfloat factor, GLfloat units)
{
   if (!evas_evgl_thread_enabled() || (_main_thread_id != eina_thread_self()))
     {
        orig_evgl_api_glPolygonOffset(factor, units);
        return;
     }

   int thread_mode = EVAS_GL_THREAD_MODE_FINISH;

   EVGL_API_Thread_Command_glPolygonOffset thread_data_local;
   EVGL_API_Thread_Command_glPolygonOffset *thread_data = &thread_data_local;

   /* command_allocated flag init. */
   thread_data->command_allocated = 0;

   if (!evas_gl_thread_force_finish())
     { /* _flush */
        EVGL_API_Thread_Command_glPolygonOffset *thread_data_new;
        thread_data_new = eina_mempool_malloc(_mp_command,
                                              sizeof(EVGL_API_Thread_Command_glPolygonOffset));
        if (thread_data_new)
          {
             thread_data = thread_data_new;
             thread_data->command_allocated = 1;
             thread_mode = EVAS_GL_THREAD_MODE_FLUSH;
          }
     }

   thread_data->factor = factor;
   thread_data->units = units;

   evas_gl_thread_cmd_enqueue(EVAS_GL_THREAD_TYPE_EVGL,
                              _evgl_api_thread_glPolygonOffset,
                              thread_data,
                              thread_mode);
}

/*
 * void
 * glReadPixels(GLint x, GLint y, GLsizei width, GLsizei height, GLenum format, GLenum type, void* pixels);
 */

typedef struct
{
   GLint x;
   GLint y;
   GLsizei width;
   GLsizei height;
   GLenum format;
   GLenum type;
   void* pixels;

} EVGL_API_Thread_Command_glReadPixels;

void (*orig_evgl_api_glReadPixels)(GLint x, GLint y, GLsizei width, GLsizei height, GLenum format, GLenum type, void* pixels);

static void
_evgl_api_thread_glReadPixels(void *data)
{
   EVGL_API_Thread_Command_glReadPixels *thread_data =
      (EVGL_API_Thread_Command_glReadPixels *)data;

   orig_evgl_api_glReadPixels(thread_data->x,
                              thread_data->y,
                              thread_data->width,
                              thread_data->height,
                              thread_data->format,
                              thread_data->type,
                              thread_data->pixels);

}

EAPI void
glReadPixels_evgl_api_thread_cmd(GLint x, GLint y, GLsizei width, GLsizei height, GLenum format, GLenum type, void* pixels)
{
   if (!evas_evgl_thread_enabled() || (_main_thread_id != eina_thread_self()))
     {
        orig_evgl_api_glReadPixels(x, y, width, height, format, type, pixels);
        return;
     }

   int thread_mode = EVAS_GL_THREAD_MODE_FINISH;

   EVGL_API_Thread_Command_glReadPixels thread_data_local;
   EVGL_API_Thread_Command_glReadPixels *thread_data = &thread_data_local;

   thread_data->x = x;
   thread_data->y = y;
   thread_data->width = width;
   thread_data->height = height;
   thread_data->format = format;
   thread_data->type = type;
   thread_data->pixels = pixels;

   evas_gl_thread_cmd_enqueue(EVAS_GL_THREAD_TYPE_EVGL,
                              _evgl_api_thread_glReadPixels,
                              thread_data,
                              thread_mode);
}

/*
 * void
 * glReleaseShaderCompiler(void);
 */

void (*orig_evgl_api_glReleaseShaderCompiler)(void);

static void
_evgl_api_thread_glReleaseShaderCompiler(void *data EINA_UNUSED)
{
   orig_evgl_api_glReleaseShaderCompiler();

}

EAPI void
glReleaseShaderCompiler_evgl_api_thread_cmd(void)
{
   if (!evas_evgl_thread_enabled() || (_main_thread_id != eina_thread_self()))
     {
        orig_evgl_api_glReleaseShaderCompiler();
        return;
     }

   int thread_mode = EVAS_GL_THREAD_MODE_FINISH;

   evas_gl_thread_cmd_enqueue(EVAS_GL_THREAD_TYPE_EVGL,
                              _evgl_api_thread_glReleaseShaderCompiler,
                              NULL,
                              thread_mode);
}

/*
 * void
 * glRenderbufferStorage(GLenum target, GLenum internalformat, GLsizei width, GLsizei height);
 */

typedef struct
{
   GLenum target;
   GLenum internalformat;
   GLsizei width;
   GLsizei height;
   int command_allocated;

} EVGL_API_Thread_Command_glRenderbufferStorage;

void (*orig_evgl_api_glRenderbufferStorage)(GLenum target, GLenum internalformat, GLsizei width, GLsizei height);

static void
_evgl_api_thread_glRenderbufferStorage(void *data)
{
   EVGL_API_Thread_Command_glRenderbufferStorage *thread_data =
      (EVGL_API_Thread_Command_glRenderbufferStorage *)data;

   orig_evgl_api_glRenderbufferStorage(thread_data->target,
                                       thread_data->internalformat,
                                       thread_data->width,
                                       thread_data->height);

   if (thread_data->command_allocated)
     eina_mempool_free(_mp_command, thread_data);
}

EAPI void
glRenderbufferStorage_evgl_api_thread_cmd(GLenum target, GLenum internalformat, GLsizei width, GLsizei height)
{
   if (!evas_evgl_thread_enabled() || (_main_thread_id != eina_thread_self()))
     {
        orig_evgl_api_glRenderbufferStorage(target, internalformat, width, height);
        return;
     }

   int thread_mode = EVAS_GL_THREAD_MODE_FINISH;

   EVGL_API_Thread_Command_glRenderbufferStorage thread_data_local;
   EVGL_API_Thread_Command_glRenderbufferStorage *thread_data = &thread_data_local;

   /* command_allocated flag init. */
   thread_data->command_allocated = 0;

   if (!evas_gl_thread_force_finish())
     { /* _flush */
        EVGL_API_Thread_Command_glRenderbufferStorage *thread_data_new;
        thread_data_new = eina_mempool_malloc(_mp_command,
                                              sizeof(EVGL_API_Thread_Command_glRenderbufferStorage));
        if (thread_data_new)
          {
             thread_data = thread_data_new;
             thread_data->command_allocated = 1;
             thread_mode = EVAS_GL_THREAD_MODE_FLUSH;
          }
     }

   thread_data->target = target;
   thread_data->internalformat = internalformat;
   thread_data->width = width;
   thread_data->height = height;

   evas_gl_thread_cmd_enqueue(EVAS_GL_THREAD_TYPE_EVGL,
                              _evgl_api_thread_glRenderbufferStorage,
                              thread_data,
                              thread_mode);
}

/*
 * void
 * glSampleCoverage(GLclampf value, GLboolean invert);
 */

typedef struct
{
   GLclampf value;
   GLboolean invert;

} EVGL_API_Thread_Command_glSampleCoverage;

void (*orig_evgl_api_glSampleCoverage)(GLclampf value, GLboolean invert);

static void
_evgl_api_thread_glSampleCoverage(void *data)
{
   EVGL_API_Thread_Command_glSampleCoverage *thread_data =
      (EVGL_API_Thread_Command_glSampleCoverage *)data;

   orig_evgl_api_glSampleCoverage(thread_data->value,
                                  thread_data->invert);

}

EAPI void
glSampleCoverage_evgl_api_thread_cmd(GLclampf value, GLboolean invert)
{
   if (!evas_evgl_thread_enabled() || (_main_thread_id != eina_thread_self()))
     {
        orig_evgl_api_glSampleCoverage(value, invert);
        return;
     }

   int thread_mode = EVAS_GL_THREAD_MODE_FINISH;

   EVGL_API_Thread_Command_glSampleCoverage thread_data_local;
   EVGL_API_Thread_Command_glSampleCoverage *thread_data = &thread_data_local;

   thread_data->value = value;
   thread_data->invert = invert;

   evas_gl_thread_cmd_enqueue(EVAS_GL_THREAD_TYPE_EVGL,
                              _evgl_api_thread_glSampleCoverage,
                              thread_data,
                              thread_mode);
}

/*
 * void
 * glScissor(GLint x, GLint y, GLsizei width, GLsizei height);
 */

typedef struct
{
   GLint x;
   GLint y;
   GLsizei width;
   GLsizei height;
   int command_allocated;

} EVGL_API_Thread_Command_glScissor;

void (*orig_evgl_api_glScissor)(GLint x, GLint y, GLsizei width, GLsizei height);

static void
_evgl_api_thread_glScissor(void *data)
{
   EVGL_API_Thread_Command_glScissor *thread_data =
      (EVGL_API_Thread_Command_glScissor *)data;

   orig_evgl_api_glScissor(thread_data->x,
                           thread_data->y,
                           thread_data->width,
                           thread_data->height);

   if (thread_data->command_allocated)
     eina_mempool_free(_mp_command, thread_data);
}

EAPI void
glScissor_evgl_api_thread_cmd(GLint x, GLint y, GLsizei width, GLsizei height)
{
   if (!evas_evgl_thread_enabled() || (_main_thread_id != eina_thread_self()))
     {
        orig_evgl_api_glScissor(x, y, width, height);
        return;
     }

   int thread_mode = EVAS_GL_THREAD_MODE_FINISH;

   EVGL_API_Thread_Command_glScissor thread_data_local;
   EVGL_API_Thread_Command_glScissor *thread_data = &thread_data_local;

   /* command_allocated flag init. */
   thread_data->command_allocated = 0;

   if (!evas_gl_thread_force_finish())
     { /* _flush */
        EVGL_API_Thread_Command_glScissor *thread_data_new;
        thread_data_new = eina_mempool_malloc(_mp_command,
                                              sizeof(EVGL_API_Thread_Command_glScissor));
        if (thread_data_new)
          {
             thread_data = thread_data_new;
             thread_data->command_allocated = 1;
             thread_mode = EVAS_GL_THREAD_MODE_FLUSH;
          }
     }

   thread_data->x = x;
   thread_data->y = y;
   thread_data->width = width;
   thread_data->height = height;

   evas_gl_thread_cmd_enqueue(EVAS_GL_THREAD_TYPE_EVGL,
                              _evgl_api_thread_glScissor,
                              thread_data,
                              thread_mode);
}

/*
 * void
 * glShaderBinary(GLsizei n, const GLuint* shaders, GLenum binaryformat, const void* binary, GLsizei length);
 */

typedef struct
{
   GLsizei n;
   const GLuint* shaders;
   GLenum binaryformat;
   const void* binary;
   GLsizei length;

} EVGL_API_Thread_Command_glShaderBinary;

void (*orig_evgl_api_glShaderBinary)(GLsizei n, const GLuint* shaders, GLenum binaryformat, const void* binary, GLsizei length);

static void
_evgl_api_thread_glShaderBinary(void *data)
{
   EVGL_API_Thread_Command_glShaderBinary *thread_data =
      (EVGL_API_Thread_Command_glShaderBinary *)data;

   orig_evgl_api_glShaderBinary(thread_data->n,
                                thread_data->shaders,
                                thread_data->binaryformat,
                                thread_data->binary,
                                thread_data->length);

}

EAPI void
glShaderBinary_evgl_api_thread_cmd(GLsizei n, const GLuint* shaders, GLenum binaryformat, const void* binary, GLsizei length)
{
   if (!evas_evgl_thread_enabled() || (_main_thread_id != eina_thread_self()))
     {
        orig_evgl_api_glShaderBinary(n, shaders, binaryformat, binary, length);
        return;
     }

   int thread_mode = EVAS_GL_THREAD_MODE_FINISH;

   EVGL_API_Thread_Command_glShaderBinary thread_data_local;
   EVGL_API_Thread_Command_glShaderBinary *thread_data = &thread_data_local;

   thread_data->n = n;
   thread_data->shaders = shaders;
   thread_data->binaryformat = binaryformat;
   thread_data->binary = binary;
   thread_data->length = length;

   evas_gl_thread_cmd_enqueue(EVAS_GL_THREAD_TYPE_EVGL,
                              _evgl_api_thread_glShaderBinary,
                              thread_data,
                              thread_mode);
}

/*
 * void
 * glShaderSource(GLuint shader, GLsizei count, const char* const * string, const GLint* length);
 */

typedef struct
{
   GLuint shader;
   GLsizei count;
   const char* const * string;
   const GLint* length;
   int command_allocated;
   GLSHADERSOURCE_COPY_VARIABLE; /* TODO */

} EVGL_API_Thread_Command_glShaderSource;

void (*orig_evgl_api_glShaderSource)(GLuint shader, GLsizei count, const char* const * string, const GLint* length);

static void
_evgl_api_thread_glShaderSource(void *data)
{
   EVGL_API_Thread_Command_glShaderSource *thread_data =
      (EVGL_API_Thread_Command_glShaderSource *)data;

   orig_evgl_api_glShaderSource(thread_data->shader,
                                thread_data->count,
                                thread_data->string,
                                thread_data->length);

   GLSHADERSOURCE_COPY_VARIABLE_FREE; /* TODO */

   if (thread_data->command_allocated)
     eina_mempool_free(_mp_command, thread_data);
}

EAPI void
glShaderSource_evgl_api_thread_cmd(GLuint shader, GLsizei count, const char* const * string, const GLint* length)
{
   if (!evas_evgl_thread_enabled() || (_main_thread_id != eina_thread_self()))
     {
        orig_evgl_api_glShaderSource(shader, count, string, length);
        return;
     }

   int thread_mode = EVAS_GL_THREAD_MODE_FINISH;

   EVGL_API_Thread_Command_glShaderSource thread_data_local;
   EVGL_API_Thread_Command_glShaderSource *thread_data = &thread_data_local;

   /* command_allocated flag init. */
   thread_data->command_allocated = 0;

   if (!evas_gl_thread_force_finish())
     { /* _flush */
        EVGL_API_Thread_Command_glShaderSource *thread_data_new;
        thread_data_new = eina_mempool_malloc(_mp_command,
                                              sizeof(EVGL_API_Thread_Command_glShaderSource));
        if (thread_data_new)
          {
             thread_data = thread_data_new;
             thread_data->command_allocated = 1;
             thread_mode = EVAS_GL_THREAD_MODE_FLUSH;
          }
     }

   thread_data->shader = shader;
   thread_data->count = count;
   thread_data->string = string;
   thread_data->length = length;

   GLSHADERSOURCE_COPY_VARIABLE_INIT; /* TODO */

   if (thread_mode == EVAS_GL_THREAD_MODE_FINISH)
     goto finish;

   GLSHADERSOURCE_COPY_TO_MEMPOOL; /* TODO */

finish:
   evas_gl_thread_cmd_enqueue(EVAS_GL_THREAD_TYPE_EVGL,
                              _evgl_api_thread_glShaderSource,
                              thread_data,
                              thread_mode);
}

/*
 * void
 * glStencilFunc(GLenum func, GLint ref, GLuint mask);
 */

typedef struct
{
   GLenum func;
   GLint ref;
   GLuint mask;

} EVGL_API_Thread_Command_glStencilFunc;

void (*orig_evgl_api_glStencilFunc)(GLenum func, GLint ref, GLuint mask);

static void
_evgl_api_thread_glStencilFunc(void *data)
{
   EVGL_API_Thread_Command_glStencilFunc *thread_data =
      (EVGL_API_Thread_Command_glStencilFunc *)data;

   orig_evgl_api_glStencilFunc(thread_data->func,
                               thread_data->ref,
                               thread_data->mask);

}

EAPI void
glStencilFunc_evgl_api_thread_cmd(GLenum func, GLint ref, GLuint mask)
{
   if (!evas_evgl_thread_enabled() || (_main_thread_id != eina_thread_self()))
     {
        orig_evgl_api_glStencilFunc(func, ref, mask);
        return;
     }

   int thread_mode = EVAS_GL_THREAD_MODE_FINISH;

   EVGL_API_Thread_Command_glStencilFunc thread_data_local;
   EVGL_API_Thread_Command_glStencilFunc *thread_data = &thread_data_local;

   thread_data->func = func;
   thread_data->ref = ref;
   thread_data->mask = mask;

   evas_gl_thread_cmd_enqueue(EVAS_GL_THREAD_TYPE_EVGL,
                              _evgl_api_thread_glStencilFunc,
                              thread_data,
                              thread_mode);
}

/*
 * void
 * glStencilFuncSeparate(GLenum face, GLenum func, GLint ref, GLuint mask);
 */

typedef struct
{
   GLenum face;
   GLenum func;
   GLint ref;
   GLuint mask;

} EVGL_API_Thread_Command_glStencilFuncSeparate;

void (*orig_evgl_api_glStencilFuncSeparate)(GLenum face, GLenum func, GLint ref, GLuint mask);

static void
_evgl_api_thread_glStencilFuncSeparate(void *data)
{
   EVGL_API_Thread_Command_glStencilFuncSeparate *thread_data =
      (EVGL_API_Thread_Command_glStencilFuncSeparate *)data;

   orig_evgl_api_glStencilFuncSeparate(thread_data->face,
                                       thread_data->func,
                                       thread_data->ref,
                                       thread_data->mask);

}

EAPI void
glStencilFuncSeparate_evgl_api_thread_cmd(GLenum face, GLenum func, GLint ref, GLuint mask)
{
   if (!evas_evgl_thread_enabled() || (_main_thread_id != eina_thread_self()))
     {
        orig_evgl_api_glStencilFuncSeparate(face, func, ref, mask);
        return;
     }

   int thread_mode = EVAS_GL_THREAD_MODE_FINISH;

   EVGL_API_Thread_Command_glStencilFuncSeparate thread_data_local;
   EVGL_API_Thread_Command_glStencilFuncSeparate *thread_data = &thread_data_local;

   thread_data->face = face;
   thread_data->func = func;
   thread_data->ref = ref;
   thread_data->mask = mask;

   evas_gl_thread_cmd_enqueue(EVAS_GL_THREAD_TYPE_EVGL,
                              _evgl_api_thread_glStencilFuncSeparate,
                              thread_data,
                              thread_mode);
}

/*
 * void
 * glStencilMask(GLuint mask);
 */

typedef struct
{
   GLuint mask;

} EVGL_API_Thread_Command_glStencilMask;

void (*orig_evgl_api_glStencilMask)(GLuint mask);

static void
_evgl_api_thread_glStencilMask(void *data)
{
   EVGL_API_Thread_Command_glStencilMask *thread_data =
      (EVGL_API_Thread_Command_glStencilMask *)data;

   orig_evgl_api_glStencilMask(thread_data->mask);

}

EAPI void
glStencilMask_evgl_api_thread_cmd(GLuint mask)
{
   if (!evas_evgl_thread_enabled() || (_main_thread_id != eina_thread_self()))
     {
        orig_evgl_api_glStencilMask(mask);
        return;
     }

   int thread_mode = EVAS_GL_THREAD_MODE_FINISH;

   EVGL_API_Thread_Command_glStencilMask thread_data_local;
   EVGL_API_Thread_Command_glStencilMask *thread_data = &thread_data_local;

   thread_data->mask = mask;

   evas_gl_thread_cmd_enqueue(EVAS_GL_THREAD_TYPE_EVGL,
                              _evgl_api_thread_glStencilMask,
                              thread_data,
                              thread_mode);
}

/*
 * void
 * glStencilMaskSeparate(GLenum face, GLuint mask);
 */

typedef struct
{
   GLenum face;
   GLuint mask;

} EVGL_API_Thread_Command_glStencilMaskSeparate;

void (*orig_evgl_api_glStencilMaskSeparate)(GLenum face, GLuint mask);

static void
_evgl_api_thread_glStencilMaskSeparate(void *data)
{
   EVGL_API_Thread_Command_glStencilMaskSeparate *thread_data =
      (EVGL_API_Thread_Command_glStencilMaskSeparate *)data;

   orig_evgl_api_glStencilMaskSeparate(thread_data->face,
                                       thread_data->mask);

}

EAPI void
glStencilMaskSeparate_evgl_api_thread_cmd(GLenum face, GLuint mask)
{
   if (!evas_evgl_thread_enabled() || (_main_thread_id != eina_thread_self()))
     {
        orig_evgl_api_glStencilMaskSeparate(face, mask);
        return;
     }

   int thread_mode = EVAS_GL_THREAD_MODE_FINISH;

   EVGL_API_Thread_Command_glStencilMaskSeparate thread_data_local;
   EVGL_API_Thread_Command_glStencilMaskSeparate *thread_data = &thread_data_local;

   thread_data->face = face;
   thread_data->mask = mask;

   evas_gl_thread_cmd_enqueue(EVAS_GL_THREAD_TYPE_EVGL,
                              _evgl_api_thread_glStencilMaskSeparate,
                              thread_data,
                              thread_mode);
}

/*
 * void
 * glStencilOp(GLenum fail, GLenum zfail, GLenum zpass);
 */

typedef struct
{
   GLenum fail;
   GLenum zfail;
   GLenum zpass;

} EVGL_API_Thread_Command_glStencilOp;

void (*orig_evgl_api_glStencilOp)(GLenum fail, GLenum zfail, GLenum zpass);

static void
_evgl_api_thread_glStencilOp(void *data)
{
   EVGL_API_Thread_Command_glStencilOp *thread_data =
      (EVGL_API_Thread_Command_glStencilOp *)data;

   orig_evgl_api_glStencilOp(thread_data->fail,
                             thread_data->zfail,
                             thread_data->zpass);

}

EAPI void
glStencilOp_evgl_api_thread_cmd(GLenum fail, GLenum zfail, GLenum zpass)
{
   if (!evas_evgl_thread_enabled() || (_main_thread_id != eina_thread_self()))
     {
        orig_evgl_api_glStencilOp(fail, zfail, zpass);
        return;
     }

   int thread_mode = EVAS_GL_THREAD_MODE_FINISH;

   EVGL_API_Thread_Command_glStencilOp thread_data_local;
   EVGL_API_Thread_Command_glStencilOp *thread_data = &thread_data_local;

   thread_data->fail = fail;
   thread_data->zfail = zfail;
   thread_data->zpass = zpass;

   evas_gl_thread_cmd_enqueue(EVAS_GL_THREAD_TYPE_EVGL,
                              _evgl_api_thread_glStencilOp,
                              thread_data,
                              thread_mode);
}

/*
 * void
 * glStencilOpSeparate(GLenum face, GLenum fail, GLenum zfail, GLenum zpass);
 */

typedef struct
{
   GLenum face;
   GLenum fail;
   GLenum zfail;
   GLenum zpass;

} EVGL_API_Thread_Command_glStencilOpSeparate;

void (*orig_evgl_api_glStencilOpSeparate)(GLenum face, GLenum fail, GLenum zfail, GLenum zpass);

static void
_evgl_api_thread_glStencilOpSeparate(void *data)
{
   EVGL_API_Thread_Command_glStencilOpSeparate *thread_data =
      (EVGL_API_Thread_Command_glStencilOpSeparate *)data;

   orig_evgl_api_glStencilOpSeparate(thread_data->face,
                                     thread_data->fail,
                                     thread_data->zfail,
                                     thread_data->zpass);

}

EAPI void
glStencilOpSeparate_evgl_api_thread_cmd(GLenum face, GLenum fail, GLenum zfail, GLenum zpass)
{
   if (!evas_evgl_thread_enabled() || (_main_thread_id != eina_thread_self()))
     {
        orig_evgl_api_glStencilOpSeparate(face, fail, zfail, zpass);
        return;
     }

   int thread_mode = EVAS_GL_THREAD_MODE_FINISH;

   EVGL_API_Thread_Command_glStencilOpSeparate thread_data_local;
   EVGL_API_Thread_Command_glStencilOpSeparate *thread_data = &thread_data_local;

   thread_data->face = face;
   thread_data->fail = fail;
   thread_data->zfail = zfail;
   thread_data->zpass = zpass;

   evas_gl_thread_cmd_enqueue(EVAS_GL_THREAD_TYPE_EVGL,
                              _evgl_api_thread_glStencilOpSeparate,
                              thread_data,
                              thread_mode);
}

/*
 * void
 * glTexImage2D(GLenum target, GLint level, GLint internalformat, GLsizei width, GLsizei height, GLint border, GLenum format, GLenum type, const void* pixels);
 */

typedef struct
{
   GLenum target;
   GLint level;
   GLint internalformat;
   GLsizei width;
   GLsizei height;
   GLint border;
   GLenum format;
   GLenum type;
   const void* pixels;
   int command_allocated;
   GLTEXIMAGE2D_COPY_VARIABLE; /* TODO */

} EVGL_API_Thread_Command_glTexImage2D;

void (*orig_evgl_api_glTexImage2D)(GLenum target, GLint level, GLint internalformat, GLsizei width, GLsizei height, GLint border, GLenum format, GLenum type, const void* pixels);

static void
_evgl_api_thread_glTexImage2D(void *data)
{
   EVGL_API_Thread_Command_glTexImage2D *thread_data =
      (EVGL_API_Thread_Command_glTexImage2D *)data;

   orig_evgl_api_glTexImage2D(thread_data->target,
                              thread_data->level,
                              thread_data->internalformat,
                              thread_data->width,
                              thread_data->height,
                              thread_data->border,
                              thread_data->format,
                              thread_data->type,
                              thread_data->pixels);

   GLTEXIMAGE2D_COPY_VARIABLE_FREE; /* TODO */

   if (thread_data->command_allocated)
     eina_mempool_free(_mp_command, thread_data);
}

EAPI void
glTexImage2D_evgl_api_thread_cmd(GLenum target, GLint level, GLint internalformat, GLsizei width, GLsizei height, GLint border, GLenum format, GLenum type, const void* pixels)
{
   if (!evas_evgl_thread_enabled() || (_main_thread_id != eina_thread_self()))
     {
        orig_evgl_api_glTexImage2D(target, level, internalformat, width, height, border, format, type, pixels);
        return;
     }

   int thread_mode = EVAS_GL_THREAD_MODE_FINISH;

   EVGL_API_Thread_Command_glTexImage2D thread_data_local;
   EVGL_API_Thread_Command_glTexImage2D *thread_data = &thread_data_local;

   /* command_allocated flag init. */
   thread_data->command_allocated = 0;

   if (!evas_gl_thread_force_finish())
     { /* _flush */
        EVGL_API_Thread_Command_glTexImage2D *thread_data_new;
        thread_data_new = eina_mempool_malloc(_mp_command,
                                              sizeof(EVGL_API_Thread_Command_glTexImage2D));
        if (thread_data_new)
          {
             thread_data = thread_data_new;
             thread_data->command_allocated = 1;
             thread_mode = EVAS_GL_THREAD_MODE_FLUSH;
          }
     }

   thread_data->target = target;
   thread_data->level = level;
   thread_data->internalformat = internalformat;
   thread_data->width = width;
   thread_data->height = height;
   thread_data->border = border;
   thread_data->format = format;
   thread_data->type = type;
   thread_data->pixels = pixels;

   GLTEXIMAGE2D_COPY_VARIABLE_INIT; /* TODO */

   if (thread_mode == EVAS_GL_THREAD_MODE_FINISH)
     goto finish;

   GLTEXIMAGE2D_COPY_TO_MEMPOOL; /* TODO */

finish:
   evas_gl_thread_cmd_enqueue(EVAS_GL_THREAD_TYPE_EVGL,
                              _evgl_api_thread_glTexImage2D,
                              thread_data,
                              thread_mode);
}

/*
 * void
 * glTexParameterf(GLenum target, GLenum pname, GLfloat param);
 */

typedef struct
{
   GLenum target;
   GLenum pname;
   GLfloat param;
   int command_allocated;

} EVGL_API_Thread_Command_glTexParameterf;

void (*orig_evgl_api_glTexParameterf)(GLenum target, GLenum pname, GLfloat param);

static void
_evgl_api_thread_glTexParameterf(void *data)
{
   EVGL_API_Thread_Command_glTexParameterf *thread_data =
      (EVGL_API_Thread_Command_glTexParameterf *)data;

   orig_evgl_api_glTexParameterf(thread_data->target,
                                 thread_data->pname,
                                 thread_data->param);

   if (thread_data->command_allocated)
     eina_mempool_free(_mp_command, thread_data);
}

EAPI void
glTexParameterf_evgl_api_thread_cmd(GLenum target, GLenum pname, GLfloat param)
{
   if (!evas_evgl_thread_enabled() || (_main_thread_id != eina_thread_self()))
     {
        orig_evgl_api_glTexParameterf(target, pname, param);
        return;
     }

   int thread_mode = EVAS_GL_THREAD_MODE_FINISH;

   EVGL_API_Thread_Command_glTexParameterf thread_data_local;
   EVGL_API_Thread_Command_glTexParameterf *thread_data = &thread_data_local;

   /* command_allocated flag init. */
   thread_data->command_allocated = 0;

   if (!evas_gl_thread_force_finish())
     { /* _flush */
        EVGL_API_Thread_Command_glTexParameterf *thread_data_new;
        thread_data_new = eina_mempool_malloc(_mp_command,
                                              sizeof(EVGL_API_Thread_Command_glTexParameterf));
        if (thread_data_new)
          {
             thread_data = thread_data_new;
             thread_data->command_allocated = 1;
             thread_mode = EVAS_GL_THREAD_MODE_FLUSH;
          }
     }

   thread_data->target = target;
   thread_data->pname = pname;
   thread_data->param = param;

   evas_gl_thread_cmd_enqueue(EVAS_GL_THREAD_TYPE_EVGL,
                              _evgl_api_thread_glTexParameterf,
                              thread_data,
                              thread_mode);
}

/*
 * void
 * glTexParameterfv(GLenum target, GLenum pname, const GLfloat* params);
 */

typedef struct
{
   GLenum target;
   GLenum pname;
   const GLfloat* params;
   void *params_copied; /* COPIED */
   int command_allocated;

} EVGL_API_Thread_Command_glTexParameterfv;

void (*orig_evgl_api_glTexParameterfv)(GLenum target, GLenum pname, const GLfloat* params);

static void
_evgl_api_thread_glTexParameterfv(void *data)
{
   EVGL_API_Thread_Command_glTexParameterfv *thread_data =
      (EVGL_API_Thread_Command_glTexParameterfv *)data;

   orig_evgl_api_glTexParameterfv(thread_data->target,
                                  thread_data->pname,
                                  thread_data->params);


   if (thread_data->params_copied)
     eina_mempool_free(_mp_default, thread_data->params_copied);

   if (thread_data->command_allocated)
     eina_mempool_free(_mp_command, thread_data);
}

EAPI void
glTexParameterfv_evgl_api_thread_cmd(GLenum target, GLenum pname, const GLfloat* params)
{
   if (!evas_evgl_thread_enabled() || (_main_thread_id != eina_thread_self()))
     {
        orig_evgl_api_glTexParameterfv(target, pname, params);
        return;
     }

   int thread_mode = EVAS_GL_THREAD_MODE_FINISH;

   EVGL_API_Thread_Command_glTexParameterfv thread_data_local;
   EVGL_API_Thread_Command_glTexParameterfv *thread_data = &thread_data_local;

   /* command_allocated flag init. */
   thread_data->command_allocated = 0;

   if (!evas_gl_thread_force_finish())
     { /* _flush */
        EVGL_API_Thread_Command_glTexParameterfv *thread_data_new;
        thread_data_new = eina_mempool_malloc(_mp_command,
                                              sizeof(EVGL_API_Thread_Command_glTexParameterfv));
        if (thread_data_new)
          {
             thread_data = thread_data_new;
             thread_data->command_allocated = 1;
             thread_mode = EVAS_GL_THREAD_MODE_FLUSH;
          }
     }

   thread_data->target = target;
   thread_data->pname = pname;
   thread_data->params = params;

   thread_data->params_copied = NULL;
   if (thread_mode == EVAS_GL_THREAD_MODE_FINISH)
     goto finish;

   /* copy variable */
   if (params)
     {
        /* 1. check memory size */
        unsigned int copy_size = sizeof(GLfloat);
        if (copy_size > _mp_default_memory_size)
          {
             thread_mode = EVAS_GL_THREAD_MODE_FINISH;
             goto finish;
          }
        /* 2. malloc & copy */
        thread_data->params_copied = eina_mempool_malloc(_mp_default, copy_size);
        if (thread_data->params_copied)
          {
             memcpy(thread_data->params_copied, params, copy_size);
          }
        else
          {
             thread_mode = EVAS_GL_THREAD_MODE_FINISH;
             goto finish;
          }
        /* 3. replace */
        thread_data->params = (const GLfloat *)thread_data->params_copied;
     }
   /* end of copy variable */

finish:
   evas_gl_thread_cmd_enqueue(EVAS_GL_THREAD_TYPE_EVGL,
                              _evgl_api_thread_glTexParameterfv,
                              thread_data,
                              thread_mode);
}

/*
 * void
 * glTexParameteri(GLenum target, GLenum pname, GLint param);
 */

typedef struct
{
   GLenum target;
   GLenum pname;
   GLint param;
   int command_allocated;

} EVGL_API_Thread_Command_glTexParameteri;

void (*orig_evgl_api_glTexParameteri)(GLenum target, GLenum pname, GLint param);

static void
_evgl_api_thread_glTexParameteri(void *data)
{
   EVGL_API_Thread_Command_glTexParameteri *thread_data =
      (EVGL_API_Thread_Command_glTexParameteri *)data;

   orig_evgl_api_glTexParameteri(thread_data->target,
                                 thread_data->pname,
                                 thread_data->param);

   if (thread_data->command_allocated)
     eina_mempool_free(_mp_command, thread_data);
}

EAPI void
glTexParameteri_evgl_api_thread_cmd(GLenum target, GLenum pname, GLint param)
{
   if (!evas_evgl_thread_enabled() || (_main_thread_id != eina_thread_self()))
     {
        orig_evgl_api_glTexParameteri(target, pname, param);
        return;
     }

   int thread_mode = EVAS_GL_THREAD_MODE_FINISH;

   EVGL_API_Thread_Command_glTexParameteri thread_data_local;
   EVGL_API_Thread_Command_glTexParameteri *thread_data = &thread_data_local;

   /* command_allocated flag init. */
   thread_data->command_allocated = 0;

   if (!evas_gl_thread_force_finish())
     { /* _flush */
        EVGL_API_Thread_Command_glTexParameteri *thread_data_new;
        thread_data_new = eina_mempool_malloc(_mp_command,
                                              sizeof(EVGL_API_Thread_Command_glTexParameteri));
        if (thread_data_new)
          {
             thread_data = thread_data_new;
             thread_data->command_allocated = 1;
             thread_mode = EVAS_GL_THREAD_MODE_FLUSH;
          }
     }

   thread_data->target = target;
   thread_data->pname = pname;
   thread_data->param = param;

   evas_gl_thread_cmd_enqueue(EVAS_GL_THREAD_TYPE_EVGL,
                              _evgl_api_thread_glTexParameteri,
                              thread_data,
                              thread_mode);
}

/*
 * void
 * glTexParameteriv(GLenum target, GLenum pname, const GLint* params);
 */

typedef struct
{
   GLenum target;
   GLenum pname;
   const GLint* params;
   void *params_copied; /* COPIED */
   int command_allocated;

} EVGL_API_Thread_Command_glTexParameteriv;

void (*orig_evgl_api_glTexParameteriv)(GLenum target, GLenum pname, const GLint* params);

static void
_evgl_api_thread_glTexParameteriv(void *data)
{
   EVGL_API_Thread_Command_glTexParameteriv *thread_data =
      (EVGL_API_Thread_Command_glTexParameteriv *)data;

   orig_evgl_api_glTexParameteriv(thread_data->target,
                                  thread_data->pname,
                                  thread_data->params);


   if (thread_data->params_copied)
     eina_mempool_free(_mp_default, thread_data->params_copied);

   if (thread_data->command_allocated)
     eina_mempool_free(_mp_command, thread_data);
}

EAPI void
glTexParameteriv_evgl_api_thread_cmd(GLenum target, GLenum pname, const GLint* params)
{
   if (!evas_evgl_thread_enabled() || (_main_thread_id != eina_thread_self()))
     {
        orig_evgl_api_glTexParameteriv(target, pname, params);
        return;
     }

   int thread_mode = EVAS_GL_THREAD_MODE_FINISH;

   EVGL_API_Thread_Command_glTexParameteriv thread_data_local;
   EVGL_API_Thread_Command_glTexParameteriv *thread_data = &thread_data_local;

   /* command_allocated flag init. */
   thread_data->command_allocated = 0;

   if (!evas_gl_thread_force_finish())
     { /* _flush */
        EVGL_API_Thread_Command_glTexParameteriv *thread_data_new;
        thread_data_new = eina_mempool_malloc(_mp_command,
                                              sizeof(EVGL_API_Thread_Command_glTexParameteriv));
        if (thread_data_new)
          {
             thread_data = thread_data_new;
             thread_data->command_allocated = 1;
             thread_mode = EVAS_GL_THREAD_MODE_FLUSH;
          }
     }

   thread_data->target = target;
   thread_data->pname = pname;
   thread_data->params = params;

   thread_data->params_copied = NULL;
   if (thread_mode == EVAS_GL_THREAD_MODE_FINISH)
     goto finish;

   /* copy variable */
   if (params)
     {
        /* 1. check memory size */
        unsigned int copy_size = sizeof(GLint);
        if (copy_size > _mp_default_memory_size)
          {
             thread_mode = EVAS_GL_THREAD_MODE_FINISH;
             goto finish;
          }
        /* 2. malloc & copy */
        thread_data->params_copied = eina_mempool_malloc(_mp_default, copy_size);
        if (thread_data->params_copied)
          {
             memcpy(thread_data->params_copied, params, copy_size);
          }
        else
          {
             thread_mode = EVAS_GL_THREAD_MODE_FINISH;
             goto finish;
          }
        /* 3. replace */
        thread_data->params = (const GLint *)thread_data->params_copied;
     }
   /* end of copy variable */

finish:
   evas_gl_thread_cmd_enqueue(EVAS_GL_THREAD_TYPE_EVGL,
                              _evgl_api_thread_glTexParameteriv,
                              thread_data,
                              thread_mode);
}

/*
 * void
 * glTexSubImage2D(GLenum target, GLint level, GLint xoffset, GLint yoffset, GLsizei width, GLsizei height, GLenum format, GLenum type, const void* pixels);
 */

typedef struct
{
   GLenum target;
   GLint level;
   GLint xoffset;
   GLint yoffset;
   GLsizei width;
   GLsizei height;
   GLenum format;
   GLenum type;
   const void* pixels;
   int command_allocated;
   GLTEXSUBIMAGE2D_COPY_VARIABLE; /* TODO */

} EVGL_API_Thread_Command_glTexSubImage2D;

void (*orig_evgl_api_glTexSubImage2D)(GLenum target, GLint level, GLint xoffset, GLint yoffset, GLsizei width, GLsizei height, GLenum format, GLenum type, const void* pixels);

static void
_evgl_api_thread_glTexSubImage2D(void *data)
{
   EVGL_API_Thread_Command_glTexSubImage2D *thread_data =
      (EVGL_API_Thread_Command_glTexSubImage2D *)data;

   orig_evgl_api_glTexSubImage2D(thread_data->target,
                                 thread_data->level,
                                 thread_data->xoffset,
                                 thread_data->yoffset,
                                 thread_data->width,
                                 thread_data->height,
                                 thread_data->format,
                                 thread_data->type,
                                 thread_data->pixels);

   GLTEXSUBIMAGE2D_COPY_VARIABLE_FREE; /* TODO */

   if (thread_data->command_allocated)
     eina_mempool_free(_mp_command, thread_data);
}

EAPI void
glTexSubImage2D_evgl_api_thread_cmd(GLenum target, GLint level, GLint xoffset, GLint yoffset, GLsizei width, GLsizei height, GLenum format, GLenum type, const void* pixels)
{
   if (!evas_evgl_thread_enabled() || (_main_thread_id != eina_thread_self()))
     {
        orig_evgl_api_glTexSubImage2D(target, level, xoffset, yoffset, width, height, format, type, pixels);
        return;
     }

   int thread_mode = EVAS_GL_THREAD_MODE_FINISH;

   EVGL_API_Thread_Command_glTexSubImage2D thread_data_local;
   EVGL_API_Thread_Command_glTexSubImage2D *thread_data = &thread_data_local;

   /* command_allocated flag init. */
   thread_data->command_allocated = 0;

   if (!evas_gl_thread_force_finish())
     { /* _flush */
        EVGL_API_Thread_Command_glTexSubImage2D *thread_data_new;
        thread_data_new = eina_mempool_malloc(_mp_command,
                                              sizeof(EVGL_API_Thread_Command_glTexSubImage2D));
        if (thread_data_new)
          {
             thread_data = thread_data_new;
             thread_data->command_allocated = 1;
             thread_mode = EVAS_GL_THREAD_MODE_FLUSH;
          }
     }

   thread_data->target = target;
   thread_data->level = level;
   thread_data->xoffset = xoffset;
   thread_data->yoffset = yoffset;
   thread_data->width = width;
   thread_data->height = height;
   thread_data->format = format;
   thread_data->type = type;
   thread_data->pixels = pixels;

   GLTEXSUBIMAGE2D_COPY_VARIABLE_INIT; /* TODO */

   if (thread_mode == EVAS_GL_THREAD_MODE_FINISH)
     goto finish;

   GLTEXSUBIMAGE2D_COPY_TO_MEMPOOL; /* TODO */

finish:
   evas_gl_thread_cmd_enqueue(EVAS_GL_THREAD_TYPE_EVGL,
                              _evgl_api_thread_glTexSubImage2D,
                              thread_data,
                              thread_mode);
}

/*
 * void
 * glUniform1f(GLint location, GLfloat x);
 */

typedef struct
{
   GLint location;
   GLfloat x;
   int command_allocated;

} EVGL_API_Thread_Command_glUniform1f;

void (*orig_evgl_api_glUniform1f)(GLint location, GLfloat x);

static void
_evgl_api_thread_glUniform1f(void *data)
{
   EVGL_API_Thread_Command_glUniform1f *thread_data =
      (EVGL_API_Thread_Command_glUniform1f *)data;

   orig_evgl_api_glUniform1f(thread_data->location,
                             thread_data->x);

   if (thread_data->command_allocated)
     eina_mempool_free(_mp_command, thread_data);
}

EAPI void
glUniform1f_evgl_api_thread_cmd(GLint location, GLfloat x)
{
   if (!evas_evgl_thread_enabled() || (_main_thread_id != eina_thread_self()))
     {
        orig_evgl_api_glUniform1f(location, x);
        return;
     }

   int thread_mode = EVAS_GL_THREAD_MODE_FINISH;

   EVGL_API_Thread_Command_glUniform1f thread_data_local;
   EVGL_API_Thread_Command_glUniform1f *thread_data = &thread_data_local;

   /* command_allocated flag init. */
   thread_data->command_allocated = 0;

   if (!evas_gl_thread_force_finish())
     { /* _flush */
        EVGL_API_Thread_Command_glUniform1f *thread_data_new;
        thread_data_new = eina_mempool_malloc(_mp_command,
                                              sizeof(EVGL_API_Thread_Command_glUniform1f));
        if (thread_data_new)
          {
             thread_data = thread_data_new;
             thread_data->command_allocated = 1;
             thread_mode = EVAS_GL_THREAD_MODE_FLUSH;
          }
     }

   thread_data->location = location;
   thread_data->x = x;

   evas_gl_thread_cmd_enqueue(EVAS_GL_THREAD_TYPE_EVGL,
                              _evgl_api_thread_glUniform1f,
                              thread_data,
                              thread_mode);
}

/*
 * void
 * glUniform1fv(GLint location, GLsizei count, const GLfloat* v);
 */

typedef struct
{
   GLint location;
   GLsizei count;
   const GLfloat* v;
   void *v_copied; /* COPIED */
   int command_allocated;

} EVGL_API_Thread_Command_glUniform1fv;

void (*orig_evgl_api_glUniform1fv)(GLint location, GLsizei count, const GLfloat* v);

static void
_evgl_api_thread_glUniform1fv(void *data)
{
   EVGL_API_Thread_Command_glUniform1fv *thread_data =
      (EVGL_API_Thread_Command_glUniform1fv *)data;

   orig_evgl_api_glUniform1fv(thread_data->location,
                              thread_data->count,
                              thread_data->v);


   if (thread_data->v_copied)
     eina_mempool_free(_mp_uniform, thread_data->v_copied);

   if (thread_data->command_allocated)
     eina_mempool_free(_mp_command, thread_data);
}

EAPI void
glUniform1fv_evgl_api_thread_cmd(GLint location, GLsizei count, const GLfloat* v)
{
   if (!evas_evgl_thread_enabled() || (_main_thread_id != eina_thread_self()))
     {
        orig_evgl_api_glUniform1fv(location, count, v);
        return;
     }

   int thread_mode = EVAS_GL_THREAD_MODE_FINISH;

   EVGL_API_Thread_Command_glUniform1fv thread_data_local;
   EVGL_API_Thread_Command_glUniform1fv *thread_data = &thread_data_local;

   /* command_allocated flag init. */
   thread_data->command_allocated = 0;

   if (!evas_gl_thread_force_finish())
     { /* _flush */
        EVGL_API_Thread_Command_glUniform1fv *thread_data_new;
        thread_data_new = eina_mempool_malloc(_mp_command,
                                              sizeof(EVGL_API_Thread_Command_glUniform1fv));
        if (thread_data_new)
          {
             thread_data = thread_data_new;
             thread_data->command_allocated = 1;
             thread_mode = EVAS_GL_THREAD_MODE_FLUSH;
          }
     }

   thread_data->location = location;
   thread_data->count = count;
   thread_data->v = v;

   thread_data->v_copied = NULL;
   if (thread_mode == EVAS_GL_THREAD_MODE_FINISH)
     goto finish;

   /* copy variable */
   if (v)
     {
        /* 1. check memory size */
        unsigned int copy_size = 1 * count * sizeof(GLfloat);
        if (copy_size > _mp_uniform_memory_size)
          {
             thread_mode = EVAS_GL_THREAD_MODE_FINISH;
             goto finish;
          }
        /* 2. malloc & copy */
        thread_data->v_copied = eina_mempool_malloc(_mp_uniform, copy_size);
        if (thread_data->v_copied)
          {
             memcpy(thread_data->v_copied, v, copy_size);
          }
        else
          {
             thread_mode = EVAS_GL_THREAD_MODE_FINISH;
             goto finish;
          }
        /* 3. replace */
        thread_data->v = (const GLfloat *)thread_data->v_copied;
     }
   /* end of copy variable */

finish:
   evas_gl_thread_cmd_enqueue(EVAS_GL_THREAD_TYPE_EVGL,
                              _evgl_api_thread_glUniform1fv,
                              thread_data,
                              thread_mode);
}

/*
 * void
 * glUniform1i(GLint location, GLint x);
 */

typedef struct
{
   GLint location;
   GLint x;
   int command_allocated;

} EVGL_API_Thread_Command_glUniform1i;

void (*orig_evgl_api_glUniform1i)(GLint location, GLint x);

static void
_evgl_api_thread_glUniform1i(void *data)
{
   EVGL_API_Thread_Command_glUniform1i *thread_data =
      (EVGL_API_Thread_Command_glUniform1i *)data;

   orig_evgl_api_glUniform1i(thread_data->location,
                             thread_data->x);

   if (thread_data->command_allocated)
     eina_mempool_free(_mp_command, thread_data);
}

EAPI void
glUniform1i_evgl_api_thread_cmd(GLint location, GLint x)
{
   if (!evas_evgl_thread_enabled() || (_main_thread_id != eina_thread_self()))
     {
        orig_evgl_api_glUniform1i(location, x);
        return;
     }

   int thread_mode = EVAS_GL_THREAD_MODE_FINISH;

   EVGL_API_Thread_Command_glUniform1i thread_data_local;
   EVGL_API_Thread_Command_glUniform1i *thread_data = &thread_data_local;

   /* command_allocated flag init. */
   thread_data->command_allocated = 0;

   if (!evas_gl_thread_force_finish())
     { /* _flush */
        EVGL_API_Thread_Command_glUniform1i *thread_data_new;
        thread_data_new = eina_mempool_malloc(_mp_command,
                                              sizeof(EVGL_API_Thread_Command_glUniform1i));
        if (thread_data_new)
          {
             thread_data = thread_data_new;
             thread_data->command_allocated = 1;
             thread_mode = EVAS_GL_THREAD_MODE_FLUSH;
          }
     }

   thread_data->location = location;
   thread_data->x = x;

   evas_gl_thread_cmd_enqueue(EVAS_GL_THREAD_TYPE_EVGL,
                              _evgl_api_thread_glUniform1i,
                              thread_data,
                              thread_mode);
}

/*
 * void
 * glUniform1iv(GLint location, GLsizei count, const GLint* v);
 */

typedef struct
{
   GLint location;
   GLsizei count;
   const GLint* v;
   void *v_copied; /* COPIED */
   int command_allocated;

} EVGL_API_Thread_Command_glUniform1iv;

void (*orig_evgl_api_glUniform1iv)(GLint location, GLsizei count, const GLint* v);

static void
_evgl_api_thread_glUniform1iv(void *data)
{
   EVGL_API_Thread_Command_glUniform1iv *thread_data =
      (EVGL_API_Thread_Command_glUniform1iv *)data;

   orig_evgl_api_glUniform1iv(thread_data->location,
                              thread_data->count,
                              thread_data->v);


   if (thread_data->v_copied)
     eina_mempool_free(_mp_uniform, thread_data->v_copied);

   if (thread_data->command_allocated)
     eina_mempool_free(_mp_command, thread_data);
}

EAPI void
glUniform1iv_evgl_api_thread_cmd(GLint location, GLsizei count, const GLint* v)
{
   if (!evas_evgl_thread_enabled() || (_main_thread_id != eina_thread_self()))
     {
        orig_evgl_api_glUniform1iv(location, count, v);
        return;
     }

   int thread_mode = EVAS_GL_THREAD_MODE_FINISH;

   EVGL_API_Thread_Command_glUniform1iv thread_data_local;
   EVGL_API_Thread_Command_glUniform1iv *thread_data = &thread_data_local;

   /* command_allocated flag init. */
   thread_data->command_allocated = 0;

   if (!evas_gl_thread_force_finish())
     { /* _flush */
        EVGL_API_Thread_Command_glUniform1iv *thread_data_new;
        thread_data_new = eina_mempool_malloc(_mp_command,
                                              sizeof(EVGL_API_Thread_Command_glUniform1iv));
        if (thread_data_new)
          {
             thread_data = thread_data_new;
             thread_data->command_allocated = 1;
             thread_mode = EVAS_GL_THREAD_MODE_FLUSH;
          }
     }

   thread_data->location = location;
   thread_data->count = count;
   thread_data->v = v;

   thread_data->v_copied = NULL;
   if (thread_mode == EVAS_GL_THREAD_MODE_FINISH)
     goto finish;

   /* copy variable */
   if (v)
     {
        /* 1. check memory size */
        unsigned int copy_size = 1 * count * sizeof(GLint);
        if (copy_size > _mp_uniform_memory_size)
          {
             thread_mode = EVAS_GL_THREAD_MODE_FINISH;
             goto finish;
          }
        /* 2. malloc & copy */
        thread_data->v_copied = eina_mempool_malloc(_mp_uniform, copy_size);
        if (thread_data->v_copied)
          {
             memcpy(thread_data->v_copied, v, copy_size);
          }
        else
          {
             thread_mode = EVAS_GL_THREAD_MODE_FINISH;
             goto finish;
          }
        /* 3. replace */
        thread_data->v = (const GLint *)thread_data->v_copied;
     }
   /* end of copy variable */

finish:
   evas_gl_thread_cmd_enqueue(EVAS_GL_THREAD_TYPE_EVGL,
                              _evgl_api_thread_glUniform1iv,
                              thread_data,
                              thread_mode);
}

/*
 * void
 * glUniform2f(GLint location, GLfloat x, GLfloat y);
 */

typedef struct
{
   GLint location;
   GLfloat x;
   GLfloat y;
   int command_allocated;

} EVGL_API_Thread_Command_glUniform2f;

void (*orig_evgl_api_glUniform2f)(GLint location, GLfloat x, GLfloat y);

static void
_evgl_api_thread_glUniform2f(void *data)
{
   EVGL_API_Thread_Command_glUniform2f *thread_data =
      (EVGL_API_Thread_Command_glUniform2f *)data;

   orig_evgl_api_glUniform2f(thread_data->location,
                             thread_data->x,
                             thread_data->y);

   if (thread_data->command_allocated)
     eina_mempool_free(_mp_command, thread_data);
}

EAPI void
glUniform2f_evgl_api_thread_cmd(GLint location, GLfloat x, GLfloat y)
{
   if (!evas_evgl_thread_enabled() || (_main_thread_id != eina_thread_self()))
     {
        orig_evgl_api_glUniform2f(location, x, y);
        return;
     }

   int thread_mode = EVAS_GL_THREAD_MODE_FINISH;

   EVGL_API_Thread_Command_glUniform2f thread_data_local;
   EVGL_API_Thread_Command_glUniform2f *thread_data = &thread_data_local;

   /* command_allocated flag init. */
   thread_data->command_allocated = 0;

   if (!evas_gl_thread_force_finish())
     { /* _flush */
        EVGL_API_Thread_Command_glUniform2f *thread_data_new;
        thread_data_new = eina_mempool_malloc(_mp_command,
                                              sizeof(EVGL_API_Thread_Command_glUniform2f));
        if (thread_data_new)
          {
             thread_data = thread_data_new;
             thread_data->command_allocated = 1;
             thread_mode = EVAS_GL_THREAD_MODE_FLUSH;
          }
     }

   thread_data->location = location;
   thread_data->x = x;
   thread_data->y = y;

   evas_gl_thread_cmd_enqueue(EVAS_GL_THREAD_TYPE_EVGL,
                              _evgl_api_thread_glUniform2f,
                              thread_data,
                              thread_mode);
}

/*
 * void
 * glUniform2fv(GLint location, GLsizei count, const GLfloat* v);
 */

typedef struct
{
   GLint location;
   GLsizei count;
   const GLfloat* v;
   void *v_copied; /* COPIED */
   int command_allocated;

} EVGL_API_Thread_Command_glUniform2fv;

void (*orig_evgl_api_glUniform2fv)(GLint location, GLsizei count, const GLfloat* v);

static void
_evgl_api_thread_glUniform2fv(void *data)
{
   EVGL_API_Thread_Command_glUniform2fv *thread_data =
      (EVGL_API_Thread_Command_glUniform2fv *)data;

   orig_evgl_api_glUniform2fv(thread_data->location,
                              thread_data->count,
                              thread_data->v);


   if (thread_data->v_copied)
     eina_mempool_free(_mp_uniform, thread_data->v_copied);

   if (thread_data->command_allocated)
     eina_mempool_free(_mp_command, thread_data);
}

EAPI void
glUniform2fv_evgl_api_thread_cmd(GLint location, GLsizei count, const GLfloat* v)
{
   if (!evas_evgl_thread_enabled() || (_main_thread_id != eina_thread_self()))
     {
        orig_evgl_api_glUniform2fv(location, count, v);
        return;
     }

   int thread_mode = EVAS_GL_THREAD_MODE_FINISH;

   EVGL_API_Thread_Command_glUniform2fv thread_data_local;
   EVGL_API_Thread_Command_glUniform2fv *thread_data = &thread_data_local;

   /* command_allocated flag init. */
   thread_data->command_allocated = 0;

   if (!evas_gl_thread_force_finish())
     { /* _flush */
        EVGL_API_Thread_Command_glUniform2fv *thread_data_new;
        thread_data_new = eina_mempool_malloc(_mp_command,
                                              sizeof(EVGL_API_Thread_Command_glUniform2fv));
        if (thread_data_new)
          {
             thread_data = thread_data_new;
             thread_data->command_allocated = 1;
             thread_mode = EVAS_GL_THREAD_MODE_FLUSH;
          }
     }

   thread_data->location = location;
   thread_data->count = count;
   thread_data->v = v;

   thread_data->v_copied = NULL;
   if (thread_mode == EVAS_GL_THREAD_MODE_FINISH)
     goto finish;

   /* copy variable */
   if (v)
     {
        /* 1. check memory size */
        unsigned int copy_size = 2 * count * sizeof(GLfloat);
        if (copy_size > _mp_uniform_memory_size)
          {
             thread_mode = EVAS_GL_THREAD_MODE_FINISH;
             goto finish;
          }
        /* 2. malloc & copy */
        thread_data->v_copied = eina_mempool_malloc(_mp_uniform, copy_size);
        if (thread_data->v_copied)
          {
             memcpy(thread_data->v_copied, v, copy_size);
          }
        else
          {
             thread_mode = EVAS_GL_THREAD_MODE_FINISH;
             goto finish;
          }
        /* 3. replace */
        thread_data->v = (const GLfloat *)thread_data->v_copied;
     }
   /* end of copy variable */

finish:
   evas_gl_thread_cmd_enqueue(EVAS_GL_THREAD_TYPE_EVGL,
                              _evgl_api_thread_glUniform2fv,
                              thread_data,
                              thread_mode);
}

/*
 * void
 * glUniform2i(GLint location, GLint x, GLint y);
 */

typedef struct
{
   GLint location;
   GLint x;
   GLint y;
   int command_allocated;

} EVGL_API_Thread_Command_glUniform2i;

void (*orig_evgl_api_glUniform2i)(GLint location, GLint x, GLint y);

static void
_evgl_api_thread_glUniform2i(void *data)
{
   EVGL_API_Thread_Command_glUniform2i *thread_data =
      (EVGL_API_Thread_Command_glUniform2i *)data;

   orig_evgl_api_glUniform2i(thread_data->location,
                             thread_data->x,
                             thread_data->y);

   if (thread_data->command_allocated)
     eina_mempool_free(_mp_command, thread_data);
}

EAPI void
glUniform2i_evgl_api_thread_cmd(GLint location, GLint x, GLint y)
{
   if (!evas_evgl_thread_enabled() || (_main_thread_id != eina_thread_self()))
     {
        orig_evgl_api_glUniform2i(location, x, y);
        return;
     }

   int thread_mode = EVAS_GL_THREAD_MODE_FINISH;

   EVGL_API_Thread_Command_glUniform2i thread_data_local;
   EVGL_API_Thread_Command_glUniform2i *thread_data = &thread_data_local;

   /* command_allocated flag init. */
   thread_data->command_allocated = 0;

   if (!evas_gl_thread_force_finish())
     { /* _flush */
        EVGL_API_Thread_Command_glUniform2i *thread_data_new;
        thread_data_new = eina_mempool_malloc(_mp_command,
                                              sizeof(EVGL_API_Thread_Command_glUniform2i));
        if (thread_data_new)
          {
             thread_data = thread_data_new;
             thread_data->command_allocated = 1;
             thread_mode = EVAS_GL_THREAD_MODE_FLUSH;
          }
     }

   thread_data->location = location;
   thread_data->x = x;
   thread_data->y = y;

   evas_gl_thread_cmd_enqueue(EVAS_GL_THREAD_TYPE_EVGL,
                              _evgl_api_thread_glUniform2i,
                              thread_data,
                              thread_mode);
}

/*
 * void
 * glUniform2iv(GLint location, GLsizei count, const GLint* v);
 */

typedef struct
{
   GLint location;
   GLsizei count;
   const GLint* v;
   void *v_copied; /* COPIED */
   int command_allocated;

} EVGL_API_Thread_Command_glUniform2iv;

void (*orig_evgl_api_glUniform2iv)(GLint location, GLsizei count, const GLint* v);

static void
_evgl_api_thread_glUniform2iv(void *data)
{
   EVGL_API_Thread_Command_glUniform2iv *thread_data =
      (EVGL_API_Thread_Command_glUniform2iv *)data;

   orig_evgl_api_glUniform2iv(thread_data->location,
                              thread_data->count,
                              thread_data->v);


   if (thread_data->v_copied)
     eina_mempool_free(_mp_uniform, thread_data->v_copied);

   if (thread_data->command_allocated)
     eina_mempool_free(_mp_command, thread_data);
}

EAPI void
glUniform2iv_evgl_api_thread_cmd(GLint location, GLsizei count, const GLint* v)
{
   if (!evas_evgl_thread_enabled() || (_main_thread_id != eina_thread_self()))
     {
        orig_evgl_api_glUniform2iv(location, count, v);
        return;
     }

   int thread_mode = EVAS_GL_THREAD_MODE_FINISH;

   EVGL_API_Thread_Command_glUniform2iv thread_data_local;
   EVGL_API_Thread_Command_glUniform2iv *thread_data = &thread_data_local;

   /* command_allocated flag init. */
   thread_data->command_allocated = 0;

   if (!evas_gl_thread_force_finish())
     { /* _flush */
        EVGL_API_Thread_Command_glUniform2iv *thread_data_new;
        thread_data_new = eina_mempool_malloc(_mp_command,
                                              sizeof(EVGL_API_Thread_Command_glUniform2iv));
        if (thread_data_new)
          {
             thread_data = thread_data_new;
             thread_data->command_allocated = 1;
             thread_mode = EVAS_GL_THREAD_MODE_FLUSH;
          }
     }

   thread_data->location = location;
   thread_data->count = count;
   thread_data->v = v;

   thread_data->v_copied = NULL;
   if (thread_mode == EVAS_GL_THREAD_MODE_FINISH)
     goto finish;

   /* copy variable */
   if (v)
     {
        /* 1. check memory size */
        unsigned int copy_size = 2 * count * sizeof(GLint);
        if (copy_size > _mp_uniform_memory_size)
          {
             thread_mode = EVAS_GL_THREAD_MODE_FINISH;
             goto finish;
          }
        /* 2. malloc & copy */
        thread_data->v_copied = eina_mempool_malloc(_mp_uniform, copy_size);
        if (thread_data->v_copied)
          {
             memcpy(thread_data->v_copied, v, copy_size);
          }
        else
          {
             thread_mode = EVAS_GL_THREAD_MODE_FINISH;
             goto finish;
          }
        /* 3. replace */
        thread_data->v = (const GLint *)thread_data->v_copied;
     }
   /* end of copy variable */

finish:
   evas_gl_thread_cmd_enqueue(EVAS_GL_THREAD_TYPE_EVGL,
                              _evgl_api_thread_glUniform2iv,
                              thread_data,
                              thread_mode);
}

/*
 * void
 * glUniform3f(GLint location, GLfloat x, GLfloat y, GLfloat z);
 */

typedef struct
{
   GLint location;
   GLfloat x;
   GLfloat y;
   GLfloat z;
   int command_allocated;

} EVGL_API_Thread_Command_glUniform3f;

void (*orig_evgl_api_glUniform3f)(GLint location, GLfloat x, GLfloat y, GLfloat z);

static void
_evgl_api_thread_glUniform3f(void *data)
{
   EVGL_API_Thread_Command_glUniform3f *thread_data =
      (EVGL_API_Thread_Command_glUniform3f *)data;

   orig_evgl_api_glUniform3f(thread_data->location,
                             thread_data->x,
                             thread_data->y,
                             thread_data->z);

   if (thread_data->command_allocated)
     eina_mempool_free(_mp_command, thread_data);
}

EAPI void
glUniform3f_evgl_api_thread_cmd(GLint location, GLfloat x, GLfloat y, GLfloat z)
{
   if (!evas_evgl_thread_enabled() || (_main_thread_id != eina_thread_self()))
     {
        orig_evgl_api_glUniform3f(location, x, y, z);
        return;
     }

   int thread_mode = EVAS_GL_THREAD_MODE_FINISH;

   EVGL_API_Thread_Command_glUniform3f thread_data_local;
   EVGL_API_Thread_Command_glUniform3f *thread_data = &thread_data_local;

   /* command_allocated flag init. */
   thread_data->command_allocated = 0;

   if (!evas_gl_thread_force_finish())
     { /* _flush */
        EVGL_API_Thread_Command_glUniform3f *thread_data_new;
        thread_data_new = eina_mempool_malloc(_mp_command,
                                              sizeof(EVGL_API_Thread_Command_glUniform3f));
        if (thread_data_new)
          {
             thread_data = thread_data_new;
             thread_data->command_allocated = 1;
             thread_mode = EVAS_GL_THREAD_MODE_FLUSH;
          }
     }

   thread_data->location = location;
   thread_data->x = x;
   thread_data->y = y;
   thread_data->z = z;

   evas_gl_thread_cmd_enqueue(EVAS_GL_THREAD_TYPE_EVGL,
                              _evgl_api_thread_glUniform3f,
                              thread_data,
                              thread_mode);
}

/*
 * void
 * glUniform3fv(GLint location, GLsizei count, const GLfloat* v);
 */

typedef struct
{
   GLint location;
   GLsizei count;
   const GLfloat* v;
   void *v_copied; /* COPIED */
   int command_allocated;

} EVGL_API_Thread_Command_glUniform3fv;

void (*orig_evgl_api_glUniform3fv)(GLint location, GLsizei count, const GLfloat* v);

static void
_evgl_api_thread_glUniform3fv(void *data)
{
   EVGL_API_Thread_Command_glUniform3fv *thread_data =
      (EVGL_API_Thread_Command_glUniform3fv *)data;

   orig_evgl_api_glUniform3fv(thread_data->location,
                              thread_data->count,
                              thread_data->v);


   if (thread_data->v_copied)
     eina_mempool_free(_mp_uniform, thread_data->v_copied);

   if (thread_data->command_allocated)
     eina_mempool_free(_mp_command, thread_data);
}

EAPI void
glUniform3fv_evgl_api_thread_cmd(GLint location, GLsizei count, const GLfloat* v)
{
   if (!evas_evgl_thread_enabled() || (_main_thread_id != eina_thread_self()))
     {
        orig_evgl_api_glUniform3fv(location, count, v);
        return;
     }

   int thread_mode = EVAS_GL_THREAD_MODE_FINISH;

   EVGL_API_Thread_Command_glUniform3fv thread_data_local;
   EVGL_API_Thread_Command_glUniform3fv *thread_data = &thread_data_local;

   /* command_allocated flag init. */
   thread_data->command_allocated = 0;

   if (!evas_gl_thread_force_finish())
     { /* _flush */
        EVGL_API_Thread_Command_glUniform3fv *thread_data_new;
        thread_data_new = eina_mempool_malloc(_mp_command,
                                              sizeof(EVGL_API_Thread_Command_glUniform3fv));
        if (thread_data_new)
          {
             thread_data = thread_data_new;
             thread_data->command_allocated = 1;
             thread_mode = EVAS_GL_THREAD_MODE_FLUSH;
          }
     }

   thread_data->location = location;
   thread_data->count = count;
   thread_data->v = v;

   thread_data->v_copied = NULL;
   if (thread_mode == EVAS_GL_THREAD_MODE_FINISH)
     goto finish;

   /* copy variable */
   if (v)
     {
        /* 1. check memory size */
        unsigned int copy_size = 3 * count * sizeof(GLfloat);
        if (copy_size > _mp_uniform_memory_size)
          {
             thread_mode = EVAS_GL_THREAD_MODE_FINISH;
             goto finish;
          }
        /* 2. malloc & copy */
        thread_data->v_copied = eina_mempool_malloc(_mp_uniform, copy_size);
        if (thread_data->v_copied)
          {
             memcpy(thread_data->v_copied, v, copy_size);
          }
        else
          {
             thread_mode = EVAS_GL_THREAD_MODE_FINISH;
             goto finish;
          }
        /* 3. replace */
        thread_data->v = (const GLfloat *)thread_data->v_copied;
     }
   /* end of copy variable */

finish:
   evas_gl_thread_cmd_enqueue(EVAS_GL_THREAD_TYPE_EVGL,
                              _evgl_api_thread_glUniform3fv,
                              thread_data,
                              thread_mode);
}

/*
 * void
 * glUniform3i(GLint location, GLint x, GLint y, GLint z);
 */

typedef struct
{
   GLint location;
   GLint x;
   GLint y;
   GLint z;
   int command_allocated;

} EVGL_API_Thread_Command_glUniform3i;

void (*orig_evgl_api_glUniform3i)(GLint location, GLint x, GLint y, GLint z);

static void
_evgl_api_thread_glUniform3i(void *data)
{
   EVGL_API_Thread_Command_glUniform3i *thread_data =
      (EVGL_API_Thread_Command_glUniform3i *)data;

   orig_evgl_api_glUniform3i(thread_data->location,
                             thread_data->x,
                             thread_data->y,
                             thread_data->z);

   if (thread_data->command_allocated)
     eina_mempool_free(_mp_command, thread_data);
}

EAPI void
glUniform3i_evgl_api_thread_cmd(GLint location, GLint x, GLint y, GLint z)
{
   if (!evas_evgl_thread_enabled() || (_main_thread_id != eina_thread_self()))
     {
        orig_evgl_api_glUniform3i(location, x, y, z);
        return;
     }

   int thread_mode = EVAS_GL_THREAD_MODE_FINISH;

   EVGL_API_Thread_Command_glUniform3i thread_data_local;
   EVGL_API_Thread_Command_glUniform3i *thread_data = &thread_data_local;

   /* command_allocated flag init. */
   thread_data->command_allocated = 0;

   if (!evas_gl_thread_force_finish())
     { /* _flush */
        EVGL_API_Thread_Command_glUniform3i *thread_data_new;
        thread_data_new = eina_mempool_malloc(_mp_command,
                                              sizeof(EVGL_API_Thread_Command_glUniform3i));
        if (thread_data_new)
          {
             thread_data = thread_data_new;
             thread_data->command_allocated = 1;
             thread_mode = EVAS_GL_THREAD_MODE_FLUSH;
          }
     }

   thread_data->location = location;
   thread_data->x = x;
   thread_data->y = y;
   thread_data->z = z;

   evas_gl_thread_cmd_enqueue(EVAS_GL_THREAD_TYPE_EVGL,
                              _evgl_api_thread_glUniform3i,
                              thread_data,
                              thread_mode);
}

/*
 * void
 * glUniform3iv(GLint location, GLsizei count, const GLint* v);
 */

typedef struct
{
   GLint location;
   GLsizei count;
   const GLint* v;
   void *v_copied; /* COPIED */
   int command_allocated;

} EVGL_API_Thread_Command_glUniform3iv;

void (*orig_evgl_api_glUniform3iv)(GLint location, GLsizei count, const GLint* v);

static void
_evgl_api_thread_glUniform3iv(void *data)
{
   EVGL_API_Thread_Command_glUniform3iv *thread_data =
      (EVGL_API_Thread_Command_glUniform3iv *)data;

   orig_evgl_api_glUniform3iv(thread_data->location,
                              thread_data->count,
                              thread_data->v);


   if (thread_data->v_copied)
     eina_mempool_free(_mp_uniform, thread_data->v_copied);

   if (thread_data->command_allocated)
     eina_mempool_free(_mp_command, thread_data);
}

EAPI void
glUniform3iv_evgl_api_thread_cmd(GLint location, GLsizei count, const GLint* v)
{
   if (!evas_evgl_thread_enabled() || (_main_thread_id != eina_thread_self()))
     {
        orig_evgl_api_glUniform3iv(location, count, v);
        return;
     }

   int thread_mode = EVAS_GL_THREAD_MODE_FINISH;

   EVGL_API_Thread_Command_glUniform3iv thread_data_local;
   EVGL_API_Thread_Command_glUniform3iv *thread_data = &thread_data_local;

   /* command_allocated flag init. */
   thread_data->command_allocated = 0;

   if (!evas_gl_thread_force_finish())
     { /* _flush */
        EVGL_API_Thread_Command_glUniform3iv *thread_data_new;
        thread_data_new = eina_mempool_malloc(_mp_command,
                                              sizeof(EVGL_API_Thread_Command_glUniform3iv));
        if (thread_data_new)
          {
             thread_data = thread_data_new;
             thread_data->command_allocated = 1;
             thread_mode = EVAS_GL_THREAD_MODE_FLUSH;
          }
     }

   thread_data->location = location;
   thread_data->count = count;
   thread_data->v = v;

   thread_data->v_copied = NULL;
   if (thread_mode == EVAS_GL_THREAD_MODE_FINISH)
     goto finish;

   /* copy variable */
   if (v)
     {
        /* 1. check memory size */
        unsigned int copy_size = 3 * count * sizeof(GLint);
        if (copy_size > _mp_uniform_memory_size)
          {
             thread_mode = EVAS_GL_THREAD_MODE_FINISH;
             goto finish;
          }
        /* 2. malloc & copy */
        thread_data->v_copied = eina_mempool_malloc(_mp_uniform, copy_size);
        if (thread_data->v_copied)
          {
             memcpy(thread_data->v_copied, v, copy_size);
          }
        else
          {
             thread_mode = EVAS_GL_THREAD_MODE_FINISH;
             goto finish;
          }
        /* 3. replace */
        thread_data->v = (const GLint *)thread_data->v_copied;
     }
   /* end of copy variable */

finish:
   evas_gl_thread_cmd_enqueue(EVAS_GL_THREAD_TYPE_EVGL,
                              _evgl_api_thread_glUniform3iv,
                              thread_data,
                              thread_mode);
}

/*
 * void
 * glUniform4f(GLint location, GLfloat x, GLfloat y, GLfloat z, GLfloat w);
 */

typedef struct
{
   GLint location;
   GLfloat x;
   GLfloat y;
   GLfloat z;
   GLfloat w;
   int command_allocated;

} EVGL_API_Thread_Command_glUniform4f;

void (*orig_evgl_api_glUniform4f)(GLint location, GLfloat x, GLfloat y, GLfloat z, GLfloat w);

static void
_evgl_api_thread_glUniform4f(void *data)
{
   EVGL_API_Thread_Command_glUniform4f *thread_data =
      (EVGL_API_Thread_Command_glUniform4f *)data;

   orig_evgl_api_glUniform4f(thread_data->location,
                             thread_data->x,
                             thread_data->y,
                             thread_data->z,
                             thread_data->w);

   if (thread_data->command_allocated)
     eina_mempool_free(_mp_command, thread_data);
}

EAPI void
glUniform4f_evgl_api_thread_cmd(GLint location, GLfloat x, GLfloat y, GLfloat z, GLfloat w)
{
   if (!evas_evgl_thread_enabled() || (_main_thread_id != eina_thread_self()))
     {
        orig_evgl_api_glUniform4f(location, x, y, z, w);
        return;
     }

   int thread_mode = EVAS_GL_THREAD_MODE_FINISH;

   EVGL_API_Thread_Command_glUniform4f thread_data_local;
   EVGL_API_Thread_Command_glUniform4f *thread_data = &thread_data_local;

   /* command_allocated flag init. */
   thread_data->command_allocated = 0;

   if (!evas_gl_thread_force_finish())
     { /* _flush */
        EVGL_API_Thread_Command_glUniform4f *thread_data_new;
        thread_data_new = eina_mempool_malloc(_mp_command,
                                              sizeof(EVGL_API_Thread_Command_glUniform4f));
        if (thread_data_new)
          {
             thread_data = thread_data_new;
             thread_data->command_allocated = 1;
             thread_mode = EVAS_GL_THREAD_MODE_FLUSH;
          }
     }

   thread_data->location = location;
   thread_data->x = x;
   thread_data->y = y;
   thread_data->z = z;
   thread_data->w = w;

   evas_gl_thread_cmd_enqueue(EVAS_GL_THREAD_TYPE_EVGL,
                              _evgl_api_thread_glUniform4f,
                              thread_data,
                              thread_mode);
}

/*
 * void
 * glUniform4fv(GLint location, GLsizei count, const GLfloat* v);
 */

typedef struct
{
   GLint location;
   GLsizei count;
   const GLfloat* v;
   void *v_copied; /* COPIED */
   int command_allocated;

} EVGL_API_Thread_Command_glUniform4fv;

void (*orig_evgl_api_glUniform4fv)(GLint location, GLsizei count, const GLfloat* v);

static void
_evgl_api_thread_glUniform4fv(void *data)
{
   EVGL_API_Thread_Command_glUniform4fv *thread_data =
      (EVGL_API_Thread_Command_glUniform4fv *)data;

   orig_evgl_api_glUniform4fv(thread_data->location,
                              thread_data->count,
                              thread_data->v);


   if (thread_data->v_copied)
     eina_mempool_free(_mp_uniform, thread_data->v_copied);

   if (thread_data->command_allocated)
     eina_mempool_free(_mp_command, thread_data);
}

EAPI void
glUniform4fv_evgl_api_thread_cmd(GLint location, GLsizei count, const GLfloat* v)
{
   if (!evas_evgl_thread_enabled() || (_main_thread_id != eina_thread_self()))
     {
        orig_evgl_api_glUniform4fv(location, count, v);
        return;
     }

   int thread_mode = EVAS_GL_THREAD_MODE_FINISH;

   EVGL_API_Thread_Command_glUniform4fv thread_data_local;
   EVGL_API_Thread_Command_glUniform4fv *thread_data = &thread_data_local;

   /* command_allocated flag init. */
   thread_data->command_allocated = 0;

   if (!evas_gl_thread_force_finish())
     { /* _flush */
        EVGL_API_Thread_Command_glUniform4fv *thread_data_new;
        thread_data_new = eina_mempool_malloc(_mp_command,
                                              sizeof(EVGL_API_Thread_Command_glUniform4fv));
        if (thread_data_new)
          {
             thread_data = thread_data_new;
             thread_data->command_allocated = 1;
             thread_mode = EVAS_GL_THREAD_MODE_FLUSH;
          }
     }

   thread_data->location = location;
   thread_data->count = count;
   thread_data->v = v;

   thread_data->v_copied = NULL;
   if (thread_mode == EVAS_GL_THREAD_MODE_FINISH)
     goto finish;

   /* copy variable */
   if (v)
     {
        /* 1. check memory size */
        unsigned int copy_size = 4 * count * sizeof(GLfloat);
        if (copy_size > _mp_uniform_memory_size)
          {
             thread_mode = EVAS_GL_THREAD_MODE_FINISH;
             goto finish;
          }
        /* 2. malloc & copy */
        thread_data->v_copied = eina_mempool_malloc(_mp_uniform, copy_size);
        if (thread_data->v_copied)
          {
             memcpy(thread_data->v_copied, v, copy_size);
          }
        else
          {
             thread_mode = EVAS_GL_THREAD_MODE_FINISH;
             goto finish;
          }
        /* 3. replace */
        thread_data->v = (const GLfloat *)thread_data->v_copied;
     }
   /* end of copy variable */

finish:
   evas_gl_thread_cmd_enqueue(EVAS_GL_THREAD_TYPE_EVGL,
                              _evgl_api_thread_glUniform4fv,
                              thread_data,
                              thread_mode);
}

/*
 * void
 * glUniform4i(GLint location, GLint x, GLint y, GLint z, GLint w);
 */

typedef struct
{
   GLint location;
   GLint x;
   GLint y;
   GLint z;
   GLint w;
   int command_allocated;

} EVGL_API_Thread_Command_glUniform4i;

void (*orig_evgl_api_glUniform4i)(GLint location, GLint x, GLint y, GLint z, GLint w);

static void
_evgl_api_thread_glUniform4i(void *data)
{
   EVGL_API_Thread_Command_glUniform4i *thread_data =
      (EVGL_API_Thread_Command_glUniform4i *)data;

   orig_evgl_api_glUniform4i(thread_data->location,
                             thread_data->x,
                             thread_data->y,
                             thread_data->z,
                             thread_data->w);

   if (thread_data->command_allocated)
     eina_mempool_free(_mp_command, thread_data);
}

EAPI void
glUniform4i_evgl_api_thread_cmd(GLint location, GLint x, GLint y, GLint z, GLint w)
{
   if (!evas_evgl_thread_enabled() || (_main_thread_id != eina_thread_self()))
     {
        orig_evgl_api_glUniform4i(location, x, y, z, w);
        return;
     }

   int thread_mode = EVAS_GL_THREAD_MODE_FINISH;

   EVGL_API_Thread_Command_glUniform4i thread_data_local;
   EVGL_API_Thread_Command_glUniform4i *thread_data = &thread_data_local;

   /* command_allocated flag init. */
   thread_data->command_allocated = 0;

   if (!evas_gl_thread_force_finish())
     { /* _flush */
        EVGL_API_Thread_Command_glUniform4i *thread_data_new;
        thread_data_new = eina_mempool_malloc(_mp_command,
                                              sizeof(EVGL_API_Thread_Command_glUniform4i));
        if (thread_data_new)
          {
             thread_data = thread_data_new;
             thread_data->command_allocated = 1;
             thread_mode = EVAS_GL_THREAD_MODE_FLUSH;
          }
     }

   thread_data->location = location;
   thread_data->x = x;
   thread_data->y = y;
   thread_data->z = z;
   thread_data->w = w;

   evas_gl_thread_cmd_enqueue(EVAS_GL_THREAD_TYPE_EVGL,
                              _evgl_api_thread_glUniform4i,
                              thread_data,
                              thread_mode);
}

/*
 * void
 * glUniform4iv(GLint location, GLsizei count, const GLint* v);
 */

typedef struct
{
   GLint location;
   GLsizei count;
   const GLint* v;
   void *v_copied; /* COPIED */
   int command_allocated;

} EVGL_API_Thread_Command_glUniform4iv;

void (*orig_evgl_api_glUniform4iv)(GLint location, GLsizei count, const GLint* v);

static void
_evgl_api_thread_glUniform4iv(void *data)
{
   EVGL_API_Thread_Command_glUniform4iv *thread_data =
      (EVGL_API_Thread_Command_glUniform4iv *)data;

   orig_evgl_api_glUniform4iv(thread_data->location,
                              thread_data->count,
                              thread_data->v);


   if (thread_data->v_copied)
     eina_mempool_free(_mp_uniform, thread_data->v_copied);

   if (thread_data->command_allocated)
     eina_mempool_free(_mp_command, thread_data);
}

EAPI void
glUniform4iv_evgl_api_thread_cmd(GLint location, GLsizei count, const GLint* v)
{
   if (!evas_evgl_thread_enabled() || (_main_thread_id != eina_thread_self()))
     {
        orig_evgl_api_glUniform4iv(location, count, v);
        return;
     }

   int thread_mode = EVAS_GL_THREAD_MODE_FINISH;

   EVGL_API_Thread_Command_glUniform4iv thread_data_local;
   EVGL_API_Thread_Command_glUniform4iv *thread_data = &thread_data_local;

   /* command_allocated flag init. */
   thread_data->command_allocated = 0;

   if (!evas_gl_thread_force_finish())
     { /* _flush */
        EVGL_API_Thread_Command_glUniform4iv *thread_data_new;
        thread_data_new = eina_mempool_malloc(_mp_command,
                                              sizeof(EVGL_API_Thread_Command_glUniform4iv));
        if (thread_data_new)
          {
             thread_data = thread_data_new;
             thread_data->command_allocated = 1;
             thread_mode = EVAS_GL_THREAD_MODE_FLUSH;
          }
     }

   thread_data->location = location;
   thread_data->count = count;
   thread_data->v = v;

   thread_data->v_copied = NULL;
   if (thread_mode == EVAS_GL_THREAD_MODE_FINISH)
     goto finish;

   /* copy variable */
   if (v)
     {
        /* 1. check memory size */
        unsigned int copy_size = 4 * count * sizeof(GLint);
        if (copy_size > _mp_uniform_memory_size)
          {
             thread_mode = EVAS_GL_THREAD_MODE_FINISH;
             goto finish;
          }
        /* 2. malloc & copy */
        thread_data->v_copied = eina_mempool_malloc(_mp_uniform, copy_size);
        if (thread_data->v_copied)
          {
             memcpy(thread_data->v_copied, v, copy_size);
          }
        else
          {
             thread_mode = EVAS_GL_THREAD_MODE_FINISH;
             goto finish;
          }
        /* 3. replace */
        thread_data->v = (const GLint *)thread_data->v_copied;
     }
   /* end of copy variable */

finish:
   evas_gl_thread_cmd_enqueue(EVAS_GL_THREAD_TYPE_EVGL,
                              _evgl_api_thread_glUniform4iv,
                              thread_data,
                              thread_mode);
}

/*
 * void
 * glUniformMatrix2fv(GLint location, GLsizei count, GLboolean transpose, const GLfloat* value);
 */

typedef struct
{
   GLint location;
   GLsizei count;
   GLboolean transpose;
   const GLfloat* value;
   void *value_copied; /* COPIED */
   int command_allocated;

} EVGL_API_Thread_Command_glUniformMatrix2fv;

void (*orig_evgl_api_glUniformMatrix2fv)(GLint location, GLsizei count, GLboolean transpose, const GLfloat* value);

static void
_evgl_api_thread_glUniformMatrix2fv(void *data)
{
   EVGL_API_Thread_Command_glUniformMatrix2fv *thread_data =
      (EVGL_API_Thread_Command_glUniformMatrix2fv *)data;

   orig_evgl_api_glUniformMatrix2fv(thread_data->location,
                                    thread_data->count,
                                    thread_data->transpose,
                                    thread_data->value);


   if (thread_data->value_copied)
     eina_mempool_free(_mp_uniform, thread_data->value_copied);

   if (thread_data->command_allocated)
     eina_mempool_free(_mp_command, thread_data);
}

EAPI void
glUniformMatrix2fv_evgl_api_thread_cmd(GLint location, GLsizei count, GLboolean transpose, const GLfloat* value)
{
   if (!evas_evgl_thread_enabled() || (_main_thread_id != eina_thread_self()))
     {
        orig_evgl_api_glUniformMatrix2fv(location, count, transpose, value);
        return;
     }

   int thread_mode = EVAS_GL_THREAD_MODE_FINISH;

   EVGL_API_Thread_Command_glUniformMatrix2fv thread_data_local;
   EVGL_API_Thread_Command_glUniformMatrix2fv *thread_data = &thread_data_local;

   /* command_allocated flag init. */
   thread_data->command_allocated = 0;

   if (!evas_gl_thread_force_finish())
     { /* _flush */
        EVGL_API_Thread_Command_glUniformMatrix2fv *thread_data_new;
        thread_data_new = eina_mempool_malloc(_mp_command,
                                              sizeof(EVGL_API_Thread_Command_glUniformMatrix2fv));
        if (thread_data_new)
          {
             thread_data = thread_data_new;
             thread_data->command_allocated = 1;
             thread_mode = EVAS_GL_THREAD_MODE_FLUSH;
          }
     }

   thread_data->location = location;
   thread_data->count = count;
   thread_data->transpose = transpose;
   thread_data->value = value;

   thread_data->value_copied = NULL;
   if (thread_mode == EVAS_GL_THREAD_MODE_FINISH)
     goto finish;

   /* copy variable */
   if (value)
     {
        /* 1. check memory size */
        unsigned int copy_size = 4 * count * sizeof(GLfloat);
        if (copy_size > _mp_uniform_memory_size)
          {
             thread_mode = EVAS_GL_THREAD_MODE_FINISH;
             goto finish;
          }
        /* 2. malloc & copy */
        thread_data->value_copied = eina_mempool_malloc(_mp_uniform, copy_size);
        if (thread_data->value_copied)
          {
             memcpy(thread_data->value_copied, value, copy_size);
          }
        else
          {
             thread_mode = EVAS_GL_THREAD_MODE_FINISH;
             goto finish;
          }
        /* 3. replace */
        thread_data->value = (const GLfloat *)thread_data->value_copied;
     }
   /* end of copy variable */

finish:
   evas_gl_thread_cmd_enqueue(EVAS_GL_THREAD_TYPE_EVGL,
                              _evgl_api_thread_glUniformMatrix2fv,
                              thread_data,
                              thread_mode);
}

/*
 * void
 * glUniformMatrix3fv(GLint location, GLsizei count, GLboolean transpose, const GLfloat* value);
 */

typedef struct
{
   GLint location;
   GLsizei count;
   GLboolean transpose;
   const GLfloat* value;
   void *value_copied; /* COPIED */
   int command_allocated;

} EVGL_API_Thread_Command_glUniformMatrix3fv;

void (*orig_evgl_api_glUniformMatrix3fv)(GLint location, GLsizei count, GLboolean transpose, const GLfloat* value);

static void
_evgl_api_thread_glUniformMatrix3fv(void *data)
{
   EVGL_API_Thread_Command_glUniformMatrix3fv *thread_data =
      (EVGL_API_Thread_Command_glUniformMatrix3fv *)data;

   orig_evgl_api_glUniformMatrix3fv(thread_data->location,
                                    thread_data->count,
                                    thread_data->transpose,
                                    thread_data->value);


   if (thread_data->value_copied)
     eina_mempool_free(_mp_uniform, thread_data->value_copied);

   if (thread_data->command_allocated)
     eina_mempool_free(_mp_command, thread_data);
}

EAPI void
glUniformMatrix3fv_evgl_api_thread_cmd(GLint location, GLsizei count, GLboolean transpose, const GLfloat* value)
{
   if (!evas_evgl_thread_enabled() || (_main_thread_id != eina_thread_self()))
     {
        orig_evgl_api_glUniformMatrix3fv(location, count, transpose, value);
        return;
     }

   int thread_mode = EVAS_GL_THREAD_MODE_FINISH;

   EVGL_API_Thread_Command_glUniformMatrix3fv thread_data_local;
   EVGL_API_Thread_Command_glUniformMatrix3fv *thread_data = &thread_data_local;

   /* command_allocated flag init. */
   thread_data->command_allocated = 0;

   if (!evas_gl_thread_force_finish())
     { /* _flush */
        EVGL_API_Thread_Command_glUniformMatrix3fv *thread_data_new;
        thread_data_new = eina_mempool_malloc(_mp_command,
                                              sizeof(EVGL_API_Thread_Command_glUniformMatrix3fv));
        if (thread_data_new)
          {
             thread_data = thread_data_new;
             thread_data->command_allocated = 1;
             thread_mode = EVAS_GL_THREAD_MODE_FLUSH;
          }
     }

   thread_data->location = location;
   thread_data->count = count;
   thread_data->transpose = transpose;
   thread_data->value = value;

   thread_data->value_copied = NULL;
   if (thread_mode == EVAS_GL_THREAD_MODE_FINISH)
     goto finish;

   /* copy variable */
   if (value)
     {
        /* 1. check memory size */
        unsigned int copy_size = 9 * count * sizeof(GLfloat);
        if (copy_size > _mp_uniform_memory_size)
          {
             thread_mode = EVAS_GL_THREAD_MODE_FINISH;
             goto finish;
          }
        /* 2. malloc & copy */
        thread_data->value_copied = eina_mempool_malloc(_mp_uniform, copy_size);
        if (thread_data->value_copied)
          {
             memcpy(thread_data->value_copied, value, copy_size);
          }
        else
          {
             thread_mode = EVAS_GL_THREAD_MODE_FINISH;
             goto finish;
          }
        /* 3. replace */
        thread_data->value = (const GLfloat *)thread_data->value_copied;
     }
   /* end of copy variable */

finish:
   evas_gl_thread_cmd_enqueue(EVAS_GL_THREAD_TYPE_EVGL,
                              _evgl_api_thread_glUniformMatrix3fv,
                              thread_data,
                              thread_mode);
}

/*
 * void
 * glUniformMatrix4fv(GLint location, GLsizei count, GLboolean transpose, const GLfloat* value);
 */

typedef struct
{
   GLint location;
   GLsizei count;
   GLboolean transpose;
   const GLfloat* value;
   void *value_copied; /* COPIED */
   int command_allocated;

} EVGL_API_Thread_Command_glUniformMatrix4fv;

void (*orig_evgl_api_glUniformMatrix4fv)(GLint location, GLsizei count, GLboolean transpose, const GLfloat* value);

static void
_evgl_api_thread_glUniformMatrix4fv(void *data)
{
   EVGL_API_Thread_Command_glUniformMatrix4fv *thread_data =
      (EVGL_API_Thread_Command_glUniformMatrix4fv *)data;

   orig_evgl_api_glUniformMatrix4fv(thread_data->location,
                                    thread_data->count,
                                    thread_data->transpose,
                                    thread_data->value);


   if (thread_data->value_copied)
     eina_mempool_free(_mp_uniform, thread_data->value_copied);

   if (thread_data->command_allocated)
     eina_mempool_free(_mp_command, thread_data);
}

EAPI void
glUniformMatrix4fv_evgl_api_thread_cmd(GLint location, GLsizei count, GLboolean transpose, const GLfloat* value)
{
   if (!evas_evgl_thread_enabled() || (_main_thread_id != eina_thread_self()))
     {
        orig_evgl_api_glUniformMatrix4fv(location, count, transpose, value);
        return;
     }

   int thread_mode = EVAS_GL_THREAD_MODE_FINISH;

   EVGL_API_Thread_Command_glUniformMatrix4fv thread_data_local;
   EVGL_API_Thread_Command_glUniformMatrix4fv *thread_data = &thread_data_local;

   /* command_allocated flag init. */
   thread_data->command_allocated = 0;

   if (!evas_gl_thread_force_finish())
     { /* _flush */
        EVGL_API_Thread_Command_glUniformMatrix4fv *thread_data_new;
        thread_data_new = eina_mempool_malloc(_mp_command,
                                              sizeof(EVGL_API_Thread_Command_glUniformMatrix4fv));
        if (thread_data_new)
          {
             thread_data = thread_data_new;
             thread_data->command_allocated = 1;
             thread_mode = EVAS_GL_THREAD_MODE_FLUSH;
          }
     }

   thread_data->location = location;
   thread_data->count = count;
   thread_data->transpose = transpose;
   thread_data->value = value;

   thread_data->value_copied = NULL;
   if (thread_mode == EVAS_GL_THREAD_MODE_FINISH)
     goto finish;

   /* copy variable */
   if (value)
     {
        /* 1. check memory size */
        unsigned int copy_size = 16 * count * sizeof(GLfloat);
        if (copy_size > _mp_uniform_memory_size)
          {
             thread_mode = EVAS_GL_THREAD_MODE_FINISH;
             goto finish;
          }
        /* 2. malloc & copy */
        thread_data->value_copied = eina_mempool_malloc(_mp_uniform, copy_size);
        if (thread_data->value_copied)
          {
             memcpy(thread_data->value_copied, value, copy_size);
          }
        else
          {
             thread_mode = EVAS_GL_THREAD_MODE_FINISH;
             goto finish;
          }
        /* 3. replace */
        thread_data->value = (const GLfloat *)thread_data->value_copied;
     }
   /* end of copy variable */

finish:
   evas_gl_thread_cmd_enqueue(EVAS_GL_THREAD_TYPE_EVGL,
                              _evgl_api_thread_glUniformMatrix4fv,
                              thread_data,
                              thread_mode);
}

/*
 * void
 * glUseProgram(GLuint program);
 */

typedef struct
{
   GLuint program;
   int command_allocated;

} EVGL_API_Thread_Command_glUseProgram;

void (*orig_evgl_api_glUseProgram)(GLuint program);

static void
_evgl_api_thread_glUseProgram(void *data)
{
   EVGL_API_Thread_Command_glUseProgram *thread_data =
      (EVGL_API_Thread_Command_glUseProgram *)data;

   orig_evgl_api_glUseProgram(thread_data->program);

   if (thread_data->command_allocated)
     eina_mempool_free(_mp_command, thread_data);
}

EAPI void
glUseProgram_evgl_api_thread_cmd(GLuint program)
{
   if (!evas_evgl_thread_enabled() || (_main_thread_id != eina_thread_self()))
     {
        orig_evgl_api_glUseProgram(program);
        return;
     }

   int thread_mode = EVAS_GL_THREAD_MODE_FINISH;

   EVGL_API_Thread_Command_glUseProgram thread_data_local;
   EVGL_API_Thread_Command_glUseProgram *thread_data = &thread_data_local;

   /* command_allocated flag init. */
   thread_data->command_allocated = 0;

   if (!evas_gl_thread_force_finish())
     { /* _flush */
        EVGL_API_Thread_Command_glUseProgram *thread_data_new;
        thread_data_new = eina_mempool_malloc(_mp_command,
                                              sizeof(EVGL_API_Thread_Command_glUseProgram));
        if (thread_data_new)
          {
             thread_data = thread_data_new;
             thread_data->command_allocated = 1;
             thread_mode = EVAS_GL_THREAD_MODE_FLUSH;
          }
     }

   thread_data->program = program;

   evas_gl_thread_cmd_enqueue(EVAS_GL_THREAD_TYPE_EVGL,
                              _evgl_api_thread_glUseProgram,
                              thread_data,
                              thread_mode);
}

/*
 * void
 * glValidateProgram(GLuint program);
 */

typedef struct
{
   GLuint program;

} EVGL_API_Thread_Command_glValidateProgram;

void (*orig_evgl_api_glValidateProgram)(GLuint program);

static void
_evgl_api_thread_glValidateProgram(void *data)
{
   EVGL_API_Thread_Command_glValidateProgram *thread_data =
      (EVGL_API_Thread_Command_glValidateProgram *)data;

   orig_evgl_api_glValidateProgram(thread_data->program);

}

EAPI void
glValidateProgram_evgl_api_thread_cmd(GLuint program)
{
   if (!evas_evgl_thread_enabled() || (_main_thread_id != eina_thread_self()))
     {
        orig_evgl_api_glValidateProgram(program);
        return;
     }

   int thread_mode = EVAS_GL_THREAD_MODE_FINISH;

   EVGL_API_Thread_Command_glValidateProgram thread_data_local;
   EVGL_API_Thread_Command_glValidateProgram *thread_data = &thread_data_local;

   thread_data->program = program;

   evas_gl_thread_cmd_enqueue(EVAS_GL_THREAD_TYPE_EVGL,
                              _evgl_api_thread_glValidateProgram,
                              thread_data,
                              thread_mode);
}

/*
 * void
 * glVertexAttrib1f(GLuint indx, GLfloat x);
 */

typedef struct
{
   GLuint indx;
   GLfloat x;

} EVGL_API_Thread_Command_glVertexAttrib1f;

void (*orig_evgl_api_glVertexAttrib1f)(GLuint indx, GLfloat x);

static void
_evgl_api_thread_glVertexAttrib1f(void *data)
{
   EVGL_API_Thread_Command_glVertexAttrib1f *thread_data =
      (EVGL_API_Thread_Command_glVertexAttrib1f *)data;

   orig_evgl_api_glVertexAttrib1f(thread_data->indx,
                                  thread_data->x);

}

EAPI void
glVertexAttrib1f_evgl_api_thread_cmd(GLuint indx, GLfloat x)
{
   if (!evas_evgl_thread_enabled() || (_main_thread_id != eina_thread_self()))
     {
        orig_evgl_api_glVertexAttrib1f(indx, x);
        return;
     }

   int thread_mode = EVAS_GL_THREAD_MODE_FINISH;

   EVGL_API_Thread_Command_glVertexAttrib1f thread_data_local;
   EVGL_API_Thread_Command_glVertexAttrib1f *thread_data = &thread_data_local;

   thread_data->indx = indx;
   thread_data->x = x;

   evas_gl_thread_cmd_enqueue(EVAS_GL_THREAD_TYPE_EVGL,
                              _evgl_api_thread_glVertexAttrib1f,
                              thread_data,
                              thread_mode);
}

/*
 * void
 * glVertexAttrib1fv(GLuint indx, const GLfloat* values);
 */

typedef struct
{
   GLuint indx;
   const GLfloat* values;

} EVGL_API_Thread_Command_glVertexAttrib1fv;

void (*orig_evgl_api_glVertexAttrib1fv)(GLuint indx, const GLfloat* values);

static void
_evgl_api_thread_glVertexAttrib1fv(void *data)
{
   EVGL_API_Thread_Command_glVertexAttrib1fv *thread_data =
      (EVGL_API_Thread_Command_glVertexAttrib1fv *)data;

   orig_evgl_api_glVertexAttrib1fv(thread_data->indx,
                                   thread_data->values);

}

EAPI void
glVertexAttrib1fv_evgl_api_thread_cmd(GLuint indx, const GLfloat* values)
{
   if (!evas_evgl_thread_enabled() || (_main_thread_id != eina_thread_self()))
     {
        orig_evgl_api_glVertexAttrib1fv(indx, values);
        return;
     }

   int thread_mode = EVAS_GL_THREAD_MODE_FINISH;

   EVGL_API_Thread_Command_glVertexAttrib1fv thread_data_local;
   EVGL_API_Thread_Command_glVertexAttrib1fv *thread_data = &thread_data_local;

   thread_data->indx = indx;
   thread_data->values = values;

   evas_gl_thread_cmd_enqueue(EVAS_GL_THREAD_TYPE_EVGL,
                              _evgl_api_thread_glVertexAttrib1fv,
                              thread_data,
                              thread_mode);
}

/*
 * void
 * glVertexAttrib2f(GLuint indx, GLfloat x, GLfloat y);
 */

typedef struct
{
   GLuint indx;
   GLfloat x;
   GLfloat y;

} EVGL_API_Thread_Command_glVertexAttrib2f;

void (*orig_evgl_api_glVertexAttrib2f)(GLuint indx, GLfloat x, GLfloat y);

static void
_evgl_api_thread_glVertexAttrib2f(void *data)
{
   EVGL_API_Thread_Command_glVertexAttrib2f *thread_data =
      (EVGL_API_Thread_Command_glVertexAttrib2f *)data;

   orig_evgl_api_glVertexAttrib2f(thread_data->indx,
                                  thread_data->x,
                                  thread_data->y);

}

EAPI void
glVertexAttrib2f_evgl_api_thread_cmd(GLuint indx, GLfloat x, GLfloat y)
{
   if (!evas_evgl_thread_enabled() || (_main_thread_id != eina_thread_self()))
     {
        orig_evgl_api_glVertexAttrib2f(indx, x, y);
        return;
     }

   int thread_mode = EVAS_GL_THREAD_MODE_FINISH;

   EVGL_API_Thread_Command_glVertexAttrib2f thread_data_local;
   EVGL_API_Thread_Command_glVertexAttrib2f *thread_data = &thread_data_local;

   thread_data->indx = indx;
   thread_data->x = x;
   thread_data->y = y;

   evas_gl_thread_cmd_enqueue(EVAS_GL_THREAD_TYPE_EVGL,
                              _evgl_api_thread_glVertexAttrib2f,
                              thread_data,
                              thread_mode);
}

/*
 * void
 * glVertexAttrib2fv(GLuint indx, const GLfloat* values);
 */

typedef struct
{
   GLuint indx;
   const GLfloat* values;

} EVGL_API_Thread_Command_glVertexAttrib2fv;

void (*orig_evgl_api_glVertexAttrib2fv)(GLuint indx, const GLfloat* values);

static void
_evgl_api_thread_glVertexAttrib2fv(void *data)
{
   EVGL_API_Thread_Command_glVertexAttrib2fv *thread_data =
      (EVGL_API_Thread_Command_glVertexAttrib2fv *)data;

   orig_evgl_api_glVertexAttrib2fv(thread_data->indx,
                                   thread_data->values);

}

EAPI void
glVertexAttrib2fv_evgl_api_thread_cmd(GLuint indx, const GLfloat* values)
{
   if (!evas_evgl_thread_enabled() || (_main_thread_id != eina_thread_self()))
     {
        orig_evgl_api_glVertexAttrib2fv(indx, values);
        return;
     }

   int thread_mode = EVAS_GL_THREAD_MODE_FINISH;

   EVGL_API_Thread_Command_glVertexAttrib2fv thread_data_local;
   EVGL_API_Thread_Command_glVertexAttrib2fv *thread_data = &thread_data_local;

   thread_data->indx = indx;
   thread_data->values = values;

   evas_gl_thread_cmd_enqueue(EVAS_GL_THREAD_TYPE_EVGL,
                              _evgl_api_thread_glVertexAttrib2fv,
                              thread_data,
                              thread_mode);
}

/*
 * void
 * glVertexAttrib3f(GLuint indx, GLfloat x, GLfloat y, GLfloat z);
 */

typedef struct
{
   GLuint indx;
   GLfloat x;
   GLfloat y;
   GLfloat z;

} EVGL_API_Thread_Command_glVertexAttrib3f;

void (*orig_evgl_api_glVertexAttrib3f)(GLuint indx, GLfloat x, GLfloat y, GLfloat z);

static void
_evgl_api_thread_glVertexAttrib3f(void *data)
{
   EVGL_API_Thread_Command_glVertexAttrib3f *thread_data =
      (EVGL_API_Thread_Command_glVertexAttrib3f *)data;

   orig_evgl_api_glVertexAttrib3f(thread_data->indx,
                                  thread_data->x,
                                  thread_data->y,
                                  thread_data->z);

}

EAPI void
glVertexAttrib3f_evgl_api_thread_cmd(GLuint indx, GLfloat x, GLfloat y, GLfloat z)
{
   if (!evas_evgl_thread_enabled() || (_main_thread_id != eina_thread_self()))
     {
        orig_evgl_api_glVertexAttrib3f(indx, x, y, z);
        return;
     }

   int thread_mode = EVAS_GL_THREAD_MODE_FINISH;

   EVGL_API_Thread_Command_glVertexAttrib3f thread_data_local;
   EVGL_API_Thread_Command_glVertexAttrib3f *thread_data = &thread_data_local;

   thread_data->indx = indx;
   thread_data->x = x;
   thread_data->y = y;
   thread_data->z = z;

   evas_gl_thread_cmd_enqueue(EVAS_GL_THREAD_TYPE_EVGL,
                              _evgl_api_thread_glVertexAttrib3f,
                              thread_data,
                              thread_mode);
}

/*
 * void
 * glVertexAttrib3fv(GLuint indx, const GLfloat* values);
 */

typedef struct
{
   GLuint indx;
   const GLfloat* values;

} EVGL_API_Thread_Command_glVertexAttrib3fv;

void (*orig_evgl_api_glVertexAttrib3fv)(GLuint indx, const GLfloat* values);

static void
_evgl_api_thread_glVertexAttrib3fv(void *data)
{
   EVGL_API_Thread_Command_glVertexAttrib3fv *thread_data =
      (EVGL_API_Thread_Command_glVertexAttrib3fv *)data;

   orig_evgl_api_glVertexAttrib3fv(thread_data->indx,
                                   thread_data->values);

}

EAPI void
glVertexAttrib3fv_evgl_api_thread_cmd(GLuint indx, const GLfloat* values)
{
   if (!evas_evgl_thread_enabled() || (_main_thread_id != eina_thread_self()))
     {
        orig_evgl_api_glVertexAttrib3fv(indx, values);
        return;
     }

   int thread_mode = EVAS_GL_THREAD_MODE_FINISH;

   EVGL_API_Thread_Command_glVertexAttrib3fv thread_data_local;
   EVGL_API_Thread_Command_glVertexAttrib3fv *thread_data = &thread_data_local;

   thread_data->indx = indx;
   thread_data->values = values;

   evas_gl_thread_cmd_enqueue(EVAS_GL_THREAD_TYPE_EVGL,
                              _evgl_api_thread_glVertexAttrib3fv,
                              thread_data,
                              thread_mode);
}

/*
 * void
 * glVertexAttrib4f(GLuint indx, GLfloat x, GLfloat y, GLfloat z, GLfloat w);
 */

typedef struct
{
   GLuint indx;
   GLfloat x;
   GLfloat y;
   GLfloat z;
   GLfloat w;

} EVGL_API_Thread_Command_glVertexAttrib4f;

void (*orig_evgl_api_glVertexAttrib4f)(GLuint indx, GLfloat x, GLfloat y, GLfloat z, GLfloat w);

static void
_evgl_api_thread_glVertexAttrib4f(void *data)
{
   EVGL_API_Thread_Command_glVertexAttrib4f *thread_data =
      (EVGL_API_Thread_Command_glVertexAttrib4f *)data;

   orig_evgl_api_glVertexAttrib4f(thread_data->indx,
                                  thread_data->x,
                                  thread_data->y,
                                  thread_data->z,
                                  thread_data->w);

}

EAPI void
glVertexAttrib4f_evgl_api_thread_cmd(GLuint indx, GLfloat x, GLfloat y, GLfloat z, GLfloat w)
{
   if (!evas_evgl_thread_enabled() || (_main_thread_id != eina_thread_self()))
     {
        orig_evgl_api_glVertexAttrib4f(indx, x, y, z, w);
        return;
     }

   int thread_mode = EVAS_GL_THREAD_MODE_FINISH;

   EVGL_API_Thread_Command_glVertexAttrib4f thread_data_local;
   EVGL_API_Thread_Command_glVertexAttrib4f *thread_data = &thread_data_local;

   thread_data->indx = indx;
   thread_data->x = x;
   thread_data->y = y;
   thread_data->z = z;
   thread_data->w = w;

   evas_gl_thread_cmd_enqueue(EVAS_GL_THREAD_TYPE_EVGL,
                              _evgl_api_thread_glVertexAttrib4f,
                              thread_data,
                              thread_mode);
}

/*
 * void
 * glVertexAttrib4fv(GLuint indx, const GLfloat* values);
 */

typedef struct
{
   GLuint indx;
   const GLfloat* values;

} EVGL_API_Thread_Command_glVertexAttrib4fv;

void (*orig_evgl_api_glVertexAttrib4fv)(GLuint indx, const GLfloat* values);

static void
_evgl_api_thread_glVertexAttrib4fv(void *data)
{
   EVGL_API_Thread_Command_glVertexAttrib4fv *thread_data =
      (EVGL_API_Thread_Command_glVertexAttrib4fv *)data;

   orig_evgl_api_glVertexAttrib4fv(thread_data->indx,
                                   thread_data->values);

}

EAPI void
glVertexAttrib4fv_evgl_api_thread_cmd(GLuint indx, const GLfloat* values)
{
   if (!evas_evgl_thread_enabled() || (_main_thread_id != eina_thread_self()))
     {
        orig_evgl_api_glVertexAttrib4fv(indx, values);
        return;
     }

   int thread_mode = EVAS_GL_THREAD_MODE_FINISH;

   EVGL_API_Thread_Command_glVertexAttrib4fv thread_data_local;
   EVGL_API_Thread_Command_glVertexAttrib4fv *thread_data = &thread_data_local;

   thread_data->indx = indx;
   thread_data->values = values;

   evas_gl_thread_cmd_enqueue(EVAS_GL_THREAD_TYPE_EVGL,
                              _evgl_api_thread_glVertexAttrib4fv,
                              thread_data,
                              thread_mode);
}

/*
 * void
 * glVertexAttribPointer(GLuint indx, GLint size, GLenum type, GLboolean normalized, GLsizei stride, const void* ptr);
 */

typedef struct
{
   GLuint indx;
   GLint size;
   GLenum type;
   GLboolean normalized;
   GLsizei stride;
   const void* ptr;

} EVGL_API_Thread_Command_glVertexAttribPointer;

void (*orig_evgl_api_glVertexAttribPointer)(GLuint indx, GLint size, GLenum type, GLboolean normalized, GLsizei stride, const void* ptr);

static void
_evgl_api_thread_glVertexAttribPointer(void *data)
{
   EVGL_API_Thread_Command_glVertexAttribPointer *thread_data =
      (EVGL_API_Thread_Command_glVertexAttribPointer *)data;

   orig_evgl_api_glVertexAttribPointer(thread_data->indx,
                                       thread_data->size,
                                       thread_data->type,
                                       thread_data->normalized,
                                       thread_data->stride,
                                       thread_data->ptr);

}

EAPI void
glVertexAttribPointer_evgl_api_thread_cmd(GLuint indx, GLint size, GLenum type, GLboolean normalized, GLsizei stride, const void* ptr)
{
   if (!evas_evgl_thread_enabled() || (_main_thread_id != eina_thread_self()))
     {
        orig_evgl_api_glVertexAttribPointer(indx, size, type, normalized, stride, ptr);
        return;
     }

   int thread_mode = EVAS_GL_THREAD_MODE_FINISH;

   EVGL_API_Thread_Command_glVertexAttribPointer thread_data_local;
   EVGL_API_Thread_Command_glVertexAttribPointer *thread_data = &thread_data_local;

   thread_data->indx = indx;
   thread_data->size = size;
   thread_data->type = type;
   thread_data->normalized = normalized;
   thread_data->stride = stride;
   thread_data->ptr = ptr;

   evas_gl_thread_cmd_enqueue(EVAS_GL_THREAD_TYPE_EVGL,
                              _evgl_api_thread_glVertexAttribPointer,
                              thread_data,
                              thread_mode);
}

/*
 * void
 * glViewport(GLint x, GLint y, GLsizei width, GLsizei height);
 */

typedef struct
{
   GLint x;
   GLint y;
   GLsizei width;
   GLsizei height;
   int command_allocated;

} EVGL_API_Thread_Command_glViewport;

void (*orig_evgl_api_glViewport)(GLint x, GLint y, GLsizei width, GLsizei height);

static void
_evgl_api_thread_glViewport(void *data)
{
   EVGL_API_Thread_Command_glViewport *thread_data =
      (EVGL_API_Thread_Command_glViewport *)data;

   orig_evgl_api_glViewport(thread_data->x,
                            thread_data->y,
                            thread_data->width,
                            thread_data->height);

   if (thread_data->command_allocated)
     eina_mempool_free(_mp_command, thread_data);
}

EAPI void
glViewport_evgl_api_thread_cmd(GLint x, GLint y, GLsizei width, GLsizei height)
{
   if (!evas_evgl_thread_enabled() || (_main_thread_id != eina_thread_self()))
     {
        orig_evgl_api_glViewport(x, y, width, height);
        return;
     }

   int thread_mode = EVAS_GL_THREAD_MODE_FINISH;

   EVGL_API_Thread_Command_glViewport thread_data_local;
   EVGL_API_Thread_Command_glViewport *thread_data = &thread_data_local;

   /* command_allocated flag init. */
   thread_data->command_allocated = 0;

   if (!evas_gl_thread_force_finish())
     { /* _flush */
        EVGL_API_Thread_Command_glViewport *thread_data_new;
        thread_data_new = eina_mempool_malloc(_mp_command,
                                              sizeof(EVGL_API_Thread_Command_glViewport));
        if (thread_data_new)
          {
             thread_data = thread_data_new;
             thread_data->command_allocated = 1;
             thread_mode = EVAS_GL_THREAD_MODE_FLUSH;
          }
     }

   thread_data->x = x;
   thread_data->y = y;
   thread_data->width = width;
   thread_data->height = height;

   evas_gl_thread_cmd_enqueue(EVAS_GL_THREAD_TYPE_EVGL,
                              _evgl_api_thread_glViewport,
                              thread_data,
                              thread_mode);
}

/*
 * void
 * glEvasGLImageTargetTexture2DOES(GLenum target, EvasGLImage image);
 */

typedef struct
{
   GLenum target;
   EvasGLImage image;

} EVGL_API_Thread_Command_glEvasGLImageTargetTexture2DOES;

void (*orig_evgl_api_glEvasGLImageTargetTexture2DOES)(GLenum target, EvasGLImage image);

static void
_evgl_api_thread_glEvasGLImageTargetTexture2DOES(void *data)
{
   EVGL_API_Thread_Command_glEvasGLImageTargetTexture2DOES *thread_data =
      (EVGL_API_Thread_Command_glEvasGLImageTargetTexture2DOES *)data;

   orig_evgl_api_glEvasGLImageTargetTexture2DOES(thread_data->target,
                                                 thread_data->image);

}

EAPI void
glEvasGLImageTargetTexture2DOES_evgl_api_thread_cmd(GLenum target, EvasGLImage image)
{
   if (!evas_evgl_thread_enabled() || (_main_thread_id != eina_thread_self()))
     {
        orig_evgl_api_glEvasGLImageTargetTexture2DOES(target, image);
        return;
     }

   int thread_mode = EVAS_GL_THREAD_MODE_FINISH;

   EVGL_API_Thread_Command_glEvasGLImageTargetTexture2DOES thread_data_local;
   EVGL_API_Thread_Command_glEvasGLImageTargetTexture2DOES *thread_data = &thread_data_local;

   thread_data->target = target;
   thread_data->image = image;

   evas_gl_thread_cmd_enqueue(EVAS_GL_THREAD_TYPE_EVGL,
                              _evgl_api_thread_glEvasGLImageTargetTexture2DOES,
                              thread_data,
                              thread_mode);
}

/*
 * void
 * glEvasGLImageTargetRenderbufferStorageOES(GLenum target, EvasGLImage image);
 */

typedef struct
{
   GLenum target;
   EvasGLImage image;

} EVGL_API_Thread_Command_glEvasGLImageTargetRenderbufferStorageOES;

void (*orig_evgl_api_glEvasGLImageTargetRenderbufferStorageOES)(GLenum target, EvasGLImage image);

static void
_evgl_api_thread_glEvasGLImageTargetRenderbufferStorageOES(void *data)
{
   EVGL_API_Thread_Command_glEvasGLImageTargetRenderbufferStorageOES *thread_data =
      (EVGL_API_Thread_Command_glEvasGLImageTargetRenderbufferStorageOES *)data;

   orig_evgl_api_glEvasGLImageTargetRenderbufferStorageOES(thread_data->target,
                                                           thread_data->image);

}

EAPI void
glEvasGLImageTargetRenderbufferStorageOES_evgl_api_thread_cmd(GLenum target, EvasGLImage image)
{
   if (!evas_evgl_thread_enabled() || (_main_thread_id != eina_thread_self()))
     {
        orig_evgl_api_glEvasGLImageTargetRenderbufferStorageOES(target, image);
        return;
     }

   int thread_mode = EVAS_GL_THREAD_MODE_FINISH;

   EVGL_API_Thread_Command_glEvasGLImageTargetRenderbufferStorageOES thread_data_local;
   EVGL_API_Thread_Command_glEvasGLImageTargetRenderbufferStorageOES *thread_data = &thread_data_local;

   thread_data->target = target;
   thread_data->image = image;

   evas_gl_thread_cmd_enqueue(EVAS_GL_THREAD_TYPE_EVGL,
                              _evgl_api_thread_glEvasGLImageTargetRenderbufferStorageOES,
                              thread_data,
                              thread_mode);
}

/*
 * void
 * glGetProgramBinaryOES(GLuint program, GLsizei bufSize, GLsizei *length, GLenum *binaryFormat, void *binary);
 */

typedef struct
{
   GLuint program;
   GLsizei bufSize;
   GLsizei *length;
   GLenum *binaryFormat;
   void *binary;

} EVGL_API_Thread_Command_glGetProgramBinaryOES;

void (*orig_evgl_api_glGetProgramBinaryOES)(GLuint program, GLsizei bufSize, GLsizei *length, GLenum *binaryFormat, void *binary);

static void
_evgl_api_thread_glGetProgramBinaryOES(void *data)
{
   EVGL_API_Thread_Command_glGetProgramBinaryOES *thread_data =
      (EVGL_API_Thread_Command_glGetProgramBinaryOES *)data;

   orig_evgl_api_glGetProgramBinaryOES(thread_data->program,
                                       thread_data->bufSize,
                                       thread_data->length,
                                       thread_data->binaryFormat,
                                       thread_data->binary);

}

EAPI void
glGetProgramBinaryOES_evgl_api_thread_cmd(GLuint program, GLsizei bufSize, GLsizei *length, GLenum *binaryFormat, void *binary)
{
   if (!evas_evgl_thread_enabled() || (_main_thread_id != eina_thread_self()))
     {
        orig_evgl_api_glGetProgramBinaryOES(program, bufSize, length, binaryFormat, binary);
        return;
     }

   int thread_mode = EVAS_GL_THREAD_MODE_FINISH;

   EVGL_API_Thread_Command_glGetProgramBinaryOES thread_data_local;
   EVGL_API_Thread_Command_glGetProgramBinaryOES *thread_data = &thread_data_local;

   thread_data->program = program;
   thread_data->bufSize = bufSize;
   thread_data->length = length;
   thread_data->binaryFormat = binaryFormat;
   thread_data->binary = binary;

   evas_gl_thread_cmd_enqueue(EVAS_GL_THREAD_TYPE_EVGL,
                              _evgl_api_thread_glGetProgramBinaryOES,
                              thread_data,
                              thread_mode);
}

/*
 * void
 * glProgramBinaryOES(GLuint program, GLenum binaryFormat, const void *binary, GLint length);
 */

typedef struct
{
   GLuint program;
   GLenum binaryFormat;
   const void *binary;
   GLint length;
   void *binary_copied; /* COPIED */
   int command_allocated;

} EVGL_API_Thread_Command_glProgramBinaryOES;

void (*orig_evgl_api_glProgramBinaryOES)(GLuint program, GLenum binaryFormat, const void *binary, GLint length);

static void
_evgl_api_thread_glProgramBinaryOES(void *data)
{
   EVGL_API_Thread_Command_glProgramBinaryOES *thread_data =
      (EVGL_API_Thread_Command_glProgramBinaryOES *)data;

   orig_evgl_api_glProgramBinaryOES(thread_data->program,
                                    thread_data->binaryFormat,
                                    thread_data->binary,
                                    thread_data->length);


   if (thread_data->binary_copied)
     eina_mempool_free(_mp_default, thread_data->binary_copied);

   if (thread_data->command_allocated)
     eina_mempool_free(_mp_command, thread_data);
}

EAPI void
glProgramBinaryOES_evgl_api_thread_cmd(GLuint program, GLenum binaryFormat, const void *binary, GLint length)
{
   if (!evas_evgl_thread_enabled() || (_main_thread_id != eina_thread_self()))
     {
        orig_evgl_api_glProgramBinaryOES(program, binaryFormat, binary, length);
        return;
     }

   int thread_mode = EVAS_GL_THREAD_MODE_FINISH;

   EVGL_API_Thread_Command_glProgramBinaryOES thread_data_local;
   EVGL_API_Thread_Command_glProgramBinaryOES *thread_data = &thread_data_local;

   /* command_allocated flag init. */
   thread_data->command_allocated = 0;

   if (!evas_gl_thread_force_finish())
     { /* _flush */
        EVGL_API_Thread_Command_glProgramBinaryOES *thread_data_new;
        thread_data_new = eina_mempool_malloc(_mp_command,
                                              sizeof(EVGL_API_Thread_Command_glProgramBinaryOES));
        if (thread_data_new)
          {
             thread_data = thread_data_new;
             thread_data->command_allocated = 1;
             thread_mode = EVAS_GL_THREAD_MODE_FLUSH;
          }
     }

   thread_data->program = program;
   thread_data->binaryFormat = binaryFormat;
   thread_data->binary = binary;
   thread_data->length = length;

   thread_data->binary_copied = NULL;
   if (thread_mode == EVAS_GL_THREAD_MODE_FINISH)
     goto finish;

   /* copy variable */
   if (binary)
     {
        /* 1. check memory size */
        unsigned int copy_size = length;
        if (copy_size > _mp_default_memory_size)
          {
             thread_mode = EVAS_GL_THREAD_MODE_FINISH;
             goto finish;
          }
        /* 2. malloc & copy */
        thread_data->binary_copied = eina_mempool_malloc(_mp_default, copy_size);
        if (thread_data->binary_copied)
          {
             memcpy(thread_data->binary_copied, binary, copy_size);
          }
        else
          {
             thread_mode = EVAS_GL_THREAD_MODE_FINISH;
             goto finish;
          }
        /* 3. replace */
        thread_data->binary = (const void  *)thread_data->binary_copied;
     }
   /* end of copy variable */

finish:
   evas_gl_thread_cmd_enqueue(EVAS_GL_THREAD_TYPE_EVGL,
                              _evgl_api_thread_glProgramBinaryOES,
                              thread_data,
                              thread_mode);
}

/*
 * void *
 * glMapBufferOES(GLenum target, GLenum access);
 */

typedef struct
{
   void * return_value;
   GLenum target;
   GLenum access;

} EVGL_API_Thread_Command_glMapBufferOES;

void * (*orig_evgl_api_glMapBufferOES)(GLenum target, GLenum access);

static void
_evgl_api_thread_glMapBufferOES(void *data)
{
   EVGL_API_Thread_Command_glMapBufferOES *thread_data =
      (EVGL_API_Thread_Command_glMapBufferOES *)data;

   thread_data->return_value = orig_evgl_api_glMapBufferOES(thread_data->target,
                                                            thread_data->access);

}

EAPI void *
glMapBufferOES_evgl_api_thread_cmd(GLenum target, GLenum access)
{
   if (!evas_evgl_thread_enabled() || (_main_thread_id != eina_thread_self()))
     {
        return orig_evgl_api_glMapBufferOES(target, access);
     }

   int thread_mode = EVAS_GL_THREAD_MODE_FINISH;

   EVGL_API_Thread_Command_glMapBufferOES thread_data_local;
   EVGL_API_Thread_Command_glMapBufferOES *thread_data = &thread_data_local;

   thread_data->target = target;
   thread_data->access = access;

   evas_gl_thread_cmd_enqueue(EVAS_GL_THREAD_TYPE_EVGL,
                              _evgl_api_thread_glMapBufferOES,
                              thread_data,
                              thread_mode);

   return thread_data->return_value;
}

/*
 * GLboolean
 * glUnmapBufferOES(GLenum target);
 */

typedef struct
{
   GLboolean return_value;
   GLenum target;

} EVGL_API_Thread_Command_glUnmapBufferOES;

GLboolean (*orig_evgl_api_glUnmapBufferOES)(GLenum target);

static void
_evgl_api_thread_glUnmapBufferOES(void *data)
{
   EVGL_API_Thread_Command_glUnmapBufferOES *thread_data =
      (EVGL_API_Thread_Command_glUnmapBufferOES *)data;

   thread_data->return_value = orig_evgl_api_glUnmapBufferOES(thread_data->target);

}

EAPI GLboolean
glUnmapBufferOES_evgl_api_thread_cmd(GLenum target)
{
   if (!evas_evgl_thread_enabled() || (_main_thread_id != eina_thread_self()))
     {
        return orig_evgl_api_glUnmapBufferOES(target);
     }

   int thread_mode = EVAS_GL_THREAD_MODE_FINISH;

   EVGL_API_Thread_Command_glUnmapBufferOES thread_data_local;
   EVGL_API_Thread_Command_glUnmapBufferOES *thread_data = &thread_data_local;

   thread_data->target = target;

   evas_gl_thread_cmd_enqueue(EVAS_GL_THREAD_TYPE_EVGL,
                              _evgl_api_thread_glUnmapBufferOES,
                              thread_data,
                              thread_mode);

   return thread_data->return_value;
}

/*
 * void
 * glGetBufferPointervOES(GLenum target, GLenum pname, void** params);
 */

typedef struct
{
   GLenum target;
   GLenum pname;
   void** params;

} EVGL_API_Thread_Command_glGetBufferPointervOES;

void (*orig_evgl_api_glGetBufferPointervOES)(GLenum target, GLenum pname, void** params);

static void
_evgl_api_thread_glGetBufferPointervOES(void *data)
{
   EVGL_API_Thread_Command_glGetBufferPointervOES *thread_data =
      (EVGL_API_Thread_Command_glGetBufferPointervOES *)data;

   orig_evgl_api_glGetBufferPointervOES(thread_data->target,
                                        thread_data->pname,
                                        thread_data->params);

}

EAPI void
glGetBufferPointervOES_evgl_api_thread_cmd(GLenum target, GLenum pname, void** params)
{
   if (!evas_evgl_thread_enabled() || (_main_thread_id != eina_thread_self()))
     {
        orig_evgl_api_glGetBufferPointervOES(target, pname, params);
        return;
     }

   int thread_mode = EVAS_GL_THREAD_MODE_FINISH;

   EVGL_API_Thread_Command_glGetBufferPointervOES thread_data_local;
   EVGL_API_Thread_Command_glGetBufferPointervOES *thread_data = &thread_data_local;

   thread_data->target = target;
   thread_data->pname = pname;
   thread_data->params = params;

   evas_gl_thread_cmd_enqueue(EVAS_GL_THREAD_TYPE_EVGL,
                              _evgl_api_thread_glGetBufferPointervOES,
                              thread_data,
                              thread_mode);
}

/*
 * void
 * glTexImage3DOES(GLenum target, GLint level, GLenum internalformat, GLsizei width, GLsizei height, GLsizei depth, GLint border, GLenum format, GLenum type, const void* pixels);
 */

typedef struct
{
   GLenum target;
   GLint level;
   GLenum internalformat;
   GLsizei width;
   GLsizei height;
   GLsizei depth;
   GLint border;
   GLenum format;
   GLenum type;
   const void* pixels;

} EVGL_API_Thread_Command_glTexImage3DOES;

void (*orig_evgl_api_glTexImage3DOES)(GLenum target, GLint level, GLenum internalformat, GLsizei width, GLsizei height, GLsizei depth, GLint border, GLenum format, GLenum type, const void* pixels);

static void
_evgl_api_thread_glTexImage3DOES(void *data)
{
   EVGL_API_Thread_Command_glTexImage3DOES *thread_data =
      (EVGL_API_Thread_Command_glTexImage3DOES *)data;

   orig_evgl_api_glTexImage3DOES(thread_data->target,
                                 thread_data->level,
                                 thread_data->internalformat,
                                 thread_data->width,
                                 thread_data->height,
                                 thread_data->depth,
                                 thread_data->border,
                                 thread_data->format,
                                 thread_data->type,
                                 thread_data->pixels);

}

EAPI void
glTexImage3DOES_evgl_api_thread_cmd(GLenum target, GLint level, GLenum internalformat, GLsizei width, GLsizei height, GLsizei depth, GLint border, GLenum format, GLenum type, const void* pixels)
{
   if (!evas_evgl_thread_enabled() || (_main_thread_id != eina_thread_self()))
     {
        orig_evgl_api_glTexImage3DOES(target, level, internalformat, width, height, depth, border, format, type, pixels);
        return;
     }

   int thread_mode = EVAS_GL_THREAD_MODE_FINISH;

   EVGL_API_Thread_Command_glTexImage3DOES thread_data_local;
   EVGL_API_Thread_Command_glTexImage3DOES *thread_data = &thread_data_local;

   thread_data->target = target;
   thread_data->level = level;
   thread_data->internalformat = internalformat;
   thread_data->width = width;
   thread_data->height = height;
   thread_data->depth = depth;
   thread_data->border = border;
   thread_data->format = format;
   thread_data->type = type;
   thread_data->pixels = pixels;

   evas_gl_thread_cmd_enqueue(EVAS_GL_THREAD_TYPE_EVGL,
                              _evgl_api_thread_glTexImage3DOES,
                              thread_data,
                              thread_mode);
}

/*
 * void
 * glTexSubImage3DOES(GLenum target, GLint level, GLint xoffset, GLint yoffset, GLint zoffset, GLsizei width, GLsizei height, GLsizei depth, GLenum format, GLenum type, const void* pixels);
 */

typedef struct
{
   GLenum target;
   GLint level;
   GLint xoffset;
   GLint yoffset;
   GLint zoffset;
   GLsizei width;
   GLsizei height;
   GLsizei depth;
   GLenum format;
   GLenum type;
   const void* pixels;

} EVGL_API_Thread_Command_glTexSubImage3DOES;

void (*orig_evgl_api_glTexSubImage3DOES)(GLenum target, GLint level, GLint xoffset, GLint yoffset, GLint zoffset, GLsizei width, GLsizei height, GLsizei depth, GLenum format, GLenum type, const void* pixels);

static void
_evgl_api_thread_glTexSubImage3DOES(void *data)
{
   EVGL_API_Thread_Command_glTexSubImage3DOES *thread_data =
      (EVGL_API_Thread_Command_glTexSubImage3DOES *)data;

   orig_evgl_api_glTexSubImage3DOES(thread_data->target,
                                    thread_data->level,
                                    thread_data->xoffset,
                                    thread_data->yoffset,
                                    thread_data->zoffset,
                                    thread_data->width,
                                    thread_data->height,
                                    thread_data->depth,
                                    thread_data->format,
                                    thread_data->type,
                                    thread_data->pixels);

}

EAPI void
glTexSubImage3DOES_evgl_api_thread_cmd(GLenum target, GLint level, GLint xoffset, GLint yoffset, GLint zoffset, GLsizei width, GLsizei height, GLsizei depth, GLenum format, GLenum type, const void* pixels)
{
   if (!evas_evgl_thread_enabled() || (_main_thread_id != eina_thread_self()))
     {
        orig_evgl_api_glTexSubImage3DOES(target, level, xoffset, yoffset, zoffset, width, height, depth, format, type, pixels);
        return;
     }

   int thread_mode = EVAS_GL_THREAD_MODE_FINISH;

   EVGL_API_Thread_Command_glTexSubImage3DOES thread_data_local;
   EVGL_API_Thread_Command_glTexSubImage3DOES *thread_data = &thread_data_local;

   thread_data->target = target;
   thread_data->level = level;
   thread_data->xoffset = xoffset;
   thread_data->yoffset = yoffset;
   thread_data->zoffset = zoffset;
   thread_data->width = width;
   thread_data->height = height;
   thread_data->depth = depth;
   thread_data->format = format;
   thread_data->type = type;
   thread_data->pixels = pixels;

   evas_gl_thread_cmd_enqueue(EVAS_GL_THREAD_TYPE_EVGL,
                              _evgl_api_thread_glTexSubImage3DOES,
                              thread_data,
                              thread_mode);
}

/*
 * void
 * glCopyTexSubImage3DOES(GLenum target, GLint level, GLint xoffset, GLint yoffset, GLint zoffset, GLint x, GLint y, GLsizei width, GLsizei height);
 */

typedef struct
{
   GLenum target;
   GLint level;
   GLint xoffset;
   GLint yoffset;
   GLint zoffset;
   GLint x;
   GLint y;
   GLsizei width;
   GLsizei height;

} EVGL_API_Thread_Command_glCopyTexSubImage3DOES;

void (*orig_evgl_api_glCopyTexSubImage3DOES)(GLenum target, GLint level, GLint xoffset, GLint yoffset, GLint zoffset, GLint x, GLint y, GLsizei width, GLsizei height);

static void
_evgl_api_thread_glCopyTexSubImage3DOES(void *data)
{
   EVGL_API_Thread_Command_glCopyTexSubImage3DOES *thread_data =
      (EVGL_API_Thread_Command_glCopyTexSubImage3DOES *)data;

   orig_evgl_api_glCopyTexSubImage3DOES(thread_data->target,
                                        thread_data->level,
                                        thread_data->xoffset,
                                        thread_data->yoffset,
                                        thread_data->zoffset,
                                        thread_data->x,
                                        thread_data->y,
                                        thread_data->width,
                                        thread_data->height);

}

EAPI void
glCopyTexSubImage3DOES_evgl_api_thread_cmd(GLenum target, GLint level, GLint xoffset, GLint yoffset, GLint zoffset, GLint x, GLint y, GLsizei width, GLsizei height)
{
   if (!evas_evgl_thread_enabled() || (_main_thread_id != eina_thread_self()))
     {
        orig_evgl_api_glCopyTexSubImage3DOES(target, level, xoffset, yoffset, zoffset, x, y, width, height);
        return;
     }

   int thread_mode = EVAS_GL_THREAD_MODE_FINISH;

   EVGL_API_Thread_Command_glCopyTexSubImage3DOES thread_data_local;
   EVGL_API_Thread_Command_glCopyTexSubImage3DOES *thread_data = &thread_data_local;

   thread_data->target = target;
   thread_data->level = level;
   thread_data->xoffset = xoffset;
   thread_data->yoffset = yoffset;
   thread_data->zoffset = zoffset;
   thread_data->x = x;
   thread_data->y = y;
   thread_data->width = width;
   thread_data->height = height;

   evas_gl_thread_cmd_enqueue(EVAS_GL_THREAD_TYPE_EVGL,
                              _evgl_api_thread_glCopyTexSubImage3DOES,
                              thread_data,
                              thread_mode);
}

/*
 * void
 * glCompressedTexImage3DOES(GLenum target, GLint level, GLenum internalformat, GLsizei width, GLsizei height, GLsizei depth, GLint border, GLsizei imageSize, const void* data);
 */

typedef struct
{
   GLenum target;
   GLint level;
   GLenum internalformat;
   GLsizei width;
   GLsizei height;
   GLsizei depth;
   GLint border;
   GLsizei imageSize;
   const void* data;

} EVGL_API_Thread_Command_glCompressedTexImage3DOES;

void (*orig_evgl_api_glCompressedTexImage3DOES)(GLenum target, GLint level, GLenum internalformat, GLsizei width, GLsizei height, GLsizei depth, GLint border, GLsizei imageSize, const void* data);

static void
_evgl_api_thread_glCompressedTexImage3DOES(void *data)
{
   EVGL_API_Thread_Command_glCompressedTexImage3DOES *thread_data =
      (EVGL_API_Thread_Command_glCompressedTexImage3DOES *)data;

   orig_evgl_api_glCompressedTexImage3DOES(thread_data->target,
                                           thread_data->level,
                                           thread_data->internalformat,
                                           thread_data->width,
                                           thread_data->height,
                                           thread_data->depth,
                                           thread_data->border,
                                           thread_data->imageSize,
                                           thread_data->data);

}

EAPI void
glCompressedTexImage3DOES_evgl_api_thread_cmd(GLenum target, GLint level, GLenum internalformat, GLsizei width, GLsizei height, GLsizei depth, GLint border, GLsizei imageSize, const void* data)
{
   if (!evas_evgl_thread_enabled() || (_main_thread_id != eina_thread_self()))
     {
        orig_evgl_api_glCompressedTexImage3DOES(target, level, internalformat, width, height, depth, border, imageSize, data);
        return;
     }

   int thread_mode = EVAS_GL_THREAD_MODE_FINISH;

   EVGL_API_Thread_Command_glCompressedTexImage3DOES thread_data_local;
   EVGL_API_Thread_Command_glCompressedTexImage3DOES *thread_data = &thread_data_local;

   thread_data->target = target;
   thread_data->level = level;
   thread_data->internalformat = internalformat;
   thread_data->width = width;
   thread_data->height = height;
   thread_data->depth = depth;
   thread_data->border = border;
   thread_data->imageSize = imageSize;
   thread_data->data = data;

   evas_gl_thread_cmd_enqueue(EVAS_GL_THREAD_TYPE_EVGL,
                              _evgl_api_thread_glCompressedTexImage3DOES,
                              thread_data,
                              thread_mode);
}

/*
 * void
 * glCompressedTexSubImage3DOES(GLenum target, GLint level, GLint xoffset, GLint yoffset, GLint zoffset, GLsizei width, GLsizei height, GLsizei depth, GLenum format, GLsizei imageSize, const void* data);
 */

typedef struct
{
   GLenum target;
   GLint level;
   GLint xoffset;
   GLint yoffset;
   GLint zoffset;
   GLsizei width;
   GLsizei height;
   GLsizei depth;
   GLenum format;
   GLsizei imageSize;
   const void* data;

} EVGL_API_Thread_Command_glCompressedTexSubImage3DOES;

void (*orig_evgl_api_glCompressedTexSubImage3DOES)(GLenum target, GLint level, GLint xoffset, GLint yoffset, GLint zoffset, GLsizei width, GLsizei height, GLsizei depth, GLenum format, GLsizei imageSize, const void* data);

static void
_evgl_api_thread_glCompressedTexSubImage3DOES(void *data)
{
   EVGL_API_Thread_Command_glCompressedTexSubImage3DOES *thread_data =
      (EVGL_API_Thread_Command_glCompressedTexSubImage3DOES *)data;

   orig_evgl_api_glCompressedTexSubImage3DOES(thread_data->target,
                                              thread_data->level,
                                              thread_data->xoffset,
                                              thread_data->yoffset,
                                              thread_data->zoffset,
                                              thread_data->width,
                                              thread_data->height,
                                              thread_data->depth,
                                              thread_data->format,
                                              thread_data->imageSize,
                                              thread_data->data);

}

EAPI void
glCompressedTexSubImage3DOES_evgl_api_thread_cmd(GLenum target, GLint level, GLint xoffset, GLint yoffset, GLint zoffset, GLsizei width, GLsizei height, GLsizei depth, GLenum format, GLsizei imageSize, const void* data)
{
   if (!evas_evgl_thread_enabled() || (_main_thread_id != eina_thread_self()))
     {
        orig_evgl_api_glCompressedTexSubImage3DOES(target, level, xoffset, yoffset, zoffset, width, height, depth, format, imageSize, data);
        return;
     }

   int thread_mode = EVAS_GL_THREAD_MODE_FINISH;

   EVGL_API_Thread_Command_glCompressedTexSubImage3DOES thread_data_local;
   EVGL_API_Thread_Command_glCompressedTexSubImage3DOES *thread_data = &thread_data_local;

   thread_data->target = target;
   thread_data->level = level;
   thread_data->xoffset = xoffset;
   thread_data->yoffset = yoffset;
   thread_data->zoffset = zoffset;
   thread_data->width = width;
   thread_data->height = height;
   thread_data->depth = depth;
   thread_data->format = format;
   thread_data->imageSize = imageSize;
   thread_data->data = data;

   evas_gl_thread_cmd_enqueue(EVAS_GL_THREAD_TYPE_EVGL,
                              _evgl_api_thread_glCompressedTexSubImage3DOES,
                              thread_data,
                              thread_mode);
}

/*
 * void
 * glFramebufferTexture3DOES(GLenum target, GLenum attachment, GLenum textarget, GLuint texture, GLint level, GLint zoffset);
 */

typedef struct
{
   GLenum target;
   GLenum attachment;
   GLenum textarget;
   GLuint texture;
   GLint level;
   GLint zoffset;

} EVGL_API_Thread_Command_glFramebufferTexture3DOES;

void (*orig_evgl_api_glFramebufferTexture3DOES)(GLenum target, GLenum attachment, GLenum textarget, GLuint texture, GLint level, GLint zoffset);

static void
_evgl_api_thread_glFramebufferTexture3DOES(void *data)
{
   EVGL_API_Thread_Command_glFramebufferTexture3DOES *thread_data =
      (EVGL_API_Thread_Command_glFramebufferTexture3DOES *)data;

   orig_evgl_api_glFramebufferTexture3DOES(thread_data->target,
                                           thread_data->attachment,
                                           thread_data->textarget,
                                           thread_data->texture,
                                           thread_data->level,
                                           thread_data->zoffset);

}

EAPI void
glFramebufferTexture3DOES_evgl_api_thread_cmd(GLenum target, GLenum attachment, GLenum textarget, GLuint texture, GLint level, GLint zoffset)
{
   if (!evas_evgl_thread_enabled() || (_main_thread_id != eina_thread_self()))
     {
        orig_evgl_api_glFramebufferTexture3DOES(target, attachment, textarget, texture, level, zoffset);
        return;
     }

   int thread_mode = EVAS_GL_THREAD_MODE_FINISH;

   EVGL_API_Thread_Command_glFramebufferTexture3DOES thread_data_local;
   EVGL_API_Thread_Command_glFramebufferTexture3DOES *thread_data = &thread_data_local;

   thread_data->target = target;
   thread_data->attachment = attachment;
   thread_data->textarget = textarget;
   thread_data->texture = texture;
   thread_data->level = level;
   thread_data->zoffset = zoffset;

   evas_gl_thread_cmd_enqueue(EVAS_GL_THREAD_TYPE_EVGL,
                              _evgl_api_thread_glFramebufferTexture3DOES,
                              thread_data,
                              thread_mode);
}

/*
 * void
 * glGetPerfMonitorGroupsAMD(GLint* numGroups, GLsizei groupsSize, GLuint* groups);
 */

typedef struct
{
   GLint* numGroups;
   GLsizei groupsSize;
   GLuint* groups;

} EVGL_API_Thread_Command_glGetPerfMonitorGroupsAMD;

void (*orig_evgl_api_glGetPerfMonitorGroupsAMD)(GLint* numGroups, GLsizei groupsSize, GLuint* groups);

static void
_evgl_api_thread_glGetPerfMonitorGroupsAMD(void *data)
{
   EVGL_API_Thread_Command_glGetPerfMonitorGroupsAMD *thread_data =
      (EVGL_API_Thread_Command_glGetPerfMonitorGroupsAMD *)data;

   orig_evgl_api_glGetPerfMonitorGroupsAMD(thread_data->numGroups,
                                           thread_data->groupsSize,
                                           thread_data->groups);

}

EAPI void
glGetPerfMonitorGroupsAMD_evgl_api_thread_cmd(GLint* numGroups, GLsizei groupsSize, GLuint* groups)
{
   if (!evas_evgl_thread_enabled() || (_main_thread_id != eina_thread_self()))
     {
        orig_evgl_api_glGetPerfMonitorGroupsAMD(numGroups, groupsSize, groups);
        return;
     }

   int thread_mode = EVAS_GL_THREAD_MODE_FINISH;

   EVGL_API_Thread_Command_glGetPerfMonitorGroupsAMD thread_data_local;
   EVGL_API_Thread_Command_glGetPerfMonitorGroupsAMD *thread_data = &thread_data_local;

   thread_data->numGroups = numGroups;
   thread_data->groupsSize = groupsSize;
   thread_data->groups = groups;

   evas_gl_thread_cmd_enqueue(EVAS_GL_THREAD_TYPE_EVGL,
                              _evgl_api_thread_glGetPerfMonitorGroupsAMD,
                              thread_data,
                              thread_mode);
}

/*
 * void
 * glGetPerfMonitorCountersAMD(GLuint group, GLint* numCounters, GLint* maxActiveCounters, GLsizei counterSize, GLuint* counters);
 */

typedef struct
{
   GLuint group;
   GLint* numCounters;
   GLint* maxActiveCounters;
   GLsizei counterSize;
   GLuint* counters;

} EVGL_API_Thread_Command_glGetPerfMonitorCountersAMD;

void (*orig_evgl_api_glGetPerfMonitorCountersAMD)(GLuint group, GLint* numCounters, GLint* maxActiveCounters, GLsizei counterSize, GLuint* counters);

static void
_evgl_api_thread_glGetPerfMonitorCountersAMD(void *data)
{
   EVGL_API_Thread_Command_glGetPerfMonitorCountersAMD *thread_data =
      (EVGL_API_Thread_Command_glGetPerfMonitorCountersAMD *)data;

   orig_evgl_api_glGetPerfMonitorCountersAMD(thread_data->group,
                                             thread_data->numCounters,
                                             thread_data->maxActiveCounters,
                                             thread_data->counterSize,
                                             thread_data->counters);

}

EAPI void
glGetPerfMonitorCountersAMD_evgl_api_thread_cmd(GLuint group, GLint* numCounters, GLint* maxActiveCounters, GLsizei counterSize, GLuint* counters)
{
   if (!evas_evgl_thread_enabled() || (_main_thread_id != eina_thread_self()))
     {
        orig_evgl_api_glGetPerfMonitorCountersAMD(group, numCounters, maxActiveCounters, counterSize, counters);
        return;
     }

   int thread_mode = EVAS_GL_THREAD_MODE_FINISH;

   EVGL_API_Thread_Command_glGetPerfMonitorCountersAMD thread_data_local;
   EVGL_API_Thread_Command_glGetPerfMonitorCountersAMD *thread_data = &thread_data_local;

   thread_data->group = group;
   thread_data->numCounters = numCounters;
   thread_data->maxActiveCounters = maxActiveCounters;
   thread_data->counterSize = counterSize;
   thread_data->counters = counters;

   evas_gl_thread_cmd_enqueue(EVAS_GL_THREAD_TYPE_EVGL,
                              _evgl_api_thread_glGetPerfMonitorCountersAMD,
                              thread_data,
                              thread_mode);
}

/*
 * void
 * glGetPerfMonitorGroupStringAMD(GLuint group, GLsizei bufSize, GLsizei* length, char* groupString);
 */

typedef struct
{
   GLuint group;
   GLsizei bufSize;
   GLsizei* length;
   char* groupString;

} EVGL_API_Thread_Command_glGetPerfMonitorGroupStringAMD;

void (*orig_evgl_api_glGetPerfMonitorGroupStringAMD)(GLuint group, GLsizei bufSize, GLsizei* length, char* groupString);

static void
_evgl_api_thread_glGetPerfMonitorGroupStringAMD(void *data)
{
   EVGL_API_Thread_Command_glGetPerfMonitorGroupStringAMD *thread_data =
      (EVGL_API_Thread_Command_glGetPerfMonitorGroupStringAMD *)data;

   orig_evgl_api_glGetPerfMonitorGroupStringAMD(thread_data->group,
                                                thread_data->bufSize,
                                                thread_data->length,
                                                thread_data->groupString);

}

EAPI void
glGetPerfMonitorGroupStringAMD_evgl_api_thread_cmd(GLuint group, GLsizei bufSize, GLsizei* length, char* groupString)
{
   if (!evas_evgl_thread_enabled() || (_main_thread_id != eina_thread_self()))
     {
        orig_evgl_api_glGetPerfMonitorGroupStringAMD(group, bufSize, length, groupString);
        return;
     }

   int thread_mode = EVAS_GL_THREAD_MODE_FINISH;

   EVGL_API_Thread_Command_glGetPerfMonitorGroupStringAMD thread_data_local;
   EVGL_API_Thread_Command_glGetPerfMonitorGroupStringAMD *thread_data = &thread_data_local;

   thread_data->group = group;
   thread_data->bufSize = bufSize;
   thread_data->length = length;
   thread_data->groupString = groupString;

   evas_gl_thread_cmd_enqueue(EVAS_GL_THREAD_TYPE_EVGL,
                              _evgl_api_thread_glGetPerfMonitorGroupStringAMD,
                              thread_data,
                              thread_mode);
}

/*
 * void
 * glGetPerfMonitorCounterStringAMD(GLuint group, GLuint counter, GLsizei bufSize, GLsizei* length, char* counterString);
 */

typedef struct
{
   GLuint group;
   GLuint counter;
   GLsizei bufSize;
   GLsizei* length;
   char* counterString;

} EVGL_API_Thread_Command_glGetPerfMonitorCounterStringAMD;

void (*orig_evgl_api_glGetPerfMonitorCounterStringAMD)(GLuint group, GLuint counter, GLsizei bufSize, GLsizei* length, char* counterString);

static void
_evgl_api_thread_glGetPerfMonitorCounterStringAMD(void *data)
{
   EVGL_API_Thread_Command_glGetPerfMonitorCounterStringAMD *thread_data =
      (EVGL_API_Thread_Command_glGetPerfMonitorCounterStringAMD *)data;

   orig_evgl_api_glGetPerfMonitorCounterStringAMD(thread_data->group,
                                                  thread_data->counter,
                                                  thread_data->bufSize,
                                                  thread_data->length,
                                                  thread_data->counterString);

}

EAPI void
glGetPerfMonitorCounterStringAMD_evgl_api_thread_cmd(GLuint group, GLuint counter, GLsizei bufSize, GLsizei* length, char* counterString)
{
   if (!evas_evgl_thread_enabled() || (_main_thread_id != eina_thread_self()))
     {
        orig_evgl_api_glGetPerfMonitorCounterStringAMD(group, counter, bufSize, length, counterString);
        return;
     }

   int thread_mode = EVAS_GL_THREAD_MODE_FINISH;

   EVGL_API_Thread_Command_glGetPerfMonitorCounterStringAMD thread_data_local;
   EVGL_API_Thread_Command_glGetPerfMonitorCounterStringAMD *thread_data = &thread_data_local;

   thread_data->group = group;
   thread_data->counter = counter;
   thread_data->bufSize = bufSize;
   thread_data->length = length;
   thread_data->counterString = counterString;

   evas_gl_thread_cmd_enqueue(EVAS_GL_THREAD_TYPE_EVGL,
                              _evgl_api_thread_glGetPerfMonitorCounterStringAMD,
                              thread_data,
                              thread_mode);
}

/*
 * void
 * glGetPerfMonitorCounterInfoAMD(GLuint group, GLuint counter, GLenum pname, void* data);
 */

typedef struct
{
   GLuint group;
   GLuint counter;
   GLenum pname;
   void* data;

} EVGL_API_Thread_Command_glGetPerfMonitorCounterInfoAMD;

void (*orig_evgl_api_glGetPerfMonitorCounterInfoAMD)(GLuint group, GLuint counter, GLenum pname, void* data);

static void
_evgl_api_thread_glGetPerfMonitorCounterInfoAMD(void *data)
{
   EVGL_API_Thread_Command_glGetPerfMonitorCounterInfoAMD *thread_data =
      (EVGL_API_Thread_Command_glGetPerfMonitorCounterInfoAMD *)data;

   orig_evgl_api_glGetPerfMonitorCounterInfoAMD(thread_data->group,
                                                thread_data->counter,
                                                thread_data->pname,
                                                thread_data->data);

}

EAPI void
glGetPerfMonitorCounterInfoAMD_evgl_api_thread_cmd(GLuint group, GLuint counter, GLenum pname, void* data)
{
   if (!evas_evgl_thread_enabled() || (_main_thread_id != eina_thread_self()))
     {
        orig_evgl_api_glGetPerfMonitorCounterInfoAMD(group, counter, pname, data);
        return;
     }

   int thread_mode = EVAS_GL_THREAD_MODE_FINISH;

   EVGL_API_Thread_Command_glGetPerfMonitorCounterInfoAMD thread_data_local;
   EVGL_API_Thread_Command_glGetPerfMonitorCounterInfoAMD *thread_data = &thread_data_local;

   thread_data->group = group;
   thread_data->counter = counter;
   thread_data->pname = pname;
   thread_data->data = data;

   evas_gl_thread_cmd_enqueue(EVAS_GL_THREAD_TYPE_EVGL,
                              _evgl_api_thread_glGetPerfMonitorCounterInfoAMD,
                              thread_data,
                              thread_mode);
}

/*
 * void
 * glGenPerfMonitorsAMD(GLsizei n, GLuint* monitors);
 */

typedef struct
{
   GLsizei n;
   GLuint* monitors;

} EVGL_API_Thread_Command_glGenPerfMonitorsAMD;

void (*orig_evgl_api_glGenPerfMonitorsAMD)(GLsizei n, GLuint* monitors);

static void
_evgl_api_thread_glGenPerfMonitorsAMD(void *data)
{
   EVGL_API_Thread_Command_glGenPerfMonitorsAMD *thread_data =
      (EVGL_API_Thread_Command_glGenPerfMonitorsAMD *)data;

   orig_evgl_api_glGenPerfMonitorsAMD(thread_data->n,
                                      thread_data->monitors);

}

EAPI void
glGenPerfMonitorsAMD_evgl_api_thread_cmd(GLsizei n, GLuint* monitors)
{
   if (!evas_evgl_thread_enabled() || (_main_thread_id != eina_thread_self()))
     {
        orig_evgl_api_glGenPerfMonitorsAMD(n, monitors);
        return;
     }

   int thread_mode = EVAS_GL_THREAD_MODE_FINISH;

   EVGL_API_Thread_Command_glGenPerfMonitorsAMD thread_data_local;
   EVGL_API_Thread_Command_glGenPerfMonitorsAMD *thread_data = &thread_data_local;

   thread_data->n = n;
   thread_data->monitors = monitors;

   evas_gl_thread_cmd_enqueue(EVAS_GL_THREAD_TYPE_EVGL,
                              _evgl_api_thread_glGenPerfMonitorsAMD,
                              thread_data,
                              thread_mode);
}

/*
 * void
 * glDeletePerfMonitorsAMD(GLsizei n, GLuint* monitors);
 */

typedef struct
{
   GLsizei n;
   GLuint* monitors;

} EVGL_API_Thread_Command_glDeletePerfMonitorsAMD;

void (*orig_evgl_api_glDeletePerfMonitorsAMD)(GLsizei n, GLuint* monitors);

static void
_evgl_api_thread_glDeletePerfMonitorsAMD(void *data)
{
   EVGL_API_Thread_Command_glDeletePerfMonitorsAMD *thread_data =
      (EVGL_API_Thread_Command_glDeletePerfMonitorsAMD *)data;

   orig_evgl_api_glDeletePerfMonitorsAMD(thread_data->n,
                                         thread_data->monitors);

}

EAPI void
glDeletePerfMonitorsAMD_evgl_api_thread_cmd(GLsizei n, GLuint* monitors)
{
   if (!evas_evgl_thread_enabled() || (_main_thread_id != eina_thread_self()))
     {
        orig_evgl_api_glDeletePerfMonitorsAMD(n, monitors);
        return;
     }

   int thread_mode = EVAS_GL_THREAD_MODE_FINISH;

   EVGL_API_Thread_Command_glDeletePerfMonitorsAMD thread_data_local;
   EVGL_API_Thread_Command_glDeletePerfMonitorsAMD *thread_data = &thread_data_local;

   thread_data->n = n;
   thread_data->monitors = monitors;

   evas_gl_thread_cmd_enqueue(EVAS_GL_THREAD_TYPE_EVGL,
                              _evgl_api_thread_glDeletePerfMonitorsAMD,
                              thread_data,
                              thread_mode);
}

/*
 * void
 * glSelectPerfMonitorCountersAMD(GLuint monitor, GLboolean enable, GLuint group, GLint numCounters, GLuint* countersList);
 */

typedef struct
{
   GLuint monitor;
   GLboolean enable;
   GLuint group;
   GLint numCounters;
   GLuint* countersList;

} EVGL_API_Thread_Command_glSelectPerfMonitorCountersAMD;

void (*orig_evgl_api_glSelectPerfMonitorCountersAMD)(GLuint monitor, GLboolean enable, GLuint group, GLint numCounters, GLuint* countersList);

static void
_evgl_api_thread_glSelectPerfMonitorCountersAMD(void *data)
{
   EVGL_API_Thread_Command_glSelectPerfMonitorCountersAMD *thread_data =
      (EVGL_API_Thread_Command_glSelectPerfMonitorCountersAMD *)data;

   orig_evgl_api_glSelectPerfMonitorCountersAMD(thread_data->monitor,
                                                thread_data->enable,
                                                thread_data->group,
                                                thread_data->numCounters,
                                                thread_data->countersList);

}

EAPI void
glSelectPerfMonitorCountersAMD_evgl_api_thread_cmd(GLuint monitor, GLboolean enable, GLuint group, GLint numCounters, GLuint* countersList)
{
   if (!evas_evgl_thread_enabled() || (_main_thread_id != eina_thread_self()))
     {
        orig_evgl_api_glSelectPerfMonitorCountersAMD(monitor, enable, group, numCounters, countersList);
        return;
     }

   int thread_mode = EVAS_GL_THREAD_MODE_FINISH;

   EVGL_API_Thread_Command_glSelectPerfMonitorCountersAMD thread_data_local;
   EVGL_API_Thread_Command_glSelectPerfMonitorCountersAMD *thread_data = &thread_data_local;

   thread_data->monitor = monitor;
   thread_data->enable = enable;
   thread_data->group = group;
   thread_data->numCounters = numCounters;
   thread_data->countersList = countersList;

   evas_gl_thread_cmd_enqueue(EVAS_GL_THREAD_TYPE_EVGL,
                              _evgl_api_thread_glSelectPerfMonitorCountersAMD,
                              thread_data,
                              thread_mode);
}

/*
 * void
 * glBeginPerfMonitorAMD(GLuint monitor);
 */

typedef struct
{
   GLuint monitor;

} EVGL_API_Thread_Command_glBeginPerfMonitorAMD;

void (*orig_evgl_api_glBeginPerfMonitorAMD)(GLuint monitor);

static void
_evgl_api_thread_glBeginPerfMonitorAMD(void *data)
{
   EVGL_API_Thread_Command_glBeginPerfMonitorAMD *thread_data =
      (EVGL_API_Thread_Command_glBeginPerfMonitorAMD *)data;

   orig_evgl_api_glBeginPerfMonitorAMD(thread_data->monitor);

}

EAPI void
glBeginPerfMonitorAMD_evgl_api_thread_cmd(GLuint monitor)
{
   if (!evas_evgl_thread_enabled() || (_main_thread_id != eina_thread_self()))
     {
        orig_evgl_api_glBeginPerfMonitorAMD(monitor);
        return;
     }

   int thread_mode = EVAS_GL_THREAD_MODE_FINISH;

   EVGL_API_Thread_Command_glBeginPerfMonitorAMD thread_data_local;
   EVGL_API_Thread_Command_glBeginPerfMonitorAMD *thread_data = &thread_data_local;

   thread_data->monitor = monitor;

   evas_gl_thread_cmd_enqueue(EVAS_GL_THREAD_TYPE_EVGL,
                              _evgl_api_thread_glBeginPerfMonitorAMD,
                              thread_data,
                              thread_mode);
}

/*
 * void
 * glEndPerfMonitorAMD(GLuint monitor);
 */

typedef struct
{
   GLuint monitor;

} EVGL_API_Thread_Command_glEndPerfMonitorAMD;

void (*orig_evgl_api_glEndPerfMonitorAMD)(GLuint monitor);

static void
_evgl_api_thread_glEndPerfMonitorAMD(void *data)
{
   EVGL_API_Thread_Command_glEndPerfMonitorAMD *thread_data =
      (EVGL_API_Thread_Command_glEndPerfMonitorAMD *)data;

   orig_evgl_api_glEndPerfMonitorAMD(thread_data->monitor);

}

EAPI void
glEndPerfMonitorAMD_evgl_api_thread_cmd(GLuint monitor)
{
   if (!evas_evgl_thread_enabled() || (_main_thread_id != eina_thread_self()))
     {
        orig_evgl_api_glEndPerfMonitorAMD(monitor);
        return;
     }

   int thread_mode = EVAS_GL_THREAD_MODE_FINISH;

   EVGL_API_Thread_Command_glEndPerfMonitorAMD thread_data_local;
   EVGL_API_Thread_Command_glEndPerfMonitorAMD *thread_data = &thread_data_local;

   thread_data->monitor = monitor;

   evas_gl_thread_cmd_enqueue(EVAS_GL_THREAD_TYPE_EVGL,
                              _evgl_api_thread_glEndPerfMonitorAMD,
                              thread_data,
                              thread_mode);
}

/*
 * void
 * glGetPerfMonitorCounterDataAMD(GLuint monitor, GLenum pname, GLsizei dataSize, GLuint* data, GLint* bytesWritten);
 */

typedef struct
{
   GLuint monitor;
   GLenum pname;
   GLsizei dataSize;
   GLuint* data;
   GLint* bytesWritten;

} EVGL_API_Thread_Command_glGetPerfMonitorCounterDataAMD;

void (*orig_evgl_api_glGetPerfMonitorCounterDataAMD)(GLuint monitor, GLenum pname, GLsizei dataSize, GLuint* data, GLint* bytesWritten);

static void
_evgl_api_thread_glGetPerfMonitorCounterDataAMD(void *data)
{
   EVGL_API_Thread_Command_glGetPerfMonitorCounterDataAMD *thread_data =
      (EVGL_API_Thread_Command_glGetPerfMonitorCounterDataAMD *)data;

   orig_evgl_api_glGetPerfMonitorCounterDataAMD(thread_data->monitor,
                                                thread_data->pname,
                                                thread_data->dataSize,
                                                thread_data->data,
                                                thread_data->bytesWritten);

}

EAPI void
glGetPerfMonitorCounterDataAMD_evgl_api_thread_cmd(GLuint monitor, GLenum pname, GLsizei dataSize, GLuint* data, GLint* bytesWritten)
{
   if (!evas_evgl_thread_enabled() || (_main_thread_id != eina_thread_self()))
     {
        orig_evgl_api_glGetPerfMonitorCounterDataAMD(monitor, pname, dataSize, data, bytesWritten);
        return;
     }

   int thread_mode = EVAS_GL_THREAD_MODE_FINISH;

   EVGL_API_Thread_Command_glGetPerfMonitorCounterDataAMD thread_data_local;
   EVGL_API_Thread_Command_glGetPerfMonitorCounterDataAMD *thread_data = &thread_data_local;

   thread_data->monitor = monitor;
   thread_data->pname = pname;
   thread_data->dataSize = dataSize;
   thread_data->data = data;
   thread_data->bytesWritten = bytesWritten;

   evas_gl_thread_cmd_enqueue(EVAS_GL_THREAD_TYPE_EVGL,
                              _evgl_api_thread_glGetPerfMonitorCounterDataAMD,
                              thread_data,
                              thread_mode);
}

/*
 * void
 * glDiscardFramebuffer(GLenum target, GLsizei numAttachments, const GLenum* attachments);
 */

typedef struct
{
   GLenum target;
   GLsizei numAttachments;
   const GLenum* attachments;

} EVGL_API_Thread_Command_glDiscardFramebuffer;

void (*orig_evgl_api_glDiscardFramebuffer)(GLenum target, GLsizei numAttachments, const GLenum* attachments);

static void
_evgl_api_thread_glDiscardFramebuffer(void *data)
{
   EVGL_API_Thread_Command_glDiscardFramebuffer *thread_data =
      (EVGL_API_Thread_Command_glDiscardFramebuffer *)data;

   orig_evgl_api_glDiscardFramebuffer(thread_data->target,
                                      thread_data->numAttachments,
                                      thread_data->attachments);

}

EAPI void
glDiscardFramebuffer_evgl_api_thread_cmd(GLenum target, GLsizei numAttachments, const GLenum* attachments)
{
   if (!evas_evgl_thread_enabled() || (_main_thread_id != eina_thread_self()))
     {
        orig_evgl_api_glDiscardFramebuffer(target, numAttachments, attachments);
        return;
     }

   int thread_mode = EVAS_GL_THREAD_MODE_FINISH;

   EVGL_API_Thread_Command_glDiscardFramebuffer thread_data_local;
   EVGL_API_Thread_Command_glDiscardFramebuffer *thread_data = &thread_data_local;

   thread_data->target = target;
   thread_data->numAttachments = numAttachments;
   thread_data->attachments = attachments;

   evas_gl_thread_cmd_enqueue(EVAS_GL_THREAD_TYPE_EVGL,
                              _evgl_api_thread_glDiscardFramebuffer,
                              thread_data,
                              thread_mode);
}

/*
 * void
 * glDiscardFramebufferEXT(GLenum target, GLsizei numAttachments, const GLenum* attachments);
 */

typedef struct
{
   GLenum target;
   GLsizei numAttachments;
   const GLenum* attachments;

} EVGL_API_Thread_Command_glDiscardFramebufferEXT;

void (*orig_evgl_api_glDiscardFramebufferEXT)(GLenum target, GLsizei numAttachments, const GLenum* attachments);

static void
_evgl_api_thread_glDiscardFramebufferEXT(void *data)
{
   EVGL_API_Thread_Command_glDiscardFramebufferEXT *thread_data =
      (EVGL_API_Thread_Command_glDiscardFramebufferEXT *)data;

   orig_evgl_api_glDiscardFramebufferEXT(thread_data->target,
                                         thread_data->numAttachments,
                                         thread_data->attachments);

}

EAPI void
glDiscardFramebufferEXT_evgl_api_thread_cmd(GLenum target, GLsizei numAttachments, const GLenum* attachments)
{
   if (!evas_evgl_thread_enabled() || (_main_thread_id != eina_thread_self()))
     {
        orig_evgl_api_glDiscardFramebufferEXT(target, numAttachments, attachments);
        return;
     }

   int thread_mode = EVAS_GL_THREAD_MODE_FINISH;

   EVGL_API_Thread_Command_glDiscardFramebufferEXT thread_data_local;
   EVGL_API_Thread_Command_glDiscardFramebufferEXT *thread_data = &thread_data_local;

   thread_data->target = target;
   thread_data->numAttachments = numAttachments;
   thread_data->attachments = attachments;

   evas_gl_thread_cmd_enqueue(EVAS_GL_THREAD_TYPE_EVGL,
                              _evgl_api_thread_glDiscardFramebufferEXT,
                              thread_data,
                              thread_mode);
}

/*
 * void
 * glMultiDrawArrays(GLenum mode, GLint* first, GLsizei* count, GLsizei primcount);
 */

typedef struct
{
   GLenum mode;
   GLint* first;
   GLsizei* count;
   GLsizei primcount;

} EVGL_API_Thread_Command_glMultiDrawArrays;

void (*orig_evgl_api_glMultiDrawArrays)(GLenum mode, GLint* first, GLsizei* count, GLsizei primcount);

static void
_evgl_api_thread_glMultiDrawArrays(void *data)
{
   EVGL_API_Thread_Command_glMultiDrawArrays *thread_data =
      (EVGL_API_Thread_Command_glMultiDrawArrays *)data;

   orig_evgl_api_glMultiDrawArrays(thread_data->mode,
                                   thread_data->first,
                                   thread_data->count,
                                   thread_data->primcount);

}

EAPI void
glMultiDrawArrays_evgl_api_thread_cmd(GLenum mode, GLint* first, GLsizei* count, GLsizei primcount)
{
   if (!evas_evgl_thread_enabled() || (_main_thread_id != eina_thread_self()))
     {
        orig_evgl_api_glMultiDrawArrays(mode, first, count, primcount);
        return;
     }

   int thread_mode = EVAS_GL_THREAD_MODE_FINISH;

   EVGL_API_Thread_Command_glMultiDrawArrays thread_data_local;
   EVGL_API_Thread_Command_glMultiDrawArrays *thread_data = &thread_data_local;

   thread_data->mode = mode;
   thread_data->first = first;
   thread_data->count = count;
   thread_data->primcount = primcount;

   evas_gl_thread_cmd_enqueue(EVAS_GL_THREAD_TYPE_EVGL,
                              _evgl_api_thread_glMultiDrawArrays,
                              thread_data,
                              thread_mode);
}

/*
 * void
 * glMultiDrawArraysEXT(GLenum mode, GLint* first, GLsizei* count, GLsizei primcount);
 */

typedef struct
{
   GLenum mode;
   GLint* first;
   GLsizei* count;
   GLsizei primcount;

} EVGL_API_Thread_Command_glMultiDrawArraysEXT;

void (*orig_evgl_api_glMultiDrawArraysEXT)(GLenum mode, GLint* first, GLsizei* count, GLsizei primcount);

static void
_evgl_api_thread_glMultiDrawArraysEXT(void *data)
{
   EVGL_API_Thread_Command_glMultiDrawArraysEXT *thread_data =
      (EVGL_API_Thread_Command_glMultiDrawArraysEXT *)data;

   orig_evgl_api_glMultiDrawArraysEXT(thread_data->mode,
                                      thread_data->first,
                                      thread_data->count,
                                      thread_data->primcount);

}

EAPI void
glMultiDrawArraysEXT_evgl_api_thread_cmd(GLenum mode, GLint* first, GLsizei* count, GLsizei primcount)
{
   if (!evas_evgl_thread_enabled() || (_main_thread_id != eina_thread_self()))
     {
        orig_evgl_api_glMultiDrawArraysEXT(mode, first, count, primcount);
        return;
     }

   int thread_mode = EVAS_GL_THREAD_MODE_FINISH;

   EVGL_API_Thread_Command_glMultiDrawArraysEXT thread_data_local;
   EVGL_API_Thread_Command_glMultiDrawArraysEXT *thread_data = &thread_data_local;

   thread_data->mode = mode;
   thread_data->first = first;
   thread_data->count = count;
   thread_data->primcount = primcount;

   evas_gl_thread_cmd_enqueue(EVAS_GL_THREAD_TYPE_EVGL,
                              _evgl_api_thread_glMultiDrawArraysEXT,
                              thread_data,
                              thread_mode);
}

/*
 * void
 * glMultiDrawElements(GLenum mode, const GLsizei* count, GLenum type, const GLvoid** indices, GLsizei primcount);
 */

typedef struct
{
   GLenum mode;
   const GLsizei* count;
   GLenum type;
   const GLvoid** indices;
   GLsizei primcount;

} EVGL_API_Thread_Command_glMultiDrawElements;

void (*orig_evgl_api_glMultiDrawElements)(GLenum mode, const GLsizei* count, GLenum type, const GLvoid** indices, GLsizei primcount);

static void
_evgl_api_thread_glMultiDrawElements(void *data)
{
   EVGL_API_Thread_Command_glMultiDrawElements *thread_data =
      (EVGL_API_Thread_Command_glMultiDrawElements *)data;

   orig_evgl_api_glMultiDrawElements(thread_data->mode,
                                     thread_data->count,
                                     thread_data->type,
                                     thread_data->indices,
                                     thread_data->primcount);

}

EAPI void
glMultiDrawElements_evgl_api_thread_cmd(GLenum mode, const GLsizei* count, GLenum type, const GLvoid** indices, GLsizei primcount)
{
   if (!evas_evgl_thread_enabled() || (_main_thread_id != eina_thread_self()))
     {
        orig_evgl_api_glMultiDrawElements(mode, count, type, indices, primcount);
        return;
     }

   int thread_mode = EVAS_GL_THREAD_MODE_FINISH;

   EVGL_API_Thread_Command_glMultiDrawElements thread_data_local;
   EVGL_API_Thread_Command_glMultiDrawElements *thread_data = &thread_data_local;

   thread_data->mode = mode;
   thread_data->count = count;
   thread_data->type = type;
   thread_data->indices = indices;
   thread_data->primcount = primcount;

   evas_gl_thread_cmd_enqueue(EVAS_GL_THREAD_TYPE_EVGL,
                              _evgl_api_thread_glMultiDrawElements,
                              thread_data,
                              thread_mode);
}

/*
 * void
 * glMultiDrawElementsEXT(GLenum mode, const GLsizei* count, GLenum type, const GLvoid** indices, GLsizei primcount);
 */

typedef struct
{
   GLenum mode;
   const GLsizei* count;
   GLenum type;
   const GLvoid** indices;
   GLsizei primcount;

} EVGL_API_Thread_Command_glMultiDrawElementsEXT;

void (*orig_evgl_api_glMultiDrawElementsEXT)(GLenum mode, const GLsizei* count, GLenum type, const GLvoid** indices, GLsizei primcount);

static void
_evgl_api_thread_glMultiDrawElementsEXT(void *data)
{
   EVGL_API_Thread_Command_glMultiDrawElementsEXT *thread_data =
      (EVGL_API_Thread_Command_glMultiDrawElementsEXT *)data;

   orig_evgl_api_glMultiDrawElementsEXT(thread_data->mode,
                                        thread_data->count,
                                        thread_data->type,
                                        thread_data->indices,
                                        thread_data->primcount);

}

EAPI void
glMultiDrawElementsEXT_evgl_api_thread_cmd(GLenum mode, const GLsizei* count, GLenum type, const GLvoid** indices, GLsizei primcount)
{
   if (!evas_evgl_thread_enabled() || (_main_thread_id != eina_thread_self()))
     {
        orig_evgl_api_glMultiDrawElementsEXT(mode, count, type, indices, primcount);
        return;
     }

   int thread_mode = EVAS_GL_THREAD_MODE_FINISH;

   EVGL_API_Thread_Command_glMultiDrawElementsEXT thread_data_local;
   EVGL_API_Thread_Command_glMultiDrawElementsEXT *thread_data = &thread_data_local;

   thread_data->mode = mode;
   thread_data->count = count;
   thread_data->type = type;
   thread_data->indices = indices;
   thread_data->primcount = primcount;

   evas_gl_thread_cmd_enqueue(EVAS_GL_THREAD_TYPE_EVGL,
                              _evgl_api_thread_glMultiDrawElementsEXT,
                              thread_data,
                              thread_mode);
}

/*
 * void
 * glDeleteFencesNV(GLsizei n, const GLuint* fences);
 */

typedef struct
{
   GLsizei n;
   const GLuint* fences;

} EVGL_API_Thread_Command_glDeleteFencesNV;

void (*orig_evgl_api_glDeleteFencesNV)(GLsizei n, const GLuint* fences);

static void
_evgl_api_thread_glDeleteFencesNV(void *data)
{
   EVGL_API_Thread_Command_glDeleteFencesNV *thread_data =
      (EVGL_API_Thread_Command_glDeleteFencesNV *)data;

   orig_evgl_api_glDeleteFencesNV(thread_data->n,
                                  thread_data->fences);

}

EAPI void
glDeleteFencesNV_evgl_api_thread_cmd(GLsizei n, const GLuint* fences)
{
   if (!evas_evgl_thread_enabled() || (_main_thread_id != eina_thread_self()))
     {
        orig_evgl_api_glDeleteFencesNV(n, fences);
        return;
     }

   int thread_mode = EVAS_GL_THREAD_MODE_FINISH;

   EVGL_API_Thread_Command_glDeleteFencesNV thread_data_local;
   EVGL_API_Thread_Command_glDeleteFencesNV *thread_data = &thread_data_local;

   thread_data->n = n;
   thread_data->fences = fences;

   evas_gl_thread_cmd_enqueue(EVAS_GL_THREAD_TYPE_EVGL,
                              _evgl_api_thread_glDeleteFencesNV,
                              thread_data,
                              thread_mode);
}

/*
 * void
 * glGenFencesNV(GLsizei n, GLuint* fences);
 */

typedef struct
{
   GLsizei n;
   GLuint* fences;

} EVGL_API_Thread_Command_glGenFencesNV;

void (*orig_evgl_api_glGenFencesNV)(GLsizei n, GLuint* fences);

static void
_evgl_api_thread_glGenFencesNV(void *data)
{
   EVGL_API_Thread_Command_glGenFencesNV *thread_data =
      (EVGL_API_Thread_Command_glGenFencesNV *)data;

   orig_evgl_api_glGenFencesNV(thread_data->n,
                               thread_data->fences);

}

EAPI void
glGenFencesNV_evgl_api_thread_cmd(GLsizei n, GLuint* fences)
{
   if (!evas_evgl_thread_enabled() || (_main_thread_id != eina_thread_self()))
     {
        orig_evgl_api_glGenFencesNV(n, fences);
        return;
     }

   int thread_mode = EVAS_GL_THREAD_MODE_FINISH;

   EVGL_API_Thread_Command_glGenFencesNV thread_data_local;
   EVGL_API_Thread_Command_glGenFencesNV *thread_data = &thread_data_local;

   thread_data->n = n;
   thread_data->fences = fences;

   evas_gl_thread_cmd_enqueue(EVAS_GL_THREAD_TYPE_EVGL,
                              _evgl_api_thread_glGenFencesNV,
                              thread_data,
                              thread_mode);
}

/*
 * GLboolean
 * glIsFenceNV(GLuint fence);
 */

typedef struct
{
   GLboolean return_value;
   GLuint fence;

} EVGL_API_Thread_Command_glIsFenceNV;

GLboolean (*orig_evgl_api_glIsFenceNV)(GLuint fence);

static void
_evgl_api_thread_glIsFenceNV(void *data)
{
   EVGL_API_Thread_Command_glIsFenceNV *thread_data =
      (EVGL_API_Thread_Command_glIsFenceNV *)data;

   thread_data->return_value = orig_evgl_api_glIsFenceNV(thread_data->fence);

}

EAPI GLboolean
glIsFenceNV_evgl_api_thread_cmd(GLuint fence)
{
   if (!evas_evgl_thread_enabled() || (_main_thread_id != eina_thread_self()))
     {
        return orig_evgl_api_glIsFenceNV(fence);
     }

   int thread_mode = EVAS_GL_THREAD_MODE_FINISH;

   EVGL_API_Thread_Command_glIsFenceNV thread_data_local;
   EVGL_API_Thread_Command_glIsFenceNV *thread_data = &thread_data_local;

   thread_data->fence = fence;

   evas_gl_thread_cmd_enqueue(EVAS_GL_THREAD_TYPE_EVGL,
                              _evgl_api_thread_glIsFenceNV,
                              thread_data,
                              thread_mode);

   return thread_data->return_value;
}

/*
 * GLboolean
 * glTestFenceNV(GLuint fence);
 */

typedef struct
{
   GLboolean return_value;
   GLuint fence;

} EVGL_API_Thread_Command_glTestFenceNV;

GLboolean (*orig_evgl_api_glTestFenceNV)(GLuint fence);

static void
_evgl_api_thread_glTestFenceNV(void *data)
{
   EVGL_API_Thread_Command_glTestFenceNV *thread_data =
      (EVGL_API_Thread_Command_glTestFenceNV *)data;

   thread_data->return_value = orig_evgl_api_glTestFenceNV(thread_data->fence);

}

EAPI GLboolean
glTestFenceNV_evgl_api_thread_cmd(GLuint fence)
{
   if (!evas_evgl_thread_enabled() || (_main_thread_id != eina_thread_self()))
     {
        return orig_evgl_api_glTestFenceNV(fence);
     }

   int thread_mode = EVAS_GL_THREAD_MODE_FINISH;

   EVGL_API_Thread_Command_glTestFenceNV thread_data_local;
   EVGL_API_Thread_Command_glTestFenceNV *thread_data = &thread_data_local;

   thread_data->fence = fence;

   evas_gl_thread_cmd_enqueue(EVAS_GL_THREAD_TYPE_EVGL,
                              _evgl_api_thread_glTestFenceNV,
                              thread_data,
                              thread_mode);

   return thread_data->return_value;
}

/*
 * void
 * glGetFenceivNV(GLuint fence, GLenum pname, GLint* params);
 */

typedef struct
{
   GLuint fence;
   GLenum pname;
   GLint* params;

} EVGL_API_Thread_Command_glGetFenceivNV;

void (*orig_evgl_api_glGetFenceivNV)(GLuint fence, GLenum pname, GLint* params);

static void
_evgl_api_thread_glGetFenceivNV(void *data)
{
   EVGL_API_Thread_Command_glGetFenceivNV *thread_data =
      (EVGL_API_Thread_Command_glGetFenceivNV *)data;

   orig_evgl_api_glGetFenceivNV(thread_data->fence,
                                thread_data->pname,
                                thread_data->params);

}

EAPI void
glGetFenceivNV_evgl_api_thread_cmd(GLuint fence, GLenum pname, GLint* params)
{
   if (!evas_evgl_thread_enabled() || (_main_thread_id != eina_thread_self()))
     {
        orig_evgl_api_glGetFenceivNV(fence, pname, params);
        return;
     }

   int thread_mode = EVAS_GL_THREAD_MODE_FINISH;

   EVGL_API_Thread_Command_glGetFenceivNV thread_data_local;
   EVGL_API_Thread_Command_glGetFenceivNV *thread_data = &thread_data_local;

   thread_data->fence = fence;
   thread_data->pname = pname;
   thread_data->params = params;

   evas_gl_thread_cmd_enqueue(EVAS_GL_THREAD_TYPE_EVGL,
                              _evgl_api_thread_glGetFenceivNV,
                              thread_data,
                              thread_mode);
}

/*
 * void
 * glFinishFenceNV(GLuint fence);
 */

typedef struct
{
   GLuint fence;

} EVGL_API_Thread_Command_glFinishFenceNV;

void (*orig_evgl_api_glFinishFenceNV)(GLuint fence);

static void
_evgl_api_thread_glFinishFenceNV(void *data)
{
   EVGL_API_Thread_Command_glFinishFenceNV *thread_data =
      (EVGL_API_Thread_Command_glFinishFenceNV *)data;

   orig_evgl_api_glFinishFenceNV(thread_data->fence);

}

EAPI void
glFinishFenceNV_evgl_api_thread_cmd(GLuint fence)
{
   if (!evas_evgl_thread_enabled() || (_main_thread_id != eina_thread_self()))
     {
        orig_evgl_api_glFinishFenceNV(fence);
        return;
     }

   int thread_mode = EVAS_GL_THREAD_MODE_FINISH;

   EVGL_API_Thread_Command_glFinishFenceNV thread_data_local;
   EVGL_API_Thread_Command_glFinishFenceNV *thread_data = &thread_data_local;

   thread_data->fence = fence;

   evas_gl_thread_cmd_enqueue(EVAS_GL_THREAD_TYPE_EVGL,
                              _evgl_api_thread_glFinishFenceNV,
                              thread_data,
                              thread_mode);
}

/*
 * void
 * glSetFenceNV(GLuint a, GLenum b);
 */

typedef struct
{
   GLuint a;
   GLenum b;

} EVGL_API_Thread_Command_glSetFenceNV;

void (*orig_evgl_api_glSetFenceNV)(GLuint a, GLenum b);

static void
_evgl_api_thread_glSetFenceNV(void *data)
{
   EVGL_API_Thread_Command_glSetFenceNV *thread_data =
      (EVGL_API_Thread_Command_glSetFenceNV *)data;

   orig_evgl_api_glSetFenceNV(thread_data->a,
                              thread_data->b);

}

EAPI void
glSetFenceNV_evgl_api_thread_cmd(GLuint a, GLenum b)
{
   if (!evas_evgl_thread_enabled() || (_main_thread_id != eina_thread_self()))
     {
        orig_evgl_api_glSetFenceNV(a, b);
        return;
     }

   int thread_mode = EVAS_GL_THREAD_MODE_FINISH;

   EVGL_API_Thread_Command_glSetFenceNV thread_data_local;
   EVGL_API_Thread_Command_glSetFenceNV *thread_data = &thread_data_local;

   thread_data->a = a;
   thread_data->b = b;

   evas_gl_thread_cmd_enqueue(EVAS_GL_THREAD_TYPE_EVGL,
                              _evgl_api_thread_glSetFenceNV,
                              thread_data,
                              thread_mode);
}

/*
 * void
 * glGetDriverControlsQCOM(GLint* num, GLsizei size, GLuint* driverControls);
 */

typedef struct
{
   GLint* num;
   GLsizei size;
   GLuint* driverControls;

} EVGL_API_Thread_Command_glGetDriverControlsQCOM;

void (*orig_evgl_api_glGetDriverControlsQCOM)(GLint* num, GLsizei size, GLuint* driverControls);

static void
_evgl_api_thread_glGetDriverControlsQCOM(void *data)
{
   EVGL_API_Thread_Command_glGetDriverControlsQCOM *thread_data =
      (EVGL_API_Thread_Command_glGetDriverControlsQCOM *)data;

   orig_evgl_api_glGetDriverControlsQCOM(thread_data->num,
                                         thread_data->size,
                                         thread_data->driverControls);

}

EAPI void
glGetDriverControlsQCOM_evgl_api_thread_cmd(GLint* num, GLsizei size, GLuint* driverControls)
{
   if (!evas_evgl_thread_enabled() || (_main_thread_id != eina_thread_self()))
     {
        orig_evgl_api_glGetDriverControlsQCOM(num, size, driverControls);
        return;
     }

   int thread_mode = EVAS_GL_THREAD_MODE_FINISH;

   EVGL_API_Thread_Command_glGetDriverControlsQCOM thread_data_local;
   EVGL_API_Thread_Command_glGetDriverControlsQCOM *thread_data = &thread_data_local;

   thread_data->num = num;
   thread_data->size = size;
   thread_data->driverControls = driverControls;

   evas_gl_thread_cmd_enqueue(EVAS_GL_THREAD_TYPE_EVGL,
                              _evgl_api_thread_glGetDriverControlsQCOM,
                              thread_data,
                              thread_mode);
}

/*
 * void
 * glGetDriverControlStringQCOM(GLuint driverControl, GLsizei bufSize, GLsizei* length, char* driverControlString);
 */

typedef struct
{
   GLuint driverControl;
   GLsizei bufSize;
   GLsizei* length;
   char* driverControlString;

} EVGL_API_Thread_Command_glGetDriverControlStringQCOM;

void (*orig_evgl_api_glGetDriverControlStringQCOM)(GLuint driverControl, GLsizei bufSize, GLsizei* length, char* driverControlString);

static void
_evgl_api_thread_glGetDriverControlStringQCOM(void *data)
{
   EVGL_API_Thread_Command_glGetDriverControlStringQCOM *thread_data =
      (EVGL_API_Thread_Command_glGetDriverControlStringQCOM *)data;

   orig_evgl_api_glGetDriverControlStringQCOM(thread_data->driverControl,
                                              thread_data->bufSize,
                                              thread_data->length,
                                              thread_data->driverControlString);

}

EAPI void
glGetDriverControlStringQCOM_evgl_api_thread_cmd(GLuint driverControl, GLsizei bufSize, GLsizei* length, char* driverControlString)
{
   if (!evas_evgl_thread_enabled() || (_main_thread_id != eina_thread_self()))
     {
        orig_evgl_api_glGetDriverControlStringQCOM(driverControl, bufSize, length, driverControlString);
        return;
     }

   int thread_mode = EVAS_GL_THREAD_MODE_FINISH;

   EVGL_API_Thread_Command_glGetDriverControlStringQCOM thread_data_local;
   EVGL_API_Thread_Command_glGetDriverControlStringQCOM *thread_data = &thread_data_local;

   thread_data->driverControl = driverControl;
   thread_data->bufSize = bufSize;
   thread_data->length = length;
   thread_data->driverControlString = driverControlString;

   evas_gl_thread_cmd_enqueue(EVAS_GL_THREAD_TYPE_EVGL,
                              _evgl_api_thread_glGetDriverControlStringQCOM,
                              thread_data,
                              thread_mode);
}

/*
 * void
 * glEnableDriverControlQCOM(GLuint driverControl);
 */

typedef struct
{
   GLuint driverControl;

} EVGL_API_Thread_Command_glEnableDriverControlQCOM;

void (*orig_evgl_api_glEnableDriverControlQCOM)(GLuint driverControl);

static void
_evgl_api_thread_glEnableDriverControlQCOM(void *data)
{
   EVGL_API_Thread_Command_glEnableDriverControlQCOM *thread_data =
      (EVGL_API_Thread_Command_glEnableDriverControlQCOM *)data;

   orig_evgl_api_glEnableDriverControlQCOM(thread_data->driverControl);

}

EAPI void
glEnableDriverControlQCOM_evgl_api_thread_cmd(GLuint driverControl)
{
   if (!evas_evgl_thread_enabled() || (_main_thread_id != eina_thread_self()))
     {
        orig_evgl_api_glEnableDriverControlQCOM(driverControl);
        return;
     }

   int thread_mode = EVAS_GL_THREAD_MODE_FINISH;

   EVGL_API_Thread_Command_glEnableDriverControlQCOM thread_data_local;
   EVGL_API_Thread_Command_glEnableDriverControlQCOM *thread_data = &thread_data_local;

   thread_data->driverControl = driverControl;

   evas_gl_thread_cmd_enqueue(EVAS_GL_THREAD_TYPE_EVGL,
                              _evgl_api_thread_glEnableDriverControlQCOM,
                              thread_data,
                              thread_mode);
}

/*
 * void
 * glDisableDriverControlQCOM(GLuint driverControl);
 */

typedef struct
{
   GLuint driverControl;

} EVGL_API_Thread_Command_glDisableDriverControlQCOM;

void (*orig_evgl_api_glDisableDriverControlQCOM)(GLuint driverControl);

static void
_evgl_api_thread_glDisableDriverControlQCOM(void *data)
{
   EVGL_API_Thread_Command_glDisableDriverControlQCOM *thread_data =
      (EVGL_API_Thread_Command_glDisableDriverControlQCOM *)data;

   orig_evgl_api_glDisableDriverControlQCOM(thread_data->driverControl);

}

EAPI void
glDisableDriverControlQCOM_evgl_api_thread_cmd(GLuint driverControl)
{
   if (!evas_evgl_thread_enabled() || (_main_thread_id != eina_thread_self()))
     {
        orig_evgl_api_glDisableDriverControlQCOM(driverControl);
        return;
     }

   int thread_mode = EVAS_GL_THREAD_MODE_FINISH;

   EVGL_API_Thread_Command_glDisableDriverControlQCOM thread_data_local;
   EVGL_API_Thread_Command_glDisableDriverControlQCOM *thread_data = &thread_data_local;

   thread_data->driverControl = driverControl;

   evas_gl_thread_cmd_enqueue(EVAS_GL_THREAD_TYPE_EVGL,
                              _evgl_api_thread_glDisableDriverControlQCOM,
                              thread_data,
                              thread_mode);
}

/*
 * void
 * glExtGetTexturesQCOM(GLuint* textures, GLint maxTextures, GLint* numTextures);
 */

typedef struct
{
   GLuint* textures;
   GLint maxTextures;
   GLint* numTextures;

} EVGL_API_Thread_Command_glExtGetTexturesQCOM;

void (*orig_evgl_api_glExtGetTexturesQCOM)(GLuint* textures, GLint maxTextures, GLint* numTextures);

static void
_evgl_api_thread_glExtGetTexturesQCOM(void *data)
{
   EVGL_API_Thread_Command_glExtGetTexturesQCOM *thread_data =
      (EVGL_API_Thread_Command_glExtGetTexturesQCOM *)data;

   orig_evgl_api_glExtGetTexturesQCOM(thread_data->textures,
                                      thread_data->maxTextures,
                                      thread_data->numTextures);

}

EAPI void
glExtGetTexturesQCOM_evgl_api_thread_cmd(GLuint* textures, GLint maxTextures, GLint* numTextures)
{
   if (!evas_evgl_thread_enabled() || (_main_thread_id != eina_thread_self()))
     {
        orig_evgl_api_glExtGetTexturesQCOM(textures, maxTextures, numTextures);
        return;
     }

   int thread_mode = EVAS_GL_THREAD_MODE_FINISH;

   EVGL_API_Thread_Command_glExtGetTexturesQCOM thread_data_local;
   EVGL_API_Thread_Command_glExtGetTexturesQCOM *thread_data = &thread_data_local;

   thread_data->textures = textures;
   thread_data->maxTextures = maxTextures;
   thread_data->numTextures = numTextures;

   evas_gl_thread_cmd_enqueue(EVAS_GL_THREAD_TYPE_EVGL,
                              _evgl_api_thread_glExtGetTexturesQCOM,
                              thread_data,
                              thread_mode);
}

/*
 * void
 * glExtGetBuffersQCOM(GLuint* buffers, GLint maxBuffers, GLint* numBuffers);
 */

typedef struct
{
   GLuint* buffers;
   GLint maxBuffers;
   GLint* numBuffers;

} EVGL_API_Thread_Command_glExtGetBuffersQCOM;

void (*orig_evgl_api_glExtGetBuffersQCOM)(GLuint* buffers, GLint maxBuffers, GLint* numBuffers);

static void
_evgl_api_thread_glExtGetBuffersQCOM(void *data)
{
   EVGL_API_Thread_Command_glExtGetBuffersQCOM *thread_data =
      (EVGL_API_Thread_Command_glExtGetBuffersQCOM *)data;

   orig_evgl_api_glExtGetBuffersQCOM(thread_data->buffers,
                                     thread_data->maxBuffers,
                                     thread_data->numBuffers);

}

EAPI void
glExtGetBuffersQCOM_evgl_api_thread_cmd(GLuint* buffers, GLint maxBuffers, GLint* numBuffers)
{
   if (!evas_evgl_thread_enabled() || (_main_thread_id != eina_thread_self()))
     {
        orig_evgl_api_glExtGetBuffersQCOM(buffers, maxBuffers, numBuffers);
        return;
     }

   int thread_mode = EVAS_GL_THREAD_MODE_FINISH;

   EVGL_API_Thread_Command_glExtGetBuffersQCOM thread_data_local;
   EVGL_API_Thread_Command_glExtGetBuffersQCOM *thread_data = &thread_data_local;

   thread_data->buffers = buffers;
   thread_data->maxBuffers = maxBuffers;
   thread_data->numBuffers = numBuffers;

   evas_gl_thread_cmd_enqueue(EVAS_GL_THREAD_TYPE_EVGL,
                              _evgl_api_thread_glExtGetBuffersQCOM,
                              thread_data,
                              thread_mode);
}

/*
 * void
 * glExtGetRenderbuffersQCOM(GLuint* renderbuffers, GLint maxRenderbuffers, GLint* numRenderbuffers);
 */

typedef struct
{
   GLuint* renderbuffers;
   GLint maxRenderbuffers;
   GLint* numRenderbuffers;

} EVGL_API_Thread_Command_glExtGetRenderbuffersQCOM;

void (*orig_evgl_api_glExtGetRenderbuffersQCOM)(GLuint* renderbuffers, GLint maxRenderbuffers, GLint* numRenderbuffers);

static void
_evgl_api_thread_glExtGetRenderbuffersQCOM(void *data)
{
   EVGL_API_Thread_Command_glExtGetRenderbuffersQCOM *thread_data =
      (EVGL_API_Thread_Command_glExtGetRenderbuffersQCOM *)data;

   orig_evgl_api_glExtGetRenderbuffersQCOM(thread_data->renderbuffers,
                                           thread_data->maxRenderbuffers,
                                           thread_data->numRenderbuffers);

}

EAPI void
glExtGetRenderbuffersQCOM_evgl_api_thread_cmd(GLuint* renderbuffers, GLint maxRenderbuffers, GLint* numRenderbuffers)
{
   if (!evas_evgl_thread_enabled() || (_main_thread_id != eina_thread_self()))
     {
        orig_evgl_api_glExtGetRenderbuffersQCOM(renderbuffers, maxRenderbuffers, numRenderbuffers);
        return;
     }

   int thread_mode = EVAS_GL_THREAD_MODE_FINISH;

   EVGL_API_Thread_Command_glExtGetRenderbuffersQCOM thread_data_local;
   EVGL_API_Thread_Command_glExtGetRenderbuffersQCOM *thread_data = &thread_data_local;

   thread_data->renderbuffers = renderbuffers;
   thread_data->maxRenderbuffers = maxRenderbuffers;
   thread_data->numRenderbuffers = numRenderbuffers;

   evas_gl_thread_cmd_enqueue(EVAS_GL_THREAD_TYPE_EVGL,
                              _evgl_api_thread_glExtGetRenderbuffersQCOM,
                              thread_data,
                              thread_mode);
}

/*
 * void
 * glExtGetFramebuffersQCOM(GLuint* framebuffers, GLint maxFramebuffers, GLint* numFramebuffers);
 */

typedef struct
{
   GLuint* framebuffers;
   GLint maxFramebuffers;
   GLint* numFramebuffers;

} EVGL_API_Thread_Command_glExtGetFramebuffersQCOM;

void (*orig_evgl_api_glExtGetFramebuffersQCOM)(GLuint* framebuffers, GLint maxFramebuffers, GLint* numFramebuffers);

static void
_evgl_api_thread_glExtGetFramebuffersQCOM(void *data)
{
   EVGL_API_Thread_Command_glExtGetFramebuffersQCOM *thread_data =
      (EVGL_API_Thread_Command_glExtGetFramebuffersQCOM *)data;

   orig_evgl_api_glExtGetFramebuffersQCOM(thread_data->framebuffers,
                                          thread_data->maxFramebuffers,
                                          thread_data->numFramebuffers);

}

EAPI void
glExtGetFramebuffersQCOM_evgl_api_thread_cmd(GLuint* framebuffers, GLint maxFramebuffers, GLint* numFramebuffers)
{
   if (!evas_evgl_thread_enabled() || (_main_thread_id != eina_thread_self()))
     {
        orig_evgl_api_glExtGetFramebuffersQCOM(framebuffers, maxFramebuffers, numFramebuffers);
        return;
     }

   int thread_mode = EVAS_GL_THREAD_MODE_FINISH;

   EVGL_API_Thread_Command_glExtGetFramebuffersQCOM thread_data_local;
   EVGL_API_Thread_Command_glExtGetFramebuffersQCOM *thread_data = &thread_data_local;

   thread_data->framebuffers = framebuffers;
   thread_data->maxFramebuffers = maxFramebuffers;
   thread_data->numFramebuffers = numFramebuffers;

   evas_gl_thread_cmd_enqueue(EVAS_GL_THREAD_TYPE_EVGL,
                              _evgl_api_thread_glExtGetFramebuffersQCOM,
                              thread_data,
                              thread_mode);
}

/*
 * void
 * glExtGetTexLevelParameterivQCOM(GLuint texture, GLenum face, GLint level, GLenum pname, GLint* params);
 */

typedef struct
{
   GLuint texture;
   GLenum face;
   GLint level;
   GLenum pname;
   GLint* params;

} EVGL_API_Thread_Command_glExtGetTexLevelParameterivQCOM;

void (*orig_evgl_api_glExtGetTexLevelParameterivQCOM)(GLuint texture, GLenum face, GLint level, GLenum pname, GLint* params);

static void
_evgl_api_thread_glExtGetTexLevelParameterivQCOM(void *data)
{
   EVGL_API_Thread_Command_glExtGetTexLevelParameterivQCOM *thread_data =
      (EVGL_API_Thread_Command_glExtGetTexLevelParameterivQCOM *)data;

   orig_evgl_api_glExtGetTexLevelParameterivQCOM(thread_data->texture,
                                                 thread_data->face,
                                                 thread_data->level,
                                                 thread_data->pname,
                                                 thread_data->params);

}

EAPI void
glExtGetTexLevelParameterivQCOM_evgl_api_thread_cmd(GLuint texture, GLenum face, GLint level, GLenum pname, GLint* params)
{
   if (!evas_evgl_thread_enabled() || (_main_thread_id != eina_thread_self()))
     {
        orig_evgl_api_glExtGetTexLevelParameterivQCOM(texture, face, level, pname, params);
        return;
     }

   int thread_mode = EVAS_GL_THREAD_MODE_FINISH;

   EVGL_API_Thread_Command_glExtGetTexLevelParameterivQCOM thread_data_local;
   EVGL_API_Thread_Command_glExtGetTexLevelParameterivQCOM *thread_data = &thread_data_local;

   thread_data->texture = texture;
   thread_data->face = face;
   thread_data->level = level;
   thread_data->pname = pname;
   thread_data->params = params;

   evas_gl_thread_cmd_enqueue(EVAS_GL_THREAD_TYPE_EVGL,
                              _evgl_api_thread_glExtGetTexLevelParameterivQCOM,
                              thread_data,
                              thread_mode);
}

/*
 * void
 * glExtTexObjectStateOverrideiQCOM(GLenum target, GLenum pname, GLint param);
 */

typedef struct
{
   GLenum target;
   GLenum pname;
   GLint param;

} EVGL_API_Thread_Command_glExtTexObjectStateOverrideiQCOM;

void (*orig_evgl_api_glExtTexObjectStateOverrideiQCOM)(GLenum target, GLenum pname, GLint param);

static void
_evgl_api_thread_glExtTexObjectStateOverrideiQCOM(void *data)
{
   EVGL_API_Thread_Command_glExtTexObjectStateOverrideiQCOM *thread_data =
      (EVGL_API_Thread_Command_glExtTexObjectStateOverrideiQCOM *)data;

   orig_evgl_api_glExtTexObjectStateOverrideiQCOM(thread_data->target,
                                                  thread_data->pname,
                                                  thread_data->param);

}

EAPI void
glExtTexObjectStateOverrideiQCOM_evgl_api_thread_cmd(GLenum target, GLenum pname, GLint param)
{
   if (!evas_evgl_thread_enabled() || (_main_thread_id != eina_thread_self()))
     {
        orig_evgl_api_glExtTexObjectStateOverrideiQCOM(target, pname, param);
        return;
     }

   int thread_mode = EVAS_GL_THREAD_MODE_FINISH;

   EVGL_API_Thread_Command_glExtTexObjectStateOverrideiQCOM thread_data_local;
   EVGL_API_Thread_Command_glExtTexObjectStateOverrideiQCOM *thread_data = &thread_data_local;

   thread_data->target = target;
   thread_data->pname = pname;
   thread_data->param = param;

   evas_gl_thread_cmd_enqueue(EVAS_GL_THREAD_TYPE_EVGL,
                              _evgl_api_thread_glExtTexObjectStateOverrideiQCOM,
                              thread_data,
                              thread_mode);
}

/*
 * void
 * glExtGetTexSubImageQCOM(GLenum target, GLint level, GLint xoffset, GLint yoffset, GLint zoffset, GLsizei width, GLsizei height, GLsizei depth, GLenum format, GLenum type, void* texels);
 */

typedef struct
{
   GLenum target;
   GLint level;
   GLint xoffset;
   GLint yoffset;
   GLint zoffset;
   GLsizei width;
   GLsizei height;
   GLsizei depth;
   GLenum format;
   GLenum type;
   void* texels;

} EVGL_API_Thread_Command_glExtGetTexSubImageQCOM;

void (*orig_evgl_api_glExtGetTexSubImageQCOM)(GLenum target, GLint level, GLint xoffset, GLint yoffset, GLint zoffset, GLsizei width, GLsizei height, GLsizei depth, GLenum format, GLenum type, void* texels);

static void
_evgl_api_thread_glExtGetTexSubImageQCOM(void *data)
{
   EVGL_API_Thread_Command_glExtGetTexSubImageQCOM *thread_data =
      (EVGL_API_Thread_Command_glExtGetTexSubImageQCOM *)data;

   orig_evgl_api_glExtGetTexSubImageQCOM(thread_data->target,
                                         thread_data->level,
                                         thread_data->xoffset,
                                         thread_data->yoffset,
                                         thread_data->zoffset,
                                         thread_data->width,
                                         thread_data->height,
                                         thread_data->depth,
                                         thread_data->format,
                                         thread_data->type,
                                         thread_data->texels);

}

EAPI void
glExtGetTexSubImageQCOM_evgl_api_thread_cmd(GLenum target, GLint level, GLint xoffset, GLint yoffset, GLint zoffset, GLsizei width, GLsizei height, GLsizei depth, GLenum format, GLenum type, void* texels)
{
   if (!evas_evgl_thread_enabled() || (_main_thread_id != eina_thread_self()))
     {
        orig_evgl_api_glExtGetTexSubImageQCOM(target, level, xoffset, yoffset, zoffset, width, height, depth, format, type, texels);
        return;
     }

   int thread_mode = EVAS_GL_THREAD_MODE_FINISH;

   EVGL_API_Thread_Command_glExtGetTexSubImageQCOM thread_data_local;
   EVGL_API_Thread_Command_glExtGetTexSubImageQCOM *thread_data = &thread_data_local;

   thread_data->target = target;
   thread_data->level = level;
   thread_data->xoffset = xoffset;
   thread_data->yoffset = yoffset;
   thread_data->zoffset = zoffset;
   thread_data->width = width;
   thread_data->height = height;
   thread_data->depth = depth;
   thread_data->format = format;
   thread_data->type = type;
   thread_data->texels = texels;

   evas_gl_thread_cmd_enqueue(EVAS_GL_THREAD_TYPE_EVGL,
                              _evgl_api_thread_glExtGetTexSubImageQCOM,
                              thread_data,
                              thread_mode);
}

/*
 * void
 * glExtGetBufferPointervQCOM(GLenum target, void** params);
 */

typedef struct
{
   GLenum target;
   void** params;

} EVGL_API_Thread_Command_glExtGetBufferPointervQCOM;

void (*orig_evgl_api_glExtGetBufferPointervQCOM)(GLenum target, void** params);

static void
_evgl_api_thread_glExtGetBufferPointervQCOM(void *data)
{
   EVGL_API_Thread_Command_glExtGetBufferPointervQCOM *thread_data =
      (EVGL_API_Thread_Command_glExtGetBufferPointervQCOM *)data;

   orig_evgl_api_glExtGetBufferPointervQCOM(thread_data->target,
                                            thread_data->params);

}

EAPI void
glExtGetBufferPointervQCOM_evgl_api_thread_cmd(GLenum target, void** params)
{
   if (!evas_evgl_thread_enabled() || (_main_thread_id != eina_thread_self()))
     {
        orig_evgl_api_glExtGetBufferPointervQCOM(target, params);
        return;
     }

   int thread_mode = EVAS_GL_THREAD_MODE_FINISH;

   EVGL_API_Thread_Command_glExtGetBufferPointervQCOM thread_data_local;
   EVGL_API_Thread_Command_glExtGetBufferPointervQCOM *thread_data = &thread_data_local;

   thread_data->target = target;
   thread_data->params = params;

   evas_gl_thread_cmd_enqueue(EVAS_GL_THREAD_TYPE_EVGL,
                              _evgl_api_thread_glExtGetBufferPointervQCOM,
                              thread_data,
                              thread_mode);
}

/*
 * void
 * glExtGetShadersQCOM(GLuint* shaders, GLint maxShaders, GLint* numShaders);
 */

typedef struct
{
   GLuint* shaders;
   GLint maxShaders;
   GLint* numShaders;

} EVGL_API_Thread_Command_glExtGetShadersQCOM;

void (*orig_evgl_api_glExtGetShadersQCOM)(GLuint* shaders, GLint maxShaders, GLint* numShaders);

static void
_evgl_api_thread_glExtGetShadersQCOM(void *data)
{
   EVGL_API_Thread_Command_glExtGetShadersQCOM *thread_data =
      (EVGL_API_Thread_Command_glExtGetShadersQCOM *)data;

   orig_evgl_api_glExtGetShadersQCOM(thread_data->shaders,
                                     thread_data->maxShaders,
                                     thread_data->numShaders);

}

EAPI void
glExtGetShadersQCOM_evgl_api_thread_cmd(GLuint* shaders, GLint maxShaders, GLint* numShaders)
{
   if (!evas_evgl_thread_enabled() || (_main_thread_id != eina_thread_self()))
     {
        orig_evgl_api_glExtGetShadersQCOM(shaders, maxShaders, numShaders);
        return;
     }

   int thread_mode = EVAS_GL_THREAD_MODE_FINISH;

   EVGL_API_Thread_Command_glExtGetShadersQCOM thread_data_local;
   EVGL_API_Thread_Command_glExtGetShadersQCOM *thread_data = &thread_data_local;

   thread_data->shaders = shaders;
   thread_data->maxShaders = maxShaders;
   thread_data->numShaders = numShaders;

   evas_gl_thread_cmd_enqueue(EVAS_GL_THREAD_TYPE_EVGL,
                              _evgl_api_thread_glExtGetShadersQCOM,
                              thread_data,
                              thread_mode);
}

/*
 * void
 * glExtGetProgramsQCOM(GLuint* programs, GLint maxPrograms, GLint* numPrograms);
 */

typedef struct
{
   GLuint* programs;
   GLint maxPrograms;
   GLint* numPrograms;

} EVGL_API_Thread_Command_glExtGetProgramsQCOM;

void (*orig_evgl_api_glExtGetProgramsQCOM)(GLuint* programs, GLint maxPrograms, GLint* numPrograms);

static void
_evgl_api_thread_glExtGetProgramsQCOM(void *data)
{
   EVGL_API_Thread_Command_glExtGetProgramsQCOM *thread_data =
      (EVGL_API_Thread_Command_glExtGetProgramsQCOM *)data;

   orig_evgl_api_glExtGetProgramsQCOM(thread_data->programs,
                                      thread_data->maxPrograms,
                                      thread_data->numPrograms);

}

EAPI void
glExtGetProgramsQCOM_evgl_api_thread_cmd(GLuint* programs, GLint maxPrograms, GLint* numPrograms)
{
   if (!evas_evgl_thread_enabled() || (_main_thread_id != eina_thread_self()))
     {
        orig_evgl_api_glExtGetProgramsQCOM(programs, maxPrograms, numPrograms);
        return;
     }

   int thread_mode = EVAS_GL_THREAD_MODE_FINISH;

   EVGL_API_Thread_Command_glExtGetProgramsQCOM thread_data_local;
   EVGL_API_Thread_Command_glExtGetProgramsQCOM *thread_data = &thread_data_local;

   thread_data->programs = programs;
   thread_data->maxPrograms = maxPrograms;
   thread_data->numPrograms = numPrograms;

   evas_gl_thread_cmd_enqueue(EVAS_GL_THREAD_TYPE_EVGL,
                              _evgl_api_thread_glExtGetProgramsQCOM,
                              thread_data,
                              thread_mode);
}

/*
 * GLboolean
 * glExtIsProgramBinaryQCOM(GLuint program);
 */

typedef struct
{
   GLboolean return_value;
   GLuint program;

} EVGL_API_Thread_Command_glExtIsProgramBinaryQCOM;

GLboolean (*orig_evgl_api_glExtIsProgramBinaryQCOM)(GLuint program);

static void
_evgl_api_thread_glExtIsProgramBinaryQCOM(void *data)
{
   EVGL_API_Thread_Command_glExtIsProgramBinaryQCOM *thread_data =
      (EVGL_API_Thread_Command_glExtIsProgramBinaryQCOM *)data;

   thread_data->return_value = orig_evgl_api_glExtIsProgramBinaryQCOM(thread_data->program);

}

EAPI GLboolean
glExtIsProgramBinaryQCOM_evgl_api_thread_cmd(GLuint program)
{
   if (!evas_evgl_thread_enabled() || (_main_thread_id != eina_thread_self()))
     {
        return orig_evgl_api_glExtIsProgramBinaryQCOM(program);
     }

   int thread_mode = EVAS_GL_THREAD_MODE_FINISH;

   EVGL_API_Thread_Command_glExtIsProgramBinaryQCOM thread_data_local;
   EVGL_API_Thread_Command_glExtIsProgramBinaryQCOM *thread_data = &thread_data_local;

   thread_data->program = program;

   evas_gl_thread_cmd_enqueue(EVAS_GL_THREAD_TYPE_EVGL,
                              _evgl_api_thread_glExtIsProgramBinaryQCOM,
                              thread_data,
                              thread_mode);

   return thread_data->return_value;
}

/*
 * void
 * glExtGetProgramBinarySourceQCOM(GLuint program, GLenum shadertype, char* source, GLint* length);
 */

typedef struct
{
   GLuint program;
   GLenum shadertype;
   char* source;
   GLint* length;

} EVGL_API_Thread_Command_glExtGetProgramBinarySourceQCOM;

void (*orig_evgl_api_glExtGetProgramBinarySourceQCOM)(GLuint program, GLenum shadertype, char* source, GLint* length);

static void
_evgl_api_thread_glExtGetProgramBinarySourceQCOM(void *data)
{
   EVGL_API_Thread_Command_glExtGetProgramBinarySourceQCOM *thread_data =
      (EVGL_API_Thread_Command_glExtGetProgramBinarySourceQCOM *)data;

   orig_evgl_api_glExtGetProgramBinarySourceQCOM(thread_data->program,
                                                 thread_data->shadertype,
                                                 thread_data->source,
                                                 thread_data->length);

}

EAPI void
glExtGetProgramBinarySourceQCOM_evgl_api_thread_cmd(GLuint program, GLenum shadertype, char* source, GLint* length)
{
   if (!evas_evgl_thread_enabled() || (_main_thread_id != eina_thread_self()))
     {
        orig_evgl_api_glExtGetProgramBinarySourceQCOM(program, shadertype, source, length);
        return;
     }

   int thread_mode = EVAS_GL_THREAD_MODE_FINISH;

   EVGL_API_Thread_Command_glExtGetProgramBinarySourceQCOM thread_data_local;
   EVGL_API_Thread_Command_glExtGetProgramBinarySourceQCOM *thread_data = &thread_data_local;

   thread_data->program = program;
   thread_data->shadertype = shadertype;
   thread_data->source = source;
   thread_data->length = length;

   evas_gl_thread_cmd_enqueue(EVAS_GL_THREAD_TYPE_EVGL,
                              _evgl_api_thread_glExtGetProgramBinarySourceQCOM,
                              thread_data,
                              thread_mode);
}

/*
 * void
 * glAlphaFunc(GLenum func, GLclampf ref);
 */

typedef struct
{
   GLenum func;
   GLclampf ref;

} EVGL_API_Thread_Command_glAlphaFunc;

void (*orig_evgl_api_glAlphaFunc)(GLenum func, GLclampf ref);

static void
_evgl_api_thread_glAlphaFunc(void *data)
{
   EVGL_API_Thread_Command_glAlphaFunc *thread_data =
      (EVGL_API_Thread_Command_glAlphaFunc *)data;

   orig_evgl_api_glAlphaFunc(thread_data->func,
                             thread_data->ref);

}

EAPI void
glAlphaFunc_evgl_api_thread_cmd(GLenum func, GLclampf ref)
{
   if (!evas_evgl_thread_enabled() || (_main_thread_id != eina_thread_self()))
     {
        orig_evgl_api_glAlphaFunc(func, ref);
        return;
     }

   int thread_mode = EVAS_GL_THREAD_MODE_FINISH;

   EVGL_API_Thread_Command_glAlphaFunc thread_data_local;
   EVGL_API_Thread_Command_glAlphaFunc *thread_data = &thread_data_local;

   thread_data->func = func;
   thread_data->ref = ref;

   evas_gl_thread_cmd_enqueue(EVAS_GL_THREAD_TYPE_EVGL,
                              _evgl_api_thread_glAlphaFunc,
                              thread_data,
                              thread_mode);
}

/*
 * void
 * glClipPlanef(GLenum plane, const GLfloat *equation);
 */

typedef struct
{
   GLenum plane;
   const GLfloat *equation;

} EVGL_API_Thread_Command_glClipPlanef;

void (*orig_evgl_api_glClipPlanef)(GLenum plane, const GLfloat *equation);

static void
_evgl_api_thread_glClipPlanef(void *data)
{
   EVGL_API_Thread_Command_glClipPlanef *thread_data =
      (EVGL_API_Thread_Command_glClipPlanef *)data;

   orig_evgl_api_glClipPlanef(thread_data->plane,
                              thread_data->equation);

}

EAPI void
glClipPlanef_evgl_api_thread_cmd(GLenum plane, const GLfloat *equation)
{
   if (!evas_evgl_thread_enabled() || (_main_thread_id != eina_thread_self()))
     {
        orig_evgl_api_glClipPlanef(plane, equation);
        return;
     }

   int thread_mode = EVAS_GL_THREAD_MODE_FINISH;

   EVGL_API_Thread_Command_glClipPlanef thread_data_local;
   EVGL_API_Thread_Command_glClipPlanef *thread_data = &thread_data_local;

   thread_data->plane = plane;
   thread_data->equation = equation;

   evas_gl_thread_cmd_enqueue(EVAS_GL_THREAD_TYPE_EVGL,
                              _evgl_api_thread_glClipPlanef,
                              thread_data,
                              thread_mode);
}

/*
 * void
 * glColor4f(GLfloat red, GLfloat green, GLfloat blue, GLfloat alpha);
 */

typedef struct
{
   GLfloat red;
   GLfloat green;
   GLfloat blue;
   GLfloat alpha;

} EVGL_API_Thread_Command_glColor4f;

void (*orig_evgl_api_glColor4f)(GLfloat red, GLfloat green, GLfloat blue, GLfloat alpha);

static void
_evgl_api_thread_glColor4f(void *data)
{
   EVGL_API_Thread_Command_glColor4f *thread_data =
      (EVGL_API_Thread_Command_glColor4f *)data;

   orig_evgl_api_glColor4f(thread_data->red,
                           thread_data->green,
                           thread_data->blue,
                           thread_data->alpha);

}

EAPI void
glColor4f_evgl_api_thread_cmd(GLfloat red, GLfloat green, GLfloat blue, GLfloat alpha)
{
   if (!evas_evgl_thread_enabled() || (_main_thread_id != eina_thread_self()))
     {
        orig_evgl_api_glColor4f(red, green, blue, alpha);
        return;
     }

   int thread_mode = EVAS_GL_THREAD_MODE_FINISH;

   EVGL_API_Thread_Command_glColor4f thread_data_local;
   EVGL_API_Thread_Command_glColor4f *thread_data = &thread_data_local;

   thread_data->red = red;
   thread_data->green = green;
   thread_data->blue = blue;
   thread_data->alpha = alpha;

   evas_gl_thread_cmd_enqueue(EVAS_GL_THREAD_TYPE_EVGL,
                              _evgl_api_thread_glColor4f,
                              thread_data,
                              thread_mode);
}

/*
 * void
 * glFogf(GLenum pname, GLfloat param);
 */

typedef struct
{
   GLenum pname;
   GLfloat param;

} EVGL_API_Thread_Command_glFogf;

void (*orig_evgl_api_glFogf)(GLenum pname, GLfloat param);

static void
_evgl_api_thread_glFogf(void *data)
{
   EVGL_API_Thread_Command_glFogf *thread_data =
      (EVGL_API_Thread_Command_glFogf *)data;

   orig_evgl_api_glFogf(thread_data->pname,
                        thread_data->param);

}

EAPI void
glFogf_evgl_api_thread_cmd(GLenum pname, GLfloat param)
{
   if (!evas_evgl_thread_enabled() || (_main_thread_id != eina_thread_self()))
     {
        orig_evgl_api_glFogf(pname, param);
        return;
     }

   int thread_mode = EVAS_GL_THREAD_MODE_FINISH;

   EVGL_API_Thread_Command_glFogf thread_data_local;
   EVGL_API_Thread_Command_glFogf *thread_data = &thread_data_local;

   thread_data->pname = pname;
   thread_data->param = param;

   evas_gl_thread_cmd_enqueue(EVAS_GL_THREAD_TYPE_EVGL,
                              _evgl_api_thread_glFogf,
                              thread_data,
                              thread_mode);
}

/*
 * void
 * glFogfv(GLenum pname, const GLfloat *params);
 */

typedef struct
{
   GLenum pname;
   const GLfloat *params;

} EVGL_API_Thread_Command_glFogfv;

void (*orig_evgl_api_glFogfv)(GLenum pname, const GLfloat *params);

static void
_evgl_api_thread_glFogfv(void *data)
{
   EVGL_API_Thread_Command_glFogfv *thread_data =
      (EVGL_API_Thread_Command_glFogfv *)data;

   orig_evgl_api_glFogfv(thread_data->pname,
                         thread_data->params);

}

EAPI void
glFogfv_evgl_api_thread_cmd(GLenum pname, const GLfloat *params)
{
   if (!evas_evgl_thread_enabled() || (_main_thread_id != eina_thread_self()))
     {
        orig_evgl_api_glFogfv(pname, params);
        return;
     }

   int thread_mode = EVAS_GL_THREAD_MODE_FINISH;

   EVGL_API_Thread_Command_glFogfv thread_data_local;
   EVGL_API_Thread_Command_glFogfv *thread_data = &thread_data_local;

   thread_data->pname = pname;
   thread_data->params = params;

   evas_gl_thread_cmd_enqueue(EVAS_GL_THREAD_TYPE_EVGL,
                              _evgl_api_thread_glFogfv,
                              thread_data,
                              thread_mode);
}

/*
 * void
 * glFrustumf(GLfloat left, GLfloat right, GLfloat bottom, GLfloat top, GLfloat zNear, GLfloat zFar);
 */

typedef struct
{
   GLfloat left;
   GLfloat right;
   GLfloat bottom;
   GLfloat top;
   GLfloat zNear;
   GLfloat zFar;

} EVGL_API_Thread_Command_glFrustumf;

void (*orig_evgl_api_glFrustumf)(GLfloat left, GLfloat right, GLfloat bottom, GLfloat top, GLfloat zNear, GLfloat zFar);

static void
_evgl_api_thread_glFrustumf(void *data)
{
   EVGL_API_Thread_Command_glFrustumf *thread_data =
      (EVGL_API_Thread_Command_glFrustumf *)data;

   orig_evgl_api_glFrustumf(thread_data->left,
                            thread_data->right,
                            thread_data->bottom,
                            thread_data->top,
                            thread_data->zNear,
                            thread_data->zFar);

}

EAPI void
glFrustumf_evgl_api_thread_cmd(GLfloat left, GLfloat right, GLfloat bottom, GLfloat top, GLfloat zNear, GLfloat zFar)
{
   if (!evas_evgl_thread_enabled() || (_main_thread_id != eina_thread_self()))
     {
        orig_evgl_api_glFrustumf(left, right, bottom, top, zNear, zFar);
        return;
     }

   int thread_mode = EVAS_GL_THREAD_MODE_FINISH;

   EVGL_API_Thread_Command_glFrustumf thread_data_local;
   EVGL_API_Thread_Command_glFrustumf *thread_data = &thread_data_local;

   thread_data->left = left;
   thread_data->right = right;
   thread_data->bottom = bottom;
   thread_data->top = top;
   thread_data->zNear = zNear;
   thread_data->zFar = zFar;

   evas_gl_thread_cmd_enqueue(EVAS_GL_THREAD_TYPE_EVGL,
                              _evgl_api_thread_glFrustumf,
                              thread_data,
                              thread_mode);
}

/*
 * void
 * glGetClipPlanef(GLenum pname, GLfloat eqn[4]);
 */

typedef struct
{
   GLenum pname;
   GLfloat eqn[4];

} EVGL_API_Thread_Command_glGetClipPlanef;

void (*orig_evgl_api_glGetClipPlanef)(GLenum pname, GLfloat eqn[4]);

static void
_evgl_api_thread_glGetClipPlanef(void *data)
{
   EVGL_API_Thread_Command_glGetClipPlanef *thread_data =
      (EVGL_API_Thread_Command_glGetClipPlanef *)data;

   orig_evgl_api_glGetClipPlanef(thread_data->pname,
                                 thread_data->eqn);

}

EAPI void
glGetClipPlanef_evgl_api_thread_cmd(GLenum pname, GLfloat eqn[4])
{
   if (!evas_evgl_thread_enabled() || (_main_thread_id != eina_thread_self()))
     {
        orig_evgl_api_glGetClipPlanef(pname, eqn);
        return;
     }

   int thread_mode = EVAS_GL_THREAD_MODE_FINISH;

   EVGL_API_Thread_Command_glGetClipPlanef thread_data_local;
   EVGL_API_Thread_Command_glGetClipPlanef *thread_data = &thread_data_local;

   thread_data->pname = pname;
   memcpy(thread_data->eqn, &eqn, sizeof(GLfloat) * 4);

   evas_gl_thread_cmd_enqueue(EVAS_GL_THREAD_TYPE_EVGL,
                              _evgl_api_thread_glGetClipPlanef,
                              thread_data,
                              thread_mode);
}

/*
 * void
 * glGetLightfv(GLenum light, GLenum pname, GLfloat *params);
 */

typedef struct
{
   GLenum light;
   GLenum pname;
   GLfloat *params;

} EVGL_API_Thread_Command_glGetLightfv;

void (*orig_evgl_api_glGetLightfv)(GLenum light, GLenum pname, GLfloat *params);

static void
_evgl_api_thread_glGetLightfv(void *data)
{
   EVGL_API_Thread_Command_glGetLightfv *thread_data =
      (EVGL_API_Thread_Command_glGetLightfv *)data;

   orig_evgl_api_glGetLightfv(thread_data->light,
                              thread_data->pname,
                              thread_data->params);

}

EAPI void
glGetLightfv_evgl_api_thread_cmd(GLenum light, GLenum pname, GLfloat *params)
{
   if (!evas_evgl_thread_enabled() || (_main_thread_id != eina_thread_self()))
     {
        orig_evgl_api_glGetLightfv(light, pname, params);
        return;
     }

   int thread_mode = EVAS_GL_THREAD_MODE_FINISH;

   EVGL_API_Thread_Command_glGetLightfv thread_data_local;
   EVGL_API_Thread_Command_glGetLightfv *thread_data = &thread_data_local;

   thread_data->light = light;
   thread_data->pname = pname;
   thread_data->params = params;

   evas_gl_thread_cmd_enqueue(EVAS_GL_THREAD_TYPE_EVGL,
                              _evgl_api_thread_glGetLightfv,
                              thread_data,
                              thread_mode);
}

/*
 * void
 * glGetMaterialfv(GLenum face, GLenum pname, GLfloat *params);
 */

typedef struct
{
   GLenum face;
   GLenum pname;
   GLfloat *params;

} EVGL_API_Thread_Command_glGetMaterialfv;

void (*orig_evgl_api_glGetMaterialfv)(GLenum face, GLenum pname, GLfloat *params);

static void
_evgl_api_thread_glGetMaterialfv(void *data)
{
   EVGL_API_Thread_Command_glGetMaterialfv *thread_data =
      (EVGL_API_Thread_Command_glGetMaterialfv *)data;

   orig_evgl_api_glGetMaterialfv(thread_data->face,
                                 thread_data->pname,
                                 thread_data->params);

}

EAPI void
glGetMaterialfv_evgl_api_thread_cmd(GLenum face, GLenum pname, GLfloat *params)
{
   if (!evas_evgl_thread_enabled() || (_main_thread_id != eina_thread_self()))
     {
        orig_evgl_api_glGetMaterialfv(face, pname, params);
        return;
     }

   int thread_mode = EVAS_GL_THREAD_MODE_FINISH;

   EVGL_API_Thread_Command_glGetMaterialfv thread_data_local;
   EVGL_API_Thread_Command_glGetMaterialfv *thread_data = &thread_data_local;

   thread_data->face = face;
   thread_data->pname = pname;
   thread_data->params = params;

   evas_gl_thread_cmd_enqueue(EVAS_GL_THREAD_TYPE_EVGL,
                              _evgl_api_thread_glGetMaterialfv,
                              thread_data,
                              thread_mode);
}

/*
 * void
 * glGetTexEnvfv(GLenum env, GLenum pname, GLfloat *params);
 */

typedef struct
{
   GLenum env;
   GLenum pname;
   GLfloat *params;

} EVGL_API_Thread_Command_glGetTexEnvfv;

void (*orig_evgl_api_glGetTexEnvfv)(GLenum env, GLenum pname, GLfloat *params);

static void
_evgl_api_thread_glGetTexEnvfv(void *data)
{
   EVGL_API_Thread_Command_glGetTexEnvfv *thread_data =
      (EVGL_API_Thread_Command_glGetTexEnvfv *)data;

   orig_evgl_api_glGetTexEnvfv(thread_data->env,
                               thread_data->pname,
                               thread_data->params);

}

EAPI void
glGetTexEnvfv_evgl_api_thread_cmd(GLenum env, GLenum pname, GLfloat *params)
{
   if (!evas_evgl_thread_enabled() || (_main_thread_id != eina_thread_self()))
     {
        orig_evgl_api_glGetTexEnvfv(env, pname, params);
        return;
     }

   int thread_mode = EVAS_GL_THREAD_MODE_FINISH;

   EVGL_API_Thread_Command_glGetTexEnvfv thread_data_local;
   EVGL_API_Thread_Command_glGetTexEnvfv *thread_data = &thread_data_local;

   thread_data->env = env;
   thread_data->pname = pname;
   thread_data->params = params;

   evas_gl_thread_cmd_enqueue(EVAS_GL_THREAD_TYPE_EVGL,
                              _evgl_api_thread_glGetTexEnvfv,
                              thread_data,
                              thread_mode);
}

/*
 * void
 * glLightModelf(GLenum pname, GLfloat param);
 */

typedef struct
{
   GLenum pname;
   GLfloat param;

} EVGL_API_Thread_Command_glLightModelf;

void (*orig_evgl_api_glLightModelf)(GLenum pname, GLfloat param);

static void
_evgl_api_thread_glLightModelf(void *data)
{
   EVGL_API_Thread_Command_glLightModelf *thread_data =
      (EVGL_API_Thread_Command_glLightModelf *)data;

   orig_evgl_api_glLightModelf(thread_data->pname,
                               thread_data->param);

}

EAPI void
glLightModelf_evgl_api_thread_cmd(GLenum pname, GLfloat param)
{
   if (!evas_evgl_thread_enabled() || (_main_thread_id != eina_thread_self()))
     {
        orig_evgl_api_glLightModelf(pname, param);
        return;
     }

   int thread_mode = EVAS_GL_THREAD_MODE_FINISH;

   EVGL_API_Thread_Command_glLightModelf thread_data_local;
   EVGL_API_Thread_Command_glLightModelf *thread_data = &thread_data_local;

   thread_data->pname = pname;
   thread_data->param = param;

   evas_gl_thread_cmd_enqueue(EVAS_GL_THREAD_TYPE_EVGL,
                              _evgl_api_thread_glLightModelf,
                              thread_data,
                              thread_mode);
}

/*
 * void
 * glLightModelfv(GLenum pname, const GLfloat *params);
 */

typedef struct
{
   GLenum pname;
   const GLfloat *params;

} EVGL_API_Thread_Command_glLightModelfv;

void (*orig_evgl_api_glLightModelfv)(GLenum pname, const GLfloat *params);

static void
_evgl_api_thread_glLightModelfv(void *data)
{
   EVGL_API_Thread_Command_glLightModelfv *thread_data =
      (EVGL_API_Thread_Command_glLightModelfv *)data;

   orig_evgl_api_glLightModelfv(thread_data->pname,
                                thread_data->params);

}

EAPI void
glLightModelfv_evgl_api_thread_cmd(GLenum pname, const GLfloat *params)
{
   if (!evas_evgl_thread_enabled() || (_main_thread_id != eina_thread_self()))
     {
        orig_evgl_api_glLightModelfv(pname, params);
        return;
     }

   int thread_mode = EVAS_GL_THREAD_MODE_FINISH;

   EVGL_API_Thread_Command_glLightModelfv thread_data_local;
   EVGL_API_Thread_Command_glLightModelfv *thread_data = &thread_data_local;

   thread_data->pname = pname;
   thread_data->params = params;

   evas_gl_thread_cmd_enqueue(EVAS_GL_THREAD_TYPE_EVGL,
                              _evgl_api_thread_glLightModelfv,
                              thread_data,
                              thread_mode);
}

/*
 * void
 * glLightf(GLenum light, GLenum pname, GLfloat param);
 */

typedef struct
{
   GLenum light;
   GLenum pname;
   GLfloat param;

} EVGL_API_Thread_Command_glLightf;

void (*orig_evgl_api_glLightf)(GLenum light, GLenum pname, GLfloat param);

static void
_evgl_api_thread_glLightf(void *data)
{
   EVGL_API_Thread_Command_glLightf *thread_data =
      (EVGL_API_Thread_Command_glLightf *)data;

   orig_evgl_api_glLightf(thread_data->light,
                          thread_data->pname,
                          thread_data->param);

}

EAPI void
glLightf_evgl_api_thread_cmd(GLenum light, GLenum pname, GLfloat param)
{
   if (!evas_evgl_thread_enabled() || (_main_thread_id != eina_thread_self()))
     {
        orig_evgl_api_glLightf(light, pname, param);
        return;
     }

   int thread_mode = EVAS_GL_THREAD_MODE_FINISH;

   EVGL_API_Thread_Command_glLightf thread_data_local;
   EVGL_API_Thread_Command_glLightf *thread_data = &thread_data_local;

   thread_data->light = light;
   thread_data->pname = pname;
   thread_data->param = param;

   evas_gl_thread_cmd_enqueue(EVAS_GL_THREAD_TYPE_EVGL,
                              _evgl_api_thread_glLightf,
                              thread_data,
                              thread_mode);
}

/*
 * void
 * glLightfv(GLenum light, GLenum pname, const GLfloat *params);
 */

typedef struct
{
   GLenum light;
   GLenum pname;
   const GLfloat *params;

} EVGL_API_Thread_Command_glLightfv;

void (*orig_evgl_api_glLightfv)(GLenum light, GLenum pname, const GLfloat *params);

static void
_evgl_api_thread_glLightfv(void *data)
{
   EVGL_API_Thread_Command_glLightfv *thread_data =
      (EVGL_API_Thread_Command_glLightfv *)data;

   orig_evgl_api_glLightfv(thread_data->light,
                           thread_data->pname,
                           thread_data->params);

}

EAPI void
glLightfv_evgl_api_thread_cmd(GLenum light, GLenum pname, const GLfloat *params)
{
   if (!evas_evgl_thread_enabled() || (_main_thread_id != eina_thread_self()))
     {
        orig_evgl_api_glLightfv(light, pname, params);
        return;
     }

   int thread_mode = EVAS_GL_THREAD_MODE_FINISH;

   EVGL_API_Thread_Command_glLightfv thread_data_local;
   EVGL_API_Thread_Command_glLightfv *thread_data = &thread_data_local;

   thread_data->light = light;
   thread_data->pname = pname;
   thread_data->params = params;

   evas_gl_thread_cmd_enqueue(EVAS_GL_THREAD_TYPE_EVGL,
                              _evgl_api_thread_glLightfv,
                              thread_data,
                              thread_mode);
}

/*
 * void
 * glLoadMatrixf(const GLfloat *m);
 */

typedef struct
{
   const GLfloat *m;

} EVGL_API_Thread_Command_glLoadMatrixf;

void (*orig_evgl_api_glLoadMatrixf)(const GLfloat *m);

static void
_evgl_api_thread_glLoadMatrixf(void *data)
{
   EVGL_API_Thread_Command_glLoadMatrixf *thread_data =
      (EVGL_API_Thread_Command_glLoadMatrixf *)data;

   orig_evgl_api_glLoadMatrixf(thread_data->m);

}

EAPI void
glLoadMatrixf_evgl_api_thread_cmd(const GLfloat *m)
{
   if (!evas_evgl_thread_enabled() || (_main_thread_id != eina_thread_self()))
     {
        orig_evgl_api_glLoadMatrixf(m);
        return;
     }

   int thread_mode = EVAS_GL_THREAD_MODE_FINISH;

   EVGL_API_Thread_Command_glLoadMatrixf thread_data_local;
   EVGL_API_Thread_Command_glLoadMatrixf *thread_data = &thread_data_local;

   thread_data->m = m;

   evas_gl_thread_cmd_enqueue(EVAS_GL_THREAD_TYPE_EVGL,
                              _evgl_api_thread_glLoadMatrixf,
                              thread_data,
                              thread_mode);
}

/*
 * void
 * glMaterialf(GLenum face, GLenum pname, GLfloat param);
 */

typedef struct
{
   GLenum face;
   GLenum pname;
   GLfloat param;

} EVGL_API_Thread_Command_glMaterialf;

void (*orig_evgl_api_glMaterialf)(GLenum face, GLenum pname, GLfloat param);

static void
_evgl_api_thread_glMaterialf(void *data)
{
   EVGL_API_Thread_Command_glMaterialf *thread_data =
      (EVGL_API_Thread_Command_glMaterialf *)data;

   orig_evgl_api_glMaterialf(thread_data->face,
                             thread_data->pname,
                             thread_data->param);

}

EAPI void
glMaterialf_evgl_api_thread_cmd(GLenum face, GLenum pname, GLfloat param)
{
   if (!evas_evgl_thread_enabled() || (_main_thread_id != eina_thread_self()))
     {
        orig_evgl_api_glMaterialf(face, pname, param);
        return;
     }

   int thread_mode = EVAS_GL_THREAD_MODE_FINISH;

   EVGL_API_Thread_Command_glMaterialf thread_data_local;
   EVGL_API_Thread_Command_glMaterialf *thread_data = &thread_data_local;

   thread_data->face = face;
   thread_data->pname = pname;
   thread_data->param = param;

   evas_gl_thread_cmd_enqueue(EVAS_GL_THREAD_TYPE_EVGL,
                              _evgl_api_thread_glMaterialf,
                              thread_data,
                              thread_mode);
}

/*
 * void
 * glMaterialfv(GLenum face, GLenum pname, const GLfloat *params);
 */

typedef struct
{
   GLenum face;
   GLenum pname;
   const GLfloat *params;

} EVGL_API_Thread_Command_glMaterialfv;

void (*orig_evgl_api_glMaterialfv)(GLenum face, GLenum pname, const GLfloat *params);

static void
_evgl_api_thread_glMaterialfv(void *data)
{
   EVGL_API_Thread_Command_glMaterialfv *thread_data =
      (EVGL_API_Thread_Command_glMaterialfv *)data;

   orig_evgl_api_glMaterialfv(thread_data->face,
                              thread_data->pname,
                              thread_data->params);

}

EAPI void
glMaterialfv_evgl_api_thread_cmd(GLenum face, GLenum pname, const GLfloat *params)
{
   if (!evas_evgl_thread_enabled() || (_main_thread_id != eina_thread_self()))
     {
        orig_evgl_api_glMaterialfv(face, pname, params);
        return;
     }

   int thread_mode = EVAS_GL_THREAD_MODE_FINISH;

   EVGL_API_Thread_Command_glMaterialfv thread_data_local;
   EVGL_API_Thread_Command_glMaterialfv *thread_data = &thread_data_local;

   thread_data->face = face;
   thread_data->pname = pname;
   thread_data->params = params;

   evas_gl_thread_cmd_enqueue(EVAS_GL_THREAD_TYPE_EVGL,
                              _evgl_api_thread_glMaterialfv,
                              thread_data,
                              thread_mode);
}

/*
 * void
 * glMultMatrixf(const GLfloat *m);
 */

typedef struct
{
   const GLfloat *m;

} EVGL_API_Thread_Command_glMultMatrixf;

void (*orig_evgl_api_glMultMatrixf)(const GLfloat *m);

static void
_evgl_api_thread_glMultMatrixf(void *data)
{
   EVGL_API_Thread_Command_glMultMatrixf *thread_data =
      (EVGL_API_Thread_Command_glMultMatrixf *)data;

   orig_evgl_api_glMultMatrixf(thread_data->m);

}

EAPI void
glMultMatrixf_evgl_api_thread_cmd(const GLfloat *m)
{
   if (!evas_evgl_thread_enabled() || (_main_thread_id != eina_thread_self()))
     {
        orig_evgl_api_glMultMatrixf(m);
        return;
     }

   int thread_mode = EVAS_GL_THREAD_MODE_FINISH;

   EVGL_API_Thread_Command_glMultMatrixf thread_data_local;
   EVGL_API_Thread_Command_glMultMatrixf *thread_data = &thread_data_local;

   thread_data->m = m;

   evas_gl_thread_cmd_enqueue(EVAS_GL_THREAD_TYPE_EVGL,
                              _evgl_api_thread_glMultMatrixf,
                              thread_data,
                              thread_mode);
}

/*
 * void
 * glMultiTexCoord4f(GLenum target, GLfloat s, GLfloat t, GLfloat r, GLfloat q);
 */

typedef struct
{
   GLenum target;
   GLfloat s;
   GLfloat t;
   GLfloat r;
   GLfloat q;

} EVGL_API_Thread_Command_glMultiTexCoord4f;

void (*orig_evgl_api_glMultiTexCoord4f)(GLenum target, GLfloat s, GLfloat t, GLfloat r, GLfloat q);

static void
_evgl_api_thread_glMultiTexCoord4f(void *data)
{
   EVGL_API_Thread_Command_glMultiTexCoord4f *thread_data =
      (EVGL_API_Thread_Command_glMultiTexCoord4f *)data;

   orig_evgl_api_glMultiTexCoord4f(thread_data->target,
                                   thread_data->s,
                                   thread_data->t,
                                   thread_data->r,
                                   thread_data->q);

}

EAPI void
glMultiTexCoord4f_evgl_api_thread_cmd(GLenum target, GLfloat s, GLfloat t, GLfloat r, GLfloat q)
{
   if (!evas_evgl_thread_enabled() || (_main_thread_id != eina_thread_self()))
     {
        orig_evgl_api_glMultiTexCoord4f(target, s, t, r, q);
        return;
     }

   int thread_mode = EVAS_GL_THREAD_MODE_FINISH;

   EVGL_API_Thread_Command_glMultiTexCoord4f thread_data_local;
   EVGL_API_Thread_Command_glMultiTexCoord4f *thread_data = &thread_data_local;

   thread_data->target = target;
   thread_data->s = s;
   thread_data->t = t;
   thread_data->r = r;
   thread_data->q = q;

   evas_gl_thread_cmd_enqueue(EVAS_GL_THREAD_TYPE_EVGL,
                              _evgl_api_thread_glMultiTexCoord4f,
                              thread_data,
                              thread_mode);
}

/*
 * void
 * glNormal3f(GLfloat nx, GLfloat ny, GLfloat nz);
 */

typedef struct
{
   GLfloat nx;
   GLfloat ny;
   GLfloat nz;

} EVGL_API_Thread_Command_glNormal3f;

void (*orig_evgl_api_glNormal3f)(GLfloat nx, GLfloat ny, GLfloat nz);

static void
_evgl_api_thread_glNormal3f(void *data)
{
   EVGL_API_Thread_Command_glNormal3f *thread_data =
      (EVGL_API_Thread_Command_glNormal3f *)data;

   orig_evgl_api_glNormal3f(thread_data->nx,
                            thread_data->ny,
                            thread_data->nz);

}

EAPI void
glNormal3f_evgl_api_thread_cmd(GLfloat nx, GLfloat ny, GLfloat nz)
{
   if (!evas_evgl_thread_enabled() || (_main_thread_id != eina_thread_self()))
     {
        orig_evgl_api_glNormal3f(nx, ny, nz);
        return;
     }

   int thread_mode = EVAS_GL_THREAD_MODE_FINISH;

   EVGL_API_Thread_Command_glNormal3f thread_data_local;
   EVGL_API_Thread_Command_glNormal3f *thread_data = &thread_data_local;

   thread_data->nx = nx;
   thread_data->ny = ny;
   thread_data->nz = nz;

   evas_gl_thread_cmd_enqueue(EVAS_GL_THREAD_TYPE_EVGL,
                              _evgl_api_thread_glNormal3f,
                              thread_data,
                              thread_mode);
}

/*
 * void
 * glOrthof(GLfloat left, GLfloat right, GLfloat bottom, GLfloat top, GLfloat zNear, GLfloat zFar);
 */

typedef struct
{
   GLfloat left;
   GLfloat right;
   GLfloat bottom;
   GLfloat top;
   GLfloat zNear;
   GLfloat zFar;

} EVGL_API_Thread_Command_glOrthof;

void (*orig_evgl_api_glOrthof)(GLfloat left, GLfloat right, GLfloat bottom, GLfloat top, GLfloat zNear, GLfloat zFar);

static void
_evgl_api_thread_glOrthof(void *data)
{
   EVGL_API_Thread_Command_glOrthof *thread_data =
      (EVGL_API_Thread_Command_glOrthof *)data;

   orig_evgl_api_glOrthof(thread_data->left,
                          thread_data->right,
                          thread_data->bottom,
                          thread_data->top,
                          thread_data->zNear,
                          thread_data->zFar);

}

EAPI void
glOrthof_evgl_api_thread_cmd(GLfloat left, GLfloat right, GLfloat bottom, GLfloat top, GLfloat zNear, GLfloat zFar)
{
   if (!evas_evgl_thread_enabled() || (_main_thread_id != eina_thread_self()))
     {
        orig_evgl_api_glOrthof(left, right, bottom, top, zNear, zFar);
        return;
     }

   int thread_mode = EVAS_GL_THREAD_MODE_FINISH;

   EVGL_API_Thread_Command_glOrthof thread_data_local;
   EVGL_API_Thread_Command_glOrthof *thread_data = &thread_data_local;

   thread_data->left = left;
   thread_data->right = right;
   thread_data->bottom = bottom;
   thread_data->top = top;
   thread_data->zNear = zNear;
   thread_data->zFar = zFar;

   evas_gl_thread_cmd_enqueue(EVAS_GL_THREAD_TYPE_EVGL,
                              _evgl_api_thread_glOrthof,
                              thread_data,
                              thread_mode);
}

/*
 * void
 * glPointParameterf(GLenum pname, GLfloat param);
 */

typedef struct
{
   GLenum pname;
   GLfloat param;

} EVGL_API_Thread_Command_glPointParameterf;

void (*orig_evgl_api_glPointParameterf)(GLenum pname, GLfloat param);

static void
_evgl_api_thread_glPointParameterf(void *data)
{
   EVGL_API_Thread_Command_glPointParameterf *thread_data =
      (EVGL_API_Thread_Command_glPointParameterf *)data;

   orig_evgl_api_glPointParameterf(thread_data->pname,
                                   thread_data->param);

}

EAPI void
glPointParameterf_evgl_api_thread_cmd(GLenum pname, GLfloat param)
{
   if (!evas_evgl_thread_enabled() || (_main_thread_id != eina_thread_self()))
     {
        orig_evgl_api_glPointParameterf(pname, param);
        return;
     }

   int thread_mode = EVAS_GL_THREAD_MODE_FINISH;

   EVGL_API_Thread_Command_glPointParameterf thread_data_local;
   EVGL_API_Thread_Command_glPointParameterf *thread_data = &thread_data_local;

   thread_data->pname = pname;
   thread_data->param = param;

   evas_gl_thread_cmd_enqueue(EVAS_GL_THREAD_TYPE_EVGL,
                              _evgl_api_thread_glPointParameterf,
                              thread_data,
                              thread_mode);
}

/*
 * void
 * glPointParameterfv(GLenum pname, const GLfloat *params);
 */

typedef struct
{
   GLenum pname;
   const GLfloat *params;

} EVGL_API_Thread_Command_glPointParameterfv;

void (*orig_evgl_api_glPointParameterfv)(GLenum pname, const GLfloat *params);

static void
_evgl_api_thread_glPointParameterfv(void *data)
{
   EVGL_API_Thread_Command_glPointParameterfv *thread_data =
      (EVGL_API_Thread_Command_glPointParameterfv *)data;

   orig_evgl_api_glPointParameterfv(thread_data->pname,
                                    thread_data->params);

}

EAPI void
glPointParameterfv_evgl_api_thread_cmd(GLenum pname, const GLfloat *params)
{
   if (!evas_evgl_thread_enabled() || (_main_thread_id != eina_thread_self()))
     {
        orig_evgl_api_glPointParameterfv(pname, params);
        return;
     }

   int thread_mode = EVAS_GL_THREAD_MODE_FINISH;

   EVGL_API_Thread_Command_glPointParameterfv thread_data_local;
   EVGL_API_Thread_Command_glPointParameterfv *thread_data = &thread_data_local;

   thread_data->pname = pname;
   thread_data->params = params;

   evas_gl_thread_cmd_enqueue(EVAS_GL_THREAD_TYPE_EVGL,
                              _evgl_api_thread_glPointParameterfv,
                              thread_data,
                              thread_mode);
}

/*
 * void
 * glPointSize(GLfloat size);
 */

typedef struct
{
   GLfloat size;

} EVGL_API_Thread_Command_glPointSize;

void (*orig_evgl_api_glPointSize)(GLfloat size);

static void
_evgl_api_thread_glPointSize(void *data)
{
   EVGL_API_Thread_Command_glPointSize *thread_data =
      (EVGL_API_Thread_Command_glPointSize *)data;

   orig_evgl_api_glPointSize(thread_data->size);

}

EAPI void
glPointSize_evgl_api_thread_cmd(GLfloat size)
{
   if (!evas_evgl_thread_enabled() || (_main_thread_id != eina_thread_self()))
     {
        orig_evgl_api_glPointSize(size);
        return;
     }

   int thread_mode = EVAS_GL_THREAD_MODE_FINISH;

   EVGL_API_Thread_Command_glPointSize thread_data_local;
   EVGL_API_Thread_Command_glPointSize *thread_data = &thread_data_local;

   thread_data->size = size;

   evas_gl_thread_cmd_enqueue(EVAS_GL_THREAD_TYPE_EVGL,
                              _evgl_api_thread_glPointSize,
                              thread_data,
                              thread_mode);
}

/*
 * void
 * glPointSizePointerOES(GLenum type, GLsizei stride, const GLvoid * pointer);
 */

typedef struct
{
   GLenum type;
   GLsizei stride;
   const GLvoid * pointer;

} EVGL_API_Thread_Command_glPointSizePointerOES;

void (*orig_evgl_api_glPointSizePointerOES)(GLenum type, GLsizei stride, const GLvoid * pointer);

static void
_evgl_api_thread_glPointSizePointerOES(void *data)
{
   EVGL_API_Thread_Command_glPointSizePointerOES *thread_data =
      (EVGL_API_Thread_Command_glPointSizePointerOES *)data;

   orig_evgl_api_glPointSizePointerOES(thread_data->type,
                                       thread_data->stride,
                                       thread_data->pointer);

}

EAPI void
glPointSizePointerOES_evgl_api_thread_cmd(GLenum type, GLsizei stride, const GLvoid * pointer)
{
   if (!evas_evgl_thread_enabled() || (_main_thread_id != eina_thread_self()))
     {
        orig_evgl_api_glPointSizePointerOES(type, stride, pointer);
        return;
     }

   int thread_mode = EVAS_GL_THREAD_MODE_FINISH;

   EVGL_API_Thread_Command_glPointSizePointerOES thread_data_local;
   EVGL_API_Thread_Command_glPointSizePointerOES *thread_data = &thread_data_local;

   thread_data->type = type;
   thread_data->stride = stride;
   thread_data->pointer = pointer;

   evas_gl_thread_cmd_enqueue(EVAS_GL_THREAD_TYPE_EVGL,
                              _evgl_api_thread_glPointSizePointerOES,
                              thread_data,
                              thread_mode);
}

/*
 * void
 * glRotatef(GLfloat angle, GLfloat x, GLfloat y, GLfloat z);
 */

typedef struct
{
   GLfloat angle;
   GLfloat x;
   GLfloat y;
   GLfloat z;

} EVGL_API_Thread_Command_glRotatef;

void (*orig_evgl_api_glRotatef)(GLfloat angle, GLfloat x, GLfloat y, GLfloat z);

static void
_evgl_api_thread_glRotatef(void *data)
{
   EVGL_API_Thread_Command_glRotatef *thread_data =
      (EVGL_API_Thread_Command_glRotatef *)data;

   orig_evgl_api_glRotatef(thread_data->angle,
                           thread_data->x,
                           thread_data->y,
                           thread_data->z);

}

EAPI void
glRotatef_evgl_api_thread_cmd(GLfloat angle, GLfloat x, GLfloat y, GLfloat z)
{
   if (!evas_evgl_thread_enabled() || (_main_thread_id != eina_thread_self()))
     {
        orig_evgl_api_glRotatef(angle, x, y, z);
        return;
     }

   int thread_mode = EVAS_GL_THREAD_MODE_FINISH;

   EVGL_API_Thread_Command_glRotatef thread_data_local;
   EVGL_API_Thread_Command_glRotatef *thread_data = &thread_data_local;

   thread_data->angle = angle;
   thread_data->x = x;
   thread_data->y = y;
   thread_data->z = z;

   evas_gl_thread_cmd_enqueue(EVAS_GL_THREAD_TYPE_EVGL,
                              _evgl_api_thread_glRotatef,
                              thread_data,
                              thread_mode);
}

/*
 * void
 * glScalef(GLfloat x, GLfloat y, GLfloat z);
 */

typedef struct
{
   GLfloat x;
   GLfloat y;
   GLfloat z;

} EVGL_API_Thread_Command_glScalef;

void (*orig_evgl_api_glScalef)(GLfloat x, GLfloat y, GLfloat z);

static void
_evgl_api_thread_glScalef(void *data)
{
   EVGL_API_Thread_Command_glScalef *thread_data =
      (EVGL_API_Thread_Command_glScalef *)data;

   orig_evgl_api_glScalef(thread_data->x,
                          thread_data->y,
                          thread_data->z);

}

EAPI void
glScalef_evgl_api_thread_cmd(GLfloat x, GLfloat y, GLfloat z)
{
   if (!evas_evgl_thread_enabled() || (_main_thread_id != eina_thread_self()))
     {
        orig_evgl_api_glScalef(x, y, z);
        return;
     }

   int thread_mode = EVAS_GL_THREAD_MODE_FINISH;

   EVGL_API_Thread_Command_glScalef thread_data_local;
   EVGL_API_Thread_Command_glScalef *thread_data = &thread_data_local;

   thread_data->x = x;
   thread_data->y = y;
   thread_data->z = z;

   evas_gl_thread_cmd_enqueue(EVAS_GL_THREAD_TYPE_EVGL,
                              _evgl_api_thread_glScalef,
                              thread_data,
                              thread_mode);
}

/*
 * void
 * glTexEnvf(GLenum target, GLenum pname, GLfloat param);
 */

typedef struct
{
   GLenum target;
   GLenum pname;
   GLfloat param;

} EVGL_API_Thread_Command_glTexEnvf;

void (*orig_evgl_api_glTexEnvf)(GLenum target, GLenum pname, GLfloat param);

static void
_evgl_api_thread_glTexEnvf(void *data)
{
   EVGL_API_Thread_Command_glTexEnvf *thread_data =
      (EVGL_API_Thread_Command_glTexEnvf *)data;

   orig_evgl_api_glTexEnvf(thread_data->target,
                           thread_data->pname,
                           thread_data->param);

}

EAPI void
glTexEnvf_evgl_api_thread_cmd(GLenum target, GLenum pname, GLfloat param)
{
   if (!evas_evgl_thread_enabled() || (_main_thread_id != eina_thread_self()))
     {
        orig_evgl_api_glTexEnvf(target, pname, param);
        return;
     }

   int thread_mode = EVAS_GL_THREAD_MODE_FINISH;

   EVGL_API_Thread_Command_glTexEnvf thread_data_local;
   EVGL_API_Thread_Command_glTexEnvf *thread_data = &thread_data_local;

   thread_data->target = target;
   thread_data->pname = pname;
   thread_data->param = param;

   evas_gl_thread_cmd_enqueue(EVAS_GL_THREAD_TYPE_EVGL,
                              _evgl_api_thread_glTexEnvf,
                              thread_data,
                              thread_mode);
}

/*
 * void
 * glTexEnvfv(GLenum target, GLenum pname, const GLfloat *params);
 */

typedef struct
{
   GLenum target;
   GLenum pname;
   const GLfloat *params;

} EVGL_API_Thread_Command_glTexEnvfv;

void (*orig_evgl_api_glTexEnvfv)(GLenum target, GLenum pname, const GLfloat *params);

static void
_evgl_api_thread_glTexEnvfv(void *data)
{
   EVGL_API_Thread_Command_glTexEnvfv *thread_data =
      (EVGL_API_Thread_Command_glTexEnvfv *)data;

   orig_evgl_api_glTexEnvfv(thread_data->target,
                            thread_data->pname,
                            thread_data->params);

}

EAPI void
glTexEnvfv_evgl_api_thread_cmd(GLenum target, GLenum pname, const GLfloat *params)
{
   if (!evas_evgl_thread_enabled() || (_main_thread_id != eina_thread_self()))
     {
        orig_evgl_api_glTexEnvfv(target, pname, params);
        return;
     }

   int thread_mode = EVAS_GL_THREAD_MODE_FINISH;

   EVGL_API_Thread_Command_glTexEnvfv thread_data_local;
   EVGL_API_Thread_Command_glTexEnvfv *thread_data = &thread_data_local;

   thread_data->target = target;
   thread_data->pname = pname;
   thread_data->params = params;

   evas_gl_thread_cmd_enqueue(EVAS_GL_THREAD_TYPE_EVGL,
                              _evgl_api_thread_glTexEnvfv,
                              thread_data,
                              thread_mode);
}

/*
 * void
 * glTranslatef(GLfloat x, GLfloat y, GLfloat z);
 */

typedef struct
{
   GLfloat x;
   GLfloat y;
   GLfloat z;

} EVGL_API_Thread_Command_glTranslatef;

void (*orig_evgl_api_glTranslatef)(GLfloat x, GLfloat y, GLfloat z);

static void
_evgl_api_thread_glTranslatef(void *data)
{
   EVGL_API_Thread_Command_glTranslatef *thread_data =
      (EVGL_API_Thread_Command_glTranslatef *)data;

   orig_evgl_api_glTranslatef(thread_data->x,
                              thread_data->y,
                              thread_data->z);

}

EAPI void
glTranslatef_evgl_api_thread_cmd(GLfloat x, GLfloat y, GLfloat z)
{
   if (!evas_evgl_thread_enabled() || (_main_thread_id != eina_thread_self()))
     {
        orig_evgl_api_glTranslatef(x, y, z);
        return;
     }

   int thread_mode = EVAS_GL_THREAD_MODE_FINISH;

   EVGL_API_Thread_Command_glTranslatef thread_data_local;
   EVGL_API_Thread_Command_glTranslatef *thread_data = &thread_data_local;

   thread_data->x = x;
   thread_data->y = y;
   thread_data->z = z;

   evas_gl_thread_cmd_enqueue(EVAS_GL_THREAD_TYPE_EVGL,
                              _evgl_api_thread_glTranslatef,
                              thread_data,
                              thread_mode);
}

/*
 * void
 * glAlphaFuncx(GLenum func, GLclampx ref);
 */

typedef struct
{
   GLenum func;
   GLclampx ref;

} EVGL_API_Thread_Command_glAlphaFuncx;

void (*orig_evgl_api_glAlphaFuncx)(GLenum func, GLclampx ref);

static void
_evgl_api_thread_glAlphaFuncx(void *data)
{
   EVGL_API_Thread_Command_glAlphaFuncx *thread_data =
      (EVGL_API_Thread_Command_glAlphaFuncx *)data;

   orig_evgl_api_glAlphaFuncx(thread_data->func,
                              thread_data->ref);

}

EAPI void
glAlphaFuncx_evgl_api_thread_cmd(GLenum func, GLclampx ref)
{
   if (!evas_evgl_thread_enabled() || (_main_thread_id != eina_thread_self()))
     {
        orig_evgl_api_glAlphaFuncx(func, ref);
        return;
     }

   int thread_mode = EVAS_GL_THREAD_MODE_FINISH;

   EVGL_API_Thread_Command_glAlphaFuncx thread_data_local;
   EVGL_API_Thread_Command_glAlphaFuncx *thread_data = &thread_data_local;

   thread_data->func = func;
   thread_data->ref = ref;

   evas_gl_thread_cmd_enqueue(EVAS_GL_THREAD_TYPE_EVGL,
                              _evgl_api_thread_glAlphaFuncx,
                              thread_data,
                              thread_mode);
}

/*
 * void
 * glClearColorx(GLclampx red, GLclampx green, GLclampx blue, GLclampx alpha);
 */

typedef struct
{
   GLclampx red;
   GLclampx green;
   GLclampx blue;
   GLclampx alpha;

} EVGL_API_Thread_Command_glClearColorx;

void (*orig_evgl_api_glClearColorx)(GLclampx red, GLclampx green, GLclampx blue, GLclampx alpha);

static void
_evgl_api_thread_glClearColorx(void *data)
{
   EVGL_API_Thread_Command_glClearColorx *thread_data =
      (EVGL_API_Thread_Command_glClearColorx *)data;

   orig_evgl_api_glClearColorx(thread_data->red,
                               thread_data->green,
                               thread_data->blue,
                               thread_data->alpha);

}

EAPI void
glClearColorx_evgl_api_thread_cmd(GLclampx red, GLclampx green, GLclampx blue, GLclampx alpha)
{
   if (!evas_evgl_thread_enabled() || (_main_thread_id != eina_thread_self()))
     {
        orig_evgl_api_glClearColorx(red, green, blue, alpha);
        return;
     }

   int thread_mode = EVAS_GL_THREAD_MODE_FINISH;

   EVGL_API_Thread_Command_glClearColorx thread_data_local;
   EVGL_API_Thread_Command_glClearColorx *thread_data = &thread_data_local;

   thread_data->red = red;
   thread_data->green = green;
   thread_data->blue = blue;
   thread_data->alpha = alpha;

   evas_gl_thread_cmd_enqueue(EVAS_GL_THREAD_TYPE_EVGL,
                              _evgl_api_thread_glClearColorx,
                              thread_data,
                              thread_mode);
}

/*
 * void
 * glClearDepthx(GLclampx depth);
 */

typedef struct
{
   GLclampx depth;

} EVGL_API_Thread_Command_glClearDepthx;

void (*orig_evgl_api_glClearDepthx)(GLclampx depth);

static void
_evgl_api_thread_glClearDepthx(void *data)
{
   EVGL_API_Thread_Command_glClearDepthx *thread_data =
      (EVGL_API_Thread_Command_glClearDepthx *)data;

   orig_evgl_api_glClearDepthx(thread_data->depth);

}

EAPI void
glClearDepthx_evgl_api_thread_cmd(GLclampx depth)
{
   if (!evas_evgl_thread_enabled() || (_main_thread_id != eina_thread_self()))
     {
        orig_evgl_api_glClearDepthx(depth);
        return;
     }

   int thread_mode = EVAS_GL_THREAD_MODE_FINISH;

   EVGL_API_Thread_Command_glClearDepthx thread_data_local;
   EVGL_API_Thread_Command_glClearDepthx *thread_data = &thread_data_local;

   thread_data->depth = depth;

   evas_gl_thread_cmd_enqueue(EVAS_GL_THREAD_TYPE_EVGL,
                              _evgl_api_thread_glClearDepthx,
                              thread_data,
                              thread_mode);
}

/*
 * void
 * glClientActiveTexture(GLenum texture);
 */

typedef struct
{
   GLenum texture;

} EVGL_API_Thread_Command_glClientActiveTexture;

void (*orig_evgl_api_glClientActiveTexture)(GLenum texture);

static void
_evgl_api_thread_glClientActiveTexture(void *data)
{
   EVGL_API_Thread_Command_glClientActiveTexture *thread_data =
      (EVGL_API_Thread_Command_glClientActiveTexture *)data;

   orig_evgl_api_glClientActiveTexture(thread_data->texture);

}

EAPI void
glClientActiveTexture_evgl_api_thread_cmd(GLenum texture)
{
   if (!evas_evgl_thread_enabled() || (_main_thread_id != eina_thread_self()))
     {
        orig_evgl_api_glClientActiveTexture(texture);
        return;
     }

   int thread_mode = EVAS_GL_THREAD_MODE_FINISH;

   EVGL_API_Thread_Command_glClientActiveTexture thread_data_local;
   EVGL_API_Thread_Command_glClientActiveTexture *thread_data = &thread_data_local;

   thread_data->texture = texture;

   evas_gl_thread_cmd_enqueue(EVAS_GL_THREAD_TYPE_EVGL,
                              _evgl_api_thread_glClientActiveTexture,
                              thread_data,
                              thread_mode);
}

/*
 * void
 * glClipPlanex(GLenum plane, const GLfixed *equation);
 */

typedef struct
{
   GLenum plane;
   const GLfixed *equation;

} EVGL_API_Thread_Command_glClipPlanex;

void (*orig_evgl_api_glClipPlanex)(GLenum plane, const GLfixed *equation);

static void
_evgl_api_thread_glClipPlanex(void *data)
{
   EVGL_API_Thread_Command_glClipPlanex *thread_data =
      (EVGL_API_Thread_Command_glClipPlanex *)data;

   orig_evgl_api_glClipPlanex(thread_data->plane,
                              thread_data->equation);

}

EAPI void
glClipPlanex_evgl_api_thread_cmd(GLenum plane, const GLfixed *equation)
{
   if (!evas_evgl_thread_enabled() || (_main_thread_id != eina_thread_self()))
     {
        orig_evgl_api_glClipPlanex(plane, equation);
        return;
     }

   int thread_mode = EVAS_GL_THREAD_MODE_FINISH;

   EVGL_API_Thread_Command_glClipPlanex thread_data_local;
   EVGL_API_Thread_Command_glClipPlanex *thread_data = &thread_data_local;

   thread_data->plane = plane;
   thread_data->equation = equation;

   evas_gl_thread_cmd_enqueue(EVAS_GL_THREAD_TYPE_EVGL,
                              _evgl_api_thread_glClipPlanex,
                              thread_data,
                              thread_mode);
}

/*
 * void
 * glColor4ub(GLubyte red, GLubyte green, GLubyte blue, GLubyte alpha);
 */

typedef struct
{
   GLubyte red;
   GLubyte green;
   GLubyte blue;
   GLubyte alpha;

} EVGL_API_Thread_Command_glColor4ub;

void (*orig_evgl_api_glColor4ub)(GLubyte red, GLubyte green, GLubyte blue, GLubyte alpha);

static void
_evgl_api_thread_glColor4ub(void *data)
{
   EVGL_API_Thread_Command_glColor4ub *thread_data =
      (EVGL_API_Thread_Command_glColor4ub *)data;

   orig_evgl_api_glColor4ub(thread_data->red,
                            thread_data->green,
                            thread_data->blue,
                            thread_data->alpha);

}

EAPI void
glColor4ub_evgl_api_thread_cmd(GLubyte red, GLubyte green, GLubyte blue, GLubyte alpha)
{
   if (!evas_evgl_thread_enabled() || (_main_thread_id != eina_thread_self()))
     {
        orig_evgl_api_glColor4ub(red, green, blue, alpha);
        return;
     }

   int thread_mode = EVAS_GL_THREAD_MODE_FINISH;

   EVGL_API_Thread_Command_glColor4ub thread_data_local;
   EVGL_API_Thread_Command_glColor4ub *thread_data = &thread_data_local;

   thread_data->red = red;
   thread_data->green = green;
   thread_data->blue = blue;
   thread_data->alpha = alpha;

   evas_gl_thread_cmd_enqueue(EVAS_GL_THREAD_TYPE_EVGL,
                              _evgl_api_thread_glColor4ub,
                              thread_data,
                              thread_mode);
}

/*
 * void
 * glColor4x(GLfixed red, GLfixed green, GLfixed blue, GLfixed alpha);
 */

typedef struct
{
   GLfixed red;
   GLfixed green;
   GLfixed blue;
   GLfixed alpha;

} EVGL_API_Thread_Command_glColor4x;

void (*orig_evgl_api_glColor4x)(GLfixed red, GLfixed green, GLfixed blue, GLfixed alpha);

static void
_evgl_api_thread_glColor4x(void *data)
{
   EVGL_API_Thread_Command_glColor4x *thread_data =
      (EVGL_API_Thread_Command_glColor4x *)data;

   orig_evgl_api_glColor4x(thread_data->red,
                           thread_data->green,
                           thread_data->blue,
                           thread_data->alpha);

}

EAPI void
glColor4x_evgl_api_thread_cmd(GLfixed red, GLfixed green, GLfixed blue, GLfixed alpha)
{
   if (!evas_evgl_thread_enabled() || (_main_thread_id != eina_thread_self()))
     {
        orig_evgl_api_glColor4x(red, green, blue, alpha);
        return;
     }

   int thread_mode = EVAS_GL_THREAD_MODE_FINISH;

   EVGL_API_Thread_Command_glColor4x thread_data_local;
   EVGL_API_Thread_Command_glColor4x *thread_data = &thread_data_local;

   thread_data->red = red;
   thread_data->green = green;
   thread_data->blue = blue;
   thread_data->alpha = alpha;

   evas_gl_thread_cmd_enqueue(EVAS_GL_THREAD_TYPE_EVGL,
                              _evgl_api_thread_glColor4x,
                              thread_data,
                              thread_mode);
}

/*
 * void
 * glColorPointer(GLint size, GLenum type, GLsizei stride, const GLvoid *pointer);
 */

typedef struct
{
   GLint size;
   GLenum type;
   GLsizei stride;
   const GLvoid *pointer;

} EVGL_API_Thread_Command_glColorPointer;

void (*orig_evgl_api_glColorPointer)(GLint size, GLenum type, GLsizei stride, const GLvoid *pointer);

static void
_evgl_api_thread_glColorPointer(void *data)
{
   EVGL_API_Thread_Command_glColorPointer *thread_data =
      (EVGL_API_Thread_Command_glColorPointer *)data;

   orig_evgl_api_glColorPointer(thread_data->size,
                                thread_data->type,
                                thread_data->stride,
                                thread_data->pointer);

}

EAPI void
glColorPointer_evgl_api_thread_cmd(GLint size, GLenum type, GLsizei stride, const GLvoid *pointer)
{
   if (!evas_evgl_thread_enabled() || (_main_thread_id != eina_thread_self()))
     {
        orig_evgl_api_glColorPointer(size, type, stride, pointer);
        return;
     }

   int thread_mode = EVAS_GL_THREAD_MODE_FINISH;

   EVGL_API_Thread_Command_glColorPointer thread_data_local;
   EVGL_API_Thread_Command_glColorPointer *thread_data = &thread_data_local;

   thread_data->size = size;
   thread_data->type = type;
   thread_data->stride = stride;
   thread_data->pointer = pointer;

   evas_gl_thread_cmd_enqueue(EVAS_GL_THREAD_TYPE_EVGL,
                              _evgl_api_thread_glColorPointer,
                              thread_data,
                              thread_mode);
}

/*
 * void
 * glDepthRangex(GLclampx zNear, GLclampx zFar);
 */

typedef struct
{
   GLclampx zNear;
   GLclampx zFar;

} EVGL_API_Thread_Command_glDepthRangex;

void (*orig_evgl_api_glDepthRangex)(GLclampx zNear, GLclampx zFar);

static void
_evgl_api_thread_glDepthRangex(void *data)
{
   EVGL_API_Thread_Command_glDepthRangex *thread_data =
      (EVGL_API_Thread_Command_glDepthRangex *)data;

   orig_evgl_api_glDepthRangex(thread_data->zNear,
                               thread_data->zFar);

}

EAPI void
glDepthRangex_evgl_api_thread_cmd(GLclampx zNear, GLclampx zFar)
{
   if (!evas_evgl_thread_enabled() || (_main_thread_id != eina_thread_self()))
     {
        orig_evgl_api_glDepthRangex(zNear, zFar);
        return;
     }

   int thread_mode = EVAS_GL_THREAD_MODE_FINISH;

   EVGL_API_Thread_Command_glDepthRangex thread_data_local;
   EVGL_API_Thread_Command_glDepthRangex *thread_data = &thread_data_local;

   thread_data->zNear = zNear;
   thread_data->zFar = zFar;

   evas_gl_thread_cmd_enqueue(EVAS_GL_THREAD_TYPE_EVGL,
                              _evgl_api_thread_glDepthRangex,
                              thread_data,
                              thread_mode);
}

/*
 * void
 * glDisableClientState(GLenum array);
 */

typedef struct
{
   GLenum array;

} EVGL_API_Thread_Command_glDisableClientState;

void (*orig_evgl_api_glDisableClientState)(GLenum array);

static void
_evgl_api_thread_glDisableClientState(void *data)
{
   EVGL_API_Thread_Command_glDisableClientState *thread_data =
      (EVGL_API_Thread_Command_glDisableClientState *)data;

   orig_evgl_api_glDisableClientState(thread_data->array);

}

EAPI void
glDisableClientState_evgl_api_thread_cmd(GLenum array)
{
   if (!evas_evgl_thread_enabled() || (_main_thread_id != eina_thread_self()))
     {
        orig_evgl_api_glDisableClientState(array);
        return;
     }

   int thread_mode = EVAS_GL_THREAD_MODE_FINISH;

   EVGL_API_Thread_Command_glDisableClientState thread_data_local;
   EVGL_API_Thread_Command_glDisableClientState *thread_data = &thread_data_local;

   thread_data->array = array;

   evas_gl_thread_cmd_enqueue(EVAS_GL_THREAD_TYPE_EVGL,
                              _evgl_api_thread_glDisableClientState,
                              thread_data,
                              thread_mode);
}

/*
 * void
 * glEnableClientState(GLenum array);
 */

typedef struct
{
   GLenum array;

} EVGL_API_Thread_Command_glEnableClientState;

void (*orig_evgl_api_glEnableClientState)(GLenum array);

static void
_evgl_api_thread_glEnableClientState(void *data)
{
   EVGL_API_Thread_Command_glEnableClientState *thread_data =
      (EVGL_API_Thread_Command_glEnableClientState *)data;

   orig_evgl_api_glEnableClientState(thread_data->array);

}

EAPI void
glEnableClientState_evgl_api_thread_cmd(GLenum array)
{
   if (!evas_evgl_thread_enabled() || (_main_thread_id != eina_thread_self()))
     {
        orig_evgl_api_glEnableClientState(array);
        return;
     }

   int thread_mode = EVAS_GL_THREAD_MODE_FINISH;

   EVGL_API_Thread_Command_glEnableClientState thread_data_local;
   EVGL_API_Thread_Command_glEnableClientState *thread_data = &thread_data_local;

   thread_data->array = array;

   evas_gl_thread_cmd_enqueue(EVAS_GL_THREAD_TYPE_EVGL,
                              _evgl_api_thread_glEnableClientState,
                              thread_data,
                              thread_mode);
}

/*
 * void
 * glFogx(GLenum pname, GLfixed param);
 */

typedef struct
{
   GLenum pname;
   GLfixed param;

} EVGL_API_Thread_Command_glFogx;

void (*orig_evgl_api_glFogx)(GLenum pname, GLfixed param);

static void
_evgl_api_thread_glFogx(void *data)
{
   EVGL_API_Thread_Command_glFogx *thread_data =
      (EVGL_API_Thread_Command_glFogx *)data;

   orig_evgl_api_glFogx(thread_data->pname,
                        thread_data->param);

}

EAPI void
glFogx_evgl_api_thread_cmd(GLenum pname, GLfixed param)
{
   if (!evas_evgl_thread_enabled() || (_main_thread_id != eina_thread_self()))
     {
        orig_evgl_api_glFogx(pname, param);
        return;
     }

   int thread_mode = EVAS_GL_THREAD_MODE_FINISH;

   EVGL_API_Thread_Command_glFogx thread_data_local;
   EVGL_API_Thread_Command_glFogx *thread_data = &thread_data_local;

   thread_data->pname = pname;
   thread_data->param = param;

   evas_gl_thread_cmd_enqueue(EVAS_GL_THREAD_TYPE_EVGL,
                              _evgl_api_thread_glFogx,
                              thread_data,
                              thread_mode);
}

/*
 * void
 * glFogxv(GLenum pname, const GLfixed *params);
 */

typedef struct
{
   GLenum pname;
   const GLfixed *params;

} EVGL_API_Thread_Command_glFogxv;

void (*orig_evgl_api_glFogxv)(GLenum pname, const GLfixed *params);

static void
_evgl_api_thread_glFogxv(void *data)
{
   EVGL_API_Thread_Command_glFogxv *thread_data =
      (EVGL_API_Thread_Command_glFogxv *)data;

   orig_evgl_api_glFogxv(thread_data->pname,
                         thread_data->params);

}

EAPI void
glFogxv_evgl_api_thread_cmd(GLenum pname, const GLfixed *params)
{
   if (!evas_evgl_thread_enabled() || (_main_thread_id != eina_thread_self()))
     {
        orig_evgl_api_glFogxv(pname, params);
        return;
     }

   int thread_mode = EVAS_GL_THREAD_MODE_FINISH;

   EVGL_API_Thread_Command_glFogxv thread_data_local;
   EVGL_API_Thread_Command_glFogxv *thread_data = &thread_data_local;

   thread_data->pname = pname;
   thread_data->params = params;

   evas_gl_thread_cmd_enqueue(EVAS_GL_THREAD_TYPE_EVGL,
                              _evgl_api_thread_glFogxv,
                              thread_data,
                              thread_mode);
}

/*
 * void
 * glFrustumx(GLfixed left, GLfixed right, GLfixed bottom, GLfixed top, GLfixed zNear, GLfixed zFar);
 */

typedef struct
{
   GLfixed left;
   GLfixed right;
   GLfixed bottom;
   GLfixed top;
   GLfixed zNear;
   GLfixed zFar;

} EVGL_API_Thread_Command_glFrustumx;

void (*orig_evgl_api_glFrustumx)(GLfixed left, GLfixed right, GLfixed bottom, GLfixed top, GLfixed zNear, GLfixed zFar);

static void
_evgl_api_thread_glFrustumx(void *data)
{
   EVGL_API_Thread_Command_glFrustumx *thread_data =
      (EVGL_API_Thread_Command_glFrustumx *)data;

   orig_evgl_api_glFrustumx(thread_data->left,
                            thread_data->right,
                            thread_data->bottom,
                            thread_data->top,
                            thread_data->zNear,
                            thread_data->zFar);

}

EAPI void
glFrustumx_evgl_api_thread_cmd(GLfixed left, GLfixed right, GLfixed bottom, GLfixed top, GLfixed zNear, GLfixed zFar)
{
   if (!evas_evgl_thread_enabled() || (_main_thread_id != eina_thread_self()))
     {
        orig_evgl_api_glFrustumx(left, right, bottom, top, zNear, zFar);
        return;
     }

   int thread_mode = EVAS_GL_THREAD_MODE_FINISH;

   EVGL_API_Thread_Command_glFrustumx thread_data_local;
   EVGL_API_Thread_Command_glFrustumx *thread_data = &thread_data_local;

   thread_data->left = left;
   thread_data->right = right;
   thread_data->bottom = bottom;
   thread_data->top = top;
   thread_data->zNear = zNear;
   thread_data->zFar = zFar;

   evas_gl_thread_cmd_enqueue(EVAS_GL_THREAD_TYPE_EVGL,
                              _evgl_api_thread_glFrustumx,
                              thread_data,
                              thread_mode);
}

/*
 * void
 * glGetClipPlanex(GLenum pname, GLfixed eqn[4]);
 */

typedef struct
{
   GLenum pname;
   GLfixed eqn[4];

} EVGL_API_Thread_Command_glGetClipPlanex;

void (*orig_evgl_api_glGetClipPlanex)(GLenum pname, GLfixed eqn[4]);

static void
_evgl_api_thread_glGetClipPlanex(void *data)
{
   EVGL_API_Thread_Command_glGetClipPlanex *thread_data =
      (EVGL_API_Thread_Command_glGetClipPlanex *)data;

   orig_evgl_api_glGetClipPlanex(thread_data->pname,
                                 thread_data->eqn);

}

EAPI void
glGetClipPlanex_evgl_api_thread_cmd(GLenum pname, GLfixed eqn[4])
{
   if (!evas_evgl_thread_enabled() || (_main_thread_id != eina_thread_self()))
     {
        orig_evgl_api_glGetClipPlanex(pname, eqn);
        return;
     }

   int thread_mode = EVAS_GL_THREAD_MODE_FINISH;

   EVGL_API_Thread_Command_glGetClipPlanex thread_data_local;
   EVGL_API_Thread_Command_glGetClipPlanex *thread_data = &thread_data_local;

   thread_data->pname = pname;
   memcpy(thread_data->eqn, &eqn, sizeof(GLfixed) * 4);

   evas_gl_thread_cmd_enqueue(EVAS_GL_THREAD_TYPE_EVGL,
                              _evgl_api_thread_glGetClipPlanex,
                              thread_data,
                              thread_mode);
}

/*
 * void
 * glGetFixedv(GLenum pname, GLfixed *params);
 */

typedef struct
{
   GLenum pname;
   GLfixed *params;

} EVGL_API_Thread_Command_glGetFixedv;

void (*orig_evgl_api_glGetFixedv)(GLenum pname, GLfixed *params);

static void
_evgl_api_thread_glGetFixedv(void *data)
{
   EVGL_API_Thread_Command_glGetFixedv *thread_data =
      (EVGL_API_Thread_Command_glGetFixedv *)data;

   orig_evgl_api_glGetFixedv(thread_data->pname,
                             thread_data->params);

}

EAPI void
glGetFixedv_evgl_api_thread_cmd(GLenum pname, GLfixed *params)
{
   if (!evas_evgl_thread_enabled() || (_main_thread_id != eina_thread_self()))
     {
        orig_evgl_api_glGetFixedv(pname, params);
        return;
     }

   int thread_mode = EVAS_GL_THREAD_MODE_FINISH;

   EVGL_API_Thread_Command_glGetFixedv thread_data_local;
   EVGL_API_Thread_Command_glGetFixedv *thread_data = &thread_data_local;

   thread_data->pname = pname;
   thread_data->params = params;

   evas_gl_thread_cmd_enqueue(EVAS_GL_THREAD_TYPE_EVGL,
                              _evgl_api_thread_glGetFixedv,
                              thread_data,
                              thread_mode);
}

/*
 * void
 * glGetLightxv(GLenum light, GLenum pname, GLfixed *params);
 */

typedef struct
{
   GLenum light;
   GLenum pname;
   GLfixed *params;

} EVGL_API_Thread_Command_glGetLightxv;

void (*orig_evgl_api_glGetLightxv)(GLenum light, GLenum pname, GLfixed *params);

static void
_evgl_api_thread_glGetLightxv(void *data)
{
   EVGL_API_Thread_Command_glGetLightxv *thread_data =
      (EVGL_API_Thread_Command_glGetLightxv *)data;

   orig_evgl_api_glGetLightxv(thread_data->light,
                              thread_data->pname,
                              thread_data->params);

}

EAPI void
glGetLightxv_evgl_api_thread_cmd(GLenum light, GLenum pname, GLfixed *params)
{
   if (!evas_evgl_thread_enabled() || (_main_thread_id != eina_thread_self()))
     {
        orig_evgl_api_glGetLightxv(light, pname, params);
        return;
     }

   int thread_mode = EVAS_GL_THREAD_MODE_FINISH;

   EVGL_API_Thread_Command_glGetLightxv thread_data_local;
   EVGL_API_Thread_Command_glGetLightxv *thread_data = &thread_data_local;

   thread_data->light = light;
   thread_data->pname = pname;
   thread_data->params = params;

   evas_gl_thread_cmd_enqueue(EVAS_GL_THREAD_TYPE_EVGL,
                              _evgl_api_thread_glGetLightxv,
                              thread_data,
                              thread_mode);
}

/*
 * void
 * glGetMaterialxv(GLenum face, GLenum pname, GLfixed *params);
 */

typedef struct
{
   GLenum face;
   GLenum pname;
   GLfixed *params;

} EVGL_API_Thread_Command_glGetMaterialxv;

void (*orig_evgl_api_glGetMaterialxv)(GLenum face, GLenum pname, GLfixed *params);

static void
_evgl_api_thread_glGetMaterialxv(void *data)
{
   EVGL_API_Thread_Command_glGetMaterialxv *thread_data =
      (EVGL_API_Thread_Command_glGetMaterialxv *)data;

   orig_evgl_api_glGetMaterialxv(thread_data->face,
                                 thread_data->pname,
                                 thread_data->params);

}

EAPI void
glGetMaterialxv_evgl_api_thread_cmd(GLenum face, GLenum pname, GLfixed *params)
{
   if (!evas_evgl_thread_enabled() || (_main_thread_id != eina_thread_self()))
     {
        orig_evgl_api_glGetMaterialxv(face, pname, params);
        return;
     }

   int thread_mode = EVAS_GL_THREAD_MODE_FINISH;

   EVGL_API_Thread_Command_glGetMaterialxv thread_data_local;
   EVGL_API_Thread_Command_glGetMaterialxv *thread_data = &thread_data_local;

   thread_data->face = face;
   thread_data->pname = pname;
   thread_data->params = params;

   evas_gl_thread_cmd_enqueue(EVAS_GL_THREAD_TYPE_EVGL,
                              _evgl_api_thread_glGetMaterialxv,
                              thread_data,
                              thread_mode);
}

/*
 * void
 * glGetPointerv(GLenum pname, GLvoid **params);
 */

typedef struct
{
   GLenum pname;
   GLvoid **params;

} EVGL_API_Thread_Command_glGetPointerv;

void (*orig_evgl_api_glGetPointerv)(GLenum pname, GLvoid **params);

static void
_evgl_api_thread_glGetPointerv(void *data)
{
   EVGL_API_Thread_Command_glGetPointerv *thread_data =
      (EVGL_API_Thread_Command_glGetPointerv *)data;

   orig_evgl_api_glGetPointerv(thread_data->pname,
                               thread_data->params);

}

EAPI void
glGetPointerv_evgl_api_thread_cmd(GLenum pname, GLvoid **params)
{
   if (!evas_evgl_thread_enabled() || (_main_thread_id != eina_thread_self()))
     {
        orig_evgl_api_glGetPointerv(pname, params);
        return;
     }

   int thread_mode = EVAS_GL_THREAD_MODE_FINISH;

   EVGL_API_Thread_Command_glGetPointerv thread_data_local;
   EVGL_API_Thread_Command_glGetPointerv *thread_data = &thread_data_local;

   thread_data->pname = pname;
   thread_data->params = params;

   evas_gl_thread_cmd_enqueue(EVAS_GL_THREAD_TYPE_EVGL,
                              _evgl_api_thread_glGetPointerv,
                              thread_data,
                              thread_mode);
}

/*
 * void
 * glGetTexEnviv(GLenum env, GLenum pname, GLint *params);
 */

typedef struct
{
   GLenum env;
   GLenum pname;
   GLint *params;

} EVGL_API_Thread_Command_glGetTexEnviv;

void (*orig_evgl_api_glGetTexEnviv)(GLenum env, GLenum pname, GLint *params);

static void
_evgl_api_thread_glGetTexEnviv(void *data)
{
   EVGL_API_Thread_Command_glGetTexEnviv *thread_data =
      (EVGL_API_Thread_Command_glGetTexEnviv *)data;

   orig_evgl_api_glGetTexEnviv(thread_data->env,
                               thread_data->pname,
                               thread_data->params);

}

EAPI void
glGetTexEnviv_evgl_api_thread_cmd(GLenum env, GLenum pname, GLint *params)
{
   if (!evas_evgl_thread_enabled() || (_main_thread_id != eina_thread_self()))
     {
        orig_evgl_api_glGetTexEnviv(env, pname, params);
        return;
     }

   int thread_mode = EVAS_GL_THREAD_MODE_FINISH;

   EVGL_API_Thread_Command_glGetTexEnviv thread_data_local;
   EVGL_API_Thread_Command_glGetTexEnviv *thread_data = &thread_data_local;

   thread_data->env = env;
   thread_data->pname = pname;
   thread_data->params = params;

   evas_gl_thread_cmd_enqueue(EVAS_GL_THREAD_TYPE_EVGL,
                              _evgl_api_thread_glGetTexEnviv,
                              thread_data,
                              thread_mode);
}

/*
 * void
 * glGetTexEnvxv(GLenum env, GLenum pname, GLfixed *params);
 */

typedef struct
{
   GLenum env;
   GLenum pname;
   GLfixed *params;

} EVGL_API_Thread_Command_glGetTexEnvxv;

void (*orig_evgl_api_glGetTexEnvxv)(GLenum env, GLenum pname, GLfixed *params);

static void
_evgl_api_thread_glGetTexEnvxv(void *data)
{
   EVGL_API_Thread_Command_glGetTexEnvxv *thread_data =
      (EVGL_API_Thread_Command_glGetTexEnvxv *)data;

   orig_evgl_api_glGetTexEnvxv(thread_data->env,
                               thread_data->pname,
                               thread_data->params);

}

EAPI void
glGetTexEnvxv_evgl_api_thread_cmd(GLenum env, GLenum pname, GLfixed *params)
{
   if (!evas_evgl_thread_enabled() || (_main_thread_id != eina_thread_self()))
     {
        orig_evgl_api_glGetTexEnvxv(env, pname, params);
        return;
     }

   int thread_mode = EVAS_GL_THREAD_MODE_FINISH;

   EVGL_API_Thread_Command_glGetTexEnvxv thread_data_local;
   EVGL_API_Thread_Command_glGetTexEnvxv *thread_data = &thread_data_local;

   thread_data->env = env;
   thread_data->pname = pname;
   thread_data->params = params;

   evas_gl_thread_cmd_enqueue(EVAS_GL_THREAD_TYPE_EVGL,
                              _evgl_api_thread_glGetTexEnvxv,
                              thread_data,
                              thread_mode);
}

/*
 * void
 * glGetTexParameterxv(GLenum target, GLenum pname, GLfixed *params);
 */

typedef struct
{
   GLenum target;
   GLenum pname;
   GLfixed *params;

} EVGL_API_Thread_Command_glGetTexParameterxv;

void (*orig_evgl_api_glGetTexParameterxv)(GLenum target, GLenum pname, GLfixed *params);

static void
_evgl_api_thread_glGetTexParameterxv(void *data)
{
   EVGL_API_Thread_Command_glGetTexParameterxv *thread_data =
      (EVGL_API_Thread_Command_glGetTexParameterxv *)data;

   orig_evgl_api_glGetTexParameterxv(thread_data->target,
                                     thread_data->pname,
                                     thread_data->params);

}

EAPI void
glGetTexParameterxv_evgl_api_thread_cmd(GLenum target, GLenum pname, GLfixed *params)
{
   if (!evas_evgl_thread_enabled() || (_main_thread_id != eina_thread_self()))
     {
        orig_evgl_api_glGetTexParameterxv(target, pname, params);
        return;
     }

   int thread_mode = EVAS_GL_THREAD_MODE_FINISH;

   EVGL_API_Thread_Command_glGetTexParameterxv thread_data_local;
   EVGL_API_Thread_Command_glGetTexParameterxv *thread_data = &thread_data_local;

   thread_data->target = target;
   thread_data->pname = pname;
   thread_data->params = params;

   evas_gl_thread_cmd_enqueue(EVAS_GL_THREAD_TYPE_EVGL,
                              _evgl_api_thread_glGetTexParameterxv,
                              thread_data,
                              thread_mode);
}

/*
 * void
 * glLightModelx(GLenum pname, GLfixed param);
 */

typedef struct
{
   GLenum pname;
   GLfixed param;

} EVGL_API_Thread_Command_glLightModelx;

void (*orig_evgl_api_glLightModelx)(GLenum pname, GLfixed param);

static void
_evgl_api_thread_glLightModelx(void *data)
{
   EVGL_API_Thread_Command_glLightModelx *thread_data =
      (EVGL_API_Thread_Command_glLightModelx *)data;

   orig_evgl_api_glLightModelx(thread_data->pname,
                               thread_data->param);

}

EAPI void
glLightModelx_evgl_api_thread_cmd(GLenum pname, GLfixed param)
{
   if (!evas_evgl_thread_enabled() || (_main_thread_id != eina_thread_self()))
     {
        orig_evgl_api_glLightModelx(pname, param);
        return;
     }

   int thread_mode = EVAS_GL_THREAD_MODE_FINISH;

   EVGL_API_Thread_Command_glLightModelx thread_data_local;
   EVGL_API_Thread_Command_glLightModelx *thread_data = &thread_data_local;

   thread_data->pname = pname;
   thread_data->param = param;

   evas_gl_thread_cmd_enqueue(EVAS_GL_THREAD_TYPE_EVGL,
                              _evgl_api_thread_glLightModelx,
                              thread_data,
                              thread_mode);
}

/*
 * void
 * glLightModelxv(GLenum pname, const GLfixed *params);
 */

typedef struct
{
   GLenum pname;
   const GLfixed *params;

} EVGL_API_Thread_Command_glLightModelxv;

void (*orig_evgl_api_glLightModelxv)(GLenum pname, const GLfixed *params);

static void
_evgl_api_thread_glLightModelxv(void *data)
{
   EVGL_API_Thread_Command_glLightModelxv *thread_data =
      (EVGL_API_Thread_Command_glLightModelxv *)data;

   orig_evgl_api_glLightModelxv(thread_data->pname,
                                thread_data->params);

}

EAPI void
glLightModelxv_evgl_api_thread_cmd(GLenum pname, const GLfixed *params)
{
   if (!evas_evgl_thread_enabled() || (_main_thread_id != eina_thread_self()))
     {
        orig_evgl_api_glLightModelxv(pname, params);
        return;
     }

   int thread_mode = EVAS_GL_THREAD_MODE_FINISH;

   EVGL_API_Thread_Command_glLightModelxv thread_data_local;
   EVGL_API_Thread_Command_glLightModelxv *thread_data = &thread_data_local;

   thread_data->pname = pname;
   thread_data->params = params;

   evas_gl_thread_cmd_enqueue(EVAS_GL_THREAD_TYPE_EVGL,
                              _evgl_api_thread_glLightModelxv,
                              thread_data,
                              thread_mode);
}

/*
 * void
 * glLightx(GLenum light, GLenum pname, GLfixed param);
 */

typedef struct
{
   GLenum light;
   GLenum pname;
   GLfixed param;

} EVGL_API_Thread_Command_glLightx;

void (*orig_evgl_api_glLightx)(GLenum light, GLenum pname, GLfixed param);

static void
_evgl_api_thread_glLightx(void *data)
{
   EVGL_API_Thread_Command_glLightx *thread_data =
      (EVGL_API_Thread_Command_glLightx *)data;

   orig_evgl_api_glLightx(thread_data->light,
                          thread_data->pname,
                          thread_data->param);

}

EAPI void
glLightx_evgl_api_thread_cmd(GLenum light, GLenum pname, GLfixed param)
{
   if (!evas_evgl_thread_enabled() || (_main_thread_id != eina_thread_self()))
     {
        orig_evgl_api_glLightx(light, pname, param);
        return;
     }

   int thread_mode = EVAS_GL_THREAD_MODE_FINISH;

   EVGL_API_Thread_Command_glLightx thread_data_local;
   EVGL_API_Thread_Command_glLightx *thread_data = &thread_data_local;

   thread_data->light = light;
   thread_data->pname = pname;
   thread_data->param = param;

   evas_gl_thread_cmd_enqueue(EVAS_GL_THREAD_TYPE_EVGL,
                              _evgl_api_thread_glLightx,
                              thread_data,
                              thread_mode);
}

/*
 * void
 * glLightxv(GLenum light, GLenum pname, const GLfixed *params);
 */

typedef struct
{
   GLenum light;
   GLenum pname;
   const GLfixed *params;

} EVGL_API_Thread_Command_glLightxv;

void (*orig_evgl_api_glLightxv)(GLenum light, GLenum pname, const GLfixed *params);

static void
_evgl_api_thread_glLightxv(void *data)
{
   EVGL_API_Thread_Command_glLightxv *thread_data =
      (EVGL_API_Thread_Command_glLightxv *)data;

   orig_evgl_api_glLightxv(thread_data->light,
                           thread_data->pname,
                           thread_data->params);

}

EAPI void
glLightxv_evgl_api_thread_cmd(GLenum light, GLenum pname, const GLfixed *params)
{
   if (!evas_evgl_thread_enabled() || (_main_thread_id != eina_thread_self()))
     {
        orig_evgl_api_glLightxv(light, pname, params);
        return;
     }

   int thread_mode = EVAS_GL_THREAD_MODE_FINISH;

   EVGL_API_Thread_Command_glLightxv thread_data_local;
   EVGL_API_Thread_Command_glLightxv *thread_data = &thread_data_local;

   thread_data->light = light;
   thread_data->pname = pname;
   thread_data->params = params;

   evas_gl_thread_cmd_enqueue(EVAS_GL_THREAD_TYPE_EVGL,
                              _evgl_api_thread_glLightxv,
                              thread_data,
                              thread_mode);
}

/*
 * void
 * glLineWidthx(GLfixed width);
 */

typedef struct
{
   GLfixed width;

} EVGL_API_Thread_Command_glLineWidthx;

void (*orig_evgl_api_glLineWidthx)(GLfixed width);

static void
_evgl_api_thread_glLineWidthx(void *data)
{
   EVGL_API_Thread_Command_glLineWidthx *thread_data =
      (EVGL_API_Thread_Command_glLineWidthx *)data;

   orig_evgl_api_glLineWidthx(thread_data->width);

}

EAPI void
glLineWidthx_evgl_api_thread_cmd(GLfixed width)
{
   if (!evas_evgl_thread_enabled() || (_main_thread_id != eina_thread_self()))
     {
        orig_evgl_api_glLineWidthx(width);
        return;
     }

   int thread_mode = EVAS_GL_THREAD_MODE_FINISH;

   EVGL_API_Thread_Command_glLineWidthx thread_data_local;
   EVGL_API_Thread_Command_glLineWidthx *thread_data = &thread_data_local;

   thread_data->width = width;

   evas_gl_thread_cmd_enqueue(EVAS_GL_THREAD_TYPE_EVGL,
                              _evgl_api_thread_glLineWidthx,
                              thread_data,
                              thread_mode);
}

/*
 * void
 * glLoadIdentity(void);
 */

void (*orig_evgl_api_glLoadIdentity)(void);

static void
_evgl_api_thread_glLoadIdentity(void *data EINA_UNUSED)
{
   orig_evgl_api_glLoadIdentity();

}

EAPI void
glLoadIdentity_evgl_api_thread_cmd(void)
{
   if (!evas_evgl_thread_enabled() || (_main_thread_id != eina_thread_self()))
     {
        orig_evgl_api_glLoadIdentity();
        return;
     }

   int thread_mode = EVAS_GL_THREAD_MODE_FINISH;

   evas_gl_thread_cmd_enqueue(EVAS_GL_THREAD_TYPE_EVGL,
                              _evgl_api_thread_glLoadIdentity,
                              NULL,
                              thread_mode);
}

/*
 * void
 * glLoadMatrixx(const GLfixed *m);
 */

typedef struct
{
   const GLfixed *m;

} EVGL_API_Thread_Command_glLoadMatrixx;

void (*orig_evgl_api_glLoadMatrixx)(const GLfixed *m);

static void
_evgl_api_thread_glLoadMatrixx(void *data)
{
   EVGL_API_Thread_Command_glLoadMatrixx *thread_data =
      (EVGL_API_Thread_Command_glLoadMatrixx *)data;

   orig_evgl_api_glLoadMatrixx(thread_data->m);

}

EAPI void
glLoadMatrixx_evgl_api_thread_cmd(const GLfixed *m)
{
   if (!evas_evgl_thread_enabled() || (_main_thread_id != eina_thread_self()))
     {
        orig_evgl_api_glLoadMatrixx(m);
        return;
     }

   int thread_mode = EVAS_GL_THREAD_MODE_FINISH;

   EVGL_API_Thread_Command_glLoadMatrixx thread_data_local;
   EVGL_API_Thread_Command_glLoadMatrixx *thread_data = &thread_data_local;

   thread_data->m = m;

   evas_gl_thread_cmd_enqueue(EVAS_GL_THREAD_TYPE_EVGL,
                              _evgl_api_thread_glLoadMatrixx,
                              thread_data,
                              thread_mode);
}

/*
 * void
 * glLogicOp(GLenum opcode);
 */

typedef struct
{
   GLenum opcode;

} EVGL_API_Thread_Command_glLogicOp;

void (*orig_evgl_api_glLogicOp)(GLenum opcode);

static void
_evgl_api_thread_glLogicOp(void *data)
{
   EVGL_API_Thread_Command_glLogicOp *thread_data =
      (EVGL_API_Thread_Command_glLogicOp *)data;

   orig_evgl_api_glLogicOp(thread_data->opcode);

}

EAPI void
glLogicOp_evgl_api_thread_cmd(GLenum opcode)
{
   if (!evas_evgl_thread_enabled() || (_main_thread_id != eina_thread_self()))
     {
        orig_evgl_api_glLogicOp(opcode);
        return;
     }

   int thread_mode = EVAS_GL_THREAD_MODE_FINISH;

   EVGL_API_Thread_Command_glLogicOp thread_data_local;
   EVGL_API_Thread_Command_glLogicOp *thread_data = &thread_data_local;

   thread_data->opcode = opcode;

   evas_gl_thread_cmd_enqueue(EVAS_GL_THREAD_TYPE_EVGL,
                              _evgl_api_thread_glLogicOp,
                              thread_data,
                              thread_mode);
}

/*
 * void
 * glMaterialx(GLenum face, GLenum pname, GLfixed param);
 */

typedef struct
{
   GLenum face;
   GLenum pname;
   GLfixed param;

} EVGL_API_Thread_Command_glMaterialx;

void (*orig_evgl_api_glMaterialx)(GLenum face, GLenum pname, GLfixed param);

static void
_evgl_api_thread_glMaterialx(void *data)
{
   EVGL_API_Thread_Command_glMaterialx *thread_data =
      (EVGL_API_Thread_Command_glMaterialx *)data;

   orig_evgl_api_glMaterialx(thread_data->face,
                             thread_data->pname,
                             thread_data->param);

}

EAPI void
glMaterialx_evgl_api_thread_cmd(GLenum face, GLenum pname, GLfixed param)
{
   if (!evas_evgl_thread_enabled() || (_main_thread_id != eina_thread_self()))
     {
        orig_evgl_api_glMaterialx(face, pname, param);
        return;
     }

   int thread_mode = EVAS_GL_THREAD_MODE_FINISH;

   EVGL_API_Thread_Command_glMaterialx thread_data_local;
   EVGL_API_Thread_Command_glMaterialx *thread_data = &thread_data_local;

   thread_data->face = face;
   thread_data->pname = pname;
   thread_data->param = param;

   evas_gl_thread_cmd_enqueue(EVAS_GL_THREAD_TYPE_EVGL,
                              _evgl_api_thread_glMaterialx,
                              thread_data,
                              thread_mode);
}

/*
 * void
 * glMaterialxv(GLenum face, GLenum pname, const GLfixed *params);
 */

typedef struct
{
   GLenum face;
   GLenum pname;
   const GLfixed *params;

} EVGL_API_Thread_Command_glMaterialxv;

void (*orig_evgl_api_glMaterialxv)(GLenum face, GLenum pname, const GLfixed *params);

static void
_evgl_api_thread_glMaterialxv(void *data)
{
   EVGL_API_Thread_Command_glMaterialxv *thread_data =
      (EVGL_API_Thread_Command_glMaterialxv *)data;

   orig_evgl_api_glMaterialxv(thread_data->face,
                              thread_data->pname,
                              thread_data->params);

}

EAPI void
glMaterialxv_evgl_api_thread_cmd(GLenum face, GLenum pname, const GLfixed *params)
{
   if (!evas_evgl_thread_enabled() || (_main_thread_id != eina_thread_self()))
     {
        orig_evgl_api_glMaterialxv(face, pname, params);
        return;
     }

   int thread_mode = EVAS_GL_THREAD_MODE_FINISH;

   EVGL_API_Thread_Command_glMaterialxv thread_data_local;
   EVGL_API_Thread_Command_glMaterialxv *thread_data = &thread_data_local;

   thread_data->face = face;
   thread_data->pname = pname;
   thread_data->params = params;

   evas_gl_thread_cmd_enqueue(EVAS_GL_THREAD_TYPE_EVGL,
                              _evgl_api_thread_glMaterialxv,
                              thread_data,
                              thread_mode);
}

/*
 * void
 * glMatrixMode(GLenum mode);
 */

typedef struct
{
   GLenum mode;

} EVGL_API_Thread_Command_glMatrixMode;

void (*orig_evgl_api_glMatrixMode)(GLenum mode);

static void
_evgl_api_thread_glMatrixMode(void *data)
{
   EVGL_API_Thread_Command_glMatrixMode *thread_data =
      (EVGL_API_Thread_Command_glMatrixMode *)data;

   orig_evgl_api_glMatrixMode(thread_data->mode);

}

EAPI void
glMatrixMode_evgl_api_thread_cmd(GLenum mode)
{
   if (!evas_evgl_thread_enabled() || (_main_thread_id != eina_thread_self()))
     {
        orig_evgl_api_glMatrixMode(mode);
        return;
     }

   int thread_mode = EVAS_GL_THREAD_MODE_FINISH;

   EVGL_API_Thread_Command_glMatrixMode thread_data_local;
   EVGL_API_Thread_Command_glMatrixMode *thread_data = &thread_data_local;

   thread_data->mode = mode;

   evas_gl_thread_cmd_enqueue(EVAS_GL_THREAD_TYPE_EVGL,
                              _evgl_api_thread_glMatrixMode,
                              thread_data,
                              thread_mode);
}

/*
 * void
 * glMultMatrixx(const GLfixed *m);
 */

typedef struct
{
   const GLfixed *m;

} EVGL_API_Thread_Command_glMultMatrixx;

void (*orig_evgl_api_glMultMatrixx)(const GLfixed *m);

static void
_evgl_api_thread_glMultMatrixx(void *data)
{
   EVGL_API_Thread_Command_glMultMatrixx *thread_data =
      (EVGL_API_Thread_Command_glMultMatrixx *)data;

   orig_evgl_api_glMultMatrixx(thread_data->m);

}

EAPI void
glMultMatrixx_evgl_api_thread_cmd(const GLfixed *m)
{
   if (!evas_evgl_thread_enabled() || (_main_thread_id != eina_thread_self()))
     {
        orig_evgl_api_glMultMatrixx(m);
        return;
     }

   int thread_mode = EVAS_GL_THREAD_MODE_FINISH;

   EVGL_API_Thread_Command_glMultMatrixx thread_data_local;
   EVGL_API_Thread_Command_glMultMatrixx *thread_data = &thread_data_local;

   thread_data->m = m;

   evas_gl_thread_cmd_enqueue(EVAS_GL_THREAD_TYPE_EVGL,
                              _evgl_api_thread_glMultMatrixx,
                              thread_data,
                              thread_mode);
}

/*
 * void
 * glMultiTexCoord4x(GLenum target, GLfixed s, GLfixed t, GLfixed r, GLfixed q);
 */

typedef struct
{
   GLenum target;
   GLfixed s;
   GLfixed t;
   GLfixed r;
   GLfixed q;

} EVGL_API_Thread_Command_glMultiTexCoord4x;

void (*orig_evgl_api_glMultiTexCoord4x)(GLenum target, GLfixed s, GLfixed t, GLfixed r, GLfixed q);

static void
_evgl_api_thread_glMultiTexCoord4x(void *data)
{
   EVGL_API_Thread_Command_glMultiTexCoord4x *thread_data =
      (EVGL_API_Thread_Command_glMultiTexCoord4x *)data;

   orig_evgl_api_glMultiTexCoord4x(thread_data->target,
                                   thread_data->s,
                                   thread_data->t,
                                   thread_data->r,
                                   thread_data->q);

}

EAPI void
glMultiTexCoord4x_evgl_api_thread_cmd(GLenum target, GLfixed s, GLfixed t, GLfixed r, GLfixed q)
{
   if (!evas_evgl_thread_enabled() || (_main_thread_id != eina_thread_self()))
     {
        orig_evgl_api_glMultiTexCoord4x(target, s, t, r, q);
        return;
     }

   int thread_mode = EVAS_GL_THREAD_MODE_FINISH;

   EVGL_API_Thread_Command_glMultiTexCoord4x thread_data_local;
   EVGL_API_Thread_Command_glMultiTexCoord4x *thread_data = &thread_data_local;

   thread_data->target = target;
   thread_data->s = s;
   thread_data->t = t;
   thread_data->r = r;
   thread_data->q = q;

   evas_gl_thread_cmd_enqueue(EVAS_GL_THREAD_TYPE_EVGL,
                              _evgl_api_thread_glMultiTexCoord4x,
                              thread_data,
                              thread_mode);
}

/*
 * void
 * glNormal3x(GLfixed nx, GLfixed ny, GLfixed nz);
 */

typedef struct
{
   GLfixed nx;
   GLfixed ny;
   GLfixed nz;

} EVGL_API_Thread_Command_glNormal3x;

void (*orig_evgl_api_glNormal3x)(GLfixed nx, GLfixed ny, GLfixed nz);

static void
_evgl_api_thread_glNormal3x(void *data)
{
   EVGL_API_Thread_Command_glNormal3x *thread_data =
      (EVGL_API_Thread_Command_glNormal3x *)data;

   orig_evgl_api_glNormal3x(thread_data->nx,
                            thread_data->ny,
                            thread_data->nz);

}

EAPI void
glNormal3x_evgl_api_thread_cmd(GLfixed nx, GLfixed ny, GLfixed nz)
{
   if (!evas_evgl_thread_enabled() || (_main_thread_id != eina_thread_self()))
     {
        orig_evgl_api_glNormal3x(nx, ny, nz);
        return;
     }

   int thread_mode = EVAS_GL_THREAD_MODE_FINISH;

   EVGL_API_Thread_Command_glNormal3x thread_data_local;
   EVGL_API_Thread_Command_glNormal3x *thread_data = &thread_data_local;

   thread_data->nx = nx;
   thread_data->ny = ny;
   thread_data->nz = nz;

   evas_gl_thread_cmd_enqueue(EVAS_GL_THREAD_TYPE_EVGL,
                              _evgl_api_thread_glNormal3x,
                              thread_data,
                              thread_mode);
}

/*
 * void
 * glNormalPointer(GLenum type, GLsizei stride, const GLvoid *pointer);
 */

typedef struct
{
   GLenum type;
   GLsizei stride;
   const GLvoid *pointer;

} EVGL_API_Thread_Command_glNormalPointer;

void (*orig_evgl_api_glNormalPointer)(GLenum type, GLsizei stride, const GLvoid *pointer);

static void
_evgl_api_thread_glNormalPointer(void *data)
{
   EVGL_API_Thread_Command_glNormalPointer *thread_data =
      (EVGL_API_Thread_Command_glNormalPointer *)data;

   orig_evgl_api_glNormalPointer(thread_data->type,
                                 thread_data->stride,
                                 thread_data->pointer);

}

EAPI void
glNormalPointer_evgl_api_thread_cmd(GLenum type, GLsizei stride, const GLvoid *pointer)
{
   if (!evas_evgl_thread_enabled() || (_main_thread_id != eina_thread_self()))
     {
        orig_evgl_api_glNormalPointer(type, stride, pointer);
        return;
     }

   int thread_mode = EVAS_GL_THREAD_MODE_FINISH;

   EVGL_API_Thread_Command_glNormalPointer thread_data_local;
   EVGL_API_Thread_Command_glNormalPointer *thread_data = &thread_data_local;

   thread_data->type = type;
   thread_data->stride = stride;
   thread_data->pointer = pointer;

   evas_gl_thread_cmd_enqueue(EVAS_GL_THREAD_TYPE_EVGL,
                              _evgl_api_thread_glNormalPointer,
                              thread_data,
                              thread_mode);
}

/*
 * void
 * glOrthox(GLfixed left, GLfixed right, GLfixed bottom, GLfixed top, GLfixed zNear, GLfixed zFar);
 */

typedef struct
{
   GLfixed left;
   GLfixed right;
   GLfixed bottom;
   GLfixed top;
   GLfixed zNear;
   GLfixed zFar;

} EVGL_API_Thread_Command_glOrthox;

void (*orig_evgl_api_glOrthox)(GLfixed left, GLfixed right, GLfixed bottom, GLfixed top, GLfixed zNear, GLfixed zFar);

static void
_evgl_api_thread_glOrthox(void *data)
{
   EVGL_API_Thread_Command_glOrthox *thread_data =
      (EVGL_API_Thread_Command_glOrthox *)data;

   orig_evgl_api_glOrthox(thread_data->left,
                          thread_data->right,
                          thread_data->bottom,
                          thread_data->top,
                          thread_data->zNear,
                          thread_data->zFar);

}

EAPI void
glOrthox_evgl_api_thread_cmd(GLfixed left, GLfixed right, GLfixed bottom, GLfixed top, GLfixed zNear, GLfixed zFar)
{
   if (!evas_evgl_thread_enabled() || (_main_thread_id != eina_thread_self()))
     {
        orig_evgl_api_glOrthox(left, right, bottom, top, zNear, zFar);
        return;
     }

   int thread_mode = EVAS_GL_THREAD_MODE_FINISH;

   EVGL_API_Thread_Command_glOrthox thread_data_local;
   EVGL_API_Thread_Command_glOrthox *thread_data = &thread_data_local;

   thread_data->left = left;
   thread_data->right = right;
   thread_data->bottom = bottom;
   thread_data->top = top;
   thread_data->zNear = zNear;
   thread_data->zFar = zFar;

   evas_gl_thread_cmd_enqueue(EVAS_GL_THREAD_TYPE_EVGL,
                              _evgl_api_thread_glOrthox,
                              thread_data,
                              thread_mode);
}

/*
 * void
 * glPointParameterx(GLenum pname, GLfixed param);
 */

typedef struct
{
   GLenum pname;
   GLfixed param;

} EVGL_API_Thread_Command_glPointParameterx;

void (*orig_evgl_api_glPointParameterx)(GLenum pname, GLfixed param);

static void
_evgl_api_thread_glPointParameterx(void *data)
{
   EVGL_API_Thread_Command_glPointParameterx *thread_data =
      (EVGL_API_Thread_Command_glPointParameterx *)data;

   orig_evgl_api_glPointParameterx(thread_data->pname,
                                   thread_data->param);

}

EAPI void
glPointParameterx_evgl_api_thread_cmd(GLenum pname, GLfixed param)
{
   if (!evas_evgl_thread_enabled() || (_main_thread_id != eina_thread_self()))
     {
        orig_evgl_api_glPointParameterx(pname, param);
        return;
     }

   int thread_mode = EVAS_GL_THREAD_MODE_FINISH;

   EVGL_API_Thread_Command_glPointParameterx thread_data_local;
   EVGL_API_Thread_Command_glPointParameterx *thread_data = &thread_data_local;

   thread_data->pname = pname;
   thread_data->param = param;

   evas_gl_thread_cmd_enqueue(EVAS_GL_THREAD_TYPE_EVGL,
                              _evgl_api_thread_glPointParameterx,
                              thread_data,
                              thread_mode);
}

/*
 * void
 * glPointParameterxv(GLenum pname, const GLfixed *params);
 */

typedef struct
{
   GLenum pname;
   const GLfixed *params;

} EVGL_API_Thread_Command_glPointParameterxv;

void (*orig_evgl_api_glPointParameterxv)(GLenum pname, const GLfixed *params);

static void
_evgl_api_thread_glPointParameterxv(void *data)
{
   EVGL_API_Thread_Command_glPointParameterxv *thread_data =
      (EVGL_API_Thread_Command_glPointParameterxv *)data;

   orig_evgl_api_glPointParameterxv(thread_data->pname,
                                    thread_data->params);

}

EAPI void
glPointParameterxv_evgl_api_thread_cmd(GLenum pname, const GLfixed *params)
{
   if (!evas_evgl_thread_enabled() || (_main_thread_id != eina_thread_self()))
     {
        orig_evgl_api_glPointParameterxv(pname, params);
        return;
     }

   int thread_mode = EVAS_GL_THREAD_MODE_FINISH;

   EVGL_API_Thread_Command_glPointParameterxv thread_data_local;
   EVGL_API_Thread_Command_glPointParameterxv *thread_data = &thread_data_local;

   thread_data->pname = pname;
   thread_data->params = params;

   evas_gl_thread_cmd_enqueue(EVAS_GL_THREAD_TYPE_EVGL,
                              _evgl_api_thread_glPointParameterxv,
                              thread_data,
                              thread_mode);
}

/*
 * void
 * glPointSizex(GLfixed size);
 */

typedef struct
{
   GLfixed size;

} EVGL_API_Thread_Command_glPointSizex;

void (*orig_evgl_api_glPointSizex)(GLfixed size);

static void
_evgl_api_thread_glPointSizex(void *data)
{
   EVGL_API_Thread_Command_glPointSizex *thread_data =
      (EVGL_API_Thread_Command_glPointSizex *)data;

   orig_evgl_api_glPointSizex(thread_data->size);

}

EAPI void
glPointSizex_evgl_api_thread_cmd(GLfixed size)
{
   if (!evas_evgl_thread_enabled() || (_main_thread_id != eina_thread_self()))
     {
        orig_evgl_api_glPointSizex(size);
        return;
     }

   int thread_mode = EVAS_GL_THREAD_MODE_FINISH;

   EVGL_API_Thread_Command_glPointSizex thread_data_local;
   EVGL_API_Thread_Command_glPointSizex *thread_data = &thread_data_local;

   thread_data->size = size;

   evas_gl_thread_cmd_enqueue(EVAS_GL_THREAD_TYPE_EVGL,
                              _evgl_api_thread_glPointSizex,
                              thread_data,
                              thread_mode);
}

/*
 * void
 * glPolygonOffsetx(GLfixed factor, GLfixed units);
 */

typedef struct
{
   GLfixed factor;
   GLfixed units;

} EVGL_API_Thread_Command_glPolygonOffsetx;

void (*orig_evgl_api_glPolygonOffsetx)(GLfixed factor, GLfixed units);

static void
_evgl_api_thread_glPolygonOffsetx(void *data)
{
   EVGL_API_Thread_Command_glPolygonOffsetx *thread_data =
      (EVGL_API_Thread_Command_glPolygonOffsetx *)data;

   orig_evgl_api_glPolygonOffsetx(thread_data->factor,
                                  thread_data->units);

}

EAPI void
glPolygonOffsetx_evgl_api_thread_cmd(GLfixed factor, GLfixed units)
{
   if (!evas_evgl_thread_enabled() || (_main_thread_id != eina_thread_self()))
     {
        orig_evgl_api_glPolygonOffsetx(factor, units);
        return;
     }

   int thread_mode = EVAS_GL_THREAD_MODE_FINISH;

   EVGL_API_Thread_Command_glPolygonOffsetx thread_data_local;
   EVGL_API_Thread_Command_glPolygonOffsetx *thread_data = &thread_data_local;

   thread_data->factor = factor;
   thread_data->units = units;

   evas_gl_thread_cmd_enqueue(EVAS_GL_THREAD_TYPE_EVGL,
                              _evgl_api_thread_glPolygonOffsetx,
                              thread_data,
                              thread_mode);
}

/*
 * void
 * glPopMatrix(void);
 */

void (*orig_evgl_api_glPopMatrix)(void);

static void
_evgl_api_thread_glPopMatrix(void *data EINA_UNUSED)
{
   orig_evgl_api_glPopMatrix();

}

EAPI void
glPopMatrix_evgl_api_thread_cmd(void)
{
   if (!evas_evgl_thread_enabled() || (_main_thread_id != eina_thread_self()))
     {
        orig_evgl_api_glPopMatrix();
        return;
     }

   int thread_mode = EVAS_GL_THREAD_MODE_FINISH;

   evas_gl_thread_cmd_enqueue(EVAS_GL_THREAD_TYPE_EVGL,
                              _evgl_api_thread_glPopMatrix,
                              NULL,
                              thread_mode);
}

/*
 * void
 * glPushMatrix(void);
 */

void (*orig_evgl_api_glPushMatrix)(void);

static void
_evgl_api_thread_glPushMatrix(void *data EINA_UNUSED)
{
   orig_evgl_api_glPushMatrix();

}

EAPI void
glPushMatrix_evgl_api_thread_cmd(void)
{
   if (!evas_evgl_thread_enabled() || (_main_thread_id != eina_thread_self()))
     {
        orig_evgl_api_glPushMatrix();
        return;
     }

   int thread_mode = EVAS_GL_THREAD_MODE_FINISH;

   evas_gl_thread_cmd_enqueue(EVAS_GL_THREAD_TYPE_EVGL,
                              _evgl_api_thread_glPushMatrix,
                              NULL,
                              thread_mode);
}

/*
 * void
 * glRotatex(GLfixed angle, GLfixed x, GLfixed y, GLfixed z);
 */

typedef struct
{
   GLfixed angle;
   GLfixed x;
   GLfixed y;
   GLfixed z;

} EVGL_API_Thread_Command_glRotatex;

void (*orig_evgl_api_glRotatex)(GLfixed angle, GLfixed x, GLfixed y, GLfixed z);

static void
_evgl_api_thread_glRotatex(void *data)
{
   EVGL_API_Thread_Command_glRotatex *thread_data =
      (EVGL_API_Thread_Command_glRotatex *)data;

   orig_evgl_api_glRotatex(thread_data->angle,
                           thread_data->x,
                           thread_data->y,
                           thread_data->z);

}

EAPI void
glRotatex_evgl_api_thread_cmd(GLfixed angle, GLfixed x, GLfixed y, GLfixed z)
{
   if (!evas_evgl_thread_enabled() || (_main_thread_id != eina_thread_self()))
     {
        orig_evgl_api_glRotatex(angle, x, y, z);
        return;
     }

   int thread_mode = EVAS_GL_THREAD_MODE_FINISH;

   EVGL_API_Thread_Command_glRotatex thread_data_local;
   EVGL_API_Thread_Command_glRotatex *thread_data = &thread_data_local;

   thread_data->angle = angle;
   thread_data->x = x;
   thread_data->y = y;
   thread_data->z = z;

   evas_gl_thread_cmd_enqueue(EVAS_GL_THREAD_TYPE_EVGL,
                              _evgl_api_thread_glRotatex,
                              thread_data,
                              thread_mode);
}

/*
 * void
 * glSampleCoveragex(GLclampx value, GLboolean invert);
 */

typedef struct
{
   GLclampx value;
   GLboolean invert;

} EVGL_API_Thread_Command_glSampleCoveragex;

void (*orig_evgl_api_glSampleCoveragex)(GLclampx value, GLboolean invert);

static void
_evgl_api_thread_glSampleCoveragex(void *data)
{
   EVGL_API_Thread_Command_glSampleCoveragex *thread_data =
      (EVGL_API_Thread_Command_glSampleCoveragex *)data;

   orig_evgl_api_glSampleCoveragex(thread_data->value,
                                   thread_data->invert);

}

EAPI void
glSampleCoveragex_evgl_api_thread_cmd(GLclampx value, GLboolean invert)
{
   if (!evas_evgl_thread_enabled() || (_main_thread_id != eina_thread_self()))
     {
        orig_evgl_api_glSampleCoveragex(value, invert);
        return;
     }

   int thread_mode = EVAS_GL_THREAD_MODE_FINISH;

   EVGL_API_Thread_Command_glSampleCoveragex thread_data_local;
   EVGL_API_Thread_Command_glSampleCoveragex *thread_data = &thread_data_local;

   thread_data->value = value;
   thread_data->invert = invert;

   evas_gl_thread_cmd_enqueue(EVAS_GL_THREAD_TYPE_EVGL,
                              _evgl_api_thread_glSampleCoveragex,
                              thread_data,
                              thread_mode);
}

/*
 * void
 * glScalex(GLfixed x, GLfixed y, GLfixed z);
 */

typedef struct
{
   GLfixed x;
   GLfixed y;
   GLfixed z;

} EVGL_API_Thread_Command_glScalex;

void (*orig_evgl_api_glScalex)(GLfixed x, GLfixed y, GLfixed z);

static void
_evgl_api_thread_glScalex(void *data)
{
   EVGL_API_Thread_Command_glScalex *thread_data =
      (EVGL_API_Thread_Command_glScalex *)data;

   orig_evgl_api_glScalex(thread_data->x,
                          thread_data->y,
                          thread_data->z);

}

EAPI void
glScalex_evgl_api_thread_cmd(GLfixed x, GLfixed y, GLfixed z)
{
   if (!evas_evgl_thread_enabled() || (_main_thread_id != eina_thread_self()))
     {
        orig_evgl_api_glScalex(x, y, z);
        return;
     }

   int thread_mode = EVAS_GL_THREAD_MODE_FINISH;

   EVGL_API_Thread_Command_glScalex thread_data_local;
   EVGL_API_Thread_Command_glScalex *thread_data = &thread_data_local;

   thread_data->x = x;
   thread_data->y = y;
   thread_data->z = z;

   evas_gl_thread_cmd_enqueue(EVAS_GL_THREAD_TYPE_EVGL,
                              _evgl_api_thread_glScalex,
                              thread_data,
                              thread_mode);
}

/*
 * void
 * glShadeModel(GLenum mode);
 */

typedef struct
{
   GLenum mode;

} EVGL_API_Thread_Command_glShadeModel;

void (*orig_evgl_api_glShadeModel)(GLenum mode);

static void
_evgl_api_thread_glShadeModel(void *data)
{
   EVGL_API_Thread_Command_glShadeModel *thread_data =
      (EVGL_API_Thread_Command_glShadeModel *)data;

   orig_evgl_api_glShadeModel(thread_data->mode);

}

EAPI void
glShadeModel_evgl_api_thread_cmd(GLenum mode)
{
   if (!evas_evgl_thread_enabled() || (_main_thread_id != eina_thread_self()))
     {
        orig_evgl_api_glShadeModel(mode);
        return;
     }

   int thread_mode = EVAS_GL_THREAD_MODE_FINISH;

   EVGL_API_Thread_Command_glShadeModel thread_data_local;
   EVGL_API_Thread_Command_glShadeModel *thread_data = &thread_data_local;

   thread_data->mode = mode;

   evas_gl_thread_cmd_enqueue(EVAS_GL_THREAD_TYPE_EVGL,
                              _evgl_api_thread_glShadeModel,
                              thread_data,
                              thread_mode);
}

/*
 * void
 * glTexCoordPointer(GLint size, GLenum type, GLsizei stride, const GLvoid *pointer);
 */

typedef struct
{
   GLint size;
   GLenum type;
   GLsizei stride;
   const GLvoid *pointer;

} EVGL_API_Thread_Command_glTexCoordPointer;

void (*orig_evgl_api_glTexCoordPointer)(GLint size, GLenum type, GLsizei stride, const GLvoid *pointer);

static void
_evgl_api_thread_glTexCoordPointer(void *data)
{
   EVGL_API_Thread_Command_glTexCoordPointer *thread_data =
      (EVGL_API_Thread_Command_glTexCoordPointer *)data;

   orig_evgl_api_glTexCoordPointer(thread_data->size,
                                   thread_data->type,
                                   thread_data->stride,
                                   thread_data->pointer);

}

EAPI void
glTexCoordPointer_evgl_api_thread_cmd(GLint size, GLenum type, GLsizei stride, const GLvoid *pointer)
{
   if (!evas_evgl_thread_enabled() || (_main_thread_id != eina_thread_self()))
     {
        orig_evgl_api_glTexCoordPointer(size, type, stride, pointer);
        return;
     }

   int thread_mode = EVAS_GL_THREAD_MODE_FINISH;

   EVGL_API_Thread_Command_glTexCoordPointer thread_data_local;
   EVGL_API_Thread_Command_glTexCoordPointer *thread_data = &thread_data_local;

   thread_data->size = size;
   thread_data->type = type;
   thread_data->stride = stride;
   thread_data->pointer = pointer;

   evas_gl_thread_cmd_enqueue(EVAS_GL_THREAD_TYPE_EVGL,
                              _evgl_api_thread_glTexCoordPointer,
                              thread_data,
                              thread_mode);
}

/*
 * void
 * glTexEnvi(GLenum target, GLenum pname, GLint param);
 */

typedef struct
{
   GLenum target;
   GLenum pname;
   GLint param;

} EVGL_API_Thread_Command_glTexEnvi;

void (*orig_evgl_api_glTexEnvi)(GLenum target, GLenum pname, GLint param);

static void
_evgl_api_thread_glTexEnvi(void *data)
{
   EVGL_API_Thread_Command_glTexEnvi *thread_data =
      (EVGL_API_Thread_Command_glTexEnvi *)data;

   orig_evgl_api_glTexEnvi(thread_data->target,
                           thread_data->pname,
                           thread_data->param);

}

EAPI void
glTexEnvi_evgl_api_thread_cmd(GLenum target, GLenum pname, GLint param)
{
   if (!evas_evgl_thread_enabled() || (_main_thread_id != eina_thread_self()))
     {
        orig_evgl_api_glTexEnvi(target, pname, param);
        return;
     }

   int thread_mode = EVAS_GL_THREAD_MODE_FINISH;

   EVGL_API_Thread_Command_glTexEnvi thread_data_local;
   EVGL_API_Thread_Command_glTexEnvi *thread_data = &thread_data_local;

   thread_data->target = target;
   thread_data->pname = pname;
   thread_data->param = param;

   evas_gl_thread_cmd_enqueue(EVAS_GL_THREAD_TYPE_EVGL,
                              _evgl_api_thread_glTexEnvi,
                              thread_data,
                              thread_mode);
}

/*
 * void
 * glTexEnvx(GLenum target, GLenum pname, GLfixed param);
 */

typedef struct
{
   GLenum target;
   GLenum pname;
   GLfixed param;

} EVGL_API_Thread_Command_glTexEnvx;

void (*orig_evgl_api_glTexEnvx)(GLenum target, GLenum pname, GLfixed param);

static void
_evgl_api_thread_glTexEnvx(void *data)
{
   EVGL_API_Thread_Command_glTexEnvx *thread_data =
      (EVGL_API_Thread_Command_glTexEnvx *)data;

   orig_evgl_api_glTexEnvx(thread_data->target,
                           thread_data->pname,
                           thread_data->param);

}

EAPI void
glTexEnvx_evgl_api_thread_cmd(GLenum target, GLenum pname, GLfixed param)
{
   if (!evas_evgl_thread_enabled() || (_main_thread_id != eina_thread_self()))
     {
        orig_evgl_api_glTexEnvx(target, pname, param);
        return;
     }

   int thread_mode = EVAS_GL_THREAD_MODE_FINISH;

   EVGL_API_Thread_Command_glTexEnvx thread_data_local;
   EVGL_API_Thread_Command_glTexEnvx *thread_data = &thread_data_local;

   thread_data->target = target;
   thread_data->pname = pname;
   thread_data->param = param;

   evas_gl_thread_cmd_enqueue(EVAS_GL_THREAD_TYPE_EVGL,
                              _evgl_api_thread_glTexEnvx,
                              thread_data,
                              thread_mode);
}

/*
 * void
 * glTexEnviv(GLenum target, GLenum pname, const GLint *params);
 */

typedef struct
{
   GLenum target;
   GLenum pname;
   const GLint *params;

} EVGL_API_Thread_Command_glTexEnviv;

void (*orig_evgl_api_glTexEnviv)(GLenum target, GLenum pname, const GLint *params);

static void
_evgl_api_thread_glTexEnviv(void *data)
{
   EVGL_API_Thread_Command_glTexEnviv *thread_data =
      (EVGL_API_Thread_Command_glTexEnviv *)data;

   orig_evgl_api_glTexEnviv(thread_data->target,
                            thread_data->pname,
                            thread_data->params);

}

EAPI void
glTexEnviv_evgl_api_thread_cmd(GLenum target, GLenum pname, const GLint *params)
{
   if (!evas_evgl_thread_enabled() || (_main_thread_id != eina_thread_self()))
     {
        orig_evgl_api_glTexEnviv(target, pname, params);
        return;
     }

   int thread_mode = EVAS_GL_THREAD_MODE_FINISH;

   EVGL_API_Thread_Command_glTexEnviv thread_data_local;
   EVGL_API_Thread_Command_glTexEnviv *thread_data = &thread_data_local;

   thread_data->target = target;
   thread_data->pname = pname;
   thread_data->params = params;

   evas_gl_thread_cmd_enqueue(EVAS_GL_THREAD_TYPE_EVGL,
                              _evgl_api_thread_glTexEnviv,
                              thread_data,
                              thread_mode);
}

/*
 * void
 * glTexEnvxv(GLenum target, GLenum pname, const GLfixed *params);
 */

typedef struct
{
   GLenum target;
   GLenum pname;
   const GLfixed *params;

} EVGL_API_Thread_Command_glTexEnvxv;

void (*orig_evgl_api_glTexEnvxv)(GLenum target, GLenum pname, const GLfixed *params);

static void
_evgl_api_thread_glTexEnvxv(void *data)
{
   EVGL_API_Thread_Command_glTexEnvxv *thread_data =
      (EVGL_API_Thread_Command_glTexEnvxv *)data;

   orig_evgl_api_glTexEnvxv(thread_data->target,
                            thread_data->pname,
                            thread_data->params);

}

EAPI void
glTexEnvxv_evgl_api_thread_cmd(GLenum target, GLenum pname, const GLfixed *params)
{
   if (!evas_evgl_thread_enabled() || (_main_thread_id != eina_thread_self()))
     {
        orig_evgl_api_glTexEnvxv(target, pname, params);
        return;
     }

   int thread_mode = EVAS_GL_THREAD_MODE_FINISH;

   EVGL_API_Thread_Command_glTexEnvxv thread_data_local;
   EVGL_API_Thread_Command_glTexEnvxv *thread_data = &thread_data_local;

   thread_data->target = target;
   thread_data->pname = pname;
   thread_data->params = params;

   evas_gl_thread_cmd_enqueue(EVAS_GL_THREAD_TYPE_EVGL,
                              _evgl_api_thread_glTexEnvxv,
                              thread_data,
                              thread_mode);
}

/*
 * void
 * glTexParameterx(GLenum target, GLenum pname, GLfixed param);
 */

typedef struct
{
   GLenum target;
   GLenum pname;
   GLfixed param;

} EVGL_API_Thread_Command_glTexParameterx;

void (*orig_evgl_api_glTexParameterx)(GLenum target, GLenum pname, GLfixed param);

static void
_evgl_api_thread_glTexParameterx(void *data)
{
   EVGL_API_Thread_Command_glTexParameterx *thread_data =
      (EVGL_API_Thread_Command_glTexParameterx *)data;

   orig_evgl_api_glTexParameterx(thread_data->target,
                                 thread_data->pname,
                                 thread_data->param);

}

EAPI void
glTexParameterx_evgl_api_thread_cmd(GLenum target, GLenum pname, GLfixed param)
{
   if (!evas_evgl_thread_enabled() || (_main_thread_id != eina_thread_self()))
     {
        orig_evgl_api_glTexParameterx(target, pname, param);
        return;
     }

   int thread_mode = EVAS_GL_THREAD_MODE_FINISH;

   EVGL_API_Thread_Command_glTexParameterx thread_data_local;
   EVGL_API_Thread_Command_glTexParameterx *thread_data = &thread_data_local;

   thread_data->target = target;
   thread_data->pname = pname;
   thread_data->param = param;

   evas_gl_thread_cmd_enqueue(EVAS_GL_THREAD_TYPE_EVGL,
                              _evgl_api_thread_glTexParameterx,
                              thread_data,
                              thread_mode);
}

/*
 * void
 * glTexParameterxv(GLenum target, GLenum pname, const GLfixed *params);
 */

typedef struct
{
   GLenum target;
   GLenum pname;
   const GLfixed *params;

} EVGL_API_Thread_Command_glTexParameterxv;

void (*orig_evgl_api_glTexParameterxv)(GLenum target, GLenum pname, const GLfixed *params);

static void
_evgl_api_thread_glTexParameterxv(void *data)
{
   EVGL_API_Thread_Command_glTexParameterxv *thread_data =
      (EVGL_API_Thread_Command_glTexParameterxv *)data;

   orig_evgl_api_glTexParameterxv(thread_data->target,
                                  thread_data->pname,
                                  thread_data->params);

}

EAPI void
glTexParameterxv_evgl_api_thread_cmd(GLenum target, GLenum pname, const GLfixed *params)
{
   if (!evas_evgl_thread_enabled() || (_main_thread_id != eina_thread_self()))
     {
        orig_evgl_api_glTexParameterxv(target, pname, params);
        return;
     }

   int thread_mode = EVAS_GL_THREAD_MODE_FINISH;

   EVGL_API_Thread_Command_glTexParameterxv thread_data_local;
   EVGL_API_Thread_Command_glTexParameterxv *thread_data = &thread_data_local;

   thread_data->target = target;
   thread_data->pname = pname;
   thread_data->params = params;

   evas_gl_thread_cmd_enqueue(EVAS_GL_THREAD_TYPE_EVGL,
                              _evgl_api_thread_glTexParameterxv,
                              thread_data,
                              thread_mode);
}

/*
 * void
 * glTranslatex(GLfixed x, GLfixed y, GLfixed z);
 */

typedef struct
{
   GLfixed x;
   GLfixed y;
   GLfixed z;

} EVGL_API_Thread_Command_glTranslatex;

void (*orig_evgl_api_glTranslatex)(GLfixed x, GLfixed y, GLfixed z);

static void
_evgl_api_thread_glTranslatex(void *data)
{
   EVGL_API_Thread_Command_glTranslatex *thread_data =
      (EVGL_API_Thread_Command_glTranslatex *)data;

   orig_evgl_api_glTranslatex(thread_data->x,
                              thread_data->y,
                              thread_data->z);

}

EAPI void
glTranslatex_evgl_api_thread_cmd(GLfixed x, GLfixed y, GLfixed z)
{
   if (!evas_evgl_thread_enabled() || (_main_thread_id != eina_thread_self()))
     {
        orig_evgl_api_glTranslatex(x, y, z);
        return;
     }

   int thread_mode = EVAS_GL_THREAD_MODE_FINISH;

   EVGL_API_Thread_Command_glTranslatex thread_data_local;
   EVGL_API_Thread_Command_glTranslatex *thread_data = &thread_data_local;

   thread_data->x = x;
   thread_data->y = y;
   thread_data->z = z;

   evas_gl_thread_cmd_enqueue(EVAS_GL_THREAD_TYPE_EVGL,
                              _evgl_api_thread_glTranslatex,
                              thread_data,
                              thread_mode);
}

/*
 * void
 * glVertexPointer(GLint size, GLenum type, GLsizei stride, const GLvoid *pointer);
 */

typedef struct
{
   GLint size;
   GLenum type;
   GLsizei stride;
   const GLvoid *pointer;

} EVGL_API_Thread_Command_glVertexPointer;

void (*orig_evgl_api_glVertexPointer)(GLint size, GLenum type, GLsizei stride, const GLvoid *pointer);

static void
_evgl_api_thread_glVertexPointer(void *data)
{
   EVGL_API_Thread_Command_glVertexPointer *thread_data =
      (EVGL_API_Thread_Command_glVertexPointer *)data;

   orig_evgl_api_glVertexPointer(thread_data->size,
                                 thread_data->type,
                                 thread_data->stride,
                                 thread_data->pointer);

}

EAPI void
glVertexPointer_evgl_api_thread_cmd(GLint size, GLenum type, GLsizei stride, const GLvoid *pointer)
{
   if (!evas_evgl_thread_enabled() || (_main_thread_id != eina_thread_self()))
     {
        orig_evgl_api_glVertexPointer(size, type, stride, pointer);
        return;
     }

   int thread_mode = EVAS_GL_THREAD_MODE_FINISH;

   EVGL_API_Thread_Command_glVertexPointer thread_data_local;
   EVGL_API_Thread_Command_glVertexPointer *thread_data = &thread_data_local;

   thread_data->size = size;
   thread_data->type = type;
   thread_data->stride = stride;
   thread_data->pointer = pointer;

   evas_gl_thread_cmd_enqueue(EVAS_GL_THREAD_TYPE_EVGL,
                              _evgl_api_thread_glVertexPointer,
                              thread_data,
                              thread_mode);
}

/*
 * void
 * glBlendEquationSeparateOES(GLenum modeRGB, GLenum modeAlpha);
 */

typedef struct
{
   GLenum modeRGB;
   GLenum modeAlpha;

} EVGL_API_Thread_Command_glBlendEquationSeparateOES;

void (*orig_evgl_api_glBlendEquationSeparateOES)(GLenum modeRGB, GLenum modeAlpha);

static void
_evgl_api_thread_glBlendEquationSeparateOES(void *data)
{
   EVGL_API_Thread_Command_glBlendEquationSeparateOES *thread_data =
      (EVGL_API_Thread_Command_glBlendEquationSeparateOES *)data;

   orig_evgl_api_glBlendEquationSeparateOES(thread_data->modeRGB,
                                            thread_data->modeAlpha);

}

EAPI void
glBlendEquationSeparateOES_evgl_api_thread_cmd(GLenum modeRGB, GLenum modeAlpha)
{
   if (!evas_evgl_thread_enabled() || (_main_thread_id != eina_thread_self()))
     {
        orig_evgl_api_glBlendEquationSeparateOES(modeRGB, modeAlpha);
        return;
     }

   int thread_mode = EVAS_GL_THREAD_MODE_FINISH;

   EVGL_API_Thread_Command_glBlendEquationSeparateOES thread_data_local;
   EVGL_API_Thread_Command_glBlendEquationSeparateOES *thread_data = &thread_data_local;

   thread_data->modeRGB = modeRGB;
   thread_data->modeAlpha = modeAlpha;

   evas_gl_thread_cmd_enqueue(EVAS_GL_THREAD_TYPE_EVGL,
                              _evgl_api_thread_glBlendEquationSeparateOES,
                              thread_data,
                              thread_mode);
}

/*
 * void
 * glBlendFuncSeparateOES(GLenum srcRGB, GLenum dstRGB, GLenum srcAlpha, GLenum dstAlpha);
 */

typedef struct
{
   GLenum srcRGB;
   GLenum dstRGB;
   GLenum srcAlpha;
   GLenum dstAlpha;

} EVGL_API_Thread_Command_glBlendFuncSeparateOES;

void (*orig_evgl_api_glBlendFuncSeparateOES)(GLenum srcRGB, GLenum dstRGB, GLenum srcAlpha, GLenum dstAlpha);

static void
_evgl_api_thread_glBlendFuncSeparateOES(void *data)
{
   EVGL_API_Thread_Command_glBlendFuncSeparateOES *thread_data =
      (EVGL_API_Thread_Command_glBlendFuncSeparateOES *)data;

   orig_evgl_api_glBlendFuncSeparateOES(thread_data->srcRGB,
                                        thread_data->dstRGB,
                                        thread_data->srcAlpha,
                                        thread_data->dstAlpha);

}

EAPI void
glBlendFuncSeparateOES_evgl_api_thread_cmd(GLenum srcRGB, GLenum dstRGB, GLenum srcAlpha, GLenum dstAlpha)
{
   if (!evas_evgl_thread_enabled() || (_main_thread_id != eina_thread_self()))
     {
        orig_evgl_api_glBlendFuncSeparateOES(srcRGB, dstRGB, srcAlpha, dstAlpha);
        return;
     }

   int thread_mode = EVAS_GL_THREAD_MODE_FINISH;

   EVGL_API_Thread_Command_glBlendFuncSeparateOES thread_data_local;
   EVGL_API_Thread_Command_glBlendFuncSeparateOES *thread_data = &thread_data_local;

   thread_data->srcRGB = srcRGB;
   thread_data->dstRGB = dstRGB;
   thread_data->srcAlpha = srcAlpha;
   thread_data->dstAlpha = dstAlpha;

   evas_gl_thread_cmd_enqueue(EVAS_GL_THREAD_TYPE_EVGL,
                              _evgl_api_thread_glBlendFuncSeparateOES,
                              thread_data,
                              thread_mode);
}

/*
 * void
 * glBlendEquationOES(GLenum mode);
 */

typedef struct
{
   GLenum mode;

} EVGL_API_Thread_Command_glBlendEquationOES;

void (*orig_evgl_api_glBlendEquationOES)(GLenum mode);

static void
_evgl_api_thread_glBlendEquationOES(void *data)
{
   EVGL_API_Thread_Command_glBlendEquationOES *thread_data =
      (EVGL_API_Thread_Command_glBlendEquationOES *)data;

   orig_evgl_api_glBlendEquationOES(thread_data->mode);

}

EAPI void
glBlendEquationOES_evgl_api_thread_cmd(GLenum mode)
{
   if (!evas_evgl_thread_enabled() || (_main_thread_id != eina_thread_self()))
     {
        orig_evgl_api_glBlendEquationOES(mode);
        return;
     }

   int thread_mode = EVAS_GL_THREAD_MODE_FINISH;

   EVGL_API_Thread_Command_glBlendEquationOES thread_data_local;
   EVGL_API_Thread_Command_glBlendEquationOES *thread_data = &thread_data_local;

   thread_data->mode = mode;

   evas_gl_thread_cmd_enqueue(EVAS_GL_THREAD_TYPE_EVGL,
                              _evgl_api_thread_glBlendEquationOES,
                              thread_data,
                              thread_mode);
}

/*
 * void
 * glDrawTexsOES(GLshort x, GLshort y, GLshort z, GLshort width, GLshort height);
 */

typedef struct
{
   GLshort x;
   GLshort y;
   GLshort z;
   GLshort width;
   GLshort height;

} EVGL_API_Thread_Command_glDrawTexsOES;

void (*orig_evgl_api_glDrawTexsOES)(GLshort x, GLshort y, GLshort z, GLshort width, GLshort height);

static void
_evgl_api_thread_glDrawTexsOES(void *data)
{
   EVGL_API_Thread_Command_glDrawTexsOES *thread_data =
      (EVGL_API_Thread_Command_glDrawTexsOES *)data;

   orig_evgl_api_glDrawTexsOES(thread_data->x,
                               thread_data->y,
                               thread_data->z,
                               thread_data->width,
                               thread_data->height);

}

EAPI void
glDrawTexsOES_evgl_api_thread_cmd(GLshort x, GLshort y, GLshort z, GLshort width, GLshort height)
{
   if (!evas_evgl_thread_enabled() || (_main_thread_id != eina_thread_self()))
     {
        orig_evgl_api_glDrawTexsOES(x, y, z, width, height);
        return;
     }

   int thread_mode = EVAS_GL_THREAD_MODE_FINISH;

   EVGL_API_Thread_Command_glDrawTexsOES thread_data_local;
   EVGL_API_Thread_Command_glDrawTexsOES *thread_data = &thread_data_local;

   thread_data->x = x;
   thread_data->y = y;
   thread_data->z = z;
   thread_data->width = width;
   thread_data->height = height;

   evas_gl_thread_cmd_enqueue(EVAS_GL_THREAD_TYPE_EVGL,
                              _evgl_api_thread_glDrawTexsOES,
                              thread_data,
                              thread_mode);
}

/*
 * void
 * glDrawTexiOES(GLint x, GLint y, GLint z, GLint width, GLint height);
 */

typedef struct
{
   GLint x;
   GLint y;
   GLint z;
   GLint width;
   GLint height;

} EVGL_API_Thread_Command_glDrawTexiOES;

void (*orig_evgl_api_glDrawTexiOES)(GLint x, GLint y, GLint z, GLint width, GLint height);

static void
_evgl_api_thread_glDrawTexiOES(void *data)
{
   EVGL_API_Thread_Command_glDrawTexiOES *thread_data =
      (EVGL_API_Thread_Command_glDrawTexiOES *)data;

   orig_evgl_api_glDrawTexiOES(thread_data->x,
                               thread_data->y,
                               thread_data->z,
                               thread_data->width,
                               thread_data->height);

}

EAPI void
glDrawTexiOES_evgl_api_thread_cmd(GLint x, GLint y, GLint z, GLint width, GLint height)
{
   if (!evas_evgl_thread_enabled() || (_main_thread_id != eina_thread_self()))
     {
        orig_evgl_api_glDrawTexiOES(x, y, z, width, height);
        return;
     }

   int thread_mode = EVAS_GL_THREAD_MODE_FINISH;

   EVGL_API_Thread_Command_glDrawTexiOES thread_data_local;
   EVGL_API_Thread_Command_glDrawTexiOES *thread_data = &thread_data_local;

   thread_data->x = x;
   thread_data->y = y;
   thread_data->z = z;
   thread_data->width = width;
   thread_data->height = height;

   evas_gl_thread_cmd_enqueue(EVAS_GL_THREAD_TYPE_EVGL,
                              _evgl_api_thread_glDrawTexiOES,
                              thread_data,
                              thread_mode);
}

/*
 * void
 * glDrawTexxOES(GLfixed x, GLfixed y, GLfixed z, GLfixed width, GLfixed height);
 */

typedef struct
{
   GLfixed x;
   GLfixed y;
   GLfixed z;
   GLfixed width;
   GLfixed height;

} EVGL_API_Thread_Command_glDrawTexxOES;

void (*orig_evgl_api_glDrawTexxOES)(GLfixed x, GLfixed y, GLfixed z, GLfixed width, GLfixed height);

static void
_evgl_api_thread_glDrawTexxOES(void *data)
{
   EVGL_API_Thread_Command_glDrawTexxOES *thread_data =
      (EVGL_API_Thread_Command_glDrawTexxOES *)data;

   orig_evgl_api_glDrawTexxOES(thread_data->x,
                               thread_data->y,
                               thread_data->z,
                               thread_data->width,
                               thread_data->height);

}

EAPI void
glDrawTexxOES_evgl_api_thread_cmd(GLfixed x, GLfixed y, GLfixed z, GLfixed width, GLfixed height)
{
   if (!evas_evgl_thread_enabled() || (_main_thread_id != eina_thread_self()))
     {
        orig_evgl_api_glDrawTexxOES(x, y, z, width, height);
        return;
     }

   int thread_mode = EVAS_GL_THREAD_MODE_FINISH;

   EVGL_API_Thread_Command_glDrawTexxOES thread_data_local;
   EVGL_API_Thread_Command_glDrawTexxOES *thread_data = &thread_data_local;

   thread_data->x = x;
   thread_data->y = y;
   thread_data->z = z;
   thread_data->width = width;
   thread_data->height = height;

   evas_gl_thread_cmd_enqueue(EVAS_GL_THREAD_TYPE_EVGL,
                              _evgl_api_thread_glDrawTexxOES,
                              thread_data,
                              thread_mode);
}

/*
 * void
 * glDrawTexsvOES(const GLshort *coords);
 */

typedef struct
{
   const GLshort *coords;

} EVGL_API_Thread_Command_glDrawTexsvOES;

void (*orig_evgl_api_glDrawTexsvOES)(const GLshort *coords);

static void
_evgl_api_thread_glDrawTexsvOES(void *data)
{
   EVGL_API_Thread_Command_glDrawTexsvOES *thread_data =
      (EVGL_API_Thread_Command_glDrawTexsvOES *)data;

   orig_evgl_api_glDrawTexsvOES(thread_data->coords);

}

EAPI void
glDrawTexsvOES_evgl_api_thread_cmd(const GLshort *coords)
{
   if (!evas_evgl_thread_enabled() || (_main_thread_id != eina_thread_self()))
     {
        orig_evgl_api_glDrawTexsvOES(coords);
        return;
     }

   int thread_mode = EVAS_GL_THREAD_MODE_FINISH;

   EVGL_API_Thread_Command_glDrawTexsvOES thread_data_local;
   EVGL_API_Thread_Command_glDrawTexsvOES *thread_data = &thread_data_local;

   thread_data->coords = coords;

   evas_gl_thread_cmd_enqueue(EVAS_GL_THREAD_TYPE_EVGL,
                              _evgl_api_thread_glDrawTexsvOES,
                              thread_data,
                              thread_mode);
}

/*
 * void
 * glDrawTexivOES(const GLint *coords);
 */

typedef struct
{
   const GLint *coords;

} EVGL_API_Thread_Command_glDrawTexivOES;

void (*orig_evgl_api_glDrawTexivOES)(const GLint *coords);

static void
_evgl_api_thread_glDrawTexivOES(void *data)
{
   EVGL_API_Thread_Command_glDrawTexivOES *thread_data =
      (EVGL_API_Thread_Command_glDrawTexivOES *)data;

   orig_evgl_api_glDrawTexivOES(thread_data->coords);

}

EAPI void
glDrawTexivOES_evgl_api_thread_cmd(const GLint *coords)
{
   if (!evas_evgl_thread_enabled() || (_main_thread_id != eina_thread_self()))
     {
        orig_evgl_api_glDrawTexivOES(coords);
        return;
     }

   int thread_mode = EVAS_GL_THREAD_MODE_FINISH;

   EVGL_API_Thread_Command_glDrawTexivOES thread_data_local;
   EVGL_API_Thread_Command_glDrawTexivOES *thread_data = &thread_data_local;

   thread_data->coords = coords;

   evas_gl_thread_cmd_enqueue(EVAS_GL_THREAD_TYPE_EVGL,
                              _evgl_api_thread_glDrawTexivOES,
                              thread_data,
                              thread_mode);
}

/*
 * void
 * glDrawTexxvOES(const GLfixed *coords);
 */

typedef struct
{
   const GLfixed *coords;

} EVGL_API_Thread_Command_glDrawTexxvOES;

void (*orig_evgl_api_glDrawTexxvOES)(const GLfixed *coords);

static void
_evgl_api_thread_glDrawTexxvOES(void *data)
{
   EVGL_API_Thread_Command_glDrawTexxvOES *thread_data =
      (EVGL_API_Thread_Command_glDrawTexxvOES *)data;

   orig_evgl_api_glDrawTexxvOES(thread_data->coords);

}

EAPI void
glDrawTexxvOES_evgl_api_thread_cmd(const GLfixed *coords)
{
   if (!evas_evgl_thread_enabled() || (_main_thread_id != eina_thread_self()))
     {
        orig_evgl_api_glDrawTexxvOES(coords);
        return;
     }

   int thread_mode = EVAS_GL_THREAD_MODE_FINISH;

   EVGL_API_Thread_Command_glDrawTexxvOES thread_data_local;
   EVGL_API_Thread_Command_glDrawTexxvOES *thread_data = &thread_data_local;

   thread_data->coords = coords;

   evas_gl_thread_cmd_enqueue(EVAS_GL_THREAD_TYPE_EVGL,
                              _evgl_api_thread_glDrawTexxvOES,
                              thread_data,
                              thread_mode);
}

/*
 * void
 * glDrawTexfOES(GLfloat x, GLfloat y, GLfloat z, GLfloat width, GLfloat height);
 */

typedef struct
{
   GLfloat x;
   GLfloat y;
   GLfloat z;
   GLfloat width;
   GLfloat height;

} EVGL_API_Thread_Command_glDrawTexfOES;

void (*orig_evgl_api_glDrawTexfOES)(GLfloat x, GLfloat y, GLfloat z, GLfloat width, GLfloat height);

static void
_evgl_api_thread_glDrawTexfOES(void *data)
{
   EVGL_API_Thread_Command_glDrawTexfOES *thread_data =
      (EVGL_API_Thread_Command_glDrawTexfOES *)data;

   orig_evgl_api_glDrawTexfOES(thread_data->x,
                               thread_data->y,
                               thread_data->z,
                               thread_data->width,
                               thread_data->height);

}

EAPI void
glDrawTexfOES_evgl_api_thread_cmd(GLfloat x, GLfloat y, GLfloat z, GLfloat width, GLfloat height)
{
   if (!evas_evgl_thread_enabled() || (_main_thread_id != eina_thread_self()))
     {
        orig_evgl_api_glDrawTexfOES(x, y, z, width, height);
        return;
     }

   int thread_mode = EVAS_GL_THREAD_MODE_FINISH;

   EVGL_API_Thread_Command_glDrawTexfOES thread_data_local;
   EVGL_API_Thread_Command_glDrawTexfOES *thread_data = &thread_data_local;

   thread_data->x = x;
   thread_data->y = y;
   thread_data->z = z;
   thread_data->width = width;
   thread_data->height = height;

   evas_gl_thread_cmd_enqueue(EVAS_GL_THREAD_TYPE_EVGL,
                              _evgl_api_thread_glDrawTexfOES,
                              thread_data,
                              thread_mode);
}

/*
 * void
 * glDrawTexfvOES(const GLfloat *coords);
 */

typedef struct
{
   const GLfloat *coords;

} EVGL_API_Thread_Command_glDrawTexfvOES;

void (*orig_evgl_api_glDrawTexfvOES)(const GLfloat *coords);

static void
_evgl_api_thread_glDrawTexfvOES(void *data)
{
   EVGL_API_Thread_Command_glDrawTexfvOES *thread_data =
      (EVGL_API_Thread_Command_glDrawTexfvOES *)data;

   orig_evgl_api_glDrawTexfvOES(thread_data->coords);

}

EAPI void
glDrawTexfvOES_evgl_api_thread_cmd(const GLfloat *coords)
{
   if (!evas_evgl_thread_enabled() || (_main_thread_id != eina_thread_self()))
     {
        orig_evgl_api_glDrawTexfvOES(coords);
        return;
     }

   int thread_mode = EVAS_GL_THREAD_MODE_FINISH;

   EVGL_API_Thread_Command_glDrawTexfvOES thread_data_local;
   EVGL_API_Thread_Command_glDrawTexfvOES *thread_data = &thread_data_local;

   thread_data->coords = coords;

   evas_gl_thread_cmd_enqueue(EVAS_GL_THREAD_TYPE_EVGL,
                              _evgl_api_thread_glDrawTexfvOES,
                              thread_data,
                              thread_mode);
}

/*
 * void
 * glAlphaFuncxOES(GLenum func, GLclampx ref);
 */

typedef struct
{
   GLenum func;
   GLclampx ref;

} EVGL_API_Thread_Command_glAlphaFuncxOES;

void (*orig_evgl_api_glAlphaFuncxOES)(GLenum func, GLclampx ref);

static void
_evgl_api_thread_glAlphaFuncxOES(void *data)
{
   EVGL_API_Thread_Command_glAlphaFuncxOES *thread_data =
      (EVGL_API_Thread_Command_glAlphaFuncxOES *)data;

   orig_evgl_api_glAlphaFuncxOES(thread_data->func,
                                 thread_data->ref);

}

EAPI void
glAlphaFuncxOES_evgl_api_thread_cmd(GLenum func, GLclampx ref)
{
   if (!evas_evgl_thread_enabled() || (_main_thread_id != eina_thread_self()))
     {
        orig_evgl_api_glAlphaFuncxOES(func, ref);
        return;
     }

   int thread_mode = EVAS_GL_THREAD_MODE_FINISH;

   EVGL_API_Thread_Command_glAlphaFuncxOES thread_data_local;
   EVGL_API_Thread_Command_glAlphaFuncxOES *thread_data = &thread_data_local;

   thread_data->func = func;
   thread_data->ref = ref;

   evas_gl_thread_cmd_enqueue(EVAS_GL_THREAD_TYPE_EVGL,
                              _evgl_api_thread_glAlphaFuncxOES,
                              thread_data,
                              thread_mode);
}

/*
 * void
 * glClearColorxOES(GLclampx red, GLclampx green, GLclampx blue, GLclampx alpha);
 */

typedef struct
{
   GLclampx red;
   GLclampx green;
   GLclampx blue;
   GLclampx alpha;

} EVGL_API_Thread_Command_glClearColorxOES;

void (*orig_evgl_api_glClearColorxOES)(GLclampx red, GLclampx green, GLclampx blue, GLclampx alpha);

static void
_evgl_api_thread_glClearColorxOES(void *data)
{
   EVGL_API_Thread_Command_glClearColorxOES *thread_data =
      (EVGL_API_Thread_Command_glClearColorxOES *)data;

   orig_evgl_api_glClearColorxOES(thread_data->red,
                                  thread_data->green,
                                  thread_data->blue,
                                  thread_data->alpha);

}

EAPI void
glClearColorxOES_evgl_api_thread_cmd(GLclampx red, GLclampx green, GLclampx blue, GLclampx alpha)
{
   if (!evas_evgl_thread_enabled() || (_main_thread_id != eina_thread_self()))
     {
        orig_evgl_api_glClearColorxOES(red, green, blue, alpha);
        return;
     }

   int thread_mode = EVAS_GL_THREAD_MODE_FINISH;

   EVGL_API_Thread_Command_glClearColorxOES thread_data_local;
   EVGL_API_Thread_Command_glClearColorxOES *thread_data = &thread_data_local;

   thread_data->red = red;
   thread_data->green = green;
   thread_data->blue = blue;
   thread_data->alpha = alpha;

   evas_gl_thread_cmd_enqueue(EVAS_GL_THREAD_TYPE_EVGL,
                              _evgl_api_thread_glClearColorxOES,
                              thread_data,
                              thread_mode);
}

/*
 * void
 * glClearDepthxOES(GLclampx depth);
 */

typedef struct
{
   GLclampx depth;

} EVGL_API_Thread_Command_glClearDepthxOES;

void (*orig_evgl_api_glClearDepthxOES)(GLclampx depth);

static void
_evgl_api_thread_glClearDepthxOES(void *data)
{
   EVGL_API_Thread_Command_glClearDepthxOES *thread_data =
      (EVGL_API_Thread_Command_glClearDepthxOES *)data;

   orig_evgl_api_glClearDepthxOES(thread_data->depth);

}

EAPI void
glClearDepthxOES_evgl_api_thread_cmd(GLclampx depth)
{
   if (!evas_evgl_thread_enabled() || (_main_thread_id != eina_thread_self()))
     {
        orig_evgl_api_glClearDepthxOES(depth);
        return;
     }

   int thread_mode = EVAS_GL_THREAD_MODE_FINISH;

   EVGL_API_Thread_Command_glClearDepthxOES thread_data_local;
   EVGL_API_Thread_Command_glClearDepthxOES *thread_data = &thread_data_local;

   thread_data->depth = depth;

   evas_gl_thread_cmd_enqueue(EVAS_GL_THREAD_TYPE_EVGL,
                              _evgl_api_thread_glClearDepthxOES,
                              thread_data,
                              thread_mode);
}

/*
 * void
 * glClipPlanexOES(GLenum plane, const GLfixed *equation);
 */

typedef struct
{
   GLenum plane;
   const GLfixed *equation;

} EVGL_API_Thread_Command_glClipPlanexOES;

void (*orig_evgl_api_glClipPlanexOES)(GLenum plane, const GLfixed *equation);

static void
_evgl_api_thread_glClipPlanexOES(void *data)
{
   EVGL_API_Thread_Command_glClipPlanexOES *thread_data =
      (EVGL_API_Thread_Command_glClipPlanexOES *)data;

   orig_evgl_api_glClipPlanexOES(thread_data->plane,
                                 thread_data->equation);

}

EAPI void
glClipPlanexOES_evgl_api_thread_cmd(GLenum plane, const GLfixed *equation)
{
   if (!evas_evgl_thread_enabled() || (_main_thread_id != eina_thread_self()))
     {
        orig_evgl_api_glClipPlanexOES(plane, equation);
        return;
     }

   int thread_mode = EVAS_GL_THREAD_MODE_FINISH;

   EVGL_API_Thread_Command_glClipPlanexOES thread_data_local;
   EVGL_API_Thread_Command_glClipPlanexOES *thread_data = &thread_data_local;

   thread_data->plane = plane;
   thread_data->equation = equation;

   evas_gl_thread_cmd_enqueue(EVAS_GL_THREAD_TYPE_EVGL,
                              _evgl_api_thread_glClipPlanexOES,
                              thread_data,
                              thread_mode);
}

/*
 * void
 * glColor4xOES(GLfixed red, GLfixed green, GLfixed blue, GLfixed alpha);
 */

typedef struct
{
   GLfixed red;
   GLfixed green;
   GLfixed blue;
   GLfixed alpha;

} EVGL_API_Thread_Command_glColor4xOES;

void (*orig_evgl_api_glColor4xOES)(GLfixed red, GLfixed green, GLfixed blue, GLfixed alpha);

static void
_evgl_api_thread_glColor4xOES(void *data)
{
   EVGL_API_Thread_Command_glColor4xOES *thread_data =
      (EVGL_API_Thread_Command_glColor4xOES *)data;

   orig_evgl_api_glColor4xOES(thread_data->red,
                              thread_data->green,
                              thread_data->blue,
                              thread_data->alpha);

}

EAPI void
glColor4xOES_evgl_api_thread_cmd(GLfixed red, GLfixed green, GLfixed blue, GLfixed alpha)
{
   if (!evas_evgl_thread_enabled() || (_main_thread_id != eina_thread_self()))
     {
        orig_evgl_api_glColor4xOES(red, green, blue, alpha);
        return;
     }

   int thread_mode = EVAS_GL_THREAD_MODE_FINISH;

   EVGL_API_Thread_Command_glColor4xOES thread_data_local;
   EVGL_API_Thread_Command_glColor4xOES *thread_data = &thread_data_local;

   thread_data->red = red;
   thread_data->green = green;
   thread_data->blue = blue;
   thread_data->alpha = alpha;

   evas_gl_thread_cmd_enqueue(EVAS_GL_THREAD_TYPE_EVGL,
                              _evgl_api_thread_glColor4xOES,
                              thread_data,
                              thread_mode);
}

/*
 * void
 * glDepthRangexOES(GLclampx zNear, GLclampx zFar);
 */

typedef struct
{
   GLclampx zNear;
   GLclampx zFar;

} EVGL_API_Thread_Command_glDepthRangexOES;

void (*orig_evgl_api_glDepthRangexOES)(GLclampx zNear, GLclampx zFar);

static void
_evgl_api_thread_glDepthRangexOES(void *data)
{
   EVGL_API_Thread_Command_glDepthRangexOES *thread_data =
      (EVGL_API_Thread_Command_glDepthRangexOES *)data;

   orig_evgl_api_glDepthRangexOES(thread_data->zNear,
                                  thread_data->zFar);

}

EAPI void
glDepthRangexOES_evgl_api_thread_cmd(GLclampx zNear, GLclampx zFar)
{
   if (!evas_evgl_thread_enabled() || (_main_thread_id != eina_thread_self()))
     {
        orig_evgl_api_glDepthRangexOES(zNear, zFar);
        return;
     }

   int thread_mode = EVAS_GL_THREAD_MODE_FINISH;

   EVGL_API_Thread_Command_glDepthRangexOES thread_data_local;
   EVGL_API_Thread_Command_glDepthRangexOES *thread_data = &thread_data_local;

   thread_data->zNear = zNear;
   thread_data->zFar = zFar;

   evas_gl_thread_cmd_enqueue(EVAS_GL_THREAD_TYPE_EVGL,
                              _evgl_api_thread_glDepthRangexOES,
                              thread_data,
                              thread_mode);
}

/*
 * void
 * glFogxOES(GLenum pname, GLfixed param);
 */

typedef struct
{
   GLenum pname;
   GLfixed param;

} EVGL_API_Thread_Command_glFogxOES;

void (*orig_evgl_api_glFogxOES)(GLenum pname, GLfixed param);

static void
_evgl_api_thread_glFogxOES(void *data)
{
   EVGL_API_Thread_Command_glFogxOES *thread_data =
      (EVGL_API_Thread_Command_glFogxOES *)data;

   orig_evgl_api_glFogxOES(thread_data->pname,
                           thread_data->param);

}

EAPI void
glFogxOES_evgl_api_thread_cmd(GLenum pname, GLfixed param)
{
   if (!evas_evgl_thread_enabled() || (_main_thread_id != eina_thread_self()))
     {
        orig_evgl_api_glFogxOES(pname, param);
        return;
     }

   int thread_mode = EVAS_GL_THREAD_MODE_FINISH;

   EVGL_API_Thread_Command_glFogxOES thread_data_local;
   EVGL_API_Thread_Command_glFogxOES *thread_data = &thread_data_local;

   thread_data->pname = pname;
   thread_data->param = param;

   evas_gl_thread_cmd_enqueue(EVAS_GL_THREAD_TYPE_EVGL,
                              _evgl_api_thread_glFogxOES,
                              thread_data,
                              thread_mode);
}

/*
 * void
 * glFogxvOES(GLenum pname, const GLfixed *params);
 */

typedef struct
{
   GLenum pname;
   const GLfixed *params;

} EVGL_API_Thread_Command_glFogxvOES;

void (*orig_evgl_api_glFogxvOES)(GLenum pname, const GLfixed *params);

static void
_evgl_api_thread_glFogxvOES(void *data)
{
   EVGL_API_Thread_Command_glFogxvOES *thread_data =
      (EVGL_API_Thread_Command_glFogxvOES *)data;

   orig_evgl_api_glFogxvOES(thread_data->pname,
                            thread_data->params);

}

EAPI void
glFogxvOES_evgl_api_thread_cmd(GLenum pname, const GLfixed *params)
{
   if (!evas_evgl_thread_enabled() || (_main_thread_id != eina_thread_self()))
     {
        orig_evgl_api_glFogxvOES(pname, params);
        return;
     }

   int thread_mode = EVAS_GL_THREAD_MODE_FINISH;

   EVGL_API_Thread_Command_glFogxvOES thread_data_local;
   EVGL_API_Thread_Command_glFogxvOES *thread_data = &thread_data_local;

   thread_data->pname = pname;
   thread_data->params = params;

   evas_gl_thread_cmd_enqueue(EVAS_GL_THREAD_TYPE_EVGL,
                              _evgl_api_thread_glFogxvOES,
                              thread_data,
                              thread_mode);
}

/*
 * void
 * glFrustumxOES(GLfixed left, GLfixed right, GLfixed bottom, GLfixed top, GLfixed zNear, GLfixed zFar);
 */

typedef struct
{
   GLfixed left;
   GLfixed right;
   GLfixed bottom;
   GLfixed top;
   GLfixed zNear;
   GLfixed zFar;

} EVGL_API_Thread_Command_glFrustumxOES;

void (*orig_evgl_api_glFrustumxOES)(GLfixed left, GLfixed right, GLfixed bottom, GLfixed top, GLfixed zNear, GLfixed zFar);

static void
_evgl_api_thread_glFrustumxOES(void *data)
{
   EVGL_API_Thread_Command_glFrustumxOES *thread_data =
      (EVGL_API_Thread_Command_glFrustumxOES *)data;

   orig_evgl_api_glFrustumxOES(thread_data->left,
                               thread_data->right,
                               thread_data->bottom,
                               thread_data->top,
                               thread_data->zNear,
                               thread_data->zFar);

}

EAPI void
glFrustumxOES_evgl_api_thread_cmd(GLfixed left, GLfixed right, GLfixed bottom, GLfixed top, GLfixed zNear, GLfixed zFar)
{
   if (!evas_evgl_thread_enabled() || (_main_thread_id != eina_thread_self()))
     {
        orig_evgl_api_glFrustumxOES(left, right, bottom, top, zNear, zFar);
        return;
     }

   int thread_mode = EVAS_GL_THREAD_MODE_FINISH;

   EVGL_API_Thread_Command_glFrustumxOES thread_data_local;
   EVGL_API_Thread_Command_glFrustumxOES *thread_data = &thread_data_local;

   thread_data->left = left;
   thread_data->right = right;
   thread_data->bottom = bottom;
   thread_data->top = top;
   thread_data->zNear = zNear;
   thread_data->zFar = zFar;

   evas_gl_thread_cmd_enqueue(EVAS_GL_THREAD_TYPE_EVGL,
                              _evgl_api_thread_glFrustumxOES,
                              thread_data,
                              thread_mode);
}

/*
 * void
 * glGetClipPlanexOES(GLenum pname, GLfixed eqn[4]);
 */

typedef struct
{
   GLenum pname;
   GLfixed eqn[4];

} EVGL_API_Thread_Command_glGetClipPlanexOES;

void (*orig_evgl_api_glGetClipPlanexOES)(GLenum pname, GLfixed eqn[4]);

static void
_evgl_api_thread_glGetClipPlanexOES(void *data)
{
   EVGL_API_Thread_Command_glGetClipPlanexOES *thread_data =
      (EVGL_API_Thread_Command_glGetClipPlanexOES *)data;

   orig_evgl_api_glGetClipPlanexOES(thread_data->pname,
                                    thread_data->eqn);

}

EAPI void
glGetClipPlanexOES_evgl_api_thread_cmd(GLenum pname, GLfixed eqn[4])
{
   if (!evas_evgl_thread_enabled() || (_main_thread_id != eina_thread_self()))
     {
        orig_evgl_api_glGetClipPlanexOES(pname, eqn);
        return;
     }

   int thread_mode = EVAS_GL_THREAD_MODE_FINISH;

   EVGL_API_Thread_Command_glGetClipPlanexOES thread_data_local;
   EVGL_API_Thread_Command_glGetClipPlanexOES *thread_data = &thread_data_local;

   thread_data->pname = pname;
   memcpy(thread_data->eqn, &eqn, sizeof(GLfixed) * 4);

   evas_gl_thread_cmd_enqueue(EVAS_GL_THREAD_TYPE_EVGL,
                              _evgl_api_thread_glGetClipPlanexOES,
                              thread_data,
                              thread_mode);
}

/*
 * void
 * glGetFixedvOES(GLenum pname, GLfixed *params);
 */

typedef struct
{
   GLenum pname;
   GLfixed *params;

} EVGL_API_Thread_Command_glGetFixedvOES;

void (*orig_evgl_api_glGetFixedvOES)(GLenum pname, GLfixed *params);

static void
_evgl_api_thread_glGetFixedvOES(void *data)
{
   EVGL_API_Thread_Command_glGetFixedvOES *thread_data =
      (EVGL_API_Thread_Command_glGetFixedvOES *)data;

   orig_evgl_api_glGetFixedvOES(thread_data->pname,
                                thread_data->params);

}

EAPI void
glGetFixedvOES_evgl_api_thread_cmd(GLenum pname, GLfixed *params)
{
   if (!evas_evgl_thread_enabled() || (_main_thread_id != eina_thread_self()))
     {
        orig_evgl_api_glGetFixedvOES(pname, params);
        return;
     }

   int thread_mode = EVAS_GL_THREAD_MODE_FINISH;

   EVGL_API_Thread_Command_glGetFixedvOES thread_data_local;
   EVGL_API_Thread_Command_glGetFixedvOES *thread_data = &thread_data_local;

   thread_data->pname = pname;
   thread_data->params = params;

   evas_gl_thread_cmd_enqueue(EVAS_GL_THREAD_TYPE_EVGL,
                              _evgl_api_thread_glGetFixedvOES,
                              thread_data,
                              thread_mode);
}

/*
 * void
 * glGetLightxvOES(GLenum light, GLenum pname, GLfixed *params);
 */

typedef struct
{
   GLenum light;
   GLenum pname;
   GLfixed *params;

} EVGL_API_Thread_Command_glGetLightxvOES;

void (*orig_evgl_api_glGetLightxvOES)(GLenum light, GLenum pname, GLfixed *params);

static void
_evgl_api_thread_glGetLightxvOES(void *data)
{
   EVGL_API_Thread_Command_glGetLightxvOES *thread_data =
      (EVGL_API_Thread_Command_glGetLightxvOES *)data;

   orig_evgl_api_glGetLightxvOES(thread_data->light,
                                 thread_data->pname,
                                 thread_data->params);

}

EAPI void
glGetLightxvOES_evgl_api_thread_cmd(GLenum light, GLenum pname, GLfixed *params)
{
   if (!evas_evgl_thread_enabled() || (_main_thread_id != eina_thread_self()))
     {
        orig_evgl_api_glGetLightxvOES(light, pname, params);
        return;
     }

   int thread_mode = EVAS_GL_THREAD_MODE_FINISH;

   EVGL_API_Thread_Command_glGetLightxvOES thread_data_local;
   EVGL_API_Thread_Command_glGetLightxvOES *thread_data = &thread_data_local;

   thread_data->light = light;
   thread_data->pname = pname;
   thread_data->params = params;

   evas_gl_thread_cmd_enqueue(EVAS_GL_THREAD_TYPE_EVGL,
                              _evgl_api_thread_glGetLightxvOES,
                              thread_data,
                              thread_mode);
}

/*
 * void
 * glGetMaterialxvOES(GLenum face, GLenum pname, GLfixed *params);
 */

typedef struct
{
   GLenum face;
   GLenum pname;
   GLfixed *params;

} EVGL_API_Thread_Command_glGetMaterialxvOES;

void (*orig_evgl_api_glGetMaterialxvOES)(GLenum face, GLenum pname, GLfixed *params);

static void
_evgl_api_thread_glGetMaterialxvOES(void *data)
{
   EVGL_API_Thread_Command_glGetMaterialxvOES *thread_data =
      (EVGL_API_Thread_Command_glGetMaterialxvOES *)data;

   orig_evgl_api_glGetMaterialxvOES(thread_data->face,
                                    thread_data->pname,
                                    thread_data->params);

}

EAPI void
glGetMaterialxvOES_evgl_api_thread_cmd(GLenum face, GLenum pname, GLfixed *params)
{
   if (!evas_evgl_thread_enabled() || (_main_thread_id != eina_thread_self()))
     {
        orig_evgl_api_glGetMaterialxvOES(face, pname, params);
        return;
     }

   int thread_mode = EVAS_GL_THREAD_MODE_FINISH;

   EVGL_API_Thread_Command_glGetMaterialxvOES thread_data_local;
   EVGL_API_Thread_Command_glGetMaterialxvOES *thread_data = &thread_data_local;

   thread_data->face = face;
   thread_data->pname = pname;
   thread_data->params = params;

   evas_gl_thread_cmd_enqueue(EVAS_GL_THREAD_TYPE_EVGL,
                              _evgl_api_thread_glGetMaterialxvOES,
                              thread_data,
                              thread_mode);
}

/*
 * void
 * glGetTexEnvxvOES(GLenum env, GLenum pname, GLfixed *params);
 */

typedef struct
{
   GLenum env;
   GLenum pname;
   GLfixed *params;

} EVGL_API_Thread_Command_glGetTexEnvxvOES;

void (*orig_evgl_api_glGetTexEnvxvOES)(GLenum env, GLenum pname, GLfixed *params);

static void
_evgl_api_thread_glGetTexEnvxvOES(void *data)
{
   EVGL_API_Thread_Command_glGetTexEnvxvOES *thread_data =
      (EVGL_API_Thread_Command_glGetTexEnvxvOES *)data;

   orig_evgl_api_glGetTexEnvxvOES(thread_data->env,
                                  thread_data->pname,
                                  thread_data->params);

}

EAPI void
glGetTexEnvxvOES_evgl_api_thread_cmd(GLenum env, GLenum pname, GLfixed *params)
{
   if (!evas_evgl_thread_enabled() || (_main_thread_id != eina_thread_self()))
     {
        orig_evgl_api_glGetTexEnvxvOES(env, pname, params);
        return;
     }

   int thread_mode = EVAS_GL_THREAD_MODE_FINISH;

   EVGL_API_Thread_Command_glGetTexEnvxvOES thread_data_local;
   EVGL_API_Thread_Command_glGetTexEnvxvOES *thread_data = &thread_data_local;

   thread_data->env = env;
   thread_data->pname = pname;
   thread_data->params = params;

   evas_gl_thread_cmd_enqueue(EVAS_GL_THREAD_TYPE_EVGL,
                              _evgl_api_thread_glGetTexEnvxvOES,
                              thread_data,
                              thread_mode);
}

/*
 * void
 * glGetTexParameterxvOES(GLenum target, GLenum pname, GLfixed *params);
 */

typedef struct
{
   GLenum target;
   GLenum pname;
   GLfixed *params;

} EVGL_API_Thread_Command_glGetTexParameterxvOES;

void (*orig_evgl_api_glGetTexParameterxvOES)(GLenum target, GLenum pname, GLfixed *params);

static void
_evgl_api_thread_glGetTexParameterxvOES(void *data)
{
   EVGL_API_Thread_Command_glGetTexParameterxvOES *thread_data =
      (EVGL_API_Thread_Command_glGetTexParameterxvOES *)data;

   orig_evgl_api_glGetTexParameterxvOES(thread_data->target,
                                        thread_data->pname,
                                        thread_data->params);

}

EAPI void
glGetTexParameterxvOES_evgl_api_thread_cmd(GLenum target, GLenum pname, GLfixed *params)
{
   if (!evas_evgl_thread_enabled() || (_main_thread_id != eina_thread_self()))
     {
        orig_evgl_api_glGetTexParameterxvOES(target, pname, params);
        return;
     }

   int thread_mode = EVAS_GL_THREAD_MODE_FINISH;

   EVGL_API_Thread_Command_glGetTexParameterxvOES thread_data_local;
   EVGL_API_Thread_Command_glGetTexParameterxvOES *thread_data = &thread_data_local;

   thread_data->target = target;
   thread_data->pname = pname;
   thread_data->params = params;

   evas_gl_thread_cmd_enqueue(EVAS_GL_THREAD_TYPE_EVGL,
                              _evgl_api_thread_glGetTexParameterxvOES,
                              thread_data,
                              thread_mode);
}

/*
 * void
 * glLightModelxOES(GLenum pname, GLfixed param);
 */

typedef struct
{
   GLenum pname;
   GLfixed param;

} EVGL_API_Thread_Command_glLightModelxOES;

void (*orig_evgl_api_glLightModelxOES)(GLenum pname, GLfixed param);

static void
_evgl_api_thread_glLightModelxOES(void *data)
{
   EVGL_API_Thread_Command_glLightModelxOES *thread_data =
      (EVGL_API_Thread_Command_glLightModelxOES *)data;

   orig_evgl_api_glLightModelxOES(thread_data->pname,
                                  thread_data->param);

}

EAPI void
glLightModelxOES_evgl_api_thread_cmd(GLenum pname, GLfixed param)
{
   if (!evas_evgl_thread_enabled() || (_main_thread_id != eina_thread_self()))
     {
        orig_evgl_api_glLightModelxOES(pname, param);
        return;
     }

   int thread_mode = EVAS_GL_THREAD_MODE_FINISH;

   EVGL_API_Thread_Command_glLightModelxOES thread_data_local;
   EVGL_API_Thread_Command_glLightModelxOES *thread_data = &thread_data_local;

   thread_data->pname = pname;
   thread_data->param = param;

   evas_gl_thread_cmd_enqueue(EVAS_GL_THREAD_TYPE_EVGL,
                              _evgl_api_thread_glLightModelxOES,
                              thread_data,
                              thread_mode);
}

/*
 * void
 * glLightModelxvOES(GLenum pname, const GLfixed *params);
 */

typedef struct
{
   GLenum pname;
   const GLfixed *params;

} EVGL_API_Thread_Command_glLightModelxvOES;

void (*orig_evgl_api_glLightModelxvOES)(GLenum pname, const GLfixed *params);

static void
_evgl_api_thread_glLightModelxvOES(void *data)
{
   EVGL_API_Thread_Command_glLightModelxvOES *thread_data =
      (EVGL_API_Thread_Command_glLightModelxvOES *)data;

   orig_evgl_api_glLightModelxvOES(thread_data->pname,
                                   thread_data->params);

}

EAPI void
glLightModelxvOES_evgl_api_thread_cmd(GLenum pname, const GLfixed *params)
{
   if (!evas_evgl_thread_enabled() || (_main_thread_id != eina_thread_self()))
     {
        orig_evgl_api_glLightModelxvOES(pname, params);
        return;
     }

   int thread_mode = EVAS_GL_THREAD_MODE_FINISH;

   EVGL_API_Thread_Command_glLightModelxvOES thread_data_local;
   EVGL_API_Thread_Command_glLightModelxvOES *thread_data = &thread_data_local;

   thread_data->pname = pname;
   thread_data->params = params;

   evas_gl_thread_cmd_enqueue(EVAS_GL_THREAD_TYPE_EVGL,
                              _evgl_api_thread_glLightModelxvOES,
                              thread_data,
                              thread_mode);
}

/*
 * void
 * glLightxOES(GLenum light, GLenum pname, GLfixed param);
 */

typedef struct
{
   GLenum light;
   GLenum pname;
   GLfixed param;

} EVGL_API_Thread_Command_glLightxOES;

void (*orig_evgl_api_glLightxOES)(GLenum light, GLenum pname, GLfixed param);

static void
_evgl_api_thread_glLightxOES(void *data)
{
   EVGL_API_Thread_Command_glLightxOES *thread_data =
      (EVGL_API_Thread_Command_glLightxOES *)data;

   orig_evgl_api_glLightxOES(thread_data->light,
                             thread_data->pname,
                             thread_data->param);

}

EAPI void
glLightxOES_evgl_api_thread_cmd(GLenum light, GLenum pname, GLfixed param)
{
   if (!evas_evgl_thread_enabled() || (_main_thread_id != eina_thread_self()))
     {
        orig_evgl_api_glLightxOES(light, pname, param);
        return;
     }

   int thread_mode = EVAS_GL_THREAD_MODE_FINISH;

   EVGL_API_Thread_Command_glLightxOES thread_data_local;
   EVGL_API_Thread_Command_glLightxOES *thread_data = &thread_data_local;

   thread_data->light = light;
   thread_data->pname = pname;
   thread_data->param = param;

   evas_gl_thread_cmd_enqueue(EVAS_GL_THREAD_TYPE_EVGL,
                              _evgl_api_thread_glLightxOES,
                              thread_data,
                              thread_mode);
}

/*
 * void
 * glLightxvOES(GLenum light, GLenum pname, const GLfixed *params);
 */

typedef struct
{
   GLenum light;
   GLenum pname;
   const GLfixed *params;

} EVGL_API_Thread_Command_glLightxvOES;

void (*orig_evgl_api_glLightxvOES)(GLenum light, GLenum pname, const GLfixed *params);

static void
_evgl_api_thread_glLightxvOES(void *data)
{
   EVGL_API_Thread_Command_glLightxvOES *thread_data =
      (EVGL_API_Thread_Command_glLightxvOES *)data;

   orig_evgl_api_glLightxvOES(thread_data->light,
                              thread_data->pname,
                              thread_data->params);

}

EAPI void
glLightxvOES_evgl_api_thread_cmd(GLenum light, GLenum pname, const GLfixed *params)
{
   if (!evas_evgl_thread_enabled() || (_main_thread_id != eina_thread_self()))
     {
        orig_evgl_api_glLightxvOES(light, pname, params);
        return;
     }

   int thread_mode = EVAS_GL_THREAD_MODE_FINISH;

   EVGL_API_Thread_Command_glLightxvOES thread_data_local;
   EVGL_API_Thread_Command_glLightxvOES *thread_data = &thread_data_local;

   thread_data->light = light;
   thread_data->pname = pname;
   thread_data->params = params;

   evas_gl_thread_cmd_enqueue(EVAS_GL_THREAD_TYPE_EVGL,
                              _evgl_api_thread_glLightxvOES,
                              thread_data,
                              thread_mode);
}

/*
 * void
 * glLineWidthxOES(GLfixed width);
 */

typedef struct
{
   GLfixed width;

} EVGL_API_Thread_Command_glLineWidthxOES;

void (*orig_evgl_api_glLineWidthxOES)(GLfixed width);

static void
_evgl_api_thread_glLineWidthxOES(void *data)
{
   EVGL_API_Thread_Command_glLineWidthxOES *thread_data =
      (EVGL_API_Thread_Command_glLineWidthxOES *)data;

   orig_evgl_api_glLineWidthxOES(thread_data->width);

}

EAPI void
glLineWidthxOES_evgl_api_thread_cmd(GLfixed width)
{
   if (!evas_evgl_thread_enabled() || (_main_thread_id != eina_thread_self()))
     {
        orig_evgl_api_glLineWidthxOES(width);
        return;
     }

   int thread_mode = EVAS_GL_THREAD_MODE_FINISH;

   EVGL_API_Thread_Command_glLineWidthxOES thread_data_local;
   EVGL_API_Thread_Command_glLineWidthxOES *thread_data = &thread_data_local;

   thread_data->width = width;

   evas_gl_thread_cmd_enqueue(EVAS_GL_THREAD_TYPE_EVGL,
                              _evgl_api_thread_glLineWidthxOES,
                              thread_data,
                              thread_mode);
}

/*
 * void
 * glLoadMatrixxOES(const GLfixed *m);
 */

typedef struct
{
   const GLfixed *m;

} EVGL_API_Thread_Command_glLoadMatrixxOES;

void (*orig_evgl_api_glLoadMatrixxOES)(const GLfixed *m);

static void
_evgl_api_thread_glLoadMatrixxOES(void *data)
{
   EVGL_API_Thread_Command_glLoadMatrixxOES *thread_data =
      (EVGL_API_Thread_Command_glLoadMatrixxOES *)data;

   orig_evgl_api_glLoadMatrixxOES(thread_data->m);

}

EAPI void
glLoadMatrixxOES_evgl_api_thread_cmd(const GLfixed *m)
{
   if (!evas_evgl_thread_enabled() || (_main_thread_id != eina_thread_self()))
     {
        orig_evgl_api_glLoadMatrixxOES(m);
        return;
     }

   int thread_mode = EVAS_GL_THREAD_MODE_FINISH;

   EVGL_API_Thread_Command_glLoadMatrixxOES thread_data_local;
   EVGL_API_Thread_Command_glLoadMatrixxOES *thread_data = &thread_data_local;

   thread_data->m = m;

   evas_gl_thread_cmd_enqueue(EVAS_GL_THREAD_TYPE_EVGL,
                              _evgl_api_thread_glLoadMatrixxOES,
                              thread_data,
                              thread_mode);
}

/*
 * void
 * glMaterialxOES(GLenum face, GLenum pname, GLfixed param);
 */

typedef struct
{
   GLenum face;
   GLenum pname;
   GLfixed param;

} EVGL_API_Thread_Command_glMaterialxOES;

void (*orig_evgl_api_glMaterialxOES)(GLenum face, GLenum pname, GLfixed param);

static void
_evgl_api_thread_glMaterialxOES(void *data)
{
   EVGL_API_Thread_Command_glMaterialxOES *thread_data =
      (EVGL_API_Thread_Command_glMaterialxOES *)data;

   orig_evgl_api_glMaterialxOES(thread_data->face,
                                thread_data->pname,
                                thread_data->param);

}

EAPI void
glMaterialxOES_evgl_api_thread_cmd(GLenum face, GLenum pname, GLfixed param)
{
   if (!evas_evgl_thread_enabled() || (_main_thread_id != eina_thread_self()))
     {
        orig_evgl_api_glMaterialxOES(face, pname, param);
        return;
     }

   int thread_mode = EVAS_GL_THREAD_MODE_FINISH;

   EVGL_API_Thread_Command_glMaterialxOES thread_data_local;
   EVGL_API_Thread_Command_glMaterialxOES *thread_data = &thread_data_local;

   thread_data->face = face;
   thread_data->pname = pname;
   thread_data->param = param;

   evas_gl_thread_cmd_enqueue(EVAS_GL_THREAD_TYPE_EVGL,
                              _evgl_api_thread_glMaterialxOES,
                              thread_data,
                              thread_mode);
}

/*
 * void
 * glMaterialxvOES(GLenum face, GLenum pname, const GLfixed *params);
 */

typedef struct
{
   GLenum face;
   GLenum pname;
   const GLfixed *params;

} EVGL_API_Thread_Command_glMaterialxvOES;

void (*orig_evgl_api_glMaterialxvOES)(GLenum face, GLenum pname, const GLfixed *params);

static void
_evgl_api_thread_glMaterialxvOES(void *data)
{
   EVGL_API_Thread_Command_glMaterialxvOES *thread_data =
      (EVGL_API_Thread_Command_glMaterialxvOES *)data;

   orig_evgl_api_glMaterialxvOES(thread_data->face,
                                 thread_data->pname,
                                 thread_data->params);

}

EAPI void
glMaterialxvOES_evgl_api_thread_cmd(GLenum face, GLenum pname, const GLfixed *params)
{
   if (!evas_evgl_thread_enabled() || (_main_thread_id != eina_thread_self()))
     {
        orig_evgl_api_glMaterialxvOES(face, pname, params);
        return;
     }

   int thread_mode = EVAS_GL_THREAD_MODE_FINISH;

   EVGL_API_Thread_Command_glMaterialxvOES thread_data_local;
   EVGL_API_Thread_Command_glMaterialxvOES *thread_data = &thread_data_local;

   thread_data->face = face;
   thread_data->pname = pname;
   thread_data->params = params;

   evas_gl_thread_cmd_enqueue(EVAS_GL_THREAD_TYPE_EVGL,
                              _evgl_api_thread_glMaterialxvOES,
                              thread_data,
                              thread_mode);
}

/*
 * void
 * glMultMatrixxOES(const GLfixed *m);
 */

typedef struct
{
   const GLfixed *m;

} EVGL_API_Thread_Command_glMultMatrixxOES;

void (*orig_evgl_api_glMultMatrixxOES)(const GLfixed *m);

static void
_evgl_api_thread_glMultMatrixxOES(void *data)
{
   EVGL_API_Thread_Command_glMultMatrixxOES *thread_data =
      (EVGL_API_Thread_Command_glMultMatrixxOES *)data;

   orig_evgl_api_glMultMatrixxOES(thread_data->m);

}

EAPI void
glMultMatrixxOES_evgl_api_thread_cmd(const GLfixed *m)
{
   if (!evas_evgl_thread_enabled() || (_main_thread_id != eina_thread_self()))
     {
        orig_evgl_api_glMultMatrixxOES(m);
        return;
     }

   int thread_mode = EVAS_GL_THREAD_MODE_FINISH;

   EVGL_API_Thread_Command_glMultMatrixxOES thread_data_local;
   EVGL_API_Thread_Command_glMultMatrixxOES *thread_data = &thread_data_local;

   thread_data->m = m;

   evas_gl_thread_cmd_enqueue(EVAS_GL_THREAD_TYPE_EVGL,
                              _evgl_api_thread_glMultMatrixxOES,
                              thread_data,
                              thread_mode);
}

/*
 * void
 * glMultiTexCoord4xOES(GLenum target, GLfixed s, GLfixed t, GLfixed r, GLfixed q);
 */

typedef struct
{
   GLenum target;
   GLfixed s;
   GLfixed t;
   GLfixed r;
   GLfixed q;

} EVGL_API_Thread_Command_glMultiTexCoord4xOES;

void (*orig_evgl_api_glMultiTexCoord4xOES)(GLenum target, GLfixed s, GLfixed t, GLfixed r, GLfixed q);

static void
_evgl_api_thread_glMultiTexCoord4xOES(void *data)
{
   EVGL_API_Thread_Command_glMultiTexCoord4xOES *thread_data =
      (EVGL_API_Thread_Command_glMultiTexCoord4xOES *)data;

   orig_evgl_api_glMultiTexCoord4xOES(thread_data->target,
                                      thread_data->s,
                                      thread_data->t,
                                      thread_data->r,
                                      thread_data->q);

}

EAPI void
glMultiTexCoord4xOES_evgl_api_thread_cmd(GLenum target, GLfixed s, GLfixed t, GLfixed r, GLfixed q)
{
   if (!evas_evgl_thread_enabled() || (_main_thread_id != eina_thread_self()))
     {
        orig_evgl_api_glMultiTexCoord4xOES(target, s, t, r, q);
        return;
     }

   int thread_mode = EVAS_GL_THREAD_MODE_FINISH;

   EVGL_API_Thread_Command_glMultiTexCoord4xOES thread_data_local;
   EVGL_API_Thread_Command_glMultiTexCoord4xOES *thread_data = &thread_data_local;

   thread_data->target = target;
   thread_data->s = s;
   thread_data->t = t;
   thread_data->r = r;
   thread_data->q = q;

   evas_gl_thread_cmd_enqueue(EVAS_GL_THREAD_TYPE_EVGL,
                              _evgl_api_thread_glMultiTexCoord4xOES,
                              thread_data,
                              thread_mode);
}

/*
 * void
 * glNormal3xOES(GLfixed nx, GLfixed ny, GLfixed nz);
 */

typedef struct
{
   GLfixed nx;
   GLfixed ny;
   GLfixed nz;

} EVGL_API_Thread_Command_glNormal3xOES;

void (*orig_evgl_api_glNormal3xOES)(GLfixed nx, GLfixed ny, GLfixed nz);

static void
_evgl_api_thread_glNormal3xOES(void *data)
{
   EVGL_API_Thread_Command_glNormal3xOES *thread_data =
      (EVGL_API_Thread_Command_glNormal3xOES *)data;

   orig_evgl_api_glNormal3xOES(thread_data->nx,
                               thread_data->ny,
                               thread_data->nz);

}

EAPI void
glNormal3xOES_evgl_api_thread_cmd(GLfixed nx, GLfixed ny, GLfixed nz)
{
   if (!evas_evgl_thread_enabled() || (_main_thread_id != eina_thread_self()))
     {
        orig_evgl_api_glNormal3xOES(nx, ny, nz);
        return;
     }

   int thread_mode = EVAS_GL_THREAD_MODE_FINISH;

   EVGL_API_Thread_Command_glNormal3xOES thread_data_local;
   EVGL_API_Thread_Command_glNormal3xOES *thread_data = &thread_data_local;

   thread_data->nx = nx;
   thread_data->ny = ny;
   thread_data->nz = nz;

   evas_gl_thread_cmd_enqueue(EVAS_GL_THREAD_TYPE_EVGL,
                              _evgl_api_thread_glNormal3xOES,
                              thread_data,
                              thread_mode);
}

/*
 * void
 * glOrthoxOES(GLfixed left, GLfixed right, GLfixed bottom, GLfixed top, GLfixed zNear, GLfixed zFar);
 */

typedef struct
{
   GLfixed left;
   GLfixed right;
   GLfixed bottom;
   GLfixed top;
   GLfixed zNear;
   GLfixed zFar;

} EVGL_API_Thread_Command_glOrthoxOES;

void (*orig_evgl_api_glOrthoxOES)(GLfixed left, GLfixed right, GLfixed bottom, GLfixed top, GLfixed zNear, GLfixed zFar);

static void
_evgl_api_thread_glOrthoxOES(void *data)
{
   EVGL_API_Thread_Command_glOrthoxOES *thread_data =
      (EVGL_API_Thread_Command_glOrthoxOES *)data;

   orig_evgl_api_glOrthoxOES(thread_data->left,
                             thread_data->right,
                             thread_data->bottom,
                             thread_data->top,
                             thread_data->zNear,
                             thread_data->zFar);

}

EAPI void
glOrthoxOES_evgl_api_thread_cmd(GLfixed left, GLfixed right, GLfixed bottom, GLfixed top, GLfixed zNear, GLfixed zFar)
{
   if (!evas_evgl_thread_enabled() || (_main_thread_id != eina_thread_self()))
     {
        orig_evgl_api_glOrthoxOES(left, right, bottom, top, zNear, zFar);
        return;
     }

   int thread_mode = EVAS_GL_THREAD_MODE_FINISH;

   EVGL_API_Thread_Command_glOrthoxOES thread_data_local;
   EVGL_API_Thread_Command_glOrthoxOES *thread_data = &thread_data_local;

   thread_data->left = left;
   thread_data->right = right;
   thread_data->bottom = bottom;
   thread_data->top = top;
   thread_data->zNear = zNear;
   thread_data->zFar = zFar;

   evas_gl_thread_cmd_enqueue(EVAS_GL_THREAD_TYPE_EVGL,
                              _evgl_api_thread_glOrthoxOES,
                              thread_data,
                              thread_mode);
}

/*
 * void
 * glPointParameterxOES(GLenum pname, GLfixed param);
 */

typedef struct
{
   GLenum pname;
   GLfixed param;

} EVGL_API_Thread_Command_glPointParameterxOES;

void (*orig_evgl_api_glPointParameterxOES)(GLenum pname, GLfixed param);

static void
_evgl_api_thread_glPointParameterxOES(void *data)
{
   EVGL_API_Thread_Command_glPointParameterxOES *thread_data =
      (EVGL_API_Thread_Command_glPointParameterxOES *)data;

   orig_evgl_api_glPointParameterxOES(thread_data->pname,
                                      thread_data->param);

}

EAPI void
glPointParameterxOES_evgl_api_thread_cmd(GLenum pname, GLfixed param)
{
   if (!evas_evgl_thread_enabled() || (_main_thread_id != eina_thread_self()))
     {
        orig_evgl_api_glPointParameterxOES(pname, param);
        return;
     }

   int thread_mode = EVAS_GL_THREAD_MODE_FINISH;

   EVGL_API_Thread_Command_glPointParameterxOES thread_data_local;
   EVGL_API_Thread_Command_glPointParameterxOES *thread_data = &thread_data_local;

   thread_data->pname = pname;
   thread_data->param = param;

   evas_gl_thread_cmd_enqueue(EVAS_GL_THREAD_TYPE_EVGL,
                              _evgl_api_thread_glPointParameterxOES,
                              thread_data,
                              thread_mode);
}

/*
 * void
 * glPointParameterxvOES(GLenum pname, const GLfixed *params);
 */

typedef struct
{
   GLenum pname;
   const GLfixed *params;

} EVGL_API_Thread_Command_glPointParameterxvOES;

void (*orig_evgl_api_glPointParameterxvOES)(GLenum pname, const GLfixed *params);

static void
_evgl_api_thread_glPointParameterxvOES(void *data)
{
   EVGL_API_Thread_Command_glPointParameterxvOES *thread_data =
      (EVGL_API_Thread_Command_glPointParameterxvOES *)data;

   orig_evgl_api_glPointParameterxvOES(thread_data->pname,
                                       thread_data->params);

}

EAPI void
glPointParameterxvOES_evgl_api_thread_cmd(GLenum pname, const GLfixed *params)
{
   if (!evas_evgl_thread_enabled() || (_main_thread_id != eina_thread_self()))
     {
        orig_evgl_api_glPointParameterxvOES(pname, params);
        return;
     }

   int thread_mode = EVAS_GL_THREAD_MODE_FINISH;

   EVGL_API_Thread_Command_glPointParameterxvOES thread_data_local;
   EVGL_API_Thread_Command_glPointParameterxvOES *thread_data = &thread_data_local;

   thread_data->pname = pname;
   thread_data->params = params;

   evas_gl_thread_cmd_enqueue(EVAS_GL_THREAD_TYPE_EVGL,
                              _evgl_api_thread_glPointParameterxvOES,
                              thread_data,
                              thread_mode);
}

/*
 * void
 * glPointSizexOES(GLfixed size);
 */

typedef struct
{
   GLfixed size;

} EVGL_API_Thread_Command_glPointSizexOES;

void (*orig_evgl_api_glPointSizexOES)(GLfixed size);

static void
_evgl_api_thread_glPointSizexOES(void *data)
{
   EVGL_API_Thread_Command_glPointSizexOES *thread_data =
      (EVGL_API_Thread_Command_glPointSizexOES *)data;

   orig_evgl_api_glPointSizexOES(thread_data->size);

}

EAPI void
glPointSizexOES_evgl_api_thread_cmd(GLfixed size)
{
   if (!evas_evgl_thread_enabled() || (_main_thread_id != eina_thread_self()))
     {
        orig_evgl_api_glPointSizexOES(size);
        return;
     }

   int thread_mode = EVAS_GL_THREAD_MODE_FINISH;

   EVGL_API_Thread_Command_glPointSizexOES thread_data_local;
   EVGL_API_Thread_Command_glPointSizexOES *thread_data = &thread_data_local;

   thread_data->size = size;

   evas_gl_thread_cmd_enqueue(EVAS_GL_THREAD_TYPE_EVGL,
                              _evgl_api_thread_glPointSizexOES,
                              thread_data,
                              thread_mode);
}

/*
 * void
 * glPolygonOffsetxOES(GLfixed factor, GLfixed units);
 */

typedef struct
{
   GLfixed factor;
   GLfixed units;

} EVGL_API_Thread_Command_glPolygonOffsetxOES;

void (*orig_evgl_api_glPolygonOffsetxOES)(GLfixed factor, GLfixed units);

static void
_evgl_api_thread_glPolygonOffsetxOES(void *data)
{
   EVGL_API_Thread_Command_glPolygonOffsetxOES *thread_data =
      (EVGL_API_Thread_Command_glPolygonOffsetxOES *)data;

   orig_evgl_api_glPolygonOffsetxOES(thread_data->factor,
                                     thread_data->units);

}

EAPI void
glPolygonOffsetxOES_evgl_api_thread_cmd(GLfixed factor, GLfixed units)
{
   if (!evas_evgl_thread_enabled() || (_main_thread_id != eina_thread_self()))
     {
        orig_evgl_api_glPolygonOffsetxOES(factor, units);
        return;
     }

   int thread_mode = EVAS_GL_THREAD_MODE_FINISH;

   EVGL_API_Thread_Command_glPolygonOffsetxOES thread_data_local;
   EVGL_API_Thread_Command_glPolygonOffsetxOES *thread_data = &thread_data_local;

   thread_data->factor = factor;
   thread_data->units = units;

   evas_gl_thread_cmd_enqueue(EVAS_GL_THREAD_TYPE_EVGL,
                              _evgl_api_thread_glPolygonOffsetxOES,
                              thread_data,
                              thread_mode);
}

/*
 * void
 * glRotatexOES(GLfixed angle, GLfixed x, GLfixed y, GLfixed z);
 */

typedef struct
{
   GLfixed angle;
   GLfixed x;
   GLfixed y;
   GLfixed z;

} EVGL_API_Thread_Command_glRotatexOES;

void (*orig_evgl_api_glRotatexOES)(GLfixed angle, GLfixed x, GLfixed y, GLfixed z);

static void
_evgl_api_thread_glRotatexOES(void *data)
{
   EVGL_API_Thread_Command_glRotatexOES *thread_data =
      (EVGL_API_Thread_Command_glRotatexOES *)data;

   orig_evgl_api_glRotatexOES(thread_data->angle,
                              thread_data->x,
                              thread_data->y,
                              thread_data->z);

}

EAPI void
glRotatexOES_evgl_api_thread_cmd(GLfixed angle, GLfixed x, GLfixed y, GLfixed z)
{
   if (!evas_evgl_thread_enabled() || (_main_thread_id != eina_thread_self()))
     {
        orig_evgl_api_glRotatexOES(angle, x, y, z);
        return;
     }

   int thread_mode = EVAS_GL_THREAD_MODE_FINISH;

   EVGL_API_Thread_Command_glRotatexOES thread_data_local;
   EVGL_API_Thread_Command_glRotatexOES *thread_data = &thread_data_local;

   thread_data->angle = angle;
   thread_data->x = x;
   thread_data->y = y;
   thread_data->z = z;

   evas_gl_thread_cmd_enqueue(EVAS_GL_THREAD_TYPE_EVGL,
                              _evgl_api_thread_glRotatexOES,
                              thread_data,
                              thread_mode);
}

/*
 * void
 * glSampleCoveragexOES(GLclampx value, GLboolean invert);
 */

typedef struct
{
   GLclampx value;
   GLboolean invert;

} EVGL_API_Thread_Command_glSampleCoveragexOES;

void (*orig_evgl_api_glSampleCoveragexOES)(GLclampx value, GLboolean invert);

static void
_evgl_api_thread_glSampleCoveragexOES(void *data)
{
   EVGL_API_Thread_Command_glSampleCoveragexOES *thread_data =
      (EVGL_API_Thread_Command_glSampleCoveragexOES *)data;

   orig_evgl_api_glSampleCoveragexOES(thread_data->value,
                                      thread_data->invert);

}

EAPI void
glSampleCoveragexOES_evgl_api_thread_cmd(GLclampx value, GLboolean invert)
{
   if (!evas_evgl_thread_enabled() || (_main_thread_id != eina_thread_self()))
     {
        orig_evgl_api_glSampleCoveragexOES(value, invert);
        return;
     }

   int thread_mode = EVAS_GL_THREAD_MODE_FINISH;

   EVGL_API_Thread_Command_glSampleCoveragexOES thread_data_local;
   EVGL_API_Thread_Command_glSampleCoveragexOES *thread_data = &thread_data_local;

   thread_data->value = value;
   thread_data->invert = invert;

   evas_gl_thread_cmd_enqueue(EVAS_GL_THREAD_TYPE_EVGL,
                              _evgl_api_thread_glSampleCoveragexOES,
                              thread_data,
                              thread_mode);
}

/*
 * void
 * glScalexOES(GLfixed x, GLfixed y, GLfixed z);
 */

typedef struct
{
   GLfixed x;
   GLfixed y;
   GLfixed z;

} EVGL_API_Thread_Command_glScalexOES;

void (*orig_evgl_api_glScalexOES)(GLfixed x, GLfixed y, GLfixed z);

static void
_evgl_api_thread_glScalexOES(void *data)
{
   EVGL_API_Thread_Command_glScalexOES *thread_data =
      (EVGL_API_Thread_Command_glScalexOES *)data;

   orig_evgl_api_glScalexOES(thread_data->x,
                             thread_data->y,
                             thread_data->z);

}

EAPI void
glScalexOES_evgl_api_thread_cmd(GLfixed x, GLfixed y, GLfixed z)
{
   if (!evas_evgl_thread_enabled() || (_main_thread_id != eina_thread_self()))
     {
        orig_evgl_api_glScalexOES(x, y, z);
        return;
     }

   int thread_mode = EVAS_GL_THREAD_MODE_FINISH;

   EVGL_API_Thread_Command_glScalexOES thread_data_local;
   EVGL_API_Thread_Command_glScalexOES *thread_data = &thread_data_local;

   thread_data->x = x;
   thread_data->y = y;
   thread_data->z = z;

   evas_gl_thread_cmd_enqueue(EVAS_GL_THREAD_TYPE_EVGL,
                              _evgl_api_thread_glScalexOES,
                              thread_data,
                              thread_mode);
}

/*
 * void
 * glTexEnvxOES(GLenum target, GLenum pname, GLfixed param);
 */

typedef struct
{
   GLenum target;
   GLenum pname;
   GLfixed param;

} EVGL_API_Thread_Command_glTexEnvxOES;

void (*orig_evgl_api_glTexEnvxOES)(GLenum target, GLenum pname, GLfixed param);

static void
_evgl_api_thread_glTexEnvxOES(void *data)
{
   EVGL_API_Thread_Command_glTexEnvxOES *thread_data =
      (EVGL_API_Thread_Command_glTexEnvxOES *)data;

   orig_evgl_api_glTexEnvxOES(thread_data->target,
                              thread_data->pname,
                              thread_data->param);

}

EAPI void
glTexEnvxOES_evgl_api_thread_cmd(GLenum target, GLenum pname, GLfixed param)
{
   if (!evas_evgl_thread_enabled() || (_main_thread_id != eina_thread_self()))
     {
        orig_evgl_api_glTexEnvxOES(target, pname, param);
        return;
     }

   int thread_mode = EVAS_GL_THREAD_MODE_FINISH;

   EVGL_API_Thread_Command_glTexEnvxOES thread_data_local;
   EVGL_API_Thread_Command_glTexEnvxOES *thread_data = &thread_data_local;

   thread_data->target = target;
   thread_data->pname = pname;
   thread_data->param = param;

   evas_gl_thread_cmd_enqueue(EVAS_GL_THREAD_TYPE_EVGL,
                              _evgl_api_thread_glTexEnvxOES,
                              thread_data,
                              thread_mode);
}

/*
 * void
 * glTexEnvxvOES(GLenum target, GLenum pname, const GLfixed *params);
 */

typedef struct
{
   GLenum target;
   GLenum pname;
   const GLfixed *params;

} EVGL_API_Thread_Command_glTexEnvxvOES;

void (*orig_evgl_api_glTexEnvxvOES)(GLenum target, GLenum pname, const GLfixed *params);

static void
_evgl_api_thread_glTexEnvxvOES(void *data)
{
   EVGL_API_Thread_Command_glTexEnvxvOES *thread_data =
      (EVGL_API_Thread_Command_glTexEnvxvOES *)data;

   orig_evgl_api_glTexEnvxvOES(thread_data->target,
                               thread_data->pname,
                               thread_data->params);

}

EAPI void
glTexEnvxvOES_evgl_api_thread_cmd(GLenum target, GLenum pname, const GLfixed *params)
{
   if (!evas_evgl_thread_enabled() || (_main_thread_id != eina_thread_self()))
     {
        orig_evgl_api_glTexEnvxvOES(target, pname, params);
        return;
     }

   int thread_mode = EVAS_GL_THREAD_MODE_FINISH;

   EVGL_API_Thread_Command_glTexEnvxvOES thread_data_local;
   EVGL_API_Thread_Command_glTexEnvxvOES *thread_data = &thread_data_local;

   thread_data->target = target;
   thread_data->pname = pname;
   thread_data->params = params;

   evas_gl_thread_cmd_enqueue(EVAS_GL_THREAD_TYPE_EVGL,
                              _evgl_api_thread_glTexEnvxvOES,
                              thread_data,
                              thread_mode);
}

/*
 * void
 * glTexParameterxOES(GLenum target, GLenum pname, GLfixed param);
 */

typedef struct
{
   GLenum target;
   GLenum pname;
   GLfixed param;

} EVGL_API_Thread_Command_glTexParameterxOES;

void (*orig_evgl_api_glTexParameterxOES)(GLenum target, GLenum pname, GLfixed param);

static void
_evgl_api_thread_glTexParameterxOES(void *data)
{
   EVGL_API_Thread_Command_glTexParameterxOES *thread_data =
      (EVGL_API_Thread_Command_glTexParameterxOES *)data;

   orig_evgl_api_glTexParameterxOES(thread_data->target,
                                    thread_data->pname,
                                    thread_data->param);

}

EAPI void
glTexParameterxOES_evgl_api_thread_cmd(GLenum target, GLenum pname, GLfixed param)
{
   if (!evas_evgl_thread_enabled() || (_main_thread_id != eina_thread_self()))
     {
        orig_evgl_api_glTexParameterxOES(target, pname, param);
        return;
     }

   int thread_mode = EVAS_GL_THREAD_MODE_FINISH;

   EVGL_API_Thread_Command_glTexParameterxOES thread_data_local;
   EVGL_API_Thread_Command_glTexParameterxOES *thread_data = &thread_data_local;

   thread_data->target = target;
   thread_data->pname = pname;
   thread_data->param = param;

   evas_gl_thread_cmd_enqueue(EVAS_GL_THREAD_TYPE_EVGL,
                              _evgl_api_thread_glTexParameterxOES,
                              thread_data,
                              thread_mode);
}

/*
 * void
 * glTexParameterxvOES(GLenum target, GLenum pname, const GLfixed *params);
 */

typedef struct
{
   GLenum target;
   GLenum pname;
   const GLfixed *params;

} EVGL_API_Thread_Command_glTexParameterxvOES;

void (*orig_evgl_api_glTexParameterxvOES)(GLenum target, GLenum pname, const GLfixed *params);

static void
_evgl_api_thread_glTexParameterxvOES(void *data)
{
   EVGL_API_Thread_Command_glTexParameterxvOES *thread_data =
      (EVGL_API_Thread_Command_glTexParameterxvOES *)data;

   orig_evgl_api_glTexParameterxvOES(thread_data->target,
                                     thread_data->pname,
                                     thread_data->params);

}

EAPI void
glTexParameterxvOES_evgl_api_thread_cmd(GLenum target, GLenum pname, const GLfixed *params)
{
   if (!evas_evgl_thread_enabled() || (_main_thread_id != eina_thread_self()))
     {
        orig_evgl_api_glTexParameterxvOES(target, pname, params);
        return;
     }

   int thread_mode = EVAS_GL_THREAD_MODE_FINISH;

   EVGL_API_Thread_Command_glTexParameterxvOES thread_data_local;
   EVGL_API_Thread_Command_glTexParameterxvOES *thread_data = &thread_data_local;

   thread_data->target = target;
   thread_data->pname = pname;
   thread_data->params = params;

   evas_gl_thread_cmd_enqueue(EVAS_GL_THREAD_TYPE_EVGL,
                              _evgl_api_thread_glTexParameterxvOES,
                              thread_data,
                              thread_mode);
}

/*
 * void
 * glTranslatexOES(GLfixed x, GLfixed y, GLfixed z);
 */

typedef struct
{
   GLfixed x;
   GLfixed y;
   GLfixed z;

} EVGL_API_Thread_Command_glTranslatexOES;

void (*orig_evgl_api_glTranslatexOES)(GLfixed x, GLfixed y, GLfixed z);

static void
_evgl_api_thread_glTranslatexOES(void *data)
{
   EVGL_API_Thread_Command_glTranslatexOES *thread_data =
      (EVGL_API_Thread_Command_glTranslatexOES *)data;

   orig_evgl_api_glTranslatexOES(thread_data->x,
                                 thread_data->y,
                                 thread_data->z);

}

EAPI void
glTranslatexOES_evgl_api_thread_cmd(GLfixed x, GLfixed y, GLfixed z)
{
   if (!evas_evgl_thread_enabled() || (_main_thread_id != eina_thread_self()))
     {
        orig_evgl_api_glTranslatexOES(x, y, z);
        return;
     }

   int thread_mode = EVAS_GL_THREAD_MODE_FINISH;

   EVGL_API_Thread_Command_glTranslatexOES thread_data_local;
   EVGL_API_Thread_Command_glTranslatexOES *thread_data = &thread_data_local;

   thread_data->x = x;
   thread_data->y = y;
   thread_data->z = z;

   evas_gl_thread_cmd_enqueue(EVAS_GL_THREAD_TYPE_EVGL,
                              _evgl_api_thread_glTranslatexOES,
                              thread_data,
                              thread_mode);
}

/*
 * GLboolean
 * glIsRenderbufferOES(GLuint renderbuffer);
 */

typedef struct
{
   GLboolean return_value;
   GLuint renderbuffer;

} EVGL_API_Thread_Command_glIsRenderbufferOES;

GLboolean (*orig_evgl_api_glIsRenderbufferOES)(GLuint renderbuffer);

static void
_evgl_api_thread_glIsRenderbufferOES(void *data)
{
   EVGL_API_Thread_Command_glIsRenderbufferOES *thread_data =
      (EVGL_API_Thread_Command_glIsRenderbufferOES *)data;

   thread_data->return_value = orig_evgl_api_glIsRenderbufferOES(thread_data->renderbuffer);

}

EAPI GLboolean
glIsRenderbufferOES_evgl_api_thread_cmd(GLuint renderbuffer)
{
   if (!evas_evgl_thread_enabled() || (_main_thread_id != eina_thread_self()))
     {
        return orig_evgl_api_glIsRenderbufferOES(renderbuffer);
     }

   int thread_mode = EVAS_GL_THREAD_MODE_FINISH;

   EVGL_API_Thread_Command_glIsRenderbufferOES thread_data_local;
   EVGL_API_Thread_Command_glIsRenderbufferOES *thread_data = &thread_data_local;

   thread_data->renderbuffer = renderbuffer;

   evas_gl_thread_cmd_enqueue(EVAS_GL_THREAD_TYPE_EVGL,
                              _evgl_api_thread_glIsRenderbufferOES,
                              thread_data,
                              thread_mode);

   return thread_data->return_value;
}

/*
 * void
 * glBindRenderbufferOES(GLenum target, GLuint renderbuffer);
 */

typedef struct
{
   GLenum target;
   GLuint renderbuffer;
   int command_allocated;

} EVGL_API_Thread_Command_glBindRenderbufferOES;

void (*orig_evgl_api_glBindRenderbufferOES)(GLenum target, GLuint renderbuffer);

static void
_evgl_api_thread_glBindRenderbufferOES(void *data)
{
   EVGL_API_Thread_Command_glBindRenderbufferOES *thread_data =
      (EVGL_API_Thread_Command_glBindRenderbufferOES *)data;

   orig_evgl_api_glBindRenderbufferOES(thread_data->target,
                                       thread_data->renderbuffer);

   if (thread_data->command_allocated)
     eina_mempool_free(_mp_command, thread_data);
}

EAPI void
glBindRenderbufferOES_evgl_api_thread_cmd(GLenum target, GLuint renderbuffer)
{
   if (!evas_evgl_thread_enabled() || (_main_thread_id != eina_thread_self()))
     {
        orig_evgl_api_glBindRenderbufferOES(target, renderbuffer);
        return;
     }

   int thread_mode = EVAS_GL_THREAD_MODE_FINISH;

   EVGL_API_Thread_Command_glBindRenderbufferOES thread_data_local;
   EVGL_API_Thread_Command_glBindRenderbufferOES *thread_data = &thread_data_local;

   /* command_allocated flag init. */
   thread_data->command_allocated = 0;

   if (!evas_gl_thread_force_finish())
     { /* _flush */
        EVGL_API_Thread_Command_glBindRenderbufferOES *thread_data_new;
        thread_data_new = eina_mempool_malloc(_mp_command,
                                              sizeof(EVGL_API_Thread_Command_glBindRenderbufferOES));
        if (thread_data_new)
          {
             thread_data = thread_data_new;
             thread_data->command_allocated = 1;
             thread_mode = EVAS_GL_THREAD_MODE_FLUSH;
          }
     }

   thread_data->target = target;
   thread_data->renderbuffer = renderbuffer;

   evas_gl_thread_cmd_enqueue(EVAS_GL_THREAD_TYPE_EVGL,
                              _evgl_api_thread_glBindRenderbufferOES,
                              thread_data,
                              thread_mode);
}

/*
 * void
 * glDeleteRenderbuffersOES(GLsizei n, const GLuint* renderbuffers);
 */

typedef struct
{
   GLsizei n;
   const GLuint* renderbuffers;
   void *renderbuffers_copied; /* COPIED */
   int command_allocated;

} EVGL_API_Thread_Command_glDeleteRenderbuffersOES;

void (*orig_evgl_api_glDeleteRenderbuffersOES)(GLsizei n, const GLuint* renderbuffers);

static void
_evgl_api_thread_glDeleteRenderbuffersOES(void *data)
{
   EVGL_API_Thread_Command_glDeleteRenderbuffersOES *thread_data =
      (EVGL_API_Thread_Command_glDeleteRenderbuffersOES *)data;

   orig_evgl_api_glDeleteRenderbuffersOES(thread_data->n,
                                          thread_data->renderbuffers);


   if (thread_data->renderbuffers_copied)
     eina_mempool_free(_mp_delete_object, thread_data->renderbuffers_copied);

   if (thread_data->command_allocated)
     eina_mempool_free(_mp_command, thread_data);
}

EAPI void
glDeleteRenderbuffersOES_evgl_api_thread_cmd(GLsizei n, const GLuint* renderbuffers)
{
   if (!evas_evgl_thread_enabled() || (_main_thread_id != eina_thread_self()))
     {
        orig_evgl_api_glDeleteRenderbuffersOES(n, renderbuffers);
        return;
     }

   int thread_mode = EVAS_GL_THREAD_MODE_FINISH;

   EVGL_API_Thread_Command_glDeleteRenderbuffersOES thread_data_local;
   EVGL_API_Thread_Command_glDeleteRenderbuffersOES *thread_data = &thread_data_local;

   /* command_allocated flag init. */
   thread_data->command_allocated = 0;

   if (!evas_gl_thread_force_finish())
     { /* _flush */
        EVGL_API_Thread_Command_glDeleteRenderbuffersOES *thread_data_new;
        thread_data_new = eina_mempool_malloc(_mp_command,
                                              sizeof(EVGL_API_Thread_Command_glDeleteRenderbuffersOES));
        if (thread_data_new)
          {
             thread_data = thread_data_new;
             thread_data->command_allocated = 1;
             thread_mode = EVAS_GL_THREAD_MODE_FLUSH;
          }
     }

   thread_data->n = n;
   thread_data->renderbuffers = renderbuffers;

   thread_data->renderbuffers_copied = NULL;
   if (thread_mode == EVAS_GL_THREAD_MODE_FINISH)
     goto finish;

   /* copy variable */
   if (renderbuffers)
     {
        /* 1. check memory size */
        unsigned int copy_size = n * sizeof(GLuint);
        if (copy_size > _mp_delete_object_memory_size)
          {
             thread_mode = EVAS_GL_THREAD_MODE_FINISH;
             goto finish;
          }
        /* 2. malloc & copy */
        thread_data->renderbuffers_copied = eina_mempool_malloc(_mp_delete_object, copy_size);
        if (thread_data->renderbuffers_copied)
          {
             memcpy(thread_data->renderbuffers_copied, renderbuffers, copy_size);
          }
        else
          {
             thread_mode = EVAS_GL_THREAD_MODE_FINISH;
             goto finish;
          }
        /* 3. replace */
        thread_data->renderbuffers = (const GLuint *)thread_data->renderbuffers_copied;
     }
   /* end of copy variable */

finish:
   evas_gl_thread_cmd_enqueue(EVAS_GL_THREAD_TYPE_EVGL,
                              _evgl_api_thread_glDeleteRenderbuffersOES,
                              thread_data,
                              thread_mode);
}

/*
 * void
 * glGenRenderbuffersOES(GLsizei n, GLuint* renderbuffers);
 */

typedef struct
{
   GLsizei n;
   GLuint* renderbuffers;

} EVGL_API_Thread_Command_glGenRenderbuffersOES;

void (*orig_evgl_api_glGenRenderbuffersOES)(GLsizei n, GLuint* renderbuffers);

static void
_evgl_api_thread_glGenRenderbuffersOES(void *data)
{
   EVGL_API_Thread_Command_glGenRenderbuffersOES *thread_data =
      (EVGL_API_Thread_Command_glGenRenderbuffersOES *)data;

   orig_evgl_api_glGenRenderbuffersOES(thread_data->n,
                                       thread_data->renderbuffers);

}

EAPI void
glGenRenderbuffersOES_evgl_api_thread_cmd(GLsizei n, GLuint* renderbuffers)
{
   if (!evas_evgl_thread_enabled() || (_main_thread_id != eina_thread_self()))
     {
        orig_evgl_api_glGenRenderbuffersOES(n, renderbuffers);
        return;
     }

   int thread_mode = EVAS_GL_THREAD_MODE_FINISH;

   EVGL_API_Thread_Command_glGenRenderbuffersOES thread_data_local;
   EVGL_API_Thread_Command_glGenRenderbuffersOES *thread_data = &thread_data_local;

   thread_data->n = n;
   thread_data->renderbuffers = renderbuffers;

   evas_gl_thread_cmd_enqueue(EVAS_GL_THREAD_TYPE_EVGL,
                              _evgl_api_thread_glGenRenderbuffersOES,
                              thread_data,
                              thread_mode);
}

/*
 * void
 * glRenderbufferStorageOES(GLenum target, GLenum internalformat, GLsizei width, GLsizei height);
 */

typedef struct
{
   GLenum target;
   GLenum internalformat;
   GLsizei width;
   GLsizei height;
   int command_allocated;

} EVGL_API_Thread_Command_glRenderbufferStorageOES;

void (*orig_evgl_api_glRenderbufferStorageOES)(GLenum target, GLenum internalformat, GLsizei width, GLsizei height);

static void
_evgl_api_thread_glRenderbufferStorageOES(void *data)
{
   EVGL_API_Thread_Command_glRenderbufferStorageOES *thread_data =
      (EVGL_API_Thread_Command_glRenderbufferStorageOES *)data;

   orig_evgl_api_glRenderbufferStorageOES(thread_data->target,
                                          thread_data->internalformat,
                                          thread_data->width,
                                          thread_data->height);

   if (thread_data->command_allocated)
     eina_mempool_free(_mp_command, thread_data);
}

EAPI void
glRenderbufferStorageOES_evgl_api_thread_cmd(GLenum target, GLenum internalformat, GLsizei width, GLsizei height)
{
   if (!evas_evgl_thread_enabled() || (_main_thread_id != eina_thread_self()))
     {
        orig_evgl_api_glRenderbufferStorageOES(target, internalformat, width, height);
        return;
     }

   int thread_mode = EVAS_GL_THREAD_MODE_FINISH;

   EVGL_API_Thread_Command_glRenderbufferStorageOES thread_data_local;
   EVGL_API_Thread_Command_glRenderbufferStorageOES *thread_data = &thread_data_local;

   /* command_allocated flag init. */
   thread_data->command_allocated = 0;

   if (!evas_gl_thread_force_finish())
     { /* _flush */
        EVGL_API_Thread_Command_glRenderbufferStorageOES *thread_data_new;
        thread_data_new = eina_mempool_malloc(_mp_command,
                                              sizeof(EVGL_API_Thread_Command_glRenderbufferStorageOES));
        if (thread_data_new)
          {
             thread_data = thread_data_new;
             thread_data->command_allocated = 1;
             thread_mode = EVAS_GL_THREAD_MODE_FLUSH;
          }
     }

   thread_data->target = target;
   thread_data->internalformat = internalformat;
   thread_data->width = width;
   thread_data->height = height;

   evas_gl_thread_cmd_enqueue(EVAS_GL_THREAD_TYPE_EVGL,
                              _evgl_api_thread_glRenderbufferStorageOES,
                              thread_data,
                              thread_mode);
}

/*
 * void
 * glGetRenderbufferParameterivOES(GLenum target, GLenum pname, GLint* params);
 */

typedef struct
{
   GLenum target;
   GLenum pname;
   GLint* params;

} EVGL_API_Thread_Command_glGetRenderbufferParameterivOES;

void (*orig_evgl_api_glGetRenderbufferParameterivOES)(GLenum target, GLenum pname, GLint* params);

static void
_evgl_api_thread_glGetRenderbufferParameterivOES(void *data)
{
   EVGL_API_Thread_Command_glGetRenderbufferParameterivOES *thread_data =
      (EVGL_API_Thread_Command_glGetRenderbufferParameterivOES *)data;

   orig_evgl_api_glGetRenderbufferParameterivOES(thread_data->target,
                                                 thread_data->pname,
                                                 thread_data->params);

}

EAPI void
glGetRenderbufferParameterivOES_evgl_api_thread_cmd(GLenum target, GLenum pname, GLint* params)
{
   if (!evas_evgl_thread_enabled() || (_main_thread_id != eina_thread_self()))
     {
        orig_evgl_api_glGetRenderbufferParameterivOES(target, pname, params);
        return;
     }

   int thread_mode = EVAS_GL_THREAD_MODE_FINISH;

   EVGL_API_Thread_Command_glGetRenderbufferParameterivOES thread_data_local;
   EVGL_API_Thread_Command_glGetRenderbufferParameterivOES *thread_data = &thread_data_local;

   thread_data->target = target;
   thread_data->pname = pname;
   thread_data->params = params;

   evas_gl_thread_cmd_enqueue(EVAS_GL_THREAD_TYPE_EVGL,
                              _evgl_api_thread_glGetRenderbufferParameterivOES,
                              thread_data,
                              thread_mode);
}

/*
 * GLboolean
 * glIsFramebufferOES(GLuint framebuffer);
 */

typedef struct
{
   GLboolean return_value;
   GLuint framebuffer;

} EVGL_API_Thread_Command_glIsFramebufferOES;

GLboolean (*orig_evgl_api_glIsFramebufferOES)(GLuint framebuffer);

static void
_evgl_api_thread_glIsFramebufferOES(void *data)
{
   EVGL_API_Thread_Command_glIsFramebufferOES *thread_data =
      (EVGL_API_Thread_Command_glIsFramebufferOES *)data;

   thread_data->return_value = orig_evgl_api_glIsFramebufferOES(thread_data->framebuffer);

}

EAPI GLboolean
glIsFramebufferOES_evgl_api_thread_cmd(GLuint framebuffer)
{
   if (!evas_evgl_thread_enabled() || (_main_thread_id != eina_thread_self()))
     {
        return orig_evgl_api_glIsFramebufferOES(framebuffer);
     }

   int thread_mode = EVAS_GL_THREAD_MODE_FINISH;

   EVGL_API_Thread_Command_glIsFramebufferOES thread_data_local;
   EVGL_API_Thread_Command_glIsFramebufferOES *thread_data = &thread_data_local;

   thread_data->framebuffer = framebuffer;

   evas_gl_thread_cmd_enqueue(EVAS_GL_THREAD_TYPE_EVGL,
                              _evgl_api_thread_glIsFramebufferOES,
                              thread_data,
                              thread_mode);

   return thread_data->return_value;
}

/*
 * void
 * glBindFramebufferOES(GLenum target, GLuint framebuffer);
 */

typedef struct
{
   GLenum target;
   GLuint framebuffer;
   int command_allocated;

} EVGL_API_Thread_Command_glBindFramebufferOES;

void (*orig_evgl_api_glBindFramebufferOES)(GLenum target, GLuint framebuffer);

static void
_evgl_api_thread_glBindFramebufferOES(void *data)
{
   EVGL_API_Thread_Command_glBindFramebufferOES *thread_data =
      (EVGL_API_Thread_Command_glBindFramebufferOES *)data;

   orig_evgl_api_glBindFramebufferOES(thread_data->target,
                                      thread_data->framebuffer);

   if (thread_data->command_allocated)
     eina_mempool_free(_mp_command, thread_data);
}

EAPI void
glBindFramebufferOES_evgl_api_thread_cmd(GLenum target, GLuint framebuffer)
{
   if (!evas_evgl_thread_enabled() || (_main_thread_id != eina_thread_self()))
     {
        orig_evgl_api_glBindFramebufferOES(target, framebuffer);
        return;
     }

   int thread_mode = EVAS_GL_THREAD_MODE_FINISH;

   EVGL_API_Thread_Command_glBindFramebufferOES thread_data_local;
   EVGL_API_Thread_Command_glBindFramebufferOES *thread_data = &thread_data_local;

   /* command_allocated flag init. */
   thread_data->command_allocated = 0;

   if (!evas_gl_thread_force_finish())
     { /* _flush */
        EVGL_API_Thread_Command_glBindFramebufferOES *thread_data_new;
        thread_data_new = eina_mempool_malloc(_mp_command,
                                              sizeof(EVGL_API_Thread_Command_glBindFramebufferOES));
        if (thread_data_new)
          {
             thread_data = thread_data_new;
             thread_data->command_allocated = 1;
             thread_mode = EVAS_GL_THREAD_MODE_FLUSH;
          }
     }

   thread_data->target = target;
   thread_data->framebuffer = framebuffer;

   evas_gl_thread_cmd_enqueue(EVAS_GL_THREAD_TYPE_EVGL,
                              _evgl_api_thread_glBindFramebufferOES,
                              thread_data,
                              thread_mode);
}

/*
 * void
 * glDeleteFramebuffersOES(GLsizei n, const GLuint* framebuffers);
 */

typedef struct
{
   GLsizei n;
   const GLuint* framebuffers;
   void *framebuffers_copied; /* COPIED */
   int command_allocated;

} EVGL_API_Thread_Command_glDeleteFramebuffersOES;

void (*orig_evgl_api_glDeleteFramebuffersOES)(GLsizei n, const GLuint* framebuffers);

static void
_evgl_api_thread_glDeleteFramebuffersOES(void *data)
{
   EVGL_API_Thread_Command_glDeleteFramebuffersOES *thread_data =
      (EVGL_API_Thread_Command_glDeleteFramebuffersOES *)data;

   orig_evgl_api_glDeleteFramebuffersOES(thread_data->n,
                                         thread_data->framebuffers);


   if (thread_data->framebuffers_copied)
     eina_mempool_free(_mp_delete_object, thread_data->framebuffers_copied);

   if (thread_data->command_allocated)
     eina_mempool_free(_mp_command, thread_data);
}

EAPI void
glDeleteFramebuffersOES_evgl_api_thread_cmd(GLsizei n, const GLuint* framebuffers)
{
   if (!evas_evgl_thread_enabled() || (_main_thread_id != eina_thread_self()))
     {
        orig_evgl_api_glDeleteFramebuffersOES(n, framebuffers);
        return;
     }

   int thread_mode = EVAS_GL_THREAD_MODE_FINISH;

   EVGL_API_Thread_Command_glDeleteFramebuffersOES thread_data_local;
   EVGL_API_Thread_Command_glDeleteFramebuffersOES *thread_data = &thread_data_local;

   /* command_allocated flag init. */
   thread_data->command_allocated = 0;

   if (!evas_gl_thread_force_finish())
     { /* _flush */
        EVGL_API_Thread_Command_glDeleteFramebuffersOES *thread_data_new;
        thread_data_new = eina_mempool_malloc(_mp_command,
                                              sizeof(EVGL_API_Thread_Command_glDeleteFramebuffersOES));
        if (thread_data_new)
          {
             thread_data = thread_data_new;
             thread_data->command_allocated = 1;
             thread_mode = EVAS_GL_THREAD_MODE_FLUSH;
          }
     }

   thread_data->n = n;
   thread_data->framebuffers = framebuffers;

   thread_data->framebuffers_copied = NULL;
   if (thread_mode == EVAS_GL_THREAD_MODE_FINISH)
     goto finish;

   /* copy variable */
   if (framebuffers)
     {
        /* 1. check memory size */
        unsigned int copy_size = n * sizeof(GLuint);
        if (copy_size > _mp_delete_object_memory_size)
          {
             thread_mode = EVAS_GL_THREAD_MODE_FINISH;
             goto finish;
          }
        /* 2. malloc & copy */
        thread_data->framebuffers_copied = eina_mempool_malloc(_mp_delete_object, copy_size);
        if (thread_data->framebuffers_copied)
          {
             memcpy(thread_data->framebuffers_copied, framebuffers, copy_size);
          }
        else
          {
             thread_mode = EVAS_GL_THREAD_MODE_FINISH;
             goto finish;
          }
        /* 3. replace */
        thread_data->framebuffers = (const GLuint *)thread_data->framebuffers_copied;
     }
   /* end of copy variable */

finish:
   evas_gl_thread_cmd_enqueue(EVAS_GL_THREAD_TYPE_EVGL,
                              _evgl_api_thread_glDeleteFramebuffersOES,
                              thread_data,
                              thread_mode);
}

/*
 * void
 * glGenFramebuffersOES(GLsizei n, GLuint* framebuffers);
 */

typedef struct
{
   GLsizei n;
   GLuint* framebuffers;

} EVGL_API_Thread_Command_glGenFramebuffersOES;

void (*orig_evgl_api_glGenFramebuffersOES)(GLsizei n, GLuint* framebuffers);

static void
_evgl_api_thread_glGenFramebuffersOES(void *data)
{
   EVGL_API_Thread_Command_glGenFramebuffersOES *thread_data =
      (EVGL_API_Thread_Command_glGenFramebuffersOES *)data;

   orig_evgl_api_glGenFramebuffersOES(thread_data->n,
                                      thread_data->framebuffers);

}

EAPI void
glGenFramebuffersOES_evgl_api_thread_cmd(GLsizei n, GLuint* framebuffers)
{
   if (!evas_evgl_thread_enabled() || (_main_thread_id != eina_thread_self()))
     {
        orig_evgl_api_glGenFramebuffersOES(n, framebuffers);
        return;
     }

   int thread_mode = EVAS_GL_THREAD_MODE_FINISH;

   EVGL_API_Thread_Command_glGenFramebuffersOES thread_data_local;
   EVGL_API_Thread_Command_glGenFramebuffersOES *thread_data = &thread_data_local;

   thread_data->n = n;
   thread_data->framebuffers = framebuffers;

   evas_gl_thread_cmd_enqueue(EVAS_GL_THREAD_TYPE_EVGL,
                              _evgl_api_thread_glGenFramebuffersOES,
                              thread_data,
                              thread_mode);
}

/*
 * GLenum
 * glCheckFramebufferStatusOES(GLenum target);
 */

typedef struct
{
   GLenum return_value;
   GLenum target;

} EVGL_API_Thread_Command_glCheckFramebufferStatusOES;

GLenum (*orig_evgl_api_glCheckFramebufferStatusOES)(GLenum target);

static void
_evgl_api_thread_glCheckFramebufferStatusOES(void *data)
{
   EVGL_API_Thread_Command_glCheckFramebufferStatusOES *thread_data =
      (EVGL_API_Thread_Command_glCheckFramebufferStatusOES *)data;

   thread_data->return_value = orig_evgl_api_glCheckFramebufferStatusOES(thread_data->target);

}

EAPI GLenum
glCheckFramebufferStatusOES_evgl_api_thread_cmd(GLenum target)
{
   if (!evas_evgl_thread_enabled() || (_main_thread_id != eina_thread_self()))
     {
        return orig_evgl_api_glCheckFramebufferStatusOES(target);
     }

   int thread_mode = EVAS_GL_THREAD_MODE_FINISH;

   EVGL_API_Thread_Command_glCheckFramebufferStatusOES thread_data_local;
   EVGL_API_Thread_Command_glCheckFramebufferStatusOES *thread_data = &thread_data_local;

   thread_data->target = target;

   evas_gl_thread_cmd_enqueue(EVAS_GL_THREAD_TYPE_EVGL,
                              _evgl_api_thread_glCheckFramebufferStatusOES,
                              thread_data,
                              thread_mode);

   return thread_data->return_value;
}

/*
 * void
 * glFramebufferRenderbufferOES(GLenum target, GLenum attachment, GLenum renderbuffertarget, GLuint renderbuffer);
 */

typedef struct
{
   GLenum target;
   GLenum attachment;
   GLenum renderbuffertarget;
   GLuint renderbuffer;
   int command_allocated;

} EVGL_API_Thread_Command_glFramebufferRenderbufferOES;

void (*orig_evgl_api_glFramebufferRenderbufferOES)(GLenum target, GLenum attachment, GLenum renderbuffertarget, GLuint renderbuffer);

static void
_evgl_api_thread_glFramebufferRenderbufferOES(void *data)
{
   EVGL_API_Thread_Command_glFramebufferRenderbufferOES *thread_data =
      (EVGL_API_Thread_Command_glFramebufferRenderbufferOES *)data;

   orig_evgl_api_glFramebufferRenderbufferOES(thread_data->target,
                                              thread_data->attachment,
                                              thread_data->renderbuffertarget,
                                              thread_data->renderbuffer);

   if (thread_data->command_allocated)
     eina_mempool_free(_mp_command, thread_data);
}

EAPI void
glFramebufferRenderbufferOES_evgl_api_thread_cmd(GLenum target, GLenum attachment, GLenum renderbuffertarget, GLuint renderbuffer)
{
   if (!evas_evgl_thread_enabled() || (_main_thread_id != eina_thread_self()))
     {
        orig_evgl_api_glFramebufferRenderbufferOES(target, attachment, renderbuffertarget, renderbuffer);
        return;
     }

   int thread_mode = EVAS_GL_THREAD_MODE_FINISH;

   EVGL_API_Thread_Command_glFramebufferRenderbufferOES thread_data_local;
   EVGL_API_Thread_Command_glFramebufferRenderbufferOES *thread_data = &thread_data_local;

   /* command_allocated flag init. */
   thread_data->command_allocated = 0;

   if (!evas_gl_thread_force_finish())
     { /* _flush */
        EVGL_API_Thread_Command_glFramebufferRenderbufferOES *thread_data_new;
        thread_data_new = eina_mempool_malloc(_mp_command,
                                              sizeof(EVGL_API_Thread_Command_glFramebufferRenderbufferOES));
        if (thread_data_new)
          {
             thread_data = thread_data_new;
             thread_data->command_allocated = 1;
             thread_mode = EVAS_GL_THREAD_MODE_FLUSH;
          }
     }

   thread_data->target = target;
   thread_data->attachment = attachment;
   thread_data->renderbuffertarget = renderbuffertarget;
   thread_data->renderbuffer = renderbuffer;

   evas_gl_thread_cmd_enqueue(EVAS_GL_THREAD_TYPE_EVGL,
                              _evgl_api_thread_glFramebufferRenderbufferOES,
                              thread_data,
                              thread_mode);
}

/*
 * void
 * glFramebufferTexture2DOES(GLenum target, GLenum attachment, GLenum textarget, GLuint texture, GLint level);
 */

typedef struct
{
   GLenum target;
   GLenum attachment;
   GLenum textarget;
   GLuint texture;
   GLint level;
   int command_allocated;

} EVGL_API_Thread_Command_glFramebufferTexture2DOES;

void (*orig_evgl_api_glFramebufferTexture2DOES)(GLenum target, GLenum attachment, GLenum textarget, GLuint texture, GLint level);

static void
_evgl_api_thread_glFramebufferTexture2DOES(void *data)
{
   EVGL_API_Thread_Command_glFramebufferTexture2DOES *thread_data =
      (EVGL_API_Thread_Command_glFramebufferTexture2DOES *)data;

   orig_evgl_api_glFramebufferTexture2DOES(thread_data->target,
                                           thread_data->attachment,
                                           thread_data->textarget,
                                           thread_data->texture,
                                           thread_data->level);

   if (thread_data->command_allocated)
     eina_mempool_free(_mp_command, thread_data);
}

EAPI void
glFramebufferTexture2DOES_evgl_api_thread_cmd(GLenum target, GLenum attachment, GLenum textarget, GLuint texture, GLint level)
{
   if (!evas_evgl_thread_enabled() || (_main_thread_id != eina_thread_self()))
     {
        orig_evgl_api_glFramebufferTexture2DOES(target, attachment, textarget, texture, level);
        return;
     }

   int thread_mode = EVAS_GL_THREAD_MODE_FINISH;

   EVGL_API_Thread_Command_glFramebufferTexture2DOES thread_data_local;
   EVGL_API_Thread_Command_glFramebufferTexture2DOES *thread_data = &thread_data_local;

   /* command_allocated flag init. */
   thread_data->command_allocated = 0;

   if (!evas_gl_thread_force_finish())
     { /* _flush */
        EVGL_API_Thread_Command_glFramebufferTexture2DOES *thread_data_new;
        thread_data_new = eina_mempool_malloc(_mp_command,
                                              sizeof(EVGL_API_Thread_Command_glFramebufferTexture2DOES));
        if (thread_data_new)
          {
             thread_data = thread_data_new;
             thread_data->command_allocated = 1;
             thread_mode = EVAS_GL_THREAD_MODE_FLUSH;
          }
     }

   thread_data->target = target;
   thread_data->attachment = attachment;
   thread_data->textarget = textarget;
   thread_data->texture = texture;
   thread_data->level = level;

   evas_gl_thread_cmd_enqueue(EVAS_GL_THREAD_TYPE_EVGL,
                              _evgl_api_thread_glFramebufferTexture2DOES,
                              thread_data,
                              thread_mode);
}

/*
 * void
 * glGetFramebufferAttachmentParameterivOES(GLenum target, GLenum attachment, GLenum pname, GLint* params);
 */

typedef struct
{
   GLenum target;
   GLenum attachment;
   GLenum pname;
   GLint* params;

} EVGL_API_Thread_Command_glGetFramebufferAttachmentParameterivOES;

void (*orig_evgl_api_glGetFramebufferAttachmentParameterivOES)(GLenum target, GLenum attachment, GLenum pname, GLint* params);

static void
_evgl_api_thread_glGetFramebufferAttachmentParameterivOES(void *data)
{
   EVGL_API_Thread_Command_glGetFramebufferAttachmentParameterivOES *thread_data =
      (EVGL_API_Thread_Command_glGetFramebufferAttachmentParameterivOES *)data;

   orig_evgl_api_glGetFramebufferAttachmentParameterivOES(thread_data->target,
                                                          thread_data->attachment,
                                                          thread_data->pname,
                                                          thread_data->params);

}

EAPI void
glGetFramebufferAttachmentParameterivOES_evgl_api_thread_cmd(GLenum target, GLenum attachment, GLenum pname, GLint* params)
{
   if (!evas_evgl_thread_enabled() || (_main_thread_id != eina_thread_self()))
     {
        orig_evgl_api_glGetFramebufferAttachmentParameterivOES(target, attachment, pname, params);
        return;
     }

   int thread_mode = EVAS_GL_THREAD_MODE_FINISH;

   EVGL_API_Thread_Command_glGetFramebufferAttachmentParameterivOES thread_data_local;
   EVGL_API_Thread_Command_glGetFramebufferAttachmentParameterivOES *thread_data = &thread_data_local;

   thread_data->target = target;
   thread_data->attachment = attachment;
   thread_data->pname = pname;
   thread_data->params = params;

   evas_gl_thread_cmd_enqueue(EVAS_GL_THREAD_TYPE_EVGL,
                              _evgl_api_thread_glGetFramebufferAttachmentParameterivOES,
                              thread_data,
                              thread_mode);
}

/*
 * void
 * glGenerateMipmapOES(GLenum target);
 */

typedef struct
{
   GLenum target;

} EVGL_API_Thread_Command_glGenerateMipmapOES;

void (*orig_evgl_api_glGenerateMipmapOES)(GLenum target);

static void
_evgl_api_thread_glGenerateMipmapOES(void *data)
{
   EVGL_API_Thread_Command_glGenerateMipmapOES *thread_data =
      (EVGL_API_Thread_Command_glGenerateMipmapOES *)data;

   orig_evgl_api_glGenerateMipmapOES(thread_data->target);

}

EAPI void
glGenerateMipmapOES_evgl_api_thread_cmd(GLenum target)
{
   if (!evas_evgl_thread_enabled() || (_main_thread_id != eina_thread_self()))
     {
        orig_evgl_api_glGenerateMipmapOES(target);
        return;
     }

   int thread_mode = EVAS_GL_THREAD_MODE_FINISH;

   EVGL_API_Thread_Command_glGenerateMipmapOES thread_data_local;
   EVGL_API_Thread_Command_glGenerateMipmapOES *thread_data = &thread_data_local;

   thread_data->target = target;

   evas_gl_thread_cmd_enqueue(EVAS_GL_THREAD_TYPE_EVGL,
                              _evgl_api_thread_glGenerateMipmapOES,
                              thread_data,
                              thread_mode);
}

/*
 * void
 * glCurrentPaletteMatrixOES(GLuint matrixpaletteindex);
 */

typedef struct
{
   GLuint matrixpaletteindex;

} EVGL_API_Thread_Command_glCurrentPaletteMatrixOES;

void (*orig_evgl_api_glCurrentPaletteMatrixOES)(GLuint matrixpaletteindex);

static void
_evgl_api_thread_glCurrentPaletteMatrixOES(void *data)
{
   EVGL_API_Thread_Command_glCurrentPaletteMatrixOES *thread_data =
      (EVGL_API_Thread_Command_glCurrentPaletteMatrixOES *)data;

   orig_evgl_api_glCurrentPaletteMatrixOES(thread_data->matrixpaletteindex);

}

EAPI void
glCurrentPaletteMatrixOES_evgl_api_thread_cmd(GLuint matrixpaletteindex)
{
   if (!evas_evgl_thread_enabled() || (_main_thread_id != eina_thread_self()))
     {
        orig_evgl_api_glCurrentPaletteMatrixOES(matrixpaletteindex);
        return;
     }

   int thread_mode = EVAS_GL_THREAD_MODE_FINISH;

   EVGL_API_Thread_Command_glCurrentPaletteMatrixOES thread_data_local;
   EVGL_API_Thread_Command_glCurrentPaletteMatrixOES *thread_data = &thread_data_local;

   thread_data->matrixpaletteindex = matrixpaletteindex;

   evas_gl_thread_cmd_enqueue(EVAS_GL_THREAD_TYPE_EVGL,
                              _evgl_api_thread_glCurrentPaletteMatrixOES,
                              thread_data,
                              thread_mode);
}

/*
 * void
 * glLoadPaletteFromModelViewMatrixOES(void);
 */

void (*orig_evgl_api_glLoadPaletteFromModelViewMatrixOES)(void);

static void
_evgl_api_thread_glLoadPaletteFromModelViewMatrixOES(void *data EINA_UNUSED)
{
   orig_evgl_api_glLoadPaletteFromModelViewMatrixOES();

}

EAPI void
glLoadPaletteFromModelViewMatrixOES_evgl_api_thread_cmd(void)
{
   if (!evas_evgl_thread_enabled() || (_main_thread_id != eina_thread_self()))
     {
        orig_evgl_api_glLoadPaletteFromModelViewMatrixOES();
        return;
     }

   int thread_mode = EVAS_GL_THREAD_MODE_FINISH;

   evas_gl_thread_cmd_enqueue(EVAS_GL_THREAD_TYPE_EVGL,
                              _evgl_api_thread_glLoadPaletteFromModelViewMatrixOES,
                              NULL,
                              thread_mode);
}

/*
 * void
 * glMatrixIndexPointerOES(GLint size, GLenum type, GLsizei stride, const GLvoid *pointer);
 */

typedef struct
{
   GLint size;
   GLenum type;
   GLsizei stride;
   const GLvoid *pointer;

} EVGL_API_Thread_Command_glMatrixIndexPointerOES;

void (*orig_evgl_api_glMatrixIndexPointerOES)(GLint size, GLenum type, GLsizei stride, const GLvoid *pointer);

static void
_evgl_api_thread_glMatrixIndexPointerOES(void *data)
{
   EVGL_API_Thread_Command_glMatrixIndexPointerOES *thread_data =
      (EVGL_API_Thread_Command_glMatrixIndexPointerOES *)data;

   orig_evgl_api_glMatrixIndexPointerOES(thread_data->size,
                                         thread_data->type,
                                         thread_data->stride,
                                         thread_data->pointer);

}

EAPI void
glMatrixIndexPointerOES_evgl_api_thread_cmd(GLint size, GLenum type, GLsizei stride, const GLvoid *pointer)
{
   if (!evas_evgl_thread_enabled() || (_main_thread_id != eina_thread_self()))
     {
        orig_evgl_api_glMatrixIndexPointerOES(size, type, stride, pointer);
        return;
     }

   int thread_mode = EVAS_GL_THREAD_MODE_FINISH;

   EVGL_API_Thread_Command_glMatrixIndexPointerOES thread_data_local;
   EVGL_API_Thread_Command_glMatrixIndexPointerOES *thread_data = &thread_data_local;

   thread_data->size = size;
   thread_data->type = type;
   thread_data->stride = stride;
   thread_data->pointer = pointer;

   evas_gl_thread_cmd_enqueue(EVAS_GL_THREAD_TYPE_EVGL,
                              _evgl_api_thread_glMatrixIndexPointerOES,
                              thread_data,
                              thread_mode);
}

/*
 * void
 * glWeightPointerOES(GLint size, GLenum type, GLsizei stride, const GLvoid *pointer);
 */

typedef struct
{
   GLint size;
   GLenum type;
   GLsizei stride;
   const GLvoid *pointer;

} EVGL_API_Thread_Command_glWeightPointerOES;

void (*orig_evgl_api_glWeightPointerOES)(GLint size, GLenum type, GLsizei stride, const GLvoid *pointer);

static void
_evgl_api_thread_glWeightPointerOES(void *data)
{
   EVGL_API_Thread_Command_glWeightPointerOES *thread_data =
      (EVGL_API_Thread_Command_glWeightPointerOES *)data;

   orig_evgl_api_glWeightPointerOES(thread_data->size,
                                    thread_data->type,
                                    thread_data->stride,
                                    thread_data->pointer);

}

EAPI void
glWeightPointerOES_evgl_api_thread_cmd(GLint size, GLenum type, GLsizei stride, const GLvoid *pointer)
{
   if (!evas_evgl_thread_enabled() || (_main_thread_id != eina_thread_self()))
     {
        orig_evgl_api_glWeightPointerOES(size, type, stride, pointer);
        return;
     }

   int thread_mode = EVAS_GL_THREAD_MODE_FINISH;

   EVGL_API_Thread_Command_glWeightPointerOES thread_data_local;
   EVGL_API_Thread_Command_glWeightPointerOES *thread_data = &thread_data_local;

   thread_data->size = size;
   thread_data->type = type;
   thread_data->stride = stride;
   thread_data->pointer = pointer;

   evas_gl_thread_cmd_enqueue(EVAS_GL_THREAD_TYPE_EVGL,
                              _evgl_api_thread_glWeightPointerOES,
                              thread_data,
                              thread_mode);
}

/*
 * GLbitfield
 * glQueryMatrixxOES(GLfixed mantissa[16], GLint exponent[16]);
 */

typedef struct
{
   GLbitfield return_value;
   GLfixed mantissa[16];
   GLint exponent[16];

} EVGL_API_Thread_Command_glQueryMatrixxOES;

GLbitfield (*orig_evgl_api_glQueryMatrixxOES)(GLfixed mantissa[16], GLint exponent[16]);

static void
_evgl_api_thread_glQueryMatrixxOES(void *data)
{
   EVGL_API_Thread_Command_glQueryMatrixxOES *thread_data =
      (EVGL_API_Thread_Command_glQueryMatrixxOES *)data;

   thread_data->return_value = orig_evgl_api_glQueryMatrixxOES(thread_data->mantissa,
                                                               thread_data->exponent);

}

EAPI GLbitfield
glQueryMatrixxOES_evgl_api_thread_cmd(GLfixed mantissa[16], GLint exponent[16])
{
   if (!evas_evgl_thread_enabled() || (_main_thread_id != eina_thread_self()))
     {
        return orig_evgl_api_glQueryMatrixxOES(mantissa, exponent);
     }

   int thread_mode = EVAS_GL_THREAD_MODE_FINISH;

   EVGL_API_Thread_Command_glQueryMatrixxOES thread_data_local;
   EVGL_API_Thread_Command_glQueryMatrixxOES *thread_data = &thread_data_local;

   memcpy(thread_data->mantissa, &mantissa, sizeof(GLfixed) * 16);
   memcpy(thread_data->exponent, &exponent, sizeof(GLint) * 16);

   evas_gl_thread_cmd_enqueue(EVAS_GL_THREAD_TYPE_EVGL,
                              _evgl_api_thread_glQueryMatrixxOES,
                              thread_data,
                              thread_mode);

   return thread_data->return_value;
}

/*
 * void
 * glDepthRangefOES(GLclampf zNear, GLclampf zFar);
 */

typedef struct
{
   GLclampf zNear;
   GLclampf zFar;

} EVGL_API_Thread_Command_glDepthRangefOES;

void (*orig_evgl_api_glDepthRangefOES)(GLclampf zNear, GLclampf zFar);

static void
_evgl_api_thread_glDepthRangefOES(void *data)
{
   EVGL_API_Thread_Command_glDepthRangefOES *thread_data =
      (EVGL_API_Thread_Command_glDepthRangefOES *)data;

   orig_evgl_api_glDepthRangefOES(thread_data->zNear,
                                  thread_data->zFar);

}

EAPI void
glDepthRangefOES_evgl_api_thread_cmd(GLclampf zNear, GLclampf zFar)
{
   if (!evas_evgl_thread_enabled() || (_main_thread_id != eina_thread_self()))
     {
        orig_evgl_api_glDepthRangefOES(zNear, zFar);
        return;
     }

   int thread_mode = EVAS_GL_THREAD_MODE_FINISH;

   EVGL_API_Thread_Command_glDepthRangefOES thread_data_local;
   EVGL_API_Thread_Command_glDepthRangefOES *thread_data = &thread_data_local;

   thread_data->zNear = zNear;
   thread_data->zFar = zFar;

   evas_gl_thread_cmd_enqueue(EVAS_GL_THREAD_TYPE_EVGL,
                              _evgl_api_thread_glDepthRangefOES,
                              thread_data,
                              thread_mode);
}

/*
 * void
 * glFrustumfOES(GLfloat left, GLfloat right, GLfloat bottom, GLfloat top, GLfloat zNear, GLfloat zFar);
 */

typedef struct
{
   GLfloat left;
   GLfloat right;
   GLfloat bottom;
   GLfloat top;
   GLfloat zNear;
   GLfloat zFar;

} EVGL_API_Thread_Command_glFrustumfOES;

void (*orig_evgl_api_glFrustumfOES)(GLfloat left, GLfloat right, GLfloat bottom, GLfloat top, GLfloat zNear, GLfloat zFar);

static void
_evgl_api_thread_glFrustumfOES(void *data)
{
   EVGL_API_Thread_Command_glFrustumfOES *thread_data =
      (EVGL_API_Thread_Command_glFrustumfOES *)data;

   orig_evgl_api_glFrustumfOES(thread_data->left,
                               thread_data->right,
                               thread_data->bottom,
                               thread_data->top,
                               thread_data->zNear,
                               thread_data->zFar);

}

EAPI void
glFrustumfOES_evgl_api_thread_cmd(GLfloat left, GLfloat right, GLfloat bottom, GLfloat top, GLfloat zNear, GLfloat zFar)
{
   if (!evas_evgl_thread_enabled() || (_main_thread_id != eina_thread_self()))
     {
        orig_evgl_api_glFrustumfOES(left, right, bottom, top, zNear, zFar);
        return;
     }

   int thread_mode = EVAS_GL_THREAD_MODE_FINISH;

   EVGL_API_Thread_Command_glFrustumfOES thread_data_local;
   EVGL_API_Thread_Command_glFrustumfOES *thread_data = &thread_data_local;

   thread_data->left = left;
   thread_data->right = right;
   thread_data->bottom = bottom;
   thread_data->top = top;
   thread_data->zNear = zNear;
   thread_data->zFar = zFar;

   evas_gl_thread_cmd_enqueue(EVAS_GL_THREAD_TYPE_EVGL,
                              _evgl_api_thread_glFrustumfOES,
                              thread_data,
                              thread_mode);
}

/*
 * void
 * glOrthofOES(GLfloat left, GLfloat right, GLfloat bottom, GLfloat top, GLfloat zNear, GLfloat zFar);
 */

typedef struct
{
   GLfloat left;
   GLfloat right;
   GLfloat bottom;
   GLfloat top;
   GLfloat zNear;
   GLfloat zFar;

} EVGL_API_Thread_Command_glOrthofOES;

void (*orig_evgl_api_glOrthofOES)(GLfloat left, GLfloat right, GLfloat bottom, GLfloat top, GLfloat zNear, GLfloat zFar);

static void
_evgl_api_thread_glOrthofOES(void *data)
{
   EVGL_API_Thread_Command_glOrthofOES *thread_data =
      (EVGL_API_Thread_Command_glOrthofOES *)data;

   orig_evgl_api_glOrthofOES(thread_data->left,
                             thread_data->right,
                             thread_data->bottom,
                             thread_data->top,
                             thread_data->zNear,
                             thread_data->zFar);

}

EAPI void
glOrthofOES_evgl_api_thread_cmd(GLfloat left, GLfloat right, GLfloat bottom, GLfloat top, GLfloat zNear, GLfloat zFar)
{
   if (!evas_evgl_thread_enabled() || (_main_thread_id != eina_thread_self()))
     {
        orig_evgl_api_glOrthofOES(left, right, bottom, top, zNear, zFar);
        return;
     }

   int thread_mode = EVAS_GL_THREAD_MODE_FINISH;

   EVGL_API_Thread_Command_glOrthofOES thread_data_local;
   EVGL_API_Thread_Command_glOrthofOES *thread_data = &thread_data_local;

   thread_data->left = left;
   thread_data->right = right;
   thread_data->bottom = bottom;
   thread_data->top = top;
   thread_data->zNear = zNear;
   thread_data->zFar = zFar;

   evas_gl_thread_cmd_enqueue(EVAS_GL_THREAD_TYPE_EVGL,
                              _evgl_api_thread_glOrthofOES,
                              thread_data,
                              thread_mode);
}

/*
 * void
 * glClipPlanefOES(GLenum plane, const GLfloat *equation);
 */

typedef struct
{
   GLenum plane;
   const GLfloat *equation;

} EVGL_API_Thread_Command_glClipPlanefOES;

void (*orig_evgl_api_glClipPlanefOES)(GLenum plane, const GLfloat *equation);

static void
_evgl_api_thread_glClipPlanefOES(void *data)
{
   EVGL_API_Thread_Command_glClipPlanefOES *thread_data =
      (EVGL_API_Thread_Command_glClipPlanefOES *)data;

   orig_evgl_api_glClipPlanefOES(thread_data->plane,
                                 thread_data->equation);

}

EAPI void
glClipPlanefOES_evgl_api_thread_cmd(GLenum plane, const GLfloat *equation)
{
   if (!evas_evgl_thread_enabled() || (_main_thread_id != eina_thread_self()))
     {
        orig_evgl_api_glClipPlanefOES(plane, equation);
        return;
     }

   int thread_mode = EVAS_GL_THREAD_MODE_FINISH;

   EVGL_API_Thread_Command_glClipPlanefOES thread_data_local;
   EVGL_API_Thread_Command_glClipPlanefOES *thread_data = &thread_data_local;

   thread_data->plane = plane;
   thread_data->equation = equation;

   evas_gl_thread_cmd_enqueue(EVAS_GL_THREAD_TYPE_EVGL,
                              _evgl_api_thread_glClipPlanefOES,
                              thread_data,
                              thread_mode);
}

/*
 * void
 * glGetClipPlanefOES(GLenum pname, GLfloat eqn[4]);
 */

typedef struct
{
   GLenum pname;
   GLfloat eqn[4];

} EVGL_API_Thread_Command_glGetClipPlanefOES;

void (*orig_evgl_api_glGetClipPlanefOES)(GLenum pname, GLfloat eqn[4]);

static void
_evgl_api_thread_glGetClipPlanefOES(void *data)
{
   EVGL_API_Thread_Command_glGetClipPlanefOES *thread_data =
      (EVGL_API_Thread_Command_glGetClipPlanefOES *)data;

   orig_evgl_api_glGetClipPlanefOES(thread_data->pname,
                                    thread_data->eqn);

}

EAPI void
glGetClipPlanefOES_evgl_api_thread_cmd(GLenum pname, GLfloat eqn[4])
{
   if (!evas_evgl_thread_enabled() || (_main_thread_id != eina_thread_self()))
     {
        orig_evgl_api_glGetClipPlanefOES(pname, eqn);
        return;
     }

   int thread_mode = EVAS_GL_THREAD_MODE_FINISH;

   EVGL_API_Thread_Command_glGetClipPlanefOES thread_data_local;
   EVGL_API_Thread_Command_glGetClipPlanefOES *thread_data = &thread_data_local;

   thread_data->pname = pname;
   memcpy(thread_data->eqn, &eqn, sizeof(GLfloat) * 4);

   evas_gl_thread_cmd_enqueue(EVAS_GL_THREAD_TYPE_EVGL,
                              _evgl_api_thread_glGetClipPlanefOES,
                              thread_data,
                              thread_mode);
}

/*
 * void
 * glClearDepthfOES(GLclampf depth);
 */

typedef struct
{
   GLclampf depth;

} EVGL_API_Thread_Command_glClearDepthfOES;

void (*orig_evgl_api_glClearDepthfOES)(GLclampf depth);

static void
_evgl_api_thread_glClearDepthfOES(void *data)
{
   EVGL_API_Thread_Command_glClearDepthfOES *thread_data =
      (EVGL_API_Thread_Command_glClearDepthfOES *)data;

   orig_evgl_api_glClearDepthfOES(thread_data->depth);

}

EAPI void
glClearDepthfOES_evgl_api_thread_cmd(GLclampf depth)
{
   if (!evas_evgl_thread_enabled() || (_main_thread_id != eina_thread_self()))
     {
        orig_evgl_api_glClearDepthfOES(depth);
        return;
     }

   int thread_mode = EVAS_GL_THREAD_MODE_FINISH;

   EVGL_API_Thread_Command_glClearDepthfOES thread_data_local;
   EVGL_API_Thread_Command_glClearDepthfOES *thread_data = &thread_data_local;

   thread_data->depth = depth;

   evas_gl_thread_cmd_enqueue(EVAS_GL_THREAD_TYPE_EVGL,
                              _evgl_api_thread_glClearDepthfOES,
                              thread_data,
                              thread_mode);
}

/*
 * void
 * glTexGenfOES(GLenum coord, GLenum pname, GLfloat param);
 */

typedef struct
{
   GLenum coord;
   GLenum pname;
   GLfloat param;

} EVGL_API_Thread_Command_glTexGenfOES;

void (*orig_evgl_api_glTexGenfOES)(GLenum coord, GLenum pname, GLfloat param);

static void
_evgl_api_thread_glTexGenfOES(void *data)
{
   EVGL_API_Thread_Command_glTexGenfOES *thread_data =
      (EVGL_API_Thread_Command_glTexGenfOES *)data;

   orig_evgl_api_glTexGenfOES(thread_data->coord,
                              thread_data->pname,
                              thread_data->param);

}

EAPI void
glTexGenfOES_evgl_api_thread_cmd(GLenum coord, GLenum pname, GLfloat param)
{
   if (!evas_evgl_thread_enabled() || (_main_thread_id != eina_thread_self()))
     {
        orig_evgl_api_glTexGenfOES(coord, pname, param);
        return;
     }

   int thread_mode = EVAS_GL_THREAD_MODE_FINISH;

   EVGL_API_Thread_Command_glTexGenfOES thread_data_local;
   EVGL_API_Thread_Command_glTexGenfOES *thread_data = &thread_data_local;

   thread_data->coord = coord;
   thread_data->pname = pname;
   thread_data->param = param;

   evas_gl_thread_cmd_enqueue(EVAS_GL_THREAD_TYPE_EVGL,
                              _evgl_api_thread_glTexGenfOES,
                              thread_data,
                              thread_mode);
}

/*
 * void
 * glTexGenfvOES(GLenum coord, GLenum pname, const GLfloat *params);
 */

typedef struct
{
   GLenum coord;
   GLenum pname;
   const GLfloat *params;

} EVGL_API_Thread_Command_glTexGenfvOES;

void (*orig_evgl_api_glTexGenfvOES)(GLenum coord, GLenum pname, const GLfloat *params);

static void
_evgl_api_thread_glTexGenfvOES(void *data)
{
   EVGL_API_Thread_Command_glTexGenfvOES *thread_data =
      (EVGL_API_Thread_Command_glTexGenfvOES *)data;

   orig_evgl_api_glTexGenfvOES(thread_data->coord,
                               thread_data->pname,
                               thread_data->params);

}

EAPI void
glTexGenfvOES_evgl_api_thread_cmd(GLenum coord, GLenum pname, const GLfloat *params)
{
   if (!evas_evgl_thread_enabled() || (_main_thread_id != eina_thread_self()))
     {
        orig_evgl_api_glTexGenfvOES(coord, pname, params);
        return;
     }

   int thread_mode = EVAS_GL_THREAD_MODE_FINISH;

   EVGL_API_Thread_Command_glTexGenfvOES thread_data_local;
   EVGL_API_Thread_Command_glTexGenfvOES *thread_data = &thread_data_local;

   thread_data->coord = coord;
   thread_data->pname = pname;
   thread_data->params = params;

   evas_gl_thread_cmd_enqueue(EVAS_GL_THREAD_TYPE_EVGL,
                              _evgl_api_thread_glTexGenfvOES,
                              thread_data,
                              thread_mode);
}

/*
 * void
 * glTexGeniOES(GLenum coord, GLenum pname, GLint param);
 */

typedef struct
{
   GLenum coord;
   GLenum pname;
   GLint param;

} EVGL_API_Thread_Command_glTexGeniOES;

void (*orig_evgl_api_glTexGeniOES)(GLenum coord, GLenum pname, GLint param);

static void
_evgl_api_thread_glTexGeniOES(void *data)
{
   EVGL_API_Thread_Command_glTexGeniOES *thread_data =
      (EVGL_API_Thread_Command_glTexGeniOES *)data;

   orig_evgl_api_glTexGeniOES(thread_data->coord,
                              thread_data->pname,
                              thread_data->param);

}

EAPI void
glTexGeniOES_evgl_api_thread_cmd(GLenum coord, GLenum pname, GLint param)
{
   if (!evas_evgl_thread_enabled() || (_main_thread_id != eina_thread_self()))
     {
        orig_evgl_api_glTexGeniOES(coord, pname, param);
        return;
     }

   int thread_mode = EVAS_GL_THREAD_MODE_FINISH;

   EVGL_API_Thread_Command_glTexGeniOES thread_data_local;
   EVGL_API_Thread_Command_glTexGeniOES *thread_data = &thread_data_local;

   thread_data->coord = coord;
   thread_data->pname = pname;
   thread_data->param = param;

   evas_gl_thread_cmd_enqueue(EVAS_GL_THREAD_TYPE_EVGL,
                              _evgl_api_thread_glTexGeniOES,
                              thread_data,
                              thread_mode);
}

/*
 * void
 * glTexGenivOES(GLenum coord, GLenum pname, const GLint *params);
 */

typedef struct
{
   GLenum coord;
   GLenum pname;
   const GLint *params;

} EVGL_API_Thread_Command_glTexGenivOES;

void (*orig_evgl_api_glTexGenivOES)(GLenum coord, GLenum pname, const GLint *params);

static void
_evgl_api_thread_glTexGenivOES(void *data)
{
   EVGL_API_Thread_Command_glTexGenivOES *thread_data =
      (EVGL_API_Thread_Command_glTexGenivOES *)data;

   orig_evgl_api_glTexGenivOES(thread_data->coord,
                               thread_data->pname,
                               thread_data->params);

}

EAPI void
glTexGenivOES_evgl_api_thread_cmd(GLenum coord, GLenum pname, const GLint *params)
{
   if (!evas_evgl_thread_enabled() || (_main_thread_id != eina_thread_self()))
     {
        orig_evgl_api_glTexGenivOES(coord, pname, params);
        return;
     }

   int thread_mode = EVAS_GL_THREAD_MODE_FINISH;

   EVGL_API_Thread_Command_glTexGenivOES thread_data_local;
   EVGL_API_Thread_Command_glTexGenivOES *thread_data = &thread_data_local;

   thread_data->coord = coord;
   thread_data->pname = pname;
   thread_data->params = params;

   evas_gl_thread_cmd_enqueue(EVAS_GL_THREAD_TYPE_EVGL,
                              _evgl_api_thread_glTexGenivOES,
                              thread_data,
                              thread_mode);
}

/*
 * void
 * glTexGenxOES(GLenum coord, GLenum pname, GLfixed param);
 */

typedef struct
{
   GLenum coord;
   GLenum pname;
   GLfixed param;

} EVGL_API_Thread_Command_glTexGenxOES;

void (*orig_evgl_api_glTexGenxOES)(GLenum coord, GLenum pname, GLfixed param);

static void
_evgl_api_thread_glTexGenxOES(void *data)
{
   EVGL_API_Thread_Command_glTexGenxOES *thread_data =
      (EVGL_API_Thread_Command_glTexGenxOES *)data;

   orig_evgl_api_glTexGenxOES(thread_data->coord,
                              thread_data->pname,
                              thread_data->param);

}

EAPI void
glTexGenxOES_evgl_api_thread_cmd(GLenum coord, GLenum pname, GLfixed param)
{
   if (!evas_evgl_thread_enabled() || (_main_thread_id != eina_thread_self()))
     {
        orig_evgl_api_glTexGenxOES(coord, pname, param);
        return;
     }

   int thread_mode = EVAS_GL_THREAD_MODE_FINISH;

   EVGL_API_Thread_Command_glTexGenxOES thread_data_local;
   EVGL_API_Thread_Command_glTexGenxOES *thread_data = &thread_data_local;

   thread_data->coord = coord;
   thread_data->pname = pname;
   thread_data->param = param;

   evas_gl_thread_cmd_enqueue(EVAS_GL_THREAD_TYPE_EVGL,
                              _evgl_api_thread_glTexGenxOES,
                              thread_data,
                              thread_mode);
}

/*
 * void
 * glTexGenxvOES(GLenum coord, GLenum pname, const GLfixed *params);
 */

typedef struct
{
   GLenum coord;
   GLenum pname;
   const GLfixed *params;

} EVGL_API_Thread_Command_glTexGenxvOES;

void (*orig_evgl_api_glTexGenxvOES)(GLenum coord, GLenum pname, const GLfixed *params);

static void
_evgl_api_thread_glTexGenxvOES(void *data)
{
   EVGL_API_Thread_Command_glTexGenxvOES *thread_data =
      (EVGL_API_Thread_Command_glTexGenxvOES *)data;

   orig_evgl_api_glTexGenxvOES(thread_data->coord,
                               thread_data->pname,
                               thread_data->params);

}

EAPI void
glTexGenxvOES_evgl_api_thread_cmd(GLenum coord, GLenum pname, const GLfixed *params)
{
   if (!evas_evgl_thread_enabled() || (_main_thread_id != eina_thread_self()))
     {
        orig_evgl_api_glTexGenxvOES(coord, pname, params);
        return;
     }

   int thread_mode = EVAS_GL_THREAD_MODE_FINISH;

   EVGL_API_Thread_Command_glTexGenxvOES thread_data_local;
   EVGL_API_Thread_Command_glTexGenxvOES *thread_data = &thread_data_local;

   thread_data->coord = coord;
   thread_data->pname = pname;
   thread_data->params = params;

   evas_gl_thread_cmd_enqueue(EVAS_GL_THREAD_TYPE_EVGL,
                              _evgl_api_thread_glTexGenxvOES,
                              thread_data,
                              thread_mode);
}

/*
 * void
 * glGetTexGenfvOES(GLenum coord, GLenum pname, GLfloat *params);
 */

typedef struct
{
   GLenum coord;
   GLenum pname;
   GLfloat *params;

} EVGL_API_Thread_Command_glGetTexGenfvOES;

void (*orig_evgl_api_glGetTexGenfvOES)(GLenum coord, GLenum pname, GLfloat *params);

static void
_evgl_api_thread_glGetTexGenfvOES(void *data)
{
   EVGL_API_Thread_Command_glGetTexGenfvOES *thread_data =
      (EVGL_API_Thread_Command_glGetTexGenfvOES *)data;

   orig_evgl_api_glGetTexGenfvOES(thread_data->coord,
                                  thread_data->pname,
                                  thread_data->params);

}

EAPI void
glGetTexGenfvOES_evgl_api_thread_cmd(GLenum coord, GLenum pname, GLfloat *params)
{
   if (!evas_evgl_thread_enabled() || (_main_thread_id != eina_thread_self()))
     {
        orig_evgl_api_glGetTexGenfvOES(coord, pname, params);
        return;
     }

   int thread_mode = EVAS_GL_THREAD_MODE_FINISH;

   EVGL_API_Thread_Command_glGetTexGenfvOES thread_data_local;
   EVGL_API_Thread_Command_glGetTexGenfvOES *thread_data = &thread_data_local;

   thread_data->coord = coord;
   thread_data->pname = pname;
   thread_data->params = params;

   evas_gl_thread_cmd_enqueue(EVAS_GL_THREAD_TYPE_EVGL,
                              _evgl_api_thread_glGetTexGenfvOES,
                              thread_data,
                              thread_mode);
}

/*
 * void
 * glGetTexGenivOES(GLenum coord, GLenum pname, GLint *params);
 */

typedef struct
{
   GLenum coord;
   GLenum pname;
   GLint *params;

} EVGL_API_Thread_Command_glGetTexGenivOES;

void (*orig_evgl_api_glGetTexGenivOES)(GLenum coord, GLenum pname, GLint *params);

static void
_evgl_api_thread_glGetTexGenivOES(void *data)
{
   EVGL_API_Thread_Command_glGetTexGenivOES *thread_data =
      (EVGL_API_Thread_Command_glGetTexGenivOES *)data;

   orig_evgl_api_glGetTexGenivOES(thread_data->coord,
                                  thread_data->pname,
                                  thread_data->params);

}

EAPI void
glGetTexGenivOES_evgl_api_thread_cmd(GLenum coord, GLenum pname, GLint *params)
{
   if (!evas_evgl_thread_enabled() || (_main_thread_id != eina_thread_self()))
     {
        orig_evgl_api_glGetTexGenivOES(coord, pname, params);
        return;
     }

   int thread_mode = EVAS_GL_THREAD_MODE_FINISH;

   EVGL_API_Thread_Command_glGetTexGenivOES thread_data_local;
   EVGL_API_Thread_Command_glGetTexGenivOES *thread_data = &thread_data_local;

   thread_data->coord = coord;
   thread_data->pname = pname;
   thread_data->params = params;

   evas_gl_thread_cmd_enqueue(EVAS_GL_THREAD_TYPE_EVGL,
                              _evgl_api_thread_glGetTexGenivOES,
                              thread_data,
                              thread_mode);
}

/*
 * void
 * glGetTexGenxvOES(GLenum coord, GLenum pname, GLfixed *params);
 */

typedef struct
{
   GLenum coord;
   GLenum pname;
   GLfixed *params;

} EVGL_API_Thread_Command_glGetTexGenxvOES;

void (*orig_evgl_api_glGetTexGenxvOES)(GLenum coord, GLenum pname, GLfixed *params);

static void
_evgl_api_thread_glGetTexGenxvOES(void *data)
{
   EVGL_API_Thread_Command_glGetTexGenxvOES *thread_data =
      (EVGL_API_Thread_Command_glGetTexGenxvOES *)data;

   orig_evgl_api_glGetTexGenxvOES(thread_data->coord,
                                  thread_data->pname,
                                  thread_data->params);

}

EAPI void
glGetTexGenxvOES_evgl_api_thread_cmd(GLenum coord, GLenum pname, GLfixed *params)
{
   if (!evas_evgl_thread_enabled() || (_main_thread_id != eina_thread_self()))
     {
        orig_evgl_api_glGetTexGenxvOES(coord, pname, params);
        return;
     }

   int thread_mode = EVAS_GL_THREAD_MODE_FINISH;

   EVGL_API_Thread_Command_glGetTexGenxvOES thread_data_local;
   EVGL_API_Thread_Command_glGetTexGenxvOES *thread_data = &thread_data_local;

   thread_data->coord = coord;
   thread_data->pname = pname;
   thread_data->params = params;

   evas_gl_thread_cmd_enqueue(EVAS_GL_THREAD_TYPE_EVGL,
                              _evgl_api_thread_glGetTexGenxvOES,
                              thread_data,
                              thread_mode);
}

/*
 * void
 * glBindVertexArrayOES(GLuint array);
 */

typedef struct
{
   GLuint array;

} EVGL_API_Thread_Command_glBindVertexArrayOES;

void (*orig_evgl_api_glBindVertexArrayOES)(GLuint array);

static void
_evgl_api_thread_glBindVertexArrayOES(void *data)
{
   EVGL_API_Thread_Command_glBindVertexArrayOES *thread_data =
      (EVGL_API_Thread_Command_glBindVertexArrayOES *)data;

   orig_evgl_api_glBindVertexArrayOES(thread_data->array);

}

EAPI void
glBindVertexArrayOES_evgl_api_thread_cmd(GLuint array)
{
   if (!evas_evgl_thread_enabled() || (_main_thread_id != eina_thread_self()))
     {
        orig_evgl_api_glBindVertexArrayOES(array);
        return;
     }

   int thread_mode = EVAS_GL_THREAD_MODE_FINISH;

   EVGL_API_Thread_Command_glBindVertexArrayOES thread_data_local;
   EVGL_API_Thread_Command_glBindVertexArrayOES *thread_data = &thread_data_local;

   thread_data->array = array;

   evas_gl_thread_cmd_enqueue(EVAS_GL_THREAD_TYPE_EVGL,
                              _evgl_api_thread_glBindVertexArrayOES,
                              thread_data,
                              thread_mode);
}

/*
 * void
 * glDeleteVertexArraysOES(GLsizei n, const GLuint *arrays);
 */

typedef struct
{
   GLsizei n;
   const GLuint *arrays;

} EVGL_API_Thread_Command_glDeleteVertexArraysOES;

void (*orig_evgl_api_glDeleteVertexArraysOES)(GLsizei n, const GLuint *arrays);

static void
_evgl_api_thread_glDeleteVertexArraysOES(void *data)
{
   EVGL_API_Thread_Command_glDeleteVertexArraysOES *thread_data =
      (EVGL_API_Thread_Command_glDeleteVertexArraysOES *)data;

   orig_evgl_api_glDeleteVertexArraysOES(thread_data->n,
                                         thread_data->arrays);

}

EAPI void
glDeleteVertexArraysOES_evgl_api_thread_cmd(GLsizei n, const GLuint *arrays)
{
   if (!evas_evgl_thread_enabled() || (_main_thread_id != eina_thread_self()))
     {
        orig_evgl_api_glDeleteVertexArraysOES(n, arrays);
        return;
     }

   int thread_mode = EVAS_GL_THREAD_MODE_FINISH;

   EVGL_API_Thread_Command_glDeleteVertexArraysOES thread_data_local;
   EVGL_API_Thread_Command_glDeleteVertexArraysOES *thread_data = &thread_data_local;

   thread_data->n = n;
   thread_data->arrays = arrays;

   evas_gl_thread_cmd_enqueue(EVAS_GL_THREAD_TYPE_EVGL,
                              _evgl_api_thread_glDeleteVertexArraysOES,
                              thread_data,
                              thread_mode);
}

/*
 * void
 * glGenVertexArraysOES(GLsizei n, GLuint *arrays);
 */

typedef struct
{
   GLsizei n;
   GLuint *arrays;

} EVGL_API_Thread_Command_glGenVertexArraysOES;

void (*orig_evgl_api_glGenVertexArraysOES)(GLsizei n, GLuint *arrays);

static void
_evgl_api_thread_glGenVertexArraysOES(void *data)
{
   EVGL_API_Thread_Command_glGenVertexArraysOES *thread_data =
      (EVGL_API_Thread_Command_glGenVertexArraysOES *)data;

   orig_evgl_api_glGenVertexArraysOES(thread_data->n,
                                      thread_data->arrays);

}

EAPI void
glGenVertexArraysOES_evgl_api_thread_cmd(GLsizei n, GLuint *arrays)
{
   if (!evas_evgl_thread_enabled() || (_main_thread_id != eina_thread_self()))
     {
        orig_evgl_api_glGenVertexArraysOES(n, arrays);
        return;
     }

   int thread_mode = EVAS_GL_THREAD_MODE_FINISH;

   EVGL_API_Thread_Command_glGenVertexArraysOES thread_data_local;
   EVGL_API_Thread_Command_glGenVertexArraysOES *thread_data = &thread_data_local;

   thread_data->n = n;
   thread_data->arrays = arrays;

   evas_gl_thread_cmd_enqueue(EVAS_GL_THREAD_TYPE_EVGL,
                              _evgl_api_thread_glGenVertexArraysOES,
                              thread_data,
                              thread_mode);
}

/*
 * GLboolean
 * glIsVertexArrayOES(GLuint array);
 */

typedef struct
{
   GLboolean return_value;
   GLuint array;

} EVGL_API_Thread_Command_glIsVertexArrayOES;

GLboolean (*orig_evgl_api_glIsVertexArrayOES)(GLuint array);

static void
_evgl_api_thread_glIsVertexArrayOES(void *data)
{
   EVGL_API_Thread_Command_glIsVertexArrayOES *thread_data =
      (EVGL_API_Thread_Command_glIsVertexArrayOES *)data;

   thread_data->return_value = orig_evgl_api_glIsVertexArrayOES(thread_data->array);

}

EAPI GLboolean
glIsVertexArrayOES_evgl_api_thread_cmd(GLuint array)
{
   if (!evas_evgl_thread_enabled() || (_main_thread_id != eina_thread_self()))
     {
        return orig_evgl_api_glIsVertexArrayOES(array);
     }

   int thread_mode = EVAS_GL_THREAD_MODE_FINISH;

   EVGL_API_Thread_Command_glIsVertexArrayOES thread_data_local;
   EVGL_API_Thread_Command_glIsVertexArrayOES *thread_data = &thread_data_local;

   thread_data->array = array;

   evas_gl_thread_cmd_enqueue(EVAS_GL_THREAD_TYPE_EVGL,
                              _evgl_api_thread_glIsVertexArrayOES,
                              thread_data,
                              thread_mode);

   return thread_data->return_value;
}

/*
 * void
 * glCopyTextureLevelsAPPLE(GLuint destinationTexture, GLuint sourceTexture, GLint sourceBaseLevel, GLsizei sourceLevelCount);
 */

typedef struct
{
   GLuint destinationTexture;
   GLuint sourceTexture;
   GLint sourceBaseLevel;
   GLsizei sourceLevelCount;

} EVGL_API_Thread_Command_glCopyTextureLevelsAPPLE;

void (*orig_evgl_api_glCopyTextureLevelsAPPLE)(GLuint destinationTexture, GLuint sourceTexture, GLint sourceBaseLevel, GLsizei sourceLevelCount);

static void
_evgl_api_thread_glCopyTextureLevelsAPPLE(void *data)
{
   EVGL_API_Thread_Command_glCopyTextureLevelsAPPLE *thread_data =
      (EVGL_API_Thread_Command_glCopyTextureLevelsAPPLE *)data;

   orig_evgl_api_glCopyTextureLevelsAPPLE(thread_data->destinationTexture,
                                          thread_data->sourceTexture,
                                          thread_data->sourceBaseLevel,
                                          thread_data->sourceLevelCount);

}

EAPI void
glCopyTextureLevelsAPPLE_evgl_api_thread_cmd(GLuint destinationTexture, GLuint sourceTexture, GLint sourceBaseLevel, GLsizei sourceLevelCount)
{
   if (!evas_evgl_thread_enabled() || (_main_thread_id != eina_thread_self()))
     {
        orig_evgl_api_glCopyTextureLevelsAPPLE(destinationTexture, sourceTexture, sourceBaseLevel, sourceLevelCount);
        return;
     }

   int thread_mode = EVAS_GL_THREAD_MODE_FINISH;

   EVGL_API_Thread_Command_glCopyTextureLevelsAPPLE thread_data_local;
   EVGL_API_Thread_Command_glCopyTextureLevelsAPPLE *thread_data = &thread_data_local;

   thread_data->destinationTexture = destinationTexture;
   thread_data->sourceTexture = sourceTexture;
   thread_data->sourceBaseLevel = sourceBaseLevel;
   thread_data->sourceLevelCount = sourceLevelCount;

   evas_gl_thread_cmd_enqueue(EVAS_GL_THREAD_TYPE_EVGL,
                              _evgl_api_thread_glCopyTextureLevelsAPPLE,
                              thread_data,
                              thread_mode);
}

/*
 * void
 * glRenderbufferStorageMultisampleAPPLE(GLenum a, GLsizei b, GLenum c, GLsizei d, GLsizei e);
 */

typedef struct
{
   GLenum a;
   GLsizei b;
   GLenum c;
   GLsizei d;
   GLsizei e;

} EVGL_API_Thread_Command_glRenderbufferStorageMultisampleAPPLE;

void (*orig_evgl_api_glRenderbufferStorageMultisampleAPPLE)(GLenum a, GLsizei b, GLenum c, GLsizei d, GLsizei e);

static void
_evgl_api_thread_glRenderbufferStorageMultisampleAPPLE(void *data)
{
   EVGL_API_Thread_Command_glRenderbufferStorageMultisampleAPPLE *thread_data =
      (EVGL_API_Thread_Command_glRenderbufferStorageMultisampleAPPLE *)data;

   orig_evgl_api_glRenderbufferStorageMultisampleAPPLE(thread_data->a,
                                                       thread_data->b,
                                                       thread_data->c,
                                                       thread_data->d,
                                                       thread_data->e);

}

EAPI void
glRenderbufferStorageMultisampleAPPLE_evgl_api_thread_cmd(GLenum a, GLsizei b, GLenum c, GLsizei d, GLsizei e)
{
   if (!evas_evgl_thread_enabled() || (_main_thread_id != eina_thread_self()))
     {
        orig_evgl_api_glRenderbufferStorageMultisampleAPPLE(a, b, c, d, e);
        return;
     }

   int thread_mode = EVAS_GL_THREAD_MODE_FINISH;

   EVGL_API_Thread_Command_glRenderbufferStorageMultisampleAPPLE thread_data_local;
   EVGL_API_Thread_Command_glRenderbufferStorageMultisampleAPPLE *thread_data = &thread_data_local;

   thread_data->a = a;
   thread_data->b = b;
   thread_data->c = c;
   thread_data->d = d;
   thread_data->e = e;

   evas_gl_thread_cmd_enqueue(EVAS_GL_THREAD_TYPE_EVGL,
                              _evgl_api_thread_glRenderbufferStorageMultisampleAPPLE,
                              thread_data,
                              thread_mode);
}

/*
 * void
 * glResolveMultisampleFramebufferAPPLE(void);
 */

void (*orig_evgl_api_glResolveMultisampleFramebufferAPPLE)(void);

static void
_evgl_api_thread_glResolveMultisampleFramebufferAPPLE(void *data EINA_UNUSED)
{
   orig_evgl_api_glResolveMultisampleFramebufferAPPLE();

}

EAPI void
glResolveMultisampleFramebufferAPPLE_evgl_api_thread_cmd(void)
{
   if (!evas_evgl_thread_enabled() || (_main_thread_id != eina_thread_self()))
     {
        orig_evgl_api_glResolveMultisampleFramebufferAPPLE();
        return;
     }

   int thread_mode = EVAS_GL_THREAD_MODE_FINISH;

   evas_gl_thread_cmd_enqueue(EVAS_GL_THREAD_TYPE_EVGL,
                              _evgl_api_thread_glResolveMultisampleFramebufferAPPLE,
                              NULL,
                              thread_mode);
}

/*
 * GLsync
 * glFenceSyncAPPLE(GLenum condition, GLbitfield flags);
 */

typedef struct
{
   GLsync return_value;
   GLenum condition;
   GLbitfield flags;

} EVGL_API_Thread_Command_glFenceSyncAPPLE;

GLsync (*orig_evgl_api_glFenceSyncAPPLE)(GLenum condition, GLbitfield flags);

static void
_evgl_api_thread_glFenceSyncAPPLE(void *data)
{
   EVGL_API_Thread_Command_glFenceSyncAPPLE *thread_data =
      (EVGL_API_Thread_Command_glFenceSyncAPPLE *)data;

   thread_data->return_value = orig_evgl_api_glFenceSyncAPPLE(thread_data->condition,
                                                              thread_data->flags);

}

EAPI GLsync
glFenceSyncAPPLE_evgl_api_thread_cmd(GLenum condition, GLbitfield flags)
{
   if (!evas_evgl_thread_enabled() || (_main_thread_id != eina_thread_self()))
     {
        return orig_evgl_api_glFenceSyncAPPLE(condition, flags);
     }

   int thread_mode = EVAS_GL_THREAD_MODE_FINISH;

   EVGL_API_Thread_Command_glFenceSyncAPPLE thread_data_local;
   EVGL_API_Thread_Command_glFenceSyncAPPLE *thread_data = &thread_data_local;

   thread_data->condition = condition;
   thread_data->flags = flags;

   evas_gl_thread_cmd_enqueue(EVAS_GL_THREAD_TYPE_EVGL,
                              _evgl_api_thread_glFenceSyncAPPLE,
                              thread_data,
                              thread_mode);

   return thread_data->return_value;
}

/*
 * GLboolean
 * glIsSyncAPPLE(GLsync sync);
 */

typedef struct
{
   GLboolean return_value;
   GLsync sync;

} EVGL_API_Thread_Command_glIsSyncAPPLE;

GLboolean (*orig_evgl_api_glIsSyncAPPLE)(GLsync sync);

static void
_evgl_api_thread_glIsSyncAPPLE(void *data)
{
   EVGL_API_Thread_Command_glIsSyncAPPLE *thread_data =
      (EVGL_API_Thread_Command_glIsSyncAPPLE *)data;

   thread_data->return_value = orig_evgl_api_glIsSyncAPPLE(thread_data->sync);

}

EAPI GLboolean
glIsSyncAPPLE_evgl_api_thread_cmd(GLsync sync)
{
   if (!evas_evgl_thread_enabled() || (_main_thread_id != eina_thread_self()))
     {
        return orig_evgl_api_glIsSyncAPPLE(sync);
     }

   int thread_mode = EVAS_GL_THREAD_MODE_FINISH;

   EVGL_API_Thread_Command_glIsSyncAPPLE thread_data_local;
   EVGL_API_Thread_Command_glIsSyncAPPLE *thread_data = &thread_data_local;

   thread_data->sync = sync;

   evas_gl_thread_cmd_enqueue(EVAS_GL_THREAD_TYPE_EVGL,
                              _evgl_api_thread_glIsSyncAPPLE,
                              thread_data,
                              thread_mode);

   return thread_data->return_value;
}

/*
 * void
 * glDeleteSyncAPPLE(GLsync sync);
 */

typedef struct
{
   GLsync sync;

} EVGL_API_Thread_Command_glDeleteSyncAPPLE;

void (*orig_evgl_api_glDeleteSyncAPPLE)(GLsync sync);

static void
_evgl_api_thread_glDeleteSyncAPPLE(void *data)
{
   EVGL_API_Thread_Command_glDeleteSyncAPPLE *thread_data =
      (EVGL_API_Thread_Command_glDeleteSyncAPPLE *)data;

   orig_evgl_api_glDeleteSyncAPPLE(thread_data->sync);

}

EAPI void
glDeleteSyncAPPLE_evgl_api_thread_cmd(GLsync sync)
{
   if (!evas_evgl_thread_enabled() || (_main_thread_id != eina_thread_self()))
     {
        orig_evgl_api_glDeleteSyncAPPLE(sync);
        return;
     }

   int thread_mode = EVAS_GL_THREAD_MODE_FINISH;

   EVGL_API_Thread_Command_glDeleteSyncAPPLE thread_data_local;
   EVGL_API_Thread_Command_glDeleteSyncAPPLE *thread_data = &thread_data_local;

   thread_data->sync = sync;

   evas_gl_thread_cmd_enqueue(EVAS_GL_THREAD_TYPE_EVGL,
                              _evgl_api_thread_glDeleteSyncAPPLE,
                              thread_data,
                              thread_mode);
}

/*
 * GLenum
 * glClientWaitSyncAPPLE(GLsync sync, GLbitfield flags, EvasGLuint64 timeout);
 */

typedef struct
{
   GLenum return_value;
   GLsync sync;
   GLbitfield flags;
   EvasGLuint64 timeout;

} EVGL_API_Thread_Command_glClientWaitSyncAPPLE;

GLenum (*orig_evgl_api_glClientWaitSyncAPPLE)(GLsync sync, GLbitfield flags, EvasGLuint64 timeout);

static void
_evgl_api_thread_glClientWaitSyncAPPLE(void *data)
{
   EVGL_API_Thread_Command_glClientWaitSyncAPPLE *thread_data =
      (EVGL_API_Thread_Command_glClientWaitSyncAPPLE *)data;

   thread_data->return_value = orig_evgl_api_glClientWaitSyncAPPLE(thread_data->sync,
                                                                   thread_data->flags,
                                                                   thread_data->timeout);

}

EAPI GLenum
glClientWaitSyncAPPLE_evgl_api_thread_cmd(GLsync sync, GLbitfield flags, EvasGLuint64 timeout)
{
   if (!evas_evgl_thread_enabled() || (_main_thread_id != eina_thread_self()))
     {
        return orig_evgl_api_glClientWaitSyncAPPLE(sync, flags, timeout);
     }

   int thread_mode = EVAS_GL_THREAD_MODE_FINISH;

   EVGL_API_Thread_Command_glClientWaitSyncAPPLE thread_data_local;
   EVGL_API_Thread_Command_glClientWaitSyncAPPLE *thread_data = &thread_data_local;

   thread_data->sync = sync;
   thread_data->flags = flags;
   thread_data->timeout = timeout;

   evas_gl_thread_cmd_enqueue(EVAS_GL_THREAD_TYPE_EVGL,
                              _evgl_api_thread_glClientWaitSyncAPPLE,
                              thread_data,
                              thread_mode);

   return thread_data->return_value;
}

/*
 * void
 * glWaitSyncAPPLE(GLsync sync, GLbitfield flags, EvasGLuint64 timeout);
 */

typedef struct
{
   GLsync sync;
   GLbitfield flags;
   EvasGLuint64 timeout;

} EVGL_API_Thread_Command_glWaitSyncAPPLE;

void (*orig_evgl_api_glWaitSyncAPPLE)(GLsync sync, GLbitfield flags, EvasGLuint64 timeout);

static void
_evgl_api_thread_glWaitSyncAPPLE(void *data)
{
   EVGL_API_Thread_Command_glWaitSyncAPPLE *thread_data =
      (EVGL_API_Thread_Command_glWaitSyncAPPLE *)data;

   orig_evgl_api_glWaitSyncAPPLE(thread_data->sync,
                                 thread_data->flags,
                                 thread_data->timeout);

}

EAPI void
glWaitSyncAPPLE_evgl_api_thread_cmd(GLsync sync, GLbitfield flags, EvasGLuint64 timeout)
{
   if (!evas_evgl_thread_enabled() || (_main_thread_id != eina_thread_self()))
     {
        orig_evgl_api_glWaitSyncAPPLE(sync, flags, timeout);
        return;
     }

   int thread_mode = EVAS_GL_THREAD_MODE_FINISH;

   EVGL_API_Thread_Command_glWaitSyncAPPLE thread_data_local;
   EVGL_API_Thread_Command_glWaitSyncAPPLE *thread_data = &thread_data_local;

   thread_data->sync = sync;
   thread_data->flags = flags;
   thread_data->timeout = timeout;

   evas_gl_thread_cmd_enqueue(EVAS_GL_THREAD_TYPE_EVGL,
                              _evgl_api_thread_glWaitSyncAPPLE,
                              thread_data,
                              thread_mode);
}

/*
 * void
 * glGetInteger64vAPPLE(GLenum pname, EvasGLint64 *params);
 */

typedef struct
{
   GLenum pname;
   EvasGLint64 *params;

} EVGL_API_Thread_Command_glGetInteger64vAPPLE;

void (*orig_evgl_api_glGetInteger64vAPPLE)(GLenum pname, EvasGLint64 *params);

static void
_evgl_api_thread_glGetInteger64vAPPLE(void *data)
{
   EVGL_API_Thread_Command_glGetInteger64vAPPLE *thread_data =
      (EVGL_API_Thread_Command_glGetInteger64vAPPLE *)data;

   orig_evgl_api_glGetInteger64vAPPLE(thread_data->pname,
                                      thread_data->params);

}

EAPI void
glGetInteger64vAPPLE_evgl_api_thread_cmd(GLenum pname, EvasGLint64 *params)
{
   if (!evas_evgl_thread_enabled() || (_main_thread_id != eina_thread_self()))
     {
        orig_evgl_api_glGetInteger64vAPPLE(pname, params);
        return;
     }

   int thread_mode = EVAS_GL_THREAD_MODE_FINISH;

   EVGL_API_Thread_Command_glGetInteger64vAPPLE thread_data_local;
   EVGL_API_Thread_Command_glGetInteger64vAPPLE *thread_data = &thread_data_local;

   thread_data->pname = pname;
   thread_data->params = params;

   evas_gl_thread_cmd_enqueue(EVAS_GL_THREAD_TYPE_EVGL,
                              _evgl_api_thread_glGetInteger64vAPPLE,
                              thread_data,
                              thread_mode);
}

/*
 * void
 * glGetSyncivAPPLE(GLsync sync, GLenum pname, GLsizei bufSize, GLsizei *length, GLint *values);
 */

typedef struct
{
   GLsync sync;
   GLenum pname;
   GLsizei bufSize;
   GLsizei *length;
   GLint *values;

} EVGL_API_Thread_Command_glGetSyncivAPPLE;

void (*orig_evgl_api_glGetSyncivAPPLE)(GLsync sync, GLenum pname, GLsizei bufSize, GLsizei *length, GLint *values);

static void
_evgl_api_thread_glGetSyncivAPPLE(void *data)
{
   EVGL_API_Thread_Command_glGetSyncivAPPLE *thread_data =
      (EVGL_API_Thread_Command_glGetSyncivAPPLE *)data;

   orig_evgl_api_glGetSyncivAPPLE(thread_data->sync,
                                  thread_data->pname,
                                  thread_data->bufSize,
                                  thread_data->length,
                                  thread_data->values);

}

EAPI void
glGetSyncivAPPLE_evgl_api_thread_cmd(GLsync sync, GLenum pname, GLsizei bufSize, GLsizei *length, GLint *values)
{
   if (!evas_evgl_thread_enabled() || (_main_thread_id != eina_thread_self()))
     {
        orig_evgl_api_glGetSyncivAPPLE(sync, pname, bufSize, length, values);
        return;
     }

   int thread_mode = EVAS_GL_THREAD_MODE_FINISH;

   EVGL_API_Thread_Command_glGetSyncivAPPLE thread_data_local;
   EVGL_API_Thread_Command_glGetSyncivAPPLE *thread_data = &thread_data_local;

   thread_data->sync = sync;
   thread_data->pname = pname;
   thread_data->bufSize = bufSize;
   thread_data->length = length;
   thread_data->values = values;

   evas_gl_thread_cmd_enqueue(EVAS_GL_THREAD_TYPE_EVGL,
                              _evgl_api_thread_glGetSyncivAPPLE,
                              thread_data,
                              thread_mode);
}

/*
 * void *
 * glMapBufferRangeEXT(GLenum target, GLintptr offset, GLsizeiptr length, GLbitfield access);
 */

typedef struct
{
   void * return_value;
   GLenum target;
   GLintptr offset;
   GLsizeiptr length;
   GLbitfield access;

} EVGL_API_Thread_Command_glMapBufferRangeEXT;

void * (*orig_evgl_api_glMapBufferRangeEXT)(GLenum target, GLintptr offset, GLsizeiptr length, GLbitfield access);

static void
_evgl_api_thread_glMapBufferRangeEXT(void *data)
{
   EVGL_API_Thread_Command_glMapBufferRangeEXT *thread_data =
      (EVGL_API_Thread_Command_glMapBufferRangeEXT *)data;

   thread_data->return_value = orig_evgl_api_glMapBufferRangeEXT(thread_data->target,
                                                                 thread_data->offset,
                                                                 thread_data->length,
                                                                 thread_data->access);

}

EAPI void *
glMapBufferRangeEXT_evgl_api_thread_cmd(GLenum target, GLintptr offset, GLsizeiptr length, GLbitfield access)
{
   if (!evas_evgl_thread_enabled() || (_main_thread_id != eina_thread_self()))
     {
        return orig_evgl_api_glMapBufferRangeEXT(target, offset, length, access);
     }

   int thread_mode = EVAS_GL_THREAD_MODE_FINISH;

   EVGL_API_Thread_Command_glMapBufferRangeEXT thread_data_local;
   EVGL_API_Thread_Command_glMapBufferRangeEXT *thread_data = &thread_data_local;

   thread_data->target = target;
   thread_data->offset = offset;
   thread_data->length = length;
   thread_data->access = access;

   evas_gl_thread_cmd_enqueue(EVAS_GL_THREAD_TYPE_EVGL,
                              _evgl_api_thread_glMapBufferRangeEXT,
                              thread_data,
                              thread_mode);

   return thread_data->return_value;
}

/*
 * void
 * glFlushMappedBufferRangeEXT(GLenum target, GLintptr offset, GLsizeiptr length);
 */

typedef struct
{
   GLenum target;
   GLintptr offset;
   GLsizeiptr length;

} EVGL_API_Thread_Command_glFlushMappedBufferRangeEXT;

void (*orig_evgl_api_glFlushMappedBufferRangeEXT)(GLenum target, GLintptr offset, GLsizeiptr length);

static void
_evgl_api_thread_glFlushMappedBufferRangeEXT(void *data)
{
   EVGL_API_Thread_Command_glFlushMappedBufferRangeEXT *thread_data =
      (EVGL_API_Thread_Command_glFlushMappedBufferRangeEXT *)data;

   orig_evgl_api_glFlushMappedBufferRangeEXT(thread_data->target,
                                             thread_data->offset,
                                             thread_data->length);

}

EAPI void
glFlushMappedBufferRangeEXT_evgl_api_thread_cmd(GLenum target, GLintptr offset, GLsizeiptr length)
{
   if (!evas_evgl_thread_enabled() || (_main_thread_id != eina_thread_self()))
     {
        orig_evgl_api_glFlushMappedBufferRangeEXT(target, offset, length);
        return;
     }

   int thread_mode = EVAS_GL_THREAD_MODE_FINISH;

   EVGL_API_Thread_Command_glFlushMappedBufferRangeEXT thread_data_local;
   EVGL_API_Thread_Command_glFlushMappedBufferRangeEXT *thread_data = &thread_data_local;

   thread_data->target = target;
   thread_data->offset = offset;
   thread_data->length = length;

   evas_gl_thread_cmd_enqueue(EVAS_GL_THREAD_TYPE_EVGL,
                              _evgl_api_thread_glFlushMappedBufferRangeEXT,
                              thread_data,
                              thread_mode);
}

/*
 * void
 * glRenderbufferStorageMultisampleEXT(GLenum a, GLsizei b, GLenum c, GLsizei d, GLsizei e);
 */

typedef struct
{
   GLenum a;
   GLsizei b;
   GLenum c;
   GLsizei d;
   GLsizei e;

} EVGL_API_Thread_Command_glRenderbufferStorageMultisampleEXT;

void (*orig_evgl_api_glRenderbufferStorageMultisampleEXT)(GLenum a, GLsizei b, GLenum c, GLsizei d, GLsizei e);

static void
_evgl_api_thread_glRenderbufferStorageMultisampleEXT(void *data)
{
   EVGL_API_Thread_Command_glRenderbufferStorageMultisampleEXT *thread_data =
      (EVGL_API_Thread_Command_glRenderbufferStorageMultisampleEXT *)data;

   orig_evgl_api_glRenderbufferStorageMultisampleEXT(thread_data->a,
                                                     thread_data->b,
                                                     thread_data->c,
                                                     thread_data->d,
                                                     thread_data->e);

}

EAPI void
glRenderbufferStorageMultisampleEXT_evgl_api_thread_cmd(GLenum a, GLsizei b, GLenum c, GLsizei d, GLsizei e)
{
   if (!evas_evgl_thread_enabled() || (_main_thread_id != eina_thread_self()))
     {
        orig_evgl_api_glRenderbufferStorageMultisampleEXT(a, b, c, d, e);
        return;
     }

   int thread_mode = EVAS_GL_THREAD_MODE_FINISH;

   EVGL_API_Thread_Command_glRenderbufferStorageMultisampleEXT thread_data_local;
   EVGL_API_Thread_Command_glRenderbufferStorageMultisampleEXT *thread_data = &thread_data_local;

   thread_data->a = a;
   thread_data->b = b;
   thread_data->c = c;
   thread_data->d = d;
   thread_data->e = e;

   evas_gl_thread_cmd_enqueue(EVAS_GL_THREAD_TYPE_EVGL,
                              _evgl_api_thread_glRenderbufferStorageMultisampleEXT,
                              thread_data,
                              thread_mode);
}

/*
 * void
 * glFramebufferTexture2DMultisample(GLenum a, GLenum b, GLenum c, GLuint d, GLint e, GLsizei f);
 */

typedef struct
{
   GLenum a;
   GLenum b;
   GLenum c;
   GLuint d;
   GLint e;
   GLsizei f;
   int command_allocated;

} EVGL_API_Thread_Command_glFramebufferTexture2DMultisample;

void (*orig_evgl_api_glFramebufferTexture2DMultisample)(GLenum a, GLenum b, GLenum c, GLuint d, GLint e, GLsizei f);

static void
_evgl_api_thread_glFramebufferTexture2DMultisample(void *data)
{
   EVGL_API_Thread_Command_glFramebufferTexture2DMultisample *thread_data =
      (EVGL_API_Thread_Command_glFramebufferTexture2DMultisample *)data;

   orig_evgl_api_glFramebufferTexture2DMultisample(thread_data->a,
                                                   thread_data->b,
                                                   thread_data->c,
                                                   thread_data->d,
                                                   thread_data->e,
                                                   thread_data->f);

   if (thread_data->command_allocated)
     eina_mempool_free(_mp_command, thread_data);
}

EAPI void
glFramebufferTexture2DMultisample_evgl_api_thread_cmd(GLenum a, GLenum b, GLenum c, GLuint d, GLint e, GLsizei f)
{
   if (!evas_evgl_thread_enabled() || (_main_thread_id != eina_thread_self()))
     {
        orig_evgl_api_glFramebufferTexture2DMultisample(a, b, c, d, e, f);
        return;
     }

   int thread_mode = EVAS_GL_THREAD_MODE_FINISH;

   EVGL_API_Thread_Command_glFramebufferTexture2DMultisample thread_data_local;
   EVGL_API_Thread_Command_glFramebufferTexture2DMultisample *thread_data = &thread_data_local;

   /* command_allocated flag init. */
   thread_data->command_allocated = 0;

   if (!evas_gl_thread_force_finish())
     { /* _flush */
        EVGL_API_Thread_Command_glFramebufferTexture2DMultisample *thread_data_new;
        thread_data_new = eina_mempool_malloc(_mp_command,
                                              sizeof(EVGL_API_Thread_Command_glFramebufferTexture2DMultisample));
        if (thread_data_new)
          {
             thread_data = thread_data_new;
             thread_data->command_allocated = 1;
             thread_mode = EVAS_GL_THREAD_MODE_FLUSH;
          }
     }

   thread_data->a = a;
   thread_data->b = b;
   thread_data->c = c;
   thread_data->d = d;
   thread_data->e = e;
   thread_data->f = f;

   evas_gl_thread_cmd_enqueue(EVAS_GL_THREAD_TYPE_EVGL,
                              _evgl_api_thread_glFramebufferTexture2DMultisample,
                              thread_data,
                              thread_mode);
}

/*
 * void
 * glFramebufferTexture2DMultisampleEXT(GLenum a, GLenum b, GLenum c, GLuint d, GLint e, GLsizei f);
 */

typedef struct
{
   GLenum a;
   GLenum b;
   GLenum c;
   GLuint d;
   GLint e;
   GLsizei f;
   int command_allocated;

} EVGL_API_Thread_Command_glFramebufferTexture2DMultisampleEXT;

void (*orig_evgl_api_glFramebufferTexture2DMultisampleEXT)(GLenum a, GLenum b, GLenum c, GLuint d, GLint e, GLsizei f);

static void
_evgl_api_thread_glFramebufferTexture2DMultisampleEXT(void *data)
{
   EVGL_API_Thread_Command_glFramebufferTexture2DMultisampleEXT *thread_data =
      (EVGL_API_Thread_Command_glFramebufferTexture2DMultisampleEXT *)data;

   orig_evgl_api_glFramebufferTexture2DMultisampleEXT(thread_data->a,
                                                      thread_data->b,
                                                      thread_data->c,
                                                      thread_data->d,
                                                      thread_data->e,
                                                      thread_data->f);

   if (thread_data->command_allocated)
     eina_mempool_free(_mp_command, thread_data);
}

EAPI void
glFramebufferTexture2DMultisampleEXT_evgl_api_thread_cmd(GLenum a, GLenum b, GLenum c, GLuint d, GLint e, GLsizei f)
{
   if (!evas_evgl_thread_enabled() || (_main_thread_id != eina_thread_self()))
     {
        orig_evgl_api_glFramebufferTexture2DMultisampleEXT(a, b, c, d, e, f);
        return;
     }

   int thread_mode = EVAS_GL_THREAD_MODE_FINISH;

   EVGL_API_Thread_Command_glFramebufferTexture2DMultisampleEXT thread_data_local;
   EVGL_API_Thread_Command_glFramebufferTexture2DMultisampleEXT *thread_data = &thread_data_local;

   /* command_allocated flag init. */
   thread_data->command_allocated = 0;

   if (!evas_gl_thread_force_finish())
     { /* _flush */
        EVGL_API_Thread_Command_glFramebufferTexture2DMultisampleEXT *thread_data_new;
        thread_data_new = eina_mempool_malloc(_mp_command,
                                              sizeof(EVGL_API_Thread_Command_glFramebufferTexture2DMultisampleEXT));
        if (thread_data_new)
          {
             thread_data = thread_data_new;
             thread_data->command_allocated = 1;
             thread_mode = EVAS_GL_THREAD_MODE_FLUSH;
          }
     }

   thread_data->a = a;
   thread_data->b = b;
   thread_data->c = c;
   thread_data->d = d;
   thread_data->e = e;
   thread_data->f = f;

   evas_gl_thread_cmd_enqueue(EVAS_GL_THREAD_TYPE_EVGL,
                              _evgl_api_thread_glFramebufferTexture2DMultisampleEXT,
                              thread_data,
                              thread_mode);
}

/*
 * GLenum
 * glGetGraphicsResetStatus(void);
 */

typedef struct
{
   GLenum return_value;

} EVGL_API_Thread_Command_glGetGraphicsResetStatus;

GLenum (*orig_evgl_api_glGetGraphicsResetStatus)(void);

static void
_evgl_api_thread_glGetGraphicsResetStatus(void *data)
{
   EVGL_API_Thread_Command_glGetGraphicsResetStatus *thread_data =
      (EVGL_API_Thread_Command_glGetGraphicsResetStatus *)data;

   thread_data->return_value = orig_evgl_api_glGetGraphicsResetStatus();

}

EAPI GLenum
glGetGraphicsResetStatus_evgl_api_thread_cmd(void)
{
   if (!evas_evgl_thread_enabled() || (_main_thread_id != eina_thread_self()))
     {
        return orig_evgl_api_glGetGraphicsResetStatus();
     }

   int thread_mode = EVAS_GL_THREAD_MODE_FINISH;

   EVGL_API_Thread_Command_glGetGraphicsResetStatus thread_data_local;
   EVGL_API_Thread_Command_glGetGraphicsResetStatus *thread_data = &thread_data_local;


   evas_gl_thread_cmd_enqueue(EVAS_GL_THREAD_TYPE_EVGL,
                              _evgl_api_thread_glGetGraphicsResetStatus,
                              thread_data,
                              thread_mode);

   return thread_data->return_value;
}

/*
 * GLenum
 * glGetGraphicsResetStatusEXT(void);
 */

typedef struct
{
   GLenum return_value;

} EVGL_API_Thread_Command_glGetGraphicsResetStatusEXT;

GLenum (*orig_evgl_api_glGetGraphicsResetStatusEXT)(void);

static void
_evgl_api_thread_glGetGraphicsResetStatusEXT(void *data)
{
   EVGL_API_Thread_Command_glGetGraphicsResetStatusEXT *thread_data =
      (EVGL_API_Thread_Command_glGetGraphicsResetStatusEXT *)data;

   thread_data->return_value = orig_evgl_api_glGetGraphicsResetStatusEXT();

}

EAPI GLenum
glGetGraphicsResetStatusEXT_evgl_api_thread_cmd(void)
{
   if (!evas_evgl_thread_enabled() || (_main_thread_id != eina_thread_self()))
     {
        return orig_evgl_api_glGetGraphicsResetStatusEXT();
     }

   int thread_mode = EVAS_GL_THREAD_MODE_FINISH;

   EVGL_API_Thread_Command_glGetGraphicsResetStatusEXT thread_data_local;
   EVGL_API_Thread_Command_glGetGraphicsResetStatusEXT *thread_data = &thread_data_local;


   evas_gl_thread_cmd_enqueue(EVAS_GL_THREAD_TYPE_EVGL,
                              _evgl_api_thread_glGetGraphicsResetStatusEXT,
                              thread_data,
                              thread_mode);

   return thread_data->return_value;
}

/*
 * void
 * glReadnPixels(GLint x, GLint y, GLsizei width, GLsizei height, GLenum format, GLenum type, GLsizei bufSize, void *data);
 */

typedef struct
{
   GLint x;
   GLint y;
   GLsizei width;
   GLsizei height;
   GLenum format;
   GLenum type;
   GLsizei bufSize;
   void *data;

} EVGL_API_Thread_Command_glReadnPixels;

void (*orig_evgl_api_glReadnPixels)(GLint x, GLint y, GLsizei width, GLsizei height, GLenum format, GLenum type, GLsizei bufSize, void *data);

static void
_evgl_api_thread_glReadnPixels(void *data)
{
   EVGL_API_Thread_Command_glReadnPixels *thread_data =
      (EVGL_API_Thread_Command_glReadnPixels *)data;

   orig_evgl_api_glReadnPixels(thread_data->x,
                               thread_data->y,
                               thread_data->width,
                               thread_data->height,
                               thread_data->format,
                               thread_data->type,
                               thread_data->bufSize,
                               thread_data->data);

}

EAPI void
glReadnPixels_evgl_api_thread_cmd(GLint x, GLint y, GLsizei width, GLsizei height, GLenum format, GLenum type, GLsizei bufSize, void *data)
{
   if (!evas_evgl_thread_enabled() || (_main_thread_id != eina_thread_self()))
     {
        orig_evgl_api_glReadnPixels(x, y, width, height, format, type, bufSize, data);
        return;
     }

   int thread_mode = EVAS_GL_THREAD_MODE_FINISH;

   EVGL_API_Thread_Command_glReadnPixels thread_data_local;
   EVGL_API_Thread_Command_glReadnPixels *thread_data = &thread_data_local;

   thread_data->x = x;
   thread_data->y = y;
   thread_data->width = width;
   thread_data->height = height;
   thread_data->format = format;
   thread_data->type = type;
   thread_data->bufSize = bufSize;
   thread_data->data = data;

   evas_gl_thread_cmd_enqueue(EVAS_GL_THREAD_TYPE_EVGL,
                              _evgl_api_thread_glReadnPixels,
                              thread_data,
                              thread_mode);
}

/*
 * void
 * glReadnPixelsEXT(GLint x, GLint y, GLsizei width, GLsizei height, GLenum format, GLenum type, GLsizei bufSize, void *data);
 */

typedef struct
{
   GLint x;
   GLint y;
   GLsizei width;
   GLsizei height;
   GLenum format;
   GLenum type;
   GLsizei bufSize;
   void *data;

} EVGL_API_Thread_Command_glReadnPixelsEXT;

void (*orig_evgl_api_glReadnPixelsEXT)(GLint x, GLint y, GLsizei width, GLsizei height, GLenum format, GLenum type, GLsizei bufSize, void *data);

static void
_evgl_api_thread_glReadnPixelsEXT(void *data)
{
   EVGL_API_Thread_Command_glReadnPixelsEXT *thread_data =
      (EVGL_API_Thread_Command_glReadnPixelsEXT *)data;

   orig_evgl_api_glReadnPixelsEXT(thread_data->x,
                                  thread_data->y,
                                  thread_data->width,
                                  thread_data->height,
                                  thread_data->format,
                                  thread_data->type,
                                  thread_data->bufSize,
                                  thread_data->data);

}

EAPI void
glReadnPixelsEXT_evgl_api_thread_cmd(GLint x, GLint y, GLsizei width, GLsizei height, GLenum format, GLenum type, GLsizei bufSize, void *data)
{
   if (!evas_evgl_thread_enabled() || (_main_thread_id != eina_thread_self()))
     {
        orig_evgl_api_glReadnPixelsEXT(x, y, width, height, format, type, bufSize, data);
        return;
     }

   int thread_mode = EVAS_GL_THREAD_MODE_FINISH;

   EVGL_API_Thread_Command_glReadnPixelsEXT thread_data_local;
   EVGL_API_Thread_Command_glReadnPixelsEXT *thread_data = &thread_data_local;

   thread_data->x = x;
   thread_data->y = y;
   thread_data->width = width;
   thread_data->height = height;
   thread_data->format = format;
   thread_data->type = type;
   thread_data->bufSize = bufSize;
   thread_data->data = data;

   evas_gl_thread_cmd_enqueue(EVAS_GL_THREAD_TYPE_EVGL,
                              _evgl_api_thread_glReadnPixelsEXT,
                              thread_data,
                              thread_mode);
}

/*
 * void
 * glGetnUniformfv(GLuint program, GLint location, GLsizei bufSize, float *params);
 */

typedef struct
{
   GLuint program;
   GLint location;
   GLsizei bufSize;
   float *params;

} EVGL_API_Thread_Command_glGetnUniformfv;

void (*orig_evgl_api_glGetnUniformfv)(GLuint program, GLint location, GLsizei bufSize, float *params);

static void
_evgl_api_thread_glGetnUniformfv(void *data)
{
   EVGL_API_Thread_Command_glGetnUniformfv *thread_data =
      (EVGL_API_Thread_Command_glGetnUniformfv *)data;

   orig_evgl_api_glGetnUniformfv(thread_data->program,
                                 thread_data->location,
                                 thread_data->bufSize,
                                 thread_data->params);

}

EAPI void
glGetnUniformfv_evgl_api_thread_cmd(GLuint program, GLint location, GLsizei bufSize, float *params)
{
   if (!evas_evgl_thread_enabled() || (_main_thread_id != eina_thread_self()))
     {
        orig_evgl_api_glGetnUniformfv(program, location, bufSize, params);
        return;
     }

   int thread_mode = EVAS_GL_THREAD_MODE_FINISH;

   EVGL_API_Thread_Command_glGetnUniformfv thread_data_local;
   EVGL_API_Thread_Command_glGetnUniformfv *thread_data = &thread_data_local;

   thread_data->program = program;
   thread_data->location = location;
   thread_data->bufSize = bufSize;
   thread_data->params = params;

   evas_gl_thread_cmd_enqueue(EVAS_GL_THREAD_TYPE_EVGL,
                              _evgl_api_thread_glGetnUniformfv,
                              thread_data,
                              thread_mode);
}

/*
 * void
 * glGetnUniformfvEXT(GLuint program, GLint location, GLsizei bufSize, float *params);
 */

typedef struct
{
   GLuint program;
   GLint location;
   GLsizei bufSize;
   float *params;

} EVGL_API_Thread_Command_glGetnUniformfvEXT;

void (*orig_evgl_api_glGetnUniformfvEXT)(GLuint program, GLint location, GLsizei bufSize, float *params);

static void
_evgl_api_thread_glGetnUniformfvEXT(void *data)
{
   EVGL_API_Thread_Command_glGetnUniformfvEXT *thread_data =
      (EVGL_API_Thread_Command_glGetnUniformfvEXT *)data;

   orig_evgl_api_glGetnUniformfvEXT(thread_data->program,
                                    thread_data->location,
                                    thread_data->bufSize,
                                    thread_data->params);

}

EAPI void
glGetnUniformfvEXT_evgl_api_thread_cmd(GLuint program, GLint location, GLsizei bufSize, float *params)
{
   if (!evas_evgl_thread_enabled() || (_main_thread_id != eina_thread_self()))
     {
        orig_evgl_api_glGetnUniformfvEXT(program, location, bufSize, params);
        return;
     }

   int thread_mode = EVAS_GL_THREAD_MODE_FINISH;

   EVGL_API_Thread_Command_glGetnUniformfvEXT thread_data_local;
   EVGL_API_Thread_Command_glGetnUniformfvEXT *thread_data = &thread_data_local;

   thread_data->program = program;
   thread_data->location = location;
   thread_data->bufSize = bufSize;
   thread_data->params = params;

   evas_gl_thread_cmd_enqueue(EVAS_GL_THREAD_TYPE_EVGL,
                              _evgl_api_thread_glGetnUniformfvEXT,
                              thread_data,
                              thread_mode);
}

/*
 * void
 * glGetnUniformiv(GLuint program, GLint location, GLsizei bufSize, GLint *params);
 */

typedef struct
{
   GLuint program;
   GLint location;
   GLsizei bufSize;
   GLint *params;

} EVGL_API_Thread_Command_glGetnUniformiv;

void (*orig_evgl_api_glGetnUniformiv)(GLuint program, GLint location, GLsizei bufSize, GLint *params);

static void
_evgl_api_thread_glGetnUniformiv(void *data)
{
   EVGL_API_Thread_Command_glGetnUniformiv *thread_data =
      (EVGL_API_Thread_Command_glGetnUniformiv *)data;

   orig_evgl_api_glGetnUniformiv(thread_data->program,
                                 thread_data->location,
                                 thread_data->bufSize,
                                 thread_data->params);

}

EAPI void
glGetnUniformiv_evgl_api_thread_cmd(GLuint program, GLint location, GLsizei bufSize, GLint *params)
{
   if (!evas_evgl_thread_enabled() || (_main_thread_id != eina_thread_self()))
     {
        orig_evgl_api_glGetnUniformiv(program, location, bufSize, params);
        return;
     }

   int thread_mode = EVAS_GL_THREAD_MODE_FINISH;

   EVGL_API_Thread_Command_glGetnUniformiv thread_data_local;
   EVGL_API_Thread_Command_glGetnUniformiv *thread_data = &thread_data_local;

   thread_data->program = program;
   thread_data->location = location;
   thread_data->bufSize = bufSize;
   thread_data->params = params;

   evas_gl_thread_cmd_enqueue(EVAS_GL_THREAD_TYPE_EVGL,
                              _evgl_api_thread_glGetnUniformiv,
                              thread_data,
                              thread_mode);
}

/*
 * void
 * glGetnUniformivEXT(GLuint program, GLint location, GLsizei bufSize, GLint *params);
 */

typedef struct
{
   GLuint program;
   GLint location;
   GLsizei bufSize;
   GLint *params;

} EVGL_API_Thread_Command_glGetnUniformivEXT;

void (*orig_evgl_api_glGetnUniformivEXT)(GLuint program, GLint location, GLsizei bufSize, GLint *params);

static void
_evgl_api_thread_glGetnUniformivEXT(void *data)
{
   EVGL_API_Thread_Command_glGetnUniformivEXT *thread_data =
      (EVGL_API_Thread_Command_glGetnUniformivEXT *)data;

   orig_evgl_api_glGetnUniformivEXT(thread_data->program,
                                    thread_data->location,
                                    thread_data->bufSize,
                                    thread_data->params);

}

EAPI void
glGetnUniformivEXT_evgl_api_thread_cmd(GLuint program, GLint location, GLsizei bufSize, GLint *params)
{
   if (!evas_evgl_thread_enabled() || (_main_thread_id != eina_thread_self()))
     {
        orig_evgl_api_glGetnUniformivEXT(program, location, bufSize, params);
        return;
     }

   int thread_mode = EVAS_GL_THREAD_MODE_FINISH;

   EVGL_API_Thread_Command_glGetnUniformivEXT thread_data_local;
   EVGL_API_Thread_Command_glGetnUniformivEXT *thread_data = &thread_data_local;

   thread_data->program = program;
   thread_data->location = location;
   thread_data->bufSize = bufSize;
   thread_data->params = params;

   evas_gl_thread_cmd_enqueue(EVAS_GL_THREAD_TYPE_EVGL,
                              _evgl_api_thread_glGetnUniformivEXT,
                              thread_data,
                              thread_mode);
}

/*
 * void
 * glTexStorage1DEXT(GLenum target, GLsizei levels, GLenum internalformat, GLsizei width);
 */

typedef struct
{
   GLenum target;
   GLsizei levels;
   GLenum internalformat;
   GLsizei width;

} EVGL_API_Thread_Command_glTexStorage1DEXT;

void (*orig_evgl_api_glTexStorage1DEXT)(GLenum target, GLsizei levels, GLenum internalformat, GLsizei width);

static void
_evgl_api_thread_glTexStorage1DEXT(void *data)
{
   EVGL_API_Thread_Command_glTexStorage1DEXT *thread_data =
      (EVGL_API_Thread_Command_glTexStorage1DEXT *)data;

   orig_evgl_api_glTexStorage1DEXT(thread_data->target,
                                   thread_data->levels,
                                   thread_data->internalformat,
                                   thread_data->width);

}

EAPI void
glTexStorage1DEXT_evgl_api_thread_cmd(GLenum target, GLsizei levels, GLenum internalformat, GLsizei width)
{
   if (!evas_evgl_thread_enabled() || (_main_thread_id != eina_thread_self()))
     {
        orig_evgl_api_glTexStorage1DEXT(target, levels, internalformat, width);
        return;
     }

   int thread_mode = EVAS_GL_THREAD_MODE_FINISH;

   EVGL_API_Thread_Command_glTexStorage1DEXT thread_data_local;
   EVGL_API_Thread_Command_glTexStorage1DEXT *thread_data = &thread_data_local;

   thread_data->target = target;
   thread_data->levels = levels;
   thread_data->internalformat = internalformat;
   thread_data->width = width;

   evas_gl_thread_cmd_enqueue(EVAS_GL_THREAD_TYPE_EVGL,
                              _evgl_api_thread_glTexStorage1DEXT,
                              thread_data,
                              thread_mode);
}

/*
 * void
 * glTexStorage2DEXT(GLenum target, GLsizei levels, GLenum internalformat, GLsizei width, GLsizei height);
 */

typedef struct
{
   GLenum target;
   GLsizei levels;
   GLenum internalformat;
   GLsizei width;
   GLsizei height;

} EVGL_API_Thread_Command_glTexStorage2DEXT;

void (*orig_evgl_api_glTexStorage2DEXT)(GLenum target, GLsizei levels, GLenum internalformat, GLsizei width, GLsizei height);

static void
_evgl_api_thread_glTexStorage2DEXT(void *data)
{
   EVGL_API_Thread_Command_glTexStorage2DEXT *thread_data =
      (EVGL_API_Thread_Command_glTexStorage2DEXT *)data;

   orig_evgl_api_glTexStorage2DEXT(thread_data->target,
                                   thread_data->levels,
                                   thread_data->internalformat,
                                   thread_data->width,
                                   thread_data->height);

}

EAPI void
glTexStorage2DEXT_evgl_api_thread_cmd(GLenum target, GLsizei levels, GLenum internalformat, GLsizei width, GLsizei height)
{
   if (!evas_evgl_thread_enabled() || (_main_thread_id != eina_thread_self()))
     {
        orig_evgl_api_glTexStorage2DEXT(target, levels, internalformat, width, height);
        return;
     }

   int thread_mode = EVAS_GL_THREAD_MODE_FINISH;

   EVGL_API_Thread_Command_glTexStorage2DEXT thread_data_local;
   EVGL_API_Thread_Command_glTexStorage2DEXT *thread_data = &thread_data_local;

   thread_data->target = target;
   thread_data->levels = levels;
   thread_data->internalformat = internalformat;
   thread_data->width = width;
   thread_data->height = height;

   evas_gl_thread_cmd_enqueue(EVAS_GL_THREAD_TYPE_EVGL,
                              _evgl_api_thread_glTexStorage2DEXT,
                              thread_data,
                              thread_mode);
}

/*
 * void
 * glTexStorage3DEXT(GLenum target, GLsizei levels, GLenum internalformat, GLsizei width, GLsizei height, GLsizei depth);
 */

typedef struct
{
   GLenum target;
   GLsizei levels;
   GLenum internalformat;
   GLsizei width;
   GLsizei height;
   GLsizei depth;

} EVGL_API_Thread_Command_glTexStorage3DEXT;

void (*orig_evgl_api_glTexStorage3DEXT)(GLenum target, GLsizei levels, GLenum internalformat, GLsizei width, GLsizei height, GLsizei depth);

static void
_evgl_api_thread_glTexStorage3DEXT(void *data)
{
   EVGL_API_Thread_Command_glTexStorage3DEXT *thread_data =
      (EVGL_API_Thread_Command_glTexStorage3DEXT *)data;

   orig_evgl_api_glTexStorage3DEXT(thread_data->target,
                                   thread_data->levels,
                                   thread_data->internalformat,
                                   thread_data->width,
                                   thread_data->height,
                                   thread_data->depth);

}

EAPI void
glTexStorage3DEXT_evgl_api_thread_cmd(GLenum target, GLsizei levels, GLenum internalformat, GLsizei width, GLsizei height, GLsizei depth)
{
   if (!evas_evgl_thread_enabled() || (_main_thread_id != eina_thread_self()))
     {
        orig_evgl_api_glTexStorage3DEXT(target, levels, internalformat, width, height, depth);
        return;
     }

   int thread_mode = EVAS_GL_THREAD_MODE_FINISH;

   EVGL_API_Thread_Command_glTexStorage3DEXT thread_data_local;
   EVGL_API_Thread_Command_glTexStorage3DEXT *thread_data = &thread_data_local;

   thread_data->target = target;
   thread_data->levels = levels;
   thread_data->internalformat = internalformat;
   thread_data->width = width;
   thread_data->height = height;
   thread_data->depth = depth;

   evas_gl_thread_cmd_enqueue(EVAS_GL_THREAD_TYPE_EVGL,
                              _evgl_api_thread_glTexStorage3DEXT,
                              thread_data,
                              thread_mode);
}

/*
 * void
 * glTextureStorage1DEXT(GLuint texture, GLenum target, GLsizei levels, GLenum internalformat, GLsizei width);
 */

typedef struct
{
   GLuint texture;
   GLenum target;
   GLsizei levels;
   GLenum internalformat;
   GLsizei width;

} EVGL_API_Thread_Command_glTextureStorage1DEXT;

void (*orig_evgl_api_glTextureStorage1DEXT)(GLuint texture, GLenum target, GLsizei levels, GLenum internalformat, GLsizei width);

static void
_evgl_api_thread_glTextureStorage1DEXT(void *data)
{
   EVGL_API_Thread_Command_glTextureStorage1DEXT *thread_data =
      (EVGL_API_Thread_Command_glTextureStorage1DEXT *)data;

   orig_evgl_api_glTextureStorage1DEXT(thread_data->texture,
                                       thread_data->target,
                                       thread_data->levels,
                                       thread_data->internalformat,
                                       thread_data->width);

}

EAPI void
glTextureStorage1DEXT_evgl_api_thread_cmd(GLuint texture, GLenum target, GLsizei levels, GLenum internalformat, GLsizei width)
{
   if (!evas_evgl_thread_enabled() || (_main_thread_id != eina_thread_self()))
     {
        orig_evgl_api_glTextureStorage1DEXT(texture, target, levels, internalformat, width);
        return;
     }

   int thread_mode = EVAS_GL_THREAD_MODE_FINISH;

   EVGL_API_Thread_Command_glTextureStorage1DEXT thread_data_local;
   EVGL_API_Thread_Command_glTextureStorage1DEXT *thread_data = &thread_data_local;

   thread_data->texture = texture;
   thread_data->target = target;
   thread_data->levels = levels;
   thread_data->internalformat = internalformat;
   thread_data->width = width;

   evas_gl_thread_cmd_enqueue(EVAS_GL_THREAD_TYPE_EVGL,
                              _evgl_api_thread_glTextureStorage1DEXT,
                              thread_data,
                              thread_mode);
}

/*
 * void
 * glTextureStorage2DEXT(GLuint texture, GLenum target, GLsizei levels, GLenum internalformat, GLsizei width, GLsizei height);
 */

typedef struct
{
   GLuint texture;
   GLenum target;
   GLsizei levels;
   GLenum internalformat;
   GLsizei width;
   GLsizei height;

} EVGL_API_Thread_Command_glTextureStorage2DEXT;

void (*orig_evgl_api_glTextureStorage2DEXT)(GLuint texture, GLenum target, GLsizei levels, GLenum internalformat, GLsizei width, GLsizei height);

static void
_evgl_api_thread_glTextureStorage2DEXT(void *data)
{
   EVGL_API_Thread_Command_glTextureStorage2DEXT *thread_data =
      (EVGL_API_Thread_Command_glTextureStorage2DEXT *)data;

   orig_evgl_api_glTextureStorage2DEXT(thread_data->texture,
                                       thread_data->target,
                                       thread_data->levels,
                                       thread_data->internalformat,
                                       thread_data->width,
                                       thread_data->height);

}

EAPI void
glTextureStorage2DEXT_evgl_api_thread_cmd(GLuint texture, GLenum target, GLsizei levels, GLenum internalformat, GLsizei width, GLsizei height)
{
   if (!evas_evgl_thread_enabled() || (_main_thread_id != eina_thread_self()))
     {
        orig_evgl_api_glTextureStorage2DEXT(texture, target, levels, internalformat, width, height);
        return;
     }

   int thread_mode = EVAS_GL_THREAD_MODE_FINISH;

   EVGL_API_Thread_Command_glTextureStorage2DEXT thread_data_local;
   EVGL_API_Thread_Command_glTextureStorage2DEXT *thread_data = &thread_data_local;

   thread_data->texture = texture;
   thread_data->target = target;
   thread_data->levels = levels;
   thread_data->internalformat = internalformat;
   thread_data->width = width;
   thread_data->height = height;

   evas_gl_thread_cmd_enqueue(EVAS_GL_THREAD_TYPE_EVGL,
                              _evgl_api_thread_glTextureStorage2DEXT,
                              thread_data,
                              thread_mode);
}

/*
 * void
 * glTextureStorage3DEXT(GLuint texture, GLenum target, GLsizei levels, GLenum internalformat, GLsizei width, GLsizei height, GLsizei depth);
 */

typedef struct
{
   GLuint texture;
   GLenum target;
   GLsizei levels;
   GLenum internalformat;
   GLsizei width;
   GLsizei height;
   GLsizei depth;

} EVGL_API_Thread_Command_glTextureStorage3DEXT;

void (*orig_evgl_api_glTextureStorage3DEXT)(GLuint texture, GLenum target, GLsizei levels, GLenum internalformat, GLsizei width, GLsizei height, GLsizei depth);

static void
_evgl_api_thread_glTextureStorage3DEXT(void *data)
{
   EVGL_API_Thread_Command_glTextureStorage3DEXT *thread_data =
      (EVGL_API_Thread_Command_glTextureStorage3DEXT *)data;

   orig_evgl_api_glTextureStorage3DEXT(thread_data->texture,
                                       thread_data->target,
                                       thread_data->levels,
                                       thread_data->internalformat,
                                       thread_data->width,
                                       thread_data->height,
                                       thread_data->depth);

}

EAPI void
glTextureStorage3DEXT_evgl_api_thread_cmd(GLuint texture, GLenum target, GLsizei levels, GLenum internalformat, GLsizei width, GLsizei height, GLsizei depth)
{
   if (!evas_evgl_thread_enabled() || (_main_thread_id != eina_thread_self()))
     {
        orig_evgl_api_glTextureStorage3DEXT(texture, target, levels, internalformat, width, height, depth);
        return;
     }

   int thread_mode = EVAS_GL_THREAD_MODE_FINISH;

   EVGL_API_Thread_Command_glTextureStorage3DEXT thread_data_local;
   EVGL_API_Thread_Command_glTextureStorage3DEXT *thread_data = &thread_data_local;

   thread_data->texture = texture;
   thread_data->target = target;
   thread_data->levels = levels;
   thread_data->internalformat = internalformat;
   thread_data->width = width;
   thread_data->height = height;
   thread_data->depth = depth;

   evas_gl_thread_cmd_enqueue(EVAS_GL_THREAD_TYPE_EVGL,
                              _evgl_api_thread_glTextureStorage3DEXT,
                              thread_data,
                              thread_mode);
}

/*
 * void
 * glClipPlanefIMG(GLenum a, const GLfloat * b);
 */

typedef struct
{
   GLenum a;
   const GLfloat * b;

} EVGL_API_Thread_Command_glClipPlanefIMG;

void (*orig_evgl_api_glClipPlanefIMG)(GLenum a, const GLfloat * b);

static void
_evgl_api_thread_glClipPlanefIMG(void *data)
{
   EVGL_API_Thread_Command_glClipPlanefIMG *thread_data =
      (EVGL_API_Thread_Command_glClipPlanefIMG *)data;

   orig_evgl_api_glClipPlanefIMG(thread_data->a,
                                 thread_data->b);

}

EAPI void
glClipPlanefIMG_evgl_api_thread_cmd(GLenum a, const GLfloat * b)
{
   if (!evas_evgl_thread_enabled() || (_main_thread_id != eina_thread_self()))
     {
        orig_evgl_api_glClipPlanefIMG(a, b);
        return;
     }

   int thread_mode = EVAS_GL_THREAD_MODE_FINISH;

   EVGL_API_Thread_Command_glClipPlanefIMG thread_data_local;
   EVGL_API_Thread_Command_glClipPlanefIMG *thread_data = &thread_data_local;

   thread_data->a = a;
   thread_data->b = b;

   evas_gl_thread_cmd_enqueue(EVAS_GL_THREAD_TYPE_EVGL,
                              _evgl_api_thread_glClipPlanefIMG,
                              thread_data,
                              thread_mode);
}

/*
 * void
 * glClipPlanexIMG(GLenum a, const GLfixed * b);
 */

typedef struct
{
   GLenum a;
   const GLfixed * b;

} EVGL_API_Thread_Command_glClipPlanexIMG;

void (*orig_evgl_api_glClipPlanexIMG)(GLenum a, const GLfixed * b);

static void
_evgl_api_thread_glClipPlanexIMG(void *data)
{
   EVGL_API_Thread_Command_glClipPlanexIMG *thread_data =
      (EVGL_API_Thread_Command_glClipPlanexIMG *)data;

   orig_evgl_api_glClipPlanexIMG(thread_data->a,
                                 thread_data->b);

}

EAPI void
glClipPlanexIMG_evgl_api_thread_cmd(GLenum a, const GLfixed * b)
{
   if (!evas_evgl_thread_enabled() || (_main_thread_id != eina_thread_self()))
     {
        orig_evgl_api_glClipPlanexIMG(a, b);
        return;
     }

   int thread_mode = EVAS_GL_THREAD_MODE_FINISH;

   EVGL_API_Thread_Command_glClipPlanexIMG thread_data_local;
   EVGL_API_Thread_Command_glClipPlanexIMG *thread_data = &thread_data_local;

   thread_data->a = a;
   thread_data->b = b;

   evas_gl_thread_cmd_enqueue(EVAS_GL_THREAD_TYPE_EVGL,
                              _evgl_api_thread_glClipPlanexIMG,
                              thread_data,
                              thread_mode);
}

/*
 * void
 * glRenderbufferStorageMultisampleIMG(GLenum a, GLsizei b, GLenum c, GLsizei d, GLsizei e);
 */

typedef struct
{
   GLenum a;
   GLsizei b;
   GLenum c;
   GLsizei d;
   GLsizei e;
   int command_allocated;

} EVGL_API_Thread_Command_glRenderbufferStorageMultisampleIMG;

void (*orig_evgl_api_glRenderbufferStorageMultisampleIMG)(GLenum a, GLsizei b, GLenum c, GLsizei d, GLsizei e);

static void
_evgl_api_thread_glRenderbufferStorageMultisampleIMG(void *data)
{
   EVGL_API_Thread_Command_glRenderbufferStorageMultisampleIMG *thread_data =
      (EVGL_API_Thread_Command_glRenderbufferStorageMultisampleIMG *)data;

   orig_evgl_api_glRenderbufferStorageMultisampleIMG(thread_data->a,
                                                     thread_data->b,
                                                     thread_data->c,
                                                     thread_data->d,
                                                     thread_data->e);

   if (thread_data->command_allocated)
     eina_mempool_free(_mp_command, thread_data);
}

EAPI void
glRenderbufferStorageMultisampleIMG_evgl_api_thread_cmd(GLenum a, GLsizei b, GLenum c, GLsizei d, GLsizei e)
{
   if (!evas_evgl_thread_enabled() || (_main_thread_id != eina_thread_self()))
     {
        orig_evgl_api_glRenderbufferStorageMultisampleIMG(a, b, c, d, e);
        return;
     }

   int thread_mode = EVAS_GL_THREAD_MODE_FINISH;

   EVGL_API_Thread_Command_glRenderbufferStorageMultisampleIMG thread_data_local;
   EVGL_API_Thread_Command_glRenderbufferStorageMultisampleIMG *thread_data = &thread_data_local;

   /* command_allocated flag init. */
   thread_data->command_allocated = 0;

   if (!evas_gl_thread_force_finish())
     { /* _flush */
        EVGL_API_Thread_Command_glRenderbufferStorageMultisampleIMG *thread_data_new;
        thread_data_new = eina_mempool_malloc(_mp_command,
                                              sizeof(EVGL_API_Thread_Command_glRenderbufferStorageMultisampleIMG));
        if (thread_data_new)
          {
             thread_data = thread_data_new;
             thread_data->command_allocated = 1;
             thread_mode = EVAS_GL_THREAD_MODE_FLUSH;
          }
     }

   thread_data->a = a;
   thread_data->b = b;
   thread_data->c = c;
   thread_data->d = d;
   thread_data->e = e;

   evas_gl_thread_cmd_enqueue(EVAS_GL_THREAD_TYPE_EVGL,
                              _evgl_api_thread_glRenderbufferStorageMultisampleIMG,
                              thread_data,
                              thread_mode);
}

/*
 * void
 * glFramebufferTexture2DMultisampleIMG(GLenum a, GLenum b, GLenum c, GLuint d, GLint e, GLsizei f);
 */

typedef struct
{
   GLenum a;
   GLenum b;
   GLenum c;
   GLuint d;
   GLint e;
   GLsizei f;
   int command_allocated;

} EVGL_API_Thread_Command_glFramebufferTexture2DMultisampleIMG;

void (*orig_evgl_api_glFramebufferTexture2DMultisampleIMG)(GLenum a, GLenum b, GLenum c, GLuint d, GLint e, GLsizei f);

static void
_evgl_api_thread_glFramebufferTexture2DMultisampleIMG(void *data)
{
   EVGL_API_Thread_Command_glFramebufferTexture2DMultisampleIMG *thread_data =
      (EVGL_API_Thread_Command_glFramebufferTexture2DMultisampleIMG *)data;

   orig_evgl_api_glFramebufferTexture2DMultisampleIMG(thread_data->a,
                                                      thread_data->b,
                                                      thread_data->c,
                                                      thread_data->d,
                                                      thread_data->e,
                                                      thread_data->f);

   if (thread_data->command_allocated)
     eina_mempool_free(_mp_command, thread_data);
}

EAPI void
glFramebufferTexture2DMultisampleIMG_evgl_api_thread_cmd(GLenum a, GLenum b, GLenum c, GLuint d, GLint e, GLsizei f)
{
   if (!evas_evgl_thread_enabled() || (_main_thread_id != eina_thread_self()))
     {
        orig_evgl_api_glFramebufferTexture2DMultisampleIMG(a, b, c, d, e, f);
        return;
     }

   int thread_mode = EVAS_GL_THREAD_MODE_FINISH;

   EVGL_API_Thread_Command_glFramebufferTexture2DMultisampleIMG thread_data_local;
   EVGL_API_Thread_Command_glFramebufferTexture2DMultisampleIMG *thread_data = &thread_data_local;

   /* command_allocated flag init. */
   thread_data->command_allocated = 0;

   if (!evas_gl_thread_force_finish())
     { /* _flush */
        EVGL_API_Thread_Command_glFramebufferTexture2DMultisampleIMG *thread_data_new;
        thread_data_new = eina_mempool_malloc(_mp_command,
                                              sizeof(EVGL_API_Thread_Command_glFramebufferTexture2DMultisampleIMG));
        if (thread_data_new)
          {
             thread_data = thread_data_new;
             thread_data->command_allocated = 1;
             thread_mode = EVAS_GL_THREAD_MODE_FLUSH;
          }
     }

   thread_data->a = a;
   thread_data->b = b;
   thread_data->c = c;
   thread_data->d = d;
   thread_data->e = e;
   thread_data->f = f;

   evas_gl_thread_cmd_enqueue(EVAS_GL_THREAD_TYPE_EVGL,
                              _evgl_api_thread_glFramebufferTexture2DMultisampleIMG,
                              thread_data,
                              thread_mode);
}

/*
 * void
 * glStartTilingQCOM(GLuint x, GLuint y, GLuint width, GLuint height, GLbitfield preserveMask);
 */

typedef struct
{
   GLuint x;
   GLuint y;
   GLuint width;
   GLuint height;
   GLbitfield preserveMask;
   int command_allocated;

} EVGL_API_Thread_Command_glStartTilingQCOM;

void (*orig_evgl_api_glStartTilingQCOM)(GLuint x, GLuint y, GLuint width, GLuint height, GLbitfield preserveMask);

static void
_evgl_api_thread_glStartTilingQCOM(void *data)
{
   EVGL_API_Thread_Command_glStartTilingQCOM *thread_data =
      (EVGL_API_Thread_Command_glStartTilingQCOM *)data;

   orig_evgl_api_glStartTilingQCOM(thread_data->x,
                                   thread_data->y,
                                   thread_data->width,
                                   thread_data->height,
                                   thread_data->preserveMask);

   if (thread_data->command_allocated)
     eina_mempool_free(_mp_command, thread_data);
}

EAPI void
glStartTilingQCOM_evgl_api_thread_cmd(GLuint x, GLuint y, GLuint width, GLuint height, GLbitfield preserveMask)
{
   if (!evas_evgl_thread_enabled() || (_main_thread_id != eina_thread_self()))
     {
        orig_evgl_api_glStartTilingQCOM(x, y, width, height, preserveMask);
        return;
     }

   int thread_mode = EVAS_GL_THREAD_MODE_FINISH;

   EVGL_API_Thread_Command_glStartTilingQCOM thread_data_local;
   EVGL_API_Thread_Command_glStartTilingQCOM *thread_data = &thread_data_local;

   /* command_allocated flag init. */
   thread_data->command_allocated = 0;

   if (!evas_gl_thread_force_finish())
     { /* _flush */
        EVGL_API_Thread_Command_glStartTilingQCOM *thread_data_new;
        thread_data_new = eina_mempool_malloc(_mp_command,
                                              sizeof(EVGL_API_Thread_Command_glStartTilingQCOM));
        if (thread_data_new)
          {
             thread_data = thread_data_new;
             thread_data->command_allocated = 1;
             thread_mode = EVAS_GL_THREAD_MODE_FLUSH;
          }
     }

   thread_data->x = x;
   thread_data->y = y;
   thread_data->width = width;
   thread_data->height = height;
   thread_data->preserveMask = preserveMask;

   evas_gl_thread_cmd_enqueue(EVAS_GL_THREAD_TYPE_EVGL,
                              _evgl_api_thread_glStartTilingQCOM,
                              thread_data,
                              thread_mode);
}

/*
 * void
 * glEndTilingQCOM(GLbitfield preserveMask);
 */

typedef struct
{
   GLbitfield preserveMask;
   int command_allocated;

} EVGL_API_Thread_Command_glEndTilingQCOM;

void (*orig_evgl_api_glEndTilingQCOM)(GLbitfield preserveMask);

static void
_evgl_api_thread_glEndTilingQCOM(void *data)
{
   EVGL_API_Thread_Command_glEndTilingQCOM *thread_data =
      (EVGL_API_Thread_Command_glEndTilingQCOM *)data;

   orig_evgl_api_glEndTilingQCOM(thread_data->preserveMask);

   if (thread_data->command_allocated)
     eina_mempool_free(_mp_command, thread_data);
}

EAPI void
glEndTilingQCOM_evgl_api_thread_cmd(GLbitfield preserveMask)
{
   if (!evas_evgl_thread_enabled() || (_main_thread_id != eina_thread_self()))
     {
        orig_evgl_api_glEndTilingQCOM(preserveMask);
        return;
     }

   int thread_mode = EVAS_GL_THREAD_MODE_FINISH;

   EVGL_API_Thread_Command_glEndTilingQCOM thread_data_local;
   EVGL_API_Thread_Command_glEndTilingQCOM *thread_data = &thread_data_local;

   /* command_allocated flag init. */
   thread_data->command_allocated = 0;

   if (!evas_gl_thread_force_finish())
     { /* _flush */
        EVGL_API_Thread_Command_glEndTilingQCOM *thread_data_new;
        thread_data_new = eina_mempool_malloc(_mp_command,
                                              sizeof(EVGL_API_Thread_Command_glEndTilingQCOM));
        if (thread_data_new)
          {
             thread_data = thread_data_new;
             thread_data->command_allocated = 1;
             thread_mode = EVAS_GL_THREAD_MODE_FLUSH;
          }
     }

   thread_data->preserveMask = preserveMask;

   evas_gl_thread_cmd_enqueue(EVAS_GL_THREAD_TYPE_EVGL,
                              _evgl_api_thread_glEndTilingQCOM,
                              thread_data,
                              thread_mode);
}

/*
 * void
 * glBeginQuery(GLenum target, GLuint id);
 */

typedef struct
{
   GLenum target;
   GLuint id;

} EVGL_API_Thread_Command_glBeginQuery;

void (*orig_evgl_api_glBeginQuery)(GLenum target, GLuint id);

static void
_evgl_api_thread_glBeginQuery(void *data)
{
   EVGL_API_Thread_Command_glBeginQuery *thread_data =
      (EVGL_API_Thread_Command_glBeginQuery *)data;

   orig_evgl_api_glBeginQuery(thread_data->target,
                              thread_data->id);

}

EAPI void
glBeginQuery_evgl_api_thread_cmd(GLenum target, GLuint id)
{
   if (!evas_evgl_thread_enabled() || (_main_thread_id != eina_thread_self()))
     {
        orig_evgl_api_glBeginQuery(target, id);
        return;
     }

   int thread_mode = EVAS_GL_THREAD_MODE_FINISH;

   EVGL_API_Thread_Command_glBeginQuery thread_data_local;
   EVGL_API_Thread_Command_glBeginQuery *thread_data = &thread_data_local;

   thread_data->target = target;
   thread_data->id = id;

   evas_gl_thread_cmd_enqueue(EVAS_GL_THREAD_TYPE_EVGL,
                              _evgl_api_thread_glBeginQuery,
                              thread_data,
                              thread_mode);
}

/*
 * void
 * glBeginTransformFeedback(GLenum primitiveMode);
 */

typedef struct
{
   GLenum primitiveMode;

} EVGL_API_Thread_Command_glBeginTransformFeedback;

void (*orig_evgl_api_glBeginTransformFeedback)(GLenum primitiveMode);

static void
_evgl_api_thread_glBeginTransformFeedback(void *data)
{
   EVGL_API_Thread_Command_glBeginTransformFeedback *thread_data =
      (EVGL_API_Thread_Command_glBeginTransformFeedback *)data;

   orig_evgl_api_glBeginTransformFeedback(thread_data->primitiveMode);

}

EAPI void
glBeginTransformFeedback_evgl_api_thread_cmd(GLenum primitiveMode)
{
   if (!evas_evgl_thread_enabled() || (_main_thread_id != eina_thread_self()))
     {
        orig_evgl_api_glBeginTransformFeedback(primitiveMode);
        return;
     }

   int thread_mode = EVAS_GL_THREAD_MODE_FINISH;

   EVGL_API_Thread_Command_glBeginTransformFeedback thread_data_local;
   EVGL_API_Thread_Command_glBeginTransformFeedback *thread_data = &thread_data_local;

   thread_data->primitiveMode = primitiveMode;

   evas_gl_thread_cmd_enqueue(EVAS_GL_THREAD_TYPE_EVGL,
                              _evgl_api_thread_glBeginTransformFeedback,
                              thread_data,
                              thread_mode);
}

/*
 * void
 * glBindBufferBase(GLenum target, GLuint index, GLuint buffer);
 */

typedef struct
{
   GLenum target;
   GLuint index;
   GLuint buffer;

} EVGL_API_Thread_Command_glBindBufferBase;

void (*orig_evgl_api_glBindBufferBase)(GLenum target, GLuint index, GLuint buffer);

static void
_evgl_api_thread_glBindBufferBase(void *data)
{
   EVGL_API_Thread_Command_glBindBufferBase *thread_data =
      (EVGL_API_Thread_Command_glBindBufferBase *)data;

   orig_evgl_api_glBindBufferBase(thread_data->target,
                                  thread_data->index,
                                  thread_data->buffer);

}

EAPI void
glBindBufferBase_evgl_api_thread_cmd(GLenum target, GLuint index, GLuint buffer)
{
   if (!evas_evgl_thread_enabled() || (_main_thread_id != eina_thread_self()))
     {
        orig_evgl_api_glBindBufferBase(target, index, buffer);
        return;
     }

   int thread_mode = EVAS_GL_THREAD_MODE_FINISH;

   EVGL_API_Thread_Command_glBindBufferBase thread_data_local;
   EVGL_API_Thread_Command_glBindBufferBase *thread_data = &thread_data_local;

   thread_data->target = target;
   thread_data->index = index;
   thread_data->buffer = buffer;

   evas_gl_thread_cmd_enqueue(EVAS_GL_THREAD_TYPE_EVGL,
                              _evgl_api_thread_glBindBufferBase,
                              thread_data,
                              thread_mode);
}

/*
 * void
 * glBindBufferRange(GLenum target, GLuint index, GLuint buffer, GLintptr offset, GLsizeiptr size);
 */

typedef struct
{
   GLenum target;
   GLuint index;
   GLuint buffer;
   GLintptr offset;
   GLsizeiptr size;

} EVGL_API_Thread_Command_glBindBufferRange;

void (*orig_evgl_api_glBindBufferRange)(GLenum target, GLuint index, GLuint buffer, GLintptr offset, GLsizeiptr size);

static void
_evgl_api_thread_glBindBufferRange(void *data)
{
   EVGL_API_Thread_Command_glBindBufferRange *thread_data =
      (EVGL_API_Thread_Command_glBindBufferRange *)data;

   orig_evgl_api_glBindBufferRange(thread_data->target,
                                   thread_data->index,
                                   thread_data->buffer,
                                   thread_data->offset,
                                   thread_data->size);

}

EAPI void
glBindBufferRange_evgl_api_thread_cmd(GLenum target, GLuint index, GLuint buffer, GLintptr offset, GLsizeiptr size)
{
   if (!evas_evgl_thread_enabled() || (_main_thread_id != eina_thread_self()))
     {
        orig_evgl_api_glBindBufferRange(target, index, buffer, offset, size);
        return;
     }

   int thread_mode = EVAS_GL_THREAD_MODE_FINISH;

   EVGL_API_Thread_Command_glBindBufferRange thread_data_local;
   EVGL_API_Thread_Command_glBindBufferRange *thread_data = &thread_data_local;

   thread_data->target = target;
   thread_data->index = index;
   thread_data->buffer = buffer;
   thread_data->offset = offset;
   thread_data->size = size;

   evas_gl_thread_cmd_enqueue(EVAS_GL_THREAD_TYPE_EVGL,
                              _evgl_api_thread_glBindBufferRange,
                              thread_data,
                              thread_mode);
}

/*
 * void
 * glBindSampler(GLuint unit, GLuint sampler);
 */

typedef struct
{
   GLuint unit;
   GLuint sampler;

} EVGL_API_Thread_Command_glBindSampler;

void (*orig_evgl_api_glBindSampler)(GLuint unit, GLuint sampler);

static void
_evgl_api_thread_glBindSampler(void *data)
{
   EVGL_API_Thread_Command_glBindSampler *thread_data =
      (EVGL_API_Thread_Command_glBindSampler *)data;

   orig_evgl_api_glBindSampler(thread_data->unit,
                               thread_data->sampler);

}

EAPI void
glBindSampler_evgl_api_thread_cmd(GLuint unit, GLuint sampler)
{
   if (!evas_evgl_thread_enabled() || (_main_thread_id != eina_thread_self()))
     {
        orig_evgl_api_glBindSampler(unit, sampler);
        return;
     }

   int thread_mode = EVAS_GL_THREAD_MODE_FINISH;

   EVGL_API_Thread_Command_glBindSampler thread_data_local;
   EVGL_API_Thread_Command_glBindSampler *thread_data = &thread_data_local;

   thread_data->unit = unit;
   thread_data->sampler = sampler;

   evas_gl_thread_cmd_enqueue(EVAS_GL_THREAD_TYPE_EVGL,
                              _evgl_api_thread_glBindSampler,
                              thread_data,
                              thread_mode);
}

/*
 * void
 * glBindTransformFeedback(GLenum target, GLuint id);
 */

typedef struct
{
   GLenum target;
   GLuint id;

} EVGL_API_Thread_Command_glBindTransformFeedback;

void (*orig_evgl_api_glBindTransformFeedback)(GLenum target, GLuint id);

static void
_evgl_api_thread_glBindTransformFeedback(void *data)
{
   EVGL_API_Thread_Command_glBindTransformFeedback *thread_data =
      (EVGL_API_Thread_Command_glBindTransformFeedback *)data;

   orig_evgl_api_glBindTransformFeedback(thread_data->target,
                                         thread_data->id);

}

EAPI void
glBindTransformFeedback_evgl_api_thread_cmd(GLenum target, GLuint id)
{
   if (!evas_evgl_thread_enabled() || (_main_thread_id != eina_thread_self()))
     {
        orig_evgl_api_glBindTransformFeedback(target, id);
        return;
     }

   int thread_mode = EVAS_GL_THREAD_MODE_FINISH;

   EVGL_API_Thread_Command_glBindTransformFeedback thread_data_local;
   EVGL_API_Thread_Command_glBindTransformFeedback *thread_data = &thread_data_local;

   thread_data->target = target;
   thread_data->id = id;

   evas_gl_thread_cmd_enqueue(EVAS_GL_THREAD_TYPE_EVGL,
                              _evgl_api_thread_glBindTransformFeedback,
                              thread_data,
                              thread_mode);
}

/*
 * void
 * glBindVertexArray(GLuint array);
 */

typedef struct
{
   GLuint array;

} EVGL_API_Thread_Command_glBindVertexArray;

void (*orig_evgl_api_glBindVertexArray)(GLuint array);

static void
_evgl_api_thread_glBindVertexArray(void *data)
{
   EVGL_API_Thread_Command_glBindVertexArray *thread_data =
      (EVGL_API_Thread_Command_glBindVertexArray *)data;

   orig_evgl_api_glBindVertexArray(thread_data->array);

}

EAPI void
glBindVertexArray_evgl_api_thread_cmd(GLuint array)
{
   if (!evas_evgl_thread_enabled() || (_main_thread_id != eina_thread_self()))
     {
        orig_evgl_api_glBindVertexArray(array);
        return;
     }

   int thread_mode = EVAS_GL_THREAD_MODE_FINISH;

   EVGL_API_Thread_Command_glBindVertexArray thread_data_local;
   EVGL_API_Thread_Command_glBindVertexArray *thread_data = &thread_data_local;

   thread_data->array = array;

   evas_gl_thread_cmd_enqueue(EVAS_GL_THREAD_TYPE_EVGL,
                              _evgl_api_thread_glBindVertexArray,
                              thread_data,
                              thread_mode);
}

/*
 * void
 * glBlitFramebuffer(GLint srcX0, GLint srcY0, GLint srcX1, GLint srcY1, GLint dstX0, GLint dstY0, GLint dstX1, GLint dstY1, GLbitfield mask, GLenum filter);
 */

typedef struct
{
   GLint srcX0;
   GLint srcY0;
   GLint srcX1;
   GLint srcY1;
   GLint dstX0;
   GLint dstY0;
   GLint dstX1;
   GLint dstY1;
   GLbitfield mask;
   GLenum filter;

} EVGL_API_Thread_Command_glBlitFramebuffer;

void (*orig_evgl_api_glBlitFramebuffer)(GLint srcX0, GLint srcY0, GLint srcX1, GLint srcY1, GLint dstX0, GLint dstY0, GLint dstX1, GLint dstY1, GLbitfield mask, GLenum filter);

static void
_evgl_api_thread_glBlitFramebuffer(void *data)
{
   EVGL_API_Thread_Command_glBlitFramebuffer *thread_data =
      (EVGL_API_Thread_Command_glBlitFramebuffer *)data;

   orig_evgl_api_glBlitFramebuffer(thread_data->srcX0,
                                   thread_data->srcY0,
                                   thread_data->srcX1,
                                   thread_data->srcY1,
                                   thread_data->dstX0,
                                   thread_data->dstY0,
                                   thread_data->dstX1,
                                   thread_data->dstY1,
                                   thread_data->mask,
                                   thread_data->filter);

}

EAPI void
glBlitFramebuffer_evgl_api_thread_cmd(GLint srcX0, GLint srcY0, GLint srcX1, GLint srcY1, GLint dstX0, GLint dstY0, GLint dstX1, GLint dstY1, GLbitfield mask, GLenum filter)
{
   if (!evas_evgl_thread_enabled() || (_main_thread_id != eina_thread_self()))
     {
        orig_evgl_api_glBlitFramebuffer(srcX0, srcY0, srcX1, srcY1, dstX0, dstY0, dstX1, dstY1, mask, filter);
        return;
     }

   int thread_mode = EVAS_GL_THREAD_MODE_FINISH;

   EVGL_API_Thread_Command_glBlitFramebuffer thread_data_local;
   EVGL_API_Thread_Command_glBlitFramebuffer *thread_data = &thread_data_local;

   thread_data->srcX0 = srcX0;
   thread_data->srcY0 = srcY0;
   thread_data->srcX1 = srcX1;
   thread_data->srcY1 = srcY1;
   thread_data->dstX0 = dstX0;
   thread_data->dstY0 = dstY0;
   thread_data->dstX1 = dstX1;
   thread_data->dstY1 = dstY1;
   thread_data->mask = mask;
   thread_data->filter = filter;

   evas_gl_thread_cmd_enqueue(EVAS_GL_THREAD_TYPE_EVGL,
                              _evgl_api_thread_glBlitFramebuffer,
                              thread_data,
                              thread_mode);
}

/*
 * void
 * glClearBufferfi(GLenum buffer, GLint drawBuffer, GLfloat depth, GLint stencil);
 */

typedef struct
{
   GLenum buffer;
   GLint drawBuffer;
   GLfloat depth;
   GLint stencil;

} EVGL_API_Thread_Command_glClearBufferfi;

void (*orig_evgl_api_glClearBufferfi)(GLenum buffer, GLint drawBuffer, GLfloat depth, GLint stencil);

static void
_evgl_api_thread_glClearBufferfi(void *data)
{
   EVGL_API_Thread_Command_glClearBufferfi *thread_data =
      (EVGL_API_Thread_Command_glClearBufferfi *)data;

   orig_evgl_api_glClearBufferfi(thread_data->buffer,
                                 thread_data->drawBuffer,
                                 thread_data->depth,
                                 thread_data->stencil);

}

EAPI void
glClearBufferfi_evgl_api_thread_cmd(GLenum buffer, GLint drawBuffer, GLfloat depth, GLint stencil)
{
   if (!evas_evgl_thread_enabled() || (_main_thread_id != eina_thread_self()))
     {
        orig_evgl_api_glClearBufferfi(buffer, drawBuffer, depth, stencil);
        return;
     }

   int thread_mode = EVAS_GL_THREAD_MODE_FINISH;

   EVGL_API_Thread_Command_glClearBufferfi thread_data_local;
   EVGL_API_Thread_Command_glClearBufferfi *thread_data = &thread_data_local;

   thread_data->buffer = buffer;
   thread_data->drawBuffer = drawBuffer;
   thread_data->depth = depth;
   thread_data->stencil = stencil;

   evas_gl_thread_cmd_enqueue(EVAS_GL_THREAD_TYPE_EVGL,
                              _evgl_api_thread_glClearBufferfi,
                              thread_data,
                              thread_mode);
}

/*
 * void
 * glClearBufferfv(GLenum buffer, GLint drawBuffer, const GLfloat * value);
 */

typedef struct
{
   GLenum buffer;
   GLint drawBuffer;
   const GLfloat * value;

} EVGL_API_Thread_Command_glClearBufferfv;

void (*orig_evgl_api_glClearBufferfv)(GLenum buffer, GLint drawBuffer, const GLfloat * value);

static void
_evgl_api_thread_glClearBufferfv(void *data)
{
   EVGL_API_Thread_Command_glClearBufferfv *thread_data =
      (EVGL_API_Thread_Command_glClearBufferfv *)data;

   orig_evgl_api_glClearBufferfv(thread_data->buffer,
                                 thread_data->drawBuffer,
                                 thread_data->value);

}

EAPI void
glClearBufferfv_evgl_api_thread_cmd(GLenum buffer, GLint drawBuffer, const GLfloat * value)
{
   if (!evas_evgl_thread_enabled() || (_main_thread_id != eina_thread_self()))
     {
        orig_evgl_api_glClearBufferfv(buffer, drawBuffer, value);
        return;
     }

   int thread_mode = EVAS_GL_THREAD_MODE_FINISH;

   EVGL_API_Thread_Command_glClearBufferfv thread_data_local;
   EVGL_API_Thread_Command_glClearBufferfv *thread_data = &thread_data_local;

   thread_data->buffer = buffer;
   thread_data->drawBuffer = drawBuffer;
   thread_data->value = value;

   evas_gl_thread_cmd_enqueue(EVAS_GL_THREAD_TYPE_EVGL,
                              _evgl_api_thread_glClearBufferfv,
                              thread_data,
                              thread_mode);
}

/*
 * void
 * glClearBufferiv(GLenum buffer, GLint drawBuffer, const GLint * value);
 */

typedef struct
{
   GLenum buffer;
   GLint drawBuffer;
   const GLint * value;

} EVGL_API_Thread_Command_glClearBufferiv;

void (*orig_evgl_api_glClearBufferiv)(GLenum buffer, GLint drawBuffer, const GLint * value);

static void
_evgl_api_thread_glClearBufferiv(void *data)
{
   EVGL_API_Thread_Command_glClearBufferiv *thread_data =
      (EVGL_API_Thread_Command_glClearBufferiv *)data;

   orig_evgl_api_glClearBufferiv(thread_data->buffer,
                                 thread_data->drawBuffer,
                                 thread_data->value);

}

EAPI void
glClearBufferiv_evgl_api_thread_cmd(GLenum buffer, GLint drawBuffer, const GLint * value)
{
   if (!evas_evgl_thread_enabled() || (_main_thread_id != eina_thread_self()))
     {
        orig_evgl_api_glClearBufferiv(buffer, drawBuffer, value);
        return;
     }

   int thread_mode = EVAS_GL_THREAD_MODE_FINISH;

   EVGL_API_Thread_Command_glClearBufferiv thread_data_local;
   EVGL_API_Thread_Command_glClearBufferiv *thread_data = &thread_data_local;

   thread_data->buffer = buffer;
   thread_data->drawBuffer = drawBuffer;
   thread_data->value = value;

   evas_gl_thread_cmd_enqueue(EVAS_GL_THREAD_TYPE_EVGL,
                              _evgl_api_thread_glClearBufferiv,
                              thread_data,
                              thread_mode);
}

/*
 * void
 * glClearBufferuiv(GLenum buffer, GLint drawBuffer, const GLuint * value);
 */

typedef struct
{
   GLenum buffer;
   GLint drawBuffer;
   const GLuint * value;

} EVGL_API_Thread_Command_glClearBufferuiv;

void (*orig_evgl_api_glClearBufferuiv)(GLenum buffer, GLint drawBuffer, const GLuint * value);

static void
_evgl_api_thread_glClearBufferuiv(void *data)
{
   EVGL_API_Thread_Command_glClearBufferuiv *thread_data =
      (EVGL_API_Thread_Command_glClearBufferuiv *)data;

   orig_evgl_api_glClearBufferuiv(thread_data->buffer,
                                  thread_data->drawBuffer,
                                  thread_data->value);

}

EAPI void
glClearBufferuiv_evgl_api_thread_cmd(GLenum buffer, GLint drawBuffer, const GLuint * value)
{
   if (!evas_evgl_thread_enabled() || (_main_thread_id != eina_thread_self()))
     {
        orig_evgl_api_glClearBufferuiv(buffer, drawBuffer, value);
        return;
     }

   int thread_mode = EVAS_GL_THREAD_MODE_FINISH;

   EVGL_API_Thread_Command_glClearBufferuiv thread_data_local;
   EVGL_API_Thread_Command_glClearBufferuiv *thread_data = &thread_data_local;

   thread_data->buffer = buffer;
   thread_data->drawBuffer = drawBuffer;
   thread_data->value = value;

   evas_gl_thread_cmd_enqueue(EVAS_GL_THREAD_TYPE_EVGL,
                              _evgl_api_thread_glClearBufferuiv,
                              thread_data,
                              thread_mode);
}

/*
 * GLenum
 * glClientWaitSync(GLsync sync, GLbitfield flags, EvasGLuint64 timeout);
 */

typedef struct
{
   GLenum return_value;
   GLsync sync;
   GLbitfield flags;
   EvasGLuint64 timeout;

} EVGL_API_Thread_Command_glClientWaitSync;

GLenum (*orig_evgl_api_glClientWaitSync)(GLsync sync, GLbitfield flags, EvasGLuint64 timeout);

static void
_evgl_api_thread_glClientWaitSync(void *data)
{
   EVGL_API_Thread_Command_glClientWaitSync *thread_data =
      (EVGL_API_Thread_Command_glClientWaitSync *)data;

   thread_data->return_value = orig_evgl_api_glClientWaitSync(thread_data->sync,
                                                              thread_data->flags,
                                                              thread_data->timeout);

}

EAPI GLenum
glClientWaitSync_evgl_api_thread_cmd(GLsync sync, GLbitfield flags, EvasGLuint64 timeout)
{
   if (!evas_evgl_thread_enabled() || (_main_thread_id != eina_thread_self()))
     {
        return orig_evgl_api_glClientWaitSync(sync, flags, timeout);
     }

   int thread_mode = EVAS_GL_THREAD_MODE_FINISH;

   EVGL_API_Thread_Command_glClientWaitSync thread_data_local;
   EVGL_API_Thread_Command_glClientWaitSync *thread_data = &thread_data_local;

   thread_data->sync = sync;
   thread_data->flags = flags;
   thread_data->timeout = timeout;

   evas_gl_thread_cmd_enqueue(EVAS_GL_THREAD_TYPE_EVGL,
                              _evgl_api_thread_glClientWaitSync,
                              thread_data,
                              thread_mode);

   return thread_data->return_value;
}

/*
 * void
 * glCompressedTexImage3D(GLenum target, GLint level, GLenum internalformat, GLsizei width, GLsizei height, GLsizei depth, GLint border, GLsizei imageSize, const GLvoid * data);
 */

typedef struct
{
   GLenum target;
   GLint level;
   GLenum internalformat;
   GLsizei width;
   GLsizei height;
   GLsizei depth;
   GLint border;
   GLsizei imageSize;
   const GLvoid * data;

} EVGL_API_Thread_Command_glCompressedTexImage3D;

void (*orig_evgl_api_glCompressedTexImage3D)(GLenum target, GLint level, GLenum internalformat, GLsizei width, GLsizei height, GLsizei depth, GLint border, GLsizei imageSize, const GLvoid * data);

static void
_evgl_api_thread_glCompressedTexImage3D(void *data)
{
   EVGL_API_Thread_Command_glCompressedTexImage3D *thread_data =
      (EVGL_API_Thread_Command_glCompressedTexImage3D *)data;

   orig_evgl_api_glCompressedTexImage3D(thread_data->target,
                                        thread_data->level,
                                        thread_data->internalformat,
                                        thread_data->width,
                                        thread_data->height,
                                        thread_data->depth,
                                        thread_data->border,
                                        thread_data->imageSize,
                                        thread_data->data);

}

EAPI void
glCompressedTexImage3D_evgl_api_thread_cmd(GLenum target, GLint level, GLenum internalformat, GLsizei width, GLsizei height, GLsizei depth, GLint border, GLsizei imageSize, const GLvoid * data)
{
   if (!evas_evgl_thread_enabled() || (_main_thread_id != eina_thread_self()))
     {
        orig_evgl_api_glCompressedTexImage3D(target, level, internalformat, width, height, depth, border, imageSize, data);
        return;
     }

   int thread_mode = EVAS_GL_THREAD_MODE_FINISH;

   EVGL_API_Thread_Command_glCompressedTexImage3D thread_data_local;
   EVGL_API_Thread_Command_glCompressedTexImage3D *thread_data = &thread_data_local;

   thread_data->target = target;
   thread_data->level = level;
   thread_data->internalformat = internalformat;
   thread_data->width = width;
   thread_data->height = height;
   thread_data->depth = depth;
   thread_data->border = border;
   thread_data->imageSize = imageSize;
   thread_data->data = data;

   evas_gl_thread_cmd_enqueue(EVAS_GL_THREAD_TYPE_EVGL,
                              _evgl_api_thread_glCompressedTexImage3D,
                              thread_data,
                              thread_mode);
}

/*
 * void
 * glCompressedTexSubImage3D(GLenum target, GLint level, GLint xoffset, GLint yoffset, GLint zoffset, GLsizei width, GLsizei height, GLsizei depth, GLenum format, GLsizei imageSize, const GLvoid * data);
 */

typedef struct
{
   GLenum target;
   GLint level;
   GLint xoffset;
   GLint yoffset;
   GLint zoffset;
   GLsizei width;
   GLsizei height;
   GLsizei depth;
   GLenum format;
   GLsizei imageSize;
   const GLvoid * data;

} EVGL_API_Thread_Command_glCompressedTexSubImage3D;

void (*orig_evgl_api_glCompressedTexSubImage3D)(GLenum target, GLint level, GLint xoffset, GLint yoffset, GLint zoffset, GLsizei width, GLsizei height, GLsizei depth, GLenum format, GLsizei imageSize, const GLvoid * data);

static void
_evgl_api_thread_glCompressedTexSubImage3D(void *data)
{
   EVGL_API_Thread_Command_glCompressedTexSubImage3D *thread_data =
      (EVGL_API_Thread_Command_glCompressedTexSubImage3D *)data;

   orig_evgl_api_glCompressedTexSubImage3D(thread_data->target,
                                           thread_data->level,
                                           thread_data->xoffset,
                                           thread_data->yoffset,
                                           thread_data->zoffset,
                                           thread_data->width,
                                           thread_data->height,
                                           thread_data->depth,
                                           thread_data->format,
                                           thread_data->imageSize,
                                           thread_data->data);

}

EAPI void
glCompressedTexSubImage3D_evgl_api_thread_cmd(GLenum target, GLint level, GLint xoffset, GLint yoffset, GLint zoffset, GLsizei width, GLsizei height, GLsizei depth, GLenum format, GLsizei imageSize, const GLvoid * data)
{
   if (!evas_evgl_thread_enabled() || (_main_thread_id != eina_thread_self()))
     {
        orig_evgl_api_glCompressedTexSubImage3D(target, level, xoffset, yoffset, zoffset, width, height, depth, format, imageSize, data);
        return;
     }

   int thread_mode = EVAS_GL_THREAD_MODE_FINISH;

   EVGL_API_Thread_Command_glCompressedTexSubImage3D thread_data_local;
   EVGL_API_Thread_Command_glCompressedTexSubImage3D *thread_data = &thread_data_local;

   thread_data->target = target;
   thread_data->level = level;
   thread_data->xoffset = xoffset;
   thread_data->yoffset = yoffset;
   thread_data->zoffset = zoffset;
   thread_data->width = width;
   thread_data->height = height;
   thread_data->depth = depth;
   thread_data->format = format;
   thread_data->imageSize = imageSize;
   thread_data->data = data;

   evas_gl_thread_cmd_enqueue(EVAS_GL_THREAD_TYPE_EVGL,
                              _evgl_api_thread_glCompressedTexSubImage3D,
                              thread_data,
                              thread_mode);
}

/*
 * void
 * glCopyBufferSubData(GLenum readtarget, GLenum writetarget, GLintptr readoffset, GLintptr writeoffset, GLsizeiptr size);
 */

typedef struct
{
   GLenum readtarget;
   GLenum writetarget;
   GLintptr readoffset;
   GLintptr writeoffset;
   GLsizeiptr size;

} EVGL_API_Thread_Command_glCopyBufferSubData;

void (*orig_evgl_api_glCopyBufferSubData)(GLenum readtarget, GLenum writetarget, GLintptr readoffset, GLintptr writeoffset, GLsizeiptr size);

static void
_evgl_api_thread_glCopyBufferSubData(void *data)
{
   EVGL_API_Thread_Command_glCopyBufferSubData *thread_data =
      (EVGL_API_Thread_Command_glCopyBufferSubData *)data;

   orig_evgl_api_glCopyBufferSubData(thread_data->readtarget,
                                     thread_data->writetarget,
                                     thread_data->readoffset,
                                     thread_data->writeoffset,
                                     thread_data->size);

}

EAPI void
glCopyBufferSubData_evgl_api_thread_cmd(GLenum readtarget, GLenum writetarget, GLintptr readoffset, GLintptr writeoffset, GLsizeiptr size)
{
   if (!evas_evgl_thread_enabled() || (_main_thread_id != eina_thread_self()))
     {
        orig_evgl_api_glCopyBufferSubData(readtarget, writetarget, readoffset, writeoffset, size);
        return;
     }

   int thread_mode = EVAS_GL_THREAD_MODE_FINISH;

   EVGL_API_Thread_Command_glCopyBufferSubData thread_data_local;
   EVGL_API_Thread_Command_glCopyBufferSubData *thread_data = &thread_data_local;

   thread_data->readtarget = readtarget;
   thread_data->writetarget = writetarget;
   thread_data->readoffset = readoffset;
   thread_data->writeoffset = writeoffset;
   thread_data->size = size;

   evas_gl_thread_cmd_enqueue(EVAS_GL_THREAD_TYPE_EVGL,
                              _evgl_api_thread_glCopyBufferSubData,
                              thread_data,
                              thread_mode);
}

/*
 * void
 * glCopyTexSubImage3D(GLenum target, GLint level, GLint xoffset, GLint yoffset, GLint zoffset, GLint x, GLint y, GLsizei width, GLsizei height);
 */

typedef struct
{
   GLenum target;
   GLint level;
   GLint xoffset;
   GLint yoffset;
   GLint zoffset;
   GLint x;
   GLint y;
   GLsizei width;
   GLsizei height;

} EVGL_API_Thread_Command_glCopyTexSubImage3D;

void (*orig_evgl_api_glCopyTexSubImage3D)(GLenum target, GLint level, GLint xoffset, GLint yoffset, GLint zoffset, GLint x, GLint y, GLsizei width, GLsizei height);

static void
_evgl_api_thread_glCopyTexSubImage3D(void *data)
{
   EVGL_API_Thread_Command_glCopyTexSubImage3D *thread_data =
      (EVGL_API_Thread_Command_glCopyTexSubImage3D *)data;

   orig_evgl_api_glCopyTexSubImage3D(thread_data->target,
                                     thread_data->level,
                                     thread_data->xoffset,
                                     thread_data->yoffset,
                                     thread_data->zoffset,
                                     thread_data->x,
                                     thread_data->y,
                                     thread_data->width,
                                     thread_data->height);

}

EAPI void
glCopyTexSubImage3D_evgl_api_thread_cmd(GLenum target, GLint level, GLint xoffset, GLint yoffset, GLint zoffset, GLint x, GLint y, GLsizei width, GLsizei height)
{
   if (!evas_evgl_thread_enabled() || (_main_thread_id != eina_thread_self()))
     {
        orig_evgl_api_glCopyTexSubImage3D(target, level, xoffset, yoffset, zoffset, x, y, width, height);
        return;
     }

   int thread_mode = EVAS_GL_THREAD_MODE_FINISH;

   EVGL_API_Thread_Command_glCopyTexSubImage3D thread_data_local;
   EVGL_API_Thread_Command_glCopyTexSubImage3D *thread_data = &thread_data_local;

   thread_data->target = target;
   thread_data->level = level;
   thread_data->xoffset = xoffset;
   thread_data->yoffset = yoffset;
   thread_data->zoffset = zoffset;
   thread_data->x = x;
   thread_data->y = y;
   thread_data->width = width;
   thread_data->height = height;

   evas_gl_thread_cmd_enqueue(EVAS_GL_THREAD_TYPE_EVGL,
                              _evgl_api_thread_glCopyTexSubImage3D,
                              thread_data,
                              thread_mode);
}

/*
 * void
 * glDeleteQueries(GLsizei n, const GLuint * ids);
 */

typedef struct
{
   GLsizei n;
   const GLuint * ids;

} EVGL_API_Thread_Command_glDeleteQueries;

void (*orig_evgl_api_glDeleteQueries)(GLsizei n, const GLuint * ids);

static void
_evgl_api_thread_glDeleteQueries(void *data)
{
   EVGL_API_Thread_Command_glDeleteQueries *thread_data =
      (EVGL_API_Thread_Command_glDeleteQueries *)data;

   orig_evgl_api_glDeleteQueries(thread_data->n,
                                 thread_data->ids);

}

EAPI void
glDeleteQueries_evgl_api_thread_cmd(GLsizei n, const GLuint * ids)
{
   if (!evas_evgl_thread_enabled() || (_main_thread_id != eina_thread_self()))
     {
        orig_evgl_api_glDeleteQueries(n, ids);
        return;
     }

   int thread_mode = EVAS_GL_THREAD_MODE_FINISH;

   EVGL_API_Thread_Command_glDeleteQueries thread_data_local;
   EVGL_API_Thread_Command_glDeleteQueries *thread_data = &thread_data_local;

   thread_data->n = n;
   thread_data->ids = ids;

   evas_gl_thread_cmd_enqueue(EVAS_GL_THREAD_TYPE_EVGL,
                              _evgl_api_thread_glDeleteQueries,
                              thread_data,
                              thread_mode);
}

/*
 * void
 * glDeleteSamplers(GLsizei n, const GLuint * samplers);
 */

typedef struct
{
   GLsizei n;
   const GLuint * samplers;

} EVGL_API_Thread_Command_glDeleteSamplers;

void (*orig_evgl_api_glDeleteSamplers)(GLsizei n, const GLuint * samplers);

static void
_evgl_api_thread_glDeleteSamplers(void *data)
{
   EVGL_API_Thread_Command_glDeleteSamplers *thread_data =
      (EVGL_API_Thread_Command_glDeleteSamplers *)data;

   orig_evgl_api_glDeleteSamplers(thread_data->n,
                                  thread_data->samplers);

}

EAPI void
glDeleteSamplers_evgl_api_thread_cmd(GLsizei n, const GLuint * samplers)
{
   if (!evas_evgl_thread_enabled() || (_main_thread_id != eina_thread_self()))
     {
        orig_evgl_api_glDeleteSamplers(n, samplers);
        return;
     }

   int thread_mode = EVAS_GL_THREAD_MODE_FINISH;

   EVGL_API_Thread_Command_glDeleteSamplers thread_data_local;
   EVGL_API_Thread_Command_glDeleteSamplers *thread_data = &thread_data_local;

   thread_data->n = n;
   thread_data->samplers = samplers;

   evas_gl_thread_cmd_enqueue(EVAS_GL_THREAD_TYPE_EVGL,
                              _evgl_api_thread_glDeleteSamplers,
                              thread_data,
                              thread_mode);
}

/*
 * void
 * glDeleteSync(GLsync sync);
 */

typedef struct
{
   GLsync sync;

} EVGL_API_Thread_Command_glDeleteSync;

void (*orig_evgl_api_glDeleteSync)(GLsync sync);

static void
_evgl_api_thread_glDeleteSync(void *data)
{
   EVGL_API_Thread_Command_glDeleteSync *thread_data =
      (EVGL_API_Thread_Command_glDeleteSync *)data;

   orig_evgl_api_glDeleteSync(thread_data->sync);

}

EAPI void
glDeleteSync_evgl_api_thread_cmd(GLsync sync)
{
   if (!evas_evgl_thread_enabled() || (_main_thread_id != eina_thread_self()))
     {
        orig_evgl_api_glDeleteSync(sync);
        return;
     }

   int thread_mode = EVAS_GL_THREAD_MODE_FINISH;

   EVGL_API_Thread_Command_glDeleteSync thread_data_local;
   EVGL_API_Thread_Command_glDeleteSync *thread_data = &thread_data_local;

   thread_data->sync = sync;

   evas_gl_thread_cmd_enqueue(EVAS_GL_THREAD_TYPE_EVGL,
                              _evgl_api_thread_glDeleteSync,
                              thread_data,
                              thread_mode);
}

/*
 * void
 * glDeleteTransformFeedbacks(GLsizei n, const GLuint *ids);
 */

typedef struct
{
   GLsizei n;
   const GLuint *ids;

} EVGL_API_Thread_Command_glDeleteTransformFeedbacks;

void (*orig_evgl_api_glDeleteTransformFeedbacks)(GLsizei n, const GLuint *ids);

static void
_evgl_api_thread_glDeleteTransformFeedbacks(void *data)
{
   EVGL_API_Thread_Command_glDeleteTransformFeedbacks *thread_data =
      (EVGL_API_Thread_Command_glDeleteTransformFeedbacks *)data;

   orig_evgl_api_glDeleteTransformFeedbacks(thread_data->n,
                                            thread_data->ids);

}

EAPI void
glDeleteTransformFeedbacks_evgl_api_thread_cmd(GLsizei n, const GLuint *ids)
{
   if (!evas_evgl_thread_enabled() || (_main_thread_id != eina_thread_self()))
     {
        orig_evgl_api_glDeleteTransformFeedbacks(n, ids);
        return;
     }

   int thread_mode = EVAS_GL_THREAD_MODE_FINISH;

   EVGL_API_Thread_Command_glDeleteTransformFeedbacks thread_data_local;
   EVGL_API_Thread_Command_glDeleteTransformFeedbacks *thread_data = &thread_data_local;

   thread_data->n = n;
   thread_data->ids = ids;

   evas_gl_thread_cmd_enqueue(EVAS_GL_THREAD_TYPE_EVGL,
                              _evgl_api_thread_glDeleteTransformFeedbacks,
                              thread_data,
                              thread_mode);
}

/*
 * void
 * glDeleteVertexArrays(GLsizei n, const GLuint *arrays);
 */

typedef struct
{
   GLsizei n;
   const GLuint *arrays;

} EVGL_API_Thread_Command_glDeleteVertexArrays;

void (*orig_evgl_api_glDeleteVertexArrays)(GLsizei n, const GLuint *arrays);

static void
_evgl_api_thread_glDeleteVertexArrays(void *data)
{
   EVGL_API_Thread_Command_glDeleteVertexArrays *thread_data =
      (EVGL_API_Thread_Command_glDeleteVertexArrays *)data;

   orig_evgl_api_glDeleteVertexArrays(thread_data->n,
                                      thread_data->arrays);

}

EAPI void
glDeleteVertexArrays_evgl_api_thread_cmd(GLsizei n, const GLuint *arrays)
{
   if (!evas_evgl_thread_enabled() || (_main_thread_id != eina_thread_self()))
     {
        orig_evgl_api_glDeleteVertexArrays(n, arrays);
        return;
     }

   int thread_mode = EVAS_GL_THREAD_MODE_FINISH;

   EVGL_API_Thread_Command_glDeleteVertexArrays thread_data_local;
   EVGL_API_Thread_Command_glDeleteVertexArrays *thread_data = &thread_data_local;

   thread_data->n = n;
   thread_data->arrays = arrays;

   evas_gl_thread_cmd_enqueue(EVAS_GL_THREAD_TYPE_EVGL,
                              _evgl_api_thread_glDeleteVertexArrays,
                              thread_data,
                              thread_mode);
}

/*
 * void
 * glDrawArraysInstanced(GLenum mode, GLint first, GLsizei count, GLsizei primcount);
 */

typedef struct
{
   GLenum mode;
   GLint first;
   GLsizei count;
   GLsizei primcount;

} EVGL_API_Thread_Command_glDrawArraysInstanced;

void (*orig_evgl_api_glDrawArraysInstanced)(GLenum mode, GLint first, GLsizei count, GLsizei primcount);

static void
_evgl_api_thread_glDrawArraysInstanced(void *data)
{
   EVGL_API_Thread_Command_glDrawArraysInstanced *thread_data =
      (EVGL_API_Thread_Command_glDrawArraysInstanced *)data;

   orig_evgl_api_glDrawArraysInstanced(thread_data->mode,
                                       thread_data->first,
                                       thread_data->count,
                                       thread_data->primcount);

}

EAPI void
glDrawArraysInstanced_evgl_api_thread_cmd(GLenum mode, GLint first, GLsizei count, GLsizei primcount)
{
   if (!evas_evgl_thread_enabled() || (_main_thread_id != eina_thread_self()))
     {
        orig_evgl_api_glDrawArraysInstanced(mode, first, count, primcount);
        return;
     }

   int thread_mode = EVAS_GL_THREAD_MODE_FINISH;

   EVGL_API_Thread_Command_glDrawArraysInstanced thread_data_local;
   EVGL_API_Thread_Command_glDrawArraysInstanced *thread_data = &thread_data_local;

   thread_data->mode = mode;
   thread_data->first = first;
   thread_data->count = count;
   thread_data->primcount = primcount;

   evas_gl_thread_cmd_enqueue(EVAS_GL_THREAD_TYPE_EVGL,
                              _evgl_api_thread_glDrawArraysInstanced,
                              thread_data,
                              thread_mode);
}

/*
 * void
 * glDrawBuffers(GLsizei n, const GLenum *bufs);
 */

typedef struct
{
   GLsizei n;
   const GLenum *bufs;

} EVGL_API_Thread_Command_glDrawBuffers;

void (*orig_evgl_api_glDrawBuffers)(GLsizei n, const GLenum *bufs);

static void
_evgl_api_thread_glDrawBuffers(void *data)
{
   EVGL_API_Thread_Command_glDrawBuffers *thread_data =
      (EVGL_API_Thread_Command_glDrawBuffers *)data;

   orig_evgl_api_glDrawBuffers(thread_data->n,
                               thread_data->bufs);

}

EAPI void
glDrawBuffers_evgl_api_thread_cmd(GLsizei n, const GLenum *bufs)
{
   if (!evas_evgl_thread_enabled() || (_main_thread_id != eina_thread_self()))
     {
        orig_evgl_api_glDrawBuffers(n, bufs);
        return;
     }

   int thread_mode = EVAS_GL_THREAD_MODE_FINISH;

   EVGL_API_Thread_Command_glDrawBuffers thread_data_local;
   EVGL_API_Thread_Command_glDrawBuffers *thread_data = &thread_data_local;

   thread_data->n = n;
   thread_data->bufs = bufs;

   evas_gl_thread_cmd_enqueue(EVAS_GL_THREAD_TYPE_EVGL,
                              _evgl_api_thread_glDrawBuffers,
                              thread_data,
                              thread_mode);
}

/*
 * void
 * glDrawElementsInstanced(GLenum mode, GLsizei count, GLenum type, const void * indices, GLsizei primcount);
 */

typedef struct
{
   GLenum mode;
   GLsizei count;
   GLenum type;
   const void * indices;
   GLsizei primcount;

} EVGL_API_Thread_Command_glDrawElementsInstanced;

void (*orig_evgl_api_glDrawElementsInstanced)(GLenum mode, GLsizei count, GLenum type, const void * indices, GLsizei primcount);

static void
_evgl_api_thread_glDrawElementsInstanced(void *data)
{
   EVGL_API_Thread_Command_glDrawElementsInstanced *thread_data =
      (EVGL_API_Thread_Command_glDrawElementsInstanced *)data;

   orig_evgl_api_glDrawElementsInstanced(thread_data->mode,
                                         thread_data->count,
                                         thread_data->type,
                                         thread_data->indices,
                                         thread_data->primcount);

}

EAPI void
glDrawElementsInstanced_evgl_api_thread_cmd(GLenum mode, GLsizei count, GLenum type, const void * indices, GLsizei primcount)
{
   if (!evas_evgl_thread_enabled() || (_main_thread_id != eina_thread_self()))
     {
        orig_evgl_api_glDrawElementsInstanced(mode, count, type, indices, primcount);
        return;
     }

   int thread_mode = EVAS_GL_THREAD_MODE_FINISH;

   EVGL_API_Thread_Command_glDrawElementsInstanced thread_data_local;
   EVGL_API_Thread_Command_glDrawElementsInstanced *thread_data = &thread_data_local;

   thread_data->mode = mode;
   thread_data->count = count;
   thread_data->type = type;
   thread_data->indices = indices;
   thread_data->primcount = primcount;

   evas_gl_thread_cmd_enqueue(EVAS_GL_THREAD_TYPE_EVGL,
                              _evgl_api_thread_glDrawElementsInstanced,
                              thread_data,
                              thread_mode);
}

/*
 * void
 * glDrawRangeElements(GLenum mode, GLuint start, GLuint end, GLsizei count, GLenum type, const GLvoid * indices);
 */

typedef struct
{
   GLenum mode;
   GLuint start;
   GLuint end;
   GLsizei count;
   GLenum type;
   const GLvoid * indices;

} EVGL_API_Thread_Command_glDrawRangeElements;

void (*orig_evgl_api_glDrawRangeElements)(GLenum mode, GLuint start, GLuint end, GLsizei count, GLenum type, const GLvoid * indices);

static void
_evgl_api_thread_glDrawRangeElements(void *data)
{
   EVGL_API_Thread_Command_glDrawRangeElements *thread_data =
      (EVGL_API_Thread_Command_glDrawRangeElements *)data;

   orig_evgl_api_glDrawRangeElements(thread_data->mode,
                                     thread_data->start,
                                     thread_data->end,
                                     thread_data->count,
                                     thread_data->type,
                                     thread_data->indices);

}

EAPI void
glDrawRangeElements_evgl_api_thread_cmd(GLenum mode, GLuint start, GLuint end, GLsizei count, GLenum type, const GLvoid * indices)
{
   if (!evas_evgl_thread_enabled() || (_main_thread_id != eina_thread_self()))
     {
        orig_evgl_api_glDrawRangeElements(mode, start, end, count, type, indices);
        return;
     }

   int thread_mode = EVAS_GL_THREAD_MODE_FINISH;

   EVGL_API_Thread_Command_glDrawRangeElements thread_data_local;
   EVGL_API_Thread_Command_glDrawRangeElements *thread_data = &thread_data_local;

   thread_data->mode = mode;
   thread_data->start = start;
   thread_data->end = end;
   thread_data->count = count;
   thread_data->type = type;
   thread_data->indices = indices;

   evas_gl_thread_cmd_enqueue(EVAS_GL_THREAD_TYPE_EVGL,
                              _evgl_api_thread_glDrawRangeElements,
                              thread_data,
                              thread_mode);
}

/*
 * void
 * glEndQuery(GLenum target);
 */

typedef struct
{
   GLenum target;

} EVGL_API_Thread_Command_glEndQuery;

void (*orig_evgl_api_glEndQuery)(GLenum target);

static void
_evgl_api_thread_glEndQuery(void *data)
{
   EVGL_API_Thread_Command_glEndQuery *thread_data =
      (EVGL_API_Thread_Command_glEndQuery *)data;

   orig_evgl_api_glEndQuery(thread_data->target);

}

EAPI void
glEndQuery_evgl_api_thread_cmd(GLenum target)
{
   if (!evas_evgl_thread_enabled() || (_main_thread_id != eina_thread_self()))
     {
        orig_evgl_api_glEndQuery(target);
        return;
     }

   int thread_mode = EVAS_GL_THREAD_MODE_FINISH;

   EVGL_API_Thread_Command_glEndQuery thread_data_local;
   EVGL_API_Thread_Command_glEndQuery *thread_data = &thread_data_local;

   thread_data->target = target;

   evas_gl_thread_cmd_enqueue(EVAS_GL_THREAD_TYPE_EVGL,
                              _evgl_api_thread_glEndQuery,
                              thread_data,
                              thread_mode);
}

/*
 * void
 * glEndTransformFeedback(void);
 */

void (*orig_evgl_api_glEndTransformFeedback)(void);

static void
_evgl_api_thread_glEndTransformFeedback(void *data EINA_UNUSED)
{
   orig_evgl_api_glEndTransformFeedback();

}

EAPI void
glEndTransformFeedback_evgl_api_thread_cmd(void)
{
   if (!evas_evgl_thread_enabled() || (_main_thread_id != eina_thread_self()))
     {
        orig_evgl_api_glEndTransformFeedback();
        return;
     }

   int thread_mode = EVAS_GL_THREAD_MODE_FINISH;

   evas_gl_thread_cmd_enqueue(EVAS_GL_THREAD_TYPE_EVGL,
                              _evgl_api_thread_glEndTransformFeedback,
                              NULL,
                              thread_mode);
}

/*
 * GLsync
 * glFenceSync(GLenum condition, GLbitfield flags);
 */

typedef struct
{
   GLsync return_value;
   GLenum condition;
   GLbitfield flags;

} EVGL_API_Thread_Command_glFenceSync;

GLsync (*orig_evgl_api_glFenceSync)(GLenum condition, GLbitfield flags);

static void
_evgl_api_thread_glFenceSync(void *data)
{
   EVGL_API_Thread_Command_glFenceSync *thread_data =
      (EVGL_API_Thread_Command_glFenceSync *)data;

   thread_data->return_value = orig_evgl_api_glFenceSync(thread_data->condition,
                                                         thread_data->flags);

}

EAPI GLsync
glFenceSync_evgl_api_thread_cmd(GLenum condition, GLbitfield flags)
{
   if (!evas_evgl_thread_enabled() || (_main_thread_id != eina_thread_self()))
     {
        return orig_evgl_api_glFenceSync(condition, flags);
     }

   int thread_mode = EVAS_GL_THREAD_MODE_FINISH;

   EVGL_API_Thread_Command_glFenceSync thread_data_local;
   EVGL_API_Thread_Command_glFenceSync *thread_data = &thread_data_local;

   thread_data->condition = condition;
   thread_data->flags = flags;

   evas_gl_thread_cmd_enqueue(EVAS_GL_THREAD_TYPE_EVGL,
                              _evgl_api_thread_glFenceSync,
                              thread_data,
                              thread_mode);

   return thread_data->return_value;
}

/*
 * GLsync
 * glFlushMappedBufferRange(GLenum target, GLintptr offset, GLsizeiptr length);
 */

typedef struct
{
   GLsync return_value;
   GLenum target;
   GLintptr offset;
   GLsizeiptr length;

} EVGL_API_Thread_Command_glFlushMappedBufferRange;

GLsync (*orig_evgl_api_glFlushMappedBufferRange)(GLenum target, GLintptr offset, GLsizeiptr length);

static void
_evgl_api_thread_glFlushMappedBufferRange(void *data)
{
   EVGL_API_Thread_Command_glFlushMappedBufferRange *thread_data =
      (EVGL_API_Thread_Command_glFlushMappedBufferRange *)data;

   thread_data->return_value = orig_evgl_api_glFlushMappedBufferRange(thread_data->target,
                                                                      thread_data->offset,
                                                                      thread_data->length);

}

EAPI GLsync
glFlushMappedBufferRange_evgl_api_thread_cmd(GLenum target, GLintptr offset, GLsizeiptr length)
{
   if (!evas_evgl_thread_enabled() || (_main_thread_id != eina_thread_self()))
     {
        return orig_evgl_api_glFlushMappedBufferRange(target, offset, length);
     }

   int thread_mode = EVAS_GL_THREAD_MODE_FINISH;

   EVGL_API_Thread_Command_glFlushMappedBufferRange thread_data_local;
   EVGL_API_Thread_Command_glFlushMappedBufferRange *thread_data = &thread_data_local;

   thread_data->target = target;
   thread_data->offset = offset;
   thread_data->length = length;

   evas_gl_thread_cmd_enqueue(EVAS_GL_THREAD_TYPE_EVGL,
                              _evgl_api_thread_glFlushMappedBufferRange,
                              thread_data,
                              thread_mode);

   return thread_data->return_value;
}

/*
 * void
 * glFramebufferTextureLayer(GLenum target, GLenum attachment, GLuint texture, GLint level, GLint layer);
 */

typedef struct
{
   GLenum target;
   GLenum attachment;
   GLuint texture;
   GLint level;
   GLint layer;

} EVGL_API_Thread_Command_glFramebufferTextureLayer;

void (*orig_evgl_api_glFramebufferTextureLayer)(GLenum target, GLenum attachment, GLuint texture, GLint level, GLint layer);

static void
_evgl_api_thread_glFramebufferTextureLayer(void *data)
{
   EVGL_API_Thread_Command_glFramebufferTextureLayer *thread_data =
      (EVGL_API_Thread_Command_glFramebufferTextureLayer *)data;

   orig_evgl_api_glFramebufferTextureLayer(thread_data->target,
                                           thread_data->attachment,
                                           thread_data->texture,
                                           thread_data->level,
                                           thread_data->layer);

}

EAPI void
glFramebufferTextureLayer_evgl_api_thread_cmd(GLenum target, GLenum attachment, GLuint texture, GLint level, GLint layer)
{
   if (!evas_evgl_thread_enabled() || (_main_thread_id != eina_thread_self()))
     {
        orig_evgl_api_glFramebufferTextureLayer(target, attachment, texture, level, layer);
        return;
     }

   int thread_mode = EVAS_GL_THREAD_MODE_FINISH;

   EVGL_API_Thread_Command_glFramebufferTextureLayer thread_data_local;
   EVGL_API_Thread_Command_glFramebufferTextureLayer *thread_data = &thread_data_local;

   thread_data->target = target;
   thread_data->attachment = attachment;
   thread_data->texture = texture;
   thread_data->level = level;
   thread_data->layer = layer;

   evas_gl_thread_cmd_enqueue(EVAS_GL_THREAD_TYPE_EVGL,
                              _evgl_api_thread_glFramebufferTextureLayer,
                              thread_data,
                              thread_mode);
}

/*
 * void
 * glGenQueries(GLsizei n, GLuint * ids);
 */

typedef struct
{
   GLsizei n;
   GLuint * ids;

} EVGL_API_Thread_Command_glGenQueries;

void (*orig_evgl_api_glGenQueries)(GLsizei n, GLuint * ids);

static void
_evgl_api_thread_glGenQueries(void *data)
{
   EVGL_API_Thread_Command_glGenQueries *thread_data =
      (EVGL_API_Thread_Command_glGenQueries *)data;

   orig_evgl_api_glGenQueries(thread_data->n,
                              thread_data->ids);

}

EAPI void
glGenQueries_evgl_api_thread_cmd(GLsizei n, GLuint * ids)
{
   if (!evas_evgl_thread_enabled() || (_main_thread_id != eina_thread_self()))
     {
        orig_evgl_api_glGenQueries(n, ids);
        return;
     }

   int thread_mode = EVAS_GL_THREAD_MODE_FINISH;

   EVGL_API_Thread_Command_glGenQueries thread_data_local;
   EVGL_API_Thread_Command_glGenQueries *thread_data = &thread_data_local;

   thread_data->n = n;
   thread_data->ids = ids;

   evas_gl_thread_cmd_enqueue(EVAS_GL_THREAD_TYPE_EVGL,
                              _evgl_api_thread_glGenQueries,
                              thread_data,
                              thread_mode);
}

/*
 * void
 * glGenSamplers(GLsizei n, GLuint *samplers);
 */

typedef struct
{
   GLsizei n;
   GLuint *samplers;

} EVGL_API_Thread_Command_glGenSamplers;

void (*orig_evgl_api_glGenSamplers)(GLsizei n, GLuint *samplers);

static void
_evgl_api_thread_glGenSamplers(void *data)
{
   EVGL_API_Thread_Command_glGenSamplers *thread_data =
      (EVGL_API_Thread_Command_glGenSamplers *)data;

   orig_evgl_api_glGenSamplers(thread_data->n,
                               thread_data->samplers);

}

EAPI void
glGenSamplers_evgl_api_thread_cmd(GLsizei n, GLuint *samplers)
{
   if (!evas_evgl_thread_enabled() || (_main_thread_id != eina_thread_self()))
     {
        orig_evgl_api_glGenSamplers(n, samplers);
        return;
     }

   int thread_mode = EVAS_GL_THREAD_MODE_FINISH;

   EVGL_API_Thread_Command_glGenSamplers thread_data_local;
   EVGL_API_Thread_Command_glGenSamplers *thread_data = &thread_data_local;

   thread_data->n = n;
   thread_data->samplers = samplers;

   evas_gl_thread_cmd_enqueue(EVAS_GL_THREAD_TYPE_EVGL,
                              _evgl_api_thread_glGenSamplers,
                              thread_data,
                              thread_mode);
}

/*
 * void
 * glGenTransformFeedbacks(GLsizei n, GLuint *ids);
 */

typedef struct
{
   GLsizei n;
   GLuint *ids;

} EVGL_API_Thread_Command_glGenTransformFeedbacks;

void (*orig_evgl_api_glGenTransformFeedbacks)(GLsizei n, GLuint *ids);

static void
_evgl_api_thread_glGenTransformFeedbacks(void *data)
{
   EVGL_API_Thread_Command_glGenTransformFeedbacks *thread_data =
      (EVGL_API_Thread_Command_glGenTransformFeedbacks *)data;

   orig_evgl_api_glGenTransformFeedbacks(thread_data->n,
                                         thread_data->ids);

}

EAPI void
glGenTransformFeedbacks_evgl_api_thread_cmd(GLsizei n, GLuint *ids)
{
   if (!evas_evgl_thread_enabled() || (_main_thread_id != eina_thread_self()))
     {
        orig_evgl_api_glGenTransformFeedbacks(n, ids);
        return;
     }

   int thread_mode = EVAS_GL_THREAD_MODE_FINISH;

   EVGL_API_Thread_Command_glGenTransformFeedbacks thread_data_local;
   EVGL_API_Thread_Command_glGenTransformFeedbacks *thread_data = &thread_data_local;

   thread_data->n = n;
   thread_data->ids = ids;

   evas_gl_thread_cmd_enqueue(EVAS_GL_THREAD_TYPE_EVGL,
                              _evgl_api_thread_glGenTransformFeedbacks,
                              thread_data,
                              thread_mode);
}

/*
 * void
 * glGenVertexArrays(GLsizei n, GLuint *arrays);
 */

typedef struct
{
   GLsizei n;
   GLuint *arrays;

} EVGL_API_Thread_Command_glGenVertexArrays;

void (*orig_evgl_api_glGenVertexArrays)(GLsizei n, GLuint *arrays);

static void
_evgl_api_thread_glGenVertexArrays(void *data)
{
   EVGL_API_Thread_Command_glGenVertexArrays *thread_data =
      (EVGL_API_Thread_Command_glGenVertexArrays *)data;

   orig_evgl_api_glGenVertexArrays(thread_data->n,
                                   thread_data->arrays);

}

EAPI void
glGenVertexArrays_evgl_api_thread_cmd(GLsizei n, GLuint *arrays)
{
   if (!evas_evgl_thread_enabled() || (_main_thread_id != eina_thread_self()))
     {
        orig_evgl_api_glGenVertexArrays(n, arrays);
        return;
     }

   int thread_mode = EVAS_GL_THREAD_MODE_FINISH;

   EVGL_API_Thread_Command_glGenVertexArrays thread_data_local;
   EVGL_API_Thread_Command_glGenVertexArrays *thread_data = &thread_data_local;

   thread_data->n = n;
   thread_data->arrays = arrays;

   evas_gl_thread_cmd_enqueue(EVAS_GL_THREAD_TYPE_EVGL,
                              _evgl_api_thread_glGenVertexArrays,
                              thread_data,
                              thread_mode);
}

/*
 * void
 * glGetActiveUniformBlockiv(GLuint program, GLuint uniformBlockIndex, GLenum pname, GLint *params);
 */

typedef struct
{
   GLuint program;
   GLuint uniformBlockIndex;
   GLenum pname;
   GLint *params;

} EVGL_API_Thread_Command_glGetActiveUniformBlockiv;

void (*orig_evgl_api_glGetActiveUniformBlockiv)(GLuint program, GLuint uniformBlockIndex, GLenum pname, GLint *params);

static void
_evgl_api_thread_glGetActiveUniformBlockiv(void *data)
{
   EVGL_API_Thread_Command_glGetActiveUniformBlockiv *thread_data =
      (EVGL_API_Thread_Command_glGetActiveUniformBlockiv *)data;

   orig_evgl_api_glGetActiveUniformBlockiv(thread_data->program,
                                           thread_data->uniformBlockIndex,
                                           thread_data->pname,
                                           thread_data->params);

}

EAPI void
glGetActiveUniformBlockiv_evgl_api_thread_cmd(GLuint program, GLuint uniformBlockIndex, GLenum pname, GLint *params)
{
   if (!evas_evgl_thread_enabled() || (_main_thread_id != eina_thread_self()))
     {
        orig_evgl_api_glGetActiveUniformBlockiv(program, uniformBlockIndex, pname, params);
        return;
     }

   int thread_mode = EVAS_GL_THREAD_MODE_FINISH;

   EVGL_API_Thread_Command_glGetActiveUniformBlockiv thread_data_local;
   EVGL_API_Thread_Command_glGetActiveUniformBlockiv *thread_data = &thread_data_local;

   thread_data->program = program;
   thread_data->uniformBlockIndex = uniformBlockIndex;
   thread_data->pname = pname;
   thread_data->params = params;

   evas_gl_thread_cmd_enqueue(EVAS_GL_THREAD_TYPE_EVGL,
                              _evgl_api_thread_glGetActiveUniformBlockiv,
                              thread_data,
                              thread_mode);
}

/*
 * void
 * glGetActiveUniformBlockName(GLuint program, GLuint uniformBlockIndex, GLsizei bufSize, GLsizei *length, GLchar *uniformBlockName);
 */

typedef struct
{
   GLuint program;
   GLuint uniformBlockIndex;
   GLsizei bufSize;
   GLsizei *length;
   GLchar *uniformBlockName;

} EVGL_API_Thread_Command_glGetActiveUniformBlockName;

void (*orig_evgl_api_glGetActiveUniformBlockName)(GLuint program, GLuint uniformBlockIndex, GLsizei bufSize, GLsizei *length, GLchar *uniformBlockName);

static void
_evgl_api_thread_glGetActiveUniformBlockName(void *data)
{
   EVGL_API_Thread_Command_glGetActiveUniformBlockName *thread_data =
      (EVGL_API_Thread_Command_glGetActiveUniformBlockName *)data;

   orig_evgl_api_glGetActiveUniformBlockName(thread_data->program,
                                             thread_data->uniformBlockIndex,
                                             thread_data->bufSize,
                                             thread_data->length,
                                             thread_data->uniformBlockName);

}

EAPI void
glGetActiveUniformBlockName_evgl_api_thread_cmd(GLuint program, GLuint uniformBlockIndex, GLsizei bufSize, GLsizei *length, GLchar *uniformBlockName)
{
   if (!evas_evgl_thread_enabled() || (_main_thread_id != eina_thread_self()))
     {
        orig_evgl_api_glGetActiveUniformBlockName(program, uniformBlockIndex, bufSize, length, uniformBlockName);
        return;
     }

   int thread_mode = EVAS_GL_THREAD_MODE_FINISH;

   EVGL_API_Thread_Command_glGetActiveUniformBlockName thread_data_local;
   EVGL_API_Thread_Command_glGetActiveUniformBlockName *thread_data = &thread_data_local;

   thread_data->program = program;
   thread_data->uniformBlockIndex = uniformBlockIndex;
   thread_data->bufSize = bufSize;
   thread_data->length = length;
   thread_data->uniformBlockName = uniformBlockName;

   evas_gl_thread_cmd_enqueue(EVAS_GL_THREAD_TYPE_EVGL,
                              _evgl_api_thread_glGetActiveUniformBlockName,
                              thread_data,
                              thread_mode);
}

/*
 * void
 * glGetActiveUniformsiv(GLuint program, GLsizei uniformCount, const GLuint *uniformIndices, GLenum pname, GLint *params);
 */

typedef struct
{
   GLuint program;
   GLsizei uniformCount;
   const GLuint *uniformIndices;
   GLenum pname;
   GLint *params;

} EVGL_API_Thread_Command_glGetActiveUniformsiv;

void (*orig_evgl_api_glGetActiveUniformsiv)(GLuint program, GLsizei uniformCount, const GLuint *uniformIndices, GLenum pname, GLint *params);

static void
_evgl_api_thread_glGetActiveUniformsiv(void *data)
{
   EVGL_API_Thread_Command_glGetActiveUniformsiv *thread_data =
      (EVGL_API_Thread_Command_glGetActiveUniformsiv *)data;

   orig_evgl_api_glGetActiveUniformsiv(thread_data->program,
                                       thread_data->uniformCount,
                                       thread_data->uniformIndices,
                                       thread_data->pname,
                                       thread_data->params);

}

EAPI void
glGetActiveUniformsiv_evgl_api_thread_cmd(GLuint program, GLsizei uniformCount, const GLuint *uniformIndices, GLenum pname, GLint *params)
{
   if (!evas_evgl_thread_enabled() || (_main_thread_id != eina_thread_self()))
     {
        orig_evgl_api_glGetActiveUniformsiv(program, uniformCount, uniformIndices, pname, params);
        return;
     }

   int thread_mode = EVAS_GL_THREAD_MODE_FINISH;

   EVGL_API_Thread_Command_glGetActiveUniformsiv thread_data_local;
   EVGL_API_Thread_Command_glGetActiveUniformsiv *thread_data = &thread_data_local;

   thread_data->program = program;
   thread_data->uniformCount = uniformCount;
   thread_data->uniformIndices = uniformIndices;
   thread_data->pname = pname;
   thread_data->params = params;

   evas_gl_thread_cmd_enqueue(EVAS_GL_THREAD_TYPE_EVGL,
                              _evgl_api_thread_glGetActiveUniformsiv,
                              thread_data,
                              thread_mode);
}

/*
 * void
 * glGetBufferParameteri64v(GLenum target, GLenum value, EvasGLint64 * data);
 */

typedef struct
{
   GLenum target;
   GLenum value;
   EvasGLint64 * data;

} EVGL_API_Thread_Command_glGetBufferParameteri64v;

void (*orig_evgl_api_glGetBufferParameteri64v)(GLenum target, GLenum value, EvasGLint64 * data);

static void
_evgl_api_thread_glGetBufferParameteri64v(void *data)
{
   EVGL_API_Thread_Command_glGetBufferParameteri64v *thread_data =
      (EVGL_API_Thread_Command_glGetBufferParameteri64v *)data;

   orig_evgl_api_glGetBufferParameteri64v(thread_data->target,
                                          thread_data->value,
                                          thread_data->data);

}

EAPI void
glGetBufferParameteri64v_evgl_api_thread_cmd(GLenum target, GLenum value, EvasGLint64 * data)
{
   if (!evas_evgl_thread_enabled() || (_main_thread_id != eina_thread_self()))
     {
        orig_evgl_api_glGetBufferParameteri64v(target, value, data);
        return;
     }

   int thread_mode = EVAS_GL_THREAD_MODE_FINISH;

   EVGL_API_Thread_Command_glGetBufferParameteri64v thread_data_local;
   EVGL_API_Thread_Command_glGetBufferParameteri64v *thread_data = &thread_data_local;

   thread_data->target = target;
   thread_data->value = value;
   thread_data->data = data;

   evas_gl_thread_cmd_enqueue(EVAS_GL_THREAD_TYPE_EVGL,
                              _evgl_api_thread_glGetBufferParameteri64v,
                              thread_data,
                              thread_mode);
}

/*
 * void
 * glGetBufferPointerv(GLenum target, GLenum pname, GLvoid ** params);
 */

typedef struct
{
   GLenum target;
   GLenum pname;
   GLvoid ** params;

} EVGL_API_Thread_Command_glGetBufferPointerv;

void (*orig_evgl_api_glGetBufferPointerv)(GLenum target, GLenum pname, GLvoid ** params);

static void
_evgl_api_thread_glGetBufferPointerv(void *data)
{
   EVGL_API_Thread_Command_glGetBufferPointerv *thread_data =
      (EVGL_API_Thread_Command_glGetBufferPointerv *)data;

   orig_evgl_api_glGetBufferPointerv(thread_data->target,
                                     thread_data->pname,
                                     thread_data->params);

}

EAPI void
glGetBufferPointerv_evgl_api_thread_cmd(GLenum target, GLenum pname, GLvoid ** params)
{
   if (!evas_evgl_thread_enabled() || (_main_thread_id != eina_thread_self()))
     {
        orig_evgl_api_glGetBufferPointerv(target, pname, params);
        return;
     }

   int thread_mode = EVAS_GL_THREAD_MODE_FINISH;

   EVGL_API_Thread_Command_glGetBufferPointerv thread_data_local;
   EVGL_API_Thread_Command_glGetBufferPointerv *thread_data = &thread_data_local;

   thread_data->target = target;
   thread_data->pname = pname;
   thread_data->params = params;

   evas_gl_thread_cmd_enqueue(EVAS_GL_THREAD_TYPE_EVGL,
                              _evgl_api_thread_glGetBufferPointerv,
                              thread_data,
                              thread_mode);
}

/*
 * GLint
 * glGetFragDataLocation(GLuint program, const char * name);
 */

typedef struct
{
   GLint return_value;
   GLuint program;
   const char * name;

} EVGL_API_Thread_Command_glGetFragDataLocation;

GLint (*orig_evgl_api_glGetFragDataLocation)(GLuint program, const char * name);

static void
_evgl_api_thread_glGetFragDataLocation(void *data)
{
   EVGL_API_Thread_Command_glGetFragDataLocation *thread_data =
      (EVGL_API_Thread_Command_glGetFragDataLocation *)data;

   thread_data->return_value = orig_evgl_api_glGetFragDataLocation(thread_data->program,
                                                                   thread_data->name);

}

EAPI GLint
glGetFragDataLocation_evgl_api_thread_cmd(GLuint program, const char * name)
{
   if (!evas_evgl_thread_enabled() || (_main_thread_id != eina_thread_self()))
     {
        return orig_evgl_api_glGetFragDataLocation(program, name);
     }

   int thread_mode = EVAS_GL_THREAD_MODE_FINISH;

   EVGL_API_Thread_Command_glGetFragDataLocation thread_data_local;
   EVGL_API_Thread_Command_glGetFragDataLocation *thread_data = &thread_data_local;

   thread_data->program = program;
   thread_data->name = name;

   evas_gl_thread_cmd_enqueue(EVAS_GL_THREAD_TYPE_EVGL,
                              _evgl_api_thread_glGetFragDataLocation,
                              thread_data,
                              thread_mode);

   return thread_data->return_value;
}

/*
 * void
 * glGetInteger64i_v(GLenum target, GLuint index, EvasGLint64 * data);
 */

typedef struct
{
   GLenum target;
   GLuint index;
   EvasGLint64 * data;

} EVGL_API_Thread_Command_glGetInteger64i_v;

void (*orig_evgl_api_glGetInteger64i_v)(GLenum target, GLuint index, EvasGLint64 * data);

static void
_evgl_api_thread_glGetInteger64i_v(void *data)
{
   EVGL_API_Thread_Command_glGetInteger64i_v *thread_data =
      (EVGL_API_Thread_Command_glGetInteger64i_v *)data;

   orig_evgl_api_glGetInteger64i_v(thread_data->target,
                                   thread_data->index,
                                   thread_data->data);

}

EAPI void
glGetInteger64i_v_evgl_api_thread_cmd(GLenum target, GLuint index, EvasGLint64 * data)
{
   if (!evas_evgl_thread_enabled() || (_main_thread_id != eina_thread_self()))
     {
        orig_evgl_api_glGetInteger64i_v(target, index, data);
        return;
     }

   int thread_mode = EVAS_GL_THREAD_MODE_FINISH;

   EVGL_API_Thread_Command_glGetInteger64i_v thread_data_local;
   EVGL_API_Thread_Command_glGetInteger64i_v *thread_data = &thread_data_local;

   thread_data->target = target;
   thread_data->index = index;
   thread_data->data = data;

   evas_gl_thread_cmd_enqueue(EVAS_GL_THREAD_TYPE_EVGL,
                              _evgl_api_thread_glGetInteger64i_v,
                              thread_data,
                              thread_mode);
}

/*
 * void
 * glGetInteger64v(GLenum pname, EvasGLint64 * data);
 */

typedef struct
{
   GLenum pname;
   EvasGLint64 * data;

} EVGL_API_Thread_Command_glGetInteger64v;

void (*orig_evgl_api_glGetInteger64v)(GLenum pname, EvasGLint64 * data);

static void
_evgl_api_thread_glGetInteger64v(void *data)
{
   EVGL_API_Thread_Command_glGetInteger64v *thread_data =
      (EVGL_API_Thread_Command_glGetInteger64v *)data;

   orig_evgl_api_glGetInteger64v(thread_data->pname,
                                 thread_data->data);

}

EAPI void
glGetInteger64v_evgl_api_thread_cmd(GLenum pname, EvasGLint64 * data)
{
   if (!evas_evgl_thread_enabled() || (_main_thread_id != eina_thread_self()))
     {
        orig_evgl_api_glGetInteger64v(pname, data);
        return;
     }

   int thread_mode = EVAS_GL_THREAD_MODE_FINISH;

   EVGL_API_Thread_Command_glGetInteger64v thread_data_local;
   EVGL_API_Thread_Command_glGetInteger64v *thread_data = &thread_data_local;

   thread_data->pname = pname;
   thread_data->data = data;

   evas_gl_thread_cmd_enqueue(EVAS_GL_THREAD_TYPE_EVGL,
                              _evgl_api_thread_glGetInteger64v,
                              thread_data,
                              thread_mode);
}

/*
 * void
 * glGetIntegeri_v(GLenum target, GLuint index, GLint * data);
 */

typedef struct
{
   GLenum target;
   GLuint index;
   GLint * data;

} EVGL_API_Thread_Command_glGetIntegeri_v;

void (*orig_evgl_api_glGetIntegeri_v)(GLenum target, GLuint index, GLint * data);

static void
_evgl_api_thread_glGetIntegeri_v(void *data)
{
   EVGL_API_Thread_Command_glGetIntegeri_v *thread_data =
      (EVGL_API_Thread_Command_glGetIntegeri_v *)data;

   orig_evgl_api_glGetIntegeri_v(thread_data->target,
                                 thread_data->index,
                                 thread_data->data);

}

EAPI void
glGetIntegeri_v_evgl_api_thread_cmd(GLenum target, GLuint index, GLint * data)
{
   if (!evas_evgl_thread_enabled() || (_main_thread_id != eina_thread_self()))
     {
        orig_evgl_api_glGetIntegeri_v(target, index, data);
        return;
     }

   int thread_mode = EVAS_GL_THREAD_MODE_FINISH;

   EVGL_API_Thread_Command_glGetIntegeri_v thread_data_local;
   EVGL_API_Thread_Command_glGetIntegeri_v *thread_data = &thread_data_local;

   thread_data->target = target;
   thread_data->index = index;
   thread_data->data = data;

   evas_gl_thread_cmd_enqueue(EVAS_GL_THREAD_TYPE_EVGL,
                              _evgl_api_thread_glGetIntegeri_v,
                              thread_data,
                              thread_mode);
}

/*
 * void
 * glGetInternalformativ(GLenum target, GLenum internalformat, GLenum pname, GLsizei bufSize, GLint *params);
 */

typedef struct
{
   GLenum target;
   GLenum internalformat;
   GLenum pname;
   GLsizei bufSize;
   GLint *params;

} EVGL_API_Thread_Command_glGetInternalformativ;

void (*orig_evgl_api_glGetInternalformativ)(GLenum target, GLenum internalformat, GLenum pname, GLsizei bufSize, GLint *params);

static void
_evgl_api_thread_glGetInternalformativ(void *data)
{
   EVGL_API_Thread_Command_glGetInternalformativ *thread_data =
      (EVGL_API_Thread_Command_glGetInternalformativ *)data;

   orig_evgl_api_glGetInternalformativ(thread_data->target,
                                       thread_data->internalformat,
                                       thread_data->pname,
                                       thread_data->bufSize,
                                       thread_data->params);

}

EAPI void
glGetInternalformativ_evgl_api_thread_cmd(GLenum target, GLenum internalformat, GLenum pname, GLsizei bufSize, GLint *params)
{
   if (!evas_evgl_thread_enabled() || (_main_thread_id != eina_thread_self()))
     {
        orig_evgl_api_glGetInternalformativ(target, internalformat, pname, bufSize, params);
        return;
     }

   int thread_mode = EVAS_GL_THREAD_MODE_FINISH;

   EVGL_API_Thread_Command_glGetInternalformativ thread_data_local;
   EVGL_API_Thread_Command_glGetInternalformativ *thread_data = &thread_data_local;

   thread_data->target = target;
   thread_data->internalformat = internalformat;
   thread_data->pname = pname;
   thread_data->bufSize = bufSize;
   thread_data->params = params;

   evas_gl_thread_cmd_enqueue(EVAS_GL_THREAD_TYPE_EVGL,
                              _evgl_api_thread_glGetInternalformativ,
                              thread_data,
                              thread_mode);
}

/*
 * void
 * glGetProgramBinary(GLuint program, GLsizei bufsize, GLsizei *length, GLenum *binaryFormat, void *binary);
 */

typedef struct
{
   GLuint program;
   GLsizei bufsize;
   GLsizei *length;
   GLenum *binaryFormat;
   void *binary;

} EVGL_API_Thread_Command_glGetProgramBinary;

void (*orig_evgl_api_glGetProgramBinary)(GLuint program, GLsizei bufsize, GLsizei *length, GLenum *binaryFormat, void *binary);

static void
_evgl_api_thread_glGetProgramBinary(void *data)
{
   EVGL_API_Thread_Command_glGetProgramBinary *thread_data =
      (EVGL_API_Thread_Command_glGetProgramBinary *)data;

   orig_evgl_api_glGetProgramBinary(thread_data->program,
                                    thread_data->bufsize,
                                    thread_data->length,
                                    thread_data->binaryFormat,
                                    thread_data->binary);

}

EAPI void
glGetProgramBinary_evgl_api_thread_cmd(GLuint program, GLsizei bufsize, GLsizei *length, GLenum *binaryFormat, void *binary)
{
   if (!evas_evgl_thread_enabled() || (_main_thread_id != eina_thread_self()))
     {
        orig_evgl_api_glGetProgramBinary(program, bufsize, length, binaryFormat, binary);
        return;
     }

   int thread_mode = EVAS_GL_THREAD_MODE_FINISH;

   EVGL_API_Thread_Command_glGetProgramBinary thread_data_local;
   EVGL_API_Thread_Command_glGetProgramBinary *thread_data = &thread_data_local;

   thread_data->program = program;
   thread_data->bufsize = bufsize;
   thread_data->length = length;
   thread_data->binaryFormat = binaryFormat;
   thread_data->binary = binary;

   evas_gl_thread_cmd_enqueue(EVAS_GL_THREAD_TYPE_EVGL,
                              _evgl_api_thread_glGetProgramBinary,
                              thread_data,
                              thread_mode);
}

/*
 * void
 * glGetQueryiv(GLenum target, GLenum pname, GLint * params);
 */

typedef struct
{
   GLenum target;
   GLenum pname;
   GLint * params;

} EVGL_API_Thread_Command_glGetQueryiv;

void (*orig_evgl_api_glGetQueryiv)(GLenum target, GLenum pname, GLint * params);

static void
_evgl_api_thread_glGetQueryiv(void *data)
{
   EVGL_API_Thread_Command_glGetQueryiv *thread_data =
      (EVGL_API_Thread_Command_glGetQueryiv *)data;

   orig_evgl_api_glGetQueryiv(thread_data->target,
                              thread_data->pname,
                              thread_data->params);

}

EAPI void
glGetQueryiv_evgl_api_thread_cmd(GLenum target, GLenum pname, GLint * params)
{
   if (!evas_evgl_thread_enabled() || (_main_thread_id != eina_thread_self()))
     {
        orig_evgl_api_glGetQueryiv(target, pname, params);
        return;
     }

   int thread_mode = EVAS_GL_THREAD_MODE_FINISH;

   EVGL_API_Thread_Command_glGetQueryiv thread_data_local;
   EVGL_API_Thread_Command_glGetQueryiv *thread_data = &thread_data_local;

   thread_data->target = target;
   thread_data->pname = pname;
   thread_data->params = params;

   evas_gl_thread_cmd_enqueue(EVAS_GL_THREAD_TYPE_EVGL,
                              _evgl_api_thread_glGetQueryiv,
                              thread_data,
                              thread_mode);
}

/*
 * void
 * glGetQueryObjectuiv(GLuint id, GLenum pname, GLuint * params);
 */

typedef struct
{
   GLuint id;
   GLenum pname;
   GLuint * params;

} EVGL_API_Thread_Command_glGetQueryObjectuiv;

void (*orig_evgl_api_glGetQueryObjectuiv)(GLuint id, GLenum pname, GLuint * params);

static void
_evgl_api_thread_glGetQueryObjectuiv(void *data)
{
   EVGL_API_Thread_Command_glGetQueryObjectuiv *thread_data =
      (EVGL_API_Thread_Command_glGetQueryObjectuiv *)data;

   orig_evgl_api_glGetQueryObjectuiv(thread_data->id,
                                     thread_data->pname,
                                     thread_data->params);

}

EAPI void
glGetQueryObjectuiv_evgl_api_thread_cmd(GLuint id, GLenum pname, GLuint * params)
{
   if (!evas_evgl_thread_enabled() || (_main_thread_id != eina_thread_self()))
     {
        orig_evgl_api_glGetQueryObjectuiv(id, pname, params);
        return;
     }

   int thread_mode = EVAS_GL_THREAD_MODE_FINISH;

   EVGL_API_Thread_Command_glGetQueryObjectuiv thread_data_local;
   EVGL_API_Thread_Command_glGetQueryObjectuiv *thread_data = &thread_data_local;

   thread_data->id = id;
   thread_data->pname = pname;
   thread_data->params = params;

   evas_gl_thread_cmd_enqueue(EVAS_GL_THREAD_TYPE_EVGL,
                              _evgl_api_thread_glGetQueryObjectuiv,
                              thread_data,
                              thread_mode);
}

/*
 * void
 * glGetSamplerParameterfv(GLuint sampler, GLenum pname, GLfloat * params);
 */

typedef struct
{
   GLuint sampler;
   GLenum pname;
   GLfloat * params;

} EVGL_API_Thread_Command_glGetSamplerParameterfv;

void (*orig_evgl_api_glGetSamplerParameterfv)(GLuint sampler, GLenum pname, GLfloat * params);

static void
_evgl_api_thread_glGetSamplerParameterfv(void *data)
{
   EVGL_API_Thread_Command_glGetSamplerParameterfv *thread_data =
      (EVGL_API_Thread_Command_glGetSamplerParameterfv *)data;

   orig_evgl_api_glGetSamplerParameterfv(thread_data->sampler,
                                         thread_data->pname,
                                         thread_data->params);

}

EAPI void
glGetSamplerParameterfv_evgl_api_thread_cmd(GLuint sampler, GLenum pname, GLfloat * params)
{
   if (!evas_evgl_thread_enabled() || (_main_thread_id != eina_thread_self()))
     {
        orig_evgl_api_glGetSamplerParameterfv(sampler, pname, params);
        return;
     }

   int thread_mode = EVAS_GL_THREAD_MODE_FINISH;

   EVGL_API_Thread_Command_glGetSamplerParameterfv thread_data_local;
   EVGL_API_Thread_Command_glGetSamplerParameterfv *thread_data = &thread_data_local;

   thread_data->sampler = sampler;
   thread_data->pname = pname;
   thread_data->params = params;

   evas_gl_thread_cmd_enqueue(EVAS_GL_THREAD_TYPE_EVGL,
                              _evgl_api_thread_glGetSamplerParameterfv,
                              thread_data,
                              thread_mode);
}

/*
 * void
 * glGetSamplerParameteriv(GLuint sampler, GLenum pname, GLint * params);
 */

typedef struct
{
   GLuint sampler;
   GLenum pname;
   GLint * params;

} EVGL_API_Thread_Command_glGetSamplerParameteriv;

void (*orig_evgl_api_glGetSamplerParameteriv)(GLuint sampler, GLenum pname, GLint * params);

static void
_evgl_api_thread_glGetSamplerParameteriv(void *data)
{
   EVGL_API_Thread_Command_glGetSamplerParameteriv *thread_data =
      (EVGL_API_Thread_Command_glGetSamplerParameteriv *)data;

   orig_evgl_api_glGetSamplerParameteriv(thread_data->sampler,
                                         thread_data->pname,
                                         thread_data->params);

}

EAPI void
glGetSamplerParameteriv_evgl_api_thread_cmd(GLuint sampler, GLenum pname, GLint * params)
{
   if (!evas_evgl_thread_enabled() || (_main_thread_id != eina_thread_self()))
     {
        orig_evgl_api_glGetSamplerParameteriv(sampler, pname, params);
        return;
     }

   int thread_mode = EVAS_GL_THREAD_MODE_FINISH;

   EVGL_API_Thread_Command_glGetSamplerParameteriv thread_data_local;
   EVGL_API_Thread_Command_glGetSamplerParameteriv *thread_data = &thread_data_local;

   thread_data->sampler = sampler;
   thread_data->pname = pname;
   thread_data->params = params;

   evas_gl_thread_cmd_enqueue(EVAS_GL_THREAD_TYPE_EVGL,
                              _evgl_api_thread_glGetSamplerParameteriv,
                              thread_data,
                              thread_mode);
}

/*
 * const GLubyte *
 * glGetStringi(GLenum name, GLuint index);
 */

typedef struct
{
   const GLubyte * return_value;
   GLenum name;
   GLuint index;

} EVGL_API_Thread_Command_glGetStringi;

const GLubyte * (*orig_evgl_api_glGetStringi)(GLenum name, GLuint index);

static void
_evgl_api_thread_glGetStringi(void *data)
{
   EVGL_API_Thread_Command_glGetStringi *thread_data =
      (EVGL_API_Thread_Command_glGetStringi *)data;

   thread_data->return_value = orig_evgl_api_glGetStringi(thread_data->name,
                                                          thread_data->index);

}

EAPI const GLubyte *
glGetStringi_evgl_api_thread_cmd(GLenum name, GLuint index)
{
   if (!evas_evgl_thread_enabled() || (_main_thread_id != eina_thread_self()))
     {
        return orig_evgl_api_glGetStringi(name, index);
     }

   int thread_mode = EVAS_GL_THREAD_MODE_FINISH;

   EVGL_API_Thread_Command_glGetStringi thread_data_local;
   EVGL_API_Thread_Command_glGetStringi *thread_data = &thread_data_local;

   thread_data->name = name;
   thread_data->index = index;

   evas_gl_thread_cmd_enqueue(EVAS_GL_THREAD_TYPE_EVGL,
                              _evgl_api_thread_glGetStringi,
                              thread_data,
                              thread_mode);

   return thread_data->return_value;
}

/*
 * void
 * glGetSynciv(GLsync sync, GLenum pname, GLsizei bufSize, GLsizei *length, GLint *values);
 */

typedef struct
{
   GLsync sync;
   GLenum pname;
   GLsizei bufSize;
   GLsizei *length;
   GLint *values;

} EVGL_API_Thread_Command_glGetSynciv;

void (*orig_evgl_api_glGetSynciv)(GLsync sync, GLenum pname, GLsizei bufSize, GLsizei *length, GLint *values);

static void
_evgl_api_thread_glGetSynciv(void *data)
{
   EVGL_API_Thread_Command_glGetSynciv *thread_data =
      (EVGL_API_Thread_Command_glGetSynciv *)data;

   orig_evgl_api_glGetSynciv(thread_data->sync,
                             thread_data->pname,
                             thread_data->bufSize,
                             thread_data->length,
                             thread_data->values);

}

EAPI void
glGetSynciv_evgl_api_thread_cmd(GLsync sync, GLenum pname, GLsizei bufSize, GLsizei *length, GLint *values)
{
   if (!evas_evgl_thread_enabled() || (_main_thread_id != eina_thread_self()))
     {
        orig_evgl_api_glGetSynciv(sync, pname, bufSize, length, values);
        return;
     }

   int thread_mode = EVAS_GL_THREAD_MODE_FINISH;

   EVGL_API_Thread_Command_glGetSynciv thread_data_local;
   EVGL_API_Thread_Command_glGetSynciv *thread_data = &thread_data_local;

   thread_data->sync = sync;
   thread_data->pname = pname;
   thread_data->bufSize = bufSize;
   thread_data->length = length;
   thread_data->values = values;

   evas_gl_thread_cmd_enqueue(EVAS_GL_THREAD_TYPE_EVGL,
                              _evgl_api_thread_glGetSynciv,
                              thread_data,
                              thread_mode);
}

/*
 * void
 * glGetTransformFeedbackVarying(GLuint program, GLuint index, GLsizei bufSize, GLsizei * length, GLsizei * size, GLenum * type, char * name);
 */

typedef struct
{
   GLuint program;
   GLuint index;
   GLsizei bufSize;
   GLsizei * length;
   GLsizei * size;
   GLenum * type;
   char * name;

} EVGL_API_Thread_Command_glGetTransformFeedbackVarying;

void (*orig_evgl_api_glGetTransformFeedbackVarying)(GLuint program, GLuint index, GLsizei bufSize, GLsizei * length, GLsizei * size, GLenum * type, char * name);

static void
_evgl_api_thread_glGetTransformFeedbackVarying(void *data)
{
   EVGL_API_Thread_Command_glGetTransformFeedbackVarying *thread_data =
      (EVGL_API_Thread_Command_glGetTransformFeedbackVarying *)data;

   orig_evgl_api_glGetTransformFeedbackVarying(thread_data->program,
                                               thread_data->index,
                                               thread_data->bufSize,
                                               thread_data->length,
                                               thread_data->size,
                                               thread_data->type,
                                               thread_data->name);

}

EAPI void
glGetTransformFeedbackVarying_evgl_api_thread_cmd(GLuint program, GLuint index, GLsizei bufSize, GLsizei * length, GLsizei * size, GLenum * type, char * name)
{
   if (!evas_evgl_thread_enabled() || (_main_thread_id != eina_thread_self()))
     {
        orig_evgl_api_glGetTransformFeedbackVarying(program, index, bufSize, length, size, type, name);
        return;
     }

   int thread_mode = EVAS_GL_THREAD_MODE_FINISH;

   EVGL_API_Thread_Command_glGetTransformFeedbackVarying thread_data_local;
   EVGL_API_Thread_Command_glGetTransformFeedbackVarying *thread_data = &thread_data_local;

   thread_data->program = program;
   thread_data->index = index;
   thread_data->bufSize = bufSize;
   thread_data->length = length;
   thread_data->size = size;
   thread_data->type = type;
   thread_data->name = name;

   evas_gl_thread_cmd_enqueue(EVAS_GL_THREAD_TYPE_EVGL,
                              _evgl_api_thread_glGetTransformFeedbackVarying,
                              thread_data,
                              thread_mode);
}

/*
 * GLuint
 * glGetUniformBlockIndex(GLuint program, const GLchar *uniformBlockName);
 */

typedef struct
{
   GLuint return_value;
   GLuint program;
   const GLchar *uniformBlockName;

} EVGL_API_Thread_Command_glGetUniformBlockIndex;

GLuint (*orig_evgl_api_glGetUniformBlockIndex)(GLuint program, const GLchar *uniformBlockName);

static void
_evgl_api_thread_glGetUniformBlockIndex(void *data)
{
   EVGL_API_Thread_Command_glGetUniformBlockIndex *thread_data =
      (EVGL_API_Thread_Command_glGetUniformBlockIndex *)data;

   thread_data->return_value = orig_evgl_api_glGetUniformBlockIndex(thread_data->program,
                                                                    thread_data->uniformBlockName);

}

EAPI GLuint
glGetUniformBlockIndex_evgl_api_thread_cmd(GLuint program, const GLchar *uniformBlockName)
{
   if (!evas_evgl_thread_enabled() || (_main_thread_id != eina_thread_self()))
     {
        return orig_evgl_api_glGetUniformBlockIndex(program, uniformBlockName);
     }

   int thread_mode = EVAS_GL_THREAD_MODE_FINISH;

   EVGL_API_Thread_Command_glGetUniformBlockIndex thread_data_local;
   EVGL_API_Thread_Command_glGetUniformBlockIndex *thread_data = &thread_data_local;

   thread_data->program = program;
   thread_data->uniformBlockName = uniformBlockName;

   evas_gl_thread_cmd_enqueue(EVAS_GL_THREAD_TYPE_EVGL,
                              _evgl_api_thread_glGetUniformBlockIndex,
                              thread_data,
                              thread_mode);

   return thread_data->return_value;
}

/*
 * void
 * glGetUniformIndices(GLuint program, GLsizei uniformCount, const GLchar *const*uniformNames, GLuint *uniformIndices);
 */

typedef struct
{
   GLuint program;
   GLsizei uniformCount;
   const GLchar *const*uniformNames;
   GLuint *uniformIndices;

} EVGL_API_Thread_Command_glGetUniformIndices;

void (*orig_evgl_api_glGetUniformIndices)(GLuint program, GLsizei uniformCount, const GLchar *const*uniformNames, GLuint *uniformIndices);

static void
_evgl_api_thread_glGetUniformIndices(void *data)
{
   EVGL_API_Thread_Command_glGetUniformIndices *thread_data =
      (EVGL_API_Thread_Command_glGetUniformIndices *)data;

   orig_evgl_api_glGetUniformIndices(thread_data->program,
                                     thread_data->uniformCount,
                                     thread_data->uniformNames,
                                     thread_data->uniformIndices);

}

EAPI void
glGetUniformIndices_evgl_api_thread_cmd(GLuint program, GLsizei uniformCount, const GLchar *const*uniformNames, GLuint *uniformIndices)
{
   if (!evas_evgl_thread_enabled() || (_main_thread_id != eina_thread_self()))
     {
        orig_evgl_api_glGetUniformIndices(program, uniformCount, uniformNames, uniformIndices);
        return;
     }

   int thread_mode = EVAS_GL_THREAD_MODE_FINISH;

   EVGL_API_Thread_Command_glGetUniformIndices thread_data_local;
   EVGL_API_Thread_Command_glGetUniformIndices *thread_data = &thread_data_local;

   thread_data->program = program;
   thread_data->uniformCount = uniformCount;
   thread_data->uniformNames = uniformNames;
   thread_data->uniformIndices = uniformIndices;

   evas_gl_thread_cmd_enqueue(EVAS_GL_THREAD_TYPE_EVGL,
                              _evgl_api_thread_glGetUniformIndices,
                              thread_data,
                              thread_mode);
}

/*
 * void
 * glGetUniformuiv(GLuint program, GLint location, GLuint* params);
 */

typedef struct
{
   GLuint program;
   GLint location;
   GLuint* params;

} EVGL_API_Thread_Command_glGetUniformuiv;

void (*orig_evgl_api_glGetUniformuiv)(GLuint program, GLint location, GLuint* params);

static void
_evgl_api_thread_glGetUniformuiv(void *data)
{
   EVGL_API_Thread_Command_glGetUniformuiv *thread_data =
      (EVGL_API_Thread_Command_glGetUniformuiv *)data;

   orig_evgl_api_glGetUniformuiv(thread_data->program,
                                 thread_data->location,
                                 thread_data->params);

}

EAPI void
glGetUniformuiv_evgl_api_thread_cmd(GLuint program, GLint location, GLuint* params)
{
   if (!evas_evgl_thread_enabled() || (_main_thread_id != eina_thread_self()))
     {
        orig_evgl_api_glGetUniformuiv(program, location, params);
        return;
     }

   int thread_mode = EVAS_GL_THREAD_MODE_FINISH;

   EVGL_API_Thread_Command_glGetUniformuiv thread_data_local;
   EVGL_API_Thread_Command_glGetUniformuiv *thread_data = &thread_data_local;

   thread_data->program = program;
   thread_data->location = location;
   thread_data->params = params;

   evas_gl_thread_cmd_enqueue(EVAS_GL_THREAD_TYPE_EVGL,
                              _evgl_api_thread_glGetUniformuiv,
                              thread_data,
                              thread_mode);
}

/*
 * void
 * glGetVertexAttribIiv(GLuint index, GLenum pname, GLint *params);
 */

typedef struct
{
   GLuint index;
   GLenum pname;
   GLint *params;

} EVGL_API_Thread_Command_glGetVertexAttribIiv;

void (*orig_evgl_api_glGetVertexAttribIiv)(GLuint index, GLenum pname, GLint *params);

static void
_evgl_api_thread_glGetVertexAttribIiv(void *data)
{
   EVGL_API_Thread_Command_glGetVertexAttribIiv *thread_data =
      (EVGL_API_Thread_Command_glGetVertexAttribIiv *)data;

   orig_evgl_api_glGetVertexAttribIiv(thread_data->index,
                                      thread_data->pname,
                                      thread_data->params);

}

EAPI void
glGetVertexAttribIiv_evgl_api_thread_cmd(GLuint index, GLenum pname, GLint *params)
{
   if (!evas_evgl_thread_enabled() || (_main_thread_id != eina_thread_self()))
     {
        orig_evgl_api_glGetVertexAttribIiv(index, pname, params);
        return;
     }

   int thread_mode = EVAS_GL_THREAD_MODE_FINISH;

   EVGL_API_Thread_Command_glGetVertexAttribIiv thread_data_local;
   EVGL_API_Thread_Command_glGetVertexAttribIiv *thread_data = &thread_data_local;

   thread_data->index = index;
   thread_data->pname = pname;
   thread_data->params = params;

   evas_gl_thread_cmd_enqueue(EVAS_GL_THREAD_TYPE_EVGL,
                              _evgl_api_thread_glGetVertexAttribIiv,
                              thread_data,
                              thread_mode);
}

/*
 * void
 * glGetVertexAttribIuiv(GLuint index, GLenum pname, GLuint *params);
 */

typedef struct
{
   GLuint index;
   GLenum pname;
   GLuint *params;

} EVGL_API_Thread_Command_glGetVertexAttribIuiv;

void (*orig_evgl_api_glGetVertexAttribIuiv)(GLuint index, GLenum pname, GLuint *params);

static void
_evgl_api_thread_glGetVertexAttribIuiv(void *data)
{
   EVGL_API_Thread_Command_glGetVertexAttribIuiv *thread_data =
      (EVGL_API_Thread_Command_glGetVertexAttribIuiv *)data;

   orig_evgl_api_glGetVertexAttribIuiv(thread_data->index,
                                       thread_data->pname,
                                       thread_data->params);

}

EAPI void
glGetVertexAttribIuiv_evgl_api_thread_cmd(GLuint index, GLenum pname, GLuint *params)
{
   if (!evas_evgl_thread_enabled() || (_main_thread_id != eina_thread_self()))
     {
        orig_evgl_api_glGetVertexAttribIuiv(index, pname, params);
        return;
     }

   int thread_mode = EVAS_GL_THREAD_MODE_FINISH;

   EVGL_API_Thread_Command_glGetVertexAttribIuiv thread_data_local;
   EVGL_API_Thread_Command_glGetVertexAttribIuiv *thread_data = &thread_data_local;

   thread_data->index = index;
   thread_data->pname = pname;
   thread_data->params = params;

   evas_gl_thread_cmd_enqueue(EVAS_GL_THREAD_TYPE_EVGL,
                              _evgl_api_thread_glGetVertexAttribIuiv,
                              thread_data,
                              thread_mode);
}

/*
 * void
 * glInvalidateFramebuffer(GLenum target, GLsizei numAttachments, const GLenum *attachments);
 */

typedef struct
{
   GLenum target;
   GLsizei numAttachments;
   const GLenum *attachments;

} EVGL_API_Thread_Command_glInvalidateFramebuffer;

void (*orig_evgl_api_glInvalidateFramebuffer)(GLenum target, GLsizei numAttachments, const GLenum *attachments);

static void
_evgl_api_thread_glInvalidateFramebuffer(void *data)
{
   EVGL_API_Thread_Command_glInvalidateFramebuffer *thread_data =
      (EVGL_API_Thread_Command_glInvalidateFramebuffer *)data;

   orig_evgl_api_glInvalidateFramebuffer(thread_data->target,
                                         thread_data->numAttachments,
                                         thread_data->attachments);

}

EAPI void
glInvalidateFramebuffer_evgl_api_thread_cmd(GLenum target, GLsizei numAttachments, const GLenum *attachments)
{
   if (!evas_evgl_thread_enabled() || (_main_thread_id != eina_thread_self()))
     {
        orig_evgl_api_glInvalidateFramebuffer(target, numAttachments, attachments);
        return;
     }

   int thread_mode = EVAS_GL_THREAD_MODE_FINISH;

   EVGL_API_Thread_Command_glInvalidateFramebuffer thread_data_local;
   EVGL_API_Thread_Command_glInvalidateFramebuffer *thread_data = &thread_data_local;

   thread_data->target = target;
   thread_data->numAttachments = numAttachments;
   thread_data->attachments = attachments;

   evas_gl_thread_cmd_enqueue(EVAS_GL_THREAD_TYPE_EVGL,
                              _evgl_api_thread_glInvalidateFramebuffer,
                              thread_data,
                              thread_mode);
}

/*
 * void
 * glInvalidateSubFramebuffer(GLenum target, GLsizei numAttachments, const GLenum *attachments, GLint x, GLint y, GLsizei width, GLsizei height);
 */

typedef struct
{
   GLenum target;
   GLsizei numAttachments;
   const GLenum *attachments;
   GLint x;
   GLint y;
   GLsizei width;
   GLsizei height;

} EVGL_API_Thread_Command_glInvalidateSubFramebuffer;

void (*orig_evgl_api_glInvalidateSubFramebuffer)(GLenum target, GLsizei numAttachments, const GLenum *attachments, GLint x, GLint y, GLsizei width, GLsizei height);

static void
_evgl_api_thread_glInvalidateSubFramebuffer(void *data)
{
   EVGL_API_Thread_Command_glInvalidateSubFramebuffer *thread_data =
      (EVGL_API_Thread_Command_glInvalidateSubFramebuffer *)data;

   orig_evgl_api_glInvalidateSubFramebuffer(thread_data->target,
                                            thread_data->numAttachments,
                                            thread_data->attachments,
                                            thread_data->x,
                                            thread_data->y,
                                            thread_data->width,
                                            thread_data->height);

}

EAPI void
glInvalidateSubFramebuffer_evgl_api_thread_cmd(GLenum target, GLsizei numAttachments, const GLenum *attachments, GLint x, GLint y, GLsizei width, GLsizei height)
{
   if (!evas_evgl_thread_enabled() || (_main_thread_id != eina_thread_self()))
     {
        orig_evgl_api_glInvalidateSubFramebuffer(target, numAttachments, attachments, x, y, width, height);
        return;
     }

   int thread_mode = EVAS_GL_THREAD_MODE_FINISH;

   EVGL_API_Thread_Command_glInvalidateSubFramebuffer thread_data_local;
   EVGL_API_Thread_Command_glInvalidateSubFramebuffer *thread_data = &thread_data_local;

   thread_data->target = target;
   thread_data->numAttachments = numAttachments;
   thread_data->attachments = attachments;
   thread_data->x = x;
   thread_data->y = y;
   thread_data->width = width;
   thread_data->height = height;

   evas_gl_thread_cmd_enqueue(EVAS_GL_THREAD_TYPE_EVGL,
                              _evgl_api_thread_glInvalidateSubFramebuffer,
                              thread_data,
                              thread_mode);
}

/*
 * GLboolean
 * glIsQuery(GLuint id);
 */

typedef struct
{
   GLboolean return_value;
   GLuint id;

} EVGL_API_Thread_Command_glIsQuery;

GLboolean (*orig_evgl_api_glIsQuery)(GLuint id);

static void
_evgl_api_thread_glIsQuery(void *data)
{
   EVGL_API_Thread_Command_glIsQuery *thread_data =
      (EVGL_API_Thread_Command_glIsQuery *)data;

   thread_data->return_value = orig_evgl_api_glIsQuery(thread_data->id);

}

EAPI GLboolean
glIsQuery_evgl_api_thread_cmd(GLuint id)
{
   if (!evas_evgl_thread_enabled() || (_main_thread_id != eina_thread_self()))
     {
        return orig_evgl_api_glIsQuery(id);
     }

   int thread_mode = EVAS_GL_THREAD_MODE_FINISH;

   EVGL_API_Thread_Command_glIsQuery thread_data_local;
   EVGL_API_Thread_Command_glIsQuery *thread_data = &thread_data_local;

   thread_data->id = id;

   evas_gl_thread_cmd_enqueue(EVAS_GL_THREAD_TYPE_EVGL,
                              _evgl_api_thread_glIsQuery,
                              thread_data,
                              thread_mode);

   return thread_data->return_value;
}

/*
 * GLboolean
 * glIsSampler(GLuint id);
 */

typedef struct
{
   GLboolean return_value;
   GLuint id;

} EVGL_API_Thread_Command_glIsSampler;

GLboolean (*orig_evgl_api_glIsSampler)(GLuint id);

static void
_evgl_api_thread_glIsSampler(void *data)
{
   EVGL_API_Thread_Command_glIsSampler *thread_data =
      (EVGL_API_Thread_Command_glIsSampler *)data;

   thread_data->return_value = orig_evgl_api_glIsSampler(thread_data->id);

}

EAPI GLboolean
glIsSampler_evgl_api_thread_cmd(GLuint id)
{
   if (!evas_evgl_thread_enabled() || (_main_thread_id != eina_thread_self()))
     {
        return orig_evgl_api_glIsSampler(id);
     }

   int thread_mode = EVAS_GL_THREAD_MODE_FINISH;

   EVGL_API_Thread_Command_glIsSampler thread_data_local;
   EVGL_API_Thread_Command_glIsSampler *thread_data = &thread_data_local;

   thread_data->id = id;

   evas_gl_thread_cmd_enqueue(EVAS_GL_THREAD_TYPE_EVGL,
                              _evgl_api_thread_glIsSampler,
                              thread_data,
                              thread_mode);

   return thread_data->return_value;
}

/*
 * GLboolean
 * glIsSync(GLsync sync);
 */

typedef struct
{
   GLboolean return_value;
   GLsync sync;

} EVGL_API_Thread_Command_glIsSync;

GLboolean (*orig_evgl_api_glIsSync)(GLsync sync);

static void
_evgl_api_thread_glIsSync(void *data)
{
   EVGL_API_Thread_Command_glIsSync *thread_data =
      (EVGL_API_Thread_Command_glIsSync *)data;

   thread_data->return_value = orig_evgl_api_glIsSync(thread_data->sync);

}

EAPI GLboolean
glIsSync_evgl_api_thread_cmd(GLsync sync)
{
   if (!evas_evgl_thread_enabled() || (_main_thread_id != eina_thread_self()))
     {
        return orig_evgl_api_glIsSync(sync);
     }

   int thread_mode = EVAS_GL_THREAD_MODE_FINISH;

   EVGL_API_Thread_Command_glIsSync thread_data_local;
   EVGL_API_Thread_Command_glIsSync *thread_data = &thread_data_local;

   thread_data->sync = sync;

   evas_gl_thread_cmd_enqueue(EVAS_GL_THREAD_TYPE_EVGL,
                              _evgl_api_thread_glIsSync,
                              thread_data,
                              thread_mode);

   return thread_data->return_value;
}

/*
 * GLboolean
 * glIsTransformFeedback(GLuint id);
 */

typedef struct
{
   GLboolean return_value;
   GLuint id;

} EVGL_API_Thread_Command_glIsTransformFeedback;

GLboolean (*orig_evgl_api_glIsTransformFeedback)(GLuint id);

static void
_evgl_api_thread_glIsTransformFeedback(void *data)
{
   EVGL_API_Thread_Command_glIsTransformFeedback *thread_data =
      (EVGL_API_Thread_Command_glIsTransformFeedback *)data;

   thread_data->return_value = orig_evgl_api_glIsTransformFeedback(thread_data->id);

}

EAPI GLboolean
glIsTransformFeedback_evgl_api_thread_cmd(GLuint id)
{
   if (!evas_evgl_thread_enabled() || (_main_thread_id != eina_thread_self()))
     {
        return orig_evgl_api_glIsTransformFeedback(id);
     }

   int thread_mode = EVAS_GL_THREAD_MODE_FINISH;

   EVGL_API_Thread_Command_glIsTransformFeedback thread_data_local;
   EVGL_API_Thread_Command_glIsTransformFeedback *thread_data = &thread_data_local;

   thread_data->id = id;

   evas_gl_thread_cmd_enqueue(EVAS_GL_THREAD_TYPE_EVGL,
                              _evgl_api_thread_glIsTransformFeedback,
                              thread_data,
                              thread_mode);

   return thread_data->return_value;
}

/*
 * GLboolean
 * glIsVertexArray(GLuint array);
 */

typedef struct
{
   GLboolean return_value;
   GLuint array;

} EVGL_API_Thread_Command_glIsVertexArray;

GLboolean (*orig_evgl_api_glIsVertexArray)(GLuint array);

static void
_evgl_api_thread_glIsVertexArray(void *data)
{
   EVGL_API_Thread_Command_glIsVertexArray *thread_data =
      (EVGL_API_Thread_Command_glIsVertexArray *)data;

   thread_data->return_value = orig_evgl_api_glIsVertexArray(thread_data->array);

}

EAPI GLboolean
glIsVertexArray_evgl_api_thread_cmd(GLuint array)
{
   if (!evas_evgl_thread_enabled() || (_main_thread_id != eina_thread_self()))
     {
        return orig_evgl_api_glIsVertexArray(array);
     }

   int thread_mode = EVAS_GL_THREAD_MODE_FINISH;

   EVGL_API_Thread_Command_glIsVertexArray thread_data_local;
   EVGL_API_Thread_Command_glIsVertexArray *thread_data = &thread_data_local;

   thread_data->array = array;

   evas_gl_thread_cmd_enqueue(EVAS_GL_THREAD_TYPE_EVGL,
                              _evgl_api_thread_glIsVertexArray,
                              thread_data,
                              thread_mode);

   return thread_data->return_value;
}

/*
 * void *
 * glMapBufferRange(GLenum target, GLintptr offset, GLsizeiptr length, GLbitfield access);
 */

typedef struct
{
   void * return_value;
   GLenum target;
   GLintptr offset;
   GLsizeiptr length;
   GLbitfield access;

} EVGL_API_Thread_Command_glMapBufferRange;

void * (*orig_evgl_api_glMapBufferRange)(GLenum target, GLintptr offset, GLsizeiptr length, GLbitfield access);

static void
_evgl_api_thread_glMapBufferRange(void *data)
{
   EVGL_API_Thread_Command_glMapBufferRange *thread_data =
      (EVGL_API_Thread_Command_glMapBufferRange *)data;

   thread_data->return_value = orig_evgl_api_glMapBufferRange(thread_data->target,
                                                              thread_data->offset,
                                                              thread_data->length,
                                                              thread_data->access);

}

EAPI void *
glMapBufferRange_evgl_api_thread_cmd(GLenum target, GLintptr offset, GLsizeiptr length, GLbitfield access)
{
   if (!evas_evgl_thread_enabled() || (_main_thread_id != eina_thread_self()))
     {
        return orig_evgl_api_glMapBufferRange(target, offset, length, access);
     }

   int thread_mode = EVAS_GL_THREAD_MODE_FINISH;

   EVGL_API_Thread_Command_glMapBufferRange thread_data_local;
   EVGL_API_Thread_Command_glMapBufferRange *thread_data = &thread_data_local;

   thread_data->target = target;
   thread_data->offset = offset;
   thread_data->length = length;
   thread_data->access = access;

   evas_gl_thread_cmd_enqueue(EVAS_GL_THREAD_TYPE_EVGL,
                              _evgl_api_thread_glMapBufferRange,
                              thread_data,
                              thread_mode);

   return thread_data->return_value;
}

/*
 * void
 * glPauseTransformFeedback(void);
 */

void (*orig_evgl_api_glPauseTransformFeedback)(void);

static void
_evgl_api_thread_glPauseTransformFeedback(void *data EINA_UNUSED)
{
   orig_evgl_api_glPauseTransformFeedback();

}

EAPI void
glPauseTransformFeedback_evgl_api_thread_cmd(void)
{
   if (!evas_evgl_thread_enabled() || (_main_thread_id != eina_thread_self()))
     {
        orig_evgl_api_glPauseTransformFeedback();
        return;
     }

   int thread_mode = EVAS_GL_THREAD_MODE_FINISH;

   evas_gl_thread_cmd_enqueue(EVAS_GL_THREAD_TYPE_EVGL,
                              _evgl_api_thread_glPauseTransformFeedback,
                              NULL,
                              thread_mode);
}

/*
 * void
 * glProgramBinary(GLuint program, GLenum binaryFormat, const void *binary, GLsizei length);
 */

typedef struct
{
   GLuint program;
   GLenum binaryFormat;
   const void *binary;
   GLsizei length;
   void *binary_copied; /* COPIED */
   int command_allocated;

} EVGL_API_Thread_Command_glProgramBinary;

void (*orig_evgl_api_glProgramBinary)(GLuint program, GLenum binaryFormat, const void *binary, GLsizei length);

static void
_evgl_api_thread_glProgramBinary(void *data)
{
   EVGL_API_Thread_Command_glProgramBinary *thread_data =
      (EVGL_API_Thread_Command_glProgramBinary *)data;

   orig_evgl_api_glProgramBinary(thread_data->program,
                                 thread_data->binaryFormat,
                                 thread_data->binary,
                                 thread_data->length);


   if (thread_data->binary_copied)
     eina_mempool_free(_mp_default, thread_data->binary_copied);

   if (thread_data->command_allocated)
     eina_mempool_free(_mp_command, thread_data);
}

EAPI void
glProgramBinary_evgl_api_thread_cmd(GLuint program, GLenum binaryFormat, const void *binary, GLsizei length)
{
   if (!evas_evgl_thread_enabled() || (_main_thread_id != eina_thread_self()))
     {
        orig_evgl_api_glProgramBinary(program, binaryFormat, binary, length);
        return;
     }

   int thread_mode = EVAS_GL_THREAD_MODE_FINISH;

   EVGL_API_Thread_Command_glProgramBinary thread_data_local;
   EVGL_API_Thread_Command_glProgramBinary *thread_data = &thread_data_local;

   /* command_allocated flag init. */
   thread_data->command_allocated = 0;

   if (!evas_gl_thread_force_finish())
     { /* _flush */
        EVGL_API_Thread_Command_glProgramBinary *thread_data_new;
        thread_data_new = eina_mempool_malloc(_mp_command,
                                              sizeof(EVGL_API_Thread_Command_glProgramBinary));
        if (thread_data_new)
          {
             thread_data = thread_data_new;
             thread_data->command_allocated = 1;
             thread_mode = EVAS_GL_THREAD_MODE_FLUSH;
          }
     }

   thread_data->program = program;
   thread_data->binaryFormat = binaryFormat;
   thread_data->binary = binary;
   thread_data->length = length;

   thread_data->binary_copied = NULL;
   if (thread_mode == EVAS_GL_THREAD_MODE_FINISH)
     goto finish;

   /* copy variable */
   if (binary)
     {
        /* 1. check memory size */
        unsigned int copy_size = length;
        if (copy_size > _mp_default_memory_size)
          {
             thread_mode = EVAS_GL_THREAD_MODE_FINISH;
             goto finish;
          }
        /* 2. malloc & copy */
        thread_data->binary_copied = eina_mempool_malloc(_mp_default, copy_size);
        if (thread_data->binary_copied)
          {
             memcpy(thread_data->binary_copied, binary, copy_size);
          }
        else
          {
             thread_mode = EVAS_GL_THREAD_MODE_FINISH;
             goto finish;
          }
        /* 3. replace */
        thread_data->binary = (const void  *)thread_data->binary_copied;
     }
   /* end of copy variable */

finish:
   evas_gl_thread_cmd_enqueue(EVAS_GL_THREAD_TYPE_EVGL,
                              _evgl_api_thread_glProgramBinary,
                              thread_data,
                              thread_mode);
}

/*
 * void
 * glProgramParameteri(GLuint program, GLenum pname, GLint value);
 */

typedef struct
{
   GLuint program;
   GLenum pname;
   GLint value;

} EVGL_API_Thread_Command_glProgramParameteri;

void (*orig_evgl_api_glProgramParameteri)(GLuint program, GLenum pname, GLint value);

static void
_evgl_api_thread_glProgramParameteri(void *data)
{
   EVGL_API_Thread_Command_glProgramParameteri *thread_data =
      (EVGL_API_Thread_Command_glProgramParameteri *)data;

   orig_evgl_api_glProgramParameteri(thread_data->program,
                                     thread_data->pname,
                                     thread_data->value);

}

EAPI void
glProgramParameteri_evgl_api_thread_cmd(GLuint program, GLenum pname, GLint value)
{
   if (!evas_evgl_thread_enabled() || (_main_thread_id != eina_thread_self()))
     {
        orig_evgl_api_glProgramParameteri(program, pname, value);
        return;
     }

   int thread_mode = EVAS_GL_THREAD_MODE_FINISH;

   EVGL_API_Thread_Command_glProgramParameteri thread_data_local;
   EVGL_API_Thread_Command_glProgramParameteri *thread_data = &thread_data_local;

   thread_data->program = program;
   thread_data->pname = pname;
   thread_data->value = value;

   evas_gl_thread_cmd_enqueue(EVAS_GL_THREAD_TYPE_EVGL,
                              _evgl_api_thread_glProgramParameteri,
                              thread_data,
                              thread_mode);
}

/*
 * void
 * glReadBuffer(GLenum src);
 */

typedef struct
{
   GLenum src;

} EVGL_API_Thread_Command_glReadBuffer;

void (*orig_evgl_api_glReadBuffer)(GLenum src);

static void
_evgl_api_thread_glReadBuffer(void *data)
{
   EVGL_API_Thread_Command_glReadBuffer *thread_data =
      (EVGL_API_Thread_Command_glReadBuffer *)data;

   orig_evgl_api_glReadBuffer(thread_data->src);

}

EAPI void
glReadBuffer_evgl_api_thread_cmd(GLenum src)
{
   if (!evas_evgl_thread_enabled() || (_main_thread_id != eina_thread_self()))
     {
        orig_evgl_api_glReadBuffer(src);
        return;
     }

   int thread_mode = EVAS_GL_THREAD_MODE_FINISH;

   EVGL_API_Thread_Command_glReadBuffer thread_data_local;
   EVGL_API_Thread_Command_glReadBuffer *thread_data = &thread_data_local;

   thread_data->src = src;

   evas_gl_thread_cmd_enqueue(EVAS_GL_THREAD_TYPE_EVGL,
                              _evgl_api_thread_glReadBuffer,
                              thread_data,
                              thread_mode);
}

/*
 * void
 * glRenderbufferStorageMultisample(GLenum target, GLsizei samples, GLenum internalformat, GLsizei width, GLsizei height);
 */

typedef struct
{
   GLenum target;
   GLsizei samples;
   GLenum internalformat;
   GLsizei width;
   GLsizei height;

} EVGL_API_Thread_Command_glRenderbufferStorageMultisample;

void (*orig_evgl_api_glRenderbufferStorageMultisample)(GLenum target, GLsizei samples, GLenum internalformat, GLsizei width, GLsizei height);

static void
_evgl_api_thread_glRenderbufferStorageMultisample(void *data)
{
   EVGL_API_Thread_Command_glRenderbufferStorageMultisample *thread_data =
      (EVGL_API_Thread_Command_glRenderbufferStorageMultisample *)data;

   orig_evgl_api_glRenderbufferStorageMultisample(thread_data->target,
                                                  thread_data->samples,
                                                  thread_data->internalformat,
                                                  thread_data->width,
                                                  thread_data->height);

}

EAPI void
glRenderbufferStorageMultisample_evgl_api_thread_cmd(GLenum target, GLsizei samples, GLenum internalformat, GLsizei width, GLsizei height)
{
   if (!evas_evgl_thread_enabled() || (_main_thread_id != eina_thread_self()))
     {
        orig_evgl_api_glRenderbufferStorageMultisample(target, samples, internalformat, width, height);
        return;
     }

   int thread_mode = EVAS_GL_THREAD_MODE_FINISH;

   EVGL_API_Thread_Command_glRenderbufferStorageMultisample thread_data_local;
   EVGL_API_Thread_Command_glRenderbufferStorageMultisample *thread_data = &thread_data_local;

   thread_data->target = target;
   thread_data->samples = samples;
   thread_data->internalformat = internalformat;
   thread_data->width = width;
   thread_data->height = height;

   evas_gl_thread_cmd_enqueue(EVAS_GL_THREAD_TYPE_EVGL,
                              _evgl_api_thread_glRenderbufferStorageMultisample,
                              thread_data,
                              thread_mode);
}

/*
 * void
 * glResumeTransformFeedback(void);
 */

void (*orig_evgl_api_glResumeTransformFeedback)(void);

static void
_evgl_api_thread_glResumeTransformFeedback(void *data EINA_UNUSED)
{
   orig_evgl_api_glResumeTransformFeedback();

}

EAPI void
glResumeTransformFeedback_evgl_api_thread_cmd(void)
{
   if (!evas_evgl_thread_enabled() || (_main_thread_id != eina_thread_self()))
     {
        orig_evgl_api_glResumeTransformFeedback();
        return;
     }

   int thread_mode = EVAS_GL_THREAD_MODE_FINISH;

   evas_gl_thread_cmd_enqueue(EVAS_GL_THREAD_TYPE_EVGL,
                              _evgl_api_thread_glResumeTransformFeedback,
                              NULL,
                              thread_mode);
}

/*
 * void
 * glSamplerParameterf(GLuint sampler, GLenum pname, GLfloat param);
 */

typedef struct
{
   GLuint sampler;
   GLenum pname;
   GLfloat param;

} EVGL_API_Thread_Command_glSamplerParameterf;

void (*orig_evgl_api_glSamplerParameterf)(GLuint sampler, GLenum pname, GLfloat param);

static void
_evgl_api_thread_glSamplerParameterf(void *data)
{
   EVGL_API_Thread_Command_glSamplerParameterf *thread_data =
      (EVGL_API_Thread_Command_glSamplerParameterf *)data;

   orig_evgl_api_glSamplerParameterf(thread_data->sampler,
                                     thread_data->pname,
                                     thread_data->param);

}

EAPI void
glSamplerParameterf_evgl_api_thread_cmd(GLuint sampler, GLenum pname, GLfloat param)
{
   if (!evas_evgl_thread_enabled() || (_main_thread_id != eina_thread_self()))
     {
        orig_evgl_api_glSamplerParameterf(sampler, pname, param);
        return;
     }

   int thread_mode = EVAS_GL_THREAD_MODE_FINISH;

   EVGL_API_Thread_Command_glSamplerParameterf thread_data_local;
   EVGL_API_Thread_Command_glSamplerParameterf *thread_data = &thread_data_local;

   thread_data->sampler = sampler;
   thread_data->pname = pname;
   thread_data->param = param;

   evas_gl_thread_cmd_enqueue(EVAS_GL_THREAD_TYPE_EVGL,
                              _evgl_api_thread_glSamplerParameterf,
                              thread_data,
                              thread_mode);
}

/*
 * void
 * glSamplerParameterfv(GLuint sampler, GLenum pname, const GLfloat * params);
 */

typedef struct
{
   GLuint sampler;
   GLenum pname;
   const GLfloat * params;

} EVGL_API_Thread_Command_glSamplerParameterfv;

void (*orig_evgl_api_glSamplerParameterfv)(GLuint sampler, GLenum pname, const GLfloat * params);

static void
_evgl_api_thread_glSamplerParameterfv(void *data)
{
   EVGL_API_Thread_Command_glSamplerParameterfv *thread_data =
      (EVGL_API_Thread_Command_glSamplerParameterfv *)data;

   orig_evgl_api_glSamplerParameterfv(thread_data->sampler,
                                      thread_data->pname,
                                      thread_data->params);

}

EAPI void
glSamplerParameterfv_evgl_api_thread_cmd(GLuint sampler, GLenum pname, const GLfloat * params)
{
   if (!evas_evgl_thread_enabled() || (_main_thread_id != eina_thread_self()))
     {
        orig_evgl_api_glSamplerParameterfv(sampler, pname, params);
        return;
     }

   int thread_mode = EVAS_GL_THREAD_MODE_FINISH;

   EVGL_API_Thread_Command_glSamplerParameterfv thread_data_local;
   EVGL_API_Thread_Command_glSamplerParameterfv *thread_data = &thread_data_local;

   thread_data->sampler = sampler;
   thread_data->pname = pname;
   thread_data->params = params;

   evas_gl_thread_cmd_enqueue(EVAS_GL_THREAD_TYPE_EVGL,
                              _evgl_api_thread_glSamplerParameterfv,
                              thread_data,
                              thread_mode);
}

/*
 * void
 * glSamplerParameteri(GLuint sampler, GLenum pname, GLint param);
 */

typedef struct
{
   GLuint sampler;
   GLenum pname;
   GLint param;

} EVGL_API_Thread_Command_glSamplerParameteri;

void (*orig_evgl_api_glSamplerParameteri)(GLuint sampler, GLenum pname, GLint param);

static void
_evgl_api_thread_glSamplerParameteri(void *data)
{
   EVGL_API_Thread_Command_glSamplerParameteri *thread_data =
      (EVGL_API_Thread_Command_glSamplerParameteri *)data;

   orig_evgl_api_glSamplerParameteri(thread_data->sampler,
                                     thread_data->pname,
                                     thread_data->param);

}

EAPI void
glSamplerParameteri_evgl_api_thread_cmd(GLuint sampler, GLenum pname, GLint param)
{
   if (!evas_evgl_thread_enabled() || (_main_thread_id != eina_thread_self()))
     {
        orig_evgl_api_glSamplerParameteri(sampler, pname, param);
        return;
     }

   int thread_mode = EVAS_GL_THREAD_MODE_FINISH;

   EVGL_API_Thread_Command_glSamplerParameteri thread_data_local;
   EVGL_API_Thread_Command_glSamplerParameteri *thread_data = &thread_data_local;

   thread_data->sampler = sampler;
   thread_data->pname = pname;
   thread_data->param = param;

   evas_gl_thread_cmd_enqueue(EVAS_GL_THREAD_TYPE_EVGL,
                              _evgl_api_thread_glSamplerParameteri,
                              thread_data,
                              thread_mode);
}

/*
 * void
 * glSamplerParameteriv(GLuint sampler, GLenum pname, const GLint * params);
 */

typedef struct
{
   GLuint sampler;
   GLenum pname;
   const GLint * params;

} EVGL_API_Thread_Command_glSamplerParameteriv;

void (*orig_evgl_api_glSamplerParameteriv)(GLuint sampler, GLenum pname, const GLint * params);

static void
_evgl_api_thread_glSamplerParameteriv(void *data)
{
   EVGL_API_Thread_Command_glSamplerParameteriv *thread_data =
      (EVGL_API_Thread_Command_glSamplerParameteriv *)data;

   orig_evgl_api_glSamplerParameteriv(thread_data->sampler,
                                      thread_data->pname,
                                      thread_data->params);

}

EAPI void
glSamplerParameteriv_evgl_api_thread_cmd(GLuint sampler, GLenum pname, const GLint * params)
{
   if (!evas_evgl_thread_enabled() || (_main_thread_id != eina_thread_self()))
     {
        orig_evgl_api_glSamplerParameteriv(sampler, pname, params);
        return;
     }

   int thread_mode = EVAS_GL_THREAD_MODE_FINISH;

   EVGL_API_Thread_Command_glSamplerParameteriv thread_data_local;
   EVGL_API_Thread_Command_glSamplerParameteriv *thread_data = &thread_data_local;

   thread_data->sampler = sampler;
   thread_data->pname = pname;
   thread_data->params = params;

   evas_gl_thread_cmd_enqueue(EVAS_GL_THREAD_TYPE_EVGL,
                              _evgl_api_thread_glSamplerParameteriv,
                              thread_data,
                              thread_mode);
}

/*
 * void
 * glTexImage3D(GLenum target, GLint level, GLint internalFormat, GLsizei width, GLsizei height, GLsizei depth, GLint border, GLenum format, GLenum type, const GLvoid * data);
 */

typedef struct
{
   GLenum target;
   GLint level;
   GLint internalFormat;
   GLsizei width;
   GLsizei height;
   GLsizei depth;
   GLint border;
   GLenum format;
   GLenum type;
   const GLvoid * data;

} EVGL_API_Thread_Command_glTexImage3D;

void (*orig_evgl_api_glTexImage3D)(GLenum target, GLint level, GLint internalFormat, GLsizei width, GLsizei height, GLsizei depth, GLint border, GLenum format, GLenum type, const GLvoid * data);

static void
_evgl_api_thread_glTexImage3D(void *data)
{
   EVGL_API_Thread_Command_glTexImage3D *thread_data =
      (EVGL_API_Thread_Command_glTexImage3D *)data;

   orig_evgl_api_glTexImage3D(thread_data->target,
                              thread_data->level,
                              thread_data->internalFormat,
                              thread_data->width,
                              thread_data->height,
                              thread_data->depth,
                              thread_data->border,
                              thread_data->format,
                              thread_data->type,
                              thread_data->data);

}

EAPI void
glTexImage3D_evgl_api_thread_cmd(GLenum target, GLint level, GLint internalFormat, GLsizei width, GLsizei height, GLsizei depth, GLint border, GLenum format, GLenum type, const GLvoid * data)
{
   if (!evas_evgl_thread_enabled() || (_main_thread_id != eina_thread_self()))
     {
        orig_evgl_api_glTexImage3D(target, level, internalFormat, width, height, depth, border, format, type, data);
        return;
     }

   int thread_mode = EVAS_GL_THREAD_MODE_FINISH;

   EVGL_API_Thread_Command_glTexImage3D thread_data_local;
   EVGL_API_Thread_Command_glTexImage3D *thread_data = &thread_data_local;

   thread_data->target = target;
   thread_data->level = level;
   thread_data->internalFormat = internalFormat;
   thread_data->width = width;
   thread_data->height = height;
   thread_data->depth = depth;
   thread_data->border = border;
   thread_data->format = format;
   thread_data->type = type;
   thread_data->data = data;

   evas_gl_thread_cmd_enqueue(EVAS_GL_THREAD_TYPE_EVGL,
                              _evgl_api_thread_glTexImage3D,
                              thread_data,
                              thread_mode);
}

/*
 * void
 * glTexStorage2D(GLenum target, GLsizei levels, GLenum internalformat, GLsizei width, GLsizei height);
 */

typedef struct
{
   GLenum target;
   GLsizei levels;
   GLenum internalformat;
   GLsizei width;
   GLsizei height;

} EVGL_API_Thread_Command_glTexStorage2D;

void (*orig_evgl_api_glTexStorage2D)(GLenum target, GLsizei levels, GLenum internalformat, GLsizei width, GLsizei height);

static void
_evgl_api_thread_glTexStorage2D(void *data)
{
   EVGL_API_Thread_Command_glTexStorage2D *thread_data =
      (EVGL_API_Thread_Command_glTexStorage2D *)data;

   orig_evgl_api_glTexStorage2D(thread_data->target,
                                thread_data->levels,
                                thread_data->internalformat,
                                thread_data->width,
                                thread_data->height);

}

EAPI void
glTexStorage2D_evgl_api_thread_cmd(GLenum target, GLsizei levels, GLenum internalformat, GLsizei width, GLsizei height)
{
   if (!evas_evgl_thread_enabled() || (_main_thread_id != eina_thread_self()))
     {
        orig_evgl_api_glTexStorage2D(target, levels, internalformat, width, height);
        return;
     }

   int thread_mode = EVAS_GL_THREAD_MODE_FINISH;

   EVGL_API_Thread_Command_glTexStorage2D thread_data_local;
   EVGL_API_Thread_Command_glTexStorage2D *thread_data = &thread_data_local;

   thread_data->target = target;
   thread_data->levels = levels;
   thread_data->internalformat = internalformat;
   thread_data->width = width;
   thread_data->height = height;

   evas_gl_thread_cmd_enqueue(EVAS_GL_THREAD_TYPE_EVGL,
                              _evgl_api_thread_glTexStorage2D,
                              thread_data,
                              thread_mode);
}

/*
 * void
 * glTexStorage3D(GLenum target, GLsizei levels, GLenum internalformat, GLsizei width, GLsizei height, GLsizei depth);
 */

typedef struct
{
   GLenum target;
   GLsizei levels;
   GLenum internalformat;
   GLsizei width;
   GLsizei height;
   GLsizei depth;

} EVGL_API_Thread_Command_glTexStorage3D;

void (*orig_evgl_api_glTexStorage3D)(GLenum target, GLsizei levels, GLenum internalformat, GLsizei width, GLsizei height, GLsizei depth);

static void
_evgl_api_thread_glTexStorage3D(void *data)
{
   EVGL_API_Thread_Command_glTexStorage3D *thread_data =
      (EVGL_API_Thread_Command_glTexStorage3D *)data;

   orig_evgl_api_glTexStorage3D(thread_data->target,
                                thread_data->levels,
                                thread_data->internalformat,
                                thread_data->width,
                                thread_data->height,
                                thread_data->depth);

}

EAPI void
glTexStorage3D_evgl_api_thread_cmd(GLenum target, GLsizei levels, GLenum internalformat, GLsizei width, GLsizei height, GLsizei depth)
{
   if (!evas_evgl_thread_enabled() || (_main_thread_id != eina_thread_self()))
     {
        orig_evgl_api_glTexStorage3D(target, levels, internalformat, width, height, depth);
        return;
     }

   int thread_mode = EVAS_GL_THREAD_MODE_FINISH;

   EVGL_API_Thread_Command_glTexStorage3D thread_data_local;
   EVGL_API_Thread_Command_glTexStorage3D *thread_data = &thread_data_local;

   thread_data->target = target;
   thread_data->levels = levels;
   thread_data->internalformat = internalformat;
   thread_data->width = width;
   thread_data->height = height;
   thread_data->depth = depth;

   evas_gl_thread_cmd_enqueue(EVAS_GL_THREAD_TYPE_EVGL,
                              _evgl_api_thread_glTexStorage3D,
                              thread_data,
                              thread_mode);
}

/*
 * void
 * glTexSubImage3D(GLenum target, GLint level, GLint xoffset, GLint yoffset, GLint zoffset, GLsizei width, GLsizei height, GLsizei depth, GLenum format, GLenum type, const GLvoid * data);
 */

typedef struct
{
   GLenum target;
   GLint level;
   GLint xoffset;
   GLint yoffset;
   GLint zoffset;
   GLsizei width;
   GLsizei height;
   GLsizei depth;
   GLenum format;
   GLenum type;
   const GLvoid * data;

} EVGL_API_Thread_Command_glTexSubImage3D;

void (*orig_evgl_api_glTexSubImage3D)(GLenum target, GLint level, GLint xoffset, GLint yoffset, GLint zoffset, GLsizei width, GLsizei height, GLsizei depth, GLenum format, GLenum type, const GLvoid * data);

static void
_evgl_api_thread_glTexSubImage3D(void *data)
{
   EVGL_API_Thread_Command_glTexSubImage3D *thread_data =
      (EVGL_API_Thread_Command_glTexSubImage3D *)data;

   orig_evgl_api_glTexSubImage3D(thread_data->target,
                                 thread_data->level,
                                 thread_data->xoffset,
                                 thread_data->yoffset,
                                 thread_data->zoffset,
                                 thread_data->width,
                                 thread_data->height,
                                 thread_data->depth,
                                 thread_data->format,
                                 thread_data->type,
                                 thread_data->data);

}

EAPI void
glTexSubImage3D_evgl_api_thread_cmd(GLenum target, GLint level, GLint xoffset, GLint yoffset, GLint zoffset, GLsizei width, GLsizei height, GLsizei depth, GLenum format, GLenum type, const GLvoid * data)
{
   if (!evas_evgl_thread_enabled() || (_main_thread_id != eina_thread_self()))
     {
        orig_evgl_api_glTexSubImage3D(target, level, xoffset, yoffset, zoffset, width, height, depth, format, type, data);
        return;
     }

   int thread_mode = EVAS_GL_THREAD_MODE_FINISH;

   EVGL_API_Thread_Command_glTexSubImage3D thread_data_local;
   EVGL_API_Thread_Command_glTexSubImage3D *thread_data = &thread_data_local;

   thread_data->target = target;
   thread_data->level = level;
   thread_data->xoffset = xoffset;
   thread_data->yoffset = yoffset;
   thread_data->zoffset = zoffset;
   thread_data->width = width;
   thread_data->height = height;
   thread_data->depth = depth;
   thread_data->format = format;
   thread_data->type = type;
   thread_data->data = data;

   evas_gl_thread_cmd_enqueue(EVAS_GL_THREAD_TYPE_EVGL,
                              _evgl_api_thread_glTexSubImage3D,
                              thread_data,
                              thread_mode);
}

/*
 * void
 * glTransformFeedbackVaryings(GLuint program, GLsizei count, const GLchar *const* varyings, GLenum bufferMode);
 */

typedef struct
{
   GLuint program;
   GLsizei count;
   const GLchar *const* varyings;
   GLenum bufferMode;

} EVGL_API_Thread_Command_glTransformFeedbackVaryings;

void (*orig_evgl_api_glTransformFeedbackVaryings)(GLuint program, GLsizei count, const GLchar *const* varyings, GLenum bufferMode);

static void
_evgl_api_thread_glTransformFeedbackVaryings(void *data)
{
   EVGL_API_Thread_Command_glTransformFeedbackVaryings *thread_data =
      (EVGL_API_Thread_Command_glTransformFeedbackVaryings *)data;

   orig_evgl_api_glTransformFeedbackVaryings(thread_data->program,
                                             thread_data->count,
                                             thread_data->varyings,
                                             thread_data->bufferMode);

}

EAPI void
glTransformFeedbackVaryings_evgl_api_thread_cmd(GLuint program, GLsizei count, const GLchar *const* varyings, GLenum bufferMode)
{
   if (!evas_evgl_thread_enabled() || (_main_thread_id != eina_thread_self()))
     {
        orig_evgl_api_glTransformFeedbackVaryings(program, count, varyings, bufferMode);
        return;
     }

   int thread_mode = EVAS_GL_THREAD_MODE_FINISH;

   EVGL_API_Thread_Command_glTransformFeedbackVaryings thread_data_local;
   EVGL_API_Thread_Command_glTransformFeedbackVaryings *thread_data = &thread_data_local;

   thread_data->program = program;
   thread_data->count = count;
   thread_data->varyings = varyings;
   thread_data->bufferMode = bufferMode;

   evas_gl_thread_cmd_enqueue(EVAS_GL_THREAD_TYPE_EVGL,
                              _evgl_api_thread_glTransformFeedbackVaryings,
                              thread_data,
                              thread_mode);
}

/*
 * void
 * glUniform1ui(GLint location, GLuint v0);
 */

typedef struct
{
   GLint location;
   GLuint v0;

} EVGL_API_Thread_Command_glUniform1ui;

void (*orig_evgl_api_glUniform1ui)(GLint location, GLuint v0);

static void
_evgl_api_thread_glUniform1ui(void *data)
{
   EVGL_API_Thread_Command_glUniform1ui *thread_data =
      (EVGL_API_Thread_Command_glUniform1ui *)data;

   orig_evgl_api_glUniform1ui(thread_data->location,
                              thread_data->v0);

}

EAPI void
glUniform1ui_evgl_api_thread_cmd(GLint location, GLuint v0)
{
   if (!evas_evgl_thread_enabled() || (_main_thread_id != eina_thread_self()))
     {
        orig_evgl_api_glUniform1ui(location, v0);
        return;
     }

   int thread_mode = EVAS_GL_THREAD_MODE_FINISH;

   EVGL_API_Thread_Command_glUniform1ui thread_data_local;
   EVGL_API_Thread_Command_glUniform1ui *thread_data = &thread_data_local;

   thread_data->location = location;
   thread_data->v0 = v0;

   evas_gl_thread_cmd_enqueue(EVAS_GL_THREAD_TYPE_EVGL,
                              _evgl_api_thread_glUniform1ui,
                              thread_data,
                              thread_mode);
}

/*
 * void
 * glUniform1uiv(GLint location, GLsizei count, const GLuint *value);
 */

typedef struct
{
   GLint location;
   GLsizei count;
   const GLuint *value;

} EVGL_API_Thread_Command_glUniform1uiv;

void (*orig_evgl_api_glUniform1uiv)(GLint location, GLsizei count, const GLuint *value);

static void
_evgl_api_thread_glUniform1uiv(void *data)
{
   EVGL_API_Thread_Command_glUniform1uiv *thread_data =
      (EVGL_API_Thread_Command_glUniform1uiv *)data;

   orig_evgl_api_glUniform1uiv(thread_data->location,
                               thread_data->count,
                               thread_data->value);

}

EAPI void
glUniform1uiv_evgl_api_thread_cmd(GLint location, GLsizei count, const GLuint *value)
{
   if (!evas_evgl_thread_enabled() || (_main_thread_id != eina_thread_self()))
     {
        orig_evgl_api_glUniform1uiv(location, count, value);
        return;
     }

   int thread_mode = EVAS_GL_THREAD_MODE_FINISH;

   EVGL_API_Thread_Command_glUniform1uiv thread_data_local;
   EVGL_API_Thread_Command_glUniform1uiv *thread_data = &thread_data_local;

   thread_data->location = location;
   thread_data->count = count;
   thread_data->value = value;

   evas_gl_thread_cmd_enqueue(EVAS_GL_THREAD_TYPE_EVGL,
                              _evgl_api_thread_glUniform1uiv,
                              thread_data,
                              thread_mode);
}

/*
 * void
 * glUniform2ui(GLint location, GLuint v0, GLuint v1);
 */

typedef struct
{
   GLint location;
   GLuint v0;
   GLuint v1;

} EVGL_API_Thread_Command_glUniform2ui;

void (*orig_evgl_api_glUniform2ui)(GLint location, GLuint v0, GLuint v1);

static void
_evgl_api_thread_glUniform2ui(void *data)
{
   EVGL_API_Thread_Command_glUniform2ui *thread_data =
      (EVGL_API_Thread_Command_glUniform2ui *)data;

   orig_evgl_api_glUniform2ui(thread_data->location,
                              thread_data->v0,
                              thread_data->v1);

}

EAPI void
glUniform2ui_evgl_api_thread_cmd(GLint location, GLuint v0, GLuint v1)
{
   if (!evas_evgl_thread_enabled() || (_main_thread_id != eina_thread_self()))
     {
        orig_evgl_api_glUniform2ui(location, v0, v1);
        return;
     }

   int thread_mode = EVAS_GL_THREAD_MODE_FINISH;

   EVGL_API_Thread_Command_glUniform2ui thread_data_local;
   EVGL_API_Thread_Command_glUniform2ui *thread_data = &thread_data_local;

   thread_data->location = location;
   thread_data->v0 = v0;
   thread_data->v1 = v1;

   evas_gl_thread_cmd_enqueue(EVAS_GL_THREAD_TYPE_EVGL,
                              _evgl_api_thread_glUniform2ui,
                              thread_data,
                              thread_mode);
}

/*
 * void
 * glUniform2uiv(GLint location, GLsizei count, const GLuint *value);
 */

typedef struct
{
   GLint location;
   GLsizei count;
   const GLuint *value;

} EVGL_API_Thread_Command_glUniform2uiv;

void (*orig_evgl_api_glUniform2uiv)(GLint location, GLsizei count, const GLuint *value);

static void
_evgl_api_thread_glUniform2uiv(void *data)
{
   EVGL_API_Thread_Command_glUniform2uiv *thread_data =
      (EVGL_API_Thread_Command_glUniform2uiv *)data;

   orig_evgl_api_glUniform2uiv(thread_data->location,
                               thread_data->count,
                               thread_data->value);

}

EAPI void
glUniform2uiv_evgl_api_thread_cmd(GLint location, GLsizei count, const GLuint *value)
{
   if (!evas_evgl_thread_enabled() || (_main_thread_id != eina_thread_self()))
     {
        orig_evgl_api_glUniform2uiv(location, count, value);
        return;
     }

   int thread_mode = EVAS_GL_THREAD_MODE_FINISH;

   EVGL_API_Thread_Command_glUniform2uiv thread_data_local;
   EVGL_API_Thread_Command_glUniform2uiv *thread_data = &thread_data_local;

   thread_data->location = location;
   thread_data->count = count;
   thread_data->value = value;

   evas_gl_thread_cmd_enqueue(EVAS_GL_THREAD_TYPE_EVGL,
                              _evgl_api_thread_glUniform2uiv,
                              thread_data,
                              thread_mode);
}

/*
 * void
 * glUniform3ui(GLint location, GLuint v0, GLuint v1, GLuint v2);
 */

typedef struct
{
   GLint location;
   GLuint v0;
   GLuint v1;
   GLuint v2;

} EVGL_API_Thread_Command_glUniform3ui;

void (*orig_evgl_api_glUniform3ui)(GLint location, GLuint v0, GLuint v1, GLuint v2);

static void
_evgl_api_thread_glUniform3ui(void *data)
{
   EVGL_API_Thread_Command_glUniform3ui *thread_data =
      (EVGL_API_Thread_Command_glUniform3ui *)data;

   orig_evgl_api_glUniform3ui(thread_data->location,
                              thread_data->v0,
                              thread_data->v1,
                              thread_data->v2);

}

EAPI void
glUniform3ui_evgl_api_thread_cmd(GLint location, GLuint v0, GLuint v1, GLuint v2)
{
   if (!evas_evgl_thread_enabled() || (_main_thread_id != eina_thread_self()))
     {
        orig_evgl_api_glUniform3ui(location, v0, v1, v2);
        return;
     }

   int thread_mode = EVAS_GL_THREAD_MODE_FINISH;

   EVGL_API_Thread_Command_glUniform3ui thread_data_local;
   EVGL_API_Thread_Command_glUniform3ui *thread_data = &thread_data_local;

   thread_data->location = location;
   thread_data->v0 = v0;
   thread_data->v1 = v1;
   thread_data->v2 = v2;

   evas_gl_thread_cmd_enqueue(EVAS_GL_THREAD_TYPE_EVGL,
                              _evgl_api_thread_glUniform3ui,
                              thread_data,
                              thread_mode);
}

/*
 * void
 * glUniform3uiv(GLint location, GLsizei count, const GLuint *value);
 */

typedef struct
{
   GLint location;
   GLsizei count;
   const GLuint *value;

} EVGL_API_Thread_Command_glUniform3uiv;

void (*orig_evgl_api_glUniform3uiv)(GLint location, GLsizei count, const GLuint *value);

static void
_evgl_api_thread_glUniform3uiv(void *data)
{
   EVGL_API_Thread_Command_glUniform3uiv *thread_data =
      (EVGL_API_Thread_Command_glUniform3uiv *)data;

   orig_evgl_api_glUniform3uiv(thread_data->location,
                               thread_data->count,
                               thread_data->value);

}

EAPI void
glUniform3uiv_evgl_api_thread_cmd(GLint location, GLsizei count, const GLuint *value)
{
   if (!evas_evgl_thread_enabled() || (_main_thread_id != eina_thread_self()))
     {
        orig_evgl_api_glUniform3uiv(location, count, value);
        return;
     }

   int thread_mode = EVAS_GL_THREAD_MODE_FINISH;

   EVGL_API_Thread_Command_glUniform3uiv thread_data_local;
   EVGL_API_Thread_Command_glUniform3uiv *thread_data = &thread_data_local;

   thread_data->location = location;
   thread_data->count = count;
   thread_data->value = value;

   evas_gl_thread_cmd_enqueue(EVAS_GL_THREAD_TYPE_EVGL,
                              _evgl_api_thread_glUniform3uiv,
                              thread_data,
                              thread_mode);
}

/*
 * void
 * glUniform4ui(GLint location, GLuint v0, GLuint v1, GLuint v2, GLuint v3);
 */

typedef struct
{
   GLint location;
   GLuint v0;
   GLuint v1;
   GLuint v2;
   GLuint v3;

} EVGL_API_Thread_Command_glUniform4ui;

void (*orig_evgl_api_glUniform4ui)(GLint location, GLuint v0, GLuint v1, GLuint v2, GLuint v3);

static void
_evgl_api_thread_glUniform4ui(void *data)
{
   EVGL_API_Thread_Command_glUniform4ui *thread_data =
      (EVGL_API_Thread_Command_glUniform4ui *)data;

   orig_evgl_api_glUniform4ui(thread_data->location,
                              thread_data->v0,
                              thread_data->v1,
                              thread_data->v2,
                              thread_data->v3);

}

EAPI void
glUniform4ui_evgl_api_thread_cmd(GLint location, GLuint v0, GLuint v1, GLuint v2, GLuint v3)
{
   if (!evas_evgl_thread_enabled() || (_main_thread_id != eina_thread_self()))
     {
        orig_evgl_api_glUniform4ui(location, v0, v1, v2, v3);
        return;
     }

   int thread_mode = EVAS_GL_THREAD_MODE_FINISH;

   EVGL_API_Thread_Command_glUniform4ui thread_data_local;
   EVGL_API_Thread_Command_glUniform4ui *thread_data = &thread_data_local;

   thread_data->location = location;
   thread_data->v0 = v0;
   thread_data->v1 = v1;
   thread_data->v2 = v2;
   thread_data->v3 = v3;

   evas_gl_thread_cmd_enqueue(EVAS_GL_THREAD_TYPE_EVGL,
                              _evgl_api_thread_glUniform4ui,
                              thread_data,
                              thread_mode);
}

/*
 * void
 * glUniform4uiv(GLint location, GLsizei count, const GLuint *value);
 */

typedef struct
{
   GLint location;
   GLsizei count;
   const GLuint *value;

} EVGL_API_Thread_Command_glUniform4uiv;

void (*orig_evgl_api_glUniform4uiv)(GLint location, GLsizei count, const GLuint *value);

static void
_evgl_api_thread_glUniform4uiv(void *data)
{
   EVGL_API_Thread_Command_glUniform4uiv *thread_data =
      (EVGL_API_Thread_Command_glUniform4uiv *)data;

   orig_evgl_api_glUniform4uiv(thread_data->location,
                               thread_data->count,
                               thread_data->value);

}

EAPI void
glUniform4uiv_evgl_api_thread_cmd(GLint location, GLsizei count, const GLuint *value)
{
   if (!evas_evgl_thread_enabled() || (_main_thread_id != eina_thread_self()))
     {
        orig_evgl_api_glUniform4uiv(location, count, value);
        return;
     }

   int thread_mode = EVAS_GL_THREAD_MODE_FINISH;

   EVGL_API_Thread_Command_glUniform4uiv thread_data_local;
   EVGL_API_Thread_Command_glUniform4uiv *thread_data = &thread_data_local;

   thread_data->location = location;
   thread_data->count = count;
   thread_data->value = value;

   evas_gl_thread_cmd_enqueue(EVAS_GL_THREAD_TYPE_EVGL,
                              _evgl_api_thread_glUniform4uiv,
                              thread_data,
                              thread_mode);
}

/*
 * void
 * glUniformBlockBinding(GLuint program, GLuint uniformBlockIndex, GLuint uniformBlockBinding);
 */

typedef struct
{
   GLuint program;
   GLuint uniformBlockIndex;
   GLuint uniformBlockBinding;

} EVGL_API_Thread_Command_glUniformBlockBinding;

void (*orig_evgl_api_glUniformBlockBinding)(GLuint program, GLuint uniformBlockIndex, GLuint uniformBlockBinding);

static void
_evgl_api_thread_glUniformBlockBinding(void *data)
{
   EVGL_API_Thread_Command_glUniformBlockBinding *thread_data =
      (EVGL_API_Thread_Command_glUniformBlockBinding *)data;

   orig_evgl_api_glUniformBlockBinding(thread_data->program,
                                       thread_data->uniformBlockIndex,
                                       thread_data->uniformBlockBinding);

}

EAPI void
glUniformBlockBinding_evgl_api_thread_cmd(GLuint program, GLuint uniformBlockIndex, GLuint uniformBlockBinding)
{
   if (!evas_evgl_thread_enabled() || (_main_thread_id != eina_thread_self()))
     {
        orig_evgl_api_glUniformBlockBinding(program, uniformBlockIndex, uniformBlockBinding);
        return;
     }

   int thread_mode = EVAS_GL_THREAD_MODE_FINISH;

   EVGL_API_Thread_Command_glUniformBlockBinding thread_data_local;
   EVGL_API_Thread_Command_glUniformBlockBinding *thread_data = &thread_data_local;

   thread_data->program = program;
   thread_data->uniformBlockIndex = uniformBlockIndex;
   thread_data->uniformBlockBinding = uniformBlockBinding;

   evas_gl_thread_cmd_enqueue(EVAS_GL_THREAD_TYPE_EVGL,
                              _evgl_api_thread_glUniformBlockBinding,
                              thread_data,
                              thread_mode);
}

/*
 * void
 * glUniformMatrix2x3fv(GLint location, GLsizei count, GLboolean transpose, const GLfloat *value);
 */

typedef struct
{
   GLint location;
   GLsizei count;
   GLboolean transpose;
   const GLfloat *value;

} EVGL_API_Thread_Command_glUniformMatrix2x3fv;

void (*orig_evgl_api_glUniformMatrix2x3fv)(GLint location, GLsizei count, GLboolean transpose, const GLfloat *value);

static void
_evgl_api_thread_glUniformMatrix2x3fv(void *data)
{
   EVGL_API_Thread_Command_glUniformMatrix2x3fv *thread_data =
      (EVGL_API_Thread_Command_glUniformMatrix2x3fv *)data;

   orig_evgl_api_glUniformMatrix2x3fv(thread_data->location,
                                      thread_data->count,
                                      thread_data->transpose,
                                      thread_data->value);

}

EAPI void
glUniformMatrix2x3fv_evgl_api_thread_cmd(GLint location, GLsizei count, GLboolean transpose, const GLfloat *value)
{
   if (!evas_evgl_thread_enabled() || (_main_thread_id != eina_thread_self()))
     {
        orig_evgl_api_glUniformMatrix2x3fv(location, count, transpose, value);
        return;
     }

   int thread_mode = EVAS_GL_THREAD_MODE_FINISH;

   EVGL_API_Thread_Command_glUniformMatrix2x3fv thread_data_local;
   EVGL_API_Thread_Command_glUniformMatrix2x3fv *thread_data = &thread_data_local;

   thread_data->location = location;
   thread_data->count = count;
   thread_data->transpose = transpose;
   thread_data->value = value;

   evas_gl_thread_cmd_enqueue(EVAS_GL_THREAD_TYPE_EVGL,
                              _evgl_api_thread_glUniformMatrix2x3fv,
                              thread_data,
                              thread_mode);
}

/*
 * void
 * glUniformMatrix3x2fv(GLint location, GLsizei count, GLboolean transpose, const GLfloat *value);
 */

typedef struct
{
   GLint location;
   GLsizei count;
   GLboolean transpose;
   const GLfloat *value;

} EVGL_API_Thread_Command_glUniformMatrix3x2fv;

void (*orig_evgl_api_glUniformMatrix3x2fv)(GLint location, GLsizei count, GLboolean transpose, const GLfloat *value);

static void
_evgl_api_thread_glUniformMatrix3x2fv(void *data)
{
   EVGL_API_Thread_Command_glUniformMatrix3x2fv *thread_data =
      (EVGL_API_Thread_Command_glUniformMatrix3x2fv *)data;

   orig_evgl_api_glUniformMatrix3x2fv(thread_data->location,
                                      thread_data->count,
                                      thread_data->transpose,
                                      thread_data->value);

}

EAPI void
glUniformMatrix3x2fv_evgl_api_thread_cmd(GLint location, GLsizei count, GLboolean transpose, const GLfloat *value)
{
   if (!evas_evgl_thread_enabled() || (_main_thread_id != eina_thread_self()))
     {
        orig_evgl_api_glUniformMatrix3x2fv(location, count, transpose, value);
        return;
     }

   int thread_mode = EVAS_GL_THREAD_MODE_FINISH;

   EVGL_API_Thread_Command_glUniformMatrix3x2fv thread_data_local;
   EVGL_API_Thread_Command_glUniformMatrix3x2fv *thread_data = &thread_data_local;

   thread_data->location = location;
   thread_data->count = count;
   thread_data->transpose = transpose;
   thread_data->value = value;

   evas_gl_thread_cmd_enqueue(EVAS_GL_THREAD_TYPE_EVGL,
                              _evgl_api_thread_glUniformMatrix3x2fv,
                              thread_data,
                              thread_mode);
}

/*
 * void
 * glUniformMatrix2x4fv(GLint location, GLsizei count, GLboolean transpose, const GLfloat *value);
 */

typedef struct
{
   GLint location;
   GLsizei count;
   GLboolean transpose;
   const GLfloat *value;

} EVGL_API_Thread_Command_glUniformMatrix2x4fv;

void (*orig_evgl_api_glUniformMatrix2x4fv)(GLint location, GLsizei count, GLboolean transpose, const GLfloat *value);

static void
_evgl_api_thread_glUniformMatrix2x4fv(void *data)
{
   EVGL_API_Thread_Command_glUniformMatrix2x4fv *thread_data =
      (EVGL_API_Thread_Command_glUniformMatrix2x4fv *)data;

   orig_evgl_api_glUniformMatrix2x4fv(thread_data->location,
                                      thread_data->count,
                                      thread_data->transpose,
                                      thread_data->value);

}

EAPI void
glUniformMatrix2x4fv_evgl_api_thread_cmd(GLint location, GLsizei count, GLboolean transpose, const GLfloat *value)
{
   if (!evas_evgl_thread_enabled() || (_main_thread_id != eina_thread_self()))
     {
        orig_evgl_api_glUniformMatrix2x4fv(location, count, transpose, value);
        return;
     }

   int thread_mode = EVAS_GL_THREAD_MODE_FINISH;

   EVGL_API_Thread_Command_glUniformMatrix2x4fv thread_data_local;
   EVGL_API_Thread_Command_glUniformMatrix2x4fv *thread_data = &thread_data_local;

   thread_data->location = location;
   thread_data->count = count;
   thread_data->transpose = transpose;
   thread_data->value = value;

   evas_gl_thread_cmd_enqueue(EVAS_GL_THREAD_TYPE_EVGL,
                              _evgl_api_thread_glUniformMatrix2x4fv,
                              thread_data,
                              thread_mode);
}

/*
 * void
 * glUniformMatrix4x2fv(GLint location, GLsizei count, GLboolean transpose, const GLfloat *value);
 */

typedef struct
{
   GLint location;
   GLsizei count;
   GLboolean transpose;
   const GLfloat *value;

} EVGL_API_Thread_Command_glUniformMatrix4x2fv;

void (*orig_evgl_api_glUniformMatrix4x2fv)(GLint location, GLsizei count, GLboolean transpose, const GLfloat *value);

static void
_evgl_api_thread_glUniformMatrix4x2fv(void *data)
{
   EVGL_API_Thread_Command_glUniformMatrix4x2fv *thread_data =
      (EVGL_API_Thread_Command_glUniformMatrix4x2fv *)data;

   orig_evgl_api_glUniformMatrix4x2fv(thread_data->location,
                                      thread_data->count,
                                      thread_data->transpose,
                                      thread_data->value);

}

EAPI void
glUniformMatrix4x2fv_evgl_api_thread_cmd(GLint location, GLsizei count, GLboolean transpose, const GLfloat *value)
{
   if (!evas_evgl_thread_enabled() || (_main_thread_id != eina_thread_self()))
     {
        orig_evgl_api_glUniformMatrix4x2fv(location, count, transpose, value);
        return;
     }

   int thread_mode = EVAS_GL_THREAD_MODE_FINISH;

   EVGL_API_Thread_Command_glUniformMatrix4x2fv thread_data_local;
   EVGL_API_Thread_Command_glUniformMatrix4x2fv *thread_data = &thread_data_local;

   thread_data->location = location;
   thread_data->count = count;
   thread_data->transpose = transpose;
   thread_data->value = value;

   evas_gl_thread_cmd_enqueue(EVAS_GL_THREAD_TYPE_EVGL,
                              _evgl_api_thread_glUniformMatrix4x2fv,
                              thread_data,
                              thread_mode);
}

/*
 * void
 * glUniformMatrix3x4fv(GLint location, GLsizei count, GLboolean transpose, const GLfloat *value);
 */

typedef struct
{
   GLint location;
   GLsizei count;
   GLboolean transpose;
   const GLfloat *value;

} EVGL_API_Thread_Command_glUniformMatrix3x4fv;

void (*orig_evgl_api_glUniformMatrix3x4fv)(GLint location, GLsizei count, GLboolean transpose, const GLfloat *value);

static void
_evgl_api_thread_glUniformMatrix3x4fv(void *data)
{
   EVGL_API_Thread_Command_glUniformMatrix3x4fv *thread_data =
      (EVGL_API_Thread_Command_glUniformMatrix3x4fv *)data;

   orig_evgl_api_glUniformMatrix3x4fv(thread_data->location,
                                      thread_data->count,
                                      thread_data->transpose,
                                      thread_data->value);

}

EAPI void
glUniformMatrix3x4fv_evgl_api_thread_cmd(GLint location, GLsizei count, GLboolean transpose, const GLfloat *value)
{
   if (!evas_evgl_thread_enabled() || (_main_thread_id != eina_thread_self()))
     {
        orig_evgl_api_glUniformMatrix3x4fv(location, count, transpose, value);
        return;
     }

   int thread_mode = EVAS_GL_THREAD_MODE_FINISH;

   EVGL_API_Thread_Command_glUniformMatrix3x4fv thread_data_local;
   EVGL_API_Thread_Command_glUniformMatrix3x4fv *thread_data = &thread_data_local;

   thread_data->location = location;
   thread_data->count = count;
   thread_data->transpose = transpose;
   thread_data->value = value;

   evas_gl_thread_cmd_enqueue(EVAS_GL_THREAD_TYPE_EVGL,
                              _evgl_api_thread_glUniformMatrix3x4fv,
                              thread_data,
                              thread_mode);
}

/*
 * void
 * glUniformMatrix4x3fv(GLint location, GLsizei count, GLboolean transpose, const GLfloat *value);
 */

typedef struct
{
   GLint location;
   GLsizei count;
   GLboolean transpose;
   const GLfloat *value;

} EVGL_API_Thread_Command_glUniformMatrix4x3fv;

void (*orig_evgl_api_glUniformMatrix4x3fv)(GLint location, GLsizei count, GLboolean transpose, const GLfloat *value);

static void
_evgl_api_thread_glUniformMatrix4x3fv(void *data)
{
   EVGL_API_Thread_Command_glUniformMatrix4x3fv *thread_data =
      (EVGL_API_Thread_Command_glUniformMatrix4x3fv *)data;

   orig_evgl_api_glUniformMatrix4x3fv(thread_data->location,
                                      thread_data->count,
                                      thread_data->transpose,
                                      thread_data->value);

}

EAPI void
glUniformMatrix4x3fv_evgl_api_thread_cmd(GLint location, GLsizei count, GLboolean transpose, const GLfloat *value)
{
   if (!evas_evgl_thread_enabled() || (_main_thread_id != eina_thread_self()))
     {
        orig_evgl_api_glUniformMatrix4x3fv(location, count, transpose, value);
        return;
     }

   int thread_mode = EVAS_GL_THREAD_MODE_FINISH;

   EVGL_API_Thread_Command_glUniformMatrix4x3fv thread_data_local;
   EVGL_API_Thread_Command_glUniformMatrix4x3fv *thread_data = &thread_data_local;

   thread_data->location = location;
   thread_data->count = count;
   thread_data->transpose = transpose;
   thread_data->value = value;

   evas_gl_thread_cmd_enqueue(EVAS_GL_THREAD_TYPE_EVGL,
                              _evgl_api_thread_glUniformMatrix4x3fv,
                              thread_data,
                              thread_mode);
}

/*
 * GLboolean
 * glUnmapBuffer(GLenum target);
 */

typedef struct
{
   GLboolean return_value;
   GLenum target;

} EVGL_API_Thread_Command_glUnmapBuffer;

GLboolean (*orig_evgl_api_glUnmapBuffer)(GLenum target);

static void
_evgl_api_thread_glUnmapBuffer(void *data)
{
   EVGL_API_Thread_Command_glUnmapBuffer *thread_data =
      (EVGL_API_Thread_Command_glUnmapBuffer *)data;

   thread_data->return_value = orig_evgl_api_glUnmapBuffer(thread_data->target);

}

EAPI GLboolean
glUnmapBuffer_evgl_api_thread_cmd(GLenum target)
{
   if (!evas_evgl_thread_enabled() || (_main_thread_id != eina_thread_self()))
     {
        return orig_evgl_api_glUnmapBuffer(target);
     }

   int thread_mode = EVAS_GL_THREAD_MODE_FINISH;

   EVGL_API_Thread_Command_glUnmapBuffer thread_data_local;
   EVGL_API_Thread_Command_glUnmapBuffer *thread_data = &thread_data_local;

   thread_data->target = target;

   evas_gl_thread_cmd_enqueue(EVAS_GL_THREAD_TYPE_EVGL,
                              _evgl_api_thread_glUnmapBuffer,
                              thread_data,
                              thread_mode);

   return thread_data->return_value;
}

/*
 * void
 * glVertexAttribDivisor(GLuint index, GLuint divisor);
 */

typedef struct
{
   GLuint index;
   GLuint divisor;

} EVGL_API_Thread_Command_glVertexAttribDivisor;

void (*orig_evgl_api_glVertexAttribDivisor)(GLuint index, GLuint divisor);

static void
_evgl_api_thread_glVertexAttribDivisor(void *data)
{
   EVGL_API_Thread_Command_glVertexAttribDivisor *thread_data =
      (EVGL_API_Thread_Command_glVertexAttribDivisor *)data;

   orig_evgl_api_glVertexAttribDivisor(thread_data->index,
                                       thread_data->divisor);

}

EAPI void
glVertexAttribDivisor_evgl_api_thread_cmd(GLuint index, GLuint divisor)
{
   if (!evas_evgl_thread_enabled() || (_main_thread_id != eina_thread_self()))
     {
        orig_evgl_api_glVertexAttribDivisor(index, divisor);
        return;
     }

   int thread_mode = EVAS_GL_THREAD_MODE_FINISH;

   EVGL_API_Thread_Command_glVertexAttribDivisor thread_data_local;
   EVGL_API_Thread_Command_glVertexAttribDivisor *thread_data = &thread_data_local;

   thread_data->index = index;
   thread_data->divisor = divisor;

   evas_gl_thread_cmd_enqueue(EVAS_GL_THREAD_TYPE_EVGL,
                              _evgl_api_thread_glVertexAttribDivisor,
                              thread_data,
                              thread_mode);
}

/*
 * void
 * glVertexAttribI4i(GLuint index, GLint v0, GLint v1, GLint v2, GLint v3);
 */

typedef struct
{
   GLuint index;
   GLint v0;
   GLint v1;
   GLint v2;
   GLint v3;

} EVGL_API_Thread_Command_glVertexAttribI4i;

void (*orig_evgl_api_glVertexAttribI4i)(GLuint index, GLint v0, GLint v1, GLint v2, GLint v3);

static void
_evgl_api_thread_glVertexAttribI4i(void *data)
{
   EVGL_API_Thread_Command_glVertexAttribI4i *thread_data =
      (EVGL_API_Thread_Command_glVertexAttribI4i *)data;

   orig_evgl_api_glVertexAttribI4i(thread_data->index,
                                   thread_data->v0,
                                   thread_data->v1,
                                   thread_data->v2,
                                   thread_data->v3);

}

EAPI void
glVertexAttribI4i_evgl_api_thread_cmd(GLuint index, GLint v0, GLint v1, GLint v2, GLint v3)
{
   if (!evas_evgl_thread_enabled() || (_main_thread_id != eina_thread_self()))
     {
        orig_evgl_api_glVertexAttribI4i(index, v0, v1, v2, v3);
        return;
     }

   int thread_mode = EVAS_GL_THREAD_MODE_FINISH;

   EVGL_API_Thread_Command_glVertexAttribI4i thread_data_local;
   EVGL_API_Thread_Command_glVertexAttribI4i *thread_data = &thread_data_local;

   thread_data->index = index;
   thread_data->v0 = v0;
   thread_data->v1 = v1;
   thread_data->v2 = v2;
   thread_data->v3 = v3;

   evas_gl_thread_cmd_enqueue(EVAS_GL_THREAD_TYPE_EVGL,
                              _evgl_api_thread_glVertexAttribI4i,
                              thread_data,
                              thread_mode);
}

/*
 * void
 * glVertexAttribI4iv(GLuint index, const GLint *v);
 */

typedef struct
{
   GLuint index;
   const GLint *v;

} EVGL_API_Thread_Command_glVertexAttribI4iv;

void (*orig_evgl_api_glVertexAttribI4iv)(GLuint index, const GLint *v);

static void
_evgl_api_thread_glVertexAttribI4iv(void *data)
{
   EVGL_API_Thread_Command_glVertexAttribI4iv *thread_data =
      (EVGL_API_Thread_Command_glVertexAttribI4iv *)data;

   orig_evgl_api_glVertexAttribI4iv(thread_data->index,
                                    thread_data->v);

}

EAPI void
glVertexAttribI4iv_evgl_api_thread_cmd(GLuint index, const GLint *v)
{
   if (!evas_evgl_thread_enabled() || (_main_thread_id != eina_thread_self()))
     {
        orig_evgl_api_glVertexAttribI4iv(index, v);
        return;
     }

   int thread_mode = EVAS_GL_THREAD_MODE_FINISH;

   EVGL_API_Thread_Command_glVertexAttribI4iv thread_data_local;
   EVGL_API_Thread_Command_glVertexAttribI4iv *thread_data = &thread_data_local;

   thread_data->index = index;
   thread_data->v = v;

   evas_gl_thread_cmd_enqueue(EVAS_GL_THREAD_TYPE_EVGL,
                              _evgl_api_thread_glVertexAttribI4iv,
                              thread_data,
                              thread_mode);
}

/*
 * void
 * glVertexAttribI4ui(GLuint index, GLuint v0, GLuint v1, GLuint v2, GLuint v3);
 */

typedef struct
{
   GLuint index;
   GLuint v0;
   GLuint v1;
   GLuint v2;
   GLuint v3;

} EVGL_API_Thread_Command_glVertexAttribI4ui;

void (*orig_evgl_api_glVertexAttribI4ui)(GLuint index, GLuint v0, GLuint v1, GLuint v2, GLuint v3);

static void
_evgl_api_thread_glVertexAttribI4ui(void *data)
{
   EVGL_API_Thread_Command_glVertexAttribI4ui *thread_data =
      (EVGL_API_Thread_Command_glVertexAttribI4ui *)data;

   orig_evgl_api_glVertexAttribI4ui(thread_data->index,
                                    thread_data->v0,
                                    thread_data->v1,
                                    thread_data->v2,
                                    thread_data->v3);

}

EAPI void
glVertexAttribI4ui_evgl_api_thread_cmd(GLuint index, GLuint v0, GLuint v1, GLuint v2, GLuint v3)
{
   if (!evas_evgl_thread_enabled() || (_main_thread_id != eina_thread_self()))
     {
        orig_evgl_api_glVertexAttribI4ui(index, v0, v1, v2, v3);
        return;
     }

   int thread_mode = EVAS_GL_THREAD_MODE_FINISH;

   EVGL_API_Thread_Command_glVertexAttribI4ui thread_data_local;
   EVGL_API_Thread_Command_glVertexAttribI4ui *thread_data = &thread_data_local;

   thread_data->index = index;
   thread_data->v0 = v0;
   thread_data->v1 = v1;
   thread_data->v2 = v2;
   thread_data->v3 = v3;

   evas_gl_thread_cmd_enqueue(EVAS_GL_THREAD_TYPE_EVGL,
                              _evgl_api_thread_glVertexAttribI4ui,
                              thread_data,
                              thread_mode);
}

/*
 * void
 * glVertexAttribI4uiv(GLuint index, const GLuint *v);
 */

typedef struct
{
   GLuint index;
   const GLuint *v;

} EVGL_API_Thread_Command_glVertexAttribI4uiv;

void (*orig_evgl_api_glVertexAttribI4uiv)(GLuint index, const GLuint *v);

static void
_evgl_api_thread_glVertexAttribI4uiv(void *data)
{
   EVGL_API_Thread_Command_glVertexAttribI4uiv *thread_data =
      (EVGL_API_Thread_Command_glVertexAttribI4uiv *)data;

   orig_evgl_api_glVertexAttribI4uiv(thread_data->index,
                                     thread_data->v);

}

EAPI void
glVertexAttribI4uiv_evgl_api_thread_cmd(GLuint index, const GLuint *v)
{
   if (!evas_evgl_thread_enabled() || (_main_thread_id != eina_thread_self()))
     {
        orig_evgl_api_glVertexAttribI4uiv(index, v);
        return;
     }

   int thread_mode = EVAS_GL_THREAD_MODE_FINISH;

   EVGL_API_Thread_Command_glVertexAttribI4uiv thread_data_local;
   EVGL_API_Thread_Command_glVertexAttribI4uiv *thread_data = &thread_data_local;

   thread_data->index = index;
   thread_data->v = v;

   evas_gl_thread_cmd_enqueue(EVAS_GL_THREAD_TYPE_EVGL,
                              _evgl_api_thread_glVertexAttribI4uiv,
                              thread_data,
                              thread_mode);
}

/*
 * void
 * glVertexAttribIPointer(GLuint index, GLint size, GLenum type, GLsizei stride, const GLvoid *pointer);
 */

typedef struct
{
   GLuint index;
   GLint size;
   GLenum type;
   GLsizei stride;
   const GLvoid *pointer;

} EVGL_API_Thread_Command_glVertexAttribIPointer;

void (*orig_evgl_api_glVertexAttribIPointer)(GLuint index, GLint size, GLenum type, GLsizei stride, const GLvoid *pointer);

static void
_evgl_api_thread_glVertexAttribIPointer(void *data)
{
   EVGL_API_Thread_Command_glVertexAttribIPointer *thread_data =
      (EVGL_API_Thread_Command_glVertexAttribIPointer *)data;

   orig_evgl_api_glVertexAttribIPointer(thread_data->index,
                                        thread_data->size,
                                        thread_data->type,
                                        thread_data->stride,
                                        thread_data->pointer);

}

EAPI void
glVertexAttribIPointer_evgl_api_thread_cmd(GLuint index, GLint size, GLenum type, GLsizei stride, const GLvoid *pointer)
{
   if (!evas_evgl_thread_enabled() || (_main_thread_id != eina_thread_self()))
     {
        orig_evgl_api_glVertexAttribIPointer(index, size, type, stride, pointer);
        return;
     }

   int thread_mode = EVAS_GL_THREAD_MODE_FINISH;

   EVGL_API_Thread_Command_glVertexAttribIPointer thread_data_local;
   EVGL_API_Thread_Command_glVertexAttribIPointer *thread_data = &thread_data_local;

   thread_data->index = index;
   thread_data->size = size;
   thread_data->type = type;
   thread_data->stride = stride;
   thread_data->pointer = pointer;

   evas_gl_thread_cmd_enqueue(EVAS_GL_THREAD_TYPE_EVGL,
                              _evgl_api_thread_glVertexAttribIPointer,
                              thread_data,
                              thread_mode);
}

/*
 * void
 * glWaitSync(GLsync sync, GLbitfield flags, EvasGLuint64 timeout);
 */

typedef struct
{
   GLsync sync;
   GLbitfield flags;
   EvasGLuint64 timeout;

} EVGL_API_Thread_Command_glWaitSync;

void (*orig_evgl_api_glWaitSync)(GLsync sync, GLbitfield flags, EvasGLuint64 timeout);

static void
_evgl_api_thread_glWaitSync(void *data)
{
   EVGL_API_Thread_Command_glWaitSync *thread_data =
      (EVGL_API_Thread_Command_glWaitSync *)data;

   orig_evgl_api_glWaitSync(thread_data->sync,
                            thread_data->flags,
                            thread_data->timeout);

}

EAPI void
glWaitSync_evgl_api_thread_cmd(GLsync sync, GLbitfield flags, EvasGLuint64 timeout)
{
   if (!evas_evgl_thread_enabled() || (_main_thread_id != eina_thread_self()))
     {
        orig_evgl_api_glWaitSync(sync, flags, timeout);
        return;
     }

   int thread_mode = EVAS_GL_THREAD_MODE_FINISH;

   EVGL_API_Thread_Command_glWaitSync thread_data_local;
   EVGL_API_Thread_Command_glWaitSync *thread_data = &thread_data_local;

   thread_data->sync = sync;
   thread_data->flags = flags;
   thread_data->timeout = timeout;

   evas_gl_thread_cmd_enqueue(EVAS_GL_THREAD_TYPE_EVGL,
                              _evgl_api_thread_glWaitSync,
                              thread_data,
                              thread_mode);
}

/*
 * void
 * glDispatchCompute(GLuint num_groups_x, GLuint num_groups_y, GLuint num_groups_z);
 */

typedef struct
{
   GLuint num_groups_x;
   GLuint num_groups_y;
   GLuint num_groups_z;

} EVGL_API_Thread_Command_glDispatchCompute;

void (*orig_evgl_api_glDispatchCompute)(GLuint num_groups_x, GLuint num_groups_y, GLuint num_groups_z);

static void
_evgl_api_thread_glDispatchCompute(void *data)
{
   EVGL_API_Thread_Command_glDispatchCompute *thread_data =
      (EVGL_API_Thread_Command_glDispatchCompute *)data;

   orig_evgl_api_glDispatchCompute(thread_data->num_groups_x,
                                   thread_data->num_groups_y,
                                   thread_data->num_groups_z);

}

EAPI void
glDispatchCompute_evgl_api_thread_cmd(GLuint num_groups_x, GLuint num_groups_y, GLuint num_groups_z)
{
   if (!evas_evgl_thread_enabled() || (_main_thread_id != eina_thread_self()))
     {
        orig_evgl_api_glDispatchCompute(num_groups_x, num_groups_y, num_groups_z);
        return;
     }

   int thread_mode = EVAS_GL_THREAD_MODE_FINISH;

   EVGL_API_Thread_Command_glDispatchCompute thread_data_local;
   EVGL_API_Thread_Command_glDispatchCompute *thread_data = &thread_data_local;

   thread_data->num_groups_x = num_groups_x;
   thread_data->num_groups_y = num_groups_y;
   thread_data->num_groups_z = num_groups_z;

   evas_gl_thread_cmd_enqueue(EVAS_GL_THREAD_TYPE_EVGL,
                              _evgl_api_thread_glDispatchCompute,
                              thread_data,
                              thread_mode);
}

/*
 * void
 * glDispatchComputeIndirect(GLintptr indirect);
 */

typedef struct
{
   GLintptr indirect;

} EVGL_API_Thread_Command_glDispatchComputeIndirect;

void (*orig_evgl_api_glDispatchComputeIndirect)(GLintptr indirect);

static void
_evgl_api_thread_glDispatchComputeIndirect(void *data)
{
   EVGL_API_Thread_Command_glDispatchComputeIndirect *thread_data =
      (EVGL_API_Thread_Command_glDispatchComputeIndirect *)data;

   orig_evgl_api_glDispatchComputeIndirect(thread_data->indirect);

}

EAPI void
glDispatchComputeIndirect_evgl_api_thread_cmd(GLintptr indirect)
{
   if (!evas_evgl_thread_enabled() || (_main_thread_id != eina_thread_self()))
     {
        orig_evgl_api_glDispatchComputeIndirect(indirect);
        return;
     }

   int thread_mode = EVAS_GL_THREAD_MODE_FINISH;

   EVGL_API_Thread_Command_glDispatchComputeIndirect thread_data_local;
   EVGL_API_Thread_Command_glDispatchComputeIndirect *thread_data = &thread_data_local;

   thread_data->indirect = indirect;

   evas_gl_thread_cmd_enqueue(EVAS_GL_THREAD_TYPE_EVGL,
                              _evgl_api_thread_glDispatchComputeIndirect,
                              thread_data,
                              thread_mode);
}

/*
 * void
 * glDrawArraysIndirect(GLenum mode, const void *indirect);
 */

typedef struct
{
   GLenum mode;
   const void *indirect;

} EVGL_API_Thread_Command_glDrawArraysIndirect;

void (*orig_evgl_api_glDrawArraysIndirect)(GLenum mode, const void *indirect);

static void
_evgl_api_thread_glDrawArraysIndirect(void *data)
{
   EVGL_API_Thread_Command_glDrawArraysIndirect *thread_data =
      (EVGL_API_Thread_Command_glDrawArraysIndirect *)data;

   orig_evgl_api_glDrawArraysIndirect(thread_data->mode,
                                      thread_data->indirect);

}

EAPI void
glDrawArraysIndirect_evgl_api_thread_cmd(GLenum mode, const void *indirect)
{
   if (!evas_evgl_thread_enabled() || (_main_thread_id != eina_thread_self()))
     {
        orig_evgl_api_glDrawArraysIndirect(mode, indirect);
        return;
     }

   int thread_mode = EVAS_GL_THREAD_MODE_FINISH;

   EVGL_API_Thread_Command_glDrawArraysIndirect thread_data_local;
   EVGL_API_Thread_Command_glDrawArraysIndirect *thread_data = &thread_data_local;

   thread_data->mode = mode;
   thread_data->indirect = indirect;

   evas_gl_thread_cmd_enqueue(EVAS_GL_THREAD_TYPE_EVGL,
                              _evgl_api_thread_glDrawArraysIndirect,
                              thread_data,
                              thread_mode);
}

/*
 * void
 * glDrawElementsIndirect(GLenum mode, GLenum type, const void *indirect);
 */

typedef struct
{
   GLenum mode;
   GLenum type;
   const void *indirect;

} EVGL_API_Thread_Command_glDrawElementsIndirect;

void (*orig_evgl_api_glDrawElementsIndirect)(GLenum mode, GLenum type, const void *indirect);

static void
_evgl_api_thread_glDrawElementsIndirect(void *data)
{
   EVGL_API_Thread_Command_glDrawElementsIndirect *thread_data =
      (EVGL_API_Thread_Command_glDrawElementsIndirect *)data;

   orig_evgl_api_glDrawElementsIndirect(thread_data->mode,
                                        thread_data->type,
                                        thread_data->indirect);

}

EAPI void
glDrawElementsIndirect_evgl_api_thread_cmd(GLenum mode, GLenum type, const void *indirect)
{
   if (!evas_evgl_thread_enabled() || (_main_thread_id != eina_thread_self()))
     {
        orig_evgl_api_glDrawElementsIndirect(mode, type, indirect);
        return;
     }

   int thread_mode = EVAS_GL_THREAD_MODE_FINISH;

   EVGL_API_Thread_Command_glDrawElementsIndirect thread_data_local;
   EVGL_API_Thread_Command_glDrawElementsIndirect *thread_data = &thread_data_local;

   thread_data->mode = mode;
   thread_data->type = type;
   thread_data->indirect = indirect;

   evas_gl_thread_cmd_enqueue(EVAS_GL_THREAD_TYPE_EVGL,
                              _evgl_api_thread_glDrawElementsIndirect,
                              thread_data,
                              thread_mode);
}

/*
 * void
 * glFramebufferParameteri(GLenum target, GLenum pname, GLint param);
 */

typedef struct
{
   GLenum target;
   GLenum pname;
   GLint param;

} EVGL_API_Thread_Command_glFramebufferParameteri;

void (*orig_evgl_api_glFramebufferParameteri)(GLenum target, GLenum pname, GLint param);

static void
_evgl_api_thread_glFramebufferParameteri(void *data)
{
   EVGL_API_Thread_Command_glFramebufferParameteri *thread_data =
      (EVGL_API_Thread_Command_glFramebufferParameteri *)data;

   orig_evgl_api_glFramebufferParameteri(thread_data->target,
                                         thread_data->pname,
                                         thread_data->param);

}

EAPI void
glFramebufferParameteri_evgl_api_thread_cmd(GLenum target, GLenum pname, GLint param)
{
   if (!evas_evgl_thread_enabled() || (_main_thread_id != eina_thread_self()))
     {
        orig_evgl_api_glFramebufferParameteri(target, pname, param);
        return;
     }

   int thread_mode = EVAS_GL_THREAD_MODE_FINISH;

   EVGL_API_Thread_Command_glFramebufferParameteri thread_data_local;
   EVGL_API_Thread_Command_glFramebufferParameteri *thread_data = &thread_data_local;

   thread_data->target = target;
   thread_data->pname = pname;
   thread_data->param = param;

   evas_gl_thread_cmd_enqueue(EVAS_GL_THREAD_TYPE_EVGL,
                              _evgl_api_thread_glFramebufferParameteri,
                              thread_data,
                              thread_mode);
}

/*
 * void
 * glGetFramebufferParameteriv(GLenum target, GLenum pname, GLint *params);
 */

typedef struct
{
   GLenum target;
   GLenum pname;
   GLint *params;

} EVGL_API_Thread_Command_glGetFramebufferParameteriv;

void (*orig_evgl_api_glGetFramebufferParameteriv)(GLenum target, GLenum pname, GLint *params);

static void
_evgl_api_thread_glGetFramebufferParameteriv(void *data)
{
   EVGL_API_Thread_Command_glGetFramebufferParameteriv *thread_data =
      (EVGL_API_Thread_Command_glGetFramebufferParameteriv *)data;

   orig_evgl_api_glGetFramebufferParameteriv(thread_data->target,
                                             thread_data->pname,
                                             thread_data->params);

}

EAPI void
glGetFramebufferParameteriv_evgl_api_thread_cmd(GLenum target, GLenum pname, GLint *params)
{
   if (!evas_evgl_thread_enabled() || (_main_thread_id != eina_thread_self()))
     {
        orig_evgl_api_glGetFramebufferParameteriv(target, pname, params);
        return;
     }

   int thread_mode = EVAS_GL_THREAD_MODE_FINISH;

   EVGL_API_Thread_Command_glGetFramebufferParameteriv thread_data_local;
   EVGL_API_Thread_Command_glGetFramebufferParameteriv *thread_data = &thread_data_local;

   thread_data->target = target;
   thread_data->pname = pname;
   thread_data->params = params;

   evas_gl_thread_cmd_enqueue(EVAS_GL_THREAD_TYPE_EVGL,
                              _evgl_api_thread_glGetFramebufferParameteriv,
                              thread_data,
                              thread_mode);
}

/*
 * void
 * glGetProgramInterfaceiv(GLuint program, GLenum programInterface, GLenum pname, GLint *params);
 */

typedef struct
{
   GLuint program;
   GLenum programInterface;
   GLenum pname;
   GLint *params;

} EVGL_API_Thread_Command_glGetProgramInterfaceiv;

void (*orig_evgl_api_glGetProgramInterfaceiv)(GLuint program, GLenum programInterface, GLenum pname, GLint *params);

static void
_evgl_api_thread_glGetProgramInterfaceiv(void *data)
{
   EVGL_API_Thread_Command_glGetProgramInterfaceiv *thread_data =
      (EVGL_API_Thread_Command_glGetProgramInterfaceiv *)data;

   orig_evgl_api_glGetProgramInterfaceiv(thread_data->program,
                                         thread_data->programInterface,
                                         thread_data->pname,
                                         thread_data->params);

}

EAPI void
glGetProgramInterfaceiv_evgl_api_thread_cmd(GLuint program, GLenum programInterface, GLenum pname, GLint *params)
{
   if (!evas_evgl_thread_enabled() || (_main_thread_id != eina_thread_self()))
     {
        orig_evgl_api_glGetProgramInterfaceiv(program, programInterface, pname, params);
        return;
     }

   int thread_mode = EVAS_GL_THREAD_MODE_FINISH;

   EVGL_API_Thread_Command_glGetProgramInterfaceiv thread_data_local;
   EVGL_API_Thread_Command_glGetProgramInterfaceiv *thread_data = &thread_data_local;

   thread_data->program = program;
   thread_data->programInterface = programInterface;
   thread_data->pname = pname;
   thread_data->params = params;

   evas_gl_thread_cmd_enqueue(EVAS_GL_THREAD_TYPE_EVGL,
                              _evgl_api_thread_glGetProgramInterfaceiv,
                              thread_data,
                              thread_mode);
}

/*
 * GLuint
 * glGetProgramResourceIndex(GLuint program, GLenum programInterface, const GLchar *name);
 */

typedef struct
{
   GLuint return_value;
   GLuint program;
   GLenum programInterface;
   const GLchar *name;

} EVGL_API_Thread_Command_glGetProgramResourceIndex;

GLuint (*orig_evgl_api_glGetProgramResourceIndex)(GLuint program, GLenum programInterface, const GLchar *name);

static void
_evgl_api_thread_glGetProgramResourceIndex(void *data)
{
   EVGL_API_Thread_Command_glGetProgramResourceIndex *thread_data =
      (EVGL_API_Thread_Command_glGetProgramResourceIndex *)data;

   thread_data->return_value = orig_evgl_api_glGetProgramResourceIndex(thread_data->program,
                                                                       thread_data->programInterface,
                                                                       thread_data->name);

}

EAPI GLuint
glGetProgramResourceIndex_evgl_api_thread_cmd(GLuint program, GLenum programInterface, const GLchar *name)
{
   if (!evas_evgl_thread_enabled() || (_main_thread_id != eina_thread_self()))
     {
        return orig_evgl_api_glGetProgramResourceIndex(program, programInterface, name);
     }

   int thread_mode = EVAS_GL_THREAD_MODE_FINISH;

   EVGL_API_Thread_Command_glGetProgramResourceIndex thread_data_local;
   EVGL_API_Thread_Command_glGetProgramResourceIndex *thread_data = &thread_data_local;

   thread_data->program = program;
   thread_data->programInterface = programInterface;
   thread_data->name = name;

   evas_gl_thread_cmd_enqueue(EVAS_GL_THREAD_TYPE_EVGL,
                              _evgl_api_thread_glGetProgramResourceIndex,
                              thread_data,
                              thread_mode);

   return thread_data->return_value;
}

/*
 * void
 * glGetProgramResourceName(GLuint program, GLenum programInterface, GLuint index, GLsizei bufSize, GLsizei *length, GLchar *name);
 */

typedef struct
{
   GLuint program;
   GLenum programInterface;
   GLuint index;
   GLsizei bufSize;
   GLsizei *length;
   GLchar *name;

} EVGL_API_Thread_Command_glGetProgramResourceName;

void (*orig_evgl_api_glGetProgramResourceName)(GLuint program, GLenum programInterface, GLuint index, GLsizei bufSize, GLsizei *length, GLchar *name);

static void
_evgl_api_thread_glGetProgramResourceName(void *data)
{
   EVGL_API_Thread_Command_glGetProgramResourceName *thread_data =
      (EVGL_API_Thread_Command_glGetProgramResourceName *)data;

   orig_evgl_api_glGetProgramResourceName(thread_data->program,
                                          thread_data->programInterface,
                                          thread_data->index,
                                          thread_data->bufSize,
                                          thread_data->length,
                                          thread_data->name);

}

EAPI void
glGetProgramResourceName_evgl_api_thread_cmd(GLuint program, GLenum programInterface, GLuint index, GLsizei bufSize, GLsizei *length, GLchar *name)
{
   if (!evas_evgl_thread_enabled() || (_main_thread_id != eina_thread_self()))
     {
        orig_evgl_api_glGetProgramResourceName(program, programInterface, index, bufSize, length, name);
        return;
     }

   int thread_mode = EVAS_GL_THREAD_MODE_FINISH;

   EVGL_API_Thread_Command_glGetProgramResourceName thread_data_local;
   EVGL_API_Thread_Command_glGetProgramResourceName *thread_data = &thread_data_local;

   thread_data->program = program;
   thread_data->programInterface = programInterface;
   thread_data->index = index;
   thread_data->bufSize = bufSize;
   thread_data->length = length;
   thread_data->name = name;

   evas_gl_thread_cmd_enqueue(EVAS_GL_THREAD_TYPE_EVGL,
                              _evgl_api_thread_glGetProgramResourceName,
                              thread_data,
                              thread_mode);
}

/*
 * void
 * glGetProgramResourceiv(GLuint program, GLenum programInterface, GLuint index, GLsizei propCount, const GLenum *props, GLsizei bufSize, GLsizei *length, GLint *params);
 */

typedef struct
{
   GLuint program;
   GLenum programInterface;
   GLuint index;
   GLsizei propCount;
   const GLenum *props;
   GLsizei bufSize;
   GLsizei *length;
   GLint *params;

} EVGL_API_Thread_Command_glGetProgramResourceiv;

void (*orig_evgl_api_glGetProgramResourceiv)(GLuint program, GLenum programInterface, GLuint index, GLsizei propCount, const GLenum *props, GLsizei bufSize, GLsizei *length, GLint *params);

static void
_evgl_api_thread_glGetProgramResourceiv(void *data)
{
   EVGL_API_Thread_Command_glGetProgramResourceiv *thread_data =
      (EVGL_API_Thread_Command_glGetProgramResourceiv *)data;

   orig_evgl_api_glGetProgramResourceiv(thread_data->program,
                                        thread_data->programInterface,
                                        thread_data->index,
                                        thread_data->propCount,
                                        thread_data->props,
                                        thread_data->bufSize,
                                        thread_data->length,
                                        thread_data->params);

}

EAPI void
glGetProgramResourceiv_evgl_api_thread_cmd(GLuint program, GLenum programInterface, GLuint index, GLsizei propCount, const GLenum *props, GLsizei bufSize, GLsizei *length, GLint *params)
{
   if (!evas_evgl_thread_enabled() || (_main_thread_id != eina_thread_self()))
     {
        orig_evgl_api_glGetProgramResourceiv(program, programInterface, index, propCount, props, bufSize, length, params);
        return;
     }

   int thread_mode = EVAS_GL_THREAD_MODE_FINISH;

   EVGL_API_Thread_Command_glGetProgramResourceiv thread_data_local;
   EVGL_API_Thread_Command_glGetProgramResourceiv *thread_data = &thread_data_local;

   thread_data->program = program;
   thread_data->programInterface = programInterface;
   thread_data->index = index;
   thread_data->propCount = propCount;
   thread_data->props = props;
   thread_data->bufSize = bufSize;
   thread_data->length = length;
   thread_data->params = params;

   evas_gl_thread_cmd_enqueue(EVAS_GL_THREAD_TYPE_EVGL,
                              _evgl_api_thread_glGetProgramResourceiv,
                              thread_data,
                              thread_mode);
}

/*
 * GLint
 * glGetProgramResourceLocation(GLuint program, GLenum programInterface, const GLchar *name);
 */

typedef struct
{
   GLint return_value;
   GLuint program;
   GLenum programInterface;
   const GLchar *name;

} EVGL_API_Thread_Command_glGetProgramResourceLocation;

GLint (*orig_evgl_api_glGetProgramResourceLocation)(GLuint program, GLenum programInterface, const GLchar *name);

static void
_evgl_api_thread_glGetProgramResourceLocation(void *data)
{
   EVGL_API_Thread_Command_glGetProgramResourceLocation *thread_data =
      (EVGL_API_Thread_Command_glGetProgramResourceLocation *)data;

   thread_data->return_value = orig_evgl_api_glGetProgramResourceLocation(thread_data->program,
                                                                          thread_data->programInterface,
                                                                          thread_data->name);

}

EAPI GLint
glGetProgramResourceLocation_evgl_api_thread_cmd(GLuint program, GLenum programInterface, const GLchar *name)
{
   if (!evas_evgl_thread_enabled() || (_main_thread_id != eina_thread_self()))
     {
        return orig_evgl_api_glGetProgramResourceLocation(program, programInterface, name);
     }

   int thread_mode = EVAS_GL_THREAD_MODE_FINISH;

   EVGL_API_Thread_Command_glGetProgramResourceLocation thread_data_local;
   EVGL_API_Thread_Command_glGetProgramResourceLocation *thread_data = &thread_data_local;

   thread_data->program = program;
   thread_data->programInterface = programInterface;
   thread_data->name = name;

   evas_gl_thread_cmd_enqueue(EVAS_GL_THREAD_TYPE_EVGL,
                              _evgl_api_thread_glGetProgramResourceLocation,
                              thread_data,
                              thread_mode);

   return thread_data->return_value;
}

/*
 * void
 * glUseProgramStages(GLuint pipeline, GLbitfield stages, GLuint program);
 */

typedef struct
{
   GLuint pipeline;
   GLbitfield stages;
   GLuint program;

} EVGL_API_Thread_Command_glUseProgramStages;

void (*orig_evgl_api_glUseProgramStages)(GLuint pipeline, GLbitfield stages, GLuint program);

static void
_evgl_api_thread_glUseProgramStages(void *data)
{
   EVGL_API_Thread_Command_glUseProgramStages *thread_data =
      (EVGL_API_Thread_Command_glUseProgramStages *)data;

   orig_evgl_api_glUseProgramStages(thread_data->pipeline,
                                    thread_data->stages,
                                    thread_data->program);

}

EAPI void
glUseProgramStages_evgl_api_thread_cmd(GLuint pipeline, GLbitfield stages, GLuint program)
{
   if (!evas_evgl_thread_enabled() || (_main_thread_id != eina_thread_self()))
     {
        orig_evgl_api_glUseProgramStages(pipeline, stages, program);
        return;
     }

   int thread_mode = EVAS_GL_THREAD_MODE_FINISH;

   EVGL_API_Thread_Command_glUseProgramStages thread_data_local;
   EVGL_API_Thread_Command_glUseProgramStages *thread_data = &thread_data_local;

   thread_data->pipeline = pipeline;
   thread_data->stages = stages;
   thread_data->program = program;

   evas_gl_thread_cmd_enqueue(EVAS_GL_THREAD_TYPE_EVGL,
                              _evgl_api_thread_glUseProgramStages,
                              thread_data,
                              thread_mode);
}

/*
 * void
 * glActiveShaderProgram(GLuint pipeline, GLuint program);
 */

typedef struct
{
   GLuint pipeline;
   GLuint program;

} EVGL_API_Thread_Command_glActiveShaderProgram;

void (*orig_evgl_api_glActiveShaderProgram)(GLuint pipeline, GLuint program);

static void
_evgl_api_thread_glActiveShaderProgram(void *data)
{
   EVGL_API_Thread_Command_glActiveShaderProgram *thread_data =
      (EVGL_API_Thread_Command_glActiveShaderProgram *)data;

   orig_evgl_api_glActiveShaderProgram(thread_data->pipeline,
                                       thread_data->program);

}

EAPI void
glActiveShaderProgram_evgl_api_thread_cmd(GLuint pipeline, GLuint program)
{
   if (!evas_evgl_thread_enabled() || (_main_thread_id != eina_thread_self()))
     {
        orig_evgl_api_glActiveShaderProgram(pipeline, program);
        return;
     }

   int thread_mode = EVAS_GL_THREAD_MODE_FINISH;

   EVGL_API_Thread_Command_glActiveShaderProgram thread_data_local;
   EVGL_API_Thread_Command_glActiveShaderProgram *thread_data = &thread_data_local;

   thread_data->pipeline = pipeline;
   thread_data->program = program;

   evas_gl_thread_cmd_enqueue(EVAS_GL_THREAD_TYPE_EVGL,
                              _evgl_api_thread_glActiveShaderProgram,
                              thread_data,
                              thread_mode);
}

/*
 * GLuint
 * glCreateShaderProgramv(GLenum type, GLsizei count, const GLchar *const*strings);
 */

typedef struct
{
   GLuint return_value;
   GLenum type;
   GLsizei count;
   const GLchar *const*strings;

} EVGL_API_Thread_Command_glCreateShaderProgramv;

GLuint (*orig_evgl_api_glCreateShaderProgramv)(GLenum type, GLsizei count, const GLchar *const*strings);

static void
_evgl_api_thread_glCreateShaderProgramv(void *data)
{
   EVGL_API_Thread_Command_glCreateShaderProgramv *thread_data =
      (EVGL_API_Thread_Command_glCreateShaderProgramv *)data;

   thread_data->return_value = orig_evgl_api_glCreateShaderProgramv(thread_data->type,
                                                                    thread_data->count,
                                                                    thread_data->strings);

}

EAPI GLuint
glCreateShaderProgramv_evgl_api_thread_cmd(GLenum type, GLsizei count, const GLchar *const*strings)
{
   if (!evas_evgl_thread_enabled() || (_main_thread_id != eina_thread_self()))
     {
        return orig_evgl_api_glCreateShaderProgramv(type, count, strings);
     }

   int thread_mode = EVAS_GL_THREAD_MODE_FINISH;

   EVGL_API_Thread_Command_glCreateShaderProgramv thread_data_local;
   EVGL_API_Thread_Command_glCreateShaderProgramv *thread_data = &thread_data_local;

   thread_data->type = type;
   thread_data->count = count;
   thread_data->strings = strings;

   evas_gl_thread_cmd_enqueue(EVAS_GL_THREAD_TYPE_EVGL,
                              _evgl_api_thread_glCreateShaderProgramv,
                              thread_data,
                              thread_mode);

   return thread_data->return_value;
}

/*
 * void
 * glBindProgramPipeline(GLuint pipeline);
 */

typedef struct
{
   GLuint pipeline;

} EVGL_API_Thread_Command_glBindProgramPipeline;

void (*orig_evgl_api_glBindProgramPipeline)(GLuint pipeline);

static void
_evgl_api_thread_glBindProgramPipeline(void *data)
{
   EVGL_API_Thread_Command_glBindProgramPipeline *thread_data =
      (EVGL_API_Thread_Command_glBindProgramPipeline *)data;

   orig_evgl_api_glBindProgramPipeline(thread_data->pipeline);

}

EAPI void
glBindProgramPipeline_evgl_api_thread_cmd(GLuint pipeline)
{
   if (!evas_evgl_thread_enabled() || (_main_thread_id != eina_thread_self()))
     {
        orig_evgl_api_glBindProgramPipeline(pipeline);
        return;
     }

   int thread_mode = EVAS_GL_THREAD_MODE_FINISH;

   EVGL_API_Thread_Command_glBindProgramPipeline thread_data_local;
   EVGL_API_Thread_Command_glBindProgramPipeline *thread_data = &thread_data_local;

   thread_data->pipeline = pipeline;

   evas_gl_thread_cmd_enqueue(EVAS_GL_THREAD_TYPE_EVGL,
                              _evgl_api_thread_glBindProgramPipeline,
                              thread_data,
                              thread_mode);
}

/*
 * void
 * glDeleteProgramPipelines(GLsizei n, const GLuint *pipelines);
 */

typedef struct
{
   GLsizei n;
   const GLuint *pipelines;

} EVGL_API_Thread_Command_glDeleteProgramPipelines;

void (*orig_evgl_api_glDeleteProgramPipelines)(GLsizei n, const GLuint *pipelines);

static void
_evgl_api_thread_glDeleteProgramPipelines(void *data)
{
   EVGL_API_Thread_Command_glDeleteProgramPipelines *thread_data =
      (EVGL_API_Thread_Command_glDeleteProgramPipelines *)data;

   orig_evgl_api_glDeleteProgramPipelines(thread_data->n,
                                          thread_data->pipelines);

}

EAPI void
glDeleteProgramPipelines_evgl_api_thread_cmd(GLsizei n, const GLuint *pipelines)
{
   if (!evas_evgl_thread_enabled() || (_main_thread_id != eina_thread_self()))
     {
        orig_evgl_api_glDeleteProgramPipelines(n, pipelines);
        return;
     }

   int thread_mode = EVAS_GL_THREAD_MODE_FINISH;

   EVGL_API_Thread_Command_glDeleteProgramPipelines thread_data_local;
   EVGL_API_Thread_Command_glDeleteProgramPipelines *thread_data = &thread_data_local;

   thread_data->n = n;
   thread_data->pipelines = pipelines;

   evas_gl_thread_cmd_enqueue(EVAS_GL_THREAD_TYPE_EVGL,
                              _evgl_api_thread_glDeleteProgramPipelines,
                              thread_data,
                              thread_mode);
}

/*
 * void
 * glGenProgramPipelines(GLsizei n, GLuint *pipelines);
 */

typedef struct
{
   GLsizei n;
   GLuint *pipelines;

} EVGL_API_Thread_Command_glGenProgramPipelines;

void (*orig_evgl_api_glGenProgramPipelines)(GLsizei n, GLuint *pipelines);

static void
_evgl_api_thread_glGenProgramPipelines(void *data)
{
   EVGL_API_Thread_Command_glGenProgramPipelines *thread_data =
      (EVGL_API_Thread_Command_glGenProgramPipelines *)data;

   orig_evgl_api_glGenProgramPipelines(thread_data->n,
                                       thread_data->pipelines);

}

EAPI void
glGenProgramPipelines_evgl_api_thread_cmd(GLsizei n, GLuint *pipelines)
{
   if (!evas_evgl_thread_enabled() || (_main_thread_id != eina_thread_self()))
     {
        orig_evgl_api_glGenProgramPipelines(n, pipelines);
        return;
     }

   int thread_mode = EVAS_GL_THREAD_MODE_FINISH;

   EVGL_API_Thread_Command_glGenProgramPipelines thread_data_local;
   EVGL_API_Thread_Command_glGenProgramPipelines *thread_data = &thread_data_local;

   thread_data->n = n;
   thread_data->pipelines = pipelines;

   evas_gl_thread_cmd_enqueue(EVAS_GL_THREAD_TYPE_EVGL,
                              _evgl_api_thread_glGenProgramPipelines,
                              thread_data,
                              thread_mode);
}

/*
 * GLboolean
 * glIsProgramPipeline(GLuint pipeline);
 */

typedef struct
{
   GLboolean return_value;
   GLuint pipeline;

} EVGL_API_Thread_Command_glIsProgramPipeline;

GLboolean (*orig_evgl_api_glIsProgramPipeline)(GLuint pipeline);

static void
_evgl_api_thread_glIsProgramPipeline(void *data)
{
   EVGL_API_Thread_Command_glIsProgramPipeline *thread_data =
      (EVGL_API_Thread_Command_glIsProgramPipeline *)data;

   thread_data->return_value = orig_evgl_api_glIsProgramPipeline(thread_data->pipeline);

}

EAPI GLboolean
glIsProgramPipeline_evgl_api_thread_cmd(GLuint pipeline)
{
   if (!evas_evgl_thread_enabled() || (_main_thread_id != eina_thread_self()))
     {
        return orig_evgl_api_glIsProgramPipeline(pipeline);
     }

   int thread_mode = EVAS_GL_THREAD_MODE_FINISH;

   EVGL_API_Thread_Command_glIsProgramPipeline thread_data_local;
   EVGL_API_Thread_Command_glIsProgramPipeline *thread_data = &thread_data_local;

   thread_data->pipeline = pipeline;

   evas_gl_thread_cmd_enqueue(EVAS_GL_THREAD_TYPE_EVGL,
                              _evgl_api_thread_glIsProgramPipeline,
                              thread_data,
                              thread_mode);

   return thread_data->return_value;
}

/*
 * void
 * glGetProgramPipelineiv(GLuint pipeline, GLenum pname, GLint *params);
 */

typedef struct
{
   GLuint pipeline;
   GLenum pname;
   GLint *params;

} EVGL_API_Thread_Command_glGetProgramPipelineiv;

void (*orig_evgl_api_glGetProgramPipelineiv)(GLuint pipeline, GLenum pname, GLint *params);

static void
_evgl_api_thread_glGetProgramPipelineiv(void *data)
{
   EVGL_API_Thread_Command_glGetProgramPipelineiv *thread_data =
      (EVGL_API_Thread_Command_glGetProgramPipelineiv *)data;

   orig_evgl_api_glGetProgramPipelineiv(thread_data->pipeline,
                                        thread_data->pname,
                                        thread_data->params);

}

EAPI void
glGetProgramPipelineiv_evgl_api_thread_cmd(GLuint pipeline, GLenum pname, GLint *params)
{
   if (!evas_evgl_thread_enabled() || (_main_thread_id != eina_thread_self()))
     {
        orig_evgl_api_glGetProgramPipelineiv(pipeline, pname, params);
        return;
     }

   int thread_mode = EVAS_GL_THREAD_MODE_FINISH;

   EVGL_API_Thread_Command_glGetProgramPipelineiv thread_data_local;
   EVGL_API_Thread_Command_glGetProgramPipelineiv *thread_data = &thread_data_local;

   thread_data->pipeline = pipeline;
   thread_data->pname = pname;
   thread_data->params = params;

   evas_gl_thread_cmd_enqueue(EVAS_GL_THREAD_TYPE_EVGL,
                              _evgl_api_thread_glGetProgramPipelineiv,
                              thread_data,
                              thread_mode);
}

/*
 * void
 * glProgramUniform1i(GLuint program, GLint location, GLint v0);
 */

typedef struct
{
   GLuint program;
   GLint location;
   GLint v0;

} EVGL_API_Thread_Command_glProgramUniform1i;

void (*orig_evgl_api_glProgramUniform1i)(GLuint program, GLint location, GLint v0);

static void
_evgl_api_thread_glProgramUniform1i(void *data)
{
   EVGL_API_Thread_Command_glProgramUniform1i *thread_data =
      (EVGL_API_Thread_Command_glProgramUniform1i *)data;

   orig_evgl_api_glProgramUniform1i(thread_data->program,
                                    thread_data->location,
                                    thread_data->v0);

}

EAPI void
glProgramUniform1i_evgl_api_thread_cmd(GLuint program, GLint location, GLint v0)
{
   if (!evas_evgl_thread_enabled() || (_main_thread_id != eina_thread_self()))
     {
        orig_evgl_api_glProgramUniform1i(program, location, v0);
        return;
     }

   int thread_mode = EVAS_GL_THREAD_MODE_FINISH;

   EVGL_API_Thread_Command_glProgramUniform1i thread_data_local;
   EVGL_API_Thread_Command_glProgramUniform1i *thread_data = &thread_data_local;

   thread_data->program = program;
   thread_data->location = location;
   thread_data->v0 = v0;

   evas_gl_thread_cmd_enqueue(EVAS_GL_THREAD_TYPE_EVGL,
                              _evgl_api_thread_glProgramUniform1i,
                              thread_data,
                              thread_mode);
}

/*
 * void
 * glProgramUniform2i(GLuint program, GLint location, GLint v0, GLint v1);
 */

typedef struct
{
   GLuint program;
   GLint location;
   GLint v0;
   GLint v1;

} EVGL_API_Thread_Command_glProgramUniform2i;

void (*orig_evgl_api_glProgramUniform2i)(GLuint program, GLint location, GLint v0, GLint v1);

static void
_evgl_api_thread_glProgramUniform2i(void *data)
{
   EVGL_API_Thread_Command_glProgramUniform2i *thread_data =
      (EVGL_API_Thread_Command_glProgramUniform2i *)data;

   orig_evgl_api_glProgramUniform2i(thread_data->program,
                                    thread_data->location,
                                    thread_data->v0,
                                    thread_data->v1);

}

EAPI void
glProgramUniform2i_evgl_api_thread_cmd(GLuint program, GLint location, GLint v0, GLint v1)
{
   if (!evas_evgl_thread_enabled() || (_main_thread_id != eina_thread_self()))
     {
        orig_evgl_api_glProgramUniform2i(program, location, v0, v1);
        return;
     }

   int thread_mode = EVAS_GL_THREAD_MODE_FINISH;

   EVGL_API_Thread_Command_glProgramUniform2i thread_data_local;
   EVGL_API_Thread_Command_glProgramUniform2i *thread_data = &thread_data_local;

   thread_data->program = program;
   thread_data->location = location;
   thread_data->v0 = v0;
   thread_data->v1 = v1;

   evas_gl_thread_cmd_enqueue(EVAS_GL_THREAD_TYPE_EVGL,
                              _evgl_api_thread_glProgramUniform2i,
                              thread_data,
                              thread_mode);
}

/*
 * void
 * glProgramUniform3i(GLuint program, GLint location, GLint v0, GLint v1, GLint v2);
 */

typedef struct
{
   GLuint program;
   GLint location;
   GLint v0;
   GLint v1;
   GLint v2;

} EVGL_API_Thread_Command_glProgramUniform3i;

void (*orig_evgl_api_glProgramUniform3i)(GLuint program, GLint location, GLint v0, GLint v1, GLint v2);

static void
_evgl_api_thread_glProgramUniform3i(void *data)
{
   EVGL_API_Thread_Command_glProgramUniform3i *thread_data =
      (EVGL_API_Thread_Command_glProgramUniform3i *)data;

   orig_evgl_api_glProgramUniform3i(thread_data->program,
                                    thread_data->location,
                                    thread_data->v0,
                                    thread_data->v1,
                                    thread_data->v2);

}

EAPI void
glProgramUniform3i_evgl_api_thread_cmd(GLuint program, GLint location, GLint v0, GLint v1, GLint v2)
{
   if (!evas_evgl_thread_enabled() || (_main_thread_id != eina_thread_self()))
     {
        orig_evgl_api_glProgramUniform3i(program, location, v0, v1, v2);
        return;
     }

   int thread_mode = EVAS_GL_THREAD_MODE_FINISH;

   EVGL_API_Thread_Command_glProgramUniform3i thread_data_local;
   EVGL_API_Thread_Command_glProgramUniform3i *thread_data = &thread_data_local;

   thread_data->program = program;
   thread_data->location = location;
   thread_data->v0 = v0;
   thread_data->v1 = v1;
   thread_data->v2 = v2;

   evas_gl_thread_cmd_enqueue(EVAS_GL_THREAD_TYPE_EVGL,
                              _evgl_api_thread_glProgramUniform3i,
                              thread_data,
                              thread_mode);
}

/*
 * void
 * glProgramUniform4i(GLuint program, GLint location, GLint v0, GLint v1, GLint v2, GLint v3);
 */

typedef struct
{
   GLuint program;
   GLint location;
   GLint v0;
   GLint v1;
   GLint v2;
   GLint v3;

} EVGL_API_Thread_Command_glProgramUniform4i;

void (*orig_evgl_api_glProgramUniform4i)(GLuint program, GLint location, GLint v0, GLint v1, GLint v2, GLint v3);

static void
_evgl_api_thread_glProgramUniform4i(void *data)
{
   EVGL_API_Thread_Command_glProgramUniform4i *thread_data =
      (EVGL_API_Thread_Command_glProgramUniform4i *)data;

   orig_evgl_api_glProgramUniform4i(thread_data->program,
                                    thread_data->location,
                                    thread_data->v0,
                                    thread_data->v1,
                                    thread_data->v2,
                                    thread_data->v3);

}

EAPI void
glProgramUniform4i_evgl_api_thread_cmd(GLuint program, GLint location, GLint v0, GLint v1, GLint v2, GLint v3)
{
   if (!evas_evgl_thread_enabled() || (_main_thread_id != eina_thread_self()))
     {
        orig_evgl_api_glProgramUniform4i(program, location, v0, v1, v2, v3);
        return;
     }

   int thread_mode = EVAS_GL_THREAD_MODE_FINISH;

   EVGL_API_Thread_Command_glProgramUniform4i thread_data_local;
   EVGL_API_Thread_Command_glProgramUniform4i *thread_data = &thread_data_local;

   thread_data->program = program;
   thread_data->location = location;
   thread_data->v0 = v0;
   thread_data->v1 = v1;
   thread_data->v2 = v2;
   thread_data->v3 = v3;

   evas_gl_thread_cmd_enqueue(EVAS_GL_THREAD_TYPE_EVGL,
                              _evgl_api_thread_glProgramUniform4i,
                              thread_data,
                              thread_mode);
}

/*
 * void
 * glProgramUniform1ui(GLuint program, GLint location, GLuint v0);
 */

typedef struct
{
   GLuint program;
   GLint location;
   GLuint v0;

} EVGL_API_Thread_Command_glProgramUniform1ui;

void (*orig_evgl_api_glProgramUniform1ui)(GLuint program, GLint location, GLuint v0);

static void
_evgl_api_thread_glProgramUniform1ui(void *data)
{
   EVGL_API_Thread_Command_glProgramUniform1ui *thread_data =
      (EVGL_API_Thread_Command_glProgramUniform1ui *)data;

   orig_evgl_api_glProgramUniform1ui(thread_data->program,
                                     thread_data->location,
                                     thread_data->v0);

}

EAPI void
glProgramUniform1ui_evgl_api_thread_cmd(GLuint program, GLint location, GLuint v0)
{
   if (!evas_evgl_thread_enabled() || (_main_thread_id != eina_thread_self()))
     {
        orig_evgl_api_glProgramUniform1ui(program, location, v0);
        return;
     }

   int thread_mode = EVAS_GL_THREAD_MODE_FINISH;

   EVGL_API_Thread_Command_glProgramUniform1ui thread_data_local;
   EVGL_API_Thread_Command_glProgramUniform1ui *thread_data = &thread_data_local;

   thread_data->program = program;
   thread_data->location = location;
   thread_data->v0 = v0;

   evas_gl_thread_cmd_enqueue(EVAS_GL_THREAD_TYPE_EVGL,
                              _evgl_api_thread_glProgramUniform1ui,
                              thread_data,
                              thread_mode);
}

/*
 * void
 * glProgramUniform2ui(GLuint program, GLint location, GLuint v0, GLuint v1);
 */

typedef struct
{
   GLuint program;
   GLint location;
   GLuint v0;
   GLuint v1;

} EVGL_API_Thread_Command_glProgramUniform2ui;

void (*orig_evgl_api_glProgramUniform2ui)(GLuint program, GLint location, GLuint v0, GLuint v1);

static void
_evgl_api_thread_glProgramUniform2ui(void *data)
{
   EVGL_API_Thread_Command_glProgramUniform2ui *thread_data =
      (EVGL_API_Thread_Command_glProgramUniform2ui *)data;

   orig_evgl_api_glProgramUniform2ui(thread_data->program,
                                     thread_data->location,
                                     thread_data->v0,
                                     thread_data->v1);

}

EAPI void
glProgramUniform2ui_evgl_api_thread_cmd(GLuint program, GLint location, GLuint v0, GLuint v1)
{
   if (!evas_evgl_thread_enabled() || (_main_thread_id != eina_thread_self()))
     {
        orig_evgl_api_glProgramUniform2ui(program, location, v0, v1);
        return;
     }

   int thread_mode = EVAS_GL_THREAD_MODE_FINISH;

   EVGL_API_Thread_Command_glProgramUniform2ui thread_data_local;
   EVGL_API_Thread_Command_glProgramUniform2ui *thread_data = &thread_data_local;

   thread_data->program = program;
   thread_data->location = location;
   thread_data->v0 = v0;
   thread_data->v1 = v1;

   evas_gl_thread_cmd_enqueue(EVAS_GL_THREAD_TYPE_EVGL,
                              _evgl_api_thread_glProgramUniform2ui,
                              thread_data,
                              thread_mode);
}

/*
 * void
 * glProgramUniform3ui(GLuint program, GLint location, GLuint v0, GLuint v1, GLuint v2);
 */

typedef struct
{
   GLuint program;
   GLint location;
   GLuint v0;
   GLuint v1;
   GLuint v2;

} EVGL_API_Thread_Command_glProgramUniform3ui;

void (*orig_evgl_api_glProgramUniform3ui)(GLuint program, GLint location, GLuint v0, GLuint v1, GLuint v2);

static void
_evgl_api_thread_glProgramUniform3ui(void *data)
{
   EVGL_API_Thread_Command_glProgramUniform3ui *thread_data =
      (EVGL_API_Thread_Command_glProgramUniform3ui *)data;

   orig_evgl_api_glProgramUniform3ui(thread_data->program,
                                     thread_data->location,
                                     thread_data->v0,
                                     thread_data->v1,
                                     thread_data->v2);

}

EAPI void
glProgramUniform3ui_evgl_api_thread_cmd(GLuint program, GLint location, GLuint v0, GLuint v1, GLuint v2)
{
   if (!evas_evgl_thread_enabled() || (_main_thread_id != eina_thread_self()))
     {
        orig_evgl_api_glProgramUniform3ui(program, location, v0, v1, v2);
        return;
     }

   int thread_mode = EVAS_GL_THREAD_MODE_FINISH;

   EVGL_API_Thread_Command_glProgramUniform3ui thread_data_local;
   EVGL_API_Thread_Command_glProgramUniform3ui *thread_data = &thread_data_local;

   thread_data->program = program;
   thread_data->location = location;
   thread_data->v0 = v0;
   thread_data->v1 = v1;
   thread_data->v2 = v2;

   evas_gl_thread_cmd_enqueue(EVAS_GL_THREAD_TYPE_EVGL,
                              _evgl_api_thread_glProgramUniform3ui,
                              thread_data,
                              thread_mode);
}

/*
 * void
 * glProgramUniform4ui(GLuint program, GLint location, GLuint v0, GLuint v1, GLuint v2, GLuint v3);
 */

typedef struct
{
   GLuint program;
   GLint location;
   GLuint v0;
   GLuint v1;
   GLuint v2;
   GLuint v3;

} EVGL_API_Thread_Command_glProgramUniform4ui;

void (*orig_evgl_api_glProgramUniform4ui)(GLuint program, GLint location, GLuint v0, GLuint v1, GLuint v2, GLuint v3);

static void
_evgl_api_thread_glProgramUniform4ui(void *data)
{
   EVGL_API_Thread_Command_glProgramUniform4ui *thread_data =
      (EVGL_API_Thread_Command_glProgramUniform4ui *)data;

   orig_evgl_api_glProgramUniform4ui(thread_data->program,
                                     thread_data->location,
                                     thread_data->v0,
                                     thread_data->v1,
                                     thread_data->v2,
                                     thread_data->v3);

}

EAPI void
glProgramUniform4ui_evgl_api_thread_cmd(GLuint program, GLint location, GLuint v0, GLuint v1, GLuint v2, GLuint v3)
{
   if (!evas_evgl_thread_enabled() || (_main_thread_id != eina_thread_self()))
     {
        orig_evgl_api_glProgramUniform4ui(program, location, v0, v1, v2, v3);
        return;
     }

   int thread_mode = EVAS_GL_THREAD_MODE_FINISH;

   EVGL_API_Thread_Command_glProgramUniform4ui thread_data_local;
   EVGL_API_Thread_Command_glProgramUniform4ui *thread_data = &thread_data_local;

   thread_data->program = program;
   thread_data->location = location;
   thread_data->v0 = v0;
   thread_data->v1 = v1;
   thread_data->v2 = v2;
   thread_data->v3 = v3;

   evas_gl_thread_cmd_enqueue(EVAS_GL_THREAD_TYPE_EVGL,
                              _evgl_api_thread_glProgramUniform4ui,
                              thread_data,
                              thread_mode);
}

/*
 * void
 * glProgramUniform1f(GLuint program, GLint location, GLfloat v0);
 */

typedef struct
{
   GLuint program;
   GLint location;
   GLfloat v0;

} EVGL_API_Thread_Command_glProgramUniform1f;

void (*orig_evgl_api_glProgramUniform1f)(GLuint program, GLint location, GLfloat v0);

static void
_evgl_api_thread_glProgramUniform1f(void *data)
{
   EVGL_API_Thread_Command_glProgramUniform1f *thread_data =
      (EVGL_API_Thread_Command_glProgramUniform1f *)data;

   orig_evgl_api_glProgramUniform1f(thread_data->program,
                                    thread_data->location,
                                    thread_data->v0);

}

EAPI void
glProgramUniform1f_evgl_api_thread_cmd(GLuint program, GLint location, GLfloat v0)
{
   if (!evas_evgl_thread_enabled() || (_main_thread_id != eina_thread_self()))
     {
        orig_evgl_api_glProgramUniform1f(program, location, v0);
        return;
     }

   int thread_mode = EVAS_GL_THREAD_MODE_FINISH;

   EVGL_API_Thread_Command_glProgramUniform1f thread_data_local;
   EVGL_API_Thread_Command_glProgramUniform1f *thread_data = &thread_data_local;

   thread_data->program = program;
   thread_data->location = location;
   thread_data->v0 = v0;

   evas_gl_thread_cmd_enqueue(EVAS_GL_THREAD_TYPE_EVGL,
                              _evgl_api_thread_glProgramUniform1f,
                              thread_data,
                              thread_mode);
}

/*
 * void
 * glProgramUniform2f(GLuint program, GLint location, GLfloat v0, GLfloat v1);
 */

typedef struct
{
   GLuint program;
   GLint location;
   GLfloat v0;
   GLfloat v1;

} EVGL_API_Thread_Command_glProgramUniform2f;

void (*orig_evgl_api_glProgramUniform2f)(GLuint program, GLint location, GLfloat v0, GLfloat v1);

static void
_evgl_api_thread_glProgramUniform2f(void *data)
{
   EVGL_API_Thread_Command_glProgramUniform2f *thread_data =
      (EVGL_API_Thread_Command_glProgramUniform2f *)data;

   orig_evgl_api_glProgramUniform2f(thread_data->program,
                                    thread_data->location,
                                    thread_data->v0,
                                    thread_data->v1);

}

EAPI void
glProgramUniform2f_evgl_api_thread_cmd(GLuint program, GLint location, GLfloat v0, GLfloat v1)
{
   if (!evas_evgl_thread_enabled() || (_main_thread_id != eina_thread_self()))
     {
        orig_evgl_api_glProgramUniform2f(program, location, v0, v1);
        return;
     }

   int thread_mode = EVAS_GL_THREAD_MODE_FINISH;

   EVGL_API_Thread_Command_glProgramUniform2f thread_data_local;
   EVGL_API_Thread_Command_glProgramUniform2f *thread_data = &thread_data_local;

   thread_data->program = program;
   thread_data->location = location;
   thread_data->v0 = v0;
   thread_data->v1 = v1;

   evas_gl_thread_cmd_enqueue(EVAS_GL_THREAD_TYPE_EVGL,
                              _evgl_api_thread_glProgramUniform2f,
                              thread_data,
                              thread_mode);
}

/*
 * void
 * glProgramUniform3f(GLuint program, GLint location, GLfloat v0, GLfloat v1, GLfloat v2);
 */

typedef struct
{
   GLuint program;
   GLint location;
   GLfloat v0;
   GLfloat v1;
   GLfloat v2;

} EVGL_API_Thread_Command_glProgramUniform3f;

void (*orig_evgl_api_glProgramUniform3f)(GLuint program, GLint location, GLfloat v0, GLfloat v1, GLfloat v2);

static void
_evgl_api_thread_glProgramUniform3f(void *data)
{
   EVGL_API_Thread_Command_glProgramUniform3f *thread_data =
      (EVGL_API_Thread_Command_glProgramUniform3f *)data;

   orig_evgl_api_glProgramUniform3f(thread_data->program,
                                    thread_data->location,
                                    thread_data->v0,
                                    thread_data->v1,
                                    thread_data->v2);

}

EAPI void
glProgramUniform3f_evgl_api_thread_cmd(GLuint program, GLint location, GLfloat v0, GLfloat v1, GLfloat v2)
{
   if (!evas_evgl_thread_enabled() || (_main_thread_id != eina_thread_self()))
     {
        orig_evgl_api_glProgramUniform3f(program, location, v0, v1, v2);
        return;
     }

   int thread_mode = EVAS_GL_THREAD_MODE_FINISH;

   EVGL_API_Thread_Command_glProgramUniform3f thread_data_local;
   EVGL_API_Thread_Command_glProgramUniform3f *thread_data = &thread_data_local;

   thread_data->program = program;
   thread_data->location = location;
   thread_data->v0 = v0;
   thread_data->v1 = v1;
   thread_data->v2 = v2;

   evas_gl_thread_cmd_enqueue(EVAS_GL_THREAD_TYPE_EVGL,
                              _evgl_api_thread_glProgramUniform3f,
                              thread_data,
                              thread_mode);
}

/*
 * void
 * glProgramUniform4f(GLuint program, GLint location, GLfloat v0, GLfloat v1, GLfloat v2, GLfloat v3);
 */

typedef struct
{
   GLuint program;
   GLint location;
   GLfloat v0;
   GLfloat v1;
   GLfloat v2;
   GLfloat v3;

} EVGL_API_Thread_Command_glProgramUniform4f;

void (*orig_evgl_api_glProgramUniform4f)(GLuint program, GLint location, GLfloat v0, GLfloat v1, GLfloat v2, GLfloat v3);

static void
_evgl_api_thread_glProgramUniform4f(void *data)
{
   EVGL_API_Thread_Command_glProgramUniform4f *thread_data =
      (EVGL_API_Thread_Command_glProgramUniform4f *)data;

   orig_evgl_api_glProgramUniform4f(thread_data->program,
                                    thread_data->location,
                                    thread_data->v0,
                                    thread_data->v1,
                                    thread_data->v2,
                                    thread_data->v3);

}

EAPI void
glProgramUniform4f_evgl_api_thread_cmd(GLuint program, GLint location, GLfloat v0, GLfloat v1, GLfloat v2, GLfloat v3)
{
   if (!evas_evgl_thread_enabled() || (_main_thread_id != eina_thread_self()))
     {
        orig_evgl_api_glProgramUniform4f(program, location, v0, v1, v2, v3);
        return;
     }

   int thread_mode = EVAS_GL_THREAD_MODE_FINISH;

   EVGL_API_Thread_Command_glProgramUniform4f thread_data_local;
   EVGL_API_Thread_Command_glProgramUniform4f *thread_data = &thread_data_local;

   thread_data->program = program;
   thread_data->location = location;
   thread_data->v0 = v0;
   thread_data->v1 = v1;
   thread_data->v2 = v2;
   thread_data->v3 = v3;

   evas_gl_thread_cmd_enqueue(EVAS_GL_THREAD_TYPE_EVGL,
                              _evgl_api_thread_glProgramUniform4f,
                              thread_data,
                              thread_mode);
}

/*
 * void
 * glProgramUniform1iv(GLuint program, GLint location, GLsizei count, const GLint *value);
 */

typedef struct
{
   GLuint program;
   GLint location;
   GLsizei count;
   const GLint *value;

} EVGL_API_Thread_Command_glProgramUniform1iv;

void (*orig_evgl_api_glProgramUniform1iv)(GLuint program, GLint location, GLsizei count, const GLint *value);

static void
_evgl_api_thread_glProgramUniform1iv(void *data)
{
   EVGL_API_Thread_Command_glProgramUniform1iv *thread_data =
      (EVGL_API_Thread_Command_glProgramUniform1iv *)data;

   orig_evgl_api_glProgramUniform1iv(thread_data->program,
                                     thread_data->location,
                                     thread_data->count,
                                     thread_data->value);

}

EAPI void
glProgramUniform1iv_evgl_api_thread_cmd(GLuint program, GLint location, GLsizei count, const GLint *value)
{
   if (!evas_evgl_thread_enabled() || (_main_thread_id != eina_thread_self()))
     {
        orig_evgl_api_glProgramUniform1iv(program, location, count, value);
        return;
     }

   int thread_mode = EVAS_GL_THREAD_MODE_FINISH;

   EVGL_API_Thread_Command_glProgramUniform1iv thread_data_local;
   EVGL_API_Thread_Command_glProgramUniform1iv *thread_data = &thread_data_local;

   thread_data->program = program;
   thread_data->location = location;
   thread_data->count = count;
   thread_data->value = value;

   evas_gl_thread_cmd_enqueue(EVAS_GL_THREAD_TYPE_EVGL,
                              _evgl_api_thread_glProgramUniform1iv,
                              thread_data,
                              thread_mode);
}

/*
 * void
 * glProgramUniform2iv(GLuint program, GLint location, GLsizei count, const GLint *value);
 */

typedef struct
{
   GLuint program;
   GLint location;
   GLsizei count;
   const GLint *value;

} EVGL_API_Thread_Command_glProgramUniform2iv;

void (*orig_evgl_api_glProgramUniform2iv)(GLuint program, GLint location, GLsizei count, const GLint *value);

static void
_evgl_api_thread_glProgramUniform2iv(void *data)
{
   EVGL_API_Thread_Command_glProgramUniform2iv *thread_data =
      (EVGL_API_Thread_Command_glProgramUniform2iv *)data;

   orig_evgl_api_glProgramUniform2iv(thread_data->program,
                                     thread_data->location,
                                     thread_data->count,
                                     thread_data->value);

}

EAPI void
glProgramUniform2iv_evgl_api_thread_cmd(GLuint program, GLint location, GLsizei count, const GLint *value)
{
   if (!evas_evgl_thread_enabled() || (_main_thread_id != eina_thread_self()))
     {
        orig_evgl_api_glProgramUniform2iv(program, location, count, value);
        return;
     }

   int thread_mode = EVAS_GL_THREAD_MODE_FINISH;

   EVGL_API_Thread_Command_glProgramUniform2iv thread_data_local;
   EVGL_API_Thread_Command_glProgramUniform2iv *thread_data = &thread_data_local;

   thread_data->program = program;
   thread_data->location = location;
   thread_data->count = count;
   thread_data->value = value;

   evas_gl_thread_cmd_enqueue(EVAS_GL_THREAD_TYPE_EVGL,
                              _evgl_api_thread_glProgramUniform2iv,
                              thread_data,
                              thread_mode);
}

/*
 * void
 * glProgramUniform3iv(GLuint program, GLint location, GLsizei count, const GLint *value);
 */

typedef struct
{
   GLuint program;
   GLint location;
   GLsizei count;
   const GLint *value;

} EVGL_API_Thread_Command_glProgramUniform3iv;

void (*orig_evgl_api_glProgramUniform3iv)(GLuint program, GLint location, GLsizei count, const GLint *value);

static void
_evgl_api_thread_glProgramUniform3iv(void *data)
{
   EVGL_API_Thread_Command_glProgramUniform3iv *thread_data =
      (EVGL_API_Thread_Command_glProgramUniform3iv *)data;

   orig_evgl_api_glProgramUniform3iv(thread_data->program,
                                     thread_data->location,
                                     thread_data->count,
                                     thread_data->value);

}

EAPI void
glProgramUniform3iv_evgl_api_thread_cmd(GLuint program, GLint location, GLsizei count, const GLint *value)
{
   if (!evas_evgl_thread_enabled() || (_main_thread_id != eina_thread_self()))
     {
        orig_evgl_api_glProgramUniform3iv(program, location, count, value);
        return;
     }

   int thread_mode = EVAS_GL_THREAD_MODE_FINISH;

   EVGL_API_Thread_Command_glProgramUniform3iv thread_data_local;
   EVGL_API_Thread_Command_glProgramUniform3iv *thread_data = &thread_data_local;

   thread_data->program = program;
   thread_data->location = location;
   thread_data->count = count;
   thread_data->value = value;

   evas_gl_thread_cmd_enqueue(EVAS_GL_THREAD_TYPE_EVGL,
                              _evgl_api_thread_glProgramUniform3iv,
                              thread_data,
                              thread_mode);
}

/*
 * void
 * glProgramUniform4iv(GLuint program, GLint location, GLsizei count, const GLint *value);
 */

typedef struct
{
   GLuint program;
   GLint location;
   GLsizei count;
   const GLint *value;

} EVGL_API_Thread_Command_glProgramUniform4iv;

void (*orig_evgl_api_glProgramUniform4iv)(GLuint program, GLint location, GLsizei count, const GLint *value);

static void
_evgl_api_thread_glProgramUniform4iv(void *data)
{
   EVGL_API_Thread_Command_glProgramUniform4iv *thread_data =
      (EVGL_API_Thread_Command_glProgramUniform4iv *)data;

   orig_evgl_api_glProgramUniform4iv(thread_data->program,
                                     thread_data->location,
                                     thread_data->count,
                                     thread_data->value);

}

EAPI void
glProgramUniform4iv_evgl_api_thread_cmd(GLuint program, GLint location, GLsizei count, const GLint *value)
{
   if (!evas_evgl_thread_enabled() || (_main_thread_id != eina_thread_self()))
     {
        orig_evgl_api_glProgramUniform4iv(program, location, count, value);
        return;
     }

   int thread_mode = EVAS_GL_THREAD_MODE_FINISH;

   EVGL_API_Thread_Command_glProgramUniform4iv thread_data_local;
   EVGL_API_Thread_Command_glProgramUniform4iv *thread_data = &thread_data_local;

   thread_data->program = program;
   thread_data->location = location;
   thread_data->count = count;
   thread_data->value = value;

   evas_gl_thread_cmd_enqueue(EVAS_GL_THREAD_TYPE_EVGL,
                              _evgl_api_thread_glProgramUniform4iv,
                              thread_data,
                              thread_mode);
}

/*
 * void
 * glProgramUniform1uiv(GLuint program, GLint location, GLsizei count, const GLuint *value);
 */

typedef struct
{
   GLuint program;
   GLint location;
   GLsizei count;
   const GLuint *value;

} EVGL_API_Thread_Command_glProgramUniform1uiv;

void (*orig_evgl_api_glProgramUniform1uiv)(GLuint program, GLint location, GLsizei count, const GLuint *value);

static void
_evgl_api_thread_glProgramUniform1uiv(void *data)
{
   EVGL_API_Thread_Command_glProgramUniform1uiv *thread_data =
      (EVGL_API_Thread_Command_glProgramUniform1uiv *)data;

   orig_evgl_api_glProgramUniform1uiv(thread_data->program,
                                      thread_data->location,
                                      thread_data->count,
                                      thread_data->value);

}

EAPI void
glProgramUniform1uiv_evgl_api_thread_cmd(GLuint program, GLint location, GLsizei count, const GLuint *value)
{
   if (!evas_evgl_thread_enabled() || (_main_thread_id != eina_thread_self()))
     {
        orig_evgl_api_glProgramUniform1uiv(program, location, count, value);
        return;
     }

   int thread_mode = EVAS_GL_THREAD_MODE_FINISH;

   EVGL_API_Thread_Command_glProgramUniform1uiv thread_data_local;
   EVGL_API_Thread_Command_glProgramUniform1uiv *thread_data = &thread_data_local;

   thread_data->program = program;
   thread_data->location = location;
   thread_data->count = count;
   thread_data->value = value;

   evas_gl_thread_cmd_enqueue(EVAS_GL_THREAD_TYPE_EVGL,
                              _evgl_api_thread_glProgramUniform1uiv,
                              thread_data,
                              thread_mode);
}

/*
 * void
 * glProgramUniform2uiv(GLuint program, GLint location, GLsizei count, const GLuint *value);
 */

typedef struct
{
   GLuint program;
   GLint location;
   GLsizei count;
   const GLuint *value;

} EVGL_API_Thread_Command_glProgramUniform2uiv;

void (*orig_evgl_api_glProgramUniform2uiv)(GLuint program, GLint location, GLsizei count, const GLuint *value);

static void
_evgl_api_thread_glProgramUniform2uiv(void *data)
{
   EVGL_API_Thread_Command_glProgramUniform2uiv *thread_data =
      (EVGL_API_Thread_Command_glProgramUniform2uiv *)data;

   orig_evgl_api_glProgramUniform2uiv(thread_data->program,
                                      thread_data->location,
                                      thread_data->count,
                                      thread_data->value);

}

EAPI void
glProgramUniform2uiv_evgl_api_thread_cmd(GLuint program, GLint location, GLsizei count, const GLuint *value)
{
   if (!evas_evgl_thread_enabled() || (_main_thread_id != eina_thread_self()))
     {
        orig_evgl_api_glProgramUniform2uiv(program, location, count, value);
        return;
     }

   int thread_mode = EVAS_GL_THREAD_MODE_FINISH;

   EVGL_API_Thread_Command_glProgramUniform2uiv thread_data_local;
   EVGL_API_Thread_Command_glProgramUniform2uiv *thread_data = &thread_data_local;

   thread_data->program = program;
   thread_data->location = location;
   thread_data->count = count;
   thread_data->value = value;

   evas_gl_thread_cmd_enqueue(EVAS_GL_THREAD_TYPE_EVGL,
                              _evgl_api_thread_glProgramUniform2uiv,
                              thread_data,
                              thread_mode);
}

/*
 * void
 * glProgramUniform3uiv(GLuint program, GLint location, GLsizei count, const GLuint *value);
 */

typedef struct
{
   GLuint program;
   GLint location;
   GLsizei count;
   const GLuint *value;

} EVGL_API_Thread_Command_glProgramUniform3uiv;

void (*orig_evgl_api_glProgramUniform3uiv)(GLuint program, GLint location, GLsizei count, const GLuint *value);

static void
_evgl_api_thread_glProgramUniform3uiv(void *data)
{
   EVGL_API_Thread_Command_glProgramUniform3uiv *thread_data =
      (EVGL_API_Thread_Command_glProgramUniform3uiv *)data;

   orig_evgl_api_glProgramUniform3uiv(thread_data->program,
                                      thread_data->location,
                                      thread_data->count,
                                      thread_data->value);

}

EAPI void
glProgramUniform3uiv_evgl_api_thread_cmd(GLuint program, GLint location, GLsizei count, const GLuint *value)
{
   if (!evas_evgl_thread_enabled() || (_main_thread_id != eina_thread_self()))
     {
        orig_evgl_api_glProgramUniform3uiv(program, location, count, value);
        return;
     }

   int thread_mode = EVAS_GL_THREAD_MODE_FINISH;

   EVGL_API_Thread_Command_glProgramUniform3uiv thread_data_local;
   EVGL_API_Thread_Command_glProgramUniform3uiv *thread_data = &thread_data_local;

   thread_data->program = program;
   thread_data->location = location;
   thread_data->count = count;
   thread_data->value = value;

   evas_gl_thread_cmd_enqueue(EVAS_GL_THREAD_TYPE_EVGL,
                              _evgl_api_thread_glProgramUniform3uiv,
                              thread_data,
                              thread_mode);
}

/*
 * void
 * glProgramUniform4uiv(GLuint program, GLint location, GLsizei count, const GLuint *value);
 */

typedef struct
{
   GLuint program;
   GLint location;
   GLsizei count;
   const GLuint *value;

} EVGL_API_Thread_Command_glProgramUniform4uiv;

void (*orig_evgl_api_glProgramUniform4uiv)(GLuint program, GLint location, GLsizei count, const GLuint *value);

static void
_evgl_api_thread_glProgramUniform4uiv(void *data)
{
   EVGL_API_Thread_Command_glProgramUniform4uiv *thread_data =
      (EVGL_API_Thread_Command_glProgramUniform4uiv *)data;

   orig_evgl_api_glProgramUniform4uiv(thread_data->program,
                                      thread_data->location,
                                      thread_data->count,
                                      thread_data->value);

}

EAPI void
glProgramUniform4uiv_evgl_api_thread_cmd(GLuint program, GLint location, GLsizei count, const GLuint *value)
{
   if (!evas_evgl_thread_enabled() || (_main_thread_id != eina_thread_self()))
     {
        orig_evgl_api_glProgramUniform4uiv(program, location, count, value);
        return;
     }

   int thread_mode = EVAS_GL_THREAD_MODE_FINISH;

   EVGL_API_Thread_Command_glProgramUniform4uiv thread_data_local;
   EVGL_API_Thread_Command_glProgramUniform4uiv *thread_data = &thread_data_local;

   thread_data->program = program;
   thread_data->location = location;
   thread_data->count = count;
   thread_data->value = value;

   evas_gl_thread_cmd_enqueue(EVAS_GL_THREAD_TYPE_EVGL,
                              _evgl_api_thread_glProgramUniform4uiv,
                              thread_data,
                              thread_mode);
}

/*
 * void
 * glProgramUniform1fv(GLuint program, GLint location, GLsizei count, const GLfloat *value);
 */

typedef struct
{
   GLuint program;
   GLint location;
   GLsizei count;
   const GLfloat *value;

} EVGL_API_Thread_Command_glProgramUniform1fv;

void (*orig_evgl_api_glProgramUniform1fv)(GLuint program, GLint location, GLsizei count, const GLfloat *value);

static void
_evgl_api_thread_glProgramUniform1fv(void *data)
{
   EVGL_API_Thread_Command_glProgramUniform1fv *thread_data =
      (EVGL_API_Thread_Command_glProgramUniform1fv *)data;

   orig_evgl_api_glProgramUniform1fv(thread_data->program,
                                     thread_data->location,
                                     thread_data->count,
                                     thread_data->value);

}

EAPI void
glProgramUniform1fv_evgl_api_thread_cmd(GLuint program, GLint location, GLsizei count, const GLfloat *value)
{
   if (!evas_evgl_thread_enabled() || (_main_thread_id != eina_thread_self()))
     {
        orig_evgl_api_glProgramUniform1fv(program, location, count, value);
        return;
     }

   int thread_mode = EVAS_GL_THREAD_MODE_FINISH;

   EVGL_API_Thread_Command_glProgramUniform1fv thread_data_local;
   EVGL_API_Thread_Command_glProgramUniform1fv *thread_data = &thread_data_local;

   thread_data->program = program;
   thread_data->location = location;
   thread_data->count = count;
   thread_data->value = value;

   evas_gl_thread_cmd_enqueue(EVAS_GL_THREAD_TYPE_EVGL,
                              _evgl_api_thread_glProgramUniform1fv,
                              thread_data,
                              thread_mode);
}

/*
 * void
 * glProgramUniform2fv(GLuint program, GLint location, GLsizei count, const GLfloat *value);
 */

typedef struct
{
   GLuint program;
   GLint location;
   GLsizei count;
   const GLfloat *value;

} EVGL_API_Thread_Command_glProgramUniform2fv;

void (*orig_evgl_api_glProgramUniform2fv)(GLuint program, GLint location, GLsizei count, const GLfloat *value);

static void
_evgl_api_thread_glProgramUniform2fv(void *data)
{
   EVGL_API_Thread_Command_glProgramUniform2fv *thread_data =
      (EVGL_API_Thread_Command_glProgramUniform2fv *)data;

   orig_evgl_api_glProgramUniform2fv(thread_data->program,
                                     thread_data->location,
                                     thread_data->count,
                                     thread_data->value);

}

EAPI void
glProgramUniform2fv_evgl_api_thread_cmd(GLuint program, GLint location, GLsizei count, const GLfloat *value)
{
   if (!evas_evgl_thread_enabled() || (_main_thread_id != eina_thread_self()))
     {
        orig_evgl_api_glProgramUniform2fv(program, location, count, value);
        return;
     }

   int thread_mode = EVAS_GL_THREAD_MODE_FINISH;

   EVGL_API_Thread_Command_glProgramUniform2fv thread_data_local;
   EVGL_API_Thread_Command_glProgramUniform2fv *thread_data = &thread_data_local;

   thread_data->program = program;
   thread_data->location = location;
   thread_data->count = count;
   thread_data->value = value;

   evas_gl_thread_cmd_enqueue(EVAS_GL_THREAD_TYPE_EVGL,
                              _evgl_api_thread_glProgramUniform2fv,
                              thread_data,
                              thread_mode);
}

/*
 * void
 * glProgramUniform3fv(GLuint program, GLint location, GLsizei count, const GLfloat *value);
 */

typedef struct
{
   GLuint program;
   GLint location;
   GLsizei count;
   const GLfloat *value;

} EVGL_API_Thread_Command_glProgramUniform3fv;

void (*orig_evgl_api_glProgramUniform3fv)(GLuint program, GLint location, GLsizei count, const GLfloat *value);

static void
_evgl_api_thread_glProgramUniform3fv(void *data)
{
   EVGL_API_Thread_Command_glProgramUniform3fv *thread_data =
      (EVGL_API_Thread_Command_glProgramUniform3fv *)data;

   orig_evgl_api_glProgramUniform3fv(thread_data->program,
                                     thread_data->location,
                                     thread_data->count,
                                     thread_data->value);

}

EAPI void
glProgramUniform3fv_evgl_api_thread_cmd(GLuint program, GLint location, GLsizei count, const GLfloat *value)
{
   if (!evas_evgl_thread_enabled() || (_main_thread_id != eina_thread_self()))
     {
        orig_evgl_api_glProgramUniform3fv(program, location, count, value);
        return;
     }

   int thread_mode = EVAS_GL_THREAD_MODE_FINISH;

   EVGL_API_Thread_Command_glProgramUniform3fv thread_data_local;
   EVGL_API_Thread_Command_glProgramUniform3fv *thread_data = &thread_data_local;

   thread_data->program = program;
   thread_data->location = location;
   thread_data->count = count;
   thread_data->value = value;

   evas_gl_thread_cmd_enqueue(EVAS_GL_THREAD_TYPE_EVGL,
                              _evgl_api_thread_glProgramUniform3fv,
                              thread_data,
                              thread_mode);
}

/*
 * void
 * glProgramUniform4fv(GLuint program, GLint location, GLsizei count, const GLfloat *value);
 */

typedef struct
{
   GLuint program;
   GLint location;
   GLsizei count;
   const GLfloat *value;

} EVGL_API_Thread_Command_glProgramUniform4fv;

void (*orig_evgl_api_glProgramUniform4fv)(GLuint program, GLint location, GLsizei count, const GLfloat *value);

static void
_evgl_api_thread_glProgramUniform4fv(void *data)
{
   EVGL_API_Thread_Command_glProgramUniform4fv *thread_data =
      (EVGL_API_Thread_Command_glProgramUniform4fv *)data;

   orig_evgl_api_glProgramUniform4fv(thread_data->program,
                                     thread_data->location,
                                     thread_data->count,
                                     thread_data->value);

}

EAPI void
glProgramUniform4fv_evgl_api_thread_cmd(GLuint program, GLint location, GLsizei count, const GLfloat *value)
{
   if (!evas_evgl_thread_enabled() || (_main_thread_id != eina_thread_self()))
     {
        orig_evgl_api_glProgramUniform4fv(program, location, count, value);
        return;
     }

   int thread_mode = EVAS_GL_THREAD_MODE_FINISH;

   EVGL_API_Thread_Command_glProgramUniform4fv thread_data_local;
   EVGL_API_Thread_Command_glProgramUniform4fv *thread_data = &thread_data_local;

   thread_data->program = program;
   thread_data->location = location;
   thread_data->count = count;
   thread_data->value = value;

   evas_gl_thread_cmd_enqueue(EVAS_GL_THREAD_TYPE_EVGL,
                              _evgl_api_thread_glProgramUniform4fv,
                              thread_data,
                              thread_mode);
}

/*
 * void
 * glProgramUniformMatrix2fv(GLuint program, GLint location, GLsizei count, GLboolean transpose, const GLfloat *value);
 */

typedef struct
{
   GLuint program;
   GLint location;
   GLsizei count;
   GLboolean transpose;
   const GLfloat *value;

} EVGL_API_Thread_Command_glProgramUniformMatrix2fv;

void (*orig_evgl_api_glProgramUniformMatrix2fv)(GLuint program, GLint location, GLsizei count, GLboolean transpose, const GLfloat *value);

static void
_evgl_api_thread_glProgramUniformMatrix2fv(void *data)
{
   EVGL_API_Thread_Command_glProgramUniformMatrix2fv *thread_data =
      (EVGL_API_Thread_Command_glProgramUniformMatrix2fv *)data;

   orig_evgl_api_glProgramUniformMatrix2fv(thread_data->program,
                                           thread_data->location,
                                           thread_data->count,
                                           thread_data->transpose,
                                           thread_data->value);

}

EAPI void
glProgramUniformMatrix2fv_evgl_api_thread_cmd(GLuint program, GLint location, GLsizei count, GLboolean transpose, const GLfloat *value)
{
   if (!evas_evgl_thread_enabled() || (_main_thread_id != eina_thread_self()))
     {
        orig_evgl_api_glProgramUniformMatrix2fv(program, location, count, transpose, value);
        return;
     }

   int thread_mode = EVAS_GL_THREAD_MODE_FINISH;

   EVGL_API_Thread_Command_glProgramUniformMatrix2fv thread_data_local;
   EVGL_API_Thread_Command_glProgramUniformMatrix2fv *thread_data = &thread_data_local;

   thread_data->program = program;
   thread_data->location = location;
   thread_data->count = count;
   thread_data->transpose = transpose;
   thread_data->value = value;

   evas_gl_thread_cmd_enqueue(EVAS_GL_THREAD_TYPE_EVGL,
                              _evgl_api_thread_glProgramUniformMatrix2fv,
                              thread_data,
                              thread_mode);
}

/*
 * void
 * glProgramUniformMatrix3fv(GLuint program, GLint location, GLsizei count, GLboolean transpose, const GLfloat *value);
 */

typedef struct
{
   GLuint program;
   GLint location;
   GLsizei count;
   GLboolean transpose;
   const GLfloat *value;

} EVGL_API_Thread_Command_glProgramUniformMatrix3fv;

void (*orig_evgl_api_glProgramUniformMatrix3fv)(GLuint program, GLint location, GLsizei count, GLboolean transpose, const GLfloat *value);

static void
_evgl_api_thread_glProgramUniformMatrix3fv(void *data)
{
   EVGL_API_Thread_Command_glProgramUniformMatrix3fv *thread_data =
      (EVGL_API_Thread_Command_glProgramUniformMatrix3fv *)data;

   orig_evgl_api_glProgramUniformMatrix3fv(thread_data->program,
                                           thread_data->location,
                                           thread_data->count,
                                           thread_data->transpose,
                                           thread_data->value);

}

EAPI void
glProgramUniformMatrix3fv_evgl_api_thread_cmd(GLuint program, GLint location, GLsizei count, GLboolean transpose, const GLfloat *value)
{
   if (!evas_evgl_thread_enabled() || (_main_thread_id != eina_thread_self()))
     {
        orig_evgl_api_glProgramUniformMatrix3fv(program, location, count, transpose, value);
        return;
     }

   int thread_mode = EVAS_GL_THREAD_MODE_FINISH;

   EVGL_API_Thread_Command_glProgramUniformMatrix3fv thread_data_local;
   EVGL_API_Thread_Command_glProgramUniformMatrix3fv *thread_data = &thread_data_local;

   thread_data->program = program;
   thread_data->location = location;
   thread_data->count = count;
   thread_data->transpose = transpose;
   thread_data->value = value;

   evas_gl_thread_cmd_enqueue(EVAS_GL_THREAD_TYPE_EVGL,
                              _evgl_api_thread_glProgramUniformMatrix3fv,
                              thread_data,
                              thread_mode);
}

/*
 * void
 * glProgramUniformMatrix4fv(GLuint program, GLint location, GLsizei count, GLboolean transpose, const GLfloat *value);
 */

typedef struct
{
   GLuint program;
   GLint location;
   GLsizei count;
   GLboolean transpose;
   const GLfloat *value;

} EVGL_API_Thread_Command_glProgramUniformMatrix4fv;

void (*orig_evgl_api_glProgramUniformMatrix4fv)(GLuint program, GLint location, GLsizei count, GLboolean transpose, const GLfloat *value);

static void
_evgl_api_thread_glProgramUniformMatrix4fv(void *data)
{
   EVGL_API_Thread_Command_glProgramUniformMatrix4fv *thread_data =
      (EVGL_API_Thread_Command_glProgramUniformMatrix4fv *)data;

   orig_evgl_api_glProgramUniformMatrix4fv(thread_data->program,
                                           thread_data->location,
                                           thread_data->count,
                                           thread_data->transpose,
                                           thread_data->value);

}

EAPI void
glProgramUniformMatrix4fv_evgl_api_thread_cmd(GLuint program, GLint location, GLsizei count, GLboolean transpose, const GLfloat *value)
{
   if (!evas_evgl_thread_enabled() || (_main_thread_id != eina_thread_self()))
     {
        orig_evgl_api_glProgramUniformMatrix4fv(program, location, count, transpose, value);
        return;
     }

   int thread_mode = EVAS_GL_THREAD_MODE_FINISH;

   EVGL_API_Thread_Command_glProgramUniformMatrix4fv thread_data_local;
   EVGL_API_Thread_Command_glProgramUniformMatrix4fv *thread_data = &thread_data_local;

   thread_data->program = program;
   thread_data->location = location;
   thread_data->count = count;
   thread_data->transpose = transpose;
   thread_data->value = value;

   evas_gl_thread_cmd_enqueue(EVAS_GL_THREAD_TYPE_EVGL,
                              _evgl_api_thread_glProgramUniformMatrix4fv,
                              thread_data,
                              thread_mode);
}

/*
 * void
 * glProgramUniformMatrix2x3fv(GLuint program, GLint location, GLsizei count, GLboolean transpose, const GLfloat *value);
 */

typedef struct
{
   GLuint program;
   GLint location;
   GLsizei count;
   GLboolean transpose;
   const GLfloat *value;

} EVGL_API_Thread_Command_glProgramUniformMatrix2x3fv;

void (*orig_evgl_api_glProgramUniformMatrix2x3fv)(GLuint program, GLint location, GLsizei count, GLboolean transpose, const GLfloat *value);

static void
_evgl_api_thread_glProgramUniformMatrix2x3fv(void *data)
{
   EVGL_API_Thread_Command_glProgramUniformMatrix2x3fv *thread_data =
      (EVGL_API_Thread_Command_glProgramUniformMatrix2x3fv *)data;

   orig_evgl_api_glProgramUniformMatrix2x3fv(thread_data->program,
                                             thread_data->location,
                                             thread_data->count,
                                             thread_data->transpose,
                                             thread_data->value);

}

EAPI void
glProgramUniformMatrix2x3fv_evgl_api_thread_cmd(GLuint program, GLint location, GLsizei count, GLboolean transpose, const GLfloat *value)
{
   if (!evas_evgl_thread_enabled() || (_main_thread_id != eina_thread_self()))
     {
        orig_evgl_api_glProgramUniformMatrix2x3fv(program, location, count, transpose, value);
        return;
     }

   int thread_mode = EVAS_GL_THREAD_MODE_FINISH;

   EVGL_API_Thread_Command_glProgramUniformMatrix2x3fv thread_data_local;
   EVGL_API_Thread_Command_glProgramUniformMatrix2x3fv *thread_data = &thread_data_local;

   thread_data->program = program;
   thread_data->location = location;
   thread_data->count = count;
   thread_data->transpose = transpose;
   thread_data->value = value;

   evas_gl_thread_cmd_enqueue(EVAS_GL_THREAD_TYPE_EVGL,
                              _evgl_api_thread_glProgramUniformMatrix2x3fv,
                              thread_data,
                              thread_mode);
}

/*
 * void
 * glProgramUniformMatrix3x2fv(GLuint program, GLint location, GLsizei count, GLboolean transpose, const GLfloat *value);
 */

typedef struct
{
   GLuint program;
   GLint location;
   GLsizei count;
   GLboolean transpose;
   const GLfloat *value;

} EVGL_API_Thread_Command_glProgramUniformMatrix3x2fv;

void (*orig_evgl_api_glProgramUniformMatrix3x2fv)(GLuint program, GLint location, GLsizei count, GLboolean transpose, const GLfloat *value);

static void
_evgl_api_thread_glProgramUniformMatrix3x2fv(void *data)
{
   EVGL_API_Thread_Command_glProgramUniformMatrix3x2fv *thread_data =
      (EVGL_API_Thread_Command_glProgramUniformMatrix3x2fv *)data;

   orig_evgl_api_glProgramUniformMatrix3x2fv(thread_data->program,
                                             thread_data->location,
                                             thread_data->count,
                                             thread_data->transpose,
                                             thread_data->value);

}

EAPI void
glProgramUniformMatrix3x2fv_evgl_api_thread_cmd(GLuint program, GLint location, GLsizei count, GLboolean transpose, const GLfloat *value)
{
   if (!evas_evgl_thread_enabled() || (_main_thread_id != eina_thread_self()))
     {
        orig_evgl_api_glProgramUniformMatrix3x2fv(program, location, count, transpose, value);
        return;
     }

   int thread_mode = EVAS_GL_THREAD_MODE_FINISH;

   EVGL_API_Thread_Command_glProgramUniformMatrix3x2fv thread_data_local;
   EVGL_API_Thread_Command_glProgramUniformMatrix3x2fv *thread_data = &thread_data_local;

   thread_data->program = program;
   thread_data->location = location;
   thread_data->count = count;
   thread_data->transpose = transpose;
   thread_data->value = value;

   evas_gl_thread_cmd_enqueue(EVAS_GL_THREAD_TYPE_EVGL,
                              _evgl_api_thread_glProgramUniformMatrix3x2fv,
                              thread_data,
                              thread_mode);
}

/*
 * void
 * glProgramUniformMatrix2x4fv(GLuint program, GLint location, GLsizei count, GLboolean transpose, const GLfloat *value);
 */

typedef struct
{
   GLuint program;
   GLint location;
   GLsizei count;
   GLboolean transpose;
   const GLfloat *value;

} EVGL_API_Thread_Command_glProgramUniformMatrix2x4fv;

void (*orig_evgl_api_glProgramUniformMatrix2x4fv)(GLuint program, GLint location, GLsizei count, GLboolean transpose, const GLfloat *value);

static void
_evgl_api_thread_glProgramUniformMatrix2x4fv(void *data)
{
   EVGL_API_Thread_Command_glProgramUniformMatrix2x4fv *thread_data =
      (EVGL_API_Thread_Command_glProgramUniformMatrix2x4fv *)data;

   orig_evgl_api_glProgramUniformMatrix2x4fv(thread_data->program,
                                             thread_data->location,
                                             thread_data->count,
                                             thread_data->transpose,
                                             thread_data->value);

}

EAPI void
glProgramUniformMatrix2x4fv_evgl_api_thread_cmd(GLuint program, GLint location, GLsizei count, GLboolean transpose, const GLfloat *value)
{
   if (!evas_evgl_thread_enabled() || (_main_thread_id != eina_thread_self()))
     {
        orig_evgl_api_glProgramUniformMatrix2x4fv(program, location, count, transpose, value);
        return;
     }

   int thread_mode = EVAS_GL_THREAD_MODE_FINISH;

   EVGL_API_Thread_Command_glProgramUniformMatrix2x4fv thread_data_local;
   EVGL_API_Thread_Command_glProgramUniformMatrix2x4fv *thread_data = &thread_data_local;

   thread_data->program = program;
   thread_data->location = location;
   thread_data->count = count;
   thread_data->transpose = transpose;
   thread_data->value = value;

   evas_gl_thread_cmd_enqueue(EVAS_GL_THREAD_TYPE_EVGL,
                              _evgl_api_thread_glProgramUniformMatrix2x4fv,
                              thread_data,
                              thread_mode);
}

/*
 * void
 * glProgramUniformMatrix4x2fv(GLuint program, GLint location, GLsizei count, GLboolean transpose, const GLfloat *value);
 */

typedef struct
{
   GLuint program;
   GLint location;
   GLsizei count;
   GLboolean transpose;
   const GLfloat *value;

} EVGL_API_Thread_Command_glProgramUniformMatrix4x2fv;

void (*orig_evgl_api_glProgramUniformMatrix4x2fv)(GLuint program, GLint location, GLsizei count, GLboolean transpose, const GLfloat *value);

static void
_evgl_api_thread_glProgramUniformMatrix4x2fv(void *data)
{
   EVGL_API_Thread_Command_glProgramUniformMatrix4x2fv *thread_data =
      (EVGL_API_Thread_Command_glProgramUniformMatrix4x2fv *)data;

   orig_evgl_api_glProgramUniformMatrix4x2fv(thread_data->program,
                                             thread_data->location,
                                             thread_data->count,
                                             thread_data->transpose,
                                             thread_data->value);

}

EAPI void
glProgramUniformMatrix4x2fv_evgl_api_thread_cmd(GLuint program, GLint location, GLsizei count, GLboolean transpose, const GLfloat *value)
{
   if (!evas_evgl_thread_enabled() || (_main_thread_id != eina_thread_self()))
     {
        orig_evgl_api_glProgramUniformMatrix4x2fv(program, location, count, transpose, value);
        return;
     }

   int thread_mode = EVAS_GL_THREAD_MODE_FINISH;

   EVGL_API_Thread_Command_glProgramUniformMatrix4x2fv thread_data_local;
   EVGL_API_Thread_Command_glProgramUniformMatrix4x2fv *thread_data = &thread_data_local;

   thread_data->program = program;
   thread_data->location = location;
   thread_data->count = count;
   thread_data->transpose = transpose;
   thread_data->value = value;

   evas_gl_thread_cmd_enqueue(EVAS_GL_THREAD_TYPE_EVGL,
                              _evgl_api_thread_glProgramUniformMatrix4x2fv,
                              thread_data,
                              thread_mode);
}

/*
 * void
 * glProgramUniformMatrix3x4fv(GLuint program, GLint location, GLsizei count, GLboolean transpose, const GLfloat *value);
 */

typedef struct
{
   GLuint program;
   GLint location;
   GLsizei count;
   GLboolean transpose;
   const GLfloat *value;

} EVGL_API_Thread_Command_glProgramUniformMatrix3x4fv;

void (*orig_evgl_api_glProgramUniformMatrix3x4fv)(GLuint program, GLint location, GLsizei count, GLboolean transpose, const GLfloat *value);

static void
_evgl_api_thread_glProgramUniformMatrix3x4fv(void *data)
{
   EVGL_API_Thread_Command_glProgramUniformMatrix3x4fv *thread_data =
      (EVGL_API_Thread_Command_glProgramUniformMatrix3x4fv *)data;

   orig_evgl_api_glProgramUniformMatrix3x4fv(thread_data->program,
                                             thread_data->location,
                                             thread_data->count,
                                             thread_data->transpose,
                                             thread_data->value);

}

EAPI void
glProgramUniformMatrix3x4fv_evgl_api_thread_cmd(GLuint program, GLint location, GLsizei count, GLboolean transpose, const GLfloat *value)
{
   if (!evas_evgl_thread_enabled() || (_main_thread_id != eina_thread_self()))
     {
        orig_evgl_api_glProgramUniformMatrix3x4fv(program, location, count, transpose, value);
        return;
     }

   int thread_mode = EVAS_GL_THREAD_MODE_FINISH;

   EVGL_API_Thread_Command_glProgramUniformMatrix3x4fv thread_data_local;
   EVGL_API_Thread_Command_glProgramUniformMatrix3x4fv *thread_data = &thread_data_local;

   thread_data->program = program;
   thread_data->location = location;
   thread_data->count = count;
   thread_data->transpose = transpose;
   thread_data->value = value;

   evas_gl_thread_cmd_enqueue(EVAS_GL_THREAD_TYPE_EVGL,
                              _evgl_api_thread_glProgramUniformMatrix3x4fv,
                              thread_data,
                              thread_mode);
}

/*
 * void
 * glProgramUniformMatrix4x3fv(GLuint program, GLint location, GLsizei count, GLboolean transpose, const GLfloat *value);
 */

typedef struct
{
   GLuint program;
   GLint location;
   GLsizei count;
   GLboolean transpose;
   const GLfloat *value;

} EVGL_API_Thread_Command_glProgramUniformMatrix4x3fv;

void (*orig_evgl_api_glProgramUniformMatrix4x3fv)(GLuint program, GLint location, GLsizei count, GLboolean transpose, const GLfloat *value);

static void
_evgl_api_thread_glProgramUniformMatrix4x3fv(void *data)
{
   EVGL_API_Thread_Command_glProgramUniformMatrix4x3fv *thread_data =
      (EVGL_API_Thread_Command_glProgramUniformMatrix4x3fv *)data;

   orig_evgl_api_glProgramUniformMatrix4x3fv(thread_data->program,
                                             thread_data->location,
                                             thread_data->count,
                                             thread_data->transpose,
                                             thread_data->value);

}

EAPI void
glProgramUniformMatrix4x3fv_evgl_api_thread_cmd(GLuint program, GLint location, GLsizei count, GLboolean transpose, const GLfloat *value)
{
   if (!evas_evgl_thread_enabled() || (_main_thread_id != eina_thread_self()))
     {
        orig_evgl_api_glProgramUniformMatrix4x3fv(program, location, count, transpose, value);
        return;
     }

   int thread_mode = EVAS_GL_THREAD_MODE_FINISH;

   EVGL_API_Thread_Command_glProgramUniformMatrix4x3fv thread_data_local;
   EVGL_API_Thread_Command_glProgramUniformMatrix4x3fv *thread_data = &thread_data_local;

   thread_data->program = program;
   thread_data->location = location;
   thread_data->count = count;
   thread_data->transpose = transpose;
   thread_data->value = value;

   evas_gl_thread_cmd_enqueue(EVAS_GL_THREAD_TYPE_EVGL,
                              _evgl_api_thread_glProgramUniformMatrix4x3fv,
                              thread_data,
                              thread_mode);
}

/*
 * void
 * glValidateProgramPipeline(GLuint pipeline);
 */

typedef struct
{
   GLuint pipeline;

} EVGL_API_Thread_Command_glValidateProgramPipeline;

void (*orig_evgl_api_glValidateProgramPipeline)(GLuint pipeline);

static void
_evgl_api_thread_glValidateProgramPipeline(void *data)
{
   EVGL_API_Thread_Command_glValidateProgramPipeline *thread_data =
      (EVGL_API_Thread_Command_glValidateProgramPipeline *)data;

   orig_evgl_api_glValidateProgramPipeline(thread_data->pipeline);

}

EAPI void
glValidateProgramPipeline_evgl_api_thread_cmd(GLuint pipeline)
{
   if (!evas_evgl_thread_enabled() || (_main_thread_id != eina_thread_self()))
     {
        orig_evgl_api_glValidateProgramPipeline(pipeline);
        return;
     }

   int thread_mode = EVAS_GL_THREAD_MODE_FINISH;

   EVGL_API_Thread_Command_glValidateProgramPipeline thread_data_local;
   EVGL_API_Thread_Command_glValidateProgramPipeline *thread_data = &thread_data_local;

   thread_data->pipeline = pipeline;

   evas_gl_thread_cmd_enqueue(EVAS_GL_THREAD_TYPE_EVGL,
                              _evgl_api_thread_glValidateProgramPipeline,
                              thread_data,
                              thread_mode);
}

/*
 * void
 * glGetProgramPipelineInfoLog(GLuint pipeline, GLsizei bufSize, GLsizei *length, GLchar *infoLog);
 */

typedef struct
{
   GLuint pipeline;
   GLsizei bufSize;
   GLsizei *length;
   GLchar *infoLog;

} EVGL_API_Thread_Command_glGetProgramPipelineInfoLog;

void (*orig_evgl_api_glGetProgramPipelineInfoLog)(GLuint pipeline, GLsizei bufSize, GLsizei *length, GLchar *infoLog);

static void
_evgl_api_thread_glGetProgramPipelineInfoLog(void *data)
{
   EVGL_API_Thread_Command_glGetProgramPipelineInfoLog *thread_data =
      (EVGL_API_Thread_Command_glGetProgramPipelineInfoLog *)data;

   orig_evgl_api_glGetProgramPipelineInfoLog(thread_data->pipeline,
                                             thread_data->bufSize,
                                             thread_data->length,
                                             thread_data->infoLog);

}

EAPI void
glGetProgramPipelineInfoLog_evgl_api_thread_cmd(GLuint pipeline, GLsizei bufSize, GLsizei *length, GLchar *infoLog)
{
   if (!evas_evgl_thread_enabled() || (_main_thread_id != eina_thread_self()))
     {
        orig_evgl_api_glGetProgramPipelineInfoLog(pipeline, bufSize, length, infoLog);
        return;
     }

   int thread_mode = EVAS_GL_THREAD_MODE_FINISH;

   EVGL_API_Thread_Command_glGetProgramPipelineInfoLog thread_data_local;
   EVGL_API_Thread_Command_glGetProgramPipelineInfoLog *thread_data = &thread_data_local;

   thread_data->pipeline = pipeline;
   thread_data->bufSize = bufSize;
   thread_data->length = length;
   thread_data->infoLog = infoLog;

   evas_gl_thread_cmd_enqueue(EVAS_GL_THREAD_TYPE_EVGL,
                              _evgl_api_thread_glGetProgramPipelineInfoLog,
                              thread_data,
                              thread_mode);
}

/*
 * void
 * glBindImageTexture(GLuint unit, GLuint texture, GLint level, GLboolean layered, GLint layer, GLenum access, GLenum format);
 */

typedef struct
{
   GLuint unit;
   GLuint texture;
   GLint level;
   GLboolean layered;
   GLint layer;
   GLenum access;
   GLenum format;

} EVGL_API_Thread_Command_glBindImageTexture;

void (*orig_evgl_api_glBindImageTexture)(GLuint unit, GLuint texture, GLint level, GLboolean layered, GLint layer, GLenum access, GLenum format);

static void
_evgl_api_thread_glBindImageTexture(void *data)
{
   EVGL_API_Thread_Command_glBindImageTexture *thread_data =
      (EVGL_API_Thread_Command_glBindImageTexture *)data;

   orig_evgl_api_glBindImageTexture(thread_data->unit,
                                    thread_data->texture,
                                    thread_data->level,
                                    thread_data->layered,
                                    thread_data->layer,
                                    thread_data->access,
                                    thread_data->format);

}

EAPI void
glBindImageTexture_evgl_api_thread_cmd(GLuint unit, GLuint texture, GLint level, GLboolean layered, GLint layer, GLenum access, GLenum format)
{
   if (!evas_evgl_thread_enabled() || (_main_thread_id != eina_thread_self()))
     {
        orig_evgl_api_glBindImageTexture(unit, texture, level, layered, layer, access, format);
        return;
     }

   int thread_mode = EVAS_GL_THREAD_MODE_FINISH;

   EVGL_API_Thread_Command_glBindImageTexture thread_data_local;
   EVGL_API_Thread_Command_glBindImageTexture *thread_data = &thread_data_local;

   thread_data->unit = unit;
   thread_data->texture = texture;
   thread_data->level = level;
   thread_data->layered = layered;
   thread_data->layer = layer;
   thread_data->access = access;
   thread_data->format = format;

   evas_gl_thread_cmd_enqueue(EVAS_GL_THREAD_TYPE_EVGL,
                              _evgl_api_thread_glBindImageTexture,
                              thread_data,
                              thread_mode);
}

/*
 * void
 * glGetBooleani_v(GLenum target, GLuint index, GLboolean *data);
 */

typedef struct
{
   GLenum target;
   GLuint index;
   GLboolean *data;

} EVGL_API_Thread_Command_glGetBooleani_v;

void (*orig_evgl_api_glGetBooleani_v)(GLenum target, GLuint index, GLboolean *data);

static void
_evgl_api_thread_glGetBooleani_v(void *data)
{
   EVGL_API_Thread_Command_glGetBooleani_v *thread_data =
      (EVGL_API_Thread_Command_glGetBooleani_v *)data;

   orig_evgl_api_glGetBooleani_v(thread_data->target,
                                 thread_data->index,
                                 thread_data->data);

}

EAPI void
glGetBooleani_v_evgl_api_thread_cmd(GLenum target, GLuint index, GLboolean *data)
{
   if (!evas_evgl_thread_enabled() || (_main_thread_id != eina_thread_self()))
     {
        orig_evgl_api_glGetBooleani_v(target, index, data);
        return;
     }

   int thread_mode = EVAS_GL_THREAD_MODE_FINISH;

   EVGL_API_Thread_Command_glGetBooleani_v thread_data_local;
   EVGL_API_Thread_Command_glGetBooleani_v *thread_data = &thread_data_local;

   thread_data->target = target;
   thread_data->index = index;
   thread_data->data = data;

   evas_gl_thread_cmd_enqueue(EVAS_GL_THREAD_TYPE_EVGL,
                              _evgl_api_thread_glGetBooleani_v,
                              thread_data,
                              thread_mode);
}

/*
 * void
 * glMemoryBarrier(GLbitfield barriers);
 */

typedef struct
{
   GLbitfield barriers;

} EVGL_API_Thread_Command_glMemoryBarrier;

void (*orig_evgl_api_glMemoryBarrier)(GLbitfield barriers);

static void
_evgl_api_thread_glMemoryBarrier(void *data)
{
   EVGL_API_Thread_Command_glMemoryBarrier *thread_data =
      (EVGL_API_Thread_Command_glMemoryBarrier *)data;

   orig_evgl_api_glMemoryBarrier(thread_data->barriers);

}

EAPI void
glMemoryBarrier_evgl_api_thread_cmd(GLbitfield barriers)
{
   if (!evas_evgl_thread_enabled() || (_main_thread_id != eina_thread_self()))
     {
        orig_evgl_api_glMemoryBarrier(barriers);
        return;
     }

   int thread_mode = EVAS_GL_THREAD_MODE_FINISH;

   EVGL_API_Thread_Command_glMemoryBarrier thread_data_local;
   EVGL_API_Thread_Command_glMemoryBarrier *thread_data = &thread_data_local;

   thread_data->barriers = barriers;

   evas_gl_thread_cmd_enqueue(EVAS_GL_THREAD_TYPE_EVGL,
                              _evgl_api_thread_glMemoryBarrier,
                              thread_data,
                              thread_mode);
}

/*
 * void
 * glMemoryBarrierByRegion(GLbitfield barriers);
 */

typedef struct
{
   GLbitfield barriers;

} EVGL_API_Thread_Command_glMemoryBarrierByRegion;

void (*orig_evgl_api_glMemoryBarrierByRegion)(GLbitfield barriers);

static void
_evgl_api_thread_glMemoryBarrierByRegion(void *data)
{
   EVGL_API_Thread_Command_glMemoryBarrierByRegion *thread_data =
      (EVGL_API_Thread_Command_glMemoryBarrierByRegion *)data;

   orig_evgl_api_glMemoryBarrierByRegion(thread_data->barriers);

}

EAPI void
glMemoryBarrierByRegion_evgl_api_thread_cmd(GLbitfield barriers)
{
   if (!evas_evgl_thread_enabled() || (_main_thread_id != eina_thread_self()))
     {
        orig_evgl_api_glMemoryBarrierByRegion(barriers);
        return;
     }

   int thread_mode = EVAS_GL_THREAD_MODE_FINISH;

   EVGL_API_Thread_Command_glMemoryBarrierByRegion thread_data_local;
   EVGL_API_Thread_Command_glMemoryBarrierByRegion *thread_data = &thread_data_local;

   thread_data->barriers = barriers;

   evas_gl_thread_cmd_enqueue(EVAS_GL_THREAD_TYPE_EVGL,
                              _evgl_api_thread_glMemoryBarrierByRegion,
                              thread_data,
                              thread_mode);
}

/*
 * void
 * glTexStorage2DMultisample(GLenum target, GLsizei samples, GLenum internalformat, GLsizei width, GLsizei height, GLboolean fixedsamplelocations);
 */

typedef struct
{
   GLenum target;
   GLsizei samples;
   GLenum internalformat;
   GLsizei width;
   GLsizei height;
   GLboolean fixedsamplelocations;

} EVGL_API_Thread_Command_glTexStorage2DMultisample;

void (*orig_evgl_api_glTexStorage2DMultisample)(GLenum target, GLsizei samples, GLenum internalformat, GLsizei width, GLsizei height, GLboolean fixedsamplelocations);

static void
_evgl_api_thread_glTexStorage2DMultisample(void *data)
{
   EVGL_API_Thread_Command_glTexStorage2DMultisample *thread_data =
      (EVGL_API_Thread_Command_glTexStorage2DMultisample *)data;

   orig_evgl_api_glTexStorage2DMultisample(thread_data->target,
                                           thread_data->samples,
                                           thread_data->internalformat,
                                           thread_data->width,
                                           thread_data->height,
                                           thread_data->fixedsamplelocations);

}

EAPI void
glTexStorage2DMultisample_evgl_api_thread_cmd(GLenum target, GLsizei samples, GLenum internalformat, GLsizei width, GLsizei height, GLboolean fixedsamplelocations)
{
   if (!evas_evgl_thread_enabled() || (_main_thread_id != eina_thread_self()))
     {
        orig_evgl_api_glTexStorage2DMultisample(target, samples, internalformat, width, height, fixedsamplelocations);
        return;
     }

   int thread_mode = EVAS_GL_THREAD_MODE_FINISH;

   EVGL_API_Thread_Command_glTexStorage2DMultisample thread_data_local;
   EVGL_API_Thread_Command_glTexStorage2DMultisample *thread_data = &thread_data_local;

   thread_data->target = target;
   thread_data->samples = samples;
   thread_data->internalformat = internalformat;
   thread_data->width = width;
   thread_data->height = height;
   thread_data->fixedsamplelocations = fixedsamplelocations;

   evas_gl_thread_cmd_enqueue(EVAS_GL_THREAD_TYPE_EVGL,
                              _evgl_api_thread_glTexStorage2DMultisample,
                              thread_data,
                              thread_mode);
}

/*
 * void
 * glGetMultisamplefv(GLenum pname, GLuint index, GLfloat *val);
 */

typedef struct
{
   GLenum pname;
   GLuint index;
   GLfloat *val;

} EVGL_API_Thread_Command_glGetMultisamplefv;

void (*orig_evgl_api_glGetMultisamplefv)(GLenum pname, GLuint index, GLfloat *val);

static void
_evgl_api_thread_glGetMultisamplefv(void *data)
{
   EVGL_API_Thread_Command_glGetMultisamplefv *thread_data =
      (EVGL_API_Thread_Command_glGetMultisamplefv *)data;

   orig_evgl_api_glGetMultisamplefv(thread_data->pname,
                                    thread_data->index,
                                    thread_data->val);

}

EAPI void
glGetMultisamplefv_evgl_api_thread_cmd(GLenum pname, GLuint index, GLfloat *val)
{
   if (!evas_evgl_thread_enabled() || (_main_thread_id != eina_thread_self()))
     {
        orig_evgl_api_glGetMultisamplefv(pname, index, val);
        return;
     }

   int thread_mode = EVAS_GL_THREAD_MODE_FINISH;

   EVGL_API_Thread_Command_glGetMultisamplefv thread_data_local;
   EVGL_API_Thread_Command_glGetMultisamplefv *thread_data = &thread_data_local;

   thread_data->pname = pname;
   thread_data->index = index;
   thread_data->val = val;

   evas_gl_thread_cmd_enqueue(EVAS_GL_THREAD_TYPE_EVGL,
                              _evgl_api_thread_glGetMultisamplefv,
                              thread_data,
                              thread_mode);
}

/*
 * void
 * glSampleMaski(GLuint maskNumber, GLbitfield mask);
 */

typedef struct
{
   GLuint maskNumber;
   GLbitfield mask;

} EVGL_API_Thread_Command_glSampleMaski;

void (*orig_evgl_api_glSampleMaski)(GLuint maskNumber, GLbitfield mask);

static void
_evgl_api_thread_glSampleMaski(void *data)
{
   EVGL_API_Thread_Command_glSampleMaski *thread_data =
      (EVGL_API_Thread_Command_glSampleMaski *)data;

   orig_evgl_api_glSampleMaski(thread_data->maskNumber,
                               thread_data->mask);

}

EAPI void
glSampleMaski_evgl_api_thread_cmd(GLuint maskNumber, GLbitfield mask)
{
   if (!evas_evgl_thread_enabled() || (_main_thread_id != eina_thread_self()))
     {
        orig_evgl_api_glSampleMaski(maskNumber, mask);
        return;
     }

   int thread_mode = EVAS_GL_THREAD_MODE_FINISH;

   EVGL_API_Thread_Command_glSampleMaski thread_data_local;
   EVGL_API_Thread_Command_glSampleMaski *thread_data = &thread_data_local;

   thread_data->maskNumber = maskNumber;
   thread_data->mask = mask;

   evas_gl_thread_cmd_enqueue(EVAS_GL_THREAD_TYPE_EVGL,
                              _evgl_api_thread_glSampleMaski,
                              thread_data,
                              thread_mode);
}

/*
 * void
 * glGetTexLevelParameteriv(GLenum target, GLint level, GLenum pname, GLint *params);
 */

typedef struct
{
   GLenum target;
   GLint level;
   GLenum pname;
   GLint *params;

} EVGL_API_Thread_Command_glGetTexLevelParameteriv;

void (*orig_evgl_api_glGetTexLevelParameteriv)(GLenum target, GLint level, GLenum pname, GLint *params);

static void
_evgl_api_thread_glGetTexLevelParameteriv(void *data)
{
   EVGL_API_Thread_Command_glGetTexLevelParameteriv *thread_data =
      (EVGL_API_Thread_Command_glGetTexLevelParameteriv *)data;

   orig_evgl_api_glGetTexLevelParameteriv(thread_data->target,
                                          thread_data->level,
                                          thread_data->pname,
                                          thread_data->params);

}

EAPI void
glGetTexLevelParameteriv_evgl_api_thread_cmd(GLenum target, GLint level, GLenum pname, GLint *params)
{
   if (!evas_evgl_thread_enabled() || (_main_thread_id != eina_thread_self()))
     {
        orig_evgl_api_glGetTexLevelParameteriv(target, level, pname, params);
        return;
     }

   int thread_mode = EVAS_GL_THREAD_MODE_FINISH;

   EVGL_API_Thread_Command_glGetTexLevelParameteriv thread_data_local;
   EVGL_API_Thread_Command_glGetTexLevelParameteriv *thread_data = &thread_data_local;

   thread_data->target = target;
   thread_data->level = level;
   thread_data->pname = pname;
   thread_data->params = params;

   evas_gl_thread_cmd_enqueue(EVAS_GL_THREAD_TYPE_EVGL,
                              _evgl_api_thread_glGetTexLevelParameteriv,
                              thread_data,
                              thread_mode);
}

/*
 * void
 * glGetTexLevelParameterfv(GLenum target, GLint level, GLenum pname, GLfloat *params);
 */

typedef struct
{
   GLenum target;
   GLint level;
   GLenum pname;
   GLfloat *params;

} EVGL_API_Thread_Command_glGetTexLevelParameterfv;

void (*orig_evgl_api_glGetTexLevelParameterfv)(GLenum target, GLint level, GLenum pname, GLfloat *params);

static void
_evgl_api_thread_glGetTexLevelParameterfv(void *data)
{
   EVGL_API_Thread_Command_glGetTexLevelParameterfv *thread_data =
      (EVGL_API_Thread_Command_glGetTexLevelParameterfv *)data;

   orig_evgl_api_glGetTexLevelParameterfv(thread_data->target,
                                          thread_data->level,
                                          thread_data->pname,
                                          thread_data->params);

}

EAPI void
glGetTexLevelParameterfv_evgl_api_thread_cmd(GLenum target, GLint level, GLenum pname, GLfloat *params)
{
   if (!evas_evgl_thread_enabled() || (_main_thread_id != eina_thread_self()))
     {
        orig_evgl_api_glGetTexLevelParameterfv(target, level, pname, params);
        return;
     }

   int thread_mode = EVAS_GL_THREAD_MODE_FINISH;

   EVGL_API_Thread_Command_glGetTexLevelParameterfv thread_data_local;
   EVGL_API_Thread_Command_glGetTexLevelParameterfv *thread_data = &thread_data_local;

   thread_data->target = target;
   thread_data->level = level;
   thread_data->pname = pname;
   thread_data->params = params;

   evas_gl_thread_cmd_enqueue(EVAS_GL_THREAD_TYPE_EVGL,
                              _evgl_api_thread_glGetTexLevelParameterfv,
                              thread_data,
                              thread_mode);
}

/*
 * void
 * glBindVertexBuffer(GLuint bindingindex, GLuint buffer, GLintptr offset, GLsizei stride);
 */

typedef struct
{
   GLuint bindingindex;
   GLuint buffer;
   GLintptr offset;
   GLsizei stride;

} EVGL_API_Thread_Command_glBindVertexBuffer;

void (*orig_evgl_api_glBindVertexBuffer)(GLuint bindingindex, GLuint buffer, GLintptr offset, GLsizei stride);

static void
_evgl_api_thread_glBindVertexBuffer(void *data)
{
   EVGL_API_Thread_Command_glBindVertexBuffer *thread_data =
      (EVGL_API_Thread_Command_glBindVertexBuffer *)data;

   orig_evgl_api_glBindVertexBuffer(thread_data->bindingindex,
                                    thread_data->buffer,
                                    thread_data->offset,
                                    thread_data->stride);

}

EAPI void
glBindVertexBuffer_evgl_api_thread_cmd(GLuint bindingindex, GLuint buffer, GLintptr offset, GLsizei stride)
{
   if (!evas_evgl_thread_enabled() || (_main_thread_id != eina_thread_self()))
     {
        orig_evgl_api_glBindVertexBuffer(bindingindex, buffer, offset, stride);
        return;
     }

   int thread_mode = EVAS_GL_THREAD_MODE_FINISH;

   EVGL_API_Thread_Command_glBindVertexBuffer thread_data_local;
   EVGL_API_Thread_Command_glBindVertexBuffer *thread_data = &thread_data_local;

   thread_data->bindingindex = bindingindex;
   thread_data->buffer = buffer;
   thread_data->offset = offset;
   thread_data->stride = stride;

   evas_gl_thread_cmd_enqueue(EVAS_GL_THREAD_TYPE_EVGL,
                              _evgl_api_thread_glBindVertexBuffer,
                              thread_data,
                              thread_mode);
}

/*
 * void
 * glVertexAttribFormat(GLuint attribindex, GLint size, GLenum type, GLboolean normalized, GLuint relativeoffset);
 */

typedef struct
{
   GLuint attribindex;
   GLint size;
   GLenum type;
   GLboolean normalized;
   GLuint relativeoffset;

} EVGL_API_Thread_Command_glVertexAttribFormat;

void (*orig_evgl_api_glVertexAttribFormat)(GLuint attribindex, GLint size, GLenum type, GLboolean normalized, GLuint relativeoffset);

static void
_evgl_api_thread_glVertexAttribFormat(void *data)
{
   EVGL_API_Thread_Command_glVertexAttribFormat *thread_data =
      (EVGL_API_Thread_Command_glVertexAttribFormat *)data;

   orig_evgl_api_glVertexAttribFormat(thread_data->attribindex,
                                      thread_data->size,
                                      thread_data->type,
                                      thread_data->normalized,
                                      thread_data->relativeoffset);

}

EAPI void
glVertexAttribFormat_evgl_api_thread_cmd(GLuint attribindex, GLint size, GLenum type, GLboolean normalized, GLuint relativeoffset)
{
   if (!evas_evgl_thread_enabled() || (_main_thread_id != eina_thread_self()))
     {
        orig_evgl_api_glVertexAttribFormat(attribindex, size, type, normalized, relativeoffset);
        return;
     }

   int thread_mode = EVAS_GL_THREAD_MODE_FINISH;

   EVGL_API_Thread_Command_glVertexAttribFormat thread_data_local;
   EVGL_API_Thread_Command_glVertexAttribFormat *thread_data = &thread_data_local;

   thread_data->attribindex = attribindex;
   thread_data->size = size;
   thread_data->type = type;
   thread_data->normalized = normalized;
   thread_data->relativeoffset = relativeoffset;

   evas_gl_thread_cmd_enqueue(EVAS_GL_THREAD_TYPE_EVGL,
                              _evgl_api_thread_glVertexAttribFormat,
                              thread_data,
                              thread_mode);
}

/*
 * void
 * glVertexAttribIFormat(GLuint attribindex, GLint size, GLenum type, GLuint relativeoffset);
 */

typedef struct
{
   GLuint attribindex;
   GLint size;
   GLenum type;
   GLuint relativeoffset;

} EVGL_API_Thread_Command_glVertexAttribIFormat;

void (*orig_evgl_api_glVertexAttribIFormat)(GLuint attribindex, GLint size, GLenum type, GLuint relativeoffset);

static void
_evgl_api_thread_glVertexAttribIFormat(void *data)
{
   EVGL_API_Thread_Command_glVertexAttribIFormat *thread_data =
      (EVGL_API_Thread_Command_glVertexAttribIFormat *)data;

   orig_evgl_api_glVertexAttribIFormat(thread_data->attribindex,
                                       thread_data->size,
                                       thread_data->type,
                                       thread_data->relativeoffset);

}

EAPI void
glVertexAttribIFormat_evgl_api_thread_cmd(GLuint attribindex, GLint size, GLenum type, GLuint relativeoffset)
{
   if (!evas_evgl_thread_enabled() || (_main_thread_id != eina_thread_self()))
     {
        orig_evgl_api_glVertexAttribIFormat(attribindex, size, type, relativeoffset);
        return;
     }

   int thread_mode = EVAS_GL_THREAD_MODE_FINISH;

   EVGL_API_Thread_Command_glVertexAttribIFormat thread_data_local;
   EVGL_API_Thread_Command_glVertexAttribIFormat *thread_data = &thread_data_local;

   thread_data->attribindex = attribindex;
   thread_data->size = size;
   thread_data->type = type;
   thread_data->relativeoffset = relativeoffset;

   evas_gl_thread_cmd_enqueue(EVAS_GL_THREAD_TYPE_EVGL,
                              _evgl_api_thread_glVertexAttribIFormat,
                              thread_data,
                              thread_mode);
}

/*
 * void
 * glVertexAttribBinding(GLuint attribindex, GLuint bindingindex);
 */

typedef struct
{
   GLuint attribindex;
   GLuint bindingindex;

} EVGL_API_Thread_Command_glVertexAttribBinding;

void (*orig_evgl_api_glVertexAttribBinding)(GLuint attribindex, GLuint bindingindex);

static void
_evgl_api_thread_glVertexAttribBinding(void *data)
{
   EVGL_API_Thread_Command_glVertexAttribBinding *thread_data =
      (EVGL_API_Thread_Command_glVertexAttribBinding *)data;

   orig_evgl_api_glVertexAttribBinding(thread_data->attribindex,
                                       thread_data->bindingindex);

}

EAPI void
glVertexAttribBinding_evgl_api_thread_cmd(GLuint attribindex, GLuint bindingindex)
{
   if (!evas_evgl_thread_enabled() || (_main_thread_id != eina_thread_self()))
     {
        orig_evgl_api_glVertexAttribBinding(attribindex, bindingindex);
        return;
     }

   int thread_mode = EVAS_GL_THREAD_MODE_FINISH;

   EVGL_API_Thread_Command_glVertexAttribBinding thread_data_local;
   EVGL_API_Thread_Command_glVertexAttribBinding *thread_data = &thread_data_local;

   thread_data->attribindex = attribindex;
   thread_data->bindingindex = bindingindex;

   evas_gl_thread_cmd_enqueue(EVAS_GL_THREAD_TYPE_EVGL,
                              _evgl_api_thread_glVertexAttribBinding,
                              thread_data,
                              thread_mode);
}

/*
 * void
 * glVertexBindingDivisor(GLuint bindingindex, GLuint divisor);
 */

typedef struct
{
   GLuint bindingindex;
   GLuint divisor;

} EVGL_API_Thread_Command_glVertexBindingDivisor;

void (*orig_evgl_api_glVertexBindingDivisor)(GLuint bindingindex, GLuint divisor);

static void
_evgl_api_thread_glVertexBindingDivisor(void *data)
{
   EVGL_API_Thread_Command_glVertexBindingDivisor *thread_data =
      (EVGL_API_Thread_Command_glVertexBindingDivisor *)data;

   orig_evgl_api_glVertexBindingDivisor(thread_data->bindingindex,
                                        thread_data->divisor);

}

EAPI void
glVertexBindingDivisor_evgl_api_thread_cmd(GLuint bindingindex, GLuint divisor)
{
   if (!evas_evgl_thread_enabled() || (_main_thread_id != eina_thread_self()))
     {
        orig_evgl_api_glVertexBindingDivisor(bindingindex, divisor);
        return;
     }

   int thread_mode = EVAS_GL_THREAD_MODE_FINISH;

   EVGL_API_Thread_Command_glVertexBindingDivisor thread_data_local;
   EVGL_API_Thread_Command_glVertexBindingDivisor *thread_data = &thread_data_local;

   thread_data->bindingindex = bindingindex;
   thread_data->divisor = divisor;

   evas_gl_thread_cmd_enqueue(EVAS_GL_THREAD_TYPE_EVGL,
                              _evgl_api_thread_glVertexBindingDivisor,
                              thread_data,
                              thread_mode);
}

/*
 * void
 * glEGLImageTargetTexture2DOES(GLenum target, void *image);
 */

typedef struct
{
   GLenum target;
   void *image;

} EVGL_API_Thread_Command_glEGLImageTargetTexture2DOES;

void (*orig_evgl_api_glEGLImageTargetTexture2DOES)(GLenum target, void *image);

static void
_evgl_api_thread_glEGLImageTargetTexture2DOES(void *data)
{
   EVGL_API_Thread_Command_glEGLImageTargetTexture2DOES *thread_data =
      (EVGL_API_Thread_Command_glEGLImageTargetTexture2DOES *)data;

   orig_evgl_api_glEGLImageTargetTexture2DOES(thread_data->target,
                                              thread_data->image);

}

EAPI void
glEGLImageTargetTexture2DOES_evgl_api_thread_cmd(GLenum target, void *image)
{
   if (!evas_evgl_thread_enabled() || (_main_thread_id != eina_thread_self()))
     {
        orig_evgl_api_glEGLImageTargetTexture2DOES(target, image);
        return;
     }

   int thread_mode = EVAS_GL_THREAD_MODE_FINISH;

   EVGL_API_Thread_Command_glEGLImageTargetTexture2DOES thread_data_local;
   EVGL_API_Thread_Command_glEGLImageTargetTexture2DOES *thread_data = &thread_data_local;

   thread_data->target = target;
   thread_data->image = image;

   evas_gl_thread_cmd_enqueue(EVAS_GL_THREAD_TYPE_EVGL,
                              _evgl_api_thread_glEGLImageTargetTexture2DOES,
                              thread_data,
                              thread_mode);
}

/*
 * void
 * glEGLImageTargetRenderbufferStorageOES(GLenum target, void *image);
 */

typedef struct
{
   GLenum target;
   void *image;
   int command_allocated;

} EVGL_API_Thread_Command_glEGLImageTargetRenderbufferStorageOES;

void (*orig_evgl_api_glEGLImageTargetRenderbufferStorageOES)(GLenum target, void *image);

static void
_evgl_api_thread_glEGLImageTargetRenderbufferStorageOES(void *data)
{
   EVGL_API_Thread_Command_glEGLImageTargetRenderbufferStorageOES *thread_data =
      (EVGL_API_Thread_Command_glEGLImageTargetRenderbufferStorageOES *)data;

   orig_evgl_api_glEGLImageTargetRenderbufferStorageOES(thread_data->target,
                                                        thread_data->image);

   if (thread_data->command_allocated)
     eina_mempool_free(_mp_command, thread_data);
}

EAPI void
glEGLImageTargetRenderbufferStorageOES_evgl_api_thread_cmd(GLenum target, void *image)
{
   if (!evas_evgl_thread_enabled() || (_main_thread_id != eina_thread_self()))
     {
        orig_evgl_api_glEGLImageTargetRenderbufferStorageOES(target, image);
        return;
     }

   int thread_mode = EVAS_GL_THREAD_MODE_FINISH;

   EVGL_API_Thread_Command_glEGLImageTargetRenderbufferStorageOES thread_data_local;
   EVGL_API_Thread_Command_glEGLImageTargetRenderbufferStorageOES *thread_data = &thread_data_local;

   /* command_allocated flag init. */
   thread_data->command_allocated = 0;

   if (!evas_gl_thread_force_finish())
     { /* _flush */
        EVGL_API_Thread_Command_glEGLImageTargetRenderbufferStorageOES *thread_data_new;
        thread_data_new = eina_mempool_malloc(_mp_command,
                                              sizeof(EVGL_API_Thread_Command_glEGLImageTargetRenderbufferStorageOES));
        if (thread_data_new)
          {
             thread_data = thread_data_new;
             thread_data->command_allocated = 1;
             thread_mode = EVAS_GL_THREAD_MODE_FLUSH;
          }
     }

   thread_data->target = target;
   thread_data->image = image;

   evas_gl_thread_cmd_enqueue(EVAS_GL_THREAD_TYPE_EVGL,
                              _evgl_api_thread_glEGLImageTargetRenderbufferStorageOES,
                              thread_data,
                              thread_mode);
}

/*
 * void
 * _evgl_glDiscardFramebufferEXT(GLenum target, GLsizei numAttachments, const GLenum* attachments);
 */

typedef struct
{
   GLenum target;
   GLsizei numAttachments;
   const GLenum* attachments;

} EVGL_API_Thread_Command__evgl_glDiscardFramebufferEXT;

void (*orig_evgl_api__evgl_glDiscardFramebufferEXT)(GLenum target, GLsizei numAttachments, const GLenum* attachments);

static void
_evgl_api_thread__evgl_glDiscardFramebufferEXT(void *data)
{
   EVGL_API_Thread_Command__evgl_glDiscardFramebufferEXT *thread_data =
      (EVGL_API_Thread_Command__evgl_glDiscardFramebufferEXT *)data;

   orig_evgl_api__evgl_glDiscardFramebufferEXT(thread_data->target,
                                               thread_data->numAttachments,
                                               thread_data->attachments);

}

EAPI void
_evgl_glDiscardFramebufferEXT_evgl_api_thread_cmd(GLenum target, GLsizei numAttachments, const GLenum* attachments)
{
   if (!evas_evgl_thread_enabled() || (_main_thread_id != eina_thread_self()))
     {
        orig_evgl_api__evgl_glDiscardFramebufferEXT(target, numAttachments, attachments);
        return;
     }

   int thread_mode = EVAS_GL_THREAD_MODE_FINISH;

   EVGL_API_Thread_Command__evgl_glDiscardFramebufferEXT thread_data_local;
   EVGL_API_Thread_Command__evgl_glDiscardFramebufferEXT *thread_data = &thread_data_local;

   thread_data->target = target;
   thread_data->numAttachments = numAttachments;
   thread_data->attachments = attachments;

   evas_gl_thread_cmd_enqueue(EVAS_GL_THREAD_TYPE_EVGL,
                              _evgl_api_thread__evgl_glDiscardFramebufferEXT,
                              thread_data,
                              thread_mode);
}

/*
 * void
 * _evgl_glEvasGLImageTargetTexture2D(GLenum target, EvasGLImage image);
 */

typedef struct
{
   GLenum target;
   EvasGLImage image;

} EVGL_API_Thread_Command__evgl_glEvasGLImageTargetTexture2D;

void (*orig_evgl_api__evgl_glEvasGLImageTargetTexture2D)(GLenum target, EvasGLImage image);

static void
_evgl_api_thread__evgl_glEvasGLImageTargetTexture2D(void *data)
{
   EVGL_API_Thread_Command__evgl_glEvasGLImageTargetTexture2D *thread_data =
      (EVGL_API_Thread_Command__evgl_glEvasGLImageTargetTexture2D *)data;

   orig_evgl_api__evgl_glEvasGLImageTargetTexture2D(thread_data->target,
                                                    thread_data->image);

}

EAPI void
_evgl_glEvasGLImageTargetTexture2D_evgl_api_thread_cmd(GLenum target, EvasGLImage image)
{
   if (!evas_evgl_thread_enabled() || (_main_thread_id != eina_thread_self()))
     {
        orig_evgl_api__evgl_glEvasGLImageTargetTexture2D(target, image);
        return;
     }

   int thread_mode = EVAS_GL_THREAD_MODE_FINISH;

   EVGL_API_Thread_Command__evgl_glEvasGLImageTargetTexture2D thread_data_local;
   EVGL_API_Thread_Command__evgl_glEvasGLImageTargetTexture2D *thread_data = &thread_data_local;

   thread_data->target = target;
   thread_data->image = image;

   evas_gl_thread_cmd_enqueue(EVAS_GL_THREAD_TYPE_EVGL,
                              _evgl_api_thread__evgl_glEvasGLImageTargetTexture2D,
                              thread_data,
                              thread_mode);
}

/*
 * void
 * _evgl_glEvasGLImageTargetRenderbufferStorage(GLenum target, EvasGLImage image);
 */

typedef struct
{
   GLenum target;
   EvasGLImage image;

} EVGL_API_Thread_Command__evgl_glEvasGLImageTargetRenderbufferStorage;

void (*orig_evgl_api__evgl_glEvasGLImageTargetRenderbufferStorage)(GLenum target, EvasGLImage image);

static void
_evgl_api_thread__evgl_glEvasGLImageTargetRenderbufferStorage(void *data)
{
   EVGL_API_Thread_Command__evgl_glEvasGLImageTargetRenderbufferStorage *thread_data =
      (EVGL_API_Thread_Command__evgl_glEvasGLImageTargetRenderbufferStorage *)data;

   orig_evgl_api__evgl_glEvasGLImageTargetRenderbufferStorage(thread_data->target,
                                                              thread_data->image);

}

EAPI void
_evgl_glEvasGLImageTargetRenderbufferStorage_evgl_api_thread_cmd(GLenum target, EvasGLImage image)
{
   if (!evas_evgl_thread_enabled() || (_main_thread_id != eina_thread_self()))
     {
        orig_evgl_api__evgl_glEvasGLImageTargetRenderbufferStorage(target, image);
        return;
     }

   int thread_mode = EVAS_GL_THREAD_MODE_FINISH;

   EVGL_API_Thread_Command__evgl_glEvasGLImageTargetRenderbufferStorage thread_data_local;
   EVGL_API_Thread_Command__evgl_glEvasGLImageTargetRenderbufferStorage *thread_data = &thread_data_local;

   thread_data->target = target;
   thread_data->image = image;

   evas_gl_thread_cmd_enqueue(EVAS_GL_THREAD_TYPE_EVGL,
                              _evgl_api_thread__evgl_glEvasGLImageTargetRenderbufferStorage,
                              thread_data,
                              thread_mode);
}

/*
 * EvasGLImage
 * _evgl_evasglCreateImage(int target, void* buffer, const int *attrib_list);
 */

typedef struct
{
   EvasGLImage return_value;
   int target;
   void* buffer;
   const int *attrib_list;

} EVGL_API_Thread_Command__evgl_evasglCreateImage;

EvasGLImage (*orig_evgl_api__evgl_evasglCreateImage)(int target, void* buffer, const int *attrib_list);

static void
_evgl_api_thread__evgl_evasglCreateImage(void *data)
{
   EVGL_API_Thread_Command__evgl_evasglCreateImage *thread_data =
      (EVGL_API_Thread_Command__evgl_evasglCreateImage *)data;

   thread_data->return_value = orig_evgl_api__evgl_evasglCreateImage(thread_data->target,
                                                                     thread_data->buffer,
                                                                     thread_data->attrib_list);

}

EAPI EvasGLImage
_evgl_evasglCreateImage_evgl_api_thread_cmd(int target, void* buffer, const int *attrib_list)
{
   if (!evas_evgl_thread_enabled() || (_main_thread_id != eina_thread_self()))
     {
        return orig_evgl_api__evgl_evasglCreateImage(target, buffer, attrib_list);
     }

   int thread_mode = EVAS_GL_THREAD_MODE_FINISH;

   EVGL_API_Thread_Command__evgl_evasglCreateImage thread_data_local;
   EVGL_API_Thread_Command__evgl_evasglCreateImage *thread_data = &thread_data_local;

   thread_data->target = target;
   thread_data->buffer = buffer;
   thread_data->attrib_list = attrib_list;

   evas_gl_thread_cmd_enqueue(EVAS_GL_THREAD_TYPE_EVGL,
                              _evgl_api_thread__evgl_evasglCreateImage,
                              thread_data,
                              thread_mode);

   return thread_data->return_value;
}

/*
 * void
 * _evgl_evasglDestroyImage(EvasGLImage image);
 */

typedef struct
{
   EvasGLImage image;

} EVGL_API_Thread_Command__evgl_evasglDestroyImage;

void (*orig_evgl_api__evgl_evasglDestroyImage)(EvasGLImage image);

static void
_evgl_api_thread__evgl_evasglDestroyImage(void *data)
{
   EVGL_API_Thread_Command__evgl_evasglDestroyImage *thread_data =
      (EVGL_API_Thread_Command__evgl_evasglDestroyImage *)data;

   orig_evgl_api__evgl_evasglDestroyImage(thread_data->image);

}

EAPI void
_evgl_evasglDestroyImage_evgl_api_thread_cmd(EvasGLImage image)
{
   if (!evas_evgl_thread_enabled() || (_main_thread_id != eina_thread_self()))
     {
        orig_evgl_api__evgl_evasglDestroyImage(image);
        return;
     }

   int thread_mode = EVAS_GL_THREAD_MODE_FINISH;

   EVGL_API_Thread_Command__evgl_evasglDestroyImage thread_data_local;
   EVGL_API_Thread_Command__evgl_evasglDestroyImage *thread_data = &thread_data_local;

   thread_data->image = image;

   evas_gl_thread_cmd_enqueue(EVAS_GL_THREAD_TYPE_EVGL,
                              _evgl_api_thread__evgl_evasglDestroyImage,
                              thread_data,
                              thread_mode);
}

/*
 * EvasGLImage
 * _evgl_evasglCreateImageForContext(Evas_GL *evas_gl, Evas_GL_Context *ctx, int target, void* buffer, const int *attrib_list);
 */

typedef struct
{
   EvasGLImage return_value;
   Evas_GL *evas_gl;
   Evas_GL_Context *ctx;
   int target;
   void* buffer;
   const int *attrib_list;

} EVGL_API_Thread_Command__evgl_evasglCreateImageForContext;

EvasGLImage (*orig_evgl_api__evgl_evasglCreateImageForContext)(Evas_GL *evas_gl, Evas_GL_Context *ctx, int target, void* buffer, const int *attrib_list);

static void
_evgl_api_thread__evgl_evasglCreateImageForContext(void *data)
{
   EVGL_API_Thread_Command__evgl_evasglCreateImageForContext *thread_data =
      (EVGL_API_Thread_Command__evgl_evasglCreateImageForContext *)data;

   thread_data->return_value = orig_evgl_api__evgl_evasglCreateImageForContext(thread_data->evas_gl,
                                                                               thread_data->ctx,
                                                                               thread_data->target,
                                                                               thread_data->buffer,
                                                                               thread_data->attrib_list);

}

EAPI EvasGLImage
_evgl_evasglCreateImageForContext_evgl_api_thread_cmd(Evas_GL *evas_gl, Evas_GL_Context *ctx, int target, void* buffer, const int *attrib_list)
{
   if (!evas_evgl_thread_enabled() || (_main_thread_id != eina_thread_self()))
     {
        return orig_evgl_api__evgl_evasglCreateImageForContext(evas_gl, ctx, target, buffer, attrib_list);
     }

   int thread_mode = EVAS_GL_THREAD_MODE_FINISH;

   EVGL_API_Thread_Command__evgl_evasglCreateImageForContext thread_data_local;
   EVGL_API_Thread_Command__evgl_evasglCreateImageForContext *thread_data = &thread_data_local;

   thread_data->evas_gl = evas_gl;
   thread_data->ctx = ctx;
   thread_data->target = target;
   thread_data->buffer = buffer;
   thread_data->attrib_list = attrib_list;

   evas_gl_thread_cmd_enqueue(EVAS_GL_THREAD_TYPE_EVGL,
                              _evgl_api_thread__evgl_evasglCreateImageForContext,
                              thread_data,
                              thread_mode);

   return thread_data->return_value;
}

/*
 * EvasGLSync
 * _evgl_evasglCreateSync(Evas_GL *evas_gl, unsigned int type, const int *attrib_list);
 */

typedef struct
{
   EvasGLSync return_value;
   Evas_GL *evas_gl;
   unsigned int type;
   const int *attrib_list;

} EVGL_API_Thread_Command__evgl_evasglCreateSync;

EvasGLSync (*orig_evgl_api__evgl_evasglCreateSync)(Evas_GL *evas_gl, unsigned int type, const int *attrib_list);

static void
_evgl_api_thread__evgl_evasglCreateSync(void *data)
{
   EVGL_API_Thread_Command__evgl_evasglCreateSync *thread_data =
      (EVGL_API_Thread_Command__evgl_evasglCreateSync *)data;

   thread_data->return_value = orig_evgl_api__evgl_evasglCreateSync(thread_data->evas_gl,
                                                                    thread_data->type,
                                                                    thread_data->attrib_list);

}

EAPI EvasGLSync
_evgl_evasglCreateSync_evgl_api_thread_cmd(Evas_GL *evas_gl, unsigned int type, const int *attrib_list)
{
   if (!evas_evgl_thread_enabled() || (_main_thread_id != eina_thread_self()))
     {
        return orig_evgl_api__evgl_evasglCreateSync(evas_gl, type, attrib_list);
     }

   int thread_mode = EVAS_GL_THREAD_MODE_FINISH;

   EVGL_API_Thread_Command__evgl_evasglCreateSync thread_data_local;
   EVGL_API_Thread_Command__evgl_evasglCreateSync *thread_data = &thread_data_local;

   thread_data->evas_gl = evas_gl;
   thread_data->type = type;
   thread_data->attrib_list = attrib_list;

   evas_gl_thread_cmd_enqueue(EVAS_GL_THREAD_TYPE_EVGL,
                              _evgl_api_thread__evgl_evasglCreateSync,
                              thread_data,
                              thread_mode);

   return thread_data->return_value;
}

/*
 * Eina_Bool
 * _evgl_evasglDestroySync(Evas_GL *evas_gl, EvasGLSync sync);
 */

typedef struct
{
   Eina_Bool return_value;
   Evas_GL *evas_gl;
   EvasGLSync sync;

} EVGL_API_Thread_Command__evgl_evasglDestroySync;

Eina_Bool (*orig_evgl_api__evgl_evasglDestroySync)(Evas_GL *evas_gl, EvasGLSync sync);

static void
_evgl_api_thread__evgl_evasglDestroySync(void *data)
{
   EVGL_API_Thread_Command__evgl_evasglDestroySync *thread_data =
      (EVGL_API_Thread_Command__evgl_evasglDestroySync *)data;

   thread_data->return_value = orig_evgl_api__evgl_evasglDestroySync(thread_data->evas_gl,
                                                                     thread_data->sync);

}

EAPI Eina_Bool
_evgl_evasglDestroySync_evgl_api_thread_cmd(Evas_GL *evas_gl, EvasGLSync sync)
{
   if (!evas_evgl_thread_enabled() || (_main_thread_id != eina_thread_self()))
     {
        return orig_evgl_api__evgl_evasglDestroySync(evas_gl, sync);
     }

   int thread_mode = EVAS_GL_THREAD_MODE_FINISH;

   EVGL_API_Thread_Command__evgl_evasglDestroySync thread_data_local;
   EVGL_API_Thread_Command__evgl_evasglDestroySync *thread_data = &thread_data_local;

   thread_data->evas_gl = evas_gl;
   thread_data->sync = sync;

   evas_gl_thread_cmd_enqueue(EVAS_GL_THREAD_TYPE_EVGL,
                              _evgl_api_thread__evgl_evasglDestroySync,
                              thread_data,
                              thread_mode);

   return thread_data->return_value;
}

/*
 * int
 * _evgl_evasglClientWaitSync(Evas_GL *evas_gl, EvasGLSync sync);
 */

typedef struct
{
   int return_value;
   Evas_GL *evas_gl;
   EvasGLSync sync;

} EVGL_API_Thread_Command__evgl_evasglClientWaitSync;

int (*orig_evgl_api__evgl_evasglClientWaitSync)(Evas_GL *evas_gl, EvasGLSync sync);

static void
_evgl_api_thread__evgl_evasglClientWaitSync(void *data)
{
   EVGL_API_Thread_Command__evgl_evasglClientWaitSync *thread_data =
      (EVGL_API_Thread_Command__evgl_evasglClientWaitSync *)data;

   thread_data->return_value = orig_evgl_api__evgl_evasglClientWaitSync(thread_data->evas_gl,
                                                                        thread_data->sync);

}

EAPI int
_evgl_evasglClientWaitSync_evgl_api_thread_cmd(Evas_GL *evas_gl, EvasGLSync sync)
{
   if (!evas_evgl_thread_enabled() || (_main_thread_id != eina_thread_self()))
     {
        return orig_evgl_api__evgl_evasglClientWaitSync(evas_gl, sync);
     }

   int thread_mode = EVAS_GL_THREAD_MODE_FINISH;

   EVGL_API_Thread_Command__evgl_evasglClientWaitSync thread_data_local;
   EVGL_API_Thread_Command__evgl_evasglClientWaitSync *thread_data = &thread_data_local;

   thread_data->evas_gl = evas_gl;
   thread_data->sync = sync;

   evas_gl_thread_cmd_enqueue(EVAS_GL_THREAD_TYPE_EVGL,
                              _evgl_api_thread__evgl_evasglClientWaitSync,
                              thread_data,
                              thread_mode);

   return thread_data->return_value;
}

/*
 * Eina_Bool
 * _evgl_evasglGetSyncAttrib(Evas_GL *evas_gl, EvasGLSync sync, int attribute, int *value);
 */

typedef struct
{
   Eina_Bool return_value;
   Evas_GL *evas_gl;
   EvasGLSync sync;
   int attribute;
   int *value;

} EVGL_API_Thread_Command__evgl_evasglGetSyncAttrib;

Eina_Bool (*orig_evgl_api__evgl_evasglGetSyncAttrib)(Evas_GL *evas_gl, EvasGLSync sync, int attribute, int *value);

static void
_evgl_api_thread__evgl_evasglGetSyncAttrib(void *data)
{
   EVGL_API_Thread_Command__evgl_evasglGetSyncAttrib *thread_data =
      (EVGL_API_Thread_Command__evgl_evasglGetSyncAttrib *)data;

   thread_data->return_value = orig_evgl_api__evgl_evasglGetSyncAttrib(thread_data->evas_gl,
                                                                       thread_data->sync,
                                                                       thread_data->attribute,
                                                                       thread_data->value);

}

EAPI Eina_Bool
_evgl_evasglGetSyncAttrib_evgl_api_thread_cmd(Evas_GL *evas_gl, EvasGLSync sync, int attribute, int *value)
{
   if (!evas_evgl_thread_enabled() || (_main_thread_id != eina_thread_self()))
     {
        return orig_evgl_api__evgl_evasglGetSyncAttrib(evas_gl, sync, attribute, value);
     }

   int thread_mode = EVAS_GL_THREAD_MODE_FINISH;

   EVGL_API_Thread_Command__evgl_evasglGetSyncAttrib thread_data_local;
   EVGL_API_Thread_Command__evgl_evasglGetSyncAttrib *thread_data = &thread_data_local;

   thread_data->evas_gl = evas_gl;
   thread_data->sync = sync;
   thread_data->attribute = attribute;
   thread_data->value = value;

   evas_gl_thread_cmd_enqueue(EVAS_GL_THREAD_TYPE_EVGL,
                              _evgl_api_thread__evgl_evasglGetSyncAttrib,
                              thread_data,
                              thread_mode);

   return thread_data->return_value;
}

/*
 * Eina_Bool
 * _evgl_evasglSignalSync(Evas_GL *evas_gl, EvasGLSync sync, unsigned mode);
 */

typedef struct
{
   Eina_Bool return_value;
   Evas_GL *evas_gl;
   EvasGLSync sync;
   unsigned mode;

} EVGL_API_Thread_Command__evgl_evasglSignalSync;

Eina_Bool (*orig_evgl_api__evgl_evasglSignalSync)(Evas_GL *evas_gl, EvasGLSync sync, unsigned mode);

static void
_evgl_api_thread__evgl_evasglSignalSync(void *data)
{
   EVGL_API_Thread_Command__evgl_evasglSignalSync *thread_data =
      (EVGL_API_Thread_Command__evgl_evasglSignalSync *)data;

   thread_data->return_value = orig_evgl_api__evgl_evasglSignalSync(thread_data->evas_gl,
                                                                    thread_data->sync,
                                                                    thread_data->mode);

}

EAPI Eina_Bool
_evgl_evasglSignalSync_evgl_api_thread_cmd(Evas_GL *evas_gl, EvasGLSync sync, unsigned mode)
{
   if (!evas_evgl_thread_enabled() || (_main_thread_id != eina_thread_self()))
     {
        return orig_evgl_api__evgl_evasglSignalSync(evas_gl, sync, mode);
     }

   int thread_mode = EVAS_GL_THREAD_MODE_FINISH;

   EVGL_API_Thread_Command__evgl_evasglSignalSync thread_data_local;
   EVGL_API_Thread_Command__evgl_evasglSignalSync *thread_data = &thread_data_local;

   thread_data->evas_gl = evas_gl;
   thread_data->sync = sync;
   thread_data->mode = mode;

   evas_gl_thread_cmd_enqueue(EVAS_GL_THREAD_TYPE_EVGL,
                              _evgl_api_thread__evgl_evasglSignalSync,
                              thread_data,
                              thread_mode);

   return thread_data->return_value;
}

/*
 * int
 * _evgl_evasglWaitSync(Evas_GL *evas_gl, EvasGLSync sync, int flags);
 */

typedef struct
{
   int return_value;
   Evas_GL *evas_gl;
   EvasGLSync sync;
   int flags;

} EVGL_API_Thread_Command__evgl_evasglWaitSync;

int (*orig_evgl_api__evgl_evasglWaitSync)(Evas_GL *evas_gl, EvasGLSync sync, int flags);

static void
_evgl_api_thread__evgl_evasglWaitSync(void *data)
{
   EVGL_API_Thread_Command__evgl_evasglWaitSync *thread_data =
      (EVGL_API_Thread_Command__evgl_evasglWaitSync *)data;

   thread_data->return_value = orig_evgl_api__evgl_evasglWaitSync(thread_data->evas_gl,
                                                                  thread_data->sync,
                                                                  thread_data->flags);

}

EAPI int
_evgl_evasglWaitSync_evgl_api_thread_cmd(Evas_GL *evas_gl, EvasGLSync sync, int flags)
{
   if (!evas_evgl_thread_enabled() || (_main_thread_id != eina_thread_self()))
     {
        return orig_evgl_api__evgl_evasglWaitSync(evas_gl, sync, flags);
     }

   int thread_mode = EVAS_GL_THREAD_MODE_FINISH;

   EVGL_API_Thread_Command__evgl_evasglWaitSync thread_data_local;
   EVGL_API_Thread_Command__evgl_evasglWaitSync *thread_data = &thread_data_local;

   thread_data->evas_gl = evas_gl;
   thread_data->sync = sync;
   thread_data->flags = flags;

   evas_gl_thread_cmd_enqueue(EVAS_GL_THREAD_TYPE_EVGL,
                              _evgl_api_thread__evgl_evasglWaitSync,
                              thread_data,
                              thread_mode);

   return thread_data->return_value;
}

/*
 * Eina_Bool
 * _evgl_evasglBindWaylandDisplay(Evas_GL *evas_gl, void *wl_display);
 */

typedef struct
{
   Eina_Bool return_value;
   Evas_GL *evas_gl;
   void *wl_display;

} EVGL_API_Thread_Command__evgl_evasglBindWaylandDisplay;

Eina_Bool (*orig_evgl_api__evgl_evasglBindWaylandDisplay)(Evas_GL *evas_gl, void *wl_display);

static void
_evgl_api_thread__evgl_evasglBindWaylandDisplay(void *data)
{
   EVGL_API_Thread_Command__evgl_evasglBindWaylandDisplay *thread_data =
      (EVGL_API_Thread_Command__evgl_evasglBindWaylandDisplay *)data;

   thread_data->return_value = orig_evgl_api__evgl_evasglBindWaylandDisplay(thread_data->evas_gl,
                                                                            thread_data->wl_display);

}

EAPI Eina_Bool
_evgl_evasglBindWaylandDisplay_evgl_api_thread_cmd(Evas_GL *evas_gl, void *wl_display)
{
   if (!evas_evgl_thread_enabled() || (_main_thread_id != eina_thread_self()))
     {
        return orig_evgl_api__evgl_evasglBindWaylandDisplay(evas_gl, wl_display);
     }

   int thread_mode = EVAS_GL_THREAD_MODE_FINISH;

   EVGL_API_Thread_Command__evgl_evasglBindWaylandDisplay thread_data_local;
   EVGL_API_Thread_Command__evgl_evasglBindWaylandDisplay *thread_data = &thread_data_local;

   thread_data->evas_gl = evas_gl;
   thread_data->wl_display = wl_display;

   evas_gl_thread_cmd_enqueue(EVAS_GL_THREAD_TYPE_EVGL,
                              _evgl_api_thread__evgl_evasglBindWaylandDisplay,
                              thread_data,
                              thread_mode);

   return thread_data->return_value;
}

/*
 * Eina_Bool
 * _evgl_evasglUnbindWaylandDisplay(Evas_GL *evas_gl, void *wl_display);
 */

typedef struct
{
   Eina_Bool return_value;
   Evas_GL *evas_gl;
   void *wl_display;

} EVGL_API_Thread_Command__evgl_evasglUnbindWaylandDisplay;

Eina_Bool (*orig_evgl_api__evgl_evasglUnbindWaylandDisplay)(Evas_GL *evas_gl, void *wl_display);

static void
_evgl_api_thread__evgl_evasglUnbindWaylandDisplay(void *data)
{
   EVGL_API_Thread_Command__evgl_evasglUnbindWaylandDisplay *thread_data =
      (EVGL_API_Thread_Command__evgl_evasglUnbindWaylandDisplay *)data;

   thread_data->return_value = orig_evgl_api__evgl_evasglUnbindWaylandDisplay(thread_data->evas_gl,
                                                                              thread_data->wl_display);

}

EAPI Eina_Bool
_evgl_evasglUnbindWaylandDisplay_evgl_api_thread_cmd(Evas_GL *evas_gl, void *wl_display)
{
   if (!evas_evgl_thread_enabled() || (_main_thread_id != eina_thread_self()))
     {
        return orig_evgl_api__evgl_evasglUnbindWaylandDisplay(evas_gl, wl_display);
     }

   int thread_mode = EVAS_GL_THREAD_MODE_FINISH;

   EVGL_API_Thread_Command__evgl_evasglUnbindWaylandDisplay thread_data_local;
   EVGL_API_Thread_Command__evgl_evasglUnbindWaylandDisplay *thread_data = &thread_data_local;

   thread_data->evas_gl = evas_gl;
   thread_data->wl_display = wl_display;

   evas_gl_thread_cmd_enqueue(EVAS_GL_THREAD_TYPE_EVGL,
                              _evgl_api_thread__evgl_evasglUnbindWaylandDisplay,
                              thread_data,
                              thread_mode);

   return thread_data->return_value;
}

/*
 * Eina_Bool
 * _evgl_evasglQueryWaylandBuffer(Evas_GL *evas_gl, void *buffer, int attribute, int *value);
 */

typedef struct
{
   Eina_Bool return_value;
   Evas_GL *evas_gl;
   void *buffer;
   int attribute;
   int *value;

} EVGL_API_Thread_Command__evgl_evasglQueryWaylandBuffer;

Eina_Bool (*orig_evgl_api__evgl_evasglQueryWaylandBuffer)(Evas_GL *evas_gl, void *buffer, int attribute, int *value);

static void
_evgl_api_thread__evgl_evasglQueryWaylandBuffer(void *data)
{
   EVGL_API_Thread_Command__evgl_evasglQueryWaylandBuffer *thread_data =
      (EVGL_API_Thread_Command__evgl_evasglQueryWaylandBuffer *)data;

   thread_data->return_value = orig_evgl_api__evgl_evasglQueryWaylandBuffer(thread_data->evas_gl,
                                                                            thread_data->buffer,
                                                                            thread_data->attribute,
                                                                            thread_data->value);

}

EAPI Eina_Bool
_evgl_evasglQueryWaylandBuffer_evgl_api_thread_cmd(Evas_GL *evas_gl, void *buffer, int attribute, int *value)
{
   if (!evas_evgl_thread_enabled() || (_main_thread_id != eina_thread_self()))
     {
        return orig_evgl_api__evgl_evasglQueryWaylandBuffer(evas_gl, buffer, attribute, value);
     }

   int thread_mode = EVAS_GL_THREAD_MODE_FINISH;

   EVGL_API_Thread_Command__evgl_evasglQueryWaylandBuffer thread_data_local;
   EVGL_API_Thread_Command__evgl_evasglQueryWaylandBuffer *thread_data = &thread_data_local;

   thread_data->evas_gl = evas_gl;
   thread_data->buffer = buffer;
   thread_data->attribute = attribute;
   thread_data->value = value;

   evas_gl_thread_cmd_enqueue(EVAS_GL_THREAD_TYPE_EVGL,
                              _evgl_api_thread__evgl_evasglQueryWaylandBuffer,
                              thread_data,
                              thread_mode);

   return thread_data->return_value;
}
