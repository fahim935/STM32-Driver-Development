/*******************************************************************************
 * @file           stm32f407xx_i2c_driver.c
 * @brief          Brief description
 * 
 * @author         Fahim Akhtar
 * @date           09-Nov-2025
 * @time           8:55:10 AM
 * @version        1.0
 * @copyright      (c) 2025
 ******************************************************************************/

#include "stm32f407xx_i2c_driver.h"
#include "stm32f407xx_rcc_driver.h"

static void I2C_GenerateStartCondition(I2C_RegDef_t *pI2Cx) {
    pI2Cx->CR1 |= (1 << I2C_CR1_START);
}

static void I2C_HandleBTFInterrupt(I2C_Handle_t *pI2CHandle){

    if(pI2CHandle->TxRxState == I2C_BUSY_IN_TX){

        if(pI2CHandle->pI2Cx->SR1 & (1 << I2C_SR1_TXE)){

            if(pI2CHandle->TxLen == 0x00){
                //1. Generate the Stop condition
                if(pI2CHandle->Sr == I2C_DISABLE_SR){
                   I2C_GenerateStopCondition(pI2CHandle->pI2Cx);
                }

                //2. Clean up the member of handler structure.
                I2C_CloseSendData(pI2CHandle);

                //3. Notify to application
                I2C_ApplicationEventCallback(pI2CHandle, I2C_EV_TX_CMPLT);
            }
        }

    } else if(pI2CHandle->TxRxState == I2C_BUSY_IN_RX){
        ;
        /* If both RXNE & BTF flags are set, then it means both SR & DR registers are FULL.
         * And SCL will be stretched. Here we need not to do anything. Once Master will be
         * available it will read the data.
         * */
    }
}

static void I2C_MasterHandleTXEInterrupt(I2C_Handle_t *pI2CHandle){

    // Check for device mode
    if(pI2CHandle->pI2Cx->SR2 & (1 << I2C_SR2_MSL)){
        if(pI2CHandle->TxRxState == I2C_BUSY_IN_TX){
            if(pI2CHandle->TxLen > 0){
                pI2CHandle->pI2Cx->DR = *(pI2CHandle->pTxBuffer);
                pI2CHandle->pTxBuffer++;
                pI2CHandle->TxLen--;
            }
        }
    } else {
        // Slave mode
       // if(pI2CHandle->pI2Cx->SR2 & (1 << I2C_SR2_TRA))
            I2C_ApplicationEventCallback(pI2CHandle, I2C_EV_DATA_REQ);
    }
}

static void I2C_MasterHandleRXNEInterrupt(I2C_Handle_t *pI2CHandle){

    // Check for device mode
    if(pI2CHandle->pI2Cx->SR2 & (1 << I2C_SR2_MSL)){
        // Device is in master mode
        //RXNE EVT triggered
        if(pI2CHandle->TxRxState == I2C_BUSY_IN_RX){
            if(pI2CHandle->RxLen == 1){

                *(pI2CHandle->pRxBuffer) = pI2CHandle->pI2Cx->DR;
                pI2CHandle->RxLen--;
            }

            if(pI2CHandle->RxLen > 1){

                if(pI2CHandle->RxLen == 2){
                    //Disable the ACK
                    I2C_ManageAcking(pI2CHandle->pI2Cx, I2C_ACK_DISABLE);
                }

                //read the DR Bytes
                *(pI2CHandle->pRxBuffer) = pI2CHandle->pI2Cx->DR;
                pI2CHandle->RxLen--;
                pI2CHandle->pRxBuffer++;
            }

            if(pI2CHandle->RxLen == 0){

                //1. Generate The STOP Condition
                if(pI2CHandle->Sr == I2C_DISABLE_SR){
                   I2C_GenerateStopCondition(pI2CHandle->pI2Cx);
                }

                //2. Close the I2C Rx
                I2C_CloseReceveData(pI2CHandle);

                //3. Notify the application
                I2C_ApplicationEventCallback(pI2CHandle, I2C_EV_RX_CMPLT);
            }
        }
    } else {
        // Slave mode
       // if(!(pI2CHandle->pI2Cx->SR2 & (1 << I2C_SR2_TRA)))
            I2C_ApplicationEventCallback(pI2CHandle, I2C_EV_DATA_RCV);

    }
}

static void I2C_ExecuteAddressPhase(I2C_RegDef_t *pI2Cx, uint8_t SlaveAddr, uint8_t rw) {

    SlaveAddr = SlaveAddr << 1; // Make a room for read/write bit
    SlaveAddr = (SlaveAddr & ~1) | (rw & 1);
    pI2Cx->DR = SlaveAddr;
}

static void I2C_ClearAddrFlag(I2C_Handle_t *pI2CHandle){

    uint32_t dummyRead;

    //Check for the device mode
    if(pI2CHandle->pI2Cx->SR2 & (1 << I2C_SR2_MSL)){
        // Device is Master Mode
        if(pI2CHandle->TxRxState == I2C_BUSY_IN_RX){

            if(pI2CHandle->RxSize == 1){
                // First disable the ACK
                I2C_ManageAcking(pI2CHandle->pI2Cx, I2C_ACK_DISABLE);

                // Clear the ADDR Flah
                dummyRead = pI2CHandle->pI2Cx->SR1;
                dummyRead = pI2CHandle->pI2Cx->SR2;
                (void)dummyRead;
            }

        } else {
            // In case of TX clear the ADDR flag directly
            dummyRead = pI2CHandle->pI2Cx->SR1;
            dummyRead = pI2CHandle->pI2Cx->SR2;
            (void)dummyRead;
        }
    } else {
        // Device is Slave Mode

        // In case of Slave mode clear the ADDR flag directly
        dummyRead = pI2CHandle->pI2Cx->SR1;
        dummyRead = pI2CHandle->pI2Cx->SR2;
        (void)dummyRead;
    }
}

/*******************************************************************
 * @fn              -   I2C_PeriClockControl
 * @brief           -   Enables or disables I2C peripheral clock
 ********************************************************************/
void I2C_PeriClockControl(I2C_RegDef_t *pI2Cx, uint8_t EnorDi){

    if(EnorDi == ENABLE){
        if(pI2Cx == I2C1) {
            I2C1_PCLK_EN();
        } else if (pI2Cx == I2C2) {
            I2C2_PCLK_EN();
        } else if (pI2Cx == I2C3) {
            I2C3_PCLK_EN();
        }
    } else if(EnorDi == DISABLE)    {
        if(pI2Cx == I2C1) {
            I2C1_PCLK_DI();
        } else if (pI2Cx == I2C2) {
            I2C2_PCLK_DI();
        } else if (pI2Cx == I2C3) {
            I2C3_PCLK_DI();
        }
    } else {
        /*invalid option*/
    }
}

/*******************************************************************
 * @fn              -   I2C_PeripheralControl
 * @brief           -   Enable/Disable I2C peripheral (PE bit)
 ********************************************************************/
void I2C_PeripheralControl(I2C_RegDef_t *pI2Cx, uint8_t EnOrDi){

    if (EnOrDi == ENABLE){
        pI2Cx->CR1 |= (1U << I2C_CR1_PE);
    } else {
        pI2Cx->CR1 &= ~(1U << I2C_CR1_PE);
    }
}

/*******************************************************************
 * @fn              -   I2C_Init
 * @brief           -   Initialize I2C peripheral with user config
 ********************************************************************/
void I2C_Init(I2C_Handle_t *pI2CHandle){

    uint32_t tempreg = 0;

    //Enable the clock for the i2cx peripheral
    I2C_PeriClockControl(pI2CHandle->pI2Cx, ENABLE);

    //1. Configure ACK control (CR1)
    tempreg = pI2CHandle->I2C_Config.I2C_ACKControl << I2C_CR1_ACK;
    /* clear ACK bit then set */
    pI2CHandle->pI2Cx->CR1 |= tempreg;

    //2. Configure the speed of the serial clock(SCL)
    // Configure the FREQ field of CR2
    tempreg = 0;
    tempreg = RCC_GetPCLK1Value()/ 1000000U;
    pI2CHandle->pI2Cx->CR2 |= (tempreg & 0x3F);

    //3. Configure the device address in slave mode
    tempreg = 0;
    tempreg = pI2CHandle->I2C_Config.I2C_DeviceAddress << I2C_OAR1_ADD71;
    pI2CHandle->pI2Cx->OAR1 |= tempreg;
    pI2CHandle->pI2Cx->OAR1 |= (1 << 14); // As per UM, Bit 14 Should always be kept at 1 by software.

    //4. Configure the CCR
    uint16_t ccr_value = 0x00;
    tempreg = 0;

    if(pI2CHandle->I2C_Config.I2C_SCLSpeed <= I2C_SCL_SPEED_SM){
        /*
         * Formula for Standard Mode (<= 100 kHz)
         * CCR = FpCLK1 / ( 2 *FSCL )   Here, FSCL is the required SCL speed
         * */

        ccr_value = (RCC_GetPCLK1Value()/ (2 * pI2CHandle->I2C_Config.I2C_SCLSpeed));
        tempreg |= (ccr_value & 0xFFF);
        /* Ensure FS bit = 0 (standard) */
        pI2CHandle->pI2Cx->CCR &= ~(1U << I2C_CCR_FS);
    } else {

        /*
         * Formula for Fast Mode (> 100 kHz)
         * Set FS = 1
         * If DUTY = 0 (Tlow / Thigh = 2)    -> CCR = FpCLK1 / 3 *FSCL
         * If DUTY = 1 (Tlow / Thigh = 16/9) -> CCR = FpCLK1 / 25 *FSCL
         * */
        tempreg |= (1 << I2C_CCR_FS); // Configure the fast mode (FS = 1)
        tempreg |= (pI2CHandle->I2C_Config.I2C_FMDutyCycle << I2C_CCR_DUTY); // Duty Cycle

        if(pI2CHandle->I2C_Config.I2C_FMDutyCycle == I2C_FM_DUTY_2){
            ccr_value = (RCC_GetPCLK1Value()/ (3U * pI2CHandle->I2C_Config.I2C_SCLSpeed));
        } else {
            ccr_value = (RCC_GetPCLK1Value()/ (25U * pI2CHandle->I2C_Config.I2C_SCLSpeed));
            pI2CHandle->pI2Cx->CCR |= (1U << I2C_CCR_DUTY); /* DUTY = 1 */
        }
        tempreg |= (ccr_value & 0xFFF);
    }
    //update the CCR register
    pI2CHandle->pI2Cx->CCR = tempreg;

    //5. Configure the rise time for I2C pins

    if(pI2CHandle->I2C_Config.I2C_SCLSpeed <= I2C_SCL_SPEED_SM){
       // Standard mode
       tempreg = (RCC_GetPLLOutputClock() / 1000000U) + 1;
    } else {
       // Fast Mode
        tempreg = ((RCC_GetPLLOutputClock() * 300) / 1000000U) + 1;
    }
    pI2CHandle->pI2Cx->TRISE = (tempreg & 0x3F);

    //6. Finally enable the peripheral (PE bit) - only after all the configuration
    // TODO

}

/*******************************************************************
 * @fn              -   I2C_DeInit
 * @brief           -   Reset I2C peripheral registers
 ********************************************************************/
void I2C_DeInit(I2C_RegDef_t *pI2Cx){

    if (pI2Cx == I2C1) {
        I2C1_REG_RESET();
    } else if (pI2Cx == I2C2){
        I2C2_REG_RESET();
    } else if (pI2Cx == I2C3){
        I2C3_REG_RESET();
    }
}

/*******************************************************************
 * @fn              -   I2C_IRQInterruptConfig
 * @brief           -   Enable/disable IRQ in NVIC (no CMSIS)
 ********************************************************************/
void I2C_IRQInterruptConfig(uint8_t IRQNumber, uint8_t EnorDi){

    if(EnorDi == ENABLE)
    {
        if(IRQNumber <= 31) {
            // Program ISER0 register (ARM Processor register)
            *NVIC_ISER0 |= (1 << IRQNumber);
        } else if (IRQNumber > 31 && IRQNumber < 64){
            // Program ISER1 register
            *NVIC_ISER1 |= (1 << (IRQNumber % 32));
        } else if (IRQNumber >= 64 && IRQNumber < 96){
            // Program ISER2 register
            *NVIC_ISER2 |= (1 << (IRQNumber % 64));
        }
    } else {
        if(IRQNumber <= 31) {
            // Program ISER0 register
            *NVIC_ICER0 |= (1 << (IRQNumber));
        } else if (IRQNumber > 31 && IRQNumber < 64){
            // Program ISER1 register
            *NVIC_ICER1 |= (1 << (IRQNumber % 32));
        } else if (IRQNumber >= 64 && IRQNumber < 96){
            // Program ISER2 register
            *NVIC_ICER2 |= (1 << (IRQNumber % 64));
        }
    }
}

/*******************************************************************
 * @fn              -   I2C_IRQPriorityConfig
 * @brief           -   Set IRQ priority (no CMSIS)
 ********************************************************************/
void I2C_IRQPriorityConfig (uint8_t IRQNumber, uint32_t IRQPriority){

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
 * @fn              -   I2C_GetFlagStatus
 * @brief           -   Returns FLAG_SET or FLAG_RESET for given flag mask
 *
 * @note            -   Flags may be in SR1 or SR2; check both.
 ********************************************************************/
uint8_t I2C_GetFlagStatus(I2C_RegDef_t *pI2Cx , uint32_t FlagName)
{
    if(pI2Cx->SR1 & FlagName) {
        return FLAG_SET;
    }
    return FLAG_RESET;
}


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
                        uint8_t sr){

    //1. Generate the start condition
    I2C_GenerateStartCondition(pI2CHandle->pI2Cx);

    //2. Confirm that START Condition generated
    //Note: Until SB is cleared SCL will be stretched (PULL to LOW)
    while(!I2C_GetFlagStatus(pI2CHandle->pI2Cx, I2C_FLAG_SB));

    //3. Send the Address of Slave with r/nw
    I2C_ExecuteAddressPhase(pI2CHandle->pI2Cx, SlaveAddr, I2C_WRITE);

    //4. Confirm that address phase is completed by checking the ADDR flag in the SR1
    while(!I2C_GetFlagStatus(pI2CHandle->pI2Cx, I2C_FLAG_ADDR));

    //5. Clear the ADDR flag according to its software sequence.
    //Note: Until ADDR is cleared SCL will be stretched(Pulled to LOW )
    I2C_ClearAddrFlag(pI2CHandle);

    //6. Sent the data until the len become zero
    while(Len > 0){
        while(!I2C_GetFlagStatus(pI2CHandle->pI2Cx, I2C_FLAG_TXE)); // Wait until TxE is set
        pI2CHandle->pI2Cx->DR = *pTxBuffer;
        pTxBuffer++;
        Len--;
    }

    /*7. When Len become zero wait for TxE=1 & BTF=1 before generating the STOP condition
     *@Note: TxE=1, BTF=1, means that both SR and DR are empty and next transmission should
     *       begin when BTF=1 SCL will be stretched (Pull to LOW)
     */

    while(!I2C_GetFlagStatus(pI2CHandle->pI2Cx, I2C_FLAG_TXE));

    while(!I2C_GetFlagStatus(pI2CHandle->pI2Cx, I2C_FLAG_BTF));

    /*8. Generate the STOP condition, Master need not to wait for the completion of stop condition
     *@Note: Generate STOP, automatically clears the BTF
     * */
    if(sr == I2C_ENABLE_SR)
        I2C_GenerateStopCondition(pI2CHandle->pI2Cx);
}

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
                           uint8_t sr){

    //1. Generate the start condition
    I2C_GenerateStartCondition(pI2CHandle->pI2Cx);

    //2. Confirm that START Condition generated
    //Note: Until SB is cleared SCL will be stretched (PULL to LOW)
    while(!I2C_GetFlagStatus(pI2CHandle->pI2Cx, I2C_FLAG_SB));

    //3. Send the address of the slave with R/nW bit to R(1) (total 8 bits)
    I2C_ExecuteAddressPhase(pI2CHandle->pI2Cx, SlaveAddr, I2C_READ);

    //4. Confirm that address phase is completed by checking the ADDR flag in the SR1
    while(!I2C_GetFlagStatus(pI2CHandle->pI2Cx, I2C_FLAG_ADDR));


    //Procedure to read only 1 byte from slave

    if (Len == 1){

        //Disable Acking
        I2C_ManageAcking(pI2CHandle->pI2Cx, I2C_ACK_DISABLE);

        //clear the ADDR flag
        I2C_ClearAddrFlag(pI2CHandle);

        //wait until RXNE become 1
        while(!I2C_GetFlagStatus(pI2CHandle->pI2Cx, I2C_FLAG_RXNE));

        // Generate the STOP condition
        if(sr == I2C_ENABLE_SR)
            I2C_GenerateStopCondition(pI2CHandle->pI2Cx);

        //read data into the buffer
        *pRxBuffer = pI2CHandle->pI2Cx->DR;
        Len--;

    } else if (Len > 1){

        //clear the ADDR flag
        I2C_ClearAddrFlag(pI2CHandle);

        //read the data until Len become zero

        for(uint32_t i = Len; i >0; i--){

            //wait until RXNE become 1
            while(!I2C_GetFlagStatus(pI2CHandle->pI2Cx, I2C_FLAG_RXNE));

            // Once reading two byte left
            if(i == 2){
                //Disable Acking
                I2C_ManageAcking(pI2CHandle->pI2Cx, I2C_ACK_DISABLE);

                // Generate the STOP condition
                if(sr == I2C_ENABLE_SR)
                    I2C_GenerateStopCondition(pI2CHandle->pI2Cx);
            }

            //read data into the buffer
            *pRxBuffer = pI2CHandle->pI2Cx->DR;
            Len--;
            pRxBuffer++;
        }
    }

    //Re-enable the Acking again
    if(pI2CHandle->I2C_Config.I2C_ACKControl == I2C_ACK_ENABLE){
        I2C_ManageAcking(pI2CHandle->pI2Cx, I2C_ACK_ENABLE);
    }
}

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
                             uint8_t Sr){

    uint8_t busystate = pI2CHandle->TxRxState;

    if( (busystate != I2C_BUSY_IN_TX) && (busystate != I2C_BUSY_IN_RX)) {

        pI2CHandle->pTxBuffer = pTxBuffer;
        pI2CHandle->TxLen = Len;
        pI2CHandle->TxRxState = I2C_BUSY_IN_TX;
        pI2CHandle->DevAddr = SlaveAddr;
        pI2CHandle->Sr = Sr;

        //Generate START Condition
        I2C_GenerateStartCondition(pI2CHandle->pI2Cx);

        //Enable ITBUFEN Control Bit
        pI2CHandle->pI2Cx->CR2 |= ( 1 << I2C_CR2_ITBUFEN);

        //Implement the code to enable ITEVFEN Control Bit
        pI2CHandle->pI2Cx->CR2 |= ( 1 << I2C_CR2_ITEVTEN);

        //Implement the code to enable ITERREN Control Bit
        pI2CHandle->pI2Cx->CR2 |= ( 1 << I2C_CR2_ITERREN);

    }

    return busystate;
}

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
                                uint8_t Sr){

    uint8_t busystate = pI2CHandle->TxRxState;

    if( (busystate != I2C_BUSY_IN_TX) && (busystate != I2C_BUSY_IN_RX))
    {
        pI2CHandle->pRxBuffer = pRxBuffer;
        pI2CHandle->RxLen = Len;
        pI2CHandle->TxRxState = I2C_BUSY_IN_RX;
        pI2CHandle->RxSize = Len; //Rxsize is used in the ISR code to manage the data reception
        pI2CHandle->DevAddr = SlaveAddr;
        pI2CHandle->Sr = Sr;

        //Generate START Condition
        I2C_GenerateStartCondition(pI2CHandle->pI2Cx);

        //Implement the code to enable ITBUFEN Control Bit
        pI2CHandle->pI2Cx->CR2 |= ( 1 << I2C_CR2_ITBUFEN);


        //Implement the code to enable ITEVFEN Control Bit
        pI2CHandle->pI2Cx->CR2 |= ( 1 << I2C_CR2_ITEVTEN);


        //Implement the code to enable ITERREN Control Bit
        pI2CHandle->pI2Cx->CR2 |= ( 1 << I2C_CR2_ITERREN);

    }

    return busystate;
}

/*******************************************************************
 * @fn              -   I2C_EVT_IRQHandling
 *
 * @brief           -   Handles I2C event interrupt (SB, ADDR, BTF, TXE, RXNE)
 *
 * @param[in]       -   pI2CHandle : I2C handle structure
 *
 * @return          -   None
 *
 * @Note            -   Called inside the I2C EV IRQ handler.
 *                  -   This IRQ Handle will applicable for both Master & slave mode
 ********************************************************************/
void I2C_EVT_IRQHandling(I2C_Handle_t *pI2CHandle){

    uint32_t isEvtTriggered;
    uint32_t isBuffTriggered;
    uint32_t isFlagSet;

    isEvtTriggered = pI2CHandle->pI2Cx->CR2 & (1 << I2C_CR2_ITEVTEN);
    isBuffTriggered = pI2CHandle->pI2Cx->CR2 & (1 << I2C_CR2_ITBUFEN);



    //1. Check for SB Flag cause this interrupt trigger
    isFlagSet = pI2CHandle->pI2Cx->SR1 & (1 << I2C_SR1_SB);
    if(isEvtTriggered && isFlagSet){
        //Interrupt is trigger because of SB Event
        if(pI2CHandle->TxRxState == I2C_BUSY_IN_TX){
            I2C_ExecuteAddressPhase(pI2CHandle->pI2Cx, pI2CHandle->DevAddr, I2C_WRITE);
        } else if(pI2CHandle->TxRxState == I2C_BUSY_IN_RX){
            I2C_ExecuteAddressPhase(pI2CHandle->pI2Cx, pI2CHandle->DevAddr, I2C_READ);
        }
    }

    //2. Check for ADDR Flag cause this interrupt trigger
    isFlagSet = pI2CHandle->pI2Cx->SR1 & (1 << I2C_SR1_ADDR);
    if(isEvtTriggered && isFlagSet){
        //Interrupt is trigger because of ADDR Event
        I2C_ClearAddrFlag(pI2CHandle);

    }

    //3. Check for BTF Flag cause this interrupt trigger
    isFlagSet = pI2CHandle->pI2Cx->SR1 & (1 << I2C_SR1_BTF);
    if(isEvtTriggered && isFlagSet){
        //Interrupt is trigger because of BTF Event
        I2C_HandleBTFInterrupt(pI2CHandle);

    }

    //4. Check for STOPF Flag cause this interrupt trigger
    // This flag will not be set in the Master mode. it will only set in the slave mode
    isFlagSet = pI2CHandle->pI2Cx->SR1 & (1 << I2C_SR1_STOPF);
    if(isEvtTriggered && isFlagSet){
        //Interrupt is trigger because of STOPF Event
        /* Clear the STOPF
         *  1. Read the SR  - Already done above while reading the STOPF flag
         *  2. Write to CR1 - We can't any garbage value.
         *                  - which leads to some unexpected behavior.
         *                  - Write | with 0x0000 so that nothing will impacted.
         * */

        pI2CHandle->pI2Cx->CR1 |= 0x0000;

        // Notify to application that STOP detected.
        I2C_ApplicationEventCallback(pI2CHandle, I2C_EV_STOP);
    }

    //5. Check for TxE Flag cause this interrupt trigger
    isFlagSet = pI2CHandle->pI2Cx->SR1 & (1 << I2C_SR1_TXE);
    if(isEvtTriggered && isBuffTriggered && isFlagSet){
        //Interrupt is trigger because of TXE Event
        I2C_MasterHandleTXEInterrupt(pI2CHandle);
    }

    //6. Check for RXNE Flag cause this interrupt trigger
    isFlagSet = pI2CHandle->pI2Cx->SR1 & (1 << I2C_SR1_RXNE);
    if(isEvtTriggered && isBuffTriggered && isFlagSet){
        //Interrupt is trigger because of RXNE Event
        I2C_MasterHandleRXNEInterrupt(pI2CHandle);
    }
}

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
void I2C_ERR_IRQHandling(I2C_Handle_t *pI2CHandle)
{
    uint32_t isErrTriggered;
    uint32_t isFlagSet;

    //Know the status of  ITERREN control bit in the CR2
    isErrTriggered = (pI2CHandle->pI2Cx->CR2) & ( 1 << I2C_CR2_ITERREN);

    // Check for BUS error
    isFlagSet = (pI2CHandle->pI2Cx->SR1) & ( 1<< I2C_SR1_BERR);
    if(isFlagSet  && isErrTriggered )
    {
        //This is Bus error
        //clear the buss error flag
        pI2CHandle->pI2Cx->SR1 &= ~( 1 << I2C_SR1_BERR);

        //notify the application about the error
       I2C_ApplicationEventCallback(pI2CHandle,I2C_ERROR_BERR);
    }

    // Check for Arbitration lost error
    isFlagSet = (pI2CHandle->pI2Cx->SR1) & ( 1 << I2C_SR1_ARLO );
    if(isFlagSet  && isErrTriggered)
    {
        //This is arbitration lost error
        //clear the arbitration lost error flag
        pI2CHandle->pI2Cx->SR1 &= ~( 1 << I2C_SR1_ARLO);

        //notify the application about the error
        I2C_ApplicationEventCallback(pI2CHandle,I2C_ERROR_ARLO);

    }

    // Check for ACK failure  error
    isFlagSet = (pI2CHandle->pI2Cx->SR1) & ( 1 << I2C_SR1_AF);
    if(isFlagSet  && isErrTriggered)
    {
        //This is ACK failure error
        //clear the ACK failure error flag
        pI2CHandle->pI2Cx->SR1 &= ~( 1 << I2C_SR1_AF);

        //notify the application about the error
        I2C_ApplicationEventCallback(pI2CHandle,I2C_ERROR_AF);
    }

    // Check for Overrun/underrun error
    isFlagSet = (pI2CHandle->pI2Cx->SR1) & ( 1 << I2C_SR1_OVR);
    if(isFlagSet  && isErrTriggered)
    {
        //This is Overrun/underrun
        //clear the Overrun/underrun error flag
        pI2CHandle->pI2Cx->SR1 &= ~( 1 << I2C_SR1_OVR);

        //notify the application about the error
        I2C_ApplicationEventCallback(pI2CHandle,I2C_ERROR_OVR);
    }

    // Check for Time out error
    isFlagSet = (pI2CHandle->pI2Cx->SR1) & ( 1 << I2C_SR1_TIMEOUT);
    if(isFlagSet  && isErrTriggered)
    {
        //This is Time out error
        //clear the Time out error flag
        pI2CHandle->pI2Cx->SR1 &= ~( 1 << I2C_SR1_TIMEOUT);

        //notify the application about the error
        I2C_ApplicationEventCallback(pI2CHandle,I2C_ERROR_TIMEOUT);
    }

}

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
void I2C_SlaveSendData(I2C_RegDef_t *pI2C, uint8_t data){
    pI2C->DR = data;
}

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
uint8_t I2C_SlaveReceiveData(I2C_RegDef_t *pI2C){
    return (uint8_t) pI2C->DR;
}

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
void I2C_SlaveEnableDisableCallbackEvents(I2C_RegDef_t *pI2Cx, uint8_t EnorDi){

    if(EnorDi){
        pI2Cx->CR2 |= (1 << I2C_CR2_ITBUFEN);
        pI2Cx->CR2 |= (1 << I2C_CR2_ITEVTEN);
        pI2Cx->CR2 |= (1 << I2C_CR2_ITERREN);
    }else{
        pI2Cx->CR2 &= ~(1 << I2C_CR2_ITBUFEN);
        pI2Cx->CR2 &= ~(1 << I2C_CR2_ITEVTEN);
        pI2Cx->CR2 &= ~(1 << I2C_CR2_ITERREN);
    }
}

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
void I2C_GenerateStopCondition(I2C_RegDef_t *pI2Cx) {
    pI2Cx->CR1 |= (1 << I2C_CR1_STOP);
}

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
void I2C_CloseSendData(I2C_Handle_t *pI2CHandle){
    //Clear all the interrupt flags
    pI2CHandle->pI2Cx->CR2 &= ~(1 << I2C_CR2_ITBUFEN);
    pI2CHandle->pI2Cx->CR2 &= ~(1 << I2C_CR2_ITEVTEN);

    //Clear the state & mameber variable to i2c control block
    pI2CHandle->TxRxState = I2C_READY;
    pI2CHandle->pTxBuffer = NULL;
    pI2CHandle->TxLen     = 0x00;

}

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
void I2C_CloseReceveData(I2C_Handle_t *pI2CHandle){
    //Clear all the interrupt flags
    pI2CHandle->pI2Cx->CR2 &= ~(1 << I2C_CR2_ITBUFEN);
    pI2CHandle->pI2Cx->CR2 &= ~(1 << I2C_CR2_ITEVTEN);

    //Clear the state & mameber variable to i2c control block
    pI2CHandle->TxRxState = I2C_READY;
    pI2CHandle->pRxBuffer = NULL;
    pI2CHandle->RxLen     = 0x00;
    pI2CHandle->RxSize    = 0x00;

    if(pI2CHandle->I2C_Config.I2C_ACKControl == I2C_ACK_ENABLE){
        I2C_ManageAcking(pI2CHandle->pI2Cx, I2C_ACK_ENABLE);
    }

}

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
void I2C_ManageAcking(I2C_RegDef_t *pI2Cx, uint8_t EmOrDi) {
    if(EmOrDi == I2C_ACK_ENABLE){
        pI2Cx->CR1 |= (1U << I2C_CR1_ACK);
    } else {
        pI2Cx->CR1 &= ~(1U << I2C_CR1_ACK);
    }
}
