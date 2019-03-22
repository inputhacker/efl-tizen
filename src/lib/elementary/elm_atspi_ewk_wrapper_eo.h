#ifndef _ELM_ATSPI_EWK_WRAPPER_EO_H_
#define _ELM_ATSPI_EWK_WRAPPER_EO_H_

#ifndef _ELM_ATSPI_EWK_WRAPPER_EO_CLASS_TYPE
#define _ELM_ATSPI_EWK_WRAPPER_EO_CLASS_TYPE

typedef Eo Elm_Atspi_Ewk_Wrapper;

#endif

#ifndef _ELM_ATSPI_EWK_WRAPPER_EO_TYPES
#define _ELM_ATSPI_EWK_WRAPPER_EO_TYPES


#endif
#define ELM_ATSPI_EWK_WRAPPER_CLASS elm_atspi_ewk_wrapper_class_get()

EWAPI const Efl_Class *elm_atspi_ewk_wrapper_class_get(void);

EOAPI void elm_obj_atspi_ewk_wrapper_ewk_constructor(Eo *obj, Efl_Object *ewk);

EOAPI void elm_obj_atspi_ewk_wrapper_connection_init(Eo *obj);

EOAPI Efl_Object *elm_obj_atspi_ewk_wrapper_ewk_view_get(const Eo *obj);

EOAPI void elm_obj_atspi_ewk_wrapper_a11y_init(Efl_Object *parent, Efl_Object *ewk);

#endif
