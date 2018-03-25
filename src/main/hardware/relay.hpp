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
#include "main.h" // cube main

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
	Relay() = default;
};

#endif /* RELAY_H_ */
