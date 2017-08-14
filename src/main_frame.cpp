/*
 * mainframe.cpp
 *
 *  Created on: Jul 21, 2017
 *      Author: mayfa
 */

#include "main_frame.hpp"

void MainFrame::drawHeader()
{
	LCD::print_string(20, LINE(3), (uint8_t *)"actual temp", LEFT_MODE, LCD::NORMAL_FONT);
	LCD::print_string(185, LINE(3), (uint8_t *)"set temp", LEFT_MODE, LCD::NORMAL_FONT);
	LCD::print_string(LCD::get_x_size()/2 - 30, LCD::get_y_size() - 60,
			(uint8_t *)"INTERVALS", LEFT_MODE, LCD::NORMAL_FONT);
}

/**
 * @brief
 * Initializes JOY and LCD peripheral and places the windows on display.
 *
 */
MainFrame::MainFrame()
{
	BSP_JOY_Init(JOY_MODE_GPIO);
	LCD::init();

	timeWindow = StaticTimeWindow(Coord(LCD::get_x_size()/2 - 30, LINE(1)), true);
	actualTempWindow = StaticMeasureTempWindow(Coord(70, LINE(4)));
	presetTempWindow = StaticPresetTempWindow(Coord(220, LINE(4)));
	overviewButton = Button(Coord(LCD::get_x_size()/2 - 30, LCD::get_y_size() - 40), "overview");
	resetButton = Button(Coord(LCD::get_x_size()/2 - 30, LCD::get_y_size() - 20), "reset");
}

/**
 * Control should remain in this method.
 */
void MainFrame::passControl()
{
	// Initialize temperature controlling.
	TempController::getInstance().controlTemperature();

	// Register time and temperature windows for
	// minute or second callbacks.
	timeWindow.runClock();
	actualTempWindow.measure();
	presetTempWindow.showPresetTemp();

	while (true) {
		drawHeader();

		// Show time and actual temp window if they were
		// hidden from last frame.
		timeWindow.show();
		actualTempWindow.show();
		presetTempWindow.show();

		// Reset button state
		overviewButton.setPushed(false);
		resetButton.setPushed(false);

		// Add window into window_system and draw them
		// immediately.
		WindowSystem system;
		system.addStatic(&timeWindow);
		system.addStatic(&actualTempWindow);
		system.addStatic(&presetTempWindow);
		system.addControl(&overviewButton);
		system.addControl(&resetButton);

		// Control from this function is returned when overview or reset button
		// are pressed.
		system.passControl();

		std::vector<IntervalFrameData> data_vec;

		// Hide windows registered for callbacks.
		timeWindow.hide();
		actualTempWindow.hide();
		presetTempWindow.hide();

		// Find out which button was pressed.
		if (overviewButton.isPushed()) {
			// Load interval data from EEPROM.
			// Suppose eeprom is not empty.
			EEPROM::getInstance().load(data_vec);

			OverviewIntervalFrame frame{data_vec};

			frame.passControl();
		}
		else if (resetButton.isPushed()) {
			SetIntervalFrame frame;

			// Set intervals.
			frame.passControl();
			data_vec = frame.getData();

			// Reload data into TempController
			TempController::getInstance().reloadIntervalData(data_vec);

			// Save intervals into EEPROM.
			EEPROM::getInstance().save(data_vec);
		}
	}
}
