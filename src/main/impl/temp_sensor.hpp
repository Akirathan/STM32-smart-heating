/**
 * @file temp_sensor.hpp
 * @author Pavel Marek
 * @date 31.6.2017
 *
 * @brief This file contains macros, structures and functions necessary for
 * 		  communication with DS18B20 temperature sensor.
 *
 * Use: First call @ref TempSensor::init function to initialize the underlying
 * one-wire peripheral, then set alarm trigger values with @ref TempSensor::set_alarm
 * function and measurement resolution with @ref TempSensor::set_resolution
 * function. Note that the implicit resolution is 9 bit as meantioned in documentation.
 * Finally call @ref TempSensor::measure_temperature for temperature measurement.
 *
 * @note This module is an implementation for functionality described in
 * 		 DS18B20 documentation.
 */

#ifndef TEMP_SENSOR_HPP_
#define TEMP_SENSOR_HPP_

#include <cmath>
#include "stm32f1xx_hal.h"
#include "one_wire.hpp"

namespace TempSensor {

// CFG register defines.
#define TEMP_SENSOR_CFG_R0		0x20
#define TEMP_SENSOR_CFG_R1		0x40

typedef enum {
	RESOLUTION_9_BIT = 0x9F,
	RESOLUTION_10_BIT = 0xBF,
	RESOLUTION_11_BIT = 0xDF,
	RESOLUTION_12_BIT = 0xFF,
} resolution_t;

/**
 * @brief Structure containing configurable registers of DS18B20 device.
 *
 * This structure is used whenever writing to sensor is performed.
 */
typedef struct {
	int8_t TH; // temperature high trigger register
	int8_t TL; // temperature low trigger register
	uint8_t CFG; // configuration register
} config_t;

/**
 * @brief Structure containing all the registers in DS18B20 device.
 *
 * This structure is used whenever reading from the sensor is performed.
 *
 * @note Does not include CRC register.
 */
typedef struct {
	uint8_t TEMP_LSB;
	uint8_t TEMP_MSB;
	uint8_t TH;
	uint8_t TL;
	uint8_t CFG;
} data_t;

// Command defines.
#define TEMP_SENSOR_CMD_SKIPROM					0xCC
#define TEMP_SENSOR_CMD_READSCRATCHPAD			0xBE
#define TEMP_SENSOR_CMD_WRITESCRATCHPAD			0x4E
#define TEMP_SENSOR_CMD_COPYSCRATCHPAD			0x48
#define TEMP_SENSOR_CMD_CONVERTT				0x44
#define TEMP_SENSOR_CMD_ALARMSEARCH				0xEC

uint32_t init();
void set_alarm(int8_t low, int8_t high);
void set_resolution(resolution_t resolution);
double measure_temperature();
bool is_alarm_set();
void read_data(data_t* data);
void read_config(config_t* config);
void copy_scratchpad();

} // namespace TempSensor

#endif // TEMP_SENSOR_HPP_
