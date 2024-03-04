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
/* Gen from s.h.in BITS=W bits=w */
#include "assert.h"
#include <math.h>
#include <stdlib.h>
#include "integer.h"
#include "tag.h"
#include "tag_type.h"
#include "ratio.h"
#include "sw.h"

sw * sw_init_cast (sw *s, const s_tag *tag)
{
  switch (tag->type) {
  case TAG_BOOL:
    *s = tag->data.bool ? 1 : 0;
    return s;
  case TAG_CHARACTER:
    *s = (sw) tag->data.character;
    return s;
  case TAG_F32:
    *s = (sw) tag->data.f32;
    return s;
  case TAG_F64:
    *s = (sw) tag->data.f64;
    return s;
  case TAG_INTEGER:
    *s = integer_to_sw(&tag->data.integer);
    return s;
  case TAG_RATIO:
    *s = ratio_to_sw(&tag->data.ratio);
    return s;
  case TAG_SW:
    *s = (sw) tag->data.sw;
    return s;
  case TAG_S64:
    *s = (sw) tag->data.s64;
    return s;
  case TAG_S32:
    *s = (sw) tag->data.s32;
    return s;
  case TAG_S16:
    *s = (sw) tag->data.s16;
    return s;
  case TAG_S8:
    *s = (sw) tag->data.s8;
    return s;
  case TAG_U8:
    *s = (sw) tag->data.u8;
    return s;
  case TAG_U16:
    *s = (sw) tag->data.u16;
    return s;
  case TAG_U32:
    *s = (sw) tag->data.u32;
    return s;
  case TAG_U64:
    *s = (sw) tag->data.u64;
    return s;
  case TAG_UW:
    *s = (sw) tag->data.uw;
    return s;
  default:
    break;
  }
  err_write_1("sw_cast: cannot cast ");
  err_write_1(tag_type_to_string(tag->type));
  err_puts(" to sw");
  assert(! "sw_cast: cannot cast to sw");
  return NULL;
}

sw * sw_init_copy (sw *s, const sw *src)
{
  assert(src);
  assert(s);
  *s = *src;
  return s;
}

sw * sw_random (sw *s)
{
  arc4random_buf(s, sizeof(sw));
  return s;
}

s_tag * sw_sqrt (const sw x, s_tag *dest)
{
  assert(dest);
  if (x < 0) {
    // FIXME
    //dest->type = TAG_COMPLEX;
    return NULL;
  }
  dest->type = TAG_SW;
  dest->data.sw = (sw) sqrtl((long double) x);
  return dest;
}
