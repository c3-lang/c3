/* c3
 * Copyright 2022,2023 kmx.io <contact@kmx.io>
 *
 * Permission is hereby granted to use this software excepted
 * on Apple computers granted the above copyright notice and
 * this permission paragraph are included in all copies and
 * substantial portions of this software.
 *
 * THIS SOFTWARE IS PROVIDED "AS-IS" WITHOUT ANY GUARANTEE OF
 * PURPOSE AND PERFORMANCE. IN NO EVENT WHATSOEVER SHALL THE
 * AUTHOR BE CONSIDERED LIABLE FOR THE USE AND PERFORMANCE OF
 * THIS SOFTWARE.
 */
/**' ,
 * @file integer.h
 * @brief Integer manipulation functions.
 *
 * Structure to manipulate byte strings.
 */
#ifndef INTEGER_H
#define INTEGER_H

#include <stdarg.h>
#include <stdio.h>
#include "types.h"

#define MP_IS_ZERO(a) ((a)->used == 0)

/* Stack allocation compatible functions */
s_integer * integer_init (s_integer *i);
s_integer * integer_init_1 (s_integer *i, const s8 *p);
s_integer * integer_init_zero (s_integer *i);
void        integer_clean (s_integer *i);

/* Modifiers */
s_integer * integer_abs (const s_integer *a, s_integer *dest);
s_integer * integer_add (const s_integer *a, const s_integer *b,
                         s_integer *dest);
s_integer * integer_band (const s_integer *a, const s_integer *b,
                          s_integer *dest);
s_integer * integer_bnot (const s_integer *a, s_integer *dest);
s_integer * integer_bor (const s_integer *a, const s_integer *b,
                         s_integer *dest);
s_integer * integer_bxor (const s_integer *a, const s_integer *b,
                          s_integer *dest);
s_integer * integer_copy (const s_integer *a, s_integer *dest);
s_integer * integer_div (const s_integer *a, const s_integer *b,
                         s_integer *dest);
s_integer * integer_gcd (const s_integer *a, const s_integer *b,
                         s_integer *dest);
s_integer * integer_lcm (const s_integer *a, const s_integer *b,
                         s_integer *dest);
s_integer * integer_lshift (s_integer *a, sw b, s_integer *dest);
s_integer * integer_mod (const s_integer *a, const s_integer *b,
                         s_integer *dest);
s_integer * integer_mul (const s_integer *a, const s_integer *b,
                         s_integer *dest);
s_integer * integer_neg (const s_integer *a, s_integer *dest);
s_integer * integer_pow (const s_integer *a, const s_integer *b,
                         s_integer *dest);
s_integer * integer_sqrt (const s_integer *a, s_integer *dest);
s_integer * integer_sub (const s_integer *a, const s_integer *b,
                         s_integer *dest);

/* Constructors, call integer_delete after use. */
s_integer * integer_new ();
s_integer * integer_new_copy (const s_integer *a);

/* Observers */
uw               integer_bits (const s_integer *i);
uw               integer_bytes (const s_integer *i);
e_bool           integer_is_negative (const s_integer *i);
e_bool           integer_is_zero (const s_integer *i);
s8               integer_to_s8 (const s_integer *i);
s16              integer_to_s16 (const s_integer *i);
s32              integer_to_s32 (const s_integer *i);
s64              integer_to_s64 (const s_integer *i);
u8               integer_to_u8 (const s_integer *i);
u16              integer_to_u16 (const s_integer *i);
u32              integer_to_u32 (const s_integer *i);
u64              integer_to_u64 (const s_integer *i);

#endif /* INTEGER_H */
