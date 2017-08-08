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

class ISecCallback {
public:
	virtual ~ISecCallback() = default;
	virtual void secCallback() = 0;
	virtual void registerSecCallback() = 0;
};

class IMinCallback {
public:
	virtual ~IMinCallback() = default;
	virtual void minCallback() = 0;
	virtual void registerMinCallback() = 0;
};


#endif /* INC_CALLBACKS_HPP_ */
