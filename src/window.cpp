#include "window.hpp"

using namespace std;

// Storage of current system font.
static uint16_t text_color;
static uint16_t back_color;

Coord::Coord(uint16_t x, uint16_t y)
	: x(x), y(y)
{ }

// FIXME
bool operator<(const Coord& c1, const Coord& c2)
{
	return (c1.x <= c2.x) && (c1.y <= c2.y);
}

Window::Window(const Coord& coord)
	: coord(coord)
{ }

/**
 * Save system's font before changing
 * it.
 */
void Window::saveFont() const
{
	text_color = BSP_LCD_GetTextColor();
	back_color = BSP_LCD_GetBackColor();
}

/**
 * TODO: Do not load it into global variables.
 * Load system's font after changing
 * it.
 */
void Window::loadFont() const
{
	BSP_LCD_SetTextColor(text_color);
	BSP_LCD_SetBackColor(back_color);
}

uint32_t Window::getX() const
{
	return coord.x;
}

uint32_t Window::getY() const
{
	return coord.y;
}

ControlWindow::ControlWindow(const Coord& coord)
	: Window(coord)
{ }

StaticWindow::StaticWindow(const Coord& coord)
	: Window(coord)
{ }

const Coord& Window::getCoord() const
{
	return coord;
}
