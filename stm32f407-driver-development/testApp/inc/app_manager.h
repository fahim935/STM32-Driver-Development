/*******************************************************************************
 * @file           app_manager.h
 * @brief          Application feature manager for STM32 driver demos
 * 
 * @author         Fahim Akhtar
 * @date           17-Oct-2025
 * @time           11:41:10 AM
 * @version        1.0
 * @copyright      (c) 2025
 ******************************************************************************/

/*
 * This header defines the interface and feature selection mechanism for running
 * various demo applications on STM32 microcontrollers. Each feature corresponds
 * to a specific functional test case—such as toggling LEDs, handling button input,
 * or performing SPI communication—designed to validate low-level driver behavior.
 *
 * The macro `FEATURE_SELECTED` determines which demo is executed at runtime,
 * enabling modular testing without altering core logic. This approach supports
 * rapid prototyping, debugging, and proof-of-functionality work-flows.
 *
 * */

#ifndef INC_APP_MANAGER_H_
#define INC_APP_MANAGER_H_

// Include headers for individual feature modules
#include <copy_data_flash_to_sram.h>
#include <hello_world.h>
#include <onboard_led_toggle.h>
#include <press_external_button_toggle_led.h>
#include <press_onboard_button_toggle_led.h>
#include <spi_send_data_no_slave.h>
#include <spi_send_data_slave.h>
#include "project_configuration.h"
#include "common.h"
#include "press_button_irq_toggle_led.h"
#include "spi_master_slave_comm.h"
#include "spi_msg_rcv_it.h"
#include "i2c_mstr_tx.h"
#include "i2c_mstr_tx_rx.h"
#include "i2c_mstr_irq_tx_rx.h"
#include "i2c_slv_irq_tx_rx.h"
#include "i2c_slv_irq_tx_rx_large_data.h"
#include "usart_tx.h"
#include "usart_tx_rx.h"


// Basic feature demos
#define STM32_PRINT_HELLO_WORLD                 1   /*!< Executes the Hello World example */
#define STM32_COPY_DATA_FROM_FLASH_TO_SRAM      2   /*!< Demonstrates copying data from Flash to SRAM */
#define STM32_TOGGLE_ON_BOARD_LED               3   /*!< Toggles the on-board LED continuously */

// Button-based LED control
#define STM32_PRESS_BUTTON_TO_ON_LED            4   /*!< Turns on LED when on-board button is pressed */
#define STM32_PRESS_EXTN_BUTTON_TO_ON_ENT_LED   5   /*!< Turns on LED when on-board button is pressed */
#define STM32_PRESS_BUTTON_IRQ_TOGGLE_LED       6   /*!< Turns on LED when on-board button is pressed */

// SPI communication demos
#define STM32_SPI2_SEND_DATA_WITH_NO_SLAVE      7   /*!< Sends SPI data without slave connection */
#define STM32_SPI2_SEND_DATA_TO_SLAVE           8   /*!< Sends SPI data to slave device */
#define STM32_SPI2_MASTER_SLAVE_COMM            9   /*!< Full-duplex SPI master-slave communication */
#define STM32_SPI2_READ_DATA_FROM_SLAVE         10  /*!< Reads SPI data from slave when interrupt triggered by slave */

// I2C communication demos
#define STM32_I2C_MSTR_TX                       11
#define STM32_I2C_MSTR_TX_RX                    12
#define STM32_I2C_MSTR_IRQ_TX_RX                13

#define STM32_I2C_SLV_IRQ_TX_RX                 14
#define STM32_I2C_SLV_IRQ_TX_RX_LARGE_DATA      15

// USART communication demos
#define STM32_USART_TX                          16
#define STM32_USART_TX_RX                       17

// Select the active feature to run
#define FEATURE_SELECTED        STM32_USART_TX_RX



/**
 * @brief Executes the selected application feature based on FEATURE_SELECTED macro.
 *
 * This function acts as the entry point for running one of the predefined demo
 * applications. It uses the value of `FEATURE_SELECTED` to dispatch control to
 * the corresponding feature implementation. This modular design simplifies testing
 * and validation of individual drivers and application logic.
 *
 * Typical use case:
 * - Set FEATURE_SELECTED to desired macro (e.g., STM32_SPI2_MASTER_SLAVE_COMM)
 * - Compile and flash the firmware
 * - Observe behavior via serial output, LED toggling, or logic analyzer
 */
void AppManager_Run(void);

#endif /* INC_APP_MANAGER_H__ */
