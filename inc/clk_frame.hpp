/*
 * clkframe.hpp
 *
 *  Created on: 4.2.2017
 *      Author: Mayfa
 */

#ifndef CLK_FRAME_HPP_
#define CLK_FRAME_HPP_

#include <button.hpp>
#include "stm3210c_eval.h"
#include "time_window.hpp"
#include "window_system.hpp"
#include "lcd.hpp"

/**
 * @brief
 * Represents class for user time selection. The user time selection consists
 * of two windows - clock (hours : minutes) and ok button.
 */
class ClkFrame {
public:
	ClkFrame();
	RTC_TimeTypeDef passControl();
private:
	void drawHeader() const;
	TimeWindow time;
	Button ok_button;
};

#endif /* CLK_FRAME_HPP_ */
