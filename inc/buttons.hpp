/*
 * buttons.hpp
 *
 *  Created on: Jul 14, 2017
 *      Author: mayfa
 *
 * This file contains definitions for generic button.
 */

#ifndef INC_BUTTONS_HPP_
#define INC_BUTTONS_HPP_

#include "stm3210c_eval_lcd.h"
#include "window.hpp"

class button : ControlWindow {
	std::string name_;
	bool pushed_;
	bool focused_;
	void unset_focus();
public:
	virtual void draw() const override;
	button(const Coord &coord, const std::string &name);
	virtual Message event_handler(JOYState_TypeDef joy_state) override;
	virtual void setFocus(Message msg) override;
	bool is_pushed() const;
	void set_pushed(bool b);
};



#endif /* INC_BUTTONS_HPP_ */
