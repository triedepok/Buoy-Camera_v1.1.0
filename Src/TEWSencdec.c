/*

 gcc encode_decode_modul.c -o test

 */

#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <math.h>
#include <string.h>
#include "parameterTEWS.h"

#define NORMAL_MODE_BUOY union un_data_normal_buoy
#define NORMAL_MODE union normalMode
#define DATA_PRESSURE_AKTUAL union dataPressureAktual
//---
#define TSUNAMI_MODE union tsunamiMode
#define DATA_PREDICT union predictData

#define DATA_ENGINEERING_1 union un_dataEngineering_1
#define DATA_ENGINEERING_2 union un_dataEngineering_2
#define DATA_ENGINEERING_BUOY union un_dataEngineering_buoy

#define ENG_BUOY_1 union un_eng_buoy_1
#define ENG_BUOY_2 union un_eng_buoy_2

#define DATA_SETTING1 union un_dataSetting1
#define DATA_SETTING2 union un_dataSetting2

#define DATA_TH union un_data_th
#define BYTE_TH 4

#define DATA_TEST union un_test

#define DATA_LAT union un_data_lat
#define DATA_POSITION1 union un_data_position1
#define DATA_POSITION2 union un_data_position2
#define DATA_POSITION3 union un_data_position3

int encode_normal(unsigned char *data_out, unsigned int mode_kirim,
		unsigned int id_obu, unsigned int status_obu, unsigned int counter,
		unsigned int bln, unsigned int tgl, unsigned int timeofday,
		unsigned int data1, unsigned int data2, unsigned int data3,
		unsigned int data4) {

	NORMAL_MODE paramNormalMode;
	DATA_PRESSURE_AKTUAL normalMode_data;
	int i = 0;
	int max_index;
	int total;

	max_index = 7;
	for (i = 0; i < max_index; i++) {
		paramNormalMode.data[i] = 0;
	}

	paramNormalMode.param.mode_kirim = mode_kirim;
	paramNormalMode.param.id_obu = id_obu;
	paramNormalMode.param.status_obu = status_obu;
	paramNormalMode.param.counter = counter;
	paramNormalMode.param.bln = bln;
	paramNormalMode.param.tgl = tgl;
	paramNormalMode.param.timeofday = timeofday;

	for (i = 0; i < max_index; i++) {
//		printf ("%d %d(%02X) \n", max_index-i-1, paramNormalMode.data[max_index-i-1], paramNormalMode.data[max_index-i-1]);

		data_out[i] = paramNormalMode.data[i];
	}
	total = max_index;

	max_index = 7;
	for (i = 0; i < max_index; i++) {
		normalMode_data.data[i] = 0;
	}

	normalMode_data.dataPressure.data1 = data1;
	normalMode_data.dataPressure.data2 = data2;
	normalMode_data.dataPressure.data3 = data3;
	normalMode_data.dataPressure.data4 = data4;
	for (i = 0; i < max_index; i++) {
//		printf ("%d %d(%02X) \n", max_index-i-1, normalMode_data.data[max_index-i-1], normalMode_data.data[max_index-i-1]);

		data_out[i + total] = normalMode_data.data[i];
	}
	total = total + max_index;

	return total;

}

int encode_tsunami(unsigned char *data_out, unsigned int mode_kirim,
		unsigned int id_obu, unsigned int status_obu, unsigned int counter,
		unsigned int bln, unsigned int tgl, unsigned int timeofday,
		unsigned int data1, unsigned int data2, unsigned int data3,
		unsigned int data4, unsigned int predict) {

	TSUNAMI_MODE paramTsunamiMode;
	DATA_PRESSURE_AKTUAL tsunamiMode_data;
	DATA_PREDICT predict_data;
	int i = 0;
	int max_index;
	int total;
	//unsigned char data[16];

	//printf ("\n----------------<encode_tsunami>-----------\n\n");
	//-------------------------------- parameter
	max_index = 7;
	for (i = 0; i < max_index; i++) {
		paramTsunamiMode.data[i] = 0;
	}

	paramTsunamiMode.param.mode_kirim = mode_kirim;
	paramTsunamiMode.param.spare = 0;
	paramTsunamiMode.param.status_obu = status_obu;
	paramTsunamiMode.param.counter = counter;
	paramTsunamiMode.param.id_obu = id_obu;
	paramTsunamiMode.param.bln = bln;
	paramTsunamiMode.param.tgl = tgl;
	paramTsunamiMode.param.timeofday = timeofday;

	for (i = 0; i < max_index; i++) {
//		printf ("%d %d(%02X) \n", max_index-i-1, paramTsunamiMode.data[max_index-i-1], paramTsunamiMode.data[max_index-i-1]);

		data_out[i] = paramTsunamiMode.data[i];
	}
	total = max_index;
//	printf ("------------------------------------------------------\n");

	//----------------------------------- actual pressure
	max_index = 7;
	for (i = 0; i < max_index; i++) {
		tsunamiMode_data.data[i] = 0;
	}

	tsunamiMode_data.dataPressure.data1 = data1;
	tsunamiMode_data.dataPressure.data2 = data2;
	tsunamiMode_data.dataPressure.data3 = data3;
	tsunamiMode_data.dataPressure.data4 = data4;
	for (i = 0; i < max_index; i++) {
//		printf ("%d %d(%02X) \n", max_index-i-1, tsunamiMode_data.data[max_index-i-1], tsunamiMode_data.data[max_index-i-1]);

		data_out[i + total] = tsunamiMode_data.data[i];
	}
	total = total + max_index;
//	printf ("------------------------------------------------------\n");

	//-------------------------------------------- predict pressure
	max_index = 2;
	for (i = 0; i < max_index; i++) {
		predict_data.data[i] = 0;
	}

	predict_data.item.predict = predict;
	predict_data.item.spare = 0;

	for (i = 0; i < max_index; i++) {
//		printf ("%d %d(%02X) \n", max_index-i-1, predict_data.data[max_index-i-1], predict_data.data[max_index-i-1]);

		data_out[i + total] = predict_data.data[i];
	}

	total = total + max_index;
	return total;
}

int encode_dataEng_OBU(unsigned char *result, unsigned int mode_kirim,
		unsigned int id_obu, unsigned int status_obu, unsigned int bln,
		unsigned int tgl, unsigned int timeofday, unsigned int tempBPR,
		unsigned int battOBU, unsigned int battModem, unsigned int tempOBU,
		unsigned int kelembabanOBU, unsigned int tilt) {

	DATA_ENGINEERING_1 dataEngineering1;
	DATA_ENGINEERING_2 dataEngineering2;

	int max_index, i;
	int total = 0;

	max_index = 7;
	for (i = 0; i < max_index; i++) {
		dataEngineering1.data[i] = 0;
	}

	dataEngineering1.dataEngineering.mode_kirim = mode_kirim;
	dataEngineering1.dataEngineering.id_obu = id_obu;
	dataEngineering1.dataEngineering.status_obu = status_obu;
	dataEngineering1.dataEngineering.bln = bln;
	dataEngineering1.dataEngineering.tgl = tgl;
	dataEngineering1.dataEngineering.timeofday = timeofday;
	dataEngineering1.dataEngineering.tempBPR = tempBPR;

	for (i = 0; i < max_index; i++) {
		result[total + i] = dataEngineering1.data[i];
	}

	total = max_index;
	max_index = 7;
	for (i = 0; i < max_index; i++) {
		dataEngineering2.data[i] = 0;
	}

	dataEngineering2.dataEngineering.battOBU = battOBU;
	dataEngineering2.dataEngineering.battModem = battModem;
	dataEngineering2.dataEngineering.tempOBU = tempOBU;
	dataEngineering2.dataEngineering.kelembabanOBU = kelembabanOBU;
	dataEngineering2.dataEngineering.tilt = tilt;

	for (i = 0; i < max_index; i++) {
		result[total + i] = dataEngineering2.data[i];
	}
	total = total + max_index;

	return total;
}

int encode_dataEng_Buoy(unsigned char *data_enc_out, unsigned int mode_kirim,
		unsigned int id_lokasi, unsigned int status_buoy, unsigned int bln,
		unsigned int tgl, unsigned int timeofday, unsigned int snr,
		unsigned int mdp, unsigned int agc, unsigned int battBuoy,
		unsigned int tempBuoy, unsigned int kelembabanBuoy) {

	ENG_BUOY_1 data_eng_buoy1;
	ENG_BUOY_2 data_eng_buoy2;

	//unsigned char data[20];
	int max_index, i;
	int total = 0;

	max_index = 8;
	for (i = 0; i < max_index; i++) {
		data_eng_buoy1.data[i] = 0;
	}

	data_eng_buoy1.eng_buoy1.mode_kirim = mode_kirim;
	data_eng_buoy1.eng_buoy1.id_lokasi = id_lokasi;
	data_eng_buoy1.eng_buoy1.status_buoy = status_buoy;
	data_eng_buoy1.eng_buoy1.bln = bln;
	data_eng_buoy1.eng_buoy1.tgl = tgl;
	data_eng_buoy1.eng_buoy1.timeofday = timeofday;
	data_eng_buoy1.eng_buoy1.snr = snr;
	data_eng_buoy1.eng_buoy1.mdp = mdp;

	for (i = 0; i < max_index; i++) {
		data_enc_out[i] = data_eng_buoy1.data[i];
	}
//	printf ("-----------------------------------------------------------\n");



	total = max_index;
	max_index = 5;

	for (i = 0; i < max_index; i++) {
		data_eng_buoy2.data[i] = 0;
	}

	data_eng_buoy2.eng_buoy2.agc = agc;
	data_eng_buoy2.eng_buoy2.voltase_batt = battBuoy;
	data_eng_buoy2.eng_buoy2.temperatur = tempBuoy;
	data_eng_buoy2.eng_buoy2.kelembaban = kelembabanBuoy;

	for (i = 0; i < max_index; i++) {
		data_enc_out[total+i] = data_eng_buoy2.data[i];
	}
	//printf ("==========================\n\n");

	total = total + max_index;

	return total;
}

int encode_dataSetting1(unsigned char *enc_data, unsigned int mode_kirim,
		unsigned int id_obu, unsigned int val) {
	DATA_SETTING1 data_setting;

	int max_index, i;
	max_index = 4;

	for (i = 0; i < max_index; i++) {
		data_setting.data[i] = 0;
	}

	data_setting.setting.mode_kirim = mode_kirim;
	data_setting.setting.id_obu = id_obu;
	data_setting.setting.value = val;

	for (i = 0; i < max_index; i++) {
		enc_data[i] = data_setting.data[i];
	}

	return max_index;

}

int encode_dataSetting2(unsigned char *enc_data, unsigned int mode_kirim,
		unsigned int id_obu, unsigned int val) {
	DATA_SETTING2 data_setting;

	int max_index, i;
	max_index = 2;

	for (i = 0; i < max_index; i++) {
		data_setting.data[i] = 0;
	}

	data_setting.setting.mode_kirim = mode_kirim;
	data_setting.setting.id_obu = id_obu;
	data_setting.setting.value = val;

	for (i = 0; i < max_index; i++) {
		enc_data[i] = data_setting.data[i];
	}
	return max_index;

}


int encode_dataNormal_Buoy(unsigned char *enc_data, unsigned int nb_try,
		unsigned int status_buoy, unsigned int snr, unsigned int mdp) {
	NORMAL_MODE_BUOY data_normalBuoy;

	int max_index, i;
	max_index = 3;

	for (i = 0; i < max_index; i++) {
		data_normalBuoy.data[i] = 0;
	}

	/*printf("\n------- encode data normal buoy ---------- \n");
	 printf("nb_try (3-bit): %d\n", nb_try);
	 printf("status_buoy (8-bit): %d\n", status_buoy);
	 printf("snr (5-bit): %d\n", snr);
	 printf("mdp (5-bit): %d\n", mdp);
	 printf("\n");*/

	data_normalBuoy.dataBuoy.nb_try = nb_try;
	data_normalBuoy.dataBuoy.status_buoy = status_buoy;
	data_normalBuoy.dataBuoy.snr = snr;
	data_normalBuoy.dataBuoy.mdp = mdp;

	for (i = 0; i < max_index; i++) {
		enc_data[i] = data_normalBuoy.data[i];
	}
//-----------------------------------------------------------------
	for (i = max_index - 1; i >= 0; i--) {
		char *pointer;
		pointer = byte_to_binary(enc_data[i]);
		printf("%s ", pointer);
		free(pointer);
	}
	printf("\n");

	return max_index;

}

int encode_dataLat(unsigned char *enc_data, unsigned int *lat1,
		unsigned int *lat2, unsigned int *sign, int val) {
	DATA_LAT data_lat;

	int max_index, i;
	max_index = 4;

	for (i = 0; i < max_index; i++)
		data_lat.data[i] = 0;

//	printf ("\n------< encode_dataLat  SEPARATE >--------- \n");
//	printf ("latitude: %d\n", val);
//	printf ("\n");

	if (val < 0)
		data_lat.lat_full.data_sign = 1;
	else
		data_lat.lat_full.data_sign = 0;

	data_lat.lat_full.data = abs(val);
	*lat1 = data_lat.separate.lat1;
	*lat2 = data_lat.separate.lat2;
	*sign = data_lat.lat_full.data_sign;

//	printf ("lat1: %d\n",  *lat1);
//	printf ("lat2: %d\n",  *lat2);
//	printf ("\n");

	for (i = 0; i < max_index; i++) {
		enc_data[i] = data_lat.data[i];
	}
//-----------------------------------------------------------------
	/*
	 for (i = max_index-1; i >= 0; i--)
	 {
	 char *pointer;
	 pointer = byte_to_binary(enc_data[i]);
	 printf("%s ", pointer);
	 free(pointer);
	 }
	 printf ("\n");
	 */
	return max_index;

}

int encode_position(unsigned char *data_out, unsigned int mode_kirim,
		unsigned int id_obu, unsigned int status_buoy, unsigned int bln,
		unsigned int tgl, unsigned int timeofday, int lat, int lon,
		unsigned int alt, unsigned int flag_kirim, unsigned int flag_jml_sat,
		unsigned int hdop) {

	unsigned char enc_data[20];
	unsigned int lat1, lat2, lat_sign;
	int i;
	int jml_byte;

	lat1 = lat2 = 0;
	jml_byte = encode_dataLat(enc_data, &lat1, &lat2, &lat_sign, lat);
	/*printf("-------------------------------------- latitude part:\n");
	 printf("-----> lat1: %d\n", lat1);
	 printf("-----> lat2: %d\n", lat2);
	 printf("-----> sign: %d\n", lat_sign);*/

	//----------------------------------------------------------------------------
	DATA_POSITION1 position1;
	DATA_POSITION2 position2;
	DATA_POSITION3 position3;

	int max_index = 0;

//---------------------position 1-----------------------------------
	jml_byte = 8;
	max_index += jml_byte;
	for (i = 0; i < max_index; i++) {
		position1.data[i] = 0;
	}
	position1.st_data.mode_kirim = mode_kirim;
	position1.st_data.id_lokasi = id_obu;
	position1.st_data.status_buoy = status_buoy;
	position1.st_data.bln = bln;
	position1.st_data.tgl = tgl;
	position1.st_data.timeofday = timeofday;
	position1.st_data.lat1 = lat1;

	//printf("position1 ---- max_index---: %d\n", max_index);
	for (i = 0; i < max_index; i++) {
		data_out[i] = position1.data[i];
	}

//---------------------position 2-----------------------------------
	position2.st_data2.lat2 = lat2;
	position2.st_data2.lat_sign = lat_sign;

	position2.st_data2.lon = abs(lon);
	if (lon < 0)
		position2.st_data2.lon_sign = 1;
	else
		position2.st_data2.lon_sign = 0;

	position2.st_data2.alt = alt;
	position2.st_data2.flag_kirim = flag_kirim;

	int max_prev = max_index;
	jml_byte = 7;
	max_index += jml_byte;

	//printf("position2 ---- max_index---: %d\n", max_index);
	for (i = max_prev; i < max_index; i++) {
		data_out[i] = position2.data[i - max_prev];
	}

//---------------------position 3--------------------------------	---
	position3.st_data3.jml_sat = flag_jml_sat;
	position3.st_data3.hdop = hdop;

	max_prev = max_index;
	jml_byte = 2;
	max_index += jml_byte;

	//printf("position3 ---- max_index---: %d\n", max_index);
	for (i = max_prev; i < max_index; i++) {
		data_out[i] = position3.data[i - max_prev];
	}

	//printf(
	//"--------------------< encode_position >---------------------------------\n");
//-----------------------------------------------------------------
	/*for (i = max_index - 1; i >= 0; i--) {
	 char *pointer;
	 pointer = byte_to_binary(data_out[i]);
	 printf("%s ", pointer);
	 free(pointer);
	 }
	 printf("\n");*/

	//printf("position return ---- max_index---: %d\n", max_index);
	jml_byte = max_index;
	return jml_byte;

}

void decode_modekirim(unsigned char *data_in, unsigned int *mode_kirim) {
	NORMAL_MODE paramNormalMode;

	int i, max_index = 7;
	for ( i=0; i<max_index; i++) {
		paramNormalMode.data[i] = data_in[i];
	}
	*mode_kirim = paramNormalMode.param.mode_kirim;
}

void decode_normal(unsigned char *data_in, unsigned int *mode_kirim,
		unsigned int *id_obu, unsigned int *status_obu, unsigned int *counter,
		unsigned int *bln, unsigned int *tgl, unsigned int *timeofday,
		unsigned int *data1, unsigned int *data2, unsigned int *data3,
		unsigned int *data4) {
	NORMAL_MODE paramNormalMode;
	DATA_PRESSURE_AKTUAL normalMode_data;
	int i, max_index = 14;
	int prev_index;

	/*
	 printf ("\n--------------------data normal mode --- DECODE %d bytes\n", max_index);
	 for ( i=0; i<max_index; i++) {
	 printf ("%d. %d [%02X] \n", max_index-i-1, data_in[max_index-i-1], data_in[max_index-i-1]);
	 }
	 */

	max_index = 7;
	for (i = 0; i < max_index; i++) {
		paramNormalMode.data[i] = data_in[i];
	}
	/*
	 printf ("\nmode_kirim: %d\n", paramNormalMode.param.mode_kirim);
	 printf ("id_obu: %d\n", paramNormalMode.param.id_obu);
	 printf ("status_obu: %d\n", paramNormalMode.param.status_obu);
	 printf ("counter: %d\n", paramNormalMode.param.counter);
	 printf ("bln: %d\n", paramNormalMode.param.bln);
	 printf ("tgl: %d\n", paramNormalMode.param.tgl);
	 printf ("timeofday: %d\n", paramNormalMode.param.timeofday);
	 */
	prev_index = max_index;
	max_index = 7;
	for (i = 0; i < max_index; i++) {
		normalMode_data.data[i] = data_in[i + prev_index];
	}
	/*
	 printf ("data1: %d\n", normalMode_data.dataPressure.data1);
	 printf ("data2: %d\n", normalMode_data.dataPressure.data2);
	 printf ("data3: %d\n", normalMode_data.dataPressure.data3);
	 printf ("data4: %d\n", normalMode_data.dataPressure.data4);
	 */
	//---------------------------------------------------------
	*mode_kirim = paramNormalMode.param.mode_kirim;
	*id_obu = paramNormalMode.param.id_obu;
	*status_obu = paramNormalMode.param.status_obu;
	*counter = paramNormalMode.param.counter;
	*bln = paramNormalMode.param.bln;
	*tgl = paramNormalMode.param.tgl;
	*timeofday = paramNormalMode.param.timeofday;
	*data1 = normalMode_data.dataPressure.data1;
	*data2 = normalMode_data.dataPressure.data2;
	*data3 = normalMode_data.dataPressure.data3;
	*data4 = normalMode_data.dataPressure.data4;
}

void decode_tsunami(unsigned char *data_in, unsigned int *mode_kirim,
		unsigned int *id_obu, unsigned int *status_obu, unsigned int *counter,
		unsigned int *bln, unsigned int *tgl, unsigned int *timeofday,
		unsigned int *data1, unsigned int *data2, unsigned int *data3,
		unsigned int *data4, unsigned int *predict) {

	TSUNAMI_MODE paramTsunamiMode;
	DATA_PRESSURE_AKTUAL tsunamiMode_data;
	DATA_PREDICT predict_data;

	int i, max_index = 16;
	int prev_index;

	/*
	 printf ("\n--------------------data normal mode --- DECODE %d bytes\n", max_index);
	 for ( i=0; i<max_index; i++) {
	 printf ("%d. %d [%02X] \n", max_index-i-1, data_in[max_index-i-1], data_in[max_index-i-1]);
	 }
	 */

	max_index = 7;
	for (i = 0; i < max_index; i++) {
		paramTsunamiMode.data[i] = data_in[i];
	}
	/*
	 printf ("\nmode_kirim: %d\n", paramTsunamiMode.param.mode_kirim);
	 printf ("id_obu: %d\n", paramTsunamiMode.param.id_obu);
	 printf ("status_obu: %d\n", paramTsunamiMode.param.status_obu);
	 printf ("counter: %d\n", paramTsunamiMode.param.counter);
	 printf ("bln: %d\n", paramTsunamiMode.param.bln);
	 printf ("tgl: %d\n", paramTsunamiMode.param.tgl);
	 printf ("timeofday: %d\n", paramTsunamiMode.param.timeofday);
	 */

	prev_index = max_index;
	max_index = 7;
	for (i = 0; i < max_index; i++) {
		tsunamiMode_data.data[i] = data_in[i + prev_index];
	}
	/*
	 printf ("data1: %d\n", tsunamiMode_data.dataPressure.data1);
	 printf ("data2: %d\n", tsunamiMode_data.dataPressure.data2);
	 printf ("data3: %d\n", tsunamiMode_data.dataPressure.data3);
	 printf ("data4: %d\n", tsunamiMode_data.dataPressure.data4);
	 */

	prev_index = prev_index + max_index;
	max_index = 2;
	for (i = 0; i < max_index; i++) {
		predict_data.data[i] = data_in[i + prev_index];
	}
	/*
	 for ( i=0; i<max_index; i++) {
	 printf ("%d %d(%02X) \n", max_index-i-1, predict_data.data[max_index-i-1], predict_data.data[max_index-i-1]);
	 }
	 printf ("\npredict: %d\n", predict_data.item.predict);
	 printf ("\n");
	 */
//-------------------------------------------------------
	*mode_kirim = paramTsunamiMode.param.mode_kirim;
	*id_obu = paramTsunamiMode.param.id_obu;
	*status_obu = paramTsunamiMode.param.status_obu;
	*counter = paramTsunamiMode.param.counter;
	*bln = paramTsunamiMode.param.bln;
	*tgl = paramTsunamiMode.param.tgl;
	*timeofday = paramTsunamiMode.param.timeofday;
	*data1 = tsunamiMode_data.dataPressure.data1;
	*data2 = tsunamiMode_data.dataPressure.data2;
	*data3 = tsunamiMode_data.dataPressure.data3;
	*data4 = tsunamiMode_data.dataPressure.data4;
	*predict = predict_data.item.predict;
}

void decode_dataEng_OBU(unsigned char *data_in, int len,
		unsigned int *mode_kirim, unsigned int *id_obu,
		unsigned int *status_obu, unsigned int *bln, unsigned int *tgl,
		unsigned int *timeofday, unsigned int *tempBPR, unsigned int *battOBU,
		unsigned int *battModem, unsigned int *tempOBU,
		unsigned int *kelembabanOBU, unsigned int *tilt) {

	DATA_ENGINEERING_1 dataEngineering1;
	DATA_ENGINEERING_2 dataEngineering2;

	int i;
	int max_index = 14;
	int prev_index;

	max_index = 7;
	for (i = 0; i < max_index; i++) {
		dataEngineering1.data[i] = data_in[i];
	}

	/*for (i = 0; i < max_index; i++) {

		byte_to_binary(data_in[i]);
		usb_printf(" ");
	}*/

	*mode_kirim = dataEngineering1.dataEngineering.mode_kirim;
	*id_obu = dataEngineering1.dataEngineering.id_obu;
	*status_obu = dataEngineering1.dataEngineering.status_obu;
	*bln = dataEngineering1.dataEngineering.bln;
	*tgl = dataEngineering1.dataEngineering.tgl;
	*timeofday = dataEngineering1.dataEngineering.timeofday;
	*tempBPR = dataEngineering1.dataEngineering.tempBPR;

	prev_index = max_index;
	max_index = 7;
	for (i = 0; i < max_index; i++) {
		dataEngineering2.data[i] = data_in[i + prev_index];
	}

	/*	for (i = 0; i < max_index; i++) {

		byte_to_binary(data_in[i + prev_index]);
		usb_printf(" ");
	}*/

	*battOBU = dataEngineering2.dataEngineering.battOBU;
	*battModem = dataEngineering2.dataEngineering.battModem;
	*tempOBU = dataEngineering2.dataEngineering.tempOBU;
	*kelembabanOBU = dataEngineering2.dataEngineering.kelembabanOBU;
	*tilt = dataEngineering2.dataEngineering.tilt;

	/*int i;
	 int max_index, max_index_prev, len_data;

	 len_data = 7;
	 max_index = len_data;
	 for (i = 0; i < len_data; i++) {
	 dataEngineering1.data[i] = data_in[i];
	 }

	 for (i = 0; i < max_index; i++) {

	 byte_to_binary(data_in[i]);
	 usb_printf(" ");
	 }

	 *mode_kirim = dataEngineering1.dataEngineering.mode_kirim;
	 *id_obu = dataEngineering1.dataEngineering.id_obu;
	 *status_obu = dataEngineering1.dataEngineering.status_obu;
	 *bln = dataEngineering1.dataEngineering.bln;
	 *tgl = dataEngineering1.dataEngineering.tgl;
	 *timeofday = dataEngineering1.dataEngineering.timeofday;
	 *tempBPR = dataEngineering1.dataEngineering.tempBPR;

	 max_index_prev = max_index;
	 len_data = 7;
	 max_index = max_index + len_data;
	 for (i = 0; i < len_data; i++) {
	 dataEngineering2.data[i] = data_in[max_index_prev + i];
	 }

	 for (i = 0; i < max_index; i++) {

	 byte_to_binary(data_in[max_index_prev + i]);
	 usb_printf(" ");
	 }

	 *battOBU = dataEngineering2.dataEngineering.battOBU;
	 *battModem = dataEngineering2.dataEngineering.battModem;
	 *tempOBU = dataEngineering2.dataEngineering.tempOBU;
	 *kelembabanOBU = dataEngineering2.dataEngineering.kelembabanOBU;
	 *tilt = dataEngineering2.dataEngineering.tilt;*/

}

void decode_dataEng_Buoy(unsigned char *data_in, unsigned int *mode_kirim,
		unsigned int *id_lokasi, unsigned int *status_buoy, unsigned int *bln,
		unsigned int *tgl, unsigned int *timeofday, unsigned int *snr,
		unsigned int *mdp, unsigned int *agc, unsigned int *battBuoy,
		unsigned int *tempBuoy, unsigned int *kelembabanBuoy) {

	ENG_BUOY_1 data_eng_buoy1;
	ENG_BUOY_2 data_eng_buoy2;

	int i;
	int max_index = 13;
	int prev_index;

	max_index = 8;
	for (i = 0; i < max_index; i++) {
		data_eng_buoy1.data[i] = data_in[i];
	}

	/*for (i = 0; i < max_index; i++) {

	 byte_to_binary(data_in[i]);
	 usb_printf(" ");
	 }*/

	*mode_kirim = data_eng_buoy1.eng_buoy1.mode_kirim;
	*id_lokasi = data_eng_buoy1.eng_buoy1.id_lokasi;
	*status_buoy = data_eng_buoy1.eng_buoy1.status_buoy;
	*bln = data_eng_buoy1.eng_buoy1.bln;
	*tgl = data_eng_buoy1.eng_buoy1.tgl;
	*timeofday = data_eng_buoy1.eng_buoy1.timeofday;
	*snr = data_eng_buoy1.eng_buoy1.snr;
	*mdp = data_eng_buoy1.eng_buoy1.mdp;

	prev_index = max_index;
	max_index = 5;
	for (i = 0; i < max_index; i++) {
		data_eng_buoy2.data[i] = data_in[i + prev_index];
	}

	/*for (i = 0; i < max_index; i++) {

	 byte_to_binary(data_in[i + prev_index]);
	 usb_printf(" ");
	 }*/

	*agc = data_eng_buoy2.eng_buoy2.agc;
	*battBuoy = data_eng_buoy2.eng_buoy2.voltase_batt;
	*tempBuoy = data_eng_buoy2.eng_buoy2.temperatur;
	*kelembabanBuoy = data_eng_buoy2.eng_buoy2.kelembaban;
}

void decode_dataSetting1(unsigned char *data, unsigned int *mode_kirim,
		unsigned int *id_obu, unsigned int *val) {
	DATA_SETTING1 data_setting;

	int max_index, i;
	max_index = 4;

	for (i = 0; i < max_index; i++) {
		data_setting.data[i] = data[i];
	}

	*mode_kirim = data_setting.setting.mode_kirim;
	*id_obu = data_setting.setting.id_obu;
	*val = data_setting.setting.value;
}

void decode_dataSetting2(unsigned char *data, unsigned int *mode_kirim,
		unsigned int *id_obu, unsigned int *val) {
	DATA_SETTING2 data_setting;

	int max_index, i;
	max_index = 2;

	for (i = 0; i < max_index; i++) {
		data_setting.data[i] = data[i];
	}

	*mode_kirim = data_setting.setting.mode_kirim;
	*id_obu = data_setting.setting.id_obu;
	*val = data_setting.setting.value;
}

char* byte_to_binary(unsigned char n) {
	/*int c, d, count;
	char *pointer;

	count = 0;
	pointer = (char*) malloc(8 + 1);

	if (pointer == NULL) exit(EXIT_FAILURE);

	for (c = 7; c >= 0; c--) {
		d = n >> c;

		if (d & 1)
			*(pointer + count) = 1 + '0';
		else
			*(pointer + count) = 0 + '0';

		count++;
	}
	*(pointer + count) = '\0';

	return pointer;*/
	int binary[8];

	for (int i = 0; i < 8; i++)
		binary[7 - i] = (n >> i) & 1;
	// print result
	for (int i = 0; i < 8; i++)
		usb_universal_printf("%d", binary[i]);
	return 0;
}

unsigned char convert_6bit_to_ascii(unsigned char input) {
	if (input == '-')
		return '-';

	if (input < 26)
		return (input + 0x41);
	if (input < 52)
		return ((input - 26) + 0x61);
	if (input < 62)
		return ((input - 52) + 0x30);
	if (input == 62)
		return (0x2b);
	if (input == 63)
		return (0x2f);
	else return (0x00);
}

void convert_bytes_to_6bit(unsigned char *result, unsigned char *data_normal,
		int *size, int max_index) {

	int value1, value2, value3, value4;
	//char value1_ch, value2_ch, value3_ch, value4_ch;
	char value_ch;

//	printf ("===============> max_index: %d\n", max_index);

	unsigned char data_16bit[50];

	int idx = 0;
	int i;
	for (i = 0; i < max_index; i += 3) {
		value1 = value2 = value3 = value4 = 0;
		if (i == max_index - 1) {
			// printf (" i == max_index-1 \n");
			value1 = data_normal[i] & 63;
			value_ch = convert_6bit_to_ascii(value1);
			data_16bit[idx] = value_ch;
			idx++;

			value2 = (data_normal[i] >> 6);
			value_ch = convert_6bit_to_ascii(value2);
			data_16bit[idx] = value_ch;
			idx++;

			value3 = '-';
			value4 = '-';
		} else if (i == max_index - 2) {
			// printf (" i == max_index-2 \n");
			value1 = data_normal[i] & 63;
			value_ch = convert_6bit_to_ascii(value1);
			data_16bit[idx] = value_ch;
			idx++;

			value2 = (data_normal[i] >> 6) | ((data_normal[i + 1] & 15) << 2);
			value_ch = convert_6bit_to_ascii(value2);
			data_16bit[idx] = value_ch;
			idx++;

			value3 = (data_normal[i + 1] >> 4);
			value_ch = convert_6bit_to_ascii(value3);
			data_16bit[idx] = value_ch;
			idx++;

			value4 = '-';
		} else {
			value1 = data_normal[i] & 63;
			value_ch = convert_6bit_to_ascii(value1);
			data_16bit[idx] = value_ch;
			idx++;

			value2 = (data_normal[i] >> 6) | ((data_normal[i + 1] & 15) << 2);
			value_ch = convert_6bit_to_ascii(value2);
			data_16bit[idx] = value_ch;
			idx++;

			value3 = (data_normal[i + 1] >> 4)
					| (data_normal[i + 2] & 0x3) << 4;
			value_ch = convert_6bit_to_ascii(value3);
			data_16bit[idx] = value_ch;
			idx++;

			value4 = data_normal[i + 2] >> 2;
			value_ch = convert_6bit_to_ascii(value4);
			data_16bit[idx] = value_ch;
			idx++;

		}

		/*
		 value1_ch = convert_6bit_to_ascii(value1);
		 value2_ch = convert_6bit_to_ascii(value2);
		 value3_ch = convert_6bit_to_ascii(value3);
		 value4_ch = convert_6bit_to_ascii(value4);

		 printf ("%d[%02X]%c  %d[%02X]%c %d[%02X]%c %d[%02X]%c   i:%d\n", value4,value4, value4_ch, value3,value3, value3_ch, value2,value2, value2_ch, value1,value1, value1_ch, i);
		 //*/
	}

	if (idx >= 50) {
		usb_universal_printf("Data Overflow... max 50 char.\n");
		exit(1);
	}

	data_16bit[idx] = 0;
	*size = idx;

	for (i = 0; i <= idx; i++) {
		result[i] = data_16bit[i];
	}

}

unsigned char conver_char6bit_to_val6bit(unsigned char input) {
	unsigned char buf;
	if ((input >= 0x41) && (input <= 0x5a))
		buf = (input - 0x41);
	if ((input >= 0x61) && (input <= 0x7a))
		buf = (input - 0x61 + 26);
	if ((input >= 0x30) && (input <= 0x39))
		buf = (input - 0x30 + 52);
	if ((input == 0x2b))
		buf = 62;
	if ((input == 0x2f))
		buf = 63;

	return buf;
}

void convert_6bit_to_bytes(unsigned char *result, int *result_size,
		unsigned char *input) {
	int len = 0;
	int i, idx;
	unsigned char val1, val2, val3;
	unsigned char tmp_6bit[50];

	len = strlen((const char*) input) - 0;

	for (i = 0; i < len; i++) {
		tmp_6bit[i] = conver_char6bit_to_val6bit(input[i]);
		//usb_printf("%d ", tmp_6bit[i]);
	}
//	usb_printf("\r\n");
	i = 0;
	val1 = val2 = val3 = 0;
	idx = 0;
	for (i = 0; i < len; i += 4) {

		/*----------------------------------------
		 printf ("\n");
		 int max=i+3;
		 int idx_0 = i;
		 for (i = max; i >= idx_0; i--)
		 {
		 char *pointer;
		 pointer = byte_to_binary(tmp_6bit[i]);
		 printf("%s ", pointer);
		 if ( i%4==0 ) printf ("x ");
		 free(pointer);
		 }
		 printf ("\n");
		 //*/

		if (i == len - 2) {
			/*
			 printf ("i == len-2  \n");
			 printf ("%d ------- %d\n", i, tmp_6bit[i]);
			 printf ("%d ------- %d\n", i+1, tmp_6bit[i+1]);
			 */
			val1 = tmp_6bit[i] | ((tmp_6bit[i + 1] & 0x03) << 6);
			result[idx] = val1;
			//usb_printf("idx = %d => %d\r\n", idx, val1);
			idx++;

//			printf ("X %d[%02X] ", val1,val1);

		} else if (i == len - 3) {
			/*
			 printf ("i == len-3  \n");
			 printf ("%d ------- %d\n", i, tmp_6bit[i]);
			 printf ("%d ------- %d\n", i+1, tmp_6bit[i+1]);
			 printf ("%d ------- %d\n", i+2, tmp_6bit[i+2]);
			 */
			val1 = tmp_6bit[i] | ((tmp_6bit[i + 1] & 0x03) << 6);
			result[idx] = val1;
			//usb_printf("idx = %d => %d\r\n", idx, val1);
			idx++;

			val2 = ((tmp_6bit[i + 1] >> 2) & 0x0F)
					| ((tmp_6bit[i + 2] & 0x0F) << 4);
			result[idx] = val2;
			//usb_printf("idx = %d => %d\r\n", idx, val2);
			idx++;

//			printf ("X %d[%02X] %d[%02X] ", val2,val2, val1,val1);

		} else {
			val1 = tmp_6bit[i] | ((tmp_6bit[i + 1] & 0x03) << 6);
			result[idx] = val1;
			//usb_printf("idx = %d => %d\r\n", idx, val1);
			idx++;

			val2 = ((tmp_6bit[i + 1] >> 2) & 0x0F)
					| ((tmp_6bit[i + 2] & 0x0F) << 4);
			result[idx] = val2;
			//usb_printf("idx = %d => %d\r\n", idx, val2);
			idx++;

			val3 = (tmp_6bit[i + 2] >> 4) | (tmp_6bit[i + 3] << 2);
			result[idx] = val3;
			//usb_printf("idx = %d => %d\r\n", idx, val3);
			idx++;

//			printf ("X %d[%02X] %d[%02X] %d[%02X] ", val3,val3, val2,val2, val1,val1);
		}
	}
	*result_size = idx;
}

uint16_t sender(unsigned char *b, int count) {
	uint16_t checksum, sum = 0;
	int i;

	for (i = 0; i < count; i++)
		sum += b[i];

	checksum = ~sum;
	return checksum;
}

int receiver(unsigned char *c, int count, short scheck) {
	int checksum, sum = 0, i;

	printf("\n\n****RECEIVER****\n");
	for (i = 0; i < count; i++)
		sum += c[i];

	printf(" RECEIVER SUM IS:%d", sum);
	sum = sum + scheck;
	checksum = ~sum;
	printf("\nRECEIVER's CHECKSUM IS:%d", checksum);
	return checksum;
}

#define CRC32_POLY 0x04c11db7
uint32_t table[256];
uint8_t have_table = 0;

uint32_t crc32_check(char *buf, uint16_t len) {

	static int have_table = 0;
	uint32_t rem;
	uint8_t octet;
	int i, j;
	const char *p, *q;
	uint32_t crc = 0;
	/* This check is not thread safe; there is no mutex. */
	if (have_table == 0) {
		/* Calculate CRC table. */
		for (i = 0; i < 256; i++) {
			rem = i; /* remainder from polynomial division */
			for (j = 0; j < 8; j++) {
				if (rem & 1) {
					rem >>= 1;
					rem ^= 0xedb88320;
				} else
					rem >>= 1;
			}
			table[i] = rem;
		}
		have_table = 1;
	}

	crc = ~crc;
	q = buf + len;
	for (p = buf; p < q; p++) {
		octet = *p;
		crc = (crc >> 8) ^ table[(crc & 0xff) ^ octet];
	}
	return ~crc;
}

uint16_t ccrc16(uint8_t *bytes, uint16_t length) {
	uint16_t crc = 0xffff;
	for (uint16_t b = 0; b < length; ++b) {
		crc = ((crc << 8) & 0xff00)
				^ CRC16_XXMODEM_TABLE[((crc >> 8) & 0xff) ^ bytes[b]];
	}
	return rreverseBits(~crc);

}

uint16_t rreverseBits(uint16_t crc) {
	uint16_t result = 0;
	for (size_t i = 0; i < 16; ++i) {
		result <<= 1;
		result |= ((crc & (1 << i)) >> i);
	}

	return result;
}

bool cekCRC(unsigned char *datainput, unsigned char *crcinput) {
	uint16_t crc2;
	//char datakirim[512];
	unsigned char crccek[50];

	crc2 = ccrc16(datainput, strlen((char*) datainput));

	memset(crccek, 0, sizeof(crccek));
	sprintf((char*) crccek, "%04X", crc2);
	//memset(datakirim, 0, sizeof(datakirim));
	//sprintf(datakirim, "%s : %s : %s \n", datainput, crcinput, crccek);
	//usb_printf(datakirim);

	if (strcmp((char*) crcinput, (char*) crccek) == 0) {
		usb_universal_printf("Data CRC sama.\r\n");
		return true;
	} else {
		usb_universal_printf("Data CRC tidak sama.\r\n");
		return false;
	}

}

char* zstring_strtok(char *str, const char *delim) {
	static char *static_str = 0; /* var to store last address */
	int index = 0, strlength = 0; /* integers for indexes */
	int found = 0; /* check if delim is found */

	/* delimiter cannot be NULL
	 * if no more char left, return NULL as well
	 */
	if (delim == 0 || (str == 0 && static_str == 0))
		return 0;

	if (str == 0)
		str = static_str;

	/* get length of string */
	while (str[strlength])
		strlength++;

	/* find the first occurance of delim */
	for (index = 0; index < strlength; index++)
		if (str[index] == delim[0]) {
			found = 1;
			break;
		}

	/* if delim is not contained in str, return str */
	if (!found) {
		static_str = 0;
		return str;
	}

	/* check for consecutive delimiters
	 *if first char is delim, return delim
	 */
	if (str[0] == delim[0]) {
		static_str = (str + 1);
		return (char*) delim;
	}

	/* terminate the string
	 * this assignmetn requires char[], so str has to
	 * be char[] rather than *char
	 */
	str[index] = '\0';

	/* save the rest of the string */
	if ((str + index + 1) != 0)
		static_str = (str + index + 1);
	else
		static_str = 0;

	return str;
}

int valueFromString(char* string,int start, int width)
{
	int value=0;
	for( int n=0; n < width; n++ )
		value = value * 10 + string[start +n] - '0';

	return value;
}
