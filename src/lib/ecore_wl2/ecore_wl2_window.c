#ifdef HAVE_CONFIG_H
# include <config.h>
#endif

#include "ecore_wl2_private.h"
#include "efl-hints-client-protocol.h"

//TIZEN_ONLY(20171216): add ecore_wl2_window_find
/* local variables */
static Eina_Hash *_windows = NULL;

/* internal functions */
void
_ecore_wl2_window_init(void)
{
   if (!_windows)
     _windows = eina_hash_string_superfast_new(NULL);
}

void
_ecore_wl2_window_shutdown(void)
{
   eina_hash_free(_windows);
   _windows = NULL;
}

Eina_Hash *
_ecore_wl2_window_hash_get(void)
{
   return _windows;
}

static char *
_ecore_wl2_window_id_str_get(int win_id)
{
   const char *vals = "qWeRtYuIoP5$&<~";
   static char id[9];
   unsigned int val;

   val = (unsigned int)win_id;
   id[0] = vals[(val >> 28) & 0xf];
   id[1] = vals[(val >> 24) & 0xf];
   id[2] = vals[(val >> 20) & 0xf];
   id[3] = vals[(val >> 16) & 0xf];
   id[4] = vals[(val >> 12) & 0xf];
   id[5] = vals[(val >> 8) & 0xf];
   id[6] = vals[(val >> 4) & 0xf];
   id[7] = vals[(val) & 0xf];
   id[8] = 0;

   return id;
}
//
static void _ecore_wl2_window_hide_send(Ecore_Wl2_Window *window);

void
_ecore_wl2_window_semi_free(Ecore_Wl2_Window *window)
{
   if (window->xdg_popup) xdg_popup_destroy(window->xdg_popup);
   window->xdg_popup = NULL;

   if (window->xdg_toplevel) xdg_toplevel_destroy(window->xdg_toplevel);
   window->xdg_toplevel = NULL;

   if (window->xdg_surface) xdg_surface_destroy(window->xdg_surface);
   window->xdg_surface = NULL;

   if (window->zxdg_popup) zxdg_popup_v6_destroy(window->zxdg_popup);
   window->zxdg_popup = NULL;

   if (window->zxdg_toplevel) zxdg_toplevel_v6_destroy(window->zxdg_toplevel);
   window->zxdg_toplevel = NULL;

   if (window->zxdg_surface) zxdg_surface_v6_destroy(window->zxdg_surface);
   window->zxdg_surface = NULL;

   if (window->www_surface)
     www_surface_destroy(window->www_surface);
   window->www_surface = NULL;

   if (window->surface) wl_surface_destroy(window->surface);
   window->surface = NULL;
   window->surface_id = -1;

   if (window->callback) wl_callback_destroy(window->callback);
   window->callback = NULL;

   window->outputs = eina_list_free(window->outputs);

   ecore_wl2_window_surface_flush(window, EINA_TRUE);

   window->commit_pending = EINA_FALSE;
}

static void
_ecore_wl2_window_activate_send(Ecore_Wl2_Window *window)
{
   Ecore_Wl2_Event_Window_Activate *ev;

   ev = calloc(1, sizeof(Ecore_Wl2_Event_Window_Activate));
   if (!ev) return;

   ev->win = window->id;
   if (window->parent)
     ev->parent_win = window->parent->id;
   ev->event_win = window->id;
   ecore_event_add(ECORE_WL2_EVENT_WINDOW_ACTIVATE, ev, NULL, NULL);
}

static void
_ecore_wl2_window_deactivate_send(Ecore_Wl2_Window *window)
{
   Ecore_Wl2_Event_Window_Deactivate *ev;

   ev = calloc(1, sizeof(Ecore_Wl2_Event_Window_Deactivate));
   if (!ev) return;

   ev->win = window->id;
   if (window->parent)
     ev->parent_win = window->parent->id;
   ev->event_win = window->id;
   ecore_event_add(ECORE_WL2_EVENT_WINDOW_DEACTIVATE, ev, NULL, NULL);
}

static void
_ecore_wl2_window_configure_send(Ecore_Wl2_Window *win)
{
   Ecore_Wl2_Event_Window_Configure *ev;

   ev = calloc(1, sizeof(Ecore_Wl2_Event_Window_Configure));
   if (!ev) return;

   ev->win = win->id;
   ev->event_win = win->id;

// TIZEN_ONLY(20171112): support tizen_position
   ev->x = win->set_config.geometry.x;
   ev->y = win->set_config.geometry.y;
//

   if ((win->set_config.geometry.w == win->def_config.geometry.w) &&
       (win->set_config.geometry.h == win->def_config.geometry.h))
     ev->w = ev->h = 0;
   else if ((!win->def_config.geometry.w) && (!win->def_config.geometry.h) &&
            (!win->def_config.fullscreen) &&
            (!win->def_config.maximized) &&
            ((win->def_config.fullscreen != win->req_config.fullscreen) ||
             (win->def_config.maximized != win->req_config.maximized)))
     ev->w = win->saved.w, ev->h = win->saved.h;
   else
     ev->w = win->def_config.geometry.w, ev->h = win->def_config.geometry.h;

   ev->edges = !!win->def_config.resizing;
   if (win->def_config.fullscreen)
     ev->states |= ECORE_WL2_WINDOW_STATE_FULLSCREEN;
   if (win->def_config.maximized)
     ev->states |= ECORE_WL2_WINDOW_STATE_MAXIMIZED;

   win->req_config = win->def_config;

   ecore_event_add(ECORE_WL2_EVENT_WINDOW_CONFIGURE, ev, NULL, NULL);

   if (win->def_config.focused)
     _ecore_wl2_window_activate_send(win);
   else
     _ecore_wl2_window_deactivate_send(win);
}

//TIZEN_ONLY(20180201) : add function to set window size by client.
static void
_ecore_wl2_window_configure_send_by_client(Ecore_Wl2_Window *win)
{
   Ecore_Wl2_Event_Window_Configure *ev;

   ev = calloc(1, sizeof(Ecore_Wl2_Event_Window_Configure));
   if (!ev) return;

   ev->win = win->id;
   ev->event_win = win->id;
   ev->x = win->set_config.geometry.x;
   ev->y = win->set_config.geometry.y;
   ev->w = win->set_config.geometry.w;
   ev->h = win->set_config.geometry.h;
   ev->edges = !!win->req_config.resizing;

   ecore_event_add(ECORE_WL2_EVENT_WINDOW_CONFIGURE, ev, NULL, NULL);
}
//

static void
_configure_complete(Ecore_Wl2_Window *window)
{
   Ecore_Wl2_Event_Window_Configure_Complete *ev;

   window->pending.configure = EINA_FALSE;

   ev = calloc(1, sizeof(Ecore_Wl2_Event_Window_Configure_Complete));
   if (!ev) return;

   ev->win = window->id;
   ecore_event_add(ECORE_WL2_EVENT_WINDOW_CONFIGURE_COMPLETE, ev, NULL, NULL);

}

#include "window_v6.x"

static void
_www_surface_end_drag(void *data, struct www_surface *www_surface EINA_UNUSED)
{
   Ecore_Wl2_Window *window = data;
   Ecore_Wl2_Event_Window_WWW_Drag *ev;

   ev = malloc(sizeof(Ecore_Wl2_Event_Window_WWW_Drag));
   EINA_SAFETY_ON_NULL_RETURN(ev);
   ev->window = window->id;
   ev->dragging = 0;

   ecore_event_add(_ecore_wl2_event_window_www_drag, ev, NULL, NULL);
}

static void
_www_surface_start_drag(void *data, struct www_surface *www_surface EINA_UNUSED)
{
   Ecore_Wl2_Window *window = data;
   Ecore_Wl2_Event_Window_WWW_Drag *ev;

   ev = malloc(sizeof(Ecore_Wl2_Event_Window_WWW_Drag));
   EINA_SAFETY_ON_NULL_RETURN(ev);
   ev->window = window->id;
   ev->dragging = 1;

   ecore_event_add(_ecore_wl2_event_window_www_drag, ev, NULL, NULL);
}

static void
_www_surface_status(void *data, struct www_surface *www_surface EINA_UNUSED, int32_t x_rel, int32_t y_rel, uint32_t timestamp)
{
   Ecore_Wl2_Window *window = data;
   Ecore_Wl2_Event_Window_WWW *ev;

   ev = malloc(sizeof(Ecore_Wl2_Event_Window_WWW));
   EINA_SAFETY_ON_NULL_RETURN(ev);
   ev->window = window->id;
   ev->x_rel = x_rel;
   ev->y_rel = y_rel;
   ev->timestamp = timestamp;

   ecore_event_add(_ecore_wl2_event_window_www, ev, NULL, NULL);
}

static struct www_surface_listener _www_surface_listener =
{
   .status = _www_surface_status,
   .start_drag = _www_surface_start_drag,
   .end_drag = _www_surface_end_drag,
};

void
_ecore_wl2_window_www_surface_init(Ecore_Wl2_Window *window)
{
   if (!window->surface) return;
   if (!window->display->wl.www) return;
   if (window->www_surface) return;
   window->www_surface = www_create(window->display->wl.www, window->surface);
   www_surface_set_user_data(window->www_surface, window);
   www_surface_add_listener(window->www_surface, &_www_surface_listener,
                            window);
}


static void
_xdg_surface_cb_configure(void *data, struct xdg_surface *xdg_surface EINA_UNUSED, uint32_t serial)
{
   Ecore_Wl2_Window *window;

   window = data;
   window->def_config.serial = serial;

   if (window->pending.configure)
     {
        window->saved.w = window->set_config.geometry.w;
        window->saved.h = window->set_config.geometry.h;
     }
   if (window->pending.configure && window->updating)
     ERR("Window shouldn't be rendering before initial configure");

   if (!window->updating)
     _ecore_wl2_window_configure_send(window);

   if (window->pending.configure)
     _configure_complete(window);
}

static const struct xdg_surface_listener _xdg_surface_listener =
{
   _xdg_surface_cb_configure,
};

static void
_xdg_toplevel_cb_configure(void *data, struct xdg_toplevel *xdg_toplevel EINA_UNUSED, int32_t width, int32_t height, struct wl_array *states)
{
   Ecore_Wl2_Window *win = data;
   uint32_t *s;

   win->def_config.minimized = EINA_FALSE;
   win->def_config.maximized = EINA_FALSE;
   win->def_config.fullscreen = EINA_FALSE;
   win->def_config.focused = EINA_FALSE;
   win->def_config.resizing = EINA_FALSE;
   win->def_config.geometry.w = width;
   win->def_config.geometry.h = height;

   wl_array_for_each(s, states)
     {
        switch (*s)
          {
           case ZXDG_TOPLEVEL_V6_STATE_MAXIMIZED:
             win->def_config.maximized = EINA_TRUE;
             break;
           case ZXDG_TOPLEVEL_V6_STATE_FULLSCREEN:
             win->def_config.fullscreen = EINA_TRUE;
             break;
           case ZXDG_TOPLEVEL_V6_STATE_RESIZING:
             win->def_config.resizing = EINA_TRUE;
             break;
           case ZXDG_TOPLEVEL_V6_STATE_ACTIVATED:
             win->def_config.focused = EINA_TRUE;
             win->def_config.minimized = EINA_FALSE;
           default:
             break;
          }
     }

   if ((!width) && (!height) && (!win->def_config.fullscreen) && (!win->def_config.maximized) &&
            ((win->def_config.fullscreen != win->set_config.fullscreen) || (win->def_config.maximized != win->set_config.maximized)))
     width = win->saved.w, height = win->saved.h;

   _ecore_wl2_window_configure_send(win);
   //, width, height, !!win->def_config.resizing,
   //                                 win->def_config.fullscreen, win->def_config.maximized);

   if (win->def_config.focused)
     _ecore_wl2_window_activate_send(win);
   else
     _ecore_wl2_window_deactivate_send(win);
}

static void
_xdg_toplevel_cb_close(void *data, struct xdg_toplevel *xdg_toplevel EINA_UNUSED)
{
   Ecore_Wl2_Window *win;

   win = data;
   if (!win) return;

   ecore_wl2_window_free(win);
}

static const struct xdg_toplevel_listener _xdg_toplevel_listener =
{
   _xdg_toplevel_cb_configure,
   _xdg_toplevel_cb_close,
};

static void
_xdg_popup_cb_configure(void *data, struct xdg_popup *xdg_popup EINA_UNUSED, int32_t x EINA_UNUSED, int32_t y EINA_UNUSED, int32_t width, int32_t height)
{
   Ecore_Wl2_Window *win = data;
   win->def_config.geometry.w = width;
   win->def_config.geometry.h = height;
}

static void
_xdg_popup_cb_done(void *data, struct xdg_popup *xdg_popup EINA_UNUSED)
{
   Ecore_Wl2_Window *win;

   win = data;
   if (!win) return;

   if (win->grab) _ecore_wl2_input_ungrab(win->grab);

   _ecore_wl2_window_hide_send(win);
}

static const struct xdg_popup_listener _xdg_popup_listener =
{
   _xdg_popup_cb_configure,
   _xdg_popup_cb_done,
};

static void
_ecore_wl2_window_xdg_popup_create(Ecore_Wl2_Window *win)
{
   struct xdg_positioner *pos;

   EINA_SAFETY_ON_NULL_RETURN(win->parent);
   if (!win->parent->zxdg_surface) return;

   pos = zxdg_shell_v6_create_positioner(win->display->wl.zxdg_shell);
   if (!pos) return;

   xdg_positioner_set_anchor_rect(pos, win->set_config.geometry.x,
                                      win->set_config.geometry.y,
                                      1, 1);
   xdg_positioner_set_size(pos, win->set_config.geometry.w,
                               win->set_config.geometry.h);
   xdg_positioner_set_anchor(pos, XDG_POSITIONER_ANCHOR_TOP_LEFT);
   xdg_positioner_set_gravity(pos, ZXDG_POSITIONER_V6_ANCHOR_BOTTOM |
                                  ZXDG_POSITIONER_V6_ANCHOR_RIGHT);

   win->xdg_popup = xdg_surface_get_popup(win->xdg_surface,
                               win->parent->xdg_surface, pos);

   xdg_positioner_destroy(pos);
   if (win->grab)
     xdg_popup_grab(win->xdg_popup, win->grab->wl.seat,
                        wl_display_get_serial(win->display->wl.display));
   xdg_popup_set_user_data(win->xdg_popup, win);
   xdg_popup_add_listener(win->xdg_popup, &_xdg_popup_listener, win);

   win->pending.configure = EINA_TRUE;

   ecore_wl2_window_commit(win, EINA_TRUE);
}

static void
_window_shell_surface_create(Ecore_Wl2_Window *window)
{
   if (window->xdg_surface) return;
   window->xdg_surface =
     xdg_wm_base_get_xdg_surface(window->display->wl.xdg_wm_base,
                                   window->surface);
   xdg_surface_set_user_data(window->xdg_surface, window);
   xdg_surface_add_listener(window->xdg_surface,
                                &_xdg_surface_listener, window);

   window->xdg_configure_ack = xdg_surface_ack_configure;
   window->pending.configure = EINA_TRUE;
   if (window->display->wl.efl_hints)
     {
        if (window->aspect.set)
          efl_hints_set_aspect(window->display->wl.efl_hints, window->xdg_surface,
            window->aspect.w, window->aspect.h, window->aspect.aspect);
        if (window->weight.set)
          efl_hints_set_weight(window->display->wl.efl_hints, window->xdg_surface,
            window->weight.w, window->weight.h);
     }

   if (window->type == ECORE_WL2_WINDOW_TYPE_MENU)
     _ecore_wl2_window_xdg_popup_create(window);
   else
     {
        struct xdg_toplevel *ptop = NULL;

        window->xdg_toplevel =
          xdg_surface_get_toplevel(window->xdg_surface);
        xdg_toplevel_set_user_data(window->xdg_toplevel, window);
        xdg_toplevel_add_listener(window->xdg_toplevel,
                                      &_xdg_toplevel_listener, window);

        if (window->title)
          xdg_toplevel_set_title(window->xdg_toplevel, window->title);
        if (window->class)
          xdg_toplevel_set_app_id(window->xdg_toplevel, window->class);

        window->xdg_set_min_size = xdg_toplevel_set_min_size;
        window->xdg_set_max_size = xdg_toplevel_set_max_size;

        if (window->parent)
          ptop = window->parent->xdg_toplevel;

        if (ptop)
          xdg_toplevel_set_parent(window->xdg_toplevel, ptop);

        if (window->set_config.maximized)
          xdg_toplevel_set_maximized(window->xdg_toplevel);

        if (window->set_config.fullscreen)
          xdg_toplevel_set_fullscreen(window->xdg_toplevel, NULL);
     }

   ecore_wl2_window_commit(window, EINA_TRUE);
}

//TIZEN_ONLY(20150424) : tizen_visibility
static void
_tizen_visibility_cb_notify(void *data, struct tizen_visibility *tizen_visibility EINA_UNUSED, uint32_t visibility)
{
   Ecore_Wl2_Window *win;
   Ecore_Wl2_Event_Window_Visibility_Change *ev;

   if (!(win = data)) return;
   if (!(ev = calloc(1, sizeof(Ecore_Wl2_Event_Window_Visibility_Change)))) return;

   ev->win = win->id;
   if (visibility == TIZEN_VISIBILITY_VISIBILITY_FULLY_OBSCURED)
     ev->fully_obscured = 1;
   else
     ev->fully_obscured = 0;

   ecore_event_add(ECORE_WL2_EVENT_WINDOW_VISIBILITY_CHANGE, ev, NULL, NULL);
}

static void
_tizen_visibility_cb_changed(void *data, struct tizen_visibility *tizen_visibility EINA_UNUSED, uint32_t type, uint32_t option)
{
   Ecore_Wl2_Window *win = (Ecore_Wl2_Window *)data;
   Ecore_Wl2_Event_Window_Pre_Visibility_Change *ev;

   EINA_SAFETY_ON_NULL_RETURN(win);

   if (type == TIZEN_VISIBILITY_VISIBILITY_PRE_UNOBSCURED)
     {
        ev = calloc(1, sizeof(Ecore_Wl2_Event_Window_Pre_Visibility_Change));
        EINA_SAFETY_ON_NULL_RETURN(ev);

        ev->win = win->id;
        ev->type = ECORE_WL2_WINDOW_VISIBILITY_TYPE_PRE_UNOBSCURED;
        ev->option = option;

        ecore_event_add(ECORE_WL2_EVENT_WINDOW_PRE_VISIBILITY_CHANGE, ev, NULL, NULL);
     }
}

static const struct tizen_visibility_listener _tizen_visibility_listener =
{
   _tizen_visibility_cb_notify,
   _tizen_visibility_cb_changed,
};

static void
_tizen_position_cb_changed(void *data, struct tizen_position *tizen_position EINA_UNUSED, int32_t x, int32_t y)
{
   Ecore_Wl2_Window *win;

   if (!(win = data)) return;

   win->def_config.geometry.x = x;
   win->def_config.geometry.y = y;

   if ((x != win->set_config.geometry.x) || (y != win->set_config.geometry.y))
     {
        win->saved.x = win->set_config.geometry.x;
        win->saved.y = win->set_config.geometry.y;

        win->set_config.geometry.x = x;
        win->set_config.geometry.y = y;

        _ecore_wl2_window_configure_send(win);
     }
}

static const struct tizen_position_listener _tizen_position_listener =
{
   _tizen_position_cb_changed,
};

static void
_tizen_rotation_preferred_angle_set(Ecore_Wl2_Window *window, int rot)
{
   enum tizen_rotation_angle tz_angle = TIZEN_ROTATION_ANGLE_NONE;

   if (!window) return;

   switch (rot)
     {
      case 0:
         tz_angle = TIZEN_ROTATION_ANGLE_0;
         break;
      case 90:
         tz_angle = TIZEN_ROTATION_ANGLE_90;
         break;
      case 180:
         tz_angle = TIZEN_ROTATION_ANGLE_180;
         break;
      case 270:
         tz_angle = TIZEN_ROTATION_ANGLE_270;
         break;
      default:
         break;
     }

   if (window->tz_rotation)
     tizen_rotation_set_preferred_angle(window->tz_rotation, (uint32_t)tz_angle);
}

static void
_tizen_rotation_available_angles_set(Ecore_Wl2_Window *window, const int *rots, unsigned int count)
{
   uint32_t angles = 0;
   unsigned int i = 0;

   if (!window) return;

   for (i = 0; i < count ; i++)
     {
        switch (rots[i])
          {
           case 0:
              angles |= (uint32_t)TIZEN_ROTATION_ANGLE_0;
              break;
           case 90:
              angles |= (uint32_t)TIZEN_ROTATION_ANGLE_90;
              break;
           case 180:
              angles |= (uint32_t)TIZEN_ROTATION_ANGLE_180;
              break;
           case 270:
              angles |= (uint32_t)TIZEN_ROTATION_ANGLE_270;
              break;
           default:
              break;
          }
     }

   if (window->tz_rotation)
     tizen_rotation_set_available_angles(window->tz_rotation, angles);
}

static void
_ecore_wl_window_cb_available_angles_done(void *data EINA_UNUSED, struct tizen_rotation *tizen_rotation EINA_UNUSED, uint32_t angles EINA_UNUSED)
{
   return;
}

static void
_ecore_wl_window_cb_preferred_angle_done(void *data EINA_UNUSED, struct tizen_rotation *tizen_rotation EINA_UNUSED, uint32_t angle EINA_UNUSED)
{
   return;
}

static void
_ecore_wl_window_cb_angle_change(void *data, struct tizen_rotation *tizen_rotation EINA_UNUSED, uint32_t angle, uint32_t serial)
{
   Ecore_Wl2_Window *win;
   Ecore_Wl2_Event_Window_Rotation *ev;
   int i = 0;

   if (!(win = data)) return;
   if (!(ev = calloc(1, sizeof(Ecore_Wl2_Event_Window_Rotation)))) return;
   DBG("PendingRotation: wayland rotation callback angle=%d serial=%d", angle, serial);

   win->wm_rot.serial = serial;

   ev->win = win->id;
   ev->w = win->def_config.geometry.w;
   ev->h = win->def_config.geometry.h;

   switch (angle)
     {
      case TIZEN_ROTATION_ANGLE_0:
         ev->angle = 0;
         break;
      case TIZEN_ROTATION_ANGLE_90:
         ev->angle = 90;
         break;
      case TIZEN_ROTATION_ANGLE_180:
         ev->angle = 180;
         break;
      case TIZEN_ROTATION_ANGLE_270:
         ev->angle = 270;
         break;
      default:
         ev->angle = 0;
         break;
     }

   i = ev->angle / 90;
   if (win->wm_rot.geometry_hints[i].valid)
     {
        ev->w = win->wm_rot.geometry_hints[i].w;
        ev->h = win->wm_rot.geometry_hints[i].h;
     }

   ecore_event_add(ECORE_WL2_EVENT_WINDOW_ROTATE, ev, NULL, NULL);
// TIZEN_ONLY(20170212): pend rotation until app set rotation
   //this code don't need. This code will be added opensource code.
   //ecore_wl2_window_rotation_set(win, ev->angle);
//
}

static void
_ecore_wl_window_cb_angle_change_with_resize(void *data, struct tizen_rotation *tizen_rotation EINA_UNUSED, uint32_t angle, uint32_t serial, uint32_t width, uint32_t height)
{
   Ecore_Wl2_Window *win = (Ecore_Wl2_Window *)data;
   int rot = 0;

   EINA_SAFETY_ON_NULL_RETURN(win);
   EINA_SAFETY_ON_NULL_RETURN(win->cb_rot_changed);

   win->wm_rot.serial = serial;

   switch (angle)
     {
       case TIZEN_ROTATION_ANGLE_0:   rot =   0; break;
       case TIZEN_ROTATION_ANGLE_90:  rot =  90; break;
       case TIZEN_ROTATION_ANGLE_180: rot = 180; break;
       case TIZEN_ROTATION_ANGLE_270: rot = 270; break;
       default:                       rot =   0; break;
     }

   ecore_wl2_window_rotation_set(win, rot);

   win->cb_rot_changed(win, rot, 0, width, height, win->cb_rot_changed_data);
}

static const struct tizen_rotation_listener _ecore_tizen_rotation_listener =
{
   _ecore_wl_window_cb_available_angles_done,
   _ecore_wl_window_cb_preferred_angle_done,
   _ecore_wl_window_cb_angle_change,
   _ecore_wl_window_cb_angle_change_with_resize,
};

static void
_tizen_resource_cb_resource_id(void *data, struct tizen_resource *tizen_resource EINA_UNUSED, uint32_t id)
{
   Ecore_Wl2_Window *win;
   Ecore_Wl2_Event_Window_Show *ev;

   if (!(win = data)) return;
   if (!(ev = calloc(1, sizeof(Ecore_Wl2_Event_Window_Show)))) return;

   ev->win = win->id;
   if (win->parent)
     ev->parent_win = win->parent->id;
   else
     ev->parent_win = 0;
   ev->event_win = win->id;
   ev->data[0] = (unsigned int)id;
   win->resource_id = (unsigned int)id;
   ecore_event_add(ECORE_WL2_EVENT_WINDOW_SHOW, ev, NULL, NULL);
}

static const struct tizen_resource_listener _tizen_resource_listener =
{
   _tizen_resource_cb_resource_id,
};
//


// TIZEN_ONLY(20171112) : support tizen protocols
// to initialize wayland tizen extention protocols after surface creation
// such as tizen_policy, tizen_position, tizen_rotation, tizen_resource
// and handle transient parent(by tizen_policy) for surfaces without shell or xdg_shell
void
_ecore_wl2_window_tz_ext_init(Ecore_Wl2_Window *window)
{
   if (!window) return;
   if (!window->surface) return;
   if (!window->display) return;

   if (window->display->wl.tz_policy)
     {
        if (!window->tz_visibility)
          {
             window->tz_visibility =
               tizen_policy_get_visibility(window->display->wl.tz_policy,
                                            window->surface);
             if (!window->tz_visibility) return;
             tizen_visibility_add_listener(window->tz_visibility,
                                           &_tizen_visibility_listener,
                                           window);
          }

        if (!window->tz_position)
          {

             window->tz_position =
               tizen_policy_get_position(window->display->wl.tz_policy,
                                          window->surface);

             if (!window->tz_position) return;
             tizen_position_add_listener(window->tz_position,
                                         &_tizen_position_listener, window);
             if (window->surface)
               tizen_position_set(window->tz_position,
                                  window->saved.x, window->saved.y);
          }

        if (window->role)
          {
             if (window->surface)
               tizen_policy_set_role(window->display->wl.tz_policy,
                                     window->surface,
                                     window->role);
          }

        if (window->focus_skip)
          {
             if (window->surface)
               tizen_policy_set_focus_skip(window->display->wl.tz_policy, window->surface);
          }
        else
          {
             if (window->surface)
               tizen_policy_unset_focus_skip(window->display->wl.tz_policy, window->surface);
          }

        if (window->floating)
          {
             if (window->surface)
               tizen_policy_set_floating_mode(window->display->wl.tz_policy,
                                              window->surface);
          }
     }

   if (window->display->wl.tz_policy_ext && !window->tz_rotation)
     {
        int i = 0, x, y, w, h, rot;
        enum tizen_rotation_angle angle = TIZEN_ROTATION_ANGLE_NONE;

        window->tz_rotation =
           tizen_policy_ext_get_rotation(window->display->wl.tz_policy_ext,
                                         window->surface);
        if (!window->tz_rotation) return;

        tizen_rotation_add_listener(window->tz_rotation,
                                    &_ecore_tizen_rotation_listener, window);

        if (window->wm_rot.preferred_rot != TIZEN_ROTATION_ANGLE_NONE)
          {
             _tizen_rotation_preferred_angle_set(window, window->wm_rot.preferred_rot);
          }

        if (window->wm_rot.count)
          {
             _tizen_rotation_available_angles_set(window, window->wm_rot.available_rots, window->wm_rot.count);
          }

        rot = ecore_wl2_window_rotation_get(window);
        for (i = 0; i <= 3; i++)
          {
             if (!window->wm_rot.geometry_hints[i].valid) continue;

             x = window->wm_rot.geometry_hints[i].x;
             y = window->wm_rot.geometry_hints[i].y;
             w = window->wm_rot.geometry_hints[i].w;
             h = window->wm_rot.geometry_hints[i].h;

             switch (i)
               {
                case 0:
                   angle = TIZEN_ROTATION_ANGLE_0;
                   break;
                case 1:
                   angle = TIZEN_ROTATION_ANGLE_90;
                   break;
                case 2:
                   angle = TIZEN_ROTATION_ANGLE_180;
                   break;
                case 3:
                   angle = TIZEN_ROTATION_ANGLE_270;
                   break;
               }
             tizen_rotation_set_geometry_hint(window->tz_rotation,
                                              (uint32_t)angle, x, y, w, h);

             if ((rot == (i * 90)) &&
                 ((window->set_config.geometry.w != w) || (window->set_config.geometry.h != h)))
               {
                  //TIZEN_ONLY(20180201) : add function to set window size by client.
                  window->set_config.geometry.w = w;
                  window->set_config.geometry.h = h;
                  _ecore_wl2_window_configure_send_by_client(window);
                  //
               }
          }
     }

   if (window->display->wl.tz_surf && !window->tz_resource)
     {
        window->tz_resource =
          tizen_surface_get_tizen_resource(window->display->wl.tz_surf, window->surface);
        if (!window->tz_resource) return;

        tizen_resource_add_listener(window->tz_resource,
                                    &_tizen_resource_listener, window);

     }

   if (window->parent)
     {
        if (window->zxdg_surface && window->parent->zxdg_surface)
          {
             // already handled above code
          }
        else if (window->display->wl.tz_policy && window->surface && window->parent->surface)
          {
             uint32_t ver = wl_proxy_get_version((struct wl_proxy *)window->display->wl.tz_policy);
             if (ver >= 3)
               tizen_policy_set_parent(window->display->wl.tz_policy, window->surface, window->parent->surface);
          }
     }
}
// END of TIZEN_ONLY(20171112) : support tizen protocols

void
_ecore_wl2_window_shell_surface_init(Ecore_Wl2_Window *window)
{
   if (!window->surface) return;
   if (window->display->wl.xdg_wm_base) _window_shell_surface_create(window);
   if (window->display->wl.zxdg_shell) _window_v6_shell_surface_create(window);

   if (window->display->wl.session_recovery)
     {
        if (window->uuid)
          {
             zwp_e_session_recovery_set_uuid(window->display->wl.session_recovery,
                                             window->surface, window->uuid);
             if (window->xdg_surface)
               xdg_surface_set_window_geometry(window->xdg_surface,
                                                   window->set_config.geometry.x,
                                                   window->set_config.geometry.y,
                                                   window->set_config.geometry.w,
                                                   window->set_config.geometry.h);
             if (window->zxdg_surface)
               zxdg_surface_v6_set_window_geometry(window->zxdg_surface,
                                                   window->set_config.geometry.x,
                                                   window->set_config.geometry.y,
                                                   window->set_config.geometry.w,
                                                   window->set_config.geometry.h);

             ecore_wl2_window_opaque_region_set(window,
                                                window->opaque.x,
                                                window->opaque.y,
                                                window->opaque.w,
                                                window->opaque.h);
          }
        else
          zwp_e_session_recovery_get_uuid(window->display->wl.session_recovery, window->surface);
     }
}

static void
_surface_enter(void *data, struct wl_surface *surf EINA_UNUSED, struct wl_output *op)
{
   Ecore_Wl2_Window *win;
   Ecore_Wl2_Output *output;

   win = data;

   output = _ecore_wl2_output_find(win->display, op);
   EINA_SAFETY_ON_NULL_RETURN(output);

   win->outputs = eina_list_append(win->outputs, output);
}

static void
_surface_leave(void *data, struct wl_surface *surf EINA_UNUSED, struct wl_output *op)
{
   Ecore_Wl2_Window *win;
   Ecore_Wl2_Output *output;

   win = data;
   output = _ecore_wl2_output_find(win->display, op);
   EINA_SAFETY_ON_NULL_RETURN(output);

   win->outputs = eina_list_remove(win->outputs, output);
   if (!win->outputs)
     {
        Ecore_Wl2_Event_Window_Offscreen *ev;
        ev = calloc(1, sizeof(Ecore_Wl2_Event_Window_Offscreen));
        if (ev)
          {
             ev->win = win->id;
             ecore_event_add(ECORE_WL2_EVENT_WINDOW_OFFSCREEN, ev, NULL, NULL);
          }
     }
}

static const struct wl_surface_listener _surface_listener =
{
   _surface_enter,
   _surface_leave,
};

void
_ecore_wl2_window_surface_create(Ecore_Wl2_Window *window)
{
   if (!window->display->wl.compositor) return;

   if (!window->surface)
     {
        window->surface =
          wl_compositor_create_surface(window->display->wl.compositor);
        if (!window->surface)
          {
             ERR("Failed to create surface for window");
             return;
          }
        wl_surface_set_user_data(window->surface, window);

        //TIZEN_ONLY(20171115): support output transform
        if (window->display->wl.tz_screen_rotation)
          tizen_screen_rotation_get_ignore_output_transform(window->display->wl.tz_screen_rotation, window->surface);
        //

        window->surface_id =
          wl_proxy_get_id((struct wl_proxy *)window->surface);

        wl_surface_add_listener(window->surface, &_surface_listener, window);
        if (window->display->wl.efl_aux_hints)
          {
             efl_aux_hints_get_supported_aux_hints(window->display->wl.efl_aux_hints, window->surface);
             if (_ecore_wl2_display_sync_get())
               wl_display_roundtrip(window->display->wl.display);
          }
     }
}

static void
_ecore_wl2_window_show_send(Ecore_Wl2_Window *window)
{
   Ecore_Wl2_Event_Window_Show *ev;

   ev = calloc(1, sizeof(Ecore_Wl2_Event_Window_Show));
   if (!ev) return;

   ev->win = window->id;
   if (window->parent)
     ev->parent_win = window->parent->id;
   ev->event_win = window->id;
   ecore_event_add(ECORE_WL2_EVENT_WINDOW_SHOW, ev, NULL, NULL);
}

static void
_ecore_wl2_window_hide_send(Ecore_Wl2_Window *window)
{
   Ecore_Wl2_Event_Window_Hide *ev;

   ev = calloc(1, sizeof(Ecore_Wl2_Event_Window_Hide));
   if (!ev) return;

   ev->win = window->id;
   if (window->parent)
     ev->parent_win = window->parent->id;
   ev->event_win = window->id;
   ecore_event_add(ECORE_WL2_EVENT_WINDOW_HIDE, ev, NULL, NULL);
}

EAPI Ecore_Wl2_Window *
ecore_wl2_window_new(Ecore_Wl2_Display *display, Ecore_Wl2_Window *parent, int x, int y, int w, int h)
{
   Ecore_Wl2_Window *win;
   static int _win_id = 1;

   EINA_SAFETY_ON_NULL_RETURN_VAL(display, NULL);
   if (display->pid) CRI("CANNOT CREATE WINDOW WITH SERVER DISPLAY");

   /* try to allocate space for window structure */
   win = calloc(1, sizeof(Ecore_Wl2_Window));
   if (!win) return NULL;
   display->refs++;

   win->display = display;
   win->parent = parent;
   win->id = _win_id++;

   win->set_config.geometry.x = x;
   win->set_config.geometry.y = y;
   win->set_config.geometry.w = w;
   win->set_config.geometry.h = h;

   win->opaque.x = x;
   win->opaque.y = y;
   win->opaque.w = w;
   win->opaque.h = h;

   // TIZEN_ONLY(20180202) : Set default type to keep backward compat.
   win->type = ECORE_WL2_WINDOW_TYPE_TOPLEVEL;
   // END

   //TIZEN_ONLY(20171115): support output transform
   win->ignore_output_transform = EINA_TRUE;
   //

   win->pending.configure = EINA_TRUE;
   display->windows =
     eina_inlist_append(display->windows, EINA_INLIST_GET(win));

   _ecore_wl2_window_surface_create(win);

//TIZEN_ONLY(20171216): add ecore_wl2_window_find
   eina_hash_add(_windows, _ecore_wl2_window_id_str_get(win->id), win);
//

   _ecore_wl2_input_devices_send(ecore_wl2_input_default_input_get(display), win);

   return win;
}

EAPI int
ecore_wl2_window_id_get(Ecore_Wl2_Window *window)
{
   EINA_SAFETY_ON_NULL_RETURN_VAL(window, -1);
   return window->id;
}

EAPI struct wl_surface *
ecore_wl2_window_surface_get(Ecore_Wl2_Window *window)
{
   EINA_SAFETY_ON_NULL_RETURN_VAL(window, NULL);

   _ecore_wl2_window_surface_create(window);

   return window->surface;
}

EAPI int
ecore_wl2_window_surface_id_get(Ecore_Wl2_Window *window)
{
   EINA_SAFETY_ON_NULL_RETURN_VAL(window, -1);
   return window->surface_id;
}

EAPI void
ecore_wl2_window_show(Ecore_Wl2_Window *window)
{
   EINA_SAFETY_ON_NULL_RETURN(window);

   _ecore_wl2_window_surface_create(window);

   if (window->input_set)
     ecore_wl2_window_input_region_set(window, window->input_rect.x,
                                       window->input_rect.y,
                                       window->input_rect.w,
                                       window->input_rect.h);
   if (window->opaque_set)
     ecore_wl2_window_opaque_region_set(window, window->opaque.x,
                                        window->opaque.y, window->opaque.w,
                                        window->opaque.h);

   if ((window->type != ECORE_WL2_WINDOW_TYPE_DND) &&
       (window->type != ECORE_WL2_WINDOW_TYPE_NONE))
     {
        _ecore_wl2_window_shell_surface_init(window);
        _ecore_wl2_window_tz_ext_init(window);  // TIZEN_ONLY(20171112) : support tizen protocols after surface creation
        _ecore_wl2_window_www_surface_init(window);
        _ecore_wl2_window_show_send(window);
     }
   else
      {
        _ecore_wl2_window_tz_ext_init(window); // TIZEN_ONLY(20171112) : support tizen protocols after surface creation
        _configure_complete(window);
     }
 }

EAPI void
ecore_wl2_window_hide(Ecore_Wl2_Window *window)
{
   Ecore_Wl2_Subsurface *subsurf;
   Eina_Inlist *tmp;

   EINA_SAFETY_ON_NULL_RETURN(window);

   _ecore_wl2_window_hide_send(window);

   // TIZEN_ONLY(20171112) : support tizen protocols
   if (window->tz_visibility) tizen_visibility_destroy(window->tz_visibility);
   window->tz_visibility = NULL;

   if (window->tz_position) tizen_position_destroy(window->tz_position);
   window->tz_position = NULL;

   if (window->tz_rotation) tizen_rotation_destroy(window->tz_rotation);
   window->tz_rotation = NULL;

   if (window->tz_resource) tizen_resource_destroy(window->tz_resource);
   window->tz_resource = NULL;
   //

   EINA_INLIST_FOREACH_SAFE(window->subsurfs, tmp, subsurf)
     _ecore_wl2_subsurf_unmap(subsurf);

   if (window->commit_pending)
     {
        /* We've probably been hidden while an animator
         * is ticking.  Cancel the callback.
         */
        window->commit_pending = EINA_FALSE;
        if (window->callback)
          {
             wl_callback_destroy(window->callback);
             window->callback = NULL;
          }
     }

   if (window->surface)
     {
        wl_surface_attach(window->surface, NULL, 0, 0);
        ecore_wl2_window_commit(window, EINA_TRUE);
        window->commit_pending = EINA_FALSE;
     }

   /* The commit added a callback, disconnect it */
   if (window->callback)
     {
        wl_callback_destroy(window->callback);
        window->callback = NULL;
     }

   window->set_config.serial = 0;
   window->req_config.serial = 0;
   window->def_config.serial = 0;
   window->zxdg_configure_ack = NULL;
   window->xdg_configure_ack = NULL;
   window->xdg_set_min_size = NULL;
   window->xdg_set_max_size = NULL;
   window->zxdg_set_min_size = NULL;
   window->zxdg_set_max_size = NULL;
}

static void
_ecore_wl2_window_aux_hint_free(Ecore_Wl2_Window *win)
{
   const char *supported;

   EINA_LIST_FREE(win->supported_aux_hints, supported)
     if (supported) eina_stringshare_del(supported);
}

EAPI void
ecore_wl2_window_free(Ecore_Wl2_Window *window)
{
   Ecore_Wl2_Display *display;
   Ecore_Wl2_Input *input;
   Ecore_Wl2_Subsurface *subsurf;
   Eina_Inlist *tmp;

   EINA_SAFETY_ON_NULL_RETURN(window);

//TIZEN_ONLY(20171216): add ecore_wl2_window_find
   eina_hash_del(_windows, _ecore_wl2_window_id_str_get(window->id), window);
//

   display = window->display;

   EINA_INLIST_FOREACH(display->inputs, input)
      _ecore_wl2_input_window_remove(input, window);

   EINA_INLIST_FOREACH_SAFE(window->subsurfs, tmp, subsurf)
     _ecore_wl2_subsurf_free(subsurf);

   _ecore_wl2_window_aux_hint_free(window);

   // TIZEN_ONLY(20171112) : support tizen protocols
   if (window->tz_visibility) tizen_visibility_destroy(window->tz_visibility);
   window->tz_visibility = NULL;

   if (window->tz_position) tizen_position_destroy(window->tz_position);
   window->tz_position = NULL;

   if (window->tz_rotation) tizen_rotation_destroy(window->tz_rotation);
   window->tz_rotation = NULL;

   if (window->tz_resource) tizen_resource_destroy(window->tz_resource);
   window->tz_resource = NULL;
   //

   if (window->callback) wl_callback_destroy(window->callback);
   window->callback = NULL;

   if (window->uuid && window->surface && window->display->wl.session_recovery)
     zwp_e_session_recovery_destroy_uuid(window->display->wl.session_recovery,
                                         window->surface, window->uuid);

   _ecore_wl2_window_semi_free(window);

   eina_stringshare_replace(&window->uuid, NULL);

   if (window->title) eina_stringshare_del(window->title);
   if (window->class) eina_stringshare_del(window->class);
   if (window->role) eina_stringshare_del(window->role);
   // TIZEN_ONLY(20160201) : support to handle input rectangle
   if (window->input_region) wl_region_destroy(window->input_region);
   window->input_region = NULL;
   //

   display->windows =
     eina_inlist_remove(display->windows, EINA_INLIST_GET(window));

   ecore_wl2_display_disconnect(window->display);
   free(window);
}

EAPI void
ecore_wl2_window_move(Ecore_Wl2_Window *window, Ecore_Wl2_Input *input)
{
   EINA_SAFETY_ON_NULL_RETURN(window);
   EINA_SAFETY_ON_NULL_RETURN(window->display->inputs);

   if (!input)
     {
        ERR("NULL input parameter is deprecated");
        input = EINA_INLIST_CONTAINER_GET(window->display->inputs, Ecore_Wl2_Input);
     }
   if (window->xdg_toplevel)
     xdg_toplevel_move(window->xdg_toplevel, input->wl.seat,
                           window->display->serial);
   if (window->zxdg_toplevel)
     zxdg_toplevel_v6_move(window->zxdg_toplevel, input->wl.seat,
                           window->display->serial);
   ecore_wl2_display_flush(window->display);

   _ecore_wl2_input_ungrab(input);
}

EAPI void
ecore_wl2_window_resize(Ecore_Wl2_Window *window, Ecore_Wl2_Input *input, int location)
{
   EINA_SAFETY_ON_NULL_RETURN(window);
   EINA_SAFETY_ON_NULL_RETURN(window->display->inputs);

   if (!input)
     {
        ERR("NULL input parameter is deprecated");
        input = EINA_INLIST_CONTAINER_GET(window->display->inputs, Ecore_Wl2_Input);
     }

   if (window->xdg_toplevel)
     xdg_toplevel_resize(window->xdg_toplevel, input->wl.seat,
                             window->display->serial, location);
   if (window->zxdg_toplevel)
     zxdg_toplevel_v6_resize(window->zxdg_toplevel, input->wl.seat,
                             window->display->serial, location);
   ecore_wl2_display_flush(window->display);

   _ecore_wl2_input_ungrab(input);
}

EAPI void
ecore_wl2_window_raise(Ecore_Wl2_Window *window)
{
   EINA_SAFETY_ON_NULL_RETURN(window);
   EINA_SAFETY_ON_NULL_RETURN(window->display);

// TIZEN_ONLY(20171114)
/*
   if (window->zxdg_toplevel)
     {
        struct wl_array states;
        uint32_t *s;

        wl_array_init(&states);
        s = wl_array_add(&states, sizeof(*s));
        *s = ZXDG_TOPLEVEL_V6_STATE_ACTIVATED;
        _zxdg_toplevel_cb_configure(window, window->zxdg_toplevel,
                                    window->set_config.geometry.w,
                                    window->set_config.geometry.h, &states);
        wl_array_release(&states);
     }
*/
   if ((window->surface) && (window->display->wl.tz_policy))
     tizen_policy_raise(window->display->wl.tz_policy, window->surface);
//
}

// TIZEN_ONLY(20171108): lower window function from ecore_wayland to ecore_wl2
EAPI Ecore_Wl2_Window *
ecore_wl2_window_surface_find(struct wl_surface *surface)
{
   Ecore_Wl2_Display *ewd;
   Ecore_Wl2_Window *window;

   EINA_SAFETY_ON_NULL_RETURN_VAL(surface, NULL);

   ewd = ecore_wl2_connected_display_get(NULL);
   if (!ewd) return NULL;

   window = ecore_wl2_display_window_find_by_surface(ewd, surface);
   return window;
}

EAPI void
ecore_wl2_window_lower(Ecore_Wl2_Window *window)
{
   Ecore_Wl2_Event_Window_Lower *ev;

   EINA_SAFETY_ON_NULL_RETURN(window);
   EINA_SAFETY_ON_NULL_RETURN(window->display);

   /* FIXME: This should lower the xdg surface also */
   if (window->display->wl.tz_policy)
     {
        tizen_policy_lower(window->display->wl.tz_policy, window->surface);

        if (!(ev = calloc(1, sizeof(Ecore_Wl2_Event_Window_Lower)))) return;

        ev->win = window->id;
        ecore_event_add(ECORE_WL2_EVENT_WINDOW_LOWER, ev, NULL, NULL);
     }
}

EAPI void
ecore_wl2_window_activate(Ecore_Wl2_Window *window)
{
   Eina_Bool iconic;

   EINA_SAFETY_ON_NULL_RETURN(window);
   EINA_SAFETY_ON_NULL_RETURN(window->display);

   iconic = ecore_wl2_window_iconified_get(window);
   if (iconic)
     ecore_wl2_window_iconified_set(window, EINA_FALSE);

   if (window->display->wl.tz_policy)
     tizen_policy_activate(window->display->wl.tz_policy, window->surface);
}

EAPI void
ecore_wl2_window_parent_set(Ecore_Wl2_Window *window, Ecore_Wl2_Window *parent)
{
   EINA_SAFETY_ON_NULL_RETURN(window);
   window->parent = parent;

// TIZEN_ONLY(20171114)
   if (window->parent)
     {
        if (window->zxdg_surface && window->parent->zxdg_surface)
          {
             struct zxdg_toplevel_v6 *ptop = NULL;

             ptop = window->parent->zxdg_toplevel;
             if (ptop)
               zxdg_toplevel_v6_set_parent(window->zxdg_toplevel, ptop);
          }
        else if (window->display->wl.tz_policy && window->surface && window->parent->surface)
          {
             uint32_t ver = wl_proxy_get_version((struct wl_proxy *)window->display->wl.tz_policy);
             if (ver >= 3)
               tizen_policy_set_parent(window->display->wl.tz_policy, window->surface, window->parent->surface);
          }
     }
   else
     {
        if (window->zxdg_surface)
          zxdg_toplevel_v6_set_parent(window->zxdg_toplevel, NULL);
     }
//
}

EAPI void
ecore_wl2_window_stack_mode_set(Ecore_Wl2_Window *window, Ecore_Wl2_Window_Stack_Mode mode)
{
   EINA_SAFETY_ON_NULL_RETURN(window);
   EINA_SAFETY_ON_NULL_RETURN(window->display);

   if ((window->surface) && (window->display->wl.tz_policy))
     tizen_policy_set_stack_mode(window->display->wl.tz_policy, window->surface, mode);
}

EAPI void
ecore_wl2_window_position_set(Ecore_Wl2_Window *window, int x, int y)
{
   EINA_SAFETY_ON_NULL_RETURN(window);

   window->saved.x = x;
   window->saved.y = y;

   window->set_config.geometry.x = x;
   window->set_config.geometry.y = y;

   if ((window->surface) && (window->tz_position))
     {
        if ((window->def_config.geometry.x != x) || (window->def_config.geometry.y != y))
          tizen_position_set(window->tz_position, x, y);
     }
}

EAPI int
ecore_wl2_window_active_angle_get(Ecore_Wl2_Window *window)
{
   struct wl_surface *surface = NULL;

   EINA_SAFETY_ON_NULL_RETURN_VAL(window, 0);
   EINA_SAFETY_ON_NULL_RETURN_VAL(window->display, 0);

   if (!window->display->wl.tz_policy_ext) return 0;

   surface = ecore_wl2_window_surface_get(window);

   tizen_policy_ext_get_active_angle(window->display->wl.tz_policy_ext, surface);
   ecore_wl2_display_sync(window->display);

   return window->display->active_angle;
}
//

EAPI Eina_Bool
ecore_wl2_window_alpha_get(Ecore_Wl2_Window *window)
{
   EINA_SAFETY_ON_NULL_RETURN_VAL(window, EINA_FALSE);

   return window->alpha;
}

EAPI void
ecore_wl2_window_alpha_set(Ecore_Wl2_Window *window, Eina_Bool alpha)
{
   Ecore_Wl2_Surface *surf = NULL;

   EINA_SAFETY_ON_NULL_RETURN(window);

   if (window->alpha == alpha) return;

   window->alpha = alpha;
   surf = window->wl2_surface;
   if (surf)
     ecore_wl2_surface_reconfigure(surf, surf->w, surf->h, 0, alpha);

// TIZEN_ONLY(20170203)
   if (!window->alpha)
     ecore_wl2_window_opaque_region_set(window, window->opaque.x,
                                        window->opaque.y, window->opaque.w,
                                        window->opaque.h);
   else if (window->surface)
     wl_surface_set_opaque_region(window->surface, NULL);
//
}

EAPI void
ecore_wl2_window_transparent_set(Ecore_Wl2_Window *window, Eina_Bool transparent)
{
   EINA_SAFETY_ON_NULL_RETURN(window);

   if (window->transparent == transparent) return;

   window->transparent = transparent;

   if (!window->transparent)
     ecore_wl2_window_opaque_region_set(window, window->opaque.x,
                                        window->opaque.y, window->opaque.w,
                                        window->opaque.h);
// TIZEN_ONLY(20170203)
/*
   else
     ecore_wl2_window_opaque_region_set(window, 0, 0, 0, 0);
*/
   else if (window->surface)
     wl_surface_set_opaque_region(window->surface, NULL);
//
}

// TIZEN_ONLY(20171108) : Get a window's transparent property
EAPI Eina_Bool
ecore_wl2_window_transparent_get(Ecore_Wl2_Window *window)
{
   EINA_SAFETY_ON_NULL_RETURN_VAL(window, EINA_FALSE);

   return window->transparent;
}
//

EAPI void
ecore_wl2_window_opaque_region_set(Ecore_Wl2_Window *window, int x, int y, int w, int h)
{
   int nx = 0, ny = 0, nw = 0, nh = 0;

   EINA_SAFETY_ON_NULL_RETURN(window);

   switch (window->rotation)
     {
      case 0:
        nx = x;
        ny = y;
        nw = w;
        nh = h;
        break;
      case 90:
        nx = y;
        ny = x;
        nw = h;
        nh = w;
        break;
      case 180:
        nx = x;
        ny = x + y;
        nw = w;
        nh = h;
        break;
      case 270:
        nx = x + y;
        ny = x;
        nw = h;
        nh = w;
        break;
      default:
        break;
     }

   if ((window->opaque.x == nx) && (window->opaque.y == ny) &&
       (window->opaque.w == nw) && (window->opaque.h == nh))
     return;

   window->opaque.x = nx;
   window->opaque.y = ny;
   window->opaque.w = nw;
   window->opaque.h = nh;

// TIZEN_ONLY(20180412) : check transparent and alpha value
   if ((window->transparent) || (window->alpha))
     return;
//

   window->opaque_set = x || y || w || h;
   window->pending.opaque = EINA_TRUE;
}

// TIZEN_ONLY(20160201) : support to handle input rectangle
EAPI void
ecore_wl2_window_input_rect_set(Ecore_Wl2_Window *win, Eina_Rectangle *input_rect)
{
   if (!win) return;
   if (!input_rect) return;
   if (win->input_region)
     {
        wl_region_destroy(win->input_region);
        win->input_region = NULL;
     }

   win->input_rect.x = input_rect->x;
   win->input_rect.y = input_rect->y;
   win->input_rect.w = input_rect->w;
   win->input_rect.h = input_rect->h;

   if (win->type != ECORE_WL2_WINDOW_TYPE_DND)
     {
        struct wl_region *region;
        region = wl_compositor_create_region(win->display->wl.compositor);
        if (!region) return;

        wl_region_add(region, input_rect->x, input_rect->y, input_rect->w, input_rect->h);
        wl_surface_set_input_region(win->surface, region);
        wl_region_destroy(region);
     }
}

EAPI void
ecore_wl2_window_input_rect_add(Ecore_Wl2_Window *win, Eina_Rectangle *input_rect)
{
   if (!win) return;
   if (!input_rect) return;
   if (input_rect->x < 0 || input_rect->y < 0) return;

   if (win->type != ECORE_WL2_WINDOW_TYPE_DND)
     {
        if (!win->input_region)
          {
             struct wl_region *region;
             region = wl_compositor_create_region(win->display->wl.compositor);
             if (!region) return;

             win->input_region = region;
          }

        wl_region_add(win->input_region, input_rect->x, input_rect->y, input_rect->w, input_rect->h);
        wl_surface_set_input_region(win->surface, win->input_region);
     }
}

EAPI void
ecore_wl2_window_input_rect_subtract(Ecore_Wl2_Window *win, Eina_Rectangle *input_rect)
{
   if (!win) return;
   if (!input_rect) return;
   if (input_rect->x < 0 || input_rect->y < 0) return;
   if (!win->input_region) return;

   if (win->type != ECORE_WL2_WINDOW_TYPE_DND)
     {
        wl_region_subtract(win->input_region, input_rect->x, input_rect->y, input_rect->w, input_rect->h);
        wl_surface_set_input_region(win->surface, win->input_region);
     }
}
//

EAPI void
ecore_wl2_window_input_region_set(Ecore_Wl2_Window *window, int x, int y, int w, int h)
{
   int nx = 0, ny = 0, nw = 0, nh = 0;

   EINA_SAFETY_ON_NULL_RETURN(window);

   switch (window->rotation)
     {
      case 0:
        nx = x;
        ny = y;
        nw = w;
        nh = h;
        break;
      case 90:
        nx = y;
        ny = x;
        nw = h;
        nh = w;
        break;
      case 180:
        nx = x;
        ny = x + y;
        nw = w;
        nh = h;
        break;
      case 270:
        nx = x + y;
        ny = x;
        nw = h;
        nh = w;
        break;
      default:
        break;
     }

   if ((window->input_rect.x == nx) && (window->input_rect.y == ny) &&
       (window->input_rect.w == nw) && (window->input_rect.h == nh))
     return;

   window->input_rect.x = nx;
   window->input_rect.y = ny;
   window->input_rect.w = nw;
   window->input_rect.h = nh;
   window->input_set = x || y || w || h;
   window->pending.input = EINA_TRUE;
}

EAPI Eina_Bool
ecore_wl2_window_maximized_get(Ecore_Wl2_Window *window)
{
   EINA_SAFETY_ON_NULL_RETURN_VAL(window, EINA_FALSE);

   return window->set_config.maximized;
}

EAPI void
ecore_wl2_window_maximized_set(Ecore_Wl2_Window *window, Eina_Bool maximized)
{
   Eina_Bool prev;

   EINA_SAFETY_ON_NULL_RETURN(window);

   prev = window->set_config.maximized;
   maximized = !!maximized;
   if (prev == maximized) return;

   window->set_config.maximized = maximized;
   if (window->updating)
     {
        window->pending.maximized = EINA_TRUE;
        return;
     }

   if (maximized)
     {
        if (!window->set_config.fullscreen)
          window->saved = window->set_config.geometry;

        if (window->xdg_toplevel)
          xdg_toplevel_set_maximized(window->xdg_toplevel);
        if (window->zxdg_toplevel)
          zxdg_toplevel_v6_set_maximized(window->zxdg_toplevel);
     }
   else
     {
        if (window->xdg_toplevel)
          xdg_toplevel_unset_maximized(window->xdg_toplevel);
        if (window->zxdg_toplevel)
          zxdg_toplevel_v6_unset_maximized(window->zxdg_toplevel);
//TIZEN_ONLY(20150625)
        _ecore_wl2_window_configure_send(window);
//
     }
   ecore_wl2_display_flush(window->display);
}

EAPI Eina_Bool
ecore_wl2_window_fullscreen_get(Ecore_Wl2_Window *window)
{
   EINA_SAFETY_ON_NULL_RETURN_VAL(window, EINA_FALSE);

   return window->set_config.fullscreen;
}

EAPI void
ecore_wl2_window_fullscreen_set(Ecore_Wl2_Window *window, Eina_Bool fullscreen)
{
   Eina_Bool prev;

   EINA_SAFETY_ON_NULL_RETURN(window);

   prev = window->set_config.fullscreen;
   fullscreen = !!fullscreen;
   if (prev == fullscreen) return;

   window->set_config.fullscreen = fullscreen;
   if (window->updating)
     {
        window->pending.fullscreen = EINA_TRUE;
        return;
     }

   if (fullscreen)
     {
        if (!window->set_config.maximized)
          window->saved = window->set_config.geometry;

        if (window->xdg_toplevel)
          xdg_toplevel_set_fullscreen(window->xdg_toplevel, NULL);
        if (window->zxdg_toplevel)
          zxdg_toplevel_v6_set_fullscreen(window->zxdg_toplevel, NULL);
     }
   else
     {
        if (window->xdg_toplevel)
          xdg_toplevel_unset_fullscreen(window->xdg_toplevel);
        if (window->zxdg_toplevel)
          zxdg_toplevel_v6_unset_fullscreen(window->zxdg_toplevel);
//TIZEN_ONLY(20150625)
        _ecore_wl2_window_configure_send(window);
//
     }
   ecore_wl2_display_flush(window->display);
}

EAPI int
ecore_wl2_window_rotation_get(Ecore_Wl2_Window *window)
{
   EINA_SAFETY_ON_NULL_RETURN_VAL(window, -1);

   return window->rotation;
}

EAPI void
ecore_wl2_window_rotation_set(Ecore_Wl2_Window *window, int rotation)
{
   EINA_SAFETY_ON_NULL_RETURN(window);

   window->rotation = rotation;
}

EAPI void
ecore_wl2_window_title_set(Ecore_Wl2_Window *window, const char *title)
{
   EINA_SAFETY_ON_NULL_RETURN(window);

   eina_stringshare_replace(&window->title, title);
   if (!window->title) return;
   if (!window->xdg_toplevel && !window->xdg_toplevel) return;

   if (window->xdg_toplevel)
     xdg_toplevel_set_title(window->xdg_toplevel, window->title);
   if (window->zxdg_toplevel)
     zxdg_toplevel_v6_set_title(window->zxdg_toplevel, window->title);
   ecore_wl2_display_flush(window->display);
}

EAPI void
ecore_wl2_window_class_set(Ecore_Wl2_Window *window, const char *clas)
{
   EINA_SAFETY_ON_NULL_RETURN(window);

   eina_stringshare_replace(&window->class, clas);
   if (!window->class) return;
   if (!window->xdg_toplevel && !window->xdg_toplevel) return;

   if (window->xdg_toplevel)
     xdg_toplevel_set_app_id(window->xdg_toplevel, window->class);
   if (window->zxdg_toplevel)
     zxdg_toplevel_v6_set_app_id(window->zxdg_toplevel, window->class);
   ecore_wl2_display_flush(window->display);
}

EAPI void
ecore_wl2_window_geometry_get(Ecore_Wl2_Window *window, int *x, int *y, int *w, int *h)
{
   EINA_SAFETY_ON_NULL_RETURN(window);

   if (x) *x = window->set_config.geometry.x;
   if (y) *y = window->set_config.geometry.y;
   if (w) *w = window->set_config.geometry.w;
   if (h) *h = window->set_config.geometry.h;
}

EAPI void
ecore_wl2_window_geometry_set(Ecore_Wl2_Window *window, int x, int y, int w, int h)
{
   EINA_SAFETY_ON_NULL_RETURN(window);

   if ((window->set_config.geometry.x == x) &&
       (window->set_config.geometry.y == y) &&
       (window->set_config.geometry.w == w) &&
       (window->set_config.geometry.h == h))
     return;

   window->set_config.geometry.x = x;
   window->set_config.geometry.y = y;
   window->set_config.geometry.w = w;
   window->set_config.geometry.h = h;
   window->pending.geom = EINA_TRUE;
}

EAPI Eina_Bool
ecore_wl2_window_iconified_get(Ecore_Wl2_Window *window)
{
   EINA_SAFETY_ON_NULL_RETURN_VAL(window, EINA_FALSE);

   // TIZEN_ONLY(20150822)
   if (window->display->wl.tz_policy)
     return window->iconified;
   else
   //
   return window->set_config.minimized;
}

// TIZEN_ONLY(20151231) : handling iconic state on tizen
static void
_ecore_wl2_window_iconified_set(Ecore_Wl2_Window *window, Eina_Bool iconified, Eina_Bool send_event)
{
   Eina_Bool prev;

   EINA_SAFETY_ON_NULL_RETURN(window);

   prev = window->set_config.minimized;
   iconified = !!iconified;
   if (prev == iconified) return;

   window->set_config.minimized = iconified;

   if (iconified)
     {
        if ((window->surface) && (window->display->wl.tz_policy))
          {
             if (send_event)
               tizen_policy_iconify(window->display->wl.tz_policy, window->surface);
             window->iconified = EINA_TRUE;
          }

        else if (window->zxdg_toplevel)
          zxdg_toplevel_v6_set_minimized(window->zxdg_toplevel);
     }
   else
     {
        if ((window->surface) && (window->display->wl.tz_policy))
          {
             if (send_event)
               tizen_policy_uniconify(window->display->wl.tz_policy, window->surface);
             window->iconified = EINA_FALSE;
          }

        else if (window->zxdg_toplevel)
          {
             struct wl_array states;
             uint32_t *s;

             wl_array_init(&states);
             s = wl_array_add(&states, sizeof(*s));
             *s = ZXDG_TOPLEVEL_V6_STATE_ACTIVATED;
             _zxdg_toplevel_cb_configure(window, window->zxdg_toplevel,
                                         window->saved.w,
                                         window->saved.h, &states);
             wl_array_release(&states);
          }
     }

   if (send_event)
     {
        Ecore_Wl2_Event_Window_Iconify_State_Change *ev;

        if (!(ev = calloc(1, sizeof(Ecore_Wl2_Event_Window_Iconify_State_Change)))) return;
        ev->win = window->id;
        ev->iconified = iconified;
        ev->force = 0;
        ecore_event_add(ECORE_WL2_EVENT_WINDOW_ICONIFY_STATE_CHANGE, ev, NULL, NULL);
     }
}
//

EAPI void
ecore_wl2_window_iconified_set(Ecore_Wl2_Window *window, Eina_Bool iconified)
{
   // TIZEN_ONLY(20151231) : handling iconic state on tizen
   /*
   Eina_Bool prev;

   EINA_SAFETY_ON_NULL_RETURN(window);

   prev = window->set_config.minimized;
   iconified = !!iconified;
   if (prev == iconified) return;

   window->set_config.minimized = iconified;

   if (iconified)
     {
        if (window->xdg_toplevel)
          xdg_toplevel_set_minimized(window->xdg_toplevel);
        if (window->zxdg_toplevel)
          zxdg_toplevel_v6_set_minimized(window->zxdg_toplevel);
        ecore_wl2_display_flush(window->display);
     }
   else
     {
        if (window->xdg_toplevel)
          {
             struct wl_array states;
             uint32_t *s;

             wl_array_init(&states);
             s = wl_array_add(&states, sizeof(*s));
             *s = XDG_TOPLEVEL_STATE_ACTIVATED;
             _xdg_toplevel_cb_configure(window, window->xdg_toplevel,
                                         window->set_config.geometry.w,
                                         window->set_config.geometry.h, &states);
             wl_array_release(&states);
          }
        if (window->zxdg_toplevel)
          {
             struct wl_array states;
             uint32_t *s;

             wl_array_init(&states);
             s = wl_array_add(&states, sizeof(*s));
             *s = ZXDG_TOPLEVEL_V6_STATE_ACTIVATED;
             _zxdg_toplevel_cb_configure(window, window->zxdg_toplevel,
                                         window->set_config.geometry.w,
                                         window->set_config.geometry.h, &states);
             wl_array_release(&states);
          }
     }
   */
   _ecore_wl2_window_iconified_set(window, iconified, EINA_TRUE);
   //
}

// TIZEN_ONLY(20151231) : handling iconic state on tizen
EAPI void
ecore_wl2_window_iconify_state_update(Ecore_Wl2_Window *window, Eina_Bool iconified, Eina_Bool send_event)
{
   _ecore_wl2_window_iconified_set(window, iconified, send_event);
}
//

//TIZEN_ONLY(20171216): add ecore_wl2_window_find
EAPI Ecore_Wl2_Window *
ecore_wl2_window_find(unsigned int id)
{
   Ecore_Wl2_Window *win = NULL;

   if (!_windows) return NULL;
   win = eina_hash_find(_windows, _ecore_wl2_window_id_str_get(id));
   return win;
}
//

//TIZEN_ONLY(20180202): Fix to send a proper window type value to the server
static inline uint32_t
_convert_to_tzsh_type(Ecore_Wl2_Window_Type type)
{
   switch (type)
     {
        case ECORE_WL2_WINDOW_TYPE_NONE:
          return TIZEN_POLICY_WIN_TYPE_NONE;
        case ECORE_WL2_WINDOW_TYPE_TOPLEVEL:
          return TIZEN_POLICY_WIN_TYPE_TOPLEVEL;
        case ECORE_WL2_WINDOW_TYPE_MENU:
          return TIZEN_POLICY_WIN_TYPE_MENU;
        case ECORE_WL2_WINDOW_TYPE_DND:
          return TIZEN_POLICY_WIN_TYPE_DND;
        case ECORE_WL2_WINDOW_TYPE_CUSTOM:
          return TIZEN_POLICY_WIN_TYPE_CUSTOM;
        case ECORE_WL2_WINDOW_TYPE_NOTIFICATION:
          return TIZEN_POLICY_WIN_TYPE_NOTIFICATION;
        case ECORE_WL2_WINDOW_TYPE_UTILITY:
          return TIZEN_POLICY_WIN_TYPE_UTILITY;
        case ECORE_WL2_WINDOW_TYPE_DIALOG:
          return TIZEN_POLICY_WIN_TYPE_DIALOG;
        case ECORE_WL2_WINDOW_TYPE_DOCK:
          return TIZEN_POLICY_WIN_TYPE_DOCK;
        case ECORE_WL2_WINDOW_TYPE_SPLASH:
          return TIZEN_POLICY_WIN_TYPE_SPLASH;
        default:
          return TIZEN_POLICY_WIN_TYPE_NONE;
     }
}
//

EAPI void
ecore_wl2_window_type_set(Ecore_Wl2_Window *window, Ecore_Wl2_Window_Type type)
{
   EINA_SAFETY_ON_NULL_RETURN(window);
   window->type = type;

   // TIZEN_ONLY(20180202): Fix to send a proper window type value to the server
   uint32_t tzsh_type;
   tzsh_type = _convert_to_tzsh_type(type);
   //

// TIZEN_ONLY(20171114)
   if ((window->surface) && (window->display->wl.tz_policy))
     tizen_policy_set_type(window->display->wl.tz_policy, window->surface, (uint32_t)tzsh_type);
//
}

// TIZEN_ONLY(20171108) : Get the type of a given window
EAPI Ecore_Wl2_Window_Type
ecore_wl2_window_type_get(Ecore_Wl2_Window *window)
{
   EINA_SAFETY_ON_NULL_RETURN_VAL(window, ECORE_WL2_WINDOW_TYPE_NONE);
   return window->type;
}
//

EAPI void
ecore_wl2_window_popup_input_set(Ecore_Wl2_Window *window, Ecore_Wl2_Input *input)
{
   EINA_SAFETY_ON_NULL_RETURN(window);
   EINA_SAFETY_ON_NULL_RETURN(input);
   EINA_SAFETY_ON_TRUE_RETURN(window->type != ECORE_WL2_WINDOW_TYPE_MENU);
   window->grab = input;
}

EAPI Ecore_Wl2_Display *
ecore_wl2_window_display_get(const Ecore_Wl2_Window *window)
{
   EINA_SAFETY_ON_NULL_RETURN_VAL(window, NULL);
   EINA_SAFETY_ON_NULL_RETURN_VAL(window->display, NULL);

   if (window->display->recovering) return NULL;

   return window->display;
}

EAPI Eina_Bool
ecore_wl2_window_shell_surface_exists(Ecore_Wl2_Window *window)
{
   EINA_SAFETY_ON_NULL_RETURN_VAL(window, EINA_FALSE);
   return !!window->zxdg_surface || !!window->xdg_surface;
}

// TIZEN_ONLY(171108) : get shell surface of a given window
EAPI struct zxdg_surface_v6 *
ecore_wl2_window_shell_surface_get(Ecore_Wl2_Window *window)
{
   EINA_SAFETY_ON_NULL_RETURN_VAL(window, NULL);
   return window->zxdg_surface;
}
//

EAPI Eina_Bool
ecore_wl2_window_activated_get(const Ecore_Wl2_Window *window)
{
   EINA_SAFETY_ON_NULL_RETURN_VAL(window, EINA_FALSE);
   return window->req_config.focused;
}

EAPI Ecore_Wl2_Output *
ecore_wl2_window_output_find(Ecore_Wl2_Window *window)
{
   Ecore_Wl2_Output *out;
   Eina_Inlist *tmp;
   int x = 0, y = 0;

   EINA_SAFETY_ON_NULL_RETURN_VAL(window, NULL);

   x = window->set_config.geometry.x;
   y = window->set_config.geometry.y;

   EINA_INLIST_FOREACH_SAFE(window->display->outputs, tmp, out)
     {
        int ox, oy, ow, oh;

        ox = out->geometry.x;
        oy = out->geometry.y;

        switch (out->transform)
          {
           case WL_OUTPUT_TRANSFORM_90:
           case WL_OUTPUT_TRANSFORM_270:
           case WL_OUTPUT_TRANSFORM_FLIPPED_90:
           case WL_OUTPUT_TRANSFORM_FLIPPED_270:
             ow = out->geometry.h;
             oh = out->geometry.w;
             break;
           default:
             ow = out->geometry.w;
             oh = out->geometry.h;
             break;
          }

        if (((x >= ox) && (x < ow)) && ((y >= oy) && (y < oh)))
          return out;
     }

   return NULL;
}

EAPI void
ecore_wl2_window_buffer_transform_set(Ecore_Wl2_Window *window, int transform)
{
   EINA_SAFETY_ON_NULL_RETURN(window);

   wl_surface_set_buffer_transform(window->surface, transform);
}

EAPI void
ecore_wl2_window_wm_rotation_supported_set(Ecore_Wl2_Window *window, Eina_Bool enabled)
{
   EINA_SAFETY_ON_NULL_RETURN(window);
   window->wm_rot.supported = enabled;
}

EAPI Eina_Bool
ecore_wl2_window_wm_rotation_supported_get(Ecore_Wl2_Window *window)
{
   EINA_SAFETY_ON_NULL_RETURN_VAL(window, EINA_FALSE);
   return window->wm_rot.supported;
}

EAPI void
ecore_wl2_window_rotation_app_set(Ecore_Wl2_Window *window, Eina_Bool set)
{
   EINA_SAFETY_ON_NULL_RETURN(window);
   window->wm_rot.app_set = set;
}

EAPI Eina_Bool
ecore_wl2_window_rotation_app_get(Ecore_Wl2_Window *window)
{
   EINA_SAFETY_ON_NULL_RETURN_VAL(window, EINA_FALSE);
   return window->wm_rot.app_set;
}

EAPI void
ecore_wl2_window_preferred_rotation_set(Ecore_Wl2_Window *window, int rot)
{
   EINA_SAFETY_ON_NULL_RETURN(window);
   window->wm_rot.preferred_rot = rot;

   // TIZEN_ONLY
   _tizen_rotation_preferred_angle_set(window, window->wm_rot.preferred_rot);
   //
}

EAPI int
ecore_wl2_window_preferred_rotation_get(Ecore_Wl2_Window *window)
{
   EINA_SAFETY_ON_NULL_RETURN_VAL(window, 0);
   return window->wm_rot.preferred_rot;
}

EAPI void
ecore_wl2_window_available_rotations_set(Ecore_Wl2_Window *window, const int *rots, unsigned int count)
{
   EINA_SAFETY_ON_NULL_RETURN(window);

   // TIZEN_ONLY
   _tizen_rotation_available_angles_set(window, rots, count);
   //
   window->wm_rot.count = count;
   window->wm_rot.available_rots = (int *)rots;
}

EAPI Eina_Bool
ecore_wl2_window_available_rotations_get(Ecore_Wl2_Window *window, int **rots, unsigned int *count)
{
   unsigned int i = 0;
   int *val = NULL;

   EINA_SAFETY_ON_NULL_RETURN_VAL(window, EINA_FALSE);

   *rots = NULL;
   *count = window->wm_rot.count;

   if (window->wm_rot.count >= 1)
     {
        val = calloc(window->wm_rot.count, sizeof(int));
        if (!val) return EINA_FALSE;

        for (; i < window->wm_rot.count; i++)
          val[i] = ((int *)window->wm_rot.available_rots)[i];

        *rots = val;
        return EINA_TRUE;
     }

   return EINA_FALSE;
}

EAPI void
ecore_wl2_window_rotation_change_prepare_send(Ecore_Wl2_Window *window, int rot, int w, int h, Eina_Bool resize)
{
   Ecore_Wl2_Event_Window_Rotation_Change_Prepare *ev;

   EINA_SAFETY_ON_NULL_RETURN(window);

   ev = calloc(1, sizeof(Ecore_Wl2_Event_Window_Rotation_Change_Prepare));
   if (!ev) return;

   ev->win = window->id;
   ev->rotation = rot;
   ev->w = w;
   ev->h = h;
   ev->resize = resize;

   ecore_event_add(ECORE_WL2_EVENT_WINDOW_ROTATION_CHANGE_PREPARE, ev, NULL, NULL);
}

EAPI void
ecore_wl2_window_rotation_change_prepare_done_send(Ecore_Wl2_Window *window, int rot)
{
   Ecore_Wl2_Event_Window_Rotation_Change_Prepare_Done *ev;

   EINA_SAFETY_ON_NULL_RETURN(window);

   ev = calloc(1, sizeof(Ecore_Wl2_Event_Window_Rotation_Change_Prepare_Done));
   if (!ev) return;

   ev->win = window->id;
   ev->rotation = rot;
   ev->w = 0;
   ev->h = 0;
   ev->resize = 0;

   ecore_event_add(ECORE_WL2_EVENT_WINDOW_ROTATION_CHANGE_PREPARE_DONE,
                   ev, NULL, NULL);
}

EAPI void
ecore_wl2_window_rotation_change_request_send(Ecore_Wl2_Window *window, int rot)
{
   Ecore_Wl2_Event_Window_Rotation_Change_Request *ev;

   EINA_SAFETY_ON_NULL_RETURN(window);

   ev = calloc(1, sizeof(Ecore_Wl2_Event_Window_Rotation_Change_Request));
   if (!ev) return;

   ev->win = window->id;
   ev->rotation = rot;
   ev->w = 0;
   ev->h = 0;
   ev->resize = 0;

   ecore_event_add(ECORE_WL2_EVENT_WINDOW_ROTATION_CHANGE_REQUEST,
                   ev, NULL, NULL);
}

EAPI void
ecore_wl2_window_rotation_change_done_send(Ecore_Wl2_Window *window, int rot, int w, int h)
{
   Ecore_Wl2_Event_Window_Rotation_Change_Done *ev;

   EINA_SAFETY_ON_NULL_RETURN(window);

   // TIZEN_ONLY
   if (window->tz_rotation)
     tizen_rotation_ack_angle_change(window->tz_rotation, window->wm_rot.serial);
   //

   ev = calloc(1, sizeof(Ecore_Wl2_Event_Window_Rotation_Change_Done));
   if (!ev) return;

   ev->win = window->id;
   ev->rotation = rot;
   ev->w = w;
   ev->h = h;
   ev->resize = 0;

   ecore_event_add(ECORE_WL2_EVENT_WINDOW_ROTATION_CHANGE_DONE,
                   ev, NULL, NULL);
}

// TIZEN_ONLY
EAPI void
ecore_wl2_window_rotation_geometry_set(Ecore_Wl2_Window *win, int rot, int x, int y, int w, int h)
{
   int i = 0;
   int rotation = 0;
   enum tizen_rotation_angle angle = TIZEN_ROTATION_ANGLE_NONE;

   if (!win) return;

   if ((rot % 90 != 0) || (rot / 90 > 3) || (rot < 0)) return;

   i = rot / 90;
   win->wm_rot.geometry_hints[i].x = x;
   win->wm_rot.geometry_hints[i].y = y;
   win->wm_rot.geometry_hints[i].w = w;
   win->wm_rot.geometry_hints[i].h = h;
   win->wm_rot.geometry_hints[i].valid = EINA_TRUE;

   if (!win->tz_rotation) return;
   switch (rot)
     {
      case 0:
         angle = TIZEN_ROTATION_ANGLE_0;
         break;
      case 90:
         angle = TIZEN_ROTATION_ANGLE_90;
         break;
      case 180:
         angle = TIZEN_ROTATION_ANGLE_180;
         break;
      case 270:
         angle = TIZEN_ROTATION_ANGLE_270;
         break;
      default:
         break;
     }
   tizen_rotation_set_geometry_hint(win->tz_rotation,
                                    (uint32_t)angle, x, y, w, h);

   rotation = ecore_wl2_window_rotation_get(win);
   if ((rotation % 90 != 0) || (rotation / 90 > 3) || (rotation < 0)) return;
   if ((i == (rotation / 90)) &&
       ((win->set_config.geometry.w != w) || (win->set_config.geometry.h != h)))
     {
        //TIZEN_ONLY(20180201) : add function to set window size by client.
        win->set_config.geometry.w = w;
        win->set_config.geometry.h = h;
        _ecore_wl2_window_configure_send_by_client(win);
        //
     }
}

EAPI void
ecore_wl2_window_rotation_changed_callback_set(Ecore_Wl2_Window *win, void *data, void (*func)(Ecore_Wl2_Window *win, int rot, Eina_Bool resize, int w, int h, void *data))
{
   if (!win) return;

   win->cb_rot_changed = func;
   win->cb_rot_changed_data = data;
}
//

EAPI Eina_List *
ecore_wl2_window_aux_hints_supported_get(Ecore_Wl2_Window *win)
{
   Eina_List *res = NULL;
   Eina_List *ll;
   char *supported_hint = NULL;
   const char *hint = NULL;

   if (!win) return NULL;
   if (!win->surface) return NULL;

   // TIZEN_ONLY : To use tizen protocols
   if (win->display->wl.tz_policy)
     {
        tizen_policy_get_supported_aux_hints(win->display->wl.tz_policy, win->surface);
        ecore_wl2_display_sync(win->display);
     }
   //

   EINA_LIST_FOREACH(win->supported_aux_hints, ll, supported_hint)
     {
        hint = eina_stringshare_add(supported_hint);
        res = eina_list_append(res, hint);
     }
   return res;
}

EAPI void
ecore_wl2_window_aux_hint_add(Ecore_Wl2_Window *win, int id, const char *hint, const char *val)
{
   if (!win) return;
   if ((!win->surface) || (!win->display->wl.efl_aux_hints)) return;

   efl_aux_hints_add_aux_hint(win->display->wl.efl_aux_hints, win->surface, id, hint, val);
   ecore_wl2_display_flush(win->display);

   // TIZEN_ONLY : To use tizen protocols
   if ((win->surface) && (win->display->wl.tz_policy))
     tizen_policy_add_aux_hint(win->display->wl.tz_policy, win->surface, id, hint, val);
   //
}

EAPI void
ecore_wl2_window_aux_hint_change(Ecore_Wl2_Window *win, int id, const char *val)
{
   if (!win) return;
   if ((!win->surface) && (!win->display->wl.efl_aux_hints)) return;

   efl_aux_hints_change_aux_hint(win->display->wl.efl_aux_hints, win->surface, id, val);
   ecore_wl2_display_flush(win->display);

   // TIZEN_ONLY : To use tizen protocols
   if ((win->surface) && (win->display->wl.tz_policy))
     tizen_policy_change_aux_hint(win->display->wl.tz_policy, win->surface, id, val);
   //
}

EAPI void
ecore_wl2_window_aux_hint_del(Ecore_Wl2_Window *win, int id)
{
   if (!win) return;
   if ((!win->surface) || (!win->display->wl.efl_aux_hints)) return;

   efl_aux_hints_del_aux_hint(win->display->wl.efl_aux_hints, win->surface, id);
   ecore_wl2_display_flush(win->display);

   // TIZEN_ONLY : To use tizen protocols
   if ((win->surface) && (win->display->wl.tz_policy))
     tizen_policy_del_aux_hint(win->display->wl.tz_policy, win->surface, id);
   //
}

EAPI void
ecore_wl2_window_focus_skip_set(Ecore_Wl2_Window *window, Eina_Bool focus_skip)
{
   EINA_SAFETY_ON_NULL_RETURN(window);

   // TIZEN_ONLY(20171112)
   if (window->focus_skip != focus_skip)
     {
        if ((window->surface) && (window->display->wl.tz_policy))
          {
             if (focus_skip)
               tizen_policy_set_focus_skip(window->display->wl.tz_policy, window->surface);
             else
               tizen_policy_unset_focus_skip(window->display->wl.tz_policy, window->surface);
          }
     }
   //

   window->focus_skip = focus_skip;
}

EAPI Eina_Bool
ecore_wl2_window_focus_skip_get(Ecore_Wl2_Window *window)
{
   EINA_SAFETY_ON_NULL_RETURN_VAL(window, EINA_FALSE);
   return window->focus_skip;
}

EAPI void
ecore_wl2_window_role_set(Ecore_Wl2_Window *window, const char *role)
{
   EINA_SAFETY_ON_NULL_RETURN(window);
   eina_stringshare_replace(&window->role, role);

   // TIZEN_ONLY(20171112)
   if ((window->surface) && (window->display->wl.tz_policy))
     tizen_policy_set_role(window->display->wl.tz_policy, window->surface, window->role);
   //
}

EAPI void
ecore_wl2_window_floating_mode_set(Ecore_Wl2_Window *window, Eina_Bool floating)
{
   EINA_SAFETY_ON_NULL_RETURN(window);
   window->floating = floating;

   // TIZEN_ONLY(20171112)
   if ((window->surface) && (window->display->wl.tz_policy))
     {
        if (floating)
          tizen_policy_set_floating_mode(window->display->wl.tz_policy,
                                         window->surface);
        else
          tizen_policy_unset_floating_mode(window->display->wl.tz_policy,
                                           window->surface);
     }
   //
}

EAPI Eina_Bool
ecore_wl2_window_floating_mode_get(Ecore_Wl2_Window *window)
{
   EINA_SAFETY_ON_NULL_RETURN_VAL(window, EINA_FALSE);
   return window->floating;
}

// TIZEN_ONLY(20150703) : support conformant
EAPI void
ecore_wl2_window_conformant_set(Ecore_Wl2_Window *win, unsigned int is_conformant)
{
   if (!win) return;
   if (!win->surface) return;
   if (!win->display) return;
   if (!win->display->wl.tz_policy) return;

   if (is_conformant)
     tizen_policy_set_conformant(win->display->wl.tz_policy, win->surface);
   else
     tizen_policy_unset_conformant(win->display->wl.tz_policy, win->surface);
}

EAPI Eina_Bool
ecore_wl2_window_conformant_get(Ecore_Wl2_Window *win)
{
   if (!win) return EINA_FALSE;
   if (!win->surface) return EINA_FALSE;
   if (!win->display) return EINA_FALSE;
   if (!win->display->wl.tz_policy) return EINA_FALSE;

   tizen_policy_get_conformant(win->display->wl.tz_policy, win->surface);
   ecore_wl2_display_sync(win->display);

   return win->conformant;
}
//

// TIZEN_ONLY(20171108) : add functions for indicator
EAPI void
ecore_wl2_window_indicator_geometry_set(Ecore_Wl2_Window *win, int x, int y, int w, int h)
{
   if(!win) return;

   win->indicator.x = x;
   win->indicator.y = y;
   win->indicator.w = w;
   win->indicator.h = h;
}

EAPI Eina_Bool
ecore_wl2_window_indicator_geometry_get(Ecore_Wl2_Window *win, int *x, int *y, int *w, int *h)
{
   if(!win) return EINA_FALSE;

   if (x)
     *x = win->indicator.x;
   if (y)
     *y = win->indicator.y;
   if (w)
     *w = win->indicator.w;
   if (h)
     *h = win->indicator.h;

   return EINA_TRUE;
}

EAPI void
ecore_wl2_window_indicator_state_set(Ecore_Wl2_Window *win, Ecore_Wl2_Indicator_State state)
{
   int32_t ind_state;

   if (!win) return;
   if (!win->surface) return;
   if (!win->display->wl.tz_indicator) return;

   if (state == ECORE_WL2_INDICATOR_STATE_ON)
     ind_state = TIZEN_INDICATOR_STATE_ON;
   else if (state == ECORE_WL2_INDICATOR_STATE_OFF)
     ind_state = TIZEN_INDICATOR_STATE_OFF;
   else
     ind_state = TIZEN_INDICATOR_STATE_UNKNOWN;

   tizen_indicator_set_state(win->display->wl.tz_indicator, win->surface, ind_state);

   win->indicator.state = state;
}

EAPI Ecore_Wl2_Indicator_State
ecore_wl2_window_indicator_state_get(Ecore_Wl2_Window *win)
{
   if (!win) return EINA_FALSE;

   return win->indicator.state;
}

EAPI void
ecore_wl2_window_indicator_opacity_set(Ecore_Wl2_Window *win, Ecore_Wl2_Indicator_Opacity_Mode mode)
{
   int32_t op_mode;

   if (!win) return;
   if (!win->surface) return;
   if (!win->display->wl.tz_indicator) return;

   switch (mode)
     {
      case ECORE_WL2_INDICATOR_OPAQUE:
         op_mode = TIZEN_INDICATOR_OPACITY_MODE_OPAQUE;
         break;

      case ECORE_WL2_INDICATOR_TRANSLUCENT:
         op_mode = TIZEN_INDICATOR_OPACITY_MODE_TRANSLUCENT;
         break;

      case ECORE_WL2_INDICATOR_TRANSPARENT:
         op_mode = TIZEN_INDICATOR_OPACITY_MODE_TRANSPARENT;
         break;

      case ECORE_WL2_INDICATOR_BG_TRANSPARENT:
         op_mode = TIZEN_INDICATOR_OPACITY_MODE_BG_TRANSPARENT;
         break;

      default:
         op_mode = TIZEN_INDICATOR_OPACITY_MODE_OPAQUE;
         break;
     }

   tizen_indicator_set_opacity_mode(win->display->wl.tz_indicator, win->surface, op_mode);

   win->indicator.mode = mode;
}

EAPI Ecore_Wl2_Indicator_Opacity_Mode
ecore_wl2_window_indicator_opacity_get(Ecore_Wl2_Window *win)
{
   if (!win) return EINA_FALSE;

   return win->indicator.mode;
}

EAPI void
ecore_wl2_indicator_visible_type_set(Ecore_Wl2_Window *win, Ecore_Wl2_Indicator_Visible_Type type)
{
   int32_t vis_type;

   if (!win) return;
   if (!win->surface) return;
   if (!win->display->wl.tz_indicator) return;

   if (type == ECORE_WL2_INDICATOR_VISIBLE_TYPE_SHOWN)
     vis_type = TIZEN_INDICATOR_VISIBLE_TYPE_SHOWN;
   else
     vis_type = TIZEN_INDICATOR_VISIBLE_TYPE_HIDDEN;

   tizen_indicator_set_visible_type(win->display->wl.tz_indicator, win->surface, vis_type);

   win->indicator.type = type;
}

EAPI Ecore_Wl2_Indicator_Visible_Type
ecore_wl2_indicator_visible_type_get(Ecore_Wl2_Window *win)
{
   if (!win) return EINA_FALSE;

   return win->indicator.type;
}
//

// TIZEN_ONLY(20171108) : add functions for clipboard
EAPI void
ecore_wl2_window_clipboard_geometry_set(Ecore_Wl2_Window *win, int x, int y, int w, int h)
{
   if(!win) return;

   win->clipboard.x = x;
   win->clipboard.y = y;
   win->clipboard.w = w;
   win->clipboard.h = h;
}

EAPI Eina_Bool
ecore_wl2_window_clipboard_geometry_get(Ecore_Wl2_Window *win, int *x, int *y, int *w, int *h)
{
   if (!win) return EINA_FALSE;

   if (x)
     *x = win->clipboard.x;
   if (y)
     *y = win->clipboard.y;
   if (w)
     *w = win->clipboard.w;
   if (h)
     *h = win->clipboard.h;

   return EINA_TRUE;
}

EAPI void
ecore_wl2_window_clipboard_state_set(Ecore_Wl2_Window *win, Ecore_Wl2_Clipboard_State state)
{
   if (!win) return;

   win->clipboard.state = state;
}

EAPI Ecore_Wl2_Clipboard_State
ecore_wl2_window_clipboard_state_get(Ecore_Wl2_Window *win)
{
   if (!win) return EINA_FALSE;

   return win->clipboard.state;
}

EAPI void
ecore_wl2_clipboard_show(Ecore_Wl2_Window *win)
{
   if (!win) return;
   if (!win->surface) return;
   if (!win->display->wl.tz_clipboard) return;

   tizen_clipboard_show(win->display->wl.tz_clipboard, win->surface);
}

EAPI void
ecore_wl2_clipboard_hide(Ecore_Wl2_Window *win)
{
   if (!win) return;
   if (!win->surface) return;
   if (!win->display->wl.tz_clipboard) return;

   tizen_clipboard_hide(win->display->wl.tz_clipboard, win->surface);
}

EAPI Eina_Bool
ecore_wl2_clipboard_data_only_set(Eina_Bool data_only)
{
   Ecore_Wl2_Display *ewd = NULL;
   Ecore_Wl2_Input *input = NULL;

   ewd = ecore_wl2_connected_display_get(NULL);
   if (!ewd) return EINA_FALSE;
   input = ecore_wl2_input_default_input_get(ewd);
   if (!input) return EINA_FALSE;

   if (!ewd->wl.tz_clipboard) return EINA_FALSE;

   tizen_clipboard_set_data_only(ewd->wl.tz_clipboard, data_only);
   ecore_wl2_display_sync(ewd);

   return input->is_data_only;
}
//

// TIZEN_ONLY(20171108) : add functions for keyboard
EAPI void
ecore_wl2_window_keyboard_geometry_set(Ecore_Wl2_Window *win, int x, int y, int w, int h)
{
   if(!win) return;

   win->keyboard.x= x;
   win->keyboard.y = y;
   win->keyboard.w = w;
   win->keyboard.h = h;
}

EAPI Eina_Bool
ecore_wl2_window_keyboard_geometry_get(Ecore_Wl2_Window *win, int *x, int *y, int *w, int *h)
{
   if (!win) return EINA_FALSE;

   if (x)
     *x = win->keyboard.x;
   if (y)
     *y = win->keyboard.y;
   if (w)
     *w = win->keyboard.w;
   if (h)
     *h = win->keyboard.h;

   return EINA_TRUE;
}

EAPI void
ecore_wl2_window_keyboard_state_set(Ecore_Wl2_Window *win, Ecore_Wl2_Virtual_Keyboard_State state)
{
   if (!win) return;

   win->keyboard.state = state;
}

EAPI Ecore_Wl2_Virtual_Keyboard_State
ecore_wl2_window_keyboard_state_get(Ecore_Wl2_Window *win)
{
   if (!win) return EINA_FALSE;

   return win->keyboard.state;
}
//

EAPI void
ecore_wl2_window_aspect_set(Ecore_Wl2_Window *window, int w, int h, unsigned int aspect)
{
   EINA_SAFETY_ON_NULL_RETURN(window);
   EINA_SAFETY_ON_TRUE_RETURN(w < 1);
   EINA_SAFETY_ON_TRUE_RETURN(h < 1);

   if ((window->aspect.aspect == aspect) && (window->aspect.w == w) &&
       (window->aspect.h == h))
     return;

   window->aspect.w = w;
   window->aspect.h = h;
   window->aspect.aspect = aspect;
   window->aspect.set = 1;
   if (!window->display->wl.efl_hints) return;
   if (window->xdg_surface)
     efl_hints_set_aspect(window->display->wl.efl_hints,
                          window->xdg_surface, w, h, aspect);
   ecore_wl2_display_flush(window->display);
}

EAPI void
ecore_wl2_window_weight_set(Ecore_Wl2_Window *window, double w, double h)
{
   int ww, hh;
   EINA_SAFETY_ON_NULL_RETURN(window);

   ww = lround(w * 100);
   hh = lround(h * 100);

   if ((window->weight.w == ww) && (window->weight.h == hh))
     return;

   window->weight.w = ww;
   window->weight.h = hh;
   window->weight.set = 1;
   if (!window->display->wl.efl_hints) return;
   if (window->xdg_surface)
     efl_hints_set_weight(window->display->wl.efl_hints,
                          window->xdg_surface, ww, hh);
   ecore_wl2_display_flush(window->display);
}

static void
_frame_cb(void *data, struct wl_callback *callback, uint32_t timestamp)
{
   Ecore_Wl2_Frame_Cb_Handle *cb;
   Ecore_Wl2_Window *window;
   Eina_List *l, *ll;

   window = data;
   window->commit_pending = EINA_FALSE;
   wl_callback_destroy(callback);
   window->callback = NULL;
   EINA_LIST_FOREACH_SAFE(window->frame_callbacks, l, ll, cb)
     cb->cb(window, timestamp, cb->data);
}

static struct wl_callback_listener _frame_listener =
{
   _frame_cb
};

static void
_maximized_set(Ecore_Wl2_Window *window)
{
   EINA_SAFETY_ON_FALSE_RETURN(window->zxdg_toplevel || window->xdg_toplevel);

   if (window->set_config.maximized)
     {
        window->saved = window->set_config.geometry;
        if (window->xdg_toplevel)
          xdg_toplevel_set_maximized(window->xdg_toplevel);
        if (window->zxdg_toplevel)
          zxdg_toplevel_v6_set_maximized(window->zxdg_toplevel);
     }
   else
     {
        if (window->xdg_toplevel)
          xdg_toplevel_unset_maximized(window->xdg_toplevel);
        if (window->zxdg_toplevel)
          zxdg_toplevel_v6_unset_maximized(window->zxdg_toplevel);
     }
}

static void
_fullscreen_set(Ecore_Wl2_Window *window)
{
   EINA_SAFETY_ON_FALSE_RETURN(window->zxdg_toplevel || window->xdg_toplevel);

   if (window->set_config.fullscreen)
     {
        window->saved = window->set_config.geometry;
        if (window->xdg_toplevel)
          xdg_toplevel_set_fullscreen(window->xdg_toplevel, NULL);
        if (window->zxdg_toplevel)
          zxdg_toplevel_v6_set_fullscreen(window->zxdg_toplevel, NULL);
     }
   else
     {
        if (window->xdg_toplevel)
          xdg_toplevel_unset_fullscreen(window->xdg_toplevel);
        if (window->zxdg_toplevel)
          zxdg_toplevel_v6_unset_fullscreen(window->zxdg_toplevel);
     }
}

static struct wl_region *
_region_create(struct wl_compositor *comp, int x, int y, int w, int h)
{
   struct wl_region *out;

   out = wl_compositor_create_region(comp);
   if (!out)
     {
        ERR("Failed to create region");
        return NULL;
     }

   wl_region_add(out, x, y, w, h);

   return out;
}

static void
_regions_set(Ecore_Wl2_Window *window)
{
   struct wl_region *region = NULL;

   if (window->pending.opaque)
     {
        if (window->opaque_set)
          {
             region = _region_create(window->display->wl.compositor,
                                     window->opaque.x, window->opaque.y,
                                     window->opaque.w, window->opaque.h);
             if (!region) return;
          }
        wl_surface_set_opaque_region(window->surface, region);
     }

   if (!window->pending.input) goto out;
   if (window->type == ECORE_WL2_WINDOW_TYPE_DND) goto out;

   if (!window->input_set)
     {
        wl_surface_set_input_region(window->surface, NULL);
        goto out;
     }

   if (region && (window->opaque.x == window->input_rect.x) &&
       (window->opaque.y == window->input_rect.y) &&
       (window->opaque.w == window->input_rect.w) &&
       (window->opaque.h == window->input_rect.h))
     {
        wl_surface_set_input_region(window->surface, region);
        goto out;
     }
   if (region) wl_region_destroy(region);

   region = _region_create(window->display->wl.compositor,
                           window->input_rect.x, window->input_rect.y,
                           window->input_rect.w, window->input_rect.h);
   if (!region) return;
   wl_surface_set_input_region(window->surface, region);

out:
   if (region) wl_region_destroy(region);
}

EAPI void
ecore_wl2_window_commit(Ecore_Wl2_Window *window, Eina_Bool flush)
{
   EINA_SAFETY_ON_NULL_RETURN(window);
   EINA_SAFETY_ON_NULL_RETURN(window->surface);

   if (window->commit_pending)
     {
        wl_callback_destroy(window->callback);
        if (window->callback)
          wl_callback_destroy(window->callback);
        window->callback = NULL;
        WRN("Commit before previous commit processed");
     }
   if (!window->pending.configure)
     {
        if (window->has_buffer)
          window->commit_pending = EINA_TRUE;
        window->callback = wl_surface_frame(window->surface);
        wl_callback_add_listener(window->callback, &_frame_listener, window);
        /* Dispatch any state we've been saving along the way */
        if (window->pending.geom)
          {
             if (window->xdg_toplevel)
               xdg_surface_set_window_geometry(window->xdg_surface,
                                                   window->set_config.geometry.x,
                                                   window->set_config.geometry.y,
                                                   window->set_config.geometry.w,
                                                   window->set_config.geometry.h);
             if (window->zxdg_surface)
               zxdg_surface_v6_set_window_geometry(window->zxdg_surface,
                                                   window->set_config.geometry.x,
                                                   window->set_config.geometry.y,
                                                   window->set_config.geometry.w,
                                                   window->set_config.geometry.h);
          }
        if (window->pending.opaque || window->pending.input)
          _regions_set(window);

        if (window->pending.maximized)
          _maximized_set(window);

        if (window->pending.fullscreen)
          _fullscreen_set(window);

        window->pending.geom = EINA_FALSE;
        window->pending.opaque = EINA_FALSE;
        window->pending.input = EINA_FALSE;
        window->pending.maximized = EINA_FALSE;
        window->pending.fullscreen = EINA_FALSE;
     }

   if (window->req_config.serial != window->set_config.serial)
     {
        if (window->xdg_configure_ack)
           window->xdg_configure_ack(window->xdg_surface,
                                      window->req_config.serial);
        if (window->zxdg_configure_ack)
           window->zxdg_configure_ack(window->zxdg_surface,
                                      window->req_config.serial);
        window->set_config.serial = window->req_config.serial;
     }
   if (flush)
     {
        wl_surface_commit(window->surface);
        ecore_wl2_display_flush(window->display);
     }

   if (!window->updating) return;

   window->updating = EINA_FALSE;
   if (window->def_config.serial != window->set_config.serial)
      _ecore_wl2_window_configure_send(window);
}

EAPI void
ecore_wl2_window_false_commit(Ecore_Wl2_Window *window)
{
   EINA_SAFETY_ON_NULL_RETURN(window);
   EINA_SAFETY_ON_NULL_RETURN(window->surface);
   EINA_SAFETY_ON_TRUE_RETURN(window->pending.configure);
   EINA_SAFETY_ON_TRUE_RETURN(window->commit_pending);

   window->callback = wl_surface_frame(window->surface);
   wl_callback_add_listener(window->callback, &_frame_listener, window);
   wl_surface_commit(window->surface);
   ecore_wl2_display_flush(window->display);
   if (window->has_buffer)
     window->commit_pending = EINA_TRUE;
}

EAPI Eina_Bool
ecore_wl2_window_pending_get(Ecore_Wl2_Window *window)
{
   EINA_SAFETY_ON_NULL_RETURN_VAL(window, EINA_FALSE);

   return window->commit_pending;
}

EAPI Ecore_Wl2_Frame_Cb_Handle *
ecore_wl2_window_frame_callback_add(Ecore_Wl2_Window *window, Ecore_Wl2_Frame_Cb cb, void *data)
{
   Ecore_Wl2_Frame_Cb_Handle *callback;

   EINA_SAFETY_ON_NULL_RETURN_VAL(window, NULL);
   EINA_SAFETY_ON_NULL_RETURN_VAL(cb, NULL);

   callback = malloc(sizeof(*callback));
   EINA_SAFETY_ON_NULL_RETURN_VAL(callback, NULL);
   callback->cb = cb;
   callback->data = data;
   callback->win = window;
   window->frame_callbacks =
     eina_list_append(window->frame_callbacks, callback);
   return callback;
}

EAPI void
ecore_wl2_window_frame_callback_del(Ecore_Wl2_Frame_Cb_Handle *handle)
{
   EINA_SAFETY_ON_NULL_RETURN(handle);

   handle->win->frame_callbacks =
     eina_list_remove(handle->win->frame_callbacks, handle);
   free(handle);
}

EAPI void
ecore_wl2_window_buffer_attach(Ecore_Wl2_Window *win, void *buffer, int x, int y, Eina_Bool implicit)
{
   EINA_SAFETY_ON_NULL_RETURN(win);
   EINA_SAFETY_ON_NULL_RETURN(win->surface);

   /* FIXME: Haven't given any thought to x and y since we always use 0... */
   if (!implicit) wl_surface_attach(win->surface, buffer, x, y);
   win->buffer = buffer;
   if (!implicit && !buffer)
     win->has_buffer = EINA_FALSE;
   else
     win->has_buffer = EINA_TRUE;
}

EAPI Eina_Bool
ecore_wl2_window_resizing_get(Ecore_Wl2_Window *window)
{
   EINA_SAFETY_ON_NULL_RETURN_VAL(window, EINA_FALSE);

   return window->req_config.resizing;
}

EAPI void
ecore_wl2_window_update_begin(Ecore_Wl2_Window *window)
{
   EINA_SAFETY_ON_NULL_RETURN(window);
   EINA_SAFETY_ON_TRUE_RETURN(window->updating);

   window->updating = EINA_TRUE;
}

EAPI void
ecore_wl2_window_damage(Ecore_Wl2_Window *window, Eina_Rectangle *rects, unsigned int count)
{
   void (*damage)(struct wl_surface *, int32_t, int32_t, int32_t, int32_t);
   unsigned int k;
   int compositor_version;

   EINA_SAFETY_ON_NULL_RETURN(window);

   compositor_version = window->display->wl.compositor_version;

   if (compositor_version >= WL_SURFACE_DAMAGE_BUFFER_SINCE_VERSION)
     damage = wl_surface_damage_buffer;
   else
     damage = wl_surface_damage;

   if ((rects) && (count > 0))
     for (k = 0; k < count; k++)
       damage(window->surface, rects[k].x, rects[k].y, rects[k].w, rects[k].h);
   else
     damage(window->surface, 0, 0, INT_MAX, INT_MAX);
}

EAPI void
ecore_wl2_window_surface_flush(Ecore_Wl2_Window *window, Eina_Bool purge)
{
   EINA_SAFETY_ON_NULL_RETURN(window);

   if (!window->wl2_surface) return;
   ecore_wl2_surface_flush(window->wl2_surface, purge);
}

// TIZEN_ONLY(20171107): add ecore_wl2_window_input_get() EAPI
EAPI Ecore_Wl2_Input *
ecore_wl2_window_input_get(Ecore_Wl2_Window *win)
{
   Ecore_Wl2_Input *input;

   EINA_SAFETY_ON_NULL_RETURN_VAL(win, NULL);
   EINA_SAFETY_ON_NULL_RETURN_VAL(win->display, NULL);

   EINA_INLIST_FOREACH(win->display->inputs, input)
     {
        if (input->focus.pointer) return input;
        //This code will be changed after adding ecore_wl2_window_keyboard_get API
        if (input->focus.keyboard) return input;
     }

   return NULL;
}
//

// TIZEN_ONLY(20171114): support a pointer warp
EAPI Eina_Bool
ecore_wl2_window_pointer_warp(Ecore_Wl2_Window *win, int x, int y)
{
   Ecore_Wl2_Display *ewd;

   /* FIXME: visible is not merged yet. */
   //if (!win || !win->surface || !win->visible) return EINA_FALSE;
   if (!win || !win->surface) return EINA_FALSE;

   ewd = win->display;
   if (!ewd || !ewd->wl.tz_input_device_manager) return EINA_FALSE;

   tizen_input_device_manager_pointer_warp(ewd->wl.tz_input_device_manager,
                                           win->surface, wl_fixed_from_int(x), wl_fixed_from_int(y));

   return EINA_TRUE;
}
//


//TIZEN_ONLY: ecore_wl2: add ecore_wl_window_video_has
EAPI void
ecore_wl2_window_video_has(Ecore_Wl2_Window *window, Eina_Bool has)
{
   Ecore_Wl2_Display *display;
   uint32_t ver;

   EINA_SAFETY_ON_NULL_RETURN(window);

   display = window->display;
   if (!display->wl.tz_policy) return;

   ver = wl_proxy_get_version((struct wl_proxy *)display->wl.tz_policy);

   if (ver >= 7)
     tizen_policy_has_video(display->wl.tz_policy, window->surface, has);
}
//

//TIZEN_ONLY(20171115): support output transform
void
_ecore_wl2_window_ignore_output_transform_set(Ecore_Wl2_Window *window, Eina_Bool ignore)
{
   if (!window) return;

   window->ignore_output_transform = ignore;
}

EAPI Eina_Bool
ecore_wl2_window_ignore_output_transform_get(Ecore_Wl2_Window *window)
{
   return window->ignore_output_transform;
}
//

// TIZEN_ONLY(20171207): add functions to set client's custom cursors
EAPI void
ecore_wl2_window_pointer_set(Ecore_Wl2_Window *win, struct wl_surface *surface, int hot_x, int hot_y)
{
   Ecore_Wl2_Input *input;

   if (!win) return;

   win->pointer.surface = surface;
   win->pointer.hot_x = hot_x;
   win->pointer.hot_y = hot_y;
   win->pointer.set = EINA_TRUE;

   if ((input = win->pointer.device))
     ecore_wl2_input_pointer_set(input, surface, hot_x, hot_y);
}

EAPI void
ecore_wl2_window_cursor_from_name_set(Ecore_Wl2_Window *win, const char *cursor_name)
{
   Ecore_Wl2_Input *input;

   if (!win) return;

   win->pointer.set = EINA_FALSE;

   if (!(input = win->pointer.device))
     return;

   eina_stringshare_replace(&win->pointer.cursor_name, cursor_name);

   if ((input->cursor.name) && (strcmp(input->cursor.name, win->pointer.cursor_name)))
     ecore_wl2_input_cursor_from_name_set(input, cursor_name);
}

EAPI void
ecore_wl2_window_cursor_default_restore(Ecore_Wl2_Window *win)
{
   Ecore_Wl2_Input *input;

   if (!win) return;

   win->pointer.set = EINA_FALSE;

   if ((input = win->pointer.device))
     ecore_wl2_input_cursor_default_restore(input);
}
//
