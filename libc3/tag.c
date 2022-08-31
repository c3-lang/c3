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

void tag_clean (s_tag *tag)
{
  assert(tag);
  switch (tag->type.type) {
  case TAG_STR: str_clean(&tag->data.str); break;
  default:
    ;
  }
}

void tag_delete (s_tag *tag)
{
  tag_clean(tag);
  free(tag);
}
