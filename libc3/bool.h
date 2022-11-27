/* c3
 * Copyright 2022 kmx.io <contact@kmx.io>
 *
 * Permission is hereby granted to use this software granted
 * the above copyright notice and this permission paragraph
 * are included in all copies and substantial portions of this
 * software.
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
#ifndef BOOL_H
#define BOOL_H

#include "hash.h"
#include "types.h"

/* Observers */
s8               bool_compare (e_bool a, e_bool b);
t_hash_context * bool_hash_update (t_hash_context *context, bool b);
s_str *          bool_inspect (e_bool b, s_str *dest);

#endif /* SYM_H */