#ifndef ECORE_DRIVERS_PERFCOUNTER_H
#define ECORE_DRIVERS_PERFCOUNTER_H

#include <stdlib.h>

void PerfCounter_open();
size_t PerfCounter_getValue();
unsigned int PerfCounter_getFrequency();
void PerfCounter_close();

#endif  // ECORE_DRIVERS_PERFCOUNTER_H
