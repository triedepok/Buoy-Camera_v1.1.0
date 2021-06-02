/*
 * TEWSvoltage.h
 *
 *  Created on: Jan 29, 2021
 *      Author: ihsan
 */

#ifndef INC_TEWSVOLTAGE_H_
#define INC_TEWSVOLTAGE_H_

#include "parameterTEWS.h"

/*!
| LTC4151 I2C Address | Value      |   AD1    |   AD2    |
| :------------------ | :------:   | :------: | :------: |
| LTC4151_I2C_ADDRESS | 0x67       |   High   |   Low    |
| LTC4151_I2C_ADDRESS | 0x68       |  Float   |  High    |
| LTC4151_I2C_ADDRESS | 0x69       |  High    |  High    |
| LTC4151_I2C_ADDRESS | 0x6A       |  Float   |  Float   |
| LTC4151_I2C_ADDRESS | 0x6B       |  Float   |  Low     |
| LTC4151_I2C_ADDRESS | 0x6C       |  Low     |  High    |
| LTC4151_I2C_ADDRESS | 0x6D       |  High    |  Float   |
| LTC4151_I2C_ADDRESS | 0x6E       |  Low     |  Float   |
| LTC4151_I2C_ADDRESS | 0x6F       |  Low     |  Low     |
| LTC4151_I2C_GLOBAL_ADDRESS | 0x66 |  X      |  X       |
*/

/*! @name I2C_Addresses
@{ */

// Address assignment
// LTC4151 I2C Address                 //  AD1       AD0
 #define LTC4151_I2C_ADDRESS 0x67<<1   //  High      Low
// #define LTC4151_I2C_ADDRESS 0x68    //  Float     High
// #define LTC4151_I2C_ADDRESS 0x69    //  High      High
// #define LTC4151_I2C_ADDRESS 0x6A    //  Float     Float
// #define LTC4151_I2C_ADDRESS 0x6B    //  Float     Low
// #define LTC4151_I2C_ADDRESS 0x6C    //  Low       High
// #define LTC4151_I2C_ADDRESS 0x6D    //  High      Float
// #define LTC4151_I2C_ADDRESS 0x6E    //  Low       Float
//#define LTC4151_I2C_ADDRESS 0x6F      //  Low       Low

// LTC4151 Global I2C Address.
// #define LTC4151_I2C_GLOBAL_ADDRESS 0x66  //  Global Address

//! @} */

//! Note: Register 0x07 is Reserved.
/*!
| Registers                 |   Location    | Descrition                                |
| :------------------------ | :-----------: | :---------------------------------------- |
| LTC4151_SENSE_MSB_REG     | 0x00          | ADC Current Sense Voltage Data (8 MSB)    |
| LTC4151_SENSE_LSB_REG     | 0x01          | ADC Current Sense Voltage Data (4 LSB)    |
| LTC4151_VIN_MSB_REG       | 0x02          | ADC V_IN Voltage Data (8 MSB)             |
| LTC4151_VIN_LSB_REG       | 0x03          | ADC V_IN Voltage Data (4 LSB)             |
| LTC4151_ADIN_MSB_REG      | 0x04          | ADC ADIN Voltage Data (8 MSB)             |
| LTC4151_ADIN_LSB_REG      | 0x05          | ADC ADIN Voltage Data (4 LSB)             |
| LTC4151_CONTROL_REG       | 0x06          | Controls ADC Operation Mode and Test Mode |

*/

/*! @name REGISTERS
@{ */
// Registers
#define LTC4151_SENSE_MSB_REG       0x00  // ADC Current Sense Voltage Data (8 MSB)
#define LTC4151_SENSE_LSB_REG       0x01  // ADC Current Sense Voltage Data (4 LSB)
#define LTC4151_VIN_MSB_REG         0x02  // ADC V_IN Voltage Data (8 MSB)
#define LTC4151_VIN_LSB_REG         0x03  // ADC V_IN Voltage Data (4 LSB)
#define LTC4151_ADIN_MSB_REG        0x04  // ADC ADIN Voltage Data (8 MSB)
#define LTC4151_ADIN_LSB_REG        0x05  // ADC ADIN Voltage Data (4 LSB)
#define LTC4151_CONTROL_REG         0x06  // Controls ADC Operation Mode and Test Mode
//!@} */

//! Bitwise AND  with channel register, and write to LTC4151_CONTROL_REG.
/*!
| Commands and Channel Registers    |   Value   |   Descrition                          |
| :----------------------------     |   :---:   |   :---------                          |
| LTC4151_CONTINUOUS_MODE           |   0x00    |   Sets LTC4151 in Continuous Mode     |
| LTC4151_SNAPSHOT_MODE             |   0x80    |   Sets LTC4151 in Snapshot Mode       |
| LTC4151_SENSE_CHANNEL_REG         |   0x00    |   ADC Sense Channel For Snapshot Mode |
| LTC4151_VIN_CHANNEL_REG           |   0x20    |   DC V_IN Channel For Snapshot Mode   |
| LTC4151_ADIN_CHANNEL_REG          |   0x40    |   ADC ADIN Channel For Snapshot Mode  |
*/

/*! @name LTC4151 Commands
@{ */

#define LTC4151_SENSE_CHANNEL_REG   0x00  // ADC Sense Channel For Snapshot Mode
#define LTC4151_VIN_CHANNEL_REG     0x20  // ADC V_IN Channel For Snapshot Mode
#define LTC4151_ADIN_CHANNEL_REG    0x40  // ADC ADIN Channel For Snapshot Mode

#define LTC4151_CONTINUOUS_MODE 0x00
#define LTC4151_SNAPSHOT_MODE   0x80
//! @} */

//! Write one byte to an LTC4151 register.
//! @return Returns the state of the acknowledge bit after the I2C address write. 0=acknowledge, 1=no acknowledge.
int8_t LTC4151_write(uint8_t i2c_address,   //!< I2C address of the LTC4151. Configured by tying the ADR0 and ADR1 pins high or low. See Table 1 of datasheet.
                     uint8_t adc_command,   //!< The "command byte" for the LTC4151.
                     uint8_t code           //!< Value that will be written to the register.
                    );

//! Reads a 12-bit value from LTC4151
//! @return Returns the state of the acknowledge bit after the I2C address write. 0=acknowledge, 1=no acknowledge.
int8_t LTC4151_read_12_bits(uint8_t i2c_address,    //!< I2C address of the LTC4151. Configured by tying the ADR0 and ADR1 pins high or low. See Table 1 of datasheet.
                            uint8_t adc_command,    //!< The "command byte" for the LTC4151.
                            uint16_t *adc_code      //!< Returns 12-bit value read from the adc
                           );

//! Calculates the LTC4151 sense current in Amps given "resistor" value in ohms and "LTC4151_sense_lsb" LSB weight in volts.
//! @return Returns the current in Amperes.
float LTC4151_code_to_sense_current(uint16_t adc_code      //!< Code read from the adc (from a function such as LTC4151_read_12_bits)
                                   );

//! Calculates the LTC4151 V_IN voltage given "LTC_vin_lsb" LSB weight in volts
//! @return the V_IN voltage in volts.
float LTC4151_code_to_vin_voltage(uint16_t adc_code        //!< Code read from the adc (from a function such as LTC4151_read_12_bits)
                                 );

//! Calculates the LTC4151 ADIN voltage in mV given "LTC4151_adin_lsb" LSB weight in mV
//! @return the ADIN voltage in volts.
float LTC4151_code_to_ADIN_voltage(uint16_t adc_code     //!< Code read from the adc (from a function such as LTC4151_read_12_bits)
                                  );

uint32_t getvoltage();
uint32_t getcurrent();
uint8_t LTC4151_snapshot(uint8_t i2c_address, //!< I2C address of the LTC4151.
                        uint8_t channel);     //!< Desired channel from the header
#endif /* INC_TEWSVOLTAGE_H_ */
