/* c3
 * Copyright 2022 Thomas de Grivel <thoxdg@gmail.com>
 */
#include <assert.h>
#include <stdlib.h>
#include <string.h>
#include "buf.h"
#include "character.h"
#include "str.h"
#include "sym.h"

sw buf_peek_str (s_buf *buf, const s_str *src)
{
  assert(buf);
  assert(src);
  if (buf->rpos + src->size > buf->size ||
      memcmp(buf->ptr.p, src->ptr.p, src->size))
    return 0;
  return src->size;
}

sw buf_read_str (s_buf *buf, const s_str *src)
{
  sw r;
  r = buf_peek_str(buf, src);
  if (r > 0)
    buf->rpos += r;
  return r;
}

sw buf_read_1 (s_buf *buf, const s8 *p)
{
  s_str stra;
  assert(buf);
  str_init_1(&stra, NULL, p);
  return buf_read_str(buf, &stra);
}

sw buf_parse_bool (s_buf *buf, bool *p)
{
  sw r;
  assert(buf);
  if ((r = buf_read_1(buf, "true")) > 0)
    *p = true;
  if (r < 0)
    return r;
  if ((r = buf_read_1(buf, "false")) > 0)
    *p = false;
  return r;
}
