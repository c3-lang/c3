/* c3
 * Copyright 2022 Thomas de Grivel <thoxdg@gmail.com>
 */
#ifndef CHARACTER_H
#define CHARACTER_H

#include "types.h"

character character_1 (const s8 *p);
bool      character_is_lowercase (character c);
bool      character_is_uppercase (character c);
sw        character_utf8 (character c, s8 *dest);
sw        character_utf8_bytes (character c);


#endif /* CHARACTER_H */
