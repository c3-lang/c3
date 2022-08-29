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

#include <assert.h>
#include <math.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>
#include "facts.h"
#include "rw.h"

void facts_init (s_facts *facts, s_set *symbols, unsigned long max)
{
        unsigned long height;
        assert(facts);
        facts->symbols = symbols ? symbols : new_set(max);
        facts->symbols_delete = !symbols;
        set_init(&facts->index, max);
        height = log(max) / log(FACTS_SKIPLIST_SPACING);
        facts->index_spo = new_skiplist(height, FACTS_SKIPLIST_SPACING);
        assert(facts->index_spo);
        facts->index_spo->compare = fact_compare_spo;
        facts->index_pos = new_skiplist(height, FACTS_SKIPLIST_SPACING);
        assert(facts->index_pos);
        facts->index_pos->compare = fact_compare_pos;
        facts->index_osp = new_skiplist(height, FACTS_SKIPLIST_SPACING);
        assert(facts->index_osp);
        facts->index_osp->compare = fact_compare_osp;
        facts->log = NULL;
}

void facts_destroy (s_facts *facts)
{
        delete_skiplist(facts->index_spo);
        delete_skiplist(facts->index_pos);
        delete_skiplist(facts->index_osp);
        set_destroy(&facts->index);
        if (facts->symbols_delete)
                delete_set(facts->symbols);
}

s_facts * new_facts (s_set *symbols, unsigned long max)
{
        s_facts *facts = malloc(sizeof(s_facts));
        if (facts)
                facts_init(facts, symbols, max);
        return facts;
}

void delete_facts (s_facts *facts)
{
        facts_destroy(facts);
        free(facts);
}

s_set_item * facts_find_symbol (s_facts *facts, const char *string)
{
        size_t len;
        assert(facts);
        assert(string);
        len = strlen(string);
        return set_get(facts->symbols, string, len);
}

const char * facts_find_symbol_str (s_facts *facts, const char *string)
{
        s_set_item *si = facts_find_symbol(facts, string);
        if (si)
                return si->data;
        return NULL;
}

const char * facts_long (s_facts *facts, long l)
{
        char buf[48];
        snprintf(buf, sizeof(buf), "%li", l);
        return facts_intern(facts, buf);
}

const char * facts_double (s_facts *facts,
                           double d)
{
        char buf[32];
        snprintf(buf, sizeof(buf), "%.16g", d);
        return facts_intern(facts, buf);
}

long facts_get_long (s_facts *facts, const char *string)
{
        s_set_item *i;
        assert(facts);
        assert(string);
        i = facts_find_symbol(facts, string);
        if (i) {
                if (!i->long_p) {
                        i->long_value = atol(string);
                        i->long_p = 1;
                }
                return i->long_value;
        }
        return 0;
}

double facts_get_double (s_facts *facts, const char *string)
{
        s_set_item *i;
        assert(facts);
        assert(string);
        i = facts_find_symbol(facts, string);
        if (i) {
                if (!i->double_p) {
                        i->double_value = atol(string);
                        i->double_p = 1;
                }
                return i->double_value;
        }
        return 0.0;
}

s_set_item * facts_add_symbol (s_facts *facts, const char *string,
                               size_t len)
{
        char *data = malloc(len + 1);
        memcpy(data, string, len + 1);
        return set_add(facts->symbols, data, len);
}

const char * facts_intern (s_facts *facts, const char *string)
{
        s_set_item *i;
        size_t len;
        assert(facts);
        assert(string);
        len = strlen(string);
        i = set_get(facts->symbols, string, len);
        if (!i)
                i = facts_add_symbol(facts, string, len);
        assert(i);
        i->usage++;
        return i->data;
}

void facts_unintern (s_facts *facts, const char *string)
{
        s_set_item *i;
        assert(facts);
        assert(string);
        i = facts_find_symbol(facts, string);
        if (i) {
                i->usage--;
                if (!i->usage)
                        set_remove(facts->symbols, i);
        }
}

void random_id (char *buf, size_t len)
{
        static const char base64url[] =
                "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
                "abcdefghijklmnopqrstuvwxyz"
                "0123456789-_";
        int r = rand();
        int m = RAND_MAX;
        while (len--) {
                if (m < 64) {
                        r = rand();
                        m = RAND_MAX;
                }
                *buf = base64url[r % 64];
                buf++;
                r /= 64;
                m /= 64;
        }
}

const char * facts_anon (s_facts *facts, const char *name)
{
        char buf[1024];
        char *b = buf;
        int i = 0;
        assert(facts);
        if (name && name[0] == '?')
                name++;
        if (!name || !name[0])
                name = "anon";
        while ((unsigned) i < sizeof(buf) - 12 && name[i])
                *b++ = name[i++];
        *b++ = '-';
        while (1) {
                random_id(b, 10);
                b[10] = 0;
                if (!facts_find_symbol(facts, buf))
                        return facts_intern(facts, buf);
        }
        return NULL;
}

s_fact * facts_add_fact (s_facts *facts, s_fact *f)
{
        s_fact intern;
        s_fact *found;
        s_fact *new;
        assert(facts);
        assert(f);
        intern.s = facts_intern(facts, f->s);
        intern.p = facts_intern(facts, f->p);
        intern.o = facts_intern(facts, f->o);
        if ((found = facts_get_fact(facts, &intern)))
                return found;
        if (facts->log)
                write_fact_log("add", &intern, facts->log);
        new = new_fact(intern.s, intern.p, intern.o);
        assert(new);
        skiplist_insert(facts->index_spo, new);
        skiplist_insert(facts->index_pos, new);
        skiplist_insert(facts->index_osp, new);
        set_add(&facts->index, new, sizeof(s_fact));
        return new;
}

s_fact * facts_add_spo (s_facts *facts, const char *s,
                        const char *p, const char *o)
{
        s_fact f;
        assert(facts);
        assert(s);
        assert(p);
        assert(o);
        f.s = s;
        f.p = p;
        f.o = o;
        return facts_add_fact(facts, &f);
}

const char ** spec_bindings_anon_assoc (s_facts *facts, p_spec spec)
{
        const char **b;
        const char **bindings;
        size_t count;
        assert(spec);
        count = spec_count_bindings(spec);
        bindings = calloc(count * 2 + 1, sizeof(char*));
        if (bindings) {
                size_t s = 0;
                b = bindings;
                while (spec[s] || spec[s + 1]) {
                        if (spec[s] && spec[s][0] == '?') {
                                *b++ = spec[s];
                                *b++ = facts_anon(facts, spec[s]);
                        }
                        s++;
                }
                *b = NULL;
        }
        return bindings;
}

const char * assoc_get (const char **kv,
                        const char *k)
{
        assert(kv);
        assert(k);
        while (*kv && strcmp(*kv, k))
                kv += 2;
        if (*kv)
                return kv[1];
        return NULL;
}

int facts_add (s_facts *facts, p_spec spec)
{
        const char **anon;
        s_spec_cursor c;
        s_fact f;
        assert(facts);
        assert(spec);
        anon = spec_bindings_anon_assoc(facts, spec);
        if (!anon)
                return -1;
        spec_cursor_init(&c, spec);
        while (spec_cursor_next(&c, &f)) {
                if (f.s[0] == '?')
                        f.s = assoc_get(anon, f.s);
                if (f.p[0] == '?')
                        f.p = assoc_get(anon, f.p);
                if (f.o[0] == '?')
                        f.o = assoc_get(anon, f.o);
                facts_add_fact(facts, &f);
        }
        free(anon);
        return 0;
}

int facts_remove_fact (s_facts *facts, s_fact *f)
{
        s_fact *found;
        assert(facts);
        found = skiplist_remove(facts->index_spo, f);
        if (found) {
                skiplist_remove(facts->index_pos, found);
                skiplist_remove(facts->index_osp, found);
                if (facts->log)
                        write_fact_log("remove", found, facts->log);
                facts_unintern(facts, found->s);
                facts_unintern(facts, found->p);
                facts_unintern(facts, found->o);
                delete_fact(found);
                return 1;
        }
        return 0;
}

int facts_remove_spo (s_facts *facts, const char *s,
                      const char *p, const char *o)
{
        s_fact f;
        assert(facts);
        assert(s);
        assert(p);
        assert(o);
        f.s = s;
        f.p = p;
        f.o = o;
        return facts_remove_fact(facts, &f);
}

int facts_remove (s_facts *facts, p_spec spec)
{
        s_facts_with_cursor wc;
        s_binding *bindings;
        s_fact_list *fl = NULL;
        s_fact_list *fli;
        int found = 0;
        bindings = spec_bindings(spec);
        facts_with(facts, bindings, &wc, spec);
        while (facts_with_cursor_next(&wc)) {
                s_fact f;
                s_spec_cursor sc;
                spec_cursor_init(&sc, spec);
                while (spec_cursor_next(&sc, &f)) {
                        s_fact *dbf;
                        fact_bindings_resolve(&f, bindings);
                        dbf = facts_get_fact(facts, &f);
                        fl = fact_list_intern(fl, dbf);
                }
        }
        facts_with_cursor_destroy(&wc);
        free(bindings);
        fli = fl;
        while (fli) {
                if (facts_remove_fact(facts, fli->fact))
                        found = 1;
                fli = fli->next;
        }
        delete_fact_list(fl);
        return found;
}

s_fact * facts_get_fact (s_facts *facts, s_fact *f)
{
        s_fact fact;
        s_set_item *si;
        assert(facts);
        assert(f);
        if (!(fact.s = facts_find_symbol_str(facts, f->s)))
                return NULL;
        if (!(fact.p = facts_find_symbol_str(facts, f->p)))
                return NULL;
        if (!(fact.o = facts_find_symbol_str(facts, f->o)))
                return NULL;
        if ((si = set_get(&facts->index, &fact, sizeof(s_fact))))
                return (s_fact *) si->data;
        return NULL;
}

s_fact * facts_get_spo (s_facts *facts, const char *s, const char *p,
                        const char *o)
{
        s_fact f;
        f.s = s;
        f.p = p;
        f.o = o;
        return facts_get_fact(facts, &f);
}

unsigned long facts_count (s_facts *facts)
{
        assert(facts);
        return facts->index_spo->length;
}

void facts_cursor_init (s_facts *facts,
                        s_facts_cursor *c,
                        s_skiplist *tree,
                        s_fact *start,
                        s_fact *end)
{
        s_skiplist_node *pred;
        (void) facts;
        assert(facts);
        assert(c);
        assert(tree);
        pred = skiplist_pred(tree, start);
        assert(pred);
        c->tree = tree;
        c->node = skiplist_node_next(pred, 0);
        if (start)
                c->start = *start;
        else {
                c->start.s = P_FIRST;
                c->start.p = P_FIRST;
                c->start.o = P_FIRST;
        }
        if (end)
                c->end = *end;
        else {
                c->end.s = P_LAST;
                c->end.p = P_LAST;
                c->end.o = P_LAST;
        }
        delete_skiplist_node(pred);
        c->var_s = NULL;
        c->var_p = NULL;
        c->var_o = NULL;
}

s_fact * facts_cursor_next (s_facts_cursor *c)
{
        assert(c);
        if (c->node) {
                c->node = skiplist_node_next(c->node, 0);
                if (c->node &&
                    c->tree->compare(c->node->value, &c->end) > 0)
                        c->node = NULL;
        }
        if (c->node) {
                s_fact *f = (s_fact*) c->node->value;
                if (c->var_s)
                        *c->var_s = f->s;
                if (c->var_p)
                        *c->var_p = f->p;
                if (c->var_o)
                        *c->var_o = f->o;
                return f;
        }
        if (c->var_s)
                *c->var_s = NULL;
        if (c->var_p)
                *c->var_p = NULL;
        if (c->var_o)
                *c->var_o = NULL;
        return NULL;
}

void facts_with_3 (s_facts *facts, s_facts_cursor *c, const char *s,
                   const char *p, const char *o)
{
        s_fact f;
        assert(facts);
        assert(c);
        assert(s);
        assert(p);
        assert(o);
        f.s = s;
        f.p = p;
        f.o = o;
        facts_cursor_init(facts, c, facts->index_spo, &f, &f);
}

void facts_with_0 (s_facts *facts, s_facts_cursor *c,
                   const char **var_s, const char **var_p,
                   const char **var_o)
{
        assert(facts);
        assert(c);
        facts_cursor_init(facts, c, facts->index_spo, NULL, NULL);
        c->var_s = var_s;
        c->var_p = var_p;
        c->var_o = var_o;
}

void facts_with_1_2 (s_facts *facts, s_facts_cursor *c, const char *s,
                     const char *p, const char *o, const char **var_s,
                     const char **var_p, const char **var_o)
{
        s_fact start;
        s_fact end;
        s_skiplist *tree;
        assert(facts);
        assert(c);
        assert(s);
        assert(p);
        assert(o);
        assert(var_s || var_p || var_o);
        start.s = var_s ? P_FIRST : s;
        start.p = var_p ? P_FIRST : p;
        start.o = var_o ? P_FIRST : o;
        end.s = var_s ? P_LAST : s;
        end.p = var_p ? P_LAST : p;
        end.o = var_o ? P_LAST : o;
        tree = (!var_s && var_o) ? facts->index_spo :
                !var_p ? facts->index_pos :
                facts->index_osp;
        facts_cursor_init(facts, c, tree, &start, &end);
        c->var_s = var_s;
        c->var_p = var_p;
        c->var_o = var_o;
}

void facts_with_spo (s_facts *facts,
                     s_binding *bindings,
                     s_facts_cursor *c,
                     const char *s,
                     const char *p,
                     const char *o)
{
        const char **var_s;
        const char **var_p;
        const char **var_o;
        assert(facts);
        assert(c);
        assert(s);
        assert(p);
        assert(o);
        var_s = (s[0] == '?') ? bindings_get(bindings, s) : NULL;
        var_p = (p[0] == '?') ? bindings_get(bindings, p) : NULL;
        var_o = (o[0] == '?') ? bindings_get(bindings, o) : NULL;
        if (var_s && var_p && var_o)
                facts_with_0(facts, c, var_s, var_p, var_o);
        else if (!(var_s || var_p || var_o))
                facts_with_3(facts, c, s, p, o);
        else
                facts_with_1_2(facts, c, s, p, o, var_s, var_p, var_o);
}


void facts_with (s_facts *facts, s_binding *bindings,
                 s_facts_with_cursor *c, p_spec spec)
{
        size_t facts_count;
        assert(facts);
        assert(c);
        assert(spec);
        facts_count = spec_count_facts(spec);
        c->facts = facts;
        c->bindings = bindings;
        bindings_nullify(c->bindings);
        c->facts_count = facts_count;
        if (facts_count > 0) {
                c->l = calloc(facts_count,
                              sizeof(s_facts_with_cursor_level));
                c->spec = spec_expand(spec);
                spec_sort(c->spec);
        }
        else {
                c->l = NULL;
                c->spec = NULL;
        }
        c->level = 0;
}

void facts_with_cursor_destroy (s_facts_with_cursor *c)
{
        assert(c);
        free(c->l);
        if (c->spec)
                free(c->spec);
        c->facts = NULL;
        c->bindings = NULL;
        c->facts_count = 0;
        c->l = NULL;
        c->level = 0;
        c->spec = NULL;
}

void spec_subst (p_spec spec, s_binding *bindings)
{
        size_t i = 0;
        if (spec && spec[0])
                while (spec[i] || spec[i + 1]) {
                        if (spec[i] && spec[i][0] == '?') {
                                const char **b =
                                        bindings_get(bindings, spec[i]);
                                if (*b)
                                        spec[i] = *b;
                        }
                        i++;
                }
}

int facts_with_cursor_next (s_facts_with_cursor *c)
{
        assert(c);
        if (!c->facts_count)
                return 0;
        if (c->level == c->facts_count) {
                s_facts_with_cursor_level *l =
                        c->l + (c->facts_count - 1);
                l->fact = facts_cursor_next(&l->c);
                if (l->fact)
                        return 1;
                free(l->spec);
                l->spec = NULL;
                c->level--;
                if (!c->level) {
                        c->facts_count = 0;
                        return 0;
                }
                c->level--;
        }
        while (c->level < c->facts_count) {
                s_facts_with_cursor_level *l = c->l + c->level;
                if (!l->spec) {
                        p_spec parent_spec = c->level ?
                                c->l[c->level - 1].spec + 4:
                                c->spec;
                        l->spec = spec_expand(parent_spec);
                        spec_subst(l->spec, c->bindings);
                        facts_with_spo(c->facts, c->bindings,
                                       &l->c, l->spec[0],
                                       l->spec[1], l->spec[2]);
                }
                l->fact = facts_cursor_next(&l->c);
                if (l->fact)
                        c->level++;
                else {
                        free(l->spec);
                        l->spec = NULL;
                        if (c->level > 0) {
                                c->level--;
                                if (!c->level) {
                                        c->facts_count = 0;
                                        return 0;
                                }
                        }
                        else {
                                c->facts_count = 0;
                                return 0;
                        }
                }
        }
        return 1;
}

const char * facts_get_prop (s_facts *facts, const char *s,
                             const char *p)
{
        const char *o = NULL;
        s_binding bindings[] = {{"?o", &o}, {NULL, NULL}};
        s_facts_cursor c;
        facts_with_spo(facts, bindings, &c, s, p, "?o");
        if (facts_cursor_next(&c))
                return o;
        return NULL;
}

long facts_get_prop_long (s_facts *facts, const char *s,
                          const char *p)
{
        const char *o = facts_get_prop(facts, s, p);
        return facts_get_long(facts, o);
}

double facts_get_prop_double (s_facts *facts, const char *s,
                              const char *p)
{
        const char *o = facts_get_prop(facts, s, p);
        return facts_get_double(facts, o);
}

s_fact * facts_set_prop (s_facts *facts, const char *s,
                         const char *p, const char *o)
{
        facts_remove(facts, (const char *[]) {s, p, "?o", NULL, NULL});
        return facts_add_spo(facts, s, p, o);
}
