/* c3
 * Copyright 2022 Thomas de Grivel <thoxdg@gmail.com>
 */
#include <stdlib.h>
#include <string.h>
#include "../libc3/buf.h"
#include "../libc3/buf_inspect.h"
#include "../libc3/str.h"
#include "test.h"

#define BUF_INSPECT_TEST_CHARACTER(test, result)                       \
  do {                                                                 \
    s8 b[32];                                                          \
    s_buf buf;                                                         \
    test_context("buf_inspect_character(" # test ") -> " # result);    \
    buf_init(&buf, false, sizeof(b), b);                               \
    TEST_EQ(buf_inspect_character(&buf, test), strlen(result));        \
    TEST_STRNCMP(buf.ptr.ps8, result, buf.wpos);                       \
    test_context(NULL);                                                \
  } while (0)

#define BUF_INSPECT_TEST_STR_CHARACTER(test, result)                   \
  do {                                                                 \
    s8 b[32];                                                          \
    s_buf buf;                                                         \
    test_context("buf_inspect_str_character(" # test ") -> "           \
                 # result);                                            \
    buf_init(&buf, false, sizeof(b), b);                               \
    TEST_EQ(buf_inspect_str_character(&buf, test), strlen(result));    \
    TEST_STRNCMP(buf.ptr.ps8, result, buf.wpos);                       \
    test_context(NULL);                                                \
  } while (0)

#define BUF_INSPECT_TEST_STR_CHARACTER_SIZE(test, result)              \
  do {                                                                 \
    s8 b[32];                                                          \
    s_buf buf;                                                         \
    test_context("buf_inspect_str_character_size("                     \
                 # test ") -> " # result);                             \
    buf_init(&buf, false, sizeof(b), b);                               \
    TEST_EQ(buf_inspect_str_character_size(test), result);             \
    test_context(NULL);                                                \
  } while (0)

#define BUF_INSPECT_TEST_STR(test, result)                             \
  do {                                                                 \
    s8 b[1024];                                                        \
    s_buf buf;                                                         \
    s_str str;                                                         \
    test_context("buf_inspect_str(" # test ") -> " # result);          \
    str_init_1(&str, false, (test));                                   \
    buf_init(&buf, false, sizeof(b), b);                               \
    TEST_EQ(buf_inspect_str(&buf, &str), strlen(result));              \
    TEST_STRNCMP(buf.ptr.p, (result), buf.wpos);                       \
    test_context(NULL);                                                \
  } while (0)

#define BUF_INSPECT_TEST_INTEGER_DEC(test, result)                     \
  do {                                                                 \
  } while (0)

void buf_inspect_test_character ();
void buf_inspect_test_str_character ();
void buf_inspect_test_str_character_size ();
void buf_inspect_test_str ();
void buf_inspect_test_integer_dec ();

void buf_inspect_test ()
{
  buf_inspect_test_str_character_size();
  buf_inspect_test_str_character();
  buf_inspect_test_character();
  buf_inspect_test_str();
  buf_inspect_test_integer_dec();
}

void buf_inspect_test_character ()
{
  BUF_INSPECT_TEST_CHARACTER(0, "'\\0'");
  BUF_INSPECT_TEST_CHARACTER(1, "'\\x01'");
  BUF_INSPECT_TEST_CHARACTER(2, "'\\x02'");
  BUF_INSPECT_TEST_CHARACTER('0', "'0'");
  BUF_INSPECT_TEST_CHARACTER('9', "'9'");
  BUF_INSPECT_TEST_CHARACTER('A', "'A'");
  BUF_INSPECT_TEST_CHARACTER('Z', "'Z'");
  BUF_INSPECT_TEST_CHARACTER('a', "'a'");
  BUF_INSPECT_TEST_CHARACTER('z', "'z'");
  BUF_INSPECT_TEST_CHARACTER(928, "'Π'");
  BUF_INSPECT_TEST_CHARACTER(0xFF, "'ÿ'");
  BUF_INSPECT_TEST_CHARACTER(42164, "'꒴'");
  BUF_INSPECT_TEST_CHARACTER(65856, "'𐅀'");
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
  BUF_INSPECT_TEST_STR("éoπꝝ꒴", "\"éoπꝝ꒴\"");
 }

void buf_inspect_test_integer_dec ()
{
  BUF_INSPECT_TEST_INTEGER_DEC("9", 9);
  BUF_INSPECT_TEST_INTEGER_DEC("256", 256);
  BUF_INSPECT_TEST_INTEGER_DEC("100000000000000000", 100000000000000000);
}