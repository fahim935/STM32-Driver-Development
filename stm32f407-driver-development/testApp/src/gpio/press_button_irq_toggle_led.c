/*******************************************************************************
 * @file           press_button_irq_toggle_led.c
 * @brief          Brief description
 * 
 * @author         Fahim Akhtar
 * @date           18-Oct-2025
 * @time           8:00:22 PM
 * @version        1.0
 * @copyright      (c) 2025
 ******************************************************************************/
#include "press_button_irq_toggle_led.h"
#if (FEATURE_SELECTED == STM32_PRESS_BUTTON_IRQ_TOGGLE_LED)
#include "common.h"

void ext_button_irq_toggle_led (void){

    GPIO_Handle_t GpioLed, GpioBtn;

    memset(&GpioLed, 0x00, sizeof(GPIO_Handle_t));
    memset(&GpioBtn, 0x00, sizeof(GPIO_Handle_t));

    GpioLed.pGPIOx = GPIOD;
    GpioLed.GPIO_PinConfig.GPIO_PinNumber = GPIO_PIN_NO_12;
    GpioLed.GPIO_PinConfig.GPIO_PinMode = GPIO_MODE_OUT;
    GpioLed.GPIO_PinConfig.GPIO_PinSpeed = GPIO_SPEED_FAST;
    GpioLed.GPIO_PinConfig.GPIO_PinOPType = GPIO_OP_TYPE_PP;
    GpioLed.GPIO_PinConfig.GPIO_PinPuPdControl = GPIO_NO_PUPD;

    GPIO_PeriClockControl(GPIOD, ENABLE);
    GPIO_Init(&GpioLed);

    GpioBtn.pGPIOx = GPIOD;
    GpioBtn.GPIO_PinConfig.GPIO_PinNumber = GPIO_PIN_NO_5;
    GpioBtn.GPIO_PinConfig.GPIO_PinMode = GPIO_MODE_IT_FT;
    GpioBtn.GPIO_PinConfig.GPIO_PinSpeed = GPIO_SPEED_FAST;
    GpioBtn.GPIO_PinConfig.GPIO_PinPuPdControl = GPIO_PIN_PU;
    GPIO_PeriClockControl(GPIOD, ENABLE);
    GPIO_Init(&GpioBtn);


    GPIO_IRQPriorityConfig(IRQ_NO_EXTI9_5, NVIC_IRQ_PRI15);
    GPIO_IRQInterruptConfig(IRQ_NO_EXTI9_5, ENABLE);

}

void EXTI9_5_IRQHandler(void) {

    delay();
    GPIO_IRQHandling(GPIO_PIN_NO_5);
    GPIO_ToggleOutputPin(GPIOD, GPIO_PIN_NO_12);
}
#endif
