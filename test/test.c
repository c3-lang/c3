/* test
 * Copyright 2022 Thomas de Grivel <thoxdg@gmail.com>
 */
#include "test.h"

long g_test_ok = 0;
long g_test_ko = 0;
long g_test_count = 0;

void test_ok ()
{
  g_test_ok++;
  fprintf(stderr, "%s.%s", TEST_COLOR_OK, TEST_COLOR_RESET);
}

void test_ko ()
{
  g_test_ko++;
  fprintf(stderr, "%sF%s", TEST_COLOR_KO, TEST_COLOR_RESET);
}

void test_summary ()
{
  printf("\nTotal %ld tests. "
         "%sOK %ld (%f%%)%s. "
         "%sKO %ld (%f%%)%s.\n",
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
