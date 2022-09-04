/* c3
 * Copyright 2022 Thomas de Grivel <thoxdg@gmail.com>
 */
#include <assert.h>
#include <stdlib.h>
#include <string.h>
#include "buf.h"
#include "buf_parse.h"
#include "ident.h"
#include "limits.h"
#include "str.h"
#include "sym.h"

sw buf_parse_str_character (s_buf *buf, character *dest);

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

sw buf_parse_character (s_buf *buf, character *dest)
{
  character c;
  sw r;
  sw r1;
  sw r2;
  s_buf save;
  assert(buf);
  assert(dest);
  buf_save(buf, &save);
  if ((r = buf_read_1(buf, "'")) > 0 &&
      (r1 = buf_parse_str_character(buf, &c)) &&
      (r2 = buf_read_1(buf, "'")) > 0) {
    *dest = c;
    return r + r1 + r2;
  }
  buf_restore(buf, &save);
  return 0;
}

sw buf_parse_ident (s_buf *buf, s_ident *ident)
{
  s_buf tmp;
  character c;
  sw r;
  sw r1;
  s_buf save;
  s_str str;
  assert(buf);
  assert(ident);
  buf_init_alloc(&tmp, IDENT_MAX);
  buf_save(buf, &save);
  if ((r = buf_read_1(buf, "~i")) > 0 &&
      (r1 = buf_parse_str(buf, &str)) > 0) {
      str_to_ident(&str, ident);
      return r + r1;
  }
  while ((r = buf_peek_character_utf8(buf, &c)) > 0 &&
         ! ident_character_is_reserved(c)) {
    if (buf_read_character_utf8(buf, &c) != r)
      return -1;
    if (buf_write_character_utf8(&tmp, c) != r)
      return -1;
  }
  if (r != 0)
    return r;
  str_init(&str, tmp.ptr.p, tmp.size, tmp.ptr.p);
  str_to_ident(&str, ident);
  return r;
}

sw buf_parse_str_character (s_buf *buf, character *dest)
{
  character c;
  u8 digit[2];
  sw r;
  sw r1;
  sw r2;
  s_buf save;
  buf_save(buf, &save);
  if ((r = buf_read_1(buf, "\\x")) > 0 &&
      (r1 = buf_parse_digit_hex(buf, &digit[0])) > 0 &&
      (r2 = buf_parse_digit_hex(buf, &digit[1])) > 0) {
    *dest = digit[0] * 16 + digit[1];
    return r + r1 + r2;
  }
  buf_restore(buf, &save);
  if ((r = buf_read_1(buf, "\\")) > 0 &&
      (r1 = buf_read_character_utf8(buf, &c)) > 0) {
    switch (r1) {
    case 'n': c = '\n'; break;
    case 'r': c = '\r'; break;
    case 't': c = '\t'; break;
    case 'v': c = '\v'; break;
    default: ;
    }
    *dest = c;
    return r + r1;
  }
  buf_restore(buf, &save);
  return 0;
}
