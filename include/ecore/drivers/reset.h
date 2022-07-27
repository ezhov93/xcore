#ifndef RESET_H
#define RESET_H

#include <stdint.h>

typedef enum {
  ResetError_Success,
  ResetError_FunctionNotSupported,
  ResetError_InvalidValue,
} ResetError;

typedef enum {
  ResetReason_PowerOnBoot,
  ResetReason_WarmBoot,
  ResetReason_ColdBoot,
  ResetReason_Watchdog,
  ResetReason_BusTimeout,
  ResetReason_Pmic,
  ResetReason_BrownOut,
  ResetReason_Other,
} ResetReason;

typedef enum { ResetBootFlag_Warm, ResetBootFlag_Cold } ResetBootFlag;

void Reset_reboot(ResetBootFlag resetBootFlag);
int Reset_shutdown();
int Reset_getResetReason(ResetReason resetReason);

#endif // RESET_H
