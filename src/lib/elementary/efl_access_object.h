#ifndef EFL_ACCESS_H
#define EFL_ACCESS_H

// TIZEN_ONLY(20171114): Add accessibility highlight for toolbar items I7892c9433d675d9ba6618991ae46488060752c3c
//#ifdef EFL_BETA_API_SUPPORT
//

//TIZEN_ONLY(20190922): add name callback, description callback.
/**
 * ATSPI reading information callback
 */
typedef char *(*Efl_Access_Reading_Info_Cb)(void *data, Evas_Object *obj);
//

//TIZEN_ONLY(20170406) Add Elm_Atspi_Reading_Info_Type_Mask
typedef unsigned int Efl_Access_Reading_Info_Type_Mask;
//

//TIZEN_ONLY(20170405) Add gesture method to accessible interface
typedef struct _Efl_Access_Gesture_Info Efl_Access_Gesture_Info;
typedef Eina_Bool (*Efl_Access_Gesture_Cb)(void *data, Efl_Access_Gesture_Info gesture_info, Evas_Object *obj);
#define TIZEN_ONLY
#include "efl_access_legacy.h"
//
#ifdef EFL_EO_API_SUPPORT
#include "efl_access_object.eo.h"
#endif
#ifndef EFL_NOLEGACY_API_SUPPORT
#ifndef TIZEN_ONLY
#include "efl_access_object.eo.legacy.h"
#endif
#endif

// TIZEN_ONLY(20171114): atspi: expose highlight information on atspi
/*
 * Sets a particilar state type for given state set.
 */
#define STATE_TYPE_SET(state_set, type)   (state_set |= ((Efl_Access_State_Set)1 << type))

/**
 * Unsets a particilar state type for given state set.
 */
#define STATE_TYPE_UNSET(state_set, type) (state_set &= ~((Efl_Access_State_Set)1 << type))

/**
 * Gets value of a particilar state type for given state set.
 */
#define STATE_TYPE_GET(state_set, type)   (state_set & ((Efl_Access_State_Set)1 << type))
//

/**
 * @brief Creates access state using given type.
 */
#define ACCESS_STATE(type) ((Efl_Access_State_Set)1 << (type))

//TIZEN_ONLY(20160623): atspi: moved highlight when object is out of screen
enum _Efl_Access_Move_Outed_Type {
     EFL_ACCESS_MOVE_OUTED_NULL,
     EFL_ACCESS_MOVE_OUTED_TOP_LEFT,
     EFL_ACCESS_MOVE_OUTED_BOTTOM_RIGHT,
     EFL_ACCESS_MOVE_OUTED_LAST_DEFINDED
};

typedef enum _Efl_Access_Move_Outed_Type Efl_Access_Move_Outed_Type;
//

/**
 * Free Efl_Access_Attributes_List
 */
EAPI void efl_access_attributes_list_free(Eina_List *list);

#ifdef EFL_EO_API_SUPPORT

/**
 * Emits Accessible 'StateChanged' signal.
 */
#define efl_access_state_changed_signal_emit(obj, tp, nvl) \
   { do { \
      Efl_Access_Event_State_Changed_Data evinfo; \
      evinfo.type = (tp); \
      evinfo.new_value = (nvl); \
      efl_access_object_event_emit(EFL_ACCESS_OBJECT_MIXIN, obj, EFL_ACCESS_OBJECT_EVENT_STATE_CHANGED, (void*)&evinfo); \
   } while(0); }

/**
 * Emits Accessible 'BoundsChanged' signal.
 */
#define efl_access_bounds_changed_signal_emit(obj, x, y, width, height) \
   do { \
         Efl_Access_Event_Geometry_Changed_Data evinfo = { x, y, width, height }; \
         efl_access_object_event_emit(EFL_ACCESS_OBJECT_MIXIN, obj, EFL_ACCESS_OBJECT_EVENT_BOUNDS_CHANGED, (void*)&evinfo); \
   } while(0);

//TIZEN_ONLY(20160623): atspi: moved highlight when object is out of screen
/**
 * Emits ATSPI 'MoveOuted' dbus signal.
 */
#define efl_access_move_outed_signal_emit(obj, type) \
   do { \
         Efl_Access_Move_Outed_Type evinfo = type; \
         efl_access_object_event_emit(EFL_ACCESS_OBJECT_MIXIN, obj, EFL_ACCESS_OBJECT_EVENT_MOVE_OUTED, (void*)&evinfo); \
   } while(0);
//

/**
 * Emits Accessible 'PropertyChanged' signal for 'Name' property.
 */
#define efl_access_i18n_name_changed_signal_emit(obj) \
   efl_access_object_event_emit(EFL_ACCESS_OBJECT_MIXIN, obj, EFL_ACCESS_OBJECT_EVENT_PROPERTY_CHANGED, "i18n_name");

/**
 * Emits Accessible 'PropertyChanged' signal for 'Description' property.
 */
#define efl_access_description_changed_signal_emit(obj) \
   efl_access_object_event_emit(EFL_ACCESS_OBJECT_MIXIN, obj, EFL_ACCESS_OBJECT_EVENT_PROPERTY_CHANGED, "description");

/**
 * Emits Accessible 'PropertyChanged' signal for 'Parent' property.
 */
#define efl_access_parent_changed_signal_emit(obj) \
   efl_access_object_event_emit(EFL_ACCESS_OBJECT_MIXIN, obj, EFL_ACCESS_OBJECT_EVENT_PROPERTY_CHANGED, "parent");

/**
 * Emits Accessible 'PropertyChanged' signal for 'Role' property.
 */
#define efl_access_role_changed_signal_emit(obj) \
   efl_access_object_event_emit(EFL_ACCESS_OBJECT_MIXIN, obj, EFL_ACCESS_OBJECT_EVENT_PROPERTY_CHANGED, "role");

/**
 * Emits Accessible 'PropertyChanged' signal for 'Value' property.
 */
#define efl_access_value_changed_signal_emit(obj) \
   efl_access_object_event_emit(EFL_ACCESS_OBJECT_MIXIN, obj, EFL_ACCESS_OBJECT_EVENT_PROPERTY_CHANGED, "value");

/**
 * Emits Accessible 'ChildrenChanged' signal with added child as argument.
 */
//TIZEN_ONLY(20170923) Block event which is not interested in
#define efl_access_children_changed_added_signal_emit(obj, child) ((void)0)/* Empty */
/*
#define efl_access_children_changed_added_signal_emit(obj, child) \
   do { \
      Efl_Access_Event_Children_Changed_Data atspi_data = { EINA_TRUE, child }; \
   efl_access_object_event_emit(EFL_ACCESS_OBJECT_MIXIN, obj, EFL_ACCESS_OBJECT_EVENT_CHILDREN_CHANGED, &atspi_data); \
   } while(0);
*/
//

/**
 * Emits Accessible 'ChildrenChanged' signal with deleted child as argument.
 */
//TIZEN_ONLY(20170923) Block event which is not interested in
#define efl_access_children_changed_del_signal_emit(obj, child) ((void)0)/* Empty */
/*
#define efl_access_children_changed_del_signal_emit(obj, child) \
   do { \
      Efl_Access_Event_Children_Changed_Data atspi_data = { EINA_FALSE, child }; \
      efl_access_object_event_emit(EFL_ACCESS_OBJECT_MIXIN, obj, EFL_ACCESS_OBJECT_EVENT_CHILDREN_CHANGED, &atspi_data); \
   } while(0);
*/
//

/**
 * Emits Accessible 'ActiveDescendantChanged' signal.
 */
#define efl_access_active_descendant_changed_signal_emit(obj, child) \
   efl_access_object_event_emit(EFL_ACCESS_OBJECT_MIXIN, obj, EFL_ACCESS_OBJECT_EVENT_ACTIVE_DESCENDANT_CHANGED, child);

/**
 * Emits Accessible 'VisibleDataChanged' signal.
 */
#define efl_access_visible_data_changed_signal_emit(obj) \
   efl_access_object_event_emit(EFL_ACCESS_OBJECT_MIXIN, obj, EFL_ACCESS_OBJECT_EVENT_VISIBLE_DATA_CHANGED, NULL);

/**
 * Emits Accessible 'AddAccessible' signal.
 */
//TIZEN_ONLY(20170923) Block event which is not interested in
#define efl_access_added(obj) ((void)0)/* Empty */
/*
#define efl_access_added(obj) \
   efl_access_object_event_emit(EFL_ACCESS_OBJECT_MIXIN, obj, EFL_ACCESS_OBJECT_EVENT_ADDED, NULL);
*/
//

/**
 * Emits Accessible 'RemoveAccessible' signal.
 */
//TIZEN_ONLY(20170923) Block event which is not interested in
#define efl_access_removed(obj) ((void)0)/* Empty */
/*
#define efl_access_removed(obj) \
   efl_access_object_event_emit(EFL_ACCESS_OBJECT_MIXIN, obj, EFL_ACCESS_OBJECT_EVENT_REMOVED, NULL);
*/
//

#endif

#endif
// TIZEN_ONLY(20171114): Add accessibility highlight for toolbar items I7892c9433d675d9ba6618991ae46488060752c3c
//#endif
//
