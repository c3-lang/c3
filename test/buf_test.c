/* c3
 * Copyright 2022 Thomas de Grivel <thoxdg@gmail.com>
 */
#include <stdlib.h>
#include <string.h>
#include "../libc3/buf.h"
#include "../libc3/str.h"
#include "test.h"

#define BUF_TEST_CLEAN(bufa)                    \
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

#define BUF_TEST_DELETE(buf) \
  do {                       \
    buf_delete(buf);         \
    test_ok();               \
  } while(0)

#define BUF_TEST_F(test, expected)                   \
  do {                                               \
    u64 pos = buf.wpos;                              \
    uw len = strlen(expected);                       \
    test_context(# test " -> " # expected);          \
    TEST_EQ(test, len);                              \
    TEST_EQ(buf.wpos, pos + len);                    \
    TEST_STRNCMP(buf.ptr.ps8 + pos, expected, len);  \
  } while (0)

#define BUF_TEST_READ_CHARACTER(test, result)                          \
  do {                                                                 \
    character c;                                                       \
    s_buf buf;                                                         \
    test_context("buf_read_character(" # test ") -> " # result);       \
    buf_init_1(&buf, (test));                                          \
    TEST_ASSERT(buf_read_character(&buf, &c) >= 0);                    \
    TEST_EQ(c, result);                                                \
    test_context(NULL);                                                \
  } while (0)

#define BUF_TEST_READ_N(test, n, result)                               \
  do {                                                                 \
    character c;                                                       \
    s_buf buf;                                                         \
    test_context("buf_read_n(" # test ", " # n ") -> " # result);      \
    buf_init_1(&buf, (test));                                          \
    TEST_ASSERT(buf_read_n(&buf, &c) >= 0);                            \
    TEST_EQ(c, result);                                                \
  } while (0)

void buf_test_f ();
void buf_test_init_clean ();
void buf_test_new_delete ();
void buf_test_new_alloc_delete ();
void buf_test_peek ();
void buf_test_read_character ();
void buf_test_read_u8 ();
void buf_test_write_str ();
void buf_test_write_u8 ();
void buf_test_write_u16 ();
void buf_test_write_u32 ();

void buf_test ()
{
  buf_test_init_clean();
  buf_test_new_delete();
  buf_test_new_alloc_delete();
  buf_test_peek();
  buf_test_read_u16();
  buf_test_read_character();
  buf_test_write_u8();
  buf_test_write_u16();
  buf_test_write_u32();
  buf_test_write_str();
  buf_test_f();
}

void buf_test_f ()
{
  s_buf buf;
  BUF_INIT_ALLOCA(&buf, 32);
  BUF_TEST_F(buf_f(&buf, "09AZaz"), "09AZaz");
  BUF_TEST_F(buf_f(&buf, "%d", 0), "0");
  BUF_TEST_F(buf_f(&buf, "%d", 42), "42");
  BUF_TEST_F(buf_f(&buf, "%d", -1), "-1");
  BUF_TEST_F(buf_f(&buf, "%s", ""), "");
  BUF_TEST_F(buf_f(&buf, "%s", " "), " ");
  TEST_EQ(buf_f(&buf, "%s", "ÉoàΠ꒴𐅀 \n\r\t\v\\\""), 0);
  buf.wpos = 0;
  BUF_TEST_F(buf_f(&buf, "%s", "ÉoàΠ꒴𐅀 \n\r\t\v\\\""),
             "ÉoàΠ꒴𐅀 \n\r\t\v\\\"");
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
  BUF_TEST_CLEAN(bufa);
  len = 4;
  m = malloc(len);
  memcpy(m, "test", len);
  buf_init(&bufa, true, len, m);
  TEST_EQ(bufa.size, len);
  TEST_EQ(strncmp(bufa.ptr.p, "test", len), 0);
  TEST_EQ(bufa.rpos, 0);
  TEST_EQ(bufa.wpos, 0);
  BUF_TEST_CLEAN(bufa);
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
  BUF_TEST_DELETE(buf);
  len = 4;
  m = malloc(len);
  memcpy(m, "test", len);
  TEST_ASSERT((buf = buf_new(true, len, m)));
  TEST_EQ(buf->size, len);
  TEST_EQ(strncmp(buf->ptr.p, "test", len), 0);
  TEST_EQ(buf->rpos, 0);
  TEST_EQ(buf->wpos, 0);
  BUF_TEST_DELETE(buf);
}

void buf_test_new_alloc_delete ()
{
  size_t len;
  s_buf *buf;
  len = 1;
  while (len < 16) {
    TEST_ASSERT((buf = buf_new_alloc(len)));
    TEST_EQ(buf->size, len);
    TEST_EQ(buf->rpos, 0);
    TEST_EQ(buf->wpos, 0);
    BUF_TEST_DELETE(buf);
    len++;
  }
}

void buf_test_peek ()
{
}

void buf_test_read_u8 ()
{
  char a[8] = "ABCDEFGH";
  s_buf buf;
  u8 byte;
  buf_init(&buf, false, sizeof(a), a);
  TEST_EQ(buf_read_u8(&buf, &byte), 0);
  TEST_EQ(buf_read_u8(&buf, &byte), 0);
  buf.wpos = 1;
  TEST_EQ(buf_read_u8(&buf, &byte), 1);
  TEST_EQ(byte, 'A');
  TEST_EQ(buf.rpos, 1);
  TEST_EQ(buf_read_u8(&buf, &byte), 0);
  TEST_EQ(buf_read_u8(&buf, &byte), 0);
  buf.wpos = 5;
  TEST_EQ(buf_read_u8(&buf, &byte), 1);
  TEST_EQ(byte, 'B');
  TEST_EQ(buf.rpos, 2);
  TEST_EQ(buf_read_u8(&buf, &byte), 1);
  TEST_EQ(byte, 'C');
  TEST_EQ(buf.rpos, 3);
  TEST_EQ(buf_read_u8(&buf, &byte), 1);
  TEST_EQ(byte, 'D');
  TEST_EQ(buf.rpos, 4);
  TEST_EQ(buf_read_u8(&buf, &byte), 1);
  TEST_EQ(byte, 'E');
  TEST_EQ(buf.rpos, 5);
  TEST_EQ(buf_read_u8(&buf, &byte), 0);
  TEST_EQ(buf_read_u8(&buf, &byte), 0);
  buf.wpos = 8;
  TEST_EQ(buf_read_u8(&buf, &byte), 1);
  TEST_EQ(byte, 'F');
  TEST_EQ(buf.rpos, 6);
  TEST_EQ(buf_read_u8(&buf, &byte), 1);
  TEST_EQ(byte, 'G');
  TEST_EQ(buf.rpos, 7);
  TEST_EQ(buf_read_u8(&buf, &byte), 1);
  TEST_EQ(byte, 'H');
  TEST_EQ(buf.rpos, 8);
  TEST_EQ(buf_read_u8(&buf, &byte), 0);
  buf_clean(&buf);
}

void buf_test_read_character ()
{
  BUF_TEST_READ_CHARACTER("0", '0');
  BUF_TEST_READ_CHARACTER("9", '9');
  BUF_TEST_READ_CHARACTER("A", 'A');
  BUF_TEST_READ_CHARACTER("Z", 'Z');
  BUF_TEST_READ_CHARACTER("a", 'a');
  BUF_TEST_READ_CHARACTER("z", 'z');
  BUF_TEST_READ_CHARACTER("Π", 928);
  BUF_TEST_READ_CHARACTER("꒴", 42164);
  BUF_TEST_READ_CHARACTER("𐅀", 65856);
}

void buf_test_write_u8 ()
{
  s_buf buf;
  BUF_INIT_ALLOCA(&buf, 4);
  TEST_EQ(buf_write_u8(&buf, 0x00), 1);
  TEST_EQ(buf.rpos, 0);
  TEST_EQ(buf.wpos, 1);
  TEST_EQ(buf.ptr.pu8[0], 0x00);
  TEST_EQ(buf_write_u8(&buf, 0x01), 1);
  TEST_EQ(buf.rpos, 0);
  TEST_EQ(buf.wpos, 2);
  TEST_EQ(buf.ptr.pu8[1], 0x01);
  TEST_EQ(buf_write_u8(&buf, 0xFE), 1);
  TEST_EQ(buf.rpos, 0);
  TEST_EQ(buf.wpos, 3);
  TEST_EQ(buf.ptr.pu8[2], 0xFE);
  TEST_EQ(buf_write_u8(&buf, 0xFF), 1);
  TEST_EQ(buf.rpos, 0);
  TEST_EQ(buf.wpos, 4);
  TEST_EQ(buf.ptr.pu8[3], 0xFF);
}

void buf_test_write_u16 ()
{
  s_buf buf;
  BUF_INIT_ALLOCA(&buf, 8);
  TEST_EQ(buf_write_u16(&buf, 0x0000), 2);
  TEST_EQ(buf.rpos, 0);
  TEST_EQ(buf.wpos, 2);
  TEST_EQ(*((u16 *) buf.ptr.pu8), 0x0000);
  TEST_EQ(buf_write_u16(&buf, 0x0001), 2);
  TEST_EQ(buf.rpos, 0);
  TEST_EQ(buf.wpos, 4);
  TEST_EQ(*((u16 *) (buf.ptr.pu8 + 2)), 0x0001);
  TEST_EQ(buf_write_u16(&buf, 0xFF00), 2);
  TEST_EQ(buf.rpos, 0);
  TEST_EQ(buf.wpos, 6);
  TEST_EQ(*((u16 *) (buf.ptr.pu8 + 4)), 0xFF00);
  TEST_EQ(buf_write_u16(&buf, 0xFFFF), 2);
  TEST_EQ(buf.rpos, 0);
  TEST_EQ(buf.wpos, 8);
  TEST_EQ(*((u16 *) (buf.ptr.pu8 + 6)), 0xFFFF);
}

void buf_test_write_u32 ()
{
  s_buf buf;
  BUF_INIT_ALLOCA(&buf, 16);
  TEST_EQ(buf_write_u32(&buf, 0x00000000), 4);
  TEST_EQ(buf.rpos, 0);
  TEST_EQ(buf.wpos, 4);
  TEST_EQ(*((u32 *) buf.ptr.pu8), 0x00000000);
  TEST_EQ(buf_write_u32(&buf, 0x00000001), 4);
  TEST_EQ(buf.rpos, 0);
  TEST_EQ(buf.wpos, 8);
  TEST_EQ(*((u32 *) (buf.ptr.pu8 + 4)), 0x00000001);
  TEST_EQ(buf_write_u32(&buf, 0xFFFF0000), 4);
  TEST_EQ(buf.rpos, 0);
  TEST_EQ(buf.wpos, 12);
  TEST_EQ(*((u32 *) (buf.ptr.pu8 + 8)), 0xFFFF0000);
  TEST_EQ(buf_write_u32(&buf, 0xFFFFFFFF), 4);
  TEST_EQ(buf.rpos, 0);
  TEST_EQ(buf.wpos, 16);
  TEST_EQ(*((u32 *) (buf.ptr.pu8 + 12)), 0xFFFFFFFF);
}

void buf_test_write_str ()
{
}
