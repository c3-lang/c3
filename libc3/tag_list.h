/* c3
 * Copyright 2022 Thomas de Grivel <thoxdg@gmail.com>
 */
/**
 * @file tag_list.h
 * @brief Tagged list
 *
 * Structure to add a type tag to data.
 */
#ifndef TAG_LIST_H
#define TAG_LIST_H

#include <stdarg.h>
#include <stdio.h>
#include "types.h"

/* Stack allocation compatible functions */
s_tag_list * tag_list_init (s_tag_list *tag_list, s_tag *tag,
                            s_tag_list *next);

/* Constructors, call tag_list_delete after use */
s_tag_list * tag_list_new (s_tag *tag, s_tag_list *next);

/* Destructor */
void tag_list_delete (s_tag_list *tag_list);

/* Observers */
sw tag_list_length (s_tag_list *tag_list);

/* Call str_delete after use. */
s_str * tag_list_inspect (const s_tag_list *x);

#endif /* STR_H */
