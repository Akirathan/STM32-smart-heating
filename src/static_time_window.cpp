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
StaticTimeWindow::StaticTimeWindow(const Coord& c, bool sec_precision)
	: StaticWindow(c), secPrecision(sec_precision)
{ }


/**
 * Unregister from callback.
 */
StaticTimeWindow::~StaticTimeWindow()
{
	RTCController::getInstance().unregisterSecondCallback(this);
}


void StaticTimeWindow::incMin()
{
	if (minutes == 59) {
		minutes = 0;
		/* Increase hours */
		if (hours == 23) {
			hours = 0;
		}
		else {
			hours++;
		}
	}
	else {
		minutes++;
	}
}

void StaticTimeWindow::incSec()
{
	if (seconds == 59) {
		seconds = 0;
		incMin();
	}
	else {
		seconds++;
	}
}

/**
 * Used as callback function.
 */
void StaticTimeWindow::secCallback_()
{
	// Increase seconds and redraw window
	incSec();
	draw();
}

void StaticTimeWindow::minCallback_()
{
	incMin();
	draw();
}

/**
 * Prints hours and minutes. Prints also seconds if SECOND_PRECISION
 * is defined.
 */
void StaticTimeWindow::draw() const
{
	if (hidden) {
		return;
	}

	/* Check if lcd is initialized */
	// This is needed because this method is called
	// from second interrupt handler.
	if (!LCD::is_initialized()) {
		return;
	}

	// Print hours.
	char text[2];
	sprintf(text, "%02u", hours);
	sFONT *font = BSP_LCD_GetFont();
	BSP_LCD_DisplayStringAt(coord.x, coord.y, (uint8_t *)text, LEFT_MODE);
	BSP_LCD_DisplayStringAt(coord.x + (font->Width * 2), coord.y, (uint8_t *)":", LEFT_MODE);

	// Print minutes.
	sprintf(text, "%02u", minutes);
	BSP_LCD_DisplayStringAt(coord.x + (font->Width * 3), coord.y, (uint8_t *)text, LEFT_MODE);

	// Print seconds.
	if (secPrecision) {
		BSP_LCD_DisplayStringAt(coord.x + (font->Width * 5), coord.y, (uint8_t *)":", LEFT_MODE);
		sprintf(text, "%02u", seconds);
		BSP_LCD_DisplayStringAt(coord.x + (font->Width * 6), coord.y, (uint8_t *)text, LEFT_MODE);
	}
}

void StaticTimeWindow::setHours(uint8_t hrs)
{
	hours = hrs;
}

void StaticTimeWindow::setMinutes(uint8_t mins)
{
	minutes = mins;
}

/**
 * Hides this window.
 */
void StaticTimeWindow::hide()
{
	hidden = true;
}

/**
 * "Unhides" this window.
 */
void StaticTimeWindow::show()
{
	hidden = false;
}

/**
 * Initializes the clock and registers for second or
 * minute callback.
 *
 * Note: that constructor just sets time as 00:00
 * Note: supposes that rtc is already initialized and the
 * time is set.
 */
void StaticTimeWindow::runClock()
{
	if (!RTCController::getInstance().isTimeSet()) {
		error_handler();
	}

	// Set time.
	RTC_TimeTypeDef rtc_time;
	RTCController::getInstance().getTime(&rtc_time);
	hours = rtc_time.Hours;
	minutes = rtc_time.Minutes;
	seconds = rtc_time.Seconds;

	// Register minute (second) callback.
	if (secPrecision) {
		registerSecCallback();
	}
	else {
		registerMinCallback();
	}
}

/**
 * Used as callback from rtc. Just calls member function.
 */
void StaticTimeWindow::secCallback()
{
	secCallback_();
}

void StaticTimeWindow::minCallback()
{
	minCallback_();
}

