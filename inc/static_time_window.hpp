/*
 * static_time_window.hpp
 *
 *  Created on: Jul 21, 2017
 *      Author: mayfa
 */

#ifndef INC_STATIC_TIME_WINDOW_HPP_
#define INC_STATIC_TIME_WINDOW_HPP_

// Define 1 for printing also seconds
#define SECOND_PRECISION	1

#include <cstdio>
#include "window.hpp"
#include "callbacks.hpp"
#include "rtc.hpp"
#include "lcd.hpp"

class static_time_window : public static_window,
#ifdef SECOND_PRECISION
	public interface_sec_callback
#else
	public Imin_callback
#endif
{
	uint8_t hours;
	uint8_t minutes;
	uint8_t seconds;
	void sec_callback_();
	void min_callback_();
	void inc_min();
	void inc_sec();
public:
	static_time_window(const coord &c = coord{0,0});
	~static_time_window();
	void draw() const override;
	void run_clock();
#ifdef SECOND_PRECISION
	virtual void sec_callback() override;
#else
	virtual void min_callback() override;
#endif
};

#endif /* INC_STATIC_TIME_WINDOW_HPP_ */
