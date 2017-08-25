/*
 * lcd.cpp
 *
 *  Created on: Jul 22, 2017
 *      Author: mayfa
 */

#include "lcd.hpp"

namespace LCD {

// Private functions.
static void set_font(const Font& font);

// Private variables.
static bool init_ = false;
static Font curr_font = Font::NORMAL_FONT;

/**
 * @brief
 * Initializes LCD peripheral and also clears display with black color.
 */
void init()
{
	if (init_) {
		return;
	}
	BSP_LCD_Init();
	BSP_LCD_Clear(LCD_COLOR_BLACK);
	init_ = true;
}

void draw_header(uint8_t* text)
{
	BSP_LCD_Clear(LCD_COLOR_BLACK);

	BSP_LCD_SetFont(&Font24);
	BSP_LCD_SetTextColor(LCD_COLOR_LIGHTBLUE);
	BSP_LCD_FillRect(0, 0, BSP_LCD_GetXSize(), 40);
	BSP_LCD_SetTextColor(LCD_COLOR_GREEN);
	BSP_LCD_SetBackColor(LCD_COLOR_LIGHTBLUE);
	BSP_LCD_DisplayStringAt(0, 10, text, CENTER_MODE);
}

static void set_font(const Font& font)
{
	if (curr_font == font) {
		return;
	}

	switch (font) {
	case Font::SEL_FONT:
		curr_font = Font::SEL_FONT;

		BSP_LCD_SetTextColor(SEL_FONT_TEXT_COLOR);
		BSP_LCD_SetBackColor(SEL_FONT_BACK_COLOR);
		BSP_LCD_SetFont(&SEL_FONT_FONT);
		break;
	case Font::NORMAL_FONT:
		curr_font = Font::NORMAL_FONT;

		BSP_LCD_SetTextColor(NORMAL_FONT_TEXT_COLOR);
		BSP_LCD_SetBackColor(NORMAL_FONT_BACK_COLOR);
		BSP_LCD_SetFont(&NORMAL_FONT_FONT);
		break;
	}
}

sFONT* get_font()
{
	return BSP_LCD_GetFont();
}

uint32_t get_x_size()
{
	return BSP_LCD_GetXSize();
}

uint32_t get_y_size()
{
	return BSP_LCD_GetYSize();
}

void print_string(uint16_t x, uint16_t y, uint8_t* text, Line_ModeTypdef mode, const Font& font)
{
	set_font(font);
	BSP_LCD_DisplayStringAt(x, y, text, mode);
}

bool is_initialized()
{
	return init_;
}

void print_char(uint16_t x, uint16_t y, uint8_t ascii)
{
}

} // namespace LCD
