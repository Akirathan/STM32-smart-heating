/**
 * @file callbacks.hpp
 * @author Pavel Marek
 * @date 21.7.2017
 * @brief This file contains classes for callback interfaces.
 */

#ifndef INC_CALLBACKS_HPP_
#define INC_CALLBACKS_HPP_

#include "stm3210c_eval.h"
#include "input.hpp"

/**
 * @brief Interface for second callback.
 *
 * Objects implementing this interface gets notification every second via
 * @ref ISecCallback::secCallback.
 */
class ISecCallback {
public:
	virtual ~ISecCallback() = default;
	virtual void secCallback() = 0;
	/**
	 * @brief
	 * This method should just register this object to @ref RTCController sec
	 * callbacks. Note that there is no default implementation because it
	 * would cause circular dependency.
	 */
	virtual void registerSecCallback() = 0;
};

/**
 * @brief Interface for minute callback.
 *
 * Objects implementing this interface gets notification every minute via
 * @ref IMinCallback::minCallback.
 */
class IMinCallback {
public:
	virtual ~IMinCallback() = default;
	virtual void minCallback() = 0;
	/**
	 * @brief
	 * This method should just register this object to @ref RTCController min
	 * callbacks. Note that there is no default implementation because it
	 * would cause circular dependency on RTCController.
	 */
	virtual void registerMinCallback() = 0;
};

/**
 * @brief Receiver for arbitrary input callback.
 */
class IInputCallback {
public:
	virtual ~IInputCallback() = default;
	virtual void inputCallback(Input input) = 0;
	virtual void registerInputCallback() = 0;
};

class IExitMessageCallback {
public:
	virtual ~IExitMessageCallback() = default;
	virtual void exitMessageCallback() = 0;
	virtual void registerExitMessageCallback() = 0;
};


#endif /* INC_CALLBACKS_HPP_ */
