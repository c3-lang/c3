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
#include <string.h>
#include "buf.h"
#include "character.h"
#include "compare.h"
#include "env.h"
#include "facts.h"
#include "facts_with.h"
#include "facts_with_cursor.h"
#include "file.h"
#include "module.h"
#include "tag.h"

bool module_ensure_loaded (const s_sym *name, s_facts *facts)
{
  s_facts_with_cursor cursor;
  s_tag tag_module_name;
  s_tag tag_is_a;
  s_tag tag_module;
  tag_init_sym(&tag_module_name, name);
  tag_init_1(  &tag_is_a,     ":is_a");
  tag_init_1(  &tag_module,   ":module");
  facts_with(facts, &cursor, (t_facts_spec) {
      &tag_module_name,
      &tag_is_a, &tag_module,     /* module exists */
      NULL, NULL });
  if (! facts_with_cursor_next(&cursor)) {
    if (! module_load(name, facts)) {
      warnx("module not found: %s",
            name->str.ptr.ps8);
      facts_with_cursor_clean(&cursor);
      return false;
    }
  }
  facts_with_cursor_clean(&cursor);
  return module_maybe_reload(name, facts);
}

bool module_load (const s_sym *name, s_facts *facts)
{
  return env_module_load(&g_c3_env, name, facts);
}

s_tag * module_load_time (const s_sym *name, s_facts *facts,
                          s_tag *dest)
{
  s_facts_with_cursor cursor;
  s_tag tag_name;
  s_tag tag_load_time;
  s_tag tag_time;
  tag_init_sym(&tag_name, name);
  tag_init_1(  &tag_load_time, ":load_time");
  tag_init_var(&tag_time);
  facts_with(facts, &cursor, (t_facts_spec) {
      &tag_name, &tag_load_time, &tag_time, NULL, NULL });
  if (! facts_with_cursor_next(&cursor)) {
    facts_with_cursor_clean(&cursor);
    return NULL;
  }
  facts_with_cursor_clean(&cursor);
  *dest = tag_time;
  return dest;
}

bool module_maybe_reload (const s_sym *name, s_facts *facts)
{
  return env_module_maybe_reload(&g_c3_env, name, facts);
}

s_str * module_name_path (const s_str *prefix, const s_sym *name,
                          s_str *dest)
{
  character b = -1;
  character c;
  s_buf in;
  s_buf out;
  sw out_size;
  sw r;
  assert(dest);
  assert(name);
  buf_init_str(&in, &name->str);
  out_size = module_name_path_size(prefix, name);
  buf_init_alloc(&out, out_size);
  if ((r = buf_write_str(&out, prefix)) < 0)
    goto error;
  if ((r = buf_write_s8(&out, '/')) < 0)
    goto error;
  while ((r = buf_read_character_utf8(&in, &c)) > 0) {
    if (c == '.')
      c = '/';
    else if (character_is_uppercase(c)) {
      if (character_is_lowercase(b)) {
        if ((r = buf_write_s8(&out, '_')) < 0)
          goto error;
      }
      c = character_to_lower(c);
    }
    if ((r = buf_write_character_utf8(&out, c)) < 0)
      goto error;
    b = c;
  }
  if ((r = buf_write_1(&out, FACTS_EXT)) < 0)
    goto error;
  buf_clean(&in);
  return buf_to_str(&out, dest);
 error:
  buf_clean(&in);
  buf_clean(&out);
  return NULL;
}

sw module_name_path_size (const s_str *prefix, const s_sym *name)
{
  character b = 0;
  character c;
  s_buf in;
  sw r;
  sw result = 0;
  assert(prefix);
  assert(name);
  buf_init_str(&in, &name->str);
  result += prefix->size;
  result++;
  while ((r = buf_read_character_utf8(&in, &c)) > 0) {
    if (character_is_uppercase(c))
      if (character_is_lowercase(b))
        result += 1;
    result += character_utf8_size(c);
    b = c;
  }
  result += strlen(FACTS_EXT);
  buf_clean(&in);
  return result;
}
