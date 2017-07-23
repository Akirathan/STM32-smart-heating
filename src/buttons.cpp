/*
 * butons.cpp
 *
 *  Created on: Jul 14, 2017
 *      Author: mayfa
 */

#include "buttons.hpp"

button::button() :
		button{coord{0,0}, ""}
{ }

button::button(const coord& coord, const std::string &name) :
		control_window{coord},
		name{name}
{ }

/**
 * Suppose font is already set.
 */
void button::draw() const
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


Message button::event_handler(JOYState_TypeDef joy_state)
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
void button::unset_focus()
{
	this->focused = false;
	this->draw();
}

/**
 * Saves current font for further drawing and
 * immediately redraws this window.
 */
void button::set_focus(Message msg)
{
	this->focused = true;
	this->draw();
}

void button::set_pushed(bool b)
{
	this->pushed = b;
}

/**
 * Needed by interval_frame, when interval_frame
 * is gathering data it needs to know whether NEXT
 * or END button was pressed.
 */
bool button::is_pushed() const
{
	return this->pushed;
}
