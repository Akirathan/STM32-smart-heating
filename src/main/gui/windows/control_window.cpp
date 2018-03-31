/**
 * @file control_window.cpp
 * @author Pavel Marek
 * @date Aug 28, 2017
 */

#include "control_window.hpp"

IControlWindow::IControlWindow(const Coord& coord)
	: Window(coord)
{ }

Message IControlWindow::eventHandler(JOYState_TypeDef joy_state)
{
	// Check if this window is locked (for drawing) or waits for draw.
	if (lock || redrawFlag) {
		return Message::NONE;
	}

	lock = true;
	Message msg = _eventHandler(joy_state);
	lock = false;

	return msg;
}
