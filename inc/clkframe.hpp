/*
 * clkframe.hpp
 *
 *  Created on: 4.2.2017
 *      Author: Mayfa
 *
 * This file contains a class for user time selection.
 * The user time selection consists of three windows - hours, minutes
 * and ok (button). User can switch between those windows with
 * left and right joystick buttons. When hours or minutes are selected
 * the up and down joystick buttons are used for increase/decrease
 * the values inside those windows.
 */

#ifndef CLKFRAME_HPP_
#define CLKFRAME_HPP_

#include "stm3210c_eval.h"
#include "stm3210c_eval_lcd.h"
#include "buttons.hpp"
#include "time_window.hpp"
#include "window_system.hpp"

class clk_frame {
	void draw_header() const;
	time_window time;
	Button ok_button;
public:
	clk_frame();
	RTC_TimeTypeDef pass_control();
};

#endif /* CLKFRAME_HPP_ */
