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

sw buf_inspect_ident_reserved (s_buf *buf, const s_ident *str);
sw buf_inspect_ident_reserved_size (const s_ident *str);
sw buf_inspect_str_byte (s_buf *buf, u8 b);
sw buf_inspect_str_reserved (s_buf *buf, const s_str *str);
sw buf_inspect_str_reserved_size (const s_str *str);
sw buf_inspect_sym_reserved (s_buf *buf, const s_sym *sym);
sw buf_inspect_sym_reserved_size (const s_sym *sym);

sw buf_inspect_bool (s_buf *buf, e_bool b)
{
  if (b)
    return buf_write_1(buf, "true");
  return buf_write_1(buf, "false");
}

sw buf_inspect_character (s_buf *buf, character c)
{
  sw size;
  size = buf_inspect_character_size(c);
  if (buf->wpos + size > buf->size) {
    assert(! "buffer overflow");
    return -1;
  }
  buf_write_u8(buf, '\'');
  buf_inspect_str_character(buf, c);
  buf_write_u8(buf, '\'');
  return size;
}

sw buf_inspect_ident_reserved (s_buf *buf, const s_ident *ident)
{
  sw size;
  assert(buf);
  assert(ident);
  size = buf_inspect_ident_reserved_size(ident);
  if (buf->wpos + size > buf->size) {
    assert(! "buffer overflow");
    return -1;
  }
  buf_write_1(buf, "~i");
  buf_inspect_str_reserved(buf, &ident->sym->str);
  return size;
}

sw buf_inspect_ident_reserved_size (const s_ident *ident)
{
  sw size;
  assert(ident);
  size = strlen("~i");
  size += buf_inspect_str_reserved_size(&ident->sym->str);
  return size;
}

sw buf_inspect_ident (s_buf *buf, const s_ident *ident)
{
  assert(buf);
  assert(ident);
  if (ident->sym->str.size == 0)
    return buf_write_1(buf, "~i\"\"");
  if (ident_has_reserved_characters(ident))
    return buf_inspect_ident_reserved(buf, ident);
  return buf_write_str(buf, &ident->sym->str);
}

sw buf_inspect_ident_size (const s_ident *ident)
{
  assert(ident);
  if (ident->sym->str.size == 0)
    return strlen("~i\"\"");
  if (ident_has_reserved_characters(ident))
    return buf_inspect_ident_reserved_size(ident);
  return ident->sym->str.size;
}

sw buf_inspect_character_size (character c)
{
  sw size;
  size = buf_inspect_str_character_size(c);
  if (size < 0)
    return size;
  if (size == 0)
    return -1;
  size += 2;
  return size;
}

sw buf_inspect_f32 (s_buf *buf, f32 x)
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

sw buf_inspect_list (s_buf *buf, const s_list *list)
{
  sw r;
  sw result = 0;
  s_buf save;
  buf_save(buf, &save);
  if ((r = buf_write_u8(buf, '[')) != 1)
    return r;
  result++;
  if ((r = buf_inspect_tag(buf, list->tag)) < 0)
    goto error;
  result += r;
  if ((r = buf_write_1(buf, " | ")) < 0)
    goto error;
  result += r;
  if ((r = buf_inspect_list(buf, list->next)) < 0)
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

sw buf_inspect_str (s_buf *buf, const s_str *str)
{
  sw size;
  assert(buf);
  assert(str);
  if (str_has_reserved_characters(str))
    return buf_inspect_str_reserved(buf, str);
  size = str->size + 2;
  if (buf->wpos + size > buf->size) {
    assert(! "buffer overflow");
    return -1;
  }
  buf_write_u8(buf, '"');
  buf_write_str(buf, str);
  buf_write_u8(buf, '"');
  return size;
}

sw buf_inspect_str_byte (s_buf *buf, u8 b)
{
  sw size = 4;
  if (buf->wpos + size > buf->size) {
    assert(! "buffer overflow");
    return -1;
  }
  buf_write_u8(buf, '\\');
  buf_write_u8(buf, 'x');
  buf_u8_to_hex(buf, b);
  return size;
}

sw buf_inspect_str_character (s_buf *buf, character c)
{
  s_buf char_buf;
  int i;
  int j;
  sw size;
  size = buf_inspect_str_character_size(c);
  if (size <= 0)
    return size;
  if (buf->wpos + size > buf->size) {
    assert(! "buffer overflow");
    return -1;
  }
  if (! str_character_is_reserved(c))
    return buf_write_character_utf8(buf, c);
  buf_write_u8(buf, '\\');
  switch (c) {
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
    i = buf_write_character_utf8(&char_buf, c);
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

sw buf_inspect_str_character_size (character c)
{
  sw csize;
  sw size;
  if (! str_character_is_reserved(c))
    return character_utf8_size(c);
  size = 0;
  switch (c) {
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
    csize = character_utf8_size(c);
    if (csize <= 0)
      return -1;
    size += csize * 4;
  }
  return size;
}

/* XXX keep in sync with buf_inspect_str_reserved_size */
sw buf_inspect_str_reserved (s_buf *buf, const s_str *src)
{
  u8 b;
  character c;
  sw r;
  sw size;
  s_str s;
  size = buf_inspect_str_reserved_size(src);
  if (size <= 0)
    return size;
  if (buf->wpos + size > buf->size) {
    assert(! "buffer overflow");
    return -1;
  }
  buf_write_u8(buf, '"');
  str_init_str(&s, src);
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
sw buf_inspect_str_reserved_size (const s_str *src)
{
  u8 b;
  character c;
  const sw quote_size = 1;
  sw r;
  s_str s;
  sw size;
  size = 2 * quote_size;
  str_init_str(&s, src);
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

sw buf_inspect_str_size (const s_str *str)
{
  const sw quote_size = 1;
  sw size;
  if (str_has_reserved_characters(str))
    return buf_inspect_str_reserved_size(str);
  size = str->size + 2 * quote_size;
  return size;
}

sw buf_inspect_sym (s_buf *buf, const s_sym *sym)
{
  sw size;
  assert(buf);
  assert(sym);
  if (sym->str.size == 0)
    return buf_write_1(buf, ":\"\"");
  if (sym_has_reserved_characters(sym))
    return buf_inspect_sym_reserved(buf, sym);
  if (sym_is_module(sym))
    return buf_write_str(buf, &sym->str);
  size = sym->str.size + 1;
  if (buf->wpos + size > buf->size) {
    assert(! "buffer overflow");
    return -1;
  }
  buf_write_u8(buf, ':');
  buf_write_str(buf, &sym->str);
  return size;
}

sw buf_inspect_sym_size (const s_sym *sym)
{
  const sw colon_size = 1;
  assert(sym);
  if (sym->str.size == 0)
    return 3;
  if (sym_has_reserved_characters(sym))
    return buf_inspect_sym_reserved_size(sym);
  if (sym_is_module(sym))
    return sym->str.size;
  return sym->str.size + colon_size;
}

/* XXX keep in sync with buf_inspect_sym_reserved_size */
sw buf_inspect_sym_reserved (s_buf *buf, const s_sym *sym)
{
  sw  size;
  size = buf_inspect_sym_reserved_size(sym);
  if (size <= 0)
    return size;
  if (buf->wpos + size > buf->size) {
    assert(! "buffer_overflow");
    return -1;
  }
  buf_write_u8(buf, ':');
  buf_inspect_str(buf, &sym->str);
  return size;
}

/* XXX keep in sync with buf_inspect_sym_reserved */
sw buf_inspect_sym_reserved_size (const s_sym *sym)
{
  const sw colon_size = 1;
  sw size;
  size = buf_inspect_str_size(&sym->str);
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
  case TAG_IDENT: return buf_inspect_ident(buf, &tag->data.ident);
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
