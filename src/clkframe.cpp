/*
 * clkframe.cpp
 *
 *  Created on: 4.2.2017
 *      Author: Mayfa
 */

#include "clkframe.hpp"

clk_frame::clk_frame()
{
	/* Initialize joystick peripheral */
	if (BSP_JOY_Init(JOY_MODE_GPIO) != IO_OK) {
		// Error
	}

	/* Clear the display and init LCD peripheral */
	if (BSP_LCD_Init() != LCD_OK) {
		// Error
	}
	BSP_LCD_Clear(LCD_COLOR_BLACK);

	time = time_window{Coord{BSP_LCD_GetXSize()/2 - 35, BSP_LCD_GetYSize()/2 + 10}};
	ok_button = Button{Coord{BSP_LCD_GetXSize()/2 - 15, LINE(8)}, "OK"};
}

void clk_frame::draw_header() const
{
	BSP_LCD_SetFont(&Font24);
	BSP_LCD_SetTextColor(LCD_COLOR_LIGHTBLUE);
	BSP_LCD_FillRect(0, 0, BSP_LCD_GetXSize(), 40);
	BSP_LCD_SetTextColor(LCD_COLOR_GREEN);
	BSP_LCD_SetBackColor(LCD_COLOR_LIGHTBLUE);
	BSP_LCD_DisplayStringAt(0, 10, (uint8_t *)"Time setting", CENTER_MODE);

	// Set font back
	BSP_LCD_SetTextColor(LCD_COLOR_WHITE);
	BSP_LCD_SetBackColor(LCD_COLOR_BLACK);
}

RTC_TimeTypeDef clk_frame::pass_control()
{
	this->draw_header();

	window_system system;
	system.add_control(&this->time);
	system.add_control(&this->ok_button);

	system.pass_control();

	/* Investigate window members */
	RTC_TimeTypeDef rtc_time;

	rtc_time.Hours = this->time.get_hours();
	rtc_time.Minutes = this->time.get_minutes();

	return rtc_time;
}



