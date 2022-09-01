/* c3
 * Copyright 2022 Thomas de Grivel <thoxdg@gmail.com>
 */
#include <assert.h>
#include <stdlib.h>
#include <string.h>
#include "buf.h"
#include "character.h"
#include "str.h"
#include "sym.h"

sw buf_parse_bool (s_buf *buf, bool *p)
{
  sw r;
  assert(buf);
  if ((r = buf_read_1(buf, "true")) > 0)
    *p = true;
  if (r != 0)
    return r;
  if ((r = buf_read_1(buf, "false")) > 0)
    *p = false;
  return r;
}
