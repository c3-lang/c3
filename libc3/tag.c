/* c3
 * Copyright 2022 Thomas de Grivel <thoxdg@gmail.com>
 */
#include <assert.h>
#include <err.h>
#include <stdlib.h>
#include "str.h"
#include "tag.h"

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
