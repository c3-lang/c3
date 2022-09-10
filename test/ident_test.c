/* c3
 * Copyright 2022 Thomas de Grivel <thoxdg@gmail.com>
 */
#include <assert.h>
#include <stdlib.h>
#include <string.h>
#include "../libc3/ident.h"
#include "../libc3/str.h"
#include "test.h"

#define IDENT_TEST_INSPECT(test, result)                   \
  do {                                                     \
    s_ident ident;                                         \
    s_str str;                                             \
    assert(test);                                          \
    assert(result);                                        \
    test_context("ident_inspect(" #test ") -> " #result);  \
    ident_init_1(&ident, test);                            \
    TEST_EQ(ident_inspect(&ident, &str), &str);            \
    TEST_STRNCMP(str.ptr.p, (result), str.size);           \
    str_clean(&str);                                       \
    test_context(NULL);                                    \
  } while (0)

void ident_test ();
void ident_test_inspect ();

void ident_test ()
{
  ident_test_inspect();
}

void ident_test_inspect ()
{
  IDENT_TEST_INSPECT("", "_\"\"");
  IDENT_TEST_INSPECT(" ", "_\" \"");
  IDENT_TEST_INSPECT("\n", "_\"\\n\"");
  IDENT_TEST_INSPECT("\r", "_\"\\r\"");
  IDENT_TEST_INSPECT("\t", "_\"\\t\"");
  IDENT_TEST_INSPECT("\v", "_\"\\v\"");
  IDENT_TEST_INSPECT("\"", "_\"\\\"\"");
  IDENT_TEST_INSPECT(".", "_\".\"");
  IDENT_TEST_INSPECT("..", "_\"..\"");
  IDENT_TEST_INSPECT("...", "_\"...\"");
  IDENT_TEST_INSPECT(".. .", "_\".. .\"");
  IDENT_TEST_INSPECT("t", "t");
  IDENT_TEST_INSPECT("T", "_\"T\"");
  IDENT_TEST_INSPECT("test", "test");
  IDENT_TEST_INSPECT("Test", "_\"Test\"");
  IDENT_TEST_INSPECT("123", "_\"123\"");
  IDENT_TEST_INSPECT("test123", "test123");
  IDENT_TEST_INSPECT("Test123", "_\"Test123\"");
  IDENT_TEST_INSPECT("test 123", "_\"test 123\"");
  IDENT_TEST_INSPECT("Test 123", "_\"Test 123\"");
  IDENT_TEST_INSPECT("test123.test456", "_\"test123.test456\"");
  IDENT_TEST_INSPECT("Test123.Test456", "_\"Test123.Test456\"");
  IDENT_TEST_INSPECT("test123(test456)", "_\"test123(test456)\"");
  IDENT_TEST_INSPECT("Test123(Test456)", "_\"Test123(Test456)\"");
  IDENT_TEST_INSPECT("test123{test456}", "_\"test123{test456}\"");
  IDENT_TEST_INSPECT("Test123{Test456}", "_\"Test123{Test456}\"");
  IDENT_TEST_INSPECT("É", "_\"É\"");
  IDENT_TEST_INSPECT("Éo", "_\"Éo\"");
  IDENT_TEST_INSPECT("Éoà \n\r\t\v\"",
                     "_\"Éoà \\n\\r\\t\\v\\\"\"");
  IDENT_TEST_INSPECT("éoà \n\r\t\v\"",
                     "_\"éoà \\n\\r\\t\\v\\\"\"");
  IDENT_TEST_INSPECT("é", "é");
  IDENT_TEST_INSPECT("éoπꝝ꒴", "éoπꝝ꒴");
}
