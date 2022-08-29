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
#include <stdlib.h>
#include <strings.h>
#include "skiplist.h"

long int random(void);

void skiplist_node_init (s_skiplist_node *n,
                         void *value,
                         unsigned long height)
{
        n->value = value;
        n->height = height;
        bzero(skiplist_node_links(n), height * sizeof(void*));
}

s_skiplist_node * new_skiplist_node (void *value, unsigned long height)
{
        s_skiplist_node *n = malloc(skiplist_node_size(height));
        if (n)
                skiplist_node_init(n, value, height);
        return n;
}

void delete_skiplist_node (s_skiplist_node *n)
{
        free(n);
}

/*
  Random height
  -------------

  ∀ U ∈ ℕ : 1 < U
  ∀ n ∈ ℕ*
  ∀ r ∈ ℕ : r ≤ n
  ∀ random : ℕ* ⟶ ℕ
             random(n) ∈ [0..n-1]
             ∀ i ∈ [0..n-1], P(random(n) = i) = n⁻¹               (i)
  Qᵣ := random(Uⁿ) < Uⁿ⁻ʳ

  (i) ⇒        P(random(n) < v) = ∑ᵢ₌₀ᵛ⁻¹ P(random(n) = i)
      ⇒        P(random(n) < v) = v . n⁻¹                        (ii)

      ⇒    P(random(Uⁿ) < Uⁿ⁻ʳ) = Uⁿ⁻ʳ . (Uⁿ)⁻¹
      ⇔                   P(Qᵣ) = U⁻ʳ                           (iii)

  P(Qₙ) = P(random(Uⁿ) < U⁰)
        = P(random(Uⁿ) < 1)
        = P(random(Uⁿ) = 0)
        = U⁻ⁿ

  R := maxᵣ(Qᵣ)
     = maxᵣ(random(Uⁿ) < Uⁿ⁻ʳ)
     = maxᵣ(random(Uⁿ) + 1 ≤ Uⁿ⁻ʳ)
     = maxᵣ(logᵤ(random(Uⁿ) + 1) ≤ n - r)
     = maxᵣ(⌈logᵤ(random(Uⁿ) + 1)⌉ ≤ n - r)
     = maxᵣ(r ≤ n - ⌈logᵤ(random(Uⁿ) + 1)⌉)
     = n - ⌈logᵤ(random(Uⁿ) + 1)⌉                                (iv)

                       0 ≤ random(Uⁿ) < Uⁿ
   ⇔                   1 ≤ random(Uⁿ)+1 ≤ Uⁿ
   ⇔        logᵤ(1) ≤ logᵤ(random(Uⁿ)+1) ≤ logᵤ(Uⁿ)
   ⇔             0 ≤ ⌈logᵤ(random(Uⁿ)+1)⌉ ≤ n
   ⇔           -n ≤ -⌈logᵤ(random(Uⁿ)+1)⌉ ≤ 0
   ⇔         0 ≤ n - ⌈logᵤ(random(Uⁿ)+1)⌉ ≤ n
   ⇔                      0 ≤ R ≤ n                               (v)
*/

static void skiplist_height_table_ (s_skiplist *sl, double spacing)
{
        unsigned h;
        double w = spacing;
        double end = w;
        for (h = 0; h < sl->max_height; h++) {
                w *= spacing;
                end += w;
                skiplist_height_table(sl)[h] = end;
        }
}

void skiplist_init (s_skiplist *sl, int max_height, double spacing)
{
        assert(sl);
        sl->head = new_skiplist_node(NULL, max_height);
        sl->compare = skiplist_compare_ptr;
        sl->length = 0;
        sl->max_height = max_height;
        skiplist_height_table_(sl, spacing);
}

void skiplist_destroy (s_skiplist *sl)
{
        s_skiplist_node *n;
        assert(sl);
        assert(sl->head);
        while ((n = skiplist_node_next(sl->head, 0)))
                skiplist_remove(sl, n->value);
        delete_skiplist_node(sl->head);
        sl->head = NULL;
}

s_skiplist * new_skiplist (int max_height, double spacing)
{
        s_skiplist *sl = malloc(skiplist_size(max_height));
        if (sl)
          skiplist_init(sl, max_height, spacing);
        return sl;
}

void delete_skiplist (s_skiplist *sl)
{
        skiplist_destroy(sl);
        free(sl);
}

int skiplist_compare_ptr (void *a, void *b)
{
        if (a < b)
                return -1;
        if (a > b)
                return 1;
        return 0;
}

s_skiplist_node * skiplist_pred (s_skiplist *sl, void *value)
{
        int level = sl->max_height;
        s_skiplist_node *pred = new_skiplist_node(NULL,
                                                  sl->max_height);
        s_skiplist_node *node = sl->head;
        assert(pred);
        while (level--) {
                s_skiplist_node *n = node;
                while (n && sl->compare(value, n->value) > 0) {
                        node = n;
                        n = skiplist_node_next(node, level);
                }
                skiplist_node_next(pred, level) = node;
        }
        return pred;
}

void skiplist_node_insert (s_skiplist_node *n, s_skiplist_node *pred)
{
        unsigned level;
        for (level = 0; level < n->height; level++) {
                s_skiplist_node *p = skiplist_node_next(pred, level);
                skiplist_node_next(n, level) =
                        skiplist_node_next(p, level);
                skiplist_node_next(p, level) = n;
        }
}

unsigned skiplist_random_height (s_skiplist *sl)
{
        long max = skiplist_height_table(sl)[sl->max_height - 1];
        long k = random() % max;
        int i;
        for (i = 0; k > skiplist_height_table(sl)[i]; i++)
                ;
        return sl->max_height - i;
}

s_skiplist_node * skiplist_insert (s_skiplist *sl, void *value)
{
        s_skiplist_node *pred = skiplist_pred(sl, value);
        s_skiplist_node *next = skiplist_node_next(pred, 0);
        unsigned height;
        s_skiplist_node *n;
        next = skiplist_node_next(next, 0);
        if (next && sl->compare(value, next->value) == 0)
                return next;
        height = skiplist_random_height(sl);
        n = new_skiplist_node(value, height);
        skiplist_node_insert(n, pred);
        sl->length++;
        return n;
}

void * skiplist_remove (s_skiplist *sl, void *x)
{
        unsigned long level;
        s_skiplist_node *pred;
        s_skiplist_node *next;
        void *value;
        pred = skiplist_pred(sl, x);
        assert(pred);
        next = skiplist_node_next(pred, 0);
        assert(next);
        next = skiplist_node_next(next, 0);
        if (!next || sl->compare(x, next->value) != 0)
                return NULL;
        for (level = 0; level < next->height; level++) {
                s_skiplist_node *p = skiplist_node_next(pred, level);
                skiplist_node_next(p, level) =
                        skiplist_node_next(next, level);
        }
        value = next->value;
        sl->length--;
        return value;
}

s_skiplist_node * skiplist_find (s_skiplist *sl, void *value)
{
        s_skiplist_node *node = sl->head;
        int level = node->height;
        while (level--) {
                s_skiplist_node *n = node;
                int c = -1;
                while (n && (c = sl->compare(value, n->value)) > 0)
                        n = skiplist_node_next(n, level);
                if (c == 0)
                        return n;
        }
        return NULL;
}

s_skiplist_node * skiplist_cursor (s_skiplist *sl, void *start)
{
        s_skiplist_node *pred;
        s_skiplist_node *n;
        assert(sl);
        pred = skiplist_pred(sl, start);
        n = skiplist_node_next(pred, 0);
        assert(n);
        n = skiplist_node_next(n, 0);
        delete_skiplist_node(pred);
        return n;
}

void skiplist_cursor_init (s_skiplist *sl, s_skiplist_cursor *c,
                           void *start, void *end)
{
        s_skiplist_node *pred;
        assert(sl);
        assert(c);
        pred = skiplist_pred(sl, start);
        c->sl = sl;
        c->n = skiplist_node_next(pred, 0);
        c->end = end;
        assert(c->n);
        delete_skiplist_node(pred);
}

s_skiplist_node * skiplist_cursor_next (s_skiplist_cursor *c)
{
        assert(c);
        if (c->n) {
                assert(c->sl);
                c->n = skiplist_node_next(c->n, 0);
                if (c->n && c->end &&
                    c->sl->compare(c->n->value, c->end) > 0)
                        c->n = NULL;
        }
        return c->n;
}
