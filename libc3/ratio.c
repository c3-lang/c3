/* c3
 * Copyright 2022,2023 kmx.io <contact@kmx.io>
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
#include "assert.h"
#include <string.h>
#include <stdlib.h>
#include "buf.h"
#include "buf_parse.h"
#include "buf_inspect.h"
#include "compare.h"
#include "integer.h"
#include "ratio.h"

s_ratio * ratio_add (const s_ratio *a, const s_ratio *b, s_ratio *dest)
{
  s_ratio tmp;
  s_integer i;
  s_integer j;
  assert(a);
  assert(b);
  assert(dest);
  assert(integer_is_positive(&a->denominator));
  assert(integer_is_positive(&b->denominator));
  if (! integer_init(&tmp.numerator) || ! integer_init(&tmp.denominator)) {
    err_puts("ratio_add: failed to initialize numerator or denominator");
    return NULL;
  }
  integer_mul(&a->numerator, &b->denominator, &i);
  integer_mul(&b->numerator, &a->denominator, &j);
  integer_add(&i, &j, &tmp.numerator);
  integer_mul(&a->denominator, &b->denominator, &tmp.denominator);
  integer_clean(&i);
  integer_clean(&j);
  if (! ratio_simplify(&tmp, dest)) {
    err_puts("ratio_add: failed to simplify ratio");
    ratio_clean(&tmp);
    return NULL;
  }
  ratio_clean(&tmp);
  return dest;
}

void ratio_clean (s_ratio *r)
{
  assert(r);
  integer_clean(&r->numerator);
  integer_clean(&r->denominator);
}

s_ratio * ratio_div (const s_ratio *a, const s_ratio *b,
                           s_ratio *dest)
{
  assert(a);
  assert(b);
  assert(dest);
  assert(! integer_is_zero(&b->numerator));
  assert(integer_is_positive(&a->denominator));
  assert(integer_is_positive(&b->denominator));
  integer_mul(&a->numerator, &b->denominator, &dest->numerator);
  integer_mul(&a->denominator, &b->numerator, &dest->denominator);
  return dest;
}

s_ratio * ratio_init (s_ratio *dest)
{
  s_ratio tmp = {0};
  assert(dest);
  if (! integer_init(&tmp.numerator))
    return NULL;
  if (! integer_init(&tmp.denominator)) {
    integer_clean(&tmp.numerator);
    return NULL;
  }
  *dest = tmp;
  return dest;
}

s_ratio * ratio_init_1 (s_ratio *r, const char *p)
{
  s_integer numerator;
  s_integer denominator;
  assert(r);
  assert(p);
  // FIXME
  integer_init_1(&numerator, p);
  integer_init_u64(&denominator, 1);
  ratio_init_integer(r, &numerator, &denominator);
  return r;
}

s_ratio * ratio_init_copy (s_ratio *dest, const s_ratio *src)
{
  assert(src);
  assert(dest);
  if (integer_init_copy(&dest->numerator, &src->numerator) == NULL)
    return NULL;
  if (integer_init_copy(&dest->denominator, &src->denominator) == NULL)
    return NULL;
  return dest;
}

s_ratio * ratio_init_integer (s_ratio *r, s_integer *numerator, s_integer *denominator)
{
  s_ratio tmp = {0};
  assert(numerator);
  assert(denominator);
  assert(r);
  if (! integer_is_positive(denominator)) {
    err_puts("ratio_init_integer: invalid denominator");
    assert(! "ratio_init_integer: invalid denominator");
    return NULL;
  }
  if (! integer_init_copy(&tmp.numerator, numerator))
    return NULL;
  if (! integer_init_copy(&tmp.denominator, denominator)) {
    integer_clean(&tmp.numerator);
    return NULL;
  }
  *r = tmp;
  return r;
}

s_ratio * ratio_init_zero (s_ratio *r)
{
  s_ratio tmp = {0};
  assert(r);
  if (! integer_init_zero(&tmp.numerator))
    return NULL;
  if (! integer_init_u8(&tmp.denominator, 1)) {
    integer_clean(&tmp.numerator);
    return NULL;
  }
  *r = tmp;
  return r;
}

s_str * ratio_inspect (const s_ratio *src, s_str *dest)
{
  s_buf buf;
  sw r;
  sw size;
  assert(src);
  assert(dest);
  assert(integer_is_positive(&src->denominator));
  size = buf_inspect_ratio_size(src);
  if (size <= 0)
    return NULL;
  if (! buf_init_alloc(&buf, size))
    return NULL;
  r = buf_inspect_ratio(&buf, src);
  if (r != size) {
   err_write_1("ratio_inspect: ");
   err_inspect_sw(&r);
   err_write_1(" != ");
   err_inspect_sw(&size);
   err_write_1("\n");
   assert(! "ratio_inspect: invalid ratio");
   return NULL;
  }
  assert(buf.wpos == (uw) size);
  return buf_to_str(&buf, dest);
}

bool ratio_is_negative (const s_ratio *r)
{
  assert(r);
  assert(integer_is_positive(&r->denominator));
  return integer_is_negative(&r->numerator);
}

bool ratio_is_zero (const s_ratio *r)
{
  assert(r);
  assert(integer_is_positive(&r->denominator));
  return integer_is_zero(&r->numerator);
}

s_ratio * ratio_mul (const s_ratio *a, const s_ratio *b,
                           s_ratio *dest)
{
  assert(a);
  assert(b);
  assert(dest);
  assert(integer_is_positive(&a->denominator));
  assert(integer_is_positive(&b->denominator));
  integer_mul(&a->numerator, &b->numerator, &dest->numerator);
  integer_mul(&a->denominator, &b->denominator,
          &dest->denominator);
  return dest;
}

s_ratio * ratio_neg (const s_ratio *r, s_ratio *dest)
{
  s_ratio tmp = {0};
  assert(r);
  assert(dest);
  assert(integer_is_positive(&r->denominator));
  if (! integer_neg(&r->numerator, &tmp.numerator))
    return NULL;
  if (! integer_init_copy(&tmp.denominator, &r->denominator)) {
    integer_clean(&tmp.numerator);
    return NULL;
  }
  *dest = tmp;
  return dest;
}

s_ratio * ratio_simplify(s_ratio *r, s_ratio *dest)
{
  s_ratio tmp;
  s_integer gcd;
  assert(r);
  assert(dest);
  if (! integer_init(&tmp.numerator) || ! integer_init(&tmp.denominator)) {
    err_puts("ratio_simplify: failed to initialize numerator or denominator");
    return NULL;
  }
  integer_gcd(&r->numerator, &r->denominator, &gcd);
  if (! integer_div(&r->numerator, &gcd, &tmp.numerator)) {
    err_puts("ratio_simplify: failed to divide numerator by gcd");
    integer_clean(&tmp.numerator);
    integer_clean(&tmp.denominator);
    return NULL;
  }
  if (! integer_div(&r->denominator, &gcd, &tmp.denominator)) {
    err_puts("ratio_simplify: failed to divide denominator by gcd");
    integer_clean(&tmp.numerator);
    integer_clean(&tmp.denominator);
    return NULL;
  }
  integer_clean(&gcd);
  *dest = tmp;
  return dest;
}

s_tag * ratio_sqrt (const s_ratio *r, s_tag *dest)
{
  s_ratio tmp = {0};
  assert(r);
  assert(dest);
  assert(integer_is_positive(&r->denominator));
  if (ratio_is_negative(r)) {
    // FIXME
    err_puts("ratio_sqrt: not implemented");
    assert(! "ratio_sqrt: not implemented");
    return NULL;
  }
  if (! integer_sqrt_positive(&r->numerator, &tmp.numerator))
    return NULL;
  if (! integer_sqrt_positive(&r->denominator, &tmp.denominator)) {
    integer_clean(&tmp.numerator);
    return NULL;
  }
  dest->type = TAG_RATIO;
  dest->data.ratio = tmp;
  return dest;
}

s_ratio * ratio_sub (const s_ratio *a, const s_ratio *b,
                           s_ratio *dest)
{
  assert(a);
  assert(b);
  assert(dest);
  assert(!integer_is_zero(&a->denominator));
  assert(!integer_is_zero(&b->denominator));

  s_integer temp1, temp2;
  integer_mul(&a->numerator, &b->denominator, &temp1);
  integer_mul(&b->numerator, &a->denominator, &temp2);
  integer_sub(&temp1, &temp2, &dest->numerator);
  integer_mul(&a->denominator, &b->denominator,
          &dest->denominator);

  return dest;
}

f32 ratio_to_f32(const s_ratio *r)
{
  assert(r);
  assert(integer_is_positive(&r->denominator));

  f32 numerator = integer_to_f32(&r->numerator);
  f32 denominator = integer_to_f32(&r->denominator);

  return numerator / denominator;
}

f64 ratio_to_f64(const s_ratio *r)
{
  assert(r);
  assert(integer_is_positive(&r->denominator));

  f64 numerator = integer_to_f64(&r->numerator);
  f64 denominator = integer_to_f64(&r->denominator);

  return numerator / denominator;
}

s8 ratio_to_s8 (const s_ratio *r)
{
  assert(r);
  assert(integer_is_positive(&r->denominator));

  s8 numerator = integer_to_s8(&r->numerator);
  s8 denominator = integer_to_s8(&r->denominator);

  return numerator / denominator;
}

s16 ratio_to_s16 (const s_ratio *r)
{
  assert(r);
  assert(integer_is_positive(&r->denominator));

  s16 numerator = integer_to_s16(&r->numerator);
  s16 denominator = integer_to_s16(&r->denominator);

  return numerator / denominator;
}

s32 ratio_to_s32 (const s_ratio *r)
{
  assert(r);
  assert(integer_is_positive(&r->denominator));

  s32 numerator = integer_to_s32(&r->numerator);
  s32 denominator = integer_to_s32(&r->denominator);

  return numerator / denominator;
}

s64 ratio_to_s64 (const s_ratio *r)
{
  assert(r);
  assert(integer_is_positive(&r->denominator));

  s64 numerator = integer_to_s64(&r->numerator);
  s64 denominator = integer_to_s64(&r->denominator);

  return numerator / denominator;
}

sw ratio_to_sw (const s_ratio *r)
{
  assert(r);
  assert(integer_is_positive(&r->denominator));

  sw numerator = integer_to_sw(&r->numerator);
  sw denominator = integer_to_sw(&r->denominator);

  return numerator / denominator;
}

u8 ratio_to_u8 (const s_ratio *r)
{
  assert(r);
  assert(integer_is_positive(&r->denominator));

  u8 numerator = integer_to_u8(&r->numerator);
  u8 denominator = integer_to_u8(&r->denominator);

  return numerator / denominator;
}

u16 ratio_to_u16 (const s_ratio *r)
{
  assert(r);
  assert(integer_is_positive(&r->denominator));

  u16 numerator = integer_to_u16(&r->numerator);
  u16 denominator = integer_to_u16(&r->denominator);

  return numerator / denominator;
}

u32 ratio_to_u32 (const s_ratio *r)
{
  assert(r);
  assert(integer_is_positive(&r->denominator));

  u32 numerator = integer_to_u32(&r->numerator);
  u32 denominator = integer_to_u32(&r->denominator);

  return numerator / denominator;
}

u64 ratio_to_u64 (const s_ratio *r)
{
  assert(r);
  assert(integer_is_positive(&r->denominator));

  u64 numerator = integer_to_u64(&r->numerator);
  u64 denominator = integer_to_u64(&r->denominator);

  return numerator / denominator;
}

uw ratio_to_uw (const s_ratio *r)
{
  assert(r);
  assert(integer_is_positive(&r->denominator));

  uw numerator = integer_to_uw(&r->numerator);
  uw denominator = integer_to_uw(&r->denominator);

  return numerator / denominator;
}
