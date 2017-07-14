/*
 * interval_frame.cpp
 *
 *  Created on: 23. 2. 2017
 *      Author: Mayfa
 */

#include "interval_frame.hpp"

using namespace std;

/**
 * Initializes all the windows.
 * FIXME: Supposes BSP_LCD and BSP_JOY are already initialized.
 */
interval_frame::interval_frame() :
		time_from{Coord{15, LINE(5)}},
		time_to{Coord{time_from.getX() + (BSP_LCD_GetFont()->Width)*6, LINE(5)}},
		temp{Coord{3*BSP_LCD_GetXSize()/4, LINE(5)}},
		next_button{Coord{25, LINE(7)}, "NEXT"},
		end_button{Coord{70, LINE(7)}, "END"}
{ }

/**
 * Draws all headers.
 */
void interval_frame::draw_header()
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

	sFONT *font = BSP_LCD_GetFont();
	BSP_LCD_DisplayChar(time_from.getX() + (font->Width)*5, LINE(5), '-');

	/* Temperature header */
	BSP_LCD_DisplayStringAt(BSP_LCD_GetXSize()/2 + 10, LINE(4), (uint8_t *)"temperature", LEFT_MODE);
}

/**
 * Processes one interval.
 */
interval_frame_data interval_frame::process_interval()
{
	/* Reset windows' inner values */
	// Deserialize last saved intervals's "to time"
	time last_time = deserialize_time(this->data[this->data.size()-1].to);
	this->time_from.setHours(last_time.hours);
	this->time_from.setMinutes(last_time.minutes);

	this->time_to.setHours(last_time.hours);
	this->time_to.setMinutes(last_time.minutes);

	this->next_button.set_pushed(false);
	this->end_button.set_pushed(false);

	WindowSystem system;

	/* Add all windows to system */
	// ...

	system.pass_control();

	/* Investigate windows members */
	// ...

	/* Return data */
	// ...
}

std::vector<interval_frame_data> & interval_frame::pass_control()
{
	this->draw_header();

	while (!this->end_button.is_pushed()) {
		// Process next interval and save data from it
		this->data.push_back(this->process_interval());
	}
}

/**
 * Deprecated.
 */
/*
static void draw_all()
{
	if (BSP_LCD_Init() != IO_OK) {
		//return APP_ERROR;
	}

	if (BSP_JOY_Init(JOY_MODE_GPIO) != IO_OK) {
		//return APP_ERROR;
	}

	BSP_LCD_Clear(LCD_COLOR_BLACK);

	BSP_LCD_SetFont(&Font24);
	BSP_LCD_SetTextColor(LCD_COLOR_LIGHTBLUE);
	BSP_LCD_FillRect(0, 0, BSP_LCD_GetXSize(), 40);
	BSP_LCD_SetTextColor(LCD_COLOR_GREEN);
	BSP_LCD_SetBackColor(LCD_COLOR_LIGHTBLUE);
	BSP_LCD_DisplayStringAt(0, 10, (uint8_t *)"Interval setting", CENTER_MODE);

	BSP_LCD_SetTextColor(LCD_COLOR_WHITE);
	BSP_LCD_SetBackColor(LCD_COLOR_BLACK);
	BSP_LCD_SetFont(&Font16);

	BSP_LCD_DisplayStringAt(20, LINE(4), (uint8_t *)"interval", LEFT_MODE);
	unique_ptr<ControlWindow> timeFrom = make_unique<TimeWindow>(Coord{15, LINE(5)});

	sFONT *font = BSP_LCD_GetFont();
	BSP_LCD_DisplayChar(timeFrom->getX() + (font->Width)*5, LINE(5), '-');

	unique_ptr<ControlWindow> timeTo = make_unique<TimeWindow>(Coord(timeFrom->getX() + (font->Width)*6, LINE(5)));

	// Temperature window
	BSP_LCD_DisplayStringAt(BSP_LCD_GetXSize()/2 + 10, LINE(4), (uint8_t *)"temperature", LEFT_MODE);

	unique_ptr<ControlWindow> temp = make_unique<TempWindow>(Coord(3*BSP_LCD_GetXSize()/4, LINE(5)));

	// Next and end buttons
	unique_ptr<ControlWindow> next_b = make_unique<button>(Coord{25, LINE(7)}, "NEXT");
	unique_ptr<ControlWindow> end_b = make_unique<button>(Coord{70, LINE(7)}, "END");

	WindowSystem system;

	window_id timeFrom_id = system.add_control(move(timeFrom));
	window_id timeTo_id = system.add_control(move(timeTo));
	window_id temp_id = system.add_control(move(temp));
	window_id next_b_id = system.add_control(move(next_b));
	window_id end_b_id = system.add_control(move(end_b));

	system.pass_control();

	button &e_b = dynamic_cast<button &>(system.get_control(end_b_id));
	if (e_b.is_pushed()) {

	}
}*/
