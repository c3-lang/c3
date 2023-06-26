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
/* Gen from buf_parse_s.h.in BITS=64 bits=64 */
#ifndef BUF_PARSE_S64_H
#define BUF_PARSE_S64_H

#include "types.h"

sw buf_parse_s64 (s_buf *buf, s64 *dest);
sw buf_parse_s64_base (s_buf *buf, const s_str *base,
                           bool negative, s64 *dest);

#endif /* #ifndef BUF_PARSE_S64_H */
