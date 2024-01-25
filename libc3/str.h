/* c3
 * Copyright 2022,2023 kmx.io <contact@kmx.io>
 *
 * Permission is hereby granted to use this software granted the above
 * copyright notice and this permission paragraph are included in all
 * copies and substantial portions of this software.
 *
 * THIS SOFTWARE IS PROVIDED "AS-IS" WITHOUT ANY GUARANTEE OF
 * PURPOSE AND PERFORMANCE. IN NO EVENT WHATSOEVER SHALL THE
 * AUTHOR BE CONSIDERED LIABLE FOR THE USE AND PERFORMANCE OF
 * THIS SOFTWARE.
 */
/**
 * @file str.h
 * @brief Byte string manipulation functions.
 *
 * Structure to manipulate byte strings.
 */
#ifndef LIBC3_STR_H
#define LIBC3_STR_H

#include <stdarg.h>
#include <stdio.h>
#include "hash.h"
#include "types.h"

#define STR_MAX (16 * 1024 * 1024)

/* Stack allocation compatible functions */
void    str_clean (s_str *str);
s_str * str_init (s_str *str, char *free, uw size, const char *p);
s_str * str_init_1 (s_str *str, char *free, const char *p);
s_str * str_init_alloc (s_str *str, uw size, const char *p);
s_str * str_init_cast (s_str *str, const s_tag *tag);
s_str * str_init_cat (s_str *str, const s_str *a, const s_str *b);
s_str * str_init_copy (s_str *str, const s_str *src);
s_str * str_init_copy_1 (s_str *str, const char *p);
s_str * str_init_empty (s_str *str);
s_str * str_init_f (s_str *str, const char *fmt, ...);
s_str * str_init_slice (s_str *str, const s_str *src, sw start, sw end);
s_str * str_init_slice_utf8 (s_str *str, const s_str *src, sw start,
                             sw end);
s_str * str_init_vf (s_str *str, const char *fmt, va_list ap);

/* Constructors, call str_delete after use */
s_str * str_new (char *free, uw size, const char *p);
s_str * str_new_1 (char *free, const char *p);
s_str * str_new_cpy (const char *p, uw size);
s_str * str_new_copy (const s_str *src);
s_str * str_new_empty (void);
s_str * str_new_f (const char *fmt, ...);
s_str * str_new_vf (const char *fmt, va_list ap);

/* Destructor */
void str_delete (s_str *str);

/* Observers */
sw            str_character (const s_str *str, uw position,
                             character *dest);
character     str_character_escape (character c);
bool          str_character_is_reserved (character c);
sw            str_character_position (const s_str *str, character c);
bool          str_has_reserved_characters (const s_str *str);
s_str *       str_inspect (const s_str *x, s_str *dest);
sw            str_length_utf8 (const s_str *str);
sw            str_peek_bool (const s_str *src, bool *p);
sw            str_peek_character_utf8 (const s_str *src, character *p);
sw            str_peek_f32 (const s_str *src, f32 *p);
sw            str_peek_f64 (const s_str *src, f64 *p);
sw            str_peek_s8 (const s_str *src, s8 *p);
sw            str_peek_s16 (const s_str *src, s16 *p);
sw            str_peek_s32 (const s_str *src, s32 *p);
sw            str_peek_s64 (const s_str *src, s64 *p);
sw            str_peek_str (const s_str *src, s_str *p);
sw            str_peek_sym (const s_str *src, s_sym *p);
sw            str_peek_u8 (const s_str *src, u8 *p);
sw            str_peek_u16 (const s_str *src, u16 *p);
sw            str_peek_u32 (const s_str *src, u32 *p);
sw            str_peek_u64 (const s_str *src, u64 *p);
uw *          str_rindex_character (const s_str *str, character c,
                                    uw *dest);
uw *          str_sw_pos_to_uw (sw pos, uw max_pos, uw *dest);
s_str *       str_to_hex (const s_str *src, s_str *dest);
s_ident *     str_to_ident (const s_str *src, s_ident *dest);
const s_sym * str_to_sym (const s_str *src);

/* Modifiers */
sw str_read_u8 (s_str *str, u8 *p);
sw str_read_character_utf8 (s_str *str, character *c);

#endif /* LIBC3_STR_H */
