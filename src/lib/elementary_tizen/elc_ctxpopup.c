#ifdef HAVE_CONFIG_H
# include "../elementary/elementary_config.h"
#endif

#define EFL_ACCESS_OBJECT_PROTECTED
#define EFL_ACCESS_WIDGET_ACTION_PROTECTED
#define ELM_WIDGET_PROTECTED
#define ELM_WIDGET_ITEM_PROTECTED
#define EFL_ACCESS_COMPONENT_PROTECTED
#define EFL_UI_L10N_PROTECTED
#define EFL_PART_PROTECTED

#include <Elementary.h>

#include "../elementary/elm_priv.h"
#include "elm_widget_ctxpopup.h"
#include "../elementary/elm_ctxpopup_item.eo.h"
#include "../elementary/elm_ctxpopup.eo.h"

#include "../elementary/elm_ctxpopup_part.eo.h"
#include "../elementary/elm_part_helper.h"

#define MY_CLASS ELM_CTXPOPUP_CLASS

#define MY_CLASS_NAME "Elm_Ctxpopup"
#define MY_CLASS_NAME_LEGACY "elm_ctxpopup"

EAPI const char ELM_CTXPOPUP_SMART_NAME[] = "elm_ctxpopup";

#define ELM_PRIV_CTXPOPUP_SIGNALS(cmd) \
   cmd(SIG_DISMISSED, "dismissed", "") \

ELM_PRIV_CTXPOPUP_SIGNALS(ELM_PRIV_STATIC_VARIABLE_DECLARE);

static const Evas_Smart_Cb_Description _smart_callbacks[] = {
   ELM_PRIV_CTXPOPUP_SIGNALS(ELM_PRIV_SMART_CALLBACKS_DESC)
   {SIG_WIDGET_LANG_CHANGED, ""}, /**< handled by elm_widget */
   {SIG_WIDGET_ACCESS_CHANGED, ""}, /**< handled by elm_widget */
   {SIG_LAYOUT_FOCUSED, ""}, /**< handled by elm_layout */
   {SIG_LAYOUT_UNFOCUSED, ""}, /**< handled by elm_layout */
   {NULL, NULL}
};
#undef ELM_PRIV_CTXPOPUP_SIGNALS

#define TTS_STR_MENU_POPUP dgettext(PACKAGE, "IDS_SCR_OPT_MENU_POP_UP_TTS")
#define TTS_STR_MENU_CLOSE dgettext(PACKAGE, "IDS_ST_BODY_DOUBLE_TAP_TO_CLOSE_THE_MENU_T_TTS")

static const char ACCESS_OUTLINE_PART[] = "access.outline";

static void _on_move(void *data EINA_UNUSED, Evas *e EINA_UNUSED, Evas_Object *obj, void *event_info EINA_UNUSED);
static void _item_sizing_eval(Elm_Ctxpopup_Item_Data *item);
static Eina_Bool _key_action_escape(Evas_Object *obj, const char *params EINA_UNUSED);
//TIZEN ONLY(20161014) : Accessibility: scroll the content when highlight frame goes out of view port
static void _content_move_down_cb(void *data, Evas_Object *obj, void *ev EINA_UNUSED);
static void _content_move_up_cb(void *data, Evas_Object *obj, void *ev EINA_UNUSED);
//

static const Elm_Action key_actions[] = {
   {"escape", _key_action_escape},
   {NULL, NULL}
};

EOLIAN static void
_elm_ctxpopup_efl_ui_l10n_translation_update(Eo *obj, Elm_Ctxpopup_Data *sd)
{
   Eina_List *l;
   Elm_Ctxpopup_Item_Data *it;

   if (sd->auto_hide)
     {
        evas_object_hide(obj);
        efl_event_callback_legacy_call(obj, ELM_CTXPOPUP_EVENT_DISMISSED, NULL);
     }

   EINA_LIST_FOREACH(sd->items, l, it)
     elm_wdg_item_translate(EO_OBJ(it));

   efl_ui_l10n_translation_update(efl_super(obj, MY_CLASS));
}

static Evas_Object *
_access_object_get(const Evas_Object *obj, const char* part)
{
   Evas_Object *po, *ao;

   ELM_WIDGET_DATA_GET_OR_RETURN(obj, wd, NULL);

   po = (Evas_Object *)edje_object_part_object_get(wd->resize_obj, part);
   ao = evas_object_data_get(po, "_part_access_obj");

   return ao;
}

EOLIAN static Eina_Bool
_elm_ctxpopup_efl_ui_widget_focus_next(Eo *obj EINA_UNUSED, Elm_Ctxpopup_Data *sd, Elm_Focus_Direction dir, Evas_Object **next, Elm_Object_Item **next_item)
{
   Eina_List *items = NULL;
   Evas_Object *ao;

   if (!sd)
     return EINA_FALSE;

   if (_elm_config->access_mode)
     {
        Eina_Bool ret;

        ao = _access_object_get(obj, ACCESS_OUTLINE_PART);
        if (ao) items = eina_list_append(items, ao);

        // scroller exists when ctxpopup has an item
        if (sd->scr)
           items = eina_list_append(items, sd->scr);
        else
           items = eina_list_append(items, sd->box);

        _elm_access_auto_highlight_set(EINA_TRUE);
        ret = efl_ui_widget_focus_list_next_get(obj, items, eina_list_data_get, dir, next, next_item);
        _elm_access_auto_highlight_set(EINA_FALSE);
        return ret;
     }
   else
     {
        efl_ui_widget_focus_next_get(sd->box, dir, next, next_item);
        if (!*next) *next = (Evas_Object *)obj;
        return EINA_TRUE;
     }
}

EOLIAN static Eina_Bool
_elm_ctxpopup_efl_ui_widget_focus_next_manager_is(Eo *obj EINA_UNUSED, Elm_Ctxpopup_Data *_pd EINA_UNUSED)
{
   return EINA_TRUE;
}

EOLIAN static Eina_Bool
_elm_ctxpopup_efl_ui_widget_focus_direction_manager_is(Eo *obj EINA_UNUSED, Elm_Ctxpopup_Data *_pd EINA_UNUSED)
{
   return EINA_TRUE;
}

EOLIAN static Eina_Bool
_elm_ctxpopup_efl_ui_widget_focus_direction(Eo *obj EINA_UNUSED, Elm_Ctxpopup_Data *sd, const Evas_Object *base, double degree, Evas_Object **direction, Elm_Object_Item **direction_item, double *weight)
{
   Eina_List *l = NULL;
   void *(*list_data_get)(const Eina_List *list);

   if (!sd)
     return EINA_FALSE;

   list_data_get = eina_list_data_get;

   l = eina_list_append(l, sd->box);

   efl_ui_widget_focus_list_direction_get
      (obj, base, l, list_data_get, degree, direction, direction_item, weight);
   eina_list_free(l);

   return EINA_TRUE;
}

static Eina_Bool
_key_action_move(Evas_Object *obj, const char *params)
{
   ELM_CTXPOPUP_DATA_GET(obj, sd);
   const char *dir = params;

   if (!sd->box) return EINA_FALSE;

   if (!strcmp(dir, "previous"))
     efl_ui_widget_focus_cycle(sd->box, ELM_FOCUS_PREVIOUS);
   else if (!strcmp(dir, "next"))
     efl_ui_widget_focus_cycle(sd->box, ELM_FOCUS_NEXT);
   else if (!strcmp(dir, "left"))
     efl_ui_widget_focus_cycle(sd->box, ELM_FOCUS_LEFT);
   else if (!strcmp(dir, "right"))
     efl_ui_widget_focus_cycle(sd->box, ELM_FOCUS_RIGHT);
   else if (!strcmp(dir, "up"))
     efl_ui_widget_focus_cycle(sd->box, ELM_FOCUS_UP);
   else if (!strcmp(dir, "down"))
     efl_ui_widget_focus_cycle(sd->box, ELM_FOCUS_DOWN);
   else return EINA_FALSE;

   return EINA_TRUE;
}

static void
_x_pos_adjust(Evas_Coord_Point *pos,
              Evas_Coord_Point *base_size,
              Evas_Coord_Rectangle *hover_area)
{
   pos->x -= (base_size->x / 2);

   if (pos->x < hover_area->x)
     pos->x = hover_area->x;
   else if ((pos->x + base_size->x) > (hover_area->x + hover_area->w))
     pos->x = (hover_area->x + hover_area->w) - base_size->x;

   if (base_size->x > hover_area->w)
     base_size->x -= (base_size->x - hover_area->w);

   if (pos->x < hover_area->x)
     pos->x = hover_area->x;
}

static void
_y_pos_adjust(Evas_Coord_Point *pos,
              Evas_Coord_Point *base_size,
              Evas_Coord_Rectangle *hover_area)
{
   pos->y -= (base_size->y / 2);

   if (pos->y < hover_area->y)
     pos->y = hover_area->y;
   else if ((pos->y + base_size->y) > (hover_area->y + hover_area->h))
     pos->y = hover_area->y + hover_area->h - base_size->y;

   if (base_size->y > hover_area->h)
     base_size->y -= (base_size->y - hover_area->h);

   if (pos->y < hover_area->y)
     pos->y = hover_area->y;
}

static void
_item_select_cb(void *data,
                Evas_Object *obj EINA_UNUSED,
                void *event_info EINA_UNUSED)
{
   Elm_Ctxpopup_Item_Data *item = data;

   if (!item) return;
   if (elm_wdg_item_disabled_get(EO_OBJ(item)))
     {
        if (_elm_config->access_mode)
          _elm_access_say(E_("Disabled"));
        return;
     }

   if (item->wcb.org_func_cb)
     item->wcb.org_func_cb((void*)item->wcb.org_data, WIDGET(item), EO_OBJ(item));
}

static char *
_access_info_cb(void *data, Evas_Object *obj EINA_UNUSED)
{
   Elm_Ctxpopup_Item_Data *it = (Elm_Ctxpopup_Item_Data *)data;
   const char *txt = NULL;
   Evas_Object *icon = NULL;

   if (!it) return NULL;

   txt = it->label;
   icon = it->icon;

   if (txt) return strdup(txt);
   if (icon) return strdup(E_("icon"));
   return NULL;
}

static char *
_access_state_cb(void *data, Evas_Object *obj EINA_UNUSED)
{
   Elm_Ctxpopup_Item_Data *it = (Elm_Ctxpopup_Item_Data *)data;
   if (!it) return NULL;

   if (elm_wdg_item_disabled_get(EO_OBJ(it)))
     return strdup(E_("Disabled"));

   return NULL;
}

static void
_access_focusable_button_register(Evas_Object *obj, Elm_Ctxpopup_Item_Data *it)
{
   Elm_Access_Info *ai;

   ai = _elm_access_info_get(obj);

   _elm_access_callback_set(ai, ELM_ACCESS_INFO, _access_info_cb, it);
   _elm_access_callback_set(ai, ELM_ACCESS_STATE, _access_state_cb, it);
   _elm_access_callback_set(ai, ELM_ACCESS_TYPE, NULL, NULL);

   it->base->access_obj = obj;
}

static void
_mouse_down_cb(Elm_Ctxpopup_Item_Data *it,
               Evas *evas EINA_UNUSED,
               Evas_Object *obj EINA_UNUSED,
               Evas_Event_Mouse_Down *ev)
{
   ELM_CTXPOPUP_DATA_GET(WIDGET(it), sd);

   if (ev->button != 1) return;

   //If counter is not zero, it means all other multi down is not released.
   if (sd->multi_down != 0) return;
   sd->mouse_down = EINA_TRUE;
//******************** TIZEN Only
   sd->pressed = EINA_TRUE;
//****************************
}

//******************** TIZEN Only
static void
_mouse_move_cb(Elm_Ctxpopup_Item_Data *it,
               Evas *evas EINA_UNUSED,
               Evas_Object *obj,
               void *event_info)
{
   ELM_CTXPOPUP_DATA_GET(WIDGET(it), sd);
   Evas_Event_Mouse_Move *ev = event_info;

   if (ev->event_flags & EVAS_EVENT_FLAG_ON_HOLD)
     {
        if (sd->pressed)
          {
             edje_object_signal_emit(obj,"elm,action,unpressed", "elm");
             sd->pressed = EINA_FALSE;
          }
     }
   else
     {
        Evas_Coord x, y, w, h;
        evas_object_geometry_get(obj, &x, &y, &w, &h);
        if ((sd->pressed) && (ELM_RECTS_POINT_OUT(x, y, w, h, ev->cur.canvas.x, ev->cur.canvas.y)))
          {
             edje_object_signal_emit(obj,"elm,action,unpressed", "elm");
             sd->pressed = EINA_FALSE;
          }
     }
}
//****************************

static void
_mouse_up_cb(Elm_Ctxpopup_Item_Data *it,
             Evas *evas EINA_UNUSED,
             Evas_Object *obj EINA_UNUSED,
             Evas_Event_Mouse_Up *ev)
{
   ELM_CTXPOPUP_DATA_GET(WIDGET(it), sd);

   if (ev->button != 1) return;

   if (!sd->mouse_down) return;
   sd->mouse_down = EINA_FALSE;
//******************** TIZEN Only
   sd->pressed = EINA_FALSE;
//****************************
}

static void
_multi_down_cb(void *data,
                    Evas *evas EINA_UNUSED,
                    Evas_Object *obj EINA_UNUSED,
                    void *event_info EINA_UNUSED)
{
   Elm_Ctxpopup_Item_Data *it = data;
   ELM_CTXPOPUP_DATA_GET(WIDGET(it), sd);

   //Emitting signal to inform edje about multi down event.
   edje_object_signal_emit(VIEW(it), "elm,action,multi,down", "elm");
   sd->multi_down++;
}

static void
_multi_up_cb(void *data,
                    Evas *evas EINA_UNUSED,
                    Evas_Object *obj EINA_UNUSED,
                    void *event_info EINA_UNUSED)
{
   Elm_Ctxpopup_Item_Data *it = data;
   ELM_CTXPOPUP_DATA_GET(WIDGET(it), sd);

   sd->multi_down--;
   if(sd->multi_down == 0)
     {
        //Emitting signal to inform edje about last multi up event.
        edje_object_signal_emit(VIEW(it), "elm,action,multi,up", "elm");
     }
}

static void
_item_theme_set(Elm_Ctxpopup_Item_Data *item,
          char *group_name)
{
   ELM_CTXPOPUP_DATA_GET(WIDGET(item), sd);
   if (!sd) return;

   VIEW(item) = edje_object_add(evas_object_evas_get(sd->box));
   edje_object_mirrored_set(VIEW(item), efl_ui_mirrored_get(WIDGET(item)));
   _elm_theme_object_set(WIDGET(item), VIEW(item), "ctxpopup", group_name,
                         elm_widget_style_get(WIDGET(item)));
   evas_object_size_hint_align_set(VIEW(item), EVAS_HINT_FILL, EVAS_HINT_FILL);
   evas_object_show(VIEW(item));

   evas_object_event_callback_add
     (VIEW(item), EVAS_CALLBACK_MOUSE_DOWN, (Evas_Object_Event_Cb)_mouse_down_cb,
     item);
//******************** TIZEN Only
   evas_object_event_callback_add
     (VIEW(item), EVAS_CALLBACK_MOUSE_MOVE, (Evas_Object_Event_Cb)_mouse_move_cb,
     item);
//****************************
   evas_object_event_callback_add
     (VIEW(item), EVAS_CALLBACK_MOUSE_UP, (Evas_Object_Event_Cb)_mouse_up_cb, item);

   /*Registering Multi down/up events to ignore mouse down/up events untill all
     multi down/up events are released.*/
   evas_object_event_callback_add
     (VIEW(item), EVAS_CALLBACK_MULTI_DOWN, (Evas_Object_Event_Cb)_multi_down_cb,
     item);
   evas_object_event_callback_add
     (VIEW(item), EVAS_CALLBACK_MULTI_UP, (Evas_Object_Event_Cb)_multi_up_cb,
     item);
}

static Evas_Object *
_item_in_focusable_button(Elm_Ctxpopup_Item_Data *item)
{
   Evas_Object *bt;

   bt = elm_button_add(WIDGET(item));
   elm_object_style_set(bt, "focus");
   evas_object_size_hint_align_set(bt, EVAS_HINT_FILL, EVAS_HINT_FILL);
   elm_object_part_content_set(bt, "elm.swallow.content", VIEW(item));
   evas_object_smart_callback_add(bt, "clicked", _item_select_cb, item);
   evas_object_show(bt);

   return bt;
}

EOLIAN static void
_elm_ctxpopup_item_elm_widget_item_del_pre(Eo *eo_item EINA_UNUSED, Elm_Ctxpopup_Item_Data *item)
{
   Evas_Object *btn;

   ELM_CTXPOPUP_DATA_GET(WIDGET(item), sd);
   if (!sd) return;

   btn = item->btn;
   elm_box_unpack(sd->box, btn);
   evas_object_smart_callback_del_full(btn, "clicked", _item_select_cb, item);
   evas_object_del(btn);

   sd->dir = ELM_CTXPOPUP_DIRECTION_UNKNOWN;

   if (item->icon)
     evas_object_del(item->icon);
   if (VIEW(item))
     evas_object_del(VIEW(item));

   eina_stringshare_del(item->label);
   if (item->style) eina_stringshare_del(item->style);
   sd->items = eina_list_remove(sd->items, item);

   if (eina_list_count(sd->items) < 1)
     {
        evas_object_hide(WIDGET(item));
        return;
     }
   if (sd->visible) elm_layout_sizing_eval(WIDGET(item));

   return;
}

static void
_items_remove(Elm_Ctxpopup_Data *sd)
{
   Eina_List *l, *l_next;
   Elm_Ctxpopup_Item_Data *item;

   if (!sd->items) return;

   EINA_LIST_FOREACH_SAFE(sd->items, l, l_next, item)
     efl_del(EO_OBJ(item));
//     elm_wdg_item_del(EO_OBJ(item));

   sd->items = NULL;
}

static void
_items_sizing_eval(Evas_Object *obj,
                   Elm_Ctxpopup_Data *sd)
{
   Elm_Ctxpopup_Item_Data *it;
   Eina_List *elist;
   int idx = 0;

   sd->dir = ELM_CTXPOPUP_DIRECTION_UNKNOWN;

   if (sd->visible)
     {
        EINA_LIST_FOREACH(sd->items, elist, it)
          {
             if (idx++ == 0)
               edje_object_signal_emit(VIEW(it), "elm,state,default", "elm");
             else
               edje_object_signal_emit(VIEW(it), "elm,state,separator", "elm");
          }
        elm_layout_sizing_eval(obj);
     }
}

static void
_item_sizing_eval(Elm_Ctxpopup_Item_Data *item)
{
   Evas_Coord min_w = -1, min_h = -1, max_w = -1, max_h = -1;

   if (!item) return;

   edje_object_signal_emit(VIEW(item), "elm,state,text,default", "elm");
   edje_object_message_signal_process(VIEW(item));
   edje_object_size_min_restricted_calc(VIEW(item), &min_w, &min_h, min_w,
                                        min_h);
   evas_object_size_hint_min_set(VIEW(item), min_w, min_h);
   evas_object_size_hint_max_set(VIEW(item), max_w, max_h);
}

static Elm_Ctxpopup_Direction
_base_geometry_calc(Evas_Object *obj,
                    Evas_Coord_Rectangle *rect)
{
   Elm_Ctxpopup_Direction dir = ELM_CTXPOPUP_DIRECTION_UNKNOWN;
   Evas_Coord_Rectangle hover_area;
   Evas_Coord_Point pos = {0, 0};
   Evas_Coord_Point arrow_size;
   Evas_Coord_Point base_size;
   Evas_Coord_Point max_size;
   Evas_Coord_Point min_size;
   Evas_Coord_Point temp;
   Evas_Object *win = NULL;
   int idx;
   const char *str;

   ELM_CTXPOPUP_DATA_GET(obj, sd);
   ELM_WIDGET_DATA_GET_OR_RETURN(obj, wd, ELM_CTXPOPUP_DIRECTION_DOWN);

   if (!rect) return ELM_CTXPOPUP_DIRECTION_DOWN;

   edje_object_part_geometry_get
     (sd->arrow, "ctxpopup_arrow", NULL, NULL, &arrow_size.x, &arrow_size.y);
   evas_object_resize(sd->arrow, arrow_size.x, arrow_size.y);

   //Initialize Area Rectangle.
   evas_object_geometry_get
     (sd->parent, &hover_area.x, &hover_area.y, &hover_area.w,
     &hover_area.h);

   evas_object_geometry_get(obj, &pos.x, &pos.y, NULL, NULL);

   /* TIZEN_ONLY(20170524): move position in hover area */
   if (pos.x < hover_area.x) pos.x = hover_area.x;
   else if (pos.x > hover_area.x + hover_area.w) pos.x = hover_area.x + hover_area.w;

   if (pos.y < hover_area.y) pos.y = hover_area.y;
   else if (pos.y > hover_area.y + hover_area.h) pos.y = hover_area.y + hover_area.h;
   /* END */

   win = elm_object_top_widget_get(obj);
   if ((win) && (!efl_isa(win, EFL_UI_WIN_CLASS)))
     win = NULL;

   //recalc the edje
   edje_object_size_min_calc
     (wd->resize_obj, &base_size.x, &base_size.y);
   evas_object_smart_calculate(wd->resize_obj);

   //Limit to Max Size
   evas_object_size_hint_max_get(obj, &max_size.x, &max_size.y);
   if (max_size.x == -1)
     {
        str = edje_object_data_get(sd->layout, "match_max_width");

        if ((str) && (win) && (!strcmp(str, "portrait_width")))
          {
             elm_win_screen_size_get(win, NULL, NULL, &max_size.x, NULL);
          }
        else
          {
             str = edje_object_data_get(sd->layout, "visible_maxw");

             if (str)
               max_size.x = (int)(atoi(str)
                                  * elm_config_scale_get()
                                  * elm_object_scale_get(obj)
                                  / edje_object_base_scale_get(sd->layout) + 0.5);
          }
     }
   if (max_size.y == -1)
     {
        str = edje_object_data_get(sd->layout, "visible_maxh");
        if (str)
          max_size.y = (int)(atoi(str)
                             * elm_config_scale_get()
                             * elm_object_scale_get(obj)
                             / edje_object_base_scale_get(sd->layout) + 0.5);
     }

   if ((max_size.y > 0) && (base_size.y > max_size.y))
     base_size.y = max_size.y;

   if ((max_size.x > 0) && (base_size.x > max_size.x))
     base_size.x = max_size.x;

   //Limit to Min Size
   evas_object_size_hint_min_get(obj, &min_size.x, &min_size.y);
   if (min_size.x == 0)
     {
        if ((str = edje_object_data_get(sd->layout, "match_min_width")) &&
            (!strcmp(str, "portrait_width")) && (win))
          elm_win_screen_size_get(win, NULL, NULL, &min_size.x, NULL);
        else
          edje_object_size_min_get(sd->layout, &min_size.x, NULL);
     }
   if (min_size.y == 0)
     edje_object_size_min_get(sd->layout, NULL, &min_size.y);

   if ((min_size.y > 0) && (base_size.y < min_size.y))
     base_size.y = min_size.y;

   if ((min_size.x > 0) && (base_size.x < min_size.x))
     base_size.x = min_size.x;

   //Check the Which direction is available.
   //If find a avaialble direction, it adjusts position and size.
   for (idx = 0; idx < 4; idx++)
     {
        switch (sd->dir_priority[idx])
          {
           case ELM_CTXPOPUP_DIRECTION_UNKNOWN:

           case ELM_CTXPOPUP_DIRECTION_UP:
             temp.y = (pos.y - base_size.y);
             if ((temp.y - arrow_size.y) < hover_area.y)
               continue;

             _x_pos_adjust(&pos, &base_size, &hover_area);
             pos.y -= base_size.y;
             dir = ELM_CTXPOPUP_DIRECTION_UP;
             break;

           case ELM_CTXPOPUP_DIRECTION_LEFT:
             temp.x = (pos.x - base_size.x);
             if ((temp.x - arrow_size.x) < hover_area.x)
               continue;

             _y_pos_adjust(&pos, &base_size, &hover_area);
             pos.x -= base_size.x;
             dir = ELM_CTXPOPUP_DIRECTION_LEFT;
             break;

           case ELM_CTXPOPUP_DIRECTION_RIGHT:
             temp.x = (pos.x + base_size.x);
             if ((temp.x + arrow_size.x) >
                 (hover_area.x + hover_area.w))
               continue;

             _y_pos_adjust(&pos, &base_size, &hover_area);
             dir = ELM_CTXPOPUP_DIRECTION_RIGHT;
             break;

           case ELM_CTXPOPUP_DIRECTION_DOWN:
             temp.y = (pos.y + base_size.y);
             if ((temp.y + arrow_size.y) >
                 (hover_area.y + hover_area.h))
               continue;

             _x_pos_adjust(&pos, &base_size, &hover_area);
             dir = ELM_CTXPOPUP_DIRECTION_DOWN;
             break;

           default:
             continue;
          }
        break;
     }

   //In this case, all directions are invalid because of lack of space.
   if (idx == 4)
     {
        Evas_Coord length[2];

        if (!sd->horizontal)
          {
             length[0] = pos.y - hover_area.y;
             length[1] = (hover_area.y + hover_area.h) - pos.y;

             // ELM_CTXPOPUP_DIRECTION_UP
             if (length[0] > length[1])
               {
                  _x_pos_adjust(&pos, &base_size, &hover_area);
                  pos.y -= base_size.y;
                  dir = ELM_CTXPOPUP_DIRECTION_UP;
                  if (pos.y < (hover_area.y + arrow_size.y))
                    {
                       base_size.y -= ((hover_area.y + arrow_size.y) - pos.y);
                       pos.y = hover_area.y + arrow_size.y;
                    }
               }
             //ELM_CTXPOPUP_DIRECTION_DOWN
             else
               {
                  _x_pos_adjust(&pos, &base_size, &hover_area);
                  dir = ELM_CTXPOPUP_DIRECTION_DOWN;
                  if ((pos.y + arrow_size.y + base_size.y) >
                      (hover_area.y + hover_area.h))
                    base_size.y -=
                      ((pos.y + arrow_size.y + base_size.y) -
                       (hover_area.y + hover_area.h));
               }
          }
        else
          {
             length[0] = pos.x - hover_area.x;
             length[1] = (hover_area.x + hover_area.w) - pos.x;

             //ELM_CTXPOPUP_DIRECTION_LEFT
             if (length[0] > length[1])
               {
                  _y_pos_adjust(&pos, &base_size, &hover_area);
                  pos.x -= base_size.x;
                  dir = ELM_CTXPOPUP_DIRECTION_LEFT;
                  if (pos.x < (hover_area.x + arrow_size.x))
                    {
                       base_size.x -= ((hover_area.x + arrow_size.x) - pos.x);
                       pos.x = hover_area.x + arrow_size.x;
                    }
               }
             //ELM_CTXPOPUP_DIRECTION_RIGHT
             else
               {
                  _y_pos_adjust(&pos, &base_size, &hover_area);
                  dir = ELM_CTXPOPUP_DIRECTION_RIGHT;
                  if (pos.x + (arrow_size.x + base_size.x) >
                      hover_area.x + hover_area.w)
                    base_size.x -=
                      ((pos.x + arrow_size.x + base_size.x) -
                       (hover_area.x + hover_area.w));
               }
          }
     }

   //Final position and size.
   rect->x = pos.x;
   rect->y = pos.y;
   rect->w = base_size.x;
   rect->h = base_size.y;

   return dir;
}

static void
_arrow_update(Evas_Object *obj,
              Elm_Ctxpopup_Direction dir,
              Evas_Coord_Rectangle base_size)
{
   Evas_Coord_Rectangle arrow_size;
   Evas_Coord x, y;
   double drag;
   Evas_Coord_Rectangle shadow_left_top, shadow_right_bottom, arrow_padding;

   ELM_CTXPOPUP_DATA_GET(obj, sd);
   ELM_WIDGET_DATA_GET_OR_RETURN(obj, wd);

   evas_object_geometry_get(obj, &x, &y, NULL, NULL);
   evas_object_geometry_get
     (sd->arrow, NULL, NULL, &arrow_size.w, &arrow_size.h);

   /* tizen only : since ctxpopup of tizen has shadow, start and end padding of arrow, it should be put together when updating arrow
    * so there are some differences between open source and tizen */
   edje_object_part_geometry_get(wd->resize_obj, "frame_shadow_left_top_padding", NULL, NULL, &shadow_left_top.w, &shadow_left_top.h);
   edje_object_part_geometry_get(wd->resize_obj, "frame_shadow_right_bottom_padding", NULL, NULL, &shadow_right_bottom.w, &shadow_right_bottom.h);
   edje_object_part_geometry_get(wd->resize_obj, "ctxpopup_frame_left_top", NULL, NULL, &arrow_padding.w, &arrow_padding.h);

   /* arrow is not being kept as sub-object on purpose, here. the
    * design of the widget does not help with the contrary */

   switch (dir)
     {
      case ELM_CTXPOPUP_DIRECTION_RIGHT:
        edje_object_signal_emit(sd->arrow, "elm,state,left", "elm");
        edje_object_part_swallow
           (wd->resize_obj,
            (efl_ui_mirrored_get(obj) ? "elm.swallow.arrow_right" :
             "elm.swallow.arrow_left"), sd->arrow);

        if (base_size.h > 0)
          {
             if (y <= ((arrow_size.h * 0.5) + base_size.y + shadow_left_top.h + arrow_padding.h))
               y = 0;
             else if (y >= (base_size.y + base_size.h - ((arrow_size.h * 0.5) + shadow_right_bottom.h + arrow_padding.h)))
               y = base_size.h - (arrow_size.h + shadow_right_bottom.h + shadow_left_top.h + (arrow_padding.h * 2));
             else
               y = y - base_size.y - ((arrow_size.h * 0.5) + shadow_left_top.h + arrow_padding.h);
             drag = (double)(y) / (double)(base_size.h - (arrow_size.h + shadow_right_bottom.h + shadow_left_top.h + (arrow_padding.h * 2)));
             edje_object_part_drag_value_set
                (wd->resize_obj,
                 (efl_ui_mirrored_get(obj) ? "elm.swallow.arrow_right" :
                  "elm.swallow.arrow_left"), 1, drag);
          }
        break;

      case ELM_CTXPOPUP_DIRECTION_LEFT:
        edje_object_signal_emit(sd->arrow, "elm,state,right", "elm");
        edje_object_part_swallow
           (wd->resize_obj,
            (efl_ui_mirrored_get(obj) ? "elm.swallow.arrow_left" :
             "elm.swallow.arrow_right"), sd->arrow);

        if (base_size.h > 0)
          {
             if (y <= ((arrow_size.h * 0.5) + base_size.y + shadow_left_top.h + arrow_padding.h))
               y = 0;
             else if (y >= (base_size.y + base_size.h - ((arrow_size.h * 0.5) + shadow_right_bottom.h + arrow_padding.h)))
               y = base_size.h - (arrow_size.h + shadow_right_bottom.h + shadow_left_top.h + (arrow_padding.h * 2));
             else
               y = y - base_size.y - ((arrow_size.h * 0.5) + shadow_left_top.h + arrow_padding.h);
             drag = (double)(y) / (double)(base_size.h - (arrow_size.h + shadow_right_bottom.h + shadow_left_top.h + (arrow_padding.h * 2)));
             edje_object_part_drag_value_set
                (wd->resize_obj,
                 (efl_ui_mirrored_get(obj) ? "elm.swallow.arrow_left" :
                  "elm.swallow.arrow_right"), 0, drag);
          }
        break;

      case ELM_CTXPOPUP_DIRECTION_DOWN:
        edje_object_signal_emit(sd->arrow, "elm,state,top", "elm");
        edje_object_part_swallow
          (wd->resize_obj, "elm.swallow.arrow_up",
          sd->arrow);

        if (base_size.w > 0)
          {
             if (x <= ((arrow_size.w * 0.5) + base_size.x + shadow_left_top.w + arrow_padding.w))
               x = 0;
             else if (x >= (base_size.x + base_size.w - ((arrow_size.w * 0.5) + shadow_right_bottom.w + arrow_padding.w)))
               x = base_size.w - (arrow_size.w + shadow_right_bottom.w + shadow_left_top.w + (arrow_padding.w * 2));
             else
               x = x - base_size.x - ((arrow_size.w * 0.5) + shadow_left_top.w + arrow_padding.w);
             drag = (double)(x) / (double)(base_size.w - (arrow_size.w + shadow_right_bottom.w + shadow_left_top.w + (arrow_padding.w * 2)));
             edje_object_part_drag_value_set
               (wd->resize_obj, "elm.swallow.arrow_up",
               drag, 1);
          }
        break;

      case ELM_CTXPOPUP_DIRECTION_UP:
        edje_object_signal_emit(sd->arrow, "elm,state,bottom", "elm");
        edje_object_part_swallow
          (wd->resize_obj, "elm.swallow.arrow_down",
          sd->arrow);

        if (base_size.w > 0)
          {
             if (x <= ((arrow_size.w * 0.5) + base_size.x + shadow_left_top.w + arrow_padding.w))
               x = 0;
             else if (x >= (base_size.x + base_size.w - ((arrow_size.w * 0.5) + shadow_right_bottom.w + arrow_padding.w)))
               x = base_size.w - (arrow_size.w + shadow_right_bottom.w + shadow_left_top.w + (arrow_padding.w * 2));
             else
               x = x - base_size.x - ((arrow_size.w * 0.5) + shadow_left_top.w + arrow_padding.w);
             drag = (double)(x) / (double)(base_size.w - (arrow_size.w + shadow_right_bottom.w + shadow_left_top.w + (arrow_padding.w * 2)));
             edje_object_part_drag_value_set
               (wd->resize_obj, "elm.swallow.arrow_down",
               drag, 0);
          }
        break;

      default:
        break;
     }

   //should be here for getting accurate geometry value
   evas_object_smart_calculate(wd->resize_obj);
}

static void
_show_signals_emit(Evas_Object *obj,
                   Elm_Ctxpopup_Direction dir)
{
   ELM_CTXPOPUP_DATA_GET(obj, sd);

   if (!sd->visible) return;

   switch (dir)
     {
      case ELM_CTXPOPUP_DIRECTION_UP:
        edje_object_signal_emit(sd->layout, "elm,state,show,up", "elm");
        break;

      case ELM_CTXPOPUP_DIRECTION_LEFT:
        edje_object_signal_emit(sd->layout, (efl_ui_mirrored_get(obj) ? "elm,state,show,right" :
               "elm,state,show,left"), "elm");
        break;

      case ELM_CTXPOPUP_DIRECTION_RIGHT:
        edje_object_signal_emit(sd->layout, (efl_ui_mirrored_get(obj) ? "elm,state,show,left" :
               "elm,state,show,right"), "elm");
        break;

      case ELM_CTXPOPUP_DIRECTION_DOWN:
        edje_object_signal_emit(sd->layout, "elm,state,show,down", "elm");
        break;

      default:
        break;
     }

   edje_object_signal_emit(sd->bg, "elm,state,show", "elm");
}

static void
_visible_signals_emit(Evas_Object *obj,
                   Elm_Ctxpopup_Direction dir)
{
   ELM_CTXPOPUP_DATA_GET(obj, sd);

   if (!sd->visible) return;

   switch (dir)
     {
      case ELM_CTXPOPUP_DIRECTION_UP:
        edje_object_signal_emit(sd->layout, "elm,state,visible,up", "elm");
        break;

      case ELM_CTXPOPUP_DIRECTION_LEFT:
        edje_object_signal_emit(sd->layout, (efl_ui_mirrored_get(obj) ? "elm,state,visible,right" :
               "elm,state,visible,left"), "elm");
        break;

      case ELM_CTXPOPUP_DIRECTION_RIGHT:
        edje_object_signal_emit(sd->layout, (efl_ui_mirrored_get(obj) ? "elm,state,visible,left" :
               "elm,state,visible,right"), "elm");
        break;

      case ELM_CTXPOPUP_DIRECTION_DOWN:
        edje_object_signal_emit(sd->layout, "elm,state,visible,down", "elm");
        break;

      default:
        break;
     }

   edje_object_signal_emit(sd->bg, "elm,state,visible", "elm");
}

static void
_hide_signals_emit(Evas_Object *obj,
                   Elm_Ctxpopup_Direction dir)
{
   ELM_CTXPOPUP_DATA_GET(obj, sd);

   if (!sd->visible) return;

   switch (dir)
     {
      case ELM_CTXPOPUP_DIRECTION_UP:
        edje_object_signal_emit(sd->layout, "elm,state,hide,up", "elm");
        break;

      case ELM_CTXPOPUP_DIRECTION_LEFT:
        edje_object_signal_emit(sd->layout, (efl_ui_mirrored_get(obj) ? "elm,state,hide,right" :
               "elm,state,hide,left"), "elm");
        break;

      case ELM_CTXPOPUP_DIRECTION_RIGHT:
        edje_object_signal_emit(sd->layout, (efl_ui_mirrored_get(obj) ? "elm,state,hide,left" :
               "elm,state,hide,right"), "elm");
        break;

      case ELM_CTXPOPUP_DIRECTION_DOWN:
        edje_object_signal_emit(sd->layout, "elm,state,hide,down", "elm");
        break;

      default:
        edje_object_signal_emit(sd->layout, "elm,state,hide,default", "elm");
        break;
     }

   edje_object_signal_emit(sd->bg, "elm,state,hide", "elm");
}

static void
_base_shift_by_arrow(Evas_Object *obj,
                     Evas_Object *arrow,
                     Elm_Ctxpopup_Direction dir,
                     Evas_Coord_Rectangle *rect)
{
   ELM_WIDGET_DATA_GET_OR_RETURN(obj, wd);

   Evas_Coord arrow_w, arrow_h, diff_w, diff_h;
   Evas_Coord_Rectangle shadow_left_top, shadow_right_bottom;

   evas_object_geometry_get(arrow, NULL, NULL, &arrow_w, &arrow_h);
   /* tizen only: since ctxpopup of tizen has shadow parts, start and end padding of arrow, it should be put together when shifting ctxpopup by arrow
    * so there are some differences between opensource and tizen*/
   edje_object_part_geometry_get(wd->resize_obj, "frame_shadow_left_top_padding", NULL, NULL, &shadow_left_top.w, &shadow_left_top.h);
   edje_object_part_geometry_get(wd->resize_obj, "frame_shadow_right_bottom_padding", NULL, NULL, &shadow_right_bottom.w, &shadow_right_bottom.h);
   //

   switch (dir)
     {
      case ELM_CTXPOPUP_DIRECTION_RIGHT:
        diff_w = arrow_w - shadow_right_bottom.w;
        rect->x += diff_w;
        break;

      case ELM_CTXPOPUP_DIRECTION_LEFT:
        diff_w = arrow_w - shadow_left_top.w;
        rect->x -= diff_w;
        break;

      case ELM_CTXPOPUP_DIRECTION_DOWN:
        diff_h = arrow_h - shadow_left_top.h;
        rect->y += diff_h;
        break;

      case ELM_CTXPOPUP_DIRECTION_UP:
        diff_h = arrow_h - shadow_right_bottom.h;
        rect->y -= diff_h;
        break;

      default:
         break;
     }
}

/* disable temporarily
EOLIAN static Eina_Bool
_elm_ctxpopup_elm_layout_sub_object_add_enable(Eo *obj EINA_UNUSED, Elm_Ctxpopup_Data *_pd EINA_UNUSED)
{
   return EINA_FALSE;
}
*/

EOLIAN static Eina_Bool
_elm_ctxpopup_efl_ui_widget_widget_sub_object_add(Eo *obj, Elm_Ctxpopup_Data *_pd EINA_UNUSED, Evas_Object *sobj)
{
   return efl_ui_widget_sub_object_add(efl_cast(obj, EFL_UI_WIDGET_CLASS), sobj);
}

EOLIAN static void
_elm_ctxpopup_elm_layout_sizing_eval(Eo *obj, Elm_Ctxpopup_Data *sd)
{
   Eina_List *elist;
   Elm_Ctxpopup_Item_Data *item;
   Evas_Coord_Rectangle rect = { 0, 0, 1, 1 };
   Evas_Coord_Point box_size = { 0, 0 };
   Evas_Coord_Point _box_size = { 0, 0 };
   Evas_Coord x, y, w, h;

   ELM_WIDGET_DATA_GET_OR_RETURN(obj, wd);

   if (!sd->parent || !(sd->items || sd->content)) return;

   //Box, Scroller
   EINA_LIST_FOREACH(sd->items, elist, item)
     {
        _item_sizing_eval(item);
        evas_object_size_hint_min_get(VIEW(item), &_box_size.x, &_box_size.y);

        /* Disable item size fix by edc data
        str = edje_object_data_get(VIEW(item), "item_max_size");
        if (str)
          {
             maxw = atoi(str);
             maxw = maxw * elm_config_scale_get()
                             * elm_widget_scale_get(obj)
                             / edje_object_base_scale_get(VIEW(item));

             if (_box_size.x > maxw)
               {
                  edje_object_signal_emit(VIEW(item), "elm,state,text,ellipsis", "elm");
                  edje_object_message_signal_process(VIEW(item));
               }
          }
        */

        if (!sd->horizontal)
          {
             if (_box_size.x > box_size.x)
               box_size.x = _box_size.x;
             if (_box_size.y != -1)
               box_size.y += _box_size.y;
          }
        else
          {
             if (_box_size.x != -1)
               box_size.x += _box_size.x;
             if (_box_size.y > box_size.y)
               box_size.y = _box_size.y;
          }
     }

//   if (!sd->arrow) return;  /* simple way to flag "under deletion" */

   if ((!sd->content) && (sd->scr))
     {
        evas_object_size_hint_min_set(sd->box, box_size.x, box_size.y);
        elm_scroller_content_min_limit(sd->scr, EINA_TRUE, EINA_TRUE);
        evas_object_size_hint_min_set(sd->scr, box_size.x, box_size.y);
     }

   //Base
   sd->dir = _base_geometry_calc(obj, &rect);

   _arrow_update(obj, sd->dir, rect);
   _base_shift_by_arrow(obj, sd->arrow, sd->dir, &rect);

   //resize scroller according to final size
   if ((!sd->content) && (sd->scr))
     {
        elm_scroller_content_min_limit(sd->scr, EINA_FALSE, EINA_FALSE);
        evas_object_smart_calculate(sd->scr);
     }

   evas_object_resize(obj, rect.w, rect.h);
   evas_object_size_hint_min_set(wd->resize_obj, rect.w, rect.h);

   evas_object_resize(sd->layout, rect.w, rect.h);
   evas_object_move(sd->layout, rect.x, rect.y);

   evas_object_geometry_get(sd->parent, &x, &y, &w, &h);
   evas_object_move(sd->bg, x, y);
   evas_object_resize(sd->bg, w, h);

   // resize item and set ellipsis state (only vertical mode)
   if (!sd->horizontal)
     {
        edje_object_part_geometry_get(wd->resize_obj, "elm.swallow.content", NULL, NULL, &w, NULL);

        EINA_LIST_FOREACH(sd->items, elist, item)
          {
             evas_object_size_hint_min_get(VIEW(item), &_box_size.x, &_box_size.y);

             if (_box_size.x > w)
               {
                  evas_object_size_hint_min_set(VIEW(item), w, _box_size.y);
                  edje_object_signal_emit(VIEW(item), "elm,state,text,ellipsis", "elm");
                  edje_object_message_signal_process(VIEW(item));
               }
          }
     }

   if (sd->visible)
     {
        if (sd->show_finished)
          _visible_signals_emit(obj, sd->dir);
        else
          _show_signals_emit(obj, sd->dir);
     }
}

static void
_on_parent_del(void *data,
               Evas *e EINA_UNUSED,
               Evas_Object *obj EINA_UNUSED,
               void *event_info EINA_UNUSED)
{
   evas_object_del(data);
}

static void
_on_parent_move(void *data,
                Evas *e EINA_UNUSED,
                Evas_Object *obj EINA_UNUSED,
                void *event_info EINA_UNUSED)
{
   ELM_CTXPOPUP_DATA_GET(data, sd);


   sd->dir = ELM_CTXPOPUP_DIRECTION_UNKNOWN;

   if (sd->visible)
     elm_layout_sizing_eval(data);
}

static void
_on_parent_resize(void *data,
                  Evas *e EINA_UNUSED,
                  Evas_Object *obj EINA_UNUSED,
                  void *event_info EINA_UNUSED)
{
   ELM_CTXPOPUP_DATA_GET(data, sd);
   ELM_WIDGET_DATA_GET_OR_RETURN(data, wd);

   if (sd->auto_hide)
     {
        sd->dir = ELM_CTXPOPUP_DIRECTION_UNKNOWN;

        evas_object_hide(data);
        efl_event_callback_legacy_call(data, ELM_CTXPOPUP_EVENT_DISMISSED, NULL);
     }
   else
     {
        if (wd->orient_mode == 90 || wd->orient_mode == 270)
         elm_widget_theme_object_set
           (data, sd->layout, "ctxpopup", "layout/landscape", elm_widget_style_get(data));
        else
         elm_widget_theme_object_set
           (data, sd->layout, "ctxpopup", "layout", elm_widget_style_get(data));
        edje_object_part_swallow(sd->layout, "swallow", wd->resize_obj);

        if (sd->visible)
          elm_layout_sizing_eval(data);

        /* TIZEN_ONLY(20170123): to avoid flickering problem when rotate device
        _show_signals_emit(data, sd->dir);
        */
     }
}

static void
_parent_detach(Evas_Object *obj)
{
   ELM_CTXPOPUP_DATA_GET(obj, sd);

   if (!sd->parent) return;

   evas_object_event_callback_del_full
     (sd->parent, EVAS_CALLBACK_DEL, _on_parent_del, obj);
   evas_object_event_callback_del_full
     (sd->parent, EVAS_CALLBACK_MOVE, _on_parent_move, obj);
   evas_object_event_callback_del_full
     (sd->parent, EVAS_CALLBACK_RESIZE, _on_parent_resize, obj);
}

static void
_on_content_resized(void *data,
                    Evas *e EINA_UNUSED,
                    Evas_Object *obj EINA_UNUSED,
                    void *event_info EINA_UNUSED)
{
   elm_layout_sizing_eval(data);
}

static void
_access_outline_activate_cb(void *data,
                        Evas_Object *part_obj EINA_UNUSED,
                        Elm_Object_Item *item EINA_UNUSED)
{
   evas_object_hide(data);
   efl_event_callback_legacy_call(data, ELM_CTXPOPUP_EVENT_DISMISSED, NULL);
}

static void
_access_obj_process(Evas_Object *obj, Eina_Bool is_access)
{
   Evas_Object *ao;
   ELM_WIDGET_DATA_GET_OR_RETURN(obj, wd);

   if (is_access)
     {
        ao = _access_object_get(obj, ACCESS_OUTLINE_PART);
        if (!ao)
          {
             ao = _elm_access_edje_object_part_object_register
                (obj, wd->resize_obj, ACCESS_OUTLINE_PART);

             const char *style = elm_widget_style_get(obj);
             if (!strcmp(style, "more/default"))
               {
                  elm_access_info_set(ao, ELM_ACCESS_TYPE, TTS_STR_MENU_POPUP);
                  elm_access_info_set(ao, ELM_ACCESS_CONTEXT_INFO, TTS_STR_MENU_CLOSE);
               }
             else
               {
                  elm_access_info_set(ao, ELM_ACCESS_TYPE, E_("Contextual popup"));
                  elm_access_info_set(ao, ELM_ACCESS_CONTEXT_INFO, E_("Double tap to close popup"));
               }
             _elm_access_activate_callback_set
                (_elm_access_info_get(ao), _access_outline_activate_cb, obj);
          }
     }
   else
     {
        _elm_access_edje_object_part_object_unregister
               (obj, wd->resize_obj, ACCESS_OUTLINE_PART);
     }
}

static void
_mirrored_set(Evas_Object *obj, Eina_Bool rtl)
{
   ELM_CTXPOPUP_DATA_GET(obj, sd);
   ELM_WIDGET_DATA_GET_OR_RETURN(obj, wd);

   edje_object_mirrored_set(sd->layout, rtl);
   edje_object_mirrored_set(sd->arrow, rtl);
   edje_object_mirrored_set(wd->resize_obj, rtl);
}

EOLIAN static Eina_Bool
_elm_ctxpopup_efl_ui_widget_event(Eo *obj, Elm_Ctxpopup_Data *sd, Evas_Object *src EINA_UNUSED, Evas_Callback_Type type, void *event_info)
{
   Evas_Event_Key_Down *ev = event_info;

   // TIZEN ONLY(20131221) : When access mode, focused ui is disabled.
   if (_elm_config->access_mode) return EINA_FALSE;

   if (elm_widget_disabled_get(obj)) return EINA_FALSE;
   if (type != EVAS_CALLBACK_KEY_DOWN) return EINA_FALSE;
   if (ev->event_flags & EVAS_EVENT_FLAG_ON_HOLD) return EINA_FALSE;

   //FIXME: for this key event, _elm_ctxpopup_smart_focus_next should be done first
   if ((!strcmp(ev->key, "Tab")) ||
       (!strcmp(ev->key, "ISO_Left_Tab")))
     {
        Eina_Bool ret;
        Evas_Object *target = NULL;
        Elm_Object_Item *target_item = NULL;
        Elm_Focus_Direction dir;

        if (evas_key_modifier_is_set(ev->modifiers, "Control") ||
            evas_key_modifier_is_set(ev->modifiers, "Alt"))
          return EINA_FALSE;
        if (evas_key_modifier_is_set(ev->modifiers, "Shift"))
          dir = ELM_FOCUS_PREVIOUS;
        else
          dir = ELM_FOCUS_NEXT;

        ret = efl_ui_widget_focus_next_get(sd->box, dir, &target, &target_item);
        if (ret) efl_ui_widget_focus_steal(target, target_item);

        ev->event_flags |= EVAS_EVENT_FLAG_ON_HOLD;
        return EINA_TRUE;
     }
   /////
   if (((!strcmp(ev->key, "Left")) ||
        (!strcmp(ev->key, "KP_Left")) ||
        (!strcmp(ev->key, "Right")) ||
        (!strcmp(ev->key, "KP_Right")) ||
        (!strcmp(ev->key, "Up")) ||
        (!strcmp(ev->key, "KP_Up")) ||
        (!strcmp(ev->key, "Down")) ||
        (!strcmp(ev->key, "KP_Down"))) && (!ev->string))
     {
        double degree = 0.0;

        if ((!strcmp(ev->key, "Left")) ||
            (!strcmp(ev->key, "KP_Left")))
          degree = 270.0;
        else if ((!strcmp(ev->key, "Right")) ||
                 (!strcmp(ev->key, "KP_Right")))
          degree = 90.0;
        else if ((!strcmp(ev->key, "Up")) ||
                 (!strcmp(ev->key, "KP_Up")))
          degree = 0.0;
        else if ((!strcmp(ev->key, "Down")) ||
                 (!strcmp(ev->key, "KP_Down")))
          degree = 180.0;

        efl_ui_widget_focus_direction_go(sd->box, degree);
        ev->event_flags |= EVAS_EVENT_FLAG_ON_HOLD;
        return EINA_TRUE;
     }
   else if (((!strcmp(ev->key, "Home")) ||
             (!strcmp(ev->key, "KP_Home")) ||
             (!strcmp(ev->key, "Prior")) ||
             (!strcmp(ev->key, "KP_Prior"))) && (!ev->string))
     {
        Elm_Ctxpopup_Item_Data *it = eina_list_data_get(sd->items);
        Evas_Object *btn = it->btn;
        elm_object_focus_set(btn, EINA_TRUE);
        ev->event_flags |= EVAS_EVENT_FLAG_ON_HOLD;
        return EINA_TRUE;
     }
   else if (((!strcmp(ev->key, "End")) ||
             (!strcmp(ev->key, "KP_End")) ||
             (!strcmp(ev->key, "Next")) ||
             (!strcmp(ev->key, "KP_Next"))) && (!ev->string))
     {
        Elm_Ctxpopup_Item_Data *it = eina_list_data_get(eina_list_last(sd->items));
        Evas_Object *btn = it->btn;
        elm_object_focus_set(btn, EINA_TRUE);
        ev->event_flags |= EVAS_EVENT_FLAG_ON_HOLD;
        return EINA_TRUE;
     }

   // TIZEN ONLY : 20130530 : ctxpopup will be dismissed by user
   //if (strcmp(ev->keyname, "Escape")) return EINA_FALSE;
   return EINA_FALSE;

/*
   _hide_signals_emit(obj, sd->dir);

   ev->event_flags |= EVAS_EVENT_FLAG_ON_HOLD;
   return EINA_TRUE;
*/
}

//FIXME: lost the content size when theme hook is called.
EOLIAN static Efl_Ui_Theme_Apply_Result
_elm_ctxpopup_efl_ui_widget_theme_apply(Eo *obj, Elm_Ctxpopup_Data *sd)
{
   Eina_List *elist;
   Elm_Ctxpopup_Item_Data *item;
   int idx = 0;
   Eina_Bool rtl;
   Efl_Ui_Theme_Apply_Result int_ret = EFL_UI_THEME_APPLY_RESULT_FAIL;
   const char *style;

   ELM_WIDGET_DATA_GET_OR_RETURN(obj, wd, EFL_UI_THEME_APPLY_RESULT_FAIL);

   int_ret = efl_ui_widget_theme_apply(efl_super(obj, MY_CLASS));
   if (!int_ret) return EFL_UI_THEME_APPLY_RESULT_FAIL;

   rtl = efl_ui_mirrored_get(obj);

   elm_widget_theme_object_set
     (obj, sd->bg, "ctxpopup", "bg", elm_widget_style_get(obj));

   elm_widget_theme_object_set
     (obj, sd->arrow, "ctxpopup", "arrow", elm_widget_style_get(obj));

   if (wd->orient_mode == 90 || wd->orient_mode == 270)
     elm_widget_theme_object_set
       (obj, sd->layout, "ctxpopup", "layout/landscape", elm_widget_style_get(obj));
   else
     elm_widget_theme_object_set
       (obj, sd->layout, "ctxpopup", "layout", elm_widget_style_get(obj));
   edje_object_part_swallow(sd->layout, "swallow", wd->resize_obj);

   _mirrored_set(obj, rtl);

   //Items
   EINA_LIST_FOREACH(sd->items, elist, item)
     {
        edje_object_mirrored_set(VIEW(item), rtl);

        style = (item->style ? item->style : elm_widget_style_get(obj));

        if (item->label && item->icon)
          {
             if (!sd->horizontal)
               _elm_theme_object_set(obj, VIEW(item), "ctxpopup",
                                     "icon_text_style_item",
                                     style);
             else
               _elm_theme_object_set(obj, VIEW(item), "ctxpopup",
                                     "icon_text_style_item_horizontal",
                                     style);
          }
        else if (item->label)
          {
             if (!sd->horizontal)
               _elm_theme_object_set(obj, VIEW(item), "ctxpopup",
                                     "text_style_item",
                                     style);
             else
               _elm_theme_object_set(obj, VIEW(item), "ctxpopup",
                                     "text_style_item_horizontal",
                                     style);
          }
        else
          {
             if (!sd->horizontal)
               _elm_theme_object_set(obj, VIEW(item), "ctxpopup",
                                     "icon_style_item",
                                     style);
             else
               _elm_theme_object_set(obj, VIEW(item), "ctxpopup",
                                     "icon_style_item_horizontal",
                                     style);
          }

        if (item->label)
          edje_object_part_text_set(VIEW(item), "elm.text", item->label);

        if (elm_wdg_item_disabled_get(EO_OBJ(item)))
          edje_object_signal_emit(VIEW(item), "elm,state,disabled", "elm");

       /*
        *  For separator, if the first item has visible separator,
        *  then it should be aligned with edge of the base part.
        *  In some cases, it gives improper display. Ex) rounded corner
        *  So the first item separator should be invisible.
        */
        if ((idx++) == 0)
          edje_object_signal_emit(VIEW(item), "elm,state,default", "elm");
        else
          edje_object_signal_emit(VIEW(item), "elm,state,separator", "elm");

        // reset state of text to be default
        edje_object_signal_emit(VIEW(item), "elm,state,text,default", "elm");
        edje_object_message_signal_process(VIEW(item));
     }

   if (sd->scr)
     {
        if (!elm_layout_theme_set(sd->scr, "scroller", "ctxpopup", elm_widget_style_get(obj)))
          CRI("Failed to set layout!");

        if (sd->horizontal)
          elm_scroller_policy_set(sd->scr, ELM_SCROLLER_POLICY_AUTO, ELM_SCROLLER_POLICY_OFF);
        else
          elm_scroller_policy_set(sd->scr, ELM_SCROLLER_POLICY_OFF, ELM_SCROLLER_POLICY_AUTO);
     }

   sd->dir = ELM_CTXPOPUP_DIRECTION_UNKNOWN;

   if (sd->visible)
     elm_layout_sizing_eval(obj);

   /* access */
  if (_elm_config->access_mode) _access_obj_process(obj, EINA_TRUE);

   return int_ret;
}

/* kind of a big and tricky override here: an internal box will hold
 * the actual content. content aliases won't be of much help here */
EOLIAN static Eina_Bool
_elm_ctxpopup_content_set(Eo *obj, Elm_Ctxpopup_Data *sd, const char *part, Evas_Object *content)
{
   Evas_Coord min_w = -1, min_h = -1;

   if ((part) && (strcmp(part, "elm.swallow.content")))
     {
        return efl_content_set(efl_part(efl_super(obj, MY_CLASS), part), content);
     }

   if (!content) return EINA_FALSE;

   if (content == sd->content) return EINA_TRUE;

   if (sd->items) elm_ctxpopup_clear(obj);
   if (sd->content) evas_object_del(sd->content);

   ELM_WIDGET_DATA_GET_OR_RETURN(obj, wd, EINA_FALSE);

   evas_object_event_callback_add
      (sd->box, EVAS_CALLBACK_RESIZE, _on_content_resized, obj);
   edje_object_part_swallow(wd->resize_obj, "elm.swallow.content", sd->box);

   evas_object_size_hint_weight_set
     (content, EVAS_HINT_EXPAND, EVAS_HINT_EXPAND);
   evas_object_size_hint_fill_set
     (content, EVAS_HINT_FILL, EVAS_HINT_FILL);

   /* since it's going to be a box content, not a layout's... */
   evas_object_show(content);

   evas_object_size_hint_min_get(content, &min_w, &min_h);
   evas_object_size_hint_min_set(sd->box, min_w, min_h);
   elm_box_pack_end(sd->box, content);

   sd->content = content;
   sd->dir = ELM_CTXPOPUP_DIRECTION_UNKNOWN;

   if (sd->visible) elm_layout_sizing_eval(obj);

   return EINA_TRUE;
}

static Evas_Object*
_elm_ctxpopup_content_get(Eo *obj, Elm_Ctxpopup_Data *sd, const char *part)
{

   if ((part) && (strcmp(part, "elm.swallow.content")))
        return efl_content_get(efl_part(efl_super(obj, MY_CLASS), part));

   return sd->content;
}

static Evas_Object*
_elm_ctxpopup_content_unset(Eo *obj, Elm_Ctxpopup_Data *sd, const char *part)
{
   Evas_Object *content = NULL;

   if ((part) && (strcmp(part, "elm.swallow.content")))
        return efl_content_unset(efl_part(efl_super(obj, MY_CLASS), part));

   content = sd->content;
   if (!content) return content;

   elm_box_unpack(sd->box, content);
   sd->content = NULL;
   sd->dir = ELM_CTXPOPUP_DIRECTION_UNKNOWN;

   if (sd->visible) elm_layout_sizing_eval(obj);

   return content;
}

EOLIAN static void
_elm_ctxpopup_item_elm_widget_item_part_text_set(Eo *eo_ctxpopup_it EINA_UNUSED,
                                                 Elm_Ctxpopup_Item_Data *ctxpopup_it,
                                                 const char *part,
                                                 const char *label)
{
   if (!ctxpopup_it) return;
   if (ctxpopup_it->label == label) return;
   if ((part) && (strcmp(part, "default"))) return;

   ELM_CTXPOPUP_DATA_GET(WIDGET(ctxpopup_it), sd);

   const char *style = (ctxpopup_it->style ?
                        ctxpopup_it->style :
                        elm_widget_style_get(WIDGET(ctxpopup_it)));

   if (ctxpopup_it->label && !label)
     {
        if (!sd->horizontal)
          _elm_theme_object_set(WIDGET(ctxpopup_it), VIEW(ctxpopup_it), "ctxpopup",
                                "icon_style_item",
                                style);
        else
          _elm_theme_object_set(WIDGET(ctxpopup_it), VIEW(ctxpopup_it), "ctxpopup",
                                "icon_style_item_horizontal",
                                style);
     }
   else if (!ctxpopup_it->label && label)
     {
        if (!ctxpopup_it->icon)
          {
             if (!sd->horizontal)
               _elm_theme_object_set(WIDGET(ctxpopup_it), VIEW(ctxpopup_it), "ctxpopup",
                                     "text_style_item",
                                     style);
             else
               _elm_theme_object_set(WIDGET(ctxpopup_it), VIEW(ctxpopup_it), "ctxpopup",
                                     "text_style_item_horizontal",
                                     style);
          }
        else
          {
             if (!sd->horizontal)
               _elm_theme_object_set(WIDGET(ctxpopup_it), VIEW(ctxpopup_it), "ctxpopup",
                                    "icon_text_style_item",
                                     style);
             else
               _elm_theme_object_set(WIDGET(ctxpopup_it), VIEW(ctxpopup_it), "ctxpopup",
                                     "icon_text_style_item_horizontal",
                                     style);
          }
     }

   eina_stringshare_replace(&ctxpopup_it->label, label);
   if (ctxpopup_it->label)
     edje_object_part_text_set(VIEW(ctxpopup_it), "elm.text", label);

   sd->dir = ELM_CTXPOPUP_DIRECTION_UNKNOWN;

   if (sd->visible)
     elm_layout_sizing_eval(WIDGET(ctxpopup_it));
}

EOLIAN static const char *
_elm_ctxpopup_item_elm_widget_item_part_text_get(const Eo *eo_ctxpopup_it EINA_UNUSED,
                                                 Elm_Ctxpopup_Item_Data *ctxpopup_it,
                                                 const char *part)
{
   if (part && strcmp(part, "default")) return NULL;

   return ctxpopup_it->label;
}

EOLIAN static void
_elm_ctxpopup_item_elm_widget_item_part_content_set(Eo *eo_ctxpopup_it EINA_UNUSED,
                                                    Elm_Ctxpopup_Item_Data *ctxpopup_it,
                                                    const char *part,
                                                    Evas_Object *content)
{
   if (!ctxpopup_it) return;
   if (ctxpopup_it->icon == content) return;
   if ((part) && (strcmp(part, "icon"))) return;

   ELM_CTXPOPUP_DATA_GET(WIDGET(ctxpopup_it), sd);

   const char *style = (ctxpopup_it->style ?
                        ctxpopup_it->style :
                        elm_widget_style_get(WIDGET(ctxpopup_it)));

   if (ctxpopup_it->icon)
     evas_object_del(ctxpopup_it->icon);

   if (ctxpopup_it->label)
     {
        if (!ctxpopup_it->icon && content)
          {
             if (!sd->horizontal)
               _elm_theme_object_set(WIDGET(ctxpopup_it), VIEW(ctxpopup_it), "ctxpopup",
                                     "icon_text_style_item",
                                     style);
             else
               _elm_theme_object_set(WIDGET(ctxpopup_it), VIEW(ctxpopup_it), "ctxpopup",
                                     "icon_text_style_item_horizontal",
                                     style);
          }
        else if (ctxpopup_it->icon && !content)
          {
             if (!sd->horizontal)
               _elm_theme_object_set(WIDGET(ctxpopup_it), VIEW(ctxpopup_it), "ctxpopup",
                                     "text_style_item",
                                     style);
             else
               _elm_theme_object_set(WIDGET(ctxpopup_it), VIEW(ctxpopup_it), "ctxpopup",
                                     "text_style_item_horizontal",
                                     style);
          }
     }

   ctxpopup_it->icon = content;
   if (ctxpopup_it->icon)
     edje_object_part_swallow(VIEW(ctxpopup_it), "elm.swallow.icon", ctxpopup_it->icon);

   sd->dir = ELM_CTXPOPUP_DIRECTION_UNKNOWN;

   if (sd->visible)
     elm_layout_sizing_eval(WIDGET(ctxpopup_it));
}

EOLIAN static Evas_Object *
_elm_ctxpopup_item_elm_widget_item_part_content_get(const Eo *eo_ctxpopup_it EINA_UNUSED,
                                                    Elm_Ctxpopup_Item_Data *ctxpopup_it,
                                                    const char *part)
{
   if (part && strcmp(part, "icon")) return NULL;

   return ctxpopup_it->icon;
}

EOLIAN static Evas_Object *
_elm_ctxpopup_item_elm_widget_item_part_content_unset(Eo *eo_ctxpopup_it EINA_UNUSED,
                                                      Elm_Ctxpopup_Item_Data *ctxpopup_it,
                                                      const char *part)
{
   Evas_Object *content = NULL;

   if (!ctxpopup_it || !ctxpopup_it->icon) return NULL;
   if ((part) && (strcmp(part, "icon"))) return NULL;

   ELM_CTXPOPUP_DATA_GET(WIDGET(ctxpopup_it), sd);

   const char *style = (ctxpopup_it->style ?
                        ctxpopup_it->style :
                        elm_widget_style_get(WIDGET(ctxpopup_it)));

   edje_object_part_unswallow(VIEW(ctxpopup_it), ctxpopup_it->icon);
   evas_object_hide(ctxpopup_it->icon);

   if (ctxpopup_it->label)
     {
        if (!sd->horizontal)
          _elm_theme_object_set(WIDGET(ctxpopup_it), VIEW(ctxpopup_it), "ctxpopup",
                                "text_style_item",
                                style);
        else
          _elm_theme_object_set(WIDGET(ctxpopup_it), VIEW(ctxpopup_it), "ctxpopup",
                                "text_style_item_horizontal",
                                style);
     }

   content = ctxpopup_it->icon;
   ctxpopup_it->icon = NULL;

   sd->dir = ELM_CTXPOPUP_DIRECTION_UNKNOWN;

   if (sd->visible)
     elm_layout_sizing_eval(WIDGET(ctxpopup_it));

   return content;
}

EOLIAN static void
_elm_ctxpopup_item_elm_widget_item_disable(Eo *eo_ctxpopup_it EINA_UNUSED,
                                           Elm_Ctxpopup_Item_Data *ctxpopup_it)
{
   ELM_CTXPOPUP_DATA_GET(WIDGET(ctxpopup_it), sd);
   if (!sd) return;

   if (elm_wdg_item_disabled_get(EO_OBJ(ctxpopup_it)))
     edje_object_signal_emit(VIEW(ctxpopup_it), "elm,state,disabled", "elm");
   else
     edje_object_signal_emit(VIEW(ctxpopup_it), "elm,state,enabled", "elm");
}

EOLIAN static void
_elm_ctxpopup_item_elm_widget_item_signal_emit(Eo *eo_ctxpopup_it EINA_UNUSED,
                                               Elm_Ctxpopup_Item_Data *ctxpopup_it,
                                               const char *emission,
                                               const char *source)
{
   edje_object_signal_emit(VIEW(ctxpopup_it), emission, source);
}

EOLIAN static void
_elm_ctxpopup_item_elm_widget_item_style_set(Eo *eo_item EINA_UNUSED,
                                             Elm_Ctxpopup_Item_Data *item,
                                             const char *style)
{
   ELM_CTXPOPUP_DATA_GET(WIDGET(item), sd);

   eina_stringshare_replace(&item->style, style);
   if (item->icon && item->label)
     {
        if (sd->horizontal)
          _elm_theme_object_set(WIDGET(item), VIEW(item), "ctxpopup", "icon_text_style_item_horizontal", style);
        else
          _elm_theme_object_set(WIDGET(item), VIEW(item), "ctxpopup", "icon_text_style_item", style);
     }
   else if (item->label)
     {
        if (sd->horizontal)
          _elm_theme_object_set(WIDGET(item), VIEW(item), "ctxpopup", "text_style_item_horizontal", style);
        else
          _elm_theme_object_set(WIDGET(item), VIEW(item), "ctxpopup", "text_style_item", style);
     }
   else
     {
        if (sd->horizontal)
          _elm_theme_object_set(WIDGET(item), VIEW(item), "ctxpopup", "icon_style_item_horizontal", style);
        else
          _elm_theme_object_set(WIDGET(item), VIEW(item), "ctxpopup", "icon_style_item", style);
     }

   if (sd->visible) elm_layout_sizing_eval(WIDGET(item));
}

EOLIAN static const char *
_elm_ctxpopup_item_elm_widget_item_style_get(const Eo *eo_it EINA_UNUSED,
                                             Elm_Ctxpopup_Item_Data *item)
{
   return item->style;
}

static void
_bg_clicked_cb(void *data,
               Evas_Object *obj EINA_UNUSED,
               const char *emission EINA_UNUSED,
               const char *source EINA_UNUSED)
{
   ELM_CTXPOPUP_DATA_GET(data, sd);

   _hide_signals_emit(data, sd->dir);
}

static void
_on_show(void *data EINA_UNUSED,
         Evas *e EINA_UNUSED,
         Evas_Object *obj,
         void *event_info EINA_UNUSED)
{
   Eina_List *elist;
   Elm_Ctxpopup_Item_Data *item;
   int idx = 0;
   const char *style;

   ELM_CTXPOPUP_DATA_GET(obj, sd);
   ELM_WIDGET_DATA_GET_OR_RETURN(obj, wd);

   sd->visible = EINA_TRUE;
   sd->show_finished = EINA_FALSE;
   //TIZEN_ONLY(20170919): Handle default label object
   _elm_win_default_label_obj_append(obj);
   //

   elm_layout_signal_emit(obj, "elm,state,show", "elm");

   if (wd->orient_mode == 90 || wd->orient_mode == 270)
     elm_widget_theme_object_set
       (obj, sd->layout, "ctxpopup", "layout/landscape", elm_widget_style_get(obj));
   else
     elm_widget_theme_object_set
       (obj, sd->layout, "ctxpopup", "layout", elm_widget_style_get(obj));
   edje_object_part_swallow(sd->layout, "swallow", wd->resize_obj);

   EINA_LIST_FOREACH(sd->items, elist, item)
     {
        style = (item->style ? item->style : elm_widget_style_get(obj));

        if (item->label && item->icon)
          {
             if (!sd->horizontal)
               _elm_theme_object_set(obj, VIEW(item), "ctxpopup",
                                     "icon_text_style_item",
                                     style);
             else
               _elm_theme_object_set(obj, VIEW(item), "ctxpopup",
                                     "icon_text_style_item_horizontal",
                                     style);
          }
        else if (item->label)
          {
             if (!sd->horizontal)
               _elm_theme_object_set(obj, VIEW(item), "ctxpopup",
                                     "text_style_item",
                                     style);
             else
               _elm_theme_object_set(obj, VIEW(item), "ctxpopup",
                                     "text_style_item_horizontal",
                                     style);
          }
        else
          {
             if (!sd->horizontal)
               _elm_theme_object_set(obj, VIEW(item), "ctxpopup",
                                     "icon_style_item",
                                     style);
             else
               _elm_theme_object_set(obj, VIEW(item), "ctxpopup",
                                     "icon_style_item_horizontal",
                                     style);
          }

        if (idx++ == 0)
          edje_object_signal_emit(VIEW(item), "elm,state,default", "elm");
        else
          edje_object_signal_emit(VIEW(item), "elm,state,separator", "elm");
     }

   elm_layout_sizing_eval(obj);

   elm_object_focus_set(obj, EINA_TRUE);
}

EOLIAN static void
_elm_ctxpopup_item_elm_widget_item_item_focus_set(Eo *eo_ctxpopup_it EINA_UNUSED,
                                             Elm_Ctxpopup_Item_Data *ctxpopup_it,
                                             Eina_Bool focused)
{
   elm_object_item_focus_set(ctxpopup_it->btn, focused);
}

EOLIAN static Eina_Bool
_elm_ctxpopup_item_elm_widget_item_item_focus_get(const Eo *eo_ctxpopup_it EINA_UNUSED,
                                             Elm_Ctxpopup_Item_Data *ctxpopup_it)
{
   return elm_object_item_focus_get(ctxpopup_it->btn);
}

static void
_on_hide(void *data EINA_UNUSED,
         Evas *e EINA_UNUSED,
         Evas_Object *obj,
         void *event_info EINA_UNUSED)
{
   ELM_CTXPOPUP_DATA_GET(obj, sd);

   if (!sd->visible) return;

   sd->visible = EINA_FALSE;
   //TIZEN_ONLY(20170919): Handle default label object
   _elm_win_default_label_obj_remove(obj);
   //
}

static void
_on_move(void *data EINA_UNUSED,
         Evas *e EINA_UNUSED,
         Evas_Object *obj,
         void *event_info EINA_UNUSED)
{
   ELM_CTXPOPUP_DATA_GET(obj, sd);

   if (sd->visible) elm_layout_sizing_eval(obj);
}

static void
_size_hints_changed_cb(void *data EINA_UNUSED,
                       Evas *e EINA_UNUSED,
                       Evas_Object *obj,
                       void *event_info EINA_UNUSED)
{
   ELM_CTXPOPUP_DATA_GET(obj, sd);

   if (sd->visible) elm_layout_sizing_eval(obj);
}

static void
_hide_finished_cb(void *data,
                  Evas_Object *obj EINA_UNUSED,
                  const char *emission EINA_UNUSED,
                  const char *source EINA_UNUSED)
{
   evas_object_hide(data);
   efl_event_callback_legacy_call(data, ELM_CTXPOPUP_EVENT_DISMISSED, NULL);
}

/* TIZEN_ONLY(20161207): add show finished callback */
static void
_show_finished_cb(void *data,
                  Evas_Object *obj EINA_UNUSED,
                  const char *emission EINA_UNUSED,
                  const char *source EINA_UNUSED)
{
   ELM_CTXPOPUP_DATA_GET(data, sd);
   sd->show_finished = EINA_TRUE;
   elm_object_focus_set(data, EINA_TRUE);
}
/* END */

static void
_list_del(Eo *obj, Elm_Ctxpopup_Data *sd)
{
   if (!sd->scr) return;

   ELM_WIDGET_DATA_GET_OR_RETURN(obj, wd);

   edje_object_part_unswallow(wd->resize_obj, sd->scr);
   evas_object_del(sd->scr);
   sd->scr = NULL;
   evas_object_del(sd->box);
   sd->box = NULL;
}

static void
_list_new(Evas_Object *obj)
{
   ELM_CTXPOPUP_DATA_GET(obj, sd);
   if (!sd) return;

   ELM_WIDGET_DATA_GET_OR_RETURN(obj, wd);
   //scroller
   sd->scr = elm_scroller_add(obj);
   if (!elm_layout_theme_set(sd->scr, "scroller", "ctxpopup", elm_widget_style_get(obj)))
     CRI("Failed to set layout!");
   evas_object_size_hint_align_set(sd->scr, EVAS_HINT_FILL, EVAS_HINT_FILL);
   _elm_widget_color_class_parent_set(sd->scr, obj);

   if (sd->horizontal)
     elm_scroller_policy_set(sd->scr, ELM_SCROLLER_POLICY_AUTO, ELM_SCROLLER_POLICY_OFF);
   else
     elm_scroller_policy_set(sd->scr, ELM_SCROLLER_POLICY_OFF, ELM_SCROLLER_POLICY_AUTO);

   edje_object_part_swallow(wd->resize_obj, "elm.swallow.content", sd->scr);
   //TIZEN ONLY(20161014) : Accessibility: scroll the content when highlight frame goes out of view port
   evas_object_smart_callback_add(sd->scr, "scroll,down", _content_move_down_cb, (void*)obj);
   evas_object_smart_callback_add(sd->scr, "scroll,up", _content_move_up_cb, (void*)obj);
   //
   elm_object_content_set(sd->scr, sd->box);
   elm_ctxpopup_horizontal_set(obj, sd->horizontal);
}

EOLIAN static Eina_Bool
_elm_ctxpopup_efl_ui_widget_on_disabled_update(Eo *obj, Elm_Ctxpopup_Data *sd, Eina_Bool disabled)
{
   Eina_List *l;
   Elm_Ctxpopup_Item_Data *it;

   if (!efl_ui_widget_on_disabled_update(efl_super(obj, MY_CLASS), disabled))
     return EINA_FALSE;

   EINA_LIST_FOREACH(sd->items, l, it)
     elm_object_disabled_set(EO_OBJ(it), disabled);

   return EINA_TRUE;
}

EOLIAN static void
_elm_ctxpopup_efl_canvas_group_group_add(Eo *obj, Elm_Ctxpopup_Data *priv)
{
   ELM_WIDGET_DATA_GET_OR_RETURN(obj, wd);

   efl_canvas_group_add(efl_super(obj, MY_CLASS));
   if (!elm_widget_sub_object_parent_add(obj))
     ERR("could not add %p as sub object of %p", obj, elm_object_parent_widget_get(obj));

   elm_layout_theme_set(obj, "ctxpopup", "base", elm_widget_style_get(obj));

   //Background
   priv->bg = edje_object_add(evas_object_evas_get(obj));
   elm_widget_theme_object_set(obj, priv->bg, "ctxpopup", "bg", "default");
   edje_object_signal_callback_add
     (priv->bg, "elm,action,click", "", _bg_clicked_cb, obj);
   evas_object_smart_member_add(priv->bg, obj);
   evas_object_stack_below(priv->bg, wd->resize_obj);
   _elm_widget_color_class_parent_set(priv->bg, obj);

   //Arrow
   priv->arrow = edje_object_add(evas_object_evas_get(obj));
   elm_widget_theme_object_set
     (obj, priv->arrow, "ctxpopup", "arrow", "default");
   _elm_widget_color_class_parent_set(priv->arrow, obj);

   priv->dir_priority[0] = ELM_CTXPOPUP_DIRECTION_UP;
   priv->dir_priority[1] = ELM_CTXPOPUP_DIRECTION_LEFT;
   priv->dir_priority[2] = ELM_CTXPOPUP_DIRECTION_RIGHT;
   priv->dir_priority[3] = ELM_CTXPOPUP_DIRECTION_DOWN;
   priv->dir = ELM_CTXPOPUP_DIRECTION_UNKNOWN;

   priv->auto_hide = EINA_TRUE;
   priv->mouse_down = EINA_FALSE;
   priv->multi_down = 0;

   priv->box = elm_box_add(obj);
   evas_object_size_hint_weight_set
     (priv->box, EVAS_HINT_EXPAND, EVAS_HINT_EXPAND);

   priv->layout = edje_object_add(evas_object_evas_get(obj));
   if (wd->orient_mode == 90 || wd->orient_mode == 270)
     elm_widget_theme_object_set(obj, priv->layout, "ctxpopup", "layout/landscape", "default");
   else
     elm_widget_theme_object_set(obj, priv->layout, "ctxpopup", "layout", "default");
   evas_object_smart_member_add(priv->layout, obj);
   _elm_widget_color_class_parent_set(priv->layout, obj);

   edje_object_signal_callback_add
     (priv->layout, "elm,action,hide,finished", "", _hide_finished_cb, obj);
   /* TIZEN_ONLY(20161207): Add show finished callback */
   edje_object_signal_callback_add
     (priv->layout, "elm,action,show,finished", "", _show_finished_cb, obj);
   /* END */

   edje_object_part_swallow(priv->layout, "swallow", wd->resize_obj);
   evas_object_size_hint_weight_set
     (priv->layout, EVAS_HINT_EXPAND, EVAS_HINT_EXPAND);

   evas_object_event_callback_add(obj, EVAS_CALLBACK_SHOW, _on_show, NULL);
   evas_object_event_callback_add(obj, EVAS_CALLBACK_HIDE, _on_hide, NULL);
   evas_object_event_callback_add(obj, EVAS_CALLBACK_MOVE, _on_move, NULL);
   evas_object_event_callback_add(obj, EVAS_CALLBACK_CHANGED_SIZE_HINTS, _size_hints_changed_cb, NULL);

   _mirrored_set(obj, efl_ui_mirrored_get(obj));

   elm_widget_can_focus_set(obj, EINA_TRUE);
   elm_ctxpopup_hover_parent_set(obj, elm_object_parent_widget_get(obj));
   /* access */
   if (_elm_config->access_mode) _access_obj_process(obj, EINA_TRUE);

   /* access: parent could be any object such as elm_list which does
      not know elc_ctxpopup as its child object in the focus_next(); */

   wd->highlight_root = EINA_TRUE;

   //Tizen Only: This should be removed when eo is applied.
   wd->on_create = EINA_FALSE;
}

EOLIAN static void
_elm_ctxpopup_efl_canvas_group_group_del(Eo *obj, Elm_Ctxpopup_Data *sd)
{
   evas_object_event_callback_del_full
     (sd->box, EVAS_CALLBACK_RESIZE, _on_content_resized, obj);
   _parent_detach(obj);

   if (sd->items)
     {
        _items_remove(sd);
        _list_del(obj, sd);
     }
   else
     {
        evas_object_del(sd->box);
        sd->box = NULL;
     }

   evas_object_del(sd->arrow);
   sd->arrow = NULL; /* stops _sizing_eval() from going on on deletion */

   evas_object_del(sd->bg);
   sd->bg = NULL;

   evas_object_del(sd->layout);
   sd->layout = NULL;

   efl_canvas_group_del(efl_super(obj, MY_CLASS));
}

EOLIAN static void
_elm_ctxpopup_efl_ui_widget_widget_parent_set(Eo *obj, Elm_Ctxpopup_Data *_pd EINA_UNUSED, Evas_Object *parent)
{
   //default parent is to be hover parent
   elm_ctxpopup_hover_parent_set(obj, parent);
}

/*
static void
_elm_ctxpopup_smart_access(Evas_Object *obj, Eina_Bool is_access)
{
   ELM_CTXPOPUP_CHECK(obj);

   _access_obj_process(obj, is_access);

   evas_object_smart_callback_call(obj, SIG_ACCESS_CHANGED, NULL);
}

static Evas_Object *
_elm_ctxpopup_smart_access_object_get(Evas_Object *obj, char *part)
{
   ELM_CTXPOPUP_CHECK(obj) NULL;

   return _access_object_get(obj, part);
}
*/

EOLIAN static void
_elm_ctxpopup_class_constructor(Efl_Class *klass)
{
   evas_smart_legacy_type_register(MY_CLASS_NAME_LEGACY, klass);
}

EAPI Evas_Object *
elm_ctxpopup_add(Evas_Object *parent)
{
   EINA_SAFETY_ON_NULL_RETURN_VAL(parent, NULL);
   Evas_Object *obj = elm_legacy_add(MY_CLASS, parent);

   /* access: parent could be any object such as elm_list which does
      not know elc_ctxpopup as its child object in the focus_next(); */
   ELM_WIDGET_DATA_GET_OR_RETURN(obj, wd, NULL);
   wd->highlight_root = EINA_TRUE;

   return obj;
}

EOLIAN static Eo *
_elm_ctxpopup_efl_object_constructor(Eo *obj, Elm_Ctxpopup_Data *_pd EINA_UNUSED)
{
   obj = efl_constructor(efl_super(obj, MY_CLASS));
   efl_canvas_object_type_set(obj, MY_CLASS_NAME_LEGACY);
   evas_object_smart_callbacks_descriptions_set(obj, _smart_callbacks);
   efl_access_object_role_set(obj, EFL_ACCESS_ROLE_POPUP_MENU);

   return obj;
}


EOLIAN static void
_elm_ctxpopup_hover_parent_set(Eo *obj, Elm_Ctxpopup_Data *sd, Evas_Object *parent)
{
   Evas_Coord x, y, w, h;

   if (!parent) return;

   _parent_detach(obj);

   evas_object_event_callback_add
     (parent, EVAS_CALLBACK_DEL, _on_parent_del, obj);
   evas_object_event_callback_add
     (parent, EVAS_CALLBACK_MOVE, _on_parent_move, obj);
   evas_object_event_callback_add
     (parent, EVAS_CALLBACK_RESIZE, _on_parent_resize, obj);

   sd->parent = parent;

   //Update Background
   evas_object_geometry_get(parent, &x, &y, &w, &h);
   evas_object_move(sd->bg, x, y);
   evas_object_resize(sd->bg, w, h);

   if (sd->visible) elm_layout_sizing_eval(obj);
}

EOLIAN static Evas_Object*
_elm_ctxpopup_hover_parent_get(const Eo *obj EINA_UNUSED, Elm_Ctxpopup_Data *sd)
{
   return sd->parent;
}

EOLIAN static void
_elm_ctxpopup_clear(Eo *obj, Elm_Ctxpopup_Data *sd)
{
   _items_remove(sd);

   ELM_WIDGET_DATA_GET_OR_RETURN(obj, wd);

   elm_object_content_unset(sd->scr);
   edje_object_part_unswallow(wd->resize_obj, sd->scr);
   evas_object_del(sd->scr);
   sd->scr = NULL;
   sd->dir = ELM_CTXPOPUP_DIRECTION_UNKNOWN;
}

EOLIAN static void
_elm_ctxpopup_horizontal_set(Eo *obj, Elm_Ctxpopup_Data *sd, Eina_Bool horizontal)
{
   Eina_List *elist;
   Elm_Ctxpopup_Item_Data *item;
   int idx = 0;
   const char *style;

   sd->horizontal = !!horizontal;

   if (!sd->scr)
      return;

  if (!horizontal)
     {
        elm_box_horizontal_set(sd->box, EINA_FALSE);
        elm_scroller_bounce_set(sd->scr, EINA_FALSE, EINA_TRUE);
        elm_scroller_policy_set(sd->scr, ELM_SCROLLER_POLICY_OFF, ELM_SCROLLER_POLICY_AUTO);
     }
   else
     {
        elm_box_horizontal_set(sd->box, EINA_TRUE);
        elm_scroller_bounce_set(sd->scr, EINA_TRUE, EINA_FALSE);
        elm_scroller_policy_set(sd->scr, ELM_SCROLLER_POLICY_AUTO, ELM_SCROLLER_POLICY_OFF);
     }

   EINA_LIST_FOREACH(sd->items, elist, item)
     {
        style = (item->style ? item->style : elm_widget_style_get(obj));

        if (item->label && item->icon)
          {
             if (!sd->horizontal)
               _elm_theme_object_set(obj, VIEW(item), "ctxpopup",
                                     "icon_text_style_item",
                                     style);
             else
               _elm_theme_object_set(obj, VIEW(item), "ctxpopup",
                                     "icon_text_style_item_horizontal",
                                     style);
          }
        else if (item->label)
          {
             if (!sd->horizontal)
               _elm_theme_object_set(obj, VIEW(item), "ctxpopup",
                                     "text_style_item",
                                     style);
             else
               _elm_theme_object_set(obj, VIEW(item), "ctxpopup",
                                     "text_style_item_horizontal",
                                     style);
          }
        else
          {
             if (!sd->horizontal)
               _elm_theme_object_set(obj, VIEW(item), "ctxpopup",
                                     "icon_style_item",
                                     style);
             else
               _elm_theme_object_set(obj, VIEW(item), "ctxpopup",
                                     "icon_style_item_horizontal",
                                     style);
          }
        if (idx++ == 0)
          edje_object_signal_emit(VIEW(item), "elm,state,default", "elm");
        else
          edje_object_signal_emit(VIEW(item), "elm,state,separator", "elm");

        elm_wdg_item_disable(EO_OBJ(item));

     }

   sd->dir = ELM_CTXPOPUP_DIRECTION_UNKNOWN;

   if (sd->visible) elm_layout_sizing_eval(obj);
}

EOLIAN static Eina_Bool
_elm_ctxpopup_horizontal_get(const Eo *obj EINA_UNUSED, Elm_Ctxpopup_Data *sd)
{
   return sd->horizontal;
}

EOLIAN static Eo *
_elm_ctxpopup_item_efl_object_constructor(Eo *obj, Elm_Ctxpopup_Item_Data *it)
{
   obj = efl_constructor(efl_super(obj, ELM_CTXPOPUP_ITEM_CLASS));
   it->base = efl_data_scope_get(obj, ELM_WIDGET_ITEM_CLASS);
//TIZEN ONLY(20150710)ctxpopup: Accessible methods for children_get, extents_get and item name_get
   efl_access_object_role_set(obj, EFL_ACCESS_ROLE_MENU_ITEM);
//
   return obj;
}

EOLIAN static void
_elm_ctxpopup_item_efl_object_destructor(Eo *eo_ctxpopup_it,
                                      Elm_Ctxpopup_Item_Data *ctxpopup_it EINA_UNUSED)
{
   efl_destructor(efl_super(eo_ctxpopup_it, ELM_CTXPOPUP_ITEM_CLASS));
}

static Elm_Ctxpopup_Item_Data *
_item_new(Evas_Object *obj,
          Elm_Ctxpopup_Data *sd,
          const char *label,
          Evas_Object *icon,
          Evas_Smart_Cb func,
          const void *data)
{
   Evas_Object *content, *focus_bt;
   Eo *eo_item;

   eo_item = efl_add(ELM_CTXPOPUP_ITEM_CLASS, obj, elm_obj_ctxpopup_item_init(efl_added, func, data));
   if (!eo_item) return NULL;

   ELM_CTXPOPUP_ITEM_DATA_GET(eo_item, item);

   //The first item is appended.
   content = elm_object_content_unset(obj);
   if (content) evas_object_del(content);

   if (!sd->items)
     _list_new(obj);

   if (icon && label)
     {
        if (!sd->horizontal)
          _item_theme_set(item, "icon_text_style_item");
        else
          _item_theme_set(item, "icon_text_style_item_horizontal");
     }
   else if (label)
     {
        if (!sd->horizontal)
          _item_theme_set(item, "text_style_item");
        else
          _item_theme_set(item, "text_style_item_horizontal");
     }
   else
     {
        if (!sd->horizontal)
          _item_theme_set(item, "icon_style_item");
        else
          _item_theme_set(item, "icon_style_item_horizontal");
     }

   if (label)
     {
        edje_object_part_text_set(VIEW(item), "elm.text", label);
        item->label = eina_stringshare_add(label);
     }

   if (icon)
     {
        edje_object_part_swallow(VIEW(item), "elm.swallow.icon", icon);
        item->icon = icon;
     }

   focus_bt = _item_in_focusable_button(item);
   item->btn = focus_bt;
   _elm_widget_color_class_parent_set(VIEW(item), obj);

   if (_elm_config->access_mode) _access_focusable_button_register(focus_bt, item);

   return item;
}

EOLIAN static Elm_Object_Item*
_elm_ctxpopup_item_insert_before(Eo *obj, Elm_Ctxpopup_Data *sd, Elm_Object_Item *eo_before, const char *label, Evas_Object *icon, Evas_Smart_Cb func, const void *data)
{
   Elm_Ctxpopup_Item_Data *item;

   EINA_SAFETY_ON_NULL_RETURN_VAL(eo_before, NULL);
   ELM_CTXPOPUP_ITEM_DATA_GET(eo_before, before_it);
   ELM_CTXPOPUP_ITEM_CHECK_OR_RETURN(before_it,  NULL);

   if (!before_it->btn) return NULL;

   item = _item_new(obj, sd, label, icon, func, data);
   if (!item) return NULL;

   elm_box_pack_before(sd->box, item->btn, before_it->btn);
   sd->items = eina_list_prepend_relative(sd->items, item, before_it);

   _items_sizing_eval(obj, sd);

   return EO_OBJ(item);
}

EOLIAN static Elm_Object_Item*
_elm_ctxpopup_item_insert_after(Eo *obj, Elm_Ctxpopup_Data *sd, Elm_Object_Item *eo_after, const char *label, Evas_Object *icon, Evas_Smart_Cb func, const void *data)
{
   Elm_Ctxpopup_Item_Data *item;

   EINA_SAFETY_ON_NULL_RETURN_VAL(eo_after, NULL);
   ELM_CTXPOPUP_ITEM_DATA_GET(eo_after, after_it);
   ELM_CTXPOPUP_ITEM_CHECK_OR_RETURN(after_it,  NULL);

   if (!after_it->btn) return NULL;

   item = _item_new(obj, sd, label, icon, func, data);
   if (!item) return NULL;

   elm_box_pack_after(sd->box, item->btn, after_it->btn);
   sd->items = eina_list_append_relative(sd->items, item, after_it);

   _items_sizing_eval(obj, sd);

   return EO_OBJ(item);
}

EOLIAN static Elm_Object_Item*
_elm_ctxpopup_item_append(Eo *obj, Elm_Ctxpopup_Data *sd, const char *label, Evas_Object *icon, Evas_Smart_Cb func, const void *data)
{
   Elm_Ctxpopup_Item_Data *item;

   item = _item_new(obj, sd, label, icon, func, data);
   if (!item) return NULL;

   elm_box_pack_end(sd->box, item->btn);
   sd->items = eina_list_append(sd->items, item);

   _items_sizing_eval(obj, sd);

   return EO_OBJ(item);
}

EOLIAN static Elm_Object_Item*
_elm_ctxpopup_item_prepend(Eo *obj EINA_UNUSED, Elm_Ctxpopup_Data *sd EINA_UNUSED, const char *label EINA_UNUSED, Evas_Object *icon EINA_UNUSED, Evas_Smart_Cb func EINA_UNUSED, const void *data EINA_UNUSED)
{
   Elm_Ctxpopup_Item_Data *item;

   item = _item_new(obj, sd, label, icon, func, data);
   if (!item) return NULL;

   elm_box_pack_start(sd->box, item->btn);
   sd->items = eina_list_prepend(sd->items, item);

   _items_sizing_eval(obj, sd);

   return EO_OBJ(item);
}

EOLIAN static void
_elm_ctxpopup_direction_priority_set(Eo *obj, Elm_Ctxpopup_Data *sd, Elm_Ctxpopup_Direction first, Elm_Ctxpopup_Direction second, Elm_Ctxpopup_Direction third, Elm_Ctxpopup_Direction fourth)
{
   sd->dir_priority[0] = first;
   sd->dir_priority[1] = second;
   sd->dir_priority[2] = third;
   sd->dir_priority[3] = fourth;

   if (sd->visible) elm_layout_sizing_eval(obj);
}

EOLIAN static void
_elm_ctxpopup_direction_priority_get(const Eo *obj EINA_UNUSED, Elm_Ctxpopup_Data *sd, Elm_Ctxpopup_Direction *first, Elm_Ctxpopup_Direction *second, Elm_Ctxpopup_Direction *third, Elm_Ctxpopup_Direction *fourth)
{
   if (first) *first = sd->dir_priority[0];
   if (second) *second = sd->dir_priority[1];
   if (third) *third = sd->dir_priority[2];
   if (fourth) *fourth = sd->dir_priority[3];
}

EOLIAN static Elm_Ctxpopup_Direction
_elm_ctxpopup_direction_get(const Eo *obj EINA_UNUSED, Elm_Ctxpopup_Data *sd)
{
   return sd->dir;
}

EAPI Eina_Bool
elm_ctxpopup_direction_available_get(Evas_Object *obj, Elm_Ctxpopup_Direction direction)
{
   ELM_CTXPOPUP_CHECK(obj) EINA_FALSE;
   ELM_CTXPOPUP_DATA_GET(obj, sd);

   elm_layout_sizing_eval(obj);

   if (sd->dir == direction) return EINA_TRUE;
   return EINA_FALSE;
}

EOLIAN static void
_elm_ctxpopup_dismiss(Eo *obj, Elm_Ctxpopup_Data *sd)
{
   _hide_signals_emit(obj, sd->dir);
}

EOLIAN static void
_elm_ctxpopup_auto_hide_disabled_set(Eo *obj EINA_UNUSED, Elm_Ctxpopup_Data *sd, Eina_Bool disabled)
{
   disabled = !!disabled;
   if (sd->auto_hide == !disabled) return;
   sd->auto_hide = !disabled;
}

EOLIAN static Eina_Bool
_elm_ctxpopup_auto_hide_disabled_get(const Eo *obj EINA_UNUSED, Elm_Ctxpopup_Data *sd)
{
   return !sd->auto_hide;
}

// TIZEN_ONLY(20170116): merge eo & header files for different profiles
EOLIAN static const Eina_List *
//EOLIAN static Eina_Iterator*
_elm_ctxpopup_items_get(const Eo *obj EINA_UNUSED, Elm_Ctxpopup_Data *sd)
{
  return sd->items;
   //return eina_list_iterator_new(sd->items);
}

EOLIAN static Elm_Object_Item*
_elm_ctxpopup_first_item_get(const Eo *obj EINA_UNUSED, Elm_Ctxpopup_Data *sd)
{
   if (!sd->items) return NULL;

   return eina_list_data_get(sd->items);
}

EOLIAN static Elm_Object_Item*
_elm_ctxpopup_last_item_get(const Eo *obj EINA_UNUSED, Elm_Ctxpopup_Data *sd)
{
   if (!sd->items) return NULL;

   return eina_list_data_get(eina_list_last(sd->items));
}

// TIZEN_ONLY(20180112): remove item selected function
EOLIAN static Elm_Object_Item*
_elm_ctxpopup_selected_item_get(const Eo *obj EINA_UNUSED, Elm_Ctxpopup_Data *sd)
{
   Eina_List *l;
   Elm_Object_Item *eo_item;

   EINA_LIST_FOREACH(sd->items, l, eo_item)
     {
        ELM_CTXPOPUP_ITEM_DATA_GET(eo_item, item);
        if (item->selected) return eo_item;
     }

   return NULL;
}
//

// TIZEN_ONLY(20180112): remove focused item get function
EOLIAN static Elm_Object_Item*
_elm_ctxpopup_elm_widget_item_container_focused_item_get(const Eo *obj EINA_UNUSED, Elm_Ctxpopup_Data *sd)
{
   if (!sd->list) return NULL;

   return elm_object_focused_item_get(sd->list);
}
//

EOLIAN static Elm_Object_Item *
_elm_ctxpopup_item_prev_get(const Eo *eo_item EINA_UNUSED, Elm_Ctxpopup_Item_Data *item)
{
   Eina_List *l;

   ELM_CTXPOPUP_DATA_GET(WIDGET(item), sd);

   if (!sd->items) return NULL;

   l = eina_list_data_find_list(sd->items, eo_item);
   if (l && l->prev) return eina_list_data_get(l->prev);

   return NULL;
}

EOLIAN static Elm_Object_Item *
_elm_ctxpopup_item_next_get(const Eo *eo_item EINA_UNUSED, Elm_Ctxpopup_Item_Data *item)
{
   Eina_List *l;

   ELM_CTXPOPUP_DATA_GET(WIDGET(item), sd);

   if (!sd->items) return NULL;

   l = eina_list_data_find_list(sd->items, eo_item);
   if (l && l->next) return eina_list_data_get(l->next);

   return NULL;
}

EOLIAN static void
_elm_ctxpopup_item_selected_set(Eo *eo_item EINA_UNUSED,
                                            Elm_Ctxpopup_Item_Data *item,
                                            Eina_Bool selected)
{
   Elm_Object_Item *eo_item2;
   Eina_List *l;

   if (selected == item->selected) return;

   if (selected)
     {
        ELM_CTXPOPUP_DATA_GET_OR_RETURN(item->wcb.cobj, sd);
        EINA_LIST_FOREACH(sd->items, l, eo_item2)
          {
             ELM_CTXPOPUP_ITEM_DATA_GET(eo_item2, item2);
             item2->selected = EINA_FALSE;
          }
     }
   elm_list_item_selected_set(item->list_item, selected);
   item->selected = selected;
}

EOLIAN static Eina_Bool
_elm_ctxpopup_item_selected_get(const Eo *eo_item EINA_UNUSED, Elm_Ctxpopup_Item_Data *item)
{
   return item->selected;
}

//

EOLIAN static void
_elm_ctxpopup_item_init(Eo *eo_item,
          Elm_Ctxpopup_Item_Data *item,
          Evas_Smart_Cb func,
          const void *data)
{
   Eo *obj;
   obj = efl_parent_get(eo_item);

   item->wcb.org_func_cb = func;
   item->wcb.org_data = data;
   item->wcb.cobj = obj;
}

//TIZEN ONLY(20160918): name interface added
EOLIAN static const char*
_elm_ctxpopup_efl_access_object_i18n_name_get(const Eo *obj, Elm_Ctxpopup_Data *sd EINA_UNUSED)
{
   Eina_Strbuf *buf;
   const char *accessible_name = NULL;
   const char *style = elm_widget_style_get(obj);

   accessible_name = efl_access_object_i18n_name_get(efl_super(obj, MY_CLASS));
   if (accessible_name) return accessible_name;

   ELM_WIDGET_DATA_GET_OR_RETURN(obj, wd, NULL);
   buf = eina_strbuf_new();
   if (!strcmp(style, "more/default"))
     eina_strbuf_append_printf(buf, "%s", N_("More menu popup"));
   else
     eina_strbuf_append_printf(buf, "%s", E_("Alert"));

   eina_stringshare_del(wd->accessible_name);
   wd->accessible_name = eina_stringshare_add(eina_strbuf_string_get(buf));
   eina_strbuf_free(buf);

   return wd->accessible_name;
}
//

EOLIAN static const Efl_Access_Action_Data*
_elm_ctxpopup_efl_access_widget_action_elm_actions_get(const Eo *obj EINA_UNUSED, Elm_Ctxpopup_Data *sd EINA_UNUSED)
{
   static Efl_Access_Action_Data atspi_actions[] = {
          { "escape", "escape", NULL, _key_action_escape},
          /* to fix error, disable below code temporarily
          { "move,previous", "move", "previous", _key_action_move},
          { "move,next", "move", "next", _key_action_move},
          { "move,left", "move", "left", _key_action_move},
          { "move,right", "move", "right", _key_action_move},
          { "move,up", "move", "up", _key_action_move},
          { "move,down", "move", "down", _key_action_move},
          { NULL, NULL, NULL, NULL }
          */
   };
   return &atspi_actions[0];
}

EOLIAN static Efl_Access_State_Set
_elm_ctxpopup_efl_access_object_state_set_get(const Eo *obj, Elm_Ctxpopup_Data *sd EINA_UNUSED)
{
   Efl_Access_State_Set ret;
   ret = efl_access_object_state_set_get(efl_super(obj, MY_CLASS));

   STATE_TYPE_SET(ret, EFL_ACCESS_STATE_MODAL);

   if (_elm_object_accessibility_currently_highlighted_get() == (void*)sd->scr)
     STATE_TYPE_SET(ret, EFL_ACCESS_STATE_HIGHLIGHTED);

   return ret;
}

//TIZEN ONLY(20150710): ctxpopup: Accessible methods for children_get, extents_get and item name_get
EOLIAN Eina_List*
_elm_ctxpopup_efl_access_object_access_children_get(const Eo *eo_item EINA_UNUSED, Elm_Ctxpopup_Data *sd)
{
   Eina_List *ret = NULL;
   Eina_List *l = NULL;
   Elm_Ctxpopup_Item_Data *it;

   EINA_LIST_FOREACH(sd->items, l, it)
     {
        if (efl_isa(EO_OBJ(it), EFL_ACCESS_OBJECT_MIXIN))
          {
             ret = eina_list_append(ret, EO_OBJ(it));
             //TIZEN_ONLY(20181024): Fix parent-children incosistencies in atspi tree
             efl_access_object_access_parent_set(EO_OBJ(it), eo_item);
             //
          }
     }

   return ret;
}

EOLIAN static Eina_Rect
_elm_ctxpopup_efl_access_component_extents_get(const Eo *obj EINA_UNUSED, Elm_Ctxpopup_Data *sd, Eina_Bool screen_coords)
{
   int ee_x, ee_y;
   Eina_Rect r;

   if (!sd->scr)
     {
        r.x = -1;
        r.y = -1;
        r.w = -1;
        r.h = -1;
        return r;
     }
   r = efl_gfx_entity_geometry_get(sd->scr);

   if (screen_coords)
     {
        Ecore_Evas *ee = ecore_evas_ecore_evas_get(evas_object_evas_get(sd->scr));
        if (!ee) return r;
        ecore_evas_geometry_get(ee, &ee_x, &ee_y, NULL, NULL);
        r.x += ee_x;
        r.y += ee_y;
     }
   return r;
}

//
//TIZEN ONLY(20150708): popup and ctxpopup accessibility highlight impementation
EOLIAN static Eina_Bool
_elm_ctxpopup_efl_access_component_highlight_grab(Eo *obj EINA_UNUSED, Elm_Ctxpopup_Data *sd)
{
   if (sd->scr)
     {
        elm_object_accessibility_highlight_set(sd->scr, EINA_TRUE);
        ///TIZEN_ONLY(20170717) : expose highlight information on atspi
        efl_access_state_changed_signal_emit(obj, ELM_ATSPI_STATE_HIGHLIGHTED, EINA_TRUE);
        ///
        return EINA_TRUE;
     }
   return EINA_FALSE;
}

EOLIAN static Eina_Bool
_elm_ctxpopup_efl_access_component_highlight_clear(Eo *obj EINA_UNUSED, Elm_Ctxpopup_Data *sd)
{
   if (sd->scr)
     {
        elm_object_accessibility_highlight_set(sd->scr, EINA_FALSE);
        ///TIZEN_ONLY(20170717) : expose highlight information on atspi
        efl_access_state_changed_signal_emit(obj, ELM_ATSPI_STATE_HIGHLIGHTED, EINA_FALSE);
        ///
        return EINA_TRUE;
     }
   return EINA_FALSE;
}
//

//TIZEN ONLY(20150710)ctxpopup: Accessible methods for children_get, extents_get and item name_get
EOLIAN const char *
_elm_ctxpopup_item_efl_access_object_i18n_name_get(const Eo *eo_it, Elm_Ctxpopup_Item_Data *item)
{
   const char *accessible_name = NULL;
   accessible_name = efl_access_object_i18n_name_get(efl_super(eo_it, ELM_CTXPOPUP_ITEM_CLASS));

   if (accessible_name) return accessible_name;

   if(item->icon && !item->label)
     {
        const char *icon_name = NULL;
        icon_name = efl_access_object_i18n_name_get(item->icon);
        return icon_name;
     }
   else
     return _elm_widget_item_accessible_plain_name_get(eo_it, item->label);
}
//

static Eina_Bool
_key_action_escape(Evas_Object *obj, const char *params EINA_UNUSED)
{
   elm_ctxpopup_dismiss(obj);
   return EINA_TRUE;
}

static Eina_Bool
_item_action_activate(Evas_Object *obj, const char *params EINA_UNUSED)
{
   ELM_CTXPOPUP_ITEM_DATA_GET(obj, item);
   edje_object_signal_emit
      (VIEW(item), "elm,key,action,activate", "elm");
   edje_object_message_signal_process(VIEW(item));


   if (item->wcb.org_func_cb)
     item->wcb.org_func_cb((void*)item->wcb.org_data, WIDGET(item), EO_OBJ(item));
   return EINA_TRUE;
}

EOLIAN static const Efl_Access_Action_Data*
_elm_ctxpopup_item_efl_access_widget_action_elm_actions_get(const Eo *obj EINA_UNUSED, Elm_Ctxpopup_Item_Data *sd EINA_UNUSED)
{
   static Efl_Access_Action_Data atspi_actions[] = {
          { "activate", "activate", NULL, _item_action_activate},
          { NULL, NULL, NULL, NULL }
   };
   return &atspi_actions[0];
}

//TIZEN ONLY(20161014) : Accessibility: scroll the content when highlight frame goes out of view port
static int
_is_item_in_viewport(int viewport_y, int viewport_h, int obj_y, int obj_h)
{
   if ((obj_y + obj_h/2) < viewport_y)
     return 1;
   else if ((obj_y + obj_h/2) > viewport_y + viewport_h)
     return -1;
   return 0;
}

EOLIAN static Eina_Bool
_elm_ctxpopup_item_efl_access_component_highlight_grab(Eo *eo_it, Elm_Ctxpopup_Item_Data *it)
{
   Evas_Coord wy, wh, x, y, w, h, bx, by;
   ELM_CTXPOPUP_DATA_GET_OR_RETURN_VAL(WIDGET(it), sd, EINA_FALSE);

   evas_object_geometry_get(VIEW(it), &x, &y, &w, &h);
   elm_interface_scrollable_content_viewport_geometry_get(sd->scr, NULL, &wy, NULL, &wh);
   int res = _is_item_in_viewport(wy, wh, y, h);

   if (res != 0)
     {
        evas_object_geometry_get(sd->box, &bx, &by, NULL, NULL);
        evas_smart_objects_calculate(evas_object_evas_get(sd->box));
        x -= bx;
        y -= by;
        if (res > 0)
          {
             y -= wh - h;
             elm_interface_scrollable_content_region_show(sd->scr, x, y, w, h);
          }
        else if (res < 0)
          {
             y += wh - h;
             elm_interface_scrollable_content_region_show(sd->scr, x, y, w, h);
          }
     }

   elm_object_accessibility_highlight_set(eo_it, EINA_TRUE);
   efl_access_state_changed_signal_emit(eo_it, ELM_ATSPI_STATE_HIGHLIGHTED, EINA_TRUE);
   //TIZEN_ONLY(20170412) Make atspi,(un)highlighted work on widget item
   // If you call eo_do_super, then you do NOT have to call smart callback.
   evas_object_smart_callback_call(WIDGET(it), "atspi,highlighted", EO_OBJ(it));
   //
   return EINA_TRUE;
}

EOLIAN static Eina_Bool
_elm_ctxpopup_item_efl_access_component_highlight_clear(Eo *eo_it, Elm_Ctxpopup_Item_Data *it EINA_UNUSED)
{
   elm_object_accessibility_highlight_set(eo_it, EINA_FALSE);
   efl_access_state_changed_signal_emit(eo_it, ELM_ATSPI_STATE_HIGHLIGHTED, EINA_FALSE);
   //TIZEN_ONLY(20170412) Make atspi,(un)highlighted work on widget item
   // If you call eo_do_super, then you do NOT have to call smart callback.
   evas_object_smart_callback_call(WIDGET(it), "atspi,unhighlighted", EO_OBJ(it));
   //
   return EINA_TRUE;
}

static void
_content_move_down_cb(void *data, Evas_Object *obj, void *ev EINA_UNUSED)
{
   if (!_elm_atspi_enabled())
     return ;
   ELM_CTXPOPUP_DATA_GET(data, pd);
   Elm_Ctxpopup_Item_Data *it_data;
   Evas_Object *highlighted_obj = _elm_object_accessibility_currently_highlighted_get();

   if (efl_isa(highlighted_obj, ELM_WIDGET_ITEM_CLASS))
     {
        Elm_Widget_Item_Data *id = efl_data_scope_get(highlighted_obj, ELM_WIDGET_ITEM_CLASS);
        highlighted_obj = id->view;
     }

   Evas_Object *parent = highlighted_obj;

   if (efl_isa(highlighted_obj, EFL_UI_WIDGET_CLASS))
     {
        while ((parent = elm_widget_parent_get(parent)))
          if (parent == obj)
            break;
     }
   else if (efl_isa(highlighted_obj, EFL_CANVAS_LAYOUT_CLASS))
     {
        while ((parent = evas_object_smart_parent_get(parent)))
          if (parent == obj)
            break;
     }
   // TIZEN_ONLY(20160805): set _accessibility_currently_highlighted_obj to NULL in object delete callback
   else
     {
        WRN("Improper highlighted object: %p", highlighted_obj);
        return;
     }
   //

   if (parent)
     {
        int obj_x, obj_y, w, h, hx, hy, hw, hh;
        elm_interface_scrollable_content_viewport_geometry_get(pd->scr, &obj_x, &obj_y, &w, &h);
        evas_object_geometry_get(highlighted_obj, &hx, &hy, &hw, &hh);

        Elm_Ctxpopup_Item_Data *next_previous_item = NULL;
        int viewport_position_result = _is_item_in_viewport(obj_y, h, hy, hh);
        if (viewport_position_result > 0)
          {
             Eina_List *l = pd->items;
             while(l)
              {
                 it_data = eina_list_data_get(l);
                 next_previous_item = it_data;
                 evas_object_geometry_get(VIEW(next_previous_item), &hx, &hy, &hw, &hh);

                 if (_is_item_in_viewport(obj_y, h, hy, hh) == 0)
                    break;
                 next_previous_item = NULL;

                 l = eina_list_next(l);
              }
          }
        if (next_previous_item)
          {
             elm_object_accessibility_highlight_set(EO_OBJ(next_previous_item), EINA_TRUE);
             efl_access_state_changed_signal_emit(EO_OBJ(next_previous_item), ELM_ATSPI_STATE_HIGHLIGHTED, EINA_TRUE);
          }
     }

}

static void
_content_move_up_cb(void *data, Evas_Object *obj, void *ev EINA_UNUSED)
{
   if (!_elm_atspi_enabled())
     return ;
   ELM_CTXPOPUP_DATA_GET(data, pd);
   Elm_Ctxpopup_Item_Data *it_data;
   Evas_Object *highlighted_obj = _elm_object_accessibility_currently_highlighted_get();

   if (efl_isa(highlighted_obj, ELM_WIDGET_ITEM_CLASS))
     {
        Elm_Widget_Item_Data *id = efl_data_scope_get(highlighted_obj, ELM_WIDGET_ITEM_CLASS);
        highlighted_obj = id->view;
     }

   Evas_Object *parent = highlighted_obj;

   if (efl_isa(highlighted_obj, EFL_UI_WIDGET_CLASS))
     {
        while ((parent = elm_widget_parent_get(parent)))
          if (parent == obj)
            break;
     }
   else if (efl_isa(highlighted_obj, EFL_CANVAS_LAYOUT_CLASS))
     {
        while ((parent = evas_object_smart_parent_get(parent)))
          if (parent == obj)
            break;
     }
   // TIZEN_ONLY(20160805): set _accessibility_currently_highlighted_obj to NULL in object delete callback
   else
     {
        WRN("Improper highlighted object: %p", highlighted_obj);
        return;
     }
   //

   if (parent)
     {
        int obj_x, obj_y, w, h, hx, hy, hw, hh;
        elm_interface_scrollable_content_viewport_geometry_get(pd->scr, &obj_x, &obj_y, &w, &h);
        evas_object_geometry_get(highlighted_obj, &hx, &hy, &hw, &hh);

        Elm_Ctxpopup_Item_Data *next_previous_item = NULL;
        int viewport_position_result = _is_item_in_viewport(obj_y, h, hy, hh);
        if (viewport_position_result < 0)
          {
             Eina_List *l = eina_list_last(pd->items);
             while(l)
              {
                 it_data = eina_list_data_get(l);
                 next_previous_item = it_data;
                 evas_object_geometry_get(VIEW(next_previous_item), &hx, &hy, &hw, &hh);

                 if (_is_item_in_viewport(obj_y, h, hy, hh) == 0)
                    break;
                 next_previous_item = NULL;

                 l = eina_list_prev(l);
              }
          }
        if (next_previous_item)
          {
             elm_object_accessibility_highlight_set(EO_OBJ(next_previous_item), EINA_TRUE);
             efl_access_state_changed_signal_emit(EO_OBJ(next_previous_item), ELM_ATSPI_STATE_HIGHLIGHTED, EINA_TRUE);
          }
     }

}
//

//TIZEN_ONLY ctx_popup: fix accessibility states
EOLIAN static Efl_Access_State_Set
_elm_ctxpopup_item_efl_access_object_state_set_get(const Eo *obj, Elm_Ctxpopup_Item_Data *it)
{
   Efl_Access_State_Set states;
   Eina_Rectangle r1, r2;
   Eina_Bool is_showing = EINA_FALSE;

   states = efl_access_object_state_set_get(efl_super(obj, ELM_CTXPOPUP_ITEM_CLASS));

   // evaluate showing state
   if (VIEW(it) &&
       WIDGET(it) &&
       evas_object_visible_get(VIEW(it)) &&
       _elm_widget_onscreen_is(WIDGET(it)))
     {
        ELM_CTXPOPUP_DATA_GET(WIDGET(it), sd);

        evas_object_geometry_get(VIEW(it), &r1.x, &r1.y, &r1.w, &r1.h);
        evas_object_geometry_get(sd->scr, &r2.x, &r2.y, &r2.w, &r2.h);

        if (eina_rectangles_intersect(&r1, &r2))
          is_showing = EINA_TRUE;
     }

   if (is_showing)
     STATE_TYPE_SET(states, EFL_ACCESS_STATE_SHOWING);
   else
     STATE_TYPE_UNSET(states, EFL_ACCESS_STATE_SHOWING);

   return states;
}

/* Internal EO APIs and hidden overrides */

ELM_WIDGET_KEY_DOWN_DEFAULT_IMPLEMENT(elm_ctxpopup, Elm_Ctxpopup_Data)

/* Efl.Part begin */

ELM_PART_OVERRIDE(elm_ctxpopup, ELM_CTXPOPUP, Elm_Ctxpopup_Data)
ELM_PART_OVERRIDE_CONTENT_SET(elm_ctxpopup, ELM_CTXPOPUP, Elm_Ctxpopup_Data)
ELM_PART_OVERRIDE_CONTENT_GET(elm_ctxpopup, ELM_CTXPOPUP, Elm_Ctxpopup_Data)
ELM_PART_OVERRIDE_CONTENT_UNSET(elm_ctxpopup, ELM_CTXPOPUP, Elm_Ctxpopup_Data)
#include "../elementary/elm_ctxpopup_part.eo.c"

/* Efl.Part end */

/* Internal EO APIs and hidden overrides */

#define ELM_CTXPOPUP_EXTRA_OPS \
   ELM_LAYOUT_SIZING_EVAL_OPS(elm_ctxpopup), \
   EFL_CANVAS_GROUP_ADD_DEL_OPS(elm_ctxpopup)
//
#include "../elementary/elm_ctxpopup_item.eo.c"
#include "../elementary/elm_ctxpopup.eo.c"
