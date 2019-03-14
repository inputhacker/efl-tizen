
EAPI void
edje_object_animation_set(Efl_Canvas_Layout *obj, Eina_Bool on)
{
   efl_canvas_layout_animation_set(obj, on);
}

EAPI Eina_Bool
edje_object_animation_get(const Efl_Canvas_Layout *obj)
{
   return efl_canvas_layout_animation_get(obj);
}

EAPI Efl_Input_Device *
edje_object_seat_get(const Efl_Canvas_Layout *obj, Eina_Stringshare *name)
{
   return efl_canvas_layout_seat_get(obj, name);
}

EAPI Eina_Stringshare *
edje_object_seat_name_get(const Efl_Canvas_Layout *obj, Efl_Input_Device *device)
{
   return efl_canvas_layout_seat_name_get(obj, device);
}

EAPI Eina_Error
edje_object_layout_load_error_get(const Efl_Canvas_Layout *obj)
{
   return efl_canvas_layout_load_error_get(obj);
}

EAPI Eina_Bool
edje_object_content_remove(Efl_Canvas_Layout *obj, Efl_Gfx_Entity *content)
{
   return efl_canvas_layout_content_remove(obj, content);
}

/***********************************************************************************
 * TIZEN_ONLY_FEATURE: API for handling text properties of Edje                    *
 ***********************************************************************************/
EAPI Eina_Bool
edje_object_part_text_min_policy_set(Efl_Canvas_Layout *obj, const char *part, const char *state_name, Eina_Bool min_x, Eina_Bool min_y)
{
   return efl_canvas_layout_part_text_min_policy_set(obj, part, state_name, min_x, min_y);
}

EAPI Eina_Bool
edje_object_part_text_min_policy_get(const Efl_Canvas_Layout *obj, const char *part, const char *state_name, Eina_Bool *min_x, Eina_Bool *min_y)
{
   return efl_canvas_layout_part_text_min_policy_get(obj, part, state_name, min_x, min_y);
}

EAPI Eina_Bool
edje_object_part_text_valign_set(Efl_Canvas_Layout *obj, const char *part, double valign)
{
   return efl_canvas_layout_part_text_valign_set(obj, part, valign);
}

EAPI double
edje_object_part_text_valign_get(const Efl_Canvas_Layout *obj, const char *part)
{
   return efl_canvas_layout_part_text_valign_get(obj, part);
}
/*******
 * END *
 *******/

/***********************************************************************************
 * TIZEN_ONLY_FEATURE: ellipsize.marquee, ellipsize.fade for TEXTBLOCK, TEXT part. *
 ***********************************************************************************/
EAPI Eina_Bool
edje_object_part_text_marquee_duration_set(Efl_Canvas_Layout *obj, const char *part, double duration)
{
   return efl_canvas_layout_part_text_marquee_duration_set(obj, part, duration);
}

EAPI double
edje_object_part_text_marquee_duration_get(const Efl_Canvas_Layout *obj, const char *part)
{
   return efl_canvas_layout_part_text_marquee_duration_get(obj, part);
}

EAPI Eina_Bool
edje_object_part_text_marquee_speed_set(Efl_Canvas_Layout *obj, const char *part, double speed)
{
   return efl_canvas_layout_part_text_marquee_speed_set(obj, part, speed);
}

EAPI double
edje_object_part_text_marquee_speed_get(const Efl_Canvas_Layout *obj, const char *part)
{
   return efl_canvas_layout_part_text_marquee_speed_get(obj, part);
}

EAPI Eina_Bool
edje_object_part_text_marquee_always_set(Efl_Canvas_Layout *obj, const char *part, Eina_Bool always)
{
   return efl_canvas_layout_part_text_marquee_always_set(obj, part, always);
}

EAPI Eina_Bool
edje_object_part_text_marquee_always_get(const Efl_Canvas_Layout *obj, const char *part)
{
   return efl_canvas_layout_part_text_marquee_always_get(obj, part);
}
/*******
 * END *
 *******/

/***********************************************************************************
 * TIZEN_ONLY_FEATURE: apply Tizen's color_class features.                         *
 ***********************************************************************************/
EAPI void
edje_object_color_class_parent_set(Efl_Canvas_Layout *obj, Efl_Object *parent)
{
   efl_canvas_layout_color_class_parent_set(obj, parent);
}

EAPI void
edje_object_color_class_parent_unset(Efl_Canvas_Layout *obj)
{
   efl_canvas_layout_color_class_parent_unset(obj);
}
/*******
 * END *
 *******/

/***********************************************************************************
 * TIZEN_ONLY_FEATURE: API for handling common properties of Edje                  *
 ***********************************************************************************/
EAPI Eina_Bool
edje_object_part_valign_set(Efl_Canvas_Layout *obj, const char *part, double valign)
{
   return efl_canvas_layout_part_valign_set(obj, part, valign);
}

EAPI double
edje_object_part_valign_get(const Efl_Canvas_Layout *obj, const char *part)
{
   return efl_canvas_layout_part_valign_get(obj, part);
}

EAPI void
edje_object_part_text_cursor_coord_get(Efl_Canvas_Layout *obj, const char *part, Edje_Cursor cur, int *x, int *y)
{
   efl_canvas_layout_part_text_cursor_coord_get(obj, part, cur, x, y);
}

EAPI void
edje_object_part_text_cursor_size_get(Efl_Canvas_Layout *obj, const char *part, Edje_Cursor cur, int *w, int *h)
{
   efl_canvas_layout_part_text_cursor_size_get(obj, part, cur, w, h);
}
/*******
 * END *
 *******/

/**********************************************************************************
 * TIZEN_ONLY(20171128): add additional cursor function for improving performance *
 **********************************************************************************/
EAPI void
edje_object_part_text_cursor_on_mouse_geometry_get(const Efl_Canvas_Layout *obj, const char *part, int *x, int *y, int *w, int *h)
{
   efl_canvas_layout_part_text_cursor_on_mouse_geometry_get(obj, part, x, y, w, h);
}
/*******
 * END *
 *******/
