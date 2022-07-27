#ifndef ECORE_DRIVERS_TIMER_H
#define ECORE_DRIVERS_TIMER_H

#include <stdlib.h>

typedef enum {
  TimerError_Sucess,
  TimerError_InvalidValue,
  TimerError_FunctionNotSupported,
  TimerError_NotRunning,
  TimerError_SetFailed
} TimerError;

struct TimerPrivate;
typedef struct TimerPrivate *Timer;
typedef void (*TimerCallback)(void *context);

Timer Timer_open(int instance);
void Timer_setCallback(Timer const self, TimerCallback callback, void *context);
int Timer_start(Timer const self);
int Timer_stop(Timer const self);
int Timer_getValue(Timer const self, size_t *us);
int Timer_delay(Timer const self, unsigned int us);
int Timer_cancel(Timer const self);
int Timer_close(Timer const self);

#endif // ECORE_DRIVERS_TIMER_H
