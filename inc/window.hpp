/*
 * window.h
 *
 *  Created on: 18. 2. 2017
 *      Author: Mayfa
 *
 * This header file contains structure(s) representing a generic
 * type of window in program's GUI.
 */
#ifndef WINDOW_HPP_
#define WINDOW_HPP_

#include <cstdint>
#include <memory>
#include <vector>
#include "stm3210c_eval_lcd.h"
#include "stats.hpp"

const uint16_t SEL_COLOR = LCD_COLOR_RED;

/**
 * Message between windows and/or between window and system.
 */
enum class Message {
	NONE, ERROR, EXIT, FOCUS_LEFT, FOCUS_RIGHT
};

struct Coord {
	uint32_t x;
	uint32_t y;
	Coord(uint32_t x, uint32_t y);
};

bool operator<(const Coord &c1, const Coord &c2);

class Window {
public:
	static bool is_control(Window *w);
	static bool is_static(Window *w);
	Window(const Coord &coord);
	virtual ~Window() = default;
	virtual void draw() const = 0;
	const Coord& getCoord() const;
	uint32_t getX() const;
	uint32_t getY() const;
protected:
	Coord coord_;
	void saveFont() const;
	void loadFont() const;
};

class ControlWindow: public Window {
public:
	ControlWindow(const Coord &coord);
	virtual ~ControlWindow() = default;
	virtual Message event_handler(JOYState_TypeDef joy_state) = 0;
	virtual void setFocus(Message msg) = 0;
};

class StaticWindow: public Window {
public:
	StaticWindow(const Coord &coord);
	virtual ~StaticWindow() = default;
};

class Windows {
public:
	Windows(std::vector<std::unique_ptr<Window>> windows);
	virtual ~Windows() = default;
	void next();
	void previous();
	/**
	 * Modules pass control to this method. Windows object then
	 * controls all the user's input and pass control back to system
	 * when some of the windows sends Message::EXIT message.
	 */
	virtual AppStatus_TypeDef pass_control() = 0;
protected:
	std::vector<std::unique_ptr<Window>> windows_;
	virtual AppStatus_TypeDef draw() = 0;
};

#endif
