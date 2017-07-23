/*
 * mainframe.cpp
 *
 *  Created on: Jul 21, 2017
 *      Author: mayfa
 */

#include "mainframe.hpp"

void mainframe::draw_header()
{
	BSP_LCD_Clear(LCD_COLOR_BLACK);
	BSP_LCD_SetTextColor(LCD_COLOR_WHITE);
	BSP_LCD_SetBackColor(LCD_COLOR_BLACK);

	BSP_LCD_DisplayStringAt(20, LINE(3), (uint8_t *)"actual temp", LEFT_MODE);
	BSP_LCD_DisplayStringAt(185, LINE(3), (uint8_t *)"set temp", LEFT_MODE);
	BSP_LCD_DisplayStringAt(BSP_LCD_GetXSize()/2 - 30, BSP_LCD_GetYSize() - 60, (uint8_t *)"INTERVALS", LEFT_MODE);
}

/**
 * Initializes JOY and LCD peripheral and places the windows
 * on display.
 */
mainframe::mainframe()
{
	BSP_JOY_Init(JOY_MODE_GPIO);
	lcd::init();
	BSP_LCD_SetFont(&Font16);

	time = static_time_window{coord{BSP_LCD_GetXSize()/2 - 30, LINE(1)}};
	act_temp = static_temp_window{coord{70, LINE(4)}};
	set_temp = static_temp_window{coord{220, LINE(4)}};
	overview_button = button{coord{BSP_LCD_GetXSize()/2 - 30, BSP_LCD_GetYSize() - 40}, "overview"};
	reset_button = button{coord{BSP_LCD_GetXSize()/2 - 30, BSP_LCD_GetYSize() - 20}, "reset"};
}

void mainframe::pass_control()
{
	this->draw_header();

	// Register these window for minute or second callbacks.
	this->time.run_clock();
	this->act_temp.measure();

	window_system system;
	system.add_static(&this->time);
	system.add_static(&this->act_temp);
	system.add_static(&this->set_temp);
	system.add_control(&this->overview_button);
	system.add_control(&this->reset_button);

	system.pass_control();

	/* Find out which button was pressed */
	// ...
}
