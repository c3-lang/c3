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
sw buf_peek (s_buf *buf, u8 *p);
sw buf_read (s_buf *buf, u8 *p);
sw buf_read_character (s_buf *buf, character *p);
sw buf_refill (s_buf *buf);
sw buf_vf (s_buf *buf, const char *fmt, va_list ap);
sw buf_write (s_buf *buf, u8 v);
sw buf_write_str (s_buf *buf, const s_str *src);

/* Str observers, call str_delete after use. */
s_str * buf_to_str (const s_buf *src);

#endif /* BUF_H */
