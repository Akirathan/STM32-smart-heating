/**
 * @file clkframe.cpp
 * @author Pavel Marek
 * @date 4.2.2017
 */

#include "clk_frame.hpp"

ClkFrame::ClkFrame()
{
	time = TimeWindow(Coord(LCD::get_x_size()/2 - 35, LCD::get_y_size()/2 + 10));
	ok_button = Button(Coord(LCD::get_x_size()/2 - 15, LINE(8)), "OK");

	windowSystem.addControl(&time);
	windowSystem.addControl(&ok_button);
}

void ClkFrame::passControl()
{
	windowSystem.registerExitMessageCallbackReceiver(this);
	windowSystem.run();
}

/**
 * Called when okButton pressed.
 */
void ClkFrame::exitMessageCallback()
{
	windowSystem.unregisterExitMessageCallbackReceiver(this);

	windowSystem.stop();
	callTerminateCallbackReceivers();
}

void ClkFrame::registerExitMessageCallback()
{
	// Intentionally left empty.
}

void ClkFrame::drawHeader()
{
	LCD::draw_header((uint8_t *)"Time setting");
}

/**
 * @brief Gets the time that was set in @ref ClkFrame::passControl.
 */
Time::Time ClkFrame::getTime() const
{
	// Investigate window members.
	Time::Time t;
	t.hours = time.getHours();
	t.minutes = time.getMinutes();

	return t;
}


