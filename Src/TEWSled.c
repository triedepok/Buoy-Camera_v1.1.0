/*
 * TEWSled.c
 *
 *  Created on: Mar 25, 2019
 *      Author: Hafiz
 */

#include "TEWSled.h"
#include "parameterTEWS.h"

void setgreenLED(void){
	  HAL_GPIO_WritePin(GPIOE, LED_Pin, 1);
}

void resetgreenLED(void){
	  HAL_GPIO_WritePin(GPIOE, LED_Pin, 0);
}

