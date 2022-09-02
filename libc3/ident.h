/* c3
 * Copyright 2022 Thomas de Grivel <thoxdg@gmail.com>
 */
#ifndef IDENT_H
#define IDENT_H

#include "types.h"

/* Maximum number of bytes in UTF-8 of an identifier. */
#define IDENT_MAX 1024

/* Observers */

/* Returns true iff c is an ident character. */
e_bool ident_character_is_reserved (character c);

/* Returns true iff ident contains reserved characters. */
e_bool ident_has_reserved_characters (const s_ident *ident);

#endif
