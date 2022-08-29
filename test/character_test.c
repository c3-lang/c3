/* c3
 * Copyright 2022 Thomas de Grivel <thoxdg@gmail.com>
 */
#include <stdlib.h>
#include <string.h>
#include "../libc3/character.h"
#include "test.h"

void character_test_character_1 ();
void character_test_character_is_lowercase ();
void character_test_character_is_uppercase ();
void character_test_character_utf8 ();
void character_test_character_utf8_size ();

void character_test ()
{
  character_test_character_1();
  character_test_character_is_lowercase();
  character_test_character_is_uppercase();
  character_test_character_utf8_size();
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

void character_test_character_is_lowercase ()
{
  character c;
  for (c = -10; c < 40; c++)
    TEST_ASSERT(! character_is_lowercase(c));
  TEST_ASSERT(! character_is_lowercase('0'));
  TEST_ASSERT(! character_is_lowercase('0'));
  TEST_ASSERT(! character_is_lowercase('0'));
  TEST_ASSERT(! character_is_lowercase('1'));
  TEST_ASSERT(! character_is_lowercase('8'));
  TEST_ASSERT(! character_is_lowercase('9'));
  TEST_ASSERT(! character_is_lowercase('A'));
  TEST_ASSERT(! character_is_lowercase('B'));
  TEST_ASSERT(! character_is_lowercase('C'));
  TEST_ASSERT(! character_is_lowercase('X'));
  TEST_ASSERT(! character_is_lowercase('Y'));
  TEST_ASSERT(! character_is_lowercase('Z'));
  TEST_ASSERT(! character_is_lowercase(character_1("À")));
  TEST_ASSERT(! character_is_lowercase(character_1("É")));
  TEST_ASSERT(character_is_lowercase('a'));
  TEST_ASSERT(character_is_lowercase('b'));
  TEST_ASSERT(character_is_lowercase('c'));
  TEST_ASSERT(character_is_lowercase('x'));
  TEST_ASSERT(character_is_lowercase('y'));
  TEST_ASSERT(character_is_lowercase('z'));
  TEST_ASSERT(character_is_lowercase(character_1("à")));
  TEST_ASSERT(character_is_lowercase(character_1("é")));
}

void character_test_character_is_uppercase ()
{
  character c;
  for (c = -10; c < 40; c++)
    TEST_ASSERT(! character_is_uppercase(c));
  TEST_ASSERT(! character_is_uppercase('0'));
  TEST_ASSERT(! character_is_uppercase('1'));
  TEST_ASSERT(! character_is_uppercase('8'));
  TEST_ASSERT(! character_is_uppercase('9'));
  TEST_ASSERT(! character_is_uppercase('a'));
  TEST_ASSERT(! character_is_uppercase('b'));
  TEST_ASSERT(! character_is_uppercase('c'));
  TEST_ASSERT(! character_is_uppercase('x'));
  TEST_ASSERT(! character_is_uppercase('y'));
  TEST_ASSERT(! character_is_uppercase('z'));
  TEST_ASSERT(! character_is_uppercase(character_1("à")));
  TEST_ASSERT(! character_is_uppercase(character_1("é")));
  TEST_ASSERT(character_is_uppercase('A'));
  TEST_ASSERT(character_is_uppercase('B'));
  TEST_ASSERT(character_is_uppercase('C'));
  TEST_ASSERT(character_is_uppercase('X'));
  TEST_ASSERT(character_is_uppercase('Y'));
  TEST_ASSERT(character_is_uppercase('Z'));
  TEST_ASSERT(character_is_uppercase(character_1("À")));
  TEST_ASSERT(character_is_uppercase(character_1("É")));
}

void character_test_character_utf8 ()
{
}

void character_test_character_utf8_size ()
{
  character c;
  for (c = -10; c < 0; c++)
    TEST_EQ(character_utf8_size(c), -1);
  TEST_EQ(character_utf8_size('0'), 1);
  TEST_EQ(character_utf8_size('1'), 1);
  TEST_EQ(character_utf8_size('2'), 1);
  TEST_EQ(character_utf8_size('7'), 1);
  TEST_EQ(character_utf8_size('8'), 1);
  TEST_EQ(character_utf8_size('9'), 1);
  TEST_EQ(character_utf8_size('A'), 1);
  TEST_EQ(character_utf8_size('B'), 1);
  TEST_EQ(character_utf8_size('C'), 1);
  TEST_EQ(character_utf8_size('X'), 1);
  TEST_EQ(character_utf8_size('Y'), 1);
  TEST_EQ(character_utf8_size('Z'), 1);
  TEST_EQ(character_utf8_size('a'), 1);
  TEST_EQ(character_utf8_size('b'), 1);
  TEST_EQ(character_utf8_size('c'), 1);
  TEST_EQ(character_utf8_size('x'), 1);
  TEST_EQ(character_utf8_size('y'), 1);
  TEST_EQ(character_utf8_size('z'), 1);
  TEST_EQ(character_utf8_size(character_1("À")), 2);
  TEST_EQ(character_utf8_size(character_1("É")), 2);
  TEST_EQ(character_utf8_size(character_1("à")), 2);
  TEST_EQ(character_utf8_size(character_1("é")), 2);
}
