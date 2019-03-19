#ifdef HAVE_CONFIG_H
# include "elementary_config.h"
#endif

#define EFL_ACCESS_OBJECT_PROTECTED
#define EFL_ACCESS_COMPONENT_PROTECTED
#define EFL_ACCESS_WIDGET_ACTION_PROTECTED
#define ELM_WIDGET_PROTECTED
#define ELM_WIDGET_ITEM_PROTECTED
#define EFL_INPUT_EVENT_PROTECTED
#define EFL_UI_L10N_PROTECTED
#define EFL_UI_FOCUS_OBJECT_PROTECTED
#define EFL_UI_WIDGET_PART_BG_PROTECTED
#define EFL_PART_PROTECTED

#include <Elementary.h>

#include "elm_priv.h"
#include "elm_widget_container.h"
#include "elm_interface_scrollable.h"
#include "elm_pan_eo.h"
#include "elm_part_helper.h"
#include "elm_widget_combobox.h"

/* FIXME: remove this when we don't rely on evas event structs anymore */
#define EFL_INTERNAL_UNSTABLE
#include "interfaces/efl_common_internal.h"

#define MY_CLASS EFL_UI_WIDGET_CLASS

#define MY_CLASS_NAME "Efl_Ui_Widget"
#define MY_CLASS_NAME_LEGACY "elm_widget"

#define API_ENTRY                                    \
  Elm_Widget_Smart_Data *sd = NULL;                  \
  if (!_elm_widget_is(obj) ||                        \
      (!(sd = efl_data_scope_get(obj, MY_CLASS))))

#define INTERNAL_ENTRY                               \
  ELM_WIDGET_DATA_GET(obj, sd);                      \
  if (!sd) return

//TIZEN_ONLY(20180607): Restore legacy focus
/*
#define ELM_WIDGET_FOCUS_GET(obj)                                          \
  (efl_isa(obj, EFL_UI_WIDGET_CLASS) &&                                    \
   ((_elm_access_auto_highlight_get()) ? (elm_widget_highlight_get(obj)) : \
                                         (efl_ui_focus_object_focus_get(obj))))
*/
#define ELM_WIDGET_FOCUS_GET(obj)                                          \
  (efl_isa(obj, EFL_UI_WIDGET_CLASS) &&                                    \
   ((_elm_access_auto_highlight_get()) ? (elm_widget_highlight_get(obj)) : \
                                         (elm_widget_is_legacy(obj) ? elm_widget_focus_get(obj) : efl_ui_focus_object_focus_get(obj))))
//
const char SIG_WIDGET_FOCUSED[] = "focused";
const char SIG_WIDGET_UNFOCUSED[] = "unfocused";
const char SIG_WIDGET_LANG_CHANGED[] = "language,changed";
const char SIG_WIDGET_ACCESS_CHANGED[] = "access,changed";

// TIZEN_ONLY(20161018): add highlighted/unhighlighted signal for atspi
const char SIG_WIDGET_ATSPI_HIGHLIGHTED[] = "atspi,highlighted";
const char SIG_WIDGET_ATSPI_UNHIGHLIGHTED[] = "atspi,unhighlighted";
//
typedef struct _Elm_Event_Cb_Data         Elm_Event_Cb_Data;
typedef struct _Elm_Label_Data            Elm_Label_Data;
typedef struct _Elm_Translate_String_Data Elm_Translate_String_Data;

struct _Elm_Event_Cb_Data
{
   Elm_Event_Cb func;
   const void  *data;
};

struct _Elm_Label_Data
{
   const char *part;
   const char *text;
};

struct _Elm_Translate_String_Data
{
   EINA_INLIST;
   Eina_Stringshare *id;
   Eina_Stringshare *domain;
   Eina_Stringshare *string;
   Eina_Bool   preset : 1;
};

// TIZEN_ONLY PROTOTYPES
static void _efl_ui_widget_efl_canvas_object_paragraph_direction_set_internal(Eo *obj EINA_UNUSED, Efl_Ui_Widget_Data *sd, Efl_Text_Bidirectional_Type dir);  // TIZEN_ONLY(20180117): Override Paragraph Direction APIs
static void _if_focused_revert(Evas_Object *obj, Eina_Bool can_focus_only); //TIZEN_ONLY(20180607): Restore legacy focus

static void _on_sub_obj_hide(void *data, const Efl_Event *event);

static inline Eina_Bool _elm_widget_focus_chain_manager_is(const Evas_Object *obj);
static inline Eina_Bool _internal_elm_widget_focus_direction_manager_is(const Evas_Object *obj);
static void _parent_focus(Evas_Object *obj, Elm_Object_Item *item);
static void _elm_object_focus_chain_del_cb(void *data, Evas *e EINA_UNUSED, Evas_Object *obj, void *event_info EINA_UNUSED);

static Eina_List *_lines_split(Eina_List *children);
static int _sort_vertically(const void *data1, const void *data2);
static int _sort_horizontally(const void *data1, const void *data2);

Eo * plug_type_proxy_get(const Eo *obj, Evas_Object *widget);

static Eina_Bool _elm_widget_can_highlight_get_by_class(Eo *obj);
static Eina_Bool _accessible_object_on_scroll_is(Eo* obj);
// TIZEN_ONLY




/* For keeping backward compatibility (EFL 1.18 or older versions).
 * Since EFL 1.19 which starts to use eolian_gen2, it does not convert
 * "." to "_" among the class name. */
static const char *legacy_type_table[][2] =
{
     { "Efl.Ui.Bg_Legacy", "Elm_Bg" },
     { "Efl.Ui.Button_Legacy", "Elm_Button" },
     { "Efl.Ui.Check_Legacy", "Elm_Check" },
     { "Efl.Ui.Clock_Legacy", "Elm_Datetime" },
     { "Efl.Ui.Flip_Legacy", "Elm_Flip" },
     { "Efl.Ui.Frame_Legacy", "Elm_Frame" },
     { "Efl.Ui.Image_Legacy", "Elm_Image" },
     { "Efl.Ui.Image_Zoomable_Legacy", "Elm_Photocam" },
     { "Efl.Ui.Layout_Legacy", "Elm_Layout" },
     { "Efl.Ui.Panes_Legacy", "Elm_Panes" },
     { "Efl.Ui.Progressbar_Legacy", "Elm_Progressbar" },
     { "Efl.Ui.Radio_Legacy", "Elm_Radio" },
     { "Efl.Ui.Video_Legacy", "Elm_Video" },
     { "Efl.Ui.Win_Legacy", "Elm_Win" },
     { "Efl.Ui.Win_Socket_Legacy", "Elm_Win" },
     { "Efl.Ui.Win_Inlined_Legacy", "Elm_Win" },
     { "Elm.Code_Widget_Legacy", "Elm_Code_Widget" },
     { "Elm.Ctxpopup", "Elm_Ctxpopup" },
     { "Elm.Entry", "Elm_Entry" },
     { "Elm.Colorselector", "Elm_Colorselector" },
     { "Elm.List", "Elm_List" },
     { "Elm.Photo", "Elm_Photo" },
     { "Elm.Actionslider", "Elm_Actionslider" },
     { "Elm.Box", "Elm_Box" },
     { "Elm.Table", "Elm_Table" },
     { "Elm.Thumb", "Elm_Thumb" },
     { "Elm.Menu", "Elm_Menu" },
     { "Elm.Icon", "Elm_Icon" },
     { "Elm.Prefs", "Elm_Prefs" },
     { "Elm.Map", "Elm_Map" },
     { "Elm.Glview", "Elm_Glview" },
     { "Elm.Web", "Elm_Web" },
     { "Elm.Toolbar", "Elm_Toolbar" },
     { "Elm.Grid", "Elm_Grid" },
     { "Elm.Diskselector", "Elm_Diskselector" },
     { "Elm.Notify", "Elm_Notify" },
     { "Elm.Mapbuf", "Elm_Mapbuf" },
     { "Elm.Separator", "Elm_Separator" },
     { "Elm.Calendar", "Elm_Calendar" },
     { "Elm.Inwin", "Elm_Inwin" },
     { "Elm.Gengrid", "Elm_Gengrid" },
     { "Elm.Scroller", "Elm_Scroller" },
     { "Elm.Player", "Elm_Player" },
     { "Elm.Segment_Control", "Elm_Segment_Control" },
     { "Elm.Fileselector", "Elm_Fileselector" },
     { "Elm.Fileselector_Button", "Elm_Fileselector_Button" },
     { "Elm.Fileselector_Entry", "Elm_Fileselector_Entry" },
     { "Elm.Flipselector", "Elm_Flipselector" },
     { "Elm.Hoversel", "Elm_Hoversel" },
     { "Elm.Naviframe", "Elm_Naviframe" },
     { "Elm.Popup", "Elm_Popup" },
     { "Elm.Bubble", "Elm_Bubble" },
     { "Elm.Clock", "Elm_Clock" },
     { "Elm.Conformant", "Elm_Conformant" },
     { "Elm.Dayselector", "Elm_Dayselector" },
     { "Elm.Genlist", "Elm_Genlist" },
     { "Elm.Hover", "Elm_Hover" },
     { "Elm.Index", "Elm_Index" },
     { "Elm.Label", "Elm_Label" },
     { "Elm.Panel", "Elm_Panel" },
     { "Elm.Slider", "Elm_Slider" },
     { "Elm.Slideshow", "Elm_Slideshow" },
     { "Elm.Spinner", "Elm_Spinner" },
     { "Elm.Plug", "Elm_Plug" },
     { "Elm.Web.None", "Elm_Web" },
     { "Elm.Multibuttonentry", "Elm_Multibuttonentry" },
     /* TIZEN_ONLY(20180423): add additional class names for legacy API calls */
     { "Efl.Ui.Bg", "Elm_Bg" },
     { "Efl.Ui.Button", "Elm_Button" },
     { "Efl.Ui.Check", "Elm_Check" },
     { "Efl.Ui.Clock", "Elm_Datetime" },
     { "Efl.Ui.Flip", "Elm_Flip" },
     { "Efl.Ui.Frame", "Elm_Frame" },
     { "Efl.Ui.Image", "Elm_Image" },
     { "Efl.Ui.Image_Zoomable", "Elm_Photocam" },
     { "Efl.Ui.Layout", "Elm_Layout" },
     { "Efl.Ui.Multibuttonentry", "Elm_Multibuttonentry" },
     { "Efl.Ui.Panes", "Elm_Panes" },
     { "Efl.Ui.Progressbar", "Elm_Progressbar" },
     { "Efl.Ui.Radio", "Elm_Radio" },
     { "Efl.Ui.Slider", "Elm_Slider" },
     { "Efl.Ui.Video", "Elm_Video" },
     { "Efl.Ui.Win", "Elm_Win" },
     { "Elm.Code_Widget", "Elm_Code_Widget" },
     { "Elm.Gesture_Layer", "Elm_Gesture_Layer" },
     /* END */
     /* TIZEN_ONLY(20180504): add missing item class names and fix edje_class parse rule for legacy */
     { "Elm.Naviframe.Item", "Elm_Naviframe_Item" },
     { "Elm.Genlist.Item", "Elm_Genlist_Item" },
     { "Elm.Gengrid.Item", "Elm_Gengrid_Item" },
     { "Elm.Toolbar_Item", "Elm_Toolbar_Item" },
     { "Elm.Multibuttonentry_Item", "Elm_Multibuttonentry_Item" },
     { "Elm.Ctxpopup.Item", "Elm_Ctxpopup_Item" },
     { "Elm.Hoversel.Item", "Elm_Hoversel_Item" },
     { "Elm.Index.Item", "Elm_Index_Item" },
     { "Elm.Popup.Item", "Elm_Popup_Item" },
     { "Elm.List.Item", "Elm_List_Item" },
     { "Elm.Color.Item", "Elm_Color_Item" },
     /* END */
     { NULL, NULL }
};

//TIZEN_ONLY(20180607): Restore legacy focus
static unsigned int focus_order = 0;
//

/* local subsystem globals */
static inline Eina_Bool
_elm_widget_is(const Evas_Object *obj)
{
   return efl_isa(obj, MY_CLASS);
}

static inline Eina_Bool
_is_focusable(Evas_Object *obj)
{
   API_ENTRY return EINA_FALSE;
//TIZEN_ONLY(20190312): Restore child_can_focus
   //return sd->can_focus || (sd->logical.child_count > 0);
   return sd->can_focus || (sd->child_can_focus);
//
}

static inline Eina_Bool
_is_focused(Evas_Object *obj)
{
   API_ENTRY return EINA_FALSE;
   return sd->focused;
}

static inline Eina_Bool
_elm_scrollable_is(const Evas_Object *obj)
{
   INTERNAL_ENTRY EINA_FALSE;
   if (elm_widget_is_legacy(obj))
     return
        efl_isa(obj, ELM_INTERFACE_SCROLLABLE_MIXIN);
   else
     return
        efl_isa(obj, EFL_UI_SCROLLABLE_INTERACTIVE_INTERFACE);
}

static void
_on_sub_obj_del(void *data, const Efl_Event *event);
static void _propagate_event(void *data, const Efl_Event *eo_event);
static void _elm_widget_focus_tree_unfocusable_handle(Eo *obj);
static void _elm_widget_shadow_update(Efl_Ui_Widget *obj);

EFL_CALLBACKS_ARRAY_DEFINE(elm_widget_subitems_callbacks,
                          //TIZEN_ONLY(20180607): Restore legacy focus
                          //{ EFL_EVENT_DEL, _on_sub_obj_del });
                          { EFL_EVENT_DEL, _on_sub_obj_del },
                          { EFL_GFX_ENTITY_EVENT_VISIBILITY_CHANGED, _on_sub_obj_hide });
                          //
EFL_CALLBACKS_ARRAY_DEFINE(efl_subitems_callbacks,
                          { EFL_EVENT_DEL, _on_sub_obj_del });
EFL_CALLBACKS_ARRAY_DEFINE(focus_callbacks,
                          { EFL_EVENT_KEY_DOWN, _propagate_event },
                          { EFL_EVENT_KEY_UP, _propagate_event },
                          { EFL_EVENT_POINTER_WHEEL, _propagate_event });

static inline void
_callbacks_add(Eo *widget, void *data)
{
   if (_elm_widget_is(widget))
     {
        efl_event_callback_array_add(widget, elm_widget_subitems_callbacks(), data);
     }
   else
     {
        efl_event_callback_array_add(widget, efl_subitems_callbacks(), data);
     }
}

static inline void
_callbacks_del(Eo *widget, void *data)
{
   if (_elm_widget_is(widget))
     {
        efl_event_callback_array_del(widget, elm_widget_subitems_callbacks(), data);
     }
   else
     {
        efl_event_callback_array_del(widget, efl_subitems_callbacks(), data);
     }
}

void
_elm_widget_item_highlight_in_theme(Evas_Object *obj, Elm_Object_Item *eo_it)
{
   const char *str;

   if (!eo_it) return;
   if (efl_isa(eo_it, ELM_WIDGET_ITEM_CLASS))
     {
        Elm_Widget_Item_Data *it = efl_data_scope_get(eo_it, ELM_WIDGET_ITEM_CLASS);

        if (efl_isa(it->view, EFL_UI_LAYOUT_BASE_CLASS))
          str = edje_object_data_get(elm_layout_edje_get(it->view), "focus_highlight");
        else
          str = edje_object_data_get(it->view, "focus_highlight");
     }
   else
      str = edje_object_data_get(((Elm_Widget_Item_Data *)eo_it)->view, "focus_highlight");
   if ((str) && (!strcmp(str, "on")))
     elm_widget_highlight_in_theme_set(obj, EINA_TRUE);
   else
     elm_widget_highlight_in_theme_set(obj, EINA_FALSE);
}

void
_elm_widget_focus_highlight_start(const Evas_Object *obj)
{
   Evas_Object *top = elm_widget_top_get(obj);

   if (top && efl_isa(top, EFL_UI_WIN_CLASS))
     _elm_win_focus_highlight_start(top);
}

Evas_Object *
_efl_ui_widget_focus_highlight_object_get(const Evas_Object *obj)
{
   Evas_Object *top = elm_widget_top_get(obj);

   if (top && efl_isa(top, EFL_UI_WIN_CLASS))
     return _elm_win_focus_highlight_object_get(top);
   return NULL;
}

static Eina_Bool
_candidacy_exam(Eo *obj)
{
   Eina_List *lst;
   Efl_Ui_Widget *wid = obj, *top;
   Elm_Widget_Smart_Data *wid_pd;

   wid_pd = efl_data_scope_get(wid, MY_CLASS);
   do {

     if (wid_pd->disabled) return EINA_TRUE;
     if (wid_pd->tree_unfocusable) return EINA_TRUE;
     top = wid;

     wid = elm_widget_parent_get(wid);
     if (!wid) break;
     wid_pd = efl_data_scope_get(wid, MY_CLASS);

     lst = wid_pd->legacy_focus.custom_chain;
     if (lst)
       {
          if (!eina_list_data_find(lst, top))
            {
               WRN("Widget %p disabled due to custom chain of %p", top, wid);
               return EINA_TRUE;
            }
       }

   } while (1);

   return !efl_isa(top, EFL_UI_WIN_CLASS);
}

static void _full_eval(Eo *obj, Elm_Widget_Smart_Data *pd);

static void
_manager_changed_cb(void *data, const Efl_Event *event EINA_UNUSED)
{
   ELM_WIDGET_DATA_GET(data, pd);

   _full_eval(data, pd);
}

static Efl_Ui_Focus_Object*
_focus_manager_eval(Eo *obj, Elm_Widget_Smart_Data *pd)
{
   Evas_Object *provider = NULL;
   Evas_Object *parent;
   Efl_Ui_Focus_Manager *new = NULL, *old = NULL;

   parent = elm_widget_parent_get(obj);
   if (efl_isa(parent, EFL_UI_FOCUS_MANAGER_INTERFACE))
     {
        new = parent;
     }
   else if (parent)
     {
        new = efl_ui_focus_object_focus_manager_get(parent);
        provider = parent;
     }

   if (new != pd->manager.manager )
     {
        old = pd->manager.manager;

        if (pd->manager.provider)
          efl_event_callback_del(pd->manager.provider, EFL_UI_FOCUS_OBJECT_EVENT_FOCUS_MANAGER_CHANGED, _manager_changed_cb, obj);

        pd->manager.manager = new;
        pd->manager.provider = provider;

        if (pd->manager.provider)
          efl_event_callback_add(pd->manager.provider, EFL_UI_FOCUS_OBJECT_EVENT_FOCUS_MANAGER_CHANGED, _manager_changed_cb, obj);
     }

   return old;
}

EOLIAN static Eina_Bool
_efl_ui_widget_focus_state_apply(Eo *obj, Elm_Widget_Smart_Data *pd EINA_UNUSED, Efl_Ui_Widget_Focus_State current_state, Efl_Ui_Widget_Focus_State *configured_state, Efl_Ui_Widget *redirect)
{
   Eina_Bool registered = EINA_TRUE;

   //shortcut for having the same configurations
   if (current_state.manager == configured_state->manager && !current_state.manager)
     return !!current_state.manager;

   if (configured_state->logical == current_state.logical &&
       configured_state->manager == current_state.manager &&
       configured_state->parent == current_state.parent)
     return !!current_state.manager;

   //this thing doesnt want to be registered, but it is ...
   if (!configured_state->manager && current_state.manager)
     {
        efl_ui_focus_manager_calc_unregister(current_state.manager, obj);
        return EINA_FALSE;
     }
   //by that point we have always a configured manager

   if (!current_state.manager) registered = EINA_FALSE;

   if ((//check if we have changed the manager
        (current_state.manager != configured_state->manager) ||
        //check if we are already registered but in a different state
        (current_state.logical != configured_state->logical))
       && registered)
     {
        //we need to unregister here
        efl_ui_focus_manager_calc_unregister(current_state.manager, obj);
        registered = EINA_FALSE;
     }

   //the parent may has changed
   if (current_state.parent != configured_state->parent && registered)
     {
        return efl_ui_focus_manager_calc_update_parent(current_state.manager, obj, configured_state->parent);
     }

   if (!registered)
     {
        if (configured_state->logical)
          return efl_ui_focus_manager_calc_register_logical(configured_state->manager, obj, configured_state->parent, redirect);
        else
          return efl_ui_focus_manager_calc_register(configured_state->manager, obj, configured_state->parent, redirect);
     }
   ERR("Uncaught focus state consider this as unregistered (%d) \n (%p,%p,%d) \n (%p,%p,%d) ", registered,
     configured_state->manager, configured_state->parent, configured_state->logical,
     current_state.manager, current_state.parent, current_state.logical
   );
   return EINA_FALSE;
}
static void
_eval_registration_candidate(Eo *obj, Elm_Widget_Smart_Data *pd, Eina_Bool *should, Eina_Bool *want_full)
{
   *should = *want_full = EINA_FALSE;

    //can focus can be overridden by the following properties
    if ((!pd->parent_obj) ||
        (!evas_object_visible_get(obj)) ||
        (_candidacy_exam(obj)))
      return;

    if (pd->can_focus)
      {
         *should = *want_full = EINA_TRUE;
      }
    else if (pd->logical.child_count > 0)
      {
         *should = EINA_TRUE;
      }
}

static void
_focus_state_eval(Eo *obj, Elm_Widget_Smart_Data *pd, Eina_Bool should, Eina_Bool want_full)
{
   Efl_Ui_Widget_Focus_State configuration;

   //this would mean we are registering again the root, we dont want that
   if (pd->manager.manager == obj) return;

   //there are two reasons to be registered, the child count is bigger than 0, or the widget is flagged to be able to handle focus
   if (should)
     {
        configuration.parent = pd->logical.parent;
        configuration.manager = pd->manager.manager;
        configuration.logical = !want_full;
     }
   else
     {
        configuration.parent = NULL;
        configuration.manager = NULL;
        configuration.logical = EINA_FALSE;
     }

   if (!efl_ui_widget_focus_state_apply(obj, pd->focus, &configuration, NULL))
     {
        //things went wrong or this thing is unregistered. Purge the current configuration.
        pd->focus.manager = NULL;
        pd->focus.parent = NULL;
        pd->focus.logical = EINA_FALSE;
     }
   else
     {
        pd->focus.parent = configuration.parent;
        pd->focus.manager = configuration.manager;
        pd->focus.logical = configuration.logical;
     }

}

static Efl_Ui_Focus_Object*
_logical_parent_eval(Eo *obj EINA_UNUSED, Elm_Widget_Smart_Data *pd, Eina_Bool should)
{
   //TIZEN_ONLY(20180607): Restore legacy focus
   if (elm_widget_is_legacy(obj))
     return NULL;
   //
   Efl_Ui_Widget *parent;
   Efl_Ui_Focus_Parent_Provider *provider;

   if (should)
     {
        provider = efl_provider_find(obj, EFL_UI_FOCUS_PARENT_PROVIDER_INTERFACE);
        EINA_SAFETY_ON_NULL_RETURN_VAL(provider, NULL);
        parent = efl_ui_focus_parent_provider_find_logical_parent(provider, obj);
     }
   else
     parent = NULL;


   if (pd->logical.parent != parent)
     {
        Efl_Ui_Focus_Object *old = NULL;

        //update old logical parent;
        if (pd->logical.parent)
          {
             if (efl_isa(pd->logical.parent, EFL_UI_WIDGET_CLASS))
               {
                  ELM_WIDGET_DATA_GET(pd->logical.parent, logical_wd);
                  if (!logical_wd)
                    {
                       ERR("Widget parent has the wrong type!");
                       return NULL;
                    }
                  logical_wd->logical.child_count --;
               }
             old = pd->logical.parent;
             efl_weak_unref(&pd->logical.parent);
             pd->logical.parent = NULL;
          }
        if (parent)
          {
             if (efl_isa(parent, EFL_UI_WIDGET_CLASS))
               {
                  ELM_WIDGET_DATA_GET(parent, parent_wd);
                  if (!parent_wd)
                    {
                       ERR("Widget parent has the wrong type!");
                       return NULL;
                    }
                  parent_wd->logical.child_count ++;
               }
             pd->logical.parent = parent;
             efl_weak_ref(&pd->logical.parent);
          }
        return old;
     }
   return NULL;
}

static void
_full_eval(Eo *obj, Elm_Widget_Smart_Data *pd)
{
   //TIZEN_ONLY(20180607): Restore legacy focus
   if (elm_widget_is_legacy(obj))
     return;
   //
   Efl_Ui_Focus_Object *old_parent;
   Efl_Ui_Focus_Object *old_registered_parent, *old_registered_manager;
   Eina_Bool should, want_full;


   _eval_registration_candidate(obj, pd, &should, &want_full);

   old_parent = _logical_parent_eval(obj, pd, should);

   if (efl_isa(old_parent, EFL_UI_WIDGET_CLASS))
     {
        //emit signal and focus eval old and new
        ELM_WIDGET_DATA_GET(old_parent, old_pd);
        _full_eval(old_parent, old_pd);
     }

   if (efl_isa(pd->logical.parent, EFL_UI_WIDGET_CLASS))
     {
        ELM_WIDGET_DATA_GET(pd->logical.parent, new_pd);
        _full_eval(pd->logical.parent, new_pd);
     }

   _focus_manager_eval(obj, pd);

   old_registered_parent = pd->focus.parent;
   old_registered_manager = pd->focus.manager;

   _focus_state_eval(obj, pd, should, want_full);

   if (old_registered_parent != pd->focus.parent)
     {
        efl_event_callback_call(obj,
             EFL_UI_FOCUS_OBJECT_EVENT_FOCUS_PARENT_CHANGED, old_registered_parent);
     }

   if (old_registered_manager != pd->focus.manager)
     {
        efl_event_callback_call(obj,
             EFL_UI_FOCUS_OBJECT_EVENT_FOCUS_MANAGER_CHANGED, old_registered_manager);
     }

}

void
_elm_widget_full_eval(Eo *obj)
{
   ELM_WIDGET_DATA_GET(obj, pd);

   _full_eval(obj, pd);
}

/**
 * @internal
 *
 * Resets the mirrored mode from the system mirror mode for widgets that are in
 * automatic mirroring mode. This function does not call elm_widget_theme.
 *
 * @param obj The widget.
 * @param mirrored EINA_TRUE to set mirrored mode. EINA_FALSE to unset.
 */
static void
_elm_widget_mirrored_reload(Evas_Object *obj)
{
   API_ENTRY return;
   Eina_Bool mirrored = elm_config_mirrored_get();

   if (efl_ui_mirrored_automatic_get(obj) && (sd->is_mirrored != mirrored))
     {
        sd->is_mirrored = mirrored;
     }
}

static void
_parents_focus(Evas_Object *obj)
{
   for (; obj; obj = elm_widget_parent_get(obj))
     {
        INTERNAL_ENTRY;
        if (sd->focused) return;
        sd->focused = 1;
     }
}

static void
_parents_unfocus(Evas_Object *obj)
{
   for (; obj; obj = elm_widget_parent_get(obj))
     {
        INTERNAL_ENTRY;
        if (!sd->focused) return;
        sd->focused = 0;
     }
}
//TIZEN_ONLY(20180607): Restore legacy focus
static void
_on_sub_obj_hide(void *data EINA_UNUSED, const Efl_Event *event)
{
   efl_ui_widget_focus_hide_handle(event->object);
}
//
static void
_on_sub_obj_del(void *data, const Efl_Event *event)
{
   ELM_WIDGET_DATA_GET_OR_RETURN(data, sd);

   if (_elm_widget_is(event->object))
     {
        if (_is_focused(event->object)) _parents_unfocus(sd->obj);
     }
   if (event->object == sd->resize_obj)
     {
        /* already dels sub object */
        elm_widget_resize_object_set(sd->obj, NULL);
     }
   else if (event->object == sd->hover_obj)
     {
        sd->hover_obj = NULL;
     }
   else
     {
        if (!elm_widget_sub_object_del(sd->obj, event->object))
          ERR("failed to remove sub object %p from %p\n", event->object, sd->obj);
     }
}

static const Evas_Smart_Cb_Description _smart_callbacks[] =
{
   {SIG_WIDGET_FOCUSED, ""},
   {SIG_WIDGET_UNFOCUSED, ""},
   {SIG_WIDGET_LANG_CHANGED, ""},
   {SIG_WIDGET_ACCESS_CHANGED, ""},
   // TIZEN_ONLY(20161018): add highlighted/unhighlighted signal for atspi
   {SIG_WIDGET_ATSPI_HIGHLIGHTED, ""},
   {SIG_WIDGET_ATSPI_UNHIGHLIGHTED, ""},
   //
   {NULL, NULL}
};

static void
_obj_mouse_down(void *data,
                Evas *e EINA_UNUSED,
                Evas_Object *obj EINA_UNUSED,
                void *event_info)
{
   Evas_Object *top;

   ELM_WIDGET_DATA_GET(data, sd);
   Evas_Event_Mouse_Down *ev = event_info;
   if (ev->event_flags & EVAS_EVENT_FLAG_ON_HOLD) return;

   top = elm_widget_top_get(data);
   if (top && efl_isa(top, EFL_UI_WIN_CLASS)) _elm_win_focus_auto_hide(top);
   sd->still_in = EINA_TRUE;
}

static void
_obj_mouse_move(void *data,
                Evas *e EINA_UNUSED,
                Evas_Object *obj,
                void *event_info)
{
   ELM_WIDGET_DATA_GET(data, sd);
   Evas_Event_Mouse_Move *ev = event_info;
   if (!sd->still_in) return;

   if (ev->event_flags & EVAS_EVENT_FLAG_ON_HOLD)
     sd->still_in = EINA_FALSE;
   else
     {
        Evas_Coord x, y, w, h;
        evas_object_geometry_get(obj, &x, &y, &w, &h);
        if (ELM_RECTS_POINT_OUT(x, y, w, h, ev->cur.canvas.x, ev->cur.canvas.y))
          sd->still_in = EINA_FALSE;
     }
}

static void
_obj_mouse_up(void *data,
              Evas *e EINA_UNUSED,
              Evas_Object *obj,
              void *event_info)
{
   ELM_WIDGET_DATA_GET(data, sd);
   Evas_Event_Mouse_Up *ev = event_info;

   if (sd->still_in && (ev->flags == EVAS_BUTTON_NONE) &&
       (sd->focus_move_policy == ELM_FOCUS_MOVE_POLICY_CLICK))
     elm_widget_focus_mouse_up_handle(evas_object_widget_parent_find(obj));

   sd->still_in = EINA_FALSE;
}

static void
_obj_mouse_in(void *data,
              Evas *e EINA_UNUSED,
              Evas_Object *obj,
              void *event_info EINA_UNUSED)
{
   ELM_WIDGET_DATA_GET(data, sd);
   if (sd->focus_move_policy == ELM_FOCUS_MOVE_POLICY_IN)
     elm_widget_focus_mouse_up_handle(evas_object_widget_parent_find(obj));
}

EOLIAN static void
_efl_ui_widget_efl_canvas_group_group_add(Eo *obj, Elm_Widget_Smart_Data *priv)
{

   priv->obj = obj;
   priv->mirrored_auto_mode = EINA_TRUE; /* will follow system locale
                                          * settings */
   priv->focus_move_policy_auto_mode = EINA_TRUE;
   priv->focus_region_show_mode = ELM_FOCUS_REGION_SHOW_WIDGET;
   elm_widget_can_focus_set(obj, EINA_TRUE);
   priv->is_mirrored = elm_config_mirrored_get();
   priv->focus_move_policy = _elm_config->focus_move_policy;

   evas_object_event_callback_add(obj, EVAS_CALLBACK_MOUSE_DOWN,
                                  _obj_mouse_down, obj);
   evas_object_event_callback_add(obj, EVAS_CALLBACK_MOUSE_MOVE,
                                  _obj_mouse_move, obj);
   evas_object_event_callback_add(obj, EVAS_CALLBACK_MOUSE_UP,
                                  _obj_mouse_up, obj);
   evas_object_event_callback_add(obj, EVAS_CALLBACK_MOUSE_IN,
                                  _obj_mouse_in, obj);
}

EOLIAN static void
_efl_ui_widget_efl_canvas_group_group_del(Eo *obj, Elm_Widget_Smart_Data *sd)
{
   Evas_Object *sobj;
   Elm_Translate_String_Data *ts;
   Elm_Event_Cb_Data *ecb;

   if (sd->hover_obj)
     {
        /* detach it from us */
        _callbacks_del(sd->hover_obj, obj);
        sd->hover_obj = NULL;
     }

   while (sd->subobjs)
     {
        sobj = eina_list_data_get(sd->subobjs);

        /* let the objects clean-up themselves and get rid of this list */
        if (!elm_widget_sub_object_del(obj, sobj))
          {
             ERR("failed to remove sub object %p from %p\n", sobj, obj);
             sd->subobjs = eina_list_remove_list
                 (sd->subobjs, sd->subobjs);
          }
        // FIXME: is that a legacy or a new object ?
        evas_object_del(sobj);
     }
   sd->tooltips = eina_list_free(sd->tooltips); /* should be empty anyway */
   sd->cursors = eina_list_free(sd->cursors); /* should be empty anyway */
   while (sd->translate_strings)
     {
        ts = EINA_INLIST_CONTAINER_GET(sd->translate_strings,
                                       Elm_Translate_String_Data);
        eina_stringshare_del(ts->id);
        eina_stringshare_del(ts->domain);
        eina_stringshare_del(ts->string);
        sd->translate_strings = eina_inlist_remove(sd->translate_strings,
                                                   sd->translate_strings);
        free(ts);
     }

   EINA_LIST_FREE(sd->event_cb, ecb)
      free(ecb);

   eina_stringshare_del(sd->klass);
   eina_stringshare_del(sd->group);
   eina_stringshare_del(sd->style);
   if (sd->theme) elm_theme_free(sd->theme);
   //TIZEN_ONLY(20180607): Restore legacy focus
   if (elm_widget_is_legacy(obj))
     {
        _if_focused_revert(obj, EINA_TRUE);
        efl_ui_widget_focus_custom_chain_unset(obj);
     }
   //
   eina_stringshare_del(sd->access_info);
   eina_stringshare_del(sd->accessible_name);
   evas_object_smart_data_set(obj, NULL);
   efl_canvas_group_del(efl_super(obj, MY_CLASS));
}

static void
_smart_reconfigure(Elm_Widget_Smart_Data *sd)
{
   if (sd->resize_obj)
     {
        evas_object_geometry_set(sd->resize_obj, sd->x, sd->y, sd->w, sd->h);
     }
   if (sd->hover_obj)
     {
        evas_object_geometry_set(sd->hover_obj, sd->x, sd->y, sd->w, sd->h);
     }
   if (sd->bg)
     {
        evas_object_geometry_set(sd->bg, sd->x, sd->y, sd->w, sd->h);
     }
   if (sd->has_shadow)
     _elm_widget_shadow_update(sd->obj);
}

EOLIAN static void
_efl_ui_widget_efl_gfx_entity_position_set(Eo *obj EINA_UNUSED, Elm_Widget_Smart_Data *sd, Eina_Position2D pos)
{
   if (_evas_object_intercept_call(obj, EVAS_OBJECT_INTERCEPT_CB_MOVE, 0, pos.x, pos.y))
     return;

   sd->x = pos.x;
   sd->y = pos.y;
   _smart_reconfigure(sd);

   efl_gfx_entity_position_set(efl_super(obj, MY_CLASS), pos);
}

EOLIAN static void
_efl_ui_widget_efl_gfx_entity_size_set(Eo *obj EINA_UNUSED, Elm_Widget_Smart_Data *sd, Eina_Size2D sz)
{
   if (_evas_object_intercept_call(obj, EVAS_OBJECT_INTERCEPT_CB_RESIZE, 0, sz.w, sz.h))
     return;

   sd->w = sz.w;
   sd->h = sz.h;
   _smart_reconfigure(sd);

   efl_gfx_entity_size_set(efl_super(obj, MY_CLASS), sz);
}

void
_elm_widget_full_eval_children(Eo *obj, Elm_Widget_Smart_Data *sd)
{
   //TIZEN_ONLY(20180607): Restore legacy focus
   if (elm_widget_is_legacy(obj))
     return;
   //
   Eina_List *l;
   Eo *child;

   _full_eval(obj, sd);

   EINA_LIST_FOREACH(sd->subobjs , l, child)
     {
        Elm_Widget_Smart_Data *sd_child;

        if (!efl_isa(child, EFL_UI_WIDGET_CLASS)) continue;

        sd_child = efl_data_scope_get(child, EFL_UI_WIDGET_CLASS);
        _elm_widget_full_eval_children(child, sd_child);
     }
}

EOLIAN static void
_efl_ui_widget_efl_gfx_entity_visible_set(Eo *obj, Elm_Widget_Smart_Data *pd, Eina_Bool vis)
{
   Eina_Iterator *it;
   Evas_Object *o;

   if (_evas_object_intercept_call(obj, EVAS_OBJECT_INTERCEPT_CB_VISIBLE, 0, vis))
     {
        _elm_widget_full_eval_children(obj, pd);
        return;
     }

   efl_gfx_entity_visible_set(efl_super(obj, MY_CLASS), vis);

   _elm_widget_full_eval_children(obj, pd);


   it = evas_object_smart_iterator_new(obj);
   EINA_ITERATOR_FOREACH(it, o)
     {
       if (evas_object_data_get(o, "_elm_leaveme")) continue;
       efl_gfx_entity_visible_set(o, vis);
     }
   eina_iterator_free(it);

   if (!_elm_atspi_enabled() || pd->on_destroy)
     return;

   if (vis)
     {
        efl_access_added(obj);
        if (_elm_widget_onscreen_is(obj))
          efl_access_state_changed_signal_emit(obj, EFL_ACCESS_STATE_TYPE_SHOWING, EINA_TRUE);
     }
   else
     {
        //TIZEN_ONLY(20161223) check if the parent of highlighted object is hide
        Eo *highlighted_obj;
        highlighted_obj = _elm_object_accessibility_currently_highlighted_get();
        if (highlighted_obj && highlighted_obj != obj)
          {
             Eo *parent;
             parent = efl_provider_find(efl_parent_get(highlighted_obj), EFL_ACCESS_OBJECT_MIXIN);
             while (parent)
               {
                  if (parent == obj)
                    {
                       efl_access_state_changed_signal_emit(highlighted_obj, EFL_ACCESS_STATE_TYPE_SHOWING, EINA_FALSE);
                       efl_access_component_highlight_clear(highlighted_obj);
                       break;
                    }
                  parent = efl_provider_find(efl_parent_get(parent), EFL_ACCESS_OBJECT_MIXIN);
               }
          }
        //
        efl_access_state_changed_signal_emit(obj, EFL_ACCESS_STATE_TYPE_SHOWING, EINA_FALSE);
     }
}

EOLIAN static void
_efl_ui_widget_efl_gfx_color_color_set(Eo *obj, Elm_Widget_Smart_Data *pd, int r, int g, int b, int a)
{
   Eina_Iterator *it;
   Evas_Object *o;

   if (_evas_object_intercept_call(obj, EVAS_OBJECT_INTERCEPT_CB_COLOR_SET, 0, r, g, b, a))
     return;

   efl_gfx_color_set(efl_super(obj, MY_CLASS), r, g, b, a);

   it = evas_object_smart_iterator_new(obj);
   EINA_ITERATOR_FOREACH(it, o)
     {
       if (pd->bg == o) continue;
       if (evas_object_data_get(o, "_elm_leaveme")) continue;
       evas_object_color_set(o, r, g, b, a);
     }
   eina_iterator_free(it);
}

EOLIAN static void
_efl_ui_widget_efl_canvas_object_no_render_set(Eo *obj, Elm_Widget_Smart_Data *_pd EINA_UNUSED, Eina_Bool hide)
{
   Eina_Iterator *it;
   Evas_Object *o;

   hide = !!hide;
   if (efl_canvas_object_no_render_get(obj) == hide)
     return;

   it = evas_object_smart_iterator_new(obj);
   EINA_ITERATOR_FOREACH(it, o)
     {
       if (evas_object_data_get(o, "_elm_leaveme")) continue;
       efl_canvas_object_no_render_set(o, hide);
     }
   eina_iterator_free(it);

   // bypass implementation in Efl.Canvas.Group
   efl_canvas_object_no_render_set(efl_super(obj, EFL_CANVAS_GROUP_CLASS), hide);
}

EOLIAN static void
_efl_ui_widget_efl_canvas_object_is_frame_object_set(Eo *obj, Elm_Widget_Smart_Data *pd, Eina_Bool frame)
{
   Evas_Object *o;
   Eina_List *li;

   frame = !!frame;
   efl_canvas_object_is_frame_object_set(efl_super(obj, MY_CLASS), frame);
   EINA_LIST_FOREACH(pd->subobjs, li, o)
     {
       if (evas_object_data_get(o, "_elm_leaveme")) continue;
       efl_canvas_object_is_frame_object_set(o, frame);
     }
}

EOLIAN static void
_efl_ui_widget_efl_canvas_object_clipper_set(Eo *obj, Elm_Widget_Smart_Data *_pd EINA_UNUSED, Evas_Object *clip)
{
   Eina_Iterator *it;
   Evas_Object *o;

   if (_evas_object_intercept_call(obj, EVAS_OBJECT_INTERCEPT_CB_CLIP_SET, 0, clip))
     return;

   efl_canvas_object_clipper_set(efl_super(obj, MY_CLASS), clip);

   it = evas_object_smart_iterator_new(obj);
   EINA_ITERATOR_FOREACH(it, o)
     {
       if (evas_object_data_get(o, "_elm_leaveme")) continue;
       evas_object_clip_set(o, clip);
     }
   eina_iterator_free(it);
}

EOLIAN static void
_efl_ui_widget_efl_canvas_group_group_calculate(Eo *obj EINA_UNUSED, Elm_Widget_Smart_Data *_pd EINA_UNUSED)
{
   /* a NO-OP, on the base */
}

EOLIAN static void
_efl_ui_widget_efl_canvas_group_group_member_add(Eo *obj, Elm_Widget_Smart_Data *pd, Evas_Object *child)
{
   int r, g, b, a;
   efl_canvas_group_member_add(efl_super(obj, MY_CLASS), child);

   if (evas_object_data_get(child, "_elm_leaveme")) return;

   if (pd->bg != child)
     {
        evas_object_color_get(obj, &r, &g, &b, &a);
        evas_object_color_set(child, r, g, b, a);
     }

   efl_canvas_object_no_render_set(child, efl_canvas_object_no_render_get(obj));
   evas_object_clip_set(child, evas_object_clip_get(obj));

   if (evas_object_visible_get(obj))
     evas_object_show(child);
   else
     evas_object_hide(child);
}

EOLIAN static void
_efl_ui_widget_efl_canvas_group_group_member_remove(Eo *obj EINA_UNUSED, Elm_Widget_Smart_Data *_pd EINA_UNUSED, Evas_Object *child)
{
   if (!evas_object_data_get(child, "_elm_leaveme"))
      evas_object_clip_unset(child);
   efl_canvas_group_member_remove(efl_super(obj, MY_CLASS), child);
}

// internal funcs
static void
_propagate_x_drag_lock(Evas_Object *obj,
                       int dir)
{
   INTERNAL_ENTRY;
   if (sd->parent_obj)
     {
        ELM_WIDGET_DATA_GET(sd->parent_obj, sd2);
        if (sd2)
          {
             sd2->child_drag_x_locked += dir;
             _propagate_x_drag_lock(sd->parent_obj, dir);
          }
     }
}

static void
_propagate_y_drag_lock(Evas_Object *obj,
                       int dir)
{
   INTERNAL_ENTRY;
   if (sd->parent_obj)
     {
        ELM_WIDGET_DATA_GET(sd->parent_obj, sd2);
        if (sd2)
          {
             sd2->child_drag_y_locked += dir;
             _propagate_y_drag_lock(sd->parent_obj, dir);
          }
     }
}

static Eina_Bool
_propagate_event_legacy(Eo *parent, const Efl_Event *event, Eo *obj, Elm_Event_Cb_Data *ecd)
{
   Evas_Callback_Type type;
   Evas_Event_Flags *event_flags, prev_flags;
   union {
      Evas_Event_Key_Down    *down;
      Evas_Event_Key_Up      *up;
      Evas_Event_Mouse_Wheel *wheel;
      void                   *any;
   } event_info;

   if (event->desc == EFL_EVENT_KEY_DOWN)
     {
        event_info.down = efl_input_legacy_info_get(event->info);
        EINA_SAFETY_ON_NULL_RETURN_VAL(event_info.down, EINA_FALSE);
        type = EVAS_CALLBACK_KEY_DOWN;
        event_flags = &event_info.down->event_flags;
     }
   else if (event->desc == EFL_EVENT_KEY_UP)
     {
        event_info.up = efl_input_legacy_info_get(event->info);
        EINA_SAFETY_ON_NULL_RETURN_VAL(event_info.up, EINA_FALSE);
        type = EVAS_CALLBACK_KEY_UP;
        event_flags = &event_info.up->event_flags;
     }
   else if (event->desc == EFL_EVENT_POINTER_WHEEL)
     {
        event_info.wheel = efl_input_legacy_info_get(event->info);
        EINA_SAFETY_ON_NULL_RETURN_VAL(event_info.wheel, EINA_FALSE);
        type = EVAS_CALLBACK_MOUSE_WHEEL;
        event_flags = &event_info.wheel->event_flags;
     }
   else
     return EINA_FALSE;

   prev_flags = *event_flags;
   if (ecd->func((void *)ecd->data, parent, obj, type, event_info.any) ||
       ((*event_flags) & EVAS_EVENT_FLAG_ON_HOLD))
     {
        if (prev_flags != *event_flags)
          efl_input_event_flags_set(event->info, *event_flags);
        return EINA_TRUE;
     }

   return EINA_FALSE;
}

/**
 * @internal
 *
 * If elm_widget_focus_region_get() returns an empty rect (w or h <= 0),
 * this function will ignore region show action.
 */
EAPI void
elm_widget_focus_region_show(Eo *obj)
{
   Evas_Coord ox, oy;
   Eina_Rect r;
   Evas_Object *o;

   o = elm_widget_parent_get(obj);
   if (!o) return;

   r = elm_widget_focus_region_get(obj);
   if (eina_rectangle_is_empty(&r.rect)) return;

   evas_object_geometry_get(obj, &ox, &oy, NULL, NULL);

   while (o)
     {
        Evas_Coord px, py;
        evas_object_geometry_get(o, &px, &py, NULL, NULL);

        if (_elm_scrollable_is(o) && !elm_widget_disabled_get(o))
          {
             Evas_Coord sx, sy;
             elm_interface_scrollable_content_region_get(o, &sx, &sy, NULL, NULL);

             // Get the object's on_focus_region position relative to the scroller.
             Evas_Coord rx, ry;
             rx = ox + r.x - px + sx;
             ry = oy + r.y - py + sy;

             switch (_elm_config->focus_autoscroll_mode)
               {
                case ELM_FOCUS_AUTOSCROLL_MODE_SHOW:
                   elm_interface_scrollable_content_region_show(o, rx, ry, r.w, r.h);
                   break;
                case ELM_FOCUS_AUTOSCROLL_MODE_BRING_IN:
                   elm_interface_scrollable_region_bring_in(o, rx, ry, r.w, r.h);
                   break;
                default:
                   break;
               }

             r = elm_widget_focus_region_get(o);
             evas_object_geometry_get(o, &ox, &oy, NULL, NULL);
          }
        else
          {
             r.x += ox - px;
             r.y += oy - py;
             ox = px;
             oy = py;
          }
        o = elm_widget_parent_get(o);
     }
}


EAPI Eina_Bool
elm_widget_api_check(int ver)
{
   if (ver != ELM_INTERNAL_API_VERSION)
     {
        CRI("Elementary widget api versions do not match");
        return EINA_FALSE;
     }
   return EINA_TRUE;
}

EAPI Eina_Bool
elm_widget_access(Evas_Object *obj,
                  Eina_Bool is_access)
{
   const Eina_List *l;
   Evas_Object *child;
   Eina_Bool ret = EINA_TRUE;

   API_ENTRY return EINA_FALSE;
   EINA_LIST_FOREACH(sd->subobjs, l, child)
     {
        if (elm_widget_is(child))
          ret &= elm_widget_access(child, is_access);
     }

   efl_ui_widget_on_access_update(obj, is_access);
   efl_event_callback_legacy_call(obj, EFL_UI_WIDGET_EVENT_ACCESS_CHANGED, NULL);

   return ret;
}

EOLIAN static void
_efl_ui_widget_on_access_update(Eo *obj EINA_UNUSED, Elm_Widget_Smart_Data *_pd EINA_UNUSED, Eina_Bool is_access EINA_UNUSED)
{
}

static void
_elm_widget_theme_helper(Eina_Error err, Eina_Bool *err_default, Eina_Bool *err_generic)
{
   if (err == EFL_UI_THEME_APPLY_ERROR_DEFAULT)
     *err_default = EINA_TRUE;
   else if (err == EFL_UI_THEME_APPLY_ERROR_GENERIC)
     *err_generic = EINA_TRUE;
}

EAPI Eina_Error
elm_widget_theme(Evas_Object *obj)
{
   const Eina_List *l;
   Evas_Object *child;
   Elm_Tooltip *tt;
   Elm_Cursor *cur;
   Eina_Bool err_default = EINA_FALSE;
   Eina_Bool err_generic = EINA_FALSE;

   API_ENTRY return EFL_UI_THEME_APPLY_ERROR_GENERIC;

   EINA_LIST_FOREACH(sd->subobjs, l, child)
     if (_elm_widget_is(child))
       _elm_widget_theme_helper(elm_widget_theme(child), &err_default, &err_generic);

   if (sd->hover_obj)
     _elm_widget_theme_helper(elm_widget_theme(sd->hover_obj), &err_default, &err_generic);

   EINA_LIST_FOREACH(sd->tooltips, l, tt)
     elm_tooltip_theme(tt);
   EINA_LIST_FOREACH(sd->cursors, l, cur)
     elm_cursor_theme(cur);

   _elm_widget_theme_helper(efl_ui_widget_theme_apply(obj), &err_default, &err_generic);
   if (err_generic) return EFL_UI_THEME_APPLY_ERROR_GENERIC;
   if (err_default) return EFL_UI_THEME_APPLY_ERROR_DEFAULT;
   return EFL_UI_THEME_APPLY_ERROR_NONE;
}

EAPI void
elm_widget_theme_specific(Evas_Object *obj,
                          Elm_Theme *th,
                          Eina_Bool force)
{
   const Eina_List *l;
   Evas_Object *child;
   Elm_Tooltip *tt;
   Elm_Cursor *cur;
   Elm_Theme *th2, *thdef;

   API_ENTRY return;

   thdef = elm_theme_default_get();
   if (!th) th = thdef;
   if (!force)
     {
        th2 = sd->theme;
        if (!th2) th2 = thdef;
        while (th2)
          {
             if (th2 == th)
               {
                  force = EINA_TRUE;
                  break;
               }
             if (th2 == thdef) break;
             th2 = th2->ref_theme;
             if (!th2) th2 = thdef;
          }
     }
   if (!force) return;
   EINA_LIST_FOREACH(sd->subobjs, l, child)
     {
        if (elm_widget_is(child))
          elm_widget_theme_specific(child, th, force);
     }
   if (sd->hover_obj) elm_widget_theme(sd->hover_obj);
   EINA_LIST_FOREACH(sd->tooltips, l, tt)
     elm_tooltip_theme(tt);
   EINA_LIST_FOREACH(sd->cursors, l, cur)
     elm_cursor_theme(cur);
   efl_ui_widget_theme_apply(obj);
}

EOLIAN static Eina_Error
_efl_ui_widget_theme_apply(Eo *obj, Elm_Widget_Smart_Data *_pd EINA_UNUSED)
{
   _elm_widget_mirrored_reload(obj);

   return EFL_UI_THEME_APPLY_ERROR_NONE;
}

/**
 * @internal
 *
 * Returns the widget's mirrored mode.
 *
 * @param obj The widget.
 * @return mirrored mode of the object.
 *
 **/
EOLIAN static Eina_Bool
_efl_ui_widget_efl_ui_i18n_mirrored_get(const Eo *obj EINA_UNUSED, Elm_Widget_Smart_Data *sd)
{
   return sd->is_mirrored;
}

/**
 * @internal
 *
 * Sets the widget's mirrored mode.
 *
 * @param obj The widget.
 * @param mirrored EINA_TRUE to set mirrored mode. EINA_FALSE to unset.
 */
EOLIAN static void
_efl_ui_widget_efl_ui_i18n_mirrored_set(Eo *obj, Elm_Widget_Smart_Data *sd, Eina_Bool mirrored)
{
   mirrored = !!mirrored;

   if (sd->is_mirrored == mirrored) return;

   sd->is_mirrored = mirrored;
   elm_widget_theme(obj);
}

/**
 * Returns the widget's mirrored mode setting.
 *
 * @param obj The widget.
 * @return mirrored mode setting of the object.
 *
 **/
EOLIAN static Eina_Bool
_efl_ui_widget_efl_ui_i18n_mirrored_automatic_get(const Eo *obj EINA_UNUSED, Elm_Widget_Smart_Data *sd)
{
   return sd->mirrored_auto_mode;
}

/**
 * @internal
 *
 * Sets the widget's mirrored mode setting.
 * When widget in automatic mode, it follows the system mirrored mode set by
 * elm_mirrored_set().
 * @param obj The widget.
 * @param automatic EINA_TRUE for auto mirrored mode. EINA_FALSE for manual.
 */
EOLIAN static void
_efl_ui_widget_efl_ui_i18n_mirrored_automatic_set(Eo *obj, Elm_Widget_Smart_Data *sd, Eina_Bool automatic)
{
   if (sd->mirrored_auto_mode != automatic)
     {
        sd->mirrored_auto_mode = automatic;

        if (automatic)
          {
             efl_ui_mirrored_set(obj, elm_config_mirrored_get());
          }
     }
}

/*
 * @internal
 *
 * Add myself as a sub object of parent object
 *
 * @see elm_widget_sub_object_add()
 */
EAPI Eina_Bool
elm_widget_sub_object_parent_add(Evas_Object *sobj)
{
   Eo *parent = NULL;

   parent = efl_parent_get(sobj);
   if (!efl_isa(parent, EFL_UI_WIDGET_CLASS))
     {
        ERR("You passed a wrong parent parameter (%p %s). "
            "Elementary widget's parent should be an elementary widget.", parent, evas_object_type_get(parent));
        return EINA_FALSE;
     }

   return elm_widget_sub_object_add(parent, sobj);
}

EOLIAN static void
_efl_ui_widget_widget_parent_set(Eo *obj, Elm_Widget_Smart_Data *pd, Efl_Ui_Widget *parent)
{
   Efl_Ui_Widget *old_parent;
   //check if we are in the subobject list of parents
   if (parent)
     {
        ELM_WIDGET_DATA_GET_OR_RETURN(parent, ppd);
        EINA_SAFETY_ON_FALSE_RETURN(eina_list_data_find(ppd->subobjs, obj));
     }

   /* NOTE: In the following two lines, 'obj' is correct. Do not change it.
    * Due to elementary's scale policy, scale and prev_scale can be different in
    * some cases. This happens when obj's previous parent and new parent have
    * different scale value.
    * For example, if obj's previous parent's scale is 5 and new parent's scale
    * is 2 while obj's scale is 0. Then 'prev_pscale' is 5 and 'scale' is 2. So
    * we need to reset obj's scale to 5.
    * Note that each widget's scale is 1.0 by default.
    */
   double scale, prev_scale = efl_gfx_entity_scale_get(obj);
   Elm_Theme *th, *prev_th = elm_widget_theme_get(obj);
   Eina_Bool mirrored, pmirrored = efl_ui_mirrored_get(parent);

   old_parent = pd->parent_obj;
   pd->parent_obj = parent;

   // now lets sync up all states
   if (pd->parent_obj)
     {
        scale = efl_gfx_entity_scale_get(obj);
        th = elm_widget_theme_get(obj);
        mirrored = efl_ui_mirrored_get(obj);

        if (!pd->on_create)
          {
             if ((scale != prev_scale) || (th != prev_th) ||
                 (pmirrored != mirrored))
               elm_widget_theme(obj);
          }
        if (_is_focused(obj)) _parents_focus(parent);
        elm_widget_display_mode_set(obj, evas_object_size_hint_display_mode_get(parent));
        elm_widget_disabled_set(obj, efl_ui_widget_disabled_get(parent));
        _elm_widget_top_win_focused_set(obj, _elm_widget_top_win_focused_get(parent));
     }

   _full_eval(obj, pd);

   if (old_parent && _elm_config->atspi_mode)
     {
        Efl_Access_Object *aparent;
        aparent = efl_provider_find(efl_parent_get(obj), EFL_ACCESS_OBJECT_MIXIN);
        if (aparent)
           efl_access_children_changed_del_signal_emit(aparent, obj);
     }

   if (pd->parent_obj && _elm_config->atspi_mode && efl_finalized_get(parent))
     {
        Efl_Access_Object *aparent;
        aparent = efl_provider_find(efl_parent_get(obj), EFL_ACCESS_OBJECT_MIXIN);
        if (aparent)
           efl_access_children_changed_added_signal_emit(aparent, obj);
     }
}

static void
_widget_add_sub(Eo *obj, Elm_Widget_Smart_Data *sd, Evas_Object *sobj)
{
   sd->subobjs = eina_list_append(sd->subobjs, sobj);
   evas_object_data_set(sobj, "elm-parent", obj);
   //TIZEN_ONLY(20181024): Fix parent-children incosistencies in atspi tree
   if (efl_isa(sobj, EFL_ACCESS_OBJECT_MIXIN))
     efl_access_object_access_parent_set(sobj, obj);
   //
   _callbacks_add(sobj, obj);
}

static void
_widget_del_sub(Eo *obj, Elm_Widget_Smart_Data *sd, Evas_Object *sobj)
{
   sd->subobjs = eina_list_remove(sd->subobjs, sobj);
   evas_object_data_del(sobj, "elm-parent");
   _callbacks_del(sobj, obj);
}

EOLIAN static Eina_Bool
_efl_ui_widget_widget_sub_object_add(Eo *obj, Elm_Widget_Smart_Data *sd, Evas_Object *sobj)
{
   Efl_Ui_Widget *parent;

   if (!sobj) return EINA_FALSE;
   EINA_SAFETY_ON_TRUE_RETURN_VAL(obj == sobj, EINA_FALSE);

   //first make sure that we unregister the sobj from the parent
   if (elm_widget_is(sobj))
     parent = efl_ui_widget_parent_get(sobj);
   else
     parent = evas_object_data_get(sobj, "elm-parent");
   if (parent == obj) return EINA_TRUE;
   if (parent)
     {
        if (!efl_ui_widget_sub_object_del(parent, sobj))
          return EINA_FALSE;
     }

   //sobj does not have a parent here
   //first add it to our own children list
   _widget_add_sub(obj, sd, sobj);

   //and if it is a widget, please set the correct parent on the widget itself
   //the parent set method will take care of the property syncing etc.
   if (elm_widget_is(sobj))
     efl_ui_widget_parent_set(sobj, obj);

//TIZEN_ONLY(20190312): Restore child_can_focus
   if (elm_widget_is(sobj))
     {
        /* update child focusable-ness on self and parents, now that a
         * focusable child got in */
        if (!sd->child_can_focus && (_is_focusable(sobj)))
          {
             Elm_Widget_Smart_Data *sdp = sd;

             sdp->child_can_focus = EINA_TRUE;
             while (sdp->parent_obj)
               {
                  sdp = efl_data_scope_get(sdp->parent_obj, MY_CLASS);

                  if (sdp->child_can_focus) break;

                  sdp->child_can_focus = EINA_TRUE;
               }
          }
//

        /***********************************************************
         * TIZEN_ONLY(20180117): Override Paragraph Direction APIs *
         ***********************************************************/
        if (sdc->inherit_paragraph_direction &&
            (sdc->paragraph_direction != efl_canvas_object_paragraph_direction_get(obj)))
          {
             sdc->paragraph_direction = efl_canvas_object_paragraph_direction_get(obj);
             _efl_ui_widget_efl_canvas_object_paragraph_direction_set_internal(sobj, sdc, sdc->paragraph_direction);
             efl_canvas_object_paragraph_direction_set(efl_super(sobj, MY_CLASS), sdc->paragraph_direction);
          }
        /*******
         * END *
         *******/
     }

   return EINA_TRUE;
}

EOLIAN static Eina_Bool
_efl_ui_widget_widget_sub_object_del(Eo *obj, Elm_Widget_Smart_Data *sd, Evas_Object *sobj)
{
   Evas_Object *sobj_parent;

   if (!sobj) return EINA_FALSE;

   EINA_SAFETY_ON_TRUE_RETURN_VAL(obj == sobj, EINA_FALSE);

   sobj_parent = evas_object_data_del(sobj, "elm-parent");
   //TIZEN_ONLY(20181024): Fix parent-children incosistencies in atspi tree
   if (efl_isa(sobj, EFL_ACCESS_OBJECT_MIXIN))
      efl_access_object_access_parent_set(sobj, NULL);
   //
   if (sobj_parent && sobj_parent != obj)
     {
        static int abort_on_warn = -1;

        ERR("removing sub object %p (%s) from parent %p (%s), "
            "but elm-parent is different %p (%s)!",
            sobj, elm_widget_type_get(sobj), obj, elm_widget_type_get(obj),
            sobj_parent, elm_widget_type_get(sobj_parent));

        if (EINA_UNLIKELY(abort_on_warn == -1))
          {
             if (getenv("ELM_ERROR_ABORT")) abort_on_warn = 1;
             else abort_on_warn = 0;
          }
        if (abort_on_warn == 1) abort();

        return EINA_FALSE;
     }

   if (_elm_widget_is(sobj))
     {
        if (_is_focused(sobj))
          {
             elm_widget_tree_unfocusable_set(sobj, EINA_TRUE);
             elm_widget_tree_unfocusable_set(sobj, EINA_FALSE);
          }

//TIZEN_ONLY(20190312): Restore child_can_focus
        if ((sd->child_can_focus) && (_is_focusable(sobj)))
          {
             Evas_Object *parent = obj;

             /* update child focusable-ness on self and parents, now that a
              * focusable child is gone */
             while (parent)
               {
                  const Eina_List *l;
                  Evas_Object *subobj;

                  ELM_WIDGET_DATA_GET(parent, sdp);

                  sdp->child_can_focus = EINA_FALSE;
                  EINA_LIST_FOREACH(sdp->subobjs, l, subobj)
                    {
                       if ((subobj != sobj) && (_is_focusable(subobj)))
                         {
                            sdp->child_can_focus = EINA_TRUE;
                            break;
                         }
                    }

                  /* break again, child_can_focus went back to
                   * original value */
                  if (sdp->child_can_focus) break;
                  parent = sdp->parent_obj;
               }
          }
//
        /***********************************************************
         * TIZEN_ONLY(20180117): Override Paragraph Direction APIs *
         ***********************************************************/
        if (sdc->inherit_paragraph_direction &&
            (sdc->paragraph_direction != EFL_TEXT_BIDIRECTIONAL_TYPE_NEUTRAL))
          {
             sdc->paragraph_direction = EFL_TEXT_BIDIRECTIONAL_TYPE_NEUTRAL;
             _efl_ui_widget_efl_canvas_object_paragraph_direction_set_internal(sobj, sdc, sdc->paragraph_direction);
             efl_canvas_object_paragraph_direction_set(efl_super(sobj, MY_CLASS), EFL_TEXT_BIDIRECTIONAL_TYPE_NEUTRAL);
          }
        /*******
         * END *
         *******/

        efl_ui_widget_parent_set(sobj, NULL);
     }

   if (sd->resize_obj == sobj) sd->resize_obj = NULL;

   _widget_del_sub(obj, sd, sobj);

   return EINA_TRUE;
}

/* protected function - for widget developers only */
EOLIAN static void
_efl_ui_widget_resize_object_set(Eo *obj, Elm_Widget_Smart_Data *sd, Eo *sobj)
{
   Evas_Object *parent;

   if (sd->resize_obj == sobj) return;
   EINA_SAFETY_ON_TRUE_RETURN(sobj && !efl_isa(sobj, EFL_CANVAS_OBJECT_CLASS));

   // orphan previous resize obj
   if (sd->resize_obj)
     {
        evas_object_clip_unset(sd->resize_obj);
        evas_object_smart_member_del(sd->resize_obj);

        if (_elm_widget_is(sd->resize_obj))
          {
             if (_is_focused(sd->resize_obj)) _parents_unfocus(obj);
          }
        elm_widget_sub_object_del(obj, sd->resize_obj);
     }

   sd->resize_obj = sobj;
   if (!sobj) return;

   // orphan new resize obj
   parent = evas_object_data_get(sobj, "elm-parent");
   if (parent && parent != obj)
     {
        ELM_WIDGET_DATA_GET(parent, sdp);

        /* should be there, just being paranoid */
        if (sdp)
          {
             if (sdp->resize_obj == sobj)
               elm_widget_resize_object_set(parent, NULL);
             else
               elm_widget_sub_object_del(parent, sobj);
          }
     }

   elm_widget_sub_object_add(obj, sobj);
   evas_object_smart_member_add(sobj, obj);
   _smart_reconfigure(sd);
}

/*
 * @internal
 *
 * WARNING: the programmer is responsible, in the scenario of
 * exchanging a hover object, of cleaning the old hover "target"
 * before
 */
EAPI void
elm_widget_hover_object_set(Eo *obj, Evas_Object *sobj)
{
   Elm_Widget_Smart_Data *sd = efl_data_scope_safe_get(obj, MY_CLASS);
   if (!sd) return;

   if (sd->hover_obj)
     {
        _callbacks_del(sd->hover_obj, obj);
     }
   sd->hover_obj = sobj;
   if (sd->hover_obj)
     {
        _callbacks_add(sobj, obj);
        _smart_reconfigure(sd);
     }
}

EOLIAN static void
_efl_ui_widget_focus_allow_set(Eo *obj, Elm_Widget_Smart_Data *sd, Eina_Bool can_focus)
{
   can_focus = !!can_focus;

   if (sd->can_focus == can_focus) return;
   sd->can_focus = can_focus;
   if (sd->can_focus)
     {
//TIZEN_ONLY(20190312): Restore child_can_focus
        /* update child_can_focus of parents */
        Evas_Object *o = obj;

        for (;;)
          {
             o = elm_widget_parent_get(o);
             if (!o) break;
             ELM_WIDGET_DATA_GET(o, sdp);
             if (!sdp || sdp->child_can_focus) break;
             sdp->child_can_focus = EINA_TRUE;
          }
//

        efl_event_callback_array_add(obj, focus_callbacks(), NULL);
     }
   else
     {
//TIZEN_ONLY(20190312): Restore child_can_focus
        // update child_can_focus of parents */
        Evas_Object *parent = elm_widget_parent_get(obj);
        while (parent)
          {
             const Eina_List *l;
             Evas_Object *subobj;

             ELM_WIDGET_DATA_GET(parent, sdp);

             sdp->child_can_focus = EINA_FALSE;
             EINA_LIST_FOREACH(sdp->subobjs, l, subobj)
               {
                  if (_is_focusable(subobj))
                    {
                       sdp->child_can_focus = EINA_TRUE;
                       break;
                    }
               }
             /* break again, child_can_focus went back to
              * original value */
             if (sdp->child_can_focus) break;
             parent = sdp->parent_obj;
          }
//
        efl_event_callback_array_del(obj, focus_callbacks(), NULL);
     }
     if (efl_finalized_get(obj))
       _full_eval(obj, sd);
}

EOLIAN static Eina_Bool
_efl_ui_widget_focus_allow_get(const Eo *obj EINA_UNUSED, Elm_Widget_Smart_Data *sd)
{
   return sd->can_focus;
}

EAPI Eina_Bool
elm_widget_child_can_focus_get(const Eo *obj)
{
   Elm_Widget_Smart_Data *sd = efl_data_scope_safe_get(obj, MY_CLASS);
   if (!sd) return EINA_FALSE;

//TIZEN_ONLY(20190312): Restore child_can_focus
   //return sd->logical.child_count > 0;
   return sd->child_can_focus;
//
}

/**
 * @internal
 *
 * This API makes the widget object and its children to be unfocusable.
 *
 * This API can be helpful for an object to be deleted.
 * When an object will be deleted soon, it and its children may not
 * want to get focus (by focus reverting or by other focus controls).
 * Then, just use this API before deleting.
 *
 * @param obj The widget root of sub-tree
 * @param tree_unfocusable If true, set the object sub-tree as unfocusable
 *
 * @ingroup Widget
 */
EAPI void
elm_widget_tree_unfocusable_set(Eo *obj, Eina_Bool tree_unfocusable)
{
   Elm_Widget_Smart_Data *sd = efl_data_scope_safe_get(obj, MY_CLASS);
   if (!sd) return;

   tree_unfocusable = !!tree_unfocusable;
   if (sd->tree_unfocusable == tree_unfocusable) return;
   sd->tree_unfocusable = tree_unfocusable;
   //TIZEN_ONLY(20180607): Restore legacy focus
   //_elm_widget_focus_tree_unfocusable_handle(obj);
   efl_ui_widget_focus_tree_unfocusable_handle(obj);
   //
   //focus state eval on all children
   _elm_widget_full_eval_children(obj, sd);
}

/**
 * @internal
 *
 * This returns true, if the object sub-tree is unfocusable.
 *
 * @param obj The widget root of sub-tree
 * @return EINA_TRUE if the object sub-tree is unfocusable
 *
 * @ingroup Widget
 */
EAPI Eina_Bool
elm_widget_tree_unfocusable_get(const Eo *obj)
{
   Elm_Widget_Smart_Data *sd = efl_data_scope_safe_get(obj, MY_CLASS);
   if (!sd) return EINA_FALSE;

   return sd->tree_unfocusable;
}

/**
 * @internal
 *
 * Get the list of focusable child objects.
 *
 * This function returns list of child objects which can get focus.
 *
 * @param obj The parent widget
 * @return list of focusable child objects.
 *
 * @ingroup Widget
 */
EAPI Eina_List*
elm_widget_can_focus_child_list_get(const Eo *obj)
{
   Elm_Widget_Smart_Data *sd = efl_data_scope_safe_get(obj, MY_CLASS);
   const Eina_List *l;
   Eina_List *child_list = NULL;
   Evas_Object *child;

   if (!sd) return NULL;
   EINA_LIST_FOREACH(sd->subobjs, l, child)
     {
        if (!_elm_widget_is(child)) continue;
        if ((elm_widget_can_focus_get(child)) &&
            (evas_object_visible_get(child)) &&
            (!elm_widget_disabled_get(child)))
          child_list = eina_list_append(child_list, child);
        else
          {
             Eina_List *can_focus_list;
             can_focus_list = elm_widget_can_focus_child_list_get(child);
             if (can_focus_list)
               child_list = eina_list_merge(child_list, can_focus_list);
          }
     }

   return child_list;
}

/** @internal */
EAPI void
elm_widget_highlight_ignore_set(Eo *obj, Eina_Bool ignore)
{
   Elm_Widget_Smart_Data *sd = efl_data_scope_safe_get(obj, MY_CLASS);
   if (!sd) return;

   sd->highlight_ignore = !!ignore;
}

/** @internal */
EAPI Eina_Bool
elm_widget_highlight_ignore_get(const Eo *obj)
{
   Elm_Widget_Smart_Data *sd = efl_data_scope_safe_get(obj, MY_CLASS);
   if (!sd) return EINA_FALSE;

   return sd->highlight_ignore;
}

/** @internal */
EAPI void
elm_widget_highlight_in_theme_set(Eo *obj, Eina_Bool highlight)
{
   Elm_Widget_Smart_Data *sd = efl_data_scope_safe_get(obj, MY_CLASS);
   if (!sd) return;

   sd->highlight_in_theme = !!highlight;
   /* FIXME: if focused, it should switch from one mode to the other */
}

void
_elm_widget_highlight_in_theme_update(Eo *obj)
{
   Evas_Object *top = elm_widget_top_get(obj);

   if (top && efl_isa(top, EFL_UI_WIN_CLASS))
     {
        _elm_win_focus_highlight_in_theme_update(
           top, elm_widget_highlight_in_theme_get(obj));
     }
}

/** @internal */
EAPI Eina_Bool
elm_widget_highlight_in_theme_get(const Eo *obj)
{
   Elm_Widget_Smart_Data *sd = efl_data_scope_safe_get(obj, MY_CLASS);
   if (!sd) return EINA_FALSE;

   return sd->highlight_in_theme;
}

/** @internal */
EAPI void
elm_widget_access_highlight_in_theme_set(Eo *obj, Eina_Bool highlight)
{
   Elm_Widget_Smart_Data *sd = efl_data_scope_safe_get(obj, MY_CLASS);
   if (!sd) return;

   sd->access_highlight_in_theme = !!highlight;
}

/** @internal */
EAPI Eina_Bool
elm_widget_access_highlight_in_theme_get(const Eo *obj)
{
   Elm_Widget_Smart_Data *sd = efl_data_scope_safe_get(obj, MY_CLASS);
   if (!sd) return EINA_FALSE;

   return sd->access_highlight_in_theme;
}

/** @internal */
EAPI Eina_Bool
elm_widget_highlight_get(const Eo *obj)
{
   Elm_Widget_Smart_Data *sd = efl_data_scope_safe_get(obj, MY_CLASS);
   if (!sd) return EINA_FALSE;

   return sd->highlighted;
}
//TIZEN_ONLY(20180607): Restore legacy focus
EAPI Eina_Bool
elm_widget_focus_get(const Eo *obj)
{
   ELM_WIDGET_DATA_GET_OR_RETURN(obj, sd, EINA_FALSE);
   return (sd->focused && sd->top_win_focused);
}

EOLIAN static Evas_Object*
_efl_ui_widget_focused_object_get(const Eo *obj, Elm_Widget_Smart_Data *sd)
{
   const Evas_Object *subobj;
   const Eina_List *l;

   if (!sd->focused || !sd->top_win_focused) return NULL;
   EINA_LIST_FOREACH(sd->subobjs, l, subobj)
     {
        Evas_Object *fobj;
        if (!_elm_widget_is(subobj)) continue;
        fobj = efl_ui_widget_focused_object_get(subobj);
        if (fobj) return fobj;
     }
   return (Evas_Object *)obj;
}
//

EAPI Eina_Bool
elm_widget_is(const Evas_Object *obj)
{
   return _elm_widget_is(obj);
}

EAPI void
elm_widget_access_info_set(Efl_Ui_Widget *obj, const char *txt)
{
   efl_ui_widget_access_info_set(obj, txt);
}

EAPI const char *
elm_widget_access_info_get(const Efl_Ui_Widget *obj)
{
   return efl_ui_widget_access_info_get(obj);
}

EAPI Eo *
elm_widget_top_get(const Eo *obj)
{
   Efl_Ui_Widget *parent = elm_widget_parent_get(obj);
   if (parent)
     {
        if (!efl_isa(parent, EFL_UI_WIDGET_CLASS)) return NULL;
        return elm_widget_top_get(parent);
     }
   /* XXX const */
   return (Evas_Object *)obj;
}

EAPI Evas_Object *
elm_widget_parent_widget_get(const Evas_Object *obj)
{
   Evas_Object *parent;

   if (_elm_widget_is(obj))
     {
        ELM_WIDGET_DATA_GET(obj, sd);
        if (!sd) return NULL;
        parent = sd->parent_obj;
     }
   else
     {
        parent = evas_object_data_get(obj, "elm-parent");
        if (!parent) parent = evas_object_smart_parent_get(obj);
     }

   while (parent)
     {
        Evas_Object *elm_parent;
        if (_elm_widget_is(parent)) break;
        elm_parent = evas_object_data_get(parent, "elm-parent");
        if (elm_parent) parent = elm_parent;
        else parent = evas_object_smart_parent_get(parent);
     }
   return parent;
}

EAPI void
elm_widget_event_callback_add(Eo *obj, Elm_Event_Cb func, const void *data)
{
   API_ENTRY return;
   EINA_SAFETY_ON_NULL_RETURN(func);

   Elm_Event_Cb_Data *ecb = ELM_NEW(Elm_Event_Cb_Data);
   if (!ecb)
     {
        ERR("Failed to allocate memory");
        return;
     }
   ecb->func = func;
   ecb->data = data;
   sd->event_cb = eina_list_append(sd->event_cb, ecb);
}

EAPI void *
elm_widget_event_callback_del(Eo *obj, Elm_Event_Cb func, const void *data)
{
   API_ENTRY return NULL;
   EINA_SAFETY_ON_NULL_RETURN_VAL(func, NULL);
   Eina_List *l;
   Elm_Event_Cb_Data *ecd;

   EINA_LIST_FOREACH(sd->event_cb, l, ecd)
     if ((ecd->func == func) && (ecd->data == data))
       {
          free(ecd);
          sd->event_cb = eina_list_remove_list(sd->event_cb, l);
          return (void *)data;
       }

   return NULL;
}

static void
_propagate_event(void *data EINA_UNUSED, const Efl_Event *eo_event)
{
   Evas_Object *obj = eo_event->object;
   Evas_Object *parent = obj;
   Elm_Event_Cb_Data *ecd;
   Eina_List *l, *l_prev;

   if ((evas_focus_get(evas_object_evas_get(obj)) != elm_widget_top_get(obj)) &&
       efl_isa(obj, EFL_UI_WIN_CLASS))
     return;

   while (parent && !efl_input_processed_get(eo_event->info))
     {
        Elm_Widget_Smart_Data *sd = efl_data_scope_safe_get(parent, MY_CLASS);
        if (!sd) return;

        if (elm_widget_disabled_get(obj))
          {
             parent = sd->parent_obj;
             continue;
          }

        if (efl_ui_widget_input_event_handler(parent, eo_event, obj))
          return;

        EINA_LIST_FOREACH_SAFE(sd->event_cb, l, l_prev, ecd)
          {
             if (_propagate_event_legacy(parent, eo_event, obj, ecd))
               return;
          }

        parent = sd->parent_obj;
     }
}

double
_elm_widget_focus_direction_weight_get(const Evas_Object *obj1,
                      const Evas_Object *obj2,
                      double degree)
{
   Evas_Coord obj_x1, obj_y1, w1, h1, obj_x2, obj_y2, w2, h2;
   double x1, yy1, x2, yy2, xx1, yyy1, xx2, yyy2;
   double ax, ay, cx, cy;
   double weight = -1.0, g = 0.0;
   // TIZEN_ONLY(20171129): add second_level for weight calculate
   Eina_Bool second_level = EINA_FALSE;
   //

   if (obj1 == obj2) return 0.0;

   degree -= 90.0;
   while (degree >= 360.0)
     degree -= 360.0;
   while (degree < 0.0)
     degree += 360.0;

   evas_object_geometry_get(obj1, &obj_x1, &obj_y1, &w1, &h1);
   cx = obj_x1 + (w1 / 2.0);
   cy = obj_y1 + (h1 / 2.0);
   evas_object_geometry_get(obj2, &obj_x2, &obj_y2, &w2, &h2);

   /* For overlapping cases. */
   if (ELM_RECTS_INTERSECT(obj_x1, obj_y1, w1, h1, obj_x2, obj_y2, w2, h2))
     return 0.0;

   /* Change all points to relative one. */
   x1 = obj_x1 - cx;
   xx1 = x1 + w1;
   yy1 = obj_y1 - cy;
   yyy1 = yy1 + h1;
   x2 = obj_x2 - cx;
   xx2 = x2 + w2;
   yy2 = obj_y2 - cy;
   yyy2 = yy2 + h2;

   /* Get crossing points (ax, ay) between obj1 and a line extending
    * to the direction of current degree. */
   if (degree == 0.0)
     {
        ax = xx1;
        ay = 0.0;
     }
   else if (degree == 90.0)
     {
        ax = 0.0;
        ay = yyy1;
     }
   else if (degree == 180.0)
     {
        ax = x1;
        ay = 0.0;
     }
   else if (degree == 270.0)
     {
        ax = 0.0;
        ay = yy1;
     }
   else
     {
        g = tan(degree * (M_PI / 180.0));
        if ((degree > 0.0) && (degree < 90.0))
          {
             ay = g * xx1;
             if (ay <= yyy1) ax = xx1;
             else
               {
                  ax = yyy1 / g;
                  ay = yyy1;
               }
          }
        else if ((degree > 90.0) && (degree < 180.0))
          {
             ay = g * x1;
             if (ay <= yyy1) ax = x1;
             else
               {
                  ax = yyy1 / g;
                  ay = yyy1;
               }
          }
        else if ((degree > 180.0) && (degree < 270.0))
          {
             ay = g * x1;
             if (ay >= yy1) ax = x1;
             else
               {
                  ax = yy1 / g;
                  ay = yy1;
               }
          }
        else
          {
             ay = g * xx1;
             if (ay >= yy1) ax = xx1;
             else
               {
                  ax = yy1 / g;
                  ay = yy1;
               }
          }
     }

   /* Filter obj2, if it is not in the specific derection. */
   int i = 0;
   double rx[4] = {0.0, 0.0, 0.0, 0.0}, ry[4] = {0.0, 0.0, 0.0, 0.0};
   double t1, t2, u1, v1, u2, v2;

   if ((degree == 45.0) || (degree == 225.0) || (degree == 135.0) ||
       (degree == 315.0))
     {
        u1 = 1.0;
        v1 = 0.0;
        u2 = 0.0;
        v2 = 1.0;
     }
   else
     {
        double g2 = tan((degree + 45.0) * (M_PI / 180.0));
        u1 = (-1.0 * g2);
        u2 = (1.0 / g2);
        v1 = v2 = 1.0;
     }
   t1 = (u1 * ax) + (v1 * ay);
   t2 = (u2 * ax) + (v2 * ay);

#define _R(x) (int)((x + 0.05) * 10.0)

   if ((_R(t1 * ((u1 * x2) + (v1 * yy2))) > 0) && (_R(t2 * ((u2 * x2) +
                                                            (v2 * yy2))) > 0))
     {
        rx[i] = x2;
        ry[i++] = yy2;
     }
   if ((_R(t1 * ((u1 * x2) + (v1 * yyy2))) > 0) && (_R(t2 * ((u2 * x2) +
                                                             (v2 * yyy2))) > 0))
     {
        rx[i] = x2;
        ry[i++] = yyy2;
     }
   if ((_R(t1 * ((u1 * xx2) + (v1 * yy2))) > 0) && (_R(t2 * ((u2 * xx2) +
                                                             (v2 * yy2))) > 0))
     {
        rx[i] = xx2;
        ry[i++] = yy2;
     }
   if ((_R(t1 * ((u1 * xx2) + (v1 * yyy2))) > 0) &&
       (_R(t2 * ((u2 * xx2) + (v2 * yyy2))) > 0))
     {
        rx[i] = xx2;
        ry[i++] = yyy2;
     }
   if (i == 0)
     {
        if (degree == 0.0)
          {
             // TIZEN_ONLY(20171129): add second_level for weight calculate
             //if ((_R(xx2) < 0) || (_R(yy2) > 0) || (_R(yyy2) < 0)) return 0.0;
             if ((_R(xx2) < 0) || (_R(yy2) > 0) || (_R(yyy2) < 0))
               {
                  if (xx1 <= x2) second_level = EINA_TRUE;
                  else return 0.0;
               }
             //
          }
        else if (degree == 90.0)
          {
             // TIZEN_ONLY(20171129): add second_level for weight calculate
             //if ((_R(yyy2) < 0) || (_R(x2) > 0) || (_R(xx2) < 0)) return 0.0;
             if ((_R(yyy2) < 0) || (_R(x2) > 0) || (_R(xx2) < 0))
               {
                  if (yyy1 <= yy2) second_level = EINA_TRUE;
                  else return 0.0;
               }
             //
          }
        else if (degree == 180.0)
          {
             // TIZEN_ONLY(20171129): add second_level for weight calculate
             //if ((_R(x2) > 0) || (_R(yy2) > 0) || (_R(yyy2) < 0)) return 0.0;
             if ((_R(x2) > 0) || (_R(yy2) > 0) || (_R(yyy2) < 0))
               {
                  if (x1 >= xx2) second_level = EINA_TRUE;
                  else return 0.0;
               }
             //
          }
        else if (degree == 270.0)
          {
             // TIZEN_ONLY(20171129): add second_level for weight calculate
             //if ((_R(yy2) > 0) || (_R(x2) > 0) || (_R(xx2) < 0)) return 0.0;
             if ((_R(yy2) > 0) || (_R(x2) > 0) || (_R(xx2) < 0))
               {
                  if (yy1 >= yyy2) second_level = EINA_TRUE;
                  else return 0.0;
               }
             //
          }
        else
          {
             if ((_R(g * x2) >= _R(yy2)) && (_R((g * x2)) <= _R(yyy2)))
               {
                  if (!((_R(ax * x2) > 0) && (_R(ay * (g * x2)) > 0)))
                    return 0.0;
               }
             else if ((_R(g * xx2) >= _R(yy2)) && (_R((g * xx2)) <= _R(yyy2)))
               {
                  if (!((_R(ax * xx2) > 0) && (_R(ay * (g * xx2)) > 0)))
                    return 0.0;
               }
             else if ((_R((1.0 / g) * yy2) >= _R(xx2)) && (_R((1.0 / g) * yy2)
                                                           <= _R(xx2)))
               {
                  if (!((_R(ax * ((1.0 / g) * yy2)) > 0)
                        && (_R(ay * yy2) > 0)))
                    return 0.0;
               }
             else if ((_R((1.0 / g) * yyy2) >= _R(xx2)) &&
                      (_R((1.0 / g) * yyy2) <= _R(xx2)))
               {
                  if (!((_R(ax * ((1.0 / g) * yyy2)) > 0)
                        && (_R(ay * yyy2) > 0))) return 0.0;
               }
             else return 0.0;
          }
     }

   /* Calculate the weight for obj2. */
   if (degree == 0.0)
     {
        if (_R(xx1) > _R(x2)) weight = -1.0;
        else if ((_R(yy2) >= _R(yy1)) && (_R(yyy2) <= _R(yyy1)))
          weight = (x2 - xx1) * (x2 - xx1);
        else if (_R(yy2) > 0)
          weight = ((x2 - xx1) * (x2 - xx1)) + (yy2 * yy2);
        else if (_R(yyy2) < 0)
          weight = ((x2 - xx1) * (x2 - xx1)) + (yyy2 * yyy2);
        else weight = (x2 - xx1) * (x2 - xx1);
     }
   else if (degree == 90.0)
     {
        if (_R(yyy1) > _R(yy2)) weight = -1.0;
        else if ((_R(x2) >= _R(x1)) && (_R(xx2) <= _R(xx1)))
          weight = (yy2 - yyy1) * (yy2 - yyy1);
        else if (_R(x2) > 0)
          weight = (x2 * x2) + ((yy2 - yyy1) * (yy2 - yyy1));
        else if (_R(xx2) < 0)
          weight = (xx2 * xx2) + ((yy2 - yyy1) * (yy2 - yyy1));
        else weight = (yy2 - yyy1) * (yy2 - yyy1);
     }
   else if (degree == 180.0)
     {
        if (_R(x1) < _R(xx2)) weight = -1.0;
        else if ((_R(yy2) >= _R(yy1)) && (_R(yyy2) <= _R(yyy1)))
          weight = (x1 - xx2) * (x1 - xx2);
        else if (_R(yy2) > 0)
          weight = ((x1 - xx2) * (x1 - xx2)) + (yy2 * yy2);
        else if (_R(yyy2) < 0)
          weight = ((x1 - xx2) * (x1 - xx2)) + (yyy2 * yyy2);
        else weight = (x1 - xx2) * (x1 - xx2);
     }
   else if (degree == 270.0)
     {
        if (_R(yy1) < _R(yyy2)) weight = -1.0;
        else if ((_R(x2) >= _R(x1)) && (_R(xx2) <= _R(xx1)))
          weight = (yy1 - yyy2) * (yy1 - yyy2);
        else if (_R(x2) > 0)
          weight = (x2 * x2) + ((yy1 - yyy2) * (yy1 - yyy2));
        else if (_R(xx2) < 0)
          weight = (xx2 * xx2) + ((yy1 - yyy2) * (yy1 - yyy2));
        else weight = (yy1 - yyy2) * (yy1 - yyy2);
     }
   else
     {
        int j = 0, k = 0;
        double sx[4] = {0.0, 0.0, 0.0, 0.0}, sy[4] = {0.0, 0.0, 0.0, 0.0};
        double t_weight[4] = {-1.0, -1.0, -1.0, -1.0};
        if ((_R(g * x2) >= _R(yy2)) && (_R(g * x2) <= _R(yyy2)))
          {
             sx[j] = x2;
             sy[j] = g * x2;
             t_weight[j++] = ((ax - x2) * (ax - x2)) +
               ((ay - (g * x2)) * (ay - (g * x2)));
          }
        if ((_R(g * xx2) >= _R(yy2)) && (_R(g * xx2) <= _R(yyy2)))
          {
             sx[j] = xx2;
             sy[j] = g * xx2;
             t_weight[j++] = ((ax - xx2) * (ax - xx2)) +
               ((ay - (g * xx2)) * (ay - (g * xx2)));
          }
        if ((_R((1.0 / g) * yy2) >= _R(x2)) && (_R((1.0 / g) * yy2) <= _R(xx2)))
          {
             sx[j] = (1.0 / g) * yy2;
             sy[j] = yy2;
             t_weight[j++] =
               ((ax - ((1.0 / g) * yy2)) * (ax - ((1.0 / g) * yy2))) +
               ((ay - yy2) * (ay - yy2));
          }
        if ((_R((1.0 / g) * yyy2) >= _R(x2)) && (_R((1.0 / g) * yyy2)
                                                 <= _R(xx2)))
          {
             sx[j] = (1.0 / g) * yyy2;
             sy[j] = yyy2;
             t_weight[j++] =
               ((ax - ((1.0 / g) * yyy2)) * (ax - ((1.0 / g) * yyy2))) +
               ((ay - yyy2) * (ay - yyy2));
          }

        if ((j > 2) || ((j == 2) && ((_R(sx[0]) != _R(sx[1])) ||
                                     (_R(sy[0]) != _R(sy[1])))))
          {
             for (; k < j; k++)
               {
                  if (_R(t_weight[k]) == 0) return -1.0;
                  if ((1 / weight) < (1 / t_weight[k])) weight = t_weight[k];
               }
          }
        else
          {
             for (; k < i; k++)
               {
                  double ccx, ccy, t1_weight, x_diff, y_diff;
                  ccx = ((1.0 / g) * rx[k] + ry[k]) / (g + (1.0 / g));
                  ccy = g * ccx;
                  x_diff = rx[k] - ccx;
                  if (x_diff < 0) x_diff *= -1.0;
                  y_diff = ry[k] - ccy;
                  if (y_diff < 0) y_diff *= -1.0;
                  t1_weight =
                    (((ax - ccx) * (ax - ccx)) + ((ay - ccy) * (ay - ccy))) +
                    ((x_diff * x_diff * x_diff) + (y_diff * y_diff * y_diff));
                  if ((_R(t1_weight) != 0) && ((1 / weight) < (1 / t1_weight)))
                    weight = t1_weight;
               }
          }
     }
   /* Return the current object's weight. */
   if (weight == -1.0) return 0.0;
   if (_R(weight) == 0) return -1.0;

#undef _R

   // TIZEN_ONLY(20171129): add second_level for weight calculate
   if (second_level) return 1.0 / (weight * 1000000.0);
   //

   return 1.0 / weight;
}

/** @internal */
EAPI void
elm_widget_parent_highlight_set(Eo *obj, Eina_Bool highlighted)
{
   Elm_Widget_Smart_Data *sd =efl_data_scope_safe_get(obj, MY_CLASS);
   if (!sd) return;

   highlighted = !!highlighted;

   Evas_Object *o = elm_widget_parent_get(obj);

   if (o) elm_widget_parent_highlight_set(o, highlighted);

   sd->highlighted = highlighted;
}

EOLIAN static Evas_Object*
_efl_ui_widget_widget_parent_get(const Eo *obj EINA_UNUSED, Elm_Widget_Smart_Data *sd)
{
   return sd->parent_obj;
}

void
_elm_widget_focus_auto_show(Evas_Object *obj)
{
   Evas_Object *top = elm_widget_top_get(obj);
   if (top && efl_isa(top, EFL_UI_WIN_CLASS)) _elm_win_focus_auto_show(top);
}

void
_elm_widget_top_win_focused_set(Evas_Object *obj,
                                Eina_Bool top_win_focused)
{
   const Eina_List *l;
   Evas_Object *child;
   API_ENTRY return;

   if (sd->top_win_focused == top_win_focused) return;
   EINA_LIST_FOREACH(sd->subobjs, l, child)
     {
        if (elm_widget_is(child))
          _elm_widget_top_win_focused_set(child, top_win_focused);
     }
   sd->top_win_focused = top_win_focused;

   if (sd->focused && !sd->top_win_focused)
     efl_ui_focus_object_on_focus_update(obj);
}

Eina_Bool
_elm_widget_top_win_focused_get(const Evas_Object *obj)
{
   API_ENTRY return EINA_FALSE;
   return sd->top_win_focused;
}

EOLIAN static void
_efl_ui_widget_disabled_set(Eo *obj EINA_UNUSED, Elm_Widget_Smart_Data *pd, Eina_Bool disabled)
{
   Efl_Ui_Widget *subs;
   Eina_List *n;

   if (disabled)
     pd->disabled ++;
   else
     pd->disabled = MAX(pd->disabled - 1 ,0);

   EINA_LIST_FOREACH(pd->subobjs, n, subs)
     {
        //TIZEN_ONLY(20180607): Restore legacy focus
        if (elm_widget_is(subs))
          {
             if (elm_widget_is_legacy(subs))
               efl_ui_widget_focus_disabled_handle((Evas_Object *)subs);
          }
        //
        if (efl_isa(subs, EFL_UI_WIDGET_CLASS))
          efl_ui_widget_disabled_set(subs, efl_ui_widget_disabled_get(obj));
     }

   if (efl_finalized_get(obj))
     _elm_widget_full_eval_children(obj, pd);
}

EOLIAN static Eina_Bool
_efl_ui_widget_disabled_get(const Eo *obj EINA_UNUSED, Elm_Widget_Smart_Data *pd)
{
   return pd->disabled > 0;
}

/**
 * @internal
 *
 * Get the focus region of the given widget.
 *
 * @return The region to show. If it's not a valid rectangle it will not show.
 *
 * The focus region is the area of a widget that should brought into the
 * visible area when the widget is focused. Mostly used to show the part of
 * an entry where the cursor is, for example. The area returned is relative
 * to the object @p obj.
 *
 * @param obj The widget object
 * @return The region to show, in relative coordinates. If it's not a valid
 *         rectangle (i.e. w or h <= 0) it will be ignored.
 *
 * @ingroup Widget
 */
EOLIAN static Eina_Rect
_efl_ui_widget_interest_region_get(const Eo *obj, Elm_Widget_Smart_Data *_pd EINA_UNUSED)
{
   Eina_Rect r = {};
   r.size = efl_gfx_entity_size_get(obj);
   return r;
}

EOLIAN static void
_efl_ui_widget_scroll_hold_push(Eo *obj, Elm_Widget_Smart_Data *sd)
{
   sd->scroll_hold++;
   if (sd->scroll_hold == 1)
     {
        if (_elm_scrollable_is(obj))
          {
             if (elm_widget_is_legacy(obj))
               elm_interface_scrollable_hold_set(obj, EINA_TRUE);
             else
               efl_ui_scrollable_scroll_hold_set(obj, EINA_TRUE);
          }
        else
          {
             Evas_Object *child;
             Eina_List *l;

             EINA_LIST_FOREACH(sd->subobjs, l, child)
               {
                  if (elm_widget_is(child) && _elm_scrollable_is(child))
                    {
                       if (elm_widget_is_legacy(obj))
                         elm_interface_scrollable_hold_set(child, EINA_TRUE);
                       else
                         efl_ui_scrollable_scroll_hold_set(child, EINA_TRUE);
                    }
               }
          }
     }
   if (sd->parent_obj) efl_ui_widget_scroll_hold_push(sd->parent_obj);
   // FIXME: on delete/reparent hold pop
}

EOLIAN static void
_efl_ui_widget_scroll_hold_pop(Eo *obj, Elm_Widget_Smart_Data *sd)
{
   sd->scroll_hold--;
   if (!sd->scroll_hold)
     {
        if (_elm_scrollable_is(obj))
          {
             if (elm_widget_is_legacy(obj))
               elm_interface_scrollable_hold_set(obj, EINA_FALSE);
             else
               efl_ui_scrollable_scroll_hold_set(obj, EINA_FALSE);
          }
        else
          {
             Evas_Object *child;
             Eina_List *l;

             EINA_LIST_FOREACH(sd->subobjs, l, child)
               {
                  if (elm_widget_is(child) && _elm_scrollable_is(child))
                    {
                       if (elm_widget_is_legacy(obj))
                         elm_interface_scrollable_hold_set(child, EINA_FALSE);
                       else
                         efl_ui_scrollable_scroll_hold_set(child, EINA_FALSE);
                    }
               }
          }
     }
   if (sd->parent_obj) efl_ui_widget_scroll_hold_pop(sd->parent_obj);
   if (sd->scroll_hold < 0) sd->scroll_hold = 0;
}

EAPI int
elm_widget_scroll_hold_get(const Eo *obj)
{
   Elm_Widget_Smart_Data *sd = efl_data_scope_safe_get(obj, MY_CLASS);

   if (!sd) return 0;
   return sd->scroll_hold;
}

EOLIAN static void
_efl_ui_widget_scroll_freeze_push(Eo *obj, Elm_Widget_Smart_Data *sd)
{
   sd->scroll_freeze++;
   if (sd->scroll_freeze == 1)
     {
        if (_elm_scrollable_is(obj))
          {
             if (elm_widget_is_legacy(obj))
               elm_interface_scrollable_freeze_set(obj, EINA_TRUE);
             else
               efl_ui_scrollable_scroll_freeze_set(obj, EINA_TRUE);
          }
        else
          {
             Evas_Object *child;
             Eina_List *l;

             EINA_LIST_FOREACH(sd->subobjs, l, child)
               {
                  if (elm_widget_is(child) && _elm_scrollable_is(child))
                    if (elm_widget_is_legacy(obj))
                      elm_interface_scrollable_freeze_set(child, EINA_TRUE);
                    else
                      efl_ui_scrollable_scroll_freeze_set(child, EINA_TRUE);
               }
          }
     }
   if (sd->parent_obj) efl_ui_widget_scroll_freeze_push(sd->parent_obj);
   // FIXME: on delete/reparent freeze pop
}

EOLIAN static void
_efl_ui_widget_scroll_freeze_pop(Eo *obj, Elm_Widget_Smart_Data *sd)
{
   sd->scroll_freeze--;
   if (!sd->scroll_freeze)
     {
        if (_elm_scrollable_is(obj))
          {
             if (elm_widget_is_legacy(obj))
               elm_interface_scrollable_freeze_set(obj, EINA_FALSE);
             else
               efl_ui_scrollable_scroll_freeze_set(obj, EINA_FALSE);
          }
        else
          {
             Evas_Object *child;
             Eina_List *l;

             EINA_LIST_FOREACH(sd->subobjs, l, child)
               {
                  if (elm_widget_is(child) && _elm_scrollable_is(child))
                    if (elm_widget_is_legacy(obj))
                      elm_interface_scrollable_freeze_set(child, EINA_FALSE);
                    else
                      efl_ui_scrollable_scroll_freeze_set(child, EINA_FALSE);
               }
          }
     }
   if (sd->parent_obj) efl_ui_widget_scroll_freeze_pop(sd->parent_obj);
   if (sd->scroll_freeze < 0) sd->scroll_freeze = 0;
}

EAPI int
elm_widget_scroll_freeze_get(const Eo *obj)
{
   Elm_Widget_Smart_Data *sd = efl_data_scope_safe_get(obj, MY_CLASS);

   if (!sd) return 0;
   return sd->scroll_freeze;
}

EOLIAN static void
_efl_ui_widget_efl_gfx_entity_scale_set(Eo *obj, Elm_Widget_Smart_Data *sd, double scale)
{
   if (scale < 0.0) scale = 0.0;
   if (sd->scale != scale)
     {
        sd->scale = scale;
        elm_widget_theme(obj);
     }
}

EOLIAN static double
_efl_ui_widget_efl_gfx_entity_scale_get(const Eo *obj EINA_UNUSED, Elm_Widget_Smart_Data *sd)
{
   // FIXME: save walking up the tree by storing/caching parent scale
   if (sd->scale == 0.0)
     {
        if (sd->parent_obj && elm_widget_is(sd->parent_obj))
          {
             return efl_gfx_entity_scale_get(sd->parent_obj);
          }
        else
          {
             return 1.0;
          }
     }
   return sd->scale;
}

EAPI void
elm_widget_theme_set(Evas_Object *obj, Elm_Theme *th)
{
   Elm_Widget_Smart_Data *sd = efl_data_scope_safe_get(obj, MY_CLASS);
   if (!sd) return;

   Eina_Bool apply = EINA_FALSE;
   if (sd->theme != th)
     {
        if (elm_widget_theme_get(obj) != th) apply = EINA_TRUE;
        if (sd->theme) elm_theme_free(sd->theme);
        sd->theme = th;
        if (th) efl_ref(th->eo_theme);
        if (apply) elm_widget_theme(obj);
     }
}

EAPI void
elm_widget_part_text_set(Eo *obj, const char *part, const char *label)
{
   /* legacy support: combobox was special (internal entry is text object). */
   //TIZEN_ONLY(20180426):stop creating unused class.
   if (efl_isa(obj, EFL_UI_LAYOUT_BASE_CLASS))
     elm_layout_text_set(obj, part, label);
   else if (efl_isa(obj, ELM_COMBOBOX_CLASS))
     _elm_combobox_part_text_set(obj, part, label);
   //
}

EAPI const char*
elm_widget_part_text_get(const Eo *obj, const char *part)
{
   /* legacy support: combobox was special (internal entry is text object). */
   //TIZEN_ONLY(20180426):stop creating unused class.
   if (efl_isa(obj, EFL_UI_LAYOUT_BASE_CLASS))
     return elm_layout_text_get(obj, part);
   else if (efl_isa(obj, ELM_COMBOBOX_CLASS))
     return _elm_combobox_part_text_get(obj, part);
   //

   return NULL;
}

static Elm_Translate_String_Data *
_translate_string_data_get(Eina_Inlist *translate_strings, const char *part)
{
   Elm_Translate_String_Data *ts;
   Eina_Stringshare *str;

   if (!translate_strings) return NULL;

   str = eina_stringshare_add(part);
   EINA_INLIST_FOREACH(translate_strings, ts)
     {
        if (ts->id == str) break;
     }

   eina_stringshare_del(str);

   return ts;
}

static Elm_Translate_String_Data *
_part_text_translatable_set(Eina_Inlist **translate_strings, const char *part, Eina_Bool translatable, Eina_Bool preset)
{
   Eina_Inlist *t;
   Elm_Translate_String_Data *ts;
   t = *translate_strings;
   ts = _translate_string_data_get(t, part);

   if (translatable)
     {
        if (!ts)
          {
             ts = ELM_NEW(Elm_Translate_String_Data);
             if (!ts) return NULL;

             ts->id = eina_stringshare_add(part);
             t = eina_inlist_append(t, (Eina_Inlist*) ts);
          }
        if (preset) ts->preset = EINA_TRUE;
     }
   //Delete this exist one if this part has been not preset.
   //see elm_widget_part_text_translatable_set()
   else if (ts && ((preset) || (!ts->preset)))
     {
        t = eina_inlist_remove(t, EINA_INLIST_GET(ts));
        eina_stringshare_del(ts->id);
        eina_stringshare_del(ts->domain);
        eina_stringshare_del(ts->string);
        ELM_SAFE_FREE(ts, free);
     }

   *translate_strings = t;

   return ts;
}

/* internal */
void
elm_widget_part_translatable_text_set(Eo *obj, const char *part, const char *label, const char *domain)
{
   Elm_Translate_String_Data *ts;
   Elm_Widget_Smart_Data *sd;

   sd = efl_data_scope_safe_get(obj, MY_CLASS);
   if (!sd) return;

   if (!label)
     {
        _part_text_translatable_set(&sd->translate_strings, part, EINA_FALSE,
                                    EINA_FALSE);
     }
   else
     {
        ts = _part_text_translatable_set(&sd->translate_strings, part,
                                         EINA_TRUE, EINA_FALSE);
        if (!ts) return;
        if (!ts->string) ts->string = eina_stringshare_add(label);
        else eina_stringshare_replace(&ts->string, label);
        if (!ts->domain) ts->domain = eina_stringshare_add(domain);
        else eina_stringshare_replace(&ts->domain, domain);
#ifdef HAVE_GETTEXT
        if (label[0]) label = dgettext(domain, label);
#endif
     }

   sd->on_translate = EINA_TRUE;
   elm_widget_part_text_set(obj, part, label);
   sd->on_translate = EINA_FALSE;
}

/* legacy only */
EAPI void
elm_widget_domain_part_text_translatable_set(Eo *obj, const char *part, const char *domain, Eina_Bool translatable)
{
   Elm_Translate_String_Data *ts;
   Elm_Widget_Smart_Data *sd;
   const char *text = NULL;

   sd = efl_data_scope_safe_get(obj, MY_CLASS);
   if (!sd) return;

   ts = _part_text_translatable_set(&sd->translate_strings, part,
                                    translatable, EINA_TRUE);
   if (!ts) return;
   if (!ts->domain) ts->domain = eina_stringshare_add(domain);
   else eina_stringshare_replace(&ts->domain, domain);

   text = elm_widget_part_text_get(obj, part);
   if (!text || !text[0]) return;

   if (!ts->string) ts->string = eina_stringshare_add(text);

//Try to translate text since we don't know the text is already translated.
#ifdef HAVE_GETTEXT
   text = dgettext(domain, text);
#endif
   sd->on_translate = EINA_TRUE;
   elm_widget_part_text_set(obj, part, text);
   sd->on_translate = EINA_FALSE;
}

/* internal */
const char *
elm_widget_part_translatable_text_get(const Eo *obj, const char *part, const char **domain)
{
   Elm_Widget_Smart_Data *sd;
   Elm_Translate_String_Data *ts;

   if (domain) *domain = NULL;

   sd = efl_data_scope_safe_get(obj, MY_CLASS);
   if (!sd) return NULL;

   ts = _translate_string_data_get(sd->translate_strings, part);
   if (!ts) return NULL;

   if (domain) *domain = ts->domain;
   return ts->string;
}

EOLIAN static void
_efl_ui_widget_efl_ui_l10n_translation_update(Eo *obj EINA_UNUSED, Elm_Widget_Smart_Data *sd)
{
   const Eina_List *l;
   Evas_Object *child;

   EINA_LIST_FOREACH(sd->subobjs, l, child)
     {
        if (elm_widget_is(child))
          efl_ui_l10n_translation_update(child);
     }

   if (sd->hover_obj) efl_ui_l10n_translation_update(sd->hover_obj);

#ifdef HAVE_GETTEXT
   Elm_Translate_String_Data *ts;
   EINA_INLIST_FOREACH(sd->translate_strings, ts)
     {
        if (!ts->string) continue;
        const char *s = dgettext(ts->domain, ts->string);
        sd->on_translate = EINA_TRUE;
        elm_widget_part_text_set(obj, ts->id, s);
        sd->on_translate = EINA_FALSE;
     }
#endif
   efl_event_callback_legacy_call(obj, EFL_UI_WIDGET_EVENT_LANGUAGE_CHANGED, NULL);
}

EOLIAN static void
_efl_ui_widget_access_info_set(Eo *obj EINA_UNUSED, Elm_Widget_Smart_Data *sd, const char *txt)
{
   eina_stringshare_replace(&sd->access_info, txt);
}

EOLIAN static const char*
_efl_ui_widget_access_info_get(const Eo *obj EINA_UNUSED, Elm_Widget_Smart_Data *sd)
{
   return sd->access_info;
}

//TIZEN_ONLY(20160822): When atspi mode is dynamically switched on/off,
//register/unregister access objects accordingly.
EAPI Eina_Bool
elm_widget_screen_reader(Evas_Object *obj,
                  Eina_Bool is_screen_reader)
{
   const Eina_List *l;
   Evas_Object *child;
   Eina_Bool ret = EINA_TRUE;

   API_ENTRY return EINA_FALSE;
   EINA_LIST_FOREACH(sd->subobjs, l, child)
     {
        if (elm_widget_is(child))
          ret &= elm_widget_screen_reader(child, is_screen_reader);
     }
   efl_ui_widget_screen_reader(obj, is_screen_reader);

   return ret;
}

EOLIAN static void
_efl_ui_widget_screen_reader(Eo *obj EINA_UNUSED, Elm_Widget_Smart_Data *_pd EINA_UNUSED, Eina_Bool is_screen_reader EINA_UNUSED)
{
}
//

//TIZEN_ONLY(20170621) handle atspi proxy connection at runtime
EAPI Eina_Bool
elm_widget_atspi(Evas_Object *obj, Eina_Bool is_atspi)
{
   Eina_List *l, *children;
   Evas_Object *child;
   Eina_Bool ret = EINA_TRUE;

   API_ENTRY return EINA_FALSE;
   children = efl_access_object_access_children_get(obj);
   EINA_LIST_FOREACH(children, l, child)
     {
        ret &= elm_widget_atspi(child, is_atspi);
     }
   efl_ui_widget_atspi(obj, is_atspi);

   return ret;
}

EOLIAN static void
_efl_ui_widget_atspi(Eo *obj EINA_UNUSED, Elm_Widget_Smart_Data *_pd EINA_UNUSED, Eina_Bool is_atspi EINA_UNUSED)
{
}
//
//

EAPI void
elm_widget_scroll_hold_push(Efl_Ui_Widget *obj)
{
   efl_ui_widget_scroll_hold_push(obj);
}

EAPI void
elm_widget_scroll_hold_pop(Efl_Ui_Widget *obj)
{
   efl_ui_widget_scroll_hold_pop(obj);
}

EAPI void
elm_widget_scroll_freeze_push(Efl_Ui_Widget *obj)
{
   efl_ui_widget_scroll_freeze_push(obj);
}

EAPI void
elm_widget_scroll_freeze_pop(Efl_Ui_Widget *obj)
{
   efl_ui_widget_scroll_freeze_pop(obj);
}

EAPI Elm_Theme *
elm_widget_theme_get(const Evas_Object *obj)
{
   Elm_Widget_Smart_Data *sd = efl_data_scope_safe_get(obj, MY_CLASS);
   if (!sd) return NULL;

   if (!sd->theme)
     {
        if (sd->parent_obj && elm_widget_is(sd->parent_obj))
           return elm_widget_theme_get(sd->parent_obj);
        else return NULL;
     }
   return sd->theme;
}

EOLIAN static Eina_Error
_efl_ui_widget_style_set(Eo *obj, Elm_Widget_Smart_Data *sd, const char *style)
{
   if (!elm_widget_is_legacy(obj) && efl_finalized_get(obj))
     {
        ERR("Efl.Ui.Widget.style can only be set before finalize!");
        return EFL_UI_THEME_APPLY_ERROR_GENERIC;
     }

   if (eina_stringshare_replace(&sd->style, style))
      return elm_widget_theme(obj);

   return EFL_UI_THEME_APPLY_ERROR_NONE;
}

EOLIAN static const char*
_efl_ui_widget_style_get(const Eo *obj EINA_UNUSED, Elm_Widget_Smart_Data *sd)
{
   const char *ret;
   ret = "default";
   if (sd->style) ret = sd->style;

   return ret;
}

EAPI void
elm_widget_tooltip_add(Eo *obj, Elm_Tooltip *tt)
{
   Elm_Widget_Smart_Data *sd = efl_data_scope_safe_get(obj, MY_CLASS);
   if (!sd) return;

   sd->tooltips = eina_list_append(sd->tooltips, tt);
}

EAPI void
elm_widget_tooltip_del(Eo *obj, Elm_Tooltip *tt)
{
   Elm_Widget_Smart_Data *sd = efl_data_scope_safe_get(obj, MY_CLASS);
   if (!sd) return;

   sd->tooltips = eina_list_remove(sd->tooltips, tt);
}

EAPI void
elm_widget_cursor_add(Eo *obj, Elm_Cursor *cur)
{
   Elm_Widget_Smart_Data *sd = efl_data_scope_safe_get(obj, MY_CLASS);
   if (!sd) return;

   sd->cursors = eina_list_append(sd->cursors, cur);
}

EAPI void
elm_widget_cursor_del(Eo *obj, Elm_Cursor *cur)
{
   Elm_Widget_Smart_Data *sd = efl_data_scope_safe_get(obj, MY_CLASS);
   if (!sd) return;

   sd->cursors = eina_list_remove(sd->cursors, cur);
}

EAPI void
elm_widget_scroll_lock_set(Eo *obj, Efl_Ui_Scroll_Block block)
{
   Elm_Widget_Smart_Data *sd = efl_data_scope_safe_get(obj, MY_CLASS);
   Eina_Bool lx, ly;

   if (!sd) return;
   lx = !!(block & EFL_UI_SCROLL_BLOCK_HORIZONTAL);
   ly = !!(block & EFL_UI_SCROLL_BLOCK_VERTICAL);
   if (sd->scroll_x_locked != lx)
     {
        sd->scroll_x_locked = lx;
        _propagate_x_drag_lock(obj, lx ? 1 : -1);
     }
   if (sd->scroll_y_locked != ly)
     {
        sd->scroll_y_locked = ly;
        _propagate_y_drag_lock(obj, ly ? 1 : -1);
     }
}

EAPI Efl_Ui_Scroll_Block
elm_widget_scroll_lock_get(const Eo *obj)
{
   Elm_Widget_Smart_Data *sd = efl_data_scope_safe_get(obj, MY_CLASS);
   Efl_Ui_Scroll_Block block = EFL_UI_SCROLL_BLOCK_NONE;

   if (!sd) return block;
   if (sd->scroll_x_locked) block |= EFL_UI_SCROLL_BLOCK_HORIZONTAL;
   if (sd->scroll_y_locked) block |= EFL_UI_SCROLL_BLOCK_VERTICAL;

   return block;
}

EAPI int
elm_widget_scroll_child_locked_x_get(const Eo *obj)
{
   Elm_Widget_Smart_Data *sd = efl_data_scope_safe_get(obj, MY_CLASS);
   if (!sd) return EINA_FALSE;
   return sd->child_drag_x_locked;
}

EAPI int
elm_widget_scroll_child_locked_y_get(const Eo *obj)
{
   Elm_Widget_Smart_Data *sd = efl_data_scope_safe_get(obj, MY_CLASS);
   if (!sd) return EINA_FALSE;
   return sd->child_drag_y_locked;
}

EAPI Eina_Error
elm_widget_theme_object_set(Evas_Object *obj, Evas_Object *edj, const char *wname, const char *welement, const char *wstyle)
{
   Elm_Widget_Smart_Data *sd = efl_data_scope_safe_get(obj, MY_CLASS);
   if (!sd) return EFL_UI_THEME_APPLY_ERROR_GENERIC;

   if (eina_streq(welement, "base"))
     welement = NULL;
   if (eina_streq(wstyle, "default"))
     wstyle = NULL;
   return _elm_theme_object_set(obj, edj, wname, welement, wstyle);
}

static void
_convert(Efl_Dbg_Info *info, Eina_Iterator *ptr_list)
{
   void *p;
   int i = 0;

   EINA_ITERATOR_FOREACH(ptr_list, p)
     {
        char name[100];

        snprintf(name, sizeof(name), "Candidate %d", i);

        EFL_DBG_INFO_APPEND(info, name, EINA_VALUE_TYPE_UINT64, p);
        i++;
     }

   eina_iterator_free(ptr_list);
}

EOLIAN static void
_efl_ui_widget_efl_object_dbg_info_get(Eo *eo_obj, Elm_Widget_Smart_Data *_pd EINA_UNUSED, Efl_Dbg_Info *root)
{
   efl_dbg_info_get(efl_super(eo_obj, MY_CLASS), root);
   Efl_Ui_Focus_Relations *rel = NULL;
   Efl_Dbg_Info *focus, *group = EFL_DBG_INFO_LIST_APPEND(root, MY_CLASS_NAME);

   EFL_DBG_INFO_APPEND(group, "Wid-Type", EINA_VALUE_TYPE_STRING, elm_widget_type_get(eo_obj));
   EFL_DBG_INFO_APPEND(group, "Style", EINA_VALUE_TYPE_STRING, elm_widget_style_get(eo_obj));
   EFL_DBG_INFO_APPEND(group, "Layer", EINA_VALUE_TYPE_INT,
         (int) evas_object_layer_get(eo_obj));
   EFL_DBG_INFO_APPEND(group, "Scale", EINA_VALUE_TYPE_DOUBLE,
         evas_object_scale_get(eo_obj));
   EFL_DBG_INFO_APPEND(group, "Has focus", EINA_VALUE_TYPE_CHAR,
         elm_object_focus_get(eo_obj));
   EFL_DBG_INFO_APPEND(group, "Can focus", EINA_VALUE_TYPE_CHAR,
         elm_widget_can_focus_get(eo_obj));
   EFL_DBG_INFO_APPEND(group, "Disabled", EINA_VALUE_TYPE_CHAR,
         elm_widget_disabled_get(eo_obj));
   EFL_DBG_INFO_APPEND(group, "Mirrored", EINA_VALUE_TYPE_CHAR,
         efl_ui_mirrored_get(eo_obj));
   EFL_DBG_INFO_APPEND(group, "Tree Unfocusable", EINA_VALUE_TYPE_CHAR,
         elm_widget_tree_unfocusable_get(eo_obj));
   EFL_DBG_INFO_APPEND(group, "Automatic mirroring", EINA_VALUE_TYPE_CHAR,
         efl_ui_mirrored_automatic_get(eo_obj));

   rel = efl_ui_focus_manager_fetch(_pd->focus.manager, eo_obj);
   if (rel)
     {
        focus = EFL_DBG_INFO_LIST_APPEND(group, "Focus");

        EFL_DBG_INFO_APPEND(focus, "logical", EINA_VALUE_TYPE_CHAR, rel->logical );
        EFL_DBG_INFO_APPEND(focus, "manager", EINA_VALUE_TYPE_UINT64, _pd->focus.manager);
        EFL_DBG_INFO_APPEND(focus, "parent", EINA_VALUE_TYPE_UINT64, rel->parent);
        EFL_DBG_INFO_APPEND(focus, "next", EINA_VALUE_TYPE_UINT64 , rel->next);
        EFL_DBG_INFO_APPEND(focus, "prev", EINA_VALUE_TYPE_UINT64 , rel->prev);

        EFL_DBG_INFO_APPEND(focus, "redirect", EINA_VALUE_TYPE_UINT64 , rel->redirect);

#define ADD_PTR_LIST(name) \
        Efl_Dbg_Info* name = EFL_DBG_INFO_LIST_APPEND(focus, ""#name""); \
        _convert(name, eina_list_iterator_new(rel->name));

        ADD_PTR_LIST(top)
        ADD_PTR_LIST(down)
        ADD_PTR_LIST(right)
        ADD_PTR_LIST(left)

#undef ADD_PTR_LIST

     }

   //if thats a focus manager, give useful information like the border elements
   if (efl_isa(eo_obj, EFL_UI_FOCUS_MANAGER_INTERFACE))
     {
        Efl_Dbg_Info *border;

        focus = EFL_DBG_INFO_LIST_APPEND(group, "Focus Manager");
        border = EFL_DBG_INFO_LIST_APPEND(focus, "Border Elements");

        _convert(border,
          efl_ui_focus_manager_border_elements_get(eo_obj)
        );

        EFL_DBG_INFO_APPEND(focus, "redirect", EINA_VALUE_TYPE_UINT64,
          efl_ui_focus_manager_redirect_get(eo_obj));
     }
}

EAPI Eina_Bool
elm_widget_is_check(const Evas_Object *obj)
{
   static int abort_on_warn = -1;
   if (elm_widget_is(obj))
     return EINA_TRUE;

   ERR("Passing Object: %p.", obj);
   if (abort_on_warn == -1)
     {
        if (getenv("ELM_ERROR_ABORT")) abort_on_warn = 1;
        else abort_on_warn = 0;
     }
   if (abort_on_warn == 1) abort();
   return EINA_FALSE;
}

/* If you changed a legacy widget's class name,
 * please update the "legacy_type_table". */
EAPI const char *
elm_widget_type_get(const Evas_Object *obj)
{
   const char *ret;
   int i;

   API_ENTRY return NULL;

   ret = efl_class_name_get(efl_class_get(obj));

   /* If the given widget is created for legacy,
    * convert type name to legacy. */
   if (elm_widget_is_legacy(obj))
     {
        for (i = 0; legacy_type_table[i][0] ; i++)
          {
             if (eina_streq(ret, legacy_type_table[i][0]))
               return legacy_type_table[i][1];
          }
     }

   return ret;
}

EAPI Eina_Bool
elm_widget_type_check(const Evas_Object *obj,
                      const char *type,
                      const char *func)
{
   const char *provided, *expected = "(unknown)";
   static int abort_on_warn = -1;

   provided = elm_widget_type_get(obj);
   /* TODO: eventually migrate to check_ptr version */
   if (evas_object_smart_type_check(obj, type)) return EINA_TRUE;
   if (type) expected = type;
   if ((!provided) || (!provided[0]))
     {
        provided = evas_object_type_get(obj);
        if ((!provided) || (!provided[0]))
          provided = "(unknown)";
     }
   ERR("Passing Object: %p in function: %s, of type: '%s' when expecting"
       " type: '%s'", obj, func, provided, expected);
   if (abort_on_warn == -1)
     {
        if (getenv("ELM_ERROR_ABORT")) abort_on_warn = 1;
        else abort_on_warn = 0;
     }
   if (abort_on_warn == 1) abort();
   return EINA_FALSE;
}

/** @internal */
EAPI Evas_Object *
elm_widget_name_find(const Eo *obj, const char *name, int recurse)
{
   Eina_List *l;
   Evas_Object *child;
   const char *s;
   INTERNAL_ENTRY NULL;

   if (!name) return NULL;
   if (!_elm_widget_is(obj)) return NULL;
   EINA_LIST_FOREACH(sd->subobjs, l, child)
     {
        s = evas_object_name_get(child);
        if ((s) && (!strcmp(s, name))) return child;
        if ((recurse != 0) &&
            ((child = elm_widget_name_find(child, name, recurse - 1))))
          return child;
     }
   if (sd->hover_obj)
     {
        s = evas_object_name_get(sd->hover_obj);
        if ((s) && (!strcmp(s, name))) return sd->hover_obj;
        if ((recurse != 0) &&
            ((child = elm_widget_name_find(sd->hover_obj, name, recurse - 1))))
          return child;
     }
   return NULL;
}

/**
 * @internal
 *
 * Split string in words
 *
 * @param str Source string
 * @return List of const words
 *
 * @see elm_widget_stringlist_free()
 * @ingroup Widget
 */
EAPI Eina_List *
elm_widget_stringlist_get(const char *str)
{
   Eina_List *list = NULL;
   const char *s, *b;
   if (!str) return NULL;
   for (b = s = str; 1; s++)
     {
        if ((*s == ' ') || (!*s))
          {
             char *t = malloc(s - b + 1);
             if (t)
               {
                  strncpy(t, b, s - b);
                  t[s - b] = 0;
                  list = eina_list_append(list, eina_stringshare_add(t));
                  free(t);
               }
             b = s + 1;
          }
        if (!*s) break;
     }
   return list;
}

EAPI void
elm_widget_stringlist_free(Eina_List *list)
{
   const char *s;
   EINA_LIST_FREE(list, s)
     eina_stringshare_del(s);
}
//TIZEN_ONLY(20180607): Restore legacy focus
EOLIAN static void
_efl_ui_widget_focus_hide_handle(Eo *obj, Elm_Widget_Smart_Data *_pd EINA_UNUSED)
{
   if (!_elm_widget_is(obj))
     return;
   _if_focused_revert(obj, EINA_TRUE);
}
//
/* internal */
EAPI void
elm_widget_focus_mouse_up_handle(Eo *obj)
{
   Elm_Widget_Smart_Data *pd = efl_data_scope_get(obj, MY_CLASS);

   if (!_is_focusable(obj)) return;

   //TIZEN_ONLY(20180607): Restore legacy focus
   if (elm_widget_is_legacy(obj))
     {
        Evas_Object *o = obj;
        do
          {
             if (_elm_widget_is(o)) break;
             o = evas_object_smart_parent_get(o);
          }
        while (o);

        efl_ui_widget_focus_mouse_up_handle(o);
        return;
     }
   //
   if (pd->focus.manager && !pd->focus.logical)
     {
        efl_ui_focus_util_focus(obj);
     }
}

EOLIAN static void
_efl_ui_widget_focus_tree_unfocusable_handle(Eo *obj, Elm_Widget_Smart_Data *_pd EINA_UNUSED)
{
   if (!elm_widget_parent_get(obj))
     efl_ui_widget_focused_object_clear(obj);
   else
     _if_focused_revert(obj, EINA_TRUE);
}

/*
 * @internal
 *
 * Get the focus highlight geometry of a widget.
 *
 * @param obj Widget object for the focus highlight
 * @param x Focus highlight x coordinate
 * @param y Focus highlight y coordinate
 * @param w Focus highlight object width
 * @param h Focus highlight object height
 * @param is_next @c EINA_TRUE if this request is for the new focused object,
 * @c EINA_FALSE if this request is for the previously focused object. This
 * information becomes important when the focus highlight is changed inside one
 * widget.
 *
 * @ingroup Widget
 */
/*
 * @internal
 *
 * Get the 'focus_part' geometry if there is any
 *
 * This queries if there is a 'focus_part' request from the edc style. If edc
 * style offers 'focus_part' edje data item, this function requests for the
 * geometry of a specific part which is described in 'focus_part' edje data.
 *
 * @param obj Widget object for the focus highlight
 * @param x Focus highlight x coordinate
 * @param y Focus highlight y coordinate
 * @param w Focus highlight object width
 * @param h Focus highlight object height
 *
 * x, y, w, h already contain the object's geometry. If there is a 'focus_part'
 * support, these values will be updated accordingly or the values will be
 * remained as they were.
 *
 * @ingroup Widget
 */
EAPI void
elm_widget_focus_highlight_focus_part_geometry_get(const Evas_Object *obj,
                                                   Evas_Coord *x,
                                                   Evas_Coord *y,
                                                   Evas_Coord *w,
                                                   Evas_Coord *h)
{
   Evas_Coord tx = 0, ty = 0, tw = 0, th = 0;
   const char *target_hl_part = NULL;
   const Evas_Object *edje_obj = NULL;

   if (obj && efl_isa(obj, EFL_CANVAS_LAYOUT_CLASS))
     {
        edje_obj = obj;
        if (!(target_hl_part = edje_object_data_get(edje_obj, "focus_part")))
          return;
     }
   else if (obj && efl_isa(obj, EFL_UI_LAYOUT_BASE_CLASS))
     {
        edje_obj = elm_layout_edje_get(obj);
        if (!(target_hl_part = elm_layout_data_get(obj, "focus_part")))
          return;
     }
   else
     return;

  edje_object_part_geometry_get(edje_obj, target_hl_part,
                                &tx, &ty, &tw, &th);
  *x += tx;
  *y += ty;
  if (tw != *w) *w = tw;
  if (th != *h) *h = th;
}

EOLIAN static Eina_Rect
_efl_ui_widget_focus_highlight_geometry_get(const Eo *obj, Elm_Widget_Smart_Data *sd)
{
   Evas_Coord ox = 0, oy = 0, ow = 0, oh = 0;
   Evas_Object *scroller = (Evas_Object *)obj;
   Eina_Rect r = {};

   evas_object_geometry_get(obj, &r.x, &r.y, &r.w, &r.h);
   elm_widget_focus_highlight_focus_part_geometry_get(sd->resize_obj, &r.x, &r.y, &r.w, &r.h);

   if (_elm_config->focus_autoscroll_mode != ELM_FOCUS_AUTOSCROLL_MODE_BRING_IN)
     return r;

   while (scroller)
     {
        if (_elm_scrollable_is(scroller))
          {
             elm_interface_scrollable_content_viewport_geometry_get(scroller, &ox, &oy, &ow, &oh);

             if (r.y < oy)
               r.y = oy;
             else if ((oy + oh) < (r.y + r.h))
               r.y = (oy + oh - r.h);
             else if (r.x < ox)
               r.x = ox;
             else if ((ox + ow) < (r.x + r.w))
               r.x = (ox + ow - r.w);

             break;
          }
        scroller = elm_widget_parent_get(scroller);
     }

   return r;
}

EAPI void
elm_widget_activate(Evas_Object *obj, Efl_Ui_Activate act)
{
   Evas_Object *parent;
   Eina_Bool ret;

   ELM_WIDGET_CHECK(obj);

   ret = EINA_FALSE;

   ret = efl_ui_widget_on_access_activate(obj, act);

   if (ret) return;

   parent = elm_widget_parent_get(obj);
   if (parent)
     elm_widget_activate(parent, act);

   return;
}

/**
 * @internal
 *
 * Sets the widget and child widget's Evas_Display_Mode.
 *
 * @param obj The widget.
 * @param dispmode Evas_Display_Mode to set widget's mode.
 *
 * Widgets are resized by several reasons.
 * Evas_Display_Mode can help for widgets to get one more reason of resize.
 * For example, elm conform widget resizes it's contents when keypad state changed.
 * After keypad showing, conform widget can change child's Evas_Display_Mode.
 * @ingroup Widget
 */
/* Legacy only */
EAPI void
elm_widget_display_mode_set(Evas_Object *obj, Evas_Display_Mode dispmode)
{
   Evas_Display_Mode prev_dispmode;
   Evas_Object *child;
   Eina_List *l;

   API_ENTRY return;
   prev_dispmode = evas_object_size_hint_display_mode_get(obj);

   if ((prev_dispmode == dispmode) ||
       (prev_dispmode == EVAS_DISPLAY_MODE_DONT_CHANGE)) return;

   evas_object_size_hint_display_mode_set(obj, dispmode);

   EINA_LIST_FOREACH (sd->subobjs, l, child)
     {
        if (elm_widget_is(child))
          elm_widget_display_mode_set(child, dispmode);
     }
}

/**
 * @internal
 *
 * Returns the widget's focus move policy.
 *
 * @param obj The widget.
 * @return focus move policy of the object.
 *
 **/
EOLIAN static Efl_Ui_Focus_Move_Policy
_efl_ui_widget_focus_move_policy_get(const Eo *obj EINA_UNUSED, Elm_Widget_Smart_Data *sd)
{
   return sd->focus_move_policy;
}

/**
 * @internal
 *
 * Sets the widget's focus move policy.
 *
 * @param obj The widget.
 * @param policy Elm_Focus_Move_Policy to set object's focus move policy.
 */

EOLIAN static void
_efl_ui_widget_focus_move_policy_set(Eo *obj EINA_UNUSED, Elm_Widget_Smart_Data *sd, Efl_Ui_Focus_Move_Policy policy)
{
   if (sd->focus_move_policy == (Elm_Focus_Move_Policy)policy) return;
   sd->focus_move_policy = policy;
}

/**
 * Returns the widget's focus_move_policy mode setting.
 *
 * @param obj The widget.
 * @return focus_move_policy mode setting of the object.
 *
 **/
EOLIAN static Eina_Bool
_efl_ui_widget_focus_move_policy_automatic_get(const Eo *obj EINA_UNUSED, Elm_Widget_Smart_Data *sd)
{
   return sd->focus_move_policy_auto_mode;
}

/**
 * @internal
 *
 * Sets the widget's focus_move_policy mode setting.
 * When widget in automatic mode, it follows the system focus_move_policy mode set by
 * elm_config_focus_move_policy_set().
 * @param obj The widget.
 * @param automatic EINA_TRUE for auto focus_move_policy mode. EINA_FALSE for manual.
 */
EOLIAN static void
_efl_ui_widget_focus_move_policy_automatic_set(Eo *obj, Elm_Widget_Smart_Data *sd, Eina_Bool automatic)
{
   if (sd->focus_move_policy_auto_mode != automatic)
     {
        sd->focus_move_policy_auto_mode = automatic;

        if (automatic)
          {
             efl_ui_widget_focus_move_policy_set(obj, elm_config_focus_move_policy_get());
          }
     }
}

/**
 * @internal
 *
 * Sets the klass name of a widget.
 * @param obj The widget.
 * @param name Name of the klass to use.
 * @return Whether the name was different and thus replaced.
 */
EAPI Eina_Bool
elm_widget_theme_klass_set(Evas_Object *obj, const char *name)
{
   Elm_Widget_Smart_Data *pd = efl_data_scope_safe_get(obj, MY_CLASS);
   if (!pd) return EINA_FALSE;

   return eina_stringshare_replace(&(pd->klass), name);
}

/**
 * @internal
 *
 * Gets the klass name of a widget.
 * @param obj The widget.
 * @return The current klass name of internal canvas object.
 */
EAPI const char *
elm_widget_theme_klass_get(const Evas_Object *obj)
{
   Elm_Widget_Smart_Data *pd = efl_data_scope_safe_get(obj, MY_CLASS);
   if (!pd) return NULL;

   return (const char *)pd->klass;
}

/**
 * @internal
 *
 * Sets the element name of a widget.
 *
 * @param obj The widget.
 * @param name Name of the element to use.
 * @return Whether the name was different and thus replaced.
 */
EAPI Eina_Bool
elm_widget_theme_element_set(Evas_Object *obj, const char *name)
{
   Elm_Widget_Smart_Data *pd = efl_data_scope_safe_get(obj, MY_CLASS);
   if (!pd) return EINA_FALSE;

   if (eina_streq(name, "base"))
     name = NULL;

   return eina_stringshare_replace(&(pd->group), name);
}

/**
 * @internal
 *
 * Gets the element name of a widget.
 * @param obj The widget.
 * @return The current element name of internal canvas object.
 */
EAPI const char *
elm_widget_theme_element_get(const Evas_Object *obj)
{
   Elm_Widget_Smart_Data *pd = efl_data_scope_safe_get(obj, MY_CLASS);
   if (!pd) return NULL;

   return (const char *)pd->group;
}

/**
 * @internal
 *
 * Sets the style name of a widget.
 *
 * @param obj The widget.
 * @param name Name of the style to use.
 * @return Whether the name was different and thus replaced.
 */
EAPI Eina_Bool
elm_widget_theme_style_set(Evas_Object *obj, const char *name)
{
   Elm_Widget_Smart_Data *pd = efl_data_scope_safe_get(obj, MY_CLASS);
   if (!pd) return EINA_FALSE;

   if (eina_streq(name, "default"))
     name = NULL;

   return eina_stringshare_replace(&(pd->style), name);
}

/**
 * @internal
 *
 * Gets the style name of a widget.
 * @param obj The widget.
 * @return The current style name of internal canvas object.
 */
EAPI const char *
elm_widget_theme_style_get(const Evas_Object *obj)
{
   Elm_Widget_Smart_Data *pd = efl_data_scope_safe_get(obj, MY_CLASS);
   if (!pd) return NULL;

   return (const char *)pd->style;
}

/**
 * @internal
 *
 * Register sub object as a group of a widget and re-apply its theme.
 * @param obj The widget.
 * @param component A sub object to be added as an element of the widget.
 * @param name An element name of sub object.
 * @return Whether the style was successfully applied or not.
 */
EAPI Eina_Error
elm_widget_element_update(Evas_Object *obj, Evas_Object *component, const char *name)
{
   Eina_Error ret = EFL_UI_THEME_APPLY_ERROR_NONE;
   Eina_Bool changed = EINA_FALSE;
   const char *obj_group;
   Eina_Stringshare *group;

   obj_group = elm_widget_theme_element_get(obj);
   if (!obj_group)
     group = eina_stringshare_add(name);
   else
     group = eina_stringshare_printf("%s/%s", elm_widget_theme_element_get(obj), name);
   if (efl_isa(component, EFL_UI_WIDGET_CLASS))
     {
        changed |= elm_widget_theme_klass_set(component, elm_widget_theme_klass_get(obj));
        changed |= elm_widget_theme_element_set(component, (const char *)group);
        changed |= elm_widget_theme_style_set(component, elm_widget_theme_style_get(obj));
        if (changed)
          ret = efl_ui_widget_theme_apply(component);
     }
   else
     {
        ret = elm_widget_theme_object_set(obj, component,
                                   elm_widget_theme_klass_get(obj),
                                   (const char *)group,
                                   elm_widget_theme_style_get(obj));
     }
   eina_stringshare_del(group);

   return ret;
}

static void
_track_obj_del(void *data, Evas *e, Evas_Object *obj, void *event_info);

static void
_track_obj_update(Evas_Object *track, Evas_Object *obj)
{
   //Geometry
   Evas_Coord x, y, w, h;
   evas_object_geometry_get(obj, &x, &y, &w, &h);
   evas_object_move(track, x, y);
   evas_object_resize(track, w, h);

   //Visibility
   if (evas_object_visible_get(obj)) evas_object_show(track);
   else evas_object_hide(track);
}

static void
_track_obj_view_update(void *data, const Efl_Event *event)
{
   Elm_Widget_Item_Data *item = data;
   _track_obj_update(item->track_obj, event->object);
}

static void
_track_obj_view_del(void *data, const Efl_Event *event);

EFL_CALLBACKS_ARRAY_DEFINE(tracker_callbacks,
                          { EFL_GFX_ENTITY_EVENT_SIZE_CHANGED, _track_obj_view_update },
                          { EFL_GFX_ENTITY_EVENT_POSITION_CHANGED, _track_obj_view_update },
                          { EFL_GFX_ENTITY_EVENT_VISIBILITY_CHANGED, _track_obj_view_update },
                          { EFL_EVENT_DEL, _track_obj_view_del });

static void
_track_obj_view_del(void *data, const Efl_Event *event EINA_UNUSED)
{
   Elm_Widget_Item_Data *item = data;

   while (evas_object_ref_get(item->track_obj) > 0)
     evas_object_unref(item->track_obj);

   evas_object_event_callback_del(item->track_obj, EVAS_CALLBACK_DEL,
                                  _track_obj_del);
   evas_object_del(item->track_obj);
   item->track_obj = NULL;
}

static void
_track_obj_del(void *data, Evas *e EINA_UNUSED,
                    Evas_Object *obj EINA_UNUSED, void *event_info EINA_UNUSED)
{
   Elm_Widget_Item_Data *item = data;
   item->track_obj = NULL;

   if (!item->view) return;

   efl_event_callback_array_del(item->view, tracker_callbacks(), item);
}

static void
_elm_widget_item_signal_cb(void *data, Evas_Object *obj EINA_UNUSED, const char *emission,
                           const char *source)
{
   Elm_Widget_Item_Signal_Data *wisd = data;
   wisd->func(wisd->data, wisd->item, emission, source);
}

static void *
_elm_widget_item_signal_callback_list_get(Elm_Widget_Item_Data *item, Eina_List *position)
{
   Elm_Widget_Item_Signal_Data *wisd = eina_list_data_get(position);
   void *data;

   item->signals = eina_list_remove_list(item->signals, position);
   data = wisd->data;

   if (_elm_widget_is(item->view))
     elm_object_signal_callback_del(item->view,
                                    wisd->emission, wisd->source,
                                    _elm_widget_item_signal_cb);
   else if (efl_isa(item->view, EFL_CANVAS_LAYOUT_CLASS))
     edje_object_signal_callback_del_full(item->view,
                                          wisd->emission, wisd->source,
                                          _elm_widget_item_signal_cb, wisd);

   eina_stringshare_del(wisd->emission);
   eina_stringshare_del(wisd->source);
   free(wisd);

   return data;
}

#define ERR_NOT_SUPPORTED(item, method)  ERR("%s does not support %s API.", elm_widget_type_get(item->widget), method);

static void
_efl_del_cb(void *data EINA_UNUSED, const Efl_Event *event)
{
   Elm_Widget_Item_Data *item = efl_data_scope_get(event->object, ELM_WIDGET_ITEM_CLASS);
   ELM_WIDGET_ITEM_CHECK_OR_RETURN(item);
   if (item->del_func)
      item->del_func((void *) WIDGET_ITEM_DATA_GET(event->object), item->widget, item->eo_obj);
}

/**
 * @internal
 *
 * Allocate a new Elm_Widget_Item-derived structure.
 *
 * The goal of this structure is to provide common ground for actions
 * that a widget item have, such as the owner widget, callback to
 * notify deletion, data pointer and maybe more.
 *
 * @param widget the owner widget that holds this item, must be an elm_widget!
 * @param alloc_size any number greater than sizeof(Elm_Widget_Item) that will
 *        be used to allocate memory.
 *
 * @return allocated memory that is already zeroed out, or NULL on errors.
 *
 * @see elm_widget_item_new() convenience macro.
 * @see elm_widget_item_del() to release memory.
 * @ingroup Widget
 */
EOLIAN static Eo *
_elm_widget_item_efl_object_constructor(Eo *eo_item, Elm_Widget_Item_Data *item)
{
   Evas_Object *widget;
   widget = efl_parent_get(eo_item);

   if (!_elm_widget_is(widget))
     {
        ERR("Failed");
        return NULL;
     }

   eo_item = efl_constructor(efl_super(eo_item, ELM_WIDGET_ITEM_CLASS));

   EINA_MAGIC_SET(item, ELM_WIDGET_ITEM_MAGIC);

   item->widget = widget;
   item->eo_obj = eo_item;
   //TIZEN_ONLY(20170717) : expose highlight information on atspi
   item->can_highlight = EINA_TRUE;
   //
   efl_event_callback_add(eo_item, EFL_EVENT_DEL, _efl_del_cb, NULL);

   return eo_item;
}

EOLIAN static void
_elm_widget_item_efl_object_destructor(Eo *eo_item, Elm_Widget_Item_Data *item)
{
   Elm_Translate_String_Data *ts;

   ELM_WIDGET_ITEM_CHECK_OR_RETURN(item);

   eina_stringshare_del(item->style);
   eina_stringshare_del(item->access_info);
   eina_stringshare_del(item->accessible_name);

   while (item->signals)
     _elm_widget_item_signal_callback_list_get(item, item->signals);

   while (item->translate_strings)
     {
        ts = EINA_INLIST_CONTAINER_GET(item->translate_strings,
                                       Elm_Translate_String_Data);
        eina_stringshare_del(ts->id);
        eina_stringshare_del(ts->domain);
        eina_stringshare_del(ts->string);
        item->translate_strings = eina_inlist_remove(item->translate_strings,
                                                     item->translate_strings);
        free(ts);
     }
   eina_hash_free(item->labels);

   //TIZEN_ONLY : elm_widget_item: add at-spi name setter
   efl_access_object_description_set(eo_item, NULL);
   efl_access_object_i18n_name_set(eo_item, NULL);

   efl_access_object_description_cb_set(eo_item, NULL, NULL);
   efl_access_object_name_cb_set(eo_item, NULL, NULL);
   //
   //TIZEN_ONLY(20170405) Add gesture method to accessible interface
   efl_access_object_gesture_cb_set(eo_item, NULL, NULL);
   //
   //TIZEN_ONLY : elm_widget_item: add at-spi name setter
   efl_access_object_translation_domain_set(eo_item, NULL);
   efl_access_object_relationships_clear(eo_item);
   //

   efl_access_object_attributes_clear(eo_item);

   // TIZEN_ONLY(20150709) : atspi relations api
   if (item->atspi_custom_relations)
     efl_access_relation_set_free(&item->atspi_custom_relations);
   //

   //TIZEN_ONLY(20150731) : add i18n support for name and description
   if (item->atspi_translation_domain)
     eina_stringshare_del(item->atspi_translation_domain);
   ///

   //TIZEN_ONLY(20150713) : add widget_item name setter
   if (item->name)
     eina_stringshare_del(item->name);
   //

   /***********************************************************************************
    * TIZEN_ONLY_FEATURE: apply Tizen's color_class features.                         *
    ***********************************************************************************/
   if (item->color_classes)
     ELM_SAFE_FREE(item->color_classes, eina_hash_free);
   /*******
    * END *
    *******/

   EINA_MAGIC_SET(item, EINA_MAGIC_NONE);

   efl_destructor(efl_super(eo_item, ELM_WIDGET_ITEM_CLASS));
}

/**
 * @internal
 *
 * Releases widget item memory, calling back item_del_pre_hook() and
 * item_del_cb() if they exist.
 *
 * @param item a valid #Elm_Widget_Item to be deleted.
 *
 * If there is an Elm_Widget_Item::del_cb, then it will be called prior
 * to memory release. Note that elm_widget_item_pre_notify_del() calls
 * this function and then unset it, thus being useful for 2 step
 * cleanup whenever the del_cb may use any of the data that must be
 * deleted from item.
 *
 * The Elm_Widget_Item::view will be deleted (evas_object_del()) if it
 * is presented!
 *
 * Note that if item_del_pre_hook() returns @c EINA_TRUE, item free will be
 * deferred, or item will be freed here if it returns @c EINA_FALSE.
 *
 * @see elm_widget_item_del() convenience macro.
 * @ingroup Widget
 */
EOLIAN static void
_elm_widget_item_efl_object_invalidate(Eo *eo_item, Elm_Widget_Item_Data *item)
{
   Evas_Object *view;

   //Widget item delete callback
   elm_wdg_item_del_pre(item->eo_obj);

   view = item->view;
   if (item->view) efl_wref_del(item->view, &item->view);
   // FIXME: Is view an Efl.Ui or a legacy object ?
   evas_object_del(view);
   item->view = NULL;

   efl_invalidate(efl_super(eo_item, ELM_WIDGET_ITEM_CLASS));
}

EOLIAN static void
_elm_widget_item_del_pre(Eo *eo_item EINA_UNUSED, Elm_Widget_Item_Data *item EINA_UNUSED)
{
}

/**
 * @internal
 *
 * Notify object will be deleted without actually deleting it.
 *
 * This function will callback Elm_Widget_Item::del_cb if it is set
 * and then unset it so it is not called twice (ie: from
 * elm_widget_item_del()).
 *
 * @param item a valid #Elm_Widget_Item to be notified
 * @see elm_widget_item_pre_notify_del() convenience macro.
 * @ingroup Widget
 */
EOLIAN static void
_elm_widget_item_pre_notify_del(Eo *eo_item, Elm_Widget_Item_Data *item)
{
   ELM_WIDGET_ITEM_CHECK_OR_RETURN(item);
   if (!item->del_func) return;
   item->del_func((void *)WIDGET_ITEM_DATA_GET(eo_item), item->widget, item->eo_obj);
   item->del_func = NULL;
}

/**
 * @internal
 *
 * Set the function to notify when item is being deleted.
 *
 * This function will complain if there was a callback set already,
 * however it will set the new one.
 *
 * The callback will be called from elm_widget_item_pre_notify_del()
 * or elm_widget_item_del() will be called with:
 *   - data: the Elm_Widget_Item::data value.
 *   - obj: the Elm_Widget_Item::widget evas object.
 *   - event_info: the item being deleted.
 *
 * @param item a valid #Elm_Widget_Item to be notified
 * @see elm_widget_item_del_cb_set() convenience macro.
 * @ingroup Widget
 */
EOLIAN static void
_elm_widget_item_del_cb_set(Eo *eo_item EINA_UNUSED,
                            Elm_Widget_Item_Data *item,
                            Evas_Smart_Cb func)
{
   ELM_WIDGET_ITEM_CHECK_OR_RETURN(item);
   ELM_WIDGET_ITEM_RETURN_IF_ONDEL(item);

   if ((item->del_func) && (item->del_func != func))
     WRN("You're replacing a previously set del_cb %p of item %p with %p",
         item->del_func, item->eo_obj, func);

   item->del_func = func;
}

/**
 * @internal
 *
 * Get owner widget of this item.
 *
 * @param item a valid #Elm_Widget_Item to get data from.
 * @return owner widget of this item.
 * @ingroup Widget
 */
EOLIAN static Evas_Object *
_elm_widget_item_widget_get(const Eo *eo_item EINA_UNUSED, Elm_Widget_Item_Data *item)
{
   ELM_WIDGET_ITEM_CHECK_OR_RETURN(item, NULL);
   ELM_WIDGET_ITEM_RETURN_IF_ONDEL(item, NULL);

   return item->widget;
}

EAPI Eina_Bool
_elm_widget_onscreen_is(const Evas_Object *widget)
{
   Evas_Object *parent = (Evas_Object *)widget;
   Eina_Rectangle r1, r2;

   Evas *evas = evas_object_evas_get(widget);
   if (!evas) return EINA_FALSE;

   evas_object_geometry_get(widget, &r1.x, &r1.y, &r1.w, &r1.h);
   if (eina_rectangle_is_empty(&r1))
     return EINA_FALSE;

   // window does not have to check viewport and geometry
   if (efl_isa(widget, EFL_UI_WIN_CLASS))
      return EINA_TRUE;

   // check if on canvas
   evas_output_viewport_get(evas, &r2.x, &r2.y, &r2.w, &r2.h);
   if (!eina_rectangles_intersect(&r1, &r2))
     return EINA_FALSE;

   // check if inside scrollable parent viewport
   do {
      parent = elm_widget_parent_get(parent);
      if (parent && !evas_object_visible_get(parent))
        return EINA_FALSE;
      if (parent && efl_isa(parent, ELM_INTERFACE_SCROLLABLE_MIXIN))
        {
           evas_object_geometry_get(parent, &r2.x, &r2.y, &r2.w, &r2.h);
           if (!eina_rectangles_intersect(&r1, &r2))
             return EINA_FALSE;
        }
   } while (parent && (parent != elm_widget_top_get(widget)));

   return EINA_TRUE;
}

EAPI Eina_Bool
_elm_widget_item_onscreen_is(const Elm_Object_Item *item)
{
   Eina_Rectangle r1, r2;
   Elm_Widget_Item_Data *id = efl_data_scope_get(item, ELM_WIDGET_ITEM_CLASS);
   if (!id || !id->view) return EINA_FALSE;

   if (!evas_object_visible_get(id->view))
     return EINA_FALSE;

   if (!_elm_widget_onscreen_is(id->widget))
     return EINA_FALSE;

   evas_object_geometry_get(id->view, &r1.x, &r1.y, &r1.w, &r1.h);
   if (eina_rectangle_is_empty(&r1))
     return EINA_FALSE;

   evas_object_geometry_get(id->widget, &r2.x, &r2.y, &r2.w, &r2.h);
   if (!eina_rectangles_intersect(&r1, &r2))
     return EINA_FALSE;

   return EINA_TRUE;
}

const char*
_elm_widget_accessible_plain_name_get(const Evas_Object *obj, const char* name)
{
   char *accessible_plain_name;

   API_ENTRY return NULL;

   accessible_plain_name = _elm_util_mkup_to_text(name);
   eina_stringshare_del(sd->accessible_name);
   sd->accessible_name =  eina_stringshare_add(accessible_plain_name);
   free(accessible_plain_name);
   return sd->accessible_name;
}

const char*
_elm_widget_item_accessible_plain_name_get(const Elm_Object_Item *item, const char* name)
{
   char *accessible_plain_name;

   Elm_Widget_Item_Data *id = efl_data_scope_get(item, ELM_WIDGET_ITEM_CLASS);
   if (!id) return NULL;

   accessible_plain_name = _elm_util_mkup_to_text(name);
   eina_stringshare_del(id->accessible_name);
   id->accessible_name =  eina_stringshare_add(accessible_plain_name);
   free(accessible_plain_name);
   return id->accessible_name;
}

EOLIAN static Efl_Access_State_Set
_elm_widget_item_efl_access_object_state_set_get(const Eo *eo_item, Elm_Widget_Item_Data *item)
{
   Efl_Access_State_Set states = 0;

   // TIZEN_ONLY(20171114) Accessibility Highlight Frame added
   // //TIZEN_ONLY(20171108): bring HIGHLIGHT related changes
   // Evas_Object *win = elm_widget_top_get(item->widget);
   // if (win && efl_isa(win, EFL_UI_WIN_CLASS))
   //   {
   //      if (_elm_win_accessibility_highlight_get(win) == item->view)
   //        STATE_TYPE_SET(states, ELM_ATSPI_STATE_HIGHLIGHTED);
   //   }
   // STATE_TYPE_SET(states, ELM_ATSPI_STATE_HIGHLIGHTABLE);
   // //
   //

   STATE_TYPE_SET(states, EFL_ACCESS_STATE_TYPE_FOCUSABLE);

   if (elm_object_item_focus_get(eo_item))
     STATE_TYPE_SET(states, EFL_ACCESS_STATE_TYPE_FOCUSED);
   if (!elm_object_item_disabled_get(eo_item))
     {
        STATE_TYPE_SET(states, EFL_ACCESS_STATE_TYPE_ENABLED);
        STATE_TYPE_SET(states, EFL_ACCESS_STATE_TYPE_SENSITIVE);
        STATE_TYPE_SET(states, EFL_ACCESS_STATE_TYPE_VISIBLE);
     }
   if (_elm_widget_item_onscreen_is(eo_item))
     STATE_TYPE_SET(states, EFL_ACCESS_STATE_TYPE_SHOWING);

   //TIZEN_ONLY(20170207) : [ATSPI] enhance expose highlight information on atspi
   /* unrealized genlist item does not have item->view,
      and item cannot change its visibility, only widget can change the visibility */
   if (evas_object_visible_get(item->widget))
     STATE_TYPE_SET(states, EFL_ACCESS_STATE_TYPE_VISIBLE);
   //

   //TIZEN_ONLY(20170717) : expose highlight information on atspi
   if (_elm_widget_item_highlightable(eo_item) && _accessible_object_on_scroll_is(item->view))
     STATE_TYPE_SET(states, EFL_ACCESS_STATE_TYPE_HIGHLIGHTABLE);
   else
     STATE_TYPE_UNSET(states, EFL_ACCESS_STATE_TYPE_HIGHLIGHTABLE);

   if (_elm_object_accessibility_currently_highlighted_get() == (void*)item->eo_obj)
     STATE_TYPE_SET(states, EFL_ACCESS_STATE_TYPE_HIGHLIGHTED);
   //
   return states;
}

EAPI void
elm_object_item_data_set(Elm_Object_Item *it, void *data)
{
   WIDGET_ITEM_DATA_SET(it, data);
}

EAPI void *
elm_object_item_data_get(const Elm_Object_Item *it)
{
   return (void *) WIDGET_ITEM_DATA_GET(it);
}

EOLIAN static void
_elm_widget_item_disabled_set(Eo *eo_item EINA_UNUSED,
                              Elm_Widget_Item_Data *item,
                              Eina_Bool disabled)
{
   ELM_WIDGET_ITEM_CHECK_OR_RETURN(item);
   ELM_WIDGET_ITEM_RETURN_IF_ONDEL(item);

   if (item->disabled == disabled) return;
   item->disabled = !!disabled;
   elm_wdg_item_disable(item->eo_obj);
}

EOLIAN static Eina_Bool
_elm_widget_item_disabled_get(const Eo *eo_item EINA_UNUSED, Elm_Widget_Item_Data *item)
{
   ELM_WIDGET_ITEM_CHECK_OR_RETURN(item, EINA_FALSE);
   return item->disabled;
}

EOLIAN static void
_elm_widget_item_style_set(Eo *eo_item EINA_UNUSED, Elm_Widget_Item_Data *item, const char *style)
{
   eina_stringshare_replace(&item->style, style);
}

EOLIAN static const char *
_elm_widget_item_style_get(const Eo *eo_item EINA_UNUSED, Elm_Widget_Item_Data *item)
{
   return item->style;
}

EOLIAN static void
_elm_widget_item_disable(Eo *eo_item EINA_UNUSED, Elm_Widget_Item_Data *item EINA_UNUSED)
{
}

EOLIAN static void
_elm_widget_item_item_focus_set(Eo *eo_item EINA_UNUSED, Elm_Widget_Item_Data *item, Eina_Bool focused EINA_UNUSED)
{
   ERR_NOT_SUPPORTED(item, "elm_object_item_focus_set");
}

EOLIAN static Eina_Bool
_elm_widget_item_item_focus_get(const Eo *eo_item EINA_UNUSED, Elm_Widget_Item_Data *item)
{
   ERR_NOT_SUPPORTED(item, "elm_object_item_focus_get");
   return EINA_FALSE;
}

EOLIAN static void
_elm_widget_item_domain_translatable_part_text_set(Eo *eo_item EINA_UNUSED,
                                                   Elm_Widget_Item_Data *item,
                                                   const char *part,
                                                   const char *domain,
                                                   const char *label)
{
   ELM_WIDGET_ITEM_CHECK_OR_RETURN(item);
   ELM_WIDGET_ITEM_RETURN_IF_ONDEL(item);
   Elm_Translate_String_Data *ts;

   if (!label)
     {
        _part_text_translatable_set(&item->translate_strings, part, EINA_FALSE,
                                    EINA_FALSE);
     }
   else
     {
        ts = _part_text_translatable_set(&item->translate_strings, part,
                                         EINA_TRUE, EINA_FALSE);
        if (!ts) return;
        if (!ts->string) ts->string = eina_stringshare_add(label);
        else eina_stringshare_replace(&ts->string, label);
        if (!ts->domain) ts->domain = eina_stringshare_add(domain);
        else eina_stringshare_replace(&ts->domain, domain);
#ifdef HAVE_GETTEXT
        if (label[0]) label = dgettext(domain, label);
#endif
     }
   item->on_translate = EINA_TRUE;
   elm_wdg_item_part_text_set(item->eo_obj, part, label);
   item->on_translate = EINA_FALSE;
}

EOLIAN static const char *
_elm_widget_item_translatable_part_text_get(const Eo *eo_item EINA_UNUSED,
                                            Elm_Widget_Item_Data *item,
                                            const char *part)
{
   ELM_WIDGET_ITEM_CHECK_OR_RETURN(item, NULL);
   ELM_WIDGET_ITEM_RETURN_IF_ONDEL(item, NULL);

   Elm_Translate_String_Data *ts;
   ts = _translate_string_data_get(item->translate_strings, part);
   if (ts) return ts->string;
   return NULL;
}

EOLIAN static void
_elm_widget_item_domain_part_text_translatable_set(Eo *eo_item EINA_UNUSED,
                                                   Elm_Widget_Item_Data *item,
                                                   const char *part,
                                                   const char *domain,
                                                   Eina_Bool translatable)
{
   ELM_WIDGET_ITEM_CHECK_OR_RETURN(item);
   ELM_WIDGET_ITEM_RETURN_IF_ONDEL(item);
   Elm_Translate_String_Data *ts;
   const char *text;

   ts = _part_text_translatable_set(&item->translate_strings, part,
                                    translatable, EINA_TRUE);
   if (!ts) return;
   if (!ts->domain) ts->domain = eina_stringshare_add(domain);
   else eina_stringshare_replace(&ts->domain, domain);

   text = elm_wdg_item_part_text_get(item->eo_obj, part);

   if (!text || !text[0]) return;

   if (!ts->string) ts->string = eina_stringshare_add(text);

//Try to translate text since we don't know the text is already translated.
#ifdef HAVE_GETTEXT
   text = dgettext(domain, text);
#endif
   item->on_translate = EINA_TRUE;
   elm_wdg_item_part_text_set(item->eo_obj, part, text);
   item->on_translate = EINA_FALSE;
}

EOLIAN static void
_elm_widget_item_track_cancel(Eo *eo_item EINA_UNUSED, Elm_Widget_Item_Data *item)
{
   ELM_WIDGET_ITEM_CHECK_OR_RETURN(item);
   ELM_WIDGET_ITEM_RETURN_IF_ONDEL(item);

   if (!item->track_obj) return;

   while (evas_object_ref_get(item->track_obj) > 0)
     evas_object_unref(item->track_obj);

   evas_object_del(item->track_obj);
}

EOLIAN static Evas_Object *
_elm_widget_item_track(Eo *eo_item EINA_UNUSED, Elm_Widget_Item_Data *item)
{
   ELM_WIDGET_ITEM_CHECK_OR_RETURN(item, NULL);
   ELM_WIDGET_ITEM_RETURN_IF_ONDEL(item, NULL);

   if (item->track_obj)
     {
        evas_object_ref(item->track_obj);
        return item->track_obj;
     }

   if (!item->view)
     {
        WRN("view obj of the item(%p) is invalid. Please make sure the view obj is created!", item);
        return NULL;
     }

   Evas_Object *track =
      evas_object_rectangle_add(evas_object_evas_get(item->widget));
   evas_object_color_set(track, 0, 0, 0, 0);
   evas_object_pass_events_set(track, EINA_TRUE);
   _track_obj_update(track, item->view);
   evas_object_event_callback_add(track, EVAS_CALLBACK_DEL, _track_obj_del,
                                  item);

   efl_event_callback_array_add(item->view, tracker_callbacks(), item);

   evas_object_ref(track);

   item->track_obj = track;

   return track;
}

EOLIAN static void
_elm_widget_item_untrack(Eo *eo_item EINA_UNUSED, Elm_Widget_Item_Data *item)
{
   ELM_WIDGET_ITEM_CHECK_OR_RETURN(item);
   ELM_WIDGET_ITEM_RETURN_IF_ONDEL(item);

   if (!item->track_obj) return;
   evas_object_unref(item->track_obj);

   if (evas_object_ref_get(item->track_obj) == 0)
     evas_object_del(item->track_obj);
}

EOLIAN static int
_elm_widget_item_track_get(const Eo *eo_item EINA_UNUSED, Elm_Widget_Item_Data *item)
{
   ELM_WIDGET_ITEM_CHECK_OR_RETURN(item, 0);
   ELM_WIDGET_ITEM_RETURN_IF_ONDEL(item, 0);

   if (!item->track_obj) return 0;
   return evas_object_ref_get(item->track_obj);
}

typedef struct _Elm_Widget_Item_Tooltip Elm_Widget_Item_Tooltip;

struct _Elm_Widget_Item_Tooltip
{
   Elm_Widget_Item_Data       *item;
   Elm_Tooltip_Item_Content_Cb func;
   Evas_Smart_Cb               del_cb;
   const void                 *data;
};

static Evas_Object *
_elm_widget_item_tooltip_label_create(void *data,
                                      Evas_Object *obj EINA_UNUSED,
                                      Evas_Object *tooltip,
                                      void *item EINA_UNUSED)
{
   Evas_Object *label = elm_label_add(tooltip);
   if (!label)
     return NULL;
   elm_object_style_set(label, "tooltip");
   elm_object_text_set(label, data);
   return label;
}

static Evas_Object *
_elm_widget_item_tooltip_trans_label_create(void *data,
                                            Evas_Object *obj EINA_UNUSED,
                                            Evas_Object *tooltip,
                                            void *item EINA_UNUSED)
{
   Evas_Object *label = elm_label_add(tooltip);
   if (!label)
     return NULL;
   elm_object_style_set(label, "tooltip");
   elm_object_translatable_text_set(label, data);
   return label;
}

static void
_elm_widget_item_tooltip_label_del_cb(void *data,
                                      Evas_Object *obj EINA_UNUSED,
                                      void *event_info EINA_UNUSED)
{
   eina_stringshare_del(data);
}

/**
 * @internal
 *
 * Set the text to be shown in the widget item.
 *
 * @param item Target item
 * @param text The text to set in the content
 *
 * Setup the text as tooltip to object. The item can have only one tooltip,
 * so any previous tooltip data is removed.
 *
 * @ingroup Widget
 */
EOLIAN static void
_elm_widget_item_tooltip_text_set(Eo *eo_item EINA_UNUSED,
                                  Elm_Widget_Item_Data *item EINA_UNUSED,
                                  const char *text)
{
   ELM_WIDGET_ITEM_CHECK_OR_RETURN(item);
   ELM_WIDGET_ITEM_RETURN_IF_ONDEL(item);
   EINA_SAFETY_ON_NULL_RETURN(text);

   text = eina_stringshare_add(text);
   elm_wdg_item_tooltip_content_cb_set(item->eo_obj, _elm_widget_item_tooltip_label_create, text, _elm_widget_item_tooltip_label_del_cb);
}

EOLIAN static void
_elm_widget_item_tooltip_translatable_text_set(Eo *eo_item EINA_UNUSED,
                                               Elm_Widget_Item_Data *item EINA_UNUSED,
                                               const char *text)
{
   ELM_WIDGET_ITEM_CHECK_OR_RETURN(item);
   ELM_WIDGET_ITEM_RETURN_IF_ONDEL(item);
   EINA_SAFETY_ON_NULL_RETURN(text);

   text = eina_stringshare_add(text);
   elm_wdg_item_tooltip_content_cb_set(item->eo_obj, _elm_widget_item_tooltip_trans_label_create, text, _elm_widget_item_tooltip_label_del_cb);
}

static Evas_Object *
_elm_widget_item_tooltip_create(void *data,
                                Evas_Object *obj,
                                Evas_Object *tooltip)
{
   Elm_Widget_Item_Tooltip *wit = data;
   return wit->func((void *)wit->data, obj, tooltip, wit->item->eo_obj);
}

static void
_elm_widget_item_tooltip_del_cb(void *data,
                                Evas_Object *obj,
                                void *event_info EINA_UNUSED)
{
   Elm_Widget_Item_Tooltip *wit = data;
   if (wit->del_cb) wit->del_cb((void *)wit->data, obj, wit->item->eo_obj);
   free(wit);
}

/**
 * @internal
 *
 * Set the content to be shown in the tooltip item
 *
 * Setup the tooltip to item. The item can have only one tooltip,
 * so any previous tooltip data is removed. @p func(with @p data) will
 * be called every time that need show the tooltip and it should
 * return a valid Evas_Object. This object is then managed fully by
 * tooltip system and is deleted when the tooltip is gone.
 *
 * @param item the widget item being attached a tooltip.
 * @param func the function used to create the tooltip contents.
 * @param data what to provide to @a func as callback data/context.
 * @param del_cb called when data is not needed anymore, either when
 *        another callback replaces @func, the tooltip is unset with
 *        elm_widget_item_tooltip_unset() or the owner @a item
 *        dies. This callback receives as the first parameter the
 *        given @a data, and @c event_info is the item.
 *
 * @ingroup Widget
 */
EOLIAN static void
_elm_widget_item_tooltip_content_cb_set(Eo *eo_item EINA_UNUSED,
                                        Elm_Widget_Item_Data *item,
                                        Elm_Tooltip_Item_Content_Cb func,
                                        const void *data,
                                        Evas_Smart_Cb del_cb)
{
   Elm_Widget_Item_Tooltip *wit;

   ELM_WIDGET_ITEM_CHECK_OR_GOTO(item, error_noitem);
   //ELM_WIDGET_ITEM_RETURN_IF_GOTO(item, error_noitem);

   if (!func)
     {
        elm_wdg_item_tooltip_unset(item->eo_obj);
        return;
     }

   wit = ELM_NEW(Elm_Widget_Item_Tooltip);
   if (!wit) goto error;
   wit->item = item;
   wit->func = func;
   wit->data = data;
   wit->del_cb = del_cb;

   elm_object_sub_tooltip_content_cb_set
     (item->view, item->widget, _elm_widget_item_tooltip_create, wit,
     _elm_widget_item_tooltip_del_cb);

   return;

error_noitem:
   if (del_cb) del_cb((void *)data, NULL, item);
   return;
error:
   if (del_cb) del_cb((void *)data, item->widget, item);
}

/**
 * @internal
 *
 * Unset tooltip from item
 *
 * @param item widget item to remove previously set tooltip.
 *
 * Remove tooltip from item. The callback provided as del_cb to
 * elm_widget_item_tooltip_content_cb_set() will be called to notify
 * it is not used anymore.
 *
 * @see elm_widget_item_tooltip_content_cb_set()
 *
 * @ingroup Widget
 */
EOLIAN static void
_elm_widget_item_tooltip_unset(Eo *eo_item EINA_UNUSED, Elm_Widget_Item_Data *item)
{
   ELM_WIDGET_ITEM_CHECK_OR_RETURN(item);
   ELM_WIDGET_ITEM_RETURN_IF_ONDEL(item);

   elm_object_tooltip_unset(item->view);
}

/**
 * @internal
 *
 * Sets a different style for this item tooltip.
 *
 * @note before you set a style you should define a tooltip with
 *       elm_widget_item_tooltip_content_cb_set() or
 *       elm_widget_item_tooltip_text_set()
 *
 * @param item widget item with tooltip already set.
 * @param style the theme style to use (default, transparent, ...)
 *
 * @ingroup Widget
 */
EOLIAN static void
_elm_widget_item_tooltip_style_set(Eo *eo_item EINA_UNUSED,
                                   Elm_Widget_Item_Data *item,
                                   const char *style)
{
   ELM_WIDGET_ITEM_CHECK_OR_RETURN(item);
   ELM_WIDGET_ITEM_RETURN_IF_ONDEL(item);

   elm_object_tooltip_style_set(item->view, style);
}

EOLIAN static Eina_Bool
_elm_widget_item_tooltip_window_mode_set(Eo *eo_item EINA_UNUSED,
                                         Elm_Widget_Item_Data *item,
                                         Eina_Bool disable)
{
   ELM_WIDGET_ITEM_CHECK_OR_RETURN(item, EINA_FALSE);
   ELM_WIDGET_ITEM_RETURN_IF_ONDEL(item, EINA_FALSE);

   return elm_object_tooltip_window_mode_set(item->view, disable);
}

EOLIAN static Eina_Bool
_elm_widget_item_tooltip_window_mode_get(const Eo *eo_item EINA_UNUSED, Elm_Widget_Item_Data *item)
{
   ELM_WIDGET_ITEM_CHECK_OR_RETURN(item, EINA_FALSE);
   ELM_WIDGET_ITEM_RETURN_IF_ONDEL(item, EINA_FALSE);

   return elm_object_tooltip_window_mode_get(item->view);
}

/**
 * @internal
 *
 * Get the style for this item tooltip.
 *
 * @param item widget item with tooltip already set.
 * @return style the theme style in use, defaults to "default". If the
 *         object does not have a tooltip set, then NULL is returned.
 *
 * @ingroup Widget
 */
EOLIAN static const char *
_elm_widget_item_tooltip_style_get(const Eo *eo_item EINA_UNUSED, Elm_Widget_Item_Data *item)
{
   ELM_WIDGET_ITEM_CHECK_OR_RETURN(item, NULL);

   return elm_object_tooltip_style_get(item->view);
}

EOLIAN static void
_elm_widget_item_cursor_set(Eo *eo_item EINA_UNUSED,
                            Elm_Widget_Item_Data *item,
                            const char *cursor)
{
   ELM_WIDGET_ITEM_CHECK_OR_RETURN(item);
   ELM_WIDGET_ITEM_RETURN_IF_ONDEL(item);

   elm_object_sub_cursor_set(item->view, item->widget, cursor);
}

EOLIAN static const char *
_elm_widget_item_cursor_get(const Eo *eo_item EINA_UNUSED, Elm_Widget_Item_Data *item)
{
   ELM_WIDGET_ITEM_CHECK_OR_RETURN(item, NULL);
   return elm_object_sub_cursor_get(item->view);
}

EOLIAN static void
_elm_widget_item_cursor_unset(Eo *eo_item EINA_UNUSED, Elm_Widget_Item_Data *item)
{
   ELM_WIDGET_ITEM_CHECK_OR_RETURN(item);
   ELM_WIDGET_ITEM_RETURN_IF_ONDEL(item);

   elm_object_cursor_unset(item->view);
}

/**
 * @internal
 *
 * Sets a different style for this item cursor.
 *
 * @note before you set a style you should define a cursor with
 *       elm_widget_item_cursor_set()
 *
 * @param item widget item with cursor already set.
 * @param style the theme style to use (default, transparent, ...)
 *
 * @ingroup Widget
 */
EOLIAN static void
_elm_widget_item_cursor_style_set(Eo *eo_item EINA_UNUSED,
                                  Elm_Widget_Item_Data *item,
                                  const char *style)
{
   ELM_WIDGET_ITEM_CHECK_OR_RETURN(item);
   ELM_WIDGET_ITEM_RETURN_IF_ONDEL(item);

   elm_object_sub_cursor_style_set(item->view, style);
}

/**
 * @internal
 *
 * Get the style for this item cursor.
 *
 * @param item widget item with cursor already set.
 * @return style the theme style in use, defaults to "default". If the
 *         object does not have a cursor set, then NULL is returned.
 *
 * @ingroup Widget
 */
EOLIAN static const char *
_elm_widget_item_cursor_style_get(const Eo *eo_item EINA_UNUSED,
                                  Elm_Widget_Item_Data *item)
{
   ELM_WIDGET_ITEM_CHECK_OR_RETURN(item, NULL);
   return elm_object_sub_cursor_style_get(item->view);
}

/**
 * @internal
 *
 * Set if the cursor set should be searched on the theme or should use
 * the provided by the engine, only.
 *
 * @note before you set if should look on theme you should define a cursor
 * with elm_object_cursor_set(). By default it will only look for cursors
 * provided by the engine.
 *
 * @param item widget item with cursor already set.
 * @param engine_only boolean to define it cursors should be looked only
 * between the provided by the engine or searched on widget's theme as well.
 *
 * @ingroup Widget
 */
EOLIAN static void
_elm_widget_item_cursor_engine_only_set(Eo *eo_item EINA_UNUSED,
                                        Elm_Widget_Item_Data *item,
                                        Eina_Bool engine_only)
{
   ELM_WIDGET_ITEM_CHECK_OR_RETURN(item);
   ELM_WIDGET_ITEM_RETURN_IF_ONDEL(item);

   elm_object_sub_cursor_theme_search_enabled_set(item->view, !engine_only);
}

/**
 * @internal
 *
 * Get the cursor engine only usage for this item cursor.
 *
 * @param item widget item with cursor already set.
 * @return engine_only boolean to define it cursors should be looked only
 * between the provided by the engine or searched on widget's theme as well. If
 *         the object does not have a cursor set, then EINA_FALSE is returned.
 *
 * @ingroup Widget
 */
EOLIAN static Eina_Bool
_elm_widget_item_cursor_engine_only_get(const Eo *eo_item EINA_UNUSED, Elm_Widget_Item_Data *item)
{
   ELM_WIDGET_ITEM_CHECK_OR_RETURN(item, EINA_FALSE);
   return !elm_object_sub_cursor_theme_search_enabled_get(item->view);
}

EOLIAN static void
_elm_widget_item_part_content_set(Eo *eo_item EINA_UNUSED,
                                  Elm_Widget_Item_Data *item,
                                  const char *part EINA_UNUSED,
                                  Evas_Object *content EINA_UNUSED)
{
   ERR_NOT_SUPPORTED(item, "elm_object_part_content_set()");
}

EOLIAN static Evas_Object *
_elm_widget_item_part_content_get(const Eo *eo_item EINA_UNUSED,
                                  Elm_Widget_Item_Data *item,
                                  const char *part EINA_UNUSED)
{
   ERR_NOT_SUPPORTED(item, "elm_object_part_content_get()");
   return NULL;
}

EOLIAN static Evas_Object *
_elm_widget_item_part_content_unset(Eo *eo_item EINA_UNUSED,
                                    Elm_Widget_Item_Data *item,
                                    const char *part EINA_UNUSED)
{
   ERR_NOT_SUPPORTED(item, "elm_object_part_content_unset()");
   return NULL;
}

EOLIAN static void
_elm_widget_item_part_text_set(Eo *eo_item EINA_UNUSED,
                               Elm_Widget_Item_Data *item,
                               const char *part EINA_UNUSED,
                               const char *label EINA_UNUSED)
{
   ERR_NOT_SUPPORTED(item, "elm_object_part_text_set()");
}

EOLIAN static const char *
_elm_widget_item_part_text_get(const Eo *eo_item EINA_UNUSED,
                               Elm_Widget_Item_Data *item,
                               const char *part EINA_UNUSED)
{
   ERR_NOT_SUPPORTED(item, "elm_object_part_text_get()");
   return NULL;
}

static void
_elm_widget_item_part_text_custom_free(void *data)
{
   Elm_Label_Data *label;
   label = data;
   eina_stringshare_del(label->part);
   eina_stringshare_del(label->text);
   free(label);
}

EOLIAN static void
_elm_widget_item_part_text_custom_set(Eo *eo_item EINA_UNUSED,
                                      Elm_Widget_Item_Data *item,
                                      const char *part,
                                      const char *text)
{
   Elm_Label_Data *label;
   ELM_WIDGET_ITEM_CHECK_OR_RETURN(item);
   ELM_WIDGET_ITEM_RETURN_IF_ONDEL(item);

   if (!item->labels)
     item->labels =
        eina_hash_stringshared_new(_elm_widget_item_part_text_custom_free);
   label = eina_hash_find(item->labels, part);
   if (!label)
     {
        label = malloc(sizeof(Elm_Label_Data));
        if (!label)
          {
             ERR("Failed to allocate memory");
             return;
          }
        label->part = eina_stringshare_add(part);
        label->text = eina_stringshare_add(text);
        eina_hash_add(item->labels, part, label);
     }
   else
     eina_stringshare_replace(&label->text, text);
}

EOLIAN static const char *
_elm_widget_item_part_text_custom_get(const Eo *eo_item EINA_UNUSED,
                                      Elm_Widget_Item_Data *item,
                                      const char *part)
{
   Elm_Label_Data *label;
   ELM_WIDGET_ITEM_CHECK_OR_RETURN(item, NULL);
   label = eina_hash_find(item->labels, part);
   return label ? label->text : NULL;
}

static Eina_Bool
_elm_widget_item_part_text_custom_foreach(const Eina_Hash *labels EINA_UNUSED,
                                          const void *key EINA_UNUSED,
                                          void *data,
                                          void *func_data)
{
   Elm_Label_Data *label;
   Elm_Widget_Item_Data *item;
   label = data;
   item = func_data;

   elm_wdg_item_part_text_set(item->eo_obj, label->part, label->text);

   return EINA_TRUE;
}

EOLIAN static void
_elm_widget_item_part_text_custom_update(Eo *eo_item EINA_UNUSED, Elm_Widget_Item_Data *item)
{
   ELM_WIDGET_ITEM_CHECK_OR_RETURN(item);
   ELM_WIDGET_ITEM_RETURN_IF_ONDEL(item);
   if (item->labels)
     eina_hash_foreach(item->labels,
                       _elm_widget_item_part_text_custom_foreach, item);
}

EOLIAN static void
_elm_widget_item_signal_emit(Eo *eo_item EINA_UNUSED,
                             Elm_Widget_Item_Data *item EINA_UNUSED,
                             const char *emission EINA_UNUSED,
                             const char *source EINA_UNUSED)
{

}

EOLIAN static void
_elm_widget_item_signal_callback_add(Eo *eo_item,
                                     Elm_Widget_Item_Data *item,
                                     const char *emission,
                                     const char *source,
                                     Elm_Object_Item_Signal_Cb func,
                                     void *data)
{
   ELM_WIDGET_ITEM_CHECK_OR_RETURN(item);
   ELM_WIDGET_ITEM_RETURN_IF_ONDEL(item);
   EINA_SAFETY_ON_NULL_RETURN(func);

   Elm_Widget_Item_Signal_Data *wisd;

   wisd = malloc(sizeof(Elm_Widget_Item_Signal_Data));
   if (!wisd) return;

   wisd->item = eo_item;
   wisd->func = (Elm_Widget_Item_Signal_Cb)func;
   wisd->data = data;
   wisd->emission = eina_stringshare_add(emission);
   wisd->source = eina_stringshare_add(source);

   if (_elm_widget_is(item->view))
     elm_object_signal_callback_add(item->view, emission, source, _elm_widget_item_signal_cb, wisd);
   else if (efl_isa(item->view, EFL_CANVAS_LAYOUT_CLASS))
     edje_object_signal_callback_add(item->view, emission, source, _elm_widget_item_signal_cb, wisd);
   else
     {
        WRN("The %s widget item doesn't support signal callback add!",
            efl_class_name_get(efl_class_get(item->widget)));
        free(wisd);
        return;
     }

   item->signals = eina_list_append(item->signals, wisd);
}

EOLIAN static void *
_elm_widget_item_signal_callback_del(Eo *eo_item EINA_UNUSED,
                                     Elm_Widget_Item_Data *item,
                                     const char *emission,
                                     const char *source,
                                     Elm_Object_Item_Signal_Cb func)
{
   Elm_Widget_Item_Signal_Data *wisd;
   Eina_List *l;

   ELM_WIDGET_ITEM_CHECK_OR_RETURN(item, NULL);
   ELM_WIDGET_ITEM_RETURN_IF_ONDEL(item, NULL);
   EINA_SAFETY_ON_NULL_RETURN_VAL(func, NULL);

   EINA_LIST_FOREACH(item->signals, l, wisd)
     {
        if ((wisd->func == (Elm_Widget_Item_Signal_Cb)func) &&
            !strcmp(wisd->emission, emission) &&
            !strcmp(wisd->source, source))
          return _elm_widget_item_signal_callback_list_get(item, l);
     }

   return NULL;
}



EOLIAN static void
_elm_widget_item_access_info_set(Eo *eo_item EINA_UNUSED,
                                 Elm_Widget_Item_Data *item,
                                 const char *txt)
{
   ELM_WIDGET_ITEM_CHECK_OR_RETURN(item);
   ELM_WIDGET_ITEM_RETURN_IF_ONDEL(item);

   eina_stringshare_del(item->access_info);
   if (!txt) item->access_info = NULL;
   else item->access_info = eina_stringshare_add(txt);
}

EOLIAN static void
_elm_widget_item_translate(Eo *eo_item EINA_UNUSED, Elm_Widget_Item_Data *item)
{
   ELM_WIDGET_ITEM_CHECK_OR_RETURN(item);
   ELM_WIDGET_ITEM_RETURN_IF_ONDEL(item);

#ifdef HAVE_GETTEXT
   Elm_Translate_String_Data *ts;
   EINA_INLIST_FOREACH(item->translate_strings, ts)
     {
        if (!ts->string) continue;
        const char *s = dgettext(ts->domain, ts->string);
        item->on_translate = EINA_TRUE;
        elm_wdg_item_part_text_set(item->eo_obj, ts->id, s);
        item->on_translate = EINA_FALSE;
     }
#endif
}

EOLIAN static void
_elm_widget_item_access_order_set(Eo *eo_item EINA_UNUSED,
                                  Elm_Widget_Item_Data *item,
                                  Eina_List *objs)
{
   _elm_access_widget_item_access_order_set(item, objs);
}

EOLIAN static const Eina_List *
_elm_widget_item_access_order_get(const Eo *eo_item EINA_UNUSED, Elm_Widget_Item_Data *item)
{
   return _elm_access_widget_item_access_order_get(item);
}

EOLIAN static void
_elm_widget_item_access_order_unset(Eo *eo_item EINA_UNUSED, Elm_Widget_Item_Data *item)
{
   _elm_access_widget_item_access_order_unset(item);
}

EOLIAN static Evas_Object*
_elm_widget_item_access_register(Eo *eo_item EINA_UNUSED, Elm_Widget_Item_Data *item)
{
   _elm_access_widget_item_register(item);
   return item->access_obj;
}

EOLIAN static void
_elm_widget_item_access_unregister(Eo *eo_item EINA_UNUSED, Elm_Widget_Item_Data *item)
{
   _elm_access_widget_item_unregister(item);
}

EOLIAN static Evas_Object*
_elm_widget_item_access_object_get(const Eo *eo_item EINA_UNUSED, Elm_Widget_Item_Data *item)
{
   return item->access_obj;
}

EOLIAN static Evas_Object *
_elm_widget_item_focus_next_object_get(const Eo *eo_item EINA_UNUSED, Elm_Widget_Item_Data *item, Elm_Focus_Direction dir)
{
   Evas_Object *ret = NULL;

   if (dir == ELM_FOCUS_PREVIOUS)
     ret = item->focus_previous;
   else if (dir == ELM_FOCUS_NEXT)
     ret = item->focus_next;
   else if (dir == ELM_FOCUS_UP)
     ret = item->focus_up;
   else if (dir == ELM_FOCUS_DOWN)
     ret = item->focus_down;
   else if (dir == ELM_FOCUS_RIGHT)
     ret = item->focus_right;
   else if (dir == ELM_FOCUS_LEFT)
     ret = item->focus_left;

   return ret;
}

EOLIAN static void
_elm_widget_item_focus_next_object_set(Eo *eo_item EINA_UNUSED, Elm_Widget_Item_Data *item, Evas_Object *next, Elm_Focus_Direction dir)
{
   if (dir == ELM_FOCUS_PREVIOUS)
     item->focus_previous = next;
   else if (dir == ELM_FOCUS_NEXT)
     item->focus_next = next;
   else if (dir == ELM_FOCUS_UP)
     item->focus_up = next;
   else if (dir == ELM_FOCUS_DOWN)
     item->focus_down = next;
   else if (dir == ELM_FOCUS_RIGHT)
     item->focus_right = next;
   else if (dir == ELM_FOCUS_LEFT)
     item->focus_left = next;
}

EOLIAN static Elm_Object_Item*
_elm_widget_item_focus_next_item_get(const Eo *eo_item EINA_UNUSED, Elm_Widget_Item_Data *item, Elm_Focus_Direction dir)
{
   Elm_Object_Item *ret = NULL;

   if (dir == ELM_FOCUS_PREVIOUS)
     ret = item->item_focus_previous;
   else if (dir == ELM_FOCUS_NEXT)
     ret = item->item_focus_next;
   else if (dir == ELM_FOCUS_UP)
     ret = item->item_focus_up;
   else if (dir == ELM_FOCUS_DOWN)
     ret = item->item_focus_down;
   else if (dir == ELM_FOCUS_RIGHT)
     ret = item->item_focus_right;
   else if (dir == ELM_FOCUS_LEFT)
     ret = item->item_focus_left;

   return ret;
}

EOLIAN static void
_elm_widget_item_focus_next_item_set(Eo *eo_item EINA_UNUSED, Elm_Widget_Item_Data *item, Elm_Object_Item *next_item, Elm_Focus_Direction dir)
{
   if (dir == ELM_FOCUS_PREVIOUS)
     item->item_focus_previous = next_item;
   else if (dir == ELM_FOCUS_NEXT)
     item->item_focus_next = next_item;
   else if (dir == ELM_FOCUS_UP)
     item->item_focus_up = next_item;
   else if (dir == ELM_FOCUS_DOWN)
     item->item_focus_down = next_item;
   else if (dir == ELM_FOCUS_RIGHT)
     item->item_focus_right = next_item;
   else if (dir == ELM_FOCUS_LEFT)
     item->item_focus_left = next_item;
}

/* happy debug functions */
#ifdef ELM_DEBUG
static void
_sub_obj_tree_dump(const Evas_Object *obj,
                   int lvl)
{
   int i;

   for (i = 0; i < lvl * 3; i++)
     putchar(' ');

   if (_elm_widget_is(obj))
     {
        Eina_List *l;
        INTERNAL_ENTRY;
        DBG("+ %s(%p)\n",
            elm_widget_type_get(obj),
            obj);
        EINA_LIST_FOREACH(sd->subobjs, l, obj)
          _sub_obj_tree_dump(obj, lvl + 1);
     }
   else
     DBG("+ %s(%p)\n", evas_object_type_get(obj), obj);
}

static void
_sub_obj_tree_dot_dump(const Evas_Object *obj,
                       FILE *output)
{
   if (!_elm_widget_is(obj))
     return;
   INTERNAL_ENTRY;

   Eina_Bool visible = evas_object_visible_get(obj);
   Eina_Bool disabled = elm_widget_disabled_get(obj);
   Eina_Bool focused = efl_ui_focus_object_focus_get(obj);
   Eina_Bool can_focus = elm_widget_can_focus_get(obj);

   if (sd->parent_obj)
     {
        fprintf(output, "\"%p\" -- \"%p\" [ color=black", sd->parent_obj, obj);

        if (focused)
          fprintf(output, ", style=bold");

        if (!visible)
          fprintf(output, ", color=gray28");

        fprintf(output, " ];\n");
     }

   fprintf(output, "\"%p\" [ label = \"{%p|%s|%s|visible: %d|"
                   "disabled: %d|focused: %d/%d|focus order:%d}\"",
           obj, obj, elm_widget_type_get(obj),
           evas_object_name_get(obj), visible, disabled, focused, can_focus,
           sd->focus_order);

   if (focused)
     fprintf(output, ", style=bold");

   if (!visible)
     fprintf(output, ", fontcolor=gray28");

   if ((disabled) || (!visible))
     fprintf(output, ", color=gray");

   fprintf(output, " ];\n");

   Eina_List *l;
   Evas_Object *o;
   EINA_LIST_FOREACH(sd->subobjs, l, o)
     _sub_obj_tree_dot_dump(o, output);
}

#endif

EAPI void
elm_widget_tree_dump(const Evas_Object *top)
{
#ifdef ELM_DEBUG
   if (!_elm_widget_is(top))
     return;
   _sub_obj_tree_dump(top, 0);
#else
   (void)top;
   return;
#endif
}

EAPI void
elm_widget_tree_dot_dump(const Evas_Object *top,
                         FILE *output)
{
#ifdef ELM_DEBUG
   if (!_elm_widget_is(top))
     return;
   fprintf(output, "graph " " { node [shape=record];\n");
   _sub_obj_tree_dot_dump(top, output);
   fprintf(output, "}\n");
#else
   (void)top;
   (void)output;
   return;
#endif
}

static void
_focus_event_changed(void *data EINA_UNUSED, const Efl_Event *event)
{
   if (efl_ui_focus_object_focus_get(event->object))
     evas_object_smart_callback_call(event->object, "focused", NULL);
   else
     evas_object_smart_callback_call(event->object, "unfocused", NULL);
}

EOLIAN static Eo *
_efl_ui_widget_efl_object_constructor(Eo *obj, Elm_Widget_Smart_Data *sd EINA_UNUSED)
{
   sd->on_create = EINA_TRUE;
   efl_canvas_group_clipped_set(obj, EINA_FALSE);
   obj = efl_constructor(efl_super(obj, MY_CLASS));
   efl_canvas_object_type_set(obj, MY_CLASS_NAME_LEGACY);
   evas_object_smart_callbacks_descriptions_set(obj, _smart_callbacks);
   if (!efl_isa(obj, EFL_UI_WIN_CLASS))
     {
        efl_ui_widget_sub_object_add(efl_parent_get(obj), obj);
     }

   sd->on_create = EINA_FALSE;

   efl_access_object_role_set(obj, EFL_ACCESS_ROLE_UNKNOWN);
   //TIZEN_ONLY(20170717) : expose highlight information on atspi
   sd->can_highlight = EINA_TRUE;
   //
   /***********************************************************
    * TIZEN_ONLY(20180117): Override Paragraph Direction APIs *
    ***********************************************************/
   sd->inherit_paragraph_direction = EINA_TRUE;

   if (efl_isa(parent, EFL_CANVAS_OBJECT_CLASS))
     {
        if (sd->paragraph_direction != efl_canvas_object_paragraph_direction_get(parent))
          {
             sd->paragraph_direction = efl_canvas_object_paragraph_direction_get(parent);
             _efl_ui_widget_efl_canvas_object_paragraph_direction_set_internal(obj, sd, sd->paragraph_direction);
             efl_canvas_object_paragraph_direction_set(efl_super(obj, MY_CLASS), sd->paragraph_direction);
          }
     }
   /*******
    * END *
    *******/

   return obj;
}

EOLIAN static Efl_Object*
_efl_ui_widget_efl_object_finalize(Eo *obj, Elm_Widget_Smart_Data *pd)
{
  Eo *eo;

  eo = efl_finalize(efl_super(obj, MY_CLASS));

  _full_eval(obj, pd);

  return eo;
}


EOLIAN static void
_efl_ui_widget_efl_object_destructor(Eo *obj, Elm_Widget_Smart_Data *sd)
{
   if (sd->manager.provider)
     {
        efl_event_callback_del(sd->manager.provider, EFL_UI_FOCUS_OBJECT_EVENT_FOCUS_MANAGER_CHANGED, _manager_changed_cb, obj);
        sd->manager.provider = NULL;
     }

   //TIZEN_ONLY : elm_widget_item: add at-spi name setter
   efl_access_object_description_set(obj, NULL);
   efl_access_object_i18n_name_set(obj, NULL);

   efl_access_object_description_cb_set(obj, NULL, NULL);
   efl_access_object_name_cb_set(obj, NULL, NULL);
   //
   //TIZEN_ONLY(20170405) Add gesture method to accessible interface
   efl_access_object_gesture_cb_set(obj, NULL, NULL);
   //
   //TIZEN_ONLY : elm_widget_item: add at-spi name setter
   efl_access_object_translation_domain_set(obj, NULL);
   efl_access_object_relationships_clear(obj);
   //

   efl_access_object_attributes_clear(obj);
   if (sd->logical.parent)
     {
        efl_weak_unref(&sd->logical.parent);
        sd->logical.parent = NULL;
     }

   // TIZEN_ONLY(20150709) : atspi relations api
   if (sd->atspi_custom_relations)
     efl_access_relation_set_free(&sd->atspi_custom_relations);
   //

   //TIZEN_ONLY(20150717) add widget name setter
   if (sd->name)
     eina_stringshare_del(sd->name);
   //

   //TIZEN_ONLY(20150731) : add i18n support for name and description
   if (sd->atspi_translation_domain)
     eina_stringshare_del(sd->atspi_translation_domain);
   ///

   sd->on_destroy = EINA_TRUE;
   efl_destructor(efl_super(obj, EFL_UI_WIDGET_CLASS));
   sd->on_destroy = EINA_FALSE;
}

/* internal eo */

EOLIAN static void
_efl_ui_widget_efl_object_debug_name_override(Eo *obj, Elm_Widget_Smart_Data *sd EINA_UNUSED, Eina_Strbuf *sb)
{
   const char *focus = "";

   if (efl_ui_focus_object_focus_get(obj)) focus = ":focused";
   efl_debug_name_override(efl_super(obj, MY_CLASS), sb);
   eina_strbuf_append_printf(sb, "%s", focus);
}

EOLIAN static Eina_Bool
_efl_ui_widget_efl_ui_focus_object_on_focus_update(Eo *obj, Elm_Widget_Smart_Data *sd)
{
   Eina_Bool focused;

   if (!elm_widget_can_focus_get(obj))
     return EINA_FALSE;

   //TIZEN_ONLY(20180607): Restore legacy focus
   if (elm_widget_is_legacy(obj))
     focused = elm_widget_focus_get(obj);
   else
   //
     focused = efl_ui_focus_object_focus_get(obj);

   if (!sd->resize_obj)
     evas_object_focus_set(obj, focused);

   //TIZEN_ONLY(20180607): Restore legacy focus
   if (elm_widget_is_legacy(obj))
     {
        if (focused)
          evas_object_smart_callback_call(obj, "focused", NULL);
        else
          evas_object_smart_callback_call(obj, "unfocused", NULL);
     }
   //

   if (_elm_atspi_enabled() && !elm_widget_child_can_focus_get(obj))
     efl_access_state_changed_signal_emit(obj, EFL_ACCESS_STATE_TYPE_FOCUSED, focused);

   return EINA_TRUE;
}

EOLIAN static Eina_Bool
_efl_ui_widget_widget_input_event_handler(Eo *obj EINA_UNUSED, Elm_Widget_Smart_Data *_pd EINA_UNUSED, const Efl_Event *eo_event EINA_UNUSED, Evas_Object *source EINA_UNUSED)
{
   return EINA_FALSE;
}

EOLIAN static Eina_Bool
_efl_ui_widget_on_access_activate(Eo *obj EINA_UNUSED, Elm_Widget_Smart_Data *_pd EINA_UNUSED, Efl_Ui_Activate act EINA_UNUSED)
{
   WRN("The %s widget does not implement the \"activate\" functions.",
       efl_class_name_get(efl_class_get(obj)));
   return EINA_TRUE;
}

EOLIAN static void
_efl_ui_widget_class_constructor(Efl_Class *klass)
{
   evas_smart_legacy_type_register(MY_CLASS_NAME_LEGACY, klass);
}

EOLIAN static Eina_Bool
_efl_ui_widget_efl_access_component_focus_grab(Eo *obj, Elm_Widget_Smart_Data *pd EINA_UNUSED)
{
   if (elm_object_focus_allow_get(obj))
     {
       Ecore_Evas *ee = ecore_evas_ecore_evas_get(evas_object_evas_get(obj));
       if (!ee) return EINA_FALSE;
       ecore_evas_activate(ee);
       elm_object_focus_set(obj, EINA_TRUE);
       return EINA_TRUE;
     }
   return EINA_FALSE;
}


EOLIAN static const char*
_efl_ui_widget_efl_access_object_i18n_name_get(const Eo *obj, Elm_Widget_Smart_Data *_pd EINA_UNUSED)
{
   const char *ret, *name;
   name = efl_access_object_i18n_name_get(efl_super(obj, EFL_UI_WIDGET_CLASS));

   if (name) return name;

   //TIZEN_ONLY(20150717) add widget name setter
   if (_pd->name)
     {
#ifdef HAVE_GETTEXT
        if (_pd->atspi_translation_domain)
          return dgettext(_pd->atspi_translation_domain, _pd->name);
#endif
        return _pd->name;
     }
   //

   //TIZEN_ONLY(20170110) : Ignore text from elm_object_text_set in accessible_name_get
   Efl_Access_Role role;
   role = efl_access_object_role_get(obj);
   if(role == EFL_ACCESS_ROLE_DIALOG || role == EFL_ACCESS_ROLE_PASSWORD_TEXT)
     return NULL;
   //

   ret = elm_object_text_get(obj);
   if (!ret) return NULL;

   return _elm_widget_accessible_plain_name_get(obj, ret);
}

EOLIAN static Eina_List*
_efl_ui_widget_efl_access_object_access_children_get(const Eo *obj EINA_UNUSED, Elm_Widget_Smart_Data *pd)
{
   Eina_List *l, *accs = NULL;
   Evas_Object *widget;
   // TIZEN_ONLY(20160824): Do not append a child, if its accessible parent is different with widget parent
   Eo *parent;
   //
   Eo *proxy = NULL;

   EINA_LIST_FOREACH(pd->subobjs, l, widget)
     {
        const char *type = evas_object_type_get(widget);
        // TIZEN ONLY
        // Ugly Tizen hack to integrate AT-SPI2 accessibility provided by WebKit/Chromium with elementary one.
        // This wrapper class should be implemented in Webkit/Chromium EFL ports
        if (type && (!strcmp(type, "EWebView") || !strcmp(type, "WebView")))
          {
             elm_atspi_ewk_wrapper_a11y_init(obj, widget);
          }
     }
   EINA_LIST_FOREACH(pd->subobjs, l, widget)
     {
        // TIZEN_ONLY(20160705) - enable atspi_proxy to work
        /* This assumes that only one proxy exists in obj */
        if (!proxy)
          {
             proxy = plug_type_proxy_get(obj, widget);
             if (proxy)
               {
                  accs = eina_list_append(accs, proxy);
                  continue;
               }
          }
        //
        // TIZEN ONLY - END
        if (!elm_object_widget_check(widget)) continue;
        if (!efl_isa(widget, EFL_ACCESS_OBJECT_MIXIN)) continue;
        accs = eina_list_append(accs, widget);
     }
   //TIZEN_ONLY(20150709) : spatially sort atspi children
   // sort children using its top-left coordinate
   accs = eina_list_sort(accs, -1, _sort_vertically);
   Eina_List *line, *lines = _lines_split(accs);
   accs = NULL;
   EINA_LIST_FREE(lines, line)
     accs = eina_list_merge(accs, eina_list_sort(line, -1, _sort_horizontally));
   //

  if (proxy)
    {
       Eo *deputy = NULL;
       accs = eina_list_remove(accs, proxy);
       EINA_LIST_FOREACH(accs, l, widget)
         {
             if (efl_isa(widget, ELM_ACCESS_CLASS))
               {
                  Elm_Access_Info *info = _elm_access_info_get(widget);
                  if (!info) continue;
                  if (obj == info->part_object)
                    {
                       deputy = widget;
                       break;
                    }
               }
         }

       if (deputy)
         {
            accs = eina_list_append_relative(accs, proxy, deputy);
         }
    }

   return accs;
}

/* Legacy APIs */

EOLIAN static Efl_Access_State_Set
_efl_ui_widget_efl_access_object_state_set_get(const Eo *obj, Elm_Widget_Smart_Data *pd EINA_UNUSED)
{
   Efl_Access_State_Set states = 0;

   states = efl_access_object_state_set_get(efl_super(obj, EFL_UI_WIDGET_CLASS));

   // TIZEN_ONLY(20171114) Accessibility Highlight Frame added
   // //TIZEN_ONLY(20171108): bring HIGHLIGHT related changes
   // Evas_Object *win = elm_widget_top_get(obj);
   // if (win && efl_isa(win, EFL_UI_WIN_CLASS))
   //   {
   //      if (_elm_win_accessibility_highlight_get(win) == obj)
   //        STATE_TYPE_SET(states, ELM_ATSPI_STATE_HIGHLIGHTED);
   //   }
   // STATE_TYPE_SET(states, ELM_ATSPI_STATE_HIGHLIGHTABLE);
   // //
   //

   if (evas_object_visible_get(obj))
     {
        STATE_TYPE_SET(states, EFL_ACCESS_STATE_TYPE_VISIBLE);
        if (_elm_widget_onscreen_is(obj))
          STATE_TYPE_SET(states, EFL_ACCESS_STATE_TYPE_SHOWING);
     }
   if (!elm_widget_child_can_focus_get(obj))
     {
        if (elm_object_focus_allow_get(obj))
          STATE_TYPE_SET(states, EFL_ACCESS_STATE_TYPE_FOCUSABLE);
        if (elm_object_focus_get(obj))
          STATE_TYPE_SET(states, EFL_ACCESS_STATE_TYPE_FOCUSED);
     }
   if (!elm_object_disabled_get(obj))
     {
        STATE_TYPE_SET(states, EFL_ACCESS_STATE_TYPE_ENABLED);
        STATE_TYPE_SET(states, EFL_ACCESS_STATE_TYPE_SENSITIVE);
     }

   //TIZEN_ONLY(20170717) : expose highlight information on atspi
   if (_elm_widget_highlightable(obj))
     STATE_TYPE_SET(states, EFL_ACCESS_STATE_TYPE_HIGHLIGHTABLE);
   else
     STATE_TYPE_UNSET(states, EFL_ACCESS_STATE_TYPE_HIGHLIGHTABLE);

   if (_elm_object_accessibility_currently_highlighted_get() == (void*)pd->obj)
     STATE_TYPE_SET(states, EFL_ACCESS_STATE_TYPE_HIGHLIGHTED);
   //

   return states;
}

EOLIAN static Eina_List*
_efl_ui_widget_efl_access_object_attributes_get(const Eo *obj, Elm_Widget_Smart_Data *pd EINA_UNUSED)
{
   const char *type = NULL;
   const char *style = NULL;
   Eina_List *attr_list = NULL;
   Efl_Access_Attribute *attr = NULL;

   attr_list = efl_access_object_attributes_get(efl_super(obj, EFL_UI_WIDGET_CLASS));

   //Add type and style information in addition.
   type = elm_widget_type_get(obj);
   if (type)
     {
        attr = calloc(1, sizeof(Efl_Access_Attribute));
        if (attr)
          {
             attr->key = eina_stringshare_add("type");
             attr->value = eina_stringshare_add(type);
             attr_list = eina_list_append(attr_list, attr);
           }
     }

   style = elm_widget_style_get(obj);
   if (style)
     {
        attr = calloc(1, sizeof(Efl_Access_Attribute));
        if (attr)
          {
             attr->key = eina_stringshare_add("style");
             attr->value = eina_stringshare_add(style);
             attr_list = eina_list_append(attr_list, attr);
          }
     }

   return attr_list;
}

EOLIAN static Eina_List *
_elm_widget_item_efl_access_object_attributes_get(const Eo *eo_item, Elm_Widget_Item_Data *pd  EINA_UNUSED)
{
   const char *style = NULL;
   Eina_List *attr_list = NULL;
   Efl_Access_Attribute *attr = NULL;

   attr_list = efl_access_object_attributes_get(efl_super(eo_item, ELM_WIDGET_ITEM_CLASS));

   style = elm_object_item_style_get(eo_item);
   if (style)
     {
        attr = calloc(1, sizeof(Efl_Access_Attribute));
        if (attr)
          {
             attr->key = eina_stringshare_add("style");
             attr->value = eina_stringshare_add(style);
             attr_list = eina_list_append(attr_list, attr);
          }
     }
   return attr_list;
}

EOLIAN static Eina_Rect
_elm_widget_item_efl_access_component_extents_get(const Eo *obj EINA_UNUSED, Elm_Widget_Item_Data *sd EINA_UNUSED, Eina_Bool screen_coords)
{
   Eina_Rect r = EINA_RECT(-1, -1, -1, -1);
   int ee_x, ee_y;

   if (!sd->view) return r;

   r = efl_gfx_entity_geometry_get(sd->view);
   if (screen_coords)
     {
        Ecore_Evas *ee = ecore_evas_ecore_evas_get(evas_object_evas_get(sd->view));
        if (ee)
          {
             ecore_evas_geometry_get(ee, &ee_x, &ee_y, NULL, NULL);
             r.x += ee_x;
             r.y += ee_y;
          }
     }
   return r;
}

EOLIAN static Eina_Bool
_elm_widget_item_efl_access_component_extents_set(Eo *obj EINA_UNUSED, Elm_Widget_Item_Data *sd EINA_UNUSED, Eina_Bool screen_coords EINA_UNUSED, Eina_Rect r EINA_UNUSED)
{
   return EINA_FALSE;
}

EOLIAN static Eina_Bool
_elm_widget_item_efl_access_component_focus_grab(Eo *obj EINA_UNUSED, Elm_Widget_Item_Data *_pd EINA_UNUSED)
{
   elm_object_item_focus_set(obj, EINA_TRUE);
   return elm_object_item_focus_get(obj);
}

EOLIAN static Efl_Object *
_efl_ui_widget_efl_object_provider_find(const Eo *obj, Elm_Widget_Smart_Data *pd, const Efl_Object *klass)
{
   Efl_Object *lookup = NULL;

   if ((klass == EFL_CONFIG_INTERFACE) || (klass == EFL_CONFIG_GLOBAL_CLASS))
     return _efl_config_obj;

   if (klass == EFL_ACCESS_OBJECT_MIXIN)
     return (Eo*)obj;

   if (pd->provider_lookup) return NULL;
   pd->provider_lookup = EINA_TRUE;

   if (pd->parent_obj) lookup = efl_provider_find(pd->parent_obj, klass);
   if (!lookup) lookup = efl_provider_find(efl_super(obj, MY_CLASS), klass);

   pd->provider_lookup = EINA_FALSE;

   return lookup;
}

EOLIAN static Efl_Ui_Focus_Manager*
_efl_ui_widget_efl_ui_focus_object_focus_parent_get(const Eo *obj EINA_UNUSED, Elm_Widget_Smart_Data *pd EINA_UNUSED)
{
   return pd->focus.parent;
}

EOLIAN static Efl_Ui_Focus_Manager*
_efl_ui_widget_efl_ui_focus_object_focus_manager_get(const Eo *obj EINA_UNUSED, Elm_Widget_Smart_Data *pd EINA_UNUSED)
{
   return pd->focus.manager;
}

EOLIAN static Eina_Rect
_efl_ui_widget_efl_ui_focus_object_focus_geometry_get(const Eo *obj, Elm_Widget_Smart_Data *pd EINA_UNUSED)
{
   return efl_gfx_entity_geometry_get(obj);
}

EOLIAN static void
_efl_ui_widget_efl_ui_focus_object_focus_set(Eo *obj, Elm_Widget_Smart_Data *pd, Eina_Bool focus)
{
   pd->focused = focus;

   efl_ui_focus_object_focus_set(efl_super(obj, MY_CLASS), focus);

   efl_ui_focus_object_on_focus_update(obj);
}

EOLIAN static Efl_Ui_Focus_Manager*
_efl_ui_widget_focus_manager_create(Eo *obj EINA_UNUSED, Elm_Widget_Smart_Data *pd EINA_UNUSED, Efl_Ui_Focus_Object *root EINA_UNUSED)
{
   ERR("No manager presented");
   return NULL;
}

//TIZEN_ONLY(20160726): add API elm_object_part_access_object_get
EOLIAN static Evas_Object*
_efl_ui_widget_part_access_object_get(const Eo *obj, Elm_Widget_Smart_Data *_pd EINA_UNUSED, const char *part EINA_UNUSED)
{
   WRN("The %s widget does not implement the \"part_access_object_get\" functions.",
       efl_class_name_get(efl_class_get(obj)));
   return NULL;
}
//

/* Legacy APIs */

EAPI void
elm_widget_on_show_region_hook_set(Eo *obj, void *data, Efl_Ui_Scrollable_On_Show_Region func, Eina_Free_Cb func_free_cb)
{
   ELM_WIDGET_DATA_GET(obj, sd);

   if ((sd->on_show_region_data == data) && (sd->on_show_region == func))
     return;

   if (sd->on_show_region_data && sd->on_show_region_data_free)
     sd->on_show_region_data_free(sd->on_show_region_data);

   sd->on_show_region = func;
   sd->on_show_region_data = data;
   sd->on_show_region_data_free = func_free_cb;
}

EAPI void
elm_widget_show_region_set(Eo *obj, Eina_Rect sr, Eina_Bool forceshow)
{
   Evas_Object *parent_obj, *child_obj;
   Evas_Coord px, py, cx, cy, nx = 0, ny = 0;

   ELM_WIDGET_DATA_GET_OR_RETURN(obj, sd);

   evas_smart_objects_calculate(evas_object_evas_get(obj));

   if (!forceshow && eina_rectangle_equal(&sr.rect, &sd->show_region.rect)) return;

   sd->show_region = sr;
   if (sd->on_show_region)
     {
        sd->on_show_region(sd->on_show_region_data, obj, sr);

        if (_elm_scrollable_is(obj))
          {
             if (elm_widget_is_legacy(obj))
               {
                  elm_interface_scrollable_content_pos_get(obj, &nx, &ny);
                  sr.x -= nx;
                  sr.y -= ny;
               }
             else
               {
                  Eina_Position2D pos;
                  pos = efl_ui_scrollable_content_pos_get(obj);
                  sr.x -= pos.x;
                  sr.y -= pos.y;
               }
          }
     }

   do
     {
        parent_obj = sd->parent_obj;
        child_obj = sd->obj;
        if ((!parent_obj) || (!_elm_widget_is(parent_obj))) break;
        sd = efl_data_scope_get(parent_obj, MY_CLASS);
        if (!sd) break;

        evas_object_geometry_get(parent_obj, &px, &py, NULL, NULL);
        evas_object_geometry_get(child_obj, &cx, &cy, NULL, NULL);

        sr.x += (cx - px);
        sr.y += (cy - py);
        sd->show_region = sr;

        if (sd->on_show_region)
          sd->on_show_region(sd->on_show_region_data, parent_obj, sr);
     }
   while (parent_obj);
}

EAPI Eina_Rect
elm_widget_show_region_get(const Eo *obj)
{
   ELM_WIDGET_DATA_GET_OR_RETURN(obj, sd, EINA_RECT_EMPTY());
   return (Eina_Rect) sd->show_region;
}
/* elm_object_content_xxx APIs are supposed to work on all objects for which
 * elm_object_widget_check() returns true. The below checks avoid printing out
 * undesired ERR messages. */
EAPI void
elm_widget_content_part_set(Evas_Object *obj, const char *part, Evas_Object *content)
{
   ELM_WIDGET_CHECK(obj);
   if (efl_isa(obj, EFL_UI_LAYOUT_BASE_CLASS))
     {
        elm_layout_content_set(obj, part, content);
        return;
     }
   if (!efl_isa(obj, EFL_PART_INTERFACE)) return;
   if (!part)
     {
        part = efl_ui_widget_default_content_part_get(obj);
        if (!part) return;
     }
   efl_content_set(efl_part(obj, part), content);
}

EAPI Evas_Object *
elm_widget_content_part_get(const Evas_Object *obj, const char *part)
{
   ELM_WIDGET_CHECK(obj) NULL;
   if (efl_isa(obj, EFL_UI_LAYOUT_BASE_CLASS))
     return elm_layout_content_get(obj, part);
   if (!efl_isa(obj, EFL_PART_INTERFACE)) return NULL;
   if (!part)
     {
        part = efl_ui_widget_default_content_part_get(obj);
        if (!part) return NULL;
     }
   return efl_content_get(efl_part(obj, part));
}

EAPI Evas_Object *
elm_widget_content_part_unset(Evas_Object *obj, const char *part)
{
   ELM_WIDGET_CHECK(obj) NULL;
   if (efl_isa(obj, EFL_UI_LAYOUT_BASE_CLASS))
     return elm_layout_content_unset(obj, part);
   if (!efl_isa(obj, EFL_PART_INTERFACE)) return NULL;
   if (!part)
     {
        part = efl_ui_widget_default_content_part_get(obj);
        if (!part) return NULL;
     }
   return efl_content_unset(efl_part(obj, part));
}

EAPI void
elm_widget_signal_emit(Eo *obj, const char *emission, const char *source)
{
   ELM_WIDGET_CHECK(obj);

   if (efl_isa(obj, EFL_UI_LAYOUT_BASE_CLASS))
     elm_layout_signal_emit(obj, emission, source);
   else if (evas_object_smart_type_check(obj, "elm_icon"))
     {
        WRN("Deprecated function. This functionality on icon objects"
            " will be dropped on a next release.");
        _elm_icon_signal_emit(obj, emission, source);
     }
}

EAPI void
elm_widget_signal_callback_add(Eo *obj, const char *emission, const char *source, Edje_Signal_Cb func, void *data)
{
   ELM_WIDGET_CHECK(obj);
   EINA_SAFETY_ON_NULL_RETURN(func);
   if (evas_object_smart_type_check(obj, "elm_layout"))
     elm_layout_signal_callback_add(obj, emission, source, func, data);
   else if (evas_object_smart_type_check(obj, "elm_icon"))
     {
        WRN("Deprecated function. This functionality on icon objects"
            " will be dropped on a next release.");

        _elm_icon_signal_callback_add(obj, emission, source, func, data);
     }
}

EAPI void *
elm_widget_signal_callback_del(Eo *obj, const char *emission, const char *source, Edje_Signal_Cb func)
{
   void *data = NULL;

   ELM_WIDGET_CHECK(obj) NULL;
   EINA_SAFETY_ON_NULL_RETURN_VAL(func, NULL);
   if (evas_object_smart_type_check(obj, "elm_layout"))
     data = elm_layout_signal_callback_del(obj, emission, source, func);
   else if (evas_object_smart_type_check(obj, "elm_icon"))
     {
        WRN("Deprecated function. This functionality on icon objects"
            " will be dropped on a next release.");

        data = _elm_icon_signal_callback_del(obj, emission, source, func);
     }

   return data;
}


/* Widget Shadow Begin */

typedef struct _Widget_Shadow
{
   Eo *widget;
   Eo *surface;
   struct {
      double rx, ry, ox, oy, grow;
      int r, g, b, a;
   } props;
   Eina_Stringshare *code, *name;
} Widget_Shadow;

static void _widget_shadow_update(Widget_Shadow *shadow);

static void
_widget_shadow_del_cb(void *data, const Efl_Event *ev EINA_UNUSED)
{
   Widget_Shadow *shadow = data;

   efl_del(shadow->surface);
   free(shadow);
}

static void
_widget_shadow_event_cb(void *data, const Efl_Event *ev EINA_UNUSED)
{
   Widget_Shadow *shadow = data;
   _widget_shadow_update(shadow);
}

EFL_CALLBACKS_ARRAY_DEFINE(widget_shadow_cb,
{ EFL_EVENT_DEL, _widget_shadow_del_cb },
{ EFL_GFX_ENTITY_EVENT_POSITION_CHANGED, _widget_shadow_event_cb },
{ EFL_GFX_ENTITY_EVENT_SIZE_CHANGED, _widget_shadow_event_cb },
{ EFL_GFX_ENTITY_EVENT_STACKING_CHANGED, _widget_shadow_event_cb },
{ EFL_GFX_ENTITY_EVENT_VISIBILITY_CHANGED, _widget_shadow_event_cb });

static Widget_Shadow *
_widget_shadow_part_get(const Eo *part_obj)
{
   Elm_Part_Data *pd = efl_data_scope_get(part_obj, EFL_UI_WIDGET_PART_CLASS);
   Widget_Shadow *shadow;
   Eo *widget = pd->obj;

   shadow = efl_key_data_get(widget, "__elm_shadow");
   if (!shadow)
     {
        shadow = calloc(1, sizeof(*shadow));
        if (!shadow) return NULL;
        shadow->widget = pd->obj;
        efl_key_data_set(widget, "__elm_shadow", shadow);
        efl_event_callback_array_add(widget, widget_shadow_cb(), shadow);
     }
   return shadow;
}

static void
_widget_shadow_update(Widget_Shadow *ws)
{
   int l = 0, r = 0, t = 0, b = 0;
   Eina_Rect srect, wrect;
   char filter[1024];

#define FILTER_FMT \
   "a = buffer { 'alpha' }" \
   "grow { %f, dst = a, alphaonly = true }" \
   "blur { src = a, rx = %f, ry = %f, color = color(%d,%d,%d,%d) }"

   if (!ws->surface)
     {
        ws->surface = efl_add(EFL_CANVAS_PROXY_CLASS, ws->widget);
        efl_gfx_fill_auto_set(ws->surface, 1);
        efl_canvas_proxy_source_clip_set(ws->surface, EINA_FALSE);
        efl_canvas_proxy_source_events_set(ws->surface, EINA_FALSE);
        efl_canvas_proxy_source_set(ws->surface, ws->widget);
     }

   if (!ws->code)
     {
        snprintf(filter, sizeof(filter), FILTER_FMT,
                 ws->props.grow, ws->props.rx, ws->props.ry,
                 ws->props.r, ws->props.g, ws->props.b, ws->props.a);
     }

   efl_gfx_filter_program_set(ws->surface,
                              ws->code ? ws->code : filter,
                              ws->name ? ws->name : "shadow");
   efl_gfx_filter_padding_get(ws->surface, &l, &r, &t, &b);

   wrect = efl_gfx_entity_geometry_get(ws->widget);
   srect.x = wrect.x + (int) (-l + ws->props.ox);
   srect.y = wrect.y + (int) (-t + ws->props.oy);
   srect.w = wrect.w + (int) (l + r);
   srect.h = wrect.h + (int) (t + b);

   if ((!ws->props.a && !ws->code) ||
       !efl_gfx_entity_visible_get(ws->widget))
     {
        efl_gfx_entity_visible_set(ws->surface, EINA_FALSE);
        return;
     }

   efl_canvas_object_clipper_set(ws->surface, efl_canvas_object_clipper_get(ws->widget));
   efl_canvas_group_member_add(efl_canvas_object_render_parent_get(ws->widget), ws->surface);
   efl_gfx_entity_geometry_set(ws->surface, srect);
   efl_gfx_stack_below(ws->surface, ws->widget);
   efl_gfx_entity_visible_set(ws->surface, EINA_TRUE);
}

static void
_elm_widget_shadow_update(Efl_Ui_Widget *obj)
{
   Widget_Shadow *shadow = _widget_shadow_part_get(obj);
   _widget_shadow_update(shadow);
}

EOLIAN static void
_efl_ui_widget_part_shadow_efl_gfx_blur_offset_set(Eo *obj, void *_pd EINA_UNUSED, double ox, double oy)
{
   Widget_Shadow *shadow = _widget_shadow_part_get(obj);
   shadow->props.ox = ox;
   shadow->props.oy = oy;
   _widget_shadow_update(shadow);
}

EOLIAN static void
_efl_ui_widget_part_shadow_efl_gfx_blur_offset_get(const Eo *obj, void *_pd EINA_UNUSED, double *ox, double *oy)
{
   Widget_Shadow *shadow = _widget_shadow_part_get(obj);
   if (ox) *ox = shadow->props.ox;
   if (oy) *oy = shadow->props.oy;
}

EOLIAN static void
_efl_ui_widget_part_shadow_efl_gfx_blur_radius_set(Eo *obj, void *_pd EINA_UNUSED, double rx, double ry)
{
   Widget_Shadow *shadow = _widget_shadow_part_get(obj);
   shadow->props.rx = rx;
   shadow->props.ry = ry;
   _widget_shadow_update(shadow);
}

EOLIAN static void
_efl_ui_widget_part_shadow_efl_gfx_blur_radius_get(const Eo *obj, void *_pd EINA_UNUSED, double *rx, double *ry)
{
   Widget_Shadow *shadow = _widget_shadow_part_get(obj);
   if (rx) *rx = shadow->props.rx;
   if (ry) *ry = shadow->props.ry;
}

EOLIAN static void
_efl_ui_widget_part_shadow_efl_gfx_color_color_set(Eo *obj, void *_pd EINA_UNUSED, int r, int g, int b, int a)
{
   Widget_Shadow *shadow = _widget_shadow_part_get(obj);
   shadow->props.r = r;
   shadow->props.g = g;
   shadow->props.b = b;
   shadow->props.a = a;
   _widget_shadow_update(shadow);
}

EOLIAN static void
_efl_ui_widget_part_shadow_efl_gfx_color_color_get(const Eo *obj, void *_pd EINA_UNUSED, int *r, int *g, int *b, int *a)
{
   Widget_Shadow *shadow = _widget_shadow_part_get(obj);
   if (r) *r = shadow->props.r;
   if (g) *g = shadow->props.g;
   if (b) *b = shadow->props.b;
   if (a) *a = shadow->props.a;
}

EOLIAN static void
_efl_ui_widget_part_shadow_efl_gfx_blur_grow_set(Eo *obj, void *_pd EINA_UNUSED, double radius)
{
   Widget_Shadow *shadow = _widget_shadow_part_get(obj);
   shadow->props.grow = radius;
   _widget_shadow_update(shadow);
}

EOLIAN static double
_efl_ui_widget_part_shadow_efl_gfx_blur_grow_get(const Eo *obj, void *_pd EINA_UNUSED)
{
   Widget_Shadow *shadow = _widget_shadow_part_get(obj);
   return shadow->props.grow;
}

EOLIAN static void
_efl_ui_widget_part_shadow_efl_gfx_filter_filter_program_set(Eo *obj, void *_pd EINA_UNUSED, const char *code, const char *name)
{
   Widget_Shadow *ws = _widget_shadow_part_get(obj);
   eina_stringshare_replace(&ws->code, code);
   eina_stringshare_replace(&ws->name, name);
   _widget_shadow_update(ws);
}

EOLIAN static void
_efl_ui_widget_part_shadow_efl_gfx_filter_filter_program_get(const Eo *obj, void *_pd EINA_UNUSED, const char **code, const char **name)
{
   Widget_Shadow *ws = _widget_shadow_part_get(obj);
   efl_gfx_filter_program_get(ws->surface, code, name);
}

EOLIAN static void
_efl_ui_widget_part_shadow_efl_gfx_filter_filter_source_set(Eo *obj, void *_pd EINA_UNUSED, const char *name, Efl_Gfx_Entity *source)
{
   Widget_Shadow *ws = _widget_shadow_part_get(obj);
   _widget_shadow_update(ws);
   efl_gfx_filter_source_set(ws->surface, name, source);
}

EOLIAN static Efl_Gfx_Entity *
_efl_ui_widget_part_shadow_efl_gfx_filter_filter_source_get(const Eo *obj, void *_pd EINA_UNUSED, const char *name)
{
   Widget_Shadow *ws = _widget_shadow_part_get(obj);
   return efl_gfx_filter_source_get(ws->surface, name);
}

EOLIAN static void
_efl_ui_widget_part_shadow_efl_gfx_filter_filter_data_set(Eo *obj, void *_pd EINA_UNUSED, const char *name, const char *value, Eina_Bool execute)
{
   Widget_Shadow *ws = _widget_shadow_part_get(obj);
   _widget_shadow_update(ws);
   efl_gfx_filter_data_set(ws->surface, name, value, execute);
}

EOLIAN static void
_efl_ui_widget_part_shadow_efl_gfx_filter_filter_data_get(const Eo *obj, void *_pd EINA_UNUSED, const char *name, const char **value, Eina_Bool *execute)
{
   Widget_Shadow *ws = _widget_shadow_part_get(obj);
   efl_gfx_filter_data_get(ws->surface, name, value, execute);
}

EOLIAN static void
_efl_ui_widget_part_shadow_efl_gfx_filter_filter_padding_get(const Eo *obj, void *_pd EINA_UNUSED, int *l, int *r, int *t, int *b)
{
   Widget_Shadow *ws = _widget_shadow_part_get(obj);
   efl_gfx_filter_padding_get(ws->surface, l, r, t, b);
}

EOLIAN static void
_efl_ui_widget_part_shadow_efl_gfx_filter_filter_state_set(Eo *obj, void *_pd EINA_UNUSED, const char *cur_state, double cur_val, const char *next_state, double next_val, double pos)
{
   Widget_Shadow *ws = _widget_shadow_part_get(obj);
   efl_gfx_filter_state_set(ws->surface, cur_state, cur_val, next_state, next_val, pos);
}

EOLIAN static void
_efl_ui_widget_part_shadow_efl_gfx_filter_filter_state_get(const Eo *obj, void *_pd EINA_UNUSED, const char **cur_state, double *cur_val, const char **next_state, double *next_val, double *pos)
{
   Widget_Shadow *ws = _widget_shadow_part_get(obj);
   efl_gfx_filter_state_get(ws->surface, cur_state, cur_val, next_state, next_val, pos);
}

#include "efl_ui_widget_part_shadow.eo.c"

/* Widget Shadow End */


/* Efl.Part implementation */

EOLIAN static Efl_Object *
_efl_ui_widget_efl_part_part_get(const Eo *obj, Elm_Widget_Smart_Data *wd EINA_UNUSED, const char *part)
{
   if (eina_streq(part, "background"))
     return ELM_PART_IMPLEMENT(EFL_UI_WIDGET_PART_BG_CLASS, obj, part);
   else if (eina_streq(part, "shadow"))
     return ELM_PART_IMPLEMENT(EFL_UI_WIDGET_PART_SHADOW_CLASS, obj, part);
   return ELM_PART_IMPLEMENT(EFL_UI_WIDGET_PART_CLASS, obj, part);
}

EOLIAN static void \
_efl_ui_widget_part_efl_object_destructor(Eo *obj, Elm_Part_Data *pd)
{
   ELM_PART_HOOK;
   eina_tmpstr_del(pd->part);
   efl_destructor(efl_super(obj, EFL_UI_WIDGET_PART_CLASS));
}

#include "efl_ui_widget_part.eo.c"

/* Efl.Part end */

/* Efl.Part Bg implementation */

Efl_Canvas_Object *
_efl_ui_widget_bg_get(const Efl_Ui_Widget *obj)
{
   Elm_Widget_Smart_Data *sd = efl_data_scope_get(obj, MY_CLASS);
   Evas_Object *bg_obj = sd->bg;

   if (!bg_obj)
     {
        bg_obj = efl_add(EFL_UI_BG_CLASS, obj);
        EINA_SAFETY_ON_NULL_RETURN_VAL(bg_obj, NULL);
        sd->bg = bg_obj;
        efl_canvas_group_member_add(obj, sd->bg);
        evas_object_stack_below(sd->bg, sd->resize_obj);
        _smart_reconfigure(sd);
     }

   return bg_obj;
}

static inline Efl_Canvas_Object *
efl_ui_widget_part_bg_get(const Eo *part_obj)
{
   Elm_Part_Data *pd = efl_data_scope_get(part_obj, EFL_UI_WIDGET_PART_CLASS);
   return _efl_ui_widget_bg_get(pd->obj);
}

EOLIAN static Eina_Error
_efl_ui_widget_part_bg_efl_file_load(Eo *obj, void *pd EINA_UNUSED)
{
   Evas_Object *bg_obj = efl_ui_widget_part_bg_get(obj);

   return efl_file_load(bg_obj);
}

EOLIAN static const char *
_efl_ui_widget_part_bg_efl_file_file_get(const Eo *obj, void *pd EINA_UNUSED)
{
   Evas_Object *bg_obj = efl_ui_widget_part_bg_get(obj);

   return efl_file_get(bg_obj);
}

EOLIAN static Eina_Error
_efl_ui_widget_part_bg_efl_file_file_set(Eo *obj, void *pd EINA_UNUSED, const char *file)
{
   Evas_Object *bg_obj = efl_ui_widget_part_bg_get(obj);

   return efl_file_set(bg_obj, file);
}

EOLIAN static const char *
_efl_ui_widget_part_bg_efl_file_key_get(const Eo *obj, void *pd EINA_UNUSED)
{
   Evas_Object *bg_obj = efl_ui_widget_part_bg_get(obj);

   return efl_file_key_get(bg_obj);
}

EOLIAN static void
_efl_ui_widget_part_bg_efl_file_key_set(Eo *obj, void *pd EINA_UNUSED, const char *key)
{
   Evas_Object *bg_obj = efl_ui_widget_part_bg_get(obj);

   efl_file_key_set(bg_obj, key);
}

EOLIAN static const Eina_File *
_efl_ui_widget_part_bg_efl_file_mmap_get(const Eo *obj, void *pd EINA_UNUSED)
{
   Evas_Object *bg_obj = efl_ui_widget_part_bg_get(obj);

   return efl_file_mmap_get(bg_obj);
}

EOLIAN static Eina_Error
_efl_ui_widget_part_bg_efl_file_mmap_set(Eo *obj, void *pd EINA_UNUSED, const Eina_File *file)
{
   Evas_Object *bg_obj = efl_ui_widget_part_bg_get(obj);

   return efl_file_mmap_set(bg_obj, file);
}

EOLIAN static void
_efl_ui_widget_part_bg_efl_gfx_color_color_set(Eo *obj, void *pd EINA_UNUSED, int r, int g, int b, int a)
{
   Evas_Object *bg_obj = efl_ui_widget_part_bg_get(obj);

   efl_gfx_color_set(bg_obj, r, g, b, a);
}

EOLIAN static void
_efl_ui_widget_part_bg_efl_gfx_color_color_get(const Eo *obj, void *pd EINA_UNUSED, int *r, int *g, int *b, int *a)
{
   Evas_Object *bg_obj = efl_ui_widget_part_bg_get(obj);

   efl_gfx_color_get(bg_obj, r, g, b, a);
}

EOLIAN static void
_efl_ui_widget_part_bg_efl_gfx_image_scale_type_set(Eo *obj, void *pd EINA_UNUSED, Efl_Gfx_Image_Scale_Type scale_type)
{
   Evas_Object *bg_obj = efl_ui_widget_part_bg_get(obj);

   efl_gfx_image_scale_type_set(bg_obj, scale_type);
}

EOLIAN static Efl_Gfx_Image_Scale_Type
_efl_ui_widget_part_bg_efl_gfx_image_scale_type_get(const Eo *obj, void *pd EINA_UNUSED)

{
   Evas_Object *bg_obj = efl_ui_widget_part_bg_get(obj);

   return efl_gfx_image_scale_type_get(bg_obj);
}

typedef struct _Efl_Ui_Property_Bound Efl_Ui_Property_Bound;
struct _Efl_Ui_Property_Bound
{
   Eina_Stringshare *key; // Local object property
   Eina_Stringshare *property; // Model property
   Eina_Future *f;
};

static void
_efl_ui_property_bind_free(void *data)
{
   Efl_Ui_Property_Bound *prop = data;

   eina_stringshare_del(prop->key);
   eina_stringshare_del(prop->property);
   free(prop);
}

static void
_efl_ui_property_bind_clean(Eo *obj EINA_UNUSED,
                            void *data,
                            const Eina_Future *f EINA_UNUSED)
{
   Efl_Ui_Property_Bound *prop = data;

   prop->f = NULL;
}

static void
_efl_ui_property_bind_get(Efl_Ui_Widget_Data *pd, Efl_Ui_Property_Bound *prop)
{
   Eina_Value *value = efl_model_property_get(pd->properties.model, prop->property);
   Eina_Future *f;
   Eina_Error err;

   err = efl_property_reflection_set(pd->obj, prop->key, eina_value_reference_copy(value));
   eina_value_free(value);

   if (!err) return ;

   // Report back the error to the model
   if (prop->f) eina_future_cancel(prop->f);
   f = efl_model_property_set(pd->properties.model, prop->property,
                              eina_value_error_new(err));
   prop->f = efl_future_then(pd->obj, f, .free = _efl_ui_property_bind_clean, .data = prop);
}

static void
_efl_ui_property_bind_set(Efl_Ui_Widget_Data *pd, Efl_Ui_Property_Bound *prop)
{
   Eina_Value value = efl_property_reflection_get(pd->obj, prop->key);
   Eina_Future *f;

   if (prop->f) eina_future_cancel(prop->f);
   f = efl_model_property_set(pd->properties.model, prop->property, eina_value_dup(&value));
   prop->f = efl_future_then(pd->obj, f, .free = _efl_ui_property_bind_clean, .data = prop);
   eina_value_flush(&value);
}

static void
_efl_ui_model_property_bind_changed(void *data, const Efl_Event *event)
{
   Efl_Model_Property_Event *evt = event->info;
   Efl_Ui_Widget_Data *pd = data;
   Eina_Array_Iterator it;
   const char *prop;
   unsigned int i;

   EINA_ARRAY_ITER_NEXT(evt->changed_properties, i, prop, it)
     {
        Efl_Ui_Property_Bound *lookup;

        lookup = eina_hash_find(pd->properties.model_lookup, prop);
        if (lookup) _efl_ui_property_bind_get(pd, lookup);
     }
}

static void
_efl_ui_view_property_bind_changed(void *data, const Efl_Event *event)
{
   Efl_Ui_Property_Event *evt = event->info;
   Efl_Ui_Widget_Data *pd = data;
   Eina_Array_Iterator it;
   Eina_Stringshare *prop;
   unsigned int i;

   EINA_ARRAY_ITER_NEXT(evt->changed_properties, i, prop, it)
     {
        Efl_Ui_Property_Bound *lookup;

        lookup = eina_hash_find(pd->properties.view_lookup, prop);
        if (lookup) _efl_ui_property_bind_set(pd, lookup);
     }
}

static Eina_Error
_efl_ui_widget_efl_ui_property_bind_property_bind(Eo *obj, Efl_Ui_Widget_Data *pd,
                                                  const char *key, const char *property)
{
   Efl_Ui_Property_Bound *prop;

   // Check if the property is available from the reflection table of the object.
   if (!efl_property_reflection_exist(obj, key)) return EFL_PROPERTY_ERROR_INVALID_KEY;

   if (!pd->properties.model_lookup)
     {
        pd->properties.model_lookup = eina_hash_stringshared_new(_efl_ui_property_bind_free);
        pd->properties.view_lookup = eina_hash_stringshared_new(NULL);
        if (pd->properties.model)
          {
             efl_event_callback_add(pd->properties.model, EFL_MODEL_EVENT_PROPERTIES_CHANGED,
                                    _efl_ui_model_property_bind_changed, pd);
             efl_event_callback_add(obj, EFL_UI_PROPERTY_BIND_EVENT_PROPERTIES_CHANGED,
                                    _efl_ui_view_property_bind_changed, pd);
          }
     }

   prop = calloc(1, sizeof (Efl_Ui_Property_Bound));
   if (!prop) return ENOMEM;
   prop->key = eina_stringshare_add(key);
   prop->property = eina_stringshare_add(property);

   eina_hash_direct_add(pd->properties.model_lookup, prop->property, prop);
   eina_hash_direct_add(pd->properties.view_lookup, prop->key, prop);

   _efl_ui_property_bind_get(pd, prop);

   efl_event_callback_call(obj, EFL_UI_PROPERTY_BIND_EVENT_PROPERTY_BOUND, (void*) prop->key);

   return 0;
}

static void
_efl_ui_widget_efl_ui_view_model_set(Eo *obj,
                                     Efl_Ui_Widget_Data *pd,
                                     Efl_Model *model)
{
   if (pd->properties.model)
     {
        // Remove any existing handler that might exist for any reason
        efl_event_callback_del(pd->properties.model, EFL_MODEL_EVENT_PROPERTIES_CHANGED,
                               _efl_ui_model_property_bind_changed, pd);
        efl_event_callback_del(obj, EFL_UI_PROPERTY_BIND_EVENT_PROPERTIES_CHANGED,
                               _efl_ui_view_property_bind_changed, pd);
     }

   efl_replace(&pd->properties.model, model);

   if (pd->properties.model && pd->properties.model_lookup)
     {
        // Set the properties handler just in case
        efl_event_callback_add(pd->properties.model, EFL_MODEL_EVENT_PROPERTIES_CHANGED,
                               _efl_ui_model_property_bind_changed, pd);
        efl_event_callback_add(obj, EFL_UI_PROPERTY_BIND_EVENT_PROPERTIES_CHANGED,
                               _efl_ui_view_property_bind_changed, pd);
     }
}

static Efl_Model *
_efl_ui_widget_efl_ui_view_model_get(const Eo *obj EINA_UNUSED, Efl_Ui_Widget_Data *pd)
{
   return pd->properties.model;
}

static void
_efl_ui_widget_efl_object_invalidate(Eo *obj, Efl_Ui_Widget_Data *pd)
{
   efl_invalidate(efl_super(obj, EFL_UI_WIDGET_CLASS));

   if (pd->properties.model)
     {
        efl_event_callback_del(pd->properties.model, EFL_MODEL_EVENT_PROPERTIES_CHANGED,
                               _efl_ui_model_property_bind_changed, pd);
        efl_event_callback_del(obj, EFL_UI_PROPERTY_BIND_EVENT_PROPERTIES_CHANGED,
                               _efl_ui_view_property_bind_changed, pd);
        efl_replace(&pd->properties.model, NULL);
     }
   if (pd->properties.view_lookup) eina_hash_free(pd->properties.view_lookup);
   pd->properties.view_lookup = NULL;
   if (pd->properties.model_lookup) eina_hash_free(pd->properties.model_lookup);
   pd->properties.model_lookup = NULL;
}

#include "efl_ui_widget_part_bg.eo.c"

/* Efl.Part Bg end */


/* Internal EO APIs and hidden overrides */

EFL_FUNC_BODY_CONST(efl_ui_widget_default_content_part_get, const char *, NULL)
EFL_FUNC_BODY_CONST(efl_ui_widget_default_text_part_get, const char *, NULL)

ELM_PART_CONTENT_DEFAULT_GET(efl_ui_widget, NULL)
ELM_PART_TEXT_DEFAULT_GET(efl_ui_widget, NULL)

/***********************************************************************************
 * TIZEN_ONLY_FEATURE: apply Tizen's color_class features.                         *
 ***********************************************************************************/
/* Internal EO APIs and hidden overrides */
EAPI EFL_FUNC_BODYV(elm_widget_class_color_set, Eina_Bool, EINA_FALSE,
                    EFL_FUNC_CALL(color_class, r, g, b, a),
                    const char *color_class, int r, int g, int b, int a)
EAPI EFL_FUNC_BODYV(elm_widget_class_color_get, Eina_Bool, EINA_FALSE,
                    EFL_FUNC_CALL(color_class, r, g, b, a),
                    const char *color_class, int *r, int *g, int *b, int *a)
EAPI EFL_FUNC_BODYV(elm_widget_class_color2_set, Eina_Bool, EINA_FALSE,
                    EFL_FUNC_CALL(color_class, r, g, b, a),
                    const char *color_class, int r, int g, int b, int a)
EAPI EFL_FUNC_BODYV(elm_widget_class_color2_get, Eina_Bool, EINA_FALSE,
                    EFL_FUNC_CALL(color_class, r, g, b, a),
                    const char *color_class, int *r, int *g, int *b, int *a)
EAPI EFL_FUNC_BODYV(elm_widget_class_color3_set, Eina_Bool, EINA_FALSE,
                    EFL_FUNC_CALL(color_class, r, g, b, a),
                    const char *color_class, int r, int g, int b, int a)
EAPI EFL_FUNC_BODYV(elm_widget_class_color3_get, Eina_Bool, EINA_FALSE,
                    EFL_FUNC_CALL(color_class, r, g, b, a),
                    const char *color_class, int *r, int *g, int *b, int *a)
EAPI EFL_VOID_FUNC_BODYV(elm_widget_class_color_del,
                         EFL_FUNC_CALL(color_class),
                         const char *color_class)
EAPI EFL_VOID_FUNC_BODY(elm_widget_class_color_clear)

static Eina_Bool _elm_widget_class_color_set(Eo *obj, Elm_Widget_Smart_Data *sd EINA_UNUSED, const char *color_class, int r, int g, int b, int a);
static Eina_Bool _elm_widget_class_color_get(Eo *obj, Elm_Widget_Smart_Data *sd EINA_UNUSED, const char *color_class, int *r, int *g, int *b, int *a);
static Eina_Bool _elm_widget_class_color2_set(Eo *obj, Elm_Widget_Smart_Data *sd EINA_UNUSED, const char *color_class, int r, int g, int b, int a);
static Eina_Bool _elm_widget_class_color2_get(Eo *obj, Elm_Widget_Smart_Data *sd EINA_UNUSED, const char *color_class, int *r, int *g, int *b, int *a);
static Eina_Bool _elm_widget_class_color3_set(Eo *obj, Elm_Widget_Smart_Data *sd EINA_UNUSED, const char *color_class, int r, int g, int b, int a);
static Eina_Bool _elm_widget_class_color3_get(Eo *obj, Elm_Widget_Smart_Data *sd EINA_UNUSED, const char *color_class, int *r, int *g, int *b, int *a);
static void _elm_widget_class_color_del(Eo *obj EINA_UNUSED, Elm_Widget_Smart_Data *sd, const char *color_class);
static void _elm_widget_class_color_clear(Eo *obj EINA_UNUSED, Elm_Widget_Smart_Data *sd);
/*******
 * END *
 *******/


#define EFL_UI_WIDGET_EXTRA_OPS \
   EFL_CANVAS_GROUP_ADD_DEL_OPS(efl_ui_widget), \
   ELM_PART_CONTENT_DEFAULT_OPS(efl_ui_widget), \
   ELM_PART_TEXT_DEFAULT_OPS(efl_ui_widget), \
   EFL_OBJECT_OP_FUNC(efl_canvas_object_is_frame_object_set, _efl_ui_widget_efl_canvas_object_is_frame_object_set), \
   EFL_OBJECT_OP_FUNC(efl_dbg_info_get, _efl_ui_widget_efl_object_dbg_info_get), \
/***********************************************************************************  \
 * TIZEN_ONLY_FEATURE: apply Tizen's color_class features.                         *  \
 ***********************************************************************************/ \
   EFL_OBJECT_OP_FUNC(elm_widget_class_color_set, _elm_widget_class_color_set), \
   EFL_OBJECT_OP_FUNC(elm_widget_class_color_get, _elm_widget_class_color_get), \
   EFL_OBJECT_OP_FUNC(elm_widget_class_color2_set, _elm_widget_class_color2_set), \
   EFL_OBJECT_OP_FUNC(elm_widget_class_color2_get, _elm_widget_class_color2_get), \
   EFL_OBJECT_OP_FUNC(elm_widget_class_color3_set, _elm_widget_class_color3_set), \
   EFL_OBJECT_OP_FUNC(elm_widget_class_color3_get, _elm_widget_class_color3_get), \
   EFL_OBJECT_OP_FUNC(elm_widget_class_color_del, _elm_widget_class_color_del), \
   EFL_OBJECT_OP_FUNC(elm_widget_class_color_clear, _elm_widget_class_color_clear)

/*******
 * END *
 *******/

// TIZEN_ONLY(20150709) : atspi relations api
EOLIAN static Efl_Access_Relation_Set
_efl_ui_widget_efl_access_object_relation_set_get(const Eo *obj EINA_UNUSED, Elm_Widget_Smart_Data *sd)
{
   return efl_access_relation_set_clone(&sd->atspi_custom_relations);
}
EOLIAN static Eina_Bool
_efl_ui_widget_efl_access_object_relationship_append(Eo *obj EINA_UNUSED, Elm_Widget_Smart_Data *sd, Efl_Access_Relation_Type type, const Eo *relation_obj)
{
   return efl_access_relation_set_relation_append(&sd->atspi_custom_relations, type, relation_obj);
}

EOLIAN static void
_efl_ui_widget_efl_access_object_relationship_remove(Eo *obj EINA_UNUSED, Elm_Widget_Smart_Data *sd, Efl_Access_Relation_Type type, const Eo *relation_obj)
{
   efl_access_relation_set_relation_remove(&sd->atspi_custom_relations, type, relation_obj);
}

EOLIAN static void
_efl_ui_widget_efl_access_object_relationships_clear(Eo *obj EINA_UNUSED, Elm_Widget_Smart_Data *sd)
{
   efl_access_relation_set_free(&sd->atspi_custom_relations);
   sd->atspi_custom_relations = NULL;
}

EOLIAN static Elm_Atspi_Relation_Set
_elm_widget_item_efl_access_object_relation_set_get(const Eo *obj EINA_UNUSED, Elm_Widget_Item_Data *sd)
{
   return efl_access_relation_set_clone(&sd->atspi_custom_relations);
}

EOLIAN static Eina_Bool
_elm_widget_item_efl_access_object_relationship_append(Eo *obj EINA_UNUSED, Elm_Widget_Item_Data *sd, Efl_Access_Relation_Type type, const Eo *relation_obj)
{
   return efl_access_relation_set_relation_append(&sd->atspi_custom_relations, type, relation_obj);
}

EOLIAN static void
_elm_widget_item_efl_access_object_relationship_remove(Eo *obj EINA_UNUSED, Elm_Widget_Item_Data *sd, Efl_Access_Relation_Type type, const Eo *relation_obj)
{
   efl_access_relation_set_relation_remove(&sd->atspi_custom_relations, type, relation_obj);
}

EOLIAN static void
_elm_widget_item_efl_access_object_relationships_clear(Eo *obj EINA_UNUSED, Elm_Widget_Item_Data *sd)
{
   efl_access_relation_set_free(&sd->atspi_custom_relations);
   sd->atspi_custom_relations = NULL;
}
//////////////////////////////

//TIZEN_ONLY(20160726): add API elm_atspi_accessible_can_highlight_set/get
static Eina_Bool
_children_highlight_check(Eo *obj)
{
   Eina_List *children, *l;
   Eo *child;

   if (_elm_object_accessibility_currently_highlighted_get() == (void *)obj)
     {
        efl_access_component_highlight_clear(obj);
        return EINA_TRUE;
     }

   children = efl_access_object_access_children_get(obj);
   EINA_LIST_FOREACH(children, l, child)
     {
        if (_children_highlight_check(child)) return EINA_TRUE;
     }

   return EINA_FALSE;
}

EOLIAN static void
_efl_ui_widget_efl_access_object_can_highlight_set(Eo *obj EINA_UNUSED, Elm_Widget_Smart_Data *_pd, Eina_Bool can_highlight)
{
   if (!can_highlight) _children_highlight_check(obj);
   _pd->can_highlight = !!can_highlight;
}

EOLIAN static Eina_Bool
_efl_ui_widget_efl_access_object_can_highlight_get(const Eo *obj, Elm_Widget_Smart_Data *_pd EINA_UNUSED)
{
   return _elm_widget_highlightable(obj);
}

EOLIAN static void
_elm_widget_item_efl_access_object_can_highlight_set(Eo *obj EINA_UNUSED, Elm_Widget_Item_Data *_pd, Eina_Bool can_highlight)
{
   if (!can_highlight) _children_highlight_check(obj);
   _pd->can_highlight = !!can_highlight;
}

EOLIAN static Eina_Bool
_elm_widget_item_efl_access_object_can_highlight_get(const Eo *obj, Elm_Widget_Item_Data *_pd EINA_UNUSED)
{
   return _elm_widget_item_highlightable(obj);
}
//

// TIZEN_ONLY(20150705): Genlist item align feature
EAPI void
elm_widget_scroll_item_align_enabled_set(Evas_Object *obj,
                                        Eina_Bool scroll_item_align_enable)
{
   API_ENTRY return;
   if (sd->scroll_item_align_enable == scroll_item_align_enable) return;
   sd->scroll_item_align_enable = scroll_item_align_enable;
}

EAPI Eina_Bool
elm_widget_scroll_item_align_enabled_get(const Evas_Object *obj)
{
   API_ENTRY return EINA_FALSE;
   return sd->scroll_item_align_enable;
}


//TIZEN_ONLY(20150717) add widget name setter
EOLIAN void
_efl_ui_widget_efl_access_object_i18n_name_set(Eo *obj EINA_UNUSED, Elm_Widget_Smart_Data* _pd EINA_UNUSED, const char *name)
{
   if (_pd->name)
     eina_stringshare_del(_pd->name);

   _pd->name = eina_stringshare_add(name);
}
//

//TIZEN_ONLY(20161111) add widget/widget_item description get/set
EOLIAN void
_efl_ui_widget_efl_access_object_description_set(Eo *obj EINA_UNUSED, Efl_Ui_Widget_Data* _pd, const char *description)
{
   if (_pd->description)
     eina_stringshare_del(_pd->description);

   _pd->description = eina_stringshare_add(description);
}

EOLIAN static const char*
_efl_ui_widget_efl_access_object_description_get(const Eo *obj EINA_UNUSED, Efl_Ui_Widget_Data *_pd EINA_UNUSED)
{
   const char *ret = NULL;
   ret = efl_access_object_description_get(efl_super(obj, EFL_UI_WIDGET_CLASS));
   if (ret) return ret;

#ifdef HAVE_GETTEXT
   if (_pd->atspi_translation_domain)
     return dgettext(_pd->atspi_translation_domain, _pd->description);
#endif
   return _pd->description;
}
//

//TIZEN_ONLY(20171108): make atspi_proxy work
static void
_proxy_widget_move_cb(void *data, Evas *e EINA_UNUSED, Evas_Object *obj, void *event_info EINA_UNUSED)
{
   Evas_Coord x, y;
   Eo *proxy = data;

   evas_object_geometry_get(obj, &x, &y, NULL, NULL);
   elm_atspi_bridge_utils_proxy_offset_set(proxy, x, y);
}

static void
_on_widget_del(void *data, const Efl_Event *event)
{
   Eo *plug = data;
   evas_object_event_callback_del_full(event->object, EVAS_CALLBACK_MOVE,
                                       _proxy_widget_move_cb, plug);
   efl_del(plug);
}

static void
_on_proxy_connected_cb(void *data, const Efl_Event *event)
{
   Evas_Coord x, y;
   Evas_Object *widget = data;

   evas_object_geometry_get(widget, &x, &y, NULL, NULL);
   elm_atspi_bridge_utils_proxy_offset_set(event->object, x, y);

   evas_object_event_callback_add(widget, EVAS_CALLBACK_MOVE, _proxy_widget_move_cb, event->object);
}
//

//TIZEN_ONLY(20161111) add widget/widget_item description get/set
EOLIAN void
_elm_widget_item_efl_access_object_description_set(Eo *obj EINA_UNUSED, Elm_Widget_Item_Data* _pd EINA_UNUSED, const char *description)
{
   if (_pd->description)
     eina_stringshare_del(_pd->description);

   _pd->description = eina_stringshare_add(description);
}

EOLIAN const char*
_elm_widget_item_efl_access_object_description_get(const Eo *obj EINA_UNUSED, Elm_Widget_Item_Data *_pd EINA_UNUSED)
{
   const char *ret = NULL;
   ret = efl_access_object_description_get(efl_super(obj, ELM_WIDGET_ITEM_CLASS));
   if (ret) return ret;

#ifdef HAVE_GETTEXT
   if (_pd->atspi_translation_domain)
     return dgettext(_pd->atspi_translation_domain, _pd->description);
#endif
   return _pd->description;
}
//

//TIZEN_ONLY(20150713) : add atspi name setter to widget_item
EOLIAN void
_elm_widget_item_efl_access_object_i18n_name_set(Eo *obj EINA_UNUSED, Elm_Widget_Item_Data* _pd EINA_UNUSED, const char *name)
{
   if (_pd->name)
     eina_stringshare_del(_pd->name);

   _pd->name = eina_stringshare_add(name);
}

EOLIAN const char*
_elm_widget_item_efl_access_object_i18n_name_get(const Eo *obj EINA_UNUSED, Elm_Widget_Item_Data *_pd EINA_UNUSED)
{
   //TIZEN_ONLY(20190922): add name callback, description callback.
   const char *ret = NULL;
   ret = efl_access_object_i18n_name_get(efl_super(obj, ELM_WIDGET_ITEM_CLASS));
   if (ret) return ret;
   //

   if (_pd->name)
     {
#ifdef HAVE_GETTEXT
        if (_pd->atspi_translation_domain)
          return dgettext(_pd->atspi_translation_domain, _pd->name);
#endif
        return _pd->name;
     }

   return NULL;
}
///

//TIZEN_ONLY(20150731) : add i18n support for name and description
EOLIAN static void
_efl_ui_widget_efl_access_object_translation_domain_set(Eo *obj EINA_UNUSED, Elm_Widget_Smart_Data *_pd, const char *domain)
{
   eina_stringshare_replace(&_pd->atspi_translation_domain, domain);
}

EOLIAN static const char*
_efl_ui_widget_efl_access_object_translation_domain_get(const Eo *obj EINA_UNUSED, Elm_Widget_Smart_Data *_pd)
{
   return _pd->atspi_translation_domain;
}

EOLIAN static void
_elm_widget_item_efl_access_object_translation_domain_set(Eo *obj EINA_UNUSED, Elm_Widget_Item_Data *_pd, const char *domain)
{
   eina_stringshare_replace(&_pd->atspi_translation_domain, domain);
}

EOLIAN static const char*
_elm_widget_item_efl_access_object_translation_domain_get(const Eo *obj EINA_UNUSED, Elm_Widget_Item_Data *_pd)
{
   return _pd->atspi_translation_domain;
}
///

static int _sort_vertically(const void *data1, const void *data2)
{
   Evas_Coord y1, y2;
   evas_object_geometry_get(data1, NULL, &y1, NULL, NULL);
   evas_object_geometry_get(data2, NULL, &y2, NULL, NULL);

   return y1 < y2 ? -1 : 1;
}

static int _sort_horizontally(const void *data1, const void *data2)
{
   Evas_Coord x1, x2;
   evas_object_geometry_get(data1, &x1, NULL, NULL, NULL);
   evas_object_geometry_get(data2, &x2, NULL, NULL, NULL);

   return x1 < x2 ? -1 : 1;
}

static Eina_List *_lines_split(Eina_List *children)
{
   Eo *c;
   Eina_List *lines, *line, *l;
   Evas_Coord yl, y, hl, h;
   lines = line = NULL;

   if (!children) return NULL;

   EINA_LIST_FOREACH(children, l, c)
     {
        evas_object_geometry_get(c, NULL, &yl, NULL, &hl);

        /* remove child if its height == 0 */
        if (hl != 0) break;
     }

   EINA_LIST_FREE(children, c)
     {
        evas_object_geometry_get(c, NULL, &y, NULL, &h);

        /* remove child if its height == 0 */
        if (h == 0) continue;

        if ((yl + (int)(0.25 * hl)) >= y)
          {
             //same line
             line = eina_list_append(line,c);
          }
        else
          {
             // finish current line & start new
             lines = eina_list_append(lines, line);
             yl = y, hl = h;
             line = eina_list_append(NULL, c);
          }
     }

   return eina_list_append(lines, line);
}
//

//TIZEN_ONLY(20171114) atspi: integrate ewk_view with elementary accessibility
static void
_on_ewk_del(void *data, const Efl_Event *desc EINA_UNUSED)
{
   Eo *plug = data;
   efl_del(plug);
}
//

//TIZEN_ONLY(20170621) handle atspi proxy connection at runtime
Eo *
plug_type_proxy_get(const Eo *obj, Evas_Object *widget)
{
   Eo *proxy = NULL;
   const char *plug_id;
   char *svcname, *svcnum;

   if ((plug_id = evas_object_data_get(widget, "___PLUGID")) != NULL)
     {
        // TIZEN_ONLY(20160930) : endless recursion fix
        efl_access_object_attribute_append(efl_super(obj, MY_CLASS), "___PlugID", plug_id);
        efl_access_object_role_set(obj, EFL_ACCESS_ROLE_EMBEDDED);

        proxy = evas_object_data_get(widget, "__widget_proxy");
        // TIZEN_ONLY(20171109) : fix for invalid proxy object, when at-spi has been restarted
        if (proxy)
          {
            if (!evas_object_data_get(proxy, "__proxy_invalid")) return proxy;
            evas_object_data_del(widget, "__widget_proxy");
          }
        //

        if (_elm_atspi_bridge_plug_id_split(plug_id, &svcname, &svcnum))
          {
             proxy = _elm_atspi_bridge_utils_proxy_create(obj, svcname, atoi(svcnum), ELM_ATSPI_PROXY_TYPE_PLUG);
             evas_object_data_set(widget, "__widget_proxy", proxy);
             efl_event_callback_add(widget, EFL_EVENT_DEL, _on_widget_del, proxy);
             efl_event_callback_add(proxy, ELM_ATSPI_PROXY_EVENT_CONNECTED, _on_proxy_connected_cb, widget);
             elm_atspi_bridge_utils_proxy_connect(proxy);
             free(svcname);
             free(svcnum);
          }
     }

   return proxy;
}

EAPI Eo *
elm_widget_atspi_plug_type_proxy_get(Evas_Object *obj)
{
   Elm_Widget_Smart_Data *wd;
   Evas_Object *widget;
   Eina_List *l;

   wd = efl_data_scope_get(obj, EFL_UI_WIDGET_CLASS);
   if (!wd) return NULL;

   Eo *proxy = NULL;
   EINA_LIST_FOREACH(wd->subobjs, l, widget)
     {
        proxy = plug_type_proxy_get(obj, widget);
        if (proxy) break;
     }
   return proxy;
}
//


//TIZEN_ONLY(20161107): enhance elm_atspi_accessible_can_highlight_set to set can_hihglight property to its children
EAPI Eina_Bool
_elm_widget_highlightable(const Evas_Object *obj)
{
   Eo *parent;

   Elm_Widget_Smart_Data *wd = efl_data_scope_get(obj, EFL_UI_WIDGET_CLASS);
   if (!wd) return EINA_FALSE;
   if (!wd->can_highlight) return EINA_FALSE;

   parent = efl_provider_find(efl_parent_get(obj), EFL_ACCESS_OBJECT_MIXIN);
   while (parent && !efl_isa(parent, ELM_ATSPI_APP_OBJECT_CLASS))
     {
        //TIZEN_ONLY(20160929) : atspi: Improves how to find the can_highlight of the widget
        if (!_elm_widget_can_highlight_get_by_class(parent)) return EINA_FALSE;
        //
        parent = efl_provider_find(efl_parent_get(parent), EFL_ACCESS_OBJECT_MIXIN);
     }
   return EINA_TRUE;
}
//

EAPI void
elm_widget_scroll_item_valign_set(Evas_Object *obj,
                                  const char *scroll_item_valign)
{
   API_ENTRY return;
   if (sd->scroll_item_valign) eina_stringshare_del(sd->scroll_item_valign);
   if (!scroll_item_valign) sd->scroll_item_valign = NULL;
   else sd->scroll_item_valign = eina_stringshare_add(scroll_item_valign);
}

EAPI const char*
elm_widget_scroll_item_valign_get(const Evas_Object *obj)
{
   API_ENTRY return NULL;
   return sd->scroll_item_valign;
}
//

/* TIZEN_ONLY(20180504): add missing item class names and fix edje_class parse rule for legacy */
const char *
_elm_widget_item_legacy_type_get(const Evas_Object *obj)
{
   const char *ret;
   int i;

   ret = efl_class_name_get(efl_class_get(obj));

   /* If the given widget is created for legacy,
    * convert type name to legacy. */
   for (i = 0; legacy_type_table[i][0] ; i++)
     {
        if (eina_streq(ret, legacy_type_table[i][0]))
          return legacy_type_table[i][1];
     }

   return ret;
}
/* END */

EOLIAN static void
_efl_ui_widget_focus_disabled_handle(Eo *obj, Elm_Widget_Smart_Data *_pd EINA_UNUSED)
{
   efl_ui_widget_focus_tree_unfocusable_handle(obj);
}

EOLIAN static unsigned int
_efl_ui_widget_focus_order_get(const Eo *obj EINA_UNUSED, Elm_Widget_Smart_Data *sd)
{
   return sd->focus_order;
}

EOLIAN static Evas_Object*
_efl_ui_widget_newest_focus_order_get(const Eo *obj, Elm_Widget_Smart_Data *sd, unsigned int *newest_focus_order, Eina_Bool can_focus_only)
{
   const Eina_List *l;
   Evas_Object *child, *cur, *best;

   if (!evas_object_visible_get(obj)
       || (elm_widget_disabled_get(obj))
       || (elm_widget_tree_unfocusable_get(obj)))
     return NULL;

   best = NULL;
   if (*newest_focus_order < sd->focus_order)
     {
        if (!can_focus_only || elm_widget_can_focus_get(obj))
          {
             *newest_focus_order = sd->focus_order;
             best = (Evas_Object *)obj;
          }
     }
   EINA_LIST_FOREACH(sd->subobjs, l, child)
     {
        if (!_elm_widget_is(child)) continue;

        cur = efl_ui_widget_newest_focus_order_get
           (child, newest_focus_order, can_focus_only);
        if (!cur) continue;
        best = cur;
     }
   return best;
}

EOLIAN static Eina_Bool
_efl_ui_widget_focus_next_manager_is(Eo *obj EINA_UNUSED, Elm_Widget_Smart_Data *_pd EINA_UNUSED)
{
   WRN("The %s widget does not implement the \"focus_next/focus_next_manager_is\" functions.",
       efl_class_name_get(efl_class_get(obj)));
   return EINA_FALSE;
}

static Eina_Bool
_efl_ui_widget_focus_direction_manager_is(Eo *obj EINA_UNUSED, Elm_Widget_Smart_Data *_pd EINA_UNUSED)
{
   WRN("The %s widget does not implement the \"focus_direction/focus_direction_manager_is\" functions.",
       efl_class_name_get(efl_class_get(obj)));
   return EINA_FALSE;
}
//
//TIZEN_ONLY(20180607): Restore legacy focus
EOLIAN static void
_efl_ui_widget_focus_mouse_up_handle(Eo *obj, Elm_Widget_Smart_Data *_pd EINA_UNUSED)
{
   if (!obj) return;
   if (!_is_focusable(obj)) return;

   efl_ui_widget_focus_steal(obj, NULL);
}

//TIZEN_ONLY(20160929) : atspi: Improves how to find the can_highlight of the widget
static Eina_Bool
_elm_widget_can_highlight_get_by_class(Eo *obj)
{
   if (efl_isa(obj, ELM_WIDGET_ITEM_CLASS))
     {
        Elm_Widget_Item_Data *id = efl_data_scope_get(obj, ELM_WIDGET_ITEM_CLASS);
        if (!id) return EINA_FALSE;
        if (!id->can_highlight) return EINA_FALSE;
     }
   else
     {
        Elm_Widget_Smart_Data *wd = efl_data_scope_get(obj, EFL_UI_WIDGET_CLASS);
        if (!wd) return EINA_FALSE;
        if (!wd->can_highlight) return EINA_FALSE;
     }
   return EINA_TRUE;
}
//

//TIZEN_ONLY(20161107): enhance elm_atspi_accessible_can_highlight_set to set can_hihglight property to its children
EAPI Eina_Bool
_elm_widget_item_highlightable(Elm_Object_Item *item)
{
   Eo *parent;

   Elm_Widget_Item_Data *id = efl_data_scope_get(item, ELM_WIDGET_ITEM_CLASS);
   if (!id) return EINA_FALSE;
   if (!id->can_highlight) return EINA_FALSE;

   parent = efl_provider_find(efl_parent_get(item), EFL_ACCESS_OBJECT_MIXIN);
   while (parent && !efl_isa(parent, ELM_ATSPI_APP_OBJECT_CLASS))
     {
        //TIZEN_ONLY(20160929) : atspi: Improves how to find the can_highlight of the widget
        if (!_elm_widget_can_highlight_get_by_class(parent)) return EINA_FALSE;
        //
        parent = efl_provider_find(efl_parent_get(parent), EFL_ACCESS_OBJECT_MIXIN);
     }
   return EINA_TRUE;
}
//

//TIZEN_ONLY(20170206): Add check the object is in the scroller content size
static Eina_Bool
_accessible_object_on_scroll_is(Eo* obj)
{
   /* in case of genlist item, the item->view is NULL if item is unrealized.
      this function is used to check if obj could have HIGHLIGHTABLE or not.
      the unrealized genlist item should have HIGHLIGHTABLE state.
      so if obj is NULL return EINA_TRUE */
   if(!obj) return EINA_TRUE;

   Evas_Object *target = obj;
   Evas_Object *parent = NULL;
   Evas_Coord x, y, w, h, wx, wy, ww = 0, wh = 0, nx = 0, ny = 0;

   evas_object_geometry_get(target, &x, &y ,&w, &h);

   if (elm_widget_is(target))
     parent = elm_widget_parent_get(target);
   else
     parent = elm_widget_parent_widget_get(target);

   while (parent)
     {
        if (efl_isa(parent, ELM_INTERFACE_SCROLLABLE_MIXIN))
          {
             evas_object_geometry_get(parent, &wx, &wy, NULL, NULL);
             elm_interface_scrollable_content_size_get(parent, &ww, &wh);
             elm_interface_scrollable_content_pos_get(parent, &nx, &ny);

             /* widget implements scrollable interface but does not use scoller
                in this case, use widget geometry */
             if (ww == 0 || wh == 0)
               {
                  INF("%s is zero sized scrollable content", efl_class_name_get(efl_class_get(parent)));
                  evas_object_geometry_get(parent, NULL, NULL, &ww, &wh);
               }

             wx -= nx;
             wy -= ny;

             if (((wx < x) && (wx + ww < x)) || ((wx > x + w) && (wx + ww > x + w)) ||
                 ((wy < y) && (wy + wh < y)) || ((wy > y + h) && (wy + wh > y + h)))
               return EINA_FALSE;

             break;
          }
        parent = elm_widget_parent_get(parent);
     }

   return EINA_TRUE;
}
//

//TIZEN_ONLY(20180607): Restore legacy focus
/**
 * @internal
 *
 * Resets the focus_move_policy mode from the system one
 * for widgets that are in automatic mode.
 *
 * @param obj The widget.
 *
 */
static void
_elm_widget_focus_move_policy_reload(Evas_Object *obj)
{
   API_ENTRY return;
   Elm_Focus_Move_Policy focus_move_policy = elm_config_focus_move_policy_get();

   if (efl_ui_widget_focus_move_policy_automatic_get(obj) &&
       (sd->focus_move_policy != focus_move_policy))
     {
        sd->focus_move_policy = focus_move_policy;
     }
}

EOLIAN static void
_efl_ui_widget_focus_reconfigure(Eo *obj, Elm_Widget_Smart_Data *_pd EINA_UNUSED)
{
   const Eina_List *l;
   Evas_Object *child;
   API_ENTRY return;

   EINA_LIST_FOREACH(sd->subobjs, l, child)
     {
        if (elm_widget_is(child))
          efl_ui_widget_focus_reconfigure(child);
     }

   if (sd->hover_obj) efl_ui_widget_focus_reconfigure(sd->hover_obj);

   _elm_widget_focus_move_policy_reload(obj);
}
//

//TIZEN_ONLY(20160329): widget: improve accessible_at_point getter (a8aff0423202b9a55dbb3843205875226678fbd6)
static void
_coordinate_system_based_point_translate(Eo *obj, Eina_Bool screen_coords, int *x, int *y)
{
   Ecore_Evas *ee;
   int ee_x = 0;
   int ee_y = 0;

   if (screen_coords)
     {
        ee = ecore_evas_ecore_evas_get(evas_object_evas_get(obj));
        if (!ee) return;

        ecore_evas_geometry_get(ee, &ee_x, &ee_y, NULL, NULL);
        *x -= ee_x;
        *y -= ee_y;
     }
}

static Evas_Object *
_parent_get(Evas_Object *obj)
{
   Evas_Object *parent;

   parent = evas_object_smart_parent_get(obj);
   if (!parent)
     {
        if (strcmp("Efl_Ui_Win", efl_class_name_get(efl_class_get(obj))))
          parent = elm_widget_parent_get(obj);
     }

   return parent;
}

static Eina_Bool
_is_inside(Evas_Object *obj, int x, int y)
{
   Eina_Bool ret = EINA_TRUE;
   Evas_Coord cx = 0;
   Evas_Coord cy = 0;
   Evas_Coord cw = 0;
   Evas_Coord ch = 0;
   if (efl_isa(obj, ELM_WIDGET_ITEM_CLASS))
     {
        Elm_Widget_Item_Data *id = efl_data_scope_get(obj, ELM_WIDGET_ITEM_CLASS);
        evas_object_geometry_get(id->view, &cx, &cy, &cw, &ch);
     }
   else
     evas_object_geometry_get(obj, &cx, &cy, &cw, &ch);

   /* check the point is out of bound */
   if (x < cx || x > cx + cw || y < cy || y > cy + ch)
     {
        ret = EINA_FALSE;
     }
   return ret;
}

static Eina_Bool
_is_ancestor_of(Evas_Object *smart_parent, Evas_Object *obj)
{
   Eina_Bool ret = EINA_FALSE;
   Evas_Object *parent = elm_widget_parent_get(obj);
   while (parent)
     {
        /* No need to check more, the smart_parent is parent of obj */
        if (smart_parent == parent)
          {
             ret = EINA_TRUE;
             break;
          }
        parent = elm_widget_parent_get(parent);
     }

   return ret;
}

static Eina_Bool
_acceptable_child_is(Eo *obj)
{
   Efl_Access_Role role;
   Eina_List *children;
   Efl_Access_State_Set ss;

   role = efl_access_object_role_get(obj);
   switch (role)
     {
       case EFL_ACCESS_ROLE_IMAGE:
       case EFL_ACCESS_ROLE_ICON:
       case EFL_ACCESS_ROLE_REDUNDANT_OBJECT:
       /* remove unacceptable leaf node */
         children = efl_access_object_access_children_get(obj);
         if (!children) return EINA_FALSE;
         break;

       case EFL_ACCESS_ROLE_PANEL:
         /* remove closed panel fron children list */
         ss = efl_access_object_state_set_get(obj);
         if (!STATE_TYPE_GET(ss, EFL_ACCESS_STATE_TYPE_SHOWING)) return EINA_FALSE;
         break;

       default:
         break;
     }

   return EINA_TRUE;
}

static int _sort_by_size(const void *data1, const void *data2)
{
   Evas_Coord w, h;
   Evas_Coord w2, h2;

   evas_object_geometry_get(data1, NULL, NULL, &w, &h);
   evas_object_geometry_get(data2, NULL, NULL, &w2, &h2);

   if ((w * h) > (w2 * h2)) return 1;
   return -1;
}

Eina_Bool
_elm_widget_atspi_role_acceptable_check(Eo *obj)
{
   Efl_Access_Role role;
   role = efl_access_object_role_get(obj);

   switch (role)
     {
       case EFL_ACCESS_ROLE_APPLICATION:
       case EFL_ACCESS_ROLE_FILLER:
       case EFL_ACCESS_ROLE_SCROLL_PANE:
       case EFL_ACCESS_ROLE_SPLIT_PANE:
       case EFL_ACCESS_ROLE_WINDOW:
       case EFL_ACCESS_ROLE_IMAGE:
       case EFL_ACCESS_ROLE_LIST:
       case EFL_ACCESS_ROLE_ICON:
       case EFL_ACCESS_ROLE_TOOL_BAR:
       case EFL_ACCESS_ROLE_REDUNDANT_OBJECT:
       case EFL_ACCESS_ROLE_COLOR_CHOOSER:
       case EFL_ACCESS_ROLE_TREE_TABLE:
       case EFL_ACCESS_ROLE_PAGE_TAB_LIST:
       case EFL_ACCESS_ROLE_PAGE_TAB:
       case EFL_ACCESS_ROLE_SPIN_BUTTON:
       case EFL_ACCESS_ROLE_INPUT_METHOD_WINDOW:
       case EFL_ACCESS_ROLE_EMBEDDED:
       case EFL_ACCESS_ROLE_INVALID:
       case EFL_ACCESS_ROLE_NOTIFICATION:
         return EINA_FALSE;
       default:
         break;
     }

   return EINA_TRUE;
}

static Eo *
_child_object_at_point_get(const Eo *obj, int x, int y)
{
   Eina_List *l, *l_next, *children, *valid_children = NULL;
   Eo *child;
   Eo *target;
   int count;

   children = efl_access_object_access_children_get(obj);

   EINA_LIST_FOREACH(children, l, child)
     {
        if (_is_inside(child, x, y))
          valid_children = eina_list_append(valid_children, child);
     }

   EINA_LIST_FOREACH_SAFE(valid_children, l, l_next, child)
     {
        children = efl_access_object_access_children_get(child);

        /* do not use unacceptable leaf node */
        if (!_elm_widget_atspi_role_acceptable_check(child) &&
            eina_list_count(children) == 0)
          valid_children = eina_list_remove_list(valid_children, l);
     }

   count = eina_list_count(valid_children);
   if (count > 0)
     {
        valid_children = eina_list_sort(valid_children, -1, _sort_by_size);
        target = eina_list_nth(valid_children, 0);

        return _child_object_at_point_get(target, x, y);
     }

   return obj;
}


static Eo *
_accessible_at_point_top_down_get(const Eo *obj, Elm_Widget_Smart_Data *_pd EINA_UNUSED, Eina_Bool screen_coords, int x, int y)
{
   Eina_List *l, *l2, *children, *valid_children = NULL;
   Eo *child;
   Evas_Object *stack_item;
   Eo *compare_obj;
   // TIZEN_ONLY(20160705) - enable atspi_proxy to work
   Eo *proxy;
   Evas_Coord px, py, pw, ph;
   //

   _coordinate_system_based_point_translate(obj, screen_coords, &x, &y);

   children = efl_access_object_access_children_get(obj);

   EINA_LIST_FOREACH(children, l2, child)
     {
        if (_is_inside(child, x, y) && _acceptable_child_is(child))
          valid_children = eina_list_append(valid_children, child);
     }

   /* If there is only one valid child at point, then return it.
      The evas_tree_objects_at_xy_get could not find proper object,
      if application does not have well aligned objects. */
   if (eina_list_count(valid_children) == 1)
     {
        eina_list_free(children);
        child = eina_list_nth(valid_children, 0);
        return child;
     }

   /* Get evas_object stacked at given x,y coordinates starting from top */
   Eina_List *stack = evas_tree_objects_at_xy_get(evas_object_evas_get(obj), NULL, x, y);
   /* Foreach stacked object starting from top */
   EINA_LIST_FOREACH(stack, l, stack_item)
     {
        /* Foreach at-spi valid children traverse stack_item evas_objects hierarchy */
        EINA_LIST_FOREACH(valid_children, l2, child)
          {
             Efl_Access_Role role;
             role = efl_access_object_role_get(child);
             if (role == EFL_ACCESS_ROLE_REDUNDANT_OBJECT)
               {
                  /* The redundant object ignores */
                  continue;
               }
             /* Compare object used to compare with stacked evas objects */
             compare_obj = child;
             /* In case of widget_items compare should be different then elm_widget_ item  object */
             if (efl_isa(child, ELM_WIDGET_ITEM_CLASS))
               {
                  Elm_Widget_Item_Data *id = efl_data_scope_get(child, ELM_WIDGET_ITEM_CLASS);
                  compare_obj = id->view;
                  if (TIZEN_PROFILE_WEARABLE)
                    {
                       Eo* it_view = evas_object_image_source_get(stack_item);
                       if (it_view && it_view == compare_obj)
                         {
                            eina_list_free(children);
                            eina_list_free(stack);
                            return child;
                         }
                    }
               }
             /* In case of access object compare should be 'wrapped' evas_object */
             if (efl_isa(child, ELM_ACCESS_CLASS))
               {
                   Elm_Access_Info *info = _elm_access_info_get(child);
                   if (!info) continue;
                   compare_obj = info->part_object;
                }
             /* In case of widget is registerd by elm_access_object_register */
             Evas_Object *ao = elm_access_object_get(child);
             if (ao)
               {
                  eina_list_free(children);
                  eina_list_free(stack);
                  return ao;
               }

             /* In case of ewk wrapper object compare with internal ewk_view evas_object */
             if (efl_isa(child, ELM_ATSPI_EWK_WRAPPER_CLASS))
               {
                  compare_obj = elm_atspi_ewk_wrapper_ewk_view_get(child);
               }

             /* If spacial eo children do not have backing evas_object continue with search */
             if (!compare_obj)
               continue;

             Evas_Object *smart_parent = stack_item;
             while (smart_parent)
               {
                   if (smart_parent == compare_obj)
                     {
                        eina_list_free(children);
                        eina_list_free(stack);
                        return child;
                     }

                   // TIZEN_ONLY(20160705) - enable atspi_proxy to work
                   proxy = evas_object_data_get(smart_parent, "__widget_proxy");
                   if (proxy)
                     {
                        // TIZEN_ONLY(20171109) : fix for invalid proxy object, when at-spi has been restarted
                        Eo *parent;
                        parent = efl_provider_find(efl_parent_get(smart_parent), EFL_ACCESS_OBJECT_MIXIN);
                        proxy = plug_type_proxy_get(parent, smart_parent);
                        //
                        evas_object_geometry_get(smart_parent, &px, &py, &pw, &ph);
                        if (x >= px && x <= px + pw && y >= py && y <= py +ph)
                          {
                             eina_list_free(children);
                             eina_list_free(stack);
                             return proxy;
                          }
                     }
                   //

                   smart_parent = _parent_get(smart_parent);
                   if (_is_ancestor_of(smart_parent, obj)) break;
               }
          }
     }

   eina_list_free(children);
   eina_list_free(stack);
   return NULL;
}

static int _sort_by_repeat_events(const void *data1, const void *data2)
{
   Evas_Object *ao1, *ao2;
   Eina_Bool repeat1, repeat2;

   ao1 = elm_access_object_get(data1);
   ao2 = elm_access_object_get(data2);

   repeat1 = evas_object_repeat_events_get(data1);
   repeat2 = evas_object_repeat_events_get(data2);

   if (repeat1 != repeat2)
     {
        if (repeat1 && !ao1) return 1;
     }
   else
     {
        if (repeat1 && !ao1 && ao2) return 1;
     }

   return -1;
}

static Eo *_item_at_point_get(Evas_Object *obj, int x, int y)
{
   Eo *child;
   Eina_List *l, *children;

   children = efl_access_object_access_children_get(obj);

   EINA_LIST_FOREACH(children, l, child)
     {
        if (_is_inside(child, x, y)) return child;
     }

   ERR("No child at point (%d, %d) on object %p", x, y, obj);
   return NULL;
}

//TIZEN_ONLY(20171108): bring HIGHLIGHT related changes
EOLIAN static Eo *
_efl_ui_widget_efl_access_component_accessible_at_point_get(Eo *obj, Elm_Widget_Smart_Data *_pd EINA_UNUSED, Eina_Bool screen_coords, int x, int y)
{
   Elm_Atspi_Role role;
   Eina_List *l;
   Evas_Object *stack_item;

   role = efl_access_object_role_get(obj);

   switch (role)
     {
      case ELM_ATSPI_ROLE_WINDOW:
      case ELM_ATSPI_ROLE_INPUT_METHOD_WINDOW:
      case ELM_ATSPI_ROLE_DIALOG:
      case ELM_ATSPI_ROLE_PAGE_TAB:
      case ELM_ATSPI_ROLE_POPUP_MENU:
      case ELM_ATSPI_ROLE_PANEL:
        DBG("Find accessible from bottom");
        break;

      default:
        return _accessible_at_point_top_down_get(obj, _pd, screen_coords, x, y);
     }

   _coordinate_system_based_point_translate(obj, screen_coords, &x, &y);

   Eina_List *stack = evas_tree_objects_at_xy_get(evas_object_evas_get(obj), NULL, x, y);
   stack = eina_list_sort(stack, -1, _sort_by_repeat_events);

   EINA_LIST_FOREACH(stack, l, stack_item)
     {
        Evas_Object *smart_parent = stack_item;
        while (smart_parent)
          {
             /* If parent equals to obj, it is not necessary to go upper.
                So the top down logic would be better than NULL return. */
             if (smart_parent == obj)
               return _accessible_at_point_top_down_get(obj, _pd, screen_coords, x, y);

             Evas_Object *ao = elm_access_object_get(smart_parent);
             if (ao) return ao;

             if (efl_isa(smart_parent, EFL_ACCESS_OBJECT_MIXIN))
               {
                  Eina_Bool acceptable = EINA_FALSE;
                  Eo *item_child;
                  role = efl_access_object_role_get(smart_parent);
                  switch (role)
                    {
                       case EFL_ACCESS_ROLE_FILLER: /* ex: View of colorselector item is layout */
                       case EFL_ACCESS_ROLE_ICON:
                       case EFL_ACCESS_ROLE_IMAGE:
                       case EFL_ACCESS_ROLE_REDUNDANT_OBJECT:
                       case EFL_ACCESS_ROLE_WINDOW:
                         DBG("Go for parent: %s (%p)\n", evas_object_type_get(smart_parent), smart_parent);
                         break;
                       case EFL_ACCESS_ROLE_LIST:
                           item_child = _item_at_point_get(smart_parent, x, y);
                           if (TIZEN_PROFILE_WEARABLE)
                             {
                                item_child = _child_object_at_point_get(item_child, x, y);
                                return item_child;
                             }
                           return item_child;
                         break;
                       default:
                         acceptable = EINA_TRUE;
                         break;
                    }

                  if (acceptable) return smart_parent;
               }

             smart_parent = _parent_get(smart_parent);
          }
     }

   eina_list_free(stack);
   return _accessible_at_point_top_down_get(obj, _pd, screen_coords, x, y);
}
// TIZEN_ONLY(20171114) Accessibility Highlight Frame added
// EOLIAN static Eina_Bool
// _elm_widget_item_efl_access_component_highlight_grab(Eo *obj, Elm_Widget_Item_Data *sd)
// {
//    Evas_Object *win = elm_widget_top_get(sd->widget);
//    if (win && efl_isa(win, EFL_UI_WIN_CLASS))
//      {
//         _elm_win_accessibility_highlight_set(win, sd->view);
//         efl_access_state_changed_signal_emit(obj, EFL_ACCESS_STATE_TYPE_HIGHLIGHTED, EINA_TRUE);
//         return EINA_TRUE;
//      }
//    return EINA_FALSE;
// }
//
//
// EOLIAN static Eina_Bool
// _elm_widget_item_efl_access_component_highlight_clear(Eo *obj, Elm_Widget_Item_Data *sd)
// {
//    Evas_Object *win = elm_widget_top_get(sd->widget);
//    if (win && efl_isa(win, EFL_UI_WIN_CLASS))
//      {
//         if (_elm_win_accessibility_highlight_get(win) != sd->view)
//           return EINA_TRUE;
//
//         _elm_win_accessibility_highlight_set(win, NULL);
//         efl_access_state_changed_signal_emit(obj, EFL_ACCESS_STATE_TYPE_HIGHLIGHTED, EINA_FALSE);
//         return EINA_TRUE;
//      }
//    return EINA_FALSE;
// }
//

//TIZEN_ONLY(20170119): Show the object highlighted by highlight_grab when the object is completely out of the scroll
void
_elm_widget_showing_geometry_get(Eo *obj, int *x, int *y, int *w, int *h)
{
   Evas_Object *parent;
   Evas_Coord px, py, sx, sy, sw, sh;

   *x = 0;
   *y = 0;
   *w = 0;
   *h = 0;
   if (!obj) return;

   evas_object_geometry_get(obj, x, y, w, h);

   if (elm_widget_is(obj))
     parent = elm_widget_parent_get(obj);
   else
     parent = elm_widget_parent_widget_get(obj);

   while (parent)
     {
        if (efl_isa(parent, ELM_INTERFACE_SCROLLABLE_MIXIN))
          {
             evas_object_geometry_get(parent, &sx, &sy, &sw, &sh);
             px = *x;
             py = *y;
             *x = *x > sx ? *x : sx;
             *y = *y > sy ? *y : sy;
             *w = px + *w < sx + sw ? px + *w - *x : sx + sw - *x;
             *h = py + *h < sy + sh ? py + *h - *y : sy + sh - *y;
          }
        parent = elm_widget_parent_get(parent);
     }
}

Eina_Bool
_accessible_object_on_screen_is(Eo *obj, Evas_Coord x, Evas_Coord y, Evas_Coord w, Evas_Coord h, Eina_Bool is_complete)
{
   if(!obj) return EINA_FALSE;

   Evas_Object *target = obj;
   Evas_Object *parent = NULL;
   Evas_Coord px, py, sx, sy, sw, sh, ox, oy, ow, oh;

   /* uninitialized data could be read */
   ow = 0;
   oh = 0;

   if (elm_widget_is(target))
     parent = elm_widget_parent_get(target);
   else
     parent = elm_widget_parent_widget_get(target);

   while (parent)
     {
        if (efl_isa(parent, ELM_INTERFACE_SCROLLABLE_MIXIN))
          {
             evas_object_geometry_get(parent, &sx, &sy, &sw, &sh);
             px = ox = x;
             py = oy = y;
             ow = w;
             oh = h;
             ox = is_complete ? ox : ox + ow;
             oy = is_complete ? oy : oy + oh;
             ox = ox > sx ? ox : sx;
             oy = oy > sy ? oy : sy;
             ow = px + ow < sx + sw ? px + ow - ox : sx + sw - ox;
             oh = py + oh < sy + sh ? py + oh - oy : sy + sh - oy;
          }
        if (ow <= 0 || oh <= 0)
          return EINA_FALSE;

        parent = elm_widget_parent_get(parent);
     }
   return EINA_TRUE;
}

Eina_List *
_accessible_scrollable_parent_list_get(const Eo *obj)
{
   if(!obj) return NULL;

   Evas_Object *parent = NULL;
   Eina_List *plist = NULL;

   if (elm_widget_is(obj))
     parent = elm_widget_parent_get(obj);
   else
     parent = elm_widget_parent_widget_get(obj);

   while (parent)
     {
        if (_elm_scrollable_is(parent))
          plist = eina_list_append(plist, parent);

        parent = elm_widget_parent_get(parent);
   }
   return plist;
}

void
_accessible_highlight_region_show(Eo* obj)
{
   if (!obj) return ;

   Evas_Object *target = obj;
   Evas_Object *parent = NULL;
   Evas_Object *parent_sub = NULL;
   Eina_List *plist, *plist_sub;
   Eina_List *l, *l2;

   Evas_Coord target_x, target_y, target_w, target_h;

   evas_object_geometry_get(target, &target_x, &target_y, &target_w, &target_h);

   plist = _accessible_scrollable_parent_list_get(target);
   if (!plist) return ;
   EINA_LIST_FOREACH(plist, l, parent)
     {
        if(!_accessible_object_on_screen_is(target, target_x, target_y, target_w, target_h, EINA_TRUE))
          {
             plist_sub = _accessible_scrollable_parent_list_get(parent);
             plist_sub = eina_list_prepend(plist_sub, parent);
             EINA_LIST_FOREACH(plist_sub, l2, parent_sub)
               {
                  Evas_Coord scroll_x = 0, scroll_y = 0;
                  Evas_Coord scroll_x_back = 0, scroll_y_back = 0;
                  Evas_Coord x, y, w, h;
                  Evas_Coord px, py;

                  elm_interface_scrollable_content_region_get(parent_sub, &scroll_x_back, &scroll_y_back, NULL, NULL);
                  evas_object_geometry_get(parent_sub, &px, &py, NULL, NULL);
                  x = target_x; y = target_y; w = target_w; h = target_h;

                  x -= (px - scroll_x_back);
                  y -= (py - scroll_y_back);
                  switch (_elm_config->focus_autoscroll_mode)
                    {
                       case ELM_FOCUS_AUTOSCROLL_MODE_SHOW:
                          elm_interface_scrollable_content_region_show(parent_sub, x, y, w, h);
                          break;
                       case ELM_FOCUS_AUTOSCROLL_MODE_BRING_IN:
                          elm_interface_scrollable_region_bring_in(parent_sub, x, y, w, h);
                          break;
                       default:
                          break;
                    }
                  elm_interface_scrollable_content_region_get(parent_sub, &scroll_x, &scroll_y, NULL, NULL);

                  target_x -= (scroll_x - scroll_x_back);
                  target_y -= (scroll_y - scroll_y_back);

                  if(_accessible_object_on_screen_is(target, target_x, target_y, target_w, target_h, EINA_FALSE))
                    break;
               }
             eina_list_free(plist_sub);
          }
     }

   eina_list_free(plist);
}
//

//TIZEN_ONLY(20171011) : atspi : During the highlight grab, out signal is not sent.
static Eo *_highlight_grabbing_object = NULL;

Eina_Bool
_elm_widget_accessibility_highlight_grabbing_get(Eo *obj)
{
  return _highlight_grabbing_object == obj;
}

void
_elm_widget_accessibility_highlight_grabbing_set(Eo *obj, Eina_Bool grabbing)
{
   if (grabbing)
     {
       if (!_highlight_grabbing_object)
           _highlight_grabbing_object = obj;
       else if (_highlight_grabbing_object == obj)
           ERR("trying to set grabbing for %p, but it's already set to this object", obj);
       else
           ERR("trying to set grabbing for %p, but it's already set to %p", obj, _highlight_grabbing_object);
     }
   else
     {
       if (_highlight_grabbing_object != obj)
           ERR("trying to clear grabbing for %p, but it's set to %p", obj, _highlight_grabbing_object);
       else
           _highlight_grabbing_object = NULL;
     }
}
//

EOLIAN static Eina_Bool
_efl_ui_widget_efl_access_component_highlight_grab(Eo *obj, Elm_Widget_Smart_Data *pd EINA_UNUSED)
{
   if(!obj) return EINA_FALSE;
   if(!_elm_atspi_enabled())
      return EINA_FALSE;

   // TIZEN_ONLY(20171020) : atspi : Do not send signal, if current object and highlight object are same
   if (_elm_object_accessibility_currently_highlighted_get() == obj)
      return EINA_FALSE;
   //

   //TIZEN_ONLY(20171011) : atspi : During the highlight grab, out signal is not sent.
   _elm_widget_accessibility_highlight_grabbing_set(obj, EINA_TRUE);
   //

   //TIZEN_ONLY(20170119): Show the object highlighted by highlight_grab when the object is completely out of the scroll
   _accessible_highlight_region_show(obj);
   //

   elm_object_accessibility_highlight_set(obj, EINA_TRUE);
   efl_access_state_changed_signal_emit(obj, EFL_ACCESS_STATE_TYPE_HIGHLIGHTED, EINA_TRUE);

   // TIZEN_ONLY(20161018): add highlighted/unhighlighted signal for atspi
   evas_object_smart_callback_call(obj, SIG_WIDGET_ATSPI_HIGHLIGHTED, NULL);
   //

   //TIZEN_ONLY(20171011) : atspi : During the highlight grab, out signal is not sent.
   _elm_widget_accessibility_highlight_grabbing_set(obj, EINA_FALSE);
   //
   return EINA_TRUE;
}

EOLIAN static Eina_Bool
_efl_ui_widget_efl_access_component_highlight_clear(Eo *obj, Elm_Widget_Smart_Data *pd EINA_UNUSED)
{
   if (!obj) return EINA_FALSE;
   if (!_elm_atspi_enabled())
      return EINA_FALSE;

   elm_object_accessibility_highlight_set(obj, EINA_FALSE);
   efl_access_state_changed_signal_emit(obj, EFL_ACCESS_STATE_TYPE_HIGHLIGHTED, EINA_FALSE);
   // TIZEN_ONLY(20161018): add highlighted/unhighlighted signal for atspi
   evas_object_smart_callback_call(obj, SIG_WIDGET_ATSPI_UNHIGHLIGHTED, NULL);
   //
   return EINA_TRUE;
}
//

// TIZEN_ONLY(20171114) Accessibility Highlight Frame added
EOLIAN static Eina_Bool
_elm_widget_item_efl_access_component_highlight_grab(Eo *obj, Elm_Widget_Item_Data *sd)
{
   // TIZEN_ONLY(20171117) Accessibility Highlight frame support for items
   // Evas_Object *win = elm_widget_top_get(sd->widget);
   // if (win && efl_isa(win, EFL_UI_WIN_CLASS))
   //   {
   //      elm_object_accessibility_highlight_set(sd->view, EINA_TRUE);
   //      efl_access_state_changed_signal_emit(obj, EFL_ACCESS_STATE_TYPE_HIGHLIGHTED, EINA_TRUE);
   //      return EINA_TRUE;
   //   }
   // return EINA_FALSE;

   if (!sd) return EINA_FALSE;
   if (!sd->view) return EINA_FALSE;
   if (!_elm_atspi_enabled())
      return EINA_FALSE;

   // TIZEN_ONLY(20171020) : atspi : Do not send signal, if current object and highlight object are same
   if (_elm_object_accessibility_currently_highlighted_get() == obj)
      return EINA_FALSE;
   //

   //TIZEN_ONLY(20171011) : atspi : During the highlight grab, out signal is not sent.
   _elm_widget_accessibility_highlight_grabbing_set(obj, EINA_TRUE);
   //

   //TIZEN_ONLY(20170119): Show the object highlighted by highlight_grab when the object is completely out of the scroll
   _accessible_highlight_region_show(sd->view);
   //

   if (!sd->eo_obj) return EINA_FALSE;
   elm_object_accessibility_highlight_set(sd->eo_obj, EINA_TRUE);

   if (!obj) return EINA_FALSE;
   efl_access_state_changed_signal_emit(obj, EFL_ACCESS_STATE_TYPE_HIGHLIGHTED, EINA_TRUE);
   //TIZEN_ONLY(20170412) Make atspi,(un)highlighted work on widget item
   evas_object_smart_callback_call(sd->widget, SIG_WIDGET_ATSPI_HIGHLIGHTED, obj);
   //

   //TIZEN_ONLY(20171011) : atspi : During the highlight grab, out signal is not sent.
    _elm_widget_accessibility_highlight_grabbing_set(obj, EINA_FALSE);
    //

   return EINA_TRUE;
   //
}

EOLIAN static Eina_Bool
_elm_widget_item_efl_access_component_highlight_clear(Eo *obj, Elm_Widget_Item_Data *sd)
{
   if (!obj) return EINA_FALSE;
   if (!_elm_atspi_enabled())
     return EINA_FALSE;

   elm_object_accessibility_highlight_set(sd->eo_obj, EINA_FALSE);
   efl_access_state_changed_signal_emit(obj, EFL_ACCESS_STATE_TYPE_HIGHLIGHTED, EINA_FALSE);
   //TIZEN_ONLY(20170412) Make atspi,(un)highlighted work on widget item
   evas_object_smart_callback_call(sd->widget, SIG_WIDGET_ATSPI_UNHIGHLIGHTED, obj);
   //
   return EINA_TRUE;
}
//

//TIZEN_ONLY(20160527): widget: add AtspiAction interface to all widgets and widget_items, add handlers for reading stopped/cancelled
EOLIAN const Efl_Access_Action_Data *
_efl_ui_widget_efl_access_widget_action_elm_actions_get(const Eo *obj EINA_UNUSED, Elm_Widget_Smart_Data *pd EINA_UNUSED)
{
   return NULL;
}

EOLIAN const Efl_Access_Action_Data *
_elm_widget_item_efl_access_widget_action_elm_actions_get(const Eo *obj EINA_UNUSED, Elm_Widget_Item_Data *pd EINA_UNUSED)
{
   return NULL;
}

/***********************************************************
 * TIZEN_ONLY(20180117): Override Paragraph Direction APIs *
 ***********************************************************/
static void
_efl_ui_widget_efl_canvas_object_paragraph_direction_set_internal(Eo *obj EINA_UNUSED, Efl_Ui_Widget_Data *sd, Efl_Text_Bidirectional_Type dir)
{
   Evas_Object *child;
   Eina_List *l;

   if (sd->on_destroy) return;

   EINA_LIST_FOREACH(sd->subobjs, l, child)
     {
        if (_elm_widget_is(child))
          {
             Efl_Ui_Widget_Data *sdc = efl_data_scope_get(child, MY_CLASS);

             if (sdc->inherit_paragraph_direction &&
                 (sdc->paragraph_direction != dir))
               {
                  sdc->paragraph_direction = dir;
                  _efl_ui_widget_efl_canvas_object_paragraph_direction_set_internal(child, sdc, dir);
                  efl_canvas_object_paragraph_direction_set(efl_super(child, MY_CLASS), dir);
               }
          }

        /* FIXME: There is no way to handle non-widget child object.
         * If a non-widget child object has smart parent, it will get the direction
         * from the smart parent. */
     }
}

EOLIAN static void
_efl_ui_widget_efl_canvas_object_paragraph_direction_set(Eo *obj, Efl_Ui_Widget_Data *sd, Efl_Text_Bidirectional_Type dir)
{
   if ((!(sd->inherit_paragraph_direction) && (sd->paragraph_direction == dir)) ||
       (sd->inherit_paragraph_direction && (dir == EFL_TEXT_BIDIRECTIONAL_TYPE_INHERIT)))
     return;

   if (dir == EFL_TEXT_BIDIRECTIONAL_TYPE_INHERIT)
     {
        sd->inherit_paragraph_direction = EINA_TRUE;
        Evas_BiDi_Direction parent_dir = EFL_TEXT_BIDIRECTIONAL_TYPE_NEUTRAL;

        if (sd->parent_obj)
          parent_dir = efl_canvas_object_paragraph_direction_get(sd->parent_obj);

        if (parent_dir != sd->paragraph_direction)
          {
             sd->paragraph_direction = parent_dir;
             _efl_ui_widget_efl_canvas_object_paragraph_direction_set_internal(obj, sd, parent_dir);
          }
     }
   else
     {
        sd->inherit_paragraph_direction = EINA_FALSE;
        sd->paragraph_direction = dir;
        _efl_ui_widget_efl_canvas_object_paragraph_direction_set_internal(obj, sd, dir);
     }

   efl_canvas_object_paragraph_direction_set(efl_super(obj, MY_CLASS), dir);
}
/*******
 * END *
 *******/

/***********************************************************************************
 * TIZEN_ONLY_FEATURE: apply Tizen's color_class features.                         *
 ***********************************************************************************/
void
_elm_widget_color_class_parent_set(Evas_Object *obj, Evas_Object *parent)
{
   Evas_Object *edje = NULL, *parent_edje = NULL;

   if (!obj || !parent) return;

   if (efl_isa(obj, EFL_UI_LAYOUT_BASE_CLASS))
     edje =  elm_layout_edje_get(obj);
   else if (efl_isa(obj, EFL_CANVAS_LAYOUT_CLASS))
     edje = obj;

   if (efl_isa(parent, EFL_UI_LAYOUT_BASE_CLASS))
     parent_edje =  elm_layout_edje_get(parent);
   else if (efl_isa(parent, EFL_CANVAS_LAYOUT_CLASS))
     parent_edje = parent;

   if (!edje || !parent_edje) return;

   edje_object_color_class_parent_set(edje, parent_edje);
}

void
_elm_widget_color_class_parent_unset(Evas_Object *obj)
{
   Evas_Object *edje = NULL;

   if (!obj) return;

   if (efl_isa(obj, EFL_UI_LAYOUT_BASE_CLASS))
     edje =  elm_layout_edje_get(obj);
   else if (efl_isa(obj, EFL_CANVAS_LAYOUT_CLASS))
     edje = obj;

   if (!edje) return;

   edje_object_color_class_parent_unset(edje);
}

void
_edje_color_class_free(void *data)
{
   Edje_Color_Class *cc = data;

   if (cc->name) eina_stringshare_del(cc->name);
   free(cc);
}

Eina_Stringshare *
_elm_widget_edje_class_get(Evas_Object *obj, const char *style, const char *part)
{
   Eina_Strbuf *buf;
   Eina_Stringshare *str;
   const char *klass_name = NULL;
   Eina_Bool is_legacy = EINA_FALSE;
   Eina_Bool is_item = efl_isa(obj, ELM_WIDGET_ITEM_CLASS);

   if (is_item)
     is_legacy = elm_widget_is_legacy(elm_object_item_widget_get(obj));
   else
     is_legacy = elm_widget_is_legacy(obj);

   buf = eina_strbuf_new();

   if (is_legacy)
     {
        if (is_item)
          klass_name = _elm_widget_item_legacy_type_get(obj);
        else
          klass_name = elm_widget_type_get(obj);
     }
   else
     {
        klass_name = efl_class_name_get(efl_class_get(obj));
     }

   if (klass_name)
     {
        if (is_legacy && strchr(klass_name, '_'))
          {
             eina_strbuf_append(buf, strchr(klass_name, '_') + 1);
             eina_strbuf_tolower(buf);
          }
        else
          {
             /* Get the last word from the given klass name */
             char *temp, *temp_orig, *temp_ret, *last_ret = NULL;
             const char *delim = NULL;

             temp_orig = temp = strdup(klass_name);

             /* If "." is not used for word serperator,
              * it assume the given klass name is legacy.
              * And legacy klass name was made with "_" as its word seperator. */
             if (strchr(klass_name, '.'))
               delim = ".";
             else
               delim = "_";

             while ((temp_ret = strsep(&temp, delim)) != NULL)
               {
                  if (strcmp(temp_ret, ""))
                    last_ret = temp_ret;
               }

             if (last_ret)
               {
                  eina_strbuf_append(buf, last_ret);
                  eina_strbuf_tolower(buf);
               }

             free(temp_orig);
          }
     }

   if (style)
     {
        eina_strbuf_append_printf(buf, "/%s/%s", style, part);
     }
   else
     {
        eina_strbuf_append_printf(buf, "/%s", part);
     }

   str = eina_stringshare_add(eina_strbuf_string_get(buf));

   eina_strbuf_free(buf);
   return str;
}

Eina_Bool
_elm_widget_color_class_set_internal(Evas_Object *obj, Evas_Object *edje, const char *color_class,
                                     int r, int g, int b, int a,
                                     int r2, int g2, int b2, int a2,
                                     int r3, int g3, int b3, int a3)
{
   Eina_Bool int_ret = EINA_TRUE;
   Eina_Stringshare *buf;
   int temp_color[3][4] = { { r,  g,  b,  a },
                            { r2, g2, b2, a2 },
                            { r3, g3, b3, a3 } };

   if (!color_class) return EINA_FALSE;

   buf = _elm_widget_edje_class_get(obj, NULL, color_class);

#define TEMP_COLOR(x, y) \
   ((temp_color[x][y] == -1) ? &temp_color[x][y] : NULL)

   edje_object_color_class_get(edje, buf,
                               TEMP_COLOR(0, 0), TEMP_COLOR(0, 1), TEMP_COLOR(0, 2), TEMP_COLOR(0, 3),
                               TEMP_COLOR(1, 0), TEMP_COLOR(1, 1), TEMP_COLOR(1, 2), TEMP_COLOR(1, 3),
                               TEMP_COLOR(2, 0), TEMP_COLOR(2, 1), TEMP_COLOR(2, 2), TEMP_COLOR(2, 3));

#undef TEMP_COLOR

#define TEMP_COLOR(x, y) \
   ((temp_color[x][y] == -1) ? 0 : temp_color[x][y])

   int_ret &= edje_object_color_class_set(edje, buf,
                                          TEMP_COLOR(0, 0), TEMP_COLOR(0, 1), TEMP_COLOR(0, 2), TEMP_COLOR(0, 3),
                                          TEMP_COLOR(1, 0), TEMP_COLOR(1, 1), TEMP_COLOR(1, 2), TEMP_COLOR(1, 3),
                                          TEMP_COLOR(2, 0), TEMP_COLOR(2, 1), TEMP_COLOR(2, 2), TEMP_COLOR(2, 3));

#undef TEMP_COLOR

   eina_stringshare_del(buf);

   return int_ret;
}

Eina_Bool
_elm_widget_color_class_get_internal(Evas_Object *obj, Evas_Object *edje, const char *color_class,
                                     int *r, int *g, int *b, int *a,
                                     int *r2, int *g2, int *b2, int *a2,
                                     int *r3, int *g3, int *b3, int *a3)
{
   Eina_Bool int_ret = EINA_TRUE;
   Eina_Stringshare *buf;

   if (!color_class) return EINA_FALSE;

   buf = _elm_widget_edje_class_get(obj, elm_widget_style_get(obj), color_class);

   int_ret &= edje_object_color_class_get(edje, buf,
                                          r,  g,  b,  a,
                                          r2, g2, b2, a2,
                                          r3, g3, b3, a3);

   eina_stringshare_del(buf);

   return int_ret;
}

/* Internal EO API */
static Eina_Bool
_elm_widget_class_color_set(Eo *obj, Elm_Widget_Smart_Data *sd EINA_UNUSED, const char *color_class, int r, int g, int b, int a)
{
   Evas_Object *edje;
   Eina_Bool int_ret = EINA_TRUE;

   if (!efl_isa(obj, EFL_UI_LAYOUT_BASE_CLASS)) return EINA_FALSE;

   edje = elm_layout_edje_get(obj);
   int_ret &= _elm_widget_color_class_set_internal(obj, edje, color_class,
                                                   r, g, b, a,
                                                   -1, -1, -1, -1,
                                                   -1, -1, -1, -1);

   return int_ret;
}

/* Internal EO API */
static Eina_Bool
_elm_widget_class_color_get(Eo *obj, Elm_Widget_Smart_Data *sd EINA_UNUSED, const char *color_class, int *r, int *g, int *b, int *a)
{
   Evas_Object *edje;
   Eina_Bool int_ret = EINA_TRUE;

   if (!efl_isa(obj, EFL_UI_LAYOUT_BASE_CLASS)) return EINA_FALSE;

   edje = elm_layout_edje_get(obj);
   int_ret &= _elm_widget_color_class_get_internal(obj, edje, color_class,
                                                   r, g, b, a,
                                                   NULL, NULL, NULL, NULL,
                                                   NULL, NULL, NULL, NULL);

   return int_ret;
}

/* Internal EO API */
static Eina_Bool
_elm_widget_class_color2_set(Eo *obj, Elm_Widget_Smart_Data *sd EINA_UNUSED, const char *color_class, int r, int g, int b, int a)
{
   Evas_Object *edje;
   Eina_Bool int_ret = EINA_TRUE;

   if (!efl_isa(obj, EFL_UI_LAYOUT_BASE_CLASS)) return EINA_FALSE;

   edje = elm_layout_edje_get(obj);
   int_ret &= _elm_widget_color_class_set_internal(obj, edje, color_class,
                                                   -1, -1, -1, -1,
                                                   r, g, b, a,
                                                   -1, -1, -1, -1);

   return int_ret;
}

/* Internal EO API */
static Eina_Bool
_elm_widget_class_color2_get(Eo *obj, Elm_Widget_Smart_Data *sd EINA_UNUSED, const char *color_class, int *r, int *g, int *b, int *a)
{
   Evas_Object *edje;
   Eina_Bool int_ret = EINA_TRUE;

   if (!efl_isa(obj, EFL_UI_LAYOUT_BASE_CLASS)) return EINA_FALSE;

   edje = elm_layout_edje_get(obj);
   int_ret &= _elm_widget_color_class_get_internal(obj, edje, color_class,
                                                   NULL, NULL, NULL, NULL,
                                                   r, g, b, a,
                                                   NULL, NULL, NULL, NULL);

   return int_ret;
}

/* Internal EO API */
static Eina_Bool
_elm_widget_class_color3_set(Eo *obj, Elm_Widget_Smart_Data *sd EINA_UNUSED, const char *color_class, int r, int g, int b, int a)
{
   Evas_Object *edje;
   Eina_Bool int_ret = EINA_TRUE;

   if (!efl_isa(obj, EFL_UI_LAYOUT_BASE_CLASS)) return EINA_FALSE;

   edje = elm_layout_edje_get(obj);
   int_ret &= _elm_widget_color_class_set_internal(obj, edje, color_class,
                                                   -1, -1, -1, -1,
                                                   -1, -1, -1, -1,
                                                   r, g, b, a);

   return int_ret;
}

/* Internal EO API */
static Eina_Bool
_elm_widget_class_color3_get(Eo *obj, Elm_Widget_Smart_Data *sd EINA_UNUSED, const char *color_class, int *r, int *g, int *b, int *a)
{
   Evas_Object *edje;
   Eina_Bool int_ret = EINA_TRUE;

   if (!efl_isa(obj, EFL_UI_LAYOUT_BASE_CLASS)) return EINA_FALSE;

   edje = elm_layout_edje_get(obj);
   int_ret &= _elm_widget_color_class_get_internal(obj, edje, color_class,
                                                   NULL, NULL, NULL, NULL,
                                                   NULL, NULL, NULL, NULL,
                                                   r, g, b, a);

   return int_ret;
}

/* Internal EO API */
static void
_elm_widget_class_color_del(Eo *obj EINA_UNUSED, Elm_Widget_Smart_Data *sd, const char *color_class)
{
   Eina_Stringshare *buf;

   if (!efl_isa(obj, EFL_UI_LAYOUT_BASE_CLASS)) return;

   buf = _elm_widget_edje_class_get(obj, NULL, color_class);
   edje_object_color_class_del(sd->resize_obj, buf);
   eina_stringshare_del(buf);
}

/* Internal EO API */
static void
_elm_widget_class_color_clear(Eo *obj EINA_UNUSED, Elm_Widget_Smart_Data *sd)
{
   if (!efl_isa(obj, EFL_UI_LAYOUT_BASE_CLASS)) return;
   edje_object_color_class_clear(sd->resize_obj);
}

#define ELM_COLOR_CLASS_UPDATE(obj, hash, cond)                                  \
   Evas_Object *edje = NULL;                                                     \
   Eina_Iterator *itr;                                                           \
   Edje_Color_Class *cc;                                                         \
   Eina_Bool int_ret = EINA_TRUE;                                                \
   if (cond) return EINA_FALSE;                                                  \
   if (efl_isa(obj, EFL_UI_LAYOUT_BASE_CLASS))                                            \
     edje =  elm_layout_edje_get(obj);                                           \
   else if (efl_isa(obj, EFL_CANVAS_LAYOUT_CLASS))                                      \
     edje = obj;                                                                 \
   if (!edje) return EINA_FALSE;                                                 \
   itr = eina_hash_iterator_data_new(hash);                                      \
   EINA_ITERATOR_FOREACH(itr, cc)                                                \
     {                                                                           \
        int_ret &= edje_object_color_class_set(edje, cc->name,                   \
                                               cc->r, cc->g, cc->b, cc->a,       \
                                               cc->r2, cc->g2, cc->b2, cc->a2,   \
                                               cc->r3, cc->g3, cc->b3, cc->a3);  \
     }                                                                           \
   eina_iterator_free(itr);                                                      \
   return int_ret


#define CHECK_BOUND(x)                                                           \
   if (x > 0xff) x = 0xff;                                                       \
   else if (x < 0) x = 0

#define ELM_COLOR_CLASS_SET_START(obj, cr, cg, cb, ca)                           \
   Eina_Bool int_ret = EINA_FALSE;                                               \
   Edje_Color_Class *cc = NULL;                                                  \
   Eina_Stringshare *buf;                                                        \
   buf = _elm_widget_edje_class_get(obj, NULL, color_class);       \
   CHECK_BOUND(r);                                                               \
   CHECK_BOUND(g);                                                               \
   CHECK_BOUND(b);                                                               \
   CHECK_BOUND(a);                                                               \
   _elm_color_unpremul(a, &r, &g, &b);                                           \
   if (!sd->color_classes)                                                       \
     sd->color_classes = eina_hash_string_small_new(_edje_color_class_free);     \
   else                                                                          \
     cc = eina_hash_find(sd->color_classes, buf);                                \
   if (!cc)                                                                      \
     {                                                                           \
        cc = calloc(1, sizeof(Edje_Color_Class));                                \
        cc->name = eina_stringshare_add(buf);                                    \
        if (!cc->name)                                                           \
          {                                                                      \
             free(cc);                                                           \
             eina_stringshare_del(buf);                                          \
             return EINA_FALSE;                                                  \
          }                                                                      \
        eina_hash_direct_add(sd->color_classes, cc->name, cc);                   \
     }                                                                           \
   else if ((cc->cr == r) && (cc->cg == g) &&                                    \
            (cc->cb == b) && (cc->ca == a))                                      \
     {                                                                           \
        eina_stringshare_del(buf);                                               \
        return EINA_TRUE;                                                        \
     }                                                                           \
   cc->cr = r;                                                                   \
   cc->cg = g;                                                                   \
   cc->cb = b;                                                                   \
   cc->ca = a;                                                                   \
   int_ret = EINA_TRUE

#define ELM_COLOR_CLASS_SET_END()                                                \
   eina_stringshare_del(buf);                                                    \
   return int_ret

#define ELM_COLOR_CLASS_GET(obj, cr, cg, cb, ca)                                 \
   Eina_Bool int_ret = EINA_FALSE;                                               \
   Edje_Color_Class *cc;                                                         \
   Eina_Stringshare *buf;                                                        \
   int alpha = 0;                                                                \
   buf = _elm_widget_edje_class_get(obj, NULL, color_class);       \
   if ((!sd->color_classes) || !(cc = eina_hash_find(sd->color_classes, buf)))   \
     {                                                                           \
        if (r) *r = 0;                                                           \
        if (g) *g = 0;                                                           \
        if (b) *b = 0;                                                           \
        if (a) *a = 0;                                                           \
        int_ret = EINA_FALSE;                                                    \
     }                                                                           \
   else                                                                          \
     {                                                                           \
        if (r) *r = cc->cr;                                                      \
        if (g) *g = cc->cg;                                                      \
        if (b) *b = cc->cb;                                                      \
        if (a) *a = cc->ca;                                                      \
        alpha = cc->ca;                                                          \
        int_ret = EINA_TRUE;                                                     \
     }                                                                           \
   _elm_color_premul(alpha, r, g, b);                                            \
   eina_stringshare_del(buf);                                                    \
   return int_ret

Eina_Bool
_elm_widget_item_color_class_update(Elm_Widget_Item_Data *sd)
{
   ELM_COLOR_CLASS_UPDATE(sd->view, sd->color_classes, (!sd) || (!sd->color_classes) || (!sd->view));
}

/* Internal EO API */
static Eina_Bool
_elm_widget_item_class_color_set(Eo *obj, Elm_Widget_Item_Data *sd, const char *color_class, int r, int g, int b, int a)
{
   ELM_COLOR_CLASS_SET_START(obj, r, g, b, a);

   int_ret &= _elm_widget_item_color_class_update(sd);

   ELM_COLOR_CLASS_SET_END();
}

/* Internal EO API */
static Eina_Bool
_elm_widget_item_class_color_get(const Eo *obj, Elm_Widget_Item_Data *sd, const char *color_class, int *r, int *g, int *b, int *a)
{
   ELM_COLOR_CLASS_GET(obj, r, g, b, a);
}

/* Internal EO API */
static Eina_Bool
_elm_widget_item_class_color2_set(Eo *obj, Elm_Widget_Item_Data *sd, const char *color_class, int r, int g, int b, int a)
{
   ELM_COLOR_CLASS_SET_START(obj, r2, g2, b2, a2);

   int_ret &= _elm_widget_item_color_class_update(sd);

   ELM_COLOR_CLASS_SET_END();
}

/* Internal EO API */
static Eina_Bool
_elm_widget_item_class_color2_get(const Eo *obj, Elm_Widget_Item_Data *sd, const char *color_class, int *r, int *g, int *b, int *a)
{
   ELM_COLOR_CLASS_GET(obj, r2, g2, b2, a2);
}

/* Internal EO API */
static Eina_Bool
_elm_widget_item_class_color3_set(Eo *obj, Elm_Widget_Item_Data *sd, const char *color_class, int r, int g, int b, int a)
{
   ELM_COLOR_CLASS_SET_START(obj, r3, g3, b3, a3);

   int_ret &= _elm_widget_item_color_class_update(sd);

   ELM_COLOR_CLASS_SET_END();
}

/* Internal EO API */
static Eina_Bool
_elm_widget_item_class_color3_get(const Eo *obj, Elm_Widget_Item_Data *sd, const char *color_class, int *r, int *g, int *b, int *a)
{
   ELM_COLOR_CLASS_GET(obj, r3, g3, b3, a3);
}

/* Internal EO API */
static Evas_Object *
_elm_widget_item_edje_get(const Eo *obj, Elm_Widget_Item_Data *sd)
{
   if (!sd)
     sd = efl_data_scope_get(obj, ELM_WIDGET_ITEM_CLASS);

   if (efl_isa(sd->view, EFL_UI_LAYOUT_BASE_CLASS))
     return elm_layout_edje_get(sd->view);
   else if (efl_isa(sd->view, EFL_CANVAS_LAYOUT_CLASS))
     return sd->view;

   return NULL;
}

/* Internal EO API */
static void
_elm_widget_item_class_color_del(Eo *obj EINA_UNUSED, Elm_Widget_Item_Data *sd, const char *color_class)
{
   Eina_Stringshare *buf;
   Evas_Object *edje;
   Edje_Color_Class *cc = NULL;

   if (!color_class) return;

   buf = _elm_widget_edje_class_get(obj, NULL, color_class);
   eina_hash_del(sd->color_classes, buf, cc);

   edje = _elm_widget_item_edje_get(obj, sd);
   if (edje)
     edje_object_color_class_del(edje, buf);

   eina_stringshare_del(buf);
}

/* Internal EO API */
static void
_elm_widget_item_class_color_clear(Eo *obj EINA_UNUSED, Elm_Widget_Item_Data *sd)
{
   Evas_Object *edje;
   ELM_SAFE_FREE(sd->color_classes, eina_hash_free);

   edje = _elm_widget_item_edje_get(obj, sd);
   if (!edje) return;

   edje_object_color_class_clear(edje);
}

/* Internal EO APIs and hidden overrides */
EAPI EFL_FUNC_BODYV(elm_widget_item_class_color_set, Eina_Bool, EINA_FALSE,
                    EFL_FUNC_CALL(color_class, r, g, b, a),
                    const char *color_class, int r, int g, int b, int a)
EAPI EFL_FUNC_BODYV(elm_widget_item_class_color_get, Eina_Bool, EINA_FALSE,
                    EFL_FUNC_CALL(color_class, r, g, b, a),
                    const char *color_class, int *r, int *g, int *b, int *a)
EAPI EFL_FUNC_BODYV(elm_widget_item_class_color2_set, Eina_Bool, EINA_FALSE,
                    EFL_FUNC_CALL(color_class, r, g, b, a),
                    const char *color_class, int r, int g, int b, int a)
EAPI EFL_FUNC_BODYV(elm_widget_item_class_color2_get, Eina_Bool, EINA_FALSE,
                    EFL_FUNC_CALL(color_class, r, g, b, a),
                    const char *color_class, int *r, int *g, int *b, int *a)
EAPI EFL_FUNC_BODYV(elm_widget_item_class_color3_set, Eina_Bool, EINA_FALSE,
                    EFL_FUNC_CALL(color_class, r, g, b, a),
                    const char *color_class, int r, int g, int b, int a)
EAPI EFL_FUNC_BODYV(elm_widget_item_class_color3_get, Eina_Bool, EINA_FALSE,
                    EFL_FUNC_CALL(color_class, r, g, b, a),
                    const char *color_class, int *r, int *g, int *b, int *a)
EAPI EFL_VOID_FUNC_BODYV(elm_widget_item_class_color_del,
                         EFL_FUNC_CALL(color_class),
                         const char *color_class)
EAPI EFL_VOID_FUNC_BODY(elm_widget_item_class_color_clear)

#define ELM_WIDGET_ITEM_EXTRA_OPS \
   EFL_OBJECT_OP_FUNC(elm_widget_item_class_color_set, _elm_widget_item_class_color_set), \
   EFL_OBJECT_OP_FUNC(elm_widget_item_class_color_get, _elm_widget_item_class_color_get), \
   EFL_OBJECT_OP_FUNC(elm_widget_item_class_color2_set, _elm_widget_item_class_color2_set), \
   EFL_OBJECT_OP_FUNC(elm_widget_item_class_color2_get, _elm_widget_item_class_color2_get), \
   EFL_OBJECT_OP_FUNC(elm_widget_item_class_color3_set, _elm_widget_item_class_color3_set), \
   EFL_OBJECT_OP_FUNC(elm_widget_item_class_color3_get, _elm_widget_item_class_color3_get), \
   EFL_OBJECT_OP_FUNC(elm_widget_item_class_color_del, _elm_widget_item_class_color_del), \
   EFL_OBJECT_OP_FUNC(elm_widget_item_class_color_clear, _elm_widget_item_class_color_clear)

/*******
 * END *
 *******/

//TIZEN_ONLY(20180607): Restore legacy focus
static void
_if_focused_revert(Evas_Object *obj,
                   Eina_Bool can_focus_only)
{
   Evas_Object *top;
   Evas_Object *newest = NULL;
   unsigned int newest_focus_order = 0;

   INTERNAL_ENTRY;

   if (!sd->focused) return;
   if (!sd->parent_obj) return;

   top = elm_widget_top_get(sd->parent_obj);
   if (top)
     {
        newest = efl_ui_widget_newest_focus_order_get
           (top, &newest_focus_order, can_focus_only);
        if (newest)
          {
             if (newest == top)
               {
                  ELM_WIDGET_DATA_GET(newest, sd2);
                  if (!sd2) return;

                  if (!_is_focused(newest))
                    efl_ui_widget_focus_steal(newest, NULL);
                  else
                    {
                       if (sd2->resize_obj && _is_focused(sd2->resize_obj))
                         efl_ui_widget_focused_object_clear(sd2->resize_obj);
                       else
                         {
                            const Eina_List *l;
                            Evas_Object *child;
                            EINA_LIST_FOREACH(sd2->subobjs, l, child)
                              {
                                 if (!_elm_widget_is(child)) continue;
                                 if (_is_focused(child))
                                   {
                                      efl_ui_widget_focused_object_clear(child);
                                      break;
                                   }
                              }
                         }
                    }
                  evas_object_focus_set(newest, EINA_TRUE);
               }
             else
               {
                  elm_object_focus_set(newest, EINA_FALSE);
                  elm_object_focus_set(newest, EINA_TRUE);
               }
          }
     }
}

/**
 * @internal
 *
 * Set custom focus chain.
 *
 * This function i set one new and overwrite any previous custom focus chain
 * with the list of objects. The previous list will be deleted and this list
 * will be managed. After setted, don't modity it.
 *
 * @note On focus cycle, only will be evaluated children of this container.
 *
 * @param obj The container widget
 * @param objs Chain of objects to pass focus
 * @ingroup Widget
 */
EOLIAN static void
_efl_ui_widget_focus_custom_chain_set(Eo *obj, Elm_Widget_Smart_Data *sd, Eina_List *objs)
{
   if (!_elm_widget_focus_chain_manager_is(obj)) return;

   efl_ui_widget_focus_custom_chain_unset(obj);

   Eina_List *l;
   Evas_Object *o;

   EINA_LIST_FOREACH(objs, l, o)
     {
        evas_object_event_callback_add(o, EVAS_CALLBACK_DEL,
                                       _elm_object_focus_chain_del_cb, obj);
     }

   sd->focus_chain = objs;
}

/**
 * @internal
 *
 * Get custom focus chain
 *
 * @param obj The container widget
 * @ingroup Widget
 */
EOLIAN static const Eina_List*
_efl_ui_widget_focus_custom_chain_get(const Eo *obj EINA_UNUSED, Elm_Widget_Smart_Data *sd)
{
   return (const Eina_List *)sd->focus_chain;
}

/**
 * @internal
 *
 * Unset custom focus chain
 *
 * @param obj The container widget
 * @ingroup Widget
 */
EOLIAN static void
_efl_ui_widget_focus_custom_chain_unset(Eo *obj EINA_UNUSED, Elm_Widget_Smart_Data *sd)
{
   Eina_List *l, *l_next;
   Evas_Object *o;

   EINA_LIST_FOREACH_SAFE(sd->focus_chain, l, l_next, o)
     {
        evas_object_event_callback_del_full(o, EVAS_CALLBACK_DEL,
                                            _elm_object_focus_chain_del_cb, obj);
        sd->focus_chain = eina_list_remove_list(sd->focus_chain, l);
     }
}

/**
 * @internal
 *
 * Append object to custom focus chain.
 *
 * @note If relative_child equal to NULL or not in custom chain, the object
 * will be added in end.
 *
 * @note On focus cycle, only will be evaluated children of this container.
 *
 * @param obj The container widget
 * @param child The child to be added in custom chain
 * @param relative_child The relative object to position the child
 * @ingroup Widget
 */
EOLIAN static void
_efl_ui_widget_focus_custom_chain_append(Eo *obj, Elm_Widget_Smart_Data *sd, Evas_Object *child, Evas_Object *relative_child)
{
   EINA_SAFETY_ON_NULL_RETURN(child);

   if (!_elm_widget_focus_chain_manager_is(obj)) return;

   evas_object_event_callback_add(child, EVAS_CALLBACK_DEL,
                                  _elm_object_focus_chain_del_cb, obj);

   if (!relative_child)
     sd->focus_chain = eina_list_append(sd->focus_chain, child);
   else
     sd->focus_chain = eina_list_append_relative(sd->focus_chain,
                                                 child, relative_child);
}

/**
 * @internal
 *
 * Prepend object to custom focus chain.
 *
 * @note If relative_child equal to NULL or not in custom chain, the object
 * will be added in begin.
 *
 * @note On focus cycle, only will be evaluated children of this container.
 *
 * @param obj The container widget
 * @param child The child to be added in custom chain
 * @param relative_child The relative object to position the child
 * @ingroup Widget
 */
EOLIAN static void
_efl_ui_widget_focus_custom_chain_prepend(Eo *obj, Elm_Widget_Smart_Data *sd, Evas_Object *child, Evas_Object *relative_child)
{
   EINA_SAFETY_ON_NULL_RETURN(child);

   if (!_elm_widget_focus_chain_manager_is(obj)) return;

   evas_object_event_callback_add(child, EVAS_CALLBACK_DEL,
                                  _elm_object_focus_chain_del_cb, obj);

   if (!relative_child)
     sd->focus_chain = eina_list_prepend(sd->focus_chain, child);
   else
     sd->focus_chain = eina_list_prepend_relative(sd->focus_chain,
                                                  child, relative_child);
}

/**
 * @internal
 *
 * Give focus to next object in object tree.
 *
 * Give focus to next object in focus chain of one object sub-tree.
 * If the last object of chain already have focus, the focus will go to the
 * first object of chain.
 *
 * @param obj The widget root of sub-tree
 * @param dir Direction to cycle the focus
 *
 * @ingroup Widget
 */
EOLIAN static void
_efl_ui_widget_focus_cycle(Eo *obj, Elm_Widget_Smart_Data *_pd EINA_UNUSED, Efl_Ui_Focus_Direction dir)
{
   Evas_Object *target = NULL;
   Elm_Object_Item *target_item = NULL;
   if (!_elm_widget_is(obj))
     return;
   efl_ui_widget_focus_next_get(obj, dir, &target, &target_item);
   if (target)
     {
        /* access */
        if (_elm_config->access_mode)
          {
             /* highlight cycle does not steal a focus, only after window gets
                the ECORE_X_ATOM_E_ILLUME_ACCESS_ACTION_ACTIVATE message,
                target will steal focus, or focus its own job. */
             if (!_elm_access_auto_highlight_get())
               efl_ui_widget_focus_steal(target, target_item);

             _elm_access_highlight_set(target);
             elm_widget_focus_region_show(target);
          }
        else efl_ui_widget_focus_steal(target, target_item);
     }
}

/**
 * @internal
 *
 * Give focus to near object(in object tree) in one direction.
 *
 * Give focus to near object(in object tree) in direction of current
 * focused object.  If none focusable object in given direction or
 * none focused object in object tree, the focus will not change.
 *
 * @param obj The reference widget
 * @param degree Degree changes clockwise. i.e. 0-degree: Up,
 *               90-degree: Right, 180-degree: Down, and 270-degree: Left
 * @return EINA_TRUE if focus is moved.
 *
 * @ingroup Widget
 */
EOLIAN static Eina_Bool
_efl_ui_widget_focus_direction_go(Eo *obj, Elm_Widget_Smart_Data *_pd EINA_UNUSED, double degree)
{
   Evas_Object *target = NULL;
   Elm_Object_Item *target_item = NULL;
   Evas_Object *current_focused = NULL;
   double weight = 0.0;

   if (!_elm_widget_is(obj)) return EINA_FALSE;
   if (!_is_focused(obj)) return EINA_FALSE;

   current_focused = efl_ui_widget_focused_object_get(obj);

   if (efl_ui_widget_focus_direction_get
         (obj, current_focused, degree, &target, &target_item, &weight))
     {
        efl_ui_widget_focus_steal(target, NULL);
        return EINA_TRUE;
     }

   return EINA_FALSE;
}

/**
 * @internal
 *
 * Get near object in one direction of base object.
 *
 * Get near object(in the object sub-tree) in one direction of
 * base object. Return the near object by reference.
 * By initializing weight, you can filter objects locating far
 * from base object. If object is in the specific direction,
 * weight is (1/(distance^2)). If object is not exactly in one
 * direction, some penalty will be added.
 *
 * @param obj The widget root of sub-tree
 * @param base The base object of the direction
 * @param degree Degree changes clockwise. i.e. 0-degree: Up,
 *               90-degree: Right, 180-degree: Down, and 270-degree: Left
 * @param direction The near object in one direction
 * @param weight The weight is bigger when the object is located near
 * @return EINA_TRUE if near object is updated.
 *
 * @ingroup Widget
 */

EOLIAN static Eina_Bool
_efl_ui_widget_focus_direction_get(const Eo *obj, Elm_Widget_Smart_Data *sd, const Evas_Object *base, double degree, Evas_Object **direction, Elm_Object_Item **direction_item, double *weight)
{
   double c_weight;

   /* -1 means the best was already decided. Don't need any more searching. */
   if (!direction || !weight || !base || (obj == base))
     return EINA_FALSE;

   /* Ignore if disabled */
   if ((!evas_object_visible_get(obj))
       || (elm_widget_disabled_get(obj))
       || (elm_widget_tree_unfocusable_get(obj)))
     return EINA_FALSE;

   /* Try use hook */
   if (_internal_elm_widget_focus_direction_manager_is(obj))
     {
        Eina_Bool int_ret = EINA_FALSE;
        int_ret = efl_ui_widget_focus_direction((Eo *)obj, base, degree, direction, direction_item, weight);
        return int_ret;
     }

   if (!elm_widget_can_focus_get(obj) || _is_focused((Eo *)obj))
     return EINA_FALSE;

   c_weight = _elm_widget_focus_direction_weight_get(base, obj, degree);
   if ((c_weight == -1.0) ||
       ((c_weight != 0.0) && (*weight != -1.0) &&
        ((int)(*weight * 1000000) <= (int)(c_weight * 1000000))))
     {
        if (*direction &&
            ((int)(*weight * 1000000) == (int)(c_weight * 1000000)))
          {
             ELM_WIDGET_DATA_GET(*direction, sd1);
             if (sd1)
               {
                  if (sd->focus_order <= sd1->focus_order)
                    return EINA_FALSE;
               }
          }
        *direction = (Evas_Object *)obj;
        *weight = c_weight;
        return EINA_TRUE;
     }

   return EINA_FALSE;
}

/**
 * @internal
 *
 * Get near object in one direction of base object in list.
 *
 * Get near object in one direction of base object in the specific
 * object list. Return the near object by reference.
 * By initializing weight, you can filter objects locating far
 * from base object. If object is in the specific direction,
 * weight is (1/(distance^2)). If object is not exactly in one
 * direction, some penalty will be added.
 *
 * @param obj The widget root of sub-tree
 * @param base The base object of the direction
 * @param items list with ordered objects
 * @param list_data_get function to get the object from one item of list
 * @param degree Degree changes clockwise. i.e. 0-degree: Up,
 *               90-degree: Right, 180-degree: Down, and 270-degree: Left
 * @param direction The near object in one direction
 * @param weight The weight is bigger when the object is located near
 * @return EINA_TRUE if near object is updated.
 *
 * @ingroup Widget
 */
EOLIAN static Eina_Bool
_efl_ui_widget_focus_list_direction_get(const Eo *obj EINA_UNUSED, Elm_Widget_Smart_Data *_pd EINA_UNUSED, const Evas_Object *base, const Eina_List *items, void* list_data_get, double degree, Evas_Object **direction, Elm_Object_Item **direction_item, double *weight)
{
   if (!direction || !weight || !base || !items)
     return EINA_FALSE;

   const Eina_List *l = items;
   Evas_Object *current_best = *direction;

   for (; l; l = eina_list_next(l))
     {
        Evas_Object *cur = ((list_data_get_func_type)list_data_get)(l);
        if (cur && _elm_widget_is(cur))
          efl_ui_widget_focus_direction_get(cur, base, degree, direction, direction_item, weight);
     }
   if (current_best != *direction) return EINA_TRUE;

   return EINA_FALSE;
}

/**
 * @internal
 *
 * Get next object in focus chain of object tree.
 *
 * Get next object in focus chain of one object sub-tree.
 * Return the next object by reference. If don't have any candidate to receive
 * focus before chain end, the first candidate will be returned.
 *
 * @param obj The widget root of sub-tree
 * @param dir Direction of focus chain
 * @param next The next object in focus chain
 * @return EINA_TRUE if don't need focus chain restart/loop back
 *         to use 'next' obj.
 *
 * @ingroup Widget
 */
EOLIAN static Eina_Bool
_efl_ui_widget_focus_next_get(const Eo *obj, Elm_Widget_Smart_Data *sd, Efl_Ui_Focus_Direction dir, Evas_Object **next, Elm_Object_Item **next_item)
{
   Elm_Access_Info *ac;

   if (!next)
     return EINA_FALSE;
   *next = NULL;

   /* Ignore if disabled */
   if (_elm_config->access_mode && _elm_access_auto_highlight_get())
     {
        if (!evas_object_visible_get(obj)
            || (elm_widget_tree_unfocusable_get(obj)))
          return EINA_FALSE;
     }
   else
     {
        if ((!evas_object_visible_get(obj))
            || (elm_widget_disabled_get(obj))
            || (elm_widget_tree_unfocusable_get(obj)))
          return EINA_FALSE;
     }

   /* Try use hook */
   if (_elm_widget_focus_chain_manager_is(obj))
     {
        Eina_Bool int_ret = EINA_FALSE;
        int_ret = efl_ui_widget_focus_next((Eo *)obj, dir, next, next_item);
        if (!int_ret && _is_focused((Eo *)obj))
          {
             Evas_Object *o = NULL;
             if (dir == EFL_UI_FOCUS_DIRECTION_PREVIOUS)
               *next_item = sd->item_focus_previous;
             else if (dir == EFL_UI_FOCUS_DIRECTION_NEXT)
               *next_item = sd->item_focus_next;
             else if (dir == EFL_UI_FOCUS_DIRECTION_UP)
               *next_item = sd->item_focus_up;
             else if (dir == EFL_UI_FOCUS_DIRECTION_DOWN)
               *next_item = sd->item_focus_down;
             else if (dir == EFL_UI_FOCUS_DIRECTION_RIGHT)
               *next_item = sd->item_focus_right;
             else if (dir == EFL_UI_FOCUS_DIRECTION_LEFT)
               *next_item = sd->item_focus_left;
             if (*next_item)
               o = elm_object_item_widget_get(*next_item);

             if (!o)
               {
                  if (dir == EFL_UI_FOCUS_DIRECTION_PREVIOUS)
                    o = sd->focus_previous;
                  else if (dir == EFL_UI_FOCUS_DIRECTION_NEXT)
                    o = sd->focus_next;
                  else if (dir == EFL_UI_FOCUS_DIRECTION_UP)
                    o = sd->focus_up;
                  else if (dir == EFL_UI_FOCUS_DIRECTION_DOWN)
                    o = sd->focus_down;
                  else if (dir == EFL_UI_FOCUS_DIRECTION_RIGHT)
                    o = sd->focus_right;
                  else if (dir == EFL_UI_FOCUS_DIRECTION_LEFT)
                    o = sd->focus_left;
               }

             if (o)
               {
                  *next = o;
                  return EINA_TRUE;
               }
          }
        return int_ret;
     }

   /* access object does not check sd->can_focus, because an object could
      have highlight even though the object is not focusable. */
   if (_elm_config->access_mode && _elm_access_auto_highlight_get())
     {
        ac = _elm_access_info_get(obj);
        if (!ac) return EINA_FALSE;

        /* check whether the hover object is visible or not */
        if (!evas_object_visible_get(ac->hoverobj))
          return EINA_FALSE;
     }
   else if (!elm_widget_can_focus_get(obj))
     return EINA_FALSE;

   if (_is_focused((Eo *)obj))
     {
        if (dir == EFL_UI_FOCUS_DIRECTION_PREVIOUS)
          *next_item = sd->item_focus_previous;
        else if (dir == EFL_UI_FOCUS_DIRECTION_NEXT)
          *next_item = sd->item_focus_next;
        else if (dir == EFL_UI_FOCUS_DIRECTION_UP)
          *next_item = sd->item_focus_up;
        else if (dir == EFL_UI_FOCUS_DIRECTION_DOWN)
          *next_item = sd->item_focus_down;
        else if (dir == EFL_UI_FOCUS_DIRECTION_RIGHT)
          *next_item = sd->item_focus_right;
        else if (dir == EFL_UI_FOCUS_DIRECTION_LEFT)
          *next_item = sd->item_focus_left;
        if (*next_item) *next = elm_object_item_widget_get(*next_item);

        if (!(*next))
          {
             if (dir == EFL_UI_FOCUS_DIRECTION_PREVIOUS)
               *next = sd->focus_previous;
             else if (dir == EFL_UI_FOCUS_DIRECTION_NEXT)
               *next = sd->focus_next;
             else if (dir == EFL_UI_FOCUS_DIRECTION_UP)
               *next = sd->focus_up;
             else if (dir == EFL_UI_FOCUS_DIRECTION_DOWN)
               *next = sd->focus_down;
             else if (dir == EFL_UI_FOCUS_DIRECTION_RIGHT)
               *next = sd->focus_right;
             else if (dir == EFL_UI_FOCUS_DIRECTION_LEFT)
               *next = sd->focus_left;
          }

        if (*next) return EINA_TRUE;
     }

   /* Return */
   *next = (Evas_Object *)obj;
   return !ELM_WIDGET_FOCUS_GET(obj);
}

/**
 * @internal
 *
 * Get next object in focus chain of object tree in list.
 *
 * Get next object in focus chain of one object sub-tree ordered by one list.
 * Return the next object by reference. If don't have any candidate to receive
 * focus before list end, the first candidate will be returned.
 *
 * @param obj The widget root of sub-tree
 * @param items list with ordered objects
 * @param list_data_get function to get the object from one item of list
 * @param dir Direction of focus chain
 * @param next The next object in focus chain
 * @return EINA_TRUE if don't need focus chain restart/loop back
 *         to use 'next' obj.
 *
 * @ingroup Widget
 */
EOLIAN static Eina_Bool
_efl_ui_widget_focus_list_next_get(const Eo *obj, Elm_Widget_Smart_Data *_pd EINA_UNUSED, const Eina_List *items, void * list_data_get, Efl_Ui_Focus_Direction dir, Evas_Object **next, Elm_Object_Item **next_item)
{
   Eina_List *(*list_next)(const Eina_List *list) = NULL;
   Evas_Object *focused_object = NULL;

   if (!next)
     return EINA_FALSE;
   *next = NULL;

   if (!_elm_widget_is(obj))
     return EINA_FALSE;

   if (!items)
     return EINA_FALSE;

   /* When Up, Down, Right, or Left, try direction_get first. */
   focused_object = efl_ui_widget_focused_object_get(obj);
   if (focused_object)
     {
        if ((dir == EFL_UI_FOCUS_DIRECTION_UP)
           || (dir == EFL_UI_FOCUS_DIRECTION_DOWN)
           || (dir == EFL_UI_FOCUS_DIRECTION_RIGHT)
           || (dir == EFL_UI_FOCUS_DIRECTION_LEFT))
          {
             *next_item = efl_ui_widget_focus_next_item_get(focused_object, dir);
             if (*next_item)
               *next = elm_object_item_widget_get(*next_item);
             else
               *next = efl_ui_widget_focus_next_object_get(focused_object, dir);
             if (*next) return EINA_TRUE;
             else
               {
                  Evas_Object *n = NULL;
                  Elm_Object_Item *n_item = NULL;
                  double degree = 0;
                  double weight = 0.0;

                  if (dir == EFL_UI_FOCUS_DIRECTION_UP) degree = 0.0;
                  else if (dir == EFL_UI_FOCUS_DIRECTION_DOWN) degree = 180.0;
                  else if (dir == EFL_UI_FOCUS_DIRECTION_RIGHT) degree = 90.0;
                  else if (dir == EFL_UI_FOCUS_DIRECTION_LEFT) degree = 270.0;

                  if (efl_ui_widget_focus_list_direction_get(obj, focused_object,
                                                          items, list_data_get,
                                                          degree, &n, &n_item,
                                                          &weight))
                    {
                       *next_item = n_item;
                       *next = n;
                       return EINA_TRUE;
                    }
               }
          }
     }

   /* Direction */
   if (dir == EFL_UI_FOCUS_DIRECTION_PREVIOUS)
     {
        items = eina_list_last(items);
        list_next = eina_list_prev;
     }
   else if ((dir == EFL_UI_FOCUS_DIRECTION_NEXT)
            || (dir == EFL_UI_FOCUS_DIRECTION_UP)
            || (dir == EFL_UI_FOCUS_DIRECTION_DOWN)
            || (dir == EFL_UI_FOCUS_DIRECTION_RIGHT)
            || (dir == EFL_UI_FOCUS_DIRECTION_LEFT))
     list_next = eina_list_next;
   else
     return EINA_FALSE;

   const Eina_List *l = items;

   /* Recovery last focused sub item */
   if (ELM_WIDGET_FOCUS_GET(obj))
     {
        for (; l; l = list_next(l))
          {
             Evas_Object *cur = ((list_data_get_func_type)list_data_get)(l);
             if (ELM_WIDGET_FOCUS_GET(cur)) break;
          }

         /* Focused object, but no focused sub item */
         if (!l) l = items;
     }

   const Eina_List *start = l;
   Evas_Object *to_focus = NULL;
   Elm_Object_Item *to_focus_item = NULL;

   /* Iterate sub items */
   /* Go to the end of list */
   for (; l; l = list_next(l))
     {
        Evas_Object *tmp = NULL;
        Elm_Object_Item *tmp_item = NULL;
        Evas_Object *cur = ((list_data_get_func_type)list_data_get)(l);

        if (!cur) continue;
        if (!_elm_widget_is(cur)) continue;
        if (elm_widget_parent_get(cur) != obj)
          continue;

        /* Try Focus cycle in subitem */
        if (efl_ui_widget_focus_next_get(cur, dir, &tmp, &tmp_item))
          {
             *next = tmp;
             *next_item = tmp_item;
             return EINA_TRUE;
          }
        else if ((dir == EFL_UI_FOCUS_DIRECTION_UP)
                 || (dir == EFL_UI_FOCUS_DIRECTION_DOWN)
                 || (dir == EFL_UI_FOCUS_DIRECTION_RIGHT)
                 || (dir == EFL_UI_FOCUS_DIRECTION_LEFT))
          {
             if (tmp && _is_focused(cur))
               {
                  *next = tmp;
                  *next_item = tmp_item;
                  return EINA_FALSE;
               }
          }
        else if ((tmp) && (!to_focus))
          {
             to_focus = tmp;
             to_focus_item = tmp_item;
          }
     }

   l = items;

   /* Get First possible */
   for (; l != start; l = list_next(l))
     {
        Evas_Object *tmp = NULL;
        Elm_Object_Item *tmp_item = NULL;
        Evas_Object *cur = ((list_data_get_func_type)list_data_get)(l);

        if (elm_widget_parent_get(cur) != obj)
          continue;

        /* Try Focus cycle in subitem */
        efl_ui_widget_focus_next_get(cur, dir, &tmp, &tmp_item);
        if (tmp)
          {
             *next = tmp;
             *next_item = tmp_item;
             return EINA_FALSE;
          }
     }

   *next = to_focus;
   *next_item = to_focus_item;
   return EINA_FALSE;
}

/**
 * @internal
 *
 * Get next object which was set with specific focus direction.
 *
 * Get next object which was set by elm_widget_focus_next_object_set
 * with specific focus directioin.
 *
 * @param obj The widget
 * @param dir Direction of focus
 * @return Widget which was registered with sepecific focus direction.
 *
 * @ingroup Widget
 */
EOLIAN static Evas_Object*
_efl_ui_widget_focus_next_object_get(const Eo *obj EINA_UNUSED, Elm_Widget_Smart_Data *sd, Efl_Ui_Focus_Direction dir)
{
   Evas_Object *ret = NULL;

   if (dir == EFL_UI_FOCUS_DIRECTION_PREVIOUS)
     ret = sd->focus_previous;
   else if (dir == EFL_UI_FOCUS_DIRECTION_NEXT)
     ret = sd->focus_next;
   else if (dir == EFL_UI_FOCUS_DIRECTION_UP)
     ret = sd->focus_up;
   else if (dir == EFL_UI_FOCUS_DIRECTION_DOWN)
     ret = sd->focus_down;
   else if (dir == EFL_UI_FOCUS_DIRECTION_RIGHT)
     ret = sd->focus_right;
   else if (dir == EFL_UI_FOCUS_DIRECTION_LEFT)
     ret = sd->focus_left;

   return ret;
}

/**
 * @internal
 *
 * Set next object with specific focus direction.
 *
 * When a widget is set with specific focus direction, this widget will be
 * the first candidate when finding the next focus object.
 * Focus next object can be registered with six directions that are previous,
 * next, up, down, right, and left.
 *
 * @param obj The widget
 * @param next Next focus object
 * @param dir Direction of focus
 *
 * @ingroup Widget
 */
EOLIAN static void
_efl_ui_widget_focus_next_object_set(Eo *obj EINA_UNUSED, Elm_Widget_Smart_Data *sd, Evas_Object *next, Efl_Ui_Focus_Direction dir)
{

   if (dir == EFL_UI_FOCUS_DIRECTION_PREVIOUS)
     sd->focus_previous = next;
   else if (dir == EFL_UI_FOCUS_DIRECTION_NEXT)
     sd->focus_next = next;
   else if (dir == EFL_UI_FOCUS_DIRECTION_UP)
     sd->focus_up = next;
   else if (dir == EFL_UI_FOCUS_DIRECTION_DOWN)
     sd->focus_down = next;
   else if (dir == EFL_UI_FOCUS_DIRECTION_RIGHT)
     sd->focus_right = next;
   else if (dir == EFL_UI_FOCUS_DIRECTION_LEFT)
     sd->focus_left = next;

}

EOLIAN static Elm_Object_Item*
_efl_ui_widget_focus_next_item_get(const Eo *obj EINA_UNUSED, Elm_Widget_Smart_Data *sd, Efl_Ui_Focus_Direction dir)
{
   Elm_Object_Item *ret = NULL;

   if (dir == EFL_UI_FOCUS_DIRECTION_PREVIOUS)
     ret = sd->item_focus_previous;
   else if (dir == EFL_UI_FOCUS_DIRECTION_NEXT)
     ret = sd->item_focus_next;
   else if (dir == EFL_UI_FOCUS_DIRECTION_UP)
     ret = sd->item_focus_up;
   else if (dir == EFL_UI_FOCUS_DIRECTION_DOWN)
     ret = sd->item_focus_down;
   else if (dir == EFL_UI_FOCUS_DIRECTION_RIGHT)
     ret = sd->item_focus_right;
   else if (dir == EFL_UI_FOCUS_DIRECTION_LEFT)
     ret = sd->item_focus_left;

   return ret;

}

EOLIAN static void
_efl_ui_widget_focus_next_item_set(Eo *obj EINA_UNUSED, Elm_Widget_Smart_Data *sd, Elm_Object_Item *next_item, Efl_Ui_Focus_Direction dir)
{
   if (dir == EFL_UI_FOCUS_DIRECTION_PREVIOUS)
     sd->item_focus_previous = next_item;
   else if (dir == EFL_UI_FOCUS_DIRECTION_NEXT)
     sd->item_focus_next = next_item;
   else if (dir == EFL_UI_FOCUS_DIRECTION_UP)
     sd->item_focus_up = next_item;
   else if (dir == EFL_UI_FOCUS_DIRECTION_DOWN)
     sd->item_focus_down = next_item;
   else if (dir == EFL_UI_FOCUS_DIRECTION_RIGHT)
     sd->item_focus_right = next_item;
   else if (dir == EFL_UI_FOCUS_DIRECTION_LEFT)
     sd->item_focus_left = next_item;
}

EAPI void
elm_widget_focus_set(Eo *obj, Eina_Bool focus)
{
   ELM_WIDGET_DATA_GET_OR_RETURN(obj, sd);
   if (!sd->focused)
     {
        focus_order++;
        sd->focus_order = focus_order;
        sd->focused = EINA_TRUE;
        efl_ui_focus_object_on_focus_update(obj);
     }

   if (focus)
     {
        if ((_is_focusable(sd->resize_obj)) &&
            (!elm_widget_disabled_get(sd->resize_obj)))
          {
             elm_widget_focus_set(sd->resize_obj, focus);
          }
        else
          {
             const Eina_List *l;
             Evas_Object *child;

             EINA_LIST_FOREACH(sd->subobjs, l, child)
               {
                  if (!_elm_widget_is(child)) continue;
                  if ((_is_focusable(child)) &&
                       (!elm_widget_disabled_get(child)))
                      {
                         elm_widget_focus_set(child, focus);
                         break;
                      }
               }
          }
     }
   else
     {
        const Eina_List *l;
        Evas_Object *child;

        EINA_LIST_REVERSE_FOREACH(sd->subobjs, l, child)
          {
             if (!_elm_widget_is(child)) continue;
             if ((_is_focusable(child)) &&
                 (!elm_widget_disabled_get(child)))
               {
                  elm_widget_focus_set(child, focus);
                  break;
               }
          }
     }
}

static void
_focused_object_clear(Elm_Widget_Smart_Data *sd)
{
   if (sd->resize_obj && elm_widget_is(sd->resize_obj) &&
       _is_focused(sd->resize_obj))
     {
        efl_ui_widget_focused_object_clear(sd->resize_obj);
     }
   else
     {
        const Eina_List *l;
        Evas_Object *child;
        EINA_LIST_FOREACH(sd->subobjs, l, child)
          {
             if (_elm_widget_is(child) && _is_focused(child))
               {
                  efl_ui_widget_focused_object_clear(child);
                  break;
               }
          }
     }
}

EOLIAN static void
_efl_ui_widget_focused_object_clear(Eo *obj, Elm_Widget_Smart_Data *sd)
{
   if (!sd->focused) return;
   _focused_object_clear(sd);
   sd->focused = EINA_FALSE;
   if (sd->top_win_focused)
     efl_ui_focus_object_on_focus_update(obj);
}

EOLIAN static void
_efl_ui_widget_focus_steal(Eo *obj, Elm_Widget_Smart_Data *sd, Elm_Object_Item *item)
{
   Evas_Object *parent, *parent2, *o;

   if (sd->focused) return;
   if (sd->disabled) return;
   if (!sd->can_focus) return;
   if (sd->tree_unfocusable) return;
   parent = obj;
   for (;; )
     {
        o = elm_widget_parent_get(parent);
        if (!o) break;
        sd = efl_data_scope_get(o, MY_CLASS);
        if (sd->disabled || sd->tree_unfocusable) return;
        if (sd->focused) break;
        parent = o;
     }
   if ((!elm_widget_parent_get(parent)))
     efl_ui_widget_focused_object_clear(parent);
   else
     {
        parent2 = elm_widget_parent_get(parent);
        parent = parent2;
        sd = efl_data_scope_get(parent, MY_CLASS);
        if (sd) _focused_object_clear(sd);
     }
   _parent_focus(obj, item);
   elm_widget_focus_region_show(obj);
   return;
}

static void
_parents_on_focus(Evas_Object *obj)
{
   API_ENTRY return;
   if (!sd->focused || !sd->top_win_focused) return;

   Evas_Object *o = elm_widget_parent_get(obj);
   if (o) _parents_on_focus(o);
   efl_ui_focus_object_on_focus_update(obj);
}

EOLIAN static void
_efl_ui_widget_focus_restore(Eo *obj, Elm_Widget_Smart_Data *_pd EINA_UNUSED)
{
   Evas_Object *newest = NULL;
   unsigned int newest_focus_order = 0;

   newest = efl_ui_widget_newest_focus_order_get(obj, &newest_focus_order, EINA_TRUE);
   if (newest)
     _parents_on_focus(newest);
}
//END


EOLIAN static void
_efl_ui_widget_show_region_set(Eo *obj, Elm_Widget_Smart_Data *sd, Eina_Rect sr, Eina_Bool forceshow)
{
   Evas_Object *parent_obj, *child_obj;
   Evas_Coord px, py, cx, cy, nx = 0, ny = 0;

   /*****************************************************************************
    * TIZEN_ONLY_FEATURE: Fix entry size/cursor/region calculation for Tizen UX *
    *****************************************************************************
    * Move this code to the below to update show region geometry properly.
   evas_smart_objects_calculate(evas_object_evas_get(obj));
    */
   /*******
    * END *
    *******/

   if (!forceshow && eina_rectangle_equal(&sr.rect, &sd->show_region.rect)) return;

   sd->show_region = sr;

   /*****************************************************************************
    * TIZEN_ONLY_FEATURE: Fix entry size/cursor/region calculation for Tizen UX *
    *****************************************************************************/
   /* Block nested call for evas_smart_objects_calculate() and region showing works */
   if (sd->on_show_region_set) return;

   sd->on_show_region_set = EINA_TRUE;

   evas_smart_objects_calculate(evas_object_evas_get(obj));

   sd->on_show_region_set = EINA_FALSE;

   /* show_region geometry could be changed during processing elm_widget_show_region_set().
      evas_smart_objects_calculate() can trigger nested show_region_set calls */
   sr = sd->show_region;
   /*******
    * END *
    *******/

   if (sd->on_show_region)
     {
        sd->on_show_region(sd->on_show_region_data, obj, sr);

        if (_elm_scrollable_is(obj))
          {
             if (elm_widget_is_legacy(obj))
               {
                  elm_interface_scrollable_content_pos_get(obj, &nx, &ny);
                  sr.x -= nx;
                  sr.y -= ny;
               }
             else
               {
                  Eina_Position2D pos;
                  pos = efl_ui_scrollable_content_pos_get(obj);
                  sr.x -= pos.x;
                  sr.y -= pos.y;
               }
          }
     }

   do
     {
        parent_obj = sd->parent_obj;
        child_obj = sd->obj;
        if ((!parent_obj) || (!_elm_widget_is(parent_obj))) break;
        sd = efl_data_scope_get(parent_obj, MY_CLASS);
        if (!sd) break;

        evas_object_geometry_get(parent_obj, &px, &py, NULL, NULL);
        evas_object_geometry_get(child_obj, &cx, &cy, NULL, NULL);

        sr.x += (cx - px);
        sr.y += (cy - py);
        sd->show_region = sr;

        if (sd->on_show_region)
          sd->on_show_region(sd->on_show_region_data, parent_obj, sr);
     }
   while (parent_obj);
}

EOLIAN static Eina_Rect
_efl_ui_widget_show_region_get(const Eo *obj EINA_UNUSED, Elm_Widget_Smart_Data *sd)
{
   return (Eina_Rect) sd->show_region;
}

//TIZEN_ONLY(20180607): Restore legacy focus
/**
 * @internal
 *
 * Check if the widget has its own focus next function.
 *
 * @param obj The widget.
 * @return focus next function is implemented/unimplemented.
 * (@c EINA_TRUE = implemented/@c EINA_FALSE = unimplemented.)
 */
static inline Eina_Bool
_elm_widget_focus_chain_manager_is(const Evas_Object *obj)
{
   ELM_WIDGET_CHECK(obj) EINA_FALSE;

   Eina_Bool manager_is = EINA_FALSE;
   manager_is = efl_ui_widget_focus_next_manager_is((Eo *)obj);
   return manager_is;
}

static inline Eina_Bool
_internal_elm_widget_focus_direction_manager_is(const Evas_Object *obj)
{
   ELM_WIDGET_CHECK(obj) EINA_FALSE;

   Eina_Bool manager_is = EINA_FALSE;
   manager_is = efl_ui_widget_focus_direction_manager_is((Eo *)obj);
   return manager_is;
}

static void
_parent_focus(Evas_Object *obj, Elm_Object_Item *item)
{
   API_ENTRY return;

   if (sd->focused) return;

   Evas_Object *o = elm_widget_parent_get(obj);
   sd->focus_order_on_calc = EINA_TRUE;

   if (o) _parent_focus(o, item);

   if (!sd->focus_order_on_calc)
     return;  /* we don't want to override it if by means of any of the
                 callbacks below one gets to calculate our order
                 first. */

   focus_order++;
   sd->focus_order = focus_order;
   sd->focused = EINA_TRUE;

   if (sd->top_win_focused)
     efl_ui_focus_object_on_focus_update(obj);
   sd->focus_order_on_calc = EINA_FALSE;

   if (_elm_config->access_mode == ELM_ACCESS_MODE_ON)
     _elm_access_highlight_set(obj);
}

static void
_elm_object_focus_chain_del_cb(void *data,
                               Evas *e EINA_UNUSED,
                               Evas_Object *obj,
                               void *event_info EINA_UNUSED)
{
   ELM_WIDGET_DATA_GET(data, sd);

   sd->focus_chain = eina_list_remove(sd->focus_chain, obj);
}
//
#include "elm_widget_item_eo.c"
#include "elm_widget_item_container_eo.c"
#include "efl_ui_widget.eo.c"
#include "efl_ui_widget_eo.legacy.c"

/* Others */
#include "efl_ui_l10n.eo.c"
