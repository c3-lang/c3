/* c3
 * Copyright 2022 Thomas de Grivel <thoxdg@gmail.com>
 */
/**
 * @file symbol.h
 * @brief Symbol data structure.
 *
 * Structure to manipulate symbols.
 * Symbols are strings that are deduplicated.
 * They are allocated once and only free'd at the end of the program.
 * Comparing symbols is fast : it's a pointer comparison.
 */
#ifndef SYMBOL_H
#define SYMBOL_H

#include <stdio.h>
#include "types.h"

/** @brief Make symbol from C string. */
s_symbol * symbol_1 (s8 *p);

/** @brief Compare symbols. */
sw symbol_cmp (s_symbol *a, s_symbol *b);

/** @brief Call when exiting program. */
void symbol_delete_all ();

/** @brief Call str_free after use. */
s_str * symbol_inspect (s_symbol *sym);

#endif /* SYMBOL_H */
