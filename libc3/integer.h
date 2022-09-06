/* c3
 * Copyright 2022 Thomas de Grivel <thoxdg@gmail.com>
 */
/**
 * @file str.h
 * @brief Integer manipulation functions.
 *
 * Structure to manipulate byte strings.
 */
#ifndef INTEGER_H
#define INTEGER_H

#include <stdarg.h>
#include <stdio.h>
#include "types.h"
#include "../libtommath/tommath.h"

struct integer {
  mp_int mp;
};

/* Stack allocation compatible functions */
s_integer * integer_init (s_integer *i);
void integer_clean (s_integer *i);

/* Constructors, call integer_delete after use. */
s_integer * integer_new ();

#endif /* INTEGER_H */
