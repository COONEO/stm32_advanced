#ifndef __STEP_MOTOR_H__
#define __STEP_MOTOR_H__

#ifdef __cplusplus
extern "C" {
#endif

#include "main.h"
#include "tim.h"

// ���貽�����������Ϊ1.8�ȣ�ÿȦ��Ҫ200������
#define STEP_ANGLE 1.8
#define STEPS_PER_REV 200

typedef struct
{
    TIM_HandleTypeDef *htim;     // ��ʱ�����
    uint32_t channel;            // PWMͨ��
    float step_angle;            // ��������Ƕ�
    uint32_t pulses;             // ��Ҫ������������
    float current_angle;         // ��ǰ�Ƕ�
    GPIO_TypeDef *dir_gpio_port; // �������GPIO�˿�
    uint16_t dir_gpio_pin;       // �������GPIO����
    int direction;               // ��ǰ��ת����trueΪ��ת��falseΪ��ת
    float now_location;
} StepMotor;

// void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin);
void stop_step_motor(uint8_t motor_id);
void StartCalibration(void);
void MoveMotorToAngle(uint8_t motor_id, float target_angle);
void MoveMotorForCalibration(uint8_t motor_id, float increment_angle) ;
#endif

