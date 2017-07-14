/*
 * interval_window.hpp
 *
 *  Created on: 18. 2. 2017
 *      Author: Mayfa
 */

#ifndef INC_INTERVAL_WINDOW_HPP_
#define INC_INTERVAL_WINDOW_HPP_

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <vector>
#include "window.hpp"

class TimeWindow : public ControlWindow {
private:
	enum selected_t {
		MINUTES,
		HOURS,
		NONE
	} selected_;
public:
	TimeWindow(const Coord &coord);
	TimeWindow(const Coord &coord, uint32_t minute_jump);
	Message event_handler(JOYState_TypeDef joy_state) override;
	void draw()const override;
	void setFocus(Message msg) override;
	void setHours(uint32_t hrs); //
	void setMinutes(uint32_t mins); //
	uint32_t getHours() const; //
	uint32_t getMinutes() const; //
private:
	uint32_t hours_;
	uint32_t minutes_;
	const uint32_t minute_jump;
	void setSelected(selected_t sel);
	void noFontDraw() const;
};

class TempWindow : public ControlWindow {
public:
	TempWindow(const Coord &coord);
	Message event_handler(JOYState_TypeDef joy_state) override;
	void draw()const override;
	void setFocus(Message msg) override;
private:
	uint32_t temp_;
	// Temperature higher boundary
	const uint32_t high_bond = 30;
	// Temperature lower boundary
	const uint32_t low_bond = 8;
	bool focused_;
	std::string getTemp() const;
	void unsetFocus();
};

#endif /* INC_INTERVAL_WINDOW_HPP_ */
