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
/* Gen from buf_parse_s.h.in BITS=16 bits=16 */
#ifndef LIBC3_BUF_PARSE_S16_H
#define LIBC3_BUF_PARSE_S16_H

#include "types.h"

sw buf_parse_s16 (s_buf *buf, s16 *dest);
sw buf_parse_s16_base (s_buf *buf, const s_str *base,
                           bool negative, s16 *dest);

#endif /* LIBC3_BUF_PARSE_S16_H */
