
void _elm_atspi_ewk_wrapper_ewk_constructor(Eo *obj, Elm_Atspi_Ewk_Wrapper_Data *pd, Efl_Object *ewk);

EOAPI EFL_VOID_FUNC_BODYV(elm_obj_atspi_ewk_wrapper_ewk_constructor, EFL_FUNC_CALL(ewk), Efl_Object *ewk);

void _elm_atspi_ewk_wrapper_connection_init(Eo *obj, Elm_Atspi_Ewk_Wrapper_Data *pd);

EOAPI EFL_VOID_FUNC_BODY(elm_obj_atspi_ewk_wrapper_connection_init);

Efl_Object *_elm_atspi_ewk_wrapper_ewk_view_get(const Eo *obj, Elm_Atspi_Ewk_Wrapper_Data *pd);

EOAPI EFL_FUNC_BODY_CONST(elm_obj_atspi_ewk_wrapper_ewk_view_get, Efl_Object *, NULL);

void _elm_atspi_ewk_wrapper_a11y_init(Efl_Object *parent, Efl_Object *ewk);

EOAPI void elm_obj_atspi_ewk_wrapper_a11y_init(Efl_Object *parent, Efl_Object *ewk)
{
   elm_atspi_ewk_wrapper_class_get();
   _elm_atspi_ewk_wrapper_a11y_init(parent, ewk);
}
EOAPI void elm_atspi_ewk_wrapper_a11y_init(Efl_Object *parent, Efl_Object *ewk)
{
   elm_atspi_ewk_wrapper_class_get();
   _elm_atspi_ewk_wrapper_a11y_init(parent, ewk);
}

void _elm_atspi_ewk_wrapper_efl_object_destructor(Eo *obj, Elm_Atspi_Ewk_Wrapper_Data *pd);


Eina_List *_elm_atspi_ewk_wrapper_efl_access_object_access_children_get(const Eo *obj, Elm_Atspi_Ewk_Wrapper_Data *pd);


Efl_Object *_elm_atspi_ewk_wrapper_efl_access_component_accessible_at_point_get(Eo *obj, Elm_Atspi_Ewk_Wrapper_Data *pd, Eina_Bool screen_coords, int x, int y);


static Eina_Bool
_elm_atspi_ewk_wrapper_class_initializer(Efl_Class *klass)
{
   const Efl_Object_Ops *opsp = NULL;

   const Efl_Object_Property_Reflection_Ops *ropsp = NULL;

#ifndef ELM_ATSPI_EWK_WRAPPER_EXTRA_OPS
#define ELM_ATSPI_EWK_WRAPPER_EXTRA_OPS
#endif

   EFL_OPS_DEFINE(ops,
      EFL_OBJECT_OP_FUNC(elm_obj_atspi_ewk_wrapper_ewk_constructor, _elm_atspi_ewk_wrapper_ewk_constructor),
      EFL_OBJECT_OP_FUNC(elm_obj_atspi_ewk_wrapper_connection_init, _elm_atspi_ewk_wrapper_connection_init),
      EFL_OBJECT_OP_FUNC(elm_obj_atspi_ewk_wrapper_ewk_view_get, _elm_atspi_ewk_wrapper_ewk_view_get),
      EFL_OBJECT_OP_FUNC(efl_destructor, _elm_atspi_ewk_wrapper_efl_object_destructor),
      EFL_OBJECT_OP_FUNC(efl_access_object_access_children_get, _elm_atspi_ewk_wrapper_efl_access_object_access_children_get),
      EFL_OBJECT_OP_FUNC(efl_access_component_accessible_at_point_get, _elm_atspi_ewk_wrapper_efl_access_component_accessible_at_point_get),
      ELM_ATSPI_EWK_WRAPPER_EXTRA_OPS
   );
   opsp = &ops;

   return efl_class_functions_set(klass, opsp, ropsp);
}

static const Efl_Class_Description _elm_atspi_ewk_wrapper_class_desc = {
   EO_VERSION,
   "Elm.Atspi.Ewk_Wrapper",
   EFL_CLASS_TYPE_REGULAR,
   sizeof(Elm_Atspi_Ewk_Wrapper_Data),
   _elm_atspi_ewk_wrapper_class_initializer,
   NULL,
   NULL
};

EFL_DEFINE_CLASS(elm_atspi_ewk_wrapper_class_get, &_elm_atspi_ewk_wrapper_class_desc, EFL_OBJECT_CLASS, EFL_ACCESS_OBJECT_MIXIN, EFL_ACCESS_COMPONENT_MIXIN, NULL);

#include "elm_atspi_ewk_wrapper_eo.legacy.c"
