/*******************************************************************************
 * @file           stm32f407xx_rcc_driver.c
 * @brief          Brief description
 * 
 * @author         Fahim Akhtar
 * @date           26-Nov-2025
 * @time           10:58:26 PM
 * @version        1.0
 * @copyright      (c) 2025
 ******************************************************************************/

#include "stm32f407xx_rcc_driver.h"

/* HPRE: AHB prescaler
 * 0xxx: system clock not divided
 * 1000: system clock divided by 2
 * 1001: system clock divided by 4
 * 1010: system clock divided by 8
 * 1011: system clock divided by 16
 * 1100: system clock divided by 64
 * 1101: system clock divided by 128
 * 1110: system clock divided by 256
 * 1111: system clock divided by 512
 *
 * */
const uint16_t AHB_PreScaler[8] = {2, 4, 8, 16, 64, 128, 256, 512};

/* PPRE1: APB Low speed prescaler (APB1)
 * 0xx: AHB clock not divided
 * 100: AHB clock divided by 2
 * 101: AHB clock divided by 4
 * 110: AHB clock divided by 8
 * 111: AHB clock divided by 16
 *
 * */
const uint8_t  APB1_PreScaler[4] = { 2, 4 , 8, 16};

/*******************************************************************
 * @fn              -   RCC_GetPCLK1Value
 * @brief           -   Return current PCLK1 frequency in Hz
 *
 * @note            -   Depends on RCC->CFGR. Assumes RCC_* bit positions
 *                      macros (RCC_SWS, RCC_HPRE, RCC_PPRE1) are defined.
 ********************************************************************/
uint32_t RCC_GetPCLK1Value(void){

    uint32_t SysClk, pclk1;

    uint8_t clksrc,temp,ahbp,apb1p;

    // 1. Check what source is selected for the System clock
    // Get the bit value of RCC_CFGR register bit number 3:2
    clksrc = ((RCC->CFGR >> RCC_SWS) & 0x03);

    if(clksrc == 0x00){
        SysClk = 16000000U; // 16 MHz - 00: HSI oscillator used as the system clock
    }else if(clksrc == 0x01){
        SysClk = 8000000U;  //  8 MHz - 01: HSE oscillator used as the system clock
    }else if(clksrc == 0x02){
        SysClk = RCC_GetPLLOutputClock();
    }

    // 2. Check the value of AHB prescaler
    // Get the bit value of RCC_CFGR register bit number 7:4
    temp = ((RCC->CFGR >> RCC_HPRE) & 0x0F);

    if(temp < 8){ // 0xxx: system clock not divided
        ahbp = 1;
    } else {
        ahbp = AHB_PreScaler[temp-8];
    }

    // 3. Check the value of APB1 prescaler
    // Get the bit value of RCC_CFGR register bit number 7:4
    temp = ((RCC->CFGR >> RCC_PPRE1) & 0x07);

    if(temp < 4){ // 0xxx: system clock not divided
        apb1p = 1;
    } else {
        apb1p = APB1_PreScaler[temp-4];
    }

    // 4. Calculate the PCLK on APB1 bus
    pclk1 =  (SysClk / ahbp) /apb1p;

    return pclk1;
}

/*******************************************************************
 * @fn              -   RCC_GetPCLK1Value
 * @brief           -   Return current PCLK2 frequency in Hz
 *
 * @note            -   Depends on RCC->CFGR. Assumes RCC_* bit positions
 *                      macros (RCC_SWS, RCC_HPRE, RCC_PPRE2) are defined.
 ********************************************************************/
uint32_t RCC_GetPCLK2Value(void){

    uint32_t SysClk, pclk2;

    uint8_t clksrc,temp,ahbp,apb2p;

    // 1. Check what source is selected for the System clock
    // Get the bit value of RCC_CFGR register bit number 3:2
    clksrc = ((RCC->CFGR >> RCC_SWS) & 0x03);

    if(clksrc == 0x00){
        SysClk = 16000000U; // 16 MHz - 00: HSI oscillator used as the system clock
    }else if(clksrc == 0x01){
        SysClk = 8000000U;  //  8 MHz - 01: HSE oscillator used as the system clock
    }else if(clksrc == 0x02){
        SysClk = RCC_GetPLLOutputClock();
    }

    // 2. Check the value of AHB prescaler
    // Get the bit value of RCC_CFGR register bit number 7:4
    temp = ((RCC->CFGR >> RCC_HPRE) & 0x0F);

    if(temp < 8){ // 0xxx: system clock not divided
        ahbp = 1;
    } else {
        ahbp = AHB_PreScaler[temp-8];
    }

    // 3. Check the value of APB2 prescaler
    // Get the bit value of RCC_CFGR register bit number 7:4
    temp = ((RCC->CFGR >> RCC_PPRE2) & 0x07);

    if(temp < 4){ // 0xxx: system clock not divided
        apb2p = 1;
    } else {
        apb2p = APB1_PreScaler[temp-4];
    }

    // 4. Calculate the PCLK on APB1 bus
    pclk2 =  (SysClk / ahbp) /apb2p;

    return pclk2;
}

/*******************************************************************
 * @fn              -   RCC_GetPLLOutputClock
 * @brief           -   Return PLL output clock (TODO: implement if PLL used)
 ********************************************************************/
uint32_t RCC_GetPLLOutputClock(){
    /* TODO: implement PLL output calculation if your system uses PLL as SYSCLK.
       For now return 0 to indicate not implemented. Callers that use PLL must
       add implementation here. */
    return 0;
}
