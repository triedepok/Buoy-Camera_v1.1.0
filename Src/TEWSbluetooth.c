/*
 * TEWSbluetooth.c
 *
 *  Created on: Mar 24, 2019
 *      Author: Hafiz
 */

#include "TEWSbluetooth.h"
#include "parameterTEWS.h"

extern uint32_t threshold;
extern uint32_t periodsampling;
extern uint32_t periodsenddata;
extern uint32_t periodsendloc;
extern uint32_t periodsendbat;
extern uint32_t timebacknormal;

extern uint8_t mode; //default
extern uint8_t debug;
extern uint8_t debugbt;
extern uint8_t mulai;
extern uint8_t idOBU;
extern uint8_t idBUOY;
extern uint8_t idCBT;
extern uint8_t operationmode;
extern uint8_t countertsunami;
extern uint8_t calculationstatus;
extern uint8_t sendmodeiridium;

extern float latitude ;
extern float longitude ;
extern uint32_t distancedrifting ;
extern uint8_t ledstatus;
extern uint8_t debugbpr;
extern uint8_t debugakustik;
extern uint8_t debugiridium;
extern uint8_t debuggps;
extern uint8_t debugbluetooth;
extern uint8_t debuguni;
extern uint8_t syncgps;
extern uint8_t AccelerometerStatus;
extern uint8_t kalibrasibat;
extern uint32_t accth;
extern uint32_t acccounter;
extern uint32_t timebacknormalstatus;

extern uint8_t synctimertc;
extern uint8_t minbat;
extern uint8_t hourbat;
extern uint8_t minloc;
extern uint8_t hourloc;
extern uint8_t periodmin;
extern uint8_t driftcalc;
extern uint8_t counterwaitbpr;
extern uint8_t AISstatus;

BT_t BT;
char printBT[512] = "print BT \n"; //reserved for print buffer iridium
extern char printusb[512]; //reserved for print buffer iridium
extern char FIRMWARE[6];

extern RTC_TimeTypeDef stimeststuctureget;
extern _RTC rtcTEWS;

extern IRIDIUM_t IRIDIUM;
extern GPS_t GPS;

void BT_Init(void){
	BT.rxIndex=0;
	HAL_UART_Receive_DMA(&_BT_USART,&BT.rxTmp,1);
}

void BT_Callback(void){
	if(BT.rxIndex < sizeof(BT.rxBuffer)-2)
	{
		BT.rxBuffer[BT.rxIndex] = BT.rxTmp;
		BT.rxIndex++;
	}
	if(BT.rxTmp == 13){
		if (HAL_GPIO_ReadPin(BT_CON_GPIO_Port, BT_CON_Pin)){
			usb_printf("%s\r\n", BT.rxBuffer);
		}
		osMessageQueuePut(usbQueueHandle, &BT.rxBuffer, 0U, 0U);
		memset(BT.rxBuffer,0,sizeof(BT.rxBuffer));
		BT.rxIndex=0;
	}

	HAL_UART_Receive_DMA(&_BT_USART,&BT.rxTmp,1);
}
