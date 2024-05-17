SET PATH=C:\Keil_v5\ARM\ARM_Compiler_5.06u7\Bin;C:\Program Files (x86)\VMware\VMware Workstation\bin\;C:\Python38\Scripts\;C:\Python38\;C:\Program Files\Microsoft\jdk-11.0.16.101-hotspot\bin;C:\WINDOWS\system32;C:\WINDOWS;C:\WINDOWS\System32\Wbem;C:\WINDOWS\System32\WindowsPowerShell\v1.0\;C:\WINDOWS\System32\OpenSSH\;C:\Program Files (x86)\NVIDIA Corporation\PhysX\Common;C:\Program Files\NVIDIA Corporation\NVIDIA NvDLISR;C:\Program Files\Git\cmd;C:\Users\tianr\AppData\Roaming\nvm;C:\Program Files\nodejs;C:\Program Files\Cloudflare\Cloudflare WARP\;C:\ProgramData\chocolatey\bin;C:\Program Files\dotnet\;C:\Program Files (x86)\Windows Kits\10\Windows Performance Toolkit\;C:\Program Files\WireGuard\;C:\Program Files\Docker\Docker\resources\bin;C:\Users\tianr\.cargo\bin;C:\Users\tianr\AppData\Local\Microsoft\WindowsApps;C:\Users\tianr\AppData\Local\Programs\Microsoft VS Code\bin;C:\Users\tianr\AppData\Roaming\nvm;C:\Program Files\nodejs;C:\Users\tianr\Downloads\platform-tools_r34.0.5-windows\platform-tools;C:\Users\tianr\AppData\Local\Yarn\bin;C:\Program Files\OpenSSL-Win64\bin\;;C:\Users\tianr\AppData\Local\Programs\Ollama
SET ARMCC5_ASMOPT=--diag_suppress=9931
SET ARMCC5_CCOPT=--diag_suppress=9931
SET ARMCC5_LINKOPT=--diag_suppress=9931
SET CPU_TYPE=STM32F103C8
SET CPU_VENDOR=STMicroelectronics
SET UV2_TARGET=APP2
SET CPU_CLOCK=0x007A1200
"C:\Keil_v5\ARM\ARM_Compiler_5.06u7\Bin\ArmAsm" --Via "app2\startup_stm32f103xb._ia"
"C:\Keil_v5\ARM\ARM_Compiler_5.06u7\Bin\ArmCC" --Via "app2\main.__i"
"C:\Keil_v5\ARM\ARM_Compiler_5.06u7\Bin\ArmCC" --Via "app2\gpio.__i"
"C:\Keil_v5\ARM\ARM_Compiler_5.06u7\Bin\ArmCC" --Via "app2\crc.__i"
"C:\Keil_v5\ARM\ARM_Compiler_5.06u7\Bin\ArmCC" --Via "app2\dma.__i"
"C:\Keil_v5\ARM\ARM_Compiler_5.06u7\Bin\ArmCC" --Via "app2\i2c.__i"
"C:\Keil_v5\ARM\ARM_Compiler_5.06u7\Bin\ArmCC" --Via "app2\spi.__i"
"C:\Keil_v5\ARM\ARM_Compiler_5.06u7\Bin\ArmCC" --Via "app2\usart.__i"
"C:\Keil_v5\ARM\ARM_Compiler_5.06u7\Bin\ArmCC" --Via "app2\stm32f1xx_it.__i"
"C:\Keil_v5\ARM\ARM_Compiler_5.06u7\Bin\ArmCC" --Via "app2\stm32f1xx_hal_msp.__i"
"C:\Keil_v5\ARM\ARM_Compiler_5.06u7\Bin\ArmCC" --Via "app2\stm32f1xx_hal_gpio_ex.__i"
"C:\Keil_v5\ARM\ARM_Compiler_5.06u7\Bin\ArmCC" --Via "app2\stm32f1xx_hal_crc.__i"
"C:\Keil_v5\ARM\ARM_Compiler_5.06u7\Bin\ArmCC" --Via "app2\stm32f1xx_hal.__i"
"C:\Keil_v5\ARM\ARM_Compiler_5.06u7\Bin\ArmCC" --Via "app2\stm32f1xx_hal_rcc.__i"
"C:\Keil_v5\ARM\ARM_Compiler_5.06u7\Bin\ArmCC" --Via "app2\stm32f1xx_hal_rcc_ex.__i"
"C:\Keil_v5\ARM\ARM_Compiler_5.06u7\Bin\ArmCC" --Via "app2\stm32f1xx_hal_gpio.__i"
"C:\Keil_v5\ARM\ARM_Compiler_5.06u7\Bin\ArmCC" --Via "app2\stm32f1xx_hal_dma.__i"
"C:\Keil_v5\ARM\ARM_Compiler_5.06u7\Bin\ArmCC" --Via "app2\stm32f1xx_hal_cortex.__i"
"C:\Keil_v5\ARM\ARM_Compiler_5.06u7\Bin\ArmCC" --Via "app2\stm32f1xx_hal_pwr.__i"
"C:\Keil_v5\ARM\ARM_Compiler_5.06u7\Bin\ArmCC" --Via "app2\stm32f1xx_hal_flash.__i"
"C:\Keil_v5\ARM\ARM_Compiler_5.06u7\Bin\ArmCC" --Via "app2\stm32f1xx_hal_flash_ex.__i"
"C:\Keil_v5\ARM\ARM_Compiler_5.06u7\Bin\ArmCC" --Via "app2\stm32f1xx_hal_exti.__i"
"C:\Keil_v5\ARM\ARM_Compiler_5.06u7\Bin\ArmCC" --Via "app2\stm32f1xx_hal_i2c.__i"
"C:\Keil_v5\ARM\ARM_Compiler_5.06u7\Bin\ArmCC" --Via "app2\stm32f1xx_hal_spi.__i"
"C:\Keil_v5\ARM\ARM_Compiler_5.06u7\Bin\ArmCC" --Via "app2\stm32f1xx_hal_tim.__i"
"C:\Keil_v5\ARM\ARM_Compiler_5.06u7\Bin\ArmCC" --Via "app2\stm32f1xx_hal_tim_ex.__i"
"C:\Keil_v5\ARM\ARM_Compiler_5.06u7\Bin\ArmCC" --Via "app2\stm32f1xx_hal_uart.__i"
"C:\Keil_v5\ARM\ARM_Compiler_5.06u7\Bin\ArmCC" --Via "app2\system_stm32f1xx.__i"
"C:\Keil_v5\ARM\ARM_Compiler_5.06u7\Bin\ArmLink" --Via "APP2\APP2.lnp"
"C:\Keil_v5\ARM\ARM_Compiler_5.06u7\Bin\fromelf.exe" "APP2\APP2.axf" --i32combined --output "APP2\APP2.hex"
