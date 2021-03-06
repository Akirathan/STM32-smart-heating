/**
 * @file window.hpp
 * @author Pavel Marek
 * @date 18.2.2017
 */

#ifndef WINDOW_HPP_
#define WINDOW_HPP_

#include "stm3210c_eval_lcd.h"
#include "stats.hpp"

const uint16_t SEL_COLOR = LCD_COLOR_RED;

/**
 * @brief Enum representing messeges between Window and WindowSystem.
 *
 * Refer to @ref IControlWindow::eventHandler for more informations.
 */
enum class Message {
	NONE, ERROR, EXIT, FOCUS_LEFT, FOCUS_RIGHT
};

/**
 * @brief Structure representing coordinates on LCD display.
 */
struct Coord {
	uint16_t x;
	uint16_t y;
	Coord(uint16_t x, uint16_t y);
};

/**
 * @brief Base interface for every window GUI element.
 *
 * Clients should not directly inherit this interface, instead they should
 * inherit @ref IControlWindow or @ref IStaticWindow.
 */
class Window {
public:
	Window(const Coord &coord);
	virtual ~Window() = default;
	virtual void draw() const = 0;
	const Coord& getCoord() const;
	uint32_t getX() const;
	uint32_t getY() const;
protected:
	Coord coord;
	void saveFont() const;
	void loadFont() const;
};


#endif
