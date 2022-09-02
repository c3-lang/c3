/* c3
 * Copyright 2022 Thomas de Grivel <thoxdg@gmail.com>
 */
#include <assert.h>
#include <stdlib.h>
#include <string.h>
#include "buf.h"
#include "ident.h"
#include "limits.h"
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

sw buf_parse_ident (s_buf *buf, s_ident *ident)
{
  s_buf tmp;
  character c;
  sw r;
  sw result = 0;
  s_str str;
  assert(buf);
  assert(ident);
  buf_init_alloc(&tmp, IDENT_MAX);
  if ((r = buf_peek_1(buf, "~i\"")) > 0) {
    result = buf_read_1(buf, "~i");
    if ((r = buf_parse_str(buf, &str)) > 0) {
      result += r;
      str_to_ident(&str, ident);
      return result;
    }
  }
  while ((r = buf_peek_character(buf, &c)) > 0 &&
         ! ident_character_is_reserved(c)) {
    if (buf_read_character(buf, &c) != r)
      return -1;
    if (buf_write_character(&tmp, c) != r)
      return -1;
  }
  if (r != 0)
    return r;
  str_init(&str, tmp.ptr.p, tmp.size, tmp.ptr.p);
  str_to_ident(&str, ident);
  return r;
}
