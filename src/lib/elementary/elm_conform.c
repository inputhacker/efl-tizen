#ifdef HAVE_CONFIG_H
# include "elementary_config.h"
#endif

#define ELM_WIDGET_PROTECTED
#define EFL_ACCESS_OBJECT_PROTECTED
#define ELM_LAYOUT_PROTECTED

#include <Elementary.h>
#include "elm_priv.h"
#include "elm_widget_conform.h"
#include "elm_widget_layout.h"
#include "elm_widget_plug.h"
#include "elm_scroller.eo.h"
#include "elm_genlist.eo.h"
#include "elm_conformant.eo.h"

#define MY_CLASS ELM_CONFORMANT_CLASS
#define MY_CLASS_PFX elm_conformant
#define MY_CLASS_NAME "Elm_Conformant"
#define MY_CLASS_NAME_LEGACY "elm_conformant"

static char CONFORMANT_KEY[] = "_elm_conform_key";

#define ELM_CONFORM_INDICATOR_TIME 1.0

static const char INDICATOR_PART[] = "elm.swallow.indicator";
static const char VIRTUALKEYPAD_PART[] = "elm.swallow.virtualkeypad";
static const char CLIPBOARD_PART[] = "elm.swallow.clipboard";
static const char SOFTKEY_PART[] = "elm.swallow.softkey";

static const char SIG_VIRTUALKEYPAD_STATE_ON[] = "virtualkeypad,state,on";
static const char SIG_VIRTUALKEYPAD_STATE_OFF[] = "virtualkeypad,state,off";
static const char SIG_VIRTUALKEYPAD_SIZE_CHANGED[] = "virtualkeypad,size,changed";
static const char SIG_CLIPBOARD_STATE_ON[] = "clipboard,state,on";
static const char SIG_CLIPBOARD_STATE_OFF[] = "clipboard,state,off";
//TIZEN_ONLY(20161213): apply screen_reader_changed callback
static const char SIG_ATSPI_SCREEN_READER_CHANGED[] = "atspi,screen,reader,changed";
//

static const Evas_Smart_Cb_Description _smart_callbacks[] = {
   {SIG_VIRTUALKEYPAD_STATE_ON, ""},
   {SIG_VIRTUALKEYPAD_STATE_OFF, ""},
   {SIG_VIRTUALKEYPAD_SIZE_CHANGED, ""},
   {SIG_CLIPBOARD_STATE_ON, ""},
   {SIG_CLIPBOARD_STATE_OFF, ""},
   //TIZEN_ONLY(20161213): apply screen_reader_changed callback
   {SIG_ATSPI_SCREEN_READER_CHANGED, ""},
   //
   {NULL, NULL}
};

static const Elm_Layout_Part_Alias_Description _content_aliases[] =
{
   {"default", "elm.swallow.content"},
   {"icon", "elm.swallow.content"}, // TODO: deprecate this in elm 2.0
   {NULL, NULL}
};

// TIZEN_ONLY(20160218): Improve launching performance.
static Evas_Object *_precreated_conform_obj = NULL;
//

/* Example of env vars:
 * ILLUME_KBD="0, 0, 800, 301"
 * ILLUME_IND="0, 0, 800, 32"
 * ILLUME_STK="0, 568, 800, 32"
 */

// TIZEN_ONLY(20161208): Support quickpanel callback
static void _indicator_hide_effect_del(void *data);
static void _indicator_hide_effect_set(void *data, double duration);
static void _indicator_show_effect(Evas_Object *conformant, double duration);

static int conform_mod_init_cnt = 0;
static Elm_Conformant_Mod_Api *qp_mod = NULL;

static void
_conform_mod_init(void)
{
   Elm_Module *mod = NULL;

   if (conform_mod_init_cnt++ > 0) return;

   if(!(mod = _elm_module_find_as("quickpanel_callback/api"))) goto on_init_fail;

   if (!mod->init_func) goto on_init_fail;
   mod->init_func(mod);

   mod->api = malloc(sizeof(Elm_Conformant_Mod_Api));
   if (!mod->api) goto on_init_fail;


   ((Elm_Conformant_Mod_Api *)(mod->api))->quickpanel_callback_add =
         _elm_module_symbol_get(mod, "module_quickpanel_status_callback_add");

   ((Elm_Conformant_Mod_Api *)(mod->api))->quickpanel_event_visible_get =
         _elm_module_symbol_get(mod, "module_quickpanel_event_visible_get");

   qp_mod = mod->api;
   return;

   on_init_fail:
   conform_mod_init_cnt--;
}

static void
_quickpanel_visibility_changed_cb(int type EINA_UNUSED, void *event_info, void *user_data)
{
   Evas_Object *conform = user_data;
   Elm_Win_Indicator_Mode indmode = ELM_WIN_INDICATOR_UNKNOWN;
   Elm_Win_Indicator_Opacity_Mode ind_o_mode = ELM_WIN_INDICATOR_OPACITY_UNKNOWN;
   int visible = -1;

   ELM_CONFORMANT_DATA_GET_OR_RETURN(conform, sd);

   indmode = elm_win_indicator_mode_get(sd->win);
   ind_o_mode = elm_win_indicator_opacity_get(sd->win);

   if(qp_mod && qp_mod->quickpanel_event_visible_get)
      qp_mod->quickpanel_event_visible_get(event_info, &visible);

   if (sd->quickpanel_state == visible) return;
   sd->quickpanel_state = visible;

   if ((visible == CONFORM_QUICKPANEL_STATE_VISIBILITY_SHOWN) ||
       (visible == CONFORM_QUICKPANEL_STATE_VISIBILITY_PARTIALY_SHOWN))
     {
        elm_object_signal_emit(conform, "elm,state,quickpanel,visible", "elm");
     }
   else if( visible == CONFORM_QUICKPANEL_STATE_VISIBILITY_HIDDEN)
     {
        elm_object_signal_emit(conform, "elm,state,quickpanel,hidden", "elm");
     }

   // Actual manipulation works only when transparent indicator is showing
   if (!((indmode == ELM_WIN_INDICATOR_SHOW) && (ind_o_mode == ELM_WIN_INDICATOR_TRANSPARENT)))
     {
        if ((sd->rot == 0) || (sd->rot == 180))
          return;
     }

   if ((visible == CONFORM_QUICKPANEL_STATE_VISIBILITY_SHOWN) ||
       (visible == CONFORM_QUICKPANEL_STATE_VISIBILITY_PARTIALY_SHOWN))
     {
        _indicator_hide_effect_del(conform);
     }
   else if( visible == CONFORM_QUICKPANEL_STATE_VISIBILITY_HIDDEN)
     {
        _indicator_hide_effect_set(conform, 0.5);
     }
}
// END

#ifdef HAVE_ELEMENTARY_X
static Eina_Bool
_conformant_part_geometry_get_from_env(const char *part,
                                       int *sx,
                                       int *sy,
                                       int *sw,
                                       int *sh)
{
   const char delimiters[] = " ,;";
   char *env_val, *token;
   char buf[PATH_MAX];
   int tsx, tsy, tsw;

   if (!(env_val = getenv(part))) return EINA_FALSE;

   /* strtok would modify env var if not copied to a buffer */
   strncpy(buf, env_val, sizeof(buf));
   buf[PATH_MAX - 1] = '\0';

   token = strtok(buf, delimiters);
   if (!token) return EINA_FALSE;
   tsx = atoi(token);

   token = strtok(NULL, delimiters);
   if (!token) return EINA_FALSE;
   tsy = atoi(token);

   token = strtok(NULL, delimiters);
   if (!token) return EINA_FALSE;
   tsw = atoi(token);

   token = strtok(NULL, delimiters);
   if (!token) return EINA_FALSE;
   *sh = atoi(token);

   *sx = tsx;
   *sy = tsy;
   *sw = tsw;

   return EINA_TRUE;
}
#endif

static void
_conformant_part_size_hints_set(Evas_Object *obj,
                                Evas_Object *sobj,
                                Evas_Coord sx,
                                Evas_Coord sy,
                                Evas_Coord sw,
                                Evas_Coord sh)
{
   Evas_Coord cx, cy, cw, ch;
   Evas_Coord part_height = 0, part_width = 0;
   //TIZEN_ONLY(20170405): add window's x,y postion when calculating overlapping size.
   Evas_Coord tx, ty, tw, th;
   Evas_Coord nx, ny;
   Evas_Object *top;
   //

   evas_object_geometry_get(obj, &cx, &cy, &cw, &ch);

   //TIZEN_ONLY(20170405): add window's x,y postion when calculating overlapping size.
   ELM_CONFORMANT_DATA_GET(obj, sd);
   top = elm_widget_top_get(obj);
   evas_object_geometry_get(top, &tx, &ty, &tw, &th);
   // position (tx, ty) is based on screen(absolute) coordinates,
   // but size (tw, th) is based on view port :(

   Evas_Coord ww = 0, wh = 0;
   elm_win_screen_size_get(top, NULL, NULL, &ww, &wh);

   if (sd->rot == 90)
     {
        nx = wh - ty - tw;
        ny = tx;
     }
   else if (sd->rot == 180)
     {
        nx = ww - tx - tw;
        ny = wh - ty - th;
     }
   else if (sd->rot == 270)
     {
        nx = ty;
        ny = ww - tx - th;
     }
   else
     {
        nx = tx;
        ny = ty;
     }

   cx += nx;
   cy += ny;
   //

   /* Part overlapping with conformant */
   if ((cx < (sx + sw)) && ((cx + cw) > sx)
       && (cy < (sy + sh)) && ((cy + ch) > sy))
     {
        part_height = MIN((cy + ch), (sy + sh)) - MAX(cy, sy);
        part_width = MIN((cx + cw), (sx + sw)) - MAX(cx, sx);
     }

   evas_object_size_hint_min_set(sobj, part_width, part_height);
   evas_object_size_hint_max_set(sobj, part_width, part_height);
}

static void
_conformant_part_sizing_eval(Evas_Object *obj,
                             Conformant_Part_Type part_type)
{
#ifdef HAVE_ELEMENTARY_X
   Ecore_X_Window zone = 0;
   Ecore_X_Window xwin;
#endif
   // TIZEN_ONLY(20150707): elm_conform for wayland, and signal if parts are changed
#ifdef HAVE_ELEMENTARY_WL2
   Ecore_Wl2_Window *wlwin;
#endif
   //
   Evas_Object *top = NULL;
   int sx = -1, sy = -1, sw = -1, sh = -1;

   ELM_CONFORMANT_DATA_GET(obj, sd);

   top = elm_widget_top_get(obj);

#ifdef HAVE_ELEMENTARY_X
   xwin = elm_win_xwindow_get(top);

   if (xwin)
     zone = ecore_x_e_illume_zone_get(xwin);
#endif

   if (part_type & ELM_CONFORMANT_INDICATOR_PART)
     {
#ifdef HAVE_ELEMENTARY_X
        if ((!_conformant_part_geometry_get_from_env
               ("ILLUME_IND", &sx, &sy, &sw, &sh)) && (xwin))
          {
             //No information of the indicator geometry, reset the geometry.
             if ((!zone) ||
                 (!ecore_x_e_illume_indicator_geometry_get
                   (zone, &sx, &sy, &sw, &sh)))
               sx = sy = sw = sh = 0;
          }
#endif
        // TIZEN_ONLY(20150707): elm_conform for wayland, and signal if parts are changed
#ifdef HAVE_ELEMENTARY_WL2
//TIZEN_ONLY(20171208): fix build break by opensource temporarily until migration finish
        wlwin = (Ecore_Wl2_Window *)elm_win_wl_window_get(top);
//TIZEN_ONLY(20171208)
        if (wlwin)
          ecore_wl2_window_indicator_geometry_get(wlwin, &sx, &sy, &sw, &sh);
#endif
//
        // TIZEN_ONLY(20170228): Remove legacy to coraborate with current indicator properly
        //if (((sd->rot == 90) || (sd->rot == 270)) && sd->landscape_indicator)
        //  _conformant_part_size_hints_set(obj, sd->landscape_indicator, sx, sy, sw, sh);
        //else if (((sd->rot == 0) || (sd->rot == 180)) && sd->portrait_indicator)
        //  _conformant_part_size_hints_set(obj, sd->portrait_indicator, sx, sy, sw, sh);
        _conformant_part_size_hints_set(obj, sd->indicator, sx, sy, sw, sh);
        //END
     }

   if (part_type & ELM_CONFORMANT_VIRTUAL_KEYPAD_PART)
     {
#ifdef HAVE_ELEMENTARY_X
        if ((!_conformant_part_geometry_get_from_env
               ("ILLUME_KBD", &sx, &sy, &sw, &sh)) && (xwin))
          {
             //No information of the keyboard geometry, reset the geometry.
             DBG("[KEYPAD]:xwin=0x%x, zone=0x%x: no env value and check window property.", xwin, zone);
             if (!ecore_x_e_illume_keyboard_geometry_get(xwin, &sx, &sy, &sw, &sh))
               {
                  DBG("[KEYPAD]:no window property, check zone property.");
                  if ((!zone) ||
                      (!ecore_x_e_illume_keyboard_geometry_get(zone, &sx, &sy, &sw, &sh)))
                    {
                       DBG("[KEYPAD]:no zone property, reset value.");
                       sx = sy = sw = sh = 0;
                    }
               }
          }
#endif
// TIZEN_ONLY(20150707): elm_conform for wayland, and signal if parts are changed
#ifdef HAVE_ELEMENTARY_WL2
        int tx = -1, ty = -1, tw = -1, th = -1;
//TIZEN_ONLY(20171208): fix build break by opensource temporarily until migration finish
        wlwin = (Ecore_Wl2_Window *)elm_win_wl_window_get(top);
//TIZEN_ONLY(20171208)
        if (wlwin)
          ecore_wl2_window_keyboard_geometry_get(wlwin, &tx, &ty, &tw, &th);
        Evas_Coord ww = 0, wh = 0;
        elm_win_screen_size_get(top, NULL, NULL, &ww, &wh);

        if (sd->rot == 90)
          {
             sx = wh - ty - th;
             sy = tx;
             sw = th;
             sh = tw;
          }
        else if (sd->rot == 180)
          {
             sx = ww - tx - tw;
             sy = wh - ty - th;
             sw = tw;
             sh = th;
          }
        else if (sd->rot == 270)
          {
             sx = ty;
             sy = ww - tx - tw;
             sw = th;
             sh = tw;
          }
        else
          {
             sx = tx;
             sy = ty;
             sw = tw;
             sh = th;
          }
#endif
        // END of TIZEN_ONLY(20150707)

        DBG("[KEYPAD]: size(%d,%d, %dx%d).", sx, sy, sw, sh);
        if (_elm_atspi_enabled())
          {
             Evas_Object *access;
             access = evas_object_data_get(sd->virtualkeypad, "_part_access_obj");
             if (access)
               efl_access_bounds_changed_signal_emit(access, sx, sy, sw, sh);
          }
        if (sd->virtualkeypad)
          {
             Eina_Rectangle rect;

             _conformant_part_size_hints_set
                (obj, sd->virtualkeypad, sx, sy, sw, sh);
             rect.x = sx; rect.y = sy; rect.w = sw; rect.h = sh;
             evas_object_smart_callback_call(obj, SIG_VIRTUALKEYPAD_SIZE_CHANGED, (void *)&rect);
          }
     }

   if (part_type & ELM_CONFORMANT_SOFTKEY_PART)
     {
#ifdef HAVE_ELEMENTARY_X
        if ((!_conformant_part_geometry_get_from_env
               ("ILLUME_STK", &sx, &sy, &sw, &sh)) && (xwin))
          {
             //No information of the softkey geometry, reset the geometry.
             if ((!zone) ||
                 (!ecore_x_e_illume_softkey_geometry_get
                     (zone, &sx, &sy, &sw, &sh)))
               sx = sy = sw = sh = 0;
          }
#endif
        if (sd->softkey)
          _conformant_part_size_hints_set(obj, sd->softkey, sx, sy, sw, sh);
     }
   if (part_type & ELM_CONFORMANT_CLIPBOARD_PART)
     {
#ifdef HAVE_ELEMENTARY_X
        if ((!_conformant_part_geometry_get_from_env
               ("ILLUME_CB", &sx, &sy, &sw, &sh)) && (xwin))
          {
             //No information of the clipboard geometry, reset the geometry.
             if ((!zone) ||
                 (!ecore_x_e_illume_clipboard_geometry_get
                   (zone, &sx, &sy, &sw, &sh)))
               sx = sy = sw = sh = 0;
          }
#endif
        if (sd->clipboard)
          _conformant_part_size_hints_set(obj, sd->clipboard, sx, sy, sw, sh);
     }
}

//TIZEN_ONLY(20160822): When atspi mode is dynamically switched on/off,
//register/unregister access objects accordingly.
static void
_atspi_expose_keypad_area(Evas_Object *obj, Eina_Bool is_atspi)
{
   ELM_CONFORMANT_DATA_GET(obj, sd);
   Evas_Object *access;

   if (sd->virtualkeypad)
     {
        if (is_atspi)
          {
             access = elm_access_object_register(sd->virtualkeypad, obj);
             efl_access_object_role_set(access, EFL_ACCESS_ROLE_INPUT_METHOD_WINDOW);
          }
        else
          elm_access_object_unregister(sd->virtualkeypad);
     }
}
//

//TIZEN_ONLY(20160822): When atspi mode is dynamically switched on/off,
//register/unregister access objects accordingly.
// TIZEN_ONLY(20170516): connect to at-spi dbus based on org.a11y.Status.IsEnabled property
EOLIAN static void
_elm_conformant_efl_ui_widget_screen_reader(Eo *obj, Elm_Conformant_Data *_pd EINA_UNUSED, Eina_Bool is_screen_reader)
{
   _atspi_expose_keypad_area(obj, is_screen_reader);
   //TIZEN_ONLY(20161213): apply screen_reader_changed callback
   evas_object_smart_callback_call(obj, SIG_ATSPI_SCREEN_READER_CHANGED, &is_screen_reader);
   //
}
//
//

static void
_conformant_parts_swallow(Evas_Object *obj)
{
   Evas *e;
   ELM_CONFORMANT_DATA_GET(obj, sd);
   ELM_WIDGET_DATA_GET_OR_RETURN(obj, wd);
   // TIZEN_ONLY(20160509): support different thing in Tizen 3.0
   Elm_Win_Keyboard_Mode mode;
   //
   e = evas_object_evas_get(obj);

   sd->scroller = NULL;

   //Virtual Keyboard
   if (edje_object_part_exists(wd->resize_obj, VIRTUALKEYPAD_PART))
     {
        // TIZEN_ONLY(20160509): support different thing in Tizen 3.0
        mode = elm_win_keyboard_mode_get(sd->win);
        //
        if (!sd->virtualkeypad)
          {
             sd->virtualkeypad = evas_object_rectangle_add(e);
             elm_widget_sub_object_add(obj, sd->virtualkeypad);
             evas_object_size_hint_max_set(sd->virtualkeypad, -1, 0);
             //TIZEN ONLY(20160628): expose virtual keypad rect as at-spi object
             // TIZEN_ONLY(20170516): connect to at-spi dbus based on org.a11y.Status.IsEnabled property
             if (elm_atspi_bridge_utils_is_screen_reader_enabled())
               _atspi_expose_keypad_area(obj, EINA_TRUE);
             //
             //
          }
        // TIZEN_ONLY(20160509): support different thing in Tizen 3.0
        else if (mode == ELM_WIN_KEYBOARD_ON)
        //
          _conformant_part_sizing_eval(obj, ELM_CONFORMANT_VIRTUAL_KEYPAD_PART);

        evas_object_color_set(sd->virtualkeypad, 0, 0, 0, 0);
        elm_layout_content_set(obj, VIRTUALKEYPAD_PART, sd->virtualkeypad);
     }
   else
     ELM_SAFE_FREE(sd->virtualkeypad, evas_object_del);

   //Clipboard
   if (edje_object_part_exists(wd->resize_obj, CLIPBOARD_PART))
     {
        if (!sd->clipboard)
          {
             sd->clipboard = evas_object_rectangle_add(e);
             evas_object_size_hint_min_set(sd->clipboard, -1, 0);
             evas_object_size_hint_max_set(sd->clipboard, -1, 0);
          }
        else
          _conformant_part_sizing_eval(obj, ELM_CONFORMANT_CLIPBOARD_PART);

        evas_object_color_set(sd->clipboard, 0, 0, 0, 0);
        elm_layout_content_set(obj, CLIPBOARD_PART, sd->clipboard);
     }
   else
     ELM_SAFE_FREE(sd->clipboard, evas_object_del);

   //Softkey
   if (edje_object_part_exists(wd->resize_obj, SOFTKEY_PART))
     {
        if (!sd->softkey)
          {
             sd->softkey = evas_object_rectangle_add(e);
             evas_object_size_hint_min_set(sd->softkey, -1, 0);
             evas_object_size_hint_max_set(sd->softkey, -1, 0);
          }
        else
          _conformant_part_sizing_eval(obj, ELM_CONFORMANT_SOFTKEY_PART);

        evas_object_color_set(sd->softkey, 0, 0, 0, 0);
        elm_layout_content_set(obj, SOFTKEY_PART, sd->softkey);
     }
   else
     ELM_SAFE_FREE(sd->softkey, evas_object_del);
}

// TIZEN_ONLY(20170228): Remove legacy to coraborate with current indicator properly
/*
static Eina_Bool
_port_indicator_connect_cb(void *data)
{
   const char   *indicator_serv_name;
   ELM_CONFORMANT_DATA_GET(data, sd);
   int rot;

   if (!sd) return ECORE_CALLBACK_CANCEL;
   if (sd->indmode != ELM_WIN_INDICATOR_SHOW)
     {
        sd->port_indi_timer = NULL;
        return ECORE_CALLBACK_CANCEL;
     }
   rot = (intptr_t) evas_object_data_get(sd->portrait_indicator, CONFORMANT_KEY);
   indicator_serv_name = elm_config_indicator_service_get(rot);
   if (!indicator_serv_name)
     {
        DBG("Conformant cannot find indicator service name: Rotation=%d\n",rot);
        sd->port_indi_timer = NULL;
        return ECORE_CALLBACK_CANCEL;
     }
   if (strchr(indicator_serv_name, '/'))
     {
        sd->port_indi_timer = NULL;
        return ECORE_CALLBACK_CANCEL;
     }
   if (elm_plug_connect(sd->portrait_indicator, indicator_serv_name, 0, EINA_FALSE))
     {
        DBG("Conformant connect to server[%s]\n", indicator_serv_name);
        sd->port_indi_timer = NULL;
        return ECORE_CALLBACK_CANCEL;
     }
   return ECORE_CALLBACK_RENEW;
}

static Eina_Bool
_land_indicator_connect_cb(void *data)
{
   const char   *indicator_serv_name;
   ELM_CONFORMANT_DATA_GET(data, sd);
   int rot;

   if (!sd) return ECORE_CALLBACK_CANCEL;
   if (sd->indmode != ELM_WIN_INDICATOR_SHOW)
     {
        sd->land_indi_timer = NULL;
        return ECORE_CALLBACK_CANCEL;
     }
   rot = (intptr_t) evas_object_data_get(sd->landscape_indicator, CONFORMANT_KEY);
   indicator_serv_name = elm_config_indicator_service_get(rot);
   if (!indicator_serv_name)
     {
        DBG("Conformant cannot find indicator service name: Rotation=%d\n",rot);
        sd->land_indi_timer = NULL;
        return ECORE_CALLBACK_CANCEL;
     }
   if (strchr(indicator_serv_name, '/'))
     {
        sd->land_indi_timer = NULL;
        return ECORE_CALLBACK_CANCEL;
     }
   if (elm_plug_connect(sd->landscape_indicator, indicator_serv_name, 0, EINA_FALSE))
     {
        DBG("Conformant connect to server[%s]\n", indicator_serv_name);
        sd->land_indi_timer = NULL;
        return ECORE_CALLBACK_CANCEL;
     }
   return ECORE_CALLBACK_RENEW;
}

static void
_land_indicator_disconnected(void *data, const Efl_Event *event EINA_UNUSED)
{
   Evas_Object *conform = data;

   ELM_CONFORMANT_DATA_GET(conform, sd);

   sd->land_indi_timer = ecore_timer_add(ELM_CONFORM_INDICATOR_TIME,
                                         _land_indicator_connect_cb, conform);
}

static void
_port_indicator_disconnected(void *data, const Efl_Event *event EINA_UNUSED)
{
   Evas_Object *conform = data;

   ELM_CONFORMANT_DATA_GET(conform, sd);

   sd->port_indi_timer = ecore_timer_add(ELM_CONFORM_INDICATOR_TIME,
                                         _port_indicator_connect_cb, conform);
}
*/
static Eina_Bool
_indicator_connect_cb(void *data)
{
   const char   *indicator_serv_name;
   ELM_CONFORMANT_DATA_GET(data, sd);

   if (!sd) return ECORE_CALLBACK_CANCEL;
   if (sd->indmode != ELM_WIN_INDICATOR_SHOW)
     {
        sd->indi_timer = NULL;
        return ECORE_CALLBACK_CANCEL;
     }
   indicator_serv_name = elm_config_indicator_service_get(0);
   if (!indicator_serv_name)
     {
        DBG("Conformant cannot find indicator service name\n");
        sd->indi_timer = NULL;
        return ECORE_CALLBACK_CANCEL;
     }
   if (strchr(indicator_serv_name, '/'))
     {
        sd->indi_timer = NULL;
        return ECORE_CALLBACK_CANCEL;
     }
   if (elm_plug_connect(sd->indicator, indicator_serv_name, 0, EINA_FALSE))
     {
        DBG("Conformant connect to server[%s]\n", indicator_serv_name);
        sd->indi_timer = NULL;
        return ECORE_CALLBACK_CANCEL;
     }
   return ECORE_CALLBACK_RENEW;
}

static void
_indicator_disconnected(void *data,
                        const Efl_Event *event EINA_UNUSED)
{
   Evas_Object *conform = data;

   ELM_CONFORMANT_DATA_GET(conform, sd);

   sd->indi_timer = ecore_timer_add(ELM_CONFORM_INDICATOR_TIME,
                                    _indicator_connect_cb, conform);
}
// END

//TIZEN_ONLY(20170104): add plug message feature.
// procotol version - change this as needed
#define MSG_DOMAIN_CONTROL_INDICATOR 0x10001
#define MSG_ID_INDICATOR_REPEAT_EVENT 0x10002
#define MSG_ID_INDICATOR_ROTATION 0x10003
#define MSG_ID_INDICATOR_OPACITY 0X1004
#define MSG_ID_INDICATOR_TYPE 0X1005
#define MSG_ID_INDICATOR_START_ANIMATION 0X10006

typedef struct _Indicator_Data_Animation Indicator_Data_Animation;

struct _Indicator_Data_Animation
{
   double         duration;
};

static void
_plug_msg_handle(void *data,
                 const Efl_Event *event)
{
   Evas_Object *conformant = data;
   Elm_Plug_Message *pm = event->info;
   int msg_domain = 0;
   int msg_id = 0;
   void *msg_data = NULL;
   int msg_data_size = 0;

   msg_domain = pm->msg_domain;
   msg_id = pm->msg_id;
   msg_data = pm->data;
   msg_data_size = pm->size;

   if (msg_domain == MSG_DOMAIN_CONTROL_INDICATOR)
     {
        if (msg_id == MSG_ID_INDICATOR_START_ANIMATION)
          {
             if (msg_data_size != (int)sizeof(Indicator_Data_Animation)) return;

             Indicator_Data_Animation *anim_data = msg_data;
             _indicator_show_effect(conformant, anim_data->duration);
          }
     }
}

// TIZEN_ONLY(20170228): Remove legacy to coraborate with current indicator properly
/*
static Evas_Object *
_create_portrait_indicator(Evas_Object *obj)
{
   Evas_Object *port_indicator = NULL;
   const char *port_indicator_serv_name;

   ELM_CONFORMANT_DATA_GET(obj, sd);

   port_indicator_serv_name = elm_config_indicator_service_get(sd->rot);
   if (!port_indicator_serv_name)
     {
        DBG("Conformant cannot get portrait indicator service name\n");
        return NULL;
     }
   if (strchr(port_indicator_serv_name, '/'))
     {
        return NULL;
     }

   port_indicator = elm_plug_add(obj);
   if (!port_indicator)
     {
        DBG("Conformant cannot create plug to server[%s]\n", port_indicator_serv_name);
        return NULL;
     }

   if (!elm_plug_connect(port_indicator, port_indicator_serv_name, 0, EINA_FALSE))
     {
        DBG("Conformant cannot connect to server[%s]\n", port_indicator_serv_name);
        sd->port_indi_timer = ecore_timer_add(ELM_CONFORM_INDICATOR_TIME,
                                          _port_indicator_connect_cb, obj);
     }

   elm_widget_sub_object_add(obj, port_indicator);
   //TIZEN_ONLY(20170104): add plug message feature.
   efl_event_callback_add
     (port_indicator, ELM_PLUG_EVENT_MESSAGE_RECEIVED, _plug_msg_handle, obj);
   //
   efl_event_callback_add
     (port_indicator, ELM_PLUG_EVENT_IMAGE_DELETED, _port_indicator_disconnected, obj);
   evas_object_size_hint_min_set(port_indicator, -1, 0);
   evas_object_size_hint_max_set(port_indicator, -1, 0);

   return port_indicator;
}

static Evas_Object *
_create_landscape_indicator(Evas_Object *obj)
{
   Evas_Object *land_indicator = NULL;
   const char *land_indicator_serv_name;

   ELM_CONFORMANT_DATA_GET(obj, sd);

   land_indicator_serv_name = elm_config_indicator_service_get(sd->rot);
   if (!land_indicator_serv_name)
     {
        DBG("Conformant cannot get portrait indicator service name\n");
        return NULL;
     }
   if (strchr(land_indicator_serv_name, '/'))
     {
        return NULL;
     }

   land_indicator = elm_plug_add(obj);
   if (!land_indicator)
     {
        DBG("Conformant cannot create plug to server[%s]\n", land_indicator_serv_name);
        return NULL;
     }

   if (!elm_plug_connect(land_indicator, land_indicator_serv_name, 0, EINA_FALSE))
     {
        DBG("Conformant cannot connect to server[%s]\n", land_indicator_serv_name);
        sd->land_indi_timer = ecore_timer_add(ELM_CONFORM_INDICATOR_TIME,
                                          _land_indicator_connect_cb, obj);
     }

   elm_widget_sub_object_add(obj, land_indicator);
   //TIZEN_ONLY(20170104): add plug message feature.
   efl_event_callback_add
     (land_indicator, ELM_PLUG_EVENT_MESSAGE_RECEIVED, _plug_msg_handle, obj);
   //
   efl_event_callback_add
     (land_indicator, ELM_PLUG_EVENT_IMAGE_DELETED, _land_indicator_disconnected, obj);
   evas_object_size_hint_min_set(land_indicator, -1, 0);
   evas_object_size_hint_max_set(land_indicator, -1, 0);
   return land_indicator;
}
*/
static Evas_Object *
_create_indicator(Evas_Object *obj)
{
   Evas_Object *indicator = NULL;
   const char *indicator_serv_name;

   ELM_CONFORMANT_DATA_GET(obj, sd);

   indicator_serv_name = elm_config_indicator_service_get(0);
   if (!indicator_serv_name)
     {
        DBG("Conformant cannot get indicator service name\n");
        return NULL;
     }
   if (strchr(indicator_serv_name, '/'))
     {
        return NULL;
     }

   indicator = elm_plug_add(obj);
   if (!indicator)
     {
        DBG("Conformant cannot create plug to server[%s]\n", indicator_serv_name);
        return NULL;
     }

   if (!elm_plug_connect(indicator, indicator_serv_name, 0, EINA_FALSE))
     {
        DBG("Conformant cannot connect to server[%s]\n", indicator_serv_name);
        sd->indi_timer = ecore_timer_add(ELM_CONFORM_INDICATOR_TIME,
                                          _indicator_connect_cb, obj);
     }

   elm_widget_sub_object_add(obj, indicator);
   //TIZEN_ONLY(20170104): add plug message feature.
   efl_event_callback_add(indicator, ELM_PLUG_EVENT_MESSAGE_RECEIVED, _plug_msg_handle, obj);
   //
   efl_event_callback_add(indicator, ELM_PLUG_EVENT_IMAGE_DELETED, _indicator_disconnected,obj);
   evas_object_size_hint_min_set(indicator, -1, 0);
   evas_object_size_hint_max_set(indicator, -1, 0);

   return indicator;
}
//END

// TIZEN_ONLY(20160801): indicator implementation
#ifdef HAVE_ELEMENTARY_WL2
static Eina_Bool
_indicator_visible_type_set(Evas_Object *obj, Eina_Bool visible)
{
   Evas_Object *top = elm_widget_top_get(obj);
   if (!top) return EINA_FALSE;

   Ecore_Wl2_Window *wlwin = elm_win_wl_window_get(top);
   if (!wlwin) return EINA_FALSE;

   if (visible)
     {
        ecore_wl2_indicator_visible_type_set(wlwin, ECORE_WL2_INDICATOR_VISIBLE_TYPE_SHOWN);
     }
   else
     {
        ecore_wl2_indicator_visible_type_set(wlwin, ECORE_WL2_INDICATOR_VISIBLE_TYPE_HIDDEN);
     }
   return EINA_TRUE;
}
#endif
static Eina_Bool
_indicator_hide_effect_done_cb(void *data)
{
   Evas_Object *conformant = data;
   ELM_CONFORMANT_DATA_GET(conformant, sd);
   DBG("[INDICATOR]Hide effect ");

   sd->on_indicator_effect = EINA_FALSE;
   sd->indicator_effect_timer = NULL;
   elm_object_signal_emit(conformant, "indicator,hide,effect", "elm");

#ifdef HAVE_ELEMENTARY_WL2
   _indicator_visible_type_set(conformant, EINA_FALSE);
#endif
   return ECORE_CALLBACK_CANCEL;
}
static void
_indicator_hide_effect_del(void *data)
{
   Evas_Object *conformant = data;
   ELM_CONFORMANT_DATA_GET(conformant, sd);

   if (sd->indicator_effect_timer)
     {
        ecore_timer_del(sd->indicator_effect_timer);
        sd->indicator_effect_timer = NULL;
     }
}
static void
_indicator_hide_effect_set(void *data, double duration)
{
   Evas_Object *conformant = data;
   ELM_CONFORMANT_DATA_GET(conformant, sd);

   _indicator_hide_effect_del(conformant);
   if (duration)
     sd->indicator_effect_timer = ecore_timer_add(duration, _indicator_hide_effect_done_cb, conformant);
}
static void
_indicator_show_effect(Evas_Object *conformant, double duration)
{
   ELM_CONFORMANT_DATA_GET(conformant, sd);
   DBG("[IND]Show effect ");

   sd->on_indicator_effect = EINA_TRUE;
   elm_object_signal_emit(conformant, "indicator,show,effect", "elm");

#ifdef HAVE_ELEMENTARY_WL2
   _indicator_visible_type_set(conformant, EINA_TRUE);
#endif
   _indicator_hide_effect_set(conformant, duration);
}
static void
_indicator_post_appearance_changed(Evas_Object *conformant)
{
   ELM_CONFORMANT_DATA_GET(conformant, sd);

   // Ignore when quickpanel is visible
   if ((sd->quickpanel_state == CONFORM_QUICKPANEL_STATE_VISIBILITY_SHOWN) ||
       (sd->quickpanel_state == CONFORM_QUICKPANEL_STATE_VISIBILITY_PARTIALY_SHOWN))
     {
        return;
     }

   _indicator_hide_effect_del(conformant);

   if((sd->indmode != ELM_WIN_INDICATOR_SHOW))
     {
#ifdef HAVE_ELEMENTARY_WL2
   _indicator_visible_type_set(conformant, EINA_FALSE);
#endif
        return;
     }

   if ( sd->rot == 90 || sd->rot == 270 )
     {
#ifdef HAVE_ELEMENTARY_WL2
   _indicator_visible_type_set(conformant, EINA_FALSE);
#endif
        return;
     }

   if ((sd->ind_o_mode == ELM_WIN_INDICATOR_OPACITY_UNKNOWN) ||
       (sd->indmode == ELM_WIN_INDICATOR_UNKNOWN))
     return;

   if (sd->ind_o_mode == ELM_WIN_INDICATOR_TRANSPARENT)
     {
#ifdef HAVE_ELEMENTARY_WL2
        _indicator_visible_type_set(conformant, EINA_FALSE);
#endif
     }
   else
     {
#ifdef HAVE_ELEMENTARY_WL2
        _indicator_visible_type_set(conformant, EINA_TRUE);
#endif
     }
}
// END
static void
_indicator_mode_set(Evas_Object *conformant, Elm_Win_Indicator_Mode indmode)
{
   Evas_Object *old_indi = NULL;
   ELM_CONFORMANT_DATA_GET(conformant, sd);
   ELM_WIDGET_DATA_GET_OR_RETURN(conformant, wd);

   sd->indmode = indmode;

   if (!edje_object_part_exists(wd->resize_obj, INDICATOR_PART))
     return;

   if (indmode == ELM_WIN_INDICATOR_SHOW)
     {
        old_indi = elm_layout_content_get(conformant, INDICATOR_PART);

        //create new indicator
        if (!old_indi)
          {
             // TIZEN_ONLY(20170228): Remove legacy to coraborate with current indicator properly
             /*
             if ((sd->rot == 90)||(sd->rot == 270))
               {
                  if (!sd->landscape_indicator)
                    sd->landscape_indicator = _create_landscape_indicator(conformant);

                  if (!sd->landscape_indicator) return;
                  elm_layout_content_set(conformant, INDICATOR_PART, sd->landscape_indicator);
               }
             else
               {
                  if (!sd->portrait_indicator)
                    sd->portrait_indicator = _create_portrait_indicator(conformant);

                  if (!sd->portrait_indicator) return;
                  elm_layout_content_set(conformant, INDICATOR_PART, sd->portrait_indicator);
               }
             */
             if (!sd->indicator)
               sd->indicator = _create_indicator(conformant);
             if (!sd->indicator) return;
               elm_layout_content_set(conformant, INDICATOR_PART, sd->indicator);
             //END

          }
        elm_object_signal_emit(conformant, "elm,state,indicator,show", "elm");
     }
   else
     elm_object_signal_emit(conformant, "elm,state,indicator,hide", "elm");
}

static void
_indicator_opacity_set(Evas_Object *conformant, Elm_Win_Indicator_Opacity_Mode ind_o_mode)
{
   ELM_CONFORMANT_DATA_GET(conformant, sd);
   sd->ind_o_mode = ind_o_mode;
   //TODO: opacity change
   //TIZEN_ONLY(20160615): Sending signal to edje with indiecator type
   DBG("[INDICATOR]The opacity mode of indicator was changed:(%d->%d) rot=%d", sd->ind_o_mode, ind_o_mode, sd->rot);
   if (ind_o_mode == ELM_WIN_INDICATOR_TRANSLUCENT)
     elm_object_signal_emit(conformant, "elm,state,indicator,translucent", "elm");
   else if (ind_o_mode == ELM_WIN_INDICATOR_TRANSPARENT)
     elm_object_signal_emit(conformant, "elm,state,indicator,transparent", "elm");
   else if (ind_o_mode == ELM_WIN_INDICATOR_BG_TRANSPARENT)
     elm_object_signal_emit(conformant, "elm,state,indicator,bg_transparent", "elm");
   else
     elm_object_signal_emit(conformant, "elm,state,indicator,opaque", "elm");
   /////////////////////////////////////////////////////////////////////////////
}

static void
_on_indicator_mode_changed(void *data, const Efl_Event *event)
{
   Evas_Object *conformant = data;
   Evas_Object *win = event->object;

   Elm_Win_Indicator_Mode indmode;
   Elm_Win_Indicator_Opacity_Mode ind_o_mode;

   ELM_CONFORMANT_DATA_GET(conformant, sd);

   indmode = elm_win_indicator_mode_get(win);
   ind_o_mode = elm_win_indicator_opacity_get(win);
   // TIZEN_ONLY(20160801):indicator implementation
   Eina_Bool dirty = ((indmode != sd->indmode) | (ind_o_mode != sd->ind_o_mode)) ; // TIZEN_ONLY(20160801):indicator implementation
   // END
   if (indmode != sd->indmode)
     _indicator_mode_set(conformant, indmode);
   if (ind_o_mode != sd->ind_o_mode)
     _indicator_opacity_set(conformant, ind_o_mode);
   // TIZEN_ONLY(20160801):indicator implementation
   if (dirty)
     _indicator_post_appearance_changed(conformant);
   // END
   return;
}

static void
_on_rotation_changed(void *data, const Efl_Event *event EINA_UNUSED)
{
   int rot = 0;
   Evas_Object *win = event->object;
   Evas_Object *conformant = data;
   /* TIZEN_ONLY(20170228): Remove legacy to coraborate with current indicator properly
    * Remove the following unused variable.
   Evas_Object *old_indi = NULL;
    */
   /* END */

   ELM_CONFORMANT_DATA_GET(data, sd);

   rot = elm_win_rotation_get(win);

   if (rot == sd->rot) return;

   sd->rot = rot;
   _indicator_post_appearance_changed(conformant); // TIZEN_ONLY(20160801):indicator implementation
   return;
   // TIZEN_ONLY(20170228): Remove legacy to coraborate with current indicator properly
   //old_indi = elm_layout_content_unset(conformant, INDICATOR_PART);
   /* this means ELM_WIN_INDICATOR_SHOW never be set.we don't need to change indicator type*/
   /*
   if (!old_indi) return EINA_TRUE;
   evas_object_hide(old_indi);

   if ((rot == 90) || (rot == 270))
     {
        if (!sd->landscape_indicator)
          sd->landscape_indicator = _create_landscape_indicator(conformant);

        if (!sd->landscape_indicator) return;

        evas_object_show(sd->landscape_indicator);
        evas_object_data_set(sd->landscape_indicator, CONFORMANT_KEY, (void *) (intptr_t) rot);
        elm_layout_content_set(conformant, INDICATOR_PART, sd->landscape_indicator);
     }
   else
     {
        if (!sd->portrait_indicator)
          sd->portrait_indicator = _create_portrait_indicator(conformant);

        if (!sd->portrait_indicator) return;

        evas_object_show(sd->portrait_indicator);
        evas_object_data_set(sd->portrait_indicator, CONFORMANT_KEY, (void *) (intptr_t) rot);
        elm_layout_content_set(conformant, INDICATOR_PART, sd->portrait_indicator);
     }
   */
   //END
}
// TIZEN_ONLY(20160801):indicator implementation
static void
_on_indicator_flick_done(void *data,
                         Evas_Object *obj EINA_UNUSED,
                         void *event_info EINA_UNUSED)
{
   Evas_Object *conformant = data;
   _indicator_show_effect(conformant, 3);
}
// END

EOLIAN static Efl_Ui_Theme_Apply
_elm_conformant_efl_ui_widget_theme_apply(Eo *obj, Elm_Conformant_Data *_pd EINA_UNUSED)
{
   Efl_Ui_Theme_Apply int_ret = EFL_UI_THEME_APPLY_FAILED;

   int_ret = efl_ui_widget_theme_apply(efl_super(obj, MY_CLASS));
   if (!int_ret) return EFL_UI_THEME_APPLY_FAILED;

   _conformant_parts_swallow(obj);

   elm_layout_sizing_eval(obj);

   return int_ret;
}

// unused now - but meant to be for making sure the focused widget is always
// visible when the vkbd comes and goes by moving the conformant obj (and thus
// its children) to  show the focused widget (and if focus changes follow)
/*
   static Evas_Object *
   _focus_object_get(const Evas_Object *obj)
   {
   Evas_Object *win, *foc;

   win = elm_widget_top_get(obj);
   if (!win) return NULL;
   foc = elm_widget_top_get(win);
   }

   static void
   _focus_object_region_get(const Evas_Object *obj, Evas_Coord *x, Evas_Coord *y, Evas_Coord *w, Evas_Coord *h)
   {
   evas_object_geometry_get(obj, x, y, w, h);
   }

   static void
   _focus_change_del(void *data, Evas_Object *obj, void *event_info)
   {
   // called from toplevel when the focused window shanges
   }

   static void
   _autoscroll_move(Evas_Object *obj)
   {
   // move conformant edje by delta to show focused widget
   }

   static void
   _autoscroll_mode_enable(Evas_Object *obj)
   {
   // called when autoscroll mode should be on - content area smaller than
   // its min size
   // 1. get focused object
   // 2. if not in visible conformant area calculate delta needed to
   //    get it in
   // 3. store delta and call _autoscroll_move() which either asanimates
   //    or jumps right there
   }

   static void
   _autoscroll_mode_disable(Evas_Object *obj)
   {
   // called when autoscroll mode should be off - set delta to 0 and
   // call _autoscroll_move()
   }
 */

static void
_move_resize_cb(void *data EINA_UNUSED,
                Evas *e EINA_UNUSED,
                Evas_Object *obj,
                void *event_info EINA_UNUSED)
{
   Conformant_Part_Type part_type;
   ELM_CONFORMANT_DATA_GET(obj, sd);
   Elm_Win_Keyboard_Mode mode;

   part_type = (ELM_CONFORMANT_INDICATOR_PART |
                ELM_CONFORMANT_SOFTKEY_PART |
                ELM_CONFORMANT_CLIPBOARD_PART);

   mode = elm_win_keyboard_mode_get(sd->win);
   if (mode == ELM_WIN_KEYBOARD_ON)
     part_type |= ELM_CONFORMANT_VIRTUAL_KEYPAD_PART;

   _conformant_part_sizing_eval(obj, part_type);
}

static void
_show_region_job(void *data)
{
   Evas_Object *focus_obj;

   ELM_CONFORMANT_DATA_GET(data, sd);

   focus_obj = elm_object_focused_object_get(data);
   if (focus_obj)
     {
        Eina_Rect r = elm_widget_focus_region_get(focus_obj);

        if (r.h < _elm_config->finger_size)
          r.h = _elm_config->finger_size;

        elm_widget_show_region_set(focus_obj, r, EINA_TRUE);
     }

   sd->show_region_job = NULL;
}

// showing the focused/important region.
static void
_on_content_resize(void *data,
                   Evas *e EINA_UNUSED,
                   Evas_Object *obj EINA_UNUSED,
                   void *event_info EINA_UNUSED)
{
   ELM_CONFORMANT_DATA_GET(data, sd);

#ifdef HAVE_ELEMENTARY_X
   if ((sd->vkb_state == ECORE_X_VIRTUAL_KEYBOARD_STATE_OFF) &&
       (sd->clipboard_state == ECORE_X_ILLUME_CLIPBOARD_STATE_OFF))
     return;
#endif

   ecore_job_del(sd->show_region_job);
   sd->show_region_job = ecore_job_add(_show_region_job, data);
}

static void
_on_top_scroller_del(void *data, const Efl_Event *event)
{
   ELM_CONFORMANT_DATA_GET(data, sd);

   if (event->object == sd->scroller)
     sd->scroller = NULL;
}

static void
_autoscroll_objects_update(void *data)
{
   Evas_Object *sub, *top_scroller = NULL;

   ELM_CONFORMANT_DATA_GET(data, sd);

   sub = elm_object_focused_object_get(data);
   //Look up for top most scroller in the focus object hierarchy
   //inside Conformant.

   while (sub)
     {
        if (efl_isa(sub, ELM_CONFORMANT_CLASS)) break;

        if (efl_isa(sub, ELM_SCROLLER_CLASS) || efl_isa(sub, ELM_GENLIST_CLASS))
          top_scroller = sub;

        sub = elm_object_parent_widget_get(sub);
     }

   //If the scroller got changed by app, replace it.
   if (top_scroller != sd->scroller)
     {
        if (sd->scroller)
          {
             evas_object_event_callback_del_full
                (sd->scroller, EVAS_CALLBACK_RESIZE, _on_content_resize, data);
             efl_event_callback_del(sd->scroller, EFL_EVENT_DEL, _on_top_scroller_del, data);
          }

        sd->scroller = top_scroller;

        if (sd->scroller)
          {
             evas_object_event_callback_add
                (sd->scroller, EVAS_CALLBACK_RESIZE, _on_content_resize, data);
             efl_event_callback_add(sd->scroller, EFL_EVENT_DEL, _on_top_scroller_del, data);
          }
     }
}

#ifdef HAVE_ELEMENTARY_X
static void
_virtualkeypad_state_change(Evas_Object *obj, Ecore_X_Event_Window_Property *ev)
{
   ELM_CONFORMANT_DATA_GET(obj, sd);

   Ecore_X_Window zone = ecore_x_e_illume_zone_get(ev->win);
   Ecore_X_Virtual_Keyboard_State state =
      ecore_x_e_virtual_keyboard_state_get(ev->win);

   DBG("[KEYPAD]:window's state win=0x%x, state=%d.", ev->win, state);
   if (state == ECORE_X_VIRTUAL_KEYBOARD_STATE_UNKNOWN)
     {
        if (zone) state = ecore_x_e_virtual_keyboard_state_get(zone);
        DBG("[KEYPAD]:zone's state zone=0x%x, state=%d.", zone, state);
     }

   if (sd->vkb_state == state) return;
   sd->vkb_state = state;

   if (state == ECORE_X_VIRTUAL_KEYBOARD_STATE_OFF)
     {
        DBG("[KEYPAD]:ECORE_X_VIRTUAL_KEYBOARD_STATE_OFF");
        evas_object_size_hint_min_set(sd->virtualkeypad, -1, 0);
        evas_object_size_hint_max_set(sd->virtualkeypad, -1, 0);
        _conformant_part_sizing_eval(obj, ELM_CONFORMANT_VIRTUAL_KEYPAD_PART);
        if (!sd->clipboard_state)
          elm_widget_display_mode_set(obj, EVAS_DISPLAY_MODE_NONE);
        efl_event_callback_legacy_call(obj, ELM_CONFORMANT_EVENT_VIRTUALKEYPAD_STATE_OFF, NULL);
     }
   else if (state == ECORE_X_VIRTUAL_KEYBOARD_STATE_ON)
     {
        DBG("[KEYPAD]:ECORE_X_VIRTUAL_KEYBOARD_STATE_ON");
        _conformant_part_sizing_eval(obj, ELM_CONFORMANT_VIRTUAL_KEYPAD_PART);
        elm_widget_display_mode_set(obj, EVAS_DISPLAY_MODE_COMPRESS);
        _autoscroll_objects_update(obj);
        efl_event_callback_legacy_call(obj, ELM_CONFORMANT_EVENT_VIRTUALKEYPAD_STATE_ON, NULL);
     }
}

static void
_clipboard_state_change(Evas_Object *obj, Ecore_X_Event_Window_Property *ev)
{
   ELM_CONFORMANT_DATA_GET(obj, sd);

   Ecore_X_Window zone = ecore_x_e_illume_zone_get(ev->win);
   Ecore_X_Illume_Clipboard_State state =
      ecore_x_e_illume_clipboard_state_get(ev->win);

   DBG("[CLIPBOARD]:window's state win=0x%x, state=%d.", ev->win, state);

   if (state == ECORE_X_ILLUME_CLIPBOARD_STATE_UNKNOWN)
     {
        state = ecore_x_e_illume_clipboard_state_get(ev->win);
        DBG("[CLIPBOARD]:zone's state zone=0x%x, state=%d.", zone, state);
     }

   if (sd->clipboard_state == state) return;
   sd->clipboard_state = state;

   if (state == ECORE_X_ILLUME_CLIPBOARD_STATE_OFF)
     {
        evas_object_size_hint_min_set(sd->clipboard, -1, 0);
        evas_object_size_hint_max_set(sd->clipboard, -1, 0);
        if (!sd->vkb_state)
          elm_widget_display_mode_set(obj, EVAS_DISPLAY_MODE_NONE);
        efl_event_callback_legacy_call(obj, ELM_CONFORMANT_EVENT_CLIPBOARD_STATE_OFF, NULL);
     }
   else if (state == ECORE_X_ILLUME_CLIPBOARD_STATE_ON)
     {
        elm_widget_display_mode_set(obj, EVAS_DISPLAY_MODE_COMPRESS);
        _autoscroll_objects_update(obj);
        efl_event_callback_legacy_call(obj, ELM_CONFORMANT_EVENT_CLIPBOARD_STATE_ON, NULL);
     }
}

static Eina_Bool
_on_prop_change(void *data,
                int type EINA_UNUSED,
                void *event)
{
   Ecore_X_Event_Window_Property *ev = event;
   Evas_Object *conformant = (Evas_Object *)data;
   ELM_CONFORMANT_DATA_GET(conformant, sd);

   if ((ev->atom == ECORE_X_ATOM_NET_WM_STATE) && !sd->win_hidden)
     {
        unsigned int i, num;
        Ecore_X_Window_State *state;
        ecore_x_netwm_window_state_get(ev->win, &state, &num);
        if (state)
          {
              for (i = 0; i < num; i++)
                {
                   if (state[i] == ECORE_X_WINDOW_STATE_HIDDEN)
                     {
                        sd->win_hidden = EINA_TRUE;

                        ecore_timer_del(sd->port_indi_timer);
                        sd->port_indi_timer = NULL;

                        ecore_timer_del(sd->land_indi_timer);
                        sd->land_indi_timer = NULL;

                        evas_object_del(sd->portrait_indicator);
                        sd->portrait_indicator = NULL;

                        evas_object_del(sd->landscape_indicator);
                        sd->landscape_indicator = NULL;

                        free(state);
                        return ECORE_CALLBACK_DONE;
                     }
                }
              free(state);
          }
     }
   else if (sd->win_hidden)
     {
        sd->win_hidden = EINA_FALSE;
        sd->indmode = elm_win_indicator_mode_get(sd->win);
        sd->ind_o_mode = elm_win_indicator_opacity_get(sd->win);

        _indicator_mode_set(conformant, sd->indmode);
        _indicator_opacity_set(conformant, sd->ind_o_mode);
     }

   if (ev->atom == ECORE_X_ATOM_E_ILLUME_ZONE)
     {
        DBG("win=0x%x, ECORE_X_ATOM_E_ILLUME_ZONE.\n", ev->win);
        Conformant_Part_Type part_type;

        part_type = (ELM_CONFORMANT_INDICATOR_PART |
                     ELM_CONFORMANT_SOFTKEY_PART |
                     ELM_CONFORMANT_VIRTUAL_KEYPAD_PART |
                     ELM_CONFORMANT_CLIPBOARD_PART);

        _conformant_part_sizing_eval(data, part_type);
     }
   else if (ev->atom == ECORE_X_ATOM_E_ILLUME_INDICATOR_GEOMETRY)
     {
        DBG("win=0x%x, ECORE_X_ATOM_E_ILLUME_INDICATOR_GEOMETRY.", ev->win);
        _conformant_part_sizing_eval(data, ELM_CONFORMANT_INDICATOR_PART);
     }
   else if (ev->atom == ECORE_X_ATOM_E_ILLUME_SOFTKEY_GEOMETRY)
     {
        DBG("win=0x%x, ECORE_X_ATOM_E_ILLUME_SOFTKEY_GEOMETRY.", ev->win);
        _conformant_part_sizing_eval(data, ELM_CONFORMANT_SOFTKEY_PART);
     }
   else if (ev->atom == ECORE_X_ATOM_E_ILLUME_KEYBOARD_GEOMETRY)
     {
        DBG("[KEYPAD]:win=0x%x, ECORE_X_ATOM_E_ILLUME_KEYBOARD_GEOMETRY.", ev->win);
        _conformant_part_sizing_eval(data, ELM_CONFORMANT_VIRTUAL_KEYPAD_PART);
     }
   else if (ev->atom == ECORE_X_ATOM_E_ILLUME_CLIPBOARD_GEOMETRY)
     {
        DBG("win=0x%x, ECORE_X_ATOM_E_ILLUME_CLIPBOARD_GEOMETRY.", ev->win);
        _conformant_part_sizing_eval(data, ELM_CONFORMANT_CLIPBOARD_PART);
     }
   else if (ev->atom == ECORE_X_ATOM_E_VIRTUAL_KEYBOARD_STATE)
     {
        DBG("[KEYPAD]:win=0x%x, ECORE_X_ATOM_E_VIRTUAL_KEYBOARD_STATE.", ev->win);
        _virtualkeypad_state_change(data, ev);
     }
   else if (ev->atom == ECORE_X_ATOM_E_ILLUME_CLIPBOARD_STATE)
     {
        DBG("win=0x%x, ECORE_X_ATOM_E_ILLUME_CLIPBOARD_STATE.", ev->win);
        _clipboard_state_change(data, ev);
     }

   return ECORE_CALLBACK_PASS_ON;
}

#endif

// TIZEN_ONLY(20150707): elm_conform for wayland, and signal if parts are changed
#ifdef HAVE_ELEMENTARY_WL2
static void
_on_conformant_changed(void *data,
                     Evas_Object *obj EINA_UNUSED,
                     void *event_info EINA_UNUSED)
{
   Efl_Ui_Win_Conformant_Property property = (Efl_Ui_Win_Conformant_Property) event_info;
   Elm_Win_Keyboard_Mode mode;

   ELM_CONFORMANT_DATA_GET(data, sd);

   /* object is already freed */
   if (!sd) return;

   mode = elm_win_keyboard_mode_get(obj);

   if (property & EFL_UI_WIN_CONFORMANT_PROPERTY_KEYBOARD_STATE)
     {
        if (mode == ELM_WIN_KEYBOARD_ON)
          {
             _conformant_part_sizing_eval(data, ELM_CONFORMANT_VIRTUAL_KEYPAD_PART);
             elm_widget_display_mode_set(data, EVAS_DISPLAY_MODE_COMPRESS);
             _autoscroll_objects_update(data);
             efl_event_callback_legacy_call(obj, ELM_CONFORMANT_EVENT_VIRTUALKEYPAD_STATE_ON, NULL);
          }
        else if (mode == ELM_WIN_KEYBOARD_OFF)
          {
             evas_object_size_hint_min_set(sd->virtualkeypad, -1, 0);
             evas_object_size_hint_max_set(sd->virtualkeypad, -1, 0);
             elm_widget_display_mode_set(data, EVAS_DISPLAY_MODE_NONE);
             efl_event_callback_legacy_call(obj, ELM_CONFORMANT_EVENT_VIRTUALKEYPAD_STATE_OFF, NULL);
             //TIZEN ONLY(20160628): update geometry of keyboard to screen-reader
             if (_elm_atspi_enabled())
               {
                  Evas_Object *access;
                  access = evas_object_data_get(sd->virtualkeypad, "_part_access_obj");
                  if (access)
                    efl_access_bounds_changed_signal_emit(access, 0, 0, 0, 0);
               }
             //
          }
     }
   if ((property & EFL_UI_WIN_CONFORMANT_PROPERTY_KEYBOARD_GEOMETRY) &&
       (mode == ELM_WIN_KEYBOARD_ON))
     {
        _conformant_part_sizing_eval(data, ELM_CONFORMANT_VIRTUAL_KEYPAD_PART);
     }
}
#endif
// END of TIZEN_ONLY(20150707)


EOLIAN static void
_elm_conformant_efl_canvas_group_group_add(Eo *obj, Elm_Conformant_Data *_pd EINA_UNUSED)
{
   efl_canvas_group_add(efl_super(obj, MY_CLASS));
   elm_widget_sub_object_parent_add(obj);
   elm_widget_can_focus_set(obj, EINA_FALSE);

   if (!elm_layout_theme_set
       (obj, "conformant", "base", elm_widget_style_get(obj)))
     CRI("Failed to set layout!");

   _conformant_parts_swallow(obj);

   evas_object_event_callback_add
     (obj, EVAS_CALLBACK_RESIZE, _move_resize_cb, obj);
   evas_object_event_callback_add
     (obj, EVAS_CALLBACK_MOVE, _move_resize_cb, obj);

   elm_layout_sizing_eval(obj);
}

EOLIAN static void
_elm_conformant_efl_canvas_group_group_del(Eo *obj, Elm_Conformant_Data *sd)
{
#ifdef HAVE_ELEMENTARY_X
   ecore_event_handler_del(sd->prop_hdl);
#endif

   ecore_job_del(sd->show_region_job);
   ecore_timer_del(sd->port_indi_timer);
   ecore_timer_del(sd->land_indi_timer);
   evas_object_del(sd->portrait_indicator);
   evas_object_del(sd->landscape_indicator);
   evas_object_del(sd->indicator);

   evas_object_data_set(sd->win, "\377 elm,conformant", NULL);

   efl_event_callback_del(sd->win, EFL_UI_WIN_EVENT_INDICATOR_PROP_CHANGED, _on_indicator_mode_changed, obj);
   efl_event_callback_del(sd->win, EFL_UI_WIN_EVENT_ROTATION_CHANGED, _on_rotation_changed, obj);

  // TIZEN_ONLY(20150707): implemented elm_win_conformant_set/get for wayland
#ifdef HAVE_ELEMENTARY_WL2
   evas_object_smart_callback_del_full
     (sd->win, "conformant,changed", _on_conformant_changed, obj);
#endif
   // END
   // TIZEN_ONLY(20160801): indicator implementation
#ifdef HAVE_ELEMENTARY_WL2
   evas_object_smart_callback_del_full
     (sd->win, "indicator,flick,done", _on_indicator_flick_done, obj);
#endif
   if (sd->indicator_effect_timer)
     {
        ecore_timer_del(sd->indicator_effect_timer);
        sd->indicator_effect_timer = NULL ;
     }
   //END
   efl_canvas_group_del(efl_super(obj, MY_CLASS));
}

EOLIAN static void
_elm_conformant_efl_ui_widget_widget_parent_set(Eo *obj, Elm_Conformant_Data *sd, Evas_Object *parent)
{
#ifdef HAVE_ELEMENTARY_X
   Evas_Object *top = elm_widget_top_get(parent);
   Ecore_X_Window xwin = elm_win_xwindow_get(parent);

   if ((xwin) && (!elm_win_inlined_image_object_get(top)))
     {

        sd->prop_hdl = ecore_event_handler_add
            (ECORE_X_EVENT_WINDOW_PROPERTY, _on_prop_change, obj);
        sd->vkb_state = ECORE_X_VIRTUAL_KEYBOARD_STATE_OFF;
        sd->clipboard_state = ECORE_X_ILLUME_CLIPBOARD_STATE_OFF;
     }
   // FIXME: get kbd region prop
#else
   (void)obj;
   (void)sd;
   (void)parent;
#endif
}

// TIZEN_ONLY(20160218): Improve launching performance.
EAPI void
elm_conformant_precreated_object_set(Evas_Object *obj)
{
   INF("Set precreated obj(%p).", obj);
   _precreated_conform_obj = obj;
}

EAPI Evas_Object *
elm_conformant_precreated_object_get(void)
{
   INF("Get precreated obj(%p).", _precreated_conform_obj);
   return _precreated_conform_obj;
}
//

EAPI Evas_Object *
elm_conformant_add(Evas_Object *parent)
{
   EINA_SAFETY_ON_NULL_RETURN_VAL(parent, NULL);
// TIZEN_ONLY(20160218): Improve launching performance.
   if (_precreated_conform_obj)
     {
        Evas_Object *par_obj = elm_widget_parent_get(_precreated_conform_obj);

        if (par_obj == parent)
          {
             Evas_Object *above_obj = evas_object_above_get(_precreated_conform_obj);
             if (above_obj)
               evas_object_raise(_precreated_conform_obj);

             Evas_Object *tmp = _precreated_conform_obj;
             _precreated_conform_obj = NULL;
             INF("Return precreated obj(%p).", tmp);
             return tmp;
          }
     }
//
   return elm_legacy_add(MY_CLASS, parent);
}

EOLIAN static Eo *
_elm_conformant_efl_object_constructor(Eo *obj, Elm_Conformant_Data *sd)
{
   obj = efl_constructor(efl_super(obj, MY_CLASS));
   efl_canvas_object_type_set(obj, MY_CLASS_NAME_LEGACY);
   evas_object_smart_callbacks_descriptions_set(obj, _smart_callbacks);
   efl_access_object_role_set(obj, EFL_ACCESS_ROLE_FILLER);

   Efl_Event event = {};
   event.object = sd->win;
   sd->win = elm_widget_top_get(obj);
//   _on_indicator_mode_changed(obj, sd->win, NULL, NULL);
//   _on_rotation_changed(obj, sd->win, NULL, NULL);

   sd->indmode = elm_win_indicator_mode_get(sd->win);
   sd->ind_o_mode = elm_win_indicator_opacity_get(sd->win);
   sd->rot = elm_win_rotation_get(sd->win);
   evas_object_data_set(sd->win, "\377 elm,conformant", obj);

   efl_event_callback_add(sd->win, EFL_UI_WIN_EVENT_INDICATOR_PROP_CHANGED, _on_indicator_mode_changed, obj);
   efl_event_callback_add(sd->win, EFL_UI_WIN_EVENT_ROTATION_CHANGED, _on_rotation_changed, obj);
   // TIZEN_ONLY(20150707): implemented elm_win_conformant_set/get for wayland
#ifdef HAVE_ELEMENTARY_WL2
   evas_object_smart_callback_add
     (sd->win, "conformant,changed", _on_conformant_changed, obj);
   // END
   // TIZEN_ONLY(20160801): indicator implementation
   evas_object_smart_callback_add
     (sd->win, "indicator,flick,done", _on_indicator_flick_done, obj);
   // END
#endif

   // TIZEN_ONLY(20161208): Support quickpanel callback
   _conform_mod_init();
   if(sd->win && qp_mod && qp_mod->quickpanel_callback_add)
     {
        qp_mod->quickpanel_callback_add
          (sd->win, CONFORM_QUICKPANEL_EVENT_VISIBILITY_CHANGED, _quickpanel_visibility_changed_cb, obj);
     }
   // END

   return obj;
}

static void
_elm_conformant_class_constructor(Efl_Class *klass)
{
   evas_smart_legacy_type_register(MY_CLASS_NAME_LEGACY, klass);
}

/* Internal EO APIs and hidden overrides */

ELM_LAYOUT_CONTENT_ALIASES_IMPLEMENT(MY_CLASS_PFX)

#define ELM_CONFORMANT_EXTRA_OPS \
   ELM_LAYOUT_CONTENT_ALIASES_OPS(MY_CLASS_PFX), \
   EFL_CANVAS_GROUP_ADD_DEL_OPS(elm_conformant)

#include "elm_conformant.eo.c"
