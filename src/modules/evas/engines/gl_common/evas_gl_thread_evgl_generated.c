/* DO NOT MODIFY THIS FILE AS IT IS AUTO-GENERATED */


/*
   GLenum
   glGetError(void);
 */

typedef struct
{
   GLenum return_value;

} EVGL_Thread_Command_glGetError;

static void
_evgl_thread_glGetError(void *data)
{
   EVGL_Thread_Command_glGetError *thread_param =
      (EVGL_Thread_Command_glGetError *)data;

   thread_param->return_value = glGetError();

}

EAPI GLenum
glGetError_evgl_thread_cmd(void)
{
   if (!evas_evgl_thread_enabled())
     {
        return glGetError();
     }

   int thread_mode = EVAS_GL_THREAD_MODE_FINISH;

   EVGL_Thread_Command_glGetError thread_param_local;
   EVGL_Thread_Command_glGetError *thread_param = &thread_param_local;


   evas_gl_thread_cmd_enqueue(EVAS_GL_THREAD_TYPE_EVGL,
                              _evgl_thread_glGetError,
                              thread_param,
                              thread_mode);

   return thread_param->return_value;
}

/*
   void
   glVertexAttribPointer(GLuint index, GLint size, GLenum type, GLboolean normalized, GLsizei stride, const void *pointer);
 */

typedef struct
{
   GLuint index;
   GLint size;
   GLenum type;
   GLboolean normalized;
   GLsizei stride;
   const void *pointer;

} EVGL_Thread_Command_glVertexAttribPointer;

static void
_evgl_thread_glVertexAttribPointer(void *data)
{
   EVGL_Thread_Command_glVertexAttribPointer *thread_param =
      (EVGL_Thread_Command_glVertexAttribPointer *)data;

   glVertexAttribPointer(thread_param->index,
                         thread_param->size,
                         thread_param->type,
                         thread_param->normalized,
                         thread_param->stride,
                         thread_param->pointer);

}

EAPI void
glVertexAttribPointer_evgl_thread_cmd(GLuint index, GLint size, GLenum type, GLboolean normalized, GLsizei stride, const void *pointer)
{
   if (!evas_evgl_thread_enabled())
     {
        glVertexAttribPointer(index, size, type, normalized, stride, pointer);
        return;
     }

   int thread_mode = EVAS_GL_THREAD_MODE_FINISH;

   EVGL_Thread_Command_glVertexAttribPointer thread_param_local;
   EVGL_Thread_Command_glVertexAttribPointer *thread_param = &thread_param_local;

   thread_param->index = index;
   thread_param->size = size;
   thread_param->type = type;
   thread_param->normalized = normalized;
   thread_param->stride = stride;
   thread_param->pointer = pointer;

   evas_gl_thread_cmd_enqueue(EVAS_GL_THREAD_TYPE_EVGL,
                              _evgl_thread_glVertexAttribPointer,
                              thread_param,
                              thread_mode);
}

/*
   void
   glEnableVertexAttribArray(GLuint index);
 */

typedef struct
{
   GLuint index;
   int command_allocated;

} EVGL_Thread_Command_glEnableVertexAttribArray;

static void
_evgl_thread_glEnableVertexAttribArray(void *data)
{
   EVGL_Thread_Command_glEnableVertexAttribArray *thread_param =
      (EVGL_Thread_Command_glEnableVertexAttribArray *)data;

   glEnableVertexAttribArray(thread_param->index);

   if (thread_param->command_allocated)
     eina_mempool_free(_mp_command, thread_param);
}

EAPI void
glEnableVertexAttribArray_evgl_thread_cmd(GLuint index)
{
   if (!evas_evgl_thread_enabled())
     {
        glEnableVertexAttribArray(index);
        return;
     }

   int thread_mode = EVAS_GL_THREAD_MODE_FINISH;

   EVGL_Thread_Command_glEnableVertexAttribArray thread_param_local;
   EVGL_Thread_Command_glEnableVertexAttribArray *thread_param = &thread_param_local;

   /* command_allocated flag init. */
   thread_param->command_allocated = 0;

   if (!evas_gl_thread_force_finish())
     { /* _flush */
        EVGL_Thread_Command_glEnableVertexAttribArray *thread_param_new;
        thread_param_new = eina_mempool_malloc(_mp_command,
                                               sizeof(EVGL_Thread_Command_glEnableVertexAttribArray));
        if (thread_param_new)
          {
             thread_param = thread_param_new;
             thread_param->command_allocated = 1;
             thread_mode = EVAS_GL_THREAD_MODE_FLUSH;
          }
     }

   thread_param->index = index;

   evas_gl_thread_cmd_enqueue(EVAS_GL_THREAD_TYPE_EVGL,
                              _evgl_thread_glEnableVertexAttribArray,
                              thread_param,
                              thread_mode);
}

/*
   void
   glDisableVertexAttribArray(GLuint index);
 */

typedef struct
{
   GLuint index;
   int command_allocated;

} EVGL_Thread_Command_glDisableVertexAttribArray;

static void
_evgl_thread_glDisableVertexAttribArray(void *data)
{
   EVGL_Thread_Command_glDisableVertexAttribArray *thread_param =
      (EVGL_Thread_Command_glDisableVertexAttribArray *)data;

   glDisableVertexAttribArray(thread_param->index);

   if (thread_param->command_allocated)
     eina_mempool_free(_mp_command, thread_param);
}

EAPI void
glDisableVertexAttribArray_evgl_thread_cmd(GLuint index)
{
   if (!evas_evgl_thread_enabled())
     {
        glDisableVertexAttribArray(index);
        return;
     }

   int thread_mode = EVAS_GL_THREAD_MODE_FINISH;

   EVGL_Thread_Command_glDisableVertexAttribArray thread_param_local;
   EVGL_Thread_Command_glDisableVertexAttribArray *thread_param = &thread_param_local;

   /* command_allocated flag init. */
   thread_param->command_allocated = 0;

   if (!evas_gl_thread_force_finish())
     { /* _flush */
        EVGL_Thread_Command_glDisableVertexAttribArray *thread_param_new;
        thread_param_new = eina_mempool_malloc(_mp_command,
                                               sizeof(EVGL_Thread_Command_glDisableVertexAttribArray));
        if (thread_param_new)
          {
             thread_param = thread_param_new;
             thread_param->command_allocated = 1;
             thread_mode = EVAS_GL_THREAD_MODE_FLUSH;
          }
     }

   thread_param->index = index;

   evas_gl_thread_cmd_enqueue(EVAS_GL_THREAD_TYPE_EVGL,
                              _evgl_thread_glDisableVertexAttribArray,
                              thread_param,
                              thread_mode);
}

/*
   void
   glDrawArrays(GLenum mode, GLint first, GLsizei count);
 */

typedef struct
{
   GLenum mode;
   GLint first;
   GLsizei count;

} EVGL_Thread_Command_glDrawArrays;

static void
_evgl_thread_glDrawArrays(void *data)
{
   EVGL_Thread_Command_glDrawArrays *thread_param =
      (EVGL_Thread_Command_glDrawArrays *)data;

   glDrawArrays(thread_param->mode,
                thread_param->first,
                thread_param->count);

}

EAPI void
glDrawArrays_evgl_thread_cmd(GLenum mode, GLint first, GLsizei count)
{
   if (!evas_evgl_thread_enabled())
     {
        glDrawArrays(mode, first, count);
        return;
     }

   int thread_mode = EVAS_GL_THREAD_MODE_FINISH;

   EVGL_Thread_Command_glDrawArrays thread_param_local;
   EVGL_Thread_Command_glDrawArrays *thread_param = &thread_param_local;

   thread_param->mode = mode;
   thread_param->first = first;
   thread_param->count = count;

   evas_gl_thread_cmd_enqueue(EVAS_GL_THREAD_TYPE_EVGL,
                              _evgl_thread_glDrawArrays,
                              thread_param,
                              thread_mode);
}

/*
   void
   glDrawElements(GLenum mode, GLsizei count, GLenum type, const void *indices);
 */

typedef struct
{
   GLenum mode;
   GLsizei count;
   GLenum type;
   const void *indices;

} EVGL_Thread_Command_glDrawElements;

static void
_evgl_thread_glDrawElements(void *data)
{
   EVGL_Thread_Command_glDrawElements *thread_param =
      (EVGL_Thread_Command_glDrawElements *)data;

   glDrawElements(thread_param->mode,
                  thread_param->count,
                  thread_param->type,
                  thread_param->indices);

}

EAPI void
glDrawElements_evgl_thread_cmd(GLenum mode, GLsizei count, GLenum type, const void *indices)
{
   if (!evas_evgl_thread_enabled())
     {
        glDrawElements(mode, count, type, indices);
        return;
     }

   int thread_mode = EVAS_GL_THREAD_MODE_FINISH;

   EVGL_Thread_Command_glDrawElements thread_param_local;
   EVGL_Thread_Command_glDrawElements *thread_param = &thread_param_local;

   thread_param->mode = mode;
   thread_param->count = count;
   thread_param->type = type;
   thread_param->indices = indices;

   evas_gl_thread_cmd_enqueue(EVAS_GL_THREAD_TYPE_EVGL,
                              _evgl_thread_glDrawElements,
                              thread_param,
                              thread_mode);
}

/*
   void
   glGenBuffers(GLsizei n, GLuint *buffers);
 */

typedef struct
{
   GLsizei n;
   GLuint *buffers;

} EVGL_Thread_Command_glGenBuffers;

static void
_evgl_thread_glGenBuffers(void *data)
{
   EVGL_Thread_Command_glGenBuffers *thread_param =
      (EVGL_Thread_Command_glGenBuffers *)data;

   glGenBuffers(thread_param->n,
                thread_param->buffers);

}

EAPI void
glGenBuffers_evgl_thread_cmd(GLsizei n, GLuint *buffers)
{
   if (!evas_evgl_thread_enabled())
     {
        glGenBuffers(n, buffers);
        return;
     }

   int thread_mode = EVAS_GL_THREAD_MODE_FINISH;

   EVGL_Thread_Command_glGenBuffers thread_param_local;
   EVGL_Thread_Command_glGenBuffers *thread_param = &thread_param_local;

   thread_param->n = n;
   thread_param->buffers = buffers;

   evas_gl_thread_cmd_enqueue(EVAS_GL_THREAD_TYPE_EVGL,
                              _evgl_thread_glGenBuffers,
                              thread_param,
                              thread_mode);
}

/*
   void
   glDeleteBuffers(GLsizei n, const GLuint *buffers);
 */

typedef struct
{
   GLsizei n;
   const GLuint *buffers;
   void *buffers_copied; /* COPIED */
   int command_allocated;

} EVGL_Thread_Command_glDeleteBuffers;

static void
_evgl_thread_glDeleteBuffers(void *data)
{
   EVGL_Thread_Command_glDeleteBuffers *thread_param =
      (EVGL_Thread_Command_glDeleteBuffers *)data;

   glDeleteBuffers(thread_param->n,
                   thread_param->buffers);


   if (thread_param->buffers_copied)
     eina_mempool_free(_mp_delete_object, thread_param->buffers_copied);

   if (thread_param->command_allocated)
     eina_mempool_free(_mp_command, thread_param);
}

EAPI void
glDeleteBuffers_evgl_thread_cmd(GLsizei n, const GLuint *buffers)
{
   if (!evas_evgl_thread_enabled())
     {
        glDeleteBuffers(n, buffers);
        return;
     }

   int thread_mode = EVAS_GL_THREAD_MODE_FINISH;

   EVGL_Thread_Command_glDeleteBuffers thread_param_local;
   EVGL_Thread_Command_glDeleteBuffers *thread_param = &thread_param_local;

   /* command_allocated flag init. */
   thread_param->command_allocated = 0;

   if (!evas_gl_thread_force_finish())
     { /* _flush */
        EVGL_Thread_Command_glDeleteBuffers *thread_param_new;
        thread_param_new = eina_mempool_malloc(_mp_command,
                                               sizeof(EVGL_Thread_Command_glDeleteBuffers));
        if (thread_param_new)
          {
             thread_param = thread_param_new;
             thread_param->command_allocated = 1;
             thread_mode = EVAS_GL_THREAD_MODE_FLUSH;
          }
     }

   thread_param->n = n;
   thread_param->buffers = buffers;

   thread_param->buffers_copied = NULL;
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
        thread_param->buffers_copied = eina_mempool_malloc(_mp_delete_object, copy_size);
        if (thread_param->buffers_copied)
          {
             memcpy(thread_param->buffers_copied, buffers, copy_size);
          }
        else
          {
             thread_mode = EVAS_GL_THREAD_MODE_FINISH;
             goto finish;
          }
        /* 3. replace */
        thread_param->buffers = (const GLuint  *)thread_param->buffers_copied;
     }
   /* end of copy variable */

finish:
   evas_gl_thread_cmd_enqueue(EVAS_GL_THREAD_TYPE_EVGL,
                              _evgl_thread_glDeleteBuffers,
                              thread_param,
                              thread_mode);
}

/*
   void
   glBindBuffer(GLenum target, GLuint buffer);
 */

typedef struct
{
   GLenum target;
   GLuint buffer;
   int command_allocated;

} EVGL_Thread_Command_glBindBuffer;

static void
_evgl_thread_glBindBuffer(void *data)
{
   EVGL_Thread_Command_glBindBuffer *thread_param =
      (EVGL_Thread_Command_glBindBuffer *)data;

   glBindBuffer(thread_param->target,
                thread_param->buffer);

   if (thread_param->command_allocated)
     eina_mempool_free(_mp_command, thread_param);
}

EAPI void
glBindBuffer_evgl_thread_cmd(GLenum target, GLuint buffer)
{
   if (!evas_evgl_thread_enabled())
     {
        glBindBuffer(target, buffer);
        return;
     }

   int thread_mode = EVAS_GL_THREAD_MODE_FINISH;

   EVGL_Thread_Command_glBindBuffer thread_param_local;
   EVGL_Thread_Command_glBindBuffer *thread_param = &thread_param_local;

   /* command_allocated flag init. */
   thread_param->command_allocated = 0;

   if (!evas_gl_thread_force_finish())
     { /* _flush */
        EVGL_Thread_Command_glBindBuffer *thread_param_new;
        thread_param_new = eina_mempool_malloc(_mp_command,
                                               sizeof(EVGL_Thread_Command_glBindBuffer));
        if (thread_param_new)
          {
             thread_param = thread_param_new;
             thread_param->command_allocated = 1;
             thread_mode = EVAS_GL_THREAD_MODE_FLUSH;
          }
     }

   thread_param->target = target;
   thread_param->buffer = buffer;

   evas_gl_thread_cmd_enqueue(EVAS_GL_THREAD_TYPE_EVGL,
                              _evgl_thread_glBindBuffer,
                              thread_param,
                              thread_mode);
}

/*
   void
   glBufferData(GLenum target, GLsizeiptr size, const void *data, GLenum usage);
 */

typedef struct
{
   GLenum target;
   GLsizeiptr size;
   const void *data;
   GLenum usage;

} EVGL_Thread_Command_glBufferData;

static void
_evgl_thread_glBufferData(void *data)
{
   EVGL_Thread_Command_glBufferData *thread_param =
      (EVGL_Thread_Command_glBufferData *)data;

   glBufferData(thread_param->target,
                thread_param->size,
                thread_param->data,
                thread_param->usage);

}

EAPI void
glBufferData_evgl_thread_cmd(GLenum target, GLsizeiptr size, const void *data, GLenum usage)
{
   if (!evas_evgl_thread_enabled())
     {
        glBufferData(target, size, data, usage);
        return;
     }

   int thread_mode = EVAS_GL_THREAD_MODE_FINISH;

   EVGL_Thread_Command_glBufferData thread_param_local;
   EVGL_Thread_Command_glBufferData *thread_param = &thread_param_local;

   thread_param->target = target;
   thread_param->size = size;
   thread_param->data = data;
   thread_param->usage = usage;

   evas_gl_thread_cmd_enqueue(EVAS_GL_THREAD_TYPE_EVGL,
                              _evgl_thread_glBufferData,
                              thread_param,
                              thread_mode);
}

/*
   GLuint
   glCreateShader(GLenum type);
 */

typedef struct
{
   GLuint return_value;
   GLenum type;

} EVGL_Thread_Command_glCreateShader;

static void
_evgl_thread_glCreateShader(void *data)
{
   EVGL_Thread_Command_glCreateShader *thread_param =
      (EVGL_Thread_Command_glCreateShader *)data;

   thread_param->return_value = glCreateShader(thread_param->type);

}

EAPI GLuint
glCreateShader_evgl_thread_cmd(GLenum type)
{
   if (!evas_evgl_thread_enabled())
     {
        return glCreateShader(type);
     }

   int thread_mode = EVAS_GL_THREAD_MODE_FINISH;

   EVGL_Thread_Command_glCreateShader thread_param_local;
   EVGL_Thread_Command_glCreateShader *thread_param = &thread_param_local;

   thread_param->type = type;

   evas_gl_thread_cmd_enqueue(EVAS_GL_THREAD_TYPE_EVGL,
                              _evgl_thread_glCreateShader,
                              thread_param,
                              thread_mode);

   return thread_param->return_value;
}

/*
   void
   glShaderSource(GLuint shader, GLsizei count, const GLchar **string, const GLint *length);
 */

typedef struct
{
   GLuint shader;
   GLsizei count;
   const GLchar **string;
   const GLint *length;
   int command_allocated;
   GLSHADERSOURCE_COPY_VARIABLE; /* TODO */

} EVGL_Thread_Command_glShaderSource;

static void
_evgl_thread_glShaderSource(void *data)
{
   EVGL_Thread_Command_glShaderSource *thread_param =
      (EVGL_Thread_Command_glShaderSource *)data;

   glShaderSource(thread_param->shader,
                  thread_param->count,
                  thread_param->string,
                  thread_param->length);

   GLSHADERSOURCE_COPY_VARIABLE_FREE; /* TODO */

   if (thread_param->command_allocated)
     eina_mempool_free(_mp_command, thread_param);
}

EAPI void
glShaderSource_evgl_thread_cmd(GLuint shader, GLsizei count, const GLchar **string, const GLint *length)
{
   if (!evas_evgl_thread_enabled())
     {
        glShaderSource(shader, count, string, length);
        return;
     }

   int thread_mode = EVAS_GL_THREAD_MODE_FINISH;

   EVGL_Thread_Command_glShaderSource thread_param_local;
   EVGL_Thread_Command_glShaderSource *thread_param = &thread_param_local;

   /* command_allocated flag init. */
   thread_param->command_allocated = 0;

   if (!evas_gl_thread_force_finish())
     { /* _flush */
        EVGL_Thread_Command_glShaderSource *thread_param_new;
        thread_param_new = eina_mempool_malloc(_mp_command,
                                               sizeof(EVGL_Thread_Command_glShaderSource));
        if (thread_param_new)
          {
             thread_param = thread_param_new;
             thread_param->command_allocated = 1;
             thread_mode = EVAS_GL_THREAD_MODE_FLUSH;
          }
     }

   thread_param->shader = shader;
   thread_param->count = count;
   thread_param->string = string;
   thread_param->length = length;

   GLSHADERSOURCE_COPY_VARIABLE_INIT; /* TODO */

   if (thread_mode == EVAS_GL_THREAD_MODE_FINISH)
     goto finish;

   GLSHADERSOURCE_COPY_TO_MEMPOOL; /* TODO */

finish:
   evas_gl_thread_cmd_enqueue(EVAS_GL_THREAD_TYPE_EVGL,
                              _evgl_thread_glShaderSource,
                              thread_param,
                              thread_mode);
}

/*
   void
   glCompileShader(GLuint shader);
 */

typedef struct
{
   GLuint shader;
   int command_allocated;

} EVGL_Thread_Command_glCompileShader;

static void
_evgl_thread_glCompileShader(void *data)
{
   EVGL_Thread_Command_glCompileShader *thread_param =
      (EVGL_Thread_Command_glCompileShader *)data;

   glCompileShader(thread_param->shader);

   if (thread_param->command_allocated)
     eina_mempool_free(_mp_command, thread_param);
}

EAPI void
glCompileShader_evgl_thread_cmd(GLuint shader)
{
   if (!evas_evgl_thread_enabled())
     {
        glCompileShader(shader);
        return;
     }

   int thread_mode = EVAS_GL_THREAD_MODE_FINISH;

   EVGL_Thread_Command_glCompileShader thread_param_local;
   EVGL_Thread_Command_glCompileShader *thread_param = &thread_param_local;

   /* command_allocated flag init. */
   thread_param->command_allocated = 0;

   if (!evas_gl_thread_force_finish())
     { /* _flush */
        EVGL_Thread_Command_glCompileShader *thread_param_new;
        thread_param_new = eina_mempool_malloc(_mp_command,
                                               sizeof(EVGL_Thread_Command_glCompileShader));
        if (thread_param_new)
          {
             thread_param = thread_param_new;
             thread_param->command_allocated = 1;
             thread_mode = EVAS_GL_THREAD_MODE_FLUSH;
          }
     }

   thread_param->shader = shader;

   evas_gl_thread_cmd_enqueue(EVAS_GL_THREAD_TYPE_EVGL,
                              _evgl_thread_glCompileShader,
                              thread_param,
                              thread_mode);
}

/*
   void
   glDeleteShader(GLuint shader);
 */

typedef struct
{
   GLuint shader;
   int command_allocated;

} EVGL_Thread_Command_glDeleteShader;

static void
_evgl_thread_glDeleteShader(void *data)
{
   EVGL_Thread_Command_glDeleteShader *thread_param =
      (EVGL_Thread_Command_glDeleteShader *)data;

   glDeleteShader(thread_param->shader);

   if (thread_param->command_allocated)
     eina_mempool_free(_mp_command, thread_param);
}

EAPI void
glDeleteShader_evgl_thread_cmd(GLuint shader)
{
   if (!evas_evgl_thread_enabled())
     {
        glDeleteShader(shader);
        return;
     }

   int thread_mode = EVAS_GL_THREAD_MODE_FINISH;

   EVGL_Thread_Command_glDeleteShader thread_param_local;
   EVGL_Thread_Command_glDeleteShader *thread_param = &thread_param_local;

   /* command_allocated flag init. */
   thread_param->command_allocated = 0;

   if (!evas_gl_thread_force_finish())
     { /* _flush */
        EVGL_Thread_Command_glDeleteShader *thread_param_new;
        thread_param_new = eina_mempool_malloc(_mp_command,
                                               sizeof(EVGL_Thread_Command_glDeleteShader));
        if (thread_param_new)
          {
             thread_param = thread_param_new;
             thread_param->command_allocated = 1;
             thread_mode = EVAS_GL_THREAD_MODE_FLUSH;
          }
     }

   thread_param->shader = shader;

   evas_gl_thread_cmd_enqueue(EVAS_GL_THREAD_TYPE_EVGL,
                              _evgl_thread_glDeleteShader,
                              thread_param,
                              thread_mode);
}

/*
   GLuint
   glCreateProgram(void);
 */

typedef struct
{
   GLuint return_value;

} EVGL_Thread_Command_glCreateProgram;

static void
_evgl_thread_glCreateProgram(void *data)
{
   EVGL_Thread_Command_glCreateProgram *thread_param =
      (EVGL_Thread_Command_glCreateProgram *)data;

   thread_param->return_value = glCreateProgram();

}

EAPI GLuint
glCreateProgram_evgl_thread_cmd(void)
{
   if (!evas_evgl_thread_enabled())
     {
        return glCreateProgram();
     }

   int thread_mode = EVAS_GL_THREAD_MODE_FINISH;

   EVGL_Thread_Command_glCreateProgram thread_param_local;
   EVGL_Thread_Command_glCreateProgram *thread_param = &thread_param_local;


   evas_gl_thread_cmd_enqueue(EVAS_GL_THREAD_TYPE_EVGL,
                              _evgl_thread_glCreateProgram,
                              thread_param,
                              thread_mode);

   return thread_param->return_value;
}

/*
   void
   glAttachShader(GLuint program, GLuint shader);
 */

typedef struct
{
   GLuint program;
   GLuint shader;
   int command_allocated;

} EVGL_Thread_Command_glAttachShader;

static void
_evgl_thread_glAttachShader(void *data)
{
   EVGL_Thread_Command_glAttachShader *thread_param =
      (EVGL_Thread_Command_glAttachShader *)data;

   glAttachShader(thread_param->program,
                  thread_param->shader);

   if (thread_param->command_allocated)
     eina_mempool_free(_mp_command, thread_param);
}

EAPI void
glAttachShader_evgl_thread_cmd(GLuint program, GLuint shader)
{
   if (!evas_evgl_thread_enabled())
     {
        glAttachShader(program, shader);
        return;
     }

   int thread_mode = EVAS_GL_THREAD_MODE_FINISH;

   EVGL_Thread_Command_glAttachShader thread_param_local;
   EVGL_Thread_Command_glAttachShader *thread_param = &thread_param_local;

   /* command_allocated flag init. */
   thread_param->command_allocated = 0;

   if (!evas_gl_thread_force_finish())
     { /* _flush */
        EVGL_Thread_Command_glAttachShader *thread_param_new;
        thread_param_new = eina_mempool_malloc(_mp_command,
                                               sizeof(EVGL_Thread_Command_glAttachShader));
        if (thread_param_new)
          {
             thread_param = thread_param_new;
             thread_param->command_allocated = 1;
             thread_mode = EVAS_GL_THREAD_MODE_FLUSH;
          }
     }

   thread_param->program = program;
   thread_param->shader = shader;

   evas_gl_thread_cmd_enqueue(EVAS_GL_THREAD_TYPE_EVGL,
                              _evgl_thread_glAttachShader,
                              thread_param,
                              thread_mode);
}

/*
   void
   glDetachShader(GLuint program, GLuint shader);
 */

typedef struct
{
   GLuint program;
   GLuint shader;
   int command_allocated;

} EVGL_Thread_Command_glDetachShader;

static void
_evgl_thread_glDetachShader(void *data)
{
   EVGL_Thread_Command_glDetachShader *thread_param =
      (EVGL_Thread_Command_glDetachShader *)data;

   glDetachShader(thread_param->program,
                  thread_param->shader);

   if (thread_param->command_allocated)
     eina_mempool_free(_mp_command, thread_param);
}

EAPI void
glDetachShader_evgl_thread_cmd(GLuint program, GLuint shader)
{
   if (!evas_evgl_thread_enabled())
     {
        glDetachShader(program, shader);
        return;
     }

   int thread_mode = EVAS_GL_THREAD_MODE_FINISH;

   EVGL_Thread_Command_glDetachShader thread_param_local;
   EVGL_Thread_Command_glDetachShader *thread_param = &thread_param_local;

   /* command_allocated flag init. */
   thread_param->command_allocated = 0;

   if (!evas_gl_thread_force_finish())
     { /* _flush */
        EVGL_Thread_Command_glDetachShader *thread_param_new;
        thread_param_new = eina_mempool_malloc(_mp_command,
                                               sizeof(EVGL_Thread_Command_glDetachShader));
        if (thread_param_new)
          {
             thread_param = thread_param_new;
             thread_param->command_allocated = 1;
             thread_mode = EVAS_GL_THREAD_MODE_FLUSH;
          }
     }

   thread_param->program = program;
   thread_param->shader = shader;

   evas_gl_thread_cmd_enqueue(EVAS_GL_THREAD_TYPE_EVGL,
                              _evgl_thread_glDetachShader,
                              thread_param,
                              thread_mode);
}

/*
   void
   glLinkProgram(GLuint program);
 */

typedef struct
{
   GLuint program;
   int command_allocated;

} EVGL_Thread_Command_glLinkProgram;

static void
_evgl_thread_glLinkProgram(void *data)
{
   EVGL_Thread_Command_glLinkProgram *thread_param =
      (EVGL_Thread_Command_glLinkProgram *)data;

   glLinkProgram(thread_param->program);

   if (thread_param->command_allocated)
     eina_mempool_free(_mp_command, thread_param);
}

EAPI void
glLinkProgram_evgl_thread_cmd(GLuint program)
{
   if (!evas_evgl_thread_enabled())
     {
        glLinkProgram(program);
        return;
     }

   int thread_mode = EVAS_GL_THREAD_MODE_FINISH;

   EVGL_Thread_Command_glLinkProgram thread_param_local;
   EVGL_Thread_Command_glLinkProgram *thread_param = &thread_param_local;

   /* command_allocated flag init. */
   thread_param->command_allocated = 0;

   if (!evas_gl_thread_force_finish())
     { /* _flush */
        EVGL_Thread_Command_glLinkProgram *thread_param_new;
        thread_param_new = eina_mempool_malloc(_mp_command,
                                               sizeof(EVGL_Thread_Command_glLinkProgram));
        if (thread_param_new)
          {
             thread_param = thread_param_new;
             thread_param->command_allocated = 1;
             thread_mode = EVAS_GL_THREAD_MODE_FLUSH;
          }
     }

   thread_param->program = program;

   evas_gl_thread_cmd_enqueue(EVAS_GL_THREAD_TYPE_EVGL,
                              _evgl_thread_glLinkProgram,
                              thread_param,
                              thread_mode);
}

/*
   void
   glUseProgram(GLuint program);
 */

typedef struct
{
   GLuint program;
   int command_allocated;

} EVGL_Thread_Command_glUseProgram;

static void
_evgl_thread_glUseProgram(void *data)
{
   EVGL_Thread_Command_glUseProgram *thread_param =
      (EVGL_Thread_Command_glUseProgram *)data;

   glUseProgram(thread_param->program);

   if (thread_param->command_allocated)
     eina_mempool_free(_mp_command, thread_param);
}

EAPI void
glUseProgram_evgl_thread_cmd(GLuint program)
{
   if (!evas_evgl_thread_enabled())
     {
        glUseProgram(program);
        return;
     }

   int thread_mode = EVAS_GL_THREAD_MODE_FINISH;

   EVGL_Thread_Command_glUseProgram thread_param_local;
   EVGL_Thread_Command_glUseProgram *thread_param = &thread_param_local;

   /* command_allocated flag init. */
   thread_param->command_allocated = 0;

   if (!evas_gl_thread_force_finish())
     { /* _flush */
        EVGL_Thread_Command_glUseProgram *thread_param_new;
        thread_param_new = eina_mempool_malloc(_mp_command,
                                               sizeof(EVGL_Thread_Command_glUseProgram));
        if (thread_param_new)
          {
             thread_param = thread_param_new;
             thread_param->command_allocated = 1;
             thread_mode = EVAS_GL_THREAD_MODE_FLUSH;
          }
     }

   thread_param->program = program;

   evas_gl_thread_cmd_enqueue(EVAS_GL_THREAD_TYPE_EVGL,
                              _evgl_thread_glUseProgram,
                              thread_param,
                              thread_mode);
}

/*
   void
   glDeleteProgram(GLuint program);
 */

typedef struct
{
   GLuint program;
   int command_allocated;

} EVGL_Thread_Command_glDeleteProgram;

static void
_evgl_thread_glDeleteProgram(void *data)
{
   EVGL_Thread_Command_glDeleteProgram *thread_param =
      (EVGL_Thread_Command_glDeleteProgram *)data;

   glDeleteProgram(thread_param->program);

   if (thread_param->command_allocated)
     eina_mempool_free(_mp_command, thread_param);
}

EAPI void
glDeleteProgram_evgl_thread_cmd(GLuint program)
{
   if (!evas_evgl_thread_enabled())
     {
        glDeleteProgram(program);
        return;
     }

   int thread_mode = EVAS_GL_THREAD_MODE_FINISH;

   EVGL_Thread_Command_glDeleteProgram thread_param_local;
   EVGL_Thread_Command_glDeleteProgram *thread_param = &thread_param_local;

   /* command_allocated flag init. */
   thread_param->command_allocated = 0;

   if (!evas_gl_thread_force_finish())
     { /* _flush */
        EVGL_Thread_Command_glDeleteProgram *thread_param_new;
        thread_param_new = eina_mempool_malloc(_mp_command,
                                               sizeof(EVGL_Thread_Command_glDeleteProgram));
        if (thread_param_new)
          {
             thread_param = thread_param_new;
             thread_param->command_allocated = 1;
             thread_mode = EVAS_GL_THREAD_MODE_FLUSH;
          }
     }

   thread_param->program = program;

   evas_gl_thread_cmd_enqueue(EVAS_GL_THREAD_TYPE_EVGL,
                              _evgl_thread_glDeleteProgram,
                              thread_param,
                              thread_mode);
}

/*
   void
   glGetProgramBinary(GLuint program, GLsizei bufSize, GLsizei *length, GLenum *binaryFormat, void *binary);
 */

typedef struct
{
   GLuint program;
   GLsizei bufSize;
   GLsizei *length;
   GLenum *binaryFormat;
   void *binary;

} EVGL_Thread_Command_glGetProgramBinary;

void (*orig_evgl_glGetProgramBinary)(GLuint program, GLsizei bufSize, GLsizei *length, GLenum *binaryFormat, void *binary);

void
glGetProgramBinary_orig_evgl_set(void *func)
{
   orig_evgl_glGetProgramBinary = func;
}

void *
glGetProgramBinary_orig_evgl_get(void)
{
   return orig_evgl_glGetProgramBinary;
}

static void
_evgl_thread_glGetProgramBinary(void *data)
{
   EVGL_Thread_Command_glGetProgramBinary *thread_param =
      (EVGL_Thread_Command_glGetProgramBinary *)data;

   orig_evgl_glGetProgramBinary(thread_param->program,
                                thread_param->bufSize,
                                thread_param->length,
                                thread_param->binaryFormat,
                                thread_param->binary);

}

EAPI void
glGetProgramBinary_evgl_thread_cmd(GLuint program, GLsizei bufSize, GLsizei *length, GLenum *binaryFormat, void *binary)
{
   if (!evas_evgl_thread_enabled())
     {
        orig_evgl_glGetProgramBinary(program, bufSize, length, binaryFormat, binary);
        return;
     }

   int thread_mode = EVAS_GL_THREAD_MODE_FINISH;

   EVGL_Thread_Command_glGetProgramBinary thread_param_local;
   EVGL_Thread_Command_glGetProgramBinary *thread_param = &thread_param_local;

   thread_param->program = program;
   thread_param->bufSize = bufSize;
   thread_param->length = length;
   thread_param->binaryFormat = binaryFormat;
   thread_param->binary = binary;

   evas_gl_thread_cmd_enqueue(EVAS_GL_THREAD_TYPE_EVGL,
                              _evgl_thread_glGetProgramBinary,
                              thread_param,
                              thread_mode);
}

/*
   void
   glProgramBinary(GLuint program, GLenum binaryFormat, const void *binary, GLint length);
 */

typedef struct
{
   GLuint program;
   GLenum binaryFormat;
   const void *binary;
   GLint length;
   void *binary_copied; /* COPIED */
   int command_allocated;

} EVGL_Thread_Command_glProgramBinary;

void (*orig_evgl_glProgramBinary)(GLuint program, GLenum binaryFormat, const void *binary, GLint length);

void
glProgramBinary_orig_evgl_set(void *func)
{
   orig_evgl_glProgramBinary = func;
}

void *
glProgramBinary_orig_evgl_get(void)
{
   return orig_evgl_glProgramBinary;
}

static void
_evgl_thread_glProgramBinary(void *data)
{
   EVGL_Thread_Command_glProgramBinary *thread_param =
      (EVGL_Thread_Command_glProgramBinary *)data;

   orig_evgl_glProgramBinary(thread_param->program,
                             thread_param->binaryFormat,
                             thread_param->binary,
                             thread_param->length);


   if (thread_param->binary_copied)
     eina_mempool_free(_mp_default, thread_param->binary_copied);

   if (thread_param->command_allocated)
     eina_mempool_free(_mp_command, thread_param);
}

EAPI void
glProgramBinary_evgl_thread_cmd(GLuint program, GLenum binaryFormat, const void *binary, GLint length)
{
   if (!evas_evgl_thread_enabled())
     {
        orig_evgl_glProgramBinary(program, binaryFormat, binary, length);
        return;
     }

   int thread_mode = EVAS_GL_THREAD_MODE_FINISH;

   EVGL_Thread_Command_glProgramBinary thread_param_local;
   EVGL_Thread_Command_glProgramBinary *thread_param = &thread_param_local;

   /* command_allocated flag init. */
   thread_param->command_allocated = 0;

   if (!evas_gl_thread_force_finish())
     { /* _flush */
        EVGL_Thread_Command_glProgramBinary *thread_param_new;
        thread_param_new = eina_mempool_malloc(_mp_command,
                                               sizeof(EVGL_Thread_Command_glProgramBinary));
        if (thread_param_new)
          {
             thread_param = thread_param_new;
             thread_param->command_allocated = 1;
             thread_mode = EVAS_GL_THREAD_MODE_FLUSH;
          }
     }

   thread_param->program = program;
   thread_param->binaryFormat = binaryFormat;
   thread_param->binary = binary;
   thread_param->length = length;

   thread_param->binary_copied = NULL;
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
        thread_param->binary_copied = eina_mempool_malloc(_mp_default, copy_size);
        if (thread_param->binary_copied)
          {
             memcpy(thread_param->binary_copied, binary, copy_size);
          }
        else
          {
             thread_mode = EVAS_GL_THREAD_MODE_FINISH;
             goto finish;
          }
        /* 3. replace */
        thread_param->binary = (const void  *)thread_param->binary_copied;
     }
   /* end of copy variable */

finish:
   evas_gl_thread_cmd_enqueue(EVAS_GL_THREAD_TYPE_EVGL,
                              _evgl_thread_glProgramBinary,
                              thread_param,
                              thread_mode);
}

/*
   void
   glGetActiveAttrib(GLuint program, GLuint index, GLsizei bufSize, GLsizei *length, GLint *size, GLenum *type, GLchar *name);
 */

typedef struct
{
   GLuint program;
   GLuint index;
   GLsizei bufSize;
   GLsizei *length;
   GLint *size;
   GLenum *type;
   GLchar *name;

} EVGL_Thread_Command_glGetActiveAttrib;

static void
_evgl_thread_glGetActiveAttrib(void *data)
{
   EVGL_Thread_Command_glGetActiveAttrib *thread_param =
      (EVGL_Thread_Command_glGetActiveAttrib *)data;

   glGetActiveAttrib(thread_param->program,
                     thread_param->index,
                     thread_param->bufSize,
                     thread_param->length,
                     thread_param->size,
                     thread_param->type,
                     thread_param->name);

}

EAPI void
glGetActiveAttrib_evgl_thread_cmd(GLuint program, GLuint index, GLsizei bufSize, GLsizei *length, GLint *size, GLenum *type, GLchar *name)
{
   if (!evas_evgl_thread_enabled())
     {
        glGetActiveAttrib(program, index, bufSize, length, size, type, name);
        return;
     }

   int thread_mode = EVAS_GL_THREAD_MODE_FINISH;

   EVGL_Thread_Command_glGetActiveAttrib thread_param_local;
   EVGL_Thread_Command_glGetActiveAttrib *thread_param = &thread_param_local;

   thread_param->program = program;
   thread_param->index = index;
   thread_param->bufSize = bufSize;
   thread_param->length = length;
   thread_param->size = size;
   thread_param->type = type;
   thread_param->name = name;

   evas_gl_thread_cmd_enqueue(EVAS_GL_THREAD_TYPE_EVGL,
                              _evgl_thread_glGetActiveAttrib,
                              thread_param,
                              thread_mode);
}

/*
   GLint
   glGetAttribLocation(GLuint program, const GLchar *name);
 */

typedef struct
{
   GLint return_value;
   GLuint program;
   const GLchar *name;

} EVGL_Thread_Command_glGetAttribLocation;

static void
_evgl_thread_glGetAttribLocation(void *data)
{
   EVGL_Thread_Command_glGetAttribLocation *thread_param =
      (EVGL_Thread_Command_glGetAttribLocation *)data;

   thread_param->return_value = glGetAttribLocation(thread_param->program,
                                                    thread_param->name);

}

EAPI GLint
glGetAttribLocation_evgl_thread_cmd(GLuint program, const GLchar *name)
{
   if (!evas_evgl_thread_enabled())
     {
        return glGetAttribLocation(program, name);
     }

   int thread_mode = EVAS_GL_THREAD_MODE_FINISH;

   EVGL_Thread_Command_glGetAttribLocation thread_param_local;
   EVGL_Thread_Command_glGetAttribLocation *thread_param = &thread_param_local;

   thread_param->program = program;
   thread_param->name = name;

   evas_gl_thread_cmd_enqueue(EVAS_GL_THREAD_TYPE_EVGL,
                              _evgl_thread_glGetAttribLocation,
                              thread_param,
                              thread_mode);

   return thread_param->return_value;
}

/*
   void
   glBindAttribLocation(GLuint program, GLuint index, const GLchar *name);
 */

typedef struct
{
   GLuint program;
   GLuint index;
   const GLchar *name;

} EVGL_Thread_Command_glBindAttribLocation;

static void
_evgl_thread_glBindAttribLocation(void *data)
{
   EVGL_Thread_Command_glBindAttribLocation *thread_param =
      (EVGL_Thread_Command_glBindAttribLocation *)data;

   glBindAttribLocation(thread_param->program,
                        thread_param->index,
                        thread_param->name);

}

EAPI void
glBindAttribLocation_evgl_thread_cmd(GLuint program, GLuint index, const GLchar *name)
{
   if (!evas_evgl_thread_enabled())
     {
        glBindAttribLocation(program, index, name);
        return;
     }

   int thread_mode = EVAS_GL_THREAD_MODE_FINISH;

   EVGL_Thread_Command_glBindAttribLocation thread_param_local;
   EVGL_Thread_Command_glBindAttribLocation *thread_param = &thread_param_local;

   thread_param->program = program;
   thread_param->index = index;
   thread_param->name = name;

   evas_gl_thread_cmd_enqueue(EVAS_GL_THREAD_TYPE_EVGL,
                              _evgl_thread_glBindAttribLocation,
                              thread_param,
                              thread_mode);
}

/*
   GLint
   glGetUniformLocation(GLuint program, const GLchar *name);
 */

typedef struct
{
   GLint return_value;
   GLuint program;
   const GLchar *name;

} EVGL_Thread_Command_glGetUniformLocation;

static void
_evgl_thread_glGetUniformLocation(void *data)
{
   EVGL_Thread_Command_glGetUniformLocation *thread_param =
      (EVGL_Thread_Command_glGetUniformLocation *)data;

   thread_param->return_value = glGetUniformLocation(thread_param->program,
                                                     thread_param->name);

}

EAPI GLint
glGetUniformLocation_evgl_thread_cmd(GLuint program, const GLchar *name)
{
   if (!evas_evgl_thread_enabled())
     {
        return glGetUniformLocation(program, name);
     }

   int thread_mode = EVAS_GL_THREAD_MODE_FINISH;

   EVGL_Thread_Command_glGetUniformLocation thread_param_local;
   EVGL_Thread_Command_glGetUniformLocation *thread_param = &thread_param_local;

   thread_param->program = program;
   thread_param->name = name;

   evas_gl_thread_cmd_enqueue(EVAS_GL_THREAD_TYPE_EVGL,
                              _evgl_thread_glGetUniformLocation,
                              thread_param,
                              thread_mode);

   return thread_param->return_value;
}

/*
   void
   glUniform1f(GLint location, GLfloat v0);
 */

typedef struct
{
   GLint location;
   GLfloat v0;
   int command_allocated;

} EVGL_Thread_Command_glUniform1f;

static void
_evgl_thread_glUniform1f(void *data)
{
   EVGL_Thread_Command_glUniform1f *thread_param =
      (EVGL_Thread_Command_glUniform1f *)data;

   glUniform1f(thread_param->location,
               thread_param->v0);

   if (thread_param->command_allocated)
     eina_mempool_free(_mp_command, thread_param);
}

EAPI void
glUniform1f_evgl_thread_cmd(GLint location, GLfloat v0)
{
   if (!evas_evgl_thread_enabled())
     {
        glUniform1f(location, v0);
        return;
     }

   int thread_mode = EVAS_GL_THREAD_MODE_FINISH;

   EVGL_Thread_Command_glUniform1f thread_param_local;
   EVGL_Thread_Command_glUniform1f *thread_param = &thread_param_local;

   /* command_allocated flag init. */
   thread_param->command_allocated = 0;

   if (!evas_gl_thread_force_finish())
     { /* _flush */
        EVGL_Thread_Command_glUniform1f *thread_param_new;
        thread_param_new = eina_mempool_malloc(_mp_command,
                                               sizeof(EVGL_Thread_Command_glUniform1f));
        if (thread_param_new)
          {
             thread_param = thread_param_new;
             thread_param->command_allocated = 1;
             thread_mode = EVAS_GL_THREAD_MODE_FLUSH;
          }
     }

   thread_param->location = location;
   thread_param->v0 = v0;

   evas_gl_thread_cmd_enqueue(EVAS_GL_THREAD_TYPE_EVGL,
                              _evgl_thread_glUniform1f,
                              thread_param,
                              thread_mode);
}

/*
   void
   glUniform1i(GLint location, GLint v0);
 */

typedef struct
{
   GLint location;
   GLint v0;
   int command_allocated;

} EVGL_Thread_Command_glUniform1i;

static void
_evgl_thread_glUniform1i(void *data)
{
   EVGL_Thread_Command_glUniform1i *thread_param =
      (EVGL_Thread_Command_glUniform1i *)data;

   glUniform1i(thread_param->location,
               thread_param->v0);

   if (thread_param->command_allocated)
     eina_mempool_free(_mp_command, thread_param);
}

EAPI void
glUniform1i_evgl_thread_cmd(GLint location, GLint v0)
{
   if (!evas_evgl_thread_enabled())
     {
        glUniform1i(location, v0);
        return;
     }

   int thread_mode = EVAS_GL_THREAD_MODE_FINISH;

   EVGL_Thread_Command_glUniform1i thread_param_local;
   EVGL_Thread_Command_glUniform1i *thread_param = &thread_param_local;

   /* command_allocated flag init. */
   thread_param->command_allocated = 0;

   if (!evas_gl_thread_force_finish())
     { /* _flush */
        EVGL_Thread_Command_glUniform1i *thread_param_new;
        thread_param_new = eina_mempool_malloc(_mp_command,
                                               sizeof(EVGL_Thread_Command_glUniform1i));
        if (thread_param_new)
          {
             thread_param = thread_param_new;
             thread_param->command_allocated = 1;
             thread_mode = EVAS_GL_THREAD_MODE_FLUSH;
          }
     }

   thread_param->location = location;
   thread_param->v0 = v0;

   evas_gl_thread_cmd_enqueue(EVAS_GL_THREAD_TYPE_EVGL,
                              _evgl_thread_glUniform1i,
                              thread_param,
                              thread_mode);
}

/*
   void
   glUniform2f(GLint location, GLfloat v0, GLfloat v1);
 */

typedef struct
{
   GLint location;
   GLfloat v0;
   GLfloat v1;
   int command_allocated;

} EVGL_Thread_Command_glUniform2f;

static void
_evgl_thread_glUniform2f(void *data)
{
   EVGL_Thread_Command_glUniform2f *thread_param =
      (EVGL_Thread_Command_glUniform2f *)data;

   glUniform2f(thread_param->location,
               thread_param->v0,
               thread_param->v1);

   if (thread_param->command_allocated)
     eina_mempool_free(_mp_command, thread_param);
}

EAPI void
glUniform2f_evgl_thread_cmd(GLint location, GLfloat v0, GLfloat v1)
{
   if (!evas_evgl_thread_enabled())
     {
        glUniform2f(location, v0, v1);
        return;
     }

   int thread_mode = EVAS_GL_THREAD_MODE_FINISH;

   EVGL_Thread_Command_glUniform2f thread_param_local;
   EVGL_Thread_Command_glUniform2f *thread_param = &thread_param_local;

   /* command_allocated flag init. */
   thread_param->command_allocated = 0;

   if (!evas_gl_thread_force_finish())
     { /* _flush */
        EVGL_Thread_Command_glUniform2f *thread_param_new;
        thread_param_new = eina_mempool_malloc(_mp_command,
                                               sizeof(EVGL_Thread_Command_glUniform2f));
        if (thread_param_new)
          {
             thread_param = thread_param_new;
             thread_param->command_allocated = 1;
             thread_mode = EVAS_GL_THREAD_MODE_FLUSH;
          }
     }

   thread_param->location = location;
   thread_param->v0 = v0;
   thread_param->v1 = v1;

   evas_gl_thread_cmd_enqueue(EVAS_GL_THREAD_TYPE_EVGL,
                              _evgl_thread_glUniform2f,
                              thread_param,
                              thread_mode);
}

/*
   void
   glUniform2i(GLint location, GLint v0, GLint v1);
 */

typedef struct
{
   GLint location;
   GLint v0;
   GLint v1;
   int command_allocated;

} EVGL_Thread_Command_glUniform2i;

static void
_evgl_thread_glUniform2i(void *data)
{
   EVGL_Thread_Command_glUniform2i *thread_param =
      (EVGL_Thread_Command_glUniform2i *)data;

   glUniform2i(thread_param->location,
               thread_param->v0,
               thread_param->v1);

   if (thread_param->command_allocated)
     eina_mempool_free(_mp_command, thread_param);
}

EAPI void
glUniform2i_evgl_thread_cmd(GLint location, GLint v0, GLint v1)
{
   if (!evas_evgl_thread_enabled())
     {
        glUniform2i(location, v0, v1);
        return;
     }

   int thread_mode = EVAS_GL_THREAD_MODE_FINISH;

   EVGL_Thread_Command_glUniform2i thread_param_local;
   EVGL_Thread_Command_glUniform2i *thread_param = &thread_param_local;

   /* command_allocated flag init. */
   thread_param->command_allocated = 0;

   if (!evas_gl_thread_force_finish())
     { /* _flush */
        EVGL_Thread_Command_glUniform2i *thread_param_new;
        thread_param_new = eina_mempool_malloc(_mp_command,
                                               sizeof(EVGL_Thread_Command_glUniform2i));
        if (thread_param_new)
          {
             thread_param = thread_param_new;
             thread_param->command_allocated = 1;
             thread_mode = EVAS_GL_THREAD_MODE_FLUSH;
          }
     }

   thread_param->location = location;
   thread_param->v0 = v0;
   thread_param->v1 = v1;

   evas_gl_thread_cmd_enqueue(EVAS_GL_THREAD_TYPE_EVGL,
                              _evgl_thread_glUniform2i,
                              thread_param,
                              thread_mode);
}

/*
   void
   glUniform3f(GLint location, GLfloat v0, GLfloat v1, GLfloat v2);
 */

typedef struct
{
   GLint location;
   GLfloat v0;
   GLfloat v1;
   GLfloat v2;
   int command_allocated;

} EVGL_Thread_Command_glUniform3f;

static void
_evgl_thread_glUniform3f(void *data)
{
   EVGL_Thread_Command_glUniform3f *thread_param =
      (EVGL_Thread_Command_glUniform3f *)data;

   glUniform3f(thread_param->location,
               thread_param->v0,
               thread_param->v1,
               thread_param->v2);

   if (thread_param->command_allocated)
     eina_mempool_free(_mp_command, thread_param);
}

EAPI void
glUniform3f_evgl_thread_cmd(GLint location, GLfloat v0, GLfloat v1, GLfloat v2)
{
   if (!evas_evgl_thread_enabled())
     {
        glUniform3f(location, v0, v1, v2);
        return;
     }

   int thread_mode = EVAS_GL_THREAD_MODE_FINISH;

   EVGL_Thread_Command_glUniform3f thread_param_local;
   EVGL_Thread_Command_glUniform3f *thread_param = &thread_param_local;

   /* command_allocated flag init. */
   thread_param->command_allocated = 0;

   if (!evas_gl_thread_force_finish())
     { /* _flush */
        EVGL_Thread_Command_glUniform3f *thread_param_new;
        thread_param_new = eina_mempool_malloc(_mp_command,
                                               sizeof(EVGL_Thread_Command_glUniform3f));
        if (thread_param_new)
          {
             thread_param = thread_param_new;
             thread_param->command_allocated = 1;
             thread_mode = EVAS_GL_THREAD_MODE_FLUSH;
          }
     }

   thread_param->location = location;
   thread_param->v0 = v0;
   thread_param->v1 = v1;
   thread_param->v2 = v2;

   evas_gl_thread_cmd_enqueue(EVAS_GL_THREAD_TYPE_EVGL,
                              _evgl_thread_glUniform3f,
                              thread_param,
                              thread_mode);
}

/*
   void
   glUniform3i(GLint location, GLint v0, GLint v1, GLint v2);
 */

typedef struct
{
   GLint location;
   GLint v0;
   GLint v1;
   GLint v2;
   int command_allocated;

} EVGL_Thread_Command_glUniform3i;

static void
_evgl_thread_glUniform3i(void *data)
{
   EVGL_Thread_Command_glUniform3i *thread_param =
      (EVGL_Thread_Command_glUniform3i *)data;

   glUniform3i(thread_param->location,
               thread_param->v0,
               thread_param->v1,
               thread_param->v2);

   if (thread_param->command_allocated)
     eina_mempool_free(_mp_command, thread_param);
}

EAPI void
glUniform3i_evgl_thread_cmd(GLint location, GLint v0, GLint v1, GLint v2)
{
   if (!evas_evgl_thread_enabled())
     {
        glUniform3i(location, v0, v1, v2);
        return;
     }

   int thread_mode = EVAS_GL_THREAD_MODE_FINISH;

   EVGL_Thread_Command_glUniform3i thread_param_local;
   EVGL_Thread_Command_glUniform3i *thread_param = &thread_param_local;

   /* command_allocated flag init. */
   thread_param->command_allocated = 0;

   if (!evas_gl_thread_force_finish())
     { /* _flush */
        EVGL_Thread_Command_glUniform3i *thread_param_new;
        thread_param_new = eina_mempool_malloc(_mp_command,
                                               sizeof(EVGL_Thread_Command_glUniform3i));
        if (thread_param_new)
          {
             thread_param = thread_param_new;
             thread_param->command_allocated = 1;
             thread_mode = EVAS_GL_THREAD_MODE_FLUSH;
          }
     }

   thread_param->location = location;
   thread_param->v0 = v0;
   thread_param->v1 = v1;
   thread_param->v2 = v2;

   evas_gl_thread_cmd_enqueue(EVAS_GL_THREAD_TYPE_EVGL,
                              _evgl_thread_glUniform3i,
                              thread_param,
                              thread_mode);
}

/*
   void
   glUniform4f(GLint location, GLfloat v0, GLfloat v1, GLfloat v2, GLfloat v3);
 */

typedef struct
{
   GLint location;
   GLfloat v0;
   GLfloat v1;
   GLfloat v2;
   GLfloat v3;
   int command_allocated;

} EVGL_Thread_Command_glUniform4f;

static void
_evgl_thread_glUniform4f(void *data)
{
   EVGL_Thread_Command_glUniform4f *thread_param =
      (EVGL_Thread_Command_glUniform4f *)data;

   glUniform4f(thread_param->location,
               thread_param->v0,
               thread_param->v1,
               thread_param->v2,
               thread_param->v3);

   if (thread_param->command_allocated)
     eina_mempool_free(_mp_command, thread_param);
}

EAPI void
glUniform4f_evgl_thread_cmd(GLint location, GLfloat v0, GLfloat v1, GLfloat v2, GLfloat v3)
{
   if (!evas_evgl_thread_enabled())
     {
        glUniform4f(location, v0, v1, v2, v3);
        return;
     }

   int thread_mode = EVAS_GL_THREAD_MODE_FINISH;

   EVGL_Thread_Command_glUniform4f thread_param_local;
   EVGL_Thread_Command_glUniform4f *thread_param = &thread_param_local;

   /* command_allocated flag init. */
   thread_param->command_allocated = 0;

   if (!evas_gl_thread_force_finish())
     { /* _flush */
        EVGL_Thread_Command_glUniform4f *thread_param_new;
        thread_param_new = eina_mempool_malloc(_mp_command,
                                               sizeof(EVGL_Thread_Command_glUniform4f));
        if (thread_param_new)
          {
             thread_param = thread_param_new;
             thread_param->command_allocated = 1;
             thread_mode = EVAS_GL_THREAD_MODE_FLUSH;
          }
     }

   thread_param->location = location;
   thread_param->v0 = v0;
   thread_param->v1 = v1;
   thread_param->v2 = v2;
   thread_param->v3 = v3;

   evas_gl_thread_cmd_enqueue(EVAS_GL_THREAD_TYPE_EVGL,
                              _evgl_thread_glUniform4f,
                              thread_param,
                              thread_mode);
}

/*
   void
   glUniform4i(GLint location, GLint v0, GLint v1, GLint v2, GLint v3);
 */

typedef struct
{
   GLint location;
   GLint v0;
   GLint v1;
   GLint v2;
   GLint v3;
   int command_allocated;

} EVGL_Thread_Command_glUniform4i;

static void
_evgl_thread_glUniform4i(void *data)
{
   EVGL_Thread_Command_glUniform4i *thread_param =
      (EVGL_Thread_Command_glUniform4i *)data;

   glUniform4i(thread_param->location,
               thread_param->v0,
               thread_param->v1,
               thread_param->v2,
               thread_param->v3);

   if (thread_param->command_allocated)
     eina_mempool_free(_mp_command, thread_param);
}

EAPI void
glUniform4i_evgl_thread_cmd(GLint location, GLint v0, GLint v1, GLint v2, GLint v3)
{
   if (!evas_evgl_thread_enabled())
     {
        glUniform4i(location, v0, v1, v2, v3);
        return;
     }

   int thread_mode = EVAS_GL_THREAD_MODE_FINISH;

   EVGL_Thread_Command_glUniform4i thread_param_local;
   EVGL_Thread_Command_glUniform4i *thread_param = &thread_param_local;

   /* command_allocated flag init. */
   thread_param->command_allocated = 0;

   if (!evas_gl_thread_force_finish())
     { /* _flush */
        EVGL_Thread_Command_glUniform4i *thread_param_new;
        thread_param_new = eina_mempool_malloc(_mp_command,
                                               sizeof(EVGL_Thread_Command_glUniform4i));
        if (thread_param_new)
          {
             thread_param = thread_param_new;
             thread_param->command_allocated = 1;
             thread_mode = EVAS_GL_THREAD_MODE_FLUSH;
          }
     }

   thread_param->location = location;
   thread_param->v0 = v0;
   thread_param->v1 = v1;
   thread_param->v2 = v2;
   thread_param->v3 = v3;

   evas_gl_thread_cmd_enqueue(EVAS_GL_THREAD_TYPE_EVGL,
                              _evgl_thread_glUniform4i,
                              thread_param,
                              thread_mode);
}

/*
   void
   glUniform1fv(GLint location, GLsizei count, const GLfloat *value);
 */

typedef struct
{
   GLint location;
   GLsizei count;
   const GLfloat *value;
   void *value_copied; /* COPIED */
   int command_allocated;

} EVGL_Thread_Command_glUniform1fv;

static void
_evgl_thread_glUniform1fv(void *data)
{
   EVGL_Thread_Command_glUniform1fv *thread_param =
      (EVGL_Thread_Command_glUniform1fv *)data;

   glUniform1fv(thread_param->location,
                thread_param->count,
                thread_param->value);


   if (thread_param->value_copied)
     eina_mempool_free(_mp_uniform, thread_param->value_copied);

   if (thread_param->command_allocated)
     eina_mempool_free(_mp_command, thread_param);
}

EAPI void
glUniform1fv_evgl_thread_cmd(GLint location, GLsizei count, const GLfloat *value)
{
   if (!evas_evgl_thread_enabled())
     {
        glUniform1fv(location, count, value);
        return;
     }

   int thread_mode = EVAS_GL_THREAD_MODE_FINISH;

   EVGL_Thread_Command_glUniform1fv thread_param_local;
   EVGL_Thread_Command_glUniform1fv *thread_param = &thread_param_local;

   /* command_allocated flag init. */
   thread_param->command_allocated = 0;

   if (!evas_gl_thread_force_finish())
     { /* _flush */
        EVGL_Thread_Command_glUniform1fv *thread_param_new;
        thread_param_new = eina_mempool_malloc(_mp_command,
                                               sizeof(EVGL_Thread_Command_glUniform1fv));
        if (thread_param_new)
          {
             thread_param = thread_param_new;
             thread_param->command_allocated = 1;
             thread_mode = EVAS_GL_THREAD_MODE_FLUSH;
          }
     }

   thread_param->location = location;
   thread_param->count = count;
   thread_param->value = value;

   thread_param->value_copied = NULL;
   if (thread_mode == EVAS_GL_THREAD_MODE_FINISH)
     goto finish;

   /* copy variable */
   if (value)
     {
        /* 1. check memory size */
        unsigned int copy_size = 1 * count * sizeof(GLfloat);
        if (copy_size > _mp_uniform_memory_size)
          {
             thread_mode = EVAS_GL_THREAD_MODE_FINISH;
             goto finish;
          }
        /* 2. malloc & copy */
        thread_param->value_copied = eina_mempool_malloc(_mp_uniform, copy_size);
        if (thread_param->value_copied)
          {
             memcpy(thread_param->value_copied, value, copy_size);
          }
        else
          {
             thread_mode = EVAS_GL_THREAD_MODE_FINISH;
             goto finish;
          }
        /* 3. replace */
        thread_param->value = (const GLfloat  *)thread_param->value_copied;
     }
   /* end of copy variable */

finish:
   evas_gl_thread_cmd_enqueue(EVAS_GL_THREAD_TYPE_EVGL,
                              _evgl_thread_glUniform1fv,
                              thread_param,
                              thread_mode);
}

/*
   void
   glUniform1iv(GLint location, GLsizei count, const GLint *value);
 */

typedef struct
{
   GLint location;
   GLsizei count;
   const GLint *value;
   void *value_copied; /* COPIED */
   int command_allocated;

} EVGL_Thread_Command_glUniform1iv;

static void
_evgl_thread_glUniform1iv(void *data)
{
   EVGL_Thread_Command_glUniform1iv *thread_param =
      (EVGL_Thread_Command_glUniform1iv *)data;

   glUniform1iv(thread_param->location,
                thread_param->count,
                thread_param->value);


   if (thread_param->value_copied)
     eina_mempool_free(_mp_uniform, thread_param->value_copied);

   if (thread_param->command_allocated)
     eina_mempool_free(_mp_command, thread_param);
}

EAPI void
glUniform1iv_evgl_thread_cmd(GLint location, GLsizei count, const GLint *value)
{
   if (!evas_evgl_thread_enabled())
     {
        glUniform1iv(location, count, value);
        return;
     }

   int thread_mode = EVAS_GL_THREAD_MODE_FINISH;

   EVGL_Thread_Command_glUniform1iv thread_param_local;
   EVGL_Thread_Command_glUniform1iv *thread_param = &thread_param_local;

   /* command_allocated flag init. */
   thread_param->command_allocated = 0;

   if (!evas_gl_thread_force_finish())
     { /* _flush */
        EVGL_Thread_Command_glUniform1iv *thread_param_new;
        thread_param_new = eina_mempool_malloc(_mp_command,
                                               sizeof(EVGL_Thread_Command_glUniform1iv));
        if (thread_param_new)
          {
             thread_param = thread_param_new;
             thread_param->command_allocated = 1;
             thread_mode = EVAS_GL_THREAD_MODE_FLUSH;
          }
     }

   thread_param->location = location;
   thread_param->count = count;
   thread_param->value = value;

   thread_param->value_copied = NULL;
   if (thread_mode == EVAS_GL_THREAD_MODE_FINISH)
     goto finish;

   /* copy variable */
   if (value)
     {
        /* 1. check memory size */
        unsigned int copy_size = 1 * count * sizeof(GLint);
        if (copy_size > _mp_uniform_memory_size)
          {
             thread_mode = EVAS_GL_THREAD_MODE_FINISH;
             goto finish;
          }
        /* 2. malloc & copy */
        thread_param->value_copied = eina_mempool_malloc(_mp_uniform, copy_size);
        if (thread_param->value_copied)
          {
             memcpy(thread_param->value_copied, value, copy_size);
          }
        else
          {
             thread_mode = EVAS_GL_THREAD_MODE_FINISH;
             goto finish;
          }
        /* 3. replace */
        thread_param->value = (const GLint  *)thread_param->value_copied;
     }
   /* end of copy variable */

finish:
   evas_gl_thread_cmd_enqueue(EVAS_GL_THREAD_TYPE_EVGL,
                              _evgl_thread_glUniform1iv,
                              thread_param,
                              thread_mode);
}

/*
   void
   glUniform2fv(GLint location, GLsizei count, const GLfloat *value);
 */

typedef struct
{
   GLint location;
   GLsizei count;
   const GLfloat *value;
   void *value_copied; /* COPIED */
   int command_allocated;

} EVGL_Thread_Command_glUniform2fv;

static void
_evgl_thread_glUniform2fv(void *data)
{
   EVGL_Thread_Command_glUniform2fv *thread_param =
      (EVGL_Thread_Command_glUniform2fv *)data;

   glUniform2fv(thread_param->location,
                thread_param->count,
                thread_param->value);


   if (thread_param->value_copied)
     eina_mempool_free(_mp_uniform, thread_param->value_copied);

   if (thread_param->command_allocated)
     eina_mempool_free(_mp_command, thread_param);
}

EAPI void
glUniform2fv_evgl_thread_cmd(GLint location, GLsizei count, const GLfloat *value)
{
   if (!evas_evgl_thread_enabled())
     {
        glUniform2fv(location, count, value);
        return;
     }

   int thread_mode = EVAS_GL_THREAD_MODE_FINISH;

   EVGL_Thread_Command_glUniform2fv thread_param_local;
   EVGL_Thread_Command_glUniform2fv *thread_param = &thread_param_local;

   /* command_allocated flag init. */
   thread_param->command_allocated = 0;

   if (!evas_gl_thread_force_finish())
     { /* _flush */
        EVGL_Thread_Command_glUniform2fv *thread_param_new;
        thread_param_new = eina_mempool_malloc(_mp_command,
                                               sizeof(EVGL_Thread_Command_glUniform2fv));
        if (thread_param_new)
          {
             thread_param = thread_param_new;
             thread_param->command_allocated = 1;
             thread_mode = EVAS_GL_THREAD_MODE_FLUSH;
          }
     }

   thread_param->location = location;
   thread_param->count = count;
   thread_param->value = value;

   thread_param->value_copied = NULL;
   if (thread_mode == EVAS_GL_THREAD_MODE_FINISH)
     goto finish;

   /* copy variable */
   if (value)
     {
        /* 1. check memory size */
        unsigned int copy_size = 2 * count * sizeof(GLfloat);
        if (copy_size > _mp_uniform_memory_size)
          {
             thread_mode = EVAS_GL_THREAD_MODE_FINISH;
             goto finish;
          }
        /* 2. malloc & copy */
        thread_param->value_copied = eina_mempool_malloc(_mp_uniform, copy_size);
        if (thread_param->value_copied)
          {
             memcpy(thread_param->value_copied, value, copy_size);
          }
        else
          {
             thread_mode = EVAS_GL_THREAD_MODE_FINISH;
             goto finish;
          }
        /* 3. replace */
        thread_param->value = (const GLfloat  *)thread_param->value_copied;
     }
   /* end of copy variable */

finish:
   evas_gl_thread_cmd_enqueue(EVAS_GL_THREAD_TYPE_EVGL,
                              _evgl_thread_glUniform2fv,
                              thread_param,
                              thread_mode);
}

/*
   void
   glUniform2iv(GLint location, GLsizei count, const GLint *value);
 */

typedef struct
{
   GLint location;
   GLsizei count;
   const GLint *value;
   void *value_copied; /* COPIED */
   int command_allocated;

} EVGL_Thread_Command_glUniform2iv;

static void
_evgl_thread_glUniform2iv(void *data)
{
   EVGL_Thread_Command_glUniform2iv *thread_param =
      (EVGL_Thread_Command_glUniform2iv *)data;

   glUniform2iv(thread_param->location,
                thread_param->count,
                thread_param->value);


   if (thread_param->value_copied)
     eina_mempool_free(_mp_uniform, thread_param->value_copied);

   if (thread_param->command_allocated)
     eina_mempool_free(_mp_command, thread_param);
}

EAPI void
glUniform2iv_evgl_thread_cmd(GLint location, GLsizei count, const GLint *value)
{
   if (!evas_evgl_thread_enabled())
     {
        glUniform2iv(location, count, value);
        return;
     }

   int thread_mode = EVAS_GL_THREAD_MODE_FINISH;

   EVGL_Thread_Command_glUniform2iv thread_param_local;
   EVGL_Thread_Command_glUniform2iv *thread_param = &thread_param_local;

   /* command_allocated flag init. */
   thread_param->command_allocated = 0;

   if (!evas_gl_thread_force_finish())
     { /* _flush */
        EVGL_Thread_Command_glUniform2iv *thread_param_new;
        thread_param_new = eina_mempool_malloc(_mp_command,
                                               sizeof(EVGL_Thread_Command_glUniform2iv));
        if (thread_param_new)
          {
             thread_param = thread_param_new;
             thread_param->command_allocated = 1;
             thread_mode = EVAS_GL_THREAD_MODE_FLUSH;
          }
     }

   thread_param->location = location;
   thread_param->count = count;
   thread_param->value = value;

   thread_param->value_copied = NULL;
   if (thread_mode == EVAS_GL_THREAD_MODE_FINISH)
     goto finish;

   /* copy variable */
   if (value)
     {
        /* 1. check memory size */
        unsigned int copy_size = 2 * count * sizeof(GLint);
        if (copy_size > _mp_uniform_memory_size)
          {
             thread_mode = EVAS_GL_THREAD_MODE_FINISH;
             goto finish;
          }
        /* 2. malloc & copy */
        thread_param->value_copied = eina_mempool_malloc(_mp_uniform, copy_size);
        if (thread_param->value_copied)
          {
             memcpy(thread_param->value_copied, value, copy_size);
          }
        else
          {
             thread_mode = EVAS_GL_THREAD_MODE_FINISH;
             goto finish;
          }
        /* 3. replace */
        thread_param->value = (const GLint  *)thread_param->value_copied;
     }
   /* end of copy variable */

finish:
   evas_gl_thread_cmd_enqueue(EVAS_GL_THREAD_TYPE_EVGL,
                              _evgl_thread_glUniform2iv,
                              thread_param,
                              thread_mode);
}

/*
   void
   glUniform3fv(GLint location, GLsizei count, const GLfloat *value);
 */

typedef struct
{
   GLint location;
   GLsizei count;
   const GLfloat *value;
   void *value_copied; /* COPIED */
   int command_allocated;

} EVGL_Thread_Command_glUniform3fv;

static void
_evgl_thread_glUniform3fv(void *data)
{
   EVGL_Thread_Command_glUniform3fv *thread_param =
      (EVGL_Thread_Command_glUniform3fv *)data;

   glUniform3fv(thread_param->location,
                thread_param->count,
                thread_param->value);


   if (thread_param->value_copied)
     eina_mempool_free(_mp_uniform, thread_param->value_copied);

   if (thread_param->command_allocated)
     eina_mempool_free(_mp_command, thread_param);
}

EAPI void
glUniform3fv_evgl_thread_cmd(GLint location, GLsizei count, const GLfloat *value)
{
   if (!evas_evgl_thread_enabled())
     {
        glUniform3fv(location, count, value);
        return;
     }

   int thread_mode = EVAS_GL_THREAD_MODE_FINISH;

   EVGL_Thread_Command_glUniform3fv thread_param_local;
   EVGL_Thread_Command_glUniform3fv *thread_param = &thread_param_local;

   /* command_allocated flag init. */
   thread_param->command_allocated = 0;

   if (!evas_gl_thread_force_finish())
     { /* _flush */
        EVGL_Thread_Command_glUniform3fv *thread_param_new;
        thread_param_new = eina_mempool_malloc(_mp_command,
                                               sizeof(EVGL_Thread_Command_glUniform3fv));
        if (thread_param_new)
          {
             thread_param = thread_param_new;
             thread_param->command_allocated = 1;
             thread_mode = EVAS_GL_THREAD_MODE_FLUSH;
          }
     }

   thread_param->location = location;
   thread_param->count = count;
   thread_param->value = value;

   thread_param->value_copied = NULL;
   if (thread_mode == EVAS_GL_THREAD_MODE_FINISH)
     goto finish;

   /* copy variable */
   if (value)
     {
        /* 1. check memory size */
        unsigned int copy_size = 3 * count * sizeof(GLfloat);
        if (copy_size > _mp_uniform_memory_size)
          {
             thread_mode = EVAS_GL_THREAD_MODE_FINISH;
             goto finish;
          }
        /* 2. malloc & copy */
        thread_param->value_copied = eina_mempool_malloc(_mp_uniform, copy_size);
        if (thread_param->value_copied)
          {
             memcpy(thread_param->value_copied, value, copy_size);
          }
        else
          {
             thread_mode = EVAS_GL_THREAD_MODE_FINISH;
             goto finish;
          }
        /* 3. replace */
        thread_param->value = (const GLfloat  *)thread_param->value_copied;
     }
   /* end of copy variable */

finish:
   evas_gl_thread_cmd_enqueue(EVAS_GL_THREAD_TYPE_EVGL,
                              _evgl_thread_glUniform3fv,
                              thread_param,
                              thread_mode);
}

/*
   void
   glUniform3iv(GLint location, GLsizei count, const GLint *value);
 */

typedef struct
{
   GLint location;
   GLsizei count;
   const GLint *value;
   void *value_copied; /* COPIED */
   int command_allocated;

} EVGL_Thread_Command_glUniform3iv;

static void
_evgl_thread_glUniform3iv(void *data)
{
   EVGL_Thread_Command_glUniform3iv *thread_param =
      (EVGL_Thread_Command_glUniform3iv *)data;

   glUniform3iv(thread_param->location,
                thread_param->count,
                thread_param->value);


   if (thread_param->value_copied)
     eina_mempool_free(_mp_uniform, thread_param->value_copied);

   if (thread_param->command_allocated)
     eina_mempool_free(_mp_command, thread_param);
}

EAPI void
glUniform3iv_evgl_thread_cmd(GLint location, GLsizei count, const GLint *value)
{
   if (!evas_evgl_thread_enabled())
     {
        glUniform3iv(location, count, value);
        return;
     }

   int thread_mode = EVAS_GL_THREAD_MODE_FINISH;

   EVGL_Thread_Command_glUniform3iv thread_param_local;
   EVGL_Thread_Command_glUniform3iv *thread_param = &thread_param_local;

   /* command_allocated flag init. */
   thread_param->command_allocated = 0;

   if (!evas_gl_thread_force_finish())
     { /* _flush */
        EVGL_Thread_Command_glUniform3iv *thread_param_new;
        thread_param_new = eina_mempool_malloc(_mp_command,
                                               sizeof(EVGL_Thread_Command_glUniform3iv));
        if (thread_param_new)
          {
             thread_param = thread_param_new;
             thread_param->command_allocated = 1;
             thread_mode = EVAS_GL_THREAD_MODE_FLUSH;
          }
     }

   thread_param->location = location;
   thread_param->count = count;
   thread_param->value = value;

   thread_param->value_copied = NULL;
   if (thread_mode == EVAS_GL_THREAD_MODE_FINISH)
     goto finish;

   /* copy variable */
   if (value)
     {
        /* 1. check memory size */
        unsigned int copy_size = 3 * count * sizeof(GLint);
        if (copy_size > _mp_uniform_memory_size)
          {
             thread_mode = EVAS_GL_THREAD_MODE_FINISH;
             goto finish;
          }
        /* 2. malloc & copy */
        thread_param->value_copied = eina_mempool_malloc(_mp_uniform, copy_size);
        if (thread_param->value_copied)
          {
             memcpy(thread_param->value_copied, value, copy_size);
          }
        else
          {
             thread_mode = EVAS_GL_THREAD_MODE_FINISH;
             goto finish;
          }
        /* 3. replace */
        thread_param->value = (const GLint  *)thread_param->value_copied;
     }
   /* end of copy variable */

finish:
   evas_gl_thread_cmd_enqueue(EVAS_GL_THREAD_TYPE_EVGL,
                              _evgl_thread_glUniform3iv,
                              thread_param,
                              thread_mode);
}

/*
   void
   glUniform4fv(GLint location, GLsizei count, const GLfloat *value);
 */

typedef struct
{
   GLint location;
   GLsizei count;
   const GLfloat *value;
   void *value_copied; /* COPIED */
   int command_allocated;

} EVGL_Thread_Command_glUniform4fv;

static void
_evgl_thread_glUniform4fv(void *data)
{
   EVGL_Thread_Command_glUniform4fv *thread_param =
      (EVGL_Thread_Command_glUniform4fv *)data;

   glUniform4fv(thread_param->location,
                thread_param->count,
                thread_param->value);


   if (thread_param->value_copied)
     eina_mempool_free(_mp_uniform, thread_param->value_copied);

   if (thread_param->command_allocated)
     eina_mempool_free(_mp_command, thread_param);
}

EAPI void
glUniform4fv_evgl_thread_cmd(GLint location, GLsizei count, const GLfloat *value)
{
   if (!evas_evgl_thread_enabled())
     {
        glUniform4fv(location, count, value);
        return;
     }

   int thread_mode = EVAS_GL_THREAD_MODE_FINISH;

   EVGL_Thread_Command_glUniform4fv thread_param_local;
   EVGL_Thread_Command_glUniform4fv *thread_param = &thread_param_local;

   /* command_allocated flag init. */
   thread_param->command_allocated = 0;

   if (!evas_gl_thread_force_finish())
     { /* _flush */
        EVGL_Thread_Command_glUniform4fv *thread_param_new;
        thread_param_new = eina_mempool_malloc(_mp_command,
                                               sizeof(EVGL_Thread_Command_glUniform4fv));
        if (thread_param_new)
          {
             thread_param = thread_param_new;
             thread_param->command_allocated = 1;
             thread_mode = EVAS_GL_THREAD_MODE_FLUSH;
          }
     }

   thread_param->location = location;
   thread_param->count = count;
   thread_param->value = value;

   thread_param->value_copied = NULL;
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
        thread_param->value_copied = eina_mempool_malloc(_mp_uniform, copy_size);
        if (thread_param->value_copied)
          {
             memcpy(thread_param->value_copied, value, copy_size);
          }
        else
          {
             thread_mode = EVAS_GL_THREAD_MODE_FINISH;
             goto finish;
          }
        /* 3. replace */
        thread_param->value = (const GLfloat  *)thread_param->value_copied;
     }
   /* end of copy variable */

finish:
   evas_gl_thread_cmd_enqueue(EVAS_GL_THREAD_TYPE_EVGL,
                              _evgl_thread_glUniform4fv,
                              thread_param,
                              thread_mode);
}

/*
   void
   glUniform4iv(GLint location, GLsizei count, const GLint *value);
 */

typedef struct
{
   GLint location;
   GLsizei count;
   const GLint *value;
   void *value_copied; /* COPIED */
   int command_allocated;

} EVGL_Thread_Command_glUniform4iv;

static void
_evgl_thread_glUniform4iv(void *data)
{
   EVGL_Thread_Command_glUniform4iv *thread_param =
      (EVGL_Thread_Command_glUniform4iv *)data;

   glUniform4iv(thread_param->location,
                thread_param->count,
                thread_param->value);


   if (thread_param->value_copied)
     eina_mempool_free(_mp_uniform, thread_param->value_copied);

   if (thread_param->command_allocated)
     eina_mempool_free(_mp_command, thread_param);
}

EAPI void
glUniform4iv_evgl_thread_cmd(GLint location, GLsizei count, const GLint *value)
{
   if (!evas_evgl_thread_enabled())
     {
        glUniform4iv(location, count, value);
        return;
     }

   int thread_mode = EVAS_GL_THREAD_MODE_FINISH;

   EVGL_Thread_Command_glUniform4iv thread_param_local;
   EVGL_Thread_Command_glUniform4iv *thread_param = &thread_param_local;

   /* command_allocated flag init. */
   thread_param->command_allocated = 0;

   if (!evas_gl_thread_force_finish())
     { /* _flush */
        EVGL_Thread_Command_glUniform4iv *thread_param_new;
        thread_param_new = eina_mempool_malloc(_mp_command,
                                               sizeof(EVGL_Thread_Command_glUniform4iv));
        if (thread_param_new)
          {
             thread_param = thread_param_new;
             thread_param->command_allocated = 1;
             thread_mode = EVAS_GL_THREAD_MODE_FLUSH;
          }
     }

   thread_param->location = location;
   thread_param->count = count;
   thread_param->value = value;

   thread_param->value_copied = NULL;
   if (thread_mode == EVAS_GL_THREAD_MODE_FINISH)
     goto finish;

   /* copy variable */
   if (value)
     {
        /* 1. check memory size */
        unsigned int copy_size = 4 * count * sizeof(GLint);
        if (copy_size > _mp_uniform_memory_size)
          {
             thread_mode = EVAS_GL_THREAD_MODE_FINISH;
             goto finish;
          }
        /* 2. malloc & copy */
        thread_param->value_copied = eina_mempool_malloc(_mp_uniform, copy_size);
        if (thread_param->value_copied)
          {
             memcpy(thread_param->value_copied, value, copy_size);
          }
        else
          {
             thread_mode = EVAS_GL_THREAD_MODE_FINISH;
             goto finish;
          }
        /* 3. replace */
        thread_param->value = (const GLint  *)thread_param->value_copied;
     }
   /* end of copy variable */

finish:
   evas_gl_thread_cmd_enqueue(EVAS_GL_THREAD_TYPE_EVGL,
                              _evgl_thread_glUniform4iv,
                              thread_param,
                              thread_mode);
}

/*
   void
   glUniformMatrix2fv(GLint location, GLsizei count, GLboolean transpose, const GLfloat *value);
 */

typedef struct
{
   GLint location;
   GLsizei count;
   GLboolean transpose;
   const GLfloat *value;
   void *value_copied; /* COPIED */
   int command_allocated;

} EVGL_Thread_Command_glUniformMatrix2fv;

static void
_evgl_thread_glUniformMatrix2fv(void *data)
{
   EVGL_Thread_Command_glUniformMatrix2fv *thread_param =
      (EVGL_Thread_Command_glUniformMatrix2fv *)data;

   glUniformMatrix2fv(thread_param->location,
                      thread_param->count,
                      thread_param->transpose,
                      thread_param->value);


   if (thread_param->value_copied)
     eina_mempool_free(_mp_uniform, thread_param->value_copied);

   if (thread_param->command_allocated)
     eina_mempool_free(_mp_command, thread_param);
}

EAPI void
glUniformMatrix2fv_evgl_thread_cmd(GLint location, GLsizei count, GLboolean transpose, const GLfloat *value)
{
   if (!evas_evgl_thread_enabled())
     {
        glUniformMatrix2fv(location, count, transpose, value);
        return;
     }

   int thread_mode = EVAS_GL_THREAD_MODE_FINISH;

   EVGL_Thread_Command_glUniformMatrix2fv thread_param_local;
   EVGL_Thread_Command_glUniformMatrix2fv *thread_param = &thread_param_local;

   /* command_allocated flag init. */
   thread_param->command_allocated = 0;

   if (!evas_gl_thread_force_finish())
     { /* _flush */
        EVGL_Thread_Command_glUniformMatrix2fv *thread_param_new;
        thread_param_new = eina_mempool_malloc(_mp_command,
                                               sizeof(EVGL_Thread_Command_glUniformMatrix2fv));
        if (thread_param_new)
          {
             thread_param = thread_param_new;
             thread_param->command_allocated = 1;
             thread_mode = EVAS_GL_THREAD_MODE_FLUSH;
          }
     }

   thread_param->location = location;
   thread_param->count = count;
   thread_param->transpose = transpose;
   thread_param->value = value;

   thread_param->value_copied = NULL;
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
        thread_param->value_copied = eina_mempool_malloc(_mp_uniform, copy_size);
        if (thread_param->value_copied)
          {
             memcpy(thread_param->value_copied, value, copy_size);
          }
        else
          {
             thread_mode = EVAS_GL_THREAD_MODE_FINISH;
             goto finish;
          }
        /* 3. replace */
        thread_param->value = (const GLfloat  *)thread_param->value_copied;
     }
   /* end of copy variable */

finish:
   evas_gl_thread_cmd_enqueue(EVAS_GL_THREAD_TYPE_EVGL,
                              _evgl_thread_glUniformMatrix2fv,
                              thread_param,
                              thread_mode);
}

/*
   void
   glUniformMatrix3fv(GLint location, GLsizei count, GLboolean transpose, const GLfloat *value);
 */

typedef struct
{
   GLint location;
   GLsizei count;
   GLboolean transpose;
   const GLfloat *value;
   void *value_copied; /* COPIED */
   int command_allocated;

} EVGL_Thread_Command_glUniformMatrix3fv;

static void
_evgl_thread_glUniformMatrix3fv(void *data)
{
   EVGL_Thread_Command_glUniformMatrix3fv *thread_param =
      (EVGL_Thread_Command_glUniformMatrix3fv *)data;

   glUniformMatrix3fv(thread_param->location,
                      thread_param->count,
                      thread_param->transpose,
                      thread_param->value);


   if (thread_param->value_copied)
     eina_mempool_free(_mp_uniform, thread_param->value_copied);

   if (thread_param->command_allocated)
     eina_mempool_free(_mp_command, thread_param);
}

EAPI void
glUniformMatrix3fv_evgl_thread_cmd(GLint location, GLsizei count, GLboolean transpose, const GLfloat *value)
{
   if (!evas_evgl_thread_enabled())
     {
        glUniformMatrix3fv(location, count, transpose, value);
        return;
     }

   int thread_mode = EVAS_GL_THREAD_MODE_FINISH;

   EVGL_Thread_Command_glUniformMatrix3fv thread_param_local;
   EVGL_Thread_Command_glUniformMatrix3fv *thread_param = &thread_param_local;

   /* command_allocated flag init. */
   thread_param->command_allocated = 0;

   if (!evas_gl_thread_force_finish())
     { /* _flush */
        EVGL_Thread_Command_glUniformMatrix3fv *thread_param_new;
        thread_param_new = eina_mempool_malloc(_mp_command,
                                               sizeof(EVGL_Thread_Command_glUniformMatrix3fv));
        if (thread_param_new)
          {
             thread_param = thread_param_new;
             thread_param->command_allocated = 1;
             thread_mode = EVAS_GL_THREAD_MODE_FLUSH;
          }
     }

   thread_param->location = location;
   thread_param->count = count;
   thread_param->transpose = transpose;
   thread_param->value = value;

   thread_param->value_copied = NULL;
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
        thread_param->value_copied = eina_mempool_malloc(_mp_uniform, copy_size);
        if (thread_param->value_copied)
          {
             memcpy(thread_param->value_copied, value, copy_size);
          }
        else
          {
             thread_mode = EVAS_GL_THREAD_MODE_FINISH;
             goto finish;
          }
        /* 3. replace */
        thread_param->value = (const GLfloat  *)thread_param->value_copied;
     }
   /* end of copy variable */

finish:
   evas_gl_thread_cmd_enqueue(EVAS_GL_THREAD_TYPE_EVGL,
                              _evgl_thread_glUniformMatrix3fv,
                              thread_param,
                              thread_mode);
}

/*
   void
   glUniformMatrix4fv(GLint location, GLsizei count, GLboolean transpose, const GLfloat *value);
 */

typedef struct
{
   GLint location;
   GLsizei count;
   GLboolean transpose;
   const GLfloat *value;
   void *value_copied; /* COPIED */
   int command_allocated;

} EVGL_Thread_Command_glUniformMatrix4fv;

static void
_evgl_thread_glUniformMatrix4fv(void *data)
{
   EVGL_Thread_Command_glUniformMatrix4fv *thread_param =
      (EVGL_Thread_Command_glUniformMatrix4fv *)data;

   glUniformMatrix4fv(thread_param->location,
                      thread_param->count,
                      thread_param->transpose,
                      thread_param->value);


   if (thread_param->value_copied)
     eina_mempool_free(_mp_uniform, thread_param->value_copied);

   if (thread_param->command_allocated)
     eina_mempool_free(_mp_command, thread_param);
}

EAPI void
glUniformMatrix4fv_evgl_thread_cmd(GLint location, GLsizei count, GLboolean transpose, const GLfloat *value)
{
   if (!evas_evgl_thread_enabled())
     {
        glUniformMatrix4fv(location, count, transpose, value);
        return;
     }

   int thread_mode = EVAS_GL_THREAD_MODE_FINISH;

   EVGL_Thread_Command_glUniformMatrix4fv thread_param_local;
   EVGL_Thread_Command_glUniformMatrix4fv *thread_param = &thread_param_local;

   /* command_allocated flag init. */
   thread_param->command_allocated = 0;

   if (!evas_gl_thread_force_finish())
     { /* _flush */
        EVGL_Thread_Command_glUniformMatrix4fv *thread_param_new;
        thread_param_new = eina_mempool_malloc(_mp_command,
                                               sizeof(EVGL_Thread_Command_glUniformMatrix4fv));
        if (thread_param_new)
          {
             thread_param = thread_param_new;
             thread_param->command_allocated = 1;
             thread_mode = EVAS_GL_THREAD_MODE_FLUSH;
          }
     }

   thread_param->location = location;
   thread_param->count = count;
   thread_param->transpose = transpose;
   thread_param->value = value;

   thread_param->value_copied = NULL;
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
        thread_param->value_copied = eina_mempool_malloc(_mp_uniform, copy_size);
        if (thread_param->value_copied)
          {
             memcpy(thread_param->value_copied, value, copy_size);
          }
        else
          {
             thread_mode = EVAS_GL_THREAD_MODE_FINISH;
             goto finish;
          }
        /* 3. replace */
        thread_param->value = (const GLfloat  *)thread_param->value_copied;
     }
   /* end of copy variable */

finish:
   evas_gl_thread_cmd_enqueue(EVAS_GL_THREAD_TYPE_EVGL,
                              _evgl_thread_glUniformMatrix4fv,
                              thread_param,
                              thread_mode);
}

/*
   void
   glViewport(GLint x, GLint y, GLsizei width, GLsizei height);
 */

typedef struct
{
   GLint x;
   GLint y;
   GLsizei width;
   GLsizei height;
   int command_allocated;

} EVGL_Thread_Command_glViewport;

static void
_evgl_thread_glViewport(void *data)
{
   EVGL_Thread_Command_glViewport *thread_param =
      (EVGL_Thread_Command_glViewport *)data;

   glViewport(thread_param->x,
              thread_param->y,
              thread_param->width,
              thread_param->height);

   if (thread_param->command_allocated)
     eina_mempool_free(_mp_command, thread_param);
}

EAPI void
glViewport_evgl_thread_cmd(GLint x, GLint y, GLsizei width, GLsizei height)
{
   if (!evas_evgl_thread_enabled())
     {
        glViewport(x, y, width, height);
        return;
     }

   int thread_mode = EVAS_GL_THREAD_MODE_FINISH;

   EVGL_Thread_Command_glViewport thread_param_local;
   EVGL_Thread_Command_glViewport *thread_param = &thread_param_local;

   /* command_allocated flag init. */
   thread_param->command_allocated = 0;

   if (!evas_gl_thread_force_finish())
     { /* _flush */
        EVGL_Thread_Command_glViewport *thread_param_new;
        thread_param_new = eina_mempool_malloc(_mp_command,
                                               sizeof(EVGL_Thread_Command_glViewport));
        if (thread_param_new)
          {
             thread_param = thread_param_new;
             thread_param->command_allocated = 1;
             thread_mode = EVAS_GL_THREAD_MODE_FLUSH;
          }
     }

   thread_param->x = x;
   thread_param->y = y;
   thread_param->width = width;
   thread_param->height = height;

   evas_gl_thread_cmd_enqueue(EVAS_GL_THREAD_TYPE_EVGL,
                              _evgl_thread_glViewport,
                              thread_param,
                              thread_mode);
}

/*
   void
   glEnable(GLenum cap);
 */

typedef struct
{
   GLenum cap;
   int command_allocated;

} EVGL_Thread_Command_glEnable;

static void
_evgl_thread_glEnable(void *data)
{
   EVGL_Thread_Command_glEnable *thread_param =
      (EVGL_Thread_Command_glEnable *)data;

   glEnable(thread_param->cap);

   if (thread_param->command_allocated)
     eina_mempool_free(_mp_command, thread_param);
}

EAPI void
glEnable_evgl_thread_cmd(GLenum cap)
{
   if (!evas_evgl_thread_enabled())
     {
        glEnable(cap);
        return;
     }

   int thread_mode = EVAS_GL_THREAD_MODE_FINISH;

   EVGL_Thread_Command_glEnable thread_param_local;
   EVGL_Thread_Command_glEnable *thread_param = &thread_param_local;

   /* command_allocated flag init. */
   thread_param->command_allocated = 0;

   if (!evas_gl_thread_force_finish())
     { /* _flush */
        EVGL_Thread_Command_glEnable *thread_param_new;
        thread_param_new = eina_mempool_malloc(_mp_command,
                                               sizeof(EVGL_Thread_Command_glEnable));
        if (thread_param_new)
          {
             thread_param = thread_param_new;
             thread_param->command_allocated = 1;
             thread_mode = EVAS_GL_THREAD_MODE_FLUSH;
          }
     }

   thread_param->cap = cap;

   evas_gl_thread_cmd_enqueue(EVAS_GL_THREAD_TYPE_EVGL,
                              _evgl_thread_glEnable,
                              thread_param,
                              thread_mode);
}

/*
   void
   glDisable(GLenum cap);
 */

typedef struct
{
   GLenum cap;
   int command_allocated;

} EVGL_Thread_Command_glDisable;

static void
_evgl_thread_glDisable(void *data)
{
   EVGL_Thread_Command_glDisable *thread_param =
      (EVGL_Thread_Command_glDisable *)data;

   glDisable(thread_param->cap);

   if (thread_param->command_allocated)
     eina_mempool_free(_mp_command, thread_param);
}

EAPI void
glDisable_evgl_thread_cmd(GLenum cap)
{
   if (!evas_evgl_thread_enabled())
     {
        glDisable(cap);
        return;
     }

   int thread_mode = EVAS_GL_THREAD_MODE_FINISH;

   EVGL_Thread_Command_glDisable thread_param_local;
   EVGL_Thread_Command_glDisable *thread_param = &thread_param_local;

   /* command_allocated flag init. */
   thread_param->command_allocated = 0;

   if (!evas_gl_thread_force_finish())
     { /* _flush */
        EVGL_Thread_Command_glDisable *thread_param_new;
        thread_param_new = eina_mempool_malloc(_mp_command,
                                               sizeof(EVGL_Thread_Command_glDisable));
        if (thread_param_new)
          {
             thread_param = thread_param_new;
             thread_param->command_allocated = 1;
             thread_mode = EVAS_GL_THREAD_MODE_FLUSH;
          }
     }

   thread_param->cap = cap;

   evas_gl_thread_cmd_enqueue(EVAS_GL_THREAD_TYPE_EVGL,
                              _evgl_thread_glDisable,
                              thread_param,
                              thread_mode);
}

/*
   void
   glLineWidth(GLfloat width);
 */

typedef struct
{
   GLfloat width;
   int command_allocated;

} EVGL_Thread_Command_glLineWidth;

static void
_evgl_thread_glLineWidth(void *data)
{
   EVGL_Thread_Command_glLineWidth *thread_param =
      (EVGL_Thread_Command_glLineWidth *)data;

   glLineWidth(thread_param->width);

   if (thread_param->command_allocated)
     eina_mempool_free(_mp_command, thread_param);
}

EAPI void
glLineWidth_evgl_thread_cmd(GLfloat width)
{
   if (!evas_evgl_thread_enabled())
     {
        glLineWidth(width);
        return;
     }

   int thread_mode = EVAS_GL_THREAD_MODE_FINISH;

   EVGL_Thread_Command_glLineWidth thread_param_local;
   EVGL_Thread_Command_glLineWidth *thread_param = &thread_param_local;

   /* command_allocated flag init. */
   thread_param->command_allocated = 0;

   if (!evas_gl_thread_force_finish())
     { /* _flush */
        EVGL_Thread_Command_glLineWidth *thread_param_new;
        thread_param_new = eina_mempool_malloc(_mp_command,
                                               sizeof(EVGL_Thread_Command_glLineWidth));
        if (thread_param_new)
          {
             thread_param = thread_param_new;
             thread_param->command_allocated = 1;
             thread_mode = EVAS_GL_THREAD_MODE_FLUSH;
          }
     }

   thread_param->width = width;

   evas_gl_thread_cmd_enqueue(EVAS_GL_THREAD_TYPE_EVGL,
                              _evgl_thread_glLineWidth,
                              thread_param,
                              thread_mode);
}

/*
   void
   glPolygonOffset(GLfloat factor, GLfloat units);
 */

typedef struct
{
   GLfloat factor;
   GLfloat units;
   int command_allocated;

} EVGL_Thread_Command_glPolygonOffset;

static void
_evgl_thread_glPolygonOffset(void *data)
{
   EVGL_Thread_Command_glPolygonOffset *thread_param =
      (EVGL_Thread_Command_glPolygonOffset *)data;

   glPolygonOffset(thread_param->factor,
                   thread_param->units);

   if (thread_param->command_allocated)
     eina_mempool_free(_mp_command, thread_param);
}

EAPI void
glPolygonOffset_evgl_thread_cmd(GLfloat factor, GLfloat units)
{
   if (!evas_evgl_thread_enabled())
     {
        glPolygonOffset(factor, units);
        return;
     }

   int thread_mode = EVAS_GL_THREAD_MODE_FINISH;

   EVGL_Thread_Command_glPolygonOffset thread_param_local;
   EVGL_Thread_Command_glPolygonOffset *thread_param = &thread_param_local;

   /* command_allocated flag init. */
   thread_param->command_allocated = 0;

   if (!evas_gl_thread_force_finish())
     { /* _flush */
        EVGL_Thread_Command_glPolygonOffset *thread_param_new;
        thread_param_new = eina_mempool_malloc(_mp_command,
                                               sizeof(EVGL_Thread_Command_glPolygonOffset));
        if (thread_param_new)
          {
             thread_param = thread_param_new;
             thread_param->command_allocated = 1;
             thread_mode = EVAS_GL_THREAD_MODE_FLUSH;
          }
     }

   thread_param->factor = factor;
   thread_param->units = units;

   evas_gl_thread_cmd_enqueue(EVAS_GL_THREAD_TYPE_EVGL,
                              _evgl_thread_glPolygonOffset,
                              thread_param,
                              thread_mode);
}

/*
   void
   glPixelStorei(GLenum pname, GLint param);
 */

typedef struct
{
   GLenum pname;
   GLint param;
   int command_allocated;

} EVGL_Thread_Command_glPixelStorei;

static void
_evgl_thread_glPixelStorei(void *data)
{
   EVGL_Thread_Command_glPixelStorei *thread_param =
      (EVGL_Thread_Command_glPixelStorei *)data;

   glPixelStorei(thread_param->pname,
                 thread_param->param);

   if (thread_param->command_allocated)
     eina_mempool_free(_mp_command, thread_param);
}

EAPI void
glPixelStorei_evgl_thread_cmd(GLenum pname, GLint param)
{
   if (!evas_evgl_thread_enabled())
     {
        glPixelStorei(pname, param);
        return;
     }

   int thread_mode = EVAS_GL_THREAD_MODE_FINISH;

   EVGL_Thread_Command_glPixelStorei thread_param_local;
   EVGL_Thread_Command_glPixelStorei *thread_param = &thread_param_local;

   /* command_allocated flag init. */
   thread_param->command_allocated = 0;

   if (!evas_gl_thread_force_finish())
     { /* _flush */
        EVGL_Thread_Command_glPixelStorei *thread_param_new;
        thread_param_new = eina_mempool_malloc(_mp_command,
                                               sizeof(EVGL_Thread_Command_glPixelStorei));
        if (thread_param_new)
          {
             thread_param = thread_param_new;
             thread_param->command_allocated = 1;
             thread_mode = EVAS_GL_THREAD_MODE_FLUSH;
          }
     }

   thread_param->pname = pname;
   thread_param->param = param;

   evas_gl_thread_cmd_enqueue(EVAS_GL_THREAD_TYPE_EVGL,
                              _evgl_thread_glPixelStorei,
                              thread_param,
                              thread_mode);
}

/*
   void
   glActiveTexture(GLenum texture);
 */

typedef struct
{
   GLenum texture;
   int command_allocated;

} EVGL_Thread_Command_glActiveTexture;

static void
_evgl_thread_glActiveTexture(void *data)
{
   EVGL_Thread_Command_glActiveTexture *thread_param =
      (EVGL_Thread_Command_glActiveTexture *)data;

   glActiveTexture(thread_param->texture);

   if (thread_param->command_allocated)
     eina_mempool_free(_mp_command, thread_param);
}

EAPI void
glActiveTexture_evgl_thread_cmd(GLenum texture)
{
   if (!evas_evgl_thread_enabled())
     {
        glActiveTexture(texture);
        return;
     }

   int thread_mode = EVAS_GL_THREAD_MODE_FINISH;

   EVGL_Thread_Command_glActiveTexture thread_param_local;
   EVGL_Thread_Command_glActiveTexture *thread_param = &thread_param_local;

   /* command_allocated flag init. */
   thread_param->command_allocated = 0;

   if (!evas_gl_thread_force_finish())
     { /* _flush */
        EVGL_Thread_Command_glActiveTexture *thread_param_new;
        thread_param_new = eina_mempool_malloc(_mp_command,
                                               sizeof(EVGL_Thread_Command_glActiveTexture));
        if (thread_param_new)
          {
             thread_param = thread_param_new;
             thread_param->command_allocated = 1;
             thread_mode = EVAS_GL_THREAD_MODE_FLUSH;
          }
     }

   thread_param->texture = texture;

   evas_gl_thread_cmd_enqueue(EVAS_GL_THREAD_TYPE_EVGL,
                              _evgl_thread_glActiveTexture,
                              thread_param,
                              thread_mode);
}

/*
   void
   glGenTextures(GLsizei n, GLuint *textures);
 */

typedef struct
{
   GLsizei n;
   GLuint *textures;

} EVGL_Thread_Command_glGenTextures;

static void
_evgl_thread_glGenTextures(void *data)
{
   EVGL_Thread_Command_glGenTextures *thread_param =
      (EVGL_Thread_Command_glGenTextures *)data;

   glGenTextures(thread_param->n,
                 thread_param->textures);

}

EAPI void
glGenTextures_evgl_thread_cmd(GLsizei n, GLuint *textures)
{
   if (!evas_evgl_thread_enabled())
     {
        glGenTextures(n, textures);
        return;
     }

   int thread_mode = EVAS_GL_THREAD_MODE_FINISH;

   EVGL_Thread_Command_glGenTextures thread_param_local;
   EVGL_Thread_Command_glGenTextures *thread_param = &thread_param_local;

   thread_param->n = n;
   thread_param->textures = textures;

   evas_gl_thread_cmd_enqueue(EVAS_GL_THREAD_TYPE_EVGL,
                              _evgl_thread_glGenTextures,
                              thread_param,
                              thread_mode);
}

/*
   void
   glBindTexture(GLenum target, GLuint texture);
 */

typedef struct
{
   GLenum target;
   GLuint texture;
   int command_allocated;

} EVGL_Thread_Command_glBindTexture;

static void
_evgl_thread_glBindTexture(void *data)
{
   EVGL_Thread_Command_glBindTexture *thread_param =
      (EVGL_Thread_Command_glBindTexture *)data;

   glBindTexture(thread_param->target,
                 thread_param->texture);

   if (thread_param->command_allocated)
     eina_mempool_free(_mp_command, thread_param);
}

EAPI void
glBindTexture_evgl_thread_cmd(GLenum target, GLuint texture)
{
   if (!evas_evgl_thread_enabled())
     {
        glBindTexture(target, texture);
        return;
     }

   int thread_mode = EVAS_GL_THREAD_MODE_FINISH;

   EVGL_Thread_Command_glBindTexture thread_param_local;
   EVGL_Thread_Command_glBindTexture *thread_param = &thread_param_local;

   /* command_allocated flag init. */
   thread_param->command_allocated = 0;

   if (!evas_gl_thread_force_finish())
     { /* _flush */
        EVGL_Thread_Command_glBindTexture *thread_param_new;
        thread_param_new = eina_mempool_malloc(_mp_command,
                                               sizeof(EVGL_Thread_Command_glBindTexture));
        if (thread_param_new)
          {
             thread_param = thread_param_new;
             thread_param->command_allocated = 1;
             thread_mode = EVAS_GL_THREAD_MODE_FLUSH;
          }
     }

   thread_param->target = target;
   thread_param->texture = texture;

   evas_gl_thread_cmd_enqueue(EVAS_GL_THREAD_TYPE_EVGL,
                              _evgl_thread_glBindTexture,
                              thread_param,
                              thread_mode);
}

/*
   void
   glDeleteTextures(GLsizei n, const GLuint *textures);
 */

typedef struct
{
   GLsizei n;
   const GLuint *textures;
   void *textures_copied; /* COPIED */
   int command_allocated;

} EVGL_Thread_Command_glDeleteTextures;

static void
_evgl_thread_glDeleteTextures(void *data)
{
   EVGL_Thread_Command_glDeleteTextures *thread_param =
      (EVGL_Thread_Command_glDeleteTextures *)data;

   glDeleteTextures(thread_param->n,
                    thread_param->textures);


   if (thread_param->textures_copied)
     eina_mempool_free(_mp_delete_object, thread_param->textures_copied);

   if (thread_param->command_allocated)
     eina_mempool_free(_mp_command, thread_param);
}

EAPI void
glDeleteTextures_evgl_thread_cmd(GLsizei n, const GLuint *textures)
{
   if (!evas_evgl_thread_enabled())
     {
        glDeleteTextures(n, textures);
        return;
     }

   int thread_mode = EVAS_GL_THREAD_MODE_FINISH;

   EVGL_Thread_Command_glDeleteTextures thread_param_local;
   EVGL_Thread_Command_glDeleteTextures *thread_param = &thread_param_local;

   /* command_allocated flag init. */
   thread_param->command_allocated = 0;

   if (!evas_gl_thread_force_finish())
     { /* _flush */
        EVGL_Thread_Command_glDeleteTextures *thread_param_new;
        thread_param_new = eina_mempool_malloc(_mp_command,
                                               sizeof(EVGL_Thread_Command_glDeleteTextures));
        if (thread_param_new)
          {
             thread_param = thread_param_new;
             thread_param->command_allocated = 1;
             thread_mode = EVAS_GL_THREAD_MODE_FLUSH;
          }
     }

   thread_param->n = n;
   thread_param->textures = textures;

   thread_param->textures_copied = NULL;
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
        thread_param->textures_copied = eina_mempool_malloc(_mp_delete_object, copy_size);
        if (thread_param->textures_copied)
          {
             memcpy(thread_param->textures_copied, textures, copy_size);
          }
        else
          {
             thread_mode = EVAS_GL_THREAD_MODE_FINISH;
             goto finish;
          }
        /* 3. replace */
        thread_param->textures = (const GLuint  *)thread_param->textures_copied;
     }
   /* end of copy variable */

finish:
   evas_gl_thread_cmd_enqueue(EVAS_GL_THREAD_TYPE_EVGL,
                              _evgl_thread_glDeleteTextures,
                              thread_param,
                              thread_mode);
}

/*
   void
   glTexImage2D(GLenum target, GLint level, GLint internalformat, GLsizei width, GLsizei height, GLint border, GLenum format, GLenum type, const void *pixels);
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
   const void *pixels;
   int command_allocated;
   GLTEXIMAGE2D_COPY_VARIABLE; /* TODO */

} EVGL_Thread_Command_glTexImage2D;

static void
_evgl_thread_glTexImage2D(void *data)
{
   EVGL_Thread_Command_glTexImage2D *thread_param =
      (EVGL_Thread_Command_glTexImage2D *)data;

   glTexImage2D(thread_param->target,
                thread_param->level,
                thread_param->internalformat,
                thread_param->width,
                thread_param->height,
                thread_param->border,
                thread_param->format,
                thread_param->type,
                thread_param->pixels);

   GLTEXIMAGE2D_COPY_VARIABLE_FREE; /* TODO */

   if (thread_param->command_allocated)
     eina_mempool_free(_mp_command, thread_param);
}

EAPI void
glTexImage2D_evgl_thread_cmd(GLenum target, GLint level, GLint internalformat, GLsizei width, GLsizei height, GLint border, GLenum format, GLenum type, const void *pixels)
{
   if (!evas_evgl_thread_enabled())
     {
        glTexImage2D(target, level, internalformat, width, height, border, format, type, pixels);
        return;
     }

   int thread_mode = EVAS_GL_THREAD_MODE_FINISH;

   EVGL_Thread_Command_glTexImage2D thread_param_local;
   EVGL_Thread_Command_glTexImage2D *thread_param = &thread_param_local;

   /* command_allocated flag init. */
   thread_param->command_allocated = 0;

   if (!evas_gl_thread_force_finish())
     { /* _flush */
        EVGL_Thread_Command_glTexImage2D *thread_param_new;
        thread_param_new = eina_mempool_malloc(_mp_command,
                                               sizeof(EVGL_Thread_Command_glTexImage2D));
        if (thread_param_new)
          {
             thread_param = thread_param_new;
             thread_param->command_allocated = 1;
             thread_mode = EVAS_GL_THREAD_MODE_FLUSH;
          }
     }

   thread_param->target = target;
   thread_param->level = level;
   thread_param->internalformat = internalformat;
   thread_param->width = width;
   thread_param->height = height;
   thread_param->border = border;
   thread_param->format = format;
   thread_param->type = type;
   thread_param->pixels = pixels;

   GLTEXIMAGE2D_COPY_VARIABLE_INIT; /* TODO */

   if (thread_mode == EVAS_GL_THREAD_MODE_FINISH)
     goto finish;

   GLTEXIMAGE2D_COPY_TO_MEMPOOL; /* TODO */

finish:
   evas_gl_thread_cmd_enqueue(EVAS_GL_THREAD_TYPE_EVGL,
                              _evgl_thread_glTexImage2D,
                              thread_param,
                              thread_mode);
}

/*
   void
   glTexSubImage2D(GLenum target, GLint level, GLint xoffset, GLint yoffset, GLsizei width, GLsizei height, GLenum format, GLenum type, const void *pixels);
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
   const void *pixels;
   int command_allocated;
   GLTEXSUBIMAGE2D_COPY_VARIABLE; /* TODO */

} EVGL_Thread_Command_glTexSubImage2D;

static void
_evgl_thread_glTexSubImage2D(void *data)
{
   EVGL_Thread_Command_glTexSubImage2D *thread_param =
      (EVGL_Thread_Command_glTexSubImage2D *)data;

   glTexSubImage2D(thread_param->target,
                   thread_param->level,
                   thread_param->xoffset,
                   thread_param->yoffset,
                   thread_param->width,
                   thread_param->height,
                   thread_param->format,
                   thread_param->type,
                   thread_param->pixels);

   GLTEXSUBIMAGE2D_COPY_VARIABLE_FREE; /* TODO */

   if (thread_param->command_allocated)
     eina_mempool_free(_mp_command, thread_param);
}

EAPI void
glTexSubImage2D_evgl_thread_cmd(GLenum target, GLint level, GLint xoffset, GLint yoffset, GLsizei width, GLsizei height, GLenum format, GLenum type, const void *pixels)
{
   if (!evas_evgl_thread_enabled())
     {
        glTexSubImage2D(target, level, xoffset, yoffset, width, height, format, type, pixels);
        return;
     }

   int thread_mode = EVAS_GL_THREAD_MODE_FINISH;

   EVGL_Thread_Command_glTexSubImage2D thread_param_local;
   EVGL_Thread_Command_glTexSubImage2D *thread_param = &thread_param_local;

   /* command_allocated flag init. */
   thread_param->command_allocated = 0;

   if (!evas_gl_thread_force_finish())
     { /* _flush */
        EVGL_Thread_Command_glTexSubImage2D *thread_param_new;
        thread_param_new = eina_mempool_malloc(_mp_command,
                                               sizeof(EVGL_Thread_Command_glTexSubImage2D));
        if (thread_param_new)
          {
             thread_param = thread_param_new;
             thread_param->command_allocated = 1;
             thread_mode = EVAS_GL_THREAD_MODE_FLUSH;
          }
     }

   thread_param->target = target;
   thread_param->level = level;
   thread_param->xoffset = xoffset;
   thread_param->yoffset = yoffset;
   thread_param->width = width;
   thread_param->height = height;
   thread_param->format = format;
   thread_param->type = type;
   thread_param->pixels = pixels;

   GLTEXSUBIMAGE2D_COPY_VARIABLE_INIT; /* TODO */

   if (thread_mode == EVAS_GL_THREAD_MODE_FINISH)
     goto finish;

   GLTEXSUBIMAGE2D_COPY_TO_MEMPOOL; /* TODO */

finish:
   evas_gl_thread_cmd_enqueue(EVAS_GL_THREAD_TYPE_EVGL,
                              _evgl_thread_glTexSubImage2D,
                              thread_param,
                              thread_mode);
}

/*
   void
   glCompressedTexImage2D(GLenum target, GLint level, GLenum internalformat, GLsizei width, GLsizei height, GLint border, GLsizei imageSize, const void *data);
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
   const void *data;
   int command_allocated;
   GLCOMPRESSEDTEXIMAGE2D_COPY_VARIABLE; /* TODO */

} EVGL_Thread_Command_glCompressedTexImage2D;

static void
_evgl_thread_glCompressedTexImage2D(void *data)
{
   EVGL_Thread_Command_glCompressedTexImage2D *thread_param =
      (EVGL_Thread_Command_glCompressedTexImage2D *)data;

   glCompressedTexImage2D(thread_param->target,
                          thread_param->level,
                          thread_param->internalformat,
                          thread_param->width,
                          thread_param->height,
                          thread_param->border,
                          thread_param->imageSize,
                          thread_param->data);

   GLCOMPRESSEDTEXIMAGE2D_COPY_VARIABLE_FREE; /* TODO */

   if (thread_param->command_allocated)
     eina_mempool_free(_mp_command, thread_param);
}

EAPI void
glCompressedTexImage2D_evgl_thread_cmd(GLenum target, GLint level, GLenum internalformat, GLsizei width, GLsizei height, GLint border, GLsizei imageSize, const void *data)
{
   if (!evas_evgl_thread_enabled())
     {
        glCompressedTexImage2D(target, level, internalformat, width, height, border, imageSize, data);
        return;
     }

   int thread_mode = EVAS_GL_THREAD_MODE_FINISH;

   EVGL_Thread_Command_glCompressedTexImage2D thread_param_local;
   EVGL_Thread_Command_glCompressedTexImage2D *thread_param = &thread_param_local;

   /* command_allocated flag init. */
   thread_param->command_allocated = 0;

   if (!evas_gl_thread_force_finish())
     { /* _flush */
        EVGL_Thread_Command_glCompressedTexImage2D *thread_param_new;
        thread_param_new = eina_mempool_malloc(_mp_command,
                                               sizeof(EVGL_Thread_Command_glCompressedTexImage2D));
        if (thread_param_new)
          {
             thread_param = thread_param_new;
             thread_param->command_allocated = 1;
             thread_mode = EVAS_GL_THREAD_MODE_FLUSH;
          }
     }

   thread_param->target = target;
   thread_param->level = level;
   thread_param->internalformat = internalformat;
   thread_param->width = width;
   thread_param->height = height;
   thread_param->border = border;
   thread_param->imageSize = imageSize;
   thread_param->data = data;

   GLCOMPRESSEDTEXIMAGE2D_COPY_VARIABLE_INIT; /* TODO */

   if (thread_mode == EVAS_GL_THREAD_MODE_FINISH)
     goto finish;

   GLCOMPRESSEDTEXIMAGE2D_COPY_TO_MEMPOOL; /* TODO */

finish:
   evas_gl_thread_cmd_enqueue(EVAS_GL_THREAD_TYPE_EVGL,
                              _evgl_thread_glCompressedTexImage2D,
                              thread_param,
                              thread_mode);
}

/*
   void
   glCompressedTexSubImage2D(GLenum target, GLint level, GLint xoffset, GLint yoffset, GLsizei width, GLsizei height, GLenum format, GLsizei imageSize, const void *data);
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
   const void *data;
   int command_allocated;
   GLCOMPRESSEDTEXSUBIMAGE2D_COPY_VARIABLE; /* TODO */

} EVGL_Thread_Command_glCompressedTexSubImage2D;

static void
_evgl_thread_glCompressedTexSubImage2D(void *data)
{
   EVGL_Thread_Command_glCompressedTexSubImage2D *thread_param =
      (EVGL_Thread_Command_glCompressedTexSubImage2D *)data;

   glCompressedTexSubImage2D(thread_param->target,
                             thread_param->level,
                             thread_param->xoffset,
                             thread_param->yoffset,
                             thread_param->width,
                             thread_param->height,
                             thread_param->format,
                             thread_param->imageSize,
                             thread_param->data);

   GLCOMPRESSEDTEXSUBIMAGE2D_COPY_VARIABLE_FREE; /* TODO */

   if (thread_param->command_allocated)
     eina_mempool_free(_mp_command, thread_param);
}

EAPI void
glCompressedTexSubImage2D_evgl_thread_cmd(GLenum target, GLint level, GLint xoffset, GLint yoffset, GLsizei width, GLsizei height, GLenum format, GLsizei imageSize, const void *data)
{
   if (!evas_evgl_thread_enabled())
     {
        glCompressedTexSubImage2D(target, level, xoffset, yoffset, width, height, format, imageSize, data);
        return;
     }

   int thread_mode = EVAS_GL_THREAD_MODE_FINISH;

   EVGL_Thread_Command_glCompressedTexSubImage2D thread_param_local;
   EVGL_Thread_Command_glCompressedTexSubImage2D *thread_param = &thread_param_local;

   /* command_allocated flag init. */
   thread_param->command_allocated = 0;

   if (!evas_gl_thread_force_finish())
     { /* _flush */
        EVGL_Thread_Command_glCompressedTexSubImage2D *thread_param_new;
        thread_param_new = eina_mempool_malloc(_mp_command,
                                               sizeof(EVGL_Thread_Command_glCompressedTexSubImage2D));
        if (thread_param_new)
          {
             thread_param = thread_param_new;
             thread_param->command_allocated = 1;
             thread_mode = EVAS_GL_THREAD_MODE_FLUSH;
          }
     }

   thread_param->target = target;
   thread_param->level = level;
   thread_param->xoffset = xoffset;
   thread_param->yoffset = yoffset;
   thread_param->width = width;
   thread_param->height = height;
   thread_param->format = format;
   thread_param->imageSize = imageSize;
   thread_param->data = data;

   GLCOMPRESSEDTEXSUBIMAGE2D_COPY_VARIABLE_INIT; /* TODO */

   if (thread_mode == EVAS_GL_THREAD_MODE_FINISH)
     goto finish;

   GLCOMPRESSEDTEXSUBIMAGE2D_COPY_TO_MEMPOOL; /* TODO */

finish:
   evas_gl_thread_cmd_enqueue(EVAS_GL_THREAD_TYPE_EVGL,
                              _evgl_thread_glCompressedTexSubImage2D,
                              thread_param,
                              thread_mode);
}

/*
   void
   glTexParameterf(GLenum target, GLenum pname, GLfloat param);
 */

typedef struct
{
   GLenum target;
   GLenum pname;
   GLfloat param;
   int command_allocated;

} EVGL_Thread_Command_glTexParameterf;

static void
_evgl_thread_glTexParameterf(void *data)
{
   EVGL_Thread_Command_glTexParameterf *thread_param =
      (EVGL_Thread_Command_glTexParameterf *)data;

   glTexParameterf(thread_param->target,
                   thread_param->pname,
                   thread_param->param);

   if (thread_param->command_allocated)
     eina_mempool_free(_mp_command, thread_param);
}

EAPI void
glTexParameterf_evgl_thread_cmd(GLenum target, GLenum pname, GLfloat param)
{
   if (!evas_evgl_thread_enabled())
     {
        glTexParameterf(target, pname, param);
        return;
     }

   int thread_mode = EVAS_GL_THREAD_MODE_FINISH;

   EVGL_Thread_Command_glTexParameterf thread_param_local;
   EVGL_Thread_Command_glTexParameterf *thread_param = &thread_param_local;

   /* command_allocated flag init. */
   thread_param->command_allocated = 0;

   if (!evas_gl_thread_force_finish())
     { /* _flush */
        EVGL_Thread_Command_glTexParameterf *thread_param_new;
        thread_param_new = eina_mempool_malloc(_mp_command,
                                               sizeof(EVGL_Thread_Command_glTexParameterf));
        if (thread_param_new)
          {
             thread_param = thread_param_new;
             thread_param->command_allocated = 1;
             thread_mode = EVAS_GL_THREAD_MODE_FLUSH;
          }
     }

   thread_param->target = target;
   thread_param->pname = pname;
   thread_param->param = param;

   evas_gl_thread_cmd_enqueue(EVAS_GL_THREAD_TYPE_EVGL,
                              _evgl_thread_glTexParameterf,
                              thread_param,
                              thread_mode);
}

/*
   void
   glTexParameterfv(GLenum target, GLenum pname, const GLfloat *params);
 */

typedef struct
{
   GLenum target;
   GLenum pname;
   const GLfloat *params;
   void *params_copied; /* COPIED */
   int command_allocated;

} EVGL_Thread_Command_glTexParameterfv;

static void
_evgl_thread_glTexParameterfv(void *data)
{
   EVGL_Thread_Command_glTexParameterfv *thread_param =
      (EVGL_Thread_Command_glTexParameterfv *)data;

   glTexParameterfv(thread_param->target,
                    thread_param->pname,
                    thread_param->params);


   if (thread_param->params_copied)
     eina_mempool_free(_mp_default, thread_param->params_copied);

   if (thread_param->command_allocated)
     eina_mempool_free(_mp_command, thread_param);
}

EAPI void
glTexParameterfv_evgl_thread_cmd(GLenum target, GLenum pname, const GLfloat *params)
{
   if (!evas_evgl_thread_enabled())
     {
        glTexParameterfv(target, pname, params);
        return;
     }

   int thread_mode = EVAS_GL_THREAD_MODE_FINISH;

   EVGL_Thread_Command_glTexParameterfv thread_param_local;
   EVGL_Thread_Command_glTexParameterfv *thread_param = &thread_param_local;

   /* command_allocated flag init. */
   thread_param->command_allocated = 0;

   if (!evas_gl_thread_force_finish())
     { /* _flush */
        EVGL_Thread_Command_glTexParameterfv *thread_param_new;
        thread_param_new = eina_mempool_malloc(_mp_command,
                                               sizeof(EVGL_Thread_Command_glTexParameterfv));
        if (thread_param_new)
          {
             thread_param = thread_param_new;
             thread_param->command_allocated = 1;
             thread_mode = EVAS_GL_THREAD_MODE_FLUSH;
          }
     }

   thread_param->target = target;
   thread_param->pname = pname;
   thread_param->params = params;

   thread_param->params_copied = NULL;
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
        thread_param->params_copied = eina_mempool_malloc(_mp_default, copy_size);
        if (thread_param->params_copied)
          {
             memcpy(thread_param->params_copied, params, copy_size);
          }
        else
          {
             thread_mode = EVAS_GL_THREAD_MODE_FINISH;
             goto finish;
          }
        /* 3. replace */
        thread_param->params = (const GLfloat  *)thread_param->params_copied;
     }
   /* end of copy variable */

finish:
   evas_gl_thread_cmd_enqueue(EVAS_GL_THREAD_TYPE_EVGL,
                              _evgl_thread_glTexParameterfv,
                              thread_param,
                              thread_mode);
}

/*
   void
   glTexParameteri(GLenum target, GLenum pname, GLint param);
 */

typedef struct
{
   GLenum target;
   GLenum pname;
   GLint param;
   int command_allocated;

} EVGL_Thread_Command_glTexParameteri;

static void
_evgl_thread_glTexParameteri(void *data)
{
   EVGL_Thread_Command_glTexParameteri *thread_param =
      (EVGL_Thread_Command_glTexParameteri *)data;

   glTexParameteri(thread_param->target,
                   thread_param->pname,
                   thread_param->param);

   if (thread_param->command_allocated)
     eina_mempool_free(_mp_command, thread_param);
}

EAPI void
glTexParameteri_evgl_thread_cmd(GLenum target, GLenum pname, GLint param)
{
   if (!evas_evgl_thread_enabled())
     {
        glTexParameteri(target, pname, param);
        return;
     }

   int thread_mode = EVAS_GL_THREAD_MODE_FINISH;

   EVGL_Thread_Command_glTexParameteri thread_param_local;
   EVGL_Thread_Command_glTexParameteri *thread_param = &thread_param_local;

   /* command_allocated flag init. */
   thread_param->command_allocated = 0;

   if (!evas_gl_thread_force_finish())
     { /* _flush */
        EVGL_Thread_Command_glTexParameteri *thread_param_new;
        thread_param_new = eina_mempool_malloc(_mp_command,
                                               sizeof(EVGL_Thread_Command_glTexParameteri));
        if (thread_param_new)
          {
             thread_param = thread_param_new;
             thread_param->command_allocated = 1;
             thread_mode = EVAS_GL_THREAD_MODE_FLUSH;
          }
     }

   thread_param->target = target;
   thread_param->pname = pname;
   thread_param->param = param;

   evas_gl_thread_cmd_enqueue(EVAS_GL_THREAD_TYPE_EVGL,
                              _evgl_thread_glTexParameteri,
                              thread_param,
                              thread_mode);
}

/*
   void
   glTexParameteriv(GLenum target, GLenum pname, const GLint *params);
 */

typedef struct
{
   GLenum target;
   GLenum pname;
   const GLint *params;
   void *params_copied; /* COPIED */
   int command_allocated;

} EVGL_Thread_Command_glTexParameteriv;

static void
_evgl_thread_glTexParameteriv(void *data)
{
   EVGL_Thread_Command_glTexParameteriv *thread_param =
      (EVGL_Thread_Command_glTexParameteriv *)data;

   glTexParameteriv(thread_param->target,
                    thread_param->pname,
                    thread_param->params);


   if (thread_param->params_copied)
     eina_mempool_free(_mp_default, thread_param->params_copied);

   if (thread_param->command_allocated)
     eina_mempool_free(_mp_command, thread_param);
}

EAPI void
glTexParameteriv_evgl_thread_cmd(GLenum target, GLenum pname, const GLint *params)
{
   if (!evas_evgl_thread_enabled())
     {
        glTexParameteriv(target, pname, params);
        return;
     }

   int thread_mode = EVAS_GL_THREAD_MODE_FINISH;

   EVGL_Thread_Command_glTexParameteriv thread_param_local;
   EVGL_Thread_Command_glTexParameteriv *thread_param = &thread_param_local;

   /* command_allocated flag init. */
   thread_param->command_allocated = 0;

   if (!evas_gl_thread_force_finish())
     { /* _flush */
        EVGL_Thread_Command_glTexParameteriv *thread_param_new;
        thread_param_new = eina_mempool_malloc(_mp_command,
                                               sizeof(EVGL_Thread_Command_glTexParameteriv));
        if (thread_param_new)
          {
             thread_param = thread_param_new;
             thread_param->command_allocated = 1;
             thread_mode = EVAS_GL_THREAD_MODE_FLUSH;
          }
     }

   thread_param->target = target;
   thread_param->pname = pname;
   thread_param->params = params;

   thread_param->params_copied = NULL;
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
        thread_param->params_copied = eina_mempool_malloc(_mp_default, copy_size);
        if (thread_param->params_copied)
          {
             memcpy(thread_param->params_copied, params, copy_size);
          }
        else
          {
             thread_mode = EVAS_GL_THREAD_MODE_FINISH;
             goto finish;
          }
        /* 3. replace */
        thread_param->params = (const GLint  *)thread_param->params_copied;
     }
   /* end of copy variable */

finish:
   evas_gl_thread_cmd_enqueue(EVAS_GL_THREAD_TYPE_EVGL,
                              _evgl_thread_glTexParameteriv,
                              thread_param,
                              thread_mode);
}

/*
   void
   glScissor(GLint x, GLint y, GLsizei width, GLsizei height);
 */

typedef struct
{
   GLint x;
   GLint y;
   GLsizei width;
   GLsizei height;
   int command_allocated;

} EVGL_Thread_Command_glScissor;

static void
_evgl_thread_glScissor(void *data)
{
   EVGL_Thread_Command_glScissor *thread_param =
      (EVGL_Thread_Command_glScissor *)data;

   glScissor(thread_param->x,
             thread_param->y,
             thread_param->width,
             thread_param->height);

   if (thread_param->command_allocated)
     eina_mempool_free(_mp_command, thread_param);
}

EAPI void
glScissor_evgl_thread_cmd(GLint x, GLint y, GLsizei width, GLsizei height)
{
   if (!evas_evgl_thread_enabled())
     {
        glScissor(x, y, width, height);
        return;
     }

   int thread_mode = EVAS_GL_THREAD_MODE_FINISH;

   EVGL_Thread_Command_glScissor thread_param_local;
   EVGL_Thread_Command_glScissor *thread_param = &thread_param_local;

   /* command_allocated flag init. */
   thread_param->command_allocated = 0;

   if (!evas_gl_thread_force_finish())
     { /* _flush */
        EVGL_Thread_Command_glScissor *thread_param_new;
        thread_param_new = eina_mempool_malloc(_mp_command,
                                               sizeof(EVGL_Thread_Command_glScissor));
        if (thread_param_new)
          {
             thread_param = thread_param_new;
             thread_param->command_allocated = 1;
             thread_mode = EVAS_GL_THREAD_MODE_FLUSH;
          }
     }

   thread_param->x = x;
   thread_param->y = y;
   thread_param->width = width;
   thread_param->height = height;

   evas_gl_thread_cmd_enqueue(EVAS_GL_THREAD_TYPE_EVGL,
                              _evgl_thread_glScissor,
                              thread_param,
                              thread_mode);
}

/*
   void
   glBlendFunc(GLenum sfactor, GLenum dfactor);
 */

typedef struct
{
   GLenum sfactor;
   GLenum dfactor;
   int command_allocated;

} EVGL_Thread_Command_glBlendFunc;

static void
_evgl_thread_glBlendFunc(void *data)
{
   EVGL_Thread_Command_glBlendFunc *thread_param =
      (EVGL_Thread_Command_glBlendFunc *)data;

   glBlendFunc(thread_param->sfactor,
               thread_param->dfactor);

   if (thread_param->command_allocated)
     eina_mempool_free(_mp_command, thread_param);
}

EAPI void
glBlendFunc_evgl_thread_cmd(GLenum sfactor, GLenum dfactor)
{
   if (!evas_evgl_thread_enabled())
     {
        glBlendFunc(sfactor, dfactor);
        return;
     }

   int thread_mode = EVAS_GL_THREAD_MODE_FINISH;

   EVGL_Thread_Command_glBlendFunc thread_param_local;
   EVGL_Thread_Command_glBlendFunc *thread_param = &thread_param_local;

   /* command_allocated flag init. */
   thread_param->command_allocated = 0;

   if (!evas_gl_thread_force_finish())
     { /* _flush */
        EVGL_Thread_Command_glBlendFunc *thread_param_new;
        thread_param_new = eina_mempool_malloc(_mp_command,
                                               sizeof(EVGL_Thread_Command_glBlendFunc));
        if (thread_param_new)
          {
             thread_param = thread_param_new;
             thread_param->command_allocated = 1;
             thread_mode = EVAS_GL_THREAD_MODE_FLUSH;
          }
     }

   thread_param->sfactor = sfactor;
   thread_param->dfactor = dfactor;

   evas_gl_thread_cmd_enqueue(EVAS_GL_THREAD_TYPE_EVGL,
                              _evgl_thread_glBlendFunc,
                              thread_param,
                              thread_mode);
}

/*
   void
   glBlendColor(GLfloat red, GLfloat green, GLfloat blue, GLfloat alpha);
 */

typedef struct
{
   GLfloat red;
   GLfloat green;
   GLfloat blue;
   GLfloat alpha;
   int command_allocated;

} EVGL_Thread_Command_glBlendColor;

static void
_evgl_thread_glBlendColor(void *data)
{
   EVGL_Thread_Command_glBlendColor *thread_param =
      (EVGL_Thread_Command_glBlendColor *)data;

   glBlendColor(thread_param->red,
                thread_param->green,
                thread_param->blue,
                thread_param->alpha);

   if (thread_param->command_allocated)
     eina_mempool_free(_mp_command, thread_param);
}

EAPI void
glBlendColor_evgl_thread_cmd(GLfloat red, GLfloat green, GLfloat blue, GLfloat alpha)
{
   if (!evas_evgl_thread_enabled())
     {
        glBlendColor(red, green, blue, alpha);
        return;
     }

   int thread_mode = EVAS_GL_THREAD_MODE_FINISH;

   EVGL_Thread_Command_glBlendColor thread_param_local;
   EVGL_Thread_Command_glBlendColor *thread_param = &thread_param_local;

   /* command_allocated flag init. */
   thread_param->command_allocated = 0;

   if (!evas_gl_thread_force_finish())
     { /* _flush */
        EVGL_Thread_Command_glBlendColor *thread_param_new;
        thread_param_new = eina_mempool_malloc(_mp_command,
                                               sizeof(EVGL_Thread_Command_glBlendColor));
        if (thread_param_new)
          {
             thread_param = thread_param_new;
             thread_param->command_allocated = 1;
             thread_mode = EVAS_GL_THREAD_MODE_FLUSH;
          }
     }

   thread_param->red = red;
   thread_param->green = green;
   thread_param->blue = blue;
   thread_param->alpha = alpha;

   evas_gl_thread_cmd_enqueue(EVAS_GL_THREAD_TYPE_EVGL,
                              _evgl_thread_glBlendColor,
                              thread_param,
                              thread_mode);
}

/*
   void
   glDepthMask(GLboolean flag);
 */

typedef struct
{
   GLboolean flag;
   int command_allocated;

} EVGL_Thread_Command_glDepthMask;

static void
_evgl_thread_glDepthMask(void *data)
{
   EVGL_Thread_Command_glDepthMask *thread_param =
      (EVGL_Thread_Command_glDepthMask *)data;

   glDepthMask(thread_param->flag);

   if (thread_param->command_allocated)
     eina_mempool_free(_mp_command, thread_param);
}

EAPI void
glDepthMask_evgl_thread_cmd(GLboolean flag)
{
   if (!evas_evgl_thread_enabled())
     {
        glDepthMask(flag);
        return;
     }

   int thread_mode = EVAS_GL_THREAD_MODE_FINISH;

   EVGL_Thread_Command_glDepthMask thread_param_local;
   EVGL_Thread_Command_glDepthMask *thread_param = &thread_param_local;

   /* command_allocated flag init. */
   thread_param->command_allocated = 0;

   if (!evas_gl_thread_force_finish())
     { /* _flush */
        EVGL_Thread_Command_glDepthMask *thread_param_new;
        thread_param_new = eina_mempool_malloc(_mp_command,
                                               sizeof(EVGL_Thread_Command_glDepthMask));
        if (thread_param_new)
          {
             thread_param = thread_param_new;
             thread_param->command_allocated = 1;
             thread_mode = EVAS_GL_THREAD_MODE_FLUSH;
          }
     }

   thread_param->flag = flag;

   evas_gl_thread_cmd_enqueue(EVAS_GL_THREAD_TYPE_EVGL,
                              _evgl_thread_glDepthMask,
                              thread_param,
                              thread_mode);
}

/*
   void
   glClear(GLbitfield mask);
 */

typedef struct
{
   GLbitfield mask;
   int command_allocated;

} EVGL_Thread_Command_glClear;

static void
_evgl_thread_glClear(void *data)
{
   EVGL_Thread_Command_glClear *thread_param =
      (EVGL_Thread_Command_glClear *)data;

   glClear(thread_param->mask);

   if (thread_param->command_allocated)
     eina_mempool_free(_mp_command, thread_param);
}

EAPI void
glClear_evgl_thread_cmd(GLbitfield mask)
{
   if (!evas_evgl_thread_enabled())
     {
        glClear(mask);
        return;
     }

   int thread_mode = EVAS_GL_THREAD_MODE_FINISH;

   EVGL_Thread_Command_glClear thread_param_local;
   EVGL_Thread_Command_glClear *thread_param = &thread_param_local;

   /* command_allocated flag init. */
   thread_param->command_allocated = 0;

   if (!evas_gl_thread_force_finish())
     { /* _flush */
        EVGL_Thread_Command_glClear *thread_param_new;
        thread_param_new = eina_mempool_malloc(_mp_command,
                                               sizeof(EVGL_Thread_Command_glClear));
        if (thread_param_new)
          {
             thread_param = thread_param_new;
             thread_param->command_allocated = 1;
             thread_mode = EVAS_GL_THREAD_MODE_FLUSH;
          }
     }

   thread_param->mask = mask;

   evas_gl_thread_cmd_enqueue(EVAS_GL_THREAD_TYPE_EVGL,
                              _evgl_thread_glClear,
                              thread_param,
                              thread_mode);
}

/*
   void
   glClearColor(GLfloat red, GLfloat green, GLfloat blue, GLfloat alpha);
 */

typedef struct
{
   GLfloat red;
   GLfloat green;
   GLfloat blue;
   GLfloat alpha;
   int command_allocated;

} EVGL_Thread_Command_glClearColor;

static void
_evgl_thread_glClearColor(void *data)
{
   EVGL_Thread_Command_glClearColor *thread_param =
      (EVGL_Thread_Command_glClearColor *)data;

   glClearColor(thread_param->red,
                thread_param->green,
                thread_param->blue,
                thread_param->alpha);

   if (thread_param->command_allocated)
     eina_mempool_free(_mp_command, thread_param);
}

EAPI void
glClearColor_evgl_thread_cmd(GLfloat red, GLfloat green, GLfloat blue, GLfloat alpha)
{
   if (!evas_evgl_thread_enabled())
     {
        glClearColor(red, green, blue, alpha);
        return;
     }

   int thread_mode = EVAS_GL_THREAD_MODE_FINISH;

   EVGL_Thread_Command_glClearColor thread_param_local;
   EVGL_Thread_Command_glClearColor *thread_param = &thread_param_local;

   /* command_allocated flag init. */
   thread_param->command_allocated = 0;

   if (!evas_gl_thread_force_finish())
     { /* _flush */
        EVGL_Thread_Command_glClearColor *thread_param_new;
        thread_param_new = eina_mempool_malloc(_mp_command,
                                               sizeof(EVGL_Thread_Command_glClearColor));
        if (thread_param_new)
          {
             thread_param = thread_param_new;
             thread_param->command_allocated = 1;
             thread_mode = EVAS_GL_THREAD_MODE_FLUSH;
          }
     }

   thread_param->red = red;
   thread_param->green = green;
   thread_param->blue = blue;
   thread_param->alpha = alpha;

   evas_gl_thread_cmd_enqueue(EVAS_GL_THREAD_TYPE_EVGL,
                              _evgl_thread_glClearColor,
                              thread_param,
                              thread_mode);
}

/*
   void
   glReadPixels(GLint x, GLint y, GLsizei width, GLsizei height, GLenum format, GLenum type, void *pixels);
 */

typedef struct
{
   GLint x;
   GLint y;
   GLsizei width;
   GLsizei height;
   GLenum format;
   GLenum type;
   void *pixels;

} EVGL_Thread_Command_glReadPixels;

static void
_evgl_thread_glReadPixels(void *data)
{
   EVGL_Thread_Command_glReadPixels *thread_param =
      (EVGL_Thread_Command_glReadPixels *)data;

   glReadPixels(thread_param->x,
                thread_param->y,
                thread_param->width,
                thread_param->height,
                thread_param->format,
                thread_param->type,
                thread_param->pixels);

}

EAPI void
glReadPixels_evgl_thread_cmd(GLint x, GLint y, GLsizei width, GLsizei height, GLenum format, GLenum type, void *pixels)
{
   if (!evas_evgl_thread_enabled())
     {
        glReadPixels(x, y, width, height, format, type, pixels);
        return;
     }

   int thread_mode = EVAS_GL_THREAD_MODE_FINISH;

   EVGL_Thread_Command_glReadPixels thread_param_local;
   EVGL_Thread_Command_glReadPixels *thread_param = &thread_param_local;

   thread_param->x = x;
   thread_param->y = y;
   thread_param->width = width;
   thread_param->height = height;
   thread_param->format = format;
   thread_param->type = type;
   thread_param->pixels = pixels;

   evas_gl_thread_cmd_enqueue(EVAS_GL_THREAD_TYPE_EVGL,
                              _evgl_thread_glReadPixels,
                              thread_param,
                              thread_mode);
}

/*
   void
   glGenFramebuffers(GLsizei n, GLuint *framebuffers);
 */

typedef struct
{
   GLsizei n;
   GLuint *framebuffers;

} EVGL_Thread_Command_glGenFramebuffers;

static void
_evgl_thread_glGenFramebuffers(void *data)
{
   EVGL_Thread_Command_glGenFramebuffers *thread_param =
      (EVGL_Thread_Command_glGenFramebuffers *)data;

   glGenFramebuffers(thread_param->n,
                     thread_param->framebuffers);

}

EAPI void
glGenFramebuffers_evgl_thread_cmd(GLsizei n, GLuint *framebuffers)
{
   if (!evas_evgl_thread_enabled())
     {
        glGenFramebuffers(n, framebuffers);
        return;
     }

   int thread_mode = EVAS_GL_THREAD_MODE_FINISH;

   EVGL_Thread_Command_glGenFramebuffers thread_param_local;
   EVGL_Thread_Command_glGenFramebuffers *thread_param = &thread_param_local;

   thread_param->n = n;
   thread_param->framebuffers = framebuffers;

   evas_gl_thread_cmd_enqueue(EVAS_GL_THREAD_TYPE_EVGL,
                              _evgl_thread_glGenFramebuffers,
                              thread_param,
                              thread_mode);
}

/*
   void
   glBindFramebuffer(GLenum target, GLuint framebuffer);
 */

typedef struct
{
   GLenum target;
   GLuint framebuffer;
   int command_allocated;

} EVGL_Thread_Command_glBindFramebuffer;

static void
_evgl_thread_glBindFramebuffer(void *data)
{
   EVGL_Thread_Command_glBindFramebuffer *thread_param =
      (EVGL_Thread_Command_glBindFramebuffer *)data;

   glBindFramebuffer(thread_param->target,
                     thread_param->framebuffer);

   if (thread_param->command_allocated)
     eina_mempool_free(_mp_command, thread_param);
}

EAPI void
glBindFramebuffer_evgl_thread_cmd(GLenum target, GLuint framebuffer)
{
   if (!evas_evgl_thread_enabled())
     {
        glBindFramebuffer(target, framebuffer);
        return;
     }

   int thread_mode = EVAS_GL_THREAD_MODE_FINISH;

   EVGL_Thread_Command_glBindFramebuffer thread_param_local;
   EVGL_Thread_Command_glBindFramebuffer *thread_param = &thread_param_local;

   /* command_allocated flag init. */
   thread_param->command_allocated = 0;

   if (!evas_gl_thread_force_finish())
     { /* _flush */
        EVGL_Thread_Command_glBindFramebuffer *thread_param_new;
        thread_param_new = eina_mempool_malloc(_mp_command,
                                               sizeof(EVGL_Thread_Command_glBindFramebuffer));
        if (thread_param_new)
          {
             thread_param = thread_param_new;
             thread_param->command_allocated = 1;
             thread_mode = EVAS_GL_THREAD_MODE_FLUSH;
          }
     }

   thread_param->target = target;
   thread_param->framebuffer = framebuffer;

   evas_gl_thread_cmd_enqueue(EVAS_GL_THREAD_TYPE_EVGL,
                              _evgl_thread_glBindFramebuffer,
                              thread_param,
                              thread_mode);
}

/*
   void
   glDeleteFramebuffers(GLsizei n, const GLuint *framebuffers);
 */

typedef struct
{
   GLsizei n;
   const GLuint *framebuffers;
   void *framebuffers_copied; /* COPIED */
   int command_allocated;

} EVGL_Thread_Command_glDeleteFramebuffers;

static void
_evgl_thread_glDeleteFramebuffers(void *data)
{
   EVGL_Thread_Command_glDeleteFramebuffers *thread_param =
      (EVGL_Thread_Command_glDeleteFramebuffers *)data;

   glDeleteFramebuffers(thread_param->n,
                        thread_param->framebuffers);


   if (thread_param->framebuffers_copied)
     eina_mempool_free(_mp_delete_object, thread_param->framebuffers_copied);

   if (thread_param->command_allocated)
     eina_mempool_free(_mp_command, thread_param);
}

EAPI void
glDeleteFramebuffers_evgl_thread_cmd(GLsizei n, const GLuint *framebuffers)
{
   if (!evas_evgl_thread_enabled())
     {
        glDeleteFramebuffers(n, framebuffers);
        return;
     }

   int thread_mode = EVAS_GL_THREAD_MODE_FINISH;

   EVGL_Thread_Command_glDeleteFramebuffers thread_param_local;
   EVGL_Thread_Command_glDeleteFramebuffers *thread_param = &thread_param_local;

   /* command_allocated flag init. */
   thread_param->command_allocated = 0;

   if (!evas_gl_thread_force_finish())
     { /* _flush */
        EVGL_Thread_Command_glDeleteFramebuffers *thread_param_new;
        thread_param_new = eina_mempool_malloc(_mp_command,
                                               sizeof(EVGL_Thread_Command_glDeleteFramebuffers));
        if (thread_param_new)
          {
             thread_param = thread_param_new;
             thread_param->command_allocated = 1;
             thread_mode = EVAS_GL_THREAD_MODE_FLUSH;
          }
     }

   thread_param->n = n;
   thread_param->framebuffers = framebuffers;

   thread_param->framebuffers_copied = NULL;
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
        thread_param->framebuffers_copied = eina_mempool_malloc(_mp_delete_object, copy_size);
        if (thread_param->framebuffers_copied)
          {
             memcpy(thread_param->framebuffers_copied, framebuffers, copy_size);
          }
        else
          {
             thread_mode = EVAS_GL_THREAD_MODE_FINISH;
             goto finish;
          }
        /* 3. replace */
        thread_param->framebuffers = (const GLuint  *)thread_param->framebuffers_copied;
     }
   /* end of copy variable */

finish:
   evas_gl_thread_cmd_enqueue(EVAS_GL_THREAD_TYPE_EVGL,
                              _evgl_thread_glDeleteFramebuffers,
                              thread_param,
                              thread_mode);
}

/*
   void
   glGenRenderbuffers(GLsizei n, GLuint *renderbuffers);
 */

typedef struct
{
   GLsizei n;
   GLuint *renderbuffers;

} EVGL_Thread_Command_glGenRenderbuffers;

static void
_evgl_thread_glGenRenderbuffers(void *data)
{
   EVGL_Thread_Command_glGenRenderbuffers *thread_param =
      (EVGL_Thread_Command_glGenRenderbuffers *)data;

   glGenRenderbuffers(thread_param->n,
                      thread_param->renderbuffers);

}

EAPI void
glGenRenderbuffers_evgl_thread_cmd(GLsizei n, GLuint *renderbuffers)
{
   if (!evas_evgl_thread_enabled())
     {
        glGenRenderbuffers(n, renderbuffers);
        return;
     }

   int thread_mode = EVAS_GL_THREAD_MODE_FINISH;

   EVGL_Thread_Command_glGenRenderbuffers thread_param_local;
   EVGL_Thread_Command_glGenRenderbuffers *thread_param = &thread_param_local;

   thread_param->n = n;
   thread_param->renderbuffers = renderbuffers;

   evas_gl_thread_cmd_enqueue(EVAS_GL_THREAD_TYPE_EVGL,
                              _evgl_thread_glGenRenderbuffers,
                              thread_param,
                              thread_mode);
}

/*
   void
   glBindRenderbuffer(GLenum target, GLuint renderbuffer);
 */

typedef struct
{
   GLenum target;
   GLuint renderbuffer;
   int command_allocated;

} EVGL_Thread_Command_glBindRenderbuffer;

static void
_evgl_thread_glBindRenderbuffer(void *data)
{
   EVGL_Thread_Command_glBindRenderbuffer *thread_param =
      (EVGL_Thread_Command_glBindRenderbuffer *)data;

   glBindRenderbuffer(thread_param->target,
                      thread_param->renderbuffer);

   if (thread_param->command_allocated)
     eina_mempool_free(_mp_command, thread_param);
}

EAPI void
glBindRenderbuffer_evgl_thread_cmd(GLenum target, GLuint renderbuffer)
{
   if (!evas_evgl_thread_enabled())
     {
        glBindRenderbuffer(target, renderbuffer);
        return;
     }

   int thread_mode = EVAS_GL_THREAD_MODE_FINISH;

   EVGL_Thread_Command_glBindRenderbuffer thread_param_local;
   EVGL_Thread_Command_glBindRenderbuffer *thread_param = &thread_param_local;

   /* command_allocated flag init. */
   thread_param->command_allocated = 0;

   if (!evas_gl_thread_force_finish())
     { /* _flush */
        EVGL_Thread_Command_glBindRenderbuffer *thread_param_new;
        thread_param_new = eina_mempool_malloc(_mp_command,
                                               sizeof(EVGL_Thread_Command_glBindRenderbuffer));
        if (thread_param_new)
          {
             thread_param = thread_param_new;
             thread_param->command_allocated = 1;
             thread_mode = EVAS_GL_THREAD_MODE_FLUSH;
          }
     }

   thread_param->target = target;
   thread_param->renderbuffer = renderbuffer;

   evas_gl_thread_cmd_enqueue(EVAS_GL_THREAD_TYPE_EVGL,
                              _evgl_thread_glBindRenderbuffer,
                              thread_param,
                              thread_mode);
}

/*
   void
   glDeleteRenderbuffers(GLsizei n, const GLuint *renderbuffers);
 */

typedef struct
{
   GLsizei n;
   const GLuint *renderbuffers;
   void *renderbuffers_copied; /* COPIED */
   int command_allocated;

} EVGL_Thread_Command_glDeleteRenderbuffers;

static void
_evgl_thread_glDeleteRenderbuffers(void *data)
{
   EVGL_Thread_Command_glDeleteRenderbuffers *thread_param =
      (EVGL_Thread_Command_glDeleteRenderbuffers *)data;

   glDeleteRenderbuffers(thread_param->n,
                         thread_param->renderbuffers);


   if (thread_param->renderbuffers_copied)
     eina_mempool_free(_mp_delete_object, thread_param->renderbuffers_copied);

   if (thread_param->command_allocated)
     eina_mempool_free(_mp_command, thread_param);
}

EAPI void
glDeleteRenderbuffers_evgl_thread_cmd(GLsizei n, const GLuint *renderbuffers)
{
   if (!evas_evgl_thread_enabled())
     {
        glDeleteRenderbuffers(n, renderbuffers);
        return;
     }

   int thread_mode = EVAS_GL_THREAD_MODE_FINISH;

   EVGL_Thread_Command_glDeleteRenderbuffers thread_param_local;
   EVGL_Thread_Command_glDeleteRenderbuffers *thread_param = &thread_param_local;

   /* command_allocated flag init. */
   thread_param->command_allocated = 0;

   if (!evas_gl_thread_force_finish())
     { /* _flush */
        EVGL_Thread_Command_glDeleteRenderbuffers *thread_param_new;
        thread_param_new = eina_mempool_malloc(_mp_command,
                                               sizeof(EVGL_Thread_Command_glDeleteRenderbuffers));
        if (thread_param_new)
          {
             thread_param = thread_param_new;
             thread_param->command_allocated = 1;
             thread_mode = EVAS_GL_THREAD_MODE_FLUSH;
          }
     }

   thread_param->n = n;
   thread_param->renderbuffers = renderbuffers;

   thread_param->renderbuffers_copied = NULL;
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
        thread_param->renderbuffers_copied = eina_mempool_malloc(_mp_delete_object, copy_size);
        if (thread_param->renderbuffers_copied)
          {
             memcpy(thread_param->renderbuffers_copied, renderbuffers, copy_size);
          }
        else
          {
             thread_mode = EVAS_GL_THREAD_MODE_FINISH;
             goto finish;
          }
        /* 3. replace */
        thread_param->renderbuffers = (const GLuint  *)thread_param->renderbuffers_copied;
     }
   /* end of copy variable */

finish:
   evas_gl_thread_cmd_enqueue(EVAS_GL_THREAD_TYPE_EVGL,
                              _evgl_thread_glDeleteRenderbuffers,
                              thread_param,
                              thread_mode);
}

/*
   void
   glRenderbufferStorage(GLenum target, GLenum internalformat, GLsizei width, GLsizei height);
 */

typedef struct
{
   GLenum target;
   GLenum internalformat;
   GLsizei width;
   GLsizei height;
   int command_allocated;

} EVGL_Thread_Command_glRenderbufferStorage;

static void
_evgl_thread_glRenderbufferStorage(void *data)
{
   EVGL_Thread_Command_glRenderbufferStorage *thread_param =
      (EVGL_Thread_Command_glRenderbufferStorage *)data;

   glRenderbufferStorage(thread_param->target,
                         thread_param->internalformat,
                         thread_param->width,
                         thread_param->height);

   if (thread_param->command_allocated)
     eina_mempool_free(_mp_command, thread_param);
}

EAPI void
glRenderbufferStorage_evgl_thread_cmd(GLenum target, GLenum internalformat, GLsizei width, GLsizei height)
{
   if (!evas_evgl_thread_enabled())
     {
        glRenderbufferStorage(target, internalformat, width, height);
        return;
     }

   int thread_mode = EVAS_GL_THREAD_MODE_FINISH;

   EVGL_Thread_Command_glRenderbufferStorage thread_param_local;
   EVGL_Thread_Command_glRenderbufferStorage *thread_param = &thread_param_local;

   /* command_allocated flag init. */
   thread_param->command_allocated = 0;

   if (!evas_gl_thread_force_finish())
     { /* _flush */
        EVGL_Thread_Command_glRenderbufferStorage *thread_param_new;
        thread_param_new = eina_mempool_malloc(_mp_command,
                                               sizeof(EVGL_Thread_Command_glRenderbufferStorage));
        if (thread_param_new)
          {
             thread_param = thread_param_new;
             thread_param->command_allocated = 1;
             thread_mode = EVAS_GL_THREAD_MODE_FLUSH;
          }
     }

   thread_param->target = target;
   thread_param->internalformat = internalformat;
   thread_param->width = width;
   thread_param->height = height;

   evas_gl_thread_cmd_enqueue(EVAS_GL_THREAD_TYPE_EVGL,
                              _evgl_thread_glRenderbufferStorage,
                              thread_param,
                              thread_mode);
}

/*
   void
   glFramebufferRenderbuffer(GLenum target, GLenum attachment, GLenum renderbuffertarget, GLuint renderbuffer);
 */

typedef struct
{
   GLenum target;
   GLenum attachment;
   GLenum renderbuffertarget;
   GLuint renderbuffer;
   int command_allocated;

} EVGL_Thread_Command_glFramebufferRenderbuffer;

static void
_evgl_thread_glFramebufferRenderbuffer(void *data)
{
   EVGL_Thread_Command_glFramebufferRenderbuffer *thread_param =
      (EVGL_Thread_Command_glFramebufferRenderbuffer *)data;

   glFramebufferRenderbuffer(thread_param->target,
                             thread_param->attachment,
                             thread_param->renderbuffertarget,
                             thread_param->renderbuffer);

   if (thread_param->command_allocated)
     eina_mempool_free(_mp_command, thread_param);
}

EAPI void
glFramebufferRenderbuffer_evgl_thread_cmd(GLenum target, GLenum attachment, GLenum renderbuffertarget, GLuint renderbuffer)
{
   if (!evas_evgl_thread_enabled())
     {
        glFramebufferRenderbuffer(target, attachment, renderbuffertarget, renderbuffer);
        return;
     }

   int thread_mode = EVAS_GL_THREAD_MODE_FINISH;

   EVGL_Thread_Command_glFramebufferRenderbuffer thread_param_local;
   EVGL_Thread_Command_glFramebufferRenderbuffer *thread_param = &thread_param_local;

   /* command_allocated flag init. */
   thread_param->command_allocated = 0;

   if (!evas_gl_thread_force_finish())
     { /* _flush */
        EVGL_Thread_Command_glFramebufferRenderbuffer *thread_param_new;
        thread_param_new = eina_mempool_malloc(_mp_command,
                                               sizeof(EVGL_Thread_Command_glFramebufferRenderbuffer));
        if (thread_param_new)
          {
             thread_param = thread_param_new;
             thread_param->command_allocated = 1;
             thread_mode = EVAS_GL_THREAD_MODE_FLUSH;
          }
     }

   thread_param->target = target;
   thread_param->attachment = attachment;
   thread_param->renderbuffertarget = renderbuffertarget;
   thread_param->renderbuffer = renderbuffer;

   evas_gl_thread_cmd_enqueue(EVAS_GL_THREAD_TYPE_EVGL,
                              _evgl_thread_glFramebufferRenderbuffer,
                              thread_param,
                              thread_mode);
}

/*
   void
   glFramebufferTexture2D(GLenum target, GLenum attachment, GLenum textarget, GLuint texture, GLint level);
 */

typedef struct
{
   GLenum target;
   GLenum attachment;
   GLenum textarget;
   GLuint texture;
   GLint level;
   int command_allocated;

} EVGL_Thread_Command_glFramebufferTexture2D;

static void
_evgl_thread_glFramebufferTexture2D(void *data)
{
   EVGL_Thread_Command_glFramebufferTexture2D *thread_param =
      (EVGL_Thread_Command_glFramebufferTexture2D *)data;

   glFramebufferTexture2D(thread_param->target,
                          thread_param->attachment,
                          thread_param->textarget,
                          thread_param->texture,
                          thread_param->level);

   if (thread_param->command_allocated)
     eina_mempool_free(_mp_command, thread_param);
}

EAPI void
glFramebufferTexture2D_evgl_thread_cmd(GLenum target, GLenum attachment, GLenum textarget, GLuint texture, GLint level)
{
   if (!evas_evgl_thread_enabled())
     {
        glFramebufferTexture2D(target, attachment, textarget, texture, level);
        return;
     }

   int thread_mode = EVAS_GL_THREAD_MODE_FINISH;

   EVGL_Thread_Command_glFramebufferTexture2D thread_param_local;
   EVGL_Thread_Command_glFramebufferTexture2D *thread_param = &thread_param_local;

   /* command_allocated flag init. */
   thread_param->command_allocated = 0;

   if (!evas_gl_thread_force_finish())
     { /* _flush */
        EVGL_Thread_Command_glFramebufferTexture2D *thread_param_new;
        thread_param_new = eina_mempool_malloc(_mp_command,
                                               sizeof(EVGL_Thread_Command_glFramebufferTexture2D));
        if (thread_param_new)
          {
             thread_param = thread_param_new;
             thread_param->command_allocated = 1;
             thread_mode = EVAS_GL_THREAD_MODE_FLUSH;
          }
     }

   thread_param->target = target;
   thread_param->attachment = attachment;
   thread_param->textarget = textarget;
   thread_param->texture = texture;
   thread_param->level = level;

   evas_gl_thread_cmd_enqueue(EVAS_GL_THREAD_TYPE_EVGL,
                              _evgl_thread_glFramebufferTexture2D,
                              thread_param,
                              thread_mode);
}

/*
   GLenum
   glCheckFramebufferStatus(GLenum target);
 */

typedef struct
{
   GLenum return_value;
   GLenum target;

} EVGL_Thread_Command_glCheckFramebufferStatus;

static void
_evgl_thread_glCheckFramebufferStatus(void *data)
{
   EVGL_Thread_Command_glCheckFramebufferStatus *thread_param =
      (EVGL_Thread_Command_glCheckFramebufferStatus *)data;

   thread_param->return_value = glCheckFramebufferStatus(thread_param->target);

}

EAPI GLenum
glCheckFramebufferStatus_evgl_thread_cmd(GLenum target)
{
   if (!evas_evgl_thread_enabled())
     {
        return glCheckFramebufferStatus(target);
     }

   int thread_mode = EVAS_GL_THREAD_MODE_FINISH;

   EVGL_Thread_Command_glCheckFramebufferStatus thread_param_local;
   EVGL_Thread_Command_glCheckFramebufferStatus *thread_param = &thread_param_local;

   thread_param->target = target;

   evas_gl_thread_cmd_enqueue(EVAS_GL_THREAD_TYPE_EVGL,
                              _evgl_thread_glCheckFramebufferStatus,
                              thread_param,
                              thread_mode);

   return thread_param->return_value;
}

/*
   void
   glFlush(void);
 */

static void
_evgl_thread_glFlush(void *data EINA_UNUSED)
{
   glFlush();

}

EAPI void
glFlush_evgl_thread_cmd(void)
{
   if (!evas_evgl_thread_enabled())
     {
        glFlush();
        return;
     }

   int thread_mode = EVAS_GL_THREAD_MODE_FINISH;

   evas_gl_thread_cmd_enqueue(EVAS_GL_THREAD_TYPE_EVGL,
                              _evgl_thread_glFlush,
                              NULL,
                              thread_mode);
}

/*
   void
   glFinish(void);
 */

static void
_evgl_thread_glFinish(void *data EINA_UNUSED)
{
   glFinish();

}

EAPI void
glFinish_evgl_thread_cmd(void)
{
   if (!evas_evgl_thread_enabled())
     {
        glFinish();
        return;
     }

   int thread_mode = EVAS_GL_THREAD_MODE_FINISH;

   evas_gl_thread_cmd_enqueue(EVAS_GL_THREAD_TYPE_EVGL,
                              _evgl_thread_glFinish,
                              NULL,
                              thread_mode);
}

/*
   void
   glHint(GLenum target, GLenum mode);
 */

typedef struct
{
   GLenum target;
   GLenum mode;
   int command_allocated;

} EVGL_Thread_Command_glHint;

static void
_evgl_thread_glHint(void *data)
{
   EVGL_Thread_Command_glHint *thread_param =
      (EVGL_Thread_Command_glHint *)data;

   glHint(thread_param->target,
          thread_param->mode);

   if (thread_param->command_allocated)
     eina_mempool_free(_mp_command, thread_param);
}

EAPI void
glHint_evgl_thread_cmd(GLenum target, GLenum mode)
{
   if (!evas_evgl_thread_enabled())
     {
        glHint(target, mode);
        return;
     }

   int thread_mode = EVAS_GL_THREAD_MODE_FINISH;

   EVGL_Thread_Command_glHint thread_param_local;
   EVGL_Thread_Command_glHint *thread_param = &thread_param_local;

   /* command_allocated flag init. */
   thread_param->command_allocated = 0;

   if (!evas_gl_thread_force_finish())
     { /* _flush */
        EVGL_Thread_Command_glHint *thread_param_new;
        thread_param_new = eina_mempool_malloc(_mp_command,
                                               sizeof(EVGL_Thread_Command_glHint));
        if (thread_param_new)
          {
             thread_param = thread_param_new;
             thread_param->command_allocated = 1;
             thread_mode = EVAS_GL_THREAD_MODE_FLUSH;
          }
     }

   thread_param->target = target;
   thread_param->mode = mode;

   evas_gl_thread_cmd_enqueue(EVAS_GL_THREAD_TYPE_EVGL,
                              _evgl_thread_glHint,
                              thread_param,
                              thread_mode);
}

/*
   const GLubyte *
   glGetString(GLenum name);
 */

typedef struct
{
   const GLubyte * return_value;
   GLenum name;

} EVGL_Thread_Command_glGetString;

static void
_evgl_thread_glGetString(void *data)
{
   EVGL_Thread_Command_glGetString *thread_param =
      (EVGL_Thread_Command_glGetString *)data;

   thread_param->return_value = glGetString(thread_param->name);

}

EAPI const GLubyte *
glGetString_evgl_thread_cmd(GLenum name)
{
   if (!evas_evgl_thread_enabled())
     {
        return glGetString(name);
     }

   int thread_mode = EVAS_GL_THREAD_MODE_FINISH;

   EVGL_Thread_Command_glGetString thread_param_local;
   EVGL_Thread_Command_glGetString *thread_param = &thread_param_local;

   thread_param->name = name;

   evas_gl_thread_cmd_enqueue(EVAS_GL_THREAD_TYPE_EVGL,
                              _evgl_thread_glGetString,
                              thread_param,
                              thread_mode);

   return thread_param->return_value;
}

/*
   void
   glGetBooleanv(GLenum pname, GLboolean *data);
 */

typedef struct
{
   GLenum pname;
   GLboolean *data;

} EVGL_Thread_Command_glGetBooleanv;

static void
_evgl_thread_glGetBooleanv(void *data)
{
   EVGL_Thread_Command_glGetBooleanv *thread_param =
      (EVGL_Thread_Command_glGetBooleanv *)data;

   glGetBooleanv(thread_param->pname,
                 thread_param->data);

}

EAPI void
glGetBooleanv_evgl_thread_cmd(GLenum pname, GLboolean *data)
{
   if (!evas_evgl_thread_enabled())
     {
        glGetBooleanv(pname, data);
        return;
     }

   int thread_mode = EVAS_GL_THREAD_MODE_FINISH;

   EVGL_Thread_Command_glGetBooleanv thread_param_local;
   EVGL_Thread_Command_glGetBooleanv *thread_param = &thread_param_local;

   thread_param->pname = pname;
   thread_param->data = data;

   evas_gl_thread_cmd_enqueue(EVAS_GL_THREAD_TYPE_EVGL,
                              _evgl_thread_glGetBooleanv,
                              thread_param,
                              thread_mode);
}

/*
   void
   glGetFloatv(GLenum pname, GLfloat *data);
 */

typedef struct
{
   GLenum pname;
   GLfloat *data;

} EVGL_Thread_Command_glGetFloatv;

static void
_evgl_thread_glGetFloatv(void *data)
{
   EVGL_Thread_Command_glGetFloatv *thread_param =
      (EVGL_Thread_Command_glGetFloatv *)data;

   glGetFloatv(thread_param->pname,
               thread_param->data);

}

EAPI void
glGetFloatv_evgl_thread_cmd(GLenum pname, GLfloat *data)
{
   if (!evas_evgl_thread_enabled())
     {
        glGetFloatv(pname, data);
        return;
     }

   int thread_mode = EVAS_GL_THREAD_MODE_FINISH;

   EVGL_Thread_Command_glGetFloatv thread_param_local;
   EVGL_Thread_Command_glGetFloatv *thread_param = &thread_param_local;

   thread_param->pname = pname;
   thread_param->data = data;

   evas_gl_thread_cmd_enqueue(EVAS_GL_THREAD_TYPE_EVGL,
                              _evgl_thread_glGetFloatv,
                              thread_param,
                              thread_mode);
}

/*
   void
   glGetIntegerv(GLenum pname, GLint *data);
 */

typedef struct
{
   GLenum pname;
   GLint *data;

} EVGL_Thread_Command_glGetIntegerv;

static void
_evgl_thread_glGetIntegerv(void *data)
{
   EVGL_Thread_Command_glGetIntegerv *thread_param =
      (EVGL_Thread_Command_glGetIntegerv *)data;

   glGetIntegerv(thread_param->pname,
                 thread_param->data);

}

EAPI void
glGetIntegerv_evgl_thread_cmd(GLenum pname, GLint *data)
{
   if (!evas_evgl_thread_enabled())
     {
        glGetIntegerv(pname, data);
        return;
     }

   int thread_mode = EVAS_GL_THREAD_MODE_FINISH;

   EVGL_Thread_Command_glGetIntegerv thread_param_local;
   EVGL_Thread_Command_glGetIntegerv *thread_param = &thread_param_local;

   thread_param->pname = pname;
   thread_param->data = data;

   evas_gl_thread_cmd_enqueue(EVAS_GL_THREAD_TYPE_EVGL,
                              _evgl_thread_glGetIntegerv,
                              thread_param,
                              thread_mode);
}

/*
   GLboolean
   glIsBuffer(GLint buffer);
 */

typedef struct
{
   GLboolean return_value;
   GLint buffer;

} EVGL_Thread_Command_glIsBuffer;

static void
_evgl_thread_glIsBuffer(void *data)
{
   EVGL_Thread_Command_glIsBuffer *thread_param =
      (EVGL_Thread_Command_glIsBuffer *)data;

   thread_param->return_value = glIsBuffer(thread_param->buffer);

}

EAPI GLboolean
glIsBuffer_evgl_thread_cmd(GLint buffer)
{
   if (!evas_evgl_thread_enabled())
     {
        return glIsBuffer(buffer);
     }

   int thread_mode = EVAS_GL_THREAD_MODE_FINISH;

   EVGL_Thread_Command_glIsBuffer thread_param_local;
   EVGL_Thread_Command_glIsBuffer *thread_param = &thread_param_local;

   thread_param->buffer = buffer;

   evas_gl_thread_cmd_enqueue(EVAS_GL_THREAD_TYPE_EVGL,
                              _evgl_thread_glIsBuffer,
                              thread_param,
                              thread_mode);

   return thread_param->return_value;
}

/*
   void
   glGetBufferParameteriv(GLenum target, GLenum pname, GLint *params);
 */

typedef struct
{
   GLenum target;
   GLenum pname;
   GLint *params;

} EVGL_Thread_Command_glGetBufferParameteriv;

static void
_evgl_thread_glGetBufferParameteriv(void *data)
{
   EVGL_Thread_Command_glGetBufferParameteriv *thread_param =
      (EVGL_Thread_Command_glGetBufferParameteriv *)data;

   glGetBufferParameteriv(thread_param->target,
                          thread_param->pname,
                          thread_param->params);

}

EAPI void
glGetBufferParameteriv_evgl_thread_cmd(GLenum target, GLenum pname, GLint *params)
{
   if (!evas_evgl_thread_enabled())
     {
        glGetBufferParameteriv(target, pname, params);
        return;
     }

   int thread_mode = EVAS_GL_THREAD_MODE_FINISH;

   EVGL_Thread_Command_glGetBufferParameteriv thread_param_local;
   EVGL_Thread_Command_glGetBufferParameteriv *thread_param = &thread_param_local;

   thread_param->target = target;
   thread_param->pname = pname;
   thread_param->params = params;

   evas_gl_thread_cmd_enqueue(EVAS_GL_THREAD_TYPE_EVGL,
                              _evgl_thread_glGetBufferParameteriv,
                              thread_param,
                              thread_mode);
}

/*
   GLboolean
   glIsShader(GLuint shader);
 */

typedef struct
{
   GLboolean return_value;
   GLuint shader;

} EVGL_Thread_Command_glIsShader;

static void
_evgl_thread_glIsShader(void *data)
{
   EVGL_Thread_Command_glIsShader *thread_param =
      (EVGL_Thread_Command_glIsShader *)data;

   thread_param->return_value = glIsShader(thread_param->shader);

}

EAPI GLboolean
glIsShader_evgl_thread_cmd(GLuint shader)
{
   if (!evas_evgl_thread_enabled())
     {
        return glIsShader(shader);
     }

   int thread_mode = EVAS_GL_THREAD_MODE_FINISH;

   EVGL_Thread_Command_glIsShader thread_param_local;
   EVGL_Thread_Command_glIsShader *thread_param = &thread_param_local;

   thread_param->shader = shader;

   evas_gl_thread_cmd_enqueue(EVAS_GL_THREAD_TYPE_EVGL,
                              _evgl_thread_glIsShader,
                              thread_param,
                              thread_mode);

   return thread_param->return_value;
}

/*
   void
   glGetShaderiv(GLuint shader, GLenum pname, GLint *params);
 */

typedef struct
{
   GLuint shader;
   GLenum pname;
   GLint *params;

} EVGL_Thread_Command_glGetShaderiv;

static void
_evgl_thread_glGetShaderiv(void *data)
{
   EVGL_Thread_Command_glGetShaderiv *thread_param =
      (EVGL_Thread_Command_glGetShaderiv *)data;

   glGetShaderiv(thread_param->shader,
                 thread_param->pname,
                 thread_param->params);

}

EAPI void
glGetShaderiv_evgl_thread_cmd(GLuint shader, GLenum pname, GLint *params)
{
   if (!evas_evgl_thread_enabled())
     {
        glGetShaderiv(shader, pname, params);
        return;
     }

   int thread_mode = EVAS_GL_THREAD_MODE_FINISH;

   EVGL_Thread_Command_glGetShaderiv thread_param_local;
   EVGL_Thread_Command_glGetShaderiv *thread_param = &thread_param_local;

   thread_param->shader = shader;
   thread_param->pname = pname;
   thread_param->params = params;

   evas_gl_thread_cmd_enqueue(EVAS_GL_THREAD_TYPE_EVGL,
                              _evgl_thread_glGetShaderiv,
                              thread_param,
                              thread_mode);
}

/*
   void
   glGetAttachedShaders(GLuint program, GLsizei maxCount, GLsizei *count, GLuint *shaders);
 */

typedef struct
{
   GLuint program;
   GLsizei maxCount;
   GLsizei *count;
   GLuint *shaders;

} EVGL_Thread_Command_glGetAttachedShaders;

static void
_evgl_thread_glGetAttachedShaders(void *data)
{
   EVGL_Thread_Command_glGetAttachedShaders *thread_param =
      (EVGL_Thread_Command_glGetAttachedShaders *)data;

   glGetAttachedShaders(thread_param->program,
                        thread_param->maxCount,
                        thread_param->count,
                        thread_param->shaders);

}

EAPI void
glGetAttachedShaders_evgl_thread_cmd(GLuint program, GLsizei maxCount, GLsizei *count, GLuint *shaders)
{
   if (!evas_evgl_thread_enabled())
     {
        glGetAttachedShaders(program, maxCount, count, shaders);
        return;
     }

   int thread_mode = EVAS_GL_THREAD_MODE_FINISH;

   EVGL_Thread_Command_glGetAttachedShaders thread_param_local;
   EVGL_Thread_Command_glGetAttachedShaders *thread_param = &thread_param_local;

   thread_param->program = program;
   thread_param->maxCount = maxCount;
   thread_param->count = count;
   thread_param->shaders = shaders;

   evas_gl_thread_cmd_enqueue(EVAS_GL_THREAD_TYPE_EVGL,
                              _evgl_thread_glGetAttachedShaders,
                              thread_param,
                              thread_mode);
}

/*
   void
   glGetShaderInfoLog(GLuint shader, GLsizei bufSize, GLsizei *length, GLchar *infoLog);
 */

typedef struct
{
   GLuint shader;
   GLsizei bufSize;
   GLsizei *length;
   GLchar *infoLog;

} EVGL_Thread_Command_glGetShaderInfoLog;

static void
_evgl_thread_glGetShaderInfoLog(void *data)
{
   EVGL_Thread_Command_glGetShaderInfoLog *thread_param =
      (EVGL_Thread_Command_glGetShaderInfoLog *)data;

   glGetShaderInfoLog(thread_param->shader,
                      thread_param->bufSize,
                      thread_param->length,
                      thread_param->infoLog);

}

EAPI void
glGetShaderInfoLog_evgl_thread_cmd(GLuint shader, GLsizei bufSize, GLsizei *length, GLchar *infoLog)
{
   if (!evas_evgl_thread_enabled())
     {
        glGetShaderInfoLog(shader, bufSize, length, infoLog);
        return;
     }

   int thread_mode = EVAS_GL_THREAD_MODE_FINISH;

   EVGL_Thread_Command_glGetShaderInfoLog thread_param_local;
   EVGL_Thread_Command_glGetShaderInfoLog *thread_param = &thread_param_local;

   thread_param->shader = shader;
   thread_param->bufSize = bufSize;
   thread_param->length = length;
   thread_param->infoLog = infoLog;

   evas_gl_thread_cmd_enqueue(EVAS_GL_THREAD_TYPE_EVGL,
                              _evgl_thread_glGetShaderInfoLog,
                              thread_param,
                              thread_mode);
}

/*
   void
   glGetShaderSource(GLuint shader, GLsizei bufSize, GLsizei *length, GLchar *source);
 */

typedef struct
{
   GLuint shader;
   GLsizei bufSize;
   GLsizei *length;
   GLchar *source;

} EVGL_Thread_Command_glGetShaderSource;

static void
_evgl_thread_glGetShaderSource(void *data)
{
   EVGL_Thread_Command_glGetShaderSource *thread_param =
      (EVGL_Thread_Command_glGetShaderSource *)data;

   glGetShaderSource(thread_param->shader,
                     thread_param->bufSize,
                     thread_param->length,
                     thread_param->source);

}

EAPI void
glGetShaderSource_evgl_thread_cmd(GLuint shader, GLsizei bufSize, GLsizei *length, GLchar *source)
{
   if (!evas_evgl_thread_enabled())
     {
        glGetShaderSource(shader, bufSize, length, source);
        return;
     }

   int thread_mode = EVAS_GL_THREAD_MODE_FINISH;

   EVGL_Thread_Command_glGetShaderSource thread_param_local;
   EVGL_Thread_Command_glGetShaderSource *thread_param = &thread_param_local;

   thread_param->shader = shader;
   thread_param->bufSize = bufSize;
   thread_param->length = length;
   thread_param->source = source;

   evas_gl_thread_cmd_enqueue(EVAS_GL_THREAD_TYPE_EVGL,
                              _evgl_thread_glGetShaderSource,
                              thread_param,
                              thread_mode);
}

/*
   void
   glGetShaderPrecisionFormat(GLenum shadertype, GLenum precisiontype, GLint *range, GLint *precision);
 */

typedef struct
{
   GLenum shadertype;
   GLenum precisiontype;
   GLint *range;
   GLint *precision;

} EVGL_Thread_Command_glGetShaderPrecisionFormat;

static void
_evgl_thread_glGetShaderPrecisionFormat(void *data)
{
   EVGL_Thread_Command_glGetShaderPrecisionFormat *thread_param =
      (EVGL_Thread_Command_glGetShaderPrecisionFormat *)data;

   glGetShaderPrecisionFormat(thread_param->shadertype,
                              thread_param->precisiontype,
                              thread_param->range,
                              thread_param->precision);

}

EAPI void
glGetShaderPrecisionFormat_evgl_thread_cmd(GLenum shadertype, GLenum precisiontype, GLint *range, GLint *precision)
{
   if (!evas_evgl_thread_enabled())
     {
        glGetShaderPrecisionFormat(shadertype, precisiontype, range, precision);
        return;
     }

   int thread_mode = EVAS_GL_THREAD_MODE_FINISH;

   EVGL_Thread_Command_glGetShaderPrecisionFormat thread_param_local;
   EVGL_Thread_Command_glGetShaderPrecisionFormat *thread_param = &thread_param_local;

   thread_param->shadertype = shadertype;
   thread_param->precisiontype = precisiontype;
   thread_param->range = range;
   thread_param->precision = precision;

   evas_gl_thread_cmd_enqueue(EVAS_GL_THREAD_TYPE_EVGL,
                              _evgl_thread_glGetShaderPrecisionFormat,
                              thread_param,
                              thread_mode);
}

/*
   void
   glGetVertexAttribfv(GLuint index, GLenum pname, GLfloat *params);
 */

typedef struct
{
   GLuint index;
   GLenum pname;
   GLfloat *params;

} EVGL_Thread_Command_glGetVertexAttribfv;

static void
_evgl_thread_glGetVertexAttribfv(void *data)
{
   EVGL_Thread_Command_glGetVertexAttribfv *thread_param =
      (EVGL_Thread_Command_glGetVertexAttribfv *)data;

   glGetVertexAttribfv(thread_param->index,
                       thread_param->pname,
                       thread_param->params);

}

EAPI void
glGetVertexAttribfv_evgl_thread_cmd(GLuint index, GLenum pname, GLfloat *params)
{
   if (!evas_evgl_thread_enabled())
     {
        glGetVertexAttribfv(index, pname, params);
        return;
     }

   int thread_mode = EVAS_GL_THREAD_MODE_FINISH;

   EVGL_Thread_Command_glGetVertexAttribfv thread_param_local;
   EVGL_Thread_Command_glGetVertexAttribfv *thread_param = &thread_param_local;

   thread_param->index = index;
   thread_param->pname = pname;
   thread_param->params = params;

   evas_gl_thread_cmd_enqueue(EVAS_GL_THREAD_TYPE_EVGL,
                              _evgl_thread_glGetVertexAttribfv,
                              thread_param,
                              thread_mode);
}

/*
   void
   glGetVertexAttribiv(GLuint index, GLenum pname, GLint *params);
 */

typedef struct
{
   GLuint index;
   GLenum pname;
   GLint *params;

} EVGL_Thread_Command_glGetVertexAttribiv;

static void
_evgl_thread_glGetVertexAttribiv(void *data)
{
   EVGL_Thread_Command_glGetVertexAttribiv *thread_param =
      (EVGL_Thread_Command_glGetVertexAttribiv *)data;

   glGetVertexAttribiv(thread_param->index,
                       thread_param->pname,
                       thread_param->params);

}

EAPI void
glGetVertexAttribiv_evgl_thread_cmd(GLuint index, GLenum pname, GLint *params)
{
   if (!evas_evgl_thread_enabled())
     {
        glGetVertexAttribiv(index, pname, params);
        return;
     }

   int thread_mode = EVAS_GL_THREAD_MODE_FINISH;

   EVGL_Thread_Command_glGetVertexAttribiv thread_param_local;
   EVGL_Thread_Command_glGetVertexAttribiv *thread_param = &thread_param_local;

   thread_param->index = index;
   thread_param->pname = pname;
   thread_param->params = params;

   evas_gl_thread_cmd_enqueue(EVAS_GL_THREAD_TYPE_EVGL,
                              _evgl_thread_glGetVertexAttribiv,
                              thread_param,
                              thread_mode);
}

/*
   GLboolean
   glIsProgram(GLuint program);
 */

typedef struct
{
   GLboolean return_value;
   GLuint program;

} EVGL_Thread_Command_glIsProgram;

static void
_evgl_thread_glIsProgram(void *data)
{
   EVGL_Thread_Command_glIsProgram *thread_param =
      (EVGL_Thread_Command_glIsProgram *)data;

   thread_param->return_value = glIsProgram(thread_param->program);

}

EAPI GLboolean
glIsProgram_evgl_thread_cmd(GLuint program)
{
   if (!evas_evgl_thread_enabled())
     {
        return glIsProgram(program);
     }

   int thread_mode = EVAS_GL_THREAD_MODE_FINISH;

   EVGL_Thread_Command_glIsProgram thread_param_local;
   EVGL_Thread_Command_glIsProgram *thread_param = &thread_param_local;

   thread_param->program = program;

   evas_gl_thread_cmd_enqueue(EVAS_GL_THREAD_TYPE_EVGL,
                              _evgl_thread_glIsProgram,
                              thread_param,
                              thread_mode);

   return thread_param->return_value;
}

/*
   void
   glGetProgramInfoLog(GLuint program, GLsizei bufSize, GLsizei *length, GLchar *infoLog);
 */

typedef struct
{
   GLuint program;
   GLsizei bufSize;
   GLsizei *length;
   GLchar *infoLog;

} EVGL_Thread_Command_glGetProgramInfoLog;

static void
_evgl_thread_glGetProgramInfoLog(void *data)
{
   EVGL_Thread_Command_glGetProgramInfoLog *thread_param =
      (EVGL_Thread_Command_glGetProgramInfoLog *)data;

   glGetProgramInfoLog(thread_param->program,
                       thread_param->bufSize,
                       thread_param->length,
                       thread_param->infoLog);

}

EAPI void
glGetProgramInfoLog_evgl_thread_cmd(GLuint program, GLsizei bufSize, GLsizei *length, GLchar *infoLog)
{
   if (!evas_evgl_thread_enabled())
     {
        glGetProgramInfoLog(program, bufSize, length, infoLog);
        return;
     }

   int thread_mode = EVAS_GL_THREAD_MODE_FINISH;

   EVGL_Thread_Command_glGetProgramInfoLog thread_param_local;
   EVGL_Thread_Command_glGetProgramInfoLog *thread_param = &thread_param_local;

   thread_param->program = program;
   thread_param->bufSize = bufSize;
   thread_param->length = length;
   thread_param->infoLog = infoLog;

   evas_gl_thread_cmd_enqueue(EVAS_GL_THREAD_TYPE_EVGL,
                              _evgl_thread_glGetProgramInfoLog,
                              thread_param,
                              thread_mode);
}

/*
   void
   glGetProgramiv(GLuint program, GLenum pname, GLint *params);
 */

typedef struct
{
   GLuint program;
   GLenum pname;
   GLint *params;

} EVGL_Thread_Command_glGetProgramiv;

static void
_evgl_thread_glGetProgramiv(void *data)
{
   EVGL_Thread_Command_glGetProgramiv *thread_param =
      (EVGL_Thread_Command_glGetProgramiv *)data;

   glGetProgramiv(thread_param->program,
                  thread_param->pname,
                  thread_param->params);

}

EAPI void
glGetProgramiv_evgl_thread_cmd(GLuint program, GLenum pname, GLint *params)
{
   if (!evas_evgl_thread_enabled())
     {
        glGetProgramiv(program, pname, params);
        return;
     }

   int thread_mode = EVAS_GL_THREAD_MODE_FINISH;

   EVGL_Thread_Command_glGetProgramiv thread_param_local;
   EVGL_Thread_Command_glGetProgramiv *thread_param = &thread_param_local;

   thread_param->program = program;
   thread_param->pname = pname;
   thread_param->params = params;

   evas_gl_thread_cmd_enqueue(EVAS_GL_THREAD_TYPE_EVGL,
                              _evgl_thread_glGetProgramiv,
                              thread_param,
                              thread_mode);
}

/*
   GLboolean
   glIsFramebuffer(GLint framebuffer);
 */

typedef struct
{
   GLboolean return_value;
   GLint framebuffer;

} EVGL_Thread_Command_glIsFramebuffer;

static void
_evgl_thread_glIsFramebuffer(void *data)
{
   EVGL_Thread_Command_glIsFramebuffer *thread_param =
      (EVGL_Thread_Command_glIsFramebuffer *)data;

   thread_param->return_value = glIsFramebuffer(thread_param->framebuffer);

}

EAPI GLboolean
glIsFramebuffer_evgl_thread_cmd(GLint framebuffer)
{
   if (!evas_evgl_thread_enabled())
     {
        return glIsFramebuffer(framebuffer);
     }

   int thread_mode = EVAS_GL_THREAD_MODE_FINISH;

   EVGL_Thread_Command_glIsFramebuffer thread_param_local;
   EVGL_Thread_Command_glIsFramebuffer *thread_param = &thread_param_local;

   thread_param->framebuffer = framebuffer;

   evas_gl_thread_cmd_enqueue(EVAS_GL_THREAD_TYPE_EVGL,
                              _evgl_thread_glIsFramebuffer,
                              thread_param,
                              thread_mode);

   return thread_param->return_value;
}

/*
   GLboolean
   glIsRenderbuffer(GLint renderbuffer);
 */

typedef struct
{
   GLboolean return_value;
   GLint renderbuffer;

} EVGL_Thread_Command_glIsRenderbuffer;

static void
_evgl_thread_glIsRenderbuffer(void *data)
{
   EVGL_Thread_Command_glIsRenderbuffer *thread_param =
      (EVGL_Thread_Command_glIsRenderbuffer *)data;

   thread_param->return_value = glIsRenderbuffer(thread_param->renderbuffer);

}

EAPI GLboolean
glIsRenderbuffer_evgl_thread_cmd(GLint renderbuffer)
{
   if (!evas_evgl_thread_enabled())
     {
        return glIsRenderbuffer(renderbuffer);
     }

   int thread_mode = EVAS_GL_THREAD_MODE_FINISH;

   EVGL_Thread_Command_glIsRenderbuffer thread_param_local;
   EVGL_Thread_Command_glIsRenderbuffer *thread_param = &thread_param_local;

   thread_param->renderbuffer = renderbuffer;

   evas_gl_thread_cmd_enqueue(EVAS_GL_THREAD_TYPE_EVGL,
                              _evgl_thread_glIsRenderbuffer,
                              thread_param,
                              thread_mode);

   return thread_param->return_value;
}

/*
   void
   glGetRenderbufferParameteriv(GLenum target, GLenum pname, GLint *params);
 */

typedef struct
{
   GLenum target;
   GLenum pname;
   GLint *params;

} EVGL_Thread_Command_glGetRenderbufferParameteriv;

static void
_evgl_thread_glGetRenderbufferParameteriv(void *data)
{
   EVGL_Thread_Command_glGetRenderbufferParameteriv *thread_param =
      (EVGL_Thread_Command_glGetRenderbufferParameteriv *)data;

   glGetRenderbufferParameteriv(thread_param->target,
                                thread_param->pname,
                                thread_param->params);

}

EAPI void
glGetRenderbufferParameteriv_evgl_thread_cmd(GLenum target, GLenum pname, GLint *params)
{
   if (!evas_evgl_thread_enabled())
     {
        glGetRenderbufferParameteriv(target, pname, params);
        return;
     }

   int thread_mode = EVAS_GL_THREAD_MODE_FINISH;

   EVGL_Thread_Command_glGetRenderbufferParameteriv thread_param_local;
   EVGL_Thread_Command_glGetRenderbufferParameteriv *thread_param = &thread_param_local;

   thread_param->target = target;
   thread_param->pname = pname;
   thread_param->params = params;

   evas_gl_thread_cmd_enqueue(EVAS_GL_THREAD_TYPE_EVGL,
                              _evgl_thread_glGetRenderbufferParameteriv,
                              thread_param,
                              thread_mode);
}

/*
   GLboolean
   glIsTexture(GLint texture);
 */

typedef struct
{
   GLboolean return_value;
   GLint texture;

} EVGL_Thread_Command_glIsTexture;

static void
_evgl_thread_glIsTexture(void *data)
{
   EVGL_Thread_Command_glIsTexture *thread_param =
      (EVGL_Thread_Command_glIsTexture *)data;

   thread_param->return_value = glIsTexture(thread_param->texture);

}

EAPI GLboolean
glIsTexture_evgl_thread_cmd(GLint texture)
{
   if (!evas_evgl_thread_enabled())
     {
        return glIsTexture(texture);
     }

   int thread_mode = EVAS_GL_THREAD_MODE_FINISH;

   EVGL_Thread_Command_glIsTexture thread_param_local;
   EVGL_Thread_Command_glIsTexture *thread_param = &thread_param_local;

   thread_param->texture = texture;

   evas_gl_thread_cmd_enqueue(EVAS_GL_THREAD_TYPE_EVGL,
                              _evgl_thread_glIsTexture,
                              thread_param,
                              thread_mode);

   return thread_param->return_value;
}
