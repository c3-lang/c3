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

typedef struct sym_list s_sym_list;

typedef struct sym {
  s_str str;
} s_sym;

typedef union sym_list_ptr {
  s_sym_list *p;
  u64 u64;
} u_sym_list_ptr;
  
struct sym_list {
  s_sym *sym;
  u_sym_list_ptr next;
};

#endif
