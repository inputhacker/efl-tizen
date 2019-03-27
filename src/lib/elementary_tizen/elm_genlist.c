#ifdef HAVE_CONFIG_H
# include "../elementary/elementary_config.h"
#endif

#include <fnmatch.h>

#define EFL_ACCESS_OBJECT_PROTECTED
#define EFL_ACCESS_SELECTION_PROTECTED
//TIZEN_ONLY(20171114) genlist: enhance accessibility scroll & highlight
#define EFL_ACCESS_COMPONENT_PROTECTED
//
#define EFL_ACCESS_WIDGET_ACTION_PROTECTED
#define ELM_WIDGET_ITEM_PROTECTED
#define EFL_UI_FOCUS_COMPOSITION_PROTECTED

#include <Elementary.h>
#include <Elementary_Cursor.h>

#include "../elementary/elm_priv.h"
#include "elm_widget_genlist.h"
#include "../elementary/elm_interface_scrollable.h"
#include "../elementary/efl_ui_focus_parent_provider_gen_eo.h"
#include "../elementary/efl_ui_focus_composition_adapter.eo.h"
#include "../elementary/elm_genlist_item_eo.h"
#include "../elementary/elm_genlist_pan_eo.h"
#include "../elementary/elm_genlist_eo.h"

#define MY_PAN_CLASS ELM_GENLIST_PAN_CLASS

#define MY_PAN_CLASS_NAME "Elm_Genlist_Pan"
#define MY_PAN_CLASS_NAME_LEGACY "elm_genlist_pan"

#define MY_CLASS ELM_GENLIST_CLASS

#define MY_CLASS_NAME "Elm_Genlist"
#define MY_CLASS_NAME_LEGACY "elm_genlist"

// internally allocated
#define CLASS_ALLOCATED     0x3a70f11f

#define REORDERED_ITEM_OFFSET (8 * elm_config_scale_get())
#define REORDER_FASTER (15 * elm_config_scale_get())
#define REORDER_ANIM_OFFSET (10 * elm_config_scale_get())
#define MAX_ITEMS_PER_BLOCK 32
#define ITEM_SELECT_TIMER 0.05  // This is needed for highlight effect when item is unhighlighted right after item is highlighted and selected
#define ANIM_CNT_MAX (_elm_config->genlist_animation_duration / 1000 * _elm_config->fps) // frame time
#define HIGHLIGHT_ALPHA_MAX 0.74
#define ELM_ITEM_HIGHLIGHT_TIMER 0.1
#define ITEM_QUEUE_MAX 128
#define DIMED_ITEM_COLOR 50
#define ALIGNED_ITEM_SCALE_FACTOR 1.5
#define CENTER_CHECK_GAP 50
#define FLICK_GAP 30
#define PINCH_ZOOM_TOLERANCE 0.4


#define ERR_ABORT(_msg)                         \
   do {                                         \
        ERR(_msg);                              \
        if (getenv("ELM_ERROR_ABORT")) abort(); \
   } while (0)

#define GL_IT(_it) (_it->item)

#define IS_ROOT_PARENT_IT(_it) \
   ((_it->group) || ((GL_IT(_it)->items && GL_IT(_it)->expanded_depth == 0)  \
                      &&(!(GL_IT(_it)->type & ELM_GENLIST_ITEM_TREE)))) \

#define ELM_PRIV_GENLIST_SIGNALS(cmd) \
    cmd(SIG_ACTIVATED, "activated", "") \
    cmd(SIG_CLICKED_DOUBLE, "clicked,double", "") \
    cmd(SIG_CLICKED_RIGHT, "clicked,right", "") \
    cmd(SIG_SELECTED, "selected", "") \
    cmd(SIG_UNSELECTED, "unselected", "") \
    cmd(SIG_EXPANDED, "expanded", "") \
    cmd(SIG_CONTRACTED, "contracted", "") \
    cmd(SIG_EXPAND_REQUEST, "expand,request", "") \
    cmd(SIG_CONTRACT_REQUEST, "contract,request", "") \
    cmd(SIG_REALIZED, "realized", "") \
    cmd(SIG_UNREALIZED, "unrealized", "") \
    cmd(SIG_DRAG_START_UP, "drag,start,up", "") \
    cmd(SIG_DRAG_START_DOWN, "drag,start,down", "") \
    cmd(SIG_DRAG_START_LEFT, "drag,start,left", "") \
    cmd(SIG_DRAG_START_RIGHT, "drag,start,right", "") \
    cmd(SIG_DRAG_STOP, "drag,stop", "") \
    cmd(SIG_DRAG, "drag", "") \
    cmd(SIG_LONGPRESSED, "longpressed", "") \
    cmd(SIG_SCROLL, "scroll", "") \
    cmd(SIG_SCROLL_ANIM_START, "scroll,anim,start", "") \
    cmd(SIG_SCROLL_ANIM_STOP, "scroll,anim,stop", "") \
    cmd(SIG_SCROLL_DRAG_START, "scroll,drag,start", "") \
    cmd(SIG_SCROLL_DRAG_STOP, "scroll,drag,stop", "") \
    cmd(SIG_EDGE_TOP, "edge,top", "") \
    cmd(SIG_EDGE_BOTTOM, "edge,bottom", "") \
    cmd(SIG_EDGE_LEFT, "edge,left", "") \
    cmd(SIG_EDGE_RIGHT, "edge,right", "") \
    cmd(SIG_VBAR_DRAG, "vbar,drag", "") \
    cmd(SIG_VBAR_PRESS, "vbar,press", "") \
    cmd(SIG_VBAR_UNPRESS, "vbar,unpress", "") \
    cmd(SIG_HBAR_DRAG, "hbar,drag", "") \
    cmd(SIG_HBAR_PRESS, "hbar,press", "") \
    cmd(SIG_HBAR_UNPRESS, "hbar,unpress", "") \
    cmd(SIG_MULTI_SWIPE_LEFT, "multi,swipe,left", "") \
    cmd(SIG_MULTI_SWIPE_RIGHT, "multi,swipe,right", "") \
    cmd(SIG_MULTI_SWIPE_UP, "multi,swipe,up", "") \
    cmd(SIG_MULTI_SWIPE_DOWN, "multi,swipe,down", "") \
    cmd(SIG_MULTI_PINCH_OUT, "multi,pinch,out", "") \
    cmd(SIG_MULTI_PINCH_IN, "multi,pinch,in", "") \
    cmd(SIG_SWIPE, "swipe", "") \
    cmd(SIG_MOVED, "moved", "") \
    cmd(SIG_MOVED_AFTER, "moved,after", "") \
    cmd(SIG_MOVED_BEFORE, "moved,before", "") \
    cmd(SIG_INDEX_UPDATE, "index,update", "") \
    cmd(SIG_TREE_EFFECT_FINISHED , "tree,effect,finished", "") \
    cmd(SIG_HIGHLIGHTED, "highlighted", "") \
    cmd(SIG_UNHIGHLIGHTED, "unhighlighted", "") \
    cmd(SIG_ITEM_FOCUSED, "item,focused", "") \
    cmd(SIG_ITEM_UNFOCUSED, "item,unfocused", "") \
    cmd(SIG_PRESSED, "pressed", "") \
    cmd(SIG_RELEASED, "released", "") \
    cmd(SIG_CHANGED, "changed", "") \
    cmd(SIG_WIDGET_ATSPI_HIGHLIGHTED, "atspi,highlighted", "") \
    cmd(SIG_ATSPI_SCREEN_READER_CHANGED, "atspi,screen,reader,changed", "") \
    cmd(SIG_FILTER_DONE, "filter,done", "") \
    cmd(SIG_LANG_CHANGED, "language,changed", "") \
    cmd(SIG_LOADED, "loaded", "")

ELM_PRIV_GENLIST_SIGNALS(ELM_PRIV_STATIC_VARIABLE_DECLARE);

static const Evas_Smart_Cb_Description _smart_callbacks[] = {
   ELM_PRIV_GENLIST_SIGNALS(ELM_PRIV_SMART_CALLBACKS_DESC)
   {SIG_WIDGET_ACCESS_CHANGED, ""}, /**< handled by elm_widget */
   {SIG_LAYOUT_FOCUSED, ""}, /**< handled by elm_layout */
   {SIG_LAYOUT_UNFOCUSED, ""}, /**< handled by elm_layout */
   {NULL, NULL}
};
#undef ELM_PRIV_GENLIST_SIGNALS

/* edje signals internally used */
static const char SIGNAL_ENABLED[] = "elm,state,enabled";
static const char SIGNAL_DISABLED[] = "elm,state,disabled";
static const char SIGNAL_SELECTED[] = "elm,state,selected";
static const char SIGNAL_UNSELECTED[] = "elm,state,unselected";
static const char SIGNAL_EXPANDED[] = "elm,state,expanded";
static const char SIGNAL_CONTRACTED[] = "elm,state,contracted";
static const char SIGNAL_FLIP_ENABLED[] = "elm,state,flip,enabled";
static const char SIGNAL_FLIP_DISABLED[] = "elm,state,flip,disabled";
static const char SIGNAL_DECORATE_ENABLED[] = "elm,state,decorate,enabled";
static const char SIGNAL_DECORATE_DISABLED[] = "elm,state,decorate,disabled";
static const char SIGNAL_DECORATE_ENABLED_EFFECT[] = "elm,state,decorate,enabled,effect";
static const char SIGNAL_REORDER_ENABLED[] = "elm,state,reorder,enabled";
static const char SIGNAL_REORDER_DISABLED[] = "elm,state,reorder,disabled";
static const char SIGNAL_REORDER_MODE_SET[] = "elm,state,reorder,mode_set";
static const char SIGNAL_REORDER_MODE_UNSET[] = "elm,state,reorder,mode_unset";
static const char SIGNAL_CONTRACT_FLIP[] = "elm,state,contract_flip";
static const char SIGNAL_SHOW[] = "elm,state,show";
static const char SIGNAL_HIDE[] = "elm,state,hide";
static const char SIGNAL_FLIP_ITEM[] = "elm,action,flip_item";
static const char SIGNAL_ODD[] = "elm,state,odd";
static const char SIGNAL_EVEN[] = "elm,state,even";
static const char SIGNAL_FOCUSED[] = "elm,state,focused";
static const char SIGNAL_UNFOCUSED[] = "elm,state,unfocused";
static const char SIGNAL_LIST_SINGLE[] = "elm,state,list,single";
static const char SIGNAL_LIST_FIRST[] = "elm,state,list,first";
static const char SIGNAL_LIST_LAST[] = "elm,state,list,last";
static const char SIGNAL_LIST_MIDDLE[] = "elm,state,list,middle";
static const char SIGNAL_GROUP_SINGLE[] = "elm,state,group,single";
static const char SIGNAL_GROUP_FIRST[] = "elm,state,group,first";
static const char SIGNAL_GROUP_LAST[] = "elm,state,group,last";
static const char SIGNAL_GROUP_MIDDLE[] = "elm,state,group,middle";
//Tizen Only(20160307)
static const char SIGNAL_DEFAULT[] = "elm,state,default";
//TIZEN ONLY
static const char SIGNAL_CLICKED[] = "elm,state,clicked";
static const char SIGNAL_BG_CHANGE[] = "bg_color_change";
static const char SIGNAL_ITEM_HIGHLIGHTED[] = "elm,state,highlighted";
static const char SIGNAL_ITEM_UNHIGHLIGHTED[] = "elm,state,unhighlighted";
static const char SIGNAL_FOCUS_BG_SHOW[] = "elm,state,focus_bg,show";
static const char SIGNAL_FOCUS_BG_HIDE[] = "elm,state,focus_bg,hide";
//

static Eina_Bool _key_action_move(Evas_Object *obj, const char *params);
static Eina_Bool _key_action_select(Evas_Object *obj, const char *params);
static Eina_Bool _key_action_escape(Evas_Object *obj, const char *params);
static Eina_Bool _item_select(Elm_Gen_Item *it);
static Eina_Bool _long_press_cb(void *data);
static Eina_Bool _queue_idle_enter(void *data);
static Eina_Bool _item_process(Elm_Genlist_Data *sd, Elm_Gen_Item *it);
static Eina_Bool _item_filtered_get(Elm_Gen_Item *it);
static Eina_Bool _item_focusable_search(Elm_Gen_Item **it, int dir);

static void _item_cache_push(Elm_Gen_Item *it, Eina_List *contents);
static void _item_mouse_down_cb(void *data, Evas *evas, Evas_Object *obj, void *event_info);
static void _item_mouse_move_cb(void *data, Evas *evas EINA_UNUSED, Evas_Object *obj, void *event_info);
static void _item_mouse_up_cb(void *data, Evas *evas, Evas_Object *obj EINA_UNUSED, void *event_info);
static void _item_edje_callbacks_add(Elm_Gen_Item *it);
static void _item_edje_callbacks_del(Elm_Gen_Item *it);
static void _item_block_calc(Item_Block *itb, Evas_Coord vw, Evas_Coord vh);
static void _item_min_calc(Elm_Gen_Item *it);
static void _item_calc(Elm_Gen_Item *it);
static void _item_mouse_callbacks_add(Elm_Gen_Item *, Evas_Object *);
static void _item_mouse_callbacks_del(Elm_Gen_Item *, Evas_Object *);
static void _access_activate_cb(void *data EINA_UNUSED,
                                Evas_Object *part_obj EINA_UNUSED,
                                Elm_Object_Item *item);
static void _item_realize(Elm_Gen_Item *it, Eina_Bool calc);
static void _item_unrealize(Elm_Gen_Item *it, Eina_Bool calc);
static void _expand_toggle_signal_cb(void *data, Evas_Object *obj EINA_UNUSED, const char *emission EINA_UNUSED, const char *source EINA_UNUSED);
static void _expand_signal_cb(void *data, Evas_Object *obj EINA_UNUSED, const char *emission EINA_UNUSED, const char *source EINA_UNUSED);
static void _contract_signal_cb(void *data, Evas_Object *obj EINA_UNUSED, const char *emission EINA_UNUSED, const char *source EINA_UNUSED);
static void _decorate_item_unrealize(Elm_Gen_Item *it);
static void _decorate_item_realize(Elm_Gen_Item *it);
static void _decorate_all_item_unrealize(Elm_Gen_Item *it);
static void _item_queue(Elm_Gen_Item *it, Eina_Compare_Cb cb);
static void _dummy_job(void *data);
static void _item_free(Elm_Gen_Item *it);
static void _evas_viewport_resize_cb(void *d, Evas *e EINA_UNUSED, void *ei EINA_UNUSED);
static int _is_item_in_viewport(int viewport_y, int viewport_h, int obj_y, int obj_h);

static const Elm_Action key_actions[] = {
   {"move", _key_action_move},
   {"select", _key_action_select},
   {"escape", _key_action_escape},
   {NULL, NULL}
};

// TIZEN_ONLY(20150828) : to prevent unnecessary genlist rendering
// _changed is called instead of evas_object_smart_changed API.
static void
_changed(Evas_Object *pan_obj)
{
   if (!pan_obj) return;
   Elm_Genlist_Pan_Data *psd = efl_data_scope_get(pan_obj, MY_PAN_CLASS);
   if (!psd) return;
   Elm_Genlist_Data *sd = psd->wsd;

   if (sd->viewport_w > 1) evas_object_smart_changed(pan_obj);
}
//

// TIZEN ONLY : for banded ux
static void
_banded_item_bg_add(Elm_Gen_Item *it, Evas_Object *target)
{
   char buf[256];
   const char *bg_area;
   Elm_Genlist_Data *sd = GL_IT(it)->wsd;

   if (sd->banded_bg_on)
     {
        bg_area = edje_object_data_get(VIEW(it), "banded_bg_area");

        if (bg_area && !GL_IT(it)->banded_bg)
          {
             GL_IT(it)->banded_bg = evas_object_rectangle_add(evas_object_evas_get(WIDGET(it)));
             edje_object_part_swallow(target, bg_area, GL_IT(it)->banded_bg);

             snprintf(buf, sizeof(buf), "elm,state,%s,visible", bg_area);
             edje_object_signal_emit(VIEW(it), buf, "elm");
          }
     }

  if (!sd->banded_bg_rect)
    {
       sd->banded_bg_rect = evas_object_rectangle_add(evas_object_evas_get(sd->obj));
       if (sd->banded_bg_on)
         evas_object_color_set(sd->banded_bg_rect, 250, 250, 250, 255);
       else
         evas_object_color_set(sd->banded_bg_rect, 0, 0, 0, 0);
       efl_canvas_group_member_add(sd->pan_obj, sd->banded_bg_rect);
       efl_ui_widget_sub_object_add(sd->obj, sd->banded_bg_rect);
       efl_gfx_entity_geometry_set(sd->banded_bg_rect, EINA_RECT(0, 0, -9999, -9999));
       evas_object_repeat_events_set(sd->banded_bg_rect, EINA_TRUE);
    }
}

static void
_banded_bg_state_check(Eo *obj, Elm_Genlist_Data *sd)
{
   ELM_WIDGET_DATA_GET_OR_RETURN(obj, wd);

   const char *banded_bg = edje_object_data_get(wd->resize_obj, "banded_bg");

   if (banded_bg && !strcmp("on", banded_bg))
     sd->banded_bg_on = EINA_TRUE;
   else
     sd->banded_bg_on = EINA_FALSE;
}

static void
_banded_item_bg_color_change(Elm_Gen_Item *it, Eina_Bool highlight)
{
   if (GL_IT(it)->banded_bg)
     {
        int alpha = 255 * (1 - 0.04 * GL_IT(it)->banded_color_index);
        int color = 250;

        if (highlight)
          alpha *= HIGHLIGHT_ALPHA_MAX;

        color = (color * alpha) / 255;
        evas_object_color_set(GL_IT(it)->banded_bg, color, color, color, alpha);
     }
}

static void
_banded_item_bg_index_color_set(Elm_Gen_Item *it_top, Evas_Coord ox, Evas_Coord oy, Evas_Coord ow, Evas_Coord oh)
{
   if (!it_top) return;
   Elm_Genlist_Data *sd = GL_IT(it_top)->wsd;
   Elm_Gen_Item *prev = NULL, *next = it_top;
   int item_count, i = 0, j = 0;
   int sign = 1;
   Eina_List *l;

   if (!sd->banded_bg_on || !next->realized) return;

   item_count = eina_list_count(l = elm_genlist_realized_items_get(WIDGET(it_top)));
   eina_list_free(l);

   while ((i < item_count) && (next))
     {
        if (!next->realized || next->hide)
          {
             prev = next;
             next = ELM_GEN_ITEM_FROM_INLIST(EINA_INLIST_GET(next)->next);
             continue;
          }

        if (next != it_top && !ELM_RECTS_INTERSECT(GL_IT(next)->scrl_x, GL_IT(next)->scrl_y,
                                GL_IT(next)->w, GL_IT(next)->h, ox, oy, ow, oh))
          break;
        else
          {
             if ((prev) &&
                 ((GL_IT(prev)->type == ELM_GENLIST_ITEM_GROUP) ||
                  (prev->itc->item_style && !strncmp("group", prev->itc->item_style, 5))))
               j = j - sign;

             if (GL_IT(next)->banded_bg)
               {
                  GL_IT(next)->banded_color_index = j;
                  _banded_item_bg_color_change(next, next->highlighted);
               }
          }

        prev = next;
        next = ELM_GEN_ITEM_FROM_INLIST(EINA_INLIST_GET(next)->next);

        j = j + sign;
        if ((j < 0) || (j > (BANDED_MAX_ITEMS - 1)))
          {
             sign = -sign;
             j += (sign * 2);
          }
        i = i + 1;
     }
}

static Eina_Bool
_is_no_select(Elm_Gen_Item *it)
{
   ELM_GENLIST_DATA_GET_FROM_ITEM(it, sd);

   if ((sd->select_mode == ELM_OBJECT_SELECT_MODE_NONE) ||
       (sd->select_mode == ELM_OBJECT_SELECT_MODE_DISPLAY_ONLY) ||
       (it->select_mode == ELM_OBJECT_SELECT_MODE_NONE) ||
       (it->select_mode == ELM_OBJECT_SELECT_MODE_DISPLAY_ONLY))
     return EINA_TRUE;
   return EINA_FALSE;
}

EOLIAN static void
_elm_genlist_pan_elm_pan_pos_set(Eo *obj, Elm_Genlist_Pan_Data *psd, Evas_Coord x, Evas_Coord y)
{
   if ((x == psd->wsd->pan_x) && (y == psd->wsd->pan_y)) return;

   if (y > psd->wsd->pan_y) psd->wsd->dir = 1;
   else psd->wsd->dir = -1;
   psd->wsd->pan_x = x;
   psd->wsd->pan_y = y;

   _changed(obj);
}

EOLIAN static void
_elm_genlist_pan_elm_pan_pos_get(const Eo *obj EINA_UNUSED, Elm_Genlist_Pan_Data *psd, Evas_Coord *x, Evas_Coord *y)
{
   if (x) *x = psd->wsd->pan_x;
   if (y) *y = psd->wsd->pan_y;
}

// TIZEN_ONLY(20150705): genlist item align feature
static Elm_Gen_Item *
_elm_genlist_pos_adjust_xy_item_get(const Evas_Object *obj,
                                    Evas_Coord x,
                                    Evas_Coord y)
{
   Item_Block *itb;

   ELM_GENLIST_CHECK(obj) NULL;
   ELM_GENLIST_DATA_GET(obj, sd);

   EINA_INLIST_FOREACH(sd->blocks, itb)
     {
        Eina_List *l = NULL;
        Elm_Gen_Item *it;
        if (!ELM_RECTS_INTERSECT(itb->x - sd->pan_x,
                                 itb->y - sd->pan_y,
                                 sd->minw, itb->minh, x, y, 1, 1))
          continue;
        EINA_LIST_FOREACH(itb->items, l, it)
          {
             Evas_Coord itx, ity, itw, ith;

             itx = itb->x + it->x - sd->pan_x;
             ity = itb->y + it->y - sd->pan_y;

             itw = (GL_IT(it)->w ? GL_IT(it)->w : sd->minw);
             ith = GL_IT(it)->minh;

             if (ELM_RECTS_INTERSECT(itx, ity, itw, ith, x, y, 1, 1))
               return it;
          }
     }

   return NULL;
}

static Elm_Gen_Item *
_adjust_item_align(Elm_Gen_Item *it)
{
   if (!it) return NULL;
   int loop_count = 0;
   int direction;
   Elm_Gen_Item *adjust_item = it;
   Elm_Object_Item *eo_item = NULL;
   Elm_Genlist_Data *sd = GL_IT(it)->wsd;

   ELM_WIDGET_DATA_GET_OR_RETURN(WIDGET(adjust_item), wd, NULL);

   if (!wd || !(wd->scroll_item_align_enable)) return it;
   if (!sd || !sd->aligned_item || (it == sd->aligned_item)) return it;

   direction = GL_IT(adjust_item)->scrl_y - GL_IT(sd->aligned_item)->scrl_y;
   while (adjust_item && loop_count < 10)
     {
        const char *align = NULL;
        if (!VIEW(adjust_item))
          {
             _item_realize(adjust_item, EINA_TRUE);
             align = edje_object_data_get(VIEW(adjust_item), "align");
             _item_unrealize(adjust_item, EINA_TRUE);
          }
        else
          align = edje_object_data_get(VIEW(adjust_item), "align");

        if (align && !strcmp(align, "off"))
          {
             if (direction < 0)
               eo_item = elm_genlist_item_prev_get(EO_OBJ(adjust_item));
             else
               eo_item = elm_genlist_item_next_get(EO_OBJ(adjust_item));
             adjust_item = efl_data_scope_get(eo_item, ELM_GENLIST_ITEM_CLASS);
          }
        else
             break;
        ++loop_count;
     }

   if (adjust_item) return adjust_item;

   adjust_item = it;
   while (adjust_item && loop_count < 10)
     {
        const char *align = NULL;
        if (!VIEW(adjust_item))
          {
             _item_realize(adjust_item, EINA_TRUE);
             align = edje_object_data_get(VIEW(adjust_item), "align");
             _item_unrealize(adjust_item, EINA_TRUE);
          }
        else
          align = edje_object_data_get(VIEW(adjust_item), "align");

        if (align && !strcmp(align, "off"))
          {
             if (direction > 0)
               eo_item = elm_genlist_item_prev_get(EO_OBJ(adjust_item));
             else
               eo_item = elm_genlist_item_next_get(EO_OBJ(adjust_item));
             adjust_item = efl_data_scope_get(eo_item, ELM_GENLIST_ITEM_CLASS);
          }
        else
             break;
        ++loop_count;
     }

   if (!adjust_item)
     adjust_item = it;

   return adjust_item;
}

EOLIAN static void
_elm_genlist_pan_elm_pan_pos_adjust(Eo *obj EINA_UNUSED, Elm_Genlist_Pan_Data *psd, int *x EINA_UNUSED, int *y)
{
   ELM_WIDGET_DATA_GET_OR_RETURN(psd->wobj, wd);
   Elm_Genlist_Data *sd = psd->wsd;

   if (!(wd->scroll_item_align_enable)) return;

   if (!y) return;

   Elm_Object_Item *eo_it;
   Evas_Coord vw, vh;
   Evas_Coord cx = 0, cy = 0;
   Evas_Coord it_y, it_h;
   Evas_Coord yy = *y;

   elm_interface_scrollable_content_viewport_geometry_get(sd->obj, NULL, NULL, &vw, &vh);
   if (!strcmp(wd->scroll_item_valign, "center"))
     {
        cx = (vw / 2);
        cy = (vh / 2) - yy;
     }

   sd->adjusted_item = _elm_genlist_pos_adjust_xy_item_get(sd->obj, cx, cy);

   if ((abs(*y) > FLICK_GAP) && (sd->adjusted_item == sd->aligned_item))
     {
        Elm_Gen_Item *adjusted_item = NULL;
        if (*y < 0)
          eo_it = elm_genlist_item_next_get(EO_OBJ(sd->adjusted_item));
        else
          eo_it = elm_genlist_item_prev_get(EO_OBJ(sd->adjusted_item));
        adjusted_item = efl_data_scope_get(eo_it, ELM_GENLIST_ITEM_CLASS);
        if (adjusted_item)
          sd->adjusted_item = adjusted_item;
     }
   sd->adjusted_item = _adjust_item_align(sd->adjusted_item);
   if (!sd->adjusted_item) return;

   cy += psd->wsd->pan_y;
   it_y = sd->adjusted_item->y + GL_IT(sd->adjusted_item)->block->y;
   it_h = GL_IT(sd->adjusted_item)->h;

   if (GL_IT(sd->adjusted_item)->h <= vh)
     {
        it_h = it_y + (it_h / 2);
        *y += (cy - it_h);
     }
   else
     {
        int it_b_y = it_y + it_h;
        int half_vh = vh/2;
        if (it_y <= cy && it_y >= cy - half_vh)
          {
             *y -= half_vh - (cy - it_y);
          }
       else if (it_b_y >= cy && it_b_y <= cy  + half_vh)
          {
             *y += half_vh - (it_b_y - cy);
          }
     }

   if (!*y && !sd->unhighlighted)
     sd->adjusted_item = NULL;
   else
     evas_object_smart_changed(sd->pan_obj);
}
//

EOLIAN static void
_elm_genlist_pan_elm_pan_pos_max_get(const Eo *obj, Elm_Genlist_Pan_Data *psd, Evas_Coord *x, Evas_Coord *y)
{
   Evas_Coord ow, oh;

   evas_object_geometry_get(obj, NULL, NULL, &ow, &oh);
   ow = psd->wsd->minw - ow;
   if (ow < 0) ow = 0;
   oh = psd->wsd->minh - oh;
   if (oh < 0) oh = 0;
   if (x) *x = ow;
   if (y) *y = oh;
}

EOLIAN static void
_elm_genlist_pan_elm_pan_pos_min_get(const Eo *obj EINA_UNUSED, Elm_Genlist_Pan_Data *_pd EINA_UNUSED, Evas_Coord *x,Evas_Coord *y)
{
   if (x) *x = 0;
   if (y) *y = 0;
}

EOLIAN static void
_elm_genlist_pan_elm_pan_content_size_get(const Eo *obj EINA_UNUSED, Elm_Genlist_Pan_Data *psd, Evas_Coord *w, Evas_Coord *h)
{
   if (w) *w = psd->wsd->minw;
   if (h) *h = psd->wsd->minh;
}

EOLIAN static void
_elm_genlist_pan_efl_canvas_group_group_del(Eo *obj, Elm_Genlist_Pan_Data *psd)
{
   ecore_job_del(psd->resize_job);

   efl_canvas_group_del(efl_super(obj, MY_PAN_CLASS));
}

EOLIAN static void
_elm_genlist_pan_efl_gfx_entity_position_set(Eo *obj, Elm_Genlist_Pan_Data *psd, Eina_Position2D pos)
{
   efl_gfx_entity_position_set(efl_super(obj, MY_PAN_CLASS), pos);
   psd->wsd->dir = 0;
   _changed(obj);
}

EOLIAN static void
_elm_genlist_pan_efl_gfx_entity_size_set(Eo *obj, Elm_Genlist_Pan_Data *psd, Eina_Size2D size)
{
   if ((size.w > 1 || size.h > 1))
     {
        if (psd->wsd->queue && !psd->wsd->queue_idle_enterer)
          {
             psd->wsd->queue_idle_enterer = ecore_idle_enterer_add(_queue_idle_enter, psd->wsd);
          }
     }
   if (psd->wsd->mode == ELM_LIST_COMPRESS &&
       psd->wsd->prev_viewport_w != size.w)
     {
        Item_Block *itb;
        ELM_WIDGET_DATA_GET_OR_RETURN(psd->wsd->obj, wd);
        EINA_INLIST_FOREACH(psd->wsd->blocks, itb)
          {
             Eina_List *l;
             Elm_Gen_Item *it;
             EINA_LIST_FOREACH(itb->items, l, it)
               {
                  if (!wd->scroll_item_align_enable && !GL_IT(it)->multiline) continue;
                  if (GL_IT(it)->wsd->realization_mode)
                    {
                       GL_IT(it)->calc_done = EINA_FALSE;
                       GL_IT(it)->block->calc_done = EINA_FALSE;
                    }
                  else
                    {
                       GL_IT(it)->resized = EINA_TRUE;
                       if (it->realized)
                         {
                            GL_IT(it)->calc_done = EINA_FALSE;
                            GL_IT(it)->block->calc_done = EINA_FALSE;
                         }
                       else _item_queue(it, NULL);
                    }
               }
          }
        psd->wsd->prev_viewport_w = size.w;
     }
   psd->wsd->viewport_w = size.w;
   psd->wsd->viewport_h = size.h;

   psd->wsd->calc_done = EINA_FALSE;
super:
   efl_gfx_entity_size_set(efl_super(obj, MY_PAN_CLASS), size);

   _changed(obj);
}

static void
_item_text_realize(Elm_Gen_Item *it,
                   Evas_Object *target,
                   const char *parts)
{
   char buf[256];

   if (it->itc->func.text_get)
     {
        Eina_List *source;
        const char *key;

        source = elm_widget_stringlist_get
           (edje_object_data_get(target, "texts"));
        EINA_LIST_FREE(source, key)
          {
             if (parts && fnmatch(parts, key, FNM_PERIOD))
               continue;

             char *s = it->itc->func.text_get
                 ((void *)WIDGET_ITEM_DATA_GET(EO_OBJ(it)), WIDGET(it), key);

             if (s)
               {
                  edje_object_part_text_escaped_set(target, key, s);
                  free(s);

                  snprintf(buf, sizeof(buf), "elm,state,%s,visible", key);
                  edje_object_signal_emit(target, buf, "elm");
               }
             else
               {
                  edje_object_part_text_set(target, key, "");
                  snprintf(buf, sizeof(buf), "elm,state,%s,hidden", key);
                  edje_object_signal_emit(target, buf, "elm");
               }
          }
        edje_object_message_signal_process(target);
        if (_elm_atspi_enabled())
          efl_access_i18n_name_changed_signal_emit(EO_OBJ(it));
     }
}

static void
_changed_size_hints(void *data, Evas *e EINA_UNUSED, Evas_Object *obj EINA_UNUSED, void *event_info EINA_UNUSED)
{
   Elm_Gen_Item *it = data;
   GL_IT(it)->calc_done = EINA_FALSE;
   GL_IT(it)->block->calc_done = EINA_FALSE;
   GL_IT(it)->wsd->calc_done = EINA_FALSE;
   _changed(GL_IT(it)->wsd->pan_obj);
}

// FIXME: There are applications which do not use elm_win as top widget.
// This is workaround! Those could not use focus!
/*
static Eina_Bool _focus_enabled(Evas_Object *obj)
{
   if (!elm_object_focus_get(obj)) return EINA_FALSE;

   const Evas_Object *win = elm_widget_top_get(obj);
   const char *type = evas_object_type_get(win);

   if (type && !strcmp(type, "elm_win"))
     {
        return elm_win_focus_highlight_enabled_get(win);
     }
   return EINA_FALSE;
}
*/

static void
_item_content_free(Evas_Object *content)
{
   // FIXME: For animation, befor del, processing edc.
   if (evas_object_smart_type_check(content, "elm_layout"))
     edje_object_message_signal_process(elm_layout_edje_get(content));
   // FIXME: If parent-child relationship was broken before 'ic'
   // is deleted, freeze_pop will not be called. ex) elm_slider
   // If layout is used instead of edje, this problme can be
   // solved.
   if (elm_widget_is(content) && (0 != elm_widget_scroll_freeze_get(content)))
     elm_widget_scroll_freeze_pop(content);
   evas_object_del(content);
}

static void
_widget_calculate_recursive(Eo *obj)
{
   Elm_Widget_Smart_Data *pd = NULL;
   Eina_List *l;
   Evas_Object *child;

   if (!efl_isa(obj, EFL_UI_WIDGET_CLASS)) return;

   pd = efl_data_scope_get(obj, EFL_UI_WIDGET_CLASS);
   if (!pd || !pd->resize_obj)
     return;

   if (!evas_object_smart_need_recalculate_get(obj) &&
       !evas_object_smart_need_recalculate_get(pd->resize_obj))
     return;

   EINA_LIST_FOREACH(pd->subobjs, l, child)
     _widget_calculate_recursive(child);

   evas_object_smart_calculate(obj);
}

static void
_item_content_realize(Elm_Gen_Item *it,
                      Evas_Object *target,
                      Eina_List **contents,
                      const char *src,
                      const char *parts)
{
   Evas_Object *content;
   char buf[256];
   Eina_List *source;
   const char *key;

   if (!parts)
     {
        EINA_LIST_FREE(*contents, content)
           _item_content_free(content);
     }
   if ((!it->itc->func.content_get) &&
      ((it->itc->version < 3) || (!it->itc->func.reusable_content_get))) return;

   source = elm_widget_stringlist_get(edje_object_data_get(target, src));

   EINA_LIST_FREE(source, key)
     {
        if (parts && fnmatch(parts, key, FNM_PERIOD))
          continue;

        Evas_Object *old = NULL;
        old = edje_object_part_swallow_get(target, key);

        // Reuse content by popping from the cache
        content = NULL;
        if (it->itc->version >= 3 && it->itc->func.reusable_content_get)
          content = it->itc->func.reusable_content_get(
             (void *)WIDGET_ITEM_DATA_GET(EO_OBJ(it)), WIDGET(it), key, old);
        if (!content)
          {
             // Call the content get
             if (it->itc->func.content_get)
               content = it->itc->func.content_get
                   ((void *)WIDGET_ITEM_DATA_GET(EO_OBJ(it)), WIDGET(it), key);
             if (!content)
               {
                  snprintf(buf, sizeof(buf), "elm,state,%s,hidden", key);
                  edje_object_signal_emit(target, buf, "elm");
                  goto out;
               }
          }
        if (content != old)
          {
             // FIXME: Genlist item doesn't update its size when the size of
             // content is changed, so deferred calculation for content should
             // be performed before realization.
             if (efl_isa(content, EFL_UI_WIDGET_CLASS))
               {
                  ELM_WIDGET_DATA_GET_OR_RETURN(content, wd);

                  // FIXME : Layout need sizing eval before group calculate
                  if (efl_class_get(content) == EFL_UI_LAYOUT_BASE_CLASS)
                    elm_layout_sizing_eval(content);

                  _widget_calculate_recursive(content);
               }

             if (!edje_object_part_swallow(target, key, content))
               {
                  ERR("%s (%p) can not be swallowed into %s",
                      evas_object_type_get(content), content, key);
                  evas_object_del(content);
                  goto out;
               }
             elm_widget_sub_object_add(WIDGET(it), content);
             if (efl_isa(content, EFL_ACCESS_OBJECT_MIXIN))
                efl_access_object_access_parent_set(content, EO_OBJ(it));
          }
        *contents = eina_list_append(*contents, content);

        if (elm_wdg_item_disabled_get(EO_OBJ(it)))
          elm_widget_disabled_set(content, EINA_TRUE);

        if (GL_IT(it)->wsd->realization_mode)
          {
             evas_object_event_callback_add
                (content, EVAS_CALLBACK_CHANGED_SIZE_HINTS,
                 _changed_size_hints, it);
          }

        snprintf(buf, sizeof(buf), "elm,state,%s,visible", key);
        edje_object_signal_emit(target, buf, "elm");

        // If content object is focused,
        // genlist item should be focused.
        if (elm_object_focused_object_get(content))
          {
             if (GL_IT(it)->wsd->focused_item != EO_OBJ(it))
               elm_object_item_focus_set(EO_OBJ(it), EINA_TRUE);

             GL_IT(it)->wsd->focused_content = content;
          }
out:
        if (old && content != old)
          {
             *contents = eina_list_remove(*contents, old);
             _item_content_free(old);
          }
     }
}

static void
_item_state_realize(Elm_Gen_Item *it,
                    Evas_Object *target,
                    const char *parts)
{
   if (it->itc->func.state_get)
     {
        Eina_List *src;
        const char *key;
        char buf[4096];

        src = elm_widget_stringlist_get
           (edje_object_data_get(target, "states"));
        EINA_LIST_FREE(src, key)
          {
             if (parts && fnmatch(parts, key, FNM_PERIOD))
               continue;

             Eina_Bool on = it->itc->func.state_get
                 ((void *)WIDGET_ITEM_DATA_GET(EO_OBJ(it)), WIDGET(it), key);

             if (on)
               {
                  snprintf(buf, sizeof(buf), "elm,state,%s,active", key);
                  edje_object_signal_emit(target, buf, "elm");
               }
             else
               {
                  snprintf(buf, sizeof(buf), "elm,state,%s,passive", key);
                  edje_object_signal_emit(target, buf, "elm");
               }
          }
     }
}

static void
_view_theme_update(Elm_Gen_Item *it, Evas_Object *view, const char *style)
{
   char buf[1024];
   const char *key;
   Eina_List *l;

   snprintf(buf, sizeof(buf), "item/%s", style ? : "default");

   Eina_Error th_ret =
      elm_widget_theme_object_set(WIDGET(it), view, "genlist", buf,
                                  elm_widget_style_get(WIDGET(it)));
   if (th_ret == EFL_UI_THEME_APPLY_ERROR_GENERIC)
     {
        ERR("%s is not a valid genlist item style. "
            "Automatically falls back into default style.",
            style);
        elm_widget_theme_object_set
          (WIDGET(it), view, "genlist", "item/default", "default");
     }

   edje_object_mirrored_set(view, efl_ui_mirrored_get(WIDGET(it)));
   edje_object_scale_set(view, efl_gfx_entity_scale_get(WIDGET(it))
                         * elm_config_scale_get());

   GL_IT(it)->multiline = EINA_FALSE;
   Eina_List *txts = elm_widget_stringlist_get
      (edje_object_data_get(view, "texts"));
   EINA_LIST_FOREACH(txts, l, key)
     {
        const Evas_Object *txt_obj = NULL;
        const char *type = NULL;
        txt_obj = edje_object_part_object_get(view, key);
        if (txt_obj) type =  evas_object_type_get(txt_obj);
        if (!type) continue;
        if (type && strcmp(type, "textblock")) continue;

        const Evas_Textblock_Style *tb_style =
           evas_object_textblock_style_get(txt_obj);
        if (tb_style)
          {
             const char *str = evas_textblock_style_get(tb_style);
             if (str)
               {
                  if (strstr(str, "wrap="))
                    {
                       GL_IT(it)->multiline = EINA_TRUE;
                       break;
                    }
               }
          }
     }
   eina_list_free(txts);
}

static Evas_Object *
_view_create(Elm_Gen_Item *it, const char *style)
{
   Evas_Object *view = edje_object_add(evas_object_evas_get(WIDGET(it)));
   efl_canvas_group_member_add(GL_IT(it)->wsd->pan_obj, view);
   elm_widget_sub_object_add(WIDGET(it), view);
   edje_object_scale_set(view, efl_gfx_entity_scale_get(WIDGET(it)) *
                         elm_config_scale_get());

   _view_theme_update(it, view, style);
   return view;
}

static void
_view_clear(Evas_Object *view, Eina_List **contents)
{
   if (!view) return;
   const char *part;
   Evas_Object *c;
   Eina_List *texts  = elm_widget_stringlist_get
     (edje_object_data_get(view, "texts"));
   EINA_LIST_FREE(texts, part)
     edje_object_part_text_set(view, part, NULL);

   if (contents)
     {
        EINA_LIST_FREE(*contents, c)
           _item_content_free(c);
     }
}

static void
_view_inflate(Evas_Object *view, Elm_Gen_Item *it, Eina_List **contents)
{
   if (!view) return;
   _item_text_realize(it, view, NULL);
   if (contents) _item_content_realize(it, view, contents, "contents", NULL);
   _item_state_realize(it, view, NULL);
}

static void
_elm_genlist_item_index_update(Elm_Gen_Item *it)
{
   if (it->position_update || GL_IT(it)->block->position_update)
     {
        efl_event_callback_legacy_call
              (WIDGET(it), ELM_GENLIST_EVENT_INDEX_UPDATE, EO_OBJ(it));
        it->position_update = EINA_FALSE;
     }
}


static void
_item_unrealize(Elm_Gen_Item *it,
                Eina_Bool calc)
{
   Evas_Object *content;
   Eina_List *cache = NULL;

   if (!it->realized) return;

   if (GL_IT(it)->wsd->reorder.it == it)
     {
        WRN("reordering item should not be unrealized");
     }

   if (!calc)
     {
        efl_event_callback_legacy_call(WIDGET(it), ELM_GENLIST_EVENT_UNREALIZED, EO_OBJ(it));
        elm_object_item_access_order_unset(EO_OBJ(it));
        //TIZEN_ONLY(20150709) Do not register children of MANAGES_DESCENDATS objects
        if (_elm_atspi_enabled())
          {
             efl_access_removed(EO_OBJ(it));
             efl_access_children_changed_del_signal_emit(sd->obj, EO_OBJ(it));
          }
        //
     }

   Eina_List *l;
   Elm_Widget_Item_Signal_Data *wisd;
   EINA_LIST_FOREACH(it->base->signals, l, wisd)
     {
        elm_object_item_signal_callback_del(EO_OBJ(it),
              wisd->emission, wisd->source, (Elm_Object_Item_Signal_Cb)wisd->func);
     }

   _decorate_item_unrealize(it);
   _decorate_all_item_unrealize(it);

   _item_edje_callbacks_del(it);

   _item_mouse_callbacks_del(it, VIEW(it));
   if (!calc && it->item->proxy)
     {
        evas_object_image_source_visible_set(it->item->proxy, EINA_TRUE);
        evas_object_smart_member_del(it->item->proxy);
        evas_object_del(it->item->proxy);
        it->item->proxy = NULL;
     }

   EINA_LIST_FREE(GL_IT(it)->flip_content_objs, content)
    evas_object_del(content);
   if (it->spacer) evas_object_del(it->spacer);
   _view_clear(VIEW(it), NULL);

// TIZEN ONLY : for banded ux
   if (!TIZEN_PROFILE_WEARABLE)
     {
        if (GL_IT(it)->banded_bg)
          ELM_SAFE_FREE(GL_IT(it)->banded_bg, evas_object_del);
        if (GL_IT(it)->wsd->banded_bg_on)
          {
             if (it->item->banded_anim) ecore_animator_del(it->item->banded_anim);
             it->item->banded_anim = NULL;
          }
     }

   if (GL_IT(it)->highlight_timer)
     {
        ecore_timer_del(GL_IT(it)->highlight_timer);
        GL_IT(it)->highlight_timer = NULL;
     }
   if (it->long_timer)
     {
        ecore_timer_del(it->long_timer);
        it->long_timer = NULL;
     }
   /* trackt */
   elm_wdg_item_track_cancel(EO_OBJ(it));

   /***********************************************************************************
    * TIZEN_ONLY_FEATURE: apply Tizen's color_class features.                         *
    ***********************************************************************************/
   if (it->base->color_classes)
     edje_object_color_class_clear(VIEW(it));
   /*******
    * END *
    *******/

   EINA_LIST_FREE(it->contents, content)
     {
        if (it->itc->version >= 3 && it->itc->func.reusable_content_get)
          {
             // Unfocus content object for initialization.
             if (GL_IT(it)->wsd->focused_content == content)
               {
                  elm_object_focus_set(content, EINA_FALSE);
                  GL_IT(it)->wsd->focused_content = NULL;
               }
             if (elm_widget_disabled_get(content))
               elm_widget_disabled_set(content, EINA_FALSE);

             cache = eina_list_append(cache, content);
          }
        else
          {
             if (GL_IT(it)->wsd->focused_content == content)
               GL_IT(it)->wsd->focused_content = NULL;
             elm_object_focus_set(content, EINA_FALSE);
             evas_object_del(content);
          }
     }

     _item_cache_push(it, cache);

   it->realized = EINA_FALSE;
}

static void
_item_block_unrealize(Item_Block *itb)
{
   Elm_Gen_Item *it;
   const Eina_List *l, *ll;
   Evas_Object *content;
   Eina_Bool unrealize = EINA_TRUE;

   // Do not check itb->realized
   // because it->item->block can be changed

   EINA_LIST_FOREACH(itb->items, l, it)
     {
        if (!(((GL_IT(it)->wsd->reorder_force) || (GL_IT(it)->wsd->reorder_mode))
              && (GL_IT(it)->wsd->reorder.it == it))
               && !it->flipped && it->realized)
          {
             /* FIXME: when content is on focused and unrealize item,
                focus is set to other object and scroll will be bring in */
             if (_elm_config->access_mode)
               {
                  EINA_LIST_FOREACH(it->contents, ll, content)
                    {
                       if (elm_object_focus_get(content))
                         {
                            unrealize = EINA_FALSE;
                            break;
                         }
                    }
               }
             if (unrealize)
               _item_unrealize(it, EINA_FALSE);
             unrealize = EINA_TRUE;
          }
     }
   itb->realized = EINA_FALSE;
}

static void
_calc(void *data)
{
   Elm_Genlist_Data *sd = data;
   Item_Block *itb;
   Evas_Coord minw = 0,  minh = 0, vw = 0, vh = 0, current_minh = 0;
   Evas_Coord processed_size = sd->minh;
   int cnt = 0;

   if (sd->calc_done) return;

// TIZEN_ONLY(20150828) : Calculate item which position on viewport area
   while (sd->queue &&
          ((sd->viewport_h > processed_size) && (cnt < MAX_ITEMS_PER_BLOCK)))
     {
        Elm_Gen_Item *tmp;
        tmp = eina_list_data_get(sd->queue);
        sd->queue = eina_list_remove_list(sd->queue, sd->queue);
        if (!_item_process(sd, tmp)) continue;
        processed_size += GL_IT(tmp)->minh;
        tmp->item->queued = EINA_FALSE;
        cnt++;
     }
//
   elm_interface_scrollable_content_viewport_geometry_get(sd->obj, NULL, NULL, &vw, &vh);
   if ((sd->mode != ELM_LIST_LIMIT) && (minw < vw)) minw = vw;

   EINA_INLIST_FOREACH(sd->blocks, itb)
     {
        itb->x = 0;
        itb->y = minh;
        _item_block_calc(itb, vw, vh);
        if (minw < itb->minw) minw = itb->minw;
        minh += itb->minh;
     }

   if ((sd->mode == ELM_LIST_COMPRESS) && (minw > vw))
      minw = vw;
   else if (minw < vw) minw = vw;

   if ((minw != sd->minw) || (minh != sd->minh))
     {
        Eina_Bool load_done = EINA_FALSE;

        current_minh = sd->minh;
        sd->minw = minw;
        sd->minh = minh;
        if (!sd->queue && (minh != current_minh)) load_done = EINA_TRUE;
        elm_layout_sizing_eval(sd->obj);
        efl_event_callback_legacy_call
          (sd->pan_obj, ELM_PAN_EVENT_CHANGED, NULL);
        if (load_done)
          evas_object_smart_callback_call(sd->obj, SIG_LOADED, NULL);
        sd->processed_sizes = 0;
     }
   sd->dir = 0;
   sd->calc_done = EINA_TRUE;
}

EOLIAN static void
_elm_genlist_elm_layout_sizing_eval(Eo *obj, Elm_Genlist_Data *sd)
{
   Evas_Coord minw = 0, minh = 0, maxw = -1, maxh = -1;

   ELM_WIDGET_DATA_GET_OR_RETURN(obj, wd);

   if (sd->on_sub_del) return;;

   evas_object_size_hint_min_get(obj, &minw, NULL);
   evas_object_size_hint_max_get(obj, &maxw, &maxh);
   edje_object_size_min_calc(wd->resize_obj, &minw, &minh);

   if (sd->scr_minw)
     {
        maxw = -1;
        minw = minw + sd->minw;
     }
   if (sd->scr_minh)
     {
        maxh = -1;
        minh = minh + sd->minh;
     }

   if ((maxw > 0) && (minw > maxw))
     minw = maxw;
   if ((maxh > 0) && (minh > maxh))
     minh = maxh;

   evas_object_size_hint_min_set(obj, minw, minh);
   evas_object_size_hint_max_set(obj, maxw, maxh);
}

static void
_content_min_limit_cb(Evas_Object *obj,
                      Eina_Bool w,
                      Eina_Bool h)
{
   ELM_GENLIST_DATA_GET(obj, sd);

   if ((sd->mode == ELM_LIST_LIMIT) ||
       (sd->mode == ELM_LIST_EXPAND)) return;
   sd->scr_minw = !!w;
   sd->scr_minh = !!h;

   elm_layout_sizing_eval(obj);
}

static void
_focus_bg_show(Elm_Gen_Item *it)
{
   Evas_Coord w;
   Evas_Coord h = 0, rectangle_h = GL_IT(it)->h;
   ELM_WIDGET_DATA_GET_OR_RETURN(WIDGET(it), wd);
   const Evas_Object *top, *bottom;
   Evas_Object *rectangle;
   const char *focus_bg;

   if (!VIEW(it)) return;
   focus_bg = edje_object_data_get(VIEW(it), "focus_bg");
   if (focus_bg && !strcmp(focus_bg, "off"))
     {
        edje_object_signal_emit(wd->resize_obj, SIGNAL_FOCUS_BG_HIDE, "elm");
        return;
     }

   evas_object_geometry_get(wd->resize_obj, NULL, NULL, &w, NULL);
   top = edje_object_part_object_get(VIEW(it), "elm.padding.focus_bg.top");
   bottom = edje_object_part_object_get(VIEW(it), "elm.padding.focus_bg.bottom");

   if (top && bottom)
     {
        evas_object_geometry_get(top, NULL, NULL, NULL, &h);
        rectangle_h -= h;
        evas_object_geometry_get(bottom, NULL, NULL, NULL, &h);
        rectangle_h -= h;
     }

   rectangle = evas_object_rectangle_add(evas_object_evas_get(wd->obj));
   evas_object_size_hint_min_set(rectangle, w, rectangle_h);
   evas_object_size_hint_max_set(rectangle, w, rectangle_h);
   evas_object_color_set(rectangle, 0, 0, 0, 0);
   elm_object_part_content_set(wd->obj, "focus_bg_size", rectangle);

   edje_object_signal_emit(wd->resize_obj, SIGNAL_FOCUS_BG_SHOW, "elm");
}

static void
_item_position(Elm_Gen_Item *it,
               Evas_Object *view,
               Evas_Coord it_x,
               Evas_Coord it_y)
{
   if (!it) return;
   if (!view) return;

   Evas_Coord ox = 0, oy = 0, ow = 0, oh = 0;
   Evas_Coord ix = 0, iy = 0, iw = 0, ih = 0;
   Elm_Gen_Item *target = it;
   Elm_Genlist_Data *sd = GL_IT(target)->wsd;
   ELM_WIDGET_DATA_GET_OR_RETURN(WIDGET(target), wsd);

   ix = it_x;
   iy = it_y;
   iw = GL_IT(target)->w;
   ih = GL_IT(target)->h;

   evas_object_resize(view, iw, ih);
   evas_object_move(view, ix, iy);
   evas_object_show(view);

   if (wsd->scroll_item_align_enable)
     {
        int color = 0;
        int pre_multiplied_color = 0;
        int resize_width = 0;
        int resize_hight = 0;
        float ratio = 0.0;
        double scale = 0.0;
        double move_v[4] = {0.49, 0.99, 0.45, 1.0};
        double scale_v[4] = {0.49, 0.14, 1, 0.63};

        evas_object_geometry_get(wsd->obj, &ox, &oy, &ow, &oh);
        int g_center = oy + oh / 2;
        int i_center = GL_IT(target)->scrl_y + GL_IT(target)->h / 2;

        if (target->item->proxy && !ELM_RECTS_INTERSECT(ix, iy, iw, ih, ox, oy, ow, oh))
          evas_object_hide(target->item->proxy);
        else
          {
             const char *vi_effect = edje_object_data_get(VIEW(target), "vi_effect");
             if (vi_effect && !strcmp(vi_effect, "on"))
               {
                  ratio = (float)abs(g_center - i_center) / (float)((oh / 2) + ih / 2);
                  color = DIMED_ITEM_COLOR + ((255 - DIMED_ITEM_COLOR) * (1.0 - ratio));
                  double scale_p = ecore_animator_pos_map_n(ratio, ECORE_POS_MAP_CUBIC_BEZIER, 4, scale_v);
                  scale = elm_config_scale_get() - (scale_p * ALIGNED_ITEM_SCALE_FACTOR);

                  resize_width = GL_IT(target)->w * (scale / elm_config_scale_get());
                  if (!((iw - resize_width) % 2))
                    iw = resize_width;
                  else
                    iw = resize_width + 1;
                  ix = GL_IT(target)->scrl_x + (GL_IT(target)->w - iw) / 2;
                  resize_hight = GL_IT(target)->h * (scale / elm_config_scale_get());

                  if (!((ih - resize_hight) % 2))
                    ih = resize_hight;
                  else
                    ih = resize_hight + 1;

                  if (i_center < g_center)
                    {
                       double move_p = ecore_animator_pos_map_n(ratio, ECORE_POS_MAP_CUBIC_BEZIER, 4, move_v);
                       iy = GL_IT(target)->scrl_y + move_p * (GL_IT(target)->h - ih);
                    }
                  else
                    iy = GL_IT(target)->scrl_y;

                  const char *dim = edje_object_data_get(view, "dim");
                  if (!dim || (dim && !strcmp(dim, "on")))
                    {
                       pre_multiplied_color = 255 * color >> 8;
                       if (!elm_object_item_disabled_get(EO_OBJ(target)))
                         evas_object_color_set(target->item->proxy, pre_multiplied_color, pre_multiplied_color, pre_multiplied_color, color);
                       else
                         evas_object_color_set(target->item->proxy, 255, 255, 255, 255);
                    }
               }
             evas_object_move(target->item->proxy, ix, iy);
             evas_object_resize(target->item->proxy, iw, ih);
             evas_object_show(target->item->proxy);

             if (!sd->on_hold &&
                 ELM_RECTS_INTERSECT(GL_IT(target)->scrl_x, GL_IT(target)->scrl_y,
                                     GL_IT(target)->w, GL_IT(target)->h,
                                     ox, g_center - CENTER_CHECK_GAP,
                                     ow, CENTER_CHECK_GAP * 2))
               {
                  if ((sd->adjusted_item == target) &&
                      ((sd->adjusted_item != sd->aligned_item) || (sd->unhighlighted)))
                    {
                       edje_object_signal_emit(VIEW(sd->adjusted_item), SIGNAL_ITEM_HIGHLIGHTED, "elm");

                       if (sd->aligned_item && sd->adjusted_item != sd->aligned_item)
                         edje_object_signal_emit(VIEW(sd->aligned_item), SIGNAL_ITEM_UNHIGHLIGHTED, "elm");
                       sd->aligned_item = sd->adjusted_item;
                       sd->unhighlighted = EINA_FALSE;
                    }

                  if (sd->adjusted_item == target &&
                      sd->aligned_item == sd->adjusted_item &&
                      ((i_center >= g_center && i_center < g_center + 10) ||
                       (i_center <= g_center && i_center > g_center - 10)))
                    {
                       sd->adjusted_item = NULL;
                       _focus_bg_show(sd->aligned_item);
                    }
               }
          }
     }
}

static void
_item_all_position(Elm_Gen_Item *it,
                   Evas_Coord it_x,
                   Evas_Coord it_y)
{
   if (!it) return;

   if (it->deco_all_view)
     _item_position(it, it->deco_all_view, it_x, it_y);
   else if (GL_IT(it)->deco_it_view)
     _item_position(it, GL_IT(it)->deco_it_view, it_x, it_y);
   else
     _item_position(it, VIEW(it), it_x, it_y);
}

static void
_decorate_item_unrealize(Elm_Gen_Item *it)
{
   if (!GL_IT(it)->deco_it_view) return;

   edje_object_part_unswallow(GL_IT(it)->deco_it_view, VIEW(it));
   _view_clear(GL_IT(it)->deco_it_view, &(GL_IT(it)->deco_it_contents));
   evas_object_del(GL_IT(it)->deco_it_view);
   GL_IT(it)->deco_it_view = NULL;

   _item_mouse_callbacks_add(it, VIEW(it));
   evas_object_smart_member_add(VIEW(it), GL_IT(it)->wsd->pan_obj);
}

static void
_decorate_all_item_realize(Elm_Gen_Item *it, Eina_Bool effect)
{
   if (it->deco_all_view || !it->itc->decorate_all_item_style) return;

   // Before adding & swallowing, delete it from smart member
   evas_object_smart_member_del(VIEW(it));

   it->deco_all_view = _view_create(it, it->itc->decorate_all_item_style);
   _view_inflate(it->deco_all_view, it, &(GL_IT(it)->deco_all_contents));
   edje_object_part_swallow
      (it->deco_all_view, "elm.swallow.decorate.content", VIEW(it));
   evas_object_show(it->deco_all_view);

   if (elm_wdg_item_disabled_get(EO_OBJ(it)))
     edje_object_signal_emit(it->deco_all_view, SIGNAL_DISABLED, "elm");
   if (it->selected)
     edje_object_signal_emit(it->deco_all_view, SIGNAL_SELECTED, "elm");
   if (GL_IT(it)->wsd->reorder_force ||
       GL_IT(it)->wsd->reorder_mode)
     edje_object_signal_emit
       (it->deco_all_view, SIGNAL_REORDER_MODE_SET, "elm");

   if (effect)
     {
        edje_object_signal_emit(VIEW(it),
                                SIGNAL_DECORATE_ENABLED_EFFECT, "elm");
        edje_object_signal_emit(it->deco_all_view,
                                SIGNAL_DECORATE_ENABLED_EFFECT, "elm");
     }
   else
     {
        edje_object_signal_emit(VIEW(it), SIGNAL_DECORATE_ENABLED, "elm");
        edje_object_signal_emit(it->deco_all_view,
                                SIGNAL_DECORATE_ENABLED,"elm");
     }
   if (it->flipped)
     edje_object_signal_emit(it->deco_all_view, SIGNAL_FLIP_ENABLED, "elm");

   _item_mouse_callbacks_add(it, it->deco_all_view);
   _item_mouse_callbacks_del(it, VIEW(it));
}

static void
_expand_toggle_signal_cb(void *data,
                         Evas_Object *obj EINA_UNUSED,
                         const char *emission EINA_UNUSED,
                         const char *source EINA_UNUSED)
{
   Elm_Gen_Item *it = data;

   if (it->item->expanded)
     efl_event_callback_legacy_call(WIDGET(it), ELM_GENLIST_EVENT_CONTRACT_REQUEST, EO_OBJ(it));
   else
     efl_event_callback_legacy_call(WIDGET(it), ELM_GENLIST_EVENT_EXPAND_REQUEST, EO_OBJ(it));
}

static void
_expand_signal_cb(void *data,
                  Evas_Object *obj EINA_UNUSED,
                  const char *emission EINA_UNUSED,
                  const char *source EINA_UNUSED)
{
   Elm_Gen_Item *it = data;

   if (!it->item->expanded)
     efl_event_callback_legacy_call(WIDGET(it), ELM_GENLIST_EVENT_EXPAND_REQUEST, EO_OBJ(it));
}

static void
_contract_signal_cb(void *data,
                    Evas_Object *obj EINA_UNUSED,
                    const char *emission EINA_UNUSED,
                    const char *source EINA_UNUSED)
{
   Elm_Gen_Item *it = data;

   if (it->item->expanded)
     efl_event_callback_legacy_call(WIDGET(it), ELM_GENLIST_EVENT_CONTRACT_REQUEST, EO_OBJ(it));
}


static void
_item_cache_free(Elm_Genlist_Data *sd, Item_Cache *ic)
{
   Evas_Object *c;
   if (!ic) return;

   if (ic->base_view) efl_del(ic->base_view);
   ic->item_class = NULL;
   EINA_LIST_FREE(ic->contents, c)
     {
        _item_content_free(c);
     }
   sd->item_cache = eina_inlist_remove(sd->item_cache, EINA_INLIST_GET(ic));
   sd->item_cache_count--;
   // Free should be performed after inlist is poped
   free(ic);
}

static void
_item_cache_all_free(Elm_Genlist_Data *sd)
{
   // It would be better not to use
   // EINA_INLIST_FOREACH or EINA_INLIST_FOREACH_SAFE
   while (sd->item_cache)
     {
        Item_Cache *ic = EINA_INLIST_CONTAINER_GET(sd->item_cache->last, Item_Cache);
        _item_cache_free(sd, ic);
     }
   sd->item_cache = NULL;
   sd->item_cache_count = 0;
}

static void
_item_cache_push(Elm_Gen_Item *it, Eina_List *contents)
{
   Elm_Genlist_Data *sd = GL_IT(it)->wsd;
   Item_Cache *ic = NULL;
   Evas_Object *content;

   if (sd->no_cache)
     {
        if (VIEW(it)) evas_object_del(VIEW(it));
        VIEW(it) = NULL;
        EINA_LIST_FREE(contents, content)
          {
             _item_content_free(content);
          }
        return;
     }

   if (sd->item_cache_count >= sd->item_cache_max)
     {
        ic = EINA_INLIST_CONTAINER_GET(sd->item_cache->last, Item_Cache);
        _item_cache_free(sd, ic);
     }

   ic = ELM_NEW(Item_Cache);
   if (!ic)
     {
        if (VIEW(it)) evas_object_del(VIEW(it));
        VIEW(it) = NULL;
        EINA_LIST_FREE(contents, content)
          {
             _item_content_free(content);
          }
        return;
     }
   // set item's state as default before pushing item into cache.
   edje_object_signal_emit(VIEW(it), SIGNAL_DEFAULT, "elm");

   edje_object_mirrored_set(VIEW(it),
                            efl_ui_mirrored_get(WIDGET(it)));
   edje_object_scale_set(VIEW(it),
                         efl_gfx_entity_scale_get(WIDGET(it))
                         * elm_config_scale_get());

   ic->base_view = VIEW(it);
   ic->multiline = GL_IT(it)->multiline;
   ic->item_class = it->itc;
   ic->contents = contents;
   evas_object_hide(ic->base_view);
   evas_object_move(ic->base_view, -9999, -9999);

   sd->item_cache = eina_inlist_prepend(sd->item_cache, EINA_INLIST_GET(ic));
   sd->item_cache_count++;

   VIEW(it) = NULL;
}

static Eina_Bool
_item_cache_pop(Elm_Gen_Item *it)
{
   Item_Cache *ic = NULL;
   Eina_Inlist *l;
   Elm_Genlist_Data *sd = GL_IT(it)->wsd;

   if (!it->itc) return EINA_FALSE;

   EINA_INLIST_FOREACH_SAFE(sd->item_cache, l, ic)
     {
        if (it->itc && ic->item_class &&
            (it->itc == ic->item_class))
          {
             sd->item_cache =
                eina_inlist_remove(sd->item_cache, EINA_INLIST_GET(ic));
             sd->item_cache_count--;

             VIEW(it) = ic->base_view;
             GL_IT(it)->multiline = ic->multiline;

             eina_list_free(ic->contents);
             ic->contents = NULL;
             free(ic);
             return EINA_TRUE;
          }
     }
   return EINA_FALSE;
}


static char *
_access_info_cb(void *data, Evas_Object *obj EINA_UNUSED)
{
   char *ret;
   Eina_Strbuf *buf;

   Elm_Gen_Item *it = (Elm_Gen_Item *)data;
   ELM_GENLIST_ITEM_CHECK_OR_RETURN(it, NULL);

   buf = eina_strbuf_new();

   if (it->itc->func.text_get)
     {
        const Eina_List *l;
        const char *key;

        if (!(it->texts)) it->texts =
          elm_widget_stringlist_get(edje_object_data_get(VIEW(it), "texts"));

        EINA_LIST_FOREACH(it->texts, l, key)
          {
             char *s = it->itc->func.text_get
                ((void *)WIDGET_ITEM_DATA_GET(EO_OBJ(it)), WIDGET(it), key);

             if (s)
               {
                  if (eina_strbuf_length_get(buf) > 0) eina_strbuf_append(buf, ", ");
                  eina_strbuf_append(buf, s);
                  free(s);
               }
          }
     }

   ret = eina_strbuf_string_steal(buf);
   eina_strbuf_free(buf);
   return ret;
}

static char *
_access_state_cb(void *data, Evas_Object *obj EINA_UNUSED)
{
   Elm_Gen_Item *it = (Elm_Gen_Item *)data;
   ELM_GENLIST_ITEM_CHECK_OR_RETURN(it, NULL);

   if (it->base->disabled)
     return strdup(E_("State: Disabled"));

   return NULL;
}

static void
_access_on_highlight_cb(void *data)
{
   Evas_Coord x, y, w, h;
   Evas_Coord sx, sy, sw, sh;
   Elm_Gen_Item *it = (Elm_Gen_Item *)data;
   ELM_GENLIST_ITEM_CHECK_OR_RETURN(it);

   evas_object_geometry_get(it->base->view, &x, &y, &w, &h);
   // XXX There would be a reason.
   if ((w == 0) && (h == 0)) return;

   evas_object_geometry_get(it->base->widget, &sx, &sy, &sw, &sh);
   if ((x < sx) || (y < sy) || ((x + w) > (sx + sw)) || ((y + h) > (sy + sh)))
     elm_genlist_item_bring_in(EO_OBJ(it),
                               ELM_GENLIST_ITEM_SCROLLTO_IN);
}


static void
_item_mouse_callbacks_add(Elm_Gen_Item *it,
                          Evas_Object *view)
{
   evas_object_event_callback_add
     (view, EVAS_CALLBACK_MOUSE_DOWN, _item_mouse_down_cb, it);
   evas_object_event_callback_add
     (view, EVAS_CALLBACK_MOUSE_UP, _item_mouse_up_cb, it);
   evas_object_event_callback_add
      (view, EVAS_CALLBACK_MOUSE_MOVE, _item_mouse_move_cb, it);
}

static void
_item_mouse_callbacks_del(Elm_Gen_Item *it,
                          Evas_Object *view)
{
   evas_object_event_callback_del_full
     (view, EVAS_CALLBACK_MOUSE_DOWN, _item_mouse_down_cb, it);
   evas_object_event_callback_del_full
     (view, EVAS_CALLBACK_MOUSE_UP, _item_mouse_up_cb, it);
   evas_object_event_callback_del_full
      (view, EVAS_CALLBACK_MOUSE_MOVE, _item_mouse_move_cb, it);

}

static void
_item_edje_callbacks_add(Elm_Gen_Item *it)
{
   edje_object_signal_callback_add
      (VIEW(it), "elm,action,expand,toggle", "elm",
       _expand_toggle_signal_cb, it);
   edje_object_signal_callback_add
      (VIEW(it), "elm,action,expand", "elm", _expand_signal_cb, it);
   edje_object_signal_callback_add
      (VIEW(it), "elm,action,contract", "elm", _contract_signal_cb, it);
}

static void
_item_edje_callbacks_del(Elm_Gen_Item *it)
{
   edje_object_signal_callback_del_full(VIEW(it), "elm,action,expand,toggle",
                                        "elm", _expand_toggle_signal_cb, it);
   edje_object_signal_callback_del_full(VIEW(it), "elm,action,expand", "elm",
                                        _expand_signal_cb, it);
   edje_object_signal_callback_del_full(VIEW(it), "elm,action,contract", "elm",
                                        _contract_signal_cb, it);
}

static void
_item_realize(Elm_Gen_Item *it,
              Eina_Bool calc)
{
   const char *treesize;

   if (it->realized) return;
   Elm_Genlist_Data *sd = GL_IT(it)->wsd;
   ELM_WIDGET_DATA_GET_OR_RETURN(sd->obj, wd);
   Evas_Object *win = elm_widget_top_get(sd->obj);

   it->realized = EINA_TRUE;

   if (!_item_cache_pop(it))
     VIEW(it) = _view_create(it, it->itc->item_style);

   /***********************************************************************************
    * TIZEN_ONLY_FEATURE: apply Tizen's color_class features.                         *
    ***********************************************************************************/
   if (it->base->color_classes)
     _elm_widget_item_color_class_update(it->base);
   /*******
    * END *
    *******/

   treesize = edje_object_data_get(VIEW(it), "treesize");
   if (treesize)
     {
        int tsize = atoi(treesize);
        if ((tsize > 0) &&
            it->parent &&
            (GL_IT(it->parent)->type == ELM_GENLIST_ITEM_TREE))
          {
             it->spacer =
                evas_object_rectangle_add(evas_object_evas_get(WIDGET(it)));
             evas_object_color_set(it->spacer, 0, 0, 0, 0);
             evas_object_size_hint_min_set
                (it->spacer, (GL_IT(it)->expanded_depth * tsize) *
                 elm_config_scale_get(), 1);
             edje_object_part_swallow(VIEW(it), "elm.swallow.pad", it->spacer);
          }
     }
   _view_inflate(VIEW(it), it, &it->contents);
   if (wd->scroll_item_align_enable && !calc)
     {
        const char *vi_effect = edje_object_data_get(VIEW(it), "vi_effect");
        if (vi_effect && !strcmp(vi_effect, "on"))
          {
             Evas_Object *clip = evas_object_clip_get(VIEW(it));
             const Eina_List *clipees = evas_object_clipees_get(clip);
             // FIXME: This clipee rect is prevent white rect generation by all clip unset.
             if ((eina_list_count(clipees) <= 2) && !sd->clipee)
               {
                  sd->clipee = evas_object_rectangle_add(evas_object_evas_get(WIDGET(it)));
                  evas_object_color_set(sd->clipee, 0, 0, 0, 0);
                  evas_object_clip_set(sd->clipee, clip);
               }
             if (!it->item->proxy)
               {
                  it->item->proxy = evas_object_image_filled_add(evas_object_evas_get(wd->obj));
                  evas_object_smart_member_add(it->item->proxy, sd->pan_obj);
               }
             evas_object_clip_set(it->item->proxy, evas_object_clip_get(sd->pan_obj));
             evas_object_clip_unset(VIEW(it));
             evas_object_image_source_set(it->item->proxy, VIEW(it));
             evas_object_image_source_visible_set(it->item->proxy, EINA_FALSE);
             evas_object_image_source_events_set(it->item->proxy, EINA_TRUE);
             evas_object_image_source_clip_set(it->item->proxy, EINA_FALSE);
             evas_object_size_hint_min_set(it->item->proxy, GL_IT(it)->w, GL_IT(it)->h);
             evas_object_repeat_events_set(it->item->proxy, EINA_TRUE);
          }
     }

   _item_mouse_callbacks_add(it, VIEW(it));
   // Expand can be supported by only VIEW(it)
   _item_edje_callbacks_add(it);

   if (elm_wdg_item_disabled_get(EO_OBJ(it)))
     edje_object_signal_emit(VIEW(it), SIGNAL_DISABLED, "elm");
   else
     edje_object_signal_emit(VIEW(it), SIGNAL_ENABLED, "elm");
   if (it->selected)
     {
        if (!TIZEN_PROFILE_WEARABLE)
          {
             if (sd->banded_bg_on)
               _banded_item_bg_color_change(it, EINA_TRUE);
          }
        edje_object_signal_emit(VIEW(it), SIGNAL_SELECTED, "elm");
        evas_object_smart_callback_call(WIDGET(it), SIG_HIGHLIGHTED, EO_OBJ(it));
     }
   if (GL_IT(it)->expanded)
     edje_object_signal_emit(VIEW(it), SIGNAL_EXPANDED, "elm");
   if (sd->reorder_force || sd->reorder_mode)
     edje_object_signal_emit(VIEW(it), SIGNAL_REORDER_MODE_SET, "elm");
   if (GL_IT(it)->expanded_depth > 0)
     edje_object_signal_emit(VIEW(it), SIGNAL_BG_CHANGE, "elm");
   if (it->flipped)
     {
        _item_content_realize(it, VIEW(it), &GL_IT(it)->flip_contents,
                              "flips", NULL);
        edje_object_signal_emit(VIEW(it), SIGNAL_FLIP_ENABLED, "elm");
     }

   if (elm_win_focus_highlight_enabled_get(win) && (EO_OBJ(it) == sd->focused_item))
     {
        edje_object_signal_emit(VIEW(it), SIGNAL_FOCUSED, "elm");
     }
   if (sd->decorate_all_mode && it->itc->decorate_all_item_style)
     {
        _decorate_all_item_realize(it, EINA_FALSE);
     }
   else if (it->decorate_it_set && it->itc->decorate_item_style &&
            (sd->mode_item == it))
     {
        _decorate_item_realize(it);
     }
   it->realized = EINA_TRUE;
   if (!calc)
     {
        _elm_genlist_item_index_update(it);

        if (it->tooltip.content_cb)
          {
             elm_wdg_item_tooltip_content_cb_set(EO_OBJ(it),
                  it->tooltip.content_cb, it->tooltip.data, NULL);
             elm_wdg_item_tooltip_style_set(EO_OBJ(it), it->tooltip.style);
             elm_wdg_item_tooltip_window_mode_set(EO_OBJ(it), it->tooltip.free_size);
          }
        if (it->mouse_cursor)
           elm_wdg_item_cursor_set(EO_OBJ(it), it->mouse_cursor);

        //TIZEN_ONLY(20150709) Do not register children of MANAGES_DESCENDATS objects
        if (_elm_atspi_enabled())
          {
             efl_access_added(EO_OBJ(it));
             efl_access_children_changed_added_signal_emit(sd->obj, EO_OBJ(it));
          }
        //
        if (!TIZEN_PROFILE_WEARABLE)
          _banded_item_bg_add(it, VIEW(it));

        // Register accessibility before realized callback
        // because user can customize accessibility.
        efl_event_callback_legacy_call(WIDGET(it), ELM_GENLIST_EVENT_REALIZED, EO_OBJ(it));

     }
   edje_object_message_signal_process(VIEW(it));
   if (it->deco_all_view)
      edje_object_message_signal_process(it->deco_all_view);
   if (GL_IT(it)->resized)
     {
        sd->queue = eina_list_remove(sd->queue, it);
        GL_IT(it)->queued = EINA_FALSE;
        GL_IT(it)->resized = EINA_FALSE;
        GL_IT(it)->calc_done = EINA_FALSE;
        GL_IT(it)->block->calc_done = EINA_FALSE;
        sd->calc_done = EINA_FALSE;
     }
   if (sd->atspi_item_to_highlight == it)
     {
        sd->atspi_item_to_highlight = NULL;
        elm_object_accessibility_highlight_set(EO_OBJ(it), EINA_TRUE);
        //TIZEN_ONLY(20170724): grab highlight using unrealized item
        efl_access_state_changed_signal_emit(EO_OBJ(it),
                                     ELM_ATSPI_STATE_HIGHLIGHTED, EINA_TRUE);
        evas_object_smart_callback_call(WIDGET(it),
          SIG_WIDGET_ATSPI_HIGHLIGHTED, EO_OBJ(it));
        //
     }
}

static Eina_Bool
_reorder_anim(void *data)
{
   Eina_List *l;
   Elm_Gen_Item *it;
   Elm_Genlist_Data *sd = data;
   Evas_Coord oy, r_y, r_y_scrl, r_y_scrl_max, r_offset;
   evas_object_geometry_get(sd->pan_obj, NULL, &oy, NULL, NULL);

   r_y = sd->reorder.it->y + sd->reorder.it->item->block->y;
   r_y_scrl = sd->reorder.it->item->scrl_y;
   r_y_scrl_max = r_y_scrl + sd->reorder.it->item->h;


   // FIXME: 1. Reorder animation time should be equal regardless of item's height
   //        2. All item have same animation speed in reordering.
   // To support 1 and 2, offset value is scaled to reorder item's height.
   r_offset = (Evas_Coord) (GL_IT(sd->reorder.it)->h / REORDER_ANIM_OFFSET);
   if (r_offset < REORDER_ANIM_OFFSET) r_offset = REORDER_ANIM_OFFSET;


   EINA_LIST_FOREACH(sd->reorder.move_items, l, it)
     {
        Evas_Coord it_y, it_y_scrl_center;
        it_y = it->y + GL_IT(it)->block->y;
        it_y_scrl_center = GL_IT(it)->scrl_y + (GL_IT(it)->h / 2);

        if ((sd->reorder.dir == -1) && (r_y_scrl <= it_y_scrl_center))
           GL_IT(it)->reorder_offset += r_offset;
        else if ((sd->reorder.dir == 1) && (r_y_scrl_max >= it_y_scrl_center))
           GL_IT(it)->reorder_offset -= r_offset;

        if (r_y > it_y)
          {
             if (sd->reorder.it->item->h < GL_IT(it)->reorder_offset)
               GL_IT(it)->reorder_offset = sd->reorder.it->item->h;
             else if (0 > GL_IT(it)->reorder_offset)
               GL_IT(it)->reorder_offset = 0;
          }
        else if (r_y < it_y)
          {
             if (-(sd->reorder.it->item->h) > GL_IT(it)->reorder_offset)
               GL_IT(it)->reorder_offset = -(sd->reorder.it->item->h);
             else if (0 < GL_IT(it)->reorder_offset)
               GL_IT(it)->reorder_offset = 0;
          }
     }
   if (!sd->reorder.move_items)
     {
        sd->reorder.anim = NULL;
        return ECORE_CALLBACK_CANCEL;
     }

   _changed(sd->pan_obj);
   return ECORE_CALLBACK_RENEW;
}

static int
_reorder_space_get(Elm_Gen_Item *it)
{
   Elm_Genlist_Data *sd = GL_IT(it)->wsd;
   Evas_Coord oy, r_y, it_y, r_y_scrl, r_y_scrl_max, it_y_center;
   evas_object_geometry_get(sd->pan_obj, NULL, &oy, NULL, NULL);

   if (GL_IT(it)->type == ELM_GENLIST_ITEM_GROUP) return 0;
   if (sd->reorder.it->parent != it->parent) return 0;

   r_y = sd->reorder.it->y + sd->reorder.it->item->block->y;
   it_y = it->y + GL_IT(it)->block->y;

   r_y_scrl = sd->reorder.it->item->scrl_y;
   r_y_scrl_max = r_y_scrl + sd->reorder.it->item->h;
   it_y_center = it_y + (GL_IT(it)->h / 2) - sd->pan_y + oy;

   if ((r_y > it_y) && (r_y_scrl <= it_y_center))
     {
        if (!eina_list_data_find(sd->reorder.move_items, it))
           sd->reorder.move_items = eina_list_append(sd->reorder.move_items, it);
        if (!sd->reorder.anim)
           sd->reorder.anim = ecore_animator_add(_reorder_anim,
                                                 GL_IT(it)->wsd);
     }
   else if ((r_y < it_y) && (r_y_scrl_max >= it_y_center))
     {
        if (!eina_list_data_find(sd->reorder.move_items, it))
           sd->reorder.move_items = eina_list_append(sd->reorder.move_items, it);
        if (!sd->reorder.anim)
           sd->reorder.anim = ecore_animator_add(_reorder_anim,
                                                 GL_IT(it)->wsd);
     }
   return GL_IT(it)->reorder_offset;
}

static void
_reorder_calc(Elm_Gen_Item *it, Elm_Genlist_Data *sd)
{
   Elm_Gen_Item *git = NULL;
   Elm_Gen_Item *ngit = NULL;

   Evas_Coord ox, oy, ow, oh;
   evas_object_geometry_get(sd->pan_obj, &ox, &oy, &ow, &oh);

   // Do not cross the parent group items and
   // Do not exceeds viewport
   git = it->parent;
   ngit = eina_list_data_get(eina_list_next
                             (eina_list_data_find_list
                              (sd->group_items, git)));
   if (git && git->realized && (GL_IT(it)->scrl_y <= (GL_IT(git)->scrl_y + GL_IT(git)->h)))
     {
        GL_IT(it)->scrl_y = GL_IT(git)->scrl_y + GL_IT(git)->h;
     }
   else if (ngit && ngit->realized && (GL_IT(it)->scrl_y >= (GL_IT(ngit)->scrl_y - GL_IT(it)->h)))
     {
        GL_IT(it)->scrl_y = GL_IT(ngit)->scrl_y - GL_IT(ngit)->h;
     }

   if (GL_IT(it)->scrl_y < oy)
     {
        GL_IT(it)->scrl_y = oy;
        elm_interface_scrollable_content_region_show((sd)->obj, sd->pan_x, sd->pan_y
                                                                      - REORDER_FASTER, ow, oh);
     }
   else if (GL_IT(it)->scrl_y + GL_IT(it)->h > oy + oh)
     {
        GL_IT(it)->scrl_y = oy + oh - GL_IT(it)->h;
        elm_interface_scrollable_content_region_show((sd)->obj, sd->pan_x, sd->pan_y
                                                                      + REORDER_FASTER, ow, oh);
     }

   if (TIZEN_PROFILE_WEARABLE)
     {
        _item_all_position(it, GL_IT(it)->scrl_x - REORDERED_ITEM_OFFSET,
                           GL_IT(it)->scrl_y - REORDERED_ITEM_OFFSET);
     }
   else
     {
        _item_all_position(it, GL_IT(it)->scrl_x, GL_IT(it)->scrl_y - REORDERED_ITEM_OFFSET);
     }

   if (it->deco_all_view)
      evas_object_raise(it->deco_all_view);
   else if (GL_IT(it)->deco_it_view)
      evas_object_raise(GL_IT(it)->deco_it_view);
   else
      evas_object_raise(VIEW(it));
}

#ifdef GENLIST_FX_SUPPORT
static Eina_Bool
_add_fx_anim(void *data)
{
   Elm_Genlist_Data *sd = data;
   Elm_Gen_Item *it;

   sd->add_fx.cnt--;

   _changed(sd->pan_obj);
   if (sd->add_fx.cnt <= 0)
     {
        while (sd->add_fx.items)
          {
             it = eina_list_data_get(sd->add_fx.items);
             sd->add_fx.items = eina_list_remove(sd->add_fx.items, it);
             if (it->deco_all_view)
                evas_object_color_set(it->deco_all_view, 255, 255, 255, 255);
             else if (it->item->deco_it_view)
                evas_object_color_set(it->item->deco_it_view, 255, 255, 255, 255);
             else if (VIEW(it))
                evas_object_color_set(VIEW(it), 255, 255, 255, 255);
          }
        sd->add_fx.cnt = ANIM_CNT_MAX;
        sd->add_fx.anim = NULL;
        return ECORE_CALLBACK_CANCEL;
     }
   return ECORE_CALLBACK_RENEW;
}

static int
_add_fx_space_get(Elm_Gen_Item *it)
{
   Elm_Gen_Item *itt;
   Elm_Genlist_Data *sd = it->item->wsd;
   Evas_Coord size = 0;

   if (it->deco_all_view) evas_object_raise(it->deco_all_view);
   else if (it->item->deco_it_view) evas_object_raise(it->item->deco_it_view);
   else if (VIEW(it)) evas_object_raise(VIEW(it));

   itt = ELM_GEN_ITEM_FROM_INLIST(EINA_INLIST_GET(it)->prev);
   while (itt && itt->realized)
     {
        if (eina_list_data_find_list(sd->add_fx.items, itt))
          {
             size += itt->item->minh;
          }
        itt = ELM_GEN_ITEM_FROM_INLIST(EINA_INLIST_GET(itt)->prev);
     }
   size = size * ((double)sd->add_fx.cnt/ANIM_CNT_MAX);
   return size;
}

static Eina_Bool
_del_fx_anim(void *data)
{
   Elm_Genlist_Data *sd = data;
   Elm_Gen_Item *it;

   sd->del_fx.cnt--;

   _changed(sd->pan_obj);
   if (sd->del_fx.cnt <= 0)
     {
        while (sd->del_fx.pending_items)
          {
             it = eina_list_data_get(sd->del_fx.pending_items);
             sd->del_fx.pending_items = eina_list_remove(sd->del_fx.pending_items, it);
             _item_free(it);
          }
        while (sd->del_fx.items)
          {
             it = eina_list_data_get(sd->del_fx.items);
             sd->del_fx.items = eina_list_remove(sd->del_fx.items, it);
             if (it->deco_all_view)
                evas_object_color_set(it->deco_all_view, 255, 255, 255, 255);
             else if (it->item->deco_it_view)
                evas_object_color_set(it->item->deco_it_view, 255, 255, 255, 255);
             else if (VIEW(it))
                evas_object_color_set(VIEW(it), 255, 255, 255, 255);
             _item_free(it);
          }
        sd->del_fx.cnt = ANIM_CNT_MAX;
        sd->del_fx.anim = NULL;
        return ECORE_CALLBACK_CANCEL;
     }
   return ECORE_CALLBACK_RENEW;
}

static int
_del_fx_space_get(Elm_Gen_Item *it)
{
   Elm_Gen_Item *itt;
   Elm_Genlist_Data *sd = it->item->wsd;
   Evas_Coord size = 0;

   if (it->deco_all_view) evas_object_raise(it->deco_all_view);
   else if (it->item->deco_it_view) evas_object_raise(it->item->deco_it_view);
   else if (VIEW(it)) evas_object_raise(VIEW(it));
   itt = ELM_GEN_ITEM_FROM_INLIST(EINA_INLIST_GET(it)->prev);
   while (itt && itt->realized)
     {
        if (eina_list_data_find_list(sd->del_fx.items, itt))
          {
             size += itt->item->minh;
          }
        itt = ELM_GEN_ITEM_FROM_INLIST(EINA_INLIST_GET(itt)->prev);
     }
   size = size * (1 - ((double)sd->del_fx.cnt/ANIM_CNT_MAX));
   return size;
}
#endif

static void
_item_block_realize(Item_Block *itb, Eina_Bool force)
{
   Elm_Genlist_Data *sd = itb->sd;
   Elm_Gen_Item *it;
   Elm_Gen_Item *aligned_item = NULL;
   Elm_Gen_Item *top_drawn_item = NULL;
   const Eina_List *l, *ll;
   Evas_Object *content;
   Eina_Bool unrealize = EINA_TRUE;
   Evas_Coord ox = 0, oy = 0, ow = 0, oh = 0, cvx = 0, cvy = 0, cvw = 0, cvh = 0;
   ELM_WIDGET_DATA_GET_OR_RETURN(sd->obj, wsd);

   // Do not check itb->realized
   // because it->item->block can be changed

   evas_object_geometry_get(sd->pan_obj, &ox, &oy, &ow, &oh);
   evas_output_viewport_get(evas_object_evas_get(sd->obj),
                            &cvx, &cvy, &cvw, &cvh);

   if (_elm_config->access_mode || _elm_atspi_enabled())
     {
        // If access is on, realize more items (3 times)
        cvy = cvy - cvh;
        cvh = cvh * 3;
     }
#ifdef GENLIST_FX_SUPPORT
   else if (sd->del_fx.anim)
     {
        int size = 0;
        EINA_LIST_FOREACH(sd->del_fx.items, l, it)
          {
             size += it->item->minh;
          }
        cvh += size;
     }
   else if (sd->add_fx.anim)
     {
        int size = 0;
        EINA_LIST_FOREACH(sd->add_fx.items, l, it)
          {
             size += it->item->minh;
          }
        cvh += size;
     }
#endif

   if (sd->reorder.it)  _reorder_calc(sd->reorder.it, sd);
   EINA_LIST_FOREACH(itb->items, l, it)
     {
        if (sd->reorder.it == it) continue;
        if (!it->filtered) _item_filtered_get(it);
        if (it->hide)
          {
             if (it->realized) evas_object_hide(VIEW(it));
             continue;
          }
        GL_IT(it)->scrl_x = it->x + itb->x - sd->pan_x + ox;
        GL_IT(it)->scrl_y = it->y + itb->y - sd->pan_y + oy;
        GL_IT(it)->w = sd->minw;
        GL_IT(it)->h = GL_IT(it)->minh;

        if (!TIZEN_PROFILE_WEARABLE)
          {
             //Kiran only
             if (!top_drawn_item)
               {
                  if (GL_IT(it)->scrl_y <= oy && ELM_RECTS_INTERSECT(GL_IT(it)->scrl_x, GL_IT(it)->scrl_y,
                                                                     GL_IT(it)->w, GL_IT(it)->h, ox, oy, ow, oh))
                    {
                       top_drawn_item = it;
                       sd->top_drawn_item = top_drawn_item;
                    }
               }
          }

        if (wsd->scroll_item_align_enable)
          {
             if (!sd->aligned_item)
               {
                  if (ELM_RECTS_INTERSECT(GL_IT(it)->scrl_x, GL_IT(it)->scrl_y,
                                          GL_IT(it)->w, GL_IT(it)->h,
                                          ox + ow / 2, oy + oh / 2, 1, 1))
                    aligned_item = sd->aligned_item = it;
               }
          }
        if (force || sd->realization_mode ||
            it->flipped ||
           (ELM_RECTS_INTERSECT(GL_IT(it)->scrl_x, GL_IT(it)->scrl_y,
                                 GL_IT(it)->w, GL_IT(it)->h,
                                 cvx, cvy, cvw, cvh)))
          {
             if (!it->realized) _item_realize(it, EINA_FALSE);
             if (sd->reorder.it)
                GL_IT(it)->scrl_y += _reorder_space_get(it);
#ifdef GENLIST_FX_SUPPORT
             if (sd->del_fx.anim && !eina_list_data_find(sd->del_fx.items, it))
                it->item->scrl_y -= _del_fx_space_get(it);
             if (sd->add_fx.anim && !eina_list_data_find(sd->add_fx.items, it))
                it->item->scrl_y -= _add_fx_space_get(it);
#endif

             _item_all_position(it, GL_IT(it)->scrl_x, GL_IT(it)->scrl_y);
          }
        else if (it->realized)
          {
             /* FIXME: when content is on focused and unrealize item,
                focus is set to other object and scroll will be bring in */
             if (_elm_config->access_mode)
               {
                  EINA_LIST_FOREACH(it->contents, ll, content)
                    {
                       if (elm_object_focus_get(content))
                         {
                            unrealize = EINA_FALSE;
                            break;
                         }
                    }
               }
             if (unrealize)
               _item_unrealize(it, EINA_FALSE);
             unrealize = EINA_TRUE;
          }
     }

   itb->realized = EINA_TRUE;

   if (!TIZEN_PROFILE_WEARABLE)
     {
        if (sd->banded_bg_on)
          _banded_item_bg_index_color_set(sd->top_drawn_item, ox, oy, ow, oh);
     }

   if (aligned_item)
     {
        edje_object_signal_emit(VIEW(aligned_item), SIGNAL_ITEM_HIGHLIGHTED, "elm");
        _focus_bg_show(aligned_item);

        edje_object_message_signal_process(VIEW(sd->aligned_item));
     }
}

// FIXME: Remove below macro after opensource is patched
#define EINA_INLIST_REVERSE_FOREACH_FROM(list, it)                                \
   for (it = NULL, it = (list ? _EINA_INLIST_CONTAINER(it, list) : NULL); \
        it; it = (EINA_INLIST_GET(it)->prev ? _EINA_INLIST_CONTAINER(it, EINA_INLIST_GET(it)->prev) : NULL))

EOLIAN static void
_elm_genlist_pan_efl_canvas_group_group_calculate(Eo *obj, Elm_Genlist_Pan_Data *psd)
{
   Evas_Coord ox, oy, ow, oh, cvx, cvy, cvw, cvh;
   Item_Block *itb;
   ELM_WIDGET_DATA_GET_OR_RETURN(psd->wsd->obj, wd);

   evas_object_geometry_get(obj, &ox, &oy, &ow, &oh);
   evas_output_viewport_get(evas_object_evas_get(obj), &cvx, &cvy, &cvw, &cvh);

   //FIXME: This is for optimizing genlist calculation after
   //       genlist sizing eval properly.
   if (ow <= 1) return;

   _calc((void *)psd->wsd);

   if (_elm_config->access_mode || _elm_atspi_enabled())
     {
        // If access is on, realize more items (3 times)
        cvy = cvy - cvh;
        cvh = cvh * 3;
     }
#ifdef GENLIST_FX_SUPPORT
   else if (psd->wsd->del_fx.anim)
     {
        Eina_List *l;
        Elm_Gen_Item *it;
        int size = 0;
        EINA_LIST_FOREACH(psd->wsd->del_fx.items, l, it)
          {
             size += it->item->minh;
          }
        cvh += size;
     }
   else if (psd->wsd->add_fx.anim)
     {
        Eina_List *l;
        Elm_Gen_Item *it;
        int size = 0;
        EINA_LIST_FOREACH(psd->wsd->add_fx.items, l, it)
          {
             size += it->item->minh;
          }
        cvh += size;
     }
#endif

   // Belows are tweaks for performance
   // Block is not needed anymore because of below algorithm..
   // later, we should remove dirty block algorithm
   Eina_Inlist *start = NULL;
   Eina_List *realized_new = NULL;
   Eina_Bool flag = EINA_FALSE;
   if ((psd->wsd->blocks_realized) && (psd->wsd->dir == -1) &&
       (!_elm_config->access_mode) && (!_elm_atspi_enabled()))
     {
        start = EINA_INLIST_GET((Item_Block *)eina_list_data_get
                               (eina_list_last(psd->wsd->blocks_realized)));
        EINA_INLIST_REVERSE_FOREACH_FROM(start, itb)
          {
             Evas_Coord itb_x, itb_y, itb_w, itb_h;
             itb_x = itb->x - psd->wsd->pan_x + ox;
             itb_y = itb->y - psd->wsd->pan_y + oy;
             itb_w = psd->wsd->minw;
             itb_h = itb->minh;
             if (psd->wsd->realization_mode ||
                 ELM_RECTS_INTERSECT(itb_x, itb_y, itb_w, itb_h,
                                     cvx, cvy, cvw, cvh))
               {
                  flag = EINA_TRUE;
                  realized_new = eina_list_prepend(realized_new, itb);
                  _item_block_realize(itb, EINA_FALSE);
               }
             else
               {
                  _item_block_unrealize(itb);
                  if (flag) break;
               }
          }
     }
   else if ((psd->wsd->blocks_realized) && (psd->wsd->dir == 1) &&
            (!_elm_config->access_mode) && (!_elm_atspi_enabled()))
     {
        start = EINA_INLIST_GET((Item_Block *)eina_list_data_get
                                (psd->wsd->blocks_realized));
        EINA_INLIST_FOREACH(start, itb)
          {
             Evas_Coord itb_x, itb_y, itb_w, itb_h;
             itb_x = itb->x - psd->wsd->pan_x + ox;
             itb_y = itb->y - psd->wsd->pan_y + oy;
             itb_w = psd->wsd->minw;
             itb_h = itb->minh;
             if (psd->wsd->realization_mode ||
                 ELM_RECTS_INTERSECT(itb_x, itb_y, itb_w, itb_h,
                                     cvx, cvy, cvw, cvh))
               {
                  flag = EINA_TRUE;
                  realized_new = eina_list_append(realized_new, itb);
                  _item_block_realize(itb, EINA_FALSE);
               }
             else
               {
                  _item_block_unrealize(itb);
                  if (flag) break;
               }
          }
     }
   else
     {
        if (!psd->wsd->show_item)
          {
              EINA_INLIST_FOREACH(psd->wsd->blocks, itb)
                {
                   Evas_Coord itb_x, itb_y, itb_w, itb_h;
                   itb_x = itb->x - psd->wsd->pan_x + ox;
                   itb_y = itb->y - psd->wsd->pan_y + oy;
                   itb_w = psd->wsd->minw;
                   itb_h = itb->minh;
                   if (psd->wsd->realization_mode ||
                       ELM_RECTS_INTERSECT(itb_x, itb_y, itb_w, itb_h,
                                          cvx, cvy, cvw, cvh))
                     {
                        realized_new = eina_list_append(realized_new, itb);
                        _item_block_realize(itb, EINA_FALSE);
                     }
                   else
                     {
                        _item_block_unrealize(itb);
                     }
                }
          }
     }
   eina_list_free(psd->wsd->blocks_realized);
   psd->wsd->blocks_realized = realized_new;

// TIZEN_ONLY(20150701) : banded color background feature. enabled only un-scrollable
   if (!TIZEN_PROFILE_WEARABLE)
     {
       if (psd->wsd->banded_bg_rect && !psd->wsd->reorder.it)
         {
            Eina_Rect bg;
            Elm_Gen_Item *tmp = NULL, *prev = NULL;

            tmp = ELM_GEN_ITEM_FROM_INLIST(psd->wsd->items->last);

            while(tmp)
              {
                 if (tmp->hide || !tmp->realized )
                   tmp = ELM_GEN_ITEM_FROM_INLIST(EINA_INLIST_GET(tmp)->prev);
                 else
                   break;
              }
            if (tmp)
              prev = ELM_GEN_ITEM_FROM_INLIST(EINA_INLIST_GET(tmp)->prev);

            while(prev)
              {
                 if(prev->hide)
                   prev = ELM_GEN_ITEM_FROM_INLIST(EINA_INLIST_GET(prev)->prev);
                 else
                   break;
              }

            if (tmp)
              {
                 int index = 0;

                 bg.x = GL_IT(tmp)->scrl_x;
                 bg.y = GL_IT(tmp)->scrl_y + GL_IT(tmp)->h;
                 bg.w = GL_IT(tmp)->w;
                 bg.h = oy + oh - bg.y;

                 evas_object_stack_below(psd->wsd->banded_bg_rect, VIEW(tmp));
                 efl_gfx_entity_geometry_set(psd->wsd->banded_bg_rect, bg);

                 //GET COLOR OF LAST ITEM AND SET NEXT COLOR TO BANDED BG RECT
                 if (psd->wsd->banded_bg_on)
                   {
                      if (!prev)
                         index = GL_IT(tmp)->banded_color_index + 1;
                      else
                        {
                           int sign = GL_IT(tmp)->banded_color_index - GL_IT(prev)->banded_color_index;

                           if (!sign)
                             {
                                prev = ELM_GEN_ITEM_FROM_INLIST(EINA_INLIST_GET(prev)->prev);

                                while(prev)
                                  {
                                     if(prev->hide)
                                        prev = ELM_GEN_ITEM_FROM_INLIST(EINA_INLIST_GET(prev)->prev);
                                     else
                                        break;
                                  }

                                if (!prev)
                                  sign = 1;
                                else
                                  sign = GL_IT(tmp)->banded_color_index - GL_IT(prev)->banded_color_index;
                             }

                           index = GL_IT(tmp)->banded_color_index + sign;

                           if (index < 0) index += 2;
                           else if (index >= BANDED_MAX_ITEMS)
                             index = BANDED_MAX_ITEMS - 2;
                        }

                      int alpha = 255 * (1 - 0.04 * index);
                      int color = (250 * alpha) / 255;

                      evas_object_color_set(psd->wsd->banded_bg_rect, color, color, color, alpha);
                   }
                 else
                   {
                      evas_object_color_set
                         (psd->wsd->banded_bg_rect, 0, 0, 0, 0);
                   }

                 evas_object_show(psd->wsd->banded_bg_rect);
              }
         }
     }
// TIZEN_ONLY

   if (psd->wsd->comp_y)
     {
        psd->wsd->unhighlight_skip = EINA_TRUE;
        elm_interface_scrollable_content_region_show((psd->wsd)->obj, psd->wsd->pan_x,
                                                                            psd->wsd->pan_y + psd->wsd->comp_y, ow, oh);
        psd->wsd->comp_y = 0;
        psd->wsd->unhighlight_skip = EINA_FALSE;
     }
   else if (psd->wsd->expanded_item &&
            GL_IT(psd->wsd->expanded_item)->calc_done)
     {
        Eina_List *l;
        Evas_Coord size = GL_IT(psd->wsd->expanded_item)->minh;
        Elm_Object_Item *eo_tmp;
        EINA_LIST_FOREACH(psd->wsd->expanded_item->item->items, l, eo_tmp)
          {
             ELM_GENLIST_ITEM_DATA_GET(eo_tmp, tmp);
             size += GL_IT(tmp)->minh;
          }
        if (size >= oh)
           elm_genlist_item_bring_in
             (EO_OBJ(psd->wsd->expanded_item),
              ELM_GENLIST_ITEM_SCROLLTO_TOP);
        else
          {
             eo_tmp = eina_list_data_get
                (eina_list_last(psd->wsd->expanded_item->item->items));

             if (eo_tmp)
               elm_genlist_item_bring_in(eo_tmp,
                                         ELM_GENLIST_ITEM_SCROLLTO_IN);
          }
        psd->wsd->expanded_item = NULL;
     }
   if (psd->wsd->show_item &&
       !psd->wsd->show_item->item->queued &&
       psd->wsd->show_item->item->calc_done &&
       psd->wsd->show_item->item->block->calc_done &&
       psd->wsd->calc_done)
     {
        Evas_Coord x, y, w, h;
        Elm_Gen_Item *it = psd->wsd->show_item;
        psd->wsd->show_item = NULL;
        x = it->x + GL_IT(it)->block->x + psd->wsd->pan_x;
        y = it->y + GL_IT(it)->block->y;
        w = ow;
        h = oh;

        switch (psd->wsd->scroll_to_type)
          {
           case ELM_GENLIST_ITEM_SCROLLTO_IN:
              h = GL_IT(it)->h;
              break;

           case ELM_GENLIST_ITEM_SCROLLTO_MIDDLE:
              y = y - (oh / 2) + (GL_IT(it)->h / 2);
              break;
// tizen_feature
           case ELM_GENLIST_ITEM_SCROLLTO_BOTTOM:
              y = y - oh + GL_IT(it)->h;
              break;
///
           default:
              break;
          }
        if (psd->wsd->bring_in)
          elm_interface_scrollable_region_bring_in(WIDGET(it), x, y, w, h);
        else
          {
             if (psd->wsd->aligned_item && (psd->wsd->aligned_item != psd->wsd->adjusted_item))
               edje_object_signal_emit(VIEW(psd->wsd->aligned_item), SIGNAL_ITEM_UNHIGHLIGHTED, "elm");
             edje_object_signal_emit(wd->resize_obj, SIGNAL_FOCUS_BG_HIDE, "elm");
             psd->wsd->aligned_item = NULL;
             psd->wsd->unhighlight_skip = EINA_TRUE;
             elm_interface_scrollable_content_region_show(WIDGET(it), x, y, ow, oh);
             psd->wsd->unhighlight_skip = EINA_FALSE;
          }

        _changed(psd->wsd->pan_obj);
     }
#ifdef GENLIST_FX_SUPPORT
   if (psd->wsd->add_fx.anim)
     {
        Eina_List *l;
        Elm_Gen_Item *it;

        int alpha = 255 * (1 - ((double)psd->wsd->add_fx.cnt/ANIM_CNT_MAX));
        EINA_LIST_FOREACH(psd->wsd->add_fx.items, l, it)
          {
             if (!it->realized) continue;
             if (it->deco_all_view)
                evas_object_color_set(it->deco_all_view, alpha, alpha, alpha, alpha);
             else if (it->item->deco_it_view)
                evas_object_color_set(it->item->deco_it_view, alpha, alpha, alpha, alpha);
             else if (VIEW(it))
                evas_object_color_set(VIEW(it), alpha, alpha, alpha, alpha);
          }
     }
   if (psd->wsd->del_fx.anim)
     {
        Eina_List *l;
        Elm_Gen_Item *it;

        int alpha = 255 * ((double)psd->wsd->del_fx.cnt/ANIM_CNT_MAX);
        EINA_LIST_FOREACH(psd->wsd->del_fx.items, l, it)
          {
             if (it->deco_all_view)
                evas_object_color_set(it->deco_all_view, alpha, alpha, alpha, alpha);
             else if (it->item->deco_it_view)
                evas_object_color_set(it->item->deco_it_view, alpha, alpha, alpha, alpha);
             else if (VIEW(it))
                evas_object_color_set(VIEW(it), alpha, alpha, alpha, alpha);
          }
     }
#endif
   efl_event_callback_legacy_call
         (psd->wobj, ELM_INTERFACE_SCROLLABLE_EVENT_CHANGED, NULL);
}

EOLIAN static void
_elm_genlist_pan_efl_object_destructor(Eo *obj, Elm_Genlist_Pan_Data *psd)
{
   efl_data_unref(psd->wobj, psd->wsd);
   efl_destructor(efl_super(obj, MY_PAN_CLASS));
}

EOLIAN static void
_elm_genlist_pan_class_constructor(Efl_Class *klass)
{
   evas_smart_legacy_type_register(MY_PAN_CLASS_NAME_LEGACY, klass);
}


//#ifdef ELM_FOCUSED_UI
static Eina_Bool
_item_multi_select_up(Elm_Genlist_Data *sd)
{
   Elm_Object_Item *prev;

   if (!sd->selected) return EINA_FALSE;
   if (!sd->multi) return EINA_FALSE;

   prev = elm_genlist_item_prev_get(sd->last_selected_item);
   if (!prev) return EINA_TRUE;

   if (elm_genlist_item_selected_get(prev))
     {
        elm_genlist_item_selected_set(sd->last_selected_item, EINA_FALSE);
        sd->last_selected_item = prev;
        elm_genlist_item_show
          (sd->last_selected_item, ELM_GENLIST_ITEM_SCROLLTO_IN);
     }
   else
     {
        elm_genlist_item_selected_set(prev, EINA_TRUE);
        elm_genlist_item_show(prev, ELM_GENLIST_ITEM_SCROLLTO_IN);
     }
   return EINA_TRUE;
}

static Eina_Bool
_item_multi_select_down(Elm_Genlist_Data *sd)
{
   Elm_Object_Item *next;

   if (!sd->selected) return EINA_FALSE;
   if (!sd->multi) return EINA_FALSE;

   next = elm_genlist_item_next_get(sd->last_selected_item);
   if (!next) return EINA_TRUE;

   if (elm_genlist_item_selected_get(next))
     {
        elm_genlist_item_selected_set(sd->last_selected_item, EINA_FALSE);
        sd->last_selected_item = next;
        elm_genlist_item_show
          (sd->last_selected_item, ELM_GENLIST_ITEM_SCROLLTO_IN);
     }
   else
     {
        elm_genlist_item_selected_set(next, EINA_TRUE);
        elm_genlist_item_show(next, ELM_GENLIST_ITEM_SCROLLTO_IN);
     }

   return EINA_TRUE;
}

static Eina_Bool
_all_items_deselect(Elm_Genlist_Data *sd)
{
   if (!sd->selected) return EINA_FALSE;

   while (sd->selected)
     elm_genlist_item_selected_set(sd->selected->data, EINA_FALSE);

   return EINA_TRUE;
}

static Eina_Bool
_item_single_select_up(Elm_Genlist_Data *sd)
{
   Elm_Object_Item *eo_prev;
   Elm_Gen_Item *prev;

   if (!sd->selected)
     {
        prev = ELM_GEN_ITEM_FROM_INLIST(sd->items->last);
        while (prev)
          prev = ELM_GEN_ITEM_FROM_INLIST(EINA_INLIST_GET(prev)->prev);
     }
   else
     {
        eo_prev = elm_genlist_item_prev_get(sd->last_selected_item);
        prev = efl_data_scope_get(eo_prev, ELM_GENLIST_ITEM_CLASS);
     }

   if (!prev) return EINA_FALSE;

   _all_items_deselect(sd);

   elm_genlist_item_selected_set(EO_OBJ(prev), EINA_TRUE);
   elm_genlist_item_show(EO_OBJ(prev), ELM_GENLIST_ITEM_SCROLLTO_IN);
   return EINA_TRUE;
}

static Eina_Bool
_item_single_select_down(Elm_Genlist_Data *sd)
{
   Elm_Object_Item *eo_next;
   Elm_Gen_Item *next;

   if (!sd->selected)
     {
        next = ELM_GEN_ITEM_FROM_INLIST(sd->items);
        while (next)
          next = ELM_GEN_ITEM_FROM_INLIST(EINA_INLIST_GET(next)->next);
     }
   else
     {
        eo_next = elm_genlist_item_next_get(sd->last_selected_item);
        next = efl_data_scope_get(eo_next, ELM_GENLIST_ITEM_CLASS);
     }

   if (!next) return EINA_FALSE;

   _all_items_deselect(sd);

   elm_genlist_item_selected_set(EO_OBJ(next), EINA_TRUE);
   elm_genlist_item_show
     (EO_OBJ(next), ELM_GENLIST_ITEM_SCROLLTO_IN);

   return EINA_TRUE;
}
//#endif

static void _item_unfocused(Elm_Gen_Item *it)
{
   if (!it) return;
   Elm_Genlist_Data *sd = GL_IT(it)->wsd;
   if (!sd->focused_item || EO_OBJ(it) != sd->focused_item) return;

   ELM_GENLIST_ITEM_DATA_GET(sd->focused_item, focus_it);

   edje_object_signal_emit
      (VIEW(focus_it), SIGNAL_UNFOCUSED, "elm");
   if (focus_it->deco_all_view)
      edje_object_signal_emit
         (focus_it->deco_all_view, SIGNAL_UNFOCUSED, "elm");
   sd->focused_item = NULL;
   efl_event_callback_legacy_call(WIDGET(it), ELM_GENLIST_EVENT_ITEM_UNFOCUSED, EO_OBJ(it));
}

static void _item_focused(Elm_Gen_Item *it, Elm_Genlist_Item_Scrollto_Type type)
{
   Evas_Coord x, y, w, h, sx, sy, sw, sh;
   if (!it) return;
   Elm_Genlist_Data *sd = GL_IT(it)->wsd;
   Evas_Object *win = elm_widget_top_get(sd->obj);

   evas_object_geometry_get(VIEW(it), &x, &y, &w, &h);
   evas_object_geometry_get(sd->obj, &sx, &sy, &sw, &sh);
   if ((x < sx) || (y < sy) || ((x + w) > (sx + sw)) || ((y + h) > (sy + sh)))
     {
        elm_genlist_item_bring_in(EO_OBJ(it), type);
     }

   if (elm_win_focus_highlight_enabled_get(win))
     {
        if (it->deco_all_view)
          edje_object_signal_emit
            (it->deco_all_view, SIGNAL_FOCUSED, "elm");
        else
          edje_object_signal_emit
            (VIEW(it), SIGNAL_FOCUSED, "elm");
     }

   sd->focused_item = EO_OBJ(it);
   efl_event_callback_legacy_call(WIDGET(it), ELM_GENLIST_EVENT_ITEM_FOCUSED, EO_OBJ(it));
   if (_elm_atspi_enabled())
     efl_access_active_descendant_changed_signal_emit(WIDGET(it), EO_OBJ(it));
}

static Eina_Bool
_banded_item_highlight_anim(void *data, double pos)
{
   Elm_Gen_Item *it = data;
   double frame = pos;
   double v[4] = {0.25, 0.46, 0.45, 1.00};

   if (GL_IT(it)->banded_bg)
     {
        if (pos < 1.0)
          frame = ecore_animator_pos_map_n(pos, ECORE_POS_MAP_CUBIC_BEZIER, 4, v);

        if (it->highlighted)
          frame = HIGHLIGHT_ALPHA_MAX + (1.0 - HIGHLIGHT_ALPHA_MAX) * (1.0 - frame);
        else
          frame = HIGHLIGHT_ALPHA_MAX + (1.0 - HIGHLIGHT_ALPHA_MAX) * frame;

        int alpha = 255 * frame * (1 - 0.04 * GL_IT(it)->banded_color_index);
        int color = 250;

        color = (color * alpha) / 255;
        evas_object_color_set(GL_IT(it)->banded_bg, color, color, color, alpha);
     }

   if (EINA_DBL_EQ(pos, 1)) it->item->banded_anim = NULL;

   return EINA_TRUE;
}

static void
_item_highlight(Elm_Gen_Item *it)
{
   Elm_Genlist_Data *sd = GL_IT(it)->wsd;
   ELM_WIDGET_DATA_GET_OR_RETURN(WIDGET(it), wsd);

   if (!sd->highlight) return;
   if (elm_wdg_item_disabled_get(EO_OBJ(it))) return;
   if ((sd->select_mode == ELM_OBJECT_SELECT_MODE_NONE) ||
       (sd->select_mode == ELM_OBJECT_SELECT_MODE_DISPLAY_ONLY ) ||
       (it->select_mode == ELM_OBJECT_SELECT_MODE_NONE) ||
       (it->select_mode == ELM_OBJECT_SELECT_MODE_DISPLAY_ONLY ))
     return;
   if (it->highlighted) return;
   if (wsd->scroll_item_align_enable)
     {
        if (sd->aligned_item == it)
          edje_object_signal_emit(wsd->resize_obj, SIGNAL_SELECTED, "elm");
        else
          return;
     }

   it->highlighted = EINA_TRUE;
   sd->highlighted_item = it;

   if (!it->realized) goto end;

   if (!TIZEN_PROFILE_WEARABLE)
     {
        if (sd->banded_bg_on)
          {
             if (it->item->banded_anim) ecore_animator_del(it->item->banded_anim);
             it->item->banded_anim = ecore_animator_timeline_add(ELM_ITEM_HIGHLIGHT_TIMER, _banded_item_highlight_anim, it);
          }
     }
   if (it->deco_all_view)
     {
        edje_object_signal_emit(it->deco_all_view, SIGNAL_SELECTED, "elm");
        edje_object_message_signal_process(it->deco_all_view);
     }
   edje_object_signal_emit(VIEW(it), SIGNAL_SELECTED, "elm");
   edje_object_message_signal_process(VIEW(it));

//***************** TIZEN Only
// If check or radio is used on genlist item, highlight it also.
   Eina_List *l;
   Evas_Object *content;
   if (sd->decorate_all_mode)
     {
        EINA_LIST_FOREACH(GL_IT(it)->deco_all_contents, l, content)
          {
             const char *type = elm_widget_type_get(content);
             if (type && (!strcmp(type, "elm_check") || (!strcmp(type, "elm_radio"))))
                elm_object_signal_emit(content, "elm,state,mouse,down", "elm");
          }
     }
   else
     {
        EINA_LIST_FOREACH(it->contents, l, content)
          {
             const char *type = elm_widget_type_get(content);
             if (type && (!strcmp(type, "elm_check") || (!strcmp(type, "elm_radio"))))
                elm_object_signal_emit(content, "elm,state,mouse,down", "elm");
          }
     }
//****************************
end:
   evas_object_smart_callback_call(WIDGET(it), SIG_HIGHLIGHTED, EO_OBJ(it));
   if (_elm_atspi_enabled())
     efl_access_active_descendant_changed_signal_emit(WIDGET(it), EO_OBJ(it));
}

static void
_item_unhighlight(Elm_Gen_Item *it, Eina_Bool effect EINA_UNUSED)
{
   ELM_WIDGET_DATA_GET_OR_RETURN(WIDGET(it), wsd);
   if (!it->highlighted) return;

   it->highlighted = EINA_FALSE;
   GL_IT(it)->wsd->highlighted_item = NULL;

   if (!it->realized) goto end;

   if (!TIZEN_PROFILE_WEARABLE)
     {
        if (GL_IT(it)->wsd->banded_bg_on && effect)
          {
             if (it->item->banded_anim) ecore_animator_del(it->item->banded_anim);
             it->item->banded_anim = ecore_animator_timeline_add(ELM_ITEM_HIGHLIGHT_TIMER, _banded_item_highlight_anim, it);
          }
        else
          {
             if (it->item->banded_anim) ELM_SAFE_FREE(it->item->banded_anim, ecore_animator_del);
             _banded_item_bg_color_change(it, EINA_FALSE);
          }
     }
   if (it->deco_all_view)
     edje_object_signal_emit(it->deco_all_view, SIGNAL_UNSELECTED, "elm");
   edje_object_signal_emit(VIEW(it), SIGNAL_UNSELECTED, "elm");
   if (wsd->scroll_item_align_enable)
     edje_object_signal_emit(wsd->resize_obj, SIGNAL_UNSELECTED, "elm");
   else
     edje_object_signal_emit(VIEW(it), SIGNAL_UNSELECTED, "elm");
//******************** TIZEN Only
// If check is used on genlist item, unhighlight it also.
   Eina_List *l;
   Evas_Object *content;
   if (GL_IT(it)->wsd->decorate_all_mode)
     {
        EINA_LIST_FOREACH(GL_IT(it)->deco_all_contents, l, content)
          {
             const char *type = elm_widget_type_get(content);
             if (type && (!strcmp(type, "elm_check") || (!strcmp(type, "elm_radio"))))
                elm_object_signal_emit(content, "elm,state,mouse,up", "elm");
          }
     }
   else
     {
        EINA_LIST_FOREACH(it->contents, l, content)
          {
             const char *type = elm_widget_type_get(content);
             if (type && (!strcmp(type, "elm_check") || (!strcmp(type, "elm_radio"))))
                elm_object_signal_emit(content, "elm,state,mouse,up", "elm");
          }
     }
   //*******************************
end:
   efl_event_callback_legacy_call(WIDGET(it), ELM_GENLIST_EVENT_UNHIGHLIGHTED, EO_OBJ(it));
}

static void
_item_unselect(Elm_Gen_Item *it)
{
   Elm_Genlist_Data *sd = GL_IT(it)->wsd;

   if (!it->selected) return;

   it->selected = EINA_FALSE;
   sd->selected = eina_list_remove(sd->selected, EO_OBJ(it));
   _item_unhighlight(it, EINA_TRUE);
   evas_object_smart_callback_call(WIDGET(it), "unselected", EO_OBJ(it));
}

static Eina_Bool
_item_select(Elm_Gen_Item *it)
{
   Evas_Object *obj = WIDGET(it);
   Elm_Genlist_Data *sd = GL_IT(it)->wsd;

   if (elm_wdg_item_disabled_get(EO_OBJ(it))) return EINA_FALSE;
   if (_is_no_select(it) || (it->decorate_it_set)) return EINA_FALSE;
   if (sd->select_mode != ELM_OBJECT_SELECT_MODE_ALWAYS &&
       it->select_mode != ELM_OBJECT_SELECT_MODE_ALWAYS &&
       it->selected)
      return EINA_FALSE;
   if (!sd->multi)
     {
        const Eina_List *l, *ll;
        Elm_Object_Item *eo_it2;
        EINA_LIST_FOREACH_SAFE(sd->selected, l, ll, eo_it2)
          if (eo_it2 != EO_OBJ(it))
            {
               ELM_GENLIST_ITEM_DATA_GET(eo_it2, it2);
               _item_unselect(it2);
            }
     }

   if (!it->selected)
     {
        sd->selected = eina_list_append(sd->selected, EO_OBJ(it));
        it->selected = EINA_TRUE;
     }
   sd->last_selected_item = EO_OBJ(it);
   _item_highlight(it);

   if (sd->focused_content)
     {
        elm_object_focus_set(sd->focused_content, EINA_FALSE);
        evas_object_focus_set(obj, EINA_TRUE);
        sd->focused_content = NULL;
     }

   sd->focus_scrollto_type = ELM_GENLIST_ITEM_SCROLLTO_IN;
   elm_object_item_focus_set(EO_OBJ(it), EINA_TRUE);
  /* FIXME: Focus Manager Feature Need to Be Implemented
   if (!(sd->focus_on_selection_enabled || _elm_config->item_select_on_focus_disable))
     {
        efl_ui_focus_manager_focus_set(obj, it->base->eo_obj);
     }
   */
   if (it->realized)
     {
        // FIXME: after evas_object_raise, mouse event callbacks(ex, UP, DOWN)
        // can be called again eventhough already received it.
        const char *selectraise = edje_object_data_get(VIEW(it), "selectraise");
        if ((selectraise) && (!strcmp(selectraise, "on")))
          {
             if (it->deco_all_view) evas_object_raise(it->deco_all_view);
             else evas_object_raise(VIEW(it));
             if ((GL_IT(it)->group_item) && (GL_IT(it)->group_item->realized))
               evas_object_raise(GL_IT(it)->VIEW(group_item));
          }

        edje_object_signal_emit
           (VIEW(it), SIGNAL_CLICKED, "elm");
        edje_object_message_signal_process(VIEW(it));
     }
   evas_object_ref(obj);
   if (it->func.func) it->func.func((void *)it->func.data, obj, EO_OBJ(it));
   if (EINA_MAGIC_CHECK(it->base, ELM_WIDGET_ITEM_MAGIC))
     evas_object_smart_callback_call(WIDGET(it), "selected", EO_OBJ(it));
   if ((it->base)->on_deletion) goto item_deleted;

   if (!(sd->focus_on_selection_enabled || _elm_config->item_select_on_focus_disable))
     {
        efl_ui_focus_manager_focus_set(obj, it->base->eo_obj);
     }

   evas_object_unref(obj);
   return EINA_FALSE;

item_deleted:
   evas_object_unref(obj);
   return EINA_TRUE;
}

static Eina_Bool
_highlight_timer(void *data)
{
   Elm_Gen_Item *it = data;
   GL_IT(it)->highlight_timer = NULL;
   _item_highlight(it);
   return EINA_FALSE;
}

static Eina_Bool
_select_timer(void *data)
{
   Elm_Gen_Item *it = data;
   GL_IT(it)->highlight_timer = NULL;

   if ((GL_IT(it)->wsd->select_mode == ELM_OBJECT_SELECT_MODE_ALWAYS) ||
       (it->select_mode == ELM_OBJECT_SELECT_MODE_ALWAYS))
      _item_select(it);
   else
     {
        // TIZEN Only(20150708) : unselect when selected item is selected again
        // There need to be implemented 'SELECT_UNSELECT' mode in elm_config
        // select mode for support upsteam and TIZEN both.
        if (!it->selected) _item_select(it);
        else _item_unselect(it);
        //
     }

   return EINA_FALSE;
}

static Evas_Object
*_item_focusable_content_search(Evas_Object *obj, Eina_List *l, int dir)
{
   if ((dir != 1) && (dir != -1)) return NULL;

   Evas_Object *next = NULL;
   Elm_Object_Item *next_item = NULL;

/* FIXME : Need to Implement focus logic based on focus manager.
   while (l)
     {
        next = eina_list_data_get(l);
        if (next && (elm_widget_can_focus_get(next) &&
                   (evas_object_visible_get(next))))
          break;
        else if (next && (elm_widget_child_can_focus_get(next)))
          {
             if (elm_widget_focused_object_get(next))
               break;
             else
               {
                  Evas_Object *child = next;
                  if ((dir == 1) &&
                      (elm_widget_focus_next_get(next, ELM_FOCUS_RIGHT, &child, &next_item)))
                    break;
                  else if ((dir == -1) &&
                           (elm_widget_focus_next_get(next, ELM_FOCUS_LEFT, &child, &next_item)))
                    break;
               }
          }

        next = NULL;
        if (dir == 1) l = eina_list_next(l);
        else l = eina_list_prev(l);
     }
*/
     if (!next) next = obj;

   return next;
}

static Eina_Bool _item_focusable_search(Elm_Gen_Item **it, int dir)
{
   if (!(*it)) return EINA_FALSE;
   if ((dir != 1) && (dir != -1)) return EINA_FALSE;

   Elm_Gen_Item *tmp = *it;

   while (tmp)
     {
        if (!elm_object_item_disabled_get(EO_OBJ(tmp)) &&
           (!tmp->hide))
          {
             if ((tmp->select_mode == ELM_OBJECT_SELECT_MODE_DISPLAY_ONLY) ||
                 (tmp->select_mode == ELM_OBJECT_SELECT_MODE_NONE) ||
                 (tmp->flipped))
               {
                  Evas_Object *obj = NULL;
                  Eina_List *contents = NULL;
                  if (tmp->flipped)
                    contents = GL_IT(tmp)->flip_content_objs;
                  else
                    {
                       contents = tmp->contents;
                       if (tmp->deco_all_view)
                         {
                            if (contents)
                              contents = GL_IT(tmp)->deco_all_contents;
                            else
                              contents = eina_list_merge(contents,
                                            GL_IT(tmp)->deco_all_contents);
                         }
                    }

                  if (dir == 1)
                    obj = _item_focusable_content_search(NULL, contents, 1);
                  else
                    {
                       contents = eina_list_last(contents);
                       obj = _item_focusable_content_search(NULL, contents, -1);
                    }

                  if (obj)
                    {
                       if (GL_IT(tmp)->wsd->focused_content)
                         elm_object_focus_set(GL_IT(tmp)->wsd->focused_content, EINA_FALSE);
                       GL_IT(tmp)->wsd->focused_content = obj;
                       elm_object_focus_set(obj, EINA_TRUE);
                       *it = tmp;
                       return EINA_TRUE;
                    }
               }
             if (GL_IT(tmp)->wsd->focused_content)
               {
                  elm_object_focus_set(GL_IT(tmp)->wsd->focused_content, EINA_FALSE);
                  evas_object_focus_set(WIDGET(tmp), EINA_TRUE);
                  GL_IT(tmp)->wsd->focused_content = NULL;
               }
             *it = tmp;
             return EINA_TRUE;
          }
        if (dir == 1)
          tmp = ELM_GEN_ITEM_FROM_INLIST(EINA_INLIST_GET(tmp)->next);
        else
          tmp = ELM_GEN_ITEM_FROM_INLIST(EINA_INLIST_GET(tmp)->prev);
     }

   *it = NULL;
   return EINA_FALSE;
}

static Eina_Bool _item_focus_next(Elm_Genlist_Data *sd, Focus_Dir dir)
{
   Elm_Gen_Item *it = NULL;
   ELM_GENLIST_ITEM_DATA_GET(sd->focused_item, old_focused);
   Evas_Object *old_content = sd->focused_content;

   if (dir == FOCUS_DIR_DOWN || dir == FOCUS_DIR_UP)
     {
        Eina_Bool find_item;

        if (dir == FOCUS_DIR_DOWN)
          {
             if (sd->focused_item)
               {
                  it = ELM_GEN_ITEM_FROM_INLIST
                     (EINA_INLIST_GET(old_focused)->next);
                  elm_object_item_focus_set(sd->focused_item, EINA_FALSE);
               }
             else it = ELM_GEN_ITEM_FROM_INLIST(sd->items);
             find_item = _item_focusable_search(&it, 1);
          }
        else if (dir == FOCUS_DIR_UP)
          {
             if (sd->focused_item)
               {
                  it = ELM_GEN_ITEM_FROM_INLIST
                     (EINA_INLIST_GET(old_focused)->prev);
                  elm_object_item_focus_set(sd->focused_item, EINA_FALSE);
               }
             else it = ELM_GEN_ITEM_FROM_INLIST(sd->items->last);
             find_item = _item_focusable_search(&it, -1);
          }

        if (!it)
          {
             if (old_focused)
               {
                  if (old_content)
                    {
                       sd->focused_content = old_content;
                       elm_object_focus_set(old_content, EINA_TRUE);
                    }
                  else
                    {
                       sd->focus_scrollto_type = ELM_GENLIST_ITEM_SCROLLTO_IN;
                       elm_object_item_focus_set(EO_OBJ(old_focused), EINA_TRUE);
                    }
               }
             return EINA_FALSE;
          }
        else if (!find_item)
          return EINA_TRUE;

        sd->focus_scrollto_type = ELM_GENLIST_ITEM_SCROLLTO_IN;
        elm_object_item_focus_set(EO_OBJ(it), EINA_TRUE);
     }
   else if (dir == FOCUS_DIR_LEFT || dir == FOCUS_DIR_RIGHT)
     {
        Evas_Object *obj = NULL;
        Eina_List *contents = NULL;
        Eina_List *l = NULL;

        if (!sd->focused_item) return EINA_FALSE;
        if (old_focused->flipped)
          contents = GL_IT(old_focused)->flip_content_objs;
        else
          {
             contents = old_focused->contents;
             if (old_focused->deco_all_view)
               {
                  if (contents)
                    contents = GL_IT(old_focused)->deco_all_contents;
                  else
                    contents = eina_list_merge(contents,
                                  GL_IT(old_focused)->deco_all_contents);
               }
          }

        if (sd->focused_content)
          {
             l = eina_list_data_find_list(contents,
                                          sd->focused_content);
             obj = sd->focused_content;
          }

        if (dir == FOCUS_DIR_LEFT)
          {
             if ((l) && (eina_list_prev(l)))
               {
                  l = eina_list_prev(l);
                  obj = _item_focusable_content_search(obj, l, -1);
                  if (!obj) obj = sd->focused_content;
               }
             else if (!l)
               {
                  //search focused content is child of content
                  if (sd->focused_content)
                    l = eina_list_data_find_list(contents,
                                                 sd->focused_content);
                  if (!l) l = eina_list_last(contents);
                  obj = _item_focusable_content_search(obj, l, -1);
               }
             else obj = sd->focused_content;
          }
        else if (dir == FOCUS_DIR_RIGHT)
          {
             if ((l) && (eina_list_next(l)))
               {
                  l = eina_list_next(l);
                  obj = _item_focusable_content_search(obj, l, 1);
                  if (!obj) obj = sd->focused_content;
               }
             else if (!l)
               {
                  //search focused content is child of content
                  if (sd->focused_content)
                    l = eina_list_data_find_list(contents,
                                                 sd->focused_content);
                  if (!l) l = contents;
                  obj = _item_focusable_content_search(obj, l, 1);
               }
             else obj = sd->focused_content;
          }

        if (obj)
          {
             sd->focused_content = obj;
             elm_object_focus_set(obj, EINA_TRUE);
          }
        else
          {
             sd->focused_content = NULL;
             return EINA_FALSE;
          }
     }
   else return EINA_FALSE;

   return EINA_TRUE;
}
/*FIXME: Need to be implemented on New Widget Event Interface
//#ifdef ELM_FOCUSED_UI
EOLIAN static Eina_Bool
_elm_genlist_efl_ui_widget_widget_event(Eo *obj, Elm_Genlist_Data *sd, Evas_Object *src EINA_UNUSED, Evas_Callback_Type type, void *event_info)
{
   Evas_Coord x = 0;
   Evas_Coord y = 0;
   Evas_Coord v_w = 0;
   Evas_Coord v_h = 0;
   Evas_Coord step_x = 0;
   Evas_Coord step_y = 0;
   Evas_Coord page_x = 0;
   Evas_Coord page_y = 0;
   Evas_Event_Key_Down *ev = event_info;
   Evas_Coord pan_max_x = 0, pan_max_y = 0;

   //ELM_GENLIST_DATA_GET(obj, sd);

   // TIZEN ONLY(20131221) : When access mode, focused ui is disabled.
   if (_elm_config->access_mode) return EINA_FALSE;

   if ((type != EVAS_CALLBACK_KEY_DOWN) && (type != EVAS_CALLBACK_KEY_UP))
     return EINA_FALSE;
   if (!sd->items) return EINA_FALSE;
   if (ev->event_flags & EVAS_EVENT_FLAG_ON_HOLD) return EINA_FALSE;
   if (elm_widget_disabled_get(obj)) return EINA_FALSE;

   elm_interface_scrollable_content_pos_get(obj, &x, &y);
   elm_interface_scrollable_step_size_get(obj, &step_x, &step_y);
   elm_interface_scrollable_page_size_get(obj, &page_x, &page_y);
   elm_interface_scrollable_content_viewport_geometry_get
         (obj, NULL, NULL, &v_w, &v_h);

   if ((!strcmp(ev->keyname, "Left")) ||
       ((!strcmp(ev->keyname, "KP_Left")) && (!ev->string)) ||
       (!strcmp(ev->keyname, "Right")) ||
       ((!strcmp(ev->keyname, "KP_Right")) && (!ev->string)) ||
       (!strcmp(ev->keyname, "Up")) ||
       ((!strcmp(ev->keyname, "KP_Up")) && (!ev->string)) ||
       (!strcmp(ev->keyname, "Down")) ||
       ((!strcmp(ev->keyname, "KP_Down")) && (!ev->string)) ||
       (!strcmp(ev->keyname, "Home")) ||
       ((!strcmp(ev->keyname, "KP_Home")) && (!ev->string)) ||
       (!strcmp(ev->keyname, "End")) ||
       ((!strcmp(ev->keyname, "KP_End")) && (!ev->string)) ||
       (!strcmp(ev->keyname, "Prior")) ||
       ((!strcmp(ev->keyname, "KP_Prior")) && (!ev->string)) ||
       (!strcmp(ev->keyname, "Next")) ||
       ((!strcmp(ev->keyname, "KP_Next")) && (!ev->string)))
     {
        if (type != EVAS_CALLBACK_KEY_DOWN) return EINA_FALSE;
        _elm_widget_focus_auto_show(obj);
     }

   if ((!strcmp(ev->keyname, "Left")) ||
       ((!strcmp(ev->keyname, "KP_Left")) && (!ev->string)))
     {
        if (type != EVAS_CALLBACK_KEY_DOWN) return EINA_FALSE;
        if (sd->select_on_focus_enabled) x -= step_x;
        else
          {
             if (_item_focus_next(sd, FOCUS_DIR_LEFT))
               {
                  ev->event_flags |= EVAS_EVENT_FLAG_ON_HOLD;
                  return EINA_TRUE;
               }
             else
               return EINA_FALSE;
          }
     }
   else if ((!strcmp(ev->keyname, "Right")) ||
            ((!strcmp(ev->keyname, "KP_Right")) && (!ev->string)))
     {
        if (type != EVAS_CALLBACK_KEY_DOWN) return EINA_FALSE;
        if (sd->select_on_focus_enabled) x += step_x;
        else
          {
             if (_item_focus_next(sd, FOCUS_DIR_RIGHT))
               {
                  ev->event_flags |= EVAS_EVENT_FLAG_ON_HOLD;
                  return EINA_TRUE;
               }
             else
               return EINA_FALSE;
          }
     }
   else if ((!strcmp(ev->keyname, "Up")) ||
            ((!strcmp(ev->keyname, "KP_Up")) && (!ev->string)))
     {
        if (type != EVAS_CALLBACK_KEY_DOWN) return EINA_FALSE;
        if (sd->select_on_focus_enabled)
          {
             if (((evas_key_modifier_is_set(ev->modifiers, "Shift")) &&
                  (_item_multi_select_up(sd))) || (_item_single_select_up(sd)))
               {
                  ev->event_flags |= EVAS_EVENT_FLAG_ON_HOLD;
                  return EINA_TRUE;
               }
          }
        else
          {
             if (_item_focus_next(sd, FOCUS_DIR_UP))
               {
                  ev->event_flags |= EVAS_EVENT_FLAG_ON_HOLD;
                  return EINA_TRUE;
               }
             else
               return EINA_FALSE;
          }
     }
   else if ((!strcmp(ev->keyname, "Down")) ||
            ((!strcmp(ev->keyname, "KP_Down")) && (!ev->string)))
     {
        if (type != EVAS_CALLBACK_KEY_DOWN) return EINA_FALSE;
        if (sd->select_on_focus_enabled)
          {
             if (((evas_key_modifier_is_set(ev->modifiers, "Shift")) &&
                  (_item_multi_select_down(sd))) ||
                 (_item_single_select_down(sd)))
               {
                  ev->event_flags |= EVAS_EVENT_FLAG_ON_HOLD;
                  return EINA_TRUE;
               }
          }
        else
          {
             if (_item_focus_next(sd, FOCUS_DIR_DOWN))
               {
                  ev->event_flags |= EVAS_EVENT_FLAG_ON_HOLD;
                  return EINA_TRUE;
               }
             else
               return EINA_FALSE;
          }
     }
   else if ((!strcmp(ev->keyname, "Home")) ||
            ((!strcmp(ev->keyname, "KP_Home")) && (!ev->string)))
     {
        if (type != EVAS_CALLBACK_KEY_DOWN) return EINA_FALSE;
        if (sd->select_on_focus_enabled)
          {
             Elm_Object_Item *it = elm_genlist_first_item_get(obj);
             elm_genlist_item_bring_in(it, ELM_GENLIST_ITEM_SCROLLTO_IN);
             elm_genlist_item_selected_set(it, EINA_TRUE);
             ev->event_flags |= EVAS_EVENT_FLAG_ON_HOLD;
          }
        else
          {
             elm_object_item_focus_set((sd->focused_item, EINA_FALSE);
             _item_focus_next(sd, FOCUS_DIR_DOWN);
             ev->event_flags |= EVAS_EVENT_FLAG_ON_HOLD;
          }
        return EINA_TRUE;
     }
   else if ((!strcmp(ev->keyname, "End")) ||
            ((!strcmp(ev->keyname, "KP_End")) && (!ev->string)))
     {
        if (type != EVAS_CALLBACK_KEY_DOWN) return EINA_FALSE;
        if (sd->select_on_focus_enabled)
          {
             Elm_Object_Item *it = elm_genlist_last_item_get(obj);
             elm_genlist_item_bring_in(it, ELM_GENLIST_ITEM_SCROLLTO_IN);
             elm_genlist_item_selected_set(it, EINA_TRUE);
          }
        else
          {
             elm_object_item_focus_set(sd->focused_item, EINA_FALSE);
             sd->focused_item = EO_OBJ(ELM_GEN_ITEM_FROM_INLIST(sd->items->last));
             _item_focus_next(sd, FOCUS_DIR_UP);
          }
        ev->event_flags |= EVAS_EVENT_FLAG_ON_HOLD;
        return EINA_TRUE;
     }
   else if ((!strcmp(ev->keyname, "Prior")) ||
            ((!strcmp(ev->keyname, "KP_Prior")) && (!ev->string)))
     {
        if (type != EVAS_CALLBACK_KEY_DOWN) return EINA_FALSE;
        ELM_GENLIST_ITEM_DATA_GET(sd->focused_item, it);
        Elm_Object_Item *eo_prev;

        while (page_y <= v_h)
          {
             if (elm_genlist_item_prev_get(EO_OBJ(it)))
               {
                  eo_prev = elm_genlist_item_prev_get(EO_OBJ(it));
                  it = efl_data_scope_get(eo_prev, ELM_GENLIST_ITEM_CLASS);
               }
             else break;
             page_y += GL_IT(it)->minh;
          }
        if (_item_focusable_search(&it, -1))
          {
             sd->focus_scrollto_type = ELM_GENLIST_ITEM_SCROLLTO_TOP;
             elm_object_item_focus_set(EO_OBJ(it), EINA_TRUE);
          }
        else if (!it)
          {
             _item_focusable_search(&it, 1);
             sd->focus_scrollto_type = ELM_GENLIST_ITEM_SCROLLTO_TOP;
             elm_object_item_focus_set(EO_OBJ(it), EINA_TRUE);
          }

        ev->event_flags |= EVAS_EVENT_FLAG_ON_HOLD;
        return EINA_TRUE;
     }
   else if ((!strcmp(ev->keyname, "Next")) ||
            ((!strcmp(ev->keyname, "KP_Next")) && (!ev->string)))
     {
        if (type != EVAS_CALLBACK_KEY_DOWN) return EINA_FALSE;
        ELM_GENLIST_ITEM_DATA_GET(sd->focused_item, it);
        Elm_Object_Item *eo_next;

        page_y = GL_IT(it)->minh;

        while (page_y <= v_h)
          {
             if (elm_genlist_item_next_get(EO_OBJ(it)))
               {
                  eo_next = elm_genlist_item_next_get(EO_OBJ(it));
                  it = efl_data_scope_get(eo_next, ELM_GENLIST_ITEM_CLASS);
               }
             else break;
             page_y += GL_IT(it)->minh;
          }
        if (_item_focusable_search(&it, 1))
          {
             sd->focus_scrollto_type = ELM_GENLIST_ITEM_SCROLLTO_TOP;
             elm_object_item_focus_set(EO_OBJ(it), EINA_TRUE);
          }
        else if(!it)
          {
             _item_focusable_search(&it, -1);
             sd->focus_scrollto_type = ELM_GENLIST_ITEM_SCROLLTO_TOP;
             elm_object_item_focus_set(EO_OBJ(it), EINA_TRUE);
          }

        sd->focus_scrollto_type = ELM_GENLIST_ITEM_SCROLLTO_TOP;
        elm_object_item_focus_set(EO_OBJ(it), EINA_TRUE);

        ev->event_flags |= EVAS_EVENT_FLAG_ON_HOLD;
        return EINA_TRUE;
     }
   else if (!strcmp(ev->keyname, "Escape"))
     {
        if (type != EVAS_CALLBACK_KEY_DOWN) return EINA_FALSE;
        if (!_all_items_deselect(sd)) return EINA_FALSE;
        ev->event_flags |= EVAS_EVENT_FLAG_ON_HOLD;
        return EINA_TRUE;
     }
   else if (!strcmp(ev->keyname, "Return") ||
            !strcmp(ev->keyname, "KP_Enter") ||
            !strcmp(ev->keyname, "space"))
     {

        if (type == EVAS_CALLBACK_KEY_DOWN && !sd->key_down_item)
          {
             if (sd->focused_item)
               {
                  ELM_GENLIST_ITEM_DATA_GET(sd->focused_item, it);
                  sd->key_down_item = it;

                  edje_object_signal_emit
                     (VIEW(it), SIGNAL_UNFOCUSED, "elm");
                  if (it->deco_all_view)
                    edje_object_signal_emit
                       (it->deco_all_view, SIGNAL_UNFOCUSED, "elm");

                  _item_highlight(sd->key_down_item);
                  if (sd->key_down_item->long_timer)
                    ecore_timer_del(sd->key_down_item->long_timer);
                  sd->key_down_item->long_timer = ecore_timer_add
                                             (sd->longpress_timeout,
                                             _long_press_cb, sd->key_down_item);
                  ev->event_flags |= EVAS_EVENT_FLAG_ON_HOLD;
                  efl_event_callback_legacy_call(obj, ELM_GENLIST_EVENT_ACTIVATED, EO_OBJ(sd->key_down_item));

                  return EINA_TRUE;
               }
             else return EINA_FALSE;
          }
        else if (type == EVAS_CALLBACK_KEY_UP && sd->key_down_item)
          {
             edje_object_signal_emit
                (VIEW(sd->key_down_item), SIGNAL_FOCUSED, "elm");
             if (sd->key_down_item->deco_all_view)
                 edje_object_signal_emit
                    (sd->key_down_item->deco_all_view, SIGNAL_FOCUSED, "elm");

             if (sd->key_down_item->long_timer)
               ecore_timer_del(sd->key_down_item->long_timer);
             sd->key_down_item->long_timer = NULL;
             if (GL_IT(sd->key_down_item)->highlight_timer)
               ecore_timer_del(GL_IT(sd->key_down_item)->highlight_timer);
             GL_IT(sd->key_down_item)->highlight_timer = ecore_timer_add
                  (ITEM_SELECT_TIMER, _select_timer, sd->key_down_item);
             sd->key_down_item = NULL;
             ev->event_flags |= EVAS_EVENT_FLAG_ON_HOLD;
             return EINA_TRUE;
          }
        else return EINA_FALSE;
     }
   else return EINA_FALSE;

   ev->event_flags |= EVAS_EVENT_FLAG_ON_HOLD;
   elm_obj_pan_pos_max_get(sd->pan_obj, &pan_max_x, &pan_max_y);
   if (x < 0) x = 0;
   if (x > pan_max_x) x = pan_max_x;
   if (y < 0) y = 0;
   if (y > pan_max_y) y = pan_max_y;

   elm_interface_scrollable_content_pos_set(obj, x, y, EINA_TRUE);

   return EINA_TRUE;
}
//#endif
*/
static Eina_Bool
_item_focused_next(Evas_Object *obj, Elm_Focus_Direction dir)
{
   ELM_GENLIST_DATA_GET(obj, sd);
   Elm_Gen_Item *next;
   Elm_Object_Item *eo_next;
   Elm_Object_Item *eo_first_item;
   Elm_Object_Item *eo_last_item;

   if (!sd->focused_item)
     {
        if (dir == ELM_FOCUS_UP)
          next = ELM_GEN_ITEM_FROM_INLIST(sd->items->last);
        else if (dir == ELM_FOCUS_DOWN)
          next = ELM_GEN_ITEM_FROM_INLIST(sd->items);
        else
          return EINA_FALSE;

        while ((next) &&
               ((elm_wdg_item_disabled_get(EO_OBJ(next))) ||
               (_is_no_select(next))))
          if (!next->hide) next = ELM_GEN_ITEM_NEXT(next);
     }
   else
     {
        ELM_GENLIST_ITEM_DATA_GET(sd->focused_item, focus_it);
        if (dir == ELM_FOCUS_UP)
          {
             eo_first_item = elm_genlist_first_item_get(WIDGET(focus_it));
             if (eo_first_item == sd->focused_item) return EINA_FALSE;
             eo_next = elm_genlist_item_prev_get(sd->focused_item);
             next = efl_data_scope_get(eo_next, ELM_GENLIST_ITEM_CLASS);

             while ((next) &&
                    ((elm_wdg_item_disabled_get(eo_next)) ||
                    (_is_no_select(next))))
               {
                  eo_next = elm_genlist_item_prev_get(eo_next);
                  next = efl_data_scope_get(eo_next, ELM_GENLIST_ITEM_CLASS);
               }
          }
        else if (dir == ELM_FOCUS_DOWN)
          {
             eo_last_item = elm_genlist_last_item_get(WIDGET(focus_it));
             if (eo_last_item == sd->focused_item) return EINA_FALSE;
             eo_next = elm_genlist_item_next_get(sd->focused_item);
             next = efl_data_scope_get(eo_next, ELM_GENLIST_ITEM_CLASS);

             while ((next) &&
                    ((elm_wdg_item_disabled_get(eo_next)) ||
                    (_is_no_select(next))))
               {
                  eo_next = elm_genlist_item_next_get(eo_next);
                  next = efl_data_scope_get(eo_next, ELM_GENLIST_ITEM_CLASS);
               }
          }
        else
          return EINA_FALSE;

        if (!next) return EINA_FALSE;
     }

   elm_object_item_focus_set(EO_OBJ(next), EINA_TRUE);

   return EINA_TRUE;
}

static void
_elm_genlist_item_content_focus_set(Elm_Gen_Item *it, Elm_Focus_Direction dir)
{
   Evas_Object *focused_obj = NULL;
   Eina_List *l;
   if (!it) return;

   if (!it->item->wsd->focus_on_selection_enabled) return;

   if (!it->item_focus_chain)
     {
        elm_object_focus_set(VIEW(it), EINA_TRUE);
        return;
     }

   EINA_LIST_FOREACH(it->item_focus_chain, l, focused_obj)
     if (elm_object_focus_get(focused_obj)) break;

   /* FOCUS-FIXME
   if (focused_obj && (dir != ELM_FOCUS_PREVIOUS))
     {
        Evas_Object *nextfocus;
        Elm_Object_Item *nextfocus_item;
        if (efl_ui_widget_focus_next_get(focused_obj, dir, &nextfocus, &nextfocus_item))
          {
             if (nextfocus_item)
               elm_object_item_focus_set(nextfocus_item, EINA_TRUE);
             else
               elm_object_focus_set(nextfocus, EINA_TRUE);
             return;
          }
     }
   */

   if (!l) l = it->item_focus_chain;

   if (dir == ELM_FOCUS_RIGHT)
     {
        l = eina_list_next(l);
        if (!l) l = it->item_focus_chain;
     }
   else if (dir == ELM_FOCUS_LEFT)
     {
        l = eina_list_prev(l);
        if (!l) l = eina_list_last(it->item_focus_chain);
     }

   elm_object_focus_set(eina_list_data_get(l), EINA_TRUE);
}

static Eina_Bool
_key_action_move_dir(Evas_Object *obj, Elm_Focus_Direction dir, Eina_Bool multi)
{
   ELM_GENLIST_DATA_GET(obj, sd);
   Elm_Object_Item *it = NULL;
   Eina_Bool ret = EINA_FALSE;
   Evas_Coord v = 0;
   Evas_Coord min = 0;
   Eina_Bool focus_only = EINA_FALSE;

   // get content size and viewport size
   elm_interface_scrollable_content_viewport_geometry_get
         (obj, NULL, NULL, NULL, &v);
   elm_interface_scrollable_content_size_get(obj, NULL, &min);

   if (_elm_config->item_select_on_focus_disable)
     {
        ret = _item_focused_next(obj, dir);
     }
   else
     {
        if (multi)
          {
             if (dir == ELM_FOCUS_UP)
               ret = _item_multi_select_up(sd);
             else if (dir == ELM_FOCUS_DOWN)
               ret = _item_multi_select_down(sd);
          }
        else
          {
             if ((sd->focused_item) && (dir == ELM_FOCUS_UP))
               ret = _item_single_select_up(sd);
             else if (dir == ELM_FOCUS_DOWN)
               ret = _item_single_select_down(sd);
          }
     }
   if (ret)
     return EINA_TRUE;

   focus_only = _elm_config->item_select_on_focus_disable;
   // handle item loop feature
   if (sd->item_loop_enable && !sd->item_looping_on)
     {
        if (min < v)
          {
             if (dir == ELM_FOCUS_UP)
               {
                  elm_layout_signal_emit(obj, "elm,action,looping,up", "elm");
                  sd->item_looping_on = EINA_TRUE;
               }
             else if (dir == ELM_FOCUS_DOWN)
               {
                  elm_layout_signal_emit(obj, "elm,action,looping,down", "elm");
                  sd->item_looping_on = EINA_TRUE;
               }
          }
        else
          {
             if (dir == ELM_FOCUS_UP)
               {
                  it = elm_genlist_last_item_get(obj);
                  ELM_GENLIST_ITEM_DATA_GET(it, gen_it);
                  while (_is_no_select(gen_it) || elm_wdg_item_disabled_get(it))
                    it = elm_genlist_item_prev_get(it);
               }
            else if (dir == ELM_FOCUS_DOWN)
              {
                 it = elm_genlist_first_item_get(obj);
                 ELM_GENLIST_ITEM_DATA_GET(it, gen_it);
                 while (_is_no_select(gen_it) || elm_wdg_item_disabled_get(it))
                   it = elm_genlist_item_next_get(it);
              }

             if (it && focus_only)
               elm_object_item_focus_set(it, EINA_TRUE);
             else if (it)
               elm_genlist_item_selected_set(it, EINA_TRUE);
          }
        return EINA_TRUE;
     }
   else if (sd->item_looping_on)
     return EINA_TRUE;

   return EINA_FALSE;
}

static Eina_Bool
_key_action_move(Evas_Object *obj, const char *params)
{
   ELM_GENLIST_DATA_GET(obj, sd);
   const char *dir = params;

   Evas_Coord x = 0;
   Evas_Coord y = 0;
   Evas_Coord v_w = 0;
   Evas_Coord v_h = 0;
   Evas_Coord step_x = 0;
   Evas_Coord step_y = 0;
   Evas_Coord page_x = 0;
   Evas_Coord page_y = 0;
   Elm_Object_Item *it = NULL;
   Evas_Coord pan_max_x = 0, pan_max_y = 0;

   if (!sd->items) return EINA_FALSE;
   elm_interface_scrollable_content_pos_get(obj, &x, &y);
   elm_interface_scrollable_step_size_get(obj, &step_x, &step_y);
   elm_interface_scrollable_page_size_get(obj, &page_x, &page_y);
   elm_interface_scrollable_content_viewport_geometry_get
         (obj, NULL, NULL, &v_w, &v_h);

   _elm_widget_focus_auto_show(obj);
   if (!strcmp(dir, "left"))
     {
        x -= step_x;

        Elm_Object_Item *eo_gt = elm_genlist_selected_item_get(obj);
        ELM_GENLIST_ITEM_DATA_GET(eo_gt, gt);
        _elm_genlist_item_content_focus_set(gt, ELM_FOCUS_LEFT);

        return EINA_FALSE;
     }
   else if (!strcmp(dir, "right"))
     {
        x += step_x;

        Elm_Object_Item *eo_gt = elm_genlist_selected_item_get(obj);
        ELM_GENLIST_ITEM_DATA_GET(eo_gt, gt);
        _elm_genlist_item_content_focus_set(gt, ELM_FOCUS_RIGHT);

        return EINA_FALSE;
     }
   else if (!strcmp(dir, "up"))
     {
        if (_key_action_move_dir(obj, ELM_FOCUS_UP, EINA_FALSE)) return EINA_TRUE;
        else return EINA_FALSE;
     }
   else if (!strcmp(dir, "up_multi"))
     {
        if (_key_action_move_dir(obj, ELM_FOCUS_UP, EINA_TRUE)) return EINA_TRUE;
        else if (_key_action_move_dir(obj, ELM_FOCUS_UP, EINA_FALSE)) return EINA_TRUE;
        else return EINA_FALSE;
     }
   else if (!strcmp(dir, "down"))
     {
        if (_key_action_move_dir(obj, ELM_FOCUS_DOWN, EINA_FALSE)) return EINA_TRUE;
        else return EINA_FALSE;
     }
   else if (!strcmp(dir, "down_multi"))
     {
        if (_key_action_move_dir(obj, ELM_FOCUS_DOWN, EINA_TRUE)) return EINA_TRUE;
        else if (_key_action_move_dir(obj, ELM_FOCUS_DOWN, EINA_FALSE)) return EINA_TRUE;
        else return EINA_FALSE;
     }
   else if (!strcmp(dir, "first"))
     {
        it = elm_genlist_first_item_get(obj);
        if (it)
          {
             if (_elm_config->item_select_on_focus_disable)
               {
                  elm_object_item_focus_set(it, EINA_TRUE);
                  elm_genlist_item_show(it, ELM_GENLIST_ITEM_SCROLLTO_TOP);
               }
             else
               {
                  elm_genlist_item_selected_set(it, EINA_TRUE);
               }
             return EINA_TRUE;
          }
     }
   else if (!strcmp(dir, "last"))
     {
        it = elm_genlist_last_item_get(obj);
        if (it)
          {
             if (_elm_config->item_select_on_focus_disable)
               {
                  elm_object_item_focus_set(it, EINA_TRUE);
                  elm_genlist_item_show(it, ELM_GENLIST_ITEM_SCROLLTO_BOTTOM);
               }
             else
               {
                  elm_genlist_item_selected_set(it, EINA_TRUE);
               }
             return EINA_TRUE;
          }
     }
   else if (!strcmp(dir, "prior"))
     {
        if (page_y < 0)
          y -= -(page_y * v_h) / 100;
        else
          y -= page_y;
     }
   else if (!strcmp(dir, "next"))
     {
        if (page_y < 0)
          y += -(page_y * v_h) / 100;
        else
          y += page_y;
     }
   else return EINA_FALSE;

   elm_obj_pan_pos_max_get(sd->pan_obj, &pan_max_x, &pan_max_y);
   if (x < 0)
     x = 0;
   else if (x > pan_max_x)
     x = pan_max_x;
   if (y < 0)
     y = 0;
   else if (y > pan_max_y)
     y = pan_max_y;

   elm_interface_scrollable_content_pos_set(obj, x, y, EINA_TRUE);

   return EINA_TRUE;
}

static Eina_Bool
_key_action_select(Evas_Object *obj, const char *params)
{
   Elm_Object_Item *eo_it = NULL;

   ELM_GENLIST_DATA_GET(obj, sd);
   if (!sd->items) return EINA_FALSE;

   eo_it = elm_object_focused_item_get(obj);
   if (!eo_it) return EINA_TRUE;
   elm_genlist_item_expanded_set(eo_it, !elm_genlist_item_expanded_get(eo_it));
   ELM_GENLIST_ITEM_DATA_GET(eo_it, it);

   if (sd->multi &&
       ((sd->multi_select_mode != ELM_OBJECT_MULTI_SELECT_MODE_WITH_CONTROL) ||
        (!strcmp(params, "multi"))))
     {
        if (!it->selected)
          {
             _item_highlight(it);
             if (_item_select(it)) goto deleted;
          }
        else
         _item_unselect(it);
     }
   else
     {
        if (!it->selected)
          {
             while (sd->selected)
               {
                  Elm_Object_Item *eo_sel = sd->selected->data;
                  Elm_Gen_Item *sel = efl_data_scope_get(eo_sel, ELM_GENLIST_ITEM_CLASS);
                  _item_unselect(sel);
               }
          }
        else
          {
             const Eina_List *l, *l_next;
             Elm_Object_Item *eo_it2;

             EINA_LIST_FOREACH_SAFE(sd->selected, l, l_next, eo_it2)
               {
                  ELM_GENLIST_ITEM_DATA_GET(eo_it2, it2);
                  if (it2 != it)
                    _item_unselect(it2);
               }
          }
        _item_highlight(it);
        if (_item_select(it)) goto deleted;
     }

   efl_event_callback_legacy_call(WIDGET(it), ELM_GENLIST_EVENT_ACTIVATED, EO_OBJ(it));

   return EINA_TRUE;

deleted:
   return EINA_FALSE;
}

static Eina_Bool
_key_action_escape(Evas_Object *obj, const char *params EINA_UNUSED)
{
   ELM_GENLIST_DATA_GET(obj, sd);

   if (!_all_items_deselect(sd)) return EINA_FALSE;
   return EINA_TRUE;
}

EOLIAN static Eina_Bool
_elm_genlist_efl_ui_widget_widget_sub_object_add(Eo *obj, Elm_Genlist_Data *_pd EINA_UNUSED, Evas_Object *sobj)
{
   /* skipping layout's code, which registers size hint changing
    * callback on sub objects. this is here because items'
    * content_get() routines may change hints on the objects after
    * creation, thus issuing TOO MANY sizing_eval()'s here. they are
    * not needed at here anyway, so let's skip listening to those
    * hints changes */
   return efl_ui_widget_sub_object_add(efl_cast(obj, EFL_UI_WIDGET_CLASS), sobj);
}

EOLIAN static Eina_Bool
_elm_genlist_efl_ui_widget_widget_sub_object_del(Eo *obj, Elm_Genlist_Data *sd, Evas_Object *sobj)
{
   Eina_Bool int_ret = EINA_FALSE;

   /* XXX: hack -- also skipping sizing recalculation on
    * sub-object-del. genlist's crazy code paths (like groups and
    * such) seem to issue a whole lot of deletions and Evas bitches
    * about too many recalculations */
   sd->on_sub_del = EINA_TRUE;
   int_ret = efl_ui_widget_sub_object_del(efl_super(obj, MY_CLASS), sobj);
   sd->on_sub_del = EINA_FALSE;
   return int_ret;
}

/*
 * This function searches the nearest visible item based on the given item.
 * If the given item is in the genlist viewport, this returns the given item.
 * Or this searches the realized items and checks the nearest fully visible item
 * according to the given item's position.
 */
static Elm_Object_Item *
_elm_genlist_nearest_visible_item_get(Evas_Object *obj, Elm_Object_Item *eo_it)
{
   Evas_Coord vx = 0, vy = 0, vw = 0, vh = 0; // genlist viewport geometry
   Evas_Coord ix = 0, iy = 0, iw = 0, ih = 0; // given item geometry
   Evas_Coord cx = 0, cy = 0, cw = 0, ch = 0; // candidate item geometry
   Eina_List *item_list = NULL, *l = NULL;
   Elm_Object_Item *eo_item = NULL;
   ELM_GENLIST_DATA_GET(obj, sd);
   Eina_Bool search_next = EINA_FALSE;

   if (!eo_it) return NULL;
   ELM_GENLIST_ITEM_DATA_GET(eo_it, it);

   evas_object_geometry_get(sd->pan_obj, &vx, &vy, &vw, &vh);
   evas_object_geometry_get(VIEW(it), &ix, &iy, &iw, &ih); // FIXME: check if the item is realized or not

   if (ELM_RECTS_INCLUDE(vx, vy, vw, vh, ix, iy, iw, ih))
     {
        if (!elm_object_item_disabled_get(eo_it))
          return eo_it;
        else
          search_next = EINA_TRUE;
     }

   item_list = elm_genlist_realized_items_get(obj);

   if ((iy < vy) || search_next)
     {
        EINA_LIST_FOREACH(item_list, l, eo_item)
          {
             ELM_GENLIST_ITEM_DATA_GET(eo_item, item);
             evas_object_geometry_get(VIEW(item), &cx, &cy, &cw, &ch);
             if (ELM_RECTS_INCLUDE(vx, vy, vw, vh, cx, cy, cw, ch) &&
                 !elm_object_item_disabled_get(eo_item))
               {
                  eina_list_free(item_list);
                  return eo_item;
               }
          }
     }
   else
     {
        EINA_LIST_REVERSE_FOREACH(item_list, l, eo_item)
          {
             ELM_GENLIST_ITEM_DATA_GET(eo_item, item);
             evas_object_geometry_get(VIEW(item), &cx, &cy, &cw, &ch);
             if (ELM_RECTS_INCLUDE(vx, vy, vw, vh, cx, cy, cw, ch) &&
                 !elm_object_item_disabled_get(eo_item))
               {
                  eina_list_free(item_list);
                  return eo_item;
               }
          }
     }
   eina_list_free(item_list);

   return eo_it;
}

static void
_elm_genlist_focus_highlight_show(void *data EINA_UNUSED,
                                  Evas_Object *obj,
                                  const char *emission EINA_UNUSED,
                                  const char *src EINA_UNUSED)
{
   ELM_GENLIST_DATA_GET(obj, sd);

   if (sd->focused_item)
     {
        if (!sd->focused_content)
          {
             Eina_Bool found = EINA_FALSE;
             ELM_GENLIST_ITEM_DATA_GET(sd->focused_item, it);
             found = _item_focusable_search(&it, 1);
             if (found)
               {
                sd->focus_scrollto_type = ELM_GENLIST_ITEM_SCROLLTO_IN;
                elm_object_item_focus_set(EO_OBJ(it), EINA_TRUE);
               }
          }
        else elm_object_focus_set(sd->focused_content, EINA_TRUE);
     }
}

static void
_elm_genlist_focus_highlight_hide(void *data EINA_UNUSED,
                                  Evas_Object *obj,
                                  const char *emission EINA_UNUSED,
                                  const char *src EINA_UNUSED)
{
   ELM_GENLIST_DATA_GET(obj, sd);
   if (sd->focused_item)
     {
        // Do not use _item_unfocused because focus should be remained
        ELM_GENLIST_ITEM_DATA_GET(sd->focused_item, it);
        edje_object_signal_emit
           (VIEW(it), SIGNAL_UNFOCUSED, "elm");
        if (it->deco_all_view)
           edje_object_signal_emit
              (it->deco_all_view, SIGNAL_UNFOCUSED, "elm");
     }
}

EOLIAN static Eina_Rect
_elm_genlist_efl_ui_widget_focus_highlight_geometry_get(const Eo *obj, Elm_Genlist_Data *sd)
{
   Evas_Coord ox, oy, oh, ow, item_x = 0, item_y = 0, item_w = 0, item_h = 0;
   Eina_Rect r = {};

   evas_object_geometry_get(sd->pan_obj, &ox, &oy, &ow, &oh);

   if (sd->focused_item)
     {
        ELM_GENLIST_ITEM_DATA_GET(sd->focused_item, focus_it);
        evas_object_geometry_get(VIEW(focus_it), &item_x, &item_y, &item_w, &item_h);
        elm_widget_focus_highlight_focus_part_geometry_get(VIEW(focus_it), &item_x, &item_y, &item_w, &item_h);
     }
   else
     {
        evas_object_geometry_get(obj, &r.x, &r.y, &r.w, &r.h);
        return r;
     }

   r.x = item_x;
   r.y = item_y;
   r.w = item_w;
   r.h = item_h;

   if (item_y < oy)
     {
        r.y = oy;
     }
   if (item_y > (oy + oh - item_h))
     {
        r.y = oy + oh - item_h;
     }

   if ((item_x + item_w) > (ox + ow))
     {
        r.w = ow;
     }
   if (item_x < ox)
     {
        r.x = ox;
     }

   return r;
}

EOLIAN static void
_elm_genlist_efl_ui_focus_manager_setup_on_first_touch(Eo *obj, Elm_Genlist_Data *sd, Efl_Ui_Focus_Direction direction EINA_UNUSED, Efl_Ui_Focus_Object *entry EINA_UNUSED)
{
   Elm_Object_Item *eo_it = NULL;
   Eina_Bool is_sel = EINA_FALSE;

   if (sd->last_focused_item)
     eo_it = sd->last_focused_item;
   else if (sd->last_selected_item)
     eo_it = sd->last_selected_item;
   else if (_elm_config->first_item_focus_on_first_focus_in || !eo_it)
     {
        eo_it = elm_genlist_first_item_get(obj);
        is_sel = EINA_TRUE;
     }

   while (eo_it)
     {
        ELM_GENLIST_ITEM_DATA_GET(eo_it, it);
        if ((!_is_no_select(it)) && (!elm_object_item_disabled_get(eo_it)))
          break;
        eo_it = EO_OBJ(ELM_GEN_ITEM_NEXT(it));
     }

   if (eo_it)
     {
        eo_it = _elm_genlist_nearest_visible_item_get(obj, eo_it);
        if (eo_it)
          {
             if (!_elm_config->item_select_on_focus_disable && is_sel)
               elm_genlist_item_selected_set(eo_it, EINA_TRUE);
             else
               elm_object_item_focus_set(eo_it, EINA_TRUE);
            _elm_widget_focus_highlight_start(obj);
            //set it again in the manager, there might be the case that the manager focus history and internal item foused logic are in different states
            efl_ui_focus_manager_focus_set(obj, eo_it);
          }
     }
   else
     {
        efl_ui_focus_object_focus_set(obj, EINA_TRUE);
     }
}

static Efl_Ui_Focus_Object*
_select_candidate(Eo *obj, Elm_Genlist_Data *pd, Efl_Ui_Focus_Direction direction)
{
   Elm_Object_Item *first = elm_genlist_first_item_get(obj);
   Elm_Object_Item *last = elm_genlist_last_item_get(obj);

   switch(direction)
     {
        case EFL_UI_FOCUS_DIRECTION_DOWN:
        case EFL_UI_FOCUS_DIRECTION_RIGHT:
          elm_object_item_focus_set(first, EINA_TRUE);
          return obj;
        break;
        case EFL_UI_FOCUS_DIRECTION_UP:
        case EFL_UI_FOCUS_DIRECTION_LEFT:
          elm_object_item_focus_set(last, EINA_TRUE);
          return obj;
        break;
        case EFL_UI_FOCUS_DIRECTION_NEXT:
        case EFL_UI_FOCUS_DIRECTION_PREVIOUS:
          //do not go further with logical movement
          return NULL;
        break;
        default:
          ERR("Uncaught focus direction");
          return NULL;
        break;
     }
}

EOLIAN static Efl_Ui_Focus_Object*
_elm_genlist_efl_ui_focus_manager_move(Eo *obj, Elm_Genlist_Data *pd, Efl_Ui_Focus_Direction direction)
{
   if (efl_ui_focus_manager_focus_get(obj) == obj)
     {
        return _select_candidate(obj, pd, direction);
     }
   else
     {
        return efl_ui_focus_manager_move(efl_super(obj, MY_CLASS), direction);
     }
}

EOLIAN static Efl_Ui_Focus_Object*
_elm_genlist_efl_ui_focus_manager_manager_focus_get(const Eo *obj, Elm_Genlist_Data *pd)
{
   Eo *focused_obj = efl_ui_focus_manager_focus_get(efl_super(obj, MY_CLASS));
   Eo *registered_manager = efl_ui_focus_object_focus_manager_get(obj);

   if (!focused_obj && efl_ui_focus_manager_redirect_get(registered_manager))
     return (Efl_Ui_Focus_Object*) obj;


   return focused_obj;
}

EOLIAN static Eina_Bool
_elm_genlist_efl_ui_focus_object_on_focus_update(Eo *obj, Elm_Genlist_Data *sd)
{
   Eina_Bool int_ret = EINA_FALSE;

   int_ret = efl_ui_focus_object_on_focus_update(efl_super(obj, MY_CLASS));
   if (!int_ret) return EINA_FALSE;

   if (efl_ui_focus_object_focus_get(obj) && (sd->items) && (sd->selected) &&
       (!sd->last_selected_item))
     {
        sd->last_selected_item = eina_list_data_get(sd->selected);
     }

   return EINA_TRUE;
}

EOLIAN static void
_elm_genlist_item_elm_widget_item_style_set(Eo *eo_it,
                                            Elm_Gen_Item *it,
                                            const char *style)
{
   if (it->itc && !strcmp(it->itc->item_style, style)) return;

   Elm_Genlist_Item_Class *itc = elm_genlist_item_class_new();

   itc->item_style = style;
   if (it->itc)
     {
        itc->func.text_get = it->itc->func.text_get;
        itc->func.content_get = it->itc->func.content_get;
        itc->func.state_get = it->itc->func.state_get;
        itc->func.filter_get = it->itc->func.filter_get;
        itc->func.reusable_content_get = it->itc->func.reusable_content_get;
        itc->decorate_item_style = it->itc->decorate_item_style;
        itc->decorate_all_item_style = it->itc->decorate_all_item_style;
     }

   elm_genlist_item_item_class_update(eo_it, itc);
   elm_genlist_item_class_free(itc);
}

EOLIAN static const char *
_elm_genlist_item_elm_widget_item_style_get(const Eo *eo_it EINA_UNUSED,
                                            Elm_Gen_Item *it)
{
   if (it->itc) return it->itc->item_style;
   else return NULL;
}

EOLIAN static void
_elm_genlist_item_elm_widget_item_item_focus_set(Eo *eo_it EINA_UNUSED, Elm_Gen_Item *it, Eina_Bool focused)
{
   Evas_Object *obj = WIDGET(it);
   ELM_GENLIST_DATA_GET(obj, sd);

   if (focused)
     {
        if (!elm_object_focus_get(obj))
          elm_object_focus_set(obj, EINA_TRUE);

        if (!elm_object_focus_get(obj))
          return;

        if (sd->focused_content)
          {
             elm_object_focus_set(sd->focused_content, EINA_FALSE);
             sd->focused_content = NULL;
          }

        if (EO_OBJ(it) != sd->focused_item)
          {
             if (sd->focused_item)
               {
                  ELM_GENLIST_ITEM_DATA_GET(sd->focused_item, focus_it);
                  _item_unfocused(focus_it);
               }
             _item_focused(it, sd->focus_scrollto_type);
             //efl_ui_focus_manager_focus_set(it->base->widget, eo_it);

             /* If item is not realized state, widget couldn't get focus_highlight data. */
             if (it->realized)
               {
                  _elm_widget_item_highlight_in_theme(obj, EO_OBJ(it));
                  _elm_widget_highlight_in_theme_update(obj);
                  _elm_widget_focus_highlight_start(obj);
               }
             efl_ui_focus_manager_focus_set(it->base->widget, eo_it);
          }
     }
   else
     {
        if (!elm_object_focus_get(obj))
          return;
        _item_unfocused(it);
     }
}

EOLIAN static Eina_Bool
_elm_genlist_item_elm_widget_item_item_focus_get(const Eo *eo_it, Elm_Gen_Item *it)
{
   Evas_Object *obj = WIDGET(it);
   ELM_GENLIST_DATA_GET(obj, sd);

   if (eo_it == sd->focused_item)
     return EINA_TRUE;

   return EINA_FALSE;
}

EOLIAN static Elm_Object_Item*
_elm_genlist_elm_widget_item_container_focused_item_get(const Eo *obj EINA_UNUSED, Elm_Genlist_Data *sd)
{
   return sd->focused_item;
}

static Eina_Bool _elm_genlist_smart_focus_next_enable = EINA_FALSE;

EOLIAN static Eina_Bool
_elm_genlist_efl_ui_widget_focus_next_manager_is(Eo *obj EINA_UNUSED, Elm_Genlist_Data *_pd EINA_UNUSED)
{
   return _elm_genlist_smart_focus_next_enable;
}

EOLIAN static Eina_Bool
_elm_genlist_efl_ui_widget_focus_direction_manager_is(Eo *obj EINA_UNUSED, Elm_Genlist_Data *_pd EINA_UNUSED)
{
   return EINA_FALSE;
}

EOLIAN static Eina_Bool
_elm_genlist_efl_ui_widget_focus_next(Eo *obj, Elm_Genlist_Data *sd, Efl_Ui_Focus_Direction dir, Evas_Object **next, Elm_Object_Item **next_item)
{
   Eina_List *items = NULL;
   Item_Block *itb = NULL;
   Elm_Object_Item *eo_it = NULL;

   if (!sd->blocks) return EINA_FALSE;
   if (!sd->is_access) return EINA_FALSE;

   if (!elm_widget_highlight_get(obj))
     {
        if (ELM_FOCUS_PREVIOUS == dir)
          {
             eo_it = elm_genlist_last_item_get(obj);
             itb = EINA_INLIST_CONTAINER_GET(sd->blocks->last, Item_Block);
          }
        else if (ELM_FOCUS_NEXT == dir)
          {
             eo_it = elm_genlist_first_item_get(obj);
             itb = EINA_INLIST_CONTAINER_GET(sd->blocks, Item_Block);
          }
        else return EINA_FALSE;

        if (eo_it && itb && itb->calc_done)
          {
             _item_block_realize(itb, EINA_TRUE);
             elm_genlist_item_show(eo_it, ELM_GENLIST_ITEM_SCROLLTO_IN);
          }
        else return EINA_FALSE;
     }

   // FIXME: do not use realized items get
   // because of above forcing realization case.
   EINA_INLIST_FOREACH(sd->blocks, itb)
     {
        Eina_List *l;
        Elm_Gen_Item *it;

        if (!itb->realized) continue;

        EINA_LIST_FOREACH(itb->items, l, it)
          {
             Eina_List *ll;
             Evas_Object *c, *ret;
             Eina_List *orders;

             if (!(it->realized)) continue;

             if (it->base->access_obj)
                items = eina_list_append(items, it->base->access_obj);
             else
                items = eina_list_append(items, VIEW(it));

             orders = (Eina_List *)elm_object_item_access_order_get(EO_OBJ(it));
             EINA_LIST_FOREACH(orders, ll, c)
               {
                  items = eina_list_append(items, c);
               }
          }
     }

   return efl_ui_widget_focus_list_next_get
            (obj, items, eina_list_data_get, dir, next, next_item);
}

static void
_mirrored_set(Evas_Object *obj,
              Eina_Bool rtl)
{
   efl_ui_mirrored_set(obj, rtl);
}

EOLIAN static Eina_Error
_elm_genlist_efl_ui_widget_theme_apply(Eo *obj, Elm_Genlist_Data *sd)
{
   Item_Block *itb;
   Eina_Error int_ret = EFL_UI_THEME_APPLY_ERROR_GENERIC;
   ELM_WIDGET_DATA_GET_OR_RETURN(obj, wsd, EFL_UI_THEME_APPLY_ERROR_GENERIC);

   int_ret = efl_ui_widget_theme_apply(efl_super(obj, MY_CLASS));
   if (int_ret) return EFL_UI_THEME_APPLY_ERROR_GENERIC;

   if (!TIZEN_PROFILE_WEARABLE)
     _banded_bg_state_check(obj, sd);

   EINA_INLIST_FOREACH(sd->blocks, itb)
     {
        Eina_List *l;
        Elm_Gen_Item *it;
        EINA_LIST_FOREACH(itb->items, l, it)
          {
             if (it->realized)
               {
                  _item_unrealize(it, EINA_FALSE);
                  GL_IT(it)->calc_done = EINA_FALSE;
                  GL_IT(it)->block->calc_done = EINA_FALSE;
               }
             else _item_queue(it, NULL);
          }
     }
   sd->calc_done = EINA_FALSE;
   _item_cache_all_free(sd);
   eina_hash_free_buckets(sd->size_caches);
   elm_layout_sizing_eval(obj);
   _changed(sd->pan_obj);

   edje_object_signal_emit(wsd->resize_obj, SIGNAL_FOCUS_BG_HIDE, "elm");

   if (wsd->scroll_item_align_enable && sd->aligned_item)
     elm_genlist_item_show(EO_OBJ(sd->aligned_item), ELM_GENLIST_ITEM_SCROLLTO_MIDDLE);

   return int_ret;
}

/* FIXME: take off later. maybe this show region coords belong in the
 * interface (new api functions, set/get)? */
static void
_show_region_hook(void *data EINA_UNUSED, Evas_Object *obj, Eina_Rect r)
{
   ELM_GENLIST_DATA_GET_OR_RETURN(obj, sd);

   //x & y are screen coordinates, Add with pan coordinates
   r.x += sd->pan_x;
   r.y += sd->pan_y;
   elm_interface_scrollable_content_region_show(obj, r.x, r.y, r.w, r.h);
}

EOLIAN static Eina_Bool
_elm_genlist_efl_ui_widget_translate(Eo *obj, Elm_Genlist_Data *sd)
{
   Item_Block *itb;

   // Before calling text_get, inform user first.
   evas_object_smart_callback_call(obj, SIG_LANG_CHANGED, NULL);

   // FIXME: We should change item's height if lang is changed??
   EINA_INLIST_FOREACH(sd->blocks, itb)
     {
        Eina_List *l;
        Elm_Gen_Item *it;
        EINA_LIST_FOREACH(itb->items, l, it)
          {
             if (it->realized)
               {
                  elm_genlist_item_fields_update(EO_OBJ(it),
                                                 NULL,
                                                 ELM_GENLIST_ITEM_FIELD_TEXT |
                                                 ELM_GENLIST_ITEM_FIELD_CONTENT);
               }
             else GL_IT(it)->calc_done = EINA_FALSE;
          }
        itb->calc_done = EINA_FALSE;
     }

   eina_hash_free_buckets(sd->size_caches);
   sd->calc_done = EINA_FALSE;

   return EINA_TRUE;
}

EOLIAN static void
_elm_genlist_elm_interface_scrollable_item_loop_enabled_set(Eo *obj EINA_UNUSED, Elm_Genlist_Data *sd, Eina_Bool enable)
{
   // Need to be implemented
   sd->item_loop_enable = !!enable;
}

EOLIAN static Eina_Bool
_elm_genlist_elm_interface_scrollable_item_loop_enabled_get(const Eo *obj EINA_UNUSED, Elm_Genlist_Data *sd)
{
   // Need to be implemented
   return sd->item_loop_enable;
}

static void
_item_block_position_update(Eina_Inlist *list,
                            int idx)
{
   Item_Block *tmp;

   EINA_INLIST_FOREACH(list, tmp)
     {
        tmp->position = idx++;
        tmp->position_update = EINA_TRUE;
     }
}

static void
_item_position_update(Eina_List *list,
                      int idx)
{
   Elm_Gen_Item *it;
   Eina_List *l;

   EINA_LIST_FOREACH(list, l, it)
     {
        it->position = idx++;
        it->position_update = EINA_TRUE;
     }
}

static void
_item_block_merge(Item_Block *left,
                  Item_Block *right)
{
   Eina_List *l;
   Elm_Gen_Item *it2;

   EINA_LIST_FOREACH(right->items, l, it2)
     {
        it2->item->block = left;
        left->count++;
        left->calc_done = EINA_FALSE;
     }
   left->items = eina_list_merge(left->items, right->items);
}

static void
_item_block_del(Elm_Gen_Item *it)
{
   Elm_Genlist_Data *sd = GL_IT(it)->wsd;
   Eina_Inlist *il;
   Item_Block *itb = GL_IT(it)->block;
   Eina_Bool block_changed = EINA_FALSE;
   Evas_Coord vh;

   elm_interface_scrollable_content_viewport_geometry_get
      (sd->obj, NULL, NULL, NULL, &vh);
   if (sd->processed_sizes >= vh) sd->processed_sizes = 0;

   itb->items = eina_list_remove(itb->items, it);
   itb->count--;
   itb->calc_done = EINA_FALSE;
   sd->calc_done = EINA_FALSE;
   _changed(sd->pan_obj);
   if (itb->count < 1)
     {
        Item_Block *itbn;

        il = EINA_INLIST_GET(itb);
        itbn = (Item_Block *)(il->next);
        if (it->parent)
          it->parent->item->items =
            eina_list_remove(it->parent->item->items, EO_OBJ(it));
        else
          {
             _item_block_position_update(il->next, itb->position);
          }
        GL_IT(it)->wsd->blocks =
          eina_inlist_remove(GL_IT(it)->wsd->blocks, il);
        GL_IT(it)->wsd->blocks_realized = eina_list_remove
           (GL_IT(it)->wsd->blocks_realized, itb);
        free(itb);
        itb = NULL;
        if (itbn) itbn->calc_done = EINA_FALSE;
     }
   else
     {
        if (itb->count < (itb->sd->max_items_per_block / 2))
          {
             Item_Block *itbp;
             Item_Block *itbn;

             il = EINA_INLIST_GET(itb);
             itbp = (Item_Block *)(il->prev);
             itbn = (Item_Block *)(il->next);

             /* merge block with previous */
             if ((itbp) &&
                 ((itbp->count + itb->count) <
                  (itb->sd->max_items_per_block +
                   (itb->sd->max_items_per_block / 2))))
               {
                  _item_block_merge(itbp, itb);
                  _item_block_position_update
                    (EINA_INLIST_GET(itb)->next, itb->position);
                  GL_IT(it)->wsd->blocks = eina_inlist_remove
                      (GL_IT(it)->wsd->blocks, EINA_INLIST_GET(itb));
                  GL_IT(it)->wsd->blocks_realized = eina_list_remove
                     (GL_IT(it)->wsd->blocks_realized, itb);
                  free(itb);
                  block_changed = EINA_TRUE;
               }
             /* merge block with next */
             else if ((itbn) &&
                      ((itbn->count + itb->count) <
                       (itb->sd->max_items_per_block +
                        (itb->sd->max_items_per_block / 2))))
               {
                  _item_block_merge(itb, itbn);
                  _item_block_position_update
                    (EINA_INLIST_GET(itbn)->next, itbn->position);
                  GL_IT(it)->wsd->blocks =
                    eina_inlist_remove(GL_IT(it)->wsd->blocks,
                                       EINA_INLIST_GET(itbn));
                  GL_IT(it)->wsd->blocks_realized = eina_list_remove
                     (GL_IT(it)->wsd->blocks_realized, itbn);
                  free(itbn);
                  block_changed = EINA_TRUE;
               }
          }
     }

   if (block_changed)
     {
        _changed(sd->pan_obj);
     }

   GL_IT(it)->block = NULL;
}

static void
_decorate_all_item_unrealize(Elm_Gen_Item *it)
{
   if (!it->deco_all_view) return;

   edje_object_part_unswallow(it->deco_all_view, VIEW(it));
   _view_clear(it->deco_all_view, &(GL_IT(it)->deco_all_contents));
   evas_object_del(it->deco_all_view);
   it->deco_all_view = NULL;

   edje_object_signal_emit(VIEW(it), SIGNAL_DECORATE_DISABLED, "elm");

   _item_mouse_callbacks_add(it, VIEW(it));
   evas_object_smart_member_add(VIEW(it), GL_IT(it)->wsd->pan_obj);
}


static void
_item_mouse_move_cb(void *data,
                    Evas *evas EINA_UNUSED,
                    Evas_Object *obj,
                    void *event_info)
{
   Evas_Event_Mouse_Move *ev = event_info;
   Elm_Gen_Item *it = data;
   Elm_Genlist_Data *sd = GL_IT(it)->wsd;

   if (!it->down) return;

   if (((sd->reorder_force) || (sd->reorder_mode)) &&
       (sd->reorder.it == it) &&
       (!it->hide))
     {
        Evas_Coord ox;
        evas_object_geometry_get(sd->pan_obj, &ox, NULL, NULL, NULL);

        if ((ev->cur.canvas.y - it->dy) > GL_IT(it)->scrl_y)
           sd->reorder.dir = 1;
        else if ((ev->cur.canvas.y - it->dy) < GL_IT(it)->scrl_y)
           sd->reorder.dir = -1;

        GL_IT(it)->scrl_x = it->x + GL_IT(it)->block->x - sd->pan_x + ox;
        GL_IT(it)->scrl_y = ev->cur.canvas.y - it->dy;
        GL_IT(it)->w = sd->minw;
        GL_IT(it)->h = GL_IT(it)->minh;
        _changed(sd->pan_obj);
     }

   if (ev->event_flags & EVAS_EVENT_FLAG_ON_HOLD)
     {
        if (GL_IT(it)->highlight_timer)
          {
             ecore_timer_del(GL_IT(it)->highlight_timer);
             GL_IT(it)->highlight_timer = NULL;
          }
        else if (!it->selected) _item_unhighlight(it, EINA_FALSE);

        if (it->long_timer)
          {
             ecore_timer_del(it->long_timer);
             it->long_timer = NULL;
          }
         sd->on_hold = EINA_TRUE; /* for checking need to start select timer */
     }
   //******************** TIZEN Only
   else
     {
        if (((sd->reorder_force) || (sd->reorder_mode)) &&
            (sd->reorder.it == it) && (!it->hide))
          return;
        Evas_Coord x, y, w, h;
        evas_object_geometry_get(obj, &x, &y, &w, &h);
        if (ELM_RECTS_POINT_OUT(x, y, w, h, ev->cur.canvas.x, ev->cur.canvas.y))
          {
             if (it->long_timer)
               {
                  ecore_timer_del(it->long_timer);
                  it->long_timer = NULL;
               }
             if (GL_IT(it)->highlight_timer)
               {
                  ecore_timer_del(GL_IT(it)->highlight_timer);
                  GL_IT(it)->highlight_timer = NULL;
               }
             else if (!it->selected) _item_unhighlight(it, EINA_FALSE);
          }
     }
   //*******************************

   evas_object_ref(obj);

   if (!it->dragging)
     {
        Evas_Coord minw = 0, minh = 0, x, y, dx, dy, adx, ady;
        minw = GL_IT(it)->wsd->finger_minw;
        minh = GL_IT(it)->wsd->finger_minh;

        evas_object_geometry_get(obj, &x, &y, NULL, NULL);
        dx = ev->cur.canvas.x - x;
        dy = ev->cur.canvas.y - y;
        dx = dx - it->dx;
        dy = dy - it->dy;
        adx = dx;
        ady = dy;
        if (adx < 0) adx = -dx;
        if (ady < 0) ady = -dy;
        if ((adx > minw) || (ady > minh))
          {
             it->dragging = EINA_TRUE;
             /* Genlist is scrolled vertically, so reduce left or right case for accuracy. */
             if (adx > (ady * 2))
               {
                  if (dx < 0)
                    evas_object_smart_callback_call
                       (WIDGET(it), SIG_DRAG_START_LEFT, EO_OBJ(it));
                  else
                    evas_object_smart_callback_call
                       (WIDGET(it), SIG_DRAG_START_RIGHT, EO_OBJ(it));
               }
             else
               {
                  if (dy < 0)
                    evas_object_smart_callback_call
                       (WIDGET(it), SIG_DRAG_START_UP, EO_OBJ(it));
                  else
                    evas_object_smart_callback_call
                       (WIDGET(it), SIG_DRAG_START_DOWN, EO_OBJ(it));
               }
          }
     }

   /* If item magic value is changed, do not call smart callback*/
   if (EINA_MAGIC_CHECK(it->base, ELM_WIDGET_ITEM_MAGIC))
     {
        if (it->dragging)
          evas_object_smart_callback_call(WIDGET(it), SIG_DRAG, EO_OBJ(it));
     }

   evas_object_unref(obj);

}

static Eina_Bool
_long_press_cb(void *data)
{
   Elm_Gen_Item *it = data;
   Elm_Object_Item *eo_it2;
   Elm_Genlist_Data *sd;
   Eina_List *l, *ll;

   sd = GL_IT(it)->wsd;
   it->long_timer = NULL;

   if (elm_wdg_item_disabled_get(EO_OBJ(it)) ||
       (it->select_mode == ELM_OBJECT_SELECT_MODE_DISPLAY_ONLY) ||
       (it->select_mode == ELM_OBJECT_SELECT_MODE_NONE) ||
       (sd->select_mode == ELM_OBJECT_SELECT_MODE_DISPLAY_ONLY) ||
       (sd->select_mode == ELM_OBJECT_SELECT_MODE_NONE))
     goto end;

   if ((sd->reorder_mode) &&
       (GL_IT(it)->type != ELM_GENLIST_ITEM_GROUP) &&
       (!sd->key_down_item))
     {
        if (elm_genlist_item_expanded_get(EO_OBJ(it)))
          {
             elm_genlist_item_expanded_set(EO_OBJ(it), EINA_FALSE);
          }
        EINA_LIST_FOREACH_SAFE(sd->selected, l, ll, eo_it2)
          {
             ELM_GENLIST_ITEM_DATA_GET(eo_it2, it2);
             _item_unselect(it2);
          }

        elm_interface_scrollable_hold_set(sd->obj, EINA_TRUE);
        elm_interface_scrollable_bounce_allow_set
              (sd->obj, EINA_FALSE, EINA_FALSE);

        if (TIZEN_PROFILE_WEARABLE)
          {

             if (sd->decorate_all_mode)
               edje_object_signal_emit(VIEW(it), SIGNAL_REORDER_ENABLED, "elm");
          }
        else
          {
             edje_object_signal_emit(VIEW(it), SIGNAL_REORDER_ENABLED, "elm");
          }

        sd->reorder.it = it;
        _changed(sd->pan_obj);
     }
   evas_object_smart_callback_call(WIDGET(it), SIG_LONGPRESSED, EO_OBJ(it));

end:
   it->long_timer = NULL;
   return ECORE_CALLBACK_CANCEL;
}

void
_gesture_do(void *data)
{
   Elm_Genlist_Data *sd = data;

   if ((sd->g_item) && (sd->g_type))
     {
        if (!strcmp(sd->g_type, SIG_MULTI_SWIPE_LEFT))
               evas_object_smart_callback_call
             (WIDGET(sd->g_item), SIG_MULTI_SWIPE_LEFT, EO_OBJ(sd->g_item));
        else if (!strcmp(sd->g_type, SIG_MULTI_SWIPE_RIGHT))
               evas_object_smart_callback_call
             (WIDGET(sd->g_item), SIG_MULTI_SWIPE_RIGHT, EO_OBJ(sd->g_item));
        else if (!strcmp(sd->g_type, SIG_MULTI_SWIPE_UP))
               evas_object_smart_callback_call
             (WIDGET(sd->g_item), SIG_MULTI_SWIPE_UP, EO_OBJ(sd->g_item));
        else if (!strcmp(sd->g_type, SIG_MULTI_SWIPE_DOWN))
               evas_object_smart_callback_call
             (WIDGET(sd->g_item), SIG_MULTI_SWIPE_DOWN, EO_OBJ(sd->g_item));
        else if (!strcmp(sd->g_type, SIG_MULTI_PINCH_OUT))
          evas_object_smart_callback_call
             (WIDGET(sd->g_item), SIG_MULTI_PINCH_OUT, EO_OBJ(sd->g_item));
        else if (!strcmp(sd->g_type, SIG_MULTI_PINCH_IN))
          evas_object_smart_callback_call
             (WIDGET(sd->g_item), SIG_MULTI_PINCH_IN, EO_OBJ(sd->g_item));
        else if (!strcmp(sd->g_type, SIG_SWIPE))
          evas_object_smart_callback_call
             (WIDGET(sd->g_item), SIG_SWIPE, EO_OBJ(sd->g_item));

        sd->g_item = NULL;
        sd->g_type = NULL;
     }
}

static void
_item_mouse_down_cb(void *data,
                    Evas *evas,
                    Evas_Object *obj,
                    void *event_info)
{
   Evas_Event_Mouse_Down *ev = event_info;
   Elm_Gen_Item *it = data;
   Elm_Genlist_Data *sd = GL_IT(it)->wsd;
   Evas_Coord x, y;

   if (ev->event_flags & EVAS_EVENT_FLAG_ON_SCROLL)
     {
        if (sd->reorder.it)
          {
             elm_genlist_item_reorder_stop(EO_OBJ(sd->reorder.it));
             sd->reorder.it = NULL;
             elm_interface_scrollable_hold_set(sd->obj, EINA_FALSE);
             elm_interface_scrollable_bounce_allow_set
                   (sd->obj, sd->h_bounce, sd->v_bounce);
          }
        return;
     }

   if (ev->button != 1) return;
   // mouse down is activate only one finger
   if (evas_event_down_count_get(evas) != 1) return;

   if (GL_IT(it)->highlight_timer)
     ecore_timer_del(GL_IT(it)->highlight_timer);
   // FIXME: To prevent timing issue about select and highlight
   else if (!sd->multi && sd->highlighted_item &&
            (GL_IT(sd->highlighted_item)->highlight_timer))
        return;

   it->down = EINA_TRUE;

   evas_object_geometry_get(obj, &x, &y, NULL, NULL);
   it->dx = ev->canvas.x - x;
   it->dy = ev->canvas.y - y;

   GL_IT(it)->wsd->was_selected = it->selected;
   GL_IT(it)->highlight_timer = ecore_timer_add(ELM_ITEM_HIGHLIGHT_TIMER,
                                               _highlight_timer, it);
   if (it->long_timer) ecore_timer_del(it->long_timer);
   it->long_timer = ecore_timer_add(sd->longpress_timeout, _long_press_cb, it);

   if (ev->flags & EVAS_BUTTON_DOUBLE_CLICK)
     {
        evas_object_smart_callback_call(WIDGET(it), SIG_CLICKED_DOUBLE, EO_OBJ(it));
        efl_event_callback_legacy_call(WIDGET(it), ELM_GENLIST_EVENT_ACTIVATED, EO_OBJ(it));

     }
   evas_object_smart_callback_call(WIDGET(it), SIG_PRESSED, EO_OBJ(it));
}

static Item_Block *
_item_block_new(Elm_Genlist_Data *sd,
                Eina_Bool prepend)
{
   Item_Block *itb;

   itb = calloc(1, sizeof(Item_Block));
   if (!itb) return NULL;
   itb->sd = sd;
   if (prepend)
     {
        sd->blocks = eina_inlist_prepend(sd->blocks, EINA_INLIST_GET(itb));
        _item_block_position_update(sd->blocks, 0);
     }
   else
     {
        sd->blocks = eina_inlist_append(sd->blocks, EINA_INLIST_GET(itb));
        itb->position_update = EINA_TRUE;
        if (sd->blocks != EINA_INLIST_GET(itb))
          {
             itb->position =
               ((Item_Block *)(EINA_INLIST_GET(itb)->prev))->position + 1;
          }
        else
          {
             itb->position = 0;
          }
     }

   return itb;
}

static Eina_Bool
_item_block_add(Elm_Genlist_Data *sd,
                Elm_Gen_Item *it)
{
   Item_Block *itb = NULL;
   if (GL_IT(it)->block) return EINA_TRUE;

   if (!GL_IT(it)->rel)
     {
newblock:
        if (GL_IT(it)->rel)
          {
             itb = calloc(1, sizeof(Item_Block));
             if (!itb) return EINA_FALSE;
             itb->sd = sd;
             if (!GL_IT(it)->rel->item->block)
               {
                  sd->blocks =
                    eina_inlist_append(sd->blocks, EINA_INLIST_GET(itb));
                  itb->items = eina_list_append(itb->items, it);
                  itb->position_update = EINA_TRUE;
                  it->position = eina_list_count(itb->items);
                  it->position_update = EINA_TRUE;

                  if (sd->blocks != EINA_INLIST_GET(itb))
                    {
                       itb->position =
                         ((Item_Block *)
                          (EINA_INLIST_GET(itb)->prev))->position + 1;
                    }
                  else
                    {
                       itb->position = 0;
                    }
               }
             else
               {
                  Eina_List *tmp;

                  tmp = eina_list_data_find_list(itb->items, GL_IT(it)->rel);
                  if (GL_IT(it)->before)
                    {
                       sd->blocks = eina_inlist_prepend_relative
                           (sd->blocks, EINA_INLIST_GET(itb),
                           EINA_INLIST_GET(GL_IT(it)->rel->item->block));
                       itb->items =
                         eina_list_prepend_relative_list(itb->items, it, tmp);

                       /* Update index from where we prepended */
                       _item_position_update
                         (eina_list_prev(tmp), GL_IT(it)->rel->position);
                       _item_block_position_update
                         (EINA_INLIST_GET(itb),
                         GL_IT(it)->rel->item->block->position);
                    }
                  else
                    {
                       sd->blocks = eina_inlist_append_relative
                           (sd->blocks, EINA_INLIST_GET(itb),
                           EINA_INLIST_GET(GL_IT(it)->rel->item->block));
                       itb->items =
                         eina_list_append_relative_list(itb->items, it, tmp);

                       /* Update block index from where we appended */
                       _item_position_update
                         (eina_list_next(tmp), GL_IT(it)->rel->position + 1);
                       _item_block_position_update
                         (EINA_INLIST_GET(itb),
                         GL_IT(it)->rel->item->block->position + 1);
                    }
               }
          }
        else
          {
             if (GL_IT(it)->before)
               {
                  if (sd->blocks)
                    {
                       itb = (Item_Block *)(sd->blocks);
                       if (itb->count >= sd->max_items_per_block)
                         {
                            itb = _item_block_new(sd, EINA_TRUE);
                            if (!itb) return EINA_FALSE;
                         }
                    }
                  else
                    {
                       itb = _item_block_new(sd, EINA_TRUE);
                       if (!itb) return EINA_FALSE;
                    }
                  itb->items = eina_list_prepend(itb->items, it);
                  _item_position_update(itb->items, 1);
               }
             else
               {
                  if (sd->blocks)
                    {
                       itb = (Item_Block *)(sd->blocks->last);
                       if (itb->count >= sd->max_items_per_block)
                         {
                            itb = _item_block_new(sd, EINA_FALSE);
                            if (!itb) return EINA_FALSE;
                         }
                    }
                  else
                    {
                       itb = _item_block_new(sd, EINA_FALSE);
                       if (!itb) return EINA_FALSE;
                    }
                  itb->items = eina_list_append(itb->items, it);
                  it->position = eina_list_count(itb->items);
               }
          }
     }
   else
     {
        Eina_List *tmp;

#if 0
        if ((!GL_IT(it)->wsd->sorting) && (GL_IT(it)->rel->item->queued))
          {
             /* NOTE: for a strange reason eina_list and eina_inlist
                don't have the same property on sorted insertion
                order, so the queue is not always ordered like the
                item list.  This lead to issue where we depend on an
                item that is not yet created. As a quick work around,
                we reschedule the calc of the item and stop reordering
                the list to prevent any nasty issue to show up here.
              */
             sd->queue = eina_list_append(sd->queue, it);
             sd->requeued = EINA_TRUE;
             GL_IT(it)->queued = EINA_TRUE;

             return EINA_FALSE;
          }
#endif
        itb = GL_IT(it)->rel->item->block;
        if (!itb) goto newblock;
        tmp = eina_list_data_find_list(itb->items, GL_IT(it)->rel);
        if (GL_IT(it)->before)
          {
             itb->items = eina_list_prepend_relative_list(itb->items, it, tmp);
             _item_position_update
               (eina_list_prev(tmp), GL_IT(it)->rel->position);
          }
        else
          {
             itb->items = eina_list_append_relative_list(itb->items, it, tmp);
             _item_position_update
               (eina_list_next(tmp), GL_IT(it)->rel->position + 1);
          }
     }

   itb->count++;
   itb->calc_done = EINA_FALSE;
   sd->calc_done = EINA_FALSE;
   GL_IT(it)->block = itb;
   _changed(itb->sd->pan_obj);

   if (itb->count > itb->sd->max_items_per_block)
     {
        int newc;
        Item_Block *itb2;
        Elm_Gen_Item *it2;
        Eina_Bool done = EINA_FALSE;

        newc = itb->count / 2;

        if (EINA_INLIST_GET(itb)->prev)
          {
             Item_Block *itbp = (Item_Block *)(EINA_INLIST_GET(itb)->prev);

             if (itbp->count + newc < sd->max_items_per_block / 2)
               {
                  /* moving items to previous block */
                  while ((itb->count > newc) && (itb->items))
                    {
                       it2 = eina_list_data_get(itb->items);
                       itb->items = eina_list_remove_list
                           (itb->items, itb->items);
                       itb->count--;

                       itbp->items = eina_list_append(itbp->items, it2);
                       it2->item->block = itbp;
                       itbp->count++;
                    }

                  done = EINA_TRUE;
               }
          }

        if (!done && EINA_INLIST_GET(itb)->next)
          {
             Item_Block *itbn = (Item_Block *)(EINA_INLIST_GET(itb)->next);

             if (itbn->count + newc < sd->max_items_per_block / 2)
               {
                  /* moving items to next block */
                  while ((itb->count > newc) && (itb->items))
                    {
                       Eina_List *l;

                       l = eina_list_last(itb->items);
                       it2 = eina_list_data_get(l);
                       itb->items = eina_list_remove_list(itb->items, l);
                       itb->count--;

                       itbn->items = eina_list_prepend(itbn->items, it2);
                       it2->item->block = itbn;
                       itbn->count++;
                    }

                  done = EINA_TRUE;
               }
          }

        if (!done)
          {
             /* moving items to new block */
             itb2 = calloc(1, sizeof(Item_Block));
             if (!itb2) return EINA_FALSE;
             itb2->sd = sd;
             sd->blocks =
               eina_inlist_append_relative(sd->blocks, EINA_INLIST_GET(itb2),
                                           EINA_INLIST_GET(itb));
             itb2->calc_done = EINA_FALSE;
             while ((itb->count > newc) && (itb->items))
               {
                  Eina_List *l;

                  l = eina_list_last(itb->items);
                  it2 = l->data;
                  itb->items = eina_list_remove_list(itb->items, l);
                  itb->count--;

                  itb2->items = eina_list_prepend(itb2->items, it2);
                  it2->item->block = itb2;
                  itb2->count++;
               }
          }
     }

   return EINA_TRUE;
}

static void
_item_min_calc(Elm_Gen_Item *it)
{
   Elm_Genlist_Data *sd = GL_IT(it)->wsd;
   ELM_WIDGET_DATA_GET_OR_RETURN(WIDGET(it), wd);
   Evas_Coord mw = 0, mh = 0;
   Evas_Coord vw = 0;

   elm_interface_scrollable_content_viewport_geometry_get
         (sd->obj, NULL, NULL, &vw, NULL);

   if ((it->select_mode != ELM_OBJECT_SELECT_MODE_DISPLAY_ONLY) &&
       (sd->select_mode != ELM_OBJECT_SELECT_MODE_DISPLAY_ONLY))
     {
        mw = sd->finger_minw;
        mh = sd->finger_minh;
     }

   if ((sd->mode == ELM_LIST_COMPRESS) && vw && mw < vw)
     mw = vw;

   //FIXME: Some widget such as entry, label need to have fixed width size before
   // min calculation to get proper height size by multiline.
   if (sd->realization_mode || GL_IT(it)->resized)
     evas_object_resize(VIEW(it), mw, mh);

   edje_object_size_min_restricted_calc(VIEW(it), &mw, &mh, mw, mh);

   if ((sd->mode != ELM_LIST_LIMIT) && vw && mw < vw)
     mw = vw;

   // TIZEN ONLY(20160630): Support homogeneous mode in item class.
   if (sd->homogeneous || it->itc->homogeneous)
     {
        Item_Size *size, *tmp;
        tmp = eina_hash_find(sd->size_caches, it->itc->item_style);
        if (tmp)
           eina_hash_del_by_key(sd->size_caches, it->itc->item_style);

        size = ELM_NEW(Item_Size);
        size->minw = mw;
        size->minh = mh;
        eina_hash_add(sd->size_caches, it->itc->item_style, size);
     }

   GL_IT(it)->w = GL_IT(it)->minw = mw;
   GL_IT(it)->h = GL_IT(it)->minh = mh;

   // FIXME: This is workaround for entry size issue.
   if (sd->realization_mode || GL_IT(it)->resized)
     {
        GL_IT(it)->resized = EINA_FALSE;
        if (it->deco_all_view)
          evas_object_resize(it->deco_all_view, GL_IT(it)->w, GL_IT(it)->h);
        else if (GL_IT(it)->deco_it_view)
          evas_object_resize(GL_IT(it)->deco_it_view, GL_IT(it)->w, GL_IT(it)->h);
        else
          evas_object_resize(VIEW(it), GL_IT(it)->w, GL_IT(it)->h);
     }
   //
   if (wd->scroll_item_align_enable && (sd->aligned_item == it))
     _focus_bg_show(sd->aligned_item);
}

static void
_item_calc(Elm_Gen_Item *it)
{
   Elm_Genlist_Data *sd = GL_IT(it)->wsd;
   Item_Size *size = NULL;
   Evas_Coord p_minw, p_minh;

   if (GL_IT(it)->calc_done) return;

   p_minw = GL_IT(it)->minw;
   p_minh = GL_IT(it)->minh;

   // TIZEN ONLY(20160630): Support homogeneous mode in item class.
   if (sd->homogeneous || it->itc->homogeneous)
     size = eina_hash_find(sd->size_caches, it->itc->item_style);
   if (size)
     {
        GL_IT(it)->w = GL_IT(it)->minw = size->minw;
        GL_IT(it)->h = GL_IT(it)->minh = size->minh;
     }
   else
     {
        if (!it->realized)
          {
             if (sd->realization_mode)
               {
                  _item_realize(it, EINA_FALSE);
                  _item_min_calc(it);
               }
             else
               {
                  _item_realize(it, EINA_TRUE);
                  _item_min_calc(it);
                  _item_unrealize(it, EINA_TRUE);
               }
          }
        else
           _item_min_calc(it);
     }

   GL_IT(it)->calc_done = EINA_TRUE;
   if ((p_minw != GL_IT(it)->minw) || (p_minh != GL_IT(it)->minh))
     {
        GL_IT(it)->block->calc_done = EINA_FALSE;
        sd->calc_done = EINA_FALSE;
     }
}

static Eina_Bool
_item_process(Elm_Genlist_Data *sd,
              Elm_Gen_Item *it)
{
   if (!_item_block_add(sd, it)) return EINA_FALSE;

   GL_IT(it)->calc_done = EINA_FALSE;
   _item_calc(it);

   return EINA_TRUE;
}

static void
_dummy_job(void *data)
{
   Elm_Genlist_Data *sd = data;
   sd->dummy_job = NULL;
   return;
}

static Eina_Bool
_queue_idle_enter(void *data)
{
   Elm_Genlist_Data *sd = data;
   Evas_Coord vw = 0;
   int n;
   double ft, t0;

   elm_interface_scrollable_content_viewport_geometry_get
        (sd->obj, NULL, NULL, &vw, NULL);
   if (!sd->queue || (vw <= 1))
     {
        if (sd->dummy_job)
          {
             ecore_job_del(sd->dummy_job);
             sd->dummy_job = NULL;
          }
        sd->queue_idle_enterer = NULL;
        return ECORE_CALLBACK_CANCEL;
     }

   ft = ecore_animator_frametime_get()/2;
   t0 = ecore_time_get();
   for (n = 0; (sd->queue) && (n < ITEM_QUEUE_MAX); n++)
     {
        Elm_Gen_Item *it;
        double t;

        it = eina_list_data_get(sd->queue);
        sd->queue = eina_list_remove_list(sd->queue, sd->queue);
        GL_IT(it)->queued = EINA_FALSE;
        GL_IT(it)->resized = EINA_FALSE;
        if (!_item_process(sd, it)) continue;
        t = ecore_time_get();
        /* same as eina_inlist_count > 1 */
        if (sd->blocks && sd->blocks->next)
          {
             if ((t - t0) > ft) break;
          }
     }

   _changed(sd->pan_obj);
   if (!sd->queue)
     {
        if (sd->dummy_job)
          {
             ecore_job_del(sd->dummy_job);
             sd->dummy_job = NULL;
          }
        sd->queue_idle_enterer = NULL;
        return ECORE_CALLBACK_CANCEL;
     }

   // Do not use smart_changed
   // Instead make any events (job, idler, etc.) to call idle enterer
   if (sd->dummy_job) ecore_job_del(sd->dummy_job);
   sd->dummy_job = ecore_job_add(_dummy_job, sd);

   return ECORE_CALLBACK_RENEW;
}

static void
_item_queue(Elm_Gen_Item *it,
            Eina_Compare_Cb cb EINA_UNUSED)
{
   if (GL_IT(it)->queued) return;

   GL_IT(it)->queued = EINA_TRUE;
   Elm_Genlist_Data *sd = GL_IT(it)->wsd;

// FIXME: Below code occurs item unsorted result.
// genlist already calculate items position by sd->items
// so no need to requeue items by sorting insert.
//   if (cb && !sd->requeued)
//     sd->queue = eina_list_sorted_insert(sd->queue, cb, it);
//   else
     sd->queue = eina_list_append(sd->queue, it);

   if (sd->queue_idle_enterer)
      ecore_idle_enterer_del(sd->queue_idle_enterer);
   sd->queue_idle_enterer = ecore_idle_enterer_add(_queue_idle_enter, sd);
}

static void
_item_queue_direct(Elm_Gen_Item *it,
                   Eina_Compare_Cb cb)
{
   Elm_Genlist_Data *sd = GL_IT(it)->wsd;

   // Processing items within viewport if items already exist.
   // This can prevent flickering issue when content size is changed.
   // This can fix the flickering issue when expanded item have subitems whose total height > vh
   if (!sd->queue && (sd->viewport_w > 1) &&
       ((sd->processed_sizes <= sd->viewport_h) || (GL_IT(it)->expanded_depth > 0)))
     {
#ifdef GENLIST_FX_SUPPORT
        if (sd->fx_mode)// && !it->item->is_prepend) // For support preppended items - hs619.choi@samsung.com
          {
              sd->add_fx.items = eina_list_append(sd->add_fx.items, it);
              if (!sd->add_fx.anim)
                {
                   sd->add_fx.cnt = ANIM_CNT_MAX;
                   sd->add_fx.anim = ecore_animator_add(_add_fx_anim, sd);
                }
           }
#endif
        _item_process(sd, it);
        sd->processed_sizes += GL_IT(it)->minh;

        _changed(sd->pan_obj);
        return;
     }
   _item_queue(it, cb);
}

/* If the application wants to know the relative item, use
 * elm_genlist_item_prev_get(it)*/
static void
_item_move_after(Elm_Gen_Item *it,
                 Elm_Gen_Item *after)
{
   if (!it) return;
   if (!after) return;
   if (it == after) return;

   GL_IT(it)->wsd->items =
     eina_inlist_remove(GL_IT(it)->wsd->items, EINA_INLIST_GET(it));
   if (GL_IT(it)->block) _item_block_del(it);

   GL_IT(it)->wsd->items = eina_inlist_append_relative
      (GL_IT(it)->wsd->items, EINA_INLIST_GET(it), EINA_INLIST_GET(after));

   if (GL_IT(it)->rel)
     GL_IT(it)->rel->item->rel_revs =
        eina_list_remove(GL_IT(it)->rel->item->rel_revs, it);
   GL_IT(it)->rel = after;
   after->item->rel_revs = eina_list_append(after->item->rel_revs, it);
   GL_IT(it)->before = EINA_FALSE;
   if (after->item->group_item) GL_IT(it)->group_item = after->item->group_item;

   if (GL_IT(it)->queued)
     {
        GL_IT(it)->wsd->queue = eina_list_remove(GL_IT(it)->wsd->queue, it);
        GL_IT(it)->queued = EINA_FALSE;
     }
   _item_queue_direct(it, NULL);

   evas_object_smart_callback_call(WIDGET(it), SIG_MOVED_AFTER, EO_OBJ(it));
}

static void
_access_activate_cb(void *data EINA_UNUSED,
                    Evas_Object *part_obj EINA_UNUSED,
                    Elm_Object_Item *eo_it)
{
   ELM_GENLIST_ITEM_DATA_GET(eo_it, it);
   if (!it) return;

   ELM_GENLIST_DATA_GET_FROM_ITEM(it, sd);
   if (!sd) return;

   if (sd->multi)
     {
        if (!it->selected)
          {
             _item_highlight(it);
             _item_select(it);
          }
        else
          _item_unselect(it);
     }
   else
     {
        if (!it->selected)
          {
             while (sd->selected)
               {
                  Elm_Object_Item *eo_sel = sd->selected->data;
                  Elm_Gen_Item *sel = efl_data_scope_get(eo_sel, ELM_GENLIST_ITEM_CLASS);
                  _item_unselect(sel);
               }
          }
        else
          {
             const Eina_List *l, *l_next;
             Elm_Object_Item *eo_it2;

             EINA_LIST_FOREACH_SAFE(sd->selected, l, l_next, eo_it2)
               {
                  ELM_GENLIST_ITEM_DATA_GET(eo_it2, it2);
                  if (it2 != it)
                    _item_unselect(it2);
               }
          }
        _item_highlight(it);
        _item_select(it);
     }
}

/* If the application wants to know the relative item, use
 * elm_genlist_item_next_get(it)*/
static void
_item_move_before(Elm_Gen_Item *it,
                  Elm_Gen_Item *before)
{
   if (!it) return;
   if (!before) return;
   if (it == before) return;

   GL_IT(it)->wsd->items =
     eina_inlist_remove(GL_IT(it)->wsd->items, EINA_INLIST_GET(it));
   if (GL_IT(it)->block) _item_block_del(it);
   GL_IT(it)->wsd->items = eina_inlist_prepend_relative
       (GL_IT(it)->wsd->items, EINA_INLIST_GET(it), EINA_INLIST_GET(before));

   if (GL_IT(it)->rel)
     GL_IT(it)->rel->item->rel_revs =
        eina_list_remove(GL_IT(it)->rel->item->rel_revs, it);
   GL_IT(it)->rel = before;
   before->item->rel_revs = eina_list_append(before->item->rel_revs, it);
   GL_IT(it)->before = EINA_TRUE;
   if (before->item->group_item)
     GL_IT(it)->group_item = before->item->group_item;
   _item_queue_direct(it, NULL);

   evas_object_smart_callback_call(WIDGET(it), SIG_MOVED_BEFORE, EO_OBJ(it));
}

static void
_item_mouse_up_cb(void *data,
                  Evas *evas,
                  Evas_Object *obj EINA_UNUSED,
                  void *event_info)
{
   Evas_Event_Mouse_Up *ev = event_info;
   Elm_Gen_Item *it = data;
   Elm_Genlist_Data *sd = GL_IT(it)->wsd;

   ELM_WIDGET_DATA_GET_OR_RETURN(sd->obj, wd);

   if (!it->down) return;
   it->down = EINA_FALSE;

   if (ev->button != 1) return;

   if (it->dragging)
     {
        it->dragging = EINA_FALSE;
        evas_object_smart_callback_call(WIDGET(it), SIG_DRAG_STOP, EO_OBJ(it));
     }
   // When the event is cancelled, need to initialize mouse flags and timers.
   if (ev->event_flags & EVAS_EVENT_FLAG_ON_HOLD) sd->on_hold = EINA_TRUE;

   _gesture_do(sd);

   if (evas_event_down_count_get(evas) != 0)
     {
        if (!GL_IT(it)->highlight_timer) _item_unhighlight(it, EINA_TRUE);
     }

   if (it->long_timer)
     {
        ecore_timer_del(it->long_timer);
        it->long_timer = NULL;
     }
   if (GL_IT(it)->highlight_timer)
     {
        ecore_timer_del(GL_IT(it)->highlight_timer);
        GL_IT(it)->highlight_timer = NULL;
        // Because naviframe can drop the all evevents.
        // highlight it before select timer is called.
        if (evas_event_down_count_get(evas) == 0) _item_highlight(it);
     }

   if (!sd->reorder.it && (evas_event_down_count_get(evas) == 0))
     {
        // FIXME: if highlight mode is not used, mouse move cannot disable
        // _item_select
        if ((sd->was_selected == it->selected) && (!sd->on_hold))
          {
             if (wd->scroll_item_align_enable)
               {
                  const char *align = edje_object_data_get(VIEW(it), "align");
                  if (!align || strcmp(align, "off"))
                    {
                       if (it != sd->aligned_item)
                         {
                            elm_genlist_item_bring_in(EO_OBJ(it), ELM_GENLIST_ITEM_SCROLLTO_MIDDLE);
                         }
                    }
               }
             if (((sd->highlight && it->highlighted) || !sd->highlight) &&
                 ((!wd->scroll_item_align_enable) || (it == sd->aligned_item)))
               GL_IT(it)->highlight_timer =
                  ecore_timer_add(ITEM_SELECT_TIMER, _select_timer, it);
             else if ((it->select_mode == ELM_OBJECT_SELECT_MODE_DISPLAY_ONLY) ||
                      (it->select_mode == ELM_OBJECT_SELECT_MODE_NONE))
               {
                  Eina_List *l;
                  Evas_Object *content;
                  if (sd->focused_item != EO_OBJ(it))
                    elm_object_item_focus_set(EO_OBJ(it), EINA_TRUE);

                  sd->focused_content = NULL;
                  EINA_LIST_FOREACH(it->contents, l, content)
                    {
                       if (elm_object_focused_object_get(content))
                         sd->focused_content = content;
                    }
               }
          }
     }
   else if (sd->reorder.it == it)
     {
        if (TIZEN_PROFILE_WEARABLE)
          {
             Elm_Gen_Item *it2, *it_max = NULL, *it_min = NULL;
             Evas_Coord r_y_scrl, it_y_max = -99999999, it_y_min = 99999999;

             if (!it->selected) _item_unhighlight(it, EINA_TRUE);
             r_y_scrl = GL_IT(it)->scrl_y;
             EINA_LIST_FREE(sd->reorder.move_items, it2)
               {
                  if (sd->reorder.it->parent != it2->parent)
                    {
                       it2->item->reorder_offset = 0;
                       continue;
                    }
                  Evas_Coord it_y = it2->item->scrl_y +
                     it2->item->reorder_offset + (it2->item->h / 2) +
                     it2->item->block->y;

                  if ((it_y < r_y_scrl) &&
                      (it_y_max < it_y))
                    {
                       it_max = it2;
                       it_y_max = it_y;
                    }
                  else if ((it_y > r_y_scrl) &&
                           (it_y_min > it_y))
                    {
                       it_min = it2;
                       it_y_min = it_y;
                    }
                  it2->item->reorder_offset = 0;
               }
             if (it_max)
               {
                  _item_move_after(it, it_max);
                  evas_object_smart_callback_call(WIDGET(it), SIG_MOVED, EO_OBJ(it));
               }
             else if (it_min)
               {
                  _item_move_before(it, it_min);
                  evas_object_smart_callback_call(WIDGET(it), SIG_MOVED, EO_OBJ(it));
               }
          }
        else
          {
             //Kiran only
             Elm_Gen_Item *moved_it = NULL;
             Elm_Gen_Item *ptr_it = sd->top_drawn_item;
             Elm_Gen_Item *last_it = NULL;
             Eina_Bool after = EINA_TRUE;

             if (!it->selected) _item_unhighlight(it, EINA_FALSE);

             while (ptr_it)
               {
                  if (it != ptr_it)
                    {
                       if (ELM_RECTS_INTERSECT(GL_IT(it)->scrl_x, GL_IT(it)->scrl_y, GL_IT(it)->w, GL_IT(it)->h,
                                            GL_IT(ptr_it)->scrl_x, GL_IT(ptr_it)->scrl_y, GL_IT(ptr_it)->w, GL_IT(ptr_it)->h))
                         {
                            if (GL_IT(it)->scrl_y < GL_IT(ptr_it)->scrl_y)
                              after = EINA_FALSE;
                            moved_it = ptr_it;
                            break;
                         }
                       else
                         {
                            if ((GL_IT(it)->scrl_y + GL_IT(it)->h) > (GL_IT(ptr_it)->scrl_y + GL_IT(ptr_it)->h))
                              moved_it = ptr_it;
                            if ((GL_IT(it)->scrl_y + GL_IT(it)->h) == GL_IT(ptr_it)->scrl_y)
                              {
                                 after = EINA_FALSE;
                                 moved_it = ptr_it;
                              }
                         }
                    }
                  last_it = ptr_it;
                  ptr_it = ELM_GEN_ITEM_FROM_INLIST(EINA_INLIST_GET(ptr_it)->next);
               }

             if (!moved_it)
               {
                  if (GL_IT(it)->scrl_y > GL_IT(last_it)->scrl_y + GL_IT(last_it)->h)
                        moved_it = last_it;
                  else
                    {
                       moved_it = sd->top_drawn_item;
                       after = EINA_FALSE;
                    }
               }

             EINA_LIST_FREE(sd->reorder.move_items, ptr_it)
               {
                  ptr_it->item->reorder_offset = 0;
               }

             if (after)
               {
                  _item_move_after(it, moved_it);
                  evas_object_smart_callback_call(WIDGET(it), SIG_MOVED, EO_OBJ(it));
               }
             else
               {
                 _item_move_before(it, moved_it);
                 evas_object_smart_callback_call(WIDGET(it), SIG_MOVED, EO_OBJ(it));
               }
          }
        sd->reorder.it = NULL;
        sd->reorder.dir = 0;
        if (sd->reorder.anim)
          {
             ecore_animator_del(sd->reorder.anim);
             sd->reorder.anim = NULL;
          }

        elm_interface_scrollable_hold_set(sd->obj, EINA_FALSE);
        elm_interface_scrollable_bounce_allow_set
              (sd->obj, sd->h_bounce, sd->v_bounce);

        edje_object_signal_emit(VIEW(it), SIGNAL_REORDER_DISABLED, "elm");
        _changed(sd->pan_obj);
     }
   sd->on_hold = EINA_FALSE; /* for checking need to start select timer */
   evas_object_smart_callback_call(WIDGET(it), SIG_RELEASED, EO_OBJ(it));
}

static Eina_Bool
_scroll_hold_timer_cb(void *data)
{
   Elm_Genlist_Data *sd = data;
   sd->scr_hold_timer = NULL;

   elm_interface_scrollable_hold_set(sd->obj, EINA_FALSE);

   return ECORE_CALLBACK_CANCEL;
}

static void
_decorate_item_finished_signal_cb(void *data,
                                  Evas_Object *obj,
                                  const char *emission EINA_UNUSED,
                                  const char *source EINA_UNUSED)
{
   Elm_Gen_Item *it = data;

   if (!data || !obj) return;
   if ((!it->realized)) return;

   _decorate_item_unrealize(it);
}

static void
_item_update(Elm_Gen_Item *it)
{
   Evas_Object *c;
   if (!it->realized) return;

   _view_clear(VIEW(it), &(it->contents));
   EINA_LIST_FREE(GL_IT(it)->flip_content_objs, c)
     evas_object_del(c);
   _view_clear(GL_IT(it)->deco_it_view, &(GL_IT(it)->deco_it_contents));
   _view_clear(it->deco_all_view, &(GL_IT(it)->deco_all_contents));

   _view_inflate(VIEW(it), it, &(it->contents));
   if (it->flipped)
     {
        _item_content_realize(it, VIEW(it), &(GL_IT(it)->flip_content_objs),
                               "flips", NULL);
        edje_object_signal_emit(VIEW(it), SIGNAL_FLIP_ENABLED, "elm");
     }
   if (GL_IT(it)->wsd->decorate_all_mode)
     _view_inflate(it->deco_all_view, it, &(GL_IT(it)->deco_all_contents));
   else if (GL_IT(it)->deco_it_view)
     _view_inflate(GL_IT(it)->deco_it_view, it, &(GL_IT(it)->deco_it_contents));

   if (!TIZEN_PROFILE_WEARABLE)
     _banded_item_bg_add(it, VIEW(it));

   if (it->selected)
     efl_event_callback_legacy_call(WIDGET(it), ELM_GENLIST_EVENT_HIGHLIGHTED, EO_OBJ(it));

   if (GL_IT(it)->wsd->aligned_item == it)
     edje_object_signal_emit(VIEW(it), SIGNAL_ITEM_HIGHLIGHTED, "elm");

   edje_object_message_signal_process(VIEW(it));
}

static void
_item_block_calc(Item_Block *itb, Evas_Coord vw EINA_UNUSED, Evas_Coord vh)
{
   const Eina_List *l;
   Elm_Gen_Item *it;
   Evas_Coord minw = 9999999, minh = 0;
   int sum_item_height = 0;
   ELM_WIDGET_DATA_GET_OR_RETURN(itb->sd->obj, wsd);

   if (itb->calc_done) return;

   EINA_LIST_FOREACH(itb->items, l, it)
     {
        if (it->hide) continue;
        if (GL_IT(it)->updateme)
          {
             if (it->realized)
               {
                  _item_update(it);
               }
             GL_IT(it)->calc_done = EINA_FALSE;
             GL_IT(it)->updateme = EINA_FALSE;
          }
        if (!GL_IT(it)->queued) _item_calc(it);

        if (wsd->scroll_item_align_enable)
          {
             int vcenter = vh / 2;
             int icenter = GL_IT(it)->h / 2;
             if ((itb->y + sum_item_height < vcenter && itb->y + sum_item_height + GL_IT(it)->h > vcenter) &&
                 (sum_item_height + icenter < vcenter - 1 || sum_item_height + icenter > vcenter + 1))
               {
                  int first_item_height = sum_item_height = vcenter - icenter;

                  Elm_Object_Item *eo_prev = elm_genlist_item_prev_get(EO_OBJ(it));
                  Elm_Gen_Item *prev = efl_data_scope_get(eo_prev, ELM_GENLIST_ITEM_CLASS);

                  if (prev)
                    minh = sum_item_height;
                  while (prev)
                    {
                       if (eo_prev == elm_genlist_first_item_get(wsd->obj))
                         {
                            GL_IT(prev)->h = GL_IT(prev)->minh = first_item_height;
                            break;
                         }
                       first_item_height -= GL_IT(prev)->h;
                       prev->y = first_item_height;
                       eo_prev = elm_genlist_item_prev_get(eo_prev);
                       prev = efl_data_scope_get(eo_prev, ELM_GENLIST_ITEM_CLASS);
                    }
               }
             sum_item_height += GL_IT(it)->h;
          }

        it->x = 0;
        it->y = minh;
        if (minw > GL_IT(it)->minw) minw = GL_IT(it)->minw;
        minh += GL_IT(it)->minh;
        if (GL_IT(it)->is_prepend)
          {
             itb->sd->comp_y += GL_IT(it)->minh;
             GL_IT(it)->is_prepend = EINA_FALSE;
          }
     }
   itb->minw = minw;
   itb->minh = minh;
   itb->calc_done = EINA_TRUE;
   itb->position_update = EINA_FALSE;
}

static void
_scroll_animate_start_cb(Evas_Object *obj,
                         void *data EINA_UNUSED)
{
   ELM_WIDGET_DATA_GET_OR_RETURN(obj, wd);
   evas_object_smart_callback_call(obj, SIG_SCROLL_ANIM_START, NULL);
   if (wd->scroll_item_align_enable)
     {
        edje_object_signal_emit(wd->resize_obj, SIGNAL_FOCUS_BG_HIDE, "elm");
     }
}

static void
_scroll_animate_stop_cb(Evas_Object *obj,
                        void *data EINA_UNUSED)
{
   ELM_WIDGET_DATA_GET_OR_RETURN(obj, wd);
   evas_object_smart_callback_call(obj, SIG_SCROLL_ANIM_STOP, NULL);

   //TIZEN_ONLY: Let the screen-reader know when scroll animation ends
   if (_elm_atspi_enabled())
     {
        efl_access_visible_data_changed_signal_emit(obj);
     }
   //
}

static void
_scroll_drag_start_cb(Evas_Object *obj,
                      void *data EINA_UNUSED)
{
   ELM_WIDGET_DATA_GET_OR_RETURN(obj, wd);
   evas_object_smart_callback_call(obj, SIG_SCROLL_DRAG_START, NULL);
   if (wd->scroll_item_align_enable)
     {
        edje_object_signal_emit(wd->resize_obj, SIGNAL_FOCUS_BG_HIDE, "elm");
     }
}

static Eina_Bool
_scroll_timeout_cb(void *data)
{
   Elm_Genlist_Data *sd = data;

   sd->scr_timer = NULL;
   if (sd->queue && !sd->queue_idle_enterer)
     {
        sd->queue_idle_enterer = ecore_idle_enterer_add(_queue_idle_enter, sd);
        if (sd->dummy_job) ecore_job_del(sd->dummy_job);
        sd->dummy_job = ecore_job_add(_dummy_job, sd);
     }
   return ECORE_CALLBACK_CANCEL;
}

static void
_scroll_cb(Evas_Object *obj,
           void *data EINA_UNUSED)
{
   ELM_GENLIST_DATA_GET(obj, sd);

   if (sd->queue_idle_enterer)
     {
        ecore_idle_enterer_del(sd->queue_idle_enterer);
        sd->queue_idle_enterer = NULL;
        if (sd->dummy_job)
          {
             ecore_job_del(sd->dummy_job);
             sd->dummy_job = NULL;
          }
     }
   if (sd->scr_timer) ecore_timer_del(sd->scr_timer);
   sd->scr_timer = ecore_timer_add(0.25, _scroll_timeout_cb, sd);

   evas_object_smart_callback_call(obj, SIG_SCROLL, NULL);
}

static void
_scroll_drag_stop_cb(Evas_Object *obj,
                     void *data EINA_UNUSED)
{
   evas_object_smart_callback_call(obj, SIG_SCROLL_DRAG_STOP, NULL);
   //TIZEN_ONLY: Let the screen-reader know when drag ends
   if (_elm_atspi_enabled())
     {
        efl_access_visible_data_changed_signal_emit(obj);
     }
   //
}

static void
_edge_left_cb(Evas_Object *obj,
              void *data EINA_UNUSED)
{
   evas_object_smart_callback_call(obj, SIG_EDGE_LEFT, NULL);
}

static void
_edge_right_cb(Evas_Object *obj,
               void *data EINA_UNUSED)
{
   evas_object_smart_callback_call(obj, SIG_EDGE_RIGHT, NULL);
}

static void
_edge_top_cb(Evas_Object *obj,
             void *data EINA_UNUSED)
{
   evas_object_smart_callback_call(obj, SIG_EDGE_TOP, NULL);
}

static void
_edge_bottom_cb(Evas_Object *obj,
                void *data EINA_UNUSED)
{
   evas_object_smart_callback_call(obj, SIG_EDGE_BOTTOM, NULL);
}

static void
_vbar_drag_cb(Evas_Object *obj,
                void *data EINA_UNUSED)
{
   evas_object_smart_callback_call(obj, SIG_VBAR_DRAG, NULL);
}

static void
_vbar_press_cb(Evas_Object *obj,
                void *data EINA_UNUSED)
{
   evas_object_smart_callback_call(obj, SIG_VBAR_PRESS, NULL);
}

static void
_vbar_unpress_cb(Evas_Object *obj,
                void *data EINA_UNUSED)
{
   evas_object_smart_callback_call(obj, SIG_VBAR_UNPRESS, NULL);
}

static void
_hbar_drag_cb(Evas_Object *obj,
                void *data EINA_UNUSED)
{
   evas_object_smart_callback_call(obj, SIG_HBAR_DRAG, NULL);
}

static void
_hbar_press_cb(Evas_Object *obj,
                void *data EINA_UNUSED)
{
   evas_object_smart_callback_call(obj, SIG_HBAR_PRESS, NULL);
}

static void
_hbar_unpress_cb(Evas_Object *obj,
                void *data EINA_UNUSED)
{
   evas_object_smart_callback_call(obj, SIG_HBAR_UNPRESS, NULL);
}

static void
_decorate_item_realize(Elm_Gen_Item *it)
{
   char buf[1024];
   if (GL_IT(it)->deco_it_view || !it->itc->decorate_item_style) return;

   // Before adding & swallowing, delete it from smart member
   evas_object_smart_member_del(VIEW(it));

   GL_IT(it)->deco_it_view = _view_create(it, it->itc->decorate_item_style);
   edje_object_part_swallow
      (GL_IT(it)->deco_it_view,
       edje_object_data_get(GL_IT(it)->deco_it_view, "mode_part"), VIEW(it));
   _view_inflate(GL_IT(it)->deco_it_view, it, &(GL_IT(it)->deco_it_contents));

   snprintf(buf, sizeof(buf), "elm,state,%s,active",
            GL_IT(it)->wsd->decorate_it_type);
   edje_object_signal_emit(GL_IT(it)->deco_it_view, buf, "elm");
   edje_object_signal_emit(VIEW(it), buf, "elm");

   _item_mouse_callbacks_add(it, GL_IT(it)->deco_it_view);
   // Redwood Only
   _item_mouse_callbacks_del(it, VIEW(it));
}

#if 0
// FIXME: difference from upstream
static void
_mouse_down_scroller(void        *data,
                     Evas        *evas EINA_UNUSED,
                     Evas_Object *obj EINA_UNUSED,
                     void        *event_info EINA_UNUSED)
{
   Widget_Data *wd = elm_widget_data_get(data);

   if (!wd) return;
   wd->drag_started = EINA_FALSE;
}

static void
_mouse_up_scroller(void        *data,
                   Evas        *evas EINA_UNUSED,
                   Evas_Object *obj EINA_UNUSED,
                   void        *event_info EINA_UNUSED)
{
   Widget_Data *wd = elm_widget_data_get(data);

   if (!wd) return;
   wd->drag_started = EINA_FALSE;
}

static void
_mouse_move_scroller(void        *data,
                     Evas        *evas EINA_UNUSED,
                     Evas_Object *obj EINA_UNUSED,
                     void        *event_info)
{
   Widget_Data *wd = elm_widget_data_get(data);
   Evas_Event_Mouse_Move *ev = event_info;
   Evas_Coord minw = 0, minh = 0, dx, dy, adx, ady;

   if (!wd) return;
   if (wd->drag_started) return;

   elm_coords_finger_size_adjust(1, &minw, 1, &minh);
   dx = ev->cur.canvas.x - ev->prev.canvas.x;
   dy = ev->cur.canvas.y - ev->prev.canvas.y;
   adx = dx;
   ady = dy;
   if (adx < 0) adx = -dx;
   if (ady < 0) ady = -dy;
   if (((ady < minh) && (ady > minh / 2)) && (ady > adx))
     {
        if (dy < 0)
          {
             evas_object_smart_callback_call(data, SIG_DRAG_START_UP, NULL);
             wd->drag_started = EINA_TRUE;
          }
        else
          {
             evas_object_smart_callback_call(data, SIG_DRAG_START_DOWN, NULL);
             wd->drag_started = EINA_TRUE;
          }
     }
}
#endif

static void
_size_cache_free(void *data)
{
   if (data) free(data);
}

static Evas_Event_Flags
_pinch_zoom_start_cb(void *data, void *event_info)
{
   Elm_Genlist_Data *sd = data;
   Elm_Gesture_Zoom_Info *p = (Elm_Gesture_Zoom_Info *) event_info;
   Elm_Object_Item *eo_it;

   eo_it = elm_genlist_at_xy_item_get(sd->obj, p->x, p->y, NULL);
   sd->g_item = efl_data_scope_get(eo_it, ELM_GENLIST_ITEM_CLASS);
   return EVAS_EVENT_FLAG_NONE;
}

static Evas_Event_Flags
_pinch_zoom_cb(void *data, void *event_info)
{
   Elm_Genlist_Data *sd = data;
   Elm_Gesture_Zoom_Info *p = (Elm_Gesture_Zoom_Info *) event_info;

   if (p->zoom > 1.0 + PINCH_ZOOM_TOLERANCE)
     sd->g_type = SIG_MULTI_PINCH_OUT;
   else if (p->zoom < 1.0 - PINCH_ZOOM_TOLERANCE)
     sd->g_type = SIG_MULTI_PINCH_IN;

   return EVAS_EVENT_FLAG_NONE;
}

static Evas_Event_Flags
_gesture_n_lines_start_cb(void *data , void *event_info)
{
   Elm_Genlist_Data *sd = data;
   Elm_Gesture_Line_Info *p = (Elm_Gesture_Line_Info *) event_info;
   Evas_Coord x,y;
   Elm_Object_Item *eo_it;

   x = (p->momentum.x1 + p->momentum.x2) / 2;
   y = (p->momentum.y1 + p->momentum.y2) / 2;

   eo_it = elm_genlist_at_xy_item_get(sd->obj, x, y, NULL);
   sd->g_item = efl_data_scope_get(eo_it, ELM_GENLIST_ITEM_CLASS);
   return EVAS_EVENT_FLAG_NONE;
}

static Evas_Event_Flags
_gesture_n_lines_cb(void *data , void *event_info)
{
   Elm_Genlist_Data *sd = data;
   Elm_Gesture_Line_Info *p = (Elm_Gesture_Line_Info *) event_info;

   if (p->momentum.n < 2)
     return EVAS_EVENT_FLAG_NONE;

   Evas_Coord minw = 0, minh = 0;
   Evas_Coord x, y, off_x, off_y;
   Evas_Coord cur_x, cur_y, prev_x, prev_y;
   Elm_Gen_Item *down_it;
   Elm_Object_Item *eo_down_it;

   minw = sd->finger_minw;
   minh = sd->finger_minh;

   prev_x = prev_y = 0;

   cur_x = p->momentum.x1;
   cur_y = p->momentum.y1;

   eo_down_it = elm_genlist_at_xy_item_get(sd->obj, cur_x, cur_y, NULL);
   down_it = efl_data_scope_get(eo_down_it, ELM_GENLIST_ITEM_CLASS);
   if (down_it)
     {
        evas_object_geometry_get(VIEW(down_it), &x, &y, NULL, NULL);
        prev_x = down_it->dx + x;
        prev_y = down_it->dy + y;

        off_x = abs(cur_x - prev_x);
        off_y = abs(cur_y - prev_y);

        if ((off_x > minw) || (off_y > minh))
          {
             if (off_x > off_y)
               {
                  if (cur_x > prev_x)
                    sd->g_type = SIG_MULTI_SWIPE_RIGHT;
                  else
                    sd->g_type = SIG_MULTI_SWIPE_LEFT;
               }
             else
               {
                  if (cur_y > prev_y)
                    sd->g_type = SIG_MULTI_SWIPE_DOWN;
                  else
                    sd->g_type = SIG_MULTI_SWIPE_UP;
               }
          }
     }
   return EVAS_EVENT_FLAG_NONE;
}

static Evas_Event_Flags
_gesture_n_flicks_cb(void *data , void *event_info)
{
   Elm_Genlist_Data *sd = data;
   Elm_Gesture_Line_Info *p = (Elm_Gesture_Line_Info *) event_info;

   if (p->momentum.n == 1)
      sd->g_type = SIG_SWIPE;

   return EVAS_EVENT_FLAG_NONE;
}

static void
_evas_viewport_resize_cb(void *d, Evas *e EINA_UNUSED, void *ei EINA_UNUSED)
{
   Elm_Genlist_Data *priv = d;
   _changed(priv->pan_obj);
}

EOLIAN static void
_elm_genlist_efl_canvas_group_group_add(Eo *obj, Elm_Genlist_Data *priv)
{
   ELM_WIDGET_DATA_GET_OR_RETURN(obj, wd);
   Evas_Coord minw, minh;
   Elm_Genlist_Pan_Data *pan_data;

   efl_canvas_group_add(efl_super(obj, MY_CLASS));
   elm_widget_sub_object_parent_add(obj);

   priv->finger_minw = 0;
   priv->finger_minh = 0;
   elm_coords_finger_size_adjust(1, &priv->finger_minw, 1, &priv->finger_minh);

   priv->size_caches = eina_hash_string_small_new(_size_cache_free);
   priv->hit_rect = evas_object_rectangle_add(evas_object_evas_get(obj));
   evas_object_smart_member_add(priv->hit_rect, obj);
   elm_widget_sub_object_add(obj, priv->hit_rect);

   /* common scroller hit rectangle setup */
   evas_object_color_set(priv->hit_rect, 0, 0, 0, 0);
   evas_object_show(priv->hit_rect);
   evas_object_repeat_events_set(priv->hit_rect, EINA_TRUE);

   elm_widget_can_focus_set(obj, EINA_TRUE);
   elm_widget_on_show_region_hook_set(obj, NULL, _show_region_hook, NULL);

   if (!elm_layout_theme_set
       (obj, "genlist", "base", elm_widget_style_get(obj)))
     CRI("Failed to set layout!");

   if (!TIZEN_PROFILE_WEARABLE)
     _banded_bg_state_check(obj, priv);

       elm_interface_scrollable_objects_set(obj, wd->resize_obj, priv->hit_rect);

   elm_interface_scrollable_bounce_allow_set
         (obj, EINA_FALSE, _elm_config->thumbscroll_bounce_enable);
   priv->v_bounce = _elm_config->thumbscroll_bounce_enable;

   elm_interface_scrollable_animate_start_cb_set(obj, _scroll_animate_start_cb);
   elm_interface_scrollable_animate_stop_cb_set(obj, _scroll_animate_stop_cb);
   elm_interface_scrollable_scroll_cb_set(obj, _scroll_cb);
   elm_interface_scrollable_drag_start_cb_set(obj, _scroll_drag_start_cb);
   elm_interface_scrollable_drag_stop_cb_set(obj, _scroll_drag_stop_cb);
   elm_interface_scrollable_edge_left_cb_set(obj, _edge_left_cb);
   elm_interface_scrollable_edge_right_cb_set(obj, _edge_right_cb);
   elm_interface_scrollable_edge_top_cb_set(obj, _edge_top_cb);
   elm_interface_scrollable_edge_bottom_cb_set(obj, _edge_bottom_cb);
   elm_interface_scrollable_vbar_drag_cb_set(obj, _vbar_drag_cb);
   elm_interface_scrollable_vbar_press_cb_set(obj, _vbar_press_cb);
   elm_interface_scrollable_vbar_unpress_cb_set(obj, _vbar_unpress_cb);
   elm_interface_scrollable_hbar_drag_cb_set(obj, _hbar_drag_cb);
   elm_interface_scrollable_hbar_press_cb_set(obj, _hbar_press_cb);
   elm_interface_scrollable_hbar_unpress_cb_set(obj, _hbar_unpress_cb);
   elm_interface_scrollable_content_min_limit_cb_set(obj, _content_min_limit_cb);

   priv->mode = ELM_LIST_SCROLL;
   priv->max_items_per_block = MAX_ITEMS_PER_BLOCK;
   priv->item_cache_max = priv->max_items_per_block * 2;
   priv->longpress_timeout = _elm_config->longpress_timeout;
   priv->highlight = EINA_TRUE;
   priv->fx_mode = EINA_FALSE;
   priv->on_hold = EINA_FALSE;

   //Kiran only
   if (!TIZEN_PROFILE_WEARABLE)
     priv->top_drawn_item = NULL;

   priv->pan_obj = efl_add(MY_PAN_CLASS, evas_object_evas_get(obj));
   pan_data = efl_data_scope_get(priv->pan_obj, MY_PAN_CLASS);
   efl_data_ref(obj, NULL);
   pan_data->wobj = obj;
   pan_data->wsd = priv;

   elm_interface_scrollable_extern_pan_set(obj, priv->pan_obj);

   edje_object_size_min_calc(wd->resize_obj, &minw, &minh);
   evas_object_size_hint_min_set(obj, minw, minh);

   _item_cache_all_free(priv);
   _mirrored_set(obj, efl_ui_mirrored_get(obj));

   const char *str = edje_object_data_get(wd->resize_obj,
                                          "focus_highlight");
   if ((str) && (!strcmp(str, "on")))
      elm_widget_highlight_in_theme_set(obj, EINA_TRUE);
   else
      elm_widget_highlight_in_theme_set(obj, EINA_FALSE);
   priv->select_on_focus_enabled = EINA_FALSE;
   elm_widget_signal_callback_add(obj, "elm,action,focus_highlight,hide", "elm", _elm_genlist_focus_highlight_hide, obj);
   elm_widget_signal_callback_add(obj, "elm,action,focus_highlight,show", "elm", _elm_genlist_focus_highlight_show, obj);
   evas_event_callback_add(evas_object_evas_get(obj),
                           EVAS_CALLBACK_CANVAS_VIEWPORT_RESIZE,
                           _evas_viewport_resize_cb, priv);

   priv->g_layer = elm_gesture_layer_add(obj);
   if (!priv->g_layer) ERR("elm_gesture_layer_add() failed");
   elm_gesture_layer_attach(priv->g_layer, priv->hit_rect);
   elm_gesture_layer_cb_set
      (priv->g_layer, ELM_GESTURE_ZOOM, ELM_GESTURE_STATE_START,
       _pinch_zoom_start_cb, priv);
   elm_gesture_layer_cb_set
      (priv->g_layer, ELM_GESTURE_ZOOM, ELM_GESTURE_STATE_MOVE,
       _pinch_zoom_cb, priv);

   elm_gesture_layer_cb_set
      (priv->g_layer, ELM_GESTURE_N_LINES, ELM_GESTURE_STATE_START,
       _gesture_n_lines_start_cb, priv);
   elm_gesture_layer_cb_set
      (priv->g_layer, ELM_GESTURE_N_LINES, ELM_GESTURE_STATE_MOVE,
       _gesture_n_lines_cb, priv);

   elm_gesture_layer_cb_set
      (priv->g_layer, ELM_GESTURE_N_FLICKS, ELM_GESTURE_STATE_START,
       _gesture_n_lines_start_cb, priv);
   elm_gesture_layer_cb_set
      (priv->g_layer, ELM_GESTURE_N_FLICKS, ELM_GESTURE_STATE_MOVE,
       _gesture_n_flicks_cb, priv);

   elm_layout_sizing_eval(obj);

// TIZEN_ONLY(20150705): Genlist item align feature
   wd->scroll_item_align_enable = _elm_config->scroll_item_align_enable;
   wd->scroll_item_valign = _elm_config->scroll_item_valign;
//
   elm_interface_scrollable_content_viewport_geometry_get
          (obj, NULL, NULL, &priv->viewport_w, &priv->viewport_h);
}

EOLIAN static void
_elm_genlist_efl_canvas_group_group_del(Eo *obj, Elm_Genlist_Data *sd)
{
   sd->no_cache = EINA_TRUE;
   elm_genlist_clear(obj);
   evas_event_callback_del_full(evas_object_evas_get(obj),
                                EVAS_CALLBACK_CANVAS_VIEWPORT_RESIZE,
                                _evas_viewport_resize_cb, sd);
   if (sd->size_caches) eina_hash_free(sd->size_caches);
   if (sd->decorate_it_type) eina_stringshare_del(sd->decorate_it_type);
   if (sd->clipee) evas_object_del(sd->clipee);

   evas_object_del(sd->pan_obj);
   efl_canvas_group_del(efl_super(obj, MY_CLASS));
}

EOLIAN static void
_elm_genlist_efl_gfx_entity_position_set(Eo *obj, Elm_Genlist_Data *sd, Eina_Position2D pos)
{
   Evas_Coord ox, oy, bg_x, bg_y;

   if (!TIZEN_PROFILE_WEARABLE)
     {
        evas_object_geometry_get(obj, &ox, &oy, NULL, NULL);
        evas_object_geometry_get(sd->banded_bg_rect, &bg_x, &bg_y, NULL, NULL);
     }

   efl_gfx_entity_position_set(efl_super(obj, MY_CLASS), pos);
   efl_gfx_entity_position_set(sd->hit_rect, pos);

   if (!TIZEN_PROFILE_WEARABLE)
     efl_gfx_entity_position_set(sd->banded_bg_rect, EINA_POSITION2D((bg_x + pos.x - ox), (bg_y + pos.y - oy)));
}

EOLIAN static void
_elm_genlist_efl_gfx_entity_size_set(Eo *obj, Elm_Genlist_Data *sd, Eina_Size2D sz)

{
   efl_gfx_entity_size_set(efl_super(obj, MY_CLASS), sz);

   efl_gfx_entity_size_set(sd->hit_rect, sz);
}

EOLIAN static void
_elm_genlist_efl_canvas_group_group_member_add(Eo *obj, Elm_Genlist_Data *sd, Evas_Object *member)
{
   efl_canvas_group_member_add(efl_super(obj, MY_CLASS), member);

   if (sd->hit_rect)
     evas_object_raise(sd->hit_rect);
}

EOLIAN static void
_elm_genlist_efl_ui_widget_on_access_update(Eo *obj EINA_UNUSED, Elm_Genlist_Data *sd, Eina_Bool acs)
{
   _elm_genlist_smart_focus_next_enable = acs;
}

//TIZEN_ONLY(20160822): When atspi mode is dynamically switched on/off,
//register/unregister access objects accordingly.
// TIZEN_ONLY(20170516): connect to at-spi dbus based on org.a11y.Status.IsEnabled property
EOLIAN static void
_elm_genlist_efl_ui_widget_screen_reader(Eo *obj EINA_UNUSED, Elm_Genlist_Data *sd, Eina_Bool is_screen_reader)
{
   Item_Block *itb;
   Eina_Bool done = EINA_FALSE;
   Evas_Object *content = NULL;

   EINA_INLIST_FOREACH(sd->blocks, itb)
     {
        if (itb->realized)
          {
             Eina_List *l;
             Elm_Gen_Item *it;

             done = EINA_TRUE;
             EINA_LIST_FOREACH(itb->items, l, it)
               {
                  if (!it->realized || it->hide) continue;
                  if (is_screen_reader)
                    {
                       efl_access_added(EO_OBJ(it));
                       efl_access_children_changed_added_signal_emit(sd->obj, EO_OBJ(it));
                       EINA_LIST_FOREACH(it->contents, l, content)
                         {
                            if (efl_isa(content, EFL_ACCESS_OBJECT_MIXIN))
                              {
                                 efl_parent_set(content, EO_OBJ(it));
                                 efl_access_children_changed_added_signal_emit(EO_OBJ(it), content);
                              }
                         }
                       efl_access_i18n_name_changed_signal_emit(EO_OBJ(it));
                    }
                  else
                    {
                       efl_access_removed(EO_OBJ(it));
                       efl_access_children_changed_del_signal_emit(sd->obj, EO_OBJ(it));
                    }
               }
          }
        else if (done) break;
     }
   //TIZEN_ONLY(20161213): apply screen_reader_changed callback
   evas_object_smart_callback_call(obj, SIG_ATSPI_SCREEN_READER_CHANGED, &is_screen_reader);
   //
}
//
//

EAPI Evas_Object *
elm_genlist_add(Evas_Object *parent)
{
   EINA_SAFETY_ON_NULL_RETURN_VAL(parent, NULL);
   return elm_legacy_add(MY_CLASS, parent);
}

static void
_genlist_element_focused(void *data, const Efl_Event *ev)
{
   ELM_GENLIST_DATA_GET(data, pd);
   Efl_Ui_Widget *focused = efl_ui_focus_manager_focus_get(ev->object);
   Elm_Object_Item *item;

   if (!focused) return;

   if (efl_isa(focused, EFL_UI_FOCUS_COMPOSITION_ADAPTER_CLASS))
     item = efl_parent_get(focused);
   else
     item = efl_ui_focus_parent_provider_find_logical_parent(pd->provider, focused);

   if (efl_isa(item, ELM_GENLIST_ITEM_CLASS))
     {
        /*
        ELM_GENLIST_ITEM_DATA_GET(item, it);
        _item_focused(it, pd->focus_scrollto_type);
        _all_items_deselect(pd);
        elm_genlist_item_selected_set(item, EINA_TRUE);
        elm_genlist_item_bring_in(item, ELM_GENLIST_ITEM_SCROLLTO_MIDDLE);
        */
        elm_object_item_focus_set(item, EINA_TRUE);
     }
}

EOLIAN static Eo *
_elm_genlist_efl_object_constructor(Eo *obj, Elm_Genlist_Data *sd)
{
   obj = efl_constructor(efl_super(obj, MY_CLASS));

   sd->content_item_map = eina_hash_pointer_new(NULL);
   sd->provider = efl_add(EFL_UI_FOCUS_PARENT_PROVIDER_GEN_CLASS, obj,
    efl_ui_focus_parent_provider_gen_container_set(efl_added, obj),
    efl_ui_focus_parent_provider_gen_content_item_map_set(efl_added, sd->content_item_map));

   efl_ui_focus_composition_logical_mode_set(obj, EINA_TRUE);

   sd->obj = obj;
   efl_canvas_object_type_set(obj, MY_CLASS_NAME_LEGACY);
   evas_object_smart_callbacks_descriptions_set(obj, _smart_callbacks);
   efl_access_object_role_set(obj, EFL_ACCESS_ROLE_LIST);

   efl_event_callback_add(obj, EFL_UI_FOCUS_MANAGER_EVENT_MANAGER_FOCUS_CHANGED, _genlist_element_focused, obj);

   return obj;
}

EOLIAN static Eo *
_elm_genlist_efl_object_finalize(Eo *obj, Elm_Genlist_Data *sd)
{
   obj = efl_finalize(efl_super(obj, MY_CLASS));

   // FIXME:
   // Genlist is a legacy-only widget which means it will always be marked as
   // "legacy" here. The proper test here is "app targets EFL 1.19 or below".
   if (elm_widget_is_legacy(obj))
     sd->legacy_order_insane = EINA_TRUE;

   return obj;
}

static void
_internal_elm_genlist_clear(Evas_Object *obj)
{
   ELM_GENLIST_DATA_GET(obj, sd);
   Elm_Gen_Item *it;

   eina_hash_free_buckets(sd->size_caches);
   elm_object_item_focus_set(sd->focused_item, EINA_FALSE);
   if (sd->key_down_item) sd->key_down_item = NULL;
   if (sd->mode_item) sd->mode_item = NULL;

   ELM_SAFE_FREE(sd->state, eina_inlist_sorted_state_free);

   sd->filter_data = NULL;
   if (sd->filter_queue)
     ELM_SAFE_FREE(sd->queue_filter_enterer, ecore_idle_enterer_del);
   ELM_SAFE_FREE(sd->filter_queue, eina_list_free);
   ELM_SAFE_FREE(sd->filtered_list, eina_list_free);

   // Do not use EINA_INLIST_FOREACH or EINA_INLIST_FOREACH_SAFE
   // because sd->items can be modified inside elm_widget_item_del()
   while (sd->items)
     {
        it = EINA_INLIST_CONTAINER_GET(sd->items->last, Elm_Gen_Item);
        //elm_widget_item_del(it);
        //_item_free(it);
        efl_del(EO_OBJ(it));
     }
   sd->reorder.it = NULL;
   sd->reorder.dir = 0;
   eina_list_free(sd->reorder.move_items);

   sd->items = NULL;
   sd->blocks = NULL;
   _item_cache_all_free(sd);

   if (sd->selected)
     {
        sd->selected = eina_list_free(sd->selected);
        sd->selected = NULL;
     }

   sd->show_item = NULL;

   sd->pan_x = 0;
   sd->pan_y = 0;
   sd->minw = 0;
   sd->minh = 0;

   if (sd->pan_obj)
     {
        evas_object_size_hint_min_set(sd->pan_obj, sd->minw, sd->minh);
        efl_event_callback_legacy_call
              (sd->pan_obj, ELM_PAN_EVENT_CHANGED, NULL);
     }
   elm_layout_sizing_eval(obj);
   elm_interface_scrollable_content_region_show(obj, 0, 0, 0, 0);

   if (sd->dummy_job)
     {
        ecore_job_del(sd->dummy_job);
        sd->dummy_job = NULL;
     }
   if (sd->queue_idle_enterer)
     {
        ecore_idle_enterer_del(sd->queue_idle_enterer);
        sd->queue_idle_enterer = NULL;
     }
   if (sd->scr_hold_timer)
     {
        ecore_timer_del(sd->scr_hold_timer);
        sd->scr_hold_timer = NULL;
     }
   if (sd->reorder.anim)
     {
        ecore_animator_del(sd->reorder.anim);
        sd->reorder.anim = NULL;
     }
   if (sd->scr_timer)
     {
        ecore_timer_del(sd->scr_timer);
        sd->scr_timer = NULL;
     }
#ifdef GENLIST_FX_SUPPORT
   if (sd->del_fx.anim)
     {
        ecore_animator_del(sd->del_fx.anim);
        sd->del_fx.anim = NULL;
     }
   if (sd->add_fx.anim)
     {
        ecore_animator_del(sd->add_fx.anim);
        sd->add_fx.anim = NULL;
     }
#endif

   if (sd->queue) sd->queue = eina_list_free(sd->queue);
   if (sd->g_item) sd->g_item = NULL;
   if (sd->g_type) sd->g_type = NULL;

   if (!TIZEN_PROFILE_WEARABLE)
     {
        if (sd->banded_bg_rect)
          {
             evas_object_smart_member_del(sd->banded_bg_rect);
             ELM_SAFE_FREE(sd->banded_bg_rect, evas_object_del);
          }
     }
}

EOLIAN static Evas_Object *
_elm_genlist_item_elm_widget_item_part_content_get(const Eo *eo_it EINA_UNUSED, Elm_Gen_Item *it, const char * part)
{
   Evas_Object *ret = NULL;
   if (it->deco_all_view)
     ret = edje_object_part_swallow_get(it->deco_all_view, part);
   else if (it->decorate_it_set)
     ret = edje_object_part_swallow_get(GL_IT(it)->deco_it_view, part);
   if (!ret)
     ret = edje_object_part_swallow_get(VIEW(it), part);
   return ret;
}

EOLIAN static const char *
_elm_genlist_item_elm_widget_item_part_text_get(const Eo *eo_it EINA_UNUSED, Elm_Gen_Item *it, const char * part)
{
   if (!it->itc->func.text_get) return NULL;
   const char *ret = NULL;
   if (it->deco_all_view)
     ret = edje_object_part_text_get(it->deco_all_view, part);
   else if (it->decorate_it_set)
     ret = edje_object_part_text_get(GL_IT(it)->deco_it_view, part);
   if (!ret)
     ret = edje_object_part_text_get(VIEW(it), part);
   return ret;
}

EOLIAN static void
_elm_genlist_item_elm_widget_item_disable(Eo *eo_it, Elm_Gen_Item *it)
{
   Eina_List *l;
   Evas_Object *obj;

   _item_unselect(it);
   if (eo_it == GL_IT(it)->wsd->focused_item) elm_object_item_focus_set(EO_OBJ(it), EINA_FALSE);
   if (GL_IT(it)->highlight_timer)
     {
        ecore_timer_del(GL_IT(it)->highlight_timer);
        GL_IT(it)->highlight_timer = NULL;
     }
   if (it->long_timer)
     {
        ecore_timer_del(it->long_timer);
        it->long_timer = NULL;
     }

   if (it->realized)
     {
        if (elm_wdg_item_disabled_get(eo_it))
          {
             edje_object_signal_emit(VIEW(it), SIGNAL_DISABLED, "elm");
             if (it->deco_all_view)
               edje_object_signal_emit
                 (it->deco_all_view, SIGNAL_DISABLED, "elm");
          }
        else
          {
             edje_object_signal_emit(VIEW(it), SIGNAL_ENABLED, "elm");
             if (it->deco_all_view)
               edje_object_signal_emit
                 (it->deco_all_view, SIGNAL_ENABLED, "elm");
          }
        EINA_LIST_FOREACH(it->contents, l, obj)
          elm_widget_disabled_set(obj,elm_wdg_item_disabled_get(eo_it));
     }
}

static void
_item_free(Elm_Gen_Item *it)
{
   Eina_List *l, *ll;
   Elm_Object_Item *eo_it2;
   Elm_Genlist_Data *sd = GL_IT(it)->wsd;
   sd->processed_sizes -= GL_IT(it)->minh;
   if (sd->processed_sizes < 0) sd->processed_sizes = 0;

   elm_wdg_item_pre_notify_del(EO_OBJ(it));
   if (it->tooltip.del_cb)
     it->tooltip.del_cb((void *)it->tooltip.data, WIDGET(it), it);

#ifdef GENLIST_FX_SUPPORT
   sd->del_fx.items = eina_list_remove(sd->del_fx.items, it);
   sd->del_fx.pending_items = eina_list_remove(sd->del_fx.pending_items, it);
   sd->add_fx.items = eina_list_remove(sd->add_fx.items, it);
#endif

   if (GL_IT(it)->rel)
     GL_IT(it)->rel->item->rel_revs =
        eina_list_remove(GL_IT(it)->rel->item->rel_revs, it);
   if (GL_IT(it)->rel_revs)
     {
        Elm_Gen_Item *tmp;
        EINA_LIST_FREE(GL_IT(it)->rel_revs, tmp)
          {
             if (tmp->item->queued && !(tmp->base)->on_deletion)
               {
                  GL_IT(tmp)->queued = EINA_FALSE;
                  GL_IT(tmp)->resized = EINA_FALSE;
                  sd->queue = eina_list_remove(sd->queue, tmp);
                  _item_process(sd, tmp);
               }
             tmp->item->rel = NULL;
          }
     }
   EINA_LIST_FOREACH_SAFE(it->item->items, l, ll, eo_it2)
     {
        //elm_widget_item_del(it2);
        ELM_GENLIST_ITEM_DATA_GET(eo_it2, it2);
        efl_del(EO_OBJ(it2));
     }
   sd->reorder.move_items = eina_list_remove(sd->reorder.move_items, it);

   if (GL_IT(it)->block) _item_block_del(it);
   if (it->parent)
     it->parent->item->items =
        eina_list_remove(it->parent->item->items, EO_OBJ(it));
   if (GL_IT(it)->queued)
     {
        GL_IT(it)->queued = EINA_FALSE;
        sd->queue = eina_list_remove(sd->queue, it);
     }
   if (sd->filter_queue && !it->filtered)
     {
        l = eina_list_data_find_list(sd->filter_queue, it);
        if (l) sd->filter_queue = eina_list_remove_list(sd->filter_queue, l);
     }
   if (GL_IT(it)->type == ELM_GENLIST_ITEM_GROUP)
      sd->group_items = eina_list_remove(sd->group_items, it);

   if (it->selected)
      {
         sd->selected = eina_list_remove(sd->selected, EO_OBJ(it));
         it->selected = EINA_FALSE;
      }
   if (sd->show_item == it) sd->show_item = NULL;

   if ((sd->g_item) && (sd->g_item == it)) sd->g_item = NULL;
   if (sd->expanded_item == it) sd->expanded_item = NULL;
   if (sd->state) ELM_SAFE_FREE(sd->state, eina_inlist_sorted_state_free);

   if (sd->last_selected_item == EO_OBJ(it))
     sd->last_selected_item = NULL;

   if (sd->realization_mode)
     {
        Evas_Object *c;
        EINA_LIST_FOREACH(GL_IT(it)->flip_content_objs, l, c)
          {
             evas_object_event_callback_del_full(c,
                                                 EVAS_CALLBACK_CHANGED_SIZE_HINTS,
                                                 _changed_size_hints, it);
          }
        EINA_LIST_FOREACH(GL_IT(it)->deco_all_contents, l, c)
          {
             evas_object_event_callback_del_full(c,
                                                 EVAS_CALLBACK_CHANGED_SIZE_HINTS,
                                                 _changed_size_hints, it);
          }
        EINA_LIST_FOREACH(it->contents, l, c)
          {
             evas_object_event_callback_del_full(c,
                                                 EVAS_CALLBACK_CHANGED_SIZE_HINTS,
                                                 _changed_size_hints, it);
          }
     }

   if (sd->mode_item) sd->mode_item = NULL;
   if (it->selected) _item_unselect(it);
   if (EO_OBJ(it) == sd->focused_item)
     {
        Elm_Gen_Item *tmp;
        Eina_Bool find;
        if (!elm_object_focus_get(WIDGET(it))) goto failed;

        tmp = ELM_GEN_ITEM_FROM_INLIST(EINA_INLIST_GET(it)->next);
        find = _item_focusable_search(&tmp, 1);
        if (find)
          {
             elm_object_item_focus_set(EO_OBJ(tmp), EINA_TRUE);
             goto end;
          }
        else
          {
             tmp = ELM_GEN_ITEM_FROM_INLIST(EINA_INLIST_GET(it)->prev);
             find = _item_focusable_search(&tmp, -1);
             if (find)
               {
                  elm_object_item_focus_set(EO_OBJ(tmp), EINA_TRUE);
                  goto end;
               }
          }
failed:
        elm_object_item_focus_set(EO_OBJ(it), EINA_FALSE);
        sd->focused_item = NULL;
     }
end:
   if (it == sd->key_down_item) sd->key_down_item = NULL;
   if (it == sd->highlighted_item) sd->highlighted_item = NULL;
   if (!TIZEN_PROFILE_WEARABLE)
     {
        if (it == sd->top_drawn_item) sd->top_drawn_item = NULL;
     }

   _item_unrealize(it, EINA_FALSE);

   if (sd->aligned_item == it) sd->aligned_item = NULL;

   sd->items = eina_inlist_remove(sd->items, EINA_INLIST_GET(it));
   sd->item_count--;

   if (it->item->proxy)
     {
        evas_object_smart_member_del(it->item->proxy);
        evas_object_del(it->item->proxy);
     }

   if (it->itc->func.del)
     it->itc->func.del((void *)WIDGET_ITEM_DATA_GET(EO_OBJ(it)), WIDGET(it));

   if (it->itc->refcount <= 1)
     {
        Eina_Inlist *l2;
        Item_Cache *ic;
        EINA_INLIST_FOREACH_SAFE(sd->item_cache, l2, ic)
          {
             if (it->itc == ic->item_class)
               _item_cache_free(sd, ic);
          }
     }

   elm_genlist_item_class_unref((Elm_Genlist_Item_Class *)it->itc);
   free(GL_IT(it));
   GL_IT(it) = NULL;

// TIZEN_ONLY(20150703) : banded color background feature. enabled only un-scrollable
   if (!TIZEN_PROFILE_WEARABLE)
     {
        if (sd->banded_bg_rect && !sd->items)
          {
             evas_object_smart_member_del(sd->banded_bg_rect);
             ELM_SAFE_FREE(sd->banded_bg_rect, evas_object_del);
          }
     }

   _changed(sd->pan_obj);
}

EOLIAN static void
_elm_genlist_item_elm_widget_item_del_pre(Eo *eo_it EINA_UNUSED,
                                          Elm_Gen_Item *it)
{
#ifdef GENLIST_FX_SUPPORT
   Elm_Genlist_Data *sd = it->item->wsd;
   Evas_Coord cvx, cvy, cvw, cvh;

   if (!sd->fx_mode)
     {
#endif
        _item_free(it);
        return ;
#ifdef GENLIST_FX_SUPPORT
     }

   sd->add_fx.items = eina_list_remove(sd->add_fx.items, it);

   // Support FX Mode
   evas_output_viewport_get(evas_object_evas_get(sd->obj),
                            &cvx, &cvy, &cvw, &cvh);
   if (!ELM_RECTS_INTERSECT(GL_IT(it)->scrl_x, GL_IT(it)->scrl_y,
                           GL_IT(it)->w, GL_IT(it)->h,
                           cvx, cvy, cvw, cvh))
     {
        // Delete later, Above items in the viewport
        // Delete right now, below items in the viewport
        if (it->item->scrl_y < cvy)
           sd->del_fx.pending_items = eina_list_append(sd->del_fx.pending_items, it);
        else
          {
             _item_free(it);
             _changed(sd->pan_obj);
             return EINA_FALSE;
          }
     }
   else
     {
        if (it->deco_all_view) evas_object_lower(it->deco_all_view);
        else if (it->item->deco_it_view) evas_object_lower(it->item->deco_it_view);
        else if (VIEW(it)) evas_object_lower(VIEW(it));

        if (it->item->expanded_depth > 0)
          {
             Eina_List *l;
             Elm_Object_Item *itt;
             EINA_LIST_FOREACH(it->item->items, l, itt)
               {
                  //elm_wdg_item_del(itt);
                  efl_del(itt);
               }
          }
        sd->del_fx.items = eina_list_append(sd->del_fx.items, it);
     }
   if (!sd->del_fx.anim)
     {
        sd->del_fx.cnt = ANIM_CNT_MAX;
        sd->del_fx.anim = ecore_animator_add(_del_fx_anim, sd);
     }
   _changed(sd->pan_obj);
   return EINA_FALSE;
#endif
}

EOLIAN static void
_elm_genlist_item_elm_widget_item_signal_emit(Eo *eo_it EINA_UNUSED, Elm_Gen_Item *it, const char *emission, const char *source)
{
   if (!it->realized)
     {
        WRN("item is not realized yet");
        return;
     }
   edje_object_signal_emit
      (VIEW(it), emission, source);
   if (it->deco_all_view)
     edje_object_signal_emit
       (it->deco_all_view, emission, source);
}

EOLIAN static Eo *
_elm_genlist_item_efl_object_constructor(Eo *eo_it, Elm_Gen_Item *it)
{
   eo_it = efl_constructor(efl_super(eo_it, ELM_GENLIST_ITEM_CLASS));

   efl_access_object_access_parent_set(eo_it, efl_parent_get(eo_it));
   it->base = efl_data_scope_get(eo_it, ELM_WIDGET_ITEM_CLASS);
   efl_access_object_role_set(eo_it, EFL_ACCESS_ROLE_LIST_ITEM);

   return eo_it;
}

static Elm_Gen_Item *
_elm_genlist_item_new(Elm_Genlist_Data *sd,
                      const Elm_Genlist_Item_Class *itc,
                      const void *data,
                      Elm_Object_Item *eo_parent,
                      Elm_Genlist_Item_Type type,
                      Evas_Smart_Cb func,
                      const void *func_data)
{
   Elm_Gen_Item *it2;
   int depth = 0;

   if (!itc) return NULL;

   Eo *eo_it = efl_add(ELM_GENLIST_ITEM_CLASS, sd->obj);
   if (!eo_it) return NULL;
   ELM_GENLIST_ITEM_DATA_GET(eo_it, it);

   it->itc = itc;
   elm_genlist_item_class_ref((Elm_Genlist_Item_Class *)itc);

   ELM_GENLIST_ITEM_DATA_GET(eo_parent, parent);
   WIDGET_ITEM_DATA_SET(EO_OBJ(it), data);
   it->parent = parent;
   it->func.func = func;
   it->func.data = func_data;

   GL_IT(it) = ELM_NEW(Elm_Gen_Item_Type);
   GL_IT(it)->wsd = sd;
   GL_IT(it)->type = type;

   if (it->parent)
     {
        if (GL_IT(it->parent)->type == ELM_GENLIST_ITEM_GROUP)
          GL_IT(it)->group_item = parent;
        else if (GL_IT(it->parent)->group_item)
          GL_IT(it)->group_item = GL_IT(it->parent)->group_item;
     }
   for (it2 = it, depth = 0; it2->parent; it2 = it2->parent)
     {
        if (GL_IT(it2->parent)->type == ELM_GENLIST_ITEM_TREE) depth += 1;
     }
   GL_IT(it)->expanded_depth = depth;
   sd->item_count++;

   return it;
}

static int
_elm_genlist_item_compare(const void *data,
                          const void *data1)
{
   const Elm_Gen_Item *it, *item1;

   it = ELM_GEN_ITEM_FROM_INLIST(data);
   item1 = ELM_GEN_ITEM_FROM_INLIST(data1);
   return GL_IT(it)->wsd->item_compare_cb(EO_OBJ(it), EO_OBJ(item1));
}

static int
_elm_genlist_item_list_compare(const void *data,
                               const void *data1)
{
   const Elm_Gen_Item *it = data;
   const Elm_Gen_Item *item1 = data1;

   return GL_IT(it)->wsd->item_compare_cb(EO_OBJ(it), EO_OBJ(item1));
}

EOLIAN static unsigned int
_elm_genlist_items_count(const Eo *obj EINA_UNUSED, Elm_Genlist_Data *sd)
{
   return sd->item_count;
}

static Eina_List *
_list_last_recursive(Eina_List *list)
{
   Eina_List *ll, *ll2;
   Elm_Object_Item *eo_it2;

   ll = eina_list_last(list);
   if (!ll) return NULL;

   eo_it2 = ll->data;
   ELM_GENLIST_ITEM_DATA_GET(eo_it2, it2);

   if (it2->item->items)
     {
        ll2 = _list_last_recursive(it2->item->items);
        if (ll2)
          {
             return ll2;
          }
     }

   return ll;
}

EOLIAN static Elm_Object_Item*
_elm_genlist_item_append(Eo *obj EINA_UNUSED, Elm_Genlist_Data *sd, const Elm_Genlist_Item_Class *itc, const void *data, Elm_Object_Item *eo_parent, Elm_Genlist_Item_Type type, Evas_Smart_Cb func, const void *func_data)
{
   Elm_Gen_Item *it;

   EINA_SAFETY_ON_NULL_RETURN_VAL(itc, NULL);

   if (eo_parent)
     {
        ELM_GENLIST_ITEM_DATA_GET(eo_parent, parent);
        ELM_GENLIST_ITEM_CHECK_OR_RETURN(parent, NULL);
        EINA_SAFETY_ON_FALSE_RETURN_VAL(obj == WIDGET(parent), NULL);
     }

   it = _elm_genlist_item_new
       (sd, itc, data, eo_parent, type, func, func_data);
   if (!it) return NULL;

   if (!it->parent)
     {
        if (GL_IT(it)->type == ELM_GENLIST_ITEM_GROUP)
          sd->group_items = eina_list_append(sd->group_items, it);
        sd->items = eina_inlist_append(sd->items, EINA_INLIST_GET(it));
        GL_IT(it)->rel = NULL;
     }
   else
     {
        Elm_Object_Item *eo_it2 = NULL;
        Eina_List *ll = _list_last_recursive(it->parent->item->items);

        if (ll) eo_it2 = ll->data;
        it->parent->item->items =
          eina_list_append(it->parent->item->items, EO_OBJ(it));
        if (!eo_it2) eo_it2 = EO_OBJ(it->parent);
        ELM_GENLIST_ITEM_DATA_GET(eo_it2, it2);
        sd->items = eina_inlist_append_relative
          (sd->items, EINA_INLIST_GET(it), EINA_INLIST_GET(it2));
        GL_IT(it)->rel = it2;
        it2->item->rel_revs = eina_list_append(it2->item->rel_revs, it);
     }
   GL_IT(it)->before = EINA_FALSE;
   _item_queue_direct(it, NULL);
   return EO_OBJ(it);
}

EOLIAN static Elm_Object_Item*
_elm_genlist_item_prepend(Eo *obj EINA_UNUSED, Elm_Genlist_Data *sd, const Elm_Genlist_Item_Class *itc, const void *data, Elm_Object_Item *eo_parent, Elm_Genlist_Item_Type type, Evas_Smart_Cb func, const void *func_data)
{
   Elm_Gen_Item *it = NULL;

   EINA_SAFETY_ON_NULL_RETURN_VAL(itc, NULL);
   if (eo_parent)
     {
        ELM_GENLIST_ITEM_DATA_GET(eo_parent, parent);
        ELM_GENLIST_ITEM_CHECK_OR_RETURN(parent, NULL);
        EINA_SAFETY_ON_FALSE_RETURN_VAL(obj == WIDGET(parent), NULL);
        /* first sub-item should allways be appended */
        if (!parent->item->items)
          return _elm_genlist_item_append(obj, sd, itc, data, eo_parent, type, func, func_data);
     }
   it = _elm_genlist_item_new
       (sd, itc, data, eo_parent, type, func, func_data);

   if (!it) return NULL;

   if (sd->items) GL_IT(it)->is_prepend = EINA_TRUE;
   if (!it->parent)
     {
        if (GL_IT(it)->type == ELM_GENLIST_ITEM_GROUP)
          sd->group_items = eina_list_prepend(sd->group_items, it);
        sd->items = eina_inlist_prepend(sd->items, EINA_INLIST_GET(it));
        GL_IT(it)->rel = NULL;
     }
   else
     {
        Elm_Object_Item *eo_it2 = NULL;
        Eina_List *ll = it->parent->item->items;

        if (ll) eo_it2 = ll->data;
        it->parent->item->items =
          eina_list_prepend(it->parent->item->items, EO_OBJ(it));
        if (!eo_it2) eo_it2 = EO_OBJ(it->parent);
        ELM_GENLIST_ITEM_DATA_GET(eo_it2, it2);
        if (it2)
          {
             sd->items = eina_inlist_prepend_relative
                 (sd->items, EINA_INLIST_GET(it), EINA_INLIST_GET(it2));
             GL_IT(it)->rel = it2;
             it2->item->rel_revs = eina_list_append(it2->item->rel_revs, it);
          }
     }
   GL_IT(it)->before = EINA_TRUE;
   _item_queue_direct(it, NULL);
   return EO_OBJ(it);
}

EOLIAN static Elm_Object_Item*
_elm_genlist_item_insert_after(Eo *obj EINA_UNUSED, Elm_Genlist_Data *sd, const Elm_Genlist_Item_Class *itc, const void *data, Elm_Object_Item *eo_parent, Elm_Object_Item *eo_after, Elm_Genlist_Item_Type type, Evas_Smart_Cb func, const void *func_data)
{
   EINA_SAFETY_ON_NULL_RETURN_VAL(eo_after, NULL);
   ELM_GENLIST_ITEM_DATA_GET(eo_after, after_it);
   Elm_Gen_Item *it;

   ELM_GENLIST_ITEM_CHECK_OR_RETURN(after_it, NULL);
   EINA_SAFETY_ON_FALSE_RETURN_VAL(obj == WIDGET(after_it), NULL);
   if (eo_parent)
     {
        ELM_GENLIST_ITEM_DATA_GET(eo_parent, parent);
        ELM_GENLIST_ITEM_CHECK_OR_RETURN(parent, NULL);
        EINA_SAFETY_ON_FALSE_RETURN_VAL(obj == WIDGET(parent), NULL);
     }

   /* It makes no sense to insert after in an empty list with after !=
    * NULL, something really bad is happening in your app. */
   EINA_SAFETY_ON_NULL_RETURN_VAL(sd->items, NULL);

   it = _elm_genlist_item_new
       (sd, itc, data, eo_parent, type, func, func_data);
   if (!it) return NULL;

   if (!it->parent)
     {
        if ((GL_IT(it)->type == ELM_GENLIST_ITEM_GROUP) &&
            (GL_IT(after_it)->type == ELM_GENLIST_ITEM_GROUP))
          sd->group_items = eina_list_append_relative
              (sd->group_items, it, after_it);
     }
   else
     {
        it->parent->item->items =
          eina_list_append_relative(it->parent->item->items, EO_OBJ(it), eo_after);
     }
   sd->items = eina_inlist_append_relative
       (sd->items, EINA_INLIST_GET(it), EINA_INLIST_GET(after_it));

   GL_IT(it)->rel = after_it;
   after_it->item->rel_revs = eina_list_append(after_it->item->rel_revs, it);
   GL_IT(it)->before = EINA_FALSE;
   _item_queue_direct(it, NULL);

   if (elm_genlist_item_next_get(eo_after) == EO_OBJ(sd->aligned_item))
     sd->aligned_item = NULL;
   return EO_OBJ(it);
}

EOLIAN static Elm_Object_Item*
_elm_genlist_item_insert_before(Eo *obj, Elm_Genlist_Data *sd, const Elm_Genlist_Item_Class *itc, const void *data, Elm_Object_Item *eo_parent, Elm_Object_Item *eo_before, Elm_Genlist_Item_Type type, Evas_Smart_Cb func, const void *func_data)
{
   EINA_SAFETY_ON_NULL_RETURN_VAL(eo_before, NULL);
   ELM_GENLIST_ITEM_DATA_GET(eo_before, before_it);
   Elm_Gen_Item *it;

   ELM_GENLIST_ITEM_CHECK_OR_RETURN(before_it, NULL);
   EINA_SAFETY_ON_FALSE_RETURN_VAL(obj == WIDGET(before_it), NULL);
   if (eo_parent)
     {
        ELM_GENLIST_ITEM_DATA_GET(eo_parent, parent);
        ELM_GENLIST_ITEM_CHECK_OR_RETURN(parent, NULL);
        EINA_SAFETY_ON_FALSE_RETURN_VAL(obj == WIDGET(parent), NULL);
     }

   /* It makes no sense to insert before in an empty list with before
    * != NULL, something really bad is happening in your app. */
   EINA_SAFETY_ON_NULL_RETURN_VAL(sd->items, NULL);

   it = _elm_genlist_item_new
       (sd, itc, data, eo_parent, type, func, func_data);
   if (!it) return NULL;

   if (!it->parent)
     {
        if ((GL_IT(it)->type == ELM_GENLIST_ITEM_GROUP) &&
            (GL_IT(before_it)->type == ELM_GENLIST_ITEM_GROUP))
          sd->group_items =
            eina_list_prepend_relative(sd->group_items, it, before_it);
     }
   else
     {
        it->parent->item->items =
          eina_list_prepend_relative(it->parent->item->items, EO_OBJ(it), eo_before);
     }
   sd->items = eina_inlist_prepend_relative
       (sd->items, EINA_INLIST_GET(it), EINA_INLIST_GET(before_it));

   GL_IT(it)->rel = before_it;
   before_it->item->rel_revs = eina_list_append(before_it->item->rel_revs, it);
   GL_IT(it)->before = EINA_TRUE;
   _item_queue_direct(it, NULL);

   if (before_it == sd->aligned_item)
     sd->aligned_item = NULL;

   return EO_OBJ(it);
}

EOLIAN static Elm_Object_Item*
_elm_genlist_item_sorted_insert(Eo *obj, Elm_Genlist_Data *sd, const Elm_Genlist_Item_Class *itc, const void *data, Elm_Object_Item *eo_parent, Elm_Genlist_Item_Type type, Eina_Compare_Cb comp, Evas_Smart_Cb func, const void *func_data)
{
   Elm_Gen_Item *rel = NULL;
   Elm_Gen_Item *it;
   Elm_Object_Item *eo_it;

   if (eo_parent)
     {
        ELM_GENLIST_ITEM_DATA_GET(eo_parent, parent);
        ELM_GENLIST_ITEM_CHECK_OR_RETURN(parent, NULL);
        EINA_SAFETY_ON_FALSE_RETURN_VAL(obj == WIDGET(parent), NULL);
     }

   it = _elm_genlist_item_new
       (sd, itc, data, eo_parent, type, func, func_data);
   if (!it) return NULL;
   eo_it = EO_OBJ(it);

   sd->item_compare_cb = comp;

   if (it->parent)
     {
        Elm_Object_Item *eo_rel = NULL;
        Eina_List *l;
        int cmp_result;

        l = eina_list_search_sorted_near_list
            (it->parent->item->items, _elm_genlist_item_list_compare, eo_it,
            &cmp_result);

        if (l)
          {
             eo_rel = eina_list_data_get(l);
             rel = efl_data_scope_get(eo_rel, ELM_GENLIST_ITEM_CLASS);

             if (cmp_result >= 0)
               {
                  it->parent->item->items = eina_list_prepend_relative_list
                      (it->parent->item->items, eo_it, l);
                  sd->items = eina_inlist_prepend_relative
                      (sd->items, EINA_INLIST_GET(it), EINA_INLIST_GET(rel));
                  it->item->before = EINA_TRUE;
               }
             else if (cmp_result < 0)
               {
                  it->parent->item->items = eina_list_append_relative_list
                      (it->parent->item->items, eo_it, l);
                  sd->items = eina_inlist_append_relative
                      (sd->items, EINA_INLIST_GET(it), EINA_INLIST_GET(rel));
                  it->item->before = EINA_FALSE;
                  it->item->is_prepend = EINA_TRUE;
               }
          }
        else
          {
             rel = it->parent;

             // ignoring the comparison
             it->parent->item->items = eina_list_prepend_relative_list
                 (it->parent->item->items, eo_it, l);
             sd->items = eina_inlist_prepend_relative
                 (sd->items, EINA_INLIST_GET(it), EINA_INLIST_GET(rel));
             it->item->before = EINA_FALSE;
          }
     }
   else
     {
        if (!sd->state)
          {
             sd->state = eina_inlist_sorted_state_new();
             eina_inlist_sorted_state_init(sd->state, sd->items);
             sd->requeued = EINA_FALSE;
          }

        if (GL_IT(it)->type == ELM_GENLIST_ITEM_GROUP)
           sd->group_items = eina_list_append(sd->group_items, it);

        sd->items = eina_inlist_sorted_state_insert
            (sd->items, EINA_INLIST_GET(it), _elm_genlist_item_compare,
            sd->state);

        if (EINA_INLIST_GET(it)->next)
          {
             rel = ELM_GEN_ITEM_FROM_INLIST(EINA_INLIST_GET(it)->next);
             GL_IT(it)->before = EINA_TRUE;
             GL_IT(it)->is_prepend = EINA_TRUE;
          }
        else if (EINA_INLIST_GET(it)->prev)
          {
             rel = ELM_GEN_ITEM_FROM_INLIST(EINA_INLIST_GET(it)->prev);
             GL_IT(it)->before = EINA_FALSE;
          }
     }

   if (rel)
     {
        GL_IT(it)->rel = rel;
        rel->item->rel_revs = eina_list_append(rel->item->rel_revs, it);
     }

   _item_queue_direct(it, _elm_genlist_item_list_compare);
   return eo_it;
}

EOLIAN static void
_elm_genlist_clear(Eo *obj, Elm_Genlist_Data *sd EINA_UNUSED)
{
   _internal_elm_genlist_clear(obj);
}


EOLIAN static void
_elm_genlist_multi_select_set(Eo *obj EINA_UNUSED, Elm_Genlist_Data *sd, Eina_Bool multi)
{
   sd->multi = !!multi;
   if (!sd->multi && sd->selected)
     {
        Eina_List *l, *ll;
        Elm_Object_Item *eo_it;
        Elm_Object_Item *last = sd->selected->data;
        EINA_LIST_FOREACH_SAFE(sd->selected, l, ll, eo_it)
           if (last != eo_it)
             {
                ELM_GENLIST_ITEM_DATA_GET(eo_it, it);
                _item_unselect(it);
             }
     }
}

EOLIAN static Eina_Bool
_elm_genlist_multi_select_get(const Eo *obj EINA_UNUSED, Elm_Genlist_Data *sd)
{
   return sd->multi;
}

EOLIAN static void
_elm_genlist_multi_select_mode_set(Eo *obj EINA_UNUSED, Elm_Genlist_Data *sd EINA_UNUSED, Elm_Object_Multi_Select_Mode mode EINA_UNUSED)
{
/* Need to be implemented
   if (mode >= ELM_OBJECT_MULTI_SELECT_MODE_MAX)
     return;

   if (sd->multi_select_mode != mode)
     sd->multi_select_mode = mode;
*/
   return;
}

EOLIAN static Elm_Object_Multi_Select_Mode
_elm_genlist_multi_select_mode_get(const Eo *obj EINA_UNUSED, Elm_Genlist_Data *sd EINA_UNUSED)
{
   // Need to be implemented
   //return sd->multi_select_mode;
   return ELM_OBJECT_MULTI_SELECT_MODE_DEFAULT;
}

EOLIAN static Elm_Object_Item*
_elm_genlist_selected_item_get(const Eo *obj EINA_UNUSED, Elm_Genlist_Data *sd)
{
   if (sd->selected)
     return sd->selected->data;

   return NULL;
}

EOLIAN static const Eina_List*
_elm_genlist_selected_items_get(const Eo *obj EINA_UNUSED, Elm_Genlist_Data *sd)
{
   return sd->selected;
}

EOLIAN static Eina_List*
_elm_genlist_realized_items_get(const Eo *obj EINA_UNUSED, Elm_Genlist_Data *sd)
{
   Item_Block *itb;
   Eina_List *list = NULL;
   Eina_Bool done = EINA_FALSE;

   EINA_INLIST_FOREACH(sd->blocks, itb)
     {
        if (itb->realized)
          {
             Eina_List *l;
             Elm_Gen_Item *it;

             done = EINA_TRUE;
             EINA_LIST_FOREACH(itb->items, l, it)
               {
                  if (it->realized) list = eina_list_append(list, EO_OBJ(it));
               }
          }
        else
          {
             if (done) break;
          }
     }
   return list;
}

EOLIAN static Elm_Object_Item *
_elm_genlist_search_by_text_item_get(Eo *obj EINA_UNUSED,
                                     Elm_Genlist_Data *sd,
                                     Elm_Object_Item *item_to_search_from,
                                     const char *part_name,
                                     const char *pattern,
                                     Elm_Glob_Match_Flags flags)
{
   Elm_Gen_Item *it = NULL;
   char *str = NULL;
   Eina_Inlist *start = NULL;
   int fnflags = 0;

   if (!pattern) return NULL;
   if (!sd->items) return NULL;

   if (flags & ELM_GLOB_MATCH_NO_ESCAPE) fnflags |= FNM_NOESCAPE;
   if (flags & ELM_GLOB_MATCH_PATH) fnflags |= FNM_PATHNAME;
   if (flags & ELM_GLOB_MATCH_PERIOD) fnflags |= FNM_PERIOD;
#ifdef FNM_CASEFOLD
   if (flags & ELM_GLOB_MATCH_NOCASE) fnflags |= FNM_CASEFOLD;
#endif

   start = (item_to_search_from) ?
   EINA_INLIST_GET((Elm_Gen_Item *)efl_data_scope_get(item_to_search_from, ELM_GENLIST_ITEM_CLASS)) :
   sd->items;
   EINA_INLIST_FOREACH(start, it)
     {
        if (!it->itc->func.text_get) continue;
        str = it->itc->func.text_get((void *)WIDGET_ITEM_DATA_GET(EO_OBJ(it)), WIDGET(it), part_name);
        if (!str) continue;
        if (!fnmatch(pattern, str, fnflags))
          {
             free(str);
             return EO_OBJ(it);
          }
        free(str);
     }
   return NULL;
}

EOLIAN static Elm_Object_Item*
_elm_genlist_at_xy_item_get(const Eo *obj EINA_UNUSED, Elm_Genlist_Data *sd, Evas_Coord x, Evas_Coord y, int *posret)
{
   Evas_Coord ox, oy, ow, oh;
   Evas_Coord lasty;
   Item_Block *itb;

   evas_object_geometry_get(sd->pan_obj, &ox, &oy, &ow, &oh);
   lasty = oy;
   EINA_INLIST_FOREACH(sd->blocks, itb)
     {
        Eina_List *l;
        Elm_Gen_Item *it;
        if (!ELM_RECTS_INTERSECT(ox + itb->x - itb->sd->pan_x,
                                 oy + itb->y - itb->sd->pan_y,
                                 sd->minw, itb->minh, x, y, 1, 1))
          continue;
        EINA_LIST_FOREACH(itb->items, l, it)
          {
             Evas_Coord itx, ity;

             itx = ox + itb->x + it->x - itb->sd->pan_x;
             ity = oy + itb->y + it->y - itb->sd->pan_y;
             if (ELM_RECTS_INTERSECT
                   (itx, ity, GL_IT(it)->w, GL_IT(it)->h, x, y, 1, 1))
               {
                  if (posret)
                    {
                       if (y <= (ity + (GL_IT(it)->h / 4))) *posret = -1;
                       else if (y >= (ity + GL_IT(it)->h - (GL_IT(it)->h / 4)))
                         *posret = 1;
                       else *posret = 0;
                    }

                  return EO_OBJ(it);
               }
             lasty = ity + GL_IT(it)->h;
          }
     }
   if (posret)
     {
        if (y > lasty) *posret = 1;
        else *posret = -1;
     }

   return NULL;
}

EOLIAN static Elm_Object_Item*
_elm_genlist_first_item_get(const Eo *obj EINA_UNUSED, Elm_Genlist_Data *sd)
{
   Elm_Gen_Item *tit, *it = NULL;

   EINA_INLIST_REVERSE_FOREACH(sd->items, tit) it = tit;

   return EO_OBJ(it);
}

EOLIAN static Elm_Object_Item*
_elm_genlist_last_item_get(const Eo *obj EINA_UNUSED, Elm_Genlist_Data *sd)
{
   Elm_Gen_Item *it;

   if (!sd->items) return NULL;

   it = ELM_GEN_ITEM_FROM_INLIST(sd->items->last);
   return EO_OBJ(it);
}

EOLIAN static Elm_Object_Item *
_elm_genlist_item_next_get(const Eo *eo_it EINA_UNUSED, Elm_Gen_Item *it)
{
   while (it)
     {
        it = ELM_GEN_ITEM_FROM_INLIST(EINA_INLIST_GET(it)->next);
        if (it) break;
     }

   if (it) return EO_OBJ(it);
   else return NULL;
}

EOLIAN static Elm_Object_Item *
_elm_genlist_item_prev_get(const Eo *eo_it EINA_UNUSED, Elm_Gen_Item *it)
{
   while (it)
     {
        it = ELM_GEN_ITEM_FROM_INLIST(EINA_INLIST_GET(it)->prev);
        if (it) break;
     }

   if (it) return EO_OBJ(it);
   else return NULL;
}

EOLIAN static Elm_Object_Item *
_elm_genlist_item_parent_item_get(const Eo *eo_it EINA_UNUSED, Elm_Gen_Item *it)
{
   ELM_GENLIST_ITEM_CHECK_OR_RETURN(it, NULL);

   return EO_OBJ(it->parent);
}

EOLIAN static unsigned int
_elm_genlist_item_subitems_count(Eo *eo_item EINA_UNUSED, Elm_Gen_Item *item)
{
   ELM_GENLIST_ITEM_CHECK_OR_RETURN(item, 0);

   return eina_list_count(item->item->items);
}

EOLIAN static const Eina_List *
_elm_genlist_item_subitems_get(const Eo *eo_item EINA_UNUSED, Elm_Gen_Item *item)
{
   ELM_GENLIST_ITEM_CHECK_OR_RETURN(item, NULL);

   return item->item->items;
}

EOLIAN static void
_elm_genlist_item_subitems_clear(Eo *eo_item EINA_UNUSED, Elm_Gen_Item *it)
{
   ELM_GENLIST_ITEM_CHECK_OR_RETURN(it);
   Eina_List *l, *ll;
   Elm_Object_Item *eo_it2;

   EINA_LIST_FOREACH_SAFE(GL_IT(it)->items, l, ll, eo_it2)
     efl_del(eo_it2);
//   elm_wdg_item_del(eo_it2);
}

EOLIAN static void
_elm_genlist_item_selected_set(Eo *eo_item EINA_UNUSED, Elm_Gen_Item *it,
      Eina_Bool selected)
{
   ELM_GENLIST_ITEM_CHECK_OR_RETURN(it);

   if (elm_wdg_item_disabled_get(EO_OBJ(it))) return;
   // FIXME: This API has highlight/unhighlith feature also..

   if (selected) _item_highlight(it);
   else _item_unhighlight(it, EINA_TRUE);

   selected = !!selected;
   if (it->selected == selected) return;

   if (selected) _item_select(it);
   else _item_unselect(it);
}

EOLIAN static Eina_Bool
_elm_genlist_item_selected_get(const Eo *eo_item EINA_UNUSED, Elm_Gen_Item *it)
{
   ELM_GENLIST_ITEM_CHECK_OR_RETURN(it, EINA_FALSE);

   return it->selected;
}

EOLIAN static void
_elm_genlist_item_expanded_set(Eo *eo_item EINA_UNUSED, Elm_Gen_Item *it, Eina_Bool expanded)
{
   ELM_GENLIST_ITEM_CHECK_OR_RETURN(it);

   expanded = !!expanded;
   if (GL_IT(it)->expanded == expanded) return;
   if (GL_IT(it)->type != ELM_GENLIST_ITEM_TREE) return;
   GL_IT(it)->expanded = expanded;
   GL_IT(it)->wsd->expanded_item = it;

   if (GL_IT(it)->expanded)
     {
        if (it->realized)
          edje_object_signal_emit(VIEW(it), SIGNAL_EXPANDED, "elm");
        efl_event_callback_legacy_call
              (WIDGET(it), ELM_GENLIST_EVENT_EXPANDED, EO_OBJ(it));
        if (_elm_atspi_enabled())
          efl_access_active_descendant_changed_signal_emit(WIDGET(it), eo_item);
     }
   else
     {
        if (it->realized)
          edje_object_signal_emit(VIEW(it), SIGNAL_CONTRACTED, "elm");
        efl_event_callback_legacy_call
              (WIDGET(it), ELM_GENLIST_EVENT_CONTRACTED, EO_OBJ(it));
        if (_elm_atspi_enabled())
          efl_access_active_descendant_changed_signal_emit(WIDGET(it), eo_item);
     }
}

EOLIAN static Eina_Bool
_elm_genlist_item_expanded_get(const Eo *eo_item EINA_UNUSED, Elm_Gen_Item *it)
{
   ELM_GENLIST_ITEM_CHECK_OR_RETURN(it, EINA_FALSE);

   return GL_IT(it)->expanded;
}

EOLIAN static int
_elm_genlist_item_expanded_depth_get(const Eo *eo_item EINA_UNUSED, Elm_Gen_Item *it)
{
   ELM_GENLIST_ITEM_CHECK_OR_RETURN(it, 0);

   return GL_IT(it)->expanded_depth;
}

EOLIAN static void
_elm_genlist_item_promote(Eo *eo_it EINA_UNUSED, Elm_Gen_Item *it)
{
   ELM_GENLIST_ITEM_CHECK_OR_RETURN(it);

   Elm_Object_Item *eo_first_item = elm_genlist_first_item_get(WIDGET(it));
   ELM_GENLIST_ITEM_DATA_GET(eo_first_item, first_item);
   _item_move_before(it, first_item);
}

EOLIAN static void
_elm_genlist_item_demote(Eo *eo_it EINA_UNUSED, Elm_Gen_Item *it)
{
   ELM_GENLIST_ITEM_CHECK_OR_RETURN(it);
   Elm_Object_Item *eo_last_item = elm_genlist_last_item_get(WIDGET(it));
   ELM_GENLIST_ITEM_DATA_GET(eo_last_item, last_item);
   _item_move_after(it, last_item);
}

EOLIAN static void
_elm_genlist_item_show(Eo *eo_item EINA_UNUSED, Elm_Gen_Item *it, Elm_Genlist_Item_Scrollto_Type type)
{
   ELM_GENLIST_ITEM_CHECK_OR_RETURN(it);

   if (it->hide)
     {
        WRN("Filter hided item cannot move to show");
        return;
     }

   Elm_Genlist_Data *sd = GL_IT(it)->wsd;
   sd->show_item = it;
   sd->bring_in = EINA_FALSE;
   sd->scroll_to_type = type;
   _changed(sd->pan_obj);
}

EOLIAN static void
_elm_genlist_item_bring_in(Eo *eo_item EINA_UNUSED, Elm_Gen_Item *it, Elm_Genlist_Item_Scrollto_Type type)
{
   ELM_GENLIST_ITEM_CHECK_OR_RETURN(it);

   if (it->hide)
     {
        WRN("Filter hided item cannot move to bring in");
        return;
     }

   Elm_Genlist_Data *sd = GL_IT(it)->wsd;
   ELM_WIDGET_DATA_GET_OR_RETURN(WIDGET(it), wd);
   sd->bring_in = EINA_TRUE;
   sd->scroll_to_type = type;

   if (wd->scroll_item_align_enable)
     {
        it = _adjust_item_align(it);
        sd->adjusted_item = sd->show_item = it;

        if (sd->aligned_item && sd->show_item != sd->aligned_item)
          edje_object_signal_emit(VIEW(sd->aligned_item), SIGNAL_ITEM_UNHIGHLIGHTED, "elm");
     }
   else
     sd->show_item = it;

   _changed(sd->pan_obj);
}

EOLIAN static void
_elm_genlist_item_all_contents_unset(Eo *eo_item EINA_UNUSED, Elm_Gen_Item *it, Eina_List **l)
{
   Evas_Object *content;

   ELM_GENLIST_ITEM_CHECK_OR_RETURN(it);

   EINA_LIST_FREE (it->contents, content)
     {
        evas_object_smart_member_del(content);
        // edje can be reused by item caching,
        // content should be un-swallowed from edje
        edje_object_part_unswallow(VIEW(it), content);
        evas_object_hide(content);
        if (l) *l = eina_list_append(*l, content);
     }
}

EOLIAN static void
_elm_genlist_item_update(Eo *eo_item EINA_UNUSED, Elm_Gen_Item *it)
{
   ELM_GENLIST_ITEM_CHECK_OR_RETURN(it);

   if (!GL_IT(it)->block) return;

   GL_IT(it)->updateme = EINA_TRUE;
   GL_IT(it)->block->calc_done = EINA_FALSE;
   GL_IT(it)->wsd->calc_done = EINA_FALSE;
   _changed(GL_IT(it)->wsd->pan_obj);
}

EOLIAN static void
_elm_genlist_item_fields_update(Eo *eo_item EINA_UNUSED, Elm_Gen_Item *it,
                               const char *parts,
                               Elm_Genlist_Item_Field_Type itf)
{
   ELM_GENLIST_ITEM_CHECK_OR_RETURN(it);

   if (!GL_IT(it)->block) return;

   if ((!itf) || (itf & ELM_GENLIST_ITEM_FIELD_TEXT))
     {
        _item_text_realize(it, VIEW(it), parts);
     }
   if ((!itf) || (itf & ELM_GENLIST_ITEM_FIELD_CONTENT))
     {
        _item_content_realize(it, VIEW(it), &it->contents, "contents", parts);
        if (it->flipped)
          {
              _item_content_realize(it, VIEW(it), &GL_IT(it)->flip_content_objs,
                                     "flips", parts);
          }
        if (GL_IT(it)->deco_it_view)
          {
             _item_content_realize(it, GL_IT(it)->deco_it_view,
                                   &GL_IT(it)->deco_it_contents,
                                   "contents", parts);
          }
        if (GL_IT(it)->wsd->decorate_all_mode)
          {
             _item_content_realize(it, it->deco_all_view,
                                   &GL_IT(it)->deco_all_contents,
                                   "contents", parts);
          }
     }

   if ((!itf) || (itf & ELM_GENLIST_ITEM_FIELD_STATE))
     _item_state_realize(it, VIEW(it), parts);

   GL_IT(it)->calc_done = EINA_FALSE;
   GL_IT(it)->block->calc_done = EINA_FALSE;
   GL_IT(it)->wsd->calc_done = EINA_FALSE;
    _changed(GL_IT(it)->wsd->pan_obj);
}

EOLIAN static void
_elm_genlist_item_item_class_update(Eo *eo_it EINA_UNUSED, Elm_Gen_Item *it,
                                   const Elm_Genlist_Item_Class *itc)
{
   ELM_GENLIST_ITEM_CHECK_OR_RETURN(it);
   EINA_SAFETY_ON_NULL_RETURN(itc);
   Elm_Genlist_Data *sd = GL_IT(it)->wsd;

   /* Decrease the orignal item class refcount to prevent memory leak */
   if (it->itc != itc)
     {
        elm_genlist_item_class_unref((Elm_Genlist_Item_Class *)it->itc);
        it->itc = itc;
        elm_genlist_item_class_ref((Elm_Genlist_Item_Class *)it->itc);
     }

   if (!GL_IT(it)->block) return;

   // FIXME(160711): orignal edje object updated by _view_theme_update
   // cannot receive signal emit properly. This edje bug must be fixed.
   _item_cache_all_free(sd);

   sd->no_cache = EINA_TRUE;
   _item_unrealize(it, EINA_FALSE);
   sd->no_cache = EINA_FALSE;
   _item_realize(it, EINA_FALSE);

   if (sd->aligned_item == it)
     edje_object_signal_emit(VIEW(it), SIGNAL_ITEM_HIGHLIGHTED, "elm");

   GL_IT(it)->calc_done = EINA_FALSE;
   GL_IT(it)->block->calc_done = EINA_FALSE;
   sd->calc_done = EINA_FALSE;

   _changed(sd->pan_obj);
}

EOLIAN static const Elm_Genlist_Item_Class *
_elm_genlist_item_item_class_get(const Eo *eo_item EINA_UNUSED, Elm_Gen_Item *it)
{
   ELM_GENLIST_ITEM_CHECK_OR_RETURN(it, NULL);
   return it->itc;
}

static Evas_Object *
_elm_genlist_item_label_create(void *data,
                               Evas_Object *obj EINA_UNUSED,
                               Evas_Object *tooltip,
                               void *it EINA_UNUSED)
{
   Evas_Object *label = elm_label_add(tooltip);

   if (!label)
     return NULL;

   elm_object_style_set(label, "tooltip");
   elm_object_text_set(label, data);

   return label;
}

static void
_elm_genlist_item_label_del_cb(void *data,
                               Evas_Object *obj EINA_UNUSED,
                               void *event_info EINA_UNUSED)
{
   eina_stringshare_del(data);
}

EAPI void
elm_genlist_item_tooltip_text_set(Elm_Object_Item *it,
                                  const char *text)
{
   elm_wdg_item_tooltip_text_set(it, text);
}

EOLIAN static void
_elm_genlist_item_elm_widget_item_tooltip_text_set(Eo *eo_it, Elm_Gen_Item *it, const char *text)
{
   ELM_GENLIST_ITEM_CHECK_OR_RETURN(it);

   text = eina_stringshare_add(text);
   elm_genlist_item_tooltip_content_cb_set
     (eo_it, _elm_genlist_item_label_create, text,
     _elm_genlist_item_label_del_cb);
}

EAPI void
elm_genlist_item_tooltip_content_cb_set(Elm_Object_Item *item,
                                        Elm_Tooltip_Item_Content_Cb func,
                                        const void *data,
                                        Evas_Smart_Cb del_cb)
{
   elm_wdg_item_tooltip_content_cb_set(item, func, data, del_cb);
}


EOLIAN static void
_elm_genlist_item_elm_widget_item_tooltip_content_cb_set(Eo *eo_it, Elm_Gen_Item *it,
                                        Elm_Tooltip_Item_Content_Cb func,
                                        const void *data,
                                        Evas_Smart_Cb del_cb)
{
   ELM_GENLIST_ITEM_CHECK_OR_GOTO(it, error);

   if ((it->tooltip.content_cb != func) || (it->tooltip.data != data))
     {
        if (it->tooltip.del_cb)
           it->tooltip.del_cb((void *)it->tooltip.data, WIDGET(it), it);

        it->tooltip.content_cb = func;
        it->tooltip.data = data;
        it->tooltip.del_cb = del_cb;
     }

   if (VIEW(it))
     {
        elm_wdg_item_tooltip_content_cb_set
              (efl_super(eo_it, ELM_GENLIST_ITEM_CLASS), it->tooltip.content_cb, it->tooltip.data, NULL);
        elm_wdg_item_tooltip_style_set(eo_it, it->tooltip.style);
        elm_wdg_item_tooltip_window_mode_set(eo_it, it->tooltip.free_size);
     }

   return;

error:
   if (del_cb) del_cb((void *)data, NULL, NULL);
}

EAPI void
elm_genlist_item_tooltip_unset(Elm_Object_Item *item)
{
   elm_wdg_item_tooltip_unset(item);
}

EOLIAN static void
_elm_genlist_item_elm_widget_item_tooltip_unset(Eo *eo_it, Elm_Gen_Item *it)
{
   ELM_GENLIST_ITEM_CHECK_OR_RETURN(it);

   if ((VIEW(it)) && (it->tooltip.content_cb))
     elm_wdg_item_tooltip_unset(efl_super(eo_it, ELM_GENLIST_ITEM_CLASS));

   if (it->tooltip.del_cb)
     it->tooltip.del_cb((void *)it->tooltip.data, WIDGET(it), it);
   it->tooltip.del_cb = NULL;
   it->tooltip.content_cb = NULL;
   it->tooltip.data = NULL;
   it->tooltip.free_size = EINA_FALSE;
   if (it->tooltip.style)
     elm_wdg_item_tooltip_style_set(eo_it, NULL);
}

EAPI void
elm_genlist_item_tooltip_style_set(Elm_Object_Item *item,
                                   const char *style)
{
   elm_wdg_item_tooltip_style_set(item, style);
}

EOLIAN static void
_elm_genlist_item_elm_widget_item_tooltip_style_set(Eo *eo_it, Elm_Gen_Item *it,
                                   const char *style)
{
   ELM_GENLIST_ITEM_CHECK_OR_RETURN(it);

   eina_stringshare_replace(&it->tooltip.style, style);
   if (VIEW(it)) elm_wdg_item_tooltip_style_set(efl_super(eo_it, ELM_GENLIST_ITEM_CLASS), style);

}

EAPI const char *
elm_genlist_item_tooltip_style_get(const Elm_Object_Item *it)
{
   return elm_wdg_item_tooltip_style_get(it);
}

EOLIAN static const char *
_elm_genlist_item_elm_widget_item_tooltip_style_get(const Eo *eo_it EINA_UNUSED, Elm_Gen_Item *it)
{
   return it->tooltip.style;
}

EAPI Eina_Bool
elm_genlist_item_tooltip_window_mode_set(Elm_Object_Item *item,
                                         Eina_Bool disable)
{
   return elm_wdg_item_tooltip_window_mode_set(item, disable);
}

EOLIAN static Eina_Bool
_elm_genlist_item_elm_widget_item_tooltip_window_mode_set(Eo *eo_it, Elm_Gen_Item *it,
                                   Eina_Bool disable)
{
   ELM_GENLIST_ITEM_CHECK_OR_RETURN(it, EINA_FALSE);

   it->tooltip.free_size = disable;
   if (VIEW(it))
     {
        Eina_Bool ret;
        ret = elm_wdg_item_tooltip_window_mode_set(efl_super(eo_it, ELM_GENLIST_ITEM_CLASS), disable);
        return ret;
     }

   return disable;
}

EAPI Eina_Bool
elm_genlist_item_tooltip_window_mode_get(const Elm_Object_Item *eo_it)
{
   return elm_wdg_item_tooltip_window_mode_get(eo_it);
}

EOLIAN static Eina_Bool
_elm_genlist_item_elm_widget_item_tooltip_window_mode_get(const Eo *eo_it EINA_UNUSED, Elm_Gen_Item *it)
{
   return it->tooltip.free_size;
}

EAPI void
elm_genlist_item_cursor_set(Elm_Object_Item *item,
                            const char *cursor)
{
   elm_wdg_item_cursor_set(item, cursor);
}

EOLIAN static void
_elm_genlist_item_elm_widget_item_cursor_set(Eo *eo_it, Elm_Gen_Item *it,
                            const char *cursor)
{
   ELM_GENLIST_ITEM_CHECK_OR_RETURN(it);
   eina_stringshare_replace(&it->mouse_cursor, cursor);
   if (VIEW(it)) elm_wdg_item_cursor_set(efl_super(eo_it, ELM_GENLIST_ITEM_CLASS), cursor);
}

EAPI const char *
elm_genlist_item_cursor_get(const Elm_Object_Item *eo_it)
{   return elm_wdg_item_cursor_get(eo_it);

}

EAPI void
elm_genlist_item_cursor_unset(Elm_Object_Item *item)
{
   elm_wdg_item_cursor_unset(item);
}

EOLIAN static void
_elm_genlist_item_elm_widget_item_cursor_unset(Eo *eo_it, Elm_Gen_Item *it)
{
   ELM_GENLIST_ITEM_CHECK_OR_RETURN(it);

   if (!it->mouse_cursor) return;

   if (VIEW(it)) elm_wdg_item_cursor_unset(efl_super(eo_it, ELM_GENLIST_ITEM_CLASS));

   ELM_SAFE_FREE(it->mouse_cursor, eina_stringshare_del);
}

EAPI void
elm_genlist_item_cursor_style_set(Elm_Object_Item *eo_it,
                                  const char *style)
{
   elm_wdg_item_cursor_style_set(eo_it, style);
}

EAPI const char *
elm_genlist_item_cursor_style_get(const Elm_Object_Item *eo_it)
{
   return elm_wdg_item_cursor_style_get(eo_it);
}

EAPI void
elm_genlist_item_cursor_engine_only_set(Elm_Object_Item *eo_it,
                                        Eina_Bool engine_only)
{
   elm_wdg_item_cursor_engine_only_set(eo_it, engine_only);
}

EAPI Eina_Bool
elm_genlist_item_cursor_engine_only_get(const Elm_Object_Item *eo_it)
{
   return elm_wdg_item_cursor_engine_only_get(eo_it);
}

EOLIAN static int
_elm_genlist_item_index_get(const Eo *eo_it EINA_UNUSED, Elm_Gen_Item *it)
{
   int cnt = 1;
   Elm_Gen_Item *tmp;
   ELM_GENLIST_ITEM_CHECK_OR_RETURN(it, -1);

   EINA_INLIST_FOREACH(GL_IT(it)->wsd->items, tmp)
     {
        if (tmp == it) break;
        cnt++;
     }
   return cnt;
}

EOLIAN static void
_elm_genlist_mode_set(Eo *obj, Elm_Genlist_Data *sd, Elm_List_Mode mode)
{
   if (sd->mode == mode) return;
   sd->mode = mode;

   if (sd->mode == ELM_LIST_LIMIT)
     {
        sd->scr_minw = EINA_TRUE;
        sd->scr_minh = EINA_FALSE;
     }
   else if (sd->mode == ELM_LIST_EXPAND)
     {
        sd->scr_minw = EINA_TRUE;
        sd->scr_minh = EINA_TRUE;
     }
   else
     {
        sd->scr_minw = EINA_FALSE;
        sd->scr_minh = EINA_FALSE;
     }

   elm_layout_sizing_eval(obj);
}

EOLIAN static Elm_List_Mode
_elm_genlist_mode_get(const Eo *obj EINA_UNUSED, Elm_Genlist_Data *sd)
{
   return sd->mode;
}

EAPI void
elm_genlist_bounce_set(Evas_Object *obj,
                       Eina_Bool h_bounce,
                       Eina_Bool v_bounce)
{
   ELM_GENLIST_CHECK(obj);
   elm_interface_scrollable_bounce_allow_set(obj, h_bounce, v_bounce);
}

EOLIAN static void
_elm_genlist_elm_interface_scrollable_bounce_allow_set(Eo *obj, Elm_Genlist_Data *sd, Eina_Bool h_bounce, Eina_Bool v_bounce)
{
   sd->h_bounce = !!h_bounce;
   sd->v_bounce = !!v_bounce;
   elm_interface_scrollable_bounce_allow_set
         (efl_super(obj, MY_CLASS), sd->h_bounce, sd->v_bounce);
}

EAPI void
elm_genlist_bounce_get(const Evas_Object *obj,
                       Eina_Bool *h_bounce,
                       Eina_Bool *v_bounce)
{
   ELM_GENLIST_CHECK(obj);
   elm_interface_scrollable_bounce_allow_get
         (obj, h_bounce, v_bounce);
}

EOLIAN static void
_elm_genlist_elm_interface_scrollable_bounce_allow_get(const Eo *obj EINA_UNUSED, Elm_Genlist_Data *sd, Eina_Bool *h_bounce, Eina_Bool *v_bounce)
{
   if (h_bounce) *h_bounce = sd->h_bounce;
   if (v_bounce) *v_bounce = sd->v_bounce;
}

EOLIAN static void
_elm_genlist_homogeneous_set(Eo *obj EINA_UNUSED, Elm_Genlist_Data *sd, Eina_Bool homogeneous)
{
   sd->homogeneous = !!homogeneous;
}

EOLIAN static Eina_Bool
_elm_genlist_homogeneous_get(const Eo *obj EINA_UNUSED, Elm_Genlist_Data *sd)
{
   return sd->homogeneous;
}

EOLIAN static void
_elm_genlist_block_count_set(Eo *obj EINA_UNUSED, Elm_Genlist_Data *sd, int count)
{
   EINA_SAFETY_ON_TRUE_RETURN(count < 1);

   sd->max_items_per_block = count;
   sd->item_cache_max = sd->max_items_per_block * 2;
   _item_cache_all_free(sd);
}

EOLIAN static int
_elm_genlist_block_count_get(const Eo *obj EINA_UNUSED, Elm_Genlist_Data *sd)
{
   return sd->max_items_per_block;
}

static void
_filter_item_internal(Elm_Gen_Item *it)
{
   ELM_GENLIST_DATA_GET_FROM_ITEM(it, sd);
   if (sd->filter_data && it->itc->func.filter_get)
     {
        if (!it->itc->func.filter_get(
               (void *)WIDGET_ITEM_DATA_GET(EO_OBJ(it)),
                WIDGET(it), sd->filter_data))
          {
             it->hide = EINA_TRUE;
             if (sd->show_item == it) sd->show_item = NULL;
             GL_IT(it)->block->calc_done = EINA_FALSE;
             sd->calc_done = EINA_FALSE;
          }
        else
          sd->filtered_count++;
     }
   it->filtered = EINA_TRUE;
   sd->processed_count++;
}

static Eina_Bool
_item_filtered_get(Elm_Gen_Item *it)
{
   Eina_List *l;
   if (!it) return EINA_FALSE;
   ELM_GENLIST_DATA_GET_FROM_ITEM(it, sd);
   if (!it->filtered)
     {
        l = eina_list_data_find_list(sd->filter_queue, it);
        if (l)
          sd->filter_queue = eina_list_remove_list(sd->filter_queue, l);
        l = eina_list_data_find_list(sd->queue, it);
        if (l)
          {
             sd->queue = eina_list_remove_list(sd->queue, l);
             GL_IT(it)->queued = EINA_FALSE;
             GL_IT(it)->resized = EINA_FALSE;
             _item_process(sd, it);
          }

        _filter_item_internal(it);
        GL_IT(it)->block->calc_done = EINA_FALSE;
        sd->calc_done = EINA_FALSE;
        _changed(sd->pan_obj);
   }
   if (!it->hide) return EINA_TRUE;
   return EINA_FALSE;
}

static int
_filter_queue_process(Elm_Genlist_Data *sd)
{
   int n = 0;
   Elm_Gen_Item *it, *first;
   double t0;

   t0 = ecore_time_get();
   for (n = 0; ((sd->filter_queue) && (sd->processed_count < ITEM_QUEUE_MAX)); n++)
     {
        first = it = eina_list_data_get(sd->filter_queue);
        //FIXME: This is added as a fail safe code for items not yet processed.
        while (it && it->item->queued)
          {
             if ((ecore_time_get() - t0) > (ecore_animator_frametime_get()))
               return n;
             sd->filter_queue = eina_list_remove_list
                              (sd->filter_queue, sd->filter_queue);
             sd->filter_queue = eina_list_append(sd->filter_queue, it);
             it = eina_list_data_get(sd->filter_queue);

             //Do not iterate more than one loop
             if (it == first) return n;
          }
        sd->filter_queue = eina_list_remove_list(sd->filter_queue, sd->filter_queue);
        if (it)
          {
             _filter_item_internal(it);
             GL_IT(it)->block->calc_done = EINA_FALSE;
          }
        sd->calc_done = EINA_FALSE;
        _changed(sd->pan_obj);
        if ((ecore_time_get() - t0) > (ecore_animator_frametime_get()))
          {
             //At least 1 item is filtered by this time, so return n+1 for first loop
             n++;
             break;
          }
     }
   return n;
}

static Eina_Bool
_filter_process(void *data,
              Eina_Bool *wakeup)
{
   Elm_Genlist_Data *sd = data;

   if (_filter_queue_process(sd) > 0) *wakeup = EINA_TRUE;
   if (!sd->filter_queue) return ECORE_CALLBACK_CANCEL;
   return ECORE_CALLBACK_RENEW;
}

static Eina_Bool
_item_filter_enterer(void *data)
{
   Eina_Bool wakeup = EINA_FALSE;
   ELM_GENLIST_DATA_GET(data, sd);
   Eina_Bool ok = _filter_process(sd, &wakeup);
   if (wakeup)
     {
        if (sd->dummy_job) ecore_job_del(sd->dummy_job);
        sd->dummy_job = ecore_job_add(_dummy_job, sd);
     }
   if (ok == ECORE_CALLBACK_CANCEL)
     {
        if (sd->dummy_job)
          {
             ecore_job_del(sd->dummy_job);
             sd->dummy_job = NULL;
          }
        sd->queue_idle_enterer = NULL;
        efl_event_callback_legacy_call(sd->obj, ELM_GENLIST_EVENT_FILTER_DONE, NULL);
     }

   return ok;
}

EOLIAN void
_elm_genlist_filter_set(Eo *obj EINA_UNUSED, Elm_Genlist_Data *sd, void *filter_data)
{
   Item_Block *itb;
   Eina_List *l;
   Elm_Gen_Item *it;

   if (sd->filter_queue)
     ELM_SAFE_FREE(sd->queue_filter_enterer, ecore_idle_enterer_del);
   ELM_SAFE_FREE(sd->filter_queue, eina_list_free);
   ELM_SAFE_FREE(sd->filtered_list, eina_list_free);
   sd->filtered_count = 0;
   sd->processed_count = 0;
   sd->filter = EINA_TRUE;
   sd->filter_data = filter_data;

   EINA_INLIST_FOREACH(sd->blocks, itb)
     {
        if (itb->realized)
          {
             EINA_LIST_FOREACH(itb->items, l, it)
               {
                  it->filtered = EINA_FALSE;
                  it->hide = EINA_FALSE;
                  if (it->realized)
                    _filter_item_internal(it);
                  else
                    sd->filter_queue = eina_list_append(sd->filter_queue, it);
               }
            itb->calc_done = EINA_FALSE;
            sd->calc_done = EINA_FALSE;
         }
       else
         {
            EINA_LIST_FOREACH(itb->items, l, it)
              {
                 it->filtered = EINA_FALSE;
                 it->hide = EINA_FALSE;
                 sd->filter_queue = eina_list_append(sd->filter_queue, it);
              }
         }
     }
   _changed(sd->pan_obj);

   sd->queue_filter_enterer = ecore_idle_enterer_add(_item_filter_enterer,
                                                     sd->obj);
}

static Eina_Bool
_filter_iterator_next(Elm_Genlist_Filter *iter, void **data)
{
   if (!iter->current) return EINA_FALSE;
   Elm_Gen_Item *item;
   while (iter->current)
     {
        item = ELM_GENLIST_FILTER_ITERATOR_ITEM_GET(iter->current, Elm_Gen_Item);
        iter->current = iter->current->next;
        if (_item_filtered_get(item))
          {
             if (data)
               *data = EO_OBJ(item);
             return EINA_TRUE;
          }
     }

   return EINA_FALSE;
}

static void
_filter_iterator_free(Elm_Genlist_Filter *iter)
{
   free(iter);
}

static Evas_Object *
_filter_iterator_get_container(Elm_Genlist_Filter *iter)
{
   Elm_Gen_Item *it = ELM_GENLIST_FILTER_ITERATOR_ITEM_GET(iter->head, Elm_Gen_Item);
   return WIDGET(it);
}

EOLIAN Eina_Iterator *
_elm_genlist_filter_iterator_new(Eo *obj EINA_UNUSED, Elm_Genlist_Data *sd)
{
   Elm_Genlist_Filter *iter;
   iter = calloc(1, sizeof (Elm_Genlist_Filter));
   if (!iter) return NULL;

   iter->head = sd->items;
   iter->current = sd->items;

   iter->iterator.version = EINA_ITERATOR_VERSION;
   iter->iterator.next = FUNC_ITERATOR_NEXT(_filter_iterator_next);
   iter->iterator.get_container = FUNC_ITERATOR_GET_CONTAINER(
                                    _filter_iterator_get_container);
   iter->iterator.free = FUNC_ITERATOR_FREE(_filter_iterator_free);

   EINA_MAGIC_SET(&iter->iterator, EINA_MAGIC_ITERATOR);

   return &iter->iterator;
}

EOLIAN static unsigned int
_elm_genlist_filtered_items_count(const Eo *obj EINA_UNUSED, Elm_Genlist_Data *sd)
{
   return sd->filtered_count;
}

EOLIAN static void
_elm_genlist_longpress_timeout_set(Eo *obj EINA_UNUSED, Elm_Genlist_Data *sd, double timeout)
{
   sd->longpress_timeout = timeout;
}

EOLIAN static double
_elm_genlist_longpress_timeout_get(const Eo *obj EINA_UNUSED, Elm_Genlist_Data *sd)
{
   return sd->longpress_timeout;
}

EAPI void
elm_genlist_scroller_policy_set(Evas_Object *obj,
                                Elm_Scroller_Policy policy_h,
                                Elm_Scroller_Policy policy_v)
{
   ELM_GENLIST_CHECK(obj);
   elm_interface_scrollable_policy_set(obj, policy_h, policy_v);
}

EOLIAN static void
_elm_genlist_elm_interface_scrollable_policy_set(Eo *obj, Elm_Genlist_Data *sd EINA_UNUSED, Elm_Scroller_Policy policy_h, Elm_Scroller_Policy policy_v)
{
   ELM_GENLIST_CHECK(obj);

   if ((policy_h >= ELM_SCROLLER_POLICY_LAST) ||
       (policy_v >= ELM_SCROLLER_POLICY_LAST))
     return;

   elm_interface_scrollable_policy_set(efl_super(obj, MY_CLASS), policy_h, policy_v);
}

EAPI void
elm_genlist_scroller_policy_get(const Evas_Object *obj,
                                Elm_Scroller_Policy *policy_h,
                                Elm_Scroller_Policy *policy_v)
{
   ELM_GENLIST_CHECK(obj);
   elm_interface_scrollable_policy_get(obj, policy_h, policy_v);
}

EOLIAN static void
_elm_genlist_elm_interface_scrollable_policy_get(const Eo *obj, Elm_Genlist_Data *sd EINA_UNUSED, Elm_Scroller_Policy *policy_h, Elm_Scroller_Policy *policy_v)
{
   Elm_Scroller_Policy s_policy_h, s_policy_v;

   elm_interface_scrollable_policy_get
         (efl_super(obj, MY_CLASS), &s_policy_h, &s_policy_v);
   if (policy_h) *policy_h = (Elm_Scroller_Policy)s_policy_h;
   if (policy_v) *policy_v = (Elm_Scroller_Policy)s_policy_v;
}

EOLIAN static void
_elm_genlist_realized_items_update(Eo *obj, Elm_Genlist_Data *_pd EINA_UNUSED)
{
   Eina_List *list;
   Elm_Object_Item *eo_it;

   ELM_GENLIST_CHECK(obj);

   list = elm_genlist_realized_items_get(obj);
   EINA_LIST_FREE(list, eo_it)
     elm_genlist_item_update(eo_it);
}

EOLIAN static void
_elm_genlist_item_decorate_mode_set(Eo *eo_it EINA_UNUSED, Elm_Gen_Item *it,
                                   const char *decorate_it_type,
                                   Eina_Bool decorate_it_set)
{
   Elm_Genlist_Data *sd;

   ELM_GENLIST_ITEM_CHECK_OR_RETURN(it);
   sd = GL_IT(it)->wsd;

   if (!decorate_it_type) return;
   if (elm_wdg_item_disabled_get(eo_it)) return;
   if (sd->decorate_all_mode) return;

   if ((sd->mode_item == it) &&
       (!strcmp(decorate_it_type, sd->decorate_it_type)) &&
       (decorate_it_set))
     return;
   if (!it->itc->decorate_item_style) return;
   it->decorate_it_set = decorate_it_set;

   _item_unselect(it);
   if (((sd->decorate_it_type)
        && (strcmp(decorate_it_type, sd->decorate_it_type))) ||
       (decorate_it_set) || ((it == sd->mode_item) && (!decorate_it_set)))
     {
        char buf[1024], buf2[1024];

        elm_interface_scrollable_hold_set(sd->obj, EINA_TRUE);
        if (sd->scr_hold_timer) ecore_timer_del(sd->scr_hold_timer);
        sd->scr_hold_timer = ecore_timer_add(0.1, _scroll_hold_timer_cb, sd);

        snprintf(buf, sizeof(buf), "elm,state,%s,passive", sd->decorate_it_type);
        edje_object_signal_emit(GL_IT(it)->deco_it_view, buf, "elm");
        edje_object_signal_emit(VIEW(it), buf, "elm");

        snprintf(buf2, sizeof(buf2), "elm,state,%s,passive,finished",
                 sd->decorate_it_type);
        edje_object_signal_callback_add(GL_IT(it)->deco_it_view, buf2,
                                        "elm",
                                        _decorate_item_finished_signal_cb, it);
        sd->mode_item = NULL;
     }

   eina_stringshare_replace(&sd->decorate_it_type, decorate_it_type);
   if (decorate_it_set)
     {
        sd->mode_item = it;
        elm_interface_scrollable_hold_set(sd->obj, EINA_TRUE);
        if (sd->scr_hold_timer) ecore_timer_del(sd->scr_hold_timer);
        sd->scr_hold_timer = ecore_timer_add(0.1, _scroll_hold_timer_cb, sd);

        _decorate_item_realize(it);
     }
}

EOLIAN static const char *
_elm_genlist_item_decorate_mode_get(const Eo *eo_i EINA_UNUSED, Elm_Gen_Item *i)
{
   ELM_GENLIST_ITEM_CHECK_OR_RETURN(i, NULL);
   return GL_IT(i)->wsd->decorate_it_type;
}

EOLIAN static Elm_Object_Item *
_elm_genlist_decorated_item_get(const Eo *obj EINA_UNUSED, Elm_Genlist_Data *sd)
{
   return EO_OBJ(sd->mode_item);
}

EOLIAN static Eina_Bool
_elm_genlist_decorate_mode_get(const Eo *obj EINA_UNUSED, Elm_Genlist_Data *sd)
{
   return sd->decorate_all_mode;
}

EOLIAN static void
_elm_genlist_decorate_mode_set(Eo *obj, Elm_Genlist_Data *sd, Eina_Bool decorated)
{
   Elm_Object_Item *eo_it;
   Eina_List *list;

   decorated = !!decorated;
   if (sd->decorate_all_mode == decorated) return;
   // decorate_all_mode should be set first
   // because content_get func. will be called after this
   // and user can check whether deocrate_all_mode is enabeld.
   sd->decorate_all_mode = decorated;

   list = elm_genlist_realized_items_get(obj);
   EINA_LIST_FREE(list, eo_it)
     {
        ELM_GENLIST_ITEM_DATA_GET(eo_it, it);

        if (!sd->decorate_all_mode)
          {
             _decorate_all_item_unrealize(it);
          }
        else if (it->itc->decorate_all_item_style)
          {
             _decorate_all_item_realize(it, EINA_TRUE);
          }
     }
   _changed(sd->pan_obj);
}

EOLIAN static void
_elm_genlist_reorder_mode_set(Eo *obj EINA_UNUSED, Elm_Genlist_Data *sd, Eina_Bool reorder_mode)
{
   Eina_List *list;
   Elm_Object_Item *eo_it;

   if (sd->reorder_mode == !!reorder_mode) return;
   sd->reorder_mode = !!reorder_mode;

   list = elm_genlist_realized_items_get(obj);
   EINA_LIST_FREE(list, eo_it)
     {
        ELM_GENLIST_ITEM_DATA_GET(eo_it, it);
        if (sd->reorder_mode)
          {
             edje_object_signal_emit
                (VIEW(it), SIGNAL_REORDER_MODE_SET, "elm");
             if (it->deco_all_view)
                edje_object_signal_emit
                   (it->deco_all_view, SIGNAL_REORDER_MODE_SET, "elm");
          }
        else
          {
             edje_object_signal_emit
                (VIEW(it), SIGNAL_REORDER_MODE_UNSET, "elm");
             if (it->deco_all_view)
                edje_object_signal_emit
                   (it->deco_all_view, SIGNAL_REORDER_MODE_UNSET, "elm");
          }
     }
}

EOLIAN static Eina_Bool
_elm_genlist_reorder_mode_get(const Eo *obj EINA_UNUSED, Elm_Genlist_Data *sd)
{
   return sd->reorder_mode;
}

EOLIAN static Elm_Genlist_Item_Type
_elm_genlist_item_type_get(const Eo *eo_it EINA_UNUSED, Elm_Gen_Item *it)
{
   ELM_GENLIST_ITEM_CHECK_OR_RETURN(it, ELM_GENLIST_ITEM_MAX);

   return GL_IT(it)->type;
}

EOLIAN static void
_elm_genlist_item_pin_set(Eo *eo_it EINA_UNUSED, Elm_Gen_Item *it, Eina_Bool pin)
{
   //Upstream New Feature. Need to be implemented.
}

EOLIAN static Eina_Bool
_elm_genlist_item_pin_get(const Eo *eo_it EINA_UNUSED, Elm_Gen_Item *it)
{
   //Upstream New Feature. Need to be implemented.
   return EINA_FALSE;
}

EAPI Elm_Genlist_Item_Class *
elm_genlist_item_class_new(void)
{
   Elm_Genlist_Item_Class *itc = ELM_NEW(Elm_Genlist_Item_Class);
   EINA_SAFETY_ON_NULL_RETURN_VAL(itc, NULL);

   itc->version = CLASS_ALLOCATED;
   itc->refcount = 1;
   itc->delete_me = EINA_FALSE;
   // TIZEN ONLY(20160630): Support homogeneous mode in item class.
   itc->homogeneous = EINA_FALSE;
   //

   return itc;
}

EAPI void
elm_genlist_item_class_free(Elm_Genlist_Item_Class *itc)
{
   if (itc && (itc->version == CLASS_ALLOCATED))
     {
        itc->delete_me = EINA_TRUE;
        if (itc->refcount > 0) elm_genlist_item_class_unref(itc);
        else
          {
             itc->version = 0;
             free(itc);
          }
     }
}

EAPI void
elm_genlist_item_class_ref(Elm_Genlist_Item_Class *itc)
{
   if (itc && (itc->version == CLASS_ALLOCATED))
     {
        itc->refcount++;
        if (itc->refcount == 0) itc->refcount--;
     }
}

EAPI void
elm_genlist_item_class_unref(Elm_Genlist_Item_Class *itc)
{
   if (itc && (itc->version == CLASS_ALLOCATED))
     {
        if (itc->refcount > 0) itc->refcount--;
        if (itc->delete_me && (!itc->refcount))
          elm_genlist_item_class_free(itc);
     }
}

EOLIAN static void
_elm_genlist_item_flip_set(Eo *eo_it EINA_UNUSED, Elm_Gen_Item *it, Eina_Bool flip)
{
   ELM_GENLIST_ITEM_CHECK_OR_RETURN(it);

   flip = !!flip;
   if (it->flipped == flip) return;
   it->flipped = flip;

   if (it->flipped)
     {
        _item_content_realize(it, VIEW(it), &GL_IT(it)->flip_content_objs,
                                 "flips", NULL);

        edje_object_signal_emit(VIEW(it), SIGNAL_FLIP_ENABLED, "elm");
        if (GL_IT(it)->wsd->decorate_all_mode)
           edje_object_signal_emit
              (it->deco_all_view, SIGNAL_FLIP_ENABLED, "elm");
     }
   else
     {
        Evas_Object *c;
        edje_object_signal_emit(VIEW(it), SIGNAL_FLIP_DISABLED, "elm");
        if (GL_IT(it)->wsd->decorate_all_mode)
           edje_object_signal_emit
              (it->deco_all_view, SIGNAL_FLIP_DISABLED, "elm");

        EINA_LIST_FREE(GL_IT(it)->flip_content_objs, c)
           evas_object_del(c);

        // FIXME: update texts should be done by app?
        _item_text_realize(it, VIEW(it), NULL);
     }
}

EOLIAN static Eina_Bool
_elm_genlist_item_flip_get(const Eo *eo_it EINA_UNUSED, Elm_Gen_Item *it)
{
   ELM_GENLIST_ITEM_CHECK_OR_RETURN(it, EINA_FALSE);

   return it->flipped;
}

EOLIAN static void
_elm_genlist_select_mode_set(Eo *obj EINA_UNUSED, Elm_Genlist_Data *sd, Elm_Object_Select_Mode mode)
{
   if ((mode >= ELM_OBJECT_SELECT_MODE_MAX) || (sd->select_mode == mode))
     return;

   sd->select_mode = mode;

   if ((sd->select_mode == ELM_OBJECT_SELECT_MODE_NONE) ||
       (sd->select_mode == ELM_OBJECT_SELECT_MODE_DISPLAY_ONLY ))
     {
        Eina_List *l, *ll;
        Elm_Object_Item *eo_it;
        EINA_LIST_FOREACH_SAFE(sd->selected, l, ll, eo_it)
          {
             ELM_GENLIST_ITEM_DATA_GET(eo_it, it);
             _item_unselect(it);
          }
     }
   if (sd->select_mode == ELM_OBJECT_SELECT_MODE_DISPLAY_ONLY)
     {
        Elm_Gen_Item *it;
        EINA_INLIST_FOREACH(sd->items, it)
          {
             if (!GL_IT(it)->block) continue;
             GL_IT(it)->calc_done = EINA_FALSE;
             GL_IT(it)->block->calc_done = EINA_FALSE;
             sd->calc_done = EINA_FALSE;
             _changed(sd->pan_obj);
          }
     }
}

EOLIAN static Elm_Object_Select_Mode
_elm_genlist_select_mode_get(const Eo *obj EINA_UNUSED, Elm_Genlist_Data *sd)
{
   return sd->select_mode;
}

EOLIAN static void
_elm_genlist_highlight_mode_set(Eo *obj EINA_UNUSED, Elm_Genlist_Data *sd, Eina_Bool highlight)
{
   sd->highlight = !!highlight;
}

EOLIAN static Eina_Bool
_elm_genlist_highlight_mode_get(const Eo *obj EINA_UNUSED, Elm_Genlist_Data *sd)
{
   return sd->highlight;
}

EOLIAN static void
_elm_genlist_item_select_mode_set(Eo *eo_it EINA_UNUSED, Elm_Gen_Item *it,
                                 Elm_Object_Select_Mode mode)
{
   ELM_GENLIST_ITEM_CHECK_OR_RETURN(it);

   if ((mode >= ELM_OBJECT_SELECT_MODE_MAX) || (it->select_mode == mode))
     return;

   it->select_mode = mode;

   if ((it->select_mode == ELM_OBJECT_SELECT_MODE_NONE) ||
       (it->select_mode == ELM_OBJECT_SELECT_MODE_DISPLAY_ONLY ))
      _item_unselect(it);

   if (GL_IT(it)->block && it->select_mode == ELM_OBJECT_SELECT_MODE_DISPLAY_ONLY)
     {
        GL_IT(it)->calc_done = EINA_FALSE;
        GL_IT(it)->block->calc_done = EINA_FALSE;
        GL_IT(it)->wsd->calc_done = EINA_FALSE;
        _changed(GL_IT(it)->wsd->pan_obj);
     }
}

EOLIAN static Elm_Object_Select_Mode
_elm_genlist_item_select_mode_get(const Eo *eo_it EINA_UNUSED, Elm_Gen_Item *it)
{
   ELM_GENLIST_ITEM_CHECK_OR_RETURN(it, ELM_OBJECT_SELECT_MODE_MAX);

   return it->select_mode;
}

EOLIAN Efl_Access_State_Set
_elm_genlist_item_efl_access_object_state_set_get(const Eo *eo_it, Elm_Gen_Item *it EINA_UNUSED)
{
   Elm_Atspi_State_Set ret;
   Eina_Bool sel;

   ret = efl_access_object_state_set_get(efl_super(eo_it, ELM_GENLIST_ITEM_CLASS));

   sel = elm_obj_genlist_item_selected_get(eo_it);

   STATE_TYPE_SET(ret, ELM_ATSPI_STATE_SELECTABLE);

   if (sel)
      STATE_TYPE_SET(ret, ELM_ATSPI_STATE_SELECTED);

   if (elm_genlist_item_type_get(eo_it) == ELM_GENLIST_ITEM_TREE)
     {
        STATE_TYPE_SET(ret, ELM_ATSPI_STATE_EXPANDABLE);
        if (elm_genlist_item_expanded_get(eo_it))
           STATE_TYPE_SET(ret, ELM_ATSPI_STATE_EXPANDED);
        //TIZEN_ONLY(20160606): In order to distinguish group item(TREE/GROUP) added STATE_TYPE_SET
        else
           STATE_TYPE_SET(ret, ELM_ATSPI_STATE_COLLAPSED);
        //
     }
   //TIZEN_ONLY(20160608): In order to distinguish group item(TREE/GROUP) added STATE_TYPE_SET
   else if (elm_genlist_item_type_get(eo_it) == ELM_GENLIST_ITEM_GROUP)
     {
        if (elm_genlist_item_expanded_get(eo_it))
            STATE_TYPE_SET(ret, ELM_ATSPI_STATE_EXPANDED);
         else
            STATE_TYPE_SET(ret, ELM_ATSPI_STATE_COLLAPSED);
     }
   //

   return ret;
}

EOLIAN const char*
_elm_genlist_item_efl_access_object_i18n_name_get(const Eo *eo_it, Elm_Gen_Item *it)
{
   const char *ret;
   Eina_Strbuf *buf;
   char *accessible_name;
   ret = efl_access_object_i18n_name_get(efl_super(eo_it, ELM_GENLIST_ITEM_CLASS));
   if (ret) return ret;

   buf = eina_strbuf_new();

   if (it->itc->func.text_get)
     {
        Eina_List *texts;
        const char *key;

        texts =
           elm_widget_stringlist_get(edje_object_data_get(VIEW(it), "texts"));

        EINA_LIST_FREE(texts, key)
          {
             char *str_markup = it->itc->func.text_get
                ((void *)WIDGET_ITEM_DATA_GET(EO_OBJ(it)), WIDGET(it), key);
             char *str_utf8 = _elm_util_mkup_to_text(str_markup);
             free(str_markup);
             if (str_utf8)
               {
                  if (eina_strbuf_length_get(buf) > 0) eina_strbuf_append(buf, ", ");
                  eina_strbuf_append(buf, str_utf8);
                  free(str_utf8);
               }
          }
     }

   accessible_name = eina_strbuf_string_steal(buf);
   eina_strbuf_free(buf);

   eina_stringshare_del(it->base->accessible_name);
   it->base->accessible_name = eina_stringshare_add(accessible_name);
   free(accessible_name);
   return it->base->accessible_name;
}

EOLIAN Eina_List*
_elm_genlist_item_efl_access_object_access_children_get(const Eo *eo_it EINA_UNUSED, Elm_Gen_Item *it)
{
   Eina_List *ret = NULL;
   if (VIEW(it))
     {
        Eina_List *parts;
        const char *key;
        parts = elm_widget_stringlist_get(edje_object_data_get(VIEW(it), "contents"));

        EINA_LIST_FREE(parts, key)
          {
             Evas_Object *part;
             part = edje_object_part_swallow_get(VIEW(it), key);
             if (part && efl_isa(part, EFL_ACCESS_OBJECT_MIXIN))
               {
                 ret = eina_list_append(ret, part);
                 efl_parent_set(part, eo_it);
                 //TIZEN_ONLY(20181024): Fix parent-children incosistencies in atspi tree
                 efl_access_object_access_parent_set(part, eo_it);
                 //
               }
          }
     }
   return ret;
}

///////////////////////////////////////////////////////////////////////////////

EOLIAN static void
_elm_genlist_tree_effect_enabled_set(Eo *obj EINA_UNUSED, Elm_Genlist_Data *sd EINA_UNUSED, Eina_Bool enabled EINA_UNUSED)
{
   // Need to implemented
   //sd->tree_effect_enabled = !!enabled;
}

EOLIAN static Eina_Bool
_elm_genlist_tree_effect_enabled_get(const Eo *obj EINA_UNUSED, Elm_Genlist_Data *sd EINA_UNUSED)
{
   return EINA_FALSE;
}

EOLIAN static void
_elm_genlist_focus_on_selection_set(Eo *obj EINA_UNUSED, Elm_Genlist_Data *sd EINA_UNUSED, Eina_Bool enabled EINA_UNUSED)
{
   // Need to Implemented
   sd->focus_on_selection_enabled = !!enabled;
}

EOLIAN static Eina_Bool
_elm_genlist_focus_on_selection_get(const Eo *obj EINA_UNUSED, Elm_Genlist_Data *sd EINA_UNUSED)
{
   // Need to Implemented
   return sd->focus_on_selection_enabled;
}

EAPI Elm_Object_Item *
elm_genlist_nth_item_get(const Evas_Object *obj, unsigned int nth)
{
   Elm_Gen_Item *it = NULL;
   Eina_Accessor *a;
   void *data;

   ELM_GENLIST_CHECK(obj) NULL;
   ELM_GENLIST_DATA_GET(obj, sd);

   if (!sd->items) return NULL;

   a = eina_inlist_accessor_new(sd->items);
   if (!a) return NULL;
   if (eina_accessor_data_get(a, nth, &data))
     it = ELM_GEN_ITEM_FROM_INLIST(data);
   eina_accessor_free(a);
   return EO_OBJ(it);
}

EAPI void
elm_genlist_fx_mode_set(Evas_Object *obj, Eina_Bool mode)
{
   ELM_GENLIST_CHECK(obj);
   ELM_GENLIST_DATA_GET(obj, sd);

   sd->fx_mode = mode;
   return;
}

EAPI Eina_Bool
elm_genlist_fx_mode_get(const Evas_Object *obj)
{
   ELM_GENLIST_CHECK(obj) EINA_FALSE;
   ELM_GENLIST_DATA_GET(obj, sd);

   return sd->fx_mode;
}

EAPI void
elm_genlist_item_hide_set(const Elm_Object_Item *eo_item EINA_UNUSED, Eina_Bool hide EINA_UNUSED)
{
   WRN("This API is deprecated, please use filter_set instead of this");
}

EAPI Eina_Bool
elm_genlist_item_hide_get(const Elm_Object_Item *eo_item EINA_UNUSED)
{
   WRN("This API is deprecated, please use filter_set instead of this");
   return EINA_FALSE;
}

///////////////////////////////////////////////////////////////////////////////
//TIZEN ONLY:(20150126): its not decided whether deprecated or not to give away
///////////////////////////////////////////////////////////////////////////////
EAPI void
elm_genlist_realization_mode_set(Evas_Object *obj, Eina_Bool realization_mode)
{
   ELM_GENLIST_CHECK(obj);
   ELM_GENLIST_DATA_GET(obj, sd);

   if (sd->realization_mode == realization_mode) return;
   sd->realization_mode = realization_mode;
}

EAPI Eina_Bool
elm_genlist_realization_mode_get(Evas_Object *obj)
{
   ELM_GENLIST_CHECK(obj) EINA_FALSE;
   ELM_GENLIST_DATA_GET(obj, sd);

   return sd->realization_mode;
}
///////////////////////////////////////////////////////////////////////////////

EAPI void
elm_genlist_item_reorder_start(Elm_Object_Item *eo_item)
{
   ELM_GENLIST_ITEM_DATA_GET(eo_item, it);
   ELM_GENLIST_ITEM_CHECK_OR_RETURN(it);
   Elm_Genlist_Data *sd = GL_IT(it)->wsd;

   sd->reorder.it = it;

   elm_interface_scrollable_hold_set(sd->obj, EINA_TRUE);
   elm_interface_scrollable_bounce_allow_set
         (sd->obj, EINA_FALSE, EINA_FALSE);
   sd->reorder_force = EINA_TRUE;
}

EAPI void
elm_genlist_item_reorder_stop(Elm_Object_Item *eo_item)
{
   ELM_GENLIST_ITEM_DATA_GET(eo_item, it);
   ELM_GENLIST_ITEM_CHECK_OR_RETURN(it);
   Elm_Genlist_Data *sd = GL_IT(it)->wsd;

   sd->reorder_force = EINA_FALSE;
}

EOLIAN static void
_elm_genlist_class_constructor(Efl_Class *klass)
{
   if (_elm_config->access_mode)
      _elm_genlist_smart_focus_next_enable = EINA_TRUE;

   evas_smart_legacy_type_register(MY_CLASS_NAME_LEGACY, klass);
}

EOLIAN const Efl_Access_Action_Data *
_elm_genlist_efl_access_widget_action_elm_actions_get(const Eo *obj EINA_UNUSED, Elm_Genlist_Data *pd EINA_UNUSED)
{
/* Need to be implemented
   static Efl_Access_Action_Data atspi_actions[] = {
          { "move,prior", "move", "prior", _key_action_move},
          { "move,next", "move", "next", _key_action_move},
          { "move,left", "move", "left", _key_action_move},
          { "move,right", "move", "right", _key_action_move},
          { "move,up", "move", "up", _key_action_move},
          { "move,up,multi", "move", "up_multi", _key_action_move},
          { "move,down", "move", "down", _key_action_move},
          { "move,down,multi", "move", "down_multi", _key_action_move},
          { "move,first", "move", "first", _key_action_move},
          { "move,last", "move", "last", _key_action_move},
          { "select", "select", NULL, _key_action_select},
          { "select,multi", "select", "multi", _key_action_select},
          { "escape", "escape", NULL, _key_action_escape},
          { NULL, NULL, NULL, NULL }
   };
          { NULL, NULL, NULL, NULL }
   };
   return &atspi_actions[0];
*/
   static Efl_Access_Action_Data atspi_actions[] = {{NULL, NULL, NULL, NULL}};
   return &atspi_actions[0];
}


EOLIAN Eina_List*
_elm_genlist_efl_access_object_access_children_get(const Eo *obj, Elm_Genlist_Data *sd)
{
   Eina_List *ret = NULL, *ret2 = NULL, *ret3;
   Elm_Gen_Item *it;

   EINA_INLIST_FOREACH(sd->items, it)
   {
     if (efl_access_object_access_parent_get(EO_OBJ(it)) == obj)
       ret = eina_list_append(ret, EO_OBJ(it));
   }

   ret2 = efl_access_object_access_children_get(efl_super(obj, ELM_GENLIST_CLASS));

   Eo *eo;
   EINA_LIST_FOREACH(ret2, ret3, eo)
   {
     if (efl_access_object_access_parent_get(eo) == obj)
       ret = eina_list_append(ret, eo);
   }

  return ret;
}

EOLIAN Efl_Access_State_Set
_elm_genlist_efl_access_object_state_set_get(const Eo *obj, Elm_Genlist_Data *sd EINA_UNUSED)
{
   Efl_Access_State_Set ret;

   ret = efl_access_object_state_set_get(efl_super(obj, ELM_GENLIST_CLASS));

   STATE_TYPE_SET(ret, EFL_ACCESS_STATE_TYPE_MANAGES_DESCENDANTS);

   if (elm_genlist_multi_select_get(obj))
     STATE_TYPE_SET(ret, EFL_ACCESS_STATE_TYPE_MULTISELECTABLE);

   if (elm_genlist_reorder_mode_get(obj))
     STATE_TYPE_SET(ret, EFL_ACCESS_STATE_TYPE_ANIMATED);

   return ret;
}

EOLIAN int
_elm_genlist_efl_access_selection_selected_children_count_get(const Eo *objm EINA_UNUSED, Elm_Genlist_Data *pd)
{
   return eina_list_count(pd->selected);
}

EOLIAN Eo*
_elm_genlist_efl_access_selection_selected_child_get(const Eo *obj EINA_UNUSED, Elm_Genlist_Data *pd, int child_idx)
{
   return eina_list_nth(pd->selected, child_idx);
}

EOLIAN Eina_Bool
_elm_genlist_efl_access_selection_child_select(Eo *obj EINA_UNUSED, Elm_Genlist_Data *pd, int child_index)
{
   Elm_Gen_Item *item;
   if (pd->select_mode != ELM_OBJECT_SELECT_MODE_NONE)
     {
        EINA_INLIST_FOREACH(pd->items, item)
          {
             if (child_index-- == 0)
               {
                  elm_genlist_item_selected_set(EO_OBJ(item), EINA_TRUE);
                  return EINA_TRUE;
               }
          }
     }
   return EINA_FALSE;
}

EOLIAN Eina_Bool
_elm_genlist_efl_access_selection_selected_child_deselect(Eo *obj EINA_UNUSED, Elm_Genlist_Data *pd, int child_index)
{
   Eo *item;
   Eina_List *l;

   EINA_LIST_FOREACH(pd->selected, l, item)
     {
        if (child_index-- == 0)
          {
             elm_genlist_item_selected_set(item, EINA_FALSE);
             return EINA_TRUE;
          }
     }
   return EINA_FALSE;
}

EOLIAN Eina_Bool
_elm_genlist_efl_access_selection_is_child_selected(Eo *obj EINA_UNUSED, Elm_Genlist_Data *pd, int child_index)
{
   Elm_Gen_Item *item;

   EINA_INLIST_FOREACH(pd->items, item)
     {
        if (child_index-- == 0)
          {
             return elm_genlist_item_selected_get(EO_OBJ(item));
          }
     }
   return EINA_FALSE;
}

EOLIAN Eina_Bool
_elm_genlist_efl_access_selection_all_children_select(Eo *obj, Elm_Genlist_Data *pd)
{
   Elm_Gen_Item *item;

   if (!elm_genlist_multi_select_get(obj))
     return EINA_FALSE;

   EINA_INLIST_FOREACH(pd->items, item)
      elm_genlist_item_selected_set(EO_OBJ(item), EINA_TRUE);

   return EINA_TRUE;
}

EOLIAN Eina_Bool
_elm_genlist_efl_access_selection_access_selection_clear(Eo *obj EINA_UNUSED, Elm_Genlist_Data *pd)
{
   return _all_items_deselect(pd);
}

EOLIAN Eina_Bool
_elm_genlist_efl_access_selection_child_deselect(Eo *obj EINA_UNUSED, Elm_Genlist_Data *pd, int child_index)
{
   Elm_Gen_Item *item;
   if (pd->select_mode != ELM_OBJECT_SELECT_MODE_NONE)
     {
        EINA_INLIST_FOREACH(pd->items, item)
          {
             if (child_index-- == 0)
               {
                  elm_genlist_item_selected_set(EO_OBJ(item), EINA_FALSE);
                  return EINA_TRUE;
               }
          }
     }
   return EINA_FALSE;
}

EOLIAN static Efl_Object*
_elm_genlist_efl_object_provider_find(const Eo *obj, Elm_Genlist_Data *pd, const Efl_Object *klass)
{
   if (klass == EFL_UI_FOCUS_PARENT_PROVIDER_INTERFACE)
     return pd->provider;
   return efl_provider_find(efl_super(obj, ELM_GENLIST_CLASS), klass);
}

EOLIAN static void
_elm_genlist_efl_ui_focus_composition_prepare(Eo *obj, Elm_Genlist_Data *pd)
{
   Elm_Gen_Item *item;
   Eina_List *order = NULL;

   EINA_INLIST_FOREACH(pd->items, item)
     {
        if (item->base->disabled)
          continue;

        order = eina_list_append(order, item->base->eo_obj);
     }

   efl_ui_focus_composition_elements_set(obj, order);

   EINA_INLIST_FOREACH(pd->items, item)
     {
        if (item->base->disabled)
          continue;

        efl_ui_focus_object_prepare_logical(item->base->eo_obj);
     }

}

EOLIAN static void
_elm_genlist_item_efl_ui_focus_object_setup_order_non_recursive(Eo *obj, Elm_Gen_Item *pd)
{
   Eina_List *n;
   Efl_Ui_Widget*wid;

   if (elm_widget_is_legacy(WIDGET(pd))) return;
//   _item_realize(pd, EINA_FALSE);

   EINA_LIST_FOREACH(pd->contents, n, wid)
     {
        if (efl_isa(wid, EFL_UI_WIDGET_CLASS))
          _elm_widget_full_eval(wid);
     }

   efl_ui_focus_object_setup_order_non_recursive(efl_super(obj, ELM_GENLIST_ITEM_CLASS));
}

EOLIAN static Eina_Bool
_elm_genlist_efl_ui_widget_focus_state_apply(Eo *obj, Elm_Genlist_Data *pd EINA_UNUSED, Efl_Ui_Widget_Focus_State current_state, Efl_Ui_Widget_Focus_State *configured_state, Efl_Ui_Widget *redirect EINA_UNUSED)
{
   return efl_ui_widget_focus_state_apply(efl_super(obj, MY_CLASS), current_state, configured_state, obj);
}

// TIZEN only (20150914) : Accessibility: updated highlight change during genlist and list scroll
static int _is_item_in_viewport(int viewport_y, int viewport_h, int obj_y, int obj_h)
{
   if (obj_y + obj_h <= viewport_y)
     return 1;
   else if (obj_y >= viewport_y + viewport_h)
     return -1;
   return 0;
}

EOLIAN static void
_elm_genlist_elm_interface_scrollable_content_pos_set(Eo *obj, Elm_Genlist_Data *sd, Evas_Coord x, Evas_Coord y, Eina_Bool sig)
{
   if (!_elm_atspi_enabled())
     {
       elm_interface_scrollable_content_pos_set(efl_super(obj, MY_CLASS), x, y, sig);
       return;
     }

   int old_x, old_y, delta_y;
   elm_interface_scrollable_content_pos_get(efl_super(obj, MY_CLASS), &old_x, &old_y);
   elm_interface_scrollable_content_pos_set(efl_super(obj, MY_CLASS), x, y, sig);
   delta_y = old_y - y;

   //check if highlighted item is genlist descendant
   Evas_Object * highlighted_obj = _elm_object_accessibility_currently_highlighted_get();
   if (efl_isa(highlighted_obj, ELM_WIDGET_ITEM_CLASS))
     {
        Elm_Widget_Item_Data *id = efl_data_scope_get(highlighted_obj, ELM_WIDGET_ITEM_CLASS);
        highlighted_obj = id->view;
     }

   Evas_Object * parent = highlighted_obj;
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
        evas_object_geometry_get(obj, &obj_x, &obj_y, &w, &h);

        evas_object_geometry_get(highlighted_obj, &hx, &hy, &hw, &hh);

        Elm_Gen_Item * next_previous_item = NULL;
        // TIZEN_ONLY(20180326) : Atspi: enhance finding next and prev item on screen's edge
        /* FIXME : the content_pos_set of genlist differs by 1 pixel from the final call when scrolled up and down.
                   If the item size is 360x360 on a 360x360 size screen, scrolling upward (0,360) to (0, 0)
                   will be called until the y coordinate of the highlight object in content_pos_set is 1.
                   But downward is not same. if scrolling downward (0,0) to (0,360)
                   will be called y coordinate of highlight object in final called content_pos_set is 360.
                   This part can be modified according to the content_pos_set call convention.
        */
        if(sd->scroll_delta_y_backup < 0)
          hy = hy - 1;
        //
        int viewport_position_result = _is_item_in_viewport(obj_y, h, hy, hh);
        //only highlight if move direction is correct
        //sometimes highlighted item is brought in and it does not fit viewport
        //however content goes to the viewport position so soon it will
        //meet _is_item_in_viewport condition
        if ((viewport_position_result < 0 && (delta_y != 0 ? delta_y > 0 : sd->scroll_delta_y_backup > 0)) ||
           (viewport_position_result > 0 && (delta_y != 0 ? delta_y < 0 : sd->scroll_delta_y_backup < 0)))
          {

             Eina_List *realized_items = elm_genlist_realized_items_get(obj);
             Eo *item;
             Eina_List *l;
             Eina_Bool traverse_direction = viewport_position_result > 0;
             l = traverse_direction ? realized_items: eina_list_last(realized_items);

             while(l)
               {
                  item = eina_list_data_get(l);
                  ELM_GENLIST_ITEM_DATA_GET(item, it_data);
                  next_previous_item = ELM_GEN_ITEM_FROM_INLIST(EINA_INLIST_GET(it_data));
                  evas_object_geometry_get(VIEW(next_previous_item), &hx, &hy, &hw, &hh);
                  //FIXME : the content_pos_set of genlist differs by 1 pixel from the final call when scrolled up and down.
                  if(sd->scroll_delta_y_backup < 0)
                    hy = hy - 1;
                  if (_is_item_in_viewport(obj_y, h, hy, hh) == 0)
                    break;

                  next_previous_item = NULL;

                  l = traverse_direction ? eina_list_next(l): eina_list_prev(l);
              }
          }
        if (next_previous_item)
          {
             elm_object_accessibility_highlight_set(EO_OBJ(next_previous_item), EINA_TRUE);
             efl_access_state_changed_signal_emit(EO_OBJ(next_previous_item), ELM_ATSPI_STATE_HIGHLIGHTED, EINA_TRUE);
          }
     }
   // TIZEN_ONLY(20180326) : Atspi: enhance finding next and prev item on screen's edge
   /* FIXME : delta_y means the direction of the scroll. However, old_y and y do not have the proper orientation
              when content_pos_set is called twice. Therefore, we use delta_y_backup temporarily.
   */
   sd->scroll_delta_y_backup = delta_y;
   //
}
// Tizen only (20150914)

EOLIAN static Eina_Bool
_elm_genlist_item_efl_access_component_highlight_grab(Eo *eo_it, Elm_Gen_Item *it)
{
   ELM_GENLIST_DATA_GET(WIDGET(it), sd);

   efl_access_component_highlight_grab(efl_super(EO_OBJ(it), ELM_GENLIST_ITEM_CLASS));

   //TIZEN_ONLY(20170412) Make atspi,(un)highlighted work on widget item
   // If you call efl_super, then you do NOT have to call smart callback.
   evas_object_smart_callback_call(WIDGET(it), "atspi,highlighted", EO_OBJ(it));
   //

   if (!VIEW(it))
     {
        if (!TIZEN_PROFILE_WEARABLE)
          {
             //TIZEN_ONLY(20170724): grab highlight using unrealized item
             elm_genlist_item_bring_in(eo_it, ELM_GENLIST_ITEM_SCROLLTO_IN);
             //
          }
     }

  //TIZEN_ONLY(20170412) Make atspi,(un)highlighted work on widget item
  // If you call efl_super, then you do NOT have to call smart callback.
  evas_object_smart_callback_call(WIDGET(it), "atspi,highlighted", eo_it);
  //
  return EINA_TRUE;
}

EOLIAN static Eina_Bool
_elm_genlist_item_efl_access_component_highlight_clear(Eo *eo_it, Elm_Gen_Item *it)
{
  Eina_Bool ret;
  ELM_GENLIST_DATA_GET(WIDGET(it), sd);
  if (sd->atspi_item_to_highlight == it)
      sd->atspi_item_to_highlight = NULL;

  efl_access_active_descendant_changed_signal_emit(WIDGET(it), eo_it);

  ret = efl_access_component_highlight_clear(efl_super(eo_it, ELM_GENLIST_ITEM_CLASS));
  //TIZEN_ONLY(20170412) Make atspi,(un)highlighted work on widget item
  // If you call efl_super, then you do NOT have to call smart callback.
  evas_object_smart_callback_call(WIDGET(it), "atspi,unhighlighted", eo_it);
  //
  return ret;
}
//

EOLIAN static Efl_Ui_Focus_Object*
_elm_genlist_item_efl_ui_focus_object_focus_parent_get(const Eo *obj EINA_UNUSED, Elm_Gen_Item *pd)
{
   if (!pd->item->block) return NULL;
   return pd->item->block->adapter;
}


/* Standard widget overrides */

ELM_WIDGET_KEY_DOWN_DEFAULT_IMPLEMENT(elm_genlist, Elm_Genlist_Data)

/* Internal EO APIs and hidden overrides */

#define ELM_GENLIST_EXTRA_OPS \
   ELM_LAYOUT_SIZING_EVAL_OPS(elm_genlist), \
   EFL_CANVAS_GROUP_ADD_DEL_OPS(elm_genlist)

#define ELM_GENLIST_PAN_EXTRA_OPS \
   EFL_CANVAS_GROUP_DEL_OPS(elm_genlist_pan)

#include "../elementary/elm_genlist_eo.c"
#include "../elementary/elm_genlist_pan_eo.c"
#include "../elementary/elm_genlist_item_eo.c"
