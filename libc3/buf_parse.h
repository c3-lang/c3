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
/**
 * @file buf_parse.h
 * @brief Byte buffer.
 *
 * Structure to manipulate byte buffers.
 */
#ifndef BUF_PARSE_H
#define BUF_PARSE_H

#include "types.h"

/**
 * # buf_parse_
 *
 * Parse UTF-8 bytes from buf and fills dest if successful.
 *
 * Returns number of bytes read in buf, possibly zero in which case
 * dest is untouched.
 */

#define BUF_PARSE_S_PROTOTYPES(bits)                                   \
  sw buf_parse_s ## bits (s_buf *buf, s ## bits *dest);                \
  sw buf_parse_s ## bits ## _base (s_buf *buf, const s_str *base,      \
                                   bool negative, s ## bits *dest)

#define BUF_PARSE_U_PROTOTYPES(bits)                                   \
  sw buf_parse_u ## bits (s_buf *buf, u ## bits *dest);                \
  sw buf_parse_u ## bits ## _base (s_buf *buf, const s_str *base,      \
                                   u ## bits *dest)

sw buf_parse_array (s_buf *buf, s_array *dest);
sw buf_parse_array_type (s_buf *buf, e_tag_type *type);
sw buf_parse_bool (s_buf *buf, bool *dest);
sw buf_parse_call (s_buf *buf, s_call *dest);
sw buf_parse_call_args_paren (s_buf *buf, s_call *dest);
sw buf_parse_call_op (s_buf *buf, s_call *dest);
sw buf_parse_call_op_rec (s_buf *buf, s_call *dest, u8 min_precedence);
sw buf_parse_cfn (s_buf *buf, s_cfn *dest);
sw buf_parse_character (s_buf *buf, character *dest);
sw buf_parse_comments (s_buf *buf);
sw buf_parse_digit (s_buf *buf, const s_str *base, u8 *dest);
sw buf_parse_digit_bin (s_buf *buf, u8 *dest);
sw buf_parse_digit_hex (s_buf *buf, u8 *dest);
sw buf_parse_digit_oct (s_buf *buf, u8 *dest);
sw buf_parse_digit_dec (s_buf *buf, u8 *dest);
sw buf_parse_f32 (s_buf *buf, f32 *dest);
sw buf_parse_f64 (s_buf *buf, f64 *dest);
sw buf_parse_fact (s_buf *buf, s_fact_w *dest);
sw buf_parse_fn (s_buf *buf, s_fn **dest);
sw buf_parse_fn_clause (s_buf *buf, s_fn *dest);
sw buf_parse_fn_algo (s_buf *buf, s_list **dest);
sw buf_parse_fn_pattern (s_buf *buf, s_list **dest);
sw buf_parse_integer (s_buf *buf, s_integer *dest);
sw buf_parse_integer_unsigned_bin (s_buf *buf, s_integer *dest);
sw buf_parse_integer_unsigned_dec (s_buf *buf, s_integer *dest);
sw buf_parse_integer_unsigned_hex (s_buf *buf, s_integer *dest);
sw buf_parse_integer_unsigned_oct (s_buf *buf, s_integer *dest);
sw buf_parse_ident (s_buf *buf, s_ident *dest);
sw buf_parse_ident_peek (s_buf *buf, s_ident *dest);
sw buf_parse_integer (s_buf *buf, s_integer *dest);
sw buf_parse_list (s_buf *buf, s_list **dest);
sw buf_parse_module_name (s_buf *buf, const s_sym **dest);
sw buf_parse_new_tag (s_buf *buf, s_tag **dest);
sw buf_parse_ptag (s_buf *buf, p_tag *dest);
sw buf_parse_quote (s_buf *buf, s_quote *dest);
BUF_PARSE_S_PROTOTYPES(8);
BUF_PARSE_S_PROTOTYPES(16);
BUF_PARSE_S_PROTOTYPES(32);
BUF_PARSE_S_PROTOTYPES(64);
BUF_PARSE_S_PROTOTYPES(w);
sw buf_parse_str (s_buf *buf, s_str *dest);
sw buf_parse_str_character (s_buf *buf, character *dest);
sw buf_parse_str_character_unicode (s_buf *buf, character *dest);
sw buf_parse_str_u8 (s_buf *buf, u8 *dest);
sw buf_parse_sym (s_buf *buf, const s_sym **dest);
sw buf_parse_tag (s_buf *buf, s_tag *dest);
sw buf_parse_tag_array (s_buf *buf, s_tag *dest);
sw buf_parse_tag_bool (s_buf *buf, s_tag *dest);
sw buf_parse_tag_call (s_buf *buf, s_tag *dest);
sw buf_parse_tag_call_op (s_buf *buf, s_tag *dest);
sw buf_parse_tag_cfn (s_buf *buf, s_tag *dest);
sw buf_parse_tag_character (s_buf *buf, s_tag *dest);
sw buf_parse_tag_fn (s_buf *buf, s_tag *dest);
sw buf_parse_tag_ident (s_buf *buf, s_tag *dest);
sw buf_parse_tag_integer (s_buf *buf, s_tag *dest);
sw buf_parse_tag_list (s_buf *buf, s_tag *dest);
sw buf_parse_tag_primary (s_buf *buf, s_tag *dest);
sw buf_parse_tag_quote (s_buf *buf, s_tag *dest);
sw buf_parse_tag_str (s_buf *buf, s_tag *dest);
sw buf_parse_tag_str_character (s_buf *buf, s_tag *dest);
sw buf_parse_tag_str_u8 (s_buf *buf, s_tag *dest);
sw buf_parse_tag_sym (s_buf *buf, s_tag *dest);
sw buf_parse_tag_tuple (s_buf *buf, s_tag *dest);
sw buf_parse_tuple (s_buf *buf, s_tuple *dest);
BUF_PARSE_U_PROTOTYPES(8);
BUF_PARSE_U_PROTOTYPES(16);
BUF_PARSE_U_PROTOTYPES(32);
BUF_PARSE_U_PROTOTYPES(64);
sw buf_parse_u64_hex (s_buf *buf, u64 *dest);
BUF_PARSE_U_PROTOTYPES(w);
sw buf_parse_var (s_buf *buf, void *dest);
sw buf_parse_void (s_buf *buf, void *dest);

#endif /* BUF_PARSE_H */
