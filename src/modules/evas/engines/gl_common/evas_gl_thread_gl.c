#include "evas_gl_common.h"


#ifdef EVAS_GL_RENDER_THREAD_COMPILE_FOR_GL_GENERIC

#define GLSHADERSOURCE_COPY_VARIABLE GLchar **string_copied
#define GLSHADERSOURCE_COPY_VARIABLE_FREE \
   if (thread_data->string_copied) \
     { \
        int i; \
        for (i = 0; i < thread_data->count; i++) \
          { \
             if (thread_data->string_copied[i]) \
                eina_mempool_free(_mp_default, thread_data->string_copied[i]); \
          }\
        eina_mempool_free(_mp_default, thread_data->string_copied); \
     }

#define GLSHADERSOURCE_COPY_VARIABLE_INIT \
   thread_data->string_copied = NULL;

#define GLSHADERSOURCE_COPY_TO_MEMPOOL \
   if (string) \
     { \
        /* 1. check memory size */ \
        if ((unsigned int)(sizeof(char *) * count) > _mp_default_memory_size) \
          { \
             thread_mode = EVAS_GL_THREAD_MODE_FINISH; \
             goto finish; \
          } \
      int i, len = 0; \
      for (i = 0; i < count; i++) \
        { \
           if (length) len = length[i]; \
           else        len = strlen(string[i]); \
           if ((unsigned int)len + 1 > _mp_default_memory_size) \
             { \
                thread_mode = EVAS_GL_THREAD_MODE_FINISH; \
                goto finish; \
             } \
        } \
      /* 2. malloc & copy */ \
      thread_data->string_copied = eina_mempool_malloc(_mp_default, sizeof(char *) * count); \
      if (thread_data->string_copied) \
        { \
           memset(thread_data->string_copied, 0x00, sizeof(char *) * count); \
           for (i = 0, len = 0; i < count; i++) \
             { \
                if (length) len = length[i]; \
                else        len = strlen(string[i]); \
                thread_data->string_copied[i] = eina_mempool_malloc(_mp_default, len + 1); \
                if (thread_data->string_copied[i]) \
                  { \
                     memcpy(thread_data->string_copied[i], string[i], len + 1); \
                  } \
                else \
                  { \
                     thread_mode = EVAS_GL_THREAD_MODE_FINISH; \
                     goto finish; \
                  } \
             } \
        } \
      else \
        { \
           thread_mode = EVAS_GL_THREAD_MODE_FINISH; \
           goto finish; \
        } \
      /* 3. replace */ \
      thread_data->string = (const GLchar **)thread_data->string_copied; \
   }

static int
get_size(GLenum format, GLenum type)
{
   int csize = 0, comp = 0;
   switch (type)
     {
#ifdef GL_UNSIGNED_BYTE_3_3_2
        case GL_UNSIGNED_BYTE_3_3_2:         csize = sizeof(GLubyte); comp = 1; break;
#endif
#ifdef GL_UNSIGNED_BYTE_2_3_3
        case GL_UNSIGNED_BYTE_2_3_3:         csize = sizeof(GLubyte); comp = 1; break;
#endif

#ifdef GL_UNSIGNED_SHORT_5_6_5
        case GL_UNSIGNED_SHORT_5_6_5:        csize = sizeof(GLushort); comp = 1; break;
#endif
#ifdef GL_UNSIGNED_SHORT_5_6_5_REV
        case GL_UNSIGNED_SHORT_5_6_5_REV:    csize = sizeof(GLushort); comp = 1; break;
#endif
#ifdef GL_UNSIGNED_SHORT_4_4_4_4
        case GL_UNSIGNED_SHORT_4_4_4_4:      csize = sizeof(GLushort); comp = 1; break;
#endif
#ifdef GL_UNSIGNED_SHORT_4_4_4_4_REV
        case GL_UNSIGNED_SHORT_4_4_4_4_REV:  csize = sizeof(GLushort); comp = 1; break;
#endif
#ifdef GL_UNSIGNED_SHORT_5_5_5_1
        case GL_UNSIGNED_SHORT_5_5_5_1:      csize = sizeof(GLushort); comp = 1; break;
#endif
#ifdef GL_UNSIGNED_SHORT_1_5_5_5_REV
        case GL_UNSIGNED_SHORT_1_5_5_5_REV:  csize = sizeof(GLushort); comp = 1; break;
#endif

#ifdef GL_UNSIGNED_INT_8_8_8_8
        case GL_UNSIGNED_INT_8_8_8_8:        csize = sizeof(GLuint); comp = 1; break;
#endif
#ifdef GL_UNSIGNED_INT_8_8_8_8_REV
        case GL_UNSIGNED_INT_8_8_8_8_REV:    csize = sizeof(GLuint); comp = 1; break;
#endif
#ifdef GL_UNSIGNED_INT_10_10_10_2
        case GL_UNSIGNED_INT_10_10_10_2:     csize = sizeof(GLuint); comp = 1; break;
#endif
#ifdef GL_UNSIGNED_INT_2_10_10_10_REV
        case GL_UNSIGNED_INT_2_10_10_10_REV: csize = sizeof(GLuint); comp = 1; break;
#endif

        case GL_UNSIGNED_BYTE:
        case GL_BYTE:
           csize = sizeof(GLbyte);
           break;

        case GL_UNSIGNED_SHORT:
        case GL_SHORT:
           csize = sizeof(GLshort);
           break;

        case GL_UNSIGNED_INT:
        case GL_INT:
           csize = sizeof(GLint);
           break;

        case GL_FLOAT:
           csize = sizeof(GLfloat);
           break;

        default:
           return -1;
     }

   if (comp == 0)
     {
        switch (format)
          {
             case GL_RED:
             case GL_RED_INTEGER:
                comp = 1;
                break;

             case GL_RG:
             case GL_RG_INTEGER:
                comp = 2;
                break;

             case GL_RGB:
             case GL_RGB_INTEGER:
             case GL_BGR:
#ifdef GL_BGR_INTEGER
             case GL_BGR_INTEGER:
#endif
                comp = 3;
                break;

             case GL_RGBA:
             case GL_BGRA:
             case GL_RGBA_INTEGER:
#ifdef GL_BGRA_INTEGER
             case GL_BGRA_INTEGER:
#endif
                comp = 4;
                break;

             default:
                return -1;
          }
     }

   return csize * comp;
}


#define GLTEXIMAGE2D_COPY_VARIABLE void *pixels_copied
#define GLTEXIMAGE2D_COPY_VARIABLE_FREE \
   if (thread_data->pixels_copied) \
      eina_mempool_free(_mp_texture, thread_data->pixels_copied);

#define GLTEXIMAGE2D_COPY_VARIABLE_INIT \
   thread_data->pixels_copied = NULL;

#define GLTEXIMAGE2D_COPY_TO_MEMPOOL \
   int size = get_size(format, type); \
   if (size < 0) \
     { \
        thread_mode = EVAS_GL_THREAD_MODE_FINISH; \
        goto finish; \
     } \
   if (pixels) \
     { \
        /* 1. check memory size */ \
        unsigned int copy_size = (width + (border * 2)) * (height + (border * 2)) * size; \
        if (copy_size > _mp_texture_memory_size) \
          { \
             thread_mode = EVAS_GL_THREAD_MODE_FINISH; \
             goto finish; \
          } \
        /* 2. malloc & copy */ \
        thread_data->pixels_copied = eina_mempool_malloc(_mp_texture, copy_size); \
        if (thread_data->pixels_copied) \
          { \
             memcpy(thread_data->pixels_copied, pixels, copy_size); \
          } \
        else \
          { \
             thread_mode = EVAS_GL_THREAD_MODE_FINISH; \
             goto finish; \
          } \
        /* 3. replace */ \
        thread_data->pixels = (const void *)thread_data->pixels_copied; \
     }


#define GLTEXSUBIMAGE2D_COPY_VARIABLE void *pixels_copied
#define GLTEXSUBIMAGE2D_COPY_VARIABLE_FREE \
   if (thread_data->pixels_copied) \
      eina_mempool_free(_mp_texture, thread_data->pixels_copied);

#define GLTEXSUBIMAGE2D_COPY_VARIABLE_INIT \
   thread_data->pixels_copied = NULL;

#define GLTEXSUBIMAGE2D_COPY_TO_MEMPOOL \
   int size = get_size(format, type); \
   if (size < 0) \
     { \
        thread_mode = EVAS_GL_THREAD_MODE_FINISH; \
        goto finish; \
     } \
   if (pixels) \
     { \
        /* 1. check memory size */ \
        unsigned int copy_size = width * height * size; \
        if (copy_size > _mp_texture_memory_size) \
          { \
             thread_mode = EVAS_GL_THREAD_MODE_FINISH; \
             goto finish; \
          } \
        /* 2. malloc & copy */ \
        thread_data->pixels_copied = eina_mempool_malloc(_mp_texture, copy_size); \
        if (thread_data->pixels_copied) \
          { \
             memcpy(thread_data->pixels_copied, pixels, copy_size); \
          } \
        else \
          { \
             thread_mode = EVAS_GL_THREAD_MODE_FINISH; \
             goto finish; \
          } \
        /* 3. replace */ \
        thread_data->pixels = (const void *)thread_data->pixels_copied; \
     }

#define GLCOMPRESSEDTEXIMAGE2D_COPY_VARIABLE void *data_copied
#define GLCOMPRESSEDTEXIMAGE2D_COPY_VARIABLE_FREE \
   if (thread_data->data_copied) \
      eina_mempool_free(_mp_texture, thread_data->data_copied);

#define GLCOMPRESSEDTEXIMAGE2D_COPY_VARIABLE_INIT \
   thread_data->data_copied = NULL;

#define GLCOMPRESSEDTEXIMAGE2D_COPY_TO_MEMPOOL \
   if (data) \
     { \
        /* 1. check memory size */ \
        if ((unsigned int)imageSize > _mp_texture_memory_size) \
          { \
             thread_mode = EVAS_GL_THREAD_MODE_FINISH; \
             goto finish; \
          } \
        /* 2. malloc & copy */ \
        thread_data->data_copied = eina_mempool_malloc(_mp_texture, imageSize); \
        if (thread_data->data_copied) \
          { \
             memcpy(thread_data->data_copied, data, imageSize); \
          } \
        else \
          { \
             thread_mode = EVAS_GL_THREAD_MODE_FINISH; \
             goto finish; \
          } \
        /* 3. replace */ \
        thread_data->data = (const void *)thread_data->data_copied; \
     }


#define GLCOMPRESSEDTEXSUBIMAGE2D_COPY_VARIABLE void *data_copied
#define GLCOMPRESSEDTEXSUBIMAGE2D_COPY_VARIABLE_FREE \
   if (thread_data->data_copied) \
      eina_mempool_free(_mp_texture, thread_data->data_copied);

#define GLCOMPRESSEDTEXSUBIMAGE2D_COPY_VARIABLE_INIT \
   thread_data->data_copied = NULL;

#define GLCOMPRESSEDTEXSUBIMAGE2D_COPY_TO_MEMPOOL \
   if (data) \
     { \
        /* 1. check memory size */ \
        if ((unsigned int)imageSize > _mp_texture_memory_size) \
          { \
             thread_mode = EVAS_GL_THREAD_MODE_FINISH; \
             goto finish; \
          } \
        /* 2. malloc & copy */ \
        thread_data->data_copied = eina_mempool_malloc(_mp_texture, imageSize); \
        if (thread_data->data_copied) \
          { \
             memcpy(thread_data->data_copied, data, imageSize); \
          } \
        else \
          { \
             thread_mode = EVAS_GL_THREAD_MODE_FINISH; \
             goto finish; \
          } \
        /* 3. replace */ \
        thread_data->data = (const void *)thread_data->data_copied; \
     }


#include "evas_gl_thread_gl_generated.c"
#include "evas_gl_thread_evgl_generated.c"
#include "evas_gl_thread_evgl_api_generated.c"

EAPI void
glTexImage2DEVAS_evgl_thread_cmd(int finish_mode, GLenum target, GLint level, GLint internalformat, GLsizei width, GLsizei height, GLint border, GLenum format, GLenum type, const void *pixels)
{
   if (!evas_evgl_thread_enabled())
     {
        glTexImage2D(target, level, internalformat, width, height, border, format, type, pixels);
        return;
     }

   int thread_mode = EVAS_GL_THREAD_MODE_FINISH;

   EVGL_Thread_Command_glTexImage2D thread_data_local;
   EVGL_Thread_Command_glTexImage2D *thread_data = &thread_data_local;

   /* command_allocated flag init. */
   thread_data->command_allocated = 0;

   if (!evas_gl_thread_force_finish() && !finish_mode)
     { /* _flush */
        EVGL_Thread_Command_glTexImage2D *thread_data_new;
        thread_data_new = eina_mempool_malloc(_mp_command,
                                              sizeof(EVGL_Thread_Command_glTexImage2D));
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
                              _evgl_thread_glTexImage2D,
                              thread_data,
                              thread_mode);
}

EAPI void
glTexSubImage2DEVAS_thread_cmd(int thread_push, GLenum target, GLint level, GLint xoffset, GLint yoffset, GLsizei width, GLsizei height, GLenum format, GLenum type, const void *pixels)
{
   if (!evas_gl_thread_enabled())
     {
        glTexSubImage2D(target, level, xoffset, yoffset, width, height, format, type, pixels);
        return;
     }

   int thread_mode = EVAS_GL_THREAD_MODE_FINISH;

   Evas_Thread_Command_glTexSubImage2D thread_data_local;
   Evas_Thread_Command_glTexSubImage2D *thread_data = &thread_data_local;

   /* command_allocated flag init. */
   thread_data->command_allocated = 0;

   if (!evas_gl_thread_force_finish())
     { /* _flush */
        Evas_Thread_Command_glTexSubImage2D *thread_data_new;
        thread_data_new = eina_mempool_malloc(_mp_command,
                                               sizeof(Evas_Thread_Command_glTexSubImage2D));
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

   if (thread_push)
     {
        /* NON COPY FASTPATH. Must finish before end of functional logic */
        thread_mode = EVAS_GL_THREAD_MODE_ENQUEUE;
     }
   else
     {
        GLTEXSUBIMAGE2D_COPY_TO_MEMPOOL; /* TODO */
     }

finish:
   evas_gl_thread_cmd_enqueue(EVAS_GL_THREAD_TYPE_GL,
                              _gl_thread_glTexSubImage2D,
                              thread_data,
                              thread_mode);
}


#else  /* ! EVAS_GL_RENDER_THREAD_COMPILE_FOR_GL_GENERIC */


#include <dlfcn.h>
#include "evas_gl_thread_gl_link_generated.c"
#include "evas_gl_thread_evgl_link_generated.c"

void (*glTexImage2DEVAS_evgl_thread_cmd)(int finish_mode, GLenum target, GLint level, GLint internalformat, GLsizei width, GLsizei height, GLint border, GLenum format, GLenum type, const void *pixels) = NULL;
void (*glTexSubImage2DEVAS_thread_cmd)(int thread_push, GLenum target, GLint level, GLint xoffset, GLint yoffset, GLsizei width, GLsizei height, GLenum format, GLenum type, const void *pixels) = NULL;

void
_gl_thread_link_init()
{
#define LINK2GENERIC(sym) \
   sym = dlsym(RTLD_DEFAULT, #sym); \
   if (!sym) ERR("Could not find function '%s'", #sym);

   LINK2GENERIC(glGetError_thread_cmd);

   _gl_thread_link_gl_generated_init();
   _gl_thread_link_evgl_generated_init();
}

#endif /* EVAS_GL_RENDER_THREAD_COMPILE_FOR_GL_GENERIC */
