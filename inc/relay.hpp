/*
 * relay.h
 *
 *  Created on: 31 Jan 2017
 *      Author: Mayfa
 */

#ifndef RELAY_H_
#define RELAY_H_

#include "stm32f1xx_hal.h"

// PD12
#define RELAY_DATA_GPIOPIN		GPIO_PIN_12
#define RELAY_DATA_GPIOPORT 	GPIOD

// PD15
#define RELAY_POWER_GPIOPIN		GPIO_PIN_15
#define RELAY_POWER_GPIOPORT	GPIOD

class Relay {
public:
	static Relay& getInstance();
	Relay(const Relay&) = delete;
	void operator=(const Relay&) = delete;
	void switchOn();
	void switchOff();
private:
	Relay();
};

#endif /* RELAY_H_ */
