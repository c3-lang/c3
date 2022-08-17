/* str
 * Copyright 2022 Thomas de Grivel <thoxdg@gmail.com>
 */
/**
 * @file str.h
 * @brief Byte strings.
 *
 * Structure to manipulate byte strings.
 */
#ifndef STR_H
#define STR_H

#include <stdio.h>
#include "types.h"

typedef struct str {
  bool free;
  u64 bytes;
  u_ptr ptr;
} s_str;

void str_init (s_str *str, bool free, uw bytes, const s8 *p);
void str_clean (s_str *str);

/* Constructors, call str_delete after use */
s_str * str_1 (const char *s, bool free);
s_str * str_copy (uw bytes, const s8 *p);
s_str * str_f (const char *fmt, ...);
s_str * str_new (bool free, uw bytes, const s8 *p);

/* Destructor */
void str_delete (s_str *str);

/* Observers */
sw str_fputs (s_str *str, FILE *fp);
sw str_puts (s_str *str);

#endif /* STR_H */
