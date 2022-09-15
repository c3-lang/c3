/* c3
 * Copyright 2022 Thomas de Grivel <thoxdg@gmail.com>
 */
#include <assert.h>
#include <stdlib.h>
#include "buf.h"
#include "buf_inspect.h"
#include "tuple.h"
#include "tag.h"

void tuple_clean (s_tuple *tuple)
{
  uw i;
  assert(tuple);
  i = tuple->count;
  while (i--)
    tag_clean(&tuple->tag[i]);
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
  assert(tuple);
  assert(2 <= count);
  tuple->count = count;
  tuple->tag = calloc(count, sizeof(s_tag));
  if (! tuple->tag)
    err(1, "tuple_init: out of memory");
  i = count;
  while (i--)
    tag_init_void(tuple->tag + i);
  return tuple;
}

s_str * tuple_inspect (const s_tuple *x)
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
  return buf_to_str_new(&buf);
 error:
  buf_clean(&buf);
  return NULL;
}

s_tuple * tuple_new (uw count)
{
  s_tuple *t;
  t = malloc(sizeof(s_tuple));
  if (! t)
    errx(1, "tuple_new: out of memory");
  return tuple_init(t, count);
}
