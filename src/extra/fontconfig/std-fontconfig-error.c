/* Copyright (C) 2024 by v66v <74927439+v66v@users.noreply.github.com> */

#include "std-fontconfig-error.h"

const char *
fontconfig_error_name (fontconfig_e error)
{
  switch (error)
    {
      FONTCONFIG_ENUM (FONTCONFIG_ENUM_STRING)
    default:
      assert (!"Unknown fontconfig error type");
    }
}

void
fontconfig_error (fontconfig_e error)
{
  switch (error)
    {
      FONTCONFIG_ENUM (FONTCONFIG_ENUM_ERROR)
    default:
      assert (!"Unknown fontconfig error type");
    }
}
