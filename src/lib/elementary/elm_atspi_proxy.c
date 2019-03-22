#ifdef HAVE_CONFIG_H
  #include "elementary_config.h"
#endif

#define EFL_ACCESS_OBJECT_PROTECTED

#include <Elementary.h>
#include "elm_widget.h"
#include "elm_priv.h"
//TIZEN_ONLY(20171222): include headerfile for eo info
#include "elm_ctxpopup_item_eo.h"
//

#define MY_CLASS ELM_ATSPI_PROXY_CLASS
#define MY_CLASS_NAME "Elm.Atspi.Proxy"

static Eina_List *_socket_list;

typedef struct _Elm_Atspi_Proxy_Data Elm_Atspi_Proxy_Data;

struct _Elm_Atspi_Proxy_Data
{
   Elm_Atspi_Proxy_Type type;
   const char *bus;
   const char *path;
   Ecore_Timer *retry_timer;
   int retry_count;
};

#define ELM_ATSPI_PROXY_RETRY_TIME 1.0
#define ELM_ATSPI_PROXY_RETRY_COUNT_LIMIT 5
static Eina_Bool
_retry_timer_cb(void *data)
{
   Eo *proxy;

   proxy = data;
   Elm_Atspi_Proxy_Data *_pd = efl_data_scope_get(proxy, ELM_ATSPI_PROXY_CLASS);

   if (_pd->bus && _pd->path)
     {
        DBG("Address allocated: %s, %s", _pd->bus, _pd->path);
        _pd->retry_count = 0;
        _pd->retry_timer = NULL;
        return ECORE_CALLBACK_CANCEL;
     }

   if (_pd->retry_count < ELM_ATSPI_PROXY_RETRY_COUNT_LIMIT)
     {
        elm_atspi_bridge_utils_proxy_connect(proxy);
        _pd->retry_count++;
        return ECORE_CALLBACK_RENEW;
     }

   _pd->retry_count = 0;
   _pd->retry_timer = NULL;
   return ECORE_CALLBACK_CANCEL;
}

EOLIAN static void
_elm_atspi_proxy_address_get_retry_timer_add(Eo *obj, Elm_Atspi_Proxy_Data *_pd)
{
   if (_pd->type != ELM_ATSPI_PROXY_TYPE_PLUG)
     {
        ERR("Proxy type is not ELM_ATSPI_PROXY_TYPE_PLUG");
        return;
     }

   if (_pd->retry_timer)
     {
        DBG("Retry timer is already added");
        return;
     }

   _pd->retry_timer = ecore_timer_add(ELM_ATSPI_PROXY_RETRY_TIME,
                                      _retry_timer_cb, obj);
}

EOLIAN static void
_elm_atspi_proxy_efl_object_destructor(Eo *obj, Elm_Atspi_Proxy_Data *_pd)
{
   if (_pd->retry_timer)
     {
        ecore_timer_del(_pd->retry_timer);
        _pd->retry_timer = NULL;
     }
   if (_pd->type == ELM_ATSPI_PROXY_TYPE_SOCKET)
      _socket_list = eina_list_remove(_socket_list, obj);

   if (_pd->bus)
     {
        eina_stringshare_del(_pd->bus);
        _pd->bus = NULL;
     }
   if (_pd->path)
     {
        eina_stringshare_del(_pd->path);
        _pd->path = NULL;
     }

   efl_destructor(efl_super(obj, MY_CLASS));
}

EOLIAN static void
_elm_atspi_proxy_ctor(Eo *obj, Elm_Atspi_Proxy_Data *_pd, Elm_Atspi_Proxy_Type type)
{
   Eo *parent;

   _pd->type = type;
   parent = efl_parent_get(obj);
   if (!parent || !efl_isa(parent, EFL_ACCESS_OBJECT_MIXIN))
     {
        CRI("Elm_Atspi_Proxy parent (%s) must implement EFL_ACCESS_OBJECT_MIXIN", efl_class_name_get(efl_class_get(parent)));
        abort();
     }
   if (type == ELM_ATSPI_PROXY_TYPE_SOCKET)
      _socket_list = eina_list_append(_socket_list, obj);
}

EOLIAN Elm_Atspi_Proxy_Type
_elm_atspi_proxy_type_get(const Eo *obj EINA_UNUSED, Elm_Atspi_Proxy_Data *_pd)
{
   return _pd->type;
}

EOLIAN void
_elm_atspi_proxy_address_get(const Eo *obj EINA_UNUSED, Elm_Atspi_Proxy_Data *_pd, const char **bus, const char **path)
{
   if (bus) *bus = _pd->bus;
   if (path) *path = _pd->path;
}

EOLIAN void
_elm_atspi_proxy_address_set(Eo *obj EINA_UNUSED, Elm_Atspi_Proxy_Data *_pd, const char *bus, const char *path)
{
   if (bus) eina_stringshare_replace(&_pd->bus, bus);
   if (path) eina_stringshare_replace(&_pd->path, path);
}

EOLIAN Eina_List*
_elm_atspi_proxy_efl_access_object_access_children_get(const Eo *obj EINA_UNUSED, Elm_Atspi_Proxy_Data *_pd)
{
   Eina_List *ret = NULL;
   if (_pd->type == ELM_ATSPI_PROXY_TYPE_SOCKET)
     {
        Eo *parent;
        parent = efl_parent_get(obj);
        ret = eina_list_append(ret, parent);
     }
   return ret;
}

Eina_List*
_elm_atspi_proxy_socket_list_get(void)
{
   return eina_list_clone(_socket_list);
}

EOLIAN static void
_elm_atspi_proxy_efl_ui_widget_atspi(Eo *obj, Elm_Atspi_Proxy_Data *_pd, Eina_Bool is_atspi)
{
   EINA_SAFETY_ON_NULL_RETURN(obj);
   if (is_atspi && _pd->type == ELM_ATSPI_PROXY_TYPE_PLUG)
     {
        elm_atspi_bridge_utils_proxy_connect(obj);
     }
}

#include "elm_atspi_proxy_eo.c"
