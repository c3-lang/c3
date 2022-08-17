/* test
 * Copyright 2022 Thomas de Grivel <thoxdg@gmail.com>
 */
#include <err.h>
#include <string.h>
#include "test.h"

char **g_test_targets = {NULL};
long   g_test_ok = 0;
long   g_test_ko = 0;
long   g_test_count = 0;

void test_init (int argc, char **argv)
{
  if (argv[argc] != NULL)
    err(1, "argv[argc] != NULL");
  g_test_targets = argv;
  g_test_ok = 0;
  g_test_ko = 0;
  g_test_count = 0;
}

void test_ok ()
{
  g_test_count++;
  g_test_ok++;
  fprintf(stderr, "%s.%s", TEST_COLOR_OK, TEST_COLOR_RESET);
}

void test_ko ()
{
  g_test_count++;
  g_test_ko++;
  fprintf(stderr, "%sF%s", TEST_COLOR_KO, TEST_COLOR_RESET);
}

void test_shutdown ()
{
}

void test_summary ()
{
  printf("\nTotal %ld tests. "
         "%sOK %ld (%.1f%%)%s. "
         "%sKO %ld (%.1f%%)%s.\n",
         g_test_count,
         TEST_COLOR_OK,
         g_test_ok,
         g_test_ok * 100.0f / g_test_count,
         TEST_COLOR_RESET,
         TEST_COLOR_KO,
         g_test_ko,
         g_test_ko * 100.0f / g_test_count,
         TEST_COLOR_RESET);
}

int test_target (const char *target)
{
  char **i;
  i = g_test_targets;
  if (!*i)
    return 1;
  while (*i) {
    if (strcmp(target, *i) == 0)
      return 1;
    i++;
  }
  return 0;
}
