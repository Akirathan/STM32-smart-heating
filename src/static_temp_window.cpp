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
StaticTempWindow::StaticTempWindow(const Coord& c)
	: StaticWindow(c)
{ }


void StaticTempWindow::hide()
{
	hidden = true;
}

void StaticTempWindow::show()
{
	hidden = false;
}

void StaticTempWindow::setTemp(double temp)
{
	this->temp = temp;
}


void StaticTempWindow::draw() const
{
	if (hidden) {
		return;
	}

	// This is needed because the draw function is called
	// from second interrupt handler.
	if (!LCD::is_initialized()) {
		return;
	}

	// FIXME: printing format %2.1f does not work
	uint32_t u_temp = (uint32_t)temp;
	char text[3];
	sprintf(text, "%lu", u_temp);
	BSP_LCD_DisplayStringAt(coord.x, coord.y, (uint8_t *)text, LEFT_MODE);
}
