/*
 * callbacks.hpp
 *
 *  Created on: Jul 21, 2017
 *      Author: mayfa
 *
 * This file contains classes for callback interfaces. The callbacks
 * shall be registered inside rtc singleton class.
 */

#ifndef INC_CALLBACKS_HPP_
#define INC_CALLBACKS_HPP_

/**
 * @brief
 * Objects implementing this interface gets notification every second via
 * @ref ISecCallback::secCallback.
 */
class ISecCallback {
public:
	virtual ~ISecCallback() = default;
	virtual void secCallback() = 0;
	/**
	 * @brief
	 * This method should just register this object to RTCController's sec
	 * callbacks. Note that there is no default implementation because it
	 * would cause circular dependency on RTCController.
	 */
	virtual void registerSecCallback() = 0;
};

/**
 * @brief
 * Objects implementing this interface gets notification every minute via
 * @ref IMinCallback::minCallback
 */
class IMinCallback {
public:
	virtual ~IMinCallback() = default;
	virtual void minCallback() = 0;
	/**
	 * @brief
	 * This method should just register this object to RTCController's min
	 * callbacks. Note that there is no default implementation because it
	 * would cause circular dependency on RTCController.
	 */
	virtual void registerMinCallback() = 0;
};


#endif /* INC_CALLBACKS_HPP_ */
