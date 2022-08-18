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
  s_sym *sym;
  s_sym *mod;
  sym = sym_test_sym_1("");
  sym = sym_test_sym_1("t");
  mod = sym_test_sym_1("T");
  TEST_ASSERT(sym != mod);
  sym = sym_test_sym_1("test");
  mod = sym_test_sym_1("Test");
  TEST_ASSERT(sym != mod);
  sym = sym_test_sym_1("123");
  sym = sym_test_sym_1("test123");
  mod = sym_test_sym_1("Test123");
  TEST_ASSERT(sym != mod);
  sym = sym_test_sym_1("123.456");
  sym = sym_test_sym_1("test123.test456");
  mod = sym_test_sym_1("Test123.Test456");
  TEST_ASSERT(sym != mod);
  sym_delete_all();
  test_ok(); /* sym_delete_all */
}
