/*
 * parameterTEWS.h
 *
 *  Created on: Mar 25, 2019
 *      Author: Hafiz
 */

#ifndef PARAMETERTEWS_H_
#define PARAMETERTEWS_H_

#include "main.h"
#include "cmsis_os.h"
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <math.h>
#include "TEWSbluetooth.h"
#include "TEWSdata.h"
#include "TEWSeeprom.h"
#include "TEWSencdec.h"
#include "TEWSgps.h"
#include "TEWSiridium.h"
#include "TEWSled.h"
#include "TEWSrtc.h"
#include "TEWSusb.h"
#include "TEWSvoltage.h"
#include "stm32l4xx_hal.h"
#include "myprint.h"

#define MASK(x)		((unsigned char)(1<<x))

extern I2C_HandleTypeDef hi2c1;
extern I2C_HandleTypeDef hi2c2;
extern I2C_HandleTypeDef hi2c3;
extern I2C_HandleTypeDef hi2c4;

extern IWDG_HandleTypeDef hiwdg;

extern RTC_HandleTypeDef hrtc;
extern UART_HandleTypeDef hlpuart1;
extern UART_HandleTypeDef huart4;
extern UART_HandleTypeDef huart1;
extern UART_HandleTypeDef huart2;
extern UART_HandleTypeDef huart3;

//extern MDMA_HandleTypeDef hmdma_mdma_channel40_sdmmc1_end_data_0;

extern osMessageQueueId_t iridiumQueueHandle;
extern osMessageQueueId_t usbQueueHandle;
extern osMessageQueueId_t gpsQueueHandle;
extern osMessageQueueId_t iridiumsendQueueHandle;
extern osMessageQueueId_t iridiumrecvQueueHandle;
extern osMessageQueueId_t decodedataQueueHandle;
extern osMessageQueueId_t crcdataQueueHandle;
extern osMessageQueueId_t sizedatairidiumQueueHandle;
extern osMessageQueueId_t sizedatadecodeQueueHandle;

extern osSemaphoreId_t iridiumrecvBinarySemHandle;
extern osSemaphoreId_t rtcBinarySemHandle;



#define	_IRIDIUM_USART				huart1
#define	_IRIDIUM_DEBUG				1
#define	_BT_USART					huart4
#define _BT_DEBUG					1
#define	_UNI_USART					hlpuart1
#define _UNI_DEBUG					1
#define	_AKUSTIK_USART				huart3
#define	_AKUSTIK_DEBUG				1
#define	_GPS_USART					huart2

#define MAX_TX_PACKET_SIZE			120

#define USB_TO_IRIDIUM_DEBUG		0
#define USB_TO_BPR_DEBUG			0
#define USB_TO_AKUSTIK_DEBUG		0

#define SERIALNUMBER	20210115

enum mode{
	BUOY,
	OBU,
	CBT,
	ENCLOSURE
};

enum debug{
	NONACTIVE,
	ACTIVE
};

enum requeststatus{
	IDLE,
	PRESSURE,
	TEMPERATURE
};

enum waitSBDWB{
	NO,
	YES
};

enum StatusSend{
	IDLESEND,
	PROCESS,
	WAITSIGNAL,
	DONE,
	ERRORSEND
};

enum OperationMode{
	NORMALOPS,
	TSUNAMIOPS,
	DRIFTINGOPS,
	LEAKOPS,
	HOLDOPS
};

enum CalculationStatus{
	SAMPLING_NO_CALCULATION,
	SAMPLING_WITH_CALCULATION
};

enum SendModeIridium{
	TEXT,
	BIN
};

enum WaitResponse{
	NONEWAIT,
	WAITRESPONSE
};

enum SendDataType{
	PRESSUREX,
	TSUNAMIX,
	LOCATION,
	BATTERY
};

enum AkustikMode{
	ONLINE,
	COMMAND
};

enum AkustikCommandModeType{
	TransmitCommandAkustik,
	ReceiveCommandAkustik
};

enum AkustikTransmitCommand{
	ATV,
	ATS6,
	ATS4,
	ATJ,
	ATX,
	idleAkustikTransmitCommand
};

enum StatusIridium{
	Idle,
	SBDWB,
	Binarysend,
	SBDWT,
	SBDIX,
	SBDIXA,
	SBDI,
	SBDRB,
	SBDRT,
	SBDD0,
	SBDD1,
	WaitSBDD01,
	Waitsignal,
	ProsesSend,
	WaitresponseSBDIX
};
enum StatusReceive{
	IDLERECEIVE,
	SBDRING,
	PROCESSRECEIVE,
	OPENRECEIVE,
	NEEDCHECKRECEIVE
};

enum statuswaitresponsebpr{
	WAITDATABPR,
	RETRYGETBPR,
	NONEWAITDATABPR,
	ALREADYGETDATABPR
};

typedef struct
{
	uint32_t		BatBUOY;
	uint32_t		BatOBU;
}BAT_t;


extern uint32_t threshold;
extern uint32_t periodsampling;
extern uint32_t periodsenddata;
extern uint32_t periodsendloc;
extern uint32_t old_periodsendloc;
extern uint32_t periodsendbat;
extern uint32_t timebacknormal;

extern uint8_t mode; //default
extern uint8_t debug;
extern uint8_t debugbt;
extern uint8_t mulai;
extern uint8_t idBUOY;
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

extern uint8_t statussend;
extern uint8_t statusreceive;
extern uint8_t receivequeue;

extern uint8_t flagBPRtimeOut;

extern uint32_t acccounterstatus;
extern uint8_t accstreamstatus;
extern uint32_t timebacknormalstatus;

extern uint8_t statuswaitresponsebpr;

extern uint8_t hourbpr;
extern uint8_t minbpr;
extern uint8_t secbpr;
extern uint8_t datebpr;
extern uint8_t monthbpr;
extern uint8_t yearbpr;

extern uint8_t bufferbprstatus;

extern IRIDIUM_t IRIDIUM;
extern _RTC rtcTEWS;
extern _RTC rtcTEWS2;
extern DATAENGINEERING_t DATAENGINEERING;
//extern ACC_t ACC;
BAT_t BAT;

extern float latitude ;
extern float longitude ;
extern uint32_t distancedrifting ;
extern char datakirim[512];

extern uint8_t nbtry;
extern uint8_t statusbuoy;

extern uint8_t poweracoustic;
extern uint8_t rateacoustic;
extern uint8_t iridiumqueuenumber;

extern uint8_t statusSendGPS;
extern uint8_t ds_wakeup;
extern uint8_t alarm_now;

/*
 * LED Config
 * RED = System full operation indicator; Toggle in loop, ON when system active
 * BLUE = Iridium Signal indicator; if ON then signal good; else signal bad
 * GREEN = Data Indicator; OBU blink for BPR; BUOY blink for IRIDIUM; CBT blink for BPR
 */

#endif /* PARAMETERTEWS_H_ */
