/* c3
 * Copyright 2022 Thomas de Grivel <thoxdg@gmail.com>
 */
#include <string.h>
#include "../libc3/str.h"
#include "../libc3/sha1.h"
#include "test.h"

#define SHA1_TEST_SHA1(test, result)                \
  do {                                              \
    s_str *hash;                                    \
    s_str stra;                                     \
    str_init_1(&stra, NULL, test);                \
    hash = sha1(&stra);                             \
    TEST_STRNCMP(hash->ptr.p, result, hash->size);  \
    str_delete(hash);                               \
  } while (0)

void sha1_test_sha1 ()
{
  SHA1_TEST_SHA1("abc", "\xa9\x99\x3e\x36\x47\x06\x81\x6a\xba\x3e\x25\x71\x78\x50\xc2\x6c\x9c\xd0\xd8\x9d");
  SHA1_TEST_SHA1("", "\xda\x39\xa3\xee\x5e\x6b\x4b\x0d\x32\x55\xbf\xef\x95\x60\x18\x90\xaf\xd8\x07\x09");
  SHA1_TEST_SHA1("abcdbcdecdefdefgefghfghighijhijkijkljklmklmnlmnomnopnopq", "\x84\x98\x3e\x44\x1c\x3b\xd2\x6e\xba\xae\x4a\xa1\xf9\x51\x29\xe5\xe5\x46\x70\xf1");
}

void sha1_test ()
{
  sha1_test_sha1();
}
