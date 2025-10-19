/*******************************************************************************
 * @file           press_external_button_toggle_led.c
 * @brief          Example to handle the GPIO as input & output using LED & Push
 *                 button
 * 
 * @author         Fahim Akhtar
 * @date           18-Oct-2025
 * @time           7:26:12 AM
 * @version        1.0
 * @copyright      (c) 2025
 ******************************************************************************/

#include <press_external_button_toggle_led.h>
#include "common.h"

/*
 * Write a example program to toggle the external LED connected to PA8
 * when ever Button is pressed ( connected to PB12 )
 *
 * */

void ext_button_press_toggle_led(void){

    GPIO_Handle_t GpioLed, GpioBtn;

    GpioLed.pGPIOx = GPIOD;
    GpioLed.GPIO_PinConfig.GPIO_PinNumber = GPIO_PIN_NO_5;
    GpioLed.GPIO_PinConfig.GPIO_PinMode = GPIO_MODE_OUT;
    GpioLed.GPIO_PinConfig.GPIO_PinSpeed = GPIO_SPEED_FAST;
    GpioLed.GPIO_PinConfig.GPIO_PinOPType = GPIO_OP_TYPE_PP;
    GpioLed.GPIO_PinConfig.GPIO_PinPuPdControl = GPIO_NO_PUPD;

    GPIO_PeriClockControl(GPIOD, ENABLE);
    GPIO_Init(&GpioLed);

    GpioBtn.pGPIOx = GPIOB;
    GpioBtn.GPIO_PinConfig.GPIO_PinNumber = GPIO_PIN_NO_12;
    GpioBtn.GPIO_PinConfig.GPIO_PinMode = GPIO_MODE_IN;
    GpioBtn.GPIO_PinConfig.GPIO_PinSpeed = GPIO_SPEED_FAST;
    GpioBtn.GPIO_PinConfig.GPIO_PinPuPdControl = GPIO_PIN_PU;
    GPIO_PeriClockControl(GPIOB, ENABLE);
    GPIO_Init(&GpioBtn);

    while(1){

        if(GPIO_ReadFromInputPin(GPIOB, GPIO_PIN_NO_12) == DISABLE) {
            delay();
            GPIO_ToggleOutputPin(GPIOA, GPIO_PIN_NO_8);
        }
    }

}

