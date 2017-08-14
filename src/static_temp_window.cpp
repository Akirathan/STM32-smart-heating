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
	: IStaticWindow(c)
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
	LCD::print_string(coord.x, coord.y, (uint8_t *)text, LEFT_MODE, LCD::NORMAL_FONT);
}
