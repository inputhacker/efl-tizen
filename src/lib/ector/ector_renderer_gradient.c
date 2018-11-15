#ifdef HAVE_CONFIG_H
# include "config.h"
#endif

#include <Eina.h>
#include <Ector.h>

#include "ector_private.h"

#define MY_CLASS ECTOR_RENDERER_GENERIC_GRADIENT_MIXIN

static void
_ector_renderer_generic_gradient_efl_gfx_gradient_base_stop_set(Eo *obj EINA_UNUSED,
                                                                Ector_Renderer_Generic_Gradient_Data *pd,
                                                                const Efl_Gfx_Gradient_Stop *colors,
                                                                unsigned int length)
{
   pd->colors = realloc(pd->colors, length * sizeof(Efl_Gfx_Gradient_Stop));
   if (!pd->colors)
     {
        pd->colors_count = 0;
        return ;
     }

   memcpy(pd->colors, colors, length * sizeof(Efl_Gfx_Gradient_Stop));
   pd->colors_count = length;
}

static void
_ector_renderer_generic_gradient_efl_gfx_gradient_base_stop_get(Eo *obj EINA_UNUSED,
                                                                Ector_Renderer_Generic_Gradient_Data *pd,
                                                                const Efl_Gfx_Gradient_Stop **colors,
                                                                unsigned int *length)
{
   if (colors) *colors = pd->colors;
   if (length) *length = pd->colors_count;
}

static void
_ector_renderer_generic_gradient_efl_gfx_gradient_base_spread_set(Eo *obj EINA_UNUSED,
                                                                  Ector_Renderer_Generic_Gradient_Data *pd,
                                                                  Efl_Gfx_Gradient_Spread s)
{
   pd->s = s;
}

static Efl_Gfx_Gradient_Spread
_ector_renderer_generic_gradient_efl_gfx_gradient_base_spread_get(Eo *obj EINA_UNUSED,
                                                                  Ector_Renderer_Generic_Gradient_Data *pd)
{
   return pd->s;
}

static void
_ector_renderer_generic_gradient_eo_base_destructor(Eo *obj,
                                                    Ector_Renderer_Generic_Gradient_Data *pd)
{
   if (pd->colors) free(pd->colors);
   eo_do_super(obj, MY_CLASS, eo_destructor());
}
#include "ector_renderer_generic_gradient.eo.c"
