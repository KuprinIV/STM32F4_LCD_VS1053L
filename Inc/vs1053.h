#ifndef __VS1053_H
#define __VS1053_H

#ifdef __cplusplus
 extern "C" {
#endif

#include <string.h>
#include "stm32f4xx_hal.h"
#include "main.h"
#include "vs1053b_plugins.h"

#define PARAMETRIC_VERSION 0x0003
#define PARAMETRIC_STRUCT_ADDRESS 0x1E00

#define MODE_REG (uint8_t)0x00
#define STATUS_REG (uint8_t)0x01
#define BASS_TREBLE_CONTROL_REG (uint8_t)0x02
#define CLOCKF_REG (uint8_t)0x03
#define DECODE_TIME_REG (uint8_t)0x04
#define AUDATA_REG (uint8_t)0x05
#define WRAM_REG (uint8_t)0x06
#define WRAMADDR_REG (uint8_t)0x07
#define HDAT0_REG (uint8_t)0x08
#define HDAT1_REG (uint8_t)0x09
#define AIADDR_REG (uint8_t)0x0A
#define VOLUME_CONTROL_REG (uint8_t)0x0B
#define AICTRL0_REG (uint8_t)0x0C
#define AICTRL1_REG (uint8_t)0x0D
#define AICTRL2_REG (uint8_t)0x0E
#define AICTRL3_REG (uint8_t)0x0F

//#define USE_ENCODER_PLUGIN 1

typedef struct
{
	/* configs are not cleared between files */
	uint32_t chipID; /*1e00/01 Initialized at reset for your convenience */
	uint16_t version; /*1e02 - structure version */
	uint16_t config1; /*1e03 ---- ---- ppss RRRR PS mode, SBR mode, Reverb */
	uint16_t playSpeed; /*1e04 0,1 = normal speed, 2 = twice, 3 = three times etc. */
	uint16_t byteRate; /*1e05 average byterate */
	uint16_t endFillByte; /*1e06 byte value to send after file sent */
	uint16_t reserved[16]; /*1e07..15 file byte offsets */
	uint32_t jumpPoints[8]; /*1e16..25 file byte offsets */
	uint16_t latestJump; /*1e26 index to lastly updated jumpPoint */
	uint32_t positionMsec; /*1e27-28 play position, if known (WMA, Ogg Vorbis) */
	int16_t resync; /*1e29 > 0 for automatic m4a, ADIF, WMA resyncs */
	union {
			struct {
				uint32_t curPacketSize;
				uint32_t packetSize;
				} wma;
			struct {
				uint16_t sceFoundMask; /*1e2a SCE’s found since last clear */
				uint16_t cpeFoundMask; /*1e2b CPE’s found since last clear */
				uint16_t lfeFoundMask; /*1e2c LFE’s found since last clear */
				uint16_t playSelect; /*1e2d 0 = first any, initialized at aac init */
				int16_t dynCompress; /*1e2e -8192=1.0, initialized at aac init */
				int16_t dynBoost; /*1e2f 8192=1.0, initialized at aac init */
				uint16_t sbrAndPsStatus; /*0x1e30 1=SBR, 2=upsample, 4=PS, 8=PS active */
				} aac;
			struct {
				uint32_t bytesLeft;
				} midi;
			struct {
				int16_t gain; /* 0x1e2a proposed gain offset in 0.5dB steps, default = -12 */
				} vorbis;
		} i;
}codecParametric;

/*static*/ void writeCommandRegister(uint8_t cmd_address, uint16_t data);
/*static*/ uint16_t readCommandRegister(uint8_t cmd_address);
void codecHardwareReset();
void startSineTest();
void stopSineTest();
void writeData(uint8_t* data, uint16_t size);
void setVolume(uint8_t left_vol, uint8_t right_vol);
void readRAMData(uint16_t ramAddr, uint16_t* data, uint16_t size);
void writeRAMData(uint16_t ramAddr, uint16_t* data, uint16_t size);

void stopPlayAudioFile(void);
void prepareToRewindFile(void);
uint32_t getCurrentFileBitrate(uint8_t* file_header); // return bitrate in Bytes/s
void selectInput(uint8_t src); // 0 - mic, 1 - line1
void startRecording(volatile recordingParams* rp);
void stopRecording(void);
void setRecordingPCM_WAVHeader(uint8_t* header_data, volatile recordingParams* rp, uint32_t file_size);
void setMonitoringVolume(uint8_t vol);
void setMicGain(uint16_t gain);
void doSoftReset(void);

#ifdef __cplusplus
}
#endif

#endif
