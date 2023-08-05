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
/* Gen from buf_inspect_u_base.c.in BITS=8 bits=8 BASE=decimal */
#include "c3.h"

sw buf_inspect_u8_decimal (s_buf *buf, const u8 *u)
{
  return buf_inspect_u8_base(buf,
                                  &g_c3_base_decimal,
                                  u);
}

sw buf_inspect_u8_decimal_size (const u8 *u)
{
  return buf_inspect_u8_base_size(&g_c3_base_decimal,
                                       u);
}