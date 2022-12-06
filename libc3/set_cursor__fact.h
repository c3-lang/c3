/* c3
 * Copyright 2022 kmx.io <contact@kmx.io>
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
#ifndef SET_CURSOR__fact_H
#define SET_CURSOR__fact_H

#include "types.h"

void
set_cursor_init__fact (s_set__fact *set, s_set_cursor__fact *c);

s_set_item__fact *
set_cursor_next__fact (s_set_cursor__fact *c);

#endif /* SET_CURSOR__fact_H */
