/*******************************************************************************
 * @file           spi_master_slave_comm.c
 * @brief          Brief description
 * 
 * @author         Fahim Akhtar
 * @date           28-Oct-2025
 * @time           10:48:51 AM
 * @version        1.0
 * @copyright      (c) 2025
 ******************************************************************************/

#include "spi_master_slave_comm.h"
#if (FEATURE_SELECTED == STM32_SPI2_MASTER_SLAVE_COMM)
#include "common.h"

/*
 * Exercise : SPI Master (STM) & SPI Slave (Arduino) command & response based communication.
 * When the button on the master is pressed, Master sends a command to the slave and
 * Slave responds as per the command implementation.
 *
 * Command formats:
 *      <command_code(1)>   <arg1>              <arg2>
 * 1.   CMD_LED_CTRL        <pin no(1)>         <value(1)>
 * 2.   CMD_SENSOR_READ     <analog pin number> -----------
 * 3.   CMD_LED_READ        <pin no(1)>         -----------
 * 4.   CMD_PRINT           <len(2)>            <message(len)>
 * 5.   CMD_ID_READ
 *
 * Note : Slave does not know how many bytes of data master is going to send.
 *        So master first sends the number of bytes info which slave is going to receive.
 * */

/* PIN CONFIGURATION :
 *
 * STM32F407 Discovery Board Pins
 * PB12 : SPI2_NSS
 * PB13 : SPI2_SCLK
 * PB14 : SPI2_MISO
 * PB15 : SPI2_MOSI
 * ALT Function mode : 5
 *
 * Arduino
 * PIN10 : CS
 * PIN11 : MOSI
 * PIN12 : MISO
 * PIN13 : SCLK
 *
 * */

//Command Codes
#define COMMAND_LED_CTRL        0x50
#define COMMAND_SENSOR_READ     0x51
#define COMMAND_LED_READ        0x52
#define COMMAND_PRINT           0x53
#define COMMAND_ID_READ         0x54

#define LED_ON      1
#define LED_OFF     0

#define NACK 0xA5
#define ACK 0xF5

//Arduino analog pins
#define ANALOG_PIN0     0
#define ANALOG_PIN1     1
#define ANALOG_PIN2     2
#define ANALOG_PIN3     3
#define ANALOG_PIN4     4

// Arduino led pin
#define LED_PIN         9


static void SPI2_GPIOInits(void){

    GPIO_Handle_t SPIPins;

    SPIPins.pGPIOx = GPIOB;
    SPIPins.GPIO_PinConfig.GPIO_PinMode = GPIO_MODE_ALTFN;
    SPIPins.GPIO_PinConfig.GPIO_PinAltFunMode = 5;
    SPIPins.GPIO_PinConfig.GPIO_PinOPType = GPIO_OP_TYPE_PP;
    SPIPins.GPIO_PinConfig.GPIO_PinPuPdControl = GPIO_NO_PUPD;
    SPIPins.GPIO_PinConfig.GPIO_PinSpeed = GPIO_SPEED_FAST;

    // Configure the NSS
    SPIPins.GPIO_PinConfig.GPIO_PinNumber = GPIO_PIN_NO_12;
    GPIO_Init(&SPIPins);

    // Configure the SCLK
    SPIPins.GPIO_PinConfig.GPIO_PinNumber = GPIO_PIN_NO_13;
    GPIO_Init(&SPIPins);

    // As in this application we are not using slave so we can disable below pin

    // Configure the MISO
    SPIPins.GPIO_PinConfig.GPIO_PinNumber = GPIO_PIN_NO_14;
    GPIO_Init(&SPIPins);

    // Configure the MOSI
    SPIPins.GPIO_PinConfig.GPIO_PinNumber = GPIO_PIN_NO_15;
    GPIO_Init(&SPIPins);

}


static void SPI2_inits(void) {

    SPI_Handle_t SPI2handle;

    SPI2handle.pSPIx = SPI2;
    SPI2handle.SPIConfig.SPI_BusConfig = SPI_BUS_CONFIG_FD;
    SPI2handle.SPIConfig.SPI_DeviceMode = SPI_DEVICE_MASTER_MODE;
    SPI2handle.SPIConfig.SPI_SclkSpeed = SPI_SCLK_SPEED_DIV32;
    SPI2handle.SPIConfig.SPI_DFF = SPI_DFF_8BITS;
    SPI2handle.SPIConfig.SPI_CPOL = SPI_CPOL_LOW;
    SPI2handle.SPIConfig.SPI_CPHA = SPI_CPHA_LOW;
    SPI2handle.SPIConfig.SPI_SSM = SPI_SSM_DI; // Hardware slave

    SPI_Init(&SPI2handle);
}

static void GPIO_ButtonInit(void){

    GPIO_Handle_t GpioBtn;

    GpioBtn.pGPIOx = GPIOA;
    GpioBtn.GPIO_PinConfig.GPIO_PinNumber = GPIO_PIN_NO_0;
    GpioBtn.GPIO_PinConfig.GPIO_PinMode = GPIO_MODE_IN;
    GpioBtn.GPIO_PinConfig.GPIO_PinSpeed = GPIO_SPEED_FAST;
    GpioBtn.GPIO_PinConfig.GPIO_PinPuPdControl = GPIO_NO_PUPD;
    GPIO_Init(&GpioBtn);

}

uint8_t SPI_VerifyResponse(uint8_t rspByte) {

    if(rspByte == ACK){
        return 1;
    }
    return 0;
}


void spi2_communicate_with_slave(void){

    uint8_t dummyTx = 0xFF;
    uint8_t dummyRx;

    SPI2_GPIOInits();

    SPI2_inits();

    GPIO_ButtonInit();

    SPI_SSOEConfig(SPI2, ENABLE);

    while(1){
        while(!GPIO_ReadFromInputPin(GPIOA, GPIO_PIN_NO_0));

        delay();

        // enable the SPI2 Peripheral
        SPI_PeripheralControl(SPI2, ENABLE);

        //1.   CMD_LED_CTRL    <pin no(1)>     <value(1)>

        // Send the LED CTRL command
        uint8_t cmdLedCtrl = COMMAND_LED_CTRL;
        SPI_SendData_Polling(SPI2, (uint8_t *)&cmdLedCtrl, 1);

        // Read the dummy byte to avoid the Overrun error (OVR)
        SPI_ReceiveData_Polling(SPI2, &dummyRx, 1);

        // Send the dummy write so Master can generate the clock.
        SPI_SendData_Polling(SPI2, (uint8_t *)&dummyTx, 1);

        //read the response of the command
        uint8_t rspByte = 0x00;
        SPI_ReceiveData_Polling(SPI2, &rspByte, 1);

        //Check the ACK or NACK
        if(SPI_VerifyResponse(rspByte)){
            uint8_t payload[2];
            payload[0] = LED_PIN;
            payload[1] = LED_ON;
            //SPI_SendData_Polling(SPI2, (uint8_t *)&payload, sizeof(payload));
            for(uint8_t i = 0; i <sizeof(payload); i++){
                SPI_SendData_Polling(SPI2, &payload[i], 1);
                // Read the dummy byte
                SPI_ReceiveData_Polling(SPI2, &dummyRx, 1);
            }
        }

        // Let confirm SPI isnot busy
        while(SPI_GetFlagStatus(SPI2, SPI_BSY_FLAG));

        // Disable the SPI2 Peripheral
        SPI_PeripheralControl(SPI2, DISABLE);
    }
}

#endif

#if 0
// Below code is for Slave device (Arduino)

// Just create a sample.ino file and import into Arduino IDE & Flash into device.
// It will start waiting for master to pull the SS line low.


#include <SPI.h>

const byte led = 9;           // Slave LED digital I/O pin
uint8_t board_id[] = "ARDUINOUNO";
uint8_t dataBuff[255];

#define NACK 0xA5
#define ACK  0xF5

// Command codes
#define COMMAND_LED_CTRL      0x50
#define COMMAND_SENSOR_READ   0x51
#define COMMAND_LED_READ      0x52
#define COMMAND_PRINT         0x53
#define COMMAND_ID_READ       0x54

#define LED_ON  1
#define LED_OFF 0

volatile uint8_t spi_state = 0;
volatile uint8_t command = 0;
volatile uint8_t index = 0;
volatile uint8_t payload[32];
volatile bool command_ready = false;

void setup() {
  Serial.begin(9600);
  pinMode(led, OUTPUT);
  digitalWrite(led, LOW);

  // SPI pin setup
  pinMode(MISO, OUTPUT);
  pinMode(MOSI, INPUT);
  pinMode(SCK, INPUT);
  pinMode(SS, INPUT);

  // Enable SPI in slave mode + interrupt
  SPCR |= (1 << SPE) | (1 << SPIE);

  Serial.println("SPI Slave Ready");
}

ISR(SPI_STC_vect) {
  uint8_t received = SPDR;

  switch (spi_state) {
    case 0: // Receive command
      command = received;
      SPDR = ACK; // Preload ACK
      spi_state = 1;
      break;

    case 1: // Dummy byte after ACK
      SPDR = 0xFF;
      spi_state = 2;
      index = 0;
      break;

    case 2: // Handle command-specific payload
      if (command == COMMAND_LED_CTRL) {
        payload[index++] = received;
        if (index == 2) {
          digitalWrite(payload[0], payload[1] == LED_ON ? HIGH : LOW);
          spi_state = 0;
        }
        SPDR = 0xFF;
      }

      else if (command == COMMAND_SENSOR_READ) {
        uint8_t analogVal = map(analogRead(received + 14), 0, 1023, 0, 255);
        SPDR = analogVal;
        spi_state = 0;
      }

      else if (command == COMMAND_LED_READ) {
        SPDR = digitalRead(received);
        spi_state = 0;
      }

      else if (command == COMMAND_PRINT) {
        if (index == 0) {
          index = received; // length
          payload[0] = 0;
          spi_state = 3;
        }
        SPDR = 0xFF;
      }

      else if (command == COMMAND_ID_READ) {
        SPDR = board_id[index++];
        if (index >= strlen((char*)board_id)) {
          spi_state = 0;
        }
      }

      else {
        SPDR = NACK;
        spi_state = 0;
      }
      break;

    case 3: // Receiving print payload
      static uint8_t printIndex = 0;
      dataBuff[printIndex++] = received;
      if (printIndex >= index) {
        dataBuff[printIndex] = '\0';
        Serial.print("RCVD: ");
        Serial.println((char*)dataBuff);
        spi_state = 0;
        printIndex = 0;
      }
      SPDR = 0xFF;
      break;
  }
}

void loop() {
  // Nothing needed here — all SPI handled in ISR
}


#endif



