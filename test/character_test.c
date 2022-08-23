/* c3
 * Copyright 2022 Thomas de Grivel <thoxdg@gmail.com>
 */
#include <stdlib.h>
#include <string.h>
#include "../libc3/character.h"
#include "test.h"

void character_test_character_1 ();
void character_test_character_is_upper ();

void character_test ()
{
  character_test_character_1();
  character_test_character_is_upper();
}

#define CHARACTER_TEST_CHARACTER_1_ASCII(string)        \
  TEST_EQ(character_1(string), string[0])

void character_test_character_1 ()
{
  CHARACTER_TEST_CHARACTER_1_ASCII("0");
  CHARACTER_TEST_CHARACTER_1_ASCII("1");
  CHARACTER_TEST_CHARACTER_1_ASCII("2");
  CHARACTER_TEST_CHARACTER_1_ASCII("3");
  CHARACTER_TEST_CHARACTER_1_ASCII("4");
  CHARACTER_TEST_CHARACTER_1_ASCII("5");
  CHARACTER_TEST_CHARACTER_1_ASCII("6");
  CHARACTER_TEST_CHARACTER_1_ASCII("7");
  CHARACTER_TEST_CHARACTER_1_ASCII("8");
  CHARACTER_TEST_CHARACTER_1_ASCII("9");
  CHARACTER_TEST_CHARACTER_1_ASCII("A");
  CHARACTER_TEST_CHARACTER_1_ASCII("B");
  CHARACTER_TEST_CHARACTER_1_ASCII("C");
  CHARACTER_TEST_CHARACTER_1_ASCII("X");
  CHARACTER_TEST_CHARACTER_1_ASCII("Y");
  CHARACTER_TEST_CHARACTER_1_ASCII("Z");
  CHARACTER_TEST_CHARACTER_1_ASCII("a");
  CHARACTER_TEST_CHARACTER_1_ASCII("b");
  CHARACTER_TEST_CHARACTER_1_ASCII("c");
  CHARACTER_TEST_CHARACTER_1_ASCII("x");
  CHARACTER_TEST_CHARACTER_1_ASCII("y");
  CHARACTER_TEST_CHARACTER_1_ASCII("z");
  TEST_EQ(character_1("À"), 192);
  TEST_EQ(character_1("É"), 201);
  TEST_EQ(character_1("à"), 224);
  TEST_EQ(character_1("é"), 233);
}

void character_test_character_is_upper ()
{
  TEST_ASSERT(! character_is_upper('0'));
  TEST_ASSERT(! character_is_upper('1'));
  TEST_ASSERT(! character_is_upper('8'));
  TEST_ASSERT(! character_is_upper('9'));
  TEST_ASSERT(character_is_upper('A'));
  TEST_ASSERT(character_is_upper('B'));
  TEST_ASSERT(character_is_upper('C'));
  TEST_ASSERT(character_is_upper('X'));
  TEST_ASSERT(character_is_upper('Y'));
  TEST_ASSERT(character_is_upper('Z'));
  TEST_ASSERT(! character_is_upper('a'));
  TEST_ASSERT(! character_is_upper('b'));
  TEST_ASSERT(! character_is_upper('c'));
  TEST_ASSERT(! character_is_upper('x'));
  TEST_ASSERT(! character_is_upper('y'));
  TEST_ASSERT(! character_is_upper('z'));
  TEST_ASSERT(character_is_upper(character_1("À")));
  TEST_ASSERT(character_is_upper(character_1("É")));
  TEST_ASSERT(! character_is_upper(character_1("à")));
  TEST_ASSERT(! character_is_upper(character_1("é")));
}
