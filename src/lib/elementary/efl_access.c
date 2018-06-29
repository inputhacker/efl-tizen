#ifdef HAVE_CONFIG_H
  #include "elementary_config.h"
#endif

#define EFL_ACCESS_PROTECTED

#include <Elementary.h>
#include "elm_widget.h"
#include "elm_priv.h"

const char* Access_Name[] = {
    "invalid",
    "accelerator label",
    "alert",
    "animation",
    "arrow",
    "calendar",
    "canvas",
    "check box",
    "check menu item",
    "color chooser",
    "column header",
    "combo box",
    "dateeditor",
    "desktop icon",
    "desktop frame",
    "dial",
    "dialog",
    "directory pane",
    "drawing area",
    "file chooser",
    "filler",
    "focus traversable",
    "font chooser",
    "frame",
    "glass pane",
    "html container",
    "icon",
    "image",
    "internal frame",
    "label",
    "layered pane",
    "list",
    "list item",
    "menu",
    "menu bar",
    "menu item",
    "option pane",
    "page tab",
    "page tab list",
    "panel",
    "password text",
    "popup menu",
    "progress bar",
    "push button",
    "radio button",
    "radio menu item",
    "root pane",
    "row header",
    "scroll bar",
    "scroll pane",
    "separator",
    "slider",
    "spin button",
    "split pane",
    "status bar",
    "table",
    "table cell",
    "table column header",
    "table row header",
    "tearoff menu item",
    "terminal",
    "text",
    "toggle button",
    "tool bar",
    "tool tip",
    "tree",
    "tree table",
    "unknown",
    "viewport",
    "window",
    "extended",
    "header",
    "footer",
    "paragraph",
    "ruler",
    "application",
    "autocomplete",
    "editbar",
    "embedded",
    "edit field",
    "chart",
    "caption",
    "document frame",
    "heading",
    "page",
    "section",
    "redundant object",
    "form",
    "link",
    "input method window",
    "table row",
    "tree item",
    "document spreadsheet",
    "document presentation",
    "document text",
    "document web",
    "document email",
    "comment",
    "list box",
    "grouping",
    "image map",
    "notification",
    "info bar",
    "last defined"
};

struct _Efl_Access_Event_Handler
{
   Efl_Event_Cb cb;
   void *data;
};

//TIZEN_ONLY(20190922): add name callback, description callback.
struct _Efl_Access_Reading_Info_Cb_Item
{
   Efl_Access_Reading_Info_Cb cb;
   const void *data;
};
typedef struct _Efl_Access_Reading_Info_Cb_Item Efl_Access_Reading_Info_Cb_Item;
//

//TIZEN_ONLY(20170405) Add gesture method to accessible interface
struct _Efl_Access_Gesture_Cb_Item
{
   Efl_Access_Gesture_Cb cb;
   const void *data;
};
typedef struct _Efl_Access_Gesture_Cb_Item Efl_Access_Gesture_Cb_Item;

struct _Elm_Atspi_Gesture_Cb_Item
{
   Elm_Atspi_Gesture_Cb cb;
   const void *data;
};
typedef struct _Elm_Atspi_Gesture_Cb_Item Elm_Atspi_Gesture_Cb_Item;
//

struct _Efl_Access_Data
{
   Efl_Access_Relation_Set relations;
   //TIZEN_ONLY(20190922): add name callback, description callback.
   Efl_Access_Reading_Info_Cb_Item name_cb_item;
   Efl_Access_Reading_Info_Cb_Item description_cb_item;
   //
   //TIZEN_ONLY(20170405) Add gesture method to accessible interface
   Efl_Access_Gesture_Cb_Item gesture_cb_item;
   Elm_Atspi_Gesture_Cb_Item legacy_gesture_cb_item;
   //
   Eina_List     *attr_list;
   const char    *name;
   const char    *description;
   const char    *translation_domain;
   Efl_Access_Role role;
   Efl_Access_Reading_Info_Type_Mask reading_info;

   Eo            *forced_parent;
};

typedef struct _Efl_Access_Data Efl_Access_Data;


static Eina_List *global_callbacks;
static Eo *root;

EOLIAN static int
_efl_access_index_in_parent_get(Eo *obj, Efl_Access_Data *pd EINA_UNUSED)
{
   Eina_List *l, *children = NULL;
   Eo *chld, *parent = NULL;
   int ret = 0;

   parent = efl_access_parent_get(obj);
   if (!parent) return -1;

   children = efl_access_children_get(parent);
   if (!children) return -1;

   EINA_LIST_FOREACH(children, l, chld)
     {
       if (obj == chld)
         break;
       ret++;
     }
   if (ret == (int)eina_list_count(children))
     {
        ERR("Object %s not present in its AT-SPI parents (%s) children list! This should never happen.", efl_class_name_get(efl_class_get(obj)), efl_class_name_get(efl_class_get(parent)));
        ret = -1;
     }
   eina_list_free(children);
   return ret;
}

EOLIAN static Efl_Access *
_efl_access_parent_get(Eo *obj EINA_UNUSED, Efl_Access_Data *pd EINA_UNUSED)
{
   Eo *parent = obj;

   if (pd->forced_parent)
     return pd->forced_parent;

   do {
      parent = efl_parent_get(obj);
      if (efl_isa(parent, EFL_ACCESS_MIXIN))
        break;
   } while (parent);

   return efl_isa(parent, EFL_ACCESS_MIXIN) ? parent : NULL;
}

EOLIAN static Efl_Access *
_efl_access_forced_parent_get(Eo *obj EINA_UNUSED, Efl_Access_Data *pd EINA_UNUSED)
{
   return pd->forced_parent;
}

EOLIAN static void
_efl_access_parent_set(Eo *obj, Efl_Access_Data *pd EINA_UNUSED, Efl_Access *new_parent EINA_UNUSED)
{
   if (efl_isa(new_parent, EFL_ACCESS_MIXIN))
     pd->forced_parent = new_parent;
}

EOLIAN Eina_List*
_efl_access_attributes_get(Eo *obj EINA_UNUSED, Efl_Access_Data *pd EINA_UNUSED)
{
   Eina_List *attr_list = NULL;
   if (pd->attr_list)
     {
        Eina_List *l = NULL;
        Efl_Access_Attribute *t_attr = NULL;
        EINA_LIST_FOREACH(pd->attr_list, l, t_attr)
          {
             Efl_Access_Attribute *attr = calloc(1, sizeof(Efl_Access_Attribute));
             if (!attr)
                  return attr_list;

             attr->key = eina_stringshare_add(t_attr->key);
             attr->value = eina_stringshare_add(t_attr->value);
             attr_list = eina_list_append(attr_list, attr);
          }
     }
   return attr_list;
}

EOLIAN static void
_efl_access_attribute_append(Eo *obj EINA_UNUSED, Efl_Access_Data *pd EINA_UNUSED, const char *key, const char *value)
{
   Eina_List *l;
   Efl_Access_Attribute *attr = NULL;

   if (!key || !value) return;

   /* Check existing attributes has this key */
   EINA_LIST_FOREACH(pd->attr_list, l, attr)
     {
        if (!strcmp((const char *)attr->key, key))
          {
             eina_stringshare_replace(&attr->value, value);
             return;
          }
     }

   /* Add new attribute */
   attr = calloc(1, sizeof(Efl_Access_Attribute));
   if (!attr) return;

   attr->key = eina_stringshare_add(key);
   attr->value = eina_stringshare_add(value);
   pd->attr_list = eina_list_append(pd->attr_list, attr);
}

EOLIAN static void _efl_access_attributes_clear(Eo *obj EINA_UNUSED, Efl_Access_Data *pd)
{
   if (!pd->attr_list) return;
   Efl_Access_Attribute *attr;
   EINA_LIST_FREE(pd->attr_list, attr)
     {
        eina_stringshare_del(attr->key);
        eina_stringshare_del(attr->value);
        free(attr);
     }
   pd->attr_list = NULL;
}

EOLIAN static void
_efl_access_reading_info_type_set(Eo *obj, Efl_Access_Data *pd, Efl_Access_Reading_Info_Type_Mask reading_info)
{
   Eina_Strbuf *buf = NULL;
   pd->reading_info = reading_info;
   buf = eina_strbuf_new();
   eina_strbuf_reset(buf);
   if (reading_info & (EFL_ACCESS_READING_INFO_TYPE_NAME))
     {
        eina_strbuf_append(buf, "name");
        eina_strbuf_append_char(buf, '|');
     }
   if (reading_info & (EFL_ACCESS_READING_INFO_TYPE_ROLE))
     {
        eina_strbuf_append(buf, "role");
        eina_strbuf_append_char(buf, '|');
     }
   if (reading_info & (EFL_ACCESS_READING_INFO_TYPE_DESCRIPTION))
     {
        eina_strbuf_append(buf, "description");
        eina_strbuf_append_char(buf, '|');
     }
   if (reading_info & (EFL_ACCESS_READING_INFO_TYPE_STATE))
     {
        eina_strbuf_append(buf, "state");
     }
   efl_access_attribute_append(obj, "reading_info_type", eina_strbuf_string_get(buf));
   eina_strbuf_free(buf);
}

EOLIAN Efl_Access_Reading_Info_Type_Mask
_efl_access_reading_info_type_get(Eo *obj EINA_UNUSED, Efl_Access_Data *pd)
{
   return pd->reading_info;
}

EOLIAN static Efl_Access_Role
_efl_access_role_get(Eo *obj EINA_UNUSED, Efl_Access_Data *pd EINA_UNUSED)
{
   return pd->role;
}

EOLIAN static void
_efl_access_role_set(Eo *obj EINA_UNUSED, Efl_Access_Data *pd, Efl_Access_Role role)
{
   if (pd->role != role)
     {
        pd->role = role;
        //TIZEN_ONLY(20160708) Do not send role changed signal - 10000 list items send 10000 IPC.
        //efl_access_role_changed_signal_emit(obj);
        //
     }
}

EOLIAN const char *
_efl_access_role_name_get(Eo *obj EINA_UNUSED, Efl_Access_Data *pd EINA_UNUSED)
{
   Efl_Access_Role role;

   role = efl_access_role_get(obj);

   return role > EFL_ACCESS_ROLE_LAST_DEFINED ? "" : Access_Name[role];
}

EOLIAN const char *
_efl_access_name_get(Eo *obj EINA_UNUSED, Efl_Access_Data *pd)
{
   //TIZEN_ONLY(20190922): add name callback, description callback.
   char *ret = NULL;
   if (pd->name_cb_item.cb)
     ret = pd->name_cb_item.cb((void *)pd->name_cb_item.data, (Evas_Object *)obj);
   if (ret)
     {
        eina_stringshare_replace(&pd->translation_domain, NULL);
        pd->translation_domain = NULL;
        eina_stringshare_replace(&pd->name, ret);
        free(ret);
     }
   //
#ifdef ENABLE_NLS
   if (pd->translation_domain)
     return dgettext(pd->translation_domain, pd->name);
#endif
   return pd->name;
}

EOLIAN static void
_efl_access_name_set(Eo *obj EINA_UNUSED, Efl_Access_Data *pd, const char *val)
{
   eina_stringshare_replace(&pd->name, val);
}

//TIZEN_ONLY(20190922): add name callback, description callback.
EOLIAN static void
_efl_access_name_cb_set(Eo *obj EINA_UNUSED, Efl_Access_Data *pd, Efl_Access_Reading_Info_Cb name_cb, const void *data)
{
   pd->name_cb_item.cb = name_cb;
   pd->name_cb_item.data = data;
}
//

const char * _efl_access_description_get(Eo *obj EINA_UNUSED, Efl_Access_Data *pd)
{
   //TIZEN_ONLY(20190922): add name callback, description callback.
   char *ret = NULL;
   if (pd->description_cb_item.cb)
     ret = pd->description_cb_item.cb((void *)pd->description_cb_item.data, (Evas_Object *)obj);
   if (ret)
     {
        eina_stringshare_replace(&pd->translation_domain, NULL);
        pd->translation_domain = NULL;
        eina_stringshare_replace(&pd->description, ret);
        free(ret);
     }
   //
#ifdef ENABLE_NLS
   if (pd->translation_domain)
      return dgettext(pd->translation_domain, pd->description);
#endif
   return pd->description;
}

EOLIAN static void
_efl_access_description_set(Eo *obj EINA_UNUSED, Efl_Access_Data *pd, const char *val)
{
   eina_stringshare_replace(&pd->description, val);
}

//TIZEN_ONLY(20190922): add name callback, description callback.
EOLIAN static void
_efl_access_description_cb_set(Eo *obj EINA_UNUSED, Efl_Access_Data *pd, Efl_Access_Reading_Info_Cb description_cb, const void *data)
{
   pd->description_cb_item.cb = description_cb;
   pd->description_cb_item.data = data;
}
//

//TIZEN_ONLY(20170405) Add gesture method to accessible interface
EOLIAN static void
_efl_access_gesture_cb_set(Eo *obj EINA_UNUSED, Efl_Access_Data *pd, Efl_Access_Gesture_Cb gesture_cb, const void *data)
{
   pd->gesture_cb_item.cb = gesture_cb;
   pd->gesture_cb_item.data = data;
}

EOLIAN static Eina_Bool
_efl_access_gesture_do(Eo *obj EINA_UNUSED, Efl_Access_Data *pd, Efl_Access_Gesture_Info gesture_info)
{
   Eina_Bool ret = EINA_FALSE;
   if (pd->gesture_cb_item.cb)
     ret = pd->gesture_cb_item.cb((void *)pd->gesture_cb_item.data, gesture_info, (Evas_Object *)obj);

   if (!ret && pd->legacy_gesture_cb_item.cb)
     {
        Elm_Atspi_Gesture_Info legacy_gesture_info;
        legacy_gesture_info.type = (Elm_Atspi_Gesture_Type)gesture_info.type;
        legacy_gesture_info.x_beg = gesture_info.x_beg;
        legacy_gesture_info.y_beg = gesture_info.y_beg;
        legacy_gesture_info.x_end = gesture_info.x_end;
        legacy_gesture_info.y_end = gesture_info.y_end;
        legacy_gesture_info.state = (Elm_Atspi_Gesture_State)gesture_info.state;
        legacy_gesture_info.event_time = gesture_info.event_time;
        ret = pd->legacy_gesture_cb_item.cb((void *)pd->legacy_gesture_cb_item.data, legacy_gesture_info, (Evas_Object *)obj);
     }

   return ret;
}
//

EOLIAN static const char *
_efl_access_localized_role_name_get(Eo *obj EINA_UNUSED, Efl_Access_Data *pd EINA_UNUSED)
{
   const char *ret = NULL;
   ret = efl_access_role_name_get(obj);
#ifdef ENABLE_NLS
   ret = gettext(ret);
#endif
   return ret;
}

EOLIAN static Eina_List *
_efl_access_children_get(Eo *obj EINA_UNUSED, Efl_Access_Data *pd EINA_UNUSED)
{
   Eina_List *children = NULL;
   Eina_Iterator *iter = NULL;
   Eo *chld;

   // By default use Efl_Object object hierarchy
   iter = efl_children_iterator_new(obj);
   if (!iter) return NULL;

   EINA_ITERATOR_FOREACH(iter, chld)
     {
        if (efl_isa(chld, EFL_ACCESS_MIXIN))
          children = eina_list_append(children, chld);
     }
   eina_iterator_free(iter);

   return children;
}

EOLIAN static Efl_Access_State_Set
_efl_access_state_set_get(Eo *obj EINA_UNUSED, Efl_Access_Data *pd EINA_UNUSED)
{
   return 0;
}

//TIZEN_ONLY(20160726): add API elm_atspi_accessible_can_highlight_set/get
EOLIAN static void
_efl_access_can_highlight_set(Eo *obj EINA_UNUSED, Efl_Access_Data *pd EINA_UNUSED, Eina_Bool can_highlight EINA_UNUSED)
{
   WRN("The %s object does not implement the \"can_highlight_set\" function.",
       efl_class_name_get(efl_class_get(obj)));
}

EOLIAN static Eina_Bool
_efl_access_can_highlight_get(Eo *obj EINA_UNUSED, Efl_Access_Data *pd EINA_UNUSED)
{
   WRN("The %s object does not implement the \"can_highlight_get\" function.",
       efl_class_name_get(efl_class_get(obj)));
   return EINA_TRUE;
}
//

EOLIAN Efl_Access_Relation_Set
_efl_access_relation_set_get(Eo *obj, Efl_Access_Data *pd EINA_UNUSED)
{
   //TIZEN_ONLY(20171115) Fixed the bugs and warnings in atspi relationship APIS
   //return efl_access_relation_set_clone(pd->relations);
   WRN("The %s object does not implement the \"accessible_relation_set\" function.",
       efl_class_name_get(efl_class_get(obj)));
   return NULL;
   //
}

EAPI void efl_access_attributes_list_free(Eina_List *list)
{
   Efl_Access_Attribute *attr;
   EINA_LIST_FREE(list, attr)
     {
        eina_stringshare_del(attr->key);
        eina_stringshare_del(attr->value);
        free(attr);
     }
}

EOLIAN void
_efl_access_event_emit(Eo *class EINA_UNUSED, void *pd EINA_UNUSED, Eo *accessible, const Efl_Event_Description *event, void *event_info)
{
   Eina_List *l;
   Efl_Access_Event_Handler *hdl;

   if (!accessible || !event || !efl_isa(accessible, EFL_ACCESS_MIXIN))
     {
        CRI("Invalid parameters, event: %s, obj: %s", event ? event->name : "NULL", accessible ? efl_class_name_get(accessible) : "NULL");
        return;
     }

   Efl_Event ev;
   ev.object = accessible;
   ev.desc = event;
   ev.info = event_info;
   EINA_LIST_FOREACH(global_callbacks, l, hdl)
     {
        if (hdl->cb)
          hdl->cb(hdl->data, &ev);
     }
}

EOLIAN Efl_Access_Event_Handler *
_efl_access_event_handler_add(Eo *class EINA_UNUSED, void *pd EINA_UNUSED, Efl_Event_Cb cb, void *data)
{
   Efl_Access_Event_Handler *ret = calloc(1, sizeof(Efl_Access_Event_Handler));

   ret->cb = cb;
   ret->data = data;

   global_callbacks = eina_list_append(global_callbacks, ret);

   return ret;
}

EOLIAN void
_efl_access_event_handler_del(Eo *class EINA_UNUSED, void *pd EINA_UNUSED, Efl_Access_Event_Handler *handler)
{
   Eina_List *l, *l2;
   Efl_Access_Event_Handler *hdl;
   EINA_LIST_FOREACH_SAFE(global_callbacks, l, l2, hdl)
     {
        if (hdl == handler)
          {
             global_callbacks = eina_list_remove_list(global_callbacks, l);
             free(hdl);
             break;
          }
     }
}

EOLIAN void
_efl_access_translation_domain_set(Eo *obj EINA_UNUSED, Efl_Access_Data *pd, const char *domain)
{
   eina_stringshare_replace(&pd->translation_domain, domain);
}


EOLIAN const char*
_efl_access_translation_domain_get(Eo *obj EINA_UNUSED, Efl_Access_Data *pd)
{
   return pd->translation_domain;
}

EAPI void
efl_access_relation_free(Efl_Access_Relation *relation)
{
   eina_list_free(relation->objects);
   free(relation);
}

EAPI Efl_Access_Relation *
efl_access_relation_clone(const Efl_Access_Relation *relation)
{
   Efl_Access_Relation *ret = calloc(1, sizeof(Efl_Access_Relation));
   if (!ret) return NULL;

   ret->type = relation->type;
   ret->objects = eina_list_clone(relation->objects);
   return ret;
}

static void
_on_rel_obj_del(void *data, const Efl_Event *event)
{
   Efl_Access_Relation_Set *set = data;
   Efl_Access_Relation *rel;
   Eina_List *l, *l2, *p, *p2;
   Eo *rel_obj;

   EINA_LIST_FOREACH_SAFE(*set, l, l2, rel)
     {
        EINA_LIST_FOREACH_SAFE(rel->objects, p, p2, rel_obj)
          {
          if (rel_obj == event->object)
               rel->objects = eina_list_remove_list(rel->objects, p);
          }
        if (!rel->objects)
          {
             *set = eina_list_remove_list(*set, l);
             free(rel);
          }
     }
}

EAPI Eina_Bool
efl_access_relation_set_relation_append(Efl_Access_Relation_Set *set, Efl_Access_Relation_Type type, const Eo *rel_obj)
{
   Efl_Access_Relation *rel;
   Eina_List *l, *ll;

   if (!efl_isa(rel_obj, EFL_ACCESS_MIXIN))
     return EINA_FALSE;

   EINA_LIST_FOREACH(*set, l, rel)
     {
        if (rel->type == type)
          {
             ll = eina_list_data_find_list(rel->objects, rel_obj);
             if (!ll)
               {
                  rel->objects = eina_list_append(rel->objects, rel_obj);
                  efl_event_callback_add((Eo *) rel_obj, EFL_EVENT_DEL, _on_rel_obj_del, set);
               }
             else
               {
                  rel->objects = eina_list_demote_list(rel->objects, ll);
               }
             return EINA_TRUE;
          }
     }

   rel = calloc(1, sizeof(Efl_Access_Relation));
   if (!rel) return EINA_FALSE;

   rel->type = type;
   rel->objects = eina_list_append(rel->objects, rel_obj);
   *set = eina_list_append(*set, rel);

   efl_event_callback_add((Eo *) rel_obj, EFL_EVENT_DEL, _on_rel_obj_del, set);
   return EINA_TRUE;
}

EAPI void
efl_access_relation_set_relation_remove(Efl_Access_Relation_Set *set, Efl_Access_Relation_Type type, const Eo *rel_obj)
{
   Eina_List *l;
   Efl_Access_Relation *rel;

   EINA_LIST_FOREACH(*set, l, rel)
     {
        if (rel->type == type)
          {
             if (eina_list_data_find(rel->objects, rel_obj))
               {
                  efl_event_callback_del((Eo *) rel_obj, EFL_EVENT_DEL, _on_rel_obj_del, set);
                  rel->objects = eina_list_remove(rel->objects, rel_obj);
               }
             if (!rel->objects)
               {
                  *set = eina_list_remove(*set, rel);
                  efl_access_relation_free(rel);
               }
             return;
          }
     }
}

EAPI void
efl_access_relation_set_relation_type_remove(Efl_Access_Relation_Set *set, Efl_Access_Relation_Type type)
{
   Eina_List *l;
   Efl_Access_Relation *rel;
   Eo *obj;

   EINA_LIST_FOREACH(*set, l, rel)
     {
        if (rel->type == type)
          {
             EINA_LIST_FOREACH(rel->objects, l, obj)
                efl_event_callback_del(obj, EFL_EVENT_DEL, _on_rel_obj_del, set);
             *set = eina_list_remove(*set, rel);
             efl_access_relation_free(rel);
             return;
          }
     }
}

//TIZEN_ONLY(20171115) Fixed the bugs and warnings in atspi relationship APIS
EAPI void
efl_access_relation_set_free(Efl_Access_Relation_Set *set)
{
   Efl_Access_Relation *rel;
   Eina_List *l;
   Eo *obj;

   EINA_LIST_FREE(*set, rel)
     {
        EINA_LIST_FOREACH(rel->objects, l, obj)
           efl_event_callback_del(obj, EFL_EVENT_DEL, _on_rel_obj_del, set);
        efl_access_relation_free(rel);
     }
}

EAPI Efl_Access_Relation_Set
efl_access_relation_set_clone(const Efl_Access_Relation_Set *set)
{
   Efl_Access_Relation_Set ret = NULL;
   Eina_List *l;
   Efl_Access_Relation *rel;

   EINA_LIST_FOREACH(*set, l, rel)
     {
        Efl_Access_Relation *cpy = efl_access_relation_clone(rel);
        ret = eina_list_append(ret, cpy);
     }

   return ret;
}
//

EOLIAN static Eina_Bool
_efl_access_relationship_append(Eo *obj EINA_UNUSED, Efl_Access_Data *sd, Efl_Access_Relation_Type type, const Efl_Access *relation_obj)
{
   return efl_access_relation_set_relation_append(&sd->relations, type, relation_obj);
}

EOLIAN static void
_efl_access_relationship_remove(Eo *obj EINA_UNUSED, Efl_Access_Data *sd, Efl_Access_Relation_Type type, const Efl_Access *relation_obj)
{
   if (relation_obj)
     efl_access_relation_set_relation_remove(&sd->relations, type, relation_obj);
   else
     efl_access_relation_set_relation_type_remove(&sd->relations, type);
}

EOLIAN static void
_efl_access_relationships_clear(Eo *obj EINA_UNUSED, Efl_Access_Data *sd)
{
   //TIZEN_ONLY(20171115) Fixed the bugs and warnings in atspi relationship APIS
   efl_access_relation_set_free(&sd->relations);
   //
   sd->relations = NULL;
}

//TIZEN_ONLY(20161122): add state_notify api
/**
 * Notify accessibility clients about current state of object
 *
 * @param mask bitfield with accessibilty field type values that should be
 * notified.
 */
static void _elm_atspi_accessibility_state_notify(Eo *obj, Efl_Access_State_Set mask)
{
   Efl_Access_State_Set ss;
   Efl_Access_State_Type type;

   ss = efl_access_state_set_get(obj);

   for (type = EFL_ACCESS_STATE_INVALID;
        type < EFL_ACCESS_STATE_LAST_DEFINED;
        type++)
     {
        if (STATE_TYPE_GET(mask, type))
          {
             efl_access_state_changed_signal_emit(
                obj, type, STATE_TYPE_GET(ss, type) ? EINA_TRUE : EINA_FALSE);
          }
     }
}

EOLIAN void
_efl_access_state_notify(Eo *obj, Efl_Access_Data *data EINA_UNUSED, Efl_Access_State_Set mask, Eina_Bool recursive)
{
   _elm_atspi_accessibility_state_notify(obj, mask);

   if (recursive)
     {
        Efl_Access *child;
        Eina_List *children;
        children = efl_access_children_get(obj);
        EINA_LIST_FREE(children, child)
          {
             efl_access_state_notify(child, mask, recursive);
          }
     }
}
//

EOLIAN Eo*
_efl_access_root_get(Eo *class EINA_UNUSED, void *pd EINA_UNUSED)
{
   if (!root)
      root = efl_add(ELM_ATSPI_APP_OBJECT_CLASS, NULL);

   return root;
}

EOLIAN void
_efl_access_efl_object_destructor(Eo *obj, Efl_Access_Data *pd)
{
   eina_stringshare_del(pd->name);
   eina_stringshare_del(pd->description);
   eina_stringshare_del(pd->translation_domain);
   efl_access_relation_set_free(&pd->relations);

   efl_destructor(efl_super(obj, EFL_ACCESS_MIXIN));
}

void
_efl_access_shutdown(void)
{
   Efl_Access_Event_Handler *hdl;

   EINA_LIST_FREE(global_callbacks, hdl)
     free(hdl);

   ELM_SAFE_DEL(root);
}

//TIZEN_ONLY(20171211): Keep Tizen Legacy API
EAPI Eina_Bool
elm_atspi_accessible_relationship_append(Elm_Interface_Atspi_Accessible *obj, Elm_Atspi_Relation_Type type, const Elm_Interface_Atspi_Accessible *relation_object)
{
   return efl_access_relationship_append(obj, type, relation_object);
}

EAPI void
elm_atspi_accessible_relationship_remove(Elm_Interface_Atspi_Accessible *obj, Elm_Atspi_Relation_Type type, const Elm_Interface_Atspi_Accessible *relation_object)
{
   efl_access_relationship_remove(obj, type, relation_object);
}

EAPI const char *
elm_atspi_accessible_translation_domain_get(const Elm_Interface_Atspi_Accessible *obj)
{
   return efl_access_translation_domain_get(obj);
}

EAPI void
elm_atspi_accessible_translation_domain_set(Elm_Interface_Atspi_Accessible *obj, const char *domain)
{
   efl_access_translation_domain_set(obj, domain);
}

EAPI const char *
elm_atspi_accessible_localized_role_name_get(const Elm_Interface_Atspi_Accessible *obj)
{
   return efl_access_localized_role_name_get(obj);
}

EAPI void elm_atspi_accessible_name_set(Elm_Interface_Atspi_Accessible *obj, const char *name)
{
   efl_access_name_set(obj, name);
}

EAPI const char *
elm_atspi_accessible_name_get(const Elm_Interface_Atspi_Accessible *obj)
{
   return efl_access_name_get(obj);
}

EAPI void
elm_atspi_accessible_name_cb_set(Elm_Interface_Atspi_Accessible *obj, Elm_Atspi_Reading_Info_Cb name_cb, const void *data)
{
   efl_access_name_cb_set(obj, name_cb, data);
}

EAPI Elm_Atspi_Relation_Set
elm_atspi_accessible_relation_set_get(const Elm_Interface_Atspi_Accessible *obj)
{
   return efl_access_relation_set_get(obj);
}

EAPI void
elm_atspi_accessible_role_set(Elm_Interface_Atspi_Accessible *obj, Elm_Atspi_Role role)
{
   efl_access_role_set(obj, role);
}

EAPI Elm_Atspi_Role
elm_atspi_accessible_role_get(const Elm_Interface_Atspi_Accessible *obj)
{
   return efl_access_role_get(obj);
}

EAPI Eina_List *elm_atspi_accessible_children_get(const Elm_Interface_Atspi_Accessible *obj)
{
   return efl_access_children_get(obj);
}

EAPI const char *
elm_atspi_accessible_role_name_get(const Elm_Interface_Atspi_Accessible *obj)
{
   return efl_access_role_name_get(obj);
}

EAPI Eina_List *
elm_atspi_accessible_attributes_get(const Elm_Interface_Atspi_Accessible *obj)
{
   return efl_access_attributes_get(obj);
}

EAPI void
elm_atspi_accessible_reading_info_type_set(Elm_Interface_Atspi_Accessible *obj, Elm_Atspi_Reading_Info_Type_Mask reading_info)
{
   efl_access_reading_info_type_set(obj, (Efl_Access_Reading_Info_Type_Mask)reading_info);
}

EAPI Elm_Atspi_Reading_Info_Type_Mask
elm_atspi_accessible_reading_info_type_get(const Elm_Interface_Atspi_Accessible *obj)
{
   return (Elm_Atspi_Reading_Info_Type_Mask)efl_access_reading_info_type_get(obj);
}

EAPI int
elm_atspi_accessible_index_in_parent_get(const Elm_Interface_Atspi_Accessible *obj)
{
   return efl_access_index_in_parent_get(obj);
}

EAPI void
elm_atspi_accessible_description_set(Elm_Interface_Atspi_Accessible *obj, const char *description)
{
   efl_access_description_set(obj, description);
}

EAPI const char *
elm_atspi_accessible_description_get(const Elm_Interface_Atspi_Accessible *obj)
{
   return efl_access_description_get(obj);
}

EAPI void
elm_atspi_accessible_description_cb_set(Elm_Interface_Atspi_Accessible *obj, Elm_Atspi_Reading_Info_Cb description_cb, const void *data)
{
   efl_access_description_cb_set(obj, description_cb, data);
}

EAPI void
elm_atspi_accessible_gesture_cb_set(Elm_Interface_Atspi_Accessible *obj, Elm_Atspi_Gesture_Cb gesture_cb, const void *data)
{
   efl_access_gesture_cb_set(obj, gesture_cb, data);
}

EAPI void
elm_atspi_accessible_parent_set(Elm_Interface_Atspi_Accessible *obj, Elm_Interface_Atspi_Accessible *parent)
{
   efl_access_parent_set(obj, parent);
}

EAPI Elm_Interface_Atspi_Accessible *
elm_atspi_accessible_parent_get(const Elm_Interface_Atspi_Accessible *obj)
{
   return efl_access_parent_get(obj);
}

EAPI Elm_Atspi_State_Set
elm_atspi_accessible_state_set_get(const Elm_Interface_Atspi_Accessible *obj)
{
   return efl_access_state_set_get(obj);
}

EAPI void
elm_atspi_accessible_can_highlight_set(Elm_Interface_Atspi_Accessible *obj, Eina_Bool can_highlight)
{
   efl_access_can_highlight_set(obj, can_highlight);
}

EAPI Eina_Bool
elm_atspi_accessible_can_highlight_get(const Elm_Interface_Atspi_Accessible *obj)
{
   return efl_access_can_highlight_get(obj);
}

EAPI void
elm_atspi_accessible_attribute_append(Elm_Interface_Atspi_Accessible *obj, const char *key, const char *value)
{
   efl_access_attribute_append(obj, key, value);
}

EAPI void
elm_atspi_accessible_attributes_clear(Elm_Interface_Atspi_Accessible *obj)
{
   efl_access_attributes_clear(obj);
}

EAPI void
elm_atspi_accessible_relationships_clear(Elm_Interface_Atspi_Accessible *obj)
{
   efl_access_relationships_clear(obj);
}

EAPI void
elm_atspi_accessible_state_notify(Elm_Interface_Atspi_Accessible *obj, Elm_Atspi_State_Set state_types_mask, Eina_Bool recursive)
{
   efl_access_state_notify(obj, state_types_mask, recursive);
}

EAPI void elm_atspi_relation_set_free(Elm_Atspi_Relation_Set *set)
{
   efl_access_relation_set_free(set);
}

EAPI Elm_Atspi_Relation_Set elm_atspi_relation_set_clone(const Elm_Atspi_Relation_Set *set)
{
   return efl_access_relation_set_clone(set);
}

EAPI void elm_atspi_relation_free(Elm_Atspi_Relation *relation)
{
   efl_access_relation_free(relation);
}

EAPI Elm_Atspi_Relation * elm_atspi_relation_clone(const Elm_Atspi_Relation *relation)
{
   return efl_access_relation_clone(relation);
}

EAPI Eina_Bool elm_atspi_relation_set_relation_append(Elm_Atspi_Relation_Set *set, Elm_Atspi_Relation_Type type, const Eo *rel_obj)
{
   return efl_access_relation_set_relation_append(set, type, rel_obj);
}

EAPI void elm_atspi_relation_set_relation_remove(Elm_Atspi_Relation_Set *set, Elm_Atspi_Relation_Type type, const Eo *rel_obj)
{
   efl_access_relation_set_relation_remove(set, type, rel_obj);
}

EAPI void elm_atspi_relation_set_relation_type_remove(Elm_Atspi_Relation_Set *set, Elm_Atspi_Relation_Type type)
{
   efl_access_relation_set_relation_type_remove(set, type);
}

EAPI void elm_atspi_attributes_list_free(Eina_List *list)
{
   efl_access_attributes_list_free(list);
}
//

#include "efl_access.eo.c"
