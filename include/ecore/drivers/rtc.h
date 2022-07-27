#ifndef ECORE_DRIVERS_RTC_H
#define ECORE_DRIVERS_RTC_H

#include <stdint.h>

typedef enum {
  RtcError_Success,
  RtcError_InvaludValue,
  RtcError_NotStarted,
  RtcError_GetFailed,
  RtcError_SetFailed,
  RtcError_FunctionNotSupported
} RtcError;

typedef enum {
  RtcStatus_TimerStopped,
  RtcStatus_TimerRunning,
  RtcStatus_TimerAlarmTriggered,
  RtcStatus_TimerWakeupTriggered
} RtcStatus;

typedef struct {
  unsigned char second;
  unsigned char minute;
  unsigned char hour;
  unsigned char day;
  unsigned char month;
  uint16_t year;
  unsigned char wDay;
} RtcDatetime;

typedef enum RtcIoctlRequest {
  RtcIoctlRequest_SetRtcAlarm,
  RtcIoctlRequest_GetRtcAlarm,
  RtcIoctlRequest_CancelRtcAlarm,
  RtcIoctlRequest_SetRtcWakeupTime,
  RtcIoctlRequest_GetRtcWakeupTime,
  RtcIoctlRequest_CancelRtcWakeup,
  RtcIoctlRequest_GetRtcStatus
} RtcIoctlRequest;

struct RtcPrivate;
typedef struct RtcPrivate *Rtc;

typedef void (*RtcCallback)(RtcStatus status, void *constext);

Rtc Rtc_open(int instance);
void Rtc_set_callback(Rtc const self, RtcCallback callback, void *context);
int Rtc_ioctl(Rtc const self, RtcIoctlRequest request, void *const buffer);
int Rtc_set_datetime(Rtc const self, const RtcDatetime *datatime);
int Rtc_get_datetime(Rtc const self, RtcDatetime *datatime);
int Rtc_close(Rtc const self);

#endif // ECORE_DRIVERS_RTC_H
