/* Copyright (C) 2024 by v66v <74927439+v66v@users.noreply.github.com> */

#pragma once
#include "std-err.h"
#include <stdlib.h>

int mallocp (void **ptr, size_t size);
int reallocp (void **ptr, size_t size);
int reallocpe (void **ptr, size_t size);
