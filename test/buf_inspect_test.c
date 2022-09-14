/* c3
 * Copyright 2022 Thomas de Grivel <thoxdg@gmail.com>
 */
#include <stdlib.h>
#include <string.h>
#include "../libc3/buf.h"
#include "../libc3/buf_inspect.h"
#include "../libc3/buf_parse.h"
#include "../libc3/str.h"
#include "test.h"

#define BUF_INSPECT_TEST_BOOL(test, expected)                          \
  do {                                                                 \
    s_buf buf;                                                         \
    test_context("buf_inspect_bool(" # test ") -> " # expected);       \
    BUF_INIT_ALLOCA(&buf, 16);                                         \
    TEST_EQ(buf_inspect_bool(&buf, (test)), strlen(expected));         \
    TEST_STRNCMP(buf.ptr.p, (expected), buf.wpos);                     \
    test_context(NULL);                                                \
  } while (0)
  
#define BUF_INSPECT_TEST_CHARACTER(test, expected)                     \
  do {                                                                 \
    s8 b[32];                                                          \
    s_buf buf;                                                         \
    test_context("buf_inspect_character(" # test ") -> " # expected);  \
    buf_init(&buf, false, sizeof(b), b);                               \
    TEST_EQ(buf_inspect_character(&buf, test), strlen(expected));      \
    TEST_STRNCMP(buf.ptr.ps8, expected, buf.wpos);                     \
    test_context(NULL);                                                \
  } while (0)

#define BUF_INSPECT_TEST_LIST(test, expected)                          \
  do {                                                                 \
    s_buf buf;                                                         \
    s_buf buf_test;                                                    \
    s_list *list_test;                                                 \
    test_context("buf_inspect_list(" # test ") -> " # expected);       \
    buf_init_1(&buf_test, (test));                                     \
    if (buf_parse_list(&buf_test, &list_test) != strlen(test))         \
      errx(1, "buf_parse_list");                                       \
    buf_init_alloc(&buf, 1024 * 1024);                                 \
    TEST_EQ(buf_inspect_list(&buf, list_test), strlen(expected));      \
    TEST_EQ(buf.wpos, strlen(expected));                               \
    if (g_test_last_ok)                                                \
      TEST_STRNCMP(buf.ptr.p, (expected), buf.wpos);                   \
    buf_clean(&buf_test);                                              \
    buf_clean(&buf);                                                   \
    test_context(NULL);                                                \
  } while (0)

#define BUF_INSPECT_TEST_STR_CHARACTER(test, expected)                 \
  do {                                                                 \
    s8 b[32];                                                          \
    s_buf buf;                                                         \
    test_context("buf_inspect_str_character(" # test ") -> "           \
                 # expected);                                          \
    buf_init(&buf, false, sizeof(b), b);                               \
    TEST_EQ(buf_inspect_str_character(&buf, test), strlen(expected));  \
    TEST_STRNCMP(buf.ptr.ps8, expected, buf.wpos);                     \
    test_context(NULL);                                                \
  } while (0)

#define BUF_INSPECT_TEST_STR_CHARACTER_SIZE(test, expected)            \
  do {                                                                 \
    s8 b[32];                                                          \
    s_buf buf;                                                         \
    test_context("buf_inspect_str_character_size("                     \
                 # test ") -> " # expected);                           \
    buf_init(&buf, false, sizeof(b), b);                               \
    TEST_EQ(buf_inspect_str_character_size(test), expected);           \
    test_context(NULL);                                                \
  } while (0)

#define BUF_INSPECT_TEST_STR(test, expected)                           \
  do {                                                                 \
    s8 b[1024];                                                        \
    s_buf buf;                                                         \
    s_str str;                                                         \
    test_context("buf_inspect_str(" # test ") -> " # expected);        \
    str_init_1(&str, false, (test));                                   \
    buf_init(&buf, false, sizeof(b), b);                               \
    TEST_EQ(buf_inspect_str(&buf, &str), strlen(expected));            \
    TEST_STRNCMP(buf.ptr.p, (expected), buf.wpos);                     \
    test_context(NULL);                                                \
  } while (0)

void buf_inspect_test ();
void buf_inspect_test_bool ();
void buf_inspect_test_character ();
void buf_inspect_test_list ();
void buf_inspect_test_str_character ();
void buf_inspect_test_str_character_size ();
void buf_inspect_test_str ();

void buf_inspect_test ()
{
  buf_inspect_test_bool();
  buf_inspect_test_str_character_size();
  buf_inspect_test_str_character();
  buf_inspect_test_character();
  buf_inspect_test_list();
  buf_inspect_test_str();
}

void buf_inspect_test_bool ()
{
  BUF_INSPECT_TEST_BOOL(true, "true");
  BUF_INSPECT_TEST_BOOL(false, "false");
}

void buf_inspect_test_character ()
{
  BUF_INSPECT_TEST_CHARACTER(0, "'\\0'");
  BUF_INSPECT_TEST_CHARACTER(1, "'\\x01'");
  BUF_INSPECT_TEST_CHARACTER(2, "'\\x02'");
  BUF_INSPECT_TEST_CHARACTER('0',    "'0'");
  BUF_INSPECT_TEST_CHARACTER('9',    "'9'");
  BUF_INSPECT_TEST_CHARACTER('A',    "'A'");
  BUF_INSPECT_TEST_CHARACTER('Z',    "'Z'");
  BUF_INSPECT_TEST_CHARACTER('a',    "'a'");
  BUF_INSPECT_TEST_CHARACTER('z',    "'z'");
  BUF_INSPECT_TEST_CHARACTER(928,    "'Π'");
  BUF_INSPECT_TEST_CHARACTER(0xFF,   "'ÿ'");
  BUF_INSPECT_TEST_CHARACTER(42164,  "'꒴'");
  BUF_INSPECT_TEST_CHARACTER(65856,  "'𐅀'");
  BUF_INSPECT_TEST_CHARACTER(127923, "'🎳'");
  BUF_INSPECT_TEST_CHARACTER(128516, "'😄'");
  BUF_INSPECT_TEST_CHARACTER(128995, "'🟣'");
  BUF_INSPECT_TEST_CHARACTER(129321, "'🤩'");
}

void buf_inspect_test_list ()
{
  BUF_INSPECT_TEST_LIST("[]", "[]");
}

void buf_inspect_test_str_character ()
{
  BUF_INSPECT_TEST_STR_CHARACTER(0, "\\0");
  BUF_INSPECT_TEST_STR_CHARACTER(1, "\\x01");
  BUF_INSPECT_TEST_STR_CHARACTER(2, "\\x02");
  BUF_INSPECT_TEST_STR_CHARACTER('0', "0");
  BUF_INSPECT_TEST_STR_CHARACTER('9', "9");
  BUF_INSPECT_TEST_STR_CHARACTER('A', "A");
  BUF_INSPECT_TEST_STR_CHARACTER('Z', "Z");
  BUF_INSPECT_TEST_STR_CHARACTER('a', "a");
  BUF_INSPECT_TEST_STR_CHARACTER('z', "z");
  BUF_INSPECT_TEST_STR_CHARACTER(928, "Π");
  BUF_INSPECT_TEST_STR_CHARACTER(0xFF, "ÿ");
  BUF_INSPECT_TEST_STR_CHARACTER(42164, "꒴");
  BUF_INSPECT_TEST_STR_CHARACTER(65856, "𐅀");
  BUF_INSPECT_TEST_STR_CHARACTER(127923, "🎳");
  BUF_INSPECT_TEST_STR_CHARACTER(128516, "😄");
  BUF_INSPECT_TEST_STR_CHARACTER(128995, "🟣");
  BUF_INSPECT_TEST_STR_CHARACTER(129321, "🤩");
}

void buf_inspect_test_str_character_size ()
{
  BUF_INSPECT_TEST_STR_CHARACTER_SIZE(0, 2);
  BUF_INSPECT_TEST_STR_CHARACTER_SIZE(1, 4);
  BUF_INSPECT_TEST_STR_CHARACTER_SIZE(2, 4);
  BUF_INSPECT_TEST_STR_CHARACTER_SIZE('0', 1);
  BUF_INSPECT_TEST_STR_CHARACTER_SIZE('9', 1);
  BUF_INSPECT_TEST_STR_CHARACTER_SIZE('A', 1);
  BUF_INSPECT_TEST_STR_CHARACTER_SIZE('Z', 1);
  BUF_INSPECT_TEST_STR_CHARACTER_SIZE('a', 1);
  BUF_INSPECT_TEST_STR_CHARACTER_SIZE('z', 1);
  BUF_INSPECT_TEST_STR_CHARACTER_SIZE(928, 2);
  BUF_INSPECT_TEST_STR_CHARACTER_SIZE(0xFF, 2);
  BUF_INSPECT_TEST_STR_CHARACTER_SIZE(42164, 3);
  BUF_INSPECT_TEST_STR_CHARACTER_SIZE(65856, 4);
}

void buf_inspect_test_str ()
{
  BUF_INSPECT_TEST_STR("", "\"\"");
  BUF_INSPECT_TEST_STR(" ", "\" \"");
  BUF_INSPECT_TEST_STR("\n", "\"\\n\"");
  BUF_INSPECT_TEST_STR("\r", "\"\\r\"");
  BUF_INSPECT_TEST_STR("\t", "\"\\t\"");
  BUF_INSPECT_TEST_STR("\v", "\"\\v\"");
  BUF_INSPECT_TEST_STR("\"", "\"\\\"\"");
  BUF_INSPECT_TEST_STR(".", "\".\"");
  BUF_INSPECT_TEST_STR("..", "\"..\"");
  BUF_INSPECT_TEST_STR("...", "\"...\"");
  BUF_INSPECT_TEST_STR(".. .", "\".. .\"");
  BUF_INSPECT_TEST_STR("t", "\"t\"");
  BUF_INSPECT_TEST_STR("T", "\"T\"");
  BUF_INSPECT_TEST_STR("test", "\"test\"");
  BUF_INSPECT_TEST_STR("Test", "\"Test\"");
  BUF_INSPECT_TEST_STR("123", "\"123\"");
  BUF_INSPECT_TEST_STR("test123", "\"test123\"");
  BUF_INSPECT_TEST_STR("Test123", "\"Test123\"");
  BUF_INSPECT_TEST_STR("test 123", "\"test 123\"");
  BUF_INSPECT_TEST_STR("Test 123", "\"Test 123\"");
  BUF_INSPECT_TEST_STR("test123.test456", "\"test123.test456\"");
  BUF_INSPECT_TEST_STR("Test123.Test456", "\"Test123.Test456\"");
  BUF_INSPECT_TEST_STR("test123(test456)", "\"test123(test456)\"");
  BUF_INSPECT_TEST_STR("Test123(Test456)", "\"Test123(Test456)\"");
  BUF_INSPECT_TEST_STR("test123{test456}", "\"test123{test456}\"");
  BUF_INSPECT_TEST_STR("Test123{Test456}", "\"Test123{Test456}\"");
  BUF_INSPECT_TEST_STR("É", "\"É\"");
  BUF_INSPECT_TEST_STR("Éo", "\"Éo\"");
  BUF_INSPECT_TEST_STR("Éoà \n\r\t\v\"",
                       "\"Éoà \\n\\r\\t\\v\\\"\"");
  BUF_INSPECT_TEST_STR("éoà \n\r\t\v\"",
                       "\"éoà \\n\\r\\t\\v\\\"\"");
  BUF_INSPECT_TEST_STR("é", "\"é\"");
  BUF_INSPECT_TEST_STR("éoπꝝ꒴𐅀🎳😄🟣🤩", "\"éoπꝝ꒴𐅀🎳😄🟣🤩\"");
 }
