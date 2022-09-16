/* c3
 * Copyright 2022 Thomas de Grivel <thoxdg@gmail.com>
 */
#include <assert.h>
#include <stdlib.h>
#include <string.h>
#include "../libc3/tuple.h"
#include "../libc3/str.h"
#include "test.h"

#define TUPLE_TEST_INIT_1(test)                                        \
  do {                                                                 \
    s_tuple tuple_test;                                                \
    test_context("tuple_init_1(" # test ")");                          \
    TEST_EQ(tuple_init_1(&tuple_test, (test)), &tuple_test);           \
    tuple_clean(&tuple_test);                                          \
    test_context(NULL);                                                \
  } while (0)

#define TUPLE_TEST_INSPECT(test, expected)                             \
  do {                                                                 \
    s_tuple tuple_test;                                                \
    s_str str_result;                                                  \
    test_context("tuple_inspect(" # test ") -> " # expected);          \
    tuple_init_1(&tuple_test, (test));                                 \
    TEST_EQ(tuple_inspect(&tuple_test, &str_result), &str_result);     \
    TEST_STRNCMP(str_result.ptr.p, (expected), str_result.size);       \
    str_clean(&str_result);                                            \
    tuple_clean(&tuple_test);                                          \
    test_context(NULL);                                                \
  } while (0)

void tuple_test_init_1 ()
{
  TUPLE_TEST_INIT_1("{a, b}");
  TUPLE_TEST_INIT_1("{a, b, c}");
  TUPLE_TEST_INIT_1("{a, b, c, d}");
  TUPLE_TEST_INIT_1("{{a, b}, {c, d}}");
  TUPLE_TEST_INIT_1("{{a, b}, {c, d}, {e, f}}");
  TUPLE_TEST_INIT_1("{{a, b}, {c, d}, {e, f}, {g, h}}");
  TUPLE_TEST_INIT_1("{{a, b}, {c, d}, {e, f}, {g, h}, {i, j}}");
}

void tuple_test_inspect ()
{
  TUPLE_TEST_INSPECT("{a, b}", "{a, b}");
  TUPLE_TEST_INSPECT("{{a, b}, {c, d}}", "{{a, b}, {c, d}}");
  TUPLE_TEST_INSPECT("{{a, b}, {c, d}, {e, f}}",
                     "{{a, b}, {c, d}, {e, f}}");
}

void tuple_test ()
{
  tuple_test_init_1();
  tuple_test_inspect();
}
