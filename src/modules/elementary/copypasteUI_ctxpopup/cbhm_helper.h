#ifndef _CBHM_HELPER_H
#define _CBHM_HELPER_H

#ifdef HAVE_CONFIG_H
#include "elementary_config.h"
#endif

#include <Elementary.h>
#include <Eina.h>
#include "elm_priv.h"
#include "elm_module_helper.h"

#ifdef HAVE_ELEMENTARY_X
#include <Ecore_X.h>
#endif

//#define DEBUG

#ifdef DEBUG
#define DMSG(fmt, args...) printf("[%s], "fmt, __func__, ##args)
#else
#define DMSG(args...)
#endif

enum ATOM_INDEX_CBHM_COUNT {
	ATOM_INDEX_CBHM_COUNT_ALL = 0,
	ATOM_INDEX_CBHM_COUNT_TEXT = 1,
	ATOM_INDEX_CBHM_COUNT_IMAGE = 2,
	ATOM_INDEX_CBHM_COUNT_MAX = 3
};

#ifdef HAVE_ELEMENTARY_WAYLAND
enum _Atom_Index_Cbhm_Item_Type {
	ATOM_INDEX_CBHM_TARGET = 0,
	ATOM_INDEX_CBHM_TEXT = 1,
	ATOM_INDEX_CBHM_HTML = 2,
	ATOM_INDEX_CBHM_EFL = 3,
	ATOM_INDEX_CBHM_IMAGE = 4,
	ATOM_INDEX_CBHM_POLARIS = 5,
	ATOM_INDEX_CBHM_MAX = 6
};
typedef enum _Atom_Index_Cbhm_Item_Type Atom_Index_Cbhm_Item_Type;
int _cbhm_item_type_get(Evas_Object *obj, int item_index);
#endif

Eina_Bool _cbhm_msg_send(Evas_Object* obj, char *msg);
int _cbhm_item_count_get(Evas_Object *obj EINA_UNUSED, int atom_index);
#ifdef HAVE_ELEMENTARY_X
Eina_Bool _cbhm_item_get(Evas_Object *obj EINA_UNUSED, int index, Ecore_X_Atom *data_type, char **buf);
#else
Eina_Bool _cbhm_item_get(Evas_Object *obj, int index, void *data_type, char **buf);
#endif

#ifdef HAVE_ELEMENTARY_WAYLAND
Eldbus_Proxy* cbhm_proxy_get(Evas_Object *obj);
#endif

#ifdef HAVE_ELEMENTARY_X
Eina_Bool _cbhm_item_set(Evas_Object *obj, Ecore_X_Atom data_type, char *item_data);
#endif

#endif /* _CBHM_HELPER_H */
