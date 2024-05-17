#ifndef __BOOTLOADER_H
#define __BOOTLOADER_H

#ifdef __cplusplus
extern "C" {
#endif

#include "main.h"

#define MAX_BLOCK_SIZE          ( 1024 )                  //1KB
#define ETX_APP_START_ADDRESS   0x08003800  // need change

void goto_application( void );



#ifdef __cplusplus
}
#endif

#endif /* __BOOTLOADER_H */
