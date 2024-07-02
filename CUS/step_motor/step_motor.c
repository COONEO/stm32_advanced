#include "step_motor.h"
#include "main.h"


//extern robot_state cooneo_car;
float current_angle_motor_0 = 0.0f;
float current_angle_motor_1 = 0.0f;
float current_angle_motor_2 = 0.0f;

// 示例初始化
StepMotor motors_sc[] = {
    {&htim4, TIM_CHANNEL_1, 1.8f , 0, 0.0f, GPIOA, GPIO_PIN_4, 0, 0.0f},  // PE5 电机1，方向控制引脚为 GPIOA PIN1 10khz
    {&htim4, TIM_CHANNEL_2, 1.8f , 0, 0.0f, GPIOA, GPIO_PIN_5, 0, 0.0f},  // PE6 电机2，方向控制引脚为 GPIOB PIN2 10khz
    {&htim4, TIM_CHANNEL_3, 1.8f , 0, 0.0f, GPIOA, GPIO_PIN_6, 0, 0.0f}, // PA7 电机2，方向控制引脚为 GPIOB PIN2 10khz
};

volatile int pulse_count = 0;
int target_pulses = 10;
extern volatile uint32_t TIM1_OverflowCount;

//void HAL_TIM_PWM_PulseFinishedCallback(TIM_HandleTypeDef *htim) F4
void HAL_TIM_PWM_PulseFinishedCallback(TIM_HandleTypeDef *htim)
{
    if (htim->Instance == TIM4) {
        for (int i = 0; i < sizeof(motors_sc) / sizeof(StepMotor); i++) {
            // 检查是否是当前电机的回调
            if (motors_sc[i].htim->Instance == htim->Instance &&
                ((htim->Channel == HAL_TIM_ACTIVE_CHANNEL_1 && motors_sc[i].channel == TIM_CHANNEL_1) ||
                 (htim->Channel == HAL_TIM_ACTIVE_CHANNEL_2 && motors_sc[i].channel == TIM_CHANNEL_2) ||
                 (htim->Channel == HAL_TIM_ACTIVE_CHANNEL_3 && motors_sc[i].channel == TIM_CHANNEL_3) ||
                 (htim->Channel == HAL_TIM_ACTIVE_CHANNEL_4 && motors_sc[i].channel == TIM_CHANNEL_4))) {
                
                if (motors_sc[i].pulses > 0) {
                    motors_sc[i].pulses--;  // 减少脉冲计数
                }

                // 检查电机是否已经完成运动
                if (motors_sc[i].pulses == 0) {
                    HAL_TIM_PWM_Stop_IT(motors_sc[i].htim, motors_sc[i].channel);  // 停止PWM并关闭中断

                    // 在这里可以添加电机完成运动后的后续操作
                    // 例如，设置标志位、发送通知等
                }
            }
        }
    }
    
}

#define REVERSE_ANGLE 10

#define MAX_ANGLE 60 // 假设电机可以旋转的最大角度
#define GEAR_RATIO 30 // 减速器比率

void MoveMotorToAngle(uint8_t motor_id, float target_angle) {
    if (motor_id >= sizeof(motors_sc) / sizeof(motors_sc[0])) {
        return; // 检查电机ID是否有效
    }
//		if(cooneo_car.delta_error == 1)
//		{
//			return;
//		}
    
    StepMotor *motor = &motors_sc[motor_id];
    if( motor-> pulses != 0 ) {
        return;
    }
    // 确保目标角度在有效范围内
    if (target_angle < 0 || target_angle > MAX_ANGLE) {
        return; // 目标角度无效
    }

    // 计算需要旋转的角度差
    float angle_difference = target_angle - motor->current_angle;

    // 确定旋转方向
    int direction = (angle_difference >= 0) ? 1 : -1;

    // 如果需要反向旋转，则将角度差转为正值
    if (angle_difference < 0) {
        angle_difference = -angle_difference;
    }

    // 计算脉冲数
    uint32_t pulses_needed = (uint32_t)(angle_difference / motor->step_angle * GEAR_RATIO);

    // 设置电机方向
    if (direction > 0) {
        HAL_GPIO_WritePin(motor->dir_gpio_port, motor->dir_gpio_pin, GPIO_PIN_RESET); // 正转
    } else {
        HAL_GPIO_WritePin(motor->dir_gpio_port, motor->dir_gpio_pin, GPIO_PIN_SET); // 反转
    }

    // 设置脉冲数并启动电机
    motor->pulses = pulses_needed;
    
    if (pulses_needed > 0) {

         // 根据电机通道设置占空比
        switch (motor->channel)
        {
        case TIM_CHANNEL_1:
            motor->htim->Instance->CCR1 = 80; // 假设占空比为20
            break;
        case TIM_CHANNEL_2:
            motor->htim->Instance->CCR2 = 80; // 假设占空比为20
            break;
				case TIM_CHANNEL_3:
            motor->htim->Instance->CCR3 = 80; // 假设占空比为20
						break;
				case TIM_CHANNEL_4:
            motor->htim->Instance->CCR4 = 80; // 假设占空比为20
						break;
            // 添加其他通道的情况
        }
        HAL_TIM_PWM_Start_IT(motor->htim, motor->channel);
    }

    // 更新当前角度
    motor->current_angle = target_angle;
}

void MoveMotorForCalibration(uint8_t motor_id, float increment_angle) {
    if (motor_id >= sizeof(motors_sc) / sizeof(motors_sc[0])) {
        return; // 检查电机ID是否有效
    }
		
//		if(cooneo_car.delta_error == 1)
//		{
//			return;
//		}
		
    
    StepMotor *motor = &motors_sc[motor_id];
    if (motor->pulses != 0) {
        return; // 如果电机正在运动，不执行校准
    }

    int direction = (increment_angle >= 0) ? 1 : -1;
    // 设置电机方向
    if (direction > 0) {
        HAL_GPIO_WritePin(motor->dir_gpio_port, motor->dir_gpio_pin, GPIO_PIN_RESET); // 正转
    } else {
        HAL_GPIO_WritePin(motor->dir_gpio_port, motor->dir_gpio_pin, GPIO_PIN_SET); // 反转
    }

    // 确定旋转方向（假设校准时始终正向旋转）
    // HAL_GPIO_WritePin(motor->dir_gpio_port, motor->dir_gpio_pin, GPIO_PIN_SET); // 正转

     // 如果需要反向旋转，则将角度差转为正值
    if (increment_angle < 0) {
        increment_angle = -increment_angle;
    }

    // 计算脉冲数
    uint32_t pulses_needed = (uint32_t)(increment_angle / motor->step_angle * GEAR_RATIO);

    // 设置脉冲数并启动电机
    motor->pulses = pulses_needed;
    
    if (pulses_needed > 0) {
        // 根据电机通道设置占空比
        switch (motor->channel) {
          case TIM_CHANNEL_1:
            motor->htim->Instance->CCR1 = 80; // 假设占空比为20
            break;
        case TIM_CHANNEL_2:
            motor->htim->Instance->CCR2 = 80; // 假设占空比为20
            break;
				case TIM_CHANNEL_3:
            motor->htim->Instance->CCR3 = 80; // 假设占空比为20
						break;
				case TIM_CHANNEL_4:
            motor->htim->Instance->CCR4 = 80; // 假设占空比为20
						break;
            // 添加其他通道的情况
        }
        HAL_TIM_PWM_Start_IT(motor->htim, motor->channel);
    }
}

void stop_step_motor(uint8_t motor_id) {
    if (motor_id >= sizeof(motors_sc) / sizeof(motors_sc[0])) {
        return; // 检查电机ID是否有效
    }

    StepMotor *motor = &motors_sc[motor_id];

    // 停止电机
    HAL_TIM_PWM_Stop_IT(motor->htim, motor->channel);

    // 清零脉冲数和当前位置
    motor->pulses = 0;
    motor->current_angle = 10.0f; // 将电机当前位置设置为零
}
