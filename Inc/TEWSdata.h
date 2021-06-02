/*
 * TEWSdata.h
 *
 *  Created on: Mar 25, 2019
 *      Author: Hafiz
 */

#ifndef TEWSDATA_H_
#define TEWSDATA_H_

#include "stm32l4xx_hal.h"

typedef struct {
	int tilt;
	uint16_t batteryakustik;
	uint32_t temperatureboard;
	uint8_t leakstatus;
	uint16_t voltage;
	int SNR;
	int MPD;
	int AGC;
} DATAENGINEERING_t;

extern DATAENGINEERING_t DATAENGINEERING;


typedef struct
{
	uint8_t modekirim;
	uint8_t idobu;
	uint8_t statusobu;
	uint16_t counter;
	uint8_t bulan;
	uint8_t tanggal;
	uint32_t timestamp;
	uint32_t data1;
	uint32_t data2;
	uint32_t data3;
	uint32_t data4;

	char 		DataEncoded[200];
	uint16_t	size;
}DataNormal_t;

typedef struct
{
	uint8_t modekirim;
	uint8_t idobu;
	uint8_t statusobu;
	uint16_t counter;
	uint8_t bulan;
	uint8_t tanggal;
	uint32_t timestamp;
	uint32_t data1;
	uint32_t data2;
	uint32_t data3;
	uint32_t data4;
	uint32_t predict;

	char 		DataEncoded[200];
	uint16_t	size;
}DataTsunami_t;

typedef struct
{
	uint8_t modekirim;
	uint8_t idobu;
	uint8_t statusobu;
	uint16_t counter;
	uint8_t bulan;
	uint8_t tanggal;
	uint32_t timestamp;
	uint16_t tempBPR;
	uint16_t batACOU;
	uint16_t batOBU;
	uint16_t tempOBU;
	uint16_t humOBU;
	uint16_t tilt;

	uint16_t SNR;
	uint16_t MPD;
	uint16_t AGC;

	char 		DataEncoded[200];
	uint16_t	size;
}DataEngOBU_t;

typedef struct
{
	uint8_t modekirim;
	uint8_t idbuoy;
	uint8_t statusbuoy;
	uint8_t bulan;
	uint8_t tanggal;
	uint32_t timestamp;
	uint16_t SNR;
	uint16_t MPD;
	uint16_t AGC;
	uint16_t batBuoy;
	uint16_t tempBuoy;
	uint16_t humBuoy;
	uint8_t leakstatus;
	uint16_t batACOU;
	uint16_t tilt;

	char 		DataEncoded[200];
	uint16_t	size;
}DataEngBuoy_t;

typedef struct
{
	uint8_t modekirim;
	uint8_t idbuoy;
	uint8_t statusbuoy;
	uint8_t bulan;
	uint8_t tanggal;
	uint32_t timestamp;
	double lat;
	double lng;
	uint16_t altitude;
	uint8_t flagpengiriman;
	uint8_t satelit;
	uint8_t hdop;

	char 		DataEncoded[200];
	uint16_t	size;
}DataLoc_t;

typedef struct
{
	uint8_t modekirim;
	uint8_t idobu;
}DataOBU_t;

extern DataNormal_t DataNormal;
extern DataTsunami_t DataTsunami;
extern DataEngOBU_t DataEngOBU;
extern DataOBU_t DataOBU;
extern DataEngBuoy_t DataEngBuoy;
extern DataLoc_t DataLoc;

void payloadToBytesx(uint8_t *bitVector, uint16_t numBits, uint8_t *byteArray);
void addBits(uint8_t *bitVector, uint16_t *size, uint32_t value, uint8_t numBits);
void checksum(uint8_t *byteArray);
void EncodeDataNormal(char *dataout, uint16_t* size, DataNormal_t* DataNormal);
void EncodeDataTsunami(char *dataout, uint16_t* size, DataTsunami_t* DataTsunami);
void EncodeDataEngOBU(char *dataout, uint16_t* size, DataEngOBU_t* DataEngOBU);
void EncodeDataEngBuoy(uint8_t *dataout, uint16_t* size, DataEngBuoy_t* DataEngBuoy);
void EncodeTEWSLoc(uint8_t *dataout, uint16_t* size, DataLoc_t* DataLoc);

void DecodeDataNormal(char* datain, DataNormal_t* DataNormal, uint8_t* dataout, uint16_t *size);
void DecodeDataTsunami(char* datain, DataTsunami_t* DataTsunami, uint8_t* dataout, uint16_t *size);
void DecodeDataEngOBU(char* datain, DataEngOBU_t* DataEngOBU, uint8_t* dataout, uint16_t *size);
void DecodeDataOBU(char* datain, DataOBU_t* DataOBU, uint8_t* dataout, uint16_t *size);

void bitarray_to_sixbit(char* dataout, uint8_t* datain, uint16_t size);
void sixbit_to_ascii(uint8_t *dataout, uint8_t *dataarray, char *datain, uint16_t* size);

void EncodeTEWSLocBuoy(uint8_t flagkirim, uint8_t modekirim);
void EncodeTEWSEngBuoy(uint8_t modekirim);
void UniversalDataDecoder(char *data, char *crc);

#endif /* TEWSDATA_H_ */
