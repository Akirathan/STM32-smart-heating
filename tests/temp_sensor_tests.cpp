/*
 * temp_sensor_tests.cpp
 *
 *  Created on: 4. 8. 2017
 *      Author: lenovo
 */

#include "temp_sensor_tests.hpp"

namespace TempSensorTests {

/**
 * Run all tests from this test namespace.
 */
bool run_all_tests()
{
	if (!alarm_test_negative()) {
		return false;
	}

	if (!alarm_test_positive()) {
		return false;
	}

	if (!set_alarm()) {
		return false;
	}

	if (!set_resolution()) {
		return false;
	}

	// All tests passed.
	return true;
}

/**
 * Alarm should not be triggered.
 */
bool alarm_test_negative()
{
	TempSensor::set_alarm(-5, 40);
	TempSensor::measure_temperature();
	if (TempSensor::is_alarm_set()) {
		return false;
	}
	else {
		return true;
	}
}

/**
 * Alarm should be triggered.
 */
bool alarm_test_positive()
{
	TempSensor::set_alarm(-90, -5);
	TempSensor::measure_temperature();
	if (TempSensor::is_alarm_set()) {
		return true;
	}
	else {
		return false;
	}
}

void read_config()
{
	TempSensor::config_t conf;
	TempSensor::read_config(&conf);
}

bool set_alarm()
{
	int8_t low = 10;
	int8_t high = 12;
	TempSensor::set_alarm(low, high);

	TempSensor::config_t conf;
	TempSensor::read_config(&conf);

	if (conf.TL != low || conf.TH != high) {
		return false;
	}
	else {
		return true;
	}
}

bool set_resolution()
{
	TempSensor::set_resolution(TempSensor::RESOLUTION_11_BIT);

	TempSensor::config_t conf;
	TempSensor::read_config(&conf);

	// TODO ...
	return true;
}

bool temp_measure()
{
	/*uint8_t temp_1_msb = 0x07; // 125
	uint8_t temp_1_lsb = 0xd0;

	uint8_t temp_3_msb = 0x01; // 25.0625
	uint8_t temp_3_lsb = 0x91;

	uint8_t temp_7_msb = 0xFF; // -0.5
	uint8_t temp_7_lsb = 0xF8;

	uint8_t temp_4_msb = 0xFC; // -55
	uint8_t temp_4_lsb = 0x90;

	double temp_1 = convert_temperature(temp_1_lsb, temp_1_msb, RESOLUTION_12_BIT);
	double temp_3 = convert_temperature(temp_3_lsb, temp_3_msb, RESOLUTION_12_BIT);
	double temp_7 = convert_temperature(temp_7_lsb, temp_7_msb, RESOLUTION_12_BIT);
	double temp_4 = convert_temperature(temp_4_lsb, temp_4_msb, RESOLUTION_12_BIT);*/

	return true;
}

} // namespace TempSensorTests

