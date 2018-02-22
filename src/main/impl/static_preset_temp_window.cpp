/**
 * @file static_preset_temp_window.cpp
 * @author Pavel Marek
 * @date 9.8.2017
 */

#include "static_preset_temp_window.hpp"

StaticPresetTempWindow::StaticPresetTempWindow(const Coord& c)
	: StaticTempWindow(c)
{ }

/**
 * @brief Unregisters from minute callback.
 */
StaticPresetTempWindow::~StaticPresetTempWindow()
{
	if (registeredCallback) {
		RTCController::getInstance().unregisterMinuteCallback(this);
	}
}

/**
 * @brief Gets temperature that is supposed to be set in the current interval
 * 		  and redraws this window with updated value.
 */
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
 * @brief Registers for minute callback.
 */
void StaticPresetTempWindow::showPresetTemp()
{
	RTCController& rtc = RTCController::getInstance();

	if (!rtc.isTimeSet()) {
		Error_Handler();
	}

	registerMinCallback();
	registeredCallback = true;
}
