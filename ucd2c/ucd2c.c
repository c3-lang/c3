/* c3
 * Copyright 2022 Thomas de Grivel <thoxdg@gmail.com>
 */
#include <assert.h>
#include <err.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>

#define BUFSZ 1024

int main (int argc, char **argv)
{
  char buf[BUFSZ];
  (void) argc;
  (void) argv;
  while (fgets(buf, BUFSZ, stdin)) {
    printf("> %s", buf);
  }
  return 0;
}
