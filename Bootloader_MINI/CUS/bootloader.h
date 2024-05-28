#ifndef __BOOTLOADER_H
#define __BOOTLOADER_H

#ifdef __cplusplus
extern "C" {
#endif

#include "main.h"

#define MAX_BLOCK_SIZE          ( 1024 )                  //1KB
#define ETX_APP_START_ADDRESS   0x8010000 //0x800C000 CHECK //0x8010000 DEMOAPP// 0x08020000 APPMAIN  

void goto_application( void );

#ifdef __cplusplus
}
#endif

#endif /* __BOOTLOADER_H */
