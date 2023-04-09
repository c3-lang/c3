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
#include <assert.h>
#include <err.h>
#include <stdio.h>
#include <stdlib.h>
#include "c3.h"

const s_str g_c3_base_bin = {{NULL}, 2, {"01"}};
const s_str g_c3_base_oct = {{NULL}, 8, {"01234567"}};
const s_str g_c3_base_dec = {{NULL}, 10, {"0123456789"}};
const s_str g_c3_bases_hex[2] = {{{NULL}, 16, {"0123456789abcdef"}},
                                 {{NULL}, 16, {"0123456789ABCDEF"}}};

void c3_break ()
{
  assert(! "break");
  errx(1, "break");
  exit(1);
}

void c3_init (s_env *env)
{
  if (! env)
    env = &g_c3_env;
  if (! env_init(env))
    exit(1);
}

void c3_clean (s_env *env)
{
  if (! env)
    env = &g_c3_env;
  env_clean(env);
  sym_delete_all();
}
