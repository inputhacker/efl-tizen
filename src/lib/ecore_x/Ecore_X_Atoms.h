#ifndef _ECORE_X_ATOMS_H
#define _ECORE_X_ATOMS_H

/**
 * @file
 * @brief Ecore X atoms
 */

/* generic atoms */
EAPI extern Ecore_X_Atom ECORE_X_ATOM_ATOM;
EAPI extern Ecore_X_Atom ECORE_X_ATOM_CARDINAL;
EAPI extern Ecore_X_Atom ECORE_X_ATOM_COMPOUND_TEXT;
EAPI extern Ecore_X_Atom ECORE_X_ATOM_FILE_NAME;
EAPI extern Ecore_X_Atom ECORE_X_ATOM_STRING;
EAPI extern Ecore_X_Atom ECORE_X_ATOM_TEXT;
EAPI extern Ecore_X_Atom ECORE_X_ATOM_UTF8_STRING;
EAPI extern Ecore_X_Atom ECORE_X_ATOM_X_MOZ_URL;
EAPI extern Ecore_X_Atom ECORE_X_ATOM_WINDOW;
EAPI extern Ecore_X_Atom ECORE_X_ATOM_PIXMAP;
EAPI extern Ecore_X_Atom ECORE_X_ATOM_VISUALID;

/* dnd atoms */
EAPI extern Ecore_X_Atom ECORE_X_ATOM_SELECTION_XDND;
EAPI extern Ecore_X_Atom ECORE_X_ATOM_SELECTION_PROP_XDND;
EAPI extern Ecore_X_Atom ECORE_X_ATOM_XDND_AWARE;
EAPI extern Ecore_X_Atom ECORE_X_ATOM_XDND_ENTER;
EAPI extern Ecore_X_Atom ECORE_X_ATOM_XDND_TYPE_LIST;
EAPI extern Ecore_X_Atom ECORE_X_ATOM_XDND_POSITION;
EAPI extern Ecore_X_Atom ECORE_X_ATOM_XDND_ACTION_COPY;
EAPI extern Ecore_X_Atom ECORE_X_ATOM_XDND_ACTION_MOVE;
EAPI extern Ecore_X_Atom ECORE_X_ATOM_XDND_ACTION_PRIVATE;
EAPI extern Ecore_X_Atom ECORE_X_ATOM_XDND_ACTION_ASK;
EAPI extern Ecore_X_Atom ECORE_X_ATOM_XDND_ACTION_LIST;
EAPI extern Ecore_X_Atom ECORE_X_ATOM_XDND_ACTION_LINK;
EAPI extern Ecore_X_Atom ECORE_X_ATOM_XDND_ACTION_DESCRIPTION;
EAPI extern Ecore_X_Atom ECORE_X_ATOM_XDND_PROXY;
EAPI extern Ecore_X_Atom ECORE_X_ATOM_XDND_STATUS;
EAPI extern Ecore_X_Atom ECORE_X_ATOM_XDND_LEAVE;
EAPI extern Ecore_X_Atom ECORE_X_ATOM_XDND_DROP;
EAPI extern Ecore_X_Atom ECORE_X_ATOM_XDND_FINISHED;
EAPI extern Ecore_X_Atom ECORE_X_ATOM_XDND_DIRECTSAVE0; /**< @since 1.8 */

/* dnd atoms that need to be exposed to the application interface */
EAPI extern Ecore_X_Atom ECORE_X_DND_ACTION_COPY;
EAPI extern Ecore_X_Atom ECORE_X_DND_ACTION_MOVE;
EAPI extern Ecore_X_Atom ECORE_X_DND_ACTION_LINK;
EAPI extern Ecore_X_Atom ECORE_X_DND_ACTION_ASK;
EAPI extern Ecore_X_Atom ECORE_X_DND_ACTION_PRIVATE;

/* old E atom */
EAPI extern Ecore_X_Atom ECORE_X_ATOM_E_FRAME_SIZE;

/* old Gnome atom */
EAPI extern Ecore_X_Atom ECORE_X_ATOM_WIN_LAYER;

/* ICCCM: client properties */
EAPI extern Ecore_X_Atom ECORE_X_ATOM_WM_NAME;
EAPI extern Ecore_X_Atom ECORE_X_ATOM_WM_ICON_NAME;
EAPI extern Ecore_X_Atom ECORE_X_ATOM_WM_NORMAL_HINTS;
EAPI extern Ecore_X_Atom ECORE_X_ATOM_WM_SIZE_HINTS;
EAPI extern Ecore_X_Atom ECORE_X_ATOM_WM_HINTS;
EAPI extern Ecore_X_Atom ECORE_X_ATOM_WM_CLASS;
EAPI extern Ecore_X_Atom ECORE_X_ATOM_WM_TRANSIENT_FOR;
EAPI extern Ecore_X_Atom ECORE_X_ATOM_WM_PROTOCOLS;
EAPI extern Ecore_X_Atom ECORE_X_ATOM_WM_COLORMAP_WINDOWS;
EAPI extern Ecore_X_Atom ECORE_X_ATOM_WM_COMMAND;
EAPI extern Ecore_X_Atom ECORE_X_ATOM_WM_CLIENT_MACHINE;

/* ICCCM: window manager properties */
EAPI extern Ecore_X_Atom ECORE_X_ATOM_WM_STATE;
EAPI extern Ecore_X_Atom ECORE_X_ATOM_WM_ICON_SIZE;

/* ICCCM: WM_STATEproperty */
EAPI extern Ecore_X_Atom ECORE_X_ATOM_WM_CHANGE_STATE;

/* ICCCM: WM_PROTOCOLS properties */
EAPI extern Ecore_X_Atom ECORE_X_ATOM_WM_TAKE_FOCUS;
EAPI extern Ecore_X_Atom ECORE_X_ATOM_WM_SAVE_YOURSELF;
EAPI extern Ecore_X_Atom ECORE_X_ATOM_WM_DELETE_WINDOW;

/* ICCCM: WM_COLORMAP properties */
EAPI extern Ecore_X_Atom ECORE_X_ATOM_WM_COLORMAP_NOTIFY;

/* ICCCM: session management properties */
EAPI extern Ecore_X_Atom ECORE_X_ATOM_SM_CLIENT_ID;
EAPI extern Ecore_X_Atom ECORE_X_ATOM_WM_CLIENT_LEADER;
EAPI extern Ecore_X_Atom ECORE_X_ATOM_WM_WINDOW_ROLE;

/* Motif WM atom */
EAPI extern Ecore_X_Atom ECORE_X_ATOM_MOTIF_WM_HINTS;

EAPI extern Ecore_X_Atom ECORE_X_ATOM_NET_SUPPORTED;
EAPI extern Ecore_X_Atom ECORE_X_ATOM_NET_CLIENT_LIST;
EAPI extern Ecore_X_Atom ECORE_X_ATOM_NET_CLIENT_LIST_STACKING;
EAPI extern Ecore_X_Atom ECORE_X_ATOM_NET_NUMBER_OF_DESKTOPS;
EAPI extern Ecore_X_Atom ECORE_X_ATOM_NET_DESKTOP_GEOMETRY;
EAPI extern Ecore_X_Atom ECORE_X_ATOM_NET_DESKTOP_VIEWPORT;
EAPI extern Ecore_X_Atom ECORE_X_ATOM_NET_CURRENT_DESKTOP;
EAPI extern Ecore_X_Atom ECORE_X_ATOM_NET_DESKTOP_NAMES;
EAPI extern Ecore_X_Atom ECORE_X_ATOM_NET_ACTIVE_WINDOW;
EAPI extern Ecore_X_Atom ECORE_X_ATOM_NET_WORKAREA;
EAPI extern Ecore_X_Atom ECORE_X_ATOM_NET_SUPPORTING_WM_CHECK;
EAPI extern Ecore_X_Atom ECORE_X_ATOM_NET_VIRTUAL_ROOTS;
EAPI extern Ecore_X_Atom ECORE_X_ATOM_NET_DESKTOP_LAYOUT;
EAPI extern Ecore_X_Atom ECORE_X_ATOM_NET_SHOWING_DESKTOP;

/* pager */
EAPI extern Ecore_X_Atom ECORE_X_ATOM_NET_CLOSE_WINDOW;
EAPI extern Ecore_X_Atom ECORE_X_ATOM_NET_MOVERESIZE_WINDOW;
EAPI extern Ecore_X_Atom ECORE_X_ATOM_NET_WM_MOVERESIZE;
EAPI extern Ecore_X_Atom ECORE_X_ATOM_NET_RESTACK_WINDOW;

EAPI extern Ecore_X_Atom ECORE_X_ATOM_NET_REQUEST_FRAME_EXTENTS;
EAPI extern Ecore_X_Atom ECORE_X_ATOM_NET_WM_NAME;
EAPI extern Ecore_X_Atom ECORE_X_ATOM_NET_WM_VISIBLE_NAME;
EAPI extern Ecore_X_Atom ECORE_X_ATOM_NET_WM_ICON_NAME;
EAPI extern Ecore_X_Atom ECORE_X_ATOM_NET_WM_VISIBLE_ICON_NAME;
EAPI extern Ecore_X_Atom ECORE_X_ATOM_NET_WM_DESKTOP;

/* window type */
EAPI extern Ecore_X_Atom ECORE_X_ATOM_NET_WM_WINDOW_TYPE;
EAPI extern Ecore_X_Atom ECORE_X_ATOM_NET_WM_WINDOW_TYPE_DESKTOP;
EAPI extern Ecore_X_Atom ECORE_X_ATOM_NET_WM_WINDOW_TYPE_DOCK;
EAPI extern Ecore_X_Atom ECORE_X_ATOM_NET_WM_WINDOW_TYPE_TOOLBAR;
EAPI extern Ecore_X_Atom ECORE_X_ATOM_NET_WM_WINDOW_TYPE_MENU;
EAPI extern Ecore_X_Atom ECORE_X_ATOM_NET_WM_WINDOW_TYPE_UTILITY;
EAPI extern Ecore_X_Atom ECORE_X_ATOM_NET_WM_WINDOW_TYPE_SPLASH;
EAPI extern Ecore_X_Atom ECORE_X_ATOM_NET_WM_WINDOW_TYPE_DIALOG;
EAPI extern Ecore_X_Atom ECORE_X_ATOM_NET_WM_WINDOW_TYPE_NORMAL;
EAPI extern Ecore_X_Atom ECORE_X_ATOM_NET_WM_WINDOW_TYPE_DROPDOWN_MENU;
EAPI extern Ecore_X_Atom ECORE_X_ATOM_NET_WM_WINDOW_TYPE_POPUP_MENU;
EAPI extern Ecore_X_Atom ECORE_X_ATOM_NET_WM_WINDOW_TYPE_TOOLTIP;
EAPI extern Ecore_X_Atom ECORE_X_ATOM_NET_WM_WINDOW_TYPE_NOTIFICATION;
EAPI extern Ecore_X_Atom ECORE_X_ATOM_NET_WM_WINDOW_TYPE_COMBO;
EAPI extern Ecore_X_Atom ECORE_X_ATOM_NET_WM_WINDOW_TYPE_DND;

/* state */
EAPI extern Ecore_X_Atom ECORE_X_ATOM_NET_WM_STATE;
EAPI extern Ecore_X_Atom ECORE_X_ATOM_NET_WM_STATE_MODAL;
EAPI extern Ecore_X_Atom ECORE_X_ATOM_NET_WM_STATE_STICKY;
EAPI extern Ecore_X_Atom ECORE_X_ATOM_NET_WM_STATE_MAXIMIZED_VERT;
EAPI extern Ecore_X_Atom ECORE_X_ATOM_NET_WM_STATE_MAXIMIZED_HORZ;
EAPI extern Ecore_X_Atom ECORE_X_ATOM_NET_WM_STATE_SHADED;
EAPI extern Ecore_X_Atom ECORE_X_ATOM_NET_WM_STATE_SKIP_TASKBAR;
EAPI extern Ecore_X_Atom ECORE_X_ATOM_NET_WM_STATE_SKIP_PAGER;
EAPI extern Ecore_X_Atom ECORE_X_ATOM_NET_WM_STATE_HIDDEN;
EAPI extern Ecore_X_Atom ECORE_X_ATOM_NET_WM_STATE_FULLSCREEN;
EAPI extern Ecore_X_Atom ECORE_X_ATOM_NET_WM_STATE_ABOVE;
EAPI extern Ecore_X_Atom ECORE_X_ATOM_NET_WM_STATE_BELOW;
EAPI extern Ecore_X_Atom ECORE_X_ATOM_NET_WM_STATE_DEMANDS_ATTENTION;

/* allowed actions */
EAPI extern Ecore_X_Atom ECORE_X_ATOM_NET_WM_ALLOWED_ACTIONS;
EAPI extern Ecore_X_Atom ECORE_X_ATOM_NET_WM_ACTION_MOVE;
EAPI extern Ecore_X_Atom ECORE_X_ATOM_NET_WM_ACTION_RESIZE;
EAPI extern Ecore_X_Atom ECORE_X_ATOM_NET_WM_ACTION_MINIMIZE;
EAPI extern Ecore_X_Atom ECORE_X_ATOM_NET_WM_ACTION_SHADE;
EAPI extern Ecore_X_Atom ECORE_X_ATOM_NET_WM_ACTION_STICK;
EAPI extern Ecore_X_Atom ECORE_X_ATOM_NET_WM_ACTION_MAXIMIZE_HORZ;
EAPI extern Ecore_X_Atom ECORE_X_ATOM_NET_WM_ACTION_MAXIMIZE_VERT;
EAPI extern Ecore_X_Atom ECORE_X_ATOM_NET_WM_ACTION_FULLSCREEN;
EAPI extern Ecore_X_Atom ECORE_X_ATOM_NET_WM_ACTION_CHANGE_DESKTOP;
EAPI extern Ecore_X_Atom ECORE_X_ATOM_NET_WM_ACTION_CLOSE;
EAPI extern Ecore_X_Atom ECORE_X_ATOM_NET_WM_ACTION_ABOVE;
EAPI extern Ecore_X_Atom ECORE_X_ATOM_NET_WM_ACTION_BELOW;

EAPI extern Ecore_X_Atom ECORE_X_ATOM_NET_WM_STRUT;
EAPI extern Ecore_X_Atom ECORE_X_ATOM_NET_WM_STRUT_PARTIAL;
EAPI extern Ecore_X_Atom ECORE_X_ATOM_NET_WM_ICON_GEOMETRY;
EAPI extern Ecore_X_Atom ECORE_X_ATOM_NET_WM_ICON;
EAPI extern Ecore_X_Atom ECORE_X_ATOM_NET_WM_PID;
EAPI extern Ecore_X_Atom ECORE_X_ATOM_NET_WM_HANDLED_ICONS;
EAPI extern Ecore_X_Atom ECORE_X_ATOM_NET_WM_USER_TIME;
EAPI extern Ecore_X_Atom ECORE_X_ATOM_NET_STARTUP_ID;
EAPI extern Ecore_X_Atom ECORE_X_ATOM_NET_FRAME_EXTENTS;
EAPI extern Ecore_X_Atom ECORE_X_ATOM_NET_WM_PING;
EAPI extern Ecore_X_Atom ECORE_X_ATOM_NET_WM_SYNC_REQUEST;
EAPI extern Ecore_X_Atom ECORE_X_ATOM_NET_WM_SYNC_REQUEST_COUNTER;
EAPI extern Ecore_X_Atom ECORE_X_ATOM_NET_WM_WINDOW_OPACITY;
EAPI extern Ecore_X_Atom ECORE_X_ATOM_NET_WM_WINDOW_SHADOW;
EAPI extern Ecore_X_Atom ECORE_X_ATOM_NET_WM_WINDOW_SHADE;
EAPI extern Ecore_X_Atom ECORE_X_ATOM_NET_STARTUP_INFO_BEGIN;
EAPI extern Ecore_X_Atom ECORE_X_ATOM_NET_STARTUP_INFO;
EAPI extern Ecore_X_Atom ECORE_X_ATOM_SELECTION_TARGETS;
EAPI extern Ecore_X_Atom ECORE_X_ATOM_SELECTION_PRIMARY;
EAPI extern Ecore_X_Atom ECORE_X_ATOM_SELECTION_SECONDARY;
EAPI extern Ecore_X_Atom ECORE_X_ATOM_SELECTION_CLIPBOARD;
EAPI extern Ecore_X_Atom ECORE_X_ATOM_SELECTION_PROP_PRIMARY;
EAPI extern Ecore_X_Atom ECORE_X_ATOM_SELECTION_PROP_SECONDARY;
EAPI extern Ecore_X_Atom ECORE_X_ATOM_SELECTION_PROP_CLIPBOARD;

/* currently E specific virtual keyboard extension, aim to submit to netwm spec
 * later */

EAPI extern Ecore_X_Atom ECORE_X_ATOM_E_VIRTUAL_KEYBOARD;
EAPI extern Ecore_X_Atom ECORE_X_ATOM_E_VIRTUAL_KEYBOARD_STATE;
EAPI extern Ecore_X_Atom ECORE_X_ATOM_E_VIRTUAL_KEYBOARD_ON;
EAPI extern Ecore_X_Atom ECORE_X_ATOM_E_VIRTUAL_KEYBOARD_OFF;
EAPI extern Ecore_X_Atom ECORE_X_ATOM_E_VIRTUAL_KEYBOARD_ALPHA;
EAPI extern Ecore_X_Atom ECORE_X_ATOM_E_VIRTUAL_KEYBOARD_NUMERIC;
EAPI extern Ecore_X_Atom ECORE_X_ATOM_E_VIRTUAL_KEYBOARD_PIN;
EAPI extern Ecore_X_Atom ECORE_X_ATOM_E_VIRTUAL_KEYBOARD_PHONE_NUMBER;
EAPI extern Ecore_X_Atom ECORE_X_ATOM_E_VIRTUAL_KEYBOARD_HEX;
EAPI extern Ecore_X_Atom ECORE_X_ATOM_E_VIRTUAL_KEYBOARD_TERMINAL;
EAPI extern Ecore_X_Atom ECORE_X_ATOM_E_VIRTUAL_KEYBOARD_PASSWORD;
EAPI extern Ecore_X_Atom ECORE_X_ATOM_E_VIRTUAL_KEYBOARD_IP;
EAPI extern Ecore_X_Atom ECORE_X_ATOM_E_VIRTUAL_KEYBOARD_HOST;
EAPI extern Ecore_X_Atom ECORE_X_ATOM_E_VIRTUAL_KEYBOARD_FILE;
EAPI extern Ecore_X_Atom ECORE_X_ATOM_E_VIRTUAL_KEYBOARD_URL;
EAPI extern Ecore_X_Atom ECORE_X_ATOM_E_VIRTUAL_KEYBOARD_KEYPAD;
EAPI extern Ecore_X_Atom ECORE_X_ATOM_E_VIRTUAL_KEYBOARD_J2ME;

/* Illume specific atoms */
EAPI extern Ecore_X_Atom ECORE_X_ATOM_E_ILLUME_ZONE;
EAPI extern Ecore_X_Atom ECORE_X_ATOM_E_ILLUME_ZONE_LIST;
EAPI extern Ecore_X_Atom ECORE_X_ATOM_E_ILLUME_CONFORMANT;
EAPI extern Ecore_X_Atom ECORE_X_ATOM_E_ILLUME_MODE;
EAPI extern Ecore_X_Atom ECORE_X_ATOM_E_ILLUME_MODE_SINGLE;
EAPI extern Ecore_X_Atom ECORE_X_ATOM_E_ILLUME_MODE_DUAL_TOP;
EAPI extern Ecore_X_Atom ECORE_X_ATOM_E_ILLUME_MODE_DUAL_LEFT;
EAPI extern Ecore_X_Atom ECORE_X_ATOM_E_ILLUME_FOCUS_BACK;
EAPI extern Ecore_X_Atom ECORE_X_ATOM_E_ILLUME_FOCUS_FORWARD;
EAPI extern Ecore_X_Atom ECORE_X_ATOM_E_ILLUME_FOCUS_HOME;
EAPI extern Ecore_X_Atom ECORE_X_ATOM_E_ILLUME_HOME_NEW;
EAPI extern Ecore_X_Atom ECORE_X_ATOM_E_ILLUME_HOME_DEL;
EAPI extern Ecore_X_Atom ECORE_X_ATOM_E_ILLUME_CLOSE;
EAPI extern Ecore_X_Atom ECORE_X_ATOM_E_ILLUME_DRAG;
EAPI extern Ecore_X_Atom ECORE_X_ATOM_E_ILLUME_DRAG_LOCKED;
EAPI extern Ecore_X_Atom ECORE_X_ATOM_E_ILLUME_DRAG_START;
EAPI extern Ecore_X_Atom ECORE_X_ATOM_E_ILLUME_DRAG_END;
EAPI extern Ecore_X_Atom ECORE_X_ATOM_E_ILLUME_INDICATOR_GEOMETRY;
EAPI extern Ecore_X_Atom ECORE_X_ATOM_E_ILLUME_SOFTKEY_GEOMETRY;
EAPI extern Ecore_X_Atom ECORE_X_ATOM_E_ILLUME_KEYBOARD_GEOMETRY;
EAPI extern Ecore_X_Atom ECORE_X_ATOM_E_ILLUME_QUICKPANEL;
EAPI extern Ecore_X_Atom ECORE_X_ATOM_E_ILLUME_QUICKPANEL_STATE;
EAPI extern Ecore_X_Atom ECORE_X_ATOM_E_ILLUME_QUICKPANEL_STATE_TOGGLE;
EAPI extern Ecore_X_Atom ECORE_X_ATOM_E_ILLUME_QUICKPANEL_ON;
EAPI extern Ecore_X_Atom ECORE_X_ATOM_E_ILLUME_QUICKPANEL_OFF;
EAPI extern Ecore_X_Atom ECORE_X_ATOM_E_ILLUME_QUICKPANEL_PRIORITY_MAJOR;
EAPI extern Ecore_X_Atom ECORE_X_ATOM_E_ILLUME_QUICKPANEL_PRIORITY_MINOR;
EAPI extern Ecore_X_Atom ECORE_X_ATOM_E_ILLUME_QUICKPANEL_ZONE;
EAPI extern Ecore_X_Atom ECORE_X_ATOM_E_ILLUME_QUICKPANEL_POSITION_UPDATE;
EAPI extern Ecore_X_Atom ECORE_X_ATOM_E_ILLUME_INDICATOR_STATE;
EAPI extern Ecore_X_Atom ECORE_X_ATOM_E_ILLUME_INDICATOR_ON;
EAPI extern Ecore_X_Atom ECORE_X_ATOM_E_ILLUME_INDICATOR_OFF;
EAPI extern Ecore_X_Atom ECORE_X_ATOM_E_ILLUME_INDICATOR_OPACITY_MODE;
EAPI extern Ecore_X_Atom ECORE_X_ATOM_E_ILLUME_INDICATOR_OPAQUE;
EAPI extern Ecore_X_Atom ECORE_X_ATOM_E_ILLUME_INDICATOR_TRANSLUCENT;
EAPI extern Ecore_X_Atom ECORE_X_ATOM_E_ILLUME_INDICATOR_TRANSPARENT;
EAPI extern Ecore_X_Atom ECORE_X_ATOM_E_ILLUME_INDICATOR_TYPE_MODE;
EAPI extern Ecore_X_Atom ECORE_X_ATOM_E_ILLUME_INDICATOR_TYPE_1;
EAPI extern Ecore_X_Atom ECORE_X_ATOM_E_ILLUME_INDICATOR_TYPE_2;
EAPI extern Ecore_X_Atom ECORE_X_ATOM_E_ILLUME_INDICATOR_TYPE_3;
EAPI extern Ecore_X_Atom ECORE_X_ATOM_E_ILLUME_ROTATE_WINDOW_AVAILABLE_ANGLE;
EAPI extern Ecore_X_Atom ECORE_X_ATOM_E_ILLUME_ROTATE_WINDOW_ANGLE;
EAPI extern Ecore_X_Atom ECORE_X_ATOM_E_ILLUME_ROTATE_ROOT_ANGLE;
EAPI extern Ecore_X_Atom ECORE_X_ATOM_E_ILLUME_CLIPBOARD_STATE;
EAPI extern Ecore_X_Atom ECORE_X_ATOM_E_ILLUME_CLIPBOARD_ON;
EAPI extern Ecore_X_Atom ECORE_X_ATOM_E_ILLUME_CLIPBOARD_OFF;
EAPI extern Ecore_X_Atom ECORE_X_ATOM_E_ILLUME_CLIPBOARD_GEOMETRY;
EAPI extern Ecore_X_Atom ECORE_X_ATOM_E_ILLUME_WINDOW_STATE;
EAPI extern Ecore_X_Atom ECORE_X_ATOM_E_ILLUME_WINDOW_STATE_NORMAL;
EAPI extern Ecore_X_Atom ECORE_X_ATOM_E_ILLUME_WINDOW_STATE_FLOATING;
EAPI extern Ecore_X_Atom ECORE_X_ATOM_E_ILLUME_ACCESS_CONTROL;
EAPI extern Ecore_X_Atom ECORE_X_ATOM_E_ILLUME_ACCESS_ACTION_NEXT;
EAPI extern Ecore_X_Atom ECORE_X_ATOM_E_ILLUME_ACCESS_ACTION_PREV;
EAPI extern Ecore_X_Atom ECORE_X_ATOM_E_ILLUME_ACCESS_ACTION_ACTIVATE;
EAPI extern Ecore_X_Atom ECORE_X_ATOM_E_ILLUME_ACCESS_ACTION_READ;
EAPI extern Ecore_X_Atom ECORE_X_ATOM_E_ILLUME_ACCESS_ACTION_READ_NEXT;
EAPI extern Ecore_X_Atom ECORE_X_ATOM_E_ILLUME_ACCESS_ACTION_READ_PREV;
EAPI extern Ecore_X_Atom ECORE_X_ATOM_E_ILLUME_ACCESS_ACTION_UP;
EAPI extern Ecore_X_Atom ECORE_X_ATOM_E_ILLUME_ACCESS_ACTION_DOWN;
EAPI extern Ecore_X_Atom ECORE_X_ATOM_E_ILLUME_ACCESS_ACTION_BACK;
EAPI extern Ecore_X_Atom ECORE_X_ATOM_E_ILLUME_ACCESS_ACTION_SCROLL;
EAPI extern Ecore_X_Atom ECORE_X_ATOM_E_ILLUME_ACCESS_ACTION_MOUSE;
EAPI extern Ecore_X_Atom ECORE_X_ATOM_E_ILLUME_ACCESS_ENABLE;
EAPI extern Ecore_X_Atom ECORE_X_ATOM_E_ILLUME_ACCESS_DISABLE;

EAPI extern Ecore_X_Atom ECORE_X_ATOM_E_COMP_SYNC_COUNTER;
EAPI extern Ecore_X_Atom ECORE_X_ATOM_E_COMP_SYNC_DRAW_DONE;
EAPI extern Ecore_X_Atom ECORE_X_ATOM_E_COMP_SYNC_SUPPORTED;
EAPI extern Ecore_X_Atom ECORE_X_ATOM_E_COMP_SYNC_BEGIN;
EAPI extern Ecore_X_Atom ECORE_X_ATOM_E_COMP_SYNC_END;
EAPI extern Ecore_X_Atom ECORE_X_ATOM_E_COMP_SYNC_CANCEL;
EAPI extern Ecore_X_Atom ECORE_X_ATOM_E_COMP_FLUSH;
EAPI extern Ecore_X_Atom ECORE_X_ATOM_E_COMP_DUMP;
EAPI extern Ecore_X_Atom ECORE_X_ATOM_E_COMP_PIXMAP;

EAPI extern Ecore_X_Atom ECORE_X_ATOM_E_VIDEO_PARENT;
EAPI extern Ecore_X_Atom ECORE_X_ATOM_E_VIDEO_POSITION;

/* currently elementary and E specific extension */
/* @deprecated use ECORE_X_ATOM_E_WINDOW_PROFILE */
EAPI extern Ecore_X_Atom ECORE_X_ATOM_E_PROFILE;
/* @deprecated use ECORE_X_ATOM_E_WINDOW_PROFILE_AVAILABLE_LIST */
EAPI extern Ecore_X_Atom ECORE_X_ATOM_E_PROFILE_LIST;
EAPI extern Ecore_X_Atom ECORE_X_ATOM_E_WINDOW_PROFILE_SUPPORTED;
EAPI extern Ecore_X_Atom ECORE_X_ATOM_E_WINDOW_PROFILE;
EAPI extern Ecore_X_Atom ECORE_X_ATOM_E_WINDOW_PROFILE_CHANGE;
EAPI extern Ecore_X_Atom ECORE_X_ATOM_E_WINDOW_PROFILE_AVAILABLE_LIST;
EAPI extern Ecore_X_Atom ECORE_X_ATOM_E_WINDOW_PROFILE_CHANGE_REQUEST;
EAPI extern Ecore_X_Atom ECORE_X_ATOM_E_WINDOW_PROFILE_CHANGE_DONE;

/* for sliding window */
EAPI extern Ecore_X_Atom ECORE_X_ATOM_E_ILLUME_SLIDING_WIN_STATE;
EAPI extern Ecore_X_Atom ECORE_X_ATOM_E_ILLUME_SLIDING_WIN_GEOMETRY;

/* for SDB(Samsung Debug Bridge) */
EAPI extern Ecore_X_Atom ECORE_X_ATOM_SDB_SERVER_CONNECT;
EAPI extern Ecore_X_Atom ECORE_X_ATOM_SDB_SERVER_DISCONNECT;

/* E window rotation extension */
EAPI extern Ecore_X_Atom ECORE_X_ATOM_E_WINDOW_ROTATION_SUPPORTED;
EAPI extern Ecore_X_Atom ECORE_X_ATOM_E_WINDOW_ROTATION_APP_SUPPORTED;
EAPI extern Ecore_X_Atom ECORE_X_ATOM_E_WINDOW_ROTATION_AVAILABLE_LIST;
EAPI extern Ecore_X_Atom ECORE_X_ATOM_E_WINDOW_ROTATION_PREFERRED_ROTATION;
EAPI extern Ecore_X_Atom ECORE_X_ATOM_E_WINDOW_ROTATION_CHANGE_PREPARE;
EAPI extern Ecore_X_Atom ECORE_X_ATOM_E_WINDOW_ROTATION_CHANGE_PREPARE_DONE;
EAPI extern Ecore_X_Atom ECORE_X_ATOM_E_WINDOW_ROTATION_CHANGE_REQUEST;
EAPI extern Ecore_X_Atom ECORE_X_ATOM_E_WINDOW_ROTATION_CHANGE_DONE;
EAPI extern Ecore_X_Atom ECORE_X_ATOM_E_WINDOW_ROTATION_0_GEOMETRY;
EAPI extern Ecore_X_Atom ECORE_X_ATOM_E_WINDOW_ROTATION_90_GEOMETRY;
EAPI extern Ecore_X_Atom ECORE_X_ATOM_E_WINDOW_ROTATION_180_GEOMETRY;
EAPI extern Ecore_X_Atom ECORE_X_ATOM_E_WINDOW_ROTATION_270_GEOMETRY;

EAPI extern Ecore_X_Atom ECORE_X_ATOM_E_VIRTUAL_KEYBOARD_CONTROL_WINDOW;
EAPI extern Ecore_X_Atom ECORE_X_ATOM_E_VIRTUAL_KEYBOARD_ON_PREPARE_REQUEST;
EAPI extern Ecore_X_Atom ECORE_X_ATOM_E_VIRTUAL_KEYBOARD_ON_PREPARE_DONE;
EAPI extern Ecore_X_Atom ECORE_X_ATOM_E_VIRTUAL_KEYBOARD_OFF_PREPARE_REQUEST;
EAPI extern Ecore_X_Atom ECORE_X_ATOM_E_VIRTUAL_KEYBOARD_OFF_PREPARE_DONE;

/* for deiconify approve protcol */
EAPI extern Ecore_X_Atom ECORE_X_ATOM_E_DEICONIFY_APPROVE;
#endif /* _ECORE_X_ATOMS_H */
