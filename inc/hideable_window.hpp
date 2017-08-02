/*
 * interface_hideable_window.hpp
 *
 *  Created on: Jul 26, 2017
 *      Author: mayfa
 */

#ifndef HIDEABLE_WINDOW_HPP_
#define HIDEABLE_WINDOW_HPP_

class IHideableWindow {
public:
	virtual ~IHideableWindow() = default;
	virtual void hide() =0;
	virtual void show() =0;
protected:
	bool hidden = false;
};



#endif /* HIDEABLE_WINDOW_HPP_ */
