/* test
 * Copyright 2022 Thomas de Grivel <thoxdg@gmail.com>
 */
#ifndef TEST_H
#define TEST_H

#include <stdio.h>

#define TEST_COLOR_KO "\033[91m"
#define TEST_COLOR_OK "\033[92m"
#define TEST_COLOR_RESET "\033[39m"

#define TEST_ASSERT(test)                               \
  do {                                                  \
    if (test) {                                         \
      test_ok();                                        \
    }                                                   \
    else {                                              \
      test_ko();                                        \
      printf("\nAssertion failed in %s:%d %s\n%s\n",    \
             __FILE__, __LINE__, __func__, # test);     \
    }                                                   \
  } while(0)

#define TEST_EQ(test, expected)                         \
  do {                                                  \
    long long signed tmp = (long long signed) (test);   \
    if (tmp == (long long signed) (expected)) {         \
      test_ok();                                        \
    }                                                   \
    else {                                              \
      test_ko();                                        \
      printf("\n%sAssertion failed in %s:%d %s\n"       \
             "%s == %s\n"                               \
             "Expected %s got %lld.%s\n",               \
             TEST_COLOR_KO,                             \
             __FILE__, __LINE__, __func__,              \
             # test, # expected, # expected, tmp,       \
             TEST_COLOR_RESET);                         \
    }                                                   \
  } while (0)

#define TEST_FLOAT_EQ(test, expected)                   \
  do {                                                  \
    long double tmp = (long double) (test);             \
    if (tmp == (long double) (expected)) {              \
      test_ok();                                        \
    }                                                   \
    else {                                              \
      test_ko();                                        \
      printf("\n%sAssertion failed in %s:%d %s\n"       \
             "%s == %s\n"                               \
             "Expected %s got %Lf.%s\n",                \
             TEST_COLOR_KO,                             \
             __FILE__, __LINE__, __func__,              \
             # test, # expected, # expected, tmp,       \
             TEST_COLOR_RESET);                         \
    }                                                   \
  } while (0)

#define TEST_STRNCMP(test, result, bytes)               \
  do {                                                  \
    const char *tmp = test;                             \
    if (strncmp(tmp, result, bytes) == 0) {             \
      test_ok();                                        \
    }                                                   \
    else {                                              \
      test_ko();                                        \
      printf("\n%sAssertion failed in %s:%d %s\n"       \
             "strncmp(%s, %s, %ld) == 0\n",             \
             TEST_COLOR_KO,                             \
             __FILE__, __LINE__, __func__,              \
             # test, # result, (long) bytes);           \
      printf("Expected %s got \"",                      \
             # result);                                 \
      fwrite(tmp, bytes, 1, stdout);                    \
      printf("\".%s\n", TEST_COLOR_RESET);              \
    }                                                   \
  } while (0)

extern long         g_test_count;
extern long         g_test_ko;
extern long         g_test_last_ok;
extern long         g_test_ok;
extern const char **g_test_targets;

void test_context (const char *context);
void test_init (int argc, char **argv);
void test_ko ();
void test_ok ();
void test_shutdown ();
void test_summary ();
int  test_target (const char *target);

#endif /* TEST_H */
