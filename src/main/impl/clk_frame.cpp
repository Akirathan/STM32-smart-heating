/**
 * @file clkframe.cpp
 * @author Pavel Marek
 * @date 4.2.2017
 */

#include "stm3210c_eval.h"
#include "clk_frame.hpp"

ClkFrame::ClkFrame()
{
	// Initialize joystick peripheral.
	if (BSP_JOY_Init(JOY_MODE_GPIO) != IO_OK) {
		// Error
	}

	// Clear the display and init LCD peripheral.
	LCD::init();

	time = TimeWindow(Coord(LCD::get_x_size()/2 - 35, LCD::get_y_size()/2 + 10));
	ok_button = Button(Coord(LCD::get_x_size()/2 - 15, LINE(8)), "OK");
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

void ClkFrame::passControl()
{
	drawHeader();

	WindowSystem system;
	system.addControl(&time);
	system.addControl(&ok_button);
	system.passControl();
}


