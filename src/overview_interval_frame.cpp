/**
 * @file overview_interval_frame.hpp
 * @author Pavel Marek
 * @date 25.8.2017
 */

#include "overview_interval_frame.hpp"

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


	// Add all windows to system.
	WindowSystem system;
	system.addStatic(&timeFromWindow);
	system.addStatic(&timeToWindow);
	system.addStatic(&tempWindow);
	system.addControl(&nextButton);
	system.addControl(&endButton);

	system.passControl();
}

void OverviewIntervalFrame::drawHeader()
{
	IntervalFrame::drawHeader();

	sFONT *font = LCD::get_font();
	LCD::print_char(timeFromWindow.getX() + (font->Width)*5, LINE(6), '-');
}

OverviewIntervalFrame::OverviewIntervalFrame(const std::vector<IntervalFrameData>& data)
	: data(data)
{
	timeFromWindow = StaticTimeWindow(Coord(15, LINE(6)), false);
	timeToWindow = StaticTimeWindow(
			Coord(timeFromWindow.getX() + (LCD::get_font()->Width)*6, LINE(6)), false);
	tempWindow = StaticTempWindow(Coord(3*LCD::get_x_size()/4, LINE(6)));
}

/**
 * @note Supposes that eeprom is not empty.
 */
void OverviewIntervalFrame::passControl()
{
	drawHeader();

	auto it = data.begin();
	while (!endButton.isPushed() && it != data.end()) {
		printData(*it);
		++it;
	}
}


