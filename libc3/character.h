/* c3
 * Copyright 2022 Thomas de Grivel <thoxdg@gmail.com>
 */
#ifndef CHARACTER_H
#define CHARACTER_H

#include "types.h"

character character_1 (const s8 *p);
sw        character_bytes (character c);
bool      character_is_upper (character c);

#endif /* CHARACTER_H */
