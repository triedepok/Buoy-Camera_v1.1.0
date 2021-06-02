/*
 * printf.c
 *
 *  Created on: Dec 7, 2018
 *      Author: pte
 */


#include <myprint.h>


void usb_printf(const char *fmt, ...){
    va_list argp;
    va_start(argp, fmt);
    char string[200];
    if(0 < vsprintf(string,fmt,argp)){
        HAL_UART_Transmit(&_UNI_USART, (uint8_t*)string, strlen(string), 0xffffff);
    }
    va_end(argp);
}

void bluetooth_printf(const char *fmt, ...){
    va_list argp;
    va_start(argp, fmt);
    char string[200];
    if(0 < vsprintf(string,fmt,argp)){
        HAL_UART_Transmit(&_BT_USART, (uint8_t*)string, strlen(string), 0xffffff);
    }
    va_end(argp);
}

void usb_universal_printf(const char *fmt, ...){
    va_list argp;
    va_start(argp, fmt);
    char string[200];
    if(0 < vsprintf(string,fmt,argp)){
        HAL_UART_Transmit(&_UNI_USART, (uint8_t*)string, strlen(string), 0xffffff);
        if(debugbluetooth == 1) HAL_UART_Transmit(&_BT_USART, (uint8_t*)string, strlen(string), 0xffffff);
    }
    va_end(argp);
}

void iridium_printf(const char *fmt, ...){
    va_list argp;
    va_start(argp, fmt);
    char string[200];
    if(0 < vsprintf(string,fmt,argp)){
        HAL_UART_Transmit(&_IRIDIUM_USART, (uint8_t*)string, strlen(string), 0xffffff);
    }
    va_end(argp);
}

void gps_printf(const char *fmt, ...){
    va_list argp;
    va_start(argp, fmt);
    char string[200];
    if(0 < vsprintf(string,fmt,argp)){
        HAL_UART_Transmit(&_GPS_USART, (uint8_t*)string, strlen(string), 0xffffff);
    }
    va_end(argp);
}

void ais_printf(const char *fmt, ...){
    va_list argp;
    va_start(argp, fmt);
    char string[200];
    if(0 < vsprintf(string,fmt,argp)){
        HAL_UART_Transmit(&_UNI_USART, (uint8_t*)string, strlen(string), 0xffffff);
    }
    va_end(argp);
}
