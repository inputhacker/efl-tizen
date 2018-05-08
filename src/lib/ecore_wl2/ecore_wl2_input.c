/* this file contains code copied from weston; the copyright notice is below */
/*
 * Copyright © 2008 Kristian Høgsberg
 * Copyright © 2012-2013 Collabora, Ltd.
 *
 * Permission is hereby granted, free of charge, to any person obtaining a
 * copy of this software and associated documentation files (the "Software"),
 * to deal in the Software without restriction, including without limitation
 * the rights to use, copy, modify, merge, publish, distribute, sublicense,
 * and/or sell copies of the Software, and to permit persons to whom the
 * Software is furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice (including the next
 * paragraph) shall be included in all copies or substantial portions of the
 * Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.  IN NO EVENT SHALL
 * THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
 * FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
 * DEALINGS IN THE SOFTWARE.
 */

#ifdef HAVE_CONFIG_H
# include <config.h>
#endif

#ifdef __linux__
# include <linux/input.h>
#else
# define BTN_LEFT 0x110
# define BTN_RIGHT 0x111
# define BTN_MIDDLE 0x112
# define BTN_SIDE 0x113
# define BTN_EXTRA 0x114
# define BTN_FORWARD 0x115
# define BTN_BACK 0x116
#endif

#include <unistd.h>
#include <sys/mman.h>
#include "ecore_wl2_private.h"
// TIZEN ONLY(20160223) : Add back/menu/home key conversion support
#include <locale.h>
//

typedef struct _Ecore_Wl2_Mouse_Down_Info
{
   EINA_INLIST;
   int device, sx, sy;
   int last_win;
   int last_last_win;
   int last_event_win;
   int last_last_event_win;
   unsigned int last_time;
   unsigned int last_last_time;
   Eina_Bool double_click : 1;
   Eina_Bool triple_click : 1;
} Ecore_Wl2_Mouse_Down_Info;

static Eina_Inlist *_ecore_wl2_mouse_down_info_list = NULL;

// TIZEN ONLY(20160223) : Add back/menu/home key conversion support
static double _tizen_api_version = 0.0;
static int _back_key_lt_24 = 0;
static int _menu_key_lt_24 = 0;
static int _home_key_lt_24 = 0;
static int _num_back_key_latest = 0;
static int _num_menu_key_latest = 0;
static int _num_home_key_latest = 0;
static xkb_keycode_t *_back_key_latest = NULL;
static xkb_keycode_t *_menu_key_latest = NULL;
static xkb_keycode_t *_home_key_latest = NULL;
//
// TIZEN_ONLY(20171107): support a tizen_keyrouter interface
static Eina_Hash *_keygrabs = NULL;
static int _ecore_wl2_keygrab_error = -1;
static struct wl_array _ecore_wl2_keygrab_result_list;
//

static void _keyboard_cb_key(void *data, struct wl_keyboard *keyboard EINA_UNUSED, unsigned int serial, unsigned int timestamp, unsigned int keycode, unsigned int state);
// TIZEN_ONLY(20171207): add functions to set client's custom cursors
static void _cb_pointer_frame(void *data, struct wl_callback *callback, unsigned int timestamp EINA_UNUSED);
//

// TIZEN ONLY(20160223) : Add back/menu/home key conversion support
static int
_ecore_wl2_input_convert_old_keys(unsigned int code)
{
   int i;

   // TIZEN ONLY(20160608) : Add option for key conversion
   const char *tmp;
   tmp = getenv("ECORE_WL_INPUT_KEY_CONVERSION_DISABLE");
   if (tmp && atoi(tmp)) return code;
   //

   for (i = 0; i < _num_back_key_latest; i++)
     {
        if (code == _back_key_latest[i])
          {
             return _back_key_lt_24;
          }
     }

   for (i=0; i<_num_menu_key_latest; i++)
     {
        if (code == _menu_key_latest[i])
          {
             return _menu_key_lt_24;
          }
     }

   for (i=0; i<_num_home_key_latest; i++)
     {
        if (code == _home_key_latest[i])
          {
             return _home_key_lt_24;
          }
     }

   return code;
}

static void
_ecore_wl2_input_key_conversion_clean_up(void)
{
   _back_key_lt_24 = 0;
   _menu_key_lt_24 = 0;
   _home_key_lt_24 = 0;

   _num_back_key_latest = 0;
   _num_menu_key_latest = 0;
   _num_home_key_latest = 0;

   if (_back_key_latest)
     {
        free(_back_key_latest);
        _back_key_latest = NULL;
     }
   if (_menu_key_latest)
     {
        free(_menu_key_latest);
        _menu_key_latest = NULL;
     }
   if (_home_key_latest)
     {
        free(_home_key_latest);
        _home_key_latest = NULL;
     }
}

static void
_ecore_wl2_input_key_conversion_set(Ecore_Wl2_Input *input)
{
   char *temp;
   xkb_keycode_t *keycodes = NULL;
   static int retry_cnt = 0;

   if ((_tizen_api_version < 0.0) || (_tizen_api_version > 0.0)) return;
   EINA_SAFETY_ON_NULL_RETURN(input);
   EINA_SAFETY_ON_NULL_RETURN(input->xkb.keymap);

   temp = getenv("TIZEN_API_VERSION");

   if (!temp)
     {
        _tizen_api_version = 0.0;
        retry_cnt++;
        if (retry_cnt > 20)
          {
             INF("No tizen api version.\n");
             _tizen_api_version = -1.0;
          }
     }
   else
     {
        setlocale(LC_NUMERIC, "C");
        _tizen_api_version = atof(temp);
        setlocale(LC_NUMERIC, "");
        INF("TIZEN_API_VERSION: %lf, Environment variable: %s\n", _tizen_api_version, temp);
        if (_tizen_api_version < 2.4)
          {
             _ecore_wl2_input_key_conversion_clean_up();

             ecore_wl2_input_keycode_from_keysym(input->xkb.keymap,
                xkb_keysym_from_name("XF86Stop", XKB_KEYSYM_NO_FLAGS), &keycodes);
             if (!keycodes)
               {
                  ERR("There is no entry available for the old name of back key. No conversion will be done for back key.\n");
               }
             else
               {
                  _back_key_lt_24 = (int)keycodes[0];
                  free(keycodes);
                  keycodes = NULL;

                  _num_back_key_latest  = ecore_wl2_input_keycode_from_keysym(input->xkb.keymap,
                    xkb_keysym_from_name("XF86Back", XKB_KEYSYM_NO_FLAGS), &_back_key_latest);
               }

             ecore_wl2_input_keycode_from_keysym(input->xkb.keymap,
                xkb_keysym_from_name("XF86Send", XKB_KEYSYM_NO_FLAGS), &keycodes);
             if (!keycodes)
               {
                  ERR("There is no entry available for the old name of back key. No conversion will be done for menu key.\n");
               }
             else
               {
                  _menu_key_lt_24 = (int)keycodes[0];
                  free(keycodes);
                  keycodes = NULL;

                  _num_menu_key_latest  = ecore_wl2_input_keycode_from_keysym(input->xkb.keymap,
                    xkb_keysym_from_name("XF86Menu", XKB_KEYSYM_NO_FLAGS), &_menu_key_latest);
               }

             ecore_wl2_input_keycode_from_keysym(input->xkb.keymap,
                xkb_keysym_from_name("XF86Phone", XKB_KEYSYM_NO_FLAGS), &keycodes);
             if (!keycodes)
               {
                  ERR("There is no entry available for the old name of back key. No conversion will be done for home key.\n");
               }
             else
               {
                  _home_key_lt_24 = (int)keycodes[0];
                  free(keycodes);
                  keycodes = NULL;

                  _num_home_key_latest  = ecore_wl2_input_keycode_from_keysym(input->xkb.keymap,
                    xkb_keysym_from_name("XF86Home", XKB_KEYSYM_NO_FLAGS), &_home_key_latest);
               }

             if ((!_back_key_lt_24) && (!_menu_key_lt_24) && (!_home_key_lt_24)) _tizen_api_version = -1.0;
          }
        else
          {
             _ecore_wl2_input_key_conversion_clean_up();
          }
     }
}
//
// TIZEN_ONLY(20171109): support a tizen_input_device_manager interface
static double
_ecore_wl2_input_touch_radius_calc(double x, double y)
{
#define PI 3.14159265358979323846
   return x*y*PI/4;
}

static void
_ecore_wl2_input_touch_axis_process(Ecore_Wl2_Input *input, int id)
{
   if (id >= ECORE_WL2_TOUCH_MAX)
      return;

   if (input->touch.last_touch_axis.radius_x)
     {
        input->touch.touch_axis[id].radius_x = input->touch.last_touch_axis.radius_x;
        input->touch.last_touch_axis.radius_x = 0.0;
     }
   if (input->touch.last_touch_axis.radius_y)
     {
        input->touch.touch_axis[id].radius_y = input->touch.last_touch_axis.radius_y;
        input->touch.last_touch_axis.radius_y = 0.0;
     }
   if (input->touch.last_touch_axis.pressure)
     {
        input->touch.touch_axis[id].pressure = input->touch.last_touch_axis.pressure;
        input->touch.last_touch_axis.pressure = 0.0;
     }
   if (input->touch.last_touch_axis.angle)
     {
        input->touch.touch_axis[id].angle = input->touch.last_touch_axis.angle;
        input->touch.last_touch_axis.angle = 0.0;
     }
}
//

static Ecore_Wl2_Mouse_Down_Info *
_ecore_wl2_input_mouse_down_info_get(int device)
{
   Eina_Inlist *l = NULL;
   Ecore_Wl2_Mouse_Down_Info *info = NULL;

   l = _ecore_wl2_mouse_down_info_list;
   EINA_INLIST_FOREACH(l, info)
     if (info->device == device) return info;

   info = calloc(1, sizeof(Ecore_Wl2_Mouse_Down_Info));
   if (!info) return NULL;

   info->device = device;
   l = eina_inlist_append(l, (Eina_Inlist *)info);
   _ecore_wl2_mouse_down_info_list = l;

   return info;
}

static Ecore_Wl2_Input_Devices *
_ecore_wl2_devices_get(const Ecore_Wl2_Input *input, int window_id)
{
   Ecore_Wl2_Input_Devices *devices;
   Eina_List *l;

   EINA_LIST_FOREACH(input->devices_list, l, devices)
     {
        if (devices->window_id == window_id)
          return devices;
     }

   return NULL;
}

static Eo *
_ecore_wl2_mouse_dev_get(Ecore_Wl2_Input *input, int window_id)
{
   Ecore_Wl2_Input_Devices *devices;
   // TIZEN_ONLY(20171109): support a tizen_input_device_manager interface

   if (input->devmgr.last_device_ptr && input->devmgr.last_device_ptr->device)
     {
        return efl_ref(input->devmgr.last_device_ptr->device);
     }
   //

   devices = _ecore_wl2_devices_get(input, window_id);
   if (devices && devices->pointer_dev)
     return efl_ref(devices->pointer_dev);

   return NULL;
}

static Eo *
_ecore_wl2_touch_dev_get(Ecore_Wl2_Input *input, int window_id)
{
   Ecore_Wl2_Input_Devices *devices;
   // TIZEN_ONLY(20171109): support a tizen_input_device_manager interface

   if (input->devmgr.last_device_touch && input->devmgr.last_device_touch->device)
     {
        return efl_ref(input->devmgr.last_device_touch->device);
     }
   //

   devices = _ecore_wl2_devices_get(input, window_id);
   if (devices && devices->touch_dev)
     return efl_ref(devices->touch_dev);

   return NULL;
}

static Eo *
_ecore_wl2_seat_dev_get(Ecore_Wl2_Input *input, int window_id)
{
   Ecore_Wl2_Input_Devices *devices;

   devices = _ecore_wl2_devices_get(input, window_id);
   if (devices)
     return efl_ref(devices->seat_dev);

   return NULL;
}

//TIZEN_ONLY(20180118): support a Ecore_Device
static void
_input_event_mouse_io_cb_free(void *data EINA_UNUSED, void *event)
{
   Ecore_Event_Mouse_IO *ev = event;
   if (ev->dev)
     efl_unref(ev->dev);
   free(ev);
}

static void
_input_event_mouse_move_cb_free(void *data EINA_UNUSED, void *event)
{
   Ecore_Event_Mouse_Move *ev = event;
   if (ev->dev)
     efl_unref(ev->dev);
   free(ev);
}

static void
_input_event_mouse_wheel_cb_free(void *data EINA_UNUSED, void *event)
{
   Ecore_Event_Mouse_Wheel *ev = event;
   if (ev->dev)
     efl_unref(ev->dev);
   free(ev);
}

static void
_input_event_mouse_button_cb_free(void *data EINA_UNUSED, void *event)
{
   Ecore_Event_Mouse_Button *ev = event;
   if (ev->dev)
     efl_unref(ev->dev);
   free(ev);
}

static void
_input_event_key_cb_free(void *data EINA_UNUSED, void *event)
{
   Ecore_Event_Key *ev = event;
   if (ev->dev)
     efl_unref(ev->dev);
   free(ev);
}
//

static void
_ecore_wl2_input_mouse_in_send(Ecore_Wl2_Input *input, Ecore_Wl2_Window *window)
{
   Ecore_Event_Mouse_IO *ev;

   ev = calloc(1, sizeof(Ecore_Event_Mouse_IO));
   if (!ev) return;

   ev->x = input->pointer.sx;
   ev->y = input->pointer.sy;
   ev->window = window->id;
   ev->event_window = window->id;
   ev->timestamp = input->timestamp;
   ev->modifiers = input->keyboard.modifiers;
   ev->dev = _ecore_wl2_mouse_dev_get(input, window->id);

   ecore_event_add(ECORE_EVENT_MOUSE_IN, ev, _input_event_mouse_io_cb_free, ev->dev);
}

static void
_ecore_wl2_input_mouse_out_send(Ecore_Wl2_Input *input, Ecore_Wl2_Window *window)
{
   Ecore_Event_Mouse_IO *ev;

   ev = calloc(1, sizeof(Ecore_Event_Mouse_IO));
   if (!ev) return;

   ev->x = input->pointer.sx;
   ev->y = input->pointer.sy;
   ev->window = window->id;
   ev->event_window = window->id;
   ev->timestamp = input->timestamp;
   ev->modifiers = input->keyboard.modifiers;
   ev->dev = _ecore_wl2_mouse_dev_get(input, window->id);

   ecore_event_add(ECORE_EVENT_MOUSE_OUT, ev, _input_event_mouse_io_cb_free, ev->dev);
}

static void
_ecore_wl2_input_mouse_move_send(Ecore_Wl2_Input *input, Ecore_Wl2_Window *window, int device)
{
   Ecore_Event_Mouse_Move *ev;
   Ecore_Wl2_Mouse_Down_Info *info;

   ev = calloc(1, sizeof(Ecore_Event_Mouse_Move));
   if (!ev) return;

   ev->window = window->id;
   ev->event_window = window->id;
   ev->timestamp = input->timestamp;
   ev->x = input->pointer.sx;
   ev->y = input->pointer.sy;
   ev->root.x = input->pointer.sx;
   ev->root.y = input->pointer.sy;
   ev->modifiers = input->keyboard.modifiers;
   ev->multi.device = device;

   // TIZEN_ONLY(20171109): support a tizen_input_device_manager interface
   if (device >= ECORE_WL2_TOUCH_MAX)
     {
        ev->multi.radius = 1;
        ev->multi.radius_x = 1;
        ev->multi.radius_y = 1;
        ev->multi.pressure = 1.0;
        ev->multi.angle = 0.0;
     }
   else
     {
        ev->multi.radius =
           _ecore_wl2_input_touch_radius_calc(input->touch.touch_axis[device].radius_x,
                                             input->touch.touch_axis[device].radius_y);
        ev->multi.radius_x = input->touch.touch_axis[device].radius_x;
        ev->multi.radius_y = input->touch.touch_axis[device].radius_y;
        ev->multi.pressure = input->touch.touch_axis[device].pressure;
        ev->multi.angle = input->touch.touch_axis[device].angle;
     }
   //

   ev->multi.x = input->pointer.sx;
   ev->multi.y = input->pointer.sy;
   ev->multi.root.x = input->pointer.sx;
   ev->multi.root.y = input->pointer.sy;

   if ((input->focus.touch) && (input->focus.touch == window))
     ev->dev = _ecore_wl2_touch_dev_get(input, window->id);
   else if ((input->focus.pointer) && (input->focus.pointer == window))
     ev->dev = _ecore_wl2_mouse_dev_get(input, window->id);

   info = _ecore_wl2_input_mouse_down_info_get(device);
   if (info)
     {
        info->sx = input->pointer.sx;
        info->sy = input->pointer.sy;
     }

   ecore_event_add(ECORE_EVENT_MOUSE_MOVE, ev, _input_event_mouse_move_cb_free, ev->dev);
}

static void
_ecore_wl2_input_mouse_wheel_send(Ecore_Wl2_Input *input, unsigned int axis, int value, unsigned int timestamp)
{
   Ecore_Event_Mouse_Wheel *ev;

   ev = calloc(1, sizeof(Ecore_Event_Mouse_Wheel));
   if (!ev) return;

   ev->timestamp = timestamp;
   ev->modifiers = input->keyboard.modifiers;
   ev->x = input->pointer.sx;
   ev->y = input->pointer.sy;
   value /= 10;

   if (axis == WL_POINTER_AXIS_VERTICAL_SCROLL)
     {
        ev->direction = 0;
        ev->z = value;
     }
   else if (axis == WL_POINTER_AXIS_HORIZONTAL_SCROLL)
     {
        ev->direction = 1;
        ev->z = value;
     }

   if (input->grab.window)
     {
        ev->window = input->grab.window->id;
        ev->event_window = input->grab.window->id;
     }
   else if (input->focus.pointer)
     {
        ev->window = input->focus.pointer->id;
        ev->event_window = input->focus.pointer->id;
        ev->dev = _ecore_wl2_mouse_dev_get(input, input->focus.pointer->id);
     }
   else if (input->focus.touch)
     {
        ev->window = input->focus.touch->id;
        ev->event_window = input->focus.touch->id;
        ev->dev = _ecore_wl2_touch_dev_get(input, input->focus.touch->id);
     }

   if (!ev->dev)
     {
        ev->dev = _ecore_wl2_mouse_dev_get(input, ev->window);
        if (!ev->dev)
          ev->dev = _ecore_wl2_touch_dev_get(input, ev->window);
     }

   ecore_event_add(ECORE_EVENT_MOUSE_WHEEL, ev, _input_event_mouse_wheel_cb_free, ev->dev);
}

static void
_ecore_wl2_input_mouse_down_send(Ecore_Wl2_Input *input, Ecore_Wl2_Window *window, int device, unsigned int button, unsigned int timestamp)
{
   Ecore_Event_Mouse_Button *ev;
   Ecore_Wl2_Mouse_Down_Info *info;

   ev = calloc(1, sizeof(Ecore_Event_Mouse_Button));
   if (!ev) return;

   if (button == BTN_LEFT)
     ev->buttons = 1;
   else if (button == BTN_MIDDLE)
     ev->buttons = 2;
   else if (button == BTN_RIGHT)
     ev->buttons = 3;
   else
     ev->buttons = button;

   ev->timestamp = timestamp;
   ev->x = input->pointer.sx;
   ev->y = input->pointer.sy;
   ev->root.x = input->pointer.sx;
   ev->root.y = input->pointer.sy;
   ev->modifiers = input->keyboard.modifiers;

   ev->double_click = 0;
   ev->triple_click = 0;

   info = _ecore_wl2_input_mouse_down_info_get(device);
   if (info)
     {
        info->sx = input->pointer.sx;
        info->sy = input->pointer.sy;
        if (info->triple_click)
          {
             info->last_win = 0;
             info->last_last_win = 0;
             info->last_event_win = 0;
             info->last_last_event_win = 0;
             info->last_time = 0;
             info->last_last_time = 0;
          }

        if (((int)(timestamp - info->last_time) <= (int)(1000 * 0.25)) &&
            ((window) && (window->id == info->last_win) &&
                (window->id == info->last_event_win)))
          {
             ev->double_click = 1;
             info->double_click = EINA_TRUE;
          }
        else
          {
             info->double_click = EINA_FALSE;
             info->triple_click = EINA_FALSE;
          }

        if (((int)(timestamp - info->last_last_time) <=
             (int)(2 * 1000 * 0.25)) &&
            ((window) && (window->id == info->last_win) &&
                (window->id == info->last_last_win) &&
                (window->id == info->last_event_win) &&
                (window->id == info->last_last_event_win)))
          {
             ev->triple_click = 1;
             info->triple_click = EINA_TRUE;
          }
        else
          info->triple_click = EINA_FALSE;
     }

   // TIZEN_ONLY(20171109): support a tizen_input_device_manager interface
   if (device >= ECORE_WL2_TOUCH_MAX)
     {
        ev->multi.radius = 1;
        ev->multi.radius_x = 1;
        ev->multi.radius_y = 1;
        ev->multi.pressure = 1.0;
        ev->multi.angle = 0.0;
     }
   else
     {
        ev->multi.radius =
           _ecore_wl2_input_touch_radius_calc(input->touch.touch_axis[device].radius_x,
                                             input->touch.touch_axis[device].radius_y);
        ev->multi.radius_x = input->touch.touch_axis[device].radius_x;
        ev->multi.radius_y = input->touch.touch_axis[device].radius_y;
        ev->multi.pressure = input->touch.touch_axis[device].pressure;
        ev->multi.angle = input->touch.touch_axis[device].angle;
     }
   //
   ev->multi.device = device;
   ev->multi.x = input->pointer.sx;
   ev->multi.y = input->pointer.sy;
   ev->multi.root.x = input->pointer.sx;
   ev->multi.root.y = input->pointer.sy;

   if (window)
     {
        ev->window = window->id;
        ev->event_window = window->id;

        if ((input->focus.touch) && (input->focus.touch == window))
          ev->dev = _ecore_wl2_touch_dev_get(input, window->id);
        else if ((input->focus.pointer) && (input->focus.pointer == window))
          ev->dev = _ecore_wl2_mouse_dev_get(input, window->id);
     }

   ecore_event_add(ECORE_EVENT_MOUSE_BUTTON_DOWN, ev,
                   _input_event_mouse_button_cb_free, ev->dev);

   if ((info) && (!info->triple_click))
     {
        info->last_last_win = info->last_win;
        info->last_win = ev->window;
        info->last_last_event_win = info->last_event_win;
        info->last_event_win = ev->window;
        info->last_last_time = info->last_time;
        info->last_time = timestamp;
     }
}

static void
_ecore_wl2_input_mouse_up_send(Ecore_Wl2_Input *input, Ecore_Wl2_Window *window, int device, unsigned int button, unsigned int timestamp)
{
   Ecore_Event_Mouse_Button *ev;
   Ecore_Wl2_Mouse_Down_Info *info;

   ev = calloc(1, sizeof(Ecore_Event_Mouse_Button));
   if (!ev) return;

   if (button == BTN_LEFT)
     ev->buttons = 1;
   else if (button == BTN_MIDDLE)
     ev->buttons = 2;
   else if (button == BTN_RIGHT)
     ev->buttons = 3;
   else
     ev->buttons = button;

   ev->timestamp = timestamp;
   ev->x = input->pointer.sx;
   ev->y = input->pointer.sy;
   ev->root.x = input->pointer.sx;
   ev->root.y = input->pointer.sy;
   ev->modifiers = input->keyboard.modifiers;

   ev->double_click = 0;
   ev->triple_click = 0;

   info = _ecore_wl2_input_mouse_down_info_get(device);
   if (info)
     {
        ev->double_click = info->double_click;
        ev->triple_click = info->triple_click;
        ev->x = info->sx;
        ev->y = info->sy;
        ev->multi.x = info->sx;
        ev->multi.y = info->sy;
     }
   else
     {
        ev->multi.x = input->pointer.sx;
        ev->multi.y = input->pointer.sy;
     }

   ev->multi.device = device;
   ev->multi.radius = 1;
   ev->multi.radius_x = 1;
   ev->multi.radius_y = 1;
   ev->multi.pressure = 1.0;
   ev->multi.angle = 0.0;
   ev->multi.root.x = input->pointer.sx;
   ev->multi.root.y = input->pointer.sy;

   ev->window = window->id;
   ev->event_window = window->id;

   if ((input->focus.touch) && (input->focus.touch == window))
     ev->dev = _ecore_wl2_touch_dev_get(input, window->id);
   else if ((input->focus.pointer) && (input->focus.pointer == window))
     ev->dev = _ecore_wl2_mouse_dev_get(input, window->id);

   ecore_event_add(ECORE_EVENT_MOUSE_BUTTON_UP, ev,
                   _input_event_mouse_button_cb_free, ev->dev);


   // TIZEN_ONLY(20171109): support a tizen_input_device_manager interface
   if (device < ECORE_WL2_TOUCH_MAX)
     {
        input->touch.touch_axis[device].radius_x = 1.0;
        input->touch.touch_axis[device].radius_y = 1.0;
        input->touch.touch_axis[device].pressure = 1.0;
        input->touch.touch_axis[device].angle = 0;
     }
   //
}

static void
_input_event_focus_cb_free(void *data, void *event)
{
   Ecore_Wl2_Event_Focus_In *ev = event;
   if (data)
     efl_unref(data);
   ecore_wl2_display_disconnect(ev->display);
   free(event);
}

static void
_ecore_wl2_input_focus_in_send(Ecore_Wl2_Window *window, Ecore_Wl2_Input *input)
{
   Ecore_Wl2_Event_Focus_In *ev;

   ev = calloc(1, sizeof(Ecore_Wl2_Event_Focus_In));
   if (!ev) return;

   ev->timestamp = input->timestamp;
   ev->window = window->id;
   ev->dev = _ecore_wl2_seat_dev_get(input, window->id);
   ev->display = input->display;
   ev->display->refs++;
   ecore_event_add(ECORE_WL2_EVENT_FOCUS_IN, ev, _input_event_focus_cb_free,
                   ev->dev);
}

static void
_ecore_wl2_input_focus_out_send(Ecore_Wl2_Window *window, Ecore_Wl2_Input *input)
{
   Ecore_Wl2_Event_Focus_Out *ev;

   ev = calloc(1, sizeof(Ecore_Wl2_Event_Focus_Out));
   if (!ev) return;

   ev->timestamp = input->timestamp;
   ev->window = window->id;
   ev->dev = _ecore_wl2_seat_dev_get(input, window->id);
   ev->display = input->display;
   ev->display->refs++;
   ecore_event_add(ECORE_WL2_EVENT_FOCUS_OUT, ev, _input_event_focus_cb_free,
                   ev->dev);
}

static int
_ecore_wl2_input_key_translate(xkb_keysym_t keysym, unsigned int modifiers, char *buffer, int bytes)
{
/* this function is copied, with slight changes in variable names, from KeyBind.c in libX11
 * the license from that file can be found below:
 */
/*

Copyright 1985, 1987, 1998  The Open Group

Permission to use, copy, modify, distribute, and sell this software and its
documentation for any purpose is hereby granted without fee, provided that
the above copyright notice appear in all copies and that both that
copyright notice and this permission notice appear in supporting
documentation.

The above copyright notice and this permission notice shall be included in
all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.  IN NO EVENT SHALL THE
OPEN GROUP BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN
AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.

Except as contained in this notice, the name of The Open Group shall not be
used in advertising or otherwise to promote the sale, use or other dealings
in this Software without prior written authorization from The Open Group.

*/
   if (!keysym) return 0;

   /* check for possible control codes */
   if (modifiers & ECORE_EVENT_MODIFIER_CTRL)
     {
        Eina_Bool valid_control_code = EINA_TRUE;
        unsigned long hbytes = 0;
        unsigned char c;

        hbytes = (keysym >> 8);
        if (!(bytes &&
        ((hbytes == 0) ||
        ((hbytes == 0xFF) &&
        (((keysym >= XKB_KEY_BackSpace) && (keysym <= XKB_KEY_Clear)) ||
        (keysym == XKB_KEY_Return) ||
        (keysym == XKB_KEY_Escape) ||
        (keysym == XKB_KEY_KP_Space) ||
        (keysym == XKB_KEY_KP_Tab) ||
        (keysym == XKB_KEY_KP_Enter) ||
        ((keysym >= XKB_KEY_KP_Multiply) && (keysym <= XKB_KEY_KP_9)) ||
        (keysym == XKB_KEY_KP_Equal) ||
        (keysym == XKB_KEY_Delete))))))
          return 0;

        if (keysym == XKB_KEY_KP_Space)
          c = (XKB_KEY_space & 0x7F);
        else if (hbytes == 0xFF)
          c = (keysym & 0x7F);
        else
          c = (keysym & 0xFF);

        /* We are building here a control code
           for more details, read:
           https://en.wikipedia.org/wiki/C0_and_C1_control_codes#C0_.28ASCII_and_derivatives.29
         */

        if (((c >= '@') && (c <= '_')) || /* those are the one defined in C0 with capital letters */
             ((c >= 'a') && (c <= 'z')) ||  /* the lowercase symbols (not part of the standard, but useful) */
              c == ' ')
          c &= 0x1F;
        else if (c == '\x7f')
          c = '\177';
        /* following codes are alternatives, they are longer here, i dont want to change them */
        else if (c == '2')
          c = '\000'; /* 0 code */
        else if ((c >= '3') && (c <= '7'))
          c -= ('3' - '\033'); /* from escape to unitseperator code*/
        else if (c == '8')
          c = '\177'; /* delete code */
        else if (c == '/')
          c = '_' & 0x1F; /* unit seperator code */
        else
          valid_control_code = EINA_FALSE;

        if (valid_control_code)
          buffer[0] = c;
        else
          return 0;
     }
   else
     {
        /* if its not a control code, try to produce useful output */
        if (!xkb_keysym_to_utf8(keysym, buffer, bytes))
          return 0;
     }

   return 1;
}

static void
_ecore_wl2_input_symbol_rep_find(xkb_keysym_t keysym, char *buffer, int size, unsigned int code)
{
    int n = 0;

    n = xkb_keysym_to_utf8(keysym, buffer, size);

    /* check if we are a control code */
    if (n > 0 && !(
        (buffer[0] > 0x0 && buffer[0] <= 0x20) || /* others 0x0 to 0x20 control codes */
        buffer[0] == 0x7F)) /*delete control code */
      return;

    if (xkb_keysym_get_name(keysym, buffer, size) != 0)
      return;

    snprintf(buffer, size, "Keycode-%u", code);
}

static Eo *
_ecore_wl2_keyboard_dev_get(Ecore_Wl2_Input *input, int window_id)
{
   Ecore_Wl2_Input_Devices *devices;
   // TIZEN_ONLY(20171109): support a tizen_input_device_manager interface

   if (input->devmgr.last_device_kbd && input->devmgr.last_device_kbd->device)
     {
        return efl_ref(input->devmgr.last_device_kbd->device);
     }
   //

   devices = _ecore_wl2_devices_get(input, window_id);
   if (devices && devices->keyboard_dev)
     return efl_ref(devices->keyboard_dev);

   return NULL;
}

static void
_ecore_wl2_input_key_send(Ecore_Wl2_Input *input, Ecore_Wl2_Window *window, xkb_keysym_t sym, xkb_keysym_t sym_name, unsigned int code, unsigned int state, unsigned int timestamp)
{
   Ecore_Event_Key *ev;
   char key[256] = "", keyname[256] = "", compose[256] = "";
   int name_len, key_len, comp_len;

   /*try to get a name or utf char of the given symbol */
   _ecore_wl2_input_symbol_rep_find(sym, key, sizeof(key), code);
   _ecore_wl2_input_symbol_rep_find(sym_name, keyname, sizeof(keyname), code);
   _ecore_wl2_input_key_translate(sym, input->keyboard.modifiers,
                                  compose, sizeof(compose));

   name_len = strlen(keyname);
   key_len = strlen(key);
   comp_len = strlen(compose);

   ev = calloc(1, sizeof(Ecore_Event_Key) + key_len + name_len + comp_len + 3);
   if (!ev) return;

   ev->keyname = (char *)(ev + 1);
   ev->key = ev->keyname + name_len + 1;
   ev->compose = comp_len ? ev->key + key_len + 1 : NULL;
   ev->string = ev->compose;

   strcpy((char *)ev->keyname, keyname);
   strcpy((char *)ev->key, key);
   if (comp_len) strcpy((char *)ev->compose, compose);

   // TIZEN_ONLY(20171107): support a tizen_keyrouter interface
   if (window)
     {
        ev->window = window->id;
        ev->event_window = window->id;
        ev->dev = _ecore_wl2_keyboard_dev_get(input, window->id);
     }
   else
     {
        ev->window = (uintptr_t)NULL;
        ev->event_window = (uintptr_t)NULL;
        ev->dev = _ecore_wl2_keyboard_dev_get(input, 0);
     }
   //
   ev->timestamp = timestamp;
   ev->modifiers = input->keyboard.modifiers;
   ev->keycode = code;

   /* DBG("Emitting Key event (%s,%s,%s,%s)\n", ev->keyname, ev->key, ev->compose, ev->string); */

   if (state)
     ecore_event_add(ECORE_EVENT_KEY_DOWN, ev, _input_event_key_cb_free, ev->dev);
   else
     ecore_event_add(ECORE_EVENT_KEY_UP, ev, _input_event_key_cb_free, ev->dev);
}

void
_ecore_wl2_input_grab(Ecore_Wl2_Input *input, Ecore_Wl2_Window *window, unsigned int button)
{
   input->grab.window = window;
   input->grab.button = button;
}

void
_ecore_wl2_input_ungrab(Ecore_Wl2_Input *input)
{
   if ((input->grab.window) && (input->grab.button) && (input->grab.count))
     _ecore_wl2_input_mouse_up_send(input, input->grab.window, 0,
                                    input->grab.button, input->grab.timestamp);

   input->grab.window = NULL;
   input->grab.button = 0;
   input->grab.count = 0;
}

static void
_pointer_cb_enter(void *data, struct wl_pointer *pointer EINA_UNUSED, unsigned int serial, struct wl_surface *surface, wl_fixed_t sx, wl_fixed_t sy)
{
   Ecore_Wl2_Input *input;
   Ecore_Wl2_Window *window;

   input = data;
   if (!input) return;

   /* trap for a surface that was just destroyed */
   if (!surface) return;

   if (!input->timestamp)
     {
        struct timeval tv;

        gettimeofday(&tv, NULL);
        input->timestamp = (tv.tv_sec * 1000 + tv.tv_usec / 1000);
     }

   input->display->serial = serial;
   input->pointer.enter_serial = serial;
   input->pointer.sx = wl_fixed_to_double(sx);
   input->pointer.sy = wl_fixed_to_double(sy);

   // TIZEN_ONLY(20171207): add functions to set client's custom cursors
   /* The cursor on the surface is undefined until we set it */
   ecore_wl2_input_cursor_from_name_set(input, input->cursor.name);
   //

   /* find the window which this surface belongs to */
   window = _ecore_wl2_display_window_surface_find(input->display, surface);
   if (!window) return;

   // TIZEN_ONLY(20171207): add functions to set client's custom cursors
   window->pointer.device = input;
   //
   input->focus.prev_pointer = NULL;
   input->focus.pointer = window;

   _ecore_wl2_input_mouse_in_send(input, window);
}

static void
_pointer_cb_leave(void *data, struct wl_pointer *pointer EINA_UNUSED, unsigned int serial, struct wl_surface *surface)
{
   Ecore_Wl2_Input *input;
   Ecore_Wl2_Window *window;

   input = data;
   if (!input) return;

   input->display->serial = serial;
   input->focus.prev_pointer = input->focus.pointer;
   input->focus.pointer = NULL;

   /* trap for a surface that was just destroyed */
   if (!surface) return;

   /* find the window which this surface belongs to */
   window = _ecore_wl2_display_window_surface_find(input->display, surface);
   if (!window) return;

   // TIZEN_ONLY(20171207): add functions to set client's custom cursors
   window->pointer.device = NULL;
   //

   _ecore_wl2_input_mouse_out_send(input, window);
}

static void
_pointer_cb_motion(void *data, struct wl_pointer *pointer EINA_UNUSED, unsigned int timestamp, wl_fixed_t sx, wl_fixed_t sy)
{
   Ecore_Wl2_Input *input;
   Ecore_Wl2_Window *window;

   input = data;
   if (!input) return;

   input->timestamp = timestamp;
   input->pointer.sx = wl_fixed_to_double(sx);
   input->pointer.sy = wl_fixed_to_double(sy);

   /* get currently focused window */
   window = input->focus.pointer;
   if (!window) return;

   /* NB: Unsure if we need this just yet, so commented out for now */
   /* if ((input->pointer.sx > window->geometry.w) || */
   /*     (input->pointer.sy > window->geometry.h)) */
   /*   return; */

   _ecore_wl2_input_mouse_move_send(input, window, 0);
}

static void
_pointer_cb_button(void *data, struct wl_pointer *pointer EINA_UNUSED, unsigned int serial, unsigned int timestamp, unsigned int button, unsigned int state)
{
   Ecore_Wl2_Input *input;

   input = data;
   if (!input) return;

   input->display->serial = serial;

   if (state == WL_POINTER_BUTTON_STATE_PRESSED)
     {
        if ((input->focus.pointer) &&
            (!input->grab.window) && (!input->grab.count))
          {
             _ecore_wl2_input_grab(input, input->focus.pointer, button);
             input->grab.timestamp = timestamp;
          }

        if (input->focus.pointer)
          _ecore_wl2_input_mouse_down_send(input, input->focus.pointer,
                                           0, button, timestamp);

        input->grab.count++;
     }
   else
     {
        if (input->focus.pointer)
          _ecore_wl2_input_mouse_up_send(input, input->focus.pointer,
                                         0, button, timestamp);

        if (input->grab.count) input->grab.count--;
        if ((input->grab.window) && (input->grab.button == button) &&
            (!input->grab.count))
          _ecore_wl2_input_ungrab(input);
     }
}

static void
_pointer_cb_axis(void *data, struct wl_pointer *pointer EINA_UNUSED, unsigned int timestamp, unsigned int axis, wl_fixed_t value)
{
   Ecore_Wl2_Input *input;

   input = data;
   if (!input) return;

   _ecore_wl2_input_mouse_wheel_send(input, axis, wl_fixed_to_int(value),
                                     timestamp);
}

static const struct wl_pointer_listener _pointer_listener =
{
   _pointer_cb_enter,
   _pointer_cb_leave,
   _pointer_cb_motion,
   _pointer_cb_button,
   _pointer_cb_axis,
   NULL, /* frame */
   NULL, /* axis_source */
   NULL, /* axis_stop */
   NULL, /* axis_discrete */
};

static void
_keyboard_cb_keymap(void *data, struct wl_keyboard *keyboard EINA_UNUSED, unsigned int format, int fd, unsigned int size)
{
   Ecore_Wl2_Input *input;
   Ecore_Wl2_Event_Seat_Keymap_Changed *ev;
   char *map = NULL;
   const char *locale;

   input = data;
   if (!input)
     {
        close(fd);
        return;
     }

   if (format != WL_KEYBOARD_KEYMAP_FORMAT_XKB_V1)
     {
        close(fd);
        return;
     }

   map = mmap(NULL, size, PROT_READ, MAP_SHARED, fd, 0);
   if (map == MAP_FAILED)
     {
        close(fd);
        return;
     }

   /* free any existing keymap and state */
   if (input->xkb.keymap) xkb_map_unref(input->xkb.keymap);
   if (input->xkb.state) xkb_state_unref(input->xkb.state);
   if (input->xkb.maskless_state) xkb_state_unref(input->xkb.maskless_state);

   input->xkb.keymap =
     xkb_map_new_from_string(input->display->xkb_context, map,
                             XKB_KEYMAP_FORMAT_TEXT_V1, 0);

   munmap(map, size);
   close(fd);

   if (!input->xkb.keymap)
     {
        ERR("Failed to compile keymap");
        return;
     }

   input->xkb.state = xkb_state_new(input->xkb.keymap);
   input->xkb.maskless_state = xkb_state_new(input->xkb.keymap);

   if (!input->xkb.state || !input->xkb.maskless_state)
     {
        ERR("Failed to create keymap state");
        xkb_map_unref(input->xkb.keymap);
        input->xkb.keymap = NULL;
        return;
     }

// TIZEN ONLY(20160223) : Add back/menu/home key conversion support
   _tizen_api_version = 0.0;
//

   if (!(locale = getenv("LC_ALL")))
     if (!(locale = getenv("LC_CTYPE")))
       if (!(locale = getenv("LANG")))
         locale = "C";

   if (input->xkb.compose_table)
     xkb_compose_table_unref(input->xkb.compose_table);

   input->xkb.compose_table =
     xkb_compose_table_new_from_locale(input->display->xkb_context,
                                       locale, XKB_COMPOSE_COMPILE_NO_FLAGS);
   if (input->xkb.compose_state)
     xkb_compose_state_unref(input->xkb.compose_state);
   input->xkb.compose_state = NULL;

   if (input->xkb.compose_table)
     {
        input->xkb.compose_state =
          xkb_compose_state_new(input->xkb.compose_table,
                                XKB_COMPOSE_STATE_NO_FLAGS);
     }

   ev = malloc(sizeof(Ecore_Wl2_Event_Seat_Keymap_Changed));
   if (ev)
     {
        ev->id = input->id;
        ev->display = input->display;
        input->display->refs++;
        ecore_event_add(ECORE_WL2_EVENT_SEAT_KEYMAP_CHANGED, ev,
                        _display_event_free, ev->display);
     }

   input->xkb.control_mask =
     1 << xkb_map_mod_get_index(input->xkb.keymap, XKB_MOD_NAME_CTRL);
   input->xkb.alt_mask =
     1 << xkb_map_mod_get_index(input->xkb.keymap, XKB_MOD_NAME_ALT);
   input->xkb.shift_mask =
     1 << xkb_map_mod_get_index(input->xkb.keymap, XKB_MOD_NAME_SHIFT);
   input->xkb.win_mask =
     1 << xkb_map_mod_get_index(input->xkb.keymap, XKB_MOD_NAME_LOGO);
   input->xkb.scroll_mask =
     1 << xkb_map_mod_get_index(input->xkb.keymap, XKB_LED_NAME_SCROLL);
   input->xkb.num_mask =
     1 << xkb_map_mod_get_index(input->xkb.keymap, XKB_LED_NAME_NUM);
   input->xkb.caps_mask =
     1 << xkb_map_mod_get_index(input->xkb.keymap, XKB_MOD_NAME_CAPS);
   input->xkb.altgr_mask =
     1 << xkb_map_mod_get_index(input->xkb.keymap, "ISO_Level3_Shift");
}

static void
_keyboard_cb_enter(void *data, struct wl_keyboard *keyboard EINA_UNUSED, unsigned int serial, struct wl_surface *surface, struct wl_array *keys EINA_UNUSED)
{
   Ecore_Wl2_Input *input;
   Ecore_Wl2_Window *window;

   input = data;
   if (!input) return;

   input->display->serial = serial;

   if (!input->timestamp)
     {
        struct timeval tv;

        gettimeofday(&tv, NULL);
        input->timestamp = (tv.tv_sec * 1000 + tv.tv_usec / 1000);
     }

   /* find the window which this surface belongs to */
   window = _ecore_wl2_display_window_surface_find(input->display, surface);
   if (!window) return;

   input->focus.keyboard = window;
   _ecore_wl2_input_focus_in_send(window, input);
}

static void
_keyboard_cb_leave(void *data, struct wl_keyboard *keyboard EINA_UNUSED, unsigned int serial, struct wl_surface *surface)
{
   Ecore_Wl2_Input *input;
   Ecore_Wl2_Window *window;

   input = data;
   if (!input) return;

   input->display->serial = serial;

// TIZEN_ONLY(20160615): Fix key repeat logic. 
/*
   input->repeat.sym = 0;
   input->repeat.key = 0;
   input->repeat.time = 0;
   if (input->repeat.timer) ecore_timer_del(input->repeat.timer);
   input->repeat.timer = NULL;
*/
//
   window = _ecore_wl2_display_window_surface_find(input->display, surface);
   if (window)
     {
        if (input->focus.keyboard != window)
          ERR("Received keyboard.leave when keyboard did not have enter");
     }
   input->focus.keyboard = NULL;
   if (window)
     _ecore_wl2_input_focus_out_send(window, input);
}

static Eina_Bool
_keyboard_cb_repeat(void *data)
{
   Ecore_Wl2_Input *input;
   Ecore_Wl2_Window *window;

   input = data;
   if (!input) return ECORE_CALLBACK_RENEW;

// TIZEN_ONLY(20160610): fix key repeat condition. 
/*
   window = input->focus.keyboard;
   if (!window) goto out;
*/
   input->repeat.time += (int)(input->repeat.rate * 1000.0);
//

   _ecore_wl2_input_key_send(input, input->focus.keyboard,
                             input->repeat.sym, input->repeat.sym_name,
                             input->repeat.key + 8,
                             WL_KEYBOARD_KEY_STATE_PRESSED,
                             input->repeat.time);

   if (!input->repeat.repeating)
     {
        ecore_timer_interval_set(input->repeat.timer, input->repeat.rate);
        input->repeat.repeating = EINA_TRUE;
     }
   return ECORE_CALLBACK_RENEW;

// TIZEN_ONLY(20160610): fix key repeat condition.
/*
out:
   input->repeat.sym = 0;
   input->repeat.key = 0;
   input->repeat.time = 0;
   return ECORE_CALLBACK_CANCEL;
*/
//
}

/* from weston/clients/window.c */
/* Translate symbols appropriately if a compose sequence is being entered */
static xkb_keysym_t
process_key_press(xkb_keysym_t sym, Ecore_Wl2_Input *input)
{
   if (!input->xkb.compose_state)
     return sym;
   if (sym == XKB_KEY_NoSymbol)
     return sym;
   if (xkb_compose_state_feed(input->xkb.compose_state, sym) !=
       XKB_COMPOSE_FEED_ACCEPTED)
     return sym;

   switch (xkb_compose_state_get_status(input->xkb.compose_state))
     {
      case XKB_COMPOSE_COMPOSING:
        return XKB_KEY_NoSymbol;
      case XKB_COMPOSE_COMPOSED:
        return xkb_compose_state_get_one_sym(input->xkb.compose_state);
      case XKB_COMPOSE_CANCELLED:
        return XKB_KEY_NoSymbol;
      case XKB_COMPOSE_NOTHING:
      default: break;
     }
   return sym;
}

static void
_keyboard_cb_key(void *data, struct wl_keyboard *keyboard EINA_UNUSED, unsigned int serial, unsigned int timestamp, unsigned int keycode, unsigned int state)
{
   Ecore_Wl2_Input *input;
   Ecore_Wl2_Window *window;
   unsigned int code;
   xkb_keysym_t sym = XKB_KEY_NoSymbol, sym_name = XKB_KEY_NoSymbol;
   const xkb_keysym_t *syms;
   // TIZEN_ONLY(20171107): support a tizen_keyrouter interface
   struct wl_surface *surface = NULL;
   //

   input = data;
   if (!input) return;

// TIZEN_ONLY(20180404): support a tizen_keyrouter event surface event
   if (input->key_win)
     {
        window = input->key_win;
     }
   else if ((input->repeat.key) && (keycode == input->repeat.key))
     {
        window = input->repeat_win;
     }
   else
//
      window = input->focus.keyboard; /* try to get the window which has keyboard focus */
   // TIZEN_ONLY(20171107): support a tizen_keyrouter interface
   if (!window)
     {
        INF("window is not focused");
        surface = (struct wl_surface *) eina_hash_find(_ecore_wl2_keygrab_hash_get(), &code);
        if (surface)
          {
             window = ecore_wl2_window_surface_find(surface);
             INF("keycode(%d) is grabbed in the window(%p)", code, window);
          }
        else
          {
             //key event callback can be called even though surface is not exist.
             //TODO: Ecore_Event_Key have event_window info, so if (surface == NULL), we should generate proper window info
             INF("surface is not exist");
          }
     }
   //

   input->display->serial = serial;

   /* xkb rules reflect X broken keycodes, so offset by 8 */
   code = keycode + 8;

// TIZEN ONLY(20160223) : Add back/menu/home key conversion support
   if (_tizen_api_version == 0.0) _ecore_wl2_input_key_conversion_set(input);

// if it is one of the back/menu/home key and _tizen_api_version is less than 2.4.
   if (0.0 < _tizen_api_version && _tizen_api_version < 2.4)
      code = _ecore_wl2_input_convert_old_keys(code);
//

   if (xkb_state_key_get_syms(input->xkb.state, code, &syms) == 1)
     sym = syms[0];
   if (state == WL_KEYBOARD_KEY_STATE_PRESSED)
     sym = process_key_press(sym, input);
   sym_name = xkb_state_key_get_one_sym(input->xkb.maskless_state, code);

// TIZEN_ONLY(20180404): support a tizen_keyrouter event surface event
   if (input->key_win) input->key_win = NULL;
//

   _ecore_wl2_input_key_send(input, window, sym, sym_name, code,
                             state, timestamp);

   if (!xkb_keymap_key_repeats(input->xkb.keymap, code)) return;

   if ((state == WL_KEYBOARD_KEY_STATE_RELEASED) &&
       (keycode == input->repeat.key))
     {
        input->repeat.sym = 0;
        input->repeat.key = 0;
        input->repeat.time = 0;
        if (input->repeat.timer) ecore_timer_del(input->repeat.timer);
        input->repeat.timer = NULL;
// TIZEN_ONLY(20180404): support a tizen_keyrouter event surface event
        input->repeat_win = NULL;
//
     }
   else if (state == WL_KEYBOARD_KEY_STATE_PRESSED)
     {
        /* don't setup key repeat timer if not enabled */
        if (!input->repeat.enabled) return;

        input->repeat.sym = sym;
        input->repeat.sym_name = sym;
        input->repeat.key = keycode;
        input->repeat.time = timestamp;
// TIZEN_ONLY(20180404): support a tizen_keyrouter event surface event
        input->repeat_win = window;
//

        /* Delete this timer if there is still one */
        if (input->repeat.timer) ecore_timer_del(input->repeat.timer);
        input->repeat.timer = NULL;

        if (!input->repeat.timer)
          {
             input->repeat.repeating = EINA_FALSE;
             input->repeat.timer =
               ecore_timer_add(input->repeat.delay, _keyboard_cb_repeat, input);
          }
     }
}

static void
_keyboard_cb_modifiers(void *data, struct wl_keyboard *keyboard EINA_UNUSED, unsigned int serial EINA_UNUSED, unsigned int depressed, unsigned int latched, unsigned int locked, unsigned int group)
{
   Ecore_Wl2_Input *input;
   xkb_mod_mask_t mask;

   input = data;
   if (!input) return;

   /* skip PC style modifiers if we have no keymap */
   if (!input->xkb.keymap) return;

   xkb_state_update_mask(input->xkb.state,
                         depressed, latched, locked, 0, 0, group);

   mask =
     xkb_state_serialize_mods(input->xkb.state,
                              XKB_STATE_MODS_DEPRESSED | XKB_STATE_MODS_LATCHED);

   /* reset modifiers to default */
   input->keyboard.modifiers = 0;

   if (mask & input->xkb.control_mask)
     input->keyboard.modifiers |= ECORE_EVENT_MODIFIER_CTRL;
   if (mask & input->xkb.alt_mask)
     input->keyboard.modifiers |= ECORE_EVENT_MODIFIER_ALT;
   if (mask & input->xkb.shift_mask)
     input->keyboard.modifiers |= ECORE_EVENT_MODIFIER_SHIFT;
   if (mask & input->xkb.win_mask)
     input->keyboard.modifiers |= ECORE_EVENT_MODIFIER_WIN;
   if (mask & input->xkb.altgr_mask)
     input->keyboard.modifiers |= ECORE_EVENT_MODIFIER_ALTGR;
   if (mask & input->xkb.scroll_mask)
     input->keyboard.modifiers |= ECORE_EVENT_MODIFIER_SCROLL;
   if (mask & input->xkb.num_mask)
     input->keyboard.modifiers |= ECORE_EVENT_MODIFIER_NUM;
   if (mask & input->xkb.caps_mask)
     input->keyboard.modifiers |= ECORE_EVENT_MODIFIER_CAPS;


   mask = xkb_state_serialize_mods(input->xkb.state, XKB_STATE_MODS_LOCKED);
   if (mask & input->xkb.scroll_mask)
     input->keyboard.modifiers |= ECORE_EVENT_LOCK_SCROLL;
   if (mask & input->xkb.num_mask)
     input->keyboard.modifiers |= ECORE_EVENT_LOCK_NUM;
   if (mask & input->xkb.caps_mask)
     input->keyboard.modifiers |= ECORE_EVENT_LOCK_CAPS;
}

static void
_keyboard_cb_repeat_setup(void *data, struct wl_keyboard *keyboard EINA_UNUSED, int32_t rate, int32_t delay)
{
   Ecore_Wl2_Input *input;
   Ecore_Wl2_Event_Seat_Keyboard_Repeat_Changed *ev;

   input = data;
   if (!input) return;

   if (rate == 0)
     {
        input->repeat.enabled = EINA_FALSE;
        return;
     }

   input->repeat.enabled = EINA_TRUE;
   //TIZEN_ONLY(20180319): add keyboard repeat info set/get API.
   if (!input->repeat.changed)
     {
        input->repeat.rate = (rate / 1000.0);
        input->repeat.delay = (delay / 1000.0);
     }
   //
   ev = malloc(sizeof(Ecore_Wl2_Event_Seat_Keymap_Changed));
   if (ev)
     {
        ev->id = input->id;
        ev->display = input->display;
        ev->display->refs++;
        ecore_event_add(ECORE_WL2_EVENT_SEAT_KEYBOARD_REPEAT_CHANGED, ev,
                        _display_event_free, ev->display);
     }
}

static const struct wl_keyboard_listener _keyboard_listener =
{
   _keyboard_cb_keymap,
   _keyboard_cb_enter,
   _keyboard_cb_leave,
   _keyboard_cb_key,
   _keyboard_cb_modifiers,
   _keyboard_cb_repeat_setup
};

static void
_touch_cb_down(void *data, struct wl_touch *touch EINA_UNUSED, unsigned int serial, unsigned int timestamp, struct wl_surface *surface, int id, wl_fixed_t x, wl_fixed_t y)
{
   Ecore_Wl2_Input *input;
   Ecore_Wl2_Window *window;

   input = data;
   if (!input) return;

   /* find the window which this surface belongs to */
   window = _ecore_wl2_display_window_surface_find(input->display, surface);
   if (!window) return;

   input->focus.touch = window;
   // TIZEN_ONLY(20171109): support a tizen_input_device_manager interface
   _ecore_wl2_input_touch_axis_process(input, id);
   //

   // TIZEN_ONLY(20171207): do not send pointer enter about all of touch down
   #if 0
    _pointer_cb_enter(data, NULL, serial, surface, x, y);
   #else

   input->grab.count++;

   if ((!input->grab.window) && (input->focus.touch))
     {
        _pointer_cb_enter(data, NULL, serial, surface, x, y);
        _ecore_wl2_input_grab(input, input->focus.touch, BTN_LEFT);
        input->grab.timestamp = timestamp;
     }
   else
     {
        input->pointer.sx = wl_fixed_to_double(x);
        input->pointer.sy = wl_fixed_to_double(y);
     }
   #endif
   //

// TIZEN_ONLY(20171107): always send move event when touch down event is occurred
   _ecore_wl2_input_mouse_move_send(input, input->focus.touch, id);
//
   _ecore_wl2_input_mouse_down_send(input, input->focus.touch, id,
                                    BTN_LEFT, timestamp);
}

static void
_touch_cb_up(void *data, struct wl_touch *touch EINA_UNUSED, unsigned int serial, unsigned int timestamp, int id)
{
   Ecore_Wl2_Input *input;

   input = data;
   if (!input) return;
   if (!input->focus.touch) return;

   input->timestamp = timestamp;
   input->display->serial = serial;

   _ecore_wl2_input_mouse_up_send(input, input->focus.touch, id,
                                  BTN_LEFT, timestamp);

   if (input->grab.count) input->grab.count--;
   if ((input->grab.window) && (input->grab.button == BTN_LEFT) &&
       (!input->grab.count))
     _ecore_wl2_input_ungrab(input);
}

static void
_touch_cb_motion(void *data, struct wl_touch *touch EINA_UNUSED, unsigned int timestamp, int id, wl_fixed_t x, wl_fixed_t y)
{
   Ecore_Wl2_Input *input;

   input = data;
   if (!input) return;
   if (!input->focus.touch) return;

   input->timestamp = timestamp;
   input->pointer.sx = wl_fixed_to_int(x);
   input->pointer.sy = wl_fixed_to_int(y);

   // TIZEN_ONLY(20171109): support a tizen_input_device_manager interface
   _ecore_wl2_input_touch_axis_process(input, id);
   //

   _ecore_wl2_input_mouse_move_send(input, input->focus.touch, id);
}

static void
_touch_cb_frame(void *data EINA_UNUSED, struct wl_touch *touch EINA_UNUSED)
{

}

static void
_touch_cb_cancel(void *data, struct wl_touch *touch EINA_UNUSED)
{
//TIZEN_ONLY(20171107): generate mouse button cancel event
   Ecore_Event_Mouse_Button *ev;
   Ecore_Wl2_Input *input;

   if (!(input = data)) return;
   if (!input->focus.touch) return;

   if (!(ev = calloc(1, sizeof(Ecore_Event_Mouse_Button)))) return;
   EINA_SAFETY_ON_NULL_RETURN(ev);

   ev->timestamp = (int)(ecore_time_get()*1000);
   ev->same_screen = 1;
   ev->window = input->focus.touch->id;
   ev->event_window = ev->window;

   ev->buttons = 1;

   ev->root.x = input->pointer.sx;
   ev->root.y = input->pointer.sy;
   ev->x = input->pointer.sx;
   ev->y = input->pointer.sy;
   ev->modifiers = input->keyboard.modifiers;

   ev->dev = _ecore_wl2_touch_dev_get(input, ev->window);

   ecore_event_add(ECORE_EVENT_MOUSE_BUTTON_CANCEL, ev, NULL, _input_event_mouse_button_cb_free);
//
}

static const struct wl_touch_listener _touch_listener =
{
   _touch_cb_down,
   _touch_cb_up,
   _touch_cb_motion,
   _touch_cb_frame,
   _touch_cb_cancel
};

static void
_data_cb_offer(void *data, struct wl_data_device *data_device EINA_UNUSED, struct wl_data_offer *offer)
{
   Ecore_Wl2_Input *input;

   input = data;
   if (!input) return;

   _ecore_wl2_dnd_add(input, offer);
}

static void
_data_cb_enter(void *data, struct wl_data_device *data_device EINA_UNUSED, uint32_t serial, struct wl_surface *surface, wl_fixed_t x, wl_fixed_t y, struct wl_data_offer *offer)
{
   Ecore_Wl2_Input *input;

   input = data;
   if (!input) return;

   _ecore_wl2_dnd_enter(input, offer, surface,
                        wl_fixed_to_int(x), wl_fixed_to_int(y), serial);
}

static void
_data_cb_leave(void *data, struct wl_data_device *data_device EINA_UNUSED)
{
   Ecore_Wl2_Input *input;

   input = data;
   if (!input) return;

   _ecore_wl2_dnd_leave(input);
}

static void
_data_cb_motion(void *data, struct wl_data_device *data_device EINA_UNUSED, uint32_t serial, wl_fixed_t x, wl_fixed_t y)
{
   Ecore_Wl2_Input *input;

   input = data;
   if (!input) return;

   _ecore_wl2_dnd_motion(input, wl_fixed_to_int(x),
                         wl_fixed_to_int(y), serial);
}

static void
_data_cb_drop(void *data, struct wl_data_device *data_device EINA_UNUSED)
{
   Ecore_Wl2_Input *input;

   input = data;
   if (!input) return;

   _ecore_wl2_dnd_drop(input);
}

static void
_data_cb_selection(void *data, struct wl_data_device *data_device EINA_UNUSED, struct wl_data_offer *offer)
{
   Ecore_Wl2_Input *input;

   input = data;
   if (!input) return;

   _ecore_wl2_dnd_selection(input, offer);
}

static const struct wl_data_device_listener _data_listener =
{
   _data_cb_offer,
   _data_cb_enter,
   _data_cb_leave,
   _data_cb_motion,
   _data_cb_drop,
   _data_cb_selection
};

static void
_seat_cb_capabilities(void *data, struct wl_seat *seat, enum wl_seat_capability caps)
{
   Ecore_Wl2_Event_Seat_Capabilities *ev;
   Ecore_Wl2_Input *input;

   input = data;
   if (!input) return;

   if ((caps & WL_SEAT_CAPABILITY_POINTER) && (!input->wl.pointer))
     {
        input->wl.pointer = wl_seat_get_pointer(seat);
        wl_pointer_set_user_data(input->wl.pointer, input);
        wl_pointer_add_listener(input->wl.pointer, &_pointer_listener, input);

        // TIZEN_ONLY(20171207): add functions to set client's custom cursors
        if (!input->cursor.surface)
          {
             input->cursor.surface =
               wl_compositor_create_surface(input->display->wl.compositor);

             if (input->cursor.surface)
               {
                  if (input->display->wl.tz_policy)
                    {
                       tizen_policy_set_role(input->display->wl.tz_policy,
                                             input->cursor.surface, "wl_pointer-cursor");
                    }
               }
          }
        if (!input->cursor.theme)
          {
             input->cursor.theme =
               wl_cursor_theme_load(input->cursor.theme_name, input->cursor.size,
                                    input->display->wl.shm);
          }
        //
     }
   else if (!(caps & WL_SEAT_CAPABILITY_POINTER) && (input->wl.pointer))
     {
        // TIZEN_ONLY(20171207): add functions to set client's custom cursors
        if (input->cursor.surface) wl_surface_destroy(input->cursor.surface);
        input->cursor.surface = NULL;
        if (input->cursor.theme)
          wl_cursor_theme_destroy(input->cursor.theme);
        input->cursor.theme = NULL;
        //
#ifdef WL_POINTER_RELEASE_SINCE_VERSION
        if (input->seat_version >= WL_POINTER_RELEASE_SINCE_VERSION)
          wl_pointer_release(input->wl.pointer);
        else
#endif
          wl_pointer_destroy(input->wl.pointer);
        input->wl.pointer = NULL;
     }

   if ((caps & WL_SEAT_CAPABILITY_KEYBOARD) && (!input->wl.keyboard))
     {
        input->wl.keyboard = wl_seat_get_keyboard(seat);
        wl_keyboard_set_user_data(input->wl.keyboard, input);
        wl_keyboard_add_listener(input->wl.keyboard, &_keyboard_listener, input);
     }
   else if (!(caps & WL_SEAT_CAPABILITY_KEYBOARD) && (input->wl.keyboard))
     {
#ifdef WL_KEYBOARD_RELEASE_SINCE_VERSION
        if (input->seat_version >= WL_KEYBOARD_RELEASE_SINCE_VERSION)
          wl_keyboard_release(input->wl.keyboard);
        else
#endif
          wl_keyboard_destroy(input->wl.keyboard);
        input->wl.keyboard = NULL;
     }

   if ((caps & WL_SEAT_CAPABILITY_TOUCH) && (!input->wl.touch))
     {
        input->wl.touch = wl_seat_get_touch(seat);
        wl_touch_set_user_data(input->wl.touch, input);
        wl_touch_add_listener(input->wl.touch, &_touch_listener, input);
     }
   else if (!(caps & WL_SEAT_CAPABILITY_TOUCH) && (input->wl.touch))
     {
#ifdef WL_TOUCH_RELEASE_SINCE_VERSION
        if (input->seat_version >= WL_TOUCH_RELEASE_SINCE_VERSION)
          wl_touch_release(input->wl.touch);
        else
#endif
          wl_touch_destroy(input->wl.touch);
        input->wl.touch = NULL;
// TIZEN ONLY(20160223) : Add back/menu/home key conversion support
        _ecore_wl2_input_key_conversion_clean_up();
//
     }
   ecore_wl2_display_flush(input->display);

// TIZEN_ONLY(20171107): support a tizen_keyrouter interface
   input->caps_update = EINA_TRUE;
//

   ev = calloc(1, sizeof(Ecore_Wl2_Event_Seat_Capabilities));
   EINA_SAFETY_ON_NULL_RETURN(ev);

   ev->id = input->id;
   ev->pointer_enabled = !!(caps & WL_SEAT_CAPABILITY_POINTER);
   ev->keyboard_enabled = !!(caps & WL_SEAT_CAPABILITY_KEYBOARD);
   ev->touch_enabled = !!(caps & WL_SEAT_CAPABILITY_TOUCH);
   ev->display = input->display;
   ev->display->refs++;

   ecore_event_add(ECORE_WL2_EVENT_SEAT_CAPABILITIES_CHANGED, ev,
                   _display_event_free, ev->display);
}

static void
_cb_seat_event_free(void *data EINA_UNUSED, void *event)
{
   Ecore_Wl2_Event_Seat_Name *ev;

   ev = event;
   eina_stringshare_del(ev->name);
   ecore_wl2_display_disconnect(ev->display);
   free(ev);
}

static void
_seat_cb_name(void *data, struct wl_seat *seat EINA_UNUSED, const char *name)
{
   Ecore_Wl2_Event_Seat_Name *ev;
   Ecore_Wl2_Input *input;

   input = data;
   eina_stringshare_replace(&input->name, name);

   ev = calloc(1, sizeof(Ecore_Wl2_Event_Seat_Name));
   EINA_SAFETY_ON_NULL_RETURN(ev);

   ev->id = input->id;
   ev->name = eina_stringshare_add(name);
   ev->display = input->display;
   ev->display->refs++;

   ecore_event_add(ECORE_WL2_EVENT_SEAT_NAME_CHANGED, ev,
                   _cb_seat_event_free, NULL);
}

static const struct wl_seat_listener _seat_listener =
{
   _seat_cb_capabilities,
   _seat_cb_name,
};

static void
_ecore_wl2_input_cursor_setup(Ecore_Wl2_Input *input)
{
   char *tmp;
   // TIZEN_ONLY(20171207): add functions to set client's custom cursors
   #if 0
   input->cursor.size = 32;
   tmp = getenv("ECORE_WL_CURSOR_SIZE");
   if (tmp) input->cursor.size = atoi(tmp);

   if (!input->cursor.name)
     input->cursor.name = eina_stringshare_add("left_ptr");
   #else
   unsigned int cursor_size;
   char *cursor_theme_name;

   tmp = getenv("ECORE_WL_CURSOR_SIZE");
   if (tmp)
     cursor_size = atoi(tmp);
   else
     cursor_size = 32;
   ecore_wl2_input_cursor_size_set(input, cursor_size);

   cursor_theme_name = getenv("ECORE_WL_CURSOR_THEME_NAME");
   ecore_wl2_input_cursor_theme_name_set(input, cursor_theme_name);
   #endif
   //
}

// TIZEN_ONLY(20171207): add functions to set client's custom cursors
static const struct wl_callback_listener _pointer_surface_listener =
{
   _cb_pointer_frame
};

static void
_cb_pointer_frame(void *data, struct wl_callback *callback, unsigned int timestamp EINA_UNUSED)
{
   Ecore_Wl2_Input *input;

   if (!(input = data)) return;

   if (callback)
     {
        if (callback != input->cursor.frame_cb) return;
        wl_callback_destroy(callback);
        input->cursor.frame_cb = NULL;
     }

   if (!input->cursor.name)
     {
        ecore_wl2_input_pointer_set(input, NULL, 0, 0);
        return;
     }

   if ((input->cursor.cursor->image_count > 1) && (!input->cursor.frame_cb))
     {
        input->cursor.frame_cb = wl_surface_frame(input->cursor.surface);
        if (!input->cursor.frame_cb) return;

        wl_callback_add_listener(input->cursor.frame_cb,
                                 &_pointer_surface_listener, input);
     }
}
//

Eina_Bool
_ecore_wl2_input_cursor_update(void *data)
{
   Ecore_Wl2_Input *input;

   input = data;
   if (!input) return EINA_FALSE;

   // TIZEN_ONLY(20171207): add functions to set client's custom cursors
   #if 0
   if (input->wl.pointer)
     wl_pointer_set_cursor(input->wl.pointer, input->pointer.enter_serial,
                           input->cursor.surface,
                           input->cursor.hot_x, input->cursor.hot_y);
   ecore_wl2_display_flush(input->display);

   return ECORE_CALLBACK_RENEW;
   #else
   struct wl_cursor_image *cursor_image;
   struct wl_buffer *buffer;
   unsigned int delay;

   if ((!input) || (!input->cursor.cursor) || (!input->cursor.surface))
     return EINA_FALSE;

   cursor_image = input->cursor.cursor->images[input->cursor.current_index];
   if (!cursor_image) return ECORE_CALLBACK_RENEW;

   if ((buffer = wl_cursor_image_get_buffer(cursor_image)))
     {
        ecore_wl2_input_pointer_set(input, input->cursor.surface,
                                   cursor_image->hotspot_x,
                                   cursor_image->hotspot_y);
        wl_surface_attach(input->cursor.surface, buffer, 0, 0);
        wl_surface_damage(input->cursor.surface, 0, 0,
                          cursor_image->width, cursor_image->height);
        wl_surface_commit(input->cursor.surface);

        if ((input->cursor.cursor->image_count > 1) && (!input->cursor.frame_cb))
          _cb_pointer_frame(input, NULL, 0);
     }

   if (input->cursor.cursor->image_count <= 1)
     return ECORE_CALLBACK_CANCEL;

   delay = cursor_image->delay;
   input->cursor.current_index =
      (input->cursor.current_index + 1) % input->cursor.cursor->image_count;

   if (!input->cursor.timer)
     input->cursor.timer =
        ecore_timer_loop_add(delay / 1000.0,
                             _ecore_wl2_input_cursor_update, input);
   else
     ecore_timer_interval_set(input->cursor.timer, delay / 1000.0);

   return ECORE_CALLBACK_RENEW;
   #endif
   //
}

static void
_ecore_wl2_devices_free(Ecore_Wl2_Input_Devices *devices)
{
   if (devices->seat_dev)
     efl_unref(devices->seat_dev);
   if (devices->pointer_dev)
     efl_unref(devices->pointer_dev);
   if (devices->keyboard_dev)
     efl_unref(devices->keyboard_dev);
   if (devices->touch_dev)
     efl_unref(devices->touch_dev);

   free(devices);
}

static Eina_Bool
_ecore_wl2_cb_device_event(void *data, int type, void *event)
{
   Ecore_Wl2_Input_Devices *devs, *devices = NULL;;
   Ecore_Wl2_Event_Device *ev = event;
   Ecore_Wl2_Input *input = data;
   Eina_List *l;

   if (input->id != ev->seat_id)
     return ECORE_CALLBACK_PASS_ON;

   EINA_LIST_FOREACH(input->devices_list, l, devs)
     {
        if (devs->window_id == ev->window_id)
          {
             devices = devs;
             break;
          }
     }

   if (type == ECORE_WL2_EVENT_DEVICE_ADDED)
     {
        if (!devices)
          {
             devices = calloc(1, sizeof(Ecore_Wl2_Input_Devices));
             EINA_SAFETY_ON_NULL_RETURN_VAL(devices, ECORE_CALLBACK_PASS_ON);
             input->devices_list =
               eina_list_append(input->devices_list, devices);
             devices->window_id = ev->window_id;
          }

        if (ev->type == ECORE_WL2_DEVICE_TYPE_POINTER)
          devices->pointer_dev = efl_ref(ev->dev);
        else if (ev->type == ECORE_WL2_DEVICE_TYPE_KEYBOARD)
          devices->keyboard_dev = efl_ref(ev->dev);
        else if (ev->type == ECORE_WL2_DEVICE_TYPE_TOUCH)
          devices->touch_dev = efl_ref(ev->dev);
        else if (ev->type == ECORE_WL2_DEVICE_TYPE_SEAT)
          devices->seat_dev = efl_ref(ev->dev);

        return ECORE_CALLBACK_PASS_ON;
     }

   if (!devices)
     return ECORE_CALLBACK_PASS_ON;

   if (ev->type == ECORE_WL2_DEVICE_TYPE_SEAT)
     {
        input->devices_list =
          eina_list_remove(input->devices_list, devices);
        _ecore_wl2_devices_free(devices);
        return ECORE_CALLBACK_PASS_ON;
     }

   if ((ev->type == ECORE_WL2_DEVICE_TYPE_POINTER) &&
       (devices->pointer_dev == ev->dev))
     {
        efl_unref(devices->pointer_dev);
        devices->pointer_dev = NULL;
     }
   else if ((ev->type == ECORE_WL2_DEVICE_TYPE_KEYBOARD) &&
            (devices->keyboard_dev == ev->dev))
     {
        efl_unref(devices->keyboard_dev);
        devices->keyboard_dev = NULL;
     }
   else if ((ev->type == ECORE_WL2_DEVICE_TYPE_TOUCH) &&
            (devices->touch_dev == ev->dev))
     {
        efl_unref(devices->touch_dev);
        devices->touch_dev = NULL;
     }

   return ECORE_CALLBACK_PASS_ON;
}

// TIZEN_ONLY(20171109): support a tizen_input_device_manager interface
static void
_ecore_wl2_input_device_last_device_set(Ecore_Wl2_Tizen_Input_Device *dev)
{
   Ecore_Wl2_Input *input = dev->input;

   if (!input) return;

   switch(dev->clas)
     {
      case ECORE_DEVICE_CLASS_MOUSE:
         input->devmgr.last_device_ptr = dev;
         break;
      case ECORE_DEVICE_CLASS_KEYBOARD:
         input->devmgr.last_device_kbd = dev;
         break;
      case ECORE_DEVICE_CLASS_TOUCH:
         input->devmgr.last_device_touch = dev;
         break;
      default:
         break;
     }
}

static void
_ecore_wl2_input_device_last_device_unset(Ecore_Wl2_Tizen_Input_Device *dev)
{
   Ecore_Wl2_Input *input = dev->input;

   if (!input) return;

   switch(dev->clas)
     {
      case ECORE_DEVICE_CLASS_MOUSE:
         if (input->devmgr.last_device_ptr == dev)
           input->devmgr.last_device_ptr = NULL;
         break;
      case ECORE_DEVICE_CLASS_KEYBOARD:
         if (input->devmgr.last_device_kbd == dev)
           input->devmgr.last_device_kbd = NULL;
         break;
      case ECORE_DEVICE_CLASS_TOUCH:
         if (input->devmgr.last_device_touch == dev)
           input->devmgr.last_device_touch = NULL;
         break;
      default:
         break;
     }
}
//

void
_ecore_wl2_input_add(Ecore_Wl2_Display *display, unsigned int id, unsigned int version)
{
   Ecore_Wl2_Input *input;

   input = calloc(1, sizeof(Ecore_Wl2_Input));
   if (!input) return;

   input->id = id;
   input->display = display;
   input->seat_version = version;
   input->repeat.rate = 0.025;
   input->repeat.delay = 0.4;
   input->repeat.enabled = EINA_TRUE;
   input->repeat.changed = EINA_FALSE;

   wl_array_init(&input->data.selection.types);
   wl_array_init(&input->data.drag.types);

   /* setup cursor size and theme */
   _ecore_wl2_input_cursor_setup(input);

   input->wl.seat =
     wl_registry_bind(display->wl.registry, id, &wl_seat_interface, 4);

   display->inputs =
     eina_inlist_append(display->inputs, EINA_INLIST_GET(input));

   wl_seat_add_listener(input->wl.seat, &_seat_listener, input);
   wl_seat_set_user_data(input->wl.seat, input);

   input->dev_add_handler =
     ecore_event_handler_add(ECORE_WL2_EVENT_DEVICE_ADDED,
                             _ecore_wl2_cb_device_event, input);

   input->dev_remove_handler =
     ecore_event_handler_add(ECORE_WL2_EVENT_DEVICE_REMOVED,
                             _ecore_wl2_cb_device_event, input);

   if (!display->wl.data_device_manager) return;

   input->data.device =
     wl_data_device_manager_get_data_device(display->wl.data_device_manager,
                                            input->wl.seat);
   wl_data_device_add_listener(input->data.device, &_data_listener, input);
}

void
_ecore_wl2_input_del(Ecore_Wl2_Input *input)
{
   Ecore_Wl2_Input_Devices *devices;
   Ecore_Wl2_Display *display;
   Eina_Inlist *l = NULL;
   Ecore_Wl2_Mouse_Down_Info *info = NULL;
   Ecore_Wl2_Window *window;

   if (!input) return;

   display = input->display;

   l = _ecore_wl2_mouse_down_info_list;
   while (l)
     {
        info = EINA_INLIST_CONTAINER_GET(l, Ecore_Wl2_Mouse_Down_Info);
        l = eina_inlist_remove(l, l);
        free(info);
     }
   _ecore_wl2_mouse_down_info_list = NULL;

   if (input->repeat.timer) ecore_timer_del(input->repeat.timer);

   if (input->cursor.name) eina_stringshare_del(input->cursor.name);

   if (input->data.selection.types.data)
     {
        char **t;

        wl_array_for_each(t, &input->data.selection.types)
          free(*t);

        wl_array_release(&input->data.selection.types);
     }
   if (input->data.drag.types.data)
     {
        char **t;

        wl_array_for_each(t, &input->data.drag.types)
          free(*t);

        wl_array_release(&input->data.drag.types);
     }

   if (input->data.selection.source)
     wl_data_source_destroy(input->data.selection.source);
   if (input->data.drag.source)
     wl_data_source_destroy(input->data.drag.source);
   if (input->drag.offer) _ecore_wl2_offer_unref(input->drag.offer);
   if (input->selection.offer) _ecore_wl2_offer_unref(input->selection.offer);
   if (input->data.device) wl_data_device_destroy(input->data.device);

   if (input->xkb.state) xkb_state_unref(input->xkb.state);
   if (input->xkb.maskless_state) xkb_state_unref(input->xkb.maskless_state);
   if (input->xkb.keymap) xkb_map_unref(input->xkb.keymap);
   if (input->xkb.compose_table)
     xkb_compose_table_unref(input->xkb.compose_table);
   if (input->xkb.compose_state)
     xkb_compose_state_unref(input->xkb.compose_state);

   if (input->wl.seat) wl_seat_destroy(input->wl.seat);

   ecore_event_handler_del(input->dev_add_handler);
   ecore_event_handler_del(input->dev_remove_handler);
   EINA_LIST_FREE(input->devices_list, devices)
      _ecore_wl2_devices_free(devices);

   display->inputs =
     eina_inlist_remove(display->inputs, EINA_INLIST_GET(input));

   EINA_INLIST_FOREACH(display->windows, window)
     if (window->grab == input) window->grab = NULL;

   eina_stringshare_replace(&input->name, NULL);
   free(input);
}

void
_ecore_wl2_input_cursor_set(Ecore_Wl2_Input *input, const char *cursor)
{
   eina_stringshare_replace(&input->cursor.name, cursor);
   if (!cursor) eina_stringshare_replace(&input->cursor.name, "left_ptr");
}

void
_ecore_wl2_input_window_remove(Ecore_Wl2_Input *input, Ecore_Wl2_Window *window)
{
   Ecore_Wl2_Input_Devices *devices;
   Eina_List *l, *l_next;

   if ((input->focus.pointer) &&
       (input->focus.pointer == window))
     input->focus.pointer = NULL;
   if ((input->focus.keyboard) &&
       (input->focus.keyboard == window))
     {
        input->focus.keyboard = NULL;
        ecore_timer_del(input->repeat.timer);
        input->repeat.timer = NULL;
     }

   EINA_LIST_FOREACH_SAFE(input->devices_list, l, l_next, devices)
      if (devices->window_id == window->id)
        {
           _ecore_wl2_devices_free(devices);
           input->devices_list = eina_list_remove_list(input->devices_list, l);
        }
}

// TIZEN_ONLY(20171107): support a tizen_keyrouter interface
static void
_ecore_wl2_cb_keygrab_notify(void *data EINA_UNUSED, struct tizen_keyrouter *tizen_keyrouter EINA_UNUSED, struct wl_surface *surface EINA_UNUSED, uint32_t key, uint32_t mode, uint32_t error)
{
   _ecore_wl2_keygrab_error = error;
   INF("[PID:%d] key=%d, mode=%d, error=%d", getpid(), key, mode, error);
}

static void
_ecore_wl2_cb_keygrab_notify_list(void *data EINA_UNUSED, struct tizen_keyrouter *tizen_keyrouter EINA_UNUSED, struct wl_surface *surface EINA_UNUSED, struct wl_array *grab_result)
{
   wl_array_init(&_ecore_wl2_keygrab_result_list);
   wl_array_copy(&_ecore_wl2_keygrab_result_list, grab_result);
}

static void
_ecore_wl2_cb_getgrab_notify_list(void *data EINA_UNUSED, struct tizen_keyrouter *tizen_keyrouter EINA_UNUSED, struct wl_surface *surface EINA_UNUSED, struct wl_array *grab_result EINA_UNUSED)
{
   ;
}

static void
_ecore_wl2_cb_set_register_none_key(void *data EINA_UNUSED, struct tizen_keyrouter *tizen_keyrouter EINA_UNUSED, struct wl_surface *surface EINA_UNUSED, uint32_t mode EINA_UNUSED)
{
   ;
}

static void
_ecore_wl2_cb_keyregister_notify(void *data EINA_UNUSED, struct tizen_keyrouter *tizen_keyrouter EINA_UNUSED, uint32_t status EINA_UNUSED)
{
   ;
}

static void
_ecore_wl2_cb_set_input_config(void *data EINA_UNUSED, struct tizen_keyrouter *tizen_keyrouter EINA_UNUSED, uint32_t status EINA_UNUSED)
{
   ;
}

static void
_ecore_wl2_cb_key_cancel(void *data, struct tizen_keyrouter *tizen_keyrouter EINA_UNUSED, uint32_t key)
{
   Ecore_Wl2_Display *ewd = (Ecore_Wl2_Display *)data;
   Ecore_Wl2_Input *input;

   if (!ewd)
     {
        WRN("Failed to get Ecore_Wl2_Display\n");
        return;
     }

   EINA_INLIST_FOREACH(ewd->inputs, input)
     {
        if (input->repeat.key == key)
          {
             input->repeat.sym = 0;
             input->repeat.key = 0;
             input->repeat.time = 0;

             if (input->repeat.timer) ecore_timer_del(input->repeat.timer);
             input->repeat.timer = NULL;
          }
     }
}

static void
_ecore_wl2_cb_event_surface(void *data, struct tizen_keyrouter *tizen_keyrouter EINA_UNUSED, struct wl_surface *surface, uint32_t mode)
{
   Ecore_Wl2_Display *ewd = (Ecore_Wl2_Display *)data;
   Ecore_Wl2_Input *input;

   if (!ewd)
     {
        WRN("Failed to get Ecore_Wl2_Display\n");
        return;
     }

   EINA_INLIST_FOREACH(ewd->inputs, input)
     {
        input->key_win = ecore_wl2_window_surface_find(surface);
        input->key_mode = mode;

        if(!input->key_win)
          {
             WRN("Get a event_surface(%p) but there was a no Ecore_Wl2_Window\n", surface);
          }
     }
}

// TIZEN_ONLY(20150722): Add ecore_wl_window_keygrab_* APIs
static const struct tizen_keyrouter_listener _tz_keyrouter_listener =
{
   _ecore_wl2_cb_keygrab_notify,
   _ecore_wl2_cb_keygrab_notify_list,
   _ecore_wl2_cb_getgrab_notify_list,
   _ecore_wl2_cb_set_register_none_key,
   _ecore_wl2_cb_keyregister_notify,
   _ecore_wl2_cb_set_input_config,
   _ecore_wl2_cb_key_cancel,
   _ecore_wl2_cb_event_surface
};
//

void
_ecore_wl2_keyrouter_setup(Ecore_Wl2_Display *ewd, unsigned int id, unsigned int version EINA_UNUSED)
{
   ewd->wl.tz_keyrouter =
          wl_registry_bind(ewd->wl.registry, id, &tizen_keyrouter_interface, 2);
   if (ewd->wl.tz_keyrouter)
     tizen_keyrouter_add_listener(ewd->wl.tz_keyrouter, &_tz_keyrouter_listener, ewd);
}

struct _Keycode_Map
{
   xkb_keysym_t keysym;
   xkb_keycode_t *keycodes;
   int num_keycodes;
};

typedef struct _Keycode_Map Keycode_Map;

static void find_keycode(struct xkb_keymap *keymap, xkb_keycode_t key, void *data)
{
   Keycode_Map *found_keycodes = (Keycode_Map *)data;
   xkb_keysym_t keysym = found_keycodes->keysym;
   int num_syms = 0;
   const xkb_keysym_t *syms_out = NULL;
   num_syms = xkb_keymap_key_get_syms_by_level(keymap, key, 0, 0, &syms_out);
   if ((num_syms) && (syms_out))
     {
        if ((*syms_out) == (keysym))
          {
             found_keycodes->num_keycodes++;
             found_keycodes->keycodes = realloc(found_keycodes->keycodes, sizeof(int) * found_keycodes->num_keycodes);
             if (found_keycodes->keycodes)
               found_keycodes->keycodes[found_keycodes->num_keycodes - 1] = key;
          }
     }
}

//If there are several keycodes, ecore_wl only deals with first keycode.
int
ecore_wl2_input_keycode_from_keysym(struct xkb_keymap *keymap, xkb_keysym_t keysym, xkb_keycode_t **keycodes)
{
   Keycode_Map found_keycodes = {0,};
   found_keycodes.keysym = keysym;

   //called fewer (max_keycode - min_keycode +1) times.
   xkb_keymap_key_for_each(keymap, find_keycode, &found_keycodes);

   *keycodes = found_keycodes.keycodes;
   INF("num of keycodes:%d ", found_keycodes.num_keycodes);
   return found_keycodes.num_keycodes;
}

EAPI Ecore_Wl2_Input *
ecore_wl2_input_default_input_get(const Ecore_Wl2_Display *ewd)
{
   Ecore_Wl2_Input *input;

   EINA_SAFETY_ON_NULL_RETURN_VAL(ewd, NULL);
   EINA_SAFETY_ON_NULL_RETURN_VAL(ewd->inputs, NULL);

   input = ecore_wl2_display_input_find_by_name(ewd, "seat0");
   if (!input) input = ecore_wl2_display_input_find_by_name(ewd, "default");

   return input;
}

// TIZEN_ONLY(20150722): Add ecore_wl_window_keygrab_* APIs
//Currently this function is only used in sink call, so use global value(_ecore_wl_keygrab_error) and just check the error is ok.
/* internal functions */
static Eina_Bool
_ecore_wl2_keygrab_hash_add(void *key, void *data)
{
   Eina_Bool ret = EINA_FALSE;

   if (!_keygrabs)
     _keygrabs = eina_hash_int32_new(NULL);
   ret = eina_hash_add(_keygrabs, key, data);
   return ret;
}

static Eina_Bool
_ecore_wl2_keygrab_hash_del(void *key)
{
   Eina_Bool ret = EINA_FALSE;

   ret = eina_hash_del_by_key(_keygrabs, key);

   return ret;
}

Eina_Hash *
_ecore_wl2_keygrab_hash_get(void)
{
   return _keygrabs;
}

//I'm not sure that keygrab function should be changed to Ecore_evas_XXX.
//In the future, keyrouter feature can be added upstream or finish stabilizing.
//After that time, we maybe change API name or other thing.
//So do not use this API if you have trouble catch keyrouter feature or rule changes.

//Keyrouter support the situation when wl_win is not exist.
//But keyrouter also can be meet situation when there are several surfaces.
//So I decided to add keygrab feature into ecore_wl_window side like x system.

//Mod, not_mod, priority will be used future.
//But now we are not support, so just use 0 for this parameter.
//win can be NULL

EAPI Eina_Bool
ecore_wl2_window_keygrab_set(Ecore_Wl2_Window *win, const char *key, int mod EINA_UNUSED, int not_mod EINA_UNUSED, int priority EINA_UNUSED, Ecore_Wl2_Window_Keygrab_Mode grab_mode)
{
   Ecore_Wl2_Display *ewd;
   xkb_keysym_t keysym = 0x0;
   int num_keycodes = 0;
   xkb_keycode_t *keycodes = NULL;
   int i;
   Ecore_Wl2_Input *input;

   Eina_Bool ret = EINA_FALSE;
   struct wl_surface *surface = NULL;

   if (win)
     ewd = win->display;
   else
     ewd = ecore_wl2_connected_display_get(NULL);

   if (!ewd) return EINA_FALSE;

   while (!ewd->wl.tz_keyrouter)
     {
        INF("Wait until keyrouter interface is ready");
        wl_display_roundtrip(ewd->wl.display);
     }
   if (!key) return EINA_FALSE;
   if ((grab_mode < ECORE_WL2_WINDOW_KEYGRAB_UNKNOWN) || (grab_mode > ECORE_WL2_WINDOW_KEYGRAB_EXCLUSIVE))
     return EINA_FALSE;

   INF("win=%p key=%s mod=%d", win, key, grab_mode);

   keysym = xkb_keysym_from_name(key, XKB_KEYSYM_NO_FLAGS);

   if (keysym == XKB_KEY_NoSymbol)
     {
        WRN("Keysym of key(\"%s\") doesn't exist", key);
        return EINA_FALSE;
     }

   //We have to find the way to get keycode from keysym before keymap notify
   //keymap event occurs after minimum 3 roundtrips
   //1. ecore_wl_init: wl_registry_add_listener
   //2. _ecore_wl_cb_handle_global: wl_seat_add_listener
   //3. _ecore_wl_input_seat_handle_capabilities: wl_keyboard_add_listener
   while (eina_inlist_count(ewd->inputs) == 0)
     {
        INF("Wait wl_registry_add_listener reply");
        wl_display_roundtrip(ewd->wl.display);
     }

   input = ecore_wl2_input_default_input_get(ewd);

   if (input)
     {
        while (!input->caps_update)
          {
             INF("Wait until wl_seat_capabilities_update is ready");
             wl_display_roundtrip(ewd->wl.display);
          }
        if (input->wl.keyboard)
          {
             while (!input->xkb.keymap)
               {
                  wl_display_roundtrip(ewd->wl.display);
                  INF("Wait until keymap event occurs");
               }
             INF("Finish keymap event");

             num_keycodes = ecore_wl2_input_keycode_from_keysym(input->xkb.keymap, keysym, &keycodes);
          }
        else
          {
             WRN("This device does not support key");
             return EINA_FALSE;
          }
     }

   if (num_keycodes == 0)
     {
        WRN("Keycode of key(\"%s\") doesn't exist", key);
        return EINA_FALSE;
     }

   /* Request to grab a key */
   if (win)
     surface = ecore_wl2_window_surface_get(win);

   for (i = 0; i < num_keycodes; i++)
     {
        INF("keycode of key:(%d)", keycodes[i]);
        tizen_keyrouter_set_keygrab(ewd->wl.tz_keyrouter, surface, keycodes[i], grab_mode);
        /* Send sync to wayland compositor and register sync callback to exit while dispatch loop below */
        ecore_wl2_display_sync(ewd);

        INF("After keygrab _ecore_wl2_keygrab_error = %d", _ecore_wl2_keygrab_error);
        if (!_ecore_wl2_keygrab_error)
          {
             INF("[PID:%d]Succeed to get return value !", getpid());
             if (_ecore_wl2_keygrab_hash_add(&keycodes[i], surface))
               INF("Succeed to add key to the keygrab hash!");
             //TODO: deal with if (win == NULL)
             else
               WRN("Failed to add key to the keygrab hash!");
             ret = EINA_TRUE;
          }
        else
          {
             WRN("[PID:%d]Failed to get return value ! ret=%d)", getpid(), _ecore_wl2_keygrab_error);
             ret = EINA_FALSE;
          }
     }

   free(keycodes);
   keycodes = NULL;

   _ecore_wl2_keygrab_error = -1;
   return ret;
}

EAPI Eina_Bool
ecore_wl2_window_keygrab_unset(Ecore_Wl2_Window *win, const char *key, int mod EINA_UNUSED, int any_mod EINA_UNUSED)
{
   Ecore_Wl2_Display *ewd;
   xkb_keysym_t keysym = 0x0;
   int num_keycodes = 0;
   xkb_keycode_t *keycodes = NULL;
   int i;

   Eina_Bool ret = EINA_FALSE;
   struct wl_surface *surface = NULL;
   Ecore_Wl2_Input *input;

   if (win)
     ewd = win->display;
   else
     ewd = ecore_wl2_connected_display_get(NULL);

   if (!ewd) return EINA_FALSE;

   if ((!ewd) || (!ewd->wl.tz_keyrouter)) return EINA_FALSE;
   if (!key) return EINA_FALSE;
   INF("win=%p key=%s ", win, key);

   keysym = xkb_keysym_from_name(key, XKB_KEYSYM_NO_FLAGS);
   if (keysym == XKB_KEY_NoSymbol)
     {
        WRN("Keysym of key(\"%s\") doesn't exist", key);
        return EINA_FALSE;
     }

   while (eina_inlist_count(ewd->inputs) == 0)
     {
        INF("Wait wl_registry_add_listener reply");
        wl_display_roundtrip(ewd->wl.display);
     }

   input = ecore_wl2_input_default_input_get(ewd);

   //We have to find the way to get keycode from keysym before keymap notify
   if ((input) && (input->xkb.keymap))
     num_keycodes = ecore_wl2_input_keycode_from_keysym(input->xkb.keymap, keysym, &keycodes);
   else
     {
        WRN("Keymap is not ready");
        return EINA_FALSE;
     }

   if (num_keycodes == 0)
     {
        WRN("Keycode of key(\"%s\") doesn't exist", key);
        return EINA_FALSE;
     }

   /* Request to ungrab a key */
   if (win)
     surface = ecore_wl2_window_surface_get(win);

   for (i = 0; i < num_keycodes; i++)
     {
        INF("keycode of key:(%d)", keycodes[i]);
        tizen_keyrouter_unset_keygrab(ewd->wl.tz_keyrouter, surface, keycodes[i]);

        /* Send sync to wayland compositor and register sync callback to exit while dispatch loop below */
        ecore_wl2_display_sync(ewd);

        INF("After keygrab unset  _ecore_wl2_keygrab_error = %d", _ecore_wl2_keygrab_error);
        if (!_ecore_wl2_keygrab_error)
          {
             INF("[PID:%d]Succeed to get return value !", getpid());
             if (_ecore_wl2_keygrab_hash_del(&keycodes[i]))
               INF("Succeed to delete key from the keygrab hash!");
             else
               WRN("Failed to delete key from the keygrab hash!");
             ret = EINA_TRUE;
          }
        else
          {
             ret = EINA_FALSE;
             WRN("[PID:%d] Failed to get return value ! (ret=%d)", getpid(), _ecore_wl2_keygrab_error);
          }
     }

   free(keycodes);
   keycodes = NULL;

   _ecore_wl2_keygrab_error = -1;
   return ret;
}

char *
_ecore_wl2_keyname_get(int keycode)
{
   xkb_keysym_t sym = XKB_KEY_NoSymbol;
   char name[256] = {0, };
   Ecore_Wl2_Display *ewd;
   Ecore_Wl2_Input *input;

   ewd = ecore_wl2_connected_display_get(NULL);
   input = ecore_wl2_input_default_input_get(ewd);

   sym = xkb_state_key_get_one_sym(input->xkb.state, keycode);
   xkb_keysym_get_name(sym, name, sizeof(name));

   return strdup(name);
}

EAPI Eina_List
*ecore_wl2_window_keygrab_list_set(Ecore_Wl2_Window *win, Eina_List *infos)
{
   Ecore_Wl2_Display *ewd;
   xkb_keysym_t keysym = 0x0;
   int num_keycodes = 0;
   xkb_keycode_t *keycodes = NULL;

   struct wl_surface *surface = NULL;

   struct wl_array grab_list;
   int *grab_data = NULL;
   Eina_List *l1, *l2;
   Eina_List *error_keys = NULL;
   int i;

   Ecore_Wl2_Keygrab_Info *info;
   Ecore_Wl2_Window_Keygrab_Info *grab_info;
   Ecore_Wl2_Input *input;

   if (win)
     ewd = win->display;
   else
     ewd = ecore_wl2_connected_display_get(NULL);

   if (!ewd) goto err;

   while (!ewd->wl.tz_keyrouter)
     {
        INF("Wait until keyrouter interface is ready");
        wl_display_roundtrip(ewd->wl.display);
     }

   while (eina_inlist_count(ewd->inputs) == 0)
     {
        INF("Wait wl_registry_add_listener reply");
        wl_display_roundtrip(ewd->wl.display);
     }

   input = ecore_wl2_input_default_input_get(ewd);

   while (!input->caps_update)
     {
        INF("Wait until wl_seat_capabilities_update is ready");
        wl_display_roundtrip(ewd->wl.display);
     }
   if (input->wl.keyboard)
     {
        while(!input->xkb.keymap)
          {
             wl_display_roundtrip(ewd->wl.display);
             INF("Wait until keymap event occurs");
          }
        INF("Finish keymap event");
     }
   else
     {
        WRN("This device does not support key");
        goto err;
     }

   if (win)
     surface = ecore_wl2_window_surface_get(win);

   wl_array_init(&grab_list);

   EINA_LIST_FOREACH_SAFE(infos, l1, l2, grab_info)
     {
        if (!grab_info->key) continue;
        if ((grab_info->mode < ECORE_WL2_WINDOW_KEYGRAB_UNKNOWN) || (grab_info->mode > ECORE_WL2_WINDOW_KEYGRAB_EXCLUSIVE))
          continue;

        keysym = xkb_keysym_from_name(grab_info->key, XKB_KEYSYM_NO_FLAGS);

        if (keysym == XKB_KEYSYM_NO_FLAGS)
          {
             WRN("Keysym of key(\"%s\") doesn't exist", grab_info->key);
             continue;
          }
        num_keycodes = ecore_wl2_input_keycode_from_keysym(input->xkb.keymap, keysym, &keycodes);

        if (num_keycodes == 0)
          {
             WRN("Keycode of key(\"%s\") doesn't exist", grab_info->key);
             continue;
          }
        for (i = 0; i < num_keycodes; i++)
          {
             INF("keycode of key:(%d)", keycodes[i]);
             grab_data = wl_array_add(&grab_list, sizeof(int));
             *grab_data = keycodes[i];
             grab_data = wl_array_add(&grab_list, sizeof(int));
             *grab_data = grab_info->mode;
             grab_data = wl_array_add(&grab_list, sizeof(int));
             *grab_data = 0;
          }
        free(keycodes);
        keycodes = NULL;
     }
   tizen_keyrouter_set_keygrab_list(ewd->wl.tz_keyrouter, surface, &grab_list);

   ecore_wl2_display_sync(ewd);

   wl_array_for_each(info, &_ecore_wl2_keygrab_result_list)
     {

        if (!info->err)
          {
             INF("[PID:%d]Succeed to get return value !", getpid());
             if (_ecore_wl2_keygrab_hash_add(&info->key, surface))
               INF("Succeed to add key to the keygrab hash!");
             else
               WRN("Failed to add key to the keygrab hash!");
          }
        else
          {
             WRN("After keygrab keycode %d error = %d", info->key, info->err);
             error_keys = eina_list_append(error_keys, _ecore_wl2_keyname_get(info->key));
          }
     }
   wl_array_release(&grab_list);
   wl_array_release(&_ecore_wl2_keygrab_result_list);

   return error_keys;

err:
   EINA_LIST_FOREACH_SAFE(infos, l1, l2, grab_info)
     {
        error_keys = eina_list_append(error_keys, strdup(grab_info->key));
     }
   return error_keys;
}

EAPI Eina_List
*ecore_wl2_window_keygrab_list_unset(Ecore_Wl2_Window *win, Eina_List *infos)
{
   Ecore_Wl2_Display *ewd;
   xkb_keysym_t keysym = 0x0;
   int num_keycodes = 0;
   xkb_keycode_t *keycodes = NULL;

   struct wl_surface *surface = NULL;

   struct wl_array ungrab_list;
   int *grab_data = NULL;
   Eina_List *l1, *l2;
   Eina_List *error_keys = NULL;
   int i;

   Ecore_Wl2_Keyungrab_Info *info;
   Ecore_Wl2_Window_Keygrab_Info *grab_info;

   Ecore_Wl2_Input *input;

   if (win)
     ewd = win->display;
   else
     ewd = ecore_wl2_connected_display_get(NULL);

   if (!ewd) goto err;

   if ((!ewd->wl.tz_keyrouter)) goto err;

   input = ecore_wl2_input_default_input_get(ewd);

   if ((!input) || (!input->xkb.keymap))
     {
        ERR("Keymap is not ready");
        goto err;
     }

   if (win)
     surface = ecore_wl2_window_surface_get(win);

   wl_array_init(&ungrab_list);

   EINA_LIST_FOREACH_SAFE(infos, l1, l2, grab_info)
     {
        if (!grab_info->key) continue;

        keysym = xkb_keysym_from_name(grab_info->key, XKB_KEYSYM_NO_FLAGS);

        if (keysym == XKB_KEYSYM_NO_FLAGS)
          {
             WRN("Keysym of key(\"%s\") doesn't exist", grab_info->key);
             continue;
          }
        num_keycodes = ecore_wl2_input_keycode_from_keysym(input->xkb.keymap, keysym, &keycodes);

        if (num_keycodes == 0)
          {
             WRN("Keycode of key(\"%s\") doesn't exist", grab_info->key);
             continue;
          }
        for (i = 0; i < num_keycodes; i++)
          {
             INF("keycode of key:(%d)", keycodes[i]);
             grab_data = wl_array_add(&ungrab_list, sizeof(int));
             *grab_data = keycodes[i];
             grab_data = wl_array_add(&ungrab_list, sizeof(int));
             *grab_data = 0;
          }
        free(keycodes);
        keycodes = NULL;
     }
   tizen_keyrouter_unset_keygrab_list(ewd->wl.tz_keyrouter, surface, &ungrab_list);

   ecore_wl2_display_sync(ewd);

   wl_array_for_each(info, &_ecore_wl2_keygrab_result_list)
     {
        if (!info->err)
          {
             INF("[PID:%d]Succeed to get return value !", getpid());
             if (_ecore_wl2_keygrab_hash_del(&info->key))
               INF("Succeed to delete key to the keygrab hash!");
             else
               WRN("Failed to delete key to the keygrab hash!");
          }
        else
          {
             WRN("After keyungrab keycode %d error = %d", info->key, info->err);
             error_keys = eina_list_append(error_keys, _ecore_wl2_keyname_get(info->key));
          }
     }
   wl_array_release(&ungrab_list);
   wl_array_release(&_ecore_wl2_keygrab_result_list);

   return error_keys;

err:
   EINA_LIST_FOREACH_SAFE(infos, l1, l2, grab_info)
     {
        error_keys = eina_list_append(error_keys, strdup(grab_info->key));
     }
   return error_keys;
}
//
//

// TIZEN_ONLY(20171109): support a tizen_input_device_manager interface
static void
_ecore_wl2_input_device_info_free(void *data EINA_UNUSED, void *ev)
{
   Ecore_Event_Device_Info *e;

   e = ev;
   eina_stringshare_del(e->name);
   eina_stringshare_del(e->identifier);
   eina_stringshare_del(e->seatname);

   free(e);
}

void
_ecore_wl2_input_device_info_send(Ecore_Window win_id, const char *name,  const char *identifier, Ecore_Device_Class clas, Ecore_Device_Subclass subclas, Eina_Bool flag)
{
   Ecore_Event_Device_Info *e;

   if (!(e = calloc(1, sizeof(Ecore_Event_Device_Info)))) return;

   e->window = win_id;
   e->name = eina_stringshare_add(name);
   e->identifier = eina_stringshare_add(identifier);
   e->seatname = eina_stringshare_add(name);
   e->clas = clas;
   e->subclas = subclas;

   if (flag)
     ecore_event_add(ECORE_EVENT_DEVICE_ADD, e, _ecore_wl2_input_device_info_free, NULL);
   else
     ecore_event_add(ECORE_EVENT_DEVICE_DEL, e, _ecore_wl2_input_device_info_free, NULL);
}

static Eina_Bool
_ecore_wl2_input_device_ecore_device_add(Ecore_Wl2_Tizen_Input_Device *dev)
{
   Ecore_Device *ecdev;
   Eina_List *l;
   const char *ecdev_name;

   if (!dev->identifier) return EINA_FALSE;

   EINA_LIST_FOREACH((Eina_List *)ecore_device_list(), l, ecdev)
     {
        ecdev_name = ecore_device_identifier_get(ecdev);
        if (!ecdev_name) continue;
        if ((ecore_device_class_get(ecdev) == dev->clas) && (!strcmp(ecdev_name, dev->identifier)))
          return EINA_FALSE;
     }

   ecdev = ecore_device_add();
   if (!ecdev)
     {
        ERR("Failed to add ecore device for name: %s (%s)\n", dev->name, dev->identifier);
        return EINA_FALSE;
     }
   ecore_device_name_set(ecdev, dev->name);
   ecore_device_identifier_set(ecdev, dev->identifier);
   ecore_device_class_set(ecdev, dev->clas);
   ecore_device_subclass_set(ecdev, dev->subclas);

   dev->device = efl_ref(ecdev);

   return EINA_TRUE;
}

static Eina_Bool
_ecore_wl2_input_device_ecore_device_remove(Ecore_Wl2_Tizen_Input_Device *dev)
{
   Ecore_Device *ecdev;
   const Eina_List *l;
   const char *ecdev_name;

   if (!dev->identifier) return EINA_FALSE;

   EINA_LIST_FOREACH(ecore_device_list(), l, ecdev)
     {
        ecdev_name = ecore_device_identifier_get(ecdev);
        if (!ecdev_name) continue;
        if ((ecore_device_class_get(ecdev) == dev->clas) &&
            (!strcmp(ecdev_name, dev->identifier)))
           {
              ecore_device_del(ecdev);
              dev->device = NULL;
              return EINA_TRUE;
           }
      }

    return EINA_FALSE;
}

static void
_ecore_wl2_input_device_info_broadcast(Ecore_Wl2_Tizen_Input_Device *dev, Eina_Bool flag)
{
   Eina_Hash *windows = NULL;
   Eina_Iterator *itr;
   Ecore_Wl2_Window *win = NULL;
   void *data;
   Eina_Bool res, has_win = EINA_FALSE;

   if (!dev) return;
   if (!dev->name) return;
   if (!dev->input || !dev->input->display) return;

   if (flag)
     res = _ecore_wl2_input_device_ecore_device_add(dev);
   else
     res = _ecore_wl2_input_device_ecore_device_remove(dev);

   if (!res) return;

   windows = _ecore_wl2_window_hash_get();
   if (windows)
     {
        itr = eina_hash_iterator_data_new(windows);
        while (eina_iterator_next(itr, &data))
          {
             win = data;
             has_win = EINA_TRUE;
             _ecore_wl2_input_device_info_send(win->id, dev->name, dev->identifier, dev->clas, dev->subclas, flag);
          }

        eina_iterator_free(itr);
     }
   if (!has_win)
     {
        _ecore_wl2_input_device_info_send((uintptr_t)NULL, dev->name, dev->identifier, dev->clas, dev->subclas, flag);
     }
}

void
_ecore_wl2_input_devices_send(Ecore_Wl2_Input *input, Ecore_Wl2_Window *win)
{
   Eina_List *l;
   Ecore_Wl2_Tizen_Input_Device *dev;

   if (!input) return;

   EINA_LIST_FOREACH(input->devmgr.devices, l, dev)
     {
        _ecore_wl2_input_device_info_send(win->id, dev->name, dev->identifier, dev->clas, dev->subclas, EINA_TRUE);
     }
}


static void
_ecore_wl2_input_device_cb_device_info(void *data, struct tizen_input_device *tizen_input_device EINA_UNUSED, const char *name, uint32_t clas, uint32_t subclas, struct wl_array *axes EINA_UNUSED)
{
   Ecore_Wl2_Tizen_Input_Device *dev;

   if (!(dev = data)) return;
   dev->clas = (Ecore_Device_Class)clas;
   dev->subclas = (Ecore_Device_Subclass)subclas;
   dev->name = eina_stringshare_add(name);

   _ecore_wl2_input_device_info_broadcast(dev, EINA_TRUE);
}

static void
_ecore_wl2_input_device_cb_event_device(void *data, struct tizen_input_device *tizen_input_device EINA_UNUSED, unsigned int serial EINA_UNUSED, const char *name EINA_UNUSED, uint32_t time EINA_UNUSED)
{
   Ecore_Wl2_Tizen_Input_Device *dev;

   if (!(dev = data)) return;
   if (!dev->identifier) return;
   _ecore_wl2_input_device_last_device_set(dev);

   return;
}

static void
_ecore_wl2_input_detent_rotate_free(void *data EINA_UNUSED, void *ev)
{
   Ecore_Event_Detent_Rotate *e = ev;
   free(e);
}

static void
_ecore_wl2_input_device_cb_axis(void *data, struct tizen_input_device *tizen_input_device EINA_UNUSED, uint32_t axis_type, wl_fixed_t value)
{
   Ecore_Wl2_Input *input;
   Ecore_Wl2_Tizen_Input_Device *dev;
   double dvalue = wl_fixed_to_double(value);
   Ecore_Event_Detent_Rotate *e;

   dev = (Ecore_Wl2_Tizen_Input_Device *)data;
   input = dev->input;

   if (!input) return;

   switch (axis_type)
     {
        case TIZEN_INPUT_DEVICE_AXIS_TYPE_RADIUS_X:
           input->touch.last_touch_axis.radius_x = dvalue;
           break;
        case TIZEN_INPUT_DEVICE_AXIS_TYPE_RADIUS_Y:
           input->touch.last_touch_axis.radius_y = dvalue;
           break;
        case TIZEN_INPUT_DEVICE_AXIS_TYPE_PRESSURE:
           input->touch.last_touch_axis.pressure = dvalue;
           break;
        case TIZEN_INPUT_DEVICE_AXIS_TYPE_ANGLE:
           input->touch.last_touch_axis.angle = dvalue;
           break;
        case TIZEN_INPUT_DEVICE_AXIS_TYPE_DETENT:
           /* Do something after get detent event.
            * value 1 is clockwise,
            * value -1 is counterclockwise,
            */
           if (!(e = calloc(1, sizeof(Ecore_Event_Detent_Rotate))))
             {
                ERR("detent: cannot allocate memory");
                return;
             }
           if (dvalue == 1)
             e->direction = ECORE_DETENT_DIRECTION_CLOCKWISE;
           else
             e->direction = ECORE_DETENT_DIRECTION_COUNTER_CLOCKWISE;
           e->timestamp = (int)(ecore_time_get() * 1000.0);
           DBG("detent: dir: %d, time: %d", e->direction, e->timestamp);
           ecore_event_add(ECORE_EVENT_DETENT_ROTATE, e, _ecore_wl2_input_detent_rotate_free, NULL);
           break;
        default:
           WRN("Invalid type(%d) is ignored.\n", axis_type);
           break;
     }
   return;
}

static const struct tizen_input_device_listener _tz_input_device_listener =
{
   _ecore_wl2_input_device_cb_device_info,
   _ecore_wl2_input_device_cb_event_device,
   _ecore_wl2_input_device_cb_axis,
};

static void
_ecore_wl2_input_device_manager_cb_device_add(void *data, struct tizen_input_device_manager *tizen_input_device_manager EINA_UNUSED,
                          unsigned int serial EINA_UNUSED, const char *identifier, struct tizen_input_device *device, struct wl_seat *seat)
{
   Ecore_Wl2_Display *ewd = (Ecore_Wl2_Display *)data;
   Ecore_Wl2_Input *input;
   Ecore_Wl2_Tizen_Input_Device *dev;

   if (!ewd) return;
   if ((!identifier) || (!device) || (!seat)) return;

   input = wl_seat_get_user_data(seat);

   if (!input) return;
   if (!(dev = calloc(1, sizeof(Ecore_Wl2_Tizen_Input_Device)))) return;

   dev->tz_device = device;
   tizen_input_device_add_listener(dev->tz_device, &_tz_input_device_listener, dev);
   dev->input = input;
   dev->identifier = eina_stringshare_add(identifier);
   dev->seat = seat;

   input->devmgr.devices = eina_list_append(input->devmgr.devices, dev);
}

static void
_ecore_wl2_input_device_manager_cb_device_remove(void *data, struct tizen_input_device_manager *tizen_input_device_manager EINA_UNUSED,
                            unsigned int serial EINA_UNUSED, const char *identifier, struct tizen_input_device *device, struct wl_seat *seat)
{
   Ecore_Wl2_Display *ewd = (Ecore_Wl2_Display *)data;
   Ecore_Wl2_Input *input;
   Eina_List *l, *l_next;
   Ecore_Wl2_Tizen_Input_Device *dev;

   if (!ewd) return;
   if ((!identifier) || (!device) || (!seat)) return;

   input = wl_seat_get_user_data(seat);

   if (!input) return;

   EINA_LIST_FOREACH_SAFE(input->devmgr.devices, l, l_next, dev)
     {
        if (!dev->identifier) continue;
        if ((!strcmp(dev->identifier, identifier)) && (seat == dev->seat) && (device == dev->tz_device))
          {
             _ecore_wl2_input_device_info_broadcast(dev, EINA_FALSE);

             _ecore_wl2_input_device_last_device_unset(dev);

             if (dev->tz_device) tizen_input_device_release(dev->tz_device);
             if (dev->name) eina_stringshare_del(dev->name);
             if (dev->identifier) eina_stringshare_del(dev->identifier);
             dev->seat = NULL;

             input->devmgr.devices = eina_list_remove_list(input->devmgr.devices, l);

             free(dev);
             break;
          }
     }
}

static void
_ecore_wl2_input_device_manager_cb_error(void *data EINA_UNUSED, struct tizen_input_device_manager *tizen_input_device_manager EINA_UNUSED, uint32_t errorcode EINA_UNUSED)
{
   ;
}

static void
_ecore_wl2_input_device_manager_cb_block_expired(void *data EINA_UNUSED, struct tizen_input_device_manager *tizen_input_device_manager EINA_UNUSED)
{
   ;
}

static const struct tizen_input_device_manager_listener _tz_input_device_mgr_listener =
{
   _ecore_wl2_input_device_manager_cb_device_add,
   _ecore_wl2_input_device_manager_cb_device_remove,
   _ecore_wl2_input_device_manager_cb_error,
   _ecore_wl2_input_device_manager_cb_block_expired,
};

void
_ecore_wl2_input_device_manager_setup(Ecore_Wl2_Display *ewd, unsigned int id, unsigned int version EINA_UNUSED)
{
   ewd->wl.tz_input_device_manager =
   wl_registry_bind(ewd->wl.registry, id, &tizen_input_device_manager_interface, 2);

   tizen_input_device_manager_add_listener(ewd->wl.tz_input_device_manager,
                                       &_tz_input_device_mgr_listener, ewd);
}
//

EAPI struct wl_seat *
ecore_wl2_input_seat_get(Ecore_Wl2_Input *input)
{
   EINA_SAFETY_ON_NULL_RETURN_VAL(input, NULL);
   EINA_SAFETY_ON_NULL_RETURN_VAL(input->display, NULL);
   return input->wl.seat;
}

EAPI Ecore_Wl2_Seat_Capabilities
ecore_wl2_input_seat_capabilities_get(Ecore_Wl2_Input *input)
{
   Ecore_Wl2_Seat_Capabilities cap = ECORE_WL2_SEAT_CAPABILITIES_NONE;

   EINA_SAFETY_ON_NULL_RETURN_VAL(input, cap);
   EINA_SAFETY_ON_NULL_RETURN_VAL(input->display, 0);
   if (input->wl.keyboard)
     cap |= ECORE_WL2_SEAT_CAPABILITIES_KEYBOARD;
   if (input->wl.pointer)
     cap |= ECORE_WL2_SEAT_CAPABILITIES_POINTER;
   if (input->wl.touch)
     cap |= ECORE_WL2_SEAT_CAPABILITIES_TOUCH;
   return cap;
}

EAPI Eina_Stringshare *
ecore_wl2_input_name_get(Ecore_Wl2_Input *input)
{
   EINA_SAFETY_ON_NULL_RETURN_VAL(input, NULL);
   return input->name;
}

EAPI unsigned int
ecore_wl2_input_seat_id_get(Ecore_Wl2_Input *input)
{
   EINA_SAFETY_ON_NULL_RETURN_VAL(input, 0);
   EINA_SAFETY_ON_NULL_RETURN_VAL(input->display, 0);
   return input->id;
}

EAPI Ecore_Wl2_Display *
ecore_wl2_input_display_get(const Ecore_Wl2_Input *input)
{
   EINA_SAFETY_ON_NULL_RETURN_VAL(input, NULL);
   EINA_SAFETY_ON_NULL_RETURN_VAL(input->display, NULL);
   return input->display;
}

EAPI struct xkb_keymap *
ecore_wl2_input_keymap_get(const Ecore_Wl2_Input *input)
{
   EINA_SAFETY_ON_NULL_RETURN_VAL(input, NULL);
   EINA_SAFETY_ON_NULL_RETURN_VAL(input->display, NULL);
   EINA_SAFETY_ON_FALSE_RETURN_VAL(input->wl.keyboard, NULL);
   return input->xkb.keymap;
}

//TIZEN_ONLY(20180319): add keyboard repeat info set/get API.
EAPI Eina_Bool
ecore_wl2_input_keyboard_repeat_set(Ecore_Wl2_Input *input, double rate, double delay)
{
   EINA_SAFETY_ON_NULL_RETURN_VAL(input, EINA_FALSE);
   EINA_SAFETY_ON_NULL_RETURN_VAL(input->display, EINA_FALSE);
   EINA_SAFETY_ON_FALSE_RETURN_VAL(input->wl.keyboard, EINA_FALSE);
   input->repeat.rate = rate;
   input->repeat.delay = delay;
   input->repeat.changed = EINA_TRUE;
   return input->repeat.enabled;
}
//

EAPI Eina_Bool
ecore_wl2_input_keyboard_repeat_get(const Ecore_Wl2_Input *input, double *rate, double *delay)
{
   EINA_SAFETY_ON_NULL_RETURN_VAL(input, EINA_FALSE);
   EINA_SAFETY_ON_NULL_RETURN_VAL(input->display, EINA_FALSE);
   EINA_SAFETY_ON_FALSE_RETURN_VAL(input->wl.keyboard, EINA_FALSE);
   if (rate) *rate = input->repeat.rate;
   if (delay) *delay = input->repeat.delay;
   return input->repeat.enabled;
}

EAPI Eo *
ecore_wl2_input_seat_device_get(const Ecore_Wl2_Input *input, const Ecore_Wl2_Window *window)
{
   Ecore_Wl2_Input_Devices *devices;
   EINA_SAFETY_ON_NULL_RETURN_VAL(input, NULL);
   EINA_SAFETY_ON_NULL_RETURN_VAL(window, NULL);

   devices = _ecore_wl2_devices_get(input, window->id);
   return devices ?  devices->seat_dev : NULL;
}

// TIZEN_ONLY(20171207): add functions to set client's custom cursors
static void
_pointer_update_stop(Ecore_Wl2_Input *input)
{
   if (!input->cursor.timer) return;

   ecore_timer_del(input->cursor.timer);
   input->cursor.timer = NULL;
}
//

EAPI void
ecore_wl2_input_pointer_set(Ecore_Wl2_Input *input, struct wl_surface *surface, int hot_x, int hot_y)
{
   EINA_SAFETY_ON_NULL_RETURN(input);

   // TIZEN_ONLY(20171207): add functions to set client's custom cursors
   _pointer_update_stop(input);
   if (input->wl.pointer)
     wl_pointer_set_cursor(input->wl.pointer, input->pointer.enter_serial,
                           surface, hot_x, hot_y);
   //

   input->cursor.surface = surface;
   input->cursor.hot_x = hot_x;
   input->cursor.hot_y = hot_y;

   // TIZEN_ONLY(20171207): add functions to set client's custom cursors
   #if 0
   _ecore_wl2_input_cursor_update(input);
   #endif
   //
}

// TIZEN_ONLY(20171207): add functions to set client's custom cursors
EAPI struct wl_cursor *
ecore_wl2_input_cursor_get(Ecore_Wl2_Input *input, const char *cursor_name)
{
   if ((!input) || (!input->cursor.theme))
     return NULL;

   return wl_cursor_theme_get_cursor(input->cursor.theme,
                                     cursor_name);
}
//

EAPI void
ecore_wl2_input_cursor_from_name_set(Ecore_Wl2_Input *input, const char *cursor_name)
{
   EINA_SAFETY_ON_NULL_RETURN(input);
   // TIZEN_ONLY(20171207): add functions to set client's custom cursors
   #if 0
   _ecore_wl2_input_cursor_set(input, cursor);
   #else
   struct wl_cursor *cursor;

   /* No pointer device. Don't need to set cursor and update it */
   if (!input->wl.pointer) return;

   _pointer_update_stop(input);

   eina_stringshare_replace(&input->cursor.name, cursor_name);

   /* No cursor. Set to default Left Pointer */
   if (!cursor_name)
     eina_stringshare_replace(&input->cursor.name, "left_ptr");

   /* try to get this cursor from the theme */
   if (!(cursor = ecore_wl2_input_cursor_get(input, input->cursor.name)))
     {
        /* if the theme does not have this cursor, default to left pointer */
        if (!(cursor = ecore_wl2_input_cursor_get(input, "left_ptr")))
          return;
     }

   input->cursor.cursor = cursor;

   if ((!cursor->images) || (!cursor->images[0]))
     {
        ecore_wl2_input_pointer_set(input, NULL, 0, 0);
        return;
     }

   input->cursor.current_index = 0;

   _ecore_wl2_input_cursor_update(input);
   #endif
}

// TIZEN_ONLY(20171207): add functions to set client's custom cursors
EAPI void
ecore_wl2_input_cursor_size_set(Ecore_Wl2_Input *input, const int size)
{
   if (!input) return;

   input->cursor.size = size;

   EINA_SAFETY_ON_NULL_RETURN(input->display->wl.shm);

   if (input->cursor.theme)
     wl_cursor_theme_destroy(input->cursor.theme);

   input->cursor.theme =
     wl_cursor_theme_load(NULL, input->cursor.size, input->display->wl.shm);
}

EAPI void
ecore_wl2_input_cursor_theme_name_set(Ecore_Wl2_Input *input, const char *cursor_theme_name)
{
   if (!input) return;

   eina_stringshare_replace(&input->cursor.theme_name, cursor_theme_name);

   EINA_SAFETY_ON_NULL_RETURN(input->display->wl.shm);

   if (input->cursor.theme)
     wl_cursor_theme_destroy(input->cursor.theme);
   input->cursor.theme =
     wl_cursor_theme_load(input->cursor.theme_name, input->cursor.size,
                          input->display->wl.shm);
}

EAPI void
ecore_wl2_input_cursor_default_restore(Ecore_Wl2_Input *input)
{
   if (!input) return;

   /* Restore to default wayland cursor */
   ecore_wl2_input_cursor_from_name_set(input, "left_ptr");
}
//

EAPI Eina_Bool
ecore_wl2_input_pointer_xy_get(const Ecore_Wl2_Input *input, int *x, int *y)
{
   EINA_SAFETY_ON_NULL_RETURN_VAL(input, EINA_FALSE);

   if (x) *x = 0;
   if (y) *y = 0;
   if (!input->wl.pointer) return EINA_FALSE;
   if (x) *x = input->pointer.sx;
   if (y) *y = input->pointer.sy;
   return EINA_TRUE;
}
