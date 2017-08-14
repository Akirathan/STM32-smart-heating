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
 * @brief
 * Class representing a static window that shows current time. Both formats
 * (hours : minutes) and (hours : minutes : second) are supported - thatswhy
 * this class derives both @ref ISecCallback and @ref IMinCallback.
 * Format can be specified with a bool flag passed to constructor.
 *
 * @note
 * Note that this class can (and should) run on background.
 * @note
 * Just one of @ref secCallback and @ref minCallback members is used at one
 * time for any instance.
 */
class StaticTimeWindow : public IStaticWindow, public IHideableWindow,
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
	virtual void registerSecCallback() override;
	virtual void minCallback() override;
	virtual void registerMinCallback() override;
private:
	uint8_t hours = 0;
	uint8_t minutes = 0;
	uint8_t seconds = 0;
	bool secPrecision = true;
	void incMin();
	void incSec();
};

#endif /* INC_STATIC_TIME_WINDOW_HPP_ */
