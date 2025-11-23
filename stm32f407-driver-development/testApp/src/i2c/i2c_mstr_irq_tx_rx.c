/*******************************************************************************
 * @file           i2c_mstr_irq_tx_rx.c
 * @brief          Brief description
 * 
 * @author         Fahim Akhtar
 * @date           22-Nov-2025
 * @time           5:05:36 PM
 * @version        1.0
 * @copyright      (c) 2025
 ******************************************************************************/

#include "i2c_mstr_irq_tx_rx.h"
#if (FEATURE_SELECTED == STM32_I2C_MSTR_IRQ_TX_RX)
#include "common.h"

/*
 * Description:
 *  12C Master{STM32) and I2C Slave(Arduino) communication.
 *  When button on the master is pressed, master should read and display data from
 *  Arduino Slave connected. First master has to get the length of the data from the
 *  slave to read subsequent data from the slave.
 *      1. Use I2C SCL=100KHz[Standard mode
 *      2. Use internal pull resistors for SDA and SCL lines
 *  0x51 : Command to read the data length
 *  0x52 : Command to read the complete data from the slave
 * */

I2C_Handle_t I2CHandle;

//Flag variable
uint8_t rxComplt = RESET;
uint8_t txComplt = RESET;


/* While choosing address for your device must check the reference manual.
 * there are some reserved addresses mentioned those you should not choose */
#define MY_ADDR 0x61

#define SLAVE_ADDR 0x68

#define GET_DATA_LEN_CMD    0x51
#define GET_FULL_DATA_CMD   0x52


void I2C1_GPIOInits(void){
    GPIO_Handle_t I2CPins;

    I2CPins.pGPIOx = GPIOB;
    I2CPins.GPIO_PinConfig.GPIO_PinMode = GPIO_MODE_ALTFN;
    I2CPins.GPIO_PinConfig.GPIO_PinOPType = GPIO_OP_TYPE_OD;
    I2CPins.GPIO_PinConfig.GPIO_PinPuPdControl = GPIO_PIN_PU;
    I2CPins.GPIO_PinConfig.GPIO_PinAltFunMode = 4;
    I2CPins.GPIO_PinConfig.GPIO_PinSpeed = GPIO_SPEED_FAST;

    //SCL
    I2CPins.GPIO_PinConfig.GPIO_PinNumber = GPIO_PIN_NO_6;
    GPIO_Init(&I2CPins);

    //SDA
    I2CPins.GPIO_PinConfig.GPIO_PinNumber = GPIO_PIN_NO_7;
    GPIO_Init(&I2CPins);
}


void I2C1_Inits(void){

    I2CHandle.pI2Cx = I2C1;
    I2CHandle.I2C_Config.I2C_ACKControl = I2C_ACK_ENABLE;
    I2CHandle.I2C_Config.I2C_DeviceAddress = MY_ADDR;
    I2CHandle.I2C_Config.I2C_FMDutyCycle = I2C_FM_DUTY_2;
    I2CHandle.I2C_Config.I2C_SCLSpeed = I2C_SCL_SPEED_SM;

    I2C_Init(&I2CHandle);
}

/* On board user button configuration */
static void GPIO_OnBoardButtonInit(void){

    GPIO_Handle_t GpioBtn;

    GpioBtn.pGPIOx = GPIOA;
    GpioBtn.GPIO_PinConfig.GPIO_PinNumber = GPIO_PIN_NO_0;
    GpioBtn.GPIO_PinConfig.GPIO_PinMode = GPIO_MODE_IN;
    GpioBtn.GPIO_PinConfig.GPIO_PinSpeed = GPIO_SPEED_FAST;
    GpioBtn.GPIO_PinConfig.GPIO_PinPuPdControl = GPIO_NO_PUPD;
    GPIO_Init(&GpioBtn);

}


void i2c_master_interrupt_base_rx_testing(void){

    uint8_t getDataLenCmd = GET_DATA_LEN_CMD;
    uint8_t getFullData = GET_FULL_DATA_CMD;
    uint8_t dataLen = 0x00;
    uint8_t data[50];

    printf("Application is running\n");

    // I2C Pin Init
    I2C1_GPIOInits();

    // I2C peripheral configuration
    I2C1_Inits();

    //enable the I2C peripheral
    I2C_PeripheralControl(I2C1, ENABLE);

    //I2C IRQ configurations
    I2C_IRQInterruptConfig(IRQ_NO_I2C1_EV, ENABLE);
    I2C_IRQInterruptConfig(IRQ_NO_I2C1_ER, ENABLE);

    GPIO_OnBoardButtonInit();



    while(1){
        // Wait for button press
        while(!GPIO_ReadFromInputPin(GPIOA, GPIO_PIN_NO_0));

        // To avoid the de-bouncing
        delay();

        I2C_ManageAcking(I2CHandle.pI2Cx, I2C_ACK_ENABLE);

        // Send GET_DATA_LEN_CMD to get the length of the data
        while(I2C_MasterSendDataIT(&I2CHandle, &getDataLenCmd, sizeof(getDataLenCmd), SLAVE_ADDR, I2C_ENABLE_SR ) != I2C_READY);

        while(txComplt != SET){;}

        // Read the data length field
        while(I2C_MasterReceiveDataIT(&I2CHandle, &dataLen, sizeof(dataLen), SLAVE_ADDR, I2C_ENABLE_SR ) != I2C_READY);

        while(rxComplt != SET){;}
        printf("Data Length = %d\n",dataLen);

        // Send GET_FULL_DATA_CMD to get the full data
        while(I2C_MasterSendDataIT(&I2CHandle, &getFullData, sizeof(getFullData), SLAVE_ADDR, I2C_ENABLE_SR ) != I2C_READY);

        while(txComplt != SET){;}
        // Read the Full data
        while(I2C_MasterReceiveDataIT(&I2CHandle, data, dataLen , SLAVE_ADDR, I2C_ENABLE_SR ) != I2C_READY);

        rxComplt = RESET;

        while(rxComplt != SET){;}

        data[dataLen+1] = '\0';

        printf("Data : %s",data);

        for(uint8_t i = 0; i < dataLen; i++){
            printf("%X",data[i]);
        }
        printf("\n");
    }
}

void I2C_ApplicationEventCallback(I2C_Handle_t *pI2CHandle,uint8_t AppEv)
{
     if(AppEv == I2C_EV_TX_CMPLT)
     {
         txComplt = SET;
         printf("Tx is completed\n");
     }else if (AppEv == I2C_EV_RX_CMPLT)
     {
         printf("Rx is completed\n");
         rxComplt = SET;
     }else if (AppEv == I2C_ERROR_AF)
     {
         printf("Error : Ack failure\n");
         //in master ACK failure happens when slave fails to send ACK for the byte
         I2C_CloseSendData(pI2CHandle);

         //generate the stop condition to release the bus
         I2C_GenerateStopCondition(pI2CHandle->pI2Cx);
         //Hang in infinite loop
         while(1);
     }else if (AppEv == I2C_ERROR_BERR)
     {
         printf("I2C_ERROR_BERR\n");
     }else if (AppEv == I2C_ERROR_ARLO)
     {
         printf("I2C_ERROR_ARLO\n");
     }else if (AppEv == I2C_ERROR_OVR)
     {
         printf("I2C_ERROR_OVR\n");
     }else if (AppEv == I2C_ERROR_TIMEOUT)
     {
         printf("I2C_ERROR_TIMEOUT\n");
     }
}

void I2C1_ER_IRQHandler(void){

    I2C_ERR_IRQHandling(&I2CHandle);

}

void I2C1_EV_IRQHandler(void){

    I2C_EVT_IRQHandling(&I2CHandle);
}

#endif
