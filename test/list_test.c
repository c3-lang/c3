/* c3
 * Copyright 2022 Thomas de Grivel <thoxdg@gmail.com>
 */
#include <assert.h>
#include <stdlib.h>
#include <string.h>
#include "../libc3/list.h"
#include "../libc3/str.h"
#include "test.h"

#define LIST_TEST_INSPECT(test, expected)                              \
  do {                                                                 \
    s_list *list_test;                                                 \
    s_str str_result;                                                  \
    assert(test);                                                      \
    assert(expected);                                                  \
    test_context("list_inspect(" # test ") -> " # expected);           \
    list_test = list_1(test);                                          \
    TEST_EQ(list_inspect(list_test, &str_result), &str_result);        \
    TEST_STRNCMP(str_result.ptr.p, (expected), str_result.size);       \
    str_clean(&str_result);                                            \
    list_delete(list_test);                                            \
    test_context(NULL);                                                \
  } while (0)

void list_test_inspect ()
{
  LIST_TEST_INSPECT("[]", "[]");
  LIST_TEST_INSPECT("[[] | []]", "[[]]");
  LIST_TEST_INSPECT("[[], [] | []]", "[[], []]");
}

void list_test ()
{
  list_test_inspect();
}
