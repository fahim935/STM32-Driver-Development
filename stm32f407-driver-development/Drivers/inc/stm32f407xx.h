/**
 ******************************************************************************
 * @file           : stm32f407xx.h
 * @brief          : Register definition header for STM32F407 MCU family
 *                   Contains base addresses, register structures, and macros
 *                   for peripheral clock control, reset, and configuration.
 *
 * @author         : Fahim Akhtar
 * @date           : Oct 8, 2025
 * @version        : 1.0
 *
 * @note            Designed for low-level driver development without HAL/LL libraries.
 ******************************************************************************
 */

#ifndef INC_STM32F407XX_H_
#define INC_STM32F407XX_H_

#include <stddef.h>
#include <stdint.h>


/* ---------------------------- Common Definitions ---------------------------- */
#define __vo volatile
#define __weak __attribute__((weak))

/*****************************************************************************
 *                          Processor Specific Details                       *
 * Notes:These information you can get from ARM Cortex M4 Generic User Guide *
 *****************************************************************************/

/**
 * @brief NVIC ISER Registers (Interrupt Set Enable)
 * @note  Used to enable interrupt lines in NVIC.
 */

#define NVIC_ISER0  ((__vo uint32_t *)0xE000E100)
#define NVIC_ISER1  ((__vo uint32_t *)0xE000E104)
#define NVIC_ISER2  ((__vo uint32_t *)0xE000E108)
#define NVIC_ISER3  ((__vo uint32_t *)0xE000E10C)

/**
 * @brief NVIC ICER Registers (Interrupt Clear Enable)
 * @note  Used to disable interrupt lines in NVIC.
 */
#define NVIC_ICER0  ((__vo uint32_t *)0XE000E180)
#define NVIC_ICER1  ((__vo uint32_t *)0XE000E184)
#define NVIC_ICER2  ((__vo uint32_t *)0XE000E188)
#define NVIC_ICER3  ((__vo uint32_t *)0XE000E18C)

/* ARM Cortex M4 Processor Priority Register Address Calculation
 */
#define NVIC_PR_BASE_ADDR   ((__vo uint32_t *)0xE000E400)

/**
 * @brief NVIC Interrupt Priority Registers
 * @note  Each interrupt has 8-bit priority, but only upper 4 bits [7:4] are implemented.
 */
#define NO_PR_BITS_IMPLEMENTED      4 /**< Number of implemented priority bits */

/****************************************************************************
 *      Base Address of FLASH, SRAM and System Memory (ROM) memories        *
 *      (These data you can get it from Reference manual of STM32F407       *
 *       Table 3. Memory mapping Vs. Boot mode/physical remap ).            *
 ****************************************************************************/

#define FLASH_BASEADDR      0x08000000U     // 1024 KB (Kilo Bytes)
#define SRAM1_BASEADDR      0x20000000U     // 112 KB - Main internal SRAM1
#define SRAM2_BASEADDR      0x2001C000U     // 16 KB Auxiliary internal SRAM2
#define ROM_BASEADDR        0x1FFF0000U     // 30 KB System memory */
#define SRAM                SRAM1_BASEADDR  // As we using SRAM1 as main memory

/****************************************************************************
 * Different Bus domains of the stm32f4x MCU
 *
 * PERIPH_BASE                  (0x4000_0000)
 *      1. APB1PERIPH_BASE      (0x4000_0000)
 *      2. APB2PERIPH_BASE      (0x4001_0000)
 *      3. AHB1PERIPH_BASE      (0x4002_0000)
 *      4. AHB2PERIPH_BASE      (0x5000_0000)
 *
 * NOTE : Different peripherals are hanging on different busses.
 *
 *      AHB bus is used for those peripherals which need high speed data
 *      communication.(eg. Camera interface, GPIOs etc)
 *
 *      APB bus is used for those peripherals for which low speed communication
 *      would suffice.(eg I2C, SPI, UART, Timer, ADC etc )
 *
 * NOTE -   FULL memory MAP OF MCU start from 0x0000_0000 to 0xFFFF_FFFF.
 *          Peripheral Bus base address start from 0x4000_0000
 ****************************************************************************/

/****************************************************************************
 *              AHBx and APBx Bus peripheral base address                   *
 ****************************************************************************/

/****************************************************************************
 * Define base addresses of
 *      1.  AHB1 peripherals
 *      2.  APB1 peripherals
 *      3.  APB2 peripherals
 *      4.  APB2 peripherals
 *
 *
 *  1. AHB1 peripherals
 *      GPIOA , GPIOB, GPIOC, ..... GPIOI
 *
 *  2 AHB1 peripherals
 *      Camera interface, high speed peripherals
 *
 *  3. APB1 peripherals
 *      I2C1, I2C2, I2C3, SPI2, SPI3, USART2, USART3, UART4, UART5
 *
 *  4. APB2 peripherals
 *      SPI1, USART1, USART6, EXTI, SYSCFG
 *
 *****************************************************************************/

#define PERIPH_BASEADDR         0x40000000U

#define APB1PERIPH_BASEADDR     PERIPH_BASEADDR
#define APB2PERIPH_BASEADDR     0x40010000U
#define AHB1PERIPH_BASEADDR     0x40020000U
#define AHB2PERIPH_BASEADDR     0x50000000U


/******************************************************************************
 *                     AHB1 Peripheral Base Addresses
 ******************************************************************************/

#define GPIOA_BASEADDR      (AHB1PERIPH_BASEADDR + 0x0000U) // 0x40020000U
#define GPIOB_BASEADDR      (AHB1PERIPH_BASEADDR + 0x0400U) // 0x40020400U
#define GPIOC_BASEADDR      (AHB1PERIPH_BASEADDR + 0x0800U) // 0x40020800U
#define GPIOD_BASEADDR      (AHB1PERIPH_BASEADDR + 0x0C00U) // 0x40020C00U
#define GPIOE_BASEADDR      (AHB1PERIPH_BASEADDR + 0x1000U) // 0x40021000U
#define GPIOF_BASEADDR      (AHB1PERIPH_BASEADDR + 0x1400U) // 0x40021400U
#define GPIOG_BASEADDR      (AHB1PERIPH_BASEADDR + 0x1800U) // 0x40021800U
#define GPIOH_BASEADDR      (AHB1PERIPH_BASEADDR + 0x1C00U) // 0x40021C00U
#define GPIOI_BASEADDR      (AHB1PERIPH_BASEADDR + 0x2000U) // 0x40022000U

#define RCC_BASEADDR        (AHB1PERIPH_BASEADDR + 0x3800U) // 0x40023800U


/******************************************************************************
 *                     APB1 Peripheral Base Addresses
 ******************************************************************************/

#define I2C1_BASEADDR       (APB1PERIPH_BASEADDR + 0X5400U)
#define I2C2_BASEADDR       (APB1PERIPH_BASEADDR + 0X5800U)
#define I2C3_BASEADDR       (APB1PERIPH_BASEADDR + 0X5C00U)

#define SPI2_BASEADDR       (APB1PERIPH_BASEADDR + 0X3800U)
#define SPI3_BASEADDR       (APB1PERIPH_BASEADDR + 0X3C00U)

#define USART2_BASEADDR     (APB1PERIPH_BASEADDR + 0X4400U)
#define USART3_BASEADDR     (APB1PERIPH_BASEADDR + 0X4800U)

#define UART4_BASEADDR      (APB1PERIPH_BASEADDR + 0X4C00U)
#define UART5_BASEADDR      (APB1PERIPH_BASEADDR + 0X5000U)


/******************************************************************************
 *                     APB2 Peripheral Base Addresses
 ******************************************************************************/

#define EXTI_BASEADDR       (APB2PERIPH_BASEADDR + 0x3C00)
#define SPI1_BASEADDR       (APB2PERIPH_BASEADDR + 0x3000)
#define SYSCFG_BASEADDR     (APB2PERIPH_BASEADDR + 0x3800)
#define USART1_BASEADDR     (APB2PERIPH_BASEADDR + 0x1000)
#define USART6_BASEADDR     (APB2PERIPH_BASEADDR + 0x1400)


/******************************************************************************
 *                           GPIO Register Definition
 ******************************************************************************/

/**
 * @note GPIO Registers - STM32F407:
 *       - MODER   : Selects pin mode (00: Input, 01: Output, 10: AF, 11: Analog)
 *       - OTYPER  : Configures output type (0: Push-Pull, 1: Open-Drain)
 *       - OSPEEDR : Selects output speed
 *       - PUPDR   : Configures pull-up/pull-down
 *       - IDR     : Input Data Register
 *       - ODR     : Output Data Register
 *       - BSRR    : Bit Set/Reset Register (atomic operations)
 *       - AFR[0/1]: Alternate function selection
 */

typedef struct
{
    __vo uint32_t MODER;    /*  GPIO port mode register                     */
    __vo uint32_t OTYPER;   /*  GPIO port output type register              */
    __vo uint32_t OSPEEDR;  /*  GPIO port output speed register             */
    __vo uint32_t PUPDR;    /*  GPIO port pull-up/pull-down register        */
    __vo uint32_t IDR;      /*  GPIO port input data register               */
    __vo uint32_t ODR;      /*  GPIO port output data register              */
    __vo uint32_t BSRR;     /*  GPIO port bit set/reset register            */
    __vo uint32_t LCKR;     /*  GPIO port configuration lock register       */
    __vo uint32_t AFR[2];   /*  AF[0]: GPIO alternate function Low register */
                            /*  AF[1]: GPIO alternate function High register*/
} GPIO_RegDef_t;

/*****************************************************************************
 * By Structuring peripheral register we can avoid writing multiple register
 * address macros. We can use this structure for all GPIO ports just we need
 * to create the pointer for the individual port and assign the BASE ADDRESS
 * of the port to the pointer.
 *
 * Eg.
 * GPIO_RegDef_t *pGPIOA = (GPIO_RegDef_t *) GPIOA_BASEADDR;
 *
 * Access the peripheral GPIO like this :
 *
 * pGPIOA->MODER = 0x25; // Storing 0x25 into GPIO port A MODER register
 * *(0x40020000U + 0x00) = 0x25; // Internally compiler will do this conversion.
 *
 * pGPIOA->ODR = 0x55; // Storing 0x55 into GPIO port A OD register
 * *(0x40020000U + 0x14) = 0x55; // Internally compiler will do this conversion.
 *
 *****************************************************************************/


/****************************************************************************
 *  Peripheral register definition structure for RCC - Reset clock control  *
 ****************************************************************************/

/**
 * @note RCC (Reset and Clock Control) - STM32F407:
 *       - Controls enabling/disabling of peripheral clocks and reset management.
 *       - AHB1ENR, AHB2ENR, APB1ENR, APB2ENR → enable clocks for GPIO, TIM, USART, etc.
 *       - Example: RCC->AHB1ENR |= (1 << 0); // Enable GPIOA clock.
 *       - Always enable the peripheral clock before accessing its registers.
 *       - Use corresponding RSTR registers (e.g., AHB1RSTR) to reset peripherals.
 */

typedef struct
{
    __vo uint32_t CR;           // RCC clock control register
    __vo uint32_t PLLCFGR;      // RCC PLL configuration register
    __vo uint32_t CFGR;         // RCC clock configuration register
    __vo uint32_t CIR;          // RCC clock interrupt register
    __vo uint32_t AHB1RSTR;     // RCC AHB1 peripheral reset register
    __vo uint32_t AHB2RSTR;     // RCC AHB2 peripheral reset register
    __vo uint32_t AHB3RSTR;     // RCC AHB3 peripheral reset register
         uint32_t RFU1;         // RFU
    __vo uint32_t APB1RSTR;     // RCC APB1 peripheral reset register
    __vo uint32_t APB2RSTR;     // RCC APB2 peripheral reset register
         uint32_t RFU2;         // RFU
         uint32_t RFU3;         // RFU
    __vo uint32_t AHB1ENR;      // RCC AHB1 peripheral clock enable register
    __vo uint32_t AHB2ENR;      // RCC AHB2 peripheral clock enable register
    __vo uint32_t AHB3ENR;      // RCC AHB3 peripheral clock enable register
         uint32_t RFU4;         // RFU
    __vo uint32_t APB1ENR;      // RCC APB1 peripheral clock enable register
    __vo uint32_t APB2ENR;      // RCC APB2 peripheral clock enable register
         uint32_t RFU5;         // RFU
         uint32_t RFU6;         // RFU      LMP = low power mode
    __vo uint32_t AHB1LPENR;    // RCC AHB1 peripheral clock enable in LPM*
    __vo uint32_t AHB2LPENR;    // RCC AHB2 peripheral clock enable in LPM*
    __vo uint32_t AHB3LPENR;    // RCC AHB3 peripheral clock enable in LMP*
         uint32_t RFU7;         // RFU
    __vo uint32_t APB1LPENR;    // RCC APB1 peripheral clock enable in LMP*
    __vo uint32_t APB2LPENR;    // RCC APB2 peripheral clock enable in LMP*
         uint32_t RFU8;         // RFU
         uint32_t RFU9;         // RFU
    __vo uint32_t BDCR;         // RCC Backup domain control register
    __vo uint32_t CSR;          // RCC clock control & status register
         uint32_t RFU10;        // RFU
         uint32_t RFU11;        // RFU
    __vo uint32_t SSCGR;        // RCC spread spectrum clock generation register
    __vo uint32_t PLLI2SCFGR;   // RCC PLLI2S configuration register

}RCC_RegDef_t;

/******************************************************************************
 *                        EXTI Register Definition
 ******************************************************************************/
/**
 * @note EXTI (External Interrupt/Event Controller) Registers - STM32F407:
 *       - IMR  : Interrupt Mask Register → enables/disables EXTI line interrupt.
 *       - EMR  : Event Mask Register → enables/disables EXTI line event.
 *       - RTSR : Rising Trigger Selection Register → trigger on rising edge.
 *       - FTSR : Falling Trigger Selection Register → trigger on falling edge.
 *       - SWIER: Software Interrupt Event Register → manually trigger EXTI line.
 *       - PR   : Pending Register → shows interrupt flag; write '1' to clear.
 */

typedef struct {
    __vo uint32_t IMR;      //Interrupt mask register (EXTI_IMR)
    __vo uint32_t EMR;      //Event mask register (EXTI_EMR)
    __vo uint32_t RTSR;     //Rising trigger selection register (EXTI_RTSR)
    __vo uint32_t FTSR;     //Falling trigger selection register (EXTI_FTSR)
    __vo uint32_t SWIER;    //Software interrupt event register (EXTI_SWIER)
    __vo uint32_t PR;       //Pending register (EXTI_PR)

}EXTI_RegDef_t;

/******************************************************************************
 *                        SYSCFG Register Definition
 ******************************************************************************/

/**
 * @note SYSCFG EXTI Configuration (EXTICR Registers) - STM32F407:
 *       - EXTICR[0..3] map EXTI lines (0–15) to specific GPIO ports (A–I).
 *       - Each EXTI line can be connected to only one GPIO pin at a time.
 *       - Example: To link PA0 to EXTI0 → set SYSCFG->EXTICR[0] = 0x0000.
 *       - Must enable SYSCFG clock (RCC->APB2ENR |= (1 << 14)) before configuring.
 */
typedef struct {
    __vo uint32_t MEMRMP;   //SYSCFG memory remap register (SYSCFG_MEMRMP)
    __vo uint32_t PMC;      //SYSCFG peripheral mode configuration register (SYSCFG_PMC)
    __vo uint32_t EXTICR[4];//SYSCFG external interrupt configuration register
         uint32_t RFU1[2];  // RFU
    __vo uint32_t CMPCR;    //Compensation cell control register (SYSCFG_CMPCR)
         uint32_t RFU2[2];  //RFU
    __vo uint32_t CFGR;
}SYSCFG_RegDef_t;

/******************************************************************************
 *                        SPI Register Definition
 ******************************************************************************/
typedef struct {
    __vo uint32_t CR1;          /* Main configuration register (mode, polarity, phase, data format, enable)  */
    __vo uint32_t CR2;          /* Interrupts, DMA, and frame format control  */
    __vo uint32_t SR;           /* Status and event flags (TXE, RXNE, BSY, etc.) */
    __vo uint32_t DR;           /* Data register for TX/RX */
    __vo uint32_t CRCPR;        /* CRC polynomial configuration */
    __vo uint32_t RXCRCR;       /* Received CRC result */
    __vo uint32_t TXCRCR;       /* Transmitted CRC result */
    __vo uint32_t I2SCFGR;      /* Configures SPI peripheral for I²S audio mode */
    __vo uint32_t I2SPR;        /* I²S clock prescaler configuration */
}SPI_RegDef_t;

/******************************************************************************
 *                        I2C Register Definition
 ******************************************************************************/
typedef struct {
    __vo uint32_t CR1;          /* Control Register 1 - Peripheral enable, Start/Stop, ACK control */
    __vo uint32_t CR2;          /* Control Register 2 - Clock frequency, interrupts, DMA */
    __vo uint32_t OAR1;         /* Own address register1 - In Case of slave mode */
    __vo uint32_t OAR2;         /* Own address register2 - Secondary address (dual mode) */
    __vo uint32_t DR;           /* Data Register - Data transmit/receive buffer*/
    __vo uint32_t SR1;          /* Status Register 1 - Status (events, errors) */
    __vo uint32_t SR2;          /* Status Register 2 - Mode and bus status */
    __vo uint32_t CCR;          /* Clock Control Register - To configure the speed/mode */
    __vo uint32_t TRISE;        /* Rise time configuration */
    __vo uint32_t FLTR;         /* Digital/analog filter configuration */
} I2C_RegDef_t;


/****************************************************************************
 *          Peripheral definitions (Type casted to XXX_RegDef_t)            *
 ****************************************************************************/
#define GPIOA       ((GPIO_RegDef_t *) GPIOA_BASEADDR)
#define GPIOB       ((GPIO_RegDef_t *) GPIOB_BASEADDR)
#define GPIOC       ((GPIO_RegDef_t *) GPIOC_BASEADDR)
#define GPIOD       ((GPIO_RegDef_t *) GPIOD_BASEADDR)
#define GPIOE       ((GPIO_RegDef_t *) GPIOE_BASEADDR)
#define GPIOF       ((GPIO_RegDef_t *) GPIOF_BASEADDR)
#define GPIOG       ((GPIO_RegDef_t *) GPIOG_BASEADDR)
#define GPIOH       ((GPIO_RegDef_t *) GPIOH_BASEADDR)
#define GPIOI       ((GPIO_RegDef_t *) GPIOI_BASEADDR)

#define RCC         ((RCC_RegDef_t *) RCC_BASEADDR)
#define EXTI        ((EXTI_RegDef_t *) EXTI_BASEADDR)
#define SYSCFG      ((SYSCFG_RegDef_t *) SYSCFG_BASEADDR)

#define SPI1        ((SPI_RegDef_t *) SPI1_BASEADDR)
#define SPI2        ((SPI_RegDef_t *) SPI2_BASEADDR)
#define SPI3        ((SPI_RegDef_t *) SPI3_BASEADDR)

#define I2C1        ((I2C_RegDef_t *)I2C1_BASEADDR)
#define I2C2        ((I2C_RegDef_t *)I2C2_BASEADDR)
#define I2C3        ((I2C_RegDef_t *)I2C3_BASEADDR)

/****************************************************************************
 *              Clock enable macros for GPIOx peripherals                   *
 ****************************************************************************/
#define GPIOA_PCLK_EN()         (RCC->AHB1ENR |= (1 << 0))
#define GPIOB_PCLK_EN()         (RCC->AHB1ENR |= (1 << 1))
#define GPIOC_PCLK_EN()         (RCC->AHB1ENR |= (1 << 2))
#define GPIOD_PCLK_EN()         (RCC->AHB1ENR |= (1 << 3))
#define GPIOE_PCLK_EN()         (RCC->AHB1ENR |= (1 << 4))
#define GPIOF_PCLK_EN()         (RCC->AHB1ENR |= (1 << 5))
#define GPIOG_PCLK_EN()         (RCC->AHB1ENR |= (1 << 6))
#define GPIOH_PCLK_EN()         (RCC->AHB1ENR |= (1 << 7))
#define GPIOI_PCLK_EN()         (RCC->AHB1ENR |= (1 << 8))

/****************************************************************************
 *              Clock Disable macros for GPIOx peripherals                  *
 ****************************************************************************/
#define GPIOA_PCLK_DI()         (RCC->AHB1ENR &= ~(1 << 0))
#define GPIOB_PCLK_DI()         (RCC->AHB1ENR &= ~(1 << 1))
#define GPIOC_PCLK_DI()         (RCC->AHB1ENR &= ~(1 << 2))
#define GPIOD_PCLK_DI()         (RCC->AHB1ENR &= ~(1 << 3))
#define GPIOE_PCLK_DI()         (RCC->AHB1ENR &= ~(1 << 4))
#define GPIOF_PCLK_DI()         (RCC->AHB1ENR &= ~(1 << 5))
#define GPIOG_PCLK_DI()         (RCC->AHB1ENR &= ~(1 << 6))
#define GPIOH_PCLK_DI()         (RCC->AHB1ENR &= ~(1 << 7))
#define GPIOI_PCLK_DI()         (RCC->AHB1ENR &= ~(1 << 8))

/****************************************************************************
 *              Macros TO RESET GPIOx peripherals                           *
 ****************************************************************************/
/* We need to set 1 to the reset register bit which will reset the PORT and
 * again set to 0. otherwise that PORT will be always in reset condition.
 * we will be unable to enable the clock for that PORT
 * */

/*******************************************************************
 * @section         -   GPIO Peripheral Reset Macros
 *
 * @brief           -   These macros perform a hardware reset of the
 *                      specified GPIO port by toggling its reset bit
 *                      in the RCC AHB1RSTR register.
 *
 * @usage           -   Call the appropriate macro to reset GPIOx:
 *                          GPIOA_REG_RESET();
 *                          GPIOB_REG_RESET();
 *                          ...
 *                          GPIOI_REG_RESET();
 *
 * @note            -   Each macro sets and clears the corresponding
 *                      reset bit. If the bit remains set, the port
 *                      stays in reset state and cannot be clocked.
 ********************************************************************/
#define GPIOA_REG_RESET()   do{ (RCC->AHB1RSTR |= (1 << 0));    \
                                (RCC->AHB1RSTR &= ~(1 << 0));   \
                              }while(0)

#define GPIOB_REG_RESET()   do{ (RCC->AHB1RSTR |= (1 << 1));    \
                                (RCC->AHB1RSTR &= ~(1 << 1));   \
                              }while(0)

#define GPIOC_REG_RESET()   do{ (RCC->AHB1RSTR |= (1 << 2));    \
                                (RCC->AHB1RSTR &= ~(1 << 2));   \
                              }while(0)

#define GPIOD_REG_RESET()   do{ (RCC->AHB1RSTR |= (1 << 3));    \
                                (RCC->AHB1RSTR &= ~(1 << 3));   \
                              }while(0)

#define GPIOE_REG_RESET()   do{ (RCC->AHB1RSTR |= (1 << 4));    \
                                (RCC->AHB1RSTR &= ~(1 << 4));   \
                              }while(0)

#define GPIOF_REG_RESET()   do{ (RCC->AHB1RSTR |= (1 << 5));    \
                                (RCC->AHB1RSTR &= ~(1 << 5));   \
                              }while(0)

#define GPIOG_REG_RESET()   do{ (RCC->AHB1RSTR |= (1 << 6));    \
                                (RCC->AHB1RSTR &= ~(1 << 6));   \
                              }while(0)

#define GPIOH_REG_RESET()   do{ (RCC->AHB1RSTR |= (1 << 7));    \
                                (RCC->AHB1RSTR &= ~(1 << 7));   \
                              }while(0)

#define GPIOI_REG_RESET()   do{ (RCC->AHB1RSTR |= (1 << 8));    \
                                (RCC->AHB1RSTR &= ~(1 << 8));   \
                              }while(0)

/*******************************************************************
 * @section         -   SPI Peripheral Reset Macros
 *
 * @brief           -   These macros perform a hardware reset of the
 *                      specified SPI peripheral by toggling the
 *                      corresponding reset bit in the RCC reset register.
 *
 * @usage           -   Call the appropriate macro to reset SPIx:
 *                          SPI1_REG_RESET();
 *                          SPI2_REG_RESET();
 *                          SPI3_REG_RESET();
 *
 * @note            -   These macros are typically used in SPI_DeInit()
 *                      to restore peripheral registers to default state.
 ********************************************************************/
#define SPI1_REG_RESET()      do { (RCC->APB2RSTR |= (1 << 12)); \
                                   (RCC->APB2RSTR &= ~(1 << 12));\
                                 } while(0)
#define SPI2_REG_RESET()      do { (RCC->APB1RSTR |= (1 << 14)); \
                                   (RCC->APB1RSTR &= ~(1 << 14));\
                                 } while(0)
#define SPI3_REG_RESET()      do { (RCC->APB1RSTR |= (1 << 15)); \
                                   (RCC->APB1RSTR &= ~(1 << 15));\
                                 } while(0)

/*******************************************************************
 * @section         -   I2C Peripheral Reset Macros
 *
 * @brief           -   These macros perform a hardware reset of the
 *                      specified I2C peripheral by toggling the
 *                      corresponding reset bit in the RCC reset register.
 *
 * @usage           -   Call the appropriate macro to reset I2Cx:
 *                          I2C1_REG_RESET();
 *                          I2C2_REG_RESET();
 *                          I2C3_REG_RESET();
 *
 * @note            -   These macros are typically used in I2C_DeInit()
 *                      to restore peripheral registers to default state.
 ********************************************************************/
#define I2C1_REG_RESET()      do { (RCC->APB1RSTR |= (1 << 21)); \
                                   (RCC->APB1RSTR &= ~(1 << 21));\
                                 } while(0)
#define I2C2_REG_RESET()      do { (RCC->APB1RSTR |= (1 << 22)); \
                                   (RCC->APB1RSTR &= ~(1 << 22));\
                                 } while(0)
#define I2C3_REG_RESET()      do { (RCC->APB1RSTR |= (1 << 23)); \
                                   (RCC->APB1RSTR &= ~(1 << 23));\
                                 } while(0)

/****************************************************************************
 *              Clock Enable Macro for I2Cx peripheral                      *
 ****************************************************************************/
#define I2C1_PCLK_EN()              (RCC->APB1ENR |= (1 << 21))
#define I2C2_PCLK_EN()              (RCC->APB1ENR |= (1 << 22))
#define I2C3_PCLK_EN()              (RCC->APB1ENR |= (1 << 23))

/****************************************************************************
 *              Clock Disable Macro for I2Cx peripheral                     *
 ****************************************************************************/
#define I2C1_PCLK_DI()              (RCC->APB1ENR &= ~(1 << 21))
#define I2C2_PCLK_DI()              (RCC->APB1ENR &= ~(1 << 22))
#define I2C3_PCLK_DI()              (RCC->APB1ENR &= ~(1 << 23))

/****************************************************************************
 *              Clock Enable Macros for SPIx peripherals                    *
 ****************************************************************************/
#define SPI1_PCLK_EN()              (RCC->APB2ENR |= (1 << 12))
#define SPI2_PCLK_EN()              (RCC->APB1ENR |= (1 << 14))
#define SPI3_PCLK_EN()              (RCC->APB1ENR |= (1 << 15))
#define SPI4_PCLK_EN()              (RCC->APB2ENR |= (1 << 13))

/****************************************************************************
 *              Clock Disable Macros for SPIx peripherals                   *
 ****************************************************************************/
#define SPI1_PCLK_DI()              (RCC->APB2ENR &= ~(1 << 12))
#define SPI2_PCLK_DI()              (RCC->APB1ENR &= ~(1 << 14))
#define SPI3_PCLK_DI()              (RCC->APB1ENR &= ~(1 << 15))
#define SPI4_PCLK_DI()              (RCC->APB2ENR &= ~(1 << 13))

/****************************************************************************
 *              Clock Enable Macros for USARTx peripherals                  *
 ****************************************************************************/
#define USART1_PCLK_EN()            (RCC->APB2ENR |= (1 << 4))
#define USART2_PCLK_EN()            (RCC->APB1ENR |= (1 << 17))
#define USART3_PCLK_EN()            (RCC->APB1ENR |= (1 << 18))
#define UART4_PCLK_EN()             (RCC->APB1ENR |= (1 << 19))
#define UART5_PCLK_EN()             (RCC->APB1ENR |= (1 << 20))
#define USART6_PCLK_EN()            (RCC->APB1ENR |= (1 << 5))

/****************************************************************************
 *              Clock Disable Macros for USARTx peripherals                 *
 ****************************************************************************/
#define USART1_PCLK_DI()            (RCC->APB2ENR &= ~(1 << 4))
#define USART2_PCLK_DI()            (RCC->APB1ENR &= ~(1 << 17))
#define USART3_PCLK_DI()            (RCC->APB1ENR &= ~(1 << 18))
#define UART4_PCLK_DI()             (RCC->APB1ENR &= ~(1 << 19))
#define UART5_PCLK_DI()             (RCC->APB1ENR &= ~(1 << 20))
#define USART6_PCLK_DI()            (RCC->APB1ENR &= ~(1 << 5))

/****************************************************************************
 *              Clock Enable/Disable Macros for SYSCFG peripherals          *
 ****************************************************************************/

#define SYSCFG_PCLK_EN()            (RCC->APB2ENR |=  (1 << 14))
#define SYSCFG_PCLK_DI()            (RCC->APB2ENR &= ~(1 << 14))

#define GPIO_BASEADDR_TO_CODE(x)  ( (x == GPIOA) ? 0 : \
                                    (x == GPIOB) ? 1 : \
                                    (x == GPIOC) ? 2 : \
                                    (x == GPIOD) ? 3 : \
                                    (x == GPIOE) ? 4 : \
                                    (x == GPIOF) ? 5 : \
                                    (x == GPIOG) ? 6 : \
                                    (x == GPIOH) ? 7 : \
                                    (x == GPIOI) ? 8 : 0)

/****************************************************************************
 *                              IRQ Pin numbers                             *
 ****************************************************************************/

/*
 * IRQ interrupt request numbers of STM32f407x MCU.
 * Note: You can get these number info from Vector table of MCU reference manual
 * */

#define IRQ_NO_EXTI0          6     // EXTI Line0 interrupt (for pin PA0, PB0, etc.)
#define IRQ_NO_EXTI1          7     // EXTI Line1 interrupt
#define IRQ_NO_EXTI2          8     // EXTI Line2 interrupt
#define IRQ_NO_EXTI3          9     // EXTI Line3 interrupt
#define IRQ_NO_EXTI4          10    // EXTI Line4 interrupt
#define IRQ_NO_EXTI9_5        23    // EXTI Line[9:5] interrupts combined
#define IRQ_NO_EXTI15_10      40    // EXTI Line[15:10] interrupts combined

#define IRQ_NO_SPI1           35    // SPI1 global interrupt
#define IRQ_NO_SPI2           36    // SPI2 global interrupt
#define IRQ_NO_SPI3           51    // SPI3 global interrupt
#define IRQ_NO_SPI4           84    // SPI4 global interrupt (if available on device)

#define IRQ_NO_I2C1_EV        31    // I2C1 Event interrupt (TXE, RXNE, etc.)
#define IRQ_NO_I2C1_ER        32    // I2C1 Error interrupt (BERR, ARLO, OVR, etc.)

#define IRQ_NO_I2C2_EV        33    // I2C2 Event interrupt (TXE, RXNE, etc.)
#define IRQ_NO_I2C2_ER        34    // I2C2 Error interrupt (BERR, ARLO, OVR, etc.)

#define IRQ_NO_I2C3_EV        72    // I2C3 Event interrupt (TXE, RXNE, etc.)
#define IRQ_NO_I2C3_ER        73    // I2C3 Error interrupt (BERR, ARLO, OVR, etc.)

#define IRQ_NO_USART1         37    // USART1 global interrupt
#define IRQ_NO_USART2         38    // USART2 global interrupt
#define IRQ_NO_USART3         39    // USART3 global interrupt
#define IRQ_NO_UART4          52    // UART4  global interrupt
#define IRQ_NO_UART5          53    // UART5  global interrupt
#define IRQ_NO_USART6         71    // USART6 global interrupt (on high-density devices)

/****************************************************************************
 *                          NVIC IRQ Priority Numbers
 ****************************************************************************/
#define NVIC_IRQ_PRI0         0     // Highest priority level
#define NVIC_IRQ_PRI1         1     // Priority level 1
#define NVIC_IRQ_PRI2         2     // Priority level 2
#define NVIC_IRQ_PRI3         3     // Priority level 3
#define NVIC_IRQ_PRI4         4     // Priority level 4
#define NVIC_IRQ_PRI5         5     // Priority level 5
#define NVIC_IRQ_PRI6         6     // Priority level 6
#define NVIC_IRQ_PRI7         7     // Priority level 7
#define NVIC_IRQ_PRI8         8     // Priority level 8
#define NVIC_IRQ_PRI9         9     // Priority level 9
#define NVIC_IRQ_PRI10        10    // Priority level 10
#define NVIC_IRQ_PRI11        11    // Priority level 11
#define NVIC_IRQ_PRI12        12    // Priority level 12
#define NVIC_IRQ_PRI13        13    // Priority level 13
#define NVIC_IRQ_PRI14        14    // Priority level 14
#define NVIC_IRQ_PRI15        15    // Lowest priority level


/****************************************************************************
 *              SPI Peripheral Register Bit Positions
 ****************************************************************************/

/*
 * SPI Control Register 1 (SPI_CR1)
 */
#define SPI_CR1_CPHA          0   // Clock Phase: 0 = first edge, 1 = second edge sampling
#define SPI_CR1_CPOL          1   // Clock Polarity: 0 = clock low when idle, 1 = clock high when idle
#define SPI_CR1_MSTR          2   // Master Selection: 1 = Master mode, 0 = Slave mode
#define SPI_CR1_BR            3   // Baud Rate Control (bits 5:3): f_PCLK / 2^(BR+1)
#define SPI_CR1_SPE           6   // SPI Enable: 1 = enable SPI peripheral
#define SPI_CR1_LSB_FIRST     7   // Frame Format: 1 = LSB first, 0 = MSB first
#define SPI_CR1_SSI           8   // Internal Slave Select (used when SSM = 1)
#define SPI_CR1_SSM           9   // Software Slave Management Enable
#define SPI_CR1_RX_ONLY       10  // Receive Only: 1 = receive-only mode (in master)
#define SPI_CR1_DFF           11  // Data Frame Format: 0 = 8-bit, 1 = 16-bit
#define SPI_CR1_CRC_NEXT      12  // Transmit CRC Next: next transfer is CRC
#define SPI_CR1_CRC_EN        13  // CRC Calculation Enable
#define SPI_CR1_BIDI_OE       14  // Output Enable in Bidirectional Mode
#define SPI_CR1_BIDI_MODE     15  // Bidirectional Data Mode Enable: 1 = 1-line, 0 = 2-line

/*
 * SPI Control Register 2 (SPI_CR2)
 */
#define SPI_CR2_RXDMAEN       0   // RX Buffer DMA Enable
#define SPI_CR2_TXDMAEN       1   // TX Buffer DMA Enable
#define SPI_CR2_SSOE          2   // SS Output Enable (for master mode)
#define SPI_CR2_FRF           4   // Frame Format: 0 = Motorola, 1 = TI frame format
#define SPI_CR2_ERRIE         5   // Error Interrupt Enable
#define SPI_CR2_RXNEIE        6   // RX Buffer Not Empty Interrupt Enable
#define SPI_CR2_TXEIE         7   // TX Buffer Empty Interrupt Enable

/*
 * SPI Status Register (SPI_SR)
 */
#define SPI_SR_RXNE           0   // Receive Buffer Not Empty
#define SPI_SR_TXE            1   // Transmit Buffer Empty
#define SPI_SR_CHSIDE         2   // Channel Side (I2S mode only)
#define SPI_SR_UDR            3   // Underrun Flag (slave mode)
#define SPI_SR_CRC_ERR        4   // CRC Error Flag
#define SPI_SR_MODF           5   // Mode Fault (master/slave conflict)
#define SPI_SR_OVR            6   // Overrun Flag (data lost)
#define SPI_SR_BSY            7   // Busy Flag: 1 = SPI currently transmitting/receiving
#define SPI_SR_FRE            8   // Frame Format Error (TI mode only)

/*
 * Possible SPI Application States
 */
#define SPI_READY             0   // SPI is ready for new communication
#define SPI_BUSY_IN_RX        1   // SPI is busy in reception
#define SPI_BUSY_IN_TX        2   // SPI is busy in transmission

/*
 * Possible SPI Application Events
 */
#define SPI_EVENT_TX_CMPLT    1   // Transmission Complete Event
#define SPI_EVENT_RX_CMPLT    2   // Reception Complete Event
#define SPI_EVENT_OVR_ERR     3   // Overrun Error Event
#define SPI_EVENT_CRC_ERR     4   // CRC Error Event


/****************************************************************************
 *              I2C Peripheral Register Bit Positions
 ****************************************************************************/

/*
 * I2C Control Register 1 (I2C_CR1)
 */
#define I2C_CR1_PE             0   // Peripheral Enable: 1 = I2C peripheral enabled
#define I2C_CR1_SMBUS          1   // SMBus Mode: 1 = SMBus mode, 0 = I2C mode
#define I2C_CR1_RFU            2   // Reserved (do not use)
#define I2C_CR1_SMB_TYPE       3   // SMBus Type: 1 = SMBus host, 0 = SMBus device
#define I2C_CR1_ENARP          4   // ARP Enable: enable ARP protocol (SMBus)
#define I2C_CR1_ENPEC          5   // PEC Enable: enable Packet Error Checking
#define I2C_CR1_ENGC           6   // General Call Enable: respond to address 0x00
#define I2C_CR1_NO_STRETCH     7   // Clock Stretch Disable (slave mode)
#define I2C_CR1_START          8   // Start Generation (master mode)
#define I2C_CR1_STOP           9   // Stop Generation (master mode)
#define I2C_CR1_ACK            10  // Acknowledge Enable: 1 = send ACK after receive
#define I2C_CR1_POS            11  // ACK/PEC Position (for reception phase)
#define I2C_CR1_PEC            12  // Packet Error Checking value / PEC request
#define I2C_CR1_ALERT          13  // SMBus Alert: drive SMBALERT low
#define I2C_CR1_RFU2           14  // Reserved (do not use)
#define I2C_CR1_SWRST          15  // Software Reset: resets the I2C peripheral

/*
 * I2C Control Register 2 (I2C_CR2)
 */
#define I2C_CR2_FREQ           0   // Peripheral clock frequency (bits 5:0, in MHz)
#define I2C_CR2_ITERREN        8   // Error Interrupt Enable
#define I2C_CR2_ITEVTEN        9   // Event Interrupt Enable
#define I2C_CR2_ITBUFEN        10  // Buffer Interrupt Enable (TXE/RXNE)
#define I2C_CR2_DMAEN          11  // DMA Requests Enable
#define I2C_CR2_LAST           12  // DMA Last Transfer Enable

/*
 * I2C Status Register 1 (I2C_SR1)
 */
#define I2C_SR1_SB             0   // Start Bit: set after START condition generated
#define I2C_SR1_ADDR           1   // Address Sent/Matched flag
#define I2C_SR1_BTF            2   // Byte Transfer Finished
#define I2C_SR1_ADD10          3   // 10-bit Address Header Sent (master mode)
#define I2C_SR1_STOPF          4   // Stop Detection (slave mode)
#define I2C_SR1_RFU1           5   // Reserved (do not use)
#define I2C_SR1_RXNE           6   // Receive Buffer Not Empty
#define I2C_SR1_TXE            7   // Transmit Buffer Empty
#define I2C_SR1_BERR           8   // Bus Error (illegal start/stop)
#define I2C_SR1_ARLO           9   // Arbitration Lost (multi-master)
#define I2C_SR1_AF             10  // Acknowledge Failure (NACK received)
#define I2C_SR1_OVR            11  // Overrun/Underrun Error
#define I2C_SR1_PECERR         12  // PEC Error (SMBus)
#define I2C_SR1_RFU2           13  // Reserved (do not use)
#define I2C_SR1_TIMEOUT        14  // Timeout or Tlow Error
#define I2C_SR1_SMBALERT       15  // SMBus Alert Flag

/*
 * I2C Status Register 2 (I2C_SR2)
 */
#define I2C_SR2_MSL            0   // Master/Slave: 1 = Master mode
#define I2C_SR2_BUSY           1   // Bus Busy: 1 = communication ongoing
#define I2C_SR2_TRA            2   // Transmitter/Receiver: 1 = Transmitter mode
#define I2C_SR2_GENCALL        4   // General Call Address Detected
#define I2C_SR2_SMBDEFAULT     5   // SMBus Default Address (0x61) match
#define I2C_SR2_SMBHOST        6   // SMBus Host Header Detected
#define I2C_SR2_DUALF          7   // Dual Address Flag (OAR2 matched)
#define I2C_SR2_PEC            8   // Packet Error Checking value (SMBus)

 /*
  * I2C Clock Control Register (I2C_CCR)
  */
#define I2C_CCR_CCR            0   // Clock Control bits (11:0): define SCL period
#define I2C_CCR_DUTY           14  // Fast Mode Duty Cycle: 0 = Tlow/Thigh=2, 1=16/9
#define I2C_CCR_FS             15  // Fast/Standard Mode: 0=Standard (≤100kHz), 1=Fast (≤400kHz)

/*
 * Bit position definitions I2C_OAR1 — Own Address Register
 */

#define I2C_OAR1_ADD0           0   // Bit 0     – Address bit 0 (7- or 10-bit mode)
#define I2C_OAR1_ADD71          1   // Bits 7:1  – 7-bit address bits
#define I2C_OAR1_ADD98          8   // Bits 9:8  – Upper bits for 10-bit addressing
#define I2C_OAR1_ADDMODE        15  // Bit 15    – Address mode: 0=7-bit, 1=10-bit

/*
 * RCC Bits position
 */
#define RCC_SWS                 2  // Set and cleared by hardware to indicate which clock source is used as the system clock.
#define RCC_HPRE                4  // Set and cleared by software to control AHB clock division factor.
#define RCC_PPRE1               10 // Set and cleared by software to control APB low-speed clock division factor.

/****************************************************************************
 *                              Generic macros                              *
 ****************************************************************************/
#define ENABLE          1
#define DISABLE         0
#define SET             ENABLE
#define RESET           DISABLE
#define GPIO_PIN_SET    SET
#define GPIO_PIN_RESET  RESET
#define FLAG_RESET      RESET
#define FLAG_SET        SET

#endif /* INC_STM32F407XX_H_ */
