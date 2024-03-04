/* c3
 * Copyright 2022-2024 kmx.io <contact@kmx.io>
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
 * @file bool.h
 * @brief Boolean operations.
 *
 * Structure to manipulate booleans.
 */
#ifndef LIBC3_BOOL_H
#define LIBC3_BOOL_H

#include "types.h"

/* Stack-allocation compatible functions */
bool * bool_init_cast (bool *b, const s_tag *tag);
bool * bool_init_copy (bool *b, const bool *src);

/* Observers */
s_str * bool_inspect (bool *b, s_str *dest);

#endif /* LIBC3_BOOL_H */
