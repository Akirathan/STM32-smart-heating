/**
 * @file application.cpp
 * @author mayfa
 * @date Mar 30, 2018
 */

#include "lcd.hpp"
#include "application.hpp"
#include "eeprom.hpp"
#include "rt_assert.h"
#include "tcp_driver.hpp"
#include "settings.h" // For INTERVALS_NUM
#include "client.hpp"
#include "response_buffer.hpp" // For ResponseBuffer::reset
#include "des.hpp" // For DES::init

IFrame            * Application::currFrame = nullptr;
bool                Application::clearDisplayFlag = false;
CommunicationDevice Application::communicationDevice;
IntervalList        Application::pendingIntervals;
SwTimerOwner      * Application::swTimerOwners[SW_TIMERS_NUM] = {nullptr};
size_t              Application::swTimerOwnersIdx = 0;

Application::Application() :
	clkFrame(),
	mainFrame(),
	mainTaskFinished(false)
{
	Client::init("127.0.0.1", 8000, &communicationDevice);
}

/**
 * Resets current frame and sets it for redrawing from GUI task.
 * Also sets display for clearing after previous frame.
 */
void Application::setCurrFrame(IFrame* frame)
{
	currFrame = frame;
	frame->setForRedraw();
	frame->passControl();
	clearDisplayFlag = true;

	if (isMainFrameCurrFrame()) {
		connectToServerIfPossible();
	}
}

void Application::frameTerminateCallback()
{
	rt_assert(currFrame != nullptr, "currFrame must be set");

	if (currFrame == &clkFrame) {
		Time::Time time = clkFrame.getTime();
		RTCController::getInstance().setTime(time);

		if (EEPROM::getInstance().isEmpty()) {
			setCurrFrame(&setIntervalFrame);
		}
		else {
			switchCurrFrameToMain();
		}
	}
	// EEPROM was empty --> set interval frame terminated
	else if (currFrame == &setIntervalFrame) {
		IntervalFrameData data[INTERVALS_NUM];
		size_t count = 0;
		setIntervalFrame.getData(data, &count);

		IntervalsChangedStmEvent event(data, count, getCurrTimestamp(), isTimeSynced());
		Application::emitEvent(event);

		switchCurrFrameToMain();
	}
}

void Application::registerFrameTerminateCallback()
{
	// Intentionally left blank.
}

void Application::run()
{
	while (true) {
		TcpDriver::poll();
		mainTask();
		pollSwTimers();
		guiTask();
	}
}

/**
 * Returns current timestamp. It does not matter if time is synchronized with
 * server or not.
 * If you want to now whether time is synchronized with server, you should
 * call @ref Application::isTimeSynced
 * @return current (Unix) timestamp
 */
uint32_t Application::getCurrTimestamp()
{
	return communicationDevice.getCurrentTimestamp();
}

bool Application::isConnectedToServer()
{
	return communicationDevice.isConnected();
}

/**
 * Returns bool whether time (specificaly timestamp) is synchronized with server
 * or not.
 * Note that time synchronization is done during connecting to server.
 * For more information about communication with server see @ref Client.
 */
bool Application::isTimeSynced()
{
	return communicationDevice.isTimeSynchronized();
}

void Application::registerSwTimerOwnerForPolling(SwTimerOwner *timer_owner)
{
	rt_assert(swTimerOwnersIdx <= SW_TIMERS_NUM, "Attempting too add too much SW timer owners");
	swTimerOwners[swTimerOwnersIdx] = timer_owner;
	swTimerOwnersIdx++;
}

/**
 * Dispatches "connected to the server" event.
 *
 * Reloads intervals metadata (timestamp and time_synced flag) in EEPROM if
 * necessary.
 * If there are some pendingIntervals - fix their timestamp and dispatch them
 * to communicationDevice.
 * @param event
 */
void Application::emitEvent(const ConnectedEvent &event)
{
	RTCController::getInstance().setTimestamp(event.getServerTime());

	updateIntervalsMetadataInEEPROM(event);

	if (!pendingIntervals.isEmpty()) {
		uint32_t curr_timestamp = pendingIntervals.getTimestamp();
		pendingIntervals.setTimestamp(curr_timestamp + event.getTimeShift());
		communicationDevice.setIntervals(pendingIntervals);
	}
}

/**
 * Dispatches "measured temperature" event.
 * More specifically displays the temperature on display (@ref MainFrame) and
 * sends the temperature to server via @ref Client.
 * @param event
 */
void Application::emitEvent(const MeasuredTempEvent &event)
{
	communicationDevice.setTemp(event.getTemp());
}

/**
 * Dispatches "intervals changed in STM" event. The event is generated on STM, more
 * specifically it was generated in @ref SetIntervalFrame.
 * Note that the "intervals changed on server" event is handled by other emitEvent
 * method.
 *
 * Intervals are saved into EEPROM and compared with intervals from server.
 * This involves comparing timestamp of the intervals.
 * @param event
 */
void Application::emitEvent(const IntervalsChangedStmEvent &event)
{
	size_t count = 0;
	const IntervalFrameData *data = event.getData(&count);

	EEPROM::getInstance().save(data, count, event.getTimestamp(), event.isTimestampSynced());
	TempController::getInstance().reloadIntervalData(data, count);

	if (isTimeSynced()) {
		communicationDevice.setIntervals(event.getList());
	}
	else {
		pendingIntervals = event.getList();
	}
}

/**
 * Dispatches "intervals changed on server" (intervals downloaded from server) event.
 *
 * For more info about this event see @ref IntervalsChangedServerEvent.
 *
 * @param event ... event to dispatch
 */
void Application::emitEvent(const IntervalsChangedServerEvent &event)
{
	size_t data_count = 0;
	const IntervalFrameData *data = event.getData(&data_count);

	EEPROM::getInstance().save(data, data_count, event.getTimestamp(), event.isTimestampSynced());
	TempController::getInstance().reloadIntervalData(data, data_count);
}

/**
 * This event is generated by @ref TcpDriver.
 * @param event
 */
void Application::emitEvent(const CommunicationErrorEvent &event)
{
	communicationDevice.disconnect();
	Client::disconnect();
	http::ResponseBuffer::reset();
}

/**
 * This event is generated by @ref TcpDriver when ethernet link is up.
 * @param event
 */
void Application::emitEvent(const EthLinkUpEvent &event)
{
	connectToServerIfPossible();
}

/**
 * This event is generated when user inputs the DES key into @ref KeyFrame
 */
void Application::emitEvent(const KeySetEvent &event)
{
	EEPROM::getInstance().saveKey(event.getKey());
	communicationDevice.setKey(event.getKey());

	connectToServerIfPossible();
}

void Application::updateIntervalsMetadataInEEPROM(const ConnectedEvent &event)
{
	uint32_t timestamp = 0;
	bool time_synced = false;
	EEPROM &eeprom = EEPROM::getInstance();

	eeprom.loadIntervalsMetadata(&timestamp, &time_synced);
	if (time_synced == false) {
		eeprom.saveIntervalsMetadata(getCurrTimestamp() + event.getTimeShift(), true);
	}
}

/**
 * We need to check if current frame is MainFrame because all connectivity
 * issues are related just to MainFrame (we want to notify user about
 * connectivity only in MainFrame, not in other frames).
 */
bool Application::isMainFrameCurrFrame()
{
	MainFrame *main_frame = dynamic_cast<MainFrame *>(currFrame);
	return main_frame != nullptr;
}

/**
 * MainFrame is going to be current frame. Try to connect to the server.
 *
 * Note that this is the only method (in all application) where connection to
 * the server may be invoked.
 */
void Application::connectToServerIfPossible()
{
	if (isConnectedToServer()) {
		return;
	}

	EEPROM &eeprom = EEPROM::getInstance();
	if (TcpDriver::isLinkUp() && eeprom.isKeySet() && isMainFrameCurrFrame()) {
		communicationDevice.setKey(eeprom.loadKey());
		communicationDevice.connect();
	}
}

void Application::guiTask()
{
	if (clearDisplayFlag) {
		LCD::clear();
		currFrame->setForRedraw();
		clearDisplayFlag = false;
	}
	currFrame->redraw();
}

void Application::pollSwTimers()
{
	for (size_t i = 0; i < swTimerOwnersIdx; i++) {
		swTimerOwners[i]->poll();
	}
}

void Application::mainTask()
{
	if (mainTaskFinished) {
		return;
	}

	EEPROM &eeprom = EEPROM::getInstance();

	if (!TcpDriver::isLinkUp()) {
		clkFrame.registerFrameTerminateCallbackReceiver(this);
		setCurrFrame(&clkFrame);
	}
	else if (TcpDriver::isLinkUp() && !eeprom.isKeySet()) {
		connectFrame.registerFrameTerminateCallbackReceiver(this);
		setCurrFrame(&connectFrame);
	}
	else if (TcpDriver::isLinkUp() && eeprom.isKeySet()) {
		// Set dummy time - it will be overwriten in a while.
		RTCController::getInstance().setTime(Time::Time(0, 0));

		setCurrFrame(&mainFrame);
	}

	mainTaskFinished = true;
}

void Application::switchCurrFrameToMain()
{
	RTCController &rtc = RTCController::getInstance();
	if (!rtc.isTimeSet()) {
		rtc.setTime(Time::Time(0, 0));
	}

	TempController::getInstance().controlTemperature();
	setCurrFrame(&mainFrame);
}

