/*
 * mainframe.cpp
 *
 *  Created on: Jul 21, 2017
 *      Author: mayfa
 */

#include "main_frame.hpp"

void MainFrame::drawHeader()
{
	BSP_LCD_Clear(LCD_COLOR_BLACK);
	BSP_LCD_SetTextColor(LCD_COLOR_WHITE);
	BSP_LCD_SetBackColor(LCD_COLOR_BLACK);

	BSP_LCD_DisplayStringAt(20, LINE(3), (uint8_t *)"actual temp", LEFT_MODE);
	BSP_LCD_DisplayStringAt(185, LINE(3), (uint8_t *)"set temp", LEFT_MODE);
	BSP_LCD_DisplayStringAt(BSP_LCD_GetXSize()/2 - 30, BSP_LCD_GetYSize() - 60,
			(uint8_t *)"INTERVALS", LEFT_MODE);
}

/**
 * Initializes JOY and LCD peripheral and places the windows
 * on display.
 */
MainFrame::MainFrame()
{
	BSP_JOY_Init(JOY_MODE_GPIO);
	LCD::init();
	BSP_LCD_SetFont(&Font16);

	timeWindow = StaticTimeWindow(Coord(BSP_LCD_GetXSize()/2 - 30, LINE(1)), true);
	actualTempWindow = StaticTempWindow(Coord(70, LINE(4)));
	presetTempWindow = StaticTempWindow(Coord(220, LINE(4)));
	overviewButton = Button(Coord(BSP_LCD_GetXSize()/2 - 30, BSP_LCD_GetYSize() - 40), "overview");
	resetButton = Button(Coord(BSP_LCD_GetXSize()/2 - 30, BSP_LCD_GetYSize() - 20), "reset");
}

/**
 * Control should remain in this method.
 */
void MainFrame::passControl()
{
	// Register time and temperature windows for
	// minute or second callbacks.
	timeWindow.runClock();
	actualTempWindow.measure();

	// Initialize temperature controlling.
	TempController::getInstance().controlTemperature();

	while (true) {
		drawHeader();

		// Show time and actual temp window if they were
		// hidden from last frame.
		timeWindow.show();
		actualTempWindow.show();

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
