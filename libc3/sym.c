/* c3
 * Copyright 2022 Thomas de Grivel <thoxdg@gmail.com>
 */
#include <assert.h>
#include <err.h>
#include <stdlib.h>
#include "buf.h"
#include "character.h"
#include "str.h"
#include "sym.h"

character    sym_character_escape (character c);
bool         sym_character_reserved (character c);
bool         sym_has_reserved_characters (s_sym *sym);
s_str *      sym_inspect_reserved (s_sym *sym);
sw           sym_inspect_reserved_size (s_sym *sym);
s_sym_list * sym_list_new (s_sym *sym, s_sym_list *next);
s_sym *      sym_new (s_str *str);

s_sym_list * g_sym_list = NULL;

s_sym * sym_1 (const s8 *p)
{
  s_str stra;
  s_sym *sym;
  s_sym_list *sym_list;
  str_init_1(&stra, false, p);
  /* TODO: locking */
  sym_list = g_sym_list;
  if (sym_list) {
    do {
      if (str_cmp(&stra, &sym_list->sym->str) == 0)
        return sym_list->sym;
    } while ((sym_list = sym_list->next.p));
  }
  sym = sym_new(&stra);
  g_sym_list = sym_list_new(sym, g_sym_list);
  /* TODO: unlocking */
  return sym;
}

character sym_character_escape (character c)
{
  switch (c) {
  case '\0': return '0';
  case '\n': return 'n';
  case '\r': return 'r';
  case '\t': return 't';
  case '\v': return 'v';
  case '\"': return '\"';
  case ' ': return 's';
  default: return -1;
  }
}

bool sym_character_reserved (character c)
{
  return (! (character_is_digit(c) ||
             character_is_uppercase(c) ||
             character_is_lowercase(c) ||
             c == '.' ||
             c == '_'));
}

bool sym_has_reserved_characters (s_sym *sym)
{
  character c;
  s_str stra;
  str_init(&stra, false, sym->str.size, sym->str.ptr.p);
  while (str_read_character(&stra, &c) > 0) {
    if (sym_character_reserved(c))
      return true;
  }
  return false;
}

void sym_delete (s_sym *sym)
{
  str_clean(&sym->str);
  free(sym);
}
  
void sym_delete_all ()
{
  s_sym_list *sym_list;
  sym_list = g_sym_list;
  while (sym_list) {
    s_sym_list *tmp;
    tmp = sym_list;
    sym_list = sym_list->next.p;
    sym_delete(tmp->sym);
    free(tmp);
  }
  g_sym_list = NULL;
}

s_str * sym_inspect (s_sym *sym)
{
  s_str colon;
  s_str *str;
  assert(sym);
  if (sym->str.size == 0)
    return str_new_1(false, ":\"\"");
  if (sym_has_reserved_characters(sym))
    return sym_inspect_reserved(sym);
  if (sym_is_module(sym))
    return str_new_dup(&sym->str);
  str_init(&colon, false, 1, ":");
  str = str_new_join(2, &colon, &sym->str);
  return str;
}

s_str * sym_inspect_reserved (s_sym *sym)
{
  character c;
  sw csize;
  character esc;
  sw  size;
  s_buf buf;
  s_str *str;
  s_str stra;
  size = sym_inspect_reserved_size(sym);
  buf_init_alloc(&buf, size);
  buf_write(&buf, ':');
  buf_write(&buf, '"');
  /* XXX keep in sync with sym_inspect_reserved_size */
  str_init(&stra, false, sym->str.size, sym->str.ptr.p);
  while (str_read_character(&stra, &c) > 0) {
    if ((esc = sym_character_escape(c)) > 0) {
      buf_write(&buf, '\\');
      if ((csize = character_utf8(esc, buf.ptr.ps8 + buf.wpos)) < 0)
        goto error;
      buf.wpos += csize;
    }
    else {
      if ((csize = character_utf8(c, buf.ptr.ps8 + buf.wpos)) < 0)
        goto error;
      buf.wpos += csize;
    }
  }
  buf_write(&buf, '"');
  assert(buf.wpos == (uw) size);
  str = str_new(true, size, buf.ptr.p);
  return str;
 error:
  fprintf(stderr, "sym_inspect_reserved: character_utf8\n");
  buf_clean(&buf);
  return NULL;
}

sw sym_inspect_reserved_size (s_sym *sym)
{
  character c;
  sw csize;
  character esc;
  uw size;
  s_str stra;
  size = 3;
  /* XXX keep in sync with sym_inspect_reserved */
  str_init(&stra, false, sym->str.size, sym->str.ptr.p);
  while (str_read_character(&stra, &c) > 0) {
    if ((esc = sym_character_escape(c)) > 0) {
      size++;
      if ((csize = character_utf8_size(esc)) < 0)
        return -1;
      size += csize;
    }
    else {
      if ((csize = character_utf8_size(c)) < 0)
        return -1;
      size += csize;
    }
  }
  return size;
}

bool sym_is_module (s_sym *sym)
{
  character c;
  if (str_to_character(&sym->str, &c) > 0)
    return character_is_uppercase(c);
  return false;
}

s_sym_list * sym_list_new (s_sym *sym, s_sym_list *next)
{
  s_sym_list *sym_list;
  sym_list = malloc(sizeof(s_sym_list));
  if (! sym_list)
    err(1, "out of memory");
  sym_list->sym = sym;
  sym_list->next.p = next;
  return sym_list;
}

s_sym * sym_new (s_str *src)
{
  s_sym *sym;
  sym = malloc(sizeof(s_sym));
  if (! sym)
    err(1, "out of memory");
  str_init_dup(&sym->str, src);
  return sym;
}
