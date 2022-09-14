/* c3
 * Copyright 2022 Thomas de Grivel <thoxdg@gmail.com>
 */
#include <assert.h>
#include <stdlib.h>
#include <string.h>
#include "buf.h"
#include "buf_parse.h"
#include "buf_save.h"
#include "character.h"
#include "ident.h"
#include "list.h"
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

sw buf_parse_character (s_buf *buf, character *dest)
{
  character c;
  sw r;
  sw r1;
  sw r2;
  s_buf_save save;
  assert(buf);
  assert(dest);
  buf_save_init(buf, &save);
  if ((r = buf_read_1(buf, "'")) <= 0)
    goto clean;
  if ((r1 = buf_parse_str_character(buf, &c)) <= 0) {
    r = r1;
    goto restore;
  }
  if ((r2 = buf_read_1(buf, "'")) <= 0) {
    r = r2;
    goto restore;
  }
  *dest = c;
  return r + r1 + r2;
 restore:
  buf_save_restore(buf, &save);
 clean:
  buf_save_clean(buf, &save);
  return r;
}

sw buf_parse_digit_hex (s_buf *buf, u8 *dest)
{
  character c;
  sw r;
  assert(buf);
  assert(dest);
  if ((r = buf_peek_character_utf8(buf, &c)) <= 0)
    return r;
  if (c >= '0' && c <= '9')
    *dest = c - '0';
  else if (c >= 'a' && c <= 'f')
    *dest = c - 'a' + 10;
  else if (c >= 'A' && c <= 'F')
    *dest = c - 'A' + 10;
  else
    return 0;
  r = buf_ignore(buf, r);
  return r;
}

sw buf_parse_digit_oct (s_buf *buf, u8 *dest)
{
  character c;
  sw r;
  assert(buf);
  assert(dest);
  if ((r = buf_peek_character_utf8(buf, &c)) <= 0)
    return r;
  if (c >= '0' && c <= '7')
    *dest = c - '0';
  else
    return 0;
  r = buf_ignore(buf, r);
  return r;
}

sw buf_parse_digit_bin (s_buf *buf, u8 *dest)
{
  character c;
  sw r;
  assert(buf);
  assert(dest);
  if ((r = buf_peek_character_utf8(buf, &c)) <= 0)
    return r;
  if (c == '0' || c == '1')
    *dest = c - '0';
  else
    return 0;
  r = buf_ignore(buf, r);
  return r;
}

sw buf_parse_digit_dec (s_buf *buf, u8 *dest)
{
  character c;
  sw r;
  assert(buf);
  assert(dest);
  if ((r = buf_peek_character_utf8(buf, &c)) <= 0)
    return r;
  if (c >= '0' && c <= '9')
    *dest = c - '0';
  else
    return 0;
  r = buf_ignore(buf, r);
  return r;
}

sw buf_parse_ident (s_buf *buf, s_ident *dest)
{
  character c;
  sw csize;
  sw r;
  sw result = 0;
  s_buf_save save;
  s_str str;
  s_buf tmp;
  assert(buf);
  assert(dest);
  buf_save_init(buf, &save);
  if ((r = buf_peek_1(buf, "_\"")) < 0)
    goto clean;
  if (r > 0) {
    if ((r = buf_read_1(buf, "_")) < 0)
      goto clean;
    result += r;
    if ((r = buf_parse_str(buf, &str)) < 0)
      goto restore;
    result += r;
    str_to_ident(&str, dest);
    str_clean(&str);
    r = result;
    goto clean;
  }
  if ((r = buf_peek_character_utf8(buf, &c)) < 0)
    goto clean;
  if (r > 0 && ! ident_first_character_is_reserved(c)) {
    csize = r;
    BUF_INIT_ALLOCA(&tmp, IDENT_MAX);
    if ((r = buf_xfer(&tmp, buf, csize)) < 0)
      goto restore;
    result += csize;
    while ((r = buf_peek_character_utf8(buf, &c)) > 0 &&
           ! ident_character_is_reserved(c)) {
      csize = r;
      if ((r = buf_xfer(&tmp, buf, csize)) != csize)
        goto restore;
      result += csize;
    }
    if (r < 0)
      goto restore;
    buf_read_to_str(&tmp, &str);
    str_to_ident(&str, dest);
    str_clean(&str);
    r = result;
    goto clean;
  }
  r = 0;
  goto clean;
 restore:
  buf_save_restore(buf, &save);
 clean:
  buf_save_clean(buf, &save);
  return r;
}

/*
sw buf_parse_integer (s_buf *buf, s_integer *dest)
{
  character c;
  sw csize;
  mp_err err;
  e_bool negative = false;
  const mp_digit radix = 10;
  mp_digit digit;
  mp_zero(dest->mp_int);
  if (radix < 2 || radix > 64)
    return -1;
  if ((r = buf_peek_1(buf, "-")) > 0) {
    if ((r = buf_read_1(buf, "-")) != 1)
      return r;
    negative = true;
  }
  mp_zero(dest->mp_int);
  if ((err = mp_mul_d(dest->mp_int, radix, a)) != MP_OKAY) {
         return err;
      }
      if ((err = mp_add_d(a, (mp_digit)y, a)) != MP_OKAY) {
         return err;
      }
      ++str;
   }
   if (!((*str == '\0') || (*str == '\r') || (*str == '\n'))) {
      mp_zero(a);
      return MP_VAL;
   }
   if (!MP_IS_ZERO(a)) {
      a->sign = neg;
   }
   return MP_OKAY;
}
*/

sw buf_parse_list (s_buf *buf, s_list **list)
{
  s_list **i;
  sw r;
  sw result = 0;
  s_buf_save save;
  buf_save_init(buf, &save);
  if ((r = buf_read_1(buf, "[")) <= 0)
    goto clean;
  result += r;
  i = list;
  *i = NULL;
  while (1) {
    if ((r = buf_ignore_spaces(buf)) < 0)
      goto restore;
    result += r;
    if ((r = buf_read_1(buf, "]")) < 0)
      goto restore;
    if (r > 0) {
      result += r;
      r = result;
      goto clean;
    }
    *i = list_new();
    if ((r = buf_parse_tag(buf, &(*i)->tag)) <= 0)
      goto restore;
    result += r;
    if ((r = buf_ignore_spaces(buf)) < 0)
      goto restore;
    result += r;
    if ((r = buf_read_1(buf, "]")) < 0)
      goto restore;
    if (r > 0) {
      result += r;
      r = result;
      goto clean;
    }
    if ((r = buf_read_1(buf, ",")) < 0)
      goto restore;
    if (r > 0) {
      result += r;
      i = &(*i)->next.data.list;
      continue;
    }
    if ((r = buf_read_1(buf, "|")) < 0)
      goto restore;
    if (r > 0) {
      result += r;
      if ((r = buf_ignore_spaces(buf)) < 0)
        goto restore;
      result += r;
      if ((r = buf_parse_tag(buf, &(*i)->next)) <= 0)
        goto restore;
      result += r;
      if ((r = buf_ignore_spaces(buf)) < 0)
        goto restore;
      result += r;
      if ((r = buf_read_1(buf, "]")) <= 0)
        goto restore;
      result += r;
      r = result;
      goto clean;
    }
    goto restore;
  }
  result += r;
  if ((r = buf_read_1(buf, "]")) <= 0)
    goto restore;
  result += r;
  r = result;
  goto clean;
 restore:
  buf_save_restore(buf, &save);
 clean:
  buf_save_clean(buf, &save);
  return r;
}

sw buf_parse_str (s_buf *buf, s_str *dest)
{
  u8 b;
  character c;
  sw r;
  sw result = 0;
  s_buf_save save;
  s_buf tmp;
  assert(buf);
  assert(dest);
  buf_save_init(buf, &save);
  if ((r = buf_read_1(buf, "\"")) <= 0)
    goto save_clean;
  result += r;
  buf_init_alloc(&tmp, STR_MAX);
  while (1) {
    if ((r = buf_read_1(buf, "\"")) < 0)
      goto restore;
    if (r > 0) {
      result += r;
      break;
    }
    if ((r = buf_parse_str_character(buf, &c)) <= 0) {
      if ((r = buf_parse_str_u8(buf, &b)) <= 0)
        goto restore;
      result += r;
      if ((r = buf_write_u8(&tmp, b)) < 0)
        goto restore;
      continue;
    }
    result += r;
    if ((r = buf_write_character_utf8(&tmp, c)) < 0)
      goto restore;
  }
  buf_read_to_str(&tmp, dest);
  r = result;
  goto clean;
 restore:
  buf_save_restore(buf, &save);
 clean:
  buf_clean(&tmp);
 save_clean:
  buf_save_clean(buf, &save);
  return r;
}

sw buf_parse_str_character (s_buf *buf, character *dest)
{
  character c;
  sw csize;
  sw r;
  sw r1 = 0;
  sw r2 = 0;
  s_buf_save save;
  buf_save_init(buf, &save);
  if ((r = buf_peek_1(buf, "\\x")) < 0)
    goto clean;
  if (r > 0) {
    r = 0;
    goto clean;
  }
  if ((r = buf_read_1(buf, "\\")) < 0)
    goto clean;
  if (r > 0) {
    if ((r1 = buf_read_character_utf8(buf, &c)) <= 0) {
      r = r1;
      goto restore;
    }
    switch (c) {
    case '0': c = 0; break;
    case 'U': case 'u':
      if ((r2 = buf_parse_str_character_unicode(buf, &c)) < 0) {
        r = r2;
        goto restore;
      }
      break;
    case 'n': c = '\n'; break;
    case 'r': c = '\r'; break;
    case 's': c = ' '; break;
    case 't': c = '\t'; break;
    case 'v': c = '\v'; break;
    default: ;
    }
    *dest = c;
    return r + r1 + r2;
  }
  if ((r = buf_peek_character_utf8(buf, &c)) < 0)
    goto clean;
  if (c == '"') {
    r = 0;
    goto clean;
  }
  csize = r;
  if ((r = buf_ignore(buf, csize)) < 0)
    goto clean;
  *dest = c;
  goto clean;
 restore:
  buf_save_restore(buf, &save);
 clean:
  buf_save_clean(buf, &save);
  return r;
}

sw buf_parse_str_character_unicode (s_buf *buf, character *dest)
{
  sw r;
  sw result = 0;
  u64 tmp;
  s_buf_save save;
  buf_save_init(buf, &save);
  if ((r = buf_read_1(buf, "+")) <= 0)
    goto clean;
  result += r;
  if ((r = buf_parse_u64_hex(buf, &tmp)) <= 0) {
    buf_save_restore(buf, &save);
    goto clean;
  }
  *dest = (character) tmp;
  result += r;
  r = result;
 clean:
  buf_save_clean(buf, &save);
  return r;
}

sw buf_parse_str_u8 (s_buf *buf, u8 *dest)
{
  u8 digit[3];
  sw r;
  sw result = 0;
  s_buf_save save;
  buf_save_init(buf, &save);
  if ((r = buf_read_1(buf, "\\")) < 0)
    goto clean;
  if (r > 0) {
    result += r;
    if ((r = buf_read_1(buf, "x")) < 0)
      goto restore;
    if (r > 0) {
      result += r;
      if ((r = buf_parse_digit_hex(buf, &digit[0])) < 0)
        goto restore;
      if (r == 0)
        goto read_u8;
      result += r;
      if ((r = buf_parse_digit_hex(buf, &digit[1])) < 0)
        goto restore;
      if (r == 0)
        goto read_u8;
      result += r;
      *dest = digit[0] * 16 + digit[1];
      r = result;
      goto clean;
    }
    if ((r = buf_parse_digit_oct(buf, &digit[0])) < 0)
      goto restore;
    if (r == 0)
      goto read_u8;
    result += r;
    if ((r = buf_parse_digit_oct(buf, &digit[1])) < 0)
      goto restore;
    if (r == 0)
      goto read_u8;
    result += r;
    if ((r = buf_parse_digit_oct(buf, &digit[2])) < 0)
      goto restore;
    if (r == 0)
      goto read_u8;
    result += r;
    *dest = digit[0] * 64 + digit[1] * 8 + digit[2];
    r = result;
    goto clean;
  read_u8:
    buf_save_restore(buf, &save);
  }
  r = buf_read_u8(buf, dest);
  goto clean;
 restore:
  buf_save_restore(buf, &save);
 clean:
  buf_save_clean(buf, &save);
  return r;
}

sw buf_parse_sym (s_buf *buf, const s_sym **dest)
{
  s_buf tmp;
  character c;
  sw csize;
  sw r;
  sw result = 0;
  s_buf_save save;
  s_str str;
  assert(buf);
  assert(dest);
  buf_save_init(buf, &save);
  if ((r = buf_peek_1(buf, ":\"")) < 0)
    goto clean;
  if (r > 0) {
    if ((r = buf_read_1(buf, ":")) <= 0)
      goto clean;
    result += r;
    if ((r = buf_parse_str(buf, &str)) <= 0)
      goto restore;
    *dest = str_to_sym(&str);
    str_clean(&str);
    result += r;
    r = result;
    goto clean;
  }
  if ((r = buf_peek_character_utf8(buf, &c)) <= 0)
    goto clean;
  if (c == ':' || character_is_uppercase(c)) {
    csize = r;
    BUF_INIT_ALLOCA(&tmp, SYM_MAX);
    if (c == ':') {
      if ((r = buf_ignore(buf, csize)) < 0)
        goto clean;
      if ((r = buf_peek_character_utf8(buf, &c)) <= 0 ||
          sym_character_is_reserved(c))
        goto restore;
    }
    else {
      if ((r = buf_xfer(&tmp, buf, csize)) < 0)
        goto clean;
    }
    result += csize;
    while ((r = buf_peek_character_utf8(buf, &c)) > 0 &&
           ! sym_character_is_reserved(c)) {
      csize = r;
      if ((r = buf_xfer(&tmp, buf, csize)) < 0)
        goto restore;
      result += csize;
    }
    if (r < 0)
      goto restore;
    buf_read_to_str(&tmp, &str);
    *dest = str_to_sym(&str);
    str_clean(&str);
    r = result;
    goto clean;
  }
  if (r > 0)
    r = 0;
  goto clean;
 restore:
  buf_save_restore(buf, &save);
 clean:
  buf_save_clean(buf, &save);
  return r;
}

sw buf_parse_tag (s_buf *buf, s_tag *dest)
{
  sw r;
  assert(buf);
  assert(dest);
  (void) ((r = buf_parse_tag_character(buf, dest)) != 0 ||
          (r = buf_parse_tag_list(buf, dest)) != 0 ||
          (r = buf_parse_tag_str(buf, dest)) != 0 ||
          (r = buf_parse_tag_sym(buf, dest)) != 0 ||
          (r = buf_parse_tag_ident(buf, dest)));
  return r;
}

sw buf_parse_tag_bool (s_buf *buf, s_tag *dest)
{
  sw r;
  if ((r = buf_parse_bool(buf, &dest->data.bool)) > 0)
    dest->type.type = TAG_BOOL;
  return r;
}

sw buf_parse_tag_character (s_buf *buf, s_tag *dest)
{
  sw r;
  if ((r = buf_parse_character(buf, &dest->data.character)) > 0)
    dest->type.type = TAG_CHARACTER;
  return r;
}

sw buf_parse_tag_ident (s_buf *buf, s_tag *dest)
{
  sw r;
  if ((r = buf_parse_ident(buf, &dest->data.ident)) > 0)
    dest->type.type = TAG_IDENT;
  return r;
}

sw buf_parse_tag_list (s_buf *buf, s_tag *dest)
{
  sw r;
  if ((r = buf_parse_list(buf, &dest->data.list)) > 0)
    dest->type.type = TAG_LIST;
  return r;
}

sw buf_parse_tag_str (s_buf *buf, s_tag *dest)
{
  sw r;
  if ((r = buf_parse_str(buf, &dest->data.str)) > 0)
    dest->type.type = TAG_STR;
  return r;
}

sw buf_parse_tag_sym (s_buf *buf, s_tag *dest)
{
  sw r;
  if ((r = buf_parse_sym(buf, &dest->data.sym)) > 0)
    dest->type.type = TAG_SYM;
  return r;
}

sw buf_parse_u64_dec (s_buf *buf, u64 *dest)
{
 sw r;
  sw result = 0;
  u8 digit;
  s_buf_save save;
  u64 tmp = 0;
  buf_save_init(buf, &save);
  while ((r = buf_parse_digit_dec(buf, &digit)) > 0) {
    tmp = tmp * 10 + digit;
    result += r;
  }
  if (r < 0) {
    buf_save_restore(buf, &save);
    goto clean;
  }
  *dest = tmp;
  r = result;
 clean:
  buf_save_clean(buf, &save);
  return r;
}

sw buf_parse_u64_hex (s_buf *buf, u64 *dest)
{
 sw r;
  sw result = 0;
  u8 digit;
  s_buf_save save;
  u64 tmp = 0;
  buf_save_init(buf, &save);
  while ((r = buf_parse_digit_hex(buf, &digit)) > 0) {
    tmp = tmp * 16 + digit;
    result += r;
  }
  if (r < 0) {
    buf_save_restore(buf, &save);
    goto clean;
  }
  *dest = tmp;
  r = result;
 clean:
  buf_save_clean(buf, &save);
  return r;
}
