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
#ifndef FACTS_CURSOR_H
#define FACTS_CURSOR_H

#include "types.h"

/* Stack allocation compatible functions */
s_facts_cursor * facts_cursor_init (s_facts_cursor *cursor,
                                    s_skiplist__fact *tree,
                                    s_fact *start,
                                    s_fact *end);

/* Modifiers */
s_fact * facts_cursor_next (s_facts_cursor *cursor);

#endif /* FACTS_H */
