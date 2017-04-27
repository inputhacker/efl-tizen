#ifdef HAVE_CONFIG_H
# include <config.h>
#endif

#include <assert.h>
#include <fcntl.h>
#include "Ecore.h"
#include "ecore_private.h"
#include "ecore_wl_private.h"

/*
 * The subsurface protocol was moved into Wayland Core
 * around v1.3.90 (i.e. v1.4.0).
 * Test if subsurface protocol is part of wayland-client.h.
 * If not, we include our own copy of the protocol header.
 */
#include <wayland-client.h>
#ifndef WL_SUBSURFACE_ERROR_ENUM
# include <subsurface-client-protocol.h>
#endif

#include "xdg-shell-client-protocol.h"
#define XDG_VERSION 5

#include "session-recovery-client-protocol.h"

/* local function prototypes */
static int _ecore_wl_shutdown(Eina_Bool close);
static Eina_Bool _ecore_wl_cb_idle_enterer(void *data);
static Eina_Bool _ecore_wl_cb_handle_data(void *data, Ecore_Fd_Handler *hdl);
static void _ecore_wl_cb_pre_handle_data(void *data, Ecore_Fd_Handler *hdl);
static void _ecore_wl_cb_awake(void *data);
static void _ecore_wl_cb_handle_global(void *data, struct wl_registry *registry, unsigned int id, const char *interface, unsigned int version EINA_UNUSED);
static void _ecore_wl_cb_handle_global_remove(void *data, struct wl_registry *registry EINA_UNUSED, unsigned int id);
static Eina_Bool _ecore_wl_xkb_init(Ecore_Wl_Display *ewd);
static Eina_Bool _ecore_wl_xkb_shutdown(Ecore_Wl_Display *ewd);
static void _ecore_wl_sync_wait(Ecore_Wl_Display *ewd);
static void _ecore_wl_sync_callback(void *data, struct wl_callback *callback, uint32_t serial);
static void _ecore_wl_animator_tick_cb_begin(void *data EINA_UNUSED);
static void _ecore_wl_animator_tick_cb_end(void *data EINA_UNUSED);
static void _ecore_wl_animator_callback(void *data, struct wl_callback *callback, uint32_t serial EINA_UNUSED);
static Eina_Bool _ecore_wl_animator_window_add(const Eina_Hash *hash EINA_UNUSED, const void *key EINA_UNUSED, void *data, void *fdata EINA_UNUSED);
static void _ecore_wl_signal_exit(void);
static void _ecore_wl_signal_exit_free(void *data EINA_UNUSED, void *event);
static void _ecore_wl_init_callback(void *data, struct wl_callback *callback, uint32_t serial EINA_UNUSED);
// TIZEN_ONLY(20150722): Add ecore_wl_window_keygrab_* APIs
static void _ecore_wl_cb_keygrab_notify(void *data, struct tizen_keyrouter *tizen_keyrouter, struct wl_surface *surface, uint32_t key, uint32_t mode, uint32_t error);
static void _ecore_wl_cb_keygrab_notify_list(void *data, struct tizen_keyrouter *tizen_keyrouter, struct wl_surface *surface, struct wl_array *grab_result);
static void _ecore_wl_cb_getgrab_notify_list(void *data EINA_UNUSED, struct tizen_keyrouter *tizen_keyrouter EINA_UNUSED, struct wl_surface *surface EINA_UNUSED, struct wl_array *grab_result EINA_UNUSED);
static void _ecore_wl_cb_set_register_none_key(void *data EINA_UNUSED, struct tizen_keyrouter *tizen_keyrouter EINA_UNUSED, struct wl_surface *surface EINA_UNUSED, uint32_t mode EINA_UNUSED);
static void _ecore_wl_cb_keyregister_notify(void *data EINA_UNUSED, struct tizen_keyrouter *tizen_keyrouter EINA_UNUSED, uint32_t status EINA_UNUSED);
static void _ecore_wl_cb_set_input_config(void *data EINA_UNUSED, struct tizen_keyrouter *tizen_keyrouter EINA_UNUSED, uint32_t status EINA_UNUSED);
static void _ecore_wl_cb_key_cancel(void *data EINA_UNUSED, struct tizen_keyrouter *tizen_keyrouter EINA_UNUSED, uint32_t key);
//
static void _ecore_wl_cb_conformant(void *data EINA_UNUSED, struct tizen_policy *tizen_policy EINA_UNUSED, struct wl_surface *surface_resource, uint32_t is_conformant);
static void _ecore_wl_cb_conformant_area(void *data EINA_UNUSED, struct tizen_policy *tizen_policy EINA_UNUSED, struct wl_surface *surface_resource, uint32_t conformant_part, uint32_t state, int32_t x, int32_t y, int32_t w, int32_t h);
static void _ecore_wl_cb_notification_done(void *data, struct tizen_policy *tizen_policy, struct wl_surface *surface, int32_t level, uint32_t state);
static void _ecore_wl_cb_transient_for_done(void *data, struct tizen_policy *tizen_policy, uint32_t child_id);
static void _ecore_wl_cb_scr_mode_done(void *data, struct tizen_policy *tizen_policy, struct wl_surface *surface, uint32_t mode, uint32_t state);
static void _ecore_wl_cb_iconify_state_changed(void *data EINA_UNUSED, struct tizen_policy *tizen_policy EINA_UNUSED, struct wl_surface *surface_resource, uint32_t iconified, uint32_t force);
static void _ecore_wl_cb_supported_aux_hints(void *data  EINA_UNUSED, struct tizen_policy *tizen_policy  EINA_UNUSED, struct wl_surface *surface_resource, struct wl_array *hints, uint32_t num_hints);
static void _ecore_wl_cb_allowed_aux_hint(void *data  EINA_UNUSED, struct tizen_policy *tizen_policy  EINA_UNUSED, struct wl_surface *surface_resource, int id);
static void _ecore_wl_cb_aux_message(void *data EINA_UNUSED, struct tizen_policy *tizen_policy EINA_UNUSED, struct wl_surface *surface_resource, const char *key, const char *val, struct wl_array *options);
static void _ecore_wl_window_conformant_area_send(Ecore_Wl_Window *win, uint32_t conformant_part, uint32_t state);
static void _ecore_wl_cb_active_angle(void *data EINA_UNUSED, struct tizen_policy_ext *tizen_policy_ext EINA_UNUSED, uint32_t angle);
static void _ecore_wl_cb_effect_start(void *data EINA_UNUSED, struct tizen_effect *tizen_effect EINA_UNUSED, struct wl_surface *surface_resource, unsigned int type);
static void _ecore_wl_cb_effect_end(void *data EINA_UNUSED, struct tizen_effect *tizen_effect EINA_UNUSED, struct wl_surface *surface_resource, unsigned int type);
static void _ecore_wl_cb_indicator_flick(void *data EINA_UNUSED, struct tizen_indicator *tizen_indicator EINA_UNUSED, struct wl_surface *surface_resource, int type);
static void _ecore_wl_cb_clipboard_data_selected(void *data EINA_UNUSED, struct tizen_clipboard *clipboard EINA_UNUSED, struct wl_surface *surface);
static void _ecore_wl_cb_clipboard_data_only_allowed(void *data EINA_UNUSED, struct tizen_clipboard *clipboard EINA_UNUSED, uint32_t allowed);
static void _ecore_wl_log_cb_print(const char *format, va_list args);
/* local variables */
static int _ecore_wl_init_count = 0;
static Eina_Bool _ecore_wl_animator_busy = EINA_FALSE;
static Eina_Bool _ecore_wl_fatal_error = EINA_FALSE;
static Eina_Bool _ecore_wl_server_mode = EINA_FALSE;
// TIZEN_ONLY(20150722): Add ecore_wl_window_keygrab_* APIs
static Eina_Hash *_keygrabs = NULL;
static int _ecore_wl_keygrab_error = -1;
static struct wl_array _ecore_wl_keygrab_result_list;
static int _ecore_wl_active_angle = -1;
//

static const struct wl_registry_listener _ecore_wl_registry_listener =
{
   _ecore_wl_cb_handle_global,
   _ecore_wl_cb_handle_global_remove
};

static const struct wl_callback_listener _ecore_wl_sync_listener =
{
   _ecore_wl_sync_callback
};

static const struct wl_callback_listener _ecore_wl_init_sync_listener =
{
   _ecore_wl_init_callback
};

static const struct wl_callback_listener _ecore_wl_anim_listener =
{
   _ecore_wl_animator_callback
};

// TIZEN_ONLY(20150722): Add ecore_wl_window_keygrab_* APIs
static const struct tizen_keyrouter_listener _ecore_tizen_keyrouter_listener =
{
   _ecore_wl_cb_keygrab_notify,
   _ecore_wl_cb_keygrab_notify_list,
   _ecore_wl_cb_getgrab_notify_list,
   _ecore_wl_cb_set_register_none_key,
   _ecore_wl_cb_keyregister_notify,
   _ecore_wl_cb_set_input_config,
   _ecore_wl_cb_key_cancel
};
//

static const struct tizen_policy_listener _ecore_tizen_policy_listener =
{
   _ecore_wl_cb_conformant,
   _ecore_wl_cb_conformant_area,
   _ecore_wl_cb_notification_done,
   _ecore_wl_cb_transient_for_done,
   _ecore_wl_cb_scr_mode_done,
   _ecore_wl_cb_iconify_state_changed,
   _ecore_wl_cb_supported_aux_hints,
   _ecore_wl_cb_allowed_aux_hint,
   _ecore_wl_cb_aux_message,
};

static const struct tizen_policy_ext_listener _ecore_tizen_policy_ext_listener =
{
   _ecore_wl_cb_active_angle,
};

static const struct tizen_effect_listener _ecore_tizen_effect_listener =
{
   _ecore_wl_cb_effect_start,
   _ecore_wl_cb_effect_end,
};

static const struct tizen_indicator_listener _ecore_tizen_indicator_listener =
{
   _ecore_wl_cb_indicator_flick,
};

static const struct tizen_clipboard_listener _ecore_tizen_clipboard_listener =
{
   _ecore_wl_cb_clipboard_data_selected,
   _ecore_wl_cb_clipboard_data_only_allowed,
};

static void
xdg_shell_ping(void *data EINA_UNUSED, struct xdg_shell *shell, uint32_t serial)
{
   xdg_shell_pong(shell, serial);
}

static const struct xdg_shell_listener xdg_shell_listener =
{
   xdg_shell_ping,
};

/* static void */
/* _ecore_wl_uuid_receive(void *data EINA_UNUSED, struct session_recovery *session_recovery EINA_UNUSED, const char *uuid) */
/* { */
/*    DBG("UUID assigned from compositor: %s", uuid); */
/* } */

/* static const struct session_recovery_listener _ecore_wl_session_recovery_listener = */
/* { */
/*    _ecore_wl_uuid_receive, */
/* }; */

/* external variables */
int _ecore_wl_log_dom = -1;
Ecore_Wl_Display *_ecore_wl_disp = NULL;

EAPI int ECORE_WL_EVENT_MOUSE_IN = 0;
EAPI int ECORE_WL_EVENT_MOUSE_OUT = 0;
EAPI int ECORE_WL_EVENT_FOCUS_IN = 0;
EAPI int ECORE_WL_EVENT_FOCUS_OUT = 0;
EAPI int ECORE_WL_EVENT_WINDOW_CONFIGURE = 0;
EAPI int ECORE_WL_EVENT_WINDOW_ACTIVATE = 0;
EAPI int ECORE_WL_EVENT_WINDOW_DEACTIVATE = 0;
EAPI int ECORE_WL_EVENT_WINDOW_VISIBILITY_CHANGE = 0;
EAPI int ECORE_WL_EVENT_WINDOW_PRE_VISIBILITY_CHANGE = 0;
EAPI int ECORE_WL_EVENT_WINDOW_SHOW = 0;
EAPI int ECORE_WL_EVENT_WINDOW_HIDE = 0;
EAPI int ECORE_WL_EVENT_WINDOW_LOWER = 0;
EAPI int ECORE_WL_EVENT_WINDOW_ROTATE = 0;
EAPI int ECORE_WL_EVENT_DND_ENTER = 0;
EAPI int ECORE_WL_EVENT_DND_POSITION = 0;
EAPI int ECORE_WL_EVENT_DND_LEAVE = 0;
EAPI int ECORE_WL_EVENT_DND_DROP = 0;
EAPI int ECORE_WL_EVENT_DND_OFFER = 0;
EAPI int ECORE_WL_EVENT_DND_END = 0;
EAPI int ECORE_WL_EVENT_DATA_SOURCE_TARGET = 0;
EAPI int ECORE_WL_EVENT_DATA_SOURCE_SEND = 0;
EAPI int ECORE_WL_EVENT_SELECTION_DATA_READY = 0;
EAPI int ECORE_WL_EVENT_DATA_SOURCE_CANCELLED = 0;
EAPI int ECORE_WL_EVENT_INTERFACES_BOUND = 0;
EAPI int ECORE_WL_EVENT_CONFORMANT_CHANGE = 0;
EAPI int ECORE_WL_EVENT_AUX_HINT_ALLOWED = 0;
EAPI int ECORE_WL_EVENT_AUX_MESSAGE = 0;
EAPI int ECORE_WL_EVENT_WINDOW_ICONIFY_STATE_CHANGE = 0;
EAPI int ECORE_WL_EVENT_EFFECT_START = 0;
EAPI int ECORE_WL_EVENT_EFFECT_END = 0;
EAPI int ECORE_WL_EVENT_OUTPUT_TRANSFORM = 0;
EAPI int ECORE_WL_EVENT_GLOBAL_ADDED = 0;
EAPI int ECORE_WL_EVENT_GLOBAL_REMOVED = 0;
EAPI int ECORE_WL_EVENT_KEYMAP_UPDATE = 0;
EAPI int ECORE_WL_EVENT_INDICATOR_FLICK = 0;
EAPI int ECORE_WL_EVENT_CLIPBOARD_DATA_SELECTED = 0;


static void
_ecore_wl_init_callback(void *data, struct wl_callback *callback, uint32_t serial EINA_UNUSED)
{
   Ecore_Wl_Display *ewd = data;

   wl_callback_destroy(callback);
   ewd->init_done = EINA_TRUE;
}

static void
_ecore_wl_init_wait(void)
{
   int ret;

   while (!_ecore_wl_disp->init_done)
     {
        ret = wl_display_dispatch(_ecore_wl_disp->wl.display);
        if ((ret < 0) && ((errno != EAGAIN) && (errno != EINVAL)))
          {
             /* raise exit signal */
             ERR("Wayland socket error: %s", strerror(errno));
             abort();
             break;
          }
     }
}

EAPI int
ecore_wl_init(const char *name)
{
   struct wl_callback *callback;
   LOGFN(__FILE__, __LINE__, __FUNCTION__);

   if (++_ecore_wl_init_count != 1) return _ecore_wl_init_count;

   if (!eina_init()) return --_ecore_wl_init_count;

   _ecore_wl_log_dom =
     eina_log_domain_register("ecore_wl", ECORE_WL_DEFAULT_LOG_COLOR);
   if (_ecore_wl_log_dom < 0)
     {
        EINA_LOG_ERR("Cannot create a log domain for Ecore Wayland");
        goto exit_eina;
     }

   if (!ecore_init())
     {
        ERR("Could not initialize ecore");
        goto exit_ecore;
     }

   if (!ecore_event_init())
     {
        ERR("Could not initialize ecore_event");
        goto exit_ecore_event;
     }

   if (!ECORE_WL_EVENT_MOUSE_IN)
     {
        ECORE_WL_EVENT_MOUSE_IN = ecore_event_type_new();
        ECORE_WL_EVENT_MOUSE_OUT = ecore_event_type_new();
        ECORE_WL_EVENT_FOCUS_IN = ecore_event_type_new();
        ECORE_WL_EVENT_FOCUS_OUT = ecore_event_type_new();
        ECORE_WL_EVENT_WINDOW_CONFIGURE = ecore_event_type_new();
        ECORE_WL_EVENT_WINDOW_ACTIVATE = ecore_event_type_new();
        ECORE_WL_EVENT_WINDOW_DEACTIVATE = ecore_event_type_new();
        ECORE_WL_EVENT_WINDOW_VISIBILITY_CHANGE = ecore_event_type_new();
        ECORE_WL_EVENT_WINDOW_PRE_VISIBILITY_CHANGE = ecore_event_type_new();
        ECORE_WL_EVENT_WINDOW_SHOW = ecore_event_type_new();
        ECORE_WL_EVENT_WINDOW_HIDE = ecore_event_type_new();
        ECORE_WL_EVENT_WINDOW_LOWER = ecore_event_type_new();
        ECORE_WL_EVENT_WINDOW_ROTATE = ecore_event_type_new();
        ECORE_WL_EVENT_DND_ENTER = ecore_event_type_new();
        ECORE_WL_EVENT_DND_POSITION = ecore_event_type_new();
        ECORE_WL_EVENT_DND_LEAVE = ecore_event_type_new();
        ECORE_WL_EVENT_DND_DROP = ecore_event_type_new();
        ECORE_WL_EVENT_DND_OFFER = ecore_event_type_new();
        ECORE_WL_EVENT_DND_END = ecore_event_type_new();
        ECORE_WL_EVENT_DATA_SOURCE_TARGET = ecore_event_type_new();
        ECORE_WL_EVENT_DATA_SOURCE_SEND = ecore_event_type_new();
        ECORE_WL_EVENT_SELECTION_DATA_READY = ecore_event_type_new();
        ECORE_WL_EVENT_DATA_SOURCE_CANCELLED = ecore_event_type_new();
        ECORE_WL_EVENT_INTERFACES_BOUND = ecore_event_type_new();
        ECORE_WL_EVENT_CONFORMANT_CHANGE = ecore_event_type_new();
        ECORE_WL_EVENT_AUX_HINT_ALLOWED = ecore_event_type_new();
        ECORE_WL_EVENT_AUX_MESSAGE = ecore_event_type_new();
        ECORE_WL_EVENT_WINDOW_ICONIFY_STATE_CHANGE = ecore_event_type_new();
        ECORE_WL_EVENT_EFFECT_START = ecore_event_type_new();
        ECORE_WL_EVENT_EFFECT_END = ecore_event_type_new();
        ECORE_WL_EVENT_OUTPUT_TRANSFORM = ecore_event_type_new();
        ECORE_WL_EVENT_GLOBAL_ADDED = ecore_event_type_new();
        ECORE_WL_EVENT_GLOBAL_REMOVED = ecore_event_type_new();
        ECORE_WL_EVENT_KEYMAP_UPDATE = ecore_event_type_new();
        ECORE_WL_EVENT_INDICATOR_FLICK = ecore_event_type_new();
        ECORE_WL_EVENT_CLIPBOARD_DATA_SELECTED = ecore_event_type_new();
     }

   wl_log_set_handler_client(_ecore_wl_log_cb_print);

   if (!(_ecore_wl_disp = calloc(1, sizeof(Ecore_Wl_Display))))
     {
        ERR("Could not allocate memory for Ecore_Wl_Display structure");
        goto exit_ecore_disp;
     }

   if (!(_ecore_wl_disp->wl.display = wl_display_connect(name)))
     {
        ERR("Could not connect to Wayland display");
        goto exit_ecore_disp_connect;
     }

   _ecore_wl_disp->fd = wl_display_get_fd(_ecore_wl_disp->wl.display);

   _ecore_wl_disp->fd_hdl =
     ecore_main_fd_handler_add(_ecore_wl_disp->fd,
                               ECORE_FD_READ | ECORE_FD_WRITE | ECORE_FD_ERROR,
                               _ecore_wl_cb_handle_data, _ecore_wl_disp,
                               NULL, NULL);
   ecore_main_fd_handler_prepare_callback_set(_ecore_wl_disp->fd_hdl,
                                              _ecore_wl_cb_pre_handle_data,
                                              _ecore_wl_disp);

   ecore_main_awake_handler_add(_ecore_wl_cb_awake, _ecore_wl_disp);

   _ecore_wl_disp->idle_enterer =
     ecore_idle_enterer_add(_ecore_wl_cb_idle_enterer, _ecore_wl_disp);

   _ecore_wl_disp->wl.registry =
     wl_display_get_registry(_ecore_wl_disp->wl.display);
   wl_registry_add_listener(_ecore_wl_disp->wl.registry,
                            &_ecore_wl_registry_listener, _ecore_wl_disp);

   //session_recovery_add_listener(_ecore_wl_disp->wl.session_recovery,
                            //&_ecore_wl_session_recovery_listener, _ecore_wl_disp);

   if (!_ecore_wl_xkb_init(_ecore_wl_disp))
     {
        ERR("Could not initialize XKB");
        goto exit_ecore_disp_connect;
     }

   _ecore_wl_window_init();
   _ecore_wl_events_init();

   _ecore_wl_disp->init_done = EINA_TRUE;
   if (!_ecore_wl_server_mode)
     {
        _ecore_wl_disp->init_done = EINA_FALSE;
        callback = wl_display_sync(_ecore_wl_disp->wl.display);
        wl_callback_add_listener(callback, &_ecore_wl_init_sync_listener,
                                 _ecore_wl_disp);
     }

   return _ecore_wl_init_count;

exit_ecore_disp_connect:
   free(_ecore_wl_disp);
   _ecore_wl_disp = NULL;

exit_ecore_disp:
   ecore_event_shutdown();

exit_ecore_event:
   ecore_shutdown();

exit_ecore:
   eina_log_domain_unregister(_ecore_wl_log_dom);
   _ecore_wl_log_dom = -1;

exit_eina:
   eina_shutdown();
   return --_ecore_wl_init_count;
}

EAPI int
ecore_wl_shutdown(void)
{
   LOGFN(__FILE__, __LINE__, __FUNCTION__);

   return _ecore_wl_shutdown(EINA_TRUE);
}

EAPI void
ecore_wl_flush(void)
{
   if ((!_ecore_wl_disp) || (!_ecore_wl_disp->wl.display)) return;
   wl_display_flush(_ecore_wl_disp->wl.display);
}

EAPI void
ecore_wl_sync(void)
{
   int ret;
   if ((!_ecore_wl_disp) || (!_ecore_wl_disp->wl.display)) return;
   _ecore_wl_sync_wait(_ecore_wl_disp);
   while (_ecore_wl_disp->sync_ref_count > 0)
     {
        ret = wl_display_dispatch(_ecore_wl_disp->wl.display);
        if ((ret < 0) && ((errno != EAGAIN) && (errno != EINVAL)))
          {
             /* raise exit signal */
             ERR("Wayland socket error: %s", strerror(errno));
             abort();
             break;
          }
     }
}

EAPI struct wl_shm *
ecore_wl_shm_get(void)
{
   if (!_ecore_wl_disp) return NULL;

   _ecore_wl_init_wait();

   return _ecore_wl_disp->wl.shm;
}

EAPI struct wl_display *
ecore_wl_display_get(void)
{
   if ((!_ecore_wl_disp) || (!_ecore_wl_disp->wl.display))
     return NULL;
   return _ecore_wl_disp->wl.display;
}

EAPI Eina_Inlist *
ecore_wl_globals_get(void)
{
   if ((!_ecore_wl_disp) || (!_ecore_wl_disp->wl.display))
     return NULL;

   _ecore_wl_init_wait();

   return _ecore_wl_disp->globals;
}

EAPI struct wl_registry *
ecore_wl_registry_get(void)
{
   if ((!_ecore_wl_disp) || (!_ecore_wl_disp->wl.display))
     return NULL;
   return _ecore_wl_disp->wl.registry;
}

EAPI struct wl_compositor *
ecore_wl_compositor_get(void)
{
   return _ecore_wl_compositor_get();
}

struct wl_compositor *
_ecore_wl_compositor_get(void)
{
   if ((!_ecore_wl_disp) || (!_ecore_wl_disp->wl.display))
     return NULL;

   _ecore_wl_init_wait();

   return _ecore_wl_disp->wl.compositor;
}

struct wl_subcompositor *
_ecore_wl_subcompositor_get(void)
{
   if ((!_ecore_wl_disp) || (!_ecore_wl_disp->wl.display))
     return NULL;

   _ecore_wl_init_wait();

   return _ecore_wl_disp->wl.subcompositor;
}

EAPI void
ecore_wl_screen_size_get(int *w, int *h)
{
   Ecore_Wl_Output *out;
   Eina_Inlist *tmp;
   int ow = 0, oh = 0;

   LOGFN(__FILE__, __LINE__, __FUNCTION__);

   if (w) *w = 0;
   if (h) *h = 0;

   if ((!_ecore_wl_disp) || (!_ecore_wl_disp->wl.display)) return;

   _ecore_wl_init_wait();

   // the first sync is in case registry replies are not back yet
   if (!_ecore_wl_disp->output)
     {
        // second sync is in case bound object replies in registry are not back
        ecore_wl_sync();
        if (!_ecore_wl_disp->output) ecore_wl_sync();
     }

   EINA_INLIST_FOREACH_SAFE(_ecore_wl_disp->outputs, tmp, out)
     {
        switch (out->transform)
          {
           case WL_OUTPUT_TRANSFORM_90:
           case WL_OUTPUT_TRANSFORM_270:
           case WL_OUTPUT_TRANSFORM_FLIPPED_90:
           case WL_OUTPUT_TRANSFORM_FLIPPED_270:
             /* Swap width and height */
             ow += out->allocation.h;
             oh += out->allocation.w;
             break;
           default:
             ow += out->allocation.w;
             oh += out->allocation.h;
          }
     }

   if (w) *w = ow;
   if (h) *h = oh;
}

EAPI Ecore_Wl_Output *
ecore_wl_window_output_find(Ecore_Wl_Window *window)
{
   Ecore_Wl_Output *out;
   Eina_Inlist *tmp;
   int x = 0, y = 0;

   ecore_wl_window_geometry_get(window, &x, &y, NULL, NULL);

   EINA_INLIST_FOREACH_SAFE(_ecore_wl_disp->outputs, tmp, out)
     {
        int ox, oy, ow, oh;

        ox = out->allocation.x;
        oy = out->allocation.y;

        switch (out->transform)
          {
           case WL_OUTPUT_TRANSFORM_90:
           case WL_OUTPUT_TRANSFORM_270:
           case WL_OUTPUT_TRANSFORM_FLIPPED_90:
           case WL_OUTPUT_TRANSFORM_FLIPPED_270:
             /* Swap width and height */
             ow = out->allocation.h;
             oh = out->allocation.w;
             break;
           default:
             ow = out->allocation.w;
             oh = out->allocation.h;
             break;
          }

        if ((x >= ox && x < ow) && (y >= oy && y < oh))
          return out;
     }

   return NULL;
}


/* @since 1.2 */
EAPI void
ecore_wl_pointer_xy_get(int *x, int *y)
{
   LOGFN(__FILE__, __LINE__, __FUNCTION__);

   _ecore_wl_input_pointer_xy_get(x, y);
}

EAPI int
ecore_wl_dpi_get(void)
{
   int w, mw;

   LOGFN(__FILE__, __LINE__, __FUNCTION__);

   if (!_ecore_wl_disp) return 0;

   _ecore_wl_init_wait();

   //TIZEN_ONLY(20161222): Insert also ecore_wl_display output sync in ecore_wl_dpi_get
   // the first sync is in case registry replies are not back yet
   if (!_ecore_wl_disp->output)
     {
        // second sync is in case bound object replies in registry are not back
        ecore_wl_sync();
        if (!_ecore_wl_disp->output) ecore_wl_sync();
     }
   //
   if (!_ecore_wl_disp->output) return 75;

   mw = _ecore_wl_disp->output->mw;
   if (mw <= 0) return 75;

   w = _ecore_wl_disp->output->allocation.w;

   //TIZEN_ONLY(20160519): The height size of screen should be considered,
   //                      when calc the dpi value.
   /* FIXME: NB: Hrrrmmm, need to verify this. xorg code is using a different
    * formula to calc this */
   //return (((w * 254) / mw) + 5) / 10;
   int h, mh, dpi;
   double target_inch;

   mh = _ecore_wl_disp->output->mh;
   if (mh <= 0) return 75;

   h = _ecore_wl_disp->output->allocation.h;

   target_inch = (round((sqrt(mw * mw + mh * mh) / 25.4) * 10) / 10);
   dpi = (round((sqrt(w * w + h * h) / target_inch) * 10) / 10);

   return dpi;
   //
}

EAPI void
ecore_wl_display_iterate(void)
{
   int ret;
   if ((!_ecore_wl_disp) || (!_ecore_wl_disp->wl.display)) return;
   if (!_ecore_wl_server_mode)
     {
        ret = wl_display_dispatch(_ecore_wl_disp->wl.display);
        if ((ret < 0) && ((errno != EAGAIN) && (errno != EINVAL)))
          {
             /* raise exit signal */
             ERR("Wayland socket error: %s", strerror(errno));
             abort();
          }
     }
}

/* @since 1.8 */
EAPI Eina_Bool
ecore_wl_animator_source_set(Ecore_Animator_Source source)
{
   LOGFN(__FILE__, __LINE__, __FUNCTION__);

   if (_ecore_wl_server_mode) return EINA_FALSE;

   /* TIZEN_ONLY : To use vsync as custom source of animator */
   switch (source)
     {
      case ECORE_ANIMATOR_SOURCE_CUSTOM:
        ecore_animator_custom_source_tick_begin_callback_set
          (_ecore_wl_animator_vsync_tick_begin, NULL);
        ecore_animator_custom_source_tick_end_callback_set
          (_ecore_wl_animator_vsync_tick_end, NULL);
        ecore_animator_custom_source_tick_quit_callback_set
          (_ecore_wl_animator_vsync_tick_quit, NULL);
        break;
      case ECORE_ANIMATOR_SOURCE_TIMER:
        ecore_animator_custom_source_tick_begin_callback_set(NULL, NULL);
        ecore_animator_custom_source_tick_end_callback_set(NULL, NULL);
        break;
      default:
        break;
     }

   /* set the source of the animator */
   ecore_animator_source_set(source);

   return EINA_TRUE;
}

EAPI struct wl_cursor *
ecore_wl_cursor_get(const char *cursor_name)
{
   if ((!_ecore_wl_disp) || (!_ecore_wl_disp->cursor_theme))
     return NULL;

   return wl_cursor_theme_get_cursor(_ecore_wl_disp->cursor_theme,
                                     cursor_name);
}

EAPI void
ecore_wl_server_mode_set(Eina_Bool on)
{
   _ecore_wl_server_mode = on;
}

EAPI Eina_Bool
ecore_wl_server_mode_get(void)
{
   return _ecore_wl_server_mode;
}

/* local functions */
static int
_ecore_wl_shutdown(Eina_Bool close)
{
   LOGFN(__FILE__, __LINE__, __FUNCTION__);

   if (_ecore_wl_init_count < 1)
     {
        ERR("Ecore_Wayland Shutdown called without Ecore_Wayland Init");
        return 0;
     }

   if (--_ecore_wl_init_count != 0) return _ecore_wl_init_count;
   if (!_ecore_wl_disp) return _ecore_wl_init_count;

   if (_keygrabs)
     {
        eina_hash_free(_keygrabs);
        _keygrabs = NULL;
     }

   _ecore_wl_events_shutdown();
   _ecore_wl_window_shutdown();

   ecore_main_awake_handler_del(_ecore_wl_cb_awake);
   if (_ecore_wl_disp->fd_hdl)
     ecore_main_fd_handler_del(_ecore_wl_disp->fd_hdl);
   if (_ecore_wl_disp->idle_enterer)
      ecore_idle_enterer_del(_ecore_wl_disp->idle_enterer);

   if ((close) && (!_ecore_wl_fatal_error))
     {
        Ecore_Wl_Output *out;
        Ecore_Wl_Input *in;
        Ecore_Wl_Global *global;
        Eina_Inlist *tmp;

        EINA_INLIST_FOREACH_SAFE(_ecore_wl_disp->outputs, tmp, out)
          _ecore_wl_output_del(out);

        EINA_INLIST_FOREACH_SAFE(_ecore_wl_disp->inputs, tmp, in)
          _ecore_wl_input_del(in);

        EINA_INLIST_FOREACH_SAFE(_ecore_wl_disp->globals, tmp, global)
          {
             _ecore_wl_disp->globals =
               eina_inlist_remove(_ecore_wl_disp->globals,
                                  EINA_INLIST_GET(global));
             free(global->interface);
             free(global);
          }

        _ecore_wl_xkb_shutdown(_ecore_wl_disp);

        if (_ecore_wl_disp->wl.session_recovery)
          session_recovery_destroy(_ecore_wl_disp->wl.session_recovery);
#ifdef USE_IVI_SHELL
        if (_ecore_wl_disp->wl.ivi_application)
          ivi_application_destroy(_ecore_wl_disp->wl.ivi_application);
#endif
        if (_ecore_wl_disp->wl.xdg_shell)
          xdg_shell_destroy(_ecore_wl_disp->wl.xdg_shell);
        if (_ecore_wl_disp->wl.shell)
          wl_shell_destroy(_ecore_wl_disp->wl.shell);
        if (_ecore_wl_disp->wl.shm) wl_shm_destroy(_ecore_wl_disp->wl.shm);
        if (_ecore_wl_disp->wl.data_device_manager)
          wl_data_device_manager_destroy(_ecore_wl_disp->wl.data_device_manager);
        if (_ecore_wl_disp->wl.tz_input_device_manager)
          tizen_input_device_manager_destroy(_ecore_wl_disp->wl.tz_input_device_manager);
        if (_ecore_wl_disp->wl.tz_policy)
          tizen_policy_destroy(_ecore_wl_disp->wl.tz_policy);
        if (_ecore_wl_disp->wl.tz_policy_ext)
          tizen_policy_ext_destroy(_ecore_wl_disp->wl.tz_policy_ext);
        if (_ecore_wl_disp->wl.tz_surf)
          tizen_surface_destroy(_ecore_wl_disp->wl.tz_surf);
        if (_ecore_wl_disp->wl.compositor)
          wl_compositor_destroy(_ecore_wl_disp->wl.compositor);
        if (_ecore_wl_disp->wl.subcompositor)
          wl_subcompositor_destroy(_ecore_wl_disp->wl.subcompositor);
// TIZEN_ONLY(20150722): Add ecore_wl_window_keygrab_* APIs
        if (_ecore_wl_disp->wl.keyrouter)
          tizen_keyrouter_destroy(_ecore_wl_disp->wl.keyrouter);
//
        if (_ecore_wl_disp->wl.tz_effect)
          tizen_effect_destroy(_ecore_wl_disp->wl.tz_effect);
        if (_ecore_wl_disp->wl.tz_indicator)
          tizen_indicator_destroy(_ecore_wl_disp->wl.tz_indicator);
        if (_ecore_wl_disp->wl.tz_clipboard)
          tizen_clipboard_destroy(_ecore_wl_disp->wl.tz_clipboard);
        if (_ecore_wl_disp->cursor_theme)
          wl_cursor_theme_destroy(_ecore_wl_disp->cursor_theme);
        if (_ecore_wl_disp->wl.display)
          {
             wl_registry_destroy(_ecore_wl_disp->wl.registry);
             wl_display_flush(_ecore_wl_disp->wl.display);
             wl_display_disconnect(_ecore_wl_disp->wl.display);
          }
        free(_ecore_wl_disp);
     }

   ecore_event_shutdown();
   ecore_shutdown();

   eina_log_domain_unregister(_ecore_wl_log_dom);
   _ecore_wl_log_dom = -1;
   eina_shutdown();

   return _ecore_wl_init_count;
}

static Eina_Bool
_ecore_wl_cb_idle_enterer(void *data)
{
   Ecore_Wl_Display *ewd;
   int ret = 0;

   if (_ecore_wl_fatal_error) return ECORE_CALLBACK_CANCEL;

   if (!(ewd = data)) return ECORE_CALLBACK_RENEW;

   ret = wl_display_get_error(ewd->wl.display);
   if (ret < 0) goto err;

   ret = wl_display_dispatch_pending(ewd->wl.display);
   if (ret < 0) goto err;

   ret = wl_display_flush(ewd->wl.display);
   if ((ret < 0) && (errno == EAGAIN))
     ecore_main_fd_handler_active_set(ewd->fd_hdl,
                                      (ECORE_FD_READ | ECORE_FD_WRITE));

   return ECORE_CALLBACK_RENEW;

err:
   if ((ret < 0) && ((errno != EAGAIN) && (errno != EINVAL)))
     {
        _ecore_wl_fatal_error = EINA_TRUE;

        /* raise exit signal */
        ERR("Wayland socket error: %s", strerror(errno));
        _ecore_wl_signal_exit();

        return ECORE_CALLBACK_CANCEL;
     }

   return ECORE_CALLBACK_RENEW;
}

static Eina_Bool
_ecore_wl_cb_handle_data(void *data, Ecore_Fd_Handler *hdl)
{
   Ecore_Wl_Display *ewd;
   int ret = 0;

   /* LOGFN(__FILE__, __LINE__, __FUNCTION__); */

   if (_ecore_wl_fatal_error) return ECORE_CALLBACK_CANCEL;

   if (!(ewd = data)) return ECORE_CALLBACK_RENEW;

   if (ecore_main_fd_handler_active_get(hdl, ECORE_FD_ERROR))
     {
        ERR("Received error on wayland display fd");
        _ecore_wl_fatal_error = EINA_TRUE;
        _ecore_wl_signal_exit();

        return ECORE_CALLBACK_CANCEL;
     }

   if (ecore_main_fd_handler_active_get(hdl, ECORE_FD_READ))
     ret = wl_display_dispatch_pending(ewd->wl.display);
   else if (ecore_main_fd_handler_active_get(hdl, ECORE_FD_WRITE))
     {
        ret = wl_display_flush(ewd->wl.display);
        if (ret == 0)
          ecore_main_fd_handler_active_set(hdl, ECORE_FD_READ);
     }

   if ((ret < 0) && ((errno != EAGAIN) && (errno != EINVAL)))
     {
        _ecore_wl_fatal_error = EINA_TRUE;

        /* raise exit signal */
        _ecore_wl_signal_exit();

        return ECORE_CALLBACK_CANCEL;
     }

   return ECORE_CALLBACK_RENEW;
}

static void
_ecore_wl_cb_pre_handle_data(void *data, Ecore_Fd_Handler *hdl EINA_UNUSED)
{
   Ecore_Wl_Display *ewd;

   if (_ecore_wl_fatal_error) return;

   if (!(ewd = data)) return;

   if (ewd->wl.prepare_read) return;

   while (wl_display_prepare_read(ewd->wl.display) != 0)
     {
        wl_display_dispatch_pending(ewd->wl.display);
     }
   wl_display_flush(ewd->wl.display);

   ewd->wl.prepare_read = EINA_TRUE;
}

static void
_cb_global_event_free(void *data EINA_UNUSED, void *event)
{
   Ecore_Wl_Event_Global *ev;

   ev = event;
   eina_stringshare_del(ev->interface);
   free(ev);
}

static void
_cb_aux_message_free(void *data EINA_UNUSED, void *event)
{
   Ecore_Wl_Event_Aux_Message *ev;
   char *str;

   ev = event;
   eina_stringshare_del(ev->key);
   eina_stringshare_del(ev->val);
   EINA_LIST_FREE(ev->options, str)
      eina_stringshare_del(str);
   free(ev);
}

static void
_ecore_wl_cb_awake(void *data)
{
   Ecore_Wl_Display *ewd;
   Ecore_Fd_Handler_Flags flags = ECORE_FD_READ|ECORE_FD_WRITE|ECORE_FD_ERROR;

   if (_ecore_wl_fatal_error) return;
   if (!(ewd = data)) return;
   if (!ewd->wl.prepare_read) return;

   ewd->wl.prepare_read = EINA_FALSE;

   if (ecore_main_fd_handler_active_get(_ecore_wl_disp->fd_hdl, flags))
     wl_display_read_events(ewd->wl.display);
   else
     wl_display_cancel_read(ewd->wl.display);
}

static void
_ecore_wl_cb_handle_global(void *data, struct wl_registry *registry, unsigned int id, const char *interface, unsigned int version)
{
   Ecore_Wl_Display *ewd;
   Ecore_Wl_Global *global;
   Ecore_Wl_Event_Global *ev;
   int client_version = 1;

   LOGFN(__FILE__, __LINE__, __FUNCTION__);

   ewd = data;

   if (!(global = calloc(1, sizeof(Ecore_Wl_Global)))) return;

   global->id = id;
   global->interface = strdup(interface);
   global->version = version;
   ewd->globals = eina_inlist_append(ewd->globals, EINA_INLIST_GET(global));

   if (!strcmp(interface, "wl_compositor"))
     {
        ewd->wl.compositor =
          wl_registry_bind(registry, id, &wl_compositor_interface, 3);
     }
   else if (!strcmp(interface, "wl_subcompositor"))
     {
        ewd->wl.subcompositor =
           wl_registry_bind(registry, id, &wl_subcompositor_interface, 1);
     }
   else if (!strcmp(interface, "wl_output"))
     _ecore_wl_output_add(ewd, id);
   else if (!strcmp(interface, "wl_seat"))
     _ecore_wl_input_add(ewd, id);
   else if (!strcmp(interface, "session_recovery"))
     {
        ewd->wl.session_recovery =
          wl_registry_bind(registry, id, &session_recovery_interface, 1);
     }
#ifdef USE_IVI_SHELL
   else if (!strcmp(interface, "ivi_application"))
     {
        ewd->wl.ivi_application =
          wl_registry_bind(registry, id, &ivi_application_interface, 1);
     }
#endif
   else if (!strcmp(interface, "xdg_shell") && !getenv("EFL_WAYLAND_DONT_USE_XDG_SHELL"))
     {
        Eina_Hash *h;
        Eina_Iterator *it;
        Ecore_Wl_Window *win;

        ewd->wl.xdg_shell =
          wl_registry_bind(registry, id, &xdg_shell_interface, 1);
        xdg_shell_use_unstable_version(ewd->wl.xdg_shell, XDG_VERSION);
        xdg_shell_add_listener(ewd->wl.xdg_shell, &xdg_shell_listener,
                               ewd->wl.display);
        h = _ecore_wl_window_hash_get();
        it = eina_hash_iterator_data_new(h);
        EINA_ITERATOR_FOREACH(it, win)
          if (win->surface)
            _ecore_wl_window_shell_surface_init(win);
        eina_iterator_free(it);
     }
   else if (!strcmp(interface, "wl_shell"))
     {
        ewd->wl.shell =
          wl_registry_bind(registry, id, &wl_shell_interface, 1);
     }
   else if (!strcmp(interface, "wl_shm"))
     {
        ewd->wl.shm = wl_registry_bind(registry, id, &wl_shm_interface, 1);

        if (ewd->input)
          _ecore_wl_input_setup(ewd->input);
        else if (!ewd->cursor_theme)
          {
             ewd->cursor_theme =
               wl_cursor_theme_load(NULL, ECORE_WL_DEFAULT_CURSOR_SIZE,
                                    ewd->wl.shm);
          }
     }
   else if (!strcmp(interface, "wl_data_device_manager"))
     {
        ewd->wl.data_device_manager =
          wl_registry_bind(registry, id, &wl_data_device_manager_interface, 1);
     }
   else if (!strcmp(interface, "tizen_policy"))
     {
        if (version >= 5)
          client_version = 5;
        else
          client_version = version;

        ewd->wl.tz_policy =
          wl_registry_bind(registry, id, &tizen_policy_interface, client_version);
        if (ewd->wl.tz_policy)
          tizen_policy_add_listener(_ecore_wl_disp->wl.tz_policy, &_ecore_tizen_policy_listener, ewd->wl.display);
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
          tizen_policy_ext_add_listener(_ecore_wl_disp->wl.tz_policy_ext, &_ecore_tizen_policy_ext_listener, ewd->wl.display);
     }
   else if (!strcmp(interface, "tizen_surface"))
     {
        ewd->wl.tz_surf =
          wl_registry_bind(registry, id, &tizen_surface_interface, 1);
     }
// TIZEN_ONLY(20150722): Add ecore_wl_window_keygrab_* APIs
   else if (!strcmp(interface, "tizen_keyrouter"))
     {
        ewd->wl.keyrouter =
          wl_registry_bind(registry, id, &tizen_keyrouter_interface, 1);
        if (ewd->wl.keyrouter)
          tizen_keyrouter_add_listener(_ecore_wl_disp->wl.keyrouter, &_ecore_tizen_keyrouter_listener, ewd->input);
     }
//
   else if (!strcmp(interface, "tizen_input_device_manager"))
     _ecore_wl_input_device_manager_setup(id);
   else if (!strcmp(interface, "tizen_effect"))
     {
        ewd->wl.tz_effect =
           wl_registry_bind(registry, id, &tizen_effect_interface, 1);
        if (ewd->wl.tz_effect)
          tizen_effect_add_listener(ewd->wl.tz_effect, &_ecore_tizen_effect_listener, ewd->wl.display);
     }
   else if (!strcmp(interface, "tizen_indicator"))
     {
        ewd->wl.tz_indicator =
          wl_registry_bind(registry, id, &tizen_indicator_interface, 1);
        if (ewd->wl.tz_indicator)
          tizen_indicator_add_listener(ewd->wl.tz_indicator, &_ecore_tizen_indicator_listener, ewd->wl.display);
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
          tizen_clipboard_add_listener(ewd->wl.tz_clipboard, &_ecore_tizen_clipboard_listener, ewd->wl.display);
     }

   if ((ewd->wl.compositor) && (ewd->wl.shm) &&
       ((ewd->wl.shell) || (ewd->wl.xdg_shell)))
     {
        Ecore_Wl_Event_Interfaces_Bound *ev;

        if (!(ev = calloc(1, sizeof(Ecore_Wl_Event_Interfaces_Bound))))
          return;

        ev->compositor = (ewd->wl.compositor != NULL);
        ev->shm = (ewd->wl.shm != NULL);
        ev->shell = ((ewd->wl.shell != NULL) || (ewd->wl.xdg_shell != NULL));
        ev->output = (ewd->output != NULL);
        ev->seat = (ewd->input != NULL);
        ev->data_device_manager = (ewd->wl.data_device_manager != NULL);
        ev->policy = (ewd->wl.tz_policy != NULL);
        ev->policy_ext = (ewd->wl.tz_policy_ext != NULL);
        ev->subcompositor = (ewd->wl.subcompositor != NULL);

        ecore_event_add(ECORE_WL_EVENT_INTERFACES_BOUND, ev, NULL, NULL);
     }

   /* allocate space for event structure */
   ev = calloc(1, sizeof(Ecore_Wl_Event_Global));
   if (!ev) return;

   ev->id = id;
   ev->display = ewd;
   ev->version = version;
   ev->interface = eina_stringshare_add(interface);

   /* raise an event saying a new global has been added */
   ecore_event_add(ECORE_WL_EVENT_GLOBAL_ADDED, ev,
                   _cb_global_event_free, NULL);
}

static void
_ecore_wl_cb_handle_global_remove(void *data, struct wl_registry *registry EINA_UNUSED, unsigned int id)
{
   Ecore_Wl_Display *ewd;
   Ecore_Wl_Global *global;
   Ecore_Wl_Event_Global *ev;
   Eina_Inlist *tmp;

   LOGFN(__FILE__, __LINE__, __FUNCTION__);

   ewd = data;

   EINA_INLIST_FOREACH_SAFE(ewd->globals, tmp, global)
     {
        if (global->id != id) continue;

        /* allocate space for event structure */
        ev = calloc(1, sizeof(Ecore_Wl_Event_Global));
        if (!ev) return;

        ev->id = id;
        ev->display = ewd;
        ev->version = global->version;
        ev->interface = eina_stringshare_add(global->interface);

        /* raise an event saying a global has been removed */
        ecore_event_add(ECORE_WL_EVENT_GLOBAL_REMOVED, ev,
                        _cb_global_event_free, NULL);

        ewd->globals =
          eina_inlist_remove(ewd->globals, EINA_INLIST_GET(global));
        free(global->interface);
        free(global);
     }
}

static Eina_Bool
_ecore_wl_xkb_init(Ecore_Wl_Display *ewd)
{
   LOGFN(__FILE__, __LINE__, __FUNCTION__);

   if (!(ewd->xkb.context = xkb_context_new(0)))
     return EINA_FALSE;

   return EINA_TRUE;
}

static Eina_Bool
_ecore_wl_xkb_shutdown(Ecore_Wl_Display *ewd)
{
   LOGFN(__FILE__, __LINE__, __FUNCTION__);

   xkb_context_unref(ewd->xkb.context);

   return EINA_TRUE;
}

static void
_ecore_wl_sync_callback(void *data, struct wl_callback *callback, uint32_t serial EINA_UNUSED)
{
   Ecore_Wl_Display *ewd = data;

   ewd->sync_ref_count--;
   wl_callback_destroy(callback);
}

static void
_ecore_wl_sync_wait(Ecore_Wl_Display *ewd)
{
   struct wl_callback *callback;

   ewd->sync_ref_count++;
   callback = wl_display_sync(ewd->wl.display);
   wl_callback_add_listener(callback, &_ecore_wl_sync_listener, ewd);
}

static void
_ecore_wl_animator_tick_cb_begin(void *data EINA_UNUSED)
{
   Eina_Hash *windows;

   _ecore_wl_animator_busy = EINA_TRUE;

   windows = _ecore_wl_window_hash_get();
   eina_hash_foreach(windows, _ecore_wl_animator_window_add, NULL);
}

static void
_ecore_wl_animator_tick_cb_end(void *data EINA_UNUSED)
{
   _ecore_wl_animator_busy = EINA_FALSE;
}

static void
_ecore_wl_animator_callback(void *data, struct wl_callback *callback, uint32_t serial EINA_UNUSED)
{
   Ecore_Wl_Window *win;

   if (!(win = data)) return;

   ecore_animator_custom_tick();

   wl_callback_destroy(callback);
   win->anim_callback = NULL;

   if (_ecore_wl_animator_busy)
     {
        win->anim_callback = wl_surface_frame(win->surface);
        wl_callback_add_listener(win->anim_callback,
                                 &_ecore_wl_anim_listener, win);
        ecore_wl_window_commit(win);
     }
}

static Eina_Bool
_ecore_wl_animator_window_add(const Eina_Hash *hash EINA_UNUSED, const void *key EINA_UNUSED, void *data, void *fdata EINA_UNUSED)
{
   Ecore_Wl_Window *win;

   if (!(win = data)) return EINA_TRUE;
   if (!win->surface) return EINA_TRUE;
   if (win->anim_callback) return EINA_TRUE;

   win->anim_callback = wl_surface_frame(win->surface);
   wl_callback_add_listener(win->anim_callback, &_ecore_wl_anim_listener, win);
   ecore_wl_window_commit(win);

   return EINA_TRUE;
}

static void
_ecore_wl_signal_exit(void)
{
   Ecore_Event_Signal_Exit *ev;

   if (!(ev = calloc(1, sizeof(Ecore_Event_Signal_Exit))))
     return;

   ev->quit = 1;
   ecore_event_add(ECORE_EVENT_SIGNAL_EXIT, ev,
                   _ecore_wl_signal_exit_free, NULL);
}

static void
_ecore_wl_signal_exit_free(void *data EINA_UNUSED, void *event)
{
   free(event);
}

// TIZEN_ONLY(20150722): Add ecore_wl_window_keygrab_* APIs
//Currently this function is only used in sink call, so use global value(_ecore_wl_keygrab_error) and just check the error is ok.
/* internal functions */
static Eina_Bool
_ecore_wl_keygrab_hash_add(void *key, void *data)
{
   Eina_Bool ret = EINA_FALSE;

   if (!_keygrabs)
     _keygrabs = eina_hash_int32_new(NULL);
   ret = eina_hash_add(_keygrabs, key, data);
   return ret;
}

static Eina_Bool
_ecore_wl_keygrab_hash_del(void *key)
{
   Eina_Bool ret = EINA_FALSE;

   ret = eina_hash_del_by_key(_keygrabs, key);

   return ret;
}

Eina_Hash *
_ecore_wl_keygrab_hash_get(void)
{
   return _keygrabs;
}

static void
_ecore_wl_cb_keygrab_notify(void *data EINA_UNUSED, struct tizen_keyrouter *tizen_keyrouter EINA_UNUSED, struct wl_surface *surface EINA_UNUSED, uint32_t key, uint32_t mode, uint32_t error)
{
   LOGFN(__FILE__, __LINE__, __FUNCTION__);

   _ecore_wl_keygrab_error = error;
   INF("[PID:%d] key=%d, mode=%d, error=%d", getpid(), key, mode, error);
}

static void
_ecore_wl_cb_keygrab_notify_list(void *data EINA_UNUSED, struct tizen_keyrouter *tizen_keyrouter EINA_UNUSED, struct wl_surface *surface EINA_UNUSED, struct wl_array *grab_result)
{
   LOGFN(__FILE__, __LINE__, __FUNCTION__);

   wl_array_init(&_ecore_wl_keygrab_result_list);
   wl_array_copy(&_ecore_wl_keygrab_result_list, grab_result);
}

static void
_ecore_wl_cb_getgrab_notify_list(void *data EINA_UNUSED, struct tizen_keyrouter *tizen_keyrouter EINA_UNUSED, struct wl_surface *surface EINA_UNUSED, struct wl_array *grab_result EINA_UNUSED)
{
   LOGFN(__FILE__, __LINE__, __FUNCTION__);
}

static void
_ecore_wl_cb_set_register_none_key(void *data EINA_UNUSED, struct tizen_keyrouter *tizen_keyrouter EINA_UNUSED, struct wl_surface *surface EINA_UNUSED, uint32_t mode EINA_UNUSED)
{
   LOGFN(__FILE__, __LINE__, __FUNCTION__);
}

static void
_ecore_wl_cb_keyregister_notify(void *data EINA_UNUSED, struct tizen_keyrouter *tizen_keyrouter EINA_UNUSED, uint32_t status EINA_UNUSED)
{
   LOGFN(__FILE__, __LINE__, __FUNCTION__);
}

static void
_ecore_wl_cb_set_input_config(void *data EINA_UNUSED, struct tizen_keyrouter *tizen_keyrouter EINA_UNUSED, uint32_t status EINA_UNUSED)
{
   LOGFN(__FILE__, __LINE__, __FUNCTION__);
}

static void
_ecore_wl_cb_key_cancel(void *data, struct tizen_keyrouter *tizen_keyrouter EINA_UNUSED, uint32_t key)
{
   Ecore_Wl_Input *input = (Ecore_Wl_Input *)data;

   if (!input)
     {
        WRN("Failed to get Ecore_Wl_Input\n");
        return;
     }

   if (input->repeat.key == key)
     {
        input->repeat.sym = 0;
        input->repeat.key = 0;
        input->repeat.time = 0;

        if (input->repeat.tmr) ecore_timer_del(input->repeat.tmr);
        input->repeat.tmr = NULL;
     }
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
ecore_wl_keycode_from_keysym(struct xkb_keymap *keymap, xkb_keysym_t keysym, xkb_keycode_t **keycodes)
{
   Keycode_Map found_keycodes = {0,};
   found_keycodes.keysym = keysym;

   //called fewer (max_keycode - min_keycode +1) times.
   xkb_keymap_key_for_each(keymap, find_keycode, &found_keycodes);

   *keycodes = found_keycodes.keycodes;
   INF("num of keycodes:%d ", found_keycodes.num_keycodes);
   return found_keycodes.num_keycodes;


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
ecore_wl_window_keygrab_set(Ecore_Wl_Window *win, const char *key, int mod EINA_UNUSED, int not_mod EINA_UNUSED, int priority EINA_UNUSED, Ecore_Wl_Window_Keygrab_Mode grab_mode)
{
   xkb_keysym_t keysym = 0x0;
   int num_keycodes = 0;
   xkb_keycode_t *keycodes = NULL;
   int i;

   Eina_Bool ret = EINA_FALSE;
   struct wl_surface *surface = NULL;

   LOGFN(__FILE__, __LINE__, __FUNCTION__);

   if (!_ecore_wl_disp) return EINA_FALSE;
   while (!_ecore_wl_disp->wl.keyrouter)
     {
        INF("Wait until keyrouter interface is ready");
        wl_display_roundtrip(_ecore_wl_disp->wl.display);
     }
   if (!key) return EINA_FALSE;
   if ((grab_mode < ECORE_WL_WINDOW_KEYGRAB_UNKNOWN) || (grab_mode > ECORE_WL_WINDOW_KEYGRAB_EXCLUSIVE))
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
   while (!_ecore_wl_disp->input)
     {
        INF("Wait wl_registry_add_listener reply");
        wl_display_roundtrip(_ecore_wl_disp->wl.display);
     }

   if (_ecore_wl_disp->input)
     {
        while (!_ecore_wl_disp->input->caps_update)
          {
             INF("Wait until wl_seat_capabilities_update is ready");
             wl_display_roundtrip(_ecore_wl_disp->wl.display);
          }
        if (_ecore_wl_disp->input->keyboard)
          {
             while (!_ecore_wl_disp->input->xkb.keymap)
               {
                  wl_display_roundtrip(_ecore_wl_disp->wl.display);
                  INF("Wait until keymap event occurs");
               }
             INF("Finish keymap event");

             num_keycodes = ecore_wl_keycode_from_keysym(_ecore_wl_disp->input->xkb.keymap, keysym, &keycodes);
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
     surface = ecore_wl_window_surface_get(win);

   for (i = 0; i < num_keycodes; i++)
     {
        INF("keycode of key:(%d)", keycodes[i]);
        tizen_keyrouter_set_keygrab(_ecore_wl_disp->wl.keyrouter, surface, keycodes[i], grab_mode);
        /* Send sync to wayland compositor and register sync callback to exit while dispatch loop below */
        ecore_wl_sync();

        INF("After keygrab _ecore_wl_keygrab_error = %d", _ecore_wl_keygrab_error);
        if (!_ecore_wl_keygrab_error)
          {
             INF("[PID:%d]Succeed to get return value !", getpid());
             if (_ecore_wl_keygrab_hash_add(&keycodes[i], surface))
               INF("Succeed to add key to the keygrab hash!");
             //TODO: deal with if (win == NULL)
             else
               WRN("Failed to add key to the keygrab hash!");
             ret = EINA_TRUE;
          }
        else
          {
             WRN("[PID:%d]Failed to get return value ! ret=%d)", getpid(), _ecore_wl_keygrab_error);
             ret = EINA_FALSE;
          }
     }

   free(keycodes);
   keycodes = NULL;

   _ecore_wl_keygrab_error = -1;
   return ret;
}

EAPI Eina_Bool
ecore_wl_window_keygrab_unset(Ecore_Wl_Window *win, const char *key, int mod EINA_UNUSED, int any_mod EINA_UNUSED)
{
   xkb_keysym_t keysym = 0x0;
   int num_keycodes = 0;
   xkb_keycode_t *keycodes = NULL;
   int i;

   Eina_Bool ret = EINA_FALSE;
   struct wl_surface *surface = NULL;

   LOGFN(__FILE__, __LINE__, __FUNCTION__);

   if ((!_ecore_wl_disp) || (!_ecore_wl_disp->wl.keyrouter)) return EINA_FALSE;
   if (!key) return EINA_FALSE;
   INF("win=%p key=%s ", win, key);

   keysym = xkb_keysym_from_name(key, XKB_KEYSYM_NO_FLAGS);
   if (keysym == XKB_KEY_NoSymbol)
     {
        WRN("Keysym of key(\"%s\") doesn't exist", key);
        return EINA_FALSE;
     }

   //We have to find the way to get keycode from keysym before keymap notify
   if ((_ecore_wl_disp->input) && (_ecore_wl_disp->input->xkb.keymap))
     num_keycodes = ecore_wl_keycode_from_keysym(_ecore_wl_disp->input->xkb.keymap, keysym, &keycodes);
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
     surface = ecore_wl_window_surface_get(win);

   for (i = 0; i < num_keycodes; i++)
     {
        INF("keycode of key:(%d)", keycodes[i]);
        tizen_keyrouter_unset_keygrab(_ecore_wl_disp->wl.keyrouter, surface, keycodes[i]);

        /* Send sync to wayland compositor and register sync callback to exit while dispatch loop below */
        ecore_wl_sync();

        INF("After keygrab unset  _ecore_wl_keygrab_error = %d", _ecore_wl_keygrab_error);
        if (!_ecore_wl_keygrab_error)
          {
             INF("[PID:%d]Succeed to get return value !", getpid());
             if (_ecore_wl_keygrab_hash_del(&keycodes[i]))
               INF("Succeed to delete key from the keygrab hash!");
             else
               WRN("Failed to delete key from the keygrab hash!");
             ret = EINA_TRUE;
          }
        else
          {
             ret = EINA_FALSE;
             WRN("[PID:%d] Failed to get return value ! (ret=%d)", getpid(), _ecore_wl_keygrab_error);
          }
     }

   free(keycodes);
   keycodes = NULL;

   _ecore_wl_keygrab_error = -1;
   return ret;
}

char *
_ecore_wl_keyname_get(int keycode)
{
   xkb_keysym_t sym = XKB_KEY_NoSymbol;
   char name[256] = {0, };

   sym = xkb_state_key_get_one_sym(_ecore_wl_disp->input->xkb.state, keycode);
   xkb_keysym_get_name(sym, name, sizeof(name));

   return strdup(name);
}

EAPI Eina_List
*ecore_wl_window_keygrab_list_set(Ecore_Wl_Window *win, Eina_List *infos)
{
   xkb_keysym_t keysym = 0x0;
   int num_keycodes = 0;
   xkb_keycode_t *keycodes = NULL;

   struct wl_surface *surface = NULL;

   struct wl_array grab_list;
   int *grab_data = NULL;
   Eina_List *l1, *l2;
   Eina_List *error_keys = NULL;
   int i;

   Ecore_Wl_Keygrab_Info *info;
   Ecore_Wl_Window_Keygrab_Info *grab_info;

   LOGFN(__FILE__, __LINE__, __FUNCTION__);

   if (!_ecore_wl_disp) goto err;
   while (!_ecore_wl_disp->wl.keyrouter)
     {
        INF("Wait until keyrouter interface is ready");
        wl_display_roundtrip(_ecore_wl_disp->wl.display);
     }

   while (!_ecore_wl_disp->input)
     {
        INF("Wait wl_registry_add_listener reply");
        wl_display_roundtrip(_ecore_wl_disp->wl.display);
     }

   while (!_ecore_wl_disp->input->caps_update)
     {
        INF("Wait until wl_seat_capabilities_update is ready");
        wl_display_roundtrip(_ecore_wl_disp->wl.display);
     }
   if (_ecore_wl_disp->input->keyboard)
     {
        while(!_ecore_wl_disp->input->xkb.keymap)
          {
             wl_display_roundtrip(_ecore_wl_disp->wl.display);
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
     surface = ecore_wl_window_surface_get(win);

   wl_array_init(&grab_list);

   EINA_LIST_FOREACH_SAFE(infos, l1, l2, grab_info)
     {
        if (!grab_info->key) continue;
        if ((grab_info->mode < ECORE_WL_WINDOW_KEYGRAB_UNKNOWN) || (grab_info->mode > ECORE_WL_WINDOW_KEYGRAB_EXCLUSIVE))
          continue;

        keysym = xkb_keysym_from_name(grab_info->key, XKB_KEYSYM_NO_FLAGS);

        if (keysym == XKB_KEYSYM_NO_FLAGS)
          {
             WRN("Keysym of key(\"%s\") doesn't exist", grab_info->key);
             continue;
          }
        num_keycodes = ecore_wl_keycode_from_keysym(_ecore_wl_disp->input->xkb.keymap, keysym, &keycodes);

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
   tizen_keyrouter_set_keygrab_list(_ecore_wl_disp->wl.keyrouter, surface, &grab_list);

   ecore_wl_sync();

   wl_array_for_each(info, &_ecore_wl_keygrab_result_list)
     {

        if (!info->err)
          {
             INF("[PID:%d]Succeed to get return value !", getpid());
             if (_ecore_wl_keygrab_hash_add(&info->key, surface))
               INF("Succeed to add key to the keygrab hash!");
             else
               WRN("Failed to add key to the keygrab hash!");
          }
        else
          {
             WRN("After keygrab keycode %d error = %d", info->key, info->err);
             error_keys = eina_list_append(error_keys, _ecore_wl_keyname_get(info->key));
          }
     }
   wl_array_release(&grab_list);
   wl_array_release(&_ecore_wl_keygrab_result_list);

   return error_keys;

err:
   EINA_LIST_FOREACH_SAFE(infos, l1, l2, grab_info)
     {
        error_keys = eina_list_append(error_keys, strdup(grab_info->key));
     }
   return error_keys;
}

EAPI Eina_List
*ecore_wl_window_keygrab_list_unset(Ecore_Wl_Window *win, Eina_List *infos)
{
   xkb_keysym_t keysym = 0x0;
   int num_keycodes = 0;
   xkb_keycode_t *keycodes = NULL;

   struct wl_surface *surface = NULL;

   struct wl_array ungrab_list;
   int *grab_data = NULL;
   Eina_List *l1, *l2;
   Eina_List *error_keys = NULL;
   int i;

   Ecore_Wl_Keyungrab_Info *info;
   Ecore_Wl_Window_Keygrab_Info *grab_info;

   LOGFN(__FILE__, __LINE__, __FUNCTION__);

   if ((!_ecore_wl_disp) || (!_ecore_wl_disp->wl.keyrouter)) goto err;

   if ((!_ecore_wl_disp->input) || (!_ecore_wl_disp->input->xkb.keymap))
     {
        ERR("Keymap is not ready");
        goto err;
     }

   if (win)
     surface = ecore_wl_window_surface_get(win);

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
        num_keycodes = ecore_wl_keycode_from_keysym(_ecore_wl_disp->input->xkb.keymap, keysym, &keycodes);

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
   tizen_keyrouter_unset_keygrab_list(_ecore_wl_disp->wl.keyrouter, surface, &ungrab_list);

   ecore_wl_sync();

   wl_array_for_each(info, &_ecore_wl_keygrab_result_list)
     {
        if (!info->err)
          {
             INF("[PID:%d]Succeed to get return value !", getpid());
             if (_ecore_wl_keygrab_hash_del(&info->key))
               INF("Succeed to delete key to the keygrab hash!");
             else
               WRN("Failed to delete key to the keygrab hash!");
          }
        else
          {
             WRN("After keyungrab keycode %d error = %d", info->key, info->err);
             error_keys = eina_list_append(error_keys, _ecore_wl_keyname_get(info->key));
          }
     }
   wl_array_release(&ungrab_list);
   wl_array_release(&_ecore_wl_keygrab_result_list);

   return error_keys;

err:
   EINA_LIST_FOREACH_SAFE(infos, l1, l2, grab_info)
     {
        error_keys = eina_list_append(error_keys, strdup(grab_info->key));
     }
   return error_keys;
}
//

EAPI int
ecore_wl_window_active_angle_get(Ecore_Wl_Window *win)
{
   struct wl_surface *surface = NULL;
   LOGFN(__FILE__, __LINE__, __FUNCTION__);

   if (!_ecore_wl_disp->wl.tz_policy_ext) return 0;

   if (win)
     surface = ecore_wl_window_surface_get(win);

   tizen_policy_ext_get_active_angle(_ecore_wl_disp->wl.tz_policy_ext, surface);
   ecore_wl_sync();

   return _ecore_wl_active_angle;
}
//

static void
_ecore_wl_window_conformant_area_send(Ecore_Wl_Window *win, uint32_t conformant_part, uint32_t state)
{
   Ecore_Wl_Event_Conformant_Change *ev;

   LOGFN(__FILE__, __LINE__, __FUNCTION__);

   if (!(ev = calloc(1, sizeof(Ecore_Wl_Event_Conformant_Change)))) return;
   ev->win = win->id;
   ev->part_type = conformant_part;
   ev->state = state;
   ecore_event_add(ECORE_WL_EVENT_CONFORMANT_CHANGE, ev, NULL, NULL);
}

static void
_ecore_wl_cb_conformant(void *data EINA_UNUSED, struct tizen_policy *tizen_policy EINA_UNUSED, struct wl_surface *surface_resource, uint32_t is_conformant)
{
   struct wl_surface *surface = surface_resource;
   Ecore_Wl_Window *win = NULL;

   if (!surface) return;
   win = ecore_wl_window_surface_find(surface);
   if (win)
     win->conformant = is_conformant;
}

static void
_ecore_wl_cb_conformant_area(void *data EINA_UNUSED, struct tizen_policy *tizen_policy EINA_UNUSED, struct wl_surface *surface_resource, uint32_t conformant_part, uint32_t state, int32_t x, int32_t y, int32_t w, int32_t h)
{
   struct wl_surface *surface = surface_resource;
   Ecore_Wl_Window *win = NULL;
   int org_x, org_y, org_w, org_h;
   Eina_Bool changed = EINA_FALSE;
   Ecore_Wl_Indicator_State ind_state;
   Ecore_Wl_Virtual_Keyboard_State kbd_state;
   Ecore_Wl_Clipboard_State clip_state;

   if (!surface) return;
   win = ecore_wl_window_surface_find(surface);
   if (!win) return;

   if (conformant_part == TIZEN_POLICY_CONFORMANT_PART_INDICATOR)
     {
        ecore_wl_window_indicator_geometry_get(win, &org_x, &org_y, &org_w, &org_h);
        if ((org_x != x) || (org_y != y) || (org_w != w) || (org_h != h))
          {
             ecore_wl_window_indicator_geometry_set(win, x, y, w, h);
             changed = EINA_TRUE;
          }

        /* The given state is based on the visibility value of indicator.
         * Thus we need to add 1 to it before comparing with indicator state.
         */
        ind_state =  ecore_wl_window_indicator_state_get(win);
        if ((state + 1) != ind_state)
          {
             ecore_wl_window_indicator_state_set(win, state + 1);
             changed = EINA_TRUE;
          }
     }
   else if (conformant_part == TIZEN_POLICY_CONFORMANT_PART_KEYBOARD)
     {
        ecore_wl_window_keyboard_geometry_get(win, &org_x, &org_y, &org_w, &org_h);
        if ((org_x != x) || (org_y != y) || (org_w != w) || (org_h != h))
          {
             ecore_wl_window_keyboard_geometry_set(win, x, y, w, h);
             changed = EINA_TRUE;
          }

        /* The given state is based on the visibility value of virtual keyboard window.
         * Thus we need to add 1 to it before comparing with keyboard state.
         */
        kbd_state = ecore_wl_window_keyboard_state_get(win);
        if ((state + 1) != (kbd_state))
          {
             ecore_wl_window_keyboard_state_set(win, state + 1);
             changed = EINA_TRUE;
          }
     }
   else if (conformant_part == TIZEN_POLICY_CONFORMANT_PART_CLIPBOARD)
     {
        ecore_wl_window_clipboard_geometry_get(win, &org_x, &org_y, &org_w, &org_h);
        if ((org_x != x) || (org_y != y) || (org_w != w) || (org_h != h))
          {
             ecore_wl_window_clipboard_geometry_set(win, x, y, w, h);
             changed = EINA_TRUE;
          }

        /* The given state is based on the visibility value of clipboard window.
         * Thus we need to add 1 to it before comparing with clipboard state.
         */
        clip_state = ecore_wl_window_clipboard_state_get(win);
        if ((state + 1) != clip_state)
          {
             ecore_wl_window_clipboard_state_set(win, state + 1);
             changed = EINA_TRUE;
          }
     }

   if (changed)
     _ecore_wl_window_conformant_area_send(win, conformant_part, state);
}

static void
_ecore_wl_cb_notification_done(void *data EINA_UNUSED, struct tizen_policy *tizen_policy EINA_UNUSED, struct wl_surface *surface EINA_UNUSED, int32_t level EINA_UNUSED, uint32_t state EINA_UNUSED)
{
}

static void
_ecore_wl_cb_transient_for_done(void *data EINA_UNUSED, struct tizen_policy *tizen_policy EINA_UNUSED, uint32_t child_id EINA_UNUSED)
{
}

static void
_ecore_wl_cb_scr_mode_done(void *data EINA_UNUSED, struct tizen_policy *tizen_policy EINA_UNUSED, struct wl_surface *surface EINA_UNUSED, uint32_t mode EINA_UNUSED, uint32_t state EINA_UNUSED)
{
}

static void
_ecore_wl_cb_iconify_state_changed(void *data EINA_UNUSED, struct tizen_policy *tizen_policy EINA_UNUSED, struct wl_surface *surface_resource, uint32_t iconified, uint32_t force)
{
   struct wl_surface *surface = surface_resource;
   Ecore_Wl_Window *win = NULL;
   Ecore_Wl_Event_Window_Iconify_State_Change *ev;

   LOGFN(__FILE__, __LINE__, __FUNCTION__);

   if (!surface) return;
   win = ecore_wl_window_surface_find(surface);
   if (!win) return;

   if (!(ev = calloc(1, sizeof(Ecore_Wl_Event_Window_Iconify_State_Change)))) return;
   ev->win = win->id;
   ev->iconified = iconified;
   ev->force = force;

   ecore_event_add(ECORE_WL_EVENT_WINDOW_ICONIFY_STATE_CHANGE, ev, NULL, NULL);
}

static void
_ecore_wl_cb_supported_aux_hints(void *data EINA_UNUSED, struct tizen_policy *tizen_policy EINA_UNUSED, struct wl_surface *surface_resource, struct wl_array *hints, uint32_t num_hints)
{
   struct wl_surface *surface = surface_resource;
   Ecore_Wl_Window *win = NULL;
   char *p = NULL;
   char **str = NULL;
   const char *hint = NULL;
   unsigned int i = 0;

   LOGFN(__FILE__, __LINE__, __FUNCTION__);

   if (!surface) return;
   win = ecore_wl_window_surface_find(surface);
   if (!win) return;

   p = hints->data;
   str = calloc(num_hints, sizeof(char *));
   if (!str) return;

   _ecore_wl_window_aux_hint_free(win);

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
_ecore_wl_cb_allowed_aux_hint(void *data  EINA_UNUSED, struct tizen_policy *tizen_policy  EINA_UNUSED, struct wl_surface *surface_resource, int id)
{
   struct wl_surface *surface = surface_resource;
   Ecore_Wl_Window *win = NULL;
   Ecore_Wl_Event_Aux_Hint_Allowed *ev;

   LOGFN(__FILE__, __LINE__, __FUNCTION__);

   if (!surface) return;
   win = ecore_wl_window_surface_find(surface);
   if (!win) return;

   if (!(ev = calloc(1, sizeof(Ecore_Wl_Event_Aux_Hint_Allowed)))) return;
   ev->win = win->id;
   ev->id = id;
   ecore_event_add(ECORE_WL_EVENT_AUX_HINT_ALLOWED, ev, NULL, NULL);
}

static void
_ecore_wl_cb_aux_message(void *data EINA_UNUSED, struct tizen_policy *tizen_policy EINA_UNUSED, struct wl_surface *surface_resource, const char *key, const char *val, struct wl_array *options)
{
   Ecore_Wl_Window *win = NULL;
   Ecore_Wl_Event_Aux_Message *ev;
   char *p = NULL, *str = NULL;
   Eina_List *opt_list = NULL;

   LOGFN(__FILE__, __LINE__, __FUNCTION__);

   if (!surface_resource) return;
   win = ecore_wl_window_surface_find(surface_resource);
   if (!win) return;

   if (!(ev = calloc(1, sizeof(Ecore_Wl_Event_Aux_Message)))) return;

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

   ecore_event_add(ECORE_WL_EVENT_AUX_MESSAGE, ev, _cb_aux_message_free, NULL);
}

static void
_ecore_wl_cb_active_angle(void *data EINA_UNUSED, struct tizen_policy_ext *tizen_policy_ext EINA_UNUSED, uint32_t angle)
{
   LOGFN(__FILE__, __LINE__, __FUNCTION__);
   _ecore_wl_active_angle = angle;
}

static void
_ecore_wl_cb_effect_start(void *data EINA_UNUSED, struct tizen_effect *tizen_effect EINA_UNUSED, struct wl_surface *surface_resource, unsigned int type)
{
   struct wl_surface *surface = surface_resource;
   Ecore_Wl_Window *win = NULL;
   Ecore_Wl_Event_Effect_Start *ev;

   LOGFN(__FILE__, __LINE__, __FUNCTION__);

   if (!surface) return;
   win = ecore_wl_window_surface_find(surface);
   if (!win) return;

   if (!(ev = calloc(1, sizeof(Ecore_Wl_Event_Effect_Start)))) return;
   ev->win = win->id;
   ev->type = type;
   ecore_event_add(ECORE_WL_EVENT_EFFECT_START, ev, NULL, NULL);
}

static void
_ecore_wl_cb_effect_end(void *data EINA_UNUSED, struct tizen_effect *tizen_effect EINA_UNUSED, struct wl_surface *surface_resource, unsigned int type)
{
   struct wl_surface *surface = surface_resource;
   Ecore_Wl_Window *win = NULL;
   Ecore_Wl_Event_Effect_End *ev;

   LOGFN(__FILE__, __LINE__, __FUNCTION__);

   if (!surface) return;
   win = ecore_wl_window_surface_find(surface);
   if (!win) return;

   if (!(ev = calloc(1, sizeof(Ecore_Wl_Event_Effect_End)))) return;
   ev->win = win->id;
   ev->type = type;
   ecore_event_add(ECORE_WL_EVENT_EFFECT_END, ev, NULL, NULL);
}

static void
_ecore_wl_cb_indicator_flick(void *data EINA_UNUSED, struct tizen_indicator *tizen_indicator EINA_UNUSED, struct wl_surface *surface_resource, int type)
{
   struct wl_surface *surface = surface_resource;
   Ecore_Wl_Window *win = NULL;
   Ecore_Wl_Event_Indicator_Flick *ev;

   LOGFN(__FILE__, __LINE__, __FUNCTION__);

   if (!surface) return;
   win = ecore_wl_window_surface_find(surface);
   if (!win) return;

   if (!(ev = calloc(1, sizeof(Ecore_Wl_Event_Indicator_Flick)))) return;
   ev->win = win->id;
   ev->type = type;

   ecore_event_add(ECORE_WL_EVENT_INDICATOR_FLICK, ev, NULL, NULL);
}

EAPI Eina_Bool
ecore_wl_clipboard_data_only_set(Eina_Bool data_only)
{
   LOGFN(__FILE__, __LINE__, __FUNCTION__);
   if (!_ecore_wl_disp) return EINA_FALSE;
   if (!_ecore_wl_disp->wl.tz_clipboard) return EINA_FALSE;
   if (!_ecore_wl_disp->input) return EINA_FALSE;

   tizen_clipboard_set_data_only(_ecore_wl_disp->wl.tz_clipboard, data_only);

   ecore_wl_sync();

   return _ecore_wl_disp->input->is_data_only;
}

static void
_ecore_wl_cb_clipboard_data_selected(void *data EINA_UNUSED, struct tizen_clipboard *tizen_clipboard EINA_UNUSED, struct wl_surface *surface)
{
   Ecore_Wl_Window *win = NULL;
   Ecore_Wl_Event_Clipboard_Data_Selected *ev;

   if (!surface) return;
   win = ecore_wl_window_surface_find(surface);
   if (!win) return;

   if (!(ev = calloc(1, sizeof(Ecore_Wl_Event_Clipboard_Data_Selected)))) return;
   ev->win = win->id;

   ecore_event_add(ECORE_WL_EVENT_CLIPBOARD_DATA_SELECTED, ev, NULL, NULL);
}

static void
_ecore_wl_cb_clipboard_data_only_allowed(void *data EINA_UNUSED, struct tizen_clipboard *tizen_clipboard EINA_UNUSED, uint32_t allowed)
{
   LOGFN(__FILE__, __LINE__, __FUNCTION__);
   if (!_ecore_wl_disp->input) return;

   if (allowed)
     _ecore_wl_disp->input->is_data_only = EINA_TRUE;
   else
     _ecore_wl_disp->input->is_data_only = EINA_FALSE;
}

static void
_ecore_wl_log_cb_print(const char *format, va_list args)
{
   char buffer[1024];
   vsnprintf(buffer, 1024, format, args);
   ERR("_ecore_wl_log_cb_print: %s", buffer);
}
