
EAPI void
elm_atspi_ewk_wrapper_ewk_constructor(Elm_Atspi_Ewk_Wrapper *obj, Efl_Object *ewk)
{
   elm_obj_atspi_ewk_wrapper_ewk_constructor(obj, ewk);
}

EAPI void
elm_atspi_ewk_wrapper_connection_init(Elm_Atspi_Ewk_Wrapper *obj)
{
   elm_obj_atspi_ewk_wrapper_connection_init(obj);
}

EAPI Efl_Object *
elm_atspi_ewk_wrapper_ewk_view_get(const Elm_Atspi_Ewk_Wrapper *obj)
{
   return elm_obj_atspi_ewk_wrapper_ewk_view_get(obj);
}
