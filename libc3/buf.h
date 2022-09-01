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

#include <assert.h>
#include <err.h>
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

extern const sw buf_u8_to_hex_size;
extern const sw buf_inspect_str_byte_size;

/* Stack constructors, call buf_clean after use. */
s_buf * buf_init (s_buf *buf, bool free, uw size, s8 *p);
s_buf * buf_init_alloc (s_buf *buf, uw size);

/* Read only stack contructors, do not write into.
   Call buf_clean after use. */
s_buf * buf_init_1 (s_buf *buf, bool free, s8 *p);

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
sw buf_peek_character (s_buf *buf, character *p);
sw buf_peek_f8 (s_buf *buf, f8 *p);
sw buf_peek_f16 (s_buf *buf, f16 *p);
sw buf_peek_f32 (s_buf *buf, f32 *p);
sw buf_peek_f64 (s_buf *buf, f64 *p);
sw buf_peek_s8 (s_buf *buf, s8 *p);
sw buf_peek_s16 (s_buf *buf, s16 *p);
sw buf_peek_s32 (s_buf *buf, s32 *p);
sw buf_peek_s64 (s_buf *buf, s64 *p);
sw buf_peek_u8 (s_buf *buf, u8 *p);
sw buf_peek_u16 (s_buf *buf, u16 *p);
sw buf_peek_u32 (s_buf *buf, u32 *p);
sw buf_peek_u64 (s_buf *buf, u64 *p);
sw buf_read_character (s_buf *buf, character *p);
sw buf_read_f32 (s_buf *buf, f32 *p);
sw buf_read_f64 (s_buf *buf, f64 *p);
sw buf_read_s8 (s_buf *buf, s8 *p);
sw buf_read_s16 (s_buf *buf, s16 *p);
sw buf_read_s32 (s_buf *buf, s32 *p);
sw buf_read_s64 (s_buf *buf, s64 *p);
sw buf_read_u8 (s_buf *buf, u8 *p);
sw buf_read_u16 (s_buf *buf, u16 *p);
sw buf_read_u32 (s_buf *buf, u32 *p);
sw buf_read_u64 (s_buf *buf, u64 *p);
sw buf_refill (s_buf *buf);
sw buf_str_to_hex (s_buf *buf, const s_str *src);
sw buf_str_to_hex_size (const s_str *src);
sw buf_u8_to_hex (s_buf *buf, u8 x);
sw buf_vf (s_buf *buf, const char *fmt, va_list ap);
sw buf_write_1 (s_buf *buf, s8 *p);
sw buf_write_character (s_buf *buf, character c);
sw buf_write_str (s_buf *buf, const s_str *src);
sw buf_write_u8 (s_buf *buf, u8 i);
sw buf_write_u16 (s_buf *buf, u16 i);
sw buf_write_u32 (s_buf *buf, u32 i);
sw buf_write_u64 (s_buf *buf, u64 i);
sw buf_xfer (s_buf *buf, s_buf *src, uw size);

#endif /* BUF_H */
