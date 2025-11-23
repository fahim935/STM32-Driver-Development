/*******************************************************************************
 * @file           i2c_mstr_tx_rx.h
 * @brief          Brief description
 * 
 * @author         Fahim Akhtar
 * @date           16-Nov-2025
 * @time           4:16:04 PM
 * @version        1.0
 * @copyright      (c) 2025
 ******************************************************************************/

#ifndef INC_I2C_MSTR_TX_RX_H_
#define INC_I2C_MSTR_TX_RX_H_

#include "app_manager.h"
#if (FEATURE_SELECTED == STM32_I2C_MSTR_TX_RX)
void i2c_master_rx_testing(void);
#endif

#endif /* INC_I2C_MSTR_TX_RX_H_ */
