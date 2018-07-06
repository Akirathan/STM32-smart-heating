/**
 * @file hex_char_window.hpp
 * @author Pavel Marek
 * @date Jul 6, 2018
 */

#ifndef HEX_CHAR_WINDOW_HPP_
#define HEX_CHAR_WINDOW_HPP_

#include "control_window.hpp"

/**
 * Window that represents one settable hexadecimal character.
 * This is needed in @ref KeyFrame.
 */
class HexCharWindow : public IControlWindow {
public:
	HexCharWindow();
	HexCharWindow(const Coord &coord);
	virtual void setFocus(Message msg) override;
private:
	char hexChar;
	bool focused;

	virtual void draw() const override;
	virtual Message _eventHandler(JOYState_TypeDef joy_state) override;
	void charUp();
	void charDown();
};



#endif /* HEX_CHAR_WINDOW_HPP_ */
