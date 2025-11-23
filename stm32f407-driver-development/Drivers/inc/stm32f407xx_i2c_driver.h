/*******************************************************************************
 * @file           stm32f407xx_i2c_driver.h
 * @brief          Brief description
 *
 * @author         Fahim Akhtar
 * @date           09-Nov-2025
 * @time           8:55:34 AM
 * @version        1.0
 * @copyright      (c) 2025
 ******************************************************************************/

#ifndef INC_STM32F407XX_I2C_DRIVER_H_
#define INC_STM32F407XX_I2C_DRIVER_H_

#include  "stm32f407xx.h"

/*
 * Configuration structure for I2Cx peripheral
 * */
typedef struct {
    uint32_t I2C_SCLSpeed;      /* SCL clock speed */
    uint8_t  I2C_DeviceAddress; /* Own device address */
    uint8_t  I2C_ACKControl;    /* ACK enable/disable */
    uint16_t I2C_FMDutyCycle;   /* Fast-mode duty cycle */
} I2C_Config_t;

/*
 * Handle structure for I2Cx peripheral
 * */
typedef struct {
    I2C_RegDef_t   *pI2Cx;      /* I2Cx Base address */
    I2C_Config_t    I2C_Config; /* User configuration */
    uint8_t        *pTxBuffer;  /* Tx Buffer Address */
    uint8_t        *pRxBuffer;  /* Rx Buffer Address */
    uint32_t        TxLen;      /* Tx Length */
    uint32_t        RxLen;      /* Rx Length */
    uint8_t         TxRxState;  /* Communication State */
    uint8_t         DevAddr;    /* Device Address */
    uint32_t        RxSize;     /* Rx Size */
    uint8_t         Sr;         /* Repeated state value*/
} I2C_Handle_t;


/*
 * I2C Application state
 * */
#define I2C_READY           0
#define I2C_BUSY_IN_RX      1
#define I2C_BUSY_IN_TX      2

/*
 * @I2C_SCLSpeed
 * */
#define I2C_SCL_SPEED_SM      100000  // Standard Mode : 100 KHz
#define I2C_SCL_SPEED_FM2K    200000  // Fast Mode : 200 KHz
#define I2C_SCL_SPEED_FM4K    400000  // Fast Mode : 400 KHz

/*
 * @I2C_ACKControl
 * */
#define I2C_ACK_ENABLE      1   // To enable set it to 1
#define I2C_ACK_DISABLE     0   // Default value

/*
 * @I2C_FMDutyCycle
 * */
#define I2C_FM_DUTY_2       0
#define I2C_FM_DUTY_16_9    1

/*
 * I2C Read / Write Bit
 * */
#define I2C_WRITE   0
#define I2C_READ    1

#define I2C_DISABLE_SR      RESET
#define I2C_ENABLE_SR       SET

/*******************************************************************
 * @defgroup        I2C_Flag_Definitions
 *
 * @brief           Bit-mask definitions for I2C SR1 status flags.
 *                  These flags are used with I2C_GetFlagStatus().
 *
 * @note            All flags map directly to SR1 bit positions.
 ********************************************************************/

#define I2C_FLAG_SB         (1 << I2C_SR1_SB)       /* Set when START condition generated */
#define I2C_FLAG_BTF        (1 << I2C_SR1_BTF)      /* Both DR and shift register empty */
#define I2C_FLAG_ADDR       (1 << I2C_SR1_ADDR)     /* Indicates ADDR event in Master/Slave */
#define I2C_FLAG_STOPF      (1 << I2C_SR1_STOPF)    /* Indicates STOPF event */
#define I2C_FLAG_BERR       (1 << I2C_SR1_BERR)     /* Misplaced START/STOP or illegal bus condition */
#define I2C_FLAG_ARLO       (1 << I2C_SR1_ARLO)     /* Another master won the arbitration */
#define I2C_FLAG_AF         (1 << I2C_SR1_AF)       /* NACK received from slave/master */
#define I2C_FLAG_RXNE       (1 << I2C_SR1_RXNE)     /* Data available in DR */
#define I2C_FLAG_TXE        (1 << I2C_SR1_TXE)      /* DR empty, ready for next byte */
#define I2C_FLAG_OVR        (1 << I2C_SR1_OVR)      /* Missed read or early write */
#define I2C_FLAG_PECERR     (1 << I2C_SR1_PECERR)   /* Packet Error Checking mismatch (SMBus) */
#define I2C_FLAG_TIMEOUT    (1 << I2C_SR1_TIMEOUT)  /* Timeout or Tlow error (SMBus) */
#define I2C_FLAG_SMBALERT   (1 << I2C_SR1_SMBALERT) /* SMBALERT line pulled low */
#define I2C_FLAG_ADD10      (1 << I2C_SR1_ADD10)    /* 10-bit header sent/received (Master mode only) */

/*******************************************************************
 * @defgroup        I2C_Application_Events
 *
 * @brief           Application-level event and error codes used by
 *                  I2C_ApplicationEventCallback() to notify the
 *                  upper layer about communication status.
 *
 * @note            These values do not map to hardware bits directly.
 *                  They represent logical events detected by the driver.
 ********************************************************************/

#define I2C_EV_TX_CMPLT     0   /* Transmission completed (Master mode) */
#define I2C_EV_RX_CMPLT     1   /* Reception completed (Master mode) */
#define I2C_EV_STOP         2   /* STOP condition detected (Slave mode) */

#define I2C_ERROR_BERR      3   /* Bus error detected */
#define I2C_ERROR_ARLO      4   /* Arbitration lost */
#define I2C_ERROR_AF        5   /* Acknowledge failure (NACK received) */
#define I2C_ERROR_OVR       6   /* Overrun/Underrun error */
#define I2C_ERROR_TIMEOUT   7   /* Timeout/Tlow error */

#define I2C_EV_DATA_REQ     8   /* Slave: master requesting data (TXE event) */
#define I2C_EV_DATA_RCV     9   /* Slave: data received from master (RXNE event) */


/*******************************************************************
 * @fn              -   I2C_PeriClockControl
 * @brief           -   Enables or disables I2C peripheral clock
 * @param[in]       -   pI2Cx: I2C peripheral base address
 * @param[in]       -   EnorDi: ENABLE or DISABLE macro
 * @return          -   None
 * @Note            -   Must be called before accessing I2C registers
 ********************************************************************/
void I2C_PeriClockControl(I2C_RegDef_t *pI2Cx, uint8_t EnorDi);

/*******************************************************************
 * @fn              -   I2C_PeripheralControl
 * @brief           -   Enables or disables I2C peripheral
 * @param[in]       -   pI2Cx: SPI peripheral base address
 * @param[in]       -   EnOrDi: ENABLE or DISABLE macro
 * @return          -   None
 * @Note            -   Controls PE bit in CR1 register
 ********************************************************************/
void I2C_PeripheralControl(I2C_RegDef_t *pI2Cx, uint8_t EnOrDi);

/*******************************************************************
 * @fn              -   I2C_Init
 * @brief           -   Initializes I2C peripheral with user config
 * @param[in]       -   pI2CHandle: Pointer to I2C handle structure
 * @return          -   None
 * @Note            -   Must be called after enabling peripheral clock
 ********************************************************************/
void I2C_Init(I2C_Handle_t *pI2CHandle);

/*******************************************************************
 * @fn              -   I2C_DeInit
 * @brief           -   Resets I2C peripheral registers
 * @param[in]       -   pI2Cx: I2C peripheral base address
 * @return          -   None
 * @Note            -   Uses RCC reset macros
 ********************************************************************/
void I2C_DeInit(I2C_RegDef_t *pI2Cx);

/*******************************************************************
 * @fn              -   I2C_IRQInterruptConfig
 * @brief           -   Enables or disables I2C IRQ in NVIC
 * @param[in]       -   IRQNumber: IRQ number
 * @param[in]       -   EnorDi: ENABLE or DISABLE macro
 * @return          -   None
 * @Note            -   Maps IRQ to NVIC ISER/ICER
 ********************************************************************/
void I2C_IRQInterruptConfig(uint8_t IRQNumber, uint8_t EnorDi);

/*******************************************************************
 * @fn              -   I2C_IRQPriorityConfig
 * @brief           -   Sets priority for I2C IRQ
 * @param[in]       -   IRQNumber: IRQ number
 * @param[in]       -   IRQPriority: Priority level (0–15)
 * @return          -   None
 * @Note            -   Only upper 4 bits are implemented
 ********************************************************************/
void I2C_IRQPriorityConfig (uint8_t IRQNumber, uint32_t IRQPriority);

/*******************************************************************
 * @fn              -   I2C_GetFlagStatus
 *
 * @brief           -   Returns the status of a specific I2C flag
 *
 * @param[in]       -   pI2Cx: I2C peripheral base address
 * @param[in]       -   FlagName: Flag mask to check
 *
 * @return          -   FLAG_SET or FLAG_RESET
 *
 * @Note            -   None
 ********************************************************************/
uint8_t I2C_GetFlagStatus(I2C_RegDef_t *pI2Cx, uint32_t FlagName);

/*******************************************************************
 * @fn              -   I2C_ApplicationEventCallback
 *
 * @brief           -   Weak callback to notify application of I2C events
 *
 * @param[in]       -   pI2CHandle: Pointer to I2C handle structure
 * @param[in]       -   AppEv: Event type (TX complete, RX complete, OVR error)
 *
 * @return          -   None
 *
 * @Note            -   Can be overridden by user application
 ********************************************************************/
void I2C_ApplicationEventCallback(I2C_Handle_t *pI2CHandle, uint8_t AppEv);

/*******************************************************************
 * @fn              -   I2C_ManageAcking
 *
 * @brief           -   Enables or disables ACKing for I2C
 *
 * @param[in]       -   pI2Cx : I2C peripheral base address
 * @param[in]       -   EnOrDi: Enable/Disable macro for ACK control
 *
 * @return          -   None
 *
 * @Note            -   Used in master receiver mode to NACK last byte
 ********************************************************************/
void I2C_ManageAcking(I2C_RegDef_t *pI2Cx, uint8_t EmOrDi);

/*******************************************************************
 * @fn              -   I2C_MasterSendData
 *
 * @brief           -   Sends data in blocking mode over I2C as master
 *
 * @param[in]       -   pI2CHandle : I2C handle structure
 * @param[in]       -   pTxBuffer  : Pointer to TX buffer
 * @param[in]       -   Len        : Number of bytes to send
 * @param[in]       -   SlaveAddr  : Slave device address
 * @param[in]       -   sr         : Repeated start enable/disable
 *
 * @return          -   None
 *
 * @Note            -   Blocks until all bytes are transmitted
 ********************************************************************/
void I2C_MasterSendData(I2C_Handle_t *pI2CHandle,
                        uint8_t *pTxBuffer,
                        uint32_t Len,
                        uint8_t SlaveAddr,
                        uint8_t sr);

/*******************************************************************
 * @fn              -   I2C_MasterReceiveData
 *
 * @brief           -   Receives data in blocking mode over I2C as master
 *
 * @param[in]       -   pI2CHandle : I2C handle structure
 * @param[in]       -   pRxBuffer  : Pointer to RX buffer
 * @param[in]       -   Len        : Number of bytes to receive
 * @param[in]       -   SlaveAddr  : Slave device address
 * @param[in]       -   sr         : Repeated start enable/disable
 *
 * @return          -   None
 *
 * @Note            -   Handles ACK/NACK logic internally
 ********************************************************************/
void I2C_MasterReceiveData(I2C_Handle_t *pI2CHandle,
                           uint8_t *pRxBuffer,
                           uint32_t Len,
                           uint8_t SlaveAddr,
                           uint8_t sr);

/*******************************************************************
 * @fn              -   I2C_MasterSendDataIT
 *
 * @brief           -   Sends data over I2C in interrupt mode (non-blocking)
 *
 * @param[in]       -   pI2CHandle : I2C handle structure
 * @param[in]       -   pTxBuffer  : Pointer to TX buffer
 * @param[in]       -   Len        : Number of bytes
 * @param[in]       -   SlaveAddr  : Slave device address
 * @param[in]       -   sr         : Repeated start enable/disable
 *
 * @return          -   I2C state (Busy/Ready)
 *
 * @Note            -   Transmission continues in IRQ context
 ********************************************************************/
uint8_t I2C_MasterSendDataIT(I2C_Handle_t *pI2CHandle,
                             uint8_t *pTxBuffer,
                             uint32_t Len,
                             uint8_t SlaveAddr,
                             uint8_t sr);

/*******************************************************************
 * @fn              -   I2C_MasterReceiveDataIT
 *
 * @brief           -   Receives data over I2C in interrupt mode (non-blocking)
 *
 * @param[in]       -   pI2CHandle : I2C handle structure
 * @param[in]       -   pRxBuffer  : Pointer to RX buffer
 * @param[in]       -   Len        : Number of bytes
 * @param[in]       -   SlaveAddr  : Slave device address
 * @param[in]       -   sr         : Repeated start enable/disable
 *
 * @return          -   I2C state (Busy/Ready)
 *
 * @Note            -   Handles ACK/NACK automatically in IRQ
 ********************************************************************/
uint8_t I2C_MasterReceiveDataIT(I2C_Handle_t *pI2CHandle,
                                uint8_t *pRxBuffer,
                                uint32_t Len,
                                uint8_t SlaveAddr,
                                uint8_t sr);


/*******************************************************************
 * @fn              -   I2C_EVT_IRQHandling
 *
 * @brief           -   Handles I2C event interrupt (SB, ADDR, BTF, TXE, RXNE)
 *
 * @param[in]       -   pI2CHandle : I2C handle structure
 *
 * @return          -   None
 *
 * @Note            -   Called inside the I2C EV IRQ handler
 ********************************************************************/
void I2C_EVT_IRQHandling(I2C_Handle_t *pI2CHandle);

/*******************************************************************
 * @fn              -   I2C_ERR_IRQHandling
 *
 * @brief           -   Handles I2C error interrupt (AF, BERR, ARLO, OVR)
 *
 * @param[in]       -   pI2CHandle : I2C handle structure
 *
 * @return          -   None
 *
 * @Note            -   Clears error flags and notifies application
 ********************************************************************/
void I2C_ERR_IRQHandling(I2C_Handle_t *pI2CHandle);

/*******************************************************************
 * @fn              -   I2C_CloseSendData
 *
 * @brief           -   Closes master TX communication and resets state
 *
 * @param[in]       -   pI2CHandle : I2C handle structure
 *
 * @return          -   None
 *
 * @Note            -   Called at end of interrupt-driven TX
 ********************************************************************/
void I2C_CloseSendData(I2C_Handle_t *pI2CHandle);

/*******************************************************************
 * @fn              -   I2C_CloseReceveData
 *
 * @brief           -   Closes master RX communication and resets state
 *
 * @param[in]       -   pI2CHandle : I2C handle structure
 *
 * @return          -   None
 *
 * @Note            -   Restores ACKing if disabled for last byte
 ********************************************************************/
void I2C_CloseReceveData(I2C_Handle_t *pI2CHandle);

/*******************************************************************
 * @fn              -   I2C_GenerateStopCondition
 *
 * @brief           -   Generates a STOP condition on the I2C bus
 *
 * @param[in]       -   pI2Cx : I2C peripheral base address
 *
 * @return          -   None
 *
 * @Note            -   Used by master to release the bus
 ********************************************************************/
void I2C_GenerateStopCondition(I2C_RegDef_t *pI2Cx);

/*******************************************************************
 * @fn              -   I2C_SlaveSendData
 *
 * @brief           -   Sends a single byte from slave to master
 *
 * @param[in]       -   pI2C : I2C peripheral base address
 * @param[in]       -   data : Byte to transmit
 *
 * @return          -   None
 *
 * @Note            -   Uses DR register; called during TXE in slave mode
 ********************************************************************/
void I2C_SlaveSendData(I2C_RegDef_t *pI2C, uint8_t data);

/*******************************************************************
 * @fn              -   I2C_SlaveReceiveData
 *
 * @brief           -   Reads a received byte in slave mode
 *
 * @param[in]       -   pI2C : I2C peripheral base address
 *
 * @return          -   Received byte
 *
 * @Note            -   Called when RXNE is set in slave mode
 ********************************************************************/
uint8_t I2C_SlaveReceiveData(I2C_RegDef_t *pI2C);

/*******************************************************************
 * @fn              -   I2C_SlaveEnableDisableCallbackEvents
 *
 * @brief           -   Enables/Disables slave mode callback event interrupts
 *
 * @param[in]       -   pI2C  : I2C peripheral base address
 * @param[in]       -   EnOrDi: Enable/Disable macro
 *
 * @return          -   None
 *
 * @Note            -   Controls ITBUFEN/ITEVTEN/ITERREN for slave callbacks
 ********************************************************************/
void I2C_SlaveEnableDisableCallbackEvents(I2C_RegDef_t *pI2C, uint8_t EnOrDi);


#endif /* INC_STM32F407XX_I2C_DRIVER_H__ */
