/*
 * interval_frame.hpp
 *
 *  Created on: 23. 2. 2017
 *      Author: Mayfa
 */

#ifndef INC_INTERVALFRAME_HPP_
#define INC_INTERVALFRAME_HPP_

#include <vector>
#include "stm3210c_eval.h"
#include "stm3210c_eval_io.h" //?
#include "stm3210c_eval_lcd.h"
#include "buttons.hpp"
#include "input.hpp"
#include "stats.hpp"
#include "time.hpp"
#include "time_window.hpp"
#include "temp_window.hpp"
#include "window_system.hpp"

struct intervalframe_data {
	intervalframe_data();
	intervalframe_data(uint32_t from, uint32_t to, uint32_t temp);
	uint32_t from;
	uint32_t to;
	uint32_t temp;
};

class intervalframe {
	time_window time_from;
	time_window time_to;
	temp_window temp;
	button next_button;
	button end_button;
	std::vector<intervalframe_data> data;
	void draw_header();
	intervalframe_data process_interval();
public:
	intervalframe();
	std::vector<intervalframe_data> & pass_control();
};


#endif /* INC_INTERVALFRAME_HPP_ */
