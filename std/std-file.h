/* Copyright (C) 2024 by v66v <74927439+v66v@users.noreply.github.com> */

#pragma once
#include <stdio.h>

#define FCLOSE(fp, filepath)                                                  \
  if (fclose (fp) != 0)                                                       \
    {                                                                         \
      ERRNO ("Error closing file %s", filepath)                               \
    }

/*
 * if feof or ferror occurs when reading file, should the buffer be realloced?
 * 0 => NO, 1 => YES
 */
#define SLURP_FILE_REALLOC 0

int file_slurp (char *filepath, char **file_str);
int file_tryopen (FILE **fp, char *filepath, char *mode, FILE *fallback);
