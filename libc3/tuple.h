/* c3
 * Copyright 2022 Thomas de Grivel <thoxdg@gmail.com>
 */
/**
 * @file tuple.h
 * @brief Tag tuples.
 *
 * A tuple is a static array of tags.
 */
#ifndef TUPLE_H
#define TUPLE_H

#include "types.h"

/* Stack allocation compatible functions */
s_tuple * tuple_init (s_tuple *tuple, uw count);
void      tuple_clean (s_tuple *tuple);

/* Constructors, call tuple_delete after use */
s_tuple * tuple_new (uw count);

/* Destructor */
void tuple_delete (s_tuple *tuple);

/* Observers */
s_list * tuple_to_list (const s_tuple *tuple, s_list **list);
/* Call str_delete after use. */
s_str *   tuple_inspect (const s_tuple *x);

#endif /* STR_H */
