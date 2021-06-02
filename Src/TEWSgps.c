/*
 * TEWSgps.c
 *
 *  Created on: Apr 4, 2019
 *      Author: Hafiz
 */

#include "TEWSgps.h"
#include "parameterTEWS.h"

GPS_t GPS;
char printGPS[512] = "print GPS \n"; //reserved for print buffer iridium

extern uint8_t driftcalc;
//_RTC rtcTEWS2;

//kuyngepush

double convertDegMinToDecDeg(float degMin) {
	double min = 0.0;
	double decDeg = 0.0;

	//get the minutes, fmod() requires double
	min = fmod((double) degMin, 100.0);

	//rebuild coordinates in decimal degrees
	degMin = (int) (degMin / 100);
	decDeg = degMin + (min / 60);

	return decDeg;
}

void GPS_Init(void) {
	GPS.rxIndex = 0;

	HAL_GPIO_WritePin(GPIOD, GPS_RESET_Pin, GPIO_PIN_SET); //enable GPS
	osDelay(50);
	HAL_GPIO_WritePin(GPIOD, GPS_RESET_Pin, GPIO_PIN_RESET); //enable GPS
	osDelay(50);
	HAL_GPIO_WritePin(GPIOD, GPS_RESET_Pin, GPIO_PIN_SET); //enable GPS

	//wait for reset to apply
	osDelay(2000);

	//HAL_GPIO_WritePin(GPIOD, GPS_RESET_Pin, GPIO_PIN_SET); //enable GPS

	HAL_UART_Receive_DMA(&_GPS_USART, &GPS.rxTmp, 1);
}

void GPS_Callback(void) {
	GPS.LastTime = HAL_GetTick();
	if (GPS.rxIndex < sizeof(GPS.rxBuffer) - 2) {
		GPS.rxBuffer[GPS.rxIndex] = GPS.rxTmp;
		GPS.rxIndex++;
	}
	if (GPS.rxTmp == 13) {
		osMessageQueuePut(gpsQueueHandle, &GPS.rxBuffer, 0U, 0U);
		memset(GPS.rxBuffer, 0, sizeof(GPS.rxBuffer));
		GPS.rxIndex = 0;
	}
	HAL_UART_Receive_DMA(&_GPS_USART, &GPS.rxTmp, 1);
}

void GPS_Process(char *buffer) {
	char *strx;
	char *timestamp;
	uint32_t timegps;
	char *glatitude;
	char *latsim;
	char *glongitude;
	char *longsim;

	if (debuggps == 1) {
		if (strstr((char*) buffer, "$PSTM")) {
			usb_universal_printf("%s\r\n",buffer);
		}
	}

	if (strstr((char*) buffer, "$GPRMC,")) {
		if (debuggps == 1) {
			if (debuguni == 1)
				usb_universal_printf("%s\r\n", buffer);
			if (debugbluetooth == 1)
				usb_universal_printf("%s\r\n", buffer);
		}

		char *GPRMC;
		char *date;

		GPRMC = strstr(buffer, "$GPRMC,"); //$GPRMC
		strtok_r(GPRMC, ",", &GPRMC);
		timestamp = strtok_r(GPRMC, ",", &GPRMC);
		strtok_r(GPRMC, ",", &GPRMC);
		glatitude = strtok_r(GPRMC, ",", &GPRMC);
		latsim = strtok_r(GPRMC, ",", &GPRMC);
		glongitude = strtok_r(GPRMC, ",", &GPRMC);
		longsim = strtok_r(GPRMC, ",", &GPRMC);
		strtok_r(GPRMC, ",", &GPRMC);
		strtok_r(GPRMC, ",", &GPRMC);
		date = strtok_r(GPRMC, ",", &GPRMC);

		timegps = atoi(timestamp);
		GPS.hour = timegps / 10000;
		GPS.minute = (timegps % 10000) / 100;
		//GPS.minute 	= GPS.minute/100;
		GPS.second = timegps % 100;

		GPS.date = valueFromString(date, 0, 2);
		GPS.month = valueFromString(date, 2, 2);
		GPS.year = valueFromString(date, 4, 2);
	}


	strx = strstr(buffer, "$GPGGA,"); //GPGGA
	if (strx != NULL) {
		if (debuggps == 1) {
			if (debuguni == 1)
				usb_universal_printf("%s\r\n", buffer);
			if (debugbluetooth == 1)
				usb_universal_printf("%s\r\n", buffer);
		}

		char *validstatus;
		char *numsat;
		char *hdop;
		char *altitude;
		char *GPGGA;
		float flatitude, flongitude;

		GPGGA = strstr(buffer, "$GPGGA,"); //GPGGA
		strtok_r(GPGGA, ",", &GPGGA);
		timestamp = strtok_r(GPGGA, ",", &GPGGA);
		glatitude = strtok_r(GPGGA, ",", &GPGGA);
		latsim = strtok_r(GPGGA, ",", &GPGGA);
		glongitude = strtok_r(GPGGA, ",", &GPGGA);
		longsim = strtok_r(GPGGA, ",", &GPGGA);
		validstatus = strtok_r(GPGGA, ",", &GPGGA);
		numsat = strtok_r(GPGGA, ",", &GPGGA);
		hdop = strtok_r(GPGGA, ",", &GPGGA);
		altitude = strtok_r(GPGGA, ",", &GPGGA);

		timegps = atoi(timestamp);
		GPS.hour = timegps / 10000;
		GPS.minute = (timegps % 10000) / 100;
		GPS.second = timegps % 100;
		GPS.numsat = atoi(numsat);
		GPS.hdop = (uint8_t) (atof(hdop) * 100.0);
		GPS.altitude = (uint16_t) (atof(altitude) * 10.0);

		//validation for gps message
		if (atoi(validstatus) == 1 || atoi(validstatus) == 2) {
			if (strcmp(latsim, "S") == 0)
				flatitude = -1 * atof(glatitude);
			else
				flatitude = atof(glatitude);
			if (strcmp(longsim, "W") == 0)
				flongitude = -1 * atof(glongitude);
			else
				flongitude = atof(glongitude);
			GPS.latitude = convertDegMinToDecDeg(flatitude);
			GPS.longitude = convertDegMinToDecDeg(flongitude);

			//0=tanggal 1=jam 2=menit 3=detik 4=sekali per detik
			uint8_t _Mode = 1;
			uint8_t _Date = rtcTEWS2.Date;
			uint8_t _Hour = rtcTEWS2.Hour;
			uint8_t _Min  = rtcTEWS2.Min;
			uint8_t _Sec  = rtcTEWS2.Sec;

			if (operationmode == NORMALOPS) {
				if (CalculateDistance(latitude,longitude, GPS.latitude,GPS.longitude) > distancedrifting){
					memset(printGPS, 0, sizeof(printGPS));
					sprintf(printGPS, "DRIFTING ALERT\r\n");
					uint16_t xsizedata = strlen(printGPS);
					osMessageQueuePut(sizedatairidiumQueueHandle,&xsizedata, 0U, 0U);
					osMessageQueuePut(iridiumsendQueueHandle,printGPS,0U,0U);
					usb_universal_printf("%s",printGPS);
					operationmode = DRIFTINGOPS;
					write_eeprom_byte(&hi2c1, 0x04, &operationmode);
					// set alarm per 1 jam
					_Hour = 1;
					alarm_now = _Hour + 1;
					if(alarm_now > 23){
						alarm_now = alarm_now - 24;
					}
					usb_universal_printf("<GPS CALCULATE> ON DRIFTING, Alarm Set : %.2d\r\n",alarm_now);
					DS3231_SetAlarm1_B(_Mode, _Date, alarm_now, _Min, _Sec);
				}
			}

			if (operationmode == DRIFTINGOPS) {
				if (CalculateDistance(latitude, longitude, GPS.latitude,GPS.longitude) < distancedrifting){
					memset(printGPS, 0, sizeof(printGPS));
					sprintf(printGPS, "DRIFT to NORMAL\r\n");
					uint16_t xsizedata = strlen(printGPS);
					osMessageQueuePut(sizedatairidiumQueueHandle,&xsizedata, 0U, 0U);
					osMessageQueuePut(iridiumsendQueueHandle,printGPS,0U,0U);
					usb_universal_printf("%s",printGPS);
					operationmode = NORMALOPS;
					write_eeprom_byte(&hi2c1, 0x04, &operationmode);
					// set alarm jam 21:17:00 UTC
					_Hour = 21;
					_Min  = 17;
					usb_universal_printf("<GPS CALCULATE> BACK TO NORMAL, Alarm Set : %.2d:%.2d:%.2d UTC\r\n",_Hour,_Min,_Sec);
					DS3231_SetAlarm1_B(_Mode, _Date, alarm_now, _Min, _Sec);
				}
			}
		}
	}
}

double CalculateDistance(double nLat1, double nLon1, double nLat2, double nLon2) {
	int nRadius = 6371; // Earth's radius in Kilometers
	// Get the difference between our two points
	// then convert the difference into radians
	double nDLat = (nLat2 - nLat1) * (M_PI / 180);
	double nDLon = (nLon2 - nLon1) * (M_PI / 180);
	double nA = pow(sin(nDLat / 2), 2)
			+ cos(nLat1) * cos(nLat2) * pow(sin(nDLon / 2), 2);

	double nC = 2 * atan2(sqrt(nA), sqrt(1 - nA));
	double nD = nRadius * nC;

	return nD * 1000.0; // Return our calculated distance
}

uint32_t coordinateToUINT32(double value) {
	uint32_t val = fabs(value) * 600000;
	if (value < 0.0)
		val = ~val + 1;
	return val;
}


void checksumgps(char *buffer, char *data) {
	uint16_t checksum;
	uint8_t i;

	for (i = 0; i < strlen(data); i++) {
		checksum ^= data[i];
		buffer[i] = data[i];
	}

	buffer[i] = checksum / 256;
	buffer[i + 1] = checksum % 256;
}

double deg2rad(double);
double rad2deg(double);

double distance(double lat1, double lon1, double lat2, double lon2, char unit) {
  double theta, dist;
  if ((lat1 == lat2) && (lon1 == lon2)) {
    return 0;
  }else{
    theta = lon1 - lon2;
    dist = sin(deg2rad(lat1)) * sin(deg2rad(lat2)) + cos(deg2rad(lat1)) * cos(deg2rad(lat2)) * cos(deg2rad(theta));
    dist = acos(dist);
    dist = rad2deg(dist);
    dist = dist * 60 * 1.1515;
    switch(unit) {
      case 'M':
        break;
      case 'K':
        dist = dist * 1.609344;
        break;
      case 'N':
        dist = dist * 0.8684;
        break;
      default:
    	dist = (dist * 1.609344)*1000;
    }
    return (dist);
  }
}

double deg2rad(double deg) {
  return (deg * M_PI / 180);
}

double rad2deg(double rad) {
  return (rad * 180 / M_PI);
}
