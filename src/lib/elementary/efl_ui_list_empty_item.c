#ifdef HAVE_CONFIG_H
# include "elementary_config.h"
#endif

#define EFL_UI_LIST_EMPTY_ITEM_PROTECTED

#include <Elementary.h>

#include "elm_priv.h"
#include "elm_part_helper.h"

#define MY_CLASS      EFL_UI_LIST_EMPTY_ITEM_CLASS
#define MY_CLASS_PFX  efl_ui_list_empty_item

#define MY_CLASS_NAME "Efl.Ui.List_Empty_Item"

EOLIAN static Efl_Object *
_efl_ui_list_empty_item_efl_object_finalize(Eo *obj, void *pd EINA_UNUSED)
{
   Eo *eo;
   eo = efl_finalize(efl_super(obj, MY_CLASS));
   ELM_WIDGET_DATA_GET_OR_RETURN(eo, wd, eo);
   Efl_Ui_Theme_Apply theme_apply = efl_ui_layout_object_theme_set(obj, "list_item", NULL, "empty");

   if (theme_apply == EFL_UI_THEME_APPLY_FAILED)
     CRI("Empty Item(%p) failed to set theme [efl/list_item:empty]!", eo);
   return eo;
}

EOLIAN static void
_efl_ui_list_empty_item_efl_object_destructor(Eo *obj, void *pd EINA_UNUSED)
{
   efl_destructor(efl_super(obj, MY_CLASS));
}

/* Efl.Part */
ELM_PART_CONTENT_DEFAULT_GET(efl_ui_list_empty_item, "efl.content")
ELM_PART_CONTENT_DEFAULT_IMPLEMENT(efl_ui_list_empty_item, void)
/* Efl.Part end */

EOLIAN static Efl_Object *
_efl_ui_list_empty_item_efl_part_part(const Eo *obj, void *wd EINA_UNUSED, const char *part)
{
   EINA_SAFETY_ON_NULL_RETURN_VAL(part, NULL);
   if (eina_streq(part, "content"))
     return ELM_PART_IMPLEMENT(EFL_UI_LAYOUT_PART_CONTENT_CLASS, obj, "efl.content");

   return efl_part(efl_super(obj, MY_CLASS), part);
}

/* Internal EO APIs and hidden overrides */

#define EFL_UI_LIST_EMPTY_ITEM_EXTRA_OPS \
  ELM_PART_CONTENT_DEFAULT_OPS(efl_ui_list_empty_item)

#include "efl_ui_list_empty_item.eo.c"
