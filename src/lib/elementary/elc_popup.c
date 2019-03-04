#ifdef HAVE_CONFIG_H
# include "elementary_config.h"
#endif

#define EFL_ACCESS_OBJECT_PROTECTED
#define EFL_ACCESS_WIDGET_ACTION_PROTECTED
#define ELM_WIDGET_PROTECTED
#define ELM_WIDGET_ITEM_PROTECTED
#define EFL_UI_L10N_PROTECTED
#define EFL_PART_PROTECTED

#define EFL_ACCESS_COMPONENT_PROTECTED

#include <Elementary.h>
#include "elm_priv.h"
#include "elm_widget_popup.h"

#include "elm_popup_part.eo.h"
#include "elm_part_helper.h"

#define MY_CLASS ELM_POPUP_CLASS

#define MY_CLASS_NAME "Elm_Popup"
#define MY_CLASS_NAME_LEGACY "elm_popup"

/* TIZEN_ONLY(20161109): check theme compatibility more precise
   Naming rules of some parts in theme is changed in Tizen 3.0,
   so popup must check theme compatibility more precise.

    - notify style changes
      Tizen 2.4 : "popup" -> Tizen 3.0 : "popup/default"
      Tizen 2.4 : "toast" -> Tizen 3.0 : "popup/toast"

    - content area & action area changes
      Tizen 2.4 : "default" -> Tizen 3.0 : "popup/default"
      Tizen 2.4 : "toast" -> Tizen 3.0 : "popup/toast"

   For some third party app which customize 2.4 popup theme,
   popup must check 4 styles according to the below style priority.
 */
#define STYLE_PRIORITY_COUNT 4
/* END */

static void _button_remove(Evas_Object *, int, Eina_Bool);

static const char ACCESS_TITLE_PART[] = "access.title";
static const char ACCESS_BODY_PART[] = "access.body";
static const char CONTENT_PART[] = "elm.swallow.content";

static const char SIG_BLOCK_CLICKED[] = "block,clicked";
static const char SIG_TIMEOUT[] = "timeout";
static const char SIG_ITEM_FOCUSED[] = "item,focused";
static const char SIG_ITEM_UNFOCUSED[] = "item,unfocused";
//TIZEN_ONLY(20161213): apply screen_reader_changed callback
static const char SIG_ATSPI_SCREEN_READER_CHANGED[] = "atspi,screen,reader,changed";
//
/* TIZEN_ONLY(20161109): check theme compatibility more precise */
static const char STYLE_PRIORITY[4][15] = { "%s", "popup/%s", "default", "popup/default" };
/* END */

static const Evas_Smart_Cb_Description _smart_callbacks[] = {
   {SIG_BLOCK_CLICKED, ""},
   {SIG_TIMEOUT, ""},
   {SIG_ITEM_FOCUSED, ""},
   {SIG_ITEM_UNFOCUSED, ""},
   {SIG_WIDGET_LANG_CHANGED, ""}, /**< handled by elm_widget */
   {SIG_WIDGET_ACCESS_CHANGED, ""}, /**< handled by elm_widget */
   {SIG_LAYOUT_FOCUSED, ""}, /**< handled by elm_layout */
   {SIG_LAYOUT_UNFOCUSED, ""}, /**< handled by elm_layout */
   //TIZEN_ONLY(20161213): apply screen_reader_changed callback
   {SIG_ATSPI_SCREEN_READER_CHANGED, ""},
   //
   {NULL, NULL}
};

//TIZEN_ONLY(20180607): Restore legacy focus
static Eina_Bool _key_action_move(Evas_Object *obj, const char *params);
//
static Eina_Bool _key_action_escape(Evas_Object *obj, const char *params);
static void _parent_geom_cb(void *data, Evas *e EINA_UNUSED, Evas_Object *obj, void *event_info EINA_UNUSED);
static void _block_clicked_cb(void *data, const Efl_Event *event);
static void _timeout_cb(void *data, const Efl_Event *event);

/* TIZEN_ONLY(20160629) : add "show,finished" internal callback */
static void _show_finished_cb(void *data, const Efl_Event *event);
/* END */

static void _hide_effect_finished_cb(void *data, const Efl_Event *event);

/* TIZEN_ONLY(20161109): add internal style set function */
static Efl_Ui_Theme_Apply_Result _notify_style_set(Evas_Object *obj, const char *style);
/* END */

static const Elm_Action key_actions[] = {
   //TIZEN_ONLY(20180607): Restore legacy focus
   {"move", _key_action_move},
   //
   {"escape", _key_action_escape},
   {NULL, NULL}
};

EFL_CALLBACKS_ARRAY_DEFINE(_notify_cb,
   { ELM_NOTIFY_EVENT_BLOCK_CLICKED, _block_clicked_cb },
   { ELM_NOTIFY_EVENT_TIMEOUT, _timeout_cb },
/* TIZEN_ONLY(20160629) : add "show,finished" internal callback */
   { ELM_NOTIFY_EVENT_SHOW_FINISHED, _show_finished_cb },
/* END */
   { ELM_NOTIFY_EVENT_DISMISSED, _hide_effect_finished_cb }
);

static void  _on_content_del(void *data, Evas *e, Evas_Object *obj, void *event_info);

EOLIAN static void
_elm_popup_efl_ui_l10n_translation_update(Eo *obj EINA_UNUSED, Elm_Popup_Data *sd)
{
   Elm_Popup_Item_Data *it;
   Eina_List *l;

   EINA_LIST_FOREACH(sd->items, l, it)
      elm_wdg_item_translate(EO_OBJ(it));

   efl_ui_l10n_translation_update(efl_super(obj, MY_CLASS));
   efl_ui_l10n_translation_update(sd->main_layout);
}

static void
_visuals_set(Evas_Object *obj)
{
   ELM_POPUP_DATA_GET(obj, sd);

   if (!sd->title_text && !sd->title_icon)
     elm_layout_signal_emit(sd->main_layout, "elm,state,title_area,hidden", "elm");
   else
     elm_layout_signal_emit(sd->main_layout, "elm,state,title_area,visible", "elm");

   if (sd->action_area)
     {
        elm_layout_signal_emit(sd->main_layout, "elm,state,action_area,visible", "elm");
        evas_object_show(sd->action_area);
     }
   else
     elm_layout_signal_emit(sd->main_layout, "elm,state,action_area,hidden", "elm");

   edje_object_message_signal_process(elm_layout_edje_get(sd->main_layout));
}

static void
_block_clicked_cb(void *data, const Efl_Event *event EINA_UNUSED)
{
   efl_event_callback_legacy_call(data, ELM_POPUP_EVENT_BLOCK_CLICKED, NULL);
}

static void
_timeout_cb(void *data, const Efl_Event *event EINA_UNUSED)
{
   evas_object_hide(data);
   efl_event_callback_legacy_call(data, ELM_POPUP_EVENT_TIMEOUT, NULL);
}

/* TIZEN_ONLY(20160629) : add "show,finished" internal callback */
static void
_show_finished_cb(void *data, const Efl_Event *event EINA_UNUSED)
{
   efl_event_callback_legacy_call(data, ELM_POPUP_EVENT_SHOW_FINISHED, NULL);
}
/* END */

static void
_hide_effect_finished_cb(void *data, const Efl_Event *event EINA_UNUSED)
{
   efl_gfx_entity_visible_set(data, EINA_FALSE);
   efl_event_callback_legacy_call(data, ELM_POPUP_EVENT_DISMISSED, NULL);
}


static Evas_Object *
_access_object_get(const Evas_Object *obj, const char* part)
{
   Evas_Object *po, *ao, *o;
   ELM_POPUP_DATA_GET(obj, sd);

   o = elm_layout_edje_get(sd->main_layout);
   edje_object_freeze(o);
   po = (Evas_Object *)edje_object_part_object_get(o, part);
   edje_object_thaw(o);
   ao = evas_object_data_get(po, "_part_access_obj");

   return ao;
}

static void
_on_show(void *data EINA_UNUSED,
         Evas *e EINA_UNUSED,
         Evas_Object *obj,
         void *event_info EINA_UNUSED)
{
   //TIZEN_ONLY(20170919): Handle default label object
   _elm_win_default_label_obj_append(obj);
   //
   elm_object_focus_set(obj, EINA_TRUE);
}

static void
_scroller_size_calc(Evas_Object *obj)
{
   Evas_Coord h;
   Evas_Coord h_title = 0;
   Evas_Coord h_action_area = 0;
   const char *action_area_height;
   /* TIZEN_ONLY(20160624): add outside padding */
   const char *outside_pad;
   Evas_Coord h_pad = 0;
   /* END */
   /* TIZEN_ONLY(20170202): fix popup max height */
   Evas_Coord y, wy;
   /* END */

   ELM_POPUP_DATA_GET(obj, sd);
   /* TIZEN_ONLY(20160623):Apply popup compress mode UX */
   ELM_WIDGET_DATA_GET_OR_RETURN(obj, wd);
   /* END */

   if (!sd->scroll && !sd->items) return;

   sd->scr_size_recalc = EINA_FALSE;
   sd->max_sc_h = -1;
   /* TIZEN_ONLY(20170202): fix popup max height
      adjust notify's height in case height of popup's parent is smaller than window's
      and there is indicator below the popup
   evas_object_geometry_get(sd->notify, NULL, NULL, NULL, &h);
   */
   evas_object_geometry_get(sd->notify, NULL, &y, NULL, &h);

   evas_object_geometry_get(elm_widget_top_get(sd->notify), NULL, &wy, NULL, NULL);
   h += (y - wy);
   /* END */
   if (sd->title_text || sd->title_icon)
     edje_object_part_geometry_get(elm_layout_edje_get(sd->main_layout),
                                   "elm.bg.title", NULL, NULL, NULL, &h_title);
   if (sd->action_area)
     {
        action_area_height = edje_object_data_get(
            elm_layout_edje_get(sd->action_area), "action_area_height");
        if (action_area_height)
          h_action_area =
            (int)(atoi(action_area_height)
                  * elm_config_scale_get() * elm_object_scale_get(obj))
                  / edje_object_base_scale_get(elm_layout_edje_get(sd->action_area));
     }

   /* TIZEN_ONLY(20160624): add out side padding */
   outside_pad = edje_object_data_get(elm_layout_edje_get(sd->main_layout), "popup_outside_pad");

   if (outside_pad) h_pad = (int)(atoi(outside_pad)
                            * elm_config_scale_get()
                            * elm_object_scale_get(obj)
                            / edje_object_base_scale_get(elm_layout_edje_get(sd->main_layout)));
   /* END */

   /* TIZEN_ONLY(20160623):Apply popup compress mode UX
   sd->max_sc_h = h - (h_title + h_action_area);
   */
   /*
   if ((sd->dispmode == EVAS_DISPLAY_MODE_COMPRESS) &&
       ((wd->orient_mode == 90) || (wd->orient_mode == 270)))
      sd->max_sc_h = h - h_action_area;
   else
      sd->max_sc_h = h - (h_title + h_action_area + h_pad);
   */
   /* END */
}

/* TIZEN_ONLY(20160623):Apply popup compress mode UX */
static void
_on_obj_size_hints_changed(void *data EINA_UNUSED, Evas *e EINA_UNUSED,
                           Evas_Object *obj, void *event_info EINA_UNUSED)
{
   Evas_Display_Mode dispmode;

   ELM_POPUP_DATA_GET(obj, sd);

   dispmode = evas_object_size_hint_display_mode_get(obj);
   if (sd->dispmode == dispmode) return;

   sd->dispmode = dispmode;

   _scroller_size_calc(obj);
   elm_layout_sizing_eval(obj);
}
/* END */

static void
_size_hints_changed_cb(void *data,
                       Evas *e EINA_UNUSED,
                       Evas_Object *obj EINA_UNUSED,
                       void *event_info EINA_UNUSED)
{
   elm_layout_sizing_eval(data);
}

static void
_notify_resize_cb(void *data,
                  Evas *e EINA_UNUSED,
                  Evas_Object *obj EINA_UNUSED,
                  void *event_info EINA_UNUSED)
{
   Evas_Object *popup = data;

   ELM_POPUP_CHECK(popup);

   _scroller_size_calc(popup);
   elm_layout_sizing_eval(popup);
}

static void
_list_del(Elm_Popup_Data *sd)
{
   if (!sd->scr) return;

   evas_object_event_callback_del
     (sd->scr, EVAS_CALLBACK_CHANGED_SIZE_HINTS, _size_hints_changed_cb);

   ELM_SAFE_FREE(sd->tbl, evas_object_del);
   sd->scr = NULL;
   sd->box = NULL;
   sd->spacer = NULL;
}

static void
_items_remove(Elm_Popup_Data *sd)
{
   Elm_Popup_Item_Data *it;

   if (!sd->items) return;

   EINA_LIST_FREE(sd->items, it)
     efl_del(EO_OBJ(it));

   sd->items = NULL;
}

static void
_focus_changed_popup(void *data, const Efl_Event *ev)
{
   //mirror property
   efl_ui_focus_object_focus_set(data, efl_ui_focus_object_focus_get(ev->object));
}

EFL_CALLBACKS_ARRAY_DEFINE(composition_cb,
   { EFL_UI_FOCUS_OBJECT_EVENT_FOCUS_CHANGED, _focus_changed_popup },
)

EOLIAN static void
_elm_popup_efl_canvas_group_group_del(Eo *obj, Elm_Popup_Data *sd)
{
   unsigned int i;

   evas_object_event_callback_del_full(sd->parent, EVAS_CALLBACK_RESIZE, _parent_geom_cb, obj);
   evas_object_event_callback_del_full(sd->parent, EVAS_CALLBACK_MOVE, _parent_geom_cb, obj);
   evas_object_event_callback_del_full(sd->notify, EVAS_CALLBACK_RESIZE, _notify_resize_cb, obj);

   efl_event_callback_array_del(sd->notify, _notify_cb(), obj);
   evas_object_event_callback_del
     (sd->content, EVAS_CALLBACK_DEL, _on_content_del);
   evas_object_event_callback_del(obj, EVAS_CALLBACK_SHOW, _on_show);
   efl_event_callback_array_del(sd->notify, composition_cb(), obj);

   sd->last_button_number = 0;

   for (i = 0; i < ELM_POPUP_ACTION_BUTTON_MAX; i++)
     {
        if (sd->buttons[i])
          {
             evas_object_del(sd->buttons[i]->btn);
             ELM_SAFE_FREE(sd->buttons[i], free);
          }
     }
   if (sd->items)
     {
        _items_remove(sd);
        _list_del(sd);
     }

   // XXX? delete other objects? just to be sure.
   ELM_SAFE_FREE(sd->notify, evas_object_del);
   ELM_SAFE_FREE(sd->title_icon, evas_object_del);
   ELM_SAFE_FREE(sd->text_content_obj, evas_object_del);
   ELM_SAFE_FREE(sd->box, evas_object_del);
   ELM_SAFE_FREE(sd->tbl, evas_object_del);
   ELM_SAFE_FREE(sd->spacer, evas_object_del);
   ELM_SAFE_FREE(sd->scr, evas_object_del);
   ELM_SAFE_FREE(sd->content, evas_object_del);
   ELM_SAFE_FREE(sd->main_layout, evas_object_del);
   ELM_SAFE_FREE(sd->content_area, evas_object_del);
   ELM_SAFE_FREE(sd->action_area, evas_object_del);
   ELM_SAFE_FREE(sd->title_text, eina_stringshare_del);
   /* TIZEN_ONLY(20160922): add subtitle text */
   ELM_SAFE_FREE(sd->subtitle_text, eina_stringshare_del);
   /* END */

   efl_canvas_group_del(efl_super(obj, MY_CLASS));
}

static void
_mirrored_set(Evas_Object *obj,
              Eina_Bool rtl)
{
   ELM_POPUP_DATA_GET(obj, sd);

   elm_object_mirrored_set(sd->notify, rtl);
   elm_object_mirrored_set(sd->main_layout, rtl);
   if (sd->scr) elm_object_mirrored_set(sd->scr, rtl);
   if (sd->action_area) elm_object_mirrored_set(sd->action_area, rtl);
   if (sd->items)
     {
        Elm_Popup_Item_Data *it;
        Eina_List *l;

        EINA_LIST_FOREACH(sd->items, l, it)
           elm_object_mirrored_set(VIEW(it), rtl);
     }
}

//TIZEN ONLY(20150717): expose title as at-spi object
static char *
_access_info_cb(void *data, Evas_Object *obj EINA_UNUSED)
{
   Eina_Strbuf *buf;
   Elm_Popup_Data *priv = data;
   char *ret = NULL;
   if (priv->title_text)
     {
        buf = eina_strbuf_new();
        eina_strbuf_append_printf(buf, "%s, %s, %s", N_("Alert"), priv->title_text, N_("Title"));
        ret = eina_strbuf_string_steal(buf);
        eina_strbuf_free(buf);
        return strdup(ret);
     }
   return NULL;
}
//

//TIZEN_ONLY(20160822): When atspi mode is dynamically switched on/off,
//register/unregister access objects accordingly.
static void
_atspi_expose_title(Eo *obj, Eina_Bool is_atspi)
{
   Evas_Object *ao = NULL;
   ELM_POPUP_DATA_GET(obj, sd);

   if (sd->title_text && is_atspi)
     {
        ao = _access_object_get(obj, ACCESS_TITLE_PART);
        if (!ao)
          {
             ao = _elm_access_edje_object_part_object_register
                   (sd->main_layout, elm_layout_edje_get(sd->main_layout), ACCESS_TITLE_PART);
             efl_access_object_role_set(ao, EFL_ACCESS_ROLE_HEADING);
             _elm_access_callback_set(_elm_access_info_get(ao),
                                       ELM_ACCESS_INFO, _access_info_cb, sd);
          }
     }
   else
     {
        ao = _access_object_get(obj, ACCESS_TITLE_PART);
        if (ao)
          _elm_access_object_unregister(ao, NULL);
     }
}
//

//TIZEN_ONLY(20170314): expose part-text as at-spi object
static char *
_access_name_set_cb(void *data, Evas_Object *obj EINA_UNUSED)
{
   char *text = NULL;
   if (data) text = _elm_util_mkup_to_text((char*)data);
   if (text) return text;

   return NULL;
}

static void
_atspi_part_text_expose(Eo *obj, const char *part, Eina_Bool is_atspi)
{
   Evas_Object *ao = NULL;
   ELM_POPUP_DATA_GET(obj, sd);
   const char *text = NULL;
   text = elm_layout_text_get(sd->main_layout, part);

   if (text && is_atspi)
     {
        ao = _access_object_get(obj, part);
        if (!ao)
          {
             ao = _elm_access_edje_object_part_object_register
                   (sd->main_layout, elm_layout_edje_get(sd->main_layout), part);
             efl_access_object_role_set(ao, EFL_ACCESS_ROLE_LABEL);
             efl_access_object_name_cb_set(ao, _access_name_set_cb, text);
          }
     }
   else
     {
        ao = _access_object_get(obj, part);
        if (ao)
          _elm_access_object_unregister(ao, NULL);
     }
}
//


static void
_access_obj_process(Eo *obj, Eina_Bool is_access)
{
   Evas_Object *ao;

   ELM_POPUP_DATA_GET(obj, sd);

   if (is_access)
     {
        if (sd->title_text)
          {
             ao = _elm_access_edje_object_part_object_register
                    (obj, elm_layout_edje_get(sd->main_layout), ACCESS_TITLE_PART);
             _elm_access_text_set(_elm_access_info_get(ao),
                                  ELM_ACCESS_TYPE, E_("Popup Title"));
             _elm_access_text_set(_elm_access_info_get(ao),
                                  ELM_ACCESS_INFO, sd->title_text);
          }

        if (sd->text_content_obj)
          {
             ao = _elm_access_edje_object_part_object_register
                    (obj, elm_layout_edje_get(sd->main_layout), ACCESS_BODY_PART);
             _elm_access_text_set(_elm_access_info_get(ao),
                                  ELM_ACCESS_TYPE, E_("Popup Body Text"));
             _elm_access_text_set(_elm_access_info_get(ao),
               ELM_ACCESS_INFO, elm_object_text_get(sd->text_content_obj));
          }
     }
   else
     {
        if (sd->title_text)
          {
             _elm_access_edje_object_part_object_unregister
                    (obj, elm_layout_edje_get(sd->main_layout), ACCESS_TITLE_PART);
          }

        if (sd->text_content_obj)
          {
             _elm_access_edje_object_part_object_unregister
                    (obj, elm_layout_edje_get(sd->main_layout), ACCESS_BODY_PART);
          }
     }
}

static void
_populate_theme_scroll(Elm_Popup_Data *sd)
{
   if (sd->content_area)
     {
        const char *content_area_width =
          edje_object_data_get(elm_layout_edje_get(sd->content_area),
                               "scroller_enable");
        if ((content_area_width) && (!strcmp(content_area_width, "on")))
          {
             sd->theme_scroll = EINA_TRUE;
             return;
          }
     }
   sd->theme_scroll = EINA_FALSE;
}

EOLIAN static Efl_Ui_Theme_Apply_Error
_elm_popup_efl_ui_widget_theme_apply(Eo *obj, Elm_Popup_Data *sd)
{
   Elm_Popup_Item_Data *it;
   Eina_List *elist;
   char buf[1024], style[1024];
   /* TIZEN_ONLY(20161109): check theme compatibility more precise */
   int i;
   /* END */

   _mirrored_set(obj, efl_ui_mirrored_get(obj));

   snprintf(style, sizeof(style), "popup/%s", elm_widget_style_get(obj));
   /* TIZEN_ONLY(20161109): check theme compatibility more precise
   elm_widget_style_set(sd->notify, style);
    */
     {
        Efl_Ui_Theme_Apply_Result ret;
        const char *obj_style = elm_widget_style_get(obj);

        if (obj_style && !strcmp(obj_style, "default"))
          ret = _notify_style_set(sd->notify, "popup");
        else
          ret = _notify_style_set(sd->notify, obj_style);

        if (ret != EFL_UI_THEME_APPLY_RESULT_SUCCESS)
          if (_notify_style_set(sd->notify, style) != EFL_UI_THEME_APPLY_RESULT_SUCCESS)
            if (_notify_style_set(sd->notify, "popup") != EFL_UI_THEME_APPLY_RESULT_SUCCESS)
              _notify_style_set(sd->notify, "popup/default");
     }
   /* END */

   if (!elm_layout_theme_set(sd->main_layout, "popup", "base",
                             elm_widget_style_get(obj)))
     CRI("Failed to set layout!");

   if (sd->action_area)
     {
        /* TIZEN_ONLY(20161109): check theme compatibility more precise
        snprintf(buf, sizeof(buf), "buttons%i", sd->last_button_number);
        if (!elm_layout_theme_set(sd->action_area, "popup", buf, style))
          CRI("Failed to set layout!");
         */
        snprintf(buf, sizeof(buf), "buttons%i", sd->last_button_number);
        for (i = 0; i < STYLE_PRIORITY_COUNT; i++)
          {
             snprintf(style, sizeof(style), STYLE_PRIORITY[i], elm_widget_style_get(obj));
             if (efl_ui_layout_theme_set(sd->action_area, "popup", buf, style) == EFL_UI_THEME_APPLY_RESULT_SUCCESS)
               break;
          }
        if (i == STYLE_PRIORITY_COUNT)
          CRI("Failed to set layout!");
        /* END */
     }
   /* TIZEN_ONLY(20161109): check theme compatibility more precise
   if (!elm_layout_theme_set(sd->content_area, "popup", "content", style))
     CRI("Failed to set layout!");
    */
   for (i = 0; i < STYLE_PRIORITY_COUNT; i++)
     {
        snprintf(style, sizeof(style), STYLE_PRIORITY[i], elm_widget_style_get(obj));
        if (efl_ui_layout_theme_set(sd->content_area, "popup", "content", style) == EFL_UI_THEME_APPLY_RESULT_SUCCESS)
          break;
     }
   if (i == STYLE_PRIORITY_COUNT)
     CRI("Failed to set layout!");
   snprintf(style, sizeof(style), "popup/%s", elm_widget_style_get(obj));
   /* END */
   if (sd->text_content_obj)
     {
        /* TIZEN_ONLY(20161109): check theme compatibility more precise
        elm_object_style_set(sd->text_content_obj, style);
        */
        if (elm_widget_style_set(sd->text_content_obj, style) != EFL_UI_THEME_APPLY_RESULT_SUCCESS)
          elm_widget_style_set(sd->text_content_obj, "popup/default");
        /* END */
     }
   else if (sd->items)
     {
        EINA_LIST_FOREACH(sd->items, elist, it)
          {
             if (!elm_layout_theme_set(VIEW(it), "popup", "item", style))
               CRI("Failed to set layout!");
             else
               {
                  if (it->label)
                    {
                       elm_layout_text_set(VIEW(it), "elm.text", it->label);
                       elm_layout_signal_emit(VIEW(it),
                                              "elm,state,item,text,visible",
                                              "elm");
                    }
                  if (it->icon)
                    elm_layout_signal_emit(VIEW(it),
                                           "elm,state,item,icon,visible",
                                           "elm");
                  if (it->disabled)
                    elm_layout_signal_emit(VIEW(it),
                                           "elm,state,item,disabled", "elm");
                  evas_object_show(VIEW(it));
                  edje_object_message_signal_process(
                     elm_layout_edje_get(VIEW(it)));
               }
          }
     }
   if (sd->title_text)
     {
        elm_layout_text_set(sd->main_layout, "elm.text.title", sd->title_text);
        elm_layout_signal_emit(sd->main_layout, "elm,state,title,text,visible", "elm");
     }
   if (sd->title_icon)
     elm_layout_signal_emit(sd->main_layout, "elm,state,title,icon,visible", "elm");
   /* TIZEN_ONLY(20160922): add subtitle text */
   if (sd->subtitle_text)
     {
        elm_layout_text_set(sd->main_layout, "elm,text,subtitle", sd->subtitle_text);
        elm_layout_signal_emit(sd->main_layout, "elm,state,subtitle,text,visible", "elm");
     }
   /* END */

   _populate_theme_scroll(sd);
   if (!sd->scroll && sd->theme_scroll)
     elm_layout_signal_emit(sd->content_area, "elm,scroll,disable", "elm");
   else if (sd->scroll && sd->theme_scroll)
     elm_layout_signal_emit(sd->content_area, "elm,scroll,enable", "elm");

   _visuals_set(obj);
   elm_layout_sizing_eval(obj);

   /* access */
   if (_elm_config->access_mode) _access_obj_process(obj, EINA_TRUE);

   return EFL_UI_THEME_APPLY_ERROR_NONE;
}

static void
_item_sizing_eval(Elm_Popup_Item_Data *it)
{
   Evas_Coord min_w = -1, min_h = -1, max_w = -1, max_h = -1;
   Evas_Object *edje = elm_layout_edje_get(VIEW(it));

   edje_object_size_min_restricted_calc
     (edje, &min_w, &min_h, min_w, min_h);
   evas_object_size_hint_min_set(edje, min_w, min_h);
   evas_object_size_hint_max_set(edje, max_w, max_h);
}

EOLIAN static void
_elm_popup_elm_layout_sizing_eval(Eo *obj, Elm_Popup_Data *sd)
{
   Eina_List *elist;
   Elm_Popup_Item_Data *it;
   Evas_Coord h_box = 0, minh_box = 0;
   Evas_Coord minw = -1, minh = -1;

   /* TIZEN_ONLY(20160624): add a allow eval flag not to call another sizing eval during sizing eval */
   if (!sd->allow_eval) return;

   sd->allow_eval = EINA_FALSE;
   /* END */

   _scroller_size_calc(obj);

   if (sd->items)
     {
        EINA_LIST_FOREACH(sd->items, elist, it)
          {
             _item_sizing_eval(it);
             evas_object_size_hint_combined_min_get(elm_layout_edje_get(VIEW(it)),
                                           NULL, &minh_box);
             if (minh_box != -1) h_box += minh_box;
          }
        evas_object_size_hint_min_set(sd->spacer, 0, MIN(h_box, sd->max_sc_h));
        evas_object_size_hint_max_set(sd->spacer, -1, sd->max_sc_h);

        evas_object_size_hint_combined_min_get(sd->scr, &minw, &minh);
        evas_object_size_hint_max_get(sd->scr, &minw, &minh);
     }
   else if (sd->scroll)
     {
        double horizontal, vertical;
        Evas_Coord w, h;
        /* TIZEN_ONLY(20160405): fix popup size problem in landscape mode */
        int rotation = -1;
        int w_content_area = 9999;
        const char *str;
        /* END */

        edje_object_message_signal_process(elm_layout_edje_get(sd->content_area));

        elm_popup_align_get(obj, &horizontal, &vertical);
        evas_object_geometry_get(sd->parent, NULL, NULL, &w, &h);

        /* TIZEN_ONLY(20161117): fix content area height problem when vertical align fill is set */
        if (sd->max_sc_h != -1)
          h = sd->max_sc_h;
        /* END */

        /* TIZEN_ONLY(20160405): fix popup size problem in landscape mode */
        str = edje_object_data_get(elm_layout_edje_get(sd->content_area), "content_area_width");
        if (str) w_content_area = (int)(atoi(str)
                                    * elm_config_scale_get()
                                    * elm_object_scale_get(obj)
                                    / edje_object_base_scale_get(elm_layout_edje_get(sd->content_area)));
        /* END */

        if (EINA_DBL_EQ(horizontal, ELM_NOTIFY_ALIGN_FILL))
          minw = w;
        if (EINA_DBL_EQ(vertical, ELM_NOTIFY_ALIGN_FILL))
          minh = h;

        /* TIZEN_ONLY(20160405): fix popup size problem in landscape mode */
        rotation = elm_win_rotation_get(elm_widget_top_get(elm_widget_parent_get(sd->notify)));
        if ((rotation == 90 || rotation == 270) && (horizontal == ELM_NOTIFY_ALIGN_FILL))
          minw = w_content_area;
        /* END */

        edje_object_size_min_restricted_calc(elm_layout_edje_get(sd->content_area),
                                             &minw, &minh, minw, minh);

        evas_object_size_hint_min_set(sd->content_area, minw, minh);

        /* TIZEN_ONLY(20161107): supprot scrollable content */
        //FIXME: genlist only now, it should be changed to support other scrollable.
        if (sd->scrollable_content)
          minh = sd->min_scrollable_content_h;
        /* END */

        if (minh > sd->max_sc_h)
          evas_object_size_hint_min_set(sd->spacer, minw, sd->max_sc_h);
        else
          evas_object_size_hint_min_set(sd->spacer, minw, minh);

        /* TIZEN_ONLY(20160624): add a allow eval flag not to call another sizing eval during sizing eval */
        sd->allow_eval = EINA_TRUE;
        /* END */

       return;
     }

   if (sd->main_layout)
     {
        edje_object_size_min_calc(elm_layout_edje_get(sd->main_layout), &minw, &minh);
        evas_object_size_hint_min_set(obj, minw, minh);
        evas_object_size_hint_max_set(obj, -1, -1);
     }

   /* TIZEN_ONLY(20160624): add a allow eval flag not to call another sizing eval during sizing eval */
   sd->allow_eval = EINA_TRUE;
   /* END */
}

EOLIAN static void
_elm_popup_efl_layout_signal_signal_emit(Eo *obj EINA_UNUSED, Elm_Popup_Data *sd, const char *emission, const char *source)
{
   elm_layout_signal_emit(sd->main_layout, emission, source);
}

EOLIAN static Eina_Bool
_elm_popup_efl_ui_widget_widget_sub_object_del(Eo *obj, Elm_Popup_Data *sd, Evas_Object *sobj)
{
   Elm_Popup_Item_Data *it;
   Eina_Bool int_ret = EINA_FALSE;

   int_ret = elm_widget_sub_object_del(efl_super(obj, MY_CLASS), sobj);
   if (!int_ret) return EINA_FALSE;

   if (sobj == sd->title_icon)
     {
        elm_layout_signal_emit(sd->main_layout, "elm,state,title,icon,hidden", "elm");
     }
   else if ((it =
               evas_object_data_get(sobj, "_popup_icon_parent_item")) != NULL)
     {
        if (sobj == it->icon)
          {
             efl_content_unset(efl_part(VIEW(it), CONTENT_PART));
             elm_layout_signal_emit(VIEW(it),
                                    "elm,state,item,icon,hidden", "elm");
             it->icon = NULL;
          }
     }

   return EINA_TRUE;
}

static void
_on_content_del(void *data,
                Evas *e EINA_UNUSED,
                Evas_Object *obj EINA_UNUSED,
                void *event_info EINA_UNUSED)
{
   ELM_POPUP_DATA_GET(data, sd);

   sd->content = NULL;
   elm_layout_sizing_eval(data);
}

static void
_on_text_content_del(void *data,
                     Evas *e EINA_UNUSED,
                     Evas_Object *obj EINA_UNUSED,
                     void *event_info EINA_UNUSED)
{
   ELM_POPUP_DATA_GET(data, sd);

   sd->text_content_obj = NULL;
   elm_layout_sizing_eval(data);
}

static void
_on_table_del(void *data,
              Evas *e EINA_UNUSED,
              Evas_Object *obj EINA_UNUSED,
              void *event_info EINA_UNUSED)
{
   ELM_POPUP_DATA_GET(data, sd);

   sd->tbl = NULL;
   sd->spacer = NULL;
   sd->scr = NULL;
   sd->box = NULL;
   elm_layout_sizing_eval(data);
}

static void
_on_button_del(void *data,
               Evas *e EINA_UNUSED,
               Evas_Object *obj,
               void *event_info EINA_UNUSED)
{
   int i;

   ELM_POPUP_DATA_GET(data, sd);

   for (i = 0; i < ELM_POPUP_ACTION_BUTTON_MAX; i++)
     {
        if (sd->buttons[i] && obj == sd->buttons[i]->btn &&
            sd->buttons[i]->delete_me == EINA_TRUE)
          {
             _button_remove(data, i, EINA_FALSE);
             break;
          }
     }
}

static void
_button_remove(Evas_Object *obj,
               int pos,
               Eina_Bool delete)
{
   int i = 0;
   char buf[128];

   ELM_POPUP_DATA_GET(obj, sd);

   if (!sd->last_button_number) return;

   if (!sd->buttons[pos]) return;

   if (delete)
     {
        evas_object_del(sd->buttons[pos]->btn);
     }
   else
     {
        evas_object_event_callback_del
          (sd->buttons[pos]->btn, EVAS_CALLBACK_DEL, _on_button_del);
        /* TIZEN_ONLY(20160318): Support legacy group and swallow part names
        snprintf(buf, sizeof(buf), "elm.swallow.content.button%i", pos + 1);
        elm_object_part_content_unset(sd->action_area, buf);
         */
        snprintf(buf, sizeof(buf), "actionbtn%i", pos + 1);
        if (edje_object_part_exists(elm_layout_edje_get(sd->action_area), buf))
          {
             elm_object_part_content_unset(sd->action_area, buf);
          }
        else
          {
             snprintf(buf, sizeof(buf), "elm.swallow.content.button%i", pos + 1);
             elm_object_part_content_unset(sd->action_area, buf);
          }
        /* END */
     }

   ELM_SAFE_FREE(sd->buttons[pos], free);

   sd->last_button_number = 0;

   for (i = ELM_POPUP_ACTION_BUTTON_MAX - 1; i >= 0; i--)
     {
        if (sd->buttons[i])
          {
             sd->last_button_number = i + 1;
             break;
          }
     }

   if (!sd->last_button_number)
     {
        ELM_SAFE_FREE(sd->action_area, evas_object_del);
        _visuals_set(obj);
     }
   else
     {
        /* TIZEN_ONLY(20161109): check theme compatibility more precise
        char style[1024];

        snprintf(style, sizeof(style), "popup/%s", elm_widget_style_get(obj));
        snprintf(buf, sizeof(buf), "buttons%i", sd->last_button_number);
        if (!elm_layout_theme_set(sd->action_area, "popup", buf, style))
          CRI("Failed to set layout!");
         */
        char style[1024];

        snprintf(buf, sizeof(buf), "buttons%i", sd->last_button_number);
        for (i = 0 ; i < STYLE_PRIORITY_COUNT; i++)
          {
             snprintf(style, sizeof(style), STYLE_PRIORITY[i], elm_widget_style_get(obj));
             if (efl_ui_layout_theme_set(sd->action_area, "popup", buf, style) == EFL_UI_THEME_APPLY_RESULT_SUCCESS)
               break;
          }
        if (i == STYLE_PRIORITY_COUNT)
          CRI("Failed to set layout!");
        /* END */
     }
}

static void
_layout_change_cb(void *data EINA_UNUSED,
                  Evas_Object *obj,
                  const char *emission EINA_UNUSED,
                  const char *source EINA_UNUSED)
{
   elm_layout_sizing_eval(obj);
}

static void
_create_scroller(Evas_Object *obj)
{
   char style[1024];

   ELM_POPUP_DATA_GET(obj, sd);

   //table
   sd->tbl = elm_table_add(sd->main_layout);
   evas_object_event_callback_add(sd->tbl, EVAS_CALLBACK_DEL,
                                  _on_table_del, obj);
   if (!sd->scroll)
     {
        if (sd->content || sd->text_content_obj) efl_content_unset(efl_part(sd->content_area, CONTENT_PART));
        efl_content_set(efl_part(sd->content_area, CONTENT_PART), sd->tbl);
        efl_content_set(efl_part(sd->main_layout, CONTENT_PART), sd->content_area);
     }

   //spacer
   sd->spacer = evas_object_rectangle_add(evas_object_evas_get(obj));
   evas_object_color_set(sd->spacer, 0, 0, 0, 0);
   elm_table_pack(sd->tbl, sd->spacer, 0, 0, 1, 1);

   //Scroller
   sd->scr = elm_scroller_add(sd->tbl);

   /***********************************************************************************
    * TIZEN_ONLY_FEATURE: apply Tizen's color_class features.                         *
    ***********************************************************************************/
   _elm_widget_color_class_parent_set(sd->scr, obj);
   /*******
    * END *
    *******/

   /* TIZEN_ONLY(20160318): Support legacy group and swallow part names
   if (!sd->scroll)
     {
        snprintf(style, sizeof(style), "popup/%s", elm_widget_style_get(obj));
        elm_object_style_set(sd->scr, style);
     }
   else
     elm_object_style_set(sd->scr, "popup/no_inset_shadow");
    */
   if (!elm_layout_theme_set(sd->scr, "scroller", "base", "effect"))
     {
        if (!sd->scroll)
          {
             snprintf(style, sizeof(style), "popup/%s", elm_widget_style_get(obj));
             elm_object_style_set(sd->scr, style);
          }
        else
          elm_object_style_set(sd->scr, "popup/no_inset_shadow");
     }
   /* END */
   evas_object_size_hint_weight_set(sd->scr, EVAS_HINT_EXPAND, EVAS_HINT_EXPAND);
   evas_object_size_hint_align_set(sd->scr, EVAS_HINT_FILL, EVAS_HINT_FILL);
   elm_scroller_policy_set(sd->scr, ELM_SCROLLER_POLICY_OFF, ELM_SCROLLER_POLICY_AUTO);
   elm_scroller_content_min_limit(sd->scr, EINA_TRUE, EINA_FALSE);
   elm_scroller_bounce_set(sd->scr, EINA_FALSE, EINA_TRUE);
   evas_object_event_callback_add(sd->scr, EVAS_CALLBACK_CHANGED_SIZE_HINTS,
                                  _size_hints_changed_cb, obj);
   efl_ui_mirrored_automatic_set(sd->scr, EINA_FALSE);
   elm_object_mirrored_set(sd->scr, elm_object_mirrored_get(obj));
   elm_table_pack(sd->tbl, sd->scr, 0, 0, 1, 1);
   evas_object_show(sd->scr);
}

static void
_list_add(Evas_Object *obj)
{
   ELM_POPUP_DATA_GET(obj, sd);

   if (!sd->scr)
     _create_scroller(obj);
   //Box
   sd->box = elm_box_add(sd->scr);
   evas_object_size_hint_weight_set(sd->box, EVAS_HINT_EXPAND, 0.0);
   evas_object_size_hint_align_set(sd->box, EVAS_HINT_FILL, 0.0);
   elm_object_content_set(sd->scr, sd->box);
   evas_object_show(sd->box);
}

static void
_item_select_cb(void *data,
                Evas_Object *obj EINA_UNUSED,
                const char *emission EINA_UNUSED,
                const char *source EINA_UNUSED)
{
   Elm_Popup_Item_Data *it = data;

   if (!it || it->disabled) return;
   if (it->func)
     it->func((void *)WIDGET_ITEM_DATA_GET(EO_OBJ(it)), WIDGET(it), EO_OBJ(it));
}

static void
_item_text_set(Elm_Popup_Item_Data *it,
               const char *label)
{
   if (!eina_stringshare_replace(&it->label, label)) return;

   elm_layout_text_set(VIEW(it), "elm.text", label);

   if (it->label)
     elm_layout_signal_emit(VIEW(it),
                            "elm,state,item,text,visible", "elm");
   else
     elm_layout_signal_emit(VIEW(it),
                            "elm,state,item,text,hidden", "elm");

   edje_object_message_signal_process(elm_layout_edje_get(VIEW(it)));
}

EOLIAN static void
_elm_popup_item_elm_widget_item_part_text_set(Eo *eo_it EINA_UNUSED, Elm_Popup_Item_Data *it,
                    const char *part,
                    const char *label)
{
   ELM_POPUP_ITEM_CHECK_OR_RETURN(it);

   if ((!part) || (!strcmp(part, "default")))
     {
        _item_text_set(it, label);
        return;
     }

   WRN("The part name is invalid! : popup=%p", WIDGET(it));
}

EOLIAN static const char *
_elm_popup_item_elm_widget_item_part_text_get(const Eo *eo_it EINA_UNUSED, Elm_Popup_Item_Data *it,
                    const char *part)
{
   ELM_POPUP_ITEM_CHECK_OR_RETURN(it, NULL);

   if ((!part) || (!strcmp(part, "default")))
     return it->label;

   WRN("The part name is invalid! : popup=%p", WIDGET(it));

   return NULL;
}

static void
_item_icon_set(Elm_Popup_Item_Data *it,
               Evas_Object *icon)
{
   if (it->icon == icon) return;

   evas_object_del(it->icon);
   it->icon = icon;
   if (it->icon)
     {
        elm_widget_sub_object_add(WIDGET(it), it->icon);
        evas_object_data_set(it->icon, "_popup_icon_parent_item", it);
        efl_content_set(efl_part(VIEW(it), CONTENT_PART), it->icon);
        elm_layout_signal_emit(VIEW(it), "elm,state,item,icon,visible", "elm");
     }
   else
     elm_layout_signal_emit(VIEW(it), "elm,state,item,icon,hidden", "elm");

   edje_object_message_signal_process(elm_layout_edje_get(VIEW(it)));
}

EOLIAN static void
_elm_popup_item_elm_widget_item_part_content_set(Eo *eo_it EINA_UNUSED, Elm_Popup_Item_Data *it,
                       const char *part,
                       Evas_Object *content)
{
   ELM_POPUP_ITEM_CHECK_OR_RETURN(it);

   if ((!(part)) || (!strcmp(part, "default")))
     _item_icon_set(it, content);
   else
     WRN("The part name is invalid! : popup=%p", WIDGET(it));
}

EOLIAN static Evas_Object *
_elm_popup_item_elm_widget_item_part_content_get(const Eo *eo_it EINA_UNUSED, Elm_Popup_Item_Data *it,
                       const char *part)
{
   ELM_POPUP_ITEM_CHECK_OR_RETURN(it, NULL);

   if ((!(part)) || (!strcmp(part, "default")))
     return it->icon;

   WRN("The part name is invalid! : popup=%p", WIDGET(it));

   return NULL;
}

static Evas_Object *
_item_icon_unset(Elm_Popup_Item_Data *it)
{
   Evas_Object *icon = it->icon;

   if (!it->icon) return NULL;
   _elm_widget_sub_object_redirect_to_top(WIDGET(it), icon);
   evas_object_data_del(icon, "_popup_icon_parent_item");
   efl_content_unset(efl_part(VIEW(it), CONTENT_PART));
   elm_layout_signal_emit(VIEW(it), "elm,state,item,icon,hidden", "elm");
   it->icon = NULL;

   return icon;
}

EOLIAN static Evas_Object *
_elm_popup_item_elm_widget_item_part_content_unset(Eo *eo_it EINA_UNUSED, Elm_Popup_Item_Data *it,
                         const char *part)
{
   Evas_Object *content = NULL;

   ELM_POPUP_ITEM_CHECK_OR_RETURN(it, NULL);

   if ((!(part)) || (!strcmp(part, "default")))
     content = _item_icon_unset(it);
   else
     WRN("The part name is invalid! : popup=%p", WIDGET(it));

   return content;
}

EOLIAN static void
_elm_popup_item_elm_widget_item_disable(Eo *eo_it, Elm_Popup_Item_Data *it)
{
   ELM_POPUP_ITEM_CHECK_OR_RETURN(it);

   if (elm_wdg_item_disabled_get(eo_it))
     elm_layout_signal_emit(VIEW(it), "elm,state,item,disabled", "elm");
   else
     elm_layout_signal_emit(VIEW(it), "elm,state,item,enabled", "elm");
}

EOLIAN static void
_elm_popup_item_efl_object_destructor(Eo *eo_it, Elm_Popup_Item_Data *it)
{
   ELM_POPUP_ITEM_CHECK_OR_RETURN(it);
   ELM_POPUP_DATA_GET(WIDGET(it), sd);

   evas_object_del(it->icon);
   eina_stringshare_del(it->label);
   sd->items = eina_list_remove(sd->items, it);
   if (!eina_list_count(sd->items))
     {
        sd->items = NULL;
        _list_del(sd);
     }
   efl_destructor(efl_super(eo_it, ELM_POPUP_ITEM_CLASS));
}

EOLIAN static void
_elm_popup_item_elm_widget_item_signal_emit(Eo *eo_it EINA_UNUSED, Elm_Popup_Item_Data *it,
                       const char *emission,
                       const char *source)
{
   elm_layout_signal_emit(VIEW(it), emission, source);
}

static void
_item_focus_change(void *data, const Efl_Event *event EINA_UNUSED)
{
   Elm_Popup_Item_Data *it = data;

   if (efl_ui_focus_object_focus_get(event->object))
     {
        efl_event_callback_legacy_call(WIDGET(it), ELM_POPUP_EVENT_ITEM_FOCUSED, EO_OBJ(it));
     }
   else
     {
        efl_event_callback_legacy_call(WIDGET(it), ELM_POPUP_EVENT_ITEM_UNFOCUSED, EO_OBJ(it));
     }
}

/* TIZEN_ONLY(20161107): support scrollable content */
//FIXME: genlist only now, it should be changed to support other scrollable.
static void
_scrollable_content_loaded_cb(void *data, Evas_Object *obj, void *event_info EINA_UNUSED)
{
   ELM_POPUP_DATA_GET(data, sd);

   Evas_Coord h;
   elm_interface_scrollable_content_size_get(obj, NULL, &h);

   sd->scrollable_content = EINA_TRUE;
   sd->min_scrollable_content_h = h;

   elm_layout_sizing_eval(data);
}
/* END */

static void
_item_focused_cb(void *data, Evas_Object *obj EINA_UNUSED, void *event_info EINA_UNUSED)
{
   Elm_Popup_Item_Data *it = data;

   efl_event_callback_legacy_call(WIDGET(it), ELM_POPUP_EVENT_ITEM_FOCUSED, EO_OBJ(it));
}

static void
_item_unfocused_cb(void *data, Evas_Object *obj EINA_UNUSED, void *event_info EINA_UNUSED)
{
   Elm_Popup_Item_Data *it = data;

   efl_event_callback_legacy_call(WIDGET(it), ELM_POPUP_EVENT_ITEM_UNFOCUSED, EO_OBJ(it));
}

EOLIAN static Eo *
_elm_popup_item_efl_object_constructor(Eo *eo_it, Elm_Popup_Item_Data *it)
{
   eo_it = efl_constructor(efl_super(eo_it, ELM_POPUP_ITEM_CLASS));
   it->base = efl_data_scope_get(eo_it, ELM_WIDGET_ITEM_CLASS);

   return eo_it;
}

static void
_item_new(Elm_Popup_Item_Data *it)
{
   char style[1024];

   VIEW_SET(it, elm_layout_add(WIDGET(it)));
   elm_object_focus_allow_set(VIEW(it), EINA_TRUE);
   efl_ui_mirrored_automatic_set(VIEW(it), EINA_FALSE);
   elm_object_mirrored_set(VIEW(it), elm_object_mirrored_get(WIDGET(it)));

   snprintf(style, sizeof(style), "popup/%s", elm_widget_style_get(WIDGET(it)));
   if (!elm_layout_theme_set(VIEW(it), "popup", "item", style))
     CRI("Failed to set layout!");
   else
     {
        elm_layout_signal_callback_add(VIEW(it), "elm,action,click", "*",
                                       _item_select_cb, it);
        evas_object_size_hint_align_set(VIEW(it), EVAS_HINT_FILL, EVAS_HINT_FILL);

        if (elm_widget_is_legacy(WIDGET(it)))
          {
             evas_object_smart_callback_add
                (VIEW(it), "focused", _item_focused_cb, it);
             evas_object_smart_callback_add
                (VIEW(it), "unfocused", _item_unfocused_cb, it);
          }
        else
          efl_event_callback_add
              (VIEW(it), EFL_UI_FOCUS_OBJECT_EVENT_FOCUS_CHANGED, _item_focus_change, it);
        evas_object_show(VIEW(it));
     }
}

static Eina_Bool
_title_text_set(Evas_Object *obj,
                const char *text)
{
   Evas_Object *ao;
   Eina_Bool title_visibility_old, title_visibility_current;

   ELM_POPUP_DATA_GET(obj, sd);

   if (sd->title_text == text) return EINA_TRUE;

   title_visibility_old = (sd->title_text) || (sd->title_icon);
   eina_stringshare_replace(&sd->title_text, text);

   elm_layout_text_set(sd->main_layout, "elm.text.title", text);

   /* access */
   if (_elm_config->access_mode)
     {
        ao = _access_object_get(obj, ACCESS_TITLE_PART);
        if (!ao)
          {
             ao = _elm_access_edje_object_part_object_register
                    (obj, elm_layout_edje_get(sd->main_layout), ACCESS_TITLE_PART);
             _elm_access_text_set(_elm_access_info_get(ao),
                                  ELM_ACCESS_TYPE, E_("Popup Title"));
          }
        _elm_access_text_set(_elm_access_info_get(ao), ELM_ACCESS_INFO, text);
     }

   //TIZEN ONLY(20150717): expose title as at-spi object
   // TIZEN_ONLY(20170516): connect to at-spi dbus based on org.a11y.Status.IsEnabled property
   if (elm_atspi_bridge_utils_is_screen_reader_enabled())
     _atspi_expose_title(obj, EINA_TRUE);
   //
   //

   if (sd->title_text)
     elm_layout_signal_emit(sd->main_layout, "elm,state,title,text,visible", "elm");
   else
     elm_layout_signal_emit(sd->main_layout, "elm,state,title,text,hidden", "elm");

   title_visibility_current = (sd->title_text) || (sd->title_icon);

   if (title_visibility_old != title_visibility_current)
     _visuals_set(obj);

   return EINA_TRUE;
}

/* TIZEN_ONLY(20160922): add subtitle text */
static Eina_Bool
_subtitle_text_set(Evas_Object *obj,
                const char *text)
{
   ELM_POPUP_DATA_GET(obj, sd);

   if (sd->subtitle_text == text) return EINA_TRUE;

   eina_stringshare_replace(&sd->subtitle_text, text);

   //bare edje here because we're inside the hook, already
   edje_object_part_text_escaped_set
     (elm_layout_edje_get(sd->main_layout), "elm.text.subtitle", text);

   if (sd->subtitle_text)
     elm_layout_signal_emit(sd->main_layout, "elm,state,subtitle,text,visible", "elm");
   else
     elm_layout_signal_emit(sd->main_layout, "elm,state,subtitle,text,hidden", "elm");

   edje_object_message_signal_process(elm_layout_edje_get(sd->main_layout));

   return EINA_TRUE;
}
/* END */

static Eina_Bool
_content_text_set(Evas_Object *obj,
                  const char *text)
{
   Evas_Object *ao;
   char style[1024];

   ELM_POPUP_DATA_GET(obj, sd);

   if (sd->items)
     {
        _items_remove(sd);
        _list_del(sd);
     }
   else
     {
        if (!sd->scroll)
          efl_content_set(efl_part(sd->main_layout, CONTENT_PART), sd->content_area);
        else
          elm_object_content_set(sd->scr, sd->content_area);
     }
   if (!text) goto end;

   if (sd->text_content_obj)
     {
        sd->text_content_obj = efl_content_unset(efl_part(sd->content_area, CONTENT_PART));
        evas_object_del(sd->text_content_obj);
        sd->text_content_obj = NULL;
     }

   sd->text_content_obj = elm_label_add(sd->content_area);
   snprintf(style, sizeof(style), "popup/%s", elm_widget_style_get(obj));
   /* TIZEN_ONLY(20161109): check theme compatibility more precise
   elm_object_style_set(sd->text_content_obj, style);
    */
   if (elm_widget_style_set(sd->text_content_obj, style) != EFL_UI_THEME_APPLY_RESULT_SUCCESS)
     elm_widget_style_set(sd->text_content_obj, "popup/default");
   /* END */
   /***********************************************************************************
    * TIZEN_ONLY_FEATURE: apply Tizen's color_class features.                         *
    ***********************************************************************************/
   _elm_widget_color_class_parent_set(sd->text_content_obj, obj);
   /*******
    * END *
    *******/

   evas_object_event_callback_add
     (sd->text_content_obj, EVAS_CALLBACK_DEL, _on_text_content_del, obj);

   elm_label_line_wrap_set(sd->text_content_obj, sd->content_text_wrap_type);
   elm_object_text_set(sd->text_content_obj, text);
   evas_object_size_hint_weight_set
     (sd->text_content_obj, EVAS_HINT_EXPAND, 0.0);
   evas_object_size_hint_align_set
     (sd->text_content_obj, EVAS_HINT_FILL, EVAS_HINT_FILL);
   efl_content_set
     (efl_part(sd->content_area, CONTENT_PART), sd->text_content_obj);

   /* access */
   if (_elm_config->access_mode)
     {
        /* unregister label, ACCESS_BODY_PART will register */
        elm_access_object_unregister(sd->text_content_obj);

        ao = _access_object_get(obj, ACCESS_BODY_PART);
        if (!ao)
          {
             ao = _elm_access_edje_object_part_object_register
                    (obj, elm_layout_edje_get(sd->main_layout), ACCESS_BODY_PART);
             _elm_access_text_set(_elm_access_info_get(ao),
                                  ELM_ACCESS_TYPE, E_("Popup Body Text"));
          }
        _elm_access_text_set(_elm_access_info_get(ao), ELM_ACCESS_INFO, text);
     }

   /* TIZEN_ONLY(20161107): support scrollable content */
   //FIXME: genlist only now, it should be changed to support other scrollable.
   sd->scrollable_content = EINA_FALSE;
   /* END */
end:
   return EINA_TRUE;
}

static Eina_Bool
_elm_popup_text_set(Eo *obj, Elm_Popup_Data *_pd, const char *part, const char *label)
{
   Eina_Bool int_ret = EINA_FALSE;

   if (!_elm_layout_part_aliasing_eval(obj, &part, EINA_TRUE))
      return EINA_FALSE;

   if (!strcmp(part, "elm.text"))
     {
        int_ret = _content_text_set(obj, label);
        //TIZEN_ONLY(20170314): expose part-text as at-spi object
        // TIZEN_ONLY(20170516): connect to at-spi dbus based on org.a11y.Status.IsEnabled property
        _atspi_part_text_expose(obj, part, elm_atspi_bridge_utils_is_screen_reader_enabled());
        //
        //
     }
   else if (!strcmp(part, "title,text"))
     int_ret = _title_text_set(obj, label);
   /* TIZEN_ONLY(20160922): add subtitle text */
   else if(!strcmp(part, "subtitle,text"))
     int_ret = _subtitle_text_set(obj, label);
   /* END */
   else
     int_ret = elm_layout_text_set(_pd->main_layout, part, label);

   elm_layout_sizing_eval(obj);

   return int_ret;
}

static const char *
_title_text_get(const Elm_Popup_Data *sd)
{
   return sd->title_text;
}

/* TIZEN_ONLY(20160922): add subtitle text */
static const char *
_subtitle_text_get(const Elm_Popup_Data *sd)
{
   return sd->subtitle_text;
}
/* END */

static const char *
_content_text_get(const Elm_Popup_Data *sd)
{
   const char *str = NULL;

   if (sd->text_content_obj)
     str = elm_object_text_get(sd->text_content_obj);

   return str;
}

static const char *
_elm_popup_text_get(Eo *obj EINA_UNUSED, Elm_Popup_Data *_pd, const char *part)
{
   const char *text = NULL;

   if (!_elm_layout_part_aliasing_eval(obj, &part, EINA_TRUE))
      return NULL;

   if (!strcmp(part, "elm.text"))
     text = _content_text_get(_pd);
   else if (!strcmp(part, "title,text"))
     text = _title_text_get(_pd);
   /* TIZEN_ONLY(20160922): add subtitle text */
   else if (!strcmp(part, "subtitle,text"))
     text = _subtitle_text_get(_pd);
   /* END */
   else
     text = elm_layout_text_get(_pd->main_layout, part);

   return text;
}

static Eina_Bool
_title_icon_set(Evas_Object *obj,
                Evas_Object *icon)
{
   Eina_Bool title_visibility_old, title_visibility_current;

   ELM_POPUP_DATA_GET(obj, sd);

   if (sd->title_icon == icon) return EINA_TRUE;
   title_visibility_old = (sd->title_text) || (sd->title_icon);
   evas_object_del(sd->title_icon);

   sd->title_icon = icon;
   title_visibility_current = (sd->title_text) || (sd->title_icon);

   efl_content_set
      (efl_part(sd->main_layout, "elm.swallow.title.icon"), sd->title_icon);

   if (sd->title_icon)
     elm_layout_signal_emit(sd->main_layout, "elm,state,title,icon,visible", "elm");
   if (title_visibility_old != title_visibility_current) _visuals_set(obj);

   return EINA_TRUE;
}

static Eina_Bool
_content_set(Evas_Object *obj,
             Evas_Object *content)
{
   ELM_POPUP_DATA_GET(obj, sd);

   /* TIZEN_ONLY(20161107): support scrollable content */
   //FIXME: genlist only now, it should be changed to support other scrollable.
   sd->scrollable_content = EINA_FALSE;
   /* END */

   if (sd->content && sd->content == content) return EINA_TRUE;
   if (sd->items)
     {
        _items_remove(sd);
        _list_del(sd);
     }

   evas_object_del(sd->content);
   sd->content = content;

   if (content)
     {
        if (!sd->scroll)
          efl_content_set(efl_part(sd->main_layout, CONTENT_PART), sd->content_area);
        else
          elm_object_content_set(sd->scr, sd->content_area);

        evas_object_show(content);
        efl_content_set(efl_part(sd->content_area, CONTENT_PART), content);

        /* TIZEN_ONLY(20161107): support scrollable content */
        //FIXME: genlist only now, it should be changed to support other scrollable.
        if (efl_isa(content, ELM_INTERFACE_SCROLLABLE_MIXIN))
          evas_object_smart_callback_add(content, "loaded", _scrollable_content_loaded_cb, obj);
        /* END */

        evas_object_event_callback_add
          (content, EVAS_CALLBACK_DEL, _on_content_del, obj);
     }

   return EINA_TRUE;
}

static void
_action_button_set(Evas_Object *obj,
                   Evas_Object *btn,
                   unsigned int idx)
{
   char buf[128], style[1024];
   unsigned int i;

   ELM_POPUP_DATA_GET(obj, sd);

   if (idx >= ELM_POPUP_ACTION_BUTTON_MAX) return;

   if (!btn)
     {
        _button_remove(obj, idx, EINA_TRUE);
        return;
     }

   if (sd->buttons[idx])
     {
        evas_object_del(sd->buttons[idx]->btn);
        free(sd->buttons[idx]);
     }

   sd->buttons[idx] = ELM_NEW(Action_Area_Data);
   sd->buttons[idx]->obj = obj;
   sd->buttons[idx]->btn = btn;

   evas_object_event_callback_add
     (btn, EVAS_CALLBACK_DEL, _on_button_del, obj);

   for (i = ELM_POPUP_ACTION_BUTTON_MAX - 1; i >= idx; i--)
     {
        if (sd->buttons[i])
          {
             sd->last_button_number = i + 1;
             break;
          }
     }

   if (!sd->action_area)
     {
        sd->action_area = elm_layout_add(sd->main_layout);
        evas_object_event_callback_add
          (sd->action_area, EVAS_CALLBACK_CHANGED_SIZE_HINTS,
           _size_hints_changed_cb, sd->main_layout);
        efl_ui_mirrored_automatic_set(sd->action_area, EINA_FALSE);
        elm_object_mirrored_set(sd->action_area, elm_object_mirrored_get(obj));
        efl_content_set(efl_part(sd->main_layout, "elm.swallow.action_area"), sd->action_area);

        _visuals_set(obj);

        /***********************************************************************************
         * TIZEN_ONLY_FEATURE: apply Tizen's color_class features.                         *
         ***********************************************************************************/
        _elm_widget_color_class_parent_set(sd->action_area, obj);
        /*******
         * END *
         *******/
     }

   /* TIZEN_ONLY(20161109): check theme compatibility more precise
   snprintf(buf, sizeof(buf), "buttons%i", sd->last_button_number);
   snprintf(style, sizeof(style), "popup/%s", elm_widget_style_get(obj));
   if (!elm_layout_theme_set(sd->action_area, "popup", buf, style))
     CRI("Failed to set layout!");
    */
   snprintf(buf, sizeof(buf), "buttons%i", sd->last_button_number);
   for (i = 0; i < STYLE_PRIORITY_COUNT; i++)
     {
        snprintf(style, sizeof(style), STYLE_PRIORITY[i], elm_widget_style_get(obj));
        if (efl_ui_layout_theme_set(sd->action_area, "popup", buf, style) == EFL_UI_THEME_APPLY_RESULT_SUCCESS)
          break;
     }
   if (i == STYLE_PRIORITY_COUNT)
     CRI("Failed to set layout!");
   /* END */

   /* TIZEN_ONLY(20161109): check theme compatibility more precise
   snprintf(buf, sizeof(buf), "elm.swallow.content.button%i", idx + 1);
   */
   evas_object_show(sd->buttons[idx]->btn);
   /* TIZEN_ONLY(20161109): check theme compatibility more precise
   elm_object_part_content_set
     (sd->action_area, buf, sd->buttons[idx]->btn);
    */
   snprintf(buf, sizeof(buf), "actionbtn%i", idx + 1);
   if (edje_object_part_exists(elm_layout_edje_get(sd->action_area), buf))
     {
        elm_object_part_content_set
           (sd->action_area, buf, sd->buttons[idx]->btn);
     }
   else
     {
        snprintf(buf, sizeof(buf), "elm.swallow.content.button%i", idx + 1);
        elm_object_part_content_set
           (sd->action_area, buf, sd->buttons[idx]->btn);
     }
   /* END */
}

static Eina_Bool
_elm_popup_content_set(Eo *obj, Elm_Popup_Data *_pd EINA_UNUSED, const char *part, Evas_Object *content)
{
   unsigned int i;
   Eina_Bool ret = EINA_TRUE;

   if (!part || !strcmp(part, "elm.swallow.content"))
     ret = _content_set(obj, content);
   else if (!strcmp(part, "title,icon"))
     ret = _title_icon_set(obj, content);
   else if (!strncmp(part, "button", 6))
     {
        i = atoi(part + 6) - 1;

        if (i >= ELM_POPUP_ACTION_BUTTON_MAX)
          {
             ERR("The part name is invalid! : popup=%p", obj);
             return EINA_FALSE;
          }

        _action_button_set(obj, content, i);
     }
   else
   /* TIZEN_ONLY(20161206): support tizen custom signal (tizen_2.3)
     ret = efl_content_set(efl_part(_pd->main_layout, part), content);
    */
     {
        if (strcmp("elm.swallow.icon", part) != 0 &&
            strcmp("elm.swallow.end", part) != 0)
          {
             const char *type;
             char buf[1024];
             if (strncmp(part, "elm.swallow.", sizeof("elm.swallow.") - 1) == 0)
               type = part + sizeof("elm.swallow.")-1;
             else
               type = part;

             snprintf(buf, sizeof(buf), "elm,state,tizen,%s,show", type);
             elm_layout_signal_emit(_pd->main_layout, buf, "elm");
          }

        ret = efl_content_set(efl_part(_pd->main_layout, part), content);
     }
   /* END */

   elm_layout_sizing_eval(obj);

   return ret;
}

static Evas_Object *
_title_icon_get(const Elm_Popup_Data *sd)
{
   return sd->title_icon;
}

static Evas_Object *
_content_get(const Elm_Popup_Data *sd)
{
   Evas_Object *ret;

   ret = sd->content ? sd->content : sd->text_content_obj;

   return ret;
}

static Evas_Object *
_action_button_get(const Evas_Object *obj,
                   unsigned int idx)
{
   Evas_Object *button = NULL;

   ELM_POPUP_DATA_GET(obj, sd);
   if (!sd->action_area) return NULL;

   if (sd->buttons[idx])
     button = sd->buttons[idx]->btn;

   return button;
}

static Evas_Object*
_elm_popup_content_get(Eo *obj, Elm_Popup_Data *_pd, const char *part)
{
   Evas_Object *content = NULL;
   unsigned int i;

   if (!part || !strcmp(part, "elm.swallow.content"))
     content = _content_get(_pd);
   else if (!strcmp(part, "title,text"))
     content = _title_icon_get(_pd);
   else if (!strncmp(part, "button", 6))
     {
        i = atoi(part + 6) - 1;

        if (i >= ELM_POPUP_ACTION_BUTTON_MAX)
          goto err;

        content = _action_button_get(obj, i);
     }
   else
      content = efl_content_get(efl_part(_pd->main_layout, part));

   if (!content)
     goto err;

   return content;

err:
   WRN("The part name is invalid! : popup=%p", obj);
   return NULL;
}

static Evas_Object *
_content_unset(Evas_Object *obj)
{
   Evas_Object *content;

   ELM_POPUP_DATA_GET(obj, sd);

   if (!sd->content) return NULL;

   evas_object_event_callback_del
     (sd->content, EVAS_CALLBACK_DEL, _on_content_del);

   content = efl_content_unset(efl_part(sd->content_area, CONTENT_PART));
   sd->content = NULL;

   /* TIZEN_ONLY(20161107): support scrollable content */
   //FIXME: genlist only now, it should be changed to support other scrollable.
   sd->scrollable_content = EINA_FALSE;
   /* END */

   elm_layout_sizing_eval(obj);

   return content;
}

static Evas_Object *
_title_icon_unset(Evas_Object *obj)
{
   Evas_Object *icon;

   ELM_POPUP_DATA_GET(obj, sd);

   if (!sd->title_icon) return NULL;

   icon = sd->title_icon;
   efl_content_unset(efl_part(sd->main_layout, "elm.swallow.title.icon"));
   sd->title_icon = NULL;

   return icon;
}

static Evas_Object*
_elm_popup_content_unset(Eo *obj, Elm_Popup_Data *_pd EINA_UNUSED, const char *part)
{
   Evas_Object *content = NULL;
   unsigned int i;

   if (!part || !strcmp(part, "elm.swallow.content"))
     content = _content_unset(obj);
   else if (!strcmp(part, "title,icon"))
     content = _title_icon_unset(obj);
   else if (!strncmp(part, "button", 6))
     {
        i = atoi(part + 6) - 1;

        if (i >= ELM_POPUP_ACTION_BUTTON_MAX)
          goto err;

        _button_remove(obj, i, EINA_FALSE);
     }
   else
   /* TIZEN_ONLY(20161206): support tizen custom signal (tizen_2.3)
     goto err;
    */
     {
        if (strcmp("elm.swallow.icon", part) != 0 &&
            strcmp("elm.swallow.end", part) != 0)
          {
             const char *type;
             char buf[1024];
             if (strncmp(part, "elm.swallow.", sizeof("elm.swallow.") - 1) == 0)
               type = part + sizeof("elm.swallow.")-1;
             else
               type = part;

             snprintf(buf, sizeof(buf), "elm,state,tizen,%s,hide", type);
             elm_layout_signal_emit(_pd->main_layout, buf, "elm");
          }

        content = efl_content_unset(efl_part(_pd->main_layout, part));
     }
   /* END */

   return content;

err:
   ERR("The part name is invalid! : popup=%p", obj);

   return NULL;
}

//TIZEN_ONLY(20180607): Restore legacy focus
EOLIAN static Eina_Bool
_elm_popup_efl_ui_widget_focus_next_manager_is(Eo *obj EINA_UNUSED, Elm_Popup_Data *_pd EINA_UNUSED)
{
   return EINA_TRUE;
}

EOLIAN static Eina_Bool
_elm_popup_efl_ui_widget_focus_next(Eo *obj, Elm_Popup_Data *sd, Elm_Focus_Direction dir, Evas_Object **next, Elm_Object_Item **next_item)
{
   Evas_Object *ao;
   Eina_List *items = NULL;
   Eina_Iterator *base_it;

   /* access */
   if (_elm_config->access_mode)
     {
        if (sd->title_text)
          {
             ao = _access_object_get(obj, ACCESS_TITLE_PART);
             items = eina_list_append(items, ao);
          }

        ao = _access_object_get(obj, ACCESS_BODY_PART);
        if (ao) items = eina_list_append(items, ao);
     }

   base_it = efl_content_iterate(sd->main_layout);
   EINA_ITERATOR_FOREACH(base_it, ao)
     if (ao) items = eina_list_append(items, ao);
   eina_iterator_free(base_it);

   if (!efl_ui_widget_focus_list_next_get(sd->main_layout, items, eina_list_data_get, dir, next, next_item))
     *next = obj;
   eina_list_free(items);

   return EINA_TRUE;
}

EOLIAN static Eina_Bool
_elm_popup_efl_ui_widget_focus_direction_manager_is(Eo *obj EINA_UNUSED, Elm_Popup_Data *_pd EINA_UNUSED)
{
   return EINA_TRUE;
}

EOLIAN static Eina_Bool
_elm_popup_efl_ui_widget_focus_direction(Eo *obj EINA_UNUSED, Elm_Popup_Data *sd, const Evas_Object *base, double degree, Evas_Object **direction, Elm_Object_Item **direction_item, double *weight)
{
   Evas_Object *ao;
   Eina_List *items = NULL;
   Eina_Iterator *base_it;

   /* access */
   if (_elm_config->access_mode)
     {
        if (sd->title_text)
          {
             ao = _access_object_get(obj, ACCESS_TITLE_PART);
             items = eina_list_append(items, ao);
          }

        ao = _access_object_get(obj, ACCESS_BODY_PART);
        if (ao) items = eina_list_append(items, ao);
     }

   base_it = efl_content_iterate(sd->main_layout);
   EINA_ITERATOR_FOREACH(base_it, ao)
     if (ao) items = eina_list_append(items, ao);
   eina_iterator_free(base_it);

   efl_ui_widget_focus_list_direction_get
     (sd->main_layout, base, items, eina_list_data_get, degree, direction, direction_item, weight);
   eina_list_free(items);

   return EINA_TRUE;
}

static Eina_Bool
_key_action_move(Evas_Object *obj, const char *params)
{
   const char *dir = params;

   _elm_widget_focus_auto_show(obj);
   if (!strcmp(dir, "previous"))
     efl_ui_widget_focus_cycle(obj, ELM_FOCUS_PREVIOUS);
   else if (!strcmp(dir, "next"))
     efl_ui_widget_focus_cycle(obj, ELM_FOCUS_NEXT);
   else if (!strcmp(dir, "left"))
     efl_ui_widget_focus_cycle(obj, ELM_FOCUS_LEFT);
   else if (!strcmp(dir, "right"))
     efl_ui_widget_focus_cycle(obj, ELM_FOCUS_RIGHT);
   else if (!strcmp(dir, "up"))
     efl_ui_widget_focus_cycle(obj, ELM_FOCUS_UP);
   else if (!strcmp(dir, "down"))
     efl_ui_widget_focus_cycle(obj, ELM_FOCUS_DOWN);
   else return EINA_FALSE;

   return EINA_TRUE;

}
//

static Eina_Bool
_key_action_escape(Evas_Object *obj, const char *params EINA_UNUSED)
{
//TIZEN_ONLY(20150709) : add keybinding for elm_popup "block,clicked" action
   evas_object_smart_callback_call(obj, SIG_BLOCK_CLICKED, NULL);
//
   ELM_POPUP_DATA_GET(obj, pd);
   elm_layout_signal_emit(pd->main_layout, "elm,state,hide", "elm");
   elm_notify_dismiss(pd->notify);
   return EINA_TRUE;
}

//TIZEN_ONLY(20160726): add API elm_object_part_access_object_get
EOLIAN static Evas_Object*
_elm_popup_efl_ui_widget_part_access_object_get(const Eo *obj, Elm_Popup_Data *_pd EINA_UNUSED, const char *part)
{
   return _access_object_get(obj, part);
}
//

EOLIAN static void
_elm_popup_efl_canvas_group_group_add(Eo *obj, Elm_Popup_Data *priv)
{
   char style[1024];
   /* TIZEN_ONLY(20161109): check theme compatibility more precise */
   int i;
   /* END */

   efl_canvas_group_add(efl_super(obj, MY_CLASS));
   elm_widget_sub_object_parent_add(obj);

   /* TIZEN_ONLY(20160624): add a allow eval flag not to call another sizing eval during sizing eval */
   priv->allow_eval = EINA_TRUE;
   /* END */
   /* TIZEN_ONLY(20161107): support scrollable content */
   //FIXME: genlist only now, it should be changed to support other scrollable.
   priv->scrollable_content = EINA_FALSE;
   /* END */

   snprintf(style, sizeof(style), "popup/%s", elm_widget_style_get(obj));

   priv->notify = elm_notify_add(obj);

   /***********************************************************************************
    * TIZEN_ONLY_FEATURE: apply Tizen's color_class features.                         *
    ***********************************************************************************/
   _elm_widget_color_class_parent_set(priv->notify, obj);
   /*******
    * END *
    *******/

   /* TIZEN_ONLY(20161109): check theme compatibility more precise */
   elm_notify_align_set(priv->notify,
                        _elm_config->popup_horizontal_align,
                        _elm_config->popup_vertical_align);
   /* END */

   /* TIZEN_ONLY(20161109): check theme compatibility more precise
   elm_object_style_set(priv->notify, style);
    */
     {
        Efl_Ui_Theme_Apply_Result ret;
        const char *obj_style = elm_widget_style_get(obj);

        if (obj_style && !strcmp(obj_style, "default"))
          ret = _notify_style_set(priv->notify, "popup");
        else
          ret = _notify_style_set(priv->notify, obj_style);

        if (ret != EFL_UI_THEME_APPLY_RESULT_SUCCESS)
          if (_notify_style_set(priv->notify, style) != EFL_UI_THEME_APPLY_RESULT_SUCCESS)
            if (_notify_style_set(priv->notify, "popup") != EFL_UI_THEME_APPLY_RESULT_SUCCESS)
              _notify_style_set(priv->notify, "popup/default");
     }
   /* END */


   /* TIZEN_ONLY(20160623):Apply popup compress mode UX */
   priv->dispmode = evas_object_size_hint_display_mode_get(obj);

   evas_object_event_callback_add(obj, EVAS_CALLBACK_CHANGED_SIZE_HINTS,
                                  _on_obj_size_hints_changed, NULL);
   /* END */

   /* TIZEN_ONLY(20161109): check theme compatibility more precise
   elm_notify_align_set(priv->notify,
                        _elm_config->popup_horizontal_align,
                        _elm_config->popup_vertical_align);
   */
   elm_notify_allow_events_set(priv->notify, EINA_FALSE);
   evas_object_size_hint_weight_set
     (priv->notify, EVAS_HINT_EXPAND, EVAS_HINT_EXPAND);
   evas_object_size_hint_align_set
     (priv->notify, EVAS_HINT_FILL, EVAS_HINT_FILL);
   evas_object_smart_member_add(priv->notify, obj);
   efl_ui_mirrored_automatic_set(priv->notify, EINA_FALSE);
   elm_object_mirrored_set(priv->notify, elm_object_mirrored_get(obj));

   evas_object_event_callback_add(priv->notify, EVAS_CALLBACK_RESIZE, _notify_resize_cb, obj);
   efl_event_callback_array_add(priv->notify, composition_cb(), obj);

   priv->main_layout = elm_layout_add(obj);
   if (!elm_layout_theme_set(priv->main_layout, "popup", "base",
                             elm_widget_style_get(obj)))
     CRI("Failed to set layout!");

   /***********************************************************************************
    * TIZEN_ONLY_FEATURE: apply Tizen's color_class features.                         *
    ***********************************************************************************/
   _elm_widget_color_class_parent_set(priv->main_layout, obj);
   /*******
    * END *
    *******/

   elm_object_content_set(priv->notify, priv->main_layout);

   evas_object_event_callback_add(obj, EVAS_CALLBACK_SHOW, _on_show, NULL);
   efl_ui_mirrored_automatic_set(priv->main_layout, EINA_FALSE);
   elm_object_mirrored_set(priv->main_layout, elm_object_mirrored_get(obj));

   elm_layout_signal_callback_add
     (priv->main_layout, "elm,state,title_area,visible", "elm", _layout_change_cb, NULL);
   elm_layout_signal_callback_add
     (priv->main_layout, "elm,state,title_area,hidden", "elm", _layout_change_cb, NULL);
   elm_layout_signal_callback_add
     (priv->main_layout, "elm,state,action_area,visible", "elm", _layout_change_cb, NULL);
   elm_layout_signal_callback_add
     (priv->main_layout, "elm,state,action_area,hidden", "elm", _layout_change_cb, NULL);

   priv->content_area = elm_layout_add(priv->main_layout);

   /***********************************************************************************
    * TIZEN_ONLY_FEATURE: apply Tizen's color_class features.                         *
    ***********************************************************************************/
   _elm_widget_color_class_parent_set(priv->content_area, obj);
   /*******
    * END *
    *******/

   /* TIZEN_ONLY(20161109): check theme compatibility more precise
   if (!elm_layout_theme_set(priv->content_area, "popup", "content", style))
     CRI("Failed to set layout!");
   else
     evas_object_event_callback_add
        (priv->content_area, EVAS_CALLBACK_CHANGED_SIZE_HINTS,
         _size_hints_changed_cb, priv->main_layout);
    */
   for (i = 0 ; i < STYLE_PRIORITY_COUNT; i++)
     {
        snprintf(style, sizeof(style), STYLE_PRIORITY[i], elm_widget_style_get(obj));
        if (efl_ui_layout_theme_set(priv->content_area, "popup", "content", style) == EFL_UI_THEME_APPLY_RESULT_SUCCESS)
          break;
     }
   if (i == STYLE_PRIORITY_COUNT)
     CRI("Failed to set layout!");
   else
     evas_object_event_callback_add
        (priv->content_area, EVAS_CALLBACK_CHANGED_SIZE_HINTS,
         _size_hints_changed_cb, obj);
   /* END */

   priv->content_text_wrap_type = ELM_WRAP_MIXED;
   efl_event_callback_array_add(priv->notify, _notify_cb(), obj);

   //TIZEN_ONLY(20180607): Restore legacy focus
   if (elm_widget_is_legacy(obj))
     {
        elm_widget_can_focus_set(obj, EINA_TRUE);
        elm_widget_can_focus_set(priv->main_layout, EINA_TRUE);
     }
   //

   _populate_theme_scroll(priv);

   _visuals_set(obj);

   if (_elm_config->popup_scrollable)
     elm_popup_scrollable_set(obj, _elm_config->popup_scrollable);
}

static void
_parent_geom_cb(void *data, Evas *e EINA_UNUSED, Evas_Object *obj, void *event_info EINA_UNUSED)
{
   Evas_Coord x, y, w, h;
   Evas_Object *popup = data;

   evas_object_geometry_get(obj, &x, &y, &w, &h);

   if (efl_isa(obj, EFL_UI_WIN_CLASS))
     {
        x = 0;
        y = 0;
     }

   evas_object_move(popup, x, y);
   evas_object_resize(popup, w, h);
}

EOLIAN static void
_elm_popup_efl_ui_widget_widget_parent_set(Eo *obj, Elm_Popup_Data *sd, Evas_Object *parent)
{
   Evas_Coord x, y, w, h;
   evas_object_geometry_get(parent, &x, &y, &w, &h);

   if (efl_isa(parent, EFL_UI_WIN_CLASS))
     {
        x = 0;
        y = 0;
     }
   evas_object_move(obj, x, y);
   evas_object_resize(obj, w, h);

   sd->parent = parent;
   evas_object_event_callback_add(parent, EVAS_CALLBACK_RESIZE, _parent_geom_cb, obj);
   evas_object_event_callback_add(parent, EVAS_CALLBACK_MOVE, _parent_geom_cb, obj);
}

EOLIAN static void
_elm_popup_efl_ui_widget_on_access_update(Eo *obj, Elm_Popup_Data *_pd EINA_UNUSED, Eina_Bool is_access)
{
   _access_obj_process(obj, is_access);
}

//TIZEN_ONLY(20160822): When atspi mode is dynamically switched on/off,
//register/unregister access objects accordingly.
// TIZEN_ONLY(20170516): connect to at-spi dbus based on org.a11y.Status.IsEnabled property
EOLIAN static void
_elm_popup_efl_ui_widget_screen_reader(Eo *obj, Elm_Popup_Data *_pd EINA_UNUSED, Eina_Bool is_screen_reader)
{
   _atspi_expose_title(obj, is_screen_reader);
   //TIZEN_ONLY(20170314): expose part-text as at-spi object
   _atspi_part_text_expose(obj, "elm.text", is_screen_reader);
   //
   //TIZEN_ONLY(20161213): apply screen_reader_changed callback
   evas_object_smart_callback_call(obj, SIG_ATSPI_SCREEN_READER_CHANGED, &is_screen_reader);
   //
}

EOLIAN static void
_elm_popup_efl_ui_widget_atspi(Eo *obj, Elm_Popup_Data *_pd EINA_UNUSED, Eina_Bool is_atspi)
{
   Efl_Access_State_Set ss;

   if (!is_atspi) return;

   ss = efl_access_object_state_set_get(obj);
   if (STATE_TYPE_GET(ss, EFL_ACCESS_STATE_SHOWING))
     {
        efl_access_state_changed_signal_emit(obj, EFL_ACCESS_STATE_SHOWING, EINA_TRUE);
     }
}
//
//

EAPI Evas_Object *
elm_popup_add(Evas_Object *parent)
{
   EINA_SAFETY_ON_NULL_RETURN_VAL(parent, NULL);
   Evas_Object *obj = elm_legacy_add(MY_CLASS, parent);

   ELM_WIDGET_DATA_GET_OR_RETURN(obj, wd, NULL);
   wd->highlight_root = EINA_TRUE;

   return obj;
}

EOLIAN static Eo *
_elm_popup_efl_object_constructor(Eo *obj, Elm_Popup_Data *_pd EINA_UNUSED)
{
   obj = efl_constructor(efl_super(obj, MY_CLASS));
   efl_canvas_object_type_set(obj, MY_CLASS_NAME_LEGACY);
   evas_object_smart_callbacks_descriptions_set(obj, _smart_callbacks);
   efl_access_object_role_set(obj, EFL_ACCESS_ROLE_DIALOG);
   legacy_object_focus_handle(obj);

   return obj;
}

EOLIAN static void
_elm_popup_content_text_wrap_type_set(Eo *obj EINA_UNUSED, Elm_Popup_Data *sd, Elm_Wrap_Type wrap)
{
   //Need to wrap the content text, so not allowing ELM_WRAP_NONE
   if (wrap == ELM_WRAP_NONE) return;

   sd->content_text_wrap_type = wrap;
   if (sd->text_content_obj)
     elm_label_line_wrap_set(sd->text_content_obj, wrap);
}

EOLIAN static Elm_Wrap_Type
_elm_popup_content_text_wrap_type_get(const Eo *obj EINA_UNUSED, Elm_Popup_Data *sd)
{
   return sd->content_text_wrap_type;
}

/* keeping old externals orient api for notify, but taking away the
 * introduced deprecation warning by copying the deprecated code
 * here */
static Elm_Notify_Orient
_elm_notify_orient_get(const Evas_Object *obj)
{
   Elm_Notify_Orient orient;
   double horizontal, vertical;

   elm_notify_align_get(obj, &horizontal, &vertical);

   if ((EINA_DBL_EQ(horizontal, 0.5)) && (EINA_DBL_EQ(vertical, 0.0)))
     orient = ELM_NOTIFY_ORIENT_TOP;
   else if ((EINA_DBL_EQ(horizontal, 0.5)) && (EINA_DBL_EQ(vertical, 0.5)))
     orient = ELM_NOTIFY_ORIENT_CENTER;
   else if ((EINA_DBL_EQ(horizontal, 0.5)) && (EINA_DBL_EQ(vertical, 1.0)))
     orient = ELM_NOTIFY_ORIENT_BOTTOM;
   else if ((EINA_DBL_EQ(horizontal, 0.0)) && (EINA_DBL_EQ(vertical, 0.5)))
     orient = ELM_NOTIFY_ORIENT_LEFT;
   else if ((EINA_DBL_EQ(horizontal, 1.0)) && (EINA_DBL_EQ(vertical, 0.5)))
     orient = ELM_NOTIFY_ORIENT_RIGHT;
   else if ((EINA_DBL_EQ(horizontal, 0.0)) && (EINA_DBL_EQ(vertical, 0.0)))
     orient = ELM_NOTIFY_ORIENT_TOP_LEFT;
   else if ((EINA_DBL_EQ(horizontal, 1.0)) && (EINA_DBL_EQ(vertical, 0.0)))
     orient = ELM_NOTIFY_ORIENT_TOP_RIGHT;
   else if ((EINA_DBL_EQ(horizontal, 0.0)) && (EINA_DBL_EQ(vertical, 1.0)))
     orient = ELM_NOTIFY_ORIENT_BOTTOM_LEFT;
   else if ((EINA_DBL_EQ(horizontal, 1.0)) && (EINA_DBL_EQ(vertical, 1.0)))
     orient = ELM_NOTIFY_ORIENT_BOTTOM_RIGHT;
   else
     orient = ELM_NOTIFY_ORIENT_TOP;
   return orient;
}

static void
_elm_notify_orient_set(Evas_Object *obj,
                       Elm_Notify_Orient orient)
{
   double horizontal = 0, vertical = 0;

   switch (orient)
     {
      case ELM_NOTIFY_ORIENT_TOP:
         horizontal = 0.5; vertical = 0.0;
        break;

      case ELM_NOTIFY_ORIENT_CENTER:
         horizontal = 0.5; vertical = 0.5;
        break;

      case ELM_NOTIFY_ORIENT_BOTTOM:
         horizontal = 0.5; vertical = 1.0;
        break;

      case ELM_NOTIFY_ORIENT_LEFT:
         horizontal = 0.0; vertical = 0.5;
        break;

      case ELM_NOTIFY_ORIENT_RIGHT:
         horizontal = 1.0; vertical = 0.5;
        break;

      case ELM_NOTIFY_ORIENT_TOP_LEFT:
         horizontal = 0.0; vertical = 0.0;
        break;

      case ELM_NOTIFY_ORIENT_TOP_RIGHT:
         horizontal = 1.0; vertical = 0.0;
        break;

      case ELM_NOTIFY_ORIENT_BOTTOM_LEFT:
         horizontal = 0.0; vertical = 1.0;
        break;

      case ELM_NOTIFY_ORIENT_BOTTOM_RIGHT:
         horizontal = 1.0; vertical = 1.0;
        break;

      case ELM_NOTIFY_ORIENT_LAST:
        break;
     }

   elm_notify_align_set(obj, horizontal, vertical);
}

EOLIAN static void
_elm_popup_orient_set(Eo *obj EINA_UNUSED, Elm_Popup_Data *sd, Elm_Popup_Orient orient)
{
   if (orient >= ELM_POPUP_ORIENT_LAST) return;
   _elm_notify_orient_set(sd->notify, (Elm_Notify_Orient)orient);
}

EOLIAN static Elm_Popup_Orient
_elm_popup_orient_get(const Eo *obj EINA_UNUSED, Elm_Popup_Data *sd)
{
   return (Elm_Popup_Orient)_elm_notify_orient_get(sd->notify);
}

EOLIAN static void
_elm_popup_align_set(Eo *obj EINA_UNUSED, Elm_Popup_Data *sd, double horizontal, double vertical)
{
   elm_notify_align_set(sd->notify, horizontal, vertical);
}

EOLIAN static void
_elm_popup_align_get(const Eo *obj EINA_UNUSED, Elm_Popup_Data *sd, double *horizontal, double *vertical)
{
   elm_notify_align_get(sd->notify, horizontal, vertical);
}

EOLIAN static void
_elm_popup_timeout_set(Eo *obj EINA_UNUSED, Elm_Popup_Data *sd, double timeout)
{
   elm_notify_timeout_set(sd->notify, timeout);
}

EOLIAN static double
_elm_popup_timeout_get(const Eo *obj EINA_UNUSED, Elm_Popup_Data *sd)
{
   return elm_notify_timeout_get(sd->notify);
}

EOLIAN static void
_elm_popup_allow_events_set(Eo *obj EINA_UNUSED, Elm_Popup_Data *sd, Eina_Bool allow)
{
   Eina_Bool allow_events = !!allow;

   elm_notify_allow_events_set(sd->notify, allow_events);
}

EOLIAN static Eina_Bool
_elm_popup_allow_events_get(const Eo *obj EINA_UNUSED, Elm_Popup_Data *sd)
{
   return elm_notify_allow_events_get(sd->notify);
}

EOLIAN static Elm_Object_Item*
_elm_popup_item_append(Eo *obj, Elm_Popup_Data *sd, const char *label, Evas_Object *icon, Evas_Smart_Cb func, const void *data)
{
   Evas_Object *prev_content;
   Eo *eo_it;

   eo_it = efl_add(ELM_POPUP_ITEM_CLASS, obj);
   if (!eo_it) return NULL;
   ELM_POPUP_ITEM_DATA_GET(eo_it, it);
   if (sd->content || sd->text_content_obj)
     {
        prev_content = efl_content_get(efl_part(sd->content_area, CONTENT_PART));
        evas_object_del(prev_content);
     }

   //The first item is appended.
   if (!sd->items)
     _list_add(obj);

   it->func = func;
   WIDGET_ITEM_DATA_SET(eo_it, data);

   _item_new(it);
   _item_icon_set(it, icon);
   _item_text_set(it, label);

   elm_box_pack_end(sd->box, VIEW(it));
   sd->items = eina_list_append(sd->items, it);

   elm_layout_sizing_eval(obj);

   return eo_it;
}

EOLIAN void
_elm_popup_scrollable_set(Eo *obj, Elm_Popup_Data *pd, Eina_Bool scroll)
{
   scroll = !!scroll;
   if (pd->scroll == scroll) return;
   pd->scroll = scroll;

   if (!pd->scr)
     _create_scroller(obj);
   else
     {
        elm_object_content_unset(pd->scr);
        ELM_SAFE_FREE(pd->tbl, evas_object_del);
        _create_scroller(obj);
     }

   if (!pd->scroll)
     {
        efl_content_set(efl_part(pd->content_area, CONTENT_PART), pd->tbl);
        efl_content_set(efl_part(pd->main_layout, CONTENT_PART), pd->content_area);
        if (pd->content) efl_content_set(efl_part(pd->content_area, CONTENT_PART), pd->content);
        else if (pd->text_content_obj) efl_content_set(efl_part(pd->content_area, CONTENT_PART), pd->text_content_obj);
        if (pd->theme_scroll)
          elm_layout_signal_emit(pd->content_area, "elm,scroll,disable", "elm");
     }
   else
     {
        if (pd->content || pd->text_content_obj)
          {
             efl_content_unset(efl_part(pd->main_layout, CONTENT_PART));
             elm_object_content_set(pd->scr, pd->content_area);
          }
        efl_content_set(efl_part(pd->main_layout, CONTENT_PART), pd->tbl);
        if (pd->theme_scroll)
          elm_layout_signal_emit(pd->content_area, "elm,scroll,enable", "elm");
     }

   elm_layout_sizing_eval(obj);
}

EOLIAN Eina_Bool
_elm_popup_scrollable_get(const Eo *obj EINA_UNUSED, Elm_Popup_Data *pd)
{
   return pd->scroll;
}

EOLIAN static void
_elm_popup_dismiss(Eo *obj, Elm_Popup_Data *pd)
{
   //TIZEN_ONLY(20170919): Handle default label object
   _elm_win_default_label_obj_remove(obj);
   //
   elm_layout_signal_emit(pd->main_layout, "elm,state,hide", "elm");
   elm_notify_dismiss(pd->notify);
}

static void
_elm_popup_class_constructor(Efl_Class *klass)
{
   evas_smart_legacy_type_register(MY_CLASS_NAME_LEGACY, klass);
}

static Eina_Bool
_action_dismiss(Evas_Object *obj, const char *params EINA_UNUSED)
{
   efl_event_callback_legacy_call(obj, ELM_POPUP_EVENT_BLOCK_CLICKED, NULL);
   return EINA_TRUE;
}

EOLIAN const Efl_Access_Action_Data *
_elm_popup_efl_access_widget_action_elm_actions_get(const Eo *obj EINA_UNUSED, Elm_Popup_Data *pd)
{
   static Efl_Access_Action_Data atspi_actions[] = {
//TIZEN_ONLY(20150709) : add keybinding for elm_popup "block,clicked" action
          { "escape", "escape", NULL, _key_action_escape},
//
          { "dismiss", NULL, NULL, _action_dismiss},
          { NULL, NULL, NULL, NULL }
   };
   if (pd->action_area)
     return NULL;
   return &atspi_actions[0];
}

EOLIAN static Efl_Access_State_Set
_elm_popup_efl_access_object_state_set_get(const Eo *obj, Elm_Popup_Data *sd EINA_UNUSED)
{
   Efl_Access_State_Set ret;
   ret = efl_access_object_state_set_get(efl_super(obj, MY_CLASS));

   STATE_TYPE_SET(ret, EFL_ACCESS_STATE_MODAL);

   return ret;
}

EOLIAN static const char*
_elm_popup_efl_access_object_i18n_name_get(const Eo *obj, Elm_Popup_Data *sd)
{
   const char *name = NULL;
   Eina_Strbuf *buf;

   name = efl_access_object_i18n_name_get(efl_super(obj, ELM_POPUP_CLASS));
   if (name) return name;

   buf = eina_strbuf_new();
   eina_strbuf_append_printf(buf, "%s", E_("Alert"));
   if (sd->title_text)
     eina_strbuf_append_printf(buf, ", %s", sd->title_text);
   else if (sd->text_content_obj)
     eina_strbuf_append_printf(buf, ", %s", elm_object_text_get(sd->text_content_obj));
   else if (elm_object_part_text_get(obj, "elm.text"))
     eina_strbuf_append_printf(buf, ", %s", elm_object_part_text_get(obj, "elm.text"));

   name = _elm_widget_accessible_plain_name_get(obj, eina_strbuf_string_get(buf));
   eina_strbuf_free(buf);

   return name;
}

//TIZEN ONLY(20150708): popup and ctxpopup accessibility highlight impementation
EOLIAN static Eina_Bool
_elm_popup_efl_access_component_highlight_grab(Eo *obj EINA_UNUSED, Elm_Popup_Data *sd)
{
   if (sd->scr)
     {
        elm_object_accessibility_highlight_set(sd->main_layout, EINA_TRUE);
        return EINA_TRUE;
     }
   return EINA_FALSE;
}

EOLIAN static Eina_Bool
_elm_popup_efl_access_component_highlight_clear(Eo *obj EINA_UNUSED, Elm_Popup_Data *sd)
{
   if (sd->scr)
     {
        elm_object_accessibility_highlight_set(sd->main_layout, EINA_FALSE);
        return EINA_TRUE;
     }
   return EINA_FALSE;
}
//

/* TIZEN_ONLY(20161109): check theme compatibility more precise
                         add internal style set function */
static Efl_Ui_Theme_Apply_Result
_notify_style_set(Evas_Object *obj, const char *style)
{
   Efl_Ui_Theme_Apply_Result ret = EFL_UI_THEME_APPLY_RESULT_SUCCESS;

   ELM_WIDGET_DATA_GET_OR_RETURN(obj, wd, EFL_UI_THEME_APPLY_RESULT_FAIL);

   if (eina_stringshare_replace(&wd->style, style))
     {
        const Eina_List *l;
        Elm_Tooltip *tt;
        Elm_Cursor *cur;

        if (wd->hover_obj) ret &= elm_widget_theme(wd->hover_obj);

        EINA_LIST_FOREACH(wd->tooltips, l, tt)
          elm_tooltip_theme(tt);
        EINA_LIST_FOREACH(wd->cursors, l, cur)
          elm_cursor_theme(cur);

        ret &= efl_ui_widget_theme_apply(obj);
     }

   return ret;
}
/* END */

/* Standard widget overrides */

ELM_WIDGET_KEY_DOWN_DEFAULT_IMPLEMENT(elm_popup, Elm_Popup_Data)

/* Efl.Part begin */

ELM_PART_OVERRIDE(elm_popup, ELM_POPUP, Elm_Popup_Data)
ELM_PART_OVERRIDE_CONTENT_SET(elm_popup, ELM_POPUP, Elm_Popup_Data)
ELM_PART_OVERRIDE_CONTENT_GET(elm_popup, ELM_POPUP, Elm_Popup_Data)
ELM_PART_OVERRIDE_CONTENT_UNSET(elm_popup, ELM_POPUP, Elm_Popup_Data)
ELM_PART_OVERRIDE_TEXT_SET(elm_popup, ELM_POPUP, Elm_Popup_Data)
ELM_PART_OVERRIDE_TEXT_GET(elm_popup, ELM_POPUP, Elm_Popup_Data)
#include "elm_popup_part.eo.c"

/* Efl.Part end */

/* Internal EO APIs and hidden overrides */

#define ELM_POPUP_EXTRA_OPS \
   ELM_LAYOUT_SIZING_EVAL_OPS(elm_popup), \
   EFL_CANVAS_GROUP_ADD_DEL_OPS(elm_popup)

#include "elm_popup.eo.c"
#include "elm_popup_item.eo.c"
