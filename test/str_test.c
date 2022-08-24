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
    TEST_EQ(stra.bytes, 0);                     \
    TEST_ASSERT(stra.ptr.p == NULL);            \
    TEST_EQ(stra.ptr.u64, 0);                   \
  } while(0)

#define TEST_STR_DELETE(str) \
  do {                       \
    str_delete(str);         \
    test_ok();               \
  } while(0)

void str_test_init_clean ()
{
  size_t len;
  char *m;
  s_str stra;
  len = 4;
  str_init(&stra, false, len, "test");
  TEST_EQ(stra.bytes, len);
  TEST_STRNCMP(stra.ptr.p, "test", len);
  TEST_STR_CLEAN(stra);
  len = 4;
  m = malloc(len);
  memcpy(m, "test", len);
  str_init(&stra, true, len, m);
  TEST_EQ(stra.bytes, len);
  TEST_STRNCMP(stra.ptr.p, "test", len);
  TEST_STR_CLEAN(stra);
}

void str_test_new_delete ()
{
  size_t len;
  char *m;
  s_str *str;
  len = 4;
  TEST_ASSERT((str = str_new(false, len, "test")));
  TEST_EQ(str->bytes, len);
  TEST_STRNCMP(str->ptr.p, "test", len);
  TEST_STR_DELETE(str);
  len = 4;
  m = malloc(len);
  memcpy(m, "test", len);
  TEST_ASSERT((str = str_new(true, len, m)));
  TEST_EQ(str->bytes, len);
  TEST_STRNCMP(str->ptr.p, "test", len);
  TEST_STR_DELETE(str);
}

void str_test_1 ()
{
  s_str *str;
  TEST_ASSERT((str = str_1(false, "test")));
  str_delete(str);
}

void str_test_cpy ()
{
  s_str *str;
  TEST_ASSERT((str = str_cpy(4, "test")));
  str_delete(str);
}

void str_test_f ()
{
  s_str *str;
  TEST_ASSERT((str = str_f("test%d", 42)));
  TEST_STRNCMP(str->ptr.p, "test42", str->bytes);
  str_delete(str);
  TEST_ASSERT((str = str_f("test%lld", (long long) 42)));
  TEST_STRNCMP(str->ptr.p, "test42", str->bytes);
  str_delete(str);
}

void str_test ()
{
  str_test_init_clean();
  str_test_new_delete();
  str_test_1();
  str_test_cpy();
  str_test_f();
}
