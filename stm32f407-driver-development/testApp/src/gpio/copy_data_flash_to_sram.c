/*******************************************************************************
 * @file           copy_data_flash_to_sram.c
 * @brief          Show-case the Data stored location for different data type
 *
 * @author         Fahim Akhtar
 * @date           09-Oct-2025
 * @time           8:14:25 AM
 * @version        1.0
 * @copyright      (c) 2025
 ******************************************************************************/

#include <copy_data_flash_to_sram.h>
#include "common.h"

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

/*
 * Write a simple example to show-case string literal stored in
 * Flash memory (Read-oly) and non-const data store in the stack memory (RAM)
 * Perform the copy from FLASH to SRAM & understand the concept behind it.
 * */

void copy_data_flash_to_sram(void) {

    printf("pMsg Addr %p\n",pMsg);
    printf("value Addr %p\n",&value);
    printf("userData Addr %p\n",&userData);

    for(uint32_t i = 0; i < strlen(pMsg); i++){
        userData[i] = *(pMsg + i); // Here Data is copy from Flash to SRAM
    }
}

