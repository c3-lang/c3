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
s_tag * tag_init (s_tag *tag, e_tag_type type, bool free, uw size,
                  s8 *p);
s_tag * tag_init_bool (s_tag *tag, bool free, bool *p);
s_tag * tag_init_character (s_tag *tag, bool free, character *p);
s_tag * tag_init_str (s_tag *tag, bool free, s_str *p);
s_tag * tag_init_sym (s_tag *tag, bool free, s_sym *p);
s_tag * tag_init_f64 (s_tag *tag, bool free, f64 *p);
s_tag * tag_init_s64 (s_tag *tag, bool free, s64 *p);
s_tag * tag_init_u64 (s_tag *tag, bool free, u64 *p);

/* Constructors, call tag_delete after use */
s_tag * tag_new (e_tag_type type, bool free, uw size, s8 *p);
s_tag * tag_new_str (bool free, s_str *src);
s_tag * tag_new_bool (bool free, bool *p);
s_tag * tag_new_character (bool free, character *p);
s_tag * tag_new_sym (bool free, const s_sym *sym);
s_tag * tag_new_f64 (bool free, f64 *f);
s_tag * tag_new_s64 (bool free, s64 *i);
s_tag * tag_new_u64 (bool free, u64 *i);

/* Call str_delete after use. */
s_str * tag_inspect (const s_tag *x);

/* Destructor */
void tag_delete (s_tag *tag);

/* Observers */
sw tag_size (const s_tag *tag);
sw tag_type_size (e_tag_type type);

/* Modifiers */

#endif /* STR_H */
