/* c3
 * Copyright 2022 Thomas de Grivel <thoxdg@gmail.com>
 */
#include "str.h"
#include "sym.h"

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
  while ((r = str_read_character(&stra, &c)) > 0) {
    if (ident_character_is_reserved(c))
      return true;
  }
  if (r < 0)
    return true;
  return false;
}
