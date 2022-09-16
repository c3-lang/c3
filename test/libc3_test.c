/* c3
 * Copyright 2022 Thomas de Grivel <thoxdg@gmail.com>
 */
#include "../libc3/c3.h"
#include "test.h"

void bool_test ();
void buf_file_test ();
void buf_inspect_test ();
void buf_parse_test ();
void buf_test ();
void character_test ();
void ident_test ();
void list_test ();
void sha1_test ();
void str_test ();
void sym_test ();
void tuple_test ();
void types_test ();

int main (int argc, char **argv)
{
  test_init(argc, argv);
  libc3_init();
  if (test_target("types")) {
    printf("\ntypes\n");
    types_test();
  }
  if (test_target("bool")) {
    printf("\nbool\n");
    bool_test();
  }
  if (test_target("character")) {
    printf("\ncharacter\n");
    character_test();
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
  if (test_target("buf_file")) {
    printf("\nbuf_file\n");
    buf_file_test();
  }
  if (test_target("str")) {
    printf("\nstr\n");
    str_test();
  }
  if (test_target("sha1")) {
    printf("\nsha1\n");
    sha1_test();
  }
  if (test_target("sym")) {
    printf("\nsym\n");
    sym_test();
  }
  if (test_target("ident")) {
    printf("\nident\n");
    ident_test();
  }
  if (test_target("list")) {
    printf("\nlist\n");
    list_test();
  }
  if (test_target("tuple")) {
    printf("\ntuple\n");
    tuple_test();
  }
  test_summary();
  test_shutdown();
  libc3_shutdown();
  return 0;
}
