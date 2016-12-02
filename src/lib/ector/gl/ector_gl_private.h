#ifndef ECTOR_GL_PRIVATE_H_
#define ECTOR_GL_PRIVATE_H_

#define SHAD_VERTEX 0
#define SHAD_TEXUV  1

#define GL_STENCIL_HIGH_BIT  0x80

typedef struct _Ector_Renderer_Gl_Gradient_Data    Ector_Renderer_Gl_Gradient_Data;
typedef struct _Ector_Gl_Surface_Data              Ector_Gl_Surface_Data;

typedef struct _Shader_Program                     Shader_Program;

struct _Shader_Program
{
  GLuint program;
  GLuint v_shader;
  GLuint f_shader;
  GLuint u_color;
  GLuint u_mvp;
  GLuint u_pos;
  GLuint u_texture;
};


struct _Ector_Gl_Surface_Data
{
   struct {
      int x, y;
   } ref_point;
   struct {
      int x, y;
   } offset;

   Shader_Program *simple_shader;
   Shader_Program *texture_shader;

   struct {
      int w, h;
      GLuint fbo;
   } dest_fbo;

   struct {
      int       w, h;
      GLuint    fbo;
      GLuint    texture;
      GLuint    rbo_color;
      GLuint    rbo_stencil;
      Eina_Bool msaa;
      Eina_Bool ext_msaa;
      int       msaa_level;
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
   uint32_t* color_table;
};

#endif
