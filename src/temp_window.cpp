/*
 * temp_window.cpp
 *
 *  Created on: Jul 21, 2017
 *      Author: mayfa
 */

#include "temp_window.hpp"

using namespace std;

/**
 * Needed by intervalframe ctor.
 */
TempWindow::TempWindow()
	: TempWindow(Coord(0,0))
{ }

TempWindow::TempWindow(const Coord &coord)
	: ControlWindow(coord), temp(lowBond)
{ }

Message TempWindow::eventHandler(JOYState_TypeDef joy_state) {
	switch (joy_state) {
	case JOY_DOWN:
		if (temp == lowBond) {
			temp = highBond;
		}
		else {
			temp--;
		}
		draw(); // Redraw
		break;
	case JOY_UP:
		if (temp == highBond) {
			temp = lowBond;
		}
		else {
			temp++;
		}
		draw(); // Redraw
		break;
	case JOY_LEFT:
		unsetFocus();
		return Message::FOCUS_LEFT;
	case JOY_RIGHT:
		unsetFocus();
		return Message::FOCUS_RIGHT;
	default:
		return Message::NONE;
	}
	return Message::NONE;
}

/**
 * Suppose font is already set.
 */
void TempWindow::draw()const
{
	BSP_LCD_DisplayStringAt(coord.x, coord.y, (uint8_t *)getTempstring().c_str(), LEFT_MODE);
}

/**
 * Redraws this window and loads font.
 */
void TempWindow::unsetFocus()
{
	focused = false;
	loadFont();
	draw();
}

/**
 * Saves current font for further drawing and
 * immediately redraws this window.
 */
void TempWindow::setFocus(Message msg)
{
	focused = true;
	saveFont();
	BSP_LCD_SetTextColor(SEL_COLOR);
	draw();
}

/**
 * Convert temperature member temp_ to string.
 */
string TempWindow::getTempstring() const
{
	char buff[2];
	sprintf(buff, "%02lu", temp);

	string s(buff);
	return s;
}

uint32_t TempWindow::getTemp() const
{
	return temp;
}
