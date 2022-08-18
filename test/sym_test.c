/* c3
 * Copyright 2022 Thomas de Grivel <thoxdg@gmail.com>
 */
#include <stdlib.h>
#include <string.h>
#include "../libc3/sym.h"
#include "test.h"

s_sym * sym_test_sym_1(const s8 *p)
{
  uw len;
  s_sym *sym;
  len = strlen(p);
  TEST_ASSERT(sym = sym_1(p));
  TEST_ASSERT(sym->str.bytes == len);
  TEST_ASSERT(strncmp(p, sym->str.ptr.p, len) == 0);
  return sym;
}

void sym_test ()
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
  sym_empty = sym_test_sym_1("");
  sym_t = sym_test_sym_1("t");
  TEST_ASSERT(sym_t != sym_empty);
  mod_t = sym_test_sym_1("T");
  TEST_ASSERT(mod_t != sym_empty);
  TEST_ASSERT(mod_t != sym_t);
  sym_test = sym_test_sym_1("test");
  mod_test = sym_test_sym_1("Test");
  TEST_ASSERT(mod_test != sym_empty);
  TEST_ASSERT(mod_test != sym_t);
  TEST_ASSERT(mod_test != mod_t);
  TEST_ASSERT(mod_test != sym_test);
  sym_123 = sym_test_sym_1("123");
  sym_test123 = sym_test_sym_1("test123");
  mod_test123 = sym_test_sym_1("Test123");
  TEST_ASSERT(sym_test123 != mod_test123);
  sym = sym_test_sym_1("123.456");
  TEST_ASSERT(sym != sym_empty);
  TEST_ASSERT(sym != sym_123);
  sym = sym_test_sym_1("test123.test456");
  TEST_ASSERT(sym != sym_test123);
  mod = sym_test_sym_1("Test123.Test456");
  TEST_ASSERT(mod != mod_test123);
  TEST_ASSERT(mod != sym);
  sym_delete_all();
  test_ok(); /* sym_delete_all */
}
