/* c3
 * Copyright 2022 Thomas de Grivel <thoxdg@gmail.com>
 */
/**
 * @file tag.h
 * @brief Run-time type annotations.
 *
 * Structure to add a type tag to data.
 */
#ifndef TAG_H
#define TAG_H

#include <stdarg.h>
#include <stdio.h>
#include "types.h"

/* Stack allocation compatible functions */
s_tag * tag_init (s_tag *tag, bool free, uw size, s8 *p);
s_tag * tag_init_str (s_tag *tag, s_str *str);

/* Constructors, call str_delete after use */
s_str * str_inspect (const s_str *x);
s_str * str_new (bool free, uw size, s8 *p);
s_str * str_new_1 (bool free, char *s);
s_str * str_new_cpy (uw size, const s8 *p);
s_str * str_new_dup (const s_str *src);
s_str * str_new_empty ();
s_str * str_new_f (const char *fmt, ...);
s_str * str_new_vf (const char *fmt, va_list ap);

/* Destructor */
void str_delete (s_str *str);

/* Observers */
character     str_character_escape (character c);
bool          str_character_is_reserved (character c);
sw            str_cmp (const s_str *a, const s_str *b);
bool          str_has_reserved_characters (const s_str *str);
sw            str_puts (const s_str *src);
sw            str_to_character (const s_str *src, character *c);
s_str *       str_to_hex (const s_str *src);
const s_sym * str_to_sym (const s_str *src);

/* Modifiers */
sw        str_read_character (s_str *str, character *c);

#endif /* STR_H */
