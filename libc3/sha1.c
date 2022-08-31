/* c3
 * Copyright 2022 Thomas de Grivel <thoxdg@gmail.com>
 */
#include <assert.h>
#include <sys/types.h>
#include <stdlib.h>
#include <sha1.h>
#include "str.h"

s_str *sha1 (s_str *src)
{
  s8 *hash;
  s_str *str;
  SHA1_CTX ctx;
  assert(src);
  hash = calloc(SHA1_DIGEST_LENGTH, 1);
  str = str_new(hash, SHA1_DIGEST_LENGTH, hash);
  assert(str);
  SHA1Init(&ctx);
  SHA1Update(&ctx, src->ptr.p, src->size);
  SHA1Final((u8 *) hash, &ctx);
  return str;
}
