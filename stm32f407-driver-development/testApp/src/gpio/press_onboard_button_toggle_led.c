
/*******************************************************************************
 * @file           press_onboard_button_toggle_led.c
 * @brief          Understand how to enable the GPIO PORT & configure the PIN
 *                 for in/out
 * 
 * @author         Fahim Akhtar
 * @date           15-Oct-2025
 * @time           8:29:40 AM
 * @version        1.0
 * @copyright      (c) 2025
 ******************************************************************************/


#include <press_onboard_button_toggle_led.h>
#include "common.h"

/*
 * There are 4 LED & 1 User button available on STM32 Discovery board. Write a simple
 * example to take input from user button & toggle all the LED one by one.
 *
 * */
void onboard_button_press_toggle_led(void){

    GPIO_Handle_t GpioLed, GpioBtn;

    GpioLed.pGPIOx = GPIOD;
    GpioLed.GPIO_PinConfig.GPIO_PinNumber = GPIO_PIN_NO_12;
    GpioLed.GPIO_PinConfig.GPIO_PinMode = GPIO_MODE_OUT;
    GpioLed.GPIO_PinConfig.GPIO_PinSpeed = GPIO_SPEED_FAST;
#if 0 // Open Drain Configuration
    // As internal PULL UP resister is very high 40kohm
    // The intensity of the LED glow will be very low.
    GpioLed.GPIO_PinConfig.GPIO_PinOPType = GPIO_OP_TYPE_OD;
    GpioLed.GPIO_PinConfig.GPIO_PinPuPdControl = GPIO_PIN_PU;
#else // Push Pull COnfiguration
    GpioLed.GPIO_PinConfig.GPIO_PinOPType = GPIO_OP_TYPE_PP;
    GpioLed.GPIO_PinConfig.GPIO_PinPuPdControl = GPIO_PIN_PU;
#endif
    GPIO_PeriClockControl(GPIOD, ENABLE);
    GPIO_Init(&GpioLed);

    /*Keep the remain configuration same only change the pin no.*/
    GpioLed.GPIO_PinConfig.GPIO_PinNumber = GPIO_PIN_NO_13;
    GPIO_Init(&GpioLed);

    GpioLed.GPIO_PinConfig.GPIO_PinNumber = GPIO_PIN_NO_14;
    GPIO_Init(&GpioLed);

    GpioLed.GPIO_PinConfig.GPIO_PinNumber = GPIO_PIN_NO_15;
    GPIO_Init(&GpioLed);


    GpioBtn.pGPIOx = GPIOA;
    GpioBtn.GPIO_PinConfig.GPIO_PinNumber = GPIO_PIN_NO_0;
    GpioBtn.GPIO_PinConfig.GPIO_PinMode = GPIO_MODE_IN;
    GpioBtn.GPIO_PinConfig.GPIO_PinSpeed = GPIO_SPEED_FAST;
    GpioBtn.GPIO_PinConfig.GPIO_PinPuPdControl = GPIO_NO_PUPD;
    GPIO_PeriClockControl(GPIOA, ENABLE);
    GPIO_Init(&GpioBtn);

    while(1){ // Blink all the 4 on-board LED one by one when Button pressed

        if(GPIO_ReadFromInputPin(GPIOA, GPIO_PIN_NO_0)) {
            delay();
            GPIO_ToggleOutputPin(GPIOD, GPIO_PIN_NO_12);
            delay();
            GPIO_ToggleOutputPin(GPIOD, GPIO_PIN_NO_13);
            delay();
            GPIO_ToggleOutputPin(GPIOD, GPIO_PIN_NO_14);
            delay();
            GPIO_ToggleOutputPin(GPIOD, GPIO_PIN_NO_15);
            delay();
        }
    }
}
