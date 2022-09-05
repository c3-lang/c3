/* c3
 * Copyright 2022 Thomas de Grivel <thoxdg@gmail.com>
 */
#include <assert.h>
#include "buf.h"
#include "buf_inspect.h"
#include "character.h"
#include "str.h"
#include "sym.h"

bool ident_first_character_is_reserved (character c)
{
  return ! character_is_lowercase(c);
}

bool ident_character_is_reserved (character c)
{
  return sym_character_is_reserved(c);
}

bool ident_has_reserved_characters (const s_ident *ident)
{
  character c;
  sw r;
  s_str stra;
  str_init(&stra, false, ident->sym->str.size, ident->sym->str.ptr.p);
  if ((r = str_read_character(&stra, &c)) > 0) {
    if (ident_first_character_is_reserved(c))
      return true;
    while ((r = str_read_character(&stra, &c)) > 0) {
      if (ident_character_is_reserved(c))
        return true;
    }
  }
  if (r < 0)
    return true;
  return false;
}

s_ident * ident_init (s_ident *ident, const s_sym *sym)
{
  assert(ident);
  assert(sym);
  ident->sym = sym;
  return ident;
}

s_ident * ident_init_1 (s_ident *ident, const s8 *p)
{
  s_str tmp;
  str_init_1(&tmp, NULL, p);
  str_to_ident(&tmp, ident);
  return ident;
}

s_str * ident_inspect (const s_ident *ident, s_str *p)
{
  sw r;
  sw size;
  s_buf buf;
  size = buf_inspect_ident_size(ident);
  if (size < 0)
    return NULL;
  buf_init_alloc(&buf, size);
  r = buf_inspect_ident(&buf, ident);
  if (r != size) {
    buf_clean(&buf);
    return NULL;
  }
  return buf_to_str(&buf, p);
}
