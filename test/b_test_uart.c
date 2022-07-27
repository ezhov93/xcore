#include <ecore/waitcounter.h>
#include <string.h>
#include <time.h>
#include <unistd.h>

#include "ecore/drivers/uart.h"
#include "unity.h"
#include "unity_fixture.h"

static const int _BaudRate = 115200;
static const int _BaudRateDefault = _BaudRate;
static const char *_BaudRateString = "Baudrate: 38400\n";
static const char *_BaudRateDefaultString = "Baudrate: 115200\n";
enum { _BaudRateBufferTestLenght = sizeof(_BaudRateString) - 1 };
enum { _BaudRateBufferDefaultLenght = sizeof(_BaudRateDefaultString) - 1 };
static const char *_BufferString = "Test for UART read-write\n";
enum { _BufferStringLenght = sizeof(_BufferString) - 1 };

static const int _BaudRateFast = 115200;
static const int _BaudRateSlow = 9600;
enum { _BaudSize = 32 };
enum { _BufferStringLenghtLarge = 200 };
static const int _Delay = 1000;
static const int _7bitWordLenght = 7;
static const int _8bitWordLenght = 8;

static const int _DefaultWaitCounterDelay = 1 * CLOCKS_PER_SEC;

static const unsigned char _UartPort = 0;
static const int _UartFlowControl = 0;
static const int _UartParity = 0;
static const int _UartWordLength = 0;
static const int _UartStopBits = 0;

static const UartConfig _SampleConfig1 = {
    .baudrate = _BaudRateFast,
    .flowControl = 0,
    .parity = UartParityNone,
    .stopbits = UartStopBitsOne,
    .wordlength = _7bitWordLenght,
};

static const UartConfig _SampleConfig2 = {
    .baudrate = _BaudRateSlow,
    .flowControl = 1,
    .parity = UartParityOdd,
    .stopbits = UartStopBitsTwo,
    .wordlength = _8bitWordLenght,
};

static WaitCounter _readCompleteWaitCounter;
static WaitCounter _writeCompleteWaitCounter;

static void _readWriteCallback(UartOperationStatus opStatus, void *params) {
  (void)params;

  if (opStatus == UartOperationStatus_ReadCompleted) {
    WaitCounter_give(&_readCompleteWaitCounter);
  } else if (opStatus == UartOperationStatus_WriteCompleted) {
    WaitCounter_give(&_writeCompleteWaitCounter);
  }
}

TEST_GROUP(TestUart);

TEST_SETUP(TestUart) {}

TEST_TEAR_DOWN(TestUart) {}

TEST_GROUP_RUNNER(TestUart) {
  RUN_TEST_CASE(TestUart, UartWriteReadAsyncWithCallback);
  RUN_TEST_CASE(TestUart, UartIoctlFuzzing);
  RUN_TEST_CASE(TestUart, UartReadSyncFuzzing);
  RUN_TEST_CASE(TestUart, UartReadAsyncFuzzing);
  RUN_TEST_CASE(TestUart, UartWriteSyncFuzzing);
  RUN_TEST_CASE(TestUart, UartWriteAsyncFuzzing);
#if (IOT_TEST_COMMON_IO_UART_SUPPORTED_CANCEL == 1)
  RUN_TEST_CASE(TestUart, UartCancel);
#endif
  RUN_TEST_CASE(TestUart, UartIoctlWhenBusy)
  RUN_TEST_CASE(TestUart, UartWriteAsyncReadAsyncLoopbackTest);
  RUN_TEST_CASE(TestUart, UartIoctlGetSet);
  RUN_TEST_CASE(TestUart, UartOpenCloseCancelFuzzing);
}

TEST(TestUart, AssistedUartWriteReadSync) {
  unsigned char buffer[_BaudSize] = {0};
  unsigned char bufferRead[_BaudSize] = {0};

  memcpy(buffer, _BufferString, _BufferStringLenght);
  Uart uart;
  int read, write, close;

  uart = Uart_open(_UartPort);
  TEST_ASSERT_NOT_EQUAL(NULL, uart);

  if (TEST_PROTECT()) {
    write = Uart_writeSync(uart, buffer, _BufferStringLenght);
    TEST_ASSERT_EQUAL(UartError_Success, write);

    read = Uart_readSync(uart, bufferRead, _BufferStringLenght);
    TEST_ASSERT_EQUAL(UartError_Success, read);

    TEST_ASSERT_EQUAL(
        0, strncmp((char *)buffer, (char *)bufferRead, _BufferStringLenght));
  }

  close = Uart_close(uart);
  TEST_ASSERT_EQUAL(UartError_Success, close);
}

TEST(TestUart, UartIoctlGetSet) {
  Uart uart;
  int ioctl, close, transferAmount;
  UartConfig getConfigBuffer, configBuffer, originalConfig;

  uart = Uart_open(_UartPort);
  TEST_ASSERT_NOT_EQUAL(NULL, uart);

  if (TEST_PROTECT()) {
    ioctl = Uart_ioctl(uart, UartIoctlRequest_GetConfig, &originalConfig);
    TEST_ASSERT_EQUAL(UartError_Success, ioctl);

    memcpy(&configBuffer, &_SampleConfig1, sizeof(UartConfig));
    ioctl = Uart_ioctl(uart, UartIoctlRequest_SetConfig, &configBuffer);
    TEST_ASSERT_EQUAL(UartError_Success, ioctl);

    configBuffer.baudrate = _SampleConfig2.baudrate;
    ioctl = Uart_ioctl(uart, UartIoctlRequest_SetConfig, &configBuffer);
    TEST_ASSERT_EQUAL(UartError_Success, ioctl);

    ioctl = Uart_ioctl(uart, UartIoctlRequest_GetConfig, &getConfigBuffer);
    TEST_ASSERT_EQUAL(UartError_Success, ioctl);
    TEST_ASSERT_EQUAL(_SampleConfig2.baudrate, getConfigBuffer.baudrate);

    if (_UartFlowControl) {
      configBuffer.flowControl = _SampleConfig2.flowControl;
      ioctl = Uart_ioctl(uart, UartIoctlRequest_SetConfig, &configBuffer);
      TEST_ASSERT_EQUAL(UartError_Success, ioctl);

      ioctl = Uart_ioctl(uart, UartIoctlRequest_GetConfig, &getConfigBuffer);
      TEST_ASSERT_EQUAL(UartError_Success, ioctl);
      TEST_ASSERT_EQUAL(_SampleConfig2.flowControl,
                        getConfigBuffer.flowControl);
    }

    configBuffer.stopbits = _SampleConfig2.stopbits;
    ioctl = Uart_ioctl(uart, UartIoctlRequest_SetConfig, &configBuffer);
    TEST_ASSERT_EQUAL(UartError_Success, ioctl);

    ioctl = Uart_ioctl(uart, UartIoctlRequest_GetConfig, &getConfigBuffer);
    TEST_ASSERT_EQUAL(UartError_Success, ioctl);
    TEST_ASSERT_EQUAL(_SampleConfig2.stopbits, getConfigBuffer.stopbits);

    configBuffer.parity = _SampleConfig2.parity;
    ioctl = Uart_ioctl(uart, UartIoctlRequest_SetConfig, &configBuffer);
    TEST_ASSERT_EQUAL(UartError_Success, ioctl);

    ioctl = Uart_ioctl(uart, UartIoctlRequest_GetConfig, &getConfigBuffer);
    TEST_ASSERT_EQUAL(UartError_Success, ioctl);
    TEST_ASSERT_EQUAL(_SampleConfig2.parity, getConfigBuffer.parity);

    configBuffer.wordlength = _SampleConfig2.wordlength;
    ioctl = Uart_ioctl(uart, UartIoctlRequest_SetConfig, &configBuffer);
    TEST_ASSERT_EQUAL(UartError_Success, ioctl);

    ioctl = Uart_ioctl(uart, UartIoctlRequest_GetConfig, &getConfigBuffer);
    TEST_ASSERT_EQUAL(UartError_Success, ioctl);
    TEST_ASSERT_EQUAL(_SampleConfig2.wordlength, getConfigBuffer.wordlength);

    ioctl = Uart_ioctl(uart, UartIoctlRequest_SetConfig, &originalConfig);
    TEST_ASSERT_EQUAL(UartError_Success, ioctl);

    ioctl = Uart_ioctl(uart, UartIoctlRequest_GetTxNoOfbytes, &transferAmount);
    TEST_ASSERT_EQUAL(UartError_Success, ioctl);

    ioctl = Uart_ioctl(uart, UartIoctlRequest_GetRxNoOfbytes, &transferAmount);
    TEST_ASSERT_EQUAL(UartError_Success, ioctl);
  }

  close = Uart_close(uart);
  TEST_ASSERT_EQUAL(UartError_Success, close);
}

TEST(TestUart, AssistedUartBaudChange) {
  Uart uart;
  int ioctl, write, read, close;
  unsigned char buffer[_BaudSize] = {0};
  unsigned char bufferRead[_BaudSize] = {0};

  memcpy(buffer, _BaudRateString, _BaudRateBufferTestLenght);

  uart = Uart_open(_UartPort);
  TEST_ASSERT_NOT_EQUAL(NULL, uart);

  if (TEST_PROTECT()) {
    write = Uart_writeSync(uart, buffer, _BaudRateBufferTestLenght);
    TEST_ASSERT_EQUAL(UartError_Success, write);

    UartConfig configBuffer;
    ioctl = Uart_ioctl(uart, UartIoctlRequest_GetConfig, &configBuffer);
    TEST_ASSERT_EQUAL(UartError_Success, ioctl);

    configBuffer.baudrate = _BaudRate;

    ioctl = Uart_ioctl(uart, UartIoctlRequest_SetConfig, &configBuffer);
    TEST_ASSERT_EQUAL(UartError_Success, ioctl);

    sleep(1);

    memset(buffer, 0, _BaudSize);
    memcpy(buffer, _BufferString, _BufferStringLenght);

    write = Uart_writeSync(uart, buffer, _BufferStringLenght);
    TEST_ASSERT_EQUAL(UartError_Success, write);

    read = Uart_readSync(uart, bufferRead, _BufferStringLenght);
    TEST_ASSERT_EQUAL(UartError_Success, read);

    TEST_ASSERT_EQUAL(
        0, strncmp((char *)buffer, (char *)bufferRead, _BufferStringLenght));

    memcpy(buffer, _BaudRateDefaultString, _BaudRateBufferDefaultLenght);

    write = Uart_writeSync(uart, buffer, _BaudRateBufferDefaultLenght);
    TEST_ASSERT_EQUAL(UartError_Success, write);

    configBuffer.baudrate = _BaudRateDefault;

    ioctl = Uart_ioctl(uart, UartIoctlRequest_SetConfig, &configBuffer);
    TEST_ASSERT_EQUAL(UartError_Success, ioctl);

    sleep(1);

    memcpy(buffer, _BufferString, _BufferStringLenght);

    write = Uart_writeSync(uart, buffer, _BufferStringLenght);
    TEST_ASSERT_EQUAL(UartError_Success, write);

    read = Uart_readSync(uart, bufferRead, _BufferStringLenght);
    TEST_ASSERT_EQUAL(UartError_Success, read);

    TEST_ASSERT_EQUAL(
        0, strncmp((char *)buffer, (char *)bufferRead, _BufferStringLenght));
  }

  close = Uart_close(uart);
  TEST_ASSERT_EQUAL(UartError_Success, close);
}

TEST(TestUart, UartWriteAsyncReadAsyncLoopbackTest) {
  Uart uart;
  int ioctl, write, close, read, transferAmount;

  unsigned char buffer[_BufferStringLenght + 1] = {0};
  unsigned char bufferRead[_BufferStringLenght + 1] = {0};
  unsigned char uStringCompare = 1;
  bool callbackReturn;

  memcpy(buffer, _BufferString, _BufferStringLenght);

  uart = Uart_open(_UartPort);
  TEST_ASSERT_NOT_EQUAL(NULL, uart);

  if (TEST_PROTECT()) {
    Uart_setCallback(uart, _readWriteCallback, NULL);

    read = Uart_readSync(uart, bufferRead, _BufferStringLenght);
    TEST_ASSERT_EQUAL(UartError_Success, read);

    write = Uart_writeAsync(uart, buffer, _BufferStringLenght);
    TEST_ASSERT_EQUAL(UartError_Success, write);

    callbackReturn =
        WaitCounter_take(&_writeCompleteWaitCounter, _DefaultWaitCounterDelay);
    TEST_ASSERT_EQUAL(true, callbackReturn);

    callbackReturn =
        WaitCounter_take(&_readCompleteWaitCounter, _DefaultWaitCounterDelay);
    TEST_ASSERT_EQUAL(true, callbackReturn);

    ioctl = Uart_ioctl(uart, UartIoctlRequest_GetTxNoOfbytes, &transferAmount);
    TEST_ASSERT_EQUAL(UartError_Success, ioctl);
    TEST_ASSERT_GREATER_THAN(0, transferAmount);

    uStringCompare = memcmp(buffer, bufferRead, _BufferStringLenght);
    TEST_ASSERT_EQUAL(0, uStringCompare);
  }

  close = Uart_close(uart);
  TEST_ASSERT_EQUAL(UartError_Success, close);
}

TEST(TestUart, AssistedUartWriteAsync) {
  Uart uart;
  int ioctl, write, close, transferAmount_1, transferAmount_2;
  unsigned char i;
  unsigned char bufferLarge[_BufferStringLenghtLarge] = {0};

  for (i = 0; i < _BufferStringLenghtLarge; i++) {
    bufferLarge[i] = 0xAA;
  }

  bufferLarge[_BufferStringLenghtLarge - 1] = '\n';

  uart = Uart_open(_UartPort);
  TEST_ASSERT_NOT_EQUAL(NULL, uart);

  if (TEST_PROTECT()) {
    write = Uart_writeAsync(uart, bufferLarge, _BufferStringLenghtLarge);
    TEST_ASSERT_EQUAL(UartError_Success, write);

    ioctl =
        Uart_ioctl(uart, UartIoctlRequest_GetTxNoOfbytes, &transferAmount_1);
    TEST_ASSERT_EQUAL(UartError_Success, ioctl);
    TEST_ASSERT_GREATER_THAN(0, transferAmount_1);

    sleep(1);

    ioctl =
        Uart_ioctl(uart, UartIoctlRequest_GetTxNoOfbytes, &transferAmount_2);
    TEST_ASSERT_EQUAL(UartError_Success, ioctl);
    TEST_ASSERT_GREATER_THAN(transferAmount_1, transferAmount_2);

    sleep(1);
  }

  close = Uart_close(uart);
  TEST_ASSERT_EQUAL(UartError_Success, close);
}

TEST(TestUart, UartWriteReadAsyncWithCallback) {
  Uart uart;
  unsigned char buffer[_BaudSize] = {0};
  unsigned char bufferRead[_BaudSize] = {0};

  memcpy(buffer, _BufferString, _BufferStringLenght);
  int read, write, close;
  bool callbackReturn;
  unsigned char port = _UartPort;

  uart = Uart_open(port);
  TEST_ASSERT_NOT_EQUAL(NULL, uart);

  if (TEST_PROTECT()) {
    Uart_setCallback(uart, _readWriteCallback, NULL);

    read = Uart_readSync(uart, bufferRead, _BufferStringLenght);
    TEST_ASSERT_EQUAL(UartError_Success, read);

    write = Uart_writeAsync(uart, buffer, _BufferStringLenght);
    TEST_ASSERT_EQUAL(UartError_Success, write);

    callbackReturn =
        WaitCounter_take(&_writeCompleteWaitCounter, _DefaultWaitCounterDelay);
    TEST_ASSERT_EQUAL(true, callbackReturn);

    callbackReturn =
        WaitCounter_take(&_readCompleteWaitCounter, _DefaultWaitCounterDelay);
    TEST_ASSERT_EQUAL(true, callbackReturn);

    TEST_ASSERT_EQUAL(
        0, strncmp((char *)buffer, (char *)bufferRead, _BufferStringLenght));
  }

  close = Uart_close(uart);
  TEST_ASSERT_EQUAL(UartError_Success, close);
}

TEST(TestUart, UartCancel) {
  Uart uart;
  int write, close, cancel;
  bool callbackReturn;

  unsigned char smallBuf[2] = {'H', 'I'};
  unsigned char buffer[_BaudSize] = {0};
  unsigned char smallBuflen = sizeof(smallBuf) / sizeof(unsigned char);

  memcpy(buffer, _BufferString, _BufferStringLenght);

  uart = Uart_open(_UartPort);
  TEST_ASSERT_NOT_EQUAL(NULL, uart);

  if (TEST_PROTECT()) {
    Uart_setCallback(uart, _readWriteCallback, NULL);

    write = Uart_writeAsync(uart, buffer, _BufferStringLenght);
    TEST_ASSERT_EQUAL(UartError_Success, write);

    cancel = Uart_cancel(uart);
    TEST_ASSERT_EQUAL(UartError_Success, cancel);

    /* Wait to make sure operation was really canceled. */
    callbackReturn =
        WaitCounter_take(&_writeCompleteWaitCounter, _DefaultWaitCounterDelay);
    TEST_ASSERT_EQUAL(true, callbackReturn);

    write = Uart_writeAsync(uart, smallBuf, smallBuflen);
    TEST_ASSERT_EQUAL(UartError_Success, write);

    /* Wait to make sure operation has completed. */
    callbackReturn =
        WaitCounter_take(&_writeCompleteWaitCounter, _DefaultWaitCounterDelay);
    TEST_ASSERT_EQUAL(true, callbackReturn);

    /* Since  smallBuf is small buffer, write is aready complete. Hence
     * nothing to cancel */
    cancel = Uart_cancel(uart);
    TEST_ASSERT_EQUAL(UartError_NothingToCancel, cancel);
  }

  close = Uart_close(uart);
  TEST_ASSERT_EQUAL(UartError_Success, close);
}

TEST(TestUart, UartWriteAsyncFuzzing) {
  Uart uart;
  int close, write;

  unsigned char buffer[_BaudSize] = {0};

  memcpy(buffer, _BufferString, _BufferStringLenght);

  write = Uart_writeAsync(NULL, buffer, _BufferStringLenght);
  TEST_ASSERT_EQUAL(UartError_InvalidValue, write);

  uart = Uart_open(_UartPort);
  TEST_ASSERT_NOT_EQUAL(NULL, uart);

  if (TEST_PROTECT()) {
    write = Uart_writeAsync(uart, NULL, _BufferStringLenght);
    TEST_ASSERT_EQUAL(UartError_InvalidValue, write);

    write = Uart_writeAsync(uart, buffer, 0);
    TEST_ASSERT_EQUAL(UartError_InvalidValue, write);
  }

  close = Uart_close(uart);
  TEST_ASSERT_EQUAL(UartError_Success, close);
}

TEST(TestUart, UartWriteSyncFuzzing) {
  Uart uart;
  int close, write;

  unsigned char buffer[_BaudSize] = {0};

  memcpy(buffer, _BufferString, _BufferStringLenght);

  write = Uart_writeSync(NULL, buffer, _BufferStringLenght);
  TEST_ASSERT_EQUAL(UartError_InvalidValue, write);

  uart = Uart_open(_UartPort);
  TEST_ASSERT_NOT_EQUAL(NULL, uart);

  if (TEST_PROTECT()) {
    write = Uart_writeSync(uart, NULL, _BufferStringLenght);
    TEST_ASSERT_EQUAL(UartError_InvalidValue, write);

    write = Uart_writeSync(uart, buffer, 0);
    TEST_ASSERT_EQUAL(UartError_InvalidValue, write);
  }

  close = Uart_close(uart);
  TEST_ASSERT_EQUAL(UartError_Success, close);
}

TEST(TestUart, UartReadAsyncFuzzing) {
  Uart uart;
  int close, read;

  unsigned char bufferRead[_BaudSize] = {0};

  read = Uart_readSync(NULL, bufferRead, _BufferStringLenght);
  TEST_ASSERT_EQUAL(UartError_InvalidValue, read);

  uart = Uart_open(_UartPort);
  TEST_ASSERT_NOT_EQUAL(NULL, uart);

  if (TEST_PROTECT()) {
    read = Uart_readSync(uart, NULL, _BufferStringLenght);
    TEST_ASSERT_EQUAL(UartError_InvalidValue, read);

    read = Uart_readSync(uart, bufferRead, 0);
    TEST_ASSERT_EQUAL(UartError_InvalidValue, read);
  }

  close = Uart_close(uart);
  TEST_ASSERT_EQUAL(UartError_Success, close);
}

TEST(TestUart, UartReadSyncFuzzing) {
  Uart uart;
  int close, read;

  unsigned char bufferRead[_BaudSize] = {0};

  read = Uart_readSync(NULL, bufferRead, _BufferStringLenght);
  TEST_ASSERT_EQUAL(UartError_InvalidValue, read);

  uart = Uart_open(_UartPort);
  TEST_ASSERT_NOT_EQUAL(NULL, uart);

  if (TEST_PROTECT()) {
    read = Uart_readSync(uart, NULL, _BufferStringLenght);
    TEST_ASSERT_EQUAL(UartError_InvalidValue, read);

    read = Uart_readSync(uart, bufferRead, 0);
    TEST_ASSERT_EQUAL(UartError_InvalidValue, read);
  }

  close = Uart_close(uart);
  TEST_ASSERT_EQUAL(UartError_Success, close);
}

TEST(TestUart, UartIoctlFuzzing) {
  Uart uart;
  int ioctl, close;
  UartConfig uartConfigTest;

  ioctl = Uart_ioctl(NULL, UartIoctlRequest_SetConfig, &uartConfigTest);
  TEST_ASSERT_EQUAL(UartError_InvalidValue, ioctl);

  uart = Uart_open(_UartPort);
  TEST_ASSERT_NOT_EQUAL(NULL, uart);

  if (TEST_PROTECT()) {
    ioctl = Uart_ioctl(uart, -1, &uartConfigTest);
    TEST_ASSERT_EQUAL(UartError_InvalidValue, ioctl);

    ioctl = Uart_ioctl(uart, UartIoctlRequest_SetConfig, NULL);
    TEST_ASSERT_EQUAL(UartError_InvalidValue, ioctl);
    ioctl = Uart_ioctl(uart, UartIoctlRequest_GetConfig, NULL);
    TEST_ASSERT_EQUAL(UartError_InvalidValue, ioctl);

    ioctl = Uart_ioctl(uart, UartIoctlRequest_GetTxNoOfbytes, NULL);
    TEST_ASSERT_EQUAL(UartError_InvalidValue, ioctl);

    ioctl = Uart_ioctl(uart, UartIoctlRequest_GetRxNoOfbytes, NULL);
    TEST_ASSERT_EQUAL(UartError_InvalidValue, ioctl);
  }

  close = Uart_close(uart);
  TEST_ASSERT_EQUAL(UartError_Success, close);
}

TEST(TestUart, UartIoctlWhenBusy) {
  Uart uart;
  int ioctl, write, close;
  unsigned char buffer[_BaudSize] = {0};

  memcpy(buffer, _BufferString, _BufferStringLenght);
  UartConfig uartConfig = {.baudrate = _BaudRateDefault};

  uart = Uart_open(_UartPort);
  TEST_ASSERT_NOT_EQUAL(NULL, uart);

  if (TEST_PROTECT()) {
    write = Uart_writeAsync(uart, buffer, _BufferStringLenght);
    TEST_ASSERT_EQUAL(UartError_Success, write);

    ioctl = Uart_ioctl(uart, UartIoctlRequest_SetConfig, &uartConfig);
    TEST_ASSERT_EQUAL(UartError_Busy, ioctl);
  }

  close = Uart_close(uart);
  TEST_ASSERT_EQUAL(UartError_Success, close);
}

TEST(TestUart, UartOpenCloseCancelFuzzing) {
  Uart uart_1, uart_2, uart_3;
  int close, cancel;

  uart_1 = Uart_open(-1);
  TEST_ASSERT_EQUAL(NULL, uart_1);

  close = Uart_close(NULL);
  TEST_ASSERT_EQUAL(UartError_InvalidValue, close);

  uart_2 = Uart_open(_UartPort);
  TEST_ASSERT_NOT_EQUAL(NULL, uart_2);

  if (TEST_PROTECT()) {
    uart_3 = Uart_open(_UartPort);
    TEST_ASSERT_EQUAL(NULL, uart_3);

    cancel = Uart_cancel(NULL);
    TEST_ASSERT_EQUAL(UartError_InvalidValue, cancel);
  }

  close = Uart_close(uart_2);
  TEST_ASSERT_EQUAL(UartError_Success, close);

  close = Uart_close(uart_2);
  TEST_ASSERT_EQUAL(UartError_InvalidValue, close);
}
