/* c3
 * Copyright 2022 Thomas de Grivel <thoxdg@gmail.com>
 */
#include <assert.h>
#include <err.h>
#include <stdlib.h>
#include "integer.h"

s_integer * integer_abs (const s_integer *a, s_integer *dest)
{
  sw r;
  assert(a);
  assert(dest);
  if ((r = mp_abs(&a->mp_int, &dest->mp_int)) != MP_OKAY)
    errx(1, "integer_abs: %s", mp_error_to_string(r));
  return dest;
}

s_integer * integer_add (const s_integer *a, const s_integer *b,
                         s_integer *dest)
{
  sw r;
  assert(dest);
  assert(a);
  assert(b);
  if ((r = mp_add(&a->mp_int, &b->mp_int, &dest->mp_int)) != MP_OKAY)
    errx(1, "integer_add: %s", mp_error_to_string(r));
  return dest;
}

s_integer * integer_band (const s_integer *a, const s_integer *b,
                          s_integer *dest)
{
  sw r;
  assert(a);
  assert(b);
  assert(dest);
  if ((r = mp_and(&a->mp_int, &b->mp_int, &dest->mp_int)) != MP_OKAY)
    errx(1, "integer_band: %s", mp_error_to_string(r));
  return dest;
}

s_integer * integer_bnot (const s_integer *a, s_integer *dest)
{
  sw r;
  assert(a);
  assert(dest);
  if ((r = mp_complement(&a->mp_int, &dest->mp_int)) != MP_OKAY)
    errx(1, "integer_bnot: %s", mp_error_to_string(r));
  return dest;
}

s_integer * integer_bor (const s_integer *a, const s_integer *b,
                         s_integer *dest)
{
  sw r;
  assert(a);
  assert(b);
  assert(dest);
  if ((r = mp_or(&a->mp_int, &b->mp_int, &dest->mp_int)) != MP_OKAY)
    errx(1, "integer_bor: %s", mp_error_to_string(r));
  return dest;
}

s_integer * integer_bxor (const s_integer *a, const s_integer *b,
                          s_integer *dest)
{
  sw r;
  assert(a);
  assert(b);
  assert(dest);
  if ((r = mp_xor(&a->mp_int, &b->mp_int, &dest->mp_int)) != MP_OKAY)
    errx(1, "integer_bxor: %s", mp_error_to_string(r));
  return dest;
}

void integer_clean (s_integer *dest)
{
  assert(dest);
  mp_clear(&dest->mp_int);
}

sw integer_cmp (const s_integer *a, const s_integer *b)
{
  sw r;
  assert(a);
  assert(b);
  r = mp_cmp(&a->mp_int, &b->mp_int);
  if (r == MP_GT)
    return 1;
  if (r == MP_EQ)
    return 0;
  if (r == MP_LT)
    return -1;
  errx(1, "integer_cmp: %s", mp_error_to_string(r));
  return -1;
}

s_integer * integer_copy (const s_integer *a, s_integer *dest)
{
  sw r;
  assert(a);
  assert(dest);
  if ((r = mp_copy(&a->mp_int, &dest->mp_int)) != MP_OKAY)
    errx(1, "integer_copy: %s", mp_error_to_string(r));
  return dest;
}

s_integer * integer_div (const s_integer *a, const s_integer *b,
                         s_integer *dest)
{
  sw r;
  assert(a);
  assert(b);
  assert(dest);
  if ((r = mp_div(&a->mp_int, &b->mp_int, &dest->mp_int,
                  NULL)) != MP_OKAY)
    errx(1, "integer_div: %s", mp_error_to_string(r));
  return dest;
}

s_integer * integer_gcd (const s_integer *a, const s_integer *b,
                         s_integer *dest)
{
  sw r;
  assert(a);
  assert(b);
  assert(dest);
  if ((r = mp_gcd(&a->mp_int, &b->mp_int, &dest->mp_int)) != MP_OKAY)
    errx(1, "integer_gcd: %s", mp_error_to_string(r));
  return dest;
}

s_integer * integer_init (s_integer *dest)
{
  sw r;
  assert(dest);
  if ((r = mp_init(&dest->mp_int)) != MP_OKAY)
    errx(1, "integer_init: %s", mp_error_to_string(r));
  return dest;
}

s_integer * integer_lcm (const s_integer *a, const s_integer *b,
                         s_integer *dest)
{
  sw r;
  assert(a);
  assert(b);
  assert(dest);
  if ((r = mp_lcm(&a->mp_int, &b->mp_int, &dest->mp_int)) != MP_OKAY)
    errx(1, "integer_lcm: %s", mp_error_to_string(r));
  return dest;
}

s_integer * integer_lshift (s_integer *a, sw b, s_integer *dest)
{
  sw r;
  assert(a);
  assert(dest);
  if ((r = mp_mul_2d(&a->mp_int, b, &dest->mp_int)) != MP_OKAY)
    errx(1, "integer_lshift: %s", mp_error_to_string(r));
  return dest;
}

s_integer * integer_mod (const s_integer *a, const s_integer *b,
                         s_integer *dest)
{
  sw r;
  assert(a);
  assert(b);
  assert(dest);
  if ((r = mp_mod(&a->mp_int, &b->mp_int, &dest->mp_int)) != MP_OKAY)
    errx(1, "integer_mod: %s", mp_error_to_string(r));
  return dest;
}

s_integer * integer_mul (const s_integer *a, const s_integer *b,
                         s_integer *dest)
{
  sw r;
  assert(a);
  assert(b);
  assert(dest);
  if ((r = mp_mul(&a->mp_int, &b->mp_int, &dest->mp_int)) != MP_OKAY)
    errx(1, "integer_mul: %s", mp_error_to_string(r));
  return dest;
}

s_integer * integer_neg (const s_integer *a, s_integer *dest)
{
  sw r;
  assert(a);
  assert(dest);
  if ((r = mp_neg(&a->mp_int, &dest->mp_int)) != MP_OKAY)
    errx(1, "integer_neg: %s", mp_error_to_string(r));
  return dest;
}

s_integer * integer_new_copy (const s_integer *a)
{
  s_integer *dest;
  assert(a);
  dest = malloc(sizeof(s_integer));
  if (!dest)
    errx(1, "integer_new_copy: out of memory");
  return integer_copy(a, dest);
}

s_integer * integer_pow (const s_integer *a, const s_integer *b,
                         s_integer *dest)
{
  sw r;
  assert(a);
  assert(b);
  assert(dest);
  if ((r = mp_exptmod(&a->mp_int, &b->mp_int, &dest->mp_int, &dest->mp_int))
      != MP_OKAY)
    errx(1, "integer_pow: %s", mp_error_to_string(r));
  return dest;
}

s_integer * integer_sqrt (const s_integer *a, s_integer *dest)
{
  sw r;
  assert(a);
  assert(dest);
  if ((r = mp_sqrt(&a->mp_int, &dest->mp_int)) != MP_OKAY)
    errx(1, "integer_sqrt: %s", mp_error_to_string(r));
  return dest;
}

s_integer * integer_sub (const s_integer *a, const s_integer *b,
                         s_integer *dest)
{
  sw r;
  assert(dest);
  assert(a);
  assert(b);
  if ((r = mp_sub(&a->mp_int, &b->mp_int, &dest->mp_int)) != MP_OKAY)
    errx(1, "integer_sub: %s", mp_error_to_string(r));
  return dest;
}
