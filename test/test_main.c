#include "test.h"

void types_test ();
void str_test ();

int main (int argc, char **argv)
{
  (void) argc;
  (void) argv;
  test_init();
  types_test();
  str_test();
  test_summary();
  test_shutdown();
  return 0;
}
