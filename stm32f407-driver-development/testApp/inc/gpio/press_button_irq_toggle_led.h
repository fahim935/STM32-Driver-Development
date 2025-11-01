/*******************************************************************************
 * @file           press_button_irq_toggle_led.h
 * @brief          Brief description
 * 
 * @author         Fahim Akhtar
 * @date           18-Oct-2025
 * @time           8:00:07 PM
 * @version        1.0
 * @copyright      (c) 2025
 ******************************************************************************/

#ifndef INC_PRESS_BUTTON_IRQ_TOGGLE_LED_H_
#define INC_PRESS_BUTTON_IRQ_TOGGLE_LED_H_

#include "app_manager.h"
#if (FEATURE_SELECTED == STM32_PRESS_BUTTON_IRQ_TOGGLE_LED)
void ext_button_irq_toggle_led(void);
#endif

#endif /* INC_PRESS_BUTTON_IRQ_TOGGLE_LED_H__ */
