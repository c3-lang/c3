/* c3
 * Copyright 2022 Thomas de Grivel <thoxdg@gmail.com>
 */
#include <assert.h>
#include <err.h>
#include <stdlib.h>
#include <string.h>
#include "buf.h"
#include "character.h"
#include "str.h"
#include "sym.h"

const sw buf_u8_to_hex_size = 2;
const sw buf_inspect_str_byte_size = 4;

void buf_refill_move (s_buf *buf);

void buf_clean (s_buf *buf)
{
  assert(buf);
  buf->size = 0;
  if (buf->free)
    free(buf->ptr.p);
  buf->free = false;
  buf->ptr.p = NULL;
}

void buf_delete (s_buf *buf)
{
  assert(buf);
  buf_clean(buf);
  free(buf);
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

sw buf_flush (s_buf *buf)
{
  if (buf->flush)
    return buf->flush(buf);
  return 0;
}

s_buf * buf_init (s_buf *buf, bool free, uw size, s8 *p)
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
  return buf;
}

s_buf * buf_init_1 (s_buf *buf, bool free, s8 *p)
{
  uw size;
  assert(p);
  size = strlen(p);
  buf_init(buf, free, size, p);
  buf->wpos = size;
  return buf;
}

s_buf * buf_init_alloc (s_buf *buf, uw size)
{
  s8 *p;
  assert(buf);
  assert(size);
  p = calloc(size, 1);
  if (!p)
    err(1, "out of memory");
  buf_init(buf, true, size, p);
  return buf;
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

sw buf_peek_character (s_buf *buf, character *c)
{
  assert(buf);
  assert(c);
  const u8 *b;
  u8 x[4];
  const u8 _00000111 = 0x07;
  const u8 _00001111 = 0x0F;
  const u8 _00011111 = 0x1F;
  const u8 _00111111 = 0x3F;
  const u8 _10000000 = 0x80;
  const u8 _11000000 = 0xC0;
  const u8 _11100000 = 0xE0;
  const u8 _11110000 = 0xF0;
  const u8 _11111000 = 0xF8;
  if (buf->wpos - buf->rpos < 1)
    return -1;
  b = (const u8 *) buf->ptr.pu8 + buf->rpos;
  if ((b[0] & _10000000) == 0) {
    *c = b[0];
    return 1;
  }
  if ((b[0] & _11100000) == _11000000) {
    if (buf->wpos - buf->rpos < 2)
      return -1;
    if ((b[1] & _11000000) != _10000000)
      return -1;
    x[0] = b[0] & _00011111;
    x[1] = b[1] & _00111111;
    *c = (x[0] << 6) | x[1];
    return 2;
  }
  if ((b[0] & _11110000) == _11100000) {
    if (buf->wpos - buf->rpos < 3)
      return -1;
    if ((b[1] & _11000000) != _10000000)
      return -1;
    if ((b[2] & _11000000) != _10000000)
      return -1;
    x[0] = b[0] & _00001111;
    x[1] = b[1] & _00111111;
    x[2] = b[2] & _00111111;
    *c = (x[0] << 12) | (x[1] << 6) | x[2];
    return 3;
  }
  if ((b[0] & _11111000) == _11110000) {
    if (buf->wpos - buf->rpos < 4)
      return -1;
    if ((b[1] & _11000000) != _10000000)
      return -1;
    if ((b[2] & _11000000) != _10000000)
      return -1;
    if ((b[3] & _11000000) != _10000000)
      return -1;
    x[0] = b[0] & _00000111;
    x[1] = b[1] & _00111111;
    x[2] = b[2] & _00111111;
    x[3] = b[3] & _00111111;
    *c = (x[0] << 18) | (x[1] << 12) | (x[2] << 6) | x[3];
    return 4;
  }
  return -1;
}

sw buf_read (s_buf *buf, u8 *p)
{
  sw r;
  r = buf_peek(buf, p);
  if (r == 1)
    buf->rpos++;
  return r;
}

sw buf_read_character (s_buf *buf, character *p)
{
  sw r;
  r = buf_peek_character(buf, p);
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
        memmove(buf->ptr.p,
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

sw buf_str_to_hex (s_buf *buf, const s_str *src)
{
  const u8 *b;
  sw size;
  uw i;
  if (src->size == 0)
    return 0;
  size = src->size * 2;
  if (buf->wpos + size > buf->size) {
    assert(! "buffer overflow");
    return -1;
  }
  b = src->ptr.pu8;
  i = 0;
  while (i++ < src->size)
    buf_u8_to_hex(buf, *(b++));
  return size;
}

sw buf_u8_to_hex (s_buf *buf, u8 x)
{
  u8 digit;
  if (buf->wpos + buf_u8_to_hex_size > buf->size) {
    assert(! "buffer overflow");
    return -1;
  }
  digit = x >> 4;
  if (digit < 10)
    buf_write(buf, digit + '0');
  else
    buf_write(buf, digit - 10 + 'A');
  digit = x & 0xF;
  if (digit < 10)
    buf_write(buf, digit + '0');
  else
    buf_write(buf, digit - 10 + 'A');
  return buf_u8_to_hex_size;
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
  buf->ptr.pu8[buf->wpos] = v;
  buf->wpos++;
  return 1;
}

sw buf_write_1 (s_buf *buf, s8 *p)
{
  s_str stra;
  str_init_1(&stra, NULL, p);
  return buf_write_str(buf, &stra);
}

sw buf_write_character (s_buf *buf, character c)
{
  sw size = character_utf8_size(c);
  if (size <= 0)
    return size;
  if (buf->wpos + size > buf->size) {
    assert(! "buffer overflow");
    return -1;
  }
  character_utf8(c, buf->ptr.ps8 + buf->wpos);
  buf->wpos += size;
  return size;
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
  memcpy(buf->ptr.pu8 + buf->wpos, src->ptr.p, src->size);
  buf->wpos += src->size;
  return src->size;
}

sw buf_xfer (s_buf *buf, s_buf *src, uw size)
{
  assert(buf);
  assert(src);
  if (size == 0)
    return 0;
  assert(src->rpos <= src->wpos);
  if (src->rpos + size > src->wpos)
    return 0;
  if (buf->wpos + size > buf->size) {
    assert(! "buffer overflow");
    return -1;
  }
  memcpy(buf->ptr.ps8 + buf->wpos, src->ptr.ps8 + src->rpos, size);
  src->rpos += size;
  buf->wpos += size;
  return size;
}
