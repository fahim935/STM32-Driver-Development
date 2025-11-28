/*******************************************************************************
 * @file           stm32f407xx_rcc_driver.h
 * @brief          Brief description
 * 
 * @author         Fahim Akhtar
 * @date           26-Nov-2025
 * @time           10:58:39 PM
 * @version        1.0
 * @copyright      (c) 2025
 ******************************************************************************/

#ifndef INC_STM32F407XX_RCC_DRIVER_H_
#define INC_STM32F407XX_RCC_DRIVER_H_

#include "stm32f407xx.h"


/*******************************************************************
 * @fn              -   RCC_GetPCLK1Value
 * @brief           -   Return current PCLK1 frequency in Hz
 *
 * @note            -   Depends on RCC->CFGR. Assumes RCC_* bit positions
 *                      macros (RCC_SWS, RCC_HPRE, RCC_PPRE1) are defined.
 ********************************************************************/
uint32_t RCC_GetPCLK1Value(void);

/*******************************************************************
 * @fn              -   RCC_GetPCLK1Value
 * @brief           -   Return current PCLK2 frequency in Hz
 *
 * @note            -   Depends on RCC->CFGR. Assumes RCC_* bit positions
 *                      macros (RCC_SWS, RCC_HPRE, RCC_PPRE2) are defined.
 ********************************************************************/
uint32_t RCC_GetPCLK2Value(void);

/*******************************************************************
 * @fn              -   RCC_GetPLLOutputClock
 * @brief           -   Return PLL output clock (TODO: implement if PLL used)
 ********************************************************************/
uint32_t RCC_GetPLLOutputClock();

#endif /* INC_STM32F407XX_RCC_DRIVER_H__ */
