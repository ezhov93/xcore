#ifndef ECORE_DRIVERS_I2S_IO_INTERFACE_H
#define ECORE_DRIVERS_I2S_IO_INTERFACE_H

#include <stdlib.h>

typedef struct IOInterface {
  int (*open)(void);
  int (*ioctl)(int request, void *const buffer);
  int (*read)(void *const buffer, size_t bytes);
  int (*write)(void *const buffer, size_t bytes);
  int (*close)(void);
} IOInterface;

#endif // ECORE_DRIVERS_I2S_IO_INTERFACE_H
