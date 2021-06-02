/*
 * TEWSrtc.c
 *
 *  Created on: Mar 24, 2019
 *      Author: Hafiz
 */

#include "TEWSrtc.h"
#include "parameterTEWS.h"

#define DS3231_ADDR  (0x68 << 1)

/* Internal RTC defines */
#define RTC_LEAP_YEAR(year)             ((((year) % 4 == 0) && ((year) % 100 != 0)) || ((year) % 400 == 0))
#define RTC_DAYS_IN_YEAR(x)             RTC_LEAP_YEAR(x) ? 366 : 365
#define RTC_OFFSET_YEAR                 1970
#define RTC_SECONDS_PER_DAY             86400
#define RTC_SECONDS_PER_HOUR            3600
#define RTC_SECONDS_PER_MINUTE          60

I2C_HandleTypeDef *i2c;

static uint8_t B2D(uint8_t bcd);
static uint8_t D2B(uint8_t decimal);

uint8_t TM_RTC_Months[2][12] = { { 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30,
		31 }, /* Not leap year */
{ 31, 29, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31 } /* Leap year */
};

void DS3231_Init(I2C_HandleTypeDef *handle) {
	i2c = handle;
}

bool DS3231_GetTime(_RTC *rtc)
{
  uint8_t startAddr = DS3231_REG_TIME;
  uint8_t buffer[7] = {0,};

  if(HAL_I2C_Master_Transmit(&hi2c2, DS3231_ADDR, &startAddr, 1, HAL_MAX_DELAY) != HAL_OK) return false;
  if(HAL_I2C_Master_Receive(&hi2c2, DS3231_ADDR, buffer, sizeof(buffer), HAL_MAX_DELAY) != HAL_OK) return false;

  rtc->Sec = B2D(buffer[0] & 0x7F);
  rtc->Min = B2D(buffer[1] & 0x7F);
  rtc->Hour = B2D(buffer[2] & 0x3F);
  rtc->DaysOfWeek = buffer[3] & 0x07;
  rtc->Date = B2D(buffer[4] & 0x3F);
  rtc->Month = B2D(buffer[5] & 0x1F);
  rtc->Year = B2D(buffer[6]);

  return true;
}

bool DS3231_GetTime_B(_RTC *rtc)
{
  uint8_t startAddr = DS3231_REG_TIME;
  uint8_t buffer[7] = {0,};

  if(HAL_I2C_Master_Transmit(&hi2c4, DS3231_ADDR, &startAddr, 1, HAL_MAX_DELAY) != HAL_OK) return false;
  if(HAL_I2C_Master_Receive(&hi2c4, DS3231_ADDR, buffer, sizeof(buffer), HAL_MAX_DELAY) != HAL_OK) return false;

  rtc->Sec = B2D(buffer[0] & 0x7F);
  rtc->Min = B2D(buffer[1] & 0x7F);
  rtc->Hour = B2D(buffer[2] & 0x3F);
  rtc->DaysOfWeek = buffer[3] & 0x07;
  rtc->Date = B2D(buffer[4] & 0x3F);
  rtc->Month = B2D(buffer[5] & 0x1F);
  rtc->Year = B2D(buffer[6]);

  return true;
}

uint32_t TM_RTC_GetUnixTimeStamp(TM_RTC_t *data) {
	uint32_t days = 0, seconds = 0;
	uint16_t i;
	uint16_t year = (uint16_t) (data->year + 2000);
	/* Year is below offset year */
	if (year < RTC_OFFSET_YEAR) {
		return 0;
	}
	/* Days in back years */
	for (i = RTC_OFFSET_YEAR; i < year; i++) {
		days += RTC_DAYS_IN_YEAR(i);
	}
	/* Days in current year */
	for (i = 1; i < data->month; i++) {
		days += TM_RTC_Months[RTC_LEAP_YEAR(year)][i - 1];
	}
	/* Day starts with 1 */
	days += data->date - 1;
	seconds = days * RTC_SECONDS_PER_DAY;
	seconds += data->hours * RTC_SECONDS_PER_HOUR;
	seconds += data->minutes * RTC_SECONDS_PER_MINUTE;
	seconds += data->seconds;

	/* seconds = days * 86400; */
	return seconds;
}

void TM_RTC_GetDateTimeFromUnix(TM_RTC_t *data, uint32_t unix) {
	uint16_t year;

	/* Store unix time to unix in struct */
	data->unix = unix;
	/* Get seconds from unix */
	data->seconds = unix % 60;
	/* Go to minutes */
	unix /= 60;
	/* Get minutes */
	data->minutes = unix % 60;
	/* Go to hours */
	unix /= 60;
	/* Get hours */
	data->hours = unix % 24;
	/* Go to days */
	unix /= 24;

	/* Get week day */
	/* Monday is day one */
	data->day = (unix + 3) % 7 + 1;

	/* Get year */
	year = 1970;
	while (1) {
		if (RTC_LEAP_YEAR(year)) {
			if (unix >= 366) {
				unix -= 366;
			} else {
				break;
			}
		} else if (unix >= 365) {
			unix -= 365;
		} else {
			break;
		}
		year++;
	}
	/* Get year in xx format */
	data->year = (uint8_t) (year - 2000);
	/* Get month */
	for (data->month = 0; data->month < 12; data->month++) {
		if (RTC_LEAP_YEAR(year)) {
			if (unix >= (uint32_t) TM_RTC_Months[1][data->month]) {
				unix -= TM_RTC_Months[1][data->month];
			} else {
				break;
			}
		} else if (unix >= (uint32_t) TM_RTC_Months[0][data->month]) {
			unix -= TM_RTC_Months[0][data->month];
		} else {
			break;
		}
	}
	/* Get month */
	/* Month starts with 1 */
	data->month++;
	/* Get date */
	/* Date starts with 1 */
	data->date = unix + 1;
}

bool DS3231_SetTime(_RTC *rtc) {
	uint8_t startAddr = DS3231_REG_TIME;
	uint8_t buffer[8] = { startAddr, D2B(rtc->Sec), D2B(rtc->Min), D2B(
			rtc->Hour), rtc->DaysOfWeek, D2B(rtc->Date), D2B(rtc->Month), D2B(
			rtc->Year) };
	if (HAL_I2C_Master_Transmit(&hi2c2, DS3231_ADDR, buffer, sizeof(buffer),
			HAL_MAX_DELAY) != HAL_OK)
		return false;

  return true;
}

bool DS3231_SetTime_B(_RTC *rtc) {
	uint8_t startAddr = DS3231_REG_TIME;
	uint8_t buffer[8] = { startAddr, D2B(rtc->Sec), D2B(rtc->Min), D2B(
			rtc->Hour), rtc->DaysOfWeek, D2B(rtc->Date), D2B(rtc->Month), D2B(
			rtc->Year) };
	if (HAL_I2C_Master_Transmit(&hi2c4, DS3231_ADDR, buffer, sizeof(buffer),
			HAL_MAX_DELAY) != HAL_OK)
		return false;

  return true;
}

bool DS3231_ReadTemperature(uint32_t *temp)
{
	float tempf;
  uint8_t startAddr = DS3231_REG_TEMP;
  uint8_t buffer[2] = {0,};

  if(HAL_I2C_Master_Transmit(&hi2c2, DS3231_ADDR, &startAddr, 1, HAL_MAX_DELAY) != HAL_OK) return false;
  if(HAL_I2C_Master_Receive(&hi2c2, DS3231_ADDR, buffer, sizeof(buffer), HAL_MAX_DELAY) != HAL_OK) return false;

  int16_t value = (buffer[0] << 8) | (buffer[1]);
  value = (value >> 6);

  tempf = (value / 4.0f)*100.0;
  *temp = (uint32_t)tempf;
  return true;
}

bool DS3231_SetAlarm1(uint8_t mode, uint8_t date, uint8_t hour, uint8_t min, uint8_t sec)
{
  uint8_t alarmSecond = D2B(sec);
  uint8_t alarmMinute = D2B(min);
  uint8_t alarmHour = D2B(hour);
  uint8_t alarmDate = D2B(date);

  switch(mode)
  {
  case ALARM_MODE_ALL_MATCHED:
    break;
  case ALARM_MODE_HOUR_MIN_SEC_MATCHED:
    alarmDate |= 0x80;
    break;
  case ALARM_MODE_MIN_SEC_MATCHED:
    alarmDate |= 0x80;
    alarmHour |= 0x80;
    break;
  case ALARM_MODE_SEC_MATCHED:
    alarmDate |= 0x80;
    alarmHour |= 0x80;
    alarmMinute |= 0x80;
    break;
  case ALARM_MODE_ONCE_PER_SECOND:
    alarmDate |= 0x80;
    alarmHour |= 0x80;
    alarmMinute |= 0x80;
    alarmSecond |= 0x80;
    break;
  default:
    break;
  }

  /* Write Alarm Registers */
  uint8_t startAddr = DS3231_REG_ALARM1;
  uint8_t buffer[5] = {startAddr, alarmSecond, alarmMinute, alarmHour, alarmDate};
  if(HAL_I2C_Master_Transmit(&hi2c2, DS3231_ADDR, buffer, sizeof(buffer), HAL_MAX_DELAY) != HAL_OK) return false;

  /* Enable Alarm1 at Control Register */
  uint8_t ctrlReg = 0x00;
  ReadRegister(DS3231_REG_CONTROL, &ctrlReg);
  ctrlReg |= DS3231_CON_A1IE;
  ctrlReg |= DS3231_CON_INTCN;
  WriteRegister(DS3231_REG_CONTROL, ctrlReg);

  return true;
}

bool DS3231_SetAlarm1_B(uint8_t mode, uint8_t date, uint8_t hour, uint8_t min, uint8_t sec)
{
  uint8_t alarmSecond = D2B(sec);
  uint8_t alarmMinute = D2B(min);
  uint8_t alarmHour = D2B(hour);
  uint8_t alarmDate = D2B(date);

  switch(mode)
  {
  case ALARM_MODE_ALL_MATCHED:
    break;
  case ALARM_MODE_HOUR_MIN_SEC_MATCHED:
    alarmDate |= 0x80;
    break;
  case ALARM_MODE_MIN_SEC_MATCHED:
    alarmDate |= 0x80;
    alarmHour |= 0x80;
    break;
  case ALARM_MODE_SEC_MATCHED:
    alarmDate |= 0x80;
    alarmHour |= 0x80;
    alarmMinute |= 0x80;
    break;
  case ALARM_MODE_ONCE_PER_SECOND:
    alarmDate |= 0x80;
    alarmHour |= 0x80;
    alarmMinute |= 0x80;
    alarmSecond |= 0x80;
    break;
  default:
    break;
  }

  /* Write Alarm Registers */
  uint8_t startAddr = DS3231_REG_ALARM1;
  uint8_t buffer[5] = {startAddr, alarmSecond, alarmMinute, alarmHour, alarmDate};
  if(HAL_I2C_Master_Transmit(&hi2c4, DS3231_ADDR, buffer, sizeof(buffer), HAL_MAX_DELAY) != HAL_OK) return false;

  /* Enable Alarm1 at Control Register */
  uint8_t ctrlReg = 0x00;
  ReadRegister_B(DS3231_REG_CONTROL, &ctrlReg);
  ctrlReg |= DS3231_CON_A1IE;
  ctrlReg |= DS3231_CON_INTCN;
  WriteRegister_B(DS3231_REG_CONTROL, ctrlReg);

  return true;
}

bool DS3231_SetAlarm2_B(uint8_t mode, uint8_t date, uint8_t hour, uint8_t min)
{
  uint8_t alarmMinute = D2B(min);
  uint8_t alarmHour = D2B(hour);
  uint8_t alarmDate = D2B(date);

  switch(mode)
  {
  case ALARM_MODE_ALL_MATCHED:
    break;
  case ALARM_MODE_HOUR_MIN_SEC_MATCHED:
    alarmDate |= 0x80;
    break;
  case ALARM_MODE_MIN_SEC_MATCHED:
    alarmDate |= 0x80;
    alarmHour |= 0x80;
    break;
  default:
    break;
  }

  /* Write Alarm Registers */
  uint8_t startAddr = DS3231_REG_ALARM2;
  uint8_t buffer[4] = {startAddr, alarmMinute, alarmHour, alarmDate};
  if(HAL_I2C_Master_Transmit(&hi2c4, DS3231_ADDR, buffer, sizeof(buffer), HAL_MAX_DELAY) != HAL_OK) return false;

  /* Enable Alarm1 at Control Register */
  uint8_t ctrlReg = 0x00;
  ReadRegister_B(DS3231_REG_CONTROL, &ctrlReg);
  ctrlReg |= DS3231_CON_A2IE;
  ctrlReg |= DS3231_CON_INTCN;
  WriteRegister_B(DS3231_REG_CONTROL, ctrlReg);

  return true;
}

bool DS3231_ClearAlarm1()
{
  uint8_t ctrlReg;
  uint8_t statusReg;

  /* Clear Control Register */
  ReadRegister(DS3231_REG_CONTROL, &ctrlReg);
  ctrlReg &= ~DS3231_CON_A1IE;
  WriteRegister(DS3231_REG_CONTROL, ctrlReg);

  /* Clear Status Register */
  ReadRegister(DS3231_REG_STATUS, &statusReg);
  statusReg &= ~DS3231_STA_A1F;
  WriteRegister(DS3231_REG_STATUS, statusReg);

  return true;
}

bool DS3231_ClearAlarm1_B()
{
  uint8_t ctrlReg;
  uint8_t statusReg;

  /* Clear Control Register */
  ReadRegister_B(DS3231_REG_CONTROL, &ctrlReg);
  ctrlReg &= ~DS3231_CON_A1IE;
  WriteRegister_B(DS3231_REG_CONTROL, ctrlReg);

  /* Clear Status Register */
  ReadRegister_B(DS3231_REG_STATUS, &statusReg);
  statusReg &= ~DS3231_STA_A1F;
  WriteRegister_B(DS3231_REG_STATUS, statusReg);

  return true;
}

bool DS3231_ClearAlarm2_B()
{
  uint8_t ctrlReg;
  uint8_t statusReg;

  /* Clear Control Register */
  ReadRegister_B(DS3231_REG_CONTROL, &ctrlReg);
  ctrlReg &= ~DS3231_CON_A2IE;
  WriteRegister_B(DS3231_REG_CONTROL, ctrlReg);

  /* Clear Status Register */
  ReadRegister_B(DS3231_REG_STATUS, &statusReg);
  statusReg &= ~DS3231_STA_A2F;
  WriteRegister_B(DS3231_REG_STATUS, statusReg);

  return true;
}

bool DS3231_ResetAlarm1_A()
{
  uint8_t statusReg;
  /* Clear Status Register */
  ReadRegister(DS3231_REG_STATUS, &statusReg);
  statusReg &= ~DS3231_STA_A1F;
  WriteRegister(DS3231_REG_STATUS, statusReg);

  return true;
}

bool DS3231_ResetAlarm1_B()
{
  uint8_t statusReg;
  /* Clear Status Register */
  ReadRegister_B(DS3231_REG_STATUS, &statusReg);
  statusReg &= ~DS3231_STA_A1F;
  WriteRegister_B(DS3231_REG_STATUS, statusReg);

  return true;
}

bool DS3231_ResetAlarm2_B()
{
  uint8_t statusReg;
  /* Clear Status Register */
  ReadRegister_B(DS3231_REG_STATUS, &statusReg);
  statusReg &= ~DS3231_STA_A2F;
  WriteRegister_B(DS3231_REG_STATUS, statusReg);

  return true;
}

bool ReadRegister(uint8_t regAddr, uint8_t *value)
{
  if(HAL_I2C_Master_Transmit(&hi2c2, DS3231_ADDR, &regAddr, 1, HAL_MAX_DELAY) != HAL_OK) return false;
  if(HAL_I2C_Master_Receive(&hi2c2, DS3231_ADDR, value, 1, HAL_MAX_DELAY) != HAL_OK) return false;

  return true;
}

bool ReadRegister_B(uint8_t regAddr, uint8_t *value)
{
  if(HAL_I2C_Master_Transmit(&hi2c4, DS3231_ADDR, &regAddr, 1, HAL_MAX_DELAY) != HAL_OK) return false;
  if(HAL_I2C_Master_Receive(&hi2c4, DS3231_ADDR, value, 1, HAL_MAX_DELAY) != HAL_OK) return false;

  return true;
}

bool WriteRegister(uint8_t regAddr, uint8_t value)
{
  uint8_t buffer[2] = {regAddr, value};
  if(HAL_I2C_Master_Transmit(&hi2c2, DS3231_ADDR, buffer, sizeof(buffer), HAL_MAX_DELAY) != HAL_OK) return false;

  return true;
}

bool WriteRegister_B(uint8_t regAddr, uint8_t value)
{
  uint8_t buffer[2] = {regAddr, value};
  if(HAL_I2C_Master_Transmit(&hi2c4, DS3231_ADDR, buffer, sizeof(buffer), HAL_MAX_DELAY) != HAL_OK) return false;

  return true;
}

static uint8_t B2D(uint8_t bcd)
{
  return (bcd >> 4) * 10 + (bcd & 0x0F);
}

static uint8_t D2B(uint8_t decimal)
{
  return (((decimal / 10) << 4) | (decimal % 10));
}

void SetTimeRTC(uint8_t hour, uint8_t min, uint8_t sec, uint8_t date, uint8_t mon, uint8_t year){
	_RTC rtcTEWSx;
	rtcTEWSx.Hour = hour;
	rtcTEWSx.Min = min;
	rtcTEWSx.Sec = sec;
	rtcTEWSx.Date = date;
	rtcTEWSx.Month = mon;
	rtcTEWSx.Year = year;
	rtcTEWSx.DaysOfWeek = MONDAY;
	DS3231_SetTime(&rtcTEWSx);
}

void SetTimeRTC_B(uint8_t hour, uint8_t min, uint8_t sec, uint8_t date, uint8_t mon, uint8_t year){
	_RTC rtcTEWSx;
	rtcTEWSx.Hour = hour;
	rtcTEWSx.Min = min;
	rtcTEWSx.Sec = sec;
	rtcTEWSx.Date = date;
	rtcTEWSx.Month = mon;
	rtcTEWSx.Year = year;
	rtcTEWSx.DaysOfWeek = MONDAY;
	DS3231_SetTime_B(&rtcTEWSx);
}
