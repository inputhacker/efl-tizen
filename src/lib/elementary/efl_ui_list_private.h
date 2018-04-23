#ifndef EFL_UI_LIST_PRIVATE_H
#define EFL_UI_LIST_PRIVATE_H

#include "Elementary.h"

typedef struct _Efl_Ui_List_Data
{
   // Eo Objects
   Eo *box; /* Internal box for Efl.Pack support */
   Eo *pan; /* Internal pan for scroll */
   Eo *smanager; /* Scroll Manager for support scroll with pan */

   struct {
      Efl_Ui_List_Item *item; /* Scroll target item */
      double align; /* Item scroll aligned position.
                       -1 is for default case of scroll in */
      Eina_Bool anim : 1; /* boolean data for animation on scroll */
   } scroll; /* Item scroll values for deferred operation */

   Eina_List  *items; /* All item list for internal maintaining */
   Eina_List  *selected; /* Selected items list */
   Efl_Ui_List_Item *last_selected; /* latest selected item */

   Efl_Ui_Select_Mode  select_mode; /* Select mode for items
                                       with single or multiple seleciton */

   // Boolean Data
   Eina_Bool  freeze_want : 1; /* Flag for Scroll freezing */
   Eina_Bool  match_content_w: 1; /* Flag for size matching on content width.
                                     it will disable horizontal scroll */
   Eina_Bool  match_content_h: 1; /* Flag for size matching on content height.
                                     it will disable vertical scroll */
   Eina_Bool  pan_resized : 1; /* Flag for pan resized.
                                   This value is used for checking scroll is
                                   calculated after initialized */
} Efl_Ui_List_Data;

typedef struct _Item_Iterator
{
   Eina_Iterator  iterator;
   Eina_List     *list;
   Eina_Iterator *real_iterator;
   Eo            *object;
} Item_Iterator;


#define EFL_UI_LIST_DATA_GET(o, pd) \
  Efl_Ui_List_Data * pd = efl_data_scope_safe_get(o, EFL_UI_LIST_CLASS)

#define EFL_UI_LIST_DATA_GET_OR_RETURN(o, ptr, ...)   \
  EFL_UI_LIST_DATA_GET(o, ptr);                       \
  if (EINA_UNLIKELY(!ptr))                            \
    {                                                 \
       ERR("No widget data for object %p (%s)",       \
           o, evas_object_type_get(o));               \
       return __VA_ARGS__;                            \
    }

#define EFL_UI_LIST_CHECK(obj)                           \
  if (EINA_UNLIKELY(!efl_isa((obj), EFL_UI_LIST_CLASS))) \
    return

#endif
