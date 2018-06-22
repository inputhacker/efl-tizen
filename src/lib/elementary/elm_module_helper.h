/* A small helper header defining EAPI for elementary modules, it should be
 * included last in the modules C files.
 */

#ifndef ELM_MODULE_HELPER_H
#define ELM_MODULE_HELPER_H

#include "elm_priv.h"

#ifdef EAPI
# undef EAPI
#endif
#ifdef EWAPI
# undef EWAPI
#endif

#ifdef _WIN32
# ifdef ELEMENTARY_BUILD
#  ifdef DLL_EXPORT
#   define EAPI __declspec(dllexport)
#  else
#   define EAPI
#  endif /* ! DLL_EXPORT */
# else
#  define EAPI __declspec(dllimport)
# endif /* ! EFL_EVAS_BUILD */
# define EAPI_WEAK
#else
# ifdef __GNUC__
#  if __GNUC__ >= 4
#   define EAPI __attribute__ ((visibility("default")))
#   define EAPI_WEAK __attribute__ ((weak))
#  else
#   define EAPI
#   define EAPI_WEAK
#  endif
# else
#  define EAPI
#  define EAPI_WEAK
# endif
#endif /* ! _WIN32 */

#define EWAPI EAPI EAPI_WEAK

/*************************************************************
 * TIZEN_ONLY_FEATURE : Tizen Copy & Paste feature with CBHM *
 *************************************************************/

typedef void (*cpfunc)(void *data, Evas_Object *obj, void *event_info);

struct _Elm_Entry_Extension_data
{
   Evas_Object *popup;
   Evas_Object *ent;
   Evas_Object *caller;
   Eina_Rectangle *viewport_rect;
   Evas_Coord_Rectangle selection_rect;
   Eina_List *items;
   cpfunc select;
   cpfunc copy;
   cpfunc cut;
   cpfunc paste;
   cpfunc cancel;
   cpfunc selectall;
   cpfunc cnpinit;
   cpfunc keep_selection;
   cpfunc paste_translation;
   cpfunc is_selected_all;
   Elm_Config *_elm_config;
   Eina_Bool password : 1;
   Eina_Bool editable : 1;
   Eina_Bool have_selection: 1;
   Eina_Bool selmode : 1;
   Eina_Bool context_menu : 1;
   Elm_Cnp_Mode cnp_mode : 2;
   Eina_Bool popup_showing : 1;
   Eina_Bool mouse_up : 1;
   Eina_Bool mouse_move : 1;
   Eina_Bool mouse_down : 1;
   Eina_Bool entry_move : 1;
   Eina_Bool popup_clicked : 1;
   Eina_Bool cursor_handler_shown : 1;
   Eina_Bool ent_scroll : 1;
   Evas_Object *ctx_par;
   Evas_Object *start_handler;
   Evas_Object *end_handler;
   Evas_Object *cursor_handler;
   Ecore_Timer *show_timer;
   char *source_text;
   char *target_text;
#ifdef HAVE_ELEMENTARY_WL2
   Eldbus_Proxy *cbhm_proxy;
#endif
   /* For setting color class parent relation to copypaste popup */
   Evas_Object *entry_edje;

   /* For checking wearable profile */
   char profile_wear;
};

typedef struct _Elm_Entry_Extension_data Elm_Entry_Extension_data;

void elm_entry_extension_module_data_get(Evas_Object *obj,Elm_Entry_Extension_data *ext_mod);
/*******
 * END *
 *******/

#endif
