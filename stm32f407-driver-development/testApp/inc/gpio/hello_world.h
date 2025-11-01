/*
 * hello_world.h
 *
 *  Created on: Oct 8, 2025
 *      Author: Fahim Akhtar
 */

#ifndef INC_HELLO_WORLD_H_
#define INC_HELLO_WORLD_H_
#include "app_manager.h"
#if(FEATURE_SELECTED == STM32_PRINT_HELLO_WORLD)
void print_hello_world();
#endif

#endif /* INC_HELLO_WORLD_H_ */
