/* c3
 * Copyright 2022 Thomas de Grivel <thoxdg@gmail.com>
 */
#include "../libc3/types.h"
#include "test.h"

void types_test () {
  u_ptr ptr;
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
  TEST_EQ(sizeof(bool), sizeof(uw));
  TEST_ASSERT(true);
  TEST_ASSERT(! false);
  TEST_ASSERT((bool) true);
  TEST_ASSERT(! (bool) false);
  TEST_EQ(sizeof(u_ptr), 8);
  TEST_EQ(sizeof(ptr.p), sizeof(void *));
  TEST_EQ(sizeof(ptr.u64), 8);
}
