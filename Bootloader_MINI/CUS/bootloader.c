#include "bootloader.h"
void goto_application(void)
{

    void (*app_reset_handler)(void) = (void (*)(void))(uintptr_t)(*((volatile uint32_t*)(ETX_APP_START_ADDRESS + 4U)));
    //check address
    if (app_reset_handler == (void*)0xFFFFFFFF)
    {
        while (1);
    }

    // 设置主堆栈指针为应用程序的起始地址
    __set_MSP(*(volatile uint32_t*)ETX_APP_START_ADDRESS);

    // 调用应用程序的复位处理程序
    app_reset_handler();
}
