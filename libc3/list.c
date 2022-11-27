/* c3
 * Copyright 2022 kmx.io <contact@kmx.io>
 *
 * Permission is hereby granted to use this software granted
 * the above copyright notice and this permission paragraph
 * are included in all copies and substantial portions of this
 * software.
 *
 * THIS SOFTWARE IS PROVIDED "AS-IS" WITHOUT ANY GUARANTEE OF
 * PURPOSE AND PERFORMANCE. IN NO EVENT WHATSOEVER SHALL THE
 * AUTHOR BE CONSIDERED LIABLE FOR THE USE AND PERFORMANCE OF
 * THIS SOFTWARE.
 */
#include <assert.h>
#include <stdlib.h>
#include "buf.h"
#include "buf_inspect.h"
#include "buf_parse.h"
#include "list.h"
#include "tag.h"
#include "tuple.h"

s_list * list_1 (const char *p)
{
  s_buf buf;
  s_list *list;
  buf_init_1(&buf, p);
  if (buf_parse_list(&buf, &list) <= 0) {
    assert(! "invalid list");
    buf_clean(&buf);
    return NULL;
  }
  buf_clean(&buf);
  return list;
}

void list_clean (s_list *list)
{
  if (list) {
    tag_clean(&list->tag);
    tag_clean(&list->next);
  }
}

/* FIXME: does not work on dotted lists or circular lists */
s8 list_compare (const s_list *a, const s_list *b)
{
  s8 r;
  while (1) {
    if (a == b)
      return 0;
    if (!a)
      return -1;
    if (!b)
      return 1;
    if ((r = tag_compare(&a->tag, &b->tag)))
      return r;
    a = list_next(a);
    b = list_next(b);
  }
}

/* FIXME: does not work on circular lists */
s_list * list_copy (const s_list *src, s_list **dest)
{
  s_list *next;
  s_list *result = NULL;
  *dest = NULL;
  while (src) {
    *dest = list_new();
    if (! result)
      result = *dest;
    tag_copy(&src->tag, &(*dest)->tag);
    if ((next = list_next(src))) {
      src = next;
      dest = &(*dest)->next.data.list;
    }
    else {
      tag_copy(&src->next, &(*dest)->next);
      break;
    }
  }
  return result;
}

void list_delete (s_list *list)
{
  list_clean(list);
  free(list);
}

/* FIXME: dotted lists, circular lists */
t_hash_context * list_hash_update (t_hash_context *context,
                                   const s_list *x)
{
  const s_list *last;
  if (x) {
    while (x) {
      const u8 two = 2;
      hash_update(context, &two, sizeof(two));
      tag_hash_update(context, &x->tag);
      last = x;
      x = list_next(x);
    }
    return tag_hash_update(context, &last->next);
  }
  return context;
}

s_list * list_init (s_list *list)
{
  assert(list);
  tag_init_void(&list->tag);
  tag_init_list(&list->next, NULL);
  return list;
}

s_str * list_inspect (const s_list *x, s_str *dest)
{
  s_buf buf;
  sw r;
  sw size;
  size = buf_inspect_list_size(x);
  buf_init_alloc(&buf, size);
  if ((r = buf_inspect_list(&buf, x)) < 0)
    goto error;
  assert(r == size);
  if (r != size)
    goto error;
  return buf_to_str(&buf, dest);
 error:
  buf_clean(&buf);
  return NULL;
}

sw list_length (const s_list *list)
{
  sw length = 0;
  while (list) {
    length++;
    list = list_next(list);
  }
  return length;
}

s_list * list_next (const s_list *list)
{
  assert(list);
  switch (list->next.type.type) {
  case TAG_LIST: return list->next.data.list;
  default: return NULL;
  }
}

s_list * list_new ()
{
  s_list *list;
  list = calloc(1, sizeof(s_list));
  if (! list)
    errx(1, "list_new: out of memory");
  return list_init(list);
}