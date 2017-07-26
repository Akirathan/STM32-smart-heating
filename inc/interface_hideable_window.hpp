/*
 * interface_hideable_window.hpp
 *
 *  Created on: Jul 26, 2017
 *      Author: mayfa
 */

#ifndef INTERFACE_HIDEABLE_WINDOW_HPP_
#define INTERFACE_HIDEABLE_WINDOW_HPP_

class interface_hideable_window {
public:
	virtual ~interface_hideable_window() = default;
	virtual void hide() =0;
	virtual void show() =0;
protected:
	bool hidden = false;
};



#endif /* INTERFACE_HIDEABLE_WINDOW_HPP_ */
