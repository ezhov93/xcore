#include <ecore/waitcounter.h>

void WaitCounter_init(WaitCounter *self, int maxCount) {
  assert(maxCount > 0);
  self->count = 0;
  self->maxCount = maxCount;
}

bool WaitCounter_take(WaitCounter *self, clock_t clocks) {
  clock_t startClock = clock();
  bool ret = false;
  do {
    if (self->count > 0) {
      self->count--;
      ret = true;
      break;
    }
  } while (clock() - startClock < clocks);
  return ret;
}
