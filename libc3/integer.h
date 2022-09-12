/* c3
 * Copyright 2022 Thomas de Grivel <thoxdg@gmail.com>
 */
/**
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
sw integer_cmp (const s_integer *a, const s_integer *b);

#endif /* INTEGER_H */
