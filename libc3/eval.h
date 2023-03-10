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
#ifndef EVAL_H
#define EVAL_H

#include "types.h"

s_tag *       eval_call_function (const s_call *call,
                                  s_tag *dest);
s_tag *       eval_call_macro (const s_call *call, s_tag *dest);
s_tag *       eval_fn (const s_fn *fn, s_tag *dest);
const s_tag * eval_ident (const s_ident *ident);
s_tag *       eval_progn (const s_list *program, s_tag *dest);
s_tag *       eval_tag (const s_tag *tag, s_tag *dest);

#endif /* EVAL_H */
