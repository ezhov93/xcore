#ifndef ECORE_WAITCOUNTER_H
#define ECORE_WAITCOUNTER_H

#include <assert.h>
#include <stdbool.h>
#include <time.h>

static const int WaitCounter_Binary = 1;

typedef struct {
  int count;
  int maxCount;
} WaitCounter;

void WaitCounter_init(WaitCounter *self, int maxCount);

static inline bool WaitCounter_give(WaitCounter *self) {
  if (self->count < self->maxCount) self->count++;
  return true;
}

bool WaitCounter_take(WaitCounter *self, clock_t clocks);

#endif  // ECORE_WAITCOUNTER_H