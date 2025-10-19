/*******************************************************************************
 * @file           app_manager.h
 * @brief          Brief description
 * 
 * @author         Fahim Akhtar
 * @date           17-Oct-2025
 * @time           11:41:10 AM
 * @version        1.0
 * @copyright      (c) 2025
 ******************************************************************************/

#ifndef INC_APP_MANAGER_H_
#define INC_APP_MANAGER_H_

#include <copy_data_flash_to_sram.h>
#include <hello_world.h>
#include <onboard_led_toggle.h>
#include <press_external_button_toggle_led.h>
#include <press_onboard_button_toggle_led.h>
#include "project_configuration.h"
#include "common.h"
#include "press_button_irq_toggle_led.h"

/**
 * @enum FeatureList_t
 * @brief Defines unique identifiers for each application feature in the project.
 *
 * These macro lists all the available test applications or feature modules
 * that can be executed in the STM32 driver development project. Each macro
 * corresponds to a specific functional demo or test scenario (for example,
 * printing a message, toggling an LED, or copying data from Flash to SRAM).
 *
 * The currently active feature is selected at compile time using the macro
 * `FEATURE_SELECTED`. This allows easy switching between multiple applications
 * without modifying the main code logic or project structure.
 *
 * Example:
 * @code
 * #define FEATURE_SELECTED STM32_PRESS_BUTTON_TO_ON_LED
 * @endcode
 *
 * The `AppManager_Run()` function uses this enumeration to determine which
 * feature’s corresponding function should be called during runtime.
 */


#define STM32_PRINT_HELLO_WORLD                 0x01   /*!< Executes the Hello World example */
#define STM32_COPY_DATA_FROM_FLASH_TO_SRAM      0x02   /*!< Demonstrates copying data from Flash to SRAM */
#define STM32_TOGGLE_ON_BOARD_LED               0x03   /*!< Toggles the on-board LED continuously */
#define STM32_PRESS_BUTTON_TO_ON_LED            0x04   /*!< Turns on LED when on-board button is pressed */
#define STM32_PRESS_EXTN_BUTTON_TO_ON_ENT_LED   0x05   /*!< Turns on LED when on-board button is pressed */
#define STM32_PRESS_BUTTON_IRQ_TOGGLE_LED       0x06   /*!< Turns on LED when on-board button is pressed */

#define FEATURE_SELECTED        STM32_PRESS_BUTTON_IRQ_TOGGLE_LED




void AppManager_Run(void);

#endif /* INC_APP_MANAGER_H__ */
