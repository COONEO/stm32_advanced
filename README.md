# STM32F407 Partition Scheme

## Overview

This README document details the partition scheme of Flash and RAM for the STM32F407VET/7ET. The scheme aims to optimize the storage and execution of applications, ensuring that each module operates efficiently within the limited storage space.

## Hardware Resources

- Flash: 512KB
- RAM: 192KB

## Partition Structure

### Flash Partition

| Block         | Size   | Address Range               | Description           |
| ------------- | ------ | --------------------------- | --------------------- |
| 1. Bootloader | 16KB   | 0x0800 0000 - 0x0800 3FFF    | Bootloader            |
| 2. Bootloader | 16KB   | 0x0800 4000 - 0x0800 7FFF    | Bootloader            |
| 3. Check      | 16KB   | 0x0800 8000 - 0x0800 BFFF    | Check Area            |
| 4. Demo       | 64KB   | 0x0800 C000 - 0x0801 BFFF    | Demo Program          |
| 5. App        | 128KB  | 0x0802 0000 - 0x0802 FFFF    | Application Program   |
| 6. App        | 128KB  | 0x0803 0000 - 0x0803 FFFF    | Application Program   |
| 7. App        | 128KB  | 0x0804 0000 - 0x0804 FFFF    | Application Program   |

### RAM Partition

| Block        | Description       |
| ------------ | ----------------- |
| Initialized  | `.data` segment   |
| Uninitialized| `.bss` segment    |
| Dynamic Area | Heap and Stack    |

## Detailed Description of Each Partition

### Bootloader
- Address Range: 0x0800 0000 - 0x0800 7FFF
- Size: 32KB (two 16KB partitions)
- Function: The bootloader is used to load and execute the application program when the device is powered on or reset.

### Check
- Address Range: 0x0800 8000 - 0x0800 BFFF
- Size: 16KB
- Function: This area is used to store check data, ensuring the integrity and security of the application program.

### Demo
- Address Range: 0x0800 C000 - 0x0801 BFFF
- Size: 64KB
- Function: Used to store demonstration applications or test code.

### App (Application Program)
- Address Range: 0x0802 0000 - 0x0804 FFFF
- Size: Each block is 128KB, totaling 384KB
- Function: Mainly used to store user application code.


