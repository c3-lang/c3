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
/* Gen from u.h.in BITS=16 bits=16 */
#ifndef LIBC3_U16_H
#define LIBC3_U16_H

#include "types.h"

u16 * u16_cast (s_tag *tag, u16 *dest);
u16 * u16_copy (const u16 *src, u16 *dest);

#endif /* LIBC3_U16_H */
