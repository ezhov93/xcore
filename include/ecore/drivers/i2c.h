#ifndef I2CH
#define I2CH

#include <stddef.h>
#include <stdint.h>

#define I2C_STANDARD_MODE_BPS (100000)
#define I2C_FAST_MODE_BPS (400000)
#define I2C_FAST_MODE_PLUS_BPS (1000000)
#define ECORE_DRIVERS_I2C_HIGH_SPEED_BPS (3400000)

typedef enum {
  I2CError_Success,
  I2CError_InvalidValue,
  I2CError_Busy,
  I2CError_WriteFailed,
  I2CError_ReadFailed,
  I2CError_Nack,
  I2CError_BusTimeout,
  I2CError_NoThingToCancel,
  I2CError_FunctionNotSsupported,
  I2CError_SlaveAddressNotSet
} I2CError;

typedef enum {
  I2CBusStatus_Idle = 0,
  I2CBusStatus_Busy = I2CError_Busy,
} I2CBusStatus;

typedef enum {
  I2COperationStatus_Completed = I2CError_Success,
  I2COperationStatus_DriverFailed,
  I2COperationStatus_NackFromSlave = I2CError_Nack,
  I2COperationStatus_MasterTimeout = I2CError_BusTimeout
} I2COperationStatus;

typedef struct {
  unsigned int masterTimeout;
  unsigned int freq;
} I2CConfig;

typedef enum {
  I2CIoctlRequest_SendNoStopFlag,
  I2CIoctlRequest_SetSlaveAddr,
  I2CIoctlRequest_SetMasterConfig,
  I2CIoctlRequest_GetMasterConfig,
  I2CIoctlRequest_GetBusState,
  I2CIoctlRequest_BusReset,
  I2CIoctlRequest_GetTxNoOfbytes,
  I2CIoctlRequest_GetRxNoOfbytes,
} I2CIoctlRequest;

struct I2CPrivate;

typedef struct I2CPrivate *I2C;
typedef void (*I2CCallback)(I2COperationStatus status, void *constext);

I2C I2C_open(int instance);

void I2C_setCallback(I2C const self, I2CCallback callback, void *context);
int I2C_readSync(I2C const self, void *const buffer, size_t bytes);
int I2C_writeSync(I2C const self, void *const buffer, size_t bytes);
int I2C_readAsync(I2C const self, void *const buffer, size_t bytes);
int I2C_writeAsync(I2C const self, void *const buffer, size_t bytes);
int I2C_ioctl(I2C const self, I2CIoctlRequest request, void *const buffer);
int I2C_close(I2C const self);
int I2C_cancel(I2C const self);

#endif // ECORE_DRIVERS_I2C_H
