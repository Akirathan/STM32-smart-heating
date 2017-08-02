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

#include <string>
#include "stm3210c_eval_lcd.h"
#include "window.hpp"

class Button : public control_window {
	std::string name;
	bool pushed = false;
	bool focused = false;
	void unset_focus();
public:
	Button(const Coord &coord, const std::string &name);
	Button();
	virtual void draw() const override;
	virtual Message event_handler(JOYState_TypeDef joy_state) override;
	virtual void set_focus(Message msg) override;
	bool is_pushed() const;
	void set_pushed(bool b);
};



#endif /* INC_BUTTONS_HPP_ */
