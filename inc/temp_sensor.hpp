/*
 * temp_sensor.hpp
 *
 *  Created on: 31 Jan 2017
 *      Author: Mayfa
 *
 * Use: First call temp_sensor_Init function that initializes the
 * underlying one-wire peripheral, then you can set alarm trigger
 * values with temp_sensor_SetAlarmHigh and temp_sensor_SetAlarmLow
 * functions and measurement resolution with temp_sensor_SetResolution
 * function. Note that the implicit resolution is 9 bit due to
 * documentation. Finally call temp_sensor_MeasureTemperature
 * function for temperature measurement.
 */

#ifndef TEMP_SENSOR_HPP_
#define TEMP_SENSOR_HPP_

//#include <cmath>
#include "stm32f1xx_hal.h"
#include "one_wire.hpp"

namespace TempSensor {


// Setting configuration register
#define SET_RES(cfg_byte, res)		(cfg_byte &= res)


typedef enum {
	RESOLUTION_9_BIT = 0x9F,
	RESOLUTION_10_BIT = 0xBF,
	RESOLUTION_11_BIT = 0xDF,
	RESOLUTION_12_BIT = 0xFF,
} resolution_t;

typedef struct {
	uint8_t TH; // temperature high trigger register
	uint8_t TL; // temperature low trigger register
	uint8_t CFG; // configuration register
} config_t;

/**
 * Doest not include CRC register.
 */
typedef struct {
	uint8_t TEMP_LSB;
	uint8_t TEMP_MSB;
	uint8_t TH;
	uint8_t TL;
	uint8_t CFG;
} data_t;

// ROM commands
const uint8_t CMD_SKIPROM = 0xCC;

// Memory commands
const uint8_t CMD_READSCRATCHPAD = 0xBE;
const uint8_t CMD_WRITESCRATCHPAD = 0x4E;
const uint8_t CMD_COPYSCRATCHPAD = 0x48;

// Functional commands
const uint8_t CMD_CONVERTT = 0x44;

uint32_t init();
void set_alarm_high(uint8_t temp_high);
void set_alarm_low(uint8_t temp_low);
void set_resolution(resolution_t resolution);
uint16_t measure_temperature();
void read_data(data_t* data);
void read_config(config_t* config);
void write_scratchpad(config_t* config);

void debug();

} // namespace TempSensor

#endif // TEMP_SENSOR_HPP_
