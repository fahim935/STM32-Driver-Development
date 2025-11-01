/*******************************************************************************
 * @file           spi_master_slave_comm.h
 * @brief          Brief description
 * 
 * @author         Fahim Akhtar
 * @date           28-Oct-2025
 * @time           10:51:43 AM
 * @version        1.0
 * @copyright      (c) 2025
 ******************************************************************************/

#ifndef INC_SPI_SPI_MASTER_SLAVE_COMM_H_
#define INC_SPI_SPI_MASTER_SLAVE_COMM_H_
#include "app_manager.h"
#if (FEATURE_SELECTED == STM32_SPI2_MASTER_SLAVE_COMM)
void spi2_communicate_with_slave(void);
#endif

#endif /* INC_SPI_SPI_MASTER_SLAVE_COMM_H__ */
