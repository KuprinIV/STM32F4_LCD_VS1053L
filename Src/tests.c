#include "stm32f4xx_hal.h"
#include "tests.h"
#include "w25q128.h"
#include "fatfs.h"

//static uint8_t buffer[_MAX_SS]; /* a work buffer for the f_mkfs() */

#define BUFFER_SIZE         ((uint32_t)0x0100)
#define WRITE_READ_ADDR     ((uint32_t)0x0800)
#define SDRAM_BANK_ADDR 	((uint32_t)0xD0000000)

static void Fill_Buffer(uint32_t *pBuffer, uint32_t uwBufferLenght, uint32_t uwOffset);

FRESULT res;                                          /* FatFs function common result code */
uint32_t byteswritten, bytesread;                     /* File write/read counts */
uint8_t wtext[] = "This is STM32 working with FatFs"; /* File write buffer */
uint8_t rtext[100];

uint32_t testSDRAM()
{
	/* Read/Write Buffers */
	uint32_t aTxBuffer[BUFFER_SIZE];
	uint32_t aRxBuffer[BUFFER_SIZE];

	/* Status variables */
	__IO uint32_t uwWriteReadStatus = 0;

	/* Counter index */
	uint32_t uwIndex = 0;

	Fill_Buffer(aTxBuffer, BUFFER_SIZE, 0xA244250F);

	  for (uwIndex = 0; uwIndex < BUFFER_SIZE; uwIndex++)
	  {
	    *(__IO uint32_t*) (SDRAM_BANK_ADDR + WRITE_READ_ADDR + 4*uwIndex) = aTxBuffer[uwIndex];
	  }

	  /* Read back data from the SDRAM memory */
	  for (uwIndex = 0; uwIndex < BUFFER_SIZE; uwIndex++)
	  {
	    aRxBuffer[uwIndex] = *(__IO uint32_t*) (SDRAM_BANK_ADDR + WRITE_READ_ADDR + 4*uwIndex);
	   }

	  /*##-3- Checking data integrity ############################################*/

	  for (uwIndex = 0; (uwIndex < BUFFER_SIZE) && (uwWriteReadStatus == 0); uwIndex++)
	  {
	    if (aRxBuffer[uwIndex] != aTxBuffer[uwIndex])
	    {
	      uwWriteReadStatus++;
	    }
	  }
	  return uwWriteReadStatus;
}

static void Fill_Buffer(uint32_t *pBuffer, uint32_t uwBufferLenght, uint32_t uwOffset)
{
  uint32_t tmpIndex = 0;

  /* Put in global buffer different values */
  for (tmpIndex = 0; tmpIndex < uwBufferLenght; tmpIndex++ )
  {
    pBuffer[tmpIndex] = tmpIndex + uwOffset + (tmpIndex << 16);
  }
}

uint32_t testFlash()
{
	uint32_t id = readJedecID();
	uint8_t testBufSrc[255] = {0};
	uint8_t testBufDst[255] = {0};
	uint8_t res = 0;
	uint32_t address = 0x1F00;

	for(uint8_t i = 0; i < 255; i++){
		testBufSrc[i] = i;
	}
//	eraseChip();
//	eraseSector(0x20, 0x1000);
//	writePage(address, testBufSrc, 255);
	while(isFlashBusy()){}
	readData(address, testBufDst, 255);

	for(uint8_t i = 0; i < 255; i++){
		if(testBufSrc[i] != testBufDst[i])
		{
			res++;
		}

	}

	return res;
}

uint8_t testFatFS()
{
	uint8_t result = 0;

//	  if(f_mount(&SDFatFS, (TCHAR const*)SDPath, 0) != FR_OK)
//	  {
//		/* FatFs Initialization Error */
//		result = 1;
//	  }
//	  else
//	  {
//		/*##-3- Create a FAT file system (format) on the logical drive #########*/
//		if(/*f_mkfs((TCHAR const*)SDPath, FM_ANY, 0, buffer, sizeof(buffer))*/0 != FR_OK)
//		{
//		  /* FatFs Format Error */
//			result = 2;
//		}
//		else
//		{
		  /*##-4- Create and Open a new text file object with write access #####*/
		if(f_open(&SDFile, "STM32.TXT", FA_CREATE_ALWAYS | FA_WRITE) != FR_OK)
		{
		  /* 'STM32.TXT' file Open for write Error */
			result = 3;
		}
		else
		{
		  /*##-5- Write data to the text file ################################*/
		  res = f_write(&SDFile, wtext, sizeof(wtext), (void *)&byteswritten);

		  if((byteswritten == 0) || (res != FR_OK))
		  {
			/* 'STM32.TXT' file Write or EOF Error */
			  result = 4;
		  }
		  else
		  {
			  /*##-6- Close the open text file #################################*/
			f_close(&SDFile);

			  /*##-7- Open the text file object with read access ###############*/
			if(f_open(&SDFile, "STM32.TXT", FA_READ) != FR_OK)
			{
			  /* 'STM32.TXT' file Open for read Error */
				result = 5;
			}
			else
			{
				/*##-8- Read data from the text file ###########################*/
			  res = f_read(&SDFile, rtext, sizeof(rtext), (void *)&bytesread);

			  if((bytesread == 0) || (res != FR_OK))
			  {
				/* 'STM32.TXT' file Read or EOF Error */
					result = 6;
			  }
			  else
			  {
				  /*##-9- Close the open text file #############################*/
				f_close(&SDFile);

				  /*##-10- Compare read data with the expected data ############*/
				  if ((bytesread != byteswritten))
				  {
					/* Read data is different from the expected data */
					  result = 7;
				  }
				  else
				  {
					/* Success of the demo: no error occurrence */
					  result = 0;
				  }
				}
			  }
			}
		  }
//		}
//	  }
//	  FATFS_UnLinkDriver(SDPath);

	  return result;
}
