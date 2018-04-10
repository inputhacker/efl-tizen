#ifdef HAVE_CONFIG_H
# include "elementary_config.h"
#endif

#define EFL_ACCESS_BETA
#include <Elementary.h>
#include "elm_suite.h"

EFL_START_TEST (elm_progressbar_legacy_type_check)
{
   Evas_Object *win, *progressbar;
   const char *type;

   win = elm_win_add(NULL, "progressbar", ELM_WIN_BASIC);

   progressbar = elm_progressbar_add(win);

   type = elm_object_widget_type_get(progressbar);
   ck_assert(type != NULL);
   ck_assert(!strcmp(type, "Elm_Progressbar"));

   type = evas_object_type_get(progressbar);
   ck_assert(type != NULL);
   ck_assert(!strcmp(type, "elm_progressbar"));

}
EFL_END_TEST

EFL_START_TEST (elm_atspi_role_get)
{
   Evas_Object *win, *progressbar;
   Efl_Access_Role role;

   win = elm_win_add(NULL, "progressbar", ELM_WIN_BASIC);

   progressbar = elm_progressbar_add(win);
   role = efl_access_role_get(progressbar);

   ck_assert(role == EFL_ACCESS_ROLE_PROGRESS_BAR);

}
EFL_END_TEST

void elm_test_progressbar(TCase *tc)
{
   tcase_add_test(tc, elm_progressbar_legacy_type_check);
   tcase_add_test(tc, elm_atspi_role_get);
}
