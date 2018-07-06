/**
 * @file connect_frame.cpp
 * @author Pavel Marek
 * @date Jul 2, 2018
 */

#include "connect_frame.hpp"
#include "lcd.hpp" // For LCD::print_string

ConnectFrame::ConnectFrame()
{
	yesButton = Button(Coord(LCD::get_x_size()/2 - 60, LINE(8)), "yes");
	noButton = Button(Coord(LCD::get_x_size()/2 + 10, LINE(8)), "not yet");

	windowSystem.addControl(&yesButton);
	windowSystem.addControl(&noButton);
}

void ConnectFrame::passControl()
{
	windowSystem.registerExitMessageCallbackReceiver(this);
	windowSystem.run();
}

void ConnectFrame::exitMessageCallback()
{
	windowSystem.unregisterExitMessageCallbackReceiver(this);
	windowSystem.stop();
	callTerminateCallbackReceivers();
}

void ConnectFrame::registerExitMessageCallback()
{
	// Intentionally left empty.
}

bool ConnectFrame::yesButtonPushed() const
{
	return yesButton.isPushed();
}

void ConnectFrame::drawHeader()
{
	LCD::print_string(10, LINE(4), (uint8_t *)"ETH initialized,", LEFT_MODE, LCD::NORMAL_FONT);
	LCD::print_string(10, LINE(5), (uint8_t *)"connect to server?", LEFT_MODE, LCD::NORMAL_FONT);
}
