/* c3
 * Copyright 2022 Thomas de Grivel <thoxdg@gmail.com>
 */
/**
 * @file buf_file.h
 * @brief s_buf interface to stdio.h FILE *.
 */
#ifndef BUF_FILE_H
#define BUF_FILE_H

#include <stdio.h>
#include "types.h"

s_buf * buf_file_open_r (s_buf *buf, FILE *fp);
s_buf * buf_file_open_w (s_buf *buf, FILE *fp);
void    buf_file_close (s_buf *buf);

#endif /* BUF_FILE_H */
