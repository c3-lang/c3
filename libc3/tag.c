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
  tag->type = type;
  tag->str = str_init(&tag->str, free, size, p);
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
