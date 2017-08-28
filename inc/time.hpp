/**
 * @file time.hpp
 * @author Pavel Marek
 * @date 14.7.2017
 *
 * @brief This file contains functions to (de)serialize @ref Time::Time struct
 * 		  to EEPROM.
 */

#ifndef INC_TIME_HPP_
#define INC_TIME_HPP_

#include <cstdint>

namespace Time {

struct Time {
	Time() = default;
	Time(uint32_t hours, uint32_t minutes);
	uint32_t hours = 0;
	uint32_t minutes = 0;
};

uint32_t serialize(Time t);
Time deserialize(uint32_t t);

} // namespace Time

#endif /* INC_TIME_HPP_ */
