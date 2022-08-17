/* c3
 * Copyright 2022 Thomas de Grivel <thoxdg@gmail.com>
 */
#ifndef C3_H
#define C3_H

#include "str.h"

typedef enum {
  C3_ATOM,
  C3_COMMENT,
  C3_IDENT,
  C3_STRING,
  C3_WHITESPACE
} e_c3_token_type;

typedef struct c3_token {
  e_c3_token_type type;
  str *str;
} s_c3_token;

#endif /* C3_H */
