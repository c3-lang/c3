/* c3
 * Copyright 2022 Thomas de Grivel <thoxdg@gmail.com>
 */
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include "../libc3/str.h"
#include "test.h"

void str_test_character_is_reserved ()
{
  TEST_ASSERT(str_character_is_reserved('\0'));
  TEST_ASSERT(str_character_is_reserved('\x01'));
  TEST_ASSERT(str_character_is_reserved('\x02'));
  TEST_ASSERT(str_character_is_reserved('\x03'));
  TEST_ASSERT(! str_character_is_reserved(' '));
  TEST_ASSERT(str_character_is_reserved('\n'));
  TEST_ASSERT(str_character_is_reserved('\r'));
  TEST_ASSERT(str_character_is_reserved('\t'));
  TEST_ASSERT(str_character_is_reserved('\v'));
  TEST_ASSERT(! str_character_is_reserved('0'));
  TEST_ASSERT(! str_character_is_reserved('1'));
  TEST_ASSERT(! str_character_is_reserved('2'));
  TEST_ASSERT(! str_character_is_reserved('7'));
  TEST_ASSERT(! str_character_is_reserved('8'));
  TEST_ASSERT(! str_character_is_reserved('9'));
  TEST_ASSERT(! str_character_is_reserved('A'));
  TEST_ASSERT(! str_character_is_reserved('B'));
  TEST_ASSERT(! str_character_is_reserved('C'));
  TEST_ASSERT(! str_character_is_reserved('X'));
  TEST_ASSERT(! str_character_is_reserved('Y'));
  TEST_ASSERT(! str_character_is_reserved('Z'));
  TEST_ASSERT(! str_character_is_reserved('a'));
  TEST_ASSERT(! str_character_is_reserved('b'));
  TEST_ASSERT(! str_character_is_reserved('c'));
  TEST_ASSERT(! str_character_is_reserved('x'));
  TEST_ASSERT(! str_character_is_reserved('y'));
  TEST_ASSERT(! str_character_is_reserved('z'));
}

void str_test_hex_ncmp (s8 *str_given, s8 *hex_result)
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
  str_test_hex_ncmp("abc", "616263");
  str_test_hex_ncmp("abcdefghijklmnopqrstuvwxyz", "6162636465666768696a6b6c6d6e6f707172737475767778797a");
  str_test_hex_ncmp("", "");
}

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

#define STR_TEST_INSPECT(test, result)                          \
  do {                                                          \
    s_str str_test;                                             \
    s_str *str_result;                                          \
    assert(test);                                               \
    assert(result);                                             \
    test_context("str_inspect(" #test ") -> " #result);         \
    str_init_1(&str_test, false, test);                         \
    TEST_ASSERT((str_result = str_inspect(&str_test)));         \
    TEST_STRNCMP(str_result->ptr.p, result, str_result->size);  \
    str_delete(str_result);                                     \
    test_context(NULL);                                         \
  } while (0)

void str_test_inspect ()
{
  STR_TEST_INSPECT("", "\"\"");
  STR_TEST_INSPECT(" ", "\" \"");
  STR_TEST_INSPECT("\n", "\"\\n\"");
  STR_TEST_INSPECT("\r", "\"\\r\"");
  STR_TEST_INSPECT("\t", "\"\\t\"");
  STR_TEST_INSPECT("\v", "\"\\v\"");
  STR_TEST_INSPECT("\"", "\"\\\"\"");
  STR_TEST_INSPECT(".", "\".\"");
  STR_TEST_INSPECT("..", "\"..\"");
  STR_TEST_INSPECT("...", "\"...\"");
  STR_TEST_INSPECT(".. .", "\".. .\"");
  STR_TEST_INSPECT("t", "\"t\"");
  STR_TEST_INSPECT("T", "\"T\"");
  STR_TEST_INSPECT("test", "\"test\"");
  STR_TEST_INSPECT("Test", "\"Test\"");
  STR_TEST_INSPECT("123", "\"123\"");
  STR_TEST_INSPECT("test123", "\"test123\"");
  STR_TEST_INSPECT("Test123", "\"Test123\"");
  STR_TEST_INSPECT("test 123", "\"test 123\"");
  STR_TEST_INSPECT("Test 123", "\"Test 123\"");
  STR_TEST_INSPECT("test123.test456", "\"test123.test456\"");
  STR_TEST_INSPECT("Test123.Test456", "\"Test123.Test456\"");
  STR_TEST_INSPECT("É", "\"É\"");
  STR_TEST_INSPECT("Éo", "\"Éo\"");
  STR_TEST_INSPECT("Éoà \n\r\t\v\"",
                   "\"Éoà \\n\\r\\t\\v\\\"\"");
  STR_TEST_INSPECT("é", "\"é\"");
  STR_TEST_INSPECT("éo", "\"éo\"");
  STR_TEST_INSPECT("éoà \n\r\t\v\"",
                   "\"éoà \\n\\r\\t\\v\\\"\"");
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

void str_test ()
{
  str_test_init_clean();
  str_test_new_delete();
  str_test_new_1();
  str_test_new_cpy();
  str_test_new_f();
  str_test_character_is_reserved();
  str_test_inspect();
}
