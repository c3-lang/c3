/* c3
 * Copyright 2022 Thomas de Grivel <thoxdg@gmail.com>
 */
#include <assert.h>
#include <err.h>
#include <stdlib.h>
#include "integer.h"

s_integer * integer_init (s_integer *i)
{
  int r;
  assert(i);
  if ((r = mp_init(&i->mp_int)) != MP_OKAY)
    err(1, "integer_init: %s", mp_error_to_string(r));
  return i;
}

void integer_clean(s_integer *i)
{
  assert(i);
  mp_clear(&i->mp_int);
}

//Basic operations

s_integer * integer_copy (s_integer *i, s_integer *j)
{
  int r;
  assert(i);
  assert(j);
  if ((r = mp_copy(&i->mp_int, &j->mp_int)) != MP_OKAY)
    err(1, "integer_copy: %s", mp_error_to_string(r));
  return j;
}

s_integer * integer_new_copy (s_integer *i)
{
  s_integer *j;
  assert(i);
  j = malloc(sizeof(s_integer));
  if (!j)
    err(1, "integer_new_copy: malloc");
  return integer_copy(i, j);
}

int integer_cmp (s_integer *i, s_integer *j)
{
  int r;
  assert(i);
  assert(j);
  r = mp_cmp(&i->mp_int, &j->mp_int);
  if (r == MP_GT)
    return 1;
  if (r == MP_EQ)
    return 0;
  return -1;
}

s_integer * integer_add (s_integer *i, s_integer *j, s_integer *k)
{
  int r;
  assert(i);
  assert(j);
  assert(k);
  if ((r = mp_add(&i->mp_int, &j->mp_int, &k->mp_int)) != MP_OKAY)
    err(1, "integer_add: %s", mp_error_to_string(r));
  return k;
}

s_integer * integer_sub (s_integer *i, s_integer *j, s_integer *k)
{
  int r;
  assert(i);
  assert(j);
  assert(k);
  if ((r = mp_sub(&i->mp_int, &j->mp_int, &k->mp_int)) != MP_OKAY)
    err(1, "integer_sub: %s", mp_error_to_string(r));
  return k;
}

s_integer * integer_mul (s_integer *i, s_integer *j, s_integer *k)
{
  int r;
  assert(i);
  assert(j);
  assert(k);
  if ((r = mp_mul(&i->mp_int, &j->mp_int, &k->mp_int)) != MP_OKAY)
    err(1, "integer_mul: %s", mp_error_to_string(r));
  return k;
}

s_integer * integer_div (s_integer *i, s_integer *j, s_integer *k)
{
  int r;
  assert(i);
  assert(j);
  assert(k);
  if ((r = mp_div(&i->mp_int, &j->mp_int, &k->mp_int, NULL)) != MP_OKAY)
    err(1, "integer_div: %s", mp_error_to_string(r));
  return k;
}

s_integer * integer_mod (s_integer *i, s_integer *j, s_integer *k)
{
  int r;
  assert(i);
  assert(j);
  assert(k);
  if ((r = mp_mod(&i->mp_int, &j->mp_int, &k->mp_int)) != MP_OKAY)
    err(1, "integer_mod: %s", mp_error_to_string(r));
  return k;
}

s_integer * integer_pow (s_integer *i, s_integer *j, s_integer *k)
{
  int r;
  assert(i);
  assert(j);
  assert(k);
  if ((r = mp_exptmod(&i->mp_int, &j->mp_int, &k->mp_int, &k->mp_int))
      != MP_OKAY)
    err(1, "integer_pow: %s", mp_error_to_string(r));
  return k;
}

s_integer * integer_neg (s_integer *i, s_integer *j)
{
  int r;
  assert(i);
  assert(j);
  if ((r = mp_neg(&i->mp_int, &j->mp_int)) != MP_OKAY)
    err(1, "integer_neg: %s", mp_error_to_string(r));
  return j;
}

s_integer * integer_abs (s_integer *i, s_integer *j)
{
  int r;
  assert(i);
  assert(j);
  if ((r = mp_abs(&i->mp_int, &j->mp_int)) != MP_OKAY)
    err(1, "integer_abs: %s", mp_error_to_string(r));
  return j;
}

s_integer * integer_lshift (s_integer *i, int j, s_integer *k)
{
  int r;
  assert(i);
  assert(k);
  if ((r = mp_mul_2d(&i->mp_int, j, &k->mp_int)) != MP_OKAY)
    err(1, "integer_lshift: %s", mp_error_to_string(r));
  return k;
}

//Algebraic operations

s_integer * integer_gcd (s_integer *i, s_integer *j, s_integer *k)
{
  int r;
  assert(i);
  assert(j);
  assert(k);
  if ((r = mp_gcd(&i->mp_int, &j->mp_int, &k->mp_int)) != MP_OKAY)
    err(1, "integer_gcd: %s", mp_error_to_string(r));
  return k;
}

s_integer * integer_lcm (s_integer *i, s_integer *j, s_integer *k)
{
  int r;
  assert(i);
  assert(j);
  assert(k);
  if ((r = mp_lcm(&i->mp_int, &j->mp_int, &k->mp_int)) != MP_OKAY)
    err(1, "integer_lcm: %s", mp_error_to_string(r));
  return k;
}

s_integer * integer_sqrt (s_integer *i, s_integer *j)
{
  int r;
  assert(i);
  assert(j);
  if ((r = mp_sqrt(&i->mp_int, &j->mp_int)) != MP_OKAY)
    err(1, "integer_sqrt: %s", mp_error_to_string(r));
  return j;
}

//Bitwise operations

s_integer * integer_and (s_integer *i, s_integer *j, s_integer *k)
{
  int r;
  assert(i);
  assert(j);
  assert(k);
  if ((r = mp_and(&i->mp_int, &j->mp_int, &k->mp_int)) != MP_OKAY)
    err(1, "integer_and: %s", mp_error_to_string(r));
  return k;
}

s_integer * integer_or (s_integer *i, s_integer *j, s_integer *k)
{
  int r;
  assert(i);
  assert(j);
  assert(k);
  if ((r = mp_or(&i->mp_int, &j->mp_int, &k->mp_int)) != MP_OKAY)
    err(1, "integer_or: %s", mp_error_to_string(r));
  return k;
}

s_integer * integer_xor (s_integer *i, s_integer *j, s_integer *k)
{
  int r;
  assert(i);
  assert(j);
  assert(k);
  if ((r = mp_xor(&i->mp_int, &j->mp_int, &k->mp_int)) != MP_OKAY)
    err(1, "integer_xor: %s", mp_error_to_string(r));
  return k;
}

s_integer * integer_not (s_integer *i, s_integer *j)
{
  int r;
  assert(i);
  assert(j);
  if ((r = mp_complement(&i->mp_int, &j->mp_int)) != MP_OKAY)
    err(1, "integer_not: %s", mp_error_to_string(r));
  return j;
}

