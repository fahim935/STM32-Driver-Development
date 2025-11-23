/*******************************************************************************
 * @file           i2c_slv_irq_tx_rx.h
 * @brief          Brief description
 * 
 * @author         Fahim Akhtar
 * @date           23-Nov-2025
 * @time           11:52:06 AM
 * @version        1.0
 * @copyright      (c) 2025
 ******************************************************************************/

#ifndef INC_I2C_SLV_TX_RX_H_
#define INC_I2C_SLV_TX_RX_H_

#include "app_manager.h"
#if (FEATURE_SELECTED == STM32_I2C_SLV_IRQ_TX_RX)
void i2c_slv_irq_tx_rx_testing(void);
#endif

#endif /* INC_I2C_SLV_TX_RX_H_ */
