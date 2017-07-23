/*
 * lcd.cpp
 *
 *  Created on: Jul 22, 2017
 *      Author: mayfa
 */

#include "lcd.hpp"

namespace lcd {

static bool init_ = false;

void init()
{
	BSP_LCD_Init();
	BSP_LCD_Clear(LCD_COLOR_BLACK);
	init_ = true;
}

bool is_initialized()
{
	return init_;
}

} // namespace lcd

