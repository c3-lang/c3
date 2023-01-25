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
/* Gen from skiplist.h.in NAME=fact TYPE=s_fact * */
/**
 * @file skiplist__fact.h
 * @brief Skiplist of facts.
 *
 * Skiplist containing s_fact *.
 */
#ifndef SKIPLIST__fact_H
#define SKIPLIST__fact_H

#include "types.h"

#define SKIPLIST_HEIGHT_TABLE__fact(skiplist) \
  ((t_skiplist_height *) (((s_skiplist__fact *) skiplist) + 1))

#define SKIPLIST_SIZE__fact(max_height) \
  (sizeof(s_skiplist__fact) + (max_height) * sizeof(t_skiplist_height))

#define SKIPLIST_INIT_ALLOCA__fact(max_height, spacing) \
  (skiplist_init__fact(alloca(SKIPLIST_SIZE__fact(max_height)), \
                         (max_height), (spacing)))

void
skiplist_clean__fact (s_skiplist__fact *skiplist);

void
skiplist_delete__fact (s_skiplist__fact *skiplist);

s_skiplist_node__fact *
skiplist_find__fact (s_skiplist__fact *skiplist, s_fact * value);

s_skiplist__fact *
skiplist_init__fact (s_skiplist__fact *skiplist, u8 max_height, f64 spacing);

s_skiplist_node__fact *
skiplist_insert__fact (s_skiplist__fact *skiplist, s_fact * value);

s_skiplist__fact *
skiplist_new__fact (u8 max_height, f64 spacing);

s_skiplist_node__fact *
skiplist_pred__fact (s_skiplist__fact *skiplist, s_fact * value);

u8
skiplist_random_height__fact (s_skiplist__fact *skiplist);

e_bool
skiplist_remove__fact (s_skiplist__fact *skiplist, s_fact * value);

#endif /* SKIPLIST__fact_H */
