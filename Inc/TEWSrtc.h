/*
 * TEWSrtc.h
 *
 *  Created on: Mar 24, 2019
 *      Author: Hafiz
 */

#ifndef TEWSRTC_H_
#define TEWSRTC_H_

#include "stm32l4xx_hal.h"
#include <stdlib.h>
#include <stdbool.h>

#define DS3231_REG_TIME         0x00
#define DS3231_REG_ALARM1       0x07
#define DS3231_REG_ALARM2       0x0B
#define DS3231_REG_CONTROL      0x0E
#define DS3231_REG_STATUS       0x0F
#define DS3231_REG_TEMP         0x11

#define DS3231_CON_EOSC         0x80
#define DS3231_CON_BBSQW        0x40
#define DS3231_CON_CONV         0x20
#define DS3231_CON_RS2          0x10
#define DS3231_CON_RS1          0x08
#define DS3231_CON_INTCN        0x04
#define DS3231_CON_A2IE         0x02
#define DS3231_CON_A1IE         0x01

#define DS3231_STA_OSF          0x80
#define DS3231_STA_32KHZ        0x08
#define DS3231_STA_BSY          0x04
#define DS3231_STA_A2F          0x02
#define DS3231_STA_A1F          0x01

typedef enum
{
  ALARM_MODE_ALL_MATCHED = 0,
  ALARM_MODE_HOUR_MIN_SEC_MATCHED,
  ALARM_MODE_MIN_SEC_MATCHED,
  ALARM_MODE_SEC_MATCHED,
  ALARM_MODE_ONCE_PER_SECOND
} AlarmMode;

typedef enum
{
  SUNDAY = 1,
  MONDAY,
  TUESDAY,
  WEDNESDAY,
  THURSDAY,
  FRIDAY,
  SATURDAY
} DaysOfWeek;

typedef struct
{
  uint8_t Year;
  uint8_t Month;
  uint8_t Date;
  uint8_t DaysOfWeek;
  uint8_t Hour;
  uint8_t Min;
  uint8_t Sec;
} _RTC;

typedef struct {
	uint8_t seconds; /*!< Seconds parameter, from 00 to 59 */
	uint16_t subseconds; /*!< Sub-second down counter. When it reaches zero, it's reload value is the same as
	 @ref RTC_SYNC_PREDIV, so in our case 0x3FF = 1023, 1024 steps in one second */
	uint8_t minutes; /*!< Minutes parameter, from 00 to 59 */
	uint8_t hours; /*!< Hours parameter, 24Hour mode, 00 to 23 */
	uint8_t day; /*!< Day in a week, from 1 to 7 */
	uint8_t date; /*!< Date in a month, 1 to 31 */
	uint8_t month; /*!< Month in a year, 1 to 12 */
	uint8_t year; /*!< Year parameter, 00 to 99, 00 is 2000 and 99 is 2099 */
	uint32_t unix; /*!< Seconds from 01.01.1970 00:00:00 */
} TM_RTC_t;

TM_RTC_t UNIX;

void DS3231_Init(I2C_HandleTypeDef *handle);
bool DS3231_GetTime(_RTC *rtc);
bool DS3231_GetTime_B(_RTC *rtc);
bool DS3231_SetTime(_RTC *rtc);
bool DS3231_SetTime_B(_RTC *rtc);
bool DS3231_ReadTemperature(uint32_t *temp);
bool DS3231_SetAlarm1(AlarmMode mode, uint8_t date, uint8_t hour, uint8_t min, uint8_t sec);
bool DS3231_SetAlarm1_B(AlarmMode mode, uint8_t date, uint8_t hour, uint8_t min, uint8_t sec);
bool DS3231_SetAlarm2_B(AlarmMode mode, uint8_t date, uint8_t hour, uint8_t min);
bool DS3231_ClearAlarm1();
bool DS3231_ClearAlarm1_B();
bool DS3231_ClearAlarm2_B();
bool DS3231_ResetAlarm1_A();
bool DS3231_ResetAlarm1_B();
bool DS3231_ResetAlarm2_B();
bool ReadRegister(uint8_t regAddr, uint8_t *value);
bool ReadRegister_B(uint8_t regAddr, uint8_t *value);
bool WriteRegister(uint8_t regAddr, uint8_t value);
bool WriteRegister_B(uint8_t regAddr, uint8_t value);
void SetTimeRTC(uint8_t hour, uint8_t min, uint8_t sec, uint8_t date, uint8_t mon, uint8_t year);
void SetTimeRTC_B(uint8_t hour, uint8_t min, uint8_t sec, uint8_t date, uint8_t mon, uint8_t year);

/**
 * @brief  Get number of seconds from date and time since 01.01.1970 00:00:00
 * @param  *data: Pointer to @ref TM_RTC_t data structure
 * @retval Calculated seconds from date and time since 01.01.1970 00:00:00
 */
uint32_t TM_RTC_GetUnixTimeStamp(TM_RTC_t* data);

/**
 * @brief  Get formatted time from seconds till 01.01.1970 00:00:00
 *         It fills struct with valid data
 * @note   Valid if year is greater or equal (>=) than 2000
 * @param  *data: Pointer to @ref TM_RTC_Time_t struct to store formatted data in
 * @param  unix: Seconds from 01.01.1970 00:00:00 to calculate user friendly time
 * @retval None
 */
void TM_RTC_GetDateTimeFromUnix(TM_RTC_t* data, uint32_t unix);


#endif /* TEWSRTC_H_ */
