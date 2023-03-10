/* c3
 * Copyright 2022,2023 kmx.io <contact@kmx.io>
 *
 * Permission is hereby granted to use this software excepted
 * on Apple computers granted the above copyright notice and
 * this permission paragraph are included in all copies and
 * substantial portions of this software.
 *
 * THIS SOFTWARE IS PROVIDED "AS-IS" WITHOUT ANY GUARANTEE OF
 * PURPOSE AND PERFORMANCE. IN NO EVENT WHATSOEVER SHALL THE
 * AUTHOR BE CONSIDERED LIABLE FOR THE USE AND PERFORMANCE OF
 * THIS SOFTWARE.
 */
#include <assert.h>
#include <err.h>
#include <errno.h>
#include <stdlib.h>
#include "buf.h"
#include "buf_file.h"
#include "buf_inspect.h"
#include "buf_parse.h"
#include "compare.h"
#include "fact.h"
#include "facts.h"
#include "facts_cursor.h"
#include "facts_with.h"
#include "hash.h"
#include "log.h"
#include "set__fact.h"
#include "set__tag.h"
#include "skiplist__fact.h"
#include "tag.h"

sw facts_open_file_create (s_facts *facts, const s8 *path);
sw facts_open_log (s_facts *facts, s_buf *buf);

s_fact * facts_add_fact (s_facts *facts, const s_fact *fact)
{
  s_fact tmp;
  s_fact *f = NULL;
  s_set_item__fact *item;
  assert(facts);
  assert(fact);
  tmp.subject   = facts_ref_tag(facts, fact->subject);
  tmp.predicate = facts_ref_tag(facts, fact->predicate);
  tmp.object    = facts_ref_tag(facts, fact->object);
  if ((item = set_get__fact(&facts->facts, &tmp)))
    return &item->data;
  if (facts->log)
    facts_log_add(facts->log, &tmp);
  item = set_add__fact(&facts->facts, &tmp);
  f = &item->data;
  skiplist_insert__fact(facts->index_spo, f);
  skiplist_insert__fact(facts->index_pos, f);
  skiplist_insert__fact(facts->index_osp, f);
  return f;
}

s_fact * facts_add_tags (s_facts *facts, const s_tag *subject,
                         const s_tag *predicate,
                         const s_tag *object)
{
  s_fact fact;
  fact_init(&fact, subject, predicate, object);
  return facts_add_fact(facts, &fact);
}

void facts_clean (s_facts *facts)
{
  if (facts->log)
    facts_close(facts);
  skiplist_delete__fact(facts->index_osp);
  skiplist_delete__fact(facts->index_pos);
  skiplist_delete__fact(facts->index_spo);
  set_clean__fact(&facts->facts);
  set_clean__tag(&facts->tags);
}

void facts_close (s_facts *facts)
{
  assert(facts->log);
  log_close(facts->log);
  log_delete(facts->log);
  facts->log = NULL;
}

void facts_delete (s_facts *facts)
{
  assert(facts);
  facts_clean(facts);
  free(facts);
}

sw facts_dump (const s_facts *facts, s_buf *buf)
{
  s_facts_cursor cursor;
  s_fact *fact;
  t_hash hash;
  s_tag predicate;
  s_tag object;
  sw r;
  sw result = 0;
  s_tag subject;
  assert(facts);
  assert(buf);
  tag_init_var(&subject);
  tag_init_var(&predicate);
  tag_init_var(&object);
  if ((r = buf_write_1(buf,
                       "%{module: C3.Facts.Dump,\n"
                       "  version: 0x0000000000000001,\n"
                       "  count: 0x")) < 0)
    return r;
  result += r;
  if ((r = buf_inspect_u64_hex(buf, facts_count(facts))) < 0)
    return r;
  result += r;
  if ((r = buf_write_1(buf, "}\n")) < 0)
    return r;
  result += r;
  hash_init(&hash);
  facts_with_0(facts, &cursor, &subject, &predicate, &object);
  while ((fact = facts_cursor_next(&cursor))) {
    hash_update_fact(&hash, fact);
    if ((r = buf_inspect_fact(buf, fact)) < 0)
      return r;
    result += r;
    if ((r = buf_write_1(buf, "\n")) < 0)
      return r;
    result += r;
  }
  if ((r = buf_write_1(buf, "%{hash: 0x")) < 0)
    return r;
  result += r;
  if ((r = buf_inspect_u64_hex(buf, hash_to_u64(&hash))) < 0)
    return r;
  result += r;
  if ((r = buf_write_1(buf, "}\n")) < 0)
    return r;
  result += r;
  return result;
}

sw facts_dump_file (const s_facts *facts, const s8 *path)
{
  s8 b[BUF_SIZE];
  s_buf buf;
  FILE *fp;
  sw r;
  assert(facts);
  assert(path);
  buf_init(&buf, false, sizeof(b), b);
  if (! (fp = fopen(path, "wb"))) {
    warn("fopen: %s", path);
    return -1;
  }
  buf_file_open_w(&buf, fp);
  r = facts_dump(facts, &buf);
  buf_file_close(&buf);
  fclose(fp);
  return r;
}

s_fact * facts_find_fact (const s_facts *facts, const s_fact *fact)
{
  s_fact f;
  s_set_item__fact *item;
  assert(facts);
  assert(fact);
  if ((f.subject   = facts_find_tag(facts, fact->subject))   &&
      (f.predicate = facts_find_tag(facts, fact->predicate)) &&
      (f.object    = facts_find_tag(facts, fact->object))    &&
      (item = set_get__fact((const s_set__fact *) &facts->facts, &f)))
    return &item->data;
  return NULL;
}

s_tag * facts_find_tag (const s_facts *facts, const s_tag *tag)
{
  s_set_item__tag *item;
  assert(facts);
  assert(tag);
  if ((item = set_get__tag(&facts->tags, tag)))
    return &item->data;
  return NULL;
}

s_facts * facts_init (s_facts *facts)
{
  const u8 max_height = 10;
  const double spacing = 2.7;
  assert(facts);
  set_init__tag(&facts->tags, 1024);
  set_init__fact(&facts->facts, 1024);
  facts->index_spo = skiplist_new__fact(max_height, spacing);
  assert(facts->index_spo);
  facts->index_spo->compare = compare_fact;
  facts->index_pos = skiplist_new__fact(max_height, spacing);
  assert(facts->index_pos);
  facts->index_pos->compare = compare_fact_pos;
  facts->index_osp = skiplist_new__fact(max_height, spacing);
  assert(facts->index_osp);
  facts->index_osp->compare = compare_fact_osp;
  facts->log = NULL;
  return facts;
}

sw facts_load (s_facts *facts, s_buf *buf)
{
  u64 count;
  s_fact_w fact;
  s_fact *factp;
  t_hash hash;
  u64 hash_u64;
  u64 hash_u64_buf;
  u64 i;
  sw r;
  sw result = 0;
  assert(facts);
  assert(buf);
  if ((r = buf_read_1(buf,
                      "%{module: C3.Facts.Dump,\n"
                      "  version: 0x0000000000000001,\n"
                      "  count: 0x")) <= 0)
    goto ko_header;
  result += r;
  if ((r = buf_parse_u64_hex(buf, &count)) <= 0)
    goto ko_header;
  result += r;
  if ((r = buf_read_1(buf, "}\n")) <= 0)
    goto ko_header;
  result += r;
  hash_init(&hash);
  for (i = 0; i < count; i++) {
    if ((r = buf_parse_fact(buf, &fact)) <= 0)
      goto ko_fact;
    result += r;
    factp = fact_r(&fact);
    hash_update_fact(&hash, factp);
    facts_add_fact(facts, factp);
    fact_w_clean(&fact);
    if ((r = buf_read_1(buf, "\n")) <= 0) {
      r = -1;
      goto ko_fact;
    }
    result += r;
  }
  hash_u64 = hash_to_u64(&hash);
  if ((r = buf_read_1(buf, "%{hash: 0x")) <= 0)
    goto ko_hash;
  result += r;
  if ((r = buf_parse_u64_hex(buf, &hash_u64_buf)) <= 0)
    goto ko_hash;
  result += r;
  if ((r = buf_read_1(buf, "}\n")) <= 0)
    goto ko_hash;
  result += r;
  i++;
  if (hash_u64_buf != hash_u64) {
    s_buf tmp;
    buf_init_alloc(&tmp, 16);
    buf_inspect_u64_hex(&tmp, hash_u64);
    buf_write_s8(&tmp, 0);
    warnx("facts_load: invalid hash line %lu: 0x%s",
          (unsigned long) i + 3,
          tmp.ptr.ps8);
    return -1;
  }
  return result;
 ko_header:
  warnx("facts_load: invalid or missing header");
  return -1;
 ko_fact:
  warnx("facts_load: %s fact line %lu", r ? "invalid" : "missing",
        (unsigned long) i + 3);
  return -1;
 ko_hash:
  warnx("facts_load: %s hash line %lu", r ? "invalid" : "missing",
        (unsigned long) i + 3);
  return -1;
}

sw facts_load_file (s_facts *facts, const s8 *path)
{
  s8 b[BUF_SIZE];
  s_buf buf;
  FILE *fp;
  sw result;
  assert(facts);
  assert(path);
  buf_init(&buf, false, sizeof(b), b);
  if (! (fp = fopen(path, "r"))) {
    warn("facts_load_file: %s", path);
    return -1;
  }
  buf_file_open_r(&buf, fp);
  result = facts_load(facts, &buf);
  buf_file_close(&buf);
  fclose(fp);
  return result;
}

sw facts_log_add (s_log *log, const s_fact *fact)
{
  sw r;
  sw result = 0;
  assert(log);
  assert(fact);
  if ((r = buf_write_1(&log->buf, "add ")) < 0)
    return r;
  result += r;
  hash_update_1(&log->hash, "add");
  if ((r = buf_inspect_fact(&log->buf, fact)) < 0)
    return r;
  result += r;
  hash_update_fact(&log->hash, fact);
  if ((r = buf_write_1(&log->buf, "\n")) < 0)
    return r;
  result += r;
  return result;
}

sw facts_log_remove (s_log *log, const s_fact *fact)
{
  sw r;
  sw result = 0;
  if ((r = buf_write_1(&log->buf, "remove ")) < 0)
    return r;
  result += r;
  hash_update_1(&log->hash, "remove");
  if ((r = buf_inspect_fact(&log->buf, fact)) < 0)
    return r;
  result += r;
  hash_update_fact(&log->hash, fact);
  if ((r = buf_write_1(&log->buf, "\n")) < 0)
    return r;
  result += r;
  return result;
}

s_facts * facts_new ()
{
  s_facts *n;
  if (! (n = malloc(sizeof(s_facts))))
    errx(1, "facts_new: out of memory");
  return facts_init(n);
}

sw facts_open_buf (s_facts *facts, s_buf *buf)
{
  sw r;
  sw result = 0;
  if ((r = buf_read_1(buf,
                      "%{module: C3.Facts.Save,\n"
                      "  version: 0x0000000000000001}\n")) <= 0) {
    warnx("facts_open_buf: invalid or missing header");
    return -1;
  }
  result += r;
  if ((r = facts_load(facts, buf)) <= 0)
    return r;
  result += r;
  if ((r = facts_open_log(facts, buf)) < 0)
    return r;
  result += r;
  return result;
}

sw facts_open_file (s_facts *facts, const s8 *path)
{
  FILE *fp;
  s8 i[BUF_SIZE];
  s_buf in;
  sw r;
  sw result = 0;
  buf_init(&in, false, sizeof(i), i);
  if (! (fp = fopen(path, "rb"))) {
    if (errno == ENOENT)
      return facts_open_file_create(facts, path);
    return -1;
  }
  buf_file_open_r(&in, fp);
  if ((r = facts_open_buf(facts, &in)) < 0)
    return r;
  result += r;
  buf_file_close(&in);
  if (! (fp = fopen(path, "ab"))) {
    warn("fopen: %s", path);
    return -1;
  }
  if (! (facts->log = log_new(BUF_SIZE)))
    return -1;
  log_open(facts->log, fp);
  return result;
}

sw facts_open_file_create (s_facts *facts, const s8 *path)
{
  FILE *fp;
  s_buf *out;
  sw r;
  sw result = 0;
  if (! (fp = fopen(path, "wb"))) {
    warn("fopen: %s", path);
    return -1;
  }
  if (facts_count(facts)) {
    /* TODO: clear facts
       facts_close(facts);
       facts_remove_all(facts);
    */
    warnx("facts_open_file: not supported");
    return -1;
  }
  out = buf_new_alloc(BUF_SIZE);
  buf_file_open_w(out, fp);
  if ((r = facts_save_header(out)) < 0)
    return r;
  result += r;
  if ((r = facts_dump(facts, out)) < 0)
    return r;
  result += r;
  buf_flush(out);
  if (! (facts->log = log_new()))
    return -1;
  buf_file_open_w(&facts->log->buf, fp);
  return result;
}

sw facts_open_log (s_facts *facts, s_buf *buf)
{
  s_fact_w fact;
  s_fact *factp;
  sw r;
  sw result = 0;
  assert(facts);
  assert(buf);
  while (1) {
    if ((r = buf_read_1(buf, "add ")) < 0)
      break;
    result += r;
    if (r) {
      if ((r = buf_parse_fact(buf, &fact)) <= 0)
        break;
      result += r;
      factp = fact_r(&fact);
      facts_add_fact(facts, factp);
      goto ok;
    }
    if ((r = buf_read_1(buf, "remove ")) <= 0)
      break;
    result += r;
    if ((r = buf_parse_fact(buf, &fact)) <= 0)
      break;
    result += r;
    factp = fact_r(&fact);
    facts_remove_fact(facts, factp);
  ok:
    fact_w_clean(&fact);
    if ((r = buf_read_1(buf, "\n")) <= 0)
      break;
    result += r;
  }
  return result;
}

s_tag * facts_ref_tag (s_facts *facts, const s_tag *tag)
{
  s_set_item__tag *item;
  assert(facts);
  assert(tag);
  item = set_add__tag(&facts->tags, tag);
  item->usage++;
  return &item->data;
}

e_bool facts_remove_fact (s_facts *facts, const s_fact *fact)
{
  s_fact f;
  s_fact *found;
  assert(facts);
  assert(fact);
  found = facts_find_fact(facts, fact);
  if (found) {
    if (facts->log)
      facts_log_remove(facts->log, found);
    skiplist_remove__fact(facts->index_spo, found);
    skiplist_remove__fact(facts->index_pos, found);
    skiplist_remove__fact(facts->index_osp, found);
    f = *found;
    set_remove__fact(&facts->facts, found);
    facts_unref_tag(facts, f.subject);
    facts_unref_tag(facts, f.predicate);
    facts_unref_tag(facts, f.object);
    return true;
  }
  return false;
}

sw facts_save_file (s_facts *facts, const s8 *path)
{
  s8 b[BUF_SIZE];
  s_buf buf;
  FILE *fp;
  sw r;
  sw result = 0;
  assert(facts);
  assert(path);
  assert(! facts->log);
  buf_init(&buf, false, sizeof(b), b);
  if (! (fp = fopen(path, "wb"))) {
    warn("fopen: %s", path);
    return -1;
  }
  buf_file_open_w(&buf, fp);
  if ((r = facts_save_header(&buf)) < 0)
    goto ko;
  result += r;
  if ((r = facts_dump(facts, &buf)) < 0)
    goto ko;
  result += r;
  buf_flush(&buf);
  free(buf.user_ptr);
  buf.user_ptr = NULL;
  if (! (facts->log = log_new()))
    goto ko;
  if (log_open(facts->log, fp) < 0)
    goto ko;
  return result;
 ko:
  fclose(fp);
  return r;
}

sw facts_save_header (s_buf *buf)
{
  sw r;
  sw result = 0;
  if ((r = buf_write_1(buf,
                       "%{module: C3.Facts.Save,\n"
                       "  version: 0x0000000000000001}\n")) < 0)
    return r;
  result += r;
  return result;
}

e_bool facts_unref_tag (s_facts *facts, const s_tag *tag)
{
  s_set_item__tag *item;
  assert(facts);
  assert(tag);
  item = set_get__tag(&facts->tags, tag);
  if (item) {
    item->usage--;
    if (! item->usage)
      set_remove_item__tag(&facts->tags, item);
    return true;
  }
  return false;
}
