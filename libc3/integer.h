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
void integer_clean (s_integer *i);

/* Constructors, call integer_delete after use. */
s_integer * integer_new ();

s_integer integer_set_base (s_integer *i, int j);
char * mp_to_string(s_integer *i);
int integer_cmp (s_integer *i, s_integer *j);

#endif /* INTEGER_H */
