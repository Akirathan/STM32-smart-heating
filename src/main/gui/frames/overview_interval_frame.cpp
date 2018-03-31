/**
 * @file overview_interval_frame.hpp
 * @author Pavel Marek
 * @date 25.8.2017
 */

#include "overview_interval_frame.hpp"
#include "rt_assert.h"

OverviewIntervalFrame::OverviewIntervalFrame() :
	dataIdx(0),
	dataCount(0)
{
	timeFromWindow = StaticTimeWindow(Coord(15, LINE(6)), false);
	timeToWindow = StaticTimeWindow(
			Coord(timeFromWindow.getX() + (LCD::get_font()->Width)*6, LINE(6)), false);
	tempWindow = StaticTempWindow(Coord(3*LCD::get_x_size()/4, LINE(6)));

	windowSystem.addStatic(&timeFromWindow);
	windowSystem.addStatic(&timeToWindow);
	windowSystem.addStatic(&tempWindow);
	windowSystem.addControl(&nextButton);
	windowSystem.addControl(&endButton);
}

OverviewIntervalFrame::OverviewIntervalFrame(const IntervalFrameData data[], size_t count) :
	OverviewIntervalFrame()
{
	for (size_t i = 0; i < count; ++i) {
		this->data[i] = data[i];
	}
	dataCount = count;
}

/**
 * Copies given IntervalFrameData array into this data.
 */
void OverviewIntervalFrame::setData(const IntervalFrameData data[], size_t count)
{
	for (size_t i = 0; i < count; ++i) {
		this->data[i] = data[i];
	}
	dataCount = count;
}

/**
 * Note: this method is called when next or end button is pushed.
 */
void OverviewIntervalFrame::exitMessageCallback()
{
	windowSystem.unregisterExitMessageCallbackReceiver(this);
	windowSystem.stop();

	// Check if end button is pushed or last interval was displayed.
	if (endButton.isPushed() || dataIdx >= dataCount - 1) {
		callTerminateCallbackReceivers();
	}
	else {
		// Draw next interval
		dataIdx++;
		printData(data[dataIdx]);
	}

	nextButton.setPushed(false);
	endButton.setPushed(false);
}

void OverviewIntervalFrame::registerExitMessageCallback()
{
	// Intentionally left empty.
}

/**
 * @note Supposes that eeprom is not empty.
 */
void OverviewIntervalFrame::passControl()
{
	rt_assert(data[0].isSet(), "Data must be initialized first");

	dataIdx = 0;
	printData(data[0]);
}

/**
 * @brief Prints the data to the display and waits until user presses NEXT or END
 * 		  button.
 */
void OverviewIntervalFrame::printData(const IntervalFrameData& data)
{
	Time::Time from = Time::deserialize(data.from);
	Time::Time to = Time::deserialize(data.to);

	timeFromWindow.setHours(from.hours);
	timeFromWindow.setMinutes(from.minutes);

	timeToWindow.setHours(to.hours);
	timeToWindow.setMinutes(to.minutes);

	tempWindow.setTemp(data.temp);

	windowSystem.registerExitMessageCallbackReceiver(this);
	windowSystem.run();
}

void OverviewIntervalFrame::drawHeader()
{
	IntervalFrame::drawIntervalHeader();

	sFONT *font = LCD::get_font();
	LCD::print_char(timeFromWindow.getX() + (font->Width)*5, LINE(6), '-');
}

