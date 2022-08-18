/* types
 * Copyright 2022 Thomas de Grivel <thoxdg@gmail.com>
 */
#ifndef TYPES_H
#define TYPES_H

typedef char               s8;
typedef short              s16;
typedef int                s32;
typedef long               sw;
typedef long long          s64;
typedef unsigned char      u8;
typedef unsigned short     u16;
typedef unsigned int       u32;
typedef unsigned long      uw;
typedef unsigned long long u64;

typedef sw bool;

typedef enum {
  false = 0,
  true = 1
} e_bool;

typedef union ptr {
  const void *p;
  u64 u64;
} u_ptr;

typedef struct str {
  bool free;
  u64 bytes;
  u_ptr ptr;
} s_str;

typedef struct symbol s_symbol;

typedef union symbol_ptr {
  s_symbol *symbol;
  u64 u64;
} u_symbol_ptr;
  
struct symbol {
  s_str str;
  u_symbol_ptr next;
};

#endif
