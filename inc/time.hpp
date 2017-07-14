/*
 * time.hpp
 *
 *  Created on: Jul 14, 2017
 *      Author: mayfa
 */

#ifndef INC_TIME_HPP_
#define INC_TIME_HPP_

struct time {
	uint32_t hours;
	uint32_t minutes;
};

uint32_t serialize_time(time t);
time deserialize_time(uint32_t t);


#endif /* INC_TIME_HPP_ */
