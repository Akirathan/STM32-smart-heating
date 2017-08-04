/*
 * temp_sensor_tests.cpp
 *
 *  Created on: 4. 8. 2017
 *      Author: lenovo
 */

#include "temp_sensor_tests.hpp"

namespace Tests {
namespace TempSensor {

void alarm_test()
{
	OneWire::init_communication();
	OneWire::write_byte(TEMP_SENSOR_CMD_ALARMSEARCH);

	uint8_t rom[8];
	for (int i=0; i<8; ++i) {
		rom[i] = OneWire::read_byte();
	}
}

void read_config()
{

}

bool set_alarm()
{
	TempSensor::set_alarm(10,20);
	TempSensor::set_resolution(TempSensor::RESOLUTION_10_BIT);

}

bool set_resolution()
{

}

bool temp_measure_test()
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

	return false;
}

} // namespace TempSensor
} // namespace Tests

