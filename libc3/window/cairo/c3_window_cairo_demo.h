/* c3
 * Copyright 2022,2023 kmx.io <contact@kmx.io>
 *
 * Permission is hereby granted to use this software granted the above
 * copyright notice and this permission paragraph are included in all
 * copies and substantial portions of this software.
 *
 * THIS SOFTWARE IS PROVIDED "AS-IS" WITHOUT ANY GUARANTEE OF
 * PURPOSE AND PERFORMANCE. IN NO EVENT WHATSOEVER SHALL THE
 * AUTHOR BE CONSIDERED LIABLE FOR THE USE AND PERFORMANCE OF
 * THIS SOFTWARE.
 */
#ifndef C3_WINDOW_CAIRO_DEMO_H
#define C3_WINDOW_CAIRO_DEMO_H

#include <libc3/types.h>
#include "types.h"

bool c3_window_cairo_demo_render (s_window_cairo *window, cairo_t *cr);
bool c3_window_cairo_demo_resize (s_window_cairo *window, uw w, uw h);

#endif /* C3_WINDOW_CAIRO_DEMO_H */
