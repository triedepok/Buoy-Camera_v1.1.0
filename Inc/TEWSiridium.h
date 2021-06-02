/*
 * TEWSiridium.h
 *
 *  Created on: Mar 24, 2019
 *      Author: Hafiz
 */

#ifndef TEWSIRIDIUM_H_
#define TEWSIRIDIUM_H_

#include <stdint.h>

typedef struct
{
	uint8_t		rxBuffer[512];
	uint16_t	rxIndex;
	uint8_t		rxTmp;
	uint32_t	LastTime;

	char		dataBytes[14];
	uint8_t		dataBin[14];
	uint8_t		dataBinTsu[14];
	uint16_t 	size;
	uint16_t 	sizeBat;
	uint16_t 	sizeLoc;

	uint8_t		dataLoc[15];
	uint8_t		dataBat[15];

	//Brand New Iridium
	uint8_t		status;
	uint8_t 	statusreceive;
	uint8_t		senddatatype;
	uint8_t		datasendbuffer[40];
	uint8_t 	sizedata;
}IRIDIUM_t;

extern IRIDIUM_t IRIDIUM;
extern uint8_t statussend;

void IRIDIUM_Init(void);
void IRIDIUM_Callback(void);
void IRIDIUM_Loop(void);
void IRIDIUM_Process(char* buffer);
void sendtext(char *data);
void sendbin(void);
void sendtsu(void);
void sendBat(void);
void senddata(char *data, uint16_t sizedata);
void sendLoc(void);
void ProcessReceiveIridium(char* buffer);
uint32_t parsingdatairidium(char* buffer, char* keyword);
void checksumiridium(char* buffer, char* data);

#endif /* TEWSIRIDIUM_H_ */
