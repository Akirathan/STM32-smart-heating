/*
 * static_temp_window.cpp
 *
 *  Created on: Jul 21, 2017
 *      Author: mayfa
 */

#include "static_temp_window.hpp"

/**
 *
 */
StaticTempWindow::StaticTempWindow(const Coord& c)
	: StaticWindow(c)
{ }

/**
 * Unregisters minute callback from rtc.
 */
StaticTempWindow::~StaticTempWindow()
{
	if (registeredCallback) {
		RTCController::getInstance().unregisterMinuteCallback(this);
	}
}

/**
 * Measures temperature and redraws window.
 */
void StaticTempWindow::minCallback()
{
	// TempSensor is initialized just once.
	TempSensor::init();
	temp = TempSensor::measure_temperature();
	draw();
}

void StaticTempWindow::setTemp(double temp)
{
	this->temp = temp;
}

void StaticTempWindow::hide()
{
	hidden = true;
}

void StaticTempWindow::show()
{
	hidden = false;
}

/**
 * Registers for minute callback.
 */
void StaticTempWindow::measure()
{
	RTCController& rtc = RTCController::getInstance();

	if (!rtc.isTimeSet()) {
		//TODO: error handling
	}

	rtc.registerMinuteCallback(this);
	registeredCallback = true;
}

void StaticTempWindow::draw() const
{
	if (hidden) {
		return;
	}

	// This is needed because the draw function is called
	// from second interrupt handler.
	if (!LCD::is_initialized()) {
		return;
	}

	// FIXME: printing format %2.1f does not work
	uint32_t u_temp = (uint32_t)temp;
	char text[3];
	sprintf(text, "%lu", u_temp);
	BSP_LCD_DisplayStringAt(coord.x, coord.y, (uint8_t *)text, LEFT_MODE);
}


