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
#include <math.h>
#include <libc3/c3.h>
#include "gl.h"
#include "gl_cylinder.h"

s_gl_cylinder * gl_cylinder_init (s_gl_cylinder *cylinder,
                                  uw segments_u, uw segments_v)
{
  f64 angle;
  uw i;
  uw j;
  s_gl_3d *p;
  assert(cylinder);
  assert(segments_u);
  assert(segments_v);
  cylinder->segments_u = segments_u;
  cylinder->segments_v = segments_v;
  p = calloc(segments_u * segments_v + 2,
             sizeof(s_gl_3d));
  if (! p) {
    err_write_1("gl_cylinder_init(");
    err_inspect_uw(&segments_u);
    err_write_1(", ");
    err_inspect_uw(&segments_v);
    err_puts("): failed to allocate memory");
    return NULL;
  }
  cylinder->vertex = p;
  i = 0;
  while (i < segments_v) {
    j = 0;
    while (j < segments_u) {
      angle = (f64) j / segments_u * M_PI * 2.0;
      p->x = cos(angle);
      p->y = sin(angle);
      p->z = i / segments_v;
      p++;
      j++;
    }
    i++;
  }
  return cylinder;
}

void gl_cylinder_render (const s_gl_cylinder *cylinder)
{
  uw i;
  uw j;
  s_gl_3d *p;
  assert(cylinder);
  glBegin(GL_POINTS);
  p = cylinder->vertex;
  i = 0;
  while (i < cylinder->segments_v) {
    j = 0;
    while (j < cylinder->segments_u) {
      gl_vertex_3d(p);
      p++;
      j++;
    }
    i++;
  }
  gl_vertex_3d(p);
  p++;
  gl_vertex_3d(p);
  glEnd();
}
