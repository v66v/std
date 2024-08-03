/* Copyright (C) 2024 by v66v <74927439+v66v@users.noreply.github.com> */

#include "str.h"
#include "std.h"
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
  size_t len = strlen (orig);
  *dest = (char *)malloc ((len + 1) * sizeof (char));

  if (dest == NULL)
    {
      ERRNO ("Error allocing %ld byte(s) of memory", len + 1)
      return 1;
    }

  strncpy (*dest, orig, len);
  (*dest)[len] = '\0';
  return 0;
}
