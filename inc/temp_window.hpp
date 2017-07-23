/*
 * temp_window.hpp
 *
 *  Created on: Jul 21, 2017
 *      Author: mayfa
 */

#ifndef INC_TEMP_WINDOW_HPP_
#define INC_TEMP_WINDOW_HPP_

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <vector>
#include "window.hpp"

class temp_window : public control_window {
	uint32_t temp;
	// Temperature higher boundary
	static const uint32_t high_bond = 30;
	// Temperature lower boundary
	static const uint32_t low_bond = 8;
	bool focused;
	std::string get_tempstring() const;
	void unset_focus();
public:
	temp_window();
	temp_window(const coord &coord);
	Message event_handler(JOYState_TypeDef joy_state) override;
	void draw()const override;
	void set_focus(Message msg) override;
	uint32_t get_temp() const;
};


#endif /* INC_TEMP_WINDOW_HPP_ */
