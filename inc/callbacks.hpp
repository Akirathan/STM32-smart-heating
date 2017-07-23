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

class interface_sec_callback {
public:
	virtual ~interface_sec_callback() = default;
	virtual void sec_callback() = 0;
};

class interface_min_callback {
public:
	virtual ~interface_min_callback() = default;
	virtual void min_callback() = 0;
};



#endif /* INC_CALLBACKS_HPP_ */
