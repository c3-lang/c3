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

sw      buf_inspect_character (s_buf *buf, character c);
sw      buf_inspect_character_size (character c);
sw      buf_inspect_ident (s_buf *buf, const s_ident *ident);
sw      buf_inspect_ident_size (const s_ident *ident);
sw      buf_inspect_str (s_buf *buf, const s_str *str);
sw      buf_inspect_str_size (const s_str *str);
sw      buf_inspect_str_character (s_buf *buf, character c);
sw      buf_inspect_str_character_size (character c);
sw      buf_inspect_sym (s_buf *buf, const s_sym *sym);
sw      buf_inspect_sym_size (const s_sym *sym);

#endif /* BUF_INSPECT_H */
