/* Copyright (C) 2024 by v66v <74927439+v66v@users.noreply.github.com> */

#pragma once

#if defined(__cplusplus)
extern "C"
{
#endif

  typedef enum
  {
    lfatal,
    lerror,
    lwarn,
    linfo,
    ldebug
  } log_level;

#define log_info(fmt, ...) log_out (linfo, "INFO: " fmt, ##__VA_ARGS__)
#define log_fatal(fmt, ...) log_out (lfatal, "FATAL: " fmt, ##__VA_ARGS__)
#define log_warn(fmt, ...) log_out (lwarn, "WARN: " fmt, ##__VA_ARGS__)
#define log_error(fmt, ...) log_out (lerror, "ERROR: " fmt, ##__VA_ARGS__)
#define log_debug(fmt, ...) log_out (ldebug, "DEBUG: " fmt, ##__VA_ARGS__)

#define log_errno(fmt, ...)                                                   \
  log_out (lerror, "ERRNO: " fmt ": %s\n", ##__VA_ARGS__, strerror (errno))

  int log_init (log_level level, char *fstdout, char *fstderr, char *mode);
  void log_deinit ();
  void log_set_level (log_level level);
  void log_disable ();
  void log_enable ();
  void log_toggle ();
  void log_out (log_level lvl, const char *fmt, ...)
      __attribute__ ((format (printf, 2, 3)));

#if defined(__cplusplus)
}
#endif
