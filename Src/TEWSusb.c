/*
 * TEWSusb.c
 *
 *  Created on: Mar 24, 2019
 *      Author: Hafiz
 */

#include "TEWSusb.h"
#include "parameterTEWS.h"

USB_t USB;
char printusb[512] = "print USB \n"; //reserved for print buffer usb
char FIRMWARE[8] = "v1.3.2.1";
char printbuffer[512] = ""; //reserved for print buffer usb
char savedsdcardusb[255] = "";
extern uint8_t AISstatus;
uint32_t lat;
uint32_t lng;

extern uint8_t statusSendGPS;

extern IRIDIUM_t IRIDIUM;
extern RTC_TimeTypeDef stimeststuctureget;
extern _RTC rtcTEWS;
extern _RTC rtcTEWS2;

void USB_Init(void) {
    HAL_NVIC_SetPriority(LPUART1_IRQn, 0, 0);
    HAL_NVIC_EnableIRQ(LPUART1_IRQn);
	USB.rxIndex = 0;
	HAL_UART_Receive_DMA(&_UNI_USART, &USB.rxTmp, 1);
}

void USB_Callback(void) {
	if (USB.rxIndex < sizeof(USB.rxBuffer) - 2) {
		USB.rxBuffer[USB.rxIndex] = USB.rxTmp;
		USB.rxIndex++;
	}
	if (USB.rxTmp == 13 || USB.rxTmp == 10) {
		osMessageQueuePut(usbQueueHandle, &USB.rxBuffer, 0U, 0U);
		memset(USB.rxBuffer, 0, sizeof(USB.rxBuffer));
		USB.rxIndex = 0;
	}
	HAL_UART_Receive_DMA(&_UNI_USART, &USB.rxTmp, 1);
}

void USB_Process(char *buffer) {

	if (strstr((char*) buffer, "TEWS+VERSION?")) {
		usb_universal_printf("VERSION: %s\r\n", FIRMWARE);
	}

	if (strstr((char*) buffer, "TEWS+SN?")) {
		usb_universal_printf("SN: %u\r\n", SERIALNUMBER);
	}

	/*
	 * Reset Microcontroller
	 */
	if (strstr((char*) buffer, "TEWS+REBOOT")) {
		HAL_GPIO_WritePin(ON_OFF_GPIO_Port, ON_OFF_Pin, GPIO_PIN_RESET);
		HAL_NVIC_SystemReset();
	}


	if (strstr((char*) buffer, "TEWS+RPERIOD=")) {
		periodsenddata = parsingdatausb(buffer, "TEWS+RPERIOD=");
		write_eeprom_union(&hi2c1, 0x14, periodsenddata);
		usb_universal_printf("OK\r\n");
	}

	if (strstr((char*) buffer, "TEWS+RPERIOD?")) {
		usb_universal_printf("RPERIOD: %u\r\n", periodsenddata);
	}

	if (strstr((char*) buffer, "TEWS+TPERIOD=")) {
		periodsendloc = parsingdatausb(buffer, "TEWS+TPERIOD=");
		write_eeprom_union(&hi2c1, 0x18, periodsendloc);
		usb_universal_printf("OK\r\n");
	}

	if (strstr((char*) buffer, "TEWS+TPERIOD?")) {
		char str[128];
		sprintf(str,"TPERIOD:  %lu\r\n", periodsendloc);
		//uint16_t sizedata = strlen(str);
		//osMessageQueuePut(sizedatairidiumQueueHandle, &sizedata, 0U, 0U);
		//osMessageQueuePut(iridiumsendQueueHandle, str, 0U, 0U);
		usb_universal_printf("GPS TRANSMITER PERIODE: %lu\r\n", periodsendloc);
	}

	if (strstr((char*) buffer, "TEWS+DIST=")) {
		distancedrifting = parsingdatausb(buffer, "TEWS+DIST=");
		write_eeprom_union(&hi2c1, 0x24, distancedrifting);
		usb_universal_printf("OK\r\n");
	}

	if (strstr((char*) buffer, "TEWS+DIST?")) {
		usb_universal_printf("DISTANCE DRIFTING: %lu\r\n", distancedrifting);
	}

	//Contoh format masukan "TEWS+LAT=6345482,S," untuk koordinat latitude -6.345482
	//Contoh format masukan "TEWS+LAT=6345482,N," untuk koordinat latitude 6.345482
	if (strstr((char*) buffer, "TEWS+LAT=")) {
		char *glatitude;
		char *latsim;
		char *strx;
		strx = strstr(buffer, "TEWS+LAT=");
		strtok_r(strx, "=", &strx);
		glatitude = strtok_r(strx, ",", &strx);
		latsim = strtok_r(strx, ",", &strx);
		lat = atol(glatitude);
		write_eeprom_union(&hi2c1, 0x26, lat);
		if (strcmp(latsim, "S") == 0) {
			latitude = (float) lat / 1000000.0;
			latitude = (-1.0) * latitude;
		} else
			latitude = (float) lat / 1000000.0;
		usb_universal_printf("OK\r\n");
	}

	if (strstr((char*) buffer, "TEWS+LAT?")) {
		if (latitude < 0)
			usb_universal_printf("DEPLOY LATITUDE: -%lu.%lu\r\n", lat / 1000000,
					lat % 1000000);
		else
			usb_universal_printf("DEPLOY LATITUDE: %lu.%lu\r\n", lat / 1000000,
					lat % 1000000);
	}

	//Contoh format masukan "TEWS+LNG=106661638,W," untuk koordinat longitude -106.661638
	//Contoh format masukan "TEWS+LNG=106661638,E," untuk koordinat longitude 106.661638
	if (strstr((char*) buffer, "TEWS+LNG=")) {
		char *glongitude;
		char *longsim;
		char *strx;
		strx = strstr(buffer, "TEWS+LNG=");
		strtok_r(strx, "=", &strx);
		glongitude = strtok_r(strx, ",", &strx);
		longsim = strtok_r(strx, ",", &strx);
		lng = atol(glongitude);
		write_eeprom_union(&hi2c1, 0x31, lng);
		if (strcmp(longsim, "W") == 0) {
			longitude = (float) lng / 1000000.0;
			longitude = (-1.0) * longitude;
		} else
			longitude = (float) lng / 1000000.0;
		usb_universal_printf("OK\r\n");
	}

	if (strstr((char*) buffer, "TEWS+LNG?")) {
		if (longitude < 0)
			usb_universal_printf("DEPLOY LONGITUDE: -%lu.%lu\r\n",
					lng / 1000000, lng % 1000000);
		else
			usb_universal_printf("DEPLOY LONGITUDE: %lu.%lu\r\n", lng / 1000000,
					lng % 1000000);
	}




	if (strstr((char*) buffer, "TEWS+IRIDIUMON")) {

		//IRIDIUM_Init();
		HAL_GPIO_WritePin(ON_OFF_GPIO_Port, ON_OFF_Pin, GPIO_PIN_SET);
		usb_universal_printf("SET IRIDIUM ON\r\n");
	}

	if (strstr((char*) buffer, "TEWS+IRIDIUMOFF")) {

		//HAL_GPIO_WritePin(NETWORKINT_GPIO_Port, NETWORKINT_Pin, GPIO_PIN_SET);
		//osDelay(50);
		HAL_GPIO_WritePin(ON_OFF_GPIO_Port, ON_OFF_Pin, GPIO_PIN_RESET);
		//osDelay(50);
		//HAL_GPIO_WritePin(NETWORKINT_GPIO_Port, NETWORKINT_Pin, GPIO_PIN_SET);

		usb_universal_printf("RESET IRIDIUM OFF\r\n");
	}

	if (strstr((char*) buffer, "TEWS+GPSON")) {

		//IRIDIUM_Init();
		GPS_Init();
		usb_universal_printf("SET GPS ON\r\n");
	}

	if (strstr((char*) buffer, "TEWS+GPSOFF")) {

		HAL_GPIO_WritePin(GPIOD, GPS_RESET_Pin, GPIO_PIN_RESET);
		usb_universal_printf("SET GPS OFF\r\n");
	}

	if (strstr((char*) buffer, "TEWS+GPSLOC")) {
		//IRIDIUM_Init();
		//osDelay(500);
		EncodeTEWSLocBuoy(0, 15);
		usb_universal_printf("GPS Location.\r\n");
	}

	if (strstr((char*) buffer, "TEWS+BATGPS?")) {
		char str[128];
		sprintf(str,"VOLT:  %lu V, CURRENT:  %lu mA\r\n", getvoltage(), getcurrent());
		uint16_t sizedata = strlen(str);
		osMessageQueuePut(sizedatairidiumQueueHandle, &sizedata, 0U, 0U);
		osMessageQueuePut(iridiumsendQueueHandle, str, 0U, 0U);

		usb_universal_printf("VOLT:  %lu V, CURRENT:  %lu mA\r\n", getvoltage(), getcurrent());
	}

	/*
	 * Set time RTC
	 */
	if (strstr((char*) buffer, "TEWS+TIME=")) {
		char *strakustik;
		char *hour;
		char *min;
		char *sec;
		char *date;
		char *mon;
		char *year;

		strakustik = strstr((char*) buffer, "TEWS+SETTIME=");

		strtok_r(strakustik, "=", &strakustik);
		hour = strtok_r(strakustik, ",", &strakustik);
		min = strtok_r(strakustik, ",", &strakustik);
		sec = strtok_r(strakustik, ",", &strakustik);
		date = strtok_r(strakustik, ",", &strakustik);
		mon = strtok_r(strakustik, ",", &strakustik);
		year = strtok_r(strakustik, "\r", &strakustik);

		rtcTEWS.Hour = atoi(hour);
		rtcTEWS.Min = atoi(min);
		rtcTEWS.Sec = atoi(sec);
		rtcTEWS.Date = atoi(date);
		rtcTEWS.Month = atoi(mon);
		rtcTEWS.Year = atoi(year);
		rtcTEWS.DaysOfWeek = MONDAY;
		DS3231_SetTime(&rtcTEWS);
		HAL_UART_Transmit(&_UNI_USART, (uint8_t*) "OK\r\n", sizeof("OK\r\n"),
				10000);

		DS3231_GetTime(&rtcTEWS);
		usb_universal_printf("TIME: %.2d:%.2d:%.2d %.2d/%.2d/%.2d \r\n",
				rtcTEWS.Hour, rtcTEWS.Min, rtcTEWS.Sec, rtcTEWS.Date,
				rtcTEWS.Month, rtcTEWS.Year);
	}

	if (strstr((char*) buffer, "TEWS+TIME?")) {
		DS3231_GetTime(&rtcTEWS);
		DS3231_GetTime_B(&rtcTEWS2);
		usb_universal_printf("TIME: %.2d:%.2d:%.2d %.2d/%.2d/%.2d \r\n",
				rtcTEWS.Hour, rtcTEWS.Min, rtcTEWS.Sec, rtcTEWS.Date,
				rtcTEWS.Month, rtcTEWS.Year);
		usb_universal_printf("TIME2: %.2d:%.2d:%.2d %.2d/%.2d/%.2d \r\n",
				rtcTEWS2.Hour, rtcTEWS2.Min, rtcTEWS2.Sec, rtcTEWS2.Date,
				rtcTEWS2.Month, rtcTEWS2.Year);
	}

	if (strstr((char*) buffer, "TEWS+ALARMRESET1A")) {
		DS3231_ResetAlarm1_A();
		usb_universal_printf("RESERT ALARM1_A OK\r\n");
	}

	if (strstr((char*) buffer, "TEWS+ALARMRESET2B")) {
		DS3231_ResetAlarm2_B();
		usb_universal_printf("RESERT ALARM2_B OK\r\n");
	}

	if (strstr((char*) buffer, "TEWS+ALARMRESET1B")) {
		DS3231_ResetAlarm1_B();
		usb_universal_printf("RESERT ALARM1_B OK\r\n");
	}

	if (strstr((char*) buffer, "TEWS+POWERRESET")) {
		DS3231_ClearAlarm1();
		DS3231_ClearAlarm1_B();
		HAL_GPIO_WritePin(SWT_TRIG_GPIO_Port, SWT_TRIG_Pin, GPIO_PIN_RESET);
	}

	if (strstr((char*) buffer, "TEWS+POWERSET")) {
		HAL_GPIO_WritePin(SWT_TRIG_GPIO_Port, SWT_TRIG_Pin, GPIO_PIN_SET);
	}

	if (strstr((char*) buffer, "TEWS+ALARMSET1A=")) {
		char *str = strstr((char*) buffer, "TEWS+ALARMSET1A=");
		strtok_r(str, "=", &str);
		char *_time = strtok_r(str, "\r", &str);
		int _Mode = valueFromString(_time, 0, 1);
		int _Date = valueFromString(_time, 1, 2);
		int _Hour = valueFromString(_time, 3, 2);
		int _Min = valueFromString(_time, 5, 2);
		int _Sec = valueFromString(_time, 7, 2);
		DS3231_SetAlarm1_B(_Mode, _Date, _Hour, _Min, _Sec);
		usb_universal_printf("SET ALARM1A: MODE=%d, DATE=%.2d, HOUR=%.2d, MINUTE=%.2d, SECON=%.2d \r\n",_Mode,_Date,_Hour,_Min,_Sec);
	}

	if (strstr((char*) buffer, "TEWS+ALARMSET1B=")) {
		char *str = strstr((char*) buffer, "TEWS+ALARMSET1B=");
		strtok_r(str, "=", &str);
		char *_time = strtok_r(str, "\r", &str);
		int _Mode = valueFromString(_time, 0, 1);
		int _Date = valueFromString(_time, 1, 2);
		int _Hour = valueFromString(_time, 3, 2);
		int _Min = valueFromString(_time, 5, 2);
		int _Sec = valueFromString(_time, 7, 2);
		DS3231_SetAlarm1(_Mode, _Date, _Hour, _Min, _Sec);
		DS3231_SetAlarm1_B(_Mode, _Date, _Hour, _Min, _Sec);
		usb_universal_printf("SET ALARM1B: MODE=%d, DATE=%.2d, HOUR=%.2d, MINUTE=%.2d, SECON=%.2d \r\n",_Mode,_Date,_Hour,_Min,_Sec);
	}

	if (strstr((char*) buffer, "TEWS+ALARMSET2B=")) {
		char *str = strstr((char*) buffer, "TEWS+ALARMSET2B=");
		strtok_r(str, "=", &str);
		char *_time = strtok_r(str, "\r", &str);
		int _Mode = valueFromString(_time, 0, 1);
		int _Date = valueFromString(_time, 1, 2);
		int _Hour = valueFromString(_time, 3, 2);
		int _Min = valueFromString(_time, 5, 2);
		DS3231_SetAlarm2_B(_Mode, _Date, _Hour, _Min);
		usb_universal_printf("SET ALARM2B: MODE=%d, DATE=%.2d, HOUR=%.2d, MINUTE=%.2d \r\n",_Mode,_Date,_Hour,_Min);
	}

	if (strstr((char*) buffer, "TEWS+ALARMISTRIGER")) {
		//usb_universal_printf("ALARMISTRIGER: 1=%d, 2=%d\r\n",DS3231_IsAlarm1Triggered_B(),DS3231_IsAlarm2Triggered_B());
	}

	//set datetime, contoh:2010122300307 = tahun:20, bulan:10, tanggal:12, jam:23, menit:00, detik:30, hari ke:7 (minggu)
	if (strstr((char*) buffer, "TEWS+SETTIME=")) {
		RTC_TimeTypeDef RTCTimeMicro;
		RTC_DateTypeDef RTCDateMicro;
		_RTC setTime;

		char *str = strstr((char*) buffer, "TEWS+SETTIME=");
		strtok_r(str, "=", &str);
		char *_time = strtok_r(str, "\r", &str);
		int _Hour = valueFromString(_time, 6, 2);
		int _Min = valueFromString(_time, 8, 2);
		int _Sec = valueFromString(_time, 10, 2);
		int _Date = valueFromString(_time, 4, 2);
		int _Month = valueFromString(_time, 2, 2);
		int _Year = valueFromString(_time, 0, 2);
		int _Monday = valueFromString(_time, 12, 1);

		usb_universal_printf(
				"DATE=20%.2d-%.2d-%.2d TIME=%.2d:%.2d:%.2d DAY=%d\r\n", _Year,
				_Month, _Date, _Hour, _Min, _Sec, _Monday);
		setTime.Hour = _Hour;
		setTime.Min = _Min;
		setTime.Sec = _Sec;
		setTime.Date = _Date;
		setTime.Month = _Month;
		setTime.Year = _Year;
		setTime.DaysOfWeek = _Monday;
		DS3231_SetTime(&setTime);
		DS3231_SetTime_B(&setTime);

		RTCTimeMicro.Hours = _Hour;
		RTCTimeMicro.Minutes = _Min;
		RTCTimeMicro.Seconds = _Sec;
		HAL_RTC_SetTime(&hrtc, &RTCTimeMicro, RTC_FORMAT_BIN);

		RTCDateMicro.Year = _Year;
		RTCDateMicro.Month = _Month;
		RTCDateMicro.Date = _Date;
		RTCDateMicro.WeekDay = _Monday;
		HAL_RTC_SetDate(&hrtc, &RTCDateMicro, RTC_FORMAT_BIN);

		//get rtc
		RTC_TimeTypeDef getRTCTimeMicro;
		RTC_DateTypeDef getRTCDateMicro;
		_RTC getTime;
		HAL_RTC_GetTime(&hrtc, &getRTCTimeMicro, RTC_FORMAT_BIN);
		HAL_RTC_GetDate(&hrtc, &getRTCDateMicro, RTC_FORMAT_BIN);
		DS3231_GetTime(&getTime);

		usb_universal_printf(
				"DATETIME ST: 20%.2d-%.2d-%.2d %.2d:%.2d:%.2d \r\n",
				getRTCDateMicro.Year, getRTCDateMicro.Month,
				getRTCDateMicro.Date, getRTCTimeMicro.Hours,
				getRTCTimeMicro.Minutes, getRTCTimeMicro.Seconds);
		usb_universal_printf(
				"DATETIME DS: 20%.2d-%.2d-%.2d %.2d:%.2d:%.2d \r\n",
				getTime.Year, getTime.Month, getTime.Date, getTime.Hour,
				getTime.Min, getTime.Sec);
	}

	if (strstr((char*) buffer, "TEWS+OPS=")) {
		//0=tanggal 1=jam 2=menit 3=detik 4=sekali per detik
		uint8_t _Mode = 1;
		uint8_t _Date = rtcTEWS2.Date;
		uint8_t _Hour = rtcTEWS2.Hour;
		uint8_t _Min  = rtcTEWS2.Min;
		uint8_t _Sec  = rtcTEWS2.Sec;

		if (strstr((char*) buffer, "NORMAL")) {
			operationmode = NORMALOPS;
			write_eeprom_byte(&hi2c1, 0x04, &operationmode);
			// set alarm jam 21:17:00 UTC
			_Hour = 21;
			_Min  = 17;
			usb_universal_printf("<USB>MODE NORMAL, Alarm Set : %.2d:%.2d:%.2d UTC\r\n",_Hour,_Min,_Sec);
			DS3231_SetAlarm1_B(_Mode, _Date, _Hour, _Min, _Sec);
		} else if (strstr((char*) buffer, "DRIFTING")) {
			operationmode = DRIFTINGOPS;
			write_eeprom_byte(&hi2c1, 0x04, &operationmode);
			// set alarm per 1 jam
			alarm_now = _Hour + 1;
			if(alarm_now > 23){
				alarm_now = alarm_now - 24;
			}
			usb_universal_printf("<USB>MODE DRIFTING, Alarm Set : %.2d\r\n",alarm_now);
			DS3231_SetAlarm1_B(_Mode, _Date, alarm_now, _Min, _Sec);
		} else if (strstr((char*) buffer, "TESTING")) {
			operationmode = HOLDOPS;
			write_eeprom_byte(&hi2c1, 0x04, &operationmode);
			// set alarm per 3 jam
			alarm_now = _Hour + periodsendloc;
			if(alarm_now > 23){
				alarm_now = alarm_now - 24;
			}
			usb_universal_printf("<USB>MODE TESTING, Alarm Set : %.2d\r\n",alarm_now);
			DS3231_SetAlarm1_B(_Mode, _Date, alarm_now, _Min, _Sec);
		}
		usb_universal_printf("OK\r\n");
	}

	if (strstr((char*) buffer, "TEWS+OPS?")) {
		if (operationmode == 0)
			usb_universal_printf("OPS: NORMAL\r\n");
		if (operationmode == 2)
			usb_universal_printf("OPS: DRIFTING\r\n");
		if (operationmode == 3)
			usb_universal_printf("OPS: LEAK\r\n");
		if (operationmode == 4)
			usb_universal_printf("OPS: TESTING\r\n");
		if (operationmode > 4)
			usb_universal_printf("OPS: ERROR\r\n");
	}

	/*
	 * Direct Access to UART in subsystem
	 */
	//usb_printf("%s\r\n", buffer);
	if (strstr((char*) buffer, "TEWS+IRIDIUM,")) {
		char *str = strstr((char*) buffer, "TEWS+IRIDIUM,");
		char *txt;
		strtok_r(str, ",", &str);
		txt = strtok_r(str, "\n\r", &str);
		usb_universal_printf("Perintah: %s\r\n",txt);
		iridium_printf("%s\r\n", txt);
	}
	if (strstr((char*) buffer, "TEWS+BT,")) {
		char *str = strstr((char*) buffer, "TEWS+BT,");
		char *txt;
		strtok_r(str, ",", &str);
		txt = strtok_r(str, "\n\r", &str);
		//usb_printf("%s\r\n", txt);
		bluetooth_printf("%s\r\n", txt);

	}
	if (strstr((char*) buffer, "TEWS+GPS,")) {
		char *str = strstr((char*) buffer, "TEWS+GPS,");
		char *txt;
		strtok_r(str, ",", &str);
		txt = strtok_r(str, "\n\r", &str);
		gps_printf("%s\r\n", txt);
	}

	///////////////////////////////////////////////////////////////////////////////////////////////////////

	/*
	 * Set Parameter debug
	 */
	if (strstr((char*) buffer, "TEWS+DEBUG=")) {
		debug = parsingdatausb(buffer, "TEWS+DEBUG=");
		write_eeprom_byte(&hi2c1, 0x05, &debug);
		usb_universal_printf("OK\r\n");
	}
	if (strstr((char*) buffer, "TEWS+DEBUG?")) {
		if (debug == 0)
			usb_universal_printf("DEBUG: NONACTIVE\r\n");
		if (debug == 1)
			usb_universal_printf("DEBUG: ACTIVE\r\n");
		if (debug > 1)
			usb_universal_printf("DEBUG: ERROR\r\n");
	}

	/*
	 * set ID BUOY
	 */
	if (strstr((char*) buffer, "TEWS+ID=")) {
		idBUOY = parsingdatausb(buffer, "TEWS+ID=");
		write_eeprom_byte(&hi2c1, 0x02, &idBUOY);
		usb_universal_printf("OK\r\n");
	}
	if (strstr((char*) buffer, "TEWS+ID?")) {
		usb_universal_printf("ID BUOY: %u\r\n", idBUOY);
	}

	if (strstr((char*) buffer, "TEWS+BTON")) {
		HAL_GPIO_WritePin(BT_CON_GPIO_Port, BT_CON_Pin, GPIO_PIN_SET);
		usb_universal_printf("BLUETOOTH ON\r\n");
	}
	if (strstr((char*) buffer, "TEWS+BTOFF")) {
		HAL_GPIO_WritePin(BT_CON_GPIO_Port, BT_CON_Pin, GPIO_PIN_RESET);
		usb_universal_printf("BLUETOOTH OFF\r\n");
	}

	if (strstr((char*) buffer, "TEWS+DEBUGUSB=")) {
		debuguni = parsingdatausb(buffer, "TEWS+DEBUGUSB=");
		write_eeprom_byte(&hi2c1, 0x3E, &debuguni);
		usb_universal_printf("OK\r\n");
	}
	if (strstr((char*) buffer, "TEWS+DEBUGIRIDIUM=")) {
		debugiridium = parsingdatausb(buffer, "TEWS+DEBUGIRIDIUM=");
		write_eeprom_byte(&hi2c1, 0x38, &debugiridium);
		usb_universal_printf("OK\r\n");
	}
	if (strstr((char*) buffer, "TEWS+DEBUGGPS=")) {
		debuggps = parsingdatausb(buffer, "TEWS+DEBUGGPS=");
		write_eeprom_byte(&hi2c1, 0x39, &debuggps);
		usb_universal_printf("OK\r\n");
	}
	if (strstr((char*) buffer, "TEWS+DEBUGBLUETOOTH=")) {
		debugbluetooth = parsingdatausb(buffer, "TEWS+DEBUGBLUETOOTH=");
		write_eeprom_byte(&hi2c1, 0x3A, &debugbluetooth);
		usb_universal_printf("OK\r\n");
	}
	if (strstr((char*) buffer, "TEWS+DEBUGUSB?")) {
		usb_universal_printf("DEBUG USB: %u\r\n", debuguni);
	}
	if (strstr((char*) buffer, "TEWS+DEBUGBLUETOOTH?")) {
		usb_universal_printf("DEBUG BLUETOOTH: %u\r\n", debugbluetooth);
	}
	if (strstr((char*) buffer, "TEWS+DEBUGIRIDIUM?")) {
		usb_universal_printf("DEBUG IRIDIUM: %u\r\n", debugiridium);
	}
	if (strstr((char*) buffer, "TEWS+DEBUGGPS?")) {
		usb_universal_printf("DEBUG GPS: %u\r\n", debuggps);
	}

	if (strstr((char*) buffer, "TEWS+DEBUGSTATUS?")) {
		usb_universal_printf("DEBUG IRIDIUM: %u\r\n", debugiridium);
		usb_universal_printf("DEBUG USB: %u\r\n", debuguni);
		usb_universal_printf("DEBUG BLUETOOTH: %u\r\n", debugbluetooth);
		usb_universal_printf("DEBUG GPS: %u\r\n", debuggps);
	}

	if (strstr((char*) buffer, "TEWS+LIST"))
		PrintParameter();

	if (strstr((char*) buffer, "TEWS+SIGNALIRIDIUM?")) {
		if (HAL_GPIO_ReadPin(NETWORKINT_GPIO_Port, NETWORKINT_Pin))
			usb_universal_printf("IRIDIUM SIGNAL AVAILABLE\r\n");
		else
			usb_universal_printf("IRIDIUM SIGNAL NOT AVAILABLE\r\n");
	}

	if (strstr((char*) buffer, "TEWS+IRIDIUMSTATUS?")) {
		usb_universal_printf(
				"Iridium Queue: %d || Iridium Status: %d || Iridium Status Receive: %d \r\n",
				iridiumqueuenumber, IRIDIUM.status, IRIDIUM.statusreceive);
	}

	if (strstr((char*) buffer, "TEWS+RTC")) {
		osSemaphoreRelease(rtcBinarySemHandle);
	}

	if (strstr((char*) buffer, "TEWS+SENDIRIDIUM,")) {
		char *str = strstr((char*) buffer, "TEWS+SENDIRIDIUM,");
		strtok_r(str, ",", &str);
		//akustik_printf("%s\r\n", str);
		uint16_t sizedata = strlen(str);
		osMessageQueuePut(sizedatairidiumQueueHandle, &sizedata, 0U, 0U);
		osMessageQueuePut(iridiumsendQueueHandle, str, 0U, 0U);
	}

	if (strstr((char*) buffer, "TEWS+LEAKTEST")) {
		uint8_t statusleak;
		statusleak = HAL_GPIO_ReadPin(LEAKAGE_GPIO_Port,
					LEAKAGE_Pin);
		usb_universal_printf("LEAK STATUS: %d\r\n", statusleak);
	}

	if (strstr((char*) buffer, "+DISCONNECTED")) {
		usb_printf("Bluetooth disconnected.\r\n");
		//BT_Sequence();
	}
}

void PrintParameter(void) {
	usb_universal_printf("==================================\r\n");
	usb_universal_printf("List Parameter GPS Tracker\r\n");
	usb_universal_printf("==================================\r\n");
	usb_universal_printf("ID TRACKER = %u\r\n", idBUOY);
	if (operationmode == 0)
		usb_universal_printf("OPS = NORMAL\r\n");
	if (operationmode == 2)
		usb_universal_printf("OPS = DRIFTING\r\n");
	if (operationmode == 3)
		usb_universal_printf("OPS = LEAK\r\n");
	if (operationmode == 4)
		usb_universal_printf("OPS = TESTING\r\n");
	if (operationmode > 4)
		usb_universal_printf("OPS = ERROR\r\n");

	if(debugiridium == 0)
		usb_universal_printf("DEBUG IRIDIUM = DISABLE\r\n");
	if(debugiridium == 1)
		usb_universal_printf("DEBUG IRIDIUM = ENABLE\r\n");
	if(debugiridium > 1)
		usb_universal_printf("DEBUG IRIDIUM = ERROR\r\n");

	if(debuguni == 0)
		usb_universal_printf("DEBUG USB = DISABLE\r\n");
	if(debuguni == 1)
		usb_universal_printf("DEBUG USB = ENABLE\r\n");
	if(debuguni > 1)
		usb_universal_printf("DEBUG USB = ERROR\r\n");

	if(debugbluetooth == 0)
		usb_universal_printf("DEBUG BLUETOOTH = DISABLE\r\n");
	if(debugbluetooth == 1)
		usb_universal_printf("DEBUG BLUETOOTH = ENABLE\r\n");
	if(debugbluetooth > 1)
		usb_universal_printf("DEBUG BLUETOOTH = ERROR\r\n");

	if(debuggps == 0)
		usb_universal_printf("DEBUG GPS = DISABLE\r\n");
	if(debuggps == 1)
		usb_universal_printf("DEBUG GPS = ENABLE\r\n");
	if(debuggps > 1)
		usb_universal_printf("DEBUG GPS = ERROR\r\n");

	usb_universal_printf("TRANSMIT PERIOD = %lu minute\r\n",periodsendloc);
	usb_universal_printf("DISTANCE DRIFTING = %lu\r\n",distancedrifting);
	usb_universal_printf("LATITUDE = %f\r\n",latitude);
	usb_universal_printf("LONGITUDE = %f\r\n",longitude);
	DS3231_GetTime(&rtcTEWS);
	usb_universal_printf("DSTIME = %.2d:%.2d:%.2d  %.2d/%.2d/20%.2d \r\n",
			rtcTEWS.Hour, rtcTEWS.Min, rtcTEWS.Sec, rtcTEWS.Date, rtcTEWS.Month,
			rtcTEWS.Year);
	usb_universal_printf("==================================\r\n");
}

void PrintOK(void) {
	char bufferok[6] = ""; //reserved for print buffer usb
	memset(bufferok, 0, sizeof(bufferok));
	sprintf(bufferok, "OK\r\n");
	if (debuguni == 1)
		HAL_UART_Transmit_IT(&_UNI_USART, (uint8_t*) bufferok,
				strlen(bufferok));
	if (debugbluetooth == 1)
		HAL_UART_Transmit_IT(&_BT_USART, (uint8_t*) bufferok, strlen(bufferok));
}

void PrintSerial(char *buffer) {
	if (debuguni == 1)
		HAL_UART_Transmit(&_UNI_USART, (uint8_t*) buffer, strlen(buffer),
				10000);
	if (debugbluetooth == 1)
		HAL_UART_Transmit(&_BT_USART, (uint8_t*) buffer, strlen(buffer), 10000);
}

uint32_t parsingdatausb(char *buffer, char *keyword) {
	char *data;
	char *str = strstr((char*) buffer, keyword);
	strtok_r(str, "=", &str);
	//data = strtok_r(str, "\r", &str);
	data = str;
	return atoi(data);
}
