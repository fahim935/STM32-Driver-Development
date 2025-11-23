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


/**
 * @brief Entry point for executing the selected STM32 driver demo.
 *
 * Based on the value of the `FEATURE_SELECTED` macro, this function calls the
 * corresponding feature implementation. If no valid feature is selected, it
 * prints a fallback message to indicate misconfiguration.
 *
 * This function is typically invoked from `main()` after system initialization.
 */

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
#elif (FEATURE_SELECTED == STM32_SPI2_SEND_DATA_WITH_NO_SLAVE)
    send_data_over_spi2();
#elif (FEATURE_SELECTED == STM32_SPI2_SEND_DATA_TO_SLAVE)
    spi2_send_data_to_slave();
#elif (FEATURE_SELECTED == STM32_SPI2_MASTER_SLAVE_COMM)
    spi2_communicate_with_slave();
#elif (FEATURE_SELECTED == STM32_SPI2_READ_DATA_FROM_SLAVE)
    spi2_read_msg_from_slave();
#elif (FEATURE_SELECTED == STM32_I2C_MSTR_TX)
    i2c_master_tx_testing();
#elif (FEATURE_SELECTED == STM32_I2C_MSTR_TX_RX)
    i2c_master_rx_testing();
#elif (FEATURE_SELECTED == STM32_I2C_MSTR_IRQ_TX_RX)
    i2c_master_interrupt_base_rx_testing();
#elif (FEATURE_SELECTED == STM32_I2C_SLV_IRQ_TX_RX)
    i2c_slv_irq_tx_rx_testing();
#elif (FEATURE_SELECTED == STM32_I2C_SLV_IRQ_TX_RX_LARGE_DATA)
    i2c_slv_irq_tx_rx_large_data_testing();
#else
    printf("STM32F407 : No valid feature selected\n");
#endif

}
