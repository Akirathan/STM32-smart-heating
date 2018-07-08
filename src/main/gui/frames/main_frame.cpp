/**
 * @file mainframe.cpp
 * @author Pavel Marek
 * @date 21.7.2017
 */

#include "main_frame.hpp"
#include "application.hpp"
#include "intervals_changed_event.hpp"
#include "tcp_driver.hpp" // For TcpDriver::isLinkUp

void MainFrame::drawHeader()
{
	// Clear display.
	LCD::clear();

	// Draw headers.
	uint16_t X = LCD::get_x_size();
	uint16_t Y = LCD::get_y_size();
	LCD::print_string(20, LINE(3), (uint8_t *)"actual temp", LEFT_MODE, LCD::NORMAL_FONT);
	LCD::print_string(X/2 + 30, LINE(4), (uint8_t *)"set temp", LEFT_MODE, LCD::NORMAL_FONT);
	LCD::print_string(X/4 - 30, Y - 60, (uint8_t *)"INTERVALS", LEFT_MODE, LCD::NORMAL_FONT);
	LCD::print_string(X/2 + 40, Y - 60, (uint8_t *)"STATUS", LEFT_MODE, LCD::NORMAL_FONT);
}

/**
 * @brief
 * Initializes JOY and LCD peripheral and places the windows on display.
 *
 */
MainFrame::MainFrame() :
	SwTimerOwner(TIMER_TIMEOUT),
	currFrameType(NONE),
	connectedStatus(UNKNOWN),
	setIntervalFrame(),
	overviewIntervalFrame(),
	callbackRegistered(false),
	connectButtonInWindowSystem(false)
{
	uint16_t X = LCD::get_x_size();
	uint16_t Y = LCD::get_y_size();
	timeWindow = StaticTimeWindow(Coord(X/2 - 30, LINE(1)), true);
	actualTempWindow = StaticMeasureTempWindow(Coord(70, LINE(4)));
	presetTempWindow = StaticPresetTempWindow(Coord(220, LINE(4)));
	overviewButton = Button(Coord(X/4 - 30, Y - 40), "overview");
	resetButton = Button(Coord(X/4 - 30, Y - 20), "reset");
	connectButton = Button(Coord(X/2 + 40, Y - 20), "connect");
	statusTextWindow = TextWindow(Coord(X/2 + 40, Y - 40), "unknown");

	updateStatus();

	windowSystem.addStatic(&timeWindow);
	windowSystem.addStatic(&actualTempWindow);
	windowSystem.addStatic(&presetTempWindow);
	windowSystem.addStatic(&statusTextWindow);
	windowSystem.addControl(&overviewButton);
	windowSystem.addControl(&resetButton);
	// Do not add connectButton to windowSystem here.
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
	startSwTimer();

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

	stopSwTimer();

	// Hide windows registered for callbacks.
	timeWindow.hide();
	actualTempWindow.hide();
	presetTempWindow.hide();

	if (overviewButton.isPushed()) {
		currFrameType = OVERVIEW_INTERVAL_FRAME;

		// Load interval data from EEPROM.
		// Suppose eeprom is not empty.
		IntervalFrameData data[INTERVALS_NUM];
		size_t count = 0;
		EEPROM::getInstance().load(data, &count, nullptr, nullptr);

		overviewIntervalFrame.setData(data, count);
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
 *
 * If @ref SetIntervalFrame was terminated it means that user changed intervals
 * and @ref IntervalsChangedEvent is generated and dispatched to @ref Application.
 * See @ref Application and its emitEvent methods for more details.
 */
void MainFrame::frameTerminateCallback()
{
	overviewIntervalFrame.unregisterFrameTerminateCallbackReceiver(this);
	setIntervalFrame.unregisterFrameTerminateCallbackReceiver(this);

	startSwTimer();

	if (currFrameType == SET_INTERVAL_FRAME) {
		IntervalFrameData data[INTERVALS_NUM];
		size_t count = 0;
		setIntervalFrame.getData(data, &count);

		IntervalsChangedStmEvent event(data, count, Application::getCurrTimestamp(), Application::isTimeSynced());
		Application::emitEvent(event);
	}

	Application::setCurrFrame(this);
}

/**
 * Checks for device status (CONNECTED / OFFLINE).
 * @note This is a callback method that is called from SW timer timeout.
 */
void MainFrame::timeout()
{
	updateStatus();
}

void MainFrame::registerFrameTerminateCallback()
{
	// Intentionally left empty: method not used.
}

void MainFrame::updateStatus()
{
	if (!TcpDriver::isLinkUp()) {
		setOfflineStatus();
	}
	else if (TcpDriver::isLinkUp() && Application::isConnectedToServer()) {
		setConnectedStatus();
	}
	else if (TcpDriver::isLinkUp() && ! EEPROM::getInstance().isKeySet()) {
		setLinkUpStatus();
	}
	else {
		setConnectingStatus();
	}
}

/**
 * Sets the "connected status" to OFFLINE - connect button will be shown
 * and OFFLINE status text will be shown.
 * @note This method is called periodically from @ref MainFrameTimer.
 */
void MainFrame::setOfflineStatus()
{
	if (connectedStatus == OFFLINE) {
		return;
	}

	connectedStatus = OFFLINE;
	statusTextWindow.setText("offline");
	hideConnectButton();
}

void MainFrame::setLinkUpStatus()
{
	if (connectedStatus == LINK_UP) {
		return;
	}

	connectedStatus = LINK_UP;
	statusTextWindow.setText("offline");
	showConnectButton();
}

void MainFrame::setConnectingStatus()
{
	if (connectedStatus == CONNECTING) {
		return;
	}

	connectedStatus = CONNECTING;
	statusTextWindow.setText("connecting");
	hideConnectButton();
}

/**
 * Sets the "connected status" to CONNECTED - this means that connected status
 * text will be shown and connected button will be hidden.
 * @note This method is called periodically from @ref MainFrameTimer.
 */
void MainFrame::setConnectedStatus()
{
	if (connectedStatus == CONNECTED) {
		return;
	}

	connectedStatus = CONNECTED;
	statusTextWindow.setText("connected");
	hideConnectButton();
}

void MainFrame::showConnectButton()
{
	if (!connectButtonInWindowSystem) {
		windowSystem.addControl(&connectButton);
		connectButtonInWindowSystem = true;
	}
}

void MainFrame::hideConnectButton()
{
	if (connectButtonInWindowSystem) {
		windowSystem.removeControl(&connectButton);
		connectButtonInWindowSystem = false;
	}
}

