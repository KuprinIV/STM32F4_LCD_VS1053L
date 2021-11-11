/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.h
  * @brief          : Header for main.c file.
  *                   This file contains the common defines of the application.
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

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __MAIN_H
#define __MAIN_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32f4xx_hal.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

/* Exported types ------------------------------------------------------------*/
/* USER CODE BEGIN ET */
typedef enum{
	IDLE = (uint8_t)0,
	OPEN_FILE,
	START_READ_FILE,
	STOP_READ_FILE,
	PAUSE_READ,
	RESUME_READ,
	REWIND_FILE,
	START_PCM_TEST,
	STOP_PCM_TEST,
	START_PCM_RECORDING,
	PAUSE_PCM_RECORDING,
	RESUME_PCM_RECORDING,
	STOP_PCM_RECORDING,
	SET_MONITORING_VOLUME,
	SET_MIC_GAIN,
	SET_BASS_TREBLE,
	SET_VOLUME
}commandType;

typedef enum{
	STATUS_IDLE = (uint8_t)0,
	FILE_OPENED,
	FILE_READING,
	FILE_PAUSED,
	FILE_STOPPED,
	FILE_ERROR,
	PCM_TEST_STARTED,
	PCM_RECORDING_STARTED,
	PCM_RECORDING_PAUSED
}fileStatus;

typedef enum{
	MP3,
	WAV
}audioFileType;

typedef enum{
	REWIND_NOT_ALLOWED = (uint8_t)0,
	REWIND_SUCCESSFULL,
	REWIND_ERROR
}rewindStatus;

typedef enum{
	SD_IS_NOT_CONNECTED = (uint8_t)0,
	SD_IS_CONNECTED
}sdConnectionStatus;

typedef enum{
	FS_NOT_MOUNTED = (uint8_t)0,
	FS_IS_MOUNTED
}fsMountStatus;

typedef struct{
	commandType command;
	fileStatus status;
	rewindStatus rewindStatus;
	uint32_t fileSize;
	uint32_t playedDataSize;
	uint32_t rewindOffset;
	uint8_t volume;
	uint32_t bitrate;
	uint32_t recordingDataSize;
	uint8_t monitoringVolume;
	uint16_t micGain;
	uint16_t bass_treble_settings[2];
	uint8_t fileErrorCode;
	uint8_t isExternalStimulator;
	uint8_t externalStimulusEnded;
//	char filePath[255];
}commandStruct;

typedef struct{
	sdConnectionStatus isConnected;
	fsMountStatus isMounted;
}sdState;

typedef struct{
	uint16_t sample_rate; // sampling frequency
	uint16_t micGain; // mic gain (1024 - 1x, ..., 65535 - 64x, 0 - autogain)
	uint16_t maxAutoGain; // max autogain value (1024 - 1x, ..., 65535 - 64x)
	uint8_t channelType; // channel: 0 - joint stereo, 1 - dual stereo, 2 - left, 3 - right
	uint8_t pcmMode; // PCM mode: 0 - IMA PCM, 1 - linear PCM
}recordingParams;
/* USER CODE END ET */

/* Exported constants --------------------------------------------------------*/
/* USER CODE BEGIN EC */

/* USER CODE END EC */

/* Exported macro ------------------------------------------------------------*/
/* USER CODE BEGIN EM */

/* USER CODE END EM */

void HAL_TIM_MspPostInit(TIM_HandleTypeDef *htim);

/* Exported functions prototypes ---------------------------------------------*/
void Error_Handler(void);

/* USER CODE BEGIN EFP */
void setCurrentFilePath(char* path);
void sendCommand(commandStruct* cmd);
void setRecordingParams(recordingParams* rp);
void resetStatus(void);
void resetRewindStatus(void);
commandStruct* getCommand();
void setPCMDataBuffer(uint8_t* buf);
/* USER CODE END EFP */

/* Private defines -----------------------------------------------------------*/
/* USER CODE BEGIN Private defines */

/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
