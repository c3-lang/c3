/* c3
 * Copyright 2022 Thomas de Grivel <thoxdg@gmail.com>
 */
#include <assert.h>
#include <err.h>
#include <stdlib.h>
#include "str.h"
#include "tag.h"

s_tag * tag_init (s_tag *tag, e_tag_type type, bool free, uw size,
                  s8 *p)
{
  assert(tag);
  tag->type.type = type;
  str_init(&tag->str, free, size, p);
  return tag;
}

void tag_clean (s_tag *tag)
{
  assert(tag);
  str_clean(&tag->str);
}

void tag_delete (s_tag *tag)
{
  tag_clean(tag);
  free(tag);
}

s_tag * tag_new (e_tag_type type, e_bool free, uw size, s8 *p)
{
  s_tag *tag;
  tag = malloc(sizeof(s_tag));
  if (! tag)
    err(1, "out of memory");
  tag_init(tag, type, free, size, p);
  return tag;
}
