/* test
 * Copyright 2022 Thomas de Grivel <thoxdg@gmail.com>
 */
#include <assert.h>
#include <err.h>
#include <stdlib.h>
#include <string.h>
#include "test.h"

#define TARGET_NAME_MAX 32
#define TARGETS_MAX     1000

const char  *g_test_context = NULL;
long         g_test_count = 0;
long         g_test_ko = 0;
long         g_test_last_ok = 0;
long         g_test_ok = 0;
const char  *g_test_targets_default[] = {NULL};
char         g_test_targets_env[TARGETS_MAX * TARGET_NAME_MAX];
char        *g_test_targets_env_v[TARGETS_MAX + 1];
const char **g_test_targets = {NULL};

void test_context (const char *context)
{
  g_test_context = context;
  /* printf("test_context(%s)\n", context); */
}

void test_init (int argc, char **argv)
{
  const char **t;
  if (argv[argc] != NULL)
    err(1, "argv[argc] != NULL");
  g_test_context = NULL;
  g_test_count = 0;
  g_test_ko = 0;
  g_test_last_ok = 0;
  g_test_ok = 0;
  g_test_targets = g_test_targets_default;
  if (argc > 1) {
    g_test_targets = (const char **) argv + 1;
  }
  else {
    const char *env_target;
    env_target = getenv("C3_TEST_TARGET");
    if (env_target) {
      char **ap;
      size_t len = 0;
      char *target;
      len = strlen(env_target);
      target = g_test_targets_env;
      if (len + 1 > sizeof(g_test_targets_env))
        err(1, "too many targets");
      memcpy(target, env_target, len + 1);
      ap = g_test_targets_env_v;
      while (ap < g_test_targets_env_v + TARGETS_MAX &&
             (*ap = strsep(&target, " \t")) != NULL)
        if (**ap != '\0')
          ap++;
      *ap = NULL;
      g_test_targets = (const char **) g_test_targets_env_v;
    }
  }
  printf("%s", argv[0]);
  t = g_test_targets;
  while (*t) {
    printf(" %s", *t);
    t++;
  }
  printf("\n");
}

void test_ko ()
{
  g_test_count++;
  g_test_ko++;
  fprintf(stderr, "%sF%s", TEST_COLOR_KO, TEST_COLOR_RESET);
  if (g_test_context)
    printf("\n%s%s%s", TEST_COLOR_KO, g_test_context,
           TEST_COLOR_RESET);
  g_test_last_ok = 0;
}

void test_ok ()
{
  g_test_count++;
  g_test_ok++;
  fprintf(stderr, "%s.%s", TEST_COLOR_OK, TEST_COLOR_RESET);
  g_test_last_ok = 1;
}

void test_shutdown ()
{
}

void test_summary ()
{
  fprintf(stderr, "\n");
  printf("Total %ld tests. "
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
  const char **i;
  i = g_test_targets;
  if (! *i)
    return 1;
  while (*i) {
    if (strcmp(target, *i) == 0)
      return 1;
    i++;
  }
  return 0;
}
