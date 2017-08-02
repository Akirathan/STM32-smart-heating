/*
 * mainframe.hpp
 *
 *  Created on: Jul 21, 2017
 *      Author: mayfa
 */

#ifndef INC_MAINFRAME_HPP_
#define INC_MAINFRAME_HPP_

#include "interval_frame.hpp"
#include "stm3210c_eval_lcd.h"
#include "buttons.hpp"
#include "eeprom.hpp"
#include "static_time_window.hpp"
#include "static_temp_window.hpp"
#include "window_system.hpp"
#include "lcd.hpp"

class MainFrame {
public:
	MainFrame();
	void passControl();
private:
	StaticTimeWindow timeWindow;
	StaticTempWindow actualTempWindow;
	StaticTempWindow presetTempWindow;
	Button overviewButton;
	Button resetButton;
	void drawHeader();
};


#endif /* INC_MAINFRAME_HPP_ */
