/*
 * TEWSvoltage.c
 *
 *  Created on: Jan 29, 2021
 *      Author: ihsan
 */

#include "TEWSvoltage.h"
#include "parameterTEWS.h"

//! @ingroup Power_Monitors
//! @{
//! @defgroup LTC4151 LTC4151: High Voltage I2C Current and Voltage Monitor
//! @}

/*! @file
    @ingroup LTC4151
    Library for LTC4151: High Voltage I2C Current and Voltage Monitor
*/

 const float resistor = 0.02;             //!< Sense resistor value
 const float LTC4151_sense_lsb = 20e-6;  //!< Typical sense lsb weight in volts
 const float LTC4151_vin_lsb = 0.025;    //!< Typical Vin lsb weight in volts
 const float LTC4151_adin_lsb = 0.5;     //!< Typical ADIN lsb weight in mV

// Write one byte to an LTC4151 register.
int8_t LTC4151_write(uint8_t i2c_address, uint8_t adc_command, uint8_t code)
{
  int32_t ack;

  //ack = i2c_write_byte_data(i2c_address, adc_command, code);
  ack = HAL_I2C_Mem_Write(&hi2c3,(uint16_t)i2c_address,adc_command,1,&code,1,HAL_MAX_DELAY);
  if(ack != HAL_OK) {
	  //usb_printf("LTC4151 Error Tx, %d\r\n", ack);
	  return false;
  } else {
	  return(ack);
  }


}

// Reads a 12-bit value from LTC4151
int8_t LTC4151_read_12_bits(uint8_t i2c_address, uint8_t adc_command, uint16_t *adc_code)
{

	/*

  uint8_t buf[12];


  memset(buf, 0, sizeof(buf));
  //ack = i2c_read_word_data(i2c_address, adc_command, adc_code);
  ack = HAL_I2C_Master_Transmit(&hi2c3, i2c_address, &adc_command, 1, HAL_MAX_DELAY);
  if ( ack != HAL_OK ) {
	  usb_printf("Error Tx\r\n");
  } else {

    // Read 2 bytes from the temperature register
	  ack = HAL_I2C_Master_Receive(&hi2c3, i2c_address, buf, 2, HAL_MAX_DELAY);
    if ( ack != HAL_OK ) {
    	usb_printf("Error Rx\r\n");
    } else {

      //Combine the bytes
    	usb_printf("%u, %u, %u, %u\r\n", buf[0], buf[1], buf[2], buf[3]);
      val = ((int16_t)buf[0] << 8) | (buf[1]);
      val = val>>4;
      usb_printf("%d\r\n", val);
      // Convert to 2's complement, since temperature can be negative
      if ( val > 0x7FF ) {
        val |= 0xF000;
      }
    }
  }
*/
	int32_t ack = 0;
	int16_t val;
	int j;
	uint8_t length = 2;
	uint8_t data1[length];
	memset(data1, 0 , sizeof(data1));
	for(j=0;j<length;j++){
		HAL_I2C_Mem_Read(&hi2c3,(uint16_t)i2c_address, adc_command, 1, (uint8_t*)&data1[j], 1, HAL_MAX_DELAY); //read memory address 08
		adc_command++;
	 }
	usb_printf("%u, %u\r\n", data1[0], data1[1]);
	val = ((int16_t)data1[0] << 8) | (data1[1]);
    val = val>>4;
    usb_printf("%d\r\n", val);

  *adc_code = val;

  return(ack);
}

// Note that the following functions are somewhat contrived, and the units are arbitrary.  LTC4151_ADIN_voltage could be given an LSB
// weight (LTC4151_adin_lsb) in volts instead of mV, and the returned voltage will be in volts instead of mV.
// Similarly, the LTC4151_sense_current function could be written to accept an LSB weight in amps or milliamps.
// That would eliminate the extra floating point divide operation based on the sense resistor value.

// Calculates the LTC4151 sense current in Amps given "resistor" value in ohms and "LTC4151_sense_lsb" LSB weight in volts.
float LTC4151_code_to_sense_current(uint16_t adc_code)
{
  float voltage, current;
  voltage = (float)adc_code * LTC4151_sense_lsb;    //! 1) Convert code to voltage from sense lsb
  current = voltage / resistor;                     //! 2) Calculate Current, I = V/R
  return (current);
}

// Calculates the LTC4151 V_IN voltage given "LTC_vin_lsb" LSB weight in volts
float LTC4151_code_to_vin_voltage(uint16_t adc_code)
{
  float voltage;
  voltage = (float)adc_code * LTC4151_vin_lsb;  //! 1) Convert code to voltage from VIN lsb
  return (voltage);
}

// Calculates the LTC4151 ADIN voltage in mV given "LTC4151_adin_lsb" LSB weight in mV
float LTC4151_code_to_ADIN_voltage(uint16_t adc_code)
{
  float voltage;
  voltage = (float)adc_code * LTC4151_adin_lsb; //! 1) Convert Code to voltage from ADIN lsb
  return (voltage);
}

uint32_t getvoltage()
{
	uint32_t volt;
	int16_t val;
	int j;
	uint8_t length = 2;
	uint8_t data1[length];
	uint8_t address = LTC4151_VIN_MSB_REG;
	memset(data1, 0 , sizeof(data1));
	for(j=0;j<length;j++){
		HAL_I2C_Mem_Read(&hi2c3,(uint16_t)LTC4151_I2C_ADDRESS, address, 1, (uint8_t*)&data1[j], 1, HAL_MAX_DELAY); //read memory address 08
		address++;
	 }
	//usb_printf("%u, %u\r\n", data1[0], data1[1]);
	val = ((int16_t)data1[0] << 8) | (data1[1]);
    val = val>>4;
    //usb_printf("%d\r\n", val);
	volt = (val * 25)/10;

	return volt;
}

uint32_t getcurrent(){
	float current;
	uint32_t curr;
	int16_t val;
	int j;
	uint8_t length = 2;
	uint8_t data1[length];
	uint8_t address = LTC4151_SENSE_MSB_REG;
	memset(data1, 0 , sizeof(data1));
	for(j=0;j<length;j++){
		HAL_I2C_Mem_Read(&hi2c3,(uint16_t)LTC4151_I2C_ADDRESS, address, 1, (uint8_t*)&data1[j], 1, HAL_MAX_DELAY); //read memory address 08
		address++;
	 }
	//usb_printf("%u, %u\r\n", data1[0], data1[1]);
	val = ((int16_t)data1[0] << 8) | (data1[1]);
    val = val>>4;
    //usb_printf("%d\r\n", val);

    current = LTC4151_code_to_sense_current(val);
    curr = (uint32_t)(current * 1000.0);
	return curr;
}

uint8_t LTC4151_snapshot(uint8_t i2c_address, //!< I2C address of the LTC4151.
                        uint8_t channel)     //!< Desired channel from the header
{
  uint8_t ack = 0;
  ack |= LTC4151_write(i2c_address, LTC4151_CONTROL_REG, LTC4151_CONTINUOUS_MODE);              //! Disable previous snapshot mode to allow a new snapshot
  ack |= LTC4151_write(i2c_address, LTC4151_CONTROL_REG, (channel | LTC4151_SNAPSHOT_MODE));    //! Re-enable snapshot mode on selected channel
  return(ack);
}
