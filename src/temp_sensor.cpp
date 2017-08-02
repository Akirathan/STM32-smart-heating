/*
 * temp_sensor.c
 *
 *  Created on: 31 Jan 2017
 *      Author: Mayfa
 */

#include "temp_sensor.hpp"

namespace TempSensor {

// Private functions
static double convert_positive_temperature(uint8_t lsb, uint8_t msb, resolution_t resolution);

/**
 * Initializes one-wire peripheral.
 */
uint32_t init()
{
	return OneWire::init();
}

uint16_t measure_temperature()
{
	uint16_t temp = 0x0000; // There is no need for initializing temp

	OneWire::init_communication();
	OneWire::write_byte(CMD_SKIPROM);

	OneWire::write_byte(CMD_CONVERTT);
	while (OneWire::read_bit() == 0) {
		// The conversion is still in progress.
		// Wait until the conversion is done.
	}

	// The conversion is now done
	OneWire::init_communication();
	OneWire::write_byte(CMD_SKIPROM);
	OneWire::write_byte(CMD_READSCRATCHPAD);
	uint8_t temp_lsb = OneWire::read_byte();
	uint8_t temp_msb = OneWire::read_byte();

	// Reset the bus, because there is no
	// need for reading other scratchpad
	// bytes.
	OneWire::reset();

	// temp = 0x0000, temp_msb = 0x002A
	temp |= temp_msb; // 0x002A
	// Shift the MS byte
	temp <<= 8; //0x002A --> 0x2A00
	temp |= temp_lsb;

	// TODO convert temperature

	return temp;
}

/**
 * Read the scratchpad from the sensor and
 * fills the data structure.
 */
void read_data(data_t* data)
{
	OneWire::init_communication();
	OneWire::write_byte(CMD_SKIPROM);
	OneWire::write_byte(CMD_READSCRATCHPAD);

	data->TEMP_LSB = OneWire::read_byte();
	data->TEMP_MSB = OneWire::read_byte();
	data->TH = OneWire::read_byte();
	data->TL = OneWire::read_byte();
	data->CFG = OneWire::read_byte();

	// Ignore the rest
	OneWire::reset();
}

/**
 * Set the TH register, so when the sensor
 * measures temperature that is higher than
 * this value, the alarm flag is set.
 */
void set_alarm_high(uint8_t temp_high)
{
	config_t config;

	read_config(&config);
	config.TH = temp_high;

	write_scratchpad(&config);
}

/**
 * Set the TL register, so when the sensor
 * measures temperature that is lower than
 * this value, the alarm flag is set.
 */
void set_alarm_low(uint8_t temp_low)
{
	config_t config;

	read_config(&config);
	config.TL = temp_low;

	write_scratchpad(&config);
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
	SET_RES(config.CFG, resolution);

	write_scratchpad(&config);

	// Copy the configuration values to EEPROM
	OneWire::init_communication();
	OneWire::write_byte(CMD_SKIPROM);
	OneWire::write_byte(CMD_COPYSCRATCHPAD);
}

/**
 * Write the config data structure to the
 * temperature sensor.
 */
void write_scratchpad(config_t* config)
{
	OneWire::init_communication();
	OneWire::write_byte(CMD_SKIPROM);
	OneWire::write_byte(CMD_WRITESCRATCHPAD);

	OneWire::write_byte(config->TH);
	OneWire::write_byte(config->TL);
	OneWire::write_byte(config->CFG);
}

void debug()
{
	config_t conf;
	conf.TH = 10;
	conf.TL = 20;
	conf.CFG = 0x9F;
	write_scratchpad(&conf);
	read_config(&conf);
	OneWire::reset();

	HAL_Delay(2000);

	read_config(&conf);
}

/**
 * Read the scratchpad from the sensor and
 * sills the config structure.
 */
void read_config(config_t* config)
{
	OneWire::init_communication();
	OneWire::write_byte(CMD_SKIPROM);
	OneWire::write_byte(CMD_READSCRATCHPAD);

	OneWire::read_byte(); // temp_LSB
	OneWire::read_byte(); // temp_MSB
	config->TH = OneWire::read_byte();
	config->TL = OneWire::read_byte();
	config->CFG = OneWire::read_byte();

	// Ignore the rest
	//one_wire::reset();

	for (int i = 0; i < 4; i++) {
		OneWire::read_byte();
	}
}

/**
 * Get the temperature registers from the sensor and convert
 * them into double format. See doc for more info.
 */
double convert_temperature(uint8_t lsb, uint8_t msb, resolution_t resolution)
{
	// Check if sign bit(s) is set
	if (msb >> 3) {
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

// FIXME
static double convert_positive_temperature(uint8_t lsb, uint8_t msb, resolution_t resolution)
{
	/* LSB */
	int lower_bound = -4;
	int upper_bound = 3;

	switch (resolution) {
	case RESOLUTION_12_BIT:
		lower_bound = -4;
		break;
	case RESOLUTION_11_BIT:
		lower_bound = -3;
		break;
	case RESOLUTION_10_BIT:
		lower_bound = -2;
		break;
	case RESOLUTION_9_BIT:
		lower_bound = -1;
		break;
	}

	double sum = 0;
	for (int i = lower_bound; i <= upper_bound; i++) {
		uint8_t last_bit = lsb & 0x01;
		if (last_bit)
			//sum += pow(2, i);
		lsb >>= 1;
	}

	// MSB
	for (int i = 4; i <= 6; i++) {
		uint8_t last_bit = msb & 0x01;
		if (last_bit)
			//sum += pow(2, i);
		msb >>= 1;
	}

	return sum;
}

} // namespace temp_sensor
