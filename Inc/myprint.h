/*
 * printf.h
 *
 *  Created on: Dec 7, 2018
 *      Author: trie
 */

#ifndef PRINTF_H_
#define PRINTF_H_

#include <stdio.h>
#include <stdarg.h>
#include <string.h>
#include "stm32l4xx_hal.h"
#include "parameterTEWS.h"

void usb_printf(const char *fmt, ...);
void bluetooth_printf(const char *fmt, ...);
void usb_universal_printf(const char *fmt, ...);
void iridium_printf(const char *fmt, ...);
void gps_printf(const char *fmt, ...);
void ais_printf(const char *fmt, ...);

#endif /* PRINTF_H_ */
