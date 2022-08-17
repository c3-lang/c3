/* str
 * Copyright 2022 Thomas de Grivel <thoxdg@gmail.com>
 */
#include <assert.h>
#include <stdarg.h>
#include <stdlib.h>
#include <string.h>
#include "str.h"

s_str * str_1 (const char *s, bool free)
{
  size_t len = strlen(s);
  s_str *str = str_new(free, len, s);
  return str;
}

void str_clean (s_str *str)
{
  assert(str);
  if (str->free)
    free((void *) str->ptr.p);
  explicit_bzero(str, sizeof(s_str));
}

s_str * str_copy (uw bytes, const s8 *p)
{
  s8 *a;
  a = malloc(bytes);
  if (a) {
    s_str *str;
    memcpy(a, p, bytes);
    str = str_new(true, bytes, a);
    return str;
  }
  return NULL;
}

void str_delete (s_str *str)
{
  str_clean(str);
  free(str);
}

s_str * str_f (const char *fmt, ...)
{
  va_list ap;
  int len;
  char *s;
  s_str *str;
  va_start(ap, fmt);
  len = vasprintf(&s, fmt, ap);
  va_end(ap);
  if (len < 0)
    return NULL;
  str = str_new(true, len, s);
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

s_str * str_n (size_t len, const char *s, bool free)
{
  s_str *str = str_new(free, len, s);
  return str;
}

s_str * str_new (bool free, uw bytes, const s8 *p)
{
  s_str *str;
  str = malloc(sizeof(s_str));
  if (str)
    str_init(str, free, bytes, p);
  return str;
}

sw str_puts (s_str *str)
{
  return str_fputs(str, stdout);
}
