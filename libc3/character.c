/* c3
 * Copyright 2022 Thomas de Grivel <thoxdg@gmail.com>
 */
#include "character.h"
#include "str.h"

character character_1 (const s8 *p)
{
  s_str stra;
  str_init_1(&stra, false, p);
  return str_to_character(&stra);
}

sw character_bytes (character c)
{
  if (c < 0)
    return -1;
  if (c < 0x80)
    return 1;
  if (c < 0x800)
    return 2;
  if (c < 0x10000)
    return 3;
  if (c < 0x11000)
    return 4;
  return -1;
}

bool character_is_upper (character c)
{
  return 'A' <= c && c <= 'Z';
}
