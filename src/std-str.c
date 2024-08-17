/* Copyright (C) 2024 by v66v <74927439+v66v@users.noreply.github.com> */

#include "std-str.h"
#include "std-err.h"
#include "std-lib.h"
#include <ctype.h>
#include <errno.h>
#include <stdlib.h>
#include <string.h>

void
tolowers (char *s)
{
  while (*s)
    {
      *s = tolower ((unsigned char)*s);
      s++;
    }
}

void
touppers (char *s)
{
  while (*s)
    {
      *s = toupper ((unsigned char)*s);
      s++;
    }
}

int
str_copym (char **dest, char *orig)
{
  unsigned long len = strlen (orig);

  if (mallocp ((void **)dest, (len + 1) * sizeof (char)))
    return 1;

  strncpy (*dest, orig, len + 1);
  (*dest)[len] = '\0';
  return 0;
}

int
palindromep (char *s)
{
  const size_t len = strlen (s);
  for (size_t i = 0; i < len / 2; i++)
    {
      if (s[i] != s[len - i - 1])
        return 1;
    }
  return 0;
}
