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
#include <assert.h>
#include <stdlib.h>
#include "facts_cursor.h"
#include "facts_spec.h"
#include "facts_with.h"
#include "tag.h"

s_facts_with_cursor * facts_with (const s_facts *facts,
                                  s_facts_with_cursor *cursor,
                                  p_facts_spec spec)
{
  uw facts_count;
  assert(facts);
  assert(cursor);
  assert(spec);
  facts_count = facts_spec_count_facts(spec);
  cursor->facts = facts;
  cursor->facts_count = facts_count;
  if (facts_count > 0) {
    cursor->levels = calloc(facts_count,
                            sizeof(s_facts_with_cursor_level));
    cursor->spec = facts_spec_new_expand(spec);
    /* facts_spec_sort(cursor->spec); */
  }
  else {
    cursor->levels = NULL;
    cursor->spec = NULL;
  }
  cursor->level = 0;
  return cursor;
}

s_facts_cursor * facts_with_0 (const s_facts *facts,
                               s_facts_cursor *cursor,
                               s_tag *var_subject,
                               s_tag *var_predicate,
                               s_tag *var_object)
{
  assert(facts);
  assert(cursor);
  facts_cursor_init(cursor, facts->index_spo, NULL, NULL);
  cursor->var_subject = var_subject;
  cursor->var_predicate = var_predicate;
  cursor->var_object = var_object;
  return cursor;
}

s_facts_cursor * facts_with_1_2 (const s_facts *facts,
                                 s_facts_cursor *cursor,
                                 const s_tag *subject,
                                 const s_tag *predicate,
                                 const s_tag *object,
                                 s_tag *var_subject,
                                 s_tag *var_predicate,
                                 s_tag *var_object)
{
  s_fact start;
  s_fact end;
  s_skiplist__fact *tree;
  assert(facts);
  assert(cursor);
  assert(subject);
  assert(predicate);
  assert(object);
  assert(var_subject || var_predicate || var_object);
  start.subject   = var_subject   ? TAG_FIRST : subject;
  start.predicate = var_predicate ? TAG_FIRST : predicate;
  start.object    = var_object    ? TAG_FIRST : object;
  end.subject   = var_subject   ? TAG_LAST : subject;
  end.predicate = var_predicate ? TAG_LAST : predicate;
  end.object    = var_object    ? TAG_LAST : object;
  if (! var_subject && var_object)
    tree = facts->index_spo;
  else if (! var_predicate)
    tree = facts->index_pos;
  else
    tree = facts->index_osp;
  facts_cursor_init(cursor, tree, &start, &end);
  cursor->var_subject = var_subject;
  cursor->var_predicate = var_predicate;
  cursor->var_object = var_object;
  return cursor;
}

s_facts_cursor * facts_with_3 (const s_facts *facts,
                               s_facts_cursor *cursor,
                               const s_tag *subject,
                               const s_tag *predicate,
                               const s_tag *object)
{
  s_fact fact;
  assert(facts);
  assert(cursor);
  assert(subject);
  assert(predicate);
  assert(object);
  fact.subject = subject;
  fact.predicate = predicate;
  fact.object = object;
  return facts_cursor_init(cursor, facts->index_spo, &fact, &fact);
}

s_facts_cursor * facts_with_tags (const s_facts *facts,
                                  s_facts_cursor *cursor,
                                  s_tag *subject,
                                  s_tag *predicate,
                                  s_tag *object)
{
  s_tag *var_subject = NULL;
  s_tag *var_predicate = NULL;
  s_tag *var_object = NULL;
  assert(facts);
  assert(cursor);
  assert(subject);
  assert(predicate);
  assert(object);
  if (tag_is_unbound_var(subject))
    var_subject = subject;
  if (tag_is_unbound_var(predicate))
    var_predicate = predicate;
  if (tag_is_unbound_var(object))
    var_object = object;
  if (var_subject && var_predicate && var_object)
    return facts_with_0(facts, cursor, var_subject, var_predicate,
                        var_object);
  if (! (var_subject || var_predicate || var_object))
    return facts_with_3(facts, cursor, subject, predicate, object);
  return facts_with_1_2(facts, cursor, subject, predicate, object,
                        var_subject, var_predicate, var_object);
}
