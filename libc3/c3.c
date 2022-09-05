/* c3
 * Copyright 2022 Thomas de Grivel
 */
#include <stdio.h>
#include <stdlib.h>
#include "c3.h"
#include "sym.h"

void libc3_init ()
{
}

void libc3_shutdown ()
{
  sym_delete_all();
}

