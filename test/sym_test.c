/* c3
 * Copyright 2022 Thomas de Grivel <thoxdg@gmail.com>
 */
#include <assert.h>
#include <stdlib.h>
#include <string.h>
#include "../libc3/str.h"
#include "../libc3/sym.h"
#include "test.h"

const s_sym * sym_test_1_test (const s8 *p)
{
  uw len;
  const s_sym *sym;
  len = strlen(p);
  TEST_ASSERT((sym = sym_1(p)));
  TEST_EQ(sym->str.size, len);
  TEST_EQ(strncmp(p, sym->str.ptr.p, len), 0);
  TEST_EQ(sym_1(p), sym_1(p));
  return sym;
}

void sym_test_1 ()
{
  const s_sym *mod;
  const s_sym *mod_t;
  const s_sym *mod_test;
  const s_sym *mod_test123;
  const s_sym *sym;
  const s_sym *sym_123;
  const s_sym *sym_empty;
  const s_sym *sym_t;
  const s_sym *sym_test;
  const s_sym *sym_test123;
  sym_empty = sym_test_1_test("");
  TEST_EQ(sym_test_1_test(""), sym_empty);
  sym_test_1_test(" ");
  sym_test_1_test(".");
  sym_test_1_test("..");
  sym_test_1_test("...");
  sym_t = sym_test_1_test("t");
  TEST_ASSERT(sym_t != sym_empty);
  mod_t = sym_test_1_test("T");
  TEST_ASSERT(mod_t != sym_empty);
  TEST_ASSERT(mod_t != sym_t);
  sym_test = sym_test_1_test("test");
  mod_test = sym_test_1_test("Test");
  TEST_ASSERT(mod_test != sym_empty);
  TEST_ASSERT(mod_test != sym_t);
  TEST_ASSERT(mod_test != mod_t);
  TEST_ASSERT(mod_test != sym_test);
  sym_123 = sym_test_1_test("123");
  sym_test123 = sym_test_1_test("test123");
  mod_test123 = sym_test_1_test("Test123");
  TEST_ASSERT(sym_test123 != mod_test123);
  sym = sym_test_1_test("123.456");
  TEST_ASSERT(sym != sym_empty);
  TEST_ASSERT(sym != sym_123);
  sym = sym_test_1_test("test123.test456");
  TEST_ASSERT(sym != sym_test123);
  mod = sym_test_1_test("Test123.Test456");
  TEST_ASSERT(mod != mod_test123);
  TEST_ASSERT(mod != sym);
  sym_test_1_test("É");
  sym_test_1_test("Éo");
  sym_test_1_test("Éoà \n\r\t\v\"");
  sym_test_1_test("é");
  sym_test_1_test("éo");
  sym_test_1_test("éoà \n\r\t\v\"");
}

void sym_test_delete_all ()
{
  sym_delete_all();
  test_ok();
}

#define SYM_TEST_INSPECT(test, result)                  \
  do {                                                  \
    const s_sym *sym;                                   \
    s_str *str;                                         \
    assert(test);                                       \
    assert(result);                                     \
    test_context("sym_inspect(" #test ") -> " #result); \
    sym = sym_1(test);                                  \
    TEST_ASSERT((str = sym_inspect(sym)));              \
    TEST_STRNCMP(str->ptr.p, result, str->size);        \
    str_delete(str);                                    \
    test_context(NULL);                                 \
  } while (0)
  
void sym_test_inspect ()
{
  SYM_TEST_INSPECT("", ":\"\"");
  SYM_TEST_INSPECT(" ", ":\" \"");
  SYM_TEST_INSPECT(".", ":.");
  SYM_TEST_INSPECT("..", ":..");
  SYM_TEST_INSPECT("...", ":...");
  SYM_TEST_INSPECT(".. .", ":\".. .\"");
  SYM_TEST_INSPECT("t", ":t");
  SYM_TEST_INSPECT("T", "T");
  SYM_TEST_INSPECT("test", ":test");
  SYM_TEST_INSPECT("Test", "Test");
  SYM_TEST_INSPECT("123", ":123");
  SYM_TEST_INSPECT("test123", ":test123");
  SYM_TEST_INSPECT("Test123", "Test123");
  SYM_TEST_INSPECT("test 123", ":\"test 123\"");
  SYM_TEST_INSPECT("Test 123", ":\"Test 123\"");
  SYM_TEST_INSPECT("test123.test456", ":test123.test456");
  SYM_TEST_INSPECT("Test123.Test456", "Test123.Test456");
  SYM_TEST_INSPECT("É", "É");
  SYM_TEST_INSPECT("Éo", "Éo");
  SYM_TEST_INSPECT("Éoà \n\r\t\v\"",
                   ":\"Éoà \\n\\r\\t\\v\\\"\"");
  SYM_TEST_INSPECT("é", ":é");
  SYM_TEST_INSPECT("éo", ":éo");
  SYM_TEST_INSPECT("éoà \n\r\t\v\"",
                   ":\"éoà \\n\\r\\t\\v\\\"\"");
}

void sym_test ()
{
  sym_test_delete_all();
  sym_test_1();
  sym_test_delete_all();
  sym_test_inspect();
  sym_test_delete_all();
}
