/* c3
 * Copyright 2022 Thomas de Grivel <thoxdg@gmail.com>
 */
/**
 * @file buf.h
 * @brief Byte buffer.
 *
 * Structure to manipulate byte buffers.
 */
#ifndef BUF_H
#define BUF_H

#include <stdarg.h>
#include "types.h"

#define BUF_INIT_ALLOCA(buf, size)              \
  do {                                          \
    s8 *p;                                      \
    assert(buf);                                \
    assert(size);                               \
    p = alloca(size);                           \
    if (!p)                                     \
      err(1, "out of memory");                  \
    buf_init((buf), false, (size), p);          \
  } while (0)

/* Stack constructors, call buf_clean after use. */
void buf_init (s_buf *buf, bool free, uw size, s8 *p);
void buf_init_alloc (s_buf *buf, uw size);

/* Constructors, call buf_delete after use. */
s_buf * buf_new (bool free, uw size, s8 *p);
s_buf * buf_new_alloc (uw bytes);
s_buf * buf_new_str (s_str *str);

/* Destructors */
void buf_clean (s_buf *buf);
void buf_delete (s_buf *buf);

/* Modifiers */
sw buf_f (s_buf *buf, const char *fmt, ...);
sw buf_flush (s_buf *buf);
sw buf_inspect_character (s_buf *buf, character c);
sw buf_inspect_character_size (character c);
sw buf_inspect_str (s_buf *buf, const s_str *str);
sw buf_inspect_str_size (const s_str *str);
sw buf_inspect_str_character (s_buf *buf, character c);
sw buf_inspect_str_character_size (character c);
sw buf_inspect_sym (s_buf *buf, const s_sym *sym);
sw buf_inspect_sym_size (const s_sym *sym);
sw buf_peek (s_buf *buf, u8 *p);
sw buf_peek_character (s_buf *buf, character *p);
sw buf_read (s_buf *buf, u8 *p);
sw buf_read_character (s_buf *buf, character *p);
sw buf_refill (s_buf *buf);
sw buf_str_to_hex (s_buf *buf, const s_str *src);
sw buf_str_to_hex_size (const s_str *src);
sw buf_u8_to_hex (s_buf *buf, u8 x);
const sw buf_u8_to_hex_size;
sw buf_vf (s_buf *buf, const char *fmt, va_list ap);
sw buf_write (s_buf *buf, u8 v);
sw buf_write_1 (s_buf *buf, s8 *p);
sw buf_write_character (s_buf *buf, character c);
sw buf_write_str (s_buf *buf, const s_str *src);
sw buf_xfer (s_buf *buf, s_buf *src);

#endif /* BUF_H */
