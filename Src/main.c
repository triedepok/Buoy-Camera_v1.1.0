/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
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
/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "cmsis_os.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "parameterTEWS.h"
#include "stm32l4xx_hal_pwr.h"
#include "stm32l4xx_hal_rtc.h"
#include "task.h"
#include "stm32l4xx_hal.h"
#include "FreeRTOSConfig.h"
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
I2C_HandleTypeDef hi2c1;
I2C_HandleTypeDef hi2c2;
I2C_HandleTypeDef hi2c3;
I2C_HandleTypeDef hi2c4;

IWDG_HandleTypeDef hiwdg;

UART_HandleTypeDef hlpuart1;
UART_HandleTypeDef huart4;
UART_HandleTypeDef huart1;
UART_HandleTypeDef huart2;
DMA_HandleTypeDef hdma_lpuart_rx;
DMA_HandleTypeDef hdma_uart4_rx;
DMA_HandleTypeDef hdma_usart1_rx;
DMA_HandleTypeDef hdma_usart2_rx;

RTC_HandleTypeDef hrtc;

/* Definitions for operationTask */
osThreadId_t operationTaskHandle;
const osThreadAttr_t operationTask_attributes = {
  .name = "operationTask",
  .stack_size = 4096 * 4,
  .priority = (osPriority_t) osPriorityAboveNormal,
};
/* Definitions for iridiumTask */
osThreadId_t iridiumTaskHandle;
const osThreadAttr_t iridiumTask_attributes = {
  .name = "iridiumTask",
  .stack_size = 6144 * 4,
  .priority = (osPriority_t) osPriorityHigh,
};
/* Definitions for gpsTask */
osThreadId_t gpsTaskHandle;
const osThreadAttr_t gpsTask_attributes = {
  .name = "gpsTask",
  .stack_size = 2048 * 4,
  .priority = (osPriority_t) osPriorityLow,
};
/* Definitions for usbTask */
osThreadId_t usbTaskHandle;
const osThreadAttr_t usbTask_attributes = {
  .name = "usbTask",
  .stack_size = 4096 * 4,
  .priority = (osPriority_t) osPriorityNormal,
};
/* Definitions for sendiridiumTask */
osThreadId_t sendiridiumTaskHandle;
const osThreadAttr_t sendiridiumTask_attributes = {
  .name = "sendiridiumTask",
  .stack_size = 6144 * 4,
  .priority = (osPriority_t) osPriorityHigh1,
};
/* Definitions for watchdogTask */
osThreadId_t watchdogTaskHandle;
const osThreadAttr_t watchdogTask_attributes = {
  .name = "watchdogTask",
  .stack_size = 2048 * 4,
  .priority = (osPriority_t) osPriorityHigh2,
};
/* Definitions for iridiumrecvTask */
osThreadId_t iridiumrecvTaskHandle;
const osThreadAttr_t iridiumrecvTask_attributes = {
  .name = "iridiumrecvTask",
  .stack_size = 4096 * 4,
  .priority = (osPriority_t) osPriorityHigh3,
};
/* Definitions for iridiumQueue */
osMessageQueueId_t iridiumQueueHandle;
const osMessageQueueAttr_t iridiumQueue_attributes = {
  .name = "iridiumQueue"
};
/* Definitions for usbQueue */
osMessageQueueId_t usbQueueHandle;
const osMessageQueueAttr_t usbQueue_attributes = {
  .name = "usbQueue"
};
/* Definitions for gpsQueue */
osMessageQueueId_t gpsQueueHandle;
const osMessageQueueAttr_t gpsQueue_attributes = {
  .name = "gpsQueue"
};
/* Definitions for iridiumsendQueue */
osMessageQueueId_t iridiumsendQueueHandle;
const osMessageQueueAttr_t iridiumsendQueue_attributes = {
  .name = "iridiumsendQueue"
};
/* Definitions for iridiumrecvQueue */
osMessageQueueId_t iridiumrecvQueueHandle;
const osMessageQueueAttr_t iridiumrecvQueue_attributes = {
  .name = "iridiumrecvQueue"
};
/* Definitions for crcdataQueue */
osMessageQueueId_t crcdataQueueHandle;
const osMessageQueueAttr_t crcdataQueue_attributes = {
  .name = "crcdataQueue"
};
/* Definitions for sizedatairidiumQueue */
osMessageQueueId_t sizedatairidiumQueueHandle;
const osMessageQueueAttr_t sizedatairidiumQueue_attributes = {
  .name = "sizedatairidiumQueue"
};
/* Definitions for sizedatadecodeQueue */
osMessageQueueId_t sizedatadecodeQueueHandle;
const osMessageQueueAttr_t sizedatadecodeQueue_attributes = {
  .name = "sizedatadecodeQueue"
};
/* Definitions for rtcBinarySem */
osSemaphoreId_t rtcBinarySemHandle;
const osSemaphoreAttr_t rtcBinarySem_attributes = {
  .name = "rtcBinarySem"
};
/* Definitions for iridiumrecvBinarySem */
osSemaphoreId_t iridiumrecvBinarySemHandle;
const osSemaphoreAttr_t iridiumrecvBinarySem_attributes = {
  .name = "iridiumrecvBinarySem"
};
/* USER CODE BEGIN PV */
uint32_t generalTaskTimer;
uint32_t iridiumTaskTimer;
uint32_t akustikTaskTimer;
uint32_t sdcardTaskTimer;
uint32_t gpsTaskTimer;
uint32_t usbTaskTimer;
uint32_t datadecodeTaskTimer;
uint32_t prevdataengTaskTimer;
uint32_t dataengTaskTimer;
uint32_t prevGPSTaskTimer1;
uint32_t prevGPSTaskTimer2;
uint8_t  statusSendGPS;
uint32_t datahandleTaskTimer;
uint32_t acoenvTaskTimer;
uint32_t iridiumrecvTaskTimer;

_RTC rtcTEWS;
_RTC rtcTEWS2;

char bufferusb[100] = "buffer USB \n"; //reserved for print buffer usb
char bufferiridium[100] = "buffer IRIDIUM \n"; //reserved for print buffer usb
char buffersdcard[100] = "buffer SDCARD \n"; //reserved for print buffer sdcard
char bufferakustik[100] = "buffer AKUSTIK \n"; //reserved for print buffer usb
char buffergps[100] = "buffer GPS \n"; //reserved for print buffer usb
char bufferdata6bit[100] = "buffer \n"; //reserved for print buffer usb
char buffercrcdata[100] = "buffer \n"; //reserved for print buffer usb
char bufferiridiumdata[100] = "buffer \n"; //reserved for print buffer usb

uint32_t threshold;
uint8_t poweracoustic;
uint8_t rateacoustic;
uint32_t periodsampling;
uint32_t periodsenddata;
uint32_t periodsendloc;
uint32_t old_periodsendloc;
uint32_t periodsendbat;
uint32_t timebacknormal;

uint8_t mode = BUOY; //default
uint8_t debug = ACTIVE;
uint8_t debugbt;
uint8_t mulai = 0;
uint8_t idOBU;
uint8_t idBUOY;
uint8_t idCBT;
uint8_t operationmode;
uint8_t countertsunami;
uint8_t calculationstatus;
uint8_t sendmodeiridium = TEXT;
uint8_t tandalat;
int latx;
float latitude;
uint8_t tandalng;
int lngx;
float longitude;
uint32_t distancedrifting;
uint8_t ledstatus;
uint8_t debugbpr;
uint8_t debugakustik;
uint8_t debugiridium;
uint8_t debuggps;
uint8_t debugbluetooth;
uint8_t debuguni;
uint8_t syncgps;
uint8_t AccelerometerStatus;
uint8_t kalibrasibat;
uint32_t accth;
uint32_t acccounter;

uint8_t flagminute;
uint8_t flaghour;

uint8_t flagBPRtimeOut;

uint32_t acccounterstatus;
uint32_t timebacknormalstatus;
uint8_t accstreamstatus;
uint8_t synctimertc;
uint8_t minbat;
uint8_t hourbat;
uint8_t minloc;
uint8_t hourloc;
uint8_t periodmin;
uint8_t driftcalc;
uint8_t leakstatus;

uint8_t statuswaitresponsebpr;
uint8_t counterwaitbpr;
uint8_t counterwaitbprflag;
uint8_t AISstatus;

uint8_t regVal;
float rtcTemp;

uint32_t PeriodSampling = 15;
uint32_t CounterSamplingOBU = 0;
uint32_t CounterSamplingBUOY = 0;
uint32_t CounterSendOBU = 0;
uint32_t CounterSendBUOY = 0;
uint32_t CounterSendBUOYBat = 0;

uint8_t settime = 1;
uint8_t timer2keeper = 0;
uint8_t counterTA;

char printbuf[400] = "print buffer \n";
_RTC rtcTEWS;
RTC_TimeTypeDef stimeststuctureget;
RTC_DateTypeDef sdateststuctureget;
RTC_TimeTypeDef getTimeRTC;
RTC_DateTypeDef getDateRTC;
uint32_t volt;
uint8_t alarm_now;

uint8_t receiveOBU;

uint8_t signaliridium;
uint8_t ds_wakeup;

uint8_t nbtry = 0;
uint8_t statusbuoy = 0;
uint8_t versionfirmware = 3;
uint8_t iridiumqueuenumber = 0;
uint8_t iridium11counter = 0;
uint8_t iridiumsleepcounter = 0;
uint8_t autosleepcounter = 0;

extern IRIDIUM_t IRIDIUM;
extern DATAENGINEERING_t DATAENGINEERING;
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_DMA_Init(void);
static void MX_I2C1_Init(void);
static void MX_I2C2_Init(void);
static void MX_I2C3_Init(void);
static void MX_IWDG_Init(void);
static void MX_LPUART1_UART_Init(void);
static void MX_UART4_Init(void);
static void MX_USART1_UART_Init(void);
static void MX_USART2_UART_Init(void);
static void MX_RTC_Init(void);
static void MX_I2C4_Init(void);
void StartOperationTask(void *argument);
void StartIridiumTask(void *argument);
void StartGpsTask(void *argument);
void StartUsbTask(void *argument);
void StartSendIridiumTask(void *argument);
void StartWatchdogTask(void *argument);
void StartIridiumrecvTask(void *argument);

static void MX_NVIC_Init(void);
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{
  /* USER CODE BEGIN 1 */

  /* USER CODE END 1 */

  /* MCU Configuration--------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();

  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* Configure the system clock */
  SystemClock_Config();

  /* USER CODE BEGIN SysInit */

  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_DMA_Init();
  MX_I2C1_Init();
  MX_I2C2_Init();
  MX_I2C3_Init();
  MX_IWDG_Init();
  MX_LPUART1_UART_Init();
  MX_UART4_Init();
  MX_USART1_UART_Init();
  MX_USART2_UART_Init();
  MX_RTC_Init();
  MX_I2C4_Init();

  /* Initialize interrupts */
  MX_NVIC_Init();
  /* USER CODE BEGIN 2 */

  /* USER CODE END 2 */

  /* Init scheduler */
  osKernelInitialize();

  /* USER CODE BEGIN RTOS_MUTEX */
  /* add mutexes, ... */
  /* USER CODE END RTOS_MUTEX */

  /* Create the semaphores(s) */
  /* creation of rtcBinarySem */
  rtcBinarySemHandle = osSemaphoreNew(1, 1, &rtcBinarySem_attributes);

  /* creation of iridiumrecvBinarySem */
  iridiumrecvBinarySemHandle = osSemaphoreNew(1, 1, &iridiumrecvBinarySem_attributes);

  /* USER CODE BEGIN RTOS_SEMAPHORES */
  /* add semaphores, ... */
  /* USER CODE END RTOS_SEMAPHORES */

  /* USER CODE BEGIN RTOS_TIMERS */
  /* start timers, add new ones, ... */
  /* USER CODE END RTOS_TIMERS */

  /* Create the queue(s) */
  /* creation of iridiumQueue */
  iridiumQueueHandle = osMessageQueueNew (30, 50, &iridiumQueue_attributes);

  /* creation of usbQueue */
  usbQueueHandle = osMessageQueueNew (5, 50, &usbQueue_attributes);

  /* creation of gpsQueue */
  gpsQueueHandle = osMessageQueueNew (30, 100, &gpsQueue_attributes);

  /* creation of iridiumsendQueue */
  iridiumsendQueueHandle = osMessageQueueNew (70, 50, &iridiumsendQueue_attributes);

  /* creation of iridiumrecvQueue */
  iridiumrecvQueueHandle = osMessageQueueNew (10, 50, &iridiumrecvQueue_attributes);

  /* creation of crcdataQueue */
  crcdataQueueHandle = osMessageQueueNew (20, 50, &crcdataQueue_attributes);

  /* creation of sizedatairidiumQueue */
  sizedatairidiumQueueHandle = osMessageQueueNew (70, sizeof(uint16_t), &sizedatairidiumQueue_attributes);

  /* creation of sizedatadecodeQueue */
  sizedatadecodeQueueHandle = osMessageQueueNew (20, sizeof(uint16_t), &sizedatadecodeQueue_attributes);

  /* USER CODE BEGIN RTOS_QUEUES */
  /* add queues, ... */
  /* USER CODE END RTOS_QUEUES */

  /* Create the thread(s) */
  /* creation of operationTask */
  operationTaskHandle = osThreadNew(StartOperationTask, NULL, &operationTask_attributes);

  /* creation of iridiumTask */
  iridiumTaskHandle = osThreadNew(StartIridiumTask, NULL, &iridiumTask_attributes);

  /* creation of gpsTask */
  gpsTaskHandle = osThreadNew(StartGpsTask, NULL, &gpsTask_attributes);

  /* creation of usbTask */
  usbTaskHandle = osThreadNew(StartUsbTask, NULL, &usbTask_attributes);

  /* creation of sendiridiumTask */
  sendiridiumTaskHandle = osThreadNew(StartSendIridiumTask, NULL, &sendiridiumTask_attributes);

  /* creation of watchdogTask */
  watchdogTaskHandle = osThreadNew(StartWatchdogTask, NULL, &watchdogTask_attributes);

  /* creation of iridiumrecvTask */
  iridiumrecvTaskHandle = osThreadNew(StartIridiumrecvTask, NULL, &iridiumrecvTask_attributes);

  /* USER CODE BEGIN RTOS_THREADS */
  /* add threads, ... */
  /* USER CODE END RTOS_THREADS */

  /* USER CODE BEGIN RTOS_EVENTS */
  /* add events, ... */
  /* USER CODE END RTOS_EVENTS */

  /* Start scheduler */
  osKernelStart();

  /* We should never get here as control is now taken by the scheduler */
  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
  }
  /* USER CODE END 3 */
}

/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};
  RCC_PeriphCLKInitTypeDef PeriphClkInit = {0};

  /** Configure LSE Drive Capability
  */
  HAL_PWR_EnableBkUpAccess();
  __HAL_RCC_LSEDRIVE_CONFIG(RCC_LSEDRIVE_LOW);
  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_LSI|RCC_OSCILLATORTYPE_HSE
                              |RCC_OSCILLATORTYPE_LSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.LSEState = RCC_LSE_ON;
  RCC_OscInitStruct.LSIState = RCC_LSI_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLM = 1;
  RCC_OscInitStruct.PLL.PLLN = 20;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV2;
  RCC_OscInitStruct.PLL.PLLQ = RCC_PLLQ_DIV2;
  RCC_OscInitStruct.PLL.PLLR = RCC_PLLR_DIV2;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }
  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_4) != HAL_OK)
  {
    Error_Handler();
  }
  PeriphClkInit.PeriphClockSelection = RCC_PERIPHCLK_RTC|RCC_PERIPHCLK_USART1
                              |RCC_PERIPHCLK_USART2|RCC_PERIPHCLK_UART4
                              |RCC_PERIPHCLK_LPUART1|RCC_PERIPHCLK_I2C1
                              |RCC_PERIPHCLK_I2C2|RCC_PERIPHCLK_I2C3
                              |RCC_PERIPHCLK_I2C4;
  PeriphClkInit.Usart1ClockSelection = RCC_USART1CLKSOURCE_PCLK2;
  PeriphClkInit.Usart2ClockSelection = RCC_USART2CLKSOURCE_PCLK1;
  PeriphClkInit.Uart4ClockSelection = RCC_UART4CLKSOURCE_PCLK1;
  PeriphClkInit.Lpuart1ClockSelection = RCC_LPUART1CLKSOURCE_PCLK1;
  PeriphClkInit.I2c1ClockSelection = RCC_I2C1CLKSOURCE_PCLK1;
  PeriphClkInit.I2c2ClockSelection = RCC_I2C2CLKSOURCE_PCLK1;
  PeriphClkInit.I2c3ClockSelection = RCC_I2C3CLKSOURCE_PCLK1;
  PeriphClkInit.I2c4ClockSelection = RCC_I2C4CLKSOURCE_PCLK1;
  PeriphClkInit.RTCClockSelection = RCC_RTCCLKSOURCE_LSE;
  if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInit) != HAL_OK)
  {
    Error_Handler();
  }
  /** Configure the main internal regulator output voltage
  */
  if (HAL_PWREx_ControlVoltageScaling(PWR_REGULATOR_VOLTAGE_SCALE1) != HAL_OK)
  {
    Error_Handler();
  }
}

/**
  * @brief NVIC Configuration.
  * @retval None
  */
static void MX_NVIC_Init(void)
{
  /* LPUART1_IRQn interrupt configuration */
  HAL_NVIC_SetPriority(LPUART1_IRQn, 5, 0);
  HAL_NVIC_EnableIRQ(LPUART1_IRQn);
  /* UART4_IRQn interrupt configuration */
  HAL_NVIC_SetPriority(UART4_IRQn, 5, 0);
  HAL_NVIC_EnableIRQ(UART4_IRQn);
  /* USART2_IRQn interrupt configuration */
  HAL_NVIC_SetPriority(USART2_IRQn, 5, 0);
  HAL_NVIC_EnableIRQ(USART2_IRQn);
}

/**
  * @brief I2C1 Initialization Function
  * @param None
  * @retval None
  */
static void MX_I2C1_Init(void)
{

  /* USER CODE BEGIN I2C1_Init 0 */

  /* USER CODE END I2C1_Init 0 */

  /* USER CODE BEGIN I2C1_Init 1 */

  /* USER CODE END I2C1_Init 1 */
  hi2c1.Instance = I2C1;
  hi2c1.Init.Timing = 0x10909CEC;
  hi2c1.Init.OwnAddress1 = 0;
  hi2c1.Init.AddressingMode = I2C_ADDRESSINGMODE_7BIT;
  hi2c1.Init.DualAddressMode = I2C_DUALADDRESS_DISABLE;
  hi2c1.Init.OwnAddress2 = 0;
  hi2c1.Init.OwnAddress2Masks = I2C_OA2_NOMASK;
  hi2c1.Init.GeneralCallMode = I2C_GENERALCALL_DISABLE;
  hi2c1.Init.NoStretchMode = I2C_NOSTRETCH_DISABLE;
  if (HAL_I2C_Init(&hi2c1) != HAL_OK)
  {
    Error_Handler();
  }
  /** Configure Analogue filter
  */
  if (HAL_I2CEx_ConfigAnalogFilter(&hi2c1, I2C_ANALOGFILTER_ENABLE) != HAL_OK)
  {
    Error_Handler();
  }
  /** Configure Digital filter
  */
  if (HAL_I2CEx_ConfigDigitalFilter(&hi2c1, 0) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN I2C1_Init 2 */

  /* USER CODE END I2C1_Init 2 */

}

/**
  * @brief I2C2 Initialization Function
  * @param None
  * @retval None
  */
static void MX_I2C2_Init(void)
{

  /* USER CODE BEGIN I2C2_Init 0 */

  /* USER CODE END I2C2_Init 0 */

  /* USER CODE BEGIN I2C2_Init 1 */

  /* USER CODE END I2C2_Init 1 */
  hi2c2.Instance = I2C2;
  hi2c2.Init.Timing = 0x10909CEC;
  hi2c2.Init.OwnAddress1 = 0;
  hi2c2.Init.AddressingMode = I2C_ADDRESSINGMODE_7BIT;
  hi2c2.Init.DualAddressMode = I2C_DUALADDRESS_DISABLE;
  hi2c2.Init.OwnAddress2 = 0;
  hi2c2.Init.OwnAddress2Masks = I2C_OA2_NOMASK;
  hi2c2.Init.GeneralCallMode = I2C_GENERALCALL_DISABLE;
  hi2c2.Init.NoStretchMode = I2C_NOSTRETCH_DISABLE;
  if (HAL_I2C_Init(&hi2c2) != HAL_OK)
  {
    Error_Handler();
  }
  /** Configure Analogue filter
  */
  if (HAL_I2CEx_ConfigAnalogFilter(&hi2c2, I2C_ANALOGFILTER_ENABLE) != HAL_OK)
  {
    Error_Handler();
  }
  /** Configure Digital filter
  */
  if (HAL_I2CEx_ConfigDigitalFilter(&hi2c2, 0) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN I2C2_Init 2 */

  /* USER CODE END I2C2_Init 2 */

}

/**
  * @brief I2C3 Initialization Function
  * @param None
  * @retval None
  */
static void MX_I2C3_Init(void)
{

  /* USER CODE BEGIN I2C3_Init 0 */

  /* USER CODE END I2C3_Init 0 */

  /* USER CODE BEGIN I2C3_Init 1 */

  /* USER CODE END I2C3_Init 1 */
  hi2c3.Instance = I2C3;
  hi2c3.Init.Timing = 0x00702991;
  hi2c3.Init.OwnAddress1 = 0;
  hi2c3.Init.AddressingMode = I2C_ADDRESSINGMODE_7BIT;
  hi2c3.Init.DualAddressMode = I2C_DUALADDRESS_DISABLE;
  hi2c3.Init.OwnAddress2 = 0;
  hi2c3.Init.OwnAddress2Masks = I2C_OA2_NOMASK;
  hi2c3.Init.GeneralCallMode = I2C_GENERALCALL_DISABLE;
  hi2c3.Init.NoStretchMode = I2C_NOSTRETCH_DISABLE;
  if (HAL_I2C_Init(&hi2c3) != HAL_OK)
  {
    Error_Handler();
  }
  /** Configure Analogue filter
  */
  if (HAL_I2CEx_ConfigAnalogFilter(&hi2c3, I2C_ANALOGFILTER_ENABLE) != HAL_OK)
  {
    Error_Handler();
  }
  /** Configure Digital filter
  */
  if (HAL_I2CEx_ConfigDigitalFilter(&hi2c3, 0) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN I2C3_Init 2 */

  /* USER CODE END I2C3_Init 2 */

}

/**
  * @brief I2C4 Initialization Function
  * @param None
  * @retval None
  */
static void MX_I2C4_Init(void)
{

  /* USER CODE BEGIN I2C4_Init 0 */

  /* USER CODE END I2C4_Init 0 */

  /* USER CODE BEGIN I2C4_Init 1 */

  /* USER CODE END I2C4_Init 1 */
  hi2c4.Instance = I2C4;
  hi2c4.Init.Timing = 0x10909CEC;
  hi2c4.Init.OwnAddress1 = 0;
  hi2c4.Init.AddressingMode = I2C_ADDRESSINGMODE_7BIT;
  hi2c4.Init.DualAddressMode = I2C_DUALADDRESS_DISABLE;
  hi2c4.Init.OwnAddress2 = 0;
  hi2c4.Init.OwnAddress2Masks = I2C_OA2_NOMASK;
  hi2c4.Init.GeneralCallMode = I2C_GENERALCALL_DISABLE;
  hi2c4.Init.NoStretchMode = I2C_NOSTRETCH_DISABLE;
  if (HAL_I2C_Init(&hi2c4) != HAL_OK)
  {
    Error_Handler();
  }
  /** Configure Analogue filter
  */
  if (HAL_I2CEx_ConfigAnalogFilter(&hi2c4, I2C_ANALOGFILTER_ENABLE) != HAL_OK)
  {
    Error_Handler();
  }
  /** Configure Digital filter
  */
  if (HAL_I2CEx_ConfigDigitalFilter(&hi2c4, 0) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN I2C4_Init 2 */

  /* USER CODE END I2C4_Init 2 */

}

/**
  * @brief IWDG Initialization Function
  * @param None
  * @retval None
  */
static void MX_IWDG_Init(void)
{

  /* USER CODE BEGIN IWDG_Init 0 */

  /* USER CODE END IWDG_Init 0 */

  /* USER CODE BEGIN IWDG_Init 1 */

  /* USER CODE END IWDG_Init 1 */
  hiwdg.Instance = IWDG;
  hiwdg.Init.Prescaler = IWDG_PRESCALER_256;
  hiwdg.Init.Window = 4095;
  hiwdg.Init.Reload = 4095;
  if (HAL_IWDG_Init(&hiwdg) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN IWDG_Init 2 */

  /* USER CODE END IWDG_Init 2 */

}

/**
  * @brief LPUART1 Initialization Function
  * @param None
  * @retval None
  */
static void MX_LPUART1_UART_Init(void)
{

  /* USER CODE BEGIN LPUART1_Init 0 */

  /* USER CODE END LPUART1_Init 0 */

  /* USER CODE BEGIN LPUART1_Init 1 */

  /* USER CODE END LPUART1_Init 1 */
  hlpuart1.Instance = LPUART1;
  hlpuart1.Init.BaudRate = 115200;
  hlpuart1.Init.WordLength = UART_WORDLENGTH_8B;
  hlpuart1.Init.StopBits = UART_STOPBITS_1;
  hlpuart1.Init.Parity = UART_PARITY_NONE;
  hlpuart1.Init.Mode = UART_MODE_TX_RX;
  hlpuart1.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  hlpuart1.Init.OneBitSampling = UART_ONE_BIT_SAMPLE_DISABLE;
  hlpuart1.AdvancedInit.AdvFeatureInit = UART_ADVFEATURE_NO_INIT;
  if (HAL_UART_Init(&hlpuart1) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN LPUART1_Init 2 */

  /* USER CODE END LPUART1_Init 2 */

}

/**
  * @brief UART4 Initialization Function
  * @param None
  * @retval None
  */
static void MX_UART4_Init(void)
{

  /* USER CODE BEGIN UART4_Init 0 */

  /* USER CODE END UART4_Init 0 */

  /* USER CODE BEGIN UART4_Init 1 */

  /* USER CODE END UART4_Init 1 */
  huart4.Instance = UART4;
  huart4.Init.BaudRate = 57600;
  huart4.Init.WordLength = UART_WORDLENGTH_8B;
  huart4.Init.StopBits = UART_STOPBITS_1;
  huart4.Init.Parity = UART_PARITY_NONE;
  huart4.Init.Mode = UART_MODE_TX_RX;
  huart4.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  huart4.Init.OverSampling = UART_OVERSAMPLING_16;
  huart4.Init.OneBitSampling = UART_ONE_BIT_SAMPLE_DISABLE;
  huart4.AdvancedInit.AdvFeatureInit = UART_ADVFEATURE_NO_INIT;
  if (HAL_UART_Init(&huart4) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN UART4_Init 2 */

  /* USER CODE END UART4_Init 2 */

}

/**
  * @brief USART1 Initialization Function
  * @param None
  * @retval None
  */
static void MX_USART1_UART_Init(void)
{

  /* USER CODE BEGIN USART1_Init 0 */

  /* USER CODE END USART1_Init 0 */

  /* USER CODE BEGIN USART1_Init 1 */

  /* USER CODE END USART1_Init 1 */
  huart1.Instance = USART1;
  huart1.Init.BaudRate = 19200;
  huart1.Init.WordLength = UART_WORDLENGTH_8B;
  huart1.Init.StopBits = UART_STOPBITS_1;
  huart1.Init.Parity = UART_PARITY_NONE;
  huart1.Init.Mode = UART_MODE_TX_RX;
  huart1.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  huart1.Init.OverSampling = UART_OVERSAMPLING_16;
  huart1.Init.OneBitSampling = UART_ONE_BIT_SAMPLE_DISABLE;
  huart1.AdvancedInit.AdvFeatureInit = UART_ADVFEATURE_NO_INIT;
  if (HAL_UART_Init(&huart1) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN USART1_Init 2 */

  /* USER CODE END USART1_Init 2 */

}

/**
  * @brief USART2 Initialization Function
  * @param None
  * @retval None
  */
static void MX_USART2_UART_Init(void)
{

  /* USER CODE BEGIN USART2_Init 0 */

  /* USER CODE END USART2_Init 0 */

  /* USER CODE BEGIN USART2_Init 1 */

  /* USER CODE END USART2_Init 1 */
  huart2.Instance = USART2;
  huart2.Init.BaudRate = 9600;
  huart2.Init.WordLength = UART_WORDLENGTH_8B;
  huart2.Init.StopBits = UART_STOPBITS_1;
  huart2.Init.Parity = UART_PARITY_NONE;
  huart2.Init.Mode = UART_MODE_TX_RX;
  huart2.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  huart2.Init.OverSampling = UART_OVERSAMPLING_16;
  huart2.Init.OneBitSampling = UART_ONE_BIT_SAMPLE_DISABLE;
  huart2.AdvancedInit.AdvFeatureInit = UART_ADVFEATURE_NO_INIT;
  if (HAL_UART_Init(&huart2) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN USART2_Init 2 */

  /* USER CODE END USART2_Init 2 */

}

/**
  * @brief RTC Initialization Function
  * @param None
  * @retval None
  */
static void MX_RTC_Init(void)
{

  /* USER CODE BEGIN RTC_Init 0 */

  /* USER CODE END RTC_Init 0 */

  RTC_TimeTypeDef sTime = {0};
  RTC_DateTypeDef sDate = {0};
  RTC_AlarmTypeDef sAlarm = {0};

  /* USER CODE BEGIN RTC_Init 1 */

  /* USER CODE END RTC_Init 1 */
  /** Initialize RTC Only
  */
  hrtc.Instance = RTC;
  hrtc.Init.HourFormat = RTC_HOURFORMAT_24;
  hrtc.Init.AsynchPrediv = 127;
  hrtc.Init.SynchPrediv = 255;
  hrtc.Init.OutPut = RTC_OUTPUT_DISABLE;
  hrtc.Init.OutPutRemap = RTC_OUTPUT_REMAP_NONE;
  hrtc.Init.OutPutPolarity = RTC_OUTPUT_POLARITY_HIGH;
  hrtc.Init.OutPutType = RTC_OUTPUT_TYPE_OPENDRAIN;
  if (HAL_RTC_Init(&hrtc) != HAL_OK)
  {
    Error_Handler();
  }

  /* USER CODE BEGIN Check_RTC_BKUP */

  /* USER CODE END Check_RTC_BKUP */

  /** Initialize RTC and set the Time and Date
  */
  sTime.Hours = 0;
  sTime.Minutes = 0;
  sTime.Seconds = 0;
  sTime.DayLightSaving = RTC_DAYLIGHTSAVING_NONE;
  sTime.StoreOperation = RTC_STOREOPERATION_RESET;
  if (HAL_RTC_SetTime(&hrtc, &sTime, RTC_FORMAT_BIN) != HAL_OK)
  {
    Error_Handler();
  }
  sDate.WeekDay = RTC_WEEKDAY_MONDAY;
  sDate.Month = RTC_MONTH_JANUARY;
  sDate.Date = 1;
  sDate.Year = 0;

  if (HAL_RTC_SetDate(&hrtc, &sDate, RTC_FORMAT_BIN) != HAL_OK)
  {
    Error_Handler();
  }
  /** Enable the Alarm A
  */
  sAlarm.AlarmTime.Hours = 0;
  sAlarm.AlarmTime.Minutes = 0;
  sAlarm.AlarmTime.Seconds = 0;
  sAlarm.AlarmTime.SubSeconds = 0;
  sAlarm.AlarmTime.DayLightSaving = RTC_DAYLIGHTSAVING_NONE;
  sAlarm.AlarmTime.StoreOperation = RTC_STOREOPERATION_RESET;
  sAlarm.AlarmMask = RTC_ALARMMASK_DATEWEEKDAY|RTC_ALARMMASK_HOURS
                              |RTC_ALARMMASK_MINUTES;
  sAlarm.AlarmSubSecondMask = RTC_ALARMSUBSECONDMASK_ALL;
  sAlarm.AlarmDateWeekDaySel = RTC_ALARMDATEWEEKDAYSEL_DATE;
  sAlarm.AlarmDateWeekDay = 1;
  sAlarm.Alarm = RTC_ALARM_A;
  if (HAL_RTC_SetAlarm_IT(&hrtc, &sAlarm, RTC_FORMAT_BIN) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN RTC_Init 2 */

  /* USER CODE END RTC_Init 2 */

}

/**
  * Enable DMA controller clock
  */
static void MX_DMA_Init(void)
{

  /* DMA controller clock enable */
  __HAL_RCC_DMA2_CLK_ENABLE();
  __HAL_RCC_DMA1_CLK_ENABLE();

  /* DMA interrupt init */
  /* DMA1_Channel5_IRQn interrupt configuration */
  HAL_NVIC_SetPriority(DMA1_Channel5_IRQn, 5, 0);
  HAL_NVIC_EnableIRQ(DMA1_Channel5_IRQn);
  /* DMA1_Channel6_IRQn interrupt configuration */
  HAL_NVIC_SetPriority(DMA1_Channel6_IRQn, 5, 0);
  HAL_NVIC_EnableIRQ(DMA1_Channel6_IRQn);
  /* DMA2_Channel5_IRQn interrupt configuration */
  HAL_NVIC_SetPriority(DMA2_Channel5_IRQn, 5, 0);
  HAL_NVIC_EnableIRQ(DMA2_Channel5_IRQn);
  /* DMA2_Channel7_IRQn interrupt configuration */
  HAL_NVIC_SetPriority(DMA2_Channel7_IRQn, 5, 0);
  HAL_NVIC_EnableIRQ(DMA2_Channel7_IRQn);

}

/**
  * @brief GPIO Initialization Function
  * @param None
  * @retval None
  */
static void MX_GPIO_Init(void)
{
  GPIO_InitTypeDef GPIO_InitStruct = {0};

  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOE_CLK_ENABLE();
  __HAL_RCC_GPIOC_CLK_ENABLE();
  __HAL_RCC_GPIOH_CLK_ENABLE();
  __HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_GPIOB_CLK_ENABLE();
  __HAL_RCC_GPIOD_CLK_ENABLE();

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOA, BT_CON_Pin|SWT_TRIG_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(LED_GPIO_Port, LED_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPS_RESET_GPIO_Port, GPS_RESET_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(ON_OFF_GPIO_Port, ON_OFF_Pin, GPIO_PIN_SET);

  /*Configure GPIO pins : PE2 PE3 PE4 PE5
                           PE6 PE9 PE10 PE11
                           PE12 PE13 PE14 PE15
                           PE0 */
  GPIO_InitStruct.Pin = GPIO_PIN_2|GPIO_PIN_3|GPIO_PIN_4|GPIO_PIN_5
                          |GPIO_PIN_6|GPIO_PIN_9|GPIO_PIN_10|GPIO_PIN_11
                          |GPIO_PIN_12|GPIO_PIN_13|GPIO_PIN_14|GPIO_PIN_15
                          |GPIO_PIN_0;
  GPIO_InitStruct.Mode = GPIO_MODE_ANALOG;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(GPIOE, &GPIO_InitStruct);

  /*Configure GPIO pins : PC13 PC2 PC3 PC4
                           PC5 PC6 PC7 PC8
                           PC9 PC10 PC11 PC12 */
  GPIO_InitStruct.Pin = GPIO_PIN_13|GPIO_PIN_2|GPIO_PIN_3|GPIO_PIN_4
                          |GPIO_PIN_5|GPIO_PIN_6|GPIO_PIN_7|GPIO_PIN_8
                          |GPIO_PIN_9|GPIO_PIN_10|GPIO_PIN_11|GPIO_PIN_12;
  GPIO_InitStruct.Mode = GPIO_MODE_ANALOG;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);

  /*Configure GPIO pins : BT_CON_Pin SWT_TRIG_Pin */
  GPIO_InitStruct.Pin = BT_CON_Pin|SWT_TRIG_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  /*Configure GPIO pins : PA3 PA4 PA5 PA8
                           PA9 PA10 PA11 PA12
                           PA15 */
  GPIO_InitStruct.Pin = GPIO_PIN_3|GPIO_PIN_4|GPIO_PIN_5|GPIO_PIN_8
                          |GPIO_PIN_9|GPIO_PIN_10|GPIO_PIN_11|GPIO_PIN_12
                          |GPIO_PIN_15;
  GPIO_InitStruct.Mode = GPIO_MODE_ANALOG;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  /*Configure GPIO pin : ALARM_IN_Pin */
  GPIO_InitStruct.Pin = ALARM_IN_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(ALARM_IN_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pins : PB0 PB1 PB2 PB12
                           PB3 PB4 */
  GPIO_InitStruct.Pin = GPIO_PIN_0|GPIO_PIN_1|GPIO_PIN_2|GPIO_PIN_12
                          |GPIO_PIN_3|GPIO_PIN_4;
  GPIO_InitStruct.Mode = GPIO_MODE_ANALOG;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

  /*Configure GPIO pin : LEAKAGE_Pin */
  GPIO_InitStruct.Pin = LEAKAGE_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(LEAKAGE_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pin : LED_Pin */
  GPIO_InitStruct.Pin = LED_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(LED_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pins : DS_WAKEUP_Pin NETWORKINT_Pin */
  GPIO_InitStruct.Pin = DS_WAKEUP_Pin|NETWORKINT_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

  /*Configure GPIO pin : RTC_RST_Pin */
  GPIO_InitStruct.Pin = RTC_RST_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(RTC_RST_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pins : PD9 PD10 PD11 PD14
                           PD15 PD0 PD1 PD2
                           PD3 */
  GPIO_InitStruct.Pin = GPIO_PIN_9|GPIO_PIN_10|GPIO_PIN_11|GPIO_PIN_14
                          |GPIO_PIN_15|GPIO_PIN_0|GPIO_PIN_1|GPIO_PIN_2
                          |GPIO_PIN_3;
  GPIO_InitStruct.Mode = GPIO_MODE_ANALOG;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(GPIOD, &GPIO_InitStruct);

  /*Configure GPIO pin : GPS_PPS_Pin */
  GPIO_InitStruct.Pin = GPS_PPS_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_IT_RISING;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(GPS_PPS_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pin : GPS_RESET_Pin */
  GPIO_InitStruct.Pin = GPS_RESET_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_PULLUP;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPS_RESET_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pin : PH3 */
  GPIO_InitStruct.Pin = GPIO_PIN_3;
  GPIO_InitStruct.Mode = GPIO_MODE_ANALOG;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(GPIOH, &GPIO_InitStruct);

  /*Configure GPIO pin : ON_OFF_Pin */
  GPIO_InitStruct.Pin = ON_OFF_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_PULLDOWN;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
  HAL_GPIO_Init(ON_OFF_GPIO_Port, &GPIO_InitStruct);

}

/* USER CODE BEGIN 4 */

void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart) {

	if (huart->Instance == USART1) //iridium
	{
		setgreenLED();
		IRIDIUM_Callback();
		resetgreenLED();
		//HAL_GPIO_TogglePin(GPIOE,LED_Pin);
	}
	if (huart->Instance == LPUART1) //USB
	{
		USB_Callback();
	}
	if (huart->Instance == UART4) //BLUETOOTH
	{
		BT_Callback();
	}
	if (huart->Instance == USART2) //GPS
	{
		GPS_Callback();
	}
}

void HAL_UART_ErrorCallback(UART_HandleTypeDef *huart){
	//
	if (huart->Instance == USART1){
		IRIDIUM.rxIndex = 0;
		HAL_UART_Receive_DMA(&_IRIDIUM_USART, &IRIDIUM.rxTmp, 1);
	}

	if (huart->Instance == LPUART1){
		USB_Init();
	}

	if (huart->Instance == USART2){
		GPS_Init();
	}

	if (huart->Instance == UART4){
		BT_Init();
	}
}

void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin) {
	if (GPIO_Pin == GPIO_PIN_1) {
		//
	}

	usb_universal_printf("WAKEUP FROM EXTI \r\n");

	if(GPIO_Pin == DS_WAKEUP_Pin){
		SystemClock_Config();
		HAL_ResumeTick();
		usb_universal_printf("WAKEUP FROM EXTI \r\n");
		HAL_PWR_DisableSleepOnExit();
	}
}

void HAL_RTCEx_WakeUpTimerEventCallback(RTC_HandleTypeDef *hrtc)
{
	//SystemClock_Config();
	//HAL_ResumeTick();
	//usb_universal_printf("WAKEUP FROM RTC\n NOW GOING IN STOP MODE AGAIN \r\n");

}

void HAL_RTC_AlarmAEventCallback(RTC_HandleTypeDef *hrtc) { //alarm A pada mode buoy dibuat per menit
	osSemaphoreRelease(rtcBinarySemHandle);
	//usb_universal_printf("Timer A On \r\n");
}

void HAL_RTC_AlarmBEventCallback(RTC_HandleTypeDef *hrtc) { //alarm A pada mode buoy dibuat per menit
}

void setNextalarmA(uint8_t second) {

	RTC_AlarmTypeDef sAlarm = { 0 };

	sAlarm.AlarmTime.Hours = 0;
	sAlarm.AlarmTime.Minutes = 0;
	sAlarm.AlarmTime.Seconds = second;
	sAlarm.AlarmTime.SubSeconds = 0;
	sAlarm.AlarmTime.DayLightSaving = RTC_DAYLIGHTSAVING_NONE;
	sAlarm.AlarmTime.StoreOperation = RTC_STOREOPERATION_RESET;
	sAlarm.AlarmMask = RTC_ALARMMASK_DATEWEEKDAY | RTC_ALARMMASK_HOURS
			| RTC_ALARMMASK_MINUTES;
	sAlarm.AlarmSubSecondMask = RTC_ALARMSUBSECONDMASK_ALL;
	sAlarm.AlarmDateWeekDaySel = RTC_ALARMDATEWEEKDAYSEL_DATE;
	sAlarm.AlarmDateWeekDay = 1;
	sAlarm.Alarm = RTC_ALARM_A;
	if (HAL_RTC_SetAlarm_IT(&hrtc, &sAlarm, RTC_FORMAT_BIN) != HAL_OK) {
		Error_Handler();
	}
}

/* USER CODE END 4 */

/* USER CODE BEGIN Header_StartOperationTask */
/**
  * @brief  Function implementing the operationTask thread.
  * @param  argument: Not used
  * @retval None
  */
/* USER CODE END Header_StartOperationTask */
void StartOperationTask(void *argument)
{
  /* USER CODE BEGIN 5 */
	RTC_TimeTypeDef RTCMicro;
	RTC_DateTypeDef RTCDateMicro;
	uint32_t boardtemp;

	InitEEPROM();
	usb_universal_printf("Inisialisasi GPS Tracker \r\n");
	PrintParameter();

	DS3231_GetTime(&rtcTEWS);
	//DS3231_GetTime_B(&rtcTEWS2);
	RTCDateMicro.Year = rtcTEWS.Year;
	RTCDateMicro.Month = rtcTEWS.Month;
	RTCDateMicro.Date = rtcTEWS.Date;
	RTCDateMicro.WeekDay = rtcTEWS.DaysOfWeek;
	HAL_RTC_SetDate(&hrtc, &RTCDateMicro, RTC_FORMAT_BIN);
	RTCMicro.Hours = rtcTEWS.Hour - 1;
	RTCMicro.Minutes = rtcTEWS.Min;
	RTCMicro.Seconds = rtcTEWS.Sec;
	HAL_RTC_SetTime(&hrtc, &RTCMicro, RTC_FORMAT_BIN);
	HAL_IWDG_Refresh(&hiwdg); //refresh counter clock watchdog timer
	osSemaphoreAcquire(rtcBinarySemHandle, 100);

	HAL_GPIO_WritePin(BT_CON_GPIO_Port, BT_CON_Pin, GPIO_PIN_RESET); // bluetooth dalam mode data

	LTC4151_write(LTC4151_I2C_ADDRESS, LTC4151_CONTROL_REG,LTC4151_CONTINUOUS_MODE);

	// Hidupkan Power FET kemudian reset SWQ Alarm
	HAL_GPIO_WritePin(SWT_TRIG_GPIO_Port, SWT_TRIG_Pin, GPIO_PIN_SET);
	osDelay(50);
	//DS3231_ResetAlarm1_B();
	//DS3231_ResetAlarm2_B();
	//osDelay(50);

	//0=tanggal 1=jam 2=menit 3=detik 4=sekali per detik
	uint8_t _Mode = 1;
	uint8_t _Date = rtcTEWS2.Date;
	uint8_t _Hour = rtcTEWS2.Hour;
	uint8_t _Min  = rtcTEWS2.Min;
	uint8_t _Sec  = rtcTEWS2.Sec;
	if(operationmode == NORMALOPS){
		// set alarm jam 21:17:00 UTC
		_Hour = 21;
		_Min  = 17;
		usb_universal_printf("MODE NORMAL, Alarm Set : %.2d:%.2d:%.2d UTC\r\n",_Hour,_Min,_Sec);
		DS3231_SetAlarm1_B(_Mode, _Date, _Hour, _Min, _Sec);
	}

	if(operationmode == DRIFTINGOPS){
		// set alarm per 1 jam
		alarm_now = _Hour + 1;
		if(alarm_now > 23){
			alarm_now = alarm_now - 24;
		}
		usb_universal_printf("MODE DRIFTING, Alarm Set : %.2d\r\n",alarm_now);
		DS3231_SetAlarm1_B(_Mode, _Date, alarm_now, _Min, _Sec);
	}

	if(operationmode == HOLDOPS){
		// set alarm per 3 jam
		alarm_now = _Hour + periodsendloc;
		if(alarm_now > 23){
			alarm_now = alarm_now - 24;
		}
		usb_universal_printf("MODE TESTING, Alarm Set : %.2d\r\n",alarm_now);
		DS3231_SetAlarm1_B(_Mode, _Date, alarm_now, _Min, _Sec);
	}

	//setgreenLED();

	  char *unit = "K";
	  double lat1 = -6.328759757512878;
	  double lon1 = 106.46331631536947;
	  double lat2 = -6.337566634988436;
	  double lon2 = 106.44137635216677;
	  float aa = distance(lat1,lon1,lat2,lon2,*unit);
	  double bb = CalculateDistance(lat1, lon1,lat2,lon2);
	  usb_universal_printf("Jarak: %f %s\r\n",aa,unit);
	  usb_universal_printf("Jarak: %f %s\r\n",bb,unit);

  /* Infinite loop */
  for(;;)
  {
	  generalTaskTimer = osKernelGetTickCount();

	  ds_wakeup  = HAL_GPIO_ReadPin(DS_WAKEUP_GPIO_Port, DS_WAKEUP_Pin);
	  uint8_t ds_wakeup2  = HAL_GPIO_ReadPin(ALARM_IN_GPIO_Port, ALARM_IN_Pin);

	  if (osSemaphoreGetCount(rtcBinarySemHandle) > 0) {
		  osSemaphoreAcquire(rtcBinarySemHandle, 100);

		  //HAL_GPIO_TogglePin(GPIOE,LED_Pin);
		  DataEngBuoy.leakstatus = HAL_GPIO_ReadPin(LEAKAGE_GPIO_Port,LEAKAGE_Pin);
		  DS3231_ReadTemperature(&boardtemp);
		  DataEngBuoy.tempBuoy = boardtemp;
		  DataEngBuoy.batBuoy = getvoltage();

		  usb_universal_printf("DS WakeUp Status: %d \r\n",ds_wakeup);
		  usb_universal_printf("DS2 WakeUp Status: %d \r\n",ds_wakeup2);
		  //usb_universal_printf("Leak Status: %d \r\n",DataEngBuoy.leakstatus);

		  DS3231_GetTime(&rtcTEWS);
		  HAL_RTC_GetTime(&hrtc, &RTCMicro, RTC_FORMAT_BIN); // Get Time
		  HAL_RTC_GetDate(&hrtc, &RTCDateMicro, RTC_FORMAT_BIN); //Get Date
		  //RTC auto sycron dengan GPS
		  if (rtcTEWS.Hour == 20 && rtcTEWS.Min == 0 && GPS.validstatus == 1) {
			  _RTC setTime;
			  setTime.Hour = GPS.hour;
			  setTime.Min = GPS.minute;
			  setTime.Sec = GPS.second;
			  setTime.Date = rtcTEWS.Date;
			  setTime.Month = rtcTEWS.Month;
			  setTime.Year = rtcTEWS.Year;
			  setTime.DaysOfWeek = rtcTEWS.DaysOfWeek;
			  DS3231_SetTime(&setTime);
			  DS3231_SetTime_B(&setTime);

			  RTCMicro.Hours = GPS.hour;
			  RTCMicro.Minutes = GPS.minute;
			  RTCMicro.Seconds = GPS.second;
			  HAL_RTC_SetTime(&hrtc, &RTCMicro, RTC_FORMAT_BIN);
		  }

		  //RTC Internal auto sycron dengan RTC External
		  if ((rtcTEWS.Sec - RTCMicro.Seconds) > 0) {
			  RTCMicro.Hours = rtcTEWS.Hour - 1;
			  RTCMicro.Minutes = rtcTEWS.Min;
			  RTCMicro.Seconds = rtcTEWS.Sec;
			  HAL_RTC_SetTime(&hrtc, &RTCMicro, RTC_FORMAT_BIN); // Set Time
			  RTCDateMicro.Year = rtcTEWS.Year;
			  RTCDateMicro.Month = rtcTEWS.Month;
			  RTCDateMicro.Date = rtcTEWS.Date;
			  RTCDateMicro.WeekDay = rtcTEWS.DaysOfWeek;
			  HAL_RTC_SetDate(&hrtc, &RTCDateMicro, RTC_FORMAT_BIN);
		  }

		  usb_universal_printf("TIME DS: %.2d:%.2d:%.2d %.2d/%.2d/20%.2d \r\n",
				  rtcTEWS.Hour, rtcTEWS.Min, rtcTEWS.Sec, rtcTEWS.Date,
				  rtcTEWS.Month, rtcTEWS.Year);
		  usb_universal_printf("TIME GPS: %.2d:%.2d:%.2d %.2d/%.2d/20%.2d \r\n",GPS.hour,GPS.minute,GPS.second,GPS.date,GPS.month,GPS.year);

		  usb_universal_printf(
				  "Iridium Queue: %d || Iridium Status: %d || Iridium Status Receive: %d || Status Kirim: %d\r\n",
				  iridiumqueuenumber, IRIDIUM.status, IRIDIUM.statusreceive, statusSendGPS);

		  // Iridium SLEEP MODE delay 2 menit
		  if (IRIDIUM.status == 0 && IRIDIUM.statusreceive == 0 && statusSendGPS == 1) {
			  if (iridiumsleepcounter < 2) {
				  iridiumsleepcounter++;
			  } else {
				  iridiumsleepcounter = 0;
				  statusSendGPS = 2;
				  IRIDIUM.status = Idle;
				  IRIDIUM.statusreceive = IDLERECEIVE;
				  iridium_printf("AT*F\r\n");
			  }
		  }

			// auto sleep setelah on 10 menit
			if (autosleepcounter < 10) {
				autosleepcounter++;
			} else {
				autosleepcounter = 0;
				usb_printf("Device life of more than 10 minutes.\r\n");
				statusSendGPS = 2;
				IRIDIUM.status = Idle;
				IRIDIUM.statusreceive = IDLERECEIVE;
				iridium_printf("AT*F\r\n");
			}

		  if (IRIDIUM.status == SBDRT) {
			  IRIDIUM.statusreceive = IDLERECEIVE;
			  IRIDIUM.status = SBDD1;
		  }

		  if (IRIDIUM.status == SBDWB) {
			  IRIDIUM.statusreceive = IDLERECEIVE;
			  IRIDIUM.status = SBDD0;
		  }

		  if (IRIDIUM.status == WaitSBDD01 && IRIDIUM.statusreceive == IDLERECEIVE) {
			  if (iridium11counter < 5) {
				  iridium11counter++;
			  } else {
				  IRIDIUM.status = IDLE;
				  iridium11counter = 0;
			  }
		  }

		  if (IRIDIUM.status == WaitresponseSBDIX) {
			  IRIDIUM.status = Waitsignal;
		  }

//		  if (rtcTEWS.Hour == 21 && rtcTEWS.Min == 30) {
//			  EncodeTEWSEngBuoy(6);
//		  }

		  //HAL_SuspendTick();
		  //HAL_PWR_EnableSleepOnExit ();
		  //HAL_PWR_EnterSLEEPMode(PWR_MAINREGULATOR_ON, PWR_SLEEPENTRY_WFI);

		  //HAL_PWREx_EnterSTOP2Mode(PWR_STOPENTRY_WFI);
	  }
    osDelay(1);
  }
  /* USER CODE END 5 */
}

/* USER CODE BEGIN Header_StartIridiumTask */
/**
* @brief Function implementing the iridiumTask thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_StartIridiumTask */
void StartIridiumTask(void *argument)
{
  /* USER CODE BEGIN StartIridiumTask */
	osStatus_t status;
	uint8_t queuenumber;
	IRIDIUM_Init();
  /* Infinite loop */
  for(;;)
  {
		iridiumTaskTimer = osKernelGetTickCount();

		queuenumber = osMessageQueueGetCount(iridiumQueueHandle);
		if (queuenumber > 0) {
			memset(bufferiridium, 0, sizeof(bufferiridium));
			status = osMessageQueueGet(iridiumQueueHandle, &bufferiridium, NULL,
					1000);   // wait for message
			if (status == osOK) {
				//proses
				IRIDIUM_Process(bufferiridium);
			}
		}
    osDelay(1);
  }
  /* USER CODE END StartIridiumTask */
}

/* USER CODE BEGIN Header_StartGpsTask */
/**
* @brief Function implementing the gpsTask thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_StartGpsTask */
void StartGpsTask(void *argument)
{
  /* USER CODE BEGIN StartGpsTask */
	osStatus_t status;
	uint8_t queuenumber;
	prevGPSTaskTimer1 = 0;
	prevGPSTaskTimer2 = 0;

	GPS_Init();
	//usb_universal_printf("initialize GPS\r\n");
  /* Infinite loop */
  for(;;)
  {
		gpsTaskTimer = osKernelGetTickCount();

		queuenumber = osMessageQueueGetCount(gpsQueueHandle);
		if (queuenumber > 0) {
			memset(buffergps, 0, sizeof(buffergps));
			status = osMessageQueueGet(gpsQueueHandle, &buffergps, NULL, 1000); // wait for message
			if (status == osOK) {
				//proses
				//usb_universal_printf("Proses GPS\r\n");
				//usb_universal_printf("GPS= %s\r\n",buffergps);
				GPS_Process(buffergps);
			}
		}

//		//Kirim data posisi dengan iridium
//		if ((gpsTaskTimer - prevGPSTaskTimer1) > (60000 * periodsendloc)) {
//			prevGPSTaskTimer1 = gpsTaskTimer;
//			//prevGPSTaskTimer2 = gpsTaskTimer;
//			statusSendGPS = 1;
//			// kirim posisi otomatis
//			EncodeTEWSLocBuoy(0, 14);
//			usb_printf("Kirim Posisi.\r\n");
//		}

		if (statusSendGPS == 0) {
			prevGPSTaskTimer1 = gpsTaskTimer;
			//prevGPSTaskTimer2 = gpsTaskTimer;
			//statusSendGPS = 1;
			// kirim posisi otomatis
			EncodeTEWSLocBuoy(0, 14);
			usb_printf("Kirim Posisi.\r\n");
		}

		//Baca data Iridium dari RDS
//		if ((gpsTaskTimer - prevGPSTaskTimer2) > (60000 * periodsenddata) && statusSendGPS == 0 && IRIDIUM.status == 0 && IRIDIUM.statusreceive == 0) {
//			//prevGPSTaskTimer1 = gpsTaskTimer;
//			prevGPSTaskTimer2 = gpsTaskTimer;
//			statusSendGPS = 1;
//			HAL_GPIO_WritePin(ON_OFF_GPIO_Port, ON_OFF_Pin, GPIO_PIN_SET);
//			osDelay(1000);
//			IRIDIUM.status = SBDIXA;
//			usb_printf("Baca Perintah dari RDS.\r\n");
//		}

    osDelay(1);
  }
  /* USER CODE END StartGpsTask */
}

/* USER CODE BEGIN Header_StartUsbTask */
/**
* @brief Function implementing the usbTask thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_StartUsbTask */
void StartUsbTask(void *argument)
{
  /* USER CODE BEGIN StartUsbTask */
	osStatus_t status;
	uint8_t queuenumber;
	BT_Init();
	USB_Init();
  /* Infinite loop */
  for(;;)
  {
		usbTaskTimer = osKernelGetTickCount();

		queuenumber = osMessageQueueGetCount(usbQueueHandle);
		if (queuenumber > 0) {
			memset(bufferusb, 0, sizeof(bufferusb));
			status = osMessageQueueGet(usbQueueHandle, &bufferusb, NULL, 1000); // wait for message
			if (status == osOK) {
				USB_Process(bufferusb);
			}
		}
    osDelay(1);
  }
  /* USER CODE END StartUsbTask */
}

/* USER CODE BEGIN Header_StartSendIridiumTask */
/**
* @brief Function implementing the sendiridiumTask thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_StartSendIridiumTask */
void StartSendIridiumTask(void *argument)
{
  /* USER CODE BEGIN StartSendIridiumTask */
	uint8_t queuenumber;
	uint16_t sizedatairidium;
	osStatus_t status;
  /* Infinite loop */
  for(;;)
  {
		datahandleTaskTimer = osKernelGetTickCount();

		queuenumber = osMessageQueueGetCount(iridiumsendQueueHandle);
		iridiumqueuenumber = queuenumber;
		if (queuenumber > 0 && IRIDIUM.status == Idle) {

			//usb_printf("Iridium Queue: %d || Iridium Status: %d || Iridium Status Receive: %d \r\n",
			//		iridiumqueuenumber, IRIDIUM.status, IRIDIUM.statusreceive);

			//osDelay(100);
			IRIDIUM.status = ProsesSend;
			memset(bufferiridiumdata, 0, sizeof(bufferiridiumdata));
			osMessageQueueGet(sizedatairidiumQueueHandle, &sizedatairidium,
			NULL, 1000);
			status = osMessageQueueGet(iridiumsendQueueHandle,
					&bufferiridiumdata, NULL, 1000); // wait for message
			if (status == osOK) {
				//WeekUp Iridium
				HAL_GPIO_WritePin(ON_OFF_GPIO_Port, ON_OFF_Pin, GPIO_PIN_SET);
				statusSendGPS = 1;
				//GPS WeekUp
				GPS_Init();
				//proses sending data to iridium
				usb_printf("Iridium Status data handle out : %d \r\n", IRIDIUM.status);
				senddata(bufferiridiumdata, sizedatairidium);
				//sendtext(bufferiridiumdata);
			}
		}
    osDelay(1);
  }
  /* USER CODE END StartSendIridiumTask */
}

/* USER CODE BEGIN Header_StartWatchdogTask */
/**
* @brief Function implementing the watchdogTask thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_StartWatchdogTask */
void StartWatchdogTask(void *argument)
{
  /* USER CODE BEGIN StartWatchdogTask */
	uint8_t ResetOnHang;
	uint8_t flaggeneral = 0;
	uint8_t flagiridium = 0;
	uint8_t flaggps = 0;
	uint8_t flagusb = 0;
	uint8_t flagdatahandle = 0;
	uint8_t flagiridiumrecv = 0;
  /* Infinite loop */
  for(;;)
  {
		ResetOnHang = 0;

		if ((osKernelGetTickCount() - generalTaskTimer) > 5000) {
			if (flaggeneral == 0) {
				flaggeneral = 1;
				usb_universal_printf("Hang General \r\n");
			}
			ResetOnHang = 1;
		} else
			flaggeneral = 0;

		if ((osKernelGetTickCount() - iridiumTaskTimer) > 5000) {
			if (flagiridium == 0) {
				flagiridium = 1;
				usb_universal_printf("Hang Iridium \r\n");
			}
			ResetOnHang = 1;
		} else
			flagiridium = 0;

		if ((osKernelGetTickCount() - gpsTaskTimer) > 5000) {
			if (flaggps == 0) {
				flaggps = 1;
				usb_universal_printf("Hang GPS \r\n");
			}
			ResetOnHang = 1;
		} else
			flaggps = 0;

		if ((osKernelGetTickCount() - usbTaskTimer) > 5000) {
			if (flagusb == 0) {
				flagusb = 1;
				usb_universal_printf("Hang USB \r\n");
			}
			ResetOnHang = 1;
		} else
			flagusb = 0;

		if ((osKernelGetTickCount() - datahandleTaskTimer) > 5000) {
			if (flagdatahandle == 0) {
				flagdatahandle = 1;
				usb_universal_printf("Hang data handle \r\n");
			}
			ResetOnHang = 1;
		} else
			flagdatahandle = 0;

		if ((osKernelGetTickCount() - iridiumrecvTaskTimer) > 5000) {
			if (flagiridiumrecv == 0) {
				flagiridiumrecv = 1;
				usb_universal_printf("Hang Iridium Receive \r\n");
			}
			ResetOnHang = 1;
		} else
			flagiridiumrecv = 0;

		if (!ResetOnHang) {
			if (HAL_IWDG_Refresh(&hiwdg) != HAL_OK) {
				// /* Refresh Error*/
				Error_Handler();
			}
		}
		//else NVIC_SystemReset();
    osDelay(1);
  }
  /* USER CODE END StartWatchdogTask */
}

/* USER CODE BEGIN Header_StartIridiumrecvTask */
/**
* @brief Function implementing the iridiumrecvTask thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_StartIridiumrecvTask */
void StartIridiumrecvTask(void *argument)
{
  /* USER CODE BEGIN StartIridiumrecvTask */
	uint8_t queuenumber;
	osStatus_t status;
	char bufferrecv[100];
	osSemaphoreAcquire(iridiumrecvBinarySemHandle, 100);

  /* Infinite loop */
  for(;;)
  {
		iridiumrecvTaskTimer = osKernelGetTickCount();

		IRIDIUM_Loop();

		queuenumber = osMessageQueueGetCount(iridiumrecvQueueHandle);
		if (queuenumber > 0) {
			memset(bufferrecv, 0, sizeof(bufferrecv));
			status = osMessageQueueGet(iridiumrecvQueueHandle, &bufferrecv,
			NULL, 1000);   // wait for message
			if (status == osOK) {
				ProcessReceiveIridium(bufferrecv);
			}
		}
    osDelay(1);
  }
  /* USER CODE END StartIridiumrecvTask */
}

 /**
  * @brief  Period elapsed callback in non blocking mode
  * @note   This function is called  when TIM6 interrupt took place, inside
  * HAL_TIM_IRQHandler(). It makes a direct call to HAL_IncTick() to increment
  * a global variable "uwTick" used as application time base.
  * @param  htim : TIM handle
  * @retval None
  */
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
  /* USER CODE BEGIN Callback 0 */

  /* USER CODE END Callback 0 */
  if (htim->Instance == TIM6) {
    HAL_IncTick();
  }
  /* USER CODE BEGIN Callback 1 */

  /* USER CODE END Callback 1 */
}

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */
  //__disable_irq();
	//usb_universal_printf("Error UART1 > IRIDIUM>>>\r\n");
  while (1)
  {
  }
  /* USER CODE END Error_Handler_Debug */
}

#ifdef  USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t *file, uint32_t line)
{
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
