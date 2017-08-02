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
	uint32_t get_x() const;
	uint32_t get_y() const;
protected:
	Coord coord_;
	void save_font() const;
	void load_font() const;
};

class control_window: public Window {
public:
	control_window(const Coord &coord);
	virtual ~control_window() = default;
	virtual Message event_handler(JOYState_TypeDef joy_state) = 0;
	virtual void set_focus(Message msg) = 0;
};

class static_window: public Window {
public:
	static_window(const Coord &coord);
	virtual ~static_window() = default;
};

#endif
