#ifndef _ELM_ATSPI_EWK_WRAPPER_EO_LEGACY_H_
#define _ELM_ATSPI_EWK_WRAPPER_EO_LEGACY_H_

#ifndef _ELM_ATSPI_EWK_WRAPPER_EO_CLASS_TYPE
#define _ELM_ATSPI_EWK_WRAPPER_EO_CLASS_TYPE

typedef Eo Elm_Atspi_Ewk_Wrapper;

#endif

#ifndef _ELM_ATSPI_EWK_WRAPPER_EO_TYPES
#define _ELM_ATSPI_EWK_WRAPPER_EO_TYPES


#endif

EAPI void elm_atspi_ewk_wrapper_ewk_constructor(Elm_Atspi_Ewk_Wrapper *obj, Efl_Object *ewk);

EAPI void elm_atspi_ewk_wrapper_connection_init(Elm_Atspi_Ewk_Wrapper *obj);

EAPI Efl_Object *elm_atspi_ewk_wrapper_ewk_view_get(const Elm_Atspi_Ewk_Wrapper *obj);

EAPI void elm_atspi_ewk_wrapper_a11y_init(Efl_Object *parent, Efl_Object *ewk);

#endif
