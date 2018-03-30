/**
 * @file clkframe.cpp
 * @author Pavel Marek
 * @date 4.2.2017
 */

#include "clk_frame.hpp"

ClkFrame::ClkFrame()
{
	// Clear the display and init LCD peripheral.
	LCD::init();

	time = TimeWindow(Coord(LCD::get_x_size()/2 - 35, LCD::get_y_size()/2 + 10));
	ok_button = Button(Coord(LCD::get_x_size()/2 - 15, LINE(8)), "OK");
}

void ClkFrame::passControl()
{
	drawHeader();

	windowSystem.addControl(&time);
	windowSystem.addControl(&ok_button);
	windowSystem.registerExitMessageCallbackReceiver(this);
	windowSystem.run();
}

/**
 * Called when okButton pressed.
 */
void ClkFrame::exitMessageCallback()
{
	windowSystem.unregisterExitMessageCallbackReceiver(this);

	// TODO: windowSystem.clear(); ?
}

void ClkFrame::registerExitMessageCallback()
{
	// Intentionally left empty.
}

void ClkFrame::drawHeader() const
{
	LCD::draw_header((uint8_t *)"Time setting");
}

/**
 * @brief Gets the time that was set in @ref ClkFrame::passControl.
 */
RTC_TimeTypeDef ClkFrame::getTime() const
{
	// Investigate window members.
	RTC_TimeTypeDef rtc_time;
	rtc_time.Hours = time.getHours();
	rtc_time.Minutes = time.getMinutes();

	return rtc_time;
}


