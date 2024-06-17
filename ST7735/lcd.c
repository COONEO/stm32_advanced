//////////////////////////////////////////////////////////////////////////////////	 
//������ֻ��ѧϰʹ�ã�δ��������ɣ��������������κ���;
//����Ӳ������Ƭ��STM32F407ZGT6,����ԭ��Explorer STM32F4������,��Ƶ168MHZ������12MHZ
//QDtech-TFTҺ������ for STM32 IOģ��
//xiao��@ShenZhen QDtech co.,LTD
//��˾��վ:www.qdtft.com
//�Ա���վ��http://qdtech.taobao.com
//wiki������վ��http://www.lcdwiki.com
//��˾�ṩ����֧�֣��κμ������⻶ӭ��ʱ����ѧϰ
//�̻�(����) :+86 0755-23594567 
//�ֻ�:15989313508���빤�� 
//����:lcdwiki01@gmail.com    support@lcdwiki.com    goodtft@163.com 
//����֧��QQ:3002773612  3002778157
//��������QQȺ:324828016
//��������:2018/08/22
//�汾��V1.0
//��Ȩ���У�����ؾ���
//Copyright(C) ������ȫ�����Ӽ������޹�˾ 2018-2028
//All rights reserved
/****************************************************************************************************
//=========================================��Դ����================================================//
//     LCDģ��                STM32��Ƭ��
//      VCC          ��        DC5V/3.3V      //��Դ
//      GND          ��          GND          //��Դ��
//=======================================Һ���������߽���==========================================//
//��ģ��Ĭ��������������ΪSPI����
//     LCDģ��                STM32��Ƭ��    
//       SDA         ��          PB5          //Һ����SPI��������д�ź�
//=======================================Һ���������߽���==========================================//
//     LCDģ�� 					      STM32��Ƭ�� 
//       LED         ��          PB13         //Һ������������źţ��������Ҫ���ƣ���5V��3.3V
//       SCK         ��          PB3          //Һ����SPI����ʱ���ź�
//       A0          ��          PB14         //Һ��������/��������ź�
//       RESET       ��          PB12         //Һ������λ�����ź�
//       CS          ��          PB15         //Һ����Ƭѡ�����ź�
//=========================================������������=========================================//
//���ģ�鲻���������ܻ��ߴ��д������ܣ����ǲ���Ҫ�������ܣ�����Ҫ���д���������
//	   LCDģ��                STM32��Ƭ�� 
//      T_IRQ        ��          PB1          //�����������ж��ź�
//      T_DO         ��          PB2          //������SPI���߶��ź�
//      T_DIN        ��          PF11         //������SPI����д�ź�
//      T_CS         ��          PC5          //������Ƭѡ�����ź�
//      T_CLK        ��          PB0          //������SPI����ʱ���ź�
**************************************************************************************************/		
 /* @attention
  *
  * THE PRESENT FIRMWARE WHICH IS FOR GUIDANCE ONLY AIMS AT PROVIDING CUSTOMERS
  * WITH CODING INFORMATION REGARDING THEIR PRODUCTS IN ORDER FOR THEM TO SAVE
  * TIME. AS A RESULT, QD electronic SHALL NOT BE HELD LIABLE FOR ANY
  * DIRECT, INDIRECT OR CONSEQUENTIAL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING
  * FROM THE CONTENT OF SUCH FIRMWARE AND/OR THE USE MADE BY CUSTOMERS OF THE
  * CODING INFORMATION CONTAINED HEREIN IN CONNECTION WITH THEIR PRODUCTS.
**************************************************************************************************/	
#include "lcd.h"
#include "main.h" 
#include "spi.h"

	   
//����LCD��Ҫ����
//Ĭ��Ϊ����
_lcd_dev lcddev;

//������ɫ,������ɫ
u16 POINT_COLOR = 0x0000,BACK_COLOR = 0xFFFF;  
u16 DeviceCode;	 

/*****************************************************************************
 * @name       :void LCD_WR_REG(u8 data)
 * @date       :2018-08-09 
 * @function   :Write an 8-bit command to the LCD screen
 * @parameters :data:Command value to be written
 * @retvalue   :None
******************************************************************************/
void LCD_WR_REG(u8 data)
{ 
   LCD_CS_CLR();     
LCD_RS_CLR() ;	  
       HAL_SPI_Transmit_DMA(&hspi1, &data, 1);
//SPI_WriteByte(SPI1,data);
   //LCD_CS_SET();	
}

/*****************************************************************************
 * @name       :void LCD_WR_DATA(u8 data)
 * @date       :2018-08-09 
 * @function   :Write an 8-bit data to the LCD screen
 * @parameters :data:data value to be written
 * @retvalue   :None
******************************************************************************/
void LCD_WR_DATA(u8 data)
{
   LCD_CS_CLR();
	 LCD_RS_SET();
        HAL_SPI_Transmit_DMA(&hspi1, &data, 1);

   //SPI_WriteByte(SPI1,data);
   //LCD_CS_SET();
}

//void LCD_WR_REG(u8 data)
//{ 
//   LCD_CS_CLR();     
//   LCD_RS_CLR();  // ����ģʽ
//   uint8_t dataBuffer[2] = {0x00, data};  // �����ֽ�ǰ��Ҫһ��0�ֽ���Ϊǰ׺
//   HAL_SPI_Transmit_DMA(&hspi1, dataBuffer, 2);
//   // LCD_CS_SET();  // ������������CS�ߣ���DMA��ɻص�������
//}

//// �ú������ڷ������ݵ�LCD
//void LCD_WR_DATA(u8 data)
//{
//   LCD_CS_CLR();
//   LCD_RS_SET();  // ����ģʽ
//   uint8_t dataBuffer[2] = {0x00, data};  // �����ֽ�ǰ��Ҫһ��0�ֽ���Ϊǰ׺
//   HAL_SPI_Transmit_DMA(&hspi1, dataBuffer, 2);
//   // LCD_CS_SET();  // ������������CS�ߣ���DMA��ɻص�������
//}

/*****************************************************************************
 * @name       :void LCD_WriteReg(u8 LCD_Reg, u16 LCD_RegValue)
 * @date       :2018-08-09 
 * @function   :Write data into registers
 * @parameters :LCD_Reg:Register address
                LCD_RegValue:Data to be written
 * @retvalue   :None
******************************************************************************/
void LCD_WriteReg(u8 LCD_Reg, u16 LCD_RegValue)
{	
	LCD_WR_REG(LCD_Reg);  
	LCD_WR_DATA(LCD_RegValue);	    		 
}	   

/*****************************************************************************
 * @name       :void LCD_WriteRAM_Prepare(void)
 * @date       :2018-08-09 
 * @function   :Write GRAM
 * @parameters :None
 * @retvalue   :None
******************************************************************************/	 
void LCD_WriteRAM_Prepare(void)
{
	LCD_WR_REG(lcddev.wramcmd);
}	 

/*****************************************************************************
 * @name       :void Lcd_WriteData_16Bit(u16 Data)
 * @date       :2018-08-09 
 * @function   :Write an 16-bit command to the LCD screen
 * @parameters :Data:Data to be written
 * @retvalue   :None
******************************************************************************/	 
//void Lcd_WriteData_16Bit(u16 Data)
//{	
//   LCD_CS_CLR();
//   LCD_RS_SET();  
//    
//   SPI_WriteByte(SPI1,Data>>8);
//	 SPI_WriteByte(SPI1,Data);
//   LCD_CS_SET();
//}

void Lcd_WriteData_16Bit(uint16_t Data)
{
    uint8_t dataBuffer[2];  // ���ڴ�������ֽڵĻ�����

    LCD_CS_CLR();  // Ƭѡ�ź����ͣ�ѡ���豸
    LCD_RS_SET();  // ���ݼĴ���ѡ��

    // �ֽ�16λ���ݵ������ֽ�
    dataBuffer[0] = (Data >> 8) & 0xFF;  // ���ֽ�
    dataBuffer[1] = Data & 0xFF;         // ���ֽ�

    // ���������ֽڵ�����
    HAL_SPI_Transmit_DMA(&hspi1, dataBuffer, 2);

    // ע�⣺�����ڴ˴���������CS�ߣ���Ҫ�ڴ�����ɻص��д���
    // LCD_CS_SET();  // ��һ��Ӧ���Ƶ�DMA��������ж���
}



/*****************************************************************************
 * @name       :void LCD_DrawPoint(u16 x,u16 y)
 * @date       :2018-08-09 
 * @function   :Write a pixel data at a specified location
 * @parameters :x:the x coordinate of the pixel
                y:the y coordinate of the pixel
 * @retvalue   :None
******************************************************************************/	
void LCD_DrawPoint(u16 x,u16 y)
{
	LCD_SetCursor(x,y);//���ù��λ�� 
	Lcd_WriteData_16Bit(POINT_COLOR); 
}

void LCD_DrawPoint_color(u16 x, u16 y, u16 color)
{
    LCD_SetCursor(x, y); // ���ù��λ��
    Lcd_WriteData_16Bit(color); // д����ɫ����
}


/*****************************************************************************
 * @name       :void LCD_Clear(u16 Color)
 * @date       :2018-08-09 
 * @function   :Full screen filled LCD screen
 * @parameters :color:Filled color
 * @retvalue   :None
******************************************************************************/	
volatile uint32_t dma_transfer_complete = 1;  // 1��ʾ�������
// void LCD_Clear(u16 Color)
//{
//  unsigned int i,m;  
//	LCD_SetWindows(0,0,lcddev.width-1,lcddev.height-1);   
//	LCD_CS_CLR();
//	LCD_RS_SET();
//	for(i=0;i<lcddev.height;i++)
//	{
//        for(m=0;m<lcddev.width;m++)
//        {	
//          
//            uint8_t dataBuffer[2];  // ���ڴ�������ֽڵĻ�����

//            LCD_CS_CLR();  // Ƭѡ�ź����ͣ�ѡ���豸
//            LCD_RS_SET();  // ���ݼĴ���ѡ��

//            // �ֽ�16λ���ݵ������ֽ�
//            dataBuffer[0] = (Color >> 8) & 0xFF;  // ���ֽ�
//            dataBuffer[1] = Color & 0xFF;         // ���ֽ�

//            // ���������ֽڵ�����
//            HAL_SPI_Transmit_DMA(&hspi1, dataBuffer, 2);
//		}
//	}
//    

//	 //LCD_CS_SET();
//} 

//uint8_t lineBuffer[128 * 2 * 2];  // ÿ�����������ֽ�
//void LCD_Clear(u16 Color)
//{
//    unsigned int i, m;  
//    LCD_SetWindows(0, 0, lcddev.width-1, lcddev.height-1);   
//    LCD_CS_CLR();
//    LCD_RS_SET();

//    // Ԥ�������е���ɫ���ݣ���ֻ������
//    for (m = 0; m < lcddev.width * 2; m++)
//    {
//        int index = m * 2;
//        lineBuffer[index] = (Color >> 8) & 0xFF;  // ���ֽ�
//        lineBuffer[index + 1] = Color & 0xFF;     // ���ֽ�
//    }

//    // ����ÿ�У������δ���
//    for (i = 0; i < lcddev.height; i++)
//    {
//        
//        HAL_SPI_Transmit_DMA(&hspi1, lineBuffer, lcddev.width * 2);  // �������
//        while (!dma_transfer_complete);  // �ȴ� DMA �������
//        dma_transfer_complete = 0;       // ���� DMA ��ɱ�־
//    }

//    LCD_CS_SET();  // ֻ���������Ƭѡ�ź�
//}

uint8_t lineBuffer[128 * 2 * 2];  // ÿ��������Ҫ���� uint8_t���� uint16_t�����ܹ���������������

void LCD_Clear(uint16_t Color)
{
    unsigned int i, m;  
    LCD_SetWindows(0, 0, 128 - 1, 128 - 1);  // ������Ҫˢ�µĴ���
    LCD_CS_CLR();  // ���Ƭѡ�źţ���ʼͨ��
    LCD_RS_SET();  // ��������/�����־λΪ����

    // Ԥ�������е���ɫ���ݣ����� uint16_t ��ɫ�� uint8_t �����ʽ
    for (m = 0; m < 128; m++) {
        int index = m * 4;  // ÿ������ռ��4��uint8_tλ��
        lineBuffer[index] = (Color >> 8) & 0xFF;    // ���ֽ�
        lineBuffer[index + 1] = Color & 0xFF;       // ���ֽ�
        lineBuffer[index + 2] = (Color >> 8) & 0xFF; // ���ֽڣ��ظ�
        lineBuffer[index + 3] = Color & 0xFF;       // ���ֽڣ��ظ�
    }

    // ����ÿ�У���������������
    for (i = 0; i < 128; i++) {
        HAL_SPI_Transmit_DMA(&hspi1, lineBuffer, 128 * 2); // �����������ݣ���������ȷ����Ч
        while (!dma_transfer_complete);  // �ȴ� DMA �������
        dma_transfer_complete = 0;       // ���� DMA ��ɱ�־
    }
    


    LCD_CS_SET();  // ������ɺ�����Ƭѡ�źţ�����ͨ��
}

void LCD_Clear_erea(int x1,int y1, int x2,int y2  ,uint16_t Color)
{
    unsigned int i, m;  
    
    
    int width = x2 - x1 + 1;   // ����������
    int height = y2 - y1 + 1;  // ��������߶�
    
    LCD_SetWindows(x1, y1, x2, y2);  // ������Ҫˢ�µĴ���
    LCD_CS_CLR();  // ���Ƭѡ�źţ���ʼͨ��
    LCD_RS_SET();  // ��������/�����־λΪ����

    // Ԥ�������е���ɫ���ݣ����� uint16_t ��ɫ�� uint8_t �����ʽ
    for (m = 0; m < width; m++) {
        int index = m * 4;  // ÿ������ռ��4��uint8_tλ��
        lineBuffer[index] = (Color >> 8) & 0xFF;    // ���ֽ�
        lineBuffer[index + 1] = Color & 0xFF;       // ���ֽ�
        lineBuffer[index + 2] = (Color >> 8) & 0xFF; // ���ֽڣ��ظ�
        lineBuffer[index + 3] = Color & 0xFF;       // ���ֽڣ��ظ�
    }

    // ����ÿ�У���������������
    for (i = 0; i < height; i++) {
        HAL_SPI_Transmit_DMA(&hspi1, lineBuffer, width * 2); // �����������ݣ���������ȷ����Ч
        while (!dma_transfer_complete);  // �ȴ� DMA �������
        dma_transfer_complete = 0;       // ���� DMA ��ɱ�־
    }

    LCD_CS_SET();  // ������ɺ�����Ƭѡ�źţ�����ͨ��
}

//void LCD_Clear_area_LVGL(int x1, int y1, int x2, int y2, uint8_t* Colors){
//    unsigned int i, m;
//    int width = x2 - x1 + 1;   // ����������
//    int height = y2 - y1 + 1;  // ��������߶�

//    LCD_SetWindows(x1, y1, x2, y2);  // ������Ҫˢ�µĴ���
//    LCD_CS_CLR();  // ���Ƭѡ�źţ���ʼͨ��
//    LCD_RS_SET();  // ��������/�����־λΪ����

//    // ����ÿ�У�����������������
//    for (i = 0; i < height; i++) {
//        uint8_t* lineBuffer = (uint8_t*)malloc(width * 4);
//        if (lineBuffer == NULL) {
//            continue;  // ����ڴ����ʧ�ܣ�������ǰ��
//        }

//        for (m = 0; m < width; m++) {
//            int index = m * 4;  // ÿ������ռ��2��uint8_tλ��
//            lineBuffer[index] = (Colors[i + m] >> 8) & 0xFF;    // ���ֽ�
//            lineBuffer[index + 1] = Colors[i  + m] & 0xFF;       // ���ֽ�
//            lineBuffer[index + 2] = (Colors[i  + m] >> 8) & 0xFF;    // ���ֽ�
//            lineBuffer[index + 3] = Colors[i  + m] & 0xFF;       // ���ֽ�
//        }

//        HAL_SPI_Transmit_DMA(&hspi1, lineBuffer, width * 2); // ������������
//        while (!dma_transfer_complete);  // �ȴ� DMA �������
//        dma_transfer_complete = 0;       // ���� DMA ��ɱ�־

//        free(lineBuffer);  // �ͷŵ�ǰ�еĻ�����
//    }

//    LCD_CS_SET();  // ������ɺ�����Ƭѡ�źţ�����ͨ��
//}

void LCD_Clear_area_LVGL(int x1, int y1, int x2, int y2, uint8_t* Colors) {
    unsigned int i, m;
    int width = x2 - x1 + 1;   // Calculate the width of the area
    int height = y2 - y1 + 1;  // Calculate the height of the area

    LCD_SetWindows(x1, y1, x2, y2);  // Set the window to refresh
    LCD_CS_CLR();  // Clear the chip select signal to start communication
    LCD_RS_SET();  // Set the register select to data

    // For each row, process and send the entire row of data
    for (i = 0; i < height; i++) {
        uint8_t* lineBuffer = (uint8_t*)malloc(width * 4);
        if (lineBuffer == NULL) {
            continue;  // If memory allocation fails, skip the current row
        }

        for (m = 0; m < width; m++) {
            uint16_t color = ((uint16_t*)Colors)[i * width + m]; // Correct indexing into the color array
            int index = m * 4;  // Each pixel occupies 4 uint8_t positions (for RGB redundancy)
            lineBuffer[index] = (color >> 8) & 0xFF;    // High byte
            lineBuffer[index + 1] = color & 0xFF;       // Low byte
            lineBuffer[index + 2] = (color >> 8) & 0xFF; // High byte repeated
            lineBuffer[index + 3] = color & 0xFF;       // Low byte repeated
        }

        HAL_SPI_Transmit_DMA(&hspi1, lineBuffer, width * 2); // Send the entire row of data
        while (!dma_transfer_complete);  // Wait for DMA transfer to complete
        dma_transfer_complete = 0;       // Reset the DMA completion flag

        free(lineBuffer);  // Free the current row buffer
    }

    LCD_CS_SET();  // Set the chip select signal to end communication
}






//uint8_t lineBuffer[128 * 2];  // ÿ��������Ҫ���� uint8_t���� uint16_t��

//void LCD_Clear(uint16_t Color)
//{
//    unsigned int i, m;  
//    LCD_SetWindows(0, 0, lcddev.width - 1, lcddev.height - 1);  // ������Ҫˢ�µĴ���
//    LCD_CS_CLR();  // ���Ƭѡ�źţ���ʼͨ��
//    LCD_RS_SET();  // ��������/�����־λΪ����

//    // Ԥ�������е���ɫ���ݣ����� uint16_t ��ɫ�� uint8_t �����ʽ
//    for (m = 0; m < lcddev.width; m++) {
//        int index = m * 2;  // ÿ������ռ��2��uint8_tλ��
//        lineBuffer[index] = (Color >> 8) & 0xFF;    // ���ֽ�
//        lineBuffer[index + 1] = Color & 0xFF;       // ���ֽ�
//    }

//    // ����ÿ�У���������������
//    for (i = 0; i < lcddev.height; i++) {
//        HAL_SPI_Transmit_DMA(&hspi1, lineBuffer, lcddev.width * 2); // �����������ݣ��ܳ���Ϊ lcddev.width * 2 �ֽ�
//        while (!dma_transfer_complete);  // �ȴ� DMA �������
//        dma_transfer_complete = 0;       // ���� DMA ��ɱ�־
//    }

//    LCD_CS_SET();  // ������ɺ�����Ƭѡ�źţ�����ͨ��
//}




/*****************************************************************************
 * @name       :void LCD_Clear(u16 Color)
 * @date       :2018-08-09 
 * @function   :Initialization LCD screen GPIO
 * @parameters :None
 * @retvalue   :None
******************************************************************************/	
void LCD_GPIOInit(void)
{
//	GPIO_InitTypeDef  GPIO_InitStructure;
//	      
//	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB ,ENABLE);
//	
//	GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_12| GPIO_Pin_13|GPIO_Pin_14| GPIO_Pin_15;
//	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;//��ͨ���ģʽ
//	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
//	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;   //�������
//	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;//����
//	GPIO_Init(GPIOB, &GPIO_InitStructure);
}

/*****************************************************************************
 * @name       :void LCD_RESET(void)
 * @date       :2018-08-09 
 * @function   :Reset LCD screen
 * @parameters :None
 * @retvalue   :None
******************************************************************************/	
void LCD_RESET(void)
{
	LCD_RST_CLR();
	HAL_Delay(100);	
	LCD_RST_SET();
	HAL_Delay(50);
}

/*****************************************************************************
 * @name       :void LCD_RESET(void)
 * @date       :2018-08-09 
 * @function   :Initialization LCD screen
 * @parameters :None
 * @retvalue   :None
******************************************************************************/	 	 
void LCD_Init(void)
{  
//	SPI1_Init(); //Ӳ��SPI��ʼ��
    LCD_CS_CLR();
//	SPI_SetSpeed(SPI1,SPI_BaudRatePrescaler_2);
	LCD_GPIOInit();//LCD GPIO��ʼ��										 
 	LCD_RESET(); //LCD ��λ
//************* ST7735S��ʼ��**********//	
	LCD_WR_REG(0x11);//Sleep exit 
	HAL_Delay (120);	
	//ST7735R Frame Rate
	LCD_WR_REG(0xB1); 
	LCD_WR_DATA(0x01); 
	LCD_WR_DATA(0x2C); 
	LCD_WR_DATA(0x2D); 
	LCD_WR_REG(0xB2); 
	LCD_WR_DATA(0x01); 
	LCD_WR_DATA(0x2C); 
	LCD_WR_DATA(0x2D); 
	LCD_WR_REG(0xB3); 
	LCD_WR_DATA(0x01); 
	LCD_WR_DATA(0x2C); 
	LCD_WR_DATA(0x2D); 
	LCD_WR_DATA(0x01); 
	LCD_WR_DATA(0x2C); 
	LCD_WR_DATA(0x2D); 	
	LCD_WR_REG(0xB4); //Column inversion 
	LCD_WR_DATA(0x07); 	
	//ST7735R Power Sequence
	LCD_WR_REG(0xC0); 
	LCD_WR_DATA(0xA2); 
	LCD_WR_DATA(0x02); 
	LCD_WR_DATA(0x84); 
	LCD_WR_REG(0xC1); 
	LCD_WR_DATA(0xC5); 
	LCD_WR_REG(0xC2); 
	LCD_WR_DATA(0x0A); 
	LCD_WR_DATA(0x00); 
	LCD_WR_REG(0xC3); 
	LCD_WR_DATA(0x8A); 
	LCD_WR_DATA(0x2A); 
	LCD_WR_REG(0xC4); 
	LCD_WR_DATA(0x8A); 
	LCD_WR_DATA(0xEE); 
	LCD_WR_REG(0xC5); //VCOM 
	LCD_WR_DATA(0x0E); 	
	LCD_WR_REG(0x36); //MX, MY, RGB mode 
	LCD_WR_DATA(0xC8); 
	//ST7735R Gamma Sequence
	LCD_WR_REG(0xe0); 
	LCD_WR_DATA(0x0f); 
	LCD_WR_DATA(0x1a); 
	LCD_WR_DATA(0x0f); 
	LCD_WR_DATA(0x18); 
	LCD_WR_DATA(0x2f); 
	LCD_WR_DATA(0x28); 
	LCD_WR_DATA(0x20); 
	LCD_WR_DATA(0x22); 
	LCD_WR_DATA(0x1f); 
	LCD_WR_DATA(0x1b); 
	LCD_WR_DATA(0x23); 
	LCD_WR_DATA(0x37); 
	LCD_WR_DATA(0x00); 	
	LCD_WR_DATA(0x07); 
	LCD_WR_DATA(0x02); 
	LCD_WR_DATA(0x10); 
	LCD_WR_REG(0xe1); 
	LCD_WR_DATA(0x0f); 
	LCD_WR_DATA(0x1b); 
	LCD_WR_DATA(0x0f); 
	LCD_WR_DATA(0x17); 
	LCD_WR_DATA(0x33); 
	LCD_WR_DATA(0x2c); 
	LCD_WR_DATA(0x29); 
	LCD_WR_DATA(0x2e); 
	LCD_WR_DATA(0x30); 
	LCD_WR_DATA(0x30); 
	LCD_WR_DATA(0x39); 
	LCD_WR_DATA(0x3f); 
	LCD_WR_DATA(0x00); 
	LCD_WR_DATA(0x07); 
	LCD_WR_DATA(0x03); 
	LCD_WR_DATA(0x10);  	
	LCD_WR_REG(0x2a);
	LCD_WR_DATA(0x00);
	LCD_WR_DATA(0x00);
	LCD_WR_DATA(0x00);
	LCD_WR_DATA(0x7f);
	LCD_WR_REG(0x2b);
	LCD_WR_DATA(0x00);
	LCD_WR_DATA(0x00);
	LCD_WR_DATA(0x00);
	LCD_WR_DATA(0x9f);
	LCD_WR_REG(0xF0); //Enable test command  
	LCD_WR_DATA(0x01); 
	LCD_WR_REG(0xF6); //Disable ram power save mode 
	LCD_WR_DATA(0x00); 	
	LCD_WR_REG(0x3A); //65k mode 
	LCD_WR_DATA(0x05); 
	LCD_WR_REG(0x29);//Display on	 	
  LCD_direction(USE_HORIZONTAL);//����LCD��ʾ����
	LCD_LED_ON(); //LCD_LED=1;//��������	 
	LCD_Clear(WHITE);//��ȫ����ɫ
}
 
/*****************************************************************************
 * @name       :void LCD_SetWindows(u16 xStar, u16 yStar,u16 xEnd,u16 yEnd)
 * @date       :2018-08-09 
 * @function   :Setting LCD display window
 * @parameters :xStar:the bebinning x coordinate of the LCD display window
								yStar:the bebinning y coordinate of the LCD display window
								xEnd:the endning x coordinate of the LCD display window
								yEnd:the endning y coordinate of the LCD display window
 * @retvalue   :None
******************************************************************************/ 
void LCD_SetWindows(u16 xStar, u16 yStar,u16 xEnd,u16 yEnd)
{	
	LCD_WR_REG(lcddev.setxcmd);	
	LCD_WR_DATA(0x00);
	LCD_WR_DATA(xStar+lcddev.xoffset);		
	LCD_WR_DATA(0x00);
	LCD_WR_DATA(xEnd+lcddev.xoffset);

	LCD_WR_REG(lcddev.setycmd);	
	LCD_WR_DATA(0x00);
	LCD_WR_DATA(yStar+lcddev.yoffset);		
	LCD_WR_DATA(0x00);
	LCD_WR_DATA(yEnd+lcddev.yoffset);

	LCD_WriteRAM_Prepare();	//��ʼд��GRAM			
}   

/*****************************************************************************
 * @name       :void LCD_SetCursor(u16 Xpos, u16 Ypos)
 * @date       :2018-08-09 
 * @function   :Set coordinate value
 * @parameters :Xpos:the  x coordinate of the pixel
								Ypos:the  y coordinate of the pixel
 * @retvalue   :None
******************************************************************************/ 
void LCD_SetCursor(u16 Xpos, u16 Ypos)
{	  	    			
	LCD_SetWindows(Xpos,Ypos,Xpos,Ypos);	
} 

/*****************************************************************************
 * @name       :void LCD_direction(u8 direction)
 * @date       :2018-08-09 
 * @function   :Setting the display direction of LCD screen
 * @parameters :direction:0-0 degree
                          1-90 degree
													2-180 degree
													3-270 degree
 * @retvalue   :None
******************************************************************************/ 
void LCD_direction(u8 direction)
{ 
			lcddev.setxcmd=0x2A;
			lcddev.setycmd=0x2B;
			lcddev.wramcmd=0x2C;
	switch(direction){		  
		case 0:						 	 		
			lcddev.width=LCD_W;
			lcddev.height=LCD_H;	
			lcddev.xoffset=2;
			lcddev.yoffset=3;
			LCD_WriteReg(0x36,(1<<3)|(1<<6)|(1<<7));//BGR==1,MY==0,MX==0,MV==0
		break;
		case 1:
			lcddev.width=LCD_H;
			lcddev.height=LCD_W;
			lcddev.xoffset=3;
			lcddev.yoffset=2;
			LCD_WriteReg(0x36,(1<<3)|(1<<7)|(1<<5));//BGR==1,MY==1,MX==0,MV==1
		break;
		case 2:						 	 		
			lcddev.width=LCD_W;
			lcddev.height=LCD_H;
      lcddev.xoffset=2;
			lcddev.yoffset=1;			
			LCD_WriteReg(0x36,(1<<3)|(0<<6)|(0<<7));//BGR==1,MY==0,MX==0,MV==0
		break;
		case 3:
			lcddev.width=LCD_H;
			lcddev.height=LCD_W;
			lcddev.xoffset=1;
			lcddev.yoffset=2;
			LCD_WriteReg(0x36,(1<<3)|(0<<7)|(1<<6)|(1<<5));//BGR==1,MY==1,MX==0,MV==1
		break;	
		default:break;
	}		
}	 

//volatile uint32_t dma_transfer_complete = 1;
void HAL_SPI_TxCpltCallback(SPI_HandleTypeDef *hspi)
{
    if (hspi->Instance == SPI1) {
        //LCD_CS_SET();  // �����ݴ�����ɺ�Ƭѡ�ź��ø�
        dma_transfer_complete = 1;  // ���DMA�������
    }
}


