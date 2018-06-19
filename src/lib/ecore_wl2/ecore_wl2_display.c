#ifdef HAVE_CONFIG_H
# include <config.h>
#endif

#include "ecore_wl2_private.h"

#include "linux-dmabuf-unstable-v1-client-protocol.h"
#include "efl-hints-client-protocol.h"

static Eina_Hash *_server_displays = NULL;
static Eina_Hash *_client_displays = NULL;

static Eina_Bool _cb_connect_idle(void *data);
static Eina_Bool _cb_connect_data(void *data, Ecore_Fd_Handler *hdl);
static Eina_Bool _ecore_wl2_display_connect(Ecore_Wl2_Display *ewd, Eina_Bool sync);
// TIZEN_ONLY(20171129): thread-safety for wl
static void      _ecore_wl_cb_awake(void *data);
// End of TIZEN_ONLY(20171129)
static void _ecore_wl2_display_sync_add(Ecore_Wl2_Display *ewd);

void
_display_event_free(void *d, void *event)
{
   ecore_wl2_display_disconnect(d);
   free(event);
}

static void
_ecore_wl2_display_event(Ecore_Wl2_Display *ewd, int event)
{
   Ecore_Wl2_Event_Connect *ev;

   ev = calloc(1, sizeof(Ecore_Wl2_Event_Connect));
   EINA_SAFETY_ON_NULL_RETURN(ev);
   ev->display = ewd;
   ewd->refs++;
   ecore_event_add(event, ev, _display_event_free, ewd);
}

static void
_ecore_wl2_display_signal_exit(void)
{
   Ecore_Event_Signal_Exit *ev;

   ev = calloc(1, sizeof(Ecore_Event_Signal_Exit));
   if (!ev) return;

   ev->quit = EINA_TRUE;
   ecore_event_add(ECORE_EVENT_SIGNAL_EXIT, ev, NULL, NULL);
}

static void
_dmabuf_cb_format(void *data EINA_UNUSED, struct zwp_linux_dmabuf_v1 *dmabuf EINA_UNUSED, uint32_t format EINA_UNUSED)
{
   /* It would be awfully nice if this actually happened */
};

static const struct zwp_linux_dmabuf_v1_listener _dmabuf_listener =
{
   _dmabuf_cb_format
};

static void
_xdg_shell_cb_ping(void *data, struct xdg_wm_base *shell, uint32_t serial)
{
   xdg_wm_base_pong(shell, serial);
   ecore_wl2_display_flush(data);
}

static const struct xdg_wm_base_listener _xdg_shell_listener =
{
   _xdg_shell_cb_ping,
};

static void
_zxdg_shell_cb_ping(void *data, struct zxdg_shell_v6 *shell, uint32_t serial)
{
   zxdg_shell_v6_pong(shell, serial);
   ecore_wl2_display_flush(data);
}

static const struct zxdg_shell_v6_listener _zxdg_shell_listener =
{
   _zxdg_shell_cb_ping,
};

static void
_session_recovery_create_uuid(void *data EINA_UNUSED, struct zwp_e_session_recovery *session_recovery EINA_UNUSED, struct wl_surface *surface, const char *uuid)
{
   Ecore_Wl2_Window *win;

   /* surface may have been destroyed */
   if (!surface) return;
   win = wl_surface_get_user_data(surface);

   eina_stringshare_replace(&win->uuid, uuid);
}

static const struct zwp_e_session_recovery_listener _session_listener =
{
   _session_recovery_create_uuid,
};

static void
_aux_hints_supported_aux_hints(void *data, struct efl_aux_hints *aux_hints EINA_UNUSED, struct wl_surface *surface_resource, struct wl_array *hints, uint32_t num_hints)
{
   Ecore_Wl2_Display *ewd = data;
   struct wl_surface *surface = surface_resource;
   Ecore_Wl2_Window *win = NULL;
   char *p = NULL;
   char **str = NULL;
   const char *hint = NULL;
   unsigned int i = 0;
   Ecore_Wl2_Event_Aux_Hint_Supported *ev;

   if (!surface) return;
   win = _ecore_wl2_display_window_surface_find(ewd, surface_resource);
   if (!win) return;

   p = hints->data;
   str = calloc(num_hints, sizeof(char *));
   if (!str) return;

   while ((const char *)p < ((const char *)hints->data + hints->size))
     {
        str[i] = (char *)eina_stringshare_add(p);
        p += strlen(p) + 1;
        i++;
     }
   for (i = 0; i < num_hints; i++)
     {
        hint = eina_stringshare_add(str[i]);
        win->supported_aux_hints =
               eina_list_append(win->supported_aux_hints, hint);
     }
   if (str)
     {
        for (i = 0; i < num_hints; i++)
          {
             if (str[i])
               {
                  eina_stringshare_del(str[i]);
                  str[i] = NULL;
               }
          }
        free(str);
     }

   if (!(ev = calloc(1, sizeof(Ecore_Wl2_Event_Aux_Hint_Supported)))) return;
   ev->win = win->id;
   ev->display = ewd;
   ewd->refs++;
   ecore_event_add(ECORE_WL2_EVENT_AUX_HINT_SUPPORTED, ev, _display_event_free, ewd);
}

static void
_aux_hints_allowed_aux_hint(void *data, struct efl_aux_hints *aux_hints  EINA_UNUSED, struct wl_surface *surface_resource, int id)
{
   struct wl_surface *surface = surface_resource;
   Ecore_Wl2_Window *win = NULL;
   Ecore_Wl2_Display *ewd = data;
   Ecore_Wl2_Event_Aux_Hint_Allowed *ev;

   if (!surface) return;
   win = _ecore_wl2_display_window_surface_find(ewd, surface_resource);
   if (!win) return;

   if (!(ev = calloc(1, sizeof(Ecore_Wl2_Event_Aux_Hint_Allowed)))) return;
   ev->win = win->id;
   ev->id = id;
   ev->display = ewd;
   ewd->refs++;
   ecore_event_add(ECORE_WL2_EVENT_AUX_HINT_ALLOWED, ev, _display_event_free, ewd);
}

 static void
_cb_aux_message_free(void *data EINA_UNUSED, void *event)
{
   Ecore_Wl2_Event_Aux_Message *ev;
   char *str;

   ev = event;
   ecore_wl2_display_disconnect(ev->display);
   eina_stringshare_del(ev->key);
   eina_stringshare_del(ev->val);
   EINA_LIST_FREE(ev->options, str)
      eina_stringshare_del(str);
   free(ev);
}

 static void
_aux_hints_aux_message(void *data, struct efl_aux_hints *aux_hints EINA_UNUSED, struct wl_surface *surface_resource, const char *key, const char *val, struct wl_array *options)
{
   Ecore_Wl2_Window *win = NULL;
   Ecore_Wl2_Event_Aux_Message *ev;
   char *p = NULL, *str = NULL;
   Eina_List *opt_list = NULL;
   Ecore_Wl2_Display *ewd = data;

   if (!surface_resource) return;
   win = _ecore_wl2_display_window_surface_find(ewd, surface_resource);
   if (!win) return;

   if (!(ev = calloc(1, sizeof(Ecore_Wl2_Event_Aux_Message)))) return;

   if ((options) && (options->size))
     {
        p = options->data;
        while ((const char *)p < ((const char *)options->data + options->size))
          {
             str = (char *)eina_stringshare_add(p);
             opt_list = eina_list_append(opt_list, str);
             p += strlen(p) + 1;
          }
     }

   ev->win = win->id;
   ev->key = eina_stringshare_add(key);
   ev->val = eina_stringshare_add(val);
   ev->options = opt_list;
   ev->display = ewd;
   ewd->refs++;

   ecore_event_add(ECORE_WL2_EVENT_AUX_MESSAGE, ev, _cb_aux_message_free, NULL);
}

static const struct efl_aux_hints_listener _aux_hints_listener =
{
   _aux_hints_supported_aux_hints,
   _aux_hints_allowed_aux_hint,
   _aux_hints_aux_message,
};

// TIZEN_ONLY : To use tizen protocols
static void
_tizen_policy_conformant_area_send(Ecore_Wl2_Window *win, uint32_t conformant_part, uint32_t state)
{
   Ecore_Wl2_Event_Conformant_Change *ev;

   if (!(ev = calloc(1, sizeof(Ecore_Wl2_Event_Conformant_Change)))) return;
   ev->win = win->id;
   ev->part_type = conformant_part;
   ev->state = state;
   ecore_event_add(ECORE_WL2_EVENT_CONFORMANT_CHANGE, ev, NULL, NULL);
}

static void
_tizen_policy_cb_conformant(void *data, struct tizen_policy *tizen_policy EINA_UNUSED, struct wl_surface *surface_resource, uint32_t is_conformant)
{
   Ecore_Wl2_Display *ewd = data;
   Ecore_Wl2_Window *win = NULL;

   if (!surface_resource) return;
   win = ecore_wl2_display_window_find_by_surface(ewd, surface_resource);
   if (win)
     win->conformant = is_conformant;
}

static void
_tizen_policy_cb_conformant_area(void *data, struct tizen_policy *tizen_policy EINA_UNUSED, struct wl_surface *surface_resource, uint32_t conformant_part, uint32_t state, int32_t x, int32_t y, int32_t w, int32_t h)
{
   Ecore_Wl2_Display *ewd = data;
   Ecore_Wl2_Window *win = NULL;
   int org_x, org_y, org_w, org_h;
   Eina_Bool changed = EINA_FALSE;
   Ecore_Wl2_Indicator_State ind_state;
   Ecore_Wl2_Virtual_Keyboard_State kbd_state;
   Ecore_Wl2_Clipboard_State clip_state;

   if (!surface_resource) return;
   win = ecore_wl2_display_window_find_by_surface(ewd, surface_resource);
   if (!win) return;

   if (conformant_part == TIZEN_POLICY_CONFORMANT_PART_INDICATOR)
     {
        ecore_wl2_window_indicator_geometry_get(win, &org_x, &org_y, &org_w, &org_h);
        if ((org_x != x) || (org_y != y) || (org_w != w) || (org_h != h))
          {
             ecore_wl2_window_indicator_geometry_set(win, x, y, w, h);
             changed = EINA_TRUE;
          }

        /* The given state is based on the visibility value of indicator.
         * Thus we need to add 1 to it before comparing with indicator state.
         */
        ind_state =  ecore_wl2_window_indicator_state_get(win);
        if ((state + 1) != ind_state)
          {
             ecore_wl2_window_indicator_state_set(win, state + 1);
             changed = EINA_TRUE;
          }
     }
   else if (conformant_part == TIZEN_POLICY_CONFORMANT_PART_KEYBOARD)
     {
        ecore_wl2_window_keyboard_geometry_get(win, &org_x, &org_y, &org_w, &org_h);
        if ((org_x != x) || (org_y != y) || (org_w != w) || (org_h != h))
          {
             ecore_wl2_window_keyboard_geometry_set(win, x, y, w, h);
             changed = EINA_TRUE;
          }

        /* The given state is based on the visibility value of virtual keyboard window.
         * Thus we need to add 1 to it before comparing with keyboard state.
         */
        kbd_state = ecore_wl2_window_keyboard_state_get(win);
        if ((state + 1) != (kbd_state))
          {
             ecore_wl2_window_keyboard_state_set(win, state + 1);
             changed = EINA_TRUE;
          }
     }
   else if (conformant_part == TIZEN_POLICY_CONFORMANT_PART_CLIPBOARD)
     {
        ecore_wl2_window_clipboard_geometry_get(win, &org_x, &org_y, &org_w, &org_h);
        if ((org_x != x) || (org_y != y) || (org_w != w) || (org_h != h))
          {
             ecore_wl2_window_clipboard_geometry_set(win, x, y, w, h);
             changed = EINA_TRUE;
          }

        /* The given state is based on the visibility value of clipboard window.
         * Thus we need to add 1 to it before comparing with clipboard state.
         */
        clip_state = ecore_wl2_window_clipboard_state_get(win);
        if ((state + 1) != clip_state)
          {
             ecore_wl2_window_clipboard_state_set(win, state + 1);
             changed = EINA_TRUE;
          }
     }

   if (changed)
     _tizen_policy_conformant_area_send(win, conformant_part, state);
}

static void
_tizen_policy_cb_notification_done(void *data EINA_UNUSED, struct tizen_policy *tizen_policy EINA_UNUSED, struct wl_surface *surface EINA_UNUSED, int32_t level EINA_UNUSED, uint32_t state EINA_UNUSED)
{
}

static void
_tizen_policy_cb_transient_for_done(void *data EINA_UNUSED, struct tizen_policy *tizen_policy EINA_UNUSED, uint32_t child_id EINA_UNUSED)
{
}

static void
_tizen_policy_cb_window_screen_mode_done(void *data EINA_UNUSED, struct tizen_policy *tizen_policy EINA_UNUSED, struct wl_surface *surface EINA_UNUSED, uint32_t mode EINA_UNUSED, uint32_t state EINA_UNUSED)
{
}

static void
_tizen_policy_cb_iconify_state_changed(void *data, struct tizen_policy *tizen_policy EINA_UNUSED, struct wl_surface *surface_resource, uint32_t iconified, uint32_t force)
{
   Ecore_Wl2_Display *ewd = data;
   Ecore_Wl2_Window *win = NULL;
   Ecore_Wl2_Event_Window_Iconify_State_Change *ev = NULL;

   if (!surface_resource) return;

   win = ecore_wl2_display_window_find_by_surface(ewd, surface_resource);
   if (!win) return;

   if (!(ev = calloc(1, sizeof(Ecore_Wl2_Event_Window_Iconify_State_Change)))) return;
   ev->win = win->id;
   ev->iconified = iconified;
   ev->force = force;

   ecore_event_add(ECORE_WL2_EVENT_WINDOW_ICONIFY_STATE_CHANGE, ev, NULL, NULL);
}

void _window_aux_hint_free(Ecore_Wl2_Window *win)
{
   const char *supported;

   EINA_LIST_FREE(win->supported_aux_hints, supported)
     if (supported) eina_stringshare_del(supported);
}

static void
_tizen_policy_cb_supported_aux_hints(void *data, struct tizen_policy *tizen_policy EINA_UNUSED, struct wl_surface *surface_resource, struct wl_array *hints, uint32_t num_hints)
{
   /* 20171112 : this can be replaced with efl_aux_hints procotol */

   Ecore_Wl2_Display *ewd = data;
   Ecore_Wl2_Window *win = NULL;
   char *p = NULL;
   char **str = NULL;
   const char *hint = NULL;
   unsigned int i = 0;
   Ecore_Wl2_Event_Aux_Hint_Supported *ev;

   if (!surface_resource) return;

   win = ecore_wl2_display_window_find_by_surface(ewd, surface_resource);
   if (!win) return;

   p = hints->data;
   str = calloc(num_hints, sizeof(char *));
   if (!str) return;

   _window_aux_hint_free(win);

   while ((const char *)p < ((const char *)hints->data + hints->size))
     {
        str[i] = (char *)eina_stringshare_add(p);
        p += strlen(p) + 1;
        i++;
     }
   for (i = 0; i < num_hints; i++)
     {
        hint = eina_stringshare_add(str[i]);
        win->supported_aux_hints =
           eina_list_append(win->supported_aux_hints, hint);
     }
   if (str)
     {
        for (i = 0; i < num_hints; i++)
          {
             if (str[i])
               {
                  eina_stringshare_del(str[i]);
                  str[i] = NULL;
               }
          }
        free(str);
     }
}

static void
_tizen_policy_cb_allowed_aux_hint(void *data, struct tizen_policy *tizen_policy  EINA_UNUSED, struct wl_surface *surface_resource, int id)
{
   /* 20171112 : this can be replaced with efl_aux_hints procotol */

   struct wl_surface *surface = surface_resource;
   Ecore_Wl2_Window *win = NULL;
   Ecore_Wl2_Display *ewd = data;
   Ecore_Wl2_Event_Aux_Hint_Allowed *ev;

   if (!surface) return;
   win = _ecore_wl2_display_window_surface_find(ewd, surface_resource);
   if (!win) return;

   if (!(ev = calloc(1, sizeof(Ecore_Wl2_Event_Aux_Hint_Allowed)))) return;
   ev->win = win->id;
   ev->id = id;
   ev->display = ewd;
   ewd->refs++;
   ecore_event_add(ECORE_WL2_EVENT_AUX_HINT_ALLOWED, ev, _display_event_free, ewd);
}

static void
_tizen_policy_cb_aux_message(void *data EINA_UNUSED, struct tizen_policy *tizen_policy EINA_UNUSED, struct wl_surface *surface_resource, const char *key, const char *val, struct wl_array *options)
{
   /* 20171112 : this can be replaced with efl_aux_hints procotol */

   Ecore_Wl2_Window *win = NULL;
   Ecore_Wl2_Event_Aux_Message *ev;
   char *p = NULL, *str = NULL;
   Eina_List *opt_list = NULL;
   Ecore_Wl2_Display *ewd = data;

   if (!surface_resource) return;
   win = _ecore_wl2_display_window_surface_find(ewd, surface_resource);
   if (!win) return;

   if (!(ev = calloc(1, sizeof(Ecore_Wl2_Event_Aux_Message)))) return;

   if ((options) && (options->size))
     {
        p = options->data;
        while ((const char *)p < ((const char *)options->data + options->size))
          {
             str = (char *)eina_stringshare_add(p);
             opt_list = eina_list_append(opt_list, str);
             p += strlen(p) + 1;
          }
     }

   ev->win = win->id;
   ev->key = eina_stringshare_add(key);
   ev->val = eina_stringshare_add(val);
   ev->options = opt_list;
   ev->display = ewd;
   ewd->refs++;

   ecore_event_add(ECORE_WL2_EVENT_AUX_MESSAGE, ev, _cb_aux_message_free, NULL);
}

static void
_tizen_policy_cb_conformant_region(void *data EINA_UNUSED, struct tizen_policy *tizen_policy EINA_UNUSED, struct wl_surface *surface EINA_UNUSED, uint32_t conformant_part EINA_UNUSED, uint32_t state EINA_UNUSED, int32_t x EINA_UNUSED, int32_t y EINA_UNUSED, int32_t w EINA_UNUSED, int32_t h EINA_UNUSED, uint32_t serial EINA_UNUSED)
{
}

static const struct tizen_policy_listener _tizen_policy_listener =
{
   _tizen_policy_cb_conformant,
   _tizen_policy_cb_conformant_area,
   _tizen_policy_cb_notification_done,
   _tizen_policy_cb_transient_for_done,
   _tizen_policy_cb_window_screen_mode_done,
   _tizen_policy_cb_iconify_state_changed,
   _tizen_policy_cb_supported_aux_hints,
   _tizen_policy_cb_allowed_aux_hint,
   _tizen_policy_cb_aux_message,
   _tizen_policy_cb_conformant_region,
};

static void
_tizen_policy_ext_cb_active_angle(void *data, struct tizen_policy_ext *tizen_policy_ext EINA_UNUSED, uint32_t angle)
{
   Ecore_Wl2_Display *ewd = data;
   ewd->active_angle = angle;
}

static const struct tizen_policy_ext_listener _tizen_policy_ext_listener =
{
   _tizen_policy_ext_cb_active_angle,
};

static void
_tizen_effect_cb_start(void *data EINA_UNUSED, struct tizen_effect *tizen_effect EINA_UNUSED, struct wl_surface *surface_resource, unsigned int type)
{
   struct wl_surface *surface = surface_resource;
   Ecore_Wl2_Window *win = NULL;
   Ecore_Wl2_Event_Effect_Start *ev;

   if (!surface) return;
   win = ecore_wl2_window_surface_find(surface);
   if (!win) return;

   if (!(ev = calloc(1, sizeof(Ecore_Wl2_Event_Effect_Start)))) return;
   ev->win = win->id;
   ev->type = type;
   ecore_event_add(ECORE_WL2_EVENT_EFFECT_START, ev, NULL, NULL);
}

static void
_tizen_effect_cb_end(void *data EINA_UNUSED, struct tizen_effect *tizen_effect EINA_UNUSED, struct wl_surface *surface_resource, unsigned int type)
{
   struct wl_surface *surface = surface_resource;
   Ecore_Wl2_Window *win = NULL;
   Ecore_Wl2_Event_Effect_End *ev;

   if (!surface) return;
   win = ecore_wl2_window_surface_find(surface);
   if (!win) return;

   if (!(ev = calloc(1, sizeof(Ecore_Wl2_Event_Effect_End)))) return;
   ev->win = win->id;
   ev->type = type;
   ecore_event_add(ECORE_WL2_EVENT_EFFECT_END, ev, NULL, NULL);
}

static const struct tizen_effect_listener _tizen_effect_listener =
{
   _tizen_effect_cb_start,
   _tizen_effect_cb_end,
};

static void
_tizen_indicator_cb_flick(void *data, struct tizen_indicator *tizen_indicator EINA_UNUSED, struct wl_surface *surface_resource, int type)
{
   Ecore_Wl2_Window *win = NULL;
   Ecore_Wl2_Display *ewd = data;
   Ecore_Wl2_Event_Indicator_Flick *ev;

   if (!surface_resource) return;
   win = _ecore_wl2_display_window_surface_find(ewd, surface_resource);
   if (!win) return;

   if (!(ev = calloc(1, sizeof(Ecore_Wl2_Event_Indicator_Flick)))) return;
   ev->win = win->id;
   ev->type = type;

   ecore_event_add(ECORE_WL2_EVENT_INDICATOR_FLICK, ev, NULL, NULL);
}

static const struct tizen_indicator_listener _tizen_indicator_listener =
{
   _tizen_indicator_cb_flick,
};

static void
_tizen_clipboard_cb_data_selected(void *data, struct tizen_clipboard *tizen_clipboard EINA_UNUSED, struct wl_surface *surface)
{
   Ecore_Wl2_Window *win = NULL;
   Ecore_Wl2_Display *ewd = data;
   Ecore_Wl2_Event_Clipboard_Data_Selected *ev;

   if (!surface) return;
   win = _ecore_wl2_display_window_surface_find(ewd, surface);
   if (!win) return;

   if (!(ev = calloc(1, sizeof(Ecore_Wl2_Event_Clipboard_Data_Selected)))) return;
   ev->win = win->id;

   ecore_event_add(ECORE_WL2_EVENT_CLIPBOARD_DATA_SELECTED, ev, NULL, NULL);
}

static void
_tizen_clipboard_cb_allowed_data_only(void *data, struct tizen_clipboard *tizen_clipboard EINA_UNUSED, uint32_t allowed)
{
   Ecore_Wl2_Display *ewd = NULL;
   Ecore_Wl2_Input *input = NULL;

   ewd = data;
   input = ecore_wl2_input_default_input_get(ewd);
   if (!input) return;

   if (allowed)
     input->is_data_only = EINA_TRUE;
   else
     input->is_data_only = EINA_FALSE;
}

static const struct tizen_clipboard_listener _tizen_clipboard_listener =
{
   _tizen_clipboard_cb_data_selected,
   _tizen_clipboard_cb_allowed_data_only,
};
//

//TIZEN_ONLY(20171115): support output transform
static void
_tizen_screen_rotation_cb_ignore_output_transform(void *data EINA_UNUSED, struct tizen_screen_rotation *tizen_screen_rotation EINA_UNUSED, struct wl_surface *surface, uint32_t ignore)
{
   Ecore_Wl2_Window *win = NULL;
   Ecore_Wl2_Event_Ignore_Output_Transform *ev;

   if (!surface) return;
   win = ecore_wl2_window_surface_find(surface);
   if (!win) return;

   _ecore_wl2_window_ignore_output_transform_set(win, ignore);

   if (!(ev = calloc(1, sizeof(Ecore_Wl2_Event_Ignore_Output_Transform)))) return;

   ev->win = win;
   ev->ignore = (ignore) ? EINA_TRUE : EINA_FALSE;
   ecore_event_add(ECORE_WL2_EVENT_IGNORE_OUTPUT_TRANSFORM, ev, NULL, NULL);
}

static const struct tizen_screen_rotation_listener _tizen_screen_rotation_listener =
{
   _tizen_screen_rotation_cb_ignore_output_transform,
};
//

static void
_cb_global_event_free(void *data EINA_UNUSED, void *event)
{
   Ecore_Wl2_Event_Global *ev;

   ev = event;
   eina_stringshare_del(ev->interface);
   ecore_wl2_display_disconnect(ev->display);
   free(ev);
}

static void
_cb_global_add(void *data, struct wl_registry *registry, unsigned int id, const char *interface, unsigned int version)
{
   Ecore_Wl2_Display *ewd;
   Ecore_Wl2_Event_Global *ev;
   // TIZEN_ONLY
   int client_version = 1;
   //

   ewd = data;

   /* test to see if we have already added this global to our hash */
   if (!eina_hash_find(ewd->globals, &id))
     {
        Ecore_Wl2_Global *global;

        /* allocate space for new global */
        global = calloc(1, sizeof(Ecore_Wl2_Global));
        if (!global) return;

        global->id = id;
        global->interface = eina_stringshare_add(interface);
        global->version = version;

        /* add this global to our hash */
        if (!eina_hash_add(ewd->globals, &global->id, global))
          {
             eina_stringshare_del(global->interface);
             free(global);
          }
     }
   else
     goto event;

   if (!strcmp(interface, "wl_compositor"))
     {
        Ecore_Wl2_Window *window;
        ewd->wl.compositor_version = MIN(version, 4);
        ewd->wl.compositor =
          wl_registry_bind(registry, id, &wl_compositor_interface,
                           ewd->wl.compositor_version);
        EINA_INLIST_FOREACH(ewd->windows, window)
          _ecore_wl2_window_surface_create(window);
     }
   else if (!strcmp(interface, "wl_subcompositor"))
     {
        ewd->wl.subcompositor =
          wl_registry_bind(registry, id, &wl_subcompositor_interface, 1);
     }
   else if (!strcmp(interface, "wl_shm"))
     {
        ewd->wl.shm =
          wl_registry_bind(registry, id, &wl_shm_interface, 1);
     }
   else if (!strcmp(interface, "zwp_linux_dmabuf_v1") && (version >= 2))
     {
        ewd->wl.dmabuf =
          wl_registry_bind(registry, id, &zwp_linux_dmabuf_v1_interface, 2);
        zwp_linux_dmabuf_v1_add_listener(ewd->wl.dmabuf, &_dmabuf_listener, ewd);
        _ecore_wl2_buffer_test(ewd);
        _ecore_wl2_display_sync_add(ewd);
     }
   else if (!strcmp(interface, "wl_data_device_manager"))
     {
        ewd->wl.data_device_manager_version = MIN(version, 3);
        ewd->wl.data_device_manager =
          wl_registry_bind(registry, id, &wl_data_device_manager_interface, ewd->wl.data_device_manager_version);
     }
   else if ((eina_streq(interface, "www")) &&
            (getenv("EFL_WAYLAND_ENABLE_WWW")))
     {
        Ecore_Wl2_Window *window;

        ewd->wl.www = wl_registry_bind(registry, id, &www_interface, 1);
        EINA_INLIST_FOREACH(ewd->windows, window)
          _ecore_wl2_window_www_surface_init(window);
     }
   else if ((!strcmp(interface, "zwp_e_session_recovery")) &&
            (!no_session_recovery))
     {
        ewd->wl.session_recovery =
          wl_registry_bind(registry, id,
                           &zwp_e_session_recovery_interface, 1);
        zwp_e_session_recovery_add_listener(ewd->wl.session_recovery,
                                            &_session_listener, ewd);
     }
   else if (!strcmp(interface, "efl_aux_hints"))
     {
        Ecore_Wl2_Window *window;
        ewd->wl.efl_aux_hints =
          wl_registry_bind(registry, id,
                           &efl_aux_hints_interface, 1);
        efl_aux_hints_add_listener(ewd->wl.efl_aux_hints, &_aux_hints_listener, ewd);
        EINA_INLIST_FOREACH(ewd->windows, window)
          if (window->surface) efl_aux_hints_get_supported_aux_hints(ewd->wl.efl_aux_hints, window->surface);
     }
   else if (!strcmp(interface, "zwp_teamwork"))
     {
        ewd->wl.teamwork =
          wl_registry_bind(registry, id,
                           &zwp_teamwork_interface, EFL_TEAMWORK_VERSION);
     }
   else if (!strcmp(interface, "wl_output"))
     _ecore_wl2_output_add(ewd, id);
   else if (!strcmp(interface, "wl_seat"))
     _ecore_wl2_input_add(ewd, id, version);
   else if (!strcmp(interface, "efl_hints"))
     {
        Ecore_Wl2_Window *window;

        ewd->wl.efl_hints = wl_registry_bind(registry, id, &efl_hints_interface, MIN(version, 2));
        EINA_INLIST_FOREACH(ewd->windows, window)
          {
             if (!window->xdg_surface) continue;
             if (window->aspect.set)
               efl_hints_set_aspect(window->display->wl.efl_hints, window->xdg_surface,
                 window->aspect.w, window->aspect.h, window->aspect.aspect);
             if (window->weight.set)
               efl_hints_set_weight(window->display->wl.efl_hints,
                 window->xdg_surface, window->weight.w, window->weight.h);
          }
     }
   // TIZEN_ONLY : To use tizen protocols
   else if (!strcmp(interface, "tizen_policy"))
     {
        if (version >= 7)
          client_version = 7;
        else
          client_version = version;

        ewd->wl.tz_policy =
           wl_registry_bind(registry, id, &tizen_policy_interface, client_version);
        if (ewd->wl.tz_policy)
          tizen_policy_add_listener(ewd->wl.tz_policy, &_tizen_policy_listener, ewd);
     }
   else if (!strcmp(interface, "tizen_policy_ext"))
     {
        if (version >= 3)
          client_version = 3;
        else
          client_version = version;

        ewd->wl.tz_policy_ext =
           wl_registry_bind(registry, id, &tizen_policy_ext_interface, client_version);
        if (ewd->wl.tz_policy_ext)
          tizen_policy_ext_add_listener(ewd->wl.tz_policy_ext, &_tizen_policy_ext_listener, ewd);
     }
   else if (!strcmp(interface, "tizen_surface"))
     {
        ewd->wl.tz_surf =
           wl_registry_bind(registry, id, &tizen_surface_interface, 1);
     }
   else if (!strcmp(interface, "tizen_effect"))
     {
        ewd->wl.tz_effect =
           wl_registry_bind(registry, id, &tizen_effect_interface, 1);
        if (ewd->wl.tz_effect)
          tizen_effect_add_listener(ewd->wl.tz_effect, &_tizen_effect_listener, ewd->wl.display);
     }
   else if (!strcmp(interface, "tizen_indicator"))
     {
        ewd->wl.tz_indicator =
           wl_registry_bind(registry, id, &tizen_indicator_interface, 1);
        if (ewd->wl.tz_indicator)
          tizen_indicator_add_listener(ewd->wl.tz_indicator, &_tizen_indicator_listener, ewd);
     }
   else if (!strcmp(interface, "tizen_clipboard"))
     {
        if (version >= 2)
          client_version = 2;
        else
          client_version = version;

        ewd->wl.tz_clipboard =
           wl_registry_bind(registry, id, &tizen_clipboard_interface, client_version);

        if (ewd->wl.tz_clipboard)
          tizen_clipboard_add_listener(ewd->wl.tz_clipboard, &_tizen_clipboard_listener, ewd);
     }
// TIZEN_ONLY(20171107): support a tizen_keyrouter interface
   else if (!strcmp(interface, "tizen_keyrouter"))
     {
        _ecore_wl2_keyrouter_setup(ewd, id, version);
     }
//
// TIZEN_ONLY(20171109): support a tizen_input_device_manager interface
   else if (!strcmp(interface, "tizen_input_device_manager"))
     {
        _ecore_wl2_input_device_manager_setup(ewd, id, version);
     }
//
//TIZEN_ONLY(20171115): support output transform
   else if (!strcmp(interface, "tizen_screen_rotation"))
     {
        ewd->wl.tz_screen_rotation =
           wl_registry_bind(registry, id, &tizen_screen_rotation_interface, 1);
        if (ewd->wl.tz_screen_rotation)
          tizen_screen_rotation_add_listener(ewd->wl.tz_screen_rotation, &_tizen_screen_rotation_listener, ewd->wl.display);
     }
//

event:
   /* allocate space for event structure */
   ev = calloc(1, sizeof(Ecore_Wl2_Event_Global));
   if (!ev) return;

   ev->id = id;
   ev->display = ewd;
   ewd->refs++;
   ev->version = version;
   ev->interface = eina_stringshare_add(interface);

   /* raise an event saying a new global has been added */
   ecore_event_add(ECORE_WL2_EVENT_GLOBAL_ADDED, ev,
                   _cb_global_event_free, NULL);
}

static void
_cb_global_remove(void *data, struct wl_registry *registry EINA_UNUSED, unsigned int id)
{
   Ecore_Wl2_Display *ewd;
   Ecore_Wl2_Global *global;
   Ecore_Wl2_Event_Global *ev;

   ewd = data;

   /* try to find this global in our hash */
   global = eina_hash_find(ewd->globals, &id);
   if (!global) return;

   /* allocate space for event structure */
   ev = calloc(1, sizeof(Ecore_Wl2_Event_Global));
   if (!ev) return;

   ev->id = id;
   ev->display = ewd;
   ewd->refs++;
   ev->version = global->version;
   ev->interface = eina_stringshare_add(global->interface);

   /* raise an event saying a global has been removed */
   ecore_event_add(ECORE_WL2_EVENT_GLOBAL_REMOVED, ev,
                   _cb_global_event_free, NULL);

   /* delete this global from our hash */
   if (ewd->globals) eina_hash_del_by_key(ewd->globals, &id);
}

static const struct wl_registry_listener _registry_listener =
{
   _cb_global_add,
   _cb_global_remove
};

static Eina_Bool
_cb_create_data(void *data, Ecore_Fd_Handler *hdl EINA_UNUSED)
{
   Ecore_Wl2_Display *ewd = data;
   struct wl_event_loop *loop;

   loop = wl_display_get_event_loop(ewd->wl.display);
   wl_event_loop_dispatch(loop, 0);

   /* wl_display_flush_clients(ewd->wl.display); */

   return ECORE_CALLBACK_RENEW;
}

static void
_cb_create_prepare(void *data, Ecore_Fd_Handler *hdlr EINA_UNUSED)
{
   Ecore_Wl2_Display *ewd = data;

   wl_display_flush_clients(ewd->wl.display);
}

static Eina_Bool 
_recovery_timer(Ecore_Wl2_Display *ewd)
{
   if (!_ecore_wl2_display_connect(ewd, 1))
     return EINA_TRUE;

   ewd->recovery_timer = NULL;
   return EINA_FALSE;
}

static void
_ecore_wl2_display_globals_cleanup(Ecore_Wl2_Display *ewd)
{
   if (ewd->wl.session_recovery)
     zwp_e_session_recovery_destroy(ewd->wl.session_recovery);
   if (ewd->wl.www) www_destroy(ewd->wl.www);
   if (ewd->wl.xdg_wm_base) xdg_wm_base_destroy(ewd->wl.xdg_wm_base);
   if (ewd->wl.zxdg_shell) zxdg_shell_v6_destroy(ewd->wl.zxdg_shell);
   if (ewd->wl.shm) wl_shm_destroy(ewd->wl.shm);
   if (ewd->wl.data_device_manager)
     wl_data_device_manager_destroy(ewd->wl.data_device_manager);
   if (ewd->wl.compositor) wl_compositor_destroy(ewd->wl.compositor);
   if (ewd->wl.subcompositor) wl_subcompositor_destroy(ewd->wl.subcompositor);
   if (ewd->wl.dmabuf) zwp_linux_dmabuf_v1_destroy(ewd->wl.dmabuf);
   if (ewd->wl.efl_aux_hints) efl_aux_hints_destroy(ewd->wl.efl_aux_hints);
   if (ewd->wl.efl_hints) efl_hints_destroy(ewd->wl.efl_hints);

// TIZEN_ONLY
   if (ewd->wl.tz_policy) tizen_policy_destroy(ewd->wl.tz_policy);
   if (ewd->wl.tz_policy_ext) tizen_policy_ext_destroy(ewd->wl.tz_policy_ext);
   if (ewd->wl.tz_surf) tizen_surface_destroy(ewd->wl.tz_surf);
   if (ewd->wl.tz_effect) tizen_effect_destroy(ewd->wl.tz_effect);
   if (ewd->wl.tz_indicator) tizen_indicator_destroy(ewd->wl.tz_indicator);
   if (ewd->wl.tz_clipboard) tizen_clipboard_destroy(ewd->wl.tz_clipboard);
   if (ewd->wl.tz_screen_rotation) tizen_screen_rotation_destroy(ewd->wl.tz_screen_rotation);
//

   if (ewd->wl.registry) wl_registry_destroy(ewd->wl.registry);
}

static void
_recovery_timer_add(Ecore_Wl2_Display *ewd)
{
   Eina_Inlist *tmp, *tmp2;
   Ecore_Wl2_Output *output;
   Ecore_Wl2_Input *input;
   Ecore_Wl2_Window *window;

   eina_hash_free_buckets(ewd->globals);
   ecore_idle_enterer_del(ewd->idle_enterer);
   ewd->idle_enterer = NULL;

// TIZEN_ONLY(20171129): thread-safety for wl
   ecore_main_awake_handler_del(_ecore_wl_cb_awake);
// End of TIZEN_ONLY(20171129)
   ecore_main_fd_handler_del(ewd->fd_hdl);
   ewd->fd_hdl = NULL;

   ewd->shell_done = EINA_FALSE;
   ewd->sync_done = EINA_FALSE;
   ewd->recovering = EINA_TRUE;

   _ecore_wl2_display_globals_cleanup(ewd);

   memset(&ewd->wl, 0, sizeof(ewd->wl));
   EINA_INLIST_FOREACH_SAFE(ewd->inputs, tmp, input)
     _ecore_wl2_input_del(input);

   EINA_INLIST_FOREACH_SAFE(ewd->outputs, tmp, output)
     _ecore_wl2_output_del(output);

   EINA_INLIST_FOREACH_SAFE(ewd->windows, tmp, window)
     {
        Ecore_Wl2_Subsurface *subsurf;

        EINA_INLIST_FOREACH_SAFE(window->subsurfs, tmp2, subsurf)
          _ecore_wl2_subsurf_unmap(subsurf);
        _ecore_wl2_window_semi_free(window);
        window->set_config.serial = 0;
        window->req_config.serial = 0;
        window->xdg_configure_ack = NULL;
        window->xdg_set_min_size = NULL;
        window->xdg_set_max_size = NULL;
        window->zxdg_configure_ack = NULL;
        window->zxdg_set_min_size = NULL;
        window->zxdg_set_max_size = NULL;
     }

   ewd->recovery_timer =
     ecore_timer_add(0.5, (Ecore_Task_Cb)_recovery_timer, ewd);
   _ecore_wl2_display_event(ewd, ECORE_WL2_EVENT_DISCONNECT);
}

static void
_begin_recovery_maybe(Ecore_Wl2_Display *ewd, int code)
{
   if ((_server_displays || (code != EPROTO)) && ewd->wl.session_recovery)// && (errno == EPIPE))
     _recovery_timer_add(ewd);
   else if (!_server_displays)
     {
        ERR("Wayland Socket Error: %s", strerror(errno));
        _ecore_wl2_display_signal_exit();
     }
}

// TIZEN_ONLY(20171129): thread-safety for wl
static void
_ecore_wl_cb_pre_handle_data(void *data, Ecore_Fd_Handler *hdl EINA_UNUSED)
{
   Ecore_Wl2_Display *ewd = (Ecore_Wl2_Display *)data;

   EINA_SAFETY_ON_NULL_RETURN(ewd);

   if (ewd->prepare_read) return;

   int ret;
   while (wl_display_prepare_read(ewd->wl.display) != 0)
     {
        ret = wl_display_dispatch_pending(ewd->wl.display);
        if (ret < 0)
          {
             ERR("Wayland Display Dispatch Pending Failed");
             break;
          }
     }

   wl_display_flush(ewd->wl.display);

   ewd->prepare_read = EINA_TRUE;
}

static void
_ecore_wl_cb_awake(void *data)
{
   Ecore_Wl2_Display *ewd = (Ecore_Wl2_Display *)data;

   EINA_SAFETY_ON_NULL_RETURN(ewd);
   EINA_SAFETY_ON_NULL_RETURN(ewd->fd_hdl);

   if (!ewd->prepare_read) return;

   ewd->prepare_read = EINA_FALSE;

   if (ecore_main_fd_handler_active_get(ewd->fd_hdl, ECORE_FD_READ))
     wl_display_read_events(ewd->wl.display);
   else
     wl_display_cancel_read(ewd->wl.display);
}
// End of TIZEN_ONLY(20171129)

static Eina_Bool
_cb_connect_data(void *data, Ecore_Fd_Handler *hdl)
{
// TIZEN_ONLY(20171129): thread-safety for wl
   Ecore_Wl2_Display *ewd = (Ecore_Wl2_Display *)data;
   int ret = 0;

   EINA_SAFETY_ON_NULL_RETURN_VAL(ewd, ECORE_CALLBACK_CANCEL);
   EINA_SAFETY_ON_NULL_RETURN_VAL(ewd->fd_hdl, ECORE_CALLBACK_CANCEL);

   if (ecore_main_fd_handler_active_get(hdl, ECORE_FD_ERROR))
     {
        ERR("Received error on wayland display fd");
        goto err;
     }

   if (ecore_main_fd_handler_active_get(hdl, ECORE_FD_READ))
     ret = wl_display_dispatch_pending(ewd->wl.display);
   else if (ecore_main_fd_handler_active_get(hdl, ECORE_FD_WRITE))
     {
        ret = wl_display_flush(ewd->wl.display);
        if (ret == 0)
          ecore_main_fd_handler_active_set(hdl, ECORE_FD_READ);
     }

   if ((ret < 0) && (errno != EAGAIN) && (errno != EINVAL))
     goto err;

   return ECORE_CALLBACK_RENEW;

err:
   ewd->fd_hdl = NULL;
   _begin_recovery_maybe(ewd, errno);

   return ECORE_CALLBACK_CANCEL;
// End of TIZEN_ONLY(20171129)
}

static void
_cb_globals_hash_del(void *data)
{
   Ecore_Wl2_Global *global;

   global = data;

   eina_stringshare_del(global->interface);

   free(global);
}

static Eina_Bool
_cb_connect_idle(void *data)
{
   Ecore_Wl2_Display *ewd = data;
   int ret = 0, code;

   ret = wl_display_get_error(ewd->wl.display);
   code = errno;
   if (ret < 0) goto err;

   ret = wl_display_dispatch_pending(ewd->wl.display);
   code = errno;
   if (ret < 0) goto err;

   return ECORE_CALLBACK_RENEW;

err:
   if ((ret < 0) && (code != EAGAIN))
     {
        ewd->idle_enterer = NULL;
        _begin_recovery_maybe(ewd, code);

        return ECORE_CALLBACK_CANCEL;
     }

   return ECORE_CALLBACK_RENEW;
}

static Ecore_Wl2_Global *
_ecore_wl2_global_find(Ecore_Wl2_Display *ewd, const char *interface)
{
   Eina_Iterator *itr;
   Ecore_Wl2_Global *global = NULL, *g = NULL;

   itr = eina_hash_iterator_data_new(ewd->globals);
   if (!itr) return NULL;

   EINA_ITERATOR_FOREACH(itr, g)
     {
        if (!strcmp(g->interface, interface))
          {
             global = g;
             break;
          }
     }

   eina_iterator_free(itr);
   return global;
}

static void
_ecore_wl2_shell_bind(Ecore_Wl2_Display *ewd)
{
   Ecore_Wl2_Global *global = NULL;
   const char **itr;
   const char *shells[] =
     {
        "xdg_wm_base",
        "zxdg_shell_v6",
        NULL
     };

   if (ewd->shell_done) return;

   for (itr = shells; *itr != NULL; itr++)
     {
        global = _ecore_wl2_global_find(ewd, *itr);
        if (!global) continue;
        break;
     }

   if (!global) return;

   if (!strcmp(global->interface, "xdg_wm_base"))
     {
        ewd->wl.xdg_wm_base =
          wl_registry_bind(ewd->wl.registry, global->id,
                           &xdg_wm_base_interface, 1);
        xdg_wm_base_add_listener(ewd->wl.xdg_wm_base,
                                   &_xdg_shell_listener, ewd);
        ewd->shell_done = EINA_TRUE;
     }
   else if (!strcmp(global->interface, "zxdg_shell_v6"))
     {
        ewd->wl.zxdg_shell =
          wl_registry_bind(ewd->wl.registry, global->id,
                           &zxdg_shell_v6_interface, 1);
        zxdg_shell_v6_add_listener(ewd->wl.zxdg_shell,
                                   &_zxdg_shell_listener, ewd);
        ewd->shell_done = EINA_TRUE;
     }
}

static void
_cb_sync_done(void *data, struct wl_callback *cb, uint32_t serial EINA_UNUSED)
{
   Ecore_Wl2_Event_Sync_Done *ev;
   Ecore_Wl2_Display *ewd;

   ewd = data;
   if (--ewd->syncs) return;
   if (ewd->sync_done) return;

   ewd->sync_done = EINA_TRUE;

   _ecore_wl2_shell_bind(ewd);

   wl_callback_destroy(cb);
   ecore_wl2_display_flush(ewd);

   ev = calloc(1, sizeof(Ecore_Wl2_Event_Sync_Done));
   if (!ev) return;

   ev->display = ewd;
   ewd->refs++;
   ecore_event_add(ECORE_WL2_EVENT_SYNC_DONE, ev, _display_event_free, ewd);
}

static const struct wl_callback_listener _sync_listener =
{
   _cb_sync_done
};

// TIZEN_ONLY(20171110): wait until sync done is called in ecore_wl2_display_sync
static void
_cb_tz_sync_done(void *data, struct wl_callback *cb, uint32_t serial EINA_UNUSED)
{
   Ecore_Wl2_Display *ewd;

   ewd = data;

   ewd->sync_ref_count--;

   wl_callback_destroy(cb);
}

static const struct wl_callback_listener _tz_sync_listener =
{
   _cb_tz_sync_done
};

static Eina_Bool
_ecore_wl2_disconnected(int error)
{
   if (error == EPIPE ||
       error == ECONNRESET)
     return EINA_TRUE;

   return EINA_FALSE;
}

static void
_ecore_wl2_signal_exit(void)
{
   Ecore_Event_Signal_Exit *ev;

   if (!(ev = calloc(1, sizeof(Ecore_Event_Signal_Exit))))
     return;

   ev->quit = 1;
   ecore_event_add(ECORE_EVENT_SIGNAL_EXIT, ev, NULL, NULL);
}

static void
_ecore_wl2_display_dispatch_error(Ecore_Wl2_Display *ewd)
{
   uint32_t ecode = 0, id = 0;
   const struct wl_interface *intf = NULL;
   int last_err;
   char buffer[1024];
   char *str;

   /* write out message about protocol error */
   ecode = wl_display_get_protocol_error(ewd->wl.display, &intf, &id);

   if (intf)
     ERR("Wayland Client: Got protocol error %u on interface %s"
         " (object %u)\n", ecode, intf->name, id);

   /* raise exit signal */
   last_err = wl_display_get_error(ewd->wl.display);
   if (_ecore_wl2_disconnected(errno) || _ecore_wl2_disconnected(last_err))
     {
        int _err = errno;
        if (last_err) _err = last_err;
        str = strerror_r(_err, buffer, sizeof(buffer));
        ERR("Disconnected from a wayland compositor : error:(%d) %s(%s)", _err, buffer, str);
        _ecore_wl2_signal_exit();
        return;
     }
   else
     {
        str = strerror_r(errno, buffer, sizeof(buffer));
        ERR("wayland socket error:(%d) %s(%s)", errno, buffer, str);
        abort();
     }
}

EAPI void
ecore_wl2_display_sync(Ecore_Wl2_Display *display)
{
   struct wl_callback *cb;
   int last_dpy_err;
   int ret;

   EINA_SAFETY_ON_NULL_RETURN(display);
   EINA_SAFETY_ON_NULL_RETURN(display->wl.display);

   display->sync_ref_count++;
   cb = wl_display_sync(display->wl.display);
   wl_callback_add_listener(cb, &_tz_sync_listener, display);

   while (display->sync_ref_count > 0)
     {
        last_dpy_err = wl_display_get_error(display->wl.display);
        if (_ecore_wl2_disconnected(last_dpy_err))
          {
             errno = last_dpy_err;
             ERR("Disconnected from a wayland compositor : %s", strerror(errno));
             _ecore_wl2_signal_exit();
             return;
          }
        ret = wl_display_dispatch(display->wl.display);
        if ((ret < 0) && (errno != EAGAIN))
          {
             _ecore_wl2_display_dispatch_error(display);
             break;
          }
     }
}
//

//TIZEN_ONLY(20171108): add a new API to ecore_wl2_sync
EAPI void
ecore_wl2_sync(void)
{
   Ecore_Wl2_Display *ewd;

   ewd = ecore_wl2_connected_display_get(NULL);
   if (!ewd) return;

   ecore_wl2_display_sync(ewd);
}
//

static void
_ecore_wl2_display_sync_add(Ecore_Wl2_Display *ewd)
{
   struct wl_callback *cb;

   ewd->syncs++;
   cb = wl_display_sync(ewd->wl.display);
   wl_callback_add_listener(cb, &_sync_listener, ewd);
}

static Eina_Bool
_ecore_wl2_display_connect(Ecore_Wl2_Display *ewd, Eina_Bool sync)
{
   /* try to connect to wayland display with this name */
   ewd->wl.display = wl_display_connect(ewd->name);
   if (!ewd->wl.display) return EINA_FALSE;

   ewd->recovering = EINA_FALSE;

   ewd->wl.registry = wl_display_get_registry(ewd->wl.display);
   wl_registry_add_listener(ewd->wl.registry, &_registry_listener, ewd);

   _ecore_wl2_display_sync_add(ewd);

   if (sync)
     {
        /* NB: If we are connecting (as a client), then we will need to setup
         * a callback for display_sync and wait for it to complete. There is no
         * other option here as we need the compositor, shell, etc, to be setup
         * before we can allow a user to make use of the API functions */
        while (!ewd->sync_done)
          {
             int ret;

             ret = wl_display_dispatch(ewd->wl.display);
             if ((ret < 0) && (errno != EAGAIN))
               {
                  ERR("Received Fatal Error on Wayland Display");

                  wl_registry_destroy(ewd->wl.registry);
                  return EINA_FALSE;
               }
          }
     }

   ewd->fd_hdl =
     ecore_main_fd_handler_add(wl_display_get_fd(ewd->wl.display),
                               ECORE_FD_READ | ECORE_FD_WRITE | ECORE_FD_ERROR,
                               _cb_connect_data, ewd, NULL, NULL);

   ewd->idle_enterer = ecore_idle_enterer_add(_cb_connect_idle, ewd);
// TIZEN_ONLY(20171129): thread-safety for wl
   ecore_main_fd_handler_prepare_callback_set
     (ewd->fd_hdl, _ecore_wl_cb_pre_handle_data, ewd);

   ecore_main_awake_handler_add(_ecore_wl_cb_awake, ewd);
// End of TIZEN_ONLY(20171129)
   _ecore_wl2_display_event(ewd, ECORE_WL2_EVENT_CONNECT);
   return EINA_TRUE;
}

static void
_ecore_wl2_display_cleanup(Ecore_Wl2_Display *ewd)
{
   Ecore_Wl2_Output *output;
   Ecore_Wl2_Input *input;
   Eina_Inlist *tmp;

   if (ewd->xkb_context) xkb_context_unref(ewd->xkb_context);

   /* free each input */
   EINA_INLIST_FOREACH_SAFE(ewd->inputs, tmp, input)
     _ecore_wl2_input_del(input);

   /* free each output */
   EINA_INLIST_FOREACH_SAFE(ewd->outputs, tmp, output)
     _ecore_wl2_output_del(output);

   if (ewd->idle_enterer) ecore_idle_enterer_del(ewd->idle_enterer);

// TIZEN_ONLY(20171129): thread-safety for wl
   ecore_main_awake_handler_del(_ecore_wl_cb_awake);
// End of TIZEN_ONLY(20171129)

   if (ewd->fd_hdl) ecore_main_fd_handler_del(ewd->fd_hdl);

   eina_hash_free(ewd->globals);

   _ecore_wl2_display_globals_cleanup(ewd);
}

Ecore_Wl2_Window *
_ecore_wl2_display_window_surface_find(Ecore_Wl2_Display *display, struct wl_surface *wl_surface)
{
   Ecore_Wl2_Window *window;

   if ((!display) || (!wl_surface)) return NULL;

   EINA_INLIST_FOREACH(display->windows, window)
     {
        if ((window->surface) &&
            (window->surface == wl_surface))
          return window;
     }

   return NULL;
}

EAPI Ecore_Wl2_Display *
ecore_wl2_display_create(const char *name)
{
   Ecore_Wl2_Display *ewd;
   struct wl_event_loop *loop;

   if (!_server_displays)
     _server_displays = eina_hash_string_superfast_new(NULL);

   if (name)
     {
        /* someone wants to create a server with a specific display */

        /* check hash of cached server displays for this name */
        ewd = eina_hash_find(_server_displays, name);
        if (ewd) goto found;
     }

   /* allocate space for display structure */
   ewd = calloc(1, sizeof(Ecore_Wl2_Display));
   if (!ewd) return NULL;

   ewd->refs++;
   ewd->pid = getpid();

   /* try to create new wayland display */
   ewd->wl.display = wl_display_create();
   if (!ewd->wl.display)
     {
        ERR("Could not create wayland display");
        goto create_err;
     }

   if (!name)
     {
        const char *n;

        n = wl_display_add_socket_auto(ewd->wl.display);
        if (!n)
          {
             ERR("Failed to add display socket");
             goto socket_err;
          }

        ewd->name = strdup(n);
     }
   else
     {
        if (wl_display_add_socket(ewd->wl.display, name))
          {
             ERR("Failed to add display socket");
             goto socket_err;
          }

        ewd->name = strdup(name);
     }

   setenv("WAYLAND_DISPLAY", ewd->name, 1);
   DBG("WAYLAND_DISPLAY: %s", ewd->name);

   loop = wl_display_get_event_loop(ewd->wl.display);

   ewd->fd_hdl =
     ecore_main_fd_handler_add(wl_event_loop_get_fd(loop),
                               ECORE_FD_READ | ECORE_FD_ERROR,
                               _cb_create_data, ewd, NULL, NULL);

   ecore_main_fd_handler_prepare_callback_set(ewd->fd_hdl,
                                              _cb_create_prepare, ewd);

   /* add this new server display to hash */
   eina_hash_add(_server_displays, ewd->name, ewd);

   return ewd;

socket_err:
   wl_display_destroy(ewd->wl.display);

create_err:
   free(ewd);
   return NULL;

found:
   ewd->refs++;
   return ewd;
}

Eina_Bool
_ecore_wl2_display_sync_get(void)
{
   return !_server_displays || !eina_hash_population(_server_displays);
}

// TIZEN_ONLY(20171107): support a tizen_keyrouter interface
EAPI Ecore_Wl2_Display *
ecore_wl2_connected_display_get(const char *name)
{
   Ecore_Wl2_Display * ewd;
   const char *n;

   if (!_client_displays) return NULL;

   if (!name)
     {
        /* client wants to connect to default display */
        n = getenv("WAYLAND_DISPLAY");
        if (!n) n = "wayland-0";

        /* we have a default wayland display */

        /* check hash of cached client displays for this name */
        ewd = eina_hash_find(_client_displays, n);
     }
   else
     {
        /* client wants to connect to specific display */

        /* check hash of cached client displays for this name */
        ewd = eina_hash_find(_client_displays, name);
     }

   return ewd;
}
//

EAPI Ecore_Wl2_Display *
ecore_wl2_display_connect(const char *name)
{
   Ecore_Wl2_Display *ewd;
   const char *n;
   Eina_Bool hash_create = !_client_displays;

   if (!_client_displays)
     _client_displays = eina_hash_string_superfast_new(NULL);

   if (!name)
     {
        /* client wants to connect to default display */
        n = getenv("WAYLAND_DISPLAY");
        if (!n) n = "wayland-0";

        /* we have a default wayland display */

        /* check hash of cached client displays for this name */
        ewd = eina_hash_find(_client_displays, n);
        if (ewd) goto found;
     }
   else
     {
        /* client wants to connect to specific display */

        /* check hash of cached client displays for this name */
        ewd = eina_hash_find(_client_displays, name);
        if (ewd) goto found;
     }

   /* allocate space for display structure */
   ewd = calloc(1, sizeof(Ecore_Wl2_Display));
   if (!ewd) return NULL;

   ewd->refs++;

   if (name)
     ewd->name = strdup(name);
   else if (n)
     ewd->name = strdup(n);

   ewd->globals = eina_hash_int32_new(_cb_globals_hash_del);

   ewd->xkb_context = xkb_context_new(0);
   if (!ewd->xkb_context) goto context_err;

   /* check server display hash and match on pid. If match, skip sync */
   if (!_ecore_wl2_display_connect(ewd, _ecore_wl2_display_sync_get()))
     goto connect_err;

   /* add this new client display to hash */
   eina_hash_add(_client_displays, ewd->name, ewd);

   return ewd;

connect_err:
   xkb_context_unref(ewd->xkb_context);
   ewd->xkb_context = NULL;

context_err:
   eina_hash_free(ewd->globals);
   free(ewd->name);
   free(ewd);

   if (hash_create)
     {
        eina_hash_free(_client_displays);
        _client_displays = NULL;
     }
   return NULL;

found:
   ewd->refs++;
   return ewd;
}

EAPI void
ecore_wl2_display_disconnect(Ecore_Wl2_Display *display)
{
   EINA_SAFETY_ON_NULL_RETURN(display);

   --display->refs;
   if (display->refs == 0)
     {
        int ret;

        do
          {
             ret = wl_display_dispatch_pending(display->wl.display);
          } while (ret > 0);

        _ecore_wl2_display_cleanup(display);

        wl_display_disconnect(display->wl.display);

        /* remove this client display from hash */
        eina_hash_del_by_key(_client_displays, display->name);

        free(display->name);
        free(display);
     }
}

EAPI void
ecore_wl2_display_destroy(Ecore_Wl2_Display *display)
{
   EINA_SAFETY_ON_NULL_RETURN(display);

   --display->refs;
   if (display->refs == 0)
     {
        /* this ensures that things like wl_registry are destroyed
         * before we destroy the actual wl_display */
        _ecore_wl2_display_cleanup(display);

        wl_display_destroy(display->wl.display);

        /* remove this client display from hash */
        eina_hash_del_by_key(_server_displays, display->name);
        ecore_timer_del(display->recovery_timer);

        free(display->name);
        free(display);
     }
}

EAPI void
ecore_wl2_display_terminate(Ecore_Wl2_Display *display)
{
   EINA_SAFETY_ON_NULL_RETURN(display);
   wl_display_terminate(display->wl.display);
}

EAPI struct wl_display *
ecore_wl2_display_get(Ecore_Wl2_Display *display)
{
   EINA_SAFETY_ON_NULL_RETURN_VAL(display, NULL);
   return display->wl.display;
}

EAPI struct wl_shm *
ecore_wl2_display_shm_get(Ecore_Wl2_Display *display)
{
   EINA_SAFETY_ON_NULL_RETURN_VAL(display, NULL);
   return display->wl.shm;
}

EAPI void *
ecore_wl2_display_dmabuf_get(Ecore_Wl2_Display *display)
{
   EINA_SAFETY_ON_NULL_RETURN_VAL(display, NULL);
   return display->wl.dmabuf;
}

EAPI Eina_Iterator *
ecore_wl2_display_globals_get(Ecore_Wl2_Display *display)
{
   EINA_SAFETY_ON_NULL_RETURN_VAL(display, NULL);
   EINA_SAFETY_ON_NULL_RETURN_VAL(display->globals, NULL);

   return eina_hash_iterator_data_new(display->globals);
}

EAPI void
ecore_wl2_display_screen_size_get(Ecore_Wl2_Display *display, int *w, int *h)
{
   Ecore_Wl2_Output *output;
   int ow = 0, oh = 0;

   EINA_SAFETY_ON_NULL_RETURN(display);

   if (w) *w = 0;
   if (h) *h = 0;

   EINA_INLIST_FOREACH(display->outputs, output)
     {
        switch (output->transform)
          {
           case WL_OUTPUT_TRANSFORM_90:
           case WL_OUTPUT_TRANSFORM_270:
           case WL_OUTPUT_TRANSFORM_FLIPPED_90:
           case WL_OUTPUT_TRANSFORM_FLIPPED_270:
             ow += output->geometry.h;
             oh += output->geometry.w;
             break;
           default:
             ow += output->geometry.w;
             oh += output->geometry.h;
             break;
          }
     }

   if (w) *w = ow;
   if (h) *h = oh;
}

EAPI Ecore_Wl2_Window *
ecore_wl2_display_window_find(Ecore_Wl2_Display *display, unsigned int id)
{
   Ecore_Wl2_Window *window;

   EINA_SAFETY_ON_NULL_RETURN_VAL(display, NULL);

   EINA_INLIST_FOREACH(display->windows, window)
     if (window->id == (int)id) return window;

   return NULL;
}

// TIZEN_ONLY(20171110)
EAPI Ecore_Wl2_Window *
ecore_wl2_display_window_find_by_surface(Ecore_Wl2_Display *display, struct wl_surface *surface)
{
   return _ecore_wl2_display_window_surface_find(display, surface);
}
//

EAPI struct wl_registry *
ecore_wl2_display_registry_get(Ecore_Wl2_Display *display)
{
   EINA_SAFETY_ON_NULL_RETURN_VAL(display, NULL);

   return display->wl.registry;
}

EAPI int
ecore_wl2_display_compositor_version_get(Ecore_Wl2_Display *display)
{
   EINA_SAFETY_ON_NULL_RETURN_VAL(display, 0);

   return display->wl.compositor_version;
}

// TIZEN_ONLY(20180123)
EAPI struct wl_compositor *
ecore_wl2_display_compositor_get(Ecore_Wl2_Display *display)
{
   EINA_SAFETY_ON_NULL_RETURN_VAL(display, 0);

   return display->wl.compositor;
}
//

EAPI Eina_Iterator *
ecore_wl2_display_inputs_get(Ecore_Wl2_Display *display)
{
   EINA_SAFETY_ON_NULL_RETURN_VAL(display, NULL);
   EINA_SAFETY_ON_TRUE_RETURN_VAL(display->pid, NULL);
   return eina_inlist_iterator_new(display->inputs);
}

EAPI Ecore_Wl2_Input *
ecore_wl2_display_input_find(const Ecore_Wl2_Display *display, unsigned int id)
{
   Ecore_Wl2_Input *input;

   EINA_SAFETY_ON_NULL_RETURN_VAL(display, NULL);
   EINA_SAFETY_ON_TRUE_RETURN_VAL(display->pid, NULL);
   EINA_INLIST_FOREACH(display->inputs, input)
     if (input->id == id) return input;
   return NULL;
}

EAPI Ecore_Wl2_Input *
ecore_wl2_display_input_find_by_name(const Ecore_Wl2_Display *display, const char *name)
{
   Ecore_Wl2_Input *input;

   EINA_SAFETY_ON_NULL_RETURN_VAL(display, NULL);
   EINA_SAFETY_ON_TRUE_RETURN_VAL(display->pid, NULL);
   EINA_INLIST_FOREACH(display->inputs, input)
     if (eina_streq(input->name, name)) return input;
   return NULL;
}

EAPI Eina_Bool
ecore_wl2_display_sync_is_done(const Ecore_Wl2_Display *display)
{
   EINA_SAFETY_ON_NULL_RETURN_VAL(display, EINA_FALSE);
   return display->sync_done;
}

EAPI const char *
ecore_wl2_display_name_get(const Ecore_Wl2_Display *display)
{
   EINA_SAFETY_ON_NULL_RETURN_VAL(display, NULL);
   return display->name;
}

EAPI void
ecore_wl2_display_flush(Ecore_Wl2_Display *display)
{
   int ret, code;

   EINA_SAFETY_ON_NULL_RETURN(display);

   ret = wl_display_flush(display->wl.display);
   if (ret >= 0) return;

   code = errno;
   if (code == EAGAIN)
     {
        ecore_main_fd_handler_active_set(display->fd_hdl,
                                         (ECORE_FD_READ | ECORE_FD_WRITE));
        return;
     }

   _begin_recovery_maybe(display, code);
}
