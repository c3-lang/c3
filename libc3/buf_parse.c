/* c3
 * Copyright 2022 Thomas de Grivel <thoxdg@gmail.com>
 */
#include <assert.h>
#include <stdlib.h>
#include <string.h>
#include "buf.h"
#include "buf_parse.h"
#include "character.h"
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

sw buf_parse_digit_hex (s_buf *buf, u8 *dest)
{
  character c;
  sw r;
  assert(buf);
  assert(dest);
  if ((r = buf_peek_character_utf8(buf, &c)) > 0) {
    if (c >= '0' && c <= '9')
      *dest = c - '0';
    else if (c >= 'a' && c <= 'f')
      *dest = c - 'a' + 10;
    else if (c >= 'A' && c <= 'F')
      *dest = c - 'A' + 10;
    else
      return 0;
    buf_read_character_utf8(buf, &c);
    return r;
  }
  return r;
}

sw buf_parse_digit_oct (s_buf *buf, u8 *dest)
{
  character c;
  sw r;
  assert(buf);
  assert(dest);
  if ((r = buf_peek_character_utf8(buf, &c)) > 0) {
    if (c >= '0' && c <= '7')
      *dest = c - '0';
    else
      return 0;
    buf_read_character_utf8(buf, &c);
    return r;
  }
  return r;
}

sw buf_parse_digit_bin (s_buf *buf, u8 *dest)
{
  character c;
  sw r;
  assert(buf);
  assert(dest);
  if ((r = buf_peek_character_utf8(buf, &c)) > 0) {
    if (c == '0' || c == '1')
      *dest = c - '0';
    else
      return 0;
    buf_read_character_utf8(buf, &c);
    return r;
  }
  return r;
}

sw buf_parse_digit_dec (s_buf *buf, u8 *dest)
{
  character c;
  sw r;
  assert(buf);
  assert(dest);
  if ((r = buf_peek_character_utf8(buf, &c)) > 0) {
    if (c >= '0' && c <= '9')
      *dest = c - '0';
    else
      return 0;
    buf_read_character_utf8(buf, &c);
    return r;
  }
  return r;
}

sw buf_parse_ident (s_buf *buf, s_ident *dest)
{
  character c;
  sw csize;
  sw r;
  sw r1 = 0;
  sw result = 0;
  s_buf save;
  s_str str;
  s_buf tmp;
  assert(buf);
  assert(dest);
  if ((r = buf_peek_1(buf, "~i\"")) > 0) {
    if ((r = buf_read_1(buf, "~i")) > 0 &&
        (r1 = buf_parse_str(buf, &str)) > 0) {
      str_to_ident(&str, dest);
      str_clean(&str);
      return r + r1;
    }
    if (r1 < 0) {
      buf_restore(buf, &save);
      return r1;
    }
  }
  if (r < 0) {
    buf_restore(buf, &save);
    return r;
  }
  if ((r = buf_peek_character_utf8(buf, &c)) > 0 &&
      character_is_lowercase(c)) {
    csize = r;
    BUF_INIT_ALLOCA(&tmp, IDENT_MAX);
    if ((r = buf_xfer(&tmp, buf, csize)) != csize)
      goto error;
    result += csize;
    while ((r = buf_peek_character_utf8(buf, &c)) > 0 &&
           ! ident_character_is_reserved(c)) {
      csize = r;
      if ((r = buf_xfer(&tmp, buf, csize)) != csize)
        goto error;
      result += csize;
    }
    if (r < 0) {
      buf_restore(buf, &save);
      return r;
    }
    buf_read_to_str(&tmp, &str);
    str_to_ident(&str, dest);
    str_clean(&str);
    return result;
  }
  if (r < 0)
    return r;
  return 0;
 error:
  buf_restore(buf, &save);
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

sw buf_parse_str (s_buf *buf, s_str *dest)
{
  u8 b;
  character c;
  sw r;
  sw result = 0;
  s_buf save;
  uw size = 0;
  s_buf tmp;
  assert(buf);
  assert(dest);
  buf_save(buf, &save);
  if ((r = buf_read_1(buf, "\"")) > 0) {
    while (1) {
      if ((r = buf_read_1(buf, "\"")) != 0)
        break;
      else if ((r = buf_parse_str_character(buf, &c)) > 0)
        size += character_utf8_size(c);
      else if (r < 0)
        break;
      else if ((r = buf_parse_str_u8(buf, &b)) > 0)
        size += 1;
      else
        break;
    }
  }
  buf_restore(buf, &save);
  if (r <= 0)
    return r;
  if (size == 0) {
    str_init_empty(dest);
    return 2;
  }
  buf_init_alloc(&tmp, size);
  if ((r = buf_read_1(buf, "\"")) > 0) {
    result += r;
    while (1) {
      if ((r = buf_read_1(buf, "\"")) > 0) {
        result += r;
        break;
      }
      else if (r < 0)
        break;
      else if ((r = buf_parse_str_character(buf, &c)) > 0) {
        result += r;
        buf_write_character_utf8(&tmp, c);
      }
      else if ((r = buf_parse_str_u8(buf, &b)) > 0) {
        result += r;
        buf_write_u8(&tmp, b);
      }
      else
        break;
    }
  }
  if (r <= 0 ||
      tmp.wpos != tmp.size) {
    buf_restore(buf, &save);
    buf_clean(&tmp);
    return r;
  }
  buf_to_str(&tmp, dest);
  return result;
}

sw buf_parse_str_character (s_buf *buf, character *dest)
{
  character c;
  sw r;
  sw r1 = 0;
  s_buf save;
  buf_save(buf, &save);
  if ((r = buf_read_1(buf, "\\")) > 0 &&
      (r1 = buf_read_character_utf8(buf, &c)) > 0) {
    switch (c) {
    case '0': c = 0; break;
    case 'n': c = '\n'; break;
    case 'r': c = '\r'; break;
    case 's': c = ' '; break;
    case 't': c = '\t'; break;
    case 'v': c = '\v'; break;
    case 'x': buf_restore(buf, &save); return 0;
    default: ;
    }
    *dest = c;
    return r + r1;
  }
  buf_restore(buf, &save);
  if (r < 0)
    return r;
  if (r > 0 && r1 <= 0)
    return r1;
  if ((r = buf_read_character_utf8(buf, &c)) > 0 &&
      ! str_character_is_reserved(c))
    *dest = c;
  return r;
}

sw buf_parse_str_u8 (s_buf *buf, u8 *dest)
{
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
  if (r <= 0)
    return r;
  if (r1 <= 0)
    return r1;
  if (r2 <= 0)
    return r2;
  assert(! "error");
  return -1;
}


sw buf_parse_sym (s_buf *buf, const s_sym **dest)
{
  s_buf tmp;
  character c;
  sw csize;
  sw r;
  sw r1 = 0;
  sw result = 0;
  s_buf save;
  s_str str;
  assert(buf);
  assert(dest);
  if ((r = buf_peek_1(buf, ":\"")) > 0) {
    if ((r = buf_read_1(buf, ":")) > 0 &&
        (r1 = buf_parse_str(buf, &str)) > 0) {
      *dest = str_to_sym(&str);
      str_clean(&str);
      return r + r1;
    }
    if (r1 < 0) {
      buf_restore(buf, &save);
      return r1;
    }
  }
  if (r < 0) {
    buf_restore(buf, &save);
    return r;
  }
  if ((r = buf_peek_character_utf8(buf, &c)) > 0 &&
      (c == ':' || character_is_uppercase(c))) {
    csize = r;
    BUF_INIT_ALLOCA(&tmp, SYM_MAX);
    if ((r = buf_xfer(&tmp, buf, csize)) != csize)
      goto error;
    result += csize;
    while ((r = buf_peek_character_utf8(buf, &c)) > 0 &&
           ! sym_character_is_reserved(c)) {
      csize = r;
      if ((r = buf_xfer(&tmp, buf, csize)) != csize)
        goto error;
      result += csize;
    }
    if (r < 0)
      goto error;
    buf_read_to_str(&tmp, &str);
    *dest = str_to_sym(&str);
    str_clean(&str);
    return result;
  }
  if (r < 0)
    return r;
  return 0;
 error:
  buf_restore(buf, &save);
  return r;
}

sw buf_parse_tag (s_buf *buf, s_tag *dest)
{
  sw r;
  assert(buf);
  assert(dest);
  (void) ((r = buf_parse_tag_str(buf, dest)) > 0 ||
          (r = buf_parse_tag_sym(buf, dest)) > 0 ||
          (r = buf_parse_tag_ident(buf, dest)) > 0);
  return r;
}

sw buf_parse_tag_ident (s_buf *buf, s_tag *dest)
{
  sw r;
  if ((r = buf_parse_ident(buf, &dest->data.ident)) > 0)
    dest->type.type = TAG_IDENT;
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
