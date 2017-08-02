/*
 * static_time_window.hpp
 *
 *  Created on: Jul 21, 2017
 *      Author: mayfa
 */

#ifndef INC_STATIC_TIME_WINDOW_HPP_
#define INC_STATIC_TIME_WINDOW_HPP_

#include <cstdio>
#include "window.hpp"
#include "callbacks.hpp"
#include "rtc.hpp"
#include "lcd.hpp"
#include "interface_hideable_window.hpp"

/**
 * Can run on background. Use hide method to force not-drawing of this window.
 */
class static_time_window : public static_window,
							public interface_hideable_window,
							public interface_sec_callback,
							public interface_min_callback
{
	uint8_t hours = 0;
	uint8_t minutes = 0;
	uint8_t seconds = 0;
	bool sec_precision = true;
	void sec_callback_();
	void min_callback_();
	void inc_min();
	void inc_sec();
public:
	static_time_window(const Coord& c = Coord{0,0}, bool sec_precision = true);
	~static_time_window();
	void draw() const override;
	void set_hours(uint8_t hours);
	void set_minutes(uint8_t minutes);
	void hide() override;
	void show() override;
	void run_clock();
	virtual void sec_callback() override;
	virtual void min_callback() override;
};

#endif /* INC_STATIC_TIME_WINDOW_HPP_ */
