/**
 * @file overview_interval_frame.hpp
 * @author Pavel Marek
 * @date 25.8.2017
 */

#include "overview_interval_frame.hpp"

OverviewIntervalFrame::OverviewIntervalFrame()
{

}

OverviewIntervalFrame::OverviewIntervalFrame(const std::vector<IntervalFrameData>& data) :
	data(data),
	dataIdx(0)
{
	timeFromWindow = StaticTimeWindow(Coord(15, LINE(6)), false);
	timeToWindow = StaticTimeWindow(
			Coord(timeFromWindow.getX() + (LCD::get_font()->Width)*6, LINE(6)), false);
	tempWindow = StaticTempWindow(Coord(3*LCD::get_x_size()/4, LINE(6)));
}

void OverviewIntervalFrame::setData(const std::vector<IntervalFrameData> &data)
{
	this->data = data;
}

/**
 * Note: this method is called when next or end button is pushed.
 */
void OverviewIntervalFrame::exitMessageCallback()
{
	windowSystem.unregisterExitMessageCallbackReceiver(this);

	// Check if end button is pushed or last interval was displayed.
	if (endButton.isPushed() || dataIdx >= data.size()) {
		callTerminateCallbackReceivers();
		return;
	}

	// Draw next interval
	dataIdx++;
	printData(data[dataIdx]);

}

void OverviewIntervalFrame::registerExitMessageCallback()
{
	windowSystem.registerExitMessageCallbackReceiver(this);
}

/**
 * @note Supposes that eeprom is not empty.
 */
void OverviewIntervalFrame::passControl()
{
	//rt_assert(data != nullptr, "Data must be initialized first");
	drawHeader();

	printData(data[dataIdx]);
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

	nextButton.setPushed(false);
	endButton.setPushed(false);

	// Add all windows to windowSystem.
	windowSystem.clear();
	windowSystem.addStatic(&timeFromWindow);
	windowSystem.addStatic(&timeToWindow);
	windowSystem.addStatic(&tempWindow);
	windowSystem.addControl(&nextButton);
	windowSystem.addControl(&endButton);

	registerExitMessageCallback();
	windowSystem.run();
}

void OverviewIntervalFrame::drawHeader()
{
	IntervalFrame::drawHeader();

	sFONT *font = LCD::get_font();
	LCD::print_char(timeFromWindow.getX() + (font->Width)*5, LINE(6), '-');
}
