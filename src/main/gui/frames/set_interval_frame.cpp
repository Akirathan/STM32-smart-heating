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
	windowSystem.unregisterExitMessageCallbackReceiver(this);
	windowSystem.stop();

	// Investigate windows members.
	Time::Time time_f(timeFromWindow.getHours(), timeFromWindow.getMinutes());
	Time::Time time_t(timeToWindow.getHours(), timeToWindow.getMinutes());
	uint32_t temp = tempWindow.getTemp();

	// Store current interval data.
	IntervalFrameData curr_data(Time::serialize(time_f), Time::serialize(time_t), temp);
	data[dataCount] = curr_data;
	dataCount++;

	if (endButton.isPushed() || dataCount >= INTERVALS_NUM) {
		callTerminateCallbackReceivers();
	}
	else if (nextButton.isPushed()){
		processInterval();
	}
}

/**
 * Registers for callback when some window emits ExitMessage.
 * Note that this message can be emitted just by next or end button.
 */
void SetIntervalFrame::registerExitMessageCallback()
{
	// Intentionally left empty.
}

/**
 * Processes one interval.
 */
void SetIntervalFrame::processInterval()
{
	nextButton.setPushed(false);
	endButton.setPushed(false);

	// Reset windows' inner values.
	Time::Time last_time;
	if (dataCount >= 1) {
		// Deserialize last saved intervals's "to time"
		last_time = Time::deserialize(data[dataCount - 1].to);
	}
	else {
		last_time = Time::Time(0,0);
	}
	timeFromWindow.setHours(last_time.hours);
	timeFromWindow.setMinutes(last_time.minutes);
	timeToWindow.setHours(last_time.hours);
	timeToWindow.setMinutes(last_time.minutes);

	windowSystem.registerExitMessageCallbackReceiver(this);
	windowSystem.run();
}


void SetIntervalFrame::drawHeader()
{
	IntervalFrame::drawIntervalHeader();

	sFONT *font = LCD::get_font();
	LCD::print_char(timeFromWindow.getX() + (font->Width)*5, LINE(6), '-', LCD::NORMAL_FONT);
}

SetIntervalFrame::SetIntervalFrame() :
	dataCount(0)
{
	timeFromWindow = TimeWindow(Coord(15, LINE(6)));
	timeToWindow = TimeWindow(Coord(timeFromWindow.getX() + (LCD::get_font()->Width)*6, LINE(6)));
	tempWindow = TempWindow(Coord(3*LCD::get_x_size()/4, LINE(6)));

	// Add all windows to the system.
	windowSystem.addControl(&timeFromWindow);
	windowSystem.addControl(&timeToWindow);
	windowSystem.addControl(&tempWindow);
	windowSystem.addControl(&nextButton);
	windowSystem.addControl(&endButton);
}

/**
 * @brief Copies intervals data into data parameter.
 * @param data must be at least INTERVALS_NUM long
 */
void SetIntervalFrame::getData(IntervalFrameData data[], size_t* count)
{
	for (size_t i = 0; i < dataCount; ++i) {
		data[i] = this->data[i];
	}
	*count = dataCount;
}

void SetIntervalFrame::passControl()
{
	reset();
	processInterval();
}

void SetIntervalFrame::reset()
{
	for (size_t i = 0; i < INTERVALS_NUM; i++) {
		data[i] = IntervalFrameData();
	}
	dataCount = 0;
	timeFromWindow.setHours(0);
	timeFromWindow.setMinutes(0);
	timeToWindow.setHours(0);
	timeToWindow.setMinutes(0);
	tempWindow.setTemp(8);
}


