/* c3
 * Copyright 2022 Thomas de Grivel <thoxdg@gmail.com>
 */
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include "../libc3/str.h"
#include "test.h"

#define STR_TEST_INSPECT_1(test, result)                        \
  do {                                                          \
    assert(test);                                               \
    assert(result);                                             \
    STR_TEST_INSPECT(str_new_1(false, test), result);           \
  } while (0)

#define STR_TEST_INSPECT(test, result)                          \
  do {                                                          \
    s_str *str_test;                                            \
    s_str *str_result;                                          \
    str_test = test;                                            \
    assert(str_test);                                           \
    assert(result);                                             \
    test_context("str_inspect(" # test ") -> " # result);       \
    TEST_ASSERT((str_result = str_inspect(str_test)));          \
    TEST_STRNCMP(str_result->ptr.p, result, str_result->size);  \
    str_delete(str_result);                                     \
    str_delete(str_test);                                       \
    test_context(NULL);                                         \
  } while (0)

#define STR_TEST_TO_HEX(test, result)                     \
  do {                                                    \
    s_str *str;                                           \
    test_context("str_to_hex(" # test ") -> " # result);  \
    str = str_to_hex(test);                               \
    TEST_STRNCMP(str->ptr.p, result, str->size);          \
    str_delete(str);                                      \
    str_delete(test);                                     \
  } while (0)

void str_test_character_is_reserved ();
void str_test_init_clean ();
void str_test_inspect ();
void str_test_new_1 ();
void str_test_new_cpy ();
void str_test_new_delete ();
void str_test_new_f ();

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

void str_test_inspect ()
{
  s8 zero[16] = {0};
  STR_TEST_INSPECT_1("", "\"\"");
  STR_TEST_INSPECT_1(" ", "\" \"");
  STR_TEST_INSPECT_1("\n", "\"\\n\"");
  STR_TEST_INSPECT_1("\r", "\"\\r\"");
  STR_TEST_INSPECT_1("\t", "\"\\t\"");
  STR_TEST_INSPECT_1("\v", "\"\\v\"");
  STR_TEST_INSPECT_1("\"", "\"\\\"\"");
  STR_TEST_INSPECT_1(".", "\".\"");
  STR_TEST_INSPECT_1("..", "\"..\"");
  STR_TEST_INSPECT_1("...", "\"...\"");
  STR_TEST_INSPECT_1(".. .", "\".. .\"");
  STR_TEST_INSPECT_1("t", "\"t\"");
  STR_TEST_INSPECT_1("T", "\"T\"");
  STR_TEST_INSPECT_1("test", "\"test\"");
  STR_TEST_INSPECT_1("Test", "\"Test\"");
  STR_TEST_INSPECT_1("123", "\"123\"");
  STR_TEST_INSPECT_1("test123", "\"test123\"");
  STR_TEST_INSPECT_1("Test123", "\"Test123\"");
  STR_TEST_INSPECT_1("test 123", "\"test 123\"");
  STR_TEST_INSPECT_1("Test 123", "\"Test 123\"");
  STR_TEST_INSPECT_1("test123.test456", "\"test123.test456\"");
  STR_TEST_INSPECT_1("Test123.Test456", "\"Test123.Test456\"");
  STR_TEST_INSPECT_1("É", "\"É\"");
  STR_TEST_INSPECT_1("Éo", "\"Éo\"");
  STR_TEST_INSPECT_1("Éoà \n\r\t\v\"",
                     "\"Éoà \\n\\r\\t\\v\\\"\"");
  STR_TEST_INSPECT_1("é", "\"é\"");
  STR_TEST_INSPECT_1("éo", "\"éo\"");
  STR_TEST_INSPECT_1("éoà \n\r\t\v\"",
                     "\"éoà \\n\\r\\t\\v\\\"\"");
  STR_TEST_INSPECT_1("\xff", "\"\\xFF\"");
  STR_TEST_INSPECT_1("\xff\xff", "\"\\xFF\\xFF\"");
  STR_TEST_INSPECT_1("\xff\xff", "\"\\xFF\\xFF\"");
  STR_TEST_INSPECT_1("\xff\xff\xff", "\"\\xFF\\xFF\\xFF\"");
  STR_TEST_INSPECT_1("\xff\xff\xff\xff", "\"\\xFF\\xFF\\xFF\\xFF\"");
  STR_TEST_INSPECT_1("\xff\xff\xff\xff\xff",
                     "\"\\xFF\\xFF\\xFF\\xFF\\xFF\"");
  STR_TEST_INSPECT_1("\xff\xff\xff\xff\xff\xff",
                     "\"\\xFF\\xFF\\xFF\\xFF\\xFF\\xFF\"");
  STR_TEST_INSPECT_1("\xff\xff\xff\xff\xff\xff\xff",
                     "\"\\xFF\\xFF\\xFF\\xFF\\xFF\\xFF\\xFF\"");
  STR_TEST_INSPECT_1("\xff\xff\xff\xff\xff\xff\xff\xff",
                     "\"\\xFF\\xFF\\xFF\\xFF\\xFF\\xFF\\xFF\\xFF\"");
  STR_TEST_INSPECT(str_new(false,  1, zero), "\"\\0\"");
  STR_TEST_INSPECT(str_new(false,  2, zero), "\"\\0\\0\"");
  STR_TEST_INSPECT(str_new(false,  3, zero), "\"\\0\\0\\0\"");
  STR_TEST_INSPECT(str_new(false,  4, zero), "\"\\0\\0\\0\\0\"");
  STR_TEST_INSPECT(str_new(false,  5, zero), "\"\\0\\0\\0\\0\\0\"");
  STR_TEST_INSPECT(str_new(false,  6, zero), "\"\\0\\0\\0\\0\\0\\0\"");
  STR_TEST_INSPECT(str_new(false,  7, zero),
                   "\"\\0\\0\\0\\0\\0\\0\\0\"");
  STR_TEST_INSPECT(str_new(false,  8, zero),
                   "\"\\0\\0\\0\\0\\0\\0\\0\\0\"");
  STR_TEST_INSPECT(str_new(false,  9, zero),
                   "\"\\0\\0\\0\\0\\0\\0\\0\\0\\0\"");
  STR_TEST_INSPECT(str_new(false, 10, zero),
                   "\"\\0\\0\\0\\0\\0\\0\\0\\0\\0\\0\"");
  STR_TEST_INSPECT(str_new(false, 11, zero),
                   "\"\\0\\0\\0\\0\\0\\0\\0\\0\\0\\0\\0\"");
  STR_TEST_INSPECT(str_new(false, 12, zero),
                   "\"\\0\\0\\0\\0\\0\\0\\0\\0\\0\\0\\0\\0\"");
  STR_TEST_INSPECT(str_new(false, 13, zero),
                   "\"\\0\\0\\0\\0\\0\\0\\0\\0\\0\\0\\0\\0\\0\"");
  STR_TEST_INSPECT(str_new(false, 14, zero),
                   "\"\\0\\0\\0\\0\\0\\0\\0\\0\\0\\0\\0\\0\\0\\0\"");
  STR_TEST_INSPECT(str_new(false, 15, zero),
                   "\"\\0\\0\\0\\0\\0\\0\\0\\0\\0\\0\\0\\0\\0\\0\\0\"");
  STR_TEST_INSPECT(str_new(false, 16, zero),
                   "\"\\0\\0\\0\\0\\0\\0\\0\\0\\0\\0\\0\\0\\0\\0\\0"
                   "\\0\"");
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

void str_test_to_hex ()
{
  s8 zero[32] = {0};
  STR_TEST_TO_HEX(str_new_1(false, ""), "");
  STR_TEST_TO_HEX(str_new(false,  1, zero), "00");
  STR_TEST_TO_HEX(str_new(false,  2, zero), "0000");
  STR_TEST_TO_HEX(str_new(false,  3, zero), "000000");
  STR_TEST_TO_HEX(str_new(false,  4, zero), "00000000");
  STR_TEST_TO_HEX(str_new(false,  5, zero), "0000000000");
  STR_TEST_TO_HEX(str_new(false,  6, zero), "000000000000");
  STR_TEST_TO_HEX(str_new(false,  7, zero), "00000000000000");
  STR_TEST_TO_HEX(str_new(false,  8, zero), "0000000000000000");
  STR_TEST_TO_HEX(str_new(false,  9, zero), "000000000000000000");
  STR_TEST_TO_HEX(str_new(false, 10, zero), "00000000000000000000");
  STR_TEST_TO_HEX(str_new(false, 11, zero), "0000000000000000000000");
  STR_TEST_TO_HEX(str_new(false, 12, zero),
                  "000000000000000000000000");
  STR_TEST_TO_HEX(str_new(false, 13, zero),
                  "00000000000000000000000000");
  STR_TEST_TO_HEX(str_new(false, 14, zero),
                  "0000000000000000000000000000");
  STR_TEST_TO_HEX(str_new(false, 15, zero),
                  "000000000000000000000000000000");
  STR_TEST_TO_HEX(str_new(false, 16, zero),
                  "00000000000000000000000000000000");
  STR_TEST_TO_HEX(str_new(false, 17, zero),
                  "00000000000000000000000000000000"
                  "00");
  STR_TEST_TO_HEX(str_new(false, 18, zero),
                  "00000000000000000000000000000000"
                  "0000");
  STR_TEST_TO_HEX(str_new(false, 19, zero),
                  "00000000000000000000000000000000"
                  "000000");
  STR_TEST_TO_HEX(str_new(false, 20, zero),
                  "00000000000000000000000000000000"
                  "00000000");
  STR_TEST_TO_HEX(str_new(false, 21, zero),
                  "00000000000000000000000000000000"
                  "0000000000");
  STR_TEST_TO_HEX(str_new(false, 22, zero),
                  "00000000000000000000000000000000"
                  "000000000000");
  STR_TEST_TO_HEX(str_new(false, 23, zero),
                  "00000000000000000000000000000000"
                  "00000000000000");
  STR_TEST_TO_HEX(str_new(false, 24, zero),
                  "00000000000000000000000000000000"
                  "0000000000000000");
  STR_TEST_TO_HEX(str_new(false, 25, zero),
                  "00000000000000000000000000000000"
                  "000000000000000000");
  STR_TEST_TO_HEX(str_new(false, 26, zero),
                  "00000000000000000000000000000000"
                  "00000000000000000000");
  STR_TEST_TO_HEX(str_new(false, 27, zero),
                  "00000000000000000000000000000000"
                  "0000000000000000000000");
  STR_TEST_TO_HEX(str_new(false, 28, zero),
                  "00000000000000000000000000000000"
                  "000000000000000000000000");
  STR_TEST_TO_HEX(str_new(false, 29, zero),
                  "00000000000000000000000000000000"
                  "00000000000000000000000000");
  STR_TEST_TO_HEX(str_new(false, 30, zero),
                  "00000000000000000000000000000000"
                  "0000000000000000000000000000");
  STR_TEST_TO_HEX(str_new(false, 31, zero),
                  "00000000000000000000000000000000"
                  "000000000000000000000000000000");
  STR_TEST_TO_HEX(str_new(false, 32, zero),
                  "00000000000000000000000000000000"
                  "00000000000000000000000000000000");
  STR_TEST_TO_HEX(str_new_1(false, "abc"), "616263");
  STR_TEST_TO_HEX(str_new_1(false, "abcdefghijklmnopqrstuvwxyz"),
                  "6162636465666768696A6B6C6D6E6F70"
                  "7172737475767778797A");
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
  str_test_to_hex();
}
