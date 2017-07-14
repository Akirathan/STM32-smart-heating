/*
 * temp_sensor.h
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

#ifndef TEMP_SENSOR_H_
#define TEMP_SENSOR_H_

#ifdef __cplusplus
	extern "C" {
#endif

#include <math.h>
#include <stdint.h>
#include <one_wire.hpp>

// PE4
#define TEMP_SENSOR_DATA_GPIOPIN				GPIO_PIN_4
#define TEMP_SENSOR_DATA_GPIOPORT				GPIOE

/* Setting configuration register */
#define TEMP_SENSOR_SET_RES(cfg_byte, res)		(cfg_byte &= res)


typedef enum {
	TEMP_SENSOR_RESOLUTION_9_BIT = 0x9F,
	TEMP_SENSOR_RESOLUTION_10_BIT = 0xBF,
	TEMP_SENSOR_RESOLUTION_11_BIT = 0xDF,
	TEMP_SENSOR_RESOLUTION_12_BIT = 0xFF,
} temp_sensor_resolution_t;

typedef struct {
	uint8_t TH; // temperature high trigger register
	uint8_t TL; // temperature low trigger register
	uint8_t CFG; // configuration register
} temp_sensor_config_t;

/**
 * Doest not include CRC register.
 */
typedef struct {
	uint8_t TEMP_LSB;
	uint8_t TEMP_MSB;
	uint8_t TH;
	uint8_t TL;
	uint8_t CFG;
} temp_sensor_data_t;

/* ROM commands */
#define TEMP_SENSOR_CMD_SKIPROM				0xCC

/* Memory commands */
#define TEMP_SENSOR_CMD_READSCRATCHPAD			0xBE
#define TEMP_SENSOR_CMD_WRITESCRATCHPAD		0x4E
#define TEMP_SENSOR_CMD_COPYSCRATCHPAD			0x48

/* Functional commands */
#define TEMP_SENSOR_CMD_CONVERTT				0x44


/* Public functions */
uint32_t temp_sensor_Init();
void temp_sensor_SetAlarmHigh(uint8_t temp_high);
void temp_sensor_SetAlarmLow(uint8_t temp_low);
void temp_sensor_SetResolution(temp_sensor_resolution_t resolution);
uint16_t temp_sensor_MeasureTemperature();
void temp_sensor_ReadData(temp_sensor_data_t* data);
void temp_sensor_ReadConfig(temp_sensor_config_t* config);
void temp_sensor_WriteScratchpad(temp_sensor_config_t* config);

void temp_sensor_debug();

#ifdef __cplusplus
	}
#endif

#endif // TEMP_SENSOR_H_
