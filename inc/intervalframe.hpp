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
#include "stm3210c_eval_lcd.h"
#include "buttons.hpp"
#include "input.hpp"
#include "intervalframe_data.hpp"
#include "stats.hpp"
#include "time.hpp"
#include "time_window.hpp"
#include "static_time_window.hpp"
#include "temp_window.hpp"
#include "static_temp_window.hpp"
#include "window_system.hpp"

class intervalframe {
protected:
	Button next_button;
	Button end_button;
	void draw_header();
public:
	intervalframe();
	virtual ~intervalframe() = default;
	virtual void pass_control() =0;
};

/**
 * Used for setting intervals.
 */
class set_intervalframe : public intervalframe {
	time_window time_from;
	time_window time_to;
	temp_window temp;
	std::vector<intervalframe_data> data;
	intervalframe_data process_interval();
	void draw_header();
public:
	set_intervalframe();
	std::vector<intervalframe_data>& get_data();
	void pass_control() override;
};

/**
 * Used for overview of intervals. When user presses overview
 * button in main frame.
 */
class overview_intervalframe : public intervalframe {
	static_time_window time_from;
	static_time_window time_to;
	static_temp_window temp;
	const std::vector<intervalframe_data>& data;
	void print_data(const intervalframe_data& data);
	void draw_header();
public:
	overview_intervalframe(const std::vector<intervalframe_data>& data);
	void pass_control() override;
};

#endif /* INC_INTERVALFRAME_HPP_ */
