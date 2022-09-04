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

sw      buf_parse_character (s_buf *buf, character *p);
sw      buf_parse_digit_hex (s_buf *buf, u8 *p);
sw      buf_parse_ident (s_buf *buf, s_ident *p);
sw      buf_parse_str (s_buf *buf, s_str *p);
sw      buf_parse_sym (s_buf *buf, const s_sym *p);

#endif /* BUF_PARSE_H */
