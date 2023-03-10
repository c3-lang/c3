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
#ifndef FACTS_WITH_H
#define FACTS_WITH_H

#include "types.h"

s_facts_with_cursor * facts_with (const s_facts *facts,
                                  s_facts_with_cursor *cursor,
                                  p_facts_spec spec);

s_facts_cursor * facts_with_0 (const s_facts *facts,
                               s_facts_cursor *cursor,
                               s_tag *var_subject,
                               s_tag *var_predicate,
                               s_tag *var_object);

s_facts_cursor * facts_with_1_2 (const s_facts *facts,
                                 s_facts_cursor *cursor,
                                 const s_tag *subject,
                                 const s_tag *predicate,
                                 const s_tag *object,
                                 s_tag *var_subject,
                                 s_tag *var_predicate,
                                 s_tag *var_object);

s_facts_cursor * facts_with_3 (const s_facts *facts,
                               s_facts_cursor *cursor,
                               const s_tag *subject,
                               const s_tag *predicate,
                               const s_tag *object);

s_facts_cursor * facts_with_tags (const s_facts *facts,
                                  s_facts_cursor *cursor,
                                  s_tag *subject,
                                  s_tag *predicate,
                                  s_tag *object);

#endif /* FACTS_WITH_H */
