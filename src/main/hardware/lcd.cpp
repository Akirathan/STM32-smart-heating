/**
 * @file lcd.cpp
 * @author Pavel Marek
 * @date 22.7.2017
 */

#include "lcd.hpp"
#include "rt_assert.h"

namespace LCD {

// Private functions.
static void set_font(const Font& font);
static void update();

// Private variables.
static bool init_ = false;
static Font curr_font = Font::NO_FONT;

/**
 * Updates current font to HAL.
 */
static void update()
{
	switch (curr_font) {
	case Font::NO_FONT:
		break;
	case Font::SEL_FONT:
		BSP_LCD_SetTextColor(SEL_FONT_TEXT_COLOR);
		BSP_LCD_SetBackColor(SEL_FONT_BACK_COLOR);
		BSP_LCD_SetFont(&SEL_FONT_FONT);
		break;
	case Font::NORMAL_FONT:
		BSP_LCD_SetTextColor(NORMAL_FONT_TEXT_COLOR);
		BSP_LCD_SetBackColor(NORMAL_FONT_BACK_COLOR);
		BSP_LCD_SetFont(&NORMAL_FONT_FONT);
		break;
	}
}

/**
 * @brief Initializes LCD peripheral and also clears display with black color.
 *
 * Ensures that @ref BSP_LCD_Init is called just once, because it takes too
 * much time.
 */
void init()
{
	if (!init_) {
		uint8_t retval = BSP_LCD_Init();
		rt_assert(retval == LCD_OK, "BSP LCD initialization failed")
		init_ = true;
	}
	BSP_LCD_Clear(LCD_COLOR_BLACK);
}

void clear()
{
	BSP_LCD_Clear(LCD_COLOR_BLACK);
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
	// Return back current font.
	update();
}

static void set_font(const Font& font)
{
	if (curr_font == font) {
		return;
	}
	else {
		curr_font = font;
		update();
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

void print_char(uint16_t x, uint16_t y, uint8_t ascii, const Font &font)
{
	set_font(font);
	BSP_LCD_DisplayChar(x, y, ascii);
}

} // namespace LCD
