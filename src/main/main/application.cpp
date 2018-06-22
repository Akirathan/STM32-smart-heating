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

		EEPROM::getInstance().save(data, count);

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

	TcpDriver::queueForSend(reinterpret_cast<const uint8_t *>("nazdar"), 7);

	while (true) {
		guiTask();
		TcpDriver::poll();
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
