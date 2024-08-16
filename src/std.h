/* Copyright (C) 2024 by v66v <74927439+v66v@users.noreply.github.com> */

#pragma once
#include <stdio.h>

#if defined(__cplusplus)
extern "C"
{
#endif

#define UNUSED(x) (void)(x)

#define STR_IMPL_(...) #__VA_ARGS__
#define STR(...) STR_IMPL_ (__VA_ARGS__)

#define pe(...) fprintf (stderr, __VA_ARGS__)
#define po(...) printf (__VA_ARGS__)

#define MAX(a, b)                                                             \
  ({                                                                          \
    __typeof__ (a) _a = (a);                                                  \
    __typeof__ (b) _b = (b);                                                  \
    _a > _b ? _a : _b;                                                        \
  })

#define MIN(a, b)                                                             \
  ({                                                                          \
    __typeof__ (a) _a = (a);                                                  \
    __typeof__ (b) _b = (b);                                                  \
    _a < _b ? _a : _b;                                                        \
  })

/* Bitwise flip the lsb */
#define flip(a) a ^= 1

/* Bitwise flip the nth bit */
#define bflip(a, n) a ^= 1 << n

#if defined(__cplusplus)
}
#endif
