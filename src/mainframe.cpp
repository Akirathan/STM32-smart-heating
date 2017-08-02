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

	time = static_time_window{Coord{BSP_LCD_GetXSize()/2 - 30, LINE(1)}, true};
	act_temp = static_temp_window{Coord{70, LINE(4)}};
	set_temp = static_temp_window{Coord{220, LINE(4)}};
	overview_button = Button{Coord{BSP_LCD_GetXSize()/2 - 30, BSP_LCD_GetYSize() - 40}, "overview"};
	reset_button = Button{Coord{BSP_LCD_GetXSize()/2 - 30, BSP_LCD_GetYSize() - 20}, "reset"};
}

/**
 * Control should remain in this method.
 */
void mainframe::pass_control()
{
	// Register time and temperature windows for
	// minute or second callbacks.
	this->time.run_clock();
	this->act_temp.measure();

	while (true) {
		this->draw_header();

		// Show time and actual temp window if they were
		// hidden from last frame.
		this->time.show();
		this->act_temp.show();

		// Reset button state
		this->overview_button.set_pushed(false);
		this->reset_button.set_pushed(false);

		window_system system;
		// Add window into window_system and draw them
		// immediately.
		system.add_static(&this->time);
		system.add_static(&this->act_temp);
		system.add_static(&this->set_temp);
		system.add_control(&this->overview_button);
		system.add_control(&this->reset_button);

		system.pass_control();

		std::vector<intervalframe_data> data_vec;

		// Hide windows registered for callbacks.
		this->time.hide();
		this->act_temp.hide();

		// Find out which button was pressed.
		if (this->overview_button.is_pushed()) {
			// Load interval data from EEPROM.
			// Suppose eeprom is not empty.
			eeprom::get_instance().load(data_vec);

			overview_intervalframe frame{data_vec};

			frame.pass_control();
		}
		else if (this->reset_button.is_pushed()) {
			set_intervalframe frame;

			// Set intervals.
			frame.pass_control();
			data_vec = frame.get_data();

			// Save intervals into EEPROM.
			eeprom::get_instance().save(data_vec);
		}
	}
}
