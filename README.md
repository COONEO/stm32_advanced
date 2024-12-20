## STM32 步进电机控制项目 README

### 项目概述

本项目是基于 STM32 微控制器的步进电机控制系统。它利用 STM32 的定时器进行精确的 PWM 输入捕获，从而实现对外部信号频率和占空比的测量。这些测量值可以用于步进电机的速度和方向控制，以及其他需要精确时间测量的应用。

该项目使用 STM32 HAL 库进行硬件抽象和外设配置。

### 硬件要求

-   STM32 微控制器开发板 (例如，使用 STM32F1 系列)
-   示波器 (用于验证 PWM 信号的频率和占空比)
-   调试器 (例如，ST-Link)

### 软件要求

-   **集成开发环境 (IDE):** Keil MDK, STM32CubeIDE, 或其他支持 STM32 开发的 IDE
-   **STM32 HAL 库:**  用于 STM32 的硬件抽象层库
-   **CMSIS:**  ARM Cortex 微控制器软件接口标准

### 文件结构

```
Project_Root/
├── Core/
│   ├── Inc/
│   │   ├── main.h
│   │   ├── ... 其他头文件
│   └── Src/
│       ├── main.c
│       ├── stm32f1xx_it.c    (中断处理函数, 根据你的 MCU 型号调整)
│       ├── stm32f1xx_hal_msp.c (HAL MSP 初始化)
│       ├── ... 其他源文件
├── Drivers/
│   ├── STM32F1xx_HAL_Driver/ (根据你的 MCU 型号调整)
│   └── CMSIS/
├── ... 其他 IDE 相关文件 (例如 .project, .cproject, Makefile 等)
└── README.md
```

### 代码功能

#### `main.c`

-   **初始化:** 初始化 HAL 库，配置系统时钟，初始化 GPIO, 定时器 (TIM1, TIM2, TIM3, TIM4) 和 UART2。
-   **PWM 输入捕获:**
    -   `TIM1` 配置为捕获 PA9 上的 PWM 信号 (通过修改`tttt`变量可以切换到PA8通道，用于测试)。
        -   `TIM1_CHANNEL_2` 捕获上升沿，计算周期。
        -   `TIM1_CHANNEL_1` 捕获下降沿，计算占空比。
    -   `TIM3` 配置为捕获外部 PWM 信号。
        -   `TIM3_CHANNEL_1` 捕获上升沿，计算周期。
        -   `TIM3_CHANNEL_2` 捕获下降沿，计算占空比。
-   **PWM 输出:** `TIM4` 配置为在 `TIM4_CHANNEL_1/2/3/4` 上产生 PWM 输出 (目前只作初始化, 具体频率和占空比未在 `main.c` 中设置)。
-   **中断处理:**
    -   `HAL_TIM_IC_CaptureCallback()`: 处理 `TIM1` 和 `TIM3` 的输入捕获中断，计算 PWM 信号的频率和占空比。
    -   目前注释掉了溢出中断回调函数`HAL_TIM_PeriodElapsedCallback()`.
-   **主循环:** 目前主循环为空，预留给步进电机控制逻辑使用。


### 如何使用

1.  **硬件连接:**
    -   将步进电机连接到驱动器。
    -   将驱动器的控制信号引脚连接到 STM32 的相应 GPIO 引脚 (根据 `step_motor.h` 中的定义)。
    -   将外部 PWM 信号源连接到 `PA9` (TIM1_CHANNEL_2) 和 `PA6` (TIM3_CHANNEL_1)。或将`tttt`变量改为0，使用PA8和PA6引脚
    -   将调试器连接到 STM32 开发板。
2.  **软件配置:**
    -   在 IDE 中打开项目。
    -   根据你的硬件连接和步进电机参数，修改 `step_motor.h` 和 `step_motor.c` 中的定义和函数。
    -   根据需要修改`main.c`中的定时器配置，例如修改`tttt`变量选择不同的PWM输入捕获通道。
    -   在 `main.c` 的主循环中调用 `step_motor.c` 中的函数来控制步进电机。
3.  **编译和下载:**
    -   编译项目。
    -   将生成的固件下载到 STM32 开发板。
4.  **测试:**
    -   给系统上电。
    -   使用示波器观察 `PA9` 或`PA8` 和 `PA6` 上的 PWM 信号。
    -   通过串口调试助手或其他方式观察 `PWM1_Frequency`, `PWM1_Duty`, `PWM2_Frequency`, `PWM2_Duty` 的值，验证输入捕获功能是否正常。
    -   根据 `step_motor.c` 中的函数，测试步进电机的控制功能。


