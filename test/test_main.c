#include "test.h"

void types_test ();

int main (int argc, char **argv)
{
  (void) argc;
  (void) argv;
  test_init();
  types_test();
  test_summary();
  test_shutdown();
  return 0;
}
