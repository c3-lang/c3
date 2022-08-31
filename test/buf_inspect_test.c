/* c3
 * Copyright 2022 Thomas de Grivel <thoxdg@gmail.com>
 */
#include <stdlib.h>
#include <string.h>
#include "../libc3/buf.h"
#include "../libc3/str.h"
#include "test.h"

#define BUF_TEST_INSPECT_CHARACTER(test, result)                       \
  do {                                                                 \
    test_context("buf_inspect_character(" # test ") -> " # result);    \
    buf_init(&buf, false, sizeof(b), b);                               \
    TEST_EQ(buf_inspect_character(&buf, test), strlen(result));        \
    TEST_STRNCMP(buf.ptr.ps8, result, buf.wpos);                       \
  } while (0)

#define BUF_TEST_INSPECT_STR_CHARACTER(test, result)                   \
  do {                                                                 \
    test_context("buf_inspect_str_character(" # test ") -> "           \
                 # result);                                            \
    buf_init(&buf, false, sizeof(b), b);                               \
    TEST_EQ(buf_inspect_str_character(&buf, test), strlen(result));    \
    TEST_STRNCMP(buf.ptr.ps8, result, buf.wpos);                       \
  } while (0)

#define BUF_TEST_INSPECT_STR_CHARACTER_SIZE(test, result)              \
  do {                                                                 \
    test_context("buf_inspect_str_character_size("                     \
                 # test ") -> " # result);                             \
    buf_init(&buf, false, sizeof(b), b);                               \
    TEST_EQ(buf_inspect_str_character_size(test), result);             \
  } while (0)

void buf_test_inspect_character ();
void buf_test_inspect_str_character ();
void buf_test_inspect_str_character_size ();

void buf_inspect_test ()
{
  buf_test_inspect_str_character_size();
  buf_test_inspect_str_character();
  buf_test_inspect_character();
}

void buf_test_inspect_character ()
{
  s8 b[32];
  s_buf buf;
  BUF_TEST_INSPECT_CHARACTER(0, "'\\0'");
  BUF_TEST_INSPECT_CHARACTER(1, "'\\x01'");
  BUF_TEST_INSPECT_CHARACTER(2, "'\\x02'");
  BUF_TEST_INSPECT_CHARACTER('0', "'0'");
  BUF_TEST_INSPECT_CHARACTER('9', "'9'");
  BUF_TEST_INSPECT_CHARACTER('A', "'A'");
  BUF_TEST_INSPECT_CHARACTER('Z', "'Z'");
  BUF_TEST_INSPECT_CHARACTER('a', "'a'");
  BUF_TEST_INSPECT_CHARACTER('z', "'z'");
  BUF_TEST_INSPECT_CHARACTER(928, "'Π'");
  BUF_TEST_INSPECT_CHARACTER(0xFF, "'ÿ'");
  BUF_TEST_INSPECT_CHARACTER(42164, "'꒴'");
  BUF_TEST_INSPECT_CHARACTER(65856, "'𐅀'");
}

void buf_test_inspect_str_character ()
{
  s8 b[32];
  s_buf buf;
  BUF_TEST_INSPECT_STR_CHARACTER(0, "\\0");
  BUF_TEST_INSPECT_STR_CHARACTER(1, "\\x01");
  BUF_TEST_INSPECT_STR_CHARACTER(2, "\\x02");
  BUF_TEST_INSPECT_STR_CHARACTER('0', "0");
  BUF_TEST_INSPECT_STR_CHARACTER('9', "9");
  BUF_TEST_INSPECT_STR_CHARACTER('A', "A");
  BUF_TEST_INSPECT_STR_CHARACTER('Z', "Z");
  BUF_TEST_INSPECT_STR_CHARACTER('a', "a");
  BUF_TEST_INSPECT_STR_CHARACTER('z', "z");
  BUF_TEST_INSPECT_STR_CHARACTER(928, "Π");
  BUF_TEST_INSPECT_STR_CHARACTER(0xFF, "ÿ");
  BUF_TEST_INSPECT_STR_CHARACTER(42164, "꒴");
  BUF_TEST_INSPECT_STR_CHARACTER(65856, "𐅀");
}

void buf_test_inspect_str_character_size ()
{
  s8 b[32];
  s_buf buf;
  BUF_TEST_INSPECT_STR_CHARACTER_SIZE(0, 2);
  BUF_TEST_INSPECT_STR_CHARACTER_SIZE(1, 4);
  BUF_TEST_INSPECT_STR_CHARACTER_SIZE(2, 4);
  BUF_TEST_INSPECT_STR_CHARACTER_SIZE('0', 1);
  BUF_TEST_INSPECT_STR_CHARACTER_SIZE('9', 1);
  BUF_TEST_INSPECT_STR_CHARACTER_SIZE('A', 1);
  BUF_TEST_INSPECT_STR_CHARACTER_SIZE('Z', 1);
  BUF_TEST_INSPECT_STR_CHARACTER_SIZE('a', 1);
  BUF_TEST_INSPECT_STR_CHARACTER_SIZE('z', 1);
  BUF_TEST_INSPECT_STR_CHARACTER_SIZE(928, 2);
  BUF_TEST_INSPECT_STR_CHARACTER_SIZE(0xFF, 2);
  BUF_TEST_INSPECT_STR_CHARACTER_SIZE(42164, 3);
  BUF_TEST_INSPECT_STR_CHARACTER_SIZE(65856, 4);
}