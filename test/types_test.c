/* c3
 * Copyright 2022 Thomas de Grivel <thoxdg@gmail.com>
 */
#include "../libc3/types.h"
#include "test.h"

void types_test ();
void types_test_sizeof ();

void types_test ()
{
  types_test_sizeof();
}

void types_test_sizeof ()
{
  TEST_EQ(sizeof(s8), 1);
  TEST_EQ(sizeof(s16), 2);
  TEST_EQ(sizeof(s32), 4);
  TEST_ASSERT(sizeof(sw) == 4 || sizeof(sw) == 8);
  TEST_EQ(sizeof(s64), 8);
  TEST_EQ(sizeof(u8), 1);
  TEST_EQ(sizeof(u16), 2);
  TEST_EQ(sizeof(u32), 4);
  TEST_ASSERT(sizeof(uw) == 4 || sizeof(uw) == 8);
  TEST_EQ(sizeof(u64), 8);
  TEST_EQ(sizeof(bool), sizeof(u8));
  TEST_EQ(sizeof(u_ptr), sizeof(void *));
}
