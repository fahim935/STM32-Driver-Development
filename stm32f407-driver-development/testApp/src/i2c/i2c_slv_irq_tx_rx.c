/*******************************************************************************
 * @file           i2c_slv_irq_tx_rx.c
 * @brief          Brief description
 * 
 * @author         Fahim Akhtar
 * @date           23-Nov-2025
 * @time           11:51:45 AM
 * @version        1.0
 * @copyright      (c) 2025
 ******************************************************************************/


#include "i2c_slv_irq_tx_rx.h"
#if (FEATURE_SELECTED == STM32_I2C_SLV_IRQ_TX_RX)
#include "common.h"

/*
 * Description:
 *  12C Master{Arduino) and I2C Slave(STM32) communication.
 *  Master should read and display data from STM32 Slave connected.
 *  First Master has to get the length of the data from the slave to read
 *  subsequent data from the slave.
 *      1. Use I2C SCL=100KHz[Standard mode
 *      2. Use internal pull resistors for SDA and SCL lines
 *  0x51 : Command to read the data length
 *  0x52 : Command to read the complete data from the slave
 * */

I2C_Handle_t I2CHandle;

//Flag variable
uint8_t rxComplt = RESET;
uint8_t txComplt = RESET;


#define SLAVE_ADDR  0x68
#define MY_ADDR     SLAVE_ADDR

#define GET_DATA_LEN_CMD    0x51
#define GET_FULL_DATA_CMD   0x52

uint8_t tx_data[32] = "STM32 Slave mode testing..";

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


void i2c_slv_irq_tx_rx_testing(void){

    printf("Application is running\n");

    GPIO_OnBoardButtonInit();

    // I2C Pin Init
    I2C1_GPIOInits();

    // I2C peripheral configuration
    I2C1_Inits();

    //I2C IRQ configurations
    I2C_IRQInterruptConfig(IRQ_NO_I2C1_EV, ENABLE);
    I2C_IRQInterruptConfig(IRQ_NO_I2C1_ER, ENABLE);

    //enable the I2C peripheral
    I2C_PeripheralControl(I2C1, ENABLE);

    I2C_SlaveEnableDisableCallbackEvents(I2C1, ENABLE);

    I2C_ManageAcking(I2CHandle.pI2Cx, I2C_ACK_ENABLE);

    while(1);
}

void I2C_ApplicationEventCallback(I2C_Handle_t *pI2CHandle,uint8_t AppEv) {

    static uint8_t commandCode = 0x00;
    static uint8_t Ctn = 0x00;

    if(AppEv == I2C_EV_DATA_REQ){
        // Master Wants some data. Slave has to send the data

        printf("Data request command Code Tx 0x%X  Ctn %d\n", commandCode, Ctn);

        if(commandCode == GET_DATA_LEN_CMD)
            I2C_SlaveSendData(pI2CHandle->pI2Cx, strlen((char *)tx_data));
        else if (commandCode == GET_FULL_DATA_CMD )
            I2C_SlaveSendData(pI2CHandle->pI2Cx, tx_data[Ctn++]);

    } else if(AppEv == I2C_EV_DATA_RCV){
        // Data is waiting for Slave to read. Slave has to read the data

        commandCode = I2C_SlaveReceiveData(pI2CHandle->pI2Cx);

        printf("command Code Rx 0x%X\n", commandCode);

    } else if(AppEv == I2C_ERROR_AF){
        //This happens only during slave transmitting
        //Master has sent the NACK. so the slave should understand
        //that master don't need more data
        commandCode = 0xFF;
        Ctn = 0x00;
        printf("I2C_ERROR_AF : Master don't need more data\n");

    } else if(AppEv == I2C_EV_STOP){
        //This happens only during slave reception
        //Master has ended the communication with the slave

    }

}

void I2C1_ER_IRQHandler(void){

    I2C_ERR_IRQHandling(&I2CHandle);

}

void I2C1_EV_IRQHandler(void){

    I2C_EVT_IRQHandling(&I2CHandle);
}

#endif

// Master Arduino sketch
#if 0

// Wire Master Transmitter and Receiver
//Uno, Ethernet A4 (SDA), A5 (SCL)
#include <Wire.h>

// Include the required Wire library for I2C<br>#include <Wire.h>
int LED = 13;

uint8_t rcv_buf[512];

uint32_t data_len=0,w_ptr = 0;
#define SLAVE_ADDR 0x68

void setup() {
    Serial.begin(9600);

  // Define the LED pin as Output
  pinMode (LED, OUTPUT);

  // join i2c bus (address optional for master)
  Wire.begin();
}


void loop() {

uint32_t rem_len=0,last_read=0;
  Serial.println("Arduino Master");
  Serial.println("Send character \"s\" to begin");
  Serial.println("-----------------------------");

   while(!Serial.available());
  char in_read=Serial.read();

  while(in_read != 's');

  Serial.println("Starting..");

  Wire.beginTransmission(SLAVE_ADDR);

  Wire.write(0X51); //Send this command to read the length
  Wire.endTransmission();


  Wire.requestFrom(SLAVE_ADDR,1); // Request the transmitted two bytes from the two registers

 for(uint32_t len = 0 ; len < 4; len++)
 {
  if(Wire.available()) {  //
    uint32_t data = (uint32_t)Wire.read();
    data_len |= ( data << ( 8 * len) ); // Reads the data
  }
 }
 //data_len = 0x2f8;
  Serial.print("Data Length:");
  Serial.println(data_len);

  Wire.beginTransmission(SLAVE_ADDR);

  Wire.write(0X52); //Send this command to ask data
  Wire.endTransmission();

  rem_len = data_len;
 while(rem_len > 0)
 {
   if(rem_len <= 32)
   {
      Wire.requestFrom(SLAVE_ADDR,rem_len);
      last_read = rem_len;
      rem_len = 0;
   }else
   {
    Wire.requestFrom(SLAVE_ADDR,32);
    last_read = 32;
    rem_len -= 32;
   }
  uint32_t i=0;
  for( i =0; i <= last_read ; i++)
  {
    if(Wire.available()) {  //
      rcv_buf[w_ptr++] = Wire.read(); // Reads the data
    }
  }
 }

  rcv_buf[w_ptr] = '\0';
 w_ptr = 0;
  Serial.print("Data:");
  Serial.println((char*)rcv_buf);
  Serial.println("*********************END*********************");
}


#endif


