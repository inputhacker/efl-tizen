#ifndef _ECORE_WL2_PRIVATE_H
# define _ECORE_WL2_PRIVATE_H

# include <unistd.h>
# include "Ecore_Wl2.h"
# include "Ecore_Input.h"
# include <xkbcommon/xkbcommon-compose.h>
# include "www-client-protocol.h"

# define EFL_TEAMWORK_VERSION 2
# include "teamwork-client-protocol.h"

# include "session-recovery-client-protocol.h"

# include "xdg-shell-unstable-v6-client-protocol.h"
# include "efl-aux-hints-client-protocol.h"

// TIZEN_ONLY : To use tizen protocols
# include <wayland-extension/tizen-extension-client-protocol.h>
# include "tizen-policy-ext-client-protocol.h"
//

extern int _ecore_wl2_log_dom;
extern Eina_Bool no_session_recovery;

# ifdef EAPI
#  undef EAPI
# endif

# ifdef __GNUC__
#  if __GNUC__ >= 4
#   define EAPI __attribute__ ((visibility("default")))
#  else
#   define EAPI
#  endif
# else
#  define EAPI
# endif

# ifdef ECORE_WL2_DEFAULT_LOG_COLOR
#  undef ECORE_WL2_DEFAULT_LOG_COLOR
# endif
# define ECORE_WL2_DEFAULT_LOG_COLOR EINA_COLOR_BLUE

# ifdef ERR
#  undef ERR
# endif
# define ERR(...) EINA_LOG_DOM_ERR(_ecore_wl2_log_dom, __VA_ARGS__)

# ifdef DBG
#  undef DBG
# endif
# define DBG(...) EINA_LOG_DOM_DBG(_ecore_wl2_log_dom, __VA_ARGS__)

# ifdef INF
#  undef INF
# endif
# define INF(...) EINA_LOG_DOM_INFO(_ecore_wl2_log_dom, __VA_ARGS__)

# ifdef WRN
#  undef WRN
# endif
# define WRN(...) EINA_LOG_DOM_WARN(_ecore_wl2_log_dom, __VA_ARGS__)

# ifdef CRI
#  undef CRI
# endif
# define CRI(...) EINA_LOG_DOM_CRIT(_ecore_wl2_log_dom, __VA_ARGS__)

typedef struct _Ecore_Wl2_Input_Devices
{
   Eo *pointer_dev;
   Eo *keyboard_dev;
   Eo *touch_dev;
   Eo *seat_dev;
   int window_id;
} Ecore_Wl2_Input_Devices;

struct _Ecore_Wl2_Display
{
   int refs;
   char *name;
   pid_t pid;

   struct
     {
        struct wl_display *display;
        struct wl_registry *registry;
        struct wl_compositor *compositor;
        struct wl_subcompositor *subcompositor;
        struct wl_data_device_manager *data_device_manager;
        int data_device_manager_version;
        struct wl_shm *shm;
        struct zwp_linux_dmabuf_v1 *dmabuf;
        struct zxdg_shell_v6 *zxdg_shell;
        struct www *www;
        struct zwp_e_session_recovery *session_recovery;
        struct efl_aux_hints *efl_aux_hints;
        struct zwp_teamwork *teamwork;
        struct efl_hints *efl_hints;
        // TIZEN_ONLY : To use tizen protocols
        struct tizen_policy *tz_policy;
        struct tizen_policy_ext *tz_policy_ext;
        struct tizen_surface *tz_surf;
        struct tizen_effect *tz_effect;
        struct tizen_indicator *tz_indicator;
        struct tizen_clipboard *tz_clipboard;
        struct tizen_keyrouter *tz_keyrouter;
        struct tizen_input_device_manager *tz_input_device_manager;
        //
        //TIZEN_ONLY(20171115): support output transform
        struct tizen_screen_rotation *tz_screen_rotation;
        //

        int compositor_version;
     } wl;

   uint32_t serial;

   struct xkb_context *xkb_context;

   Ecore_Idle_Enterer *idle_enterer;
   Ecore_Fd_Handler *fd_hdl;

   Eina_Hash *globals;
   Ecore_Timer *recovery_timer;

   Eina_Inlist *windows;
   Eina_Inlist *outputs;
   Eina_Inlist *inputs;
   Eina_Inlist *seats;

   int syncs;

   Eina_Bool sync_done : 1;
   Eina_Bool shell_done : 1;
   Eina_Bool recovering : 1;

// TIZEN_ONLY(20171107): support a tizen_keyrouter interface
   int sync_ref_count;
//
// TIZEN_ONLY(20171114): active angle
   int active_angle;
//
// TIZEN_ONLY(20171129): thread-safety for wl
   Eina_Bool prepare_read;
// End of TIZEN_ONLY(20171129)
};

struct _Ecore_Wl2_Subsurface
{
   EINA_INLIST;

   int x, y;

   Ecore_Wl2_Window *parent;

   struct
     {
        struct wl_surface *surface;
        struct wl_subsurface *subsurface;
     } wl;

   Eina_Bool sync : 1;
};

struct _Ecore_Wl2_Aux_Hint
{
   EINA_INLIST;

   int id;
   const char *hint, *val;
};

struct _Ecore_Wl2_Frame_Cb_Handle
{
   Ecore_Wl2_Window *win;
   Ecore_Wl2_Frame_Cb cb;
   void *data;
};

typedef struct _Ecore_Wl2_Window_Configure_State
{
   uint32_t serial;
   Eina_Rectangle geometry;
   Eina_Bool minimized : 1;
   Eina_Bool maximized : 1;
   Eina_Bool fullscreen : 1;
   Eina_Bool focused : 1;
   Eina_Bool resizing : 1;
} Ecore_Wl2_Window_Configure_State;

struct _Ecore_Wl2_Window
{
   EINA_INLIST;

   Ecore_Wl2_Display *display;

   Ecore_Wl2_Window *parent;
   Ecore_Wl2_Input *grab;

   Ecore_Wl2_Surface *wl2_surface;

   int id, rotation, surface_id;
   const char *title;
   const char *class;
   const char *role;

   struct wl_surface *surface;
   void *buffer;
   struct wl_callback *callback;
   struct www_surface *www_surface;
   struct zxdg_surface_v6 *zxdg_surface;
   struct zxdg_toplevel_v6 *zxdg_toplevel;
   struct zxdg_popup_v6 *zxdg_popup;

   // TIZEN_ONLY(20150424) : tizen_visibility
   struct tizen_visibility *tz_visibility;
   struct tizen_position *tz_position;
   struct tizen_rotation *tz_rotation;
   struct tizen_resource *tz_resource;
   unsigned int resource_id;
   //

   Eina_Stringshare *uuid;

   void (*zxdg_configure_ack)(struct zxdg_surface_v6 *surface, uint32_t serial);
   void (*zxdg_set_min_size)(struct zxdg_toplevel_v6 *toplevel, int32_t w, int32_t h);
   void (*zxdg_set_max_size)(struct zxdg_toplevel_v6 *toplevel, int32_t w, int32_t h);

   Eina_Rectangle saved;
   Eina_Rectangle opaque;
   Eina_Rectangle input_rect;
   // TIZEN_ONLY(20160201) : support to handle input rectangle
   struct wl_region *input_region;
   //

   // TIZEN_ONLY(20160323)
   struct
     {
        int x, y; /* it indicates a position configured by window manager. */
        int w, h;
        unsigned int edges;
     } configured;
   //

   Ecore_Wl2_Window_Type type;

   struct
   {
      int w, h;
      unsigned int aspect;
      Eina_Bool set : 1;
   } aspect;

   struct
   {
      int w, h;
      Eina_Bool set : 1;
   } weight;

   Eina_Inlist *subsurfs;
   Eina_List *supported_aux_hints;
   Eina_List *frame_callbacks;

   Ecore_Wl2_Window_Configure_State set_config;
   Ecore_Wl2_Window_Configure_State req_config;
   Ecore_Wl2_Window_Configure_State def_config;

   //TIZEN_ONLY(20171115): support output transform
   Eina_Bool ignore_output_transform : 1;
   //
   Eina_Bool moving : 1;
   //TODO: move the iconified into the Ecore_Wl2_Window_Configure_State structure.
   // TIZEN_ONLY(20150822)
   Eina_Bool iconified : 1;
   //
   Eina_Bool alpha : 1;
   Eina_Bool transparent : 1;

   Eina_Bool input_set : 1;
   Eina_Bool opaque_set : 1;

   Eina_Bool focus_skip : 1;
   Eina_Bool floating : 1;

   // TIZEN_ONLY(20150703) : support conformant
   Eina_Bool conformant : 1;
   //

   Eina_Bool commit_pending : 1;

   struct
     {
        Eina_Bool configure : 1;
        Eina_Bool min : 1;
        Eina_Bool max : 1;
        Eina_Bool geom : 1;
        Eina_Bool opaque : 1;
        Eina_Bool input : 1;
        Eina_Bool fullscreen : 1;
        Eina_Bool maximized : 1;
     } pending;

   struct
     {
        Eina_Bool supported : 1;
        Eina_Bool app_set : 1;
        int preferred_rot;
        int *available_rots;
        unsigned int count;
        // TIZEN_ONLY
        unsigned int serial;
        struct
          {
             int x, y, w, h;
             Eina_Bool valid : 1;
          } geometry_hints[4];
        //
     } wm_rot;
   Eina_Bool has_buffer : 1;
   Eina_Bool updating : 1;

   // TIZEN_ONLY(20171108) : indicator information
   struct
     {
        int x, y, w, h;
        Ecore_Wl2_Indicator_State state;
        Ecore_Wl2_Indicator_Opacity_Mode mode;
        Ecore_Wl2_Indicator_Visible_Type type;
     } indicator;
   //

   // TIZEN_ONLY(20171108) : clipboard information
   struct
     {
        int x, y, w, h;
        Ecore_Wl2_Clipboard_State state;
     } clipboard;
   //

   // TIZEN_ONLY(20171108) : keyboard information
   struct
     {
        int x, y, w, h;
        Ecore_Wl2_Virtual_Keyboard_State state;
     } keyboard;
   //

   // TIZEN_ONLY(20161228) : tizen_rotation v2
   void (*cb_rot_changed)(Ecore_Wl2_Window *win, int rot, Eina_Bool resize, int w, int h, void *data);
   void *cb_rot_changed_data;
   //

   // TIZEN_ONLY(20171207): add functions to set client's custom cursors
   struct
     {
        struct wl_surface *surface;
        int hot_x, hot_y;
        Eina_Bool set : 1;
        Ecore_Wl2_Input *device;
        const char *cursor_name;
     } pointer;
   //
};

struct _Ecore_Wl2_Output
{
   EINA_INLIST;

   Ecore_Wl2_Display *display;
   struct wl_output *wl_output;

   int mw, mh, transform;
   const char *make, *model;
   Eina_Rectangle geometry;
};

typedef struct _Ecore_Wl2_Dnd_Source
{
   Ecore_Wl2_Input *input;

   int x, y;
   Ecore_Fd_Handler *fdh;

   struct wl_data_offer *offer;
   struct wl_array types;
   uint32_t dnd_action;
   uint32_t source_actions;
   Ecore_Wl2_Selection_Type sel_type;
   Eina_Bool active_read;
   void *read_data;
   unsigned int len;
} Ecore_Wl2_Dnd_Source;

// TIZEN_ONLY(20171109): support a tizen_input_device_manager interface
struct _Ecore_Wl2_Tizen_Input_Device
{
   struct tizen_input_device *tz_device;
   Ecore_Wl2_Input *input;
   const char *name;
   const char *identifier;
   Efl_Input_Device_Type clas;
   Efl_Input_Device_Subtype subclas;
   struct wl_seat *seat;
   Eo *eo_dev;
};

typedef struct _Ecore_Wl2_Touch_Axis
{
   double radius_x;
   double radius_y;
   double pressure;
   double angle;
} Ecore_Wl2_Touch_Axis;

#define ECORE_WL2_TOUCH_MAX 10
//

/** TODO: Refactor ALL Input code :(
 *
 * wl_seat is a GROUP of Input Devices (such as):
 *      keyboards, pointers, touch devices
 */
struct _Ecore_Wl2_Pointer
{
   EINA_INLIST;

   Ecore_Wl2_Seat *seat;

   double sx, sy;
   unsigned int button;
   unsigned int enter_serial;

   struct
     {
        unsigned int button, count, timestamp;
        Ecore_Wl2_Window *window;
     } grab;

   Ecore_Wl2_Window *focus;

   Eina_List *resources;
};

struct _Ecore_Wl2_Keyboard
{
   EINA_INLIST;

   Ecore_Wl2_Seat *seat;

   unsigned int modifiers;

   struct
     {
        struct xkb_keymap *keymap;
        struct xkb_state *state;
        xkb_mod_mask_t control_mask;
        xkb_mod_mask_t alt_mask;
        xkb_mod_mask_t shift_mask;
        xkb_mod_mask_t win_mask;
        xkb_mod_mask_t scroll_mask;
        xkb_mod_mask_t num_mask;
        xkb_mod_mask_t caps_mask;
        xkb_mod_mask_t altgr_mask;
        unsigned int mods_depressed;
        unsigned int mods_latched;
        unsigned int mods_locked;
        unsigned int mods_group;
     } xkb;

   struct
     {
        Ecore_Timer *tmr;
        unsigned int sym, key, time;
        double rate, delay;
        Eina_Bool enabled : 1;
     } repeat;

   struct
     {
        unsigned int button, count, timestamp;
        Ecore_Wl2_Window *window;
     } grab;

   Ecore_Wl2_Window *focus;

   Eina_List *resources;
};

struct _Ecore_Wl2_Touch
{
   EINA_INLIST;

   struct
     {
        unsigned int button, count, timestamp;
        Ecore_Wl2_Window *window;
     } grab;
};

struct _Ecore_Wl2_Seat
{
   EINA_INLIST;

   uint32_t id;
   uint32_t version;
   const char *name;
   struct wl_global *global;
   const struct wl_seat_interface *implementation;

   struct
     {
        struct wl_global *global;
        struct wl_resource *resource;
     } im;

   Ecore_Wl2_Bind_Cb bind_cb;
   Ecore_Wl2_Unbind_Cb unbind_cb;

   Ecore_Wl2_Pointer *pointer;
   int pointer_count;

   Ecore_Wl2_Keyboard *keyboard;
   int keyboard_count;

   Ecore_Wl2_Touch *touch;
   int touch_count;

   Eina_List *resources;
};

struct _Ecore_Wl2_Input
{
   EINA_INLIST;

   Ecore_Wl2_Display *display;

   unsigned int timestamp;

   struct
     {
        struct wl_seat *seat;
        struct wl_pointer *pointer;
        struct wl_keyboard *keyboard;
        struct wl_touch *touch;
     } wl;

   struct
     {
        struct wl_data_device *device;
        struct
        {
           struct wl_data_source *source;
           struct wl_array types;
           uint32_t serial;
        } selection;
        struct
        {
           struct wl_data_source *source;
           struct wl_array types;
           uint32_t action;
           uint32_t serial;
        } drag;
     } data;

   struct
     {
        const char *name;
        unsigned int size;
        struct wl_surface *surface;
        int hot_x, hot_y;
        // TIZEN_ONLY(20171207): add functions to set client's custom cursors
        struct wl_cursor *cursor;
        struct wl_callback *frame_cb;
        Ecore_Timer *timer;
        unsigned int current_index;
        const char *theme_name;
        struct wl_cursor_theme *theme;
        //
     } cursor;

   struct
     {
        double sx, sy;
        unsigned int button;
        unsigned int enter_serial;
     } pointer;

   struct
     {
        unsigned int modifiers;
     } keyboard;

   struct
     {
        Ecore_Wl2_Window *pointer;
        Ecore_Wl2_Window *prev_pointer;
        Ecore_Wl2_Window *keyboard;
        Ecore_Wl2_Window *touch;
     } focus;

   struct
     {
        unsigned int button, count, timestamp;
        Ecore_Wl2_Window *window;
     } grab;

   struct
     {
        struct xkb_keymap *keymap;
        struct xkb_state *state;
        struct xkb_state *maskless_state;
        struct xkb_compose_table *compose_table;
        struct xkb_compose_state *compose_state;
        xkb_mod_mask_t control_mask;
        xkb_mod_mask_t alt_mask;
        xkb_mod_mask_t shift_mask;
        xkb_mod_mask_t win_mask;
        xkb_mod_mask_t scroll_mask;
        xkb_mod_mask_t num_mask;
        xkb_mod_mask_t caps_mask;
        xkb_mod_mask_t altgr_mask;
        unsigned int mods_depressed;
        unsigned int mods_latched;
        unsigned int mods_locked;
        unsigned int mods_group;
     } xkb;

   struct
     {
        Ecore_Timer *timer;
        unsigned int sym, sym_name, key, time;
        double rate, delay;
        Eina_Bool enabled : 1;
        Eina_Bool repeating : 1;
     } repeat;

   struct
   {
      Ecore_Wl2_Offer *offer;
      uint32_t enter_serial;
      Ecore_Window window_id;
   } drag, selection;
// TIZEN_ONLY(20170227) : tizen clipboard v2
   Eina_Bool is_data_only;
//

   unsigned int seat_version;
   unsigned int id;
   Eina_Stringshare *name;

   Ecore_Event_Handler *dev_add_handler;
   Ecore_Event_Handler *dev_remove_handler;
   Eina_List *devices_list;

// TIZEN_ONLY(20171109): support a tizen_input_device_manager interface
   Ecore_Event_Handler *tzdev_add_handler;
   Ecore_Event_Handler *tzdev_remove_handler;
   struct
     {
        Eina_List *devices;
        Ecore_Wl2_Tizen_Input_Device *last_device_ptr;
        Ecore_Wl2_Tizen_Input_Device *last_device_touch;
        Ecore_Wl2_Tizen_Input_Device *last_device_kbd;
     } devmgr;
   struct
     {
        Ecore_Wl2_Touch_Axis touch_axis[ECORE_WL2_TOUCH_MAX];
        Ecore_Wl2_Touch_Axis last_touch_axis;
     } touch;
//

// TIZEN_ONLY(20171107): support a tizen_keyrouter interface
   Eina_Bool caps_update;
//
};

typedef struct Ecore_Wl2_Event_Window_WWW
{
   unsigned int window;
   int x_rel;
   int y_rel;
   uint32_t timestamp;
} Ecore_Wl2_Event_Window_WWW;

typedef struct Ecore_Wl2_Event_Window_WWW_Drag
{
   unsigned int window;
   Eina_Bool dragging;
} Ecore_Wl2_Event_Window_WWW_Drag;

typedef struct _Buffer_Handle Buffer_Handle;
typedef struct _Ecore_Wl2_Buffer
{
   struct wl_buffer *wl_buffer;
   int size;
   int w, h;
   int age;
   unsigned long stride;
   Buffer_Handle *bh;
   int fd;
   void *mapping;

   int index;
   Eina_Bool locked : 1;
   Eina_Bool busy : 1;
   Eina_Bool orphaned : 1;
   Eina_Bool alpha : 1;
} Ecore_Wl2_Buffer;

typedef struct _Ecore_Wl2_Surface
{
   Ecore_Wl2_Window *wl2_win;
   Ecore_Wl2_Buffer *current;
   Eina_List *buffers;

   int w, h;
   Eina_Bool alpha : 1;
   struct
     {
        void (*destroy)(Ecore_Wl2_Surface *surface);
        void (*reconfigure)(Ecore_Wl2_Surface *surface, int w, int h, uint32_t flags, Eina_Bool force);
        void *(*data_get)(Ecore_Wl2_Surface *surface, int *w, int *h);
        int  (*assign)(Ecore_Wl2_Surface *surface);
        void (*post)(Ecore_Wl2_Surface *surface, Eina_Rectangle *rects, unsigned int count);
        void (*flush)(Ecore_Wl2_Surface *surface);
     } funcs;
} Ecore_Wl2_Surface;

Ecore_Wl2_Window *_ecore_wl2_display_window_surface_find(Ecore_Wl2_Display *display, struct wl_surface *wl_surface);
void _display_event_free(void *d, void *event EINA_UNUSED);

void _ecore_wl2_output_add(Ecore_Wl2_Display *display, unsigned int id);
void _ecore_wl2_output_del(Ecore_Wl2_Output *output);

void _ecore_wl2_input_add(Ecore_Wl2_Display *display, unsigned int id, unsigned int version);
void _ecore_wl2_input_del(Ecore_Wl2_Input *input);

void _ecore_wl2_input_ungrab(Ecore_Wl2_Input *input);
void _ecore_wl2_input_grab(Ecore_Wl2_Input *input, Ecore_Wl2_Window *window, unsigned int button);

void _ecore_wl2_input_cursor_set(Ecore_Wl2_Input *input, const char *cursor);
Eina_Bool _ecore_wl2_input_cursor_update(void *data);
void _ecore_wl2_input_window_remove(Ecore_Wl2_Input *input, Ecore_Wl2_Window *window);

void _ecore_wl2_dnd_add(Ecore_Wl2_Input *input, struct wl_data_offer *offer);
void _ecore_wl2_dnd_enter(Ecore_Wl2_Input *input, struct wl_data_offer *offer, struct wl_surface *surface, int x, int y, uint32_t serial);
void _ecore_wl2_dnd_leave(Ecore_Wl2_Input *input);
void _ecore_wl2_dnd_motion(Ecore_Wl2_Input *input, int x, int y, uint32_t serial);
void _ecore_wl2_dnd_drop(Ecore_Wl2_Input *input);
void _ecore_wl2_dnd_selection(Ecore_Wl2_Input *input, struct wl_data_offer *offer);
void _ecore_wl2_dnd_del(Ecore_Wl2_Dnd_Source *source);

void _ecore_wl2_subsurf_unmap(Ecore_Wl2_Subsurface *subsurf);
void _ecore_wl2_subsurf_free(Ecore_Wl2_Subsurface *subsurf);

void _ecore_wl2_window_surface_create(Ecore_Wl2_Window *window);
void _ecore_wl2_window_shell_surface_init(Ecore_Wl2_Window *window);
void _ecore_wl2_window_www_surface_init(Ecore_Wl2_Window *window);
void _ecore_wl2_window_semi_free(Ecore_Wl2_Window *window);

void _ecore_wl2_offer_unref(Ecore_Wl2_Offer *offer);
Eina_Bool _ecore_wl2_display_sync_get(void);

void _ecore_wl2_buffer_test(Ecore_Wl2_Display *ewd);

EAPI void ecore_wl2_window_weight_set(Ecore_Wl2_Window *window, double w, double h);

// TIZEN_ONLY(20171107): support a tizen_keyrouter interface
int ecore_wl2_input_keycode_from_keysym(struct xkb_keymap *keymap, xkb_keysym_t keysym, xkb_keycode_t **keycodes);
void _ecore_wl2_keyrouter_setup(Ecore_Wl2_Display *ewd, unsigned int id, unsigned int version);
Eina_Hash *_ecore_wl2_keygrab_hash_get(void);
//

// TIZEN_ONLY(20171109): support a tizen_input_device_manager interface
void _ecore_wl2_input_device_manager_setup(Ecore_Wl2_Display *ewd, unsigned int id, unsigned int version);
//

//TIZEN_ONLY(20171115): support output transform
void _ecore_wl2_window_ignore_output_transform_set(Ecore_Wl2_Window *window, Eina_Bool ignore);
//

//TIZEN_ONLY(20171216): add ecore_wl2_window_find
void _ecore_wl2_window_init(void);
void _ecore_wl2_window_shutdown(void);
//

EAPI extern int _ecore_wl2_event_window_www;
EAPI extern int _ecore_wl2_event_window_www_drag;

#endif
