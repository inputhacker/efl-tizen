#ifndef ECTOR_GL_PRIVATE_H_
#define ECTOR_GL_PRIVATE_H_

#define SHAD_VERTEX 0
#define GL_STENCIL_HIGH_BIT  0x80

typedef struct _Ector_Renderer_Gl_Gradient_Data    Ector_Renderer_Gl_Gradient_Data;
typedef struct _Ector_Gl_Surface_Data              Ector_Gl_Surface_Data;

struct _Ector_Gl_Surface_Data
{
   struct {
      int x, y;
   } ref_point;
   struct {
      int x, y;
   } offset;
   int width;
   int height;
   GLuint simple_program;
   GLuint uniform_color;
   GLuint uniform_mvp;
   GLuint uniform_pos;

   struct {
      int w, h;
      GLuint fbo;
   } dest_fbo;

   struct {
      int w, h;
      GLuint fbo;
      GLuint rbo_color;
      GLuint rbo_stencil;
      Eina_Bool msaa;
      int msaa_level;
   } scratch_fbo;
};

// Gradient related structure
struct _Ector_Renderer_Gl_Gradient_Data
{
   Ector_Renderer_Generic_Gradient_Data *gd;
   union {
      Ector_Renderer_Generic_Gradient_Linear_Data *gld;
      Ector_Renderer_Generic_Gradient_Radial_Data *grd;
   };
   Eina_Bool alpha;
   uint* color_table;
};

#endif
