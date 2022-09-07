/* c3
 * Copyright 2022 Thomas de Grivel <thoxdg@gmail.com>
 */
#include <unistd.h>
#include "../libc3/c3.h"
#include "buf_readline.h"

#define BUFSZ 1024

int usage (char *argv0);

sw buf_xfer_spaces (s_buf *in, s_buf *out)
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
  }
  return size;
}

int main (int argc, char **argv)
{
  u8 byte;
  s_buf in;
  s_buf out;
  sw r;
  s_tag tag;
  libc3_init();
  if (argc != 1)
    return usage(argv[0]);
  BUF_INIT_ALLOCA(&in, BUFSZ);
  BUF_INIT_ALLOCA(&out, BUFSZ);
  if (false && isatty(STDIN_FILENO))
    buf_readline_open_r(&in, "ic3> ");
  else
    buf_file_open_r(&in, stdin);
  buf_file_open_w(&out, stdout);
  while ((r = buf_peek_u8(&in, &byte)) > 0) {
    if (buf_xfer_spaces(&in, &out) < 0)
      break;
    if ((r = buf_parse_tag(&in, &tag)) > 0) {
      if (buf_inspect_tag(&out, &tag) < 0 ||
          buf_write_1(&out, "\n") < 0 ||
          buf_flush(&out) < 0) {
	tag_clean(&tag);
        break;
      }
      tag_clean(&tag);
    }
    if (r < 0)
      break;
  }
  if (isatty(STDIN_FILENO))
    buf_readline_close(&in);
  else
    buf_file_close(&in);
  buf_file_close(&out);
  libc3_shutdown();
  return 0;
}

int usage (char *argv0)
{
  printf("Usage: %s\n", argv0);
  return 1;
}
