/* c3
 * Copyright 2022 Thomas de Grivel <thoxdg@gmail.com>
 */
#include <stdlib.h>
#include <string.h>
#include "../libc3/buf.h"
#include "test.h"

#define TEST_BUF_CLEAN(bufa)                    \
  do {                                          \
    buf_clean(&bufa);                           \
    TEST_ASSERT(bufa.str.bytes == 0);           \
    TEST_ASSERT(bufa.str.ptr.p == NULL);        \
    TEST_ASSERT(bufa.str.ptr.u64 == 0);         \
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
  TEST_ASSERT(bufa.str.bytes == len);
  TEST_ASSERT(strncmp(bufa.str.ptr.p, "test", len) == 0);
  TEST_ASSERT(bufa.rpos == 0);
  TEST_ASSERT(bufa.wpos == 0);
  TEST_BUF_CLEAN(bufa);
  len = 4;
  m = malloc(len);
  memcpy(m, "test", len);
  buf_init(&bufa, true, len, m);
  TEST_ASSERT(bufa.str.bytes == len);
  TEST_ASSERT(strncmp(bufa.str.ptr.p, "test", len) == 0);
  TEST_ASSERT(bufa.rpos == 0);
  TEST_ASSERT(bufa.wpos == 0);
  TEST_BUF_CLEAN(bufa);
}

void buf_test_new_delete ()
{
  char a[4] = "test";
  size_t len;
  char *m;
  s_buf *buf;
  len = 4;
  TEST_ASSERT(buf = buf_new(false, len, a));
  TEST_ASSERT(buf->str.bytes == len);
  TEST_ASSERT(strncmp(buf->str.ptr.p, "test", len) == 0);
  TEST_ASSERT(buf->rpos == 0);
  TEST_ASSERT(buf->wpos == 0);
  TEST_BUF_DELETE(buf);
  len = 4;
  m = malloc(len);
  memcpy(m, "test", len);
  TEST_ASSERT(buf = buf_new(true, len, m));
  TEST_ASSERT(buf->str.bytes == len);
  TEST_ASSERT(strncmp(buf->str.ptr.p, "test", len) == 0);
  TEST_ASSERT(buf->rpos == 0);
  TEST_ASSERT(buf->wpos == 0);
  TEST_BUF_DELETE(buf);
}

void buf_test_read ()
{
  char a[4] = "Test";
  s_buf buf;
  u8 byte;
  buf_init(&buf, false, sizeof(a), a);
  TEST_EQ(buf_read(&buf, &byte), 1);
  buf_clean(&buf);
}

void buf_test_read_str ()
{
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
  buf_test_read_str();
  buf_test_write();
  buf_test_write_str();
}
