/* Copyright (C) 2024 by v66v <74927439+v66v@users.noreply.github.com> */

#include "std-log.h"
#include "std-file.h"
#include "std-lib.h"
#include "std.h"
#include <errno.h>
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define LOGDO(x) (logger.state && x <= logger.level)
#define LOGBUFSIZE 1000

typedef struct
{
  log_level level;
  int state;
  FILE *fo;
  int fom;
  FILE *fe;
  int fem;
  char mode[3];
  time_t tm;
  char *buf;
  size_t buf_size;
  struct tm *tm_info;

} logger_t;

logger_t logger = { 0 };

void
log_out (log_level lvl, const char *prefix, const char *suffix,
         const char *fmt, ...)
{
  char *c = logger.buf;
  int res = 0;
  if (LOGDO (lvl))
    {
      FILE *f = lvl <= lwarn ? logger.fe : logger.fo;

      strftime (c, 23, "[%Y-%m-%d %H:%M:%S] ", logger.tm_info);
      c += 22;

      if (prefix != NULL)
        {
          res = snprintf (c, LOGBUFSIZE - (c - logger.buf), "%s", prefix);
          c = MIN ((c - logger.buf) + (res > 0 ? res : 0), LOGBUFSIZE)
              + logger.buf;
        }

      va_list ap;
      va_start (ap, fmt);
      res = vsnprintf (c, LOGBUFSIZE - (c - logger.buf), fmt, ap);
      va_end (ap);
      c = MIN ((c - logger.buf) + (res > 0 ? res : 0), LOGBUFSIZE)
          + logger.buf;

      if (suffix != NULL)
        {
          res = snprintf (c, LOGBUFSIZE - (c - logger.buf), "%s", suffix);
          c = MIN ((c - logger.buf) + (res > 0 ? res : 0), LOGBUFSIZE)
              + logger.buf;
        }

      if ((c - logger.buf) < LOGBUFSIZE)
        c += 2;

      c[-1] = '\n';
      c[0] = '\0';

      flockfile (f); /* thread safety */
      fwrite (logger.buf, 1, c - logger.buf, f);
      funlockfile (f);
    }
}

void
log_set_fp (FILE **fp, char *filepath, FILE *fallback)
{
  int status;
  if ((status = file_tryopen (fp, filepath, logger.mode, fallback)) == 1)
    {
      log_errno ("Error opening file %s", filepath);
      log_warn ("Defaulting to stdout\n");
    }
  else if (status == 0)
    {
      logger.fom = 1;
    }
}

inline int
log_init (log_level level, char *fstdout, char *fstderr, char *mode)
{
  if (logger.level != 0)
    {
      log_error ("Logger already initialized\n");
      return 1;
    }

  logger.level = level;
  logger.state = 1;
  if (mode != NULL)
    strncpy (logger.mode, mode, 3);

  logger.buf_size = LOGBUFSIZE;

  if (mallocp ((void **)&logger.buf, LOGBUFSIZE))
    return 1;

  logger.tm = time (NULL);
  logger.tm_info = localtime (&logger.tm);

  log_set_fp (&logger.fo, fstdout, stdout);
  log_set_fp (&logger.fe, fstderr, stderr);
  return 0;
}

inline void
log_deinit ()
{
  if (logger.fom)
    fclose (logger.fo);
  if (logger.fem)
    fclose (logger.fe);
  free (logger.buf);
}

inline void
log_set_level (log_level level)
{
  logger.level = level;
}

inline void
log_disable ()
{
  logger.state = 0;
}

inline void
log_enable ()
{
  logger.state = 1;
}

inline void
log_toggle ()
{
  logger.state = !logger.state;
}
