/*******************************************************************************
 * @file           i2c_mstr_tx.h
 * @brief          Brief description
 * 
 * @author         Fahim Akhtar
 * @date           11-Nov-2025
 * @time           7:53:24 AM
 * @version        1.0
 * @copyright      (c) 2025
 ******************************************************************************/

#ifndef INC_I2C_MSTR_TX_H_
#define INC_I2C_MSTR_TX_H_

#include "app_manager.h"
#if (FEATURE_SELECTED == STM32_I2C_MSTR_TX)
void i2c_master_tx_testing(void);
#endif

#endif /* INC_I2C_MSTR_TX_H_ */
