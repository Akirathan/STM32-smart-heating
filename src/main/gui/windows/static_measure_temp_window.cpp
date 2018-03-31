/**
 * @file static_measure_temp_window.cpp
 * @author Pavel Marek
 * @date 9.8.2017
 */

#include "static_measure_temp_window.hpp"
#include "rt_assert.h"

StaticMeasureTempWindow::StaticMeasureTempWindow(const Coord& c)
	: StaticTempWindow(c)
{ }

/**
 * @brief Unregisters minute callback from @ref RTCController.
 */
StaticMeasureTempWindow::~StaticMeasureTempWindow()
{
	if (registeredCallback) {
		RTCController::getInstance().unregisterMinuteCallback(this);
	}
}

/**
 * @brief Registers for minute callback to @ref RTCController.
 */
void StaticMeasureTempWindow::measure()
{
	RTCController& rtc = RTCController::getInstance();

	rt_assert(rtc.isTimeSet(), "Time needs to be set in RTC");

	registerMinCallback();
	registeredCallback = true;
}

/**
 * @brief Measures the temperature and redraws this window.
 */
void StaticMeasureTempWindow::minCallback()
{
	// TempSensor is initialized just once.
	TempSensor::init();
	temp = TempSensor::measure_temperature();
	redrawFlag = true;
}

void StaticMeasureTempWindow::registerMinCallback()
{
	RTCController::getInstance().registerMinuteCallback(this);
}


