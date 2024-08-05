#include <setjmp.h>
#include <stdarg.h>
#include <stddef.h>
#include <stdint.h>

// should be last
#include <cmocka.h>

#include "std/std-lib.h"
#include <unistd.h>

int
disable_stderr ()
{
  fflush (stderr);
  int fd = dup (STDERR_FILENO);
  freopen ("/dev/null", "w", stderr);
  return fd;
}

void
enable_stderr (int fd)
{
  fflush (stderr);
  dup2 (fd, fileno (stderr));
  close (fd);
}

static void
mallocp_test ()
{
  int *ptr, size = 100;
  int res = mallocp ((void **)&ptr, size);
  assert_int_equal (res, 0);
  assert_non_null (ptr);
  free (ptr);
}

static void
reallocp_test ()
{
  int *ptr, size = 100;
  int res = mallocp ((void **)&ptr, size);
  res = reallocp ((void **)&ptr, 20 * size);

  assert_int_equal (res, 0);
  assert_non_null (ptr);

  int fd = disable_stderr ();
  res = reallocp ((void **)&ptr, -20 * size);
  enable_stderr (fd);

  assert_int_equal (res, 1);
  assert_non_null (ptr);
  free (ptr);
}

static void
reallocpe_test ()
{
  int *ptr, size = 100;
  int res = mallocp ((void **)&ptr, size);

  int fd = disable_stderr ();

  res = reallocpe ((void **)&ptr, 20 * size);

  assert_int_equal (res, 0);
  assert_non_null (ptr);

  res = reallocpe ((void **)&ptr, -20 * size);

  assert_int_equal (res, 1);
  assert_null (ptr);

  enable_stderr (fd);
}

static const char file_name[] = __FILE__;
int
main (void)
{
  const struct CMUnitTest tests[] = {
    cmocka_unit_test (mallocp_test),
    cmocka_unit_test (reallocp_test),
    cmocka_unit_test (reallocpe_test),
  };

  printf ("\n\e[1mRunning test suite: %.*s\e[m\n",
          (int)strcspn (file_name, "."), file_name);
  return cmocka_run_group_tests (tests, NULL, NULL);
}
