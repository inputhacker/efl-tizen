#ifdef HAVE_CONFIG_H
# include "elementary_config.h"
#endif

#define EFL_ACCESS_COMPONENT_PROTECTED
#define EFL_ACCESS_OBJECT_PROTECTED

#include <Elementary.h>

#include "elm_priv.h"

//TIZEN_ONLY(20161114): make atspi proxy work for more than two sockets created in one process
struct _Efl_Access_Component_Data
{
   Evas_Point socket_offset;
};

typedef struct _Efl_Access_Component_Data Efl_Access_Component_Data;
//


EOLIAN static void
_efl_access_component_screen_position_get(const Eo *obj EINA_UNUSED, Efl_Access_Component_Data *_pd EINA_UNUSED, int *x, int *y)
{
   Eina_Rect r;

   r = efl_access_component_extents_get(obj, EINA_TRUE);
   if (x) *x = r.x;
   if (y) *y = r.y;
}

EOLIAN static Eina_Bool
_efl_access_component_screen_position_set(Eo *obj EINA_UNUSED, Efl_Access_Component_Data *_pd EINA_UNUSED, int x, int y)
{
   Eina_Rect r;

   r = efl_access_component_extents_get(obj, EINA_TRUE);
   r.x = x;
   r.y = y;

   return efl_access_component_extents_set(obj, EINA_TRUE, r);
}

EOLIAN static Eina_Bool
_efl_access_component_contains(Eo *obj EINA_UNUSED, Efl_Access_Component_Data *_pd EINA_UNUSED, Eina_Bool type, int x, int y)
{
   Eina_Rect r;

   r = efl_access_component_extents_get(obj, type);
   return eina_rectangle_coords_inside(&r.rect, x, y);
}

EOLIAN static Eo *
_efl_access_component_accessible_at_point_get(Eo *obj, Efl_Access_Component_Data *_pd EINA_UNUSED, Eina_Bool screen_coords, int x, int y)
{
   Eina_List *l, *children;
   Eo *ret = NULL, *child;

   children = efl_access_object_access_children_get(obj);

   //TIZEN_ONLY (20171114) imporove object at xy get function
   EINA_LIST_REVERSE_FOREACH(children, l, child)
   //
     {
        Eina_Bool contains;
        if (efl_isa(child, EFL_ACCESS_COMPONENT_MIXIN))
          {
              //TIZEN_ONLY (20171114) imporove object at xy get function
              Efl_Access_Role role = efl_access_object_role_get(child);
              if (role == EFL_ACCESS_ROLE_REDUNDANT_OBJECT)
                continue;
              //
              contains = efl_access_component_contains(child, screen_coords, x, y);
              if (contains)
                {
                   ret = child;
                   break;
                }
          }
     }

   eina_list_free(children);
   return ret;
}

EOLIAN static Eina_Rect
_efl_access_component_extents_get(const Eo *obj, Efl_Access_Component_Data *_pd EINA_UNUSED, Eina_Bool screen_coords)
{
   Eina_Rect r;

   r = efl_gfx_entity_geometry_get(obj);
   if (screen_coords)
     {
        Ecore_Evas *ee = ecore_evas_ecore_evas_get(evas_object_evas_get(obj));
        if (ee)
          {
             int ee_x = 0, ee_y = 0;
             ecore_evas_geometry_get(ee, &ee_x, &ee_y, NULL, NULL);
             r.x += ee_x;
             r.y += ee_y;
          }
     }
   return r;
}

EOLIAN static Eina_Bool
_efl_access_component_extents_set(Eo *obj, Efl_Access_Component_Data *_pd EINA_UNUSED, Eina_Bool screen_coords, Eina_Rect r)
{
   int wx, wy;

   //if (!eina_rectangle_is_valid(&r)) return EINA_FALSE;
   if ((r.x < 0) || (r.y < 0) || (r.w < 0) || (r.h < 0)) return EINA_FALSE;

   if (screen_coords)
     {
        Ecore_Evas *ee = ecore_evas_ecore_evas_get(evas_object_evas_get(obj));
        if (!ee) return EINA_FALSE;
        evas_object_geometry_get(obj, &wx, &wy, NULL, NULL);
        ecore_evas_move(ee, r.x - wx, r.y - wy);
     }
   else
     evas_object_move(obj, r.x, r.y);

   evas_object_resize(obj, r.w, r.h);
   return EINA_TRUE;
}

EOLIAN static int
_efl_access_component_z_order_get(const Eo *obj EINA_UNUSED, Efl_Access_Component_Data *_pd EINA_UNUSED)
{
   // Currently not used.
   return 0;
}

EOLIAN static Eina_Bool
_efl_access_component_focus_grab(Eo *obj EINA_UNUSED, Efl_Access_Component_Data *_pd EINA_UNUSED)
{
   evas_object_focus_set(obj, EINA_TRUE);
   return evas_object_focus_get(obj);
}

//TIZEN_ONLY(20171108): bring HIGHLIGHT related changes
EOLIAN static Eina_Bool
_efl_access_component_highlight_grab(Eo *obj EINA_UNUSED, Efl_Access_Component_Data *_pd EINA_UNUSED)
{
   WRN("The %s object does not implement the \"component_highlight_grab\" function.",
       efl_class_name_get(efl_class_get(obj)));
   return EINA_FALSE;
}

EOLIAN static Eina_Bool
_efl_access_component_highlight_clear(Eo *obj EINA_UNUSED, Efl_Access_Component_Data *_pd EINA_UNUSED)
{
   WRN("The %s object does not implement the \"component_highlight_clear\" function.",
       efl_class_name_get(efl_class_get(obj)));
   return EINA_FALSE;
}
//

//TIZEN_ONLY(20161114): make atspi proxy work for more than two sockets created in one process
EOLIAN static void
_efl_access_component_socket_offset_set(Eo *obj EINA_UNUSED, Efl_Access_Component_Data *_pd, int x, int y)
{
   _pd->socket_offset.x = x;
   _pd->socket_offset.y = y;
}

EOLIAN static void
_efl_access_component_socket_offset_get(const Eo *obj EINA_UNUSED, Efl_Access_Component_Data *_pd, int *x, int *y)
{
   *x = _pd->socket_offset.x;
   *y = _pd->socket_offset.y;
}
//

//TIZEN_ONLY(20160721): atspi: provide highlight legacy API
EAPI Eina_Bool
elm_atspi_component_highlight_grab(Efl_Access_Component *obj)
{
   return efl_access_component_highlight_grab(obj);
}

EAPI Eina_Bool
elm_atspi_component_highlight_clear(Efl_Access_Component *obj)
{
   return efl_access_component_highlight_clear(obj);
}
//
#include "efl_access_component.eo.c"
