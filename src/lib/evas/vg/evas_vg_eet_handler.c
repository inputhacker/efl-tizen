#include "evas_vg_common.h"

Eet_Data_Descriptor *_edje_edd_edje_rect_node = NULL;
Eet_Data_Descriptor *_edje_edd_edje_circle_node = NULL;
Eet_Data_Descriptor *_edje_edd_edje_ellipse_node = NULL;
Eet_Data_Descriptor *_edje_edd_edje_gradient_stops_node = NULL;
Eet_Data_Descriptor *_edje_edd_edje_linear_gradient_node = NULL;
Eet_Data_Descriptor *_edje_edd_edje_radial_gradient_node = NULL;
Eet_Data_Descriptor *_edje_edd_edje_style_gradient_node = NULL;
Eet_Data_Descriptor *_edje_edd_edje_style_property_node = NULL;
Eet_Data_Descriptor *_edje_edd_edje_matrix3_node = NULL;
Eet_Data_Descriptor *_edje_edd_edje_doc_node = NULL;
Eet_Data_Descriptor *_edje_edd_edje_defs_node = NULL;
Eet_Data_Descriptor *_edje_edd_edje_g_node = NULL;
Eet_Data_Descriptor *_edje_edd_edje_arc_node = NULL;
Eet_Data_Descriptor *_edje_edd_edje_path_node = NULL;
Eet_Data_Descriptor *_edje_edd_edje_polygon_node = NULL;
Eet_Data_Descriptor *_edje_edd_edje_vg_node = NULL;
Eet_Data_Descriptor *_edje_edd_edje_line_node = NULL;

#define FREE_DESCRIPTOR(eed)                      \
  if (eed)                              \
    {                                   \
       eet_data_descriptor_free((eed)); \
       (eed) = NULL;                    \
    }

static inline Eet_Data_Descriptor*
_eet_for_rect_node(void)
{
   Eet_Data_Descriptor *eet;
   Eet_Data_Descriptor_Class eetc;

   EET_EINA_STREAM_DATA_DESCRIPTOR_CLASS_SET(&eetc, Svg_Rect_Node);
   eet = eet_data_descriptor_stream_new(&eetc);
   EET_DATA_DESCRIPTOR_ADD_BASIC(eet, Svg_Rect_Node, "x", x, EET_T_DOUBLE);
   EET_DATA_DESCRIPTOR_ADD_BASIC(eet, Svg_Rect_Node, "y", y, EET_T_DOUBLE);
   EET_DATA_DESCRIPTOR_ADD_BASIC(eet, Svg_Rect_Node, "w", w, EET_T_DOUBLE);
   EET_DATA_DESCRIPTOR_ADD_BASIC(eet, Svg_Rect_Node, "h", h, EET_T_DOUBLE);
   EET_DATA_DESCRIPTOR_ADD_BASIC(eet, Svg_Rect_Node, "rx", rx, EET_T_DOUBLE);
   EET_DATA_DESCRIPTOR_ADD_BASIC(eet, Svg_Rect_Node, "ry", ry, EET_T_DOUBLE);
   return eet;
}

static inline Eet_Data_Descriptor*
_eet_for_line_node(void)
{
   Eet_Data_Descriptor *eet;
   Eet_Data_Descriptor_Class eetc;

   EET_EINA_STREAM_DATA_DESCRIPTOR_CLASS_SET(&eetc, Svg_Line_Node);
   eet = eet_data_descriptor_stream_new(&eetc);
   EET_DATA_DESCRIPTOR_ADD_BASIC(eet, Svg_Line_Node, "x1", x1, EET_T_DOUBLE);
   EET_DATA_DESCRIPTOR_ADD_BASIC(eet, Svg_Line_Node, "y1", y1, EET_T_DOUBLE);
   EET_DATA_DESCRIPTOR_ADD_BASIC(eet, Svg_Line_Node, "x2", x2, EET_T_DOUBLE);
   EET_DATA_DESCRIPTOR_ADD_BASIC(eet, Svg_Line_Node, "y2", y2, EET_T_DOUBLE);
   return eet;
}

static inline Eet_Data_Descriptor*
_eet_for_circle_node(void)
{
   Eet_Data_Descriptor *eet;
   Eet_Data_Descriptor_Class eetc;

   EET_EINA_STREAM_DATA_DESCRIPTOR_CLASS_SET(&eetc, Svg_Circle_Node);
   eet = eet_data_descriptor_stream_new(&eetc);
   EET_DATA_DESCRIPTOR_ADD_BASIC(eet, Svg_Circle_Node, "cx", cx, EET_T_DOUBLE);
   EET_DATA_DESCRIPTOR_ADD_BASIC(eet, Svg_Circle_Node, "cy", cy, EET_T_DOUBLE);
   EET_DATA_DESCRIPTOR_ADD_BASIC(eet, Svg_Circle_Node, "r", r, EET_T_DOUBLE);
   return eet;
}

static inline Eet_Data_Descriptor*
_eet_for_ellipse_node(void)
{
   Eet_Data_Descriptor *eet;
   Eet_Data_Descriptor_Class eetc;

   EET_EINA_STREAM_DATA_DESCRIPTOR_CLASS_SET(&eetc, Svg_Ellipse_Node);
   eet = eet_data_descriptor_stream_new(&eetc);
   EET_DATA_DESCRIPTOR_ADD_BASIC(eet, Svg_Ellipse_Node, "cx", cx, EET_T_DOUBLE);
   EET_DATA_DESCRIPTOR_ADD_BASIC(eet, Svg_Ellipse_Node, "cy", cy, EET_T_DOUBLE);
   EET_DATA_DESCRIPTOR_ADD_BASIC(eet, Svg_Ellipse_Node, "rx", rx, EET_T_DOUBLE);
   EET_DATA_DESCRIPTOR_ADD_BASIC(eet, Svg_Ellipse_Node, "ry", ry, EET_T_DOUBLE);
   return eet;
}


static inline Eet_Data_Descriptor*
_eet_for_gradient_stops(void)
{
   Eet_Data_Descriptor *eet;
   Eet_Data_Descriptor_Class eetc;

   EET_EINA_STREAM_DATA_DESCRIPTOR_CLASS_SET(&eetc, Efl_Gfx_Gradient_Stop);
   eet = eet_data_descriptor_stream_new(&eetc);
   EET_DATA_DESCRIPTOR_ADD_BASIC(eet, Efl_Gfx_Gradient_Stop, "offset", offset, EET_T_DOUBLE);
   EET_DATA_DESCRIPTOR_ADD_BASIC(eet, Efl_Gfx_Gradient_Stop, "r", r, EET_T_INT);
   EET_DATA_DESCRIPTOR_ADD_BASIC(eet, Efl_Gfx_Gradient_Stop, "g", g, EET_T_INT);
   EET_DATA_DESCRIPTOR_ADD_BASIC(eet, Efl_Gfx_Gradient_Stop, "b", b, EET_T_INT);
   EET_DATA_DESCRIPTOR_ADD_BASIC(eet, Efl_Gfx_Gradient_Stop, "a", a, EET_T_INT);
   return eet;
}

static inline Eet_Data_Descriptor*
_eet_for_linear_gradient(void)
{
   Eet_Data_Descriptor *eet;
   Eet_Data_Descriptor_Class eetc;

   EET_EINA_STREAM_DATA_DESCRIPTOR_CLASS_SET(&eetc, Svg_Linear_Gradient);
   eet = eet_data_descriptor_stream_new(&eetc);
   EET_DATA_DESCRIPTOR_ADD_BASIC(eet, Svg_Linear_Gradient, "x1", x1, EET_T_DOUBLE);
   EET_DATA_DESCRIPTOR_ADD_BASIC(eet, Svg_Linear_Gradient, "y1", y1, EET_T_DOUBLE);
   EET_DATA_DESCRIPTOR_ADD_BASIC(eet, Svg_Linear_Gradient, "x2", x2, EET_T_DOUBLE);
   EET_DATA_DESCRIPTOR_ADD_BASIC(eet, Svg_Linear_Gradient, "y2", y2, EET_T_DOUBLE);
   return eet;
}

static inline Eet_Data_Descriptor*
_eet_for_radial_gradient(void)
{
   Eet_Data_Descriptor *eet;
   Eet_Data_Descriptor_Class eetc;

   EET_EINA_STREAM_DATA_DESCRIPTOR_CLASS_SET(&eetc, Svg_Radial_Gradient);
   eet = eet_data_descriptor_stream_new(&eetc);
   EET_DATA_DESCRIPTOR_ADD_BASIC(eet, Svg_Radial_Gradient, "cx", cx, EET_T_DOUBLE);
   EET_DATA_DESCRIPTOR_ADD_BASIC(eet, Svg_Radial_Gradient, "cy", cy, EET_T_DOUBLE);
   EET_DATA_DESCRIPTOR_ADD_BASIC(eet, Svg_Radial_Gradient, "fx", fx, EET_T_DOUBLE);
   EET_DATA_DESCRIPTOR_ADD_BASIC(eet, Svg_Radial_Gradient, "fy", fy, EET_T_DOUBLE);
   EET_DATA_DESCRIPTOR_ADD_BASIC(eet, Svg_Radial_Gradient, "r", r, EET_T_DOUBLE);
   return eet;
}

static inline Eet_Data_Descriptor*
_eet_for_style_gradient(void)
{
   Eet_Data_Descriptor_Class eetc;

   if (_edje_edd_edje_style_gradient_node) return _edje_edd_edje_style_gradient_node;

   EET_EINA_STREAM_DATA_DESCRIPTOR_CLASS_SET(&eetc, Svg_Style_Gradient);
   _edje_edd_edje_style_gradient_node = eet_data_descriptor_stream_new(&eetc);
   _edje_edd_edje_gradient_stops_node = _eet_for_gradient_stops();
   _edje_edd_edje_linear_gradient_node = _eet_for_linear_gradient();
   _edje_edd_edje_radial_gradient_node = _eet_for_radial_gradient();

   EET_DATA_DESCRIPTOR_ADD_BASIC(_edje_edd_edje_style_gradient_node, Svg_Style_Gradient, "type", type, EET_T_INT);
   EET_DATA_DESCRIPTOR_ADD_BASIC(_edje_edd_edje_style_gradient_node, Svg_Style_Gradient, "id", id, EET_T_STRING);
   EET_DATA_DESCRIPTOR_ADD_BASIC(_edje_edd_edje_style_gradient_node, Svg_Style_Gradient, "spread", spread, EET_T_INT);
   EET_DATA_DESCRIPTOR_ADD_LIST(_edje_edd_edje_style_gradient_node, Svg_Style_Gradient, "stops", stops, _edje_edd_edje_gradient_stops_node);
   EET_DATA_DESCRIPTOR_ADD_SUB(_edje_edd_edje_style_gradient_node, Svg_Style_Gradient, "radial", radial, _edje_edd_edje_radial_gradient_node);
   EET_DATA_DESCRIPTOR_ADD_SUB(_edje_edd_edje_style_gradient_node, Svg_Style_Gradient, "linear", linear, _edje_edd_edje_linear_gradient_node);

   return _edje_edd_edje_style_gradient_node;
}

static inline Eet_Data_Descriptor*
_eet_for_style_property(void)
{
   Eet_Data_Descriptor *eet, *eet_gradient, *eet_dash;
   Eet_Data_Descriptor_Class eetc, eetc_dash;

   EET_EINA_STREAM_DATA_DESCRIPTOR_CLASS_SET(&eetc, Svg_Style_Property);
   eet = eet_data_descriptor_stream_new(&eetc);
   eet_gradient = _eet_for_style_gradient();

   EET_EINA_STREAM_DATA_DESCRIPTOR_CLASS_SET(&eetc_dash, Efl_Gfx_Dash);
   eet_dash = eet_data_descriptor_stream_new(&eetc_dash);
   EET_DATA_DESCRIPTOR_ADD_BASIC(eet_dash, Efl_Gfx_Dash, "length", length, EET_T_DOUBLE);
   EET_DATA_DESCRIPTOR_ADD_BASIC(eet_dash, Efl_Gfx_Dash, "gap", gap, EET_T_DOUBLE);

   EET_DATA_DESCRIPTOR_ADD_BASIC(eet, Svg_Style_Property, "r", r, EET_T_INT);
   EET_DATA_DESCRIPTOR_ADD_BASIC(eet, Svg_Style_Property, "g", g, EET_T_INT);
   EET_DATA_DESCRIPTOR_ADD_BASIC(eet, Svg_Style_Property, "b", b, EET_T_INT);
   // for fill
   EET_DATA_DESCRIPTOR_ADD_BASIC(eet, Svg_Style_Property, "fill.flags", fill.flags, EET_T_INT);
   EET_DATA_DESCRIPTOR_ADD_BASIC(eet, Svg_Style_Property, "fill.paint.r", fill.paint.r, EET_T_INT);
   EET_DATA_DESCRIPTOR_ADD_BASIC(eet, Svg_Style_Property, "fill.paint.g", fill.paint.g, EET_T_INT);
   EET_DATA_DESCRIPTOR_ADD_BASIC(eet, Svg_Style_Property, "fill.paint.b", fill.paint.b, EET_T_INT);
   EET_DATA_DESCRIPTOR_ADD_BASIC(eet, Svg_Style_Property, "fill.paint.none", fill.paint.none, EET_T_INT);
   EET_DATA_DESCRIPTOR_ADD_BASIC(eet, Svg_Style_Property, "fill.paint.cur_color", fill.paint.cur_color, EET_T_INT);
   EET_DATA_DESCRIPTOR_ADD_SUB(eet, Svg_Style_Property, "fill.paint.gradient", fill.paint.gradient, eet_gradient);
   EET_DATA_DESCRIPTOR_ADD_BASIC(eet, Svg_Style_Property, "fill.paint.url", fill.paint.url, EET_T_STRING);
   EET_DATA_DESCRIPTOR_ADD_BASIC(eet, Svg_Style_Property, "fill.opacity", fill.opacity, EET_T_INT);
   EET_DATA_DESCRIPTOR_ADD_BASIC(eet, Svg_Style_Property, "fill.fill_rule", fill.fill_rule, EET_T_INT);

   // for stroke
   EET_DATA_DESCRIPTOR_ADD_BASIC(eet, Svg_Style_Property, "stroke.flags", stroke.flags, EET_T_INT);
   EET_DATA_DESCRIPTOR_ADD_BASIC(eet, Svg_Style_Property, "stroke.paint.r", stroke.paint.r, EET_T_INT);
   EET_DATA_DESCRIPTOR_ADD_BASIC(eet, Svg_Style_Property, "stroke.paint.g", stroke.paint.g, EET_T_INT);
   EET_DATA_DESCRIPTOR_ADD_BASIC(eet, Svg_Style_Property, "stroke.paint.b", stroke.paint.b, EET_T_INT);
   EET_DATA_DESCRIPTOR_ADD_BASIC(eet, Svg_Style_Property, "stroke.paint.none", stroke.paint.none, EET_T_INT);
   EET_DATA_DESCRIPTOR_ADD_BASIC(eet, Svg_Style_Property, "stroke.paint.cur_color", stroke.paint.cur_color, EET_T_INT);
   EET_DATA_DESCRIPTOR_ADD_SUB(eet, Svg_Style_Property, "stroke.paint.gradient", stroke.paint.gradient, eet_gradient);
   EET_DATA_DESCRIPTOR_ADD_BASIC(eet, Svg_Style_Property, "stroke.paint.url", stroke.paint.url, EET_T_STRING);
   EET_DATA_DESCRIPTOR_ADD_BASIC(eet, Svg_Style_Property, "stroke.opacity", stroke.opacity, EET_T_INT);
   EET_DATA_DESCRIPTOR_ADD_BASIC(eet, Svg_Style_Property, "stroke.scale", stroke.scale, EET_T_DOUBLE);
   EET_DATA_DESCRIPTOR_ADD_BASIC(eet, Svg_Style_Property, "stroke.width", stroke.width, EET_T_DOUBLE);
   EET_DATA_DESCRIPTOR_ADD_BASIC(eet, Svg_Style_Property, "stroke.centered", stroke.centered, EET_T_DOUBLE);
   EET_DATA_DESCRIPTOR_ADD_BASIC(eet, Svg_Style_Property, "stroke.cap", stroke.cap, EET_T_INT);
   EET_DATA_DESCRIPTOR_ADD_BASIC(eet, Svg_Style_Property, "stroke.join", stroke.join, EET_T_INT);
   EET_DATA_DESCRIPTOR_ADD_VAR_ARRAY(eet, Svg_Style_Property, "stroke.dash", stroke.dash, eet_dash);
   EET_DATA_DESCRIPTOR_ADD_BASIC(eet, Svg_Style_Property, "stroke.dash_count", stroke.dash_count, EET_T_INT);

   return eet;
}

static Eet_Data_Descriptor*
_eet_for_eina_matrix3(void)
{
   Eet_Data_Descriptor *eet;
   Eet_Data_Descriptor_Class eetc;

   EET_EINA_STREAM_DATA_DESCRIPTOR_CLASS_SET(&eetc, Eina_Matrix3);
   eet = eet_data_descriptor_stream_new(&eetc);
   EET_DATA_DESCRIPTOR_ADD_BASIC(eet, Eina_Matrix3, "xx", xx, EET_T_DOUBLE);
   EET_DATA_DESCRIPTOR_ADD_BASIC(eet, Eina_Matrix3, "xy", xy, EET_T_DOUBLE);
   EET_DATA_DESCRIPTOR_ADD_BASIC(eet, Eina_Matrix3, "xz", xz, EET_T_DOUBLE);
   EET_DATA_DESCRIPTOR_ADD_BASIC(eet, Eina_Matrix3, "yx", yx, EET_T_DOUBLE);
   EET_DATA_DESCRIPTOR_ADD_BASIC(eet, Eina_Matrix3, "yy", yy, EET_T_DOUBLE);
   EET_DATA_DESCRIPTOR_ADD_BASIC(eet, Eina_Matrix3, "yz", yz, EET_T_DOUBLE);
   EET_DATA_DESCRIPTOR_ADD_BASIC(eet, Eina_Matrix3, "zx", zx, EET_T_DOUBLE);
   EET_DATA_DESCRIPTOR_ADD_BASIC(eet, Eina_Matrix3, "zy", zy, EET_T_DOUBLE);
   EET_DATA_DESCRIPTOR_ADD_BASIC(eet, Eina_Matrix3, "zz", zz, EET_T_DOUBLE);

   return eet;
}

static inline Eet_Data_Descriptor*
_eet_for_doc_node(void)
{
   Eet_Data_Descriptor *eet;
   Eet_Data_Descriptor_Class eetc;

   EET_EINA_STREAM_DATA_DESCRIPTOR_CLASS_SET(&eetc, Svg_Doc_Node);
   eet = eet_data_descriptor_stream_new(&eetc);
   EET_DATA_DESCRIPTOR_ADD_BASIC(eet, Svg_Doc_Node, "width", width, EET_T_DOUBLE);
   EET_DATA_DESCRIPTOR_ADD_BASIC(eet, Svg_Doc_Node, "height", height, EET_T_DOUBLE);
   EET_DATA_DESCRIPTOR_ADD_BASIC(eet, Svg_Doc_Node, "vx", vx, EET_T_DOUBLE);
   EET_DATA_DESCRIPTOR_ADD_BASIC(eet, Svg_Doc_Node, "vy", vy, EET_T_DOUBLE);
   EET_DATA_DESCRIPTOR_ADD_BASIC(eet, Svg_Doc_Node, "vw", vw, EET_T_DOUBLE);
   EET_DATA_DESCRIPTOR_ADD_BASIC(eet, Svg_Doc_Node, "vh", vh, EET_T_DOUBLE);
   return eet;
}

static inline Eet_Data_Descriptor*
_eet_for_defs_node(void)
{
   Eet_Data_Descriptor *eet, *eet_gradient;
   Eet_Data_Descriptor_Class eetc;

   EET_EINA_STREAM_DATA_DESCRIPTOR_CLASS_SET(&eetc, Svg_Defs_Node);
   eet = eet_data_descriptor_stream_new(&eetc);
   eet_gradient = _eet_for_style_gradient();

   EET_DATA_DESCRIPTOR_ADD_LIST(eet, Svg_Defs_Node, "gradients", gradients, eet_gradient);
   return eet;
}

static inline Eet_Data_Descriptor*
_eet_for_g_node(void)
{
   Eet_Data_Descriptor *eet;
   Eet_Data_Descriptor_Class eetc;

   EET_EINA_STREAM_DATA_DESCRIPTOR_CLASS_SET(&eetc, Svg_G_Node);
   eet = eet_data_descriptor_stream_new(&eetc);
   return eet;
}

static inline Eet_Data_Descriptor*
_eet_for_arc_node(void)
{
   Eet_Data_Descriptor *eet;
   Eet_Data_Descriptor_Class eetc;

   EET_EINA_STREAM_DATA_DESCRIPTOR_CLASS_SET(&eetc, Svg_Arc_Node);
   eet = eet_data_descriptor_stream_new(&eetc);
   return eet;
}

static inline Eet_Data_Descriptor*
_eet_for_polygon_node(void)
{
   Eet_Data_Descriptor *eet;
   Eet_Data_Descriptor_Class eetc;

   EET_EINA_STREAM_DATA_DESCRIPTOR_CLASS_SET(&eetc, Svg_Polygon_Node);
   eet = eet_data_descriptor_stream_new(&eetc);
   EET_DATA_DESCRIPTOR_ADD_BASIC(eet, Svg_Polygon_Node, "points_count", points_count, EET_T_INT);
   EET_DATA_DESCRIPTOR_ADD_BASIC_VAR_ARRAY(eet, Svg_Polygon_Node, "points", points, EET_T_DOUBLE);
   return eet;
}

static inline Eet_Data_Descriptor*
_eet_for_path_node(void)
{
   Eet_Data_Descriptor *eet;
   Eet_Data_Descriptor_Class eetc;

   EET_EINA_STREAM_DATA_DESCRIPTOR_CLASS_SET(&eetc, Svg_Path_Node);
   eet = eet_data_descriptor_stream_new(&eetc);
   EET_DATA_DESCRIPTOR_ADD_BASIC(eet, Svg_Path_Node, "path", path, EET_T_STRING);
   return eet;
}

struct
{
   Svg_Node_Type u;
   const char       *name;
} eet_mapping[] = {
   { SVG_NODE_DOC, "doc" },
   { SVG_NODE_G, "g" },
   { SVG_NODE_DEFS, "defs" },
   { SVG_NODE_ARC, "arc" },
   { SVG_NODE_CIRCLE, "circle" },
   { SVG_NODE_ELLIPSE, "ellipse" },
   { SVG_NODE_POLYGON, "polygon" },
   { SVG_NODE_POLYLINE, "polyline" },
   { SVG_NODE_RECT, "rect" },
   { SVG_NODE_PATH, "path" },
   { SVG_NODE_LINE, "line" },
   { SVG_NODE_UNKNOWN, NULL }
};

static const char *
/* union
   type_get() */
_union_type_get(const void *data,
                Eina_Bool  *unknow)
{
   const Svg_Node_Type *u = data;
   int i;

   if (unknow)
     *unknow = EINA_FALSE;

   for (i = 0; eet_mapping[i].name != NULL; ++i)
     if (*u == eet_mapping[i].u)
       return eet_mapping[i].name;

   if (unknow)
     *unknow = EINA_TRUE;
   return NULL;
} /* _union_type_get */

static Eina_Bool
_union_type_set(const char *type,
                void       *data,
                Eina_Bool   unknow)
{
   Svg_Node_Type *u = data;
   int i;

   if (unknow)
     return EINA_FALSE;

   for (i = 0; eet_mapping[i].name != NULL; ++i)
     if (strcmp(eet_mapping[i].name, type) == 0)
       {
          *u = eet_mapping[i].u;
          return EINA_TRUE;
       }
   return EINA_FALSE;
} /* _union_type_set */

EAPI Eet_Data_Descriptor *
_evas_vg_svg_node_eet(void)
{
   Eet_Data_Descriptor *eet_union;
   Eet_Data_Descriptor_Class eetc;

   if (_edje_edd_edje_vg_node) return _edje_edd_edje_vg_node;

   EET_EINA_STREAM_DATA_DESCRIPTOR_CLASS_SET(&eetc, Svg_Node);
   _edje_edd_edje_vg_node = eet_data_descriptor_stream_new(&eetc);

   eetc.version = EET_DATA_DESCRIPTOR_CLASS_VERSION;
   eetc.func.type_get = _union_type_get;
   eetc.func.type_set = _union_type_set;
   eet_union = eet_data_descriptor_stream_new(&eetc);

   _edje_edd_edje_doc_node = _eet_for_doc_node();
   _edje_edd_edje_g_node = _eet_for_g_node();
   _edje_edd_edje_defs_node = _eet_for_defs_node();
   _edje_edd_edje_arc_node = _eet_for_arc_node();
   _edje_edd_edje_circle_node = _eet_for_circle_node();
   _edje_edd_edje_ellipse_node = _eet_for_ellipse_node();
   _edje_edd_edje_rect_node = _eet_for_rect_node();
   _edje_edd_edje_line_node = _eet_for_line_node();
   _edje_edd_edje_path_node = _eet_for_path_node();
   _edje_edd_edje_polygon_node = _eet_for_polygon_node();
   _edje_edd_edje_style_property_node = _eet_for_style_property();
   _edje_edd_edje_matrix3_node = _eet_for_eina_matrix3();


   EET_DATA_DESCRIPTOR_ADD_MAPPING(eet_union, "doc", _edje_edd_edje_doc_node);
   EET_DATA_DESCRIPTOR_ADD_MAPPING(eet_union, "g", _edje_edd_edje_g_node);
   EET_DATA_DESCRIPTOR_ADD_MAPPING(eet_union, "defs", _edje_edd_edje_defs_node);
   EET_DATA_DESCRIPTOR_ADD_MAPPING(eet_union, "arc", _edje_edd_edje_arc_node);
   EET_DATA_DESCRIPTOR_ADD_MAPPING(eet_union, "circle", _edje_edd_edje_circle_node);
   EET_DATA_DESCRIPTOR_ADD_MAPPING(eet_union, "ellipse", _edje_edd_edje_ellipse_node);
   EET_DATA_DESCRIPTOR_ADD_MAPPING(eet_union, "rect", _edje_edd_edje_rect_node);
   EET_DATA_DESCRIPTOR_ADD_MAPPING(eet_union, "line", _edje_edd_edje_line_node);
   EET_DATA_DESCRIPTOR_ADD_MAPPING(eet_union, "path", _edje_edd_edje_path_node);
   EET_DATA_DESCRIPTOR_ADD_MAPPING(eet_union, "polygon", _edje_edd_edje_polygon_node);
   EET_DATA_DESCRIPTOR_ADD_MAPPING(eet_union, "polyline", _edje_edd_edje_polygon_node);

   EET_DATA_DESCRIPTOR_ADD_UNION(_edje_edd_edje_vg_node, Svg_Node, "node", node, type, eet_union);


   EET_DATA_DESCRIPTOR_ADD_LIST(_edje_edd_edje_vg_node, Svg_Node, "child", child, _edje_edd_edje_vg_node);
   EET_DATA_DESCRIPTOR_ADD_BASIC(_edje_edd_edje_vg_node, Svg_Node, "id", id, EET_T_STRING);
   EET_DATA_DESCRIPTOR_ADD_SUB(_edje_edd_edje_vg_node, Svg_Node, "style", style, _edje_edd_edje_style_property_node);
   EET_DATA_DESCRIPTOR_ADD_SUB(_edje_edd_edje_vg_node, Svg_Node, "transform", transform, _edje_edd_edje_matrix3_node);

   return _edje_edd_edje_vg_node;
}

EAPI void 
_evas_vg_svg_node_eet_destroy(void)
{
   FREE_DESCRIPTOR(_edje_edd_edje_rect_node);
   FREE_DESCRIPTOR(_edje_edd_edje_circle_node);
   FREE_DESCRIPTOR(_edje_edd_edje_ellipse_node);
   FREE_DESCRIPTOR(_edje_edd_edje_gradient_stops_node);
   FREE_DESCRIPTOR(_edje_edd_edje_linear_gradient_node);
   FREE_DESCRIPTOR(_edje_edd_edje_radial_gradient_node);
   FREE_DESCRIPTOR(_edje_edd_edje_style_gradient_node);
   FREE_DESCRIPTOR(_edje_edd_edje_style_property_node);
   FREE_DESCRIPTOR(_edje_edd_edje_matrix3_node);
   FREE_DESCRIPTOR(_edje_edd_edje_doc_node);
   FREE_DESCRIPTOR(_edje_edd_edje_defs_node);
   FREE_DESCRIPTOR(_edje_edd_edje_g_node);
   FREE_DESCRIPTOR(_edje_edd_edje_arc_node);
   FREE_DESCRIPTOR(_edje_edd_edje_path_node);
   FREE_DESCRIPTOR(_edje_edd_edje_polygon_node);
   FREE_DESCRIPTOR(_edje_edd_edje_vg_node);
   FREE_DESCRIPTOR(_edje_edd_edje_line_node);
}



static Efl_VG *
_apply_gradient_property(Svg_Style_Gradient *g)
{
   Efl_VG *grad_obj = NULL;
   Efl_Gfx_Gradient_Stop *stops, *stop;
   int stop_count = 0, i = 0;
   Eina_List *l;

   if (g->type == SVG_LINEAR_GRADIENT)
     {
        grad_obj = eo_add(EFL_VG_GRADIENT_LINEAR_CLASS, NULL);
        evas_vg_gradient_linear_start_set(grad_obj, g->linear->x1, g->linear->y1);
        evas_vg_gradient_linear_end_set(grad_obj, g->linear->x2, g->linear->y2);
     }
   else if (g->type == SVG_RADIAL_GRADIENT)
     {
        grad_obj = eo_add(EFL_VG_GRADIENT_RADIAL_CLASS, NULL);
        evas_vg_gradient_radial_center_set(grad_obj, g->radial->cx, g->radial->cy);
        evas_vg_gradient_radial_radius_set(grad_obj, g->radial->r);
        evas_vg_gradient_radial_focal_set(grad_obj, g->radial->fx, g->radial->fy);
     }
   else
     {
        // not a known gradient
        return NULL;
     }
   // apply common prperty
   evas_vg_gradient_spread_set(grad_obj, g->spread);
   // update the stops
   stop_count = eina_list_count(g->stops);
   if (stop_count)
     {
        stops = calloc(stop_count, sizeof(Efl_Gfx_Gradient_Stop));
        i = 0;
        EINA_LIST_FOREACH(g->stops, l, stop)
          {
             stops[i].r = stop->r;
             stops[i].g = stop->g;
             stops[i].b = stop->b;
             stops[i].a = stop->a;
             stops[i].offset = stop->offset;
             i++;
          }
        evas_vg_gradient_stop_set(grad_obj, stops, stop_count);
        free(stops);
     }
   return grad_obj;
}

// vg tree creation
static void
_apply_vg_property(Svg_Node *node, Efl_VG *vg)
{
   Svg_Style_Property *style = node->style;

   // update the vg name
   if (node->id)
     evas_vg_node_name_set(vg, node->id);

   // apply the transformation
   if (node->transform)
     evas_vg_node_transformation_set(vg, node->transform);

   if (node->type == SVG_NODE_G) return;

   // apply the fill style property
   eo_do(vg, efl_gfx_shape_fill_rule_set(style->fill.fill_rule));
   // if fill property is NULL then do nothing
   if (style->fill.paint.none)
     {
        //do nothing
     }
   else if (style->fill.paint.gradient)
     {
        // if the fill has gradient then apply.
        evas_vg_shape_fill_set(vg, _apply_gradient_property(style->fill.paint.gradient));
     }
   else if (style->fill.paint.cur_color)
     {
        // apply the current style color
        evas_vg_node_color_set(vg, style->r, style->g,
                               style->b, style->fill.opacity);
     }
   else
     {
        // apply the fill color
        evas_vg_node_color_set(vg, style->fill.paint.r, style->fill.paint.g,
                               style->fill.paint.b, style->fill.opacity);
     }

   // apply the stroke style property
   //@TODO HACK, fix the below api to take the stroke width as pixels
   // rightnow it draws double the pixel (inside and outside the outline)
   evas_vg_shape_stroke_width_set(vg, style->stroke.width/2.0);
   evas_vg_shape_stroke_cap_set(vg, style->stroke.cap);
   evas_vg_shape_stroke_join_set(vg, style->stroke.join);
   evas_vg_shape_stroke_scale_set(vg, style->stroke.scale);
   // if stroke property is NULL then do nothing
   if (style->stroke.paint.none)
     {
        //do nothing
     }
   else if (style->stroke.paint.gradient)
     {
        // if the fill has gradient then apply.
        evas_vg_shape_stroke_fill_set(vg, _apply_gradient_property(style->stroke.paint.gradient));
     }
   else if (style->stroke.paint.url)
     {
        // apply the color pointed by url
        // TODO
     }
   else if (style->stroke.paint.cur_color)
     {
        // apply the current style color
        evas_vg_shape_stroke_color_set(vg, style->r, style->g,
                                       style->b, style->stroke.opacity);
     }
   else
     {
        // apply the stroke color
        evas_vg_shape_stroke_color_set(vg, style->stroke.paint.r, style->stroke.paint.g,
                                       style->stroke.paint.b, style->stroke.opacity);
     }
}

static void
_add_polyline(Efl_VG *vg, double *array, int size, Eina_Bool polygon)
{
   int i;

   if (size < 2) return;

   evas_vg_shape_shape_append_move_to(vg, array[0], array[1]);
   for (i=2; i < size; i+=2)
     evas_vg_shape_shape_append_line_to(vg, array[i], array[i+1]);

   if (polygon)
     evas_vg_shape_shape_append_close(vg);
}

static void
_create_vg_node(Svg_Node *node, Efl_VG *parent)
{
   Efl_VG *vg = NULL;
   Svg_Node *child;
   Eina_List *l;

   switch (node->type)
     {
        case SVG_NODE_G:
           {
              vg = evas_vg_container_add(parent);
              _apply_vg_property(node, vg);
              EINA_LIST_FOREACH(node->child, l, child)
                {
                   _create_vg_node(child, vg);
                }
              return;
           }
           break;
        case SVG_NODE_PATH:
           vg = evas_vg_shape_add(parent);
           evas_vg_shape_shape_append_svg_path(vg, node->node.path.path);
           break;
        case SVG_NODE_POLYGON:
           vg = evas_vg_shape_add(parent);
           _add_polyline(vg, node->node.polygon.points, node->node.polygon.points_count, EINA_TRUE);
           break;
        case SVG_NODE_POLYLINE:
           vg = evas_vg_shape_add(parent);
           _add_polyline(vg, node->node.polygon.points, node->node.polygon.points_count, EINA_FALSE);
           break;
        case SVG_NODE_ELLIPSE:
           vg = evas_vg_shape_add(parent);
           evas_vg_shape_shape_append_arc(vg, node->node.ellipse.cx - node->node.ellipse.rx,
                                          node->node.ellipse.cy - node->node.ellipse.ry,
                                          2*node->node.ellipse.rx, 2*node->node.ellipse.ry, 0, 360);
           evas_vg_shape_shape_append_close(vg);
           break;
        case SVG_NODE_CIRCLE:
           vg = evas_vg_shape_add(parent);
           evas_vg_shape_shape_append_circle(vg, node->node.circle.cx, node->node.circle.cy, node->node.circle.r);
           break;
        case SVG_NODE_RECT:
           vg = evas_vg_shape_add(parent);
           evas_vg_shape_shape_append_rect(vg, node->node.rect.x, node->node.rect.y, node->node.rect.w, node->node.rect.h,
                                           node->node.rect.rx, node->node.rect.ry);
           break;
        case SVG_NODE_LINE:
           vg = evas_vg_shape_add(parent);
           evas_vg_shape_shape_append_move_to(vg, node->node.line.x1, node->node.line.y1);
           evas_vg_shape_shape_append_line_to(vg, node->node.line.x2, node->node.line.y2);
           break;
       default:
           break;
     }
   _apply_vg_property(node, vg);
}

static void
_apply_stroke_scale(Efl_VG *node, double scale)
{
   Efl_VG *child;
   Eina_Iterator *itr;

   if (eo_isa(node, EFL_VG_CONTAINER_CLASS))
     {
        eo_do(node, itr = efl_vg_container_children_get());
        EINA_ITERATOR_FOREACH(itr, child)
          _apply_stroke_scale(child, scale);
        eina_iterator_free(itr);
     }
   else
     {
         evas_vg_shape_stroke_scale_set(node, scale);
     }
}

Vg_Data *
_evas_vg_load_vg_data(Eina_Stringshare *path, int svg_id)
{
   Eina_List *l;
   Vg_Data *vector;
   char svg_key[20];
   Eet_Data_Descriptor *svg_node_eet;
   Svg_Node *child;
   Svg_Node *node;
   Eet_File *ef;
   Efl_VG *root = NULL;

   ef = eet_open(path, EET_FILE_MODE_READ);
   if (!ef) return NULL;

   // create and put it in the cache.
   vector = calloc(1, sizeof(Vg_Data));
   vector->svg_id = svg_id;

   snprintf(svg_key, sizeof(svg_key), "edje/vectors/%i", svg_id);
   svg_node_eet = _evas_vg_svg_node_eet();
   node = eet_data_read(ef, svg_node_eet, svg_key);

   if (!node || (node->type != SVG_NODE_DOC))
     {
        root = NULL;
     }
   else
     {
        root = evas_vg_container_add(NULL);
        EINA_LIST_FOREACH(node->child, l, child)
          {
             _create_vg_node(child, root);
          }
        vector->x = node->node.doc.vx;
        vector->y = node->node.doc.vy;
        vector->w = node->node.doc.vw;
        vector->h = node->node.doc.vh;
     }
   vector->vg = root;
   _evas_vg_svg_node_free(node);
   eet_close(ef);

   return vector;
}

static void
_apply_transformation(Efl_VG *root, double w, double h, Vg_Data *vg_data)
{
   double sx, sy, scale;
   Eina_Matrix3 m;

   sx = w/vg_data->w;
   sy = h/vg_data->h;
   scale = sx < sy ? sx: sy;
   eina_matrix3_identity(&m);
   // allign hcenter and vcenter
   //@TODO take care of the preserveaspectratio attribute
   eina_matrix3_translate(&m, (w - vg_data->w * scale)/2.0, (h - vg_data->h * scale)/2.0);
   eina_matrix3_scale(&m, scale, scale);
   eina_matrix3_translate(&m, -vg_data->x, -vg_data->y);
   evas_vg_node_transformation_set(root, &m);
   _apply_stroke_scale(root, scale);
}

Efl_VG *
_evas_vg_dup_vg_tree(Vg_Data *src, Vg_Data *dest, float pos, double w, double h)
{

   Efl_VG *root;
   Eina_Matrix3 m;

   if (!src) return NULL;
   if (w==0 || h ==0 ) return NULL;
   if (!dest)
     {
        root = evas_vg_container_add(NULL);
        evas_vg_node_dup(root, src->vg);
        _apply_transformation(root, w, h, src);
     }
   else
     {
        root = evas_vg_container_add(NULL);
        evas_vg_node_dup(root, src->vg);

        // for start vector
        _apply_transformation(src->vg, w, h, src);

        // for end vector
        _apply_transformation(dest->vg, w, h, dest);

        // do the interpolation
        if (!evas_vg_node_interpolate(root, src->vg, dest->vg, pos))
          {
             ;//ERR(" Can't interpolate check the svg file \n");
          }
        // performance hack
        // instead of duplicating the tree and applying the transformation
        // i just updated the transformation matrix and reset it back to null.
        // assumption is that the root vg will never have a transformation
        eina_matrix3_identity(&m);
        evas_vg_node_transformation_set(src->vg, &m);
        evas_vg_node_transformation_set(dest->vg, &m);
     }
   return root;
}

static void
_svg_style_gradient_free(Svg_Style_Gradient *grad)
{
   Efl_Gfx_Gradient_Stop *stop;

   if (!grad) return;

   eina_stringshare_del(grad->id);
   eina_stringshare_del(grad->ref);
   free(grad->radial);
   free(grad->linear);

   EINA_LIST_FREE(grad->stops, stop)
     {
        free(stop);
     }
   free(grad);
}

static void
_node_style_free(Svg_Style_Property *style)
{
   if (!style) return;

   _svg_style_gradient_free(style->fill.paint.gradient);
   eina_stringshare_del(style->fill.paint.url);
   _svg_style_gradient_free(style->stroke.paint.gradient);
   eina_stringshare_del(style->stroke.paint.url);
   free(style);
}

EAPI void
_evas_vg_svg_node_free(Svg_Node *node)
{
   Svg_Node *child;
   Svg_Style_Gradient *grad;

   if (!node) return;

   EINA_LIST_FREE(node->child, child)
     {
        _evas_vg_svg_node_free(child);
     }

   eina_stringshare_del(node->id);
   free(node->transform);
   _node_style_free(node->style);
   switch (node->type)
     {
        case SVG_NODE_PATH:
           eina_stringshare_del(node->node.path.path);
           break;
        case SVG_NODE_POLYGON:
           free(node->node.polygon.points);
           break;
        case SVG_NODE_POLYLINE:
           free(node->node.polyline.points);
           break;
        case SVG_NODE_DOC:
           _evas_vg_svg_node_free(node->node.doc.defs);
           break;
        case SVG_NODE_DEFS:
           EINA_LIST_FREE(node->node.defs.gradients, grad)
             {
                _svg_style_gradient_free(grad);
             }
           break;
        default:
           break;
     }
  free(node);
}
