/* str
 * Copyright 2022 Thomas de Grivel <thoxdg@gmail.com>
 */
#include <assert.h>
#include <err.h>
#include <stdarg.h>
#include <stdlib.h>
#include <string.h>
#include "str.h"

s_str * str_1 (bool free, const char *s)
{
  size_t len = strlen(s);
  s_str *str = str_new(free, len, s);
  return str;
}

s_str * str_append (s_str *str, ...)
{
  va_list ap;
  s_str *i;
  va_start(ap, str);
  /* TODO: realloc only once */
  while ((i = va_arg(ap, s_str *)))
    str_append_copy(str, i->bytes, i->ptr.p);
  va_end(ap);
  return str;
}

s_str * str_append_copy (s_str *str, uw bytes, const s8 *p)
{
  assert(str);
  if (bytes == 0)
    return str;
  if (str->free)
    return str_append_realloc(str, bytes, p);
  {
    size_t nbytes = str->bytes + bytes;
    char *n = malloc(nbytes);
    if (! n)
      err(1, "out of memory");
    memcpy(n, str->ptr.p, str->bytes);
    memcpy(n + str->bytes, p, bytes);
    str->bytes = nbytes;
    str->ptr.p = n;
  }
  return str;
}

s_str * str_append_1 (s_str *str, const char *s)
{
  size_t len;
  len = strlen(s);
  return str_append(str, len, s);
}

s_str * str_append_f (s_str *str, const char *fmt, ...)
{
  va_list ap;
  s_str *f;
  s_str *n;
  va_start(ap, fmt);
  f = str_vf(fmt, ap);
  va_end(ap);
  n = str_append(str, f);
  str_delete(f);
  return n;
}

s_str * str_append_realloc (s_str *str, uw bytes, const s8 *p)
{
  uw nbytes = str->bytes + bytes;
  char *n = realloc((void *) str->ptr.p, nbytes);
  if (! n)
    err(1, "out of memory");
  memcpy(n + str->bytes, p, bytes);
  str->ptr.p = n;
  str->bytes = nbytes;
  return str;
}

s_str * str_append_inspect (s_str *str, s_str *x)
{
  /* TODO */
  (void) str;
  (void) x;
  return NULL;
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


s_str * str_inspect (s_str *str)
{
  s_str *o = str_empty();
  str_append_1(o, "\"");
  /* TODO: \0 and \n */
  str_append(o, str);
  str_append_1(o, "\"");
  return o;
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
