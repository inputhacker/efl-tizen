#ifdef HAVE_CONFIG_H
# include "config.h"
#endif

#include <math.h>
#include <float.h>

#include <Eina.h>
#include <Ector.h>
#include <gl/Ector_Gl.h>

#include "ector_private.h"
#include "ector_gl_private.h"
#include "triangulator_stroker.h"
#include "triangulator_simple.h"

typedef struct _Ector_Renderer_Gl_Shape_Data Ector_Renderer_Gl_Shape_Data;
struct _Ector_Renderer_Gl_Shape_Data
{
   Eo                                  *surface;
   Efl_Gfx_Shape_Public                *public_shape;
   Ector_Renderer_Generic_Shape_Data   *shape;
   Ector_Renderer_Generic_Base_Data    *base;
};

static Eina_Bool
_ector_renderer_gl_shape_ector_renderer_generic_base_prepare(Eo *obj EINA_UNUSED,
                                                             Ector_Renderer_Gl_Shape_Data *pd)
{
   // FIXME: shouldn't that be part of the shape generic implementation ?
   if (pd->shape->fill)
     eo_do(pd->shape->fill, ector_renderer_prepare());
   if (pd->shape->stroke.fill)
     eo_do(pd->shape->stroke.fill, ector_renderer_prepare());

   if (!pd->surface)
     {
        eo_do(obj, pd->surface = eo_parent_get());
        if (!pd->surface)
          return EINA_FALSE;
     }

   return EINA_TRUE;
}

static void _shape_transform(double *pts, int count , Eina_Matrix3 *m)
{
   int i;
   double x, y;
   if (m && (eina_matrix3_type_get(m) != EINA_MATRIX_TYPE_IDENTITY))
     {
        for (i = 0; i < count; i= i+2)
          {
             eina_matrix3_point_transform(m,
                                          pts[i],
                                          pts[i+1],
                                          &x, &y);

             pts[i] = x;
             pts[i+1] = y;
          }
     }
}


static Eina_Bool
_ector_renderer_gl_shape_ector_renderer_generic_base_draw(Eo *obj EINA_UNUSED,
                                                          Ector_Renderer_Gl_Shape_Data *pd EINA_UNUSED,
                                                          Ector_Rop op EINA_UNUSED, Eina_Array *clips EINA_UNUSED,
                                                          unsigned int mul_col EINA_UNUSED)
{
   const Efl_Gfx_Path_Command *cmds = NULL;
   const double *pts = NULL;
   double *new_pts = NULL;
   unsigned int cmd_count = 0, pt_count = 0;
   int count;
   float *vertex;

   eo_do(obj, efl_gfx_shape_path_get(&cmds, &pts));
   eo_do(obj, efl_gfx_shape_path_length_get(&cmd_count, &pt_count));
   new_pts =  malloc(sizeof(double) * pt_count);
   memcpy(new_pts, pts, sizeof(double) * pt_count);
   _shape_transform(new_pts, pt_count, pd->base->m);
   if (!cmd_count) return EINA_FALSE;

   eo_do(pd->surface, ector_gl_surface_offset(pd->base->origin.x, pd->base->origin.y));
   if (pd->base->color.a > 0)
     {
        unsigned int fill_color = ECTOR_ARGB_JOIN(pd->base->color.a, pd->base->color.r, pd->base->color.g, pd->base->color.b);
        unsigned int res_color = ECTOR_MUL4_SYM(fill_color, mul_col);
        Triangulator_Simple *s = triangulator_simple_new();
        triangulator_simple_process(s, cmds, new_pts, 0);
        vertex = eina_inarray_nth(s->vertices, 0);
        count = eina_inarray_count(s->vertices);
        int stop_count = eina_inarray_count(s->stops);
        int *stops = eina_inarray_nth(s->stops, 0);
        eo_do(pd->surface, ector_gl_surface_path_fill(vertex, count, stops, stop_count, res_color));
        triangulator_simple_free(s);
     }
   if (pd->public_shape->stroke.color.a > 0)
     {
        unsigned int stroke_color = ECTOR_ARGB_JOIN(pd->public_shape->stroke.color.a, pd->public_shape->stroke.color.r,
                                                    pd->public_shape->stroke.color.g, pd->public_shape->stroke.color.b);
        unsigned int res_color = ECTOR_MUL4_SYM(stroke_color, mul_col);
        Triangulator_Stroker *s = triangulator_stroker_new();
        triangulator_stroker_stroke_set(s, (pd->public_shape->stroke.width * pd->public_shape->stroke.scale),
                                        pd->public_shape->stroke.cap, pd->public_shape->stroke.join, pd->base->m);

         triangulator_stroker_process(s, cmds, new_pts, cmd_count, pt_count);
         vertex = eina_inarray_nth(s->vertices, 0);
         count = eina_inarray_count(s->vertices);
         eo_do(pd->surface, ector_gl_surface_path_stroke(vertex, count, res_color));
         triangulator_stroker_free(s);
     }
   free(new_pts);
   return EINA_TRUE;
}

static Eina_Bool
_ector_renderer_gl_shape_ector_renderer_gl_base_fill(Eo *obj EINA_UNUSED,
                                                     Ector_Renderer_Gl_Shape_Data *pd EINA_UNUSED)
{
   // FIXME: let's find out how to fill a shape with a shape later.
   // I need to read SVG specification and see how to map that with software.
   return EINA_FALSE;
}

static void
_ector_renderer_gl_shape_efl_gfx_shape_path_set(Eo *obj,
                                                Ector_Renderer_Gl_Shape_Data *pd EINA_UNUSED,
                                                const Efl_Gfx_Path_Command *op,
                                                const double *points)
{

   eo_do_super(obj, ECTOR_RENDERER_GL_SHAPE_CLASS, efl_gfx_shape_path_set(op, points));
}


static Eina_Bool
_ector_renderer_gl_shape_path_changed(void *data EINA_UNUSED, Eo *obj EINA_UNUSED,
                                      const Eo_Event_Description *desc EINA_UNUSED,
                                      void *event_info EINA_UNUSED)
{
   return EINA_TRUE;
}

Eo *
_ector_renderer_gl_shape_eo_base_constructor(Eo *obj, Ector_Renderer_Gl_Shape_Data *pd)
{
   obj = eo_do_super_ret(obj, ECTOR_RENDERER_GL_SHAPE_CLASS, obj, eo_constructor());
   if (!obj) return NULL;

   pd->public_shape = eo_data_xref(obj, EFL_GFX_SHAPE_MIXIN, obj);
   pd->shape = eo_data_xref(obj, ECTOR_RENDERER_GENERIC_SHAPE_MIXIN, obj);
   pd->base = eo_data_xref(obj, ECTOR_RENDERER_GENERIC_BASE_CLASS, obj);
   eo_do(obj,
         eo_event_callback_add(EFL_GFX_PATH_CHANGED, _ector_renderer_gl_shape_path_changed, pd));

   return obj;
}

void
_ector_renderer_gl_shape_eo_base_destructor(Eo *obj, Ector_Renderer_Gl_Shape_Data *pd)
{
   //FIXME, As base class  destructor can't call destructor of mixin class.
   // call explicit API to free shape data.
   eo_do(obj, efl_gfx_shape_reset());

   eo_data_xunref(obj, pd->public_shape, obj);
   eo_data_xunref(obj, pd->shape, obj);
   eo_data_xunref(obj, pd->base, obj);
   eo_do_super(obj, ECTOR_RENDERER_GL_SHAPE_CLASS, eo_destructor());
}


unsigned int
_ector_renderer_gl_shape_ector_renderer_generic_base_crc_get(Eo *obj EINA_UNUSED,
                                                             Ector_Renderer_Gl_Shape_Data *pd EINA_UNUSED)
{
   return 0;
}

#include "ector_renderer_gl_shape.eo.c"
