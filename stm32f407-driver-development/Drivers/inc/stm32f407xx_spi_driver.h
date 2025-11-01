/*******************************************************************************
 * @file           stm32f407xx_spi_driver.h
 * @brief          SPI driver header for STM32F407xx microcontroller
 *
 * This file contains macros, data structures, and function prototypes for
 * configuring and using the SPI peripheral in master/slave mode, with support
 * for polling and interrupt-based communication.
 * 
 * @author         Fahim Akhtar
 * @date           23-Oct-2025
 * @time           10:54:55 PM
 * @version        1.0
 * @copyright      (c) 2025
 ******************************************************************************/

#ifndef INC_STM32F407XX_SPI_DRIVER_H_
#define INC_STM32F407XX_SPI_DRIVER_H_

#include  "stm32f407xx.h"

/*******************************************************************
 * @struct          SPI_Config_t
 * @brief           Configuration structure for SPI peripheral
 ********************************************************************/
typedef struct
{
    uint8_t SPI_DeviceMode;         // Master or slave
    uint8_t SPI_BusConfig;          // Full Duplex or Half Duplex or Simplex
    uint8_t SPI_SclkSpeed;          // Clock speed of the SPI Bus
    uint8_t SPI_DFF;                // Data Frame format (8bits/ 16bits)
    uint8_t SPI_CPOL;               // Clock polarity
    uint8_t SPI_CPHA;               // Clock phase
    uint8_t SPI_SSM;                // Slave Select management (Hardware or software)
}SPI_Config_t;

/*******************************************************************
 * @struct          SPI_Handle_t
 * @brief           Handle structure for SPI peripheral
 ********************************************************************/
typedef struct
{
    SPI_RegDef_t    *pSPIx; // Holds BaseAddr of SPIx Base-address
    SPI_Config_t    SPIConfig;
    uint8_t         *pTxBuffer; // To store the app. Tx Buffer address
    uint8_t         *pRxBuffer; // To store the app. Rx Buffer address
    uint32_t        TxLen;      // To store Tx Length
    uint32_t        RxLen;      // To store Rx Length
    uint8_t         TxState;    // To store Tx state
    uint8_t         RxState;    // To store Rx state
}SPI_Handle_t;

/****************************************************************************
 *              SPI Configuration modes: @SPI_Configuration                 *
 ****************************************************************************/

/*
 * @SPI_DeviceMode
 * */
#define SPI_DEVICE_SLAVE_MODE           0   // Slave Mode
#define SPI_DEVICE_MASTER_MODE          1   // Master Mode

/*
 * @SPI_BusConfig
 * */
#define SPI_BUS_CONFIG_FD               1   // Full Duplex
#define SPI_BUS_CONFIG_HD               2   // Half Duplex
#define SPI_BUS_CONFIG_SIMPLEX_RXONLY   3   // Simplex Rx. Only
#define SPI_BUS_CONFIG_SIMPLEX_TXONLY   4   // Simplex Tx. Only // remove MISO line.

/*
 * @SPI_SclkSpeed
 * */
#define SPI_SCLK_SPEED_DIV2             0   // Prescaler divide by 2 (Default - Minimum)
#define SPI_SCLK_SPEED_DIV4             1   // Prescaler divide by 4
#define SPI_SCLK_SPEED_DIV8             2   // Prescaler divide by 8
#define SPI_SCLK_SPEED_DIV16            3   // Prescaler divide by 16
#define SPI_SCLK_SPEED_DIV32            4   // Prescaler divide by 32
#define SPI_SCLK_SPEED_DIV64            5   // Prescaler divide by 64
#define SPI_SCLK_SPEED_DIV128           6   // Prescaler divide by 128
#define SPI_SCLK_SPEED_DIV256           7   // Prescaler divide by 256

/*
 * @SPI_SclkSpeed
 * */
#define SPI_DFF_8BITS       0   // 8-Bits Data Format
#define SPI_DFF_16BITS      1   // 16-Bits Data Format

/*
 * @SPI_CPOL
 * */
#define SPI_CPOL_LOW        0
#define SPI_CPOL_HIGH       1

/*
 * @SPI_CPHA
 * */
#define SPI_CPHA_LOW        0
#define SPI_CPHA_HIGH       1

/*
 * @SPI_SSM
 * */
#define SPI_SSM_EN        0
#define SPI_SSM_DI        1

/*
 * SPI Status flag definitions
 * */
#define SPI_RXNE_FLAG       (1 << SPI_SR_RXNE)
#define SPI_TXE_FLAG        (1 << SPI_SR_TXE)
#define SPI_CHSIDE_FLAG     (1 << SPI_SR_CHSIDE)
#define SPI_UDR_FLAG        (1 << SPI_SR_UDR)
#define SPI_CRCERR_FLAG     (1 << SPI_SR_CRC_ERR)
#define SPI_MODF_FLAG       (1 << SPI_SR_MODF)
#define SPI_OVR_FLAG        (1 << SPI_SR_OVR)
#define SPI_BSY_FLAG        (1 << SPI_SR_BSY)
#define SPI_FRE_FLAG        (1 << SPI_SR_FRE)


/*******************************************************************
 * @fn              -   SPI_PeriClockControl
 * @brief           -   Enables or disables SPI peripheral clock
 * @param[in]       -   pSPIx: SPI peripheral base address
 * @param[in]       -   EnorDi: ENABLE or DISABLE macro
 * @return          -   None
 * @Note            -   Must be called before accessing SPI registers
 ********************************************************************/
void SPI_PeriClockControl(SPI_RegDef_t *pSPIOx, uint8_t EnorDi);

/*******************************************************************
 * @fn              -   SPI_Init
 * @brief           -   Initializes SPI peripheral with user config
 * @param[in]       -   pSPIHandle: Pointer to SPI handle structure
 * @return          -   None
 * @Note            -   Must be called after enabling peripheral clock
 ********************************************************************/
void SPI_Init(SPI_Handle_t *pSPIHandle);

/*******************************************************************
 * @fn              -   SPI_DeInit
 * @brief           -   Resets SPI peripheral registers
 * @param[in]       -   pSPIx: SPI peripheral base address
 * @return          -   None
 * @Note            -   Uses RCC reset macros
 ********************************************************************/
void SPI_DeInit(SPI_RegDef_t *pSPIx);

/*******************************************************************
 * @fn              -   SPI_SendData_Polling
 * @brief           -   Sends data over SPI using polling
 * @param[in]       -   pSPIx: SPI peripheral base address
 * @param[in]       -   pTxBuffer: Pointer to transmit buffer
 * @param[in]       -   Len: Number of bytes to send
 * @return          -   None
 * @Note            -   Blocking call
 ********************************************************************/
void SPI_SendData_Polling(SPI_RegDef_t *pSPIx, uint8_t *pTxBuffer, uint32_t Len);

/*******************************************************************
 * @fn              -   SPI_ReceiveData_Polling
 * @brief           -   Receives data over SPI using polling
 * @param[in]       -   pSPIx: SPI peripheral base address
 * @param[in]       -   pRxBuffer: Pointer to receive buffer
 * @param[in]       -   Len: Number of bytes to receive
 * @return          -   None
 * @Note            -   Blocking call
 ********************************************************************/
void SPI_ReceiveData_Polling(SPI_RegDef_t *pSPIx, uint8_t *pRxBuffer, uint32_t Len);

/*******************************************************************
 * @fn              -   SPI_CloseTransmission
 *
 * @brief           -   Closes SPI transmission and resets state
 *
 * @param[in]       -   pSPIHandle: Pointer to SPI handle structure
 *
 * @return          -   None
 *
 * @Note            -   Disables TXEIE and clears buffer pointers
 ********************************************************************/
void SPI_CloseTransmission(SPI_Handle_t *pSPIHandle);

/*******************************************************************
 * @fn              -   SPI_CloseReception
 *
 * @brief           -   Closes SPI reception and resets state
 *
 * @param[in]       -   pSPIHandle: Pointer to SPI handle structure
 *
 * @return          -   None
 *
 * @Note            -   Disables RXNEIE and clears buffer pointers
 ********************************************************************/
void SPI_CloseReception(SPI_Handle_t *pSPIHandle);

/*******************************************************************
 * @fn              -   SPI_ClearOVRFlag
 *
 * @brief           -   Clears Overrun (OVR) flag by dummy read
 *
 * @param[in]       -   pSPIHandle: Pointer to SPI handle structure
 *
 * @return          -   None
 *
 * @Note            -   Must be called after OVR error
 ********************************************************************/
void SPI_ClearOVRFlag(SPI_Handle_t *pSPIHandle);

/*******************************************************************
 * @fn              -   SPI_SendDataIT
 * @brief           -   Sends data over SPI using interrupt
 * @param[in]       -   pSPIHandle: Pointer to SPI handle structure
 * @param[in]       -   pTxBuffer: Pointer to transmit buffer
 * @param[in]       -   Len: Number of bytes to send
 * @return          -   SPI state before transmission
 * @Note            -   Non-blocking call
 ********************************************************************/
uint8_t SPI_SendDataIT(SPI_Handle_t *pSPIHandle, uint8_t *pTxBuffer, uint32_t Len);

/*******************************************************************
 * @fn              -   SPI_ReceiveDataIT
 * @brief           -   Receives data over SPI using interrupt
 * @param[in]       -   pSPIHandle: Pointer to SPI handle structure
 * @param[in]       -   pRxBuffer: Pointer to receive buffer
 * @param[in]       -   Len: Number of bytes to receive
 * @return          -   SPI state before reception
 * @Note            -   Non-blocking call
 ********************************************************************/
uint8_t SPI_ReceiveDataIT(SPI_Handle_t *pSPIHandle, uint8_t *pRxBuffer, uint32_t Len);

/*******************************************************************
 * @fn              -   SPI_IRQInterruptConfig
 * @brief           -   Enables or disables SPI IRQ in NVIC
 * @param[in]       -   IRQNumber: IRQ number
 * @param[in]       -   EnorDi: ENABLE or DISABLE macro
 * @return          -   None
 * @Note            -   Maps IRQ to NVIC ISER/ICER
 ********************************************************************/
void SPI_IRQInterruptConfig(uint8_t IRQNumber, uint8_t EnorDi);

/*******************************************************************
 * @fn              -   SPI_IRQPriorityConfig
 * @brief           -   Sets priority for SPI IRQ
 * @param[in]       -   IRQNumber: IRQ number
 * @param[in]       -   IRQPriority: Priority level (0–15)
 * @return          -   None
 * @Note            -   Only upper 4 bits are implemented
 ********************************************************************/
void SPI_IRQPriorityConfig (uint8_t IRQNumber, uint32_t IRQPriority);

/*******************************************************************
 * @fn              -   SPI_IRQHandling
 * @brief           -   Handles SPI interrupt events
 * @param[in]       -   pHandle: Pointer to SPI handle structure
 * @return          -   None
 * @Note            -   Dispatches to TXE, RXNE, OVR handlers
 ********************************************************************/
void SPI_IRQHandling(SPI_Handle_t *pHandle);

/*******************************************************************
 * @fn              -   SPI_PeripheralControl
 * @brief           -   Enables or disables SPI peripheral
 * @param[in]       -   pSPIx: SPI peripheral base address
 * @param[in]       -   EnOrDi: ENABLE or DISABLE macro
 * @return          -   None
 * @Note            -   Controls SPE bit
 ********************************************************************/
void SPI_PeripheralControl(SPI_RegDef_t *pSPIx, uint8_t EnOrDi);

/*******************************************************************
 * @fn              -   SPI_SSIConfig
 *
 * @brief           -   Configures SSI bit for software slave management
 *
 * @param[in]       -   pSPIx: SPI peripheral base address
 * @param[in]       -   EnOrDi: ENABLE or DISABLE macro
 *
 * @return          -   None
 *
 * @Note            -   Required when SSM is enabled
 ********************************************************************/
void SPI_SSIConfig(SPI_RegDef_t *pSPIx, uint8_t EnOrDi);

/*******************************************************************
 * @fn              -   SPI_SSOEConfig
 *
 * @brief           -   Configures SSOE bit for NSS output enable
 *
 * @param[in]       -   pSPIx: SPI peripheral base address
 * @param[in]       -   EnOrDi: ENABLE or DISABLE macro
 *
 * @return          -   None
 *
 * @Note            -   Required for master mode NSS output
 ********************************************************************/
void SPI_SSOEConfig(SPI_RegDef_t *pSPIx, uint8_t EnOrDi);

/*******************************************************************
 * @fn              -   SPI_GetFlagStatus
 *
 * @brief           -   Returns the status of a specific SPI flag
 *
 * @param[in]       -   pSPIx: SPI peripheral base address
 * @param[in]       -   FlagName: Flag mask to check
 *
 * @return          -   FLAG_SET or FLAG_RESET
 *
 * @Note            -   None
 ********************************************************************/
uint8_t SPI_GetFlagStatus(SPI_RegDef_t *pSPIx, uint32_t FlagName);

/*******************************************************************
 * @fn              -   SPI_ApplicationEventCallback
 *
 * @brief           -   Weak callback to notify application of SPI events
 *
 * @param[in]       -   pSPIHandle: Pointer to SPI handle structure
 * @param[in]       -   AppEv: Event type (TX complete, RX complete, OVR error)
 *
 * @return          -   None
 *
 * @Note            -   Can be overridden by user application
 ********************************************************************/
void SPI_ApplicationEventCallback(SPI_Handle_t *pSPIHandle, uint8_t AppEv);


#endif /* INC_STM32F407XX_SPI_DRIVER_H__ */
