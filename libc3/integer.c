/* c3
 * Copyright 2022 Thomas de Grivel <thoxdg@gmail.com>
 */
#include <assert.h>
#include <err.h>
#include "integer.h"

s_integer * integer_init (s_integer *i)
{
  int r;
  assert(i);
  if ((r = mp_init(&i->mp)) != MP_OKAY)
    err(1, "integer_init: %s", mp_error_to_string(r));
  return i;
}

void integer_clean(s_integer *i)
{
  assert(i);
  mp_clear(&i->mp);
}

s_integer * integer_add (s_integer *result, s_integer *a, s_integer *b)
{
  int r;
  assert(result && a && b);
  if ((r = mp_add(&a->mp, &b->mp, &result->mp)) != MP_OKAY)
    err(1, "integer_add: %s", mp_error_to_string(r));
  return result;
}

