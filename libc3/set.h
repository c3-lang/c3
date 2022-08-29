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
#ifndef SET_H
#define SET_H

typedef struct set_item s_set_item;

struct set_item {
        void *data;
        size_t len;
        size_t hash;
        s_set_item *next;
        unsigned long usage;
        int  long_p;
        long long_value;
        int    double_p;
        double double_value;
};

s_set_item * new_set_item (size_t len, void *data, size_t hash, s_set_item *next);

typedef size_t (*f_hash) (const void *data,
                          size_t len);

typedef struct set {
        size_t max;
        s_set_item *items;
        size_t count;
        size_t collisions;
        f_hash hash;
} s_set;

void         set_init (s_set *set,
                       size_t max);

void         set_destroy (s_set *set);

s_set *  new_set (size_t max);

void  delete_set (s_set *set);

s_set_item * set_add (s_set *set,
                      void *data,
                      size_t len);

s_set_item * set_add_h (s_set *set,
                        void *data,
                        size_t len,
                        size_t hash);

int          set_remove (s_set *set,
                         s_set_item *item);

s_set_item * set_get (s_set *set,
                      const void *data,
                      size_t len);

s_set_item * set_get_h (s_set *set,
                        const void *data,
                        size_t len,
                        size_t hash);

s_set_item * set_get_hash (s_set *set,
                           size_t hash);

s_set_item * set_get_hash_next (s_set_item *item);

void         set_resize (s_set *set,
                         size_t max);

typedef struct set_cursor {
        s_set *set;
        size_t i;
        s_set_item *item;
        size_t count;
} s_set_cursor;

void         set_cursor_init (s_set *set,
                              s_set_cursor *c);

s_set_item * set_cursor_next (s_set_cursor *c);

#endif
