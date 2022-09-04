/* c3
 * Copyright 2022 Thomas de Grivel <thoxdg@gmail.com>
 */
#include <stdlib.h>
#include <string.h>
#include "../libc3/buf.h"
#include "../libc3/buf_parse.h"
#include "../libc3/str.h"
#include "test.h"

#define BUF_PARSE_TEST_CHARACTER(test, result)                         \
  do {                                                                 \
    s_buf buf;                                                         \
    character c;                                                       \
    test_context("buf_parse_character(" # test ") -> " # result);      \
    buf_init_1(&buf, (test));                                          \
    TEST_EQ(buf_parse_character(&buf, &c), strlen(test));              \
    TEST_EQ(buf.wpos, strlen(test));                                   \
    TEST_EQ(c, result);                                                \
  } while (0)

#define BUF_PARSE_TEST_STR(test, result)                               \
  do {                                                                 \
    s_buf buf;                                                         \
    test_context("buf_parse_str(" # test ") -> " # result);            \
    buf_init(&buf, false, sizeof(b), b);                               \
    TEST_EQ(buf_inspect_str(&buf, test), strlen(result));              \
  } while (0)

void buf_parse_test_character ();
void buf_parse_test_ident ();
void buf_parse_test_str ();
void buf_parse_test_sym ();

void buf_parse_test ()
{
  buf_parse_test_character();
  buf_parse_test_ident();
  buf_parse_test_str();
  buf_parse_test_sym();
}

void buf_parse_test_character ()
{
  BUF_PARSE_TEST_CHARACTER("'\\0'", 0);
  BUF_PARSE_TEST_CHARACTER("'\\x01'", 1);
  BUF_PARSE_TEST_CHARACTER("'\\x02'", 2);
  BUF_PARSE_TEST_CHARACTER("'0'", '0');
  BUF_PARSE_TEST_CHARACTER("'9'", '9');
  BUF_PARSE_TEST_CHARACTER("'A'", 'A');
  BUF_PARSE_TEST_CHARACTER("'Z'", 'Z');
  BUF_PARSE_TEST_CHARACTER("'a'", 'a');
  BUF_PARSE_TEST_CHARACTER("'z'", 'z');
  BUF_PARSE_TEST_CHARACTER("'Π'", 928);
  BUF_PARSE_TEST_CHARACTER("'ÿ'", 0xFF);
  BUF_PARSE_TEST_CHARACTER("'꒴'", 42164);
  BUF_PARSE_TEST_CHARACTER("'𐅀'", 65856);
}

void buf_parse_test_ident ()
{
}

void buf_parse_test_str ()
{
}

void buf_parse_test_sym ()
{
}
