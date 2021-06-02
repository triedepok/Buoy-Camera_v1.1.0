/*
 * TEWSgps.h
 *
 *  Created on: Apr 4, 2019
 *      Author: Hafiz
 */

#ifndef TEWSGPS_H_
#define TEWSGPS_H_

#include <stdint.h>

typedef struct
{
	uint8_t		rxBuffer[1024];
	uint16_t	rxIndex;
	uint8_t		rxTmp;
	uint32_t	LastTime;

	char		dataBytes[17];
	uint16_t 	size;
	uint8_t		waitresponse;

	float		latitude;
	float		longitude;
	uint32_t	timestamp;
	uint8_t		validstatus;
	uint8_t		hour;
	uint8_t		minute;
	uint8_t		second;
	uint8_t		numsat;
	uint8_t		hdop;
	uint16_t	altitude;
	uint8_t		date;
	uint8_t 	month;
	uint8_t 	year;
}GPS_t;

extern GPS_t GPS;

double convertDegMinToDecDeg (float degMin);
void GPS_Init(void);
void GPS_Callback(void);
void GPS_Process(char* buffer);
double CalculateDistance( double nLat1, double nLon1, double nLat2, double nLon2 );
uint32_t coordinateToUINT32(double value);
void checksumgps(char *buffer, char *data);
double distance(double lat1, double lon1, double lat2, double lon2, char unit);

#endif /* TEWSGPS_H_ */
