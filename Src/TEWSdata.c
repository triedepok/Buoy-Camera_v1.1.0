/*
 * TEWSdata.c
 *
 *  Created on: Mar 25, 2019
 *      Author: Hafiz
 */

#include "TEWSdata.h"
#include "parameterTEWS.h"

DATAENGINEERING_t DATAENGINEERING;
RTC_TimeTypeDef RTCMicro;

uint8_t crc_try = 0;
extern uint8_t versionfirmware;
char savedsdcarddata[512];

DataNormal_t DataNormal;
DataTsunami_t DataTsunami;
DataEngOBU_t DataEngOBU;
DataOBU_t DataOBU;
DataEngBuoy_t DataEngBuoy;
DataLoc_t DataLoc;

/*
 * payloadToBytesx = convert bit vektor to byte array
 *
 * bitVector	: variable bit vector
 * numBits		: size of bit vector
 * byteArray	x: variable byte array
 */
void payloadToBytesx(uint8_t *bitVector, uint16_t numBits, uint8_t *byteArray) {
	for (uint16_t i = 0; i < numBits; i += 8) {
		uint8_t byte = 0;
		for (uint8_t b = 0; b < 8; ++b) {
			byte |= (bitVector[i + b] << (7 - b));
		}
		byteArray[i / 8] = byte;
	}
}

/*
 * addBits = arrange value to bit vector
 *
 * bitVector	: variable bit vector
 * size			: size of bit vector
 * value		: value of variable you want to arrange to bit
 * numBits		: capacity of bits
 */
void addBits(uint8_t *bitVector, uint16_t *size, uint32_t value,
		uint8_t numBits) {
	//ASSERT(numBits > 0  && numBits <= 32);
	uint16_t pos = *size;
	for (uint8_t bit = 0; bit < numBits; ++bit, value >>= 1) {
		bitVector[pos + numBits - bit - 1] = value & 1;
	}

	*size += numBits;
}

void checksum(uint8_t *byteArray) {
	uint16_t sumByte = 0;
	uint16_t devide = 256;

	for (uint8_t i = 0; i < 14; i++) {
		sumByte += byteArray[i];
	}

	byteArray[12] = sumByte / devide;
	byteArray[13] = sumByte % devide;
}

/*
 * Encode process for Message 1 Ina-TEWS : Water Lever Height
 *
 */
void EncodeDataNormal(char *dataout, uint16_t *size, DataNormal_t *DataNormal) {
	uint8_t payload[200];
	*size = 0;
	uint32_t value = 0;
	uint16_t crc = 0xFFFF;
	char buffersixbit[200];

	value = DataNormal->modekirim;
	addBits(payload, size, value, 4);   	// modekirim
	value = DataNormal->idobu;
	addBits(payload, size, value, 8);   		// idobu
	value = DataNormal->statusobu;
	addBits(payload, size, value, 8);  		// statusobu
	value = DataNormal->counter;
	addBits(payload, size, value, 10);   		// counter
	value = DataNormal->bulan;
	addBits(payload, size, value, 4);  			//
	value = DataNormal->tanggal;
	addBits(payload, size, value, 5);  		//
	value = DataNormal->timestamp;
	addBits(payload, size, value, 17);  	//
	value = DataNormal->data1;
	addBits(payload, size, value, 24);  		//
	value = DataNormal->data2;
	addBits(payload, size, value, 24);  		//
	value = DataNormal->data3;
	addBits(payload, size, value, 24);  		//
	value = DataNormal->data4;
	addBits(payload, size, value, 24);  		//
	value = 0;
	addBits(payload, size, value, 4);  		//

	memset(buffersixbit, 0, sizeof(buffersixbit));
	bitarray_to_sixbit(buffersixbit, payload, *size);

	memset(&crc, 0, sizeof(crc));
	crc = ccrc16((unsigned char*) buffersixbit, strlen(buffersixbit));
	sprintf((char*) dataout, "#,%s,%04X", buffersixbit, crc);
}

void EncodeDataTsunami(char *dataout, uint16_t *size,
		DataTsunami_t *DataTsunami) {
	uint8_t payload[200];
	*size = 0;
	uint32_t value = 0;
	uint16_t crc_ok = 0xFFFF;
	char buffersixbit[200];

	value = DataTsunami->modekirim;
	addBits(payload, size, value, 4);   	// modekirim
	value = DataTsunami->idobu;
	addBits(payload, size, value, 8);   		// idobu
	value = DataTsunami->statusobu;
	addBits(payload, size, value, 8);  		// statusobu
	value = DataTsunami->counter;
	addBits(payload, size, value, 10);   		// counter
	value = DataTsunami->bulan;
	addBits(payload, size, value, 4);  			//
	value = DataTsunami->tanggal;
	addBits(payload, size, value, 5);  		//
	value = DataTsunami->timestamp;
	addBits(payload, size, value, 17);  	//
	value = DataTsunami->data1;
	addBits(payload, size, value, 24);  		//
	value = DataTsunami->data2;
	addBits(payload, size, value, 24);  		//
	value = DataTsunami->data3;
	addBits(payload, size, value, 24);  		//
	value = DataTsunami->data4;
	addBits(payload, size, value, 24);  		//
	value = DataTsunami->predict;
	addBits(payload, size, value, 24);  		//
	value = 0;
	addBits(payload, size, value, 4);

	memset(buffersixbit, 0, sizeof(buffersixbit));
	bitarray_to_sixbit(buffersixbit, payload, *size);

	memset(&crc_ok, 0, sizeof(crc_ok));
	crc_ok = ccrc16((unsigned char*) buffersixbit, strlen(buffersixbit));
	sprintf((char*) dataout, "#,%s,%04X", buffersixbit, crc_ok);
}

void EncodeDataEngOBU(char *dataout, uint16_t *size, DataEngOBU_t *DataEngOBU) {
	uint8_t payload[200];
	*size = 0;
	uint32_t value = 0;
	uint16_t crc_ok = 0xFFFF;
	char buffersixbit[200];

	value = DataEngOBU->modekirim;
	addBits(payload, size, value, 4);   	// modekirim
	value = DataEngOBU->idobu;
	addBits(payload, size, value, 8);   		// idobu
	value = DataEngOBU->statusobu;
	addBits(payload, size, value, 8);  		// statusobu
	value = DataEngOBU->bulan;
	addBits(payload, size, value, 4);  			//
	value = DataEngOBU->tanggal;
	addBits(payload, size, value, 5);  		//
	value = DataEngOBU->timestamp;
	addBits(payload, size, value, 17);  	//
	value = DataEngOBU->tempBPR;
	addBits(payload, size, value, 12);  		//
	value = DataEngOBU->batACOU;
	addBits(payload, size, value, 12);  		//
	value = DataEngOBU->batOBU;
	addBits(payload, size, value, 12);  		//
	value = DataEngOBU->tempOBU;
	addBits(payload, size, value, 12);  		//
	value = DataEngOBU->humOBU;
	addBits(payload, size, value, 10);  		//
	value = DataEngOBU->tilt;
	addBits(payload, size, value, 12);  			//
	value = 0;
	addBits(payload, size, value, 4);  				//

	memset(buffersixbit, 0, sizeof(buffersixbit));
	bitarray_to_sixbit(buffersixbit, payload, *size);

	memset(&crc_ok, 0, sizeof(crc_ok));
	crc_ok = ccrc16((unsigned char*) buffersixbit, strlen(buffersixbit));
	sprintf((char*) dataout, "#,%s,%04X", buffersixbit, crc_ok);

}

void EncodeDataEngBuoy(uint8_t *dataout, uint16_t *size,DataEngBuoy_t *DataEngBuoy) {
	uint8_t payload[200];
	uint16_t sizebuf = 0;
	*size = 0;
	uint32_t value = 0;

	value = DataEngBuoy->modekirim;
	addBits(payload, size, value, 4);   	// modekirim
	value = DataEngBuoy->idbuoy;
	addBits(payload, size, value, 8);   		// idobu
	value = DataEngBuoy->statusbuoy;
	addBits(payload, size, value, 8);  	// statusobu
	value = DataEngBuoy->bulan;
	addBits(payload, size, value, 4);  			//
	value = DataEngBuoy->tanggal;
	addBits(payload, size, value, 5);  		//
	value = DataEngBuoy->timestamp;
	addBits(payload, size, value, 17);  	//
	value = DataEngBuoy->SNR;
	addBits(payload, size, value, 10);  			//
	value = DataEngBuoy->MPD;
	addBits(payload, size, value, 10);  			//
	value = DataEngBuoy->AGC;
	addBits(payload, size, value, 10);  			//
	value = DataEngBuoy->batBuoy;
	addBits(payload, size, value, 12);  		//
	value = DataEngBuoy->tempBuoy;
	addBits(payload, size, value, 12);  		//
	value = DataEngBuoy->humBuoy;
	addBits(payload, size, value, 12);  		//

	payloadToBytesx(payload, *size, (uint8_t*) dataout);
	sizebuf = *size;
	sizebuf = sizebuf / 8;
	*size = sizebuf;
}

void EncodeTEWSLoc(uint8_t *dataout, uint16_t *size, DataLoc_t *DataLoc) {
	uint8_t payload[200];
	*size = 0;
	uint16_t sizebuf = 0;
	uint32_t value = 0;

	value = DataLoc->modekirim;
	addBits(payload, size, value, 4);   	// modekirim
	value = DataLoc->idbuoy;
	addBits(payload, size, value, 8);   		// idobu
	value = DataLoc->statusbuoy;
	addBits(payload, size, value, 8);  	// statusobu
	value = DataLoc->bulan;
	addBits(payload, size, value, 4);  			//
	value = DataLoc->tanggal;
	addBits(payload, size, value, 5);  		//
	value = DataLoc->timestamp;
	addBits(payload, size, value, 17);  	//
	if (DataLoc->lat > 0)
		addBits(payload, size, 1, 1);  // Latitude
	else
		addBits(payload, size, 0, 1);  // Latitude
	uint32_t lat32 = (uint32_t) abs(DataLoc->lat * 1000000);
	addBits(payload, size, lat32, 27);  	//

	if (DataLoc->lng > 0)
		addBits(payload, size, 1, 1);  		// LNG
	else
		addBits(payload, size, 0, 1);  		// LNG
	uint32_t lng32 = (uint32_t) abs(DataLoc->lng * 1000000);
	addBits(payload, size, lng32, 28);  	//

	value = DataLoc->altitude;
	addBits(payload, size, value, 12);  	//
	value = DataLoc->flagpengiriman;
	addBits(payload, size, value, 5); 		//
	value = DataLoc->satelit;
	addBits(payload, size, value, 5);  		//
	value = DataLoc->hdop;
	addBits(payload, size, value, 8);  		//
	value = 0;
	addBits(payload, size, value, 3);

	payloadToBytesx(payload, *size, (uint8_t*) dataout);
	sizebuf = *size;
	sizebuf = sizebuf / 8;
	*size = sizebuf;
}

void DecodeDataNormal(char *datain, DataNormal_t *DataNormal, uint8_t *dataout,
		uint16_t *size) {
	*size = 0;
	uint8_t buffer[200];
	memset(buffer, 0, sizeof(buffer));
	sixbit_to_ascii(dataout, buffer, datain, size);

	uint8_t i;
	for (i = 0; i <= 3; i++)
		DataNormal->modekirim |= (buffer[i] << (3 - i));
	for (i = 4; i <= 11; i++)
		DataNormal->idobu |= (buffer[i] << (11 - i));
	for (i = 12; i <= 19; i++)
		DataNormal->statusobu |= (buffer[i] << (19 - i));
	for (i = 20; i <= 29; i++)
		DataNormal->counter |= (buffer[i] << (29 - i));
	for (i = 30; i <= 33; i++)
		DataNormal->bulan |= (buffer[i] << (33 - i));
	for (i = 34; i <= 38; i++)
		DataNormal->tanggal |= (buffer[i] << (38 - i));
	for (i = 39; i <= 55; i++)
		DataNormal->timestamp |= (buffer[i] << (55 - i));
	for (i = 56; i <= 79; i++)
		DataNormal->data1 |= (buffer[i] << (79 - i));
	for (i = 80; i <= 103; i++)
		DataNormal->data2 |= (buffer[i] << (103 - i));
	for (i = 104; i <= 127; i++)
		DataNormal->data3 |= (buffer[i] << (127 - i));
	for (i = 128; i <= 151; i++)
		DataNormal->data4 |= (buffer[i] << (151 - i));
}

void DecodeDataTsunami(char *datain, DataTsunami_t *DataTsunami,
		uint8_t *dataout, uint16_t *size) {
	*size = 0;
	uint8_t buffer[200];
	memset(buffer, 0, sizeof(buffer));
	sixbit_to_ascii(dataout, buffer, datain, size);

	uint8_t i;
	for (i = 0; i <= 3; i++)
		DataTsunami->modekirim |= (buffer[i] << (3 - i));
	for (i = 4; i <= 11; i++)
		DataTsunami->idobu |= (buffer[i] << (11 - i));
	for (i = 12; i <= 19; i++)
		DataTsunami->statusobu |= (buffer[i] << (19 - i));
	for (i = 20; i <= 29; i++)
		DataTsunami->counter |= (buffer[i] << (29 - i));
	for (i = 30; i <= 33; i++)
		DataTsunami->bulan |= (buffer[i] << (33 - i));
	for (i = 34; i <= 38; i++)
		DataTsunami->tanggal |= (buffer[i] << (38 - i));
	for (i = 39; i <= 55; i++)
		DataTsunami->timestamp |= (buffer[i] << (55 - i));
	for (i = 56; i <= 79; i++)
		DataTsunami->data1 |= (buffer[i] << (79 - i));
	for (i = 80; i <= 103; i++)
		DataTsunami->data2 |= (buffer[i] << (103 - i));
	for (i = 104; i <= 127; i++)
		DataTsunami->data3 |= (buffer[i] << (127 - i));
	for (i = 128; i <= 151; i++)
		DataTsunami->data4 |= (buffer[i] << (151 - i));
	for (i = 152; i <= 175; i++)
		DataTsunami->predict |= (buffer[i] << (175 - i));
}

void DecodeDataEngOBU(char *datain, DataEngOBU_t *DataEngOBU, uint8_t *dataout,
		uint16_t *size) {
	uint8_t buffer[200];
	*size = 0;
	memset(buffer, 0, sizeof(buffer));
	sixbit_to_ascii(dataout, buffer, datain, size);

	uint8_t i;
	for (i = 0; i <= 3; i++)
		DataEngOBU->modekirim |= (buffer[i] << (3 - i));
	for (i = 4; i <= 11; i++)
		DataEngOBU->idobu |= (buffer[i] << (11 - i));
	for (i = 12; i <= 19; i++)
		DataEngOBU->statusobu |= (buffer[i] << (19 - i));
	for (i = 20; i <= 23; i++)
		DataEngOBU->bulan |= (buffer[i] << (23 - i));
	for (i = 24; i <= 28; i++)
		DataEngOBU->tanggal |= (buffer[i] << (28 - i));
	for (i = 29; i <= 45; i++)
		DataEngOBU->timestamp |= (buffer[i] << (45 - i));
	for (i = 46; i <= 57; i++)
		DataEngOBU->tempBPR |= (buffer[i] << (57 - i));
	for (i = 58; i <= 69; i++)
		DataEngOBU->batACOU |= (buffer[i] << (69 - i));
	for (i = 70; i <= 81; i++)
		DataEngOBU->batOBU |= (buffer[i] << (81 - i));
	for (i = 82; i <= 93; i++)
		DataEngOBU->tempOBU |= (buffer[i] << (93 - i));
	for (i = 94; i <= 103; i++)
		DataEngOBU->humOBU |= (buffer[i] << (103 - i));
	for (i = 104; i <= 115; i++)
		DataEngOBU->tilt |= (buffer[i] << (115 - i));

}

void DecodeDataOBU(char *datain, DataOBU_t *DataOBU, uint8_t *dataout,
		uint16_t *size) {
	*size = 0;
	uint8_t buffer[200];
	memset(buffer, 0, sizeof(buffer));
	sixbit_to_ascii(dataout, buffer, datain, size);

	uint8_t i;
	for (i = 0; i <= 3; i++)
		DataOBU->modekirim |= (buffer[i] << (3 - i));
	for (i = 4; i <= 11; i++)
		DataOBU->idobu |= (buffer[i] << (11 - i));
}

void bitarray_to_sixbit(char *dataout, uint8_t *datain, uint16_t size) {
	for (uint8_t i = 0; i < size; i += 6) {
		uint8_t num = 0;
		for (uint8_t j = 0; j < 6; ++j) {
			num |= (datain[i + j] << (5 - j));
		}
		if (num < 40)
			num += 48;
		else
			num += 56;

		dataout[i / 6] = (char) num;
	}
}

void sixbit_to_ascii(uint8_t *dataout, uint8_t *dataarray, char *datain,
		uint16_t *size) {

	*size = 0;
	//uint16_t sizebyte = strlen(datain) / 8;
	for (uint8_t i = 0; i < strlen(datain); i++) {
		uint8_t num = 0;
		if (datain[i] < 88)
			num = (uint8_t) datain[i] - 48;
		else
			num = (uint8_t) datain[i] - 56;

		for (uint8_t j = 0; j < 6; j++) {
			dataarray[(i * 6) + j] = (num >> (5 - j)) & 1;
		}
	}
	uint8_t sizebit = (strlen(datain) * 6) / 8;
	sizebit = sizebit * 8;
	*size = sizebit / 8;
	payloadToBytesx(dataarray, sizebit, dataout);
}

void EncodeTEWSLocBuoy(uint8_t flagkirim, uint8_t modekirim) {
	char tampung[512];

	uint8_t dataforiridium[200];
	uint16_t sizedatairidium;

	DS3231_GetTime(&rtcTEWS);
	uint32_t timestamp = (rtcTEWS.Hour * 60 * 60) + (rtcTEWS.Min * 60)
			+ (rtcTEWS.Sec);

	DataLoc.modekirim = modekirim;
	DataLoc.idbuoy = idBUOY;
	DataLoc.statusbuoy = statusbuoy;
	DataLoc.bulan = rtcTEWS.Month;
	DataLoc.tanggal = rtcTEWS.Date;
	DataLoc.timestamp = timestamp;

	DataLoc.lat = GPS.latitude;
	DataLoc.lng = GPS.longitude;
	DataLoc.altitude = GPS.altitude;
	DataLoc.flagpengiriman = flagkirim;
	DataLoc.satelit = GPS.numsat;
	DataLoc.hdop = GPS.hdop;

	//usb_universal_printf("------ DATA POSISI BUOY ------\r\n");
	memset(tampung, 0, sizeof(tampung));
	sprintf(tampung, "%d,%d,%d,%d,%d,%lu,%f,%f,%d,%d,%d,%d", DataLoc.modekirim,
			DataLoc.idbuoy, DataLoc.statusbuoy, DataLoc.bulan, DataLoc.tanggal,
			DataLoc.timestamp, DataLoc.lat, DataLoc.lng, DataLoc.altitude,
			DataLoc.flagpengiriman, DataLoc.satelit, DataLoc.hdop);
	usb_universal_printf("%s\r\n", tampung);

	EncodeTEWSLoc(dataforiridium, &DataLoc.size, &DataLoc);

	dataforiridium[DataLoc.size] = versionfirmware;
	dataforiridium[DataLoc.size + 1] = DataLoc.size + 2;
	sizedatairidium = DataLoc.size + 2;
	osMessageQueuePut(sizedatairidiumQueueHandle, &sizedatairidium, 0U, 0U);
	osMessageQueuePut(iridiumsendQueueHandle, dataforiridium, 0U, 0U);
}

void EncodeTEWSEngBuoy(uint8_t modekirim) {
	char tampung[512];
	uint8_t dataforiridium[200];
	uint16_t sizedatairidium;

	DS3231_GetTime(&rtcTEWS);

	uint32_t timestamp = (rtcTEWS.Hour * 60 * 60) + (rtcTEWS.Min * 60)
			+ (rtcTEWS.Sec);

	//data encode
	DataEngBuoy.modekirim = modekirim;  //6 = 1 hari, 7 request
	DataEngBuoy.idbuoy = idBUOY;
	DataEngBuoy.statusbuoy = statusbuoy;
	DataEngBuoy.bulan = rtcTEWS.Month;
	DataEngBuoy.tanggal = rtcTEWS.Date;
	DataEngBuoy.timestamp = timestamp;
	DataEngBuoy.humBuoy = 0;

	//usb_universal_printf("------ DATA ENGINEERING BUOY ------\r\n");
	memset(tampung, 0, sizeof(tampung));
	sprintf(tampung, "%d,%d,%d,%d,%d,%ld,%d,%d,%d,%d,%d,%d",
			DataEngBuoy.modekirim, DataEngBuoy.idbuoy, DataEngBuoy.statusbuoy,
			DataEngBuoy.bulan, DataEngBuoy.tanggal, DataEngBuoy.timestamp,
			DataEngBuoy.SNR, DataEngBuoy.MPD, DataEngBuoy.AGC,
			DataEngBuoy.batBuoy, DataEngBuoy.tempBuoy, DataEngBuoy.humBuoy);
	usb_universal_printf("%s\r\n", tampung);

	memset(dataforiridium, 0, sizeof(dataforiridium));
	EncodeDataEngBuoy(dataforiridium, &DataEngBuoy.size, &DataEngBuoy);

	// Kirim data Engineering iridium ketika CRC benar
	dataforiridium[DataEngBuoy.size] = versionfirmware;
	dataforiridium[DataEngBuoy.size + 1] = DataEngBuoy.size + 2;
	sizedatairidium = DataEngBuoy.size + 2;
	osMessageQueuePut(sizedatairidiumQueueHandle, &sizedatairidium, 0U, 0U);
	osMessageQueuePut(iridiumsendQueueHandle, dataforiridium, 0U, 0U);
}

void UniversalDataDecoder(char *data, char *crc) {
	char tampung[512];
	char dataforiridium[200];
	uint16_t sizedatairidium;

	DataOBU.modekirim = 0;
	DecodeDataOBU(data, &DataOBU, (uint8_t*) dataforiridium, &sizedatairidium);

	memset(tampung, 0, sizeof(tampung));
	sprintf(tampung, "MODE KIRIM = %d \r\n", DataOBU.modekirim);
	//usb_printf(tampung);

	////////////////////////////////////////////////////////////////////////////////////
	// Data TSUNAMI
	// mode 0 tsunami ; mode 3 tsunami req
	////////////////////////////////////////////////////////////////////////////////////
	if ((DataOBU.modekirim == 0) || (DataOBU.modekirim == 3)) {

		DataTsunami.modekirim = 0;
		DataTsunami.idobu = 0;
		DataTsunami.statusobu = 0;
		DataTsunami.counter = 0;
		DataTsunami.bulan = 0;
		DataTsunami.tanggal = 0;
		DataTsunami.timestamp = 0;
		DataTsunami.data1 = 0;
		DataTsunami.data2 = 0;
		DataTsunami.data3 = 0;
		DataTsunami.data4 = 0;
		DataTsunami.predict = 0;

		//printf ("---------- convert_6bit_to_bytes ------------\n");

		memset(dataforiridium, 0, sizeof(dataforiridium));
		DecodeDataTsunami(data, &DataTsunami, (uint8_t*) dataforiridium,
				&sizedatairidium);

		//usb_universal_printf("------ HASIL DECODE DATA TSUNAMI ------\r\n");
		memset(tampung, 0, sizeof(tampung));
		sprintf(tampung, "%u,%u,%u,%u,%u,%u,%lu,%lu,%lu,%lu,%lu,%lu",
				DataTsunami.modekirim, DataTsunami.idobu, DataTsunami.statusobu,
				DataTsunami.counter, DataTsunami.bulan, DataTsunami.tanggal,
				DataTsunami.timestamp, DataTsunami.data1, DataTsunami.data2,
				DataTsunami.data3, DataTsunami.data4, DataTsunami.predict);
		usb_universal_printf("%s\r\n", tampung);

		UNIX.year = rtcTEWS.Year;
		UNIX.month = DataTsunami.bulan;
		UNIX.date = DataTsunami.tanggal;
		UNIX.hours = DataTsunami.timestamp / 3600;
		UNIX.minutes = (DataTsunami.timestamp - (UNIX.hours * 3600)) / 60;
		UNIX.seconds = DataTsunami.timestamp - (UNIX.hours * 3600)
				- (UNIX.minutes * 60);
		UNIX.unix = TM_RTC_GetUnixTimeStamp(&UNIX)
				- ((DataTsunami.counter - 1) * 60);
		usb_universal_printf("Unix time = %lu\r\n", UNIX.unix);

		if (cekCRC((unsigned char*) data, (unsigned char*) crc) != 1) {
			if (DataTsunami.modekirim == 0) {

			} else if (DataTsunami.modekirim == 3) {

			}
		} else {
			if (DataTsunami.modekirim == 0) {

			} else if (DataTsunami.modekirim == 3) {

			}
			// Kirim data Tsunami iridium ketika CRC benar
			dataforiridium[sizedatairidium] = statusbuoy;
			dataforiridium[sizedatairidium + 1] = versionfirmware;
			dataforiridium[sizedatairidium + 2] = sizedatairidium + 3;
			sizedatairidium = sizedatairidium + 3;
			osMessageQueuePut(sizedatairidiumQueueHandle, &sizedatairidium, 0U,
					0U);
			osMessageQueuePut(iridiumsendQueueHandle, dataforiridium, 0U, 0U);

		}
		if (timebacknormalstatus == DataTsunami.counter) {
			operationmode = 0;
			write_eeprom_byte(&hi2c1, 0x04, &operationmode);
		}
	}
	////////////////////////////////////////////////////////////////////////////////////
	// Data NORMAL
	// mode 1 normal ; mode 2 normal request
	////////////////////////////////////////////////////////////////////////////////////
	else if ((DataOBU.modekirim == 1) || (DataOBU.modekirim == 2)) {
		statuswaitresponsebpr = ALREADYGETDATABPR;

		DataNormal.modekirim = 0;
		DataNormal.idobu = 0;
		DataNormal.statusobu = 0;
		DataNormal.counter = 0;
		DataNormal.bulan = 0;
		DataNormal.tanggal = 0;
		DataNormal.timestamp = 0;
		DataNormal.data1 = 0;
		DataNormal.data2 = 0;
		DataNormal.data3 = 0;
		DataNormal.data4 = 0;

		//-------------------------- DECODE NORMAL
		memset(dataforiridium, 0, sizeof(dataforiridium));
		DecodeDataNormal(data, &DataNormal, (uint8_t*) dataforiridium,
				&sizedatairidium);

		//usb_universal_printf("------ HASIL DECODE DATA NORMAL ------\r\n");
		memset(tampung, 0, sizeof(tampung));
		sprintf(tampung, "%u,%u,%u,%u,%u,%u,%lu,%lu,%lu,%lu,%lu",
				DataNormal.modekirim, DataNormal.idobu, DataNormal.statusobu,
				DataNormal.counter, DataNormal.bulan, DataNormal.tanggal,
				DataNormal.timestamp, DataNormal.data1, DataNormal.data2,
				DataNormal.data3, DataNormal.data4);
		usb_universal_printf("%s\r\n", tampung);

		if (cekCRC((unsigned char*) data, (unsigned char*) crc) != 1) {
			if (DataNormal.modekirim == 1) {

			} else if (DataNormal.modekirim == 2) {

			}

			/*HAL_UART_Transmit(&_USB_USART,
			 (uint8_t*) "Data CRC tidak sama.\n",
			 sizeof("Data CRC tidak sama.\n"), 10000);*/
			usb_universal_printf("crc_try = %d\r\n", crc_try);

			if (crc_try < 5) {
				crc_try++;
				//osDelay(200);  //percobaan delay rsend
			} else {
				usb_universal_printf("Kirim notifikasi ke Iridium, 5 kali terima CRC Error.\r\n");
				crc_try = 0;
				EncodeTEWSLocBuoy(3, 12);
			}

		} else {
			/*HAL_UART_Transmit(&_USB_USART,
			 (uint8_t*) "Data CRC sama.\n",
			 sizeof("Data CRC sama.\n"), 10000);*/

			if (DataNormal.modekirim == 1) {
				//osMessageQueuePut(iridiumsendQueueHandle, retval_bytes, 0U, 0U);
			} else if (DataNormal.modekirim == 2) {

			}

			// Kirim data Normal iridium ketika CRC benar 16
			dataforiridium[sizedatairidium] = crc_try;
			dataforiridium[sizedatairidium + 1] = statusbuoy;
			dataforiridium[sizedatairidium + 2] = versionfirmware;
			dataforiridium[sizedatairidium + 3] = sizedatairidium + 4;
			sizedatairidium = sizedatairidium + 4;

			osMessageQueuePut(sizedatairidiumQueueHandle, &sizedatairidium, 0U,
					0U);
			osMessageQueuePut(iridiumsendQueueHandle, dataforiridium, 0U, 0U);

			crc_try = 0;
		}
	}
	////////////////////////////////////////////////////////////////////////////////////
	// Data ENGINEERING
	// mode 4 tanpa request ; mode 5 dengan request
	////////////////////////////////////////////////////////////////////////////////////

	else if ((DataOBU.modekirim == 4) || (DataOBU.modekirim == 5)) {

		DataEngOBU.modekirim = 0;
		DataEngOBU.idobu = 0;
		DataEngOBU.statusobu = 0;
		DataEngOBU.bulan = 0;
		DataEngOBU.tanggal = 0;
		DataEngOBU.timestamp = 0;
		DataEngOBU.tempBPR = 0;
		DataEngOBU.batACOU = 0;
		DataEngOBU.batOBU = 0;
		DataEngOBU.tempOBU = 0;
		DataEngOBU.humOBU = 0;
		DataEngOBU.tilt = 0;

		memset(dataforiridium, 0, sizeof(dataforiridium));
		DecodeDataEngOBU(data, &DataEngOBU, (uint8_t*) dataforiridium,
				&sizedatairidium);

		//usb_universal_printf("------ HASIL DECODE DATA ENGINEERING OBU ------\r\n");
		memset(tampung, 0, sizeof(tampung));
		sprintf(tampung, "%u,%u,%u,%u,%u,%lu,%u,%u,%u,%u,%u,%u",
				DataEngOBU.modekirim, DataEngOBU.idobu, DataEngOBU.statusobu,
				DataEngOBU.bulan, DataEngOBU.tanggal, DataEngOBU.timestamp,
				DataEngOBU.tempBPR, DataEngOBU.batACOU, DataEngOBU.batOBU,
				DataEngOBU.tempOBU, DataEngOBU.humOBU, DataEngOBU.tilt);
		usb_universal_printf("%s\r\n", tampung);

		if (cekCRC((unsigned char*) data, (unsigned char*) crc) != 1) {
			if (DataEngOBU.modekirim == 4) {

			} else if (DataEngOBU.modekirim == 5) {

			}

			/*HAL_UART_Transmit(&_USB_USART,
			 (uint8_t*) "Data CRC tidak sama.\n",
			 sizeof("Data CRC tidak sama.\n"), 10000);
			 usb_printf("crc_try = %d\r\n", crc_try);

			 if (crc_try < 5) {
			 memset(savedsdcarddata, 0, sizeof(savedsdcarddata));
			 sprintf(savedsdcarddata, "7,%d,0,%s\n", crc_try, data);
			 osMessageQueuePut(sdcardQueueHandle, savedsdcarddata, 0U, 0U);
			 crc_try++;
			 HAL_Delay(200);  //percobaan delay rsend
			 AKUSTIK_sendESend();
			 } else {
			 usb_universal_printf(
			 "Kirim notifikasi ke Iridium, 5 kali terima CRC Error.\r\n");
			 memset(savedsdcarddata, 0, sizeof(savedsdcarddata));
			 sprintf(savedsdcarddata,
			 "4,0,0,[ Send Iridium ] 5 kali CRC Error.\n");
			 osMessageQueuePut(sdcardQueueHandle, savedsdcarddata, 0U, 0U);
			 memset(savedsdcarddata, 0, sizeof(savedsdcarddata));
			 sprintf(savedsdcarddata, "7,%d,0,%s\n", crc_try, data);
			 osMessageQueuePut(sdcardQueueHandle, savedsdcarddata, 0U, 0U);
			 crc_try = 0;
			 }*/
		} else {
			/*HAL_UART_Transmit(&_USB_USART,
			 (uint8_t*) "Data CRC sama.\n",
			 sizeof("Data CRC sama.\n"), 10000);*/

			if (DataEngOBU.modekirim == 4) {

			} else if (DataEngOBU.modekirim == 5) {

			}

			// Kirim data Engineering iridium ketika CRC benar 15
			dataforiridium[sizedatairidium] = versionfirmware;
			dataforiridium[sizedatairidium + 1] = sizedatairidium + 2;
			sizedatairidium = sizedatairidium + 2;
			osMessageQueuePut(sizedatairidiumQueueHandle, &sizedatairidium, 0U,
					0U);
			osMessageQueuePut(iridiumsendQueueHandle, dataforiridium, 0U, 0U);
		}
	}

	////////////////////////////////////////////////////////////////////////////////////
	// Data ENGINEERING BUOY
	// mode 6 tanpa request ; mode 7 dengan request
	////////////////////////////////////////////////////////////////////////////////////

	/*else if ((DECODE.mode_kirim == 6) || (DECODE.mode_kirim == 7)) {
	 DECODE.mode_kirim = 0;
	 DECODE.id_lokasi = 0;
	 DECODE.status_buoy = 0;
	 DECODE.bln = 0;
	 DECODE.tgl = 0;
	 DECODE.timeofday = 0;
	 DECODE.snr = 0;
	 DECODE.mdp = 0;
	 DECODE.agc = 0;
	 DECODE.battBuoy = 0;
	 DECODE.tempBuoy = 0;
	 DECODE.kelembabanBuoy = 0;

	 decode_dataEng_Buoy((unsigned char*) buffer, &DECODE.mode_kirim,
	 &DECODE.id_lokasi, &DECODE.status_buoy, &DECODE.bln,
	 &DECODE.tgl, &DECODE.timeofday, &DECODE.snr, &DECODE.mdp,
	 &DECODE.agc, &DECODE.battBuoy, &DECODE.tempBuoy,
	 &DECODE.kelembabanBuoy);

	 usb_universal_printf(
	 "------ HASIL DECODE DATA ENGINEERING BUOY ------\r\n");
	 memset(tampung, 0, sizeof(tampung));
	 sprintf(tampung, "%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d",
	 DECODE.mode_kirim, DECODE.id_lokasi, DECODE.status_buoy,
	 DECODE.bln, DECODE.tgl, DECODE.timeofday, DECODE.snr,
	 DECODE.mdp, DECODE.agc, DECODE.battBuoy, DECODE.tempBuoy,
	 DECODE.kelembabanBuoy);
	 usb_universal_printf("%s\r\n", tampung);

	 if (cekCRC((unsigned char*) data, (unsigned char*) crc) != 1) {
	 if (DECODE.mode_kirim == 6) {
	 memset(savedsdcarddata, 0, sizeof(savedsdcarddata));
	 //2020-09-04 10:03:25 [ Recv OBU ] Data Engineering
	 sprintf(savedsdcarddata,
	 "4,0,0,[ Recv OBU ] Data Engineering BUOY.\n");
	 osMessageQueuePut(sdcardQueueHandle, savedsdcarddata, 0U, 0U);
	 } else if (DECODE.mode_kirim == 7) {
	 memset(savedsdcarddata, 0, sizeof(savedsdcarddata));
	 //2020-09-04 10:03:25 [ Recv OBU ] Data Engineering
	 sprintf(savedsdcarddata,
	 "4,0,0,[ Recv OBU ] Data Engineering BUOY Request.\n");
	 osMessageQueuePut(sdcardQueueHandle, savedsdcarddata, 0U, 0U);
	 }
	 HAL_UART_Transmit(&_USB_USART,
	 (uint8_t*) "Data CRC tidak sama.\n",
	 sizeof("Data CRC tidak sama.\n"), 10000);
	 usb_printf("crc_try = %d\r\n", crc_try);

	 if (crc_try < 5) {
	 memset(savedsdcarddata, 0, sizeof(savedsdcarddata));
	 sprintf(savedsdcarddata, "7,%d,0,%s\n", crc_try, data);
	 osMessageQueuePut(sdcardQueueHandle, savedsdcarddata, 0U, 0U);
	 crc_try++;
	 HAL_Delay(200);  //percobaan delay rsend
	 AKUSTIK_sendESend();
	 } else {
	 usb_universal_printf(
	 "Kirim notifikasi ke Iridium, 5 kali terima CRC Error.\r\n");
	 memset(savedsdcarddata, 0, sizeof(savedsdcarddata));
	 sprintf(savedsdcarddata,
	 "4,0,0,[ Send Iridium ] 5 kali CRC Error.\n");
	 osMessageQueuePut(sdcardQueueHandle, savedsdcarddata, 0U, 0U);
	 memset(savedsdcarddata, 0, sizeof(savedsdcarddata));
	 sprintf(savedsdcarddata, "7,%d,0,%s\n", crc_try, data);
	 osMessageQueuePut(sdcardQueueHandle, savedsdcarddata, 0U, 0U);
	 crc_try = 0;
	 }
	 } else {
	 HAL_UART_Transmit(&_USB_USART,
	 (uint8_t*) "Data CRC sama.\n",
	 sizeof("Data CRC sama.\n"), 10000);

	 if (DECODE.mode_kirim == 6) {
	 memset(savedsdcarddata, 0, sizeof(savedsdcarddata));
	 //2020-09-04 10:03:25 [ Recv OBU ] Data Engineering
	 sprintf(savedsdcarddata, "4,0,0,[ Recv OBU ] Data Engineering BUOY.\n");
	 osMessageQueuePut(sdcardQueueHandle, savedsdcarddata, 0U, 0U);

	 memset(savedsdcarddata, 0, sizeof(savedsdcarddata));
	 sprintf(savedsdcarddata, "3,3,0,%s\n", tampung);
	 osMessageQueuePut(sdcardQueueHandle, savedsdcarddata, 0U, 0U);
	 } else if (DECODE.mode_kirim == 7) {
	 memset(savedsdcarddata, 0, sizeof(savedsdcarddata));
	 //2020-09-04 10:03:25 [ Recv OBU ] Data Engineering
	 sprintf(savedsdcarddata,
	 "4,0,0,[ Recv OBU ] Data Engineering BUOY Request.\n");
	 osMessageQueuePut(sdcardQueueHandle, savedsdcarddata, 0U, 0U);

	 memset(savedsdcarddata, 0, sizeof(savedsdcarddata));
	 sprintf(savedsdcarddata, "11,2,0,%s\n", tampung);
	 osMessageQueuePut(sdcardQueueHandle, savedsdcarddata, 0U, 0U);
	 }

	 // Kirim data Engineering iridium ketika CRC benar
	 dataforiridium[sizebyte] = versionfirmware;
	 dataforiridium[sizebyte + 1] = sizebyte + 2;
	 sizedatairidium = sizebyte + 2;
	 osMessageQueuePut(sizedatairidiumQueueHandle, &sizedatairidium, 0U, 0U);
	 osMessageQueuePut(iridiumsendQueueHandle, dataforiridium, 0U, 0U);
	 }

	 }
	 else if (DECODE.mode_kirim == 8) {
	 DECODE.mode_kirim = 0;
	 DECODE.id_obu = 0;
	 DECODE.val_TH = 0;

	 decode_dataSetting1((unsigned char*) buffer, &DECODE.mode_kirim,
	 &DECODE.id_obu, &DECODE.val_TH);
	 usb_universal_printf(
	 "---------- HASIL DECODE THRESHOLD ------------\r\n");

	 memset(tampung, 0, sizeof(tampung));
	 sprintf(tampung, "%d,%d,%d", DECODE.mode_kirim, DECODE.id_obu,
	 DECODE.val_TH);
	 usb_universal_printf("%s\r\n", tampung);

	 memset(savedsdcarddata, 0, sizeof(savedsdcarddata));
	 //2020-09-04 10:03:25 [ Recv OBU ] Data Engineering
	 sprintf(savedsdcarddata, "4,0,0,[ Recv OBU ] Data Threshold.\n");
	 osMessageQueuePut(sdcardQueueHandle, savedsdcarddata, 0U, 0U);

	 memset(savedsdcarddata, 0, sizeof(savedsdcarddata));
	 sprintf(savedsdcarddata, "3,1,0,%s\n", tampung);
	 osMessageQueuePut(sdcardQueueHandle, savedsdcarddata, 0U, 0U);

	 // Kirim data Threshold iridium ketika CRC benar
	 dataforiridium[sizebyte] = versionfirmware;
	 dataforiridium[sizebyte + 1] = sizebyte + 2;
	 sizedatairidium = sizebyte + 2;
	 osMessageQueuePut(sizedatairidiumQueueHandle, &sizedatairidium, 0U, 0U);
	 osMessageQueuePut(iridiumsendQueueHandle, dataforiridium, 0U, 0U);

	 } else if (DECODE.mode_kirim == 9) {
	 DECODE.mode_kirim = 0;
	 DECODE.id_obu = 0;
	 DECODE.val_TBN = 0;

	 decode_dataSetting1((unsigned char*) buffer, &DECODE.mode_kirim,
	 &DECODE.id_obu, &DECODE.val_TBN);
	 usb_universal_printf(
	 "---------- HASIL DECODE TIME BACK TO NORMAL ------------\r\n");
	 memset(tampung, 0, sizeof(tampung));
	 sprintf(tampung, "%d,%d,%d", DECODE.mode_kirim, DECODE.id_obu,
	 DECODE.val_TBN);
	 usb_universal_printf("%s\r\n", tampung);

	 memset(savedsdcarddata, 0, sizeof(savedsdcarddata));
	 //2020-09-04 10:03:25 [ Recv OBU ] Data Engineering
	 sprintf(savedsdcarddata,
	 "4,0,0,[ Recv OBU ] Data Turn Back to Normal.\n");
	 osMessageQueuePut(sdcardQueueHandle, savedsdcarddata, 0U, 0U);

	 memset(savedsdcarddata, 0, sizeof(savedsdcarddata));
	 sprintf(savedsdcarddata, "3,1,0,%s\n", tampung);
	 osMessageQueuePut(sdcardQueueHandle, savedsdcarddata, 0U, 0U);

	 // Kirim data Time Back to Normal iridium ketika CRC benar
	 dataforiridium[sizebyte] = versionfirmware;
	 dataforiridium[sizebyte + 1] = sizebyte + 2;
	 sizedatairidium = sizebyte + 2;
	 osMessageQueuePut(sizedatairidiumQueueHandle, &sizedatairidium, 0U, 0U);
	 osMessageQueuePut(iridiumsendQueueHandle, dataforiridium, 0U, 0U);

	 } else if (DECODE.mode_kirim == 10) {
	 DECODE.mode_kirim = 0;
	 DECODE.id_obu = 0;
	 DECODE.val_POWM = 0;

	 decode_dataSetting2((unsigned char*) buffer, &DECODE.mode_kirim,
	 &DECODE.id_obu, &DECODE.val_POWM);
	 usb_universal_printf(
	 "---------- HASIL DECODE POWER ACOUSTIC MODEM ------------\r\n");
	 memset(tampung, 0, sizeof(tampung));
	 sprintf(tampung, "%d,%d,%d", DECODE.mode_kirim, DECODE.id_obu,
	 DECODE.val_POWM);
	 usb_universal_printf("%s\r\n", tampung);

	 memset(savedsdcarddata, 0, sizeof(savedsdcarddata));
	 //2020-09-04 10:03:25 [ Recv OBU ] Data Engineering
	 sprintf(savedsdcarddata,
	 "4,0,0,[ Recv OBU ] Data Power Acoustic Modem.\n");
	 osMessageQueuePut(sdcardQueueHandle, savedsdcarddata, 0U, 0U);

	 memset(savedsdcarddata, 0, sizeof(savedsdcarddata));
	 sprintf(savedsdcarddata, "3,1,0,%s\n", tampung);
	 osMessageQueuePut(sdcardQueueHandle, savedsdcarddata, 0U, 0U);

	 // Kirim data Power Acoustic Modem iridium ketika CRC benar
	 dataforiridium[sizebyte] = versionfirmware;
	 dataforiridium[sizebyte + 1] = sizebyte + 2;
	 sizedatairidium = sizebyte + 2;
	 osMessageQueuePut(sizedatairidiumQueueHandle, &sizedatairidium, 0U, 0U);
	 osMessageQueuePut(iridiumsendQueueHandle, dataforiridium, 0U, 0U);

	 }else if (DECODE.mode_kirim == 11) {

	 } */

}
