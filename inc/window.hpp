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
	uint16_t x;
	uint16_t y;
	Coord(uint16_t x, uint16_t y);
};

bool operator<(const Coord &c1, const Coord &c2);

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

class ControlWindow: public Window {
public:
	ControlWindow(const Coord& coord);
	virtual ~ControlWindow() = default;
	virtual Message eventHandler(JOYState_TypeDef joy_state) = 0;
	virtual void setFocus(Message msg) = 0;
};

class StaticWindow: public Window {
public:
	StaticWindow(const Coord &coord);
	virtual ~StaticWindow() = default;
};

#endif
