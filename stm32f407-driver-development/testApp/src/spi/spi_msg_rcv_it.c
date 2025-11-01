/*******************************************************************************
 * @file           spi_msg_rcv_it.c
 * @brief          Brief description
 * 
 * @author         Fahim Akhtar
 * @date           01-Nov-2025
 * @time           12:37:37 PM
 * @version        1.0
 * @copyright      (c) 2025
 ******************************************************************************/

/*
 * This application receives and prints the user message received from the Arduino peripheral in SPI interrupt mode
 * User sends the message through Arduino IDE's serial monitor tool
 * Monitor the message received in the SWV itm data console
 */
/*
 * Note : Follow the instructions to test this code
 * 1. Download this code on to STM32 board , acts as Master
 * 2. Download Slave code (Giving at end of this file)on to Arduino board (Slave)
 * 3. Reset both the boards
 * 4. Enable SWV ITM data console to see the message
 * 5. Open Arduino IDE serial monitor tool
 * 6. Type anything and send the message (Make sure that in the serial monitor tool line ending set to carriage return)
 */

/* PIN CONFIGURATION :
 *
 * STM32F407 Discovery Board Pins
 * PB12 : SPI2_NSS
 * PB13 : SPI2_SCLK
 * PB14 : SPI2_MISO
 * PB15 : SPI2_MOSI
 * ALT Function mode : 5
 * PD6  : IRQ in
 *
 * Arduino
 * PIN10 : CS
 * PIN11 : MOSI
 * PIN12 : MISO
 * PIN13 : SCLK
 * PIN8  : IRQ out
 *
 * */


#include "spi_msg_rcv_it.h"
#if (FEATURE_SELECTED == STM32_SPI2_READ_DATA_FROM_SLAVE)
#include "common.h"
#include "stm32f407xx.h"


SPI_Handle_t SPI2handle;

#define MAX_LEN 500

char RcvBuff[MAX_LEN];

char ReadByte;


volatile uint8_t rcvStop = 0;

/*This flag will be set in the interrupt handler of the Arduino interrupt GPIO */
volatile uint8_t dataAvailable = 0;

static void SPI2_GPIOInits(void){
    GPIO_Handle_t SPIPins;

    SPIPins.pGPIOx = GPIOB;
    SPIPins.GPIO_PinConfig.GPIO_PinMode = GPIO_MODE_ALTFN;
    SPIPins.GPIO_PinConfig.GPIO_PinAltFunMode = 5;
    SPIPins.GPIO_PinConfig.GPIO_PinOPType = GPIO_OP_TYPE_PP;
    SPIPins.GPIO_PinConfig.GPIO_PinPuPdControl = GPIO_NO_PUPD;
    SPIPins.GPIO_PinConfig.GPIO_PinSpeed = GPIO_SPEED_FAST;

    //SCLK
    SPIPins.GPIO_PinConfig.GPIO_PinNumber = GPIO_PIN_NO_13;
    GPIO_Init(&SPIPins);

    //MOSI
    SPIPins.GPIO_PinConfig.GPIO_PinNumber = GPIO_PIN_NO_15;
    GPIO_Init(&SPIPins);

    //MISO
    SPIPins.GPIO_PinConfig.GPIO_PinNumber = GPIO_PIN_NO_14;
    GPIO_Init(&SPIPins);


    //NSS
    SPIPins.GPIO_PinConfig.GPIO_PinNumber = GPIO_PIN_NO_12;
    GPIO_Init(&SPIPins);


}

static void SPI2_Inits(void)
{
    SPI2handle.pSPIx = SPI2;
    SPI2handle.SPIConfig.SPI_BusConfig = SPI_BUS_CONFIG_FD;
    SPI2handle.SPIConfig.SPI_DeviceMode = SPI_DEVICE_MASTER_MODE;
    SPI2handle.SPIConfig.SPI_SclkSpeed = SPI_SCLK_SPEED_DIV32;
    SPI2handle.SPIConfig.SPI_DFF = SPI_DFF_8BITS;
    SPI2handle.SPIConfig.SPI_CPOL = SPI_CPOL_LOW;
    SPI2handle.SPIConfig.SPI_CPHA = SPI_CPHA_LOW;
    SPI2handle.SPIConfig.SPI_SSM = SPI_SSM_DI; //Hardware slave management enabled for NSS pin

    SPI_Init(&SPI2handle);
}

/* Slave data available interrupt handler */
void EXTI9_5_IRQHandler(void){
    GPIO_IRQHandling(GPIO_PIN_NO_6);
    dataAvailable = 1;
}

/*This function configures the gpio pin over which SPI peripheral issues data available interrupt */
static void Slave_GPIO_InterruptPinInit(void){
    GPIO_Handle_t spiIntPin;
    memset(&spiIntPin,0,sizeof(spiIntPin));

    //this is led gpio configuration
    spiIntPin.pGPIOx = GPIOD;
    spiIntPin.GPIO_PinConfig.GPIO_PinNumber = GPIO_PIN_NO_6;
    spiIntPin.GPIO_PinConfig.GPIO_PinMode = GPIO_MODE_IT_FT;
    spiIntPin.GPIO_PinConfig.GPIO_PinSpeed = GPIO_SPEED_LOW;
    spiIntPin.GPIO_PinConfig.GPIO_PinPuPdControl = GPIO_PIN_PU;

    GPIO_Init(&spiIntPin);

    GPIO_IRQPriorityConfig(IRQ_NO_EXTI9_5,NVIC_IRQ_PRI15);
    GPIO_IRQInterruptConfig(IRQ_NO_EXTI9_5,ENABLE);

}


void spi2_read_msg_from_slave(void ) {

    uint8_t dummy = 0xff;

    Slave_GPIO_InterruptPinInit();

    //this function is used to initialize the GPIO pins to behave as SPI2 pins
    SPI2_GPIOInits();

    //This function is used to initialize the SPI2 peripheral parameters
    SPI2_Inits();

    /*
    * making SSOE 1 does NSS output enable.
    * The NSS pin is automatically managed by the hardware.
    * i.e when SPE=1 , NSS will be pulled to low
    * and NSS pin will be high when SPE=0
    */
    SPI_SSOEConfig(SPI2,ENABLE);

    SPI_IRQInterruptConfig(IRQ_NO_SPI2,ENABLE);

    while(1){

        rcvStop = 0;

        while(!dataAvailable); //wait till data available interrupt from transmitter device(slave)

        GPIO_IRQInterruptConfig(IRQ_NO_EXTI9_5,DISABLE);

        //enable the SPI2 peripheral
        SPI_PeripheralControl(SPI2,ENABLE);


        while(!rcvStop)
        {
            /* fetch the data from the SPI peripheral byte by byte in interrupt mode */
            while ( SPI_SendDataIT(&SPI2handle,&dummy,1) == SPI_BUSY_IN_TX);
            while ( SPI_ReceiveDataIT(&SPI2handle,(uint8_t *)&ReadByte,1) == SPI_BUSY_IN_RX );
        }


        // confirm SPI is not busy
        while( SPI_GetFlagStatus(SPI2,SPI_BSY_FLAG) );

        //Disable the SPI2 peripheral
        SPI_PeripheralControl(SPI2,DISABLE);

        printf("Rcvd data = %s\n",RcvBuff);

        dataAvailable = 0;

        GPIO_IRQInterruptConfig(IRQ_NO_EXTI9_5,ENABLE);
    }
}

/* Runs when a data byte is received from the peripheral over SPI*/
void SPI2_IRQHandler(void) {
    SPI_IRQHandling(&SPI2handle);
}

void SPI_ApplicationEventCallback(SPI_Handle_t *pSPIHandle,uint8_t AppEv){
    static uint32_t i = 0;
    /* In the RX complete event , copy data in to rcv buffer . '\0' indicates end of message(rcvStop = 1) */
    if(AppEv == SPI_EVENT_RX_CMPLT)
    {
        RcvBuff[i++] = ReadByte;
        if(ReadByte == '\0' || ( i == MAX_LEN)){
            rcvStop = 1;
            RcvBuff[i-1] = '\0';
            i = 0;
        }
    }
}
#endif

#if 0
// Slave (Arduino) code. Just create slave.ino and paste the below code and flash in the board

#include <SPI.h>

#define MAX_LEN 500

bool msgComplete = false;  // whether the string is complete
uint8_t userBuffer[MAX_LEN];
uint32_t cnt = 0;

//Initialize SPI slave.
void SPI_SlaveInit(void)
{
  // Initialize SPI pins.
  pinMode(SCK, INPUT);
  pinMode(MOSI, INPUT);
  pinMode(MISO, OUTPUT);
  pinMode(SS, INPUT);
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
void SPI_SlaveTransmit(uint8_t data)
{
  /* Start transmission */
  SPDR = data;

  /* Wait for transmission complete */
  while(!(SPSR & (1<<SPIF)));
}

void setup()
{
  // Initialize serial for troubleshooting.
  Serial.begin(9600);

  // Initialize SPI Slave.
  SPI_SlaveInit();

  pinMode(8, INPUT_PULLUP);
  //digitalWrite(8,LOW);

  Serial.println("Slave Initialized");
}

void notify_controller(void)
{
  pinMode(8,OUTPUT);
  digitalWrite(8,HIGH);
  delayMicroseconds(50);
  digitalWrite(8,LOW);
}


void loop() {

  Serial.println("Type anything and send...");

  while(!msgComplete){
    if (Serial.available()) {
      //Read a byte of incoming serial data.
      char readByte = (char)Serial.read();
      //Accumalate in to the buffer
      userBuffer[cnt++] = readByte;
      if(readByte == '\r' || ( cnt == MAX_LEN)){
        msgComplete = true;
        userBuffer[cnt -1 ] = '\0'; //replace '\r' by '\0'
      }
    }
  }

  Serial.println("Your message...");
  Serial.println((char*)userBuffer);


   notify_controller();

  /*Transmit the user buffer over SPI */
  for(uint32_t i = 0 ; i < cnt ; i++)
  {
    SPI_SlaveTransmit(userBuffer[i]);
  }
  cnt = 0;
  msgComplete = false;
  Serial.println("Message sent...");

  while(!digitalRead(SS));
  Serial.println("Master ends communication");

}


#endif


