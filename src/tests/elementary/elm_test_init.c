#ifdef HAVE_CONFIG_H
# include "elementary_config.h"
#endif

#include <Elementary.h>
#include "elm_suite.h"

EFL_START_TEST (elm_main)
{
   char *args[] = { "exe" };
   ck_assert(elm_init(1, args) == 1);
   ck_assert(elm_shutdown() == 0);
   ck_assert(ecore_init() == 1);
   ck_assert(ecore_shutdown() == 0);
}
EFL_END_TEST

void elm_test_init(TCase *tc)
{
   tcase_add_test(tc, elm_main);
}
