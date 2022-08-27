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

void         sym_delete (s_sym *sym);
s_str *      sym_inspect_reserved (const s_sym *sym);
sw           sym_inspect_reserved_size (const s_sym *sym);
s_sym_list * sym_list_new (s_sym *sym, s_sym_list *next);
bool         sym_character_reserved (character c);

static s_sym_list * g_sym_list = NULL;

const s_sym * sym_1 (const s8 *p)
{
  const s_sym *found;
  s_str stra;
  s_sym *sym;
  str_init_1(&stra, false, (void *) p);
  found = sym_find(&stra);
  if (found)
    return found;
  sym = sym_new(&stra);
  if (! sym) {
    assert(false);
    return NULL;
  }
  g_sym_list = sym_list_new(sym, g_sym_list);
  return sym;
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

const s_sym * sym_find (const s_str *str)
{
  s_sym_list *sym_list;
  sym_list = g_sym_list;
  while (sym_list) {
    s_sym *sym = sym_list->sym;
    if (str_cmp(str, &sym->str) == 0)
      return sym;
    sym_list = sym_list->next.p;
  }
  return NULL;
}

bool sym_reserved_character (character c)
{
  return str_character_reserved(c);
}

bool sym_has_reserved_characters (const s_sym *sym)
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

bool sym_is_module (const s_sym *sym)
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

s_sym * sym_new (const s_str *src)
{
  s_sym *sym;
  sym = malloc(sizeof(s_sym));
  if (! sym)
    err(1, "out of memory");
  str_init_dup(&sym->str, src);
  return sym;
}
