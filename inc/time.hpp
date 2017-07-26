/*
 * time.hpp
 *
 *  Created on: Jul 14, 2017
 *      Author: mayfa
 */

#ifndef INC_TIME_HPP_
#define INC_TIME_HPP_

#include <cstdint>

namespace time {

struct time {
	time() = default;
	time(uint32_t hours, uint32_t minutes);
	uint32_t hours = 0;
	uint32_t minutes = 0;
};

uint32_t serialize(time t);
time deserialize(uint32_t t);

} // namespace time

#endif /* INC_TIME_HPP_ */
