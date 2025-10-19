/*
 * common.h
 *
 *  Created on: Oct 8, 2025
 *      Author: Fahim Akhtar
 */

#ifndef INC_COMMON_H_
#define INC_COMMON_H_

#include <stdio.h>
#include <string.h>
#include <stdint.h>
#include "stm32f407xx.h"
#include "stm32f407xx_gpio_driver.h"

static inline void delay(void) {
    // just waste some cycles
    // Approx 200msec delay
    for(volatile int i = 0; i < 500000/2; i++);
}
#endif /* INC_COMMON_H_ */
