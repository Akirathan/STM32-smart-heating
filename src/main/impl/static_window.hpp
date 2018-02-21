/**
 * @file static_window.hpp
 * @author Pavel Marek
 * @date Aug 28, 2017
 */

#ifndef STATIC_WINDOW_HPP_
#define STATIC_WINDOW_HPP_

#include "window.hpp"

/**
 * @brief Interface for user non-controllable windows.
 *
 * User non-controllable windows are those windows that cannot be focused.
 * Classes that inherits this interface only shows some (text) information.
 */
class IStaticWindow: public Window {
public:
	IStaticWindow(const Coord &coord);
	virtual ~IStaticWindow() = default;
};


#endif /* STATIC_WINDOW_HPP_ */
