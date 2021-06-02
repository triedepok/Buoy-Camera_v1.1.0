/*
 * TEWSiridium.c
 *
 *  Created on: Mar 24, 2019
 *      Author: Hafiz
 */

#include "TEWSiridium.h"
#include "parameterTEWS.h"

IRIDIUM_t IRIDIUM;
char printiridium[512] = "print iridium \n"; //reserved for print buffer iridium

extern RTC_TimeTypeDef stimeststuctureget;
extern _RTC rtcTEWS;
char replyiridium[255] = "";
uint16_t sizedata;
extern uint8_t signaliridium;
char savedsdcardiridium[255] = "";

void IRIDIUM_Init(void) {
	IRIDIUM.status = Idle;
	//usb_universal_printf("idle karena inisialisasi..\r\n");
	IRIDIUM.statusreceive = IDLERECEIVE;
	IRIDIUM.rxIndex = 0;
	signaliridium = 0;
	statusSendGPS = 0;

	HAL_GPIO_WritePin(ON_OFF_GPIO_Port, ON_OFF_Pin, GPIO_PIN_SET); //activation IRIDIUM Transceiver
	HAL_UART_Receive_DMA(&_IRIDIUM_USART, &IRIDIUM.rxTmp, 1);
}

void IRIDIUM_Callback(void) {
	__HAL_UART_FLUSH_DRREGISTER(&_IRIDIUM_USART);

	if (IRIDIUM.rxIndex < sizeof(IRIDIUM.rxBuffer) - 2) {
		IRIDIUM.rxBuffer[IRIDIUM.rxIndex] = IRIDIUM.rxTmp;
		IRIDIUM.rxIndex++;
	}

	if (IRIDIUM.rxTmp == 13 || IRIDIUM.rxTmp == 10) {
		if (debugiridium == 1) {
			usb_universal_printf("%s",IRIDIUM.rxBuffer);
		}

		osMessageQueuePut(iridiumQueueHandle, &IRIDIUM.rxBuffer, 0U, 0U);
		memset(IRIDIUM.rxBuffer, 0, sizeof(IRIDIUM.rxBuffer));
		IRIDIUM.rxIndex = 0;
	}

	HAL_UART_Receive_DMA(&_IRIDIUM_USART, &IRIDIUM.rxTmp, 1);
}

void IRIDIUM_Loop(void) {
	signaliridium = HAL_GPIO_ReadPin(NETWORKINT_GPIO_Port, NETWORKINT_Pin);

	if (IRIDIUM.status == SBDD0) {
		//Iridium Status loop sbdd0.
		IRIDIUM.status = WaitSBDD01;
		iridium_printf("AT+SBDD0\r\n");
		usb_universal_printf("ClearBuf-Tx\r\n");
	}

	if (IRIDIUM.status == SBDD1) {
		IRIDIUM.status = WaitSBDD01;
		iridium_printf("AT+SBDD1\r\n");
		usb_universal_printf("ClearBuf-Rx\r\n");
	}

	if (IRIDIUM.status == SBDIX && signaliridium == 1) {
		//usb_universal_printf("Iridium Status loop sbdix in : %d \r\n",
		//		IRIDIUM.status);
		IRIDIUM.status = WaitresponseSBDIX;
		iridium_printf("AT+SBDIX\r\n");
		usb_universal_printf("run-IX\r\n");	//Iridium Status loop sbdix out  : %d \r\n",
		//IRIDIUM.status);
	}

	if (IRIDIUM.status == SBDIXA && signaliridium == 1) {
		IRIDIUM.status = WaitresponseSBDIX;
		iridium_printf("AT+SBDIXA\r\n");
		usb_universal_printf("XIA\r\n");
	}

	if (IRIDIUM.status == Waitsignal && signaliridium == 1) {
		//usb_universal_printf("Iridium Status waitsignal + 1 in : %d \r\n",
		//		IRIDIUM.status);
		IRIDIUM.status = SBDIX;
		usb_universal_printf("IX-again\r\n");//Iridium Status waitsignal + 1 out : %d \r\n",
		//IRIDIUM.status);
	}

	// Receive Checking
	if (IRIDIUM.statusreceive == SBDRING && IRIDIUM.status == Idle
			&& signaliridium == 1) {
		usb_universal_printf("RING-XIA\r\n");
		IRIDIUM.statusreceive = PROCESSRECEIVE;
		IRIDIUM.status = SBDIXA;
	}

	if (IRIDIUM.statusreceive == OPENRECEIVE && IRIDIUM.status == Idle) {
		usb_universal_printf("SBDRT\r\n");
		IRIDIUM.statusreceive = PROCESSRECEIVE;
		IRIDIUM.status = SBDRT;
		iridium_printf("AT+SBDRT\r\n");
	}

	if (IRIDIUM.statusreceive == NEEDCHECKRECEIVE && IRIDIUM.status == Idle
			&& signaliridium == 1) {
		usb_universal_printf("Repeat synchronize......\r\n");
		IRIDIUM.statusreceive = PROCESSRECEIVE;
		IRIDIUM.status = SBDIXA;
	}

	if (osSemaphoreGetCount(iridiumrecvBinarySemHandle) > 0
			&& IRIDIUM.status == Idle && IRIDIUM.statusreceive == IDLERECEIVE
			&& signaliridium == 1) {
		osSemaphoreRelease(iridiumrecvBinarySemHandle);
		usb_universal_printf("Get Data in Queue......\r\n");
		//IRIDIUM.statusreceive = PROCESSRECEIVE;
		//IRIDIUM.status = SBDIXA;
	}

}

void IRIDIUM_Process(char *buffer) {

	if (IRIDIUM.status == Binarysend) {
		if (strstr((char*) buffer, "0")) {
			//usb_universal_printf("Iridium Status binarysend in : %d \r\n",
			//		IRIDIUM.status);
			IRIDIUM.status = SBDIX;
			usb_universal_printf("WB-IX\r\n");//Iridium Status binarysend out : %d \r\n",
			//IRIDIUM.status);
		}
		if (strstr((char*) buffer, "1")) {
			//time out
			usb_universal_printf("Buffer Iridium Timeout\r\n");
			IRIDIUM.status = SBDD0;
		}
		if (strstr((char*) buffer, "2")) {
			//checksum false
			usb_universal_printf("Buffer Iridium checksum false\r\n");
			IRIDIUM.status = SBDD0;
		}
		if (strstr((char*) buffer, "3")) {
			//message incorrect
			usb_universal_printf("Buffer Iridium message incorrect\r\n");
			IRIDIUM.status = SBDD0;
		}
	}

	if (strstr((char*) buffer, "OK")) {
		if (IRIDIUM.status == SBDWT) {
			//usb_universal_printf("Iridium Status ok sbdwt in : %d \r\n",
			//		IRIDIUM.status);
			IRIDIUM.status = SBDIX;
			usb_universal_printf("WT-IX\r\n");//Iridium Status ok sbdwt out : %d \r\n",
			//IRIDIUM.status);
		}
		if (IRIDIUM.status == WaitSBDD01) {
			usb_universal_printf("Iridium Status ok waitsbdd01 in : %d \r\n",IRIDIUM.status);
			IRIDIUM.status = Idle;
			//usb_universal_printf(
			//"idle karena sudah diclear buffer..\r\nIridium Status ok waitsbbd01 : %d \r\n",
			//IRIDIUM.status);
		}
		if (statusSendGPS == 2) {
			// Iridium SLEEP
			HAL_GPIO_WritePin(ON_OFF_GPIO_Port, ON_OFF_Pin, GPIO_PIN_RESET);
			// GPS Sleep
			HAL_GPIO_WritePin(GPIOD, GPS_RESET_Pin, GPIO_PIN_RESET);
			statusSendGPS = 0;
			usb_printf("The Device Is Turned Off.\r\n");
			osDelay(50);
			HAL_GPIO_WritePin(SWT_TRIG_GPIO_Port, SWT_TRIG_Pin, GPIO_PIN_RESET);
			DS3231_ResetAlarm1_B();
			DS3231_ResetAlarm2_B();
		}
	}

	if (strstr((char*) buffer, "ADY")) {
		//usb_universal_printf("Iridium Status ready in : %d \r\n",
		//		IRIDIUM.status);
		IRIDIUM.status = Binarysend;
		HAL_UART_Transmit_IT(&_IRIDIUM_USART, (uint8_t*) IRIDIUM.datasendbuffer,IRIDIUM.sizedata);
		usb_universal_printf("Iridium Status ready out : %d \r\n",IRIDIUM.status);
	}

	if (strstr((char*) buffer, "300")) {
/*		if (debugiridium == 1) {
			usb_universal_printf("%s\r\n", buffer);
		}*/
	}

	/*
	 * Sending buffer to sattelite
	 */
	if (strstr((char*) buffer, "+SBDIX:")) {
		usb_universal_printf("Iridium Status sbdix in : %d \r\n",IRIDIUM.status);

/*		if (debugiridium == 1) {
			usb_universal_printf("%s\r\n", buffer);
		}*/

		char *strakustik;
		char *MOstatus;
		char *MTstatus;
		char *MTqueue;
		uint8_t intMOstatus, intMTstatus, intMTqueue;

		strakustik = strstr((char*) buffer, "+SBDIX:");
		strtok_r(strakustik, ":", &strakustik);
		MOstatus = strtok_r(strakustik, ",", &strakustik);
		strtok_r(strakustik, ",", &strakustik);
		MTstatus = strtok_r(strakustik, ",", &strakustik);
		strtok_r(strakustik, ",", &strakustik);
		strtok_r(strakustik, ",", &strakustik);
		MTqueue = strtok_r(strakustik, "\r", &strakustik);

		intMOstatus = atoi(MOstatus);
		intMTstatus = atoi(MTstatus);
		intMTqueue = atoi(MTqueue);

		// TRANSMIT STATUS ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

		switch (intMOstatus) {
		case 0:
			usb_universal_printf("oksend\r\n");
			IRIDIUM.status = SBDD0;
			break;
		case 1:
			usb_universal_printf("oksend\r\n");
			IRIDIUM.status = SBDD0;
			break;
		case 2:
			usb_universal_printf("oksend\r\n");
			IRIDIUM.status = SBDD0;
			break;
		default:
			//usb_universal_printf("FAILED send SBDIX.. Wait Signal \r\n");
			IRIDIUM.status = Waitsignal;
			break;
		}

		// RECEIVE STATUS ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		switch (intMTstatus) {
		case 0:
			IRIDIUM.statusreceive = IDLERECEIVE;
			break;
		case 1:
			usb_universal_printf(
					"Receive message from gateway ready to open\r\n");
			IRIDIUM.statusreceive = OPENRECEIVE;
			break;
		case 2:
			usb_universal_printf("Receive failed, need to check again \r\n");
			IRIDIUM.statusreceive = NEEDCHECKRECEIVE;
			break;
		default:
			break;
		}

		if (intMTqueue > 0) {
			usb_universal_printf("Queue Data from satellite available...\r\n");
			//osSemaphoreRelease(IridiumRecvBinarySemHandle);
		}
	}

	/*
	 * Receive data indicator
	 */
	if (strstr((char*) buffer, "SBDRING")) {
		IRIDIUM.statusreceive = SBDRING;
/*		if (debugiridium == 1) {
			usb_universal_printf("%s\r\n", buffer);
		}*/
	}
	/*
	 * Get Receive buffer from Sattelite
	 */
	if (strstr((char*) buffer, "+SBDIXA:")) {

/*		if (debugiridium == 1) {
			usb_universal_printf("%s\r\n", buffer);
		}*/

		char *strakustik;
		char *MTstatus;
		char *MTqueue;
		uint8_t intMTstatus, intMTqueue;

		strakustik = strstr((char*) buffer, "+SBDIXA:");
		strtok_r(strakustik, ":", &strakustik);
		strtok_r(strakustik, ",", &strakustik);
		strtok_r(strakustik, ",", &strakustik);
		MTstatus = strtok_r(strakustik, ",", &strakustik);
		strtok_r(strakustik, ",", &strakustik);
		strtok_r(strakustik, ",", &strakustik);
		MTqueue = strtok_r(strakustik, "\r", &strakustik);

		intMTstatus = atoi(MTstatus);
		intMTqueue = atoi(MTqueue);

		// TRANSMIT STATUS ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

		// RECEIVE STATUS ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		switch (intMTstatus) {
		case 0:
			IRIDIUM.statusreceive = IDLERECEIVE;
			IRIDIUM.status = Idle;
			//usb_universal_printf("idle karena SBDIX = 0..\r\n");
			break;
		case 1:
			usb_universal_printf(
					"Receive message from gateway ready to open\r\n");
			IRIDIUM.statusreceive = OPENRECEIVE;
			IRIDIUM.status = Idle;
			//usb_universal_printf("idle karena SBDIX = 1..\r\n");
			break;
		case 2:
			usb_universal_printf("Receive failed, need to check again \r\n");
			IRIDIUM.statusreceive = NEEDCHECKRECEIVE;
			IRIDIUM.status = Idle;
			//usb_universal_printf("idle karena SBDIX = 2..\r\n");
			break;
		default:
			IRIDIUM.statusreceive = IDLERECEIVE;
			break;
		}

		if (intMTqueue > 0) {
			usb_universal_printf("Queue Data from satellite available...\r\n");
			//osSemaphoreRelease(IridiumRecvBinarySemHandle);
		}
	}

	if (strstr((char*) buffer, "+SBDRT:")) {
/*		if (debugiridium == 1) {
			usb_universal_printf("%s\r\n", buffer);
		}*/
	}

	if (strstr((char*) buffer, "#")) {
		if (debugiridium == 1) {
			usb_universal_printf("TERIMA DATA: %s\r\n", buffer);
		}

		char *strx;
		strx = strstr((char*) buffer, "#");
		//osMessageQueuePut(iridiumrecvQueueHandle, &strx, 0U, 0U);
		ProcessReceiveIridium(strx);
		IRIDIUM.statusreceive = IDLERECEIVE;
		IRIDIUM.status = SBDD1;
	}

	///////////////////// EXTENDED ///////////////////////////////////////////
	///////////////////// EXTENDED ///////////////////////////////////////////
	///////////////////// EXTENDED ///////////////////////////////////////////

	if (strstr(buffer, "CSQ") || strstr(buffer, "SBDREG")) {
/*		if (debugiridium == 1) {
			usb_universal_printf("%s\r\n", buffer);
		}*/
	}
}

void sendtext(char *data) {
	char buffer[] = "AT+SBDWT=";
	strcat(buffer, data);
	strcat(buffer, "\r\n");
	HAL_UART_Transmit(&_IRIDIUM_USART, (uint8_t*) buffer, strlen(buffer),
			10000);
	IRIDIUM.status = SBDWT;
}

void senddata(char *data, uint16_t sizedata) {
	//usb_universal_printf("Iridium Status data in : %d \r\n", IRIDIUM.status);
	memset(IRIDIUM.datasendbuffer, 0, sizeof(IRIDIUM.datasendbuffer));

	uint16_t scheck;
	int i;

	for (i = 0; i < sizedata; i++) {
		scheck += data[i];
		IRIDIUM.datasendbuffer[i] = data[i];
	}
	IRIDIUM.datasendbuffer[sizedata] = scheck / 256;
	IRIDIUM.datasendbuffer[sizedata + 1] = scheck % 256;
	IRIDIUM.sizedata = sizedata + 2;
	usb_universal_printf("Start SBDWB\r\n");//Iridium Status data out : %d \r\n",
	//		IRIDIUM.status);
	iridium_printf("AT+SBDWB=%u\r\n", sizedata);
	IRIDIUM.status = SBDWB;
}

void ProcessReceiveIridium(char *buffer) {
	char *strx;

	if (strstr(buffer, "GPSLOC?")) {
		usb_universal_printf("<IRIDIUM>Permintaan data lokasi di proses\r\n");
		EncodeTEWSLocBuoy(0, 15);
	}

	if (strstr(buffer, "BATGPS?")) {
		usb_universal_printf("<IRIDIUM>Permintaan Status Baterai di proses\r\n");
		char str[128];
		sprintf(str,"VOLT:  %lu V, CURRENT:  %lu mA\r\n", getvoltage(), getcurrent());
		uint16_t sizedata = strlen(str);
		osMessageQueuePut(sizedatairidiumQueueHandle, &sizedata, 0U, 0U);
		osMessageQueuePut(iridiumsendQueueHandle, str, 0U, 0U);
	}

	if (strstr(buffer, "TPERIOD?")) {
		usb_universal_printf("<IRIDIUM>Permintaan Transmite Periode di proses\r\n");
		char str[128];
		sprintf(str,"TPERIOD:  %lu\r\n", periodsendloc);
		uint16_t sizedata = strlen(str);
		osMessageQueuePut(sizedatairidiumQueueHandle, &sizedata, 0U, 0U);
		osMessageQueuePut(iridiumsendQueueHandle, str, 0U, 0U);
	}

	if (strstr((char*) buffer, "TPERIOD=")) {
		threshold = parsingdatairidium(buffer, "TPERIOD=");
		write_eeprom_union(&hi2c1, 0x18, periodsendloc);
		usb_universal_printf("<IRIDIUM>TPERIOD SET: %lu\r\n",periodsendloc);
	}

	if (strstr(buffer, "RPERIOD?")) {
		usb_universal_printf("<IRIDIUM>Permintaan Recaive Periode di proses\r\n");
		char str[128];
		sprintf(str,"RPERIOD:  %lu\r\n", periodsenddata);
		uint16_t sizedata = strlen(str);
		osMessageQueuePut(sizedatairidiumQueueHandle, &sizedata, 0U, 0U);
		osMessageQueuePut(iridiumsendQueueHandle, str, 0U, 0U);
	}

	if (strstr((char*) buffer, "RPERIOD=")) {
		threshold = parsingdatairidium(buffer, "RPERIOD=");
		write_eeprom_union(&hi2c1, 0x14, periodsenddata);
		usb_universal_printf("<IRIDIUM>RPERIOD SET: %lu\r\n",periodsenddata);
	}


/*=========================================================================================*/

	if (strstr(buffer, "OPS=")) {
		operationmode = parsingdatairidium(buffer, "OPS=");
		write_eeprom_byte(&hi2c1, 0x04, &operationmode);

		//0=tanggal 1=jam 2=menit 3=detik 4=sekali per detik
		uint8_t _Mode = 1;
		uint8_t _Date = 0;
		uint8_t _Hour = 0;
		uint8_t _Min  = 0;
		uint8_t _Sec  = 0;
		switch(operationmode){
		case 0:
			// set alarm jam 21:17:00 UTC
			_Hour = 21;
			_Min  = 17;
			usb_universal_printf("<IRIDIUM> MODE NORMAL, Alarm Set : %.2d:%.2d:%.2d UTC\r\n",_Hour,_Min,_Sec);
			DS3231_SetAlarm1_B(_Mode, _Date, alarm_now, _Min, _Sec);
			break;
		case 2:
			alarm_now = rtcTEWS2.Hour + 1;
			if(alarm_now > 23){
				alarm_now = alarm_now - 24;
			}
			usb_universal_printf("<IRIDIUM> MODE DRIFTING, Alarm Set : %.2d H\r\n",alarm_now);
			DS3231_SetAlarm1_B(_Mode, _Date, alarm_now, _Min, _Sec);
			break;
		case 4:
			alarm_now = rtcTEWS2.Hour + periodsendloc;
			if(alarm_now > 23){
				alarm_now = alarm_now - 24;
			}
			usb_universal_printf("<IRIDIUM> MODE TESTING, Alarm Set : %.2d H\r\n",alarm_now);
			DS3231_SetAlarm1_B(_Mode, _Date, alarm_now, _Min, _Sec);
			break;
		}
	}
	/*if (strstr(buffer, "OPSOBU=")) {
	 uint8_t opsobu;
	 opsobu = parsingdatairidium(buffer, "OPSOBU=");
	 if (opsobu == 0) {
	 akustik_printf("OPS=0\r\n");
	 memset(savedsdcardiridium, 0, sizeof(savedsdcardiridium));
	 sprintf(savedsdcardiridium,
	 "4,0,0,[ Send OBU ] SET OBU OPERATION MODE = 0 \n");
	 osMessageQueuePut(sdcardQueueHandle, savedsdcardiridium, 0U, 0U);
	 }
	 if (opsobu == 1) {
	 akustik_printf("OPS=1\r\n");
	 memset(savedsdcardiridium, 0, sizeof(savedsdcardiridium));
	 sprintf(savedsdcardiridium,
	 "4,0,0,[ Send OBU ] SET OBU OPERATION MODE = 1 \n");
	 osMessageQueuePut(sdcardQueueHandle, savedsdcardiridium, 0U, 0U);
	 }
	 }*/

	if (strstr(buffer, "CTS=")) {
		countertsunami = parsingdatairidium(buffer, "CTS=");
		write_eeprom_byte(&hi2c1, 0x08, &countertsunami);
	}

	if (strstr(buffer, "ID=")) {
		idBUOY = parsingdatairidium(buffer, "ID=");
		write_eeprom_byte(&hi2c1, 0x02, &idBUOY);
	}

	if (strstr(buffer, "TBN=")) {
		timebacknormal = parsingdatairidium(buffer, "TBN=");
		timebacknormalstatus = timebacknormal;
		write_eeprom_union(&hi2c1, 0x20, timebacknormal);
	}

	if (strstr(buffer, "DATAENG?")) {
		EncodeTEWSEngBuoy(7);
	}

	/*if (strstr(buffer, "BATBUOY?")) {
	 DS3231_GetTime(&rtcTEWS);
	 uint32_t timestamp = (rtcTEWS.Hour * 60 * 60) + (rtcTEWS.Min * 60)
	 + (rtcTEWS.Sec);

	 float voltf = getvoltagebat() * 100.0;
	 uint32_t volt = (uint32_t) voltf;

	 EncodeTEWSBat(IRIDIUM.dataBat, &IRIDIUM.sizeBat, idBUOY, timestamp,
	 volt, 0);
	 sendBat();
	 memset(savedsdcardiridium, 0, sizeof(savedsdcardiridium));
	 sprintf(savedsdcardiridium,
	 "4,1,0,[ Send IRIDIUM ] REQUEST BUOY BATTERY \n");
	 osMessageQueuePut(sdcardQueueHandle, savedsdcardiridium, 0U, 0U);
	 }*/

	if (strstr(buffer, "RBB")) {
		NVIC_SystemReset();
	}

	if (strstr(buffer, "DIST=")) {
		distancedrifting = parsingdatausb(buffer, "TEWS+DIST=");
		write_eeprom_union(&hi2c1, 0x24, distancedrifting);
		usb_universal_printf("OK\r\n");
		memset(replyiridium, 0, sizeof(replyiridium));
		sprintf(replyiridium, "DISTANCE DRIFTING: %lu\r\n", distancedrifting);
		sizedata = strlen(replyiridium);
		osMessageQueuePut(sizedatairidiumQueueHandle, &sizedata, 0U, 0U);
		osMessageQueuePut(iridiumsendQueueHandle, replyiridium, 0U, 0U);
	}
	if (strstr(buffer, "DIST?")) {
		usb_universal_printf("DISTANCE DRIFTING: %lu\r\n", distancedrifting);
		memset(replyiridium, 0, sizeof(replyiridium));
		sprintf(replyiridium, "DISTANCE DRIFTING: %lu\r\n", distancedrifting);
		sizedata = strlen(replyiridium);
		osMessageQueuePut(sizedatairidiumQueueHandle, &sizedata, 0U, 0U);
		osMessageQueuePut(iridiumsendQueueHandle, replyiridium, 0U, 0U);
	}

	if (strstr(buffer, "RTC=")) {
		synctimertc = parsingdatairidium(buffer, "RTC=");
		write_eeprom_byte(&hi2c1, 0x48, &synctimertc);
	}
	if (strstr(buffer, "RTC?")) {
		//iridium_printf("AT+SBDWT=RTC:%u\r\n", synctimertc);
		memset(replyiridium, 0, sizeof(replyiridium));
		sprintf(replyiridium, "RTC: %u\r\n", synctimertc);
		sizedata = strlen(replyiridium);
		osMessageQueuePut(sizedatairidiumQueueHandle, &sizedata, 0U, 0U);
		osMessageQueuePut(iridiumsendQueueHandle, replyiridium, 0U, 0U);
	}
	if (strstr(buffer, "PMIN=")) {
		periodmin = parsingdatairidium(buffer, "PMIN=");
		write_eeprom_byte(&hi2c1, 0x4D, &periodmin);
	}
	if (strstr(buffer, "PMIN?")) {
		//iridium_printf("AT+SBDWT=PMIN:%u\r\n", periodmin);
		//IRIDIUM.status = SBDWT;
		memset(replyiridium, 0, sizeof(replyiridium));
		sprintf(replyiridium, "PMIN:%u\r\n", periodmin);
		sizedata = strlen(replyiridium);
		osMessageQueuePut(sizedatairidiumQueueHandle, &sizedata, 0U, 0U);
		osMessageQueuePut(iridiumsendQueueHandle, replyiridium, 0U, 0U);
	}
	if (strstr(buffer, "DCAL=")) {
		driftcalc = parsingdatairidium(buffer, "DCAL=");
		write_eeprom_byte(&hi2c1, 0x4E, &driftcalc);
	}
	if (strstr(buffer, "DCAL?")) {
		//iridium_printf("AT+SBDWT=DCAL:%u\r\n", driftcalc);
		//IRIDIUM.status = SBDWT;
		memset(replyiridium, 0, sizeof(replyiridium));
		sprintf(replyiridium, "DCAL: %u\r\n", driftcalc);
		sizedata = strlen(replyiridium);
		osMessageQueuePut(sizedatairidiumQueueHandle, &sizedata, 0U, 0U);
		osMessageQueuePut(iridiumsendQueueHandle, replyiridium, 0U, 0U);
	}
	/*if (strstr(buffer, "TIMEBT=")) {
	 char *bufx;
	 char *bufxx;
	 strx = strstr(buffer, "TIMEBT=");
	 strtok_r(strx, "=", &strx);
	 bufx = strtok_r(strx, "=", &strx);
	 bufxx = strx;
	 hourbat = atoi(bufx);
	 minbat = atoi(bufxx);
	 write_eeprom_byte(&hi2c1, 0x4A, &hourbat);
	 write_eeprom_byte(&hi2c1, 0x49, &minbat);
	 memset(savedsdcardiridium, 0, sizeof(savedsdcardiridium));
	 sprintf(savedsdcardiridium,
	 "4,2,0,[ Write EEPROM ] SET TIME BATTERY = %d:%d \n", hourbat,
	 minbat);
	 osMessageQueuePut(sdcardQueueHandle, savedsdcardiridium, 0U, 0U);
	 }
	 if (strstr(buffer, "TIMEBT?")) {
	 iridium_printf("AT+SBDWT=TIMEBT:%u,%u\r\n", hourbat, minbat);
	 IRIDIUM.status = SBDWT;
	 memset(savedsdcardiridium, 0, sizeof(savedsdcardiridium));
	 sprintf(savedsdcardiridium,
	 "4,1,[ Send IRIDIUM ] REQUEST TIME BATTERY \n");
	 osMessageQueuePut(sdcardQueueHandle, savedsdcardiridium, 0U, 0U);
	 }*/
	if (strstr(buffer, "TIMELC=")) {
		char *bufx;
		char *bufxx;
		strx = strstr(buffer, "TIMELC=");
		strtok_r(strx, "=", &strx);
		bufx = strtok_r(strx, "=", &strx);
		bufxx = strx;
		hourloc = atoi(bufx);
		minloc = atoi(bufxx);
		write_eeprom_byte(&hi2c1, 0x4C, &hourloc);
		write_eeprom_byte(&hi2c1, 0x4B, &minloc);
	}
	if (strstr(buffer, "TIMELC?")) {
		//iridium_printf("AT+SBDWT=TIMELC:%u,%u\r\n", hourloc, minloc);
		//IRIDIUM.status = SBDWT;
		memset(replyiridium, 0, sizeof(replyiridium));
		sprintf(replyiridium, "TIMELC: %u,%u\r\n", hourloc, minloc);
		sizedata = strlen(replyiridium);
		osMessageQueuePut(sizedatairidiumQueueHandle, &sizedata, 0U, 0U);
		osMessageQueuePut(iridiumsendQueueHandle, replyiridium, 0U, 0U);
	}

	if (strstr(buffer, "CWBPR=")) {
		counterwaitbpr = parsingdatairidium(buffer, "CWBPR=");
		write_eeprom_byte(&hi2c1, 0x4F, &counterwaitbpr);
	}
	if (strstr(buffer, "CWBPR?")) {
		//iridium_printf("AT+SBDWT=CWBPR:%u\r\n", counterwaitbpr);
		//IRIDIUM.status = SBDWT;
		memset(replyiridium, 0, sizeof(replyiridium));
		sprintf(replyiridium, "CWBPR: %u\r\n", counterwaitbpr);
		sizedata = strlen(replyiridium);
		osMessageQueuePut(sizedatairidiumQueueHandle, &sizedata, 0U, 0U);
		osMessageQueuePut(iridiumsendQueueHandle, replyiridium, 0U, 0U);
	}
	if (strstr(buffer, "SYNCGPS=")) {
		syncgps = parsingdatairidium(buffer, "SYNCGPS=");
		write_eeprom_byte(&hi2c1, 0x3B, &syncgps);
	}
	if (strstr(buffer, "SYNCGPS?")) {
		//iridium_printf("AT+SBDWT=SYNCGPS:%u\r\n", syncgps);
		//IRIDIUM.status = SBDWT;
		memset(replyiridium, 0, sizeof(replyiridium));
		sprintf(replyiridium, "SYNCGPS: %u\r\n", syncgps);
		sizedata = strlen(replyiridium);
		osMessageQueuePut(sizedatairidiumQueueHandle, &sizedata, 0U, 0U);
		osMessageQueuePut(iridiumsendQueueHandle, replyiridium, 0U, 0U);
	}
}

uint32_t parsingdatairidium(char *buffer, char *keyword) {
	char *data;
	char *str = strstr((char*) buffer, keyword);
	strtok_r(str, "=", &str);
	//data = strtok_r(str, "\r", &str);
	data = str;
	return atoi(data);
}

void checksumiridium(char *buffer, char *data) {
	uint16_t checksum;
	uint8_t i;

	for (i = 0; i < strlen(data); i++) {
		checksum += data[i];
		buffer[i] = data[i];
	}

	buffer[i] = checksum / 256;
	buffer[i + 1] = checksum % 256;
}

/* untuk tes hello data
 char data[7];
 data[0] = 0x68;
 data[1] = 0x65;
 data[2] = 0x6c;
 data[3] = 0x6c;
 data[4] = 0x6f;
 data[5] = 0x02;
 data[6] = 0x14;
 */
