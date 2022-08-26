/* c3
 * Copyright 2022 Thomas de Grivel <thoxdg@gmail.com>
 */
/**
 * @file inspect.h
 * @brief Inspect anything into a UTF-8 string.
 */
#ifndef INSPECT_H
#define INSPECT_H

/* Call str_delete after use. */
s_str * inspect_str (const s_str *x);
s_str * inspect_sym (const s_sym *sym);

#endif /* INSPECT_H */
