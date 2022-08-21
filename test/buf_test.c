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
    TEST_EQ(bufa.str.bytes, 0);                 \
    TEST_ASSERT(bufa.str.ptr.p == NULL);        \
    TEST_EQ(bufa.str.ptr.u64, 0);               \
  } while(0)

#define TEST_BUF_DELETE(buf) \
  do {                       \
    buf_delete(buf);         \
    test_ok();               \
  } while(0)

void buf_test_init_clean ()
{
  char a[4] = "test";
  size_t len;
  char *m;
  s_buf bufa;
  len = 4;
  buf_init(&bufa, false, len, a);
  TEST_EQ(bufa.str.bytes, len);
  TEST_EQ(strncmp(bufa.str.ptr.p, "test", len), 0);
  TEST_EQ(bufa.rpos, 0);
  TEST_EQ(bufa.wpos, 0);
  TEST_BUF_CLEAN(bufa);
  len = 4;
  m = malloc(len);
  memcpy(m, "test", len);
  buf_init(&bufa, true, len, m);
  TEST_EQ(bufa.str.bytes, len);
  TEST_EQ(strncmp(bufa.str.ptr.p, "test", len), 0);
  TEST_EQ(bufa.rpos, 0);
  TEST_EQ(bufa.wpos, 0);
  TEST_BUF_CLEAN(bufa);
}

void buf_test_new_delete ()
{
  char a[4] = "test";
  size_t len;
  char *m;
  s_buf *buf;
  len = 4;
  TEST_ASSERT((buf = buf_new(false, len, a)));
  TEST_EQ(buf->str.bytes, len);
  TEST_EQ(strncmp(buf->str.ptr.p, "test", len), 0);
  TEST_EQ(buf->rpos, 0);
  TEST_EQ(buf->wpos, 0);
  TEST_BUF_DELETE(buf);
  len = 4;
  m = malloc(len);
  memcpy(m, "test", len);
  TEST_ASSERT((buf = buf_new(true, len, m)));
  TEST_EQ(buf->str.bytes, len);
  TEST_EQ(strncmp(buf->str.ptr.p, "test", len), 0);
  TEST_EQ(buf->rpos, 0);
  TEST_EQ(buf->wpos, 0);
  TEST_BUF_DELETE(buf);
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

void buf_test_read_str ()
{
  char a[8]  = "ABCDEFGH";
  char a1[8] = "ABCDEFGH";
  s_buf buf;
  s_str str0;
  s_str str1;
  s_str str2;
  s_str str3;
  buf_init(&buf, false, sizeof(a), a);
  str_init(&str0, false, 0, a1);
  str_init(&str1, false, 1, a1);
  str_init(&str2, false, 2, a1);
  str_init(&str3, false, 3, a1);
  TEST_EQ(buf_read_str(&buf, &str0), 0);
  TEST_EQ(buf.rpos, 0);
  TEST_EQ(buf.wpos, 0);
  TEST_EQ(buf_read_str(&buf, &str1), 0);
  TEST_EQ(buf.rpos, 0);
  TEST_EQ(buf.wpos, 0);
  TEST_EQ(buf_read_str(&buf, &str2), 0);
  TEST_EQ(buf.rpos, 0);
  TEST_EQ(buf.wpos, 0);
  TEST_EQ(buf_read_str(&buf, &str3), 0);
  TEST_EQ(buf.rpos, 0);
  TEST_EQ(buf.wpos, 0);
  buf.wpos = 1;
  TEST_EQ(buf_read_str(&buf, &str0), 0);
  TEST_EQ(buf.rpos, 0);
  TEST_EQ(buf.wpos, 1);
  TEST_EQ(buf_read_str(&buf, &str1), 1);
  TEST_EQ(buf.rpos, 1);
  TEST_EQ(buf.wpos, 1);
  TEST_STRNCMP(str1.ptr.p, "A", 1);
  buf.wpos = 2;
  TEST_EQ(buf_read_str(&buf, &str2), 0);
  TEST_EQ(buf.rpos, 1);
  TEST_EQ(buf.wpos, 2);
  TEST_EQ(buf_read_str(&buf, &str3), 0);
  TEST_EQ(buf.rpos, 1);
  TEST_EQ(buf.wpos, 2);
  buf.wpos = 3;
  TEST_EQ(buf_read_str(&buf, &str2), 2);
  TEST_EQ(buf.rpos, 3);
  TEST_EQ(buf.wpos, 3);
  TEST_STRNCMP(str2.ptr.p, "BC", 1);
  buf.wpos = 5;
  TEST_EQ(buf_read_str(&buf, &str3), 0);
  TEST_EQ(buf.rpos, 3);
  TEST_EQ(buf.wpos, 5);
  buf.wpos = 6;
  TEST_EQ(buf_read_str(&buf, &str3), 3);
  TEST_EQ(buf.rpos, 6);
  TEST_EQ(buf.wpos, 6);
  TEST_STRNCMP(str3.ptr.p, "DEF", 3);
}

void buf_test_write ()
{
}

void buf_test_write_str ()
{
}

void buf_test ()
{
  buf_test_init_clean();
  buf_test_new_delete();
  buf_test_read();
  buf_test_write();
  buf_test_write_str();
  buf_test_read_str();
}
