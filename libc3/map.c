/* c3
 * Copyright 2022,2023 kmx.io <contact@kmx.io>
 *
 * Permission is hereby granted to use this software granted the above
 * copyright notice and this permission paragraph are included in all
 * copies and substantial portions of this software.
 *
 * THIS SOFTWARE IS PROVIDED "AS-IS" WITHOUT ANY GUARANTEE OF
 * PURPOSE AND PERFORMANCE. IN NO EVENT WHATSOEVER SHALL THE
 * AUTHOR BE CONSIDERED LIABLE FOR THE USE AND PERFORMANCE OF
 * THIS SOFTWARE.
 */
#include <assert.h>
#include <err.h>
#include <stdlib.h>
#include "compare.h"
#include "list.h"
#include "map.h"
#include "tag.h"

s_tag * map_access (const s_map *map, const s_tag *key, s_tag *value)
{
  assert(map);
  assert(key);
  assert(value);
  if (key->type != TAG_SYM) {
    warnx("map_access: only works with symbol key");
    return NULL;
  }
  return map_get(map, key, value);
}

void map_clean (s_map *map)
{
  assert(map);
  free(map->keys);
  free(map->values);
}

s_map * map_copy (const s_map *src, s_map *dest)
{
  uw i = 0;
  assert(src);
  assert(dest);
  map_init(dest, src->count);
  while (i < src->count) {
    tag_copy(src->keys + i, dest->keys + i);
    tag_copy(src->values + i, dest->values + i);
    i++;
  }
  return dest;
}

void map_delete (s_map *map)
{
  assert(map);
  map_clean(map);
  free(map);
}

s_tag * map_get (const s_map *map, const s_tag *key, s_tag *value)
{
  uw i = 0;
  while (i < map->count) {
    if (compare_tag(key, map->keys + i) == 0)
      return tag_copy(map->values + i, value);
    i++;
  }
  return NULL;
}

s_map * map_init (s_map *map, uw count)
{
  assert(map);
  map->count = count;
  map->keys = calloc(count, sizeof(s_tag));
  map->values = calloc(count, sizeof(s_tag));
  return map;
}

s_map * map_init_from_lists (s_map *map, const s_list *keys,
                             const s_list *values)
{
  sw i = 0;
  const s_list *k;
  sw len;
  const s_list *v;
  assert(map);
  if ((len = list_length(keys)) != list_length(values)) {
    warnx("map_init_from_lists: list length don't match");
    return NULL;
  }
  map_init(map, len);
  k = keys;
  v = values;
  while (i < len) {
    if (! tag_copy(&k->tag, map->keys + i) ||
        ! tag_copy(&v->tag, map->values + i))
      goto ko;
    k = list_next(k);
    v = list_next(v);
    i++;
  }
  if (! map_sort(map))
    goto ko;
  return map;
 ko:
  map_clean(map);
  return NULL;
}

s_map * map_new (uw count)
{
  s_map *map;
  if (! (map = malloc(sizeof(s_map)))) {
    warn("map_new");
    return NULL;
  }
  return map_init(map, count);
}

s_map * map_new_from_lists (const s_list *keys, const s_list *values)
{
  s_map *map;
  if (! (map = malloc(sizeof(s_map)))) {
    warn("map_new");
    return NULL;
  }
  return map_init_from_lists(map, keys, values);
}

s_map * map_set (s_map *map, const s_tag *key, const s_tag *value)
{
  uw i = 0;
  while (i < map->count) {
    if (compare_tag(key, map->keys + i) == 0) {
      if (! tag_copy(value, map->values + i))
        return NULL;
      return map;
    }
    i++;
  }
  return NULL;
}

/* bubble sort */
s_map * map_sort (s_map *map)
{
  uw i;
  uw j;
  s_tag k;
  s_tag v;
  i = map->count;
  while (i > 0) {
    i--;
    j = 1;
    while (j <= i) {
      if (compare_tag(map->keys + j, map->keys + (j - 1)) < 0) {
        k = map->keys[j];
        v = map->values[j];
        map->keys[j] = map->keys[j - 1];
        map->values[j] = map->values[j - 1];
        map->keys[j - 1] = k;
        map->values[j - 1] = v;
      }
      j++;
    }
  }
  return map;
}

s_map * map_update (const s_map *map, const s_tag *key,
                    const s_tag *value, s_map *dest)
{
  s_map tmp;
  uw i = 0;
  map_copy(map, &tmp);
  while (i < map->count) {
    if (compare_tag(key, map->keys + i) == 0) {
      if (! tag_copy(value, map->values + i))
        goto ko;
      *dest = tmp;
      return dest;
    }
    i++;
  }
 ko:
  map_clean(&tmp);
  return NULL;
}

s_map * map_update_list (const s_map *map, const s_list *alist, s_map *dest)
{
  const s_list *i;
  s_map tmp;
  assert(map);
  map_copy(map, &tmp);
  i = alist;
  while (i) {
    assert(i->tag.type == TAG_TUPLE && i->tag.data.tuple.count == 2);
    if (i->tag.type != TAG_TUPLE || i->tag.data.tuple.count != 2) {
      warnx("map_update_list: not an associative list");
      goto ko;
    }
    if (! map_set(&tmp, i->tag.data.tuple.tag, i->tag.data.tuple.tag + 1))
      goto ko;
    i = list_next(i);
  }
  *dest = tmp;
  return dest;
 ko:
  map_clean(&tmp);
  return NULL;
}
