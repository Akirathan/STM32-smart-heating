/**
 * @file lcd.cpp
 * @author Pavel Marek
 * @date 22.7.2017
 */

#include "lcd.hpp"

namespace LCD {

// Private functions.
static void set_font(const Font& font);

// Private variables.
static bool init_ = false;
static Font curr_font = Font::NORMAL_FONT;

/**
 * @brief Initializes LCD peripheral and also clears display with black color.
 *
 * Ensures that @ref BSP_LCD_Init is called just once, because it takes too
 * much time.
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

/**
 * @brief Draws given text in a "header" style.
 */
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

/**
 * @brief Returns x size of the display in pixels.
 */
uint32_t get_x_size()
{
	return BSP_LCD_GetXSize();
}

/**
 * @brief Returns y size of the display in pixels.
 */
uint32_t get_y_size()
{
	return BSP_LCD_GetYSize();
}

/**
 * @brief Prints text in specified font.
 */
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
	BSP_LCD_DisplayChar(x, y, ascii);
}

} // namespace LCD
