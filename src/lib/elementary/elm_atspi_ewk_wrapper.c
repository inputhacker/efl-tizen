#ifdef HAVE_CONFIG_H
# include "elementary_config.h"
#endif

#define EFL_ACCESS_OBJECT_PROTECTED
#define EFL_ACCESS_COMPONENT_PROTECTED

#include <Elementary.h>
#include "elm_widget.h"
#include "elm_priv.h"

#define EWK_A11Y_DATA_KEY "__a11y_wrapper"

typedef struct _Elm_Atspi_Ewk_Wrapper_Data Elm_Atspi_Ewk_Wrapper_Data;

struct _Elm_Atspi_Ewk_Wrapper_Data
{
   Elm_Atspi_Proxy *proxy;
   Evas_Object *ewk_view;
   char *plug_id;
};

static void
_ewk_view_geometry_changed(void *data, const Efl_Event *event);

EFL_CALLBACKS_ARRAY_DEFINE(resize_watch,
                          { EFL_GFX_ENTITY_EVENT_MOVE, _ewk_view_geometry_changed },
                          { EFL_GFX_ENTITY_EVENT_RESIZE, _ewk_view_geometry_changed });

static void
_elm_atspi_ewk_wrapper_disconnect(Elm_Atspi_Ewk_Wrapper_Data *_pd)
{
   if (_pd->proxy) efl_del(_pd->proxy);
   free(_pd->plug_id);
   _pd->proxy = NULL;
   _pd->plug_id = NULL;
}

static void
_ewk_view_geometry_changed(void *data, const Efl_Event *event)
{
   Evas_Coord x, y, w, h;
   Elm_Atspi_Ewk_Wrapper *wrapper = data;

   evas_object_geometry_get(event->object, &x, &y, &w, &h);
   evas_object_move(wrapper, x, y);
   evas_object_resize(wrapper, w, h);
}

EOLIAN static void
_elm_atspi_ewk_wrapper_ewk_constructor(Eo *obj, Elm_Atspi_Ewk_Wrapper_Data *_pd, Evas_Object *ewk_view)
{
   Evas_Coord x, y, w, h;
   _pd->ewk_view = ewk_view;

   elm_widget_sub_object_parent_add(obj);
   efl_access_object_role_set(obj, EFL_ACCESS_ROLE_EMBEDDED);

   evas_object_geometry_get(ewk_view, &x, &y, &w, &h);
   evas_object_move(obj, x, y);
   evas_object_resize(obj, w, h);
   evas_object_show(obj);
   efl_event_callback_array_add(ewk_view, resize_watch(), obj);

   elm_obj_atspi_ewk_wrapper_connection_init(obj);
}

static Eo*
_elm_atspi_ewk_wrapper_proxy_create(const char *plugid, Eo *parent)
{
   Elm_Atspi_Proxy *proxy;
   char *bus = NULL, *path = NULL;

   if (!_elm_atspi_bridge_plug_id_split(plugid, &bus, &path)) {
        ERR("_elm_atspi_bridge_plug_id_split failed");
        return NULL;
   }

   proxy = efl_add(ELM_ATSPI_PROXY_CLASS, parent, elm_obj_atspi_proxy_ctor(efl_added, ELM_ATSPI_PROXY_TYPE_PLUG));
   if (!proxy) {
        ERR("Unable to create Elm_Atspi_Proxy object");
        free(bus); free(path);
        return NULL;
   }

   elm_obj_atspi_proxy_address_set(proxy, bus, path);

   free(bus);
   free(path);

   elm_atspi_bridge_utils_proxy_connect(proxy);
   return proxy;
}

EOLIAN static void
_elm_atspi_ewk_wrapper_connection_init(Eo *obj, Elm_Atspi_Ewk_Wrapper_Data *_pd)
{
   // PlugID is set by chromium
   const char *plug_id = evas_object_data_get(_pd->ewk_view, "__PlugID");

   // when plug_id is NULL, disconnect
   if (!plug_id)
     {
        _elm_atspi_ewk_wrapper_disconnect(_pd);
        return;
     }

   // check if already connected to atspi object referenced by plug_id
   if (_pd->plug_id && !strcmp(_pd->plug_id, plug_id))
     return;

   // destroy current connection
   _elm_atspi_ewk_wrapper_disconnect(_pd);

   // make new connection using proxy object
   _pd->proxy = _elm_atspi_ewk_wrapper_proxy_create(plug_id, obj);
   if (!_pd->proxy) {
        ERR("Failed connect to Ewk_View root accessible object");
        return;
   }
   _pd->plug_id = strdup(plug_id);
}

EOLIAN static void
_elm_atspi_ewk_wrapper_efl_object_destructor(Eo *obj EINA_UNUSED, Elm_Atspi_Ewk_Wrapper_Data *_pd)
{
   free(_pd->plug_id);
   if (_pd->ewk_view)
     efl_event_callback_array_del(_pd->ewk_view, resize_watch(), obj);
}

EOLIAN static Eina_List*
_elm_atspi_ewk_wrapper_efl_access_object_access_children_get(const Eo *obj EINA_UNUSED, Elm_Atspi_Ewk_Wrapper_Data *_pd)
{
   if (_pd->proxy)
     {
       //TIZEN_ONLY(20181024): Fix parent-children incosistencies in atspi tree
       efl_access_object_access_parent_set(_pd->proxy, obj);
       //
       return eina_list_append(NULL, _pd->proxy);
     }
   return NULL;
}

static void
_wrapper_widget_del(void *data, const Efl_Event *event EINA_UNUSED)
{
   Evas_Object *ewk_view = data;
   evas_object_data_set(ewk_view, EWK_A11Y_DATA_KEY, NULL);
}

static void
_ewk_view_widget_del(void *data, const Efl_Event *event EINA_UNUSED)
{
   Elm_Atspi_Ewk_Wrapper *wrapper = data;
   efl_del(wrapper);
}

EOLIAN void
_elm_atspi_ewk_wrapper_a11y_init(Eo *obj EINA_UNUSED, void *pd EINA_UNUSED,
                                 Evas_Object *parent, Evas_Object *ewk_view)
{
   Evas_Object *wrapper = evas_object_data_get(ewk_view, EWK_A11Y_DATA_KEY);
   if (!wrapper) {
        wrapper = efl_add(ELM_ATSPI_EWK_WRAPPER_CLASS, parent, elm_obj_atspi_ewk_wrapper_ewk_constructor(efl_added, ewk_view));
        evas_object_data_set(ewk_view, EWK_A11Y_DATA_KEY, wrapper);
        // make sure that wrapper will not outlive ewk_view
        efl_event_callback_del(wrapper, EFL_EVENT_DEL, _wrapper_widget_del, ewk_view);
        efl_event_callback_del(ewk_view, EFL_EVENT_DEL, _ewk_view_widget_del, wrapper);
   } else {
        // Check if reparenting occured on Ewk_View
        // This may happen in case when ewk_view is taken out from layout and put into another one.
        // In order to avoid situation when ewk a11y may be accessed from different
        // a11y node we check if reparenting occured and reset wrapper parent
        if (parent != elm_widget_parent_get(wrapper)) {
          elm_widget_sub_object_add(parent, wrapper);
        }
   }
   elm_atspi_ewk_wrapper_connection_init(wrapper);
}

EOLIAN static Eo *
_elm_atspi_ewk_wrapper_efl_access_component_accessible_at_point_get(Eo *obj EINA_UNUSED, Elm_Atspi_Ewk_Wrapper_Data *_pd, Eina_Bool screen_coords, int x, int y)
{
   Eina_Rectangle rect;
   int ee_x, ee_y;

   if (!_pd->ewk_view)
     return NULL;

   if (screen_coords)
     {
        Ecore_Evas *ee = ecore_evas_ecore_evas_get(evas_object_evas_get(_pd->ewk_view));
        if (!ee) return NULL;
        ecore_evas_geometry_get(ee, &ee_x, &ee_y, NULL, NULL);
        x -= ee_x;
        y -= ee_y;
     }

   evas_object_geometry_get(_pd->ewk_view, &rect.x, &rect.y, &rect.w, &rect.h);

   if (eina_rectangle_coords_inside(&rect, x, y))
      return _pd->proxy;
   else
      return NULL;
}

EOLIAN static Evas_Object *
_elm_atspi_ewk_wrapper_ewk_view_get(const Eo *obj EINA_UNUSED, Elm_Atspi_Ewk_Wrapper_Data *_pd)
{
   return _pd->ewk_view;
}


#include "elm_atspi_ewk_wrapper.eo.c"
