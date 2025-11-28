/*******************************************************************************
 * @file           usart_tx.h
 * @brief          Brief description
 * 
 * @author         Fahim Akhtar
 * @date           27-Nov-2025
 * @time           9:15:27 AM
 * @version        1.0
 * @copyright      (c) 2025
 ******************************************************************************/

#ifndef INC_USART_USART_TX_H_
#define INC_USART_USART_TX_H_

#include "app_manager.h"
#if (FEATURE_SELECTED == STM32_USART_TX)
void usart_tx_testing(void);
#endif

#endif /* INC_USART_USART_TX_H__ */
