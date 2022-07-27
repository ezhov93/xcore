#ifndef ECORE_DRIVERS_POWER_H
#define ECORE_DRIVERS_POWER_H

#include <stdbool.h>
#include <stdint.h>

typedef enum {
  PowerError_Success,
  PowerError_InvalidValue,
  PowerError_NotInitialized,
  PowerError_GetFailed,
  PowerError_SetFailed,
  PowerError_FunctionNotSupported
} PowerError;

struct PowerPrivate;
typedef struct PowerPrivate *Power;

typedef enum {
  PowerMode_HighPowerPerfMode,
  PowerMode_NormalPowerPerfMode,
  PowerMode_LowPowerPerfMode,
  PowerMode_UnknownPowerMode = 0xffff
} PowerMode;

typedef enum {
  PowerIdleMode_NoneMode,
  PowerIdleMode_PCMode,
  PowerIdleMode_ClkSrcOffModeMode,
  PowerIdleMode_VddOffMode
} PowerIdleMode;

typedef struct PowerWakeupSources {
  unsigned char *wakeupSources;
  uint16_t length;
} PowerWakeupSources;

typedef enum PowerIoctlRequest {
  PowerIoctlRequest_SetPCWakeThreshold,
  PowerIoctlRequest_GetPCWakeThreshold,
  PowerIoctlRequest_SetClkSrcOffWakeThreshold,
  PowerIoctlRequest_GetClkSrcOffWakeThreshold,
  PowerIoctlRequest_SetVddOfffWakeThreshold,
  PowerIoctlRequest_GetVddOfffWakeThreshold,
  PowerIoctlRequest_SetWakeupSources,
  PowerIoctlRequest_GetWakeupSources,
  PowerIoctlRequest_GetLastIdleMode
} PowerIoctlRequest;

typedef void (*PowerCallback)(bool idleState, void *context);

Power Power_open();
int Power_setMode(Power const self, PowerMode mode);
int Power_resetMode(Power const self);
void Power_setCallback(Power const self, PowerCallback callback, void *context);
int Power_ioctl(Power const self, PowerIoctlRequest request,
                void *const buffer);
int Power_getMode(Power const self, PowerMode *mode);
int Power_close(Power const self);

#endif // ECORE_DRIVERS_POWER_H
