/* Copyright (C) 2024 by v66v <74927439+v66v@users.noreply.github.com> */

#pragma once
#include <string.h>
#include <stdio.h>
#include <errno.h>

#define ERRNO(fmt, ...)                                                       \
  fprintf (stderr, "ERRNO: %s: " fmt ": %s\n", __func__, ##__VA_ARGS__,       \
           strerror (errno));
