/* c3
 * Copyright 2022,2023 kmx.io <contact@kmx.io>
 *
 * Permission is hereby granted to use this software granted the above
 * copyright notice and this permission paragraph are included in all
 * copies and substantial portions of this software.
 *
 * THIS SOFTWARE IS PROVIDED "AS-IS" WITHOUT ANY GUARANTEE OF
 * PURPOSE AND PERFORMANCE. IN NO EVENT WHATSOEVER SHALL THE
 * AUTHOR BE CONSIDERED LIABLE FOR THE USE AND PERFORMANCE OF
 * THIS SOFTWARE.
 */
#include "assert.h"
#include <stdlib.h>
#include <string.h>
#include "buf.h"
#include "buf_inspect.h"
#include "buf_parse.h"
#include "io.h"
#include "tuple.h"
#include "tag.h"

void tuple_clean (s_tuple *tuple)
{
  uw i;
  assert(tuple);
  i = tuple->count;
  while (i--)
    tag_clean(tuple->tag + i);
  free(tuple->tag);
}

void tuple_delete (s_tuple *tuple)
{
  tuple_clean(tuple);
  free(tuple);
}

s_tuple * tuple_init (s_tuple *tuple, uw count)
{
  uw i;
  s_tuple tmp;
  assert(tuple);
  assert(2 <= count);
  tmp.count = count;
  tmp.tag = calloc(count, sizeof(s_tag));
  if (! tmp.tag) {
    err_puts("tuple_init: failed to allocate memory");
    assert(! "tuple_init: failed to allocate memory");
    return NULL;
  }
  i = count;
  while (i--)
    tag_init_void(tmp.tag + i);
  *tuple = tmp;
  return tuple;
}

s_tuple * tuple_init_1 (s_tuple *tuple, const s8 *p)
{
  s_buf buf;
  uw len;
  sw r;
  assert(tuple);
  assert(p);
  len = strlen(p);
  buf_init(&buf, false, len, (s8 *) p);
  buf.wpos = len;
  r = buf_parse_tuple(&buf, tuple);
  if (r < 0 || (uw) r != len) {
    err_write_1("tuple_init_1: invalid tuple: \"");
    err_write_1(p);
    err_write_1("\", ");
    err_inspect_uw(&len);
    err_write_1(" != ");
    err_inspect_sw(&r);
    err_write_1("\n");
    assert(! "tuple_init_1: invalid tuple");
    return NULL;
  }
  return tuple;
}

s_tuple * tuple_init_2 (s_tuple *tuple, const s_tag *a, const s_tag *b)
{
  tuple_init(tuple, 2);
  tag_init_copy(tuple->tag + 0, a);
  tag_init_copy(tuple->tag + 1, b);
  return tuple;
}

s_tuple * tuple_init_cast (s_tuple *tuple, const s_tag *tag)
{
  switch (tag->type) {
  case TAG_TUPLE:
    return tuple_init_copy(tuple, &tag->data.tuple);
  default:
    break;
  }
  err_write_1("tuple_init_cast: cannot cast ");
  err_write_1(tag_type_to_string(tag->type));
  err_puts(" to Tuple");
  assert(! "tuple_init_cast: cannot cast to Tuple");
  return NULL;
}

s_tuple * tuple_init_copy (s_tuple *tuple, const s_tuple *src)
{
  uw i = 0;
  assert(src);
  assert(tuple);
  tuple_init(tuple, src->count);
  while (i < src->count) {
    tag_init_copy(tuple->tag + i, src->tag + i);
    i++;
  }
  return tuple;
}

s_str * tuple_inspect (const s_tuple *x, s_str *dest)
{
  s_buf buf;
  sw r;
  sw size;
  size = buf_inspect_tuple_size(x);
  buf_init_alloc(&buf, size);
  if ((r = buf_inspect_tuple(&buf, x)) < 0)
    goto error;
  assert(r == size);
  if (r != size)
    goto error;
  return buf_to_str(&buf, dest);
 error:
  buf_clean(&buf);
  return NULL;
}

s_tuple * tuple_new (uw count)
{
  s_tuple *tuple;
  tuple = malloc(sizeof(s_tuple));
  if (! tuple) {
    err_puts("tuple_new: failed to allocate memory");
    assert(! "tuple_new: failed to allocate memory");
    return NULL;
  }
  if (! tuple_init(tuple, count)) {
    free(tuple);
    return NULL;
  }
  return tuple;
}

s_tuple * tuple_new_1 (const s8 *p)
{
  s_tuple *tuple;
  tuple = malloc(sizeof(s_tuple));
  if (! tuple) {
    err_puts("tuple_new_1: failed to allocate memory");
    assert(! "tuple_new_1: failed to allocate memory");
    return NULL;
  }
  if (! tuple_init_1(tuple, p)) {
    free(tuple);
    return NULL;
  }
  return tuple;
}
