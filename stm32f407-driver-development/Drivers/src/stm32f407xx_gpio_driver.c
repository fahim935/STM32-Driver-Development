/*
 * stm32f407xx_gpio_driver.c
 *
 *  Created on: Oct 8, 2025
 *      Author: Fahim Akhtar
 */


#include "stm32f407xx_gpio_driver.h"


/*
 * Peripheral Clock setup
 */
/*******************************************************************
 * @fn              -   GPIO_PeriClockControl
 *
 * @brief           -   This function enable and disable the
 *                      peripheral clock for the given GPIO port
 *
 * @param[in]       -   Base Address of the GPIOx Peripheral
 * @param[in]       -   Enable and Disable Macro
 *
 * @return          -   None
 *
 * @Note            -   None
 ********************************************************************/
void GPIO_PeriClockControl(GPIO_RegDef_t *pGPIOx, uint8_t EnorDi)
{
    if(EnorDi == ENABLE){
        if(pGPIOx == GPIOA) {
            GPIOA_PCLK_EN();
        } else if (pGPIOx == GPIOB) {
            GPIOB_PCLK_EN();
        } else if (pGPIOx == GPIOC) {
            GPIOC_PCLK_EN();
        } else if (pGPIOx == GPIOD) {
            GPIOD_PCLK_EN();
        } else if (pGPIOx == GPIOE) {
            GPIOE_PCLK_EN();
        } else if (pGPIOx == GPIOF) {
            GPIOF_PCLK_EN();
        } else if (pGPIOx == GPIOG) {
            GPIOG_PCLK_EN();
        } else if (pGPIOx == GPIOH) {
            GPIOH_PCLK_EN();
        } else if (pGPIOx == GPIOI) {
            GPIOI_PCLK_EN();
        }
    } else if(EnorDi == DISABLE)    {
        if(pGPIOx == GPIOA) {
            GPIOA_PCLK_DI();
        } else if (pGPIOx == GPIOB) {
            GPIOB_PCLK_DI();
        } else if (pGPIOx == GPIOC) {
            GPIOC_PCLK_DI();
        } else if (pGPIOx == GPIOD) {
            GPIOD_PCLK_DI();
        } else if (pGPIOx == GPIOE) {
            GPIOE_PCLK_DI();
        } else if (pGPIOx == GPIOF) {
            GPIOF_PCLK_DI();
        } else if (pGPIOx == GPIOG) {
            GPIOG_PCLK_DI();
        } else if (pGPIOx == GPIOH) {
            GPIOH_PCLK_DI();
        } else if (pGPIOx == GPIOI) {
            GPIOI_PCLK_DI();
        }
    } else {
        /*invalid option*/
    }
}

/*******************************************************************
 * @fn              -   GPIO_Init
 *
 * @brief           -   Initialize the GPIOx
 *
 * @param[in]       -   GPIO Handler which contains Base Address of
 *                      GPIO port and Pin configuration details.
 *
 * @return          -   None
 *
 * @Note            -   None
 *******************************************************************/
void GPIO_Init(GPIO_Handle_t *pGPIOHandle)
{
    uint32_t temp = 0; // temporary register

    //1. Configure the MODE of GPIO pin
    if(pGPIOHandle->GPIO_PinConfig.GPIO_PinMode <= GPIO_MODE_ANALOG)
    {
        // Non Interrupt mode
        temp = (pGPIOHandle->GPIO_PinConfig.GPIO_PinMode <<
                        ( 2 * pGPIOHandle->GPIO_PinConfig.GPIO_PinNumber));

        // Clear the bit before set
        pGPIOHandle->pGPIOx->MODER &=
                ~(0x3 << ( 2 * pGPIOHandle->GPIO_PinConfig.GPIO_PinNumber));

        pGPIOHandle->pGPIOx->MODER |= temp;
    }
    else
    {
        // Interrupt mode
        if(pGPIOHandle->GPIO_PinConfig.GPIO_PinMode == GPIO_MODE_IT_FT)
        {
            //1. Configure THE FTSR
            EXTI->FTSR |= (1 << pGPIOHandle->GPIO_PinConfig.GPIO_PinNumber);

            // Clear the corresponding RTSR bit to double confirm only FTSR is enabled
            EXTI->RTSR &= ~(1 << pGPIOHandle->GPIO_PinConfig.GPIO_PinNumber);
        }
        else if(pGPIOHandle->GPIO_PinConfig.GPIO_PinMode == GPIO_MODE_IT_RT)
        {
            //2. Configure the RTSR
            EXTI->RTSR |= (1 << pGPIOHandle->GPIO_PinConfig.GPIO_PinNumber);

            // Clear the corresponding FTSR bit to double confirm only RTSR is enabled
            EXTI->FTSR &= ~(1 << pGPIOHandle->GPIO_PinConfig.GPIO_PinNumber);
        }
        else if(pGPIOHandle->GPIO_PinConfig.GPIO_PinMode == GPIO_MODE_IT_RFT)
        {
            //3. Configure both the FTSR & RTSR Registers
            EXTI->RTSR |= (1 << pGPIOHandle->GPIO_PinConfig.GPIO_PinNumber);
            EXTI->FTSR |= (1 << pGPIOHandle->GPIO_PinConfig.GPIO_PinNumber);
        }

        //4. Configure the GPIO port selection in SYSCFG_EXTICR
        uint8_t temp1 = pGPIOHandle->GPIO_PinConfig.GPIO_PinNumber /4;
        uint8_t temp2 = pGPIOHandle->GPIO_PinConfig.GPIO_PinNumber %4;
        uint8_t portcode = GPIO_BASEADDR_TO_CODE(pGPIOHandle->pGPIOx);

        SYSCFG_PCLK_EN();
        SYSCFG->EXTICR[temp1] |= portcode << (temp2 * 4);

        //5. Enable the EXTI interrupt delivery using IMR(interrupt Mask register)
        EXTI->IMR |= (1 << pGPIOHandle->GPIO_PinConfig.GPIO_PinNumber);
    }

    //2. Configure the Speed
    temp = 0;
    temp = (pGPIOHandle->GPIO_PinConfig.GPIO_PinSpeed
                <<  (2 * pGPIOHandle->GPIO_PinConfig.GPIO_PinNumber));

    // Clear the bit before set
    pGPIOHandle->pGPIOx->OSPEEDR &=
                    ~(0x3 << (2 *pGPIOHandle->GPIO_PinConfig.GPIO_PinNumber));

    pGPIOHandle->pGPIOx->OSPEEDR |= temp;

    //3. Configure the PuPd setting
    temp = 0;
    temp = (pGPIOHandle->GPIO_PinConfig.GPIO_PinPuPdControl
                << (2 * pGPIOHandle->GPIO_PinConfig.GPIO_PinNumber));

     // Clear the bit before set
    pGPIOHandle->pGPIOx->PUPDR &=
            ~(0x3 << (2 *pGPIOHandle->GPIO_PinConfig.GPIO_PinNumber));

    pGPIOHandle->pGPIOx->PUPDR |= temp;

    //4. Configure the OPType
    temp = 0;
    temp = (pGPIOHandle->GPIO_PinConfig.GPIO_PinOPType
            << (pGPIOHandle->GPIO_PinConfig.GPIO_PinNumber));

    // Clear the bit before set
    pGPIOHandle->pGPIOx->OTYPER &=
            ~(0x1 << pGPIOHandle->GPIO_PinConfig.GPIO_PinNumber);

    pGPIOHandle->pGPIOx->OTYPER |= temp;

    //5. Configure the Alternative functionality
    if(pGPIOHandle->GPIO_PinConfig.GPIO_PinMode == GPIO_MODE_ALTFN)
    {
        uint8_t temp1,temp2;
        // Decide which register GPIOx_AFRL/GPIOx_AFRH
        temp1 = (pGPIOHandle->GPIO_PinConfig.GPIO_PinNumber / 8);
        // Decide which rPIN number
        temp2 = (pGPIOHandle->GPIO_PinConfig.GPIO_PinNumber % 8);

        // Clear the bit before set
        pGPIOHandle->pGPIOx->AFR[temp1] &= ~(0xF << (4 * temp2));
        // As each register has 4 bits so X4 to shift to correct pin number
        pGPIOHandle->pGPIOx->AFR[temp1] |=
                pGPIOHandle->GPIO_PinConfig.GPIO_PinAltFunMode << (4 * temp2);
    }
}
/*******************************************************************
 * @fn              -   GPIO_DeInit
 *
 * @brief           -   De Initialize the specific GPIO Port
 *
 * @param[in]       -   GPIO Port (Port base address)
 *
 * @return          -   None
 *
 * @Note            -   None
 *******************************************************************/
void GPIO_DeInit(GPIO_RegDef_t *pGPIOx)
{
    if(pGPIOx == GPIOA) {
        GPIOA_REG_RESET();
    } else if (pGPIOx == GPIOB) {
        GPIOB_REG_RESET();
    } else if (pGPIOx == GPIOC) {
        GPIOC_REG_RESET();
    } else if (pGPIOx == GPIOD) {
        GPIOD_REG_RESET();
    } else if (pGPIOx == GPIOE) {
        GPIOE_REG_RESET();
    } else if (pGPIOx == GPIOF) {
        GPIOF_REG_RESET();
    } else if (pGPIOx == GPIOG) {
        GPIOG_REG_RESET();
    } else if (pGPIOx == GPIOH) {
        GPIOH_REG_RESET();
    } else if (pGPIOx == GPIOI) {
        GPIOI_REG_RESET();
    }
}

/*******************************************************************
 * @fn              -   GPIO_ReadFromInputPin
 *
 * @brief           -   Read the GPIO PIN
 *
 * @param[in]       -   GIOP Port
 * @param[in]       -   Pin Number of the port
 *
 * @return          -   0 0r 1
 *
 * @Note            -   None
 *******************************************************************/
uint8_t GPIO_ReadFromInputPin(GPIO_RegDef_t *pGPIOx, uint8_t PinNumber)
{
    uint8_t value = (uint8_t)((pGPIOx->IDR >> PinNumber) & 0x1);
    return value;
}
/*******************************************************************
 * @fn              -   GPIO_ReadFromInputPort
 *
 * @brief           -   Read complete GPIOx port in one short
 *
 * @param[in]       -   GPIOx Port address
 *
 * @return          -   Read Value of complete GPIOx port
 *
 * @Note            -   None
 *******************************************************************/
uint16_t GPIO_ReadFromInputPort(GPIO_RegDef_t *pGPIOx)
{
    uint16_t value = (uint16_t)pGPIOx->IDR ;
    return value;
}
/*******************************************************************
 * @fn              -   GPIO_WriteToOutputPin
 *
 * @brief           -   Send the data to specific pin
 *
 * @param[in]       -   GPIOx Port number
 * @param[in]       -   Pin Number
 * @param[in]       -   Value want to send (GPIO_PIN_SET/GPIO_PIN_RESET)
 *
 * @return          -   None
 *
 * @Note            -   None
 *******************************************************************/
void GPIO_WriteToOutputPin(GPIO_RegDef_t *pGPIOx, uint8_t PinNumber, uint8_t Value)
{
    if(Value == GPIO_PIN_SET) {
        // write 1
        pGPIOx->ODR |= (1 << PinNumber);
    } else {
        // write 0
        pGPIOx->ODR &= ~(1 << PinNumber);
    }
}
/*******************************************************************
 * @fn              -   GPIO_WriteToOutputPort
 *
 * @brief           -   Send the data to PORT
 *
 * @param[in]       -   GPIOx Port number (base Address of the PORT)
 * @param[in]       -   Value want to send (GPIO_PIN_SET/GPIO_PIN_RESET)
 *
 * @return          -   None
 *
 * @Note            -   None
 *******************************************************************/
void GPIO_WriteToOutputPort(GPIO_RegDef_t *pGPIOx, uint16_t Value)
{
    pGPIOx->ODR = Value;
}
/*******************************************************************
 * @fn              -   GPIO_ToggleOutputPin
 *
 * @brief           -   Toggle the Pin state
 *
 * @param[in]       -   GPIOx Port number (base Address of the PORT)
 * @param[in]       -   Pin Number
 *
 * @return          -   None
 *
 * @Note            -   None
 *******************************************************************/
void GPIO_ToggleOutputPin(GPIO_RegDef_t *pGPIOx, uint8_t PinNumber)
{
    pGPIOx->ODR ^= (1 << PinNumber);
}

/*
 * IRQ Configuration and ISR Handling
 */
/*******************************************************************
 * @fn              -   GPIO_IRQInterruptConfig
 *
 * @brief           -   Configure the IRQ
 *
 * @param[in]       -   IRQ number
 * @param[in]       -   Priority of The IRQ
 * @param[in]       -   Enable/Disable
 *
 * @return          -   void
 *
 * @Note            -   This function will configure ARM Cortex M4 NVIC register
 *******************************************************************/
void GPIO_IRQInterruptConfig(uint8_t IRQNumber, uint8_t EnorDi)
{
    if(EnorDi == ENABLE)
    {
        if(IRQNumber <= 31)
        {
            // Program ISER0 register (ARM Processor register)
            *NVIC_ISER0 |= (1 << IRQNumber);
        }
        else if (IRQNumber > 31 && IRQNumber < 64)
        {
            // Program ISER1 register
            *NVIC_ISER1 |= (1 << (IRQNumber % 32));
        }
        else if (IRQNumber >= 64 && IRQNumber < 96)
        {
            // Program ISER2 register
            *NVIC_ISER2 |= (1 << (IRQNumber % 64));
        }
    }
    else
    {
        if(IRQNumber <= 31)
        {
            // Program ISER0 register
            *NVIC_ICER0 |= (1 << (IRQNumber));
        }
        else if (IRQNumber > 31 && IRQNumber < 64)
        {
            // Program ISER1 register
            *NVIC_ICER1 |= (1 << (IRQNumber % 32));
        }
        else if (IRQNumber >= 64 && IRQNumber < 96)
        {
            // Program ISER2 register
            *NVIC_ICER2 |= (1 << (IRQNumber % 64));
        }
    }


}

/*******************************************************************
 * @fn              -   GPIO_IRQPriorityConfig
 *
 * @brief           -   Set the Priority of specific PIN
 *
 * @param1[in]      -   IRQ Pin number.
 * @param2[in]      -   IRQ Priority.
 *
 * @return          -   None
 *
 * @Note            -   There are 60 Registers (32 bits) with 4 section
 *                      each. lower nibble is not implemented therefore
 *                      whatever bit you wanted to set, set it into upper
 *                      nibble of the section (NO_PR_BITS_IMPLEMENTED).
 *******************************************************************/

void GPIO_IRQPriorityConfig (uint8_t IRQNumber, uint32_t IRQPriority)
{
    //1. Find out the interrupt priority Register
    uint8_t iprx = IRQNumber / 4; // Find out the register number out of 0 to 59
    uint8_t irpx_section = IRQNumber % 4;   // find out the sub section of register.

    /**
     * @note NVIC interrupt priority in Cortex-M4 (STM32F407):
     *       - Each IRQ has an 8-bit priority field, but only upper 4 bits [7:4] are implemented.
     *       - Lower 4 bits are read as 0 and ignored by hardware.
     *       - Valid priority levels: 0 (highest) to 15 (lowest).
     *       - CMSIS shifts priority value left by 4 bits internally (priority << 4).
     */

    uint8_t shift_amount = (8 * irpx_section) + (8 - NO_PR_BITS_IMPLEMENTED);

   *(NVIC_PR_BASE_ADDR + iprx) |= IRQPriority << shift_amount;
}

/*******************************************************************
 * @fn              -   GPIO_IRQHandling
 *
 * @brief           -   Handle the IRQ (ISR)
 *
 * @param[in]       -   IRQ Pin number
 *
 *******************************************************************/
void GPIO_IRQHandling(uint8_t PinNumber)
{
    // Clear the exit PR register corresponding to the pin number
    if(EXTI->PR & (1 << PinNumber))
    {
        //clear
        /* To clear the pending register(PR) write 1 to the register.
         * as mentioned in the MCU reference manual
         * */
        EXTI->PR |= (1 << PinNumber);
    }
}
