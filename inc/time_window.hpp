/*
 * time_window.hpp
 *
 *  Created on: Jul 21, 2017
 *      Author: mayfa
 */

#ifndef INC_TIME_WINDOW_HPP_
#define INC_TIME_WINDOW_HPP_

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <vector>
#include "window.hpp"

class time_window : public control_window {
	enum selected_t {
		MINUTES,
		HOURS,
		NONE
	} selected_;

	uint32_t hours;
	uint32_t minutes;
	static const uint32_t minute_jump = 5;
	void set_selected(selected_t sel);
	void nofont_draw() const;
public:
	time_window();
	time_window(const Coord &coord);
	Message event_handler(JOYState_TypeDef joy_state) override;
	void draw()const override;
	void set_focus(Message msg) override;
	void set_hours(uint32_t hrs); //
	void set_minutes(uint32_t mins); //
	uint32_t get_hours() const; //
	uint32_t get_minutes() const; //
};



#endif /* INC_TIME_WINDOW_HPP_ */
