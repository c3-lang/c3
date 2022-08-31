/* c3
 * Copyright 2022 Thomas de Grivel <thoxdg@gmail.com>
 */
#include <assert.h>
#include <stdlib.h>
#include "buf.h"
#include "character.h"
#include "str.h"
#include "sym.h"

sw buf_inspect_str_byte (s_buf *buf, u8 b);
sw buf_inspect_str_reserved (s_buf *buf, const s_str *str);
sw buf_inspect_str_reserved_size (const s_str *str);
sw buf_inspect_sym_reserved (s_buf *buf, const s_sym *sym);
sw buf_inspect_sym_reserved_size (const s_sym *sym);

sw buf_inspect_character (s_buf *buf, character c)
{
  sw size;
  size = buf_inspect_character_size(c);
  if (buf->wpos + size > buf->size) {
    assert(! "buffer overflow");
    return -1;
  }
  buf_write(buf, '\'');
  buf_inspect_str_character(buf, c);
  buf_write(buf, '\'');
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
  buf_write(buf, '"');
  buf_write_str(buf, str);
  buf_write(buf, '"');
  return size;
}

sw buf_inspect_str_byte (s_buf *buf, u8 b)
{
  sw size = 4;
  if (buf->wpos + size > buf->size) {
    assert(! "buffer overflow");
    return -1;
  }
  buf_write(buf, '\\');
  buf_write(buf, 'x');
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
    return buf_write_character(buf, c);
  buf_write(buf, '\\');
  switch (c) {
  case '\0': buf_write(buf, '0'); break;
  case '\n': buf_write(buf, 'n'); break;
  case '\r': buf_write(buf, 'r'); break;
  case '\t': buf_write(buf, 't'); break;
  case '\v': buf_write(buf, 'v'); break;
  case '\"': buf_write(buf, '"'); break;
  case '\'': buf_write(buf, '\''); break;
  case '\\': buf_write(buf, '\\'); break;
  default:
    BUF_INIT_ALLOCA(&char_buf, 4);
    i = buf_write_character(&char_buf, c);
    j = 0;
    if (i-- > 0) {
      buf_write(buf, 'x');
      buf_u8_to_hex(buf, char_buf.ptr.pu8[j++]);
      while (i--) {
        buf_write(buf, '\\');
        buf_write(buf, 'x');
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
  buf_write(buf, '"');
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
  buf_write(buf, '"');
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
  buf_write(buf, ':');
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
  buf_write(buf, ':');
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
