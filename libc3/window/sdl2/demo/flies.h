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
#ifndef FLIES_H
#define FLIES_H

#include "../types.h"
#include "window_sdl2_demo.h"

extern s_sdl2_sprite g_dead_fly_sprite;
extern s_sdl2_sprite g_fly_sprite;

bool flies_load (s_sequence *seq, s_window_sdl2 *window);
bool flies_render (s_sequence *seq, s_window_sdl2 *window,
                   void *context);

#endif /* FLIES_H */
