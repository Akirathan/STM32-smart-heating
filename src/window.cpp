#include "window.hpp"

using namespace std;

/* Storage of current system font */
static uint16_t text_color;
static uint16_t back_color;

coord::coord(uint16_t x, uint16_t y) : x(x), y(y) {

}

// FIXME
bool operator<(const coord &c1, const coord &c2) {
	return (c1.x <= c2.x) && (c1.y <= c2.y);
}

/**
 * Return true if the pointer to window is of type
 * ControlWindow.
 */
bool window::is_control(window *w) {
	control_window *ptr = dynamic_cast<control_window *>(w);
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
bool window::is_static(window *w) {
	static_window *ptr = dynamic_cast<static_window *>(w);
	if (ptr) {
		return true;
	}
	else {
		return false;
	}
}

window::window(const coord &coord) : coord_(coord) {

}

/**
 * Save system's font before changing
 * it.
 */
void window::save_font() const
{
	text_color = BSP_LCD_GetTextColor();
	back_color = BSP_LCD_GetBackColor();
}

/**
 * TODO: Do not load it into global variables.
 * Load system's font after changing
 * it.
 */
void window::load_font() const
{
	BSP_LCD_SetTextColor(text_color);
	BSP_LCD_SetBackColor(back_color);
}

uint32_t window::get_x() const {
	return this->coord_.x;
}

uint32_t window::get_y() const {
	return this->coord_.y;
}

control_window::control_window(const coord &coord) : window(coord) {

}

static_window::static_window(const coord &coord) : window(coord) {

}

const coord& window::getCoord() const {
	return this->coord_;
}
