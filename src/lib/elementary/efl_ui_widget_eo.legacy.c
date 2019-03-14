
//TIZEN_ONLY: Missing method from opensource
EAPI Eina_Bool
elm_widget_cursor_set(Efl_Ui_Widget *obj, const char *cursor)
{
   return efl_ui_widget_cursor_set(obj, cursor);
}

EAPI const char *
elm_widget_cursor_get(const Efl_Ui_Widget *obj)
{
   return efl_ui_widget_cursor_get(obj);
}

EAPI Eina_Bool
elm_widget_cursor_style_set(Efl_Ui_Widget *obj, const char *style)
{
   return efl_ui_widget_cursor_style_set(obj, style);
}

EAPI const char *
elm_widget_cursor_style_get(const Efl_Ui_Widget *obj)
{
   return efl_ui_widget_cursor_style_get(obj);
}

EAPI Eina_Bool
elm_widget_cursor_theme_search_enabled_set(Efl_Ui_Widget *obj, Eina_Bool allow)
{
   return efl_ui_widget_cursor_theme_search_enabled_set(obj, allow);
}

EAPI Eina_Bool
elm_widget_cursor_theme_search_enabled_get(const Efl_Ui_Widget *obj)
{
   return efl_ui_widget_cursor_theme_search_enabled_get(obj);
}
//

EAPI void
elm_widget_resize_object_set(Efl_Ui_Widget *obj, Efl_Canvas_Object *sobj)
{
   efl_ui_widget_resize_object_set(obj, sobj);
}

EAPI void
elm_widget_disabled_set(Efl_Ui_Widget *obj, Eina_Bool disabled)
{
   efl_ui_widget_disabled_set(obj, disabled);
}

EAPI Eina_Bool
elm_widget_disabled_get(const Efl_Ui_Widget *obj)
{
   return efl_ui_widget_disabled_get(obj);
}

EAPI Eina_Error
elm_widget_style_set(Efl_Ui_Widget *obj, const char *style)
{
   return efl_ui_widget_style_set(obj, style);
}

EAPI const char *
elm_widget_style_get(const Efl_Ui_Widget *obj)
{
   return efl_ui_widget_style_get(obj);
}

EAPI void
elm_widget_can_focus_set(Efl_Ui_Widget *obj, Eina_Bool can_focus)
{
   efl_ui_widget_focus_allow_set(obj, can_focus);
}

EAPI Eina_Bool
elm_widget_can_focus_get(const Efl_Ui_Widget *obj)
{
   return efl_ui_widget_focus_allow_get(obj);
}

EAPI void
elm_widget_parent_set(Efl_Ui_Widget *obj, Efl_Ui_Widget *parent)
{
   efl_ui_widget_parent_set(obj, parent);
}

EAPI Efl_Ui_Widget *
elm_widget_parent_get(const Efl_Ui_Widget *obj)
{
   return efl_ui_widget_parent_get(obj);
}

EAPI Eina_Bool
elm_widget_sub_object_add(Efl_Ui_Widget *obj, Efl_Canvas_Object *sub_obj)
{
   return efl_ui_widget_sub_object_add(obj, sub_obj);
}

EAPI Eina_Bool
elm_widget_sub_object_del(Efl_Ui_Widget *obj, Efl_Canvas_Object *sub_obj)
{
   return efl_ui_widget_sub_object_del(obj, sub_obj);
}

EAPI Eina_Error
elm_widget_theme_apply(Efl_Ui_Widget *obj)
{
   return efl_ui_widget_theme_apply(obj);
}

EAPI Eina_Rect
elm_widget_focus_region_get(const Efl_Ui_Widget *obj)
{
   return efl_ui_widget_interest_region_get(obj);
}

EAPI Eina_Rect
elm_widget_focus_highlight_geometry_get(const Efl_Ui_Widget *obj)
{
   return efl_ui_widget_focus_highlight_geometry_get(obj);
}

//TIZEN_ONLY(20160726): add API elm_object_part_access_object_get
EAPI Efl_Canvas_Object *
elm_widget_part_access_object_get(const Efl_Ui_Widget *obj, const char *part)
{
   return efl_ui_widget_part_access_object_get(obj, part);
//
}

//TIZEN_ONLY(20180607): Restore legacy focus
EAPI Efl_Canvas_Object *
elm_widget_newest_focus_order_get(const Efl_Ui_Widget *obj, unsigned int *newest_focus_order, Eina_Bool can_focus_only)
{
   return efl_ui_widget_newest_focus_order_get(obj, newest_focus_order, can_focus_only);
}

EAPI void
elm_widget_focus_tree_unfocusable_handle(Efl_Ui_Widget *obj)
{
   efl_ui_widget_focus_tree_unfocusable_handle(obj);
}

EAPI Eina_Bool
elm_widget_focus_list_direction_get(const Efl_Ui_Widget *obj, const Efl_Canvas_Object *base, const Eina_List *items, void *list_data_get, double degree, Efl_Canvas_Object **direction, Elm_Widget_Item **direction_item, double *weight)
{
   return efl_ui_widget_focus_list_direction_get(obj, base, items, list_data_get, degree, direction, direction_item, weight);
}

EAPI Eina_Bool
elm_widget_focus_list_next_get(const Efl_Ui_Widget *obj, const Eina_List *items, void *list_data_get, Efl_Ui_Focus_Direction dir, Efl_Canvas_Object **next, Elm_Widget_Item **next_item)
{
   return efl_ui_widget_focus_list_next_get(obj, items, list_data_get, dir, next, next_item);
}

EAPI Eina_Bool
elm_widget_focus_state_apply(Efl_Ui_Widget *obj, Efl_Ui_Widget_Focus_State current_state, Efl_Ui_Widget_Focus_State *configured_state, Efl_Ui_Widget *redirect)
{
   return efl_ui_widget_focus_state_apply(obj, current_state, configured_state, redirect);
}
//
