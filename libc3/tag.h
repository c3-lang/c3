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
s_tag * tag_init_str (s_tag *tag, s8 *free, uw size, const s8 *p);
s_tag * tag_init_bool (s_tag *tag, bool p);
s_tag * tag_init_character (s_tag *tag, character c);
s_tag * tag_init_f64 (s_tag *tag, f64 f);
s_tag * tag_init_s64 (s_tag *tag, s64 i);
s_tag * tag_init_sym (s_tag *tag, const s_sym *p);
s_tag * tag_init_u64 (s_tag *tag, u64 i);

/* Constructors, call tag_delete after use */
s_tag * tag_new_str (s8 *free, uw size, const s8 *p);
s_tag * tag_new_bool (bool p);
s_tag * tag_new_character (character c);
s_tag * tag_new_sym (const s_sym *sym);
s_tag * tag_new_f64 (f64 f);
s_tag * tag_new_s64 (s64 i);
s_tag * tag_new_u64 (u64 i);

/* Destructor */
void tag_delete (s_tag *tag);

/* Observers */
sw tag_size (const s_tag *tag);
sw tag_type_size (e_tag_type type);

/* Call str_delete after use. */
s_str * tag_inspect (const s_tag *x);

/* Modifiers */

#endif /* STR_H */
