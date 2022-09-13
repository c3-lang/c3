/* c3
 * Copyright 2022 Thomas de Grivel <thoxdg@gmail.com>
 */
#include <assert.h>
#include <stdlib.h>
#include <string.h>
#include "buf.h"
#include "buf_inspect.h"
#include "buf_save.h"
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

sw buf_inspect_bool_size (e_bool b)
{
  if (b)
    return strlen("true");
  return strlen("false");
}

sw buf_inspect_character (s_buf *buf, character c)
{
  sw r;
  sw size;
  size = buf_inspect_character_size(c);
  if ((r = buf_write_u8(buf, '\'')) < 0 ||
      (r = buf_inspect_str_character(buf, c)) < 0 ||
      (r = buf_write_u8(buf, '\'')) < 0)
    return r;
  return size;
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
  if (x < 0) {
    if ((r = buf_write_u8(buf, '-')) < 0)
      return r;
    x = -x;
  }
  /* TODO */
  errx(1, "buf_inspect_f32: not implemented");
  return -1;
}

sw buf_inspect_f32_size (f32 x)
{
  (void) x;
  assert(! "buf_inspect_f32_size: not implemented");
  return -1;
}

sw buf_inspect_f64 (s_buf *buf, f64 x)
{
  sw r;
  if (x < 0) {
    if ((r = buf_write_u8(buf, '-')) < 0)
      return r;
    x = -x;
  }
  /* TODO */
  errx(1, "buf_inspect_f64: not implemented");
  return -1;
}

sw buf_inspect_f64_size (f64 x)
{
  (void) x;
  assert(! "buf_inspect_f64_size: not implemented");
  return -1;
}

sw buf_inspect_ident (s_buf *buf, const s_ident *ident)
{
  assert(buf);
  assert(ident);
  if (ident->sym->str.size == 0)
    return buf_write_1(buf, "_\"\"");
  if (ident_has_reserved_characters(ident))
    return buf_inspect_ident_reserved(buf, ident);
  return buf_write_str(buf, &ident->sym->str);
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
  buf_write_1(buf, "_");
  buf_inspect_str_reserved(buf, &ident->sym->str);
  return size;
}

sw buf_inspect_ident_reserved_size (const s_ident *ident)
{
  sw size;
  assert(ident);
  size = strlen("_");
  size += buf_inspect_str_reserved_size(&ident->sym->str);
  return size;
}

sw buf_inspect_ident_size (const s_ident *ident)
{
  assert(ident);
  if (ident->sym->str.size == 0)
    return strlen("_\"\"");
  if (ident_has_reserved_characters(ident))
    return buf_inspect_ident_reserved_size(ident);
  return ident->sym->str.size;
}


sw buf_inspect_list (s_buf *buf, const s_list *list)
{
  const s_list *i;
  sw r;
  sw result = 0;
  assert(buf);
  assert(list);
  if ((r = buf_write_u8(buf, '[')) <= 0)
    return r;
  result++;
  i = list;
  while (i) {
    if ((r = buf_inspect_tag(buf, &i->tag)) < 0)
      return r;
    result += r;
    switch (i->next.type.type) {
    case TAG_LIST:
      if (i->next.data.list) {
        if ((r = buf_write_1(buf, ", ")) < 0)
          return r;
        result += r;
      }
      i = i->next.data.list;
      continue;
    default:
      if ((r = buf_write_1(buf, " | ")) < 0)
        return r;
      result += r;
      if ((r = buf_inspect_tag(buf, &i->next)) < 0)
        return r;
      result += r;
      break;
    }
  }
  if ((r = buf_write_1(buf, "]")) < 0)
    return r;
  result += r;
  return result;
}

sw buf_inspect_list_size (const s_list *list)
{
  const s_list *i;
  sw r;
  sw result = 0;
  assert(list);
  result += strlen("[");
  i = list;
  while (i) {
    if ((r = buf_inspect_tag_size(&i->tag)) < 0)
      return r;
    result += r;
    switch (i->next.type.type) {
    case TAG_LIST:
      if (i->next.data.list)
        result += strlen(", ");
      i = i->next.data.list;
      continue;
    default:
      result += strlen(" | ");
      if ((r = buf_inspect_tag_size(&i->next)) < 0)
        return r;
      result += r;
      break;
    }
  }
  result += strlen("]");
  return result;
}

sw buf_inspect_s8 (s_buf *buf, s8 x)
{
  sw r;
  sw result = 0;
  if (x < 0) {
    if ((r = buf_write_1(buf, "-")) < 0)
      return r;
    result += r;
    x = -x;
  }
  if ((r = buf_inspect_u8(buf, x)) < 0)
    return r;
  result += r;
  return result;
}

sw buf_inspect_s8_size (s8 x)
{
  sw result = 0;
  if (x < 0) {
    result += strlen("-");
    x = -x;
  }
  result += buf_inspect_u8_size(x);
  return result;
}

sw buf_inspect_s16 (s_buf *buf, s16 x)
{
  sw r;
  sw r1;
  if (x > 0)
    return buf_inspect_u16(buf, (u16) x);
  if ((r = buf_write_1(buf, "-")) < 0)
    return r;
  if ((r1 = buf_inspect_u16(buf, (u16) x)) < 0)
    return r1;
  return r + r1;
}

sw buf_inspect_s16_size (s16 x)
{
  sw result = 0;
  if (x < 0) {
    result += strlen("-");
    x = -x;
  }
  result += buf_inspect_u16_size(x);
  return result;
}

sw buf_inspect_s32 (s_buf *buf, s32 x)
{
  sw r;
  sw r1;
  if (x > 0)
    return buf_inspect_u32(buf, (u32) x);
  if ((r = buf_write_1(buf, "-")) < 0)
    return r;
  if ((r1 = buf_inspect_u32(buf, (u32) x)) < 0)
    return r1;
  return r + r1;
}

sw buf_inspect_s32_size (s32 x)
{
  sw result = 0;
  if (x < 0) {
    result += strlen("-");
    x = -x;
  }
  result += buf_inspect_u32_size(x);
  return result;
}

sw buf_inspect_s64 (s_buf *buf, s64 x)
{
  sw r;
  sw r1;
  if (x > 0)
    return buf_inspect_u64(buf, (u64) x);
  if ((r = buf_write_1(buf, "-")) < 0)
    return r;
  if ((r1 = buf_inspect_u64(buf, (u64) x)) < 0)
    return r1;
  return r + r1;
}

sw buf_inspect_s64_size (s64 x)
{
  sw result = 0;
  if (x < 0) {
    result += strlen("-");
    x = -x;
  }
  result += buf_inspect_u64_size(x);
  return result;
}

sw buf_inspect_str (s_buf *buf, const s_str *str)
{
  sw r;
  sw size;
  assert(buf);
  assert(str);
  if (str_has_reserved_characters(str))
    return buf_inspect_str_reserved(buf, str);
  size = str->size + 2;
  if ((r = buf_write_u8(buf, '"')) < 0 ||
      (r = buf_write_str(buf, str)) < 0 ||
      (r = buf_write_u8(buf, '"') < 0))
    return r;
  return size;
}

sw buf_inspect_str_byte (s_buf *buf, u8 b)
{
  sw r;
  const sw size = 4;
  if ((r = buf_write_u8(buf, '\\')) < 0 ||
      (r = buf_write_u8(buf, 'x')) < 0 ||
      (r = buf_u8_to_hex(buf, b)) < 0)
    return r;
  return size;
}

sw buf_inspect_str_character (s_buf *buf, character c)
{
  s_buf char_buf;
  int i;
  int j;
  sw r;
  sw size;
  size = buf_inspect_str_character_size(c);
  if (size <= 0)
    return size;
  if (! str_character_is_reserved(c))
    return buf_write_character_utf8(buf, c);
  buf_write_u8(buf, '\\');
  switch (c) {
  case '\0': if ((r = buf_write_u8(buf, '0')) < 0) return r; break;
  case '\n': if ((r = buf_write_u8(buf, 'n')) < 0) return r; break;
  case '\r': if ((r = buf_write_u8(buf, 'r')) < 0) return r; break;
  case '\t': if ((r = buf_write_u8(buf, 't')) < 0) return r; break;
  case '\v': if ((r = buf_write_u8(buf, 'v')) < 0) return r; break;
  case '\"': if ((r = buf_write_u8(buf, '"')) < 0) return r; break;
  case '\'': if ((r = buf_write_u8(buf, '\'')) < 0) return r; break;
  case '\\': if ((r = buf_write_u8(buf, '\\')) < 0) return r; break;
  default:
    BUF_INIT_ALLOCA(&char_buf, 4);
    if ((i = buf_write_character_utf8(&char_buf, c)) < 0)
      return i;
    j = 0;
    if (i-- > 0) {
      if ((r = buf_write_u8(buf, 'x')) < 0)
        return r;
      if ((r = buf_u8_to_hex(buf, char_buf.ptr.pu8[j++])) < 0)
        return r;
      while (i--) {
        if ((r = buf_write_1(buf, "\\x")) < 0 ||
            (r = buf_u8_to_hex(buf, char_buf.ptr.pu8[j++])) < 0)
          return r;
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
  if ((r = buf_write_u8(buf, '"')) < 0)
    return r;
  str_init_str(&s, src);
  while (1) {
    if ((r = str_read_character(&s, &c)) > 0) {
      if ((r = buf_inspect_str_character(buf, c)) < 0)
        return r;
    }
    else if ((r = str_read(&s, &b)) == 1) {
      if ((r = buf_inspect_str_byte(buf, b)) < 0)
        return r;
    }
    else if (r < 0)
      return r;
    else if (r == 0)
      break;
  }
  if ((r = buf_write_u8(buf, '"')) < 0)
    return r;
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
  sw r;
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
  if ((r = buf_write_1(buf, ":")) < 0 ||
      (r = buf_write_str(buf, &sym->str)) < 0)
    return r;
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
  sw r;
  sw size;
  size = buf_inspect_sym_reserved_size(sym);
  if (size <= 0)
    return size;
  if ((r = buf_write_u8(buf, ':')) < 0 ||
      (r = buf_inspect_str(buf, &sym->str)) < 0)
    return r;
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
  case TAG_LIST:  return buf_inspect_list(buf, tag->data.list);
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

sw buf_inspect_tag_size (const s_tag *tag)
{
  assert(tag);
  switch(tag->type.type) {
  case TAG_VOID:  return 0;
  case TAG_BOOL:  return buf_inspect_bool_size(tag->data.bool);
  case TAG_CHARACTER:
    return buf_inspect_character_size(tag->data.character);
  case TAG_F32:   return buf_inspect_f32_size(tag->data.f32);
  case TAG_F64:   return buf_inspect_f64_size(tag->data.f64);
  case TAG_IDENT: return buf_inspect_ident_size(&tag->data.ident);
  case TAG_LIST:  return buf_inspect_list_size(tag->data.list);
  case TAG_S8:    return buf_inspect_s8_size(tag->data.s8);
  case TAG_S16:   return buf_inspect_s16_size(tag->data.s16);
  case TAG_S32:   return buf_inspect_s32_size(tag->data.s32);
  case TAG_S64:   return buf_inspect_s64_size(tag->data.s64);
  case TAG_STR:   return buf_inspect_str_size(&tag->data.str);
  case TAG_SYM:   return buf_inspect_sym_size(tag->data.sym);
  case TAG_TUPLE: return buf_inspect_tuple_size(&tag->data.tuple);
  case TAG_U8:    return buf_inspect_u8_size(tag->data.u8);
  case TAG_U16:   return buf_inspect_u16_size(tag->data.u16);
  case TAG_U32:   return buf_inspect_u32_size(tag->data.u32);
  case TAG_U64:   return buf_inspect_u64_size(tag->data.u64);
  }
  assert(! "unknown tag type");
  return -1;
}

sw buf_inspect_tuple (s_buf *buf, const s_tuple *tuple)
{
  u64 i = 0;
  sw r;
  sw result = 0;
  if ((r = buf_write_1(buf, "{")) < 0)
    return r;
  result += r;
  if (tuple->count) {
    while (i < tuple->count - 1) {
      if ((r = buf_inspect_tag(buf, tuple->tag + i)) < 0)
        return r;
      result += r;
      if ((r = buf_write_1(buf, ", ")) < 0)
        return r;
      result += r;
      i++;
    }
    if ((r = buf_inspect_tag(buf, tuple->tag + i)) < 0)
      return r;
    result += r;
  }
  if ((r = buf_write_1(buf, "}")) < 0)
    return r;
  result += r;
  return result;
}

sw buf_inspect_tuple_size (const s_tuple *tuple)
{
  u64 i = 0;
  sw r;
  sw result;
  result = strlen("{");
  if (tuple->count) {
    while (i < tuple->count - 1) {
      if ((r = buf_inspect_tag_size(tuple->tag + i)) < 0)
        return r;
      result += r;
      result += strlen(", ");
      i++;
    }
    if ((r = buf_inspect_tag_size(tuple->tag + i)) < 0)
      return r;
    result += r;
  }
  result += strlen("}");
  return result;
}

sw buf_inspect_u8 (s_buf *buf, u8 x)
{
  u8 digit;
  sw i;
  sw r;
  sw size = 0;
  s_buf tmp;
  BUF_INIT_ALLOCA(&tmp, 3);
  if (x == 0)
    return buf_write_u8(buf, '0');
  while (x > 0) {
    digit = x % 10;
    x /= 10;
    if (buf_write_u8(&tmp, '0' + digit) != 1) {
      return -1;
    }
    size++;
  }
  i = size;
  while (i--)
    if ((r = buf_write_u8(buf, tmp.ptr.pu8[i])) < 0)
      return r;
  return size;
}

sw buf_inspect_u8_size (u8 x)
{
  sw size = 0;
  if (x == 0)
    return 1;
  while (x > 0) {
    x /= 10;
    size++;
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
  BUF_INIT_ALLOCA(&tmp, 5);
  if (x == 0)
    return buf_write_u8(buf, '0');
  while (x > 0) {
    digit = x % 10;
    x /= 10;
    if (buf_write_u8(&tmp, '0' + digit) != 1) {
      return -1;
    }
    size++;
  }
  i = size;
  while (i--)
    if ((r = buf_write_u8(buf, tmp.ptr.pu8[i])) < 0)
      return r;
  return size;
}

sw buf_inspect_u16_size (u16 x)
{
  sw size = 0;
  if (x == 0)
    return 1;
  while (x > 0) {
    x /= 10;
    size++;
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
  BUF_INIT_ALLOCA(&tmp, 10);
  if (x == 0)
    return buf_write_u8(buf, '0');
  while (x > 0) {
    digit = x % 10;
    x /= 10;
    if (buf_write_u8(&tmp, '0' + digit) != 1) {
      return -1;
    }
    size++;
  }
  i = size;
  while (i--)
    if ((r = buf_write_u8(buf, tmp.ptr.pu8[i])) < 0)
      return r;
  return size;
}

sw buf_inspect_u32_size (u32 x)
{
  sw size = 0;
  if (x == 0)
    return 1;
  while (x > 0) {
    x /= 10;
    size++;
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
  BUF_INIT_ALLOCA(&tmp, 20);
  if (x == 0)
    return buf_write_u8(buf, '0');
  while (x > 0) {
    digit = x % 10;
    x /= 10;
    if (buf_write_u8(&tmp, '0' + digit) != 1) {
      return -1;
    }
    size++;
  }
  i = size;
  while (i--)
    if ((r = buf_write_u8(buf, tmp.ptr.pu8[i])) < 0)
      return r;
  return size;
}

sw buf_inspect_u64_size (u64 x)
{
  sw size = 0;
  if (x == 0)
    return 1;
  while (x > 0) {
    x /= 10;
    size++;
  }
  return size;
}
