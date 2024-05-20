#include "ota_protocol.h"
#include "stm32f1xx_hal_def.h"

OTA_Status OTA_ParsePacket(uint8_t *buffer, OTA_Packet *packet) {
    // Check start marker
    if (buffer[0] != 0xAB || buffer[1] != 0xCD) {
        return OTA_ERROR_START_MARKER;
    }

    // Data packet number
    packet->seq_number = buffer[2];

    // Copy data
    for (int i = 0; i < PACKET_DATA_SIZE; i++) {
        packet->data[i] = buffer[3 + i];
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
    if (buffer[68] != 0xDC || buffer[69] != 0xBA) {
        return OTA_ERROR_END_MARKER;
    }

    return OTA_OK;
}

// Define UART handle
extern UART_HandleTypeDef huart1;

// Define buffer size for received data
#define RX_BUFFER_SIZE 100
static uint8_t rx_buffer[RX_BUFFER_SIZE];
static uint8_t rx_index = 0;

//// Function to write data to flash
//void WriteToFlash(uint32_t address, uint8_t *data, uint32_t size) {
//    HAL_FLASH_Unlock();

//    for (uint32_t i = 0; i < size; i += 4) {
//        HAL_FLASH_Program(FLASH_TYPEPROGRAM_WORD, address + i, *((uint32_t*)(data + i)));
//    }

//    HAL_FLASH_Lock();
//}

// 自定义擦除函数，擦除指定地址范围的 Flash 扇区
void EraseFlash(uint32_t start_address, uint32_t length) {
    HAL_StatusTypeDef status;
    FLASH_EraseInitTypeDef erase_init;
    uint32_t sector_error;

    // 初始化擦除参数
    erase_init.TypeErase = FLASH_TYPEERASE_PAGES;  // 使用页擦除方式
    erase_init.PageAddress = start_address;        // 起始地址
    erase_init.NbPages = length / FLASH_SECTOR_SIZE; // 擦除的页数

    // 擦除 Flash
    status = HAL_FLASHEx_Erase(&erase_init, &sector_error);
    
    // 检查擦除操作是否成功
    if (status != HAL_OK) {
        // 擦除失败
        // 可以在此处添加处理代码
    }
}



HAL_StatusTypeDef WriteToFlash(uint32_t address, uint8_t *data, uint32_t size) {
    HAL_StatusTypeDef status;
    
    HAL_FLASH_Unlock();
    
    // 计算要擦除的扇区数量
    uint32_t num_sectors_to_erase = (size + FLASH_SECTOR_SIZE - 1) / FLASH_SECTOR_SIZE; // 向上取整
    
    // 擦除相应数量的扇区
//    for (uint32_t i = 0; i < num_sectors_to_erase; ++i) {
//        EraseFlash(address + i * FLASH_SECTOR_SIZE, FLASH_SECTOR_SIZE);
//    }
    
    // 写入数据到 Flash
    for (uint32_t i = 0; i < size; i += 4) {
//        HAL_FLASH_Program(FLASH_TYPEPROGRAM_WORD, address + i, *((uint32_t*)(data + i)));
        status = HAL_FLASH_Program(FLASH_TYPEPROGRAM_WORD, address + i, *((uint32_t*)(data + i)));
        if (status != HAL_OK) {
            HAL_FLASH_Lock();
            return status;  // Return the failure status if writing fails
        }
    }

//    HAL_FLASH_Lock();
    HAL_FLASH_Lock();
    return HAL_OK;  // Return OK if all data written successfully

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

#define TOTAL_PACKETS 200  // 假设总共需要200个数据包完成更新  25kb * 1024 / 64

void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart) {
    // Process received byte
    rx_buffer[rx_index++] = huart->Instance->DR;

    // Check if a complete packet has been received
    if (rx_index >= PACKET_TOTAL_SIZE) {  // 检查是否接收到完整的数据包，每包总长度为70字节
        // Search for start marker
        for (int i = 0; i < rx_index - 1; i++) {
            if (rx_buffer[i] == 0xAB && rx_buffer[i + 1] == 0xCD) {
                // Found start marker, check if there is enough data for a complete packet
                if (rx_index - i >= PACKET_TOTAL_SIZE) {
                    // Parse received packet
                    OTA_Packet packet;
                    OTA_Status status = OTA_ParsePacket(&rx_buffer[i], &packet);
                    if (status == OTA_OK) {
//                        // Write data to flash
//                        WriteToFlash(FLASH_START_ADDR + packet.seq_number * PACKET_DATA_SIZE, packet.data, PACKET_DATA_SIZE);
//                        // Adjust buffer indices
//                        
                        if (WriteToFlash(FLASH_START_ADDR + packet.seq_number * PACKET_DATA_SIZE, packet.data, PACKET_DATA_SIZE) == HAL_OK) {
                            received_packets++;  // Only increment if write was successful
                            if (received_packets >= TOTAL_PACKETS) {
                                goto_application();
                            }
                        }
                        
                        rx_index -= (i + PACKET_TOTAL_SIZE);
                        for (int j = 0; j < rx_index; j++) {
                            rx_buffer[j] = rx_buffer[j + i + PACKET_TOTAL_SIZE];
                        }
                        // Restart parsing from the beginning of the buffer
                        i = -1;
                    }
                }
            }
        }
    }

    // Re-enable UART receive interrupt
    HAL_UART_Receive_IT(&huart1, &rx_buffer[rx_index], 1);
}

