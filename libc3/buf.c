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
  assert(buf);
  if (buf->free)
    free(buf->ptr.p);
  buf->free = false;
  buf->ptr.p = NULL;
  buf->size = 0;
}

void buf_delete (s_buf *buf)
{
  assert(buf);
  buf_clean(buf);
  free(buf);
}

void buf_init (s_buf *buf, bool free, uw size, s8 *p)
{
  assert(buf);
  assert(size);
  assert(p);
  buf->free = free;
  buf->size = size;
  buf->ptr.u64 = (u64) p;
  buf->rpos = 0;
  buf->wpos = 0;
  buf->refill = NULL;
  buf->flush = NULL;
}

void buf_init_alloc (s_buf *buf, uw size)
{
  s8 *p;
  assert(buf);
  assert(size);
  p = calloc(size, 1);
  if (!p)
    err(1, "out of memory");
  buf_init(buf, true, size, p);
}

s_buf * buf_new (bool free, uw size, s8 *p)
{
  s_buf *buf;
  buf = malloc(sizeof(s_buf));
  if (! buf)
    err(1, "out of memory");
  buf_init(buf, free, size, p);
  return buf;
}

s_buf * buf_new_alloc (uw size)
{
  s_buf *buf;
  buf = malloc(sizeof(s_buf));
  if (! buf)
    err(1, "out of memory");
  buf_init_alloc(buf, size);
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
  assert(buf->wpos <= buf->size);
  if (buf->rpos == buf->wpos) {
    r = buf_refill(buf);
    if (r <= 0)
      return r;
  }
  if (buf->rpos == buf->wpos)
    return 0;
  if (buf->rpos >= buf->size) {
    assert(! "buffer overflow");
    return -1;
  }
  *p = buf->ptr.pu8[buf->rpos];
  return 1;
}

sw buf_peek_str (s_buf *buf, s_str *str)
{
  sw r;
  assert(buf);
  assert(str);
  assert(buf->rpos <= buf->wpos);
  if (buf->rpos + str->size > buf->wpos) {
    r = buf_refill(buf);
    if (r <= 0)
      return r;
  }
  if (buf->rpos + str->size > buf->size) {
    assert(! "buffer is too small");
    return -1;
  }
  if (buf->rpos + str->size > buf->wpos)
    return 0;
  memcpy((void*) str->ptr.p, (s8 *) buf->ptr.p + buf->rpos, str->size);
  return str->size;
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
        uw size;
        size = buf->wpos - buf->rpos;
        assert(size < buf->size);
        memmove((void *) buf->ptr.p,
                buf->ptr.ps8 + buf->rpos,
                size);
        buf->rpos = 0;
        buf->wpos = size;
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
  assert(buf->wpos <= buf->size);
  if (buf->wpos == buf->size &&
      (r = buf_flush(buf)) < 0)
    return r;
  if (buf->wpos >= buf->size) {
    assert(! "buffer overflow");
    return -1;
  }
  ((u8 *) buf->ptr.pu8)[buf->wpos] = v;
  buf->wpos++;
  return 1;
}

sw buf_write_str (s_buf *buf, const s_str *src)
{
  sw r;
  assert(buf);
  assert(src);
  assert(buf->wpos <= buf->size);
  if (buf->wpos + src->size > buf->size &&
      (r = buf_flush(buf)) <= 0)
    return r;
  if (buf->wpos + src->size > buf->size) {
    assert(! "buffer overflow");
    return -1;
  }
  memcpy(((u8 *) buf->ptr.pu8) + buf->wpos, src->ptr.p, src->size);
  buf->wpos += src->size;
  return src->size;
}

sw buf_f (s_buf *buf, const char *fmt, ...)
{
  va_list ap;
  sw r;
  va_start(ap, fmt);
  r = buf_vf(buf, fmt, ap);
  va_end(ap);
  return r;
}

sw buf_vf (s_buf *buf, const char *fmt, va_list ap)
{
  sw r;
  s_str *str;
  str = str_new_vf(fmt, ap);
  r = buf_write_str(buf, str);
  str_delete(str);
  return r;
}
