/*******************************************************************************
 * @file           stm32f407xx_spi_driver.c
 * @brief          SPI driver implementation for STM32F407xx microcontroller
 *
 * This file contains the low-level SPI peripheral driver functions including
 * initialization, data transmission/reception (polling and interrupt-based),
 * peripheral control, and interrupt handling. It supports full-duplex, half-duplex,
 * and simplex configurations with flexible frame formats and clock settings.
 * 
 * @author         Fahim Akhtar
 * @date           23-Oct-2025
 * @time           10:55:22 PM
 * @version        1.0
 * @copyright      (c) 2025
 ******************************************************************************/

#include "stm32f407xx_spi_driver.h"

// Internal helper functions for interrupt handling
static void spi_txe_interrupt_handle(SPI_Handle_t *pSPIHandle);
static void spi_rxne_interrupt_handle(SPI_Handle_t *pSPIHandle);
static void spi_ovr_err_interrupt_handle(SPI_Handle_t *pSPIHandle);

/*******************************************************************
 * @fn              -   SPI_GetFlagStatus
 *
 * @brief           -   Returns the status of a specific SPI flag
 *
 * @param[in]       -   pSPIx: Base address of SPI peripheral
 * @param[in]       -   FlagName: Flag mask to check
 *
 * @return          -   FLAG_SET or FLAG_RESET
 *
 * @Note            -   None
 ********************************************************************/
uint8_t SPI_GetFlagStatus(SPI_RegDef_t *pSPIx, uint32_t FlagName){
    if(pSPIx->SR & FlagName){
        return FLAG_SET;
    }
    return FLAG_RESET;
}

/*******************************************************************
 * @fn              -   SPI_PeriClockControl
 *
 * @brief           -   Enables or disables the SPI peripheral clock
 *
 * @param[in]       -   pSPIx: Base address of SPI peripheral
 * @param[in]       -   EnorDi: ENABLE or DISABLE macro
 *
 * @return          -   None
 *
 * @Note            -   Must be called before accessing SPI registers
 ********************************************************************/
void SPI_PeriClockControl(SPI_RegDef_t *pSPIOx, uint8_t EnorDi){
    if(EnorDi == ENABLE){
        if(pSPIOx == SPI1) {
            SPI1_PCLK_EN();
        } else if (pSPIOx == SPI2) {
            SPI2_PCLK_EN();
        } else if (pSPIOx == SPI3) {
            SPI3_PCLK_EN();
        }
    } else if(EnorDi == DISABLE)    {
        if(pSPIOx == SPI1) {
            SPI1_PCLK_DI();
        } else if (pSPIOx == SPI2) {
            SPI2_PCLK_DI();
        } else if (pSPIOx == SPI3) {
            SPI3_PCLK_DI();
        }
    } else {
        /*invalid option*/
    }
}

/*******************************************************************
 * @fn              -   SPI_Init
 *
 * @brief           -   Initializes the SPI peripheral with user-defined settings
 *
 * @param[in]       -   pSPIHandle: Pointer to SPI handle structure
 *
 * @return          -   None
 *
 * @Note            -   Configures mode, bus type, speed, polarity, phase, etc.
 ********************************************************************/
void SPI_Init(SPI_Handle_t *pSPIHandle){

    uint32_t tempReg = 0x00;

    //perpheral clock enable
    SPI_PeriClockControl(pSPIHandle->pSPIx, ENABLE);

    // Configure the device mode
    tempReg |= pSPIHandle->SPIConfig.SPI_DeviceMode << SPI_CR1_MSTR;

    // Configure the Bus mode
    if(pSPIHandle->SPIConfig.SPI_BusConfig == SPI_BUS_CONFIG_FD) {
        // BiDi Mode should be clear
        tempReg &= ~(1 << SPI_CR1_BIDI_MODE);
    } else if(pSPIHandle->SPIConfig.SPI_BusConfig == SPI_BUS_CONFIG_HD) {
        //BiDi Mode should be set
        tempReg |= 1 << SPI_CR1_BIDI_MODE;
    } else if(pSPIHandle->SPIConfig.SPI_BusConfig == SPI_BUS_CONFIG_SIMPLEX_RXONLY) {
        // BiDi Mode should be clear
        tempReg &= ~(1 << SPI_CR1_BIDI_MODE);
        // RXONLY bit must be set
        tempReg |= 1 << SPI_CR1_RX_ONLY;
    }

    // Configure the Clock Speed (Baud rate)
    tempReg |= pSPIHandle->SPIConfig.SPI_SclkSpeed << SPI_CR1_BR;

    // Configure the Data frame format
    tempReg |= pSPIHandle->SPIConfig.SPI_DFF << SPI_CR1_DFF;

    // Configure the CPOL
    tempReg |= pSPIHandle->SPIConfig.SPI_CPOL << SPI_CR1_CPOL;

    // Configure the CPHA
    tempReg |= pSPIHandle->SPIConfig.SPI_CPHA << SPI_CR1_CPHA;

    // Configure the Slave Select
    tempReg |= pSPIHandle->SPIConfig.SPI_SSM << SPI_CR1_SSM;

    pSPIHandle->pSPIx->CR1 = tempReg;

}

/*******************************************************************
 * @fn              -   SPI_DeInit
 *
 * @brief           -   Resets the SPI peripheral
 *
 * @param[in]       -   pSPIx: Base address of SPI peripheral
 *
 * @return          -   None
 *
 * @Note            -   Placeholder for future implementation
 ********************************************************************/
void SPI_DeInit(SPI_RegDef_t *pSPIx){
    if (pSPIx == SPI1)
    {
        SPI1_REG_RESET();
    }
    else if (pSPIx == SPI2)
    {
        SPI2_REG_RESET();
    }
    else if (pSPIx == SPI3)
    {
        SPI3_REG_RESET();
    }

}

/*******************************************************************
 * @fn              -   SPI_SendData_Polling
 *
 * @brief           -   Sends data over SPI using polling method
 *
 * @param[in]       -   pSPIx: Base address of SPI peripheral
 * @param[in]       -   pTxBuffer: Pointer to transmit buffer
 * @param[in]       -   len: Length of data to transmit
 *
 * @return          -   None
 *
 * @Note            -   Blocking call; waits for TXE flag
 ********************************************************************/
void SPI_SendData_Polling(SPI_RegDef_t *pSPIx, uint8_t *pTxBuffer, uint32_t len){

    while(len > 0){
        //1. Wait until the TXE is set
        // Polling mechanism (Not recommended )
        while(!((pSPIx->SR >> SPI_SR_TXE ) & 0x01));

        while(SPI_GetFlagStatus(pSPIx, SPI_TXE_FLAG) == FLAG_RESET);
        // Check the DFF bit to check the frame format
        // 16 Bits Data format
        if(pSPIx->CR1 & (1 << SPI_CR1_DFF)){
            pSPIx->DR = *(uint16_t *)pTxBuffer; // This dereference 2 bytes of data.
            len -= 2; // Decrease the data length by 2 byte
            //(uint16_t *)pTxBuffer++; // Move the data pointer by 2 byte
            pTxBuffer += 2;
        } else {
            pSPIx->DR = *pTxBuffer; // This dereference 1 bytes of data.
            len--;  // Decrease the data length by 1 byte
            pTxBuffer++;// Move the data pointer
        }
    }
}

/*******************************************************************
 * @fn              -   SPI_ReceiveData_Polling
 *
 * @brief           -   Receives data over SPI using polling method
 *
 * @param[in]       -   pSPIx: Base address of SPI peripheral
 * @param[in]       -   pRxBuffer: Pointer to receive buffer
 * @param[in]       -   len: Length of data to receive
 *
 * @return          -   None
 *
 * @Note            -   Blocking call; waits for RXNE flag
 ********************************************************************/
void SPI_ReceiveData_Polling(SPI_RegDef_t *pSPIx, uint8_t *pRxBuffer, uint32_t len){

    while(len > 0){
        //1. Wait until the RXNE is set
        // Polling mechanism (Not recommended )
        while(!((pSPIx->SR >> SPI_SR_RXNE ) & 0x01));

        while(SPI_GetFlagStatus(pSPIx, SPI_RXNE_FLAG) == FLAG_RESET);
        // Check the DFF bit to check the frame format
        // 16 Bits Data format
        if(pSPIx->CR1 & (1 << SPI_CR1_DFF)){
            *(uint16_t *)pRxBuffer = pSPIx->DR; // Copy two byte of data to Rx buffer.
            len -= 2; // Decrease the data length by 2 byte
            //(uint16_t *)pRxBuffer++; // Move the data pointer by 2 byte
            pRxBuffer +=2;
            //TODO : Write two byte of dummy data to generate the clock.
        } else {
            *pRxBuffer = pSPIx->DR; // Copy one byte of data to Rx buffer.
            len--;  // Decrease the data length by 1 byte
            pRxBuffer++;// Move the data pointer
            //TODO : Write one byte of dummy data to generate the clock.
        }
    }
}

/*******************************************************************
 * @fn              -   SPI_SendDataIT
 *
 * @brief           -   Initiates interrupt-based SPI transmission
 *
 * @param[in]       -   pSPIHandle: Pointer to SPI handle structure
 * @param[in]       -   pTxBuffer: Pointer to transmit buffer
 * @param[in]       -   Len: Length of data to transmit
 *
 * @return          -   SPI state before transmission
 *
 * @Note            -   Enables TXEIE interrupt
 ********************************************************************/
uint8_t SPI_SendDataIT(SPI_Handle_t *pSPIHandle, uint8_t *pTxBuffer, uint32_t Len){

    uint8_t state = pSPIHandle->TxState;

    if(state != SPI_BUSY_IN_TX){

        // 1. Save the Tx Buffer address and len info in global variables
        pSPIHandle->pTxBuffer = pTxBuffer;
        pSPIHandle->TxLen = Len;

        //2. Mark the SPI state as busy in transmission so the no other code can take
        //   over same SPI peripheral until transmission is over

        pSPIHandle->TxState = SPI_BUSY_IN_TX;

        //3. Enable the TXETE control bit to get interrupt whenever TXE flag is set in
        //   Status register (SR)
        pSPIHandle->pSPIx->CR2 |= (1 << SPI_CR2_TXEIE);
    }
    return state;
}

/*******************************************************************
 * @fn              -   SPI_ReceiveDataIT
 *
 * @brief           -   Initiates interrupt-based SPI reception
 *
 * @param[in]       -   pSPIHandle: Pointer to SPI handle structure
 * @param[in]       -   pRxBuffer: Pointer to receive buffer
 * @param[in]       -   Len: Length of data to receive
 *
 * @return          -   SPI state before reception
 *
 * @Note            -   Enables RXNEIE interrupt
 ********************************************************************/
uint8_t SPI_ReceiveDataIT(SPI_Handle_t *pSPIHandle, uint8_t *pRxBuffer, uint32_t Len){
    uint8_t state = pSPIHandle->RxState;

    if(state != SPI_BUSY_IN_RX){

        // 1. Save the Tx Buffer address and len info in global variables
        pSPIHandle->pRxBuffer = pRxBuffer;
        pSPIHandle->RxLen = Len;

        //2. Mark the SPI state as busy in receiving so the no other code can take
        //   over same SPI peripheral until transmission is over

        pSPIHandle->RxState = SPI_BUSY_IN_RX;

        //3. Enable the RXNEIE control bit to get interrupt whenever RXNE flag is set in
        //   Status register (SR)
        pSPIHandle->pSPIx->CR2 |= (1 << SPI_CR2_RXNEIE);
    }
    return state;
}

/*******************************************************************
 * @fn              -   SPI_IRQInterruptConfig
 *
 * @brief           -   Configures NVIC interrupt enable/disable for SPI IRQ
 *
 * @param[in]       -   IRQNumber: IRQ number to configure
 * @param[in]       -   EnorDi: ENABLE or DISABLE macro
 *
 * @return          -   None
 *
 * @Note            -   Maps IRQ to NVIC ISER/ICER registers
 ********************************************************************/
void SPI_IRQInterruptConfig(uint8_t IRQNumber, uint8_t EnorDi){
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
 * @fn              -   SPI_IRQPriorityConfig
 *
 * @brief           -   Configures NVIC interrupt priority for SPI IRQ
 *
 * @param[in]       -   IRQNumber: IRQ number to configure
 * @param[in]       -   IRQPriority: Priority level (0–15)
 *
 * @return          -   None
 *
 * @Note            -   Only upper 4 bits [7:4] are implemented
 ********************************************************************/
void SPI_IRQPriorityConfig (uint8_t IRQNumber, uint32_t IRQPriority){
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
 * @fn              -   SPI_IRQHandling
 *
 * @brief           -   Handles SPI interrupt events (TXE, RXNE, OVR)
 *
 * @param[in]       -   pHandle: Pointer to SPI handle structure
 *
 * @return          -   None
 *
 * @Note            -   Dispatches to internal handlers
 ********************************************************************/
void SPI_IRQHandling(SPI_Handle_t *pHandle){

    uint8_t temp1, temp2;
    // Check the event type : TXE
    temp1 = pHandle->pSPIx->SR & (1 << SPI_SR_TXE);
    temp2 = pHandle->pSPIx->CR2 & (1 << SPI_CR2_TXEIE);

    if(temp1 && temp2){

        spi_txe_interrupt_handle(pHandle);
    }

    // Check the event type : TXE
    temp1 = pHandle->pSPIx->SR & (1 << SPI_SR_RXNE);
    temp2 = pHandle->pSPIx->CR2 & (1 << SPI_CR2_RXNEIE);

    if(temp1 && temp2){

        spi_rxne_interrupt_handle(pHandle);
    }

    // Check the event type : OVR
    temp1 = pHandle->pSPIx->SR & (1 << SPI_SR_OVR);
    temp2 = pHandle->pSPIx->CR2 & (1 << SPI_CR2_ERRIE);

    if(temp1 && temp2){
        spi_ovr_err_interrupt_handle(pHandle);
    }
}

/*******************************************************************
 * @fn              -   SPI_PeripheralControl
 *
 * @brief           -   Enables or disables the SPI peripheral
 *
 * @param[in]       -   pSPIx: Base address of SPI peripheral
 * @param[in]       -   EnOrDi: ENABLE or DISABLE macro
 *
 * @return          -   None
 *
 * @Note            -   Controls SPE bit
 ********************************************************************/
void SPI_PeripheralControl(SPI_RegDef_t *pSPIx, uint8_t EnOrDi){

    if (EnOrDi == ENABLE){
        pSPIx->CR1 |= 1 << SPI_CR1_SPE;
    } else {
        pSPIx->CR1 &= ~(1 << SPI_CR1_SPE);
    }
}

/*******************************************************************
 * @fn              -   SPI_SSIConfig
 *
 * @brief           -   Configures SSI bit for software slave management
 *
 * @param[in]       -   pSPIx: Base address of SPI peripheral
 * @param[in]       -   EnOrDi: ENABLE or DISABLE macro
 *
 * @return          -   None
 *
 * @Note            -   Required when SSM is enabled
 ********************************************************************/
void SPI_SSIConfig(SPI_RegDef_t *pSPIx, uint8_t EnOrDi) {
    if (EnOrDi == ENABLE){
        pSPIx->CR1 |= 1 << SPI_CR1_SSI;
    } else {
        pSPIx->CR1 &= ~(1 << SPI_CR1_SSI);
    }
}

/*******************************************************************
 * @fn              -   SPI_SSOEConfig
 *
 * @brief           -   Configures SSOE bit for NSS output enable
 *
 * @param[in]       -   pSPIx: Base address of SPI peripheral
 * @param[in]       -   EnOrDi: ENABLE or DISABLE macro
 *
 * @return          -   None
 *
 * @Note            -   Required for master mode NSS output
 ********************************************************************/
void SPI_SSOEConfig(SPI_RegDef_t *pSPIx, uint8_t EnOrDi) {
    if (EnOrDi == ENABLE){
        pSPIx->CR2 |= 1 << SPI_CR2_SSOE;
    } else {
        pSPIx->CR2 &= ~(1 << SPI_CR2_SSOE);
    }
}

/*******************************************************************
 * @fn              -   SPI_CloseTransmission
 *
 * @brief           -   Closes SPI transmission and clears state
 *
 * @param[in]       -   pSPIHandle: Pointer to SPI handle structure
 *
 * @return          -   None
 *
 * @Note            -   Disables TXEIE and resets buffer pointers
 ********************************************************************/
void SPI_CloseTransmission(SPI_Handle_t *pSPIHandle){
    pSPIHandle->pSPIx->CR2 &= ~(1 << SPI_CR2_TXEIE);
    pSPIHandle->pTxBuffer = NULL;
    pSPIHandle->TxLen = 0;
    pSPIHandle->TxState = SPI_READY;
}

/*******************************************************************
 * @fn              -   SPI_CloseReception
 *
 * @brief           -   Closes SPI reception and clears state
 *
 * @param[in]       -   pSPIHandle: Pointer to SPI handle structure
 *
 * @return          -   None
 *
 * @Note            -   Disables RXNEIE and resets buffer pointers
 ********************************************************************/
void SPI_CloseReception(SPI_Handle_t *pSPIHandle){
    pSPIHandle->pSPIx->CR2 &= ~(1 << SPI_CR2_RXNEIE);
    pSPIHandle->pRxBuffer = NULL;
    pSPIHandle->RxLen = 0;
    pSPIHandle->RxState = SPI_READY;
}

/*******************************************************************
 * @fn              -   SPI_ClearOVRFlag
 *
 * @brief           -   Clears the Overrun (OVR) flag by dummy read
 *
 * @param[in]       -   pSPIHandle: Pointer to SPI handle structure
 *
 * @return          -   None
 *
 * @Note            -   Must be called after OVR error to resume SPI
 ********************************************************************/
void SPI_ClearOVRFlag(SPI_Handle_t *pSPIHandle){
    __vo uint8_t temp;
    temp = pSPIHandle->pSPIx->DR;
    temp = pSPIHandle->pSPIx->SR;
    (void)temp;
}

/*******************************************************************
 * @fn              -   spi_txe_interrupt_handle
 *
 * @brief           -   Internal handler for TXE interrupt
 *
 * @param[in]       -   pSPIHandle: Pointer to SPI handle structure
 *
 * @return          -   None
 *
 * @Note            -   Called when TXE flag is set and TXEIE is enabled
 ********************************************************************/
static void spi_txe_interrupt_handle(SPI_Handle_t *pSPIHandle){

    if(pSPIHandle->pSPIx->CR1 & (1 << SPI_CR1_DFF)){
        pSPIHandle->pSPIx->DR = *(uint16_t *)pSPIHandle->pTxBuffer; // This dereference 2 bytes of data.
        pSPIHandle->TxLen -= 2; // Decrease the data length by 2 byte
        pSPIHandle->pTxBuffer += 2; // Move the data pointer by 2 byte
    } else {
        pSPIHandle->pSPIx->DR = *pSPIHandle->pTxBuffer; // This dereference 1 bytes of data.
        pSPIHandle->TxLen--;  // Decrease the data length by 1 byte
        pSPIHandle->pTxBuffer++;// Move the data pointer
    }

    if(!pSPIHandle->TxLen){
        //TxLen zero, So close the SPI transmission and inform the application that Tx is over

        // This will prevent interrupt from setting up of TXE flag
        SPI_CloseTransmission(pSPIHandle);

        // Notify the application that Tx completed
        SPI_ApplicationEventCallback(pSPIHandle, SPI_EVENT_TX_CMPLT);
    }

}

/*******************************************************************
 * @fn              -   spi_rxne_interrupt_handle
 *
 * @brief           -   Internal handler for RXNE interrupt
 *
 * @param[in]       -   pSPIHandle: Pointer to SPI handle structure
 *
 * @return          -   None
 *
 * @Note            -   Called when RXNE flag is set and RXNEIE is enabled
 ********************************************************************/
static void spi_rxne_interrupt_handle(SPI_Handle_t *pSPIHandle){

    if(pSPIHandle->pSPIx->CR1 & (1 << SPI_CR1_DFF)){
        *(uint16_t *)pSPIHandle->pRxBuffer = pSPIHandle->pSPIx->DR; // Copy two byte of data to Rx buffer.
        pSPIHandle->RxLen -= 2; // Decrease the data length by 2 byte
        pSPIHandle->pRxBuffer +=2; // Move the data pointer by 2 byte
        //TODO : Write two byte of dummy data to generate the clock.
    } else {
        *pSPIHandle->pRxBuffer = pSPIHandle->pSPIx->DR; // Copy one byte of data to Rx buffer.
        pSPIHandle->RxLen--;  // Decrease the data length by 1 byte
        pSPIHandle->pRxBuffer++;// Move the data pointer
        //TODO : Write one byte of dummy data to generate the clock.
    }

    if(!pSPIHandle->RxLen){
        //RxLen zero, So close the SPI Rx channel and inform the application that Rx completed

        // This will prevent interrupt from setting up of RXNEIE flag
        SPI_CloseReception(pSPIHandle);

        // Notify the application that Rx completed
        SPI_ApplicationEventCallback(pSPIHandle, SPI_EVENT_RX_CMPLT);
    }
}

/*******************************************************************
 * @fn              -   spi_ovr_err_interrupt_handle
 *
 * @brief           -   Internal handler for Overrun (OVR) error interrupt
 *
 * @param[in]       -   pSPIHandle: Pointer to SPI handle structure
 *
 * @return          -   None
 *
 * @Note            -   Clears OVR flag and notifies application
 ********************************************************************/
static void spi_ovr_err_interrupt_handle(SPI_Handle_t *pSPIHandle){

    //clear OVR Flag
    __vo uint8_t temp;

    if(pSPIHandle->TxState != SPI_BUSY_IN_TX){
        temp = pSPIHandle->pSPIx->DR;
        temp = pSPIHandle->pSPIx->SR;
    }
    (void)temp;

    // Notify the application
    SPI_ApplicationEventCallback(pSPIHandle, SPI_EVENT_OVR_ERR);
}

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
__weak void SPI_ApplicationEventCallback(SPI_Handle_t *pSPIHandle, uint8_t AppEv){

    // This is a weak implementation. the application may override this funtion.
}
