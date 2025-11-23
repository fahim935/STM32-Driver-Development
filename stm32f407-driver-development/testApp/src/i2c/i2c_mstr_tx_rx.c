/*******************************************************************************
 * @file           i2c_master_rx_data_from_slave.c
 * @brief          Brief description
 * 
 * @author         Fahim Akhtar
 * @date           16-Nov-2025
 * @time           4:15:47 PM
 * @version        1.0
 * @copyright      (c) 2025
 ******************************************************************************/

#include <i2c_mstr_tx_rx.h>

#if (FEATURE_SELECTED == STM32_I2C_MSTR_TX_RX)
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


void i2c_master_rx_testing(void){

    uint8_t getDataLenCmd = GET_DATA_LEN_CMD;
    uint8_t getFullData = GET_FULL_DATA_CMD;
    uint8_t dataLen = 0x00;
    uint8_t data[50];

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

        I2C_ManageAcking(I2CHandle.pI2Cx, I2C_ACK_ENABLE);

        // Send GET_DATA_LEN_CMD to get the length of the data
        I2C_MasterSendData(&I2CHandle, &getDataLenCmd, sizeof(getDataLenCmd), SLAVE_ADDR, I2C_DISABLE_SR );

        // Read the data length field
        I2C_MasterReceiveData(&I2CHandle, &dataLen, sizeof(dataLen), SLAVE_ADDR, I2C_DISABLE_SR );

        //printf("Data Length = %d\n",dataLen);

        // Send GET_FULL_DATA_CMD to get the full data
        I2C_MasterSendData(&I2CHandle, &getFullData, sizeof(getFullData), SLAVE_ADDR, I2C_DISABLE_SR );

        // Read the Full data
        I2C_MasterReceiveData(&I2CHandle, data, dataLen, SLAVE_ADDR, I2C_ENABLE_SR );

//        for(uint8_t i = 0; i < dataLen; i++){
//            printf("%X",data[i]);
//        }
//        printf("\n");
    }
}
#endif
// Slave code just create slave.ino and improt to IDE
#if 0

// Wire Slave Transmitter and receiver
//Uno, Ethernet A4 (SDA), A5 (SCL)
#include <Wire.h>

// Include the required Wire library for I2C<br>#include <Wire.h>
int LED = 13;
uint8_t active_command = 0xff,led_status=0;
char name_msg[32] = "Hi Fahim - Did you rx my msg\n";
#define MY_ADDR   0x68
uint8_t message[50];

uint16_t device_id = 0xFF45;

#define SLAVE_ADDR 0x68

uint8_t get_len_of_data(void)
{
  return (uint8_t)strlen(name_msg);
}
void setup() {
  Serial.begin(9600);
  // Define the LED pin as Output
  pinMode (LED, OUTPUT);

  // Start the I2C Bus as Slave on address 9
  Wire.begin(SLAVE_ADDR);

  // Attach a function to trigger when something is received.
  Wire.onReceive(receiveEvent);
  Wire.onRequest(requestEvent);

  sprintf(message,"Slave is ready : Address 0x%x",MY_ADDR);
  Serial.println((char*)message );
  Serial.println("Waiting for data from master");

}


//write
void receiveEvent(int bytes) {
  active_command = Wire.read();    // read one character from the I2C
}

//read
void requestEvent() {

  Serial.println("requestEvent Rx");
  if(active_command == 0X51)
  {
    Serial.println("requestEvent : Data Length");
    uint8_t len = get_len_of_data();
    Wire.write(&len,1);
    active_command = 0xff;
  }


  if(active_command == 0x52)
  {
    Serial.println("requestEvent : Full Data");
   // Wire.write(strlen(name));
    Wire.write(name_msg,get_len_of_data());
   // Wire.write((uint8_t*)&name_msg[32],18);
    active_command = 0xff;
  }
  //Wire.write("hello "); // respond with message of 6 bytes
  // as expected by master
}
void loop() {


}


#endif
