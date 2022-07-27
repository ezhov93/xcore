#ifndef ECORE_DRIVERS_I2S_H
#define ECORE_DRIVERS_I2S_H

#include <stdlib.h>

typedef enum {
  I2SError_Success = 0,
  I2SError_InvalidValue = 1,
  I2SError_NotInitialized = 2,
  I2SError_Busy = 3,
  I2SError_WriteFailed = 4,
  I2SError_ReadFailed = 5,
  I2SError_NothingToCancel = 7,
  I2SError_FunctionNotSupported = 8
} I2SError;

typedef enum {
  I2SBusStatus_Idle = 0,
  I2SBusStatus_Busy = I2SError_Busy,
} I2SBusStatus;

typedef enum {
  I2SOperationStatus_Completed = I2SError_Success,
  I2SOperationStatus_LastWriteFailed = I2SError_WriteFailed,
  I2SOperationStatus_LastReadFailed = I2SError_ReadFailed
} I2SOperationStatus;

typedef enum {
  I2SIoctlRequest_SetConfig,
  I2SIoctlRequest_GetConfig,
  I2SIoctlRequest_GetBusState,
  I2SIoctlRequest_GetTxNoOfbytes,
  I2SIoctlRequest_GetRxNoOfbytes,
} I2SIoctlRequest;

typedef enum {
  I2SMode_NormalMode,
  I2SMode_LeftJustifiedMode,
  I2SMode_RightJustifiedMode,
  I2SMode_PcmMode,
  I2SMode_DspMode,
} I2SMode;

typedef enum {
  I2SChannel_Stereo,
  I2SChannel_Mono,
} I2SChannel;

typedef enum {
  I2SClkPolarity_FallingEdge,
  I2SClkPolarity_RisingEdge,
} I2SClkPolarity;

typedef struct {
  I2SMode mode;
  I2SChannel channel;
  I2SClkPolarity sckPolarity;
  I2SClkPolarity wsPolarity;
  unsigned int frequency;
  unsigned int dataLength;
  unsigned int frameLength;
} I2SIoctlConfig;

struct I2SPrivate;

typedef struct I2SPrivate *I2S;

typedef void (*I2SCallback)(I2SOperationStatus opStatus, void *context);

I2S I2S_open(int instance);
void I2S_setCallback(I2S const self, I2SCallback callback, void *context);
int I2S_readAsync(I2S const self, void *const buffer, size_t bytes);
int I2S_writeAsync(I2S const self, void *const buffer, size_t bytes);
int I2Sread_sync(I2S const self, void *const buffer, size_t bytes);
int I2S_writeSync(I2S const self, void *const buffer, size_t bytes);
int I2S_close(I2S const self);
int I2S_ioctl(I2S const self, I2SIoctlRequest request, void *const buffer);
int I2S_cancel(I2S const self);

#endif // ECORE_DRIVERS_I2S_H
