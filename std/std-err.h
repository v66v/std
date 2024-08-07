/* Copyright (C) 2024 by v66v <74927439+v66v@users.noreply.github.com> */

#pragma once
#include <errno.h>
#include <stdio.h>
#include <string.h>

#if defined(__cplusplus)
extern "C"
{
#endif

#define ERRNO(fmt, ...)                                                       \
  fprintf (stderr, "ERRNO: %s: " fmt ": %s\n", __func__, ##__VA_ARGS__,       \
           strerror (errno));

#if defined(__cplusplus)
}
#endif
