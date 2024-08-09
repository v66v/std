/* Copyright (C) 2024 by v66v <74927439+v66v@users.noreply.github.com> */

#include "std-fontconfig.h"
#include "std-str.h"

fontconfig_e
font_get_path (fontconfig_t *fc, const char *name, char **filepath)
{
  fc->conf = FcInitLoadConfigAndFonts ();
  fc->pat = FcNameParse ((FcChar8 *)name);

  if (!fc->pat)
    {
      return FONT_NAME;
    }

  FcConfigSubstitute (fc->conf, fc->pat, FcMatchPattern);
  FcDefaultSubstitute (fc->pat);

  fc->fs = FcFontSetCreate ();
  fc->os = FcObjectSetBuild (FC_FAMILY, FC_STYLE, FC_FILE, (char *)0);

  FcFontSet *font_patterns;
  font_patterns = FcFontSort (fc->conf, fc->pat, FcTrue, 0, &fc->result);

  if (!font_patterns || font_patterns->nfont == 0)
    {
      return FONT_LIST;
    }

  FcPattern *font_pattern;
  font_pattern
      = FcFontRenderPrepare (fc->conf, fc->pat, font_patterns->fonts[0]);
  if (font_pattern)
    {
      FcFontSetAdd (fc->fs, font_pattern);
    }
  else
    {
      return FONT_LOAD;
    }

  FcFontSetSortDestroy (font_patterns);
  FcPatternDestroy (fc->pat);

  if (fc->fs)
    {
      if (fc->fs->nfont > 0)
        {
          FcValue v;
          FcPattern *font;

          font = FcPatternFilter (fc->fs->fonts[0], fc->os);

          FcPatternGet (font, FC_FILE, 0, &v);
          str_copym (filepath, (char *)v.u.f);

          FcPatternDestroy (font);
        }
      FcFontSetDestroy (fc->fs);
    }
  else
    {
      if (fc->os)
        FcObjectSetDestroy (fc->os);
      return FONT_SET;
    }

  if (fc->os)
    FcObjectSetDestroy (fc->os);

  return 0;
}
