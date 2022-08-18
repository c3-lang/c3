/* c3
 * Copyright 2022 Thomas de Grivel <thoxdg@gmail.com>
 */
#include <err.h>
#include <stdlib.h>
#include "str.h"
#include "symbol.h"

s_symbol * symbol_new (s_str *str, s_symbol *next);

s_symbol * g_symbol = NULL;

s_symbol * symbol (s8 *p)
{
  s_str stra;
  s_symbol *sym;
  str_init_1(&stra, false, p);
  /* TODO: locking */
  sym = g_symbol;
  if (sym) {
    do {
      if (str_cmp(&sym->str, &stra) == 0)
        return sym;
    } while ((sym = sym->next.symbol));
  }
  sym = symbol_new(&stra, g_symbol);
  g_symbol = sym;
  /* TODO: unlocking */
  return sym;
}

sw symbol_cmp (s_symbol *a, s_symbol *b)
{
  if (a == b)
    return 0;
  if (a < b)
    return -1;
  return 1;
}

void symbol_delete (s_symbol *sym)
{
  str_clean(&sym->str);
}
  
void symbol_delete_all (s_symbol *sym)
{
  while (sym) {
    symbol_delete(sym);
    sym = sym->next.symbol;
  }
}

s_symbol * symbol_new (s_str *str, s_symbol *next)
{
  s_symbol *sym;
  sym = malloc(sizeof(s_symbol));
  if (! sym)
    err(1, "out of memory");
  str_init_dup(&sym->str, str);
  sym->next.symbol = next;
  return sym;
}
