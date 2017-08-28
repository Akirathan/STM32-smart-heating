/**
 * @file relay.hpp
 * @author Pavel Marek
 * @date 31.6.2017
 * @brief This file contains defines and a class for controlling the relay
 * 		  module.
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

/**
 * @brief Class for controlling relay module.
 * @note Implemented as a singleton class.
 */
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
