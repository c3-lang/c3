/* c3
 * Copyright 2022 Thomas de Grivel <thoxdg@gmail.com>
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
#include "types.h"

/* Stack allocation compatible functions */
s_list * list_init (s_list *list);
void     list_clean (s_list *list);

/* Constructors, call list_delete after use */
s_list * list_new ();

/* Destructor */
void list_delete (s_list *list);

/* Observers */
sw        list_length (const s_list *list);
s_list *  list_next (const s_list *list);
s_tuple * list_to_tuple_reverse (const s_list *list, s_tuple *tuple);
/* Call str_delete after use. */
s_str *   list_inspect (const s_list *x);

#endif /* STR_H */
