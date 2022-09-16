/* c3
 * Copyright 2022 Thomas de Grivel <thoxdg@gmail.com>
 */
#include <assert.h>
#include <err.h>
#include <stdlib.h>
#include "integer.h"
#include "list.h"
#include "str.h"
#include "tag.h"
#include "tuple.h"

void tag_clean (s_tag *tag)
{
  assert(tag);
  switch (tag->type.type) {
  case TAG_LIST:  list_delete(tag->data.list);   break;
  case TAG_STR:   str_clean(&tag->data.str);     break;
  case TAG_TUPLE: tuple_clean(&tag->data.tuple); break;
  default: ;
  }
}

s_tag * tag_copy (const s_tag *src, s_tag *dest)
{
  assert(src);
  assert(dest);
  switch (src->type.type) {
  case TAG_LIST: list_copy(src->data.list, &dest->data.list); break;
  case TAG_STR: str_copy(&src->data.str, &dest->data.str); break;
  default: dest->data = src->data;
  }
  dest->type.type = src->type.type;
  return dest;
}

s_tag * tag_init_bool (s_tag *tag, bool b)
{
  assert(tag);
  tag->type.type = TAG_BOOL;
  tag->data.bool = b;
  return tag;
}

s_tag * tag_init_character (s_tag *tag, character c)
{
    assert(tag);
    tag->type.type = TAG_CHARACTER;
    tag->data.character = c;
    return tag;
}

s_tag * tag_init_f64 (s_tag *tag, f64 f)
{
    assert(tag);
    tag->type.type = TAG_F64;
    tag->data.f64 = f;
    return tag;
}

s_tag * tag_init_integer (s_tag *tag)
{
  assert(tag);
  tag->type.type = TAG_INTEGER;
  integer_init(&tag->data.integer);
  return tag;
}

s_tag * tag_init_list (s_tag *tag, s_list *list)
{
  assert(tag);
  tag->type.type = TAG_LIST;
  tag->data.list = list;
  return tag;
}

s_tag * tag_init_s64 (s_tag *tag, s64 i)
{
  assert(tag);
  tag->type.type = TAG_S64;
  tag->data.s64 = i;
  return tag;
}

s_tag * tag_init_str (s_tag *tag, s8 * free, uw size, const s8 *p)
{
  assert(tag);
  tag->type.type = TAG_STR;
  str_init(&tag->data.str, free, size, p);
  return tag;
}

s_tag * tag_init_sym (s_tag *tag, const s_sym *p)
{
  assert(tag);
  assert(p);
  tag->type.type = TAG_SYM;
  tag->data.sym = p;
  return tag;
}

s_tag * tag_init_u64 (s_tag *tag, u64 i)
{
  assert(tag);
  tag->type.type = TAG_U64;
  tag->data.u64 = i;
  return tag;
}

s_tag * tag_init_void (s_tag *tag)
{
  assert(tag);
  tag->type.type = TAG_VOID;
  return tag;
}

void tag_delete (s_tag *tag)
{
  tag_clean(tag);
  free(tag);
}
