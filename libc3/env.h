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
#ifndef ENV_H
#define ENV_H

#include "types.h"

extern s_env g_c3_env;

/* Stack allocation compatible functions, call env_clean after use. */
void    env_clean (s_env *env);
s_env * env_init (s_env *env);

/* Modifiers. */
bool       env_eval_call (s_env *env, const s_call *call,
                          s_tag *dest);
bool       env_eval_call_arguments (s_env *env, s_list *args,
                                    s_list **dest);
bool       env_eval_call_cfn (s_env *env, const s_call *call,
                              s_tag *dest);
bool       env_eval_call_fn (s_env *env, const s_call *call,
                             s_tag *dest);
bool       env_eval_call_macro (s_env *env, const s_call *call,
                                s_tag *dest);
bool       env_eval_call_resolve (s_env *env, s_call *call);
bool       env_eval_call_special_operator (s_env *env,
                                           const s_call *call,
                                           s_tag *dest);
bool       env_eval_equal_list (s_env *env, const s_list *a,
                                const s_list *b, s_list **dest);
bool       env_eval_equal_tag (s_env *env, const s_tag *a,
                               const s_tag *b, s_tag *dest);
bool       env_eval_equal_tuple (s_env *env, const s_tuple *a,
                                 const s_tuple *b, s_tuple *dest);
bool       env_eval_fn (s_env *env, const s_fn *fn, s_tag *dest);
bool       env_eval_ident (s_env *env, const s_ident *ident,
                           s_tag *dest);
bool       env_eval_progn (s_env *env, const s_list *program,
                           s_tag *dest);
bool       env_eval_quote(s_env *env, const s_quote *quote,
                          s_tag *dest);
bool       env_eval_tag (s_env *env, const s_tag *tag, s_tag *dest);
s_module * env_module_load (s_env *env, s_module *module,
                            const s_sym *name, s_facts *facts);
bool       env_operator_is_binary (s_env *env, const s_ident *op);
bool       env_operator_is_right_associative (s_env *env,
                                              const s_ident *op);
bool       env_operator_is_unary (s_env *env, const s_ident *op);
s8         env_operator_precedence (s_env *env,
                                    const s_ident *op);

/* Control structures. */
void env_error_f (s_env *env, const char *fmt, ...);
void env_error_tag (s_env *env, const s_tag *tag);
void env_longjmp (s_env *env, jmp_buf *jmp_buf);

#endif /* ENV_H */
