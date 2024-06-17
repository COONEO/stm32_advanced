/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * File Name          : freertos.c
  * Description        : Code for freertos applications
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2024 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
/* USER CODE END Header */

/* Includes ------------------------------------------------------------------*/
#include "FreeRTOS.h"
#include "task.h"
#include "main.h"
#include "cmsis_os.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "lcd.h"
#include "test.h"
#include "lvgl.h"
#include "lv_port_disp_template.h"
#include "demos/lv_demos.h"
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
typedef StaticTask_t osStaticThreadDef_t;
typedef StaticTimer_t osStaticTimerDef_t;
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
/* USER CODE BEGIN Variables */

/* USER CODE END Variables */
/* Definitions for defaultTask */
osThreadId_t defaultTaskHandle;
uint32_t defaultTaskBuffer[ 3000 ];
osStaticThreadDef_t defaultTaskControlBlock;
const osThreadAttr_t defaultTask_attributes = {
  .name = "defaultTask",
  .cb_mem = &defaultTaskControlBlock,
  .cb_size = sizeof(defaultTaskControlBlock),
  .stack_mem = &defaultTaskBuffer[0],
  .stack_size = sizeof(defaultTaskBuffer),
  .priority = (osPriority_t) osPriorityNormal,
};
/* Definitions for myTask02 */
osThreadId_t myTask02Handle;
uint32_t myTask02Buffer[ 3000 ];
osStaticThreadDef_t myTask02ControlBlock;
const osThreadAttr_t myTask02_attributes = {
  .name = "myTask02",
  .cb_mem = &myTask02ControlBlock,
  .cb_size = sizeof(myTask02ControlBlock),
  .stack_mem = &myTask02Buffer[0],
  .stack_size = sizeof(myTask02Buffer),
  .priority = (osPriority_t) osPriorityLow,
};
/* Definitions for myTask03 */
osThreadId_t myTask03Handle;
uint32_t myTask03Buffer[ 3000 ];
osStaticThreadDef_t myTask03ControlBlock;
const osThreadAttr_t myTask03_attributes = {
  .name = "myTask03",
  .cb_mem = &myTask03ControlBlock,
  .cb_size = sizeof(myTask03ControlBlock),
  .stack_mem = &myTask03Buffer[0],
  .stack_size = sizeof(myTask03Buffer),
  .priority = (osPriority_t) osPriorityLow,
};
/* Definitions for myTimer01 */
osTimerId_t myTimer01Handle;
osStaticTimerDef_t myTimer01ControlBlock;
const osTimerAttr_t myTimer01_attributes = {
  .name = "myTimer01",
  .cb_mem = &myTimer01ControlBlock,
  .cb_size = sizeof(myTimer01ControlBlock),
};
/* Definitions for myTimer02 */
osTimerId_t myTimer02Handle;
osStaticTimerDef_t myTimer02ControlBlock;
const osTimerAttr_t myTimer02_attributes = {
  .name = "myTimer02",
  .cb_mem = &myTimer02ControlBlock,
  .cb_size = sizeof(myTimer02ControlBlock),
};

/* Private function prototypes -----------------------------------------------*/
/* USER CODE BEGIN FunctionPrototypes */

/* USER CODE END FunctionPrototypes */

void StartDefaultTask(void *argument);
void StartTask02(void *argument);
void StartTask03(void *argument);
void Callback01(void *argument);
void Callback02(void *argument);

void MX_FREERTOS_Init(void); /* (MISRA C 2004 rule 8.1) */

/**
  * @brief  FreeRTOS initialization
  * @param  None
  * @retval None
  */
void MX_FREERTOS_Init(void) {
  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* USER CODE BEGIN RTOS_MUTEX */
  /* add mutexes, ... */
  /* USER CODE END RTOS_MUTEX */

  /* USER CODE BEGIN RTOS_SEMAPHORES */
  /* add semaphores, ... */
  /* USER CODE END RTOS_SEMAPHORES */

  /* Create the timer(s) */
  /* creation of myTimer01 */
  myTimer01Handle = osTimerNew(Callback01, osTimerPeriodic, NULL, &myTimer01_attributes);

  /* creation of myTimer02 */
  myTimer02Handle = osTimerNew(Callback02, osTimerPeriodic, NULL, &myTimer02_attributes);

  /* USER CODE BEGIN RTOS_TIMERS */
  /* start timers, add new ones, ... */
  /* USER CODE END RTOS_TIMERS */

  /* USER CODE BEGIN RTOS_QUEUES */
  /* add queues, ... */
  /* USER CODE END RTOS_QUEUES */

  /* Create the thread(s) */
  /* creation of defaultTask */
  defaultTaskHandle = osThreadNew(StartDefaultTask, NULL, &defaultTask_attributes);

  /* creation of myTask02 */
  myTask02Handle = osThreadNew(StartTask02, NULL, &myTask02_attributes);

  /* creation of myTask03 */
  myTask03Handle = osThreadNew(StartTask03, NULL, &myTask03_attributes);

  /* USER CODE BEGIN RTOS_THREADS */
  /* add threads, ... */
  /* USER CODE END RTOS_THREADS */

  /* USER CODE BEGIN RTOS_EVENTS */
  /* add events, ... */
  /* USER CODE END RTOS_EVENTS */

}

/* USER CODE BEGIN Header_StartDefaultTask */
int x1,x2,y1,y2;
uint16_t CColor;

/**
  * @brief  Function implementing the defaultTask thread.
  * @param  argument: Not used
  * @retval None
  */
/* USER CODE END Header_StartDefaultTask */
void StartDefaultTask(void *argument)
{
  /* USER CODE BEGIN StartDefaultTask */
    LCD_Init();
//    HAL_Delay(500);
//    LCD_Clear(GREEN);
//        HAL_Delay(500);
//    LCD_Clear(GRAY0);
//    // menu_test();
//    HAL_Delay(500);
//    LCD_Clear(GREEN);
//    HAL_Delay(500);
    lv_init();           // Initialize the LittlevGL library
    lv_port_disp_init(); // Initialize the display port for LittlevGL

    /* Change the active screen's background color */
    lv_obj_set_style_bg_color(lv_scr_act(), lv_color_hex(0x003a57), LV_PART_MAIN);

    /* Create a label on the screen */
    lv_obj_t *label = lv_label_create(lv_scr_act());
    lv_label_set_text(label, "Hello world"); // Set label text
    lv_obj_set_style_text_color(label, lv_color_hex(0xffffff), LV_PART_MAIN);
    lv_obj_align(label, LV_ALIGN_CENTER, 0, 0); // Center the label

    /* Create and start an animation for the label */
    lv_anim_t a;
    lv_anim_init(&a);
    lv_anim_set_var(&a, label);
    lv_anim_set_values(&a, lv_obj_get_x(label) - 50, lv_obj_get_x(label) + 50); // Move label left and right 50 pixels from center
    lv_anim_set_exec_cb(&a, (lv_anim_exec_xcb_t)lv_obj_set_x); // Callback to update the label's position
    lv_anim_set_time(&a, 2000);  // Animation duration 2000 milliseconds
    lv_anim_set_playback_delay(&a, 100); // Playback delay 100 milliseconds
    lv_anim_set_playback_time(&a, 2000); // Playback time 2000 milliseconds
    lv_anim_set_repeat_delay(&a, 500);   // Repeat delay 500 milliseconds
    lv_anim_set_repeat_count(&a, LV_ANIM_REPEAT_INFINITE); // Repeat infinitely
    lv_anim_start(&a); // Start the animation

 //   lv_demo_widgets();

 //   lv_demo_benchmark();
    
  /* Infinite loop */
  for(;;)
  {
      
//      LCD_Clear_erea(x1,y1,x2,y2 ,CColor);
      
//      #define GBLUE			 	0X07FF
//#define RED         0xF800
//#define MAGENTA     0xF81F
//#define GREEN       0x07E0
//      menu_test();
//          osDelay(500);
//    LCD_Clear(BLUE);
//        osDelay(500);
//    LCD_Clear(RED);
//              osDelay(500);
//    LCD_Clear(GREEN);
//      
//      lv_timer_periodic_handler();
      lv_timer_periodic_handler();
    osDelay(1);
  }
  /* USER CODE END StartDefaultTask */
}

/* USER CODE BEGIN Header_StartTask02 */
/**
* @brief Function implementing the myTask02 thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_StartTask02 */
void StartTask02(void *argument)
{
  /* USER CODE BEGIN StartTask02 */
  /* Infinite loop */
  for(;;)
  {
          //lv_timer_periodic_handler();
    osDelay(5);
  }
  /* USER CODE END StartTask02 */
}

/* USER CODE BEGIN Header_StartTask03 */
/**
* @brief Function implementing the myTask03 thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_StartTask03 */
void StartTask03(void *argument)
{
  /* USER CODE BEGIN StartTask03 */
  /* Infinite loop */
  for(;;)
  {
    osDelay(1);
  }
  /* USER CODE END StartTask03 */
}

/* Callback01 function */
void Callback01(void *argument)
{
  /* USER CODE BEGIN Callback01 */

  /* USER CODE END Callback01 */
}

/* Callback02 function */
void Callback02(void *argument)
{
  /* USER CODE BEGIN Callback02 */

  /* USER CODE END Callback02 */
}

/* Private application code --------------------------------------------------*/
/* USER CODE BEGIN Application */

/* USER CODE END Application */

