/* c3
 * Copyright 2022 Thomas de Grivel <thoxdg@gmail.com>
 */
#include <assert.h>
#include <err.h>
#include <stdio.h>
#include <stdlib.h>
#include "buf.h"
#include "buf_file.h"

typedef struct buf_file {
  FILE *fp;
} s_buf_file;

sw buf_file_open_r_refill (s_buf *buf);
sw buf_file_open_w_flush (s_buf *buf);

void buf_file_close (s_buf *buf)
{
  assert(buf);
  buf->flush = NULL;
  buf->refill = NULL;
  free(buf->user_ptr);
  buf->user_ptr = NULL;
}

s_buf * buf_file_open_r (s_buf *buf, FILE *fp)
{
  s_buf_file *buf_file;
  assert(buf);
  assert(fp);
  buf_file = malloc(sizeof(s_buf_file));
  if (! buf_file)
    errx(1, "buf_file_open_r: out of memory");
  buf_file->fp = fp;
  buf->refill = buf_file_open_r_refill;
  buf->user_ptr = buf_file;
  return buf;
}

sw buf_file_open_r_refill (s_buf *buf)
{
  uw r;
  uw size;
  assert(buf);
  assert(buf->user_ptr);
  if (buf->rpos > buf->wpos ||
      buf->wpos > buf->size)
    return -1;
  size = buf->size - buf->wpos;
  r = fread(buf->ptr.ps8 + buf->wpos, 1, size,
            ((s_buf_file *) (buf->user_ptr))->fp);
  if (buf->wpos + r > buf->size) {
    assert(! "buffer overflow");
    return -1;
  }
  buf->wpos += r;
  return r;
}

s_buf * buf_file_open_w (s_buf *buf, FILE *fp)
{
  s_buf_file *buf_file;
  assert(buf);
  assert(fp);
  buf_file = malloc(sizeof(s_buf_file));
  if (! buf_file)
    errx(1, "buf_file_open_w: out of memory");
  buf_file->fp = fp;
  buf->flush = buf_file_open_w_flush;
  buf->user_ptr = buf_file;
  return buf;
}

sw buf_file_open_w_flush (s_buf *buf)
{
  /* TODO */
  errx(1, "not implemented");
  (void) buf;
  return 0;
}
