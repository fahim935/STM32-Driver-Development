/*
 * copyDataFromFlashToSram.c
 *
 *  Created on: 09-Oct-2025
 *      Author: Fahim Akhtar
 */

#include "common.h"
#include "copyDataFromFlashToSram.h"

/* This is string literal (constant data).
 * This will be stored in the ROM (FLASH Memory)
 */
const char *pMsg = "Hey There, Good Morning";

/* This is a constant data.
 * This will be also stored in the ROM (FLASH Memory)
 */
const int value = 200;

/* This is not a constant data.
 * This will be also stored in the SRAM
 */
char userData[50];

/* STM32F4xx
 *      FLASH Memory Range 0x0800 0000 to 0x080F FFFF
 *      SRAM1 Memory Range 0x2000 0000 to 0x2001 BFFF
 *      SRAM2 Memory Range 0x2001 C000 to 0x2001 FFFF
 * */

void copyDataFromFlashToSram(void) {

    printf("pMsg Addr %p\n",pMsg);
    printf("value Addr %p\n",&value);
    printf("userData Addr %p\n",&userData);

    for(uint32_t i = 0; i < strlen(pMsg); i++){
        userData[i] = *(pMsg + i); // Here Data is copy from Flash to SRAM
    }
}

