#include "evas_gl_common.h"
#include "evas_gl_thread.h"


#ifdef EVAS_GL_RENDER_THREAD_IS_GENERIC

#define GLSHADERSOURCE_COPY_VARIABLE GLchar **string_copied
#define GLSHADERSOURCE_COPY_VARIABLE_FREE \
   if (thread_param->string_copied) { \
      int i; \
      for (i = 0; i < thread_param->count; i++) { \
        if (thread_param->string_copied[i]) { \
           eina_mempool_free(_mp_default, thread_param->string_copied[i]); \
        } \
      }\
     eina_mempool_free(_mp_default, thread_param->string_copied); \
   }

#define GLSHADERSOURCE_COPY_VARIABLE_INIT \
   thread_param->string_copied = NULL;

#define GLSHADERSOURCE_COPY_TO_MEMPOOL \
   if (string) { \
      /* 1. check memory size */ \
      if ((unsigned int)(sizeof(char *) * count) > _mp_default_memory_size) { \
         thread_mode = EVAS_GL_THREAD_MODE_FINISH; \
         goto finish; \
      } \
      int i, len = 0; \
      for (i = 0; i < count; i++) { \
         if (length) len = length[i]; \
         else        len = strlen(string[i]); \
         if ((unsigned int)len + 1 > _mp_default_memory_size) { \
            thread_mode = EVAS_GL_THREAD_MODE_FINISH; \
            goto finish; \
         } \
      } \
      /* 2. malloc & copy */ \
      thread_param->string_copied = eina_mempool_malloc(_mp_default, sizeof(char *) * count); \
      if (thread_param->string_copied) { \
         memset(thread_param->string_copied, 0x00, sizeof(char *) * count); \
            for (i = 0, len = 0; i < count; i++) { \
               if (length) len = length[i]; \
               else        len = strlen(string[i]); \
               thread_param->string_copied[i] = eina_mempool_malloc(_mp_default, len + 1); \
               if (thread_param->string_copied[i]) { \
                  memcpy(thread_param->string_copied[i], string[i], len + 1); \
               } \
               else { \
                  thread_mode = EVAS_GL_THREAD_MODE_FINISH; \
                  goto finish; \
               } \
            } \
      } \
      else { \
         thread_mode = EVAS_GL_THREAD_MODE_FINISH; \
         goto finish; \
      } \
      /* 3. replace */ \
      thread_param->string = (const GLchar **)thread_param->string_copied; \
   }

static int getSize(GLenum format, GLenum type)
{
   int csize = 0, comp = 0;
   switch(type)
   {
#ifdef GL_UNSIGNED_BYTE_3_3_2
      case GL_UNSIGNED_BYTE_3_3_2:
#endif
#ifdef GL_UNSIGNED_BYTE_2_3_3
     case GL_UNSIGNED_BYTE_2_3_3:
#endif
         csize = 1; comp = 1;

#ifdef GL_UNSIGNED_SHORT_5_6_5
      case GL_UNSIGNED_SHORT_5_6_5:
#endif

#ifdef GL_UNSIGNED_SHORT_5_6_5_REV
     case GL_UNSIGNED_SHORT_5_6_5_REV:
#endif

#ifdef GL_UNSIGNED_SHORT_4_4_4_4
      case GL_UNSIGNED_SHORT_4_4_4_4:
#endif

#ifdef GL_UNSIGNED_SHORT_4_4_4_4_REV
     case GL_UNSIGNED_SHORT_4_4_4_4_REV:
#endif

#ifdef GL_UNSIGNED_SHORT_5_5_5_1
      case GL_UNSIGNED_SHORT_5_5_5_1:
#endif

#ifdef GL_UNSIGNED_SHORT_1_5_5_5_REV
     case GL_UNSIGNED_SHORT_1_5_5_5_REV:
#endif
         csize = 2; comp = 1;

#ifdef GL_UNSIGNED_INT_8_8_8_8
      case GL_UNSIGNED_INT_8_8_8_8:
#endif

#ifdef GL_UNSIGNED_INT_8_8_8_8_REV
     case GL_UNSIGNED_INT_8_8_8_8_REV:
#endif

#ifdef GL_UNSIGNED_INT_10_10_10_2
      case GL_UNSIGNED_INT_10_10_10_2:
#endif

#ifdef GL_UNSIGNED_INT_2_10_10_10_REV
     case GL_UNSIGNED_INT_2_10_10_10_REV:
#endif
         csize = 4; comp = 1;

      case GL_UNSIGNED_BYTE: case GL_BYTE:
         csize = 1;
#ifdef GL_UNGINED_SHORT
      case GL_UNGINED_SHORT:
#endif
     case GL_SHORT:
         csize = 2;
      case GL_UNSIGNED_INT: case GL_INT:
         csize = 4;
      case GL_FLOAT:
         csize = sizeof(float);
      default:
         return -1;
   }

   if (comp == 0) {
      switch(format)
      {
         case GL_RED: case GL_RG:
            comp = 1;
         case GL_RGB: case GL_BGR:
         case GL_RGBA: case GL_BGRA:
         case GL_RED_INTEGER: case GL_RG_INTEGER: case GL_RGB_INTEGER:
#ifdef GL_BGR_INTEGER
         case GL_BGR_INTEGER:
#endif
         case GL_RGBA_INTEGER:
#ifdef GL_BGRA_INTEGER
         case GL_BGRA_INTEGER:
#endif
            comp = 1;
         default:
            return -1;
      }
   }

   return csize * comp;
}


#define GLTEXIMAGE2D_COPY_VARIABLE void *pixels_copied
#define GLTEXIMAGE2D_COPY_VARIABLE_FREE \
   if (thread_param->pixels_copied) \
     eina_mempool_free(_mp_texture, thread_param->pixels_copied);

#define GLTEXIMAGE2D_COPY_VARIABLE_INIT \
   thread_param->pixels_copied = NULL;

#define GLTEXIMAGE2D_COPY_TO_MEMPOOL \
   int size = getSize(format, type); \
   if (size < 0) { \
      thread_mode = EVAS_GL_THREAD_MODE_FINISH; \
      goto finish; \
   } \
   if (pixels) { \
      /* 1. check memory size */ \
      unsigned int copy_size = (width + (border * 2)) * (height + (border * 2)) * size; \
      if (copy_size > _mp_texture_memory_size) { \
         thread_mode = EVAS_GL_THREAD_MODE_FINISH; \
         goto finish; \
     } \
      /* 2. malloc & copy */ \
      thread_param->pixels_copied = eina_mempool_malloc(_mp_texture, copy_size); \
     if (thread_param->pixels_copied) { \
         memcpy(thread_param->pixels_copied, pixels, copy_size); \
     } \
     else { \
         thread_mode = EVAS_GL_THREAD_MODE_FINISH; \
         goto finish; \
     } \
      /* 3. replace */ \
     thread_param->pixels = (const void *)thread_param->pixels_copied; \
   }


#define GLTEXSUBIMAGE2D_COPY_VARIABLE void *pixels_copied
#define GLTEXSUBIMAGE2D_COPY_VARIABLE_FREE \
   if (thread_param->pixels_copied) \
     eina_mempool_free(_mp_texture, thread_param->pixels_copied);

#define GLTEXSUBIMAGE2D_COPY_VARIABLE_INIT \
   thread_param->pixels_copied = NULL;

#define GLTEXSUBIMAGE2D_COPY_TO_MEMPOOL \
   int size = getSize(format, type); \
   if (size < 0) { \
      thread_mode = EVAS_GL_THREAD_MODE_FINISH; \
      goto finish; \
   } \
   if (pixels) { \
      /* 1. check memory size */ \
      unsigned int copy_size = width * height * size; \
      if (copy_size > _mp_texture_memory_size) { \
         thread_mode = EVAS_GL_THREAD_MODE_FINISH; \
         goto finish; \
     } \
      /* 2. malloc & copy */ \
      thread_param->pixels_copied = eina_mempool_malloc(_mp_texture, copy_size); \
     if (thread_param->pixels_copied) { \
         memcpy(thread_param->pixels_copied, pixels, copy_size); \
     } \
     else { \
         thread_mode = EVAS_GL_THREAD_MODE_FINISH; \
         goto finish; \
     } \
      /* 3. replace */ \
     thread_param->pixels = (const void *)thread_param->pixels_copied; \
   }

#define GLCOMPRESSEDTEXIMAGE2D_COPY_VARIABLE void *data_copied
#define GLCOMPRESSEDTEXIMAGE2D_COPY_VARIABLE_FREE \
   if (thread_param->data_copied) \
      eina_mempool_free(_mp_texture, thread_param->data_copied);

#define GLCOMPRESSEDTEXIMAGE2D_COPY_VARIABLE_INIT \
   thread_param->data_copied = NULL;

#define GLCOMPRESSEDTEXIMAGE2D_COPY_TO_MEMPOOL \
    if (data) { \
      /* 1. check memory size */ \
      if ((unsigned int)imageSize > _mp_texture_memory_size) { \
         thread_mode = EVAS_GL_THREAD_MODE_FINISH; \
         goto finish; \
      } \
      /* 2. malloc & copy */ \
      thread_param->data_copied = eina_mempool_malloc(_mp_texture, imageSize); \
      if (thread_param->data_copied) { \
         memcpy(thread_param->data_copied, data, imageSize); \
      } \
      else { \
         thread_mode = EVAS_GL_THREAD_MODE_FINISH; \
         goto finish; \
      } \
      /* 3. replace */ \
      thread_param->data = (const void *)thread_param->data_copied; \
   }


#define GLCOMPRESSEDTEXSUBIMAGE2D_COPY_VARIABLE void *data_copied
#define GLCOMPRESSEDTEXSUBIMAGE2D_COPY_VARIABLE_FREE \
   if (thread_param->data_copied) \
      eina_mempool_free(_mp_texture, thread_param->data_copied);

#define GLCOMPRESSEDTEXSUBIMAGE2D_COPY_VARIABLE_INIT \
   thread_param->data_copied = NULL;

#define GLCOMPRESSEDTEXSUBIMAGE2D_COPY_TO_MEMPOOL \
    if (data) { \
      /* 1. check memory size */ \
      if ((unsigned int)imageSize > _mp_texture_memory_size) { \
         thread_mode = EVAS_GL_THREAD_MODE_FINISH; \
         goto finish; \
      } \
      /* 2. malloc & copy */ \
      thread_param->data_copied = eina_mempool_malloc(_mp_texture, imageSize); \
      if (thread_param->data_copied) { \
         memcpy(thread_param->data_copied, data, imageSize); \
      } \
      else { \
         thread_mode = EVAS_GL_THREAD_MODE_FINISH; \
         goto finish; \
      } \
      /* 3. replace */ \
      thread_param->data = (const void *)thread_param->data_copied; \
   }


#include "evas_gl_thread_gl_generated.c"
#include "evas_gl_thread_evgl_generated.c"
#include "evas_gl_thread_evgl_api_generated.c"

EAPI void
glTexSubImage2DEVAS_thread_cmd(int thread_push, GLenum target, GLint level, GLint xoffset, GLint yoffset, GLsizei width, GLsizei height, GLenum format, GLenum type, const void *pixels)
{
   if (!evas_gl_thread_enabled())
     {
        glTexSubImage2D(target, level, xoffset, yoffset, width, height, format, type, pixels);
        return;
     }

   int thread_mode = EVAS_GL_THREAD_MODE_FINISH;

   Thread_Command_glTexSubImage2D thread_param_local;
   Thread_Command_glTexSubImage2D *thread_param = &thread_param_local;

   /* command_allocated flag init. */
   thread_param->command_allocated = 0;

   if (!evas_gl_thread_force_finish())
     { /* _flush */
        Thread_Command_glTexSubImage2D *thread_param_new;
        thread_param_new = eina_mempool_malloc(_mp_command,
                                               sizeof(Thread_Command_glTexSubImage2D));
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
                              thread_param,
                              thread_mode);
}


#else  /* ! EVAS_GL_RENDER_THREAD_IS_GENERIC */


#include <dlfcn.h>
#include "evas_gl_thread_gl_link_generated.c"
#include "evas_gl_thread_evgl_link_generated.c"

void (*glTexSubImage2DEVAS_thread_cmd)(int thread_push, GLenum target, GLint level, GLint xoffset, GLint yoffset, GLsizei width, GLsizei height, GLenum format, GLenum type, const void *pixels) = NULL;

#endif /* EVAS_GL_RENDER_THREAD_IS_GENERIC */
