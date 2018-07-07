/**
 * @file lcd.hpp
 * @author Pavel Marek
 * @date 24.4.2017
 * @brief This file contains wrapper functions for BSP LCD in HAL library.
 *
 * It also contains defines for normal and selection fonts (selection font is
 * the font that control windows is displayed in when it is focused).
 */

#ifndef INC_LCD_HPP_
#define INC_LCD_HPP_

#include "stm3210c_eval_lcd.h"

#define SEL_FONT_TEXT_COLOR			LCD_COLOR_RED
#define SEL_FONT_BACK_COLOR			LCD_COLOR_BLACK
#define SEL_FONT_FONT				Font16

#define NORMAL_FONT_TEXT_COLOR		LCD_COLOR_WHITE
#define NORMAL_FONT_BACK_COLOR		LCD_COLOR_BLACK
#define NORMAL_FONT_FONT			Font16

namespace LCD {

enum Font {
	NO_FONT,
	SEL_FONT,
	NORMAL_FONT
};

void init();
void clear();
bool is_initialized();
sFONT* get_font();
uint32_t get_x_size();
uint32_t get_y_size();
void draw_header(uint8_t* text);
void print_string(uint16_t x, uint16_t y, uint8_t* text, Line_ModeTypdef mode, const Font& font);
void print_char(uint16_t x, uint16_t y, uint8_t ascii, const Font &font);

} // namespace LCD

#endif /* INC_LCD_HPP_ */
