/**
 * @file connect_frame.cpp
 * @author Pavel Marek
 * @date Jul 2, 2018
 */

#include "connect_frame.hpp"
#include "lcd.hpp" // For LCD::print_string

ConnectFrame::ConnectFrame()
{
	yesButton = Button(Coord(LCD::get_x_size()/2 - 15, LINE(8)), "yes");
	noButton = Button(Coord(LCD::get_x_size()/2 + 30, LINE(8)), "not yet");

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
	LCD::print_string(10, LINE(2), (uint8_t *)"ETH initialized, connect to server?",
			CENTER_MODE, LCD::NORMAL_FONT);
}
