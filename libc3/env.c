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
#include <assert.h>
#include <err.h>
#include <stdio.h>
#include <stdlib.h>
#include "binding.h"
#include "c3.h"
#include "error.h"
#include "error_handler.h"
#include "frame.h"

s_env g_c3_env;

void env_clean (s_env *env)
{
  assert(env);
  frame_delete_all(env->frame);
  error_handler_delete_all(env->error_handler);
  facts_clean(&env->facts);
  buf_file_close(&env->in);
  buf_clean(&env->in);
  buf_file_close(&env->out);
  buf_clean(&env->out);
  buf_file_close(&env->err);
  buf_clean(&env->err);
}

void env_error_f (s_env *env, const char *fmt, ...)
{
  va_list ap;
  s_tag tag;
  assert(env);
  assert(fmt);
  va_start(ap, fmt);
  tag.type.type = TAG_STR;
  str_init_vf(&tag.data.str, fmt, ap);
  va_end(ap);
  env_error_tag(env, &tag);
}

void env_error_tag (s_env *env, const s_tag *tag)
{
  s_error_handler *error_handler;
  assert(env);
  assert(tag);
  error_handler = env->error_handler;
  if (error_handler) {
    tag_copy(tag, &error_handler->tag);
    error_handler->backtrace = env->backtrace;
    env_longjmp(env, &error_handler->jmp_buf);
    /* never reached */
    return;
  }
  if (buf_file_is_open(&env->err)) {
    buf_write_1(&env->err, "error: ");
    buf_inspect_tag(&env->err, tag);
    buf_write_1(&env->err, "\n");
    return;
  }
}

s_tag * env_eval_call (s_env *env, const s_call *call, s_tag *dest)
{
  s_call c;
  s_facts_with_cursor cursor;
  s_tag *result;
  s_tag tag_fn;
  s_tag tag_function;
  s_tag tag_ident;
  s_tag tag_is_a;
  s_tag tag_macro;
  s_tag tag_module;
  s_tag tag_module_name;
  s_tag tag_sym;
  s_tag tag_var_fn;
  assert(env);
  assert(call);
  assert(dest);
  call_copy(call, &c);
  ident_resolve_module(&c.ident, env);
  tag_init_ident(&tag_ident, &c.ident);
  tag_init_1(    &tag_fn,       ":fn");
  tag_init_1(    &tag_function, ":function");
  tag_init_1(    &tag_is_a,     ":is-a");
  tag_init_1(    &tag_macro,    ":macro");
  tag_init_1(    &tag_module,   ":module");
  tag_init_sym(  &tag_module_name, c.ident.module_name);
  tag_init_sym(  &tag_sym, call->ident.sym);
  tag_init_var(  &tag_var_fn);
  facts_with(&env->facts, &cursor, (t_facts_spec) {
      &tag_module_name,
      &tag_is_a, &tag_module,       /* module exists */
      &tag_function, &tag_ident, NULL,   /* module exports symbol */
      &tag_ident, &tag_fn, &tag_var_fn,
      NULL, NULL });
  if (! facts_with_cursor_next(&cursor))
    errx(1, "symbol %s not found in module %s",
         c.ident.sym->str.ptr.ps8,
         c.ident.module_name->str.ptr.ps8);
  if (tag_var_fn.type.type != TAG_FN)
    errx(1, "%s.%s is not a function",
         c.ident.module_name->str.ptr.ps8,
         c.ident.sym->str.ptr.ps8);
  c.fn = &tag_var_fn.data.fn;
  facts_with_cursor_clean(&cursor);
  facts_with(&env->facts, &cursor, (t_facts_spec) {
      &tag_ident, &tag_is_a, &tag_macro, NULL, NULL });
  if (facts_with_cursor_next(&cursor))
    result = env_eval_call_macro(env, &c, dest);
  else
    result = env_eval_call_fn(env, &c, dest);
  facts_with_cursor_clean(&cursor);
  return result;
}

s_list * env_eval_call_arguments (s_env *env, s_list *args)
{
  s_list **dest;
  s_list *result;
  dest = &result;
  while (args) {
    *dest = list_new();
    env_eval_tag(env, &args->tag, &(*dest)->tag);
    tag_init_list(&(*dest)->next, NULL);
    dest = &(*dest)->next.data.list;
    args = list_next(args);
  }
  return result;
}

s_tag * env_eval_call_fn (s_env *env, const s_call *call, s_tag *dest)
{
  s_list *args;
  s_frame frame;
  s_fn *fn;
  s_list *tmp;
  assert(env);
  assert(call);
  assert(dest);
  fn = call->fn;
  assert(fn);
  frame_init(&frame, env->frame);
  env->frame = &frame;
  args = env_eval_call_arguments(env, call->arguments);
  env_eval_equal_list(env, fn->pattern, args, &tmp);
  env_eval_progn(env, fn->algo, dest);
  env->frame = frame_clean(&frame);
  list_delete_all(tmp);
  return dest;
}

s_tag * env_eval_call_macro (s_env *env, const s_call *call, s_tag *dest)
{
  s_tag *expanded;
  assert(env);
  assert(call);
  assert(dest);
  (void) env;
  (void) call;
  (void) expanded;
  return dest;
}

bool env_eval_equal_list (s_env *env, const s_list *a, const s_list *b,
                          s_list **dest)
{
  s_list *tmp;
  s_list **t;
  t = &tmp;
  while (1) {
    if (! a && ! b) {
      *t = NULL;
      goto ok;
    }
    if (! a)
      goto ko;
    if (! b)
      goto ko;
    *t = list_new();
    if (! env_eval_equal_tag(env, &a->tag, &b->tag, &(*t)->tag))
      goto ko;
    a = list_next(a);
    b = list_next(b);
    t = &(*t)->next.data.list;
  }
 ok:
  *dest = tmp;
  return true;
 ko:
  list_delete_all(tmp);
  return false;
}

bool env_eval_equal_tag (s_env *env, const s_tag *a, const s_tag *b,
                         s_tag *dest)
{
  assert(env);
  assert(a);
  assert(b);
  assert(dest);
  if (a->type.type == TAG_IDENT) {
    if (b->type.type == TAG_IDENT)
      warnx("TAG_IDENT = TAG_IDENT");
    tag_copy(b, dest);
    frame_binding_new(env->frame, a->data.ident.sym, dest);
    return true;
  }
  if (b->type.type == TAG_IDENT) {
    tag_copy(a, dest);
    frame_binding_new(env->frame, b->data.ident.sym, dest);
    return true;
  }
  if (a->type.type != b->type.type) {
    warnx("env_eval_equal_tag: type mismatch");
    return false;
  }
  switch (a->type.type) {
  case TAG_VOID:
    tag_init_void(dest);
    return true;
  case TAG_IDENT:
    error("env_eval_equal_tag: TAG_IDENT");
  case TAG_LIST:
    tag_init_list(dest, NULL);
    return env_eval_equal_list(env, a->data.list, b->data.list,
                               &dest->data.list);
  case TAG_TUPLE:
    dest->type.type = TAG_TUPLE;
    return env_eval_equal_tuple(env, &a->data.tuple, &b->data.tuple,
                                &dest->data.tuple);
  case TAG_BOOL:
  case TAG_CALL:
  case TAG_CALL_FN:
  case TAG_CALL_MACRO:
  case TAG_CHARACTER:
  case TAG_F32:
  case TAG_F64:
  case TAG_FN:
  case TAG_INTEGER:
  case TAG_PTAG:
  case TAG_QUOTE:
  case TAG_S16:
  case TAG_S32:
  case TAG_S64:
  case TAG_S8:
  case TAG_STR:
  case TAG_SYM:
  case TAG_U16:
  case TAG_U32:
  case TAG_U64:
  case TAG_U8:
  case TAG_VAR:
    if (compare_tag(a, b)) {
      warnx("env_eval_compare_tag: value mismatch");
      return false;
    }
    tag_copy(a, dest);
    return true;
  }
  error("env_eval_equal_tag: invalid tag");
  return false;
}

bool env_eval_equal_tuple (s_env *env, const s_tuple *a,
                           const s_tuple *b, s_tuple *dest)
{
  uw i;
  s_tuple tmp;
  assert(env);
  assert(a);
  assert(b);
  assert(dest);
  if (a->count != b->count)
    return false;
  tuple_init(&tmp, a->count);
  i = 0;
  while (i < a->count) {
    if (! env_eval_equal_tag(env, a->tag + i, b->tag + i, tmp.tag + i)) {
      tuple_clean(&tmp);
      return false;
    }
    i++;
  }
  *dest = tmp;
  return true;
}

const s_tag * env_eval_ident (s_env *env, const s_ident *ident)
{
  const s_tag *tag;
  assert(env);
  assert(ident);
  if (! (tag = frame_get(env->frame, ident->sym))) {
    assert(! "env_eval_ident: unbound variable");
    errx(1, "env_eval_ident: unbound variable");
  }
  return tag;
}

s_tag * env_eval_progn (s_env *env, const s_list *program, s_tag *dest)
{
  s_tag tmp;
  assert(env);
  assert(program);
  assert(dest);
  while (program) {
    env_eval_tag(env, &program->tag, &tmp);
    program = list_next(program);
  }
  *dest = tmp;
  return dest;
}

s_tag * env_eval_tag (s_env *env, const s_tag *tag, s_tag *dest)
{
  switch (tag->type.type) {
  case TAG_VOID: return tag_init_void(dest);
  case TAG_CALL:
    return env_eval_call(env, &tag->data.call, dest);
  case TAG_CALL_FN:
    return env_eval_call_fn(env, &tag->data.call, dest);
  case TAG_CALL_MACRO:
    return env_eval_call_macro(env, &tag->data.call, dest);
  case TAG_IDENT:
    return tag_copy(env_eval_ident(env, &tag->data.ident), dest);
  case TAG_BOOL:
  case TAG_CHARACTER:
  case TAG_F32:
  case TAG_F64:
  case TAG_FN:
  case TAG_INTEGER:
  case TAG_LIST:
  case TAG_PTAG:
  case TAG_QUOTE:
  case TAG_S16:
  case TAG_S32:
  case TAG_S64:
  case TAG_S8:
  case TAG_STR:
  case TAG_SYM:
  case TAG_TUPLE:
  case TAG_U16:
  case TAG_U32:
  case TAG_U64:
  case TAG_U8:
  case TAG_VAR:
    return tag_copy(tag, dest);
  }
  assert(! "env_eval_tag: invalid tag");
  errx(1, "env_eval_tag: invalid tag");
  return NULL;
}

s_env * env_init (s_env *env)
{
  assert(env);
  env->error_handler = NULL;
  env->frame = NULL;
  buf_init_alloc(&env->in, BUF_SIZE);
  buf_file_open_r(&env->in, stdin);
  buf_init_alloc(&env->out, BUF_SIZE);
  buf_file_open_w(&env->out, stdout);
  buf_init_alloc(&env->err, BUF_SIZE);
  buf_file_open_w(&env->err, stderr);
  facts_init(&env->facts);
  str_init_1(&env->module_path, NULL, "/home/dx/c/c3-lang/c3/lib");
  if (! module_load(&env->c3_module, sym_1("C3"), &env->facts))
    return NULL;
  env->current_module = &env->c3_module;
  return env;
}

void env_longjmp (s_env *env, jmp_buf *jmp_buf)
{
  if (env->unwind_protect && *jmp_buf > env->unwind_protect->buf) {
    s_unwind_protect *unwind_protect = env->unwind_protect;
    while (unwind_protect->next && *jmp_buf > unwind_protect->next->buf) {
      unwind_protect->jmp = &unwind_protect->next->buf;
      unwind_protect = unwind_protect->next;
    }
    unwind_protect->jmp = jmp_buf;
    longjmp(env->unwind_protect->buf, 1);
  }
  longjmp(*jmp_buf, 1);
}

s_module * env_module_load (s_env *env, s_module *module,
                            const s_sym *name, s_facts *facts)
{
  s_str path;
  assert(env);
  assert(module);
  assert(name);
  assert(facts);
  module->name = name;
  module->facts = facts;
  if (! module_name_path(&env->module_path, name, &path))
    return 0;
  /*
  buf_write_1(&env->out, "module_load ");
  buf_write_str(&env->out, &name->str);
  buf_write_1(&env->out, " -> ");
  buf_write_str(&env->out, &path);
  buf_write_s8(&env->out, '\n');
  buf_flush(&env->out);
  */
  if (facts_load_file(facts, path.ptr.ps8) < 0) {
    str_clean(&path);
    return 0;
  }
  str_clean(&path);
  return module;
}

void env_pop_error_handler (s_env *env)
{
  if (env->error_handler)
    env->error_handler = error_handler_delete(env->error_handler);
}

void env_pop_unwind_protect (s_env *env)
{
  if (env->unwind_protect)
    env->unwind_protect = env->unwind_protect->next;
}

void env_push_error_handler (s_env *env, s_error_handler *error_handler)
{
  tag_init_void(&error_handler->tag);
  error_handler->next = env->error_handler;
  env->error_handler = error_handler;
}

void env_push_unwind_protect (s_env *env,
                              s_unwind_protect *unwind_protect)
{
  unwind_protect->next = env->unwind_protect;
  env->unwind_protect = unwind_protect;
}

s_tag * env_unwind_protect (s_env *env, s_tag *protected, s_list *cleanup,
                            s_tag *dest)
{
  s_tag tmp;
  s_unwind_protect unwind_protect;
  assert(env);
  assert(protected);
  if (setjmp(unwind_protect.buf)) {
    env_pop_unwind_protect(env);
    env_eval_progn(env, cleanup, &tmp);
    longjmp(*unwind_protect.jmp, 1);
  }
  env_eval_tag(env, protected, dest);
  env_pop_unwind_protect(env);
  env_eval_progn(env, cleanup, &tmp);
  return dest;
}
