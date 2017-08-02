/*
 * static_time_window.hpp
 *
 *  Created on: Jul 21, 2017
 *      Author: mayfa
 */

#ifndef INC_STATIC_TIME_WINDOW_HPP_
#define INC_STATIC_TIME_WINDOW_HPP_

#include <cstdio>
#include "hideable_window.hpp"
#include "rtc_controller.hpp"
#include "window.hpp"
#include "callbacks.hpp"
#include "lcd.hpp"

/**
 * Can run on background. Use hide method to force not-drawing of this window.
 */
class StaticTimeWindow : public StaticWindow, public IHideableWindow,
	public ISecCallback, public IMinCallback
{
public:
	StaticTimeWindow(const Coord& c = Coord(0,0), bool sec_precision = true);
	~StaticTimeWindow();
	void draw() const override;
	void setHours(uint8_t hours);
	void setMinutes(uint8_t minutes);
	void hide() override;
	void show() override;
	void runClock();
	virtual void secCallback() override;
	virtual void minCallback() override;
private:
	uint8_t hours = 0;
	uint8_t minutes = 0;
	uint8_t seconds = 0;
	bool secPrecision = true;
	void secCallback_();
	void minCallback_();
	void incMin();
	void incSec();
};

#endif /* INC_STATIC_TIME_WINDOW_HPP_ */
