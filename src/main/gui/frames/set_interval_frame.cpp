/**
 * @file set_interval_frame.cpp
 * @author Pavel Marek
 * @date 25.8.2017
 */

#include "set_interval_frame.hpp"

/**
 * @note This callback method is called when next or end button is pushed.
 */
void SetIntervalFrame::exitMessageCallback()
{
	// Investigate windows members.
	Time::Time time_f(timeFromWindow.getHours(), timeFromWindow.getMinutes());
	Time::Time time_t(timeToWindow.getHours(), timeToWindow.getMinutes());
	uint32_t temp = tempWindow.getTemp();

	// Store current interval data.
	IntervalFrameData curr_data(Time::serialize(time_f), Time::serialize(time_t), temp);
	data.push_back(curr_data);

	if (nextButton.isPushed()) {
		// Process next interval.
		processInterval();
	}
}

void SetIntervalFrame::registerExitMessageCallback()
{
	windowSystem.registerExitMessageCallbackReceiver(this);
}

/**
 * Processes one interval.
 */
void SetIntervalFrame::processInterval()
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

	if (!callbackRegistered) {
		windowSystem.registerExitMessageCallbackReceiver(this);
		callbackRegistered = true;
	}
	windowSystem.run();
}


void SetIntervalFrame::drawHeader()
{
	IntervalFrame::drawHeader();

	sFONT *font = LCD::get_font();
	LCD::print_char(timeFromWindow.getX() + (font->Width)*5, LINE(6), '-');
}

SetIntervalFrame::SetIntervalFrame()
	: callbackRegistered(false)
{
	timeFromWindow = TimeWindow(Coord(15, LINE(6)));
	timeToWindow = TimeWindow(Coord(timeFromWindow.getX() + (LCD::get_font()->Width)*6, LINE(6)));
	tempWindow = TempWindow(Coord(3*LCD::get_x_size()/4, LINE(6)));

	nextButton.setPushed(false);
	endButton.setPushed(false);

	// Add all windows to the system.
	windowSystem.addControl(&timeFromWindow);
	windowSystem.addControl(&timeToWindow);
	windowSystem.addControl(&tempWindow);
	windowSystem.addControl(&nextButton);
	windowSystem.addControl(&endButton);
}

/**
 * @brief Returns data from one interval.
 *
 * Must be called after @ref passControl function.
 */
std::vector<IntervalFrameData>& SetIntervalFrame::getData()
{
	return data;
}

void SetIntervalFrame::passControl()
{
	drawHeader();

	processInterval();
}


