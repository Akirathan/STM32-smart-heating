/*
 * static_temp_window.hpp
 *
 *  Created on: Jul 21, 2017
 *      Author: mayfa
 */

#ifndef INC_STATIC_TEMP_WINDOW_HPP_
#define INC_STATIC_TEMP_WINDOW_HPP_

#include <cstdio>
#include "hideable_window.hpp"
#include "rtc_controller.hpp"
#include "stm3210c_eval_lcd.h"
#include "window.hpp"
#include "lcd.hpp"
#include "callbacks.hpp"

/**
 * Can run on background. Use hide method to force not-drawing of this window.
 */
class StaticTempWindow : public StaticWindow, public IMinCallback, public IHideableWindow
{
public:
	StaticTempWindow(const Coord& c = Coord(0,0));
	~StaticTempWindow();
	virtual void draw() const override;
	virtual void minCallback() override;
	void hide() override;
	void show() override;
	void setTemp(uint32_t temp);
	void measure();
private:
	uint32_t temp = 0;
	bool registeredCallback = false;
};


#endif /* INC_STATIC_TEMP_WINDOW_HPP_ */
