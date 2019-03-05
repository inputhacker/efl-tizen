#ifdef HAVE_CONFIG_H
# include "elementary_config.h"
#endif

#define EFL_ACCESS_OBJECT_PROTECTED
#define EFL_UI_WIN_PROTECTED
#define EFL_UI_WIN_SOCKET_PROTECTED

#include <Elementary.h>
//TIZEN_ONLY(20171108): make atspi_proxy work
// _elm_config
#include "elm_priv.h"
//

#include "elm_priv.h"
#include "efl_ui_win_socket_legacy_eo.h"

#define MY_CLASS EFL_UI_WIN_SOCKET_CLASS
#define MY_CLASS_NAME "Efl.Ui.Win_Socket"
#define MY_CLASS_NAME_LEGACY "elm_win"

typedef struct
{
   //TIZEN_ONLY(20171108): make atspi_proxy work
   Eo *socket_proxy; /* reference object to atspi object in separate process @since 1.15 */
   //
} Efl_Ui_Win_Socket_Data;

//TIZEN_ONLY(20170613) -listen if access mode is enabled
#define EFL_UI_WIN_SOCKET_DATA_GET(o, sd) \
   Efl_Ui_Win_Socket_Data * sd = efl_data_scope_get(o, MY_CLASS)
//

EOLIAN static Efl_Object *
_efl_ui_win_socket_efl_object_finalize(Eo *obj, Efl_Ui_Win_Socket_Data *pd EINA_UNUSED)
{
   efl_ui_win_type_set(obj, EFL_UI_WIN_TYPE_SOCKET_IMAGE);
   obj = efl_finalize(efl_super(obj, MY_CLASS));

   return obj;
}

EOLIAN static Eina_Bool
_efl_ui_win_socket_socket_listen(Eo *obj, Efl_Ui_Win_Socket_Data *pd EINA_UNUSED, const char *svcname, int svcnum, Eina_Bool svcsys)
{
   Ecore_Evas *ee = ecore_evas_ecore_evas_get(evas_object_evas_get(obj));

   if (!ee) return EINA_FALSE;
   /* TIZEN_ONLY(20171108): make atspi_proxy work
   return ecore_evas_extn_socket_listen(ee, svcname, svcnum, svcsys);
   */
   //TIZEN_ONLY(20171108): make atspi_proxy work
   if (!ecore_evas_extn_socket_listen(ee, svcname, svcnum, svcsys))
     return EINA_FALSE;

   if (_elm_atspi_enabled())
     {
        if (pd->socket_proxy)
          efl_unref(pd->socket_proxy);
        pd->socket_proxy = _elm_atspi_bridge_utils_proxy_create(obj, svcname, svcnum, ELM_ATSPI_PROXY_TYPE_SOCKET);
        elm_atspi_bridge_utils_proxy_listen(pd->socket_proxy);
     }

   return EINA_TRUE;
   //
}

//TIZEN_ONLY(20170613) -listen if access mode is enabled
void
_access_socket_proxy_listen(Eo * obj)
{
   EFL_UI_WIN_SOCKET_DATA_GET(obj, sd);
   const char *plug_id;

   if ((plug_id = evas_object_data_get(obj, "___PLUGID")) != NULL)
     {
        char *svcname, *svcnum;
       // TIZEN_ONLY(20171109) : fix for invalid proxy object, when at-spi has been restarted
       if (_elm_atspi_bridge_plug_id_split(plug_id, &svcname, &svcnum))
         {
            if (sd->socket_proxy)
              {
                if (!evas_object_data_get(sd->socket_proxy, "__proxy_invalid")) return;
                efl_unref(sd->socket_proxy);
              }
             sd->socket_proxy = _elm_atspi_bridge_utils_proxy_create(obj, svcname, atoi(svcnum), ELM_ATSPI_PROXY_TYPE_SOCKET);
             elm_atspi_bridge_utils_proxy_listen(sd->socket_proxy);
             free(svcname);
             free(svcnum);
          }
       //
     }
}


void
_access_socket_proxy_unref(Eo * obj)
{
   EFL_UI_WIN_SOCKET_DATA_GET(obj, sd);
   if (sd->socket_proxy)
     {
        efl_unref(sd->socket_proxy);
        sd->socket_proxy = NULL;
     }
}
//

//TIZEN_ONLY(20171108): make atspi_proxy work
EOLIAN static void
_efl_ui_win_socket_efl_gfx_entity_visible_set(Eo *obj, Efl_Ui_Win_Socket_Data *sd EINA_UNUSED, Eina_Bool vis)
{
   efl_gfx_entity_visible_set(efl_super(obj, EFL_UI_WIN_SOCKET_CLASS), vis);

   // TIZEN_ONLY(20160705) - enable atspi_proxy to work
   if (vis)
     {
        if (_elm_atspi_enabled())
          {
             //TIZEN_ONLY(20170613) -listen if atspi is enabled
             _access_socket_proxy_listen(obj);
             //
          }
     }
   //
}

EOLIAN static Eo*
_efl_ui_win_socket_efl_object_parent_get(const Eo *obj, Efl_Ui_Win_Socket_Data *pd)
{
   Eo *parent, *prov;

   if (pd->socket_proxy) return pd->socket_proxy;

   parent = efl_parent_get(efl_super(obj, EFL_UI_WIN_SOCKET_CLASS));
   prov = efl_provider_find(parent, EFL_ACCESS_OBJECT_MIXIN);

   return (prov ? prov : parent);
}

//
#include "efl_ui_win_socket.eo.c"

static void
_efl_ui_win_socket_legacy_class_constructor(Efl_Class *klass)
{
   evas_smart_legacy_type_register(MY_CLASS_NAME_LEGACY, klass);
}

EOLIAN static Eo *
_efl_ui_win_socket_legacy_efl_object_finalize(Eo *obj, void *pd EINA_UNUSED)
{
   obj = efl_finalize(efl_super(obj, EFL_UI_WIN_SOCKET_LEGACY_CLASS));
   efl_canvas_object_type_set(obj, MY_CLASS_NAME_LEGACY);
   return obj;
}

#include "efl_ui_win_socket_legacy_eo.c"
