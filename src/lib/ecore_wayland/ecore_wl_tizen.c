// TIZEN_ONLY(20171109): merged with legacy ecore_wayland
#include "ecore_wl_private.h"

/* external variables */
EAPI int ECORE_WL_EVENT_WINDOW_ACTIVATE = 0;
EAPI int ECORE_WL_EVENT_WINDOW_DEACTIVATE = 0;
EAPI int ECORE_WL_EVENT_WINDOW_VISIBILITY_CHANGE = 0;
EAPI int ECORE_WL_EVENT_WINDOW_PRE_VISIBILITY_CHANGE = 0;
EAPI int ECORE_WL_EVENT_WINDOW_SHOW = 0;
EAPI int ECORE_WL_EVENT_WINDOW_HIDE = 0;
EAPI int ECORE_WL_EVENT_WINDOW_LOWER = 0;
EAPI int ECORE_WL_EVENT_WINDOW_ROTATE = 0;
EAPI int ECORE_WL_EVENT_CONFORMANT_CHANG = 0;
EAPI int ECORE_WL_EVENT_AUX_HINT_ALLOWED = 0;
EAPI int ECORE_WL_EVENT_AUX_MESSAGE = 0;
EAPI int ECORE_WL_EVENT_WINDOW_ICONIFY_STATE_CHANGE = 0;
EAPI int ECORE_WL_EVENT_EFFECT_START = 0;
EAPI int ECORE_WL_EVENT_EFFECT_END = 0;
EAPI int ECORE_WL_EVENT_OUTPUT_TRANSFORM = 0;
EAPI int ECORE_WL_EVENT_IGNORE_OUTPUT_TRANSFORM = 0;
EAPI int ECORE_WL_EVENT_GLOBAL_ADDED = 0;
EAPI int ECORE_WL_EVENT_GLOBAL_REMOVED = 0;
EAPI int ECORE_WL_EVENT_KEYMAP_UPDATE = 0;
EAPI int ECORE_WL_EVENT_INDICATOR_FLICK = 0;
EAPI int ECORE_WL_EVENT_CLIPBOARD_DATA_SELECTED = 0;

EAPI Ecore_Wl_Output *
ecore_wl_window_output_find(Ecore_Wl_Window *window EINA_UNUSED)
{
   EINA_LOG_ERR("Deprecated function. Plz use ecore_wl2.");
   return NULL;
}

EAPI int
ecore_wl_output_transform_get(Ecore_Wl_Output *output EINA_UNUSED)
{
   EINA_LOG_ERR("Deprecated function. Plz use ecore_wl2.");
   return -1;
}

EAPI void
ecore_wl_window_lower(Ecore_Wl_Window *win EINA_UNUSED)
{
   EINA_LOG_ERR("Deprecated function. Plz use ecore_wl2.");
}

EAPI void
ecore_wl_window_activate(Ecore_Wl_Window *win EINA_UNUSED)
{
   EINA_LOG_ERR("Deprecated function. Plz use ecore_wl2.");
}

EAPI void
ecore_wl_window_buffer_transform_set(Ecore_Wl_Window *win EINA_UNUSED, int buffer_transform EINA_UNUSED)
{
   EINA_LOG_ERR("Deprecated function. Plz use ecore_wl2.");
}

EAPI Ecore_Wl_Window_Type
ecore_wl_window_type_get(Ecore_Wl_Window *win EINA_UNUSED)
{
   EINA_LOG_ERR("Deprecated function. Plz use ecore_wl2.");
   return ECORE_WL_WINDOW_TYPE_NONE;
}

EAPI Eina_Bool
ecore_wl_window_pointer_warp(Ecore_Wl_Window *win EINA_UNUSED, int x EINA_UNUSED, int y EINA_UNUSED)
{
   EINA_LOG_ERR("Deprecated function. Plz use ecore_wl2.");
   return EINA_FALSE;
}

EAPI void
ecore_wl_window_position_set(Ecore_Wl_Window *win EINA_UNUSED, int x EINA_UNUSED, int y EINA_UNUSED)
{
   EINA_LOG_ERR("Deprecated function. Plz use ecore_wl2.");
}

EAPI void
ecore_wl_window_focus_skip_set(Ecore_Wl_Window *win EINA_UNUSED, Eina_Bool focus_skip EINA_UNUSED)
{
   EINA_LOG_ERR("Deprecated function. Plz use ecore_wl2.");
}

EAPI void
ecore_wl_window_role_set(Ecore_Wl_Window *win EINA_UNUSED, const char *role EINA_UNUSED)
{
   EINA_LOG_ERR("Deprecated function. Plz use ecore_wl2.");
}

EAPI int
ecore_wl_window_active_angle_get(Ecore_Wl_Window *win EINA_UNUSED)
{
   EINA_LOG_ERR("Deprecated function. Plz use ecore_wl2.");
   return -1;
}

EAPI void
ecore_wl_window_iconify_state_update(Ecore_Wl_Window *win EINA_UNUSED, Eina_Bool iconified EINA_UNUSED, Eina_Bool send_event EINA_UNUSED)
{
   EINA_LOG_ERR("Deprecated function. Plz use ecore_wl2.");
}

EAPI void
ecore_wl_window_stack_mode_set(Ecore_Wl_Window *win EINA_UNUSED, Ecore_Wl_Window_Stack_Mode mode EINA_UNUSED)
{
   EINA_LOG_ERR("Deprecated function. Plz use ecore_wl2.");
}

EAPI void
ecore_wl_window_input_rect_set(Ecore_Wl_Window *win EINA_UNUSED, Eina_Rectangle *input_rect EINA_UNUSED)
{
   EINA_LOG_ERR("Deprecated function. Plz use ecore_wl2.");
}

EAPI void
ecore_wl_window_input_rect_add(Ecore_Wl_Window *win EINA_UNUSED, Eina_Rectangle *input_rect EINA_UNUSED)
{
   EINA_LOG_ERR("Deprecated function. Plz use ecore_wl2.");
}

EAPI void
ecore_wl_window_input_rect_subtract(Ecore_Wl_Window *win EINA_UNUSED, Eina_Rectangle *input_rect EINA_UNUSED)
{
   EINA_LOG_ERR("Deprecated function. Plz use ecore_wl2.");
}

EAPI Eina_Bool
ecore_wl_server_mode_get(void)
{
   EINA_LOG_ERR("Deprecated function. Plz use ecore_wl2.");
   return EINA_FALSE;
}

EAPI Eina_Bool
ecore_wl_window_keygrab_set(Ecore_Wl_Window *win EINA_UNUSED, const char *key EINA_UNUSED, int mod EINA_UNUSED, int not_mod EINA_UNUSED, int priority EINA_UNUSED, Ecore_Wl_Window_Keygrab_Mode grab_mode EINA_UNUSED)
{
   EINA_LOG_ERR("Deprecated function. Plz use ecore_wl2.");
   return EINA_FALSE;
}

EAPI Eina_Bool
ecore_wl_window_keygrab_unset(Ecore_Wl_Window *win EINA_UNUSED, const char *key EINA_UNUSED, int mod EINA_UNUSED, int any_mod EINA_UNUSED)
{
   EINA_LOG_ERR("Deprecated function. Plz use ecore_wl2.");
   return EINA_FALSE;
}

EAPI Eina_List *
ecore_wl_window_keygrab_list_set(Ecore_Wl_Window *win EINA_UNUSED, Eina_List *infos EINA_UNUSED)
{
   EINA_LOG_ERR("Deprecated function. Plz use ecore_wl2.");
   return NULL;
}

EAPI Eina_List *
ecore_wl_window_keygrab_list_unset(Ecore_Wl_Window *win EINA_UNUSED, Eina_List *infos EINA_UNUSED)
{
   EINA_LOG_ERR("Deprecated function. Plz use ecore_wl2.");
   return NULL;
}

EAPI void
ecore_wl_window_conformant_set(Ecore_Wl_Window *win EINA_UNUSED, unsigned int is_conformant EINA_UNUSED)
{
   EINA_LOG_ERR("Deprecated function. Plz use ecore_wl2.");
}

EAPI Eina_Bool
ecore_wl_window_conformant_get(Ecore_Wl_Window *win EINA_UNUSED)
{
   EINA_LOG_ERR("Deprecated function. Plz use ecore_wl2.");
   return EINA_FALSE;
}

EAPI void
ecore_wl_window_indicator_geometry_set(Ecore_Wl_Window *win EINA_UNUSED, int x EINA_UNUSED, int y EINA_UNUSED, int w EINA_UNUSED, int h EINA_UNUSED)
{
   EINA_LOG_ERR("Deprecated function. Plz use ecore_wl2.");
}

EAPI Eina_Bool
ecore_wl_window_indicator_geometry_get(Ecore_Wl_Window *win EINA_UNUSED, int *x EINA_UNUSED, int *y EINA_UNUSED, int *w EINA_UNUSED, int *h EINA_UNUSED)
{
   EINA_LOG_ERR("Deprecated function. Plz use ecore_wl2.");
   return EINA_FALSE;
}

EAPI void
ecore_wl_window_indicator_state_set(Ecore_Wl_Window *win EINA_UNUSED, Ecore_Wl_Indicator_State state EINA_UNUSED)
{
   EINA_LOG_ERR("Deprecated function. Plz use ecore_wl2.");
}

EAPI Ecore_Wl_Indicator_State
ecore_wl_window_indicator_state_get(Ecore_Wl_Window *win EINA_UNUSED)
{
   EINA_LOG_ERR("Deprecated function. Plz use ecore_wl2.");
   return ECORE_WL_INDICATOR_STATE_UNKNOWN;
}

EAPI void
ecore_wl_window_indicator_opacity_set(Ecore_Wl_Window *win EINA_UNUSED, Ecore_Wl_Indicator_Opacity_Mode mode EINA_UNUSED)
{
   EINA_LOG_ERR("Deprecated function. Plz use ecore_wl2.");
}

EAPI Ecore_Wl_Indicator_Opacity_Mode
ecore_wl_window_indicator_opacity_get(Ecore_Wl_Window *win EINA_UNUSED)
{
   EINA_LOG_ERR("Deprecated function. Plz use ecore_wl2.");
   return ECORE_WL_INDICATOR_OPACITY_UNKNOWN;
}

EAPI void
ecore_wl_indicator_visible_type_set(Ecore_Wl_Window *win EINA_UNUSED, Ecore_Wl_Indicator_Visible_Type type EINA_UNUSED)
{
   EINA_LOG_ERR("Deprecated function. Plz use ecore_wl2.");
}

EAPI Ecore_Wl_Indicator_Visible_Type
ecore_wl_indicator_visible_type_get(Ecore_Wl_Window *win EINA_UNUSED)
{
   EINA_LOG_ERR("Deprecated function. Plz use ecore_wl2.");
   return ECORE_WL_INDICATOR_VISIBLE_TYPE_HIDDEN;
}

EAPI void
ecore_wl_window_clipboard_geometry_set(Ecore_Wl_Window *win EINA_UNUSED, int x EINA_UNUSED, int y EINA_UNUSED, int w EINA_UNUSED, int h EINA_UNUSED)
{
   EINA_LOG_ERR("Deprecated function. Plz use ecore_wl2.");
}

EAPI Eina_Bool
ecore_wl_window_clipboard_geometry_get(Ecore_Wl_Window *win EINA_UNUSED, int *x EINA_UNUSED, int *y EINA_UNUSED, int *w EINA_UNUSED, int *h EINA_UNUSED)
{
   EINA_LOG_ERR("Deprecated function. Plz use ecore_wl2.");
   return EINA_FALSE;
}

EAPI void
ecore_wl_window_clipboard_state_set(Ecore_Wl_Window *win EINA_UNUSED, Ecore_Wl_Clipboard_State state EINA_UNUSED)
{
   EINA_LOG_ERR("Deprecated function. Plz use ecore_wl2.");
}

EAPI Ecore_Wl_Clipboard_State
ecore_wl_window_clipboard_state_get(Ecore_Wl_Window *win EINA_UNUSED)
{
   EINA_LOG_ERR("Deprecated function. Plz use ecore_wl2.");
   return ECORE_WL_CLIPBOARD_STATE_UNKNOWN;
}

EAPI void
ecore_wl_clipboard_show(Ecore_Wl_Window *win EINA_UNUSED)
{
   EINA_LOG_ERR("Deprecated function. Plz use ecore_wl2.");
}

EAPI void
ecore_wl_clipboard_hide(Ecore_Wl_Window *win EINA_UNUSED)
{
   EINA_LOG_ERR("Deprecated function. Plz use ecore_wl2.");
}

EAPI Eina_Bool
ecore_wl_clipboard_data_only_set(Eina_Bool data_only EINA_UNUSED)
{
   EINA_LOG_ERR("Deprecated function. Plz use ecore_wl2.");
   return EINA_FALSE;
}

EAPI void
ecore_wl_window_keyboard_geometry_set(Ecore_Wl_Window *win EINA_UNUSED, int x EINA_UNUSED, int y EINA_UNUSED, int w EINA_UNUSED, int h EINA_UNUSED)
{
   EINA_LOG_ERR("Deprecated function. Plz use ecore_wl2.");
}

EAPI Eina_Bool
ecore_wl_window_keyboard_geometry_get(Ecore_Wl_Window *win EINA_UNUSED, int *x EINA_UNUSED, int *y EINA_UNUSED, int *w EINA_UNUSED, int *h EINA_UNUSED)
{
   EINA_LOG_ERR("Deprecated function. Plz use ecore_wl2.");
   return EINA_FALSE;
}

EAPI void
ecore_wl_window_keyboard_state_set(Ecore_Wl_Window *win EINA_UNUSED, Ecore_Wl_Virtual_Keyboard_State state EINA_UNUSED)
{
   EINA_LOG_ERR("Deprecated function. Plz use ecore_wl2.");
}

EAPI Ecore_Wl_Virtual_Keyboard_State
ecore_wl_window_keyboard_state_get(Ecore_Wl_Window *win EINA_UNUSED)
{
   EINA_LOG_ERR("Deprecated function. Plz use ecore_wl2.");
   return ECORE_WL_VIRTUAL_KEYBOARD_STATE_UNKNOWN;
}

EAPI void
ecore_wl_window_rotation_preferred_rotation_set(Ecore_Wl_Window *win EINA_UNUSED, int rot EINA_UNUSED)
{
   EINA_LOG_ERR("Deprecated function. Plz use ecore_wl2.");
}

EAPI void
ecore_wl_window_rotation_available_rotations_set(Ecore_Wl_Window *win EINA_UNUSED, const int *rots EINA_UNUSED, unsigned int count EINA_UNUSED)
{
   EINA_LOG_ERR("Deprecated function. Plz use ecore_wl2.");
}

EAPI void
ecore_wl_window_rotation_change_done_send(Ecore_Wl_Window *win EINA_UNUSED)
{
   EINA_LOG_ERR("Deprecated function. Plz use ecore_wl2.");
}

EAPI void
ecore_wl_window_rotation_geometry_set(Ecore_Wl_Window *win EINA_UNUSED, int rot EINA_UNUSED, int x EINA_UNUSED, int y EINA_UNUSED, int w EINA_UNUSED, int h EINA_UNUSED)
{
   EINA_LOG_ERR("Deprecated function. Plz use ecore_wl2.");
}

EAPI void
ecore_wl_window_rotation_changed_callback_set(Ecore_Wl_Window *win EINA_UNUSED, void *data EINA_UNUSED, void (*func)(Ecore_Wl_Window *win, int rot, Eina_Bool resize, int w, int h, void *data) EINA_UNUSED)
{
   EINA_LOG_ERR("Deprecated function. Plz use ecore_wl2.");
}

EAPI Eina_List *
ecore_wl_window_aux_hints_supported_get(Ecore_Wl_Window *win EINA_UNUSED)
{
   EINA_LOG_ERR("Deprecated function. Plz use ecore_wl2.");
   return NULL;
}

EAPI void
ecore_wl_window_aux_hint_add(Ecore_Wl_Window *win EINA_UNUSED, int id EINA_UNUSED, const char *hint EINA_UNUSED, const char *val EINA_UNUSED)
{
   EINA_LOG_ERR("Deprecated function. Plz use ecore_wl2.");
}

EAPI void
ecore_wl_window_aux_hint_change(Ecore_Wl_Window *win EINA_UNUSED, int id EINA_UNUSED, const char *val EINA_UNUSED)
{
   EINA_LOG_ERR("Deprecated function. Plz use ecore_wl2.");
}

EAPI void
ecore_wl_window_aux_hint_del(Ecore_Wl_Window *win EINA_UNUSED, int id EINA_UNUSED)
{
   EINA_LOG_ERR("Deprecated function. Plz use ecore_wl2.");
}

EAPI void
ecore_wl_window_floating_mode_set(Ecore_Wl_Window *win EINA_UNUSED, Eina_Bool floating EINA_UNUSED)
{
   EINA_LOG_ERR("Deprecated function. Plz use ecore_wl2.");
}

EAPI void
ecore_wl_window_geometry_get(Ecore_Wl_Window *win EINA_UNUSED, int *x EINA_UNUSED, int *y EINA_UNUSED, int *w EINA_UNUSED, int *h EINA_UNUSED)
{
   EINA_LOG_ERR("Deprecated function. Plz use ecore_wl2.");
}

EAPI Ecore_Wl_Input *
ecore_wl_window_input_get(Ecore_Wl_Window *win EINA_UNUSED)
{
   EINA_LOG_ERR("Deprecated function. Plz use ecore_wl2.");
   return NULL;
}

EAPI Eina_Bool
ecore_wl_window_ignore_output_transform_get(Ecore_Wl_Window *win EINA_UNUSED)
{
   EINA_LOG_ERR("Deprecated function. Plz use ecore_wl2.");
   return EINA_FALSE;
}

EAPI void
ecore_wl_window_video_has(Ecore_Wl_Window *win EINA_UNUSED, Eina_Bool has EINA_UNUSED)
{
   EINA_LOG_ERR("Deprecated function. Plz use ecore_wl2.");
}
// End of TIZEN_ONLY(20171109)
