/*
 * temp_sensor_tests.hpp
 *
 *  Created on: 4. 8. 2017
 *      Author: lenovo
 */

#ifndef TESTS_TEMP_SENSOR_TESTS_HPP_
#define TESTS_TEMP_SENSOR_TESTS_HPP_

#include "temp_sensor.hpp"

namespace TempSensorTests {

bool run_all_tests();
bool alarm_test_negative();
bool alarm_test_positive();
void read_config();
bool set_alarm();
bool set_resolution();
bool temp_measure();

} // namespace TempSensorTests

#endif /* TESTS_TEMP_SENSOR_TESTS_HPP_ */
