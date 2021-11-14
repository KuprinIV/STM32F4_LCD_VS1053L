/**
  ******************************************************************************
  * @file   fatfs.c
  * @brief  Code for fatfs applications
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

#include "fatfs.h"

uint8_t retSD;    /* Return value for SD */
char SDPath[4];   /* SD logical drive path */
FATFS SDFatFS;    /* File system object for SD logical drive */
FIL SDFile;       /* File object for SD */

/* USER CODE BEGIN Variables */
static UINT out_stream (const BYTE *p,UINT btf);
/* USER CODE END Variables */

void MX_FATFS_Init(void)
{
  /*## FatFS: Link the SD driver ###########################*/
  retSD = FATFS_LinkDriver(&SD_Driver, SDPath);

  /* USER CODE BEGIN Init */
  /* additional user code for init */
  /* USER CODE END Init */
}

/**
  * @brief  Gets Time from RTC
  * @param  None
  * @retval Time in DWORD
  */
DWORD get_fattime(void)
{
  /* USER CODE BEGIN get_fattime */
  return 0;
  /* USER CODE END get_fattime */
}

/* USER CODE BEGIN Application */
FRESULT FAT_FS_Mount(void)
{
	FRESULT res = FR_OK;
	retSD = FATFS_LinkDriver(&SD_Driver, SDPath);
	HAL_Delay(100);
	res = f_mount(&SDFatFS, (TCHAR const*)SDPath, 1);
	return res;
}

FRESULT FAT_FS_Unmount(void)
{
	FRESULT res = FR_OK;
	res = f_mount(NULL, "", 0);
	HAL_Delay(100);
	FATFS_UnLinkDriver(SDPath);
	return res;
}

FRESULT FAT_FS_ReadFileData(char* file_path)
{
    FRESULT rc;
    FIL fil;
    UINT dmy;

    /* Open the audio file in read only mode */
    rc = f_open(&fil, file_path, FA_READ);
    if (rc) return rc;

    /* Repeat until the file pointer reaches end of the file */
    while (rc == FR_OK && !f_eof(&fil)) {

        /* some processes... */

        /* Fill output stream periodically or on-demand */
    	rc = f_forward(&fil, out_stream, 512, &dmy);
    }

    /* Close the file and return */
    f_close(&fil);
    return rc;
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

uint8_t FAT_FS_GetWAV_FilesCount(char* path)
{
    FRESULT res;
    FILINFO fno;
    DIR dir;
    uint8_t i = 0;
    char *fn;

    res = f_opendir(&dir, path);
    if (res == FR_OK)
    {
        for (;;)
        {
            res = f_readdir(&dir, &fno);
            if (res != FR_OK || fno.fname[0] == 0)
               break;

            if (fno.fname[0] == '.')
               continue;

            fn = fno.fname;

            if (!(fno.fattrib & AM_DIR))
            {
                if(strstr(fn, ".wav") != NULL){
                	i++;
                }
            }
        }
        f_closedir(&dir);
    }
    return i;
}
/* USER CODE END Application */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
