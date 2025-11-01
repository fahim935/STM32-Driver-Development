/*******************************************************************************
 * @file           press_onboard_button_toggle_led.h
 * @brief          Brief description
 * 
 * @author         Fahim Akhtar
 * @date           15-Oct-2025
 * @time           9:07:51 AM
 * @version        1.0
 * @copyright      (c) 2025
 ******************************************************************************/

#ifndef INC_PRESS_ONBOARD_BUTTON_TOGGLE_LED_H_
#define INC_PRESS_ONBOARD_BUTTON_TOGGLE_LED_H_
#include "app_manager.h"
#if (FEATURE_SELECTED == STM32_PRESS_BUTTON_TO_ON_LED)
void onboard_button_press_toggle_led(void);
#endif

#endif /* INC_PRESS_ONBOARD_BUTTON_TOGGLE_LED_H_ */
