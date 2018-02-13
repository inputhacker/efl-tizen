#ifdef HAVE_CONFIG_H
# include "elementary_config.h"
#endif

#define EFL_ACCESS_PROTECTED
#define EFL_ACCESS_WIDGET_ACTION_PROTECTED
#define ELM_LAYOUT_PROTECTED
#define EFL_UI_NSTATE_PROTECTED

#include <Elementary.h>
#include "elm_priv.h"
#include "efl_ui_nstate.eo.h"
#include "efl_ui_check_private.h"
#include "efl_ui_nstate_private.h"

#define MY_CLASS EFL_UI_CHECK_CLASS
#define MY_CLASS_PFX efl_ui_check

#define MY_CLASS_NAME "Efl.Ui.Check"
#define MY_CLASS_NAME_LEGACY "elm_check"

static const Elm_Layout_Part_Alias_Description _text_aliases[] =
{
   {"default", "elm.text"},
   {"on", "elm.ontext"},
   {"off", "elm.offtext"},
   {NULL, NULL}
};

static const char SIG_CHANGED[] = "changed";

/* smart callbacks coming from elm check objects: */
static const Evas_Smart_Cb_Description _smart_callbacks[] = {
   {SIG_CHANGED, ""},
   {SIG_WIDGET_LANG_CHANGED, ""}, /**< handled by elm_widget */
   {SIG_WIDGET_ACCESS_CHANGED, ""}, /**< handled by elm_widget */
   {SIG_LAYOUT_FOCUSED, ""}, /**< handled by elm_layout */
   {SIG_LAYOUT_UNFOCUSED, ""}, /**< handled by elm_layout */
   {NULL, NULL}
};

static Eina_Bool _key_action_activate(Evas_Object *obj, const char *params);

static const Elm_Action key_actions[] = {
   {"activate", _key_action_activate},
   {NULL, NULL}
};

static void
_activate(Evas_Object *obj)
{
   EFL_UI_CHECK_DATA_GET(obj, sd);

   if (sd->statep) *sd->statep = !efl_ui_nstate_value_get(obj);

   // state will be changed to 1 by efl_ui_nstate_activate(obj)
   if (efl_ui_nstate_value_get(obj) == 0)
     {
        // FIXME: to do animation during state change , we need different signal
        // so that we can distinguish between state change by user or state change
        // by calling state_change() api. Keep both the signal for backward compatibility
        // and remove "elm,state,check,on" signal emission when we can break ABI.
        elm_layout_signal_emit(obj, "elm,activate,check,on", "elm");
        elm_layout_signal_emit(obj, "elm,state,check,on", "elm");
        if (_elm_config->access_mode != ELM_ACCESS_MODE_OFF)
             _elm_access_say(E_("State: On"));
     }
   else
     {
        // FIXME: to do animation during state change , we need different signal
        // so that we can distinguish between state change by user or state change
        // by calling state_change() api. Keep both the signal for backward compatibility
        // and remove "elm,state,check,off" signal emission when we can break ABI.
        elm_layout_signal_emit(obj, "elm,activate,check,off", "elm");
        elm_layout_signal_emit(obj, "elm,state,check,off", "elm");
        if (_elm_config->access_mode != ELM_ACCESS_MODE_OFF)
             _elm_access_say(E_("State: Off"));
     }

   efl_ui_nstate_activate(obj);

   if (_elm_config->atspi_mode)
     efl_access_state_changed_signal_emit(obj,
                                          EFL_ACCESS_STATE_CHECKED,
                                          efl_ui_nstate_value_get(obj));
}

/* FIXME: replicated from elm_layout just because check's icon spot
 * is elm.swallow.content, not elm.swallow.icon. Fix that whenever we
 * can changed the theme API */
static void
_icon_signal_emit(Evas_Object *obj)
{
   ELM_WIDGET_DATA_GET_OR_RETURN(obj, wd);
   char buf[64];

   if (!elm_widget_resize_object_get(obj)) return;
   snprintf(buf, sizeof(buf), "elm,state,icon,%s",
            elm_layout_content_get(obj, "icon") ? "visible" : "hidden");

   elm_layout_signal_emit(obj, buf, "elm");
   edje_object_message_signal_process(wd->resize_obj);
}

EOLIAN static Efl_Access_State_Set
_efl_ui_check_efl_access_state_set_get(Eo *obj, Efl_Ui_Check_Data *_pd EINA_UNUSED)
{
   Efl_Access_State_Set states = 0;

   states = efl_access_state_set_get(efl_super(obj, EFL_UI_CHECK_CLASS));

   if (elm_check_state_get(obj))
       STATE_TYPE_SET(states, EFL_ACCESS_STATE_CHECKED);

   return states;
}

/* FIXME: replicated from elm_layout just because check's icon spot
 * is elm.swallow.content, not elm.swallow.icon. Fix that whenever we
 * can changed the theme API */
EOLIAN static Eina_Bool
_efl_ui_check_efl_ui_widget_widget_sub_object_del(Eo *obj, Efl_Ui_Check_Data *_pd EINA_UNUSED, Evas_Object *sobj)
{
   Eina_Bool int_ret = EINA_FALSE;

   int_ret = elm_widget_sub_object_del(efl_super(obj, MY_CLASS), sobj);
   if (!int_ret) return EINA_FALSE;

   _icon_signal_emit(obj);

   elm_layout_sizing_eval(obj);

   return EINA_TRUE;
}

EOLIAN static Eina_Bool
_efl_ui_check_efl_ui_widget_on_access_activate(Eo *obj EINA_UNUSED, Efl_Ui_Check_Data *_pd EINA_UNUSED, Efl_Ui_Activate act)
{
   if (elm_widget_disabled_get(obj)) return EINA_FALSE;
   if (act != EFL_UI_ACTIVATE_DEFAULT) return EINA_FALSE;

   _activate(obj);

   return EINA_TRUE;
}

static Eina_Bool
_key_action_activate(Evas_Object *obj, const char *params EINA_UNUSED)
{
   _activate(obj);
   return EINA_TRUE;
}

EOLIAN static Efl_Ui_Theme_Apply
_efl_ui_check_efl_ui_widget_theme_apply(Eo *obj, Efl_Ui_Check_Data *sd EINA_UNUSED)
{
   Efl_Ui_Theme_Apply int_ret = EFL_UI_THEME_APPLY_FAILED;

   ELM_WIDGET_DATA_GET_OR_RETURN(obj, wd, EFL_UI_THEME_APPLY_FAILED);

   int_ret = efl_ui_widget_theme_apply(efl_super(obj, MY_CLASS));

   if (!int_ret) return EFL_UI_THEME_APPLY_FAILED;

   if (efl_ui_nstate_value_get(obj) == 0)
     elm_layout_signal_emit(obj, "elm,state,check,off", "elm");
   else
     elm_layout_signal_emit(obj, "elm,state,check,on", "elm");

   edje_object_message_signal_process(wd->resize_obj);

   /* FIXME: replicated from elm_layout just because check's icon spot
    * is elm.swallow.content, not elm.swallow.icon. Fix that whenever
    * we can changed the theme API */
   _icon_signal_emit(obj);

   elm_layout_sizing_eval(obj);

   return int_ret;
}

static char *
_access_info_cb(void *data EINA_UNUSED, Evas_Object *obj)
{
   const char *txt = elm_widget_access_info_get(obj);

   if (!txt) txt = elm_layout_text_get(obj, NULL);
   if (txt) return strdup(txt);

   return NULL;
}

static char *
_access_state_cb(void *data, Evas_Object *obj)
{
   const char *on_text, *off_text;

   if (elm_widget_disabled_get(obj))
     return strdup(E_("State: Disabled"));
   if (efl_ui_nstate_value_get(obj))
     {
        on_text = elm_layout_text_get(data, "on");

        if (on_text)
          {
             char buf[1024];

             snprintf(buf, sizeof(buf), "%s: %s", E_("State"), on_text);
             return strdup(buf);
          }
        else
          return strdup(E_("State: On"));
     }

   off_text = elm_layout_text_get(data, "off");

   if (off_text)
     {
        char buf[1024];

        snprintf(buf, sizeof(buf), "%s: %s", E_("State"), off_text);
        return strdup(buf);
     }
   return strdup(E_("State: Off"));
}

static void
_on_check_off(void *data,
              Evas_Object *o EINA_UNUSED,
              const char *emission EINA_UNUSED,
              const char *source EINA_UNUSED)
{
   Evas_Object *obj = data;

   EFL_UI_CHECK_DATA_GET(obj, sd);

   if (sd->statep) *sd->statep = 0;
   elm_layout_signal_emit(obj, "elm,state,check,off", "elm");
   efl_ui_nstate_value_set(obj, 0);

   if (_elm_config->atspi_mode)
     efl_access_state_changed_signal_emit(data,
                                          EFL_ACCESS_STATE_CHECKED,
                                          efl_ui_nstate_value_get(obj));
}

static void
_on_check_on(void *data,
             Evas_Object *o EINA_UNUSED,
             const char *emission EINA_UNUSED,
             const char *source EINA_UNUSED)
{
   Evas_Object *obj = data;

   EFL_UI_CHECK_DATA_GET(obj, sd);

   if (sd->statep) *sd->statep = 1;
   elm_layout_signal_emit(obj, "elm,state,check,on", "elm");
   efl_ui_nstate_value_set(obj, 1);

   if (_elm_config->atspi_mode)
     efl_access_state_changed_signal_emit(data,
                                          EFL_ACCESS_STATE_CHECKED,
                                          efl_ui_nstate_value_get(obj));
}

static void
_on_check_toggle(void *data,
                 Evas_Object *o EINA_UNUSED,
                 const char *emission EINA_UNUSED,
                 const char *source EINA_UNUSED)
{
   _activate(data);
}

EOLIAN static Eina_Bool
_efl_ui_check_selected_get(Eo *obj, Efl_Ui_Check_Data *pd EINA_UNUSED)
{
   return !!efl_ui_nstate_value_get(obj);
}

EOLIAN static void
_efl_ui_check_selected_set(Eo *obj, Efl_Ui_Check_Data *sd, Eina_Bool value)
{
   ELM_WIDGET_DATA_GET_OR_RETURN(obj, wd);

   if (sd->statep) *sd->statep = value;

   if (value == 1)
     elm_layout_signal_emit(obj, "elm,state,check,on", "elm");
   else
     elm_layout_signal_emit(obj, "elm,state,check,off", "elm");

   edje_object_message_signal_process(wd->resize_obj);

   efl_ui_nstate_value_set(obj, value);
}

EOLIAN static void
_efl_ui_check_efl_ui_nstate_count_set(Eo *obj EINA_UNUSED, Efl_Ui_Check_Data *pd EINA_UNUSED, int nstate EINA_UNUSED)
{
   //NOP;
}

EOLIAN static void
_efl_ui_check_efl_ui_nstate_value_set(Eo *obj, Efl_Ui_Check_Data *pd EINA_UNUSED, int state)
{
   Eina_Bool _state = !!state;
   if (_state == efl_ui_nstate_value_get(obj)) return;

   efl_ui_nstate_value_set(efl_super(obj, MY_CLASS), _state);
}

EAPI Evas_Object *
elm_check_add(Evas_Object *parent)
{
   EINA_SAFETY_ON_NULL_RETURN_VAL(parent, NULL);
   return elm_legacy_add(MY_CLASS, parent);
}

EOLIAN static Eo *
_efl_ui_check_efl_object_constructor(Eo *obj, Efl_Ui_Check_Data *pd EINA_UNUSED)
{
   if (!elm_widget_theme_klass_get(obj))
     elm_widget_theme_klass_set(obj, "check");
   obj = efl_constructor(efl_super(obj, MY_CLASS));
   efl_canvas_object_type_set(obj, MY_CLASS_NAME_LEGACY);
   evas_object_smart_callbacks_descriptions_set(obj, _smart_callbacks);

   ELM_WIDGET_DATA_GET_OR_RETURN(obj, wd, NULL);
   efl_layout_signal_callback_add
     (wd->resize_obj, "elm,action,check,on", "*", _on_check_on, obj);
   efl_layout_signal_callback_add
     (wd->resize_obj, "elm,action,check,off", "*", _on_check_off, obj);
   efl_layout_signal_callback_add
     (wd->resize_obj, "elm,action,check,toggle", "*", _on_check_toggle, obj);

   efl_access_role_set(obj, EFL_ACCESS_ROLE_CHECK_BOX);
   _elm_access_object_register(obj, wd->resize_obj);
   _elm_access_text_set
     (_elm_access_info_get(obj), ELM_ACCESS_TYPE, E_("Check"));
   _elm_access_callback_set
     (_elm_access_info_get(obj), ELM_ACCESS_INFO, _access_info_cb, obj);
   _elm_access_callback_set
     (_elm_access_info_get(obj), ELM_ACCESS_STATE, _access_state_cb, obj);

   elm_widget_can_focus_set(obj, EINA_TRUE);

   return obj;
}

EAPI void
elm_check_state_set(Evas_Object *obj, Eina_Bool state)
{
   ELM_WIDGET_DATA_GET_OR_RETURN(obj, wd);
   EFL_UI_NSTATE_DATA_GET_OR_RETURN(obj, nd);
   EFL_UI_CHECK_DATA_GET_OR_RETURN(obj, sd);

   if (state != nd->state)
     {
        nd->state = state;
        if (sd->statep) *sd->statep = state;

        if (state == 1)
          elm_layout_signal_emit(obj, "elm,state,check,on", "elm");
        else
          elm_layout_signal_emit(obj, "elm,state,check,off", "elm");
        edje_object_message_signal_process(wd->resize_obj);
     }
}

EAPI Eina_Bool
elm_check_state_get(const Evas_Object *obj)
{
   return !!efl_ui_nstate_value_get(obj);
}

EAPI void
elm_check_state_pointer_set(Eo *obj, Eina_Bool *statep)
{
   EFL_UI_CHECK_DATA_GET_OR_RETURN(obj, sd);
   EFL_UI_NSTATE_DATA_GET_OR_RETURN(obj, nd);

   if (!statep)
     {
        sd->statep = NULL;
        return;
     }

   sd->statep = statep;
   if (*sd->statep != nd->state)
     {
        nd->state = *sd->statep;
        if (nd->state == 1)
          elm_layout_signal_emit(obj, "elm,state,check,on", "elm");
        else
          elm_layout_signal_emit(obj, "elm,state,check,off", "elm");
     }
}

EOLIAN const Efl_Access_Action_Data *
_efl_ui_check_efl_access_widget_action_elm_actions_get(Eo *obj EINA_UNUSED, Efl_Ui_Check_Data *pd EINA_UNUSED)
{
   static Efl_Access_Action_Data atspi_action[] = {
          { "activate", "activate", NULL, _key_action_activate },
          { NULL, NULL, NULL, NULL }
   };
   return &atspi_action[0];
}

static void
_efl_ui_check_class_constructor(Efl_Class *klass)
{
   evas_smart_legacy_type_register(MY_CLASS_NAME_LEGACY, klass);
}

/* Standard widget overrides */

ELM_WIDGET_KEY_DOWN_DEFAULT_IMPLEMENT(efl_ui_check, Efl_Ui_Check_Data)

/* Internal EO APIs and hidden overrides */

ELM_LAYOUT_TEXT_ALIASES_IMPLEMENT(MY_CLASS_PFX)

#define EFL_UI_CHECK_EXTRA_OPS \
   ELM_LAYOUT_TEXT_ALIASES_OPS(MY_CLASS_PFX)

#include "efl_ui_check.eo.c"
