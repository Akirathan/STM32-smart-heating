/*
 * interval_frame.hpp
 *
 *  Created on: 23. 2. 2017
 *      Author: Mayfa
 */

#ifndef INC_INTERVAL_FRAME_HPP_
#define INC_INTERVAL_FRAME_HPP_

#include <memory>
#include <vector>
#include "stm3210c_eval.h"
#include "stm3210c_eval_io.h" //?
#include "stm3210c_eval_lcd.h"
#include "buttons.hpp"
#include "input.hpp"
#include "stats.hpp"
#include "time.hpp"
#include "window_system.hpp"

// TODO: serialize to flash memory
struct interval_frame_data {
	uint32_t from;
	uint32_t to;
	uint32_t temp;
};

class interval_frame {
	TimeWindow time_to, time_from;
	TempWindow temp;
	button next_button, end_button;
	std::vector<interval_frame_data> data;
	interval_frame_data process_interval();
public:
	void draw_header(); //
	interval_frame();
	std::vector<interval_frame_data> & pass_control();
};


#endif /* INC_INTERVAL_FRAME_HPP_ */
