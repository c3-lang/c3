/* c3
 * Copyright 2022 Thomas de Grivel <thoxdg@gmail.com>
 */
#include <assert.h>
#include <err.h>
#include <stdarg.h>
#include <stdlib.h>
#include <string.h>
#include "buf.h"
#include "character.h"
#include "str.h"

s_str * str_1 (bool free, const char *s)
{
  size_t len = strlen(s);
  s_str *str = str_new(free, len, s);
  return str;
}

void str_init_join_v (s_str *str, uw count, va_list ap)
{
  va_list ap2;
  s_buf buf;
  uw bytes;
  uw c;
  s_str *s;
  va_copy(ap2, ap);
  bytes = 0;
  c = count;
  while (c--) {
    s = va_arg(ap, s_str *);
    bytes += s->bytes;
  }
  str_init(str, true, bytes, malloc(bytes));
  buf_init_str(&buf, str);
  c = count;
  while (c--) {
    s = va_arg(ap2, s_str *);
    buf_write_str(&buf, s);
  }
  va_end(ap2);
  assert(buf.wpos == bytes);
}

void str_init_join (s_str *str, uw count, ...)
{
  va_list ap;
  va_start(ap, count);
  str_init_join_v(str, count, ap);
  va_end(ap);
}

void str_clean (s_str *str)
{
  assert(str);
  if (str->free)
    free((void *) str->ptr.p);
  explicit_bzero(str, sizeof(s_str));
}

sw str_cmp (s_str *a, s_str *b)
{
  assert(a);
  assert(b);
  if (a == b)
    return 0;
  if (a->bytes < b->bytes)
    return -1;
  if (a->bytes > b->bytes)
    return 1;
  return memcmp(a->ptr.p, b->ptr.p, a->bytes);
}

s_str * str_cpy (uw bytes, const s8 *p)
{
  s8 *a;
  s_str *str;
  a = malloc(bytes);
  if (! a)
    err(1, "out of memory");
  memcpy(a, p, bytes);
  str = str_new(true, bytes, a);
  return str;
}

void str_delete (s_str *str)
{
  str_clean(str);
  free(str);
}

s_str * str_dup (s_str *src)
{
  s8 *n;
  s_str *str;
  assert(src);
  n = malloc(src->bytes);
  memcpy(n, src->ptr.p, src->bytes);
  str = str_new(true, src->bytes, n);
  return str;
}


s_str * str_empty ()
{
  s_str *str;
  str = str_new(true, 0, NULL);
  return str;
}

s_str * str_f (const char *fmt, ...)
{
  va_list ap;
  s_str *str;
  va_start(ap, fmt);
  str = str_vf(fmt, ap);
  va_end(ap);
  return str;
}

sw str_fputs (s_str *str, FILE *fp)
{
  size_t r;
  r = fwrite(str->ptr.p, str->bytes, 1, fp);
  if (r == 1)
    return str->bytes;
  return -1;
}

void str_init (s_str *str, bool free, uw bytes, const s8 *p)
{
  assert(str);
  str->free = free;
  str->bytes = bytes;
  str->ptr.p = p;
}

void str_init_1 (s_str *str, bool free, const s8 *p)
{
  assert(str);
  str->free = free;
  str->bytes = strlen(p);
  str->ptr.p = p;
}

void str_init_dup (s_str *str, s_str *src)
{
  assert(str);
  assert(src);
  str->free = 1;
  str->bytes = src->bytes;
  str->ptr.p = malloc(src->bytes);
  memcpy((void *) str->ptr.p, src->ptr.p, str->bytes);
}


s_str * str_inspect (s_str *src)
{
  s_str quote;
  s_str *str;
  str_init(&quote, false, 1, "\"");
  str = str_join(3, &quote, src, &quote);
  return str;
}

s_str * str_join (uw count, ...)
{
  va_list ap;
  s_str *str;
  va_start(ap, count);
  str = str_join_v(count, ap);
  va_end(ap);
  return str;
}

s_str * str_join_v (uw count, va_list ap)
{
  s_str *str;
  str = str_new(false, 0, NULL);
  str_init_join_v(str, count, ap);
  return str;
}

s_str * str_n (size_t len, const char *s, bool free)
{
  s_str *str = str_new(free, len, s);
  return str;
}

s_str * str_new (bool free, uw bytes, const s8 *p)
{
  s_str *str;
  str = malloc(sizeof(s_str));
  if (! str)
    err(1, "out of memory");
  str_init(str, free, bytes, p);
  return str;
}

sw str_puts (s_str *str)
{
  return str_fputs(str, stdout);
}

character str_read_character (s_str *str)
{
  character c;
  sw bytes;
  c = str_to_character(str);
  if (c < 0)
    return -1;
  bytes = character_utf8_bytes(c);
  if (bytes < 0)
    return -1;
  str->bytes -= bytes;
  str->ptr.p = (s8 *) str->ptr.p + bytes;
  return c;
}

void str_resize (s_str *str, uw bytes)
{
  str_clean(str);
  str_init(str, true, bytes, calloc(bytes, 1));
}

character str_to_character (s_str *str)
{
  assert(str);
  u8 *b;
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
  if (str->bytes <= 0)
    return -1;
  b = (u8 *) str->ptr.p;
  if ((b[0] & _10000000) == 0)
    return *b;
  if ((b[0] & _11100000) == _11000000) {
    if (str->bytes < 2)
      return -1;
    if ((b[1] & _11000000) != _10000000)
      return -1;
    x[0] = b[0] & _00011111;
    x[1] = b[1] & _00111111;
    return (x[0] << 6) | x[1];
  }
  if ((b[0] & _11110000) == _11100000) {
    if (str->bytes < 3)
      return -1;
    if ((b[1] & _11000000) != _10000000)
      return -1;
    if ((b[2] & _11000000) != _10000000)
      return -1;
    x[0] = b[0] & _00001111;
    x[1] = b[1] & _00111111;
    x[2] = b[2] & _00111111;
    return (x[0] << 12) | (x[1] << 6) | x[2];
  }
  if ((b[0] & _11111000) == _11110000) {
    if (str->bytes < 4)
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
    return (x[0] << 18) | (x[1] << 12) | (x[2] << 6) | x[3];
  }
  return -1;
}

s_str * str_vf (const char *fmt, va_list ap)
{
  int len;
  char *s;
  s_str *str;
  len = vasprintf(&s, fmt, ap);
  if (len < 0)
    err(1, "vasprintf");
  str = str_new(true, len, s);
  return str;
}
