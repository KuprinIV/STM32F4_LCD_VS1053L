/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2020 STMicroelectronics.
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
#include "fatfs.h"
#include "app_touchgfx.h"
#include "usb_device.h"
#include "usb_device.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include <stdio.h>
#include <math.h>
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
#define REFRESH_COUNT       ((uint32_t)0x056A)   /* SDRAM refresh counter (90MHz SDRAM clock)*/
#define SDRAM_MEMORY_WIDTH            FMC_SDRAM_MEM_BUS_WIDTH_16

#define SDCLOCK_PERIOD                   FMC_SDRAM_CLOCK_PERIOD_3

#define SDRAM_TIMEOUT     ((uint32_t)0xFFFF)

#define SDRAM_MODEREG_BURST_LENGTH_1             ((uint16_t)0x0000)
#define SDRAM_MODEREG_BURST_LENGTH_2             ((uint16_t)0x0001)
#define SDRAM_MODEREG_BURST_LENGTH_4             ((uint16_t)0x0002)
#define SDRAM_MODEREG_BURST_LENGTH_8             ((uint16_t)0x0004)
#define SDRAM_MODEREG_BURST_TYPE_SEQUENTIAL      ((uint16_t)0x0000)
#define SDRAM_MODEREG_BURST_TYPE_INTERLEAVED     ((uint16_t)0x0008)
#define SDRAM_MODEREG_CAS_LATENCY_2              ((uint16_t)0x0020)
#define SDRAM_MODEREG_CAS_LATENCY_3              ((uint16_t)0x0030)
#define SDRAM_MODEREG_OPERATING_MODE_STANDARD    ((uint16_t)0x0000)
#define SDRAM_MODEREG_WRITEBURST_MODE_PROGRAMMED ((uint16_t)0x0000)
#define SDRAM_MODEREG_WRITEBURST_MODE_SINGLE     ((uint16_t)0x0200)

#define NCoef 4
#define DCgain 2

#define HDAT1_LOW_LIMIT (uint16_t)255
#define HDAT1_UP_LIMIT (uint16_t)896

#define RECORDING_BUFFER_SIZE 16384
#define RECORDING_BUFFER_MASK (RECORDING_BUFFER_SIZE-1)
/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
CRC_HandleTypeDef hcrc;

DMA2D_HandleTypeDef hdma2d;

I2C_HandleTypeDef hi2c1;

LTDC_HandleTypeDef hltdc;

SD_HandleTypeDef hsd;
DMA_HandleTypeDef hdma_sdio_rx;
DMA_HandleTypeDef hdma_sdio_tx;

SPI_HandleTypeDef hspi2;
SPI_HandleTypeDef hspi5;

TIM_HandleTypeDef htim2;

SDRAM_HandleTypeDef hsdram1;

/* Definitions for GUI_Task */
osThreadId_t GUI_TaskHandle;
const osThreadAttr_t GUI_Task_attributes = {
  .name = "GUI_Task",
  .stack_size = 3072 * 4,
  .priority = (osPriority_t) osPriorityNormal,
};
/* Definitions for defaultTask */
osThreadId_t defaultTaskHandle;
const osThreadAttr_t defaultTask_attributes = {
  .name = "defaultTask",
  .stack_size = 128 * 4,
  .priority = (osPriority_t) osPriorityNormal,
};
/* Definitions for readFileTask */
osThreadId_t readFileTaskHandle;
const osThreadAttr_t readFileTask_attributes = {
  .name = "readFileTask",
  .stack_size = 1024 * 4,
  .priority = (osPriority_t) osPriorityNormal,
};
/* Definitions for writeFileTask */
osThreadId_t writeFileTaskHandle;
const osThreadAttr_t writeFileTask_attributes = {
  .name = "writeFileTask",
  .stack_size = 4096 * 4,
  .priority = (osPriority_t) osPriorityNormal,
};
/* Definitions for messageQueue */
osMessageQueueId_t messageQueueHandle;
const osMessageQueueAttr_t messageQueue_attributes = {
  .name = "messageQueue"
};
/* Definitions for dataReadySemaphore */
osSemaphoreId_t dataReadySemaphoreHandle;
const osSemaphoreAttr_t dataReadySemaphore_attributes = {
  .name = "dataReadySemaphore"
};
/* USER CODE BEGIN PV */
osMessageQId messageQueue;
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_DMA_Init(void);
static void MX_LTDC_Init(void);
static void MX_FMC_Init(void);
static void MX_SPI5_Init(void);
static void MX_I2C1_Init(void);
static void MX_DMA2D_Init(void);
static void MX_TIM2_Init(void);
static void MX_SDIO_SD_Init(void);
static void MX_SPI2_Init(void);
static void MX_CRC_Init(void);
void TouchGFX_Task(void *argument);
void StartDefaultTask(void *argument);
void StartReadFileTask(void *argument);
void StartWriteFileTask(void *argument);

/* USER CODE BEGIN PFP */
static void SDRAM_Initialization_Sequence(SDRAM_HandleTypeDef *hsdram, FMC_SDRAM_CommandTypeDef *Command);
static UINT out_stream (const BYTE *p,UINT btf);

static void switchRecordingBuffer(void);
static void switchStimulusBuffer(void);
static int16_t* getRecordingBackBuffer(void);
/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */
commandStruct fileCmd = {IDLE, STATUS_IDLE, REWIND_NOT_ALLOWED, 0, 0, 0, 0, 0, 0, 0, 0, {0, 0}, 0, 0, 0};
volatile recordingParams recording_params = {0, 0, 0, 0, 0};
char currentFilePath[255] = {0};

// define WAV header for 48000 Hz 16-bit stereo PCM
uint8_t WAV_header_48kHz_16bit_stereo[44] = {
0x52, 0x49, 0x46, 0x46, 0xFF, 0xFF, 0xFF, 0xFF, 0x57, 0x41, 0x56, 0x45, 0x66, 0x6D, 0x74, 0x20,
0x10, 0x00, 0x00, 0x00, 0x01, 0x00, 0x02, 0x00, 0x80, 0xBB, 0x00, 0x00, 0x00, 0xEE, 0x02, 0x00,
0x04, 0x00, 0x10, 0x00, 0x64, 0x61, 0x74, 0x61, 0xFF, 0xFF, 0xFF, 0xFF};

uint8_t recording_wav_header[44] = {0};

// PCM data buffer
__attribute__ ((section (".TouchGFX_Framebuffer"))) uint8_t PCM_data_Buffer1[19200];
__attribute__ ((section (".TouchGFX_Framebuffer"))) uint8_t PCM_data_Buffer2[19200];
__attribute__ ((section (".TouchGFX_Framebuffer"))) int16_t PCM_recording_Buffer1[RECORDING_BUFFER_SIZE];
__attribute__ ((section (".TouchGFX_Framebuffer"))) int16_t PCM_recording_Buffer2[RECORDING_BUFFER_SIZE];
volatile int16_t* currentRecordingBuffer;
uint8_t* currentStimulusBuffer;
volatile uint16_t currentIndex = 0;
volatile uint8_t currentBuffer = 0;
volatile uint8_t currentDataBuffer = 0;

uint8_t volume = 0, wav_files_count = 0;
volatile uint8_t isFileStopped = 0, isFileOpened = 0, isRecording = 0;
volatile uint32_t idleHookCounter = 0, stackOverflowConter = 0;

volatile uint32_t delayCntr1 = 0, delayCntr2 = 0, delayCntr = 0;
volatile uint16_t hdat1_eval = 0;

FIL fil;

char recordingFilesPath[] = "0:/Recordings";
char currentFileRecordingName[255] = {0};
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
  MX_LTDC_Init();
  MX_FMC_Init();
  MX_SPI5_Init();
  MX_I2C1_Init();
  MX_DMA2D_Init();
  MX_TIM2_Init();
  MX_SDIO_SD_Init();
  MX_SPI2_Init();
  MX_CRC_Init();
  MX_FATFS_Init();
  MX_TouchGFX_Init();
  /* USER CODE BEGIN 2 */
  MX_USB_DEVICE_Init();
  HAL_TIM_PWM_Start(&htim2, TIM_CHANNEL_1);
  // init pause buffer data
  for(int i = 0; i < 19200; i++){
	  PCM_data_Buffer2[i] = 0;
  }
  /* USER CODE END 2 */

  /* Init scheduler */
  osKernelInitialize();

  /* USER CODE BEGIN RTOS_MUTEX */
  /* add mutexes, ... */
  /* USER CODE END RTOS_MUTEX */

  /* Create the semaphores(s) */
  /* creation of dataReadySemaphore */
  dataReadySemaphoreHandle = osSemaphoreNew(1, 1, &dataReadySemaphore_attributes);

  /* USER CODE BEGIN RTOS_SEMAPHORES */
  /* add semaphores, ... */
  /* USER CODE END RTOS_SEMAPHORES */

  /* USER CODE BEGIN RTOS_TIMERS */
  /* start timers, add new ones, ... */
  /* USER CODE END RTOS_TIMERS */

  /* Create the queue(s) */
  /* creation of messageQueue */
  messageQueueHandle = osMessageQueueNew (1, sizeof(int), &messageQueue_attributes);

  /* USER CODE BEGIN RTOS_QUEUES */
  /* add queues, ... */

  /* USER CODE END RTOS_QUEUES */

  /* Create the thread(s) */
  /* creation of GUI_Task */
  GUI_TaskHandle = osThreadNew(TouchGFX_Task, NULL, &GUI_Task_attributes);

  /* creation of defaultTask */
  defaultTaskHandle = osThreadNew(StartDefaultTask, NULL, &defaultTask_attributes);

  /* creation of readFileTask */
  readFileTaskHandle = osThreadNew(StartReadFileTask, NULL, &readFileTask_attributes);

  /* creation of writeFileTask */
  writeFileTaskHandle = osThreadNew(StartWriteFileTask, NULL, &writeFileTask_attributes);

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

  /** Configure the main internal regulator output voltage
  */
  __HAL_RCC_PWR_CLK_ENABLE();
  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);
  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLM = 25;
  RCC_OscInitStruct.PLL.PLLN = 336;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV2;
  RCC_OscInitStruct.PLL.PLLQ = 7;
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
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV4;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV2;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_5) != HAL_OK)
  {
    Error_Handler();
  }
}

/**
  * @brief CRC Initialization Function
  * @param None
  * @retval None
  */
static void MX_CRC_Init(void)
{

  /* USER CODE BEGIN CRC_Init 0 */

  /* USER CODE END CRC_Init 0 */

  /* USER CODE BEGIN CRC_Init 1 */

  /* USER CODE END CRC_Init 1 */
  hcrc.Instance = CRC;
  if (HAL_CRC_Init(&hcrc) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN CRC_Init 2 */

  /* USER CODE END CRC_Init 2 */

}

/**
  * @brief DMA2D Initialization Function
  * @param None
  * @retval None
  */
static void MX_DMA2D_Init(void)
{

  /* USER CODE BEGIN DMA2D_Init 0 */

  /* USER CODE END DMA2D_Init 0 */

  /* USER CODE BEGIN DMA2D_Init 1 */

  /* USER CODE END DMA2D_Init 1 */
  hdma2d.Instance = DMA2D;
  hdma2d.Init.Mode = DMA2D_M2M;
  hdma2d.Init.ColorMode = DMA2D_OUTPUT_RGB888;
  hdma2d.Init.OutputOffset = 0;
  hdma2d.LayerCfg[1].InputOffset = 0;
  hdma2d.LayerCfg[1].InputColorMode = DMA2D_INPUT_RGB888;
  hdma2d.LayerCfg[1].AlphaMode = DMA2D_NO_MODIF_ALPHA;
  hdma2d.LayerCfg[1].InputAlpha = 0;
  if (HAL_DMA2D_Init(&hdma2d) != HAL_OK)
  {
    Error_Handler();
  }
  if (HAL_DMA2D_ConfigLayer(&hdma2d, 1) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN DMA2D_Init 2 */

  /* USER CODE END DMA2D_Init 2 */

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
  hi2c1.Init.ClockSpeed = 100000;
  hi2c1.Init.DutyCycle = I2C_DUTYCYCLE_2;
  hi2c1.Init.OwnAddress1 = 224;
  hi2c1.Init.AddressingMode = I2C_ADDRESSINGMODE_7BIT;
  hi2c1.Init.DualAddressMode = I2C_DUALADDRESS_DISABLE;
  hi2c1.Init.OwnAddress2 = 0;
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
  * @brief LTDC Initialization Function
  * @param None
  * @retval None
  */
static void MX_LTDC_Init(void)
{

  /* USER CODE BEGIN LTDC_Init 0 */

  /* USER CODE END LTDC_Init 0 */

  LTDC_LayerCfgTypeDef pLayerCfg = {0};

  /* USER CODE BEGIN LTDC_Init 1 */

  /* USER CODE END LTDC_Init 1 */
  hltdc.Instance = LTDC;
  hltdc.Init.HSPolarity = LTDC_HSPOLARITY_AL;
  hltdc.Init.VSPolarity = LTDC_VSPOLARITY_AL;
  hltdc.Init.DEPolarity = LTDC_DEPOLARITY_AL;
  hltdc.Init.PCPolarity = LTDC_PCPOLARITY_IIPC;
  hltdc.Init.HorizontalSync = 19;
  hltdc.Init.VerticalSync = 19;
  hltdc.Init.AccumulatedHBP = 24;
  hltdc.Init.AccumulatedVBP = 21;
  hltdc.Init.AccumulatedActiveW = 504;
  hltdc.Init.AccumulatedActiveH = 293;
  hltdc.Init.TotalWidth = 509;
  hltdc.Init.TotalHeigh = 295;
  hltdc.Init.Backcolor.Blue = 0;
  hltdc.Init.Backcolor.Green = 0;
  hltdc.Init.Backcolor.Red = 0;
  if (HAL_LTDC_Init(&hltdc) != HAL_OK)
  {
    Error_Handler();
  }
  pLayerCfg.WindowX0 = 0;
  pLayerCfg.WindowX1 = 480;
  pLayerCfg.WindowY0 = 0;
  pLayerCfg.WindowY1 = 272;
  pLayerCfg.PixelFormat = LTDC_PIXEL_FORMAT_RGB888;
  pLayerCfg.Alpha = 255;
  pLayerCfg.Alpha0 = 0;
  pLayerCfg.BlendingFactor1 = LTDC_BLENDING_FACTOR1_CA;
  pLayerCfg.BlendingFactor2 = LTDC_BLENDING_FACTOR2_CA;
  pLayerCfg.FBStartAdress = 0x0;
  pLayerCfg.ImageWidth = 480;
  pLayerCfg.ImageHeight = 272;
  pLayerCfg.Backcolor.Blue = 0;
  pLayerCfg.Backcolor.Green = 0;
  pLayerCfg.Backcolor.Red = 0;
  if (HAL_LTDC_ConfigLayer(&hltdc, &pLayerCfg, 0) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN LTDC_Init 2 */

  /* USER CODE END LTDC_Init 2 */

}

/**
  * @brief SDIO Initialization Function
  * @param None
  * @retval None
  */
static void MX_SDIO_SD_Init(void)
{

  /* USER CODE BEGIN SDIO_Init 0 */

  /* USER CODE END SDIO_Init 0 */

  /* USER CODE BEGIN SDIO_Init 1 */

  /* USER CODE END SDIO_Init 1 */
  hsd.Instance = SDIO;
  hsd.Init.ClockEdge = SDIO_CLOCK_EDGE_RISING;
  hsd.Init.ClockBypass = SDIO_CLOCK_BYPASS_DISABLE;
  hsd.Init.ClockPowerSave = SDIO_CLOCK_POWER_SAVE_DISABLE;
  hsd.Init.BusWide = SDIO_BUS_WIDE_1B;
  hsd.Init.HardwareFlowControl = SDIO_HARDWARE_FLOW_CONTROL_DISABLE;
  hsd.Init.ClockDiv = 4;
  /* USER CODE BEGIN SDIO_Init 2 */

  /* USER CODE END SDIO_Init 2 */

}

/**
  * @brief SPI2 Initialization Function
  * @param None
  * @retval None
  */
static void MX_SPI2_Init(void)
{

  /* USER CODE BEGIN SPI2_Init 0 */

  /* USER CODE END SPI2_Init 0 */

  /* USER CODE BEGIN SPI2_Init 1 */

  /* USER CODE END SPI2_Init 1 */
  /* SPI2 parameter configuration*/
  hspi2.Instance = SPI2;
  hspi2.Init.Mode = SPI_MODE_MASTER;
  hspi2.Init.Direction = SPI_DIRECTION_2LINES;
  hspi2.Init.DataSize = SPI_DATASIZE_8BIT;
  hspi2.Init.CLKPolarity = SPI_POLARITY_LOW;
  hspi2.Init.CLKPhase = SPI_PHASE_1EDGE;
  hspi2.Init.NSS = SPI_NSS_SOFT;
  hspi2.Init.BaudRatePrescaler = SPI_BAUDRATEPRESCALER_16;
  hspi2.Init.FirstBit = SPI_FIRSTBIT_MSB;
  hspi2.Init.TIMode = SPI_TIMODE_DISABLE;
  hspi2.Init.CRCCalculation = SPI_CRCCALCULATION_DISABLE;
  hspi2.Init.CRCPolynomial = 10;
  if (HAL_SPI_Init(&hspi2) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN SPI2_Init 2 */

  /* USER CODE END SPI2_Init 2 */

}

/**
  * @brief SPI5 Initialization Function
  * @param None
  * @retval None
  */
static void MX_SPI5_Init(void)
{

  /* USER CODE BEGIN SPI5_Init 0 */

  /* USER CODE END SPI5_Init 0 */

  /* USER CODE BEGIN SPI5_Init 1 */

  /* USER CODE END SPI5_Init 1 */
  /* SPI5 parameter configuration*/
  hspi5.Instance = SPI5;
  hspi5.Init.Mode = SPI_MODE_MASTER;
  hspi5.Init.Direction = SPI_DIRECTION_2LINES;
  hspi5.Init.DataSize = SPI_DATASIZE_8BIT;
  hspi5.Init.CLKPolarity = SPI_POLARITY_LOW;
  hspi5.Init.CLKPhase = SPI_PHASE_1EDGE;
  hspi5.Init.NSS = SPI_NSS_SOFT;
  hspi5.Init.BaudRatePrescaler = SPI_BAUDRATEPRESCALER_2;
  hspi5.Init.FirstBit = SPI_FIRSTBIT_MSB;
  hspi5.Init.TIMode = SPI_TIMODE_DISABLE;
  hspi5.Init.CRCCalculation = SPI_CRCCALCULATION_DISABLE;
  hspi5.Init.CRCPolynomial = 10;
  if (HAL_SPI_Init(&hspi5) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN SPI5_Init 2 */

  /* USER CODE END SPI5_Init 2 */

}

/**
  * @brief TIM2 Initialization Function
  * @param None
  * @retval None
  */
static void MX_TIM2_Init(void)
{

  /* USER CODE BEGIN TIM2_Init 0 */

  /* USER CODE END TIM2_Init 0 */

  TIM_MasterConfigTypeDef sMasterConfig = {0};
  TIM_OC_InitTypeDef sConfigOC = {0};

  /* USER CODE BEGIN TIM2_Init 1 */

  /* USER CODE END TIM2_Init 1 */
  htim2.Instance = TIM2;
  htim2.Init.Prescaler = 44;
  htim2.Init.CounterMode = TIM_COUNTERMODE_UP;
  htim2.Init.Period = 99;
  htim2.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
  htim2.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_ENABLE;
  if (HAL_TIM_PWM_Init(&htim2) != HAL_OK)
  {
    Error_Handler();
  }
  sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
  sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
  if (HAL_TIMEx_MasterConfigSynchronization(&htim2, &sMasterConfig) != HAL_OK)
  {
    Error_Handler();
  }
  sConfigOC.OCMode = TIM_OCMODE_PWM1;
  sConfigOC.Pulse = 49;
  sConfigOC.OCPolarity = TIM_OCPOLARITY_HIGH;
  sConfigOC.OCFastMode = TIM_OCFAST_DISABLE;
  if (HAL_TIM_PWM_ConfigChannel(&htim2, &sConfigOC, TIM_CHANNEL_1) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN TIM2_Init 2 */
  __HAL_RCC_TIM7_CLK_ENABLE();
  TIM7->PSC = 89;
  TIM7->ARR = 59999;
  TIM7->CR1 |= TIM_CR1_CEN;

  /* USER CODE END TIM2_Init 2 */
  HAL_TIM_MspPostInit(&htim2);

}

/**
  * Enable DMA controller clock
  */
static void MX_DMA_Init(void)
{

  /* DMA controller clock enable */
  __HAL_RCC_DMA2_CLK_ENABLE();

  /* DMA interrupt init */
  /* DMA2_Stream3_IRQn interrupt configuration */
  HAL_NVIC_SetPriority(DMA2_Stream3_IRQn, 6, 0);
  HAL_NVIC_EnableIRQ(DMA2_Stream3_IRQn);
  /* DMA2_Stream6_IRQn interrupt configuration */
  HAL_NVIC_SetPriority(DMA2_Stream6_IRQn, 6, 0);
  HAL_NVIC_EnableIRQ(DMA2_Stream6_IRQn);

}

/* FMC initialization function */
static void MX_FMC_Init(void)
{

  /* USER CODE BEGIN FMC_Init 0 */
  FMC_SDRAM_CommandTypeDef command;
  /* USER CODE END FMC_Init 0 */

  FMC_SDRAM_TimingTypeDef SdramTiming = {0};

  /* USER CODE BEGIN FMC_Init 1 */

  /* USER CODE END FMC_Init 1 */

  /** Perform the SDRAM1 memory initialization sequence
  */
  hsdram1.Instance = FMC_SDRAM_DEVICE;
  /* hsdram1.Init */
  hsdram1.Init.SDBank = FMC_SDRAM_BANK2;
  hsdram1.Init.ColumnBitsNumber = FMC_SDRAM_COLUMN_BITS_NUM_8;
  hsdram1.Init.RowBitsNumber = FMC_SDRAM_ROW_BITS_NUM_12;
  hsdram1.Init.MemoryDataWidth = FMC_SDRAM_MEM_BUS_WIDTH_16;
  hsdram1.Init.InternalBankNumber = FMC_SDRAM_INTERN_BANKS_NUM_4;
  hsdram1.Init.CASLatency = FMC_SDRAM_CAS_LATENCY_3;
  hsdram1.Init.WriteProtection = FMC_SDRAM_WRITE_PROTECTION_DISABLE;
  hsdram1.Init.SDClockPeriod = FMC_SDRAM_CLOCK_PERIOD_3;
  hsdram1.Init.ReadBurst = FMC_SDRAM_RBURST_DISABLE;
  hsdram1.Init.ReadPipeDelay = FMC_SDRAM_RPIPE_DELAY_1;
  /* SdramTiming */
  SdramTiming.LoadToActiveDelay = 2;
  SdramTiming.ExitSelfRefreshDelay = 7;
  SdramTiming.SelfRefreshTime = 4;
  SdramTiming.RowCycleDelay = 7;
  SdramTiming.WriteRecoveryTime = 3;
  SdramTiming.RPDelay = 2;
  SdramTiming.RCDDelay = 2;

  if (HAL_SDRAM_Init(&hsdram1, &SdramTiming) != HAL_OK)
  {
    Error_Handler( );
  }

  /* USER CODE BEGIN FMC_Init 2 */
  SDRAM_Initialization_Sequence(&hsdram1, &command);
  /* USER CODE END FMC_Init 2 */
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
  __HAL_RCC_GPIOI_CLK_ENABLE();
  __HAL_RCC_GPIOF_CLK_ENABLE();
  __HAL_RCC_GPIOH_CLK_ENABLE();
  __HAL_RCC_GPIOC_CLK_ENABLE();
  __HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_GPIOB_CLK_ENABLE();
  __HAL_RCC_GPIOG_CLK_ENABLE();
  __HAL_RCC_GPIOD_CLK_ENABLE();

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOF, GPIO_PIN_6, GPIO_PIN_RESET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOA, GPIO_PIN_1|GPIO_PIN_2, GPIO_PIN_RESET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOB, GPIO_PIN_12, GPIO_PIN_SET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOG, GPIO_PIN_2, GPIO_PIN_SET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOC, GPIO_PIN_6, GPIO_PIN_SET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOB, GPIO_PIN_3, GPIO_PIN_RESET);

  /*Configure GPIO pin : PF6 */
  GPIO_InitStruct.Pin = GPIO_PIN_6;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOF, &GPIO_InitStruct);

  /*Configure GPIO pins : PA1 PA2 */
  GPIO_InitStruct.Pin = GPIO_PIN_1|GPIO_PIN_2;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  /*Configure GPIO pins : PB12 PB3 */
  GPIO_InitStruct.Pin = GPIO_PIN_12|GPIO_PIN_3;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

  /*Configure GPIO pin : PD13 */
  GPIO_InitStruct.Pin = GPIO_PIN_13;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_PULLUP;
  HAL_GPIO_Init(GPIOD, &GPIO_InitStruct);

  /*Configure GPIO pin : PG2 */
  GPIO_InitStruct.Pin = GPIO_PIN_2;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOG, &GPIO_InitStruct);

  /*Configure GPIO pin : PC6 */
  GPIO_InitStruct.Pin = GPIO_PIN_6;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);

  /*Configure GPIO pin : PA15 */
  GPIO_InitStruct.Pin = GPIO_PIN_15;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_PULLUP;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  /*Configure GPIO pin : PB5 */
  GPIO_InitStruct.Pin = GPIO_PIN_5;
  GPIO_InitStruct.Mode = GPIO_MODE_IT_RISING;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

}

/* USER CODE BEGIN 4 */
static void SDRAM_Initialization_Sequence(SDRAM_HandleTypeDef *hsdram, FMC_SDRAM_CommandTypeDef *Command)
{
  __IO uint32_t tmpmrd =0;
  /* Step 3:  Configure a clock configuration enable command */
  Command->CommandMode 			 = FMC_SDRAM_CMD_CLK_ENABLE;
  Command->CommandTarget 		 = FMC_SDRAM_CMD_TARGET_BANK2;
  Command->AutoRefreshNumber 	 = 1;
  Command->ModeRegisterDefinition = 0;

  /* Send the command */
  HAL_SDRAM_SendCommand(hsdram, Command, 0x1000);

  /* Step 4: Insert 100 ms delay */
  HAL_Delay(100);

  /* Step 5: Configure a PALL (precharge all) command */
  Command->CommandMode 			 = FMC_SDRAM_CMD_PALL;
  Command->CommandTarget 	     = FMC_SDRAM_CMD_TARGET_BANK2;
  Command->AutoRefreshNumber 	 = 1;
  Command->ModeRegisterDefinition = 0;

  /* Send the command */
  HAL_SDRAM_SendCommand(hsdram, Command, 0x1000);

  /* Step 6 : Configure a Auto-Refresh command */
  Command->CommandMode 			 = FMC_SDRAM_CMD_AUTOREFRESH_MODE;
  Command->CommandTarget 		 = FMC_SDRAM_CMD_TARGET_BANK2;
  Command->AutoRefreshNumber 	 = 4;
  Command->ModeRegisterDefinition = 0;

  /* Send the command */
  HAL_SDRAM_SendCommand(hsdram, Command, 0x1000);

  /* Step 7: Program the external memory mode register */
  tmpmrd = (uint32_t)SDRAM_MODEREG_BURST_LENGTH_2          |
                     SDRAM_MODEREG_BURST_TYPE_SEQUENTIAL   |
                     SDRAM_MODEREG_CAS_LATENCY_3           |
                     SDRAM_MODEREG_OPERATING_MODE_STANDARD |
                     SDRAM_MODEREG_WRITEBURST_MODE_SINGLE;

  Command->CommandMode = FMC_SDRAM_CMD_LOAD_MODE;
  Command->CommandTarget 		 = FMC_SDRAM_CMD_TARGET_BANK2;
  Command->AutoRefreshNumber 	 = 1;
  Command->ModeRegisterDefinition = tmpmrd;

  /* Send the command */
  HAL_SDRAM_SendCommand(hsdram, Command, 0x1000);

  /* Step 8: Set the refresh rate counter */
  /* (15.62 us x Freq) - 20 */
  /* Set the device refresh counter */
  HAL_SDRAM_ProgramRefreshRate(hsdram, REFRESH_COUNT);
}

static UINT out_stream (   /* Returns number of bytes sent or stream status */
    const BYTE *p,  /* Pointer to the data block to be sent */
    UINT btf        /* >0: Transfer call (Number of bytes to be sent). 0: Sense call */
)
{
    UINT cnt = 0;


    if (btf == 0) {     /* Sense call */
        /* Return stream status (0: Busy, 1: Ready) */
        /* When once it returned ready to sense call, it must accept a byte at least */
        /* at subsequent transfer call, or f_forward will fail with FR_INT_ERR. */
        cnt = 1;
    }
    else {              /* Transfer call */
        do {    /* Repeat while there is any data to be sent and the stream is ready */
            writeData((uint8_t*)p, (btf - cnt >= 32) ? 32 : btf - cnt);
            p += (btf - cnt >= 32) ? 32 : btf - cnt;
            cnt += (btf - cnt >= 32) ? 32 : btf - cnt;
        } while (cnt < btf);
    }

    return cnt;
}

void setCurrentFilePath(char* path){
	sprintf(currentFilePath, "%s", path);
}

void sendCommand(commandStruct* cmd)
{
	fileCmd = *cmd;
}

void setRecordingParams(recordingParams* rp)
{
	recording_params = *rp;
}

commandStruct* getCommand()
{
	return &fileCmd;
}

void resetStatus(void)
{
	fileCmd.status = STATUS_IDLE;
}

void resetRewindStatus(void)
{
	fileCmd.rewindStatus = REWIND_NOT_ALLOWED;
}

void setPCMDataBuffer(uint8_t* buf)
{
	memcpy(PCM_data_Buffer1, buf, sizeof(PCM_data_Buffer1));
}

static void switchRecordingBuffer(void)
{
	if(currentBuffer == 0){
		currentRecordingBuffer = PCM_recording_Buffer2;
		currentBuffer = 1;
	}else{
		currentRecordingBuffer = PCM_recording_Buffer1;
		currentBuffer = 0;
	}
}

static void switchStimulusBuffer(void)
{
	if(currentDataBuffer == 0){
		currentStimulusBuffer = PCM_data_Buffer2;
		currentDataBuffer = 1;
	}else{
		currentStimulusBuffer = PCM_data_Buffer1;
		currentDataBuffer = 0;
	}
}

static int16_t* getRecordingBackBuffer(void)
{
	if(currentBuffer == 0){
		return PCM_recording_Buffer2;
	}else{
		return PCM_recording_Buffer1;
	}
}

int16_t iir(int16_t NewSample) {
   int16_t ACoef[NCoef+1] = {
         9236,
            0,
        -18472,
            0,
         9236
    };

    int16_t BCoef[NCoef+1] = {
        16384,
        -32568,
        17561,
        -2550,
         1177
    };

    static int32_t y[NCoef+1]; //output samples
    //Warning!!!!!! This variable should be signed (input sample width + Coefs width + 4 )-bit width to avoid saturation.

    static int16_t x[NCoef+1]; //input samples
    int n;

    //shift the old samples
    for(n=NCoef; n>0; n--) {
       x[n] = x[n-1];
       y[n] = y[n-1];
    }

    //Calculate the new output
    x[0] = NewSample;
    y[0] = ACoef[0] * x[0];
    for(n=1; n<=NCoef; n++)
        y[0] += ACoef[n] * x[n] - BCoef[n] * y[n];

    y[0] /= BCoef[0];

    return y[0] / DCgain;
}

void vApplicationIdleHook (void)
{
	idleHookCounter++;
}

void vApplicationStackOverflowHook( xTaskHandle xTask, signed char *pcTaskName)
{
	stackOverflowConter++;
}
/* USER CODE END 4 */

/* USER CODE BEGIN Header_TouchGFX_Task */
/**
  * @brief  Function implementing the GUI_Task thread.
  * @param  argument: Not used
  * @retval None
  */
/* USER CODE END Header_TouchGFX_Task */
__weak void TouchGFX_Task(void *argument)
{
  /* init code for USB_DEVICE */
  /* USER CODE BEGIN 5 */

  /* Infinite loop */
  for(;;)
  {
    osDelay(1);
  }
  /* USER CODE END 5 */
}

/* USER CODE BEGIN Header_StartDefaultTask */
/**
* @brief Function implementing the defaultTask thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_StartDefaultTask */
void StartDefaultTask(void *argument)
{
  /* USER CODE BEGIN StartDefaultTask */
  static int sdStatus;
  /* Infinite loop */
  for(;;)
  {
	  if(GPIOA->IDR & GPIO_PIN_15){
		  sdStatus = 0;
	  }else{
		  sdStatus = 1;
	  }
	  osMessageQueuePut(messageQueueHandle, &sdStatus, 0, 100);
	  osDelay(100);
	  idleHookCounter = 0;
  }
  /* USER CODE END StartDefaultTask */
}

/* USER CODE BEGIN Header_StartReadFileTask */
/**
* @brief Function implementing the readFileTask thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_StartReadFileTask */
void StartReadFileTask(void *argument)
{
  /* USER CODE BEGIN StartReadFileTask */
  FRESULT rc;
  UINT dmy;
  UINT bytesRead = 0;
  uint16_t hdat0 = 0, hdat1 = 0;
#ifdef USE_ENCODER_PLUGIN
  uint16_t aictrl3_reg = 0;
#endif
  uint8_t isRecordingStopped = 0;
  uint8_t isDataReady = 0;
  uint8_t periodsCntr = 0;
  uint8_t stimulusCntr = 0;
  /* Infinite loop */
  codecHardwareReset();

  for(;;)
  {
	GPIOA->BSRR = GPIO_PIN_1<<16; // reset sync out
	switch(fileCmd.command){
		case IDLE:
		default:
			// do nothing
			osDelay(1);
			break;

		case OPEN_FILE:
			fileCmd.command = IDLE;
			rc = f_open(&fil, currentFilePath, FA_READ);
			rc = f_read(&fil, recording_wav_header, sizeof(recording_wav_header), &bytesRead); // read file header (need for getting wav-files byterate)
			rc = f_lseek(&fil, 0); // return file offset to file beginning
			if(rc == FR_OK){
				fileCmd.fileSize = f_size(&fil);
				writeCommandRegister(CLOCKF_REG, 0x9800);
				fileCmd.status = FILE_OPENED;
				isFileStopped = 0;
				isFileOpened = 1;
			}else{
				fileCmd.status = FILE_ERROR;
				fileCmd.fileErrorCode = (uint8_t)rc;
				codecHardwareReset();
			}
			break;

		case START_READ_FILE:
			fileCmd.command = IDLE;
			if(isFileOpened){
				fileCmd.status = FILE_READING;
				volume = fileCmd.volume;
				setVolume(volume, volume);
			}
			break;

		case STOP_READ_FILE:
			fileCmd.command = IDLE;
			fileCmd.playedDataSize = 0;
			isFileStopped = 1;
			break;

		case PAUSE_READ:
			fileCmd.command = IDLE;
			if(fileCmd.status == FILE_READING){
				fileCmd.status = FILE_PAUSED;
				setVolume(254, 254);
			}
			break;

		case RESUME_READ:
			fileCmd.command = IDLE;
			if(fileCmd.status == FILE_PAUSED){
				fileCmd.status = FILE_READING;
				setVolume(volume, volume);
			}
			break;

		case REWIND_FILE:
			fileCmd.command = IDLE;
			if(!(readCommandRegister(STATUS_REG) & 0x8000)){ //check rewind allow
				prepareToRewindFile();
				rc = f_lseek(&fil, fileCmd.rewindOffset<<16);
				if(rc == FR_OK){
					fileCmd.playedDataSize = (fileCmd.rewindOffset<<16);
					fileCmd.rewindStatus = REWIND_SUCCESSFULL;
				}else{
					fileCmd.rewindStatus = REWIND_ERROR;
				}
			}else{
				fileCmd.rewindStatus = REWIND_NOT_ALLOWED;
			}
			break;

		case START_PCM_TEST:
			fileCmd.command = IDLE;
			volume = fileCmd.volume;
			setVolume(volume, volume);
			writeData(WAV_header_48kHz_16bit_stereo, sizeof(WAV_header_48kHz_16bit_stereo));
			if(fileCmd.isExternalStimulator){
				currentStimulusBuffer = PCM_data_Buffer1;
				currentDataBuffer = 0;
			}
			fileCmd.status = PCM_TEST_STARTED;
			break;

		case STOP_PCM_TEST:
			fileCmd.command = IDLE;
			//set SM_CANCEL bit in SCI_MODE register
			writeCommandRegister(MODE_REG, 0x4808);
			fileCmd.status = STATUS_IDLE;
			periodsCntr = 0;
			stimulusCntr = 0;
			break;

		case START_PCM_RECORDING:
			fileCmd.command = IDLE;
			if(!isRecording){
				// try to create new wav-file
				wav_files_count = FAT_FS_GetWAV_FilesCount(recordingFilesPath);
				sprintf(currentFileRecordingName, "%s/Record_%d.wav", recordingFilesPath, wav_files_count);
				rc = f_open(&fil, currentFileRecordingName, FA_CREATE_ALWAYS | FA_WRITE);
#ifndef USE_ENCODER_PLUGIN
				rc = f_lseek(&fil, 44);
#endif
				if(rc == FR_OK){
					isRecording = 1;
					startRecording(&recording_params);
					setMonitoringVolume(fileCmd.monitoringVolume);
					isRecordingStopped = 0;
					currentRecordingBuffer = PCM_recording_Buffer1;
					currentBuffer = 0;
					fileCmd.status = PCM_RECORDING_STARTED;
				}else{
					isRecording = 0;
					fileCmd.status = FILE_ERROR;
					fileCmd.fileErrorCode = (uint8_t)rc;
				}
			}
			break;

		case STOP_PCM_RECORDING:
			fileCmd.command = IDLE;
			if(isRecording){
				isRecordingStopped = 1;
#ifdef USE_ENCODER_PLUGIN
				writeCommandRegister(AICTRL3_REG, 1);
#endif
				}
			break;

		case PAUSE_PCM_RECORDING:
			fileCmd.command = IDLE;
			if(fileCmd.status == PCM_RECORDING_STARTED){
				fileCmd.status = PCM_RECORDING_PAUSED;
			}
			break;

		case RESUME_PCM_RECORDING:
			fileCmd.command = IDLE;
			if(fileCmd.status == PCM_RECORDING_PAUSED){
				fileCmd.status = PCM_RECORDING_STARTED;
			}
			break;

		case SET_MONITORING_VOLUME:
			fileCmd.command = IDLE;
			setMonitoringVolume(fileCmd.monitoringVolume);
			break;

		case SET_MIC_GAIN:
			fileCmd.command = IDLE;
			setMicGain(fileCmd.micGain);
			break;

		case SET_BASS_TREBLE:
			fileCmd.command = IDLE;
			writeCommandRegister(BASS_TREBLE_CONTROL_REG, fileCmd.bass_treble_settings[0]);
			if((readCommandRegister(MODE_REG) & 0x0001) != fileCmd.bass_treble_settings[1]){
				writeCommandRegister(MODE_REG, (readCommandRegister(MODE_REG) & 0xFFFE)|fileCmd.bass_treble_settings[1]);
			}
			break;

		case SET_VOLUME:
			fileCmd.command = IDLE;
			if(volume != fileCmd.volume){
				volume = fileCmd.volume;
				setVolume(volume, volume);
			}
			break;
	}

	if(fileCmd.status == FILE_READING){
		if(rc == FR_OK && !f_eof(&fil) && !isFileStopped){
			rc = f_forward(&fil, out_stream, 4096, &dmy);
			fileCmd.playedDataSize += dmy;
			if(fileCmd.bitrate == 0){
				fileCmd.bitrate = getCurrentFileBitrate(recording_wav_header);
			}
		}else{ // close file if it playing now
			if(isFileStopped){
				fileCmd.status = FILE_STOPPED;
			}else if(rc != FR_OK){
				fileCmd.status = FILE_ERROR;
				fileCmd.fileErrorCode = (uint8_t)rc;
			} else{
				fileCmd.status = STATUS_IDLE;
			}
			f_close(&fil);
			stopPlayAudioFile();
			writeCommandRegister(DECODE_TIME_REG, 0);
			fileCmd.playedDataSize = 0;
			fileCmd.bitrate = 0;
			isFileStopped = 0;
			isFileOpened = 0;
		}
	}else if(isFileStopped){ // close file if it not playing
		f_close(&fil);
		stopPlayAudioFile();
		writeCommandRegister(DECODE_TIME_REG, 0);
		fileCmd.status = FILE_STOPPED;
		fileCmd.playedDataSize = 0;
		fileCmd.bitrate = 0;
		isFileStopped = 0;
		isFileOpened = 0;
	}

	if(fileCmd.status == PCM_TEST_STARTED){
		if(!fileCmd.isExternalStimulator){
			writeData(PCM_data_Buffer1, sizeof(PCM_data_Buffer1));
		}else{
			if(periodsCntr < 10){
				if(stimulusCntr % 2 == 0 && periodsCntr == 0){
					GPIOA->BSRR = GPIO_PIN_1; // set sync out
				}
				periodsCntr++;
				writeData(currentStimulusBuffer, sizeof(PCM_data_Buffer1));
			}else{
				periodsCntr = 0;
				switchStimulusBuffer();
				fileCmd.externalStimulusEnded = 1;
				stimulusCntr++;
				if(stimulusCntr % 2 == 0 && stimulusCntr != 0){
					volume -= 10;
					setVolume(volume, volume);
				}
				if(stimulusCntr > 9){
					stimulusCntr = 0;
					periodsCntr = 0;
					//set SM_CANCEL bit in SCI_MODE register
					writeCommandRegister(MODE_REG, 0x4808);
					fileCmd.status = STATUS_IDLE;
				}
			}
		}
	}

	if(fileCmd.status == PCM_RECORDING_STARTED){
		hdat1 = readCommandRegister(HDAT1_REG);
		// data reading and stop recording have some differences depending sample frequency
#ifndef USE_ENCODER_PLUGIN
			if(hdat1 > HDAT1_LOW_LIMIT/* && hdat1 < HDAT1_UP_LIMIT*/){
				taskENTER_CRITICAL();
				for(uint16_t i = 0; i < hdat1; i++){
					hdat0 = readCommandRegister(HDAT0_REG);
					currentRecordingBuffer[currentIndex] = hdat0;
					currentIndex = (currentIndex+1) & RECORDING_BUFFER_MASK;
					if(currentIndex == 0){
						switchRecordingBuffer();
						isDataReady = 1;
					}
				}
				taskEXIT_CRITICAL();
			}else{
				osDelay(1);
			}

			if(isDataReady){
				isDataReady = 0;
				osSemaphoreRelease(dataReadySemaphoreHandle);
			}

			if(isRecordingStopped){
				isRecordingStopped = 0;
				stopRecording();
				fileCmd.status = STATUS_IDLE;

				switchRecordingBuffer();
				osSemaphoreRelease(dataReadySemaphoreHandle);
			}
#else
			if(hdat1 > HDAT1_LOW_LIMIT){
				taskENTER_CRITICAL();
				for(uint16_t i = 0; i < hdat1; i++){
					hdat0 = readCommandRegister(HDAT0_REG);
					currentRecordingBuffer[currentIndex] = ((hdat0 & 0xFF) << 8)|(hdat0 >> 8);
					currentIndex = (currentIndex+1) & RECORDING_BUFFER_MASK;
					if(currentIndex == 0){
						switchRecordingBuffer();
						isDataReady = 1;
					}
				}
				taskEXIT_CRITICAL();
			}

			if(isDataReady){
				isDataReady = 0;
				osSemaphoreRelease(dataReadySemaphoreHandle);
			}

			if(isRecordingStopped){
				aictrl3_reg = readCommandRegister(AICTRL3_REG);
				if(aictrl3_reg & 0x0002){
					isRecordingStopped = 0;
					stopRecording();
					fileCmd.status = STATUS_IDLE;

					switchRecordingBuffer();
					osSemaphoreRelease(dataReadySemaphoreHandle);
				}
			}
#endif
	}
	if(fileCmd.status == PCM_RECORDING_PAUSED){
		if(isRecordingStopped){
#ifndef USE_ENCODER_PLUGIN
				isRecordingStopped = 0;
				stopRecording();
				fileCmd.status = STATUS_IDLE;

				switchRecordingBuffer();
				osSemaphoreRelease(dataReadySemaphoreHandle);
#else
				aictrl3_reg = readCommandRegister(AICTRL3_REG);
				if(aictrl3_reg & 0x0002){
					isRecordingStopped = 0;
					stopRecording();
					fileCmd.status = STATUS_IDLE;

					switchRecordingBuffer();
					osSemaphoreRelease(dataReadySemaphoreHandle);
				}
#endif
			}
			osDelay(1);
		}
	}
  /* USER CODE END StartReadFileTask */
}

/* USER CODE BEGIN Header_StartWriteFileTask */
/**
* @brief Function implementing the writeFileTask thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_StartWriteFileTask */
void StartWriteFileTask(void *argument)
{
  /* USER CODE BEGIN StartWriteFileTask */
  FRESULT rc;
  uint32_t byteswritten;
  /* Infinite loop */
  for(;;)
  {
	if(dataReadySemaphoreHandle != NULL){
		if(osSemaphoreAcquire(dataReadySemaphoreHandle , portMAX_DELAY) == osOK)
		{
			if(fileCmd.status == PCM_RECORDING_STARTED){
				//write recording wav file
				rc = f_write(&fil, getRecordingBackBuffer(), sizeof(PCM_recording_Buffer1), (void *)&byteswritten);
				if(rc != FR_OK || byteswritten != sizeof(PCM_recording_Buffer1)){
					f_close(&fil);
					// reset flags and counters
					currentIndex = 0;
					isRecording = 0;
					fileCmd.recordingDataSize = 0;
					fileCmd.status = FILE_ERROR;
					fileCmd.fileErrorCode = (uint8_t)rc;
				}else{
					f_sync(&fil);
				}
				fileCmd.recordingDataSize += sizeof(PCM_recording_Buffer1);
			}
			if(fileCmd.status == STATUS_IDLE && isRecording){
				//write remaining data in buffer
				if(currentIndex > 0){
					rc = f_write(&fil, getRecordingBackBuffer(), currentIndex*2, (void *)&byteswritten);
					fileCmd.recordingDataSize += byteswritten;
				}
				// write file header
				rc = f_lseek(&fil, 0); // shift data start for header
				setRecordingPCM_WAVHeader(recording_wav_header, &recording_params, f_size(&fil));
				rc = f_write(&fil, recording_wav_header, sizeof(recording_wav_header), (void *)&byteswritten);
				if(rc != FR_OK || byteswritten != sizeof(recording_wav_header)){
					fileCmd.status = FILE_ERROR;
					fileCmd.fileErrorCode = (uint8_t)rc;
				}else{
					fileCmd.status = STATUS_IDLE;
				}
				f_close(&fil);
				// reset flags and counters
				currentIndex = 0;
				isRecording = 0;
				fileCmd.recordingDataSize = 0;
			}
		}
	}
    osDelay(1);
  }
  /* USER CODE END StartWriteFileTask */
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
     tex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
