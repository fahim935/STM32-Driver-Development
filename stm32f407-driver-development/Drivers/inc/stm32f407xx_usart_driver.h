/*******************************************************************************
 * @file           stm32f407xx_usart_driver.h
 * @brief          Brief description
 *
 * @author         Fahim Akhtar
 * @date           24-Nov-2025
 * @time           9:38:29 PM
 * @version        1.0
 * @copyright      (c) 2025
 ******************************************************************************/

#ifndef INC_STM32F407XX_USART_DRIVER_H_
#define INC_STM32F407XX_USART_DRIVER_H_

#include  "stm32f407xx.h"

/*******************************************************************
 * @struct          USART_Config_t
 * @brief           Configuration structure for USART peripheral
 *                  These parameters are used to configure the USART in Init().
 ********************************************************************/
typedef struct
{
    uint8_t  USART_Mode;          // Select TX, RX, or TX+RX mode
    uint8_t  USART_NoOfStopBits;  // Number of stop bits used in frame
    uint8_t  USART_WordLength;    // Frame word length (8-bit or 9-bit)
    uint8_t  USART_ParityControl; // Parity configuration (None/Even/Odd)
    uint8_t  USART_HWFlowControl; // Hardware flow control (RTS/CTS)
    uint32_t USART_Baud;          // USART communication baud rate
} USART_Config_t;


/*******************************************************************
 * @struct          USART_Handle_t
 * @brief           Handle structure for USART peripheral
 ********************************************************************/
typedef struct
{
    USART_RegDef_t   *pUSARTx;      // Pointer to the USART peripheral base address
    USART_Config_t   USART_Config;  // USART configuration settings
    uint8_t *pTxBuffer;
    uint8_t *pRxBuffer;
    uint32_t TxLen;
    uint32_t RxLen;
    uint8_t TxBusyState;
    uint8_t RxBusyState;
}USART_Handle_t;


/*
 *@USART_Mode
 *Possible options for USART_Mode
 */
#define USART_MODE_ONLY_TX      0
#define USART_MODE_ONLY_RX      1
#define USART_MODE_TXRX         2

/*
 * Application states
 */
#define USART_READY             0
#define USART_BUSY_IN_RX        1
#define USART_BUSY_IN_TX        2

/*
 * USART flags
 */

#define USART_FLAG_TXE          ( 1 << USART_SR_TXE)
#define USART_FLAG_RXNE         ( 1 << USART_SR_RXNE)
#define USART_FLAG_TC           ( 1 << USART_SR_TC)

/*
 *@USART_Baud
 *Possible options for USART_Baud
 */
#define USART_STD_BAUD_1200         1200
#define USART_STD_BAUD_2400         2400
#define USART_STD_BAUD_9600         9600
#define USART_STD_BAUD_19200        19200
#define USART_STD_BAUD_38400        38400
#define USART_STD_BAUD_57600        57600
#define USART_STD_BAUD_115200       115200
#define USART_STD_BAUD_230400       230400
#define USART_STD_BAUD_460800       460800
#define USART_STD_BAUD_921600       921600
#define USART_STD_BAUD_2M           2000000
#define SUART_STD_BAUD_3M           3000000

/*
 *@USART_ParityControl
 *Possible options for USART_ParityControl
 */
#define USART_PARITY_DISABLE        0
#define USART_PARITY_EN_EVEN        1
#define USART_PARITY_EN_ODD         2

/*
 *@USART_WordLength
 *Possible options for USART_WordLength
 */
#define USART_WORDLEN_8BITS         0
#define USART_WORDLEN_9BITS         1

/*
 *@USART_NoOfStopBits
 *Possible options for USART_NoOfStopBits
 */
#define USART_STOPBITS_1            0
#define USART_STOPBITS_0_5          1
#define USART_STOPBITS_2            2
#define USART_STOPBITS_1_5          3

/*
 *@USART_HWFlowControl
 *Possible options for USART_HWFlowControl
 */
#define USART_HW_FLOW_CTRL_NONE     0
#define USART_HW_FLOW_CTRL_CTS      1
#define USART_HW_FLOW_CTRL_RTS      2
#define USART_HW_FLOW_CTRL_CTS_RTS  3

#define     USART_EVENT_TX_CMPLT    0
#define     USART_EVENT_RX_CMPLT    1
#define     USART_EVENT_IDLE        2
#define     USART_EVENT_CTS         3
#define     USART_EVENT_PE          4
#define     USART_ERR_FE            5
#define     USART_ERR_NE            6
#define     USART_ERR_ORE           7

/******************************************************************************************
 *                              APIs supported by this driver
 *       For more information about the APIs check the function definitions
 ******************************************************************************************/

/*******************************************************************
 * @fn              -   USART_PeriClockControl
 *
 * @brief           -   Enables or disables clock for the given USART peripheral
 *
 * @param[in]       -   pUSARTx : Pointer to USART peripheral base address
 * @param[in]       -   EnorDi  : ENABLE or DISABLE macro
 *
 * @return          -   None
 *
 * @note            -   Must be called before accessing USART registers
 ********************************************************************/
void USART_PeriClockControl(USART_RegDef_t *pUSARTx, uint8_t EnorDi);

/*******************************************************************
 * @fn              -   USART_Init
 *
 * @brief           -   Initializes the USART peripheral according to
 *                      the configuration in USART_Config structure
 *
 * @param[in]       -   pUSARTHandle : Handle structure with config and base address
 *
 * @return          -   None
 *
 * @note            -   Configures baud rate, word length, parity,
 *                      stop bits, mode, and flow control
 ********************************************************************/
void USART_Init(USART_Handle_t *pUSARTHandle);

/*******************************************************************
 * @fn              -   USART_DeInit
 *
 * @brief           -   Resets the USART peripheral registers to defaults
 *
 * @param[in]       -   pUSARTx : Pointer to USART peripheral base address
 *
 * @return          -   None
 *
 * @note            -   Uses RCC reset mechanism (APB1RSTR/APB2RSTR)
 ********************************************************************/
void USART_DeInit(USART_RegDef_t *pUSARTx);

/*******************************************************************
 * @fn              -   USART_SendData
 *
 * @brief           -   Sends data in blocking mode (polling)
 *
 * @param[in]       -   pUSARTHandle : USART handle structure
 * @param[in]       -   pTxBuffer : Pointer to transmit buffer
 * @param[in]       -   Len       : Number of bytes to send
 *
 * @return          -   None
 *
 * @note            -   Waits for TXE and TC flags internally
 ********************************************************************/
void USART_SendData(USART_Handle_t *pUSARTHandle, uint8_t *pTxBuffer, uint32_t Len);

/*******************************************************************
 * @fn              -   USART_ReceiveData
 *
 * @brief           -   Receives data in blocking mode (polling)
 *
 * @param[in]       -   pUSARTHandle : USART handle structure
 * @param[in]       -   pRxBuffer : Pointer to receive buffer
 * @param[in]       -   Len       : Number of bytes to receive
 *
 * @return          -   None
 *
 * @note            -   Handles parity and word length internally
 ********************************************************************/
void USART_ReceiveData(USART_Handle_t *pUSARTHandle, uint8_t *pRxBuffer, uint32_t Len);

/*******************************************************************
 * @fn              -   USART_SendDataIT
 *
 * @brief           -   Sends data using interrupt mode
 *
 * @param[in]       -   pUSARTHandle : USART handle structure
 * @param[in]       -   pTxBuffer    : Pointer to transmit buffer
 * @param[in]       -   Len          : Number of bytes to send
 *
 * @return          -   Tx state (BUSY_IN_TX / READY)
 *
 * @note            -   Enables TXEIE and TCIE interrupts
 ********************************************************************/
uint8_t USART_SendDataIT(USART_Handle_t *pUSARTHandle, uint8_t *pTxBuffer, uint32_t Len);

/*******************************************************************
 * @fn              -   USART_ReceiveDataIT
 *
 * @brief           -   Receives data using interrupt mode
 *
 * @param[in]       -   pUSARTHandle : USART handle structure
 * @param[in]       -   pRxBuffer    : Pointer to receive buffer
 * @param[in]       -   Len          : Number of bytes to receive
 *
 * @return          -   Rx state (BUSY_IN_RX / READY)
 *
 * @note            -   Enables RXNEIE interrupt
 ********************************************************************/
uint8_t USART_ReceiveDataIT(USART_Handle_t *pUSARTHandle, uint8_t *pRxBuffer, uint32_t Len);

/*******************************************************************
 * @fn              -   USART_IRQInterruptConfig
 *
 * @brief           -   Enables or disables an NVIC interrupt line
 *
 * @param[in]       -   IRQNumber : IRQ position in NVIC
 * @param[in]       -   EnorDi    : ENABLE or DISABLE
 *
 * @return          -   None
 *
 * @note            -   Used for USART global interrupt enabling
 ********************************************************************/
void USART_IRQInterruptConfig(uint8_t IRQNumber, uint8_t EnorDi);

/*******************************************************************
 * @fn              -   USART_IRQPriorityConfig
 *
 * @brief           -   Configures priority of a given interrupt
 *
 * @param[in]       -   IRQNumber    : USART IRQ number
 * @param[in]       -   IRQPriority  : Priority value
 *
 * @return          -   None
 *
 * @note            -   Lower value means higher priority
 ********************************************************************/
void USART_IRQPriorityConfig(uint8_t IRQNumber, uint32_t IRQPriority);

/*******************************************************************
 * @fn              -   USART_IRQHandling
 *
 * @brief           -   Handles all USART interrupt events
 *
 * @param[in]       -   pHandle : USART handle structure
 *
 * @return          -   None
 *
 * @note            -   Handles TXE, TC, RXNE, CTS, ORE, FE, PE flags
 ********************************************************************/
void USART_IRQHandling(USART_Handle_t *pHandle);


/*******************************************************************
 * @fn              -   USART_PeripheralControl
 *
 * @brief           -   Enables or disables the USART peripheral
 *
 * @param[in]       -   pUSARTx : USART peripheral base address
 * @param[in]       -   EnOrDi  : ENABLE or DISABLE
 *
 * @return          -   None
 *
 * @note            -   Controls UE bit in CR1 register
 ********************************************************************/
void USART_PeripheralControl(USART_RegDef_t *pUSARTx, uint8_t EnOrDi);

/*******************************************************************
 * @fn              -   USART_GetFlagStatus
 *
 * @brief           -   Reads the status of a USART flag
 *
 * @param[in]       -   pUSARTx  : USART peripheral base address
 * @param[in]       -   FlagName : USART_SR flag name
 *
 * @return          -   FLAG_SET or FLAG_RESET
 *
 * @note            -   Checks bits in the SR register
 ********************************************************************/
uint8_t USART_GetFlagStatus(USART_RegDef_t *pUSARTx, uint32_t FlagName);

/*******************************************************************
 * @fn              -   USART_ClearFlag
 *
 * @brief           -   Clears specific USART status flags
 *
 * @param[in]       -   pUSARTx         : USART peripheral base address
 * @param[in]       -   StatusFlagName  : Flag to clear
 *
 * @return          -   None
 *
 * @note            -   Some flags are cleared by software, others by
 *                      reading SR/DR sequence (PE, FE, ORE, NF)
 ********************************************************************/
void USART_ClearFlag(USART_RegDef_t *pUSARTx, uint16_t StatusFlagName);

/*******************************************************************
 * @fn          - USART_SetBaudRate
 *
 * @brief       - Configures the baud rate of the given USART peripheral.
 *
 * @param[in]   - pUSARTx : Pointer to the USART peripheral base address
 * @param[in]   - BaudRate: Desired baud rate (e.g., 9600, 115200)
 *
 * @return      - None
 *
 * @note        - This function calculates the value of the BRR register
 *                based on the peripheral clock frequency and the desired
 *                baud rate. The formula differs depending on whether the
 *                USART is configured for oversampling by 16 or oversampling
 *                by 8. The function writes the computed mantissa and
 *                fraction values into the USART_BRR register.
 *
 ********************************************************************/
void USART_SetBaudRate(USART_RegDef_t *pUSARTx, uint32_t BaudRate);


/*******************************************************************
 * @fn              -   USART_ApplicationEventCallback
 *
 * @brief           -   Application callback for USART events
 *
 * @param[in]       -   pUSARTHandle : USART handle
 * @param[in]       -   AppEv        : Event (TX complete, RX complete, error)
 *
 * @return          -   None
 *
 * @note            -   Weak implementation; user overrides it
 ********************************************************************/
void USART_ApplicationEventCallback(USART_Handle_t *pUSARTHandle, uint8_t AppEv);



#endif /* INC_STM32F407XX_USART_DRIVER_H__ */
