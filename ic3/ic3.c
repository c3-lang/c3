/* c3
 * Copyright 2022 Thomas de Grivel <thoxdg@gmail.com>
 */
#include "../libc3/c3.h"

#define BUFSZ 1024

int usage (char *argv0);

sw buf_xfer_spaces (s_buf *in, s_buf *out)
{
  u8 b;
  character c;
  sw r;
  sw r1;
  uw size = 0;
  s_buf in_save;
  assert(in);
  assert(out);
  while ((r = buf_peek_character_utf8(in, &c)) > 0 &&
         character_is_space(c)) {
    while (r--) {
      buf_save(in, &in_save);
      if ((r1 = buf_read_u8(in, &b)) != 1)
        break;
      if ((r1 = buf_write_u8(out, b)) != 1) {
        buf_restore(in, &in_save);
        break;
      }
    }
    if (r1 != 1) {
      if (size)
        return size;
      return r1;
    }
  }
  if (size)
    return size;
  return r;
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
  buf_file_open_r(&in, stdin);
  buf_file_open_w(&out, stdout);
  while ((r = buf_peek_u8(&in, &byte)) > 0) {
    buf_xfer_spaces(&in, &out);
    if ((r = buf_parse_tag(&in, &tag)) > 0)
      buf_inspect_tag(&out, &tag);
    
  }
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
