/**
 * @file text_window.cpp
 * @author Pavel Marek
 * @date Jul 7, 2018
 */

#include "text_window.hpp"
#include <cstring> // For std::strcpy
#include "lcd.hpp" // For LCD::print_string


TextWindow::TextWindow() :
	TextWindow(Coord(0, 0), "0")
{ }

TextWindow::TextWindow(const Coord &coord, const char *text) :
	IStaticWindow(coord),
	text{0}
{
	std::strcpy(this->text, text);
}

void TextWindow::setText(const char *text)
{
	std::strcpy(this->text, text);
	redrawFlag = true;
}

const char * TextWindow::getText() const
{
	return text;
}

void TextWindow::draw() const
{
	LCD::print_string(coord.x, coord.y, (uint8_t *)(text), LEFT_MODE, LCD::NORMAL_FONT);
}
