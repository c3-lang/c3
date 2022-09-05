/* c3
 * Copyright 2022 Thomas de Grivel <thoxdg@gmail.com>
 */
#include "../libc3/c3.h"

int main (int argc, char **argv)
{
  libc3_init();
  (void) argc;
  (void) argv;
  libc3_shutdown();
  return 0;
}
