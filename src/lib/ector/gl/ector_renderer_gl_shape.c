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
   Efl_Gfx_Shape_Public                *public_shape;
   Ector_Renderer_Generic_Shape_Data   *shape;
   Ector_Renderer_Generic_Base_Data    *base;

   Triangulator_Simple                 *filler;
   Triangulator_Stroker                *stroker;
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

   return EINA_TRUE;
}

static Eina_Bool
_generate_stroke_data(Ector_Renderer_Gl_Shape_Data *pd)
{
   if (pd->stroker) return EINA_FALSE;

   if (!pd->shape->stroke.fill &&
       ((pd->public_shape->stroke.color.a == 0) ||
        (pd->public_shape->stroke.width < 0.01)))
     return EINA_FALSE;

   return EINA_TRUE;
}

static Eina_Bool
_generate_shape_data(Ector_Renderer_Gl_Shape_Data *pd)
{
   if (pd->filler) return EINA_FALSE;

   if (!pd->shape->fill && (pd->base->color.a == 0)) return EINA_FALSE;

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

static void
_update_triangulators(Eo *obj, Ector_Renderer_Gl_Shape_Data *pd)
{
   const Efl_Gfx_Path_Command *cmds = NULL;
   const double *pts;
   double *new_pts;
   unsigned int cmd_count = 0, pt_count = 0;
   Eina_Bool cleanup = EINA_FALSE;

   eo_do(obj, efl_gfx_shape_path_get(&cmds, &pts));
   if (cmds && (_generate_stroke_data(pd) || _generate_shape_data(pd)))
     {
        eo_do(obj, efl_gfx_shape_path_length_get(&cmd_count, &pt_count));

        if (pd->base->m &&
            (eina_matrix3_type_get(pd->base->m) != EINA_MATRIX_TYPE_IDENTITY))
          {
             new_pts =  malloc(sizeof(double) * pt_count);
             memcpy(new_pts, pts, sizeof(double) * pt_count);
             _shape_transform(new_pts, pt_count, pd->base->m);
             cleanup = EINA_TRUE;
          }
        else
          {
             new_pts = (double *)pts;
          }

        //shape triangulator creation
        if (_generate_shape_data(pd))
          {
             pd->filler = triangulator_simple_new();
             triangulator_simple_process(pd->filler, cmds, new_pts, 0);
          }

        //stroke triangulator creation
        if ( _generate_stroke_data(pd))
          {
             pd->stroker = triangulator_stroker_new();
             triangulator_stroker_stroke_set(pd->stroker,
                                             (pd->public_shape->stroke.width * pd->public_shape->stroke.scale),
                                             pd->public_shape->stroke.cap,
                                             pd->public_shape->stroke.join,
                                             pd->base->m);

             triangulator_stroker_process(pd->stroker, cmds, new_pts, cmd_count, pt_count);
          }
        if (cleanup) free(new_pts);
     }
}


static Eina_Bool
_ector_renderer_gl_shape_ector_renderer_generic_base_draw(Eo *obj EINA_UNUSED,
                                                          Ector_Renderer_Gl_Shape_Data *pd EINA_UNUSED,
                                                          Ector_Rop op EINA_UNUSED, Eina_Array *clips EINA_UNUSED,
                                                          unsigned int mul_col EINA_UNUSED)
{
   float *vertex;
   int *stops , count, stop_count;
   unsigned int color;

   _update_triangulators(obj, pd);

   ector_gl_engine_param_offset(pd->base->origin.x, pd->base->origin.y);

   if (pd->filler)
     {
        if (pd->shape->fill)
          eo_do(pd->shape->fill, ector_renderer_gl_base_fill());
        else
          {
             color = ECTOR_ARGB_JOIN(pd->base->color.a,
                                     pd->base->color.r,
                                     pd->base->color.g,
                                     pd->base->color.b);
             color = ECTOR_MUL4_SYM(color, mul_col);
             ector_gl_engine_param_color(color);
          }

        vertex = eina_inarray_nth(pd->filler->vertices, 0);
        count = eina_inarray_count(pd->filler->vertices);
        stop_count = eina_inarray_count(pd->filler->stops);
        stops = eina_inarray_nth(pd->filler->stops, 0);

        ector_gl_engine_path_fill(vertex, count, stops, stop_count);
     }

  if (pd->stroker)
    {
        if (pd->shape->stroke.fill)
          eo_do(pd->shape->stroke.fill, ector_renderer_gl_base_fill());
        else
          {
             color = ECTOR_ARGB_JOIN(pd->public_shape->stroke.color.a,
                                     pd->public_shape->stroke.color.r,
                                     pd->public_shape->stroke.color.g,
                                     pd->public_shape->stroke.color.b);
             color = ECTOR_MUL4_SYM(color, mul_col);
             ector_gl_engine_param_color(color);
          }

        vertex = eina_inarray_nth(pd->stroker->vertices, 0);
        count = eina_inarray_count(pd->stroker->vertices);

        ector_gl_engine_path_stroke(vertex, count);
    }

   return EINA_TRUE;
}

static void
_clear_path_compute_data(Ector_Renderer_Gl_Shape_Data *pd)
{
   if (pd->filler) triangulator_simple_free(pd->filler);
   if (pd->stroker) triangulator_stroker_free(pd->stroker);

   pd->filler = NULL;
   pd->stroker = NULL;
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

   _clear_path_compute_data(pd);
}


static Eina_Bool
_ector_renderer_gl_shape_path_changed(void *data EINA_UNUSED, Eo *obj EINA_UNUSED,
                                      const Eo_Event_Description *desc EINA_UNUSED,
                                      void *event_info EINA_UNUSED)
{
   Ector_Renderer_Gl_Shape_Data *pd = data;

   _clear_path_compute_data(pd);
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

   _clear_path_compute_data(pd);

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
