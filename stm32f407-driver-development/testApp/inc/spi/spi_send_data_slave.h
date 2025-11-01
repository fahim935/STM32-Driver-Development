/*******************************************************************************
 * @file           send_data_slave.h
 * @brief          Brief description
 * 
 * @author         Fahim Akhtar
 * @date           26-Oct-2025
 * @time           7:13:00 PM
 * @version        1.0
 * @copyright      (c) 2025
 ******************************************************************************/

#ifndef INC_SPI_SPI_SEND_DATA_SLAVE_H_
#define INC_SPI_SPI_SEND_DATA_SLAVE_H_
#include "app_manager.h"
#if (FEATURE_SELECTED == STM32_SPI2_SEND_DATA_TO_SLAVE)
void spi2_send_data_to_slave(void);
#endif
#endif /* INC_SPI_SPI_SEND_DATA_SLAVE_H__ */
