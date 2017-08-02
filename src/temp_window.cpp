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
temp_window::temp_window() :
		temp_window{Coord{0,0}}
{ }

temp_window::temp_window(const Coord &coord) :
		control_window{coord}, temp{this->low_bond}, focused{false}
{

}

Message temp_window::event_handler(JOYState_TypeDef joy_state) {
	switch (joy_state) {
	case JOY_DOWN:
		if (this->temp == this->low_bond) {
			this->temp = this->high_bond;
		}
		else {
			this->temp--;
		}
		this->draw(); // Redraw
		break;
	case JOY_UP:
		if (this->temp == this->high_bond) {
			this->temp = this->low_bond;
		}
		else {
			this->temp++;
		}
		this->draw(); // Redraw
		break;
	case JOY_LEFT:
		this->unset_focus();
		return Message::FOCUS_LEFT;
	case JOY_RIGHT:
		this->unset_focus();
		return Message::FOCUS_RIGHT;
	default:
		return Message::NONE;
	}
	return Message::NONE;
}

/**
 * Suppose font is already set.
 */
void temp_window::draw()const
{
	BSP_LCD_DisplayStringAt(this->coord_.x, this->coord_.y, (uint8_t *)this->get_tempstring().c_str(), LEFT_MODE);
}

/**
 * Redraws this window and loads font.
 */
void temp_window::unset_focus()
{
	this->focused = false;
	this->load_font();
	this->draw();
}

/**
 * Saves current font for further drawing and
 * immediately redraws this window.
 */
void temp_window::set_focus(Message msg) {
	this->focused = true;
	this->save_font();
	BSP_LCD_SetTextColor(SEL_COLOR);
	this->draw();
}

/**
 * Convert temperature member temp_ to string.
 */
string temp_window::get_tempstring() const {
	char buff[2];
	sprintf(buff, "%02lu", this->temp);

	string s{buff};
	return s;
}

uint32_t temp_window::get_temp() const
{
	return this->temp;
}
