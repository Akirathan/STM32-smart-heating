#include "window.hpp"

Coord::Coord(uint16_t x, uint16_t y)
	: x(x), y(y)
{ }

Window::Window(const Coord& coord) :
	coord(coord),
	redrawFlag(true),
	lock(false)
{ }

const Coord& Window::getCoord() const
{
	return coord;
}

uint16_t Window::getX() const
{
	return coord.x;
}

uint16_t Window::getY() const
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
