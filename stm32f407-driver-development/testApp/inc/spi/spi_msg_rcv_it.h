/*******************************************************************************
 * @file           spi_msg_rcv_it.h
 * @brief          Brief description
 * 
 * @author         Fahim Akhtar
 * @date           01-Nov-2025
 * @time           12:37:52 PM
 * @version        1.0
 * @copyright      (c) 2025
 ******************************************************************************/

#ifndef INC_SPI_SPI_MSG_RCV_IT_H_
#define INC_SPI_SPI_MSG_RCV_IT_H_

#include "app_manager.h"
#if (FEATURE_SELECTED == STM32_SPI2_READ_DATA_FROM_SLAVE)
void spi2_read_msg_from_slave(void);

#endif
#endif /* INC_SPI_SPI_MSG_RCV_IT_H__ */
