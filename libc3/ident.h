/* c3
 * Copyright 2022 Thomas de Grivel <thoxdg@gmail.com>
 */
#ifndef IDENT_H
#define IDENT_H

#include "types.h"

/* Maximum number of bytes in UTF-8 of an identifier. */
#define IDENT_MAX 1024

/* Constructors, call ident_clean after use. */
s_ident * ident_init_1 (s_ident *ident, const s8 *p);

/* Observers */

/* Returns true iff c is an ident character. */
e_bool ident_character_is_reserved (character c);

/* Returns true iff ident contains reserved characters. */
e_bool ident_has_reserved_characters (const s_ident *ident);

s_ident * ident_init (s_ident *ident, const s_sym *sym);

s_str * ident_inspect (const s_ident *ident, s_str *dest);

#endif
