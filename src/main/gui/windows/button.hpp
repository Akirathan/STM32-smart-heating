/**
 * @file buttons.hpp
 * @author Pavel Marek
 * @date 14.7.2017
 * @brief This file contains a definition for generic button.
 */

#ifndef INC_BUTTONS_HPP_
#define INC_BUTTONS_HPP_

#include "lcd.hpp"
#include "control_window.hpp"

/**
 * Currently there are just OK, END or NEXT buttons in the application and
 * pressing of any of those buttons means ending current frame.
 */
class Button : public IControlWindow {
public:
	Button(const Coord& coord, const char *name);
	Button();
	virtual void draw() const override;
	virtual void clear() override;
	virtual void setFocus(Message msg) override;
	bool isPushed() const;
	void setPushed(bool b);
private:
	static const size_t NAME_LEN = 15;

	char name[15];
	bool pushed = false;
	bool focused = false;
	bool hidden = false;
	virtual Message _eventHandler(JOYState_TypeDef joy_state) override;
	void unsetFocus();
};


#endif /* INC_BUTTONS_HPP_ */
