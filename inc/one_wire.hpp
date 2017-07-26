/*
 * one_wire.hpp
 *
 *  Created on: 26. 12. 2016
 *      Author: Mayfa
 */

#ifndef ONE_WIRE_HPP_
#define ONE_WIRE_HPP_

#include "stm32f1xx_hal.h"
#include "temp_sensor.hpp" //DEBUG

namespace one_wire {

uint32_t init();
int init_communication();
void write_byte(uint8_t byte);
uint8_t read_byte();
uint8_t read_bit();
void reset();

/* Testing functions */
uint32_t time_test(uint32_t microsecs);
void debug();

} // namespace one_wire

#endif /* ONE_WIRE_HPP_ */
