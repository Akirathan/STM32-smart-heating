/*
 * buttons.hpp
 *
 *  Created on: Jul 14, 2017
 *      Author: mayfa
 *
 * This file contains definition for generic button.
 */

#ifndef INC_BUTTONS_HPP_
#define INC_BUTTONS_HPP_

#include <string>
#include "stm3210c_eval_lcd.h"
#include "window.hpp"

class Button : public ControlWindow {
public:
	Button(const Coord& coord, const std::string& name);
	Button();
	virtual void draw() const override;
	virtual Message eventHandler(JOYState_TypeDef joy_state) override;
	virtual void setFocus(Message msg) override;
	bool isPushed() const;
	void setPushed(bool b);
private:
	std::string name;
	bool pushed = false;
	bool focused = false;
	void unsetFocus();
};


#endif /* INC_BUTTONS_HPP_ */
