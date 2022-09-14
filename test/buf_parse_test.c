/* c3
 * Copyright 2022 Thomas de Grivel <thoxdg@gmail.com>
 */
#include <stdlib.h>
#include <string.h>
#include "../libc3/buf.h"
#include "../libc3/buf_parse.h"
#include "../libc3/str.h"
#include "../libc3/integer.h"
#include "test.h"

#define BUF_PARSE_TEST_BOOL(test, result)                              \
  do {                                                                 \
    s_buf buf;                                                         \
    bool dest = -1;                                                    \
    test_context("buf_parse_bool(" # test ") -> " # result);           \
    buf_init_1(&buf, (test));                                          \
    if ((result) != -1) {                                              \
      TEST_EQ(buf_parse_bool(&buf, &dest), strlen(test));              \
      TEST_EQ(buf.rpos, strlen(test));                                 \
    }                                                                  \
    else {                                                             \
      TEST_EQ(buf_parse_bool(&buf, &dest), 0);                         \
      TEST_EQ(buf.rpos, 0);                                            \
    }                                                                  \
    TEST_EQ(dest, (result));                                           \
    buf_clean(&buf);                                                   \
  } while (0)

#define BUF_PARSE_TEST_CHARACTER(test, result)                         \
  do {                                                                 \
    s_buf buf;                                                         \
    character dest = -1;                                               \
    test_context("buf_parse_character(" # test ") -> " # result);      \
    buf_init_1(&buf, (test));                                          \
    TEST_EQ(buf_parse_character(&buf, &dest), strlen(test));           \
    TEST_EQ(buf.wpos, strlen(test));                                   \
    TEST_EQ(dest, (result));                                           \
    buf_clean(&buf);                                                   \
  } while (0)

#define BUF_PARSE_TEST_DIGIT_HEX(test, result)                         \
  do {                                                                 \
    s_buf buf;                                                         \
    u8 dest = 0x80;                                                    \
    test_context("buf_parse_digit_hex(" # test ") -> " # result);      \
    buf_init_1(&buf, (test));                                          \
    TEST_EQ(buf_parse_digit_hex(&buf, &dest), 1);                      \
    TEST_EQ(buf.rpos, 1);                                              \
    TEST_EQ(dest, (result));                                           \
    buf_clean(&buf);                                                   \
  } while (0)

#define BUF_PARSE_TEST_DIGIT_OCT(test, result)                         \
  do {                                                                 \
    s_buf buf;                                                         \
    u8 dest = 0x80;                                                    \
    test_context("buf_parse_digit_oct(" # test ") -> " # result);      \
    buf_init_1(&buf, (test));                                          \
    TEST_EQ(buf_parse_digit_oct(&buf, &dest), 1);                      \
    TEST_EQ(buf.rpos, 1);                                              \
    TEST_EQ(dest, (result));                                           \
    buf_clean(&buf);                                                   \
  } while (0)

#define BUF_PARSE_TEST_DIGIT_BIN(test, result)                         \
  do {                                                                 \
    s_buf buf;                                                         \
    u8 dest = 0x80;                                                    \
    test_context("buf_parse_digit_bin(" # test ") -> " # result);      \
    buf_init_1(&buf, (test));                                          \
    TEST_EQ(buf_parse_digit_bin(&buf, &dest), 1);                      \
    TEST_EQ(buf.rpos, 1);                                              \
    TEST_EQ(dest, (result));                                           \
    buf_clean(&buf);                                                   \
  } while (0)

#define BUF_PARSE_TEST_DIGIT_DEC(test, result)                         \
  do {                                                                 \
    s_buf buf;                                                         \
    u8 dest = 0x80;                                                    \
    test_context("buf_parse_digit_dec(" # test ") -> " # result);      \
    buf_init_1(&buf, (test));                                          \
    TEST_EQ(buf_parse_digit_dec(&buf, &dest), 1);                      \
    TEST_EQ(buf.rpos, 1);                                              \
    TEST_EQ(dest, (result));                                           \
    buf_clean(&buf);                                                   \
  } while (0)

#define BUF_PARSE_TEST_IDENT(test, result)                             \
  do {                                                                 \
    s_buf buf;                                                         \
    s_ident dest = {0};                                                \
    test_context("buf_parse_ident(" # test ") -> " # result);          \
    buf_init_1(&buf, (test));                                          \
    TEST_EQ(buf_parse_ident(&buf, &dest), strlen(test));               \
    if (g_test_last_ok)                                                \
      TEST_EQ(dest.sym->str.size, strlen(result));                    \
    if (g_test_last_ok)                                                \
      TEST_STRNCMP(dest.sym->str.ptr.p, (result), dest.sym->str.size); \
    buf_clean(&buf);                                                   \
  } while (0)

#define BUF_PARSE_TEST_INTEGER_DEC(test, expected)                           \
do {                                                                   \
  s_buf buf;                                                           \
  s_integer s_tmp;                                                     \
  u64 u64_tmp;                                                         \
  test_context("buf_parse_integer_dec(" # test ") -> " # expected);        \
  buf_init_1(&buf, (test));                                            \
  integer_init(&s_tmp);                                                \
  TEST_EQ(buf_parse_integer_dec(&buf, &s_tmp), strlen(test));              \
  u64_tmp = mp_get_u64(&s_tmp.mp_int);                                 \
  TEST_EQ(u64_tmp, (expected));                                        \
  buf_clean(&buf);                                                     \
  mp_clear(&s_tmp.mp_int);                                             \
} while (0)

#define BUF_PARSE_TEST_INTEGER_HEX(test, expected) \
  do {                                             \
    s_buf buf;                                    \
    s_integer s_tmp;                              \
    u64 u64_tmp;                                  \
    test_context("buf_parse_integer_hex(" # test ") -> " # expected); \
    buf_init_1(&buf, (test));                     \
    integer_init(&s_tmp);                         \
    TEST_EQ(buf_parse_integer_hex(&buf, &s_tmp), strlen(test)); \
    u64_tmp = mp_get_u64(&s_tmp.mp_int);          \
    TEST_EQ(buf.wpos, strlen(test));              \
    TEST_EQ(u64_tmp, (expected));                 \
    buf_clean(&buf);                              \
    mp_clear(&s_tmp.mp_int);                      \
  } while (0)

#define BUF_PARSE_TEST_INTEGER_OCT(test, expected) \
  do {                                             \
    s_buf buf;                                    \
    s_integer s_tmp;                              \
    u64 u64_tmp;                                  \
    test_context("buf_parse_integer_oct(" # test ") -> " # expected); \
    buf_init_1(&buf, (test));                     \
    integer_init(&s_tmp);                         \
    TEST_EQ(buf_parse_integer_oct(&buf, &s_tmp), strlen(test)); \
    u64_tmp = mp_get_u64(&s_tmp.mp_int);          \
    TEST_EQ(buf.wpos, strlen(test));              \
    TEST_EQ(u64_tmp, (expected));                 \
    buf_clean(&buf);                              \
    mp_clear(&s_tmp.mp_int);                      \
  } while (0)

#define BUF_PARSE_TEST_NOT_INTEGER_HEX(test) \
  do {                                       \
    s_buf buf;                              \
    s_integer s_tmp;                        \
    test_context("buf_parse_integer_hex(" # test ") -> 0"); \
    buf_init_1(&buf, (test));               \
    integer_init(&s_tmp);                   \
    TEST_EQ(buf_parse_integer_hex(&buf, &s_tmp), 0); \
    TEST_EQ(buf.rpos, 0);                   \
    buf_clean(&buf);                        \
    mp_clear(&s_tmp.mp_int);                \
  } while (0)

#define BUF_PARSE_TEST_NOT_CHARACTER(test)                             \
  do {                                                                 \
    s_buf buf;                                                         \
    character dest = -1;                                               \
    test_context("buf_parse_character(" # test ") -> 0");              \
    buf_init_1(&buf, (test));                                          \
    TEST_EQ(buf_parse_character(&buf, &dest), 0);                      \
    TEST_EQ(buf.rpos, 0);                                              \
    TEST_EQ(dest, -1);                                                 \
    buf_clean(&buf);                                                   \
  } while (0)

#define BUF_PARSE_TEST_NOT_DIGIT_BIN(test, expected)                   \
  do {                                                                 \
    s_buf buf;                                                         \
    u8 dest = 0;                                                       \
    test_context("buf_parse_digit_bin(" # test ") -> " # expected);    \
    buf_init_1(&buf, (test));                                          \
    TEST_EQ(buf_parse_digit_bin(&buf, &dest), 0);                      \
    TEST_EQ(buf.rpos, 0);                                              \
    TEST_EQ(dest, 0);                                                  \
    buf_clean(&buf);                                                   \
  } while (0)

#define BUF_PARSE_TEST_NOT_DIGIT_DEC(test, expected)                   \
  do {                                                                 \
    s_buf buf;                                                         \
    u8 dest = 0;                                                       \
    test_context("buf_parse_digit_dec(" # test ") -> 0");              \
    buf_init_1(&buf, (test));                                          \
    TEST_EQ(buf_parse_digit_dec(&buf, &dest), (expected));             \
    TEST_EQ(buf.rpos, 0);                                              \
    TEST_EQ(dest, 0);                                                  \
    buf_clean(&buf);                                                   \
  } while (0)

#define BUF_PARSE_TEST_NOT_DIGIT_HEX(test, expected)                   \
  do {                                                                 \
    s_buf buf;                                                         \
    u8 dest = 0;                                                       \
    test_context("buf_parse_digit_hex(" # test ") -> 0");              \
    buf_init_1(&buf, (test));                                          \
    TEST_EQ(buf_parse_digit_hex(&buf, &dest), (expected));             \
    TEST_EQ(buf.rpos, 0);                                              \
    TEST_EQ(dest, 0);                                                  \
    buf_clean(&buf);                                                   \
  } while (0)

#define BUF_PARSE_TEST_NOT_DIGIT_OCT(test, expected)                   \
  do {                                                                 \
    s_buf buf;                                                         \
    u8 dest = 0;                                                       \
    test_context("buf_parse_digit_oct(" # test ") -> 0");              \
    buf_init_1(&buf, (test));                                          \
    TEST_EQ(buf_parse_digit_oct(&buf, &dest), (expected));             \
    TEST_EQ(buf.rpos, 0);                                              \
    TEST_EQ(dest, 0);                                                  \
    buf_clean(&buf);                                                   \
  } while (0)

#define BUF_PARSE_TEST_NOT_IDENT(test)                                 \
  do {                                                                 \
    s_buf buf;                                                         \
    s_ident dest = {0};                                                \
    test_context("buf_parse_ident(" # test ") -> 0");                  \
    buf_init_1(&buf, (test));                                          \
    TEST_EQ(buf_parse_ident(&buf, &dest), 0);                          \
    TEST_EQ(buf.rpos, 0);                                              \
    buf_clean(&buf);                                                   \
  } while (0)

#define BUF_PARSE_TEST_NOT_STR(test)                                   \
  do {                                                                 \
    s_buf buf;                                                         \
    s_str dest = {0};                                                  \
    test_context("buf_parse_str(" # test ") -> 0");                    \
    buf_init_1(&buf, (test));                                          \
    TEST_EQ(buf_parse_str(&buf, &dest), 0);                            \
    TEST_EQ(buf.rpos, 0);                                              \
    buf_clean(&buf);                                                   \
  } while (0)

#define BUF_PARSE_TEST_NOT_INTEGER_DEC(test, expected)                 \
do {                                                                   \
  s_buf buf;                                                           \
  s_integer s_tmp;                                                     \
  u64 u64_tmp;                                                         \
  test_context("buf_parse_integer_dec(" # test ") -> " # expected);    \
  buf_init_1(&buf, (test));                                            \
  integer_init(&s_tmp);                                                \
  TEST_EQ(buf_parse_integer_dec(&buf, &s_tmp), 0);                     \
  u64_tmp = mp_get_u64(&s_tmp.mp_int);                                 \
  TEST_EQ(buf.rpos, 0);                                                \
  TEST_EQ(u64_tmp, (expected));                                        \
  buf_clean(&buf);                                                     \
  mp_clear(&s_tmp.mp_int);                                             \
} while (0)

#define BUF_PARSE_TEST_NOT_STR_U8(test)                                \
  do {                                                                 \
    s_buf buf;                                                         \
    u8 dest = 0;                                                       \
    test_context("buf_parse_str_u8(" # test ") -> 0");                 \
    buf_init_1(&buf, (test));                                          \
    TEST_EQ(buf_parse_str_u8(&buf, &dest), 0);                         \
    TEST_EQ(buf.rpos, 0);                                              \
    TEST_EQ(dest, 0);                                                  \
    buf_clean(&buf);                                                   \
  } while (0)

#define BUF_PARSE_TEST_NOT_SYM(test)                                   \
  do {                                                                 \
    s_buf buf;                                                         \
    const s_sym *dest = NULL;                                          \
    test_context("buf_parse_sym(" # test ") -> 0");                    \
    buf_init_1(&buf, (test));                                          \
    TEST_EQ(buf_parse_sym(&buf, &dest), 0);                            \
    TEST_EQ(buf.rpos, 0);                                              \
    buf_clean(&buf);                                                   \
  } while (0)

#define BUF_PARSE_TEST_STR(test, result)                               \
  do {                                                                 \
    s_buf buf;                                                         \
    s_str dest = {0};                                                  \
    test_context("buf_parse_str(" # test ") -> " # result);            \
    buf_init_1(&buf, (test));                                          \
    TEST_EQ(buf_parse_str(&buf, &dest), strlen(test));                 \
    if (g_test_last_ok)                                                \
      TEST_EQ(dest.size, strlen(result));                              \
    if (g_test_last_ok)                                                \
      TEST_STRNCMP(dest.ptr.p, (result), dest.size);                   \
    buf_clean(&buf);                                                   \
    str_clean(&dest);                                                  \
  } while (0)

#define BUF_PARSE_TEST_STR_CHARACTER(test, result)                     \
  do {                                                                 \
    s_buf buf;                                                         \
    character dest = -1;                                               \
    test_context("buf_parse_str_character(" # test ") -> " # result);  \
    buf_init_1(&buf, (test));                                          \
    TEST_EQ(buf_parse_str_character(&buf, &dest), strlen(test));       \
    TEST_EQ(buf.rpos, strlen(test));                                   \
    TEST_EQ(dest, (result));                                           \
    buf_clean(&buf);                                                   \
  } while (0)

#define BUF_PARSE_TEST_STR_N(test, n, result)                          \
  do {                                                                 \
    s_buf buf;                                                         \
    s_str dest;                                                        \
    test_context("buf_parse_str(" # test ") -> " # result);            \
    buf_init_1(&buf, (test));                                          \
    TEST_EQ(buf_parse_str(&buf, &dest), strlen(test));                 \
    if (g_test_last_ok)                                                \
      TEST_EQ(dest.size, n);                                           \
    if (g_test_last_ok)                                                \
      TEST_EQ(memcmp(dest.ptr.p, result, n), 0);                       \
    buf_clean(&buf);                                                   \
    str_clean(&dest);						       \
  } while (0)

#define BUF_PARSE_TEST_STR_U8(test, size, result)                      \
  do {                                                                 \
    s_buf buf;                                                         \
    u8 dest = 0x80;                                                    \
    test_context("buf_parse_str_u8(" # test ") -> " # result);         \
    buf_init_1(&buf, (test));                                          \
    TEST_EQ(buf_parse_str_u8(&buf, &dest), (size));                    \
    TEST_EQ(buf.rpos, (size));                                         \
    TEST_EQ(dest, (result));                                           \
    buf_clean(&buf);                                                   \
  } while (0)

#define BUF_PARSE_TEST_SYM(test, result)                               \
  do {                                                                 \
    s_buf buf;                                                         \
    const s_sym *dest = NULL;                                          \
    test_context("buf_parse_sym(" # test ") -> " # result);            \
    buf_init_1(&buf, (test));                                          \
    TEST_EQ(buf_parse_sym(&buf, &dest), strlen(test));                 \
    if (g_test_last_ok)                                                \
      TEST_ASSERT(dest);                                               \
    if (g_test_last_ok)                                                \
      TEST_EQ(dest->str.size, strlen(result));                         \
    if (g_test_last_ok)                                                \
      TEST_STRNCMP(dest->str.ptr.p, (result), dest->str.size);         \
    buf_clean(&buf);                                                   \
  } while (0)

#define BUF_PARSE_TEST_F32(test, expected)                             \
  do {                                                                 \
    s_buf buf;                                                         \
    f32 f32_tmp;                                                       \
    test_context("buf_parse_f32(" # test ") -> " # expected);          \
    buf_init_1(&buf, (test));                                          \
    TEST_EQ(buf_parse_f32(&buf, &f32_tmp), strlen(test));              \
    TEST_EQ(f32_tmp, (expected));                                      \
    buf_clean(&buf);                                                   \
  } while (0)

void buf_parse_test_bool ();
void buf_parse_test_character ();
void buf_parse_test_digit_bin ();
void buf_parse_test_digit_hex ();
void buf_parse_test_digit_oct ();
void buf_parse_test_digit_dec ();
void buf_parse_test_integer_dec();
void buf_parse_test_integer_hex();
void buf_parse_test_integer_oct();
void buf_parse_test_integer_bin();
void buf_parse_test_ident ();
void buf_parse_test_str ();
void buf_parse_test_str_character ();
void buf_parse_test_str_u8 ();
void buf_parse_test_sym ();
void buf_parse_test_f32();


void buf_parse_test ()
{
  buf_parse_test_bool();
  buf_parse_test_digit_hex();
  buf_parse_test_digit_oct();
  buf_parse_test_digit_dec();
  buf_parse_test_str_character();
  buf_parse_test_str_u8();
  buf_parse_test_character();
  buf_parse_test_integer_dec();
  buf_parse_test_integer_hex();
  buf_parse_test_integer_oct();
  buf_parse_test_str();
  buf_parse_test_sym();
  buf_parse_test_ident();
  buf_parse_test_f32();
}

void buf_parse_test_bool ()
{
  BUF_PARSE_TEST_BOOL("0", -1);
  BUF_PARSE_TEST_BOOL("1", -1);
  BUF_PARSE_TEST_BOOL("a", -1);
  BUF_PARSE_TEST_BOOL("T", -1);
  BUF_PARSE_TEST_BOOL("NIL", -1);
  BUF_PARSE_TEST_BOOL("N", -1);
  BUF_PARSE_TEST_BOOL("Y", -1);
  BUF_PARSE_TEST_BOOL("t", -1);
  BUF_PARSE_TEST_BOOL("nil", -1);
  BUF_PARSE_TEST_BOOL("n", -1);
  BUF_PARSE_TEST_BOOL("y", -1);
  BUF_PARSE_TEST_BOOL("false", 0);
  BUF_PARSE_TEST_BOOL("true", 1);
}

void buf_parse_test_character ()
{
  BUF_PARSE_TEST_NOT_CHARACTER("\x01");
  BUF_PARSE_TEST_NOT_CHARACTER("\xFF");
  BUF_PARSE_TEST_NOT_CHARACTER("0");
  BUF_PARSE_TEST_NOT_CHARACTER("9");
  BUF_PARSE_TEST_NOT_CHARACTER("A");
  BUF_PARSE_TEST_NOT_CHARACTER("Z");
  BUF_PARSE_TEST_NOT_CHARACTER("a");
  BUF_PARSE_TEST_NOT_CHARACTER("z");
  BUF_PARSE_TEST_NOT_CHARACTER("'");
  BUF_PARSE_TEST_NOT_CHARACTER("'\\");
  BUF_PARSE_TEST_NOT_CHARACTER("'\\'");
  BUF_PARSE_TEST_NOT_CHARACTER("'\\x");
  BUF_PARSE_TEST_NOT_CHARACTER("'\\x'");
  BUF_PARSE_TEST_NOT_CHARACTER("'\\x0'");
  BUF_PARSE_TEST_NOT_CHARACTER("'\\x00'");
  BUF_PARSE_TEST_NOT_CHARACTER("'\\x61'");
  BUF_PARSE_TEST_NOT_CHARACTER("'\\00'");
  BUF_PARSE_TEST_NOT_CHARACTER("'\\0X'");
  BUF_PARSE_TEST_NOT_CHARACTER("'\\0x'");
  BUF_PARSE_TEST_NOT_CHARACTER("'\\nx'");
  BUF_PARSE_TEST_NOT_CHARACTER("'\\rx'");
  BUF_PARSE_TEST_NOT_CHARACTER("'\\tx'");
  BUF_PARSE_TEST_NOT_CHARACTER("'\\vx'");
  BUF_PARSE_TEST_NOT_CHARACTER("'\\sx'");
  BUF_PARSE_TEST_NOT_CHARACTER("'0x'");
  BUF_PARSE_TEST_NOT_CHARACTER("'9x'");
  BUF_PARSE_TEST_NOT_CHARACTER("'Ax'");
  BUF_PARSE_TEST_NOT_CHARACTER("'Zx'");
  BUF_PARSE_TEST_NOT_CHARACTER("'ax'");
  BUF_PARSE_TEST_NOT_CHARACTER("'zx'");
  BUF_PARSE_TEST_NOT_CHARACTER("'Πx'");
  BUF_PARSE_TEST_NOT_CHARACTER("'ÿx'");
  BUF_PARSE_TEST_NOT_CHARACTER("'꒴x'");
  BUF_PARSE_TEST_NOT_CHARACTER("'𐅀x'");
  BUF_PARSE_TEST_NOT_CHARACTER("'x\\0'");
  BUF_PARSE_TEST_NOT_CHARACTER("'x\\n'");
  BUF_PARSE_TEST_NOT_CHARACTER("'x\\r'");
  BUF_PARSE_TEST_NOT_CHARACTER("'x\\t'");
  BUF_PARSE_TEST_NOT_CHARACTER("'x\\v'");
  BUF_PARSE_TEST_NOT_CHARACTER("'x\\s'");
  BUF_PARSE_TEST_NOT_CHARACTER("'x0'");
  BUF_PARSE_TEST_NOT_CHARACTER("'x9'");
  BUF_PARSE_TEST_NOT_CHARACTER("'xA'");
  BUF_PARSE_TEST_NOT_CHARACTER("'xZ'");
  BUF_PARSE_TEST_NOT_CHARACTER("'xa'");
  BUF_PARSE_TEST_NOT_CHARACTER("'xz'");
  BUF_PARSE_TEST_NOT_CHARACTER("'xΠ'");
  BUF_PARSE_TEST_NOT_CHARACTER("'xÿ'");
  BUF_PARSE_TEST_NOT_CHARACTER("'x꒴'");
  BUF_PARSE_TEST_NOT_CHARACTER("'x𐅀'");
  BUF_PARSE_TEST_CHARACTER("'\\0'", 0);
  BUF_PARSE_TEST_CHARACTER("'\\n'", '\n');
  BUF_PARSE_TEST_CHARACTER("'\\r'", '\r');
  BUF_PARSE_TEST_CHARACTER("'\\t'", '\t');
  BUF_PARSE_TEST_CHARACTER("'\\v'", '\v');
  BUF_PARSE_TEST_CHARACTER("'\\s'", ' ');
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

void buf_parse_test_digit_bin ()
{
  BUF_PARSE_TEST_NOT_DIGIT_BIN("\x01", 0);
  BUF_PARSE_TEST_NOT_DIGIT_BIN("\x02", 0);
  BUF_PARSE_TEST_NOT_DIGIT_BIN("\xF0", 0);
  BUF_PARSE_TEST_NOT_DIGIT_BIN("\xFF", -1);
  BUF_PARSE_TEST_NOT_DIGIT_BIN(".", 0);
  BUF_PARSE_TEST_NOT_DIGIT_BIN(":", 0);
  BUF_PARSE_TEST_NOT_DIGIT_BIN(",", 0);
  BUF_PARSE_TEST_NOT_DIGIT_BIN(";", 0);
  BUF_PARSE_TEST_NOT_DIGIT_BIN("G", 0);
  BUF_PARSE_TEST_NOT_DIGIT_BIN("H", 0);
  BUF_PARSE_TEST_NOT_DIGIT_BIN("g", 0);
  BUF_PARSE_TEST_NOT_DIGIT_BIN("h", 0);
  BUF_PARSE_TEST_DIGIT_BIN("0", 0);
  BUF_PARSE_TEST_DIGIT_BIN("1", 1);
}

void buf_parse_test_digit_hex ()
{
  BUF_PARSE_TEST_NOT_DIGIT_HEX("\x01", 0);
  BUF_PARSE_TEST_NOT_DIGIT_HEX("\x02", 0);
  BUF_PARSE_TEST_NOT_DIGIT_HEX("\xF0", 0);
  BUF_PARSE_TEST_NOT_DIGIT_HEX("\xFF", -1);
  BUF_PARSE_TEST_NOT_DIGIT_HEX(".", 0);
  BUF_PARSE_TEST_NOT_DIGIT_HEX(":", 0);
  BUF_PARSE_TEST_NOT_DIGIT_HEX(",", 0);
  BUF_PARSE_TEST_NOT_DIGIT_HEX(";", 0);
  BUF_PARSE_TEST_NOT_DIGIT_HEX("G", 0);
  BUF_PARSE_TEST_NOT_DIGIT_HEX("H", 0);
  BUF_PARSE_TEST_NOT_DIGIT_HEX("g", 0);
  BUF_PARSE_TEST_NOT_DIGIT_HEX("h", 0);
  BUF_PARSE_TEST_DIGIT_HEX("0", 0);
  BUF_PARSE_TEST_DIGIT_HEX("9", 9);
  BUF_PARSE_TEST_DIGIT_HEX("A", 0x0A);
  BUF_PARSE_TEST_DIGIT_HEX("F", 0x0F);
  BUF_PARSE_TEST_DIGIT_HEX("a", 0x0A);
  BUF_PARSE_TEST_DIGIT_HEX("f", 0x0F);
}

void buf_parse_test_digit_oct ()
{
  BUF_PARSE_TEST_NOT_DIGIT_OCT("\x01", 0);
  BUF_PARSE_TEST_NOT_DIGIT_OCT("\x02", 0);
  BUF_PARSE_TEST_NOT_DIGIT_OCT("\xF0", 0);
  BUF_PARSE_TEST_NOT_DIGIT_OCT("\xFF", -1);
  BUF_PARSE_TEST_NOT_DIGIT_OCT(".", 0);
  BUF_PARSE_TEST_NOT_DIGIT_OCT(":", 0);
  BUF_PARSE_TEST_NOT_DIGIT_OCT(",", 0);
  BUF_PARSE_TEST_NOT_DIGIT_OCT(";", 0);
  BUF_PARSE_TEST_NOT_DIGIT_OCT("G", 0);
  BUF_PARSE_TEST_NOT_DIGIT_OCT("H", 0);
  BUF_PARSE_TEST_NOT_DIGIT_OCT("g", 0);
  BUF_PARSE_TEST_NOT_DIGIT_OCT("h", 0);
  BUF_PARSE_TEST_DIGIT_OCT("0", 0);
  BUF_PARSE_TEST_DIGIT_OCT("7", 7);
}

void buf_parse_test_digit_dec ()
{
  BUF_PARSE_TEST_NOT_DIGIT_DEC("\x01", 0);
  BUF_PARSE_TEST_NOT_DIGIT_DEC("\x02", 0);
  BUF_PARSE_TEST_NOT_DIGIT_DEC("\xF0", 0);
  BUF_PARSE_TEST_NOT_DIGIT_DEC("\xFF", -1);
  BUF_PARSE_TEST_NOT_DIGIT_DEC(".", 0);
  BUF_PARSE_TEST_NOT_DIGIT_DEC(":", 0);
  BUF_PARSE_TEST_NOT_DIGIT_DEC(",", 0);
  BUF_PARSE_TEST_NOT_DIGIT_DEC(";", 0);
  BUF_PARSE_TEST_NOT_DIGIT_DEC("G", 0);
  BUF_PARSE_TEST_NOT_DIGIT_DEC("H", 0);
  BUF_PARSE_TEST_NOT_DIGIT_DEC("g", 0);
  BUF_PARSE_TEST_NOT_DIGIT_DEC("h", 0);
  BUF_PARSE_TEST_DIGIT_DEC("0", 0);
  BUF_PARSE_TEST_DIGIT_DEC("9", 9);
}

void buf_parse_test_ident ()
{
  BUF_PARSE_TEST_NOT_IDENT("0");
  BUF_PARSE_TEST_NOT_IDENT("9");
  BUF_PARSE_TEST_NOT_IDENT("A");
  BUF_PARSE_TEST_NOT_IDENT("Z");
  BUF_PARSE_TEST_NOT_IDENT("09AZ");
  BUF_PARSE_TEST_NOT_IDENT("Π");
  BUF_PARSE_TEST_IDENT("_\"0\"", "0");
  BUF_PARSE_TEST_IDENT("_\"9\"", "9");
  BUF_PARSE_TEST_IDENT("_\"A\"", "A");
  BUF_PARSE_TEST_IDENT("_\"Z\"", "Z");
  BUF_PARSE_TEST_IDENT("_\"a\"", "a");
  BUF_PARSE_TEST_IDENT("_\"z\"", "z");
  BUF_PARSE_TEST_IDENT("_\"09AZaz\"", "09AZaz");
  BUF_PARSE_TEST_IDENT("_", "_");
  BUF_PARSE_TEST_IDENT("__", "__");
  BUF_PARSE_TEST_IDENT("_0", "_0");
  BUF_PARSE_TEST_IDENT("_9", "_9");
  BUF_PARSE_TEST_IDENT("_A", "_A");
  BUF_PARSE_TEST_IDENT("_Z", "_Z");
  BUF_PARSE_TEST_IDENT("_a", "_a");
  BUF_PARSE_TEST_IDENT("_z", "_z");
  BUF_PARSE_TEST_IDENT("_Π", "_Π");
  BUF_PARSE_TEST_IDENT("_az09AZ", "_az09AZ");
  BUF_PARSE_TEST_IDENT("a", "a");
  BUF_PARSE_TEST_IDENT("z", "z");
  BUF_PARSE_TEST_IDENT("+", "+");
  BUF_PARSE_TEST_IDENT("-", "-");
  BUF_PARSE_TEST_IDENT("*", "*");
  BUF_PARSE_TEST_IDENT("/", "/");
  BUF_PARSE_TEST_IDENT("=", "=");
  BUF_PARSE_TEST_IDENT("az09AZ", "az09AZ");
  BUF_PARSE_TEST_IDENT("a+b", "a+b");
  BUF_PARSE_TEST_IDENT("+a", "+a");
  BUF_PARSE_TEST_IDENT("/a", "/a");
  BUF_PARSE_TEST_IDENT("=a", "=a");
  BUF_PARSE_TEST_IDENT("a+", "a+");
  BUF_PARSE_TEST_IDENT("a-", "a-");
  BUF_PARSE_TEST_IDENT("a*", "a*");
  BUF_PARSE_TEST_IDENT("a/", "a/");
  BUF_PARSE_TEST_IDENT("a=", "a=");
  BUF_PARSE_TEST_IDENT("é", "é");
  BUF_PARSE_TEST_IDENT("ÿ", "ÿ");
  BUF_PARSE_TEST_IDENT("π", "π");
  BUF_PARSE_TEST_IDENT("꒴", "꒴");
  BUF_PARSE_TEST_IDENT("𐅀", "𐅀");
  BUF_PARSE_TEST_IDENT("🎳", "🎳");
  BUF_PARSE_TEST_IDENT("😄", "😄");
  BUF_PARSE_TEST_IDENT("🟣", "🟣");
  BUF_PARSE_TEST_IDENT("🤩", "🤩");
}

void buf_parse_test_integer_dec()
{
  BUF_PARSE_TEST_INTEGER_DEC("9", 9);
  BUF_PARSE_TEST_INTEGER_DEC("256", 256);
  BUF_PARSE_TEST_INTEGER_DEC("100000000000000000", 100000000000000000);
  BUF_PARSE_TEST_INTEGER_DEC("0", 0);
  BUF_PARSE_TEST_INTEGER_DEC("-256", -256);
  BUF_PARSE_TEST_INTEGER_DEC("-100000000000000000", -100000000000000000);
  BUF_PARSE_TEST_NOT_INTEGER_DEC("A", 0);
  BUF_PARSE_TEST_NOT_INTEGER_DEC("STR", 0);
}

void buf_parse_test_integer_hex()
{
  BUF_PARSE_TEST_INTEGER_HEX("9", 9);
  BUF_PARSE_TEST_INTEGER_HEX("256", 0x256);
  BUF_PARSE_TEST_INTEGER_HEX("0", 0);
  BUF_PARSE_TEST_INTEGER_HEX("-256", -0x256);
  BUF_PARSE_TEST_INTEGER_HEX("-E8D4A51000", -1000000000000);
  BUF_PARSE_TEST_INTEGER_HEX("A", 10);
  BUF_PARSE_TEST_NOT_INTEGER_HEX("STR");
}

void buf_parse_test_integer_oct()
{
  BUF_PARSE_TEST_INTEGER_OCT("777", 511);
}

void buf_parse_test_str ()
{
  BUF_PARSE_TEST_NOT_STR("");
  BUF_PARSE_TEST_NOT_STR("\x00");
  BUF_PARSE_TEST_NOT_STR("\x01");
  BUF_PARSE_TEST_NOT_STR("\xFF");
  BUF_PARSE_TEST_NOT_STR("\x00\x01\xFF");
  BUF_PARSE_TEST_NOT_STR("0");
  BUF_PARSE_TEST_NOT_STR("9");
  BUF_PARSE_TEST_NOT_STR("A");
  BUF_PARSE_TEST_NOT_STR("Z");
  BUF_PARSE_TEST_NOT_STR("a");
  BUF_PARSE_TEST_NOT_STR("z");
  BUF_PARSE_TEST_NOT_STR("09AZaz");
  BUF_PARSE_TEST_NOT_STR("\"");
  BUF_PARSE_TEST_NOT_STR("\"0");
  BUF_PARSE_TEST_NOT_STR("\"9");
  BUF_PARSE_TEST_NOT_STR("\"09AZaz");
  BUF_PARSE_TEST_STR_N("\"\\0\"", 1, "\0");
  BUF_PARSE_TEST_STR_N("\"\\0\\0\"", 2, "\0\0");
  BUF_PARSE_TEST_STR_N("\"\\0\\0\\0\"", 3, "\0\0\0");
  BUF_PARSE_TEST_STR("\"\"", "");
  BUF_PARSE_TEST_STR("\"\\n\"", "\n");
  BUF_PARSE_TEST_STR("\"\\r\"", "\r");
  BUF_PARSE_TEST_STR("\"\\t\"", "\t");
  BUF_PARSE_TEST_STR("\"\\v\"", "\v");
  BUF_PARSE_TEST_STR("\"\\s\"", " ");
  BUF_PARSE_TEST_STR("\"\\x01\"", "\x01");
  BUF_PARSE_TEST_STR("\"\\x02\"", "\x02");
  BUF_PARSE_TEST_STR("\"\\xFF\"", "\xFF");
  BUF_PARSE_TEST_STR("\"0\"", "0");
  BUF_PARSE_TEST_STR("\"9\"", "9");
  BUF_PARSE_TEST_STR("\"A\"", "A");
  BUF_PARSE_TEST_STR("\"Z\"", "Z");
  BUF_PARSE_TEST_STR("\"a\"", "a");
  BUF_PARSE_TEST_STR("\"z\"", "z");
  BUF_PARSE_TEST_STR("\"Π\"", "Π");
  BUF_PARSE_TEST_STR("\"ÿ\"", "ÿ");
  BUF_PARSE_TEST_STR("\"꒴\"", "꒴");
  BUF_PARSE_TEST_STR("\"𐅀\"", "𐅀");
  BUF_PARSE_TEST_STR("\"●\"", "●");
  BUF_PARSE_TEST_STR("\"🎳\"", "🎳");
  BUF_PARSE_TEST_STR("\"😄\"", "😄");
  BUF_PARSE_TEST_STR("\"🟣\"", "🟣");
  BUF_PARSE_TEST_STR("\"🤩\"", "🤩");
  BUF_PARSE_TEST_STR("\"ꇤ\"", "ꇤ");
}

void buf_parse_test_str_character ()
{
  BUF_PARSE_TEST_STR_CHARACTER("0", '0');
  BUF_PARSE_TEST_STR_CHARACTER("9", '9');
  BUF_PARSE_TEST_STR_CHARACTER("A", 'A');
  BUF_PARSE_TEST_STR_CHARACTER("Z", 'Z');
  BUF_PARSE_TEST_STR_CHARACTER("\\0", 0);
  BUF_PARSE_TEST_STR_CHARACTER("\\n", '\n');
  BUF_PARSE_TEST_STR_CHARACTER("\\r", '\r');
  BUF_PARSE_TEST_STR_CHARACTER("\\t", '\t');
  BUF_PARSE_TEST_STR_CHARACTER("\\v", '\v');
  BUF_PARSE_TEST_STR_CHARACTER("\\s", ' ');
  BUF_PARSE_TEST_STR_CHARACTER("\\U+0000", 0);
  BUF_PARSE_TEST_STR_CHARACTER("\\U+1F595", 128405);
  BUF_PARSE_TEST_STR_CHARACTER("\\U+FF", 0xFF);
  BUF_PARSE_TEST_STR_CHARACTER("a", 'a');
  BUF_PARSE_TEST_STR_CHARACTER("z", 'z');
  BUF_PARSE_TEST_STR_CHARACTER("ÿ", 0xFF);
  BUF_PARSE_TEST_STR_CHARACTER("Π", 928);
  BUF_PARSE_TEST_STR_CHARACTER("꒴", 42164);
  BUF_PARSE_TEST_STR_CHARACTER("𐅀", 65856);
  BUF_PARSE_TEST_STR_CHARACTER("🎳", 127923);
  BUF_PARSE_TEST_STR_CHARACTER("😄", 128516);
  BUF_PARSE_TEST_STR_CHARACTER("🟣", 128995);
  BUF_PARSE_TEST_STR_CHARACTER("🤩", 129321);
}

void buf_parse_test_str_u8 ()
{
  BUF_PARSE_TEST_NOT_STR_U8("");
  BUF_PARSE_TEST_STR_U8("\x01", 1, 0x01);
  BUF_PARSE_TEST_STR_U8("0", 1, '0');
  BUF_PARSE_TEST_STR_U8("00", 1, '0');
  BUF_PARSE_TEST_STR_U8("000", 1, '0');
  BUF_PARSE_TEST_STR_U8("\\", 1, '\\');
  BUF_PARSE_TEST_STR_U8("\\x", 1, '\\');
  BUF_PARSE_TEST_STR_U8("\\x0", 1, '\\');
  BUF_PARSE_TEST_STR_U8("x", 1, 'x');
  BUF_PARSE_TEST_STR_U8("x0", 1, 'x');
  BUF_PARSE_TEST_STR_U8("x00", 1, 'x');
  BUF_PARSE_TEST_STR_U8("\\0", 1, '\\');
  BUF_PARSE_TEST_STR_U8("\\00", 1, '\\');
  BUF_PARSE_TEST_STR_U8("\\000", 4, 0x00);
  BUF_PARSE_TEST_STR_U8("\\001", 4, 0x01);
  BUF_PARSE_TEST_STR_U8("\\010", 4, 0x08);
  BUF_PARSE_TEST_STR_U8("\\100", 4, 0x40);
  BUF_PARSE_TEST_STR_U8("\\377", 4, 0xFF);
  BUF_PARSE_TEST_STR_U8("\\0008", 4, 0x00);
  BUF_PARSE_TEST_STR_U8("\\0018", 4, 0x01);
  BUF_PARSE_TEST_STR_U8("\\0108", 4, 0x08);
  BUF_PARSE_TEST_STR_U8("\\1008", 4, 0x40);
  BUF_PARSE_TEST_STR_U8("\\3778", 4, 0xFF);
  BUF_PARSE_TEST_STR_U8("\\x00", 4, 0x00);
  BUF_PARSE_TEST_STR_U8("\\x01", 4, 0x01);
  BUF_PARSE_TEST_STR_U8("\\x0F", 4, 0x0F);
  BUF_PARSE_TEST_STR_U8("\\x80", 4, 0x80);
  BUF_PARSE_TEST_STR_U8("\\xF0", 4, 0xF0);
  BUF_PARSE_TEST_STR_U8("\\xFF8", 4, 0xFF);
  BUF_PARSE_TEST_STR_U8("\\x008", 4, 0x00);
  BUF_PARSE_TEST_STR_U8("\\x018", 4, 0x01);
  BUF_PARSE_TEST_STR_U8("\\x0F8", 4, 0x0F);
  BUF_PARSE_TEST_STR_U8("\\x808", 4, 0x80);
  BUF_PARSE_TEST_STR_U8("\\xF08", 4, 0xF0);
  BUF_PARSE_TEST_STR_U8("\\xFF8", 4, 0xFF);
}

void buf_parse_test_sym ()
{
  BUF_PARSE_TEST_NOT_SYM("0");
  BUF_PARSE_TEST_NOT_SYM("9");
  BUF_PARSE_TEST_NOT_SYM("a");
  BUF_PARSE_TEST_NOT_SYM("z");
  BUF_PARSE_TEST_NOT_SYM("00");
  BUF_PARSE_TEST_NOT_SYM("0Abc");
  BUF_PARSE_TEST_NOT_SYM("0abc");
  BUF_PARSE_TEST_NOT_SYM(":");
  BUF_PARSE_TEST_NOT_SYM("_");
  BUF_PARSE_TEST_NOT_SYM("_abc");
  BUF_PARSE_TEST_SYM(":\"0\"", "0");
  BUF_PARSE_TEST_SYM(":\"9\"", "9");
  BUF_PARSE_TEST_SYM(":\"A\"", "A");
  BUF_PARSE_TEST_SYM(":\"Z\"", "Z");
  BUF_PARSE_TEST_SYM(":\"a\"", "a");
  BUF_PARSE_TEST_SYM(":\"z\"", "z");
  BUF_PARSE_TEST_SYM(":\"_az\"", "_az");
  BUF_PARSE_TEST_SYM("A", "A");
  BUF_PARSE_TEST_SYM("Z", "Z");
  BUF_PARSE_TEST_SYM("Az09az", "Az09az");
  BUF_PARSE_TEST_SYM(":az09AZ", "az09AZ");
}

void buf_parse_test_f32()
{
  BUF_PARSE_TEST_F32("123.123", 123.123);
  BUF_PARSE_TEST_F32("3.14159", 3.14159);
  BUF_PARSE_TEST_F32("2.1e+2", 209.99998);
}