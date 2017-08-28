/**
 * @file static_measure_temp_window.cpp
 * @author Pavel Marek
 * @date 9.8.2017
 */

#include "static_measure_temp_window.hpp"

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

	if (!rtc.isTimeSet()) {
		//TODO: error handling
	}

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
	draw();
}

void StaticMeasureTempWindow::registerMinCallback()
{
	RTCController::getInstance().registerMinuteCallback(this);
}


