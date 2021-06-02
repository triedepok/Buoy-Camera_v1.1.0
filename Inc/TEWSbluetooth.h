/*
 * TEWSbluetooth.h
 *
 *  Created on: Mar 24, 2019
 *      Author: Hafiz
 */

#ifndef TEWSBLUETOOTH_H_
#define TEWSBLUETOOTH_H_

#include <stdint.h>

typedef struct
{
	uint8_t		rxBuffer[512];
	uint16_t	rxIndex;
	uint8_t		rxTmp;
	uint32_t	LastTime;

	char		dataBytes[17];
	uint16_t 	size;
	uint8_t		waitresponse;
}BT_t;

extern BT_t BT;

void BT_Init(void);
void BT_Callback(void);

#endif /* TEWSBLUETOOTH_H_ */
