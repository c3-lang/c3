/* c3
 * Copyright 2022 Thomas de Grivel <thoxdg@gmail.com>
 */
#include "../libc3/types.h"
#include "test.h"

void bool_test ();
void bool_test_compat ();

void bool_test ()
{
  bool_test_compat();
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
