#ifndef __W25Q128_H
#define __W25Q128_H

#ifdef __cplusplus
 extern "C" {
#endif

#define WRITE_ENABLE 0x06
#define VOLATILE_SR_WRITE_ENABLE 0x50
#define WRITE_DISABLE 0x04
#define READ_STATUS_REGISTER_1 0x05
#define WRITE_STATUS_REGISTER_1 0x01
#define READ_STATUS_REGISTER_2 0x35
#define WRITE_STATUS_REGISTER_2 0x31
#define READ_STATUS_REGISTER_3 0x15
#define WRITE_STATUS_REGISTER_3 0x11
#define CHIP_ERASE 0xC7
#define ERASE_PROGRAM_SUSPEND 0x75
#define ERASE_PROGRAM_RESUME 0x7A
#define POWER_DOWN 0xB9
#define RELEASE_POWER_DOWN_ID 0xAB
#define MANUFACTURER_DEVICE_ID 0x90
#define JEDEC_ID 0x9F
#define GLOBAL_BLOCK_LOCK 0x7E
#define GLOBAL_BLOCK_UNLOCK 0x98
#define ENTER_QSPI_MODE 0x38
#define ENABLE_RESET 0x66
#define RESET_DEVICE 0x99
#define READ_UNIQUE_ID 0x4B
#define PAGE_PROGRAM 0x02
#define QUAD_PAGE_PROGRAM 0x32
#define SECTOR_ERASE_4KB 0x20
#define BLOCK_ERASE_32KB 0x52
#define BLOCK_ERASE_64KB 0xD8
#define READ_DATA 0x03
#define FAST_READ 0x0B
#define FAST_READ_DUAL_OUTPUT 0x3B
#define FAST_READ_QUAD_OUTPUT 0x6B
#define READ_SFDP_REGISTER 0x5A
#define ERASE_SECURITY_REGISTER 0x44
#define PROGRAM_SECURITY_REGISTER 0x42
#define READ_SECUURITY_REGISTER 0x48
#define INDIVIDUAL_BLOCK_LOCK 0x36
#define INDIVIDUAL_BLOCK_UNLOCK 0x39
#define READ_BLOCK_LOCK 0x3D
#define SET_BURST_WITH_WRAP 0x77

void writeCommand(uint8_t cmd);
void writeStatusRegister(uint8_t reg_num, uint8_t reg_data);
uint8_t readStatusRegister(uint8_t reg_num);
uint32_t readJedecID();
void writePage(uint32_t pageAddress, uint8_t* data, uint16_t size);
void eraseSector(uint8_t sector_type, uint32_t pageAddress);
void eraseChip();
void readData(uint32_t pageAddress, uint8_t* data, uint16_t size);
uint8_t isFlashBusy();
uint8_t isWriteEnabled();

#ifdef __cplusplus
}
#endif

#endif
