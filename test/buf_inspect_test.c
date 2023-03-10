/* c3
 * Copyright 2022,2023 kmx.io <contact@kmx.io>
 *
 * Permission is hereby granted to use this software excepted
 * on Apple computers granted the above copyright notice and
 * this permission paragraph are included in all copies and
 * substantial portions of this software.
 *
 * THIS SOFTWARE IS PROVIDED "AS-IS" WITHOUT ANY GUARANTEE OF
 * PURPOSE AND PERFORMANCE. IN NO EVENT WHATSOEVER SHALL THE
 * AUTHOR BE CONSIDERED LIABLE FOR THE USE AND PERFORMANCE OF
 * THIS SOFTWARE.
 */
#include <stdlib.h>
#include <string.h>
#include "../libc3/c3.h"
#include "test.h"

#define BUF_INSPECT_TEST_BOOL(test, expected)                          \
  do {                                                                 \
    s8 b[16];                                                          \
    s_buf buf;                                                         \
    test_context("buf_inspect_bool(" # test ") -> " # expected);       \
    buf_init(&buf, false, sizeof(b), b);                               \
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
    TEST_STRNCMP(buf.ptr.ps8, (expected), buf.wpos);                   \
    test_context(NULL);                                                \
  } while (0)

#define BUF_INSPECT_TEST_F32(test, expected)                           \
  do {                                                                 \
    s8 b[32];                                                          \
    s_buf buf;                                                         \
    test_context("buf_inspect_f32(" # test ") -> " # expected);        \
    buf_init(&buf, false, sizeof(b), b);                               \
    TEST_EQ(buf_inspect_f32_size(test), strlen(expected));             \
    TEST_EQ(buf_inspect_f32(&buf, test), strlen(expected));            \
    TEST_STRNCMP(buf.ptr.ps8, (expected), buf.wpos);                   \
    test_context(NULL);                                                \
  } while (0)

#define BUF_INSPECT_TEST_F64(test, expected)                           \
  do {                                                                 \
    s8 b[64];                                                          \
    s_buf buf;                                                         \
    test_context("buf_inspect_f64(" # test ") -> " # expected);        \
    buf_init(&buf, false, sizeof(b), b);                               \
    TEST_EQ(buf_inspect_f64_size(test), strlen(expected));             \
    TEST_EQ(buf_inspect_f64(&buf, test), strlen(expected));            \
    TEST_STRNCMP(buf.ptr.ps8, (expected), buf.wpos);                   \
    test_context(NULL);                                                \
  } while (0)

#define BUF_INSPECT_TEST_INTEGER(test, expected)                       \
  do {                                                                 \
    s8 b[1024];                                                        \
    s_buf buf_test;                                                    \
    s_buf buf_result;                                                  \
    s_integer i;                                                       \
    test_context("buf_inspect_integer(" # test ") -> " # expected);    \
    buf_init_1(&buf_test, (test));                                     \
    buf_parse_integer(&buf_test, &i);                                  \
    buf_clean(&buf_test);                                              \
    buf_init(&buf_result, false, sizeof(b), b);                        \
    TEST_EQ(buf_inspect_integer(&buf_result, &i), strlen(test));       \
    if (g_test_last_ok)                                                \
      integer_clean(&i);					       \
    TEST_EQ(buf_result.wpos, strlen(test));                            \
    if (g_test_last_ok)                                                \
      TEST_STRNCMP(buf_result.ptr.ps8, (expected), buf_result.wpos);   \
    buf_clean(&buf_result);                                            \
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
    TEST_STRNCMP(buf.ptr.ps8, (expected), buf.wpos);                   \
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
    str_init_1(&str, NULL, (test));                                   \
    buf_init(&buf, false, sizeof(b), b);                               \
    TEST_EQ(buf_inspect_str(&buf, &str), strlen(expected));            \
    TEST_STRNCMP(buf.ptr.p, (expected), buf.wpos);                     \
    test_context(NULL);                                                \
  } while (0)

#define BUF_INSPECT_TEST_TAG(test, expected)			       \
  do {                                                                 \
    s_buf buf;                                                         \
    s_tag *test_tag;                                                   \
    test_context("buf_inspect_tag(" # test ") -> " # expected);        \
    test_tag = (test);                                                 \
    buf_init_alloc(&buf, strlen(expected));                            \
    TEST_EQ(buf_inspect_tag_size(test_tag), strlen(expected));         \
    TEST_EQ(buf_inspect_tag(&buf, test_tag), strlen(expected));        \
    TEST_EQ(buf.wpos, strlen(expected));                               \
    if (g_test_last_ok)                                                \
      TEST_STRNCMP(buf.ptr.ps8, (expected), buf.wpos);                 \
    buf_clean(&buf);                                                   \
  } while (0)

void buf_inspect_test ();
void buf_inspect_test_bool ();
void buf_inspect_test_character ();
void buf_inspect_test_f32 ();
void buf_inspect_test_f64 ();
void buf_inspect_test_integer ();
void buf_inspect_test_list ();
void buf_inspect_test_str ();
void buf_inspect_test_str_character ();
void buf_inspect_test_str_character_size ();
void buf_inspect_test_tag ();

void buf_inspect_test ()
{
  buf_inspect_test_bool();
  buf_inspect_test_str_character_size();
  buf_inspect_test_str_character();
  buf_inspect_test_character();
  buf_inspect_test_f32();
  buf_inspect_test_f64();
  buf_inspect_test_integer();
  buf_inspect_test_list();
  buf_inspect_test_str();
  buf_inspect_test_tag();
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
  BUF_INSPECT_TEST_CHARACTER(928,    "'??'");
  BUF_INSPECT_TEST_CHARACTER(0xFF,   "'??'");
  BUF_INSPECT_TEST_CHARACTER(42164,  "'???'");
  BUF_INSPECT_TEST_CHARACTER(65856,  "'????'");
  BUF_INSPECT_TEST_CHARACTER(127923, "'????'");
  BUF_INSPECT_TEST_CHARACTER(128516, "'????'");
  BUF_INSPECT_TEST_CHARACTER(128995, "'????'");
  BUF_INSPECT_TEST_CHARACTER(129321, "'????'");
}

void buf_inspect_test_f32 ()
{
  BUF_INSPECT_TEST_F32(0.0, "0");
  BUF_INSPECT_TEST_F32(0.1, "0.1");
  BUF_INSPECT_TEST_F32(0.123456789, "0.123457");
  BUF_INSPECT_TEST_F32(1.23456789, "1.23457");
  BUF_INSPECT_TEST_F32(123456789.0, "1.23457e+08");
  BUF_INSPECT_TEST_F32(-0.1, "-0.1");
  BUF_INSPECT_TEST_F32(-0.123456789, "-0.123457");
  BUF_INSPECT_TEST_F32(-1.23456789, "-1.23457");
  BUF_INSPECT_TEST_F32(-123456789.0, "-1.23457e+08");
}

void buf_inspect_test_f64 ()
{
  BUF_INSPECT_TEST_F64(0.0, "0");
  BUF_INSPECT_TEST_F64(0.1, "0.1");
  BUF_INSPECT_TEST_F64(0.123456789, "0.123457");
  BUF_INSPECT_TEST_F64(1.23456789, "1.23457");
  BUF_INSPECT_TEST_F64(123456789.0, "1.23457e+08");
  BUF_INSPECT_TEST_F64(-0.1, "-0.1");
  BUF_INSPECT_TEST_F64(-0.123456789, "-0.123457");
  BUF_INSPECT_TEST_F64(-1.23456789, "-1.23457");
  BUF_INSPECT_TEST_F64(-123456789.0, "-1.23457e+08");
}

void buf_inspect_test_integer ()
{
  BUF_INSPECT_TEST_INTEGER("0", "0");
  BUF_INSPECT_TEST_INTEGER("1234", "1234");
  BUF_INSPECT_TEST_INTEGER("-1234", "-1234");
  BUF_INSPECT_TEST_INTEGER("12345678901234567890123456789013",
			   "12345678901234567890123456789013");
  BUF_INSPECT_TEST_INTEGER("-1234567890123456789012345678901",
			   "-1234567890123456789012345678901");
  BUF_INSPECT_TEST_INTEGER("12345678901234567890123456789012332132131",
			   "12345678901234567890123456789012332132131");
  BUF_INSPECT_TEST_INTEGER("12345678901234567890123456789012332132131123456789012345678901234567890123321321311234567890123456789012345678901233213213112345678901234567890123456789012332132131123456789012345678901234567890123321321311234567890123456789012345678901233213213112345678901234567890123456789012332132131",
			   "12345678901234567890123456789012332132131123456789012345678901234567890123321321311234567890123456789012345678901233213213112345678901234567890123456789012332132131123456789012345678901234567890123321321311234567890123456789012345678901233213213112345678901234567890123456789012332132131");
  BUF_INSPECT_TEST_INTEGER("-12345678901234567890123456789012332132131123456789012345678901234567890123321321311234567890123456789012345678901233213213112345678901234567890123456789012332132131123456789012345678901234567890123321321311234567890123456789012345678901233213213112345678901234567890123456789012332132131",
			   "-12345678901234567890123456789012332132131123456789012345678901234567890123321321311234567890123456789012345678901233213213112345678901234567890123456789012332132131123456789012345678901234567890123321321311234567890123456789012345678901233213213112345678901234567890123456789012332132131");
  BUF_INSPECT_TEST_INTEGER("123456789012345678901234567890123321321311234567890123456789012345678901233213213112345678901234567890123456789012332132131123456789012345678901234567890123321321311234567890123456789012345678901233213213112345678901234567890123456789012332132131123456789012345678901234567890123321321311234567890123456789012345678901233213213112345678901234567890123456789012332132131123456789012345678901234567890123321321311234567890123456789012345678901233213213112345678901234567890123456789012332132131123456789012345678901234567890123321321311234567890123456789012345678901233213213112345678901234567890123456789012332132131123456789012345678901234567890123321321311234567890123456789012345678901233213213112345678901234567890123456789012332132131123456789012345678901234567890123321321311234567890123456789012345678901233213213112345678901234567890123456789012332132131",
			   "123456789012345678901234567890123321321311234567890123456789012345678901233213213112345678901234567890123456789012332132131123456789012345678901234567890123321321311234567890123456789012345678901233213213112345678901234567890123456789012332132131123456789012345678901234567890123321321311234567890123456789012345678901233213213112345678901234567890123456789012332132131123456789012345678901234567890123321321311234567890123456789012345678901233213213112345678901234567890123456789012332132131123456789012345678901234567890123321321311234567890123456789012345678901233213213112345678901234567890123456789012332132131123456789012345678901234567890123321321311234567890123456789012345678901233213213112345678901234567890123456789012332132131123456789012345678901234567890123321321311234567890123456789012345678901233213213112345678901234567890123456789012332132131");
}

void buf_inspect_test_list ()
{
  BUF_INSPECT_TEST_LIST("[]", "[]");
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
  BUF_INSPECT_TEST_STR("??", "\"??\"");
  BUF_INSPECT_TEST_STR("??o", "\"??o\"");
  BUF_INSPECT_TEST_STR("??o?? \n\r\t\v\"",
                       "\"??o?? \\n\\r\\t\\v\\\"\"");
  BUF_INSPECT_TEST_STR("??o?? \n\r\t\v\"",
                       "\"??o?? \\n\\r\\t\\v\\\"\"");
  BUF_INSPECT_TEST_STR("??", "\"??\"");
  BUF_INSPECT_TEST_STR("??o????????????????????????????", "\"??o????????????????????????????\"");
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
  BUF_INSPECT_TEST_STR_CHARACTER(928, "??");
  BUF_INSPECT_TEST_STR_CHARACTER(0xFF, "??");
  BUF_INSPECT_TEST_STR_CHARACTER(42164, "???");
  BUF_INSPECT_TEST_STR_CHARACTER(65856, "????");
  BUF_INSPECT_TEST_STR_CHARACTER(127923, "????");
  BUF_INSPECT_TEST_STR_CHARACTER(128516, "????");
  BUF_INSPECT_TEST_STR_CHARACTER(128995, "????");
  BUF_INSPECT_TEST_STR_CHARACTER(129321, "????");
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

void buf_inspect_test_tag ()
{
  s_tag tag;
  tag_init(&tag);
  BUF_INSPECT_TEST_TAG(tag_bool(&tag, false), "false");
  BUF_INSPECT_TEST_TAG(tag_bool(&tag, true), "true");
  BUF_INSPECT_TEST_TAG(tag_character(&tag, '\n'), "'\\n'");
  BUF_INSPECT_TEST_TAG(tag_f32(&tag, 1.234), "1.234");
  BUF_INSPECT_TEST_TAG(tag_f64(&tag, 1.2345), "1.2345");
  BUF_INSPECT_TEST_TAG(tag_ident_1(&tag, "ident"), "ident");
  BUF_INSPECT_TEST_TAG(tag_integer_1(&tag, "-0x10000000000000000"), "-18446744073709551616");
  BUF_INSPECT_TEST_TAG(tag_integer_1(&tag, "0x10000000000000000"), "18446744073709551616");
  BUF_INSPECT_TEST_TAG(tag_list(&tag, NULL), "[]");
  BUF_INSPECT_TEST_TAG(tag_list_1(&tag, "[[] | []]"), "[[]]");
  BUF_INSPECT_TEST_TAG(tag_list_1(&tag, "[[]]"), "[[]]");
  BUF_INSPECT_TEST_TAG(tag_list_1(&tag, "[]"), "[]");
  BUF_INSPECT_TEST_TAG(tag_s16(&tag, -0x100), "-256");
  BUF_INSPECT_TEST_TAG(tag_s32(&tag, -0x10000), "-65536");
  BUF_INSPECT_TEST_TAG(tag_s64(&tag, -0x100000000), "-4294967296");
  BUF_INSPECT_TEST_TAG(tag_s8(&tag, -1), "-1");
  BUF_INSPECT_TEST_TAG(tag_s8(&tag, -10), "-10");
  BUF_INSPECT_TEST_TAG(tag_str(&tag, NULL, 0, ""), "\"\"");
  BUF_INSPECT_TEST_TAG(tag_str_1(&tag, NULL, ""), "\"\"");
  BUF_INSPECT_TEST_TAG(tag_str_1(&tag, NULL, "09AZaz"), "\"09AZaz\"");
  BUF_INSPECT_TEST_TAG(tag_sym(&tag, sym_1("Module")), "Module");
  BUF_INSPECT_TEST_TAG(tag_sym_1(&tag, "Module"), "Module");
  BUF_INSPECT_TEST_TAG(tag_u16(&tag, 0x100), "256");
  BUF_INSPECT_TEST_TAG(tag_u32(&tag, 0x10000), "65536");
  BUF_INSPECT_TEST_TAG(tag_u64(&tag, 0x100000000), "4294967296");
  BUF_INSPECT_TEST_TAG(tag_u8(&tag, 0), "0");
  BUF_INSPECT_TEST_TAG(tag_u8(&tag, 1), "1");
  BUF_INSPECT_TEST_TAG(tag_u8(&tag, 10), "10");
  BUF_INSPECT_TEST_TAG(tag_void(&tag), "");
  tag_clean(&tag);
}
