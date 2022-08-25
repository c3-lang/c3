/* c3
 * Copyright 2022 Thomas de Grivel <thoxdg@gmail.com>
 */
#include <assert.h>
#include <err.h>
#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "ucd.h"

#define BUFSZ 1024

unsigned long read_hex (const char **src) {
  unsigned long ul = 0;
  char c;
  while ((c = **src) != '\0' && c != ';') {
    if ('0' <= c && c <= '9') {
      ul *= 16;
      ul += c - '0';
      (*src)++;
    }
    else if ('A' <= c && c <= 'F') {
      ul *= 16;
      ul += c - 'A' + 10;
      (*src)++;
    }
    else if ('a' <= c && c <= 'f') {
      ul *= 16;
      ul += c - 'a' + 10;
      (*src)++;
    }
    else {
      errx(1, "invalid character in index: %c", c);
    }
  }
  if (c == ';')
    (*src)++;
  return ul;
}

void ucd_parse (s_ucd ucd[UCD_MAX], char *line,
                unsigned long lineno) {
  unsigned long i;
  char *p = NULL;
  char *sep;
  p = line;
  i = read_hex((const char **) &p);
  if (i >= UCD_MAX)
    goto error;
  ucd[i].flags = 0;
  if (! (sep = strchr(p, ';')))
    goto error;
  *sep = '\0';
  ucd[i].name = strdup(p);
  p = sep + 1;
  if (! (sep = strchr(p, ';')))
    goto error;
  while (p < sep) {
    switch (*p) {
    case 'C': ucd[i].flags |= UCD_FLAG_C; break;
    case 'P': ucd[i].flags |= UCD_FLAG_P; break;
    case 'S': ucd[i].flags |= UCD_FLAG_S; break;
    case 'Z': ucd[i].flags |= UCD_FLAG_Z; break;
    case 'c': ucd[i].flags |= UCD_FLAG_c; break;
    case 'e': ucd[i].flags |= UCD_FLAG_e; break;
    case 'l': ucd[i].flags |= UCD_FLAG_LOWERCASE; break;
    case 'o': ucd[i].flags |= UCD_FLAG_o; break;
    case 'p': ucd[i].flags |= UCD_FLAG_p; break;
    case 's': ucd[i].flags |= UCD_FLAG_s; break;
    case 'u': ucd[i].flags |= UCD_FLAG_UPPERCASE; break;
    default: warnx("line %lu: unknown flag: '%c'", lineno, *p);
    }
    p++;
  }
  return;
 error:
  warnx("invalid entry line %lu: %s", lineno, line);
}

void ucd_write_c (s_ucd ucd[UCD_MAX])
{
  unsigned long i = 0;
  printf("static const s_ucd g_ucd[UCD_MAX] = {\n");
  while (i < UCD_MAX) {
    printf("  {%lx, \"%s\"},\n",
           ucd[i].flags,
           ucd[i].name);
    i++;
  }
}

int main (int argc, char **argv)
{
  char buf[BUFSZ];
  unsigned long lineno;
  s_ucd ucd[UCD_MAX];
  (void) argc;
  (void) argv;
  lineno = 0;
  while (fgets(buf, BUFSZ, stdin)) {
    size_t len = strlen(buf);
    if (len > 0 && buf[len - 1] == '\n')
      buf[len - 1] = 0;
    ucd_parse(ucd, buf, ++lineno);
  }
  ucd_write_c(ucd);
  return 0;
}
