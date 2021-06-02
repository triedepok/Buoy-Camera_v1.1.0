/*
 * TEWSusb.h
 *
 *  Created on: Mar 24, 2019
 *      Author: Hafiz
 */

#ifndef TEWSUSB_H_
#define TEWSUSB_H_

#include <stdint.h>

typedef struct
{
	uint8_t		rxBuffer[512];
	uint16_t	rxIndex;
	uint8_t		rxTmp;
	uint32_t	LastTime;

	//GPGGA_t		GPGGA;
	//GPRMC_t		GPRMC;
}USB_t;

extern USB_t USB;

//################################################################################################

void USB_Init(void);
void USB_Callback(void);
void USB_Process(char *buffer);
void PrintParameter(void);
void PrintOK(void);
void PrintSerial(char *buffer);
uint32_t parsingdatausb(char* buffer, char* keyword);
//################################################################################################

#endif /* TEWSUSB_H_ */
