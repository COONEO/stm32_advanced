#ifndef __STEP_MOTOR_H__
#define __STEP_MOTOR_H__

#ifdef __cplusplus
extern "C" {
#endif

#include "main.h"
#include "tim.h"

// 假设步进电机步进角为1.8度，每圈需要200个脉冲
#define STEP_ANGLE 1.8
#define STEPS_PER_REV 200

typedef struct
{
    TIM_HandleTypeDef *htim;     // 定时器句柄
    uint32_t channel;            // PWM通道
    float step_angle;            // 电机单步角度
    uint32_t pulses;             // 需要产生的脉冲数
    float current_angle;         // 当前角度
    GPIO_TypeDef *dir_gpio_port; // 方向控制GPIO端口
    uint16_t dir_gpio_pin;       // 方向控制GPIO引脚
    int direction;               // 当前旋转方向，true为正转，false为反转
    float now_location;
} StepMotor;

// void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin);
void stop_step_motor(uint8_t motor_id);
void StartCalibration(void);
void MoveMotorToAngle(uint8_t motor_id, float target_angle);
void MoveMotorForCalibration(uint8_t motor_id, float increment_angle) ;
#endif

