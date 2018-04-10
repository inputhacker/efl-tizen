#ifdef HAVE_CONFIG_H
# include "config.h"
#endif

#include <Eina.h>
#include "eolian_database.h"

EAPI Eolian_Variable_Type
eolian_variable_type_get(const Eolian_Variable *var)
{
   EINA_SAFETY_ON_NULL_RETURN_VAL(var, EOLIAN_VAR_UNKNOWN);
   return var->type;
}

EAPI const Eolian_Documentation *
eolian_variable_documentation_get(const Eolian_Variable *var)
{
   EINA_SAFETY_ON_NULL_RETURN_VAL(var, NULL);
   return var->doc;
}

EAPI const Eolian_Type *
eolian_variable_base_type_get(const Eolian_Variable *var)
{
   EINA_SAFETY_ON_NULL_RETURN_VAL(var, NULL);
   return var->base_type;
}

EAPI const Eolian_Expression *
eolian_variable_value_get(const Eolian_Variable *var)
{
   EINA_SAFETY_ON_NULL_RETURN_VAL(var, NULL);
   return var->value;
}

EAPI Eina_Stringshare *
eolian_variable_name_get(const Eolian_Variable *var)
{
   EINA_SAFETY_ON_NULL_RETURN_VAL(var, NULL);
   return var->name;
}

EAPI Eina_Stringshare *
eolian_variable_full_name_get(const Eolian_Variable *var)
{
   EINA_SAFETY_ON_NULL_RETURN_VAL(var, NULL);
   return var->base.name;
}

EAPI Eina_Iterator *
eolian_variable_namespaces_get(const Eolian_Variable *var)
{
   EINA_SAFETY_ON_NULL_RETURN_VAL(var, NULL);
   if (!var->namespaces) return NULL;
   return eina_list_iterator_new(var->namespaces);
}

EAPI Eina_Bool
eolian_variable_is_extern(const Eolian_Variable *var)
{
   EINA_SAFETY_ON_NULL_RETURN_VAL(var, EINA_FALSE);
   return var->is_extern;
}
