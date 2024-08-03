/* Copyright (C) 2024 by v66v <74927439+v66v@users.noreply.github.com> */

#include "std-file.h"
#include "std-err.h"
#include "std-lib.h"
#include "std.h"
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>

int
file_slurp (char *filepath, char **file_str)
{
  FILE *fp = fopen (filepath, "r");
  ssize_t size;
  int read;

  if (fp == NULL)
    {
      ERRNO ("Error opening file %s", filepath)
      return 1;
    }

  if ((size = fseek (fp, 0L, SEEK_END)) == -1)
    {
      ERRNO ("Error seeking file %s", filepath)
      FCLOSE (fp, filepath)
      return 1;
    }

  if ((size = ftell (fp)) == -1)
    {
      ERRNO ("Error telling file %s", filepath)
      FCLOSE (fp, filepath)
      return 1;
    }

  rewind (fp); /* Go back to the beginning of the file */

  if (mallocp ((void **)file_str, (size + 1) * sizeof (char)))
    {
      FCLOSE (fp, filepath)
      return 1;
    }

  read = fread (*file_str, sizeof (char), size, fp) / sizeof (char);

  if (read != size)
    {
      if (feof (fp) != 0)
        {
          fprintf (stderr, "EOF hit while reading file: %s", filepath);
        }
      else if (ferror (fp) != 0)
        {
          fprintf (stderr, "Error occured while reading file: %s", filepath);
        }

      if (SLURP_FILE_REALLOC) /* Compiler will optimize, no need for #if */
        {
          if (reallocpe ((void **)file_str, read + 1))
            {
              FCLOSE (fp, filepath)
              return 1;
            }
        }
    }

  (*file_str)[read] = '\0';

  if (fclose (fp) != 0)
    {
      ERRNO ("Error closing file %s", filepath)
      return 1;
    }
  return 0;
}

inline int
file_tryopen (FILE **fp, char *filepath, char *mode, FILE *fallback)
{
  if (filepath == NULL)
    {
      *fp = fallback;
      return 2;
    }

  *fp = fopen (filepath, mode);

  if (*fp == NULL)
    {
      *fp = fallback;
      return 1;
    }

  return 0;
}
