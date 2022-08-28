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

sw   buf_inspect_str_reserved (s_buf *buf, const s_str *str);
sw   buf_inspect_str_reserved_size (const s_str *str);
sw   buf_inspect_sym_reserved (s_buf *buf, const s_sym *sym);
sw   buf_inspect_sym_reserved_size (const s_sym *sym);
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

sw buf_inspect_character (s_buf *buf, character c)
{
  s_buf tmp;
  BUF_INIT_ALLOCA(&tmp, 18);
  buf_write(&tmp, '\'');
  buf_inspect_str_character(&tmp, c);
  buf_write(&tmp, '\'');
  return buf_xfer(buf, &tmp);
}

sw buf_inspect_character_size (character c)
{
  sw size;
  size = buf_inspect_str_character_size(c);
  if (size < 0)
    return size;
  size += 2;
  return size;
}

sw buf_inspect_str (s_buf *buf, const s_str *str)
{
  sw size;
  assert(buf);
  assert(str);
  if (str_has_reserved_characters(str))
    return buf_inspect_str_reserved(buf, str);
  size = str->size + 2;
  if (buf->wpos + size > buf->size) {
    assert(! "buffer overflow");
    return -1;
  }
  buf_write(buf, '"');
  buf_write_str(buf, str);
  buf_write(buf, '"');
  return size;
}

sw buf_inspect_str_character (s_buf *buf, character c)
{
  s_buf char_buf;
  int i;
  int j;
  sw size;
  size = buf_inspect_str_character_size(c);
  if (size <= 0)
    return size;
  if (buf->wpos + size > buf->size) {
    assert(! "buffer overflow");
    return -1;
  }
  if (! str_character_is_reserved(c))
    return buf_write_character(buf, c);
  buf_write(buf, '\\');
  switch (c) {
  case '\0': buf_write(buf, '0'); break;
  case '\n': buf_write(buf, 'n'); break;
  case '\r': buf_write(buf, 'r'); break;
  case '\t': buf_write(buf, 't'); break;
  case '\v': buf_write(buf, 'v'); break;
  case '\"': buf_write(buf, '"'); break;
  case '\'': buf_write(buf, '\''); break;
  case '\\': buf_write(buf, '\\'); break;
  default:
    BUF_INIT_ALLOCA(&char_buf, 4);
    i = buf_write_character(&char_buf, c);
    if (i <= 0) {
      buf_write(buf, 'x');
      buf_u8_to_hex(buf, c);
    }
    else {
      j = 0;
      if (i-- > 0) {
        buf_write(buf, 'x');
        buf_u8_to_hex(buf, char_buf.ptr.pu8[j++]);
        while (i--) {
          buf_write(buf, '\\');
          buf_write(buf, 'x');
          buf_u8_to_hex(buf, char_buf.ptr.pu8[j++]);
        }
      }
    }
  }
  return size;
}

sw buf_inspect_str_character_size (character c)
{
  sw csize;
  sw size;
  if (! str_character_is_reserved(c))
    return character_utf8_size(c);
  size = 0;
  switch (c) {
  case '\0':
  case '\n':
  case '\r':
  case '\t':
  case '\v':
  case '\"':
  case '\'':
  case '\\':
    size += 2;
    break;
  default:
    csize = character_utf8_size(c);
    if (csize < 0)
      csize = 1;
    size += csize * 4;
  }
  return size;
}

sw buf_inspect_str_reserved (s_buf *buf, const s_str *src)
{
  character c;
  sw size;
  s_str stra;
  size = buf_inspect_str_reserved_size(src);
  if (size <= 0)
    return size;
  if (buf->wpos + size > buf->size) {
    assert(! "buffer overflow");
    return -1;
  }
  buf_write(buf, '"');
  str_init(&stra, false, src->size, src->ptr.p);
  while (str_read_character(&stra, &c) > 0) {
    buf_inspect_str_character(buf, c);
  }
  buf_write(buf, '"');
  return size;
}

sw buf_inspect_str_reserved_size (const s_str *src)
{
  character c;
  const sw quote_size = 1;
  sw size;
  s_str stra;
  size = 2 * quote_size;
  str_init(&stra, false, src->size, src->ptr.p);
  while (str_read_character(&stra, &c) > 0) {
    const sw csize = buf_inspect_str_character_size(c);
    if (csize < 0)
      return csize;
    size += csize;
  }
  return size;
}

sw buf_inspect_str_size (const s_str *str)
{
  const sw quote_size = 1;
  sw size;
  if (str_has_reserved_characters(str))
    return buf_inspect_str_reserved_size(str);
  size = str->size + 2 * quote_size;
  return size;
}

sw buf_inspect_sym (s_buf *buf, const s_sym *sym)
{
  sw size;
  assert(buf);
  assert(sym);
  if (sym->str.size == 0)
    return buf_write_1(buf, ":\"\"");
  if (sym_has_reserved_characters(sym))
    return buf_inspect_sym_reserved(buf, sym);
  if (sym_is_module(sym))
    return buf_write_str(buf, &sym->str);
  size = sym->str.size + 1;
  if (buf->wpos + size > buf->size) {
    assert(! "buffer overflow");
    return -1;
  }
  buf_write(buf, ':');
  buf_write_str(buf, &sym->str);
  return size;
}

sw buf_inspect_sym_size (const s_sym *sym)
{
  const sw colon_size = 1;
  assert(sym);
  if (sym->str.size == 0)
    return 3;
  if (sym_has_reserved_characters(sym))
    return buf_inspect_sym_reserved_size(sym);
  if (sym_is_module(sym))
    return sym->str.size;
  return sym->str.size + colon_size;
}

sw buf_inspect_sym_reserved (s_buf *buf, const s_sym *sym)
{
  sw  size;
  size = buf_inspect_sym_reserved_size(sym);
  if (size <= 0)
    return size;
  if (buf->wpos + size > buf->size) {
    assert(! "buffer_overflow");
    return -1;
  }
  buf_write(buf, ':');
  buf_inspect_str(buf, &sym->str);
  return size;
}

sw buf_inspect_sym_reserved_size (const s_sym *sym)
{
  const sw colon_size = 1;
  sw size;
  size = buf_inspect_str_size(&sym->str);
  if (size < 0)
    return size;
  size += colon_size;
  return size;
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
    *c = *b;
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

sw buf_read_character (s_buf *buf, character *p)
{
  sw r;
  r = buf_peek_character(buf, p);
  if (r > 0)
    buf->rpos += r;
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

sw buf_str_to_hex (s_buf *buf, const s_str *src)
{
  u8 *b;
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
  ((u8 *) buf->ptr.pu8)[buf->wpos] = v;
  buf->wpos++;
  return 1;
}

sw buf_write_1 (s_buf *buf, s8 *p)
{
  s_str stra;
  str_init_1(&stra, false, p);
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
  memcpy(((u8 *) buf->ptr.pu8) + buf->wpos, src->ptr.p, src->size);
  buf->wpos += src->size;
  return src->size;
}

sw buf_xfer (s_buf *buf, s_buf *src)
{
  sw size;
  assert(buf);
  assert(src);
  size = src->wpos - src->rpos;
  assert(size >= 0);
  if (size == 0)
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
