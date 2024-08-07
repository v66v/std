#include <setjmp.h>
#include <stdarg.h>
#include <stddef.h>
#include <stdint.h>

// should be last
#include <cmocka.h>

#include "global.h"
#include "std/std-lib.h"

static void
callocp_test ()
{
  int *ptr, size = 100;
  int arr[100] = { 0 };
  int res = callocp ((void **)&ptr, sizeof (int), size);

  assert_int_equal (res, 0);
  assert_non_null (ptr);
  assert_memory_equal (ptr, arr, sizeof (int) * size);
  free (ptr);
}

static void
mallocp_test ()
{
  int *ptr, size = 100;
  int res = mallocp ((void **)&ptr, sizeof (int) * size);
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

int
main (void)
{
  const struct CMUnitTest tests[] = {
    cmocka_unit_test (callocp_test),
    cmocka_unit_test (mallocp_test),
    cmocka_unit_test (reallocp_test),
    cmocka_unit_test (reallocpe_test),
  };

  TEST_START
  return cmocka_run_group_tests (tests, NULL, NULL);
}
