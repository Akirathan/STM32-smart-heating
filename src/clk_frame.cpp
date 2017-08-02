/*
 * clkframe.cpp
 *
 *  Created on: 4.2.2017
 *      Author: Mayfa
 */

#include <clk_frame.hpp>

ClkFrame::ClkFrame()
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

	time = TimeWindow(Coord(BSP_LCD_GetXSize()/2 - 35, BSP_LCD_GetYSize()/2 + 10));
	ok_button = Button(Coord(BSP_LCD_GetXSize()/2 - 15, LINE(8)), "OK");
}

void ClkFrame::drawHeader() const
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

RTC_TimeTypeDef ClkFrame::passControl()
{
	drawHeader();

	WindowSystem system;
	system.addControl(&time);
	system.addControl(&ok_button);
	system.passControl();

	// Investigate window members
	RTC_TimeTypeDef rtc_time;
	rtc_time.Hours = time.getHours();
	rtc_time.Minutes = time.getMinutes();

	return rtc_time;
}



