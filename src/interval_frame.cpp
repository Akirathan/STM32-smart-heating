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
	BSP_LCD_Clear(LCD_COLOR_BLACK);

	BSP_LCD_SetFont(&Font24);
	BSP_LCD_SetTextColor(LCD_COLOR_LIGHTBLUE);
	BSP_LCD_FillRect(0, 0, BSP_LCD_GetXSize(), 40);
	BSP_LCD_SetTextColor(LCD_COLOR_GREEN);
	BSP_LCD_SetBackColor(LCD_COLOR_LIGHTBLUE);
	BSP_LCD_DisplayStringAt(0, 10, (uint8_t *)"Interval setting", CENTER_MODE);


	/* Interval header */
	BSP_LCD_SetTextColor(LCD_COLOR_WHITE);
	BSP_LCD_SetBackColor(LCD_COLOR_BLACK);
	BSP_LCD_SetFont(&Font16);
	BSP_LCD_DisplayStringAt(20, LINE(4), (uint8_t *)"interval", LEFT_MODE);

	/* Temperature header */
	BSP_LCD_DisplayStringAt(BSP_LCD_GetXSize()/2 + 10, LINE(4), (uint8_t *)"temperature", LEFT_MODE);
}

IntervalFrame::IntervalFrame()
{
	BSP_JOY_Init(JOY_MODE_GPIO);
	BSP_LCD_Init();
	BSP_LCD_SetFont(&Font16);

	nextButton = Button(Coord(25, LINE(12)), "NEXT");
	endButton = Button(Coord(230, LINE(12)), "END");
}

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

	sFONT *font = BSP_LCD_GetFont();
	BSP_LCD_DisplayChar(timeFromWindow.getX() + (font->Width)*5, LINE(6), '-');
}

/**
 * Initializes all the windows.
 * TODO: error handling
 */
SetIntervalFrame::SetIntervalFrame()
{
	timeFromWindow = TimeWindow(Coord(15, LINE(6)));
	timeToWindow = TimeWindow(Coord(timeFromWindow.getX() + (BSP_LCD_GetFont()->Width)*6, LINE(6)));
	tempWindow = TempWindow(Coord(3*BSP_LCD_GetXSize()/4, LINE(6)));
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

	sFONT *font = BSP_LCD_GetFont();
	BSP_LCD_DisplayChar(timeFromWindow.getX() + (font->Width)*5, LINE(6), '-');
}

OverviewIntervalFrame::OverviewIntervalFrame(const std::vector<IntervalFrameData>& data)
	: data(data)
{
	timeFromWindow = StaticTimeWindow(Coord(15, LINE(6)), false);
	timeToWindow = StaticTimeWindow(
			Coord(timeFromWindow.getX() + (BSP_LCD_GetFont()->Width)*6, LINE(6)), false);
	tempWindow = StaticTempWindow(Coord(3*BSP_LCD_GetXSize()/4, LINE(6)));
}

/**
 * Supposes that eeprom is not empty.
 */
void OverviewIntervalFrame::passControl()
{
	drawHeader();

	/* Print the data until end button is pushed, or every
	 * interval is printed. */
	auto it = data.begin();
	while (!endButton.isPushed() && it != data.end()) {
		printData(*it);
		++it;
	}
}
