/* c3
 * Copyright 2022,2023 kmx.io <contact@kmx.io>
 *
 * Permission is hereby granted to use this software excepted
 * on Apple computers granted the above copyright notice and
 * this permission paragraph are included in all copies and
 * substantial portions of this software.
 *
 * THIS SOFTWARE IS PROVIDED "AS-IS" WITHOUT ANY GUARANTEE OF
 * PURPOSE AND PERFORMANCE. IN NO EVENT WHATSOEVER SHALL THE
 * AUTHOR BE CONSIDERED LIABLE FOR THE USE AND PERFORMANCE OF
 * THIS SOFTWARE.
 */
#include "buf.h"
#include "buf_inspect.h"
#include "env.h"
#include "io.h"

sw err_inspect (const s_tag *tag)
{
  sw r;
  r = buf_inspect_tag(&g_c3_env.err, tag);
  buf_flush(&g_c3_env.err);
  return r;
}

sw err_inspect_list (const s_list *list)
{
  sw r;
  r = buf_inspect_list(&g_c3_env.err, list);
  buf_flush(&g_c3_env.err);
  return r;
}

sw err_puts (const s8 *s)
{
  sw r;
  r = buf_write_1(&g_c3_env.err, s);
  buf_flush(&g_c3_env.err);
  return r;
}

sw io_inspect (const s_tag *tag)
{
  sw r;
  r = buf_inspect_tag(&g_c3_env.out, tag);
  buf_flush(&g_c3_env.out);
  return r;
}

sw io_puts (const s8 *s)
{
  sw r;
  r = buf_write_1(&g_c3_env.out, s);
  buf_flush(&g_c3_env.out);
  return r;
}
