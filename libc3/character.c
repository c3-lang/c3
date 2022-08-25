/* c3
 * Copyright 2022 Thomas de Grivel <thoxdg@gmail.com>
 */
#include <assert.h>
#include "character.h"
#include "str.h"
#include "ucd.h"

character character_1 (const s8 *p)
{
  s_str stra;
  assert(p);
  str_init_1(&stra, false, p);
  return str_to_character(&stra);
}

bool character_is_digit (character c)
{
  if ('0' <= c && c <= '9')
    return true;
  return false;
}

bool character_is_lowercase (character c)
{
  if (c < UCD_MAX && g_ucd[c].flags & UCD_FLAG_LOWERCASE)
    return true;
  return false;
}

bool character_is_uppercase (character c)
{
  if (c < UCD_MAX && g_ucd[c].flags & UCD_FLAG_UPPERCASE)
    return true;
  return false;
}

sw character_utf8 (character c, s8 *dest)
{
  const u8 _00000111 = 0x07;
  const u8 _00001111 = 0x0F;
  const u8 _00011111 = 0x1F;
  const u8 _00111111 = 0x3F;
  const u8 _10000000 = 0x80;
  const u8 _11000000 = 0xC0;
  const u8 _11100000 = 0xE0;
  const u8 _11110000 = 0xF0;
  if (c == -1)
    return -1;
  if (((u64) c) < 0x80) {
    dest[0] = (s8) c;
    return 1;
  }
  if (((u64) c) < 0x800) {
    dest[0] = _11000000 | ((c >> 6) & _00011111);
    dest[1] = _10000000 | ( c       & _00111111);
    return 2;
  }
  if (((u64) c) < 0x10000) {
    dest[0] = _11100000 | ((c >> 12) & _00001111);
    dest[1] = _10000000 | ((c >>  6) & _00111111);
    dest[2] = _10000000 | ( c        & _00111111);
    return 3;
  }
  if (((u64) c) < 0x11000) {
    dest[0] = _11110000 | ((c >> 18) & _00000111);
    dest[1] = _10000000 | ((c >> 12) & _00111111);
    dest[2] = _10000000 | ((c >>  6) & _00111111);
    dest[3] = _10000000 | ( c        & _00111111);
    return 4;
  }
  return -1;
}

sw character_utf8_bytes (character c)
{
  if (c == -1)
    return -1;
  if (((u64) c) < 0x80)
    return 1;
  if (((u64) c) < 0x800)
    return 2;
  if (((u64) c) < 0x10000)
    return 3;
  if (((u64) c) < 0x11000)
    return 4;
  return -1;
}
