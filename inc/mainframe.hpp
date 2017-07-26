/*
 * mainframe.hpp
 *
 *  Created on: Jul 21, 2017
 *      Author: mayfa
 */

#ifndef INC_MAINFRAME_HPP_
#define INC_MAINFRAME_HPP_

#include "stm3210c_eval_lcd.h"
#include "buttons.hpp"
#include "intervalframe.hpp"
#include "eeprom.hpp"
#include "static_time_window.hpp"
#include "static_temp_window.hpp"
#include "window_system.hpp"
#include "lcd.hpp"

class mainframe {
	static_time_window time;
	static_temp_window act_temp;
	static_temp_window set_temp;
	button overview_button;
	button reset_button;
	void draw_header();
public:
	mainframe();
	void pass_control();
};


#endif /* INC_MAINFRAME_HPP_ */
