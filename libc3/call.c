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
#include <assert.h>
#include <string.h>
#include "buf.h"
#include "buf_inspect.h"
#include "buf_parse.h"
#include "call.h"
#include "cfn.h"
#include "facts_with.h"
#include "facts_with_cursor.h"
#include "fn.h"
#include "ident.h"
#include "list.h"
#include "tag.h"

void call_clean (s_call *call)
{
  assert(call);
  list_delete_all(call->arguments);
  if (call->cfn)
    cfn_delete(call->cfn);
  if (call->fn)
    fn_delete(call->fn);
}

s_call * call_copy (const s_call *src, s_call *dest)
{
  assert(src);
  assert(dest);
  ident_copy(&src->ident, &dest->ident);
  list_copy(src->arguments, &dest->arguments);
  dest->cfn = src->cfn;
  dest->fn = src->fn;
  return dest;
}

bool call_get (s_call *call, s_facts *facts)
{
  s_facts_with_cursor cursor;
  s_tag tag_cfn;
  s_tag tag_fn;
  s_tag tag_ident;
  s_tag tag_is_a;
  s_tag tag_macro;
  s_tag tag_module_name;
  s_tag tag_special_operator;
  s_tag tag_sym;
  s_tag tag_symbol;
  s_tag tag_var;
  tag_init_1(    &tag_cfn,      ":cfn");
  tag_init_1(    &tag_fn,       ":fn");
  tag_init_ident(&tag_ident, &call->ident);
  tag_init_1(    &tag_is_a,     ":is_a");
  tag_init_1(    &tag_macro,    ":macro");
  tag_init_sym(  &tag_module_name, call->ident.module_name);
  tag_init_1(    &tag_special_operator, ":special_operator");
  tag_init_sym(  &tag_sym, call->ident.sym);
  tag_init_1(    &tag_symbol,   ":symbol");
  tag_init_var(  &tag_var);
  facts_with_cursor_clean(&cursor);
  facts_with(facts, &cursor, (t_facts_spec) {
      &tag_module_name,
      &tag_symbol, &tag_ident,    /* module exports symbol */
      NULL, NULL });
  if (! facts_with_cursor_next(&cursor)) {
    warnx("symbol %s not found in module %s",
          call->ident.sym->str.ptr.ps8,
          call->ident.module_name->str.ptr.ps8);
    facts_with_cursor_clean(&cursor);
    return false;
  }
  facts_with_cursor_clean(&cursor);
  facts_with(facts, &cursor, (t_facts_spec) {
      &tag_ident, &tag_fn, &tag_var,
      NULL, NULL });
  if (facts_with_cursor_next(&cursor)) {
    if (tag_var.type != TAG_FN)
      errx(1, "%s.%s is not a function",
           call->ident.module_name->str.ptr.ps8,
           call->ident.sym->str.ptr.ps8);
    call->fn = fn_new_copy(&tag_var.data.fn);
  }
  facts_with_cursor_clean(&cursor);
  facts_with(facts, &cursor, (t_facts_spec) {
      &tag_ident, &tag_cfn, &tag_var,
      NULL, NULL });
  if (facts_with_cursor_next(&cursor)) {
    if (tag_var.type != TAG_CFN)
      errx(1, "%s.%s is not a C function",
           call->ident.module_name->str.ptr.ps8,
           call->ident.sym->str.ptr.ps8);
    call->cfn = cfn_new_copy(&tag_var.data.cfn);
  }
  facts_with_cursor_clean(&cursor);
  facts_with(facts, &cursor, (t_facts_spec) {
      &tag_ident, &tag_is_a, &tag_macro, NULL, NULL });
  if (facts_with_cursor_next(&cursor)) {
    if (call->fn)
      call->fn->macro = true;
    if (call->cfn)
      call->cfn->macro = true;
  }
  facts_with_cursor_clean(&cursor);
  facts_with(facts, &cursor, (t_facts_spec) {
      &tag_ident, &tag_is_a, &tag_special_operator, NULL, NULL});
  if (facts_with_cursor_next(&cursor)) {
    if (call->fn)
      call->fn->special_operator = true;
    if (call->cfn)
      call->cfn->special_operator = true;
  }
  facts_with_cursor_clean(&cursor);
  return true;
}

s_call * call_init (s_call *call)
{
  assert(call);
  bzero(call, sizeof(s_call));
  return call;
}

s_call * call_init_1 (s_call *call, const s8 *p)
{
  s_buf buf;
  buf_init_1(&buf, p);
  if (buf_parse_call(&buf, call) != (sw) strlen(p))
    errx(1, "invalid call: %s", p);
  buf_clean(&buf);
  return call;
}

s_call * call_init_op (s_call *call)
{
  assert(call);
  bzero(call, sizeof(s_call));
  call->arguments = list_new(NULL, list_new(NULL, NULL));
  return call;
}

s_call * call_init_op_unary (s_call *call)
{
  assert(call);
  bzero(call, sizeof(s_call));
  call->arguments = list_new(NULL, NULL);
  return call;
}

s_str * call_inspect (const s_call *call, s_str *dest)
{
  sw size;
  s_buf tmp;
  size = buf_inspect_call_size(call);
  if (size < 0) {
    assert(! "error");
    return NULL;
  }
  buf_init_alloc(&tmp, size);
  buf_inspect_call(&tmp, call);
  assert(tmp.wpos == tmp.size);
  return buf_to_str(&tmp, dest);
}
