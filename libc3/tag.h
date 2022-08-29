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
s_tag * tag_init_str (s_tag *tag, const s_str *src);

/* Constructors, call tag_delete after use */
s_tag * tag_new (e_tag_type type, bool free, uw size, s8 *p);
s_tag * tag_new_str (const s_str *src);

/* Call str_delete after use. */
s_str * tag_inspect (const s_tag *x);

/* Destructor */
void tag_delete (s_tag *tag);

/* Observers */
sw tag_size (const s_tag *tag);
sw tag_type_size (e_tag_type type);

/* Modifiers */

#endif /* STR_H */
