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

static GLuint
_init_shader()
{
   const char *p;
   unsigned int     program;
   unsigned int     vtx_shader;
   unsigned int     fgmt_shader;
   GLint ok = 0;

   p = vertex_shader;
   vtx_shader = GL.glCreateShader(GL_VERTEX_SHADER);
   GL.glShaderSource(vtx_shader, 1, &p, NULL);
   GL.glCompileShader(vtx_shader);
   GL.glGetShaderiv(vtx_shader, GL_COMPILE_STATUS, &ok);
   if (!ok)
     {
        ERR("vertex shader compilation failed ");
        GL.glDeleteShader(vtx_shader);
        return 0;
     }

   p = fragment_shader;
   fgmt_shader = GL.glCreateShader(GL_FRAGMENT_SHADER);
   GL.glShaderSource(fgmt_shader, 1, &p, NULL);
   GL.glCompileShader(fgmt_shader);
   GL.glGetShaderiv(fgmt_shader, GL_COMPILE_STATUS, &ok);
   if (!ok)
     {
        ERR("fragment shader compilation failed ");
        GL.glDeleteShader(fgmt_shader);
        return 0;
     }

   program = GL.glCreateProgram();
   GL.glAttachShader(program, vtx_shader);
   GL.glAttachShader(program, fgmt_shader);
   GL.glBindAttribLocation(program, SHAD_VERTEX, "vertex");
   GL.glLinkProgram(program);
   GL.glGetProgramiv(program, GL_LINK_STATUS, &ok);
   if (!ok)
     {
        ERR("linking failed for shader program");
        GL.glDeleteShader(vtx_shader);
        GL.glDeleteShader(fgmt_shader);
        return 0;
     }
   return program;
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

   GL.glUniform4f(pd->uniform_color, (R_VAL(&color)/255.0), (G_VAL(&color)/255.0), (B_VAL(&color)/255.0), (A_VAL(&color)/255.0));

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
   GL.glUniform4f(pd->uniform_color, (R_VAL(&color)/255.0), (G_VAL(&color)/255.0), (B_VAL(&color)/255.0), (A_VAL(&color)/255.0));
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

Eina_Bool
_update_scratch_fbo(Ector_Gl_Surface_Data *pd, int width, int height)
{
   GLint current_fbo;

   GL.glGetIntegerv(GL_FRAMEBUFFER_BINDING, &current_fbo);
   // update dest info
   pd->dest_fbo.fbo = current_fbo;
   pd->dest_fbo.w = width;
   pd->dest_fbo.h = height;

   if (!pd->scratch_fbo.msaa) return EINA_TRUE;

   if (pd->scratch_fbo.w >= width && pd->scratch_fbo.h >= height)
     {
        // no need to update the scratch buffer
        GL.glBindFramebuffer(GL_FRAMEBUFFER, pd->scratch_fbo.fbo);
        return EINA_TRUE;
     }

   // delete old buffer if any
   if (pd->scratch_fbo.fbo)
     {
        GL.glDeleteRenderbuffers(1, &(pd->scratch_fbo.rbo_color));
        GL.glDeleteRenderbuffers(1, &(pd->scratch_fbo.rbo_stencil));
        GL.glDeleteFramebuffers(1, &(pd->scratch_fbo.fbo));
        pd->scratch_fbo.fbo = 0;
     }

   pd->scratch_fbo.w = pd->dest_fbo.w;
   pd->scratch_fbo.h = pd->dest_fbo.h;

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

   // attach depthbuffer image to FBO
   GL.glFramebufferRenderbuffer(GL_FRAMEBUFFER,
                                GL_STENCIL_ATTACHMENT,
                                GL_RENDERBUFFER,
                                pd->scratch_fbo.rbo_stencil);

   // check FBO status
   GLenum status = GL.glCheckFramebufferStatus(GL_FRAMEBUFFER);
   if(status != GL_FRAMEBUFFER_COMPLETE)
     {
        ERR("cretaing MSAA fbo failed");
        GL.glDeleteRenderbuffers(1, &(pd->scratch_fbo.rbo_color));
        GL.glDeleteRenderbuffers(1, &(pd->scratch_fbo.rbo_stencil));
        GL.glDeleteFramebuffers(1, &(pd->scratch_fbo.fbo));
        pd->scratch_fbo.fbo = 0;
        return EINA_FALSE;
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
        GL.glUseProgram(pd->simple_program);
        GL.glUniformMatrix4fv(pd->uniform_mvp, 1, GL_FALSE, mvp);
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
    }
}

static void
_ector_gl_surface_offset(Eo *obj EINA_UNUSED,
                         Ector_Gl_Surface_Data *pd, int x, int y)
{
   pd->offset.x = x;
   pd->offset.y = y;
   GL.glUniform4f(pd->uniform_pos, pd->ref_point.x + pd->offset.x, pd->ref_point.y + pd->offset.y, 0, 0);
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

   pd->simple_program = _init_shader();
   pd->uniform_color = GL.glGetUniformLocation(pd->simple_program, "color");
   pd->uniform_pos = GL.glGetUniformLocation(pd->simple_program, "pos");
   pd->uniform_mvp = GL.glGetUniformLocation(pd->simple_program, "mvp");

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
   if (GL.glFuncExist(GL.glBlitFramebuffer))
     {
        pd->scratch_fbo.msaa = EINA_FALSE;
     }
   else
     {
        pd->scratch_fbo.msaa = EINA_FALSE;
     }
   return obj;
}

static void
_ector_gl_surface_eo_base_destructor(Eo *obj EINA_UNUSED,
                                     Ector_Gl_Surface_Data *pd EINA_UNUSED)
{
   if (pd->simple_program)
     {
        GL.glDeleteProgram(pd->simple_program);
     }
   if (pd->scratch_fbo.fbo)
     {
        GL.glDeleteRenderbuffers(1, &(pd->scratch_fbo.rbo_color));
        GL.glDeleteRenderbuffers(1, &(pd->scratch_fbo.rbo_stencil));
        GL.glDeleteFramebuffers(1, &(pd->scratch_fbo.fbo));
        pd->scratch_fbo.fbo = 0;
     }
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
