/*******************************************************************************
 * @file           press_external_button_toggle_led.h
 * @brief          Brief description
 * 
 * @author         Fahim Akhtar
 * @date           18-Oct-2025
 * @time           7:26:29 AM
 * @version        1.0
 * @copyright      (c) 2025
 ******************************************************************************/

#ifndef INC_PRESS_EXTERNAL_BUTTON_TOGGLE_LED_H_
#define INC_PRESS_EXTERNAL_BUTTON_TOGGLE_LED_H_
#include "app_manager.h"
#if (FEATURE_SELECTED == STM32_PRESS_EXTN_BUTTON_TO_ON_ENT_LED)
void ext_button_press_toggle_led(void);
#endif

#endif /* INC_PRESS_EXTERNAL_BUTTON_TOGGLE_LED_H__ */
