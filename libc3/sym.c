/* c3
 * Copyright 2022 Thomas de Grivel <thoxdg@gmail.com>
 */
#include <err.h>
#include <stdlib.h>
#include "character.h"
#include "str.h"
#include "sym.h"

s_sym_list * sym_list_new (s_sym *sym, s_sym_list *next);
s_sym      * sym_new (s_str *str);

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
  if (sym_is_module(sym))
    return str_dup(&sym->str);
  str_init(&colon, false, 1, ":");
  return str_append(str_empty(), &colon, &sym->str);
}

bool sym_is_module (s_sym *sym)
{
  character c;
  c = str_to_character(&sym->str);
  if (character_is_upper(c))
    return true;
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
