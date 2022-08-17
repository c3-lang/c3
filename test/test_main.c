#include "test.h"

void types_test ();
void str_test ();

int main (int argc, char **argv)
{
  test_init(argc - 1, argv + 1);
  if (test_target("types"))
    types_test();
  if (test_target("str"))
    str_test();
  test_summary();
  test_shutdown();
  return 0;
}
