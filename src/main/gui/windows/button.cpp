/**
 * @file butons.cpp
 * @author Pavel Marek
 * @date 14.7.2017
 */

#include "button.hpp"
#include <cstring> // For std::strcpy

Button::Button()
	: Button(Coord(0,0), "")
{ }

Button::Button(const Coord& coord, const char *name)
	: IControlWindow(coord)
{
	std::strcpy(this->name, name);
}

void Button::draw() const
{
	if (hidden) {
		sFONT *font = LCD::get_font();
		LCD::fill_rectangle(coord.x, coord.y, std::strlen(name) * font->Width, font->Height);
	}

	LCD::Font font;

	if (focused) {
		font = LCD::SEL_FONT;
	}
	else {
		font = LCD::NORMAL_FONT;
	}

	LCD::print_string(coord.x, coord.y, (uint8_t *)name, LEFT_MODE, font);
}

Message Button::_eventHandler(JOYState_TypeDef joy_state)
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

void Button::unsetFocus()
{
	focused = false;
	redrawFlag = true;
}

void Button::setFocus(Message msg)
{
	focused = true;
	redrawFlag = true;
}

void Button::show()
{
	hidden = false;
	redrawFlag = true;
}

void Button::hide()
{
	hidden = true;
	redrawFlag = true;
}

void Button::setPushed(bool b)
{
	pushed = b;
}

/**
 * Needed by @ref SetIntervalFrame, when SetIntervalFrame is gathering data it
 * needs to know whether NEXT or END button was pressed.
 */
bool Button::isPushed() const
{
	return pushed;
}
