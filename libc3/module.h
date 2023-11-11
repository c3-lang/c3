/* c3
 * Copyright 2022,2023 kmx.io <contact@kmx.io>
 *
 * Permission is hereby granted to use this software granted the above
 * copyright notice and this permission paragraph are included in all
 * copies and substantial portions of this software.
 *
 * THIS SOFTWARE IS PROVIDED "AS-IS" WITHOUT ANY GUARANTEE OF
 * PURPOSE AND PERFORMANCE. IN NO EVENT WHATSOEVER SHALL THE
 * AUTHOR BE CONSIDERED LIABLE FOR THE USE AND PERFORMANCE OF
 * THIS SOFTWARE.
 */
/**
 * @file module.h
 * @brief Module
 *
 * Structure to manipulate modules.
 * A module contains symbols which have a type and an optional value.
 */
#ifndef MODULE_H
#define MODULE_H

#include "types.h"

/* Modifiers */
bool          module_ensure_loaded (const s_sym *module, s_facts *facts);
bool          module_load (const s_sym *module, s_facts *facts);
bool          module_maybe_reload (const s_sym *module, s_facts *facts);

/* Observers */
s_tag *       module_load_time (const s_sym *module, s_facts *facts,
                                s_tag *dest);
s_str *       module_path (const s_sym *module, const s_str *prefix,
                           s_str *dest);
sw            module_path_size (const s_sym *module, const s_str *prefix);
const s_sym * module_to_sym (const s_sym *module);

/* Operations */
bool          module_defmodule (const s_sym *name, const s_quote *block);
bool          module_def (const s_sym *module, const s_sym *name,
                          const s_quote *block);

#endif /* MODULE_H */
