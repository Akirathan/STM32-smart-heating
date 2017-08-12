/*
 * mainframe.hpp
 *
 *  Created on: Jul 21, 2017
 *      Author: mayfa
 */

#ifndef INC_MAINFRAME_HPP_
#define INC_MAINFRAME_HPP_

#include <button.hpp>
#include "interval_frame.hpp"
#include "stm3210c_eval_lcd.h"
#include "eeprom.hpp"
#include "static_time_window.hpp"
#include "static_measure_temp_window.hpp"
#include "static_preset_temp_window.hpp"
#include "window_system.hpp"
#include "lcd.hpp"
#include "temp_controller.hpp"

class MainFrame {
public:
	MainFrame();
	void passControl();
private:
	StaticTimeWindow timeWindow;
	StaticMeasureTempWindow actualTempWindow;
	StaticPresetTempWindow presetTempWindow;
	Button overviewButton;
	Button resetButton;
	void drawHeader();
};


#endif /* INC_MAINFRAME_HPP_ */
