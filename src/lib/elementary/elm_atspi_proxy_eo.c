EWAPI const Efl_Event_Description _ELM_ATSPI_PROXY_EVENT_CONNECTED =
   EFL_EVENT_DESCRIPTION("connected");
EWAPI const Efl_Event_Description _ELM_ATSPI_PROXY_EVENT_DISCONNECTED =
   EFL_EVENT_DESCRIPTION("disconnected");

void _elm_atspi_proxy_ctor(Eo *obj, Elm_Atspi_Proxy_Data *pd, Elm_Atspi_Proxy_Type type);

EOAPI EFL_VOID_FUNC_BODYV(elm_obj_atspi_proxy_ctor, EFL_FUNC_CALL(type), Elm_Atspi_Proxy_Type type);

void _elm_atspi_proxy_address_set(Eo *obj, Elm_Atspi_Proxy_Data *pd, const char *bus, const char *path);

EOAPI EFL_VOID_FUNC_BODYV(elm_obj_atspi_proxy_address_set, EFL_FUNC_CALL(bus, path), const char *bus, const char *path);

void _elm_atspi_proxy_address_get(const Eo *obj, Elm_Atspi_Proxy_Data *pd, const char **bus, const char **path);

EOAPI EFL_VOID_FUNC_BODYV_CONST(elm_obj_atspi_proxy_address_get, EFL_FUNC_CALL(bus, path), const char **bus, const char **path);

Elm_Atspi_Proxy_Type _elm_atspi_proxy_type_get(const Eo *obj, Elm_Atspi_Proxy_Data *pd);

EOAPI EFL_FUNC_BODY_CONST(elm_obj_atspi_proxy_type_get, Elm_Atspi_Proxy_Type, 0);

void _elm_atspi_proxy_address_get_retry_timer_add(Eo *obj, Elm_Atspi_Proxy_Data *pd);

EOAPI EFL_VOID_FUNC_BODY(elm_obj_atspi_proxy_address_get_retry_timer_add);

void _elm_atspi_proxy_efl_object_destructor(Eo *obj, Elm_Atspi_Proxy_Data *pd);


Eina_List *_elm_atspi_proxy_efl_access_object_access_children_get(const Eo *obj, Elm_Atspi_Proxy_Data *pd);


void _elm_atspi_proxy_efl_ui_widget_atspi(Eo *obj, Elm_Atspi_Proxy_Data *pd, Eina_Bool is_atspi);


static Eina_Bool
_elm_atspi_proxy_class_initializer(Efl_Class *klass)
{
   const Efl_Object_Ops *opsp = NULL;

   const Efl_Object_Property_Reflection_Ops *ropsp = NULL;

#ifndef ELM_ATSPI_PROXY_EXTRA_OPS
#define ELM_ATSPI_PROXY_EXTRA_OPS
#endif

   EFL_OPS_DEFINE(ops,
      EFL_OBJECT_OP_FUNC(elm_obj_atspi_proxy_ctor, _elm_atspi_proxy_ctor),
      EFL_OBJECT_OP_FUNC(elm_obj_atspi_proxy_address_set, _elm_atspi_proxy_address_set),
      EFL_OBJECT_OP_FUNC(elm_obj_atspi_proxy_address_get, _elm_atspi_proxy_address_get),
      EFL_OBJECT_OP_FUNC(elm_obj_atspi_proxy_type_get, _elm_atspi_proxy_type_get),
      EFL_OBJECT_OP_FUNC(elm_obj_atspi_proxy_address_get_retry_timer_add, _elm_atspi_proxy_address_get_retry_timer_add),
      EFL_OBJECT_OP_FUNC(efl_destructor, _elm_atspi_proxy_efl_object_destructor),
      EFL_OBJECT_OP_FUNC(efl_access_object_access_children_get, _elm_atspi_proxy_efl_access_object_access_children_get),
      EFL_OBJECT_OP_FUNC(efl_ui_widget_atspi, _elm_atspi_proxy_efl_ui_widget_atspi),
      ELM_ATSPI_PROXY_EXTRA_OPS
   );
   opsp = &ops;

   return efl_class_functions_set(klass, opsp, ropsp);
}

static const Efl_Class_Description _elm_atspi_proxy_class_desc = {
   EO_VERSION,
   "Elm.Atspi.Proxy",
   EFL_CLASS_TYPE_REGULAR,
   sizeof(Elm_Atspi_Proxy_Data),
   _elm_atspi_proxy_class_initializer,
   NULL,
   NULL
};

EFL_DEFINE_CLASS(elm_atspi_proxy_class_get, &_elm_atspi_proxy_class_desc, EFL_UI_WIDGET_CLASS, NULL);
