#include "stm32f4xx_hal.h"
#include "w25q128.h"

extern SPI_HandleTypeDef hspi5;

void writeCommand(uint8_t cmd)
{
	GPIOF->BSRR = GPIO_PIN_6 << 16;
	HAL_SPI_Transmit(&hspi5, &cmd, 1, 1000);
	GPIOF->BSRR = GPIO_PIN_6;
}

void writeStatusRegister(uint8_t reg_num, uint8_t reg_data)
{
	uint8_t cmd_data[2] = {0};
	uint8_t reg_addr = 0;
	switch(reg_num)
	{
		case 0:
			reg_addr = WRITE_STATUS_REGISTER_1;
			break;

		case 1:
			reg_addr = WRITE_STATUS_REGISTER_2;
			break;

		case 2:
			reg_addr = WRITE_STATUS_REGISTER_3;
			break;
	}

	cmd_data[0] = reg_addr;
	cmd_data[1] = reg_data;

	if(!isWriteEnabled()){
		writeCommand(WRITE_ENABLE);
	}

	GPIOF->BSRR = GPIO_PIN_6 << 16;
	HAL_SPI_Transmit(&hspi5, cmd_data, 2, 1000);
	GPIOF->BSRR = GPIO_PIN_6;
}

uint8_t readStatusRegister(uint8_t reg_num)
{
	uint8_t reg_addr = 0;
	uint8_t cmd_data[2] = {0}, reg_data[2] = {0};
	switch(reg_num)
	{
		case 0:
			reg_addr = READ_STATUS_REGISTER_1;
			break;

		case 1:
			reg_addr = READ_STATUS_REGISTER_2;
			break;

		case 2:
			reg_addr = READ_STATUS_REGISTER_3;
			break;
	}

	cmd_data[0] = reg_addr;
	cmd_data[1] = 0xAA; // dummy byte

	GPIOF->BSRR = GPIO_PIN_6 << 16;
	HAL_SPI_TransmitReceive(&hspi5, cmd_data, reg_data, 2, 1000);
	GPIOF->BSRR = GPIO_PIN_6;

	return reg_data[1];
}

uint32_t readJedecID()
{
	uint8_t cmdData[4] = {JEDEC_ID, 0xAA, 0xAA, 0xAA}; // command and 3 dummy bytes
	uint8_t ID_data[4] = {0};

	GPIOF->BSRR = GPIO_PIN_6 << 16;
	HAL_SPI_TransmitReceive(&hspi5, cmdData, ID_data, 4, 1000);
	GPIOF->BSRR = GPIO_PIN_6;

	return (ID_data[1]<<16)|(ID_data[2]<<8)|ID_data[3];
}

void writePage(uint32_t pageAddress, uint8_t* data, uint16_t size)
{
	uint8_t cmd_data[4] = {PAGE_PROGRAM, (pageAddress & 0xFF0000)>>16, (pageAddress & 0xFF00)>>8, pageAddress & 0xFF};
	if(size > 256)
	{
		size = 256;
	}
	if(!isWriteEnabled()){
		writeCommand(WRITE_ENABLE);
	}
	GPIOF->BSRR = GPIO_PIN_6 << 16;
	HAL_SPI_Transmit(&hspi5, cmd_data, 4, 1000);
	HAL_SPI_Transmit(&hspi5, data, size, 1000);
	GPIOF->BSRR = GPIO_PIN_6;
}

void eraseSector(uint8_t sector_type, uint32_t pageAddress)
{
	uint8_t cmd_data[4] = {sector_type, (pageAddress & 0xFF0000)>>16, (pageAddress & 0xFF00)>>8, pageAddress & 0xFF};

	if(!isWriteEnabled()){
		writeCommand(WRITE_ENABLE);
	}

	GPIOF->BSRR = GPIO_PIN_6 << 16;
	HAL_SPI_Transmit(&hspi5, cmd_data, 4, 1000);
	GPIOF->BSRR = GPIO_PIN_6;
}

void eraseChip()
{
	if(!isWriteEnabled()){
		writeCommand(WRITE_ENABLE);
	}
	writeCommand(CHIP_ERASE);
}

void readData(uint32_t pageAddress, uint8_t* data, uint16_t size)
{
	uint8_t cmd_data[4] = {READ_DATA, (pageAddress & 0xFF0000)>>16, (pageAddress & 0xFF00)>>8, pageAddress & 0xFF};
	GPIOF->BSRR = GPIO_PIN_6 << 16;
	HAL_SPI_Transmit(&hspi5, cmd_data, 4, 1000);
	HAL_SPI_Receive(&hspi5, data, size, 1000);
	GPIOF->BSRR = GPIO_PIN_6;
}

uint8_t isFlashBusy()
{
	uint8_t sr1 = readStatusRegister(0);
	return ((sr1 & 0x01) != 0) ? 1 : 0;
}

uint8_t isWriteEnabled()
{
	uint8_t sr1 = readStatusRegister(0);
	return ((sr1 & 0x02) != 0) ? 1 : 0;
}
