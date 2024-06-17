/**
 * @file lv_port_disp_templ.c
 *
 */

/*Copy this file as "lv_port_disp.c" and set this value to "1" to enable content*/
#if 1

/*********************
 *      INCLUDES
 *********************/
#include "lvgl.h"
#include "lv_port_disp_template.h"
#include <stdbool.h>

#include "lcd.h"
/*********************
 *      DEFINES
 *********************/

#define MY_DISP_HOR_RES 128
#define MY_DISP_VER_RES 128
 
#ifndef MY_DISP_HOR_RES
    #warning Please define or replace the macro MY_DISP_HOR_RES with the actual screen width, default value 320 is used for now.
    #define MY_DISP_HOR_RES    320
#endif

#ifndef MY_DISP_VER_RES
    #warning Please define or replace the macro MY_DISP_VER_RES with the actual screen height, default value 240 is used for now.
    #define MY_DISP_VER_RES    240
#endif

/**********************
 *      TYPEDEFS
 **********************/

/**********************
 *  STATIC PROTOTYPES
 **********************/
static void disp_init(void);

static void disp_flush(lv_display_t * disp, const lv_area_t * area, uint8_t * px_map);

/**********************
 *  STATIC VARIABLES
 **********************/

/**********************
 *      MACROS
 **********************/

/**********************
 *   GLOBAL FUNCTIONS
 **********************/

void lv_port_disp_init(void)
{
    /*-------------------------
     * Initialize your display
     * -----------------------*/
    disp_init();

    /*------------------------------------
     * Create a display and set a flush_cb
     * -----------------------------------*/
    lv_display_t * disp = lv_display_create(MY_DISP_HOR_RES, MY_DISP_VER_RES);
    lv_display_set_flush_cb(disp, disp_flush);

    /* Example 1
     * One buffer for partial rendering*/
//    static lv_color_t buf_1_1[MY_DISP_HOR_RES * 10];                          /*A buffer for 10 rows*/
//    lv_display_set_buffers(disp, buf_1_1, NULL, sizeof(buf_1_1), LV_DISPLAY_RENDER_MODE_PARTIAL);

//    /* Example 2
//     * Two buffers for partial rendering
//     * In flush_cb DMA or similar hardware should be used to update the display in the background.*/
    static lv_color_t buf_2_1[MY_DISP_HOR_RES * 10];
    static lv_color_t buf_2_2[MY_DISP_HOR_RES * 10];
    lv_display_set_buffers(disp, buf_2_1, buf_2_2, sizeof(buf_2_1), LV_DISPLAY_RENDER_MODE_PARTIAL);

//    /* Example 3
//     * Two buffers screen sized buffer for double buffering.
//     * Both LV_DISPLAY_RENDER_MODE_DIRECT and LV_DISPLAY_RENDER_MODE_FULL works, see their comments*/
//    static lv_color_t buf_3_1[MY_DISP_HOR_RES * MY_DISP_VER_RES];
//    static lv_color_t buf_3_2[MY_DISP_HOR_RES * MY_DISP_VER_RES];
//    lv_display_set_buffers(disp, buf_3_1, buf_3_2, sizeof(buf_3_1), LV_DISPLAY_RENDER_MODE_DIRECT);

}

/**********************
 *   STATIC FUNCTIONS
 **********************/

/*Initialize your display and the required peripherals.*/
static void disp_init(void)
{
    /*You code here*/
}

volatile bool disp_flush_enabled = true;

/* Enable updating the screen (the flushing process) when disp_flush() is called by LVGL
 */
void disp_enable_update(void)
{
    disp_flush_enabled = true;
}

/* Disable updating the screen (the flushing process) when disp_flush() is called by LVGL
 */
void disp_disable_update(void)
{
    disp_flush_enabled = false;
}

/*Flush the content of the internal buffer the specific area on the display.
 *`px_map` contains the rendered image as raw pixel map and it should be copied to `area` on the display.
 *You can use DMA or any hardware acceleration to do this operation in the background but
 *'lv_display_flush_ready()' has to be called when it's finished.*/
//static void disp_flush(lv_display_t * disp_drv, const lv_area_t * area, uint8_t * px_map)
//{
//    if(disp_flush_enabled) {
//        /*The most simple case (but also the slowest) to put all pixels to the screen one-by-one*/

//        int32_t x;
//        int32_t y;
//        for(y = area->y1; y <= area->y2; y++) {
//            for(x = area->x1; x <= area->x2; x++) {
//                /*Put a pixel to the display. For example:*/
//                /*put_px(x, y, *px_map)*/
//                px_map++;
//            }
//        }
//    }

//    /*IMPORTANT!!!
//     *Inform the graphics library that you are ready with the flushing*/
//    lv_display_flush_ready(disp_drv);
//}

extern volatile uint32_t dma_transfer_complete;

//static void disp_flush(lv_display_t * disp_drv, const lv_area_t * area, uint8_t * color_p)
//{
//    if(disp_flush_enabled) {
//        /*The most simple case (but also the slowest) to put all pixels to the screen one-by-one*/

//        int32_t x;
//        int32_t y;
//        for(y = area->y1; y <= area->y2; y++) {
//            for(x = area->x1; x <= area->x2; x++) {
//                u16 color = *((u16*)color_p); // Assumes color data is in u16 format
//                LCD_DrawPoint_color(x, y, color);
//                color_p += 2; // Move to the next color data (2 bytes per color)
//            }
//        }
//    }

//    /*IMPORTANT!!!
//     *Inform the graphics library that you are ready with the flushing*/
//    lv_display_flush_ready(disp_drv);
//}

//static void disp_flush(lv_display_t * disp_drv, const lv_area_t * area, uint8_t * color_p)
//{
//    if(disp_flush_enabled) {
//        // Calculate the number of pixels to update
//        uint32_t num_pixels = (area->x2 - area->x1 + 1) * (area->y2 - area->y1 + 1);

//        // Set the area to be updated
//        LCD_SetWindows(area->x1, area->y1, area->x2, area->y2);

//        // Clear the CS and set RS to data mode before starting the transmission
//        LCD_CS_CLR();
//        LCD_RS_SET();

//        // Start DMA transfer
//        HAL_SPI_Transmit_DMA(&hspi1, color_p, num_pixels * 2);  // num_pixels * 2 because each pixel is 2 bytes

//        // Wait for DMA transfer to complete
//        while (!dma_transfer_complete);
//        dma_transfer_complete = 0;

//        // Set CS high after transmission completed
//        LCD_CS_SET();
//    }

//    // Inform the graphics library that flushing is done
//    lv_display_flush_ready(disp_drv);
//}


//static void disp_flush(lv_display_t * disp_drv, const lv_area_t * area, uint8_t * color_p)
//{
//    if(disp_flush_enabled) {
//        /* Set the LCD drawing window */
//        LCD_SetWindows(area->x1, area->y1, area->x2, area->y2);

//        /* Clear the CS pin to start the transaction */
//        LCD_CS_CLR();
//        /* Set RS for data mode */
//        LCD_RS_SET();

//        /* Calculate the number of bytes to transfer */
//        uint32_t size = (area->x2 - area->x1 + 1) * (area->y2 - area->y1 + 1) * 2;

//        /* Start the DMA transfer using the provided color buffer */
//        HAL_SPI_Transmit_DMA(&hspi1, color_p, size);

//        /* Wait for DMA transfer to complete in the callback */
//        while(!dma_transfer_complete);
//        dma_transfer_complete = 0;  // Reset the completion flag
//    }

//    /* Inform the graphics library that you are ready with the flushing */
//    lv_display_flush_ready(disp_drv);
//}


//static void disp_flush(lv_display_t * disp_drv, const lv_area_t * area, uint8_t * color_p)
//{
//    if(disp_flush_enabled) {
//        uint32_t num_pixels = (area->x2 - area->x1 + 1) * (area->y2 - area->y1 + 1);

//        // 设置窗口和控制信号
//        LCD_SetWindows(area->x1, area->y1, area->x2, area->y2);
//        LCD_CS_CLR();
//        LCD_RS_SET();

//        // 重置传输完成标志
//        dma_transfer_complete = 0;

//        // 开始 DMA 传输
//        HAL_SPI_Transmit_DMA(&hspi1, color_p, num_pixels * 2);

//        // 等待 DMA 传输完成
//        while (dma_transfer_complete == 0) {
//            // 可以加入一些微小的延迟来减少 CPU 负载
//            __NOP(); // NOP 操作是空操作，对于简单的延迟很有用
//        }

//        // 完成传输，设置片选信号高
//        LCD_CS_SET();
//    }

//    // 通知 LVGL 刷新完成
//    lv_display_flush_ready(disp_drv);
//}

static void disp_flush(lv_display_t * disp_drv, const lv_area_t * area, uint8_t * color_p) {
    if(disp_flush_enabled) {
        uint32_t width = area->x2 - area->x1 + 1;
        uint32_t height = area->y2 - area->y1 + 1;
        uint32_t row_size = width * 2;

        LCD_SetWindows(area->x1, area->y1, area->x2, area->y2);

        for (uint32_t row = 0; row < height; row++) {
            uint8_t *row_start = color_p + row * row_size;

            LCD_CS_CLR();
            LCD_RS_SET();
            HAL_SPI_Transmit_DMA(&hspi1, row_start, row_size);
            while (!dma_transfer_complete);  // 等待DMA传输完成
            dma_transfer_complete = 0;

            // 短暂的延时来帮助硬件处理数据
            for (int i = 0; i < 100; i++) {
                __NOP();  // NOP操作提供简短延时
            }

            LCD_CS_SET();  // 结束传输前确保CS设置为高
        }

    }

    lv_display_flush_ready(disp_drv);
}




#else /*Enable this file at the top*/

/*This dummy typedef exists purely to silence -Wpedantic.*/
typedef int keep_pedantic_happy;
#endif
