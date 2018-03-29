/**
 * @file frame.hpp
 * @author Pavel Marek
 * @date 25.8.2017
 */

#ifndef FRAME_HPP_
#define FRAME_HPP_

#include "window_system.hpp"

/**
 * @brief Base class for every frame in the application.
 */
class IFrame {
public:
	virtual ~IFrame() = default;
	/**
	 * @brief Main function for frames.
	 *
	 * Lifecycle of frame ends when this method returns.
	 */
	virtual void passControl() =0;

	/**
	 * WindowSystem is supposed to contain all windows (ie. disposable data)
	 * so one definition for all frames here is OK.
	 */
	inline void clear() {
		windowSystem.clear();
	}

protected:
	WindowSystem windowSystem;
};

#endif /* FRAME_HPP_ */
