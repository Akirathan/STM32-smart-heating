/*
 * butons.cpp
 *
 *  Created on: Jul 14, 2017
 *      Author: mayfa
 */

#include "buttons.hpp"

Button::Button() :
		Button{Coord{0,0}, ""}
{ }

Button::Button(const Coord& coord, const std::string &name) :
		control_window{coord},
		name{name}
{ }

/**
 * Suppose font is already set.
 */
void Button::draw() const
{
	if (this->focused) {
		this->save_font();
		BSP_LCD_SetTextColor(SEL_COLOR);
	}

	BSP_LCD_DisplayStringAt(this->coord_.x, this->coord_.y, (uint8_t *)this->name.c_str(), LEFT_MODE);

	if (this->focused) {
		this->load_font();
	}
}


Message Button::event_handler(JOYState_TypeDef joy_state)
{
	switch (joy_state) {
	case JOY_UP:
	case JOY_DOWN:
		return Message::NONE;
	case JOY_LEFT:
		this->unset_focus();
		return Message::FOCUS_LEFT;
	case JOY_RIGHT:
		this->unset_focus();
		return Message::FOCUS_RIGHT;
	case JOY_SEL:
		this->pushed = true;
		this->unset_focus();
		return Message::EXIT;
	}

	return Message::ERROR;
}

/**
 * Redraws this window and loads previously
 * configured font.
 */
void Button::unset_focus()
{
	this->focused = false;
	this->draw();
}

/**
 * Saves current font for further drawing and
 * immediately redraws this window.
 */
void Button::set_focus(Message msg)
{
	this->focused = true;
	this->draw();
}

void Button::set_pushed(bool b)
{
	this->pushed = b;
}

/**
 * Needed by interval_frame, when interval_frame
 * is gathering data it needs to know whether NEXT
 * or END button was pressed.
 */
bool Button::is_pushed() const
{
	return this->pushed;
}
