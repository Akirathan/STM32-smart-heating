/*
 * static_temp_window.hpp
 *
 *  Created on: Jul 21, 2017
 *      Author: mayfa
 */

#ifndef INC_STATIC_TEMP_WINDOW_HPP_
#define INC_STATIC_TEMP_WINDOW_HPP_

#include <cstdio>
#include "stm3210c_eval_lcd.h"
#include "window.hpp"
#include "rtc.hpp"
#include "lcd.hpp"
#include "callbacks.hpp"
#include "interface_hideable_window.hpp"

/**
 * Can run on background. Use hide method to force not-drawing of this window.
 */
class static_temp_window : public static_window,
							public interface_min_callback,
							public interface_hideable_window
{
	uint32_t temp = 0;
	bool registered_callback = false;
public:
	static_temp_window(const coord &c = coord{0,0});
	~static_temp_window();
	virtual void draw() const override;
	virtual void min_callback() override;
	void set_temp(uint32_t temp);
	void hide() override;
	void show() override;
	void measure();
};



#endif /* INC_STATIC_TEMP_WINDOW_HPP_ */
