/* c3
 * Copyright 2022,2023 kmx.io <contact@kmx.io>
 *
 * Permission is hereby granted to use this software excepted
 * on Apple computers granted the above copyright notice and
 * this permission paragraph are included in all copies and
 * substantial portions of this software.
 *
 * THIS SOFTWARE IS PROVIDED "AS-IS" WITHOUT ANY GUARANTEE OF
 * PURPOSE AND PERFORMANCE. IN NO EVENT WHATSOEVER SHALL THE
 * AUTHOR BE CONSIDERED LIABLE FOR THE USE AND PERFORMANCE OF
 * THIS SOFTWARE.
 */
#ifndef CALL_H
#define CALL_H

#include "types.h"

void     call_clean (s_call *call);
s_call * call_init_1 (s_call *call, const s8 *p);
s_call * call_copy (const s_call *src, s_call *dest);
s_str *  call_inspect (const s_call *call, s_str *dest);

#endif /* CALL_H */
