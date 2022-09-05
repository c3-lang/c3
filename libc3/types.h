/* c3
 * Copyright 2022 Thomas de Grivel <thoxdg@gmail.com>
 */
#ifndef TYPES_H
#define TYPES_H

#include <stdio.h>

/* Basic integer types. */
typedef char                s8;
typedef short               s16;
typedef int                 s32;
typedef long                sw;
typedef long long           s64;
typedef unsigned char       u8;
typedef unsigned short      u16;
typedef unsigned int        u32;
typedef unsigned long       uw;
typedef unsigned long long  u64;

/* IEEE 754 floating point numbers. */
typedef float               f32;
typedef double              f64;

/* Boolean : true or false. */
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
  u64   u64;
} u_ptr;

typedef union ptr_w {
  void *p;
  s8   *ps8;
  u8   *pu8;
  u64   u64;
} u_ptr_w;

typedef struct str {
  u_ptr_w free;        /**< Pointer to free or NULL. */
  u64     size;        /**< Size in bytes. */
  u_ptr   ptr;         /**< Pointer to memory. */
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

typedef struct ident {
  const s_sym *sym;
} s_ident;

typedef struct buf s_buf;

struct buf {
  bool free;
  u64 size;
  u_ptr_w ptr;
  u64 rpos;
  u64 wpos;
  sw (*flush) (s_buf *buf);
  sw (*refill) (s_buf *buf);
  FILE *fp;
};

typedef struct tag      s_tag;
typedef struct list     s_list;

struct list {
  s_tag *tag;
  s_list *next;
};

typedef struct tuple {
  u64 count;
  s_tag *ptr;
} s_tuple;

typedef enum tag_type {
  TAG_VOID = 0,
  TAG_BOOL = 1,
  TAG_CHARACTER,
  TAG_F32,
  TAG_F64,
  TAG_LIST,
  TAG_S8,
  TAG_S16,
  TAG_S32,
  TAG_S64,
  TAG_STR,
  TAG_SYM,
  TAG_TUPLE,
  TAG_U8,
  TAG_U16,
  TAG_U32,
  TAG_U64
} e_tag_type;

typedef union tag_type_ {
  e_tag_type type;
  u64 u64;
} u_tag_type;

typedef union tag_data {
  bool bool;
  character character;
  f32 f32;
  f64 f64;
  s_list list;
  s_str str;
  const s_sym *sym;
  s8 s8;
  s16 s16;
  s32 s32;
  s64 s64;
  s_tuple tuple;
  u8 u8;
  u16 u16;
  u32 u32;
  u64 u64;
} u_tag_data;

struct tag {
  u_tag_type type;
  u_tag_data data;
};

#endif
