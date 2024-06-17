//////////////////////////////////////////////////////////////////////////////////	 
//本程序只供学习使用，未经作者许可，不得用于其它任何用途
//测试硬件：单片机STM32F407ZGT6,正点原子Explorer STM32F4开发板,主频168MHZ，晶振12MHZ
//QDtech-TFT液晶驱动 for STM32 IO模拟
//xiao冯@ShenZhen QDtech co.,LTD
//公司网站:www.qdtft.com
//淘宝网站：http://qdtech.taobao.com
//wiki技术网站：http://www.lcdwiki.com
//我司提供技术支持，任何技术问题欢迎随时交流学习
//固话(传真) :+86 0755-23594567 
//手机:15989313508（冯工） 
//邮箱:lcdwiki01@gmail.com    support@lcdwiki.com    goodtft@163.com 
//技术支持QQ:3002773612  3002778157
//技术交流QQ群:324828016
//创建日期:2018/08/22
//版本：V1.0
//版权所有，盗版必究。
//Copyright(C) 深圳市全动电子技术有限公司 2018-2028
//All rights reserved
/****************************************************************************************************
//=========================================电源接线================================================//
//     LCD模块                STM32单片机
//      VCC          接        DC5V/3.3V      //电源
//      GND          接          GND          //电源地
//=======================================液晶屏数据线接线==========================================//
//本模块默认数据总线类型为SPI总线
//     LCD模块                STM32单片机    
//       SDA         接          PB5          //液晶屏SPI总线数据写信号
//=======================================液晶屏控制线接线==========================================//
//     LCD模块 					      STM32单片机 
//       LED         接          PB13         //液晶屏背光控制信号，如果不需要控制，接5V或3.3V
//       SCK         接          PB3          //液晶屏SPI总线时钟信号
//       A0          接          PB14         //液晶屏数据/命令控制信号
//       RESET       接          PB12         //液晶屏复位控制信号
//       CS          接          PB15         //液晶屏片选控制信号
//=========================================触摸屏触接线=========================================//
//如果模块不带触摸功能或者带有触摸功能，但是不需要触摸功能，则不需要进行触摸屏接线
//	   LCD模块                STM32单片机 
//      T_IRQ        接          PB1          //触摸屏触摸中断信号
//      T_DO         接          PB2          //触摸屏SPI总线读信号
//      T_DIN        接          PF11         //触摸屏SPI总线写信号
//      T_CS         接          PC5          //触摸屏片选控制信号
//      T_CLK        接          PB0          //触摸屏SPI总线时钟信号
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

	   
//管理LCD重要参数
//默认为竖屏
_lcd_dev lcddev;

//画笔颜色,背景颜色
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
//   LCD_RS_CLR();  // 命令模式
//   uint8_t dataBuffer[2] = {0x00, data};  // 命令字节前需要一个0字节作为前缀
//   HAL_SPI_Transmit_DMA(&hspi1, dataBuffer, 2);
//   // LCD_CS_SET();  // 不在这里设置CS高，由DMA完成回调来处理
//}

//// 该函数用于发送数据到LCD
//void LCD_WR_DATA(u8 data)
//{
//   LCD_CS_CLR();
//   LCD_RS_SET();  // 数据模式
//   uint8_t dataBuffer[2] = {0x00, data};  // 数据字节前需要一个0字节作为前缀
//   HAL_SPI_Transmit_DMA(&hspi1, dataBuffer, 2);
//   // LCD_CS_SET();  // 不在这里设置CS高，由DMA完成回调来处理
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
    uint8_t dataBuffer[2];  // 用于存放两个字节的缓冲区

    LCD_CS_CLR();  // 片选信号拉低，选择设备
    LCD_RS_SET();  // 数据寄存器选中

    // 分解16位数据到两个字节
    dataBuffer[0] = (Data >> 8) & 0xFF;  // 高字节
    dataBuffer[1] = Data & 0xFF;         // 低字节

    // 发送两个字节的数据
    HAL_SPI_Transmit_DMA(&hspi1, dataBuffer, 2);

    // 注意：不能在此处立即设置CS高，需要在传输完成回调中处理
    // LCD_CS_SET();  // 这一行应该移到DMA传输完成中断中
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
	LCD_SetCursor(x,y);//设置光标位置 
	Lcd_WriteData_16Bit(POINT_COLOR); 
}

void LCD_DrawPoint_color(u16 x, u16 y, u16 color)
{
    LCD_SetCursor(x, y); // 设置光标位置
    Lcd_WriteData_16Bit(color); // 写入颜色数据
}


/*****************************************************************************
 * @name       :void LCD_Clear(u16 Color)
 * @date       :2018-08-09 
 * @function   :Full screen filled LCD screen
 * @parameters :color:Filled color
 * @retvalue   :None
******************************************************************************/	
volatile uint32_t dma_transfer_complete = 1;  // 1表示传输完成
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
//            uint8_t dataBuffer[2];  // 用于存放两个字节的缓冲区

//            LCD_CS_CLR();  // 片选信号拉低，选择设备
//            LCD_RS_SET();  // 数据寄存器选中

//            // 分解16位数据到两个字节
//            dataBuffer[0] = (Color >> 8) & 0xFF;  // 高字节
//            dataBuffer[1] = Color & 0xFF;         // 低字节

//            // 发送两个字节的数据
//            HAL_SPI_Transmit_DMA(&hspi1, dataBuffer, 2);
//		}
//	}
//    

//	 //LCD_CS_SET();
//} 

//uint8_t lineBuffer[128 * 2 * 2];  // 每个像素两个字节
//void LCD_Clear(u16 Color)
//{
//    unsigned int i, m;  
//    LCD_SetWindows(0, 0, lcddev.width-1, lcddev.height-1);   
//    LCD_CS_CLR();
//    LCD_RS_SET();

//    // 预处理整行的颜色数据，但只填充半行
//    for (m = 0; m < lcddev.width * 2; m++)
//    {
//        int index = m * 2;
//        lineBuffer[index] = (Color >> 8) & 0xFF;  // 高字节
//        lineBuffer[index + 1] = Color & 0xFF;     // 低字节
//    }

//    // 对于每行，分两次传输
//    for (i = 0; i < lcddev.height; i++)
//    {
//        
//        HAL_SPI_Transmit_DMA(&hspi1, lineBuffer, lcddev.width * 2);  // 传输半行
//        while (!dma_transfer_complete);  // 等待 DMA 传输完成
//        dma_transfer_complete = 0;       // 重置 DMA 完成标志
//    }

//    LCD_CS_SET();  // 只在最后设置片选信号
//}

uint8_t lineBuffer[128 * 2 * 2];  // 每个像素需要两个 uint8_t（即 uint16_t），总共发送两倍数据量

void LCD_Clear(uint16_t Color)
{
    unsigned int i, m;  
    LCD_SetWindows(0, 0, 128 - 1, 128 - 1);  // 设置需要刷新的窗口
    LCD_CS_CLR();  // 清除片选信号，开始通信
    LCD_RS_SET();  // 设置数据/命令标志位为数据

    // 预处理整行的颜色数据，适配 uint16_t 颜色到 uint8_t 传输格式
    for (m = 0; m < 128; m++) {
        int index = m * 4;  // 每个像素占用4个uint8_t位置
        lineBuffer[index] = (Color >> 8) & 0xFF;    // 高字节
        lineBuffer[index + 1] = Color & 0xFF;       // 低字节
        lineBuffer[index + 2] = (Color >> 8) & 0xFF; // 高字节，重复
        lineBuffer[index + 3] = Color & 0xFF;       // 低字节，重复
    }

    // 对于每行，发送完整行数据
    for (i = 0; i < 128; i++) {
        HAL_SPI_Transmit_DMA(&hspi1, lineBuffer, 128 * 2); // 发送整行数据，驱动两次确保生效
        while (!dma_transfer_complete);  // 等待 DMA 传输完成
        dma_transfer_complete = 0;       // 重置 DMA 完成标志
    }
    


    LCD_CS_SET();  // 传输完成后设置片选信号，结束通信
}

void LCD_Clear_erea(int x1,int y1, int x2,int y2  ,uint16_t Color)
{
    unsigned int i, m;  
    
    
    int width = x2 - x1 + 1;   // 计算区域宽度
    int height = y2 - y1 + 1;  // 计算区域高度
    
    LCD_SetWindows(x1, y1, x2, y2);  // 设置需要刷新的窗口
    LCD_CS_CLR();  // 清除片选信号，开始通信
    LCD_RS_SET();  // 设置数据/命令标志位为数据

    // 预处理整行的颜色数据，适配 uint16_t 颜色到 uint8_t 传输格式
    for (m = 0; m < width; m++) {
        int index = m * 4;  // 每个像素占用4个uint8_t位置
        lineBuffer[index] = (Color >> 8) & 0xFF;    // 高字节
        lineBuffer[index + 1] = Color & 0xFF;       // 低字节
        lineBuffer[index + 2] = (Color >> 8) & 0xFF; // 高字节，重复
        lineBuffer[index + 3] = Color & 0xFF;       // 低字节，重复
    }

    // 对于每行，发送完整行数据
    for (i = 0; i < height; i++) {
        HAL_SPI_Transmit_DMA(&hspi1, lineBuffer, width * 2); // 发送整行数据，驱动两次确保生效
        while (!dma_transfer_complete);  // 等待 DMA 传输完成
        dma_transfer_complete = 0;       // 重置 DMA 完成标志
    }

    LCD_CS_SET();  // 传输完成后设置片选信号，结束通信
}

//void LCD_Clear_area_LVGL(int x1, int y1, int x2, int y2, uint8_t* Colors){
//    unsigned int i, m;
//    int width = x2 - x1 + 1;   // 计算区域宽度
//    int height = y2 - y1 + 1;  // 计算区域高度

//    LCD_SetWindows(x1, y1, x2, y2);  // 设置需要刷新的窗口
//    LCD_CS_CLR();  // 清除片选信号，开始通信
//    LCD_RS_SET();  // 设置数据/命令标志位为数据

//    // 对于每行，处理并发送整行数据
//    for (i = 0; i < height; i++) {
//        uint8_t* lineBuffer = (uint8_t*)malloc(width * 4);
//        if (lineBuffer == NULL) {
//            continue;  // 如果内存分配失败，跳过当前行
//        }

//        for (m = 0; m < width; m++) {
//            int index = m * 4;  // 每个像素占用2个uint8_t位置
//            lineBuffer[index] = (Colors[i + m] >> 8) & 0xFF;    // 高字节
//            lineBuffer[index + 1] = Colors[i  + m] & 0xFF;       // 低字节
//            lineBuffer[index + 2] = (Colors[i  + m] >> 8) & 0xFF;    // 高字节
//            lineBuffer[index + 3] = Colors[i  + m] & 0xFF;       // 低字节
//        }

//        HAL_SPI_Transmit_DMA(&hspi1, lineBuffer, width * 2); // 发送整行数据
//        while (!dma_transfer_complete);  // 等待 DMA 传输完成
//        dma_transfer_complete = 0;       // 重置 DMA 完成标志

//        free(lineBuffer);  // 释放当前行的缓冲区
//    }

//    LCD_CS_SET();  // 传输完成后设置片选信号，结束通信
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






//uint8_t lineBuffer[128 * 2];  // 每个像素需要两个 uint8_t（即 uint16_t）

//void LCD_Clear(uint16_t Color)
//{
//    unsigned int i, m;  
//    LCD_SetWindows(0, 0, lcddev.width - 1, lcddev.height - 1);  // 设置需要刷新的窗口
//    LCD_CS_CLR();  // 清除片选信号，开始通信
//    LCD_RS_SET();  // 设置数据/命令标志位为数据

//    // 预处理整行的颜色数据，适配 uint16_t 颜色到 uint8_t 传输格式
//    for (m = 0; m < lcddev.width; m++) {
//        int index = m * 2;  // 每个像素占用2个uint8_t位置
//        lineBuffer[index] = (Color >> 8) & 0xFF;    // 高字节
//        lineBuffer[index + 1] = Color & 0xFF;       // 低字节
//    }

//    // 对于每行，发送完整行数据
//    for (i = 0; i < lcddev.height; i++) {
//        HAL_SPI_Transmit_DMA(&hspi1, lineBuffer, lcddev.width * 2); // 发送整行数据，总长度为 lcddev.width * 2 字节
//        while (!dma_transfer_complete);  // 等待 DMA 传输完成
//        dma_transfer_complete = 0;       // 重置 DMA 完成标志
//    }

//    LCD_CS_SET();  // 传输完成后设置片选信号，结束通信
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
//	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;//普通输出模式
//	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
//	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;   //推挽输出
//	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;//上拉
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
//	SPI1_Init(); //硬件SPI初始化
    LCD_CS_CLR();
//	SPI_SetSpeed(SPI1,SPI_BaudRatePrescaler_2);
	LCD_GPIOInit();//LCD GPIO初始化										 
 	LCD_RESET(); //LCD 复位
//************* ST7735S初始化**********//	
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
  LCD_direction(USE_HORIZONTAL);//设置LCD显示方向
	LCD_LED_ON(); //LCD_LED=1;//点亮背光	 
	LCD_Clear(WHITE);//清全屏白色
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

	LCD_WriteRAM_Prepare();	//开始写入GRAM			
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
        //LCD_CS_SET();  // 在数据传输完成后，片选信号置高
        dma_transfer_complete = 1;  // 标记DMA传输完成
    }
}


