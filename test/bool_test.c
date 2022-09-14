/* c3
 * Copyright 2022 Thomas de Grivel <thoxdg@gmail.com>
 */
#include <string.h>
#include "../libc3/bool.h"
#include "test.h"

#define BOOL_TEST_INSPECT(test, expected)                              \
  do {                                                                 \
    s_str str;                                                         \
    test_context("bool_inspect(" # test ") -> " # expected);           \
    TEST_EQ(bool_inspect(test, &str), &str);                           \
    TEST_EQ(str.size, strlen(expected));                               \
    if (g_test_last_ok)                                                \
      TEST_STRNCMP(str.ptr.p, (expected), str.size);                   \
  } while (0)

void bool_test ();
void bool_test_compat ();
void bool_test_inspect ();

void bool_test ()
{
  bool_test_compat();
  bool_test_inspect();
}

void bool_test_compat ()
{
  TEST_EQ(true, 1);
  TEST_EQ(false, 0);
  TEST_ASSERT(true);
  TEST_ASSERT(! false);
  TEST_ASSERT((bool) true);
  TEST_ASSERT(! (bool) false);
}

void bool_test_inspect ()
{
  BOOL_TEST_INSPECT(true, "true");
  BOOL_TEST_INSPECT(false, "false");
}
