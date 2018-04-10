#include "elm_test_focus_common.h"

START_TEST(focus_unregister_twice)
{
   char *args[] = { "exe" };
   elm_init(1, args);
   Efl_Ui_Focus_Object *r1 = efl_add(FOCUS_TEST_CLASS, efl_main_loop_get());
   Efl_Ui_Focus_Object *r2 = efl_add(FOCUS_TEST_CLASS, efl_main_loop_get());

   Efl_Ui_Focus_Manager *m = efl_add(EFL_UI_FOCUS_MANAGER_CALC_CLASS, efl_main_loop_get(),
    efl_ui_focus_manager_root_set(efl_added, r1)
   );

   fail_if(!efl_ui_focus_manager_calc_register(m, r2, r1, NULL));

   efl_ui_focus_manager_calc_unregister(m, r1);
   efl_ui_focus_manager_calc_unregister(m, r1);
   efl_ui_focus_manager_calc_unregister(m, r1);

   efl_del(r2);
   efl_del(r1);
   efl_del(m);

   elm_shutdown();
}
END_TEST

START_TEST(focus_register_twice)
{
   char *args[] = { "exe" };
   elm_init(1, args);

   Efl_Ui_Focus_Object *r1 = elm_focus_test_object_new("r1", 0, 0, 10, 10);
   Efl_Ui_Focus_Object *r2 = elm_focus_test_object_new("r2", 0, 10, 10, 10);

   Efl_Ui_Focus_Manager *m = efl_add(EFL_UI_FOCUS_MANAGER_CALC_CLASS, efl_main_loop_get(),
    efl_ui_focus_manager_root_set(efl_added, r1)
   );

   fail_if(!efl_ui_focus_manager_calc_register(m, r2, r1, NULL));
   fail_if(efl_ui_focus_manager_calc_register(m, r2, r1, NULL));

   efl_del(r1);
   efl_del(m);

   elm_shutdown();
}
END_TEST

START_TEST(pos_check)
{
   Efl_Ui_Focus_Manager *m;
   Efl_Ui_Focus_Object *middle, *east, *west, *north, *south, *root;

   char *args[] = { "exe" };
   elm_init(1, args);

   elm_focus_test_setup_cross(&middle, &south, &north, &east, &west);

   m = elm_focus_test_manager_new(&root);
   efl_ui_focus_manager_calc_register(m, middle, root, NULL);
   efl_ui_focus_manager_calc_register(m, north, root, NULL);
   efl_ui_focus_manager_calc_register(m, south, root, NULL);
   efl_ui_focus_manager_calc_register(m, west, root, NULL);
   efl_ui_focus_manager_calc_register(m, east, root, NULL);

#define CHECK(obj, r,l,u,d) \
   efl_ui_focus_manager_focus_set(m, obj); \
   ck_assert_ptr_eq(efl_ui_focus_manager_move(m, EFL_UI_FOCUS_DIRECTION_RIGHT), r); \
   efl_ui_focus_manager_focus_set(m, obj); \
   ck_assert_ptr_eq(efl_ui_focus_manager_move(m, EFL_UI_FOCUS_DIRECTION_LEFT), l); \
   efl_ui_focus_manager_focus_set(m, obj); \
   ck_assert_ptr_eq(efl_ui_focus_manager_move(m, EFL_UI_FOCUS_DIRECTION_UP), u); \
   efl_ui_focus_manager_focus_set(m, obj); \
   ck_assert_ptr_eq(efl_ui_focus_manager_move(m, EFL_UI_FOCUS_DIRECTION_DOWN), d); \
   efl_ui_focus_manager_focus_set(m, obj);

   CHECK(middle, east, west, north, south)
   CHECK(east, NULL, middle, north, south)
   CHECK(west, middle, NULL, north, south)
   CHECK(north, east, west, NULL, middle)
   CHECK(south, east, west, middle, NULL)

   efl_del(middle);
   efl_del(south);
   efl_del(north);
   efl_del(east);
   efl_del(west);
   efl_del(m);

   elm_shutdown();
}
END_TEST

static Eina_Bool
_equal_set(Eina_List *elems, Efl_Ui_Focus_Object *lst[])
{
   unsigned int i = 0;

   for (i = 0; lst[i]; ++i)
     {
        Eina_Bool found = EINA_FALSE;
        Eina_List *n;
        Efl_Ui_Focus_Object *elem;

        EINA_LIST_FOREACH(elems, n, elem)
          {
            if (lst[i] != elem) continue;

            found = EINA_TRUE;
            break;
          }

        if (!found) return EINA_FALSE;
     }

   if (eina_list_count(elems) != i) return EINA_FALSE;
   return EINA_TRUE;
}

START_TEST(pos_check2)
{
   Efl_Ui_Focus_Manager *m;
   Efl_Ui_Focus_Relations *rel;
   Efl_Ui_Focus_Object *root, *middle, *north_east, *north_west, *south_east, *south_west;

   char *args[] = { "exe" };
   elm_init(1, args);

   middle = elm_focus_test_object_new("middle", 40, 40, 5, 5);

   north_east = elm_focus_test_object_new("north_east", 60, 20, 5, 5);
   north_west = elm_focus_test_object_new("north_west", 20, 20, 5, 5);
   south_east = elm_focus_test_object_new("south_east", 60, 60, 5, 5);
   south_west = elm_focus_test_object_new("south_west", 20, 60, 5, 5);

   m = elm_focus_test_manager_new(&root);
   efl_ui_focus_manager_calc_register(m, middle, root, NULL);
   efl_ui_focus_manager_calc_register(m, north_east, root, NULL);
   efl_ui_focus_manager_calc_register(m, north_west, root, NULL);
   efl_ui_focus_manager_calc_register(m, south_east, root, NULL);
   efl_ui_focus_manager_calc_register(m, south_west, root, NULL);

   rel = efl_ui_focus_manager_fetch(m, middle);

#define ck_assert_set_eq(set, ...) \
   { \
      Efl_Ui_Focus_Object *tmp[] = { __VA_ARGS__ }; \
      ck_assert_int_eq(_equal_set(set, tmp), EINA_TRUE); \
   }

   ck_assert_set_eq(rel->left, north_west, south_west, NULL);
   ck_assert_set_eq(rel->right, north_east, south_east, NULL);
   ck_assert_set_eq(rel->top, north_west, north_east, NULL);
   ck_assert_set_eq(rel->down, south_west, south_east, NULL);

#undef ck_assert_set_eq

   efl_del(middle);
   efl_del(north_east);
   efl_del(north_west);
   efl_del(south_east);
   efl_del(south_west);

   elm_shutdown();
}
END_TEST
START_TEST(redirect)
{
   char *args[] = { "exe" };
   elm_init(1, args);

   TEST_OBJ_NEW(root, 0, 0, 20, 20);
   TEST_OBJ_NEW(root2, 0, 0, 20, 20);
   TEST_OBJ_NEW(one, 0, 0, 20, 20);
   TEST_OBJ_NEW(two, 20, 0, 20, 20);

   Efl_Ui_Focus_Manager *m = efl_add(EFL_UI_FOCUS_MANAGER_CALC_CLASS, efl_main_loop_get(),
    efl_ui_focus_manager_root_set(efl_added, root)
   );

   Efl_Ui_Focus_Manager *m2 = efl_add(EFL_UI_FOCUS_MANAGER_CALC_CLASS, efl_main_loop_get(),
    efl_ui_focus_manager_root_set(efl_added, root2)
   );

   efl_ui_focus_manager_calc_register(m2, one, root2, NULL);
   efl_ui_focus_manager_calc_register(m2, two, root2, NULL);

   efl_ui_focus_manager_redirect_set(m, m2);
   efl_ui_focus_manager_focus_set(m2, one);

   ck_assert_ptr_eq(efl_ui_focus_manager_move(m, EFL_UI_FOCUS_DIRECTION_RIGHT), two);

   efl_del(m);
   efl_del(m2);

   elm_shutdown();
}
END_TEST

START_TEST(border_check)
{
   Efl_Ui_Focus_Manager *m;
   Efl_Ui_Focus_Object *middle, *east, *west, *north, *south, *root;
   Eina_List *list = NULL;
   Eina_Iterator *iter;
   Efl_Ui_Focus_Object *obj;

   char *args[] = { "exe" };
   elm_init(1, args);

   elm_focus_test_setup_cross(&middle, &south, &north, &east, &west);

   m = elm_focus_test_manager_new(&root);
   efl_ui_focus_manager_calc_register(m, middle, root, NULL);
   efl_ui_focus_manager_calc_register(m, south, root, NULL);
   efl_ui_focus_manager_calc_register(m, north, root, NULL);
   efl_ui_focus_manager_calc_register(m, east, root, NULL);
   efl_ui_focus_manager_calc_register(m, west, root, NULL);

   iter = efl_ui_focus_manager_border_elements_get(m);

   EINA_ITERATOR_FOREACH(iter, obj)
     {
        list = eina_list_append(list, obj);
     }

   eina_iterator_free(iter);

   ck_assert(eina_list_data_find(list, east) == east);
   ck_assert(eina_list_data_find(list, north) == north);
   ck_assert(eina_list_data_find(list, west) == west);
   ck_assert(eina_list_data_find(list, east) == east);
   ck_assert(eina_list_data_find(list, middle) == NULL);
   ck_assert(eina_list_count(list) == 4);

   elm_shutdown();
}
END_TEST

START_TEST(logical_chain)
{
   Efl_Ui_Focus_Manager *m;
   Efl_Ui_Focus_Object *lroot;
   int i = 0;

   char *args[] = { "exe" };
   elm_init(1, args);

   TEST_OBJ_NEW(root, 0, 0, 20, 20);

   m = elm_focus_test_manager_new(&lroot);

   fail_if(!m);

   i++;
   TEST_OBJ_NEW(child1, 0, i*20, 20, 20);
   i++;
   TEST_OBJ_NEW(child2, 0, i*20, 20, 20);
   i++;
   TEST_OBJ_NEW(child3, 0, i*20, 20, 20);

   i++;
   TEST_OBJ_NEW(subchild11, 0, i*20, 20, 20);
   i++;
   TEST_OBJ_NEW(subchild12, 0, i*20, 20, 20);
   i++;
   TEST_OBJ_NEW(subchild13, 0, i*20, 20, 20);

   i++;
   TEST_OBJ_NEW(subchild21, 0, i*20, 20, 20);
   i++;
   TEST_OBJ_NEW(subchild22, 0, i*20, 20, 20);
   i++;
   TEST_OBJ_NEW(subchild23, 0, i*20, 20, 20);

   //register everything
   efl_ui_focus_manager_calc_register(m, root, lroot, NULL);
   efl_ui_focus_manager_calc_register(m, child1, root, NULL);
   efl_ui_focus_manager_calc_register(m, child2, root, NULL);
   efl_ui_focus_manager_calc_register_logical(m, child3, root, NULL);
   efl_ui_focus_manager_calc_register(m, subchild11, child1, NULL);
   efl_ui_focus_manager_calc_register(m, subchild12, child1, NULL);
   efl_ui_focus_manager_calc_register(m, subchild13, child1, NULL);
   efl_ui_focus_manager_calc_register(m, subchild21, child3, NULL);
   efl_ui_focus_manager_calc_register(m, subchild22, child3, NULL);
   efl_ui_focus_manager_calc_register(m, subchild23, child3, NULL);

   efl_ui_focus_manager_focus_set(m, root);

   Efl_Object *logical_chain[] = {
    child1, subchild11, subchild12, subchild13,
    child2, subchild21, subchild22, subchild23, NULL
   };
   for (i = 0; logical_chain[i]; ++i)
     {
        ck_assert_ptr_eq(logical_chain[i], efl_ui_focus_manager_move(m, EFL_UI_FOCUS_DIRECTION_NEXT));
     }
   i-= 2;
   for (; i > 0; --i)
     {
        ck_assert_ptr_eq(logical_chain[i], efl_ui_focus_manager_move(m, EFL_UI_FOCUS_DIRECTION_PREVIOUS));
     }
   elm_shutdown();
}
END_TEST

START_TEST(finalize_check)
{
   Efl_Ui_Focus_Manager *m;

   char *args[] = { "exe" };
   elm_init(1, args);

   m = efl_add(EFL_UI_FOCUS_MANAGER_CALC_CLASS, efl_main_loop_get());
   fail_if(m);

   efl_del(m);
   elm_shutdown();
}
END_TEST

START_TEST(redirect_param)
{
   Efl_Ui_Focus_Manager *m, *m2;

   char *args[] = { "exe" };
   elm_init(1, args);

   TEST_OBJ_NEW(root, 0, 20, 20, 20);
   TEST_OBJ_NEW(root2, 0, 20, 20, 20);
   TEST_OBJ_NEW(child, 0, 20, 20, 20);

   m = efl_add(EFL_UI_FOCUS_MANAGER_CALC_CLASS, efl_main_loop_get(),
    efl_ui_focus_manager_root_set(efl_added, root)
   );

   m2 = efl_add(EFL_UI_FOCUS_MANAGER_CALC_CLASS, efl_main_loop_get(),
    efl_ui_focus_manager_root_set(efl_added, root2)
   );

   efl_ui_focus_manager_calc_register(m, child, root, m2);
   efl_ui_focus_manager_focus_set(m, child);

   ck_assert_ptr_eq(efl_ui_focus_manager_redirect_get(m), m2);

   efl_del(m);
   efl_del(m2);
   elm_shutdown();
}
END_TEST

START_TEST(invalid_args_check)
{
   Efl_Ui_Focus_Manager *m;

   char *args[] = { "exe" };
   elm_init(1, args);

   TEST_OBJ_NEW(root, 0, 20, 20, 20);
   TEST_OBJ_NEW(child, 0, 20, 20, 20);
   TEST_OBJ_NEW(child2, 0, 20, 20, 20);

   m = efl_add(EFL_UI_FOCUS_MANAGER_CALC_CLASS, efl_main_loop_get(),
    efl_ui_focus_manager_root_set(efl_added, root)
   );

   //no child and no parent
   ck_assert_int_eq(efl_ui_focus_manager_calc_register(m, NULL, NULL, NULL), 0);
   ck_assert_int_eq(efl_ui_focus_manager_calc_register(m, child, NULL, NULL), 0);
   ck_assert_int_eq(efl_ui_focus_manager_calc_register(m, NULL, root, NULL), 0);

   ck_assert_int_eq(efl_ui_focus_manager_calc_register(m, child, root, NULL), 1);

   ck_assert_int_eq(efl_ui_focus_manager_calc_update_parent(m, child, NULL), 0);
   ck_assert_int_eq(efl_ui_focus_manager_calc_update_parent(m, NULL, NULL), 0);
   ck_assert_int_eq(efl_ui_focus_manager_calc_update_parent(m, child, child2), 0);

   ck_assert_int_eq(efl_ui_focus_manager_calc_register(m, child2, root, NULL), 1);
   ck_assert_int_eq(efl_ui_focus_manager_calc_update_parent(m, child, child2), 1);

   efl_del(m);
   elm_shutdown();
}
END_TEST

START_TEST(order_check)
{
   Efl_Ui_Focus_Manager *m;
   Eina_List *order = NULL;

   char *args[] = { "exe" };
   elm_init(1, args);

   TEST_OBJ_NEW(root, 0, 20, 20, 20);
   TEST_OBJ_NEW(child1, 0, 20, 20, 20);
   TEST_OBJ_NEW(child2, 0, 20, 20, 20);
   TEST_OBJ_NEW(child3, 0, 20, 20, 20);

   m = efl_add(EFL_UI_FOCUS_MANAGER_CALC_CLASS, efl_main_loop_get(),
    efl_ui_focus_manager_root_set(efl_added, root)
   );

   //no child and no parent
   efl_ui_focus_manager_calc_register(m, child1, root, NULL);
   efl_ui_focus_manager_calc_register(m, child2, root, NULL);
   efl_ui_focus_manager_calc_register(m, child3, root, NULL);

   //positiv check
   order = eina_list_append(order, child2);
   order = eina_list_append(order, child3);
   order = eina_list_append(order, child1);
   ck_assert_int_eq(efl_ui_focus_manager_calc_update_children(m, root, order), 1);

   order = NULL;

   //negativ check
   order = eina_list_append(order, child1);
   order = eina_list_append(order, child2);
   ck_assert_int_eq(efl_ui_focus_manager_calc_update_children(m, root, order), 0);

   efl_del(m);
   elm_shutdown();
}
END_TEST

START_TEST(logical_shift)
{
   Efl_Ui_Focus_Manager *m;

   char *args[] = { "exe" };
   elm_init(1, args);

   TEST_OBJ_NEW(root, 0, 0, 10, 10);
   TEST_OBJ_NEW(child, 0, 0, 10, 10);
   TEST_OBJ_NEW(sub, 0, 0, 10, 10);
   TEST_OBJ_NEW(sub_sub, 0, 0, 10, 10);
   TEST_OBJ_NEW(sub_child, 0, 0, 10, 10);

   m = efl_add(EFL_UI_FOCUS_MANAGER_CALC_CLASS, efl_main_loop_get(),
    efl_ui_focus_manager_root_set(efl_added, root)
   );

   efl_ui_focus_manager_calc_register(m, child, root, NULL);
   efl_ui_focus_manager_calc_register_logical(m, sub, root, NULL);
   efl_ui_focus_manager_calc_register_logical(m, sub_sub, sub, NULL);
   efl_ui_focus_manager_calc_register(m, sub_child, sub, NULL);

   efl_ui_focus_manager_focus_set(m, root);
   ck_assert_ptr_eq(efl_ui_focus_manager_focus_get(m), child);

   efl_ui_focus_manager_focus_set(m, sub);
   ck_assert_ptr_eq(efl_ui_focus_manager_focus_get(m), sub_child);

   efl_ui_focus_manager_focus_set(m, sub_sub);
   ck_assert_ptr_eq(efl_ui_focus_manager_focus_get(m), sub_child);

   efl_del(m);
   elm_shutdown();
}
END_TEST

START_TEST(root_redirect_chain)
{
   Efl_Ui_Focus_Manager *m, *m2;

   char *args[] = { "exe" };
   elm_init(1, args);

   TEST_OBJ_NEW(root, 0, 20, 20, 20);
   TEST_OBJ_NEW(root2, 0, 20, 20, 20);
   TEST_OBJ_NEW(child, 0, 20, 20, 20);

   m = efl_add(EFL_UI_FOCUS_MANAGER_CALC_CLASS, efl_main_loop_get(),
    efl_ui_focus_manager_root_set(efl_added, root)
   );

   focus_test_manager_set(root2, m);

   m2 = efl_add(EFL_UI_FOCUS_MANAGER_CALC_CLASS, efl_main_loop_get(),
    efl_ui_focus_manager_root_set(efl_added, root2)
   );

   efl_ui_focus_manager_calc_register(m, root2, root, m2);
   efl_ui_focus_manager_calc_register(m2, child, root2, NULL);
   efl_ui_focus_manager_focus_set(m2, child);

   ck_assert_ptr_eq(efl_ui_focus_manager_redirect_get(m), m2);

   efl_del(m);
   efl_del(m2);
   elm_shutdown();
}
END_TEST

START_TEST(root_redirect_chain_unset)
{
   Efl_Ui_Focus_Manager *m, *m2, *m3, *m4;

   char *args[] = { "exe" };
   elm_init(1, args);

   TEST_OBJ_NEW(root, 0, 20, 20, 20);
   TEST_OBJ_NEW(root2, 0, 20, 20, 20);
   TEST_OBJ_NEW(child, 0, 20, 20, 20);

   m = efl_add(EFL_UI_FOCUS_MANAGER_CALC_CLASS, efl_main_loop_get(),
    efl_ui_focus_manager_root_set(efl_added, root)
   );

   focus_test_manager_set(root2, m);

   m2 = efl_add(EFL_UI_FOCUS_MANAGER_CALC_CLASS, efl_main_loop_get(),
    efl_ui_focus_manager_root_set(efl_added, root2)
   );

   m3 = efl_add(EFL_UI_FOCUS_MANAGER_CALC_CLASS, efl_main_loop_get(),
    efl_ui_focus_manager_root_set(efl_added, root2)
   );

   m4 = efl_add(EFL_UI_FOCUS_MANAGER_CALC_CLASS, efl_main_loop_get(),
    efl_ui_focus_manager_root_set(efl_added, root2)
   );

   efl_ui_focus_manager_redirect_set(m, m3);
   efl_ui_focus_manager_redirect_set(m3, m4);
   efl_ui_focus_manager_redirect_set(m4, m2);

   efl_ui_focus_manager_calc_register(m, root2, root, m2);
   efl_ui_focus_manager_calc_register(m2, child, root2, NULL);
   efl_ui_focus_manager_focus_set(m2, child);

   ck_assert_ptr_eq(efl_ui_focus_manager_redirect_get(m), m2);
   ck_assert_ptr_eq(efl_ui_focus_manager_redirect_get(m2), NULL);
   ck_assert_ptr_eq(efl_ui_focus_manager_redirect_get(m3), NULL);
   ck_assert_ptr_eq(efl_ui_focus_manager_redirect_get(m4), NULL);

   efl_del(m);
   efl_del(m2);
   efl_del(m3);
   efl_del(m4);
   elm_shutdown();
}
END_TEST

static Efl_Ui_Focus_Manager_Calc*
_recursive_triangle_manager(int recusion_depth, Efl_Ui_Focus_Object **most_right, Efl_Ui_Focus_Object **most_left, Eina_List **managers)
{
   Efl_Ui_Focus_Manager *m, *m_child1 = NULL, *m_child3 = NULL;
   Efl_Ui_Focus_Object *child1, *child3;

   TEST_OBJ_NEW(root, 0, 20, 20, 20);
   TEST_OBJ_NEW(child2, 0, 20, 20, 20);

   m = efl_add(EFL_UI_FOCUS_MANAGER_CALC_CLASS, efl_main_loop_get(),
    efl_ui_focus_manager_root_set(efl_added, root)
   );

   printf("New manaager %p\n", m);

   if (recusion_depth < 3)
     {
        m_child1 = _recursive_triangle_manager(recusion_depth + 1, NULL, most_left, managers);
        m_child3 = _recursive_triangle_manager(recusion_depth + 1, most_right, NULL, managers);
        child1 = efl_ui_focus_manager_root_get(m_child1);
        child3 = efl_ui_focus_manager_root_get(m_child3);
        focus_test_manager_set(child1, m);
        focus_test_manager_set(child3, m);
     }
   else
     {
        TEST_OBJ_NEW(tmp_child1, 0, 20, 20, 20);
        TEST_OBJ_NEW(tmp_child3, 0, 20, 20, 20);

        child1 = tmp_child1;
        child3 = tmp_child3;

        if (most_left)
          {
             *most_left = child1;
             printf("SETTING %p\n", child1);
          }

        if (most_right)
          {
             *most_right = child3;
             printf("SETTING %p\n", child3);
          }
     }

   printf("Child 1 %p\n", child1);
   printf("Child 2 %p\n", child2);
   printf("Child 3 %p\n", child3);

   efl_ui_focus_manager_calc_register(m, child1, root, m_child1);
   efl_ui_focus_manager_calc_register(m, child2, root, NULL);
   efl_ui_focus_manager_calc_register(m, child3, root, m_child3);

   *managers = eina_list_append(*managers , m);

   return m;
}

static Efl_Ui_Focus_Object*
_get_highest_redirect(Efl_Ui_Focus_Manager *manager)
{
   Efl_Ui_Focus_Manager *tmp = manager;

   do {
     manager = tmp;
     tmp = efl_ui_focus_manager_redirect_get(tmp);
   } while (tmp);

   return manager;
}

START_TEST(first_touch_check)
{
   Efl_Ui_Focus_Manager *m;
   Efl_Ui_Focus_Object *most_left, *most_right;
   Eina_List *managers = NULL;

   char *args[] = { "exe" };
   elm_init(1, args);

   m = _recursive_triangle_manager(0, &most_right, &most_left, &managers);

   efl_ui_focus_manager_setup_on_first_touch(m, EFL_UI_FOCUS_DIRECTION_NEXT, efl_ui_focus_manager_root_get(m));
   printf("%p %p\n", most_left, most_right);

   ck_assert_ptr_eq(efl_ui_focus_manager_focus_get(_get_highest_redirect(m)), most_left);

   efl_ui_focus_manager_reset_history(m);

   efl_ui_focus_manager_setup_on_first_touch(m, EFL_UI_FOCUS_DIRECTION_PREVIOUS, efl_ui_focus_manager_root_get(m));
   printf("%s\n", efl_name_get(efl_ui_focus_manager_focus_get(_get_highest_redirect(m))));
   ck_assert_ptr_eq(efl_ui_focus_manager_focus_get(_get_highest_redirect(m)), most_right);

   EINA_LIST_FREE(managers, m)
     efl_del(m);

   elm_shutdown();
}
END_TEST

START_TEST(test_request_subchild_empty)
{
   Efl_Ui_Focus_Manager *m;
   Efl_Ui_Focus_Object *root;

   char *args[] = { "exe" };
   elm_init(1, args);

   m = elm_focus_test_manager_new(&root);

   ck_assert_ptr_eq(efl_ui_focus_manager_request_subchild(m, root), NULL);

   efl_del(m);
   elm_shutdown();
}
END_TEST

START_TEST(test_request_subchild_one_element)
{
   Efl_Ui_Focus_Manager *m;
   Efl_Ui_Focus_Object *root, *c1;

   char *args[] = { "exe" };
   elm_init(1, args);

   m = elm_focus_test_manager_new(&root);
   c1 = elm_focus_test_object_new("child1", 0, 0, 20, 20);
   efl_ui_focus_manager_calc_register(m, c1, root, NULL);

   ck_assert_ptr_eq(efl_ui_focus_manager_request_subchild(m, root), c1);
   ck_assert_ptr_eq(efl_ui_focus_manager_request_subchild(m, c1), NULL);

   efl_del(m);
   elm_shutdown();
}
END_TEST

START_TEST(test_request_subchild_child_alongside)
{
   Efl_Ui_Focus_Manager *m;
   Efl_Ui_Focus_Object *root, *c1, *c2;

   char *args[] = { "exe" };
   elm_init(1, args);

   m = elm_focus_test_manager_new(&root);
   c1 = elm_focus_test_object_new("child1", 0, 0, 20, 20);
   c2 = elm_focus_test_object_new("child2", 0, 0, 20, 20);
   efl_ui_focus_manager_calc_register(m, c1, root, NULL);
   efl_ui_focus_manager_calc_register(m, c2, root, NULL);

   ck_assert_ptr_eq(efl_ui_focus_manager_request_subchild(m, root), c1);
   ck_assert_ptr_eq(efl_ui_focus_manager_request_subchild(m, c1), NULL);
   ck_assert_ptr_eq(efl_ui_focus_manager_request_subchild(m, c2), NULL);

   efl_del(m);
   elm_shutdown();
}
END_TEST

START_TEST(test_request_subchild_child_logical_regular)
{
   Efl_Ui_Focus_Manager *m;
   Efl_Ui_Focus_Object *root, *c1, *c2;

   char *args[] = { "exe" };
   elm_init(1, args);

   m = elm_focus_test_manager_new(&root);
   c1 = elm_focus_test_object_new("child1", 0, 0, 20, 20);
   c2 = elm_focus_test_object_new("child2", 0, 0, 20, 20);
   efl_ui_focus_manager_calc_register_logical(m, c1, root, NULL);
   efl_ui_focus_manager_calc_register(m, c2, c1, NULL);

   ck_assert_ptr_eq(efl_ui_focus_manager_request_subchild(m, root), c2);
   ck_assert_ptr_eq(efl_ui_focus_manager_request_subchild(m, c1), c2);
   ck_assert_ptr_eq(efl_ui_focus_manager_request_subchild(m, c2), NULL);

   efl_del(m);
   elm_shutdown();
}
END_TEST

START_TEST(test_request_subchild_child_regular_regular)
{
   Efl_Ui_Focus_Manager *m;
   Efl_Ui_Focus_Object *root, *c1, *c2;

   char *args[] = { "exe" };
   elm_init(1, args);

   m = elm_focus_test_manager_new(&root);
   c1 = elm_focus_test_object_new("child1", 0, 0, 20, 20);
   c2 = elm_focus_test_object_new("child2", 0, 0, 20, 20);
   efl_ui_focus_manager_calc_register(m, c1, root, NULL);
   efl_ui_focus_manager_calc_register(m, c2, c1, NULL);

   ck_assert_ptr_eq(efl_ui_focus_manager_request_subchild(m, root), c1);
   ck_assert_ptr_eq(efl_ui_focus_manager_request_subchild(m, c1), c2);
   ck_assert_ptr_eq(efl_ui_focus_manager_request_subchild(m, c2), NULL);

   efl_del(m);
   elm_shutdown();
}
END_TEST

START_TEST(test_unregister_last_focused_no_history)
{
   Efl_Ui_Focus_Manager *m;
   Efl_Ui_Focus_Object *root, *c1, *c2, *c3;

   char *args[] = { "exe" };
   elm_init(1, args);

   m = elm_focus_test_manager_new(&root);
   c1 = elm_focus_test_object_new("child1", 0, 0, 20, 20);
   c2 = elm_focus_test_object_new("child2", 0, 0, 20, 20);
   c3 = elm_focus_test_object_new("child3", 0, 0, 20, 20);
   efl_ui_focus_manager_calc_register(m, c1, root, NULL);
   efl_ui_focus_manager_calc_register(m, c2, root, NULL);
   efl_ui_focus_manager_calc_register(m, c3, root, NULL);

   efl_ui_focus_manager_focus_set(m, c2);
   ck_assert_ptr_eq(efl_ui_focus_manager_focus_get(m), c2);

   efl_ui_focus_manager_calc_unregister(m, c2);
   ck_assert_ptr_eq(efl_ui_focus_manager_focus_get(m), c1);

   efl_del(m);
   elm_shutdown();
}
END_TEST

START_TEST(test_unregister_last_focused)
{
   Efl_Ui_Focus_Manager *m;
   Efl_Ui_Focus_Object *root, *c1, *c2, *c3;

   char *args[] = { "exe" };
   elm_init(1, args);

   m = elm_focus_test_manager_new(&root);
   c1 = elm_focus_test_object_new("child1", 0, 0, 20, 20);
   c2 = elm_focus_test_object_new("child2", 0, 0, 20, 20);
   c3 = elm_focus_test_object_new("child3", 0, 0, 20, 20);
   efl_ui_focus_manager_calc_register(m, c1, root, NULL);
   efl_ui_focus_manager_calc_register(m, c2, root, NULL);
   efl_ui_focus_manager_calc_register(m, c3, root, NULL);

   efl_ui_focus_manager_focus_set(m, c2);
   ck_assert_ptr_eq(efl_ui_focus_manager_focus_get(m), c2);
   efl_ui_focus_manager_focus_set(m, c3);
   ck_assert_ptr_eq(efl_ui_focus_manager_focus_get(m), c3);

   efl_ui_focus_manager_calc_unregister(m, c3);
   ck_assert_ptr_eq(efl_ui_focus_manager_focus_get(m), c2);

   efl_del(m);
   elm_shutdown();
}
END_TEST

START_TEST(test_unregister_last_focused_no_child)
{
   Efl_Ui_Focus_Manager *m;
   Efl_Ui_Focus_Object *root, *c1;

   char *args[] = { "exe" };
   elm_init(1, args);

   m = elm_focus_test_manager_new(&root);
   c1 = elm_focus_test_object_new("child1", 0, 0, 20, 20);
   efl_ui_focus_manager_calc_register(m, c1, root, NULL);

   efl_ui_focus_manager_focus_set(m, c1);
   ck_assert_ptr_eq(efl_ui_focus_manager_focus_get(m), c1);
   efl_ui_focus_manager_calc_unregister(m, c1);
   ck_assert_ptr_eq(efl_ui_focus_manager_focus_get(m), NULL);

   efl_del(m);
   elm_shutdown();
}
END_TEST

void elm_test_focus(TCase *tc)
{
    tcase_add_test(tc, focus_register_twice);
    tcase_add_test(tc, focus_unregister_twice);
    tcase_add_test(tc, pos_check);
    tcase_add_test(tc, pos_check2);
    tcase_add_test(tc, redirect);
    tcase_add_test(tc, border_check);
    tcase_add_test(tc, finalize_check);
    tcase_add_test(tc, logical_chain);
    tcase_add_test(tc, redirect_param);
    tcase_add_test(tc, invalid_args_check);
    tcase_add_test(tc, order_check);
    tcase_add_test(tc, logical_shift);
    tcase_add_test(tc, root_redirect_chain);
    tcase_add_test(tc, root_redirect_chain_unset);
    tcase_add_test(tc, first_touch_check);
    tcase_add_test(tc, test_request_subchild_empty);
    tcase_add_test(tc, test_request_subchild_one_element);
    tcase_add_test(tc, test_request_subchild_child_alongside);
    tcase_add_test(tc, test_request_subchild_child_logical_regular);
    tcase_add_test(tc, test_request_subchild_child_regular_regular);
    tcase_add_test(tc, test_unregister_last_focused_no_history);
    tcase_add_test(tc, test_unregister_last_focused);
    tcase_add_test(tc, test_unregister_last_focused_no_child);
}
