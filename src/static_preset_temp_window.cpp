/*
 * static_preset_temp_window.cpp
 *
 *  Created on: Aug 9, 2017
 *      Author: mayfa
 */

#include "static_preset_temp_window.hpp"

StaticPresetTempWindow::StaticPresetTempWindow(const Coord& c)
	: StaticTempWindow(c)
{ }

StaticPresetTempWindow::~StaticPresetTempWindow()
{
	if (registeredCallback) {
		RTCController::getInstance().unregisterMinuteCallback(this);
	}
}

void StaticPresetTempWindow::minCallback()
{
	// Get supposed temperature from TempController and redraw this window.
	TempController& tempController = TempController::getInstance();
	temp = (double)tempController.currentIntervalTemperature();
	draw();
}

void StaticPresetTempWindow::registerMinCallback()
{
	RTCController::getInstance().registerMinuteCallback(this);
}

/**
 * Registers for minute callback.
 */
void StaticPresetTempWindow::showPresetTemp()
{
	RTCController& rtc = RTCController::getInstance();

	if (!rtc.isTimeSet()) {
		//TODO: error handling
	}

	registerMinCallback();
	registeredCallback = true;
}
