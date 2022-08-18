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
void str_init (s_str *str, bool free, uw bytes, const s8 *p);
void str_init_1 (s_str *str, bool free, const s8 *p);
void str_init_dup (s_str *str, s_str *src);
void str_clean (s_str *str);

/* Constructors, call str_delete after use */
s_str * str_1 (bool free, const char *s);
s_str * str_append (s_str *str, ...);
s_str * str_append_copy (s_str *str, uw bytes, const s8 *p);
s_str * str_append_1 (s_str *str, const char *s);
s_str * str_append_f (s_str *str, const char *fmt, ...);
s_str * str_append_inspect (s_str *str, s_str *x);
s_str * str_append_realloc (s_str *str, uw bytes, const s8 *p);
s_str * str_cpy (uw bytes, const s8 *p);
s_str * str_empty ();
s_str * str_f (const char *fmt, ...);
s_str * str_inspect (s_str *x);
s_str * str_new (bool free, uw bytes, const s8 *p);
s_str * str_vf (const char *fmt, va_list ap);

/* Destructor */
void str_delete (s_str *str);

/* Observers */
sw      str_cmp (s_str *a, s_str *b);
sw      str_fputs (s_str *str, FILE *fp);
sw      str_puts (s_str *str);
s_sym * str_to_sym (s_str *str);

#endif /* STR_H */
