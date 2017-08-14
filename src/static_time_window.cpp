/*
 * static_time_window.cpp
 *
 *  Created on: Jul 21, 2017
 *      Author: mayfa
 */

#include "static_time_window.hpp"

/**
 *
 * @param c Coordinates.
 * @param sec_precision Specifies clock format. When set to true, then
 * long format (ie. hours:minutes:seconds) is used. Default is true.
 */
StaticTimeWindow::StaticTimeWindow(const Coord& c, bool sec_precision)
	: IStaticWindow(c), secPrecision(sec_precision)
{ }


/**
 * @brief
 * Unregisters from callback.
 */
StaticTimeWindow::~StaticTimeWindow()
{
	if (secPrecision) {
		RTCController::getInstance().unregisterSecondCallback(this);
	}
	else {
		RTCController::getInstance().unregisterMinuteCallback(this);
	}
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
 * @brief
 * Initializes the clock and registers for second or minute callback.
 *
 * @note
 * Supposes that @ref RTCController is already initialized and the time is set.
 */
void StaticTimeWindow::runClock()
{
	if (!RTCController::getInstance().isTimeSet()) {
		// TODO: error
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
 * @note
 * Used (called from @ref RTCController) with second precision.
 */
void StaticTimeWindow::secCallback()
{
	// Increase seconds and redraw window
	incSec();
	draw();
}

void StaticTimeWindow::registerSecCallback()
{
	RTCController::getInstance().registerSecondCallback(this);
}

/**
 * @note
 * Used (called from @ref RTCController) without second precision.
 */
void StaticTimeWindow::minCallback()
{
	incMin();
	draw();
}

void StaticTimeWindow::registerMinCallback()
{
	RTCController::getInstance().registerMinuteCallback(this);
}
