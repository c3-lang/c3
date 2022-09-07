/* c3
 * Copyright 2022 Thomas de Grivel <thoxdg@gmail.com>
 */
/**
 * @file buf_readline.h
 * @brief Readline backend for s_buf.
 */
#ifndef BUF_READLINE_H
#define BUF_READLINE_H

void    buf_readline_close (s_buf *buf);
s_buf * buf_readline_open_r (s_buf *buf, const char *prompt);

#endif /* BUF_READLINE_H */
