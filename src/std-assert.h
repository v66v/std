#pragma once
#include <stdio.h>
#include <stdlib.h>

#ifndef NDEBUG

void assert_p (const char *expr, const char *filepath, size_t linenumber,
               const char *fmt, ...) __attribute__ ((format (printf, 4, 5)));

#ifdef ASSERT_LOG
#include "std-log.h"
#define assertp(...)                                                          \
  log_fatal ("%s:%ld:0: assertion failure: %s: " fmt, filepath, linenumber,   \
             expr, ##__VA_ARGS__);                                            \
  abort ()
#else
#define assertp(...) assert_p (##__VA_ARGS__)
#endif

#define asserti(exp)                                                          \
  {                                                                           \
    if (!(exp))                                                               \
      {                                                                       \
        assertp (#exp, __FILE__, __LINE__, "");                               \
      }                                                                       \
  }

#define assertm(exp, fmt, ...)                                                \
  {                                                                           \
    if (!(exp))                                                               \
      {                                                                       \
        assertp (#exp, __FILE__, __LINE__, fmt, ##__VA_ARGS__);               \
      }                                                                       \
  }

#else
#define asserti(exp)
#define assertm(exp, fmt)
#endif
