#include "ota_protocol.h"

OTA_Status OTA_ParsePacket(uint8_t *buffer, OTA_Packet *packet) {
    // Check start marker
    if ((buffer[0] != (PACKET_START_MARKER >> 8)) || (buffer[1] != (PACKET_START_MARKER & 0xFF))) {
        return OTA_ERROR_START_MARKER;
    }

    // Check data length
    if (buffer[2] != PACKET_DATA_SIZE) {
        return OTA_ERROR_DATA_LENGTH;
    }

    // Copy data
    for (int i = 0; i < PACKET_DATA_SIZE; i++) {
        packet->data[i] = buffer[3 + i];
    }

//    // Calculate checksum
//    uint8_t checksum = 0;
//    for (int i = 0; i < PACKET_DATA_SIZE; i++) {
//        checksum += packet->data[i];
//    }

//    // Check checksum
//    if (checksum != buffer[11]) {
//        return OTA_ERROR_CHECKSUM;
//    }

    // Check end marker
//    if ((buffer[12] != (PACKET_END_MARKER >> 8)) || (buffer[13] != (PACKET_END_MARKER & 0xFF))) {
//        return OTA_ERROR_END_MARKER;
//    }
    if (buffer[12] != 0xDC || buffer[13] != 0xBA) {
        return OTA_ERROR_END_MARKER;
    }


    return OTA_OK;
}


// Define flash address range
#define FLASH_START_ADDR 0x08003800
#define FLASH_END_ADDR   0x08006400

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

// �Զ����������������ָ����ַ��Χ�� Flash ����
void EraseFlash(uint32_t start_address, uint32_t length) {
    HAL_StatusTypeDef status;
    FLASH_EraseInitTypeDef erase_init;
    uint32_t sector_error;

    // ��ʼ����������
    erase_init.TypeErase = FLASH_TYPEERASE_PAGES;  // ʹ��ҳ������ʽ
    erase_init.PageAddress = start_address;        // ��ʼ��ַ
    erase_init.NbPages = length / FLASH_SECTOR_SIZE; // ������ҳ��

    // ���� Flash
    status = HAL_FLASHEx_Erase(&erase_init, &sector_error);
    
    // �����������Ƿ�ɹ�
    if (status != HAL_OK) {
        // ����ʧ��
        // �����ڴ˴���Ӵ������
    }
}

void WriteToFlash(uint32_t address, uint8_t *data, uint32_t size) {
    HAL_FLASH_Unlock();
    
    // ����Ҫ��������������
    uint32_t num_sectors_to_erase = (size + FLASH_SECTOR_SIZE - 1) / FLASH_SECTOR_SIZE; // ����ȡ��
    
    // ������Ӧ����������
//    for (uint32_t i = 0; i < num_sectors_to_erase; ++i) {
//        EraseFlash(address + i * FLASH_SECTOR_SIZE, FLASH_SECTOR_SIZE);
//    }
    
    // д�����ݵ� Flash
    for (uint32_t i = 0; i < size; i += 4) {
        HAL_FLASH_Program(FLASH_TYPEPROGRAM_WORD, address + i, *((uint32_t*)(data + i)));
    }

    HAL_FLASH_Lock();
}

int yy = 0;
void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart) {
    // Process received byte
    rx_buffer[rx_index++] = huart->Instance->DR;

    // Check if a complete packet has been received
    if (rx_index >= 14) {
        // Search for start marker
        for (int i = 0; i < rx_index - 1; i++) {
            if (rx_buffer[i] == 0xAB && rx_buffer[i + 1] == 0xCD) {
                // Found start marker, check if there is enough data for a complete packet
                if (rx_index - i >= 14) {
                    // Parse received packet
                    OTA_Packet packet;
                    OTA_Status status = OTA_ParsePacket(&rx_buffer[i], &packet);
                    if (status == OTA_OK) {
                        // Write data to flash
                        WriteToFlash(FLASH_START_ADDR+ yy *8, packet.data, PACKET_DATA_SIZE);
                        // Adjust buffer indices
                        rx_index -= (i + 14);
                        for (int j = 0; j < rx_index; j++) {
                            rx_buffer[j] = rx_buffer[j + i + 14];
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
