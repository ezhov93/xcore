#ifndef ECORE_DRIVERS_SPI_H
#define ECORE_DRIVERS_SPI_H

#include <stdlib.h>

typedef enum {
  SpiError_Success,
  SpiError_InvalidValue,
  SpiError_WriteFailed,
  SpiError_ReadFailed,
  SpiError_TransferError,
  SpiError_BusBusy,
  SpiError_NothingToCancel,
  SpiError_FunctionNotSupport
} SpiError;

typedef enum {
  SpiTransactionStatus_Success = SpiError_Success,
  SpiTransactionStatus_WriteError = SpiError_WriteFailed,
  SpiTransactionStatus_ReadError = SpiError_ReadFailed,
  SpiTransactionStatus_TransferError = SpiError_TransferError
} SpiTransactionStatus;

typedef enum {
  SpiMode_0,
  SpiMode_1,
  SpiMode_2,
  SpiMode_3,
} SpiMode;

typedef enum { SpiBitOrder_MSBFirst, SpiBitOrder_LSBFirst } SpiBitOrder;

typedef enum {
  SpiIoctlRequest_SetMasterConfig,
  SpiIoctlRequest_GetMasterConfig,
  SpiIoctlRequest_GetTxNoOfbytes,
  SpiIoctlRequest_GetRxNoOfbytes,
} SpiIoctlRequest;

typedef struct SpiMasterConfig {
  unsigned int freq;
  SpiMode mode;
  SpiBitOrder spiSetBitOrder;
  unsigned char dummyValue;
} SpiMasterConfig;

struct SpiPrivate;
typedef struct SpiPrivate *Spi;

typedef void (*SpiCallback)(SpiTransactionStatus status, void *param);
Spi Spi_open(int instance);
void Spi_setCallback(Spi const self, SpiCallback callback, void *const context);
int Spi_ioctl(Spi const self, SpiIoctlRequest request, void *const buffer);
int Spi_readSync(Spi const self, void *const buffer, size_t bytes);
int Spi_readAsync(Spi const self, void *const buffer, size_t bytes);
int Spi_writeSync(Spi const self, void *const buffer, size_t bytes);
int Spi_writeAsync(Spi const self, void *const buffer, size_t bytes);
int Spi_transferSync(Spi const self, void *const txBuffer, void *const rxBuffer,
                     size_t bytes);
int Spi_transferAsync(Spi const self, void *const txBuffer,
                      void *const rxBuffer, size_t xBytes);
int Spi_close(Spi const self);
int Spi_cancel(Spi const self);
int Spi_selectSlave(int instance, int slave);

#endif // ECORE_DRIVERS_SPI_H
