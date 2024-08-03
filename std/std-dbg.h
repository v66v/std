/* Copyright (C) 2024 by v66v <74927439+v66v@users.noreply.github.com> */

#pragma once
#include <stdio.h>

#ifdef DEBUG
#define DEBUG_T(file, type, fmt, ...)                                         \
  do                                                                          \
    {                                                                         \
      fprintf (file, "%s:%d:0: %s: %s: " fmt, __FILE__, __LINE__, type,       \
               __func__, ##__VA_ARGS__);                                      \
      fflush (file);                                                          \
    }                                                                         \
  while (0)

#define DEBUG_LOG(fmt, ...) DEBUG_T (stdout, "info", fmt, ##__VA_ARGS__)
#define DEBUG_ERROR(fmt, ...) DEBUG_T (stderr, "error", fmt, ##__VA_ARGS__)
#define DEBUG_WARN(fmt, ...) DEBUG_T (stderr, "warning", fmt, ##__VA_ARGS__)

#else
#define DEBUG_T(file, type, fmt, ...)
#define DEBUG_LOG(fmt, ...)
#define DEBUG_ERROR(fmt, ...)
#define DEBUG_WARN(fmt, ...)
#endif
