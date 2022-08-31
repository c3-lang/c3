/* c3
 * Copyright 2022 Thomas de Grivel <thoxdg@gmail.com>
 */
#include <assert.h>
#include <err.h>
#include <stdlib.h>
#include "str.h"
#include "tag.h"

s_tag * tag_init_bool (s_tag *tag, bool b)
{
  assert(tag);
  tag->type.type = TAG_BOOL;
  tag->data.bool = b;
  return tag;
}

s_tag * tag_init_character (s_tag *tag, character *p)
{
}

s_tag * tag_init_f64 (s_tag *tag, f64 *p)
{
}

s_tag * tag_init_s64 (s_tag *tag, s64 *p)
{
}

s_tag * tag_init_str (s_tag *tag, e_bool free, uw size, s8 *p)
{
  assert(tag);
  assert(src);
  tag->type.type = TAG_STR;
  str_init(&tag->str, free, src->size, src->ptr.p);
  return tag;
}

s_tag * tag_init_sym (s_tag *tag, uw count, const s_sym *p)
{
}

s_tag * tag_init_u64 (s_tag *tag, u64 *p)
{
}

void tag_clean (s_tag *tag)
{
  assert(tag);
  switch (tag->type) {
  case TAG_STR: str_clean(&tag->str); break;
  default:
  }
}

void tag_delete (s_tag *tag)
{
  tag_clean(tag);
  free(tag);
}
