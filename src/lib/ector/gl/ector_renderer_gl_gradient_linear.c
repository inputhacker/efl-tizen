#ifdef HAVE_CONFIG_H
# include "config.h"
#endif

#include <Eina.h>
#include <Ector.h>
#include <gl/Ector_Gl.h>

#include "ector_private.h"
#include "ector_gl_private.h"

static Eina_Bool
_ector_renderer_gl_gradient_linear_ector_renderer_generic_base_prepare(Eo *obj EINA_UNUSED,
                                                                       Ector_Renderer_Gl_Gradient_Data *pd EINA_UNUSED)
{
   return EINA_TRUE;
}

static Eina_Bool
_ector_renderer_gl_gradient_linear_ector_renderer_generic_base_draw(Eo *obj EINA_UNUSED,
                                                                    Ector_Renderer_Gl_Gradient_Data *pd EINA_UNUSED,
                                                                    Ector_Rop op EINA_UNUSED, Eina_Array *clips EINA_UNUSED,
                                                                    unsigned int mul_col EINA_UNUSED)
{
   return EINA_TRUE;
}

Eo *
_ector_renderer_gl_gradient_linear_eo_base_constructor(Eo *obj,
                                                       Ector_Renderer_Gl_Gradient_Data *pd EINA_UNUSED)
{
   obj = eo_do_super_ret(obj, ECTOR_RENDERER_GL_GRADIENT_LINEAR_CLASS, obj, eo_constructor());
   return obj;
}

void
_ector_renderer_gl_gradient_linear_eo_base_destructor(Eo *obj,
                                                      Ector_Renderer_Gl_Gradient_Data *pd EINA_UNUSED)
{
   eo_do_super(obj, ECTOR_RENDERER_GL_GRADIENT_LINEAR_CLASS, eo_destructor());
}

static Eina_Bool
_ector_renderer_gl_gradient_linear_ector_renderer_gl_base_fill(Eo *obj EINA_UNUSED,
                                                               Ector_Renderer_Gl_Gradient_Data *pd EINA_UNUSED)
{
   return EINA_FALSE;
}

void
_ector_renderer_gl_gradient_linear_efl_gfx_gradient_base_stop_set(Eo *obj EINA_UNUSED,
                                                                  Ector_Renderer_Gl_Gradient_Data *pd EINA_UNUSED,
                                                                  const Efl_Gfx_Gradient_Stop *colors EINA_UNUSED,
                                                                  unsigned int length EINA_UNUSED)
{

}

static unsigned int
_ector_renderer_gl_gradient_linear_ector_renderer_generic_base_crc_get(Eo *obj EINA_UNUSED,
                                                                       Ector_Renderer_Gl_Gradient_Data *pd EINA_UNUSED)
{
   return 0;
}

#include "ector_renderer_gl_gradient_linear.eo.c"
