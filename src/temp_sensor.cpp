/*
 * temp_sensor.c
 *
 *  Created on: 31 Jan 2017
 *      Author: Mayfa
 */

#include "temp_sensor.hpp"

namespace TempSensor {

// Private functions
static double convert_temperature(uint8_t lsb, uint8_t msb, resolution_t resolution);
static double convert_positive_temperature(uint8_t lsb, uint8_t msb, resolution_t resolution);
static resolution_t get_resolution(uint8_t cfg_byte);
static void write_scratchpad(config_t* config);

/**
 * Initializes one-wire peripheral.
 */
uint32_t init()
{
	return OneWire::init();
}

double measure_temperature()
{
	OneWire::init_communication();
	OneWire::write_byte(TEMP_SENSOR_CMD_SKIPROM);
	OneWire::write_byte(TEMP_SENSOR_CMD_CONVERTT);
	while (OneWire::read_bit() == 0) {
		// The conversion is still in progress.
		// Wait until the conversion is done.
	}

	// The conversion is now done.
	data_t data;
	read_data(&data);

	return convert_temperature(data.TEMP_LSB, data.TEMP_MSB, get_resolution(data.CFG));
}

/**
 * This function should be called after temperature measurement, otherwise
 * bad result could be returned.
 */
bool is_alarm_set()
{
	OneWire::init_communication();
	OneWire::write_byte(TEMP_SENSOR_CMD_ALARMSEARCH);

	// Search ROM algorithm implemented for just one device on the bus.
	for (int i = 0; i < 64; ++i) {
		// Slave should write bit of it's ROM and then complement bit.
		uint8_t byte = OneWire::read_bit();
		uint8_t byte_compl = OneWire::read_bit();
		if (!((byte == 1 && byte_compl == 0) || (byte == 0 && byte_compl == 1))) {
			return false;
		}
		// Master responds by sending the bit that slave has sent.
		OneWire::write_bit(byte);
	}

	return true;
}

/**
 * Read the scratchpad from the sensor and
 * fills the data structure.
 */
void read_data(data_t* data)
{
	OneWire::init_communication();
	OneWire::write_byte(TEMP_SENSOR_CMD_SKIPROM);
	OneWire::write_byte(TEMP_SENSOR_CMD_READSCRATCHPAD);

	data->TEMP_LSB = OneWire::read_byte();
	data->TEMP_MSB = OneWire::read_byte();
	data->TH = OneWire::read_byte();
	data->TL = OneWire::read_byte();
	data->CFG = OneWire::read_byte();

	// Ignore the rest
	OneWire::reset();
}

/**
 * Sets the TH and TL registers. When the sensor measures temperature that is
 * outside given bounds, the alarm flag is set.
 */
void set_alarm(int8_t low, int8_t high)
{
	config_t config;

	read_config(&config);
	config.TL = low;
	config.TH = high;

	write_scratchpad(&config);
}

/**
 * Gets resolution from CFG byte.
 */
static resolution_t get_resolution(uint8_t cfg_byte)
{
	if (READ_BIT(cfg_byte, TEMP_SENSOR_CFG_R0 | TEMP_SENSOR_CFG_R1) == 0) {
		return RESOLUTION_9_BIT;
	}
	else if (READ_BIT(cfg_byte, TEMP_SENSOR_CFG_R0 | TEMP_SENSOR_CFG_R1) ==
			(TEMP_SENSOR_CFG_R0 | TEMP_SENSOR_CFG_R1))
	{
		return RESOLUTION_12_BIT;
	}
	else if (READ_BIT(cfg_byte, TEMP_SENSOR_CFG_R0) == TEMP_SENSOR_CFG_R0) {
		return RESOLUTION_10_BIT;
	}
	else if (READ_BIT(cfg_byte, TEMP_SENSOR_CFG_R1) == TEMP_SENSOR_CFG_R1) {
		return RESOLUTION_11_BIT;
	}

	else {
		return RESOLUTION_9_BIT;
	}
}

/**
 * Set the given resolution. Note that the
 * higher the resolution is, more time for
 * the measurement is needed.
 *
 * @param resolution should be one of
 *   one_wire_resolution_t values
 */
void set_resolution(resolution_t resolution)
{
	config_t config;
	read_config(&config);

	switch (resolution) {
	case RESOLUTION_9_BIT:
		CLEAR_BIT(config.CFG, TEMP_SENSOR_CFG_R0 | TEMP_SENSOR_CFG_R1);
		break;
	case RESOLUTION_10_BIT:
		MODIFY_REG(config.CFG, TEMP_SENSOR_CFG_R1, TEMP_SENSOR_CFG_R0);
		break;
	case RESOLUTION_11_BIT:
		MODIFY_REG(config.CFG, TEMP_SENSOR_CFG_R0, TEMP_SENSOR_CFG_R1);
		break;
	case RESOLUTION_12_BIT:
		SET_BIT(config.CFG, TEMP_SENSOR_CFG_R0 | TEMP_SENSOR_CFG_R1);
		break;
	}

	write_scratchpad(&config);
}

/**
 * Copies the configuration values to EEPROM.
 */
void copy_scratchpad()
{
	OneWire::init_communication();
	OneWire::write_byte(TEMP_SENSOR_CMD_SKIPROM);
	OneWire::write_byte(TEMP_SENSOR_CMD_COPYSCRATCHPAD);
}

/**
 * Write the config data structure to the
 * temperature sensor.
 */
static void write_scratchpad(config_t* config)
{
	OneWire::init_communication();
	OneWire::write_byte(TEMP_SENSOR_CMD_SKIPROM);
	OneWire::write_byte(TEMP_SENSOR_CMD_WRITESCRATCHPAD);

	OneWire::write_byte(config->TH);
	OneWire::write_byte(config->TL);
	OneWire::write_byte(config->CFG);
}

/**
 * Read the scratchpad from the sensor and
 * sills the config structure.
 */
void read_config(config_t* config)
{
	OneWire::init_communication();
	OneWire::write_byte(TEMP_SENSOR_CMD_SKIPROM);
	OneWire::write_byte(TEMP_SENSOR_CMD_READSCRATCHPAD);

	OneWire::read_byte(); // temp_LSB
	OneWire::read_byte(); // temp_MSB
	config->TH = OneWire::read_byte();
	config->TL = OneWire::read_byte();
	config->CFG = OneWire::read_byte();

	// Ignore the rest
	//one_wire::reset();

	// Read rest.
	for (int i = 0; i < 4; i++) {
		OneWire::read_byte();
	}
}

/**
 * Get the temperature registers from the sensor and convert
 * them into double format. See doc for more info.
 */
static double convert_temperature(uint8_t lsb, uint8_t msb, resolution_t resolution)
{
	// Check if sign bit(s) is set
	if (msb >> 3) { // Sign bit is set
		uint16_t temp = msb;
		temp <<= 8;
		temp += lsb;

		temp ^= UINT16_MAX; // Complement
		temp += 1; // Add one

		return -convert_positive_temperature(temp, (temp >> 8), resolution);
	}
	else {
		return convert_positive_temperature(lsb, msb, resolution);
	}
}

static double convert_positive_temperature(uint8_t lsb, uint8_t msb, resolution_t resolution)
{
	// LSB
	int lower_bound = -4;
	int upper_bound = 3;

	switch (resolution) {
	case RESOLUTION_12_BIT:
		lower_bound = -4;
		break;
	case RESOLUTION_11_BIT:
		lower_bound = -3;
		lsb >>= 1;
		break;
	case RESOLUTION_10_BIT:
		lower_bound = -2;
		lsb >>= 2;
		break;
	case RESOLUTION_9_BIT:
		lower_bound = -1;
		lsb >>= 3;
		break;
	}

	double sum = 0;
	for (int i = lower_bound; i <= upper_bound; i++) {
		uint8_t last_bit = lsb & 0x01;
		if (last_bit)
			sum += pow(2, i);
		lsb >>= 1;
	}

	// MSB
	for (int i = 4; i <= 6; i++) {
		uint8_t last_bit = msb & 0x01;
		if (last_bit)
			sum += pow(2, i);
		msb >>= 1;
	}

	return sum;
}

} // namespace temp_sensor
