/**
 * @file time_window.hpp
 * @author Pavel Marek
 * @date 21.7.2017
 */

#ifndef INC_TIME_WINDOW_HPP_
#define INC_TIME_WINDOW_HPP_

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include "control_window.hpp"

/**
 * @brief Control window for user time configuration.
 *
 * Format of time is (hours : minutes) where hours and minutes acts like inner
 * control windows that can be focused. When minutes are focused, the value
 * jumps by @ref minuteJump value.
 */
class TimeWindow : public IControlWindow {
private:
	enum selected_t {
		MINUTES,
		HOURS,
		NONE
	} selected = NONE;
public:
	TimeWindow(const Coord& coord = Coord{0,0});
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
	Message _eventHandler(JOYState_TypeDef joy_state) override;
	void setSelected(selected_t sel);
	void noFontDraw() const;
};



#endif /* INC_TIME_WINDOW_HPP_ */
