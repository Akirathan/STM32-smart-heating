/**
 * @file interval_frame.cpp
 * @author Pavel Marek
 * @date 23.2.2017
 */

#include "interval_frame.hpp"

IntervalFrame::IntervalFrame()
{
	LCD::init();

	nextButton = Button(Coord(25, LINE(12)), "NEXT");
	endButton = Button(Coord(230, LINE(12)), "END");
}

void IntervalFrame::drawHeader()
{
	LCD::draw_header((uint8_t *)"Interval setting");

	// Interval header.
	LCD::print_string(20, LINE(4), (uint8_t *)"interval", LEFT_MODE, LCD::NORMAL_FONT);

	// Temperature header.
	LCD::print_string(LCD::get_x_size()/2 + 10, LINE(4), (uint8_t *)"temperature",
			LEFT_MODE, LCD::NORMAL_FONT);
}

void IntervalFrame::registerFrameTerminateCallbackReceiver(IFrameTerminateCallback *receiver)
{

}

void IntervalFrame::callTerminateCallbackReceivers()
{

}
