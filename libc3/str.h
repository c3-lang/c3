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
void str_init (s_str *str, bool free, uw size, s8 *p);
void str_init_1 (s_str *str, bool free, s8 *p);
void str_init_dup (s_str *str, const s_str *src);
void str_init_join (s_str *str, uw count, ...);
void str_init_join_v (s_str *str, uw count, va_list ap);
void str_clean (s_str *str);

/* Constructors, call str_delete after use */
s_str * str_new (bool free, uw size, s8 *p);
s_str * str_new_1 (bool free, char *s);
s_str * str_new_cpy (uw size, const s8 *p);
s_str * str_new_dup (const s_str *src);
s_str * str_new_empty ();
s_str * str_new_f (const char *fmt, ...);
s_str * str_new_join (uw count, ...);
s_str * str_new_join_v (uw count, va_list ap);
s_str * str_new_vf (const char *fmt, va_list ap);

/* Destructor */
void str_delete (s_str *str);

/* Observers */
sw            str_cmp (const s_str *a, const s_str *b);
sw            str_fputs (const s_str *src, FILE *fp);
s_str *       str_inspect (const s_str *x);
sw            str_puts (const s_str *src);
sw            str_to_character (const s_str *src, character *c);
s_str *       str_to_hex (const s_str *src);
const s_sym * str_to_sym (const s_str *src);

/* Modifiers */
sw        str_read_character (s_str *str, character *c);
void      str_resize (s_str *str, uw size);

#endif /* STR_H */
