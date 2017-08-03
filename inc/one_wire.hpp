/*
 * one_wire.hpp
 *
 *  Created on: 26. 12. 2016
 *      Author: Mayfa
 */

#ifndef ONE_WIRE_HPP_
#define ONE_WIRE_HPP_

#include "stm32f1xx.h"

namespace OneWire {

// PE4
#define TEMP_DATA_GPIOPIN		GPIO_PIN_4
#define TEMP_DATA_GPIOPORT		GPIOE

uint32_t init();
int init_communication();
void write_byte(uint8_t byte);
uint8_t read_byte();
void reset();
uint8_t read_bit();

// Testing functions.
uint32_t time_test(uint32_t microsecs);
void debug();

} // namespace OneWire

#endif /* ONE_WIRE_HPP_ */
