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
#include <unistd.h>
#include "alloc.h"
#include "assert.h"
#include "array.h"
#include "binding.h"
#include "block.h"
#include "buf.h"
#include "buf_file.h"
#include "buf_inspect.h"
#include "buf_save.h"
#include "call.h"
#include "cfn.h"
#include "compare.h"
#include "data.h"
#include "env.h"
#include "error.h"
#include "error_handler.h"
#include "facts.h"
#include "facts_cursor.h"
#include "facts_with.h"
#include "facts_with_cursor.h"
#include "file.h"
#include "fn.h"
#include "fn_clause.h"
#include "frame.h"
#include "ident.h"
#include "io.h"
#include "list.h"
#include "map.h"
#include "module.h"
#include "str.h"
#include "struct.h"
#include "struct_type.h"
#include "tag.h"
#include "tag_init.h"
#include "tuple.h"
#include "integer.h"

s_env g_c3_env;

static s_env * env_init_args (s_env *env, int argc, char **argv);

void env_clean (s_env *env)
{
  assert(env);
  //facts_save_file(&env->facts, "debug.facts"); // debug
  frame_delete_all(env->frame);
  error_handler_delete_all(env->error_handler);
  facts_clean(&env->facts);
  buf_file_close(&env->in);
  buf_clean(&env->in);
  buf_file_close(&env->out);
  buf_clean(&env->out);
  buf_file_close(&env->err);
  buf_clean(&env->err);
  str_clean(&env->argv0_dir);
  str_clean(&env->module_path);
  list_delete_all(env->path);
}

s_tag * env_def (s_env *env, const s_call *call, s_tag *dest)
{
  s_struct *s;
  s_tag tag_ident;
  s_tag tag_module;
  s_tag tag_symbol;
  s_tag tag_symbol_value;
  s_tag tag_value;
  (void) env;
  assert(env);
  assert(call);
  assert(dest);
  if (call->ident.module != &g_sym_C3 ||
      call->ident.sym != &g_sym_operator_equal ||
      call->arguments->tag.type != TAG_IDENT ||
      ! list_next(call->arguments) ||
      list_next(list_next(call->arguments))) {
    err_puts("env_def: invalid assignment: expected Ident = value");
    assert(! "env_def: invalid assignment: expected Ident = value");
    return NULL;
  }
  tag_ident.type = TAG_IDENT;
  env_ident_resolve_module(env, &call->arguments->tag.data.ident,
                           &tag_ident.data.ident);
  tag_init_sym(&tag_module, tag_ident.data.ident.module);
  tag_init_sym(&tag_symbol, &g_sym_symbol);
  if (! facts_add_tags(&env->facts, &tag_module, &tag_symbol,
                       &tag_ident))
    return NULL;
  env_eval_tag(env, &list_next(call->arguments)->tag, &tag_value);
  if (tag_value.type == TAG_STRUCT &&
      (s = &tag_value.data.struct_) &&
      s->type->module == &g_sym_C3__Operator) {
    if (! env_defoperator(env, &tag_ident.data.ident.sym,
                          struct_get_sym(&tag_value.data.struct_,
                                         &g_sym_sym),
                          struct_get_tag(&tag_value.data.struct_,
                                         &g_sym_symbol_value),
                          struct_get_u8(&tag_value.data.struct_,
                                        &g_sym_operator_precedence),
                          struct_get_sym(&tag_value.data.struct_,
                                         &g_sym_operator_associativity),
                          dest)) {
      tag_clean(&tag_value);
      return NULL;
    }
  }
  else {
    tag_init_sym(&tag_symbol_value, &g_sym_symbol_value);
    if (! facts_replace_tags(&env->facts, &tag_ident, &tag_symbol_value,
                             &tag_value)) {
      tag_clean(&tag_value);
      return NULL;
    }
    tag_init_ident(dest, &tag_ident.data.ident);
  }
  tag_clean(&tag_value);
  return dest;
}

s_tag * env_defmodule (s_env *env, const s_sym **name,
                       const s_block *block, s_tag *dest)
{
  const s_sym *module;
  s_tag *result = NULL;
  s_tag tag_is_a;
  s_tag tag_module;
  s_tag tag_module_name;
  s_tag tmp = {0};
  assert(env);
  assert(name);
  assert(*name);
  assert(block);
  assert(dest);
  module = env->current_module;
  env_module_is_loading_set(env, *name, true);
  env->current_module = *name;
  tag_init_sym(&tag_is_a, &g_sym_is_a);
  tag_init_sym(&tag_module, &g_sym_module);
  tag_init_sym(&tag_module_name, *name);
  if (facts_add_tags(&env->facts, &tag_module_name, &tag_is_a,
                     &tag_module) &&
      env_eval_block(env, block, &tmp)) {
    tag_clean(&tmp);
    tag_init_sym(dest, *name);
    result = dest;
  }
  env->current_module = module;
  env_module_is_loading_set(env, *name, false);
  return result;
}

s_tag * env_defoperator (s_env *env, const s_sym **name,
                         const s_sym **sym, const s_tag *symbol_value,
                         u8 operator_precedence,
                         const s_sym **operator_associativity,
                         s_tag *dest)
{
  s8 arity;
  s_tag tag_module_name;
  s_tag tag_operator;
  s_tag tag_ident;
  s_tag tag_is_a;
  s_tag tag_symbol;
  s_tag tag_sym;
  s_tag tag_arity_sym;
  s_tag tag_arity_u8;
  s_tag tag_symbol_value;
  s_tag tag_operator_precedence_sym;
  s_tag tag_operator_precedence_u8;
  s_tag tag_operator_associativity_rel;
  s_tag tag_operator_associativity_value;
  tag_init_sym(&tag_module_name, env->current_module);
  tag_init_sym(&tag_operator, &g_sym_operator);
  tag_ident.type = TAG_IDENT;
  tag_ident.data.ident.module = env->current_module;
  tag_ident.data.ident.sym = *name;
  tag_init_sym(&tag_is_a, &g_sym_is_a);
  tag_init_sym(&tag_symbol, &g_sym_symbol);
  tag_init_sym(&tag_sym, *sym);
  tag_init_sym(&tag_arity_sym, &g_sym_arity);
  arity = tag_arity(symbol_value);
  if (arity < 1) {
    err_write_1("env_defoperator: invalid arity: ");
    err_inspect_s8(&arity);
    err_write_1("\n");
    assert(! "env_defoperator: invalid arity");
  };
  tag_init_u8( &tag_arity_u8, arity);
  tag_init_sym(&tag_symbol_value, &g_sym_symbol_value);
  tag_init_sym(&tag_operator_precedence_sym,
               &g_sym_operator_precedence);
  tag_init_u8( &tag_operator_precedence_u8, operator_precedence);
  tag_init_sym(&tag_operator_associativity_rel,
               &g_sym_operator_associativity);
  tag_init_sym(&tag_operator_associativity_value,
               *operator_associativity);
  facts_add_tags(&env->facts, &tag_module_name, &tag_operator,
                 &tag_ident);
  facts_replace_tags(&env->facts, &tag_ident, &tag_is_a, &tag_operator);
  facts_replace_tags(&env->facts, &tag_ident, &tag_symbol, &tag_sym);
  facts_replace_tags(&env->facts, &tag_ident, &tag_arity_sym,
                     &tag_arity_u8);
  facts_replace_tags(&env->facts, &tag_ident, &tag_symbol_value,
                     symbol_value);
  facts_replace_tags(&env->facts, &tag_ident,
                     &tag_operator_precedence_sym,
                     &tag_operator_precedence_u8);
  facts_replace_tags(&env->facts, &tag_ident,
                     &tag_operator_associativity_rel,
                     &tag_operator_associativity_value);
  *dest = tag_ident;
  return dest;
}

void env_error_f (s_env *env, const char *fmt, ...)
{
  va_list ap;
  s_tag tag;
  assert(env);
  assert(fmt);
  va_start(ap, fmt);
  tag.type = TAG_STR;
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
    tag_init_copy(&error_handler->tag, tag);
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

bool env_eval_array (s_env *env, const s_array *array, s_array *dest)
{
  u8 *data;
  uw i;
  uw item_size;
  s_tag       *tag;
  s_tag        tag_eval;
  s_array tmp = {0};
  assert(env);
  assert(array);
  assert(dest);
  array_init_copy(&tmp, array);
  if (tmp.dimension) {
    item_size = tmp.dimensions[tmp.dimension - 1].item_size;
    if (! tmp.data && tmp.tags) {
      tmp.free_data = alloc(tmp.dimensions[0].count *
                            tmp.dimensions[0].item_size);
      if (! tmp.free_data)
        return false;
      tmp.data = tmp.free_data;
      data = tmp.data;
      tag = tmp.tags;
      i = 0;
      while (i < tmp.count) {
        if (! env_eval_tag(env, tag, &tag_eval))
          goto ko;
        if (! data_init_cast(data, tmp.element_type, &tag_eval)) {
          err_write_1("env_eval_array: cannot cast ");
          err_inspect_tag(&tag_eval);
          err_write_1(" to ");
          err_inspect_sym(&tmp.element_type);
          err_puts(".");
          goto ko;
        }
        tag_clean(&tag_eval);
        data += item_size;
        tag++;
        i++;
      }
    }
  }
  *dest = tmp;
  return true;
 ko:
  array_clean(&tmp);
  return false;
}

bool env_eval_array_tag (s_env *env, const s_array *array, s_tag *dest)
{
  s_array tmp = {0};
  if (! env_eval_array(env, array, &tmp))
    return false;
  dest->type = TAG_ARRAY;
  dest->data.array = tmp;
  return true;
}
bool env_eval_block (s_env *env, const s_block *block, s_tag *dest)
{
  uw i = 0;
  bool r;
  s_tag tmp;
  assert(env);
  assert(block);
  assert(dest);
  if (! block->count) {
    tag_init_void(dest);
    return true;
  }
  while (i < block->count - 1) {
    r = env_eval_tag(env, block->tag + i, &tmp);
    tag_clean(&tmp);
    if (! r)
      return false;
    i++;
  }
  return env_eval_tag(env, block->tag + i, dest);
}

bool env_eval_call (s_env *env, const s_call *call, s_tag *dest)
{
  s_call c;
  bool result;
  assert(env);
  assert(call);
  assert(dest);
  (void) call;
  call_init_copy(&c, call);
  env_eval_call_resolve(env, &c);
  if (c.cfn)
    result = env_eval_call_cfn(env, &c, dest);
  else if (c.fn)
    result = env_eval_call_fn(env, &c, dest);
  else {
    err_write_1("env_eval_call: could not resolve call ");
    err_inspect_ident(&c.ident);
    err_write_1("\n");
    result = false;
  }
  call_clean(&c);
  return result;
}

// FIXME: better error message (cite call function name if any)
bool env_eval_call_arguments (s_env *env, const s_list *args,
                              s_list **dest)
{
  s_list **tail;
  s_list *tmp;
  tail = &tmp;
  while (args) {
    *tail = list_new(NULL);
    if (! env_eval_tag(env, &args->tag, &(*tail)->tag)) {
      list_delete_all(tmp);
      err_write_1("env_eval_call_arguments: invalid argument: ");
      err_inspect(&args->tag);
      err_write_1("\n");
      return false;
    }
    tail = &(*tail)->next.data.list;
    args = list_next(args);
  }
  *dest = tmp;
  return true;
}

bool env_eval_call_cfn (s_env *env, const s_call *call, s_tag *dest)
{
  s_list *args = NULL;
  s_list *args_final = NULL;
  s_cfn *cfn;
  s_tag tag;
  assert(env);
  assert(call);
  assert(dest);
  cfn = call->cfn;
  assert(cfn);
  if (call->arguments) {
    if (cfn->macro || cfn->special_operator)
      args_final = call->arguments;
    else {
      if (! env_eval_call_arguments(env, call->arguments, &args))
        return false;
      args_final = args;
    }
  }
  if (! cfn_apply(cfn, args_final, &tag)) {
    list_delete_all(args);
    return false;
  }
  *dest = tag;
  list_delete_all(args);
  return true;
}

bool env_eval_call_fn (s_env *env, const s_call *call, s_tag *dest)
{
  assert(env);
  assert(call);
  assert(dest);
  return env_eval_fn_call(env, call->fn, call->arguments, dest);
}

bool env_eval_call_resolve (s_env *env, s_call *call)
{
  s_ident tmp_ident;
  const s_tag *value;
  assert(env);
  assert(call);
  if (call->ident.module == NULL &&
      (value = env_frames_get(env, call->ident.sym))) {
    if (value->type == TAG_CFN) {
      call->cfn = cfn_new_copy(&value->data.cfn);
      return true;
    }
    else if (value->type == TAG_FN) {
      call->fn = fn_new_copy(&value->data.fn);
      return true;
    }
  }
  ident_init_copy(&tmp_ident, &call->ident);
  if (! env_ident_resolve_module(env, &tmp_ident, &call->ident) ||
      ! module_ensure_loaded(call->ident.module, &env->facts) ||
      ! module_has_symbol(call->ident.module, &call->ident,
                          &env->facts) ||
      ! call_get(call, &env->facts)) {
    ident_init_copy(&call->ident, &tmp_ident);
    call->ident.module = &g_sym_C3;
    if (! module_ensure_loaded(call->ident.module, &env->facts) ||
        ! module_has_symbol(call->ident.module, &call->ident,
                            &env->facts) ||
        ! call_get(call, &env->facts)) {      
      ident_init_copy(&call->ident, &tmp_ident);
      return false;
    }
  }
  return true;
}

bool env_eval_complex (s_env *env, const s_complex *c, s_tag *dest)
{
  s_complex *tmp = NULL;
  assert(env);
  assert(c);
  assert(dest);
  tmp = alloc(sizeof(s_complex));
  if (! tmp)
    return false;
  if (! env_eval_tag(env, &c->x, &tmp->x)) {
    free(tmp);
    return false;
  }
  if (! env_eval_tag(env, &c->y, &tmp->y)) {
    tag_clean(&tmp->x);
    free(tmp);
    return false;
  }
  dest->type = TAG_COMPLEX;
  dest->data.complex = tmp;
  return true;
}

bool env_eval_equal_list (s_env *env, bool macro, const s_list *a,
                          const s_list *b, s_list **dest)
{
  s_list *a_next;
  s_list *b_next;
  s_list *tmp = NULL;
  s_list **t;
  assert(env);
  assert(dest);
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
    *t = list_new(NULL);
    if (! env_eval_equal_tag(env, macro, &a->tag, &b->tag,
                             &(*t)->tag))
      goto ko;
    a_next = list_next(a);
    b_next = list_next(b);
    if (! a_next || ! b_next) {
      if (! env_eval_equal_tag(env, macro, &a->next, &b->next,
                               &(*t)->next))
        goto ko;
      goto ok;
    }
    a = a_next;
    b = b_next;
    if (dest)
      t = &(*t)->next.data.list;
  }
 ok:
  *dest = tmp;
  return true;
 ko:
  list_delete_all(tmp);
  return false;
}

bool env_eval_equal_map (s_env *env, bool macro, const s_map *a,
                         const s_map *b, s_map *dest)
{
  const s_map *c;
  uw i;
  uw j;
  s_tag tmp;
  assert(env);
  assert(a);
  assert(b);
  assert(dest);
  if (! a->count) {
    map_init_copy(dest, b);
    return true;
  }
  if (! b->count) {
    map_init_copy(dest, a);
    return true;
  }
  if (a->count > b->count) {
    c = a;
    a = b;
    b = c;
  }
  i = 0;
  while (i < a->count) {
    j = 0;
    while (j < b->count) {
      if (! compare_tag(a->key + i, b->key + j)) {
        if (! env_eval_equal_tag(env, macro, a->value + i, b->value + j,
                                 &tmp)) {
          return false;
        }
        tag_clean(&tmp);
        goto next;
      }
      j++;
    }
    return false;
  next:
    i++;
  }
  map_init_copy(dest, b);
  return true;
}

bool env_eval_equal_tag (s_env *env, bool macro, const s_tag *a,
                         const s_tag *b, s_tag *dest)
{
  bool is_unbound_a;
  bool is_unbound_b;
  s_tag tmp_a;
  s_tag tmp_b;
  assert(env);
  assert(a);
  assert(b);
  assert(dest);
  tag_init_void(&tmp_a);
  tag_init_void(&tmp_b);
  is_unbound_a = a->type == TAG_IDENT;
  is_unbound_b = ! macro && b->type == TAG_IDENT;
  if (is_unbound_a && is_unbound_b) {
    err_write_1("env_eval_equal_tag: unbound equal on both sides: ");
    err_inspect_ident(&a->data.ident);
    err_write_1(" = ");
    err_inspect_ident(&b->data.ident);
    err_write_1(".\nTo match an existing variable please use the"
                " pin operator, e.g.: ");
    err_inspect_ident(&a->data.ident);
    err_write_1(" = ^ ");
    err_inspect_ident(&b->data.ident);
    err_write_1("\n");
    return false;
  }
  if (is_unbound_a) {
    if (macro)
      tag_init_copy(dest, b);
    else
      env_eval_tag(env, b, dest);
    frame_binding_new(env->frame, a->data.ident.sym, dest);
    return true;
  }
  if (is_unbound_b) {
    if (macro)
      tag_init_copy(dest, a);
    else
      env_eval_tag(env, a, dest);
    frame_binding_new(env->frame, b->data.ident.sym, dest);
    return true;
  }
  if (! macro &&
      a->type == TAG_CALL) {
    if (! env_eval_tag(env, a, &tmp_a))
      return false;
    if (! env_eval_equal_tag(env, macro, &tmp_a, b, dest)) {
      tag_clean(&tmp_a);
      return false;
    }
    tag_clean(&tmp_a);
    return true;
  }
  if (! macro &&
      b->type == TAG_CALL) {
    if (! env_eval_tag(env, b, &tmp_b))
      return false;
    if (! env_eval_equal_tag(env, macro, a, &tmp_b, dest)) {
      tag_clean(&tmp_b);
      return false;
    }
    tag_clean(&tmp_b);
    return true;
  }
  switch (a->type) {
  case TAG_COMPLEX:
  case TAG_F32:
  case TAG_F64:
  case TAG_F128:
  case TAG_INTEGER:
  case TAG_RATIO:
  case TAG_S8:
  case TAG_S16:
  case TAG_S32:
  case TAG_S64:
  case TAG_SW:
  case TAG_U8:
  case TAG_U16:
  case TAG_U32:
  case TAG_U64:
  case TAG_UW:
    switch (b->type) {
    case TAG_COMPLEX:
    case TAG_F32:
    case TAG_F64:
    case TAG_F128:
    case TAG_INTEGER:
    case TAG_RATIO:
    case TAG_S8:
    case TAG_S16:
    case TAG_S32:
    case TAG_S64:
    case TAG_SW:
    case TAG_U8:
    case TAG_U16:
    case TAG_U32:
    case TAG_U64:
    case TAG_UW:
      if (compare_tag(a, b)) {
        return false;
      }
      tag_init_copy(dest, a);
      return true;
    default:
      break;
    }
  default:
    break;
  }
  if (a->type != b->type) {
    return false;
  }
  switch (a->type) {
  case TAG_VOID:
    tag_init_void(dest);
    return true;
  case TAG_LIST:
    tag_init_list(dest, NULL);
    return env_eval_equal_list(env, macro, a->data.list, b->data.list,
                               &dest->data.list);
  case TAG_MAP:
    dest->type = TAG_MAP;
    return env_eval_equal_map(env, macro, &a->data.map, &b->data.map,
                              &dest->data.map);
  /*
  case TAG_STRUCT:
    dest->type = TAG_STRUCT;
    return env_eval_equal_struct(env, macro, &a->data.struct_,
                                 &b->data.struct_, &dest->data.struct_);
  */
  case TAG_TUPLE:
    dest->type = TAG_TUPLE;
    return env_eval_equal_tuple(env, macro, &a->data.tuple,
                                &b->data.tuple, &dest->data.tuple);
  case TAG_ARRAY:
  case TAG_BLOCK:
  case TAG_BOOL:
  case TAG_CALL:
  case TAG_CFN:
  case TAG_CHARACTER:
  case TAG_FACT:
  case TAG_FN:
  case TAG_IDENT:
  case TAG_INTEGER:
  case TAG_PTAG:
  case TAG_PTR:
  case TAG_PTR_FREE:
  case TAG_QUOTE:
  case TAG_STR:
  case TAG_STRUCT:
  case TAG_STRUCT_TYPE:
  case TAG_SYM:
  case TAG_VAR:
    if (compare_tag(a, b)) {
      err_puts("env_eval_compare_tag: value mismatch");
      return false;
    }
    tag_init_copy(dest, a);
    return true;
  default:
    break;
  }
  error("env_eval_equal_tag: invalid tag");
  return false;
}

bool env_eval_equal_tuple (s_env *env, bool macro, const s_tuple *a,
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
    if (! env_eval_equal_tag(env, macro, a->tag + i, b->tag + i,
                             tmp.tag + i)) {
      tuple_clean(&tmp);
      return false;
    }
    i++;
  }
  *dest = tmp;
  return true;
}

bool env_eval_fn_call (s_env *env, const s_fn *fn,
                       const s_list *arguments, s_tag *dest)
{
  s_list *args = NULL;
  const s_list *args_final = NULL;
  s_fn_clause *clause;
  s_frame frame;
  s_tag tag;
  s_list *tmp = NULL;
  assert(env);
  assert(fn);
  assert(dest);
  clause = fn->clauses;
  if (arguments) {
    if (fn->macro || fn->special_operator)
      args_final = arguments;
    else {
      if (! env_eval_call_arguments(env, arguments, &args)) {
        env->frame = frame_clean(&frame);
        return false;
      }
      args_final = args;
    }
    while (clause) {
      frame_init(&frame, env->frame);
      env->frame = &frame;
      if (env_eval_equal_list(env, fn->macro || fn->special_operator,
                              clause->pattern, args_final, &tmp))
        break;
      env->frame = frame_clean(&frame);
      clause = clause->next_clause;
    }
    if (! clause) {
      err_puts("env_eval_fn_call: no clause matching.\nTried clauses :\n");
      clause = fn->clauses;
      while (clause) {
        err_inspect_fn_pattern(clause->pattern);
        err_puts("\n");
        clause = clause->next_clause;
      }
      err_puts("\nArguments :\n");
      err_inspect_fn_pattern(args);
      err_puts("\n");
      list_delete_all(args);
      return false;
    }
  }
  else {
    frame_init(&frame, env->frame);
    env->frame = &frame;
  }
  if (! env_eval_block(env, &clause->algo, &tag)) {
    list_delete_all(args);
    list_delete_all(tmp);
    env->frame = frame_clean(&frame);
    return false;
  }
  list_delete_all(args);
  list_delete_all(tmp);
  env->frame = frame_clean(&frame);
  if (fn->macro) {
    if (! env_eval_tag(env, &tag, dest)) {
      tag_clean(&tag);
      return false;
    }
    tag_clean(&tag);
  }
  else
    *dest = tag;
  return true;
}

bool env_eval_ident (s_env *env, const s_ident *ident, s_tag *dest)
{
  const s_tag *tag;
  s_tag tmp;
  s_ident tmp_ident;
  assert(env);
  assert(ident);
  env_ident_resolve_module(env, ident, &tmp_ident);
  if (! ((tag = env_frames_get(env, tmp_ident.sym)) ||
         (tag = ident_get(&tmp_ident, &env->facts, &tmp)))) {
    err_write_1("env_eval_ident: unbound ident: ");
    err_inspect_ident(ident);
    err_write_1("\n");
    assert(! "env_eval_ident: unbound ident");
    return false;
  }
  tag_init_copy(dest, tag);
  return true;
}

bool env_eval_ident_is_bound (s_env *env, const s_ident *ident)
{
  s_ident tmp_ident;
  s_tag tmp;
  assert(env);
  assert(ident);
  if (env_frames_get(env, ident->sym))
    return true;
  env_ident_resolve_module(env, ident, &tmp_ident);
  if (ident_get(&tmp_ident, &env->facts, &tmp)) {
    tag_clean(&tmp);
    return true;
  }
  return false;
}

bool env_eval_list (s_env *env, const s_list *list, s_tag *dest)
{
  s_list *next;
  s_list *tmp = NULL;
  s_list **tail = &tmp;
  assert(env);
  assert(dest);
  while (list) {
    *tail = list_new(NULL);
    if (! env_eval_tag(env, &list->tag, &(*tail)->tag))
      goto ko;
    next = list_next(list);
    if (! next)
      if (! env_eval_tag(env, &list->next, &(*tail)->next))
        goto ko;
    tail = &(*tail)->next.data.list;
    list = next;
  }
  dest->type = TAG_LIST;
  dest->data.list = tmp;
  return true;
 ko:
  list_delete_all(tmp);
  return false;
}

bool env_eval_map (s_env *env, const s_map *map, s_tag *dest)
{
  s_map tmp;
  uw i = 0;
  assert(env);
  assert(map);
  assert(dest);
  if (! map_init(&tmp, map->count))
    return false;
  while (i < tmp.count) {
    if (! env_eval_tag(env, map->key + i, tmp.key + i) ||
        ! env_eval_tag(env, map->value + i, tmp.value + i))
      goto ko;
    i++;
  }
  map_sort(&tmp);
  dest->type = TAG_MAP;
  dest->data.map = tmp;
  return true;
 ko:
  map_clean(&tmp);
  return false;
}

bool env_eval_quote (s_env *env, const s_quote *quote, s_tag *dest)
{
  bool r;
  assert(env);
  assert(quote);
  assert(dest);
  env->quote_level++;
  r = env_eval_quote_tag(env, quote->tag, dest);
  env->quote_level--;
  return r;
}

bool env_eval_quote_array (s_env *env, const s_array *array,
                           s_tag *dest)
{
  uw i;
  s_tag *tag;
  s_array tmp = {0};
  s_tag  *tmp_tag;
  assert(env);
  assert(array);
  assert(dest);
  if (! array->dimension || array->data || ! array->tags) {
    if (! tag_init_array_copy(dest, array))
      return false;
    return true;
  }
  if (! array_init_copy_shallow(&tmp, array))
    return false;
  tag = array->tags;
  tmp.tags = alloc(tmp.count * sizeof(s_tag));
  if (! tmp.tags)
    goto ko;
  tmp_tag = tmp.tags;
  i = 0;
  while (i < array->count) {
    if (! env_eval_quote_tag(env, tag, tmp_tag))
      goto ko;
    tag++;
    tmp_tag++;
    i++;
  }
  dest->type = TAG_ARRAY;
  dest->data.array = tmp;
  return true;
 ko:
  array_clean(&tmp);
  return false;
}

bool env_eval_quote_block (s_env *env, const s_block *block, s_tag *dest)
{
  uw i = 0;
  s_block tmp;
  assert(env);
  assert(block);
  assert(dest);
  block_init(&tmp, block->count);
  while (i < block->count) {
    if (! env_eval_quote_tag(env, block->tag + i, tmp.tag + i))
      goto ko;
    i++;
  }
  dest->type = TAG_BLOCK;
  dest->data.block = tmp;
  return true;
 ko:
  block_clean(&tmp);
  return false;
}

bool env_eval_quote_call (s_env *env, const s_call *call, s_tag *dest)
{
  const s_list *arg;
  s_call        tmp = {0};
  s_list **tmp_arg_last;
  assert(call);
  assert(dest);
  if (! ident_init_copy(&tmp.ident, &call->ident))
    return false;
  tmp_arg_last = &tmp.arguments;
  arg = call->arguments;
  while (arg) {
    *tmp_arg_last = list_new(NULL);
    if (! env_eval_quote_tag(env, &arg->tag, &(*tmp_arg_last)->tag))
      goto ko;
    tmp_arg_last = &(*tmp_arg_last)->next.data.list;
    arg = list_next(arg);
  }
  // TODO: copy cfn and fn ?
  tmp.cfn = call->cfn;
  tmp.fn = call->fn;
  dest->type = TAG_CALL;
  dest->data.call = tmp;
  return true;
 ko:
  call_clean(&tmp);
  return false;
}

bool env_eval_quote_list (s_env *env, const s_list *list, s_tag *dest)
{
  s_list *next;
  s_list *tmp = NULL;
  s_list **tail = &tmp;
  assert(env);
  assert(dest);
  while (list) {
    *tail = list_new(NULL);
    if (! *tail)
      goto ko;
    if (! env_eval_quote_tag(env, &list->tag, &(*tail)->tag))
      goto ko;
    next = list_next(list);
    if (! next)
      if (! env_eval_quote_tag(env, &list->next, &(*tail)->next))
        goto ko;
    tail = &(*tail)->next.data.list;
    list = next;
  }
  dest->type = TAG_LIST;
  dest->data.list = tmp;
  return true;
 ko:
  list_delete_all(tmp);
  return false;
}

bool env_eval_quote_map (s_env *env, const s_map *map, s_tag *dest)
{
  s_map tmp;
  uw i = 0;
  assert(env);
  assert(map);
  assert(dest);
  if (! map_init(&tmp, map->count))
    return false;
  while (i < tmp.count) {
    if (! env_eval_quote_tag(env, map->key + i, tmp.key + i) ||
        ! env_eval_quote_tag(env, map->value + i, tmp.value + i))
      goto ko;
    i++;
  }
  dest->type = TAG_MAP;
  dest->data.map = tmp;
  return true;
 ko:
  map_clean(&tmp);
  return false;
}

bool env_eval_quote_quote (s_env *env, const s_quote *quote, s_tag *dest)
{
  bool r;
  s_quote tmp = {0};
  assert(env);
  assert(quote);
  assert(dest);
  tmp.tag = tag_new();
  if (! tmp.tag)
    return false;
  env->quote_level++;
  r = env_eval_quote_tag(env, quote->tag, tmp.tag);
  env->quote_level--;
  if (! r)
    return false;
  dest->type = TAG_QUOTE;
  dest->data.quote = tmp;
  return true;
}

bool env_eval_quote_struct (s_env *env, const s_struct *s, s_tag *dest)
{
  uw i;
  s_struct *t;
  s_tag tmp = {0};
  assert(env);
  assert(s);
  assert(dest);
  tmp.type = TAG_STRUCT;
  t = &tmp.data.struct_;
  if (s->data || ! s->tag) {
    if (! struct_init_copy(t, s))
      return false;
    *dest = tmp;
    return true;
  }
  t->type = s->type;
  t->tag = alloc(t->type->map.count * sizeof(s_tag));
  if (! t->tag)
    return false;
  i = 0;
  while (i < t->type->map.count) {
    if (! env_eval_quote_tag(env, s->tag + i, t->tag + i))
      goto ko;
    i++;
  }
  *dest = tmp;
  return true;
 ko:
  struct_clean(t);
  return false;
}

// Like tag_init_copy excepted that the unquote parts get evaluated.
bool env_eval_quote_tag (s_env *env, const s_tag *tag, s_tag *dest)
{
  assert(env);
  assert(tag);
  assert(dest);
  switch (tag->type) {
  case TAG_ARRAY:
    return env_eval_quote_array(env, &tag->data.array, dest);
  case TAG_BLOCK:
    return env_eval_quote_block(env, &tag->data.block, dest);
  case TAG_CALL:
    return env_eval_quote_call(env, &tag->data.call, dest);
  case TAG_LIST:
    return env_eval_quote_list(env, tag->data.list, dest);
  case TAG_MAP:
    return env_eval_quote_map(env, &tag->data.map, dest);
  case TAG_QUOTE:
    return env_eval_quote_quote(env, &tag->data.quote, dest);
  case TAG_STRUCT:
    return env_eval_quote_struct(env, &tag->data.struct_, dest);
  case TAG_TUPLE:
    return env_eval_quote_tuple(env, &tag->data.tuple, dest);
  case TAG_UNQUOTE:
    return env_eval_quote_unquote(env, &tag->data.unquote, dest);
  case TAG_VOID:
  case TAG_BOOL:
  case TAG_CFN:
  case TAG_CHARACTER:
  case TAG_COMPLEX:
  case TAG_F32:
  case TAG_F64:
  case TAG_F128:
  case TAG_FACT:
  case TAG_FN:
  case TAG_IDENT:
  case TAG_INTEGER:
  case TAG_PTAG:
  case TAG_PTR:
  case TAG_PTR_FREE:
  case TAG_RATIO:
  case TAG_S8:
  case TAG_S16:
  case TAG_S32:
  case TAG_S64:
  case TAG_STR:
  case TAG_STRUCT_TYPE:
  case TAG_SW:
  case TAG_SYM:
  case TAG_U8:
  case TAG_U16:
  case TAG_U32:
  case TAG_U64:
  case TAG_UW:
  case TAG_VAR:
    if (! tag_init_copy(dest, tag))
      return false;
    return true;
  }
  err_puts("env_eval_quote_tag: invalid tag type");
  assert(! "env_eval_quote_tag: invalid tag type");
  return false;
}

bool env_eval_quote_tuple (s_env *env, const s_tuple *tuple, s_tag *dest)
{
  uw i = 0;
  s_tuple tmp;
  assert(env);
  assert(tuple);
  assert(dest);
  tuple_init(&tmp, tuple->count);
  while (i < tuple->count) {
    if (! env_eval_quote_tag(env, tuple->tag + i, tmp.tag + i))
      goto ko;
    i++;
  }
  dest->type = TAG_TUPLE;
  dest->data.tuple = tmp;
  return true;
 ko:
  tuple_clean(&tmp);
  return false;
}

bool env_eval_quote_unquote (s_env *env, const s_unquote *unquote, s_tag *dest)
{
  bool r;
  s_tag tmp;
  assert(env);
  assert(unquote);
  assert(dest);
  if (env->unquote_level >= env->quote_level) {
    err_puts("env_eval_quote_unquote: unquote outside of a quote");
    assert(! "env_eval_quote_unquote: unquote outside of a quote");
    return false;
  }
  env->unquote_level++;
  if (env->unquote_level == 1) {
    if (env->quote_level == 1)
      r = env_eval_tag(env, unquote->tag, &tmp);
    else
      r = tag_init_copy(&tmp, unquote->tag) ? true : false;
  }
  else
    r = tag_init_unquote_copy(&tmp, unquote) ? true : false;
  env->unquote_level--;
  if (! r)
    return false;
  *dest = tmp;
  return true;
}

bool env_eval_str (s_env *env, const s_str *str, s_tag *dest)
{
  bool r = true;
  s_tag tmp;
  if (! str_parse_eval(str, &tmp)) {
    err_puts("env_eval_str: invalid Str");
    assert(! "env_eval_str: invalid Str");
    return false;
  }
  if (tmp.type == TAG_STR) {
    *dest = tmp;
    return true;
  }
  r = env_eval_tag(env, &tmp, dest);
  tag_clean(&tmp);
  return r;
}

bool env_eval_struct (s_env *env, const s_struct *s, s_tag *dest)
{
  uw i;
  s_struct *t;
  s_tag tag = {0};
  s_tag tmp = {0};
  const s_sym *type;
  assert(env);
  assert(s);
  assert(dest);
  tmp.type = TAG_STRUCT;
  t = &tmp.data.struct_;
  if (s->data) {
    if (! struct_init_copy(t, s))
      return false;
    *dest = tmp;
    return true;
  }
  t->type = s->type;
  if (! struct_allocate(t))
    return false;
  i = 0;
  while (i < t->type->map.count) {
    if (t->type->map.value[i].type == TAG_VAR)
      type = t->type->map.value[i].data.var.type;
    else
      if (! tag_type(t->type->map.value + i, &type))
        goto ko;
    if (s->tag) {
      if (! env_eval_tag(env, s->tag + i, &tag))
        goto ko;
      if (! data_init_cast((s8 *) t->data + t->type->offset[i],
                           type, &tag)) {
        err_write_1("env_eval_struct: invalid type ");
        err_write_1(tag_type_to_string(tag.type));
        err_write_1(" for key ");
        err_write_1(t->type->map.key[i].data.sym->str.ptr.pchar);
        err_write_1(", expected ");
        err_puts(tag_type_to_string(t->type->map.value[i].type));
        goto ko_tag;
      }
      tag_clean(&tag);
    }
    else {
      const void *value;
      if (! tag_to_const_pointer(t->type->map.value + i, type, &value))
        goto ko;
      if (! data_init_copy(type, (s8 *) t->data + t->type->offset[i],
                           value))
        goto ko;
    }
    i++;
  }
  *dest = tmp;
  return true;
 ko_tag:
  tag_clean(&tag);
 ko:
  struct_clean(t);
  return false;
}

bool env_eval_fn (s_env *env, const s_fn *fn, s_tag *dest)
{
  uw i;
  s_block     *src_block;
  s_fn_clause *src_clause;
  s_list      *src_pattern;
  s_tag       *src_tag;
  s_tag         tmp = {0};
  s_block      *tmp_block;
  s_fn_clause **tmp_clause;
  s_fn         *tmp_fn;
  s_list      **tmp_pattern;
  s_tag        *tmp_tag;
  assert(env);
  assert(fn);
  assert(dest);
  (void) env;
  tmp.type = TAG_FN;
  tmp_fn = &tmp.data.fn;
  src_clause = fn->clauses;
  tmp_clause = &tmp_fn->clauses;
  while (src_clause) {
    *tmp_clause = fn_clause_new(NULL);
    src_pattern = src_clause->pattern;
    tmp_pattern = &(*tmp_clause)->pattern;
    while (src_pattern) {
      *tmp_pattern = list_new(NULL);
      if (! env_eval_fn_tag(env, &src_pattern->tag,
                            &(*tmp_pattern)->tag))
        goto ko;
      src_pattern = list_next(src_pattern);
      tmp_pattern = &(*tmp_pattern)->next.data.list;
    }
    src_block = &src_clause->algo;
    tmp_block = &(*tmp_clause)->algo;
    if (! block_init(tmp_block, src_block->count))
      goto ko;
    tmp_block->short_form = src_block->short_form;
    i = 0;
    while (i < src_block->count) {
      src_tag = src_block->tag + i;
      tmp_tag = tmp_block->tag + i;
      if (! env_eval_fn_tag(env, src_tag, tmp_tag))
        goto ko;
      i++;
    }
    src_clause = src_clause->next_clause;
    tmp_clause = &(*tmp_clause)->next_clause;
  }
  tmp_fn->macro = fn->macro;
  tmp_fn->special_operator = fn->special_operator;
  *dest = tmp;
  return true;
 ko:
  fn_clean(&tmp.data.fn);
  return false;
}

bool env_eval_fn_tag (s_env *env, const s_tag *tag, s_tag *dest)
{
  assert(env);
  assert(tag);
  assert(dest);
  (void) env;
  if (! tag_init_copy(dest, tag))
    return false;
  return true;
}

bool env_eval_tag (s_env *env, const s_tag *tag, s_tag *dest)
{
  assert(env);
  assert(tag);
  assert(dest);
  switch (tag->type) {
  case TAG_VOID:
    return env_eval_void(env, NULL, dest);
  case TAG_ARRAY:
    return env_eval_array_tag(env, &tag->data.array, dest);
  case TAG_BLOCK:
    return env_eval_block(env, &tag->data.block, dest);
  case TAG_CALL:
    return env_eval_call(env, &tag->data.call, dest);
  case TAG_COMPLEX:
    return env_eval_complex(env, tag->data.complex, dest);
  case TAG_FN:
    return env_eval_fn(env, &tag->data.fn, dest);
  case TAG_IDENT:
    return env_eval_ident(env, &tag->data.ident, dest);
  case TAG_LIST:
    return env_eval_list(env, tag->data.list, dest);
  case TAG_MAP:
    return env_eval_map(env, &tag->data.map, dest);
  case TAG_QUOTE:
    return env_eval_quote(env, &tag->data.quote, dest);
  case TAG_STR:
    return env_eval_str(env, &tag->data.str, dest);
  case TAG_STRUCT:
    return env_eval_struct(env, &tag->data.struct_, dest);
  case TAG_TUPLE:
    return env_eval_tuple(env, &tag->data.tuple, dest);
  case TAG_BOOL:
  case TAG_CFN:
  case TAG_CHARACTER:
  case TAG_F32:
  case TAG_F64:
  case TAG_F128:
  case TAG_FACT:
  case TAG_INTEGER:
  case TAG_RATIO:
  case TAG_PTAG:
  case TAG_PTR:
  case TAG_PTR_FREE:
  case TAG_S8:
  case TAG_S16:
  case TAG_S32:
  case TAG_S64:
  case TAG_SW:
  case TAG_STRUCT_TYPE:
  case TAG_SYM:
  case TAG_U8:
  case TAG_U16:
  case TAG_U32:
  case TAG_U64:
  case TAG_UNQUOTE:
  case TAG_UW:
  case TAG_VAR:
    if (! tag_init_copy(dest, tag))
      return false;
    return true;
  }
  err_puts("env_eval_tag: unknown tag type");
  assert(! "env_eval_tag: unknown tag type");
  return false;
}

bool env_eval_tuple (s_env *env, const s_tuple *tuple, s_tag *dest)
{
  uw i = 0;
  s_tuple tmp;
  assert(env);
  assert(tuple);
  assert(dest);
  tuple_init(&tmp, tuple->count);
  while (i < tuple->count) {
    if (! env_eval_tag(env, tuple->tag + i, tmp.tag + i))
      return false;
    i++;
  }
  dest->type = TAG_TUPLE;
  dest->data.tuple = tmp;
  return true;
}

bool env_eval_void (s_env *env, const void *_, s_tag *dest)
{
  assert(env);
  assert(dest);
  (void) env;
  (void) _;
  tag_init_void(dest);
  return true;
}

const s_tag * env_frames_get (const s_env *env, const s_sym *name)
{
  const s_tag *tag;
  if ((tag = frame_get(env->frame, name)) ||
      (tag = frame_get(&env->global_frame, name)))
    return tag;
  return NULL;
}

bool env_ident_is_special_operator (s_env *env,
                                    const s_ident *ident)
{
  s_tag tag_ident;
  s_tag tag_is_a;
  s_tag tag_special_operator;
  assert(env);
  assert(ident);
  tag_ident.type = TAG_IDENT;
  env_ident_resolve_module(env, ident, &tag_ident.data.ident);
  tag_init_sym(&tag_is_a, &g_sym_is_a);
  tag_init_sym(&tag_special_operator, &g_sym_special_operator);
  if (facts_find_fact_by_tags(&env->facts, &tag_ident, &tag_is_a,
                              &tag_special_operator))
    return true;
  return false;
}

s_ident * env_ident_resolve_module (const s_env *env,
                                    const s_ident *ident,
                                    s_ident *dest)
{
  s_ident tmp;
  assert(env);
  assert(ident);
  ident_init_copy(&tmp, ident);
  if (! tmp.module) {
    if (! env->current_module) {
      err_puts("env_ident_resolve_module: env current module is NULL");
      assert(! "env_ident_resolve_module: env current module is NULL");
      return NULL;
    }
    tmp.module = env->current_module;
  }
  *dest = tmp;
  return dest;
}

s_env * env_init (s_env *env, int argc, char **argv)
{
  s_str path;
  assert(env);
  if (! env_init_args(env, argc, argv))
    return NULL;
  sym_init_g_sym();
  env->error_handler = NULL;
  env->frame = frame_new(NULL);         // toplevel
  frame_init(&env->global_frame, NULL); // globals
  buf_init_alloc(&env->in, BUF_SIZE);
  buf_file_open_r(&env->in, stdin);
  buf_init_alloc(&env->out, BUF_SIZE);
  buf_file_open_w(&env->out, stdout);
  buf_init_alloc(&env->err, BUF_SIZE);
  buf_file_open_w(&env->err, stderr);
  facts_init(&env->facts);
  env->path = list_new_str_1
    (NULL, "./", list_new_str_1
     (NULL, "../", list_new_str_1
      (NULL, "../Resources/", list_new_str_1
       (NULL, "../../", list_new_str_1
        (NULL, "../../../", list_new_str_1
         (NULL, "../../../../", list_new_str_1
          (NULL, "../../../../../", list_new_str_1
           (NULL, "../../../../../../", NULL))))))));
  str_init_1(&path, NULL, "lib/c3/0.1/");
  if (! file_search(&path, &g_sym_x, &env->module_path)) {
    err_puts("env_init: module_path not found");
    assert(! "env_init: module path not found");
    return NULL;
  }
  env->current_module = &g_sym_C3;
  env->quote_level = 0;
  env->unquote_level = 0;
  if (! module_load(&g_sym_C3, &env->facts)) {
    env_clean(env);
    return NULL;
  }
  return env;
}

s_env * env_init_args (s_env *env, int argc, char **argv)
{
  char a[PATH_MAX];
  s_str argv0;
  s_buf buf;
  s_str dir;
  sw r;
  assert(env);
  if (argc && argv) {
    env->argc = argc;
    env->argv = argv;
    str_init_1(&argv0, NULL, argv[0]);
    file_dirname(&argv0, &dir);
    buf_init(&buf, false, sizeof(a), a);
    if ((r = buf_write_str(&buf, &dir)) < 0 ||
        (r = buf_write_u8(&buf, '/') < 0)) {
      str_clean(&dir);
      goto ko;
    }
    str_clean(&dir);
    buf_read_to_str(&buf, &env->argv0_dir);
    return env;
  }
 ko:
  env->argc = 0;
  env->argv = NULL;
  str_init_1(&env->argv0_dir, NULL, "./");
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

bool env_module_is_loading (s_env *env, const s_sym *module)
{
  s_tag tag_module;
  s_tag tag_is_loading;
  s_tag tag_true;
  assert(env);
  assert(module);
  tag_init_sym(&tag_module, module);
  tag_init_sym(&tag_is_loading, &g_sym_is_loading);
  tag_init_bool(&tag_true, true);
  return facts_find_fact_by_tags(&env->facts, &tag_module,
                                 &tag_is_loading, &tag_true) ?
    true : false;
}

bool env_module_is_loading_set (s_env *env, const s_sym *module,
                                bool is_loading)
{
  s_tag tag_module;
  s_tag tag_is_loading;
  s_tag tag_true;
  assert(env);
  assert(module);
  tag_init_sym(&tag_module, module);
  tag_init_sym(&tag_is_loading, &g_sym_is_loading);
  tag_init_bool(&tag_true, true);
  if (is_loading) {
    if (! facts_replace_tags(&env->facts, &tag_module, &tag_is_loading,
                             &tag_true))
      return false;
  }
  else
    if (! facts_remove_fact_tags(&env->facts, &tag_module,
                                 &tag_is_loading, &tag_true))
      return false;
  return true;
}

bool env_module_load (s_env *env, const s_sym *module, s_facts *facts)
{
  s_str path;
  s_struct_type *st;
  s_list        *st_spec;
  s_tag tag_module_name;
  s_tag tag_load_time;
  s_tag tag_st = {0};
  s_tag tag_struct_type;
  s_tag tag_time;
  assert(env);
  assert(module);
  assert(facts);
  if (env_module_is_loading(env, module))
    return true;
  if (! env_module_is_loading_set(env, module, true))
    return false;
  if (! module_path(module, &env->module_path, &path)) {
    err_write_1("env_module_load: ");
    err_write_1(module->str.ptr.pchar);
    err_puts(": module_path");
    goto ko;
  }
  if (! file_access(&path, &g_sym_r))
    goto ko;
  tag_init_time(&tag_time);
  if (facts_load_file(facts, &path) < 0) {
    err_write_1("env_module_load: ");
    err_write_1(module->str.ptr.pchar);
    err_puts(": facts_load_file");
    str_clean(&path);
    goto ko;
  }
  str_clean(&path);
  tag_init_sym(&tag_module_name, module);
  tag_init_sym(&tag_load_time, &g_sym_load_time);
  facts_replace_tags(facts, &tag_module_name, &tag_load_time,
                     &tag_time);
  tag_clean(&tag_time);
  if (env_struct_type_get_spec(env, module, &st_spec)) {
    tag_init_sym(&tag_struct_type, &g_sym_struct_type);
    tag_st.type = TAG_STRUCT_TYPE;
    st = &tag_st.data.struct_type;
    struct_type_init(st, module, st_spec);
    st->clean = env_struct_type_get_clean(env, module);
    facts_replace_tags(facts, &tag_module_name, &tag_struct_type,
                       &tag_st);
    list_delete_all(st_spec);
  }
  env_module_is_loading_set(env, module, false);
  return true;
 ko:
  env_module_is_loading_set(env, module, false);
  return false;
}

bool env_module_maybe_reload (s_env *env, const s_sym *module,
                              s_facts *facts)
{
  s_str path;
  bool r = true;
  s_tag tag_load_time = {0};
  s_tag tag_mtime;
  module_load_time(module, facts, &tag_load_time);
  if (! module_path(module, &env->module_path, &path)) {
    return false;
  }
  //io_inspect_str(&path);
  if (! file_access(&path, &g_sym_r) ||
      ! file_mtime(&path, &tag_mtime)) {
    str_clean(&path);
    return false;
  }
  if (compare_tag(&tag_load_time, &tag_mtime) < 0)
    r = module_load(module, facts);
  str_clean(&path);
  tag_clean(&tag_mtime);
  return r;
}

s8 env_operator_arity (s_env *env, const s_ident *op)
{
  s_facts_cursor cursor;
  s8 r = -1;
  s_tag tag_op;
  s_tag tag_arity;
  s_tag tag_var;
  assert(env);
  assert(op);
  tag_init_ident(&tag_op, op);
  tag_init_sym(  &tag_arity, &g_sym_arity);
  tag_init_var(  &tag_var);
  facts_with_tags(&env->facts, &cursor, &tag_op, &tag_arity, &tag_var);
  if (facts_cursor_next(&cursor) &&
      tag_var.type == TAG_U8) {
    r = tag_var.data.u8;
  }
  else {
    err_write_1("env_operator_arity: arity for operator ");
    err_write_1(op->sym->str.ptr.pchar);
    err_write_1(" not found in module ");
    err_puts(op->module->str.ptr.pchar);
    r = -1;
  }
  facts_cursor_clean(&cursor);
  return r;
}

bool env_operator_find (s_env *env, const s_ident *op)
{
  s_tag tag_is_a;
  s_tag tag_op;
  s_tag tag_operator;
  assert(env);
  assert(op);
  tag_init_sym(  &tag_is_a, &g_sym_is_a);
  tag_init_ident(&tag_op, op);
  tag_init_sym(  &tag_operator, &g_sym_operator);
  return facts_find_fact_by_tags(&env->facts, &tag_op, &tag_is_a,
                                 &tag_operator) ? true : false;
}

s_ident * env_operator_ident (s_env *env, const s_ident *op,
                              s_ident *dest)
{
  assert(env);
  assert(op);
  assert(dest);
  if (env->current_module == op->module)
    dest->module = NULL;
  else
    dest->module = op->module;
  dest->sym = env_operator_symbol(env, op);
  return dest;
}

bool env_operator_is_right_associative (s_env *env, const s_ident *op)
{
  s_tag tag_assoc;
  s_tag tag_op;
  s_tag tag_right;
  assert(env);
  assert(op);
  tag_init_sym(  &tag_assoc, &g_sym_operator_associativity);
  tag_init_ident(&tag_op, op);
  tag_init_sym(  &tag_right, &g_sym_right);
  return facts_find_fact_by_tags(&env->facts, &tag_op, &tag_assoc,
                                 &tag_right) ? true : false;
}

s8 env_operator_precedence (s_env *env, const s_ident *op)
{
  s_facts_cursor cursor;
  s8 r = -1;
  s_tag tag_op;
  s_tag tag_precedence;
  s_tag tag_var;
  assert(env);
  assert(op);
  tag_init_ident(&tag_op, op);
  tag_init_sym(  &tag_precedence, &g_sym_operator_precedence);
  tag_init_var(  &tag_var);
  facts_with_tags(&env->facts, &cursor, &tag_op, &tag_precedence,
                  &tag_var);
  if (facts_cursor_next(&cursor) &&
      tag_var.type == TAG_U8) {
    r = tag_var.data.u8;
  }
  else {
    err_write_1("env_operator_precedence: precedence for operator ");
    err_write_1(op->sym->str.ptr.pchar);
    err_write_1(" not found in module ");
    err_puts(op->module->str.ptr.pchar);
    r = -1;
  }
  facts_cursor_clean(&cursor);
  return r;
}

s_ident * env_operator_resolve (s_env *env, const s_ident *op,
                                u8 arity, s_ident *dest)
{
  s_facts_with_cursor cursor;
  s_tag tag_arity;
  s_tag tag_arity_u8;
  s_tag tag_is_a;
  s_tag tag_module;
  s_tag tag_module_name;
  s_tag tag_operator;
  s_tag tag_var;
  s_tag tag_sym;
  s_tag tag_symbol;
  s_ident tmp;
  env_ident_resolve_module(env, op, &tmp);
  tag_init_sym(&tag_arity, &g_sym_arity);
  tag_init_u8( &tag_arity_u8, arity);
  tag_init_sym(&tag_is_a, &g_sym_is_a);
  tag_init_sym(&tag_module, &g_sym_module);
  tag_init_sym(&tag_module_name, tmp.module);
  tag_init_sym(&tag_operator, &g_sym_operator);
  tag_init_var(&tag_var);
  tag_init_sym(&tag_sym, tmp.sym);
  tag_init_sym(&tag_symbol, &g_sym_symbol);
  facts_with(&env->facts, &cursor, (t_facts_spec) {
      &tag_module_name, &tag_is_a, &tag_module,
      &tag_operator, &tag_var, NULL,   /* module exports operator */
      &tag_var, &tag_symbol, &tag_sym,
      &tag_arity, &tag_arity_u8,
      NULL, NULL });
  if (facts_with_cursor_next(&cursor)) {
    if (tag_var.type == TAG_IDENT) {
      *dest = tag_var.data.ident;
      facts_with_cursor_clean(&cursor);
      return dest;
    }
  }
  facts_with_cursor_clean(&cursor);
  return NULL;
}

const s_sym * env_operator_symbol (s_env *env, const s_ident *op)
{
  s_facts_cursor cursor;
  const s_sym *r;
  s_tag tag_op;
  s_tag tag_symbol;
  s_tag tag_var;
  assert(env);
  assert(op);
  tag_init_ident(&tag_op, op);
  tag_init_sym(  &tag_symbol, &g_sym_symbol);
  tag_init_var(  &tag_var);
  facts_with_tags(&env->facts, &cursor, &tag_op, &tag_symbol, &tag_var);
  if (facts_cursor_next(&cursor) &&
      tag_var.type == TAG_SYM) {
    r = tag_var.data.sym;
  }
  else {
    err_write_1("env_operator_symbol: symbol for operator ");
    err_write_1(op->sym->str.ptr.pchar);
    err_write_1(" not found in module ");
    err_puts(op->module->str.ptr.pchar);
    r = NULL;
  }
  facts_cursor_clean(&cursor);
  return r;
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

u8 env_special_operator_arity (s_env *env, const s_ident *ident)
{
  u8 arity;
  s_facts_cursor cursor;
  s_tag tag_arity;
  s_tag tag_ident;
  s_tag tag_var;
  assert(env);
  assert(ident);
  tag_ident.type = TAG_IDENT;
  env_ident_resolve_module(env, ident, &tag_ident.data.ident);
  tag_init_sym(  &tag_arity, &g_sym_arity);
  tag_init_var(  &tag_var);
  facts_with_tags(&env->facts, &cursor,
                  &tag_ident, &tag_arity, &tag_var);
  if (facts_cursor_next(&cursor)) {
    if (tag_var.type != TAG_U8 || ! tag_var.data.u8) {
      err_write_1("env_special_operator_arity: "
                  "invalid arity for special operator ");
      err_inspect_ident(&tag_ident.data.ident);
      err_write_1("\n");
      facts_cursor_clean(&cursor);
      return 0;
    }
    arity = tag_var.data.u8;
    facts_cursor_clean(&cursor);
    return arity;
  }
  facts_cursor_clean(&cursor);
  err_write_1("env_special_operator_arity: "
              "arity not found for special operator ");
  err_inspect_ident(&tag_ident.data.ident);
  err_write_1("\n");
  return 0;
}

bool env_struct_type_exists (s_env *env, const s_sym *module)
{
  s_facts_cursor cursor;
  bool result;
  s_tag tag_defstruct;
  s_tag tag_module;
  s_tag tag_var;
  assert(env);
  assert(module);
  tag_init_sym(&tag_module, module);
  tag_init_sym(&tag_defstruct, &g_sym_defstruct);
  tag_init_var(&tag_var);
  env_module_maybe_reload(env, module, &env->facts);
  facts_with_tags(&env->facts, &cursor, &tag_module,
                  &tag_defstruct, &tag_var);
  result = facts_cursor_next(&cursor) ? true : false;
  facts_cursor_clean(&cursor);
  return result;
}

const s_struct_type * env_struct_type_find (s_env *env,
                                            const s_sym *module)
{
  s_facts_with_cursor cursor;
  s_fact *found;
  const s_struct_type *result;
  s_tag tag_struct_type;
  s_tag tag_module;
  s_tag tag_var;
  const s_sym *type;
  assert(env);
  assert(module);
  tag_init_sym(&tag_module, module);
  tag_init_sym(&tag_struct_type, &g_sym_struct_type);
  tag_init_var(&tag_var);
  env_module_maybe_reload(env, module, &env->facts);
  facts_with(&env->facts, &cursor, (t_facts_spec) {
      &tag_module, &tag_struct_type, &tag_var, NULL, NULL });
  found = facts_with_cursor_next(&cursor);
  if (! found) {
    facts_with_cursor_clean(&cursor);
    return NULL;
  }
  if (found->object->type != TAG_STRUCT_TYPE) {
    tag_type(found->object, &type);
    err_write_1("env_struct_type_find: module ");
    err_inspect_sym(&module);
    err_write_1(" :struct_type is actually a ");
    err_inspect_sym(&type);
    err_write_1("\n");
    assert(! "env_struct_type_find: invalid struct_type");
    return NULL;
  }
  result = &found->object->data.struct_type;
  facts_with_cursor_clean(&cursor);
  return result;
}

f_clean env_struct_type_get_clean (s_env *env, const s_sym *module)
{
  s_facts_with_cursor cursor;
  s_fact *found;
  s_tag tag_clean;
  s_tag tag_module;
  s_tag tag_var;
  f_clean tmp;
  const s_sym *type;
  tag_init_sym(&tag_module, module);
  tag_init_sym(&tag_clean, &g_sym_clean);
  tag_init_var(&tag_var);
  facts_with(&env->facts, &cursor, (t_facts_spec) {
      &tag_module, &tag_clean, &tag_var, NULL, NULL });
  found = facts_with_cursor_next(&cursor);
  if (! found) {
    facts_with_cursor_clean(&cursor);
    return NULL;
  }
  if (found->object->type != TAG_CFN) {
    tag_type(found->object, &type);
    err_write_1("env_struct_type_get_clean: ");
    err_inspect_sym(&module);
    err_write_1(": clean is actually a ");
    err_inspect_sym(&type);
    err_write_1(", it should be a Cfn.\n");
    assert(! "env_struct_type_get_clean: invalid object");
    facts_with_cursor_clean(&cursor);
    return NULL;
  }
  if (found->object->data.cfn.arity != 1) {
    err_write_1("env_struct_type_get_clean: ");
    err_inspect_sym(&module);
    err_write_1(": clean arity is ");
    err_inspect_u8(&found->object->data.cfn.arity);
    err_write_1(", it should be 1.\n");
    assert(! "env_struct_type_get_clean: invalid arity");
    facts_with_cursor_clean(&cursor);
    return NULL;
  }
  tmp = (f_clean) found->object->data.cfn.ptr.f;
  facts_with_cursor_clean(&cursor);
  return tmp;
}

s_list ** env_struct_type_get_spec (s_env *env,
                                    const s_sym *module,
                                    s_list **dest)
{
  s_facts_with_cursor cursor;
  s_fact *found;
  s_tag tag_defstruct;
  s_tag tag_module;
  s_tag tag_var;
  s_tag tmp;
  assert(env);
  assert(module);
  assert(dest);
  tag_init_sym(&tag_defstruct, &g_sym_defstruct);
  tag_init_sym(&tag_module, module);
  tag_init_var(&tag_var);
  if (! env_module_maybe_reload(env, module, &env->facts))
    return NULL;
  facts_with(&env->facts, &cursor, (t_facts_spec) {
      &tag_module, &tag_defstruct, &tag_var, NULL, NULL });
  found = facts_with_cursor_next(&cursor);
  if (! found) {
    facts_with_cursor_clean(&cursor);
    return NULL;
  }
  if (! env_eval_tag(env, found->object, &tmp)) {
    facts_with_cursor_clean(&cursor);
    return NULL;
  }
  facts_with_cursor_clean(&cursor);
  if (tmp.type != TAG_LIST ||
      ! list_is_plist(tmp.data.list)) {
    err_write_1("env_get_struct_type_spec: module ");
    err_write_1(module->str.ptr.pchar);
    err_puts(" has a defstruct that is not a property list");
    tag_clean(&tmp);
    return NULL;
  }
  *dest = tmp.data.list;
  return dest;
}

bool env_tag_ident_is_bound (const s_env *env, const s_tag *tag,
                             s_facts *facts)
{
  s_tag tmp;
  assert(tag);
  assert(tag->type == TAG_IDENT);
  return tag->type == TAG_IDENT &&
    (env_frames_get(env, tag->data.ident.sym) ||
     ident_get(&tag->data.ident, facts, &tmp));
}

s_tag * env_unwind_protect (s_env *env, s_tag *protected, s_block *cleanup,
                            s_tag *dest)
{
  s_tag tmp;
  s_unwind_protect unwind_protect;
  assert(env);
  assert(protected);
  if (setjmp(unwind_protect.buf)) {
    env_pop_unwind_protect(env);
    env_eval_block(env, cleanup, &tmp);
    longjmp(*unwind_protect.jmp, 1);
  }
  env_eval_tag(env, protected, dest);
  env_pop_unwind_protect(env);
  env_eval_block(env, cleanup, &tmp);
  return dest;
}
