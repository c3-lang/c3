/* c3
 * Copyright 2022 Thomas de Grivel <thoxdg@gmail.com>
 */
#include <assert.h>
#include <err.h>
#include <stdlib.h>
#include "buf.h"
#include "buf_save.h"

s_buf * buf_save_clean (s_buf *buf, s_buf_save *save)
{
  assert(buf);
  assert(save);
  buf->save = save->save;
  return buf;
}

s_buf * buf_save_init (s_buf *buf, s_buf_save *save)
{
  assert(buf);
  assert(save);
  save->rpos = buf->rpos;
  if (! buf->save)
    buf_refill_compact(buf);
  save->save = buf->save;
  save->wpos = buf->wpos;
  buf->save = save;
  return buf;
}

uw buf_save_min_rpos (const s_buf_save *save)
{
  uw rpos;
  assert(save);
  rpos = save->rpos;
  while ((save = save->save))
    if (save->rpos < rpos)
      rpos = save->rpos;
  return rpos;
}

uw buf_save_min_wpos (const s_buf_save *save)
{
  uw wpos;
  assert(save);
  wpos = save->wpos;
  while ((save = save->save))
    if (save->wpos < wpos)
      wpos = save->wpos;
  return wpos;
}

s_buf * buf_save_restore (s_buf *buf, const s_buf_save *save)
{
  assert(buf);
  assert(save);
  buf->rpos = save->rpos;
  buf->wpos = save->wpos;
  return buf;
}
