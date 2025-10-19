/*******************************************************************************
 * @file           app_manager.c
 * @brief          Implements the application manager responsible for executing
 *                 the selected feature demo from the available STM32 driver
 *                 test applications.
 * 
 * @author         Fahim Akhtar
 * @date           17-Oct-2025
 * @time           11:42:25 AM
 * @version        1.0
 * @copyright      (c) 2025
 ******************************************************************************/

#include "app_manager.h"

void AppManager_Run(void)
{

#if(FEATURE_SELECTED == STM32_PRINT_HELLO_WORLD)
    print_hello_world();
#elif (FEATURE_SELECTED == STM32_COPY_DATA_FROM_FLASH_TO_SRAM)
    copy_data_flash_to_sram();
#elif (FEATURE_SELECTED == STM32_TOGGLE_ON_BOARD_LED)
    toggle_onboard_led();
#elif (FEATURE_SELECTED == STM32_PRESS_BUTTON_TO_ON_LED)
    onboard_button_press_toggle_led();
#elif (FEATURE_SELECTED == STM32_PRESS_EXTN_BUTTON_TO_ON_ENT_LED)
    ext_button_press_toggle_led();
#elif (FEATURE_SELECTED == STM32_PRESS_BUTTON_IRQ_TOGGLE_LED)
    ext_button_irq_toggle_led();
#else
    printf("STM32F407 : No valid feature selected\n");
#endif

}
