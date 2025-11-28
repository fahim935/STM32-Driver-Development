/*******************************************************************************
 * @file           stm32f407xx_usart_driver.c
 * @brief          Brief description
 *
 * @author         Fahim Akhtar
 * @date           24-Nov-2025
 * @time           9:38:16 PM
 * @version        1.0
 * @copyright      (c) 2025
 ******************************************************************************/

#include "stm32f407xx_usart_driver.h"
#include "stm32f407xx_rcc_driver.h"


static void usart_handle_tc(USART_Handle_t *pUSARTHandle);
static void usart_handle_txe(USART_Handle_t *pUSARTHandle);
static void usart_handle_rxne(USART_Handle_t *pUSARTHandle);
static void usart_handle_cts(USART_Handle_t *pUSARTHandle);
static void usart_handle_idle(USART_Handle_t *pUSARTHandle);
static void usart_handle_ore(USART_Handle_t *pUSARTHandle);
static void usart_handle_error_flags(USART_Handle_t *pUSARTHandle, uint32_t sr);

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
void USART_PeriClockControl(USART_RegDef_t *pUSARTx, uint8_t EnorDi){
    if(EnorDi == ENABLE){
        if(pUSARTx == USART1) {
            USART1_PCLK_EN();
        } else if (pUSARTx == USART2) {
            USART2_PCLK_EN();
        } else if (pUSARTx == USART3) {
            USART3_PCLK_EN();
        } else if (pUSARTx == UART4) {
            UART4_PCLK_EN();
        } else if (pUSARTx == UART5) {
            UART5_PCLK_EN();
        } else if (pUSARTx == USART6) {
            USART6_PCLK_EN();
        }
    } else if(EnorDi == DISABLE)    {
        if(pUSARTx == USART1) {
            USART1_PCLK_DI();
        } else if (pUSARTx == USART2) {
            USART2_PCLK_DI();
        } else if (pUSARTx == USART3) {
            USART3_PCLK_DI();
        } else if (pUSARTx == UART4) {
            UART4_PCLK_DI();
        } else if (pUSARTx == UART5) {
            UART5_PCLK_DI();
        } else if (pUSARTx == USART6) {
            USART6_PCLK_DI();
        }
    } else {
        /*invalid option*/
    }
}

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
void USART_Init(USART_Handle_t *pUSARTHandle){

    uint32_t tempreg=0;

    //Enable the Clock for given USART peripheral
    USART_PeriClockControl(pUSARTHandle->pUSARTx, ENABLE);

    //Enable USART Tx and Rx engines according to the USART_Mode configuration item
    if ( pUSARTHandle->USART_Config.USART_Mode == USART_MODE_ONLY_RX){
        //Enable the Receiver bit field
        tempreg|= (1 << USART_CR1_RE);
    }else if (pUSARTHandle->USART_Config.USART_Mode == USART_MODE_ONLY_TX){
        //Enable the Transmitter bit field
        tempreg |= ( 1 << USART_CR1_TE );

    }else if (pUSARTHandle->USART_Config.USART_Mode == USART_MODE_TXRX){
        //Enable the both Transmitter and Receiver bit fields
        tempreg |= ( ( 1 << USART_CR1_RE) | ( 1 << USART_CR1_TE) );
    }

    //Configure the Word length configuration item
    tempreg |= pUSARTHandle->USART_Config.USART_WordLength << USART_CR1_M ;


    //Configuration of parity control bit fields
    if ( pUSARTHandle->USART_Config.USART_ParityControl == USART_PARITY_EN_EVEN){
        //Enable the parity control
        tempreg |= ( 1 << USART_CR1_PCE);

        //Implement the code to enable EVEN parity
        /*Not required because by default EVEN parity will be
        /selected once you enable the parity control*/
    }else if (pUSARTHandle->USART_Config.USART_ParityControl == USART_PARITY_EN_ODD ){
        //Enable the parity control
        tempreg |= ( 1 << USART_CR1_PCE);

        //Enable ODD parity
        tempreg |= ( 1 << USART_CR1_PS);
    }

   //Update the CR1 register
    pUSARTHandle->pUSARTx->CR1 = tempreg;


    tempreg=0;

    //Configure the number of stop bits inserted during USART frame transmission
    tempreg |= pUSARTHandle->USART_Config.USART_NoOfStopBits << USART_CR2_STOP;

    //Update the CR2 register
    pUSARTHandle->pUSARTx->CR2 = tempreg;

    tempreg=0;

    //Configuration of USART hardware flow control
    if ( pUSARTHandle->USART_Config.USART_HWFlowControl == USART_HW_FLOW_CTRL_CTS){
        //Enable CTS flow control
        tempreg |= ( 1 << USART_CR3_CTSE);
    }else if (pUSARTHandle->USART_Config.USART_HWFlowControl == USART_HW_FLOW_CTRL_RTS){
        //Enable RTS flow control
        tempreg |= USART_CR3_RTSE;
    }else if (pUSARTHandle->USART_Config.USART_HWFlowControl == USART_HW_FLOW_CTRL_CTS_RTS){
        //Enable both CTS and RTS Flow control
        tempreg |= ( ( 1 << USART_CR3_CTSE) | ( 1 << USART_CR3_RTSE) );
    }

    pUSARTHandle->pUSARTx->CR3 = tempreg;

    //Configure the baud rate
    USART_SetBaudRate(pUSARTHandle->pUSARTx, pUSARTHandle->USART_Config.USART_Baud);
}

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
void USART_DeInit(USART_RegDef_t *pUSARTx){
    if (pUSARTx == USART1){
        USART1_REG_RESET();
    }else if (pUSARTx == USART2){
        USART2_REG_RESET();
    }else if (pUSARTx == USART3){
        USART3_REG_RESET();
    }else if (pUSARTx == UART4){
        UART4_REG_RESET();
    }else if (pUSARTx == UART5){
        UART5_REG_RESET();
    }else if (pUSARTx == USART6){
        USART6_REG_RESET();
    }
}

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
void USART_SendData(USART_Handle_t *pUSARTHandle, uint8_t *pTxBuffer, uint32_t Len){

    uint16_t *pdata;

    for(uint32_t i = 0 ; i < Len; i++)
    {
        //Wait until TXE flag is set in the SR
        while(! USART_GetFlagStatus(pUSARTHandle->pUSARTx,USART_FLAG_TXE));

         //Check the USART_WordLength item for 9BIT or 8BIT in a frame
        if(pUSARTHandle->USART_Config.USART_WordLength == USART_WORDLEN_9BITS)
        {
            //if 9BIT, load the DR with 2bytes masking the bits other than first 9 bits
            pdata = (uint16_t*) pTxBuffer;
            pUSARTHandle->pUSARTx->DR = (*pdata & (uint16_t)0x01FF);

            //check for USART_ParityControl
            if(pUSARTHandle->USART_Config.USART_ParityControl == USART_PARITY_DISABLE){
                //No parity is used in this transfer. so, 9bits of user data will be sent
                //Increment pTxBuffer twice
                pTxBuffer++;
                pTxBuffer++;
            }else{
                //Parity bit is used in this transfer. so, 8bits of user data will be sent
                //The 9th bit will be replaced by parity bit by the hardware
                pTxBuffer++;
            }
        } else {
            //This is 8bit data transfer
            pUSARTHandle->pUSARTx->DR = (*pTxBuffer  & (uint8_t)0xFF);

            //Increment the buffer address
            pTxBuffer++;
        }
    }

    //Wait till TC flag is set in the SR
    while( ! USART_GetFlagStatus(pUSARTHandle->pUSARTx,USART_FLAG_TC));
}

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
void USART_ReceiveData(USART_Handle_t *pUSARTHandle, uint8_t *pRxBuffer, uint32_t Len){

     for(uint32_t i = 0 ; i < Len; i++)
     {
         //Implement the code to wait until RXNE flag is set in the SR
         while(! USART_GetFlagStatus(pUSARTHandle->pUSARTx,USART_FLAG_RXNE));

         //Check the USART_WordLength to decide whether we are going to receive 9bit of data in a frame or 8 bit
         if(pUSARTHandle->USART_Config.USART_WordLength == USART_WORDLEN_9BITS){
             //We are going to receive 9bit data in a frame

             //check are we using USART_ParityControl control or not
             if(pUSARTHandle->USART_Config.USART_ParityControl == USART_PARITY_DISABLE){
                 //No parity is used. so, all 9bits will be of user data

                 //read only first 9 bits. so, mask the DR with 0x01FF
                 *((uint16_t*) pRxBuffer) = (pUSARTHandle->pUSARTx->DR  & (uint16_t)0x01FF);

                 //Increment the pRxBuffer two times
                 pRxBuffer++;
                 pRxBuffer++;
             }else{
                  //Parity is used, so, 8bits will be of user data and 1 bit is parity
                  *pRxBuffer = (pUSARTHandle->pUSARTx->DR  & (uint8_t)0xFF);

                  //Increment the pRxBuffer
                  pRxBuffer++;
             }
         }else{
             //We are going to receive 8bit data in a frame

             //check are we using USART_ParityControl control or not
             if(pUSARTHandle->USART_Config.USART_ParityControl == USART_PARITY_DISABLE){
                 //No parity is used , so all 8bits will be of user data

                 //read 8 bits from DR
                 *pRxBuffer = (uint8_t) (pUSARTHandle->pUSARTx->DR  & (uint8_t)0xFF);
             }else{
                 //Parity is used, so , 7 bits will be of user data and 1 bit is parity

                 //read only 7 bits , hence mask the DR with 0X7F
                 *pRxBuffer = (uint8_t) (pUSARTHandle->pUSARTx->DR  & (uint8_t)0X7F);
             }

             //increment the pRxBuffer
             pRxBuffer++;
         }
     }
}

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
uint8_t USART_SendDataIT(USART_Handle_t *pUSARTHandle, uint8_t *pTxBuffer, uint32_t Len){
    uint8_t txstate = pUSARTHandle->TxBusyState;

    if(txstate != USART_BUSY_IN_TX)
    {
        pUSARTHandle->TxLen = Len;
        pUSARTHandle->pTxBuffer = pTxBuffer;
        pUSARTHandle->TxBusyState = USART_BUSY_IN_TX;

        //Enable interrupt for TXE
        pUSARTHandle->pUSARTx->CR1 |= (1 << USART_CR1_TXEIE);

        //Enable interrupt for TC
        pUSARTHandle->pUSARTx->CR1 |= (1 << USART_CR1_TCIE);
    }
    return txstate;
}

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
uint8_t USART_ReceiveDataIT(USART_Handle_t *pUSARTHandle, uint8_t *pRxBuffer, uint32_t Len){
    uint8_t rxstate = pUSARTHandle->RxBusyState;

    if(rxstate != USART_BUSY_IN_RX){
        pUSARTHandle->RxLen = Len;
        pUSARTHandle->pRxBuffer = pRxBuffer;
        pUSARTHandle->RxBusyState = USART_BUSY_IN_RX;

        //Enable interrupt for RXNE
        pUSARTHandle->pUSARTx->CR1 |= (1 << USART_CR1_RXNEIE);
    }
    return rxstate;
}

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
void USART_IRQInterruptConfig(uint8_t IRQNumber, uint8_t EnorDi){
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
void USART_IRQPriorityConfig(uint8_t IRQNumber, uint32_t IRQPriority){
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
 * @fn              -   USART_IRQHandling
 *
 * @brief           -   Central IRQ dispatcher, calls modular handlers
 *
 * @param[in]       -   pHandle : USART handle structure
 *
 * @return          -   None
 *
 * @note            -   Handles TXE, TC, RXNE, CTS, ORE, FE, PE flags
 ********************************************************************/
void USART_IRQHandling(USART_Handle_t *pUSARTHandle)
{
    uint32_t sr  = pUSARTHandle->pUSARTx->SR;
    uint32_t cr1 = pUSARTHandle->pUSARTx->CR1;
    uint32_t cr3 = pUSARTHandle->pUSARTx->CR3;

    /* TC: Transmission complete */
    if ( (sr & (1 << USART_SR_TC)) && (cr1 & (1 << USART_CR1_TCIE)) )
    {
        usart_handle_tc(pUSARTHandle);
    }

    /* TXE: Transmit data register empty */
    if ( (sr & (1 << USART_SR_TXE)) && (cr1 & (1 << USART_CR1_TXEIE)) )
    {
        usart_handle_txe(pUSARTHandle);
    }

    /* RXNE: Received data ready */
    if ( (sr & (1 << USART_SR_RXNE)) && (cr1 & (1 << USART_CR1_RXNEIE)) )
    {
        usart_handle_rxne(pUSARTHandle);
    }

    /* CTS: Clear To Send (not for UART4/5) */
    if ( (sr & (1 << USART_SR_CTS)) && (cr3 & (1 << USART_CR3_CTSE)) )
    {
        usart_handle_cts(pUSARTHandle);
    }

    /* IDLE: Idle line detection */
    if ( (sr & (1 << USART_SR_IDLE)) && (cr1 & (1 << USART_CR1_IDLEIE)) )
    {
        usart_handle_idle(pUSARTHandle);
    }

    /* ORE: Overrun error (with RXNE interrupt enabled) */
    if ( (sr & USART_SR_ORE) && (cr1 & USART_CR1_RXNEIE) )
    {
        usart_handle_ore(pUSARTHandle);
    }

    /* FE / NF / ORE when EIE is set (multi-buffer / error interrupt enable) */
    if (cr3 & (1 << USART_CR3_EIE))
    {
        usart_handle_error_flags(pUSARTHandle, sr);
    }
}

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
void USART_PeripheralControl(USART_RegDef_t *pUSARTx, uint8_t EnOrDi){

    if (EnOrDi == ENABLE){
        pUSARTx->CR1 |= (1 << USART_CR1_UE);      // Enable USART
    } else {
        pUSARTx->CR1 &= ~(1 << USART_CR1_UE);
    }
}

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
uint8_t USART_GetFlagStatus(USART_RegDef_t *pUSARTx, uint32_t FlagName){
    if(pUSARTx->SR & FlagName){
        return FLAG_SET;
    }
    return FLAG_RESET;
}

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
void USART_ClearFlag(USART_RegDef_t *pUSARTx, uint16_t StatusFlagName){
    //TODO
}

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
void USART_SetBaudRate(USART_RegDef_t *pUSARTx, uint32_t BaudRate)
{

    //Variable to hold the APB clock
    uint32_t PCLKx;

    uint32_t usartdiv;

    //variables to hold Mantissa and Fraction values
    uint32_t M_part,F_part;

    uint32_t tempreg=0;

    //Get the value of APB bus clock in to the variable PCLKx
    if(pUSARTx == USART1 || pUSARTx == USART6){
       //USART1 and USART6 are hanging on APB2 bus
       PCLKx = RCC_GetPCLK2Value();
    }else{
       PCLKx = RCC_GetPCLK1Value();
    }

    //Check for OVER8 configuration bit
    if(pUSARTx->CR1 & (1 << USART_CR1_OVER8)){
       //OVER8 = 1 , over sampling by 8
       usartdiv = ((25 * PCLKx) / (2 *BaudRate));
    }else{
       //over sampling by 16
       usartdiv = ((25 * PCLKx) / (4 *BaudRate));
    }

    //Calculate the Mantissa part
    M_part = usartdiv/100;

    //Place the Mantissa part in appropriate bit position . refer USART_BRR
    tempreg |= M_part << 4;

    //Extract the fraction part
    F_part = (usartdiv - (M_part * 100));

    //Calculate the final fractional
    if(pUSARTx->CR1 & ( 1 << USART_CR1_OVER8))
    {
      //OVER8 = 1 , over sampling by 8
      F_part = ((( F_part * 8)+ 50) / 100)& ((uint8_t)0x07);

    }else{
       //over sampling by 16
       F_part = ((( F_part * 16)+ 50) / 100) & ((uint8_t)0x0F);

    }

    //Place the fractional part in appropriate bit position . refer USART_BRR
    tempreg |= F_part;

    //copy the value of tempreg in to BRR register
    pUSARTx->BRR = tempreg;
}

/*******************************************************************
 * @fn              -   usart_handle_tc
 *
 * @brief           -   Handles the Transmission Complete (TC) interrupt.
 *
 * @param[in]       -   pUSARTHandle : Pointer to USART handle.
 *
 * @return          -   None
 *
 * @note            -   TC is set when the last frame has fully shifted
 *                      out on TX line. This helper:
 *                      1) Clears TC flag
 *                      2) Stops the TX operation if buffer is finished
 *                      3) Resets TX state and buffer pointers
 *                      4) Notifies the application via callback
 ********************************************************************/
static void usart_handle_tc(USART_Handle_t *pUSARTHandle)
{
    if ((pUSARTHandle->TxBusyState == USART_BUSY_IN_TX) &&
       (pUSARTHandle->TxLen == 0))
       {
            /* Clear TC flag */
            pUSARTHandle->pUSARTx->SR &= ~(1 << USART_SR_TC);

            /* Disable TC interrupt */
            pUSARTHandle->pUSARTx->CR1 &= ~(1 << USART_CR1_TCIE);

            /* Reset state */
            pUSARTHandle->TxBusyState = USART_READY;
            pUSARTHandle->pTxBuffer   = NULL;
            pUSARTHandle->TxLen       = 0;

            /* Callback: TX complete */
            USART_ApplicationEventCallback(pUSARTHandle, USART_EVENT_TX_CMPLT);
       }
}

/*******************************************************************
 * @fn              -   usart_handle_txe
 *
 * @brief           -   Handles the Transmit Data Register Empty (TXE)
 *                      interrupt.
 *
 * @param[in]       -   pUSARTHandle : Pointer to USART handle.
 *
 * @return          -   None
 *
 * @note            -   TXE = 1 means the DR register is empty and ready
 *                      for the next byte. This helper:
 *                      1) Loads next byte/word into DR
 *                      2) Advances TX buffer pointer and length
 *                      3) Disables TXE interrupt when transmission ends
 ********************************************************************/
static void usart_handle_txe(USART_Handle_t *pUSARTHandle)
{
    uint16_t *pdata;

    if (pUSARTHandle->TxBusyState != USART_BUSY_IN_TX)
        return;

    if (pUSARTHandle->TxLen > 0)
    {
        if (pUSARTHandle->USART_Config.USART_WordLength == USART_WORDLEN_9BITS)
        {
            /* 9-bit data */
            pdata = (uint16_t *)pUSARTHandle->pTxBuffer;
            pUSARTHandle->pUSARTx->DR = (*pdata & (uint16_t)0x01FF);

            if (pUSARTHandle->USART_Config.USART_ParityControl == USART_PARITY_DISABLE)
            {
                /* 9 bits of user data → increment buffer by 2 bytes */
                pUSARTHandle->pTxBuffer += 2;
                pUSARTHandle->TxLen     -= 2;
            }
            else
            {
                /* 8 data bits + 1 parity bit → increment buffer by 1 byte */
                pUSARTHandle->pTxBuffer += 1;
                pUSARTHandle->TxLen     -= 1;
            }
        }
        else
        {
            /* 8-bit data */
            pUSARTHandle->pUSARTx->DR = (*pUSARTHandle->pTxBuffer & (uint8_t)0xFF);
            pUSARTHandle->pTxBuffer++;
            pUSARTHandle->TxLen--;
        }
    }

    if (pUSARTHandle->TxLen == 0)
    {
        /* No more data: disable TXE interrupt */
        pUSARTHandle->pUSARTx->CR1 &= ~(1 << USART_CR1_TXEIE);
    }
}

/*******************************************************************
 * @fn              -   usart_handle_rxne
 *
 * @brief           -   Handles the Receive Data Register Not Empty
 *                      (RXNE) interrupt.
 *
 * @param[in]       -   pUSARTHandle : Pointer to USART handle.
 *
 * @return          -   None
 *
 * @note            -   RXNE = 1 means valid data has been received and
 *                      copied to DR. This helper:
 *                      1) Reads received byte/word from DR
 *                      2) Stores it into user buffer
 *                      3) Updates buffer pointer and length
 *                      4) Disables RXNE interrupt when reception ends
 ********************************************************************/
static void usart_handle_rxne(USART_Handle_t *pUSARTHandle)
{
    if (pUSARTHandle->RxBusyState != USART_BUSY_IN_RX)
        return;

    if (pUSARTHandle->RxLen == 0)
        return;

    if (pUSARTHandle->USART_Config.USART_WordLength == USART_WORDLEN_9BITS)
    {
        if (pUSARTHandle->USART_Config.USART_ParityControl == USART_PARITY_DISABLE)
        {
            /* 9-bit user data */
            *((uint16_t *)pUSARTHandle->pRxBuffer) =
                (pUSARTHandle->pUSARTx->DR & (uint16_t)0x01FF);

            pUSARTHandle->pRxBuffer += 2;
            pUSARTHandle->RxLen     -= 2;
        }
        else
        {
            /* 8-bit user data + parity */
            *pUSARTHandle->pRxBuffer =
                (uint8_t)(pUSARTHandle->pUSARTx->DR & (uint8_t)0xFF);

            pUSARTHandle->pRxBuffer++;
            pUSARTHandle->RxLen--;
        }
    }
    else
    {
        /* 8-bit frame */
        if (pUSARTHandle->USART_Config.USART_ParityControl == USART_PARITY_DISABLE)
        {
            *pUSARTHandle->pRxBuffer =
                (uint8_t)(pUSARTHandle->pUSARTx->DR & (uint8_t)0xFF);
        }
        else
        {
            *pUSARTHandle->pRxBuffer =
                (uint8_t)(pUSARTHandle->pUSARTx->DR & (uint8_t)0x7F);
        }

        pUSARTHandle->pRxBuffer++;
        pUSARTHandle->RxLen--;
    }

    if (pUSARTHandle->RxLen == 0)
    {
        /* Disable RXNE interrupt */
        pUSARTHandle->pUSARTx->CR1 &= ~(1 << USART_CR1_RXNEIE);
        pUSARTHandle->RxBusyState = USART_READY;

        /* Callback: RX complete */
        USART_ApplicationEventCallback(pUSARTHandle, USART_EVENT_RX_CMPLT);
    }
}

/*******************************************************************
 * @fn              -   usart_handle_cts
 *
 * @brief           -   Handles the Clear-To-Send (CTS) interrupt event.
 *
 * @param[in]       -   pUSARTHandle : Pointer to USART handle.
 *
 * @return          -   None
 *
 * @note            -   CTS is used only when hardware flow control is
 *                      enabled. This helper clears the CTS flag and
 *                      notifies the application.
 ********************************************************************/
static void usart_handle_cts(USART_Handle_t *pUSARTHandle)
{
    /* Clear CTS flag */
    pUSARTHandle->pUSARTx->SR &= ~(1 << USART_SR_CTS);

    /* Callback: CTS event */
    USART_ApplicationEventCallback(pUSARTHandle, USART_EVENT_CTS);
}

/*******************************************************************
 * @fn              -   usart_handle_idle
 *
 * @brief           -   Handles the Idle Line detection interrupt.
 *
 * @param[in]       -   pUSARTHandle : Pointer to USART handle.
 *
 * @return          -   None
 *
 * @note            -   IDLE flag is set when no data has been received
 *                      for 1 frame time after last stop bit. Flag is
 *                      cleared by read of SR then DR. Application is
 *                      notified after the flag is cleared.
 ********************************************************************/
static void usart_handle_idle(USART_Handle_t *pUSARTHandle)
{
    /* Clear IDLE flag: read SR then DR (here we directly clear per your style) */
    (void)pUSARTHandle->pUSARTx->SR;
    (void)pUSARTHandle->pUSARTx->DR;

    USART_ApplicationEventCallback(pUSARTHandle, USART_EVENT_IDLE);
}

/*******************************************************************
 * @fn              -   usart_handle_ore
 *
 * @brief           -   Handles the Overrun Error (ORE) condition.
 *
 * @param[in]       -   pUSARTHandle : Pointer to USART handle.
 *
 * @return          -   None
 *
 * @note            -   ORE occurs when new data arrives before previous
 *                      data was read from DR. This helper simply notifies
 *                      application. Clearing is done via SR then DR read.
 ********************************************************************/
static void usart_handle_ore(USART_Handle_t *pUSARTHandle)
{
    /* App can decide how/when to clear ORE (read SR + DR) */
    USART_ApplicationEventCallback(pUSARTHandle, USART_ERR_ORE);
}

/*******************************************************************
 * @fn              -   usart_handle_error_flags
 *
 * @brief           -   Handles error status flags: FE, NE, ORE when
 *                      Error Interrupt Enable (EIE) is set.
 *
 * @param[in]       -   pUSARTHandle : Pointer to USART handle.
 * @param[in]       -   sr           : Cached USART->SR value.
 *
 * @return          -   None
 *
 * @note            -   These flags indicate reception problems:
 *                      FE  = Framing Error (Start/Stop mismatch)
 *                      NE  = Noise Error (sampling noise)
 *                      ORE = Overrun Error (data lost)
 *                      Clearing is done by read sequence or callback.
 ********************************************************************/
static void usart_handle_error_flags(USART_Handle_t *pUSARTHandle, uint32_t sr)
{
    if (sr & (1 << USART_SR_FE))
    {
        USART_ApplicationEventCallback(pUSARTHandle, USART_ERR_FE);
    }

    if (sr & (1 << USART_SR_NF))
    {
        USART_ApplicationEventCallback(pUSARTHandle, USART_ERR_NE);
    }

    if (sr & (1 << USART_SR_ORE))
    {
        USART_ApplicationEventCallback(pUSARTHandle, USART_ERR_ORE);
    }
}

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
__weak void USART_ApplicationEventCallback(USART_Handle_t *pUSARTHandle, uint8_t AppEv){

    // This is a weak implementation. the application may override this funtion.
}
