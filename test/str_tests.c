/* c3
 * Copyright 2022 Thomas de Grivel <thoxdg@gmail.com>
 */
#include <stdlib.h>
#include <string.h>
#include "../libc3/str.h"
#include "test.h"

#define TEST_STR_CLEAN(stra)                    \
  do {                                          \
    str_clean(&stra);                           \
    TEST_ASSERT(stra.bytes == 0);               \
    TEST_ASSERT(stra.ptr.p == NULL);             \
    TEST_ASSERT(stra.ptr.u64 == 0);              \
  } while(0)

#define TEST_STR_DELETE(str) \
  do {                       \
    str_delete(str);         \
    test_ok();               \
  } while(0)

void str_test ()
{
  size_t len;
  char *m;
  s_str *str;
  s_str stra;
  len = 4;
  str_init(&stra, false, len, "test");
  TEST_ASSERT(stra.bytes == len);
  TEST_ASSERT(strncmp(stra.ptr.p, "test", len) == 0);
  TEST_STR_CLEAN(stra);
  len = 4;
  m = malloc(len);
  memcpy(m, "test", len);
  str_init(&stra, true, len, m);
  TEST_ASSERT(stra.bytes == len);
  TEST_ASSERT(strncmp(stra.ptr.p, "test", len) == 0);
  TEST_STR_CLEAN(stra);
  TEST_ASSERT(str = str_1(false, "test"));
  TEST_STR_DELETE(str);
  TEST_ASSERT(str = str_cpy(4, "test"));
  TEST_STR_DELETE(str);
  TEST_ASSERT(str = str_f("test%d", 42));
  TEST_STR_DELETE(str);
  TEST_ASSERT(str = str_f("test%d", 42));
  TEST_STR_DELETE(str);
  len = 4;
  TEST_ASSERT(str = str_new(false, len, "test"));
  TEST_ASSERT(str->bytes == len);
  TEST_ASSERT(strncmp(str->ptr.p, "test", len) == 0);
  TEST_STR_DELETE(str);
  len = 4;
  m = malloc(len);
  memcpy(m, "test", len);
  TEST_ASSERT(str = str_new(true, len, m));
  TEST_ASSERT(str->bytes == len);
  TEST_ASSERT(strncmp(str->ptr.p, "test", len) == 0);
  TEST_STR_DELETE(str);
  /* TODO: str_append_* */
  /* TODO: str_fputs */
  /* TODO: str_puts */
  /* TODO: str_inspect */
}
