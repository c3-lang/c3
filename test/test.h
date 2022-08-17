/* test
 * Copyright 2022 Thomas de Grivel <thoxdg@gmail.com>
 */
#ifndef TEST_H
#define TEST_H

#include <stdio.h>

#define TEST_COLOR_KO "\033[91m"
#define TEST_COLOR_OK "\033[92m"
#define TEST_COLOR_RESET "\033[39m"

#define TEST_ASSERT(pred)                               \
  do {                                                  \
    if ((pred)) {                                       \
      test_ok();                                        \
    }                                                   \
    else {                                              \
      test_ko();                                        \
      printf("\nAssertion failed in %s:%d %s\n%s\n",    \
             __FILE__, __LINE__, __func__, # pred);     \
    }                                                   \
  } while(0)

extern char **g_test_targets;
extern long   g_test_ok;
extern long   g_test_ko;
extern long   g_test_count;

void test_init (int argc, char **argv);
void test_ok ();
void test_ko ();
void test_shutdown ();
void test_summary ();
int  test_target (const char *target);

#endif /* TEST_H */
