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
#include <dlfcn.h>
#include <err.h>
#include <stdlib.h>
#include <string.h>
#include "cfn.h"
#include "list.h"
#include "str.h"
#include "sym.h"
#include "tag.h"

ffi_type * cfn_sym_to_ffi_type (const s_sym *sym,
                                ffi_type *result_type);
s_tag * cfn_tag_init (s_tag *tag, const s_sym *type);
void * cfn_tag_to_ffi_value (s_tag *tag, const s_sym *type);

s_tag * cfn_apply (s_cfn *cfn, s_list *args, s_tag *dest)
{
  s_list *a;
  void **arg_pointers = NULL;
  void **arg_values = NULL;
  u8 arity;
  s_list *cfn_arg_type;
  sw i = 0;
  sw num_args;
  void *result;
  s_tag tmp;
  s_tag tmp2;
  assert(cfn);
  assert(cfn->arity == cfn->cif.nargs);
  num_args = list_length(args);
  arity = cfn->arity - (cfn->arg_result ? 1 : 0);
  if (arity != num_args) {
    warnx("cfn_apply: invalid number of arguments, expected %d, have %ld",
          arity, num_args);
    return NULL;
  }
  cfn_tag_init(&tmp, cfn->result_type);
  if (cfn->arg_result)
    cfn_tag_init(&tmp2, cfn->result_type);
  /* make result point to tmp value */
  result = cfn_tag_to_ffi_value(&tmp, cfn->result_type);
  if (cfn->arity) {
    if (! (arg_pointers = calloc(sizeof(void *), cfn->arity + 1)))
      err(1, "cfn_apply");
    if (! (arg_values = calloc(sizeof(void *), cfn->arity + 1)))
      err(1, "cfn_apply");
    cfn_arg_type = cfn->arg_types;
    a = args;
    while (cfn_arg_type) {
      assert(cfn_arg_type->tag.type.type == TAG_SYM);
      if (cfn_arg_type->tag.data.sym == sym_1("&result"))
        if (cfn->cif.rtype == &ffi_type_pointer) {
          arg_pointers[i] = cfn_tag_to_ffi_value(&tmp2, cfn->result_type);
          arg_values[i] = &arg_pointers[i];
        }
        else
          arg_values[i] = cfn_tag_to_ffi_value(&tmp2, cfn->result_type);
      else {
        if (cfn->cif.arg_types[i] == &ffi_type_pointer) {
          arg_pointers[i] = cfn_tag_to_ffi_value(&a->tag,
                                                cfn_arg_type->tag.data.sym);
          arg_values[i] = &arg_pointers[i];
        }
        else
          arg_values[i] = cfn_tag_to_ffi_value(&a->tag,
                                               cfn_arg_type->tag.data.sym);
        a = list_next(a);
      }
      cfn_arg_type = list_next(cfn_arg_type);
      i++;
    }
  }
  if (cfn->ptr.f) {
    ffi_call(&cfn->cif, cfn->ptr.f, result, arg_values);
    if (cfn->arg_result) {
      *dest = tmp2;
      tag_clean(&tmp);
    }
    else
      *dest = tmp;
  }
  else {
    warnx("cfn_apply: NULL function pointer");
    tag_init_void(dest);
  }
  free(arg_pointers);
  free(arg_values);
  return dest;
}

void cfn_clean (s_cfn *cfn)
{
  assert(cfn);
  str_clean(&cfn->name);
  list_delete_all(cfn->arg_types);
  if (cfn->cif.nargs)
    free(cfn->cif.arg_types);
}

s_cfn * cfn_copy (const s_cfn *cfn, s_cfn *dest)
{
  assert(cfn);
  assert(dest);
  str_copy(&cfn->name, &dest->name);
  dest->arg_result = cfn->arg_result;
  list_copy(cfn->arg_types, &dest->arg_types);
  dest->arity = cfn->arity;
  dest->cif = cfn->cif;
  dest->cif.arg_types = calloc(cfn->cif.nargs + 1, sizeof(ffi_type *));
  memcpy(dest->cif.arg_types, cfn->cif.arg_types,
         (cfn->cif.nargs + 1) * sizeof(ffi_type *));
  dest->result_type = cfn->result_type;
  dest->ptr = cfn->ptr;
  return dest;
}

s_cfn * cfn_init (s_cfn *cfn)
{
  assert(cfn);
  bzero(cfn, sizeof(s_cfn));
  return cfn;
}

s_cfn * cfn_link (s_cfn *cfn)
{
  assert(cfn);
  if (! (cfn->ptr.p = dlsym(RTLD_DEFAULT, cfn->name.ptr.ps8)))
    warnx("cfn_link: %s: %s", cfn->name.ptr.ps8, dlerror());
  return cfn;
}

s_cfn * cfn_set_type (s_cfn *cfn, s_list *arg_type,
                      const s_sym *result_type)
{
  s_list *a;
  ffi_type **arg_ffi_type = NULL;
  sw arity;
  u8 i = 0;
  ffi_type *result_ffi_type;
  ffi_status status;
  assert(cfn);
  if (! (result_ffi_type = cfn_sym_to_ffi_type(result_type, NULL)))
    return NULL;
  if ((arity = list_length(arg_type))) {
    if (arity > 255) {
      assert(arity <= 255);
      errx(1, "cfn_set_arg_types: arity > 255");
    }
    if (! (arg_ffi_type = calloc(sizeof(ffi_type *), arity + 1)))
      err(1, "cfn_set_arg_types");
    a = arg_type;
    while (a) {
      assert(i < arity);
      if (a->tag.type.type != TAG_SYM) {
        assert(! "cfn_set_type: invalid type");
        errx(1, "cfn_set_type: invalid type");
      }
      if (! (arg_ffi_type[i] = cfn_sym_to_ffi_type(a->tag.data.sym, result_ffi_type))) {
        free(arg_ffi_type);
        return NULL;
      }
      if (a->tag.data.sym == sym_1("&result"))
        cfn->arg_result = true;
      i++;
      a = list_next(a);
    }
  }
  cfn->arg_types = arg_type;
  cfn->arity = arity;
  cfn->result_type = result_type;
  status = ffi_prep_cif(&cfn->cif, FFI_DEFAULT_ABI, cfn->arity, result_ffi_type,
                        arg_ffi_type);
  if (status == FFI_BAD_TYPEDEF) {
    warnx("cfn_set_type: ffi_prep_cif: FFI_BAD_TYPEDEF");
    return NULL;
  }
  if (status == FFI_BAD_ABI) {
    warnx("cfn_set_type: ffi_prep_cif: FFI_BAD_ABI");
    return NULL;
  }
  if (status != FFI_OK) {
    warnx("cfn_set_type: ffi_prep_cif: unknown error");
    return NULL;
  }
  return cfn;
}

ffi_type * cfn_sym_to_ffi_type (const s_sym *sym, ffi_type *result_type)
{
  if (sym == sym_1("tag"))
    return &ffi_type_pointer;
  if (sym == sym_1("&result")) {
    if (! result_type)
      warnx("invalid result type: &result");
    return result_type;
  }
  if (sym == sym_1("s8"))
    return &ffi_type_sint8;
  if (sym == sym_1("s16"))
    return &ffi_type_sint16;
  if (sym == sym_1("s32"))
    return &ffi_type_sint32;
  if (sym == sym_1("s64"))
    return &ffi_type_sint64;
  if (sym == sym_1("sw"))
    return &ffi_type_sint;
  if (sym == sym_1("u8"))
    return &ffi_type_uint8;
  if (sym == sym_1("u16"))
    return &ffi_type_uint16;
  if (sym == sym_1("u32"))
    return &ffi_type_uint32;
  if (sym == sym_1("u64"))
    return &ffi_type_uint64;
  if (sym == sym_1("uw"))
    return &ffi_type_uint;
  assert(! "cfn_sym_to_ffi_type: unknown type");
  errx(1, "cfn_sym_to_ffi_type: unknown type");
  return NULL;
}

e_tag_type cfn_sym_to_tag_type (const s_sym *sym)
{
  if (sym == sym_1("void"))
    return TAG_VOID;
  if (sym == sym_1("bool"))
    return TAG_BOOL;
  if (sym == sym_1("call"))
    return TAG_CALL;
  if (sym == sym_1("call_fn"))
    return TAG_CALL_FN;
  if (sym == sym_1("call_macro"))
    return TAG_CALL_MACRO;
  if (sym == sym_1("cfn"))
    return TAG_CFN;
  if (sym == sym_1("character"))
    return TAG_CHARACTER;
  if (sym == sym_1("f32"))
    return TAG_F32;
  if (sym == sym_1("f64"))
    return TAG_F64;
  if (sym == sym_1("fn"))
    return TAG_FN;
  if (sym == sym_1("ident"))
    return TAG_IDENT;
  if (sym == sym_1("integer"))
    return TAG_INTEGER;
  if (sym == sym_1("s64"))
    return TAG_S64;
  if (sym == sym_1("s32"))
    return TAG_S32;
  if (sym == sym_1("s16"))
    return TAG_S16;
  if (sym == sym_1("s8"))
    return TAG_S8;
  if (sym == sym_1("u8"))
    return TAG_U8;
  if (sym == sym_1("u16"))
    return TAG_U16;
  if (sym == sym_1("u32"))
    return TAG_U32;
  if (sym == sym_1("u64"))
    return TAG_U64;
  if (sym == sym_1("list"))
    return TAG_LIST;
  if (sym == sym_1("ptag"))
    return TAG_PTAG;
  if (sym == sym_1("quote"))
    return TAG_QUOTE;
  if (sym == sym_1("str"))
    return TAG_STR;
  if (sym == sym_1("sym"))
    return TAG_SYM;
  if (sym == sym_1("tag"))
    return TAG_VOID;
  if (sym == sym_1("tuple"))
    return TAG_TUPLE;
  assert(! "cfn_sym_to_tag_type: unknown type");
  errx(1, "cfn_sym_to_tag_type: unknown type: %s", sym->str.ptr.ps8);
  return TAG_VOID;
}

s_tag * cfn_tag_init (s_tag *tag, const s_sym *type)
{
  assert(tag);
  bzero(tag, sizeof(s_tag));
  tag->type.type = cfn_sym_to_tag_type(type);
  return tag;
}
