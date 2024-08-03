/* Copyright (C) 2024 by v66v <74927439+v66v@users.noreply.github.com> */

#include "std-lib.h"

inline int
mallocp (void **ptr, size_t size)
{
  *ptr = (void *)malloc (size);
  if (*ptr == NULL)
    {
      ERRNO ("Error allocating %ld byte(s) of memory", size)
      return 1;
    }
  return 0;
}

int
reallocp (void **ptr, size_t size)
{
  void *ptr_tmp = realloc (*ptr, size);
  if (ptr_tmp == NULL)
    {
      ERRNO ("Error reallocating %ld byte(s) of memory", size)
      return 1;
    }

  *ptr = ptr_tmp;
  return 0;
}

inline int
reallocpe (void **ptr, size_t size)
{
  int res = reallocp (ptr, size);
  if (res)
    {
      free (*ptr);
      *ptr = NULL;
    }

  return res;
}
