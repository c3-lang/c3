/* c3
 * Copyright 2022 Thomas de Grivel <thoxdg@gmail.com>
 */
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include "../libc3/str.h"
#include "test.h"

void str_test_init_clean ()
{
  size_t len;
  char *m;
  s_str stra;
  len = 4;
  str_init(&stra, false, len, "test");
  TEST_EQ(stra.size, len);
  TEST_STRNCMP(stra.ptr.p, "test", len);
  str_clean(&stra);
  test_ok();
  len = 4;
  m = malloc(len);
  assert(m);
  memcpy(m, "test", len);
  str_init(&stra, true, len, m);
  TEST_EQ(stra.size, len);
  TEST_STRNCMP(stra.ptr.p, "test", len);
  str_clean(&stra);
  test_ok();
}

void str_test_new_delete ()
{
  size_t len;
  char *m;
  s_str *str;
  len = 4;
  TEST_ASSERT((str = str_new(false, len, "test")));
  TEST_EQ(str->size, len);
  TEST_STRNCMP(str->ptr.p, "test", len);
  str_delete(str);
  test_ok();
  len = 4;
  m = malloc(len);
  memcpy(m, "test", len);
  TEST_ASSERT((str = str_new(true, len, m)));
  TEST_EQ(str->size, len);
  TEST_STRNCMP(str->ptr.p, "test", len);
  str_delete(str);
  test_ok();
}

void str_test_new_1 ()
{
  s_str *str;
  TEST_ASSERT((str = str_new_1(false, "test")));
  str_delete(str);
}

void str_test_new_cpy ()
{
  s_str *str;
  TEST_ASSERT((str = str_new_cpy(4, "test")));
  str_delete(str);
}

void str_test_new_f ()
{
  s_str *str;
  TEST_ASSERT((str = str_new_f("test%d", 42)));
  TEST_STRNCMP(str->ptr.p, "test42", str->size);
  str_delete(str);
  TEST_ASSERT((str = str_new_f("test%lld", (long long) 42)));
  TEST_STRNCMP(str->ptr.p, "test42", str->size);
  str_delete(str);
}

static void test_hex_ncmp (s8 *str_given, s8 *hex_result)
{
  s_str *str;
  s_str stra;
  str_init_1(&stra, false, str_given);
  str = str_to_hex(&stra);
  TEST_STRNCMP(str->ptr.p, hex_result, str->size);
  str_delete(str);
}

void str_test_hex ()
{
  test_hex_ncmp("abc", "616263");
  test_hex_ncmp("abcdefghijklmnopqrstuvwxyz", "6162636465666768696a6b6c6d6e6f707172737475767778797a");
  test_hex_ncmp("", "");
}

void str_test ()
{
  str_test_init_clean();
  str_test_new_delete();
  str_test_new_1();
  str_test_new_cpy();
  str_test_new_f();
}
