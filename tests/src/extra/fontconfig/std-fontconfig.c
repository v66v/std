#include <setjmp.h>
#include <stdarg.h>
#include <stddef.h>
#include <stdint.h>

// should be last
#include <cmocka.h>

#include "global.h"
#include "std/extra/fontconfig/std-fontconfig.h"
#include <stdlib.h>
#include <unistd.h>

static void
font_get_path_test ()
{
  fontconfig_t fc;
  char *filepath;
  char path[PATH_MAX];
  snprintf (path, PATH_MAX, "%s%s%s", getenv ("HOME"),
            "/.guix-home/profile/share/fonts/",
            "truetype/iosevka-comfy-regular.ttf");

  font_get_path (&fc, "iosevka comfy", &filepath);
  assert_string_equal (filepath, path);
  free(filepath);
}

int
main (void)
{
  const struct CMUnitTest tests[] = {
    cmocka_unit_test (font_get_path_test),
  };

  TEST_START
  return cmocka_run_group_tests (tests, NULL, NULL);
}
