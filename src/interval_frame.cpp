/*
 * interval_frame.cpp
 *
 *  Created on: 23. 2. 2017
 *      Author: Mayfa
 */

#include <interval_frame.hpp>

/**
 * Draws all headers.
 */
void IntervalFrame::drawHeader()
{
	LCD::draw_header((uint8_t *)"Interval setting");

	// Interval header.
	LCD::print_string(20, LINE(4), (uint8_t *)"interval", LEFT_MODE, LCD::NORMAL_FONT);

	// Temperature header.
	LCD::print_string(LCD::get_x_size()/2 + 10, LINE(4), (uint8_t *)"temperature",
			LEFT_MODE, LCD::NORMAL_FONT);
}

IntervalFrame::IntervalFrame()
{
	BSP_JOY_Init(JOY_MODE_GPIO);
	LCD::init();

	nextButton = Button(Coord(25, LINE(12)), "NEXT");
	endButton = Button(Coord(230, LINE(12)), "END");
}


