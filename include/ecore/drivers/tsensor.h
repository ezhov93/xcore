#ifndef ECORE_DRIVERS_TSENSOR_H
#define ECORE_DRIVERS_TSENSOR_H

#include <stdint.h>
typedef enum {
  TSensorError_Sucess,
  TSensorError_InvalidValue,
  TSensorError_Disabled,
  TSensorError_Closed,
  TSensorError_GetTempFailed,
  TSensorError_SetTempFailed,
  TSensorError_NotSupported,
} TSensorError;

typedef enum {
  TSensorStatus_MinThresholdReached,
  TSensorStatus_MaxThresholdReached
} TSensorStatus;

typedef enum {
  TSensorIoctlRequest_SetMinThreshold,
  TSensorIoctlRequest_SetMaxThreshold,
  TSensorIoctlRequest_GetMinThreshold,
  TSensorIoctlRequest_GetMaxThreshold,
  TSensorIoctlRequest_PerformCalibration
} TSensorIoctlRequest;

struct TSensorPrivate;
typedef struct TSensorPrivate *TSensor;

typedef void (*TSensorCallback)(TSensorStatus status, void *context);

TSensor TSensor_open(int instance);
void TSensor_set_callback(TSensor const self, TSensorCallback callback,
                          void *context);
int TSensor_enable(TSensor const self);
int TSensor_disable(TSensor const self);
int TSensor_getTemp(TSensor const self, int *temp);
int TSensor_ioctl(TSensor const self, TSensorIoctlRequest request,
                  void *const pvBuffer);
int TSensor_close(TSensor const self);

#endif // ECORE_DRIVERS_TSENSOR_H
