#include "window.hpp"

using namespace std;

/* Storage of current system font */
static uint16_t text_color;
static uint16_t back_color;

Coord::Coord(uint32_t x, uint32_t y) : x(x), y(y) {

}

// FIXME
bool operator<(const Coord &c1, const Coord &c2) {
	return (c1.x <= c2.x) && (c1.y <= c2.y);
}

/**
 * Return true if the pointer to window is of type
 * ControlWindow.
 */
bool Window::is_control(Window *w) {
	ControlWindow *ptr = dynamic_cast<ControlWindow *>(w);
	if (ptr) {
		return true;
	}
	else {
		return false;
	}
}

/**
 * Return true if the pointer to window is of type
 * StaticWindow.
 */
bool Window::is_static(Window *w) {
	StaticWindow *ptr = dynamic_cast<StaticWindow *>(w);
	if (ptr) {
		return true;
	}
	else {
		return false;
	}
}

Window::Window(const Coord &coord) : coord_(coord) {

}

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

uint32_t Window::getX() const {
	return this->coord_.x;
}

uint32_t Window::getY() const {
	return this->coord_.y;
}

ControlWindow::ControlWindow(const Coord &coord) : Window(coord) {

}

StaticWindow::StaticWindow(const Coord &coord) : Window(coord) {

}

Windows::Windows(vector<unique_ptr<Window>> windows) : windows_(move(windows)) {

}

const Coord& Window::getCoord() const {
	return this->coord_;
}
