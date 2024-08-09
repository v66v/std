/* Copyright (C) 2024 by v66v <74927439+v66v@users.noreply.github.com> */

#pragma once

/**************************************************/
/* To use add the bellow to the makefile:		  */
/* CFLAGS  = `pkg-config --cflags fontconfig`	  */
/* LDFLAGS = `pkg-config --libs fontconfig` -lstd */
/**************************************************/

#include "std-fontconfig-error.h"
#include <fontconfig/fontconfig.h>

#if defined(__cplusplus)
extern "C"
{
#endif

  typedef struct
  {
    FcConfig *conf;
    FcFontSet *fs;
    FcObjectSet *os;
    FcPattern *pat;
    FcResult result;
  } fontconfig_t;

  fontconfig_e font_get_path (fontconfig_t *fc, const char *name,
                              char **filepath);
#if defined(__cplusplus)
}
#endif
