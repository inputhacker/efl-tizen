#include "evas_common_private.h"
#include "evas_private.h"

#include "evas_vg_private.h"

#define MY_CLASS EFL_CANVAS_VG_SHAPE_CLASS

typedef struct _Efl_Canvas_Vg_Shape_Data Efl_Canvas_Vg_Shape_Data;
struct _Efl_Canvas_Vg_Shape_Data
{
   Efl_Canvas_Vg_Node *fill;

   struct {
      Efl_Canvas_Vg_Node *fill;
      Efl_Canvas_Vg_Node *marker;
   } stroke;
};

// FIXME: Use the renderer bounding box when it has been created instead of an estimation

static void
_efl_canvas_vg_shape_fill_set(Eo *obj EINA_UNUSED,
                       Efl_Canvas_Vg_Shape_Data *pd,
                       Efl_Canvas_Vg_Node *f)
{
   Efl_Canvas_Vg_Node *tmp = pd->fill;

   pd->fill = efl_ref(f);
   efl_unref(tmp);

   _efl_canvas_vg_node_changed(obj);
}

static Efl_Canvas_Vg_Node *
_efl_canvas_vg_shape_fill_get(const Eo *obj EINA_UNUSED, Efl_Canvas_Vg_Shape_Data *pd)
{
   return pd->fill;
}

static void
_efl_canvas_vg_shape_stroke_fill_set(Eo *obj EINA_UNUSED,
                              Efl_Canvas_Vg_Shape_Data *pd,
                              Efl_Canvas_Vg_Node *f)
{
   Efl_Canvas_Vg_Node *tmp = pd->fill;

   pd->stroke.fill = efl_ref(f);
   efl_unref(tmp);

   _efl_canvas_vg_node_changed(obj);
}

static Efl_Canvas_Vg_Node *
_efl_canvas_vg_shape_stroke_fill_get(const Eo *obj EINA_UNUSED,
                              Efl_Canvas_Vg_Shape_Data *pd)
{
   return pd->stroke.fill;
}

static void
_efl_canvas_vg_shape_stroke_marker_set(Eo *obj EINA_UNUSED,
                                Efl_Canvas_Vg_Shape_Data *pd,
                                Efl_Canvas_Vg_Shape *m)
{
   Efl_Canvas_Vg_Node *tmp = pd->stroke.marker;

   pd->stroke.marker = efl_ref(m);
   efl_unref(tmp);

   _efl_canvas_vg_node_changed(obj);
}

static Efl_Canvas_Vg_Shape *
_efl_canvas_vg_shape_stroke_marker_get(const Eo *obj EINA_UNUSED,
                                Efl_Canvas_Vg_Shape_Data *pd)
{
   return pd->stroke.marker;
}

static void
_efl_canvas_vg_shape_render_pre(Eo *obj EINA_UNUSED,
                         Eina_Matrix3 *parent,
                         Ector_Surface *s,
                         void *data,
                         Efl_Canvas_Vg_Node_Data *nd)
{
   Efl_Canvas_Vg_Shape_Data *pd = data;
   Efl_Canvas_Vg_Node_Data *fill, *stroke_fill, *stroke_marker, *mask;

   if (nd->flags == EFL_GFX_CHANGE_FLAG_NONE) return ;

   nd->flags = EFL_GFX_CHANGE_FLAG_NONE;

   EFL_CANVAS_VG_COMPUTE_MATRIX(current, parent, nd);

   fill = _evas_vg_render_pre(pd->fill, s, current);
   stroke_fill = _evas_vg_render_pre(pd->stroke.fill, s, current);
   stroke_marker = _evas_vg_render_pre(pd->stroke.marker, s, current);
   mask = _evas_vg_render_pre(nd->mask, s, current);

   if (!nd->renderer)
     {
        efl_domain_current_push(EFL_ID_DOMAIN_SHARED);
        nd->renderer = ector_surface_renderer_factory_new(s, ECTOR_RENDERER_SHAPE_MIXIN);
        efl_domain_current_pop();
     }

   ector_renderer_transformation_set(nd->renderer, current);
   ector_renderer_origin_set(nd->renderer, nd->x, nd->y);
   ector_renderer_color_set(nd->renderer, nd->r, nd->g, nd->b, nd->a);
   ector_renderer_visibility_set(nd->renderer, nd->visibility);
   ector_renderer_mask_set(nd->renderer, mask ? mask->renderer : NULL);
   ector_renderer_shape_fill_set(nd->renderer, fill ? fill->renderer : NULL);
   ector_renderer_shape_stroke_fill_set(nd->renderer, stroke_fill ? stroke_fill->renderer : NULL);
   ector_renderer_shape_stroke_marker_set(nd->renderer, stroke_marker ? stroke_marker->renderer : NULL);
   efl_gfx_path_copy_from(nd->renderer, obj);
   ector_renderer_prepare(nd->renderer);
}

static Eo *
_efl_canvas_vg_shape_efl_object_constructor(Eo *obj, Efl_Canvas_Vg_Shape_Data *pd)
{
   Efl_Canvas_Vg_Node_Data *nd;

   obj = efl_constructor(efl_super(obj, MY_CLASS));

   efl_gfx_shape_stroke_scale_set(obj, 1);
   efl_gfx_shape_stroke_location_set(obj, 0.5);
   efl_gfx_shape_stroke_cap_set(obj, EFL_GFX_CAP_BUTT);
   efl_gfx_shape_stroke_join_set(obj, EFL_GFX_JOIN_MITER);

   nd = efl_data_scope_get(obj, EFL_CANVAS_VG_NODE_CLASS);
   nd->render_pre = _efl_canvas_vg_shape_render_pre;
   nd->data = pd;

   return obj;
}

static void
_efl_canvas_vg_shape_efl_object_destructor(Eo *obj, Efl_Canvas_Vg_Shape_Data *pd EINA_UNUSED)
{
   efl_gfx_path_reset(obj);
   efl_destructor(efl_super(obj, MY_CLASS));
}

static Eina_Bool
_efl_canvas_vg_shape_efl_gfx_path_interpolate(Eo *obj,
                                      Efl_Canvas_Vg_Shape_Data *pd,
                                      const Efl_Canvas_Vg_Node *from, const Efl_Canvas_Vg_Node *to,
                                      double pos_map)
{
   Efl_Canvas_Vg_Shape_Data *fromd, *tod;
   Eina_Bool r;

   fromd = efl_data_scope_get(from, EFL_CANVAS_VG_SHAPE_CLASS);
   tod = efl_data_scope_get(to, EFL_CANVAS_VG_SHAPE_CLASS);

   r = efl_gfx_path_interpolate(efl_super(obj, MY_CLASS), from, to, pos_map);

   if (fromd->fill && tod->fill && pd->fill)
     {
        r &= efl_gfx_path_interpolate(pd->fill, fromd->fill, tod->fill, pos_map);
     }
   if (fromd->stroke.fill && tod->stroke.fill && pd->stroke.fill)
     {
        r &= efl_gfx_path_interpolate(pd->stroke.fill, fromd->stroke.fill, tod->stroke.fill, pos_map);
     }
   if (fromd->stroke.marker && tod->stroke.marker && pd->stroke.marker)
     {
        r &= efl_gfx_path_interpolate(pd->stroke.marker, fromd->stroke.marker, tod->stroke.marker, pos_map);
     }

   return r;
}


EOLIAN static Efl_Canvas_Vg_Node *
_efl_canvas_vg_shape_efl_duplicate_duplicate(const Eo *obj, Efl_Canvas_Vg_Shape_Data *pd)
{
   Efl_Canvas_Vg_Node *cn = NULL;
   Efl_Canvas_Vg_Shape_Data *cd = NULL;

   cn = efl_duplicate(efl_super(obj, MY_CLASS));
   cd = efl_data_scope_get(cn, MY_CLASS);
   if (pd->fill)
     cd->fill = efl_duplicate(pd->fill);
   if (pd->stroke.fill)
     cd->stroke.fill = efl_duplicate(pd->stroke.fill);
   if (pd->stroke.marker)
     cd->stroke.marker = efl_duplicate(pd->stroke.marker);

   efl_gfx_path_copy_from(cn, obj);
   return cn;
}

EAPI double
evas_vg_shape_stroke_scale_get(Eo *obj)
{
   return efl_gfx_shape_stroke_scale_get(obj);
}

EAPI void
evas_vg_shape_stroke_scale_set(Eo *obj, double s)
{
   efl_gfx_shape_stroke_scale_set(obj, s);
}

EAPI void
evas_vg_shape_stroke_color_get(Eo *obj, int *r, int *g, int *b, int *a)
{
   efl_gfx_shape_stroke_color_get(obj, r, g, b, a);
}

EAPI void
evas_vg_shape_stroke_color_set(Eo *obj, int r, int g, int b, int a)
{
   efl_gfx_shape_stroke_color_set(obj, r, g, b, a);
}

EAPI double
evas_vg_shape_stroke_width_get(Eo *obj)
{
   return efl_gfx_shape_stroke_width_get(obj);
}

EAPI void
evas_vg_shape_stroke_width_set(Eo *obj, double w)
{
   efl_gfx_shape_stroke_width_set(obj, w);
}

EAPI double
evas_vg_shape_stroke_location_get(Eo *obj)
{
   return efl_gfx_shape_stroke_location_get(obj);
}

EAPI void
evas_vg_shape_stroke_location_set(Eo *obj, double centered)
{
   efl_gfx_shape_stroke_location_set(obj, centered);
}

EAPI void
evas_vg_shape_stroke_dash_get(Eo *obj, const Efl_Gfx_Dash **dash, unsigned int *length)
{
   efl_gfx_shape_stroke_dash_get(obj, dash, length);
}

EAPI void
evas_vg_shape_stroke_dash_set(Eo *obj, const Efl_Gfx_Dash *dash, unsigned int length)
{
   efl_gfx_shape_stroke_dash_set(obj, dash, length);
}

EAPI Efl_Gfx_Cap
evas_vg_shape_stroke_cap_get(Eo *obj)
{
   return efl_gfx_shape_stroke_cap_get(obj);
}

EAPI void
evas_vg_shape_stroke_cap_set(Eo *obj, Efl_Gfx_Cap c)
{
   efl_gfx_shape_stroke_cap_set(obj, c);
}

EAPI Efl_Gfx_Join
evas_vg_shape_stroke_join_get(Eo *obj)
{
   return efl_gfx_shape_stroke_join_get(obj);
}

EAPI void
evas_vg_shape_stroke_join_set(Eo *obj, Efl_Gfx_Join j)
{
   efl_gfx_shape_stroke_join_set(obj, j);
}

EAPI void
evas_vg_shape_path_set(Eo *obj, const Efl_Gfx_Path_Command *op, const double *points)
{
   efl_gfx_path_set(obj, op, points);
}

EAPI void
evas_vg_shape_path_get(Eo *obj, const Efl_Gfx_Path_Command **op, const double **points)
{
   efl_gfx_path_get(obj, op, points);
}

EAPI void
evas_vg_shape_path_length_get(Eo *obj, unsigned int *commands, unsigned int *points)
{
   efl_gfx_path_length_get(obj, commands, points);
}

EAPI void
evas_vg_shape_current_get(Eo *obj, double *x, double *y)
{
   efl_gfx_path_current_get(obj, x, y);
}

EAPI void
evas_vg_shape_current_ctrl_get(Eo *obj, double *x, double *y)
{
   efl_gfx_path_current_ctrl_get(obj, x, y);
}

EAPI void
evas_vg_shape_dup(Eo *obj, Eo *dup_from)
{
   efl_gfx_path_copy_from(obj, dup_from);
}

EAPI void
evas_vg_shape_reset(Eo *obj)
{
   efl_gfx_path_reset(obj);
}

EAPI void
evas_vg_shape_append_move_to(Eo *obj, double x, double y)
{
   efl_gfx_path_append_move_to(obj, x, y);
}

EAPI void
evas_vg_shape_append_line_to(Eo *obj, double x, double y)
{
   efl_gfx_path_append_line_to(obj, x, y);
}

EAPI void
evas_vg_shape_append_quadratic_to(Eo *obj, double x, double y, double ctrl_x, double ctrl_y)
{
   efl_gfx_path_append_quadratic_to(obj, x, y, ctrl_x, ctrl_y);
}

EAPI void
evas_vg_shape_append_squadratic_to(Eo *obj, double x, double y)
{
   efl_gfx_path_append_squadratic_to(obj, x, y);
}

EAPI void
evas_vg_shape_append_cubic_to(Eo *obj, double x, double y, double ctrl_x0, double ctrl_y0, double ctrl_x1, double ctrl_y1)
{
   efl_gfx_path_append_cubic_to(obj, x, y, ctrl_x0, ctrl_y0, ctrl_x1, ctrl_y1);
}

EAPI void
evas_vg_shape_append_scubic_to(Eo *obj, double x, double y, double ctrl_x, double ctrl_y)
{
   efl_gfx_path_append_scubic_to(obj, x, y, ctrl_x, ctrl_y);
}

EAPI void
evas_vg_shape_append_arc_to(Eo *obj, double x, double y, double rx, double ry, double angle, Eina_Bool large_arc, Eina_Bool sweep)
{
   efl_gfx_path_append_arc_to(obj, x, y, rx, ry, angle, large_arc, sweep);
}

EAPI void
evas_vg_shape_append_arc(Eo *obj, double x, double y, double w, double h, double start_angle, double sweep_length)
{
   efl_gfx_path_append_arc(obj, x, y, w, h, start_angle, sweep_length);
}

EAPI void
evas_vg_shape_append_close(Eo *obj)
{
   efl_gfx_path_append_close(obj);
}

EAPI void
evas_vg_shape_append_circle(Eo *obj, double x, double y, double radius)
{
   efl_gfx_path_append_circle(obj, x, y, radius);
}

EAPI void
evas_vg_shape_append_rect(Eo *obj, double x, double y, double w, double h, double rx, double ry)
{
   efl_gfx_path_append_rect(obj, x, y, w, h, rx, ry);
}

EAPI void
evas_vg_shape_append_svg_path(Eo *obj, const char *svg_path_data)
{
   efl_gfx_path_append_svg_path(obj, svg_path_data);
}

EAPI Eina_Bool
evas_vg_shape_interpolate(Eo *obj, const Eo *from, const Eo *to, double pos_map)
{
   return efl_gfx_path_interpolate(obj, from, to, pos_map);
}

EAPI Eina_Bool
evas_vg_shape_equal_commands(Eo *obj, const Eo *with)
{
   return efl_gfx_path_equal_commands(obj, with);
}

EAPI Efl_Canvas_Vg_Node*
evas_vg_shape_add(Efl_Canvas_Vg_Node *parent)
{
   return efl_add(EFL_CANVAS_VG_SHAPE_CLASS, parent);
}

#include "efl_canvas_vg_shape.eo.c"
