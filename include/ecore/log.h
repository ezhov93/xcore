#ifndef ECORE_LOG_H
#define ECORE_LOG_H

#include <stdarg.h>
#include <stdbool.h>
#include <stdio.h>
#include <time.h>

typedef enum {
  LogLevel_None = -1,
  LogLevel_Msg,
  LogLevel_Time,
  LogLevel_Trace,
  LogLevel_Debug,
  LogLevel_Info,
  LogLevel_Warn,
  LogLevel_Error,
  LogLevel_Fatal,
  LogLevel_Max
} LogLevel;

#ifdef LOG_USE_FILE
#define eMessage(...) Log_log(LogLevel_Msg, __FILE__, __LINE__, __VA_ARGS__)
#define eTime(...) Log_log(LogLevel_Time, __FILE__, __LINE__, __VA_ARGS__)
#define eTace(...) Log_log(LogLevel_Trace, __FILE__, __LINE__, __VA_ARGS__)
#define eDebug(...) Log_log(LogLevel_Debug, __FILE__, __LINE__, __VA_ARGS__)
#define eInfo(...) Log_log(LogLevel_Info, __FILE__, __LINE__, __VA_ARGS__)
#define eWarn(...) Log_log(LogLevel_Warn, __FILE__, __LINE__, __VA_ARGS__)
#define eError(...) Log_log(LogLevel_Error, __FILE__, __LINE__, __VA_ARGS__)
#define eFatal(...) Log_log(LogLevel_Fatal, __FILE__, __LINE__, __VA_ARGS__)
#else
#define eMessage(...) Log_log(LogLevel_Msg, 0, 0, __VA_ARGS__)
#define eTime(...) Log_log(LogLevel_Time, 0, 0, __VA_ARGS__)
#define eTace(...) Log_log(LogLevel_Trace, 0, 0, __VA_ARGS__)
#define eDebug(...) Log_log(LogLevel_Debug, 0, 0, __VA_ARGS__)
#define eInfo(...) Log_log(LogLevel_Info, 0, 0, __VA_ARGS__)
#define eWarn(...) Log_log(LogLevel_Warn, 0, 0, __VA_ARGS__)
#define eError(...) Log_log(LogLevel_Error, 0, 0, __VA_ARGS__)
#define eFatal(...) Log_log(LogLevel_Fatal, 0, 0, __VA_ARGS__)
#endif

const char *Log_levelString(int level);
void Log_setLevel(int level);
void Log_log(int level, const char *file, int line, const char *fmt, ...);
void Log_welcome(const char *name, const char *version, const char *author,
                 const char *board);

#endif // ECORE_LOG_H
