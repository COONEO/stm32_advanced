#ifndef OTA_PROTOCOL_H
#define OTA_PROTOCOL_H

#include "main.h"
#include "usart.h"


#define PACKET_START_MARKER 0xABCD
#define PACKET_END_MARKER   0xDCBA
#define PACKET_DATA_SIZE    8



// 定义 Flash 每个扇区的大小
#define FLASH_SECTOR_SIZE 0x400


typedef struct {
    uint16_t start_marker;
    uint8_t data_length;
    uint8_t data[PACKET_DATA_SIZE];
    uint8_t checksum;
    uint16_t end_marker;
} OTA_Packet;

typedef enum {
    OTA_OK,
    OTA_ERROR_START_MARKER,
    OTA_ERROR_CHECKSUM,
    OTA_ERROR_END_MARKER,
    OTA_ERROR_DATA_LENGTH,
    OTA_ERROR_INVALID_ADDRESS
} OTA_Status;

OTA_Status OTA_ParsePacket(uint8_t *buffer, OTA_Packet *packet);
OTA_Status OTA_WriteData(uint32_t address, uint8_t *data, uint32_t size);
void WriteToFlash(uint32_t address, uint8_t *data, uint32_t size);
uint32_t GetSector(uint32_t address);
void EraseFlash(uint32_t start_address, uint32_t length);
void EraseFlashSector(uint32_t sector_address);
uint32_t GetSector(uint32_t address);
void WriteToFlash(uint32_t address, uint8_t *data, uint32_t size);
#endif /* OTA_PROTOCOL_H */
