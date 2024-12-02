# STM32F1 OTA Bootloader Project

## 项目概述
这是一个基于STM32F1系列单片机的OTA升级项目，包含Bootloader和Application两个部分。项目实现了通过串口进行固件在线更新的功能，使用自定义的数据包协议确保数据传输的可靠性。

## 项目结构
```
project/
├── Bootloader_MINI/       # Bootloader程序
│   ├── Core/             # 核心代码
│   ├── MDK-ARM/          # Keil工程文件
│   └── Inc/              # 头文件
├── APP_MINI1/            # 应用程序
│   ├── Core/             # 核心代码
│   ├── MDK-ARM/          # Keil工程文件
│   └── Inc/              # 头文件
└── upload.py             # 固件上传工具
```

## 技术规格
- MCU: STM32F103系列
- 编程语言: C/Python
- 开发环境: Keil MDK
- 通信接口: UART (115200-8-N-1)
- Flash分区:
  - Bootloader: 0x08000000 - 0x08003800
  - Application: 0x08003800 - 0x08020000

## 通信协议
### 数据包格式
```
+------------+-------------+------------+-----------+------------+
|  起始标志   | 序列号      |  数据       | 校验和    |  结束标志   |
| 0xAB 0xCD  |  1 byte    | 64 bytes   | 1 byte   | 0xDC 0xBA  |
+------------+-------------+------------+-----------+------------+
```

### 校验和计算
```python
checksum = sum(data_bytes) % 256
```

## 使用说明

### 1. 环境准备
- Python 3.x
- pyserial库：`pip install pyserial`
- Keil MDK 5.x
- STM32CubeMX

### 2. 编译项目
1. 编译Bootloader：
   ```
   打开 Bootloader_MINI/MDK-ARM/Project.uvprojx
   编译生成Bootloader固件
   ```

2. 编译Application：
   ```
   打开 APP_MINI1/MDK-ARM/Project.uvprojx
   编译生成Application固件
   ```

### 3. 固件烧录
1. 首次烧录：
   - 使用ST-Link烧录Bootloader到设备
   - 确保烧录地址正确（0x08000000）

2. OTA更新：
   ```bash
   # 运行上传工具
   python upload.py
   ```
   - 选择正确的COM口
   - 确认固件大小
   - 按'Y'开始更新

## 关键功能说明

### Bootloader功能
1. 启动检查：
   ```c
   if (isFlashWritten(FLASH_START_ADDR, FLASH_TTT_ADDR)) {
       goto_application();  // 检查并跳转到应用程序
   }
   ```

2. 固件接收：
   - 通过UART中断接收数据
   - 检查数据包完整性
   - 写入Flash存储器

### 固件更新工具
1. 数据包发送：
   - 64字节数据块
   - 自动分包处理
   - 重发机制（每包发送3次）

## 故障排除

### 常见问题
1. 串口连接失败
   - 检查COM端口号
   - 验证波特率设置
   - 确认串口线连接

2. 更新失败
   - 检查固件大小是否正确
   - 验证校验和计算
   - 确认Flash写入权限

## 烧录Bootloader后 可以正常引导 APP程序 

在bootloader运行时候串口会输出 bootloader
在app运行时候串口会输出app


## 开发注意事项
1. Flash操作：
   - 写入前需要解锁
   - 确保按字对齐
   - 操作完成后加锁

2. 中断处理：
   - 及时清除中断标志
   - 避免中断嵌套
   - 控制中断处理时间

3. 内存管理：
   - 注意堆栈大小设置
   - 避免内存碎片
   - 合理分配Flash空间

## 可以改进的关键点
1. 添加固件加密功能
2. 实现固件版本管理
3. 添加固件回滚机制
4. 优化通信协议
5. 增加错误处理机制
