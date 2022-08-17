/* c3
 * Copyright 2022 Thomas de Grivel <thoxdg@gmail.com>
 */
#include "../libc3/types.h"
#include "test.h"

void types_test () {
  TEST_ASSERT(sizeof(s8)  == 1);
  TEST_ASSERT(sizeof(s16) == 2);
  TEST_ASSERT(sizeof(s32) == 4);
  TEST_ASSERT(sizeof(s64) == 8);
  TEST_ASSERT(sizeof(u8)  == 1);
  TEST_ASSERT(sizeof(u16) == 2);
  TEST_ASSERT(sizeof(u32) == 4);
  TEST_ASSERT(sizeof(u64) == 8);
}
