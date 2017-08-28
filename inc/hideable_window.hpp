/**
 * @file interface_hideable_window.hpp
 * @author Pavel Marek
 * @date 26.7.2017
 */

#ifndef HIDEABLE_WINDOW_HPP_
#define HIDEABLE_WINDOW_HPP_

/**
 * @brief Simple interface for hideable window.
 */
class IHideableWindow {
public:
	virtual ~IHideableWindow() = default;
	virtual void hide() =0;
	virtual void show() =0;
protected:
	bool hidden = false;
};


#endif /* HIDEABLE_WINDOW_HPP_ */
