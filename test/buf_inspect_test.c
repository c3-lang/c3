/* c3
 * Copyright 2022 Thomas de Grivel <thoxdg@gmail.com>
 */
#include <stdlib.h>
#include <string.h>
#include "../libc3/buf.h"
#include "../libc3/str.h"
#include "test.h"

#define BUF_INSPECT_TEST_CHARACTER(test, result)                       \
  do {                                                                 \
    test_context("buf_inspect_character(" # test ") -> " # result);    \
    buf_init(&buf, false, sizeof(b), b);                               \
    TEST_EQ(buf_inspect_character(&buf, test), strlen(result));        \
    TEST_STRNCMP(buf.ptr.ps8, result, buf.wpos);                       \
  } while (0)

#define BUF_INSPECT_TEST_STR_CHARACTER(test, result)                   \
  do {                                                                 \
    test_context("buf_inspect_str_character(" # test ") -> "           \
                 # result);                                            \
    buf_init(&buf, false, sizeof(b), b);                               \
    TEST_EQ(buf_inspect_str_character(&buf, test), strlen(result));    \
    TEST_STRNCMP(buf.ptr.ps8, result, buf.wpos);                       \
  } while (0)

#define BUF_INSPECT_TEST_STR_CHARACTER_SIZE(test, result)              \
  do {                                                                 \
    test_context("buf_inspect_str_character_size("                     \
                 # test ") -> " # result);                             \
    buf_init(&buf, false, sizeof(b), b);                               \
    TEST_EQ(buf_inspect_str_character_size(test), result);             \
  } while (0)

void buf_inspect_test_character ();
void buf_inspect_test_str_character ();
void buf_inspect_test_str_character_size ();

void buf_inspect_test ()
{
  buf_inspect_test_str_character_size();
  buf_inspect_test_str_character();
  buf_inspect_test_character();
}

void buf_inspect_test_character ()
{
  s8 b[32];
  s_buf buf;
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
  s8 b[32];
  s_buf buf;
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
  s8 b[32];
  s_buf buf;
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
