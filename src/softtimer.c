#include "ecore/softtimer.h"

static inline clock_t msToClock_(unsigned int ms) {
  return (CLOCKS_PER_SEC / 1000.0) * ms;
}
static inline clock_t clockToMs_(clock_t clock) {
  return (float)(1000.0 / CLOCKS_PER_SEC) * clock;
}

void SoftTimer_init(SoftTimer *self) {
  self->interval = 0;
  self->timer = clock();
  self->isActive = false;
  self->isReady = false;
  self->singleShot = false;
  self->callback = 0;
  self->callbackContext = 0;
}

int SoftTimer_interval(const SoftTimer *self) {
  return clockToMs_(self->interval);
}

void SoftTimer_setInterval(SoftTimer *self, int ms) {
  self->interval = msToClock_(ms);
}

bool SoftTimer_isReady(SoftTimer *self) {
  SoftTimer_update(self);
  bool tmp = self->isReady;
  self->isReady = false;
  return tmp;
}

void SoftTimer_start(SoftTimer *self, int ms) {
  SoftTimer_setInterval(self, ms);
  SoftTimer_reset(self);
  self->isActive = true;
}

void SoftTimer_reset(SoftTimer *self) {
  self->timer = clock();
  self->isReady = false;
  self->timerSinceStart = self->timer;
}

void SoftTimer_update(SoftTimer *self) {
  if (!self->isActive)
    return;
  else {
    clock_t currentTime = clock();
    if (currentTime - self->timer >= self->interval) {
      self->isReady = true;
      self->timer = currentTime;
      if (self->singleShot)
        self->isActive = false;
      if (self->callback)
        self->callback(self->callbackContext);
    }
  }
}

int SoftTimer_elapsed(const SoftTimer *self) {
  int tmp = SoftTimer_isActive(self) ? clock() - self->timer : 0;
  tmp = tmp < 0 ? ~tmp : tmp;
  return clockToMs_(tmp);
}

int SoftTimer_timeSinceStart(const SoftTimer *self) {
  int tmp = SoftTimer_isActive(self) ? clock() - self->timerSinceStart : 0;
  tmp = tmp < 0 ? ~tmp : tmp;
  return clockToMs_(tmp);
}
