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

typedef s64 character;

typedef union ptr {
  const void *p;
  const s8   *ps8;
  const u8   *pu8;
  const u64   u64;
} u_ptr;

typedef union ptr_w {
  void *p;
  s8   *ps8;
  u8   *pu8;
  u64   u64;
} u_ptr_w;

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

typedef struct buf s_buf;

struct buf {
  bool free;
  u64 size;
  u_ptr_w ptr;
  u64 rpos;
  u64 wpos;
  sw (*flush) (s_buf *buf);
  sw (*refill) (s_buf *buf);
};

#endif
