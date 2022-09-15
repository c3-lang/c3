/* c3
 * Copyright 2022 Thomas de Grivel <thoxdg@gmail.com>
 */
#include <assert.h>
#include <stdlib.h>
#include "buf.h"
#include "buf_inspect.h"
#include "list.h"
#include "tag.h"
#include "tuple.h"

void list_clean (s_list *list)
{
  if (list) {
    tag_clean(&list->tag);
    tag_clean(&list->next);
  }
}

void list_delete (s_list *list)
{
  list_clean(list);
  free(list);
}

s_list * list_init (s_list *list)
{
  assert(list);
  tag_init_void(&list->tag);
  tag_init_list(&list->next, NULL);
  return list;
}

s_str * list_inspect (const s_list *x)
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
  return buf_to_str_new(&buf);
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
  s_list *l;
  l = malloc(sizeof(s_list));
  if (! l)
    errx(1, "list_new: out of memory");
  return list_init(l);
}

s_tuple * list_to_tuple_reverse (const s_list *list, s_tuple *dest)
{
  sw i;
  i = list_length(list);
  tuple_init(dest, i);
  while (i--) {
    tag_copy(&list->tag, dest->tag + i);
    list = list_next(list);
  }
  return dest;
}
