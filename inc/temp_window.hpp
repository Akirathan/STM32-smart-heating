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
#include <vector>
#include "window.hpp"
#include "lcd.hpp"

class TempWindow : public IControlWindow {
public:
	TempWindow();
	TempWindow(const Coord& coord);
	Message eventHandler(JOYState_TypeDef joy_state) override;
	void draw()const override;
	void setFocus(Message msg) override;
	uint32_t getTemp() const;
private:
	uint32_t temp;
	// Temperature higher boundary
	static const uint32_t highBond = 30;
	// Temperature lower boundary
	static const uint32_t lowBond = 8;
	bool focused = false;
	void unsetFocus();
};


#endif /* INC_TEMP_WINDOW_HPP_ */
