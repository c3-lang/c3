/* c3
 * Copyright 2022 Thomas de Grivel <thoxdg@gmail.com>
 */
/**
 * @file buf_inspect.h
 * @brief Inspect data into UTF-8.
 *
 * Structure to inspect data into UTF-8.
 */
#ifndef BUF_INSPECT_H
#define BUF_INSPECT_H

#include "types.h"

extern const sw buf_inspect_str_byte_size;

sw buf_inspect_bool (s_buf *buf, e_bool b);
sw buf_inspect_character (s_buf *buf, character c);
sw buf_inspect_character_size (character c);
sw buf_inspect_f32 (s_buf *buf, f32 x);
sw buf_inspect_f64 (s_buf *buf, f64 x);
sw buf_inspect_ident (s_buf *buf, const s_ident *ident);
sw buf_inspect_ident_size (const s_ident *ident);
sw buf_inspect_list (s_buf *buf, const s_list *list);
sw buf_inspect_s8 (s_buf *buf, s8 i);
sw buf_inspect_s16 (s_buf *buf, s16 i);
sw buf_inspect_s32 (s_buf *buf, s32 i);
sw buf_inspect_s64 (s_buf *buf, s64 i);
sw buf_inspect_str (s_buf *buf, const s_str *str);
sw buf_inspect_str_size (const s_str *str);
sw buf_inspect_str_character (s_buf *buf, character c);
sw buf_inspect_str_character_size (character c);
sw buf_inspect_sym (s_buf *buf, const s_sym *sym);
sw buf_inspect_sym_size (const s_sym *sym);
sw buf_inspect_tag (s_buf *buf, const s_tag *tag);
sw buf_inspect_tuple (s_buf *buf, const s_tuple *tuple);
sw buf_inspect_u8 (s_buf *buf, u8 i);
sw buf_inspect_u16 (s_buf *buf, u16 i);
sw buf_inspect_u32 (s_buf *buf, u32 i);
sw buf_inspect_u64 (s_buf *buf, u64 i);

#endif /* BUF_INSPECT_H */
