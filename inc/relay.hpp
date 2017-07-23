/*
 * relay.h
 *
 *  Created on: 31 Jan 2017
 *      Author: Mayfa
 */

#ifndef RELAY_H_
#define RELAY_H_

#include "stm32f1xx_hal.h"

#define RELAY_DATA_GPIOPIN		GPIO_PIN_12
#define RELAY_DATA_GPIOPORT 	GPIOD

#define RELAY_POWER_GPIOPIN		GPIO_PIN_15
#define RELAY_POWER_GPIOPORT	GPIOD


void relay_Init();
void relay_SwitchOn();
void relay_SwitchOff();

#endif /* RELAY_H_ */
