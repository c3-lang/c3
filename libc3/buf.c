/* c3
 * Copyright 2022 Thomas de Grivel <thoxdg@gmail.com>
 */
#include <assert.h>
#include <err.h>
#include <stdlib.h>
#include <string.h>
#include "buf.h"
#include "str.h"

void buf_refill_move (s_buf *buf);

void buf_clean (s_buf *buf)
{
  str_clean(&buf->str);
}

void buf_delete (s_buf *buf)
{
  assert(buf);
  buf_clean(buf);
  free(buf);
}

void buf_init (s_buf *buf, bool free, uw bytes, const s8 *p)
{
  assert(buf);
  assert(bytes);
  assert(p);
  str_init(&buf->str, free, bytes, p);
  buf->rpos = 0;
  buf->wpos = 0;
  buf->refill = NULL;
  buf->flush = NULL;
}

void buf_init_alloc (s_buf *buf, uw bytes)
{
  s8 *p;
  assert(buf);
  assert(bytes);
  p = malloc(bytes);
  if (!p)
    err(1, "out of memory");
  buf_init(buf, true, bytes, p);
}

void buf_init_str (s_buf *buf, s_str *str)
{
  assert(buf);
  assert(str);
  buf_init(buf, str->free, str->bytes, str->ptr.p);
}

s_buf * buf_new (bool free, uw bytes, const s8 *p)
{
  s_buf *buf;
  buf = malloc(sizeof(s_buf));
  if (! buf)
    err(1, "out of memory");
  buf_init(buf, free, bytes, p);
  return buf;
}

s_buf * buf_new_alloc (uw bytes)
{
  s_buf *buf;
  buf = malloc(sizeof(s_buf));
  if (! buf)
    err(1, "out of memory");
  buf_init_alloc(buf, bytes);
  return buf;
}

s_buf * buf_new_str (s_str *str)
{
  s_buf *buf;
  buf = malloc(sizeof(s_buf));
  if (! buf)
    err(1, "out of memory");
  buf_init_str(buf, str);
  return buf;
}

sw buf_flush (s_buf *buf)
{
  if (buf->flush)
    return buf->flush(buf);
  return 0;
}

sw buf_peek (s_buf *buf, u8 *p)
{
  sw r;
  assert(buf);
  assert(p);
  assert(buf->rpos <= buf->wpos);
  assert(buf->wpos <= buf->str.bytes);
  if (buf->rpos == buf->wpos) {
    r = buf_refill(buf);
    if (r <= 0)
      return r;
  }
  if (buf->rpos == buf->wpos)
    return 0;
  if (buf->rpos >= buf->str.bytes) {
    assert(! "buffer overflow");
    return -1;
  }
  *p = buf->str.ptr.pu8[buf->rpos];
  return 1;
}

sw buf_peek_str (s_buf *buf, s_str *str)
{
  sw r;
  assert(buf);
  assert(str);
  assert(buf->rpos <= buf->wpos);
  if (buf->rpos + str->bytes > buf->wpos) {
    r = buf_refill(buf);
    if (r <= 0)
      return r;
  }
  if (buf->rpos + str->bytes > buf->str.bytes) {
    assert(! "buffer is too small");
    return -1;
  }
  if (buf->rpos + str->bytes > buf->wpos)
    return 0;
  memcpy((void*) str->ptr.p, (s8 *) buf->str.ptr.p + buf->rpos, str->bytes);
  return str->bytes;
}

sw buf_read (s_buf *buf, u8 *p)
{
  sw r;
  r = buf_peek(buf, p);
  if (r == 1)
    buf->rpos++;
  return r;
}

sw buf_read_str (s_buf *buf, s_str *str)
{
  sw r;
  r = buf_peek_str(buf, str);
  if (r > 0)
    buf->rpos += r;
  return r;
}

sw buf_refill (s_buf *buf)
{
  assert(buf);
  if (buf->refill) {
    if (buf->rpos > 0) {
      if (buf->rpos == buf->wpos) {
        buf->rpos = 0;
        buf->wpos = 0;
      }
      else {
        uw bytes;
        bytes = buf->wpos - buf->rpos;
        assert(bytes < buf->str.bytes);
        memmove((void *) buf->str.ptr.p,
                buf->str.ptr.ps8 + buf->rpos,
                bytes);
        buf->rpos = 0;
        buf->wpos = bytes;
      }
    }
    return buf->refill(buf);
  }
  return 0;
}

sw buf_write (s_buf *buf, u8 v)
{
  sw r;
  assert(buf);
  assert(buf->wpos <= buf->str.bytes);
  if (buf->wpos == buf->str.bytes &&
      (r = buf_flush(buf)) < 0)
    return r;
  if (buf->wpos >= buf->str.bytes) {
    assert(! "buffer overflow");
    return -1;
  }
  ((u8 *) buf->str.ptr.pu8)[buf->wpos++] = v;
  return 1;
}

sw buf_write_str (s_buf *buf, const s_str *src)
{
  sw r;
  assert(buf);
  assert(src);
  assert(buf->wpos <= buf->str.bytes);
  if (buf->wpos + src->bytes > buf->str.bytes &&
      (r = buf_flush(buf)) < 0)
    return r;
  if (buf->wpos + src->bytes > buf->str.bytes) {
    assert(! "buffer overflow");
    return -1;
  }
  memcpy(((u8 *) buf->str.ptr.pu8) + buf->wpos, src->ptr.p, src->bytes);
  return src->bytes;
}
