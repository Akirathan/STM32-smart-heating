/*
 * temp_sensor.c
 *
 *  Created on: 31 Jan 2017
 *      Author: Mayfa
 */

#include <temp_sensor.hpp>

/* Private functions */
static double convert_positive_temperature(uint8_t lsb, uint8_t msb, temp_sensor_resolution_t resolution);

/**
 * Initializes one-wire peripheral.
 */
uint32_t temp_sensor_Init() {
	return one_wire_Init();
}

uint16_t temp_sensor_MeasureTemperature() {
	uint16_t temp = 0x0000; // There is no need for initializing temp

	one_wire_InitCommunication();
	one_wire_WriteByte(TEMP_SENSOR_CMD_SKIPROM);

	one_wire_WriteByte(TEMP_SENSOR_CMD_CONVERTT);
	while (one_wire_ReadBit() == 0) {
		// The conversion is still in progress.
		// Wait until the conversion is done.
	}

	// The conversion is now done
	one_wire_InitCommunication();
	one_wire_WriteByte(TEMP_SENSOR_CMD_SKIPROM);
	one_wire_WriteByte(TEMP_SENSOR_CMD_READSCRATCHPAD);
	uint8_t temp_lsb = one_wire_ReadByte();
	uint8_t temp_msb = one_wire_ReadByte();

	// Reset the bus, because there is no
	// need for reading other scratchpad
	// bytes.
	one_wire_Reset();

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
void temp_sensor_ReadData(temp_sensor_data_t* data) {
	one_wire_InitCommunication();
	one_wire_WriteByte(TEMP_SENSOR_CMD_SKIPROM);
	one_wire_WriteByte(TEMP_SENSOR_CMD_READSCRATCHPAD);

	data->TEMP_LSB = one_wire_ReadByte();
	data->TEMP_MSB = one_wire_ReadByte();
	data->TH = one_wire_ReadByte();
	data->TL = one_wire_ReadByte();
	data->CFG = one_wire_ReadByte();

	// Ignore the rest
	one_wire_Reset();
}

/**
 * Set the TH register, so when the sensor
 * measures temperature that is higher than
 * this value, the alarm flag is set.
 */
void temp_sensor_SetAlarmHigh(uint8_t temp_high) {
	temp_sensor_config_t config;

	temp_sensor_ReadConfig(&config);
	config.TH = temp_high;

	temp_sensor_WriteScratchpad(&config);
}

/**
 * Set the TL register, so when the sensor
 * measures temperature that is lower than
 * this value, the alarm flag is set.
 */
void temp_sensor_SetAlarmLow(uint8_t temp_low) {
	temp_sensor_config_t config;

	temp_sensor_ReadConfig(&config);
	config.TL = temp_low;

	temp_sensor_WriteScratchpad(&config);
}

/**
 * Set the given resolution. Note that the
 * higher the resolution is, more time for
 * the measurement is needed.
 *
 * @param resolution should be one of
 *   one_wire_resolution_t values
 */
void temp_sensor_SetResolution(temp_sensor_resolution_t resolution) {
	temp_sensor_config_t config;

	temp_sensor_ReadConfig(&config);
	TEMP_SENSOR_SET_RES(config.CFG, resolution);

	temp_sensor_WriteScratchpad(&config);

	// Copy the configuration values to EEPROM
	one_wire_InitCommunication();
	one_wire_WriteByte(TEMP_SENSOR_CMD_SKIPROM);
	one_wire_WriteByte(TEMP_SENSOR_CMD_COPYSCRATCHPAD);
}

/**
 * Write the config data structure to the
 * temperature sensor.
 */
void temp_sensor_WriteScratchpad(temp_sensor_config_t* config) {
	one_wire_InitCommunication();
	one_wire_WriteByte(TEMP_SENSOR_CMD_SKIPROM);
	one_wire_WriteByte(TEMP_SENSOR_CMD_WRITESCRATCHPAD);

	one_wire_WriteByte(config->TH);
	one_wire_WriteByte(config->TL);
	//one_wire_WriteByte(config->CFG);
	one_wire_WriteByte(0x9F);
}

void temp_sensor_debug() {
	temp_sensor_config_t conf;
	conf.TH = 10;
	conf.TL = 20;
	conf.CFG = 0x9F;
	temp_sensor_WriteScratchpad(&conf);
	temp_sensor_ReadConfig(&conf);
	one_wire_Reset();

	HAL_Delay(2000);

	temp_sensor_ReadConfig(&conf);
}

/**
 * Read the scratchpad from the sensor and
 * sills the config structure.
 */
void temp_sensor_ReadConfig(temp_sensor_config_t* config) {
	one_wire_InitCommunication();
	one_wire_WriteByte(TEMP_SENSOR_CMD_SKIPROM);
	one_wire_WriteByte(TEMP_SENSOR_CMD_READSCRATCHPAD);

	one_wire_ReadByte(); // temp_LSB
	one_wire_ReadByte(); // temp_MSB
	config->TH = one_wire_ReadByte();
	config->TL = one_wire_ReadByte();
	config->CFG = one_wire_ReadByte();

	// Ignore the rest
	//one_wire_Reset();

	for (int i = 0; i < 4; i++) {
		one_wire_ReadByte();
	}
}

/**
 * Get the temperature registers from the sensor and convert
 * them into double format. See doc for more info.
 */
double temp_sensor_ConvertTemperature(uint8_t lsb, uint8_t msb, temp_sensor_resolution_t resolution) {
	// Check if sign bit(s) is set
	if (msb >> 3) {
		uint16_t temp = msb;
		temp <<= 8;
		temp += lsb;

		temp ^= UINT16_MAX; // Complement
		temp += 1; // Add one

		return - convert_positive_temperature(temp, (temp>>8), resolution);
	}
	else {
		return convert_positive_temperature(lsb, msb, resolution);
	}
}

static double convert_positive_temperature(uint8_t lsb, uint8_t msb, temp_sensor_resolution_t resolution) {
	/* LSB */
	int lower_bound = -4;
	int upper_bound = 3;

	switch (resolution) {
	case TEMP_SENSOR_RESOLUTION_12_BIT:
		lower_bound = -4;
		break;
	case TEMP_SENSOR_RESOLUTION_11_BIT:
			lower_bound = -3;
			break;
	case TEMP_SENSOR_RESOLUTION_10_BIT:
			lower_bound = -2;
			break;
	case TEMP_SENSOR_RESOLUTION_9_BIT:
			lower_bound = -1;
			break;
	}

	double sum = 0;
	for (int i=lower_bound; i<=upper_bound; i++) {
		uint8_t last_bit = lsb & 0x01;
		if (last_bit) sum += pow(2, i);
		lsb >>= 1;
	}

	/* MSB */
	for (int i=4; i<=6; i++) {
		uint8_t last_bit = msb & 0x01;
		if (last_bit) sum += pow(2, i);
		msb >>= 1;
	}

	return sum;
}
