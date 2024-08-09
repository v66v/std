/* Copyright (C) 2024 by v66v <74927439+v66v@users.noreply.github.com> */

#pragma once
#include <assert.h>
#include <stdio.h>

#if defined(__cplusplus)
extern "C"
{
#endif

#define FONTCONFIG_ENUM(VARIANT)                                              \
  VARIANT (FONT_NAME)                                                         \
  VARIANT (FONT_LIST)                                                         \
  VARIANT (FONT_LOAD)                                                         \
  VARIANT (FONT_SET)

#define FONTCONFIG_ENUM_NAME(NAME) NAME,

#define FONTCONFIG_ENUM_STRING(NAME)                                          \
  case NAME:                                                                  \
    return #NAME;

#define FONTCONFIG_ENUM_ERROR(NAME)                                           \
  case NAME:                                                                  \
    fprintf (stderr, "ERROR: " #NAME);                                        \
    break;

  typedef enum
  {
    FONTCONFIG_ENUM (FONTCONFIG_ENUM_NAME)
  } fontconfig_e;

  const char *fontconfig_error_name (fontconfig_e error);
  void fontconfig_error (fontconfig_e error);

#if defined(__cplusplus)
}
#endif
