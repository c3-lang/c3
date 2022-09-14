/* c3
 * Copyright 2022 Thomas de Grivel <thoxdg@gmail.com>
 */
#include "buf.h"
#include "buf_inspect.h"

s_str * bool_inspect (bool x, s_str *dest)
{
  sw size;
  s_buf tmp;
  size = buf_inspect_bool_size(x);
  if (size < 0) {
    assert(! "bool_inspect: error");
    return NULL;
  }
  buf_init_alloc(&tmp, size);
  buf_inspect_bool(&tmp, x);
  assert(tmp.wpos == tmp.size);
  if (tmp.wpos != tmp.size) {
    buf_clean(&tmp);
    return NULL;
  }
  return buf_to_str(&tmp, dest);
}
