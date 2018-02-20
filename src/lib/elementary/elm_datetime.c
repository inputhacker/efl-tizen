#ifdef HAVE_CONFIG_H
# include "elementary_config.h"
#endif

#define EFL_ACCESS_PROTECTED

#include <Elementary.h>
#include "elm_priv.h"

#ifdef HAVE_LOCALE_H
# include <locale.h>
#endif

#ifdef HAVE_LANGINFO_H
# include <langinfo.h>
#endif

#include "elm_datetime.h"

EAPI Evas_Object *
elm_datetime_add(Evas_Object *parent)
{
   EINA_SAFETY_ON_NULL_RETURN_VAL(parent, NULL);
   return elm_legacy_add(EFL_UI_CLOCK_CLASS, parent);
}

EAPI void
elm_datetime_format_set(Evas_Object *obj,
                        const char *fmt)
{
   //TIZEN_ONLY(20180220): Considering wrong param has been passed case.
   EINA_SAFETY_ON_NULL_RETURN(obj);
   //

   efl_ui_clock_format_set(obj, fmt);
}

EAPI const char *
elm_datetime_format_get(const Evas_Object *obj)
{
   //TIZEN_ONLY(20180220): Considering wrong param has been passed case.
   EINA_SAFETY_ON_NULL_RETURN_VAL(obj, NULL);
   //

   return efl_ui_clock_format_get(obj);
}

static Efl_Ui_Clock_Type
adjust_field_type(Elm_Datetime_Field_Type type)
{
   Efl_Ui_Clock_Type ctype;

   switch(type)
     {
      case ELM_DATETIME_MONTH:
         ctype = EFL_UI_CLOCK_TYPE_MONTH;
         break;
      case ELM_DATETIME_DATE:
         ctype = EFL_UI_CLOCK_TYPE_DATE;
         break;
      case ELM_DATETIME_HOUR:
         ctype = EFL_UI_CLOCK_TYPE_HOUR;
         break;
      case ELM_DATETIME_MINUTE:
         ctype = EFL_UI_CLOCK_TYPE_MINUTE;
         break;
      case ELM_DATETIME_AMPM:
         ctype = EFL_UI_CLOCK_TYPE_AMPM;
         break;
      case ELM_DATETIME_YEAR:
      //TIZEN_ONLY(20180220): Considering wrong param has been passed case.
      //default:
      //   ctype = EFL_UI_CLOCK_TYPE_YEAR;
         ctype = EFL_UI_CLOCK_TYPE_YEAR;
         break;
      default:
         return -1;
      //
     }

   return ctype;
}

EAPI void
elm_datetime_field_limit_set(Evas_Object *obj, Elm_Datetime_Field_Type type, int min, int max)
{
   //TIZEN_ONLY(20180220): Considering wrong param has been passed case.
   EINA_SAFETY_ON_NULL_RETURN(obj);
   //

   efl_ui_clock_field_limit_set(obj, adjust_field_type(type), min, max);
}

EAPI void
elm_datetime_field_limit_get(const Evas_Object *obj, Elm_Datetime_Field_Type fieldtype, int *min, int *max)
{
   //TIZEN_ONLY(20180220): Considering wrong param has been passed case.
   EINA_SAFETY_ON_NULL_RETURN(obj);
   //

   efl_ui_clock_field_limit_get(obj, adjust_field_type(fieldtype), min, max);
}

EAPI Eina_Bool
elm_datetime_value_min_set(Evas_Object *obj, const Efl_Time *mintime)
{
   //TIZEN_ONLY(20180220): Considering wrong param has been passed case.
   //if (mintime) efl_ui_clock_time_min_set(obj, *mintime);
   EINA_SAFETY_ON_NULL_RETURN_VAL(obj, EINA_FALSE);
   EINA_SAFETY_ON_NULL_RETURN_VAL(mintime, EINA_FALSE);
   //

   efl_ui_clock_time_min_set(obj, *mintime);

   return EINA_TRUE;
}

EAPI Eina_Bool
elm_datetime_value_min_get(const Evas_Object *obj, Efl_Time *mintime)
{
   //TIZEN_ONLY(20180220): Considering wrong param has been passed case.
   //if (mintime) *mintime = efl_ui_clock_time_min_get(obj);
   EINA_SAFETY_ON_NULL_RETURN_VAL(obj, EINA_FALSE);
   EINA_SAFETY_ON_NULL_RETURN_VAL(mintime, EINA_FALSE);
   //

   *mintime = efl_ui_clock_time_min_get(obj);

   return EINA_TRUE;
}

EAPI Eina_Bool
elm_datetime_value_set(Evas_Object *obj, const Efl_Time *newtime)
{
   //TIZEN_ONLY(20180220): Considering wrong param has been passed case.
   //if (newtime) efl_ui_clock_time_set(obj, *newtime);
   EINA_SAFETY_ON_NULL_RETURN_VAL(obj, EINA_FALSE);
   EINA_SAFETY_ON_NULL_RETURN_VAL(newtime, EINA_FALSE);
   //

   efl_ui_clock_time_set(obj, *newtime);

   return EINA_TRUE;
}

EAPI Eina_Bool
elm_datetime_value_get(const Evas_Object *obj, Efl_Time *currtime)
{
   //TIZEN_ONLY(20180220): Considering wrong param has been passed case.
   //if (currtime) *currtime = efl_ui_clock_time_get(obj);
   EINA_SAFETY_ON_NULL_RETURN_VAL(obj, EINA_FALSE);
   EINA_SAFETY_ON_NULL_RETURN_VAL(currtime, EINA_FALSE);
   //

   *currtime = efl_ui_clock_time_get(obj);

   return EINA_TRUE;
}

EAPI void
elm_datetime_field_visible_set(Evas_Object *obj, Elm_Datetime_Field_Type fieldtype, Eina_Bool visible)
{
   //TIZEN_ONLY(20180220): Considering wrong param has been passed case.
   EINA_SAFETY_ON_NULL_RETURN(obj);
   //

   efl_ui_clock_field_visible_set(obj, adjust_field_type(fieldtype), visible);
}

EAPI Eina_Bool elm_datetime_field_visible_get(const Evas_Object *obj, Elm_Datetime_Field_Type fieldtype)
{
   //TIZEN_ONLY(20180220): Considering wrong param has been passed case.
   EINA_SAFETY_ON_NULL_RETURN_VAL(obj, EINA_FALSE);
   //

   return efl_ui_clock_field_visible_get(obj, adjust_field_type(fieldtype));
}

EAPI Eina_Bool
elm_datetime_value_max_set(Evas_Object *obj, const Efl_Time *maxtime)
{
   //TIZEN_ONLY(20180220): Considering wrong param has been passed case.
   //if (maxtime) efl_ui_clock_time_max_set(obj, *maxtime);
   EINA_SAFETY_ON_NULL_RETURN_VAL(obj, EINA_FALSE);
   EINA_SAFETY_ON_NULL_RETURN_VAL(maxtime, EINA_FALSE);
   //

   efl_ui_clock_time_max_set(obj, *maxtime);

   return EINA_TRUE;
}

EAPI Eina_Bool
elm_datetime_value_max_get(const Evas_Object *obj, Efl_Time *maxtime)
{
   //TIZEN_ONLY(20180220): Considering wrong param has been passed case.
   //if (maxtime) *maxtime = efl_ui_clock_time_max_get(obj);
   EINA_SAFETY_ON_NULL_RETURN_VAL(obj, EINA_FALSE);
   EINA_SAFETY_ON_NULL_RETURN_VAL(maxtime, EINA_FALSE);
   //

   *maxtime = efl_ui_clock_time_max_get(obj);

   return EINA_TRUE;
}
