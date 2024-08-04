#include "std/std-str.h"
#include <setjmp.h>
#include <stdarg.h>
#include <stddef.h>
#include <stdint.h>

// should be last
#include <cmocka.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>

static void
touppers_test ()
{
  char str[] = "HeLlO";
  touppers (str);
  assert_string_equal (str, "HELLO");
}

static void
tolowers_test ()
{
  char str[] = "HeLlO";
  tolowers (str);
  assert_string_equal (str, "hello");
}

static void
str_copym_test ()
{
  char str[] = "HeLlO";
  char *new_str;
  str_copym (&new_str, str);
  assert_ptr_not_equal (new_str, str);
  assert_string_equal (new_str, str);
}

static const char file_name[] = __FILE__;
int
main (void)
{
  const struct CMUnitTest tests[] = {
    cmocka_unit_test (touppers_test),
    cmocka_unit_test (tolowers_test),
    cmocka_unit_test (str_copym_test),
  };

  printf ("\n\e[1mRunning test suite: %.*s\e[m\n",
          (int)strcspn (file_name, "."), file_name);
  return cmocka_run_group_tests (tests, NULL, NULL);
}
