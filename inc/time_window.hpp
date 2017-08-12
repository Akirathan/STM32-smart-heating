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

class TimeWindow : public IControlWindow {
private:
	enum selected_t {
		MINUTES,
		HOURS,
		NONE
	} selected = NONE;
public:
	TimeWindow();
	TimeWindow(const Coord& coord);
	Message eventHandler(JOYState_TypeDef joy_state) override;
	void draw()const override;
	void setFocus(Message msg) override;
	void setHours(uint32_t hrs); //
	void setMinutes(uint32_t mins); //
	uint32_t getHours() const; //
	uint32_t getMinutes() const; //
private:
	uint32_t hours = 0;
	uint32_t minutes = 0;
	static const uint32_t minuteJump = 5;
	void setSelected(selected_t sel);
	void noFontDraw() const;
};



#endif /* INC_TIME_WINDOW_HPP_ */
