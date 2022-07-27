#include "ecore/log.h"

#include <string.h>
#include <time.h>

static const char *_levelStrings[] = {"",     "",     "TRACE", "DEBUG",
                                      "INFO", "WARN", "ERROR", "FATAL"};

#ifdef LOG_USE_COLOR
static const char *_levelColors[] = {"\x1b[94m", "\x1b[94m", "\x1b[94m",
                                     "\x1b[36m", "\x1b[32m", "\x1b[33m",
                                     "\x1b[31m", "\x1b[35m"};
#endif

int _level = -1;

void Log_setLevel(int level) { _level = level; }

void Log_log(int level, const char *file, int line, const char *fmt, ...) {
  if (level > _level)
    return;

  char buf[80];
  struct timespec ts = {};
  clock_gettime(CLOCK_MONOTONIC, &ts);
  if (level == LogLevel_Time)
    sprintf(buf, "%6d.%06d", (int)ts.tv_sec, (int)ts.tv_nsec / 1000);
  else
    sprintf(buf, "%-5s", _levelStrings[level]);

  if (level != LogLevel_Msg) {
#ifdef LOG_USE_COLOR
#ifdef LOG_USE_FILE
    printf("[%s%s\x1b[0m]\x1b[90m%s:%d:\x1b[0m ", _levelColors[_level], buf,
           file, line);
#else
    printf("[%s%s\x1b[0m] ", _levelColors[_level], buf);
#endif
#else
#ifdef LOG_USE_FILE
    printf("[%s] %s:%d: ", buf, file, line);
#else
    printf("[%s] ", buf);
#endif
#endif
  }
  va_list args;
  va_start(args, fmt);

  vsnprintf(buf, sizeof(buf), fmt, args);
  va_end(args);
  puts(buf);
}

void Log_welcome(const char *name, const char *version, const char *author,
                 const char *board) {
  printf("   __\r\n"
         "  /\\ \\      name: %s\r\n"
         " / /\\ \\  version: %s\r\n"
         "/ /__\\ \\  author: %s\r\n"
         "\\/____\\/   board: %s\r\n\r\n",
         name, version, author, board);
}
