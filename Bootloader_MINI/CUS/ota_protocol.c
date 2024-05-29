#include "ota_protocol.h"
#include "stdlib.h"
#include "string.h"

//#include "stm32f1xx_hal_def.h"

OTA_Status OTA_ParsePacket(uint8_t *buffer, OTA_Packet *packet) {
    // Check start marker
    if (buffer[0] != 0xAB || buffer[1] != 0xCD) {
        return OTA_ERROR_START_MARKER;
    }

    // Data packet number
    //packet->seq_number = buffer[2]; //buffer[3];
    
    // Data packet number (combining buffer[2] and buffer[3])
    packet->seq_number = (buffer[2] << 8) | buffer[3];

    // Copy data
    for (int i = 0; i < PACKET_DATA_SIZE; i++) {
        packet->data[i] = buffer[4 + i];
    }

    // Calculate checksum
    uint8_t checksum = 0;
    for (int i = 0; i < PACKET_DATA_SIZE; i++) {
        checksum += packet->data[i];
    }

    // Check checksum
//    if (checksum != buffer[67]) {
//        return OTA_ERROR_CHECKSUM;
//    }

    // Check end marker
    if (buffer[70] != 0xDC || buffer[71] != 0xBA) {
        return OTA_ERROR_END_MARKER;
    }

    return OTA_OK;
}

// Define UART handle
extern UART_HandleTypeDef huart1;

// Define buffer size for received data
#define RX_BUFFER_SIZE 300
static uint8_t rx_buffer[RX_BUFFER_SIZE];
static uint8_t rx_index = 0;

HAL_StatusTypeDef WriteToFlash(uint32_t address, uint8_t *data, uint32_t size) {
    HAL_StatusTypeDef status;
    uint32_t Address = address;
    uint32_t EndAddress = address + size;

    // Unlock the Flash to enable the flash control register access
    HAL_FLASH_Unlock();

    // Program the user Flash area word by word
    while (Address < EndAddress) {
        status = HAL_FLASH_Program(FLASH_TYPEPROGRAM_WORD, Address, *((uint32_t*)data));
        if (status != HAL_OK) {
            // Lock the Flash to disable the flash control register access
            HAL_FLASH_Lock();
            return status; // Return the failure status if writing fails
        }
        Address += 4;
        data += 4;
    }

    // Lock the Flash to disable the flash control register access
    HAL_FLASH_Lock();

    return HAL_OK; // Return OK if all data written successfully
}

HAL_StatusTypeDef EraseFlashSectors(uint32_t startAddress, uint32_t size) {
    HAL_StatusTypeDef status;

    // Unlock the Flash to enable the flash control register access
    HAL_FLASH_Unlock();

    FLASH_EraseInitTypeDef EraseInitStruct;
    uint32_t SectorError;

    // Find the start sector
    uint32_t StartSector = GetSector(startAddress);
    uint32_t EndSector = GetSector(startAddress + size - 1);

    EraseInitStruct.TypeErase = FLASH_TYPEERASE_SECTORS;
    EraseInitStruct.VoltageRange = FLASH_VOLTAGE_RANGE_3;
    EraseInitStruct.Sector = StartSector;
    EraseInitStruct.NbSectors = EndSector - StartSector + 1;

    status = HAL_FLASHEx_Erase(&EraseInitStruct, &SectorError);
    if (status != HAL_OK) {
        // Lock the Flash to disable the flash control register access
        HAL_FLASH_Lock();
        return status; // Return the failure status if erasing fails
    }

    // Lock the Flash to disable the flash control register access
    HAL_FLASH_Lock();

    return HAL_OK; // Return OK if all sectors erased successfully
}

// Define the addresses of the sectors
#define ADDR_FLASH_SECTOR_0     ((uint32_t)0x08000000) // Base address of Sector 0, 16 Kbytes
#define ADDR_FLASH_SECTOR_1     ((uint32_t)0x08004000) // Base address of Sector 1, 16 Kbytes
#define ADDR_FLASH_SECTOR_2     ((uint32_t)0x08008000) // Base address of Sector 2, 16 Kbytes
#define ADDR_FLASH_SECTOR_3     ((uint32_t)0x0800C000) // Base address of Sector 3, 16 Kbytes
#define ADDR_FLASH_SECTOR_4     ((uint32_t)0x08010000) // Base address of Sector 4, 64 Kbytes
#define ADDR_FLASH_SECTOR_5     ((uint32_t)0x08020000) // Base address of Sector 5, 128 Kbytes
#define ADDR_FLASH_SECTOR_6     ((uint32_t)0x08040000) // Base address of Sector 6, 128 Kbytes
#define ADDR_FLASH_SECTOR_7     ((uint32_t)0x08060000) // Base address of Sector 7, 128 Kbytes  total end
#define ADDR_FLASH_SECTOR_8     ((uint32_t)0x08080000) // Base address of Sector 8, 128 Kbytes  end address
//#define ADDR_FLASH_SECTOR_9     ((uint32_t)0x080A0000) // Base address of Sector 9, 128 Kbytes
//#define ADDR_FLASH_SECTOR_10    ((uint32_t)0x080C0000) // Base address of Sector 10, 128 Kbytes
//#define ADDR_FLASH_SECTOR_11    ((uint32_t)0x080E0000) // Base address of Sector 11, 128 Kbytes

// Function to get the sector number based on the address
uint32_t GetSector(uint32_t Address) {
    uint32_t sector = 0;

    if ((Address < ADDR_FLASH_SECTOR_1) && (Address >= ADDR_FLASH_SECTOR_0)) {
        sector = FLASH_SECTOR_0;
    } else if ((Address < ADDR_FLASH_SECTOR_2) && (Address >= ADDR_FLASH_SECTOR_1)) {
        sector = FLASH_SECTOR_1;
    } else if ((Address < ADDR_FLASH_SECTOR_3) && (Address >= ADDR_FLASH_SECTOR_2)) {
        sector = FLASH_SECTOR_2;
    } else if ((Address < ADDR_FLASH_SECTOR_4) && (Address >= ADDR_FLASH_SECTOR_3)) {
        sector = FLASH_SECTOR_3;
    } else if ((Address < ADDR_FLASH_SECTOR_5) && (Address >= ADDR_FLASH_SECTOR_4)) {
        sector = FLASH_SECTOR_4;
    } else if ((Address < ADDR_FLASH_SECTOR_6) && (Address >= ADDR_FLASH_SECTOR_5)) {
        sector = FLASH_SECTOR_5;
    } else if ((Address < ADDR_FLASH_SECTOR_7) && (Address >= ADDR_FLASH_SECTOR_6)) {
        sector = FLASH_SECTOR_6;
    } else if ((Address < ADDR_FLASH_SECTOR_8) && (Address >= ADDR_FLASH_SECTOR_7)) {
        sector = FLASH_SECTOR_7;
//    } else if ((Address < ADDR_FLASH_SECTOR_9) && (Address >= ADDR_FLASH_SECTOR_8)) {
//        sector = FLASH_SECTOR_8;
//    } else if ((Address < ADDR_FLASH_SECTOR_10) && (Address >= ADDR_FLASH_SECTOR_9)) {
//        sector = FLASH_SECTOR_9;
//    } else if ((Address < ADDR_FLASH_SECTOR_11) && (Address >= ADDR_FLASH_SECTOR_10)) {
//        sector = FLASH_SECTOR_10;
//    } else {
//        sector = FLASH_SECTOR_11;
    }

    return sector;
}


bool isFlashWritten(uint32_t startAddress, uint32_t endAddress) {
    while (startAddress < endAddress) {
        if (*(volatile uint32_t*)startAddress != 0xFFFFFFFF) {
            return true;  // 发现非擦除的数据，表示已写入数据
        }
        startAddress += 4;  // 增加地址步长，假设Flash字大小为4字节
    }
    return false;  // 全部是擦除状态，无数据写入
}

int yy = 0;

volatile uint32_t received_packets = 0;

#define TOTAL_PACKETS 300  // 假设总共需要200个数据包完成更新  25kb * 1024 / 64

void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart) {
    // Process received byte
    rx_buffer[rx_index++] = huart->Instance->DR;
    
    if(rx_index>=6)
    {
        for (int i = 0; i <= rx_index - 6; i++) {
            if (rx_buffer[i] == 0xAB && rx_buffer[i + 1] == 0xCC && rx_buffer[i + 4] == 0xCC && rx_buffer[i + 5] == 0xBA) {
                // Extract address from the command
                uint32_t address = (rx_buffer[i + 2] << 8) | rx_buffer[i + 3];
                address = FLASH_START_ADDR + address * PACKET_DATA_SIZE;

                // Erase the sector containing the specified address
                if (EraseFlashSectors(address, PACKET_DATA_SIZE) == HAL_OK) {
                    // Handle successful erase (e.g., log the success, etc.)
                }
                rx_index =0;
                memset(rx_buffer, 0, sizeof(rx_buffer));
                break;
            }
        }
    }
    
    // Check if a complete packet has been received
    if (rx_index >= PACKET_TOTAL_SIZE) {  // 检查是否接收到完整的数据包，每包总长度为72字节
        // Search for start marker
        for (int i = 0; i <= rx_index - 72; i++) {
            if (rx_buffer[i] == 0xAB && rx_buffer[i + 1] == 0xCD) {
                // Found start marker, check if there is enough data for a complete packet
                if (rx_index - i >= PACKET_TOTAL_SIZE) {
                    // Parse received packet
                    OTA_Packet packet;
                    OTA_Status status = OTA_ParsePacket(&rx_buffer[i], &packet);
                    if (status == OTA_OK) {
                        // Write data to flash
                        if (WriteToFlash(FLASH_START_ADDR + packet.seq_number * PACKET_DATA_SIZE, packet.data, PACKET_DATA_SIZE) == HAL_OK) {
                            received_packets++;  // Only increment if write was successful
                            if (received_packets >= TOTAL_PACKETS) {
                                goto_application();
                            }
                        }

                        // Clear rx_buffer and reset rx_index
                        rx_index = 0;
                        memset(rx_buffer, 0, sizeof(rx_buffer));

                        // Restart parsing from the beginning of the buffer
                        break;
                    } else {
                        // Handle parsing error (e.g., log the error, reset index, etc.)
                        rx_index = 0;
                        memset(rx_buffer, 0, sizeof(rx_buffer));
                    }

                    // Restart parsing from the beginning of the buffer
                    break;
                }
            }
        }
    }
    


    // Re-enable UART receive interrupt
    HAL_UART_Receive_IT(huart, &rx_buffer[rx_index], 1);
}





#define OTA_TOTAL_SIZE 0x60000
void StartOTAUpdate() {
    // 假设 OTA 数据写入从 FLASH_START_ADDR 开始，总大小为 OTA_TOTAL_SIZE
    if (EraseFlashSectors(FLASH_START_ADDR, OTA_TOTAL_SIZE) != HAL_OK) {
        // 处理擦除失败的情况
    }
    
    // 初始化接收数据包的变量
    received_packets = 0;
    rx_index = 0;

    // 开始接收第一个数据包
    HAL_UART_Receive_IT(&huart1, rx_buffer, 1);
}

