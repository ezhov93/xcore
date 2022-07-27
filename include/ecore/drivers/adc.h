#ifndef ECORE_DRIVERS_ADC_H
#define ECORE_DRIVERS_ADC_H

#include <stdint.h>

typedef enum {
  AdcError_Success,
  AdcError_Failed,
  AdcError_InvalideValue,
  AdcError_NotOpen,
  AdcError_FunctionNotSupported,
  AdcError_ChBusy
} AdcError;

struct AdcPrivate;

typedef struct AdcPrivate *Adc;

typedef void (*AdcCallback)(int *convertedData, void *usercontext);

typedef struct {
  int sampleTime;
  int resolution;
} AdcConfig;

typedef enum {
  AdcChStateIdle,
  AdcChStateBusy,
} AdcChState;

typedef struct {
  unsigned char number;
  AdcChState state;
} AdcChStatus;

typedef struct {
  unsigned char channel;
  void *buffer;
  unsigned char bufLen;
} AdcChBuffer;

typedef struct {
  unsigned char channel;
  void *buffer;
  unsigned char bufLen;
  uint16_t chainMask;
} AdcChain;

typedef enum IotAdcIoctlRequest {
  AdcIoctlRequest_SetAdcConfig, // Takes AdcConfig parameter
  AdcIoctlRequest_GetAdcConfig, // Returns results in AdcConfig parameter
  AdcIoctlRequest_GetChStatus,  // Returns results in  AdcChStatus parameter
  AdcIoctlRequest_SetChBuffer,  // Takes AdcChBuffer parameter
  AdcIoctlRequest_SetAdcChain   // Takes AdcChain  parameter
} AdcIoctlRequest;

Adc Adc_open(int instance);
int Adc_close(Adc const self);
void Adc_setCallback(Adc const self, unsigned char channel,
                     AdcCallback callback, void *context);
int Adc_start(Adc const self, unsigned char channel);
int Adc_stop(Adc const self, unsigned char channel);
int Adc_readSample(Adc const self, unsigned char channel, int *sample);
int Adc_ioctl(Adc const self, AdcIoctlRequest request, void *const buffer);

#endif // ECORE_DRIVERS_ADC_H
