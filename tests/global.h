#pragma once
#include <stdio.h>
#include <string.h>
#include <unistd.h>

#define ETC "./etc/"
#define TEST_START                                                            \
  printf ("\n\e[1mRunning test suite: %.*s\e[m\n",                            \
          (int)strcspn (__FILE__, "."), __FILE__);

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
