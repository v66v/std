#include "std-assert.h"
#include "std.h"
#include <stdarg.h>

#define ASSERT_BUFSIZE 300

static char assert_buf[ASSERT_BUFSIZE];

void
assert_p (const char *expr, const char *filepath, size_t linenumber,
          const char *fmt, ...)
{
  char *c = assert_buf;
  int res = snprintf (c, ASSERT_BUFSIZE,
                      "%s:%ld:0: assertion failure: %s: ", filepath,
                      linenumber, expr);
  c = MIN ((c - assert_buf) + (res > 0 ? res : 0), ASSERT_BUFSIZE)
      + assert_buf;

  va_list ap;
  va_start (ap, fmt);
  res = vsnprintf (c, ASSERT_BUFSIZE - (c - assert_buf), fmt, ap);
  va_end (ap);

  if ((c - assert_buf) < ASSERT_BUFSIZE)
    c += 2;

  c[-1] = '\n';
  c[0] = '\0';

  fprintf (stderr, "%." STR (ASSERT_BUFSIZE) "s", assert_buf);
  fflush (stderr);

  abort ();
}
