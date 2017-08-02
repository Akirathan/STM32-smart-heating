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
	// TODO: measure temperature

	draw();
}

void StaticTempWindow::setTemp(uint32_t temp)
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
	RTCController &rtc = RTCController::getInstance();

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

	// This is needed because this method is called
	// from second interrupt handler.
	if (!LCD::is_initialized()) {
		return;
	}

	char text[2];
	sprintf(text, "%02lu", temp);
	BSP_LCD_DisplayStringAt(coord.x, coord.y, (uint8_t *)text, LEFT_MODE);
}

