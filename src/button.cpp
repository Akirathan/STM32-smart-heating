/*
 * butons.cpp
 *
 *  Created on: Jul 14, 2017
 *      Author: mayfa
 */

#include <button.hpp>

Button::Button()
	: Button(Coord(0,0), "")
{ }

Button::Button(const Coord& coord, const std::string& name)
	: IControlWindow(coord), name(name)
{ }

/**
 * Suppose font is already set.
 */
void Button::draw() const
{
	if (focused) {
		saveFont();
		BSP_LCD_SetTextColor(SEL_COLOR);
	}

	BSP_LCD_DisplayStringAt(coord.x, coord.y, (uint8_t *)name.c_str(), LEFT_MODE);

	if (focused) {
		loadFont();
	}
}


Message Button::eventHandler(JOYState_TypeDef joy_state)
{
	switch (joy_state) {
	case JOY_UP:
	case JOY_DOWN:
	case JOY_NONE:
		return Message::NONE;
	case JOY_LEFT:
		unsetFocus();
		return Message::FOCUS_LEFT;
	case JOY_RIGHT:
		unsetFocus();
		return Message::FOCUS_RIGHT;
	case JOY_SEL:
		pushed = true;
		unsetFocus();
		return Message::EXIT;
	}

	return Message::ERROR;
}

/**
 * @brief
 * Redraws this window and loads previously configured font.
 */
void Button::unsetFocus()
{
	focused = false;
	draw();
}

/**
 * Saves current font for further drawing and
 * immediately redraws this window.
 */
void Button::setFocus(Message msg)
{
	focused = true;
	draw();
}

void Button::setPushed(bool b)
{
	pushed = b;
}

/**
 * Needed by interval_frame, when interval_frame
 * is gathering data it needs to know whether NEXT
 * or END button was pressed.
 */
bool Button::isPushed() const
{
	return pushed;
}
