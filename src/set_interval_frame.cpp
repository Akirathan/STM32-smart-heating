/*
 * set_interval_frame.cpp
 *
 *  Created on: Aug 25, 2017
 *      Author: mayfa
 */

#include "set_interval_frame.hpp"

/**
 * Processes one interval.
 */
IntervalFrameData SetIntervalFrame::processInterval()
{
	// Reset windows' inner values.
	Time::Time last_time;
	if (data.size() >= 1) {
		// Deserialize last saved intervals's "to time"
		last_time = Time::deserialize(data[data.size()-1].to);
	}
	else {
		last_time = Time::Time(0,0);
	}
	timeFromWindow.setHours(last_time.hours);
	timeFromWindow.setMinutes(last_time.minutes);
	timeToWindow.setHours(last_time.hours);
	timeToWindow.setMinutes(last_time.minutes);
	nextButton.setPushed(false);
	endButton.setPushed(false);

	// Add all windows to the system.
	WindowSystem system;
	system.addControl(&timeFromWindow);
	system.addControl(&timeToWindow);
	system.addControl(&tempWindow);
	system.addControl(&nextButton);
	system.addControl(&endButton);

	system.passControl();

	// Investigate windows members.
	Time::Time time_f(timeFromWindow.getHours(), timeFromWindow.getMinutes());
	Time::Time time_t(timeToWindow.getHours(), timeToWindow.getMinutes());
	uint32_t temp = tempWindow.getTemp();

	// Return data.
	IntervalFrameData data(Time::serialize(time_f), Time::serialize(time_t), temp);
	return data;
}

void SetIntervalFrame::drawHeader()
{
	IntervalFrame::drawHeader();

	sFONT *font = LCD::get_font();
	LCD::print_char(timeFromWindow.getX() + (font->Width)*5, LINE(6), '-');
}

/**
 * Initializes all the windows.
 * TODO: error handling
 */
SetIntervalFrame::SetIntervalFrame()
{
	timeFromWindow = TimeWindow(Coord(15, LINE(6)));
	timeToWindow = TimeWindow(Coord(timeFromWindow.getX() + (LCD::get_font()->Width)*6, LINE(6)));
	tempWindow = TempWindow(Coord(3*LCD::get_x_size()/4, LINE(6)));
}

std::vector<IntervalFrameData>& SetIntervalFrame::getData()
{
	return data;
}

void SetIntervalFrame::passControl()
{
	drawHeader();

	while (!endButton.isPushed()) {
		// Process next interval and save data from it
		data.push_back(processInterval());
	}
}


