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
#include <string.h>
#include "farmhash.h"
#include "set.h"

s_set_item * new_set_item (size_t len, void *data, size_t hash,
                           s_set_item *next)
{
        s_set_item *i = calloc(1, sizeof(s_set_item));
        if (i) {
                i->len = len;
                i->data = data;
                i->hash = hash;
                i->next = next;
        }
        return i;
}

void set_init (s_set *set, size_t max)
{
        assert(set);
        assert(max > 0);
        set->max = max;
        set->items = calloc(max, sizeof(s_set_item));
        set->count = 0;
        set->collisions = 0;
        set->hash = (f_hash) farmhash;
}

void set_destroy (s_set *set)
{
        size_t i;
        assert(set);
        for (i = 0; i < set->max; i++) {
                s_set_item *j = set->items[i].next;
                while (j) {
                        s_set_item *k = j;
                        j = j->next;
                        free(k);
                }
        }
        set->max = 0;
        free(set->items);
        set->items = NULL;
        set->count = 0;
        set->collisions = 0;
}

s_set * new_set (size_t max)
{
        s_set *set;
        assert(max > 0);
        set = malloc(sizeof(s_set));
        if (set)
                set_init(set, max);
        return set;
}

void delete_set (s_set *set)
{
        if (set) {
                set_destroy(set);
                free(set);
        }
}

static s_set_item * set_add_collision (s_set *set, void *data,
                                       size_t len, size_t hash,
                                       s_set_item *i)
{
        s_set_item *n = new_set_item(len, data, hash, i->next);
        assert(n);
        i->next = n;
        set->count++;
        set->collisions++;
        return n;
}

s_set_item * set_add (s_set *set, void *data, size_t len)
{
        size_t hash;
        assert(set);
        assert(data);
        assert(len > 0);
        hash = set->hash(data, len);
        return set_add_h(set, data, len, hash);
}

s_set_item * set_add_h (s_set *set, void *data, size_t len, size_t hash)
{
        s_set_item *i;
        assert(set);
        assert(data);
        assert(len > 0);
        i = set_get_h(set, data, len, hash);
        if (i)
                return i;
        i = set->items + hash % set->max;
        if (i->len)
                return set_add_collision(set, data, len, hash, i);
        i->len = len;
        i->data = data;
        i->hash = hash;
        i->next = NULL;
        set->count++;
        return i;
}

static int set_remove_first (s_set *set, s_set_item *i)
{
        s_set_item *j = i->next;
        if (j) {
                i->len = j->len;
                i->data = j->data;
                i->hash = j->hash;
                i->next = j->next;
                free(j);
                set->collisions--;
        }
        else {
                i->len = 0;
                i->data = 0;
                i->hash = 0;
                i->next = 0;
        }
        set->count--;
        return 1;
}

static int set_remove_linked (s_set *set, s_set_item *item,
                              s_set_item *i)
{
        s_set_item **j = &i->next;
        while (*j && *j != item)
                j = &(*j)->next;
        if (!*j)
                return 0;
        i = *j;
        *j = (*j)->next;
        free(i);
        set->count--;
        set->collisions--;
        return 1;
}

int set_remove (s_set *set, s_set_item *item)
{
        s_set_item *i;
        assert(set);
        if (!item)
                return 0;
        i = set->items + (item->hash % set->max);
        if (i == item)
                return set_remove_first(set, i);
        else
                return set_remove_linked(set, item, i);
}

s_set_item * set_get (s_set *set, const void *data, size_t len)
{
        size_t hash;
        assert(set);
        assert(data);
        assert(len > 0);
        hash = set->hash(data, len);
        return set_get_h(set, data, len, hash);
}

s_set_item * set_get_h (s_set *set, const void *data, size_t len,
                        size_t hash)
{
        s_set_item *i;
        assert(set);
        assert(data);
        assert(len > 0);
        i = set_get_hash(set, hash);
        while (i) {
                if (len == i->len && memcmp(data, i->data, len) == 0)
                        return i;
                i = set_get_hash_next(i);
        }
        return NULL;
}

s_set_item * set_get_hash (s_set *set, size_t hash)
{
        s_set_item *i;
        assert(set);
        i = set->items + (hash % set->max);
        while (i && i->hash != hash)
                i = i->next;
        return i;
}

s_set_item * set_get_hash_next (s_set_item *item)
{
        s_set_item *i;
        assert(item);
        i = item->next;
        while (i && i->hash != item->hash)
                i = i->next;
        return i;
}

void set_resize (s_set *set, size_t max)
{
        size_t i;
        s_set n;
        if (max == set->max)
                return;
        set_init(&n, max);
        for (i = 0; i < set->max; i++) {
                s_set_item *item = set->items + i;
                if (!item->len)
                        item = NULL;
                while (item) {
                        set_add_h(&n, item->data, item->len,
                                  item->hash);
                        item = item->next;
                }
        }
        set_destroy(set);
        set->max = n.max;
        set->items = n.items;
        set->count = n.count;
        set->collisions = n.collisions;
}

void set_cursor_init (s_set *set, s_set_cursor *c)
{
        assert(set);
        assert(set->max > 0);
        assert(c);
        c->set = set;
        c->i = 0;
        c->item = NULL;
        c->count = 0;
}

static s_set_item * set_cursor_next_index (s_set_cursor *c)
{
        assert(c);
        assert(c->set);
        while (c->i < c->set->max && (!c->item || !c->item->len)) {
                c->item = c->set->items + c->i;
                c->i++;
        }
        if (c->item && c->item->len) {
                c->count++;
                return c->item;
        }
        return NULL;
}

s_set_item * set_cursor_next (s_set_cursor *c)
{
        assert(c);
        if (c->count == c->set->count)
                return NULL;
        if (!c->item)
                return set_cursor_next_index(c);
        c->item = c->item->next;
        if (!c->item)
                return set_cursor_next_index(c);
        c->count++;
        return c->item;
}
