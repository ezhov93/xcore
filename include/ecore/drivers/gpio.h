#ifndef ECORE_DRIVERS_GPIO_H
#define ECORE_DRIVERS_GPIO_H

#include <stdbool.h>

typedef enum {
  GpioError_Success,
  GpioError_InvalidValue,
  GpioError_ReadFailed,
  GpioError_WriteFailed,
  GpioError_FunctionNotSupported
} GpioError;

typedef enum { GpioDirection_Input, GpioDirection_Output } GpioDirection;

typedef enum {
  GpioOutputMode_OpenDrain,
  GpioOutputMode_PushPull
} GpioOutputMode;

typedef enum { GpioPull_None, GpioPull_Up, GpioPull_Down } GpioPull;

typedef enum {
  GpioInterrupt_None,
  GpioInterrupt_Rising,
  GpioInterrupt_Falling,
  GpioInterrupt_Edge,
  GpioInterrupt_Low,
  GpioInterrupt_High
} GpioInterrupt;

typedef enum {
  GpioIoctlRequest_SetFunction,
  GpioIoctlRequest_SetDirection,
  GpioIoctlRequest_SetPull,
  GpioIoctlRequest_SetOutputMode,
  GpioIoctlRequest_SetInterrupt,
  GpioIoctlRequest_SetSpeed,
  GpioIoctlRequest_SetDriveStrength,
  GpioIoctlRequest_GetFunction,
  GpioIoctlRequest_GetDirection,
  GpioIoctlRequest_GetPull,
  GpioIoctlRequest_GetOutputType,
  GpioIoctlRequest_GetInterrupt,
  GpioIoctlRequest_GetSpeed,
  GpioIoctlRequest_GetDriveStrength
} GpioIoctlRequest;

struct GpioPrivate;
typedef struct GpioPrivate *Gpio;
typedef void (*GpioCallback)(bool state, void *context);

Gpio Gpio_open(int instance);
void Gpio_setCallback(Gpio const self, GpioCallback callback, void *context);
int Gpio_readSync(Gpio const self, bool *state);
int Gpio_writeSync(Gpio const self, bool state);
int Gpio_close(Gpio const self);
int Gpio_ioctl(Gpio const self, GpioIoctlRequest request, void *const buffer);

#endif // ECORE_DRIVERS_GPIO_H
