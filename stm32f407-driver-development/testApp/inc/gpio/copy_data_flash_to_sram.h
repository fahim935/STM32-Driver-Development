/*
 * copy_data_flash_to_sram.h
 *
 *  Created on: 09-Oct-2025
 *      Author: Fahim Akhtar
 */

#ifndef INC_COPY_DATA_FROM_FLASH_TO_SRAM_H_
#define INC_COPY_DATA_FROM_FLASH_TO_SRAM_H_
#include "app_manager.h"
#if (FEATURE_SELECTED == STM32_COPY_DATA_FROM_FLASH_TO_SRAM)
void copy_data_flash_to_sram(void);
#endif

#endif /* INC_COPY_DATA_FROM_FLASH_TO_SRAM_H_ */
