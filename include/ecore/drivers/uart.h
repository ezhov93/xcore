#ifndef ECORE_DRIVERS_UART_H
#define ECORE_DRIVERS_UART_H

#include <stddef.h>
#include <stdint.h>

#define UARTBAUD_RATE_DEFAULT (115200)

typedef enum {
  UartError_Success,
  UartError_InvalidValue,
  UartError_WriteFailed,
  UartError_ReadFailed,
  UartError_Busy,
  UartError_NothingToCancel,
  UartError_FunctionNotSupported,
} UartError;

typedef enum {
  UartOperationStatus_WriteCompleted,
  UartOperationStatus_ReadCompleted,
  UartOperationStatus_LastWriteFailed,
  UartOperationStatus_LastReadFailed
} UartOperationStatus;

typedef enum { UartParityNone, UartParityOdd, UartParityEven } UartParity;

typedef enum { UartStopBitsOne, UartStopBitsTwo } UartStopBits;

typedef void (*UartCallback)(UartOperationStatus status, void *context);

struct UartPrivate;

typedef struct UartPrivate *Uart;

typedef enum {
  UartIoctlRequest_SetConfig,
  UartIoctlRequest_GetConfig,
  UartIoctlRequest_GetTxNoOfbytes,
  UartIoctlRequest_GetRxNoOfbytes
} UartIoctlRequest;

typedef struct {
  unsigned int baudrate;
  UartParity parity;
  UartStopBits stopbits;
  unsigned char wordlength;
  unsigned char flowControl;
} UartConfig;

Uart Uart_open(int instance);
void Uart_setCallback(Uart const self, UartCallback callback, void *context);
int Uart_readSync(Uart const self, void *const buffer, size_t bytes);
int Uart_writeSync(Uart const self, void *const buffer, size_t bytes);
int Uart_readAsync(Uart const self, void *const buffer, size_t bytes);
int Uart_writeAsync(Uart const self, void *const buffer, size_t bytes);
int Uart_ioctl(Uart const self, UartIoctlRequest request, void *const buffer);
int Uart_cancel(Uart const self);
int Uart_close(Uart const self);

#endif // ECORE_DRIVERS_UART_H
