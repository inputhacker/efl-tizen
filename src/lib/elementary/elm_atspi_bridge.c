#ifdef HAVE_CONFIG_H
# include "elementary_config.h"
#endif

#define EFL_ACCESS_COMPONENT_PROTECTED
#define EFL_ACCESS_OBJECT_PROTECTED
#define EFL_ACCESS_ACTION_PROTECTED
#define EFL_ACCESS_VALUE_PROTECTED
#define EFL_ACCESS_SELECTION_PROTECTED
#define EFL_ACCESS_TEXT_PROTECTED
#define EFL_ACCESS_EDITABLE_TEXT_PROTECTED

#define EFL_ACCESS_ACTION_BETA

#include "atspi/atspi-constants.h"

#include <stdint.h>
#include <assert.h>
#include <Elementary.h>
#include "elm_priv.h"
//TIZEN_ONLY(20171222): include eo info header for class check
#include "elm_widget_index.h"
//

/*
 * Accessibility Bus info not defined in atspi-constants.h
 */
#define A11Y_DBUS_NAME "org.a11y.Bus"
#define A11Y_DBUS_PATH "/org/a11y/bus"
#define A11Y_DBUS_INTERFACE "org.a11y.Bus"
#define A11Y_DBUS_STATUS_INTERFACE "org.a11y.Status"
// TIZEN_ONLY(20170516): connect to at-spi dbus based on org.a11y.Status.IsEnabled property
#define A11Y_DBUS_ENABLED_PROPERTY "IsEnabled"
//
#define ATSPI_DBUS_INTERFACE_EVENT_WINDOW "org.a11y.atspi.Event.Window"

#define CACHE_ITEM_SIGNATURE "((so)(so)(so)a(so)assusau)"
#define CACHE_INTERFACE_PATH "/org/a11y/atspi/cache"

#define ELM_ACCESS_OBJECT_PATH_ROOT "root"
#define ELM_ACCESS_OBJECT_PATH_PREFIX  "/org/a11y/atspi/accessible/"
#define ELM_ACCESS_OBJECT_PATH_PREFIX2  "/org/a11y/atspi/accessible"
#define ELM_ACCESS_OBJECT_REFERENCE_TEMPLATE ELM_ACCESS_OBJECT_PATH_PREFIX "%llu"

//TIZEN_ONLY(20171108): make atspi_proxy work
#define ELM_ATSPI_DBUS_INTERFACE_PROXY "elm.atspi.bridge.proxy.Socket"
//

//TIZEN_ONLY(20160527) - Add direct reading feature
#define ELM_ATSPI_DIRECT_READ_BUS "org.tizen.ScreenReader"
#define ELM_ATSPI_DIRECT_READ_PATH "/org/tizen/DirectReading"
#define ELM_ATSPI_DIRECT_READ_INTERFACE "org.tizen.DirectReading"
struct _Elm_Atspi_Say_Info
{
   void                    *data;
   Elm_Atspi_Say_Signal_Cb  func; //this function will be called when state of related reading is changed
};
typedef struct _Elm_Atspi_Say_Info Elm_Atspi_Say_Info;
static Eina_Hash *read_command_id = NULL;
//

#define SIZE(x) sizeof(x)/sizeof(x[0])
#define ELM_ATSPI_BRIDGE_CLASS_NAME "__Elm_Atspi_Bridge"

#define ELM_ATSPI_BRIDGE_DATA_GET_OR_RETURN(obj, sd) \
   Elm_Atspi_Bridge_Data *sd = efl_data_scope_get(obj, ELM_ATSPI_BRIDGE_CLASS); \
   if (!sd) return;

#define ELM_ATSPI_BRIDGE_DATA_GET_OR_RETURN_VAL(obj, sd, val) \
   Elm_Atspi_Bridge_Data *sd = efl_data_scope_get(obj, ELM_ATSPI_BRIDGE_CLASS); \
   if (!sd) return val;

#define ELM_ATSPI_PROPERTY_OBJ_CHECK_OR_RETURN_DBUS_ERROR(obj, class, msg, error) \
   if (!(obj) || !efl_isa(obj, class)) \
     { \
        *(error) = _dbus_invalid_ref_error_new(msg); \
        return EINA_FALSE; \
     }

#define ELM_ATSPI_OBJ_CHECK_OR_RETURN_DBUS_ERROR(obj, class, msg) \
   if (!(obj) || !efl_isa(obj, class)) \
     return _dbus_invalid_ref_error_new(msg);

typedef struct Key_Event_Info {
     Ecore_Event_Key event;
     int type;
     Eo *bridge;
} Key_Event_Info;

typedef struct _Elm_Atspi_Bridge_Data
{
   Eldbus_Connection *session_bus;
   Eldbus_Connection *a11y_bus;
   Eina_List *reemited_events;
   Eina_Hash *cache;
   Eldbus_Service_Interface *cache_interface;
   Eldbus_Signal_Handler *register_hdl;
   Eldbus_Signal_Handler *unregister_hdl;
   //TIZEN_ONLY(20160614):apply callbacks on direct reading stop/cancel/skipp
   Eldbus_Signal_Handler *reading_state_changed_hdl;
   //
   unsigned long object_broadcast_mask;
   unsigned long object_property_broadcast_mask;
   unsigned long object_children_broadcast_mask;
   unsigned long long object_state_broadcast_mask;
   unsigned long long window_signal_broadcast_mask;
   Ecore_Event_Filter *key_flr;
   Eldbus_Object *bus_obj;
   Eina_List *pending_requests;
   int id;
   Eina_Hash *state_hash;
   struct {
        Eldbus_Service_Interface *accessible;
        Eldbus_Service_Interface *application;
        Eldbus_Service_Interface *action;
        Eldbus_Service_Interface *component;
        Eldbus_Service_Interface *collection;
        Eldbus_Service_Interface *editable_text;
        Eldbus_Service_Interface *image;
        Eldbus_Service_Interface *selection;
        Eldbus_Service_Interface *text;
        Eldbus_Service_Interface *value;
        //TIZEN_ONLY(20171108): make atspi_proxy work
        Eldbus_Service_Interface *socket;
        //
   } interfaces;
   Efl_Access_Event_Handler *event_hdlr;
   Eina_Hash *event_hash;
   //TIZEN_ONLY(20171108): make atspi_proxy work
   Eo *root;
   Eina_List *socket_queue;
   Eina_List *plug_queue;
   // TIZEN_ONLY(20171109) : fix for invalid proxy object, when at-spi has been restarted
   Eina_List *connected_plugs_sockets;
   //
   Evas_Point socket_offset;
   //
   Eina_Bool connected : 1;
   // TIZEN_ONLY(20160802): do not handle events if the window is not activated
   Eina_Bool window_activated : 1;
   //
   //TIZEN_ONLY(20161027) - Export elm_atspi_bridge_utils_is_screen_reader_enabled
   Eina_Bool screen_reader_enabled : 1;
   //
} Elm_Atspi_Bridge_Data;


struct collection_match_rule {
     Efl_Access_State_Set states;
     AtspiCollectionMatchType statematchtype;
     Eina_List *attributes;
     AtspiCollectionMatchType attributematchtype;
     uint64_t roles[2];
     AtspiCollectionMatchType rolematchtype;
     Eina_List *ifaces;
     AtspiCollectionMatchType interfacematchtype;
     Eina_Bool reverse : 1;
};

static Eo *_instance;
static int _init_count = 0;
//TIZEN_ONLY(20171108): make atspi_proxy work
static const char *_a11y_socket_address;
//

// Object Event handlers
static void _state_changed_signal_send(void *data, const Efl_Event *event);
static void _bounds_changed_signal_send(void *data, const Efl_Event *event);
static void _property_changed_signal_send(void *data, const Efl_Event *event);
static void _children_changed_signal_send(void *data, const Efl_Event *event);
static void _window_signal_send(void *data, const Efl_Event *event);
static void _visible_data_changed_signal_send(void *data, const Efl_Event *event);
static void _active_descendant_changed_signal_send(void *data, const Efl_Event *event);
static void _selection_signal_send(void *data, const Efl_Event *event);
static void _text_text_inserted_send(void *data, const Efl_Event *event);
static void _text_text_removed_send(void *data, const Efl_Event *event);
static void _text_caret_moved_send(void *data, const Efl_Event *event);
static void _text_selection_changed_send(void *data, const Efl_Event *event);
//TIZEN_ONLY(20160623): atspi: moved highlight when object is out of screen
static void _move_outed_signal_send(void *data, const Efl_Event *event);
//

// bridge private methods
//TIZEN_ONLY(20171108): make atspi_proxy work
static void _bridge_cache_build(Eo *bridge, void *obj);
//
static void _bridge_object_register(Eo *bridge, Eo *obj);
static void _bridge_object_unregister(Eo *bridge, Eo *obj);
static const char * _path_from_object(const Eo *eo);
static void _bridge_signal_send(Eo *bridge, Eo *obj, const char *ifc, const Eldbus_Signal *signal, const char *minor, unsigned int det1, unsigned int det2, const char *variant_sig, ...);
static Eo * _bridge_object_from_path(Eo *bridge, const char *path);
static void _bridge_iter_object_reference_append(Eo *bridge, Eldbus_Message_Iter *iter, const Eo *obj);
static void _object_get_bus_name_and_path(Eo *bridge, const Eo *obj, const char **bus_name, const char **path);
// TIZEN_ONLY(20170310) - implementation of get object under coordinates for accessibility
static Eo *_calculate_navigable_accessible_at_point(Eo *bridge, Eo *root, Eina_Bool coord_type, int x, int y);
static Eo *_calculate_neighbor(Eo *bridge, Eo *root, Eo *current, Eina_Bool forward, int search_mode);
//
// TIZEN_ONLY(20171109) : fix for invalid proxy object, when at-spi has been restarted
Eo *plug_type_proxy_get(Eo *obj, Evas_Object *widget);
//
// utility functions
static void _iter_interfaces_append(Eldbus_Message_Iter *iter, const Eo *obj);
static Eina_Bool _elm_atspi_bridge_key_filter(void *data, void *loop, int type, void *event);
static void _object_desktop_reference_append(Eldbus_Message_Iter *iter);
static void _on_object_add(void *data, const Efl_Event *event);
static void _on_object_del(void *data, const Efl_Event *event);

//TIZEN_ONLY(20171108): make atspi_proxy work
static void _plug_connect(Eldbus_Connection *conn, Eo *proxy);
static void _socket_ifc_create(Eldbus_Connection *conn, Eo *proxy);
//

typedef struct {
     const Efl_Event_Description *desc;
     const Efl_Event_Cb callback;
} Elm_Atspi_Bridge_Event_Handler;

static const Elm_Atspi_Bridge_Event_Handler event_handlers[] = {
   { EFL_ACCESS_OBJECT_EVENT_CHILDREN_CHANGED, _children_changed_signal_send},
   { EFL_ACCESS_OBJECT_EVENT_PROPERTY_CHANGED, _property_changed_signal_send},
   { EFL_ACCESS_OBJECT_EVENT_BOUNDS_CHANGED, _bounds_changed_signal_send},
   { EFL_ACCESS_OBJECT_EVENT_STATE_CHANGED, _state_changed_signal_send},
   { EFL_ACCESS_OBJECT_EVENT_VISIBLE_DATA_CHANGED, _visible_data_changed_signal_send},
   { EFL_ACCESS_OBJECT_EVENT_ACTIVE_DESCENDANT_CHANGED, _active_descendant_changed_signal_send},
   { EFL_ACCESS_OBJECT_EVENT_ADDED, _on_object_add},
   { EFL_ACCESS_OBJECT_EVENT_REMOVED, _on_object_del},
   { EFL_ACCESS_WINDOW_EVENT_WINDOW_CREATED, _window_signal_send},
   { EFL_ACCESS_WINDOW_EVENT_WINDOW_DESTROYED, _window_signal_send},
   { EFL_ACCESS_WINDOW_EVENT_WINDOW_ACTIVATED, _window_signal_send},
   { EFL_ACCESS_WINDOW_EVENT_WINDOW_DEACTIVATED, _window_signal_send},
   { EFL_ACCESS_WINDOW_EVENT_WINDOW_MAXIMIZED, _window_signal_send},
   { EFL_ACCESS_WINDOW_EVENT_WINDOW_MINIMIZED, _window_signal_send},
   { EFL_ACCESS_WINDOW_EVENT_WINDOW_RESTORED, _window_signal_send},
   { EFL_ACCESS_SELECTION_EVENT_SELECTION_CHANGED, _selection_signal_send},
   { EFL_ACCESS_TEXT_EVENT_ACCESS_TEXT_CARET_MOVED, _text_caret_moved_send },
   { EFL_ACCESS_TEXT_EVENT_ACCESS_TEXT_INSERTED, _text_text_inserted_send },
   { EFL_ACCESS_TEXT_EVENT_ACCESS_TEXT_REMOVED, _text_text_removed_send },
   { EFL_ACCESS_TEXT_EVENT_ACCESS_TEXT_SELECTION_CHANGED, _text_selection_changed_send },
   //TIZEN_ONLY(20160623): atspi: moved highlight when object is out of screen
   { EFL_ACCESS_OBJECT_EVENT_MOVE_OUTED, _move_outed_signal_send}
   //
};

//TIZEN_ONLY(20170925) atspi: send detail value for window activated signal
enum
{
  EFL_ACCESS_WINDOW_ACTIVATE_INFO_DEFAULT_LABEL_ENABLED = 0,
  EFL_ACCESS_WINDOW_ACTIVATE_INFO_DEFAULT_LABEL_ENABLED_WITHOUT_WINDOW = 1 << 0,
  EFL_ACCESS_WINDOW_ACTIVATE_INFO_DEFAULT_LABEL_DISABLED = 1 << 1,
  EFL_ACCESS_WINDOW_ACTIVATE_INFO_KEYBOARD = 1 << 2,
};
//

enum _Atspi_Object_Child_Event_Type
{
   ATSPI_OBJECT_CHILD_ADDED = 0,
   ATSPI_OBJECT_CHILD_REMOVED
};

enum _Atspi_Object_Property
{
   ATSPI_OBJECT_PROPERTY_NAME = 0,
   ATSPI_OBJECT_PROPERTY_DESCRIPTION,
   ATSPI_OBJECT_PROPERTY_VALUE,
   ATSPI_OBJECT_PROPERTY_ROLE,
   ATSPI_OBJECT_PROPERTY_PARENT,
   ATSPI_OBJECT_PROPERTY_LAST
};

enum _Atspi_Object_Signals {
   ATSPI_OBJECT_EVENT_PROPERTY_CHANGED = 0,
   ATSPI_OBJECT_EVENT_BOUNDS_CHANGED,
   ATSPI_OBJECT_EVENT_LINK_SELECTED,
   ATSPI_OBJECT_EVENT_STATE_CHANGED,
   ATSPI_OBJECT_EVENT_CHILDREN_CHANGED,
   ATSPI_OBJECT_EVENT_VISIBLE_DATA_CHANGED,
   ATSPI_OBJECT_EVENT_SELECTION_CHANGED,
   ATSPI_OBJECT_EVENT_MODEL_CHANGED,
   ATSPI_OBJECT_EVENT_ACTIVE_DESCENDANT_CHANGED,
   ATSPI_OBJECT_EVENT_ROW_INSERTED,
   ATSPI_OBJECT_EVENT_ROW_REORDERED,
   ATSPI_OBJECT_EVENT_ROW_DELETED,
   ATSPI_OBJECT_EVENT_COLUMN_INSERTED,
   ATSPI_OBJECT_EVENT_COLUMN_REORDERED,
   ATSPI_OBJECT_EVENT_COLUMN_DELETED,
   ATSPI_OBJECT_EVENT_TEXT_BOUNDS_CHANGED,
   ATSPI_OBJECT_EVENT_TEXT_SELECTION_CHANGED,
   ATSPI_OBJECT_EVENT_TEXT_CHANGED,
   ATSPI_OBJECT_EVENT_TEXT_ATTRIBUTES_CHANGED,
   ATSPI_OBJECT_EVENT_TEXT_CARET_MOVED,
   ATSPI_OBJECT_EVENT_ATTRIBUTES_CHANGED,
   //TIZEN_ONLY(20160623): atspi: moved highlight when object is out of screen
   ATSPI_OBJECT_EVENT_MOVE_OUTED
   //
};

enum _Atspi_Window_Signals
{
   ATSPI_WINDOW_EVENT_PROPERTY_CHANGE = 0,
   ATSPI_WINDOW_EVENT_MINIMIZE,
   ATSPI_WINDOW_EVENT_MAXIMIZE,
   ATSPI_WINDOW_EVENT_RESTORE,
   ATSPI_WINDOW_EVENT_CLOSE,
   ATSPI_WINDOW_EVENT_CREATE,
   ATSPI_WINDOW_EVENT_REPARENT,
   ATSPI_WINDOW_EVENT_DESKTOPCREATE,
   ATSPI_WINDOW_EVENT_DESKTOPDESTROY,
   ATSPI_WINDOW_EVENT_DESTROY,
   ATSPI_WINDOW_EVENT_ACTIVATE,
   ATSPI_WINDOW_EVENT_DEACTIVATE,
   ATSPI_WINDOW_EVENT_RAISE,
   ATSPI_WINDOW_EVENT_LOWER,
   ATSPI_WINDOW_EVENT_MOVE,
   ATSPI_WINDOW_EVENT_RESIZE,
   ATSPI_WINDOW_EVENT_SHADE,
   ATSPI_WINDOW_EVENT_UUSHADE,
   ATSPI_WINDOW_EVENT_RESTYLE,
};

static const Eldbus_Signal _event_obj_signals[] = {
   [ATSPI_OBJECT_EVENT_PROPERTY_CHANGED] = {"PropertyChange", ELDBUS_ARGS({"siiv(so)", NULL}), 0},
   [ATSPI_OBJECT_EVENT_BOUNDS_CHANGED] = {"BoundsChanged", ELDBUS_ARGS({"siiv(iiii)", NULL}), 0},
   [ATSPI_OBJECT_EVENT_LINK_SELECTED] = {"LinkSelected", ELDBUS_ARGS({"siiv(so)", NULL}), 0},
   [ATSPI_OBJECT_EVENT_STATE_CHANGED] = {"StateChanged", ELDBUS_ARGS({"siiv(so)", NULL}), 0},
   [ATSPI_OBJECT_EVENT_CHILDREN_CHANGED] = {"ChildrenChanged", ELDBUS_ARGS({"siiv(so)", NULL}), 0},
   [ATSPI_OBJECT_EVENT_VISIBLE_DATA_CHANGED] = {"VisibleDataChanged", ELDBUS_ARGS({"siiv(so)", NULL}), 0},
   [ATSPI_OBJECT_EVENT_SELECTION_CHANGED] = {"SelectionChanged", ELDBUS_ARGS({"siiv(so)", NULL}), 0},
   [ATSPI_OBJECT_EVENT_MODEL_CHANGED] = {"ModelChanged", ELDBUS_ARGS({"siiv(so)", NULL}), 0},
   [ATSPI_OBJECT_EVENT_ACTIVE_DESCENDANT_CHANGED] = {"ActiveDescendantChanged", ELDBUS_ARGS({"siiv(so)", NULL}), 0},
   [ATSPI_OBJECT_EVENT_ROW_INSERTED] = {"RowInserted", ELDBUS_ARGS({"siiv(so)", NULL}), 0},
   [ATSPI_OBJECT_EVENT_ROW_REORDERED] = {"RowReordered", ELDBUS_ARGS({"siiv(so)", NULL}), 0},
   [ATSPI_OBJECT_EVENT_ROW_DELETED] = {"RowDeleted", ELDBUS_ARGS({"siiv(so)", NULL}), 0},
   [ATSPI_OBJECT_EVENT_COLUMN_INSERTED] = {"ColumnInserted", ELDBUS_ARGS({"siiv(so)", NULL}), 0},
   [ATSPI_OBJECT_EVENT_COLUMN_REORDERED] = {"ColumnReordered", ELDBUS_ARGS({"siiv(so)", NULL}), 0},
   [ATSPI_OBJECT_EVENT_COLUMN_DELETED] = {"ColumnDeleted", ELDBUS_ARGS({"siiv(so)", NULL}), 0},
   [ATSPI_OBJECT_EVENT_TEXT_BOUNDS_CHANGED] = {"TextBoundsChanged", ELDBUS_ARGS({"siiv(so)", NULL}), 0},
   [ATSPI_OBJECT_EVENT_TEXT_SELECTION_CHANGED] = {"TextSelectionChanged", ELDBUS_ARGS({"siiv(so)", NULL}), 0},
   [ATSPI_OBJECT_EVENT_TEXT_CHANGED] = {"TextChanged", ELDBUS_ARGS({"siiv(so)", NULL}), 0},
   [ATSPI_OBJECT_EVENT_TEXT_ATTRIBUTES_CHANGED] = {"TextAttributesChanged", ELDBUS_ARGS({"siiv(so)", NULL}), 0},
   [ATSPI_OBJECT_EVENT_TEXT_CARET_MOVED] = {"TextCaretMoved", ELDBUS_ARGS({"siiv(so)", NULL}), 0},
   [ATSPI_OBJECT_EVENT_ATTRIBUTES_CHANGED] = {"AttributesChanged", ELDBUS_ARGS({"siiv(so)", NULL}), 0},
   //TIZEN_ONLY(20160623): atspi: moved highlight when object is out of screen
   [ATSPI_OBJECT_EVENT_MOVE_OUTED] = {"MoveOuted", ELDBUS_ARGS({"siiv(i)", NULL}), 0},
   //
   {NULL, ELDBUS_ARGS({NULL, NULL}), 0}
};

static const Eldbus_Signal _window_obj_signals[] = {
   [ATSPI_WINDOW_EVENT_PROPERTY_CHANGE] = {"PropertyChange", ELDBUS_ARGS({"siiv(so)", NULL}), 0},
   [ATSPI_WINDOW_EVENT_MINIMIZE] = {"Minimize", ELDBUS_ARGS({"siiv(so)", NULL}), 0},
   [ATSPI_WINDOW_EVENT_MAXIMIZE] = {"Maximize", ELDBUS_ARGS({"siiv(so)", NULL}), 0},
   [ATSPI_WINDOW_EVENT_RESTORE] = {"Restore", ELDBUS_ARGS({"siiv(so)", NULL}), 0},
   [ATSPI_WINDOW_EVENT_CLOSE] = {"Close", ELDBUS_ARGS({"siiv(so)", NULL}), 0},
   [ATSPI_WINDOW_EVENT_CREATE] = {"Create", ELDBUS_ARGS({"siiv(so)", NULL}), 0},
   [ATSPI_WINDOW_EVENT_REPARENT] = {"Reparent", ELDBUS_ARGS({"siiv(so)", NULL}), 0},
   [ATSPI_WINDOW_EVENT_DESKTOPCREATE] = {"DesktopCreate", ELDBUS_ARGS({"siiv(so)", NULL}), 0},
   [ATSPI_WINDOW_EVENT_DESKTOPDESTROY] = {"DesktopDestroy", ELDBUS_ARGS({"siiv(so)", NULL}), 0},
   [ATSPI_WINDOW_EVENT_DESTROY] = {"Destroy", ELDBUS_ARGS({"siiv(so)", NULL}), 0},
   [ATSPI_WINDOW_EVENT_ACTIVATE] = {"Activate", ELDBUS_ARGS({"siiv(so)", NULL}), 0},
   [ATSPI_WINDOW_EVENT_DEACTIVATE] = {"Deactivate", ELDBUS_ARGS({"siiv(so)", NULL}), 0},
   [ATSPI_WINDOW_EVENT_RAISE] = {"Raise", ELDBUS_ARGS({"siiv(so)", NULL}), 0},
   [ATSPI_WINDOW_EVENT_LOWER] = {"Lower", ELDBUS_ARGS({"siiv(so)", NULL}), 0},
   [ATSPI_WINDOW_EVENT_MOVE] = {"Move", ELDBUS_ARGS({"siiv(so)", NULL}), 0},
   [ATSPI_WINDOW_EVENT_RESIZE] = {"Resize", ELDBUS_ARGS({"siiv(so)", NULL}), 0},
   [ATSPI_WINDOW_EVENT_SHADE] = {"Shade", ELDBUS_ARGS({"siiv(so)", NULL}), 0},
   [ATSPI_WINDOW_EVENT_UUSHADE] = {"uUshade", ELDBUS_ARGS({"siiv(so)", NULL}), 0},
   [ATSPI_WINDOW_EVENT_RESTYLE] = {"Restyle", ELDBUS_ARGS({"siiv(so)", NULL}), 0},
   {NULL, ELDBUS_ARGS({NULL, NULL}), 0}
};

static const int elm_roles_to_atspi_roles[][2] = {
   { EFL_ACCESS_ROLE_INVALID, ATSPI_ROLE_INVALID },
   { EFL_ACCESS_ROLE_ACCELERATOR_LABEL, ATSPI_ROLE_ACCELERATOR_LABEL },
   { EFL_ACCESS_ROLE_ALERT, ATSPI_ROLE_ALERT },
   { EFL_ACCESS_ROLE_ANIMATION, ATSPI_ROLE_ANIMATION },
   { EFL_ACCESS_ROLE_ARROW, ATSPI_ROLE_ARROW },
   { EFL_ACCESS_ROLE_CALENDAR, ATSPI_ROLE_CALENDAR },
   { EFL_ACCESS_ROLE_CANVAS, ATSPI_ROLE_CANVAS },
   { EFL_ACCESS_ROLE_CHECK_BOX, ATSPI_ROLE_CHECK_BOX },
   { EFL_ACCESS_ROLE_CHECK_MENU_ITEM, ATSPI_ROLE_CHECK_MENU_ITEM },
   { EFL_ACCESS_ROLE_COLOR_CHOOSER, ATSPI_ROLE_COLOR_CHOOSER },
   { EFL_ACCESS_ROLE_COLUMN_HEADER, ATSPI_ROLE_COLUMN_HEADER },
   { EFL_ACCESS_ROLE_COMBO_BOX, ATSPI_ROLE_COMBO_BOX },
   { EFL_ACCESS_ROLE_DATE_EDITOR, ATSPI_ROLE_DATE_EDITOR },
   { EFL_ACCESS_ROLE_DESKTOP_ICON, ATSPI_ROLE_DESKTOP_ICON },
   { EFL_ACCESS_ROLE_DESKTOP_FRAME, ATSPI_ROLE_DESKTOP_FRAME },
   { EFL_ACCESS_ROLE_DIAL, ATSPI_ROLE_DIAL },
   { EFL_ACCESS_ROLE_DIALOG, ATSPI_ROLE_DIALOG },
   { EFL_ACCESS_ROLE_DIRECTORY_PANE, ATSPI_ROLE_DIRECTORY_PANE },
   { EFL_ACCESS_ROLE_DRAWING_AREA, ATSPI_ROLE_DRAWING_AREA },
   { EFL_ACCESS_ROLE_FILE_CHOOSER, ATSPI_ROLE_FILE_CHOOSER },
   { EFL_ACCESS_ROLE_FILLER, ATSPI_ROLE_FILLER },
   { EFL_ACCESS_ROLE_FOCUS_TRAVERSABLE, ATSPI_ROLE_FOCUS_TRAVERSABLE },
   { EFL_ACCESS_ROLE_FONT_CHOOSER, ATSPI_ROLE_FONT_CHOOSER },
   { EFL_ACCESS_ROLE_FRAME, ATSPI_ROLE_FRAME },
   { EFL_ACCESS_ROLE_GLASS_PANE, ATSPI_ROLE_GLASS_PANE },
   { EFL_ACCESS_ROLE_HTML_CONTAINER, ATSPI_ROLE_HTML_CONTAINER },
   { EFL_ACCESS_ROLE_ICON, ATSPI_ROLE_ICON },
   { EFL_ACCESS_ROLE_IMAGE, ATSPI_ROLE_IMAGE },
   { EFL_ACCESS_ROLE_INTERNAL_FRAME, ATSPI_ROLE_INTERNAL_FRAME },
   { EFL_ACCESS_ROLE_LABEL, ATSPI_ROLE_LABEL },
   { EFL_ACCESS_ROLE_LAYERED_PANE, ATSPI_ROLE_LAYERED_PANE },
   { EFL_ACCESS_ROLE_LIST, ATSPI_ROLE_LIST },
   { EFL_ACCESS_ROLE_LIST_ITEM, ATSPI_ROLE_LIST_ITEM },
   { EFL_ACCESS_ROLE_MENU, ATSPI_ROLE_MENU },
   { EFL_ACCESS_ROLE_MENU_BAR, ATSPI_ROLE_MENU_BAR },
   { EFL_ACCESS_ROLE_MENU_ITEM, ATSPI_ROLE_MENU_ITEM },
   { EFL_ACCESS_ROLE_OPTION_PANE, ATSPI_ROLE_OPTION_PANE },
   { EFL_ACCESS_ROLE_PAGE_TAB, ATSPI_ROLE_PAGE_TAB },
   { EFL_ACCESS_ROLE_PAGE_TAB_LIST, ATSPI_ROLE_PAGE_TAB_LIST },
   { EFL_ACCESS_ROLE_PANEL, ATSPI_ROLE_PANEL },
   { EFL_ACCESS_ROLE_PASSWORD_TEXT, ATSPI_ROLE_PASSWORD_TEXT },
   { EFL_ACCESS_ROLE_POPUP_MENU, ATSPI_ROLE_POPUP_MENU },
   { EFL_ACCESS_ROLE_PROGRESS_BAR, ATSPI_ROLE_PROGRESS_BAR },
   { EFL_ACCESS_ROLE_PUSH_BUTTON, ATSPI_ROLE_PUSH_BUTTON },
   { EFL_ACCESS_ROLE_RADIO_BUTTON, ATSPI_ROLE_RADIO_BUTTON },
   { EFL_ACCESS_ROLE_RADIO_MENU_ITEM, ATSPI_ROLE_RADIO_MENU_ITEM },
   { EFL_ACCESS_ROLE_ROOT_PANE, ATSPI_ROLE_ROOT_PANE },
   { EFL_ACCESS_ROLE_ROW_HEADER, ATSPI_ROLE_ROW_HEADER },
   { EFL_ACCESS_ROLE_SCROLL_BAR, ATSPI_ROLE_SCROLL_BAR },
   { EFL_ACCESS_ROLE_SCROLL_PANE, ATSPI_ROLE_SCROLL_PANE },
   { EFL_ACCESS_ROLE_SEPARATOR, ATSPI_ROLE_SEPARATOR },
   { EFL_ACCESS_ROLE_SLIDER, ATSPI_ROLE_SLIDER },
   { EFL_ACCESS_ROLE_SPIN_BUTTON, ATSPI_ROLE_SPIN_BUTTON },
   { EFL_ACCESS_ROLE_SPLIT_PANE, ATSPI_ROLE_SPLIT_PANE },
   { EFL_ACCESS_ROLE_STATUS_BAR, ATSPI_ROLE_STATUS_BAR },
   { EFL_ACCESS_ROLE_TABLE, ATSPI_ROLE_TABLE },
   { EFL_ACCESS_ROLE_TABLE_CELL, ATSPI_ROLE_TABLE_CELL },
   { EFL_ACCESS_ROLE_TABLE_COLUMN_HEADER, ATSPI_ROLE_TABLE_COLUMN_HEADER },
   { EFL_ACCESS_ROLE_TABLE_ROW_HEADER, ATSPI_ROLE_TABLE_ROW_HEADER },
   { EFL_ACCESS_ROLE_TEAROFF_MENU_ITEM, ATSPI_ROLE_TEAROFF_MENU_ITEM },
   { EFL_ACCESS_ROLE_TERMINAL, ATSPI_ROLE_TERMINAL },
   { EFL_ACCESS_ROLE_TEXT, ATSPI_ROLE_TEXT },
   { EFL_ACCESS_ROLE_TOGGLE_BUTTON, ATSPI_ROLE_TOGGLE_BUTTON },
   { EFL_ACCESS_ROLE_TOOL_BAR, ATSPI_ROLE_TOOL_BAR },
   { EFL_ACCESS_ROLE_TOOL_TIP, ATSPI_ROLE_TOOL_TIP },
   { EFL_ACCESS_ROLE_TREE, ATSPI_ROLE_TREE },
   { EFL_ACCESS_ROLE_TREE_TABLE, ATSPI_ROLE_TREE_TABLE },
   { EFL_ACCESS_ROLE_UNKNOWN, ATSPI_ROLE_UNKNOWN },
   { EFL_ACCESS_ROLE_VIEWPORT, ATSPI_ROLE_VIEWPORT },
   { EFL_ACCESS_ROLE_WINDOW, ATSPI_ROLE_WINDOW },
   { EFL_ACCESS_ROLE_EXTENDED, ATSPI_ROLE_EXTENDED },
   { EFL_ACCESS_ROLE_HEADER, ATSPI_ROLE_HEADER },
   { EFL_ACCESS_ROLE_FOOTER, ATSPI_ROLE_FOOTER },
   { EFL_ACCESS_ROLE_PARAGRAPH, ATSPI_ROLE_PARAGRAPH },
   { EFL_ACCESS_ROLE_RULER, ATSPI_ROLE_RULER },
   { EFL_ACCESS_ROLE_APPLICATION, ATSPI_ROLE_APPLICATION },
   { EFL_ACCESS_ROLE_AUTOCOMPLETE, ATSPI_ROLE_AUTOCOMPLETE },
   { EFL_ACCESS_ROLE_EDITBAR, ATSPI_ROLE_EDITBAR },
   { EFL_ACCESS_ROLE_EMBEDDED, ATSPI_ROLE_EMBEDDED },
   { EFL_ACCESS_ROLE_ENTRY, ATSPI_ROLE_ENTRY },
   { EFL_ACCESS_ROLE_CHART, ATSPI_ROLE_CHART },
   { EFL_ACCESS_ROLE_CAPTION, ATSPI_ROLE_CAPTION },
   { EFL_ACCESS_ROLE_DOCUMENT_FRAME, ATSPI_ROLE_DOCUMENT_FRAME },
   { EFL_ACCESS_ROLE_HEADING, ATSPI_ROLE_HEADING },
   { EFL_ACCESS_ROLE_PAGE, ATSPI_ROLE_PAGE },
   { EFL_ACCESS_ROLE_SECTION, ATSPI_ROLE_SECTION },
   { EFL_ACCESS_ROLE_REDUNDANT_OBJECT, ATSPI_ROLE_REDUNDANT_OBJECT },
   { EFL_ACCESS_ROLE_FORM, ATSPI_ROLE_FORM },
   { EFL_ACCESS_ROLE_LINK, ATSPI_ROLE_LINK },
   { EFL_ACCESS_ROLE_INPUT_METHOD_WINDOW, ATSPI_ROLE_INPUT_METHOD_WINDOW },
   { EFL_ACCESS_ROLE_TABLE_ROW, ATSPI_ROLE_TABLE_ROW },
   { EFL_ACCESS_ROLE_TREE_ITEM, ATSPI_ROLE_TREE_ITEM },
   { EFL_ACCESS_ROLE_DOCUMENT_SPREADSHEET, ATSPI_ROLE_DOCUMENT_SPREADSHEET },
   { EFL_ACCESS_ROLE_DOCUMENT_PRESENTATION, ATSPI_ROLE_DOCUMENT_PRESENTATION },
   { EFL_ACCESS_ROLE_DOCUMENT_TEXT, ATSPI_ROLE_DOCUMENT_TEXT },
   { EFL_ACCESS_ROLE_DOCUMENT_WEB, ATSPI_ROLE_DOCUMENT_WEB },
   { EFL_ACCESS_ROLE_DOCUMENT_EMAIL, ATSPI_ROLE_DOCUMENT_EMAIL },
   { EFL_ACCESS_ROLE_COMMENT, ATSPI_ROLE_COMMENT },
   { EFL_ACCESS_ROLE_LIST_BOX, ATSPI_ROLE_LIST_BOX },
   { EFL_ACCESS_ROLE_GROUPING, ATSPI_ROLE_GROUPING },
   { EFL_ACCESS_ROLE_IMAGE_MAP, ATSPI_ROLE_IMAGE_MAP },
   { EFL_ACCESS_ROLE_NOTIFICATION, ATSPI_ROLE_NOTIFICATION },
   { EFL_ACCESS_ROLE_INFO_BAR, ATSPI_ROLE_INFO_BAR },
   { EFL_ACCESS_ROLE_LAST_DEFINED, ATSPI_ROLE_LAST_DEFINED },
};

struct atspi_state_desc
{
   Efl_Access_State_Type elm_state;
   AtspiStateType atspi_state;
   const char *name;
};

static const struct atspi_state_desc elm_states_to_atspi_state[] = {
   { EFL_ACCESS_STATE_INVALID, ATSPI_STATE_INVALID, "invalid" },
   { EFL_ACCESS_STATE_ACTIVE, ATSPI_STATE_ACTIVE, "active" },
   { EFL_ACCESS_STATE_ARMED, ATSPI_STATE_ARMED, "armed" },
   { EFL_ACCESS_STATE_BUSY, ATSPI_STATE_BUSY, "busy" },
   { EFL_ACCESS_STATE_CHECKED, ATSPI_STATE_CHECKED, "checked" },
   { EFL_ACCESS_STATE_COLLAPSED, ATSPI_STATE_COLLAPSED, "collapsed" },
   { EFL_ACCESS_STATE_DEFUNCT, ATSPI_STATE_DEFUNCT, "defunct" },
   { EFL_ACCESS_STATE_EDITABLE, ATSPI_STATE_EDITABLE, "editable" },
   { EFL_ACCESS_STATE_ENABLED, ATSPI_STATE_ENABLED, "enabled" },
   { EFL_ACCESS_STATE_EXPANDABLE, ATSPI_STATE_EXPANDABLE, "expandable" },
   { EFL_ACCESS_STATE_EXPANDED, ATSPI_STATE_EXPANDED, "expanded" },
   { EFL_ACCESS_STATE_FOCUSABLE, ATSPI_STATE_FOCUSABLE, "focusable" },
   { EFL_ACCESS_STATE_FOCUSED, ATSPI_STATE_FOCUSED, "focused" },
   { EFL_ACCESS_STATE_HAS_TOOLTIP, ATSPI_STATE_HAS_TOOLTIP, "has-tooltip" },
   { EFL_ACCESS_STATE_HORIZONTAL, ATSPI_STATE_HORIZONTAL, "horizontal" },
   { EFL_ACCESS_STATE_ICONIFIED, ATSPI_STATE_ICONIFIED, "iconified" },
   { EFL_ACCESS_STATE_MODAL, ATSPI_STATE_MODAL, "modal" },
   { EFL_ACCESS_STATE_MULTI_LINE, ATSPI_STATE_MULTI_LINE, "multi-line" },
   { EFL_ACCESS_STATE_MULTISELECTABLE, ATSPI_STATE_MULTISELECTABLE, "multiselectable" },
   { EFL_ACCESS_STATE_OPAQUE, ATSPI_STATE_OPAQUE, "opaque" },
   { EFL_ACCESS_STATE_PRESSED, ATSPI_STATE_PRESSED, "pressed" },
   { EFL_ACCESS_STATE_RESIZABLE, ATSPI_STATE_RESIZABLE, "resizable" },
   { EFL_ACCESS_STATE_SELECTABLE, ATSPI_STATE_SELECTABLE, "selectable" },
   { EFL_ACCESS_STATE_SELECTED, ATSPI_STATE_SELECTED, "selected" },
   { EFL_ACCESS_STATE_SENSITIVE, ATSPI_STATE_SENSITIVE, "sensitive" },
   { EFL_ACCESS_STATE_SHOWING, ATSPI_STATE_SHOWING, "showing" },
   { EFL_ACCESS_STATE_SINGLE_LINE, ATSPI_STATE_SINGLE_LINE, "single-line" },
   { EFL_ACCESS_STATE_STALE, ATSPI_STATE_STALE, "stale" },
   { EFL_ACCESS_STATE_TRANSIENT, ATSPI_STATE_TRANSIENT, "transient" },
   { EFL_ACCESS_STATE_VERTICAL, ATSPI_STATE_VERTICAL, "vertical" },
   { EFL_ACCESS_STATE_VISIBLE, ATSPI_STATE_VISIBLE, "visible" },
   { EFL_ACCESS_STATE_MANAGES_DESCENDANTS, ATSPI_STATE_MANAGES_DESCENDANTS, "manages-descendants" },
   { EFL_ACCESS_STATE_INDETERMINATE, ATSPI_STATE_INDETERMINATE, "indeterminate" },
   { EFL_ACCESS_STATE_REQUIRED, ATSPI_STATE_REQUIRED, "required" },
   { EFL_ACCESS_STATE_TRUNCATED, ATSPI_STATE_TRUNCATED, "truncated" },
   { EFL_ACCESS_STATE_ANIMATED, ATSPI_STATE_ANIMATED, "animated" },
   { EFL_ACCESS_STATE_INVALID_ENTRY, ATSPI_STATE_INVALID_ENTRY, "invalid-entry" },
   { EFL_ACCESS_STATE_SUPPORTS_AUTOCOMPLETION, ATSPI_STATE_SUPPORTS_AUTOCOMPLETION, "supports-autocompletion" },
   { EFL_ACCESS_STATE_SELECTABLE_TEXT, ATSPI_STATE_SELECTABLE_TEXT, "selectable-text" },
   { EFL_ACCESS_STATE_IS_DEFAULT, ATSPI_STATE_IS_DEFAULT, "is-default" },
   { EFL_ACCESS_STATE_VISITED, ATSPI_STATE_VISITED, "visited" },
   { EFL_ACCESS_STATE_CHECKABLE, ATSPI_STATE_CHECKABLE, "checkable" },
   { EFL_ACCESS_STATE_HAS_POPUP, ATSPI_STATE_HAS_POPUP, "has-popup" },
   { EFL_ACCESS_STATE_READ_ONLY, ATSPI_STATE_READ_ONLY, "read-only" },
   { EFL_ACCESS_STATE_HIGHLIGHTED, ATSPI_STATE_HIGHLIGHTED, "highlighted" },
   { EFL_ACCESS_STATE_HIGHLIGHTABLE, ATSPI_STATE_HIGHLIGHTABLE, "highlightable" },
   { EFL_ACCESS_STATE_LAST_DEFINED, ATSPI_STATE_LAST_DEFINED, "last-defined" },
};

static const int elm_relation_to_atspi_relation_mapping[] = {
   [EFL_ACCESS_RELATION_NULL] =  ATSPI_RELATION_NULL,
   [EFL_ACCESS_RELATION_LABEL_FOR] =  ATSPI_RELATION_LABEL_FOR,
   [EFL_ACCESS_RELATION_LABELLED_BY] = ATSPI_RELATION_LABELLED_BY,
   [EFL_ACCESS_RELATION_CONTROLLER_FOR] = ATSPI_RELATION_CONTROLLER_FOR,
   [EFL_ACCESS_RELATION_CONTROLLED_BY] = ATSPI_RELATION_CONTROLLED_BY,
   [EFL_ACCESS_RELATION_MEMBER_OF] = ATSPI_RELATION_MEMBER_OF,
   [EFL_ACCESS_RELATION_TOOLTIP_FOR] = ATSPI_RELATION_TOOLTIP_FOR,
   [EFL_ACCESS_RELATION_NODE_CHILD_OF] = ATSPI_RELATION_NODE_CHILD_OF,
   [EFL_ACCESS_RELATION_NODE_PARENT_OF] = ATSPI_RELATION_NODE_PARENT_OF,
   [EFL_ACCESS_RELATION_EXTENDED] = ATSPI_RELATION_EXTENDED,
   [EFL_ACCESS_RELATION_FLOWS_TO] = ATSPI_RELATION_FLOWS_TO,
   [EFL_ACCESS_RELATION_FLOWS_FROM] = ATSPI_RELATION_FLOWS_FROM,
   [EFL_ACCESS_RELATION_SUBWINDOW_OF] = ATSPI_RELATION_SUBWINDOW_OF,
   [EFL_ACCESS_RELATION_EMBEDS] = ATSPI_RELATION_EMBEDS,
   [EFL_ACCESS_RELATION_EMBEDDED_BY] = ATSPI_RELATION_EMBEDDED_BY,
   [EFL_ACCESS_RELATION_POPUP_FOR] = ATSPI_RELATION_POPUP_FOR,
   [EFL_ACCESS_RELATION_PARENT_WINDOW_OF] = ATSPI_RELATION_PARENT_WINDOW_OF,
   [EFL_ACCESS_RELATION_DESCRIPTION_FOR] = ATSPI_RELATION_DESCRIPTION_FOR,
   [EFL_ACCESS_RELATION_DESCRIBED_BY] = ATSPI_RELATION_DESCRIBED_BY,
   [EFL_ACCESS_RELATION_LAST_DEFINED] = ATSPI_RELATION_LAST_DEFINED,
};

static inline Eldbus_Message *_dbus_invalid_ref_error_new(const Eldbus_Message *msg)
{
  return eldbus_message_error_new(msg, "org.freedesktop.DBus.Error.UnknownObject", "Path is not valid accessible object reference.");
}

static AtspiRelationType _elm_relation_to_atspi_relation(Efl_Access_Relation_Type type)
{
   if ((type < EFL_ACCESS_RELATION_LAST_DEFINED) && (type > EFL_ACCESS_RELATION_NULL))
     return elm_relation_to_atspi_relation_mapping[type];
   return ATSPI_RELATION_NULL;
}

static Efl_Access_Relation_Type _atspi_relation_to_elm_relation(AtspiRelationType type)
{
   unsigned int i;
   for(i = 0; i < sizeof(elm_relation_to_atspi_relation_mapping) / sizeof(elm_relation_to_atspi_relation_mapping[0]); ++i)
     {
       if (elm_relation_to_atspi_relation_mapping[i] == (int)type) return (Elm_Atspi_Relation_Type)i;
     }
   return EFL_ACCESS_RELATION_NULL;
}

static AtspiRole _efl_role_to_atspi_role(Efl_Access_Role role)
{
   return role > EFL_ACCESS_ROLE_LAST_DEFINED ? ATSPI_ROLE_LAST_DEFINED : elm_roles_to_atspi_roles[role][1];
}

static Eldbus_Message *
_accessible_get_role(const Eldbus_Service_Interface *iface, const Eldbus_Message *msg)
{
   const char *obj_path = eldbus_message_path_get(msg);
   Eo *bridge = eldbus_service_object_data_get(iface, ELM_ATSPI_BRIDGE_CLASS_NAME);
   Eo *obj = _bridge_object_from_path(bridge, obj_path);
   Efl_Access_Role role;

   ELM_ATSPI_OBJ_CHECK_OR_RETURN_DBUS_ERROR(obj, EFL_ACCESS_OBJECT_MIXIN, msg);

   role = efl_access_object_role_get(obj);

   Eldbus_Message *ret = eldbus_message_method_return_new(msg);
   EINA_SAFETY_ON_NULL_RETURN_VAL(ret, NULL);

   AtspiRole atspi_role = _efl_role_to_atspi_role(role);
   eldbus_message_arguments_append(ret, "u", atspi_role);
   return ret;
}

static Eldbus_Message *
_accessible_get_role_name(const Eldbus_Service_Interface *iface, const Eldbus_Message *msg)
{
   const char *role_name = NULL, *obj_path = eldbus_message_path_get(msg);
   Eo *bridge = eldbus_service_object_data_get(iface, ELM_ATSPI_BRIDGE_CLASS_NAME);
   Eo *obj = _bridge_object_from_path(bridge, obj_path);

   ELM_ATSPI_OBJ_CHECK_OR_RETURN_DBUS_ERROR(obj, EFL_ACCESS_OBJECT_MIXIN, msg);

   role_name = efl_access_object_role_name_get(obj);

   Eldbus_Message *ret = eldbus_message_method_return_new(msg);
   EINA_SAFETY_ON_NULL_RETURN_VAL(ret, NULL);
   eldbus_message_arguments_append(ret, "s", role_name);

   return ret;
}

static Eldbus_Message *
_accessible_get_localized_role_name(const Eldbus_Service_Interface *iface, const Eldbus_Message *msg)
{
   const char *l_role_name = NULL, *obj_path = eldbus_message_path_get(msg);
   Eo *bridge = eldbus_service_object_data_get(iface, ELM_ATSPI_BRIDGE_CLASS_NAME);
   Eo *obj = _bridge_object_from_path(bridge, obj_path);

   ELM_ATSPI_OBJ_CHECK_OR_RETURN_DBUS_ERROR(obj, EFL_ACCESS_OBJECT_MIXIN, msg);

   l_role_name = efl_access_object_localized_role_name_get(obj);
   EINA_SAFETY_ON_NULL_RETURN_VAL(l_role_name, NULL);

   Eldbus_Message *ret = eldbus_message_method_return_new(msg);
   EINA_SAFETY_ON_NULL_RETURN_VAL(ret, NULL);
   eldbus_message_arguments_append(ret, "s", l_role_name);

   return ret;
}

static Eldbus_Message *
_accessible_get_children(const Eldbus_Service_Interface *iface, const Eldbus_Message *msg)
{
   const char *obj_path = eldbus_message_path_get(msg);
   Eo *bridge = eldbus_service_object_data_get(iface, ELM_ATSPI_BRIDGE_CLASS_NAME);
   Eo *obj = _bridge_object_from_path(bridge, obj_path);
   Eina_List *children_list = NULL, *l;
   Eldbus_Message *ret;
   Eldbus_Message_Iter *iter, *iter_array;
   Eo *children;

   ELM_ATSPI_OBJ_CHECK_OR_RETURN_DBUS_ERROR(obj, EFL_ACCESS_OBJECT_MIXIN, msg);

   children_list = efl_access_object_access_children_get(obj);

   ret = eldbus_message_method_return_new(msg);
   EINA_SAFETY_ON_NULL_RETURN_VAL(ret, NULL);

   iter = eldbus_message_iter_get(ret);
   iter_array = eldbus_message_iter_container_new(iter, 'a', "(so)");
   EINA_SAFETY_ON_NULL_GOTO(iter_array, fail);

   EINA_LIST_FOREACH(children_list, l, children)
     {
        _bridge_iter_object_reference_append(bridge, iter_array, children);
        _bridge_object_register(bridge, children);
     }

   eldbus_message_iter_container_close(iter, iter_array);
   eina_list_free(children_list);

   return ret;

fail:
   if (ret) eldbus_message_unref(ret);
   return NULL;
}

static Eldbus_Message *
_accessible_get_application(const Eldbus_Service_Interface *iface, const Eldbus_Message *msg)
{
   Eldbus_Message *ret;
   const char *obj_path = eldbus_message_path_get(msg);
   Eo *bridge = eldbus_service_object_data_get(iface, ELM_ATSPI_BRIDGE_CLASS_NAME);
   Eo *root, *obj = _bridge_object_from_path(bridge, obj_path);

   ELM_ATSPI_OBJ_CHECK_OR_RETURN_DBUS_ERROR(obj, EFL_ACCESS_OBJECT_MIXIN, msg);

   ret = eldbus_message_method_return_new(msg);
   EINA_SAFETY_ON_NULL_RETURN_VAL(ret, NULL);

   Eldbus_Message_Iter *iter = eldbus_message_iter_get(ret);
   root = efl_access_object_access_root_get(EFL_ACCESS_OBJECT_MIXIN);
   _bridge_iter_object_reference_append(bridge, iter, root);

   return ret;
}

static Eldbus_Message *
_accessible_attributes_get(const Eldbus_Service_Interface *iface, const Eldbus_Message *msg)
{
   Eina_List *attrs = NULL, *l;
   Efl_Access_Attribute *attr;
   Eldbus_Message_Iter *iter, *iter_dict = NULL, *iter_entry;
   Eldbus_Message *ret;

   const char *obj_path = eldbus_message_path_get(msg);
   Eo *bridge = eldbus_service_object_data_get(iface, ELM_ATSPI_BRIDGE_CLASS_NAME);
   Eo *obj = _bridge_object_from_path(bridge, obj_path);

   ELM_ATSPI_OBJ_CHECK_OR_RETURN_DBUS_ERROR(obj, EFL_ACCESS_OBJECT_MIXIN, msg);

   ret = eldbus_message_method_return_new(msg);
   if (!ret) goto error;

   attrs = efl_access_object_attributes_get(obj);

   iter = eldbus_message_iter_get(ret);
   if (!iter) goto error;

   iter_dict = eldbus_message_iter_container_new(iter, 'a', "{ss}");
   if (!iter_dict) goto error;


   EINA_LIST_FOREACH(attrs, l, attr)
     {
        iter_entry = eldbus_message_iter_container_new(iter_dict, 'e', NULL);
        if (!iter_entry) goto error;
        eldbus_message_iter_arguments_append(iter_entry, "ss", attr->key, attr->value);
        eldbus_message_iter_container_close(iter_dict, iter_entry);
     }

   eldbus_message_iter_container_close(iter, iter_dict);
   efl_access_attributes_list_free(attrs);
   return ret;

error:
   if (iter_dict) eldbus_message_iter_container_close(iter, iter_dict);
   if (ret) eldbus_message_unref(ret);
   efl_access_attributes_list_free(attrs);
   return NULL;
}

static Eldbus_Message *
_accessible_interfaces_get(const Eldbus_Service_Interface *iface, const Eldbus_Message *msg)
{
   Eldbus_Message *ret;
   Eldbus_Message_Iter *iter;
   const char *obj_path = eldbus_message_path_get(msg);
   Eo *bridge = eldbus_service_object_data_get(iface, ELM_ATSPI_BRIDGE_CLASS_NAME);
   Eo *obj = _bridge_object_from_path(bridge, obj_path);

   ELM_ATSPI_OBJ_CHECK_OR_RETURN_DBUS_ERROR(obj, EFL_ACCESS_OBJECT_MIXIN, msg);

   ret = eldbus_message_method_return_new(msg);
   EINA_SAFETY_ON_NULL_RETURN_VAL(ret, NULL);

   iter = eldbus_message_iter_get(ret);
   _iter_interfaces_append(iter, obj);

   return ret;
}

static uint64_t
_elm_atspi_state_set_to_atspi_state_set(Efl_Access_State_Set states)
{
   uint64_t ret = 0;
   unsigned int i = 0;

   for (i = 0; i < SIZE(elm_states_to_atspi_state); i++)
     {
        if (STATE_TYPE_GET(states, elm_states_to_atspi_state[i].elm_state))
          STATE_TYPE_SET(ret, elm_states_to_atspi_state[i].atspi_state);
     }
   return ret;
}

static Efl_Access_State_Set
_atspi_state_set_to_elm_atspi_state_set(uint64_t states)
{
   //Currently Elm_Atspi_State and Atspi_State_Set are binary compatible,
   //implement proper coversion when it will be needed.
   Efl_Access_State_Set ret = states;
   return ret;
}

static Eina_Hash*
_elm_atspi_state_hash_build(void)
{
   Eina_Hash *ret = eina_hash_string_superfast_new(NULL);
   unsigned int i = 0;

   for (i = 0; i < SIZE(elm_states_to_atspi_state); i++)
     eina_hash_add(ret, elm_states_to_atspi_state[i].name, &elm_states_to_atspi_state[i]);

   return ret;
}

static Eina_Hash*
_elm_atspi_event_hash_build(void)
{
   Eina_Hash *ret = eina_hash_pointer_new(NULL);
   unsigned int i = 0;

   for (i = 0; i < SIZE(event_handlers); i++)
     eina_hash_add(ret, &(event_handlers[i].desc), event_handlers[i].callback);

   return ret;
}

static Eldbus_Message *
_accessible_get_state(const Eldbus_Service_Interface *iface, const Eldbus_Message *msg)
{
   Eldbus_Message *ret;
   Eldbus_Message_Iter *iter, *iter_array;
   Efl_Access_State_Set states;
   uint64_t atspi_states = 0;

   const char *obj_path = eldbus_message_path_get(msg);
   Eo *bridge = eldbus_service_object_data_get(iface, ELM_ATSPI_BRIDGE_CLASS_NAME);
   Eo *obj = _bridge_object_from_path(bridge, obj_path);

   ELM_ATSPI_OBJ_CHECK_OR_RETURN_DBUS_ERROR(obj, EFL_ACCESS_OBJECT_MIXIN, msg);

   ret = eldbus_message_method_return_new(msg);
   EINA_SAFETY_ON_NULL_RETURN_VAL(ret, NULL);

   iter = eldbus_message_iter_get(ret);
   iter_array = eldbus_message_iter_container_new(iter, 'a', "u");
   EINA_SAFETY_ON_NULL_GOTO(iter_array, fail);

   states = efl_access_object_state_set_get(obj);

   atspi_states = _elm_atspi_state_set_to_atspi_state_set(states);

   unsigned int s1 = atspi_states & 0xFFFFFFFF;
   unsigned int s2 = (atspi_states >> 32) & 0xFFFFFFFF;

   eldbus_message_iter_basic_append(iter_array, 'u', s1);
   eldbus_message_iter_basic_append(iter_array, 'u', s2);
   eldbus_message_iter_container_close(iter, iter_array);

   return ret;

fail:
   if (ret) eldbus_message_unref(ret);
   return NULL;
}

static Eldbus_Message *
_accessible_get_index_in_parent(const Eldbus_Service_Interface *iface EINA_UNUSED, const Eldbus_Message *msg)
{
   const char *obj_path = eldbus_message_path_get(msg);
   Eo *bridge = eldbus_service_object_data_get(iface, ELM_ATSPI_BRIDGE_CLASS_NAME);
   Eo *obj = _bridge_object_from_path(bridge, obj_path);
   Eldbus_Message *ret;
   int idx = -1;

   ELM_ATSPI_OBJ_CHECK_OR_RETURN_DBUS_ERROR(obj, EFL_ACCESS_OBJECT_MIXIN, msg);

   ret = eldbus_message_method_return_new(msg);
   EINA_SAFETY_ON_NULL_RETURN_VAL(ret, NULL);

   idx = efl_access_object_index_in_parent_get(obj);

   eldbus_message_arguments_append(ret, "i", idx);

   return ret;
}

static Eldbus_Message *
_accessible_child_at_index(const Eldbus_Service_Interface *iface EINA_UNUSED, const Eldbus_Message *msg)
{
   const char *obj_path = eldbus_message_path_get(msg);
   Eo *bridge = eldbus_service_object_data_get(iface, ELM_ATSPI_BRIDGE_CLASS_NAME);
   Eo *obj = _bridge_object_from_path(bridge, obj_path);
   Eo *child = NULL;
   Eina_List *children = NULL;
   int idx;
   Eldbus_Message *ret;
   Eldbus_Message_Iter *iter;

   ELM_ATSPI_OBJ_CHECK_OR_RETURN_DBUS_ERROR(obj, EFL_ACCESS_OBJECT_MIXIN, msg);

   if (!eldbus_message_arguments_get(msg, "i", &idx))
     return eldbus_message_error_new(msg, "org.freedesktop.DBus.Error.InvalidArgs", "Invalid index type.");

   ret = eldbus_message_method_return_new(msg);
   EINA_SAFETY_ON_NULL_RETURN_VAL(ret, NULL);

   iter = eldbus_message_iter_get(ret);
   children = efl_access_object_access_children_get(obj);

   child = eina_list_nth(children, idx);
   _bridge_iter_object_reference_append(bridge, iter, child);
   _bridge_object_register(bridge, child);
   eina_list_free(children);

   return ret;
}

static Eldbus_Message *
_accessible_get_relation_set(const Eldbus_Service_Interface *iface EINA_UNUSED, const Eldbus_Message *msg)
{
   const char *obj_path = eldbus_message_path_get(msg);
   Eo *bridge = eldbus_service_object_data_get(iface, ELM_ATSPI_BRIDGE_CLASS_NAME);
   Eo *rel_obj, *obj = _bridge_object_from_path(bridge, obj_path);
   Eldbus_Message *ret = NULL;
   Eldbus_Message_Iter *iter = NULL, *iter_array = NULL, *iter_array2 = NULL, *iter_struct;
   Efl_Access_Relation *rel;
   Eina_List *l, *l2;
   Efl_Access_Relation_Set rels;

   ELM_ATSPI_OBJ_CHECK_OR_RETURN_DBUS_ERROR(obj, EFL_ACCESS_OBJECT_MIXIN, msg);

   ret = eldbus_message_method_return_new(msg);
   EINA_SAFETY_ON_NULL_RETURN_VAL(ret, NULL);

   iter = eldbus_message_iter_get(ret);
   iter_array = eldbus_message_iter_container_new(iter, 'a', "(ua(so))");
   EINA_SAFETY_ON_NULL_GOTO(iter_array, fail);

   rels = efl_access_object_relation_set_get(obj);

   EINA_LIST_FOREACH(rels, l, rel)
     {
        iter_struct = eldbus_message_iter_container_new(iter_array, 'r', NULL);
        eldbus_message_iter_basic_append(iter_struct, 'u', _elm_relation_to_atspi_relation(rel->type));
        iter_array2 = eldbus_message_iter_container_new(iter_struct, 'a', "(so)");
        EINA_SAFETY_ON_NULL_GOTO(iter_array2, fail);
        EINA_LIST_FOREACH(rel->objects, l2, rel_obj)
          {
             _bridge_iter_object_reference_append(bridge, iter_array2, rel_obj);
             _bridge_object_register(bridge, rel_obj);
          }
        eldbus_message_iter_container_close(iter_struct, iter_array2);
        eldbus_message_iter_container_close(iter_array, iter_struct);
     }
   //TIZEN_ONLY(20171115) Fixed the bugs and warnings in atspi relationship APIS
   efl_access_relation_set_free(&rels);
   //
   eldbus_message_iter_container_close(iter, iter_array);

   return ret;

fail:
   eldbus_message_unref(ret);
   return eldbus_message_error_new(msg, "org.freedesktop.DBus.Error.Failed", "Unable to get relation set.");
}

//TIZEN_ONLY(20170405) Add gesture method to accessible interface
static Eldbus_Message *
_accessible_gesture_do(const Eldbus_Service_Interface *iface, const Eldbus_Message *msg)
{
   const char *obj_path = eldbus_message_path_get(msg);
   Eo *bridge = eldbus_service_object_data_get(iface, ELM_ATSPI_BRIDGE_CLASS_NAME);
   Eo *obj = _bridge_object_from_path(bridge, obj_path);
   int type, x_beg, y_beg, x_end, y_end, state;
   unsigned int event_time;
   Eldbus_Message *ret;
   Eina_Bool result = EINA_FALSE;

   if (!eldbus_message_arguments_get(msg, "iiiiiiu", &type, &x_beg, &y_beg,
                                     &x_end, &y_end, &state, &event_time))
     return eldbus_message_error_new(msg, "org.freedesktop.DBus.Error.InvalidArgs", "Invalid index type.");

   ret = eldbus_message_method_return_new(msg);
   EINA_SAFETY_ON_NULL_RETURN_VAL(ret, NULL);

   Efl_Access_Gesture_Info gesture_info;
   gesture_info.type = (Efl_Access_Gesture)type;
   gesture_info.x_beg = x_beg;
   gesture_info.y_beg = y_beg;
   gesture_info.x_end = x_end;
   gesture_info.y_end = y_end;
   gesture_info.state = (Efl_Access_Gesture_State)state;
   gesture_info.event_time = event_time;
   result = efl_access_object_gesture_do(obj, gesture_info);
   eldbus_message_arguments_append(ret, "b", result);

   return ret;
}
//

static Eina_Bool
_ee_obscured_get(Eo *obj)
{
   const Ecore_Evas *ee;
   if (efl_isa(obj, ELM_WIDGET_ITEM_CLASS))
     {
        Elm_Widget_Item_Data *id = efl_data_scope_get(obj, ELM_WIDGET_ITEM_CLASS);
        ee = ecore_evas_ecore_evas_get(evas_object_evas_get(id->view));
     }
   else
     {
        ee = ecore_evas_ecore_evas_get(evas_object_evas_get(obj));
     }

   return ecore_evas_obscured_get(ee);
}

// TIZEN_ONLY(20170310) - implementation of get object under coordinates for accessibility
static Eldbus_Message *
_accessible_get_neighbor(const Eldbus_Service_Interface *iface EINA_UNUSED, const Eldbus_Message *msg)
{
   const char *start_path = eldbus_message_path_get(msg);
   Eo *bridge = eldbus_service_object_data_get(iface, ELM_ATSPI_BRIDGE_CLASS_NAME);
   // TIZEN_ONLY(20160705) - enable atspi_proxy to work
   ELM_ATSPI_BRIDGE_DATA_GET_OR_RETURN_VAL(bridge, pd, NULL);
   //
   Eo *start = _bridge_object_from_path(bridge, start_path);
   Eldbus_Message *ret;
   Eldbus_Message_Iter *iter;
   int direction, search_mode;

   ELM_ATSPI_OBJ_CHECK_OR_RETURN_DBUS_ERROR(start, EFL_ACCESS_OBJECT_MIXIN, msg);

   char *root_path = "";
   if (!eldbus_message_arguments_get(msg, "sii", &root_path, &direction, &search_mode))
     return eldbus_message_error_new(msg, "org.freedesktop.DBus.Error.InvalidArgs", "Invalid index type.");

   Eo *root = _bridge_object_from_path(bridge, root_path);

   // TIZEN_ONLY(20161213) - do not response if ecore evas is obscured
   if (root)
     {
        if (_ee_obscured_get(root))
          return eldbus_message_error_new(msg, "org.freedesktop.DBus.Error.Failed", "ecore evas is obscured.");
     }
   //

   ret = eldbus_message_method_return_new(msg);
   EINA_SAFETY_ON_NULL_RETURN_VAL(ret, NULL);

   iter = eldbus_message_iter_get(ret);

   Eo *accessible = _calculate_neighbor(bridge, root, start, direction == 1, search_mode);
   _bridge_iter_object_reference_append(bridge, iter, accessible);
   _bridge_object_register(bridge, accessible);

   const char *obj_bus_name = NULL, *ret_bus_name = NULL;
   _object_get_bus_name_and_path(bridge, start, &obj_bus_name, NULL);
   if (accessible) _object_get_bus_name_and_path(bridge, accessible, &ret_bus_name, NULL);

   unsigned char recurse = obj_bus_name && ret_bus_name && strcmp(obj_bus_name, ret_bus_name) != 0;
   eldbus_message_iter_basic_append(iter, 'y', recurse);
   return ret;
}

// TIZEN_ONLY(20170310) - implementation of get object under coordinates for accessibility
static Eldbus_Message *
_accessible_get_navigable_at_point(const Eldbus_Service_Interface *iface EINA_UNUSED, const Eldbus_Message *msg)
{
   const char *obj_path = eldbus_message_path_get(msg);
   Eo *bridge = eldbus_service_object_data_get(iface, ELM_ATSPI_BRIDGE_CLASS_NAME);
   // TIZEN_ONLY(20160705) - enable atspi_proxy to work
   ELM_ATSPI_BRIDGE_DATA_GET_OR_RETURN_VAL(bridge, pd, NULL);
   //
   Eo *obj = _bridge_object_from_path(bridge, obj_path);
   int x, y;
   Eldbus_Message *ret;
   AtspiCoordType coord_type;
   Eldbus_Message_Iter *iter;

   ELM_ATSPI_OBJ_CHECK_OR_RETURN_DBUS_ERROR(obj, EFL_ACCESS_OBJECT_MIXIN, msg);

   // TIZEN_ONLY(20161213) - do not response if ecore evas is obscured
   if (_ee_obscured_get(obj))
     return eldbus_message_error_new(msg, "org.freedesktop.DBus.Error.Failed", "ecore evas is obscured.");
   //

   if (!eldbus_message_arguments_get(msg, "iiu", &x, &y, &coord_type))
     return eldbus_message_error_new(msg, "org.freedesktop.DBus.Error.InvalidArgs", "Invalid index type.");

   // TIZEN_ONLY(20160705) - enable atspi_proxy to work
   Evas_Object *top = elm_object_top_widget_get(obj);
   int sx = 0;
   int sy = 0;
   efl_access_component_socket_offset_get(top, &sx, &sy);
   x = x - sx;
   y = y - sy;
   //

   ret = eldbus_message_method_return_new(msg);
   EINA_SAFETY_ON_NULL_RETURN_VAL(ret, NULL);

   iter = eldbus_message_iter_get(ret);

   Eina_Bool type = coord_type == ATSPI_COORD_TYPE_SCREEN ? EINA_TRUE : EINA_FALSE;
   Eo *accessible = _calculate_navigable_accessible_at_point(bridge, obj, type, x, y);

   /* Check deputy */
   Eo *deputy = NULL;
   if (accessible && efl_isa(accessible, ELM_ACCESS_CLASS))
     {
        Evas_Object *parent = elm_widget_parent_get(accessible);

        Elm_Widget_Smart_Data *wd;
        wd = efl_data_scope_get(parent, EFL_UI_WIDGET_CLASS);
        if (wd)
          {
             Eina_List *l;
             Evas_Object *widget;

             EINA_LIST_FOREACH(wd->subobjs, l, widget)
               {
                  // TIZEN_ONLY(20171109) : fix for invalid proxy object, when at-spi has been restarted
                  Eo *proxy = plug_type_proxy_get(parent, widget);
                  //
                  if (proxy)
                    {
                       int px, py, pw, ph;
                       evas_object_geometry_get(widget, &px, &py, &pw, &ph);
                       if (x >= px && x <= px + pw && y >= py && y <= py +ph)
                         {
                            /* proxy is also selectable */
                            deputy = accessible;
                            accessible = proxy;
                            break;
                         }
                    }
               }
          }
     }

   _bridge_iter_object_reference_append(bridge, iter, accessible);
   _bridge_object_register(bridge, accessible);

   const char *obj_bus_name = NULL, *ret_bus_name = NULL;
   _object_get_bus_name_and_path(bridge, obj, &obj_bus_name, NULL);
   if (accessible) _object_get_bus_name_and_path(bridge, accessible, &ret_bus_name, NULL);

   unsigned char recurse = obj_bus_name && ret_bus_name && strcmp(obj_bus_name, ret_bus_name) != 0;
   eldbus_message_iter_basic_append(iter, 'y', recurse);

   /* Send deputy */
   _bridge_iter_object_reference_append(bridge, iter, deputy);
   if (deputy) _bridge_object_register(bridge, deputy);

   return ret;
}
//

//TIZEN_ONLY(20170531): add "GetReadingMaterial" interface method
static int
_list_children_count_check(Eo *obj)
{
   int i;
   int list_count = 0;
   Eina_List *children;
   Eo *child = NULL;
   Efl_Access_Role role;

   if (!obj)
     return 0;

   role = efl_access_object_role_get(obj);
   if (role == EFL_ACCESS_ROLE_LIST)
     {
        int children_count = 0;
        children = efl_access_object_access_children_get(obj);
        children_count = eina_list_count(children);

        for (i = 0; i < children_count; i++)
          {
             child = eina_list_nth(children, i);
             role = efl_access_object_role_get(child);
             if (role == EFL_ACCESS_ROLE_LIST_ITEM)
               list_count++;
          }
        eina_list_free(children);
     }

   return list_count;
}

static int
_list_children_count(Eo *obj)
{
   Eina_List *children;
   int list_items_count = 0;
   int children_count = 0;

   children = efl_access_object_access_children_get(obj);
   children_count = eina_list_count(children);

   int i;
   Eo *child = NULL;

   list_items_count = _list_children_count_check(obj);
   if (list_items_count > 0)
     {
        eina_list_free(children);
        return list_items_count;
     }

   for (i = 0; i < children_count; i++)
     {
        child = eina_list_nth(children, i);
        list_items_count = _list_children_count(child);
        if (list_items_count > 0)
          {
             eina_list_free(children);
             return list_items_count;
          }
     }

   return 0;
}

static Eldbus_Message *
_accessible_reading_material_get(const Eldbus_Service_Interface *iface, const Eldbus_Message *msg)
{
   Eldbus_Message *ret;
   Eldbus_Message_Iter *iter, *iter_array, *iter_dict, *iter_entry;
   Eina_List *attrs, *l, *children;
   const char *name = NULL;
   unsigned int s1, s2;
   double value = 0;
   double increment = 0;
   double max_value = 0;
   double min_value = 0;
   int idx = 0;
   int child_count = 0;
   int selected_child_count = 0;
   uint64_t atspi_states = 0;
   Efl_Access_Role role;
   Efl_Access_Attribute *attr;
   Efl_Access_State_Set states;
   Efl_Access_Relation_Set rels = NULL;
   Efl_Access_Relation *rel;
   Eo *relation_obj = NULL;
   Eo *parent = NULL;
   Eo *child = NULL;
   Eina_Bool is_selected = EINA_FALSE;
   AtspiRole atspi_role = ATSPI_ROLE_INVALID;

   const char *obj_path = eldbus_message_path_get(msg);
   Eo *bridge = eldbus_service_object_data_get(iface, ELM_ATSPI_BRIDGE_CLASS_NAME);
   Eo *obj = _bridge_object_from_path(bridge, obj_path);

   ELM_ATSPI_OBJ_CHECK_OR_RETURN_DBUS_ERROR(obj, EFL_ACCESS_OBJECT_MIXIN, msg);

   ret = eldbus_message_method_return_new(msg);
   EINA_SAFETY_ON_NULL_RETURN_VAL(ret, NULL);

   iter = eldbus_message_iter_get(ret);
   /* attributes */
   attrs = efl_access_object_attributes_get(obj);
   iter_dict = eldbus_message_iter_container_new(iter, 'a', "{ss}");
   EINA_SAFETY_ON_NULL_GOTO(iter_dict, fail);
   EINA_LIST_FOREACH(attrs, l, attr)
     {
        iter_entry = eldbus_message_iter_container_new(iter_dict, 'e', NULL);
        EINA_SAFETY_ON_NULL_GOTO(iter_entry, fail);
        eldbus_message_iter_arguments_append(iter_entry, "ss", attr->key, attr->value);
        eldbus_message_iter_container_close(iter_dict, iter_entry);
     }

   eldbus_message_iter_container_close(iter, iter_dict);
   efl_access_attributes_list_free(attrs);

   /* name */
   name = efl_access_object_i18n_name_get(obj);
   if (!name)
     name = "";
   eldbus_message_iter_basic_append(iter, 's', name);

   /* name - LABELED_BY relation */
   rels = efl_access_object_relation_set_get(obj);
   EINA_LIST_FOREACH(rels, l, rel)
     {
        if (rel->type == EFL_ACCESS_RELATION_LABELLED_BY)
          {
             int last_index = eina_list_count(rel->objects) - 1;
             relation_obj = eina_list_nth(rel->objects, last_index);
             break;
          }
     }
   name = efl_access_object_i18n_name_get(relation_obj);
   if (!name)
     name = "";
   eldbus_message_iter_basic_append(iter, 's', name);

   /* name - text interface */
   name = NULL;
   if (efl_isa(obj, EFL_ACCESS_TEXT_INTERFACE))
     {
        int val;
        val = efl_access_text_character_count_get(obj);
        name = efl_access_text_get(obj, 0, val);
     }
   if (!name)
     name = "";
   eldbus_message_iter_basic_append(iter, 's', name);

   /* role */
   role = efl_access_object_role_get(obj);
   atspi_role = role > EFL_ACCESS_ROLE_LAST_DEFINED ? ATSPI_ROLE_LAST_DEFINED : elm_roles_to_atspi_roles[role][1];
   eldbus_message_iter_basic_append(iter, 'u', atspi_role);

   /* states */
   iter_array = eldbus_message_iter_container_new(iter, 'a', "u");
   EINA_SAFETY_ON_NULL_GOTO(iter_array, fail);
   states = efl_access_object_state_set_get(obj);
   atspi_states = _elm_atspi_state_set_to_atspi_state_set(states);
   s1 = atspi_states & 0xFFFFFFFF;
   s2 = (atspi_states >> 32) & 0xFFFFFFFF;
   eldbus_message_iter_basic_append(iter_array, 'u', s1);
   eldbus_message_iter_basic_append(iter_array, 'u', s2);
   eldbus_message_iter_container_close(iter, iter_array);

   /* localized role name */
   name = efl_access_object_localized_role_name_get(obj);
   if (!name)
     name = "";
   eldbus_message_iter_basic_append(iter, 's', name);

   /* child count */
   l = efl_access_object_access_children_get(obj);
   eldbus_message_iter_basic_append(iter, 'i', eina_list_count(l));
   eina_list_free(l);

   /* current value, increment, max, min */
   value = 0;
   increment = 0;
   max_value = 0;
   min_value = 0;
   if (efl_isa(obj, EFL_ACCESS_VALUE_INTERFACE))
     {
        efl_access_value_and_text_get(obj, &value, NULL);
        increment = efl_access_value_increment_get(obj);
        efl_access_value_range_get(obj, &min_value, &max_value, NULL);
     }
   eldbus_message_iter_basic_append(iter, 'd', value);
   eldbus_message_iter_basic_append(iter, 'd', increment);
   eldbus_message_iter_basic_append(iter, 'd', max_value);
   eldbus_message_iter_basic_append(iter, 'd', min_value);

   /* description */
   name = efl_access_object_description_get(obj);
   if (!name)
     name = "";
   eldbus_message_iter_basic_append(iter, 's', name);

   /* index in parent */
   idx = efl_access_object_index_in_parent_get(obj);
   eldbus_message_iter_basic_append(iter, 'i', idx);

   /* is selected in parent */
   parent = efl_provider_find(efl_parent_get(obj), EFL_ACCESS_OBJECT_MIXIN);
   if (efl_isa(parent, EFL_ACCESS_SELECTION_INTERFACE))
     {
        is_selected = efl_access_selection_is_child_selected(parent, idx);
     }

   eldbus_message_arguments_append(ret, "b", is_selected);

   /* has checkbox child */
   is_selected = EINA_FALSE;
   children = efl_access_object_access_children_get(obj);
   EINA_LIST_FOREACH(children, l, child)
     {
        if (efl_isa(child, EFL_UI_CHECK_CLASS))
          {
             is_selected = EINA_TRUE;
             break;
          }
     }
   eldbus_message_iter_basic_append(iter, 'b', is_selected);
   eina_list_free(children);

   /* list children count */
   child_count = 0;
   if (role == EFL_ACCESS_ROLE_DIALOG)
     {
        child_count = _list_children_count(obj);
     }
   eldbus_message_iter_basic_append(iter, 'i', child_count);

   /* first selected child index */
   idx = 0;
   if (efl_isa(obj, ELM_INDEX_CLASS))
     {
        children = efl_access_object_access_children_get(obj);
        EINA_LIST_FOREACH(children, l, child)
          {
             states = efl_access_object_state_set_get(child);
             if (STATE_TYPE_GET(states, EFL_ACCESS_STATE_SELECTED))
               break;
             idx++;
          }
        eina_list_free(children);
     }
   eldbus_message_iter_basic_append(iter, 'i', idx);

   /* parent */
   role = EFL_ACCESS_ROLE_INVALID;
   role = efl_access_object_role_get(obj);
   if ((!parent) && (EFL_ACCESS_ROLE_APPLICATION == role))
     _object_desktop_reference_append(iter);
   else
     _bridge_iter_object_reference_append(bridge, iter, parent);

   /* parent - states */
   iter_array = eldbus_message_iter_container_new(iter, 'a', "u");
   EINA_SAFETY_ON_NULL_GOTO(iter_array, fail);
   states = efl_access_object_state_set_get(parent);
   atspi_states = _elm_atspi_state_set_to_atspi_state_set(states);
   s1 = atspi_states & 0xFFFFFFFF;
   s2 = (atspi_states >> 32) & 0xFFFFFFFF;
   eldbus_message_iter_basic_append(iter_array, 'u', s1);
   eldbus_message_iter_basic_append(iter_array, 'u', s2);
   eldbus_message_iter_container_close(iter, iter_array);

   /* parent - child count */
   l = efl_access_object_access_children_get(parent);
   eldbus_message_iter_basic_append(iter, 'i', eina_list_count(l));
   eina_list_free(l);

   /* parent - role */
   role = efl_access_object_role_get(parent);
   atspi_role = role > EFL_ACCESS_ROLE_LAST_DEFINED ? ATSPI_ROLE_LAST_DEFINED : elm_roles_to_atspi_roles[role][1];
   eldbus_message_iter_basic_append(iter, 'u', atspi_role);

   /* parent - child count */
   selected_child_count = efl_access_selection_selected_children_count_get(parent);
   eldbus_message_iter_basic_append(iter, 'i', selected_child_count);

   /* relation object - DESCRIBED_BY */
   relation_obj = NULL;
   EINA_LIST_FOREACH(rels, l, rel)
     {
        if (rel->type == EFL_ACCESS_RELATION_DESCRIBED_BY)
          {
             int last_index = eina_list_count(rel->objects) - 1;
             relation_obj = eina_list_nth(rel->objects, last_index);
             break;
          }
     }
   _bridge_iter_object_reference_append(bridge, iter, relation_obj);
   efl_access_relation_set_free(&rels);

   return ret;

fail:
   if (rels) efl_access_relation_set_free(&rels);
   if (ret) eldbus_message_unref(ret);
   return NULL;
}
//

//TIZEN_ONLY(20170919): Handle default label object
static Eldbus_Message *
_accessible_default_label_info_get(const Eldbus_Service_Interface *iface, const Eldbus_Message *msg)
{
   Eldbus_Message *ret;
   Eldbus_Message_Iter *iter, *iter_dict, *iter_entry;
   const char *obj_path = eldbus_message_path_get(msg);
   Eo *bridge = eldbus_service_object_data_get(iface, ELM_ATSPI_BRIDGE_CLASS_NAME);
   Eo *obj = _bridge_object_from_path(bridge, obj_path);
   Eo *default_label_obj;
   Efl_Access_Role role;
   AtspiRole atspi_role = ATSPI_ROLE_INVALID;
   Eina_List *attrs, *l;
   Efl_Access_Attribute *attr = NULL;

   ELM_ATSPI_OBJ_CHECK_OR_RETURN_DBUS_ERROR(obj, EFL_ACCESS_OBJECT_MIXIN, msg);

   ret = eldbus_message_method_return_new(msg);
   EINA_SAFETY_ON_NULL_RETURN_VAL(ret, NULL);

   iter = eldbus_message_iter_get(ret);

    default_label_obj = _elm_win_default_label_obj_get(obj);
    if (!default_label_obj) default_label_obj = obj;
   _bridge_iter_object_reference_append(bridge, iter, default_label_obj);
   _bridge_object_register(bridge, default_label_obj);

   role = efl_access_object_role_get(default_label_obj);
   atspi_role = role > EFL_ACCESS_ROLE_LAST_DEFINED ? ATSPI_ROLE_LAST_DEFINED : elm_roles_to_atspi_roles[role][1];
   eldbus_message_iter_basic_append(iter, 'u', atspi_role);

   attrs = efl_access_object_attributes_get(default_label_obj);
   iter_dict = eldbus_message_iter_container_new(iter, 'a', "{ss}");
   EINA_SAFETY_ON_NULL_GOTO(iter_dict, fail);

   EINA_LIST_FOREACH(attrs, l, attr)
     {
        iter_entry = eldbus_message_iter_container_new(iter_dict, 'e', NULL);
        EINA_SAFETY_ON_NULL_GOTO(iter_entry, fail);
        eldbus_message_iter_arguments_append(iter_entry, "ss", attr->key, attr->value);
        eldbus_message_iter_container_close(iter_dict, iter_entry);
     }

   eldbus_message_iter_container_close(iter, iter_dict);
   efl_access_attributes_list_free(attrs);

   return ret;

fail:
   if (ret) eldbus_message_unref(ret);
   return NULL;
}
//

static const Eldbus_Method accessible_methods[] = {
   // TIZEN_ONLY(20170310) - implementation of get object under coordinates for accessibility
   { "GetNavigableAtPoint", ELDBUS_ARGS({"i", "x"}, {"i", "y"}, {"u", "coord_type"}), ELDBUS_ARGS({"(so)y", "accessible"}, {"(so)", "deputy"}), _accessible_get_navigable_at_point, 0 },
   { "GetNeighbor", ELDBUS_ARGS({"s", "current"}, {"i", "direction"}, {"i", "force_next"}), ELDBUS_ARGS({"(so)y", "accessible"}), _accessible_get_neighbor, 0 },
   //
   { "GetChildAtIndex", ELDBUS_ARGS({"i", "index"}), ELDBUS_ARGS({"(so)", "Accessible"}), _accessible_child_at_index, 0 },
   { "GetChildren", NULL, ELDBUS_ARGS({"a(so)", "children"}), _accessible_get_children, 0 },
   { "GetIndexInParent", NULL, ELDBUS_ARGS({"i", "index"}), _accessible_get_index_in_parent, 0 },
   { "GetRelationSet", NULL, ELDBUS_ARGS({"a(ua(so))", NULL}), _accessible_get_relation_set, 0 },
   { "GetRole", NULL, ELDBUS_ARGS({"u", "Role"}), _accessible_get_role, 0 },
   { "GetRoleName", NULL, ELDBUS_ARGS({"s", "Name"}), _accessible_get_role_name, 0 },
   { "GetLocalizedRoleName", NULL, ELDBUS_ARGS({"s", "LocalizedName"}), _accessible_get_localized_role_name, 0},
   { "GetState", NULL, ELDBUS_ARGS({"au", NULL}), _accessible_get_state, 0},
   { "GetApplication", NULL, ELDBUS_ARGS({"(so)", NULL}), _accessible_get_application, 0},
   { "GetAttributes", NULL, ELDBUS_ARGS({"a{ss}", NULL}), _accessible_attributes_get, 0},
   { "GetInterfaces", NULL, ELDBUS_ARGS({"as", NULL}), _accessible_interfaces_get, 0},
   //TIZEN_ONLY(20170405) Add gesture method to accessible interface
   { "DoGesture",
     ELDBUS_ARGS({"i", "type"}, {"i", "x_beg"}, {"i", "y_beg"},
                 {"i", "x_end"}, {"i", "y_end"}, {"i", "state"},
                 {"u", "event_time"}),
     ELDBUS_ARGS({"b", "result"}), _accessible_gesture_do, 0 },
   //
   //
   //TIZEN_ONLY(20170531): add "GetReadingMaterial" interface method
   { "GetReadingMaterial",
     NULL,
     ELDBUS_ARGS({"a{ss}", "attributes"}, {"s", "name"},
                 {"s", "labledByName"},{"s", "textIfceName"},
                 {"u", "role"}, {"au", "stateSet"},
                 {"s", "localizedName"}, {"i", "childCount"},
                 {"d", "currentValue"},{"d", "minimumIncrement"},
                 {"d", "maximumValue"},{"d", "minimumValue"},
                 {"s", "description"}, {"i", "indexInParent"},
                 {"b", "isSelectedInParent"}, {"b", "hasCheckboxChild"},
                 {"i", "listChildrenCount"},
                 {"i", "firstSelectedChildIndex"},
                 {"(so)", "parent"}, {"au", "parentStateSet"},
                 {"i", "parentChildCount"}, {"u", "parentRole"},
                 {"i", "selectedChildCount"},
                 {"(so)", "describecByObject"}),
     _accessible_reading_material_get, 0},
   //
   //TIZEN_ONLY(20170919): Handle default label object
   { "GetDefaultLabelInfo",
     NULL, ELDBUS_ARGS({"(so)", "defaultLabelObject"}, {"u", "defaultLabelRole"},{"a{ss}", "attributes"}),
     _accessible_default_label_info_get, 0},
   //
   { NULL, NULL, NULL, NULL, 0 }
};

// TIZEN_ONLY(20171207): make sure object is visible before performing required atspi action
static Eina_Bool
_is_operation_permitted(Eo *obj)
{
   Efl_Access_State_Set states;
   states = efl_access_object_state_set_get(obj);

   if (!STATE_TYPE_GET(states, EFL_ACCESS_STATE_SHOWING)) return EINA_FALSE;

   Eo *parent = obj;
   while (parent)
     {
        if (evas_object_freeze_events_get(parent)) return EINA_FALSE;
        parent = evas_object_smart_parent_get(parent);
     }
   return EINA_TRUE;
}
//

static Eldbus_Message *
_selection_selected_child_get(const Eldbus_Service_Interface *iface EINA_UNUSED, const Eldbus_Message *msg)
{
   const char *obj_path = eldbus_message_path_get(msg);
   Eo *bridge = eldbus_service_object_data_get(iface, ELM_ATSPI_BRIDGE_CLASS_NAME);
   Eo *obj = _bridge_object_from_path(bridge, obj_path);
   Eo *child = NULL;

   int idx;
   Eldbus_Message *ret;
   Eldbus_Message_Iter *iter;

   ELM_ATSPI_OBJ_CHECK_OR_RETURN_DBUS_ERROR(obj, EFL_ACCESS_SELECTION_INTERFACE, msg);

   if (!eldbus_message_arguments_get(msg, "i", &idx))
     return eldbus_message_error_new(msg, "org.freedesktop.DBus.Error.InvalidArgs", "Invalid index type.");

   ret = eldbus_message_method_return_new(msg);
   EINA_SAFETY_ON_NULL_RETURN_VAL(ret, NULL);

   iter = eldbus_message_iter_get(ret);
   child = efl_access_selection_selected_child_get(obj, idx);

   _bridge_iter_object_reference_append(bridge, iter, child);
   _bridge_object_register(bridge, child);

   return ret;
}

static Eldbus_Message *
_selection_child_select(const Eldbus_Service_Interface *iface, const Eldbus_Message *msg)
{
   const char *obj_path = eldbus_message_path_get(msg);
   Eo *bridge = eldbus_service_object_data_get(iface, ELM_ATSPI_BRIDGE_CLASS_NAME);
   Eo *obj = _bridge_object_from_path(bridge, obj_path);
   int idx;
   Eldbus_Message *ret;
   Eina_Bool result = EINA_FALSE;

   ELM_ATSPI_OBJ_CHECK_OR_RETURN_DBUS_ERROR(obj, EFL_ACCESS_SELECTION_INTERFACE, msg);

   if (!eldbus_message_arguments_get(msg, "i", &idx))
     return eldbus_message_error_new(msg, "org.freedesktop.DBus.Error.InvalidArgs", "Invalid index type.");

   ret = eldbus_message_method_return_new(msg);
   EINA_SAFETY_ON_NULL_RETURN_VAL(ret, NULL);

   if (_is_operation_permitted(obj))
     result = efl_access_selection_child_select(obj, idx);
   eldbus_message_arguments_append(ret, "b", result);

   return ret;
}

static Eldbus_Message *
_selection_selected_child_deselect(const Eldbus_Service_Interface *iface, const Eldbus_Message *msg)
{
   const char *obj_path = eldbus_message_path_get(msg);
   Eo *bridge = eldbus_service_object_data_get(iface, ELM_ATSPI_BRIDGE_CLASS_NAME);
   Eo *obj = _bridge_object_from_path(bridge, obj_path);
   int idx;
   Eldbus_Message *ret;
   Eina_Bool result = EINA_FALSE;

   ELM_ATSPI_OBJ_CHECK_OR_RETURN_DBUS_ERROR(obj, EFL_ACCESS_SELECTION_INTERFACE, msg);

   if (!eldbus_message_arguments_get(msg, "i", &idx))
     return eldbus_message_error_new(msg, "org.freedesktop.DBus.Error.InvalidArgs", "Invalid index type.");

   ret = eldbus_message_method_return_new(msg);
   EINA_SAFETY_ON_NULL_RETURN_VAL(ret, NULL);

   if (_is_operation_permitted(obj))
     result = efl_access_selection_selected_child_deselect(obj, idx);
   eldbus_message_arguments_append(ret, "b", result);

   return ret;
}

static Eldbus_Message *
_selection_is_child_selected(const Eldbus_Service_Interface *iface, const Eldbus_Message *msg)
{
   const char *obj_path = eldbus_message_path_get(msg);
   Eo *bridge = eldbus_service_object_data_get(iface, ELM_ATSPI_BRIDGE_CLASS_NAME);
   Eo *obj = _bridge_object_from_path(bridge, obj_path);
   int idx;
   Eldbus_Message *ret;
   Eina_Bool result;

   ELM_ATSPI_OBJ_CHECK_OR_RETURN_DBUS_ERROR(obj, EFL_ACCESS_SELECTION_INTERFACE, msg);

   if (!eldbus_message_arguments_get(msg, "i", &idx))
     return eldbus_message_error_new(msg, "org.freedesktop.DBus.Error.InvalidArgs", "Invalid index type.");

   ret = eldbus_message_method_return_new(msg);
   EINA_SAFETY_ON_NULL_RETURN_VAL(ret, NULL);

   result = efl_access_selection_is_child_selected(obj, idx);
   eldbus_message_arguments_append(ret, "b", result);

   return ret;
}

static Eldbus_Message *
_selection_all_children_select(const Eldbus_Service_Interface *iface, const Eldbus_Message *msg)
{
   const char *obj_path = eldbus_message_path_get(msg);
   Eo *bridge = eldbus_service_object_data_get(iface, ELM_ATSPI_BRIDGE_CLASS_NAME);
   Eo *obj = _bridge_object_from_path(bridge, obj_path);
   Eldbus_Message *ret;
   Eina_Bool result = EINA_FALSE;

   ELM_ATSPI_OBJ_CHECK_OR_RETURN_DBUS_ERROR(obj, EFL_ACCESS_SELECTION_INTERFACE, msg);

   ret = eldbus_message_method_return_new(msg);
   EINA_SAFETY_ON_NULL_RETURN_VAL(ret, NULL);

   if (_is_operation_permitted(obj))
     result = efl_access_selection_all_children_select(obj);
   eldbus_message_arguments_append(ret, "b", result);

   return ret;
}

static Eldbus_Message *
_selection_clear(const Eldbus_Service_Interface *iface, const Eldbus_Message *msg)
{
   const char *obj_path = eldbus_message_path_get(msg);
   Eo *bridge = eldbus_service_object_data_get(iface, ELM_ATSPI_BRIDGE_CLASS_NAME);
   Eo *obj = _bridge_object_from_path(bridge, obj_path);
   Eldbus_Message *ret;
   Eina_Bool result = EINA_FALSE;

   ELM_ATSPI_OBJ_CHECK_OR_RETURN_DBUS_ERROR(obj, EFL_ACCESS_SELECTION_INTERFACE, msg);

   ret = eldbus_message_method_return_new(msg);
   EINA_SAFETY_ON_NULL_RETURN_VAL(ret, NULL);

   if (_is_operation_permitted(obj))
     result = efl_access_selection_clear(obj);
   eldbus_message_arguments_append(ret, "b", result);

   return ret;
}

static Eldbus_Message *
_selection_child_deselect(const Eldbus_Service_Interface *iface, const Eldbus_Message *msg)
{
   const char *obj_path = eldbus_message_path_get(msg);
   Eo *bridge = eldbus_service_object_data_get(iface, ELM_ATSPI_BRIDGE_CLASS_NAME);
   Eo *obj = _bridge_object_from_path(bridge, obj_path);
   int idx;
   Eldbus_Message *ret;
   Eina_Bool result = EINA_FALSE;

   ELM_ATSPI_OBJ_CHECK_OR_RETURN_DBUS_ERROR(obj, EFL_ACCESS_SELECTION_INTERFACE, msg);

   if (!eldbus_message_arguments_get(msg, "i", &idx))
     return eldbus_message_error_new(msg, "org.freedesktop.DBus.Error.InvalidArgs", "Invalid index type.");

   ret = eldbus_message_method_return_new(msg);
   EINA_SAFETY_ON_NULL_RETURN_VAL(ret, NULL);

   if (_is_operation_permitted(obj))
     result = efl_access_selection_child_deselect(obj, idx);
   eldbus_message_arguments_append(ret, "b", result);

   return ret;
}

static const Eldbus_Method selection_methods[] = {
   { "GetSelectedChild", ELDBUS_ARGS({"i", "selectedChildIndex"}), ELDBUS_ARGS({"(so)", "Accessible"}), _selection_selected_child_get, 0 },
   { "SelectChild", ELDBUS_ARGS({"i", "childIndex"}), ELDBUS_ARGS({"b", "result"}), _selection_child_select, 0 },
   { "DeselectSelectedChild", ELDBUS_ARGS({"i", "selectedChildIndex"}), ELDBUS_ARGS({"b", "result"}), _selection_selected_child_deselect, 0 },
   { "IsChildSelected", ELDBUS_ARGS({"i", "childIndex"}), ELDBUS_ARGS({"b", "result"}), _selection_is_child_selected, 0 },
   { "SelectAll", NULL, ELDBUS_ARGS({"b", "result"}), _selection_all_children_select, 0},
   { "ClearSelection", NULL, ELDBUS_ARGS({"b", "result"}), _selection_clear, 0},
   { "DeselectChild", ELDBUS_ARGS({"i", "childIndex"}), ELDBUS_ARGS({"b", "result"}), _selection_child_deselect, 0 },
   { NULL, NULL, NULL, NULL, 0 }
};

static Eldbus_Message *
_action_description_get(const Eldbus_Service_Interface *iface, const Eldbus_Message *msg)
{
   const char *description, *obj_path = eldbus_message_path_get(msg);
   Eo *bridge = eldbus_service_object_data_get(iface, ELM_ATSPI_BRIDGE_CLASS_NAME);
   Eo *obj = _bridge_object_from_path(bridge, obj_path);
   int idx;
   Eldbus_Message *ret;

   ELM_ATSPI_OBJ_CHECK_OR_RETURN_DBUS_ERROR(obj, EFL_ACCESS_ACTION_MIXIN, msg);

   if (!eldbus_message_arguments_get(msg, "i", &idx))
     return eldbus_message_error_new(msg, "org.freedesktop.DBus.Error.InvalidArgs", "Invalid index type.");

   ret = eldbus_message_method_return_new(msg);
   EINA_SAFETY_ON_NULL_RETURN_VAL(ret, NULL);

   description = efl_access_action_description_get(obj, idx);
   description = description ? description : "";
   eldbus_message_arguments_append(ret, "s", description);

   return ret;
}

static Eldbus_Message *
_action_name_get(const Eldbus_Service_Interface *iface, const Eldbus_Message *msg)
{
   const char *name, *obj_path = eldbus_message_path_get(msg);
   Eo *bridge = eldbus_service_object_data_get(iface, ELM_ATSPI_BRIDGE_CLASS_NAME);
   Eo *obj = _bridge_object_from_path(bridge, obj_path);
   int idx;
   Eldbus_Message *ret;

   ELM_ATSPI_OBJ_CHECK_OR_RETURN_DBUS_ERROR(obj, EFL_ACCESS_ACTION_MIXIN, msg);

   if (!eldbus_message_arguments_get(msg, "i", &idx))
     return eldbus_message_error_new(msg, "org.freedesktop.DBus.Error.InvalidArgs", "Invalid index type.");

   ret = eldbus_message_method_return_new(msg);
   EINA_SAFETY_ON_NULL_RETURN_VAL(ret, NULL);

   name = efl_access_action_name_get(obj, idx);
   name = name ? name : "";
   eldbus_message_arguments_append(ret, "s", name);

   return ret;
}

static Eldbus_Message *
_action_localized_name_get(const Eldbus_Service_Interface *iface, const Eldbus_Message *msg)
{
   const char *name, *obj_path = eldbus_message_path_get(msg);
   Eo *bridge = eldbus_service_object_data_get(iface, ELM_ATSPI_BRIDGE_CLASS_NAME);
   Eo *obj = _bridge_object_from_path(bridge, obj_path);
   int idx;
   Eldbus_Message *ret;

   ELM_ATSPI_OBJ_CHECK_OR_RETURN_DBUS_ERROR(obj, EFL_ACCESS_ACTION_MIXIN, msg);

   if (!eldbus_message_arguments_get(msg, "i", &idx))
     return eldbus_message_error_new(msg, "org.freedesktop.DBus.Error.InvalidArgs", "Invalid index type.");

   ret = eldbus_message_method_return_new(msg);
   EINA_SAFETY_ON_NULL_RETURN_VAL(ret, NULL);

   name = efl_access_action_localized_name_get(obj, idx);
   name = name ? name : "";
   eldbus_message_arguments_append(ret, "s", name);

   return ret;
}

static Eldbus_Message *
_action_key_binding_get(const Eldbus_Service_Interface *iface, const Eldbus_Message *msg)
{
   const char *obj_path = eldbus_message_path_get(msg);
   char *key;
   Eo *bridge = eldbus_service_object_data_get(iface, ELM_ATSPI_BRIDGE_CLASS_NAME);
   Eo *obj = _bridge_object_from_path(bridge, obj_path);
   int idx;
   Eldbus_Message *ret;

   ELM_ATSPI_OBJ_CHECK_OR_RETURN_DBUS_ERROR(obj, EFL_ACCESS_ACTION_MIXIN, msg);

   if (!eldbus_message_arguments_get(msg, "i", &idx))
     return eldbus_message_error_new(msg, "org.freedesktop.DBus.Error.InvalidArgs", "Invalid index type.");

   ret = eldbus_message_method_return_new(msg);
   EINA_SAFETY_ON_NULL_RETURN_VAL(ret, NULL);

   key = efl_access_action_keybinding_get(obj, idx);
   eldbus_message_arguments_append(ret, "s", key ? key : "");
   if (key) free(key);

   return ret;
}

static Eldbus_Message *
_action_actions_get(const Eldbus_Service_Interface *iface, const Eldbus_Message *msg)
{
   const char *action, *obj_path = eldbus_message_path_get(msg);
   Eo *bridge = eldbus_service_object_data_get(iface, ELM_ATSPI_BRIDGE_CLASS_NAME);
   Eo *obj = _bridge_object_from_path(bridge, obj_path);
   Eina_List *actions;
   Eldbus_Message *ret;
   Eldbus_Message_Iter *iter, *iter_array = NULL;

   ELM_ATSPI_OBJ_CHECK_OR_RETURN_DBUS_ERROR(obj, EFL_ACCESS_ACTION_MIXIN, msg);

   ret = eldbus_message_method_return_new(msg);
   if (!ret) goto error;

   iter = eldbus_message_iter_get(ret);
   if (!iter) goto error;

   iter_array = eldbus_message_iter_container_new(iter, 'a', "sss");
   if (!iter_array) goto error;

   actions = efl_access_action_actions_get(obj);

   int id = 0;
   EINA_LIST_FREE(actions, action)
     {
        const char *descr;
        char *key;

        key = efl_access_action_keybinding_get(obj, id);
        descr = efl_access_action_description_get(obj, id);
        descr = descr ? descr : "";
        eldbus_message_iter_arguments_append(iter_array, "sss", action, descr, key ? key : "");
        if (key) free(key);
        id++;
     }
   eldbus_message_iter_container_close(iter, iter_array);
   return ret;

error:
   if (ret) eldbus_message_unref(ret);
   return NULL;
}

static Eldbus_Message *
_action_action_do(const Eldbus_Service_Interface *iface, const Eldbus_Message *msg)
{
   const char *obj_path = eldbus_message_path_get(msg);
   Eo *bridge = eldbus_service_object_data_get(iface, ELM_ATSPI_BRIDGE_CLASS_NAME);
   Eo *obj = _bridge_object_from_path(bridge, obj_path);
   int idx;
   Eldbus_Message *ret;
   Eina_Bool result = EINA_FALSE;

   ELM_ATSPI_OBJ_CHECK_OR_RETURN_DBUS_ERROR(obj, EFL_ACCESS_ACTION_MIXIN, msg);

   if (!eldbus_message_arguments_get(msg, "i", &idx))
     return eldbus_message_error_new(msg, "org.freedesktop.DBus.Error.InvalidArgs", "Invalid index type.");

   ret = eldbus_message_method_return_new(msg);
   EINA_SAFETY_ON_NULL_RETURN_VAL(ret, NULL);

   if (_is_operation_permitted(obj))
     result = efl_access_action_do(obj, idx);
   eldbus_message_arguments_append(ret, "b", result);

   return ret;
}

static Eldbus_Message *
_action_action_name_do(const Eldbus_Service_Interface *iface, const Eldbus_Message *msg)
{
   const char *obj_path = eldbus_message_path_get(msg);
   Eo *bridge = eldbus_service_object_data_get(iface, ELM_ATSPI_BRIDGE_CLASS_NAME);
   Eo *obj = _bridge_object_from_path(bridge, obj_path);
   char *action_name;
   int idx = 0;
   Eina_List *actions;
   const char *action;
   Eldbus_Message *ret;
   Eina_Bool result = EINA_FALSE;

   ELM_ATSPI_OBJ_CHECK_OR_RETURN_DBUS_ERROR(obj, EFL_ACCESS_ACTION_MIXIN, msg);

   if (!eldbus_message_arguments_get(msg, "s", &action_name))
     return eldbus_message_error_new(msg, "org.freedesktop.DBus.Error.InvalidArgs", "Invalid index type.");

   ret = eldbus_message_method_return_new(msg);
   EINA_SAFETY_ON_NULL_RETURN_VAL(ret, NULL);

   actions = efl_access_action_actions_get(obj);
   EINA_LIST_FREE(actions, action)
     {
        if (!result && action && !strcmp(action, action_name))
          result = EINA_TRUE;

        if (!result)
          idx++;
     }

   if (result)
     result = efl_access_action_do(obj, idx);

   eldbus_message_arguments_append(ret, "b", result);

   return ret;
}

static const Eldbus_Method action_methods[] = {
   { "GetDescription", ELDBUS_ARGS({"i", "index"}), ELDBUS_ARGS({"s", "description"}), _action_description_get, 0 },
   { "GetName", ELDBUS_ARGS({"i", "index"}), ELDBUS_ARGS({"s", "name"}), _action_name_get, 0 },
   { "GetLocalizedName", ELDBUS_ARGS({"i", "index"}), ELDBUS_ARGS({"s", "name"}), _action_localized_name_get, 0 },
   { "GetKeyBinding", ELDBUS_ARGS({"i", "index"}), ELDBUS_ARGS({"s", "key"}), _action_key_binding_get, 0 },
   { "GetActions", NULL, ELDBUS_ARGS({"a(sss)", "actions"}), _action_actions_get, 0 },
   { "DoAction", ELDBUS_ARGS({"i", "index"}), ELDBUS_ARGS({"b", "result"}), _action_action_do, 0 },
   { "DoActionName", ELDBUS_ARGS({"s", "name"}), ELDBUS_ARGS({"b", "result"}), _action_action_name_do, 0 },
   { NULL, NULL, NULL, NULL, 0 }
};

static Eldbus_Message *
_image_extents_get(const Eldbus_Service_Interface *iface, const Eldbus_Message *msg)
{
   AtspiCoordType type;
   Eldbus_Message *ret;
   const char *obj_path = eldbus_message_path_get(msg);
   Eina_Bool screen_coords;
   Eo *bridge = eldbus_service_object_data_get(iface, ELM_ATSPI_BRIDGE_CLASS_NAME);
   Eo *obj = _bridge_object_from_path(bridge, obj_path);
   Eina_Rect r;

   ELM_ATSPI_OBJ_CHECK_OR_RETURN_DBUS_ERROR(obj, EFL_ACCESS_OBJECT_MIXIN, msg);

   if (!eldbus_message_arguments_get(msg, "u", &type))
     return eldbus_message_error_new(msg, "org.freedesktop.DBus.Error.InvalidArgs", "Invalid index type.");

   ret = eldbus_message_method_return_new(msg);
   EINA_SAFETY_ON_NULL_RETURN_VAL(ret, NULL);

   screen_coords = type == ATSPI_COORD_TYPE_SCREEN ? EINA_TRUE : EINA_FALSE;
   r = efl_access_component_extents_get(obj, screen_coords);
   eldbus_message_arguments_append(ret, "iiii", r.x, r.y, r.w, r.h);


   return ret;
}

static Eldbus_Message *
_image_position_get(const Eldbus_Service_Interface *iface, const Eldbus_Message *msg)
{
   AtspiCoordType type;
   Eldbus_Message *ret;
   const char *obj_path = eldbus_message_path_get(msg);
   Eo *bridge = eldbus_service_object_data_get(iface, ELM_ATSPI_BRIDGE_CLASS_NAME);
   Eo *obj = _bridge_object_from_path(bridge, obj_path);
   int x = -1, y = -1;
   Eina_Bool screen_coords;

   ELM_ATSPI_OBJ_CHECK_OR_RETURN_DBUS_ERROR(obj, EFL_ACCESS_OBJECT_MIXIN, msg);

   if (!eldbus_message_arguments_get(msg, "u", &type))
     return eldbus_message_error_new(msg, "org.freedesktop.DBus.Error.InvalidArgs", "Invalid index type.");

   ret = eldbus_message_method_return_new(msg);
   EINA_SAFETY_ON_NULL_RETURN_VAL(ret, NULL);

   screen_coords = type == ATSPI_COORD_TYPE_SCREEN ? EINA_TRUE : EINA_FALSE;
   if (screen_coords)
     efl_access_component_screen_position_get(obj, &x, &y);
   else
     evas_object_geometry_get(obj, &x, &y, NULL, NULL);
   eldbus_message_arguments_append(ret, "i", x);
   eldbus_message_arguments_append(ret, "i", y);

   return ret;
}

static Eldbus_Message *
_image_size_get(const Eldbus_Service_Interface *iface, const Eldbus_Message *msg)
{
   Eldbus_Message *ret;
   const char *obj_path = eldbus_message_path_get(msg);
   Eo *bridge = eldbus_service_object_data_get(iface, ELM_ATSPI_BRIDGE_CLASS_NAME);
   Eo *obj = _bridge_object_from_path(bridge, obj_path);
   int w = -1, h = -1;

   ELM_ATSPI_OBJ_CHECK_OR_RETURN_DBUS_ERROR(obj, EFL_ACCESS_OBJECT_MIXIN, msg);

   ret = eldbus_message_method_return_new(msg);
   EINA_SAFETY_ON_NULL_RETURN_VAL(ret, NULL);

   evas_object_geometry_get(obj, NULL, NULL, &w, &h);
   eldbus_message_arguments_append(ret, "i", w);
   eldbus_message_arguments_append(ret, "i", h);

   return ret;
}

static const Eldbus_Method image_methods[] = {
   { "GetImageExtents", ELDBUS_ARGS({"u", "coordType"}), ELDBUS_ARGS({"iiii", "extents"}), _image_extents_get, 0 },
   { "GetImagePosition", ELDBUS_ARGS({"u", "coordType"}), ELDBUS_ARGS({"i", "x"}, {"i", "y"}), _image_position_get, 0 },
   { "GetImageSize", NULL, ELDBUS_ARGS({"i", "width"}, {"i", "height"}), _image_size_get, 0 },
   { NULL, NULL, NULL, NULL, 0 }
};

static Eldbus_Message *
_text_string_at_offset_get(const Eldbus_Service_Interface *iface, const Eldbus_Message *msg)
{
   const char *obj_path = eldbus_message_path_get(msg);
   char *str;
   Efl_Access_Text_Granularity gran;
   int start, end;
   Eldbus_Message *ret;
   Eo *bridge = eldbus_service_object_data_get(iface, ELM_ATSPI_BRIDGE_CLASS_NAME);
   Eo *obj = _bridge_object_from_path(bridge, obj_path);

   ELM_ATSPI_OBJ_CHECK_OR_RETURN_DBUS_ERROR(obj, EFL_ACCESS_TEXT_INTERFACE, msg);

   if (!eldbus_message_arguments_get(msg, "iu", &start, &gran))
     return eldbus_message_error_new(msg, "org.freedesktop.DBus.Error.InvalidArgs", "Offset and granularity expected.");

   ret = eldbus_message_method_return_new(msg);
   EINA_SAFETY_ON_NULL_RETURN_VAL(ret, NULL);

   str = efl_access_text_string_get(obj, gran, &start, &end);
   str = str ? str : strdup("");

   eldbus_message_arguments_append(ret, "sii", str, start, end);
   free(str);

   return ret;
}

static Eldbus_Message *
_text_text_get(const Eldbus_Service_Interface *iface, const Eldbus_Message *msg)
{
   const char *obj_path = eldbus_message_path_get(msg);
   char *str;
   Eo *bridge = eldbus_service_object_data_get(iface, ELM_ATSPI_BRIDGE_CLASS_NAME);
   Eo *obj = _bridge_object_from_path(bridge, obj_path);
   int start, end;

   ELM_ATSPI_OBJ_CHECK_OR_RETURN_DBUS_ERROR(obj, EFL_ACCESS_TEXT_INTERFACE, msg);

   if (!eldbus_message_arguments_get(msg, "ii", &start, &end))
     return eldbus_message_error_new(msg, "org.freedesktop.DBus.Error.InvalidArgs", "Offset and granularity expected.");

   Eldbus_Message *ret = eldbus_message_method_return_new(msg);
   EINA_SAFETY_ON_NULL_RETURN_VAL(ret, NULL);

   str = efl_access_text_get(obj, start, end);
   str = str ? str : strdup("");

   eldbus_message_arguments_append(ret, "s", str);
   free(str);

   return ret;
}

static Eldbus_Message *
_text_caret_offset_set(const Eldbus_Service_Interface *iface, const Eldbus_Message *msg)
{
   const char *obj_path = eldbus_message_path_get(msg);
   Eo *bridge = eldbus_service_object_data_get(iface, ELM_ATSPI_BRIDGE_CLASS_NAME);
   Eo *obj = _bridge_object_from_path(bridge, obj_path);
   int offset;
   Eldbus_Message *ret;
   Eina_Bool res;

   ELM_ATSPI_OBJ_CHECK_OR_RETURN_DBUS_ERROR(obj, EFL_ACCESS_TEXT_INTERFACE, msg);

   if (!eldbus_message_arguments_get(msg, "i", &offset))
     return eldbus_message_error_new(msg, "org.freedesktop.DBus.Error.InvalidArgs", "Offset expected.");

   ret = eldbus_message_method_return_new(msg);
   EINA_SAFETY_ON_NULL_RETURN_VAL(ret, NULL);

   res = efl_access_text_caret_offset_set(obj, offset);

   eldbus_message_arguments_append(ret, "b", res);

   return ret;
}

static Eldbus_Message *
_text_character_at_offset_get(const Eldbus_Service_Interface *iface, const Eldbus_Message *msg)
{
   const char *obj_path = eldbus_message_path_get(msg);
   Eo *bridge = eldbus_service_object_data_get(iface, ELM_ATSPI_BRIDGE_CLASS_NAME);
   Eo *obj = _bridge_object_from_path(bridge, obj_path);
   int offset;
   Eldbus_Message *ret;
   Eina_Unicode res;

   ELM_ATSPI_OBJ_CHECK_OR_RETURN_DBUS_ERROR(obj, EFL_ACCESS_TEXT_INTERFACE, msg);

   if (!eldbus_message_arguments_get(msg, "i", &offset))
     return eldbus_message_error_new(msg, "org.freedesktop.DBus.Error.InvalidArgs", "Offset expected.");

   ret = eldbus_message_method_return_new(msg);
   EINA_SAFETY_ON_NULL_RETURN_VAL(ret, NULL);

   res = efl_access_text_character_get(obj, offset);

   eldbus_message_arguments_append(ret, "i", res);

   return ret;
}

static Eldbus_Message *
_text_attribute_value_get(const Eldbus_Service_Interface *iface, const Eldbus_Message *msg)
{
   const char *name, *obj_path = eldbus_message_path_get(msg);
   char *value = NULL;
   Eo *bridge = eldbus_service_object_data_get(iface, ELM_ATSPI_BRIDGE_CLASS_NAME);
   Eo *obj = _bridge_object_from_path(bridge, obj_path);
   int start, end;
   Eldbus_Message *ret;
   Eina_Bool res;

   ELM_ATSPI_OBJ_CHECK_OR_RETURN_DBUS_ERROR(obj, EFL_ACCESS_TEXT_INTERFACE, msg);

   if (!eldbus_message_arguments_get(msg, "is", &start, &name))
     return eldbus_message_error_new(msg, "org.freedesktop.DBus.Error.InvalidArgs", "Offset and attribute name expected.");

   ret = eldbus_message_method_return_new(msg);
   EINA_SAFETY_ON_NULL_RETURN_VAL(ret, NULL);

   res = efl_access_text_attribute_get(obj, name, &start, &end, &value);
   eldbus_message_arguments_append(ret, "siib", value ? value : "", start, end, res);

   if (value) free(value);
   return ret;
}

static Eldbus_Message *
_text_attributes_get(const Eldbus_Service_Interface *iface, const Eldbus_Message *msg)
{
   const char *obj_path = eldbus_message_path_get(msg);
   Eo *bridge = eldbus_service_object_data_get(iface, ELM_ATSPI_BRIDGE_CLASS_NAME);
   Eo *obj = _bridge_object_from_path(bridge, obj_path);
   int start, end;
   Eldbus_Message *ret;
   Eldbus_Message_Iter *iter, *iter_array;
   Eina_List *attrs;
   Efl_Access_Text_Attribute *attr;

   ELM_ATSPI_OBJ_CHECK_OR_RETURN_DBUS_ERROR(obj, EFL_ACCESS_TEXT_INTERFACE, msg);

   if (!eldbus_message_arguments_get(msg, "i", &start))
     return eldbus_message_error_new(msg, "org.freedesktop.DBus.Error.InvalidArgs", "Offset expected.");

   ret = eldbus_message_method_return_new(msg);
   EINA_SAFETY_ON_NULL_RETURN_VAL(ret, NULL);

   iter = eldbus_message_iter_get(ret);
   iter_array = eldbus_message_iter_container_new(iter, 'a', "{ss}");
   EINA_SAFETY_ON_NULL_GOTO(iter_array, fail);

   attrs = efl_access_text_attributes_get(obj, &start, &end);

   EINA_LIST_FREE(attrs, attr)
    {
       eldbus_message_iter_arguments_append(iter_array, "ss", attr->name, attr->value);
       elm_atspi_text_text_attribute_free(attr);
    }

   eldbus_message_iter_container_close(iter, iter_array);
   eldbus_message_iter_arguments_append(iter, "ii", start, end);

   return ret;

fail:
   if (ret) eldbus_message_unref(ret);
   return NULL;
}

static Eldbus_Message *
_text_default_attributes_get(const Eldbus_Service_Interface *iface, const Eldbus_Message *msg)
{
   const char *obj_path = eldbus_message_path_get(msg);
   Eo *bridge = eldbus_service_object_data_get(iface, ELM_ATSPI_BRIDGE_CLASS_NAME);
   Eo *obj = _bridge_object_from_path(bridge, obj_path);
   Eldbus_Message *ret;
   Eldbus_Message_Iter *iter, *iter_array;
   Eina_List *attrs;
   Efl_Access_Text_Attribute *attr;

   ELM_ATSPI_OBJ_CHECK_OR_RETURN_DBUS_ERROR(obj, EFL_ACCESS_TEXT_INTERFACE, msg);

   ret = eldbus_message_method_return_new(msg);
   EINA_SAFETY_ON_NULL_RETURN_VAL(ret, NULL);

   iter = eldbus_message_iter_get(ret);
   iter_array = eldbus_message_iter_container_new(iter, 'a', "{ss}");
   EINA_SAFETY_ON_NULL_GOTO(iter_array, fail);

   attrs = efl_access_text_default_attributes_get(obj);

   EINA_LIST_FREE(attrs, attr)
    {
      eldbus_message_iter_arguments_append(iter_array, "ss", attr->name, attr->value);
      elm_atspi_text_text_attribute_free(attr);
    }

   eldbus_message_iter_container_close(iter, iter_array);

   return ret;

fail:
   if (ret) eldbus_message_unref(ret);
   return NULL;
}

static Eldbus_Message *
_text_character_extents_get(const Eldbus_Service_Interface *iface, const Eldbus_Message *msg)
{
   const char *obj_path = eldbus_message_path_get(msg);
   Eo *bridge = eldbus_service_object_data_get(iface, ELM_ATSPI_BRIDGE_CLASS_NAME);
   Eo *obj = _bridge_object_from_path(bridge, obj_path);
   int offset;
   Eina_Rect rect;
   AtspiCoordType type;
   Eina_Bool screen_coords, res;
   Eldbus_Message *ret;

   ELM_ATSPI_OBJ_CHECK_OR_RETURN_DBUS_ERROR(obj, EFL_ACCESS_TEXT_INTERFACE, msg);

   if (!eldbus_message_arguments_get(msg, "iu", &offset, &type))
     return eldbus_message_error_new(msg, "org.freedesktop.DBus.Error.InvalidArgs", "Offset and coordinates type expected.");

   ret = eldbus_message_method_return_new(msg);
   EINA_SAFETY_ON_NULL_RETURN_VAL(ret, NULL);

   screen_coords = type == ATSPI_COORD_TYPE_SCREEN ? EINA_TRUE : EINA_FALSE;

   res = efl_access_text_character_extents_get(obj, offset, screen_coords, &rect);

   if (!res)
     {
        eldbus_message_unref(ret);
        return eldbus_message_error_new(msg, "org.freedesktop.DBus.Error.Failed", "Unable to get character extents.");
     }
   eldbus_message_arguments_append(ret, "iiii", rect.x, rect.y, rect.w, rect.h);

   return ret;
}

static Eldbus_Message *
_text_offset_at_point_get(const Eldbus_Service_Interface *iface, const Eldbus_Message *msg)
{
   const char *obj_path = eldbus_message_path_get(msg);
   Eo *bridge = eldbus_service_object_data_get(iface, ELM_ATSPI_BRIDGE_CLASS_NAME);
   Eo *obj = _bridge_object_from_path(bridge, obj_path);
   int offset, x, y;
   AtspiCoordType type;
   Eina_Bool screen_coords;
   Eldbus_Message *ret;

   ELM_ATSPI_OBJ_CHECK_OR_RETURN_DBUS_ERROR(obj, EFL_ACCESS_TEXT_INTERFACE, msg);

   if (!eldbus_message_arguments_get(msg, "iiu", &x, &y, &type))
     return eldbus_message_error_new(msg, "org.freedesktop.DBus.Error.InvalidArgs", "Offset and coordinates type expected.");

   ret = eldbus_message_method_return_new(msg);
   EINA_SAFETY_ON_NULL_RETURN_VAL(ret, NULL);

   x = y = -1;
   screen_coords = type == ATSPI_COORD_TYPE_SCREEN ? EINA_TRUE : EINA_FALSE;

   offset = efl_access_text_offset_at_point_get(obj, screen_coords, x, y);

   eldbus_message_arguments_append(ret, "i", offset);

   return ret;
}

static Eldbus_Message *
_text_n_selections_get(const Eldbus_Service_Interface *iface, const Eldbus_Message *msg)
{
   const char *obj_path = eldbus_message_path_get(msg);
   Eo *bridge = eldbus_service_object_data_get(iface, ELM_ATSPI_BRIDGE_CLASS_NAME);
   Eo *obj = _bridge_object_from_path(bridge, obj_path);
   int n;
   Eldbus_Message *ret;

   ELM_ATSPI_OBJ_CHECK_OR_RETURN_DBUS_ERROR(obj, EFL_ACCESS_TEXT_INTERFACE, msg);

   ret = eldbus_message_method_return_new(msg);
   EINA_SAFETY_ON_NULL_RETURN_VAL(ret, NULL);

   n = efl_access_text_selections_count_get(obj);

   eldbus_message_arguments_append(ret, "i", n);

   return ret;
}

static Eldbus_Message *
_text_selection_get(const Eldbus_Service_Interface *iface, const Eldbus_Message *msg)
{
   const char *obj_path = eldbus_message_path_get(msg);
   Eo *bridge = eldbus_service_object_data_get(iface, ELM_ATSPI_BRIDGE_CLASS_NAME);
   Eo *obj = _bridge_object_from_path(bridge, obj_path);
   int sel_num, start, end;
   Eldbus_Message *ret;

   ELM_ATSPI_OBJ_CHECK_OR_RETURN_DBUS_ERROR(obj, EFL_ACCESS_TEXT_INTERFACE, msg);

   if (!eldbus_message_arguments_get(msg, "i", &sel_num))
     return eldbus_message_error_new(msg, "org.freedesktop.DBus.Error.InvalidArgs", "Selection number expected.");

   ret = eldbus_message_method_return_new(msg);
   EINA_SAFETY_ON_NULL_RETURN_VAL(ret, NULL);

   efl_access_text_access_selection_get(obj, sel_num, &start, &end);

   eldbus_message_arguments_append(ret, "ii", start, end);

   return ret;
}

static Eldbus_Message *
_text_selection_add(const Eldbus_Service_Interface *iface, const Eldbus_Message *msg)
{
   const char *obj_path = eldbus_message_path_get(msg);
   Eo *bridge = eldbus_service_object_data_get(iface, ELM_ATSPI_BRIDGE_CLASS_NAME);
   Eo *obj = _bridge_object_from_path(bridge, obj_path);
   int start, end;
   Eina_Bool res;
   Eldbus_Message *ret;

   ELM_ATSPI_OBJ_CHECK_OR_RETURN_DBUS_ERROR(obj, EFL_ACCESS_TEXT_INTERFACE, msg);

   if (!eldbus_message_arguments_get(msg, "ii", &start, &end))
     return eldbus_message_error_new(msg, "org.freedesktop.DBus.Error.InvalidArgs", "Start and end text offset expected.");

   ret = eldbus_message_method_return_new(msg);
   EINA_SAFETY_ON_NULL_RETURN_VAL(ret, NULL);

   res = efl_access_text_selection_add(obj, start, end);

   eldbus_message_arguments_append(ret, "b", res);

   return ret;
}

static Eldbus_Message *
_text_selection_remove(const Eldbus_Service_Interface *iface, const Eldbus_Message *msg)
{
   const char *obj_path = eldbus_message_path_get(msg);
   Eo *bridge = eldbus_service_object_data_get(iface, ELM_ATSPI_BRIDGE_CLASS_NAME);
   Eo *obj = _bridge_object_from_path(bridge, obj_path);
   int sel_num;
   Eina_Bool res;
   Eldbus_Message *ret;

   ELM_ATSPI_OBJ_CHECK_OR_RETURN_DBUS_ERROR(obj, EFL_ACCESS_TEXT_INTERFACE, msg);

   if (!eldbus_message_arguments_get(msg, "i", &sel_num))
     return eldbus_message_error_new(msg, "org.freedesktop.DBus.Error.InvalidArgs", "Selection number expected.");

   ret = eldbus_message_method_return_new(msg);
   EINA_SAFETY_ON_NULL_RETURN_VAL(ret, NULL);

   res = efl_access_text_selection_remove(obj, sel_num);

   eldbus_message_arguments_append(ret, "b", res);

   return ret;
}

static Eldbus_Message *
_text_selection_set(const Eldbus_Service_Interface *iface, const Eldbus_Message *msg)
{
   const char *obj_path = eldbus_message_path_get(msg);
   Eo *bridge = eldbus_service_object_data_get(iface, ELM_ATSPI_BRIDGE_CLASS_NAME);
   Eo *obj = _bridge_object_from_path(bridge, obj_path);
   int sel_num, start, end;
   Eina_Bool res;
   Eldbus_Message *ret;

   ELM_ATSPI_OBJ_CHECK_OR_RETURN_DBUS_ERROR(obj, EFL_ACCESS_TEXT_INTERFACE, msg);

   if (!eldbus_message_arguments_get(msg, "iii", &sel_num, &start, &end))
     return eldbus_message_error_new(msg, "org.freedesktop.DBus.Error.InvalidArgs", "Selection number expected.");

   ret = eldbus_message_method_return_new(msg);
   EINA_SAFETY_ON_NULL_RETURN_VAL(ret, NULL);

   res = efl_access_text_access_selection_set(obj, sel_num, start, end);

   eldbus_message_arguments_append(ret, "b", res);

   return ret;
}

static Eldbus_Message *
_text_range_extents_get(const Eldbus_Service_Interface *iface, const Eldbus_Message *msg)
{
   const char *obj_path = eldbus_message_path_get(msg);
   Eo *bridge = eldbus_service_object_data_get(iface, ELM_ATSPI_BRIDGE_CLASS_NAME);
   Eo *obj = _bridge_object_from_path(bridge, obj_path);
   int start, end;
   Eina_Rect rect;
   Eina_Bool screen_coords, res;
   AtspiCoordType type;
   Eldbus_Message *ret;

   ELM_ATSPI_OBJ_CHECK_OR_RETURN_DBUS_ERROR(obj, EFL_ACCESS_TEXT_INTERFACE, msg);

   if (!eldbus_message_arguments_get(msg, "iiu", &start, &end, &type))
     return eldbus_message_error_new(msg, "org.freedesktop.DBus.Error.InvalidArgs", "Selection number expected.");

   ret = eldbus_message_method_return_new(msg);
   EINA_SAFETY_ON_NULL_RETURN_VAL(ret, NULL);

   screen_coords = type == ATSPI_COORD_TYPE_SCREEN ? EINA_TRUE : EINA_FALSE;
   res = efl_access_text_range_extents_get(obj, screen_coords, start, end, &rect);
   if (!res)
     {
        eldbus_message_unref(ret);
        return eldbus_message_error_new(msg, "org.freedesktop.DBus.Error.Failed", "Can't get range extents.");
     }

   eldbus_message_arguments_append(ret, "iiii", rect.x, rect.y, rect.w, rect.h);

   return ret;
}

static Eldbus_Message *
_text_bounded_ranges_get(const Eldbus_Service_Interface *iface, const Eldbus_Message *msg)
{
   const char *obj_path = eldbus_message_path_get(msg);
   Eo *bridge = eldbus_service_object_data_get(iface, ELM_ATSPI_BRIDGE_CLASS_NAME);
   Eo *obj = _bridge_object_from_path(bridge, obj_path);
   Eina_Rect rect;
   Eina_Bool screen_coords;
   AtspiCoordType type;
   Efl_Access_Text_Clip_Type xclip, yclip;
   Eina_List *ranges;
   Eldbus_Message *ret;
   Efl_Access_Text_Range *range;
   Eldbus_Message_Iter *iter, *iter_array, *iter_struct, *iter_var;

   ELM_ATSPI_OBJ_CHECK_OR_RETURN_DBUS_ERROR(obj, EFL_ACCESS_TEXT_INTERFACE, msg);

   if (!eldbus_message_arguments_get(msg, "iiiiuuu", &rect.x, &rect.y, &rect.w, &rect.h, &type, &xclip, &yclip))
     return eldbus_message_error_new(msg, "org.freedesktop.DBus.Error.InvalidArgs", "Expected (x,y,w,h) of bounding box, screen coord type and x, y text clip types.");

   ret = eldbus_message_method_return_new(msg);
   EINA_SAFETY_ON_NULL_RETURN_VAL(ret, NULL);

   iter = eldbus_message_iter_get(ret);
   iter_array = eldbus_message_iter_container_new(iter, 'a', "(iisv)");
   EINA_SAFETY_ON_NULL_GOTO(iter_array, fail);

   screen_coords = type == ATSPI_COORD_TYPE_SCREEN ? EINA_TRUE : EINA_FALSE;
   ranges = efl_access_text_bounded_ranges_get(obj, screen_coords, rect, xclip, yclip);

   EINA_LIST_FREE(ranges, range)
     {
        iter_struct = eldbus_message_iter_container_new(iter_array, 'r', NULL);
        if (iter_struct)
          {
             eldbus_message_iter_basic_append(iter_struct, 'i', range->start_offset);
             eldbus_message_iter_basic_append(iter_struct, 'i', range->end_offset);
             range->content = range->content ? range->content : strdup("");
             eldbus_message_iter_basic_append(iter_struct, 's', range->content);
             /* AT-SPI specification requires variant type in return, however
              * ATK or other implementations as well as AT Clients don't use it .
              * To cover spec a dummy value will be returned */
             iter_var = eldbus_message_iter_container_new(iter_struct, 'v', "i");
             if (iter_var)
               {
                  eldbus_message_iter_basic_append(iter_var, 'i', 0);
                  eldbus_message_iter_container_close(iter_struct, iter_var);
               }
             eldbus_message_iter_container_close(iter_array, iter_struct);
          }
        if (range->content) free(range->content);
        free(range);
     }

   eldbus_message_iter_container_close(iter, iter_array);

   return ret;

fail:
   if (ret) eldbus_message_unref(ret);
   return NULL;
}

static Eldbus_Message *
_text_run_attributes_get(const Eldbus_Service_Interface *iface, const Eldbus_Message *msg)
{
   const char *obj_path = eldbus_message_path_get(msg);
   Eo *bridge = eldbus_service_object_data_get(iface, ELM_ATSPI_BRIDGE_CLASS_NAME);
   Eo *obj = _bridge_object_from_path(bridge, obj_path);
   int start, end;
   Eldbus_Message *ret;
   Eldbus_Message_Iter *iter, *iter_array;
   Eina_List *attrs, *defaults;
   Efl_Access_Text_Attribute *attr;
   Eina_Bool incl_def;

   ELM_ATSPI_OBJ_CHECK_OR_RETURN_DBUS_ERROR(obj, EFL_ACCESS_TEXT_INTERFACE, msg);

   if (!eldbus_message_arguments_get(msg, "ib", &start, &incl_def))
     return eldbus_message_error_new(msg, "org.freedesktop.DBus.Error.InvalidArgs", "Offset and include defaults flag expected.");

   ret = eldbus_message_method_return_new(msg);
   EINA_SAFETY_ON_NULL_RETURN_VAL(ret, NULL);

   iter = eldbus_message_iter_get(ret);
   iter_array = eldbus_message_iter_container_new(iter, 'a', "{ss}");
   EINA_SAFETY_ON_NULL_GOTO(iter_array, fail);

   attrs = efl_access_text_attributes_get(obj, &start, &end);

   if (incl_def)
     {
        defaults = efl_access_text_default_attributes_get(obj);
        attrs = eina_list_merge(attrs, defaults);
     }

   EINA_LIST_FREE(attrs, attr)
     {
        eldbus_message_iter_arguments_append(iter_array, "ss", attr->name, attr->value);
        elm_atspi_text_text_attribute_free(attr);
     }

   eldbus_message_iter_container_close(iter, iter_array);
   eldbus_message_iter_arguments_append(iter, "ii", start, end);

   return ret;

fail:
   if (ret) eldbus_message_unref(ret);
   return NULL;
}

static const Eldbus_Method text_methods[] = {
   { "GetTextAtOffset", ELDBUS_ARGS({"i", "offset"}, {"u", "granularity"}), ELDBUS_ARGS({"s", "string"}, {"i", "startOffset"}, {"i", "endOffset"}), _text_string_at_offset_get, 0 },
   { "GetText", ELDBUS_ARGS({"i", "startOffset"}, {"i", "endOffset"}), ELDBUS_ARGS({"s", "string"}), _text_text_get, 0 },
   { "SetCaretOffset", ELDBUS_ARGS({"i", "offset"}), ELDBUS_ARGS({"b", NULL}), _text_caret_offset_set, 0 },
   { "GetCharacterAtOffset", ELDBUS_ARGS({"i", "offset"}), ELDBUS_ARGS({"i", NULL}), _text_character_at_offset_get, 0 },
   { "GetAttributeValue", ELDBUS_ARGS({"i", "offset"}, {"s", "attributeName"}), ELDBUS_ARGS({"s", NULL}, {"i", "startOffset"}, {"i", "endOffset"}, {"b", "defined"}), _text_attribute_value_get, 0 },
   { "GetAttributes", ELDBUS_ARGS({"i", "offset"}), ELDBUS_ARGS({"a(ss)", NULL}, {"i", "startOffset"}, {"i", "endOffset"}), _text_attributes_get, 0 },
   { "GetDefaultAttributes", NULL, ELDBUS_ARGS({"a(ss)", NULL}), _text_default_attributes_get, 0 },
   { "GetCharacterExtents", ELDBUS_ARGS({"i", "offset"}, {"u", "coordType"}), ELDBUS_ARGS({"i", "x"}, {"i", "y"}, {"i","w"}, {"i","h"}), _text_character_extents_get, 0 },
   { "GetOffsetAtPoint", ELDBUS_ARGS({"i", "x"}, {"i","y"}, {"u", "coordType"}), ELDBUS_ARGS({"i", NULL}), _text_offset_at_point_get, 0 },
   { "GetNSelections", NULL, ELDBUS_ARGS({"i", NULL}), _text_n_selections_get, 0 },
   { "GetSelection", ELDBUS_ARGS({"i", "selectionNum"}), ELDBUS_ARGS({"i", "startOffset"}, {"i", "endOffset"}), _text_selection_get, 0 },
   { "AddSelection", ELDBUS_ARGS({"i", "startOffset"}, {"i", "endOffset"}), ELDBUS_ARGS({"b", NULL}), _text_selection_add, 0 },
   { "RemoveSelection", ELDBUS_ARGS({"i", "selectionNum"}), ELDBUS_ARGS({"b", NULL}), _text_selection_remove, 0 },
   { "SetSelection", ELDBUS_ARGS({"i", "selectionNum"}, {"i", "startOffset"}, {"i", "endOffset"}), ELDBUS_ARGS({"b", NULL}), _text_selection_set, 0 },
   { "GetRangeExtents", ELDBUS_ARGS({"i", "startOffset"}, {"i", "endOffset"}, {"u", "coordType"}), ELDBUS_ARGS({"i", "x"}, {"i", "y"}, {"i","w"}, {"i","h"}), _text_range_extents_get, 0 },
   { "GetBoundedRanges", ELDBUS_ARGS({"i", "x"}, {"i", "y"}, {"i", "w"}, {"i", "h"}, {"u", "coordType"}, {"u", "xClipType"}, {"u", "yClipType"}), ELDBUS_ARGS({"a(issv)", NULL}), _text_bounded_ranges_get, 0 },
   { "GetAttributeRun", ELDBUS_ARGS({"i", "offset"}, {"b", "includeDefaults"}), ELDBUS_ARGS({"a(ss)", NULL}, {"i", "startOffset"}, {"i", "endOffset"}), _text_run_attributes_get, 0 },
   { NULL, NULL, NULL, NULL, 0 }
};

static Eldbus_Message *
_editable_text_text_content_set(const Eldbus_Service_Interface *iface, const Eldbus_Message *msg)
{
   const char *obj_path = eldbus_message_path_get(msg);
   Eo *bridge = eldbus_service_object_data_get(iface, ELM_ATSPI_BRIDGE_CLASS_NAME);
   Eo *obj = _bridge_object_from_path(bridge, obj_path);
   const char *content;
   Eldbus_Message *ret;
   Eina_Bool res;

   ELM_ATSPI_OBJ_CHECK_OR_RETURN_DBUS_ERROR(obj, EFL_ACCESS_EDITABLE_TEXT_INTERFACE, msg);

   if (!eldbus_message_arguments_get(msg, "s", &content))
     return eldbus_message_error_new(msg, "org.freedesktop.DBus.Error.InvalidArgs", "String expected.");

   ret = eldbus_message_method_return_new(msg);
   EINA_SAFETY_ON_NULL_RETURN_VAL(ret, NULL);

   res = efl_access_editable_text_content_set(obj, content);

   eldbus_message_arguments_append(ret, "b", res);

   return ret;
}

static Eldbus_Message *
_editable_text_text_insert(const Eldbus_Service_Interface *iface, const Eldbus_Message *msg)
{
   const char *obj_path = eldbus_message_path_get(msg);
   Eo *bridge = eldbus_service_object_data_get(iface, ELM_ATSPI_BRIDGE_CLASS_NAME);
   Eo *obj = _bridge_object_from_path(bridge, obj_path);
   const char *text;
   Eldbus_Message *ret;
   int pos, len;
   Eina_Bool res;

   ELM_ATSPI_OBJ_CHECK_OR_RETURN_DBUS_ERROR(obj, EFL_ACCESS_EDITABLE_TEXT_INTERFACE, msg);

   if (!eldbus_message_arguments_get(msg, "isi", &pos, &text, &len))
     return eldbus_message_error_new(msg, "org.freedesktop.DBus.Error.InvalidArgs", "Position, string, length expected.");

   ret = eldbus_message_method_return_new(msg);
   EINA_SAFETY_ON_NULL_RETURN_VAL(ret, NULL);

   res = efl_access_editable_text_insert(obj, text, pos);

   eldbus_message_arguments_append(ret, "b", res);

   return ret;
}

static Eldbus_Message *
_editable_text_text_copy(const Eldbus_Service_Interface *iface, const Eldbus_Message *msg)
{
   const char *obj_path = eldbus_message_path_get(msg);
   Eo *bridge = eldbus_service_object_data_get(iface, ELM_ATSPI_BRIDGE_CLASS_NAME);
   Eo *obj = _bridge_object_from_path(bridge, obj_path);
   Eldbus_Message *ret;
   int start, end;
   Eina_Bool res;

   ELM_ATSPI_OBJ_CHECK_OR_RETURN_DBUS_ERROR(obj, EFL_ACCESS_EDITABLE_TEXT_INTERFACE, msg);

   if (!eldbus_message_arguments_get(msg, "ii", &start, &end))
     return eldbus_message_error_new(msg, "org.freedesktop.DBus.Error.InvalidArgs", "Start and end index expected.");

   ret = eldbus_message_method_return_new(msg);
   EINA_SAFETY_ON_NULL_RETURN_VAL(ret, NULL);

   res = efl_access_editable_text_copy(obj, start, end);

   eldbus_message_arguments_append(ret, "b", res);

   return ret;
}

static Eldbus_Message *
_editable_text_text_cut(const Eldbus_Service_Interface *iface, const Eldbus_Message *msg)
{
   const char *obj_path = eldbus_message_path_get(msg);
   Eo *bridge = eldbus_service_object_data_get(iface, ELM_ATSPI_BRIDGE_CLASS_NAME);
   Eo *obj = _bridge_object_from_path(bridge, obj_path);
   Eldbus_Message *ret;
   int start, end;
   Eina_Bool res;

   ELM_ATSPI_OBJ_CHECK_OR_RETURN_DBUS_ERROR(obj, EFL_ACCESS_EDITABLE_TEXT_INTERFACE, msg);

   if (!eldbus_message_arguments_get(msg, "ii", &start, &end))
     return eldbus_message_error_new(msg, "org.freedesktop.DBus.Error.InvalidArgs", "Start and end index expected.");

   ret = eldbus_message_method_return_new(msg);
   EINA_SAFETY_ON_NULL_RETURN_VAL(ret, NULL);

   res = efl_access_editable_text_cut(obj, start, end);

   eldbus_message_arguments_append(ret, "b", res);

   return ret;
}

static Eldbus_Message *
_editable_text_text_delete(const Eldbus_Service_Interface *iface, const Eldbus_Message *msg)
{
   const char *obj_path = eldbus_message_path_get(msg);
   Eo *bridge = eldbus_service_object_data_get(iface, ELM_ATSPI_BRIDGE_CLASS_NAME);
   Eo *obj = _bridge_object_from_path(bridge, obj_path);
   Eldbus_Message *ret;
   int start, end;
   Eina_Bool res;

   ELM_ATSPI_OBJ_CHECK_OR_RETURN_DBUS_ERROR(obj, EFL_ACCESS_EDITABLE_TEXT_INTERFACE, msg);

   if (!eldbus_message_arguments_get(msg, "ii", &start, &end))
     return eldbus_message_error_new(msg, "org.freedesktop.DBus.Error.InvalidArgs", "Start and end index expected.");

   ret = eldbus_message_method_return_new(msg);
   EINA_SAFETY_ON_NULL_RETURN_VAL(ret, NULL);

   res = efl_access_editable_text_delete(obj, start, end);

   eldbus_message_arguments_append(ret, "b", res);

   return ret;
}

static Eldbus_Message *
_editable_text_text_paste(const Eldbus_Service_Interface *iface, const Eldbus_Message *msg)
{
   const char *obj_path = eldbus_message_path_get(msg);
   Eo *bridge = eldbus_service_object_data_get(iface, ELM_ATSPI_BRIDGE_CLASS_NAME);
   Eo *obj = _bridge_object_from_path(bridge, obj_path);
   Eldbus_Message *ret;
   int pos;
   Eina_Bool res;

   ELM_ATSPI_OBJ_CHECK_OR_RETURN_DBUS_ERROR(obj, EFL_ACCESS_EDITABLE_TEXT_INTERFACE, msg);

   if (!eldbus_message_arguments_get(msg, "i", &pos))
     return eldbus_message_error_new(msg, "org.freedesktop.DBus.Error.InvalidArgs", "Start and end index expected.");

   ret = eldbus_message_method_return_new(msg);
   EINA_SAFETY_ON_NULL_RETURN_VAL(ret, NULL);

   res = efl_access_editable_text_paste(obj, pos);

   eldbus_message_arguments_append(ret, "b", res);

   return ret;
}

//TIZEN_ONLY(20171108): make atspi_proxy work
Eina_Bool
_elm_atspi_bridge_plug_id_split(const char *plug_id, char **bus, char **path)
{
   if (!plug_id || !strcmp(plug_id, "")) return EINA_FALSE;
   unsigned int tokens = 0;
   char **split = eina_str_split_full(plug_id, ":", 0, &tokens);
   Eina_Bool ret = EINA_FALSE;
   if (tokens == 2)
     {
        if (!split[0] || !split[1])
          ret = EINA_FALSE;
        else
          {
             if (bus) *bus = strdup(split[0]);
             if (path) *path = strdup(split[1]);
             ret = EINA_TRUE;
          }
     }
   else if (tokens == 3)
     {
        if (!split[0] || !split[1] || !split[2])
          ret = EINA_FALSE;
        else
          {
             char buf[128];
             snprintf(buf, sizeof(buf), "%s.%s",split[0], split[1]);
             if (bus) *bus = strdup(buf);
             if (path) *path = strdup(split[2]);
             ret = EINA_TRUE;
          }
     }
   if (split)
     {
        free(split[0]);
        free(split);
     }
   return ret;
}

static Eldbus_Message *
_socket_embedded(const Eldbus_Service_Interface *iface EINA_UNUSED, const Eldbus_Message *msg)
{
   Eo *proxy;
   // TIZEN_ONLY(20160705) - enable atspi_proxy to work
   const char *obj_path = eldbus_message_path_get(msg);
   //
   const char *bus, *path;
   Eo *bridge = _elm_atspi_bridge_get();
   Eo *obj = _bridge_object_from_path(bridge, obj_path);
   proxy = efl_provider_find(efl_parent_get(obj), EFL_ACCESS_OBJECT_MIXIN);

   if (!efl_isa(proxy, ELM_ATSPI_PROXY_CLASS))
     return eldbus_message_error_new(msg, "org.freedesktop.DBus.Error.Failed", "Unable to embed object.");

   if (!eldbus_message_arguments_get(msg, "s", &path))
     return eldbus_message_error_new(msg, "org.freedesktop.DBus.Error.InvalidArgs", "Plug id expected.");

   bus = eldbus_message_sender_get(msg);

   elm_obj_atspi_proxy_address_set(proxy, bus, path);

   _bridge_cache_build(bridge, proxy);

   return eldbus_message_method_return_new(msg);
}
//

// TIZEN_ONLY(20160705) - enable atspi_proxy to work
static Eldbus_Message *
_socket_offset_set(const Eldbus_Service_Interface *iface EINA_UNUSED, const Eldbus_Message *msg)
{
   int x, y;
   Eo *parent;
   Eo *obj = eldbus_service_object_data_get(iface, "_atspi_obj");
   parent = efl_parent_get(obj);
   Evas_Object *top = elm_object_top_widget_get(parent);

   if (!eldbus_message_arguments_get(msg, "ii", &x, &y))
     return eldbus_message_error_new(msg, "org.freedesktop.DBus.Error.InvalidArgs", "Invalid index type.");

   efl_access_component_socket_offset_set(top, x, y);

   return eldbus_message_method_return_new(msg);
}
//

static const Eldbus_Method editable_text_methods[] = {
   { "SetTextContents", ELDBUS_ARGS({"s", "newcontents"}), ELDBUS_ARGS({"b", NULL}), _editable_text_text_content_set, 0 },
   { "InsertText", ELDBUS_ARGS({"i", "position"}, {"s", "text"}, {"i", "length"}), ELDBUS_ARGS({"b", NULL}), _editable_text_text_insert, 0 },
   { "CopyText", ELDBUS_ARGS({"i", "startPos"}, {"i", "endPos"}), NULL, _editable_text_text_copy, 0 },
   { "CutText", ELDBUS_ARGS({"i", "startPos"}, {"i", "endPos"}), ELDBUS_ARGS({"b", NULL}), _editable_text_text_cut, 0 },
   { "DeleteText", ELDBUS_ARGS({"i", "startPos"}, {"i", "endPos"}), ELDBUS_ARGS({"b", NULL}), _editable_text_text_delete, 0 },
   { "PasteText", ELDBUS_ARGS({"i", "position"}), ELDBUS_ARGS({"b", NULL}), _editable_text_text_paste, 0 },
   { NULL, NULL, NULL, NULL, 0 }
};

//TIZEN_ONLY(20171108): make atspi_proxy work
static const Eldbus_Method socket_methods[] = {
   { "Embedded", ELDBUS_ARGS({"s", "id"}), ELDBUS_ARGS({NULL, NULL}), _socket_embedded, 0 },
   // TIZEN_ONLY(20160705) - enable atspi_proxy to work
   { "SetOffset", ELDBUS_ARGS({"i", "x"}, {"i", "y"}), ELDBUS_ARGS({NULL, NULL}), _socket_offset_set, 0 },
   //
   { NULL, NULL, NULL, NULL, 0 }
};

static const Eldbus_Service_Interface_Desc socket_iface_desc = {
   ATSPI_DBUS_INTERFACE_SOCKET, socket_methods, NULL, NULL, NULL, NULL
};
//

static Eo *
_bridge_object_from_path(Eo *bridge, const char *path)
{
   EINA_SAFETY_ON_NULL_RETURN_VAL(path, NULL);
   unsigned long long eo_ptr = 0;
   Eo *eo = NULL;
   const char *tmp = path;
   Eo *ret, *root;

   int len = strlen(ELM_ACCESS_OBJECT_PATH_PREFIX);
   if (strncmp(path, ELM_ACCESS_OBJECT_PATH_PREFIX, len))
     return NULL;

   tmp = path + len; /* Skip over the prefix */
   if (!strcmp(ELM_ACCESS_OBJECT_PATH_ROOT, tmp))
     {
        root = efl_access_object_access_root_get(EFL_ACCESS_OBJECT_MIXIN);
        return root;
     }

   sscanf(tmp, "%llu", &eo_ptr);
   eo = (Eo *) (uintptr_t) eo_ptr;

   ELM_ATSPI_BRIDGE_DATA_GET_OR_RETURN_VAL(bridge, pd, NULL);
   if (!eina_hash_find(pd->cache, &eo))
     {
        WRN("Request for non-registered object: %s", path);
        return NULL;
     }

   ret = efl_isa(eo, EFL_ACCESS_OBJECT_MIXIN) ? eo : NULL;

   return ret;
}

static const char *
_path_from_object(const Eo *eo)
{
   static char path[64];
   Eo *root;

   if (!eo)
     return ATSPI_DBUS_PATH_NULL;
   root = efl_access_object_access_root_get(EFL_ACCESS_OBJECT_MIXIN);

   if (eo == root)
     snprintf(path, sizeof(path), "%s%s", ELM_ACCESS_OBJECT_PATH_PREFIX, ELM_ACCESS_OBJECT_PATH_ROOT);
   else
     snprintf(path, sizeof(path), ELM_ACCESS_OBJECT_REFERENCE_TEMPLATE, (unsigned long long)(uintptr_t)eo);
   return path;
}

static Eina_Bool
_accessible_property_get(const Eldbus_Service_Interface *interface, const char *property,
                         Eldbus_Message_Iter *iter, const Eldbus_Message *request_msg,
                         Eldbus_Message **error)
{
   const char *ret = NULL, *obj_path = eldbus_message_path_get(request_msg);
   Eo *bridge = eldbus_service_object_data_get(interface, ELM_ATSPI_BRIDGE_CLASS_NAME);
   Eo *ret_obj = NULL, *obj = _bridge_object_from_path(bridge, obj_path);

   ELM_ATSPI_PROPERTY_OBJ_CHECK_OR_RETURN_DBUS_ERROR(obj, EFL_ACCESS_OBJECT_MIXIN, request_msg, error);

   if (!strcmp(property, "Name"))
     {
        ret = efl_access_object_i18n_name_get(obj);
        if (!ret)
          ret = "";
        eldbus_message_iter_basic_append(iter, 's', ret);
        return EINA_TRUE;
     }
   else if (!strcmp(property, "Description"))
     {
        ret = efl_access_object_description_get(obj);
        if (!ret)
          ret = "";
        eldbus_message_iter_basic_append(iter, 's', ret);
        return EINA_TRUE;
     }
   else if (!strcmp(property, "Parent"))
     {
       ret_obj = efl_provider_find(efl_parent_get(obj), EFL_ACCESS_OBJECT_MIXIN);
       Efl_Access_Role role = EFL_ACCESS_ROLE_INVALID;
       role = efl_access_object_role_get(obj);
       if ((!ret_obj) && (EFL_ACCESS_ROLE_APPLICATION == role))
         _object_desktop_reference_append(iter);
       else
         _bridge_iter_object_reference_append(bridge, iter, ret_obj);
       return EINA_TRUE;
     }
   else if (!strcmp(property, "ChildCount"))
     {
        Eina_List *l = NULL;
        l = efl_access_object_access_children_get(obj);
        eldbus_message_iter_basic_append(iter, 'i', eina_list_count(l));
        eina_list_free(l);
        return EINA_TRUE;
     }
   return EINA_FALSE;
}

static Eina_Bool
_selection_property_get(const Eldbus_Service_Interface *interface, const char *property,
                         Eldbus_Message_Iter *iter, const Eldbus_Message *request_msg,
                         Eldbus_Message **error)
{
   int n;
   const char *obj_path = eldbus_message_path_get(request_msg);
   Eo *bridge = eldbus_service_object_data_get(interface, ELM_ATSPI_BRIDGE_CLASS_NAME);
   Eo *obj = _bridge_object_from_path(bridge, obj_path);

   ELM_ATSPI_PROPERTY_OBJ_CHECK_OR_RETURN_DBUS_ERROR(obj, EFL_ACCESS_SELECTION_INTERFACE, request_msg, error);

   if (!strcmp(property, "NSelectedChildren"))
     {
        n = efl_access_selection_selected_children_count_get(obj);
        eldbus_message_iter_basic_append(iter, 'i', n);
        return EINA_TRUE;
     }
   return EINA_FALSE;
}

static Eina_Bool
_action_property_get(const Eldbus_Service_Interface *interface, const char *property,
                         Eldbus_Message_Iter *iter, const Eldbus_Message *request_msg,
                         Eldbus_Message **error)
{
   Eina_List *actions;
   const char *obj_path = eldbus_message_path_get(request_msg);
   Eo *bridge = eldbus_service_object_data_get(interface, ELM_ATSPI_BRIDGE_CLASS_NAME);
   Eo *obj = _bridge_object_from_path(bridge, obj_path);

   ELM_ATSPI_PROPERTY_OBJ_CHECK_OR_RETURN_DBUS_ERROR(obj, EFL_ACCESS_ACTION_MIXIN, request_msg, error);

   if (!strcmp(property, "NActions"))
     {
        actions = efl_access_action_actions_get(obj);
        eldbus_message_iter_basic_append(iter, 'i', eina_list_count(actions));
        eina_list_free(actions);
        return EINA_TRUE;
     }
   return EINA_FALSE;
}

static Eldbus_Message*
_value_properties_set(const Eldbus_Service_Interface *interface, const char *property,
                         Eldbus_Message_Iter *iter, const Eldbus_Message *request_msg)
{
   double value;
   Eina_Bool ret;
   const char *obj_path = eldbus_message_path_get(request_msg);
   Eo *bridge = eldbus_service_object_data_get(interface, ELM_ATSPI_BRIDGE_CLASS_NAME);
   Eo *obj = _bridge_object_from_path(bridge, obj_path);

   ELM_ATSPI_OBJ_CHECK_OR_RETURN_DBUS_ERROR(obj, EFL_ACCESS_VALUE_INTERFACE, request_msg);

   if (!eldbus_message_iter_arguments_get(iter, "d", &value))
     {
       return eldbus_message_error_new(request_msg, "org.freedesktop.DBus.Error.InvalidArgs", "Expected value of type: double.");
     }

   if (!strcmp(property, "CurrentValue"))
     {
        ret = efl_access_value_and_text_set(obj, value, NULL);
        Eldbus_Message *answer = eldbus_message_method_return_new(request_msg);
        eldbus_message_arguments_append(answer, "b", ret);
        return answer;
     }

   return NULL;
}

static Eina_Bool
_value_properties_get(const Eldbus_Service_Interface *interface, const char *property,
                         Eldbus_Message_Iter *iter, const Eldbus_Message *request_msg,
                         Eldbus_Message **error)
{
   double value;
   const char *obj_path = eldbus_message_path_get(request_msg);
   Eo *bridge = eldbus_service_object_data_get(interface, ELM_ATSPI_BRIDGE_CLASS_NAME);
   Eo *obj = _bridge_object_from_path(bridge, obj_path);

   ELM_ATSPI_PROPERTY_OBJ_CHECK_OR_RETURN_DBUS_ERROR(obj, EFL_ACCESS_VALUE_INTERFACE, request_msg, error);

   if (!strcmp(property, "CurrentValue"))
     {
        efl_access_value_and_text_get(obj, &value, NULL);
        eldbus_message_iter_basic_append(iter, 'd', value);
        return EINA_TRUE;
     }
   if (!strcmp(property, "MinimumValue"))
     {
        efl_access_value_range_get(obj, &value, NULL, NULL);
        eldbus_message_iter_basic_append(iter, 'd', value);
        return EINA_TRUE;
     }
   if (!strcmp(property, "MaximumValue"))
     {
        efl_access_value_range_get(obj, NULL, &value, NULL);
        eldbus_message_iter_basic_append(iter, 'd', value);
        return EINA_TRUE;
     }
   if (!strcmp(property, "MinimumIncrement"))
     {
        value = efl_access_value_increment_get(obj);
        eldbus_message_iter_basic_append(iter, 'd', value);
        return EINA_TRUE;
     }
   return EINA_FALSE;
}

static Eina_Bool
_image_properties_get(const Eldbus_Service_Interface *interface, const char *property,
                         Eldbus_Message_Iter *iter, const Eldbus_Message *request_msg,
                         Eldbus_Message **error)
{
   const char *value;
   const char *obj_path = eldbus_message_path_get(request_msg);
   Eo *bridge = eldbus_service_object_data_get(interface, ELM_ATSPI_BRIDGE_CLASS_NAME);
   Eo *obj = _bridge_object_from_path(bridge, obj_path);

   ELM_ATSPI_PROPERTY_OBJ_CHECK_OR_RETURN_DBUS_ERROR(obj, EFL_ACCESS_OBJECT_MIXIN, request_msg, error);

   if (!strcmp(property, "ImageDescription"))
     {
        value = efl_access_object_description_get(obj);
        value = value ? value : "";
        eldbus_message_iter_basic_append(iter, 's', value);
        return EINA_TRUE;
     }
   if (!strcmp(property, "ImageLocale"))
     {
        value = efl_access_object_translation_domain_get(obj);
        value = value ? value : "";
        eldbus_message_iter_basic_append(iter, 's', value);
        return EINA_TRUE;
     }
   return EINA_FALSE;
}

static Eina_Bool
_text_properties_get(const Eldbus_Service_Interface *interface, const char *property,
                         Eldbus_Message_Iter *iter, const Eldbus_Message *request_msg,
                         Eldbus_Message **error)
{
   const char *obj_path = eldbus_message_path_get(request_msg);
   Eo *bridge = eldbus_service_object_data_get(interface, ELM_ATSPI_BRIDGE_CLASS_NAME);
   Eo *obj = _bridge_object_from_path(bridge, obj_path);
   int val;

   ELM_ATSPI_PROPERTY_OBJ_CHECK_OR_RETURN_DBUS_ERROR(obj, EFL_ACCESS_TEXT_INTERFACE, request_msg, error);

   if (!strcmp(property, "CharacterCount"))
     {
        val = efl_access_text_character_count_get(obj);
        eldbus_message_iter_basic_append(iter, 'i', val);
        return EINA_TRUE;
     }
   if (!strcmp(property, "CaretOffset"))
     {
        val = efl_access_text_caret_offset_get(obj);
        eldbus_message_iter_basic_append(iter, 'i', val);
        return EINA_TRUE;
     }
   return EINA_FALSE;
}

static Eldbus_Message*
_application_properties_set(const Eldbus_Service_Interface *iface, const char *property, Eldbus_Message_Iter *iter, const Eldbus_Message *input_msg)
{
   Eo *bridge = eldbus_service_object_data_get(iface, ELM_ATSPI_BRIDGE_CLASS_NAME);
   const char *obj_path = eldbus_message_path_get(input_msg);
   Eo *obj = _bridge_object_from_path(bridge, obj_path);
   int value;

   ELM_ATSPI_BRIDGE_DATA_GET_OR_RETURN_VAL(bridge, pd, NULL);
   ELM_ATSPI_OBJ_CHECK_OR_RETURN_DBUS_ERROR(obj, ELM_ATSPI_APP_OBJECT_CLASS, input_msg);

   if (!eldbus_message_iter_arguments_get(iter, "i", &value))
     {
       return eldbus_message_error_new(input_msg, "org.freedesktop.DBus.Error.InvalidArgs", "Expected value of type: int.");
     }

   if (!strcmp(property, "Id"))
     {
        pd->id = value;
        Eldbus_Message *answer = eldbus_message_method_return_new(input_msg);
        eldbus_message_arguments_append(answer, "b", EINA_TRUE);
        return answer;
     }

   return NULL;
}

static Eina_Bool
_application_properties_get(const Eldbus_Service_Interface *interface, const char *property,
                         Eldbus_Message_Iter *iter, const Eldbus_Message *request_msg,
                         Eldbus_Message **error)
{
   const char *obj_path = eldbus_message_path_get(request_msg);
   Eo *bridge = eldbus_service_object_data_get(interface, ELM_ATSPI_BRIDGE_CLASS_NAME);
   Eo *obj = _bridge_object_from_path(bridge, obj_path);

   ELM_ATSPI_BRIDGE_DATA_GET_OR_RETURN_VAL(bridge, pd, EINA_FALSE);
   ELM_ATSPI_PROPERTY_OBJ_CHECK_OR_RETURN_DBUS_ERROR(obj, ELM_ATSPI_APP_OBJECT_CLASS, request_msg, error);

   if (!strcmp(property, "ToolkitName"))
     {
        eldbus_message_iter_basic_append(iter, 's', "elementary");
        return EINA_TRUE;
     }
   if (!strcmp(property, "Version"))
     {
        char buf[64];
        snprintf(buf, sizeof(buf), "%d.%d", ELM_VERSION_MAJOR, ELM_VERSION_MINOR);
        eldbus_message_iter_basic_append(iter, 's', buf);
        return EINA_TRUE;
     }
   if (!strcmp(property, "Id"))
     {
        eldbus_message_iter_basic_append(iter, 'i', pd->id);
        return EINA_TRUE;
     }
   return EINA_FALSE;
}

static const Eldbus_Property accessible_properties[] = {
   { "Name", "s", NULL, NULL, 0 },
   { "Description", "s", NULL, NULL, 0 },
   { "Parent", "(so)", NULL, NULL, 0 },
   { "ChildCount", "i", NULL, NULL, 0 },
   { NULL, NULL, NULL, NULL, 0 }
};

static const Eldbus_Property action_properties[] = {
   { "NActions", "i", _action_property_get, NULL, 0 },
   { NULL, NULL, NULL, NULL, 0 }
};

static const Eldbus_Property value_properties[] = {
   { "MinimumValue", "d", NULL, NULL, 0 },
   { "MaximumValue", "d", NULL, NULL, 0 },
   { "MinimumIncrement", "d", NULL, NULL, 0 },
   { "CurrentValue", "d", NULL, NULL, 0 },
   { NULL, NULL, NULL, NULL, 0 }
};

static const Eldbus_Property image_properties[] = {
   { "ImageDescription", "s", NULL, NULL, 0 },
   { "ImageLocale", "s", NULL, NULL, 0 },
   { NULL, NULL, NULL, NULL, 0 }
};

static const Eldbus_Property selection_properties[] = {
   { "NSelectedChildren", "i", _selection_property_get, NULL, 0 },
   { NULL, NULL, NULL, NULL, 0 }
};

static const Eldbus_Property text_properties[] = {
   { "CharacterCount", "i", NULL, NULL, 0 },
   { "CaretOffset", "i", NULL, NULL, 0 },
   { NULL, NULL, NULL, NULL, 0 }
};

static const Eldbus_Property application_properties[] = {
   { "ToolkitName", "s", NULL, NULL, 0 },
   { "Version", "s", NULL, NULL, 0 },
   { "Id", "i", NULL, NULL, 0 },
   { NULL, NULL, NULL, NULL, 0 }
};

static const Eldbus_Service_Interface_Desc accessible_iface_desc = {
   ATSPI_DBUS_INTERFACE_ACCESSIBLE, accessible_methods, NULL, accessible_properties, _accessible_property_get, NULL
};

static const Eldbus_Service_Interface_Desc action_iface_desc = {
   ATSPI_DBUS_INTERFACE_ACTION, action_methods, NULL, action_properties, NULL, NULL
};

static const Eldbus_Service_Interface_Desc value_iface_desc = {
   ATSPI_DBUS_INTERFACE_VALUE, NULL, NULL, value_properties, _value_properties_get, _value_properties_set
};

static const Eldbus_Service_Interface_Desc image_iface_desc = {
   ATSPI_DBUS_INTERFACE_IMAGE, image_methods, NULL, image_properties, _image_properties_get, NULL
};

static const Eldbus_Service_Interface_Desc selection_iface_desc = {
   ATSPI_DBUS_INTERFACE_SELECTION, selection_methods, NULL, selection_properties, NULL, NULL
};

static const Eldbus_Service_Interface_Desc text_iface_desc = {
   ATSPI_DBUS_INTERFACE_TEXT, text_methods, NULL, text_properties, _text_properties_get, NULL
};

static const Eldbus_Service_Interface_Desc editable_text_iface_desc = {
   ATSPI_DBUS_INTERFACE_EDITABLE_TEXT, editable_text_methods, NULL, NULL, NULL, NULL
};

static const Eldbus_Service_Interface_Desc application_iface_desc = {
   ATSPI_DBUS_INTERFACE_APPLICATION, NULL, NULL, application_properties, _application_properties_get, _application_properties_set
};

void
_collection_match_rule_free(struct collection_match_rule *rule)
{
   Efl_Access_Attribute *attr;
   eina_list_free(rule->ifaces);
   EINA_LIST_FREE(rule->attributes, attr)
     {
        eina_stringshare_del(attr->key);
        eina_stringshare_del(attr->value);
     }
}

static void
_collection_roles_convert(uint64_t roles[2])
{
   // Currently elm roles and atspi roles are binary compatible.
   // Implement this function when it will be needed.
   (void)roles;
}

static Eina_Bool
_collection_iter_match_rule_get(Eldbus_Message_Iter *iter, struct collection_match_rule *rule)
{
   Eldbus_Message_Iter *states_iter, *attrib_iter, *iter_arg, *role_iter, *ifc_iter;
   unsigned int *array;
   int array_count, state_match, attrib_match, role_match, ifc_match, reverse;
   const char *ifc_name;

   if (!eldbus_message_iter_arguments_get(iter, "aiia{ss}iaiiasib", &states_iter, &state_match, &attrib_iter, &attrib_match, &role_iter, &role_match, &ifc_iter, &ifc_match, &reverse))
     {
        ERR("Unable to get message arguments");
        return EINA_FALSE;
     }

   memset(rule, 0x0, sizeof(struct collection_match_rule));
   rule->statematchtype = state_match;
   rule->attributematchtype = attrib_match;
   rule->rolematchtype = role_match;
   rule->interfacematchtype = ifc_match;
   rule->reverse = reverse;

   if (!eldbus_message_iter_fixed_array_get(states_iter, 'i', &array, &array_count))
     return EINA_FALSE;

   //Roles according to libatspi implementation are transferred in 2-int element fixed bit array
   if (array_count != 2)
     {
        ERR("Unexpected states array size");
        return EINA_FALSE;
     }
   uint64_t states = ((uint64_t)array[0] | ((uint64_t)array[1] << 32));
   rule->states = _atspi_state_set_to_elm_atspi_state_set(states);

   //Roles according to libatspi implementation are transferred in 4-int element fixed bit array
   if (!eldbus_message_iter_fixed_array_get(role_iter, 'i', &array, &array_count))
     return EINA_FALSE;

   if (array_count != 4)
     {
        ERR("Unexpected roles array size");
        return EINA_FALSE;
     }

   //convert atspi roles to elm_roles
   rule->roles[0] = ((uint64_t)array[0] | ((uint64_t)array[1] << 32));
   rule->roles[1] = ((uint64_t)array[2] | ((uint64_t)array[3] << 32));

   _collection_roles_convert(rule->roles);

   //Get matching properties
   while (eldbus_message_iter_get_and_next(attrib_iter, '{', &iter_arg))
     {
        const char *key, *value;
        if (eldbus_message_iter_arguments_get(iter_arg, "ss", &key, &value))
          {
             Efl_Access_Attribute *attrib = calloc(1, sizeof(Efl_Access_Attribute));
             attrib->key = eina_stringshare_add(key);
             attrib->value = eina_stringshare_add(value);
             rule->attributes = eina_list_append(rule->attributes, attrib);
          }
     }

   //Get interfaces to match
   while (eldbus_message_iter_get_and_next(ifc_iter, 's', &ifc_name))
     {
        const Efl_Class *class = NULL;
        if (!strcmp(ifc_name, "action"))
          class = EFL_ACCESS_ACTION_MIXIN;
        else if (!strcmp(ifc_name, "component"))
          class = EFL_ACCESS_COMPONENT_MIXIN;
        else if (!strcmp(ifc_name, "editabletext"))
          class = EFL_ACCESS_EDITABLE_TEXT_INTERFACE;
        else if (!strcmp(ifc_name, "text"))
          class = EFL_ACCESS_TEXT_INTERFACE;
        else if (!strcmp(ifc_name, "image"))
          class = EFL_ACCESS_SELECTION_INTERFACE;
        else if (!strcmp(ifc_name, "value"))
          class = EFL_ACCESS_VALUE_INTERFACE;

        if (class)
          rule->ifaces = eina_list_append(rule->ifaces, class);
        else
          {
             _collection_match_rule_free(rule);
             return EINA_FALSE;
          }
     }

   return EINA_TRUE;
}

static Eina_Bool
_collection_match_interfaces_helper(Eo *obj, Eina_List *ifcs, Eina_Bool condition, Eina_Bool ret_if_true, Eina_Bool ret_if_false)
{
   Efl_Class *class;
   Eina_List *l;

   EINA_LIST_FOREACH(ifcs, l, class)
     {
        if (efl_isa(obj, class) == condition)
          return ret_if_true;
     }
   return ret_if_false;
}

static Eina_Bool
_collection_match_interfaces_lookup(Eo *obj, struct collection_match_rule *rule)
{
   Eina_Bool ret = EINA_FALSE;

   switch (rule->interfacematchtype)
     {
        case ATSPI_Collection_MATCH_INVALID:
           ret = EINA_TRUE;
           break;
        case ATSPI_Collection_MATCH_ALL:
           ret = _collection_match_interfaces_helper(
              obj, rule->ifaces, EINA_FALSE, EINA_FALSE, EINA_TRUE);
           break;
        case ATSPI_Collection_MATCH_ANY:
           ret = _collection_match_interfaces_helper(
              obj, rule->ifaces, EINA_TRUE, EINA_TRUE, EINA_FALSE);
           break;
        case ATSPI_Collection_MATCH_NONE:
           ret = _collection_match_interfaces_helper(
              obj, rule->ifaces, EINA_TRUE, EINA_FALSE, EINA_TRUE);
           break;
        default:
           break;
     }
   return ret;
}

static Eina_Bool
_collection_match_states_lookup(Eo *obj, struct collection_match_rule *rule)
{
   Eina_Bool ret = EINA_FALSE;
   Efl_Access_State_Set ss;

   ss = efl_access_object_state_set_get(obj);

   switch (rule->statematchtype)
     {
        case ATSPI_Collection_MATCH_INVALID:
           ret = EINA_TRUE;
           break;
        case ATSPI_Collection_MATCH_ALL:
           ret = (ss & rule->states) == rule->states;
           break;
        case ATSPI_Collection_MATCH_ANY:
           ret = (ss & rule->states) > 0;
           break;
        case ATSPI_Collection_MATCH_NONE:
           ret = (ss & rule->states) == 0;
           break;
        default:
           break;
     }

   return ret;
}

static Eina_Bool
_collection_match_roles_lookup(Eo *obj, struct collection_match_rule *rule)
{
   Eina_Bool ret = EINA_FALSE;
   Efl_Access_Role role;
   int64_t role_set;

   role = efl_access_object_role_get(obj);

   if (role >= 64)
     {
        role -= 64;
        role_set = rule->roles[1];
     }
   else
     role_set = rule->roles[0];

   if (role >= 64)
     {
        ERR("Efl_Access_Role enum value exceeds 127. Unable to compare with roles bit field.");
        return EINA_FALSE;
     }

   switch (rule->rolematchtype)
     {
        case ATSPI_Collection_MATCH_INVALID:
           ret = EINA_TRUE;
           break;
        case ATSPI_Collection_MATCH_ALL:
        case ATSPI_Collection_MATCH_ANY:
           ret = (role_set & (1ULL << role)) > 0;
           break;
        case ATSPI_Collection_MATCH_NONE:
           ret = (role_set & (1ULL << role)) == 0;
           break;
        default:
           break;
     }

   return ret;
}

static Eina_Bool
_collection_match_attributes_helper(Eina_List *obj_attribs, Eina_List *attribs, AtspiCollectionMatchType mode)
{
   Eina_List *l, *l2;
   Efl_Access_Attribute *attr, *attr2;
   Eina_Bool obj_empty = eina_list_count(obj_attribs) == 0;
   Eina_Bool empty = eina_list_count(attribs) == 0;

   switch (mode)
     {
        case ATSPI_Collection_MATCH_ANY:
           if (empty || obj_empty) return EINA_FALSE;
           break;
        case ATSPI_Collection_MATCH_ALL:
           if (empty) return EINA_TRUE;
           if (obj_empty) return EINA_FALSE;
           break;
        case ATSPI_Collection_MATCH_NONE:
           if (empty || obj_empty) return EINA_TRUE;
           break;
        case ATSPI_Collection_MATCH_EMPTY:
           if (empty && obj_empty) return EINA_TRUE;
           if (empty || obj_empty) return EINA_FALSE;
           break;
        case ATSPI_Collection_MATCH_INVALID:
        case ATSPI_Collection_MATCH_LAST_DEFINED:
           assert(0);
           break;
     }
   EINA_LIST_FOREACH(attribs, l, attr)
     {
       Eina_Bool found = EINA_FALSE;
       EINA_LIST_FOREACH(obj_attribs, l2, attr2)
         {
            Eina_Bool compare = (attr->key && attr2->key &&
                 attr->value && attr2->value &&
                 !strcmp(attr->key, attr2->key) &&
                 !strcmp(attr->value, attr2->value));
            if (compare)
              {
                 found = EINA_TRUE;
                 break;
              }
         }
       switch (mode)
         {
            case ATSPI_Collection_MATCH_EMPTY:
            case ATSPI_Collection_MATCH_ALL:
               if (!found) return EINA_FALSE;
               break;
            case ATSPI_Collection_MATCH_ANY:
               if (found) return EINA_TRUE;
               break;
            case ATSPI_Collection_MATCH_NONE:
               if (found) return EINA_FALSE;
               break;
            case ATSPI_Collection_MATCH_INVALID:
            case ATSPI_Collection_MATCH_LAST_DEFINED:
               assert(0);
               break;
         }
     }

   switch (mode)
     {
        case ATSPI_Collection_MATCH_EMPTY:
        case ATSPI_Collection_MATCH_ALL:
        case ATSPI_Collection_MATCH_NONE:
           return EINA_TRUE;
        case ATSPI_Collection_MATCH_ANY:
           return EINA_FALSE;
        case ATSPI_Collection_MATCH_INVALID:
        case ATSPI_Collection_MATCH_LAST_DEFINED:
           assert(0);
           break;
     }
   return EINA_FALSE;
}

static Eina_Bool
_collection_match_attributes_lookup(Eo *obj, struct collection_match_rule *rule)
{
   Eina_Bool ret = EINA_FALSE;
   Eina_List *obj_attribs;

   obj_attribs = efl_access_object_attributes_get(obj);

   switch (rule->attributematchtype)
     {
        case ATSPI_Collection_MATCH_INVALID:
           ret = EINA_TRUE;
           break;
        case ATSPI_Collection_MATCH_ALL:
        case ATSPI_Collection_MATCH_ANY:
        case ATSPI_Collection_MATCH_NONE:
        case ATSPI_Collection_MATCH_EMPTY:
           ret = _collection_match_attributes_helper(obj_attribs, rule->attributes, rule->attributematchtype);
           break;
        default:
           DBG("invalid match type");
           break;
     }

   efl_access_attributes_list_free(obj_attribs);

   return ret;
}

static int
_collection_sort_order_canonical(struct collection_match_rule *rule, Eina_List **ls,
                      int count, int max,
                      Eo *obj, long index, Eina_Bool flag,
                      Eo *pobj, Eina_Bool recurse, Eina_Bool traverse)
{
   int i = index;
   Eina_List *children;
   children = efl_access_object_access_children_get(obj);
   long acount = eina_list_count(children);
   Eina_Bool prev = pobj ? EINA_TRUE : EINA_FALSE;

   for (; i < acount && (max == 0 || count < max); i++)
     {
        Eo *child = eina_list_nth(children, i);

        if (prev && child == pobj)
          {
             eina_list_free(children);
             return count;
          }

        if (flag && _collection_match_interfaces_lookup(child, rule)
            && _collection_match_states_lookup(child, rule)
            && _collection_match_roles_lookup(child, rule)
            && _collection_match_attributes_lookup(child, rule))
          {
             *ls = eina_list_append(*ls, child);
             count++;
          }

       if (!flag)
         flag = EINA_TRUE;

       if (recurse && traverse)
         count = _collection_sort_order_canonical(rule, ls, count,
                                                  max, child, 0, EINA_TRUE,
                                                  pobj, recurse, traverse);
     }
   eina_list_free(children);
   return count;
}

static int
_collection_sort_order_reverse_canonical(struct collection_match_rule *rule, Eina_List **ls,
                      int count, int max, Eo *obj, Eina_Bool flag, Eo *pobj)
{
  Eo *nextobj, *parent;
  long indexinparent;
  Eina_List *children;

  /* This breaks us out of the recursion. */
  if (!obj || obj == pobj)
    {
      return count;
    }

  /* Add to the list if it matches */
  if (flag && _collection_match_interfaces_lookup(obj, rule)
      && _collection_match_states_lookup(obj, rule)
      && _collection_match_roles_lookup(obj, rule)
      && _collection_match_attributes_lookup(obj, rule)
      && (max == 0 || count < max))
    {
       *ls = eina_list_append(*ls, obj);
       count++;
    }

  if (!flag)
    flag = EINA_TRUE;

  /* Get the current nodes index in it's parent and the parent object. */
  indexinparent = efl_access_object_index_in_parent_get(obj);
  parent = efl_provider_find(efl_parent_get(obj), EFL_ACCESS_OBJECT_MIXIN);

  if ((indexinparent > 0) && ((max == 0) || (count < max)))
    {
       /* there are still some siblings to visit so get the previous sibling
          and get it's last descendant.
          First, get the previous sibling */
       children = efl_access_object_access_children_get(parent);
       nextobj = eina_list_nth(children, indexinparent - 1);
       eina_list_free(children);

       /* Now, drill down the right side to the last descendant */
       do {
            children = efl_access_object_access_children_get(nextobj);
            if (children) nextobj = eina_list_last_data_get(children);
            eina_list_free(children);
       } while (children);

       /* recurse with the last descendant */
       count = _collection_sort_order_reverse_canonical(rule, ls, count, max,
                                       nextobj, EINA_TRUE, pobj);
    }
  else if (max == 0 || count < max)
    {
      /* no more siblings so next node must be the parent */
      count = _collection_sort_order_reverse_canonical(rule, ls, count, max,
                                        parent, EINA_TRUE, pobj);

    }
  return count;
}

static int
_collection_inbackorder(Eo *collection, struct collection_match_rule *rule, Eina_List **list,
                        int max, Eo *obj)
{
   *list = eina_list_append(*list, obj);

   _collection_sort_order_reverse_canonical(rule, list, 0, max, obj, EINA_TRUE, collection);

   *list = eina_list_remove_list(*list, *list);

   return 0;
}

static int
_collection_inorder(Eo *collection, struct collection_match_rule *rule, Eina_List **list,
                    int count, int max, Eo *obj, Eina_Bool traverse)
{
   int idx = 0;

   count = _collection_sort_order_canonical(rule, list, count, max, obj, 0, EINA_TRUE, NULL, EINA_TRUE, traverse);

  while ((max == 0 || count < max) && obj && obj != collection)
    {
       Eo *parent;
       parent = efl_provider_find(efl_parent_get(obj), EFL_ACCESS_OBJECT_MIXIN);
       idx = efl_access_object_index_in_parent_get(obj);
       count = _collection_sort_order_canonical(rule, list, count, max, parent,
                                     idx + 1, EINA_TRUE, NULL, EINA_TRUE, traverse);
       obj = parent;
    }

  if (max == 0 || count < max)
    count = _collection_sort_order_canonical(rule, list, count, max,
                                    obj, idx + 1, EINA_TRUE, NULL, EINA_TRUE, traverse);

  return count;
}

static int
_collection_query(struct collection_match_rule *rule, AtspiCollectionSortOrder sortby,
                         Eina_List **list, int count, int max, Eo *obj, long index,
                         Eina_Bool flag, Eo *pobj, Eina_Bool recurse, Eina_Bool traverse)
{
   switch (sortby)
     {
        case ATSPI_Collection_SORT_ORDER_CANONICAL:
           count = _collection_sort_order_canonical(rule, list, 0, max, obj, index, flag,
                                                    pobj, recurse, traverse);
           break;
        case ATSPI_Collection_SORT_ORDER_REVERSE_CANONICAL:
           count = _collection_sort_order_canonical(rule, list, 0, max, obj, index, flag,
                                                    pobj, recurse, traverse);
           *list = eina_list_reverse(*list);
           break;
        default:
          count = 0;
          WRN("Unhandled sort method");
          break;
     }
   return count;
}

static Eldbus_Message*
_collection_return_msg_from_list(Elm_Atspi_Bridge *bridge, const Eldbus_Message *msg, const Eina_List *objs)
{
   Eldbus_Message *ret;
   const Eina_List *l;
   Eldbus_Message_Iter *iter, *array_iter;
   Eo *obj;

   ret = eldbus_message_method_return_new(msg);
   if (!ret) return NULL;

   iter = eldbus_message_iter_get(ret);
   array_iter = eldbus_message_iter_container_new(iter, 'a', "(so)");

   EINA_LIST_FOREACH(objs, l, obj)
     {
        _bridge_object_register(bridge, obj);
        _bridge_iter_object_reference_append(bridge, array_iter, obj);
     }

   eldbus_message_iter_container_close(iter, array_iter);
   return ret;
}

static Eina_List*
_collection_get_matches_from_handle(Eo *collection, Eo *current, struct collection_match_rule *rule, AtspiCollectionSortOrder sortby, AtspiCollectionTreeTraversalType tree, int max, Eina_Bool traverse)
{
   Eina_List *result = NULL;
   Eo *parent;
   int idx;

   switch (tree)
     {
      case ATSPI_Collection_TREE_INORDER:
           _collection_inorder(collection, rule, &result, 0, max, current, traverse);
         if (sortby == ATSPI_Collection_SORT_ORDER_REVERSE_CANONICAL)
           result = eina_list_reverse(result);
         break;
      case ATSPI_Collection_TREE_RESTRICT_CHILDREN:
         idx = efl_access_object_index_in_parent_get(current);
         parent = efl_provider_find(efl_parent_get(current), EFL_ACCESS_OBJECT_MIXIN);
         _collection_query(rule, sortby, &result, 0, max, parent, idx, EINA_FALSE, NULL, EINA_TRUE, traverse);
         break;
      case ATSPI_Collection_TREE_RESTRICT_SIBLING:
         _collection_query(rule, sortby, &result, 0, max, current, 0, EINA_FALSE, NULL, EINA_TRUE, traverse);
         break;
      default:
         ERR("Tree parameter value not handled");
         break;
     }
   return result;
}

static Eldbus_Message*
_collection_get_matches_from(const Eldbus_Service_Interface *iface EINA_UNUSED, const Eldbus_Message *msg EINA_UNUSED)
{
   const char *obj_path = eldbus_message_path_get(msg);
   Eo *bridge = eldbus_service_object_data_get(iface, ELM_ATSPI_BRIDGE_CLASS_NAME);
   Eo *current, *obj = _bridge_object_from_path(bridge, obj_path);
   Eldbus_Message *ret;
   Eldbus_Message_Iter *iter, *rule_iter;
   struct collection_match_rule rule;
   int count;
   AtspiCollectionTreeTraversalType tree;
   Eina_Bool traverse;
   AtspiCollectionSortOrder sortby;
   Eina_List *result = NULL;

   ELM_ATSPI_OBJ_CHECK_OR_RETURN_DBUS_ERROR(obj, EFL_ACCESS_OBJECT_MIXIN, msg);

   iter = eldbus_message_iter_get(msg);
   EINA_SAFETY_ON_NULL_RETURN_VAL(iter, NULL);

   if (!eldbus_message_iter_arguments_get(iter, "o(aiia{ss}iaiiasib)uuib", &obj_path, &rule_iter, &sortby, &tree, &count, &traverse))
     {
        return eldbus_message_error_new(msg, "org.freedesktop.DBus.Error.Failed", "Unable to get matchule, sortby, count or traverse values.");
     }

   current = _bridge_object_from_path(bridge, obj_path);

   ELM_ATSPI_OBJ_CHECK_OR_RETURN_DBUS_ERROR(current, EFL_ACCESS_OBJECT_MIXIN, msg);

   if (!_collection_iter_match_rule_get(rule_iter, &rule))
     return eldbus_message_error_new(msg, "org.freedesktop.DBus.Error.Failed", "Invalid match rule parameters.");

   result = _collection_get_matches_from_handle(obj, current, &rule, sortby, tree, count, traverse);
   ret = _collection_return_msg_from_list(bridge, msg, result);

   eina_list_free(result);
   _collection_match_rule_free(&rule);

   return ret;
}

static Eina_List*
_collection_get_matches_to_handle(Eo *obj, Eo *current, struct collection_match_rule *rule, AtspiCollectionSortOrder sortby, AtspiCollectionTreeTraversalType tree, Eina_Bool limit, int max, Eina_Bool traverse)
{
   Eina_List *result = NULL;
   Eo *collection = obj;

   if (limit)
     collection = efl_provider_find(efl_parent_get(obj), EFL_ACCESS_OBJECT_MIXIN);

   switch (tree)
     {
      case ATSPI_Collection_TREE_INORDER:
         _collection_inbackorder(obj, rule, &result, max, current);
         if (sortby == ATSPI_Collection_SORT_ORDER_REVERSE_CANONICAL)
           result = eina_list_reverse(result);
         break;
      case ATSPI_Collection_TREE_RESTRICT_CHILDREN:
         _collection_query(rule, sortby, &result, 0, max, collection, 0, EINA_FALSE, current, EINA_TRUE, traverse);
         break;
      case ATSPI_Collection_TREE_RESTRICT_SIBLING:
         _collection_query(rule, sortby, &result, 0, max, collection, 0, EINA_FALSE, current, EINA_TRUE, traverse);
         break;
      default:
         ERR("Tree parameter value not handled");
         break;
     }

   return result;
}

static Eldbus_Message*
_collection_get_matches_to(const Eldbus_Service_Interface *iface EINA_UNUSED, const Eldbus_Message *msg EINA_UNUSED)
{
   const char *obj_path = eldbus_message_path_get(msg);
   Eo *bridge = eldbus_service_object_data_get(iface, ELM_ATSPI_BRIDGE_CLASS_NAME);
   Eo *current, *obj = _bridge_object_from_path(bridge, obj_path);
   Eldbus_Message *ret;
   Eldbus_Message_Iter *iter, *rule_iter;
   struct collection_match_rule rule;
   int count;
   AtspiCollectionTreeTraversalType tree;
   Eina_Bool traverse;
   AtspiCollectionSortOrder sortby;
   Eina_List *result = NULL;
   Eina_Bool limit;

   ELM_ATSPI_OBJ_CHECK_OR_RETURN_DBUS_ERROR(obj, EFL_ACCESS_OBJECT_MIXIN, msg);

   iter = eldbus_message_iter_get(msg);
   EINA_SAFETY_ON_NULL_RETURN_VAL(iter, NULL);

   if (!eldbus_message_iter_arguments_get(iter, "o(aiia{ss}iaiiasib)uubib", &obj_path, &rule_iter, &sortby, &tree, &limit, &count, &traverse))
     {
        return eldbus_message_error_new(msg, "org.freedesktop.DBus.Error.Failed", "Unable to get matchule, sortby, tree, limit count or traverse values.");
     }

   current = _bridge_object_from_path(bridge, obj_path);

   ELM_ATSPI_OBJ_CHECK_OR_RETURN_DBUS_ERROR(current, EFL_ACCESS_OBJECT_MIXIN, msg);

   if (!_collection_iter_match_rule_get(rule_iter, &rule))
     return eldbus_message_error_new(msg, "org.freedesktop.DBus.Error.Failed", "Invalid match rule parameters.");

   result = _collection_get_matches_to_handle(obj, current, &rule, sortby, tree, limit, count, traverse);
   ret = _collection_return_msg_from_list(bridge, msg, result);

   eina_list_free(result);
   _collection_match_rule_free(&rule);

   return ret;
}

static Eldbus_Message*
_collection_get_matches(const Eldbus_Service_Interface *iface, const Eldbus_Message *msg)
{
   const char *obj_path = eldbus_message_path_get(msg);
   Eo *bridge = eldbus_service_object_data_get(iface, ELM_ATSPI_BRIDGE_CLASS_NAME);
   Eo *obj = _bridge_object_from_path(bridge, obj_path);
   Eldbus_Message *ret;
   Eldbus_Message_Iter *iter, *rule_iter;
   struct collection_match_rule rule;
   int count;
   Eina_Bool traverse;
   AtspiCollectionSortOrder sortby;
   Eina_List *result = NULL;

   ELM_ATSPI_OBJ_CHECK_OR_RETURN_DBUS_ERROR(obj, EFL_ACCESS_OBJECT_MIXIN, msg);

   iter = eldbus_message_iter_get(msg);
   EINA_SAFETY_ON_NULL_RETURN_VAL(iter, NULL);

   if (!eldbus_message_iter_arguments_get(iter, "(aiia{ss}iaiiasib)uib", &rule_iter, &sortby, &count, &traverse))
     {
        return eldbus_message_error_new(msg, "org.freedesktop.DBus.Error.Failed", "Unable to get matchule, sortby, count or traverse values.");
     }

   if (!_collection_iter_match_rule_get(rule_iter, &rule))
     return eldbus_message_error_new(msg, "org.freedesktop.DBus.Error.Failed", "Invalid match rule parameters.");

   _collection_query(&rule, sortby, &result, 0, count, obj, 0, EINA_TRUE, NULL, EINA_TRUE, traverse);

   ret = _collection_return_msg_from_list(bridge, msg, result);

   eina_list_free(result);
   _collection_match_rule_free(&rule);

   return ret;
}

static const Eldbus_Method collection_methods[] = {
   { "GetMatchesFrom",
      ELDBUS_ARGS({"o", "current_object"}, {"(aiia{ss}iaiiasib)", "match_rule"},
                  {"u", "sortby"}, {"u", "tree"}, {"i", "count"}, {"b", "traverse"}),
      ELDBUS_ARGS({"a(so)", "objects"}), _collection_get_matches_from, 0 },
   { "GetMatchesTo",
      ELDBUS_ARGS({"o", "current_object"}, {"(aiia{ss}iaiiasib)", "match_rule"},
                  {"u", "sortby"}, {"u", "tree"}, {"b", "limit_scope"},
                  {"i", "count"}, {"b", "traverse"}),
      ELDBUS_ARGS({"a(so)", "objects"}), _collection_get_matches_to, 0 },
   { "GetMatches",
      ELDBUS_ARGS({"(aiia{ss}iaiiasib)", "match_rule"},
                  {"u", "sortby"}, {"i", "count"}, {"b", "traverse"}),
      ELDBUS_ARGS({"a(so)", "objects"}), _collection_get_matches, 0 },
   { NULL, NULL, NULL, NULL, 0 }
};

static const Eldbus_Service_Interface_Desc collection_iface_desc = {
   ATSPI_DBUS_INTERFACE_COLLECTION, collection_methods, NULL, NULL, NULL, NULL
};

static void
_object_get_bus_name_and_path(Eo *bridge, const Eo *obj, const char **bus_name, const char **path)
{
   ELM_ATSPI_BRIDGE_DATA_GET_OR_RETURN(bridge, pd);

   // TIZEN_ONLY(20160705) - enable atspi_proxy to work
   if (efl_isa(obj, ELM_ATSPI_PROXY_CLASS))
     {
        const char *pbus = "", *ppath = ATSPI_DBUS_PATH_NULL;
        elm_obj_atspi_proxy_address_get(obj, &pbus, &ppath);
        if (pbus && ppath)
          {
            if (bus_name) *bus_name = pbus;
            if (path) *path = ppath;
             return;
          }
       DBG("Invalid proxy address! Address not set before connecting/listening. Or after proxy is removed.");
     }
   if (bus_name) *bus_name = eldbus_connection_unique_name_get(pd->a11y_bus);
   if (path) *path = _path_from_object(obj);
   //
}

static void
_bridge_iter_object_reference_append(Eo *bridge, Eldbus_Message_Iter *iter, const Eo *obj)
{
   EINA_SAFETY_ON_NULL_RETURN(iter);

   const char *pbus = NULL, *ppath = ATSPI_DBUS_PATH_NULL;
   _object_get_bus_name_and_path(bridge, obj, &pbus, &ppath);
   Eldbus_Message_Iter *iter_struct = eldbus_message_iter_container_new(iter, 'r', NULL);
   eldbus_message_iter_basic_append(iter_struct, 's', pbus);
   eldbus_message_iter_basic_append(iter_struct, 'o', ppath);
   eldbus_message_iter_container_close(iter, iter_struct);
}

static void
_object_desktop_reference_append(Eldbus_Message_Iter *iter)
{
  Eldbus_Message_Iter *iter_struct = eldbus_message_iter_container_new(iter, 'r', NULL);
  EINA_SAFETY_ON_NULL_RETURN(iter);

  eldbus_message_iter_basic_append(iter_struct, 's', ATSPI_DBUS_NAME_REGISTRY);
  eldbus_message_iter_basic_append(iter_struct, 'o', ATSPI_DBUS_PATH_ROOT);
  eldbus_message_iter_container_close(iter, iter_struct);
}

static void
_iter_interfaces_append(Eldbus_Message_Iter *iter, const Eo *obj)
{
  Eldbus_Message_Iter *iter_array;
  iter_array = eldbus_message_iter_container_new(iter, 'a', "s");
  if (!iter_array) return;

  if (efl_isa(obj, EFL_ACCESS_OBJECT_MIXIN))
    {
       eldbus_message_iter_basic_append(iter_array, 's', ATSPI_DBUS_INTERFACE_ACCESSIBLE);
       eldbus_message_iter_basic_append(iter_array, 's', ATSPI_DBUS_INTERFACE_COLLECTION);
    }
  if (efl_isa(obj, EFL_ACCESS_ACTION_MIXIN))
    eldbus_message_iter_basic_append(iter_array, 's', ATSPI_DBUS_INTERFACE_ACTION);
  if (efl_isa(obj, ELM_ATSPI_APP_OBJECT_CLASS))
    eldbus_message_iter_basic_append(iter_array, 's', ATSPI_DBUS_INTERFACE_APPLICATION);
  if (efl_isa(obj, EFL_ACCESS_COMPONENT_MIXIN))
    eldbus_message_iter_basic_append(iter_array, 's', ATSPI_DBUS_INTERFACE_COMPONENT);
  if (efl_isa(obj, EFL_ACCESS_EDITABLE_TEXT_INTERFACE))
    eldbus_message_iter_basic_append(iter_array, 's', ATSPI_DBUS_INTERFACE_EDITABLE_TEXT);
  if (efl_isa(obj, EFL_ACCESS_OBJECT_MIXIN))
    eldbus_message_iter_basic_append(iter_array, 's', ATSPI_DBUS_INTERFACE_IMAGE);
  if (efl_isa(obj, EFL_ACCESS_SELECTION_INTERFACE))
    eldbus_message_iter_basic_append(iter_array, 's', ATSPI_DBUS_INTERFACE_SELECTION);
  if (efl_isa(obj, EFL_ACCESS_TEXT_INTERFACE))
    eldbus_message_iter_basic_append(iter_array, 's', ATSPI_DBUS_INTERFACE_TEXT);
  if (efl_isa(obj, EFL_ACCESS_VALUE_INTERFACE))
    eldbus_message_iter_basic_append(iter_array, 's', ATSPI_DBUS_INTERFACE_VALUE);

  eldbus_message_iter_container_close(iter, iter_array);
}

static Eina_Bool
_cache_item_reference_append_cb(Eo *bridge, Eo *data, Eldbus_Message_Iter *iter_array)
{
  if (!efl_ref_count(data) || efl_destructed_is(data))
    return EINA_TRUE;

   Eldbus_Message_Iter *iter_struct, *iter_sub_array;
   Efl_Access_State_Set states;
   Efl_Access_Role role;
   Eo *root;
   root = efl_access_object_access_root_get(EFL_ACCESS_OBJECT_MIXIN);

   role = efl_access_object_role_get(data);

   iter_struct = eldbus_message_iter_container_new(iter_array, 'r', NULL);
   EINA_SAFETY_ON_NULL_RETURN_VAL(iter_struct, EINA_TRUE);

   /* Marshall object path */
   _bridge_iter_object_reference_append(bridge, iter_struct, data);

   /* Marshall application */
   _bridge_iter_object_reference_append(bridge, iter_struct, root);

   Eo *parent = NULL;
   parent = efl_provider_find(efl_parent_get(data), EFL_ACCESS_OBJECT_MIXIN);
   /* Marshall parent */
   if ((!parent) && (EFL_ACCESS_ROLE_APPLICATION == role))
     _object_desktop_reference_append(iter_struct);
   else
     _bridge_iter_object_reference_append(bridge, iter_struct, parent);

   /* Marshall children  */
   Eina_List *children_list = NULL, *l;
   Eo *child;

   //TIZEN_ONLY(20150709) Do not register children of MANAGES_DESCENDATS objects
   Efl_Access_State_Set ss;
   ss = efl_access_object_state_set_get(data);
   //

   //TIZEN_ONLY(20150709) Do not register children of MANAGES_DESCENDATS objects
   //children_list = efl_access_object_access_children_get(data);
   //

  iter_sub_array = eldbus_message_iter_container_new(iter_struct, 'a', "(so)");
  EINA_SAFETY_ON_NULL_GOTO(iter_sub_array, fail);
   //TIZEN_ONLY(20150709) Do not register children of MANAGES_DESCENDATS objects
   if (!STATE_TYPE_GET(ss, EFL_ACCESS_STATE_MANAGES_DESCENDANTS))
     {
        children_list = efl_access_object_access_children_get(data);
   //
  EINA_LIST_FOREACH(children_list, l, child)
     _bridge_iter_object_reference_append(bridge, iter_sub_array, child);
   //TIZEN_ONLY(20150709) Do not register children of MANAGES_DESCENDATS objects
        eina_list_free(children_list);
     }
   //
  eldbus_message_iter_container_close(iter_struct, iter_sub_array);
   //TIZEN_ONLY(20150709) Do not register children of MANAGES_DESCENDATS objects
   //eina_list_free(children_list);
   //

  /* Marshall interfaces */
  _iter_interfaces_append(iter_struct, data);

  /* Marshall name */
  const char *name = NULL;
  name = efl_access_object_i18n_name_get(data);
  if (!name)
    name = "";

  eldbus_message_iter_basic_append(iter_struct, 's', name);

  /* Marshall role */
  eldbus_message_iter_basic_append(iter_struct, 'u', role);

  /* Marshall description */
  const char* description = NULL;
  description = efl_access_object_description_get(data);
  if (!description)
    description = "";
  eldbus_message_iter_basic_append(iter_struct, 's', description);

  /* Marshall state set */
  iter_sub_array = eldbus_message_iter_container_new(iter_struct, 'a', "u");
  EINA_SAFETY_ON_NULL_GOTO(iter_sub_array, fail);

  states = efl_access_object_state_set_get(data);

  unsigned int s1 = states & 0xFFFFFFFF;
  unsigned int s2 = (states >> 32) & 0xFFFFFFFF;
  eldbus_message_iter_basic_append(iter_sub_array, 'u', s1);
  eldbus_message_iter_basic_append(iter_sub_array, 'u', s2);

  eldbus_message_iter_container_close(iter_struct, iter_sub_array);
  eldbus_message_iter_container_close(iter_array, iter_struct);

  return EINA_TRUE;

fail:
  if (iter_struct) eldbus_message_iter_del(iter_struct);
  return EINA_TRUE;
}

static Eldbus_Message *
_cache_get_items(const Eldbus_Service_Interface *iface, const Eldbus_Message *msg)
{
   Eldbus_Message_Iter *iter, *iter_array;
   Eldbus_Message *ret;
   Eina_List *to_process;
   Eo *root;

   Eo *bridge = eldbus_service_object_data_get(iface, ELM_ATSPI_BRIDGE_CLASS_NAME);
   if (!bridge) return NULL;

   ELM_ATSPI_BRIDGE_DATA_GET_OR_RETURN_VAL(bridge, pd, NULL);

   ret = eldbus_message_method_return_new(msg);
   EINA_SAFETY_ON_NULL_RETURN_VAL(ret, NULL);

   iter = eldbus_message_iter_get(ret);
   iter_array = eldbus_message_iter_container_new(iter, 'a', CACHE_ITEM_SIGNATURE);
   EINA_SAFETY_ON_NULL_GOTO(iter_array, fail);

   root = efl_access_object_access_root_get(EFL_ACCESS_OBJECT_MIXIN);
   to_process = eina_list_append(NULL, root);

   while (to_process)
     {
        Eo *obj = eina_list_data_get(to_process);
        to_process = eina_list_remove_list(to_process, to_process);
        _cache_item_reference_append_cb(bridge, obj, iter_array);
        _bridge_object_register(bridge, obj);

        Eina_List *children;
        children = efl_access_object_access_children_get(obj);
        to_process = eina_list_merge(to_process, children);
     }

   eldbus_message_iter_container_close(iter, iter_array);

   return ret;
fail:
   if (ret) eldbus_message_unref(ret);
   return NULL;
}

static const Eldbus_Method cache_methods[] = {
   { "GetItems", NULL, ELDBUS_ARGS({CACHE_ITEM_SIGNATURE, "items"}), _cache_get_items, 0 },
   { NULL, NULL, NULL, NULL, 0 }
};

static const Eldbus_Signal cache_signals[] = {
  [ATSPI_OBJECT_CHILD_ADDED] = { "AddAccessible", ELDBUS_ARGS({"((so)(so)a(so)assusau)", "added"}), 0},
  [ATSPI_OBJECT_CHILD_REMOVED] = { "RemoveAccessible", ELDBUS_ARGS({ "(so)", "removed" }), 0},
  {NULL, NULL, 0}
};

static const Eldbus_Service_Interface_Desc cache_iface_desc = {
   ATSPI_DBUS_INTERFACE_CACHE, cache_methods, cache_signals, NULL, NULL, NULL
};

// Component interface
static Eldbus_Message *
_component_contains(const Eldbus_Service_Interface *iface EINA_UNUSED, const Eldbus_Message *msg)
{
   const char *obj_path = eldbus_message_path_get(msg);
   Eo *bridge = eldbus_service_object_data_get(iface, ELM_ATSPI_BRIDGE_CLASS_NAME);
   Eo *obj = _bridge_object_from_path(bridge, obj_path);
   int x, y;
   Eina_Bool contains = EINA_FALSE;
   AtspiCoordType coord_type;
   Eldbus_Message *ret;

   ELM_ATSPI_OBJ_CHECK_OR_RETURN_DBUS_ERROR(obj, EFL_ACCESS_COMPONENT_MIXIN, msg);

   if (!eldbus_message_arguments_get(msg, "iiu", &x, &y, &coord_type))
     return eldbus_message_error_new(msg, "org.freedesktop.DBus.Error.InvalidArgs", "Invalid index type.");

   Eina_Bool type = coord_type == ATSPI_COORD_TYPE_SCREEN ? EINA_TRUE : EINA_FALSE;
   contains = efl_access_component_contains(obj, type, x, y);

   ret = eldbus_message_method_return_new(msg);
   EINA_SAFETY_ON_NULL_RETURN_VAL(ret, NULL);

   eldbus_message_arguments_append(ret, "b", contains);

   return ret;
}

static Eldbus_Message *
_component_get_accessible_at_point(const Eldbus_Service_Interface *iface EINA_UNUSED, const Eldbus_Message *msg)
{
   const char *obj_path = eldbus_message_path_get(msg);
   Eo *bridge = eldbus_service_object_data_get(iface, ELM_ATSPI_BRIDGE_CLASS_NAME);
   // TIZEN_ONLY(20160705) - enable atspi_proxy to work
   ELM_ATSPI_BRIDGE_DATA_GET_OR_RETURN_VAL(bridge, pd, NULL);
   //
   Eo *obj = _bridge_object_from_path(bridge, obj_path);
   int x, y;
   Eo *accessible = NULL;
   AtspiCoordType coord_type;
   Eldbus_Message *ret;
   Eldbus_Message_Iter *iter;

   ELM_ATSPI_OBJ_CHECK_OR_RETURN_DBUS_ERROR(obj, EFL_ACCESS_COMPONENT_MIXIN, msg);

   // TIZEN_ONLY(20161213) - do not response if ecore evas is obscured
   if (_ee_obscured_get(obj))
     return eldbus_message_error_new(msg, "org.freedesktop.DBus.Error.Failed", "ecore evas is obscured.");
   //

   if (!eldbus_message_arguments_get(msg, "iiu", &x, &y, &coord_type))
     return eldbus_message_error_new(msg, "org.freedesktop.DBus.Error.InvalidArgs", "Invalid index type.");


   // TIZEN_ONLY(20160705) - enable atspi_proxy to work
   Evas_Object *top = elm_object_top_widget_get(obj);
   int sx = 0;
   int sy = 0;

   efl_access_component_socket_offset_get(top, &sx, &sy);

   x = x - sx;
   y = y - sy;
   //


   ret = eldbus_message_method_return_new(msg);
   EINA_SAFETY_ON_NULL_RETURN_VAL(ret, NULL);

   iter = eldbus_message_iter_get(ret);
   Eina_Bool type = coord_type == ATSPI_COORD_TYPE_SCREEN ? EINA_TRUE : EINA_FALSE;
   accessible = efl_access_component_accessible_at_point_get(obj, type, x, y);
   _bridge_iter_object_reference_append(bridge, iter, accessible);
   _bridge_object_register(bridge, accessible);

   return ret;
}

// TIZEN_ONLY(20170310) - implementation of get object under coordinates for accessibility
typedef enum {
  NEIGHBOR_SEARCH_MODE_NORMAL = 0,
  NEIGHBOR_SEARCH_MODE_RECURSE_FROM_ROOT = 1,
  NEIGHBOR_SEARCH_MODE_CONTINUE_AFTER_FAILED_RECURSING = 2,
  NEIGHBOR_SEARCH_MODE_RECURSE_TO_OUTSIDE = 3,
} GetNeighborSearchMode;

typedef struct accessibility_navigation_pointer_table {
   AtspiRole (*object_get_role)(struct accessibility_navigation_pointer_table *t, void *ptr);
   uint64_t (*object_get_state_set)(struct accessibility_navigation_pointer_table *t, void *ptr);
   void *(*get_object_in_relation_by_type)(struct accessibility_navigation_pointer_table *t, void *ptr, AtspiRelationType type);
   unsigned char (*object_is_zero_size)(struct accessibility_navigation_pointer_table *t, void *ptr);
   void *(*get_parent)(struct accessibility_navigation_pointer_table *t, void *ptr);
   unsigned char (*object_is_scrollable)(struct accessibility_navigation_pointer_table *t, void *ptr);
   void *(*get_object_at_point)(struct accessibility_navigation_pointer_table *t, void *ptr, int x, int y, unsigned char coordinates_are_screen_based);
   unsigned char (*object_contains)(struct accessibility_navigation_pointer_table *t, void *ptr, int x, int y, unsigned char coordinates_are_screen_based);
   unsigned char (*object_is_proxy)(struct accessibility_navigation_pointer_table *t, void *ptr);
} accessibility_navigation_pointer_table;

#define CALL(fncname, ...) table->fncname(table, __VA_ARGS__)
static unsigned char _accept_object_check_role(accessibility_navigation_pointer_table *table EINA_UNUSED, void *obj)
{
   return _elm_widget_atspi_role_acceptable_check(obj);
}

static unsigned char _state_set_is_set(uint64_t state_set, AtspiStateType state)
{
   return (state_set & ((uint64_t)1 << (unsigned int)state)) != 0;
}

static unsigned char _object_is_defunct(accessibility_navigation_pointer_table *table, void *ptr)
{
   uint64_t states = CALL(object_get_state_set, ptr);
   return _state_set_is_set(states, ATSPI_STATE_DEFUNCT);
}

static unsigned char _object_role_is_acceptable_when_navigating_next_prev(accessibility_navigation_pointer_table *table, void *obj)
{
   AtspiRole role = CALL(object_get_role, obj);
   return role != ATSPI_ROLE_POPUP_MENU && role != ATSPI_ROLE_DIALOG;
}

static void *_get_object_in_relation_flow(accessibility_navigation_pointer_table *table, void *source, unsigned char forward)
{
    return CALL(get_object_in_relation_by_type, source, forward ? ATSPI_RELATION_FLOWS_TO : ATSPI_RELATION_FLOWS_FROM);
}

static unsigned char _object_is_item(accessibility_navigation_pointer_table *table, void *obj)
{
   AtspiRole role = CALL(object_get_role, obj);
   return role == ATSPI_ROLE_LIST_ITEM || role == ATSPI_ROLE_MENU_ITEM;
}

static unsigned char _object_is_highlightable(accessibility_navigation_pointer_table *table, void *obj)
{
   uint64_t state_set = CALL(object_get_state_set, obj);
   return _state_set_is_set(state_set, ATSPI_STATE_HIGHLIGHTABLE);
}

static unsigned char _object_is_visible(accessibility_navigation_pointer_table *table, void *obj)
{
   uint64_t state_set = CALL(object_get_state_set, obj);
   return _state_set_is_set(state_set, ATSPI_STATE_VISIBLE);
}

static unsigned char _object_is_showing(accessibility_navigation_pointer_table *table, void *obj)
{
   uint64_t state_set = CALL(object_get_state_set, obj);
   return _state_set_is_set(state_set, ATSPI_STATE_SHOWING);
}

static unsigned char _object_is_collapsed(accessibility_navigation_pointer_table *table, void *obj)
{
   uint64_t state_set = CALL(object_get_state_set, obj);
   return
      _state_set_is_set(state_set, ATSPI_STATE_EXPANDABLE) &&
      !_state_set_is_set(state_set, ATSPI_STATE_EXPANDED);
}

static unsigned char _object_has_modal_state(accessibility_navigation_pointer_table *table, void *obj)
{
   uint64_t state_set = CALL(object_get_state_set, obj);
   return _state_set_is_set(state_set, ATSPI_STATE_MODAL);
}

static unsigned char _object_is_zero_size(accessibility_navigation_pointer_table *table, void *obj)
{
   return CALL(object_is_zero_size, obj);
}

static void *_get_scrollable_parent(accessibility_navigation_pointer_table *table, void *obj)
{
   while(obj)
     {
       obj = CALL(get_parent, obj);
       if (obj && CALL(object_is_scrollable, obj)) return obj;
     }
   return NULL;
}
static unsigned char _accept_object(accessibility_navigation_pointer_table *table, void *obj)
{
   if (!obj) return 0;
   if (!_object_is_visible(table, obj)) return 0;
   if (!_accept_object_check_role(table, obj)) return 0;
   if (CALL(get_object_in_relation_by_type, obj, ATSPI_RELATION_CONTROLLED_BY) != NULL) return 0;
   if (!_object_is_highlightable(table, obj)) return 0;

   if (_get_scrollable_parent(table, obj) != NULL)
     {
       void *parent = CALL(get_parent, obj);

       if (parent)
         {
           return !_object_is_item(table, obj) || !_object_is_collapsed(table, parent);
         }
     }
   else
     {
       if (_object_is_zero_size(table, obj)) return 0;
       if (!_object_is_showing(table, obj)) return 0;
     }
   return 1;
}

/* The target cannot be a parent of root */
static Eina_Bool _target_validation_check(Eo *target, Eo *root)
{
   Eo *parent;
   parent = efl_provider_find(efl_parent_get(root), EFL_ACCESS_OBJECT_MIXIN);

   while (parent)
     {
        if (parent == target) return EINA_FALSE;
        parent = efl_provider_find(efl_parent_get(parent), EFL_ACCESS_OBJECT_MIXIN);
     }

   return EINA_TRUE;
}

static void *_calculate_navigable_accessible_at_point_impl(accessibility_navigation_pointer_table *table,
          void *root, int x, int y, unsigned char coordinates_are_screen_based)
{
   if (!root) return NULL;
   void *prev_root = root;

   void *return_value = NULL;
   while (1)
     {
       void *target = CALL(get_object_at_point, root, x, y, coordinates_are_screen_based);
       if (!target) break;
       if (target == root || target == prev_root) break;
       if (!_target_validation_check(target, root)) break;

       // always return proxy, so atspi lib can call on it again
       if (CALL(object_is_proxy, target)) return target;

       root = target;
       void *relation_obj = CALL(get_object_in_relation_by_type, root, ATSPI_RELATION_CONTROLLED_BY);
       unsigned char contains = 0;
       if (relation_obj)
         {
           contains = CALL(object_contains, relation_obj, x, y, coordinates_are_screen_based);
           if (contains) root = relation_obj;
         }

       if (_accept_object(table, root))
         {
           return_value = root;
           if (contains) break;
         }
     }

   if (return_value && _object_has_modal_state(table, return_value)) return_value = NULL;
   return return_value;
}

static void *_find_non_defunct_child(accessibility_navigation_pointer_table *table,
            Eina_List *children, unsigned int current_index, unsigned char forward)
{
   unsigned int children_count = eina_list_count(children);
   for(; current_index < children_count; forward ? ++current_index : --current_index)
     {
       void *n = eina_list_nth(children, current_index);
       if (n && !_object_is_defunct(table, n)) return n;
     }
   return NULL;
}

static void *_directional_depth_first_search_try_non_defunct_child(accessibility_navigation_pointer_table *table,
            void *node, Eina_List *children, unsigned char forward)
{
   unsigned int children_count = eina_list_count(children);
   if (children_count > 0)
     {
       unsigned char is_showing = _get_scrollable_parent(table, node) == NULL ? _object_is_showing(table, node) : 1;
       if (is_showing)
         {
           return _find_non_defunct_child(table, children, forward ? 0 : children_count - 1, forward);
         }
     }
   return NULL;
}

static Eina_List *_scrollable_parent_list_get(Eo *obj)
{
   Eina_List *ret = NULL;
   Eo *parent;

   if (obj)
     {
        parent = efl_provider_find(efl_parent_get(obj), EFL_ACCESS_OBJECT_MIXIN);
        while (parent)
          {
             if (efl_isa(parent, ELM_INTERFACE_SCROLLABLE_MIXIN))
               {
                  ret = eina_list_append(ret, parent);
               }
             parent = efl_provider_find(efl_parent_get(parent), EFL_ACCESS_OBJECT_MIXIN);
          }
     }

   return ret;
}

static void _viewport_geometry_get(Eo *obj, int *x, int *y, int *w, int *h)
{
   elm_interface_scrollable_content_viewport_geometry_get(obj, x, y, w, h);
   /* widget implements scrollable interface but does not use scoller
      in this case, use widget geometry */
   if (*w == 0 || *h == 0)
     {
        INF("%s is zero sized content viewport", efl_class_name_get(efl_class_get(obj)));
        Eina_Rect r = efl_access_component_extents_get(obj, EINA_FALSE);
        *x = r.x;
        *y = r.y;
        *w = r.w;
        *h = r.h;
     }
}

static Eina_Bool
_new_scrollable_parent_viewport_geometry_get(Eo *node, Eo *start,
                                             int *x, int *y, int *w, int *h)
{
   Eina_Bool ret = EINA_FALSE;
   Eina_List *n_spl;
   Eina_List *s_spl;

   n_spl = _scrollable_parent_list_get(node);
   s_spl = _scrollable_parent_list_get(start);

   Eo *sp;
   Eina_List *l;
   EINA_LIST_FOREACH(s_spl, l, sp)
     {
        n_spl = eina_list_remove(n_spl, sp);
     }

   Evas_Coord sx = 0, sy = 0, sw = 0, sh = 0;

   unsigned int count = eina_list_count(n_spl);
   if (count > 0)
     {
        sp = eina_list_nth(n_spl, count - 1);
        _viewport_geometry_get(sp, &sx, &sy, &sw, &sh);
        ret = EINA_TRUE;
     }

   *x = sx;
   *y = sy;
   *w = sw;
   *h = sh;

   return ret;
}

static Eina_List *_valid_children_get(Eina_List *children, Eo *start, Eo *root)
{
   /* condition to find first(last) object regardless of scrollable parent.
      looping navigation does not care scrollable parent.
      1. currently highlighted object exists
      2. both start and root are same */
   Eo *current = _elm_object_accessibility_currently_highlighted_get();
   if (current && start == root) return children;

   Eo *child = NULL;
   child = eina_list_nth(children, 0);

   if (child)
     {
        Evas_Coord x = 0, y = 0, w = 0, h = 0;
        Evas_Coord sx = 0, sy = 0, sw = 0, sh = 0;

        if (_new_scrollable_parent_viewport_geometry_get(child, start,
                                                   &sx, &sy, &sw, &sh))
          {
             Eina_List *l, *l_next;
             EINA_LIST_FOREACH_SAFE(children, l, l_next, child)
               {
                  Eina_Rect r = efl_access_component_extents_get(child, EINA_FALSE);
                  x = r.x;
                  y = r.y;
                  w = r.w;
                  h = r.h;
                  if (w == 0 || h == 0 ||
                      !ELM_RECTS_INTERSECT(x, y, w, h, sx, sy, sw, sh))
                     children = eina_list_remove_list(children, l);
               }
          }
     }
   return children;
}

static void *_get_next_non_defunct_sibling(accessibility_navigation_pointer_table *table,
            void *obj, void *start, void *root, unsigned char forward)
{
   if (!obj) return NULL;
   void *parent = CALL(get_parent, obj);
   if (!parent) return NULL;

   Eina_List *children;
   children = efl_access_object_access_children_get(parent);
   children = _valid_children_get(children, start, root);

   unsigned int children_count = eina_list_count(children);
   if (children_count == 0)
     {
        eina_list_free(children);
        return NULL;
     }
   unsigned int current = 0;
   for(; current < children_count && eina_list_nth(children, current) != obj; ++current) ;
   if (current >= children_count)
     {
        eina_list_free(children);
        return NULL;
     }
   forward ? ++current : --current;
   void *ret = _find_non_defunct_child(table, children, current, forward);
   eina_list_free(children);
   return ret;
}

static void *
_directional_depth_first_search_try_non_defunct_sibling(accessibility_navigation_pointer_table *table,
                                                              unsigned char *all_children_visited_ptr,
                                                                  void *node, void *start, void *root,
                                                                                unsigned char forward)
{
   while(1)
     {
       void *sibling = _get_next_non_defunct_sibling(table, node, start, root, forward);
       if (sibling != NULL)
         {
           node = sibling;
           *all_children_visited_ptr = 0;
           break;
         }

       // walk up...
       node = CALL(get_parent, node);
       if (node == NULL || node == root) return NULL;

       // in backward traversing stop the walk up on parent
       if (!forward) break;
     }
   return node;
}

typedef struct {
    const void *key;
    unsigned int current_search_size;
    unsigned int counter;
} cycle_detection_data;

void cycle_detection_initialize(cycle_detection_data *data, const void *key)
{
   if (!data) return;
   data->key = key;
   data->current_search_size = 1;
   data->counter = 1;
}

unsigned char cycle_detection_check_if_in_cycle(cycle_detection_data *data, const void *key)
{
   if (!data) return 1;
   if (data->key == key) return 1;
   if (--data->counter == 0)
     {
       data->current_search_size <<= 1;
       if (data->current_search_size == 0) return 1;
       data->counter = data->current_search_size;
       data->key = key;
     }
   return 0;
}

static Eina_Bool
_deputy_is(Eo *obj)
{
   if (efl_isa(obj, ELM_ACCESS_CLASS))
     {
        Elm_Access_Info *info;

        info = _elm_access_info_get(obj);
        if (info && efl_isa(info->part_object, EFL_UI_LAYOUT_OBJECT_CLASS))
          {
             Eina_List *attrs, *l;
             Efl_Access_Attribute *attr;

             attrs = efl_access_object_attributes_get(info->part_object);
             EINA_LIST_FOREACH(attrs, l, attr)
               {
                  if (!strcmp(attr->key, "___PlugID"))
                    {
                       efl_access_attributes_list_free(attrs);
                       return EINA_TRUE;
                    }
               }
             efl_access_attributes_list_free(attrs);
          }
     }
   return EINA_FALSE;
}

static Eo *
_proxy_in_parent_get(Eo *obj)
{
   Eina_List *l;
   Eo *proxy = NULL;
   Eina_List *children_list = NULL;
   children_list = efl_access_object_access_children_get(obj);

   Evas_Object *child;
   EINA_LIST_FOREACH(children_list, l, child)
     {
        if (efl_isa(child, ELM_ATSPI_PROXY_CLASS))
          {
             proxy = child;
             break;
          }
     }
   eina_list_free(children_list);

   return proxy;
}

static Eo *
_deputy_of_proxy_in_parent_get(Eo *obj)
{
   Eina_List *l;
   Eo *deputy = NULL;
   Eina_List *children_list = NULL;
   children_list = efl_access_object_access_children_get(obj);

   unsigned int index = 0;
   Evas_Object *child;
   EINA_LIST_FOREACH(children_list, l, child)
     {
        if (efl_isa(child, ELM_ATSPI_PROXY_CLASS))
          {
             if (index == 0)
               {
                  WRN("Proxy does not have deputy object");
                  break;
               }

             deputy = eina_list_nth(children_list, index - 1);
             break;
          }
        index++;
     }
   eina_list_free(children_list);

   return deputy;
}

static void *_calculate_neighbor_impl(accessibility_navigation_pointer_table *table, void *root, void *start, unsigned char forward, GetNeighborSearchMode search_mode)
{
   if (root && _object_is_defunct(table, root)) return NULL;
   if (start && _object_is_defunct(table, start))
     {
       start = NULL;
       forward = 1;
     }

   if (search_mode == NEIGHBOR_SEARCH_MODE_RECURSE_TO_OUTSIDE)
     {
        /* This only works if we navigate backward, and it is not possible to
           find in embedded process. In this case the deputy should be used */
        return _deputy_of_proxy_in_parent_get(start);
     }

   void *node = start ? start : root;
   if (!node) return NULL;

   // initialization of all-children-visited flag for start node - we assume
   // that when we begin at start node and we navigate backward, then all children
   // are visited, so navigation will ignore start's children and go to
   // previous sibling available.
   unsigned char all_children_visited = (search_mode != NEIGHBOR_SEARCH_MODE_RECURSE_FROM_ROOT && !forward && start != NULL && start != root);

   // true, if starting element should be ignored. this is only used in rare case of
   // recursive search failing to find an object.
   // consider tree, where element A on bus BUS_A has child B on bus BUS_B. when going "next" from
   // element A algorithm has to descend into BUS_B and search element B and its children. this is done
   // by returning to our caller object B with special flag set (meaning - continue the search from B on bus BUS_B).
   // if next object will be found there (on BUS_B), then search ends. but if not, then our caller will find it out
   // and will call us again with object A and flag search_mode set to NEIGHBOR_SEARCH_MODE_CONTINUE_AFTER_FAILED_RECURSING.
   // this flag means, that object A was already checked previously and we should skip it and its children.
   unsigned char force_next = (search_mode == NEIGHBOR_SEARCH_MODE_CONTINUE_AFTER_FAILED_RECURSING);

   cycle_detection_data cycle_detection;
   cycle_detection_initialize(&cycle_detection, node);
   while (node)
     {
       if (_object_is_defunct(table, node)) return NULL;

       // always accept proxy object from different world
       if (!force_next && CALL(object_is_proxy, node)) return node;

       Eina_List *children;
       children = efl_access_object_access_children_get(node);
       children = _valid_children_get(children, start, root);

       // do accept:
       // 1. not start node
       // 2. parent after all children in backward traversing
       // 3. Nodes with roles: ATSPI_ROLE_PAGE_TAB, ATSPI_ROLE_POPUP_MENU and ATSPI_ROLE_DIALOG, only when looking for first or last element.
       //    Objects with those roles shouldnt be reachable, when navigating next / prev.
       unsigned char all_children_visited_or_moving_forward = (eina_list_count(children) == 0 || forward || all_children_visited);
       if (!force_next && node != start && all_children_visited_or_moving_forward && _accept_object(table, node))
         {
           if (start == NULL || _object_role_is_acceptable_when_navigating_next_prev(table, node))
             {
                eina_list_free(children);
                return node;
             }
         }

       //TIZEN_ONLY(20181022): Fix for navigation problems related with relations
       void *next_related_in_direction = NULL;
       if (all_children_visited || forward)
         next_related_in_direction = !force_next ? _get_object_in_relation_flow(table, node, forward) : NULL;
       //

       /* force_next means that the search_mode is NEIGHBOR_SEARCH_MODE_CONTINUE_AFTER_FAILED_RECURSING
          in this case the node is elm_layout which is parent of proxy object.
          There is an access object working for the proxy object, and the access
          object could have relation information. This relation information should
          be checked first before using the elm_layout as a node. */
       if (force_next && forward)
         {
            Eo *deputy;
            deputy = _deputy_of_proxy_in_parent_get(node);
            next_related_in_direction =
              _get_object_in_relation_flow(table, deputy, forward);
         }

       if (next_related_in_direction && _object_is_defunct(table, next_related_in_direction))
           next_related_in_direction = NULL;
       unsigned char want_cycle_detection = 0;
       if (next_related_in_direction)
         {
           /* Check next_related_in_direction is deputy object */
           Eo *parent;
           if (!forward)
             {
                /* If the prev object is deputy, then go to inside of its proxy first */
                if (_deputy_is(next_related_in_direction))
                  {
                     parent = efl_ui_widget_parent_get(next_related_in_direction);
                     next_related_in_direction =
                       _proxy_in_parent_get(parent);
                  }
             }
           else
             {
                /* If current object is deputy, and it has relation next object,
                   then do not use the relation next object, and use proxy first */
                if (_deputy_is(node))
                  {
                     parent = efl_ui_widget_parent_get(node);
                     next_related_in_direction =
                       _proxy_in_parent_get(parent);
                  }
             }

           node = next_related_in_direction;
           want_cycle_detection = 1;
         }
       else {
           void *child = !force_next && !all_children_visited ?
                          _directional_depth_first_search_try_non_defunct_child(table, node, children, forward) : NULL;
           if (child != NULL) want_cycle_detection = 1;
           else
             {
               if (!force_next && node == root)
                 {
                    eina_list_free(children);
                    return NULL;
                 }
               all_children_visited = 1;
               child = _directional_depth_first_search_try_non_defunct_sibling(table, &all_children_visited, node, start, root, forward);
             }
           node = child;
       }

       force_next = 0;
       if (want_cycle_detection && cycle_detection_check_if_in_cycle(&cycle_detection, node))
         {
            eina_list_free(children);
            return NULL;
         }
       eina_list_free(children);
     }
   return NULL;
}

typedef struct accessibility_navigation_pointer_table_impl {
  accessibility_navigation_pointer_table ptrs;
  Eo *bridge;
} accessibility_navigation_pointer_table_impl;

static AtspiRole _object_get_role_impl(struct accessibility_navigation_pointer_table *table EINA_UNUSED, void *ptr)
{
   Efl_Access_Role role;
   Eo *obj = (Eo*)ptr;
   role = efl_access_object_role_get(obj);
   return _efl_role_to_atspi_role(role);
}

static uint64_t _object_get_state_set_impl(struct accessibility_navigation_pointer_table *table EINA_UNUSED, void *ptr)
{
   Efl_Access_State_Set states;
   Eo *obj = (Eo*)ptr;
   states = efl_access_object_state_set_get(obj);
   return _elm_atspi_state_set_to_atspi_state_set(states);
}

static void *_get_object_in_relation_by_type_impl(struct accessibility_navigation_pointer_table *table EINA_UNUSED, void *ptr, AtspiRelationType type)
{
   if (ptr)
     {
       const Eo *source = ptr;
       Efl_Access_Relation_Set relations;
       Efl_Access_Relation_Type expected_relation_type = _atspi_relation_to_elm_relation(type);
       relations = efl_access_object_relation_set_get(source);
       Efl_Access_Relation *rel;
       Eina_List *l;
       EINA_LIST_FOREACH(relations, l, rel)
         {
           if (rel->type == expected_relation_type)
             {
                Eina_List *last = eina_list_last(rel->objects);
                return eina_list_data_get(last);
             }
         }
     }
   return NULL;
}

static unsigned char _object_is_zero_size_impl(struct accessibility_navigation_pointer_table *table EINA_UNUSED, void *ptr)
{
   Eo *obj = (Eo*)ptr;
   Eina_Rect r = efl_access_component_extents_get(obj, EINA_TRUE);
   return r.w == 0 || r.h == 0;
}

unsigned char _object_is_scrollable_impl(struct accessibility_navigation_pointer_table *table EINA_UNUSED, void *ptr)
{
   Eo *obj = (Eo*)ptr;
   return efl_isa(obj, ELM_INTERFACE_SCROLLABLE_MIXIN);
}

void *_get_parent_impl(struct accessibility_navigation_pointer_table *table EINA_UNUSED, void *ptr)
{
   Eo *obj = (Eo*)ptr, *ret_obj;
   ret_obj = efl_provider_find(efl_parent_get(obj), EFL_ACCESS_OBJECT_MIXIN);
   return ret_obj;
}

void *_get_object_at_point_impl(struct accessibility_navigation_pointer_table *table EINA_UNUSED, void *ptr, int x, int y, unsigned char coordinates_are_screen_based)
{
   Eo *obj = (Eo*)ptr, *target;
   target = efl_access_component_accessible_at_point_get(obj, coordinates_are_screen_based, x, y);
   return target;
}

unsigned char _object_contains_impl(struct accessibility_navigation_pointer_table *table EINA_UNUSED, void *ptr, int x, int y, unsigned char coordinates_are_screen_based)
{
   Eo *obj = (Eo*)ptr;
   Eina_Bool return_value;
   return_value = efl_access_component_contains(obj, coordinates_are_screen_based, x, y);
   return return_value ? 1 : 0;
}

unsigned char _object_is_proxy_impl(struct accessibility_navigation_pointer_table *table_, void *obj)
{
   accessibility_navigation_pointer_table_impl *table = (accessibility_navigation_pointer_table_impl*)table_;
   ELM_ATSPI_BRIDGE_DATA_GET_OR_RETURN_VAL(table->bridge, pd, 0);
   const char *our_bus_name = eldbus_connection_unique_name_get(pd->a11y_bus);
   const char *obj_bus_name;
   _object_get_bus_name_and_path(table->bridge, (Eo*)obj, &obj_bus_name, NULL);
   return our_bus_name && obj_bus_name && strcmp(our_bus_name, obj_bus_name) != 0;
}

accessibility_navigation_pointer_table_impl construct_accessibility_navigation_pointer_table(Eo *bridge)
{
   accessibility_navigation_pointer_table_impl table;
#define INIT(n) table.ptrs.n = _## n ## _impl
   INIT(object_get_role);
   INIT(object_get_state_set);
   INIT(get_object_in_relation_by_type);
   INIT(object_is_zero_size);
   INIT(get_parent);
   INIT(object_is_scrollable);
   INIT(get_object_at_point);
   INIT(object_contains);
   INIT(object_is_proxy);
#undef INIT
   table.bridge = bridge;
   return table;
}
static Eo *_calculate_navigable_accessible_at_point(Eo *bridge, Eo *root, Eina_Bool coord_type, int x, int y)
{
   accessibility_navigation_pointer_table_impl table = construct_accessibility_navigation_pointer_table(bridge);
   Eo *result = (Eo*)_calculate_navigable_accessible_at_point_impl(&table.ptrs, root, x, y, coord_type ? 1 : 0);

   return result;
}

static Eo *_calculate_neighbor(Eo *bridge, Eo *root, Eo *start, Eina_Bool forward, int search_mode)
{
   accessibility_navigation_pointer_table_impl table = construct_accessibility_navigation_pointer_table(bridge);
   Eo *result = (Eo*)_calculate_neighbor_impl(&table.ptrs, root, start, forward ? 1 : 0, (GetNeighborSearchMode)search_mode);
   return result;
}
//

static Eldbus_Message *
_component_get_extents(const Eldbus_Service_Interface *iface EINA_UNUSED, const Eldbus_Message *msg)
{
   const char *obj_path = eldbus_message_path_get(msg);
   Eo *bridge = eldbus_service_object_data_get(iface, ELM_ATSPI_BRIDGE_CLASS_NAME);
   Eo *obj = _bridge_object_from_path(bridge, obj_path);
   AtspiCoordType coord_type;
   Eldbus_Message *ret;
   Eldbus_Message_Iter *iter, *iter_struct;
   Eina_Rect r;

   ELM_ATSPI_OBJ_CHECK_OR_RETURN_DBUS_ERROR(obj, EFL_ACCESS_COMPONENT_MIXIN, msg);

   if (!eldbus_message_arguments_get(msg, "u", &coord_type))
     return eldbus_message_error_new(msg, "org.freedesktop.DBus.Error.InvalidArgs", "Invalid index type.");

   ret = eldbus_message_method_return_new(msg);
   EINA_SAFETY_ON_NULL_RETURN_VAL(ret, NULL);

   iter = eldbus_message_iter_get(ret);

   r = efl_access_component_extents_get(obj, (coord_type == ATSPI_COORD_TYPE_SCREEN));
   iter_struct = eldbus_message_iter_container_new(iter, 'r', NULL);
   EINA_SAFETY_ON_NULL_GOTO(iter_struct, fail);

   eldbus_message_iter_basic_append(iter_struct, 'i', r.x);
   eldbus_message_iter_basic_append(iter_struct, 'i', r.y);
   eldbus_message_iter_basic_append(iter_struct, 'i', r.w);
   eldbus_message_iter_basic_append(iter_struct, 'i', r.h);

   eldbus_message_iter_container_close(iter, iter_struct);

   return ret;
fail:
   if (ret) eldbus_message_unref(ret);
   return NULL;
}

static Eldbus_Message *
_component_get_position(const Eldbus_Service_Interface *iface EINA_UNUSED, const Eldbus_Message *msg)
{
   const char *obj_path = eldbus_message_path_get(msg);
   Eo *bridge = eldbus_service_object_data_get(iface, ELM_ATSPI_BRIDGE_CLASS_NAME);
   Eo *obj = _bridge_object_from_path(bridge, obj_path);
   int x = 1, y = 1;
   AtspiCoordType coord_type;
   Eldbus_Message *ret;

   ELM_ATSPI_OBJ_CHECK_OR_RETURN_DBUS_ERROR(obj, EFL_ACCESS_COMPONENT_MIXIN, msg);

   if (!eldbus_message_arguments_get(msg, "u", &coord_type))
     return eldbus_message_error_new(msg, "org.freedesktop.DBus.Error.InvalidArgs", "Invalid index type.");

   Eina_Bool type = coord_type == ATSPI_COORD_TYPE_SCREEN ? EINA_TRUE : EINA_FALSE;
   if (efl_gfx_entity_visible_get(obj))
     {
        if (type)
          efl_access_component_screen_position_get(obj, &x, &y);
        else
          evas_object_geometry_get(obj, &x, &y, NULL, NULL);
     }

   ret = eldbus_message_method_return_new(msg);
   EINA_SAFETY_ON_NULL_RETURN_VAL(ret, NULL);

   eldbus_message_arguments_append(ret, "i", x);
   eldbus_message_arguments_append(ret, "i", y);

   return ret;
}

static Eldbus_Message *
_component_get_size(const Eldbus_Service_Interface *iface EINA_UNUSED, const Eldbus_Message *msg)
{
   const char *obj_path = eldbus_message_path_get(msg);
   Eo *bridge = eldbus_service_object_data_get(iface, ELM_ATSPI_BRIDGE_CLASS_NAME);
   Eo *obj = _bridge_object_from_path(bridge, obj_path);
   int x = -1, y = -1;
   Eldbus_Message *ret;

   ELM_ATSPI_OBJ_CHECK_OR_RETURN_DBUS_ERROR(obj, EFL_ACCESS_COMPONENT_MIXIN, msg);

   if (efl_gfx_entity_visible_get(obj))
      evas_object_geometry_get(obj, NULL, NULL, &x, &y);

   ret = eldbus_message_method_return_new(msg);
   EINA_SAFETY_ON_NULL_RETURN_VAL(ret, NULL);

   eldbus_message_arguments_append(ret, "i", x);
   eldbus_message_arguments_append(ret, "i", y);

   return ret;
}

static AtspiComponentLayer
_elm_layer_2_atspi_layer(int layer)
{
   if (layer <= ELM_OBJECT_LAYER_BACKGROUND) return ATSPI_LAYER_CANVAS;
   if (layer < ELM_OBJECT_LAYER_FOCUS) return ATSPI_LAYER_WIDGET;
   if (layer <= ELM_OBJECT_LAYER_TOOLTIP) return ATSPI_LAYER_POPUP;

   return ATSPI_LAYER_OVERLAY;
}

static Eldbus_Message *
_component_get_layer(const Eldbus_Service_Interface *iface EINA_UNUSED, const Eldbus_Message *msg)
{
   const char *obj_path = eldbus_message_path_get(msg);
   Eo *bridge = eldbus_service_object_data_get(iface, ELM_ATSPI_BRIDGE_CLASS_NAME);
   Eo *obj = _bridge_object_from_path(bridge, obj_path);
   int layer = -1;
   Eldbus_Message *ret;
   AtspiComponentLayer atspi_layer;

   ELM_ATSPI_OBJ_CHECK_OR_RETURN_DBUS_ERROR(obj, EFL_ACCESS_COMPONENT_MIXIN, msg);

   if (efl_gfx_entity_visible_get(obj))
     layer = evas_object_layer_get(obj);

   ret = eldbus_message_method_return_new(msg);
   EINA_SAFETY_ON_NULL_RETURN_VAL(ret, NULL);

   atspi_layer = _elm_layer_2_atspi_layer(layer);
   eldbus_message_arguments_append(ret, "u", atspi_layer);

   return ret;
}

static Eldbus_Message *
_component_grab_focus(const Eldbus_Service_Interface *iface EINA_UNUSED, const Eldbus_Message *msg)
{
   const char *obj_path = eldbus_message_path_get(msg);
   Eo *bridge = eldbus_service_object_data_get(iface, ELM_ATSPI_BRIDGE_CLASS_NAME);
   Eo *obj = _bridge_object_from_path(bridge, obj_path);
   Eldbus_Message *ret;
   Eina_Bool focus = EINA_FALSE;

   if (!obj)
     return _dbus_invalid_ref_error_new(msg);

   focus = efl_access_component_focus_grab(obj);

   ret = eldbus_message_method_return_new(msg);
   EINA_SAFETY_ON_NULL_RETURN_VAL(ret, NULL);

   eldbus_message_arguments_append(ret, "b", focus);

   return ret;
}

//TIZEN_ONLY(20171108): bring HIGHLIGHT related changes
static Eldbus_Message *
_component_grab_highlight(const Eldbus_Service_Interface *iface EINA_UNUSED, const Eldbus_Message *msg)
{
   const char *obj_path = eldbus_message_path_get(msg);
   Eo *bridge = eldbus_service_object_data_get(iface, ELM_ATSPI_BRIDGE_CLASS_NAME);
   Eo *obj = _bridge_object_from_path(bridge, obj_path);
   Eldbus_Message *ret;
   Eina_Bool highlight = EINA_FALSE;

   ELM_ATSPI_OBJ_CHECK_OR_RETURN_DBUS_ERROR(obj, EFL_ACCESS_COMPONENT_MIXIN, msg);

   highlight = efl_access_component_highlight_grab(obj);

   ret = eldbus_message_method_return_new(msg);
   EINA_SAFETY_ON_NULL_RETURN_VAL(ret, NULL);

   eldbus_message_arguments_append(ret, "b", highlight);

   return ret;
}

static Eldbus_Message *
_component_clear_highlight(const Eldbus_Service_Interface *iface EINA_UNUSED, const Eldbus_Message *msg)
{
   const char *obj_path = eldbus_message_path_get(msg);
   Eo *bridge = eldbus_service_object_data_get(iface, ELM_ATSPI_BRIDGE_CLASS_NAME);
   Eo *obj = _bridge_object_from_path(bridge, obj_path);
   Eldbus_Message *ret;
   Eina_Bool highlight = EINA_FALSE;

   ELM_ATSPI_OBJ_CHECK_OR_RETURN_DBUS_ERROR(obj, EFL_ACCESS_COMPONENT_MIXIN, msg);

   highlight = efl_access_component_highlight_clear(obj);

   ret = eldbus_message_method_return_new(msg);
   EINA_SAFETY_ON_NULL_RETURN_VAL(ret, NULL);

   eldbus_message_arguments_append(ret, "b", highlight);

   return ret;
}
//

static Eldbus_Message *
_component_get_alpha(const Eldbus_Service_Interface *iface EINA_UNUSED, const Eldbus_Message *msg)
{
   const char *obj_path = eldbus_message_path_get(msg);
   Eo *bridge = eldbus_service_object_data_get(iface, ELM_ATSPI_BRIDGE_CLASS_NAME);
   Eo *obj = _bridge_object_from_path(bridge, obj_path);
   Eldbus_Message *ret;
   double alpha = -1;
   int a;

   if (!obj)
     return _dbus_invalid_ref_error_new(msg);

   if (efl_gfx_entity_visible_get(obj))
     {
        evas_object_color_get(obj, NULL, NULL, NULL, &a);
        alpha = a / 255.0;
     }

   ret = eldbus_message_method_return_new(msg);
   EINA_SAFETY_ON_NULL_RETURN_VAL(ret, NULL);

   eldbus_message_arguments_append(ret, "d", alpha);

   return ret;
}

static Eldbus_Message *
_component_set_extends(const Eldbus_Service_Interface *iface EINA_UNUSED, const Eldbus_Message *msg)
{
   const char *obj_path = eldbus_message_path_get(msg);
   Eo *bridge = eldbus_service_object_data_get(iface, ELM_ATSPI_BRIDGE_CLASS_NAME);
   Eo *obj = _bridge_object_from_path(bridge, obj_path);
   AtspiCoordType coord_type;
   Eldbus_Message *ret;
   int x, y, w, h;
   Eina_Bool result = EINA_FALSE;

   ELM_ATSPI_OBJ_CHECK_OR_RETURN_DBUS_ERROR(obj, EFL_ACCESS_COMPONENT_MIXIN, msg);

   if (!eldbus_message_arguments_get(msg, "iiiiu", &x, &y, &w, &h, &coord_type))
     return eldbus_message_error_new(msg, "org.freedesktop.DBus.Error.InvalidArgs", "Invalid index type.");

   Eina_Bool type = coord_type == ATSPI_COORD_TYPE_SCREEN ? EINA_TRUE : EINA_FALSE;
   result = efl_access_component_extents_set(obj, type, EINA_RECT(x, y, w, h));

   ret = eldbus_message_method_return_new(msg);
   EINA_SAFETY_ON_NULL_RETURN_VAL(ret, NULL);

   eldbus_message_arguments_append(ret, "b", result);

   return ret;
}

static Eldbus_Message *
_component_set_position(const Eldbus_Service_Interface *iface EINA_UNUSED, const Eldbus_Message *msg)
{
   const char *obj_path = eldbus_message_path_get(msg);
   Eo *bridge = eldbus_service_object_data_get(iface, ELM_ATSPI_BRIDGE_CLASS_NAME);
   Eo *obj = _bridge_object_from_path(bridge, obj_path);
   int x = -1, y = -1;
   Eina_Bool result = EINA_FALSE;
   AtspiCoordType coord_type;
   Eldbus_Message *ret;

   ELM_ATSPI_OBJ_CHECK_OR_RETURN_DBUS_ERROR(obj, EFL_ACCESS_COMPONENT_MIXIN, msg);

   if (!eldbus_message_arguments_get(msg, "iiu", &x, &y, &coord_type))
     return eldbus_message_error_new(msg, "org.freedesktop.DBus.Error.InvalidArgs", "Invalid index type.");

   Eina_Bool type = coord_type == ATSPI_COORD_TYPE_SCREEN ? EINA_TRUE : EINA_FALSE;
   if (efl_gfx_entity_visible_get(obj))
     {
        if (type)
          result = efl_access_component_screen_position_set(obj, x, y);
        else
          {
             result = EINA_TRUE;
             evas_object_move(obj, x, y);
          }
     }

   ret = eldbus_message_method_return_new(msg);
   EINA_SAFETY_ON_NULL_RETURN_VAL(ret, NULL);

   eldbus_message_arguments_append(ret, "b", result);

   return ret;
}

static Eldbus_Message *
_component_set_size(const Eldbus_Service_Interface *iface EINA_UNUSED, const Eldbus_Message *msg)
{
   const char *obj_path = eldbus_message_path_get(msg);
   Eo *bridge = eldbus_service_object_data_get(iface, ELM_ATSPI_BRIDGE_CLASS_NAME);
   Eo *obj = _bridge_object_from_path(bridge, obj_path);
   int w, h;
   Eina_Bool result = EINA_TRUE;
   Eldbus_Message *ret;

   ELM_ATSPI_OBJ_CHECK_OR_RETURN_DBUS_ERROR(obj, EFL_ACCESS_COMPONENT_MIXIN, msg);

   if (!eldbus_message_arguments_get(msg, "ii", &w, &h))
     return eldbus_message_error_new(msg, "org.freedesktop.DBus.Error.InvalidArgs", "Invalid index type.");

   evas_object_resize(obj, w, h);

   ret = eldbus_message_method_return_new(msg);
   EINA_SAFETY_ON_NULL_RETURN_VAL(ret, NULL);

   eldbus_message_arguments_append(ret, "b", result);

   return ret;
}

static const Eldbus_Method component_methods[] = {
   { "Contains", ELDBUS_ARGS({"i", "x"}, {"i", "y"}, {"u", "coord_type"}), ELDBUS_ARGS({"b", "contains"}), _component_contains, 0 },
   { "GetAccessibleAtPoint", ELDBUS_ARGS({"i", "x"}, {"i", "y"}, {"u", "coord_type"}), ELDBUS_ARGS({"(so)", "accessible"}), _component_get_accessible_at_point, 0 },
   { "GetExtents", ELDBUS_ARGS({"u", "coord_type"}), ELDBUS_ARGS({"(iiii)", "extents"}), _component_get_extents, 0 },
   { "GetPosition", ELDBUS_ARGS({"u", "coord_type"}), ELDBUS_ARGS({"i", "x"}, {"i","y"}), _component_get_position, 0 },
   { "GetSize", NULL, ELDBUS_ARGS({"i", "w"}, {"i", "h"}), _component_get_size, 0 },
   { "GetLayer", NULL, ELDBUS_ARGS({"u", "layer"}), _component_get_layer, 0 },
//   { "GetMDIZOrder", NULL, ELDBUS_ARGS({"n", "MDIZOrder"}), _component_get_mdizorder, 0 },
   { "GrabFocus", NULL, ELDBUS_ARGS({"b", "focus"}), _component_grab_focus, 0 },
   { "GetAlpha", NULL, ELDBUS_ARGS({"d", "alpha"}), _component_get_alpha, 0 },
   { "SetExtents", ELDBUS_ARGS({"i", "x"}, {"i", "y"}, {"i", "width"}, {"i", "height"}, {"u", "coord_type"}), ELDBUS_ARGS({"b", "result"}), _component_set_extends, 0 },
   { "SetPosition", ELDBUS_ARGS({"i", "x"}, {"i", "y"}, {"u", "coord_type"}), ELDBUS_ARGS({"b", "result"}), _component_set_position, 0 },
   { "SetSize", ELDBUS_ARGS({"i", "width"}, {"i", "height"}), ELDBUS_ARGS({"b", "result"}), _component_set_size, 0 },
   // TIZEN_ONLY(20171108): bring HIGHLIGHT related changes
   { "GrabHighlight", NULL, ELDBUS_ARGS({"b", "result"}), _component_grab_highlight, 0 },
   { "ClearHighlight", NULL, ELDBUS_ARGS({"b", "result"}), _component_clear_highlight, 0 },
   //
   { NULL, NULL, NULL, NULL, 0 }
};

static const Eldbus_Service_Interface_Desc component_iface_desc = {
   ATSPI_DBUS_INTERFACE_COMPONENT, component_methods, NULL, NULL, NULL, NULL
};

static void
_on_elm_atspi_bridge_app_register(void *data EINA_UNUSED, const Eldbus_Message *msg, Eldbus_Pending *pending EINA_UNUSED)
{
   const char *errname, *errmsg;

   if (eldbus_message_error_get(msg, &errname, &errmsg))
     {
        ERR("%s %s", errname, errmsg);
        return;
     }
   DBG("Application successfully registered at ATSPI2 bus.");
}

EAPI Eina_Bool
_elm_atspi_bridge_app_register(Eo *bridge)
{
   Eo *root;
   ELM_ATSPI_BRIDGE_DATA_GET_OR_RETURN_VAL(bridge, pd, EINA_FALSE);

   Eldbus_Message *message = eldbus_message_method_call_new(ATSPI_DBUS_NAME_REGISTRY,
                                    ATSPI_DBUS_PATH_ROOT,
                                    ATSPI_DBUS_INTERFACE_SOCKET,
                                    "Embed");
   Eldbus_Message_Iter *iter = eldbus_message_iter_get(message);

   root = efl_access_object_access_root_get(EFL_ACCESS_OBJECT_MIXIN);
   _bridge_iter_object_reference_append(bridge, iter, root);
   eldbus_connection_send(pd->a11y_bus, message, _on_elm_atspi_bridge_app_register, NULL, -1);

   return EINA_TRUE;
}

EAPI Eina_Bool
_elm_atspi_bridge_app_unregister(Eo *bridge)
{
   Eo *root;
   ELM_ATSPI_BRIDGE_DATA_GET_OR_RETURN_VAL(bridge, pd, EINA_FALSE);

   root = efl_access_object_access_root_get(EFL_ACCESS_OBJECT_MIXIN);

   Eldbus_Message *message = eldbus_message_method_call_new(ATSPI_DBUS_NAME_REGISTRY,
                                    ATSPI_DBUS_PATH_ROOT,
                                    ATSPI_DBUS_INTERFACE_SOCKET,
                                    "Unembed");
   Eldbus_Message_Iter *iter = eldbus_message_iter_get(message);

   _bridge_iter_object_reference_append(bridge, iter, root);
   eldbus_connection_send(pd->a11y_bus, message, NULL, NULL, -1);

   return EINA_TRUE;
}

static void
_cache_register(Eo *obj)
{
   ELM_ATSPI_BRIDGE_DATA_GET_OR_RETURN(obj, pd);
   pd->cache_interface = eldbus_service_interface_register(pd->a11y_bus, CACHE_INTERFACE_PATH, &cache_iface_desc);
   eldbus_service_object_data_set(pd->cache_interface, ELM_ATSPI_BRIDGE_CLASS_NAME, obj);
}

static void
_set_broadcast_flag(const char *event, Eo *bridge)
{
   char **tokens;
   ELM_ATSPI_BRIDGE_DATA_GET_OR_RETURN(bridge, pd);

   tokens = eina_str_split(event, ":", 3);

   if (!tokens) return;

   if (!strcmp(tokens[0], "Object"))
     {
        if (!tokens[1] || *tokens[1] == '\0') return; // do not handle "Object:*"
        else if (!strcmp(tokens[1], "StateChanged"))
          {
             if (!tokens[2] || *tokens[2] == '\0')
               pd->object_state_broadcast_mask = -1; // broadcast all
             eina_str_tolower(&tokens[2]);
             struct atspi_state_desc *sd = eina_hash_find(pd->state_hash, tokens[2]);
             if (sd)
               STATE_TYPE_SET(pd->object_state_broadcast_mask, sd->elm_state);
          }
        else if (!strcmp(tokens[1], "PropertyChange"))
          {
             if (!tokens[2] || *tokens[2] == '\0')
               pd->object_property_broadcast_mask = -1; //broadcast all
             else if (!strcmp(tokens[2], "AccessibleValue"))
               STATE_TYPE_SET(pd->object_property_broadcast_mask, ATSPI_OBJECT_PROPERTY_VALUE);
             else if (!strcmp(tokens[2], "AccessibleName"))
               STATE_TYPE_SET(pd->object_property_broadcast_mask, ATSPI_OBJECT_PROPERTY_NAME);
             else if (!strcmp(tokens[2], "AccessibleDescription"))
               STATE_TYPE_SET(pd->object_property_broadcast_mask, ATSPI_OBJECT_PROPERTY_DESCRIPTION);
             else if (!strcmp(tokens[2], "AccessibleParent"))
               STATE_TYPE_SET(pd->object_property_broadcast_mask, ATSPI_OBJECT_PROPERTY_PARENT);
             else if (!strcmp(tokens[2], "AccessibleRole"))
               STATE_TYPE_SET(pd->object_property_broadcast_mask, ATSPI_OBJECT_PROPERTY_ROLE);
          }
        else if (!strcmp(tokens[1], "ChildrenChanged"))
          {
             if (!tokens[2] || *tokens[2] == '\0')
               pd->object_children_broadcast_mask = -1; // broadcast all
             else if (!strcmp(tokens[2], "add"))
               STATE_TYPE_SET(pd->object_children_broadcast_mask, ATSPI_OBJECT_CHILD_ADDED);
             else if (!strcmp(tokens[2], "remove"))
               STATE_TYPE_SET(pd->object_children_broadcast_mask, ATSPI_OBJECT_CHILD_REMOVED);
          }
        else if (!strcmp(tokens[1], "TextChanged"))
          STATE_TYPE_SET(pd->object_broadcast_mask, ATSPI_OBJECT_EVENT_TEXT_CHANGED);
        else if (!strcmp(tokens[1], "TextCaretMoved"))
          STATE_TYPE_SET(pd->object_broadcast_mask, ATSPI_OBJECT_EVENT_TEXT_CARET_MOVED);
        else if (!strcmp(tokens[1], "TextBoundsChanged"))
          STATE_TYPE_SET(pd->object_broadcast_mask, ATSPI_OBJECT_EVENT_TEXT_BOUNDS_CHANGED);
        else if (!strcmp(tokens[1], "TextSelectionChanged"))
          STATE_TYPE_SET(pd->object_broadcast_mask, ATSPI_OBJECT_EVENT_TEXT_SELECTION_CHANGED);
        else if (!strcmp(tokens[1], "TextAttributesChanged"))
          STATE_TYPE_SET(pd->object_broadcast_mask, ATSPI_OBJECT_EVENT_TEXT_ATTRIBUTES_CHANGED);
        else if (!strcmp(tokens[1], "VisibleDataChanged"))
          STATE_TYPE_SET(pd->object_broadcast_mask, ATSPI_OBJECT_EVENT_VISIBLE_DATA_CHANGED);
        else if (!strcmp(tokens[1], "ActiveDescendantChanged"))
          STATE_TYPE_SET(pd->object_broadcast_mask, ATSPI_OBJECT_EVENT_ACTIVE_DESCENDANT_CHANGED);
        else if (!strcmp(tokens[1], "SelectionChanged"))
          STATE_TYPE_SET(pd->object_broadcast_mask, ATSPI_OBJECT_EVENT_SELECTION_CHANGED);
        else if (!strcmp(tokens[1], "BoundsChanged"))
          STATE_TYPE_SET(pd->object_broadcast_mask, ATSPI_OBJECT_EVENT_BOUNDS_CHANGED);
        //TIZEN_ONLY(20160623): atspi: moved highlight when object is out of screen
        else if (!strcmp(tokens[1], "MoveOuted"))
          STATE_TYPE_SET(pd->object_broadcast_mask, ATSPI_OBJECT_EVENT_MOVE_OUTED);
        //
     }
   else if (!strcmp(tokens[0], "Window"))
     {
        if (!tokens[1] || *tokens[1] == '\0')
          pd->window_signal_broadcast_mask = -1; // broadcast all
        else if (!strcmp(tokens[1], "Create"))
          STATE_TYPE_SET(pd->window_signal_broadcast_mask, ATSPI_WINDOW_EVENT_CREATE);
        else if (!strcmp(tokens[1], "Destroy"))
          STATE_TYPE_SET(pd->window_signal_broadcast_mask, ATSPI_WINDOW_EVENT_DESTROY);
        else if (!strcmp(tokens[1], "Activate"))
          STATE_TYPE_SET(pd->window_signal_broadcast_mask, ATSPI_WINDOW_EVENT_ACTIVATE);
        else if (!strcmp(tokens[1], "Deactivate"))
          STATE_TYPE_SET(pd->window_signal_broadcast_mask, ATSPI_WINDOW_EVENT_DEACTIVATE);
        else if (!strcmp(tokens[1], "Maximize"))
          STATE_TYPE_SET(pd->window_signal_broadcast_mask, ATSPI_WINDOW_EVENT_MAXIMIZE);
        else if (!strcmp(tokens[1], "Minimize"))
          STATE_TYPE_SET(pd->window_signal_broadcast_mask, ATSPI_WINDOW_EVENT_MINIMIZE);
        else if (!strcmp(tokens[1], "Resize"))
          STATE_TYPE_SET(pd->window_signal_broadcast_mask, ATSPI_WINDOW_EVENT_RESIZE);
        else if (!strcmp(tokens[1], "Restore"))
          STATE_TYPE_SET(pd->window_signal_broadcast_mask, ATSPI_WINDOW_EVENT_RESTORE);
     }

   free(tokens[0]);
   free(tokens);
}

static void
_registered_listeners_get(void *data, const Eldbus_Message *msg, Eldbus_Pending *pending)
{
   const char *event, *bus;
   ELM_ATSPI_BRIDGE_DATA_GET_OR_RETURN(data, pd);
   pd->pending_requests = eina_list_remove(pd->pending_requests, pending);

   DBG("Updating registered ATSPI signals list.");
   pd->object_broadcast_mask = 0;
   pd->object_children_broadcast_mask = 0;
   pd->object_property_broadcast_mask = 0;
   pd->object_state_broadcast_mask = 0;
   pd->window_signal_broadcast_mask = 0;

   if (eldbus_message_error_get(msg, &event, &bus))
     {
        WRN("%s %s", event, bus);
        return;
     }
   Eldbus_Message_Iter *iter, *siter;
   if (!eldbus_message_arguments_get(msg, "a(ss)", &iter))
     {
        ERR("Invalid answer type from GetRegisteredEvents method call!");
        return;
     }
   while (eldbus_message_iter_get_and_next(iter, 'r', &siter))
     {
        if (!eldbus_message_iter_arguments_get(siter, "ss", &bus, &event))
          ERR("Cannot get bus and event from registered listener");
        else _set_broadcast_flag(event, data);
     }

   //TIZEN_ONLY(20171108): make atspi_proxy work
   if (!pd->connected)
     {
        Eo *root, *pr;
        //TIZEN_ONLY(20170910) atspi: emit signal after atspi bridge is connected
        pd->connected = EINA_TRUE;
        efl_event_callback_legacy_call(data, ELM_ATSPI_BRIDGE_EVENT_CONNECTED, NULL);
        _elm_win_atspi(EINA_TRUE);
        //

        // buid cache
        root = elm_obj_atspi_bridge_root_get(data);
        _bridge_cache_build(data, root);

        // initialize pending proxy
        EINA_LIST_FREE(pd->socket_queue, pr)
           _socket_ifc_create(pd->a11y_bus, pr);
        EINA_LIST_FREE(pd->plug_queue, pr)
           _plug_connect(pd->a11y_bus, pr);

        pd->socket_queue = pd->plug_queue = NULL;
     }
   //
}

static void
_registered_events_list_update(Eo *bridge)
{
   Eldbus_Message *msg;
   ELM_ATSPI_BRIDGE_DATA_GET_OR_RETURN(bridge, pd);
   Eldbus_Pending *p;

   msg = eldbus_message_method_call_new(ATSPI_DBUS_NAME_REGISTRY, ATSPI_DBUS_PATH_REGISTRY, ATSPI_DBUS_INTERFACE_REGISTRY, "GetRegisteredEvents");
   p = eldbus_connection_send(pd->a11y_bus, msg, _registered_listeners_get, bridge, -1);
   pd->pending_requests = eina_list_append(pd->pending_requests, p);
}

static void
_handle_listener_change(void *data, const Eldbus_Message *msg EINA_UNUSED)
{
   _registered_events_list_update(data);
}

//TIZEN_ONLY(20170802): handle "gesture_required" attribute
static Eina_Bool
_scroll_gesture_required_is(Eo *obj)
{
   Eina_Bool ret = EINA_FALSE;
   Eina_List *l, *attr_list = NULL;
   Efl_Access_Attribute *attr = NULL;

   attr_list = efl_access_object_attributes_get(obj);
   EINA_LIST_FOREACH(attr_list, l, attr)
     {
        if (!strcmp(attr->key, "gesture_required") && !strcmp(attr->value, "scroll"))
          {
             ret = EINA_TRUE;
             break;
          }
     }
   if (attr_list)
     efl_access_attributes_list_free(attr_list);

   return ret;
}
//

static void
_state_changed_signal_send(void *data, const Efl_Event *event)
{
   Efl_Access_Event_State_Changed_Data *state_data = event->info;
   const char *type_desc;
   ELM_ATSPI_BRIDGE_DATA_GET_OR_RETURN(data, pd);

   // TIZEN_ONLY(20160802): do not handle events if the window is not activated
   if ((state_data->type == EFL_ACCESS_STATE_ACTIVE) && efl_isa(event->object, EFL_UI_WIN_CLASS))
     {
        pd->window_activated = state_data->new_value;
     }
   //

   // TIZEN_ONLY(20161209): reduce IPC of object:state-changed:showing
   if ((state_data->type == EFL_ACCESS_STATE_SHOWING) ||
       (state_data->type == EFL_ACCESS_STATE_VISIBLE))
     {
        Efl_Access_Role role = EFL_ACCESS_ROLE_INVALID;
        Efl_Access_State_Set ss;

        role = efl_access_object_role_get(event->object);
        ss = efl_access_object_state_set_get(event->object);
        if (state_data->new_value) /* Showing */
          {
             if ((role != EFL_ACCESS_ROLE_WINDOW) &&
                 (role != EFL_ACCESS_ROLE_PAGE_TAB) &&
                 (!STATE_TYPE_GET(ss, EFL_ACCESS_STATE_MODAL)))
               {
                  efl_event_callback_stop(event->object);
                  return;
               }
          }
        else /* Not Showing */
          {
             if ((role != EFL_ACCESS_ROLE_WINDOW) &&
                 (!STATE_TYPE_GET(ss, EFL_ACCESS_STATE_MODAL)) &&
                 (_elm_object_accessibility_currently_highlighted_get() != (void *)event->object))
               {
                  efl_event_callback_stop(event->object);
                  return;
               }
          }
     }
   //

   if (!STATE_TYPE_GET(pd->object_state_broadcast_mask, state_data->type))
     {
        efl_event_callback_stop(event->object);
        return;
     }

   if ((state_data->type > EFL_ACCESS_STATE_LAST_DEFINED) ||
        (int)state_data->type < 0)
     {
        efl_event_callback_stop(event->object);
        return;
     }

   type_desc = elm_states_to_atspi_state[state_data->type].name;
   //TIZEN_ONLY(20170802): handle "gesture_required" attribute
   unsigned int det2 = 0;
   if (state_data->type == EFL_ACCESS_STATE_HIGHLIGHTED)
    {
       if (_ee_obscured_get(event->object))
         return;

       if (_scroll_gesture_required_is(event->object))
        det2++;
    }

   _bridge_signal_send(data, event->object, ATSPI_DBUS_INTERFACE_EVENT_OBJECT,
                       &_event_obj_signals[ATSPI_OBJECT_EVENT_STATE_CHANGED], type_desc, state_data->new_value, det2, NULL);
   //
}

static void
_bounds_changed_signal_send(void *data, const Efl_Event *event)
{
   Efl_Access_Event_Geometry_Changed_Data *geo_data = event->info;

   _bridge_signal_send(data, event->object, ATSPI_DBUS_INTERFACE_EVENT_OBJECT,
                       &_event_obj_signals[ATSPI_OBJECT_EVENT_BOUNDS_CHANGED], "", 0, 0, "(iiii)",
                       geo_data->x, geo_data->y, geo_data->width, geo_data->height);
}

static void
_property_changed_signal_send(void *data, const Efl_Event *event)
{
   const char *property = event->info;
   char *atspi_desc;
   enum _Atspi_Object_Property prop = ATSPI_OBJECT_PROPERTY_LAST;

   ELM_ATSPI_BRIDGE_DATA_GET_OR_RETURN(data, pd);

   if (!strcmp(property, "parent"))
     {
        prop = ATSPI_OBJECT_PROPERTY_PARENT;
        atspi_desc = "accessible-parent";
     }
   else if (!strcmp(property, "i18n_name"))
     {
        prop = ATSPI_OBJECT_PROPERTY_NAME;
        atspi_desc = "accessible-name";
     }
   else if (!strcmp(property, "description"))
     {
        prop = ATSPI_OBJECT_PROPERTY_DESCRIPTION;
        atspi_desc = "accessible-description";
     }
   else if (!strcmp(property, "role"))
     {
        prop = ATSPI_OBJECT_PROPERTY_ROLE;
        atspi_desc = "accessible-role";
     }
   else if (!strcmp(property, "value"))
     {
        prop = ATSPI_OBJECT_PROPERTY_VALUE;
        atspi_desc = "accessible-value";
     }
   if (prop == ATSPI_OBJECT_PROPERTY_LAST)
     {
        ERR("Unrecognized property name!");
        efl_event_callback_stop(event->object);
        return;
     }
   if (!STATE_TYPE_GET(pd->object_property_broadcast_mask, prop))
     {
        DBG("Masking property %s changed event.", property);
        efl_event_callback_stop(event->object);
        return;
     }

   _bridge_signal_send(data, event->object, ATSPI_DBUS_INTERFACE_EVENT_OBJECT,
                       &_event_obj_signals[ATSPI_OBJECT_EVENT_PROPERTY_CHANGED], atspi_desc, 0, 0, NULL);
}

static void
_visible_data_changed_signal_send(void *data, const Efl_Event *event)
{
   ELM_ATSPI_BRIDGE_DATA_GET_OR_RETURN(data, pd);

   if (!STATE_TYPE_GET(pd->object_broadcast_mask, ATSPI_OBJECT_EVENT_VISIBLE_DATA_CHANGED))
     {
        efl_event_callback_stop(event->object);
        return;
     }

   _bridge_signal_send(data, event->object, ATSPI_DBUS_INTERFACE_EVENT_OBJECT,
                       &_event_obj_signals[ATSPI_OBJECT_EVENT_VISIBLE_DATA_CHANGED], "",
                       0, 0, NULL);
}

static void
_active_descendant_changed_signal_send(void *data, const Efl_Event *event)
{
   Eo *child = event->info;
   int idx;

   ELM_ATSPI_BRIDGE_DATA_GET_OR_RETURN(data, pd);

   if (!STATE_TYPE_GET(pd->object_broadcast_mask, ATSPI_OBJECT_EVENT_ACTIVE_DESCENDANT_CHANGED))
     {
        efl_event_callback_stop(event->object);
        return;
     }

   idx = efl_access_object_index_in_parent_get(child);

   _bridge_signal_send(data, event->object, ATSPI_DBUS_INTERFACE_EVENT_OBJECT,
                       &_event_obj_signals[ATSPI_OBJECT_EVENT_ACTIVE_DESCENDANT_CHANGED], "",
                       idx, 0, "(so)", eldbus_connection_unique_name_get(pd->a11y_bus), child);
}

static void
_children_changed_signal_send(void *data, const Efl_Event *event)
{
   const char *atspi_desc = NULL;
   Efl_Access_Event_Children_Changed_Data *ev_data = event->info;
   int idx;
   enum _Atspi_Object_Child_Event_Type type;

   ELM_ATSPI_BRIDGE_DATA_GET_OR_RETURN(data, pd);

   if (ev_data->is_added)
     {
        type = ATSPI_OBJECT_CHILD_ADDED;
        atspi_desc = "add";
     }
   else
     {
        type = ATSPI_OBJECT_CHILD_REMOVED;
        atspi_desc = "remove";
     }

   //TIZEN_ONLY(20171108): make atspi_proxy work
   // update cached objects
   if (ev_data->is_added)
     _bridge_cache_build(data, ev_data->child);
   //

   if (!STATE_TYPE_GET(pd->object_children_broadcast_mask, type))
     {
        efl_event_callback_stop(event->object);
        return;
     }

   idx = efl_access_object_index_in_parent_get(ev_data->child);
   _bridge_signal_send(data, event->object, ATSPI_DBUS_INTERFACE_EVENT_OBJECT,
                       &_event_obj_signals[ATSPI_OBJECT_EVENT_CHILDREN_CHANGED], atspi_desc,
                       idx, 0, "(so)", eldbus_connection_unique_name_get(pd->a11y_bus), ev_data->child);
}

//TIZEN_ONLY(20160623): atspi: moved highlight when object is out of screen
static void
_move_outed_signal_send(void *data, const Efl_Event *event)
{
   const Efl_Access_Move_Outed_Type *type = event->info;

   ELM_ATSPI_BRIDGE_DATA_GET_OR_RETURN(data, pd);

   if (!STATE_TYPE_GET(pd->object_broadcast_mask, ATSPI_OBJECT_EVENT_MOVE_OUTED))
     {
        efl_event_callback_stop(event->object);
        return;
     }
   _bridge_signal_send(data, event->object, ATSPI_DBUS_INTERFACE_EVENT_OBJECT,
                       &_event_obj_signals[ATSPI_OBJECT_EVENT_MOVE_OUTED], "", *type, 0, NULL, NULL);
}
//

static unsigned int
_window_activated_detail_value_add(Eo *obj)
{
   unsigned int ret = EFL_ACCESS_WINDOW_ACTIVATE_INFO_DEFAULT_LABEL_ENABLED;
   Eina_List *l, *attr_list = NULL;
   Efl_Access_Attribute *attr = NULL;

   attr_list = efl_access_object_attributes_get(obj);
   EINA_LIST_FOREACH(attr_list, l, attr)
     {
        if (!strcmp(attr->key, "default_label"))
          {
             if(!strcmp(attr->value, "enabled_without_window"))
               {
                  ret |= EFL_ACCESS_WINDOW_ACTIVATE_INFO_DEFAULT_LABEL_ENABLED_WITHOUT_WINDOW;
               }
             else if(!strcmp(attr->value, "disabled"))
               {
                  ret |= EFL_ACCESS_WINDOW_ACTIVATE_INFO_DEFAULT_LABEL_DISABLED;
               }
          }
     }
   if (attr_list)
     efl_access_attributes_list_free(attr_list);

   Efl_Access_Role role = EFL_ACCESS_ROLE_INVALID;
   role = efl_access_object_role_get(obj);
   if (role == EFL_ACCESS_ROLE_INPUT_METHOD_WINDOW)
     ret |= EFL_ACCESS_WINDOW_ACTIVATE_INFO_KEYBOARD;

   return ret;
}

static void
_window_signal_send(void *data, const Efl_Event *event)
{
   enum _Atspi_Window_Signals type;

   ELM_ATSPI_BRIDGE_DATA_GET_OR_RETURN(data, pd);

   if (event->desc == EFL_ACCESS_WINDOW_EVENT_WINDOW_CREATED)
     type = ATSPI_WINDOW_EVENT_CREATE;
   else if (event->desc == EFL_ACCESS_WINDOW_EVENT_WINDOW_DESTROYED)
     type = ATSPI_WINDOW_EVENT_DESTROY;
   else if (event->desc == EFL_ACCESS_WINDOW_EVENT_WINDOW_DEACTIVATED)
     type = ATSPI_WINDOW_EVENT_DEACTIVATE;
   else if (event->desc == EFL_ACCESS_WINDOW_EVENT_WINDOW_ACTIVATED)
     type = ATSPI_WINDOW_EVENT_ACTIVATE;
   else if (event->desc == EFL_ACCESS_WINDOW_EVENT_WINDOW_MAXIMIZED)
     type = ATSPI_WINDOW_EVENT_MAXIMIZE;
   else if (event->desc == EFL_ACCESS_WINDOW_EVENT_WINDOW_MINIMIZED)
     type = ATSPI_WINDOW_EVENT_MINIMIZE;
   else if (event->desc == EFL_ACCESS_WINDOW_EVENT_WINDOW_RESTORED)
     type = ATSPI_WINDOW_EVENT_RESTORE;
   else
     {
        efl_event_callback_stop(event->object);
        return;
     }

   if (!STATE_TYPE_GET(pd->window_signal_broadcast_mask, type))
     {
        efl_event_callback_stop(event->object);
        return;
     }

   if (!pd->a11y_bus)
     {
        ERR("A11Y connection closed. Unable to send ATSPI event.");
        efl_event_callback_stop(event->object);
        return;
     }
   //TIZEN_ONLY(20170925) atspi: send detail value for window activated signal
   unsigned int det1 = 0;
   if (event->desc == EFL_ACCESS_WINDOW_EVENT_WINDOW_ACTIVATED)
     det1 = _window_activated_detail_value_add(event->object);
   //

   _bridge_signal_send(data, event->object, ATSPI_DBUS_INTERFACE_EVENT_WINDOW,
                       &_window_obj_signals[type], "", det1, 0, "i", 0);
}

static void
_selection_signal_send(void *data, const Efl_Event *event)
{
   ELM_ATSPI_BRIDGE_DATA_GET_OR_RETURN(data, pd);

   if (!STATE_TYPE_GET(pd->object_broadcast_mask, ATSPI_OBJECT_EVENT_SELECTION_CHANGED))
     {
        efl_event_callback_stop(event->object);
        return;
     }

   _bridge_signal_send(data, event->object, ATSPI_DBUS_INTERFACE_EVENT_OBJECT,
                       &_event_obj_signals[ATSPI_OBJECT_EVENT_SELECTION_CHANGED], "", 0, 0, "i", 0);
}

static void _bridge_signal_send(Eo *bridge, Eo *obj, const char *infc, const Eldbus_Signal *signal, const char *minor, unsigned int det1, unsigned int det2, const char *variant_sig, ...)
{
   Eldbus_Message *msg;
   Eldbus_Message_Iter *iter , *iter_stack[64], *iter_struct;
   va_list va;
   Eo *atspi_obj, *root;
   const char *path;
   int top = 0;

   EINA_SAFETY_ON_NULL_RETURN(infc);
   EINA_SAFETY_ON_NULL_RETURN(signal);
   EINA_SAFETY_ON_NULL_RETURN(minor);
   EINA_SAFETY_ON_NULL_RETURN(obj);
   ELM_ATSPI_BRIDGE_DATA_GET_OR_RETURN(bridge, pd);

   path = _path_from_object(obj);
   root = efl_access_object_access_root_get(EFL_ACCESS_OBJECT_MIXIN);

   msg = eldbus_message_signal_new(path, infc, signal->name);
   if (!msg) return;

   va_start(va, variant_sig);

   iter = eldbus_message_iter_get(msg);
   eldbus_message_iter_arguments_append(iter, "sii", minor, det1, det2);

   if (variant_sig)
     {
        iter_stack[top] = eldbus_message_iter_container_new(iter, 'v', variant_sig);

        const char *tmp = variant_sig;
        while (*tmp)
          {
             switch (*tmp)
               {
                case '(':
                   iter_stack[top + 1] = eldbus_message_iter_container_new(iter_stack[top], 'r', NULL);
                   top++;
                   break;
                case 's':
                   eldbus_message_iter_basic_append(iter_stack[top], 's', va_arg(va, char*));
                   break;
                case 'i':
                   eldbus_message_iter_basic_append(iter_stack[top], 'i', va_arg(va, int));
                   break;
                case 'o':
                   atspi_obj = va_arg(va, Eo*);
                   path = _path_from_object(atspi_obj);
                   eldbus_message_iter_basic_append(iter_stack[top], 'o', path);
                   break;
                case ')':
                   eldbus_message_iter_container_close(iter_stack[top - 1], iter_stack[top]);
                   top--;
                   break;
                default:
                   ERR("Not supported d-bus type: %c.", *tmp);
                   break;
               }
             tmp++;
          }
     }
   else // AT-SPI implementation forces checks on variant in signature even if not used.
     {
        iter_stack[top] = eldbus_message_iter_container_new(iter, 'v', "i");
        eldbus_message_iter_basic_append(iter_stack[top], 'i', 0);
     }

   va_end(va);
   if (top != 0)
     ERR("Invalid d-bus signature: () do not match.");

   eldbus_message_iter_container_close(iter, iter_stack[0]);

   iter_struct = eldbus_message_iter_container_new(iter, 'r', NULL);
   path = _path_from_object(root);
   eldbus_message_iter_basic_append(iter_struct, 's', eldbus_connection_unique_name_get(pd->a11y_bus));
   eldbus_message_iter_basic_append(iter_struct, 'o', path);
   eldbus_message_iter_container_close(iter, iter_struct);

   eldbus_connection_send(pd->a11y_bus, msg, NULL, NULL, -1);
   DBG("Send %s.%s[%s,%d,%d]", infc, signal->name, minor, det1, det2);
}

static void
_text_caret_moved_send(void *data, const Efl_Event *event)
{
   int cursor_pos = 0;

   ELM_ATSPI_BRIDGE_DATA_GET_OR_RETURN(data, pd);

   if (!STATE_TYPE_GET(pd->object_broadcast_mask, ATSPI_OBJECT_EVENT_TEXT_CARET_MOVED))
     return;

   cursor_pos = efl_access_text_caret_offset_get(event->object);

   _bridge_signal_send(data, event->object, ATSPI_DBUS_INTERFACE_EVENT_OBJECT,
                       &_event_obj_signals[ATSPI_OBJECT_EVENT_TEXT_CARET_MOVED], "", cursor_pos, 0, NULL);
}

static void
_text_text_inserted_send(void *data, const Efl_Event *event)
{
   Efl_Access_Text_Change_Info *info = event->info;

   ELM_ATSPI_BRIDGE_DATA_GET_OR_RETURN(data, pd);

   if (!STATE_TYPE_GET(pd->object_broadcast_mask, ATSPI_OBJECT_EVENT_TEXT_CHANGED))
     return;

   if (!info->content)
     {
        WRN("Try to send signal with NULL value");
        return;
     }

   _bridge_signal_send(data, event->object, ATSPI_DBUS_INTERFACE_EVENT_OBJECT,
                       &_event_obj_signals[ATSPI_OBJECT_EVENT_TEXT_CHANGED], "insert", info->pos, info->len, "s", info->content);
}

static void
_text_text_removed_send(void *data, const Efl_Event *event)
{
   Efl_Access_Text_Change_Info *info = event->info;

   ELM_ATSPI_BRIDGE_DATA_GET_OR_RETURN(data, pd);

   if (!STATE_TYPE_GET(pd->object_broadcast_mask, ATSPI_OBJECT_EVENT_TEXT_CHANGED))
     return;

   if (!info->content)
     {
        WRN("Try to send signal with NULL value");
        return;
     }

   _bridge_signal_send(data, event->object, ATSPI_DBUS_INTERFACE_EVENT_OBJECT,
                       &_event_obj_signals[ATSPI_OBJECT_EVENT_TEXT_CHANGED], "delete", info->pos, info->len, "s", info->content);
}

static void
_text_selection_changed_send(void *data, const Efl_Event *event)
{
   ELM_ATSPI_BRIDGE_DATA_GET_OR_RETURN(data, pd);

   if (!STATE_TYPE_GET(pd->object_broadcast_mask, ATSPI_OBJECT_EVENT_TEXT_SELECTION_CHANGED))
     return;

   _bridge_signal_send(data, event->object, ATSPI_DBUS_INTERFACE_EVENT_OBJECT,
                       &_event_obj_signals[ATSPI_OBJECT_EVENT_TEXT_SELECTION_CHANGED], "", 0, 0, NULL);
}

//TIZEN_ONLY(20160527) - Add direct reading feature
static void
_on_reading_state_changed(void *data EINA_UNUSED, const Eldbus_Message *msg)
{
   const int32_t i;
   const char *say_signal_name = "";
   Elm_Atspi_Say_Info *say_info;

   if (eldbus_message_arguments_get(msg, "is", &i, &say_signal_name))
     {  if (read_command_id)
          {
             say_info = eina_hash_find(read_command_id, &i);
             if (say_info)
               {
                  if (say_info->func && say_signal_name)
                     say_info->func(say_info->data, say_signal_name);
                  eina_hash_del(read_command_id, &i, NULL);
                  free(say_info);
               }
          }
     }
}
//

static void
_event_handlers_register(Eo *bridge)
{
   ELM_ATSPI_BRIDGE_DATA_GET_OR_RETURN(bridge, pd);

   _registered_events_list_update(bridge);

   // register signal handlers in order to update list of registered listeners of ATSPI-Clients
   pd->register_hdl = eldbus_signal_handler_add(pd->a11y_bus, ATSPI_DBUS_NAME_REGISTRY, ATSPI_DBUS_PATH_REGISTRY, ATSPI_DBUS_INTERFACE_REGISTRY, "EventListenerRegistered", _handle_listener_change, bridge);
   pd->unregister_hdl = eldbus_signal_handler_add(pd->a11y_bus, ATSPI_DBUS_NAME_REGISTRY, ATSPI_DBUS_PATH_REGISTRY, ATSPI_DBUS_INTERFACE_REGISTRY, "EventListenerDeregistered", _handle_listener_change, bridge);
   //TIZEN_ONLY(20160527) - Add direct reading feature
   pd->reading_state_changed_hdl = eldbus_signal_handler_add(pd->a11y_bus, ELM_ATSPI_DIRECT_READ_BUS, ELM_ATSPI_DIRECT_READ_PATH, ELM_ATSPI_DIRECT_READ_INTERFACE, "ReadingStateChanged", _on_reading_state_changed, bridge);
   //

   pd->key_flr = ecore_event_filter_add(NULL, _elm_atspi_bridge_key_filter, NULL, bridge);
}

static void
_bridge_object_unregister(Eo *bridge, Eo *obj)
{
   ELM_ATSPI_BRIDGE_DATA_GET_OR_RETURN(bridge, pd);

   eina_hash_del(pd->cache, &obj, obj);
}

static void
_on_object_add(void *data, const Efl_Event *event)
{
   Eldbus_Message *sig;
   Eldbus_Message_Iter *iter;

   ELM_ATSPI_BRIDGE_DATA_GET_OR_RETURN(data, pd);

   sig = eldbus_service_signal_new(pd->cache_interface, ATSPI_OBJECT_CHILD_ADDED);
   iter = eldbus_message_iter_get(sig);
   _cache_item_reference_append_cb(data, event->object, iter);

   eldbus_service_signal_send(pd->cache_interface, sig);
}

static void
_on_object_del(void *data, const Efl_Event *event)
{
   Eldbus_Message *sig;

   ELM_ATSPI_BRIDGE_DATA_GET_OR_RETURN(data, pd);

   _bridge_object_unregister(data, event->object);

   sig = eldbus_service_signal_new(pd->cache_interface, ATSPI_OBJECT_CHILD_REMOVED);
   Eldbus_Message_Iter *iter = eldbus_message_iter_get(sig);
   _bridge_iter_object_reference_append(data, iter, event->object);
   eldbus_service_signal_send(pd->cache_interface, sig);
}

//TIZEN_ONLY(20171108): make atspi_proxy work
static void
_bridge_cache_build(Eo *bridge, void *obj)
{
   Eina_List *children;
   Efl_Access_State_Set ss;
   Eo *child;

   ELM_ATSPI_BRIDGE_DATA_GET_OR_RETURN(bridge, pd);

   if (!efl_isa(obj, EFL_ACCESS_OBJECT_MIXIN))
     return;

   if (!efl_isa(obj, ELM_ATSPI_PROXY_CLASS))
      _bridge_object_register(bridge, obj);

   ss = efl_access_object_state_set_get(obj);
   if (STATE_TYPE_GET(ss, EFL_ACCESS_STATE_MANAGES_DESCENDANTS))
     return;
   if (efl_isa(obj, EFL_ACCESS_WINDOW_INTERFACE))
     {
        if (STATE_TYPE_GET(ss, EFL_ACCESS_STATE_ACTIVE))
          {
             // TIZEN_ONLY(20160802): do not handle events if the window is not activated
             pd->window_activated = EINA_TRUE;
             //
          }
        else
          {
             // TIZEN_ONLY(20160802): do not handle events if the window is not activated
             pd->window_activated = EINA_FALSE;
             //
          }
     }
   children = efl_access_object_access_children_get(obj);
   EINA_LIST_FREE(children, child)
     _bridge_cache_build(bridge, child);
}
//

static void
_interfaces_unregister(Eo *bridge)
{
    ELM_ATSPI_BRIDGE_DATA_GET_OR_RETURN(bridge, pd);

#define INTERFACE_SAFE_FREE(ifc) \
   if (ifc) \
      eldbus_service_interface_unregister(ifc); \
   ifc = NULL;

   INTERFACE_SAFE_FREE(pd->interfaces.accessible);
   INTERFACE_SAFE_FREE(pd->interfaces.application);
   INTERFACE_SAFE_FREE(pd->interfaces.action);
   INTERFACE_SAFE_FREE(pd->interfaces.component);
   INTERFACE_SAFE_FREE(pd->interfaces.collection);
   INTERFACE_SAFE_FREE(pd->interfaces.editable_text);
   INTERFACE_SAFE_FREE(pd->interfaces.image);
   INTERFACE_SAFE_FREE(pd->interfaces.selection);
   INTERFACE_SAFE_FREE(pd->interfaces.text);
   INTERFACE_SAFE_FREE(pd->interfaces.value);
}

static void
_a11y_connection_shutdown(Eo *bridge)
{
   ELM_ATSPI_BRIDGE_DATA_GET_OR_RETURN(bridge, pd);

   // TIZEN_ONLY(20171109) : fix for invalid proxy object, when at-spi has been restarted
   Eo *socket;
   EINA_LIST_FREE(pd->connected_plugs_sockets, socket)
      evas_object_data_set(socket, "__proxy_invalid", (void*)1);
   //

   Eldbus_Pending *pending;

   if (pd->connected)
      _elm_atspi_bridge_app_unregister(bridge);

   if (pd->cache)
     eina_hash_free(pd->cache);
   pd->cache = NULL;

   if (pd->cache_interface)
     eldbus_service_object_unregister(pd->cache_interface);
   pd->cache_interface = NULL;

   _interfaces_unregister(bridge);

   if (pd->key_flr) ecore_event_filter_del(pd->key_flr);
   pd->key_flr = NULL;

   if (pd->register_hdl) eldbus_signal_handler_del(pd->register_hdl);
   pd->register_hdl = NULL;

   if (pd->unregister_hdl) eldbus_signal_handler_del(pd->unregister_hdl);
   pd->unregister_hdl = NULL;

   //TIZEN_ONLY(20160527) - Add direct reading feature
   if (pd->reading_state_changed_hdl) eldbus_signal_handler_del(pd->reading_state_changed_hdl);
   pd->reading_state_changed_hdl = NULL;
   //

   EINA_LIST_FREE(pd->pending_requests, pending)
      eldbus_pending_cancel(pending);
   pd->pending_requests = NULL;

   if (pd->a11y_bus) eldbus_connection_unref(pd->a11y_bus);
   pd->a11y_bus = NULL;

   if (pd->state_hash) eina_hash_free(pd->state_hash);
   pd->state_hash = NULL;

   if (pd->event_hash) eina_hash_free(pd->event_hash);
   pd->event_hash = NULL;

   efl_access_object_event_handler_del(EFL_ACCESS_OBJECT_MIXIN, pd->event_hdlr);
   pd->event_hdlr = NULL;

   efl_event_callback_legacy_call(bridge, ELM_ATSPI_BRIDGE_EVENT_DISCONNECTED, NULL);
   pd->connected = EINA_FALSE;
}

static void _disconnect_cb(void *data, Eldbus_Connection *conn EINA_UNUSED, void *event_info EINA_UNUSED)
{
   _a11y_connection_shutdown(data);
}

static void
_interfaces_register(Eo *bridge)
{
   ELM_ATSPI_BRIDGE_DATA_GET_OR_RETURN(bridge, pd);

   pd->interfaces.accessible =
      eldbus_service_interface_fallback_register(pd->a11y_bus, ELM_ACCESS_OBJECT_PATH_PREFIX2, &accessible_iface_desc);
   eldbus_service_object_data_set(pd->interfaces.accessible, ELM_ATSPI_BRIDGE_CLASS_NAME, bridge);

   pd->interfaces.application =
      eldbus_service_interface_fallback_register(pd->a11y_bus, ELM_ACCESS_OBJECT_PATH_PREFIX2, &application_iface_desc);
   eldbus_service_object_data_set(pd->interfaces.application, ELM_ATSPI_BRIDGE_CLASS_NAME, bridge);

   pd->interfaces.action =
      eldbus_service_interface_fallback_register(pd->a11y_bus, ELM_ACCESS_OBJECT_PATH_PREFIX2, &action_iface_desc);
   eldbus_service_object_data_set(pd->interfaces.action, ELM_ATSPI_BRIDGE_CLASS_NAME, bridge);

   pd->interfaces.component =
      eldbus_service_interface_fallback_register(pd->a11y_bus, ELM_ACCESS_OBJECT_PATH_PREFIX2, &component_iface_desc);
   eldbus_service_object_data_set(pd->interfaces.component, ELM_ATSPI_BRIDGE_CLASS_NAME, bridge);

   pd->interfaces.collection =
      eldbus_service_interface_fallback_register(pd->a11y_bus, ELM_ACCESS_OBJECT_PATH_PREFIX2, &collection_iface_desc);
   eldbus_service_object_data_set(pd->interfaces.collection, ELM_ATSPI_BRIDGE_CLASS_NAME, bridge);

   pd->interfaces.editable_text =
      eldbus_service_interface_fallback_register(pd->a11y_bus, ELM_ACCESS_OBJECT_PATH_PREFIX2, &editable_text_iface_desc);
   eldbus_service_object_data_set(pd->interfaces.editable_text, ELM_ATSPI_BRIDGE_CLASS_NAME, bridge);

   pd->interfaces.image =
      eldbus_service_interface_fallback_register(pd->a11y_bus, ELM_ACCESS_OBJECT_PATH_PREFIX2, &image_iface_desc);
   eldbus_service_object_data_set(pd->interfaces.image, ELM_ATSPI_BRIDGE_CLASS_NAME, bridge);

   pd->interfaces.selection =
      eldbus_service_interface_fallback_register(pd->a11y_bus, ELM_ACCESS_OBJECT_PATH_PREFIX2, &selection_iface_desc);
   eldbus_service_object_data_set(pd->interfaces.selection, ELM_ATSPI_BRIDGE_CLASS_NAME, bridge);

   pd->interfaces.text =
      eldbus_service_interface_fallback_register(pd->a11y_bus, ELM_ACCESS_OBJECT_PATH_PREFIX2, &text_iface_desc);
   eldbus_service_object_data_set(pd->interfaces.text, ELM_ATSPI_BRIDGE_CLASS_NAME, bridge);

   pd->interfaces.value =
      eldbus_service_interface_fallback_register(pd->a11y_bus, ELM_ACCESS_OBJECT_PATH_PREFIX2, &value_iface_desc);
   eldbus_service_object_data_set(pd->interfaces.value, ELM_ATSPI_BRIDGE_CLASS_NAME, bridge);
}

static void
_bridge_accessible_event_dispatch(void *data, const Efl_Event *event)
{
   ELM_ATSPI_BRIDGE_DATA_GET_OR_RETURN(data, pd);

   _bridge_object_register(data, event->object);

   Efl_Event_Cb cb = eina_hash_find(pd->event_hash, &event->desc);
   return cb ? cb(data, event) : EINA_TRUE;
}

static void
_a11y_bus_initialize(Eo *obj, const char *socket_addr)
{
   ELM_ATSPI_BRIDGE_DATA_GET_OR_RETURN(obj, pd);

   pd->a11y_bus = eldbus_private_address_connection_get(socket_addr);
   if (!pd->a11y_bus)
     return;

   eldbus_connection_event_callback_add(pd->a11y_bus, ELDBUS_CONNECTION_EVENT_DISCONNECTED, _disconnect_cb, obj);

   // init data structures
   pd->cache = eina_hash_pointer_new(NULL);
   pd->state_hash = _elm_atspi_state_hash_build();
   pd->event_hash = _elm_atspi_event_hash_build();

   // dbus init
   _cache_register(obj);
   _interfaces_register(obj);
   _event_handlers_register(obj);
   /* TIZEN_ONLY(20171108): make atspi_proxy work
   _elm_atspi_bridge_app_register(obj);
   */
   //TIZEN_ONLY(20171108): make atspi_proxy work
   if (!getenv("ELM_ATSPI_NO_EMBED"))
     _elm_atspi_bridge_app_register(obj);

   // register accessible object event listener
   pd->event_hdlr = efl_access_object_event_handler_add(EFL_ACCESS_OBJECT_MIXIN, _bridge_accessible_event_dispatch, obj);

}

static void
_a11y_bus_address_get(void *data, const Eldbus_Message *msg, Eldbus_Pending *pending)
{
   const char *errname, *errmsg, *sock_addr = NULL;
   ELM_ATSPI_BRIDGE_DATA_GET_OR_RETURN(data, pd);

   pd->pending_requests = eina_list_remove(pd->pending_requests, pending);

   if (eldbus_message_error_get(msg, &errname, &errmsg))
     {
        ERR("%s %s", errname, errmsg);
        return;
     }

   if (!eldbus_message_arguments_get(msg, "s", &sock_addr) || !sock_addr)
     {
        ERR("Could not get A11Y Bus socket address.");
        return;
     }

   //TIZEN_ONLY(20171108): make atspi_proxy work
   _a11y_socket_address = eina_stringshare_add(sock_addr);
   //
   _a11y_bus_initialize((Eo*)data, sock_addr);
}

static void _a11y_connection_init(Eo *bridge)
{
   ELM_ATSPI_BRIDGE_DATA_GET_OR_RETURN(bridge, pd);
   Eina_Bool is_connected;

   is_connected = elm_obj_atspi_bridge_connected_get(bridge);

   if (is_connected) return;

   Eldbus_Message *m = eldbus_object_method_call_new(pd->bus_obj, A11Y_DBUS_INTERFACE, "GetAddress");
   Eldbus_Pending *p = eldbus_object_send(pd->bus_obj, m, _a11y_bus_address_get, bridge, 100);

   if (p)
      pd->pending_requests = eina_list_append(pd->pending_requests, p);
}

static void
_screen_reader_enabled_get(void *data, const Eldbus_Message *msg, Eldbus_Pending *pending)
{
   ELM_ATSPI_BRIDGE_DATA_GET_OR_RETURN(data, pd);
   const char *errname, *errmsg;
   Eina_Bool is_enabled;
   Eldbus_Message_Iter *variant;

   pd->pending_requests = eina_list_remove(pd->pending_requests, pending);

   if (eldbus_message_error_get(msg, &errname, &errmsg))
     {
        WRN("%s %s", errname, errmsg);
        return;
     }
   if (!eldbus_message_arguments_get(msg, "v", &variant))
     {
        ERR("'ScreenReaderEnabled' not packed into variant.");
        return;
     }
   if (!eldbus_message_iter_arguments_get(variant, "b", &is_enabled))
     {
        ERR("Could not get 'ScreenReaderEnabled' boolean property");
        return;
     }

   //TIZEN_ONLY(20161027) - Export elm_atspi_bridge_utils_is_screen_reader_enabled
   pd->screen_reader_enabled = !!is_enabled;
   //

   //TIZEN_ONLY(20160822): When atspi mode is dynamically switched on/off,
   //register/unregister access objects accordingly.
   // TIZEN_ONLY(20170516): connect to at-spi dbus based on org.a11y.Status.IsEnabled property
   _elm_win_screen_reader(is_enabled);
   //
   //
}

// TIZEN_ONLY(20170516): connect to at-spi dbus based on org.a11y.Status.IsEnabled property
static void
_at_spi_client_enabled_get(void *data, const Eldbus_Message *msg, Eldbus_Pending *pending)
{
   ELM_ATSPI_BRIDGE_DATA_GET_OR_RETURN(data, pd);
   const char *errname, *errmsg;
   Eina_Bool is_enabled;
   Eldbus_Message_Iter *variant;

   pd->pending_requests = eina_list_remove(pd->pending_requests, pending);

   if (eldbus_message_error_get(msg, &errname, &errmsg))
     {
        WRN("%s %s", errname, errmsg);
        return;
     }
   if (!eldbus_message_arguments_get(msg, "v", &variant))
     {
        ERR("'" A11Y_DBUS_ENABLED_PROPERTY "' not packed into variant.");
        return;
     }
   if (!eldbus_message_iter_arguments_get(variant, "b", &is_enabled))
     {
        ERR("Could not get '" A11Y_DBUS_ENABLED_PROPERTY "' boolean property");
        return;
     }
   if (is_enabled)
     _a11y_connection_init(data);
   else
     {
        _elm_win_atspi(is_enabled);
        DBG("AT-SPI2 stack not enabled.");
     }
}
//

static void _bridge_object_register(Eo *bridge, Eo *obj)
{
   ELM_ATSPI_BRIDGE_DATA_GET_OR_RETURN(bridge, pd);

   if (!efl_isa(obj, EFL_ACCESS_OBJECT_MIXIN))
     {
        WRN("Unable to register class w/o Efl_Access_Object!");
        return;
     }

   if (eina_hash_find(pd->cache, &obj))
        return;

   eina_hash_add(pd->cache, &obj, obj);
}

void
_elm_atspi_bridge_init(void)
{
   if (!_init_count)
     {
        _instance = efl_add_ref(ELM_ATSPI_BRIDGE_CLASS, NULL);
        _init_count = 1;
     }
}

EAPI Eo*
_elm_atspi_bridge_get(void)
{
   return _instance;
}

void
_elm_atspi_bridge_shutdown(void)
{
   if (_init_count)
     {
        efl_unref(_instance);
        _init_count = 0;
        _instance = NULL;
     }

   //TIZEN_ONLY(20171108): make atspi_proxy work
   if (_a11y_socket_address)
     eina_stringshare_del(_a11y_socket_address);
   _a11y_socket_address = NULL;
   _efl_access_shutdown();
}

static Key_Event_Info*
_key_event_info_new(int event_type, const Ecore_Event_Key *data, Eo *bridge)
{
   Key_Event_Info *ret;
   EINA_SAFETY_ON_NULL_RETURN_VAL(data, NULL);

   ret = calloc(1, sizeof(Key_Event_Info));

   ret->type = event_type;
   ret->event = *data;
   ret->bridge = bridge;

   ret->event.keyname = eina_stringshare_add(data->keyname);
   ret->event.key = eina_stringshare_add(data->key);
   ret->event.string = eina_stringshare_add(data->string);
   ret->event.compose = eina_stringshare_add(data->compose);
   ret->event.modifiers = data->modifiers;

   // not sure why it is here, but explicite keep it NULLed.
   ret->event.data = NULL;

   return ret;
}

static void
_key_event_info_free(Key_Event_Info *data)
{
   EINA_SAFETY_ON_NULL_RETURN(data);

   eina_stringshare_del(data->event.keyname);
   eina_stringshare_del(data->event.key);
   eina_stringshare_del(data->event.string);
   eina_stringshare_del(data->event.compose);

   free(data);
}

static short
_ecore_modifiers_2_atspi(unsigned int modifiers)
{
   short ret = 0;

   if (modifiers & ECORE_EVENT_MODIFIER_SHIFT)
     ret |= (1 << ATSPI_MODIFIER_SHIFT);
   if (modifiers & ECORE_EVENT_MODIFIER_CAPS)
     ret |= (1 << ATSPI_MODIFIER_SHIFTLOCK);
   if (modifiers & ECORE_EVENT_MODIFIER_CTRL)
     ret |= (1 << ATSPI_MODIFIER_CONTROL);
   if (modifiers & ECORE_EVENT_MODIFIER_ALT)
     ret |= (1 << ATSPI_MODIFIER_ALT);
   if (modifiers & ECORE_EVENT_MODIFIER_WIN)
     ret |= (1 << ATSPI_MODIFIER_META);
   if (modifiers & ECORE_EVENT_MODIFIER_NUM)
     ret |= (1 << ATSPI_MODIFIER_NUMLOCK);

   return ret;
}

static void
_iter_marshall_key_event(Eldbus_Message_Iter *iter, Key_Event_Info *data)
{
   Eldbus_Message_Iter *struct_iter;
   EINA_SAFETY_ON_NULL_RETURN(data);

   struct_iter = eldbus_message_iter_container_new(iter, 'r', NULL);

   const char *str = data->event.keyname ? data->event.keyname : "";
   int is_text = data->event.keyname ? 1 : 0;
   int type;
   if (data->type == ECORE_EVENT_KEY_DOWN)
     type = ATSPI_KEY_PRESSED_EVENT;
   else
     type = ATSPI_KEY_RELEASED_EVENT;

   eldbus_message_iter_arguments_append(struct_iter, "uinnisb", type, 0, data->event.keycode, _ecore_modifiers_2_atspi(data->event.modifiers), data->event.timestamp, str, is_text);
   eldbus_message_iter_container_close(iter, struct_iter);
}

static void
_on_event_del(void *user_data, void *func_data EINA_UNUSED)
{
   Key_Event_Info *info = user_data;
   _key_event_info_free(info);
}

static void
_on_listener_answer(void *data, const Eldbus_Message *msg, Eldbus_Pending *pending EINA_UNUSED)
{
   Key_Event_Info *info = data;
   const char *errname, *errmsg;
   Eina_Bool ret = EINA_TRUE;

   ELM_ATSPI_BRIDGE_DATA_GET_OR_RETURN(info->bridge, pd);

   if (eldbus_message_error_get(msg, &errname, &errmsg))
     {
        ERR("%s %s", errname, errmsg);
        goto reemit;
     }
   if (!eldbus_message_arguments_get(msg, "b", &ret))
     {
        ERR("Return message does not contain return value");
        goto reemit;
     }
   if (ret)
     {
        _key_event_info_free(info);
        return;
     }
reemit:
   ecore_event_add(info->type, &info->event, _on_event_del, info);
   pd->reemited_events = eina_list_append(pd->reemited_events, &info->event);
}

static Eina_Bool
_elm_atspi_bridge_key_filter(void *data, void *loop EINA_UNUSED, int type, void *event)
{
   Eldbus_Message *msg;
   Eldbus_Message_Iter *iter;
   Ecore_Event_Key *key_event = event;
   Key_Event_Info *ke;
   Eo *bridge = data;

   ELM_ATSPI_BRIDGE_DATA_GET_OR_RETURN_VAL(bridge, pd, EINA_TRUE);

   // TIZEN_ONLY(20160802): do not handle events if the window is not activated
   if (!pd->window_activated) return EINA_TRUE;
   //

   if ((type != ECORE_EVENT_KEY_DOWN) && (type != ECORE_EVENT_KEY_UP)) return EINA_TRUE;

   // check if reemited
   if (eina_list_data_find(pd->reemited_events, event))
     {
        pd->reemited_events = eina_list_remove(pd->reemited_events, event);
        return EINA_TRUE;
     }
   // TIZEN_ONLY(20170118): Not handle events if keyboard is on
   if (pd->root)
     {
        Eina_List *children, *l;
        Evas_Object *child;
        children = efl_access_object_access_children_get(pd->root);

        EINA_LIST_FOREACH(children, l, child)
          {
             if (efl_ui_focus_object_focus_get(child)) break;
          }
        eina_list_free(children);

        Elm_Win_Keyboard_Mode mode;
        mode = elm_win_keyboard_mode_get(child);
        if (mode == ELM_WIN_KEYBOARD_ON) return EINA_TRUE;
     }
   //

   ke = _key_event_info_new(type, key_event, bridge);
   if (!ke) return EINA_TRUE;

   msg = eldbus_message_method_call_new(ATSPI_DBUS_NAME_REGISTRY, ATSPI_DBUS_PATH_DEC,
                                        ATSPI_DBUS_INTERFACE_DEC, "NotifyListenersSync");
   iter = eldbus_message_iter_get(msg);
   _iter_marshall_key_event(iter, ke);

   // timeout should be kept reasonably low to avoid delays
   if (!eldbus_connection_send(pd->a11y_bus, msg, _on_listener_answer, ke, 100))
     return EINA_TRUE;

   return EINA_FALSE;
}

EOLIAN Eina_Bool
_elm_atspi_bridge_connected_get(const Eo *obj EINA_UNUSED, Elm_Atspi_Bridge_Data *pd)
{
   return pd->connected;
}

//TIZEN_ONLY(20171108): make atspi_proxy work
EOLIAN Eo*
_elm_atspi_bridge_root_get(const Eo *obj EINA_UNUSED, Elm_Atspi_Bridge_Data *pd)
{
   if (!pd->root)
     {
        pd->root = efl_add(ELM_ATSPI_APP_OBJECT_CLASS, NULL);
        efl_access_added(pd->root);
     }

   return pd->root;
}
//

static void
_properties_changed_cb(void *data, Eldbus_Proxy *proxy EINA_UNUSED, void *event)
{
   Eldbus_Proxy_Event_Property_Changed *ev = event;
   Eo *bridge = data;
   Eina_Bool val;
   const char *ifc = eldbus_proxy_interface_get(ev->proxy);
   if (ev->name && !strcmp(ev->name, "ScreenReaderEnabled" ) &&
       ifc && !strcmp(A11Y_DBUS_STATUS_INTERFACE, ifc))
     {
        if (!eina_value_get(ev->value, &val))
          {
             ERR("Unable to get ScreenReaderEnabled property value");
             return;
          }

        //TIZEN_ONLY(20161027) - Export elm_atspi_bridge_utils_is_screen_reader_enabled
        ELM_ATSPI_BRIDGE_DATA_GET_OR_RETURN(bridge, pd);
        pd->screen_reader_enabled = !!val;
        //

        //TIZEN_ONLY(20160822): When atspi mode is dynamically switched on/off,
         //register/unregister access objects accordingly.
        // TIZEN_ONLY(20170516): connect to at-spi dbus based on org.a11y.Status.IsEnabled property
        _elm_win_screen_reader(val);
        //
        //
     }
   // TIZEN_ONLY(20170516): connect to at-spi dbus based on org.a11y.Status.IsEnabled property
   if (ev->name && !strcmp(ev->name, A11Y_DBUS_ENABLED_PROPERTY) &&
       ifc && !strcmp(A11Y_DBUS_STATUS_INTERFACE, ifc))
     {
        if (!eina_value_get(ev->value, &val))
          {
             // TIZEN_ONLY(20170516): connect to at-spi dbus based on org.a11y.Status.IsEnabled property
             ERR("Unable to get " A11Y_DBUS_ENABLED_PROPERTY " property value");
             return;
          }

        _elm_win_atspi(val);

        if (val)
          _a11y_connection_init(bridge);
        else
          {
             _elm_win_atspi(EINA_FALSE);
             _a11y_connection_shutdown(bridge);
          }
     }
   //
}

EOLIAN Efl_Object*
_elm_atspi_bridge_efl_object_constructor(Eo *obj, Elm_Atspi_Bridge_Data *pd)
{
   Eldbus_Proxy *proxy;
   Eldbus_Pending *req;

   efl_constructor(efl_super(obj, ELM_ATSPI_BRIDGE_CLASS));

   elm_need_eldbus();

   if (!(pd->session_bus = eldbus_connection_get(ELDBUS_CONNECTION_TYPE_SESSION)))
     {
        ERR("Unable to connect to Session Bus");
        return NULL;
     }
   if (!(pd->bus_obj = eldbus_object_get(pd->session_bus, A11Y_DBUS_NAME, A11Y_DBUS_PATH)))
     {
        ERR("Could not get /org/a11y/bus object");
        goto obj_err;
     }
   if (!(proxy = eldbus_proxy_get(pd->bus_obj, A11Y_DBUS_STATUS_INTERFACE)))
     {
        ERR("Could not get proxy object for %s interface", A11Y_DBUS_STATUS_INTERFACE);
        goto proxy_err;
     }
   if (!(req = eldbus_proxy_property_get(proxy, "ScreenReaderEnabled", _screen_reader_enabled_get, obj)))
     {
        ERR("Could not send PropertyGet request");
        goto proxy_err;
     }
   pd->pending_requests = eina_list_append(pd->pending_requests, req);
   // TIZEN_ONLY(20170516): connect to at-spi dbus based on org.a11y.Status.IsEnabled property
   if (!(req = eldbus_proxy_property_get(proxy, A11Y_DBUS_ENABLED_PROPERTY, _at_spi_client_enabled_get, obj)))
     {
        ERR("Could not send PropertyGet request");
        goto proxy_err;
     }

   pd->pending_requests = eina_list_append(pd->pending_requests, req);
   //

   eldbus_proxy_properties_monitor(proxy, EINA_TRUE);
   eldbus_proxy_event_callback_add(proxy, ELDBUS_PROXY_EVENT_PROPERTY_CHANGED,
                                   _properties_changed_cb, obj);

   return obj;

proxy_err:
   eldbus_object_unref(pd->bus_obj);
   pd->bus_obj = NULL;
obj_err:
   eldbus_connection_unref(pd->session_bus);
   pd->session_bus = NULL;
   return NULL;
}

static Eina_Bool _from_list_remove(void *data, Eo *obj, const Efl_Event_Description *desc EINA_UNUSED, void *event_info EINA_UNUSED)
{
   Eina_List **list = data;
   *list = eina_list_remove(*list, obj);
   return EINA_TRUE;
}

EOLIAN void
_elm_atspi_bridge_efl_object_destructor(Eo *obj, Elm_Atspi_Bridge_Data *pd)
{
   _a11y_connection_shutdown(obj);

   Eo *socket_elem = NULL;
   EINA_LIST_FREE(pd->connected_plugs_sockets, socket_elem)
     {
       efl_event_callback_del(socket_elem, EFL_EVENT_DEL, _from_list_remove, &pd->connected_plugs_sockets);
     }

   if (pd->bus_obj) eldbus_object_unref(pd->bus_obj);
   if (pd->session_bus) eldbus_connection_unref(pd->session_bus);

   efl_destructor(efl_super(obj, ELM_ATSPI_BRIDGE_CLASS));
}

//TIZEN_ONLY(20171108): make atspi_proxy work
EAPI Eina_Bool
elm_atspi_bridge_object_address_get(Eo *obj, char **bus, char **path)
{
   Eo *bridge = _elm_atspi_bridge_get();
   if (!bridge)
     {
        ERR("Connection with accessibility bus not established.");
        return EINA_FALSE;
     }
   ELM_ATSPI_BRIDGE_DATA_GET_OR_RETURN_VAL(bridge, pd, EINA_FALSE);
   if (!efl_isa(obj, EFL_ACCESS_OBJECT_MIXIN))
     {
        ERR("Connection with accessibility bus not established.");
        return EINA_FALSE;
     }
   if (bus) *bus = strdup(eldbus_connection_unique_name_get(pd->a11y_bus));
   if (path) *path = strdup(_path_from_object(obj));

   return EINA_TRUE;
}

static Eina_Bool
_proxy_property_get(const Eldbus_Service_Interface *interface, const char *property,
                         Eldbus_Message_Iter *iter, const Eldbus_Message *request_msg EINA_UNUSED,
                         Eldbus_Message **error EINA_UNUSED)
{
   char *bus, *path;
   Eo *obj = eldbus_service_object_data_get(interface, "_atspi_obj");
   EINA_SAFETY_ON_NULL_RETURN_VAL(obj, EINA_FALSE);

   if (!strcmp(property, "Object"))
     {
       Eo *parent;
       parent = efl_parent_get(obj);
       if (!elm_atspi_bridge_object_address_get(parent, &bus, &path))
          return EINA_FALSE;

       Eldbus_Message_Iter *iter_struct = eldbus_message_iter_container_new(iter, 'r', NULL);
       if (iter_struct)
         {
            eldbus_message_iter_basic_append(iter_struct, 's', bus);
            eldbus_message_iter_basic_append(iter_struct, 'o', path);
            eldbus_message_iter_container_close(iter, iter_struct);
         }
       free(bus);
       free(path);
       return EINA_TRUE;
     }
   return EINA_FALSE;
}

static const Eldbus_Property proxy_properties[] = {
   { "Object", "(so)", _proxy_property_get, NULL, 0 },
   { NULL, NULL, NULL, NULL, 0 }
};

static const Eldbus_Service_Interface_Desc _proxy_iface_desc = {
   ELM_ATSPI_DBUS_INTERFACE_PROXY, socket_methods, NULL, proxy_properties, NULL, NULL
};

static void _embedded_reply_cb(void *data, const Eldbus_Message *msg, Eldbus_Pending *pending EINA_UNUSED)
{
   Eo *parent, *proxy = data;
   const char *err, *txt;

   if (eldbus_message_error_get(msg, &err, &txt))
     {
        ERR("AT-SPI: Embedded method call failed: %s %s", err, txt);
        efl_event_callback_call(proxy, ELM_ATSPI_PROXY_EVENT_DISCONNECTED, NULL);
        return;
     }
   efl_event_callback_call(proxy, ELM_ATSPI_PROXY_EVENT_CONNECTED, NULL);

   parent = efl_parent_get(proxy);
   if (parent)
     efl_access_children_changed_added_signal_emit(parent, proxy);
}

static void
_plug_embedded_send(Eldbus_Connection *conn, Eo *proxy, const char *bus, const char *path)
{
   char *obj_path = NULL;
   Eo *parent;
   Eldbus_Message *msg = NULL;

   parent = efl_parent_get(proxy);
   if (!parent) goto fail;

   msg = eldbus_message_method_call_new(bus, path, ATSPI_DBUS_INTERFACE_SOCKET, "Embedded");
   if (!msg) goto fail;

   if (!elm_atspi_bridge_object_address_get(parent, NULL, &obj_path))
     goto fail;

   if (!eldbus_message_arguments_append(msg, "s", obj_path))
     goto fail;

   //TIZEN_ONLY(20171114) elm_atspi_bridge: fix double free issue (WGID:81393)
   // free(obj_path);
   ELM_SAFE_FREE(obj_path, free);
   //

   if (!eldbus_connection_send(conn, msg, _embedded_reply_cb, proxy, 100))
     goto fail;

   return;

fail:
   ERR("AT-SPI: Unable to send Embedded request.");
   if (msg) eldbus_message_unref(msg);
   //TIZEN_ONLY(20171114) elm_atspi_bridge: fix double free issue (WGID:81393)
   // free(obj_path);
   ELM_SAFE_FREE(obj_path, free);
   //
   efl_event_callback_call(proxy, ELM_ATSPI_PROXY_EVENT_DISCONNECTED, NULL);
}

static void _socket_addr_get_cb(void *data, const Eldbus_Message *msg, Eldbus_Pending *pending EINA_UNUSED)
{
   Eo *proxy = data;
   const char *bus, *path, *err, *txt;
   Eldbus_Message_Iter *iter, *iter_variant, *iter_struct;

   //TIZEN_ONLY(20171114) elm_atspi_bridge: ELM_ATSPI_BRIDGE_DATA_GET_OR_RETURN should use elm_atspi_bridge object
   // ELM_ATSPI_BRIDGE_DATA_GET_OR_RETURN(proxy, pd);
   Eo *bridge = _elm_atspi_bridge_get();
   if (!bridge)
     {
        ERR("AT-SPI: Atspi bridge is not enabled.");
        return;
     }
   ELM_ATSPI_BRIDGE_DATA_GET_OR_RETURN(bridge, pd);
   //

   if (eldbus_message_error_get(msg, &err, &txt))
     {
        ERR("Unable to connect to socket: %s %s", err, txt);
        goto retry;
     }

   iter = eldbus_message_iter_get(msg);
   if (!eldbus_message_iter_arguments_get(iter, "v", &iter_variant))
     {
        ERR("Unable to get variant parameter");
        goto fail;
     }

   if (!eldbus_message_iter_arguments_get(iter_variant, "(so)", &iter_struct))
     {
        ERR("Unable to get so parameters");
        goto fail;
     }

   if (!eldbus_message_iter_arguments_get(iter_struct, "so", &bus, &path))
     {
        ERR("Unable to get so parameters");
        goto fail;
     }

   elm_obj_atspi_proxy_address_set(proxy, bus, path);

   _plug_embedded_send(pd->a11y_bus, proxy, bus, path);

   return;

fail:
   efl_event_callback_call(proxy, ELM_ATSPI_PROXY_EVENT_DISCONNECTED, NULL);
   return;

retry:
   elm_obj_atspi_proxy_address_get_retry_timer_add(proxy);
}

static void
_plug_address_discover(Eldbus_Connection *conn, Eo *proxy, const char *svc_bus, const char *svc_path)
{
   Eldbus_Object *dobj;
   dobj = eldbus_object_get(conn, svc_bus, svc_path);

   //TIZEN_ONLY(20171114) atspi: sanitize service name before creating bus
   if (!dobj)
     {
       ERR("Unable to get eldbus object from: %s %s", svc_bus, svc_path);
       return;
     }
   //

   Eldbus_Message *msg = eldbus_object_method_call_new(dobj, ELDBUS_FDO_INTERFACE_PROPERTIES, "Get");
   eldbus_message_arguments_append(msg, "ss", ELM_ATSPI_DBUS_INTERFACE_PROXY, "Object");
   eldbus_object_send(dobj, msg, _socket_addr_get_cb, proxy, 100);
}

static void _add_plug_or_socket_to_connected_list_in_bridge(Eo *plug_or_socket)
{
   Eo *bridge = _elm_atspi_bridge_get();
   ELM_ATSPI_BRIDGE_DATA_GET_OR_RETURN(bridge, pd);
   pd->connected_plugs_sockets = eina_list_append(pd->connected_plugs_sockets, plug_or_socket);
   efl_event_callback_add(plug_or_socket, EFL_EVENT_DEL, _from_list_remove, &pd->connected_plugs_sockets);
}

static void _plug_connect(Eldbus_Connection *conn, Eo *proxy)
{
   const char *bus, *path;

   bus = efl_key_data_get(proxy, "__svc_bus");
   path = efl_key_data_get(proxy, "__svc_path");

   _add_plug_or_socket_to_connected_list_in_bridge(proxy);

   if (bus && path)
     {
        _plug_address_discover(conn, proxy, bus, path);
        return;
     }
   else
     {
        elm_obj_atspi_proxy_address_get(proxy, &bus, &path);
        if (!bus || !path)
          {
             ERR("AT-SPI: Elm_Atspi_Proxy bus or path not set. Unable to connect");
             efl_event_callback_call(proxy, ELM_ATSPI_PROXY_EVENT_DISCONNECTED, NULL);
             return;
          }
        _plug_embedded_send(conn, proxy, bus, path);
     }
   return;
}

EAPI void elm_atspi_bridge_utils_proxy_connect(Eo *proxy)
{
   Eo *bridge = _elm_atspi_bridge_get();
   if (!bridge)
     {
        ERR("AT-SPI: Atspi bridge is not enabled.");
        efl_event_callback_call(proxy, ELM_ATSPI_PROXY_EVENT_DISCONNECTED, NULL);
        return;
     }
   ELM_ATSPI_BRIDGE_DATA_GET_OR_RETURN(bridge, pd);

   if (!pd->a11y_bus)
     {
        if (!eina_list_data_find(pd->plug_queue, proxy))
          {
             pd->plug_queue = eina_list_append(pd->plug_queue, proxy);
             efl_event_callback_add(proxy, EFL_EVENT_DEL, _from_list_remove, &pd->plug_queue);
          }
        return;
     }
   _plug_connect(pd->a11y_bus, proxy);
}

//TIZEN_ONLY(20180810) atspi: check service, bus and path names for Dbus communication
/**
 * @brief In according to following specification:
 * http://dbus.freedesktop.org/doc/dbus-specification.html#message-protocol-names
 * http://dbus.freedesktop.org/doc/dbus-specification.html#message-protocol-marshaling-object-path
 */
static Eina_Bool _check_service_name(const char *str)
{
   if (!str)
      return NULL;

   char *tmp = str;
   while (*tmp)
      {
         if (!isalnum(*tmp) && *tmp != '_' && *tmp != '-' && *tmp != '.')
           {
              ERR("Invalid service name: %s, following character is forbidden in Dbus specification: %c", str, *tmp);
              return EINA_FALSE;
           }
         ++tmp;
      }
   return EINA_TRUE;
}

static char *_sanitize_bus_name(const char *str)
{
   char *res = strdup(str);
   char *tmp = res;

   if (res[0] == ':')
     {
         while (*tmp)
          {
            if (*tmp == '.' && isdigit(*(tmp + 1)))
            *tmp = '_';
            ++tmp;
          }
     }

   return res;
}

static char *_sanitize_path_name(const char *str)
{
   char *res = strdup(str);
   char *tmp = res;

   while (*tmp)
     {
        if (*tmp == '-' || *tmp == '.')
           *tmp = '_';
        ++tmp;
     }

   return res;
}
//

Eo* _elm_atspi_bridge_utils_proxy_create(Eo *parent, const char *svcname, int svcnum, Elm_Atspi_Proxy_Type type)
{
   Eo *ret;
   //TIZEN_ONLY(20180810) atspi: check service, bus and path names for Dbus communication
   char bus[256], path[256];
   int res;

   Eina_Bool c = _check_service_name(svcname);
   if (!c) return NULL;

   res = snprintf(bus, sizeof(bus), "elm.atspi.proxy.socket-%s-%d", svcname, svcnum);
   if (res < 0 || (res >= (int)sizeof(bus)))
     {
         ERR("Error occured during creating bus name");
         return NULL;
     }

   res = snprintf(path, sizeof(path), "/elm/atspi/proxy/socket/%s/%d", svcname, svcnum);
   if (res < 0 || (res >= (int)sizeof(path)))
     {
         ERR("Error occured during creating bus name");
         return NULL;
     }

   ret = efl_add(ELM_ATSPI_PROXY_CLASS, parent, elm_obj_atspi_proxy_ctor(efl_added, type));
   if (!ret) return NULL;

   char *bus_sanitized = _sanitize_bus_name(bus);
   char *path_sanitized = _sanitize_path_name(path);
   //

   efl_key_data_set(ret, "__svc_bus", eina_stringshare_add(bus_sanitized));
   efl_key_data_set(ret, "__svc_path", eina_stringshare_add(path_sanitized));

   free(bus_sanitized);
   free(path_sanitized);

   return ret;
}

static void
_on_socket_del(void *data, const Efl_Event *event)
{
   Eldbus_Service_Interface *ifc = data;
   const char *bus;
   Eldbus_Connection *conn = eldbus_service_connection_get(ifc);
   bus = efl_key_data_get(event->object, "__svc_bus");
   eldbus_name_release(conn, bus, NULL, NULL);
   eldbus_service_interface_unregister(ifc);
}

static void
_proxy_interface_register(Eldbus_Connection *conn, Eo *proxy, const char *bus, const char *path)
{
   Eldbus_Service_Interface *proxy_infc;
   //TIZEN_ONLY(20171114) elm_atspi_bridge: ELM_ATSPI_BRIDGE_DATA_GET_OR_RETURN should use elm_atspi_bridge object
   // ELM_ATSPI_BRIDGE_DATA_GET_OR_RETURN(proxy, pd);
   Eo *bridge = _elm_atspi_bridge_get();
   if (!bridge)
     {
        ERR("AT-SPI: Atspi bridge is not enabled.");
        return;
     }
   ELM_ATSPI_BRIDGE_DATA_GET_OR_RETURN(bridge, pd);
   //
   eldbus_name_request(conn, bus, ELDBUS_NAME_REQUEST_FLAG_DO_NOT_QUEUE, NULL, NULL);
   proxy_infc = eldbus_service_interface_register(pd->a11y_bus, path, &_proxy_iface_desc);
   if (!proxy_infc)
     ERR("AT-SPI: Proxy interface registration failed");
   efl_event_callback_add(proxy, EFL_EVENT_DEL, _on_socket_del, proxy_infc);
   eldbus_service_object_data_set(proxy_infc, "_atspi_obj", proxy);
}

static void _socket_ifc_create(Eldbus_Connection *conn, Eo *proxy)
{
   const char *bus, *path;
   // TIZEN_ONLY(20160705) - enable atspi_proxy to work
   Eo *bridge = _elm_atspi_bridge_get();
   ELM_ATSPI_BRIDGE_DATA_GET_OR_RETURN(bridge, pd);
   //

   bus = efl_key_data_get(proxy, "__svc_bus");
   path = efl_key_data_get(proxy, "__svc_path");

   if (bus && path)
     _proxy_interface_register(conn, proxy, bus, path);

   // TIZEN_ONLY(20160705) - enable atspi_proxy to work
   pd->interfaces.socket =
      eldbus_service_interface_fallback_register(pd->a11y_bus, ELM_ACCESS_OBJECT_PATH_PREFIX2, &socket_iface_desc);
   //
   _add_plug_or_socket_to_connected_list_in_bridge(proxy);
}

EAPI void elm_atspi_bridge_utils_proxy_listen(Eo *proxy)
{
   Eo *bridge = _elm_atspi_bridge_get();
   if (!bridge)
     {
        ERR("AT-SPI: Atspi bridge is not enabled.");
        return;
     }
   ELM_ATSPI_BRIDGE_DATA_GET_OR_RETURN(bridge, pd);
   if (!pd->a11y_bus)
     {
        if (!eina_list_data_find(pd->socket_queue, proxy))
          {
             pd->socket_queue = eina_list_append(pd->socket_queue, proxy);
             efl_event_callback_add(proxy, EFL_EVENT_DEL, _from_list_remove, &pd->socket_queue);
          }
        return;
     }
   _socket_ifc_create(pd->a11y_bus, proxy);
}
//

// TIZEN_ONLY(20160705) - enable atspi_proxy to work
static void
_offset_set_reply_cb(void *data EINA_UNUSED, const Eldbus_Message *msg, Eldbus_Pending *pending EINA_UNUSED)
{
   const char *err, *txt;

   if (eldbus_message_error_get(msg, &err, &txt))
     {
        ERR("AT-SPI: SetOffset method call failed: %s %s", err, txt);
        return;
     }
}

// TIZEN_ONLY(20160705) - enable atspi_proxy to work
void elm_atspi_bridge_utils_proxy_offset_set(Eo *proxy, int x, int y)
{
   const char *bus, *path;
   Eo *bridge = _elm_atspi_bridge_get();
   if (!bridge) return;

   ELM_ATSPI_BRIDGE_DATA_GET_OR_RETURN(bridge, pd);

   if (!pd->a11y_bus) return;

   bus = efl_key_data_get(proxy, "__svc_bus");
   path = efl_key_data_get(proxy, "__svc_path");

   Eldbus_Message *msg = NULL;

   msg = eldbus_message_method_call_new(bus, path, ELM_ATSPI_DBUS_INTERFACE_PROXY, "SetOffset");
   if (!msg) goto fail;

   if (!eldbus_message_arguments_append(msg, "i", x))
     goto fail;

   if (!eldbus_message_arguments_append(msg, "i", y))
     goto fail;

   if (!eldbus_connection_send(pd->a11y_bus, msg, _offset_set_reply_cb, NULL, 100))
     goto fail;

   return;

fail:
   ERR("AT-SPI: Unable to send SetOffset request.");
   if (msg) eldbus_message_unref(msg);
}
//

//TIZEN_ONLY(20160527) - Add direct reading feature
static void
_on_read_command_call(void *data, const Eldbus_Message *msg, Eldbus_Pending *pending EINA_UNUSED)
{
   const char *errname, *errmsg;
   const char *s;
   const Eina_Bool b;
   const int32_t i;
   Elm_Atspi_Say_Info *say_info = data;

   if (eldbus_message_error_get(msg, &errname, &errmsg))
     {
        ERR("%s %s", errname, errmsg);
        return;
     }

   if (say_info)
     {
        // get read command id and map it to obj
        if (eldbus_message_arguments_get(msg, "sbi", &s, &b, &i))
          {
             if (!read_command_id)
               read_command_id = eina_hash_int32_new(NULL);

             if (!read_command_id) {
               ERR("eina_hash_int32_new() failed to create new map to store callbacks for direct reading commands");
               return;
             }

             eina_hash_add(read_command_id, &i, say_info);
          }
     }
}

EAPI void
elm_atspi_bridge_utils_say(const char* text,
                           Eina_Bool discardable,
                           const Elm_Atspi_Say_Signal_Cb func,
                           const void *data)
{
   Eldbus_Message *msg;
   Eldbus_Message_Iter *iter;
   Elm_Atspi_Say_Info *say_info = NULL;
   Eo *bridge = _elm_atspi_bridge_get();
   if (!bridge)
     {
        ERR("AT-SPI: Atspi bridge is not enabled.");
        return;
     }
   ELM_ATSPI_BRIDGE_DATA_GET_OR_RETURN(bridge, pd);
   if (!pd->a11y_bus)
     {
        ERR("AT-SPI: a11y bus is not set.");
        return;
     }

   msg = eldbus_message_method_call_new(ELM_ATSPI_DIRECT_READ_BUS,
                                        ELM_ATSPI_DIRECT_READ_PATH,
                                        ELM_ATSPI_DIRECT_READ_INTERFACE,
                                        "ReadCommand");
   iter = eldbus_message_iter_get(msg);
   eldbus_message_iter_arguments_append(iter, "sb", text, discardable);
   if (func) {
      say_info = calloc(1, sizeof(Elm_Atspi_Say_Info));
      if (say_info)
        {
           say_info->func = func;
           say_info->data = (void *)data;
        }
   }
   eldbus_connection_send(pd->a11y_bus, msg, _on_read_command_call, say_info, -1);
}
//

//TIZEN_ONLY(20161027) - Export elm_atspi_bridge_utils_is_screen_reader_enabled
EAPI Eina_Bool elm_atspi_bridge_utils_is_screen_reader_enabled(void)
{
   Eo *bridge = _elm_atspi_bridge_get();
   if (!bridge)
     {
        ERR("AT-SPI: Atspi bridge is not enabled.");
        return EINA_FALSE;
     }
   ELM_ATSPI_BRIDGE_DATA_GET_OR_RETURN_VAL(bridge, pd, EINA_FALSE);
   return pd->screen_reader_enabled;
}
//

#include "elm_atspi_bridge.eo.c"
