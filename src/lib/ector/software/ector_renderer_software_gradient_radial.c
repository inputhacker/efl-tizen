#ifdef HAVE_CONFIG_H
# include "config.h"
#endif

#include <Eina.h>
#include <Ector.h>
#include <software/Ector_Software.h>

#include "ector_private.h"
#include "ector_software_private.h"

static Eina_Bool
_ector_renderer_software_gradient_radial_ector_renderer_generic_base_prepare(Eo *obj, Ector_Renderer_Software_Gradient_Data *pd)
{
   if (!pd->surface)
     {
        Eo *parent;

        eo_do(obj, parent = eo_parent_get());
        if (!parent) return EINA_FALSE;
        pd->surface = eo_data_xref(parent, ECTOR_SOFTWARE_SURFACE_CLASS, obj);
     }

   update_color_table(pd);

   pd->radial.cx = pd->grd->radial.x;
   pd->radial.cy = pd->grd->radial.y;
   pd->radial.cradius = pd->grd->radius;

   if (!pd->grd->focal.x)
     pd->radial.fx = pd->grd->radial.x;
   else
     pd->radial.fx = pd->grd->focal.x;

   if (!pd->grd->focal.y)
     pd->radial.fy = pd->grd->radial.y;
   else
     pd->radial.fy = pd->grd->focal.y;

   pd->radial.fradius = 0;

   pd->radial.dx = pd->radial.cx - pd->radial.fx;
   pd->radial.dy = pd->radial.cy - pd->radial.fy;

   pd->radial.dr = pd->radial.cradius - pd->radial.fradius;
   pd->radial.sqrfr = pd->radial.fradius * pd->radial.fradius;

   pd->radial.a = pd->radial.dr * pd->radial.dr -
     pd->radial.dx * pd->radial.dx -
     pd->radial.dy * pd->radial.dy;
   pd->radial.inv2a = 1 / (2 * pd->radial.a);

   pd->radial.extended = (pd->radial.fradius >= 0.00001f) || pd->radial.a >= 0.00001f;

   return EINA_FALSE;
}

// Clearly duplicated and should be in a common place...
static Eina_Bool
_ector_renderer_software_gradient_radial_ector_renderer_generic_base_draw(Eo *obj EINA_UNUSED,
                                                                          Ector_Renderer_Software_Gradient_Data *pd EINA_UNUSED,
                                                                          Ector_Rop op EINA_UNUSED, Eina_Array *clips EINA_UNUSED,
                                                                          unsigned int mul_col EINA_UNUSED)
{
   return EINA_TRUE;
}

// Clearly duplicated and should be in a common place...
static Eina_Bool
_ector_renderer_software_gradient_radial_ector_renderer_software_base_fill(Eo *obj EINA_UNUSED, Ector_Renderer_Software_Gradient_Data *pd)
{
   ector_software_rasterizer_radial_gradient_set(pd->surface->software, pd);
   return EINA_TRUE;
}

Eo *
_ector_renderer_software_gradient_radial_eo_base_constructor(Eo *obj,
                                                             Ector_Renderer_Software_Gradient_Data *pd)
{
   obj = eo_do_super_ret(obj, ECTOR_RENDERER_SOFTWARE_GRADIENT_RADIAL_CLASS, obj, eo_constructor());
   pd->gd  = eo_data_xref(obj, ECTOR_RENDERER_GENERIC_GRADIENT_MIXIN, obj);
   pd->gld = eo_data_xref(obj, ECTOR_RENDERER_GENERIC_GRADIENT_RADIAL_MIXIN, obj);

   return obj;
}

void
_ector_renderer_software_gradient_radial_eo_base_destructor(Eo *obj,
                                                            Ector_Renderer_Software_Gradient_Data *pd)
{
   Eo *parent;

   destroy_color_table(pd);

   eo_do(obj, parent = eo_parent_get());
   eo_data_xunref(parent, pd->surface, obj);

   eo_data_xunref(obj, pd->gd, obj);
   eo_data_xunref(obj, pd->gld, obj);

   eo_do_super(obj, ECTOR_RENDERER_SOFTWARE_GRADIENT_RADIAL_CLASS, eo_destructor());
}

void
_ector_renderer_software_gradient_radial_efl_gfx_gradient_base_stop_set(Eo *obj, Ector_Renderer_Software_Gradient_Data *pd, const Efl_Gfx_Gradient_Stop *colors, unsigned int length)
{
   eo_do_super(obj, ECTOR_RENDERER_SOFTWARE_GRADIENT_RADIAL_CLASS,
               efl_gfx_gradient_stop_set(colors, length));

   destroy_color_table(pd);
}

static unsigned int
_ector_renderer_software_gradient_radial_ector_renderer_generic_base_crc_get(Eo *obj, Ector_Renderer_Software_Gradient_Data *pd)
{
   unsigned int crc;

   eo_do_super(obj, ECTOR_RENDERER_SOFTWARE_GRADIENT_RADIAL_CLASS,
               crc = ector_renderer_crc_get());

   crc = eina_crc((void*) pd->gd->s, sizeof (Efl_Gfx_Gradient_Spread), crc, EINA_FALSE);
   if (pd->gd->colors_count)
     crc = eina_crc((void*) pd->gd->colors, sizeof (Efl_Gfx_Gradient_Stop) * pd->gd->colors_count, crc, EINA_FALSE);
   crc = eina_crc((void*) pd->gld, sizeof (Ector_Renderer_Generic_Gradient_Radial_Data), crc, EINA_FALSE);

   return crc;
}

#include "ector_renderer_software_gradient_radial.eo.c"
