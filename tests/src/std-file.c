#include <setjmp.h>
#include <stdarg.h>
#include <stddef.h>
#include <stdint.h>

// should be last
#include <cmocka.h>

#include "global.h"
#include "std/std-file.h"
#include "std/std-str.h"
#include <stdio.h>
#include <string.h>

char *file_string
    = "Lorem ipsum dolor sit amet, consectetur adipiscing elit. Aliquam at "
      "pulvinar\nlibero, ac pharetra magna. Interdum et malesuada fames ac "
      "ante ipsum primis in\nfaucibus. Vestibulum ante ipsum primis in "
      "faucibus orci luctus et ultrices\nposuere cubilia curae; Duis nec "
      "pellentesque diam, at eleifend nulla. In\ntristique augue turpis, ut "
      "tempor sapien luctus ut. Nulla dictum dapibus\ntristique. Praesent "
      "imperdiet ex eu augue placerat varius. Lorem ipsum dolor sit\namet, "
      "consectetur adipiscing elit.\n";

#define File ETC "lorem-ipsum.txt"
#define File_Wrong ETC "lorem-wrong-filename-ipsum.txt"

static void
fopenp_test ()
{
  FILE *fp;
  int res = fopenp (&fp, File, "r"), fd = disable_stderr ();

  assert_int_equal (res, 0);
  assert_non_null (fp);
  fclose (fp);

  res = fopenp (&fp, File_Wrong, "r");

  assert_int_equal (res, 1);
  assert_null (fp);
  enable_stderr (fd);
}

static void
fclosep_test ()
{
  FILE *fp;
  int res, fd = disable_stderr ();
  fopenp (&fp, File, "r");

  res = fclosep (fp, File);

  assert_int_equal (res, 0);
  assert_non_null (fp);
  enable_stderr (fd);
}

static void
fseek_test ()
{
  FILE *fp;
  int res, fd = disable_stderr ();
  fopenp (&fp, File, "r");

  res = fseekp (fp, File, 0L, SEEK_END);
  assert_int_equal (res, 0);

  rewind (fp);

  res = fseekp (fp, File, 0L, 69);
  assert_int_equal (res, 1);

  res = fclosep (fp, File);
  enable_stderr (fd);
}

static void
ftellp_test ()
{
  FILE *fp;
  int res, fd = disable_stderr ();
  ssize_t loc;
  fopenp (&fp, File, "r");

  res = ftellp (fp, &loc, File);
  assert_int_equal (res, 0);
  assert_int_equal (loc, 0);

  fclosep (fp, File);
  enable_stderr (fd);
}

static void
file_slurp_test ()
{
  char *str;
  FILE *fp;
  int res, fd = disable_stderr ();
  fopenp (&fp, File, "r");

  res = file_slurp (File, &str);
  assert_int_equal (res, 0);
  assert_string_equal (str, file_string);
  fclosep (fp, File);

  res = file_slurp (File_Wrong, &str);
  assert_int_equal (res, 1);

  enable_stderr (fd);
}

static void
file_tryopen_test ()
{
  FILE *fp;
  int res, fd = disable_stderr ();

  res = file_tryopen (&fp, File, "r", stdout);
  assert_int_equal (res, 0);
  assert_non_null (fp);
  fclosep (fp, File);

  res = file_tryopen (&fp, File_Wrong, "r", stdout);
  assert_int_equal (res, 1);
  assert_ptr_equal (fp, stdout);

  enable_stderr (fd);
}

int
main (void)
{
  const struct CMUnitTest tests[] = {
    cmocka_unit_test (fopenp_test),     cmocka_unit_test (fclosep_test),
    cmocka_unit_test (fseek_test),      cmocka_unit_test (ftellp_test),
    cmocka_unit_test (file_slurp_test), cmocka_unit_test (file_tryopen_test),
  };

  TEST_START
  return cmocka_run_group_tests (tests, NULL, NULL);
}
