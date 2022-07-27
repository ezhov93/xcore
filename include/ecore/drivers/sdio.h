#ifndef ECORE_DRIVERS_SDIO_H
#define ECORE_DRIVERS_SDIO_H

#include <stdbool.h>
#include <stdint.h>

typedef enum {
  SdioError_Success = 0,
  SdioError_IoctlFail = 1,
  SdioError_HostInitFail = 2,
  SdioError_InvalidValue = 3,
  SdioError_TransferFail = 4,
  SdioError_SwitchHightSpeedFail = 5,
  SdioError_LowSpeedCard = 6,
  SdioError_SelectBusimingFail = 7,
  SdioError_SetcardBlockSizeFail = 8,
  SdioError_SwitchVoltageFail = 9,
  SdioError_HostNotReady = 10,
  SdioError_InvalidCard = 11,
  SdioError_SendRelativeAddressFail = 12,
  SdioError_SelectCardFail = 13,
  SdioError_ReadCisFail = 14,
  SdioError_SetDataBusWidthFail = 15,
  SdioError_AsyncIntNotSupported = 16,
  SdioError_GetCardCapabilityFail = 17,
  SdioError_FunctionNotSupported = 18
} SdioError;

typedef enum { SdioBus_1bit = 0, SdioBus_4bit = 2, SdioBus_8bit = 3 } SdioBus;

typedef enum SdioDirection {
  SdioDirection_Read = 0U,
  SdioDirection_Write = 1U,
} SdioDirection;

typedef enum SdioFunction {
  SdioFunction_0, /*!< refer to common register area present on all I/O cards*/
  SdioFunction_1, /*!< refer to register area of 1st I/O function if card
                     supports it*/
  SdioFunction_2, /*!< refer to register area of 2nd I/O function if card
                     supports it*/
  SdioFunction_3, /*!< refer to register area of 3rd I/O function if card
                     supports it*/
  SdioFunction_4, /*!< refer to register area of 4th I/O function if card
                     supports it*/
  SdioFunction_5, /*!< refer to register area of 5th I/O function if card
                     supports it*/
  SdioFunction_6, /*!< refer to register area of 6th I/O function if card
                     supports it*/
  SdioFunction_7, /*!< refer to register area of 7th I/O function if card
                     supports it*/
  SdioFunction_Memory, /*!< refer to register area of memory function of a combo
                          card*/
} SdioFunction;

typedef enum SdioCardDetectType {
  SdioCardDetectType_ByGpioCD, /*!< sdio card detect by CD pin through GPIO */
  SdioCardDetectType_ByHostCD, /*!< sdio card detect by CD pin through host */
  SdioCardDetectType_ByHostDATA3, /*!< sdio card detect by DAT3 pin through host
                                   */
  SdioCardDetectType_NonStandard, /*!< sdio card detect by non standard method
                                   *   certain sdio host may support */
} SdioCardDetectType;

typedef enum SdioEventType {
  SdioEventType_InterruptEvent, /*!< Interrupt received from sdio card */
  SdioEventType_InsertedEvent,  /*!< Sdio card insertion is detected */
  SdioEventType_RemovedEvent,   /*!< Sdio card removal is detected */
  SdioEventType_PowerOnEvent,   /*!< Powering on sdio card */
  SdioEventType_PowerOffEvent,  /*!< Powering off sdio card */
} SdioEventType;

typedef enum SdioIoctlRequest {
  SdioIoctlRequest_SetDriverStrength, /*!< Set driver strength type
                                       *   input: SDIO card driver strength.
                                       *          input data type
                                       * SdioSdDriverStrength
                                       */
  SdioIoctlRequest_SetCardInactive,   /*!< put card in inactive state */
  SdioIoctlRequest_SwitchToHighSpeed, /*!< switch to high speed mode if card
                                         supports it */
  SdioIoctlRequest_SetDataBusWidth, /*!< set sdio card data bus width and change
                                     * host setting to support it   input: sdio
                                     * bus width, input data type SdioBusWidth
                                     */
  SdioIoctlRequest_GetCardCapabilities, /*!< request host to learn capabilities
                                         * of specified card function input: I/O
                                         * function number, of type
                                         * SdioFunction*/
  SdioIoctlRequest_SetCardDetectParams, /*!< Set card detect parameters
                                         *   input: card detect parameters.
                                         *          input data type
                                         * SdioCardDetectParam*/
  SdioIoctlRequest_SetFuncBlockSize, /*!< Set transfer block size for a function
                                      *   input: I/O function and block size.
                                      *          input data type
                                      * SdioFuncBlkSize*/
  SdioIoctlRequest_EnableIOFunctionIrq, /*!< enable/disable irq for individual
                                         * io functions input: I/O function
                                         * number and enable or disable, input
                                         * data type SdioPerFuncEnable */
  SdioIoctlRequest_EnableAsyncIrqMode,  /*!< enable/disable asynchronous
                                         * interrupt mode  input: boolean to
                                         * indicate enable or disable */
  SdioIoctlRequest_EnableIo, /*!< enable/disable an I/O function on card
                              *   input: I/O function number and boolean to
                              * indicate enable or disable, input data type
                              * SdioPerFuncEnable
                              */
  SdioIoctlRequest_SelectIo, /*!< Select an I/O function for suspend/resume
                              *   input: I/O function number, input data type
                              * SdioFunction */
  SdioIoctlRequest_AbortIo,  /*!< Abort an ongoing transfer initiated by the
                              * specified I/O  function input: I/O function
                              * number. Input data type  SdioFunction */
  SdioIoctlRequest_ReadCis,  /*!< Host read list of tuples from card CIS
                              *   input: I/O function number and tuple list to
                              * read,  input data type SdioReadCis*/
  SdioIoctlRequest_PowerOnOffCard,    /*!< Power on/off card
                                       *   input: boolean true for power on, false
                                       * for power    off    boolean true to use
                                       * board's default power    method,    false
                                       * to    use user defined power method via
                                       * callback.    input data type
                                       * SdioPowerOnOffCard
                                       */
  SdioIoctlRequest_CheckCardPresence, /*!< check card presence
                                       *   output: boolean to indicate card
                                       * present or not present*/
} SdioIoctlRequest;

typedef enum SdioBusWidth {
  SdioBusWidth_1bit = SdioBus_1bit, /*!< 1 bit bus mode */
  SdioBusWidth_4bit = SdioBus_4bit, /*!< 4 bit bus mode*/
  SdioBusWidth_8bit = SdioBus_8bit, /*!< 8 bit bus mode*/
} SdioBusWidth;

typedef struct SdioCardDetectParam {
  SdioCardDetectType cardDetectType; /*!< One of the Sd card detect types
                                              supported by sdio host */
  bool callbackOnInsert;  /*!< If true sdio callback will be invoked on card
                              insert. */
  bool callbackOnRemoval; /*!< If true sdio callback will be invoked on card
                              card removal. */
} SdioCardDetectParam;

typedef struct SdioPowerOnOffCard {
  bool powerOn;     /*!< True to power on card, false to power off card. */
  bool useCallback; /*!< indicate if user callback should be used to power
                        on/off card. */
} SdioPowerOnOffCard;

typedef struct SdioFuncBlkSize {
  SdioFunction func;      /*!< IO function number range from SdioFunction0 to
                                   SdioFunction7*/
  unsigned int blockSize; /*!< Block size to be set for I/O function. Must not
                           * be greater than the maximum block size supported by
                           * IO function.*/
} SdioFuncBlkSize;

typedef struct SdioPerFuncEnable {
  SdioFunction func; /*!< IO function number range from SdioFunction0 to
                              SdioFunction7 */
  bool enable;       /*!< true to enable, false to disable */
} SdioPerFuncEnable;

typedef struct SdioReadCis {
  SdioFunction func; /*!< IO function number range from SdioFunction0 to
                              SdioFunction7 */
  const unsigned int *tupleList; /*!< Point to a list of tuple codes to be
                                  * read. Tuple code that is not supported by
                                  * sdio card will be ignored. */
  unsigned int tupleNum;         /*!< Number of tuple codes in the list. */
} SdioReadCis;

typedef enum SdioSdDriverStrength {
  SdioSdDriverStrength_TypeB = 0U, /*!< default driver strength (50 ohm x1)*/
  SdioSdDriverStrength_TypeA = 1U, /*!< driver strength TYPE A  (33 ohm x1.5)*/
  SdioSdDriverStrength_TypeC = 2U, /*!< driver strength TYPE C  (66 ohm x0.75)*/
  SdioSdDriverStrength_TypeD = 3U, /*!< driver strength TYPE D  (100 ohm x0.5)*/
} SdioSdDriverStrength;

struct SdioSlotPrivate;

typedef struct SdioSlotPrivate *SdioSlot;

typedef void (*SdioCallback)(SdioSlot const self, SdioEventType event,
                             void *context);

typedef void (*SdioIOFunctionCallback)(SdioSlot const self, SdioFunction func,
                                       void *context);

SdioSlot Sdio_open(unsigned char hostIdx, unsigned char slotIdx);

int Sdio_close(SdioSlot const self);
int Sdio_cardConnect(SdioSlot const self);
int Sdio_cardDisconnect(SdioSlot const self);
int Sdio_cardReset(SdioSlot const self);
int Sdio_ioWriteDirect(SdioSlot const self, SdioFunction func,
                       unsigned int regAddr, unsigned char *data, bool raw);
int Sdio_ioReadDirect(SdioSlot const self, SdioFunction func,
                      unsigned int regAddr, unsigned char *data);
int Sdio_ioWriteExtended(SdioSlot const self, SdioFunction func, bool blockMode,
                         bool opcode, unsigned int regAddr, unsigned char *buf,
                         unsigned int count);
int Sdio_ioReadExtended(SdioSlot const self, SdioFunction func, bool blockMode,
                        bool opcode, unsigned int regAddr, unsigned char *buf,
                        unsigned int count);
int Sdio_setIOFunctionCallback(SdioSlot const self, SdioFunction func,
                               SdioIOFunctionCallback callback, void *context);
int Sdio_setSdioCallback(SdioSlot const self, SdioCallback callback,
                         void *context);
int Sdio_ioctl(SdioSlot const self, SdioIoctlRequest request,
               void *const buffer);

#endif /* ECORE_DRIVERS_SDIO_H */
