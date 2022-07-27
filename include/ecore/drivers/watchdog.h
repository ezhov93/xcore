#ifndef ECORE_DRIVERS_WATCHDOG_H
#define ECORE_DRIVERS_WATCHDOG_H

typedef enum {
  WatchdogError_Sucess,
  WatchdogError_InvalidValue,
  WatchdogError_TimeNotSet,
  WatchdogError_FunctionNotSupported
} WatchdogError;

typedef enum {
  WatchdogStatus_Stopped,
  WatchdogStatus_Running,
  WatchdogStatus_BarkExpired,
  WatchdogStatus_BiteExpired
} WatchdogStatus;

typedef enum {
  WatchdogBiteConfig_Reset,
  WatchdogBiteConfig_Interrupt
} WatchdogBiteConfig;

struct WatchdogPrivate;
typedef struct WatchdogPrivate *Watchdog;

typedef enum {
  WatchdogIoctlRequest_SetWatchdogBarkTime,
  WatchdogIoctlRequest_GetWatchdogBarkTime,
  WatchdogIoctlRequest_SetWatchdogBiteTime,
  WatchdogIoctlRequest_GetWatchdogBiteTime,
  WatchdogIoctlRequest_GetWatchdogStatus,
  WatchdogIoctlRequest_SetWatchdogBiteBehaviour
} WatchdogIoctlRequest;

typedef void (*WatchdogCallback)(void *context);

Watchdog Watchdog_open(int instance);
int Watchdog_start(Watchdog const self);
int Watchdog_stop(Watchdog const self);
int Watchdog_restart(Watchdog const self);
void Watchdog_setCallback(Watchdog const self, WatchdogCallback callback,
                          void *context);
int Watchdog_ioctl(Watchdog const self, WatchdogIoctlRequest xRequest,
                   void *const buffer);
int Watchdog_close(Watchdog const self);

#endif // ECORE_DRIVERS_WATCHDOG_H
