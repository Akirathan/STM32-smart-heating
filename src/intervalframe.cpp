/*
 * interval_frame.cpp
 *
 *  Created on: 23. 2. 2017
 *      Author: Mayfa
 */

#include <intervalframe.hpp>

using namespace std;

intervalframe_data::intervalframe_data() :
	from{0},
	to{0},
	temp{0}
{ }

intervalframe_data::intervalframe_data(uint32_t from, uint32_t to, uint32_t temp) :
	from{from},
	to{to},
	temp{temp}
{ }

/**
 * Initializes all the windows.
 * TODO: error handling
 */
intervalframe::intervalframe()
{
	BSP_JOY_Init(JOY_MODE_GPIO);
	BSP_LCD_Init();
	BSP_LCD_SetFont(&Font16);

	time_from = time_window{coord{15, LINE(6)}};
	time_to = time_window{coord{time_from.get_x() + (BSP_LCD_GetFont()->Width)*6, LINE(6)}};
	temp = temp_window{coord{3*BSP_LCD_GetXSize()/4, LINE(6)}};

	next_button = button{coord{25, LINE(12)}, "NEXT"};
	end_button = button{coord{230, LINE(12)}, "END"};
}


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

	sFONT *font = BSP_LCD_GetFont();
	BSP_LCD_DisplayChar(time_from.get_x() + (font->Width)*5, LINE(6), '-');

	/* Temperature header */
	BSP_LCD_DisplayStringAt(BSP_LCD_GetXSize()/2 + 10, LINE(4), (uint8_t *)"temperature", LEFT_MODE);
}

/**
 * Processes one interval.
 */
intervalframe_data intervalframe::process_interval()
{
	/* Reset windows' inner values */
	time last_time;

	if (this->data.size() >= 1) {
		// Deserialize last saved intervals's "to time"
		last_time = deserialize_time(this->data[this->data.size()-1].to);
	}
	else {
		last_time = time{0,0};
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
	time time_f{this->time_from.get_hours(), this->time_from.get_minutes()};
	time time_t{this->time_to.get_hours(), this->time_to.get_minutes()};
	uint32_t temp = this->temp.get_temp();

	/* Return data */
	intervalframe_data data{serialize_time(time_f), serialize_time(time_t), temp};
	return data;
}

std::vector<intervalframe_data> & intervalframe::pass_control()
{
	this->draw_header();

	while (!this->end_button.is_pushed()) {
		// Process next interval and save data from it
		this->data.push_back(this->process_interval());
	}

	return this->data;
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
