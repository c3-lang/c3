/* c3
 * Copyright 2022 Thomas de Grivel <thoxdg@gmail.com>
 */
#include <assert.h>
#include <stdlib.h>
#include <string.h>
#include "buf.h"
#include "buf_inspect.h"
#include "character.h"
#include "ident.h"
#include "str.h"
#include "sym.h"
#include "integer.h"
#include "../libtommath/tommath.h"

sw buf_inspect_ident_reserved (s_buf *buf, const s_ident *x);
sw buf_inspect_ident_reserved_size (const s_ident *x);
sw buf_inspect_str_byte (s_buf *buf, u8 x);
sw buf_inspect_str_reserved (s_buf *buf, const s_str *x);
sw buf_inspect_str_reserved_size (const s_str *x);
sw buf_inspect_sym_reserved (s_buf *buf, const s_sym *x);
sw buf_inspect_sym_reserved_size (const s_sym *x);

sw buf_inspect_bool (s_buf *buf, e_bool x)
{
  if (x)
    return buf_write_1(buf, "true");
  return buf_write_1(buf, "false");
}

sw buf_inspect_character (s_buf *buf, character x)
{
  sw size;
  size = buf_inspect_character_size(x);
  if (buf->wpos + size > buf->size) {
    assert(! "buffer overflow");
    return -1;
  }
  buf_write_u8(buf, '\'');
  buf_inspect_str_character(buf, x);
  buf_write_u8(buf, '\'');
  return size;
}

sw buf_inspect_ident_reserved (s_buf *buf, const s_ident *x)
{
  sw size;
  assert(buf);
  assert(x);
  size = buf_inspect_ident_reserved_size(x);
  if (buf->wpos + size > buf->size) {
    assert(! "buffer overflow");
    return -1;
  }
  buf_write_1(buf, "~i");
  buf_inspect_str_reserved(buf, &x->sym->str);
  return size;
}

sw buf_inspect_ident_reserved_size (const s_ident *x)
{
  sw size;
  assert(x);
  size = strlen("~i");
  size += buf_inspect_str_reserved_size(&x->sym->str);
  return size;
}

sw buf_inspect_ident (s_buf *buf, const s_ident *x)
{
  assert(buf);
  assert(x);
  if (x->sym->str.size == 0)
    return buf_write_1(buf, "~i\"\"");
  if (ident_has_reserved_characters(x))
    return buf_inspect_ident_reserved(buf, x);
  return buf_write_str(buf, &x->sym->str);
}

sw buf_inspect_ident_size (const s_ident *x)
{
  assert(x);
  if (x->sym->str.size == 0)
    return strlen("~i\"\"");
  if (ident_has_reserved_characters(x))
    return buf_inspect_ident_reserved_size(x);
  return x->sym->str.size;
}

sw buf_inspect_character_size (character x)
{
  sw size;
  size = buf_inspect_str_character_size(x);
  if (size < 0)
    return size;
  if (size == 0)
    return -1;
  size += 2;
  return size;
}
sw buf_inspect_integer_dec_size(const s_integer *x)
{
  sw size;
  assert(x);
  int size_tmp = 0;
  int err;
  err = mp_radix_size(&x->mp_int, 10,&size_tmp);
  if (err != MP_OKAY) {
    assert(! "mp_radix_size failed");
    return -1;
  }
  size = size_tmp;
  if (size < 0)
    return size;
  if (size == 0)
    return -1;
  return size;
}

sw buf_inspect_str_integer_dec(s_buf *buf, const s_integer *x)
{
  sw size;
  assert(buf);
  assert(x);
  size = buf_inspect_integer_dec_size(x);
  if (buf->wpos + size > buf->size) {
    assert(! "buffer overflow");
    return -1;
  }
  if (x->mp_int.sign == MP_NEG) {
    buf_write_u8(buf, '-');
  }
  buf_write_u8(buf, '0');
  return size;
}

sw buf_inspect_integer_dec(s_buf *buf, const s_integer *x)
{
  sw size;
  assert(buf);
  assert(x);
  size = buf_inspect_integer_dec_size(x);
  if (buf->wpos + size > buf->size) {
    assert(! "buffer overflow");
    return -1;
  }
  buf_write_u8(buf, '~');
  buf_write_u8(buf, 'd');
  buf_write_u8(buf, '"');
  buf_inspect_str_integer_dec(buf, x);
  buf_write_u8(buf, '"');
  return size;
}


sw buf_inspect_f64 (s_buf *buf, f64 x)
{
  sw r;
  s_buf save;
  buf_save(buf, &save);
  if (x < 0) {
    if ((r = buf_write_u8(buf, '-')) != 1)
      return r;
    x = -x;
  }
  /* TODO */
  err(1, "not implemented");
  return -1;
}

sw buf_inspect_list (s_buf *buf, const s_list *x)
{
  sw r;
  sw result = 0;
  s_buf save;
  buf_save(buf, &save);
  if ((r = buf_write_u8(buf, '[')) != 1)
    return r;
  result++;
  if ((r = buf_inspect_tag(buf, x->tag)) < 0)
    goto error;
  result += r;
  if ((r = buf_write_1(buf, " | ")) < 0)
    goto error;
  result += r;
  if ((r = buf_inspect_list(buf, x->next)) < 0)
    goto error;
  result += r;
  if ((r = buf_write_1(buf, "]")) < 0)
    goto error;
  result += r;
  return result;
 error:
  buf_restore(buf, &save);
  return r;
}

sw buf_inspect_s8 (s_buf *buf, s8 x)
{
  sw r;
  s_buf save;
  buf_save(buf, &save);
  if (x < 0) {
    if ((r = buf_write_u8(buf, '-')) != 1)
      return r;
    if ((r = buf_inspect_u8(buf, (u8) x)) <= 0) {
      buf_restore(buf, &save);
      return r;
    }
    return r + 1;
  }
  return buf_inspect_u8(buf, (u8) x);
}

sw buf_inspect_s16 (s_buf *buf, s16 x)
{
  sw r;
  s_buf save;
  buf_save(buf, &save);
  if (x < 0) {
    if ((r = buf_write_u8(buf, '-')) != 1)
      return r;
    if ((r = buf_inspect_u16(buf, (u16) x)) <= 0) {
      buf_restore(buf, &save);
      return r;
    }
    return r + 1;
  }
  return buf_inspect_u16(buf, (u16) x);
}

sw buf_inspect_s32 (s_buf *buf, s32 x)
{
  sw r;
  s_buf save;
  buf_save(buf, &save);
  if (x < 0) {
    if ((r = buf_write_u8(buf, '-')) != 1)
      return r;
    if ((r = buf_inspect_u32(buf, (u32) x)) <= 0) {
      buf_restore(buf, &save);
      return r;
    }
    return r + 1;
  }
  return buf_inspect_u32(buf, (u32) x);
}

sw buf_inspect_s64 (s_buf *buf, s64 x)
{
  sw r;
  s_buf save;
  buf_save(buf, &save);
  if (x < 0) {
    if ((r = buf_write_u8(buf, '-')) != 1)
      return r;
    if ((r = buf_inspect_u64(buf, (u64) x)) <= 0) {
      buf_restore(buf, &save);
      return r;
    }
    return r + 1;
  }
  return buf_inspect_u64(buf, (u64) x);
}

sw buf_inspect_str (s_buf *buf, const s_str *x)
{
  sw size;
  assert(buf);
  assert(x);
  if (str_has_reserved_characters(x))
    return buf_inspect_str_reserved(buf, x);
  size = x->size + 2;
  if (buf->wpos + size > buf->size) {
    assert(! "buffer overflow");
    return -1;
  }
  buf_write_u8(buf, '"');
  buf_write_str(buf, x);
  buf_write_u8(buf, '"');
  return size;
}

sw buf_inspect_str_byte (s_buf *buf, u8 x)
{
  sw size = 4;
  if (buf->wpos + size > buf->size) {
    assert(! "buffer overflow");
    return -1;
  }
  buf_write_u8(buf, '\\');
  buf_write_u8(buf, 'x');
  buf_u8_to_hex(buf, x);
  return size;
}

sw buf_inspect_str_character (s_buf *buf, character x)
{
  s_buf char_buf;
  int i;
  int j;
  sw size;
  size = buf_inspect_str_character_size(x);
  if (size <= 0)
    return size;
  if (buf->wpos + size > buf->size) {
    assert(! "buffer overflow");
    return -1;
  }
  if (! str_character_is_reserved(x))
    return buf_write_character_utf8(buf, x);
  buf_write_u8(buf, '\\');
  switch (x) {
  case '\0': buf_write_u8(buf, '0'); break;
  case '\n': buf_write_u8(buf, 'n'); break;
  case '\r': buf_write_u8(buf, 'r'); break;
  case '\t': buf_write_u8(buf, 't'); break;
  case '\v': buf_write_u8(buf, 'v'); break;
  case '\"': buf_write_u8(buf, '"'); break;
  case '\'': buf_write_u8(buf, '\''); break;
  case '\\': buf_write_u8(buf, '\\'); break;
  default:
    BUF_INIT_ALLOCA(&char_buf, 4);
    i = buf_write_character_utf8(&char_buf, x);
    j = 0;
    if (i-- > 0) {
      buf_write_u8(buf, 'x');
      buf_u8_to_hex(buf, char_buf.ptr.pu8[j++]);
      while (i--) {
        buf_write_u8(buf, '\\');
        buf_write_u8(buf, 'x');
        buf_u8_to_hex(buf, char_buf.ptr.pu8[j++]);
      }
    }
  }
  return size;
}

sw buf_inspect_str_character_size (character x)
{
  sw csize;
  sw size;
  if (! str_character_is_reserved(x))
    return character_utf8_size(x);
  size = 0;
  switch (x) {
  case '\0':
  case '\n':
  case '\r':
  case '\t':
  case '\v':
  case '\"':
  case '\'':
  case '\\':
    size += 2;
    break;
  default:
    csize = character_utf8_size(x);
    if (csize <= 0)
      return -1;
    size += csize * 4;
  }
  return size;
}

/* XXX keep in sync with buf_inspect_str_reserved_size */
sw buf_inspect_str_reserved (s_buf *buf, const s_str *x)
{
  u8 b;
  character c;
  sw r;
  sw size;
  s_str s;
  size = buf_inspect_str_reserved_size(x);
  if (size <= 0)
    return size;
  if (buf->wpos + size > buf->size) {
    assert(! "buffer overflow");
    return -1;
  }
  buf_write_u8(buf, '"');
  str_init_str(&s, x);
  while (1) {
    if ((r = str_read_character(&s, &c)) > 0)
      buf_inspect_str_character(buf, c);
    else if (r && (r = str_read(&s, &b)) == 1)
      buf_inspect_str_byte(buf, b);
    else if (r < 0)
      return -1;
    else if (r == 0)
      break;
  }
  buf_write_u8(buf, '"');
  return size;
}

/* XXX keep in sync with buf_inspect_str_reserved */
sw buf_inspect_str_reserved_size (const s_str *x)
{
  u8 b;
  character c;
  const sw quote_size = 1;
  sw r;
  s_str s;
  sw size;
  size = 2 * quote_size;
  str_init_str(&s, x);
  while (1) {
    if ((r = str_read_character(&s, &c)) > 0)
      size += buf_inspect_str_character_size(c);
    else if (r && (r = str_read(&s, &b)) == 1)
      size += buf_inspect_str_byte_size;
    else if (r < 0)
      return -1;
    else if (r == 0)
      break;
  }
  return size;
}

sw buf_inspect_str_size (const s_str *x)
{
  const sw quote_size = 1;
  sw size;
  if (str_has_reserved_characters(x))
    return buf_inspect_str_reserved_size(x);
  size = x->size + 2 * quote_size;
  return size;
}

sw buf_inspect_sym (s_buf *buf, const s_sym *x)
{
  sw size;
  assert(buf);
  assert(x);
  if (x->str.size == 0)
    return buf_write_1(buf, ":\"\"");
  if (sym_has_reserved_characters(x))
    return buf_inspect_sym_reserved(buf, x);
  if (sym_is_module(x))
    return buf_write_str(buf, &x->str);
  size = x->str.size + 1;
  if (buf->wpos + size > buf->size) {
    assert(! "buffer overflow");
    return -1;
  }
  buf_write_u8(buf, ':');
  buf_write_str(buf, &x->str);
  return size;
}

sw buf_inspect_sym_size (const s_sym *x)
{
  const sw colon_size = 1;
  assert(x);
  if (x->str.size == 0)
    return 3;
  if (sym_has_reserved_characters(x))
    return buf_inspect_sym_reserved_size(x);
  if (sym_is_module(x))
    return x->str.size;
  return x->str.size + colon_size;
}

/* XXX keep in sync with buf_inspect_sym_reserved_size */
sw buf_inspect_sym_reserved (s_buf *buf, const s_sym *x)
{
  sw  size;
  size = buf_inspect_sym_reserved_size(x);
  if (size <= 0)
    return size;
  if (buf->wpos + size > buf->size) {
    assert(! "buffer_overflow");
    return -1;
  }
  buf_write_u8(buf, ':');
  buf_inspect_str(buf, &x->str);
  return size;
}

/* XXX keep in sync with buf_inspect_sym_reserved */
sw buf_inspect_sym_reserved_size (const s_sym *x)
{
  const sw colon_size = 1;
  sw size;
  size = buf_inspect_str_size(&x->str);
  if (size < 0)
    return size;
  size += colon_size;
  return size;
}

sw buf_inspect_tag (s_buf *buf, const s_tag *tag)
{
  assert(tag);
  switch(tag->type.type) {
  case TAG_VOID:  return 0;
  case TAG_BOOL:  return buf_inspect_bool(buf, tag->data.bool);
  case TAG_CHARACTER:
    return buf_inspect_character(buf, tag->data.character);
  case TAG_F32:   return buf_inspect_f32(buf, tag->data.f32);
  case TAG_F64:   return buf_inspect_f64(buf, tag->data.f64);
  case TAG_LIST:  return buf_inspect_list(buf, &tag->data.list);
  case TAG_S8:    return buf_inspect_s8(buf, tag->data.s8);
  case TAG_S16:   return buf_inspect_s16(buf, tag->data.s16);
  case TAG_S32:   return buf_inspect_s32(buf, tag->data.s32);
  case TAG_S64:   return buf_inspect_s64(buf, tag->data.s64);
  case TAG_STR:   return buf_inspect_str(buf, &tag->data.str);
  case TAG_SYM:   return buf_inspect_sym(buf, tag->data.sym);
  case TAG_TUPLE: return buf_inspect_tuple(buf, &tag->data.tuple);
  case TAG_U8:    return buf_inspect_u8(buf, tag->data.u8);
  case TAG_U16:   return buf_inspect_u16(buf, tag->data.u16);
  case TAG_U32:   return buf_inspect_u32(buf, tag->data.u32);
  case TAG_U64:   return buf_inspect_u64(buf, tag->data.u64);
  }
  assert(! "unknown tag type");
  return -1;
}

sw buf_inspect_tuple (s_buf *buf, const s_tuple *tuple)
{
  u64 i = 0;
  sw r;
  sw result = 0;
  s_buf save;
  buf_save(buf, &save);
  if ((r = buf_write_1(buf, "{")) < 0)
    return r;
  result += r;
  while (i < tuple->count) {
    if ((r = buf_inspect_tag(buf, tuple->tag + i)) < 0)
      goto error;
    result += r;
    if ((r = buf_write_1(buf, ", ")) < 0)
      goto error;
    result += r;
    i++;
  }
  if ((r = buf_write_1(buf, "}")) < 0)
    goto error;
  result += r;
  return result;
 error:
  buf_restore(buf, &save);
  return r;
}

sw buf_inspect_u8 (s_buf *buf, u8 x)
{
  u8 digit;
  sw i;
  sw r;
  sw size = 0;
  s_buf tmp;
  s_buf save;
  BUF_INIT_ALLOCA(&tmp, 3);
  while (x > 0) {
    digit = x % 10;
    x /= 10;
    if (buf_write_u8(&tmp, '0' + digit) != 1) {
      assert(! "error");
      return -1;
    }
    size++;
  }
  buf_save(buf, &save);
  i = size;
  while (i--)
    if ((r = buf_write_u8(buf, tmp.ptr.pu8[i])) != 1) {
      buf_restore(buf, &save);
      return r;
    }
  return size;
}

sw buf_inspect_u16 (s_buf *buf, u16 x)
{
  u8 digit;
  sw i;
  sw r;
  sw size = 0;
  s_buf tmp;
  s_buf save;
  BUF_INIT_ALLOCA(&tmp, 5);
  while (x > 0) {
    digit = x % 10;
    x /= 10;
    if (buf_write_u8(&tmp, '0' + digit) != 1) {
      assert(! "error");
      return -1;
    }
    size++;
  }
  buf_save(buf, &save);
  i = size;
  while (i--)
    if ((r = buf_write_u8(buf, tmp.ptr.pu8[i])) != 1) {
      buf_restore(buf, &save);
      return r;
    }
  return size;
}

sw buf_inspect_u32 (s_buf *buf, u32 x)
{
  u8 digit;
  sw i;
  sw r;
  sw size = 0;
  s_buf tmp;
  s_buf save;
  BUF_INIT_ALLOCA(&tmp, 10);
  while (x > 0) {
    digit = x % 10;
    x /= 10;
    if (buf_write_u8(&tmp, '0' + digit) != 1) {
      assert(! "error");
      return -1;
    }
    size++;
  }
  buf_save(buf, &save);
  i = size;
  while (i--)
    if ((r = buf_write_u8(buf, tmp.ptr.pu8[i])) != 1) {
      buf_restore(buf, &save);
      return r;
    }
  return size;
}

sw buf_inspect_u64 (s_buf *buf, u64 x)
{
  u8 digit;
  sw i;
  sw r;
  sw size = 0;
  s_buf tmp;
  s_buf save;
  BUF_INIT_ALLOCA(&tmp, 20);
  while (x > 0) {
    digit = x % 10;
    x /= 10;
    if (buf_write_u8(&tmp, '0' + digit) != 1) {
      assert(! "error");
      return -1;
    }
    size++;
  }
  buf_save(buf, &save);
  i = size;
  while (i--)
    if ((r = buf_write_u8(buf, tmp.ptr.pu8[i])) != 1) {
      buf_restore(buf, &save);
      return r;
    }
  return size;
}

sw buf_inspect_f32 (s_buf *buf, f32 x)
{
  u8 digit;
  sw i;
  sw r;
  sw size = 0;
  s_buf tmp;
  s_buf save;
  BUF_INIT_ALLOCA(&tmp, 20);
  while (x > 0) {
    digit = (int)x % 10;
    x /= 10;
    if (buf_write_u8(&tmp, '0' + digit) != 1) {
      assert(! "error");
      return -1;
      }
    size++;
    }
  buf_save(buf, &save);
  i = size;
  while (i--)
    if ((r = buf_write_u8(buf, tmp.ptr.pu8[i])) != 1) {
      buf_restore(buf, &save);
      return r;
    }
  return size;
}
