/**
 * @file one_wire.hpp
 * @author Pavel Marek
 * @date 26.12.2016
 * @brief This file contains functions for communication with one-wire device
 * 		  and defines for one-wire bus connection with GPIO.
 *
 * Those functions are implementation for one-wire protocol as described in
 * DS18B20 manual.
 *
 * Only one slave device is supported on one-wire bus.
 */

#ifndef ONE_WIRE_HPP_
#define ONE_WIRE_HPP_

#include "stm32f1xx.h"
#include "cube_main.h"

namespace OneWire {

uint32_t init();
int init_communication();
void write_byte(uint8_t byte);
uint8_t read_byte();
uint8_t read_bit();
void write_bit(uint8_t bit);
void reset();

} // namespace OneWire

#endif /* ONE_WIRE_HPP_ */
