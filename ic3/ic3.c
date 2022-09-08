/* c3
 * Copyright 2022 Thomas de Grivel <thoxdg@gmail.com>
 */
#include "../libc3/c3.h"
#include "buf_readline.h"

#define BUFSZ 1024

int usage (char *argv0);

sw buf_ignore_character (s_buf *buf)
{
  u8 b;
  character c = 0;
  sw csize;
  sw r;
  if ((r = buf_peek_character_utf8(buf, &c)) > 0)
    csize = r;
  else if ((r = buf_peek_u8(buf, &b)) > 0)
    csize = 1;
  else
    return r;
  if ((r = buf_ignore(buf, csize)) < 0)
    return r;
  return csize;
}

sw buf_xfer_spaces (s_buf *out, s_buf *in)
{
  character c;
  sw csize;
  sw r;
  sw size = 0;
  assert(in);
  assert(out);
  while ((r = buf_peek_character_utf8(in, &c)) > 0 &&
         c >= 0 &&
         c < UCD_MAX &&
         g_ucd[c].flags & (UCD_OTHER_CONTROL | UCD_SEPARATOR_SPACE)) {
    csize = r;
    if ((r = buf_xfer(out, in, csize)) != csize)
      return -1;
    size += csize;
    if ((r = buf_flush(out)) < 0)
      return -1;
  }
  if (r < 0)
    return r;
  return size;
}

int main (int argc, char **argv)
{
  s_buf in;
  s_buf out;
  sw r;
  s_tag tag;
  libc3_init();
  if (argc < 1)
    return usage(argv[0]);
  BUF_INIT_ALLOCA(&in, BUFSZ);
  BUF_INIT_ALLOCA(&out, BUFSZ);
  buf_readline_open_r(&in, "ic3> ");
  buf_file_open_w(&out, stdout);
  while ((r = buf_xfer_spaces(&out, &in)) >= 0) {
    if ((r = buf_parse_tag(&in, &tag)) > 0) {
      if (buf_inspect_tag(&out, &tag) < 0) {
	tag_clean(&tag);
        break;
      }
      tag_clean(&tag);
      if ((r = buf_flush(&out)) < 0)
        break;
    }
    if (r < 0 ||
        (r == 0 &&
         (r = buf_ignore_character(&in)) <= 0))
      break;
  }
  buf_readline_close(&in);
  buf_file_close(&out);
  libc3_shutdown();
  return 0;
}

int usage (char *argv0)
{
  printf("Usage: %s\n", argv0);
  return 1;
}
