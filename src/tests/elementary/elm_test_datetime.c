#ifdef HAVE_CONFIG_H
# include "elementary_config.h"
#endif

#define EFL_ACCESS_BETA
#include <Elementary.h>
#include "elm_suite.h"

EFL_START_TEST (elm_datetime_legacy_type_check)
{
   Evas_Object *win, *datetime;
   const char *type;

   char *args[] = { "exe" };
   elm_init(1, args);
   win = elm_win_add(NULL, "datetime", ELM_WIN_BASIC);

   datetime = elm_datetime_add(win);

   type = elm_object_widget_type_get(datetime);
   ck_assert(type != NULL);
   ck_assert(!strcmp(type, "Elm_Datetime"));

   type = evas_object_type_get(datetime);
   ck_assert(type != NULL);
   ck_assert(!strcmp(type, "elm_datetime"));

   elm_shutdown();
}
EFL_END_TEST

EFL_START_TEST (elm_atspi_role_get)
{
   Evas_Object *win, *datetime;
   Efl_Access_Role role;

   char *args[] = { "exe" };
   elm_init(1, args);
   win = elm_win_add(NULL, "datetime", ELM_WIN_BASIC);

   datetime = elm_datetime_add(win);
   role = efl_access_role_get(datetime);

   ck_assert(role == EFL_ACCESS_ROLE_DATE_EDITOR);

   elm_shutdown();
}
EFL_END_TEST

void elm_test_datetime(TCase *tc)
{
   tcase_add_test(tc, elm_datetime_legacy_type_check);
   tcase_add_test(tc, elm_atspi_role_get);
}
