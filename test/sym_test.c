/* c3
 * Copyright 2022 Thomas de Grivel <thoxdg@gmail.com>
 */
#include <assert.h>
#include <stdlib.h>
#include <string.h>
#include "../libc3/str.h"
#include "../libc3/sym.h"
#include "test.h"

s_sym * sym_test_sym_1_test(const s8 *p)
{
  uw len;
  s_sym *sym;
  len = strlen(p);
  TEST_ASSERT(sym = sym_1(p));
  TEST_EQ(sym->str.bytes, len);
  TEST_EQ(strncmp(p, sym->str.ptr.p, len), 0);
  return sym;
}

void sym_test_sym_1 ()
{
  s_sym *mod;
  s_sym *mod_t;
  s_sym *mod_test;
  s_sym *mod_test123;
  s_sym *sym;
  s_sym *sym_123;
  s_sym *sym_empty;
  s_sym *sym_t;
  s_sym *sym_test;
  s_sym *sym_test123;
  sym_empty = sym_test_sym_1_test("");
  sym_test_sym_1_test(" ");
  sym_test_sym_1_test(".");
  sym_test_sym_1_test("..");
  sym_test_sym_1_test("...");
  sym_t = sym_test_sym_1_test("t");
  TEST_ASSERT(sym_t != sym_empty);
  mod_t = sym_test_sym_1_test("T");
  TEST_ASSERT(mod_t != sym_empty);
  TEST_ASSERT(mod_t != sym_t);
  sym_test = sym_test_sym_1_test("test");
  mod_test = sym_test_sym_1_test("Test");
  TEST_ASSERT(mod_test != sym_empty);
  TEST_ASSERT(mod_test != sym_t);
  TEST_ASSERT(mod_test != mod_t);
  TEST_ASSERT(mod_test != sym_test);
  sym_123 = sym_test_sym_1_test("123");
  sym_test123 = sym_test_sym_1_test("test123");
  mod_test123 = sym_test_sym_1_test("Test123");
  TEST_ASSERT(sym_test123 != mod_test123);
  sym = sym_test_sym_1_test("123.456");
  TEST_ASSERT(sym != sym_empty);
  TEST_ASSERT(sym != sym_123);
  sym = sym_test_sym_1_test("test123.test456");
  TEST_ASSERT(sym != sym_test123);
  mod = sym_test_sym_1_test("Test123.Test456");
  TEST_ASSERT(mod != mod_test123);
  TEST_ASSERT(mod != sym);
  sym_test_sym_1_test("É");
  sym_test_sym_1_test("Éo");
  sym_test_sym_1_test("Éoà \n\r\t\v\"");
  sym_test_sym_1_test("é");
  sym_test_sym_1_test("éo");
  sym_test_sym_1_test("éoà \n\r\t\v\"");
}

void sym_test_sym_delete_all ()
{
  sym_delete_all();
  test_ok(); /* sym_delete_all */
}

void sym_test_sym_inspect_test (const char *test, const char *result)
{
  size_t len;
  s_sym *sym;
  s_str *str;
  assert(test);
  assert(result);
  sym = sym_1(test);
  TEST_ASSERT(str = sym_inspect(sym));
  len = strlen(result);
  TEST_EQ(str->bytes, len);
  TEST_EQ(strncmp(result, str->ptr.p, len), 0);
  str_delete(str);
}

#define TEST_STR_CMP(str, result)                         \
  do {                                                    \
    size_t len;                                           \
    len = strlen(result);                                 \
    TEST_EQ(str->bytes, len);                             \
    TEST_ASSERT(strncmp(result, str->ptr.p, len) == 0);   \
    if (!g_test_last_ok) {                                \
      printf("%sExpected \"%s\" got \"",                  \
             TEST_COLOR_KO,                               \
             result);                                     \
      fwrite(str->ptr.p, str->bytes, 1, stdout);          \
      printf("\".%s\n", TEST_COLOR_RESET);                \
    }                                                     \
  } while (0)

#define SYM_TEST_SYM_INSPECT(test, result)                \
  do {                                                    \
    s_sym *sym;                                           \
    s_str *str;                                           \
    assert(test);                                         \
    assert(result);                                       \
    test_context("sym_inspect(" #test ") -> " #result);   \
    sym = sym_1(test);                                    \
    TEST_ASSERT(str = sym_inspect(sym));                  \
    TEST_STR_CMP(str, result);                            \
    str_delete(str);                                      \
    test_context(NULL);                                   \
  } while (0)
  
void sym_test_sym_inspect ()
{
  SYM_TEST_SYM_INSPECT("", ":\"\"");
  SYM_TEST_SYM_INSPECT(" ", ":\"\\s\"");
  SYM_TEST_SYM_INSPECT(".", ":.");
  SYM_TEST_SYM_INSPECT("..", ":..");
  SYM_TEST_SYM_INSPECT("...", ":...");
  SYM_TEST_SYM_INSPECT("t", ":t");
  SYM_TEST_SYM_INSPECT("T", "T");
  SYM_TEST_SYM_INSPECT("test", ":test");
  SYM_TEST_SYM_INSPECT("Test", "Test");
  SYM_TEST_SYM_INSPECT("123", ":123");
  SYM_TEST_SYM_INSPECT("test123", ":test123");
  SYM_TEST_SYM_INSPECT("Test123", "Test123");
  SYM_TEST_SYM_INSPECT("test123.test456", ":test123.test456");
  SYM_TEST_SYM_INSPECT("Test123.Test456", "Test123.Test456");
  SYM_TEST_SYM_INSPECT("É", ":\"É\"");
  SYM_TEST_SYM_INSPECT("Éo", ":\"Éo\"");
  SYM_TEST_SYM_INSPECT("Éoà \n\r\t\v\"",
                       ":\"Éoà\\s\\n\\r\\t\\v\\\"\"");
  SYM_TEST_SYM_INSPECT("é", ":\"é\"");
  SYM_TEST_SYM_INSPECT("éo", ":\"éo\"");
  SYM_TEST_SYM_INSPECT("éoà \n\r\t\v\"",
                       ":\"éoà\\s\\n\\r\\t\\v\\\"\"");
}

void sym_test ()
{
  sym_test_sym_delete_all();
  sym_test_sym_1();
  sym_test_sym_delete_all();
  sym_test_sym_inspect();
  sym_test_sym_delete_all();
}
