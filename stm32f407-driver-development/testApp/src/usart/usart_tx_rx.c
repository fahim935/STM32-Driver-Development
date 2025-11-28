/*******************************************************************************
 * @file           usart_tx_rx.c
 * @brief          Brief description
 * 
 * @author         Fahim Akhtar
 * @date           27-Nov-2025
 * @time           11:59:16 AM
 * @version        1.0
 * @copyright      (c) 2025
 *
 * Usecase :
 * Write a program to send some message over UART from the STM32 board to Arduino.
 * Arduino will toggel the case of message and send back to STM32.
 * Eg. Hello World 123 --> hELLO wORLD 123
 *
 * Baudrate         : 115200 bps
 * Frame formate    : 1 stop bits, 8 bits, no parity
 ******************************************************************************/

#include "usart_tx_rx.h"

#if (FEATURE_SELECTED == STM32_USART_TX_RX)
#include "common.h"

//3 different messages that we transmit to arduino
char *msg[3] = {"hihihihihihi123", "Hello How are you ?" , "Today is Monday !"};

//reply from arduino will be stored here
char rx_buf[1024] ;

USART_Handle_t usart2_handle;

//This flag indicates reception completion
uint8_t rxCmplt = RESET;

uint8_t g_data = 0;

extern void initialise_monitor_handles();

void USART2_Init(void)
{
    usart2_handle.pUSARTx = USART2;
    usart2_handle.USART_Config.USART_Baud = USART_STD_BAUD_115200;
    usart2_handle.USART_Config.USART_HWFlowControl = USART_HW_FLOW_CTRL_NONE;
    usart2_handle.USART_Config.USART_Mode = USART_MODE_TXRX;
    usart2_handle.USART_Config.USART_NoOfStopBits = USART_STOPBITS_1;
    usart2_handle.USART_Config.USART_WordLength = USART_WORDLEN_8BITS;
    usart2_handle.USART_Config.USART_ParityControl = USART_PARITY_DISABLE;
    USART_Init(&usart2_handle);
}

void    USART2_GPIOInit(void)
{
    GPIO_Handle_t usart_gpios;

    usart_gpios.pGPIOx = GPIOA;
    usart_gpios.GPIO_PinConfig.GPIO_PinMode = GPIO_MODE_ALTFN;
    usart_gpios.GPIO_PinConfig.GPIO_PinOPType = GPIO_OP_TYPE_PP;
    usart_gpios.GPIO_PinConfig.GPIO_PinPuPdControl = GPIO_PIN_PU;
    usart_gpios.GPIO_PinConfig.GPIO_PinSpeed = GPIO_SPEED_FAST;
    usart_gpios.GPIO_PinConfig.GPIO_PinAltFunMode =7;

    usart_gpios.GPIO_PinConfig.GPIO_PinNumber  = GPIO_PIN_NO_2;
    GPIO_Init(&usart_gpios);

    usart_gpios.GPIO_PinConfig.GPIO_PinNumber = GPIO_PIN_NO_3;
    GPIO_Init(&usart_gpios);

}

void GPIO_ButtonInit(void)
{
    GPIO_Handle_t GPIOBtn,GpioLed;

    //this is btn gpio configuration
    GPIOBtn.pGPIOx = GPIOA;
    GPIOBtn.GPIO_PinConfig.GPIO_PinNumber = GPIO_PIN_NO_0;
    GPIOBtn.GPIO_PinConfig.GPIO_PinMode = GPIO_MODE_IN;
    GPIOBtn.GPIO_PinConfig.GPIO_PinSpeed = GPIO_SPEED_FAST;
    GPIOBtn.GPIO_PinConfig.GPIO_PinPuPdControl = GPIO_NO_PUPD;

    GPIO_Init(&GPIOBtn);

    //this is led gpio configuration
    GpioLed.pGPIOx = GPIOD;
    GpioLed.GPIO_PinConfig.GPIO_PinNumber = GPIO_PIN_NO_12;
    GpioLed.GPIO_PinConfig.GPIO_PinMode = GPIO_MODE_OUT;
    GpioLed.GPIO_PinConfig.GPIO_PinSpeed = GPIO_SPEED_FAST;
    GpioLed.GPIO_PinConfig.GPIO_PinOPType = GPIO_OP_TYPE_OD;
    GpioLed.GPIO_PinConfig.GPIO_PinPuPdControl = GPIO_NO_PUPD;

    GPIO_PeriClockControl(GPIOD,ENABLE);

    GPIO_Init(&GpioLed);
}

void  usart_tx_rx_testing(void)
{
    uint32_t cnt = 0;


    initialise_monitor_handles();

    USART2_GPIOInit();

    USART2_Init();

    USART_IRQInterruptConfig(IRQ_NO_USART2,ENABLE);

    USART_PeripheralControl(USART2,ENABLE);

    printf("Application is running\n");

    //do forever
    while(1)
    {
        //wait till button is pressed
        while( ! GPIO_ReadFromInputPin(GPIOA,GPIO_PIN_NO_0) );

        //to avoid button de-bouncing related issues 200ms of delay
        delay();

        // Next message index ; make sure that cnt value doesn't cross 2
        cnt = cnt % 3;

        // Enable the interrupt to receive data from Arduino. Non-Blocking call
        while ( USART_ReceiveDataIT(&usart2_handle,(uint8_t *)rx_buf,strlen(msg[cnt])) != USART_READY );

        //Send the msg indexed by cnt in blocking mode
        USART_SendData(&usart2_handle,(uint8_t*)msg[cnt],strlen(msg[cnt]));

        printf("Transmitted : %s\n",msg[cnt]);

        //When all the bytes are received rxCmplt will be SET in application callback
        while(rxCmplt != SET);

        //just make sure that last byte should be null otherwise %s fails while printing
        rx_buf[strlen(msg[cnt])+ 1] = '\0';

        printf("Received    : %s\n",rx_buf);

        //invalidate the flag
        rxCmplt = RESET;

        //move on to next message indexed in msg[]
        cnt++;
    }
}


void USART2_IRQHandler(void)
{
    USART_IRQHandling(&usart2_handle);
}

void USART_ApplicationEventCallback( USART_Handle_t *pUSARTHandle,uint8_t ApEv)
{
   if(ApEv == USART_EVENT_RX_CMPLT)
   {
            rxCmplt = SET;

   }else if (ApEv == USART_EVENT_TX_CMPLT)
   {
       ;
   }
   // More events can be handled here
}

#if 0

// Arduino code
void setup() {
  Serial.begin(115200);

  // Define the LED pin as Output
  pinMode (13, OUTPUT);

 // Serial.println("Arduino Case Converter program running");
 // Serial.println("-------------------------------------");


}

char changeCase(char ch)
{
  if (ch >= 'A' && ch <= 'Z')
  ch = ch + 32;
    else if (ch >= 'a' && ch <= 'z')
  ch = ch - 32;

  return ch;
}
void loop() {

  digitalWrite(13, LOW);
  //wait until something is received
  while(! Serial.available());
  digitalWrite(13, HIGH);
  //read the data
  char in_read=Serial.read();
  //print the data
  Serial.print(changeCase(in_read));
}

#endif

#endif
