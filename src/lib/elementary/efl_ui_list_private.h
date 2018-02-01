#ifndef EFL_UI_LIST_PRIVATE_H
#define EFL_UI_LIST_PRIVATE_H

#ifdef HAVE_CONFIG_H
# include "elementary_config.h"
#endif

#define ELM_INTERFACE_ATSPI_ACCESSIBLE_PROTECTED
#define ELM_INTERFACE_ATSPI_SELECTION_PROTECTED
#define ELM_INTERFACE_ATSPI_WIDGET_ACTION_PROTECTED

#include <Elementary.h>
#include "elm_priv.h"

typedef struct _Efl_Ui_List_Item Efl_Ui_List_Item;
typedef struct _Efl_Ui_List_Data Efl_Ui_List_Data;

struct _Efl_Ui_List_Item
{
   Efl_Ui_List_LayoutItem item;
   Efl_Future           *future;
   Efl_Ui_List          *list;
   // Evas_Coord           x, y, minw, minh, w, h;
   // // double               h, v, wx, wy;
   // double               wx, wy;
};

int efl_ui_list_item_index_get(Efl_Ui_List_Item const* item);

typedef struct _Efl_Ui_List_Data Efl_Ui_List_Data;

#include "efl_ui_list_segarray.h"

struct _Efl_Ui_List_Data
{
   Eo                           *obj;
   Eo                           *scrl_mgr;
   Efl_Ui_List_Pan              *pan_obj;
   Efl_Model                    *model;

   Efl_Orient                   orient;

   struct {
      double                    h, v;
      Eina_Bool                 scalable: 1;
   } pad;

   struct {
      double                    h, v;
   } align;

   struct {
      double                    x, y;
   } weight;

   // struct {
   //    Evas_Coord                w, h;
   //    int                       start;
   //    int                       slice;
   // } realized;

   int segarray_first;
   Efl_Ui_List_SegArray segarray;

   Efl_Ui_Layout_Factory        *factory;
   Eina_List                    *selected_items;
   // struct {
   //   Eina_Inarray               array;
   // } items;
   Eina_Stringshare             *style;
   Elm_Object_Select_Mode       select_mode;
   Elm_List_Mode                mode;

   Efl_Ui_Focus_Manager         *manager;
   Eina_Rect                    gmt;
   Eina_Size2D                  min;
   int                          /*average_item_size, avsom, */item_count;
   Efl_Future                   *slice_future;
   Efl_Future                   *count_future;
   Efl_Ui_List_Relayout         *relayout;
   struct {
     int slice_start;
     int slice_count;
   } outstanding_slice;

   Eina_Bool                    homogeneous : 1;
   Eina_Bool                    recalc : 1;
   Eina_Bool                    on_hold : 1;
   Eina_Bool                    scrl_freeze : 1;
};

typedef struct _Efl_Ui_List_Pan_Data Efl_Ui_List_Pan_Data;

struct _Efl_Ui_List_Pan_Data
{
   Eo                     *wobj;
   Eina_Rect              gmt;
   Evas_Coord             move_diff;

   Ecore_Job              *resize_job;
};

typedef struct _Efl_Ui_List_Slice Efl_Ui_List_Slice;

struct _Efl_Ui_List_Slice
{
   Efl_Ui_List_Data       *pd;
   int                    newstart, slicestart, newslice;
};



#define EFL_UI_LIST_DATA_GET(o, ptr) \
  Efl_Ui_List_Data * ptr = efl_data_scope_get(o, EFL_UI_LIST_CLASS)

#define EFL_UI_LIST_DATA_GET_OR_RETURN(o, ptr)       \
  EFL_UI_LIST_DATA_GET(o, ptr);                      \
  if (EINA_UNLIKELY(!ptr))                           \
    {                                                \
       ERR("No widget data for object %p (%s)",      \
           o, evas_object_type_get(o));              \
       return;                                       \
    }

#define EFL_UI_LIST_DATA_GET_OR_RETURN_VAL(o, ptr, val) \
  EFL_UI_LIST_DATA_GET(o, ptr);                         \
  if (EINA_UNLIKELY(!ptr))                              \
    {                                                   \
       ERR("No widget data for object %p (%s)",         \
           o, evas_object_type_get(o));                 \
       return val;                                      \
    }

#endif
