/* c3
 * Copyright 2022 Thomas de Grivel <thoxdg@gmail.com>
 */
#include <stdlib.h>
#include <string.h>
#include "../libc3/buf.h"
#include "../libc3/str.h"
#include "test.h"

#define TEST_BUF_CLEAN(bufa)                    \
  do {                                          \
    buf_clean(&bufa);                           \
    TEST_EQ(bufa.size, 0);                      \
    TEST_ASSERT(bufa.ptr.p == NULL);            \
    TEST_EQ(bufa.ptr.u64, 0);                   \
    TEST_EQ(bufa.rpos, 0);                      \
    TEST_EQ(bufa.wpos, 0);                      \
    TEST_ASSERT(bufa.flush == NULL);            \
    TEST_ASSERT(bufa.refill == NULL);           \
  } while(0)

#define TEST_BUF_DELETE(buf) \
  do {                       \
    buf_delete(buf);         \
    test_ok();               \
  } while(0)

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
    TEST_EQ(buf_inspect_str_character_size(test), result);       \
  } while (0)

void buf_test_init_clean ();
void buf_test_inspect_str_character ();
void buf_test_inspect_str_character_size ();
void buf_test_new_delete ();
void buf_test_peek ();
void buf_test_peek_str ();
void buf_test_read ();
void buf_test_write ();
void buf_test_write_str ();

void buf_test ()
{
  buf_test_init_clean();
  buf_test_new_delete();
  buf_test_peek();
  buf_test_peek_str();
  buf_test_read();
  buf_test_write();
  buf_test_write_str();
  buf_test_inspect_str_character_size();
  buf_test_inspect_str_character();
}

void buf_test_init_clean ()
{
  char a[4] = "test";
  size_t len;
  char *m;
  s_buf bufa;
  len = 4;
  buf_init(&bufa, false, len, a);
  TEST_EQ(bufa.size, len);
  TEST_EQ(strncmp(bufa.ptr.p, "test", len), 0);
  TEST_EQ(bufa.rpos, 0);
  TEST_EQ(bufa.wpos, 0);
  TEST_BUF_CLEAN(bufa);
  len = 4;
  m = malloc(len);
  memcpy(m, "test", len);
  buf_init(&bufa, true, len, m);
  TEST_EQ(bufa.size, len);
  TEST_EQ(strncmp(bufa.ptr.p, "test", len), 0);
  TEST_EQ(bufa.rpos, 0);
  TEST_EQ(bufa.wpos, 0);
  TEST_BUF_CLEAN(bufa);
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
  BUF_TEST_INSPECT_STR_CHARACTER(0xFF, "ÿ");
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
  BUF_TEST_INSPECT_STR_CHARACTER_SIZE(0xFF, 2);
}

void buf_test_new_delete ()
{
  char a[4] = "test";
  size_t len;
  char *m;
  s_buf *buf;
  len = 4;
  TEST_ASSERT((buf = buf_new(false, len, a)));
  TEST_EQ(buf->size, len);
  TEST_EQ(strncmp(buf->ptr.p, "test", len), 0);
  TEST_EQ(buf->rpos, 0);
  TEST_EQ(buf->wpos, 0);
  TEST_BUF_DELETE(buf);
  len = 4;
  m = malloc(len);
  memcpy(m, "test", len);
  TEST_ASSERT((buf = buf_new(true, len, m)));
  TEST_EQ(buf->size, len);
  TEST_EQ(strncmp(buf->ptr.p, "test", len), 0);
  TEST_EQ(buf->rpos, 0);
  TEST_EQ(buf->wpos, 0);
  TEST_BUF_DELETE(buf);
}

void buf_test_peek ()
{
}

void buf_test_peek_str ()
{
}

void buf_test_read ()
{
  char a[8] = "ABCDEFGH";
  s_buf buf;
  u8 byte;
  buf_init(&buf, false, sizeof(a), a);
  TEST_EQ(buf_read(&buf, &byte), 0);
  TEST_EQ(buf_read(&buf, &byte), 0);
  buf.wpos = 1;
  TEST_EQ(buf_read(&buf, &byte), 1);
  TEST_EQ(byte, 'A');
  TEST_EQ(buf.rpos, 1);
  TEST_EQ(buf_read(&buf, &byte), 0);
  TEST_EQ(buf_read(&buf, &byte), 0);
  buf.wpos = 5;
  TEST_EQ(buf_read(&buf, &byte), 1);
  TEST_EQ(byte, 'B');
  TEST_EQ(buf.rpos, 2);
  TEST_EQ(buf_read(&buf, &byte), 1);
  TEST_EQ(byte, 'C');
  TEST_EQ(buf.rpos, 3);
  TEST_EQ(buf_read(&buf, &byte), 1);
  TEST_EQ(byte, 'D');
  TEST_EQ(buf.rpos, 4);
  TEST_EQ(buf_read(&buf, &byte), 1);
  TEST_EQ(byte, 'E');
  TEST_EQ(buf.rpos, 5);
  TEST_EQ(buf_read(&buf, &byte), 0);
  TEST_EQ(buf_read(&buf, &byte), 0);
  buf.wpos = 8;
  TEST_EQ(buf_read(&buf, &byte), 1);
  TEST_EQ(byte, 'F');
  TEST_EQ(buf.rpos, 6);
  TEST_EQ(buf_read(&buf, &byte), 1);
  TEST_EQ(byte, 'G');
  TEST_EQ(buf.rpos, 7);
  TEST_EQ(buf_read(&buf, &byte), 1);
  TEST_EQ(byte, 'H');
  TEST_EQ(buf.rpos, 8);
  TEST_EQ(buf_read(&buf, &byte), 0);
  buf_clean(&buf);
}

void buf_test_write ()
{
}

void buf_test_write_str ()
{
}
