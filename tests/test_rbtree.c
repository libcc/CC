#include <check.h>
#include <stdlib.h>
#include "../src/rbtree.h"

START_TEST(test_rbtree_insert) {
    _cc_rbtree_t tree = {0};
    _cc_rbtree_iterator_t node = {0};

    // Test inserting a single node
    _cc_rbtree_insert_color(&tree, &node);
    ck_assert_ptr_eq(tree.rb_node, &node);
    ck_assert_int_eq(_rb_is_black(tree.rb_node), 1);

    // Test inserting multiple nodes
    _cc_rbtree_iterator_t node2 = {0};
    _cc_rbtree_insert_color(&tree, &node2);
    ck_assert_ptr_ne(tree.rb_node, &node2);
}
END_TEST

START_TEST(test_rbtree_erase) {
    _cc_rbtree_t tree = {0};
    _cc_rbtree_iterator_t node = {0};
    _cc_rbtree_iterator_t node2 = {0};

    _cc_rbtree_insert_color(&tree, &node);
    _cc_rbtree_insert_color(&tree, &node2);

    // Test erasing a node
    _cc_rbtree_erase(&tree, &node);
    ck_assert_ptr_eq(tree.rb_node, &node2);
}
END_TEST

START_TEST(test_rbtree_first_last) {
    _cc_rbtree_t tree = {0};
    _cc_rbtree_iterator_t node = {0};
    _cc_rbtree_iterator_t node2 = {0};

    _cc_rbtree_insert_color(&tree, &node);
    _cc_rbtree_insert_color(&tree, &node2);

    // Test first node
    ck_assert_ptr_eq(_cc_rbtree_first(&tree), &node);

    // Test last node
    ck_assert_ptr_eq(_cc_rbtree_last(&tree), &node2);
}
END_TEST

START_TEST(test_rbtree_next_prev) {
    _cc_rbtree_t tree = {0};
    _cc_rbtree_iterator_t node = {0};
    _cc_rbtree_iterator_t node2 = {0};

    _cc_rbtree_insert_color(&tree, &node);
    _cc_rbtree_insert_color(&tree, &node2);

    // Test next node
    ck_assert_ptr_eq(_cc_rbtree_next(&node), &node2);

    // Test previous node
    ck_assert_ptr_eq(_cc_rbtree_prev(&node2), &node);
}
END_TEST

START_TEST(test_rbtree_replace_node) {
    _cc_rbtree_t tree = {0};
    _cc_rbtree_iterator_t node = {0};
    _cc_rbtree_iterator_t node2 = {0};
    _cc_rbtree_iterator_t new_node = {0};

    _cc_rbtree_insert_color(&tree, &node);
    _cc_rbtree_insert_color(&tree, &node2);

    // Test replacing a node
    _cc_rbtree_replace_node(&tree, &node, &new_node);
    ck_assert_ptr_eq(tree.rb_node, &new_node);
}
END_TEST

Suite *rbtree_suite(void) {
    Suite *s;
    TCase *tc_core;

    s = suite_create("Red-Black Tree");
    tc_core = tcase_create("Core");

    tcase_add_test(tc_core, test_rbtree_insert);
    tcase_add_test(tc_core, test_rbtree_erase);
    tcase_add_test(tc_core, test_rbtree_first_last);
    tcase_add_test(tc_core, test_rbtree_next_prev);
    tcase_add_test(tc_core, test_rbtree_replace_node);

    suite_add_tcase(s, tc_core);
    return s;
}

int main(void) {
    int number_failed;
    Suite *s;
    SRunner *sr;

    s = rbtree_suite();
    sr = srunner_create(s);

    srunner_run_all(sr, CK_NORMAL);
    number_failed = srunner_ntests_failed(sr);
    srunner_free(sr);
    return (number_failed == 0) ? EXIT_SUCCESS : EXIT_FAILURE;
}