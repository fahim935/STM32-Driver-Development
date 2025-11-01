/*******************************************************************************
 * @file           spi_send_data_slave.c
 * @brief          Brief description
 * 
 * @author         Fahim Akhtar
 * @date           26-Oct-2025
 * @time           7:12:44 PM
 * @version        1.0
 * @copyright      (c) 2025
 ******************************************************************************/


#include <spi_send_data_slave.h>
#if (FEATURE_SELECTED == STM32_SPI2_SEND_DATA_TO_SLAVE)
#include "common.h"


/*
 * Exercise : SPI Master (STM) & SPI Slave (Arduino) Communication.
 * When the button on the master is pressed, Master should send string of data to
 * the Arduino Slave connected. The data received by Arduino will be displayed on
 * the Arduino serial port.
 *
 * Note : Slave does not know how many bytes of data master is going to send.
 *        So master first sends the number of bytes info which slave is going to receive.
 * */


#include "stm32f407xx_gpio_driver.h"
#include "stm32f407xx_spi_driver.h"
#include <string.h>

static void SPI2_GPIOInits(void){

    /* STM32F407 Discovery Board Pins
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
    SPI2handle.SPIConfig.SPI_SclkSpeed = SPI_SCLK_SPEED_DIV8;
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


void spi2_send_data_to_slave(void){

    char user_data[] = "Hello SPI Data";

    SPI2_GPIOInits();

    SPI2_inits();

    GPIO_ButtonInit();

    SPI_SSOEConfig(SPI2, ENABLE);

    while(1){
        while(!GPIO_ReadFromInputPin(GPIOA, GPIO_PIN_NO_0));

        delay();

        // enable the SPI2 Peripheral
        SPI_PeripheralControl(SPI2, ENABLE);

        //first send length info
        uint8_t data_len = strlen(user_data);
        SPI_SendData_Polling(SPI2, (uint8_t *)&data_len, 1);

        SPI_SendData_Polling(SPI2, (uint8_t *)user_data, strlen(user_data));

        // Let confirm SPI isnot busy
        while(SPI_GetFlagStatus(SPI2, SPI_BSY_FLAG));

        // Disable the SPI2 Peripheral
        SPI_PeripheralControl(SPI2, DISABLE);
    }
}
#endif


#if 0

The below code is for Slave Arduino Uno.
Just create on file slaveCode.ino
paste the below code and flash into Arduino board.

/* SPI Slave Demo

 *
 * SPI pin numbers:
 * SCK   13  // Serial Clock.
 * MISO  12  // Master In Slave Out.
 * MOSI  11  // Master Out Slave In.
 * SS    10  // Slave Select . Arduino SPI pins respond only if SS pulled low by the master
 *

 */
#include <SPI.h>
#include<stdint.h>
#define SPI_SCK 13
#define SPI_MISO 12
#define SPI_MOSI 11
#define SPI_SS 10

char dataBuff[500];

//Initialize SPI slave.
void SPI_SlaveInit(void)
{
  // Initialize SPI pins.
  pinMode(SCK, INPUT);
  pinMode(MOSI, INPUT);
  pinMode(MISO, OUTPUT);
  pinMode(SS, INPUT);
  //make SPI as slave

  // Enable SPI as slave.
  SPCR = (1 << SPE);
}

//This function returns SPDR Contents
uint8_t SPI_SlaveReceive(void)
{
  /* Wait for reception complete */
  while(!(SPSR & (1<<SPIF)));
  /* Return Data Register */
  return SPDR;
}


//sends one byte of data
void SPI_SlaveTransmit(char data)
{
  /* Start transmission */
  SPDR = data;
  /* Wait for transmission complete */
  while(!(SPSR & (1<<SPIF)));
}


// The setup() function runs right after reset.
void setup()
{
  // Initialize serial communication
  Serial.begin(9600);
  // Initialize SPI Slave.
  SPI_SlaveInit();
  Serial.println("Slave Initialized");
}

// The loop function runs continuously after setup().
void loop()
{
  uint32_t i;
  uint16_t dataLen = 0;
  Serial.println("Slave waiting for ss to go low");
  while(digitalRead(SS));

  i = 0;
  dataLen = SPI_SlaveReceive();
  for(i = 0 ; i < dataLen ; i++ )
  {
    dataBuff[i] =  SPI_SlaveReceive();
  }


  //  Serial.println(String(i,HEX));
  dataBuff[i] = '\0';

  Serial.println("Rcvd:");
  Serial.println(dataBuff);
  Serial.print("Length:");
  Serial.println(dataLen);
}


#endif
