# STM32F407 分区方案

## 概述

本 README 文档详细说明了 STM32F407VET/7ET 的 Flash 和 RAM 分区方案。该方案旨在优化应用程序的存储和执行，确保各个模块能够在有限的存储空间中高效运行。

## 硬件资源

- Flash: 512KB
- RAM: 192KB

## 分区结构

### Flash 分区

| 区块         | 大小   | 地址范围               | 描述           |
| ------------ | ------ | ---------------------- | -------------- |
| 1. Bootloader | 16KB  | 0x0800 0000 - 0x0800 3FFF | 引导程序       |
| 2. Bootloader | 16KB  | 0x0800 4000 - 0x0800 7FFF | 引导程序       |
| 3. Check      | 16KB  | 0x0800 8000 - 0x0800 BFFF | 校验区         |
| 4. Demo       | 64KB  | 0x0800 C000 - 0x0801 BFFF | 示例程序       |
| 5. App        | 128KB | 0x0802 0000 - 0x0803 FFFF | 应用程序       |
| 6. App        | 128KB | 0x0804 0000 - 0x0805 FFFF | 应用程序       |
| 7. App        | 128KB | 0x0806 0000 - 0x0807 FFFF | 应用程序       |

### RAM 分区

| 区块        | 描述          |
| ----------- | ------------- |
| 初始数据段  | `.data` 段    |
| 未初始化段  | `.bss` 段     |
| 动态分配区  | 堆和栈        |

## 各分区详细描述

### Bootloader (引导程序)
- 地址范围：0x0800 0000 - 0x0800 7FFF
- 大小：32KB (两个16KB分区)
- 功能：引导程序用于在设备上电或复位时加载并执行应用程序。

### Check (校验区)
- 地址范围：0x0800 8000 - 0x0800 BFFF
- 大小：16KB
- 功能：用于存储校验数据，确保应用程序的完整性和安全性。

### Demo (示例程序)
- 地址范围：0x0800 C000 - 0x0801 BFFF
- 大小：64KB
- 功能：存储演示应用程序或测试代码。

### App (应用程序)
- 地址范围：0x0802 0000 - 0x0807 FFFF
- 大小：每个128KB，总计384KB
- 功能：主要存储用户应用程序代码。