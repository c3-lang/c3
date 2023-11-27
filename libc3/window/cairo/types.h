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
/** @file types.h
 *  @brief Module C3.Window.Cairo
 *
 *  Struct for all GUI window Cairo graphics operations.
 */
#ifndef LIBC3_WINDOW_CAIRO_TYPES_H
#define LIBC3_WINDOW_CAIRO_TYPES_H

#include <cairo.h>
#include <cairo-ft.h>
#include <ft2build.h>
#include FT_FREETYPE_H
#include <libc3/types.h>
#include "../types.h"

typedef struct cairo_font   s_cairo_font;
typedef struct cairo_sprite s_cairo_sprite;
typedef struct rgb          s_rgb;
typedef struct rgba         s_rgba;
typedef struct window_cairo s_window_cairo;

/* return false to break event loop */
typedef bool (*f_window_cairo_button) (s_window_cairo *window,
                                       u8 button, sw x, sw y);

/* return false to break event loop */
typedef bool (*f_window_cairo_key) (s_window_cairo *window, uw keysym);

/* return false to break event loop */
typedef bool (*f_window_cairo_load) (s_window_cairo *window);

/* return false to break event loop */
typedef bool (*f_window_cairo_motion) (s_window_cairo *window, sw x,
                                       sw y);

/* return false to break event loop */
typedef bool (*f_window_cairo_render) (s_window_cairo *window,
                                       cairo_t *cr);

/* return false to break event loop */
typedef bool (*f_window_cairo_resize) (s_window_cairo *window,
                                       uw w, uw h);

typedef bool (*f_window_cairo_sequence_load) (s_sequence *seq,
                                              s_window_cairo *window);

typedef bool (*f_window_cairo_sequence_render) (s_sequence *seq,
                                                s_window_cairo *window,
                                                cairo_t *cr);

struct cairo_font {
  cairo_font_face_t *cairo_font_face;
  FT_Face ft_face;
  s_str path;
};

struct cairo_sprite {
  uw surface_w;
  uw surface_h;
  uw dim_x;
  uw dim_y;
  uw frame_count;
  uw w;
  uw h;
  cairo_surface_t *surface;
};

struct rgb {
  double r;
  double g;
  double b;
};

struct rgba {
  double r;
  double g;
  double b;
  double a;
};

/* Subtype of s_window. See libc3/window/types.h */
struct window_cairo {
  sw                    x;
  sw                    y;
  uw                    w;
  uw                    h;
  f_window_cairo_button button;
  f_window_cairo_key    key;
  f_window_cairo_load   load;
  f_window_cairo_motion motion;
  f_window_cairo_render render;
  cairo_t              *cr;
  f_window_cairo_resize resize;
  s_sequence           *sequence;
  uw                    sequence_count;
  uw                    sequence_pos;
  const s8             *title;
};

#endif /* LIBC3_WINDOW_CAIRO_TYPES_H */
