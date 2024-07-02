#include "step_motor.h"
#include "main.h"


//extern robot_state cooneo_car;
float current_angle_motor_0 = 0.0f;
float current_angle_motor_1 = 0.0f;
float current_angle_motor_2 = 0.0f;

// ʾ����ʼ��
StepMotor motors_sc[] = {
    {&htim4, TIM_CHANNEL_1, 1.8f , 0, 0.0f, GPIOA, GPIO_PIN_4, 0, 0.0f},  // PE5 ���1�������������Ϊ GPIOA PIN1 10khz
    {&htim4, TIM_CHANNEL_2, 1.8f , 0, 0.0f, GPIOA, GPIO_PIN_5, 0, 0.0f},  // PE6 ���2�������������Ϊ GPIOB PIN2 10khz
    {&htim4, TIM_CHANNEL_3, 1.8f , 0, 0.0f, GPIOA, GPIO_PIN_6, 0, 0.0f}, // PA7 ���2�������������Ϊ GPIOB PIN2 10khz
};

volatile int pulse_count = 0;
int target_pulses = 10;
extern volatile uint32_t TIM1_OverflowCount;

//void HAL_TIM_PWM_PulseFinishedCallback(TIM_HandleTypeDef *htim) F4
void HAL_TIM_PWM_PulseFinishedCallback(TIM_HandleTypeDef *htim)
{
    if (htim->Instance == TIM4) {
        for (int i = 0; i < sizeof(motors_sc) / sizeof(StepMotor); i++) {
            // ����Ƿ��ǵ�ǰ����Ļص�
            if (motors_sc[i].htim->Instance == htim->Instance &&
                ((htim->Channel == HAL_TIM_ACTIVE_CHANNEL_1 && motors_sc[i].channel == TIM_CHANNEL_1) ||
                 (htim->Channel == HAL_TIM_ACTIVE_CHANNEL_2 && motors_sc[i].channel == TIM_CHANNEL_2) ||
                 (htim->Channel == HAL_TIM_ACTIVE_CHANNEL_3 && motors_sc[i].channel == TIM_CHANNEL_3) ||
                 (htim->Channel == HAL_TIM_ACTIVE_CHANNEL_4 && motors_sc[i].channel == TIM_CHANNEL_4))) {
                
                if (motors_sc[i].pulses > 0) {
                    motors_sc[i].pulses--;  // �����������
                }

                // ������Ƿ��Ѿ�����˶�
                if (motors_sc[i].pulses == 0) {
                    HAL_TIM_PWM_Stop_IT(motors_sc[i].htim, motors_sc[i].channel);  // ֹͣPWM���ر��ж�

                    // �����������ӵ������˶���ĺ�������
                    // ���磬���ñ�־λ������֪ͨ��
                }
            }
        }
    }
    
}

#define REVERSE_ANGLE 10

#define MAX_ANGLE 60 // ������������ת�����Ƕ�
#define GEAR_RATIO 30 // ����������

void MoveMotorToAngle(uint8_t motor_id, float target_angle) {
    if (motor_id >= sizeof(motors_sc) / sizeof(motors_sc[0])) {
        return; // �����ID�Ƿ���Ч
    }
//		if(cooneo_car.delta_error == 1)
//		{
//			return;
//		}
    
    StepMotor *motor = &motors_sc[motor_id];
    if( motor-> pulses != 0 ) {
        return;
    }
    // ȷ��Ŀ��Ƕ�����Ч��Χ��
    if (target_angle < 0 || target_angle > MAX_ANGLE) {
        return; // Ŀ��Ƕ���Ч
    }

    // ������Ҫ��ת�ĽǶȲ�
    float angle_difference = target_angle - motor->current_angle;

    // ȷ����ת����
    int direction = (angle_difference >= 0) ? 1 : -1;

    // �����Ҫ������ת���򽫽ǶȲ�תΪ��ֵ
    if (angle_difference < 0) {
        angle_difference = -angle_difference;
    }

    // ����������
    uint32_t pulses_needed = (uint32_t)(angle_difference / motor->step_angle * GEAR_RATIO);

    // ���õ������
    if (direction > 0) {
        HAL_GPIO_WritePin(motor->dir_gpio_port, motor->dir_gpio_pin, GPIO_PIN_RESET); // ��ת
    } else {
        HAL_GPIO_WritePin(motor->dir_gpio_port, motor->dir_gpio_pin, GPIO_PIN_SET); // ��ת
    }

    // �������������������
    motor->pulses = pulses_needed;
    
    if (pulses_needed > 0) {

         // ���ݵ��ͨ������ռ�ձ�
        switch (motor->channel)
        {
        case TIM_CHANNEL_1:
            motor->htim->Instance->CCR1 = 80; // ����ռ�ձ�Ϊ20
            break;
        case TIM_CHANNEL_2:
            motor->htim->Instance->CCR2 = 80; // ����ռ�ձ�Ϊ20
            break;
				case TIM_CHANNEL_3:
            motor->htim->Instance->CCR3 = 80; // ����ռ�ձ�Ϊ20
						break;
				case TIM_CHANNEL_4:
            motor->htim->Instance->CCR4 = 80; // ����ռ�ձ�Ϊ20
						break;
            // �������ͨ�������
        }
        HAL_TIM_PWM_Start_IT(motor->htim, motor->channel);
    }

    // ���µ�ǰ�Ƕ�
    motor->current_angle = target_angle;
}

void MoveMotorForCalibration(uint8_t motor_id, float increment_angle) {
    if (motor_id >= sizeof(motors_sc) / sizeof(motors_sc[0])) {
        return; // �����ID�Ƿ���Ч
    }
		
//		if(cooneo_car.delta_error == 1)
//		{
//			return;
//		}
		
    
    StepMotor *motor = &motors_sc[motor_id];
    if (motor->pulses != 0) {
        return; // �����������˶�����ִ��У׼
    }

    int direction = (increment_angle >= 0) ? 1 : -1;
    // ���õ������
    if (direction > 0) {
        HAL_GPIO_WritePin(motor->dir_gpio_port, motor->dir_gpio_pin, GPIO_PIN_RESET); // ��ת
    } else {
        HAL_GPIO_WritePin(motor->dir_gpio_port, motor->dir_gpio_pin, GPIO_PIN_SET); // ��ת
    }

    // ȷ����ת���򣨼���У׼ʱʼ��������ת��
    // HAL_GPIO_WritePin(motor->dir_gpio_port, motor->dir_gpio_pin, GPIO_PIN_SET); // ��ת

     // �����Ҫ������ת���򽫽ǶȲ�תΪ��ֵ
    if (increment_angle < 0) {
        increment_angle = -increment_angle;
    }

    // ����������
    uint32_t pulses_needed = (uint32_t)(increment_angle / motor->step_angle * GEAR_RATIO);

    // �������������������
    motor->pulses = pulses_needed;
    
    if (pulses_needed > 0) {
        // ���ݵ��ͨ������ռ�ձ�
        switch (motor->channel) {
          case TIM_CHANNEL_1:
            motor->htim->Instance->CCR1 = 80; // ����ռ�ձ�Ϊ20
            break;
        case TIM_CHANNEL_2:
            motor->htim->Instance->CCR2 = 80; // ����ռ�ձ�Ϊ20
            break;
				case TIM_CHANNEL_3:
            motor->htim->Instance->CCR3 = 80; // ����ռ�ձ�Ϊ20
						break;
				case TIM_CHANNEL_4:
            motor->htim->Instance->CCR4 = 80; // ����ռ�ձ�Ϊ20
						break;
            // �������ͨ�������
        }
        HAL_TIM_PWM_Start_IT(motor->htim, motor->channel);
    }
}

void stop_step_motor(uint8_t motor_id) {
    if (motor_id >= sizeof(motors_sc) / sizeof(motors_sc[0])) {
        return; // �����ID�Ƿ���Ч
    }

    StepMotor *motor = &motors_sc[motor_id];

    // ֹͣ���
    HAL_TIM_PWM_Stop_IT(motor->htim, motor->channel);

    // �����������͵�ǰλ��
    motor->pulses = 0;
    motor->current_angle = 10.0f; // �������ǰλ������Ϊ��
}
