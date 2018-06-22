/**
 * @file temp_window.hpp
 * @author Pavel Marek
 * @date 21.7.2017
 */

#include "temp_window.hpp"

TempWindow::TempWindow(const Coord &coord)
	: IControlWindow(coord), temp(lowBond)
{ }

Message TempWindow::_eventHandler(JOYState_TypeDef joy_state)
{
	switch (joy_state) {
	case JOY_DOWN:
		if (temp == lowBond) {
			temp = highBond;
		}
		else {
			temp--;
		}
		draw();
		break;

	case JOY_UP:
		if (temp == highBond) {
			temp = lowBond;
		}
		else {
			temp++;
		}
		draw();
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

void TempWindow::draw()const
{
	char text[3];
	LCD::Font font;

	if (focused) {
		font = LCD::SEL_FONT;
	}
	else {
		font = LCD::NORMAL_FONT;
	}

	sprintf(text, "%02lu", temp);
	LCD::print_string(coord.x, coord.y, (uint8_t *)text, LEFT_MODE, font);
}

/**
 * @brief Redraws this window with @ref LCD::NORMAL_FONT.
 */
void TempWindow::unsetFocus()
{
	focused = false;
	draw();
}

/**
 * @brief Redraws this window with @ref LCD::SEL_FONT.
 */
void TempWindow::setFocus(Message msg)
{
	focused = true;
	draw();
}

uint32_t TempWindow::getTemp() const
{
	return temp;
}
