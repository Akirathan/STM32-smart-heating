#include "window.hpp"

// Storage of current system font.
static uint16_t text_color;
static uint16_t back_color;

Coord::Coord(uint16_t x, uint16_t y)
	: x(x), y(y)
{ }

Window::Window(const Coord& coord) :
	coord(coord),
	redrawFlag(true),
	lock(false)
{ }

/**
 * @brief
 * Saves system's font before changing it.
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

const Coord& Window::getCoord() const
{
	return coord;
}

uint32_t Window::getX() const
{
	return coord.x;
}

uint32_t Window::getY() const
{
	return coord.y;
}

void Window::redraw()
{
	// Check if this window is not locked for event handling.
	if (lock || !redrawFlag) {
		return;
	}

	// Call draw implementation from derived classes.
	lock = true;
	draw();
	redrawFlag = false;
	lock = false;
}

/**
 * Sets the window so it will be redrawed in GUI task.
 */
void Window::setRedrawFlag()
{
	redrawFlag = true;
}
