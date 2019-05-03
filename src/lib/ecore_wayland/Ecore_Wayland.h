#ifndef _ECORE_WAYLAND_H_
# define _ECORE_WAYLAND_H_

/*
 * Wayland supoprt is considered experimental as wayland itself is still
 * unstable and liable to change core protocol. If you use this api, it is
 * possible it will break in future, until this notice is removed.
 */

# include <Eina.h>
# include <Ecore.h>
# include <wayland-client.h>
# include <wayland-cursor.h>
# include <xkbcommon/xkbcommon.h>

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

/* TIZEN_ONLY(20171205) : turn off wayland warning until tizen finishs wayland2 changing job
# warning The Ecore_Wayland library has been deprecated. Please use the Ecore_Wl2 library
*/

#ifdef __cplusplus
extern "C" {
#endif

typedef struct _Ecore_Wl_Output Ecore_Wl_Output;
typedef struct _Ecore_Wl_Input Ecore_Wl_Input;
typedef struct _Ecore_Wl_Global Ecore_Wl_Global; /** @since 1.7.6 */
typedef struct _Ecore_Wl_Subsurf Ecore_Wl_Subsurf; /** @since 1.8 */

# ifndef _ECORE_WAYLAND_WINDOW_PREDEF
typedef struct _Ecore_Wl_Window Ecore_Wl_Window;
# endif

// TIZEN_ONLY(20171109): merged with legacy ecore_wayland
typedef struct _Ecore_Wl_Input_Device Ecore_Wl_Input_Device;
// End of TIZEN_ONLY(20171109)
/**
 * @deprecated Do Not Use
 * @since 1.7
 */
typedef struct _Ecore_Wl_Dnd Ecore_Wl_Dnd;

typedef struct _Ecore_Wl_Dnd_Source Ecore_Wl_Dnd_Source;
typedef struct _Ecore_Wl_Dnd_Target Ecore_Wl_Dnd_Target;
// TIZEN_ONLY(20171109): merged with legacy ecore_wayland
typedef struct _Ecore_Wl_Aux_Hint Ecore_Wl_Aux_Hint;
// End of TIZEN_ONLY(20171109)

typedef struct _Ecore_Wl_Event_Mouse_In Ecore_Wl_Event_Mouse_In;
typedef struct _Ecore_Wl_Event_Mouse_Out Ecore_Wl_Event_Mouse_Out;
typedef struct _Ecore_Wl_Event_Focus_In Ecore_Wl_Event_Focus_In;
typedef struct _Ecore_Wl_Event_Focus_Out Ecore_Wl_Event_Focus_Out;
typedef struct _Ecore_Wl_Event_Window_Configure Ecore_Wl_Event_Window_Configure;
// TIZEN_ONLY(20171109): merged with legacy ecore_wayland
typedef struct _Ecore_Wl_Event_Window_Activate Ecore_Wl_Event_Window_Activate;
typedef struct _Ecore_Wl_Event_Window_Deactivate Ecore_Wl_Event_Window_Deactivate;
typedef struct _Ecore_Wl_Event_Window_Visibility_Change Ecore_Wl_Event_Window_Visibility_Change;
typedef struct _Ecore_Wl_Event_Window_Pre_Visibility_Change Ecore_Wl_Event_Window_Pre_Visibility_Change;
typedef struct _Ecore_Wl_Event_Window_Rotate Ecore_Wl_Event_Window_Rotate;
typedef struct _Ecore_Wl_Event_Window_Show Ecore_Wl_Event_Window_Show;
typedef struct _Ecore_Wl_Event_Window_Hide Ecore_Wl_Event_Window_Hide;
typedef struct _Ecore_Wl_Event_Window_Lower Ecore_Wl_Event_Window_Lower;
// End of TIZEN_ONLY(20171109)
typedef struct _Ecore_Wl_Event_Dnd_Enter Ecore_Wl_Event_Dnd_Enter;
// TIZEN_ONLY(20171109): merged with legacy ecore_wayland
typedef struct _Ecore_Wl_Event_Dnd_Selection Ecore_Wl_Event_Dnd_Selection;
// End of TIZEN_ONLY(20171109)
typedef struct _Ecore_Wl_Event_Dnd_Position Ecore_Wl_Event_Dnd_Position;
typedef struct _Ecore_Wl_Event_Dnd_Leave Ecore_Wl_Event_Dnd_Leave;
typedef struct _Ecore_Wl_Event_Dnd_Drop Ecore_Wl_Event_Dnd_Drop;
typedef struct _Ecore_Wl_Event_Dnd_End Ecore_Wl_Event_Dnd_End;
typedef struct _Ecore_Wl_Event_Data_Source_Cancelled Ecore_Wl_Event_Data_Source_Cancelled; /** @since 1.12 */
typedef struct _Ecore_Wl_Event_Data_Source_Send Ecore_Wl_Event_Data_Source_Send; /** @since 1.7 */
typedef struct _Ecore_Wl_Event_Data_Source_Target  Ecore_Wl_Event_Data_Source_Target; /** @since 1.12 */
typedef struct _Ecore_Wl_Event_Selection_Data_Ready Ecore_Wl_Event_Selection_Data_Ready; /** @since 1.7 */
typedef struct _Ecore_Wl_Event_Interfaces_Bound Ecore_Wl_Event_Interfaces_Bound;
// TIZEN_ONLY(20171109): merged with legacy ecore_wayland
typedef struct _Ecore_Wl_Event_Conformant_Change Ecore_Wl_Event_Conformant_Change;
typedef struct _Ecore_Wl_Event_Aux_Hint_Allowed Ecore_Wl_Event_Aux_Hint_Allowed;
typedef struct _Ecore_Wl_Event_Aux_Message Ecore_Wl_Event_Aux_Message;
typedef struct _Ecore_Wl_Event_Window_Iconify_State_Change Ecore_Wl_Event_Window_Iconify_State_Change;
typedef struct _Ecore_Wl_Event_Effect Ecore_Wl_Event_Effect_Start;
typedef struct _Ecore_Wl_Event_Effect Ecore_Wl_Event_Effect_End;
typedef struct _Ecore_Wl_Display Ecore_Wl_Display;
typedef struct _Ecore_Wl_Event_Output_Transform Ecore_Wl_Event_Output_Transform;
typedef struct _Ecore_Wl_Event_Ignore_Output_Transform Ecore_Wl_Event_Ignore_Output_Transform;
typedef struct _Ecore_Wl_Event_Global Ecore_Wl_Event_Global;
typedef struct _Ecore_Wl_Event_Keymap_Update Ecore_Wl_Event_Keymap_Update;
typedef struct _Ecore_Wl_Event_Indicator_Flick Ecore_Wl_Event_Indicator_Flick;
typedef struct _Ecore_Wl_Event_Clipboard_Data_Selected Ecore_Wl_Event_Clipboard_Data_Selected;
typedef struct _Ecore_Wl_Keygrab_Info Ecore_Wl_Keygrab_Info;
typedef struct _Ecore_Wl_Keyungrab_Info Ecore_Wl_Keyungrab_Info;
typedef struct _Ecore_Wl_Window_Keygrab_Info Ecore_Wl_Window_Keygrab_Info;

struct _Ecore_Wl_Event_Global
{
   Ecore_Wl_Display *display;
   Eina_Stringshare *interface;
   unsigned int id, version;
};

struct _Ecore_Wl_Event_Output_Transform
{
   Ecore_Wl_Output *output;
   int old_transform;
   int transform;
};

struct _Ecore_Wl_Event_Ignore_Output_Transform
{
   Ecore_Wl_Window *win;
   Eina_Bool ignore;
};

struct _Ecore_Wl_Event_Keymap_Update
{
   Ecore_Wl_Input *input;
   struct xkb_keymap *keymap;
};

enum _Ecore_Wl_Window_Visibility_Type
{
   ECORE_WL_WINDOW_VISIBILITY_TYPE_UNKNOWN = 0,
   ECORE_WL_WINDOW_VISIBILITY_TYPE_UNOBSCURED = 1,
   ECORE_WL_WINDOW_VISIBILITY_TYPE_PARTIALLY_OBSCURED = 2,
   ECORE_WL_WINDOW_VISIBILITY_TYPE_FULLY_OBSCURED = 3,
   ECORE_WL_WINDOW_VISIBILITY_TYPE_PRE_UNOBSCURED = 4,
};
// End of TIZEN_ONLY(20171109)

enum _Ecore_Wl_Window_Type
{
   ECORE_WL_WINDOW_TYPE_NONE,
   ECORE_WL_WINDOW_TYPE_TOPLEVEL,
   ECORE_WL_WINDOW_TYPE_FULLSCREEN,
   ECORE_WL_WINDOW_TYPE_MAXIMIZED,
   ECORE_WL_WINDOW_TYPE_TRANSIENT,
   ECORE_WL_WINDOW_TYPE_MENU,
   ECORE_WL_WINDOW_TYPE_DND,
   ECORE_WL_WINDOW_TYPE_CUSTOM,
// TIZEN_ONLY(20171109): merged with legacy ecore_wayland
   ECORE_WL_WINDOW_TYPE_NOTIFICATION,
   ECORE_WL_WINDOW_TYPE_UTILITY,
   ECORE_WL_WINDOW_TYPE_DIALOG,
   ECORE_WL_WINDOW_TYPE_DOCK,
   ECORE_WL_WINDOW_TYPE_SPLASH,
// End of TIZEN_ONLY(20171109)
};

// TIZEN_ONLY(20171109): merged with legacy ecore_wayland
enum _Ecore_Wl_Window_Style
{
   ECORE_WL_WINDOW_STYLE_NONE,
   ECORE_WL_WINDOW_STYLE_FULLSCREEN,
   ECORE_WL_WINDOW_STYLE_MAXIMIZED,
};
// End of TIZEN_ONLY(20171109)

enum _Ecore_Wl_Window_Buffer_Type
{
   ECORE_WL_WINDOW_BUFFER_TYPE_EGL_WINDOW,
   ECORE_WL_WINDOW_BUFFER_TYPE_EGL_IMAGE,
   ECORE_WL_WINDOW_BUFFER_TYPE_SHM
};

// TIZEN_ONLY(20171109): merged with legacy ecore_wayland
//this enum for keyrouter and client window side
//keycode (8~255)
enum _Ecore_Wl_Window_Keygrab_Mode
{
   ECORE_WL_WINDOW_KEYGRAB_UNKNOWN = 0, /**< Unknown keygrab mode */
   ECORE_WL_WINDOW_KEYGRAB_SHARED = 1, /**< Getting the grabbed-key together with the other client windows */
   ECORE_WL_WINDOW_KEYGRAB_TOPMOST = 2, /**< Getting the grabbed-key only when window is top of the stack */
   ECORE_WL_WINDOW_KEYGRAB_OVERRIDE_EXCLUSIVE = 3, /**< Getting the grabbed-key exclusively regardless of window's position. Being overrided the grab by the other client window  */
   ECORE_WL_WINDOW_KEYGRAB_EXCLUSIVE = 4 /**< Getting the grabbed-key exclusively regardless of window's position */
};

enum _Ecore_Wl_Conformant_Part_Type
{
   ECORE_WL_INDICATOR_PART = 0,
   ECORE_WL_KEYBOARD_PART = 1,
   ECORE_WL_CLIPBOARD_PART = 2
};

enum _Ecore_Wl_Virtual_Keyboard_State
{
   ECORE_WL_VIRTUAL_KEYBOARD_STATE_UNKNOWN = 0,
   ECORE_WL_VIRTUAL_KEYBOARD_STATE_OFF,
   ECORE_WL_VIRTUAL_KEYBOARD_STATE_ON,
};

enum _Ecore_Wl_Indicator_State
{
   ECORE_WL_INDICATOR_STATE_UNKNOWN = 0,
   ECORE_WL_INDICATOR_STATE_OFF,
   ECORE_WL_INDICATOR_STATE_ON
};

enum _Ecore_Wl_Clipboard_State
{
   ECORE_WL_CLIPBOARD_STATE_UNKNOWN = 0,
   ECORE_WL_CLIPBOARD_STATE_OFF,
   ECORE_WL_CLIPBOARD_STATE_ON
};

enum _Ecore_Wl_Indicator_Opacity_Mode
{
   ECORE_WL_INDICATOR_OPACITY_UNKNOWN = 0,
   ECORE_WL_INDICATOR_OPAQUE,
   ECORE_WL_INDICATOR_TRANSLUCENT,
   ECORE_WL_INDICATOR_TRANSPARENT,
   ECORE_WL_INDICATOR_BG_TRANSPARENT,
};

enum _Ecore_Wl_Indicator_Visible_Type
{
   ECORE_WL_INDICATOR_VISIBLE_TYPE_HIDDEN = 0,
   ECORE_WL_INDICATOR_VISIBLE_TYPE_SHOWN,
};

enum _Ecore_Wl_Window_Stack_Mode
{
   ECORE_WL_WINDOW_STACK_NONE  = 0,
   ECORE_WL_WINDOW_STACK_ABOVE = 1,
   ECORE_WL_WINDOW_STACK_BELOW = 2,
};
// End of TIZEN_ONLY(20171109)

typedef enum _Ecore_Wl_Window_Type Ecore_Wl_Window_Type;
// TIZEN_ONLY(20171109): merged with legacy ecore_wayland
typedef enum _Ecore_Wl_Window_Style Ecore_Wl_Window_Style;
// End of TIZEN_ONLY(20171109)
typedef enum _Ecore_Wl_Window_Buffer_Type Ecore_Wl_Window_Buffer_Type;
// TIZEN_ONLY(20171109): merged with legacy ecore_wayland
typedef enum _Ecore_Wl_Window_Keygrab_Mode Ecore_Wl_Window_Keygrab_Mode;
typedef enum _Ecore_Wl_Conformant_Part_Type Ecore_Wl_Conformant_Part_Type;
typedef enum _Ecore_Wl_Virtual_Keyboard_State Ecore_Wl_Virtual_Keyboard_State;
typedef enum _Ecore_Wl_Indicator_State Ecore_Wl_Indicator_State;
typedef enum _Ecore_Wl_Clipboard_State Ecore_Wl_Clipboard_State;
typedef enum _Ecore_Wl_Indicator_Opacity_Mode Ecore_Wl_Indicator_Opacity_Mode;
typedef enum _Ecore_Wl_Indicator_Visible_Type Ecore_Wl_Indicator_Visible_Type;
typedef enum _Ecore_Wl_Window_Stack_Mode Ecore_Wl_Window_Stack_Mode;
typedef enum _Ecore_Wl_Window_Visibility_Type Ecore_Wl_Window_Visibility_Type;
// End of TIZEN_ONLY(20171109)

/** @since 1.7.6 */
struct _Ecore_Wl_Global
{
   EINA_INLIST;
   unsigned int id;
   char *interface;
   unsigned int version;
};

struct _Ecore_Wl_Event_Mouse_In
{
   int modifiers;
   int x, y;
   struct
     {
        int x, y;
     } root;
   unsigned int window;
   unsigned int event_window;
   unsigned int root_window;
   unsigned int timestamp;
};

struct _Ecore_Wl_Event_Mouse_Out
{
   int modifiers;
   int x, y;
   struct
     {
        int x, y;
     } root;
   unsigned int window;
   unsigned int event_window;
   unsigned int root_window;
   unsigned int timestamp;
};

struct _Ecore_Wl_Event_Focus_In
{
   unsigned int win;
   unsigned int timestamp;
};

struct _Ecore_Wl_Event_Focus_Out
{
   unsigned int win;
   unsigned int timestamp;
};

struct _Ecore_Wl_Event_Window_Configure
{
   unsigned int win;
   unsigned int event_win;
   int x, y, w, h;
   int edges;
};

// TIZEN_ONLY(20171109): merged with legacy ecore_wayland
struct _Ecore_Wl_Event_Window_Activate
{
   unsigned int win;
   unsigned int parent_win;
   unsigned int event_win;
   Eina_Bool fobscured;
};

struct _Ecore_Wl_Event_Window_Deactivate
{
   unsigned int win;
   unsigned int parent_win;
   unsigned int event_win;
   Eina_Bool fobscured;
};

struct _Ecore_Wl_Event_Window_Visibility_Change
{
   unsigned int win;
   int          fully_obscured;
};

struct _Ecore_Wl_Event_Window_Pre_Visibility_Change
{
   unsigned int win;
   Ecore_Wl_Window_Visibility_Type type;
   unsigned int option;
};

struct _Ecore_Wl_Event_Window_Rotate
{
   unsigned int win;
   unsigned int angle;
   int w, h;
};

struct _Ecore_Wl_Event_Window_Show
{
   unsigned int win;
   unsigned int parent_win;
   unsigned int event_win;
   unsigned int data[4];
};

struct _Ecore_Wl_Event_Window_Hide
{
   unsigned int win;
   unsigned int parent_win;
   unsigned int event_win;
};

struct _Ecore_Wl_Event_Window_Lower
{
   unsigned int win;
   unsigned int timestamp;
};

struct _Ecore_Wl_Event_Dnd_Selection
{
   char **types;
   int num_types;
};
// End of TIZEN_ONLY(20171109)

struct _Ecore_Wl_Event_Dnd_Enter
{
   unsigned int win, source;
   char **types;
   int num_types;
   unsigned int serial;
   struct wl_data_offer *offer;
   struct
     {
        int x, y;
     } position;
};

struct _Ecore_Wl_Event_Dnd_Position
{
   unsigned int win, source;
   struct
     {
        int x, y;
     } position;
};

struct _Ecore_Wl_Event_Dnd_Leave
{
   unsigned int win, source;
};

struct _Ecore_Wl_Event_Dnd_Drop
{
   unsigned int win, source;
   struct
     {
        int x, y;
     } position;
};

struct _Ecore_Wl_Event_Dnd_End
{
   unsigned int win, source;
};

/** @since 1.12 */
struct _Ecore_Wl_Event_Data_Source_Cancelled
{
   unsigned int win, source;
};

/** @since 1.7 */
struct _Ecore_Wl_Event_Data_Source_Send
{
   char *type;
   int fd;
};

// TIZEN_ONLY(20171109): merged with legacy ecore_wayland
typedef enum
{
   ECORE_WL_SELECTION_CNP,
   ECORE_WL_SELECTION_DND
} Ecore_Wl_Selection_Type;
// End of TIZEN_ONLY(20171109)

/** @since 1.12 */
struct _Ecore_Wl_Event_Data_Source_Target
{
   char *type;
};

/** @since 1.7 */
struct _Ecore_Wl_Event_Selection_Data_Ready
{
   char *data;
   int len;
   Eina_Bool done;
// TIZEN_ONLY(20171109): merged with legacy ecore_wayland
   char **types;
   int num_types;
   Ecore_Wl_Selection_Type sel_type;
// End of TIZEN_ONLY(20171109)
};

struct _Ecore_Wl_Event_Interfaces_Bound
{
   Eina_Bool compositor : 1;
   Eina_Bool shm : 1;
   Eina_Bool shell : 1;
   Eina_Bool output : 1;
   Eina_Bool seat : 1;
   Eina_Bool data_device_manager : 1;
   Eina_Bool subcompositor : 1;
// TIZEN_ONLY(20171109): merged with legacy ecore_wayland
   Eina_Bool policy : 1;
   Eina_Bool policy_ext : 1;
// End of TIZEN_ONLY(20171109)
};

// TIZEN_ONLY(20171109): merged with legacy ecore_wayland
struct _Ecore_Wl_Event_Conformant_Change
{
   unsigned int win;
   Ecore_Wl_Conformant_Part_Type part_type;
   Eina_Bool state;
};

struct _Ecore_Wl_Event_Aux_Hint_Allowed
{
   unsigned int win;
   int id;
};

struct _Ecore_Wl_Event_Aux_Message
{
   int win;
   const char *key;
   const char *val;
   Eina_List *options;
};

struct _Ecore_Wl_Event_Window_Iconify_State_Change
{
   unsigned int win;
   int iconified;
   int force;
};

struct _Ecore_Wl_Event_Effect
{
   int win;
   unsigned int type;
};

struct _Ecore_Wl_Event_Indicator_Flick
{
   unsigned int win;
   int type;
};

struct _Ecore_Wl_Event_Clipboard_Data_Selected
{
   unsigned int win;
};

struct _Ecore_Wl_Keygrab_Info
{
   int key;
   int mode;
   int err;
};

struct _Ecore_Wl_Keyungrab_Info
{
   int key;
   int err;
};

struct _Ecore_Wl_Window_Keygrab_Info
{
   char *key;
   Ecore_Wl_Window_Keygrab_Mode mode;
};
// End of TIZEN_ONLY(20171109)

/**
 * @internal
 *
 * @file
 * @defgroup Ecore_Wl_Group Ecore_Wayland - Wayland integration
 * @ingroup Ecore
 * @brief Ecore functions for dealing with the Wayland window system.
 * Ecore_Wl provides a wrapper and convenience functions for using the
 * Wayland protocol in implementing a window system. Function groups for
 * this part of the library include the following:
 *
 * @li @ref Ecore_Wl_Init_Group
 * @li @ref Ecore_Wl_Display_Group
 * @li @ref Ecore_Wl_Flush_Group
 * @li @ref Ecore_Wl_Window_Group
 * @li @ref Ecore_Wl_Input_Group
 * @li @ref Ecore_Wl_Dnd_Group
 * @li @ref Ecore_Wl_Subsurf
 */

EAPI extern int ECORE_WL_EVENT_MOUSE_IN;
EAPI extern int ECORE_WL_EVENT_MOUSE_OUT;
EAPI extern int ECORE_WL_EVENT_FOCUS_IN;
EAPI extern int ECORE_WL_EVENT_FOCUS_OUT;
EAPI extern int ECORE_WL_EVENT_WINDOW_CONFIGURE;
// TIZEN_ONLY(20171109): merged with legacy ecore_wayland
EAPI extern int ECORE_WL_EVENT_WINDOW_ACTIVATE;
EAPI extern int ECORE_WL_EVENT_WINDOW_DEACTIVATE;
EAPI extern int ECORE_WL_EVENT_WINDOW_VISIBILITY_CHANGE;
EAPI extern int ECORE_WL_EVENT_WINDOW_PRE_VISIBILITY_CHANGE;
EAPI extern int ECORE_WL_EVENT_WINDOW_SHOW;
EAPI extern int ECORE_WL_EVENT_WINDOW_HIDE;
EAPI extern int ECORE_WL_EVENT_WINDOW_LOWER;
EAPI extern int ECORE_WL_EVENT_WINDOW_ROTATE;
// End of TIZEN_ONLY(20171109)
EAPI extern int ECORE_WL_EVENT_DND_ENTER;
EAPI extern int ECORE_WL_EVENT_DND_POSITION;
EAPI extern int ECORE_WL_EVENT_DND_LEAVE;
EAPI extern int ECORE_WL_EVENT_DND_DROP;
EAPI extern int ECORE_WL_EVENT_DND_OFFER; /** @since 1.8 */
EAPI extern int ECORE_WL_EVENT_DND_END; /** @since 1.8 */
EAPI extern int ECORE_WL_EVENT_DATA_SOURCE_TARGET; /** @since 1.7 */
EAPI extern int ECORE_WL_EVENT_DATA_SOURCE_SEND; /** @since 1.7 */
EAPI extern int ECORE_WL_EVENT_DATA_SOURCE_CANCELLED; /** @since 1.7 */
EAPI extern int ECORE_WL_EVENT_SELECTION_DATA_READY; /** @since 1.7 */
EAPI extern int ECORE_WL_EVENT_INTERFACES_BOUND;
// TIZEN_ONLY(20171109): merged with legacy ecore_wayland
EAPI extern int ECORE_WL_EVENT_CONFORMANT_CHANGE;
EAPI extern int ECORE_WL_EVENT_AUX_HINT_ALLOWED;
EAPI extern int ECORE_WL_EVENT_AUX_MESSAGE;
EAPI extern int ECORE_WL_EVENT_WINDOW_ICONIFY_STATE_CHANGE;
EAPI extern int ECORE_WL_EVENT_EFFECT_START;
EAPI extern int ECORE_WL_EVENT_EFFECT_END;
EAPI extern int ECORE_WL_EVENT_OUTPUT_TRANSFORM;
EAPI extern int ECORE_WL_EVENT_IGNORE_OUTPUT_TRANSFORM;
EAPI extern int ECORE_WL_EVENT_GLOBAL_ADDED;
EAPI extern int ECORE_WL_EVENT_GLOBAL_REMOVED;
EAPI extern int ECORE_WL_EVENT_KEYMAP_UPDATE;
EAPI extern int ECORE_WL_EVENT_INDICATOR_FLICK;
EAPI extern int ECORE_WL_EVENT_CLIPBOARD_DATA_SELECTED;
// End of TIZEN_ONLY(20171109)

/**
 * @internal
 * @defgroup Ecore_Wl_Init_Group Wayland Library Init and Shutdown Functions
 * @ingroup Ecore_Wl_Group
 *
 * Functions that start and shutdown the Ecore Wayland Library.
 */

/**
 * @internal
 * @ingroup Ecore_Wl_Init_Group
 * @brief Initializes the Wayland display connection to the given display.
 *
 * @param   name Display target name. if @c NULL, the default display is
 *          assumed.
 * @return  The number of times the library has been initialized without being
 *          shut down. @c 0 is returned if an error occurs.
 *
 */
EAPI int ecore_wl_init(const char *name);

/**
 * @internal
 * @ingroup Ecore_Wl_Init_Group
 * @brief Shuts down the Ecore Wayland Library.
 * In shutting down the library, the Wayland display connection is terminated
 * and any event handlers for it are removed.
 *
 * @return  The number of times the library has been initialized without
 *          being shut down.
 *
 */
EAPI int ecore_wl_shutdown(void);

/**
 * @internal
 * @defgroup Ecore_Wl_Flush_Group Wayland Synchronization Functions
 * @ingroup Ecore_Wl_Group
 *
 * Functions that ensure that all commands which have been issued by the
 * Ecore Wayland library have been sent to the server.
 */

/**
 * @internal
 * @ingroup Ecore_Wl_Flush_Group
 * @brief Sends all Wayland commands to the Wayland Display.
 *
 * @since 1.2
 */
EAPI void ecore_wl_flush(void);

/**
 * @internal
 * @ingroup Ecore_Wl_Flush_Group
 * @brief Flushes the command buffer and waits until all requests have been
 * processed by the server.
 *
 * @since 1.2
 */
EAPI void ecore_wl_sync(void);

/**
 * @internal
 * @defgroup Ecore_Wl_Display_Group Wayland Display Functions
 * @ingroup Ecore_Wl_Group
 *
 * Functions that set and retrieve various information about the Wayland Display.
 */

/**
 * @internal
 * @ingroup Ecore_Wl_Display_Group
 * @brief Retrieves the Wayland Shm Interface used for the current Wayland connection.
 *
 * @return The current wayland shm interface, or NULL on error
 *
 * @since 1.2
 */
EAPI struct wl_shm *ecore_wl_shm_get(void);

/**
 * @internal
 * @ingroup Ecore_Wl_Display_Group
 * @brief Retrieves the Wayland Display Interface used for the current Wayland connection.
 *
 * @return The current wayland display interface, or NULL on error
 *
 * @since 1.2
 */
EAPI struct wl_display *ecore_wl_display_get(void);

/**
 * @brief Retrieves the Compositor interface.
 *
 * This interface is used by clients to request the creation of surfaces and
 * regions.
 *
 * @return The current wayland compositor interface, or NULL on error
 * @since 1.8
 */

/**
 * @internal
 * @ingroup Ecore_Wl_Display_Group
 * @brief Retrieves the size of the current screen.
 *
 * @param w Where to return the width. May be NULL. Returns @c 0 on error.
 * @param h Where to return the height. May be NULL. Returns @c 0 on error.
 *
 * @since 1.2
 */
EAPI void ecore_wl_screen_size_get(int *w, int *h);

/**
 * @internal
 * @ingroup Ecore_Wl_Display_Group
 * @brief Retrieves the position of the current screen.
 *
 * @param x Where to return the horizontal position. May be NULL. Returns @c 0 on error.
 * @param y Where to return the vertical position. May be NULL. Returns @c 0 on error.
 *
 * @since 1.2
 */
EAPI void ecore_wl_pointer_xy_get(int *x, int *y);

/**
 * @internal
 * @brief Returns the screen DPI.
 *
 * This is a simplistic call to get DPI. It does not account for differing
 * DPI in the x and y axes nor does it account for multihead or xinerama and
 * xrandr where different parts of the screen may have different DPI etc.
 *
 * @return the general screen DPI (dots/pixels per inch).
 *
 * @since 1.2
 */
EAPI int ecore_wl_dpi_get(void);

/**
 * @internal
 * @brief Dispatches Wayland events.
 *
 * Dispatches any events queued on the default queue.  Wayland will attempt
 * to read from the display fd and queue things up according to the proxy
 * assignment.
 */
EAPI void ecore_wl_display_iterate(void);

/*
 * @ingroup Ecore_Wl_Display_Group
 * @since 1.8
 */
EAPI Eina_Bool ecore_wl_animator_source_set(Ecore_Animator_Source source);

/**
 * @internal
 * @brief Retrieves the requested cursor from the cursor theme.
 *
 * @param cursor_name The desired cursor name to be looked up in the theme
 * @return the cursor or NULL if the cursor cannot be found
 *
 * @since 1.2
 */
EAPI struct wl_cursor *ecore_wl_cursor_get(const char *cursor_name);

/**
 * @internal
 * @defgroup Ecore_Wl_Input_Group Functions dealing with Wayland Input
 * @ingroup Ecore_Wl_Group
 *
 * Functions to interface with Wayland Input.
 */

/**
 * @internal
 * @ingroup Ecore_Wl_Input_Group
 * @since 1.8
 */
EAPI Ecore_Wl_Input *ecore_wl_input_get(void);
EAPI void ecore_wl_input_grab(Ecore_Wl_Input *input, Ecore_Wl_Window *win, unsigned int button);
EAPI void ecore_wl_input_ungrab(Ecore_Wl_Input *input);
EAPI void ecore_wl_input_pointer_set(Ecore_Wl_Input *input, struct wl_surface *surface, int hot_x, int hot_y);
EAPI void ecore_wl_input_cursor_from_name_set(Ecore_Wl_Input *input, const char *cursor_name);
EAPI void ecore_wl_input_cursor_default_restore(Ecore_Wl_Input *input);
EAPI struct wl_seat *ecore_wl_input_seat_get(Ecore_Wl_Input *input);

EAPI Eina_Inlist *ecore_wl_outputs_get(void);

// TIZEN_ONLY(20171109): merged with legacy ecore_wayland
EAPI Ecore_Wl_Output * ecore_wl_window_output_find(Ecore_Wl_Window *window);
EAPI int ecore_wl_output_transform_get(Ecore_Wl_Output *output);
// End of TIZEN_ONLY(20171109)
/**
 * @internal
 * @ingroup Ecore_Wl_Input_Group
 * @since 1.12
 */
EAPI void ecore_wl_input_cursor_size_set(Ecore_Wl_Input *input, const int size);
EAPI void ecore_wl_input_cursor_theme_name_set(Ecore_Wl_Input *input, const char *cursor_theme_name);

/**
 * @ingroup Ecore_Wl_Input_Group
 * @since 1.17
 */
EAPI struct xkb_keymap *ecore_wl_input_keymap_get(Ecore_Wl_Input *input);


/**
 * @internal
 * @ingroup Ecore_Wl_Display_Group
 * @brief Retrieves the Wayland Globals Interface list used for the current Wayland connection.
 *
 * This call, if done after the ECORE_WL_EVENT_INTERFACES_BOUND event was
 * received already, won't block the mainloop or trigger a dispatch. It will
 * return the current globals immediately. However, if done before this event,
 * it will probably block the mainloop waiting for the sync "done" event to be
 * received (by using one or more wl_display_dispatch call), and then finally
 * return the wl globals list.
 *
 * There's no need to call dispatch manually, since this call will do it if necessary.
 *
 * @return The current wayland globals interface list, or NULL on error
 *
 * @since 1.7.6
 */
EAPI Eina_Inlist *ecore_wl_globals_get(void);

/**
 * @internal
 * @ingroup Ecore_Wl_Display_Group
 * @brief Retrieves the Wayland Registry used for the current Wayland connection.
 *
 * @return The current wayland registry, or NULL on error
 *
 * @since 1.7.6
 */
EAPI struct wl_registry *ecore_wl_registry_get(void);

/**
 * @internal
 * @defgroup Ecore_Wl_Window_Group Wayland Window functions.
 * @ingroup Ecore_Wl_Group
 * Functions that can be used to create a Wayland window.
 */

/**
 * @internal
 * @ingroup Ecore_Wl_Window_Group
 * @brief Creates a new window.
 *
 * @param parent The parent window to use. If @p parent is @c 0, the root window
 *               of the default display is used.
 * @param x      X Position
 * @param y      Y position
 * @param w      Width
 * @param h      Height
 * @param buffer_type The type of the buffer to be used to create a new Ecore_Wl_Window.
 *
 * @return The new window, or NULL on failure
 *
 * @since 1.2
 */
EAPI Ecore_Wl_Window *ecore_wl_window_new(Ecore_Wl_Window *parent, int x, int y, int w, int h, int buffer_type);

/**
 * @internal
 * @ingroup Ecore_Wl_Window_Group
 * @brief Deletes the given window.
 *
 * @param win The given window
 *
 * @since 1.2
 */
EAPI void ecore_wl_window_free(Ecore_Wl_Window *win);

/**
 * @internal
 * @ingroup Ecore_Wl_Window_Group
 * @brief Signals for Wayland to initiate a window move.
 *
 * The position requested (@p x, @p y) is not honored by Wayland because
 * Wayland does not allow specific window placement to be set.
 *
 * @param win The window to move.
 * @param x   X Position
 * @param y   Y Position
 *
 * @since 1.2
 */
EAPI void ecore_wl_window_move(Ecore_Wl_Window *win, int x, int y);

/**
 * @internal
 * @ingroup Ecore_Wl_Window_Group
 * @brief Signals for Wayland to initiate a window resize.
 *
 * The size requested (@p w, @p h) is not honored by Wayland because
 * Wayland does not allow specific window sizes to be set.
 *
 * @param win      The window to resize.
 * @param w        Width
 * @param h        Height
 * @param location The edge of the window from where the resize should start.
 *
 * @since 1.2
 */
EAPI void ecore_wl_window_resize(Ecore_Wl_Window *win, int w, int h, int location);
EAPI void ecore_wl_window_damage(Ecore_Wl_Window *win, int x, int y, int w, int h);
EAPI void ecore_wl_window_buffer_attach(Ecore_Wl_Window *win, struct wl_buffer *buffer, int x, int y);

/* @since 1.8 */
EAPI void ecore_wl_window_commit(Ecore_Wl_Window *win);

/**
 * @internal
 * @ingroup Ecore_Wl_Window_Group
 * @brief Shows a window.
 *
 * Synonymous to "mapping" a window in Wayland System terminology.
 *
 * @param win The window to show.
 *
 * @since 1.2
 */
EAPI void ecore_wl_window_show(Ecore_Wl_Window *win);

/**
 * @internal
 * @ingroup Ecore_Wl_Window_Group
 * @brief Hides a window.
 *
 * Synonymous to "unmapping" a window in Wayland System terminology.
 *
 * @param win The window to hide.
 *
 * @since 1.2
 */
EAPI void ecore_wl_window_hide(Ecore_Wl_Window *win);

/**
 * @internal
 * @ingroup Ecore_Wl_Window_Group
 * @brief Raises a window.
 *
 * @param win The window to raise.
 *
 * @since 1.2
 */
EAPI void ecore_wl_window_raise(Ecore_Wl_Window *win);
// TIZEN_ONLY(20171109): merged with legacy ecore_wayland
EAPI void ecore_wl_window_lower(Ecore_Wl_Window *win);
EAPI void ecore_wl_window_activate(Ecore_Wl_Window *win);
// End of TIZEN_ONLY(20171109)
EAPI void ecore_wl_window_maximized_set(Ecore_Wl_Window *win, Eina_Bool maximized);
EAPI Eina_Bool ecore_wl_window_maximized_get(Ecore_Wl_Window *win);
EAPI void ecore_wl_window_fullscreen_set(Ecore_Wl_Window *win, Eina_Bool fullscreen);
EAPI Eina_Bool ecore_wl_window_fullscreen_get(Ecore_Wl_Window *win);
EAPI void ecore_wl_window_transparent_set(Ecore_Wl_Window *win, Eina_Bool transparent);
EAPI Eina_Bool ecore_wl_window_transparent_get(Ecore_Wl_Window *win);
// TIZEN_ONLY(20171109): merged with legacy ecore_wayland
EAPI void ecore_wl_window_buffer_transform_set(Ecore_Wl_Window *win, int buffer_transform);
// End of TIZEN_ONLY(20171109)
EAPI void ecore_wl_window_update_size(Ecore_Wl_Window *win, int w, int h);
EAPI void ecore_wl_window_update_location(Ecore_Wl_Window *win, int x, int y);
EAPI struct wl_surface *ecore_wl_window_surface_get(Ecore_Wl_Window *win);
EAPI struct wl_shell_surface *ecore_wl_window_shell_surface_get(Ecore_Wl_Window *win);
EAPI struct xdg_surface *ecore_wl_window_xdg_surface_get(Ecore_Wl_Window *win); /** @since 1.12 */
EAPI Ecore_Wl_Window *ecore_wl_window_find(unsigned int id);
// TIZEN_ONLY(20171109): merged with legacy ecore_wayland
EAPI Ecore_Wl_Window_Type ecore_wl_window_type_get(Ecore_Wl_Window *win);
// End of TIZEN_ONLY(20171109)
EAPI void ecore_wl_window_type_set(Ecore_Wl_Window *win, Ecore_Wl_Window_Type type);
EAPI void ecore_wl_window_pointer_set(Ecore_Wl_Window *win, struct wl_surface *surface, int hot_x, int hot_y);
// TIZEN_ONLY(20171109): merged with legacy ecore_wayland
EAPI Eina_Bool ecore_wl_window_pointer_warp(Ecore_Wl_Window *win, int x, int y);
// End of TIZEN_ONLY(20171109)
EAPI void ecore_wl_window_cursor_from_name_set(Ecore_Wl_Window *win, const char *cursor_name);
EAPI void ecore_wl_window_cursor_default_restore(Ecore_Wl_Window *win);
EAPI void ecore_wl_window_parent_set(Ecore_Wl_Window *win, Ecore_Wl_Window *parent);
// TIZEN_ONLY(20171109): merged with legacy ecore_wayland
EAPI void ecore_wl_window_position_set(Ecore_Wl_Window *win, int x, int y);
EAPI void ecore_wl_window_focus_skip_set(Ecore_Wl_Window *win, Eina_Bool focus_skip);
EAPI void ecore_wl_window_role_set(Ecore_Wl_Window *win, const char *role);
EAPI int ecore_wl_window_active_angle_get(Ecore_Wl_Window *win);
// End of TIZEN_ONLY(20171109)

/**
 * @internal
 * @ingroup Ecore_Wl_Window_Group
 * @brief Iconifies a window.
 *
 * @param win The window to iconifiy
 *
 * @since 1.12
 */
EAPI void ecore_wl_window_iconified_set(Ecore_Wl_Window *win, Eina_Bool iconified);
EAPI Eina_Bool ecore_wl_window_iconified_get(Ecore_Wl_Window *win);

// TIZEN_ONLY(20171109): merged with legacy ecore_wayland
EAPI void ecore_wl_window_iconify_state_update(Ecore_Wl_Window *win, Eina_Bool iconified, Eina_Bool send_event);
// End of TIZEN_ONLY(20171109)

EAPI int ecore_wl_window_id_get(Ecore_Wl_Window *win);
EAPI void ecore_wl_window_title_set(Ecore_Wl_Window *win, const char *title);
EAPI void ecore_wl_window_class_name_set(Ecore_Wl_Window *win, const char *class_name);
EAPI int ecore_wl_window_surface_id_get(Ecore_Wl_Window *win);

EAPI Ecore_Wl_Input *ecore_wl_window_keyboard_get(Ecore_Wl_Window *win);

// TIZEN_ONLY(20171109): merged with legacy ecore_wayland
EAPI void ecore_wl_window_stack_mode_set(Ecore_Wl_Window *win, Ecore_Wl_Window_Stack_Mode mode);
// End of TIZEN_ONLY(20171109)
/**
 * @internal
 * @ingroup Ecore_Wl_Window_Group
 * @brief Returns a wl_surface with no association to any wl_shell_surface.
 *
 * @param win The window which will use this newly created surface.
 * @return wl_surface or NULL on error
 *
 * @since 1.8
 */
EAPI struct wl_surface *ecore_wl_window_surface_create(Ecore_Wl_Window *win);

EAPI void ecore_wl_window_alpha_set(Ecore_Wl_Window *win, Eina_Bool alpha);
EAPI Eina_Bool ecore_wl_window_alpha_get(Ecore_Wl_Window *win);

/**
 * @internal
 * @ingroup Ecore_Wl_Window_Group
 * @brief Returns the Ecore_Wl_Window from a wl_surface.
 *
 * @param surface The surface for which to find the Ecore_Wl_Window from
 * @return the Ecore_Wl_Window associated with this surface, or NULL if not found
 *
 * @since 1.8
 */
EAPI Ecore_Wl_Window *ecore_wl_window_surface_find(struct wl_surface *surface);

// TIZEN_ONLY(20171109): merged with legacy ecore_wayland
/**
 * @ingroup Ecore_Wl_Window_Group
 * @brief Sets the input rect of the Ecore_Wl_Window.
 *
 * To set an empty rect, pass x and y and width and height as -1, -1, 1, 1.
 *
 * @param win The window
 * @param input_rect The rectangle to be set as input
 *
 * @since 1.8
 */
EAPI void ecore_wl_window_input_rect_set(Ecore_Wl_Window *win, Eina_Rectangle *input_rect);

/**
 * @ingroup Ecore_Wl_Window_Group
 * @brief Adds the input rect of the Ecore_Wl_Window.
 *
 * @param win The window
 * @param input_rect The rectangle to be added as input
 *
 * @since 1.8
 */
EAPI void ecore_wl_window_input_rect_add(Ecore_Wl_Window *win, Eina_Rectangle *input_rect);

/**
 * @ingroup Ecore_Wl_Window_Group
 * @brief Subtracts the input rect of the Ecore_Wl_Window.
 *
 * @param win The window
 * @param input_rect The rectangle to be subtracted as input
 *
 * @since 1.8
 */
EAPI void ecore_wl_window_input_rect_subtract(Ecore_Wl_Window *win, Eina_Rectangle *input_rect);
// End of TIZEN_ONLY(20171109)

/**
 * @internal
 * @ingroup Ecore_Wl_Window_Group
 * @brief Sets the input region of the Ecore_Wl_Window.
 *
 * To set an empty region, pass width and height as 0.
 *
 * An empty input region means the entire window surface will accept input.
 *
 * @param win The window
 * @param x The left point of the region.
 * @param y The top point of the region.
 * @param w The width of the region.
 * @param h The height of the region.
 *
 * @since 1.8
 */
EAPI void ecore_wl_window_input_region_set(Ecore_Wl_Window *win, int x, int y, int w, int h);

/**
 * @internal
 * @ingroup Ecore_Wl_Window_Group
 * @brief Sets the opaque region of the Ecore_Wl_Window.
 *
 * @param win The window
 * @param x The left point of the region.
 * @param y The top point of the region.
 * @param w The width of the region.
 * @param h The height of the region.
 *
 * @since 1.8
 */
EAPI void ecore_wl_window_opaque_region_set(Ecore_Wl_Window *win, int x, int y, int w, int h);

/**
 * @internal
 * @ingroup Ecore_Wl_Window_Group
 * @brief Sets the rotation of the Ecore_Wl_Window.
 *
 * @param win The window
 * @param rotation The degree of rotation for this window
 *
 * @since 1.8
 */
EAPI void ecore_wl_window_rotation_set(Ecore_Wl_Window *win, int rotation);

/**
 * @internal
 * @ingroup Ecore_Wl_Window_Group
 * @brief Gets the rotation of the Ecore_Wl_Window.
 *
 * @param win The window
 * @return The degree of rotation for this window
 *
 * @since 1.8
 */
EAPI int ecore_wl_window_rotation_get(Ecore_Wl_Window *win);

/**
 * @internal
 * @defgroup Ecore_Wl_Dnd_Group Functions dealing with Wayland Drag-n-Drop
 * @ingroup Ecore_Wl_Group
 *
 * Functions to interface with Wayland Drag-n-Drop
 */

/**
 * @internal
 * @deprecated use ecore_wl_dnd_selection_set
 * @since 1.7
*/
EINA_DEPRECATED EAPI Eina_Bool ecore_wl_dnd_set_selection(Ecore_Wl_Dnd *dnd, const char **types_offered);

/**
 * @internal
 * @deprecated use ecore_wl_dnd_selection_get
 * @since 1.7
*/
EINA_DEPRECATED EAPI Eina_Bool ecore_wl_dnd_get_selection(Ecore_Wl_Dnd *dnd, const char *type);

/**
 * @internal
 * @deprecated Do Not Use
 * @since 1.7
 */
EINA_DEPRECATED EAPI Ecore_Wl_Dnd *ecore_wl_dnd_get(void);

/**
 * @internal
 * @deprecated use ecore_wl_dnd_drag_start
 * @since 1.7
 */
EINA_DEPRECATED EAPI Eina_Bool ecore_wl_dnd_start_drag(Ecore_Wl_Dnd *dnd);

/**
 * @internal
 * @deprecated use ecore_wl_dnd_selection_owner_has
 * @since 1.7
 */
EINA_DEPRECATED EAPI Eina_Bool ecore_wl_dnd_selection_has_owner(Ecore_Wl_Dnd *dnd);

/**
 * @internal
 * @ingroup Ecore_Wl_Dnd_Group
 * @return false on error, true otherwise
 *
 * @since 1.8
 */
EAPI Eina_Bool ecore_wl_dnd_selection_set(Ecore_Wl_Input *input, const char **types_offered);

/**
 * @internal
 * @ingroup Ecore_Wl_Dnd_Group
 * @return false if type not found or on error, true otherwise
 *
 * @since 1.8
 */
EAPI Eina_Bool ecore_wl_dnd_selection_get(Ecore_Wl_Input *input, const char *type);

/**
 * @internal
 * @ingroup Ecore_Wl_Dnd_Group
 * @return true if input has a selection source, false otherwise or on error
 *
 * @since 1.8
 */
EAPI Eina_Bool ecore_wl_dnd_selection_owner_has(Ecore_Wl_Input *input);

/**
 * @internal
 * @ingroup Ecore_Wl_Dnd_Group
 * @return false on error, true otherwise
 *
 * @since 1.8
 */
EAPI Eina_Bool ecore_wl_dnd_selection_clear(Ecore_Wl_Input *input);

/**
 * @internal
 * @ingroup Ecore_Wl_Dnd_Group
 * @since 1.8
 */
EAPI void ecore_wl_dnd_drag_start(Ecore_Wl_Input *input, Ecore_Wl_Window *win, Ecore_Wl_Window *dragwin, int x, int y, int w, int h);

/**
 * @internal
 * @ingroup Ecore_Wl_Dnd_Group
 * @since 1.8
 */
EAPI void ecore_wl_dnd_drag_end(Ecore_Wl_Input *input);

/**
 * @internal
 * @ingroup Ecore_Wl_Dnd_Group
 * @return false if type not found or on error, true otherwise
 *
 * @since 1.8
 */
EAPI Eina_Bool ecore_wl_dnd_drag_get(Ecore_Wl_Input *input, const char *type);

/**
 * @internal
 * @ingroup Ecore_Wl_Dnd_Group
 * @since 1.8
 */
EAPI void ecore_wl_dnd_drag_types_set(Ecore_Wl_Input *input, const char **types_offered);

/**
 * @internal
 * @ingroup Ecore_Wl_Dnd_Group
 * @return the data types being offered for drag-and-drop, or NULL on error
 *
 * @since 1.8
 */
EAPI struct wl_array *ecore_wl_dnd_drag_types_get(Ecore_Wl_Input *input);

/**
 * @ingroup Ecore_Wl_Dnd_Group
 * @since 1.8
 */
/* EAPI Ecore_Wl_Dnd_Source *ecore_wl_dnd_drag_source_create(Ecore_Wl_Dnd *dnd); */

/**
 * @internal
 * Enables/disables server mode.
 *
 * With this enabled, the compositor acts as a Wayland server, iterating
 * the display and dispatching events.  With it disabled, the process is
 * handled as a client.
 */
EAPI void ecore_wl_server_mode_set(Eina_Bool on);

// TIZEN_ONLY(20171109): merged with legacy ecore_wayland
EAPI Eina_Bool ecore_wl_server_mode_get(void);
// End of TIZEN_ONLY(20171109)

/**
 * @internal
 * @defgroup Ecore_Wl_Subsurf Functions to manipulate subsurfaces.
 * @ingroup Ecore_Wl_Group
 *
 * Functions to manipulate wayland subsurfaces, using Ecore_Wl_Subsurf.
 *
 * This API is intended to expose Wayland subsurface functionality, although it
 * should not be necessary for most applications to use it, as soon as we have
 * means to make Evas automatically switch Evas images to use subsurfaces.
 *
 * It can/should be used, for instance, when subsurfaces are needed to be not
 * in sync with the main window surface.
 */

/**
 * @internal
 * @ingroup Ecore_Wl_Subsurf
 * @brief Creates and returns a new subsurface.
 *
 * Create a new surface (and subsurface interface), with the parent surface
 * being the one associated with the given @param win.
 *
 * The @param win must be visible, otherwise there will be no surface created
 * for it yet.
 *
 * @return the allocated and initialized Ecore_Wl_Subsurf object, or
 * NULL on failure
 *
 * @since 1.8
 */
EAPI Ecore_Wl_Subsurf *ecore_wl_subsurf_create(Ecore_Wl_Window *win);

/**
 * @internal
 * @ingroup Ecore_Wl_Subsurf
 * @brief Destroys the given subsurface, as well as the surface associated with it.
 *
 * @param ess the subsurface
 *
 * @since 1.8
 */
EAPI void ecore_wl_subsurf_del(Ecore_Wl_Subsurf *ess);

/**
 * @internal
 * @ingroup Ecore_Wl_Subsurf
 * @brief Gets the wl_surface for this subsurface.
 *
 * @param ess the subsurface
 * @return the wl_surface associated with this subsurface, or NULL on failure
 *
 * @since 1.8
 */
EAPI struct wl_surface *ecore_wl_subsurf_surface_get(Ecore_Wl_Subsurf *ess);

/**
 * @internal
 * @ingroup Ecore_Wl_Subsurf
 * @brief Sets the position of this subsurface, relative to its parent surface.
 * If ess is defined and the x, y coordinates differ from the currently
 * tracked position, this also schedules a sub-surface position change.
 *
 * @param ess the subsurface
 * @param x coordinate in the parent surface
 * @param y coordinate in the parent surface
 *
 * @since 1.8
 */
EAPI void ecore_wl_subsurf_position_set(Ecore_Wl_Subsurf *ess, int x, int y);

/**
 * @internal
 * @ingroup Ecore_Wl_Subsurf
 * @brief Gets the position of this subsurface, relative to its parent surface.
 * Coordinates will be returned in x and y if non-NULL.
 *
 * @param ess the subsurface
 * @param x coordinate in the parent surface, or NULL to ignore
 * @param y coordinate in the parent surface, or NULL to ignore
 *
 * @since 1.8
 */
EAPI void ecore_wl_subsurf_position_get(Ecore_Wl_Subsurf *ess, int *x, int *y);

/**
 * @internal
 * @ingroup Ecore_Wl_Subsurf
 * @brief Places subsurface on layer above a reference surface.
 *
 * Moves the @param ess subsurface to just above the reference @param
 * surface, changing the z-order.  The reference @param surface must
 * be either a sibling or parent surface, else a protocol error will
 * be generated.
 *
 * @param ess the subsurface
 * @param surface the sibling reference surface
 *
 * @since 1.8
 */
EAPI void ecore_wl_subsurf_place_above(Ecore_Wl_Subsurf *ess, struct wl_surface *surface);

/**
 * @internal
 * @ingroup Ecore_Wl_Subsurf
 * @brief Places subsurface on layer below a reference surface.
 *
 * See ecore_wl_subsurf_place_above.
 *
 * @param ess the subsurface
 * @param surface the sibling reference surface
 *
 * @since 1.8
 */
EAPI void ecore_wl_subsurf_place_below(Ecore_Wl_Subsurf *ess, struct wl_surface *surface);

/**
 * @internal
 * @ingroup Ecore_Wl_Subsurf
 * @brief Enables or disables sub-surface synchronization.
 *
 * When synchronization is enabled, surface commits on the subsurface
 * will be cached and only applied when the parent surface's state is
 * applied.  This ensures atomic updates of the parent and all of its
 * synchronized sub-surfaces.
 *
 * When synchronization is disabled, commits will apply to the pending
 * state directly without caching, just like a normal wl_surface.  If
 * there are already cached events when this is set, those events are
 * applied simultaneously with the desync event.
 *
 * Attempting to enable synchronization when the subsurface already
 * thinks it's sync'd, or desync when it believes its desync'd, will
 * be trivially ignored and will not generate a Wayland event.
 *
 * See Wayland's set_desync documentation for further details and
 * exceptional cases.
 *
 * @param ess the subsurface
 * @param val true to enable synchronization, false to desynchronize
 *
 * @since 1.8
 */
EAPI void ecore_wl_subsurf_sync_set(Ecore_Wl_Subsurf *ess, Eina_Bool val);

/**
 * @internal
 * @ingroup Ecore_Wl_Subsurf
 * @brief Sets an opaque region for the given subsurface.
 *
 * This is an optimization hint to the compositor to allow it avoid
 * redrawing content unnecessarily.  Note that marking transparent
 * content as opaque will cause repaint artifacts.
 *
 * Use a 0x0 region size to unset the opaque region.
 *
 * @param ess the subsurface
 * @param x coordinate in the parent surface
 * @param y coordinate in the parent surface
 * @param w width to set as opaque
 * @param h height to set as opaque
 *
 * @since 1.8
 */
EAPI void ecore_wl_subsurf_opaque_region_set(Ecore_Wl_Subsurf *ess, int x, int y, int w, int h);

// TIZEN_ONLY(20171109): merged with legacy ecore_wayland
/**
 * @ingroup Ecore_Wl_Window
 * @brief Sets keygrab value of the window.
 *
 */
EAPI Eina_Bool ecore_wl_window_keygrab_set(Ecore_Wl_Window *win, const char *key, int mod, int not_mod, int priority, Ecore_Wl_Window_Keygrab_Mode grab_mode);

/**
 * @ingroup Ecore_Wl_Window
 * @brief Unsets keygrab value of the window.
 *
 */
EAPI Eina_Bool ecore_wl_window_keygrab_unset(Ecore_Wl_Window *win, const char *key, int mod, int any_mod);

/**
 * @ingroup Ecore_Wl_Window
 * @brief Sets keygrab values of the window.
 *
 * The returned list should be freed with @c eina_list_free() when you no
 * longer need it.
 *
 * @param win The window
 * @param infos The list of keys and grab modes for setting keygrab
 *
 * @return NULL, or the list of keys being failed keygrab set.
 *
 * @Example
 *
 * Ecore_Wl_Window_Keygrab_Info *info = malloc(sizeof(Ecore_Wl_Window_Keygrab_Info));
 * info->key = "XF86AudioLowerVolume";
 * info->mode = ECORE_WL_WINDOW_KEYGRAB_SHARED;
 * infos = eina_list_append(infos, info);
 *
 * ecore_wl_window_keygrab_list_set(win, infos);
 *
 * @since tizen 3.0
 */
EAPI Eina_List *ecore_wl_window_keygrab_list_set(Ecore_Wl_Window *win, Eina_List *infos);

/**
 * @ingroup Ecore_Wl_Window
 * @brief Unsets keygrab values of the window.
 *
 * The returned list should be freed with @c eina_list_free() when you no
 * longer need it.
 *
 * @param win The window
 * @param infos The list of infos for unsetting keygrab
 *
 * @return NULL, or the list of keys being failed keygrab unset.
 *
 * @Example
 *
 * Ecore_Wl_Window_Keygrab_Info *info = malloc(sizeof(Ecore_Wl_Window_Keygrab_Info));
 * info->key = "XF86AudioLowerVolume";
 * infos = eina_list_append(infos, info);
 *
 * ecore_wl_window_keygrab_list_unset(win, infos);
 *
 * @since tizen 3.0
 */
EAPI Eina_List *ecore_wl_window_keygrab_list_unset(Ecore_Wl_Window *win, Eina_List *infos);

EAPI void ecore_wl_window_conformant_set(Ecore_Wl_Window *win, unsigned int is_conformant);
EAPI Eina_Bool ecore_wl_window_conformant_get(Ecore_Wl_Window *win);

EAPI void ecore_wl_window_indicator_geometry_set(Ecore_Wl_Window *win, int x, int y, int w, int h);
EAPI Eina_Bool ecore_wl_window_indicator_geometry_get(Ecore_Wl_Window *win, int *x, int *y, int *w, int *h);
EAPI void ecore_wl_window_indicator_state_set(Ecore_Wl_Window *win, Ecore_Wl_Indicator_State state);
EAPI Ecore_Wl_Indicator_State ecore_wl_window_indicator_state_get(Ecore_Wl_Window *win);
EAPI void ecore_wl_window_indicator_opacity_set(Ecore_Wl_Window *win, Ecore_Wl_Indicator_Opacity_Mode mode);
EAPI Ecore_Wl_Indicator_Opacity_Mode ecore_wl_window_indicator_opacity_get(Ecore_Wl_Window *win);
EAPI void ecore_wl_indicator_visible_type_set(Ecore_Wl_Window *win, Ecore_Wl_Indicator_Visible_Type type);
EAPI Ecore_Wl_Indicator_Visible_Type ecore_wl_indicator_visible_type_get(Ecore_Wl_Window *win);

EAPI void ecore_wl_window_clipboard_geometry_set(Ecore_Wl_Window *win, int x, int y, int w, int h);
EAPI Eina_Bool ecore_wl_window_clipboard_geometry_get(Ecore_Wl_Window *win, int *x, int *y, int *w, int *h);
EAPI void ecore_wl_window_clipboard_state_set(Ecore_Wl_Window *win, Ecore_Wl_Clipboard_State state);
EAPI Ecore_Wl_Clipboard_State ecore_wl_window_clipboard_state_get(Ecore_Wl_Window *win);
EAPI void ecore_wl_clipboard_show(Ecore_Wl_Window *win);
EAPI void ecore_wl_clipboard_hide(Ecore_Wl_Window *win);
EAPI Eina_Bool ecore_wl_clipboard_data_only_set(Eina_Bool data_only);

EAPI void ecore_wl_window_keyboard_geometry_set(Ecore_Wl_Window *win, int x, int y, int w, int h);
EAPI Eina_Bool ecore_wl_window_keyboard_geometry_get(Ecore_Wl_Window *win, int *x, int *y, int *w, int *h);
EAPI void ecore_wl_window_keyboard_state_set(Ecore_Wl_Window *win, Ecore_Wl_Virtual_Keyboard_State state);
EAPI Ecore_Wl_Virtual_Keyboard_State ecore_wl_window_keyboard_state_get(Ecore_Wl_Window *win);

EAPI void ecore_wl_window_rotation_preferred_rotation_set(Ecore_Wl_Window *win, int rot);
EAPI void ecore_wl_window_rotation_available_rotations_set(Ecore_Wl_Window *win, const int *rots, unsigned int count);
EAPI void ecore_wl_window_rotation_change_done_send(Ecore_Wl_Window *win);
EAPI void ecore_wl_window_rotation_geometry_set(Ecore_Wl_Window *win, int rot, int x, int y, int w, int h);
EAPI void ecore_wl_window_rotation_changed_callback_set(Ecore_Wl_Window *win, void *data, void (*func)(Ecore_Wl_Window *win, int rot, Eina_Bool resize, int w, int h, void *data));

EAPI Eina_List * ecore_wl_window_aux_hints_supported_get(Ecore_Wl_Window *win);
EAPI void ecore_wl_window_aux_hint_add(Ecore_Wl_Window *win, int id, const char *hint, const char *val);
EAPI void ecore_wl_window_aux_hint_change(Ecore_Wl_Window *win, int id, const char *val);
EAPI void ecore_wl_window_aux_hint_del(Ecore_Wl_Window *win, int id);

EAPI void ecore_wl_window_floating_mode_set(Ecore_Wl_Window *win, Eina_Bool floating);
EAPI void ecore_wl_window_geometry_get(Ecore_Wl_Window *win, int *x, int *y, int *w, int *h);
EAPI Ecore_Wl_Input * ecore_wl_window_input_get(Ecore_Wl_Window *win);

EAPI Eina_Bool ecore_wl_window_ignore_output_transform_get(Ecore_Wl_Window *win);
EAPI void ecore_wl_window_video_has(Ecore_Wl_Window *win, Eina_Bool has);
// End of TIZEN_ONLY(20171109)

#ifdef __cplusplus
}
#endif

#undef EAPI
#define EAPI

#endif
