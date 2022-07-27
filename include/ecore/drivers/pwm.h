#ifndef ECORE_DRIVERS_POWER_H
#define ECORE_DRIVERS_POWER_H

typedef enum {
  PwmError_Success,
  PwmError_InvalidValue,
  PwmError_NotConfigured,
  PwmError_FunctionNotSupported
} PwmError;

typedef struct PwmConfig {
  unsigned int frequency;
  unsigned char dutyCycle;
  unsigned char channel;
} PwmConfig;

struct PwmPrivate;
typedef struct PwmPrivate *Pwm;

Pwm Pwm_open(int instance);
int Pwm_setConfig(Pwm const self, const PwmConfig config);
PwmConfig *Pwm_getConfig(Pwm const self);
int Pwm_start(Pwm const self);
int Pwm_stop(Pwm const self);
int Pwm_close(Pwm const self);

#endif // ECORE_DRIVERS_POWER_H
