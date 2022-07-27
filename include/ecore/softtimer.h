#ifndef ECORE_SOFT_TIMER_H
#define ECORE_SOFT_TIMER_H

#include <stdbool.h>
#include <time.h>

typedef void (*SoftTimerCallback)(void *context);

typedef struct SoftTimer {
  clock_t timer;
  clock_t timerSinceStart;
  clock_t interval;
  SoftTimerCallback callback;
  void *callbackContext;
  bool isActive;
  bool isReady;
  bool singleShot;
} SoftTimer;

void SoftTimer_init(SoftTimer *self);
int SoftTimer_interval(const SoftTimer *self);
static inline bool SoftTimer_isActive(const SoftTimer *self) {
  return self->isActive;
}
bool SoftTimer_isReady(SoftTimer *self);
static inline bool SoftTimer_isSingleShot(const SoftTimer *self) {
  return self->singleShot;
}
static inline void SoftTimer_setSingleShot(SoftTimer *self, bool singleShot) {
  self->singleShot = true;
}
void SoftTimer_setInterval(SoftTimer *self, int ms);
void SoftTimer_start(SoftTimer *self, int ms);
static inline void SoftTimer_stop(SoftTimer *self) { self->isActive = false; }
void SoftTimer_reset(SoftTimer *self);
void SoftTimer_update(SoftTimer *self);
int SoftTimer_elapsed(const SoftTimer *self);
static inline int SoftTimer_remaningTime(const SoftTimer *self) {
  return self->interval - SoftTimer_elapsed(self);
}
int SoftTimer_timeSinceStart(const SoftTimer *self);
static inline void SoftTimer_setCallback(SoftTimer *self,
                                         SoftTimerCallback callback,
                                         void *context) {
  self->callback = callback;
  self->callbackContext = context;
}

#endif // ECORE_SOFT_TIMER_H