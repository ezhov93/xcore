#ifndef ECORE_DRIVERS_FLASH_H
#define ECORE_DRIVERS_FLASH_H

#include <stdlib.h>

typedef enum {
  FlashError_Success,
  FlashError_InvalidValue,
  FlashError_WriteFailed,
  FlashError_ReadFailed,
  FlashError_EraseFailed,
  FlashError_DeviceBusy,
  FlashError_CtrlOpFailed,
  FlashError_FunctionNotSupported
} FlashError;

typedef enum {
  FlashStatus_Idle,
  FlashStatus_CmdInProgress,
  FlashStatus_EraseFailed,
  FlashStatus_ProgramSuspended,
  FlashStatus_EraseSuspended
} FlashStatus;

typedef enum {
  FlashOperationStatus_Completed = FlashError_Success,
  FlashOperationStatus_EraseFailed = FlashError_EraseFailed,
  FlashOperationStatus_LastReadFailed = FlashError_ReadFailed,
  FlashOperationStatus_LastWriteFailed = FlashError_WriteFailed,
} FlashOperationStatus;

typedef enum {
  FlashWriteProtect_ReadWrite,
  FlashWriteProtect_ReadOnly
} FlashWriteProtect;

typedef struct {
  unsigned int flashSize;
  unsigned int blockSize;
  unsigned int sectorSize;
  unsigned int pageSize;
  unsigned int lockSupportSize;
  unsigned char asyncSupported;
  unsigned int flashID;
} FlashInfo;

typedef struct {
  unsigned int address;
  unsigned int size;
  FlashWriteProtect protectionLevel;
} FlashWriteProtectConfig;

typedef enum {
  FlashIoctlRequest_SetFlashBlockProtection,
  FlashIoctlRequest_GetFlashBlockProtection,
  FlashIoctlRequest_SuspendFlashProgramErase,
  FlashIoctlRequest_ResumeFlashProgramErase,
  FlashIoctlRequest_GetFlashStatus,
  FlashIoctlRequest_GetFlashTxNoOfbytes,
  FlashIoctlRequest_GetFlashRxNoOfbytes,
} FlashIoctlRequest;

struct FlashPrivate;
typedef struct FlashPrivate *Flash;

typedef void (*FlashCallback)(FlashOperationStatus status, void *context);

Flash Flash_open(int instance);
FlashInfo *Flash_getInfo(Flash const self);
void Flash_setCallback(Flash const self, FlashCallback calback, void *context);
int Flash_ioctl(Flash const self, FlashIoctlRequest request,
                void *const buffer);
int Flash_eraseSectors(Flash const self, unsigned int startAddress,
                       size_t size);
int Flash_eraseChip(Flash const self);
int Flash_writeSync(Flash const self, unsigned int address, void *const buffer,
                    size_t bytes);
int Flash_readSync(Flash const self, unsigned int address, void *const buffer,
                   size_t bytes);
int Flash_writeAsync(Flash const self, unsigned int address, void *const buffer,
                     size_t bytes);
int Flash_readAsync(Flash const self, unsigned int address, void *const buffer,
                    size_t bytes);
int Flash_close(Flash const self);

#endif // ECORE_DRIVERS_FLASH_H
