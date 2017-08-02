/*
 * interval_frame.cpp
 *
 *  Created on: 23. 2. 2017
 *      Author: Mayfa
 */

#include <intervalframe.hpp>

using namespace std;

/**
 * Draws all headers.
 */
void intervalframe::draw_header()
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

intervalframe::intervalframe()
{
	BSP_JOY_Init(JOY_MODE_GPIO);
	BSP_LCD_Init();
	BSP_LCD_SetFont(&Font16);

	this->next_button = Button{Coord{25, LINE(12)}, "NEXT"};
	this->end_button = Button{Coord{230, LINE(12)}, "END"};
}

/**
 * Processes one interval.
 */
intervalframe_data set_intervalframe::process_interval()
{
	/* Reset windows' inner values */
	time::time last_time;

	if (this->data.size() >= 1) {
		// Deserialize last saved intervals's "to time"
		last_time = time::deserialize(this->data[this->data.size()-1].to);
	}
	else {
		last_time = time::time{0,0};
	}

	this->time_from.set_hours(last_time.hours);
	this->time_from.set_minutes(last_time.minutes);

	this->time_to.set_hours(last_time.hours);
	this->time_to.set_minutes(last_time.minutes);

	this->next_button.set_pushed(false);
	this->end_button.set_pushed(false);

	window_system system;

	/* Add all windows to system */
	system.add_control(&this->time_from);
	system.add_control(&this->time_to);
	system.add_control(&this->temp);
	system.add_control(&this->next_button);
	system.add_control(&this->end_button);

	system.pass_control();

	/* Investigate windows members */
	time::time time_f{this->time_from.get_hours(), this->time_from.get_minutes()};
	time::time time_t{this->time_to.get_hours(), this->time_to.get_minutes()};
	uint32_t temp = this->temp.get_temp();

	/* Return data */
	intervalframe_data data{time::serialize(time_f), time::serialize(time_t), temp};
	return data;
}

void set_intervalframe::draw_header()
{
	intervalframe::draw_header();

	sFONT *font = BSP_LCD_GetFont();
	BSP_LCD_DisplayChar(this->time_from.get_x() + (font->Width)*5, LINE(6), '-');
}

/**
 * Initializes all the windows.
 * TODO: error handling
 */
set_intervalframe::set_intervalframe()
{
	time_from = time_window{Coord{15, LINE(6)}};
	time_to = time_window{Coord{time_from.get_x() + (BSP_LCD_GetFont()->Width)*6, LINE(6)}};
	temp = temp_window{Coord{3*BSP_LCD_GetXSize()/4, LINE(6)}};
}

std::vector<intervalframe_data>& set_intervalframe::get_data()
{
	return this->data;
}

void set_intervalframe::pass_control()
{
	this->draw_header();

	while (!this->end_button.is_pushed()) {
		// Process next interval and save data from it
		this->data.push_back(this->process_interval());
	}
}

void overview_intervalframe::print_data(const intervalframe_data& data)
{
	time::time from = time::deserialize(data.from);
	time::time to = time::deserialize(data.to);

	this->time_from.set_hours(from.hours);
	this->time_from.set_minutes(from.minutes);

	this->time_to.set_hours(to.hours);
	this->time_to.set_minutes(to.minutes);

	this->temp.set_temp(data.temp);

	this->next_button.set_pushed(false);
	this->end_button.set_pushed(false);

	window_system system;

	/* Add all windows to system */
	system.add_static(&this->time_from);
	system.add_static(&this->time_to);
	system.add_static(&this->temp);
	system.add_control(&this->next_button);
	system.add_control(&this->end_button);

	system.pass_control();
}

void overview_intervalframe::draw_header()
{
	intervalframe::draw_header();

	sFONT *font = BSP_LCD_GetFont();
	BSP_LCD_DisplayChar(this->time_from.get_x() + (font->Width)*5, LINE(6), '-');
}

overview_intervalframe::overview_intervalframe(const std::vector<intervalframe_data>& data) :
		data{data}
{
	time_from = static_time_window{Coord{15, LINE(6)}, false};
	time_to = static_time_window{Coord{time_from.get_x() + (BSP_LCD_GetFont()->Width)*6, LINE(6)}, false};
	temp = static_temp_window{Coord{3*BSP_LCD_GetXSize()/4, LINE(6)}};
}

/**
 * Supposes that eeprom is not empty.
 */
void overview_intervalframe::pass_control()
{
	this->draw_header();

	/* Print the data until end button is pushed, or every
	 * interval is printed. */
	auto it = this->data.begin();
	while (!this->end_button.is_pushed() && it != this->data.end()) {
		this->print_data(*it);
		++it;
	}
}
