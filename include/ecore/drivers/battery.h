#ifndef ECORE_DRIVERS_BATTERY_H
#define ECORE_DRIVERS_BATTERY_H

#include <stdint.h>

typedef enum {
  BattaryError_Success,
  BattaryError_InvalidValue,
  BattaryError_NotExist,
  BattaryError_ReadFailed,
  BattaryError_FunctionNotSupported
} BattaryError;

typedef enum { BatteryType_Chargeable, BatteryType_NotChargeable } BatteryType;

typedef enum {
  BatteryChargeSource_None,
  BatteryChargeSource_USB,
  BatteryChargeSource_PowerSupply,
  BatteryChargeSource_Wireless,
} BatteryChargeSource;

typedef enum {
  BatteryStatus_Charging,
  BatteryStatus_Discharging,
  BatteryStatus_NotCharging,
  BatteryStatus_ChargeFull,
  BatteryStatus_ChargeLow,
  BatteryStatus_OverVoltage,
  BatteryStatus_UnderVoltage,
  BatteryStatus_OverTemp,
  BatteryStatus_UnderTemp,
  BatteryStatus_OverChargingTemp,
  BatteryStatus_UnderhargingTemp,
  BatteryStatus_ChargeTimeExpired,
  BatteryStatus_Unknown
} BatteryStatus;

typedef struct {
  BatteryType type;
  uint16_t minVoltage;
  uint16_t maxVoltage;
  int16_t minTemperature;
  int maxTemperature;
  uint16_t maxCapacity;
  unsigned char asyncSupported;
} BatteryInfo;

typedef enum {
  BatteryIoctlRequest_SetMinVoltageThreshold,
  BatteryIoctlRequest_SetMaxVoltageThreshold,
  BatteryIoctlRequest_SetBatteryMinTempThreshold,
  BatteryIoctlRequest_SetBatteryMaxTempThreshold,
  BatteryIoctlRequest_SetBatteryMinChargeTempThreshold,
  BatteryIoctlRequest_SetBatteryMaxChargeTempThreshold,
  BatteryIoctlRequest_SetBatteryMaxOutputCurrent,
  BatteryIoctlRequest_SetBatteryMaxInputChargeCurrent,
  BatteryIoctlRequest_SetBatteryChargeTimer,
  BatteryIoctlRequest_GetBatteryMinVoltageThreshold,
  BatteryIoctlRequest_GetBatteryMaxVoltageThreshold,
  BatteryIoctlRequest_GetBatteryOCV,
  BatteryIoctlRequest_GetBatteryBootVoltage,
  BatteryIoctlRequest_GetBatteryMinBatteryTempThreshold,
  BatteryIoctlRequest_GetBatteryMaxBatteryTempThreshold,
  BatteryIoctlRequest_GetBatteryMinChargeTempThreshold,
  BatteryIoctlRequest_GetBatteryMaxChargeTempThreshold,
  BatteryIoctlRequest_GetBatteryMaxOutputCurrent,
  BatteryIoctlRequest_GetBatteryMaxInputChargeCurrent,
  BatteryIoctlRequest_GetBatteryChargeTimer,
  BatteryIoctlRequest_GetBatteryChargeSource,
  BatteryIoctlRequest_GetBatteryStatus
} BatteryIoctlRequest;

struct BatteryPrivate;
typedef struct BatteryPrivate *Battery;
typedef void (*BatteryCallback)(BatteryStatus status, void *context);

Battery Battery_open(int instance);

void Battery_setCallback(Battery const self, BatteryCallback callback,
                         void *context);
BatteryInfo *Battery_getInfo(Battery const self);
int Battery_current(Battery const self, int *current);
int Battery_voltage(Battery const self, int *voltage);
int Battery_chargeLevel(Battery const self, int *chargeLevel);
int Battery_capacity(Battery const self, int *capacity);
int Battery_temp(Battery const self, int *temp);
int Battery_enableCharging(Battery const self, bool enable);
int Batteryis_charging(Battery const self, bool *pucCharging);
int Battery_ioctl(Battery const self, BatteryIoctlRequest request,
                  void *const buffer);
int Battery_close(Battery const self);

#endif // ECORE_DRIVERS_BATTERY_H
