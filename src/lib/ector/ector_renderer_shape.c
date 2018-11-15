#ifdef HAVE_CONFIG_H
# include "config.h"
#endif

#include <Eina.h>
#include <Ector.h>

#include "ector_private.h"

#define MY_CLASS ECTOR_RENDERER_GENERIC_SHAPE_MIXIN

static void
_ector_renderer_generic_shape_fill_set(Eo *obj EINA_UNUSED,
                                       Ector_Renderer_Generic_Shape_Data *pd,
                                       const Ector_Renderer *r)
{
   _ector_renderer_replace(&pd->fill, r);
}

static const Ector_Renderer *
_ector_renderer_generic_shape_fill_get(Eo *obj EINA_UNUSED,
                                       Ector_Renderer_Generic_Shape_Data *pd)
{
   return pd->fill;
}

static void
_ector_renderer_generic_shape_stroke_fill_set(Eo *obj EINA_UNUSED,
                                              Ector_Renderer_Generic_Shape_Data *pd,
                                              const Ector_Renderer *r)
{
   _ector_renderer_replace(&pd->stroke.fill, r);
}

static const Ector_Renderer *
_ector_renderer_generic_shape_stroke_fill_get(Eo *obj EINA_UNUSED,
                                              Ector_Renderer_Generic_Shape_Data *pd)
{
   return pd->stroke.fill;
}

static void
_ector_renderer_generic_shape_stroke_marker_set(Eo *obj EINA_UNUSED,
                                                Ector_Renderer_Generic_Shape_Data *pd,
                                                const Ector_Renderer *r)
{
   _ector_renderer_replace(&pd->stroke.marker, r);
}

static const Ector_Renderer *
_ector_renderer_generic_shape_stroke_marker_get(Eo *obj EINA_UNUSED,
                                                Ector_Renderer_Generic_Shape_Data *pd)
{
   return pd->stroke.marker;
}

static void
_ector_renderer_generic_shape_eo_base_destructor(Eo *obj,
                                                 Ector_Renderer_Generic_Shape_Data *pd)
{
   if (pd->fill) eo_unref(pd->fill);
   if (pd->stroke.fill) eo_unref(pd->stroke.fill);
   if (pd->stroke.marker) eo_unref(pd->stroke.marker);

   eo_do_super(obj, MY_CLASS, eo_destructor());
}


#include "ector_renderer_generic_shape.eo.c"
