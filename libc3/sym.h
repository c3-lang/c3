/* c3
 * Copyright 2022 Thomas de Grivel <thoxdg@gmail.com>
 */
/**
 * @file sym.h
 * @brief Symbol data structure.
 *
 * Structure to manipulate symbols.
 * Symbols are strings that are deduplicated.
 * They are allocated once and only free'd at the end of the program.
 * Comparing symbols is fast : it's a pointer comparison.
 */
#ifndef SYM_H
#define SYM_H

#include "types.h"

/** @brief Make symbol from C string. */
s_sym * sym_1 (const s8 *p);

/** @brief Call when exiting program. */
void sym_delete_all ();

/** @brief Call str_free after use. */
s_str * sym_inspect (s_sym *sym);

/** @brief True iff sym is a module name (starts with a capital). */
bool sym_is_module (s_sym *sym);

#endif /* SYM_H */
