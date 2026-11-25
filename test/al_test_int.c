/*
 * test.c
 *
 * Test cases for the API.
 * Written with the help of a coding agent running locally
 */

#include "../arraylist.h"
#include <assert.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>

#define TEST(name)                                                             \
  printf("Running %s...\n", #name);                                            \
  fflush(stdout);
#define PASS() printf(" > PASSED\n")

al_define(int, int);

int int_to_string(int t, char *buf) { return snprintf(buf, 32, "%d", t); }

static void test_empty_array() {
  TEST("test_empty_array");

  int *array = al_init_int();

  assert(array != NULL);
  assert(al_length(array) == 0);
  assert(al_capacity(array) == AL_INIT_CAPACITY);

  assert(al_free(array));
  PASS();
}

static void test_new_array() {
  TEST("test_new_array");

  int *array = al_init_int();
  assert(al_length(array) == 0);
  assert(al_free(array));

  array = al_create_int(0);
  assert(al_length(array) == 0);
  assert(al_capacity(array) >= AL_INIT_CAPACITY);
  assert(al_free(array));

  array = al_create_int(AL_INIT_CAPACITY);
  assert(al_length(array) == 0);
  assert(al_capacity(array) >= AL_INIT_CAPACITY);
  assert(al_free(array));

  array = al_create_int(44);
  assert(al_length(array) == 0);
  assert(al_capacity(array) == 44);
  assert(al_free(array));

  PASS();
}

static void test_push_basic() {
  TEST("test_push_basic");

  int *array = al_init_int();

  al_push_int(&array, 1);
  assert(al_length(array) == 1);
  assert(array[0] == 1);

  al_push_int(&array, 2);
  assert(al_length(array) == 2);
  assert(array[1] == 2);

  al_push_int(&array, 3);
  assert(al_length(array) == 3);
  assert(array[2] == 3);

  assert(al_free(array));
  PASS();
}

static void test_push_capacity_growth() {
  TEST("test_push_capacity_growth");

  int *array = al_init_int();

  for (size_t i = 0; i < AL_INIT_CAPACITY; i++) {
    al_push_int(&array, i);
    assert(al_length(array) == i + 1);
    assert(array[i] == (int)i);
  }

  size_t original_capacity = al_capacity(array);
  for (size_t i = AL_INIT_CAPACITY; i < AL_INIT_CAPACITY + 10; i++) {
    al_push_int(&array, i);
    assert(al_length(array) == i + 1);
    assert(array[i] == (int)i);
    assert(al_capacity(array) >= original_capacity);
  }

  assert(al_free(array));
  PASS();
}

static void test_remove_by_value_first_occurrence() {
  TEST("test_remove_by_value_first_occurrence");

  int *array = al_init_int();

  al_push_int(&array, 1);
  al_push_int(&array, 2);
  al_push_int(&array, 1);
  al_push_int(&array, 3);
  al_push_int(&array, 1);

  assert(al_length(array) == 5);

  al_rm_int(array, 1, NULL);
  assert(al_length(array) == 4);
  assert(array[al_length(array)] == 0);

  assert(array[0] == 2);
  assert(array[1] == 1);
  assert(array[2] == 3);
  assert(array[3] == 1);

  PASS();
  assert(al_free(array));
}

static void test_remove_nonexistent_value() {
  TEST("test_remove_nonexistent_value");

  int *array = al_init_int();

  al_push_int(&array, 1);
  assert(al_length(array) == 1);

  assert(al_rm_int(array, 999, NULL) == AL_ERRTYPE(int));

  assert(al_length(array) == 1);
  assert(array[0] == 1);

  assert(al_free(array));
  PASS();
}

static void test_remove_more_occurrences() {
  TEST("test_remove_all_occurrences");

  int *array = al_init_int();

  al_push_int(&array, 5);
  al_push_int(&array, 3);
  al_push_int(&array, 5);
  al_push_int(&array, 7);
  al_push_int(&array, 5);

  assert(al_length(array) == 5);

  al_rm_int(array, 5, NULL);
  al_rm_int(array, 5, NULL);

  assert(al_length(array) == 3);
  assert(array[0] == 3);
  assert(array[1] == 7);
  assert(array[2] == 5);

  assert(al_free(array));
  PASS();
}

static void test_iremove_basic() {
  TEST("test_iremove_basic");

  int *array = al_init_int();

  al_push_int(&array, 1);
  al_push_int(&array, 2);
  al_push_int(&array, 3);
  al_push_int(&array, 4);
  al_push_int(&array, 5);

  assert(al_length(array) == 5);

  // Remove element at index 2 (value 3)
  al_rmat_int(array, 2);

  assert(al_length(array) == 4);
  assert(array[2] == 4); // Element shifted from index 3

  assert(al_free(array));
  PASS();
}

static void test_iremove_first_and_last() {
  TEST("test_iremove_first_and_last");

  int *array = al_init_int();

  al_push_int(&array, 10);
  al_push_int(&array, 20);
  al_push_int(&array, 30);
  al_push_int(&array, 40);
  al_push_int(&array, 50);

  assert(al_length(array) == 5);

  al_rmat_int(array, 0);
  assert(al_length(array) == 4);
  assert(array[0] == 20);

  al_rmat_int(array, 3);
  assert(al_length(array) == 3);
  assert(array[2] == 40);

  al_rmat_int(array, 1);
  assert(al_length(array) == 2);

  assert(al_free(array));
  PASS();
}

static void test_iremove_empty_array() {
  TEST("test_iremove_empty_array");

  int *array = al_init_int();
  assert(al_rm_int(array, 0, NULL) == AL_ERRTYPE(int));
  assert(al_rm_int(array, 10, NULL) == AL_ERRTYPE(int));
  assert(al_rm_int(array, -10, NULL) == AL_ERRTYPE(int));

  assert(al_free(array));
  PASS();
}

static void test_iremove_middle_element() {
  TEST("test_iremove_middle_element");

  int *array = al_init_int();

  al_push_int(&array, 100);
  al_push_int(&array, 200);
  al_push_int(&array, 300);
  al_push_int(&array, 400);
  al_push_int(&array, 500);

  assert(al_length(array) == 5);

  al_rmat_int(array, 2);

  assert(al_length(array) == 4);
  assert(array[1] == 200);
  assert(array[2] == 400); // Was at index 3

  assert(al_free(array));
  PASS();
}

static void test_push_remove_same_value() {
  TEST("test_push_remove_same_value");

  int *array = al_init_int();

  al_push_int(&array, 10);
  assert(al_length(array) == 1);
  assert(al_rm_int(array, 10, NULL));
  assert(al_length(array) == 0);

  al_push_int(&array, 10);
  assert(al_length(array) == 1);
  assert(al_rm_int(array, 10, NULL));
  assert(al_length(array) == 0);

  assert(al_free(array));
  PASS();
}

static void test_push_remove_interleaved() {
  TEST("test_push_remove_interleaved");

  int *array = al_init_int();

  al_push_int(&array, 1);
  al_push_int(&array, 2);
  al_push_int(&array, 3);

  assert(al_length(array) == 3);

  // Remove middle value
  al_rm_int(array, 2, NULL);
  assert(al_length(array) == 2);
  assert(array[0] == 1);
  assert(array[1] == 3);

  // Insert another value
  al_push_int(&array, 4);
  assert(al_length(array) == 3);
  assert(array[1] == 3);
  assert(array[2] == 4);

  assert(al_free(array));
  PASS();
}

static void test_capacity_boundary() {
  TEST("test_capacity_boundary");

  int *array = al_init_int();

  // Insert many elements to near capacity
  for (size_t i = 0; i < AL_INIT_CAPACITY - 1; i++) {
    al_push_int(&array, i);
  }

  assert(al_length(array) == AL_INIT_CAPACITY - 1);
  assert(al_capacity(array) >= AL_INIT_CAPACITY);

  // Insert one more to trigger growth
  al_push_int(&array, AL_INIT_CAPACITY - 1);

  assert(al_length(array) == AL_INIT_CAPACITY);
  assert(al_capacity(array) >= AL_INIT_CAPACITY);

  assert(al_free(array));
  PASS();
}

static void test_remove_at_boundary() {
  TEST("test_remove_at_boundary");

  int *array = al_init_int();

  for (size_t i = 1; i <= 5; i++) {
    al_push_int(&array, i);
  }
  assert(al_length(array) == 5);

  // Remove last element
  al_rm_int(array, 5, NULL);
  assert(al_length(array) == 4);

  // Remove new last element
  al_rm_int(array, 4, NULL);
  assert(al_length(array) == 3);

  assert(al_free(array));
  PASS();
}

static void test_iremove_out_of_bounds() {
  TEST("test_iremove_out_of_bounds");

  int *array = al_init_int();

  for (size_t i = 0; i < 10; i++) {
    al_push_int(&array, i);
  }
  assert(al_length(array) == 10);

  assert(al_rmat_int(array, -1) == AL_ERRTYPE(int));
  assert(al_length(array) == 10); // Should still be 10

  assert(al_rmat_int(array, 99) == AL_ERRTYPE(int));
  assert(al_length(array) == 10); // Should still be 10

  assert(al_free(array));
  PASS();
}

static void test_remove_first_occurrence_edge_case() {
  TEST("test_remove_first_occurrence_edge_case");

  int *array = al_init_int();

  // All elements are the same value
  for (size_t i = 0; i < 5; i++) {
    al_push_int(&array, 42);
  }

  assert(al_length(array) == 5);

  // Remove should only remove first occurrence
  al_rm_int(array, 42, NULL);

  assert(al_length(array) == 4);
  assert(array[0] == 42); // Still has 42 at index 0 (was the second original)

  // Remove again - should still be first remaining occurrence
  al_rm_int(array, 42, NULL);
  assert(al_length(array) == 3);

  assert(al_free(array));
  PASS();
}

static void test_mixed_operations() {
  TEST("test_mixed_operations");

  int *array = al_init_int();

  al_push_int(&array, 10);
  al_push_int(&array, 20);
  al_push_int(&array, 30);
  al_push_int(&array, 40);

  assert(al_length(array) == 4);

  // Remove middle value
  al_rm_int(array, 30, NULL);
  assert(al_length(array) == 3);

  // Insert new value
  al_push_int(&array, 99);
  assert(al_length(array) == 4);

  // Remove first value
  al_rmat_int(array, 0);
  assert(al_length(array) == 3);

  // Remove last value
  al_rmat_int(array, al_length(array) - 1);
  assert(al_length(array) == 2);

  assert(al_free(array));
  PASS();
}

static void test_empty_value_removal() {
  TEST("test_empty_value_removal");

  int *array = al_init_int();

  al_push_int(&array, 0);
  al_push_int(&array, 1);
  al_push_int(&array, 2);

  assert(al_length(array) == 3);

  // Remove 0 (valid integer value, not empty array)
  al_rm_int(array, 0, NULL);
  assert(al_length(array) == 2);
  assert(array[0] == 1);
  assert(array[1] == 2);

  assert(al_free(array));
  PASS();
}

static void test_negative_value_removal() {
  TEST("test_negative_value_removal");

  int *array = al_init_int();

  al_push_int(&array, -100);
  al_push_int(&array, 0);
  al_push_int(&array, 100);

  assert(al_length(array) == 3);

  // Remove negative value
  al_rm_int(array, -100, NULL);
  assert(al_length(array) == 2);
  assert(array[0] == 0);
  assert(array[1] == 100);

  assert(al_free(array));
  PASS();
}

static void test_large_number_of_items() {
  TEST("test_large_number_of_items");

  int *array = al_init_int();

  for (size_t i = 0; i < 1000; i++) {
    al_push_int(&array, i);
  }

  assert(al_length(array) == 1000);

  al_rm_int(array, 500, NULL);
  assert(al_length(array) == 999);
  assert(array[500] == 501);

  al_rmat_int(array, 100);
  assert(al_length(array) == 998);

  assert(al_free(array));
  PASS();
}

static void test_remove_then_push_same() {
  TEST("test_remove_then_push_same");

  int *array = al_init_int();

  al_push_int(&array, 5);
  assert(al_length(array) == 1);

  al_rm_int(array, 5, NULL);
  assert(al_length(array) == 0);

  // Insert same value again
  al_push_int(&array, 5);
  assert(al_length(array) == 1);
  assert(array[0] == 5);

  // Remove again - should be empty
  al_rm_int(array, 5, NULL);
  assert(al_length(array) == 0);

  assert(al_free(array));
  PASS();
}

static void test_print_empty_array() {
  TEST("test_print_empty_array");

  int *array = al_init_int();

#ifdef AL_TEST_LOG
  printf("Empty array: ");
  al_print_int(array);
#endif
  assert(al_length(array) == 0);

  assert(al_free(array));
  PASS();
}

static void test_print_single_element() {
  TEST("test_print_single_element");

  int *array = al_init_int();
  al_push_int(&array, 42);

#ifdef AL_TEST_LOG
  printf("Single element array: ");
  al_print_int(array);
#endif

  assert(al_length(array) == 1);

  assert(al_free(array));
  PASS();
}

static void test_all_functions_used() {
  TEST("test_all_functions_used");

  int *array = al_init_int();
  assert(al_length(array) == 0);
  assert(al_capacity(array) == AL_INIT_CAPACITY);

  int *array2 = al_create_int(0);
  assert(al_length(array2) == 0);
  assert(al_capacity(array2) == AL_INIT_CAPACITY);

  al_push_int(&array, 99);
  assert(al_length(array) == 1);
  al_rm_int(array, 99, NULL);
  assert(al_length(array) == 0);

  assert(al_push_int(&array, 1));
  assert(al_push_int(&array, 2));
  assert(al_push_int(&array, 3));
  assert(al_length(array) == 3);
  assert(al_rmat_int(array, 1));
  assert(al_length(array) == 2);

  assert(al_free(array));
  assert(al_free(array2));
  PASS();
}

static void test_new_exceed_maximum_capacity() {
  TEST("test_new_exceed_maximum_capacity");

  int *array = al_create_int(AL_MAXCAP(int));
  assert(array == NULL);

  PASS();
}

static void test_create_large_number() {
  TEST("test_create_large_number");

  int *array = al_create_int(AL_MAXCAP(int) - 1);
  assert(!array);
  assert(!al_free(array));

  PASS();
}

static void test_nullptr_guard() {
  TEST("test_nullptr_guard");

  assert(al_length(NULL) == AL_ERRSIZE);
  assert(al_capacity(NULL) == AL_ERRSIZE);

  PASS();
}
static void test_insert_start() {
  TEST("test_insert_start");

  int *array = al_init_int();

  assert(al_push_int(&array, 100));
  assert(al_push_int(&array, 200));
  assert(al_push_int(&array, 300));
  assert(al_push_int(&array, 400));

  assert(al_length(array) == 4);

  assert(al_insert_int(&array, 0, 1000));
  assert(al_length(array) == 5);
  assert(array[0] == 1000);
  assert(array[1] == 100);

  assert(al_insert_int(&array, 0, 2000));
  assert(al_length(array) == 6);
  assert(array[0] == 2000);
  assert(array[1] == 1000);
  assert(array[2] == 100);

  assert(al_insert_int(&array, 0, 3000));
  assert(al_insert_int(&array, 0, 4000));
  assert(al_insert_int(&array, 0, 5000));
  assert(array[0] == 5000);
  assert(array[1] == 4000);
  assert(array[2] == 3000);
  assert(al_length(array) == 9);

  assert(al_free(array));
  PASS();
}

static void test_insert_end() {
  TEST("test_insert_end");

  int *array = al_init_int();

  assert(al_push_int(&array, 100));
  assert(al_push_int(&array, 200));
  assert(al_push_int(&array, 300));
  assert(al_push_int(&array, 400));

  assert(al_length(array) == 4);

  assert(al_insert_int(&array, al_length(array), 1000));
  assert(al_length(array) == 5);
  assert(array[0] == 100);
  assert(array[1] == 200);
  assert(array[2] == 300);
  assert(array[3] == 400);
  assert(array[4] == 1000);

  assert(al_insert_int(&array, al_length(array), 2000));
  assert(al_length(array) == 6);
  assert(array[4] == 1000);
  assert(array[5] == 2000);

  assert(al_insert_int(&array, al_length(array), 3000));
  assert(al_insert_int(&array, al_length(array), 4000));
  assert(al_insert_int(&array, al_length(array), 5000));
  assert(array[6] == 3000);
  assert(array[7] == 4000);
  assert(array[8] == 5000);
  assert(al_length(array) == 9);

  assert(al_free(array));
  PASS();
}

static void test_insert_middle() {
  TEST("test_insert_middle");

  int *array = al_init_int();

  assert(al_push_int(&array, 100));
  assert(al_push_int(&array, 200));
  assert(al_push_int(&array, 300));
  assert(al_push_int(&array, 400));

  assert(al_length(array) == 4);

  assert(al_insert_int(&array, 2, 1000));
  assert(al_length(array) == 5);
  assert(array[0] == 100);
  assert(array[1] == 200);
  assert(array[2] == 1000);
  assert(array[3] == 300);
  assert(array[4] == 400);

  assert(al_insert_int(&array, 3, 2000));
  assert(al_length(array) == 6);
  assert(array[0] == 100);
  assert(array[1] == 200);
  assert(array[2] == 1000);
  assert(array[3] == 2000);
  assert(array[4] == 300);
  assert(array[5] == 400);

  assert(al_insert_int(&array, al_length(array), 3000));
  assert(al_insert_int(&array, al_length(array), 4000));
  assert(al_insert_int(&array, al_length(array), 5000));
  assert(al_length(array) == 9);
  assert(array[6] == 3000);
  assert(array[7] == 4000);
  assert(array[8] == 5000);

  assert(al_insert_int(&array, 0, 6000));
  assert(al_insert_int(&array, 1, 7000));
  assert(al_length(array) == 11);
  assert(array[0] == 6000);
  assert(array[1] == 7000);

  assert(al_free(array));
  PASS();
}

static void test_insert_fail() {
  TEST("test_insert_fail");

  int *array = al_init_int();

  assert(al_push_int(&array, 100));
  assert(al_push_int(&array, 200));
  assert(al_push_int(&array, 300));
  assert(al_push_int(&array, 400));

  assert(al_length(array) == 4);

  assert(al_insert_int(&array, 999, 0) == AL_FAIL);
  assert(al_length(array) == 4);
  assert(array[0] == 100);
  assert(array[1] == 200);
  assert(array[2] == 300);
  assert(array[3] == 400);

  assert(al_insert_int(NULL, 0, 0) == AL_FAIL);
  assert(al_length(array) == 4);

  int *nullarray = NULL;
  assert(al_insert_int(&nullarray, 0, 0) == AL_FAIL);

  assert(al_free(array));
  PASS();
}

static void test_remove_all() {
  TEST("test_remove_all");

  int *array = al_init_int();

  assert(al_push_int(&array, 100));
  assert(al_push_int(&array, 88888888));
  assert(al_push_int(&array, 200));
  assert(al_push_int(&array, 88888888));
  assert(al_push_int(&array, 300));
  assert(al_push_int(&array, 88888888));
  assert(al_push_int(&array, 400));
  assert(al_push_int(&array, 88888888));

  assert(al_length(array) == 8);
  assert(al_rmall_int(array, 88888888, NULL) == 4);
  assert(al_length(array) == 4);

  assert(al_rmall_int(array, 100, NULL) == 1);
  assert(al_length(array) == 3);
  assert(al_rmall_int(array, 200, NULL) == 1);
  assert(al_length(array) == 2);
  assert(al_rmall_int(array, 300, NULL) == 1);
  assert(al_length(array) == 1);
  assert(al_rmall_int(array, 400, NULL) == 1);
  assert(al_length(array) == 0);

  assert(al_push_int(&array, 666666));
  assert(al_push_int(&array, 88888888));
  assert(al_push_int(&array, 666666));
  assert(al_push_int(&array, 88888888));
  assert(al_push_int(&array, 666666));
  assert(al_push_int(&array, 88888888));
  assert(al_push_int(&array, 666666));
  assert(al_push_int(&array, 88888888));
  assert(al_length(array) == 8);

  assert(al_rmall_int(array, 0, NULL) == 0);
  assert(al_length(array) == 8);

  assert(al_rmall_int(array, 666666, NULL) == 4);
  assert(al_length(array) == 4);
  assert(al_rmall_int(array, 88888888, NULL) == 4);
  assert(al_length(array) == 0);

  assert(al_free(array));
  PASS();
}

static void test_remove_all_same() {
  TEST("test_remove_all_same");

  int *array = al_init_int();
  size_t orig_len = 14;
  assert(al_push_int(&array, 8));
  assert(al_push_int(&array, 8));
  assert(al_push_int(&array, 8));
  assert(al_push_int(&array, 8));
  assert(al_push_int(&array, 8));
  assert(al_push_int(&array, 8));
  assert(al_push_int(&array, 8));
  assert(al_push_int(&array, 8));
  assert(al_push_int(&array, 8));
  assert(al_push_int(&array, 8));
  assert(al_push_int(&array, 8));
  assert(al_push_int(&array, 8));
  assert(al_push_int(&array, 8));
  assert(al_push_int(&array, 8));
  assert(al_length(array) == orig_len);

  al_rmall_int(array, 8, NULL);
  assert(al_length(array) == 0);

  for (size_t i = 0; i < orig_len; i++) {
    assert(array[i] == 0);
  }

#ifdef AL_TEST_LOG
  puts("");
  for (size_t i = al_length(array); i < al_capacity(array); i++) {
    printf("array[%zu]: %d\n", i, array[i]);
  }
#endif

  assert(al_free(array));
  PASS();
}

static void test_remove_all_alternating() {
  TEST("test_remove_all_alternating");

  // {8, 8, 1, 8, 8, 8, 2, 8}
  int *array = al_init_int();
  assert(al_push_int(&array, 8));
  assert(al_push_int(&array, 8));
  assert(al_push_int(&array, 1));
  assert(al_push_int(&array, 8));
  assert(al_push_int(&array, 8));
  assert(al_push_int(&array, 2));
  assert(al_push_int(&array, 8));
  assert(al_length(array) == 7);

  al_rmall_int(array, 8, NULL);
  printf("len: %zu", al_length(array));
  assert(al_length(array) == 2);

#ifdef AL_TEST_LOG
  puts("");
  for (size_t i = 0; i < al_length(array); i++) {
    printf("array[%zu]: %d\n", i, array[i]);
  }
#endif

  assert(al_free(array));
  PASS();
}

static void test_grow_basic() {
  TEST("test_grow_generic");

  int *array = al_init_int();
  for (size_t i = 0; i < AL_INIT_CAPACITY; i++) {
    al_push_int(&array, 90);
  }
  assert(al_length(array) == AL_INIT_CAPACITY);
  assert(al_capacity(array) == AL_INIT_CAPACITY);

  al_push_int(&array, 91);
  assert(al_length(array) == AL_INIT_CAPACITY + 1);
  assert(al_capacity(array) == AL_GROWTH_FACTOR * AL_INIT_CAPACITY);

  size_t j;
  for (j = 0; j < AL_INIT_CAPACITY; j++) {
    assert(array[j] == 90);
  }
  assert(array[j] == 91);

// #define AL_TEST_LOG()
#ifdef AL_TEST_LOG
  puts("");
  for (size_t i = 0; i < al_length(array); i++) {
    printf("array[%zu]: %d\n", i, array[i]);
  }
#endif

  assert(al_free(array));
  PASS();
}

static void test_grow_manual() {
  TEST("test_grow_manual");

  int *array = al_init_int();
  assert(al_length(array) == 0);
  assert(al_capacity(array) == AL_INIT_CAPACITY);

  assert(al_grow_int(&array));
  assert(al_length(array) == 0);
  assert(al_capacity(array) == AL_GROWTH_FACTOR * AL_INIT_CAPACITY);

  assert(al_grow_int(&array));
  assert(al_length(array) == 0);
  assert(al_capacity(array) == 2 * AL_GROWTH_FACTOR * AL_INIT_CAPACITY);

  assert(al_grow_int(&array));
  assert(al_length(array) == 0);
  assert(al_capacity(array) == 4 * AL_GROWTH_FACTOR * AL_INIT_CAPACITY);

  assert(al_grow_int(NULL) == AL_FAIL);

  assert(al_free(array));
  PASS();
}

static void test_resize_basic() {
  TEST("test_resize_basic");

  int *array = al_init_int();
  assert(al_length(array) == 0);
  assert(al_capacity(array) == AL_INIT_CAPACITY);

  assert(al_resize_int(&array, 10));
  assert(al_length(array) == 0);
  assert(al_capacity(array) == 10);

  assert(al_resize_int(&array, 100));
  assert(al_length(array) == 0);
  assert(al_capacity(array) == 100);

  for (size_t i = 0; i < 100; i++) {
    assert(al_push_int(&array, 7));
  }

  assert(al_length(array) == 100);
  assert(al_capacity(array) == 100);

  assert(al_resize_int(&array, 50));
  assert(al_length(array) == 50);
  assert(al_capacity(array) == 50);

  assert(al_resize_int(&array, 50));
  assert(al_length(array) == 50);
  assert(al_capacity(array) == 50);

  assert(al_free(array));
  PASS();
}

static void test_resize_edge_cases() {
  TEST("test_resize_edge_cases");

  int *arr = al_init_int();
  assert(al_length(arr) == 0);
  assert(al_capacity(arr) == AL_INIT_CAPACITY);

  assert(al_resize_int(&arr, -1) == AL_FAIL);
  assert(al_length(arr) == 0);
  assert(al_capacity(arr) == AL_INIT_CAPACITY);

  assert(al_resize_int(&arr, AL_MAXCAP(int)) == AL_FAIL);
  assert(al_length(arr) == 0);
  assert(al_capacity(arr) == AL_INIT_CAPACITY);

  assert(al_resize_int(NULL, AL_MAXCAP(int)) == AL_FAIL);
  assert(al_length(arr) == 0);
  assert(al_capacity(arr) == AL_INIT_CAPACITY);

  int *nullarr = NULL;
  assert(al_resize_int(&nullarr, AL_MAXCAP(int)) == AL_FAIL);
  assert(al_length(arr) == 0);
  assert(al_capacity(arr) == AL_INIT_CAPACITY);

  assert(al_resize_int(&arr, 0));
  assert(al_length(arr) == 0);
  assert(al_capacity(arr) == AL_INIT_CAPACITY);

  assert(al_resize_int(&arr, 1));
  assert(al_length(arr) == 0);
  assert(al_capacity(arr) == 1);

  assert(al_push_int(&arr, 100));
  assert(al_push_int(&arr, 101));
  assert(al_push_int(&arr, 102));
  assert(al_length(arr) == 3);

  assert(al_capacity(arr) == 1 * 2 * 2);

  assert(al_resize_int(&arr, 0));
  assert(al_length(arr) == 3);
  assert(al_capacity(arr) == AL_INIT_CAPACITY);

  assert(al_push_int(&arr, 100));
  assert(al_push_int(&arr, 101));
  assert(al_push_int(&arr, 102));
  assert(al_length(arr) == 6);

  assert(al_free(arr));
  PASS();
}

static void test_clear() {
  TEST("test_clear");

  int *arr = al_init_int();
  assert(al_length(arr) == 0);
  assert(al_capacity(arr) == AL_INIT_CAPACITY);

  for (size_t i = 0; i < 100; i++) {
    assert(al_push_int(&arr, 100));
  }
  assert(al_length(arr) == 100);
  assert(al_capacity(arr) == AL_INIT_CAPACITY * 2 * 2);

  assert(al_clear_int(arr));
  assert(al_length(arr) == 0);
  assert(al_capacity(arr) == AL_INIT_CAPACITY * 2 * 2);

  for (size_t i = 0; i < al_capacity(arr); i++) {
    assert(arr[i] == 0);
  }

  assert(al_free(arr));
  PASS();
}

static int reverse_cmp(int a, int b) { return (a != b) ? 0 : 1; }

static void test_contains() {
  TEST("test_contains");

  int *arr = al_init_int();
  assert(al_length(arr) == 0);
  assert(al_capacity(arr) == AL_INIT_CAPACITY);

  assert(al_push_int(&arr, 100));
  assert(al_push_int(&arr, 101));
  assert(al_push_int(&arr, 102));
  assert(al_push_int(&arr, 103));

  assert(al_length(arr) == 4);
  assert(al_capacity(arr) == AL_INIT_CAPACITY);

  assert(al_contains_int(arr, 100, NULL) == 0);
  assert(al_contains_int(arr, 101, NULL) == 1);
  assert(al_contains_int(arr, 102, NULL) == 2);
  assert(al_contains_int(arr, 103, NULL) == 3);

  assert(al_contains_int(arr, 0, NULL) == AL_ERRSIZE);
  assert(al_contains_int(arr, 1, NULL) == AL_ERRSIZE);
  assert(al_contains_int(arr, 2, NULL) == AL_ERRSIZE);

  assert(al_contains_int(arr, 0, reverse_cmp) == 0);
  assert(al_contains_int(arr, 1, reverse_cmp) == 0);
  assert(al_contains_int(arr, 2, reverse_cmp) == 0);

  // skips `100` and returns on the first different item
  assert(al_contains_int(arr, 100, reverse_cmp) == 1);

  assert(al_free(arr));
  PASS();
}

static void test_copy() {
  TEST("test_copy");

  int *arr = al_init_int();
  assert(al_length(arr) == 0);
  assert(al_capacity(arr) == AL_INIT_CAPACITY);

  assert(al_push_int(&arr, 100));
  assert(al_push_int(&arr, 101));
  assert(al_push_int(&arr, 102));
  assert(al_push_int(&arr, 103));

  assert(arr[0] == 100);
  assert(arr[1] == 101);
  assert(arr[2] == 102);
  assert(arr[3] == 103);

  assert(al_length(arr) == 4);
  assert(al_capacity(arr) == AL_INIT_CAPACITY);

  int *arr1 = al_copy_int(arr);

  assert(al_length(arr) == 4);
  assert(al_capacity(arr) == AL_INIT_CAPACITY);

  int *empty = al_init_int(), *emptycopy = al_copy_int(empty);
  assert(al_length(emptycopy) == 0);
  assert(al_capacity(emptycopy) == AL_INIT_CAPACITY);

  assert(arr1[0] == 100);
  assert(arr1[1] == 101);
  assert(arr1[2] == 102);
  assert(arr1[3] == 103);

  assert(al_free(arr));
  assert(al_free(arr1));
  assert(al_free(empty));
  assert(al_free(emptycopy));
  PASS();
}

static void test_swap() {
  TEST("test_swap");

  int *arr = al_init_int();
  assert(al_length(arr) == 0);
  assert(al_capacity(arr) == AL_INIT_CAPACITY);

  assert(al_push_int(&arr, 100));
  assert(al_push_int(&arr, 101));
  assert(al_push_int(&arr, 102));
  assert(al_push_int(&arr, 103));

  assert(arr[0] == 100);
  assert(arr[1] == 101);
  assert(arr[2] == 102);
  assert(arr[3] == 103);

  assert(al_length(arr) == 4);
  assert(al_capacity(arr) == AL_INIT_CAPACITY);

  assert(al_swap_int(arr, 3, 0));

  assert(arr[0] == 103);
  assert(arr[1] == 101);
  assert(arr[2] == 102);
  assert(arr[3] == 100);
  assert(al_length(arr) == 4);
  assert(al_capacity(arr) == AL_INIT_CAPACITY);

  assert(al_swap_int(arr, 0, 3));
  assert(arr[0] == 100);
  assert(arr[1] == 101);
  assert(arr[2] == 102);
  assert(arr[3] == 103);
  assert(al_length(arr) == 4);
  assert(al_capacity(arr) == AL_INIT_CAPACITY);

  assert(al_swap_int(arr, 1, 2));
  assert(arr[0] == 100);
  assert(arr[1] == 102);
  assert(arr[2] == 101);
  assert(arr[3] == 103);
  assert(al_length(arr) == 4);
  assert(al_capacity(arr) == AL_INIT_CAPACITY);

  assert(al_swap_int(arr, 2, 0));
  assert(arr[0] == 101);
  assert(arr[1] == 102);
  assert(arr[2] == 100);
  assert(arr[3] == 103);
  assert(al_length(arr) == 4);
  assert(al_capacity(arr) == AL_INIT_CAPACITY);

  assert(al_swap_int(arr, al_length(arr), 103) == AL_FAIL);
  assert(al_swap_int(arr, 0, al_length(arr)) == AL_FAIL);

  assert(al_free(arr));
  PASS();
}

int main() {
  puts("=== Running ArrayList Tests ===\n");

  test_empty_array();
  test_new_array();
  test_push_basic();
  test_push_capacity_growth();
  test_remove_by_value_first_occurrence();
  test_remove_nonexistent_value();
  test_remove_more_occurrences();
  test_iremove_basic();
  test_iremove_first_and_last();
  test_iremove_empty_array();
  test_iremove_middle_element();
  test_push_remove_same_value();
  test_push_remove_interleaved();
  test_capacity_boundary();
  test_remove_at_boundary();
  test_iremove_out_of_bounds();
  test_remove_first_occurrence_edge_case();
  test_mixed_operations();
  test_empty_value_removal();
  test_negative_value_removal();
  test_large_number_of_items();
  test_remove_then_push_same();
  test_print_empty_array();
  test_print_single_element();
  test_all_functions_used();
  test_new_exceed_maximum_capacity();
  test_create_large_number();
  test_nullptr_guard();
  test_insert_start();
  test_insert_end();
  test_insert_fail();
  test_insert_middle();
  test_remove_all();
  test_remove_all_same();
  test_remove_all_alternating();
  test_grow_basic();
  test_grow_manual();
  test_resize_basic();
  test_resize_edge_cases();
  test_clear();
  test_contains();
  test_copy();
  test_swap();

  puts("\n=== All Tests Completed ===");

  return 0;
}
