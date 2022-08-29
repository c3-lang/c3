/*
 *  facts_db - in-memory graph database
 *  Copyright 2020 Thomas de Grivel <thoxdg@gmail.com>
 *
 *  Permission to use, copy, modify, and distribute this software for any
 *  purpose with or without fee is hereby granted, provided that the above
 *  copyright notice and this permission notice appear in all copies.
 *
 *  THE SOFTWARE IS PROVIDED "AS IS" AND THE AUTHOR DISCLAIMS ALL WARRANTIES
 *  WITH REGARD TO THIS SOFTWARE INCLUDING ALL IMPLIED WARRANTIES OF
 *  MERCHANTABILITY AND FITNESS. IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR
 *  ANY SPECIAL, DIRECT, INDIRECT, OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES
 *  WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS, WHETHER IN AN
 *  ACTION OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING OUT OF
 *  OR IN CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.
 */
#ifndef FACTS_H
#define FACTS_H

#include <stdio.h>
#include "binding.h"
#include "fact.h"
#include "set.h"
#include "skiplist.h"
#include "spec.h"

#define FACTS_SKIPLIST_SPACING 2.7
#define FACTS_LOAD_BUFSZ (1024 * 1024)

typedef struct facts
{
        s_set *symbols;
        size_t symbols_delete;
        s_set index;
        s_skiplist *index_spo;
        s_skiplist *index_pos;
        s_skiplist *index_osp;
        FILE *log;
} s_facts;

void              facts_init (s_facts *facts,
                              s_set *symbols,
                              unsigned long max);

void              facts_destroy (s_facts *facts);

s_facts *     new_facts (s_set *symbols,
                         unsigned long max);

void       delete_facts (s_facts *facts);

s_set_item *      facts_find_symbol (s_facts *facts,
                                     const char *string);

const char *      facts_find_symbol_str (s_facts *facts,
                                         const char *string);

const char *      facts_long (s_facts *facts,
                              long l);

const char *      facts_double (s_facts *facts,
                                double d);

long              facts_get_long (s_facts *facts,
                                  const char *string);

double            facts_get_double (s_facts *facts,
                                    const char *string);

const char *      facts_anon (s_facts *facts,
                              const char *name);

const char *      facts_intern (s_facts *facts,
                                const char *string);

s_fact *          facts_add_fact (s_facts *facts,
                                  s_fact *f);

s_fact *          facts_add_spo (s_facts *facts,
                                 const char *s,
                                 const char *p,
                                 const char *o);

int               facts_add (s_facts *facts,
                             p_spec spec);

int               facts_remove_fact (s_facts *facts,
                                     s_fact *f);

int               facts_remove_spo (s_facts *facts,
                                    const char *s,
                                    const char *p,
                                    const char *o);

int               facts_remove (s_facts *facts,
                                p_spec spec);

s_fact *          facts_get_fact (s_facts *facts,
                                  s_fact *f);

s_fact *          facts_get_spo (s_facts *facts,
                                 const char *s,
                                 const char *p,
                                 const char *o);

unsigned long     facts_count (s_facts *facts);

typedef struct facts_cursor {
        s_skiplist *tree;
        s_skiplist_node *node;
        s_fact start;
        s_fact end;
        const char **var_s;
        const char **var_p;
        const char **var_o;
} s_facts_cursor;

void              facts_cursor_init (s_facts *facts,
                                     s_facts_cursor *c,
                                     s_skiplist *tree,
                                     s_fact *start,
                                     s_fact *end);

s_fact *          facts_cursor_next (s_facts_cursor *c);

void              facts_with_3 (s_facts *facts,
                                s_facts_cursor *c,
                                const char *s,
                                const char *p,
                                const char *o);

void              facts_with_0 (s_facts *facts,
                                s_facts_cursor *c,
                                const char **var_s,
                                const char **var_p,
                                const char **var_o);

void              facts_with_1_2 (s_facts *facts,
                                  s_facts_cursor *c,
                                  const char *s,
                                  const char *p,
                                  const char *o,
                                  const char **var_s,
                                  const char **var_p,
                                  const char **var_o);

void              facts_with_spo (s_facts *facts,
                                  s_binding *bindings,
                                  s_facts_cursor *c,
                                  const char *s,
                                  const char *p,
                                  const char *o);

typedef struct facts_with_cursor_level {
        s_facts_cursor c;
        s_fact *fact;
        p_spec spec;
} s_facts_with_cursor_level;

typedef struct facts_with_cursor {
        s_facts *facts;
        s_binding *bindings;
        size_t facts_count;
        s_facts_with_cursor_level *l;
        size_t level;
        p_spec spec;
} s_facts_with_cursor;

void              facts_with (s_facts *facts,
                              s_binding *bindings,
                              s_facts_with_cursor *c,
                              p_spec spec);

void              facts_with_cursor_destroy (s_facts_with_cursor *c);

int               facts_with_cursor_next (s_facts_with_cursor *c);

const char *      facts_get_prop (s_facts *facts,
                                  const char *s,
                                  const char *p);

long              facts_get_prop_long (s_facts *facts,
                                       const char *s,
                                       const char *p);

double            facts_get_prop_double (s_facts *facts,
                                         const char *s,
                                         const char *p);

s_fact *          facts_set_prop (s_facts *facts,
                                  const char *s,
                                  const char *p,
                                  const char *o);

#endif
