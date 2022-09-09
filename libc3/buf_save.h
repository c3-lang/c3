/* c3
 * Copyright 2022 Thomas de Grivel <thoxdg@gmail.com>
 */
#ifndef BUF_SAVE_H
#define BUF_SAVE_H

#include "types.h"

s_buf * buf_save_clean (s_buf *buf, s_buf_save *save);
s_buf * buf_save_init (s_buf *buf, s_buf_save *save);
uw      buf_save_min_rpos (const s_buf_save *save);
uw      buf_save_min_wpos (const s_buf_save *save);
s_buf * buf_save_restore (s_buf *buf, const s_buf_save *save);

#endif /* BUF_SAVE */
