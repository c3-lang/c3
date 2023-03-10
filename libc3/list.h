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
/**
 * @file list.h
 * @brief Tag list
 *
 * Linked list of tags.
 */
#ifndef LIST_H
#define LIST_H

#include <stdarg.h>
#include <stdio.h>
#include "hash.h"
#include "types.h"

/* Stack allocation compatible functions, do not use */
s_list * list_init (s_list *list);
void     list_clean (s_list *list);

/* Constructors, call list_delete after use */
s_list * list_1 (const s8 *p);
s_list * list_new ();

/* Destructor */
s_list * list_delete (s_list *list);
void     list_delete_all (s_list *list);

/* Observers */
s_list *  list_copy (const s_list *src, s_list **dest);
sw        list_length (const s_list *list);
s_list *  list_next (const s_list *list);
s_tuple * list_to_tuple_reverse (s_list *list, s_tuple *tuple);
/* Call str_delete after use. */
s_str *   list_inspect (const s_list *list, s_str *dest);

#endif /* STR_H */
