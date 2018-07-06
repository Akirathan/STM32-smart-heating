/**
 * @file hex_char_window.cpp
 * @author Pavel Marek
 * @date Jul 6, 2018
 */

#include "hex_char_window.hpp"
#include "lcd.hpp"

HexCharWindow::HexCharWindow() :
	HexCharWindow(Coord(0, 0))
{ }

HexCharWindow::HexCharWindow(const Coord &coord) :
	IControlWindow(coord),
	hexChar('0'),
	focused(false)
{

}

void HexCharWindow::setFocus(Message msg)
{
	focused = true;
	redrawFlag = true;
}

void HexCharWindow::draw() const
{
	LCD::Font font;

	if (focused) {
		font = LCD::SEL_FONT;
	}
	else {
		font = LCD::NORMAL_FONT;
	}

	LCD::print_char(coord.x, coord.y, hexChar, font);
}

Message HexCharWindow::_eventHandler(JOYState_TypeDef joy_state)
{
	switch (joy_state) {
		case JOY_SEL:
		case JOY_NONE:
			return Message::NONE;

		case JOY_UP:
			charUp();
			redrawFlag = true;
			return Message::NONE;

		case JOY_DOWN:
			charDown();
			redrawFlag = true;
			return Message::NONE;

		case JOY_LEFT:
			focused = false;
			redrawFlag = true;
			return Message::FOCUS_LEFT;

		case JOY_RIGHT:
			focused = false;
			redrawFlag = true;
			return Message::FOCUS_RIGHT;
	}

	return Message::ERROR;
}

void HexCharWindow::charUp()
{
	if ((hexChar >= '0' && hexChar <= '8') || (hexChar >= 'A' && hexChar <= 'E')) {
		hexChar++;
	}
	else if (hexChar == '9') {
		hexChar = 'A';
	}
	else if (hexChar == 'F') {
		hexChar = '0';
	}
}

void HexCharWindow::charDown()
{
	if ((hexChar >= '1' && hexChar <= '9') || (hexChar >= 'B' && hexChar <= 'F')) {
		hexChar--;
	}
	else if (hexChar == '0') {
		hexChar = 'F';
	}
	else if (hexChar == 'A') {
		hexChar = '9';
	}
}

