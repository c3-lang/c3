/* c3
 * Copyright 2022 Thomas de Grivel <thoxdg@gmail.com>
 */
/**
 * @file str.h
 * @brief Byte string manipulation functions.
 *
 * Structure to manipulate byte strings.
 */
#ifndef STR_H
#define STR_H

#include <stdarg.h>
#include <stdio.h>
#include "types.h"

/* Stack allocation compatible functions */
s_str * str_init (s_str *str, s8 *free, uw size, const s8 *p);
s_str * str_init_1 (s_str *str, s8 *free, const s8 *p);
s_str * str_init_alloc (s_str *str, uw size, const s8 *p);
s_str * str_init_dup (s_str *str, const s_str *src);
s_str * str_init_empty (s_str *str);
s_str * str_init_str (s_str *str, const s_str *src);
void str_clean (s_str *str);

/* Constructors, call str_delete after use */
s_str * str_inspect (const s_str *x);
s_str * str_new (s8 *free, uw size, const s8 *p);
s_str * str_new_1 (s8 *free, const s8 *p);
s_str * str_new_cpy (uw size, const s8 *p);
s_str * str_new_dup (const s_str *src);
s_str * str_new_empty ();
s_str * str_new_f (const char *fmt, ...);
s_str * str_new_vf (const char *fmt, va_list ap);

/* Destructor */
void str_delete (s_str *str);

/* Observers */
character     str_character_escape (character c);
e_bool        str_character_is_reserved (character c);
sw            str_cmp (const s_str *a, const s_str *b);
e_bool        str_has_reserved_characters (const s_str *str);
sw            str_peek_bool (const s_str *src, bool *p);
sw            str_peek_character (const s_str *src, character *p);
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
sw            str_peek_tag_list (const s_str *src, s_tag_list *p);
s_str *       str_to_hex (const s_str *src, s_str *dest);
s_ident *     str_to_ident (const s_str *src, s_ident *dest);
const s_sym * str_to_sym (const s_str *src);

/* Modifiers */
sw str_read (s_str *str, u8 *p);
sw str_read_character (s_str *str, character *c);

#endif /* STR_H */
