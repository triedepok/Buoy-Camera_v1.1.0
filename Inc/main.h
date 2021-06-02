/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.h
  * @brief          : Header for main.c file.
  *                   This file contains the common defines of the application.
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2021 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under Ultimate Liberty license
  * SLA0044, the "License"; You may not use this file except in compliance with
  * the License. You may obtain a copy of the License at:
  *                             www.st.com/SLA0044
  *
  ******************************************************************************
  */
/* USER CODE END Header */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __MAIN_H
#define __MAIN_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32l4xx_hal.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

/* Exported types ------------------------------------------------------------*/
/* USER CODE BEGIN ET */

/* USER CODE END ET */

/* Exported constants --------------------------------------------------------*/
/* USER CODE BEGIN EC */

/* USER CODE END EC */

/* Exported macro ------------------------------------------------------------*/
/* USER CODE BEGIN EM */

/* USER CODE END EM */

/* Exported functions prototypes ---------------------------------------------*/
void Error_Handler(void);

/* USER CODE BEGIN EFP */

/* USER CODE END EFP */

/* Private defines -----------------------------------------------------------*/
#define V_SCL_Pin GPIO_PIN_0
#define V_SCL_GPIO_Port GPIOC
#define V_SDA_Pin GPIO_PIN_1
#define V_SDA_GPIO_Port GPIOC
#define BT_TX_Pin GPIO_PIN_0
#define BT_TX_GPIO_Port GPIOA
#define BT_RX_Pin GPIO_PIN_1
#define BT_RX_GPIO_Port GPIOA
#define BT_CON_Pin GPIO_PIN_2
#define BT_CON_GPIO_Port GPIOA
#define ALARM_IN_Pin GPIO_PIN_6
#define ALARM_IN_GPIO_Port GPIOA
#define SWT_TRIG_Pin GPIO_PIN_7
#define SWT_TRIG_GPIO_Port GPIOA
#define LEAKAGE_Pin GPIO_PIN_7
#define LEAKAGE_GPIO_Port GPIOE
#define LED_Pin GPIO_PIN_8
#define LED_GPIO_Port GPIOE
#define UNI_RX_Pin GPIO_PIN_10
#define UNI_RX_GPIO_Port GPIOB
#define UNI_TX_Pin GPIO_PIN_11
#define UNI_TX_GPIO_Port GPIOB
#define RTC_SCL_Pin GPIO_PIN_13
#define RTC_SCL_GPIO_Port GPIOB
#define RTC_SDA_Pin GPIO_PIN_14
#define RTC_SDA_GPIO_Port GPIOB
#define DS_WAKEUP_Pin GPIO_PIN_15
#define DS_WAKEUP_GPIO_Port GPIOB
#define RTC_RST_Pin GPIO_PIN_8
#define RTC_RST_GPIO_Port GPIOD
#define RTC2_SCL_Pin GPIO_PIN_12
#define RTC2_SCL_GPIO_Port GPIOD
#define RTC2_SDA_Pin GPIO_PIN_13
#define RTC2_SDA_GPIO_Port GPIOD
#define GPS_PPS_Pin GPIO_PIN_4
#define GPS_PPS_GPIO_Port GPIOD
#define GPS_TX_Pin GPIO_PIN_5
#define GPS_TX_GPIO_Port GPIOD
#define GPS_RX_Pin GPIO_PIN_6
#define GPS_RX_GPIO_Port GPIOD
#define GPS_RESET_Pin GPIO_PIN_7
#define GPS_RESET_GPIO_Port GPIOD
#define NETWORKINT_Pin GPIO_PIN_5
#define NETWORKINT_GPIO_Port GPIOB
#define DF_S_TX_Pin GPIO_PIN_6
#define DF_S_TX_GPIO_Port GPIOB
#define DF_S_RX_Pin GPIO_PIN_7
#define DF_S_RX_GPIO_Port GPIOB
#define EEPROM_SCL_Pin GPIO_PIN_8
#define EEPROM_SCL_GPIO_Port GPIOB
#define EEPROM_SDA_Pin GPIO_PIN_9
#define EEPROM_SDA_GPIO_Port GPIOB
#define ON_OFF_Pin GPIO_PIN_1
#define ON_OFF_GPIO_Port GPIOE
/* USER CODE BEGIN Private defines */

/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
