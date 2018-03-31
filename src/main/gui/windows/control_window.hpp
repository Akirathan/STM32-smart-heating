/**
 * @file control_window.hpp
 * @author Pavel Marek
 * @date Aug 28, 2017
 */

#ifndef CONTROL_WINDOW_HPP_
#define CONTROL_WINDOW_HPP_

#include "window.hpp"

/**
 * @brief Interface for user-controllable windows.
 *
 * User-controllable windows are those windows whose values can be modified
 * via up/down joystick.
 */
class IControlWindow: public Window {
public:
	IControlWindow(const Coord& coord);
	virtual ~IControlWindow() = default;
	/**
	 * @brief Handles joystick button presses.
	 *
	 * Also tries to acquire lock for this window. Note that this window could
	 * be locked for drawing. If lock is acquired, _eventHandler implementation
	 * is called.
	 *
	 * @return @ref Message::FOCUS_LEFT or @ref Message::FOCUS_RIGHT if the
	 * 		   focus should be placed onto (left or right) neighbor control
	 * 		   window, or @ref Message::NONE if the focus should stay in
	 * 		   current window, or @ref Message::EXIT if the current frame should
	 * 		   quit.
	 */
	Message eventHandler(JOYState_TypeDef joy_state);
	/**
	 * @brief Sets focus for this window.
	 * @param msg is either @ref Message::LEFT or @ref Message::RIGHT, depending
	 * 		  from where the focus was passed to this window.
	 */
	virtual void setFocus(Message msg) = 0;

private:
	virtual Message _eventHandler(JOYState_TypeDef joy_state) = 0;
};


#endif /* CONTROL_WINDOW_HPP_ */
