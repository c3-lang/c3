/* c3
 * Copyright 2022 Thomas de Grivel <thoxdg@gmail.com>
 */
#include <assert.h>
#include <err.h>
#include <stdio.h>
#include <stdlib.h>
#include <readline/readline.h>
#include <readline/history.h>
#include "../libc3/c3.h"

typedef struct buf_readline {
  s_buf buf;
  const char *prompt;
} s_buf_readline;

sw buf_readline_refill (s_buf *buf);

void buf_readline_close (s_buf *buf)
{
  assert(buf);
  buf->refill = NULL;
  free(buf->user_ptr);
  buf->user_ptr = NULL;
}

s_buf * buf_readline_open_r (s_buf *buf, const char *prompt)
{
  s_buf_readline *buf_readline;
  assert(buf);
  buf_readline = malloc(sizeof(s_buf_readline));
  if (! buf_readline)
    errx(1, "buf_readline_open_r: out of memory");
  buf_init_1(&buf_readline->buf, "");
  buf_readline->prompt = prompt;
  buf->refill = buf_readline_refill;
  buf->user_ptr = buf_readline;
  return buf;
}

sw buf_readline_refill (s_buf *buf)
{
  s_buf_readline *buf_readline;
  uw buf_readline_len;
  uw r;
  uw size;
  assert(buf);
  assert(buf->user_ptr);
  if (buf->rpos > buf->wpos ||
      buf->wpos > buf->size)
    return -1;
  size = buf->size - buf->wpos;
  if (size == 0)
    return 0;
  buf_readline = buf->user_ptr;
  buf_readline_len = buf_readline->buf.wpos - buf_readline->buf.rpos;
  if (buf_readline_len == 0) {
    free(buf_readline->buf.ptr.p);
    if (! (buf_readline->buf.ptr.p = readline(buf_readline->prompt)))
      return -1;
  }
  if (buf_readline_len < size)
    size = buf_readline_len;
  if ((r = buf_xfer(buf, &buf_readline->buf, size)) != size)
    return -1;
  return size;
}
