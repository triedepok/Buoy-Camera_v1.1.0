/*
 * TEWSeeprom.c
 *
 *  Created on: Mar 24, 2019
 *      Author: Hafiz
 */

#include "TEWSeeprom.h"
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

extern float latitude;
extern float longitude;
extern uint8_t tandalat;
extern int latx;
extern uint8_t tandalng;
extern int lngx;
extern uint32_t distancedrifting;
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

extern uint8_t synctimertc;
extern uint8_t minbat;
extern uint8_t hourbat;
extern uint8_t minloc;
extern uint8_t hourloc;
extern uint8_t periodmin;
extern uint8_t driftcalc;
extern uint8_t counterwaitbpr;
extern uint8_t leakstatus;

extern uint8_t AISstatus;

void write_eeprom_byte(I2C_HandleTypeDef *hi2c, unsigned int position,
		uint8_t *data) {   //Menulis Per Byte Character
	HAL_I2C_Mem_Write(hi2c, (uint16_t) EEPROM_ADDRESS_for_write << 1, position,
			1, data, 1, 5);
}

void write_eeprom_page(I2C_HandleTypeDef *hi2c, unsigned int position,
		uint8_t *data, uint8_t length) { //Menulis Per Page
	for (int i = 0; i < length; i++) {
		HAL_I2C_Mem_Write(hi2c, (uint16_t) EEPROM_ADDRESS_for_write << 1,
				position, 1, (uint8_t*) &data[i], 1, 5);
		position++;
	}
}

unsigned int read_eeprom_byte(I2C_HandleTypeDef *hi2c, unsigned int position,
		uint8_t *data) {
	HAL_I2C_Mem_Read(hi2c, (uint16_t) EEPROM_ADDRESS_for_write << 1, position,
			1, data, 1, 5); //read memory address 08
	return *data;
}

unsigned int read_eeprom_page(I2C_HandleTypeDef *hi2c, unsigned int position,
		uint8_t *data, uint8_t length) {

	int j;
	uint8_t data1[length];
	for (j = 0; j < length; j++) {
		HAL_I2C_Mem_Read(hi2c, (uint16_t) EEPROM_ADDRESS_for_write << 1,
				position, 1, (uint8_t*) &data1[j], 1, 5); //read memory address 08
		position++;
		data[j] = data1[j];
	}
	return *data;
}

void check_eeprom(UART_HandleTypeDef *uart, I2C_HandleTypeDef *hi2c,
		uint16_t DevAddress, uint32_t Trials, uint32_t Timeout) {
	HAL_StatusTypeDef result;
	result = HAL_I2C_IsDeviceReady(hi2c, DevAddress, Trials, Timeout);
	if (result != HAL_OK) // HAL_ERROR or HAL_BUSY or HAL_TIMEOUT
			{
		HAL_UART_Transmit(&_UNI_USART, (uint8_t*) &"Failed Eeprom \r\n", 15,
				10);
	}
	if (result == HAL_OK) {
		HAL_UART_Transmit(&_UNI_USART, (uint8_t*) "Found Eeprom \r\n", 25,
				1000);
	}
}

uint32_t read_eeprom_union(I2C_HandleTypeDef *hi2c, unsigned int position) {
	char four;
	char three;
	char two;
	char one;
	HAL_I2C_Mem_Read(hi2c, (uint16_t) EEPROM_ADDRESS_for_write << 1, position,
			1, (uint8_t*) &four, 1, 100);
	HAL_Delay(5);
	HAL_I2C_Mem_Read(hi2c, (uint16_t) EEPROM_ADDRESS_for_write << 1,
			position + 1, 1, (uint8_t*) &three, 1, 100);
	HAL_Delay(5);
	HAL_I2C_Mem_Read(hi2c, (uint16_t) EEPROM_ADDRESS_for_write << 1,
			position + 2, 1, (uint8_t*) &two, 1, 100);
	HAL_Delay(5);
	HAL_I2C_Mem_Read(hi2c, (uint16_t) EEPROM_ADDRESS_for_write << 1,
			position + 3, 1, (uint8_t*) &one, 1, 100);
	HAL_Delay(5);
	return ((four << 0) & 0xFF) + ((three << 8) & 0xFFFF)
			+ ((two << 16) & 0xFFFFFF) + ((one << 24) & 0xFFFFFFFF);
}

void write_eeprom_union(I2C_HandleTypeDef *hi2c, unsigned int position,
		uint32_t data) { //Menulis Per Page
	char four = (data & 0xFF);
	char three = ((data >> 8) & 0xFF);
	char two = ((data >> 16) & 0xFF);
	char one = ((data >> 24) & 0xFF);

	HAL_I2C_Mem_Write(hi2c, (uint16_t) EEPROM_ADDRESS_for_write << 1, position,
			1, (uint8_t*) &four, 1, 100);
	HAL_Delay(5);
	HAL_I2C_Mem_Write(hi2c, (uint16_t) EEPROM_ADDRESS_for_write << 1,
			position + 1, 1, (uint8_t*) &three, 1, 100);
	HAL_Delay(5);
	HAL_I2C_Mem_Write(hi2c, (uint16_t) EEPROM_ADDRESS_for_write << 1,
			position + 2, 1, (uint8_t*) &two, 1, 100);
	HAL_Delay(5);
	HAL_I2C_Mem_Write(hi2c, (uint16_t) EEPROM_ADDRESS_for_write << 1,
			position + 3, 1, (uint8_t*) &one, 1, 100);
	HAL_Delay(5);
}

void InitEEPROM(void) {
	//read_eeprom_byte(&hi2c2, 0x00, &mode); //mode CPU :
	mode = BUOY;
	//operationmode = 0;
	//read_eeprom_byte(&hi2c1, 0x01, &idOBU); //ID OBU
	read_eeprom_byte(&hi2c1, 0x02, &idBUOY); //ID BUOY
	//read_eeprom_byte(&hi2c1, 0x03, &idCBT); //ID CBT
	read_eeprom_byte(&hi2c1, 0x04, &operationmode);
	read_eeprom_byte(&hi2c1, 0x05, &debug);
	read_eeprom_byte(&hi2c1, 0x06, &debugbt);
	read_eeprom_byte(&hi2c1, 0x07, &sendmodeiridium);
	read_eeprom_byte(&hi2c1, 0x08, &countertsunami);
	read_eeprom_byte(&hi2c1, 0x09, &calculationstatus);
	threshold = read_eeprom_union(&hi2c1, 0x0A);
	periodsampling = read_eeprom_union(&hi2c1, 0x10);
	periodsenddata = read_eeprom_union(&hi2c1, 0x14);
	periodsendloc = read_eeprom_union(&hi2c1, 0x18);
	periodsendbat = read_eeprom_union(&hi2c1, 0x1C);
	timebacknormal = read_eeprom_union(&hi2c1, 0x20);

	distancedrifting = read_eeprom_union(&hi2c1, 0x24);
	read_eeprom_byte(&hi2c1, 0x25, &tandalat);
	latx = read_eeprom_union(&hi2c1, 0x26);
	read_eeprom_byte(&hi2c1, 0x30, &tandalng);
	lngx = read_eeprom_union(&hi2c1, 0x31);
	latitude = (float) latx / 1000000.0;
	latitude = (-1.0) * latitude;
	longitude = (float) lngx / 1000000.0;

	read_eeprom_byte(&hi2c1, 0x35, &ledstatus);
	read_eeprom_byte(&hi2c1, 0x36, &debugbpr);
	read_eeprom_byte(&hi2c1, 0x37, &debugakustik);
	read_eeprom_byte(&hi2c1, 0x38, &debugiridium);
	read_eeprom_byte(&hi2c1, 0x39, &debuggps);
	read_eeprom_byte(&hi2c1, 0x3A, &debugbluetooth);
	read_eeprom_byte(&hi2c1, 0x3B, &syncgps);
	read_eeprom_byte(&hi2c1, 0x3C, &AccelerometerStatus);
	read_eeprom_byte(&hi2c1, 0x3D, &kalibrasibat);
	read_eeprom_byte(&hi2c1, 0x3E, &debuguni);

	accth = read_eeprom_union(&hi2c1, 0x40);
	acccounter = read_eeprom_union(&hi2c1, 0x44);

	read_eeprom_byte(&hi2c1, 0x48, &synctimertc);
	read_eeprom_byte(&hi2c1, 0x49, &minbat);
	read_eeprom_byte(&hi2c1, 0x4A, &hourbat);
	read_eeprom_byte(&hi2c1, 0x4B, &minloc);
	read_eeprom_byte(&hi2c1, 0x4C, &hourloc);
	read_eeprom_byte(&hi2c1, 0x4D, &periodmin);
	read_eeprom_byte(&hi2c1, 0x4E, &driftcalc);
	read_eeprom_byte(&hi2c1, 0x4F, &counterwaitbpr);
	read_eeprom_byte(&hi2c1, 0x50, &AISstatus);

}
