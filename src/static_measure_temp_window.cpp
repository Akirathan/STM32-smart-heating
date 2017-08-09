/*
 * static_measure_temp_window.cpp
 *
 *  Created on: Aug 9, 2017
 *      Author: mayfa
 */

#include "static_measure_temp_window.hpp"

StaticMeasureTempWindow::StaticMeasureTempWindow(const Coord& c)
	: StaticTempWindow(c)
{ }

/**
 * Unregisters minute callback from rtc.
 */
StaticMeasureTempWindow::~StaticMeasureTempWindow()
{
	if (registeredCallback) {
		RTCController::getInstance().unregisterMinuteCallback(this);
	}
}

/**
 * Registers for minute callback.
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
 * Measures temperature and redraws window.
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


