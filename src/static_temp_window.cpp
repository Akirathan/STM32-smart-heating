/*
 * static_temp_window.cpp
 *
 *  Created on: Jul 21, 2017
 *      Author: mayfa
 */

#include "static_temp_window.hpp"

/**
 *
 */
static_temp_window::static_temp_window(const coord& c)  :
		static_window{c}
{ }

/**
 * Unregisters minute callback from rtc.
 */
static_temp_window::~static_temp_window()
{
	if (this->registered_callback) {
		rtc::get_instance().unregister_minute_callback(this);
	}
}

/**
 * Measures temperature and redraws window.
 */
void static_temp_window::min_callback()
{
	// TODO: measure temperature

	this->draw();
}

void static_temp_window::set_temp(uint32_t temp)
{
	this->temp = temp;
}

void static_temp_window::hide()
{
	this->hidden = true;
}

void static_temp_window::show()
{
	this->hidden = false;
}

/**
 * Registers for minute callback.
 */
void static_temp_window::measure()
{
	rtc &rtc = rtc::get_instance();

	if (!rtc.is_time_set()) {
		//TODO: error handling
	}

	rtc.register_minute_callback(this);
	this->registered_callback = true;
}

void static_temp_window::draw() const
{
	if (this->hidden) {
		return;
	}

	// This is needed because this method is called
	// from second interrupt handler.
	if (!lcd::is_initialized()) {
		return;
	}

	char text[2];
	sprintf(text, "%02lu", this->temp);
	BSP_LCD_DisplayStringAt(this->coord_.x, this->coord_.y, (uint8_t *)text, LEFT_MODE);
}

