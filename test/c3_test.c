/* c3
 * Copyright 2022 Thomas de Grivel <thoxdg@gmail.com>
 */
#include "test.h"

void types_test ();
void character_test ();
void str_test ();
void sym_test ();
void buf_test ();
void buf_inspect_test ();
void buf_parse_test ();
void sha1_test ();

int main (int argc, char **argv)
{
  test_init(argc, argv);
  if (test_target("types")) {
    printf("\ntypes\n");
    types_test();
  }
  if (test_target("character")) {
    printf("\ncharacter\n");
    character_test();
  }
  if (test_target("str")) {
    printf("\nstr\n");
    str_test();
  }
  if (test_target("sym")) {
    printf("\nsym\n");
    sym_test();
  }
  if (test_target("buf")) {
    printf("\nbuf\n");
    buf_test();
  }
  if (test_target("buf_inspect")) {
    printf("\nbuf_inspect\n");
    buf_inspect_test();
  }
  if (test_target("buf_parse")) {
    printf("\nbuf_parse\n");
    buf_parse_test();
  }
  if (test_target("sha1")) {
    printf("\nsha1\n");
    sha1_test();
  }
  test_summary();
  test_shutdown();
  return 0;
}
