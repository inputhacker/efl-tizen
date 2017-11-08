#ifdef HAVE_CONFIG_H
# include "elementary_config.h"
#endif

#define EFL_UI_WIN_PROTECTED
#define EFL_UI_WIN_SOCKET_PROTECTED

#include <Elementary.h>
//TIZEN_ONLY(20171108): make atspi_proxy work
// _elm_config
#include "elm_priv.h"
//

#define MY_CLASS EFL_UI_WIN_SOCKET_CLASS
#define MY_CLASS_NAME "Efl.Ui.Win_Socket"

typedef struct
{
   //TIZEN_ONLY(20171108): make atspi_proxy work
   Eo *socket_proxy; /* reference object to atspi object in separate process @since 1.15 */
   //
} Efl_Ui_Win_Socket_Data;

EOLIAN static Efl_Object *
_efl_ui_win_socket_efl_object_finalize(Eo *obj, Efl_Ui_Win_Socket_Data *pd EINA_UNUSED)
{
   efl_ui_win_type_set(obj, ELM_WIN_SOCKET_IMAGE);
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

   if (_elm_config->atspi_mode)
     {
        if (pd->socket_proxy)
          efl_unref(pd->socket_proxy);
        pd->socket_proxy = _elm_atspi_bridge_utils_proxy_create(obj, svcname, svcnum, ELM_ATSPI_PROXY_TYPE_SOCKET);
        elm_atspi_bridge_utils_proxy_listen(pd->socket_proxy);
     }

   return EINA_TRUE;
   //
}

//TIZEN_ONLY(20171108): make atspi_proxy work
EOLIAN static void
_efl_ui_win_socket_efl_gfx_visible_set(Eo *obj, Efl_Ui_Win_Socket_Data *sd, Eina_Bool vis)
{
   efl_gfx_visible_set(efl_super(obj, EFL_UI_WIN_SOCKET_CLASS), vis);

   const char *plug_id_2;
   if (vis)
     {
        if ((plug_id_2 = evas_object_data_get(obj, "___PLUGID")) != NULL)
          {
             char *svcname, *svcnum;
             if (!sd->socket_proxy && _elm_atspi_bridge_plug_id_split(plug_id_2, &svcname, &svcnum))
               {
                  sd->socket_proxy = _elm_atspi_bridge_utils_proxy_create(obj, svcname, atoi(svcnum), ELM_ATSPI_PROXY_TYPE_SOCKET);
                  elm_atspi_bridge_utils_proxy_listen(sd->socket_proxy);
                  free(svcname);
                  free(svcnum);
               }
          }
     }
}

EOLIAN static Eo*
_efl_ui_win_socket_elm_interface_atspi_accessible_parent_get(Eo *obj, Efl_Ui_Win_Socket_Data *pd)
{
   if (pd->socket_proxy) return pd->socket_proxy;

   return elm_interface_atspi_accessible_parent_get(efl_super(obj, EFL_UI_WIN_SOCKET_CLASS));
}
//
#include "efl_ui_win_socket.eo.c"
