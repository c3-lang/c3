/* c3
 * Copyright 2022 Thomas de Grivel <thoxdg@gmail.com>
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

sw buf_parse_bool (s_buf *buf, bool *dest);
sw buf_parse_character (s_buf *buf, character *dest);
sw buf_parse_digit_hex (s_buf *buf, u8 *dest);
sw buf_parse_digit_oct (s_buf *buf, u8 *dest);
sw buf_parse_digit_dec (s_buf *buf, u8 *dest);
sw buf_parse_ident (s_buf *buf, s_ident *dest);
sw buf_parse_str (s_buf *buf, s_str *dest);
sw buf_parse_str_character (s_buf *buf, character *dest);
sw buf_parse_str_u8 (s_buf *buf, u8 *dest);
sw buf_parse_sym (s_buf *buf, const s_sym **dest);

#endif /* BUF_PARSE_H */
