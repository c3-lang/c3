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

#define TEST_EQ(test, expected)                         \
  do {                                                  \
    long long tmp = test;                               \
    if (tmp == (long long) expected) {                  \
      test_ok();                                        \
    }                                                   \
    else {                                              \
      test_ko();                                        \
      printf("\nAssertion failed in %s:%d %s\n"         \
             "%s == %s\nExpected %s got %lld",          \
             __FILE__, __LINE__, __func__,              \
             # test, # expected, # expected, tmp);      \
    }                                                   \
  } while (0)

extern long   g_test_count;
extern long   g_test_ko;
extern long   g_test_last_ok;
extern long   g_test_ok;
extern char **g_test_targets;

void test_context (const char *context);
void test_init (int argc, char **argv);
void test_ko ();
void test_ok ();
void test_shutdown ();
void test_summary ();
int  test_target (const char *target);

#endif /* TEST_H */
