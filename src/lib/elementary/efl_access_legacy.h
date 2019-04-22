/**
 * @defgroup Elm_Interface_Atspi_Accessible Atspi Accessible
 * @ingroup elm_accessibility_group
 * @{
 */
#include "efl_access_object_eo.legacy.h"

typedef Eo Elm_Interface_Atspi_Accessible;

enum _Elm_Atspi_Role
{
    ELM_ATSPI_ROLE_INVALID,
    ELM_ATSPI_ROLE_ACCELERATOR_LABEL,
    ELM_ATSPI_ROLE_ALERT,
    ELM_ATSPI_ROLE_ANIMATION,
    ELM_ATSPI_ROLE_ARROW,
    ELM_ATSPI_ROLE_CALENDAR,
    ELM_ATSPI_ROLE_CANVAS,
    ELM_ATSPI_ROLE_CHECK_BOX,
    ELM_ATSPI_ROLE_CHECK_MENU_ITEM,
    ELM_ATSPI_ROLE_COLOR_CHOOSER,
    ELM_ATSPI_ROLE_COLUMN_HEADER,
    ELM_ATSPI_ROLE_COMBO_BOX,
    ELM_ATSPI_ROLE_DATE_EDITOR,
    ELM_ATSPI_ROLE_DESKTOP_ICON,
    ELM_ATSPI_ROLE_DESKTOP_FRAME,
    ELM_ATSPI_ROLE_DIAL,
    ELM_ATSPI_ROLE_DIALOG,
    ELM_ATSPI_ROLE_DIRECTORY_PANE,
    ELM_ATSPI_ROLE_DRAWING_AREA,
    ELM_ATSPI_ROLE_FILE_CHOOSER,
    ELM_ATSPI_ROLE_FILLER,
    ELM_ATSPI_ROLE_FOCUS_TRAVERSABLE,
    ELM_ATSPI_ROLE_FONT_CHOOSER,
    ELM_ATSPI_ROLE_FRAME,
    ELM_ATSPI_ROLE_GLASS_PANE,
    ELM_ATSPI_ROLE_HTML_CONTAINER,
    ELM_ATSPI_ROLE_ICON,
    ELM_ATSPI_ROLE_IMAGE,
    ELM_ATSPI_ROLE_INTERNAL_FRAME,
    ELM_ATSPI_ROLE_LABEL,
    ELM_ATSPI_ROLE_LAYERED_PANE,
    ELM_ATSPI_ROLE_LIST,
    ELM_ATSPI_ROLE_LIST_ITEM,
    ELM_ATSPI_ROLE_MENU,
    ELM_ATSPI_ROLE_MENU_BAR,
    ELM_ATSPI_ROLE_MENU_ITEM,
    ELM_ATSPI_ROLE_OPTION_PANE,
    ELM_ATSPI_ROLE_PAGE_TAB,
    ELM_ATSPI_ROLE_PAGE_TAB_LIST,
    ELM_ATSPI_ROLE_PANEL,
    ELM_ATSPI_ROLE_PASSWORD_TEXT,
    ELM_ATSPI_ROLE_POPUP_MENU,
    ELM_ATSPI_ROLE_PROGRESS_BAR,
    ELM_ATSPI_ROLE_PUSH_BUTTON,
    ELM_ATSPI_ROLE_RADIO_BUTTON,
    ELM_ATSPI_ROLE_RADIO_MENU_ITEM,
    ELM_ATSPI_ROLE_ROOT_PANE,
    ELM_ATSPI_ROLE_ROW_HEADER,
    ELM_ATSPI_ROLE_SCROLL_BAR,
    ELM_ATSPI_ROLE_SCROLL_PANE,
    ELM_ATSPI_ROLE_SEPARATOR,
    ELM_ATSPI_ROLE_SLIDER,
    ELM_ATSPI_ROLE_SPIN_BUTTON,
    ELM_ATSPI_ROLE_SPLIT_PANE,
    ELM_ATSPI_ROLE_STATUS_BAR,
    ELM_ATSPI_ROLE_TABLE,
    ELM_ATSPI_ROLE_TABLE_CELL,
    ELM_ATSPI_ROLE_TABLE_COLUMN_HEADER,
    ELM_ATSPI_ROLE_TABLE_ROW_HEADER,
    ELM_ATSPI_ROLE_TEAROFF_MENU_ITEM,
    ELM_ATSPI_ROLE_TERMINAL,
    ELM_ATSPI_ROLE_TEXT,
    ELM_ATSPI_ROLE_TOGGLE_BUTTON,
    ELM_ATSPI_ROLE_TOOL_BAR,
    ELM_ATSPI_ROLE_TOOL_TIP,
    ELM_ATSPI_ROLE_TREE,
    ELM_ATSPI_ROLE_TREE_TABLE,
    ELM_ATSPI_ROLE_UNKNOWN,
    ELM_ATSPI_ROLE_VIEWPORT,
    ELM_ATSPI_ROLE_WINDOW,
    ELM_ATSPI_ROLE_EXTENDED,
    ELM_ATSPI_ROLE_HEADER,
    ELM_ATSPI_ROLE_FOOTER,
    ELM_ATSPI_ROLE_PARAGRAPH,
    ELM_ATSPI_ROLE_RULER,
    ELM_ATSPI_ROLE_APPLICATION,
    ELM_ATSPI_ROLE_AUTOCOMPLETE,
    ELM_ATSPI_ROLE_EDITBAR,
    ELM_ATSPI_ROLE_EMBEDDED,
    ELM_ATSPI_ROLE_ENTRY,
    ELM_ATSPI_ROLE_CHART,
    ELM_ATSPI_ROLE_CAPTION,
    ELM_ATSPI_ROLE_DOCUMENT_FRAME,
    ELM_ATSPI_ROLE_HEADING,
    ELM_ATSPI_ROLE_PAGE,
    ELM_ATSPI_ROLE_SECTION,
    ELM_ATSPI_ROLE_REDUNDANT_OBJECT,
    ELM_ATSPI_ROLE_FORM,
    ELM_ATSPI_ROLE_LINK,
    ELM_ATSPI_ROLE_INPUT_METHOD_WINDOW,
    ELM_ATSPI_ROLE_TABLE_ROW,
    ELM_ATSPI_ROLE_TREE_ITEM,
    ELM_ATSPI_ROLE_DOCUMENT_SPREADSHEET,
    ELM_ATSPI_ROLE_DOCUMENT_PRESENTATION,
    ELM_ATSPI_ROLE_DOCUMENT_TEXT,
    ELM_ATSPI_ROLE_DOCUMENT_WEB,
    ELM_ATSPI_ROLE_DOCUMENT_EMAIL,
    ELM_ATSPI_ROLE_COMMENT,
    ELM_ATSPI_ROLE_LIST_BOX,
    ELM_ATSPI_ROLE_GROUPING,
    ELM_ATSPI_ROLE_IMAGE_MAP,
    ELM_ATSPI_ROLE_NOTIFICATION,
    ELM_ATSPI_ROLE_INFO_BAR,
    ELM_ATSPI_ROLE_LAST_DEFINED,
};
/**
 * Describes the role of an object visible to assistive technology client.
 * @if MOBILE @since_tizen 3.0 @elseif WEARABLE @since_tizen 3.0 @endif
 */
typedef enum _Elm_Atspi_Role Elm_Atspi_Role;

enum _Elm_Atspi_State_Type
{
    ELM_ATSPI_STATE_INVALID,
    ELM_ATSPI_STATE_ACTIVE,
    ELM_ATSPI_STATE_ARMED,
    ELM_ATSPI_STATE_BUSY,
    ELM_ATSPI_STATE_CHECKED,
    ELM_ATSPI_STATE_COLLAPSED,
    ELM_ATSPI_STATE_DEFUNCT,
    ELM_ATSPI_STATE_EDITABLE,
    ELM_ATSPI_STATE_ENABLED,
    ELM_ATSPI_STATE_EXPANDABLE,
    ELM_ATSPI_STATE_EXPANDED,
    ELM_ATSPI_STATE_FOCUSABLE,
    ELM_ATSPI_STATE_FOCUSED,
    ELM_ATSPI_STATE_HAS_TOOLTIP,
    ELM_ATSPI_STATE_HORIZONTAL,
    ELM_ATSPI_STATE_ICONIFIED,
    ELM_ATSPI_STATE_MODAL,
    ELM_ATSPI_STATE_MULTI_LINE,
    ELM_ATSPI_STATE_MULTISELECTABLE,
    ELM_ATSPI_STATE_OPAQUE,
    ELM_ATSPI_STATE_PRESSED,
    ELM_ATSPI_STATE_RESIZABLE,
    ELM_ATSPI_STATE_SELECTABLE,
    ELM_ATSPI_STATE_SELECTED,
    ELM_ATSPI_STATE_SENSITIVE,
    ELM_ATSPI_STATE_SHOWING,
    ELM_ATSPI_STATE_SINGLE_LINE,
    ELM_ATSPI_STATE_STALE,
    ELM_ATSPI_STATE_TRANSIENT,
    ELM_ATSPI_STATE_VERTICAL,
    ELM_ATSPI_STATE_VISIBLE,
    ELM_ATSPI_STATE_MANAGES_DESCENDANTS,
    ELM_ATSPI_STATE_INDETERMINATE,
    ELM_ATSPI_STATE_REQUIRED,
    ELM_ATSPI_STATE_TRUNCATED,
    ELM_ATSPI_STATE_ANIMATED,
    ELM_ATSPI_STATE_INVALID_ENTRY,
    ELM_ATSPI_STATE_SUPPORTS_AUTOCOMPLETION,
    ELM_ATSPI_STATE_SELECTABLE_TEXT,
    ELM_ATSPI_STATE_IS_DEFAULT,
    ELM_ATSPI_STATE_VISITED,
    //TIZEN_ONLY(20160329): atspi: implement HighlightGrab and HighlightClear methods (29e253e2f7ef3c632ac3a64c489bf569df407f30)
    ELM_ATSPI_STATE_CHECKABLE,
    ELM_ATSPI_STATE_HAS_POPUP,
    ELM_ATSPI_STATE_READ_ONLY,
    ELM_ATSPI_STATE_HIGHLIGHTED,
    ELM_ATSPI_STATE_HIGHLIGHTABLE,
    //
    ELM_ATSPI_STATE_LAST_DEFINED,
};
/**
 * Describes a possible states of an object visible to AT-SPI clients.
 * @if MOBILE @since_tizen 3.0 @elseif WEARABLE @since_tizen 3.0 @endif
 */
typedef enum _Elm_Atspi_State_Type Elm_Atspi_State_Type;

enum _Elm_Atspi_Relation_Type {
     ELM_ATSPI_RELATION_NULL,
     ELM_ATSPI_RELATION_LABEL_FOR,
     ELM_ATSPI_RELATION_LABELLED_BY,
     ELM_ATSPI_RELATION_CONTROLLER_FOR,
     ELM_ATSPI_RELATION_CONTROLLED_BY,
     ELM_ATSPI_RELATION_MEMBER_OF,
     ELM_ATSPI_RELATION_TOOLTIP_FOR,
     ELM_ATSPI_RELATION_NODE_CHILD_OF,
     ELM_ATSPI_RELATION_NODE_PARENT_OF,
     ELM_ATSPI_RELATION_EXTENDED,
     ELM_ATSPI_RELATION_FLOWS_TO,
     ELM_ATSPI_RELATION_FLOWS_FROM,
     ELM_ATSPI_RELATION_SUBWINDOW_OF,
     ELM_ATSPI_RELATION_EMBEDS,
     ELM_ATSPI_RELATION_EMBEDDED_BY,
     ELM_ATSPI_RELATION_POPUP_FOR,
     ELM_ATSPI_RELATION_PARENT_WINDOW_OF,
     ELM_ATSPI_RELATION_DESCRIPTION_FOR,
     ELM_ATSPI_RELATION_DESCRIBED_BY,
     ELM_ATSPI_RELATION_LAST_DEFINED,
};
/**
 * Elm_Atspi_Relation_Type describes relation between two objects.
 * @if MOBILE @since_tizen 3.0 @elseif WEARABLE @since_tizen 3.0 @endif
 */
typedef enum _Elm_Atspi_Relation_Type Elm_Atspi_Relation_Type;

/**
 * @brief The accessible Reading information type that can be read.
 * @if MOBILE @since_tizen 4.0 @elseif WEARABLE @since_tizen 3.0 @endif
 * @ingroup Elm_Accessible_Reading_Info
 */
typedef enum
{
  ELM_ACCESSIBLE_READING_INFO_TYPE_NAME = 1 /* 0 << 0 */, /** Name should be
                                                           * read */
  ELM_ACCESSIBLE_READING_INFO_TYPE_ROLE = 2 /* 0 << 0 */, /** Role should be
                                                           * read */
  ELM_ACCESSIBLE_READING_INFO_TYPE_DESCRIPTION = 4 /* 0 << 0 */, /** description should be read. */
  ELM_ACCESSIBLE_READING_INFO_TYPE_STATE = 8 /* 0 << 0 */ /** State should be
                                                           * read. */
} Elm_Accessible_Reading_Info_Type;

#define ELM_ATSPI_GESTURE_ONE_FINGER_HOVER EFL_ACCESS_GESTURE_ONE_FINGER_HOVER
#define ELM_ATSPI_GESTURE_TWO_FINGERS_HOVER EFL_ACCESS_GESTURE_TWO_FINGERS_HOVER
#define ELM_ATSPI_GESTURE_THREE_FINGERS_HOVER EFL_ACCESS_GESTURE_THREE_FINGERS_HOVER
#define ELM_ATSPI_GESTURE_ONE_FINGER_FLICK_LEFT EFL_ACCESS_GESTURE_ONE_FINGER_FLICK_LEFT
#define ELM_ATSPI_GESTURE_ONE_FINGER_FLICK_RIGHT EFL_ACCESS_GESTURE_ONE_FINGER_FLICK_RIGHT
#define ELM_ATSPI_GESTURE_ONE_FINGER_FLICK_UP EFL_ACCESS_GESTURE_ONE_FINGER_FLICK_UP
#define ELM_ATSPI_GESTURE_ONE_FINGER_FLICK_DOWN EFL_ACCESS_GESTURE_ONE_FINGER_FLICK_DOWN
#define ELM_ATSPI_GESTURE_TWO_FINGERS_FLICK_LEFT EFL_ACCESS_GESTURE_TWO_FINGERS_FLICK_LEFT
#define ELM_ATSPI_GESTURE_TWO_FINGERS_FLICK_RIGHT EFL_ACCESS_GESTURE_TWO_FINGERS_FLICK_RIGHT
#define ELM_ATSPI_GESTURE_TWO_FINGERS_FLICK_UP EFL_ACCESS_GESTURE_TWO_FINGERS_FLICK_UP
#define ELM_ATSPI_GESTURE_TWO_FINGERS_FLICK_DOWN EFL_ACCESS_GESTURE_TWO_FINGERS_FLICK_DOWN
#define ELM_ATSPI_GESTURE_THREE_FINGERS_FLICK_LEFT EFL_ACCESS_GESTURE_THREE_FINGERS_FLICK_LEFT
#define ELM_ATSPI_GESTURE_THREE_FINGERS_FLICK_RIGHT EFL_ACCESS_GESTURE_THREE_FINGERS_FLICK_RIGHT
#define ELM_ATSPI_GESTURE_THREE_FINGERS_FLICK_UP EFL_ACCESS_GESTURE_THREE_FINGERS_FLICK_UP
#define ELM_ATSPI_GESTURE_THREE_FINGERS_FLICK_DOWN EFL_ACCESS_GESTURE_THREE_FINGERS_FLICK_DOWN
#define ELM_ATSPI_GESTURE_ONE_FINGER_SINGLE_TAP EFL_ACCESS_GESTURE_ONE_FINGER_SINGLE_TAP
#define ELM_ATSPI_GESTURE_ONE_FINGER_DOUBLE_TAP EFL_ACCESS_GESTURE_ONE_FINGER_DOUBLE_TAP
#define ELM_ATSPI_GESTURE_ONE_FINGER_TRIPLE_TAP EFL_ACCESS_GESTURE_ONE_FINGER_TRIPLE_TAP
#define ELM_ATSPI_GESTURE_TWO_FINGERS_SINGLE_TAP EFL_ACCESS_GESTURE_TWO_FINGERS_SINGLE_TAP
#define ELM_ATSPI_GESTURE_TWO_FINGERS_DOUBLE_TAP EFL_ACCESS_GESTURE_TWO_FINGERS_DOUBLE_TAP
#define ELM_ATSPI_GESTURE_TWO_FINGERS_TRIPLE_TAP EFL_ACCESS_GESTURE_TWO_FINGERS_TRIPLE_TAP
#define ELM_ATSPI_GESTURE_THREE_FINGERS_SINGLE_TAP EFL_ACCESS_GESTURE_THREE_FINGERS_SINGLE_TAP
#define ELM_ATSPI_GESTURE_THREE_FINGERS_DOUBLE_TAP EFL_ACCESS_GESTURE_THREE_FINGERS_DOUBLE_TAP
#define ELM_ATSPI_GESTURE_THREE_FINGERS_TRIPLE_TAP EFL_ACCESS_GESTURE_THREE_FINGERS_TRIPLE_TAP
#define ELM_ATSPI_GESTURE_ONE_FINGER_FLICK_LEFT_RETURN EFL_ACCESS_GESTURE_ONE_FINGER_FLICK_LEFT_RETURN
#define ELM_ATSPI_GESTURE_ONE_FINGER_FLICK_RIGHT_RETURN EFL_ACCESS_GESTURE_ONE_FINGER_FLICK_RIGHT_RETURN
#define ELM_ATSPI_GESTURE_ONE_FINGER_FLICK_UP_RETURN EFL_ACCESS_GESTURE_ONE_FINGER_FLICK_UP_RETURN
#define ELM_ATSPI_GESTURE_ONE_FINGER_FLICK_DOWN_RETURN EFL_ACCESS_GESTURE_ONE_FINGER_FLICK_DOWN_RETURN
#define ELM_ATSPI_GESTURE_TWO_FINGERS_FLICK_LEFT_RETURN EFL_ACCESS_GESTURE_TWO_FINGERS_FLICK_LEFT_RETURN
#define ELM_ATSPI_GESTURE_TWO_FINGERS_FLICK_RIGHT_RETURN EFL_ACCESS_GESTURE_TWO_FINGERS_FLICK_RIGHT_RETURN
#define ELM_ATSPI_GESTURE_TWO_FINGERS_FLICK_UP_RETURN EFL_ACCESS_GESTURE_TWO_FINGERS_FLICK_UP_RETURN
#define ELM_ATSPI_GESTURE_TWO_FINGERS_FLICK_DOWN_RETURN EFL_ACCESS_GESTURE_TWO_FINGERS_FLICK_DOWN_RETURN
#define ELM_ATSPI_GESTURE_THREE_FINGERS_FLICK_LEFT_RETURN EFL_ACCESS_GESTURE_THREE_FINGERS_FLICK_LEFT_RETURN
#define ELM_ATSPI_GESTURE_THREE_FINGERS_FLICK_RIGHT_RETURN EFL_ACCESS_GESTURE_THREE_FINGERS_FLICK_RIGHT_RETURN
#define ELM_ATSPI_GESTURE_THREE_FINGERS_FLICK_UP_RETURN EFL_ACCESS_GESTURE_THREE_FINGERS_FLICK_UP_RETURN
#define ELM_ATSPI_GESTURE_THREE_FINGERS_FLICK_DOWN_RETURN EFL_ACCESS_GESTURE_THREE_FINGERS_FLICK_DOWN_RETURN

/**
 * Type of gesture that is emitted on assistive technology client side.
 *
 * @warning do not use ELM_ATSPI_GESTURE_TWO_FINGERS_FLICK_*, and
 * ELM_ATSPI_GESTURE_TWO_FINGERS_HOVER. These gestures are not emitted on
 * assistive technology client side. The window manager emits mouse event
 * directly. So if you need to check these gestures, you have to handle mouse
 * events. If you want to make highlighted object to get mouse events directly,
 * then please use elm_atspi_accessible_attribute_append as below.
 *
 * elm_atspi_accessible_attribute_append(obj, "gesture_required", "scroll");
 *
 * @if MOBILE @since_tizen 4.0 @elseif WEARABLE @since_tizen 3.0 @endif
 */
typedef Efl_Access_Gesture Elm_Atspi_Gesture_Type;

enum _Elm_Atspi_Gesture_State
{
   ELM_ATSPI_GESTURE_STATE_START,
   ELM_ATSPI_GESTURE_STATE_MOVE,
   ELM_ATSPI_GESTURE_STATE_END,
   ELM_ATSPI_GESTURE_STATE_ABORT,
};
/**
 * State of gesture that is emitted on assistive technology client side.
 * @if MOBILE @since_tizen 4.0 @elseif WEARABLE @since_tizen 3.0 @endif
 */
typedef enum _Elm_Atspi_Gesture_State Elm_Atspi_Gesture_State;

struct _Elm_Atspi_Gesture_Info
{
   Elm_Atspi_Gesture_Type type;
   int x_beg;
   int y_beg;
   int x_end;
   int y_end;
   Elm_Atspi_Gesture_State state;
   unsigned int event_time;
};
/**
 * Detailed information of gesture that is emitted on assistive technology client side.
 * @if MOBILE @since_tizen 4.0 @elseif WEARABLE @since_tizen 3.0 @endif
 */
#define Elm_Atspi_Gesture_Info Efl_Access_Gesture_Info

/**
 * A Elm_Accessible_Reading_Info_Type mask indicating which types of reading
 * information are requested.
 * @see elm_atspi_accessible_reading_info_type_set
 *
 * @if MOBILE @since_tizen 4.0 @elseif WEARABLE @since_tizen 3.0 @endif
 */
typedef unsigned int Elm_Atspi_Reading_Info_Type_Mask;

/**
 * This callback type is used to provide accessibility reading information.
 * @param data The data specified as the last param when setting this callback
 * @param obj The accessible object specified as the fisrt param when setting
 * this callback
 * @return The information to be used by accessibilty client
 * @see elm_atspi_accessible_name_cb_set
 * @see elm_atspi_accessible_description_cb_set
 *
 * @if MOBILE @since_tizen 4.0 @elseif WEARABLE @since_tizen 3.0 @endif
 */
typedef char *(*Elm_Atspi_Reading_Info_Cb)(void *data, Evas_Object *obj);

/**
 * This callback type is used to provide accessibility gesture information.
 * @param data The data specified as the last param when setting this callback
 * @param obj The accessible object specified as the fisrt param when setting
 * this callback
 * @return @c true if application consumes gesutre, @c false if application does
 * not consume gesture, then assistive technology client consumes gesture
 * @see elm_atspi_accessible_gesture_cb_set
 * @if MOBILE @since_tizen 4.0 @elseif WEARABLE @since_tizen 3.0 @endif
 */
#define Elm_Atspi_Gesture_Cb Efl_Access_Gesture_Cb

/** AT-SPI object state set.
 *
 * @ingroup Elm_Atspi
 */
typedef uint64_t Elm_Atspi_State_Set;

typedef Eina_List *Elm_Atspi_Relation_Set;

/**
 * @brief Defines the relationship between two accessible objects.
 *
 * Adds unique relation between source object and relation_object of a given
 * type.
 *
 * Relationships can be queried by Assistive Technology clients to provide
 * customized feedback, improving overall user experience.
 *
 * Relationship_append API is asymmetric, which means that appending, for
 * example, relation ELM_ATSPI_RELATION_FLOWS_TO from object A to B, do NOT
 * append relation ELM_ATSPI_RELATION_FLOWS_FROM from object B to object A.
 *
 * return: EINA_TRUE is relationship was successfully appended, EINA_FALSE
 * otherwise
 *
 * @if MOBILE @since_tizen 2.4 @elseif WEARABLE @since_tizen 3.0 @endif
 *
 * @param[in] obj The object.
 * @param[in] type Relation type between two objects
 * @param[in] relation_object Relation object of source object
 *
 * @ingroup Elm_Interface_Atspi_Accessible
 */
EAPI Eina_Bool elm_atspi_accessible_relationship_append(Elm_Interface_Atspi_Accessible *obj, Elm_Atspi_Relation_Type type, const Elm_Interface_Atspi_Accessible *relation_object);

/**
 * @brief Removes the relationship between two accessible objects.
 *
 * If relation_object is NULL function removes all relations of given type.
 *
 * @if MOBILE @since_tizen 2.4 @elseif WEARABLE @since_tizen 3.0 @endif
 *
 * @param[in] obj The object.
 * @param[in] type Relation type between two objects
 * @param[in] relation_object Relation object of source object
 *
 * @ingroup Elm_Interface_Atspi_Accessible
 */
EAPI void elm_atspi_accessible_relationship_remove(Elm_Interface_Atspi_Accessible *obj, Elm_Atspi_Relation_Type type, const Elm_Interface_Atspi_Accessible *relation_object);

/**
 * @brief Gets the translation domain of "name" and "description" properties.
 *
 * @if MOBILE @since_tizen 2.4 @elseif WEARABLE @since_tizen 3.0 @endif
 *
 * @param[in] obj The object.
 *
 * @return translation domain
 *
 * @ingroup Elm_Interface_Atspi_Accessible
 */
EAPI const char *elm_atspi_accessible_translation_domain_get(const Elm_Interface_Atspi_Accessible *obj);

/**
 * @brief Sets the translation domain of "name" and "description" properties.
 *
 * Translation domain should be set if application wants to support i18n for
 * accessibily "name" and "description" properties.
 *
 * When translation domain is set values of "name" and "description" properties
 * will be translated with dgettext function using current translation domain
 * as "domainname" parameter.
 *
 * It is application developer responsibility to ensure that translation files
 * are loaded and binded to translation domain when accessibility is enabled.
 *
 * @if MOBILE @since_tizen 2.4 @elseif WEARABLE @since_tizen 3.0 @endif
 *
 * @param[in] obj The object.
 * @param[in] domain translation domain
 *
 * @ingroup Elm_Interface_Atspi_Accessible
 */
EAPI void elm_atspi_accessible_translation_domain_set(Elm_Interface_Atspi_Accessible *obj, const char *domain);

/**
 * @brief Gets an localized string describing accessible object role name.
 *
 * @internal
 *
 * @param[in] obj The object.
 *
 * @return Localized accessible role name
 *
 * @ingroup Elm_Interface_Atspi_Accessible
 */
EAPI const char *elm_atspi_accessible_localized_role_name_get(const Elm_Interface_Atspi_Accessible *obj);

/**
 * @brief Sets an accessible name of the object.
 *
 * @if MOBILE @since_tizen 2.4 @elseif WEARABLE @since_tizen 3.0 @endif
 *
 * @param[in] obj The object.
 * @param[in] name Accessible name
 *
 * @ingroup Elm_Interface_Atspi_Accessible
 */
EAPI void elm_atspi_accessible_name_set(Elm_Interface_Atspi_Accessible *obj, const char *name);

/**
 * @brief Gets an accessible name of the object.
 *
 * @if MOBILE @since_tizen 2.4 @elseif WEARABLE @since_tizen 3.0 @endif
 *
 * @param[in] obj The object.
 *
 * @return Accessible name
 *
 * @ingroup Elm_Interface_Atspi_Accessible
 */
EAPI const char *elm_atspi_accessible_name_get(const Elm_Interface_Atspi_Accessible *obj);

/**
 * @brief Sets name information callback about widget.
 *
 * @if MOBILE @since_tizen 4.0 @elseif WEARABLE @since_tizen 3.0 @endif
 *
 * @param[in] obj The object.
 * @param[in] name_cb The function called to provide the accessible name
 * @param[in] data The data passed to @c name_cb
 *
 * @ingroup Elm_Interface_Atspi_Accessible
 */
EAPI void elm_atspi_accessible_name_cb_set(Elm_Interface_Atspi_Accessible *obj, Elm_Atspi_Reading_Info_Cb name_cb, const void *data);

/**
 * @brief Gets an all relations between accessible object and other accessible
 * objects.
 *
 * @internal
 *
 * @param[in] obj The object.
 *
 * @return Accessible relation set
 *
 * @ingroup Elm_Interface_Atspi_Accessible
 */
EAPI Elm_Atspi_Relation_Set elm_atspi_accessible_relation_set_get(const Elm_Interface_Atspi_Accessible *obj);

/**
 * @brief Sets the role of the object in accessibility domain.
 *
 * @if MOBILE @since_tizen 2.4 @elseif WEARABLE @since_tizen 3.0 @endif
 *
 * @param[in] obj The object.
 * @param[in] role Accessible role
 *
 * @ingroup Elm_Interface_Atspi_Accessible
 */
EAPI void elm_atspi_accessible_role_set(Elm_Interface_Atspi_Accessible *obj, Elm_Atspi_Role role);

/**
 * @brief Gets the role of the object in accessibility domain.
 *
 * @if MOBILE @since_tizen 2.4 @elseif WEARABLE @since_tizen 3.0 @endif
 *
 * @param[in] obj The object.
 *
 * @return Accessible role
 *
 * @ingroup Elm_Interface_Atspi_Accessible
 */
EAPI Elm_Atspi_Role elm_atspi_accessible_role_get(const Elm_Interface_Atspi_Accessible *obj);

/**
 * @brief Gets object's accessible children.
 *
 * @internal
 * @param[in] obj The object.
 *
 * @ingroup Elm_Interface_Atspi_Accessible
 */
EAPI Eina_List *elm_atspi_accessible_children_get(const Elm_Interface_Atspi_Accessible *obj);

/**
 * @brief Gets human-readable string indentifying widget accessibility role.
 *
 * @internal
 *
 * @param[in] obj The object.
 *
 * @return Accessible role name
 *
 * @ingroup Elm_Interface_Atspi_Accessible
 */
EAPI const char *elm_atspi_accessible_role_name_get(const Elm_Interface_Atspi_Accessible *obj);

/**
 * @brief Gets key-value pairs identifying object extra attributes. Must be
 * free by a user.
 *
 * @internal
 *
 * @param[in] obj The object.
 *
 * @return List of object attributes
 *
 * @ingroup Elm_Interface_Atspi_Accessible
 */
EAPI Eina_List *elm_atspi_accessible_attributes_get(const Elm_Interface_Atspi_Accessible *obj);

/**
 * @brief Gets reading information of an accessible object.
 *
 * @if MOBILE @since_tizen 4.0 @elseif WEARABLE @since_tizen 3.0 @endif
 *
 * @param[in] obj The object.
 * @param[in] reading_info Reading information types
 *
 * @ingroup Elm_Interface_Atspi_Accessible
 */
EAPI void elm_atspi_accessible_reading_info_type_set(Elm_Interface_Atspi_Accessible *obj, Elm_Atspi_Reading_Info_Type_Mask reading_info);

/**
 * @brief Gets reading information types of an accessible object.
 *
 * @if MOBILE @since_tizen 4.0 @elseif WEARABLE @since_tizen 3.0 @endif
 *
 * @param[in] obj The object.
 *
 * @return Reading information types
 *
 * @ingroup Elm_Interface_Atspi_Accessible
 */
EAPI Elm_Atspi_Reading_Info_Type_Mask elm_atspi_accessible_reading_info_type_get(const Elm_Interface_Atspi_Accessible *obj);

/**
 * @brief Gets index of the child in parent's children list.
 *
 * @internal
 *
 * @param[in] obj The object.
 *
 * @return Index in children list
 *
 * @ingroup Elm_Interface_Atspi_Accessible
 */
EAPI int elm_atspi_accessible_index_in_parent_get(const Elm_Interface_Atspi_Accessible *obj);

/**
 * @brief Sets widget contextual information.
 *
 * @if MOBILE @since_tizen 2.4 @elseif WEARABLE @since_tizen 3.0 @endif
 *
 * @param[in] obj The object.
 * @param[in] description Accessible contextual information
 *
 * @ingroup Elm_Interface_Atspi_Accessible
 */
EAPI void elm_atspi_accessible_description_set(Elm_Interface_Atspi_Accessible *obj, const char *description);

/**
 * @brief Gets contextual information about object.
 *
 * @if MOBILE @since_tizen 2.4 @elseif WEARABLE @since_tizen 3.0 @endif
 *
 * @param[in] obj The object.
 *
 * @return Accessible contextual information
 *
 * @ingroup Elm_Interface_Atspi_Accessible
 */
EAPI const char *elm_atspi_accessible_description_get(const Elm_Interface_Atspi_Accessible *obj);

/**
 * @brief Sets contextual information callback about widget.
 *
 * @if MOBILE @since_tizen 4.0 @elseif WEARABLE @since_tizen 3.0 @endif
 *
 * @param[in] obj The object.
 * @param[in] description_cb The function called to provide the accessible
 * description
 * @param[in] data The data passed to @c description_cb
 *
 * @ingroup Elm_Interface_Atspi_Accessible
 */
EAPI void elm_atspi_accessible_description_cb_set(Elm_Interface_Atspi_Accessible *obj, Elm_Atspi_Reading_Info_Cb description_cb, const void *data);

/**
 * @brief Sets gesture callback to give widget.
 *
 * @warning Please do not abuse this API. The purpose of this API is to support
 * special application such as screen-reader guidance. Before using this API,
 * please check if there is another way.
 *
 * @if MOBILE @since_tizen 4.0 @elseif WEARABLE @since_tizen 3.0 @endif
 *
 * @param[in] obj The object.
 * @param[in] gesutre_cb The function called to get gesture information
 * @param[in] data The data passed to @c description_cb
 *
 * @ingroup Elm_Interface_Atspi_Accessible
 */
EAPI void elm_atspi_accessible_gesture_cb_set(Elm_Interface_Atspi_Accessible *obj, Elm_Atspi_Gesture_Cb gesture_cb, const void *data);

/**
 * @brief Sets object's accessible parent.
 *
 * @internal
 *
 * @param[in] obj The object.
 * @param[in] parent
 *
 * @ingroup Elm_Interface_Atspi_Accessible
 */
EAPI void elm_atspi_accessible_parent_set(Elm_Interface_Atspi_Accessible *obj, Elm_Interface_Atspi_Accessible *parent);

/**
 * @brief Gets object's accessible parent.
 *
 * @internal
 * @param[in] obj The object.
 *
 * @ingroup Elm_Interface_Atspi_Accessible
 */
EAPI Elm_Interface_Atspi_Accessible *elm_atspi_accessible_parent_get(const Elm_Interface_Atspi_Accessible *obj);

/**
 * @brief Gets set describing object accessible states.
 *
 * @internal
 * @param[in] obj The object.
 *
 * @ingroup Elm_Interface_Atspi_Accessible
 */
EAPI Elm_Atspi_State_Set elm_atspi_accessible_state_set_get(const Elm_Interface_Atspi_Accessible *obj);

/**
 * @brief Sets highlightable to given widget.
 *
 * @if MOBILE @since_tizen 4.0 @elseif WEARABLE @since_tizen 3.0 @endif
 *
 * @param[in] obj The object.
 * @param[in] can_highlight If @c true, the object is highlightable
 *
 * @ingroup Elm_Interface_Atspi_Accessible
 */
EAPI void elm_atspi_accessible_can_highlight_set(Elm_Interface_Atspi_Accessible *obj, Eina_Bool can_highlight);

/**
 * @brief Gets highlightable of given widget.
 *
 * @if MOBILE @since_tizen 4.0 @elseif WEARABLE @since_tizen 3.0 @endif
 *
 * @param[in] obj The object.
 *
 * @return If @c true, the object is highlightable
 *
 * @ingroup Elm_Interface_Atspi_Accessible
 */
EAPI Eina_Bool elm_atspi_accessible_can_highlight_get(const Elm_Interface_Atspi_Accessible *obj);

/**
 * @brief Add key-value pair identifying widget extra attribute
 *
 * @if MOBILE @since_tizen 4.0 @elseif WEARABLE @since_tizen 3.0 @endif
 *
 * @param[in] obj The object.
 * @param[in] key The string key to give extra information
 * @param[in] value The string value to give extra information
 *
 * @ingroup Elm_Interface_Atspi_Accessible
 */
EAPI void elm_atspi_accessible_attribute_append(Elm_Interface_Atspi_Accessible *obj, const char *key, const char *value);

/**
 * @brief Removes all attributes in accessible object.
 *
 * @internal
 * @param[in] obj The object.
 *
 * @ingroup Elm_Interface_Atspi_Accessible
 */
EAPI void elm_atspi_accessible_attributes_clear(Elm_Interface_Atspi_Accessible *obj);

/**
 * @brief Register accessibility event listener
 *
 * @internal
 *
 * @param[in] cb callback
 * @param[in] data data
 *
 * @return Event handler
 *
 * @ingroup Elm_Interface_Atspi_Accessible
 */
//TODO: check if it is necessary
//EAPI Elm_Atspi_Event_Handler *elm_atspi_accessible_event_handler_add(Eo_Event_Cb cb, void *data);

/**
 * @brief Deregister accessibility event listener
 *
 * @internal
 *
 * @param[in] handler Event handler
 *
 * @ingroup Elm_Interface_Atspi_Accessible
 */
//TODO: check if it is necessary
//EAPI void elm_atspi_accessible_event_handler_del(Elm_Atspi_Event_Handler *handler);

/**
 * @brief Emit accessibility event which could occur atspi event
 *
 * @internal
 *
 * @param[in] accessible Accessibility object.
 * @param[in] event Accessibility event type.
 * @param[in] event_info Accessibility event details.
 *
 * @ingroup Elm_Interface_Atspi_Accessible
 */
//TODO: check if it is necessary
//EAPI void elm_atspi_accessible_event_emit(Elm_Interface_Atspi_Accessible *accessible, const Eo_Event_Description *event, void *event_info);

/**
 * @brief Removes all relationships in accessible object.
 *
 * @since_tizen 4.0
 * @param[in] obj The object.
 *
 * @ingroup Elm_Interface_Atspi_Accessible
 */
EAPI void elm_atspi_accessible_relationships_clear(Elm_Interface_Atspi_Accessible *obj);

/**
 * @brief Notifies assistive technology clients about current state of the
 * accessible object.
 *
 * Function limits information broadcast to clients to types specified by
 * state_types_mask parameter.
 *
 * if recursive parameter is set, function will traverse all accessible
 * children and call state_notify function on them.
 *
 * @internal
 *
 * @param[in] obj The object.
 * @param[in] state_types_mask
 * @param[in] recursive
 *
 * @ingroup Elm_Interface_Atspi_Accessible
 */
EAPI void elm_atspi_accessible_state_notify(Elm_Interface_Atspi_Accessible *obj, Elm_Atspi_State_Set state_types_mask, Eina_Bool recursive);

/**
 * Frees Elm_Atspi_Relation_Set
 *
 * @if MOBILE @since_tizen 3.0
 * @elseif WEARABLE @since_tizen 3.0
 * @endif
 */
EAPI void elm_atspi_relation_set_free(Elm_Atspi_Relation_Set *set);

/**
 * Clones Elm_Atspi_Relation_Set
 *
 * @if MOBILE @since_tizen 3.0
 * @elseif WEARABLE @since_tizen 3.0
 * @endif
 */
EAPI Elm_Atspi_Relation_Set elm_atspi_relation_set_clone(const Elm_Atspi_Relation_Set *set);

#define Elm_Atspi_Relation Efl_Access_Relation

/**
 * Frees relation.
 *
 * @if MOBILE @since_tizen 3.0
 * @elseif WEARABLE @since_tizen 3.0
 * @endif
 */
EAPI void elm_atspi_relation_free(Elm_Atspi_Relation *relation);

/**
 * Clones relation.
 *
 * @if MOBILE @since_tizen 3.0
 * @elseif WEARABLE @since_tizen 3.0
 * @endif
 */
EAPI Elm_Atspi_Relation * elm_atspi_relation_clone(const Elm_Atspi_Relation *relation);

/**
 * Appends relation to relation set
 *
 * @if MOBILE @since_tizen 3.0
 * @elseif WEARABLE @since_tizen 3.0
 * @endif
 */
EAPI Eina_Bool elm_atspi_relation_set_relation_append(Elm_Atspi_Relation_Set *set, Elm_Atspi_Relation_Type type, const Eo *rel_obj);

/**
 * Removes relation from relation set
 *
 * @if MOBILE @since_tizen 3.0
 * @elseif WEARABLE @since_tizen 3.0
 * @endif
 */
EAPI void elm_atspi_relation_set_relation_remove(Elm_Atspi_Relation_Set *set, Elm_Atspi_Relation_Type type, const Eo *rel_obj);

/**
 * Removes all relation from relation set of a given type
 *
 * @if MOBILE @since_tizen 3.0
 * @elseif WEARABLE @since_tizen 3.0
 * @endif
 */
EAPI void elm_atspi_relation_set_relation_type_remove(Elm_Atspi_Relation_Set *set, Elm_Atspi_Relation_Type type);

/**
 * Free Elm_Atspi_Attributes_List
 *
 * @if MOBILE @since_tizen 3.0
 * @elseif WEARABLE @since_tizen 3.0
 * @endif
 */
EAPI void elm_atspi_attributes_list_free(Eina_List *list);

//TIZEN_ONLY(20180509) : efl_access: Support legacy API for elm_atspi_text
struct _Elm_Atspi_Text_Attribute
{
   const char *name;
   const char *value;
};

typedef struct _Elm_Atspi_Text_Attribute Elm_Atspi_Text_Attribute;
//

//TIZEN_ONLY(20160721): atspi: provide highlight legacy API
typedef Eo Efl_Access_Component;
/**
 * @brief Highlights accessible widget. returns true if highlight grab has
 * successed, false otherwise.
 *
 * @if MOBILE @since_tizen 4.0 @elseif WEARABLE @since_tizen 3.0 @endif
 * @param[in] obj The object.
 *
 * @ingroup (null)_Group
 */
EAPI Eina_Bool elm_atspi_component_highlight_grab(Efl_Access_Component *obj);

/**
 * @brief Clears highlight of accessible widget. returns true if clear has
 * successed, false otherwise.
 *
 * @if MOBILE @since_tizen 4.0 @elseif WEARABLE @since_tizen 3.0 @endif
 * @param[in] obj The object.
 *
 * @ingroup (null)_Group
 */
EAPI Eina_Bool elm_atspi_component_highlight_clear(Efl_Access_Component *obj);
//

/**
 * @}
 */

