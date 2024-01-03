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
#include "c3.h"

sw data_buf_inspect (const s_sym *type, s_buf *buf, const void *data)
{
  s_struct_type *st;
  if (type == sym_1("Array"))
    return buf_inspect_array(buf, data);
  if (type == sym_1("Bool"))
    return buf_inspect_bool(buf, data);
  if (type == sym_1("Call"))
    return buf_inspect_call(buf, data);
  if (type == sym_1("Cfn"))
    return buf_inspect_cfn(buf, data);
  if (type == sym_1("Character"))
    return buf_inspect_character(buf, data);
  if (type == sym_1("F32"))
    return buf_inspect_f32(buf, data);
  if (type == sym_1("F64"))
    return buf_inspect_f64(buf, data);
  if (type == sym_1("Fact"))
    return buf_inspect_fact(buf, data);
  if (type == sym_1("Fn"))
    return buf_inspect_fn(buf, data);
  if (type == sym_1("Ident"))
    return buf_inspect_ident(buf, data);
  if (type == sym_1("Integer"))
    return buf_inspect_integer(buf, data);
  if (type == sym_1("List"))
    return buf_inspect_list(buf, data);
  if (type == sym_1("Ptag"))
    return buf_inspect_ptag(buf, data);
  if (type == sym_1("Ptr"))
    return buf_inspect_ptr(buf, data);
  if (type == sym_1("PtrFree"))
    return buf_inspect_ptr_free(buf, data);
  if (type == sym_1("Quote"))
    return buf_inspect_quote(buf, data);
  if (type == sym_1("S8"))
    return buf_inspect_s8(buf, data);
  if (type == sym_1("S16"))
    return buf_inspect_s16(buf, data);
  if (type == sym_1("S32"))
    return buf_inspect_s32(buf, data);
  if (type == sym_1("S64"))
    return buf_inspect_s64(buf, data);
  if (type == sym_1("Str"))
    return buf_inspect_str(buf, data);
  if (type == sym_1("Struct"))
    return buf_inspect_struct(buf, data);
  if (type == sym_1("Sw"))
    return buf_inspect_sw(buf, data);
  if (type == sym_1("Sym"))
    return buf_inspect_sym(buf, data);
  if (type == sym_1("Tuple"))
    return buf_inspect_tuple(buf, data);
  if (type == sym_1("U8"))
    return buf_inspect_u8(buf, data);
  if (type == sym_1("U16"))
    return buf_inspect_u16(buf, data);
  if (type == sym_1("U32"))
    return buf_inspect_u32(buf, data);
  if (type == sym_1("U64"))
    return buf_inspect_u64(buf, data);
  if (type == sym_1("Uw"))
    return buf_inspect_uw(buf, data);
  if (type == sym_1("Var"))
    return buf_inspect_var(buf, data);
  if (type == sym_1("Void"))
    return buf_inspect_void(buf, data);
  /*
  if (sym_is_array_type(type)) {
  */
  st = struct_type_find(type);
  if (st) {
    s_struct s = {0};
    s.type = st;
    s.data = (void *) data;
    return buf_inspect_struct(buf, &s);
  }
  err_write_1("data_buf_inspect: unknown type: ");
  err_inspect_sym(&type);
  err_write_1("\n");
  assert(! "data_buf_inspect: unknown type");
  return -1;
}

sw data_buf_inspect_size (const s_sym *type, const void *data)
{
  s_struct_type *st;
  if (type == sym_1("Array"))
    return buf_inspect_array_size(data);
  if (type == sym_1("Bool"))
    return buf_inspect_bool_size(data);
  if (type == sym_1("Call"))
    return buf_inspect_call_size(data);
  if (type == sym_1("Cfn"))
    return buf_inspect_cfn_size(data);
  if (type == sym_1("Character"))
    return buf_inspect_character_size(data);
  if (type == sym_1("F32"))
    return buf_inspect_f32_size(data);
  if (type == sym_1("F64"))
    return buf_inspect_f64_size(data);
  if (type == sym_1("Fact"))
    return buf_inspect_fact_size(data);
  if (type == sym_1("Fn"))
    return buf_inspect_fn_size(data);
  if (type == sym_1("Ident"))
    return buf_inspect_ident_size(data);
  if (type == sym_1("Integer"))
    return buf_inspect_integer_size(data);
  if (type == sym_1("List"))
    return buf_inspect_list_size(data);
  if (type == sym_1("Ptag"))
    return buf_inspect_ptag_size(data);
  if (type == sym_1("Ptr"))
    return buf_inspect_ptr_size(data);
  if (type == sym_1("PtrFree"))
    return buf_inspect_ptr_free_size(data);
  if (type == sym_1("Quote"))
    return buf_inspect_quote_size(data);
  if (type == sym_1("S8"))
    return buf_inspect_s8_size(data);
  if (type == sym_1("S16"))
    return buf_inspect_s16_size(data);
  if (type == sym_1("S32"))
    return buf_inspect_s32_size(data);
  if (type == sym_1("S64"))
    return buf_inspect_s64_size(data);
  if (type == sym_1("Str"))
    return buf_inspect_str_size(data);
  if (type == sym_1("Struct"))
    return buf_inspect_struct_size(data);
  if (type == sym_1("Sw"))
    return buf_inspect_sw_size(data);
  if (type == sym_1("Sym"))
    return buf_inspect_sym_size(data);
  if (type == sym_1("Tuple"))
    return buf_inspect_tuple_size(data);
  if (type == sym_1("U8"))
    return buf_inspect_u8_size(data);
  if (type == sym_1("U16"))
    return buf_inspect_u16_size(data);
  if (type == sym_1("U32"))
    return buf_inspect_u32_size(data);
  if (type == sym_1("U64"))
    return buf_inspect_u64_size(data);
  if (type == sym_1("Uw"))
    return buf_inspect_uw_size(data);
  if (type == sym_1("Var"))
    return buf_inspect_var_size(data);
  if (type == sym_1("Void"))
    return buf_inspect_void_size(data);
  /*
  if (sym_is_array_type(type)) {
  */
  st = struct_type_find(type);
  if (st) {
    s_struct s = {0};
    s.type = st;
    s.data = (void *) data;
    return buf_inspect_struct_size(&s);
  }
  err_write_1("data_buf_inspect_size: unknown type: ");
  err_inspect_sym(&type);
  err_write_1("\n");
  assert(! "data_buf_inspect_size: unknown type");
  return -1;
}

bool data_clean (const s_sym *type, void *data)
{
  s_struct_type *st;
  assert(type);
  if (type == sym_1("Array")) {
    array_clean(data);
    return true;
  }
  if (type == sym_1("Bool")) {
    return true;
  }
  if (type == sym_1("Call")) {
    call_clean(data);
    return true;
  }
  if (type == sym_1("Cfn")) {
    cfn_clean(data);
    return true;
  }
  if (type == sym_1("Character")) {
    return true;
  }
  if (type == sym_1("F32")) {
    return true;
  }
  if (type == sym_1("F64")) {
    return true;
  }
  if (type == sym_1("Fact")) {
    return true;
  }
  if (type == sym_1("Fn")) {
    fn_clean(data);
    return true;
  }
  if (type == sym_1("Ident")) {
    return true;
  }
  if (type == sym_1("Integer")) {
    integer_clean(data);
    return true;
  }
  if (type == sym_1("List")) {
    list_f_clean(data);
    return true;
  }
  if (type == sym_1("Map")) {
    map_clean(data);
    return true;
  }
  if (type == sym_1("Ptag")) {
    return true;
  }
  if (type == sym_1("Ptr")) {
    return true;
  }
  if (type == sym_1("PtrFree")) {
    ptr_free_clean(data);
    return true;
  }
  if (type == sym_1("Quote")) {
    quote_clean(data);
    return true;
  }
  if (type == sym_1("S8")) {
    return true;
  }
  if (type == sym_1("S16")) {
    return true;
  }
  if (type == sym_1("S32")) {
    return true;
  }
  if (type == sym_1("S64")) {
    return true;
  }
  if (type == sym_1("Str")) {
    str_clean(data);
    return true;
  }
  if (type == sym_1("Struct")) {
    struct_clean(data);
    return true;
  }
  if (type == sym_1("Sw")) {
    return true;
  }
  if (type == sym_1("Sym")) {
    return true;
  }
  if (type == sym_1("Tuple")) {
    tuple_clean(data);
    return true;
  }
  if (type == sym_1("U8")) {
    return true;
  }
  if (type == sym_1("U16")) {
    return true;
  }
  if (type == sym_1("U32")) {
    return true;
  }
  if (type == sym_1("U64")) {
    return true;
  }
  if (type == sym_1("Uw")) {
    return true;
  }
  if (type == sym_1("Var")) {
    return true;
  }
  if (type == sym_1("Void")) {
    return true;
  }
  /*
  if (sym_is_array_type(type)) {
  */
  st = struct_type_find(type);
  if (st) {
    s_struct s = {0};
    s.type = st;
    s.data = data;
    struct_clean(&s);
    return true;
  }
  err_write_1("data_clean: unknown type: ");
  err_inspect_sym(&type);
  err_write_1("\n");
  assert(! "data_clean: unknown type");
  return false;
}

bool data_hash_update (const s_sym *type, t_hash *hash, const void *data)
{
  s_struct_type *st;
  if (type == sym_1("Array"))
    return hash_update_array(hash, data);
  if (type == sym_1("Bool"))
    return hash_update_bool(hash, data);
  if (type == sym_1("Call"))
    return hash_update_call(hash, data);
  if (type == sym_1("Cfn"))
    return hash_update_cfn(hash, data);
  if (type == sym_1("Character"))
    return hash_update_character(hash, data);
  if (type == sym_1("F32"))
    return hash_update_f32(hash, data);
  if (type == sym_1("F64"))
    return hash_update_f64(hash, data);
  if (type == sym_1("Fact"))
    return hash_update_fact(hash, data);
  if (type == sym_1("Fn"))
    return hash_update_fn(hash, data);
  if (type == sym_1("Ident"))
    return hash_update_ident(hash, data);
  if (type == sym_1("Integer"))
    return hash_update_integer(hash, data);
  if (type == sym_1("List"))
    return hash_update_list(hash, data);
  if (type == sym_1("Ptag"))
    return hash_update_ptag(hash, data);
  if (type == sym_1("Ptr"))
    return hash_update_ptr(hash, data);
  if (type == sym_1("PtrFree"))
    return hash_update_ptr_free(hash, data);
  if (type == sym_1("Quote"))
    return hash_update_quote(hash, data);
  if (type == sym_1("S8"))
    return hash_update_s8(hash, data);
  if (type == sym_1("S16"))
    return hash_update_s16(hash, data);
  if (type == sym_1("S32"))
    return hash_update_s32(hash, data);
  if (type == sym_1("S64"))
    return hash_update_s64(hash, data);
  if (type == sym_1("Str"))
    return hash_update_str(hash, data);
  if (type == sym_1("Struct"))
    return hash_update_struct(hash, data);
  if (type == sym_1("Sw"))
    return hash_update_sw(hash, data);
  if (type == sym_1("Sym"))
    return hash_update_sym(hash, data);
  if (type == sym_1("Tuple"))
    return hash_update_tuple(hash, data);
  if (type == sym_1("U8"))
    return hash_update_u8(hash, data);
  if (type == sym_1("U16"))
    return hash_update_u16(hash, data);
  if (type == sym_1("U32"))
    return hash_update_u32(hash, data);
  if (type == sym_1("U64"))
    return hash_update_u64(hash, data);
  if (type == sym_1("Uw"))
    return hash_update_uw(hash, data);
  if (type == sym_1("Var"))
    return hash_update_var(hash, data);
  if (type == sym_1("Void"))
    return hash_update_void(hash, data);
  /*
  if (sym_is_array_type(type)) {
  */
  st = struct_type_find(type);
  if (st) {
    s_struct s = {0};
    s.type = st;
    s.data = (void *) data;
    return hash_update_struct(hash, &s);
  }
  err_write_1("data_hash_update: unknown type: ");
  err_inspect_sym(&type);
  err_write_1("\n");
  assert(! "data_hash_update: unknown type");
  return false;
}

void * data_init_cast (const s_sym *type, void *data, const s_tag *tag)
{
  s_struct_type *st;
  if (type == sym_1("Array"))
    return array_init_cast(data, tag);
  if (type == sym_1("Bool"))
    return bool_init_cast(data, tag);
  if (type == sym_1("Call"))
    return call_init_cast(data, tag);
  if (type == sym_1("Cfn"))
    return cfn_init_cast(data, tag);
  if (type == sym_1("Character"))
    return character_init_cast(data, tag);
  if (type == sym_1("F32"))
    return f32_init_cast(data, tag);
  if (type == sym_1("F64"))
    return f64_init_cast(data, tag);
  if (type == sym_1("Fact"))
    return fact_init_cast(data, tag);
  if (type == sym_1("Fn"))
    return fn_init_cast(data, tag);
  if (type == sym_1("Ident"))
    return ident_init_cast(data, tag);
  if (type == sym_1("Integer"))
    return integer_init_cast(data, tag);
  if (type == sym_1("List"))
    return list_init_cast(data, tag);
  if (type == sym_1("Ptag"))
    return ptag_init_cast(data, tag);
  if (type == sym_1("Ptr"))
    return ptr_init_cast(data, tag);
  if (type == sym_1("PtrFree"))
    return ptr_free_init_cast(data, tag);
  if (type == sym_1("Quote"))
    return quote_init_cast(data, tag);
  if (type == sym_1("S8"))
    return s8_init_cast(data, tag);
  if (type == sym_1("S16"))
    return s16_init_cast(data, tag);
  if (type == sym_1("S32"))
    return s32_init_cast(data, tag);
  if (type == sym_1("S64"))
    return s64_init_cast(data, tag);
  if (type == sym_1("Str"))
    return str_init_cast(data, tag);
  if (type == sym_1("Struct"))
    return struct_init_cast(data, tag);
  if (type == sym_1("Sw"))
    return sw_init_cast(data, tag);
  if (type == sym_1("Sym"))
    return sym_init_cast(data, tag);
  if (type == sym_1("Tuple"))
    return tuple_init_cast(data, tag);
  if (type == sym_1("U8"))
    return u8_init_cast(data, tag);
  if (type == sym_1("U16"))
    return u16_init_cast(data, tag);
  if (type == sym_1("U32"))
    return u32_init_cast(data, tag);
  if (type == sym_1("U64"))
    return u64_init_cast(data, tag);
  if (type == sym_1("Uw"))
    return uw_init_cast(data, tag);
  if (type == sym_1("Var"))
    return data;
  if (type == sym_1("Void"))
    return data;
  /*
  if (sym_is_array_type(type)) {
  */
  st = struct_type_find(type);
  if (st) {
    s_struct s = {0};
    s.type = st;
    s.data = data;
    return struct_init_cast(&s, tag);
  }
  err_write_1("data_init_cast: unknown type: ");
  err_inspect_sym(&type);
  err_write_1("\n");
  assert(! "data_init_cast: unknown type");
  return NULL;
}

void * data_init_copy (const s_sym *type, void *data, const void *src)
{
  s_struct_type *st;
  if (type == sym_1("Array"))
    return array_init_copy(data, src);
  if (type == sym_1("Bool"))
    return bool_init_copy(data, src);
  if (type == sym_1("Call"))
    return call_init_copy(data, src);
  if (type == sym_1("Cfn"))
    return cfn_init_copy(data, src);
  if (type == sym_1("Character"))
    return character_init_copy(data, src);
  if (type == sym_1("F32"))
    return f32_init_copy(data, src);
  if (type == sym_1("F64"))
    return f64_init_copy(data, src);
  if (type == sym_1("Fact"))
    return fact_init_copy(data, src);
  if (type == sym_1("Fn"))
    return fn_init_copy(data, src);
  if (type == sym_1("Ident"))
    return ident_init_copy(data, src);
  if (type == sym_1("Integer"))
    return integer_init_copy(data, src);
  if (type == sym_1("List"))
    return list_init_copy(data, src);
  if (type == sym_1("Ptag"))
    return ptag_init_copy(data, src);
  if (type == sym_1("Ptr"))
    return ptr_init_copy(data, src);
  if (type == sym_1("PtrFree"))
    return ptr_free_init_copy(data, src);
  if (type == sym_1("Quote"))
    return quote_init_copy(data, src);
  if (type == sym_1("S8"))
    return s8_init_copy(data, src);
  if (type == sym_1("S16"))
    return s16_init_copy(data, src);
  if (type == sym_1("S32"))
    return s32_init_copy(data, src);
  if (type == sym_1("S64"))
    return s64_init_copy(data, src);
  if (type == sym_1("Str"))
    return str_init_copy(data, src);
  if (type == sym_1("Struct"))
    return struct_init_copy(data, src);
  if (type == sym_1("Sw"))
    return sw_init_copy(data, src);
  if (type == sym_1("Sym"))
    return sym_init_copy(data, src);
  if (type == sym_1("Tuple"))
    return tuple_init_copy(data, src);
  if (type == sym_1("U8"))
    return u8_init_copy(data, src);
  if (type == sym_1("U16"))
    return u16_init_copy(data, src);
  if (type == sym_1("U32"))
    return u32_init_copy(data, src);
  if (type == sym_1("U64"))
    return u64_init_copy(data, src);
  if (type == sym_1("Uw"))
    return uw_init_copy(data, src);
  if (type == sym_1("Var"))
    return data;
  if (type == sym_1("Void"))
    return data;
  st = struct_type_find(type);
  if (st) {
    s_struct s = {0};
    s_struct t = {0};
    t.type = s.type = st;
    s.data = data;
    t.data = (void *) src;
    return struct_init_copy(&s, &t);
  }
  err_write_1("data_init_copy: unknown type: ");
  err_inspect_sym(&type);
  err_write_1("\n");
  assert(! "data_init_copy: unknown type");
  return NULL;
}
