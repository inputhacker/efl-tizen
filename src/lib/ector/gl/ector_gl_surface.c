#ifdef HAVE_CONFIG_H
# include "config.h"
#endif

#include <Ector.h>
#include <gl/Ector_Gl.h>
#include "ector_private.h"
#include "ector_gl_private.h"

typedef struct _Ector_Renderer_Gl_Base_Data Ector_Renderer_Gl_Base_Data;
struct _Ector_Renderer_Gl_Base_Data
{
};

static const char vertex_shader[] =
   "#ifdef GL_ES\n"
   "precision mediump float;\n"
   "#endif\n"
   "attribute vec4 vertex;\n"
   "attribute vec2 tex_coord;\n"
   "uniform mat4 mvp;\n"
   "uniform vec4 pos;\n"
   "void main()\n"
   "{\n"
   "   vec4 new_vert = pos + vertex;\n"
   "   gl_Position = mvp * new_vert;\n"
   "}\n";

static const char fragment_shader[] =
   "#ifdef GL_ES\n"
   "precision mediump float;\n"
   "# endif\n"
   "uniform vec4 color;\n"
   "void main()\n"
   "{\n"
   "   gl_FragColor = color;\n"
   "}\n";

static const char tex_vertex_shader[] =
   "#ifdef GL_ES\n"
   "precision mediump float;\n"
   "#endif\n"
   "attribute vec4 vertex;\n"
   "attribute vec2 tex_coord;\n"
   "uniform mat4 mvp;\n"
   "uniform vec4 pos;\n"
   "varying vec2 v_tex_coord;\n"
   "void main()\n"
   "{\n"
   "   vec4 new_vert = pos + vertex;\n"
   "   gl_Position = mvp * new_vert;\n"
   "   v_tex_coord = tex_coord;\n"
   "}\n";

static const char tex_fragment_shader[] =
   "#ifdef GL_ES\n"
   "precision mediump float;\n"
   "# endif\n"
   "varying vec2 v_tex_coord;\n"
   "uniform sampler2D texture;\n"
   "void main()\n"
   "{\n"
   "   gl_FragColor = texture2D(texture, v_tex_coord);\n"
   "}\n";

static void
init_matrix(float matrix[16])
{
   matrix[0] = 1.0f;
   matrix[1] = 0.0f;
   matrix[2] = 0.0f;
   matrix[3] = 0.0f;

   matrix[4] = 0.0f;
   matrix[5] = 1.0f;
   matrix[6] = 0.0f;
   matrix[7] = 0.0f;

   matrix[8] = 0.0f;
   matrix[9] = 0.0f;
   matrix[10] = 1.0f;
   matrix[11] = 0.0f;

   matrix[12] = 0.0f;
   matrix[13] = 0.0f;
   matrix[14] = 0.0f;
   matrix[15] = 1.0f;
}

static void
matrix_ortho(GLfloat *m,
             GLfloat l, GLfloat r,
             GLfloat t, GLfloat b,
             GLfloat near_, GLfloat far_,
             int rot, int vw, int vh,
             int foc, GLfloat orth)
{
   GLfloat rotf;
   GLfloat cosv, sinv;
   GLfloat tx, ty;

   rotf = (((rot / 90) & 0x3) * M_PI) / 2.0;

   tx = -0.5 * (1.0 - orth);
   ty = -0.5 * (1.0 - orth);

   if (rot == 90)
     {
        tx += -(vw * 1.0);
        ty += -(vh * 0.0);
     }
   if (rot == 180)
     {
        tx += -(vw * 1.0);
        ty += -(vh * 1.0);
     }
   if (rot == 270)
     {
        tx += -(vw * 0.0);
        ty += -(vh * 1.0);
     }

   cosv = cos(rotf);
   sinv = sin(rotf);

   m[0] = (2.0 / (r - l)) * ( cosv);
   m[1] = (2.0 / (r - l)) * ( sinv);
   m[2] = 0.0;
   m[3] = 0.0;

   m[4] = (2.0 / (t - b)) * (-sinv);
   m[5] = (2.0 / (t - b)) * ( cosv);
   m[6] = 0.0;
   m[7] = 0.0;

   m[8] = 0.0;
   m[9] = 0.0;
   m[10] = -(2.0 / (far_ - near_));
   m[11] = 1.0 / (GLfloat)foc;

   m[12] = (m[0] * tx) + (m[4] * ty) - ((r + l) / (r - l));
   m[13] = (m[1] * tx) + (m[5] * ty) - ((t + b) / (t - b));
   m[14] = (m[2] * tx) + (m[6] * ty) - ((near_ + far_) / (far_ - near_));
   m[15] = (m[3] * tx) + (m[7] * ty) + orth;
}

static void
_destroy_shader_program(Shader_Program *shader)
{
   if (shader)
     {
        if (shader->v_shader) GL.glDeleteShader(shader->v_shader);
        if (shader->f_shader)GL.glDeleteShader(shader->f_shader);
        if (shader->program) GL.glDeleteProgram(shader->program);
        free(shader);
     }
}
static Shader_Program *
_create_shader_program(const char *v_shader, const char* f_shader)
{
   Shader_Program *shader = calloc(1, sizeof(Shader_Program));
   GLint ok = 0;

   shader->v_shader = GL.glCreateShader(GL_VERTEX_SHADER);
   GL.glShaderSource(shader->v_shader, 1, &v_shader, NULL);
   GL.glCompileShader(shader->v_shader);
   GL.glGetShaderiv(shader->v_shader, GL_COMPILE_STATUS, &ok);
   if (!ok)
     {
        ERR("vertex shader compilation failed ");
        goto fail;
     }

   shader->f_shader = GL.glCreateShader(GL_FRAGMENT_SHADER);
   GL.glShaderSource(shader->f_shader, 1, &f_shader, NULL);
   GL.glCompileShader(shader->f_shader);
   GL.glGetShaderiv(shader->f_shader, GL_COMPILE_STATUS, &ok);
   if (!ok)
     {
        ERR("fragment shader compilation failed ");
        goto fail;
     }

   shader->program = GL.glCreateProgram();
   GL.glAttachShader(shader->program, shader->v_shader);
   GL.glAttachShader(shader->program, shader->f_shader);
   GL.glBindAttribLocation(shader->program, SHAD_VERTEX, "vertex");
   GL.glBindAttribLocation(shader->program, SHAD_TEXUV,  "v_tex_coord");
   GL.glLinkProgram(shader->program);
   GL.glGetProgramiv(shader->program, GL_LINK_STATUS, &ok);
   if (!ok)
     {
        ERR("linking failed for shader program");
        goto fail;
     }
   shader->u_color = GL.glGetUniformLocation(shader->program, "color");
   shader->u_pos = GL.glGetUniformLocation(shader->program, "pos");
   shader->u_mvp = GL.glGetUniformLocation(shader->program, "mvp");
   shader->u_texture = GL.glGetUniformLocation(shader->program, "texture");
   return shader;
fail:
   _destroy_shader_program(shader);
   return NULL;
}

static Ector_Renderer *
_ector_gl_surface_ector_generic_surface_renderer_factory_new(Eo *obj,
                                                             Ector_Gl_Surface_Data *pd EINA_UNUSED,
                                                             const Eo_Class *type)
{
   if (type == ECTOR_RENDERER_GENERIC_SHAPE_MIXIN)
     return eo_add(ECTOR_RENDERER_GL_SHAPE_CLASS, obj);
   else if (type == ECTOR_RENDERER_GENERIC_GRADIENT_LINEAR_MIXIN)
     return eo_add(ECTOR_RENDERER_GL_GRADIENT_LINEAR_CLASS, obj);
   else if (type == ECTOR_RENDERER_GENERIC_GRADIENT_RADIAL_MIXIN)
     return eo_add(ECTOR_RENDERER_GL_GRADIENT_RADIAL_CLASS, obj);
   ERR("Couldn't find class for type: %s\n", eo_class_name_get(type));
   return NULL;
}

static void
_ector_gl_surface_path_fill(Eo *obj EINA_UNUSED,
                              Ector_Gl_Surface_Data *pd,
                              float *vertex,
                              unsigned int vertex_count EINA_UNUSED,
                              int *stops,
                              unsigned int stop_count,
                              unsigned int color)
{
   unsigned int i, previous_stop = 0, stop = 0;

   GL.glUniform4f(pd->simple_shader->u_color, (R_VAL(&color)/255.0), (G_VAL(&color)/255.0), (B_VAL(&color)/255.0), (A_VAL(&color)/255.0));
   GL.glVertexAttribPointer(SHAD_VERTEX, 2, GL_FLOAT, GL_FALSE, 0, vertex);

   // 1. Disable color writes
   GL.glColorMask(GL_FALSE, GL_FALSE, GL_FALSE, GL_FALSE);

   // 2. Enable Stencil and draw the shape in stencil
   GL.glEnable(GL_STENCIL_TEST);
   GL.glStencilMask(0xff);
   GL.glClear(GL_STENCIL_BUFFER_BIT);
   GL.glStencilMask(GL_STENCIL_HIGH_BIT);

   // 3. set the Op for odd/even fill
   GL.glStencilOp(GL_KEEP, GL_KEEP, GL_INVERT); // Simply invert the stencil bit

   // 4. draw the shape
   for (i = 0 ; i < stop_count; i++)
     {
        stop = stops[i];
        GL.glDrawArrays(GL_TRIANGLE_FAN, previous_stop/2, (stop - previous_stop)/2);
        previous_stop = stop;
      }

   // 5. Enable color writes
   GL.glColorMask(GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE);

   // 6. setup stencil op
   GL.glStencilOp(GL_KEEP, GL_REPLACE, GL_REPLACE);
   GL.glStencilFunc(GL_NOTEQUAL, 0, GL_STENCIL_HIGH_BIT);

   if (A_VAL(&color) == 255)
     {
         GL.glDisable(GL_BLEND);
     }
   else
     {
        GL.glEnable(GL_BLEND);
     }

   // 7.draw a bounding rectangle covering the shape
   float rect_bound[10] = {0, 0 , pd->dest_fbo.w, 0, pd->dest_fbo.w, pd->dest_fbo.h, 0, pd->dest_fbo.h};
   GL.glVertexAttribPointer(SHAD_VERTEX, 2, GL_FLOAT, GL_FALSE, 0, rect_bound);
   GL.glDrawArrays(GL_TRIANGLE_FAN, 0, 4);

   // reset
   GL.glStencilFunc(GL_ALWAYS, 0, 0xff);
   GL.glDisable(GL_STENCIL_TEST);
}

static void
_ector_gl_surface_path_stroke(Eo *obj EINA_UNUSED,
                            Ector_Gl_Surface_Data *pd,
                            float *vertex,
                            unsigned int vertex_count,
                            unsigned int color)
{
   GL.glUniform4f(pd->simple_shader->u_color, (R_VAL(&color)/255.0), (G_VAL(&color)/255.0), (B_VAL(&color)/255.0), (A_VAL(&color)/255.0));
   GL.glVertexAttribPointer(SHAD_VERTEX, 2, GL_FLOAT, GL_FALSE, 0, vertex);

   if (A_VAL(&color) == 255)
     {
        GL.glDisable(GL_BLEND);
        GL.glDrawArrays(GL_TRIANGLE_STRIP, 0, vertex_count/2);
        return;
     }
   else
     {
        GL.glEnable(GL_BLEND);

        GL.glColorMask(GL_FALSE, GL_FALSE, GL_FALSE, GL_FALSE); // Disable color writes

        GL.glEnable(GL_STENCIL_TEST);
        GL.glStencilMask(0xff);
        GL.glClear(GL_STENCIL_BUFFER_BIT);
        GL.glStencilMask(GL_STENCIL_HIGH_BIT);

        GL.glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);

        GL.glStencilFunc(GL_ALWAYS, GL_STENCIL_HIGH_BIT, 0xff);

        GL.glDrawArrays(GL_TRIANGLE_STRIP, 0, vertex_count/2); // here the shape is drawn in the stencil

        // Enable color writes & disable stencil writes
        GL.glColorMask(GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE);

        GL.glStencilOp(GL_KEEP, GL_REPLACE, GL_REPLACE);
        // Pass when high bit is set, replace stencil value with 0
        GL.glStencilFunc(GL_NOTEQUAL, 0, GL_STENCIL_HIGH_BIT);

         // draw a rectangle
         float rect_bound[10] = {0, 0 , pd->dest_fbo.w, 0, pd->dest_fbo.w, pd->dest_fbo.h, 0, pd->dest_fbo.h};
         GL.glVertexAttribPointer(SHAD_VERTEX, 2, GL_FLOAT, GL_FALSE, 0, rect_bound);
         GL.glDrawArrays(GL_TRIANGLE_FAN, 0, 4);

        // reset
        GL.glStencilFunc(GL_ALWAYS, 0, 0xff);
        GL.glDisable(GL_STENCIL_TEST);
     }
}

static void
_delete_fbo(Ector_Gl_Surface_Data *pd)
{
   if (!pd->scratch_fbo.fbo) return;

   if (pd->scratch_fbo.rbo_color)
     GL.glDeleteRenderbuffers(1, &(pd->scratch_fbo.rbo_color));
   if (pd->scratch_fbo.rbo_stencil)
     GL.glDeleteRenderbuffers(1, &(pd->scratch_fbo.rbo_stencil));
   if (pd->scratch_fbo.texture)
     GL.glDeleteTextures(1, &(pd->scratch_fbo.texture));
   if (pd->scratch_fbo.fbo)
     GL.glDeleteFramebuffers(1, &(pd->scratch_fbo.fbo));

   pd->scratch_fbo.fbo = 0;
   pd->scratch_fbo.rbo_color = 0;
   pd->scratch_fbo.rbo_stencil = 0;
   pd->scratch_fbo.texture = 0;
}

static void
_create_msaa_fbo_rbo(Ector_Gl_Surface_Data *pd)
{
   GL.glGenRenderbuffers(1, &pd->scratch_fbo.rbo_color);
   GL.glBindRenderbuffer(GL_RENDERBUFFER, pd->scratch_fbo.rbo_color);
   GL.glRenderbufferStorageMultisample(GL_RENDERBUFFER,
                                       pd->scratch_fbo.msaa_level,
                                       GL_RGBA,
                                       pd->scratch_fbo.w, pd->scratch_fbo.h);

   // create a 4x MSAA renderbuffer object for depthbuffer
   GL.glGenRenderbuffers(1, &pd->scratch_fbo.rbo_stencil);
   GL.glBindRenderbuffer(GL_RENDERBUFFER, pd->scratch_fbo.rbo_stencil);
   GL.glRenderbufferStorageMultisample(GL_RENDERBUFFER,
                                       pd->scratch_fbo.msaa_level,
                                       GL_STENCIL_INDEX8,
                                       pd->scratch_fbo.w, pd->scratch_fbo.h);


   GL.glGenFramebuffers(1, &pd->scratch_fbo.fbo);
   GL.glBindFramebuffer(GL_FRAMEBUFFER, pd->scratch_fbo.fbo);

   // attach colorbuffer image to FBO
   GL.glFramebufferRenderbuffer(GL_FRAMEBUFFER,
                                GL_COLOR_ATTACHMENT0,
                                GL_RENDERBUFFER,
                                pd->scratch_fbo.rbo_color);

   // attach stencil buffer to FBO
   GL.glFramebufferRenderbuffer(GL_FRAMEBUFFER,
                                GL_STENCIL_ATTACHMENT,
                                GL_RENDERBUFFER,
                                pd->scratch_fbo.rbo_stencil);

   // check FBO status
   GLenum status = GL.glCheckFramebufferStatus(GL_FRAMEBUFFER);
   if(status != GL_FRAMEBUFFER_COMPLETE)
     {
        ERR("cretaing MSAA fbo rbo failed");
        GL.glDeleteRenderbuffers(1, &(pd->scratch_fbo.rbo_color));
        GL.glDeleteRenderbuffers(1, &(pd->scratch_fbo.rbo_stencil));
        GL.glDeleteFramebuffers(1, &(pd->scratch_fbo.fbo));
        pd->scratch_fbo.fbo = 0;
     }
}

static void
_create_msaa_fbo_texture_ext(Ector_Gl_Surface_Data *pd)
{
   GL.glGenTextures(1, &pd->scratch_fbo.texture);
   GL.glBindTexture(GL_TEXTURE_2D, pd->scratch_fbo.texture);
   GL.glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
   GL.glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
   GL.glTexImage2D(GL_TEXTURE_2D, 0,
                   GL_RGBA, pd->scratch_fbo.w, pd->scratch_fbo.h, 0, GL_RGBA,
                   GL_UNSIGNED_BYTE, NULL);
   GL.glBindTexture(GL_TEXTURE_2D, 0);


   // create a MSAA renderbuffer object for stencil buffer
   GL.glGenRenderbuffers(1, &pd->scratch_fbo.rbo_stencil);
   GL.glBindRenderbuffer(GL_RENDERBUFFER, pd->scratch_fbo.rbo_stencil);
   GL.glRenderbufferStorageMultisampleEXT(GL_RENDERBUFFER,
                                          pd->scratch_fbo.msaa_level,
                                          GL_STENCIL_INDEX8,
                                          pd->scratch_fbo.w, pd->scratch_fbo.h);


   GL.glGenFramebuffers(1, &pd->scratch_fbo.fbo);
   GL.glBindFramebuffer(GL_FRAMEBUFFER, pd->scratch_fbo.fbo);

   // attach texture to FBO
   GL.glFramebufferTexture2DMultisampleEXT(GL_FRAMEBUFFER,
                                           GL_COLOR_ATTACHMENT0,
                                           GL_TEXTURE_2D,
                                           pd->scratch_fbo.texture,
                                           0,
                                           pd->scratch_fbo.msaa_level);

   // attach stencil buffer to FBO
   GL.glFramebufferRenderbuffer(GL_FRAMEBUFFER,
                                GL_STENCIL_ATTACHMENT,
                                GL_RENDERBUFFER,
                                pd->scratch_fbo.rbo_stencil);

   // check FBO status
   GLenum status = GL.glCheckFramebufferStatus(GL_FRAMEBUFFER);
   if(status != GL_FRAMEBUFFER_COMPLETE)
     {
        ERR("cretaing MSAA fbo texture failed");
        GL.glDeleteTextures(1, &(pd->scratch_fbo.texture));
        GL.glDeleteRenderbuffers(1, &(pd->scratch_fbo.rbo_stencil));
        GL.glDeleteFramebuffers(1, &(pd->scratch_fbo.fbo));
        pd->scratch_fbo.fbo = 0;
     }
}

static void
_create_fbo_texture(Ector_Gl_Surface_Data *pd)
{
   GL.glGenTextures(1, &pd->scratch_fbo.texture);
   GL.glBindTexture(GL_TEXTURE_2D, pd->scratch_fbo.texture);
   GL.glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
   GL.glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
   GL.glTexImage2D(GL_TEXTURE_2D, 0,
                   GL_RGBA, pd->scratch_fbo.w, pd->scratch_fbo.h, 0, GL_RGBA,
                   GL_UNSIGNED_BYTE, NULL);
   GL.glBindTexture(GL_TEXTURE_2D, 0);


   GL.glGenRenderbuffers(1, &pd->scratch_fbo.rbo_stencil);
   GL.glBindRenderbuffer(GL_RENDERBUFFER, pd->scratch_fbo.rbo_stencil);
   GL.glRenderbufferStorage(GL_RENDERBUFFER,
                            GL_STENCIL_INDEX8,
                            pd->scratch_fbo.w, pd->scratch_fbo.h);


   GL.glGenFramebuffers(1, &pd->scratch_fbo.fbo);
   GL.glBindFramebuffer(GL_FRAMEBUFFER, pd->scratch_fbo.fbo);

   // attach texture to FBO
   GL.glFramebufferTexture2D(GL_FRAMEBUFFER,
                             GL_COLOR_ATTACHMENT0,
                             GL_TEXTURE_2D,
                             pd->scratch_fbo.texture,
                             0);

   // attach stencil buffer to FBO
   GL.glFramebufferRenderbuffer(GL_FRAMEBUFFER,
                                GL_STENCIL_ATTACHMENT,
                                GL_RENDERBUFFER,
                                pd->scratch_fbo.rbo_stencil);

   // check FBO status
   GLenum status = GL.glCheckFramebufferStatus(GL_FRAMEBUFFER);
   if(status != GL_FRAMEBUFFER_COMPLETE)
     {
        ERR("cretaing normal fbo texture failed");
        GL.glDeleteTextures(1, &(pd->scratch_fbo.texture));
        GL.glDeleteRenderbuffers(1, &(pd->scratch_fbo.rbo_stencil));
        GL.glDeleteFramebuffers(1, &(pd->scratch_fbo.fbo));
        pd->scratch_fbo.fbo = 0;
     }
}


Eina_Bool
_update_scratch_fbo(Ector_Gl_Surface_Data *pd, int width, int height)
{
   GLint current_fbo;

   GL.glGetIntegerv(GL_FRAMEBUFFER_BINDING, &current_fbo);
   // update dest info
   pd->dest_fbo.fbo = current_fbo;
   pd->dest_fbo.w = width;
   pd->dest_fbo.h = height;

   if (pd->scratch_fbo.w >= width && pd->scratch_fbo.h >= height)
     {
        // no need to update the scratch buffer
        GL.glBindFramebuffer(GL_FRAMEBUFFER, pd->scratch_fbo.fbo);
        return EINA_TRUE;
     }

   // delete old buffer if any
   _delete_fbo(pd);

   // update the new size
   pd->scratch_fbo.w = pd->dest_fbo.w > pd->scratch_fbo.w ? pd->dest_fbo.w : pd->scratch_fbo.w;
   pd->scratch_fbo.h = pd->dest_fbo.h > pd->scratch_fbo.h ? pd->dest_fbo.h : pd->scratch_fbo.h;

   if (pd->scratch_fbo.msaa)
     {
        _create_msaa_fbo_rbo(pd);
     }
   else if (pd->scratch_fbo.ext_msaa)
     {
        _create_msaa_fbo_texture_ext(pd);
     }
   else
     {
       _create_fbo_texture(pd);
     }
   return EINA_TRUE;
}


void
_ector_gl_surface_surface_set(Eo *obj EINA_UNUSED,
                              Ector_Gl_Surface_Data *pd,
                              void *pixels EINA_UNUSED, unsigned int width,
                              unsigned int height, unsigned int stride EINA_UNUSED)
{
   if (pixels)
     {
        if (!_update_scratch_fbo(pd, width, height))
          return;

        float  mvp[16];
        GL.glViewport(0, 0, pd->dest_fbo.w, pd->dest_fbo.h);
        GL.glClearStencil(0);
        GL.glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        GL.glClearColor(0, 0, 0, 0);
        GL.glDisable(GL_DEPTH_TEST);
        GL.glDisable(GL_BLEND);
        GL.glDisable(GL_STENCIL_TEST);
        GL.glBlendFunc( GL_ONE, GL_ONE_MINUS_SRC_ALPHA );
        init_matrix(mvp);
        matrix_ortho(mvp,0, pd->dest_fbo.w, pd->dest_fbo.h, 0,-1000000.0, 1000000.0,0, pd->dest_fbo.w, pd->dest_fbo.h,1, 1.0);
        GL.glUseProgram(pd->simple_shader->program);
        GL.glUniformMatrix4fv(pd->simple_shader->u_mvp, 1, GL_FALSE, mvp);
        GL.glEnableVertexAttribArray(SHAD_VERTEX);
     }
   else
    {
       if (pd->scratch_fbo.msaa)
         {
            GL.glBindFramebuffer(GL_READ_FRAMEBUFFER, pd->scratch_fbo.fbo);
            GL.glBindFramebuffer(GL_DRAW_FRAMEBUFFER, pd->dest_fbo.fbo);
            GL.glBlitFramebuffer(0, 0, pd->dest_fbo.w, pd->dest_fbo.h,
                                 0, 0, pd->dest_fbo.w, pd->dest_fbo.h,
                                 GL_COLOR_BUFFER_BIT,
                                 GL_LINEAR);
         }
       else
         {
            float  mvp[16];
            GL.glBindFramebuffer(GL_FRAMEBUFFER, pd->dest_fbo.fbo);
            GL.glViewport(0, 0, pd->dest_fbo.w, pd->dest_fbo.h);
            GL.glClearColor(0, 0, 0, 0);
            GL.glDisable(GL_BLEND);
            GL.glClear(GL_COLOR_BUFFER_BIT);
            init_matrix(mvp);
            matrix_ortho(mvp,0, pd->dest_fbo.w, pd->dest_fbo.h, 0,-1000000.0, 1000000.0,0, pd->dest_fbo.w, pd->dest_fbo.h,1, 1.0);
            GL.glUseProgram(pd->texture_shader->program);
            GL.glUniformMatrix4fv(pd->texture_shader->u_mvp, 1, GL_FALSE, mvp);
            GL.glUniform4f(pd->texture_shader->u_pos, 0, 0, 0, 0);
            GL.glEnableVertexAttribArray(SHAD_VERTEX);
            GL.glEnableVertexAttribArray(SHAD_TEXUV);
            GL.glActiveTexture(GL_TEXTURE0);
            GL.glBindTexture(GL_TEXTURE_2D, pd->scratch_fbo.texture);
            GL.glUniform1i(pd->texture_shader->u_texture, 0);
            float rect_bound[10] = {0, 0 , pd->dest_fbo.w, 0, pd->dest_fbo.w, pd->dest_fbo.h, 0, pd->dest_fbo.h};
            float w_r = (float)pd->dest_fbo.w/ pd->scratch_fbo.w;
            float h_r = (float)pd->dest_fbo.h/ pd->scratch_fbo.h;
            float tex_uv[10] = {0, 0 , w_r, 0, w_r, h_r, 0, h_r};
            GL.glVertexAttribPointer(SHAD_VERTEX, 2, GL_FLOAT, GL_FALSE, 0, rect_bound);
            GL.glVertexAttribPointer(SHAD_TEXUV, 2, GL_FLOAT, GL_FALSE, 0, tex_uv);
            GL.glDrawArrays(GL_TRIANGLE_FAN, 0, 4);
         }
    }
}

static void
_ector_gl_surface_offset(Eo *obj EINA_UNUSED,
                         Ector_Gl_Surface_Data *pd, int x, int y)
{
   pd->offset.x = x;
   pd->offset.y = y;
   GL.glUniform4f(pd->simple_shader->u_pos, pd->ref_point.x + pd->offset.x, pd->ref_point.y + pd->offset.y, 0, 0);
}

void
_ector_gl_surface_surface_get(Eo *obj EINA_UNUSED,
                              Ector_Gl_Surface_Data *pd EINA_UNUSED,
                              void **pixels EINA_UNUSED, unsigned int *width EINA_UNUSED,
                              unsigned int *height EINA_UNUSED, unsigned int *stride EINA_UNUSED)
{

}

static Eo *
_ector_gl_surface_eo_base_constructor(Eo *obj,
                                      Ector_Gl_Surface_Data *pd EINA_UNUSED)
{
   const char *ector_msaa = NULL;

   obj = eo_do_super_ret(obj, ECTOR_GL_SURFACE_CLASS, obj, eo_constructor());

   GL.finalize();

   pd->simple_shader = _create_shader_program(vertex_shader, fragment_shader);
   pd->texture_shader = _create_shader_program(tex_vertex_shader, tex_fragment_shader);
   ector_msaa = getenv("ECTOR_MSAA");
   if (ector_msaa)
     {
        //@TODO parse the msaa level;
        pd->scratch_fbo.msaa_level = 4;
     }
   else
     {
        pd->scratch_fbo.msaa_level = 4;
     }

   if (GL.version == 3)
     pd->scratch_fbo.msaa = EINA_TRUE;
   if (GL.ext_ms_render_to_tex)
     pd->scratch_fbo.ext_msaa = EINA_TRUE;

   return obj;
}

static void
_ector_gl_surface_eo_base_destructor(Eo *obj EINA_UNUSED,
                                     Ector_Gl_Surface_Data *pd EINA_UNUSED)
{
  _destroy_shader_program(pd->simple_shader);
  _destroy_shader_program(pd->texture_shader);
   _delete_fbo(pd);

   eo_do_super(obj, ECTOR_GL_SURFACE_CLASS, eo_destructor());
}

static void
_ector_gl_surface_ector_generic_surface_reference_point_set(Eo *obj EINA_UNUSED,
                                                            Ector_Gl_Surface_Data *pd,
                                                            int x, int y)
{
   pd->ref_point.x = x;
   pd->ref_point.y = y;
}
#include "ector_gl_surface.eo.c"
#include "ector_renderer_gl_base.eo.c"
