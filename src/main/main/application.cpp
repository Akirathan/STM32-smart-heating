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

IFrame * Application::currFrame = nullptr;
bool Application::clearDisplayFlag = false;

Application::Application() :
	clkFrame(),
	mainFrame()
{

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
}

void Application::frameTerminateCallback()
{
	rt_assert(currFrame != nullptr, "currFrame must be set");

	if (currFrame == &clkFrame) {
		RTC_TimeTypeDef rtc_time = clkFrame.getTime();
		RTCController::getInstance().setTime(&rtc_time);

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

		IntervalsChangedEvent event(data, count, getCurrTimestamp(), isTimeSynced());
		Application::emitEvent(event);

		switchCurrFrameToMain();
	}
}

void Application::registerFrameTerminateCallback()
{
	// Intentionally left blank.
}

void Application::switchCurrFrameToMain()
{
	TempController::getInstance().controlTemperature();
	setCurrFrame(&mainFrame);
}

void Application::run()
{
	// Prepare first frame for displaying.
	clkFrame.registerFrameTerminateCallbackReceiver(this);
	setCurrFrame(&clkFrame);

	Client::init("127.0.0.1", 8000, &communicationDevice);
	communicationDevice.setKey("key");
	communicationDevice.connect();

	while (true) {
		guiTask();
		TcpDriver::poll();
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

/**
 * Dispatches "measured temperature" event.
 * More specifically displays the temperature on display (@ref MainFrame) and
 * sends the temperature to server via @ref Client.
 * @param event
 */
void Application::emitEvent(const MeasuredTempEvent &event)
{

}

/**
 * Dispatches "intervals changed" event. The event is generated on STM, more
 * specifically it was generated in @ref SetIntervalFrame.
 * Note that the "intervals changed on server" event is handled by @ref Client
 * via calling intervalsRecvCb callback.
 *
 * Intervals are saved into EEPROM and compared with intervals from server.
 * This involves comparing timestamp of the intervals.
 * @param event
 */
void Application::emitEvent(const IntervalsChangedEvent &event)
{
	size_t count = 0;
	const IntervalFrameData *data = event.getData(&count);

	EEPROM::getInstance().save(data, count, getCurrTimestamp(), isTimeSynced());
	TempController::getInstance().reloadIntervalData(data, count);
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
