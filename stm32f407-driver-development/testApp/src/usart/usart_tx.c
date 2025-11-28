/*******************************************************************************
 * @file           usart_tx.c
 * @brief          Brief description
 * 
 * @author         Fahim Akhtar
 * @date           27-Nov-2025
 * @time           9:15:08 AM
 * @version        1.0
 * @copyright      (c) 2025
 *
 * Usecase :
 * Write a program to send some message over UART from the STM32 board to Arduino
 * board. The Arduino board will display the message (on Arduino serial monitor)
 * sent from the ST board.
 *
 * Baudrate         : 115200 bps
 * Frame formate    : 1 stop bits, 8 bits, no parity
 ******************************************************************************/

#include "usart_tx.h"

#if (FEATURE_SELECTED == STM32_USART_TX)
#include "common.h"

char msg[1024] = "USART ASYNC Tx testing, Hope you got the data ....\n\r";

USART_Handle_t usart2_handle;

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

void USART2_GPIOInit(void){

    GPIO_Handle_t usart_gpios;

    usart_gpios.pGPIOx = GPIOA;
    usart_gpios.GPIO_PinConfig.GPIO_PinMode = GPIO_MODE_ALTFN;
    usart_gpios.GPIO_PinConfig.GPIO_PinOPType = GPIO_OP_TYPE_PP;
    usart_gpios.GPIO_PinConfig.GPIO_PinPuPdControl = GPIO_PIN_PU;
    usart_gpios.GPIO_PinConfig.GPIO_PinAltFunMode = 7;
    usart_gpios.GPIO_PinConfig.GPIO_PinSpeed = GPIO_SPEED_FAST;

    //USART TX
    usart_gpios.GPIO_PinConfig.GPIO_PinNumber = GPIO_PIN_NO_2;
    GPIO_Init(&usart_gpios);

    //USART RX
    usart_gpios.GPIO_PinConfig.GPIO_PinNumber = GPIO_PIN_NO_3;
    GPIO_Init(&usart_gpios);
}

void USART2_init(void){

    usart2_handle.pUSARTx = USART2;
    usart2_handle.USART_Config.USART_Baud = USART_STD_BAUD_115200;
    usart2_handle.USART_Config.USART_HWFlowControl = USART_HW_FLOW_CTRL_NONE;
    usart2_handle.USART_Config.USART_Mode = USART_MODE_ONLY_TX;
    usart2_handle.USART_Config.USART_NoOfStopBits = USART_STOPBITS_2;
    usart2_handle.USART_Config.USART_WordLength = USART_WORDLEN_8BITS;
    usart2_handle.USART_Config.USART_ParityControl = USART_PARITY_DISABLE;

    USART_Init(&usart2_handle);
}

void usart_tx_testing(void){

    GPIO_OnBoardButtonInit();

    USART2_GPIOInit();

    USART2_init();

    USART_PeripheralControl(USART2, ENABLE);

    while(1){

        // Wait for button press
        while(!GPIO_ReadFromInputPin(GPIOA, GPIO_PIN_NO_0));

        // To avoid the de-bouncing
        delay();

        USART_SendData(&usart2_handle, (uint8_t *)msg, strlen(msg));
    }
}

#if 0

// Arduino code
void setup() {
  Serial.begin(115200);

  // Define the LED pin as Output
  pinMode (13, OUTPUT);

  Serial.println("Arduino UART Receiver");
  Serial.println("-----------------------------");

}


void loop() {

  digitalWrite(13, LOW);
  //wait until something is received
  while(! Serial.available());
  digitalWrite(13, HIGH);
  //read the data
  char in_read=Serial.read();
  //print the data
  Serial.print(in_read);

}
#endif


#endif //#if (FEATURE_SELECTED == STM32_USART_TX)
