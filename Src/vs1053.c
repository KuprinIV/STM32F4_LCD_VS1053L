#include "vs1053.h"

static void loadPlugin(const uint16_t* plugin, uint16_t size);

extern SPI_HandleTypeDef hspi2;

uint16_t mp3_bitrates[3][2][14] =
{{{8, 16, 24, 32, 40, 48, 56, 64, 80, 96, 112, 128, 144, 160}, {32, 40, 48, 56, 64, 80, 96, 112, 128, 160, 192, 224, 256, 320}},
{{8, 16, 24, 32, 40, 48, 56, 64, 80, 96, 112, 128, 144, 160}, {32, 48, 56, 64, 80, 96, 112, 128, 160, 192, 224, 256, 320, 384}},
{{32, 48, 56, 64, 80, 96, 112, 128, 144, 160, 176, 192, 224, 256}, {32, 64, 96, 128, 160, 192, 224, 256, 288, 320, 352, 384, 416, 448}}};

static void loadPlugin(const uint16_t* plugin, uint16_t size) {
  int i = 0;

  while (i < size) {
    uint16_t addr, n, val;
    addr = plugin[i++];
    n = plugin[i++];
    if (n & 0x8000U) { /* RLE run, replicate n samples */
      n &= 0x7FFF;
      val = plugin[i++];
      while (n--) {
    	  writeCommandRegister((uint8_t)addr, val);
      }
    } else {           /* Copy run, copy n samples */
      while (n--) {
        val = plugin[i++];
        writeCommandRegister((uint8_t)addr, val);
      }
    }
  }
}

/*static*/ void writeCommandRegister(uint8_t cmd_address, uint16_t data)
{
	uint8_t cmd_data[4] = {0};
	cmd_data[0] = 0x02; //write instruction
	cmd_data[1] = cmd_address;
	cmd_data[2] = (uint8_t)((data>>8) & 0xFF);
	cmd_data[3] = (uint8_t)(data & 0xFF);

	GPIOG->BSRR = GPIO_PIN_2; // set to 1 xDCS
	GPIOB->BSRR = GPIO_PIN_12 << 16;
	HAL_SPI_Transmit(&hspi2, cmd_data, 4, 1000);
	GPIOB->BSRR = GPIO_PIN_12;

	while(!(GPIOD->IDR & GPIO_PIN_13)){} //wait while DREQ set to 1
}

/*static*/ uint16_t readCommandRegister(uint8_t cmd_address)
{
	uint8_t cmd_data[4] = {0};
	uint8_t reg_data[4] = {0};

	cmd_data[0] = 0x03; //read instruction
	cmd_data[1] = cmd_address;

	SPI2->CR1 |= SPI_CR1_SPE;
	GPIOG->BSRR = GPIO_PIN_2;  // set to 1 xDCS
	GPIOB->BSRR = GPIO_PIN_12 << 16;
//	HAL_SPI_TransmitReceive(&hspi2, cmd_data, reg_data, 4, 1000);
	for(uint8_t i = 0; i < 4; i++){
		SPI2->DR = cmd_data[i];
		while(!(SPI2->SR & SPI_SR_TXE)){}
		while(!(SPI2->SR & SPI_SR_RXNE)){}
		reg_data[i] = SPI2->DR;
	}
	GPIOB->BSRR = GPIO_PIN_12;
	SPI2->CR1 &= ~SPI_CR1_SPE;

	while(!(GPIOD->IDR & GPIO_PIN_13)){} //wait while DREQ set to 1

	return (reg_data[2]<<8)|reg_data[3];
}

void codecHardwareReset()
{
	  GPIOC->BSRR = GPIO_PIN_6<<16;
	  HAL_Delay(10);
	  GPIOC->BSRR = GPIO_PIN_6;
	  while(!(GPIOD->IDR & GPIO_PIN_13)){} //wait while DREQ set to 1
	  HAL_Delay(10);
}

void startSineTest()
{
	uint16_t mode_reg = 0;
	uint8_t test_cmd[8] = {0x53, 0xEF, 0x6E, 0x22, 0, 0, 0, 0}; //start 750 Hz sine test command

	codecHardwareReset();
	mode_reg = 0x4820;
	HAL_Delay(10);
	writeCommandRegister(MODE_REG, mode_reg); // allow tests
	setVolume(127, 127); //set middle point volume
	writeData(test_cmd, 8); // transmit test sine command
}

void stopSineTest()
{
	uint16_t mode_reg = 0;
	uint8_t test_cmd[8] = {0x45, 0x78, 0x69, 0x74, 0, 0, 0, 0};

	mode_reg = 0x4800;
	HAL_Delay(10);
	writeCommandRegister(MODE_REG, mode_reg); // deny test
	writeData(test_cmd, 8); // transmit stop sine test command
	codecHardwareReset();
}

void setVolume(uint8_t left_vol, uint8_t right_vol)
{
	writeCommandRegister(VOLUME_CONTROL_REG, (left_vol<<8)|right_vol);
}

void writeData(uint8_t* data, uint16_t size)
{
	uint16_t offset = 0;
	GPIOB->BSRR = GPIO_PIN_12;
	GPIOG->BSRR = GPIO_PIN_2 << 16;
	while(offset < size){
		HAL_SPI_Transmit(&hspi2, data + offset, (size-offset < 32) ? (size-offset) : (32), 1000);
		while(!(GPIOD->IDR & GPIO_PIN_13)){} //wait while DREQ set to 1
		offset += 32;
	}
	GPIOG->BSRR = GPIO_PIN_2;
}

void readRAMData(uint16_t ramAddr, uint16_t* data, uint16_t size)
{
	//write RAM start address
	writeCommandRegister(WRAMADDR_REG, ramAddr);
	//read data
	for(uint16_t i = 0; i < size; i++){
		data[i] = readCommandRegister(WRAM_REG);
	}
}

void writeRAMData(uint16_t ramAddr, uint16_t* data, uint16_t size)
{
	//write RAM start address
	writeCommandRegister(WRAMADDR_REG, ramAddr);
	//read data
	for(uint16_t i = 0; i < size; i++){
		writeCommandRegister(WRAM_REG, data[i]);
	}
}

void stopPlayAudioFile(void)
{
	uint8_t endFillByteArray[32] = {0};
	uint8_t endFillByte = 0;
	uint16_t endFillByteVal = 0;
	uint16_t mode_reg_value = 0, cntr = 0, hdat0 = 0, hdat1 = 0;

	//read endFillByte
	readRAMData(PARAMETRIC_STRUCT_ADDRESS+6, &endFillByteVal, 1);
	endFillByte = (uint8_t)(endFillByteVal & 0x00FF);
	memset(endFillByteArray, endFillByte, 32);
	//send at least 2052 endFillByte
	for(uint16_t i = 0; i < 65; i++){
		writeData(endFillByteArray, 32);
	}
	//set SM_CANCEL bit in SCI_MODE register
	writeCommandRegister(MODE_REG, 0x4808);
	//send at least 32 bytes of endFillByte
	writeData(endFillByteArray, 32);
	//read SCI MODE register and check SM CANCEL bit
	mode_reg_value = readCommandRegister(MODE_REG);
	while((mode_reg_value & 0x0008) && (cntr++ < 64)){
		//send at least 32 bytes of endFillByte
		writeData(endFillByteArray, 32);
		//read SCI MODE register and check SM CANCEL bit
		mode_reg_value = readCommandRegister(MODE_REG);
	}
	//if SM CANCEL hasn’t cleared after sending 2048 bytes, do a software reset
	if(cntr >= 64){
		writeCommandRegister(MODE_REG, mode_reg_value|0x0004);
	}
	hdat0 = readCommandRegister(HDAT0_REG);
	hdat1 = readCommandRegister(HDAT1_REG);
	if(hdat0 != 0 || hdat1 != 0){
		writeCommandRegister(MODE_REG, mode_reg_value|0x0004);
	}
}

void prepareToRewindFile(void)
{
	uint8_t endFillByteArray[2052] = {0};
	uint8_t endFillByte = 0;
	uint16_t endFillByteVal = 0;
	//read endFillByte
	readRAMData(PARAMETRIC_STRUCT_ADDRESS+6, &endFillByteVal, 1);
	endFillByte = (uint8_t)(endFillByteVal & 0x00FF);
	memset(endFillByteArray, endFillByte, 2052);
	//send at least 2048 bytes of endFillByte
	writeData(endFillByteArray, 2052);
}

uint32_t getCurrentFileBitrate(uint8_t* file_header)
{
	uint32_t currentBitrate = 0;
	uint16_t hdat1 = 0, hdat0 = 0;
	uint8_t id = 0, layer = 0, bitrate = 0;
	// read HDAT0 and HDAT1 registers
	hdat1 = readCommandRegister(HDAT1_REG);
	hdat0 = readCommandRegister(HDAT0_REG);

	if(hdat1 == 0x7665){ // it's WAV
		currentBitrate = (file_header[31]<<24)|(file_header[30]<<16)|(file_header[29]<<8)|(file_header[28]);
	}
	if((hdat1 & 0xFF00) == 0xFF00){ // it's MP3
		id = ((uint8_t)((hdat1>>3) & 0x03) == 3) ? 1 : 0;
		layer = (uint8_t)((hdat1>>1) & 0x03) - 1;
		bitrate = (uint8_t)((hdat0>>12)&0x0F) - 1;
		currentBitrate = (mp3_bitrates[layer][id][bitrate]>>3)*1000; // convert to Bytes/s
	}

	return currentBitrate;
}

void selectInput(uint8_t src)
{
	uint16_t mode_reg = 0;
	mode_reg = readCommandRegister(MODE_REG);
	if(src){
		mode_reg |= 0x4800;
	}else{
		mode_reg &= 0xBFFF;
	}
	writeCommandRegister(MODE_REG, mode_reg);
}

void startRecording(volatile recordingParams* rp)
{
	uint16_t mode_reg = 0, ramData = 0;
	uint16_t aictrl_reg3 = 0;

	mode_reg = readCommandRegister(MODE_REG);
	mode_reg |= 0x0004;
	writeCommandRegister(MODE_REG, mode_reg);
	HAL_Delay(1);
	// reset SM_ADPCM bit
	mode_reg &= 0xEBFB;
	writeCommandRegister(MODE_REG, mode_reg);
	//set clock close to 55,3 MHz
	writeCommandRegister(CLOCKF_REG, 0xC000);
	//set SCI_BASS to 0
	writeCommandRegister(BASS_TREBLE_CONTROL_REG, 0);
	//disable any potential user application
	writeCommandRegister(AIADDR_REG, 0);
	//disable all interrupts except SCI interrupts
	ramData = 2;
	writeRAMData(0xC01A, &ramData, 1);
	//initialize PCM recording differs for high sample frequencies, because another plugin is applied
#ifdef USE_ENCODER_PLUGIN
		//load plugin
		switch(rp->sample_rate){
			case 24000:
				if(rp->channelType == 2){ // mono, left channel
					loadPlugin(vs1053b_24k_l, 1542);
				}else if (rp->channelType == 3){ // mono, right channel
					loadPlugin(vs1053b_24k_r, 1538);
				}
				break;

			case 48000:
				if(rp->channelType == 2){ // mono, left channel
					loadPlugin(vs1053b_48k_l, 1567);
				}else if (rp->channelType == 3){ // mono, right channel
					loadPlugin(vs1053b_48k_r, 1567);
				}
				break;

			default:
				break;
		}
		//set ADPCM bit, select input
		mode_reg |= 0x1800;
		writeCommandRegister(MODE_REG, mode_reg);
		//set AICTRL registers
		writeCommandRegister(AICTRL1_REG, rp->micGain);
		writeCommandRegister(AICTRL2_REG, rp->maxAutoGain);
		writeCommandRegister(AICTRL0_REG, 0);
		writeCommandRegister(AICTRL3_REG, 0);
		// activate encoder
		writeCommandRegister(AIADDR_REG, 0x34);
		//wait at least 1 us
		HAL_Delay(1);
#else
		aictrl_reg3 = ((rp->pcmMode & 0x01)<<2)|(rp->channelType & 0x03);
		//set AICTRL registers
		writeCommandRegister(AICTRL0_REG, rp->sample_rate);
		writeCommandRegister(AICTRL1_REG, rp->micGain);
		writeCommandRegister(AICTRL2_REG, rp->maxAutoGain);
		writeCommandRegister(AICTRL3_REG, aictrl_reg3);

		//set ADPCM bit, select input
		mode_reg |= 0x1800;
		writeCommandRegister(MODE_REG, mode_reg);

		//write patch
		loadPlugin(IMA_fix_plugin, 74);
#endif
	//wait until DREQ pin is high
	while(!(GPIOD->IDR & GPIO_PIN_13)){} //wait while DREQ set to 1
}

void stopRecording(void)
{
	uint16_t mode_reg = 0;
	mode_reg = readCommandRegister(MODE_REG);
	mode_reg &= 0xEFFF;
	mode_reg |= 0x0804;
	writeCommandRegister(MODE_REG, mode_reg);
#ifdef USE_ENCODER_PLUGIN
	writeCommandRegister(AICTRL3_REG, 0);
	writeCommandRegister(AIADDR_REG, 0);
#endif
}

void setRecordingPCM_WAVHeader(uint8_t* header_data, volatile recordingParams* rp, uint32_t file_size)
{
	uint32_t chunk_size = file_size - 8, data_size = file_size - 36;
	uint8_t ch_num = 0;
	if(rp->channelType > 1){
		ch_num = 1;
	}else{
		ch_num = 2;
	}
	// chunk ID
	header_data[0] = 0x52;
	header_data[1] = 0x49;
	header_data[2] = 0x46;
	header_data[3] = 0x46;
	// chunk size
	header_data[4] = (uint8_t)(chunk_size & 0xFF);
	header_data[5] = (uint8_t)((chunk_size>>8) & 0xFF);
	header_data[6] = (uint8_t)((chunk_size>>16) & 0xFF);
	header_data[7] = (uint8_t)((chunk_size>>24) & 0xFF);
	// format
	header_data[8] = 0x57;
	header_data[9] = 0x41;
	header_data[10] = 0x56;
	header_data[11] = 0x45;
	// subChunk1ID
	header_data[12] = 0x66;
	header_data[13] = 0x6D;
	header_data[14] = 0x74;
	header_data[15] = 0x20;
	// subChunk1Size
	header_data[16] = 0x10;
	header_data[17] = 0x00;
	header_data[18] = 0x00;
	header_data[19] = 0x00;
	// audio format (PCM)
	header_data[20] = 0x01;
	header_data[21] = 0x00;
	// num of channels
	header_data[22] = ch_num;
	header_data[23] = 0x00;
	// sample rate
	header_data[24] = (uint8_t)(rp->sample_rate & 0xFF);
	header_data[25] = (uint8_t)((rp->sample_rate>>8) & 0xFF);
	header_data[26] = (uint8_t)((rp->sample_rate>>16) & 0xFF);
	header_data[27] = (uint8_t)((rp->sample_rate>>24) & 0xFF);
	// byte rate
	header_data[28] = (uint8_t)(((rp->sample_rate*ch_num)<<1) & 0xFF);
	header_data[29] = (uint8_t)(((rp->sample_rate*ch_num)>>7) & 0xFF);
	header_data[30] = (uint8_t)(((rp->sample_rate*ch_num)>>15) & 0xFF);
	header_data[31] = (uint8_t)(((rp->sample_rate*ch_num)>>23) & 0xFF);
	// block align
	header_data[32] = ch_num<<1;
	header_data[33] = 0x00;
	// bits per sample
	header_data[34] = 0x10;
	header_data[35] = 0x00;
	// subChunk3ID
	header_data[36] = 0x64;
	header_data[37] = 0x61;
	header_data[38] = 0x74;
	header_data[39] = 0x61;
	// subChunk3Size
	header_data[40] = (uint8_t)(data_size & 0xFF);
	header_data[41] = (uint8_t)((data_size>>8) & 0xFF);
	header_data[42] = (uint8_t)((data_size>>16) & 0xFF);
	header_data[43] = (uint8_t)((data_size>>24) & 0xFF);
}

void setMicGain(uint16_t gain)
{
	writeCommandRegister(AICTRL1_REG, gain);
	if(gain == 0){
		writeCommandRegister(AICTRL2_REG, 4096);
	}else{
		writeCommandRegister(AICTRL2_REG, 0);
	}
}

void setMonitoringVolume(uint8_t vol)
{
	uint16_t vol_data = (vol<<8)|(vol);
	writeRAMData(0xC045, &vol_data, 1);
}

void doSoftReset(void)
{
	uint16_t mode_reg = 0;
	mode_reg = readCommandRegister(MODE_REG);
	mode_reg |= 0x0004;
	writeCommandRegister(MODE_REG, mode_reg);
}
