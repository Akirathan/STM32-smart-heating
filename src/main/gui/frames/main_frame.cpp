/**
 * @file mainframe.cpp
 * @author Pavel Marek
 * @date 21.7.2017
 */

#include "main_frame.hpp"
#include "application.hpp"

void MainFrame::drawHeader()
{
	// Clear display.
	LCD::init();

	// Draw headers.
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
MainFrame::MainFrame() :
	currFrameType(NONE),
	setIntervalFrame(),
	overviewIntervalFrame(),
	callbackRegistered(false)
{
	LCD::init();

	timeWindow = StaticTimeWindow(Coord(LCD::get_x_size()/2 - 30, LINE(1)), true);
	actualTempWindow = StaticMeasureTempWindow(Coord(70, LINE(4)));
	presetTempWindow = StaticPresetTempWindow(Coord(220, LINE(4)));
	overviewButton = Button(Coord(LCD::get_x_size()/2 - 30, LCD::get_y_size() - 40), "overview");
	resetButton = Button(Coord(LCD::get_x_size()/2 - 30, LCD::get_y_size() - 20), "reset");

	windowSystem.addStatic(&timeWindow);
	windowSystem.addStatic(&actualTempWindow);
	windowSystem.addStatic(&presetTempWindow);
	windowSystem.addControl(&overviewButton);
	windowSystem.addControl(&resetButton);
}

/**
 * @brief Controlls the main frame.
 *
 * When overview button is pressed, then current time and current temperature
 * windows are hidden and @ref OverviewIntervalFrame is displayed. The same is
 * done when reset button is pressed, except @ref SetIntervalFrame is displayed.
 */
void MainFrame::passControl()
{
	// Register time and temperature windows for minute or second callbacks.
	if (!callbackRegistered) {
		timeWindow.runClock();
		actualTempWindow.measure();
		presetTempWindow.showPresetTemp();

		callbackRegistered = true;
	}

	timeWindow.show();
	actualTempWindow.show();
	presetTempWindow.show();

	windowSystem.registerExitMessageCallbackReceiver(this);
	windowSystem.run();
}

/**
 * Called when overviewButton or resetButton is pushed.
 * All windows of MainFrame will be overdrawed with windows from SetIntervalFrame
 * or OverviewIntervalFrame.
 */
void MainFrame::exitMessageCallback()
{
	windowSystem.unregisterExitMessageCallbackReceiver(this);
	windowSystem.stop();

	// Hide windows registered for callbacks.
	timeWindow.hide();
	actualTempWindow.hide();
	presetTempWindow.hide();

	if (overviewButton.isPushed()) {
		currFrameType = OVERVIEW_INTERVAL_FRAME;

		// Load interval data from EEPROM.
		// Suppose eeprom is not empty.
		std::vector<IntervalFrameData> data_vec;
		EEPROM::getInstance().load(data_vec);

		overviewIntervalFrame.setData(data_vec);
		overviewIntervalFrame.registerFrameTerminateCallbackReceiver(this);
		Application::setCurrFrame(&overviewIntervalFrame);
	}
	else if (resetButton.isPushed()) {
		currFrameType = SET_INTERVAL_FRAME;

		setIntervalFrame.registerFrameTerminateCallbackReceiver(this);
		Application::setCurrFrame(&setIntervalFrame);
	}

	overviewButton.setPushed(false);
	resetButton.setPushed(false);
}

/**
 * Note that this method is currently not used. Use this->windowSystem.register...
 * for registration exit message callback.
 */
void MainFrame::registerExitMessageCallback()
{
	// Intentionally left empty.
}

/**
 * This callback is called from setIntervalFrame or overviewIntervalFrame in case
 * when one of them is terminating.
 */
void MainFrame::frameTerminateCallback()
{
	overviewIntervalFrame.unregisterFrameTerminateCallbackReceiver(this);
	setIntervalFrame.unregisterFrameTerminateCallbackReceiver(this);

	if (currFrameType == SET_INTERVAL_FRAME) {
		// Reload data into TempController.
		std::vector<IntervalFrameData> data_vec = setIntervalFrame.getData();
		TempController::getInstance().reloadIntervalData(data_vec);

		// Save intervals into EEPROM.
		EEPROM::getInstance().save(data_vec);
	}

	Application::setCurrFrame(this);
}

void MainFrame::registerFrameTerminateCallback()
{
	// Intentionally left empty: method not used.
}

