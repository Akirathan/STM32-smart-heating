/*
 * butons.cpp
 *
 *  Created on: Jul 14, 2017
 *      Author: mayfa
 */

#include "buttons.hpp"

/**
 * Suppose font is already set.
 */
void button::draw() const
{
	BSP_LCD_DisplayStringAt(this->coord_.x, this->coord_.y, (uint8_t *)this->name_.c_str(), LEFT_MODE);
}

button::button(const Coord& coord, const std::string &name) : ControlWindow{coord}, name_{name}, pushed_{false},
		focused_{false}
{ }

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
		this->pushed_ = true;
		return Message::EXIT;
	}

	return Message::ERROR;
}

/**
 * Redraws this window and loads font.
 */
void button::unset_focus()
{
	this->focused_ = false;
	this->loadFont();
	this->draw();
}

/**
 * Saves current font for further drawing and
 * immediately redraws this window.
 */
void button::setFocus(Message msg)
{
	this->focused_ = true;
	this->saveFont();
	BSP_LCD_SetTextColor(SEL_COLOR);
	this->draw();
}

void button::set_pushed(bool b)
{
	this->pushed_ = b;
}

/**
 * Needed by interval_frame, when interval_frame
 * is gathering data it needs to know whether NEXT
 * or END button was pressed.
 */
bool button::is_pushed() const
{
	return this->pushed_;
}
