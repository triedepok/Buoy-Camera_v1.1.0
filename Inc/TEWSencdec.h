#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <math.h>
#include <string.h>

struct st_data_normal_buoy {
	unsigned long long int nb_try :3;
	unsigned long long int status_buoy :8;
	unsigned long long int snr :5;
	unsigned long long int mdp :5;
	unsigned long long int reserved :3;
};

struct st_pressureAktual {
	unsigned long long int data1 :20;
	unsigned long long int data2 :12;
	unsigned long long int data3 :12;
	unsigned long long int data4 :12;
};

struct st_dataPressureTsu {
	unsigned long long int data1 :20;
	unsigned long long int data2 :12;
	unsigned long long int data3 :12;
	unsigned long long int data4 :12;
};

struct st_dataPredict {
	unsigned long long int predict :12;
	unsigned long long int spare :4;
};

struct st_dataSetting1 { //--- untuk TH, TBN
	unsigned long long int mode_kirim :4;
	unsigned long long int id_obu :8;
	unsigned long long int value :16;
	unsigned long long int spare :4;
};

struct st_dataSetting2 { //--- untuk Power Modem Akustik OBU
	unsigned long long int mode_kirim :4;
	unsigned long long int id_obu :8;
	unsigned long long int value :4;
};

struct st_normalMode {
	unsigned long long int mode_kirim :4;
	unsigned long long int id_obu :8;
	unsigned long long int status_obu :8;
	unsigned long long int counter :10;
	unsigned long long int bln :4;
	unsigned long long int tgl :5;
	unsigned long long int timeofday :17;
};

struct st_tsunamiMode {
	unsigned long long int mode_kirim :4;
	unsigned long long int id_obu :8;
	unsigned long long int status_obu :8;
	unsigned long long int counter :8;
	unsigned long long int spare :2;
	unsigned long long int bln :4;
	unsigned long long int tgl :5;
	unsigned long long int timeofday :17;
};

struct st_dataEngineering_1 { //--- total 56-bit
	unsigned long long int mode_kirim :4;
	unsigned long long int id_obu :8;
	unsigned long long int status_obu :8;
	unsigned long long int bln :4;
	unsigned long long int tgl :5;
	unsigned long long int timeofday :17;
	unsigned long long int tempBPR :10;
};

struct st_dataEngineering_2 { //--- total 56-bit
	unsigned long long int battOBU :11;
	unsigned long long int battModem :11;
	unsigned long long int tempOBU :9;
	unsigned long long int kelembabanOBU :10;
	unsigned long long int tilt :10;
	unsigned long long int reserved :5;
};

struct st_dataEngineering_buoy { //--- total 32-bit
	unsigned long long int battBuoy :12;
	unsigned long long int tempBuoy :10;
	unsigned long long int kelembabanBuoy :10;
};

struct st_data_lat_separate {
	unsigned long long int lat1 :18;
	unsigned long long int lat2 :9;
	unsigned long long int lat_sign :1;
	unsigned long long int kosong :4;
};

struct st_data_lat_full {
	int data :27;
	int data_sign :1;
	unsigned long long int kosong :4;
};

struct st_data_position1 { // 64bit ---> 8byte
	unsigned long long int mode_kirim :4;
	unsigned long long int id_lokasi :8;
	unsigned long long int status_buoy :8;
	unsigned long long int bln :4;
	unsigned long long int tgl :5;
	unsigned long long int timeofday :17;
	unsigned long long int lat1 :18;
};
struct st_data_position2 { // 56bit ---> 7byte
	unsigned long long int lat2 :9;
	unsigned long long int lat_sign :1;
	long long int lon :28;
	long long int lon_sign :1;
	unsigned long long int alt :12;
	unsigned long long int flag_kirim :5;
};
struct st_data_position3 { // 2byte
	unsigned long long int jml_sat :8;
	unsigned long long int hdop :8;
};

struct st_eng_buoy_1 { //-------------------------------- 64 bit
	unsigned long long int mode_kirim :4;
	unsigned long long int id_lokasi :8;
	unsigned long long int status_buoy :8;
	unsigned long long int bln :4;
	unsigned long long int tgl :5;
	unsigned long long int timeofday :17;
	unsigned long long int snr :9;
	unsigned long long int mdp :9;
};

struct st_eng_buoy_2 { //--------------------- 40 bit
	unsigned long long int agc :9;
	unsigned long long int voltase_batt :12;
	unsigned long long int temperatur :9;
	unsigned long long int kelembaban :10;
};

union un_eng_buoy_1 {
	struct st_eng_buoy_1 eng_buoy1;
	unsigned char data[8];
};

union un_eng_buoy_2 {
	struct st_eng_buoy_2 eng_buoy2;
	unsigned char data[5];
};

union un_data_lat {
	struct st_data_lat_separate separate;
	struct st_data_lat_full lat_full;
	unsigned char data[4];
};
union un_data_position1 {
	struct st_data_position1 st_data;
	unsigned char data[8];
};
union un_data_position2 {
	struct st_data_position2 st_data2;
	unsigned char data[7];
};
union un_data_position3 {
	struct st_data_position3 st_data3;
	unsigned char data[2];
};

union un_test {
	unsigned int data_int_unsigned;
	int data_int;
	unsigned char chr[4];
};

union un_dataSetting1 {
	struct st_dataSetting1 setting;
	unsigned char data[4];
};

union un_dataSetting2 {
	struct st_dataSetting2 setting;
	unsigned char data[3];
};

union un_dataEngineering_buoy {
	struct st_dataEngineering_buoy dataEngineering;
	unsigned char data[4];
};

union un_dataEngineering_1 {
	struct st_dataEngineering_1 dataEngineering;
	unsigned char data[7];
};

union un_dataEngineering_2 {
	struct st_dataEngineering_2 dataEngineering;
	unsigned char data[7];
};

union normalMode {
	struct st_normalMode param;
	unsigned char data[13];
};

union tsunamiMode {
	struct st_tsunamiMode param;
	unsigned char data[13];
};

union dataPressureAktual {
	struct st_pressureAktual dataPressure;
	unsigned char data[7];
};

union predictData {
	struct st_dataPredict item;
	unsigned char data[2];
};

union un_data_normal_buoy {
	struct st_data_normal_buoy dataBuoy;
	unsigned char data[3];
};

typedef struct {
	unsigned int mode_kirim;
	unsigned int id_obu;
	unsigned int id_lokasi;
	unsigned int status_obu;
	unsigned int status_buoy;
	unsigned int counter;
	unsigned int bln;
	unsigned int tgl;
	unsigned int timeofday;
	unsigned int data1;
	unsigned int data2;
	unsigned int data3;
	unsigned int data4;
	unsigned int predict;
	unsigned int crc;
	unsigned char DataInOut[50];
	unsigned int tempBPR;
	unsigned int battOBU;
	unsigned int battModem;
	unsigned int tempOBU;
	unsigned int kelembabanOBU;
	unsigned int tilt;
	unsigned int snr;
	unsigned int mdp;
	unsigned int agc;
	unsigned int battBuoy;
	unsigned int tempBuoy;
	unsigned int kelembabanBuoy;
	unsigned int val_TH;
	unsigned int val_TBN;
	unsigned int val_POWM;
	int lat;
	int lon;
	unsigned int alt;
	unsigned int flag_kirim;
	unsigned int flag_jml_sat;
	unsigned int hdop;
} DECODE_t;

extern DECODE_t DECODE;

int encode_normal(unsigned char *data_out, unsigned int mode_kirim,
		unsigned int id_obu, unsigned int status_obu, unsigned int counter,
		unsigned int bln, unsigned int tgl, unsigned int timeofday,
		unsigned int data1, unsigned int data2, unsigned int data3,
		unsigned int data4);
int encode_tsunami(unsigned char *data_out, unsigned int mode_kirim,
		unsigned int id_obu, unsigned int status_obu, unsigned int counter,
		unsigned int bln, unsigned int tgl, unsigned int timeofday,
		unsigned int data1, unsigned int data2, unsigned int data3,
		unsigned int data4, unsigned int predict);
int encode_dataEng_OBU(unsigned char *result, unsigned int mode_kirim,
		unsigned int id_obu, unsigned int status_obu, unsigned int bln,
		unsigned int tgl, unsigned int timeofday, unsigned int tempBPR,
		unsigned int battOBU, unsigned int battModem, unsigned int tempOBU,
		unsigned int kelembabanOBU, unsigned int tilt);
int encode_dataEng_Buoy(unsigned char *data_enc_out, unsigned int mode_kirim,
		unsigned int id_lokasi, unsigned int status_buoy, unsigned int bln,
		unsigned int tgl, unsigned int timeofday, unsigned int snr,
		unsigned int mdp, unsigned int agc, unsigned int battBuoy,
		unsigned int tempBuoy, unsigned int kelembabanBuoy);
int encode_dataNormal_Buoy(unsigned char *enc_data, unsigned int nb_try,
		unsigned int status_buoy, unsigned int snr, unsigned int mdp);
int encode_dataSetting1(unsigned char *enc_data, unsigned int mode_kirim,
		unsigned int id_obu, unsigned int val);
int encode_dataSetting2(unsigned char *enc_data, unsigned int mode_kirim,
		unsigned int id_obu, unsigned int val);
int encode_dataLat(unsigned char *enc_data, unsigned int *lat1,
		unsigned int *lat2, unsigned int *sign, int val);
int encode_position(unsigned char *data_out, unsigned int mode_kirim,
		unsigned int id_obu, unsigned int status_buoy, unsigned int bln,
		unsigned int tgl, unsigned int timeofday, int lat, int lon,
		unsigned int alt, unsigned int flag_kirim, unsigned int flag_jml_sat,
		unsigned int hdop);
void decode_modekirim(unsigned char *data_in, unsigned int *mode_kirim);
void decode_normal(unsigned char *data_in, unsigned int *mode_kirim,
		unsigned int *id_obu, unsigned int *status_obu, unsigned int *counter,
		unsigned int *bln, unsigned int *tgl, unsigned int *timeofday,
		unsigned int *data1, unsigned int *data2, unsigned int *data3,
		unsigned int *data4);
void decode_tsunami(unsigned char *data_in, unsigned int *mode_kirim,
		unsigned int *id_obu, unsigned int *status_obu, unsigned int *counter,
		unsigned int *bln, unsigned int *tgl, unsigned int *timeofday,
		unsigned int *data1, unsigned int *data2, unsigned int *data3,
		unsigned int *data4, unsigned int *predict);
void decode_dataEng_OBU(unsigned char *data_in, int len,
		unsigned int *mode_kirim, unsigned int *id_obu,
		unsigned int *status_obu, unsigned int *bln, unsigned int *tgl,
		unsigned int *timeofday, unsigned int *tempBPR, unsigned int *battOBU,
		unsigned int *battModem, unsigned int *tempOBU,
		unsigned int *kelembabanOBU, unsigned int *tilt);
void decode_dataEng_Buoy(unsigned char *data_in, unsigned int *mode_kirim,
		unsigned int *id_lokasi, unsigned int *status_buoy, unsigned int *bln,
		unsigned int *tgl, unsigned int *timeofday, unsigned int *snr,
		unsigned int *mdp, unsigned int *agc, unsigned int *battBuoy,
		unsigned int *tempBuoy, unsigned int *kelembabanBuoy);
void decode_dataSetting1(unsigned char *data, unsigned int *mode_kirim,
		unsigned int *id_obu, unsigned int *val);
void decode_dataSetting2(unsigned char *data, unsigned int *mode_kirim,
		unsigned int *id_obu, unsigned int *val);
char* byte_to_binary(unsigned char n);
unsigned char convert_6bit_to_ascii(unsigned char input);
void convert_bytes_to_6bit(unsigned char *result, unsigned char *data_normal,
		int *size, int max_index);
unsigned char convert_char6bit_to_val6bit(unsigned char input);
void convert_6bit_to_bytes(unsigned char *result, int *result_size,
		unsigned char *input);

uint16_t sender(unsigned char *b, int count);
int receiver(unsigned char *c, int count, short scheck);

uint32_t crc32_check(char *buf, uint16_t len);

int valueFromString(char* string,int start, int width);

uint16_t ccrc16(uint8_t *bytes, uint16_t length);
uint16_t rreverseBits(uint16_t crc);
bool cekCRC(unsigned char *datainput, unsigned char *crcinput);
char* zstring_strtok(char *str, const char *delim);

static const uint16_t CRC16_XXMODEM_TABLE[] = { 0x0000, 0x1021, 0x2042, 0x3063,
		0x4084, 0x50a5, 0x60c6, 0x70e7, 0x8108, 0x9129, 0xa14a, 0xb16b, 0xc18c,
		0xd1ad, 0xe1ce, 0xf1ef, 0x1231, 0x0210, 0x3273, 0x2252, 0x52b5, 0x4294,
		0x72f7, 0x62d6, 0x9339, 0x8318, 0xb37b, 0xa35a, 0xd3bd, 0xc39c, 0xf3ff,
		0xe3de, 0x2462, 0x3443, 0x0420, 0x1401, 0x64e6, 0x74c7, 0x44a4, 0x5485,
		0xa56a, 0xb54b, 0x8528, 0x9509, 0xe5ee, 0xf5cf, 0xc5ac, 0xd58d, 0x3653,
		0x2672, 0x1611, 0x0630, 0x76d7, 0x66f6, 0x5695, 0x46b4, 0xb75b, 0xa77a,
		0x9719, 0x8738, 0xf7df, 0xe7fe, 0xd79d, 0xc7bc, 0x48c4, 0x58e5, 0x6886,
		0x78a7, 0x0840, 0x1861, 0x2802, 0x3823, 0xc9cc, 0xd9ed, 0xe98e, 0xf9af,
		0x8948, 0x9969, 0xa90a, 0xb92b, 0x5af5, 0x4ad4, 0x7ab7, 0x6a96, 0x1a71,
		0x0a50, 0x3a33, 0x2a12, 0xdbfd, 0xcbdc, 0xfbbf, 0xeb9e, 0x9b79, 0x8b58,
		0xbb3b, 0xab1a, 0x6ca6, 0x7c87, 0x4ce4, 0x5cc5, 0x2c22, 0x3c03, 0x0c60,
		0x1c41, 0xedae, 0xfd8f, 0xcdec, 0xddcd, 0xad2a, 0xbd0b, 0x8d68, 0x9d49,
		0x7e97, 0x6eb6, 0x5ed5, 0x4ef4, 0x3e13, 0x2e32, 0x1e51, 0x0e70, 0xff9f,
		0xefbe, 0xdfdd, 0xcffc, 0xbf1b, 0xaf3a, 0x9f59, 0x8f78, 0x9188, 0x81a9,
		0xb1ca, 0xa1eb, 0xd10c, 0xc12d, 0xf14e, 0xe16f, 0x1080, 0x00a1, 0x30c2,
		0x20e3, 0x5004, 0x4025, 0x7046, 0x6067, 0x83b9, 0x9398, 0xa3fb, 0xb3da,
		0xc33d, 0xd31c, 0xe37f, 0xf35e, 0x02b1, 0x1290, 0x22f3, 0x32d2, 0x4235,
		0x5214, 0x6277, 0x7256, 0xb5ea, 0xa5cb, 0x95a8, 0x8589, 0xf56e, 0xe54f,
		0xd52c, 0xc50d, 0x34e2, 0x24c3, 0x14a0, 0x0481, 0x7466, 0x6447, 0x5424,
		0x4405, 0xa7db, 0xb7fa, 0x8799, 0x97b8, 0xe75f, 0xf77e, 0xc71d, 0xd73c,
		0x26d3, 0x36f2, 0x0691, 0x16b0, 0x6657, 0x7676, 0x4615, 0x5634, 0xd94c,
		0xc96d, 0xf90e, 0xe92f, 0x99c8, 0x89e9, 0xb98a, 0xa9ab, 0x5844, 0x4865,
		0x7806, 0x6827, 0x18c0, 0x08e1, 0x3882, 0x28a3, 0xcb7d, 0xdb5c, 0xeb3f,
		0xfb1e, 0x8bf9, 0x9bd8, 0xabbb, 0xbb9a, 0x4a75, 0x5a54, 0x6a37, 0x7a16,
		0x0af1, 0x1ad0, 0x2ab3, 0x3a92, 0xfd2e, 0xed0f, 0xdd6c, 0xcd4d, 0xbdaa,
		0xad8b, 0x9de8, 0x8dc9, 0x7c26, 0x6c07, 0x5c64, 0x4c45, 0x3ca2, 0x2c83,
		0x1ce0, 0x0cc1, 0xef1f, 0xff3e, 0xcf5d, 0xdf7c, 0xaf9b, 0xbfba, 0x8fd9,
		0x9ff8, 0x6e17, 0x7e36, 0x4e55, 0x5e74, 0x2e93, 0x3eb2, 0x0ed1, 0x1ef0 };
