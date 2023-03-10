/* c3
 * Copyright 2022,2023 kmx.io <contact@kmx.io>
 *
 * Permission is hereby granted to use this software excepted
 * on Apple computers granted the above copyright notice and
 * this permission paragraph are included in all copies and
 * substantial portions of this software.
 *
 * THIS SOFTWARE IS PROVIDED "AS-IS" WITHOUT ANY GUARANTEE OF
 * PURPOSE AND PERFORMANCE. IN NO EVENT WHATSOEVER SHALL THE
 * AUTHOR BE CONSIDERED LIABLE FOR THE USE AND PERFORMANCE OF
 * THIS SOFTWARE.
 */
#include <assert.h>
#include <err.h>
#include <stdlib.h>
#include <string.h>
#include "buf.h"
#include "buf_save.h"
#include "character.h"
#include "str.h"
#include "sym.h"

const sw buf_u8_to_hex_size = 2;
const sw buf_inspect_str_byte_size = 4;

void buf_clean (s_buf *buf)
{
  assert(buf);
  if (buf->free)
    free(buf->ptr.p);
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
  assert(buf);
  if (buf->flush)
    return buf->flush(buf);
  return buf->size - buf->wpos;
}

sw buf_ignore (s_buf *buf, uw size)
{
  u8 b;
  character c;
  uw i = 0;
  sw r;
  assert(buf);
  if (size == 0)
    return 0;
  if ((r = buf_refill(buf, size)) < 0)
    return r;
  if ((uw) r < size)
    return -1;
  if (buf->line >= 0) {
    while (i < size) {
      if ((r = buf_read_character_utf8(buf, &c)) < 0)
        return r;
      if (r > 0) {
        if (c == '\n') {
          buf->column = 0;
          buf->line++;
        }
        else
          buf->column++;
        i += r;
        continue;
      }
      if ((r = buf_read_u8(buf, &b)) < 0)
        return r;
      if (r > 0) {
          buf->column++;
        i += r;
        continue;
      }
      assert(! "error");
      return -1;
    }
    assert(i == size);
  }
  else
    buf->rpos += size;
  return size;
}

sw buf_ignore_line (s_buf *buf)
{
  character c;
  sw csize;
  sw r;
  sw result = 0;
  while ((r = buf_peek_character_utf8(buf, &c)) > 0) {
    csize = r;
    if ((r = buf_ignore(buf, csize)) < 0)
      return r;
    result += csize;
    if (c == '\n')
      break;
  }
  if (r < 0)
    return r;
  return result;
}

sw buf_ignore_spaces (s_buf *buf)
{
  character c;
  sw csize;
  sw r;
  sw result = 0;
  assert(buf);
  while ((r = buf_peek_character_utf8(buf, &c)) > 0 &&
         character_is_space(c)) {
    csize = r;
    if ((r = buf_ignore(buf, csize)) < 0)
      return r;
    result += csize;
  }
  if (! result && r < 0)
    return r;
  return result;
}

s_buf * buf_init (s_buf *buf, bool free, uw size, s8 *p)
{
  assert(buf);
  assert((!size || p) && (size || !p));
  buf->column = 0;
  buf->flush = NULL;
  buf->free = free;
  buf->line = -1;
  buf->ptr.ps8 = p;
  buf->refill = NULL;
  buf->rpos = 0;
  buf->save = NULL;
  buf->size = size;
  buf->user_ptr = NULL;
  buf->wpos = 0;
  return buf;
}

s_buf * buf_init_str (s_buf *buf, const s_str *str)
{
  assert(buf);
  assert(str);
  buf_init_alloc(buf, str->size);
  memcpy(buf->ptr.p, str->ptr.p, str->size);
  buf->wpos = str->size;
  return buf;
}

s_buf * buf_init_1 (s_buf *buf, const s8 *p)
{
  uw size;
  assert(buf);
  assert(p);
  size = strlen(p);
  buf_init_alloc(buf, size);
  memcpy(buf->ptr.p, p, size);
  buf->wpos = size;
  return buf;
}

s_buf * buf_init_alloc (s_buf *buf, uw size)
{
  s8 *p;
  assert(buf);
  if (! size)
    return buf_init(buf, false, 0, NULL);
  p = calloc(size + 1, 1);
  if (!p)
    err(1, "out of memory");
  return buf_init(buf, true, size, p);
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

sw buf_peek_1 (s_buf *buf, const s8 *p)
{
  s_str stra;
  assert(buf);
  str_init_1(&stra, NULL, p);
  return buf_peek_str(buf, &stra);
}

sw buf_peek_character_utf8 (s_buf *buf, character *c)
{
  assert(buf);
  assert(c);
  const u8 *b;
  sw r;
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
  if ((r = buf_refill(buf, 1)) < 0)
    return r;
  if (r < 1)
    return -1;
  b = buf->ptr.pu8 + buf->rpos;
  if ((b[0] & _10000000) == 0) {
    *c = b[0];
    return 1;
  }
  if ((b[0] & _11100000) == _11000000) {
    if ((r = buf_refill(buf, 2)) < 0)
      return r;
    if (r < 2)
      return -1;
    if ((b[1] & _11000000) != _10000000)
      return 0;
    x[0] = b[0] & _00011111;
    x[1] = b[1] & _00111111;
    *c = (x[0] << 6) | x[1];
    return 2;
  }
  if ((b[0] & _11110000) == _11100000) {
    if ((r = buf_refill(buf, 3)) < 0)
      return r;
    if (r < 3)
      return -1;
    if ((b[1] & _11000000) != _10000000)
      return 0;
    if ((b[2] & _11000000) != _10000000)
      return 0;
    x[0] = b[0] & _00001111;
    x[1] = b[1] & _00111111;
    x[2] = b[2] & _00111111;
    *c = (x[0] << 12) | (x[1] << 6) | x[2];
    return 3;
  }
  if ((b[0] & _11111000) == _11110000) {
    if ((r = buf_refill(buf, 4)) < 0)
      return r;
    if (r < 4)
      return -1;
    if ((b[1] & _11000000) != _10000000)
      return 0;
    if ((b[2] & _11000000) != _10000000)
      return 0;
    if ((b[3] & _11000000) != _10000000)
      return 0;
    x[0] = b[0] & _00000111;
    x[1] = b[1] & _00111111;
    x[2] = b[2] & _00111111;
    x[3] = b[3] & _00111111;
    *c = (x[0] << 18) | (x[1] << 12) | (x[2] << 6) | x[3];
    return 4;
  }
  return 0;
}

sw buf_peek_f32 (s_buf *buf, f32 *p)
{
  const sw size = sizeof(f32);
  assert(buf);
  assert(p);
  if (buf->rpos > buf->wpos ||
      buf->wpos > buf->size) {
    assert(! "buf error");
    return -1;
  }
  if (buf->rpos + size > buf->wpos)
    return 0;
  *p = *((f32 *) (buf->ptr.pu8 + buf->rpos));
  return size;
}

sw buf_peek_f64 (s_buf *buf, f64 *p)
{
  const sw size = sizeof(f64);
  assert(buf);
  assert(p);
  if (buf->rpos > buf->wpos ||
      buf->wpos > buf->size) {
    assert(! "buf error");
    return -1;
  }
  if (buf->rpos + size > buf->wpos)
    return 0;
  *p = *((f64 *) (buf->ptr.pu8 + buf->rpos));
  return size;
}

sw buf_peek_s8 (s_buf *buf, s8 *p)
{
    const sw size = sizeof(s8);
    assert(buf);
    assert(p);
    if (buf->rpos > buf->wpos ||
        buf->wpos > buf->size) {
        assert(! "buf error");
        return -1;
    }
    if (buf->rpos + size > buf->wpos)
        return 0;
    *p = *((s8 *) (buf->ptr.pu8 + buf->rpos));
    return size;
}

sw buf_peek_s16 (s_buf *buf, s16 *p)
{
    const sw size = sizeof(s16);
    assert(buf);
    assert(p);
    if (buf->rpos > buf->wpos ||
        buf->wpos > buf->size) {
        assert(! "buf error");
        return -1;
    }
    if (buf->rpos + size > buf->wpos)
        return 0;
    *p = *((s16 *) (buf->ptr.pu8 + buf->rpos));
    return size;
}

sw buf_peek_s32 (s_buf *buf, s32 *p)
{
  const sw size = sizeof(s32);
  assert(buf);
  assert(p);
  if (buf->rpos > buf->wpos ||
      buf->wpos > buf->size) {
    assert(! "buf error");
    return -1;
  }
  if (buf->rpos + size > buf->wpos)
    return 0;
  *p = *((s32 *) (buf->ptr.pu8 + buf->rpos));
  return size;
}

sw buf_peek_s64 (s_buf *buf, s64 *p)
{
  const sw size = sizeof(s64);
  assert(buf);
  assert(p);
  if (buf->rpos > buf->wpos ||
      buf->wpos > buf->size) {
    assert(! "buf error");
    return -1;
  }
  if (buf->rpos + size > buf->wpos)
    return 0;
  *p = *((s64 *) (buf->ptr.pu8 + buf->rpos));
  return size;
}

sw buf_peek_str (s_buf *buf, const s_str *src)
{
  sw size;
  assert(buf);
  assert(src);
  if (buf->rpos > buf->wpos) {
    assert(buf->rpos <= buf->wpos);
    return -1;
  }
  if (buf->wpos > buf->size) {
    assert(buf->wpos <= buf->size);
    return -1;
  }
  size = buf->wpos - buf->rpos;
  if ((uw) size < src->size) {
    if (memcmp(buf->ptr.ps8 + buf->rpos, src->ptr.p, size))
      return 0;
  }
  if (buf->rpos + src->size > buf->wpos &&
      buf_refill(buf, src->size) < (sw) src->size)
    return 0;
  if (memcmp(buf->ptr.ps8 + buf->rpos, src->ptr.p, src->size))
    return 0;
  return src->size;
}

sw buf_peek_u8 (s_buf *buf, u8 *p)
{
  sw r;
  const sw size = sizeof(u8);
  assert(buf);
  assert(p);
  if (buf->rpos > buf->wpos) {
    assert(! "buf_peek_u8: rpos > wpos");
    return -1;
  }
  if (buf->wpos > buf->size) {
    assert(! "buf_peek_u8: wpos > size");
    return -1;
  }
  if (buf->rpos + size > buf->wpos) {
    if ((r = buf_refill(buf, size)) < 0)
      return r;
    if (r < size)
      return -1;
  }
  if (buf->rpos + size > buf->wpos) {
    assert(! "buf_peek_u8: buffer overflow");
    return -1;
  }
  *p = buf->ptr.pu8[buf->rpos];
  return size;
}

sw buf_peek_u16 (s_buf *buf, u16 *p)
{
  const sw size = sizeof(u16);
  assert(buf);
  assert(p);
  if (buf->rpos > buf->wpos ||
      buf->wpos > buf->size) {
    assert(! "buf error");
    return -1;
  }
  if (buf->rpos + size > buf->wpos)
    return 0;
  *p = *((u16 *) (buf->ptr.pu8 + buf->rpos));
  return size;
}

sw buf_peek_u32 (s_buf *buf, u32 *p)
{
  const sw size = sizeof(u32);
  assert(buf);
  assert(p);
  if (buf->rpos > buf->wpos ||
      buf->wpos > buf->size) {
    assert(! "buf error");
    return -1;
  }
  if (buf->rpos + size > buf->wpos)
    return 0;
  *p = *((u32 *) (buf->ptr.pu8 + buf->rpos));
  return size;
}

sw buf_peek_u64 (s_buf *buf, u64 *p)
{
    const sw size = sizeof(u64);
    assert(buf);
    assert(p);
    if (buf->rpos > buf->wpos ||
        buf->wpos > buf->size) {
        assert(! "buf error");
        return -1;
    }
    if (buf->rpos + size > buf->wpos)
        return 0;
    *p = *((u64 *) (buf->ptr.pu8 + buf->rpos));
    return size;
}

sw buf_read_1 (s_buf *buf, const s8 *p)
{
  s_str stra;
  assert(buf);
  str_init_1(&stra, NULL, p);
  return buf_read_str(buf, &stra);
}

sw buf_read_character_utf8 (s_buf *buf, character *p)
{
  sw r;
  r = buf_peek_character_utf8(buf, p);
  if (r > 0) {
    if (buf->line >= 0) {
      if (*p == '\n') {
        buf->column = 0;
        buf->line++;
      }
      else
        buf->column++;
    }
    buf->rpos += r;
  }
  return r;
}

sw buf_read_f32 (s_buf *buf, f32 *p)
{
  sw r;
  r = buf_peek_f32(buf, p);
  if (r > 0) {
    assert(r == sizeof(f32));
    buf->rpos += r;
  }
  return r;
}

sw buf_read_f64 (s_buf *buf, f64 *p)
{
  sw r;
  r = buf_peek_f64(buf, p);
  if (r > 0) {
    assert(r == sizeof(f64));
    buf->rpos += r;
  }
  return r;
}

sw buf_read_s8(s_buf *buf, s8 *p)
{
  sw r;
  r = buf_peek_s8(buf, p);
  if (r > 0) {
    assert(r == sizeof(s8));
    buf->rpos += r;
  }
  return r;
}

sw buf_read_s16(s_buf *buf, s16 *p)
{
  sw r;
  r = buf_peek_s16(buf, p);
  if (r > 0) {
    assert(r == sizeof(s16));
    buf->rpos += r;
  }
  return r;
}

sw buf_read_s32(s_buf *buf, s32 *p)
{
  sw r;
  r = buf_peek_s32(buf, p);
  if (r > 0) {
    assert(r == sizeof(s32));
    buf->rpos += r;
  }
  return r;
}

sw buf_read_s64(s_buf *buf, s64 *p)
{
  sw r;
  r = buf_peek_s64(buf, p);
  if (r > 0) {
    assert(r == sizeof(s64));
    buf->rpos += r;
  }
  return r;
}

sw buf_read_str (s_buf *buf, const s_str *src)
{
  sw r;
  r = buf_peek_str(buf, src);
  if (r > 0)
    return buf_ignore(buf, r);
  return r;
}

sw buf_read_to_str (s_buf *buf, s_str *dest)
{
  sw size;
  assert(buf);
  assert(dest);
  if (buf->rpos > buf->wpos)
    return -1;
  if (buf->wpos > buf->size)
    return -1;
  size = buf->wpos - buf->rpos;
  if (size == 0) {
    str_init_empty(dest);
    return 0;
  }
  str_init_alloc(dest, size, buf->ptr.ps8 + buf->rpos);
  return buf_ignore(buf, size);
}

sw buf_read_u8 (s_buf *buf, u8 *p)
{
  sw r;
  r = buf_peek_u8(buf, p);
  if (r > 0)
    buf->rpos += r;
  return r;
}

sw buf_read_u16 (s_buf *buf, u16 *p)
{
  sw r;
  r = buf_peek_u16(buf, p);
  if (r > 0) {
    assert(r == sizeof(u16));
    buf->rpos += r;
  }
  return r;
}

sw buf_read_u32 (s_buf *buf, u32 *p)
{
  sw r;
  r = buf_peek_u32(buf, p);
  if (r > 0) {
    assert(r == sizeof(u32));
    buf->rpos += r;
  }
  return r;
}

sw buf_read_u64 (s_buf *buf, u64 *p)
{
  sw r;
  r = buf_peek_u64(buf, p);
  if (r > 0) {
    assert(r == sizeof(u64));
    buf->rpos += r;
  }
  return r;
}

sw buf_refill (s_buf *buf, sw size)
{
  sw r = buf->wpos - buf->rpos;
  assert(buf);
  if (size <= 0)
    return r;
  if (buf->rpos + size > buf->wpos &&
      (r = buf_refill_compact(buf)) >= 0 &&
      buf->refill)
    while ((r = buf->refill(buf)) > 0 &&
           buf->wpos - buf->rpos < (uw) size)
      ;
  if (r >= 0)
    r = buf->wpos - buf->rpos;
  return r;
}

sw buf_refill_compact (s_buf *buf)
{
  uw min_rpos;
  s_buf_save *save;
  assert(buf);
  min_rpos = buf_save_min_rpos(buf);
  if (min_rpos > buf->wpos) {
    assert(min_rpos <= buf->wpos);
    return -1;
  }
  if (buf->wpos > buf->size) {
    assert(buf->wpos <= buf->size);
    return -1;
  }
  if (min_rpos > 0) {
    if (min_rpos == buf->wpos) {
      buf->rpos = 0;
      buf->wpos = 0;
    }
    else {
      uw size;
      size = buf->wpos - min_rpos;
      assert(size < buf->size);
      memmove(buf->ptr.p,
              buf->ptr.ps8 + min_rpos,
              size);
      buf->rpos -= min_rpos;
      save = buf->save;
      while (save) {
        save->rpos -= min_rpos;
        save->wpos -= min_rpos;
        save = save->next;
      }
      buf->wpos = size;
    }
    return 1;
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

s_str * buf_to_str (const s_buf *buf, s_str *str)
{
  void *free;
  assert(buf);
  assert(str);
  free = buf->free ? buf->ptr.p : NULL;
  return str_init(str, free, buf->size, buf->ptr.p);
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
    buf_write_u8(buf, digit + '0');
  else
    buf_write_u8(buf, digit - 10 + 'A');
  digit = x & 0xF;
  if (digit < 10)
    buf_write_u8(buf, digit + '0');
  else
    buf_write_u8(buf, digit - 10 + 'A');
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

sw buf_write_1 (s_buf *buf, s8 *p)
{
  s_str stra;
  str_init_1(&stra, NULL, p);
  return buf_write_str(buf, &stra);
}

sw buf_write_character_utf8 (s_buf *buf, character c)
{
  sw size = character_utf8_size(c);
  if (size <= 0)
    return size;
  if (buf->wpos + size > buf->size &&
      buf_flush(buf) < size) {
    return -1;
  }
  if (buf->wpos + size > buf->size) {
    assert(! "buffer overflow");
    return -1;
  }
  character_utf8(c, buf->ptr.ps8 + buf->wpos);
  buf->wpos += size;
  return size;
}

sw buf_write_f32 (s_buf *buf, f32 x)
{
  const sw size = 4;
  assert(buf);
  if (buf->wpos + size > buf->size &&
      buf_flush(buf) < size)
    return -1;
  if (buf->wpos + size > buf->size) {
    assert(! "buffer overflow");
    return -1;
  }
  *((f32 *) (buf->ptr.pu8 + buf->wpos)) = x;
  buf->wpos += size;
  return size;
}

sw buf_write_f64 (s_buf *buf, f64 v)
{
  const sw size = 8;
  assert(buf);
  if (buf->wpos + size > buf->size &&
      buf_flush(buf) < size)
    return -1;
  if (buf->wpos + size > buf->size) {
    assert(! "buffer overflow");
    return -1;
  }
  *((f64 *) (buf->ptr.pu8 + buf->wpos)) = v;
  buf->wpos += size;
  return size;
}

sw buf_write_s8 (s_buf *buf, s8 x)
{
  const sw size = 1;
  assert(buf);
  if (buf->wpos + size > buf->size &&
      buf_flush(buf) < size)
    return -1;
  if (buf->wpos + size > buf->size) {
    assert(! "buffer overflow");
    return -1;
  }
  buf->ptr.ps8[buf->wpos] = x;
  buf->wpos += size;
  return size;
}

sw buf_write_s16 (s_buf *buf, s16 x)
{
  const sw size = 2;
  assert(buf);
  if (buf->wpos + size > buf->size) {
    assert(! "buffer overflow");
    return -1;
  }
  *((s16 *) (buf->ptr.pu8 + buf->wpos)) = x;
  buf->wpos += size;
  return size;
}

sw buf_write_s32 (s_buf *buf, s32 x)
{
  const sw size = 4;
  assert(buf);
  if (buf->wpos + size > buf->size) {
    assert(! "buffer overflow");
    return -1;
  }
  *((s32 *) (buf->ptr.pu8 + buf->wpos)) = x;
  buf->wpos += size;
  return size;
}

sw buf_write_s64 (s_buf *buf, s64 v)
{
  const sw size = 8;
  assert(buf);
  if (buf->wpos + size > buf->size) {
    assert(! "buffer overflow");
    return -1;
  }
  *((s64 *) (buf->ptr.pu8 + buf->wpos)) = v;
  buf->wpos += size;
  return size;
}

sw buf_write_str (s_buf *buf, const s_str *src)
{
  sw r;
  assert(buf);
  assert(src);
  if (buf->wpos > buf->size)
    return -1;
  if (buf->wpos + src->size > buf->size &&
      (r = buf_flush(buf)) < (sw) src->size)
    return -1;
  if (buf->wpos + src->size > buf->size) {
    assert(! "buffer overflow");
    return -1;
  }
  memcpy(buf->ptr.pu8 + buf->wpos, src->ptr.p, src->size);
  buf->wpos += src->size;
  return src->size;
}

sw buf_write_u8 (s_buf *buf, u8 x)
{
  const sw size = 1;
  assert(buf);
  if (buf->wpos + size > buf->size &&
      buf_flush(buf) < size) {
    return -1;
  }
  if (buf->wpos + size > buf->size) {
    assert(! "buffer overflow");
    return -1;
  }
  buf->ptr.pu8[buf->wpos] = x;
  buf->wpos += size;
  return size;
}

sw buf_write_u16 (s_buf *buf, u16 x)
{
  const sw size = 2;
  assert(buf);
  if (buf->wpos + size > buf->size) {
    assert(! "buffer overflow");
    return -1;
  }
  *((u16 *) (buf->ptr.pu8 + buf->wpos)) = x;
  buf->wpos += size;
  return size;
}

sw buf_write_u32 (s_buf *buf, u32 x)
{
  const sw size = 4;
  assert(buf);
  if (buf->wpos + size > buf->size) {
    assert(! "buffer overflow");
    return -1;
  }
  *((u32 *) (buf->ptr.pu8 + buf->wpos)) = x;
  buf->wpos += size;
  return size;
}

sw buf_write_u64 (s_buf *buf, u64 v)
{
  const sw size = 8;
  assert(buf);
  if (buf->wpos + size > buf->size) {
    assert(! "buffer overflow");
    return -1;
  }
  *((u64 *) (buf->ptr.pu8 + buf->wpos)) = v;
  buf->wpos += size;
  return size;
}

sw buf_xfer (s_buf *dest, s_buf *src, uw size)
{
  sw r = 0;
  assert(dest);
  assert(src);
  if (size == 0)
    return 0;
  assert(src->rpos <= src->wpos);
  assert(dest->rpos <= dest->wpos);
  if (src->rpos + size > src->wpos &&
      (r = buf_refill(src, size)) < (sw) size) {
    if (r < 0)
      return r;
    return 0;
  }
  if (dest->wpos + size > dest->size &&
      (r = buf_flush(dest)) < (sw) size) {
    if (r < 0)
      return r;
    return -1;
  }
  memcpy(dest->ptr.ps8 + dest->wpos, src->ptr.ps8 + src->rpos, size);
  src->rpos += size;
  dest->wpos += size;
  return size;
}

sw buf_xfer_reverse (s_buf *src, s_buf *dest)
{
  sw size;
  assert(src);
  assert(dest);
  size = src->wpos - src->rpos;
  if (dest->wpos + size > dest->size &&
      buf_flush(dest) < size)
    return -1;
  for (sw i = 0; i < size; i++)
    dest->ptr.ps8[dest->wpos + i] = src->ptr.ps8[src->wpos - 1 - i];
  src->rpos += size;
  dest->wpos += size;
  return size;
}
