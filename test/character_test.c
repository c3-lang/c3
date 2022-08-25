/* c3
 * Copyright 2022 Thomas de Grivel <thoxdg@gmail.com>
 */
#include <stdlib.h>
#include <string.h>
#include "../libc3/character.h"
#include "test.h"

void character_test_character_1 ();
void character_test_character_is_uppercase ();
void character_test_character_utf8 ();
void character_test_character_utf8_bytes ();

void character_test ()
{
  character_test_character_1();
  character_test_character_is_uppercase();
  character_test_character_utf8_bytes();
  character_test_character_utf8();
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

void character_test_character_is_uppercase ()
{
  TEST_ASSERT(! character_is_uppercase('0'));
  TEST_ASSERT(! character_is_uppercase('1'));
  TEST_ASSERT(! character_is_uppercase('8'));
  TEST_ASSERT(! character_is_uppercase('9'));
  TEST_ASSERT(character_is_uppercase('A'));
  TEST_ASSERT(character_is_uppercase('B'));
  TEST_ASSERT(character_is_uppercase('C'));
  TEST_ASSERT(character_is_uppercase('X'));
  TEST_ASSERT(character_is_uppercase('Y'));
  TEST_ASSERT(character_is_uppercase('Z'));
  TEST_ASSERT(! character_is_uppercase('a'));
  TEST_ASSERT(! character_is_uppercase('b'));
  TEST_ASSERT(! character_is_uppercase('c'));
  TEST_ASSERT(! character_is_uppercase('x'));
  TEST_ASSERT(! character_is_uppercase('y'));
  TEST_ASSERT(! character_is_uppercase('z'));
  TEST_ASSERT(character_is_uppercase(character_1("À")));
  TEST_ASSERT(character_is_uppercase(character_1("É")));
  TEST_ASSERT(! character_is_uppercase(character_1("à")));
  TEST_ASSERT(! character_is_uppercase(character_1("é")));
}

void character_test_character_utf8 ()
{
}

void character_test_character_utf8_bytes ()
{
  TEST_EQ(character_utf8_bytes('0'), 1);
  TEST_EQ(character_utf8_bytes('1'), 1);
  TEST_EQ(character_utf8_bytes('2'), 1);
  TEST_EQ(character_utf8_bytes('7'), 1);
  TEST_EQ(character_utf8_bytes('8'), 1);
  TEST_EQ(character_utf8_bytes('9'), 1);
  TEST_EQ(character_utf8_bytes('A'), 1);
  TEST_EQ(character_utf8_bytes('B'), 1);
  TEST_EQ(character_utf8_bytes('C'), 1);
  TEST_EQ(character_utf8_bytes('X'), 1);
  TEST_EQ(character_utf8_bytes('Y'), 1);
  TEST_EQ(character_utf8_bytes('Z'), 1);
  TEST_EQ(character_utf8_bytes('a'), 1);
  TEST_EQ(character_utf8_bytes('b'), 1);
  TEST_EQ(character_utf8_bytes('c'), 1);
  TEST_EQ(character_utf8_bytes('x'), 1);
  TEST_EQ(character_utf8_bytes('y'), 1);
  TEST_EQ(character_utf8_bytes('z'), 1);
  TEST_EQ(character_utf8_bytes(character_1("À")), 2);
  TEST_EQ(character_utf8_bytes(character_1("É")), 2);
  TEST_EQ(character_utf8_bytes(character_1("à")), 2);
  TEST_EQ(character_utf8_bytes(character_1("é")), 2);
}
