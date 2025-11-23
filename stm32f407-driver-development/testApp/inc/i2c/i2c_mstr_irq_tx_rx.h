/*******************************************************************************
 * @file           i2c_mstr_irq_tx_rx.h
 * @brief          Brief description
 * 
 * @author         Fahim Akhtar
 * @date           22-Nov-2025
 * @time           5:05:54 PM
 * @version        1.0
 * @copyright      (c) 2025
 ******************************************************************************/

#ifndef INC_I2C_MSTR_IRQ_TX_RX_H_
#define INC_I2C_MSTR_IRQ_TX_RX_H_

#include "app_manager.h"
#if (FEATURE_SELECTED == STM32_I2C_MSTR_IRQ_TX_RX)
void i2c_master_interrupt_base_rx_testing(void);
#endif

#endif /* INC_I2C_MSTR_IRQ_TX_RX_H_ */
