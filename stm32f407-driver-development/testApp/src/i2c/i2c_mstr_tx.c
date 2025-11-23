/*******************************************************************************
 * @file           i2c_mstr_tx..c
 * @brief          Brief description
 * 
 * @author         Fahim Akhtar
 * @date           11-Nov-2025
 * @time           7:53:13 AM
 * @version        1.0
 * @copyright      (c) 2025
 ******************************************************************************/

#include <i2c_mstr_tx.h>
#if (FEATURE_SELECTED == STM32_I2C_MSTR_TX)
#include "common.h"


/*
 * Description :
 *  I2C Master(STM32 Discovery) and 12C Slave(Arduino board ) communication
 *  When button on the STM32 board (master) is pressed, master should send
 *  data to the Arduino board (slave). The data received by the Arduino board
 *  will be displayed on the serial monitor terminal of the Arduino IDE
 *      1. Use I2C SCL = 100kHz(Standard mode)
 *      2. Use external pull up resistors(3.3kOhm) for SDA and SCL line
 *  Note : if don't have external pull up resistors, we can try with
 *  activating the STM32 I2C pin's internal pull up resistors as well.
 *
 * */

/*
 * PB6 : SCL
 * PB7 : SDA
 *
 * */

I2C_Handle_t I2CHandle;


/* While choosing address for your device must check the reference manual.
 * there are some reserved addresses mentioned those you should not choose */
#define MY_ADDR 0x61

#define SLAVE_ADDR 0x68

uint8_t tx_data[] = "Hello Slave, Did you got my msg\n";

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


void i2c_master_tx_testing(void){

    // I2C Pin Init
    I2C1_GPIOInits();

    // I2C peripheral configuration
    I2C1_Inits();

    //enable the I2C peripheral
    I2C_PeripheralControl(I2C1, ENABLE);

    GPIO_OnBoardButtonInit();

    while(1){
        // Wait for button press
        while(!GPIO_ReadFromInputPin(GPIOA, GPIO_PIN_NO_0));

        // To avoid the de-bouncing
        delay();

        // send some data
        I2C_MasterSendData(&I2CHandle, tx_data, strlen((char *)tx_data), SLAVE_ADDR, I2C_ENABLE_SR );
    }
}
#endif //(FEATURE_SELECTED == STM32_I2C_MSTR_TX)

/*
 * Slave source code
 *
 * */
#if 0
    // Wire Slave Receiver
    //Uno, Ethernet A4 (SDA), A5 (SCL)
    #include <Wire.h>

    #define MY_ADDR   0x68

    int LED = 13;
    char rx_buffer[32] ;
    uint32_t cnt =0;
    uint8_t message[50];
    void setup() {

      Serial.begin(9600);
      // Define the LED pin as Output
      pinMode (LED, OUTPUT);

     // Start the I2C Bus as Slave on address 0X69
      Wire.begin(MY_ADDR);

      // Attach a function to trigger when something is received.
      Wire.onReceive(receiveEvent);

      sprintf(message,"Slave is ready : Address 0x%x",MY_ADDR);
      Serial.println((char*)message );
      Serial.println("Waiting for data from master");
    }

    void loop(void)
    {

    }

    void receiveEvent(int bytes)
    {
     while( Wire.available() )
     {
       rx_buffer[cnt++] = Wire.read();
     }
      rx_buffer[cnt] = '\0';
      cnt=0;
      Serial.print("Received:");
      Serial.println((char*)rx_buffer);
    }
#endif

