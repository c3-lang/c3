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
#ifndef SKIPLIST_H
#define SKIPLIST_H

#include <math.h>

typedef struct skiplist_node {
        void *value;
        unsigned long height;
} s_skiplist_node;

#define skiplist_node_size(height) \
        (sizeof(s_skiplist_node) + height * sizeof(void*))

void                  skiplist_node_init (s_skiplist_node *n,
                                          void *value,
                                          unsigned long height);

s_skiplist_node * new_skiplist_node (void *value,
                                     unsigned long height);

void           delete_skiplist_node (s_skiplist_node *n);

#define               skiplist_node_links(n) \
        ((s_skiplist_node**) (((s_skiplist_node*) n) + 1))

#define               skiplist_node_next(n, height)     \
        (skiplist_node_links(n)[height])

void                  skiplist_node_insert (s_skiplist_node *n,
                                            s_skiplist_node *pred);

typedef struct skiplist {
        s_skiplist_node *head;
        int (*compare) (void *value1, void *value2);
        unsigned long length;
        unsigned long max_height;
} s_skiplist;

#define skiplist_size(max_height) \
        (sizeof(s_skiplist) + max_height * sizeof(long))

#define skiplist_height_table(sl) \
        ((long*) (((s_skiplist*) sl) + 1))

void              skiplist_init (s_skiplist *sl,
                                 int max_height,
                                 double spacing);

void              skiplist_destroy (s_skiplist *sl);

s_skiplist *  new_skiplist (int max_height,
                            double spacing);

void       delete_skiplist (s_skiplist *sl);

int               skiplist_compare_ptr (void *a,
                                        void *b);

unsigned          skiplist_random_height (s_skiplist *sl);

s_skiplist_node * skiplist_pred (s_skiplist *sl,
                                 void *value);

s_skiplist_node * skiplist_insert (s_skiplist *sl,
                                   void *value);

void *            skiplist_remove (s_skiplist *sl,
                                   void *value);

s_skiplist_node * skiplist_find (s_skiplist *sl,
                                 void *value);

s_skiplist_node * skiplist_cursor (s_skiplist *sl,
                                   void *start);

typedef struct skiplist_cursor {
        s_skiplist *sl;
        s_skiplist_node *n;
        void *end;
} s_skiplist_cursor;

void              skiplist_cursor_init (s_skiplist *sl,
                           s_skiplist_cursor *c,
                           void *start,
                           void *end);

s_skiplist_node * skiplist_cursor_next (s_skiplist_cursor *c);

#endif
