#ifndef OTA_PROTOCOL_H
#define OTA_PROTOCOL_H

#include "main.h"
#include "usart.h"
#include "bootloader.h"

#include "stdbool.h"

// Define flash sector size
#define FLASH_SECTOR_SIZE 1024

// Define start and end markers for packet
#define PACKET_START_MARKER 0xABCD
#define PACKET_END_MARKER   0xDCBA

// Define flash address range
#define FLASH_START_ADDR 0x08003800
#define FLASH_END_ADDR   0x08006400

// Define packet data size
#define PACKET_DATA_SIZE 64

// 每个数据包的总长度为70字节
#define PACKET_TOTAL_SIZE 70

// Define OTA status
typedef enum {
    OTA_OK = 0,
    OTA_ERROR_START_MARKER,
    OTA_ERROR_DATA_LENGTH,
    OTA_ERROR_CHECKSUM,
    OTA_ERROR_END_MARKER
} OTA_Status;

// Define OTA packet structure
typedef struct {
    uint8_t seq_number;         // Packet sequence number
    uint8_t data[PACKET_DATA_SIZE]; // Packet data
} OTA_Packet;

// Function prototypes
OTA_Status OTA_ParsePacket(uint8_t *buffer, OTA_Packet *packet);
//void WriteToFlash(uint32_t base_address, uint8_t *data, uint32_t seq_number);
HAL_StatusTypeDef WriteToFlash(uint32_t address, uint8_t *data, uint32_t size);
void EraseFlash(uint32_t start_address, uint32_t length);
void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart);

bool isFlashWritten(uint32_t startAddress, uint32_t endAddress);

#endif /* OTA_PROTOCOL_H */
