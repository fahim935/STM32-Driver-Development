/*******************************************************************************
 * @file           spi_data_Send_test.h
 * @brief          Brief description
 * 
 * @author         Fahim Akhtar
 * @date           26-Oct-2025
 * @time           11:45:50 AM
 * @version        1.0
 * @copyright      (c) 2025
 ******************************************************************************/

#ifndef INC_SPI_SEND_DATA_NO_SLAVE_H_
#define INC_SPI_SEND_DATA_NO_SLAVE_H_
#include "app_manager.h"
#if (FEATURE_SELECTED == STM32_SPI2_SEND_DATA_WITH_NO_SLAVE)
void send_data_over_spi2(void);
#endif

#endif /* INC_SPI_SEND_DATA_NO_SLAVE_H__ */
