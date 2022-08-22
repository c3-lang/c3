/* c3
 * Copyright 2022 Thomas de Grivel <thoxdg@gmail.com>
 */
#include "test.h"

void types_test ();
void character_test ();
void str_test ();
void sym_test ();
void buf_test ();

int main (int argc, char **argv)
{
  test_init(argc, argv);
  if (test_target("types"))
    types_test();
  if (test_target("character"))
    character_test();
  if (test_target("str"))
    str_test();
  if (test_target("sym"))
    sym_test();
  if (test_target("buf"))
    buf_test();
  test_summary();
  test_shutdown();
  return 0;
}
