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

#include "types.h"

/* Stack allocation compatible functions */
void buf_init (s_buf *buf, bool free, uw bytes, const s8 *p);
void buf_init_str (s_buf *buf, s_str *str);
void buf_clean (s_buf *buf);

/* Constructors, call buf_delete after use. */
s_buf * buf_new (bool free, uw bytes, const s8 *p);
s_buf * buf_new_alloc (uw bytes);
s_buf * buf_new_str (s_str *str);

/* Destructor */
void buf_delete (s_buf *buf);

/* Modifiers */
sw buf_flush (s_buf *buf);
sw buf_read (s_buf *buf, u8 *p);
sw buf_read_str (s_buf *buf, s_str *str);
sw buf_refill (s_buf *buf);
sw buf_write (s_buf *buf, u8 v);
sw buf_write_str (s_buf *buf, const s_str *src);

#endif /* BUF_H */
