/*
 * static_time_window.cpp
 *
 *  Created on: Jul 21, 2017
 *      Author: mayfa
 */

#include "static_time_window.hpp"

void error_handler(); // From main module

/**
 * Constructor does not register for callbacks.
 */
static_time_window::static_time_window(const Coord& c, bool sec_precision) :
		static_window(c),
		sec_precision(sec_precision)
{ }


/**
 * Unregister from callback.
 */
static_time_window::~static_time_window()
{
	rtc::get_instance().unregister_second_callback(this);
}


void static_time_window::inc_min()
{
	if (this->minutes == 59) {
		this->minutes = 0;
		/* Increase hours */
		if (this->hours == 23) {
			this->hours = 0;
		}
		else {
			this->hours++;
		}
	}
	else {
		this->minutes++;
	}
}

void static_time_window::inc_sec()
{
	if (this->seconds == 59) {
		this->seconds = 0;
		this->inc_min();
	}
	else {
		this->seconds++;
	}
}

/**
 * Used as callback function.
 */
void static_time_window::sec_callback_()
{
	// Increase seconds and redraw window
	this->inc_sec();
	this->draw();
}

void static_time_window::min_callback_()
{
	this->inc_min();
	this->draw();
}

/**
 * Prints hours and minutes. Prints also seconds if SECOND_PRECISION
 * is defined.
 */
void static_time_window::draw() const
{
	if (this->hidden) {
		return;
	}

	/* Check if lcd is initialized */
	// This is needed because this method is called
	// from second interrupt handler.
	if (!lcd::is_initialized()) {
		return;
	}

	/* Print hours */
	char text[2];
	sprintf(text, "%02u", this->hours);

	sFONT *font = BSP_LCD_GetFont();
	BSP_LCD_DisplayStringAt(this->coord_.x, this->coord_.y, (uint8_t *)text, LEFT_MODE);
	BSP_LCD_DisplayStringAt(this->coord_.x + (font->Width * 2), this->coord_.y, (uint8_t *)":", LEFT_MODE);

	/* Print minutes */
	sprintf(text, "%02u", this->minutes);
	BSP_LCD_DisplayStringAt(this->coord_.x + (font->Width * 3), this->coord_.y, (uint8_t *)text, LEFT_MODE);

	/* Print seconds */
	if (this->sec_precision) {
		BSP_LCD_DisplayStringAt(this->coord_.x + (font->Width * 5), this->coord_.y, (uint8_t *)":", LEFT_MODE);

		sprintf(text, "%02u", this->seconds);
		BSP_LCD_DisplayStringAt(this->coord_.x + (font->Width * 6), this->coord_.y, (uint8_t *)text, LEFT_MODE);
	}
}

void static_time_window::set_hours(uint8_t hours)
{
	this->hours = hours;
}

void static_time_window::set_minutes(uint8_t minutes)
{
	this->minutes = minutes;
}

/**
 * Hides this window.
 */
void static_time_window::hide()
{
	this->hidden = true;
}

/**
 * "Unhides" this window.
 */
void static_time_window::show()
{
	this->hidden = false;
}

/**
 * Initializes the clock and registers for second or
 * minute callback.
 *
 * Note: that constructor just sets time as 00:00
 * Note: supposes that rtc is already initialized and the
 * time is set.
 */
void static_time_window::run_clock()
{
	if (!rtc::get_instance().is_time_set()) {
		error_handler();
	}

	/* Set time */
	RTC_TimeTypeDef rtc_time;
	rtc::get_instance().get_time(&rtc_time);

	this->hours = rtc_time.Hours;
	this->minutes = rtc_time.Minutes;
	this->seconds = rtc_time.Seconds;

	/* Register minute (second) callback */
	if (this->sec_precision) {
		rtc::get_instance().register_second_callback(this);
	}
	else {
		rtc::get_instance().register_minute_callback(this);
	}
}

/**
 * Used as callback from rtc. Just calls member function.
 */
void static_time_window::sec_callback()
{
	this->sec_callback_();
}

void static_time_window::min_callback()
{
	this->min_callback_();
}

