/**
 * @file window.hpp
 * @author Pavel Marek
 * @date 18.2.2017
 * @brief This header file contains structure(s) representing a generic type
 * 		  of window in program's GUI.
 */

#ifndef WINDOW_HPP_
#define WINDOW_HPP_

#include "stm3210c_eval_lcd.h"
#include "stats.hpp"

const uint16_t SEL_COLOR = LCD_COLOR_RED;

/**
 * @brief
 * Enum representing messeges between Window and WindowSystem.
 *
 * Refer to @ref IControlWindow::eventHandler for more informations.
 */
enum class Message {
	NONE, ERROR, EXIT, FOCUS_LEFT, FOCUS_RIGHT
};

/**
 * @brief
 * Structure representing coordinates on LCD display.
 */
struct Coord {
	uint16_t x;
	uint16_t y;
	Coord(uint16_t x, uint16_t y);
};

/**
 * @brief
 * Base interface for every window GUI element.
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

/**
 * @brief
 * Interface for user-controllable windows.
 *
 * User-controllable windows are those windows whose values can be modified
 * via up/down joystick.
 */
class IControlWindow: public Window {
public:
	IControlWindow(const Coord& coord);
	virtual ~IControlWindow() = default;
	virtual Message eventHandler(JOYState_TypeDef joy_state) = 0;
	/**
	 * @brief Sets focus for this window.
	 * @param msg is either @ref Message::LEFT or @ref Message::RIGHT.
	 */
	virtual void setFocus(Message msg) = 0;
};

/**
 * @brief
 * Interface for user non-controllable windows.
 *
 * User non-controllable windows are those windows that cannot be focused.
 * Classes that inherits this interface only shows some (text) information.
 */
class IStaticWindow: public Window {
public:
	IStaticWindow(const Coord &coord);
	virtual ~IStaticWindow() = default;
};

#endif
