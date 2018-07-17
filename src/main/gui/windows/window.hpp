/**
 * @file window.hpp
 * @author Pavel Marek
 * @date 18.2.2017
 */

#ifndef WINDOW_HPP_
#define WINDOW_HPP_

#include "stm3210c_eval_lcd.h"

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
	const Coord& getCoord() const;
	uint16_t getX() const;
	uint16_t getY() const;
	void redraw();
	void setRedrawFlag();
	virtual void clear();
protected:
	Coord coord;
	bool redrawFlag;
	bool lock;
private:
	virtual void draw() const = 0;
};


#endif
