/**
 * @file time.hpp
 * @author Pavel Marek
 * @date 14.7.2017
 */

#include "time.hpp"

namespace Time {

Time::Time(uint32_t hours, uint32_t minutes)
	: Time(hours, minutes, 0)
{ }

Time::Time(uint32_t hours, uint32_t minutes, uint32_t seconds)
	: hours(hours),
	  minutes(minutes),
	  seconds(seconds)
{ }

/**
 * Serialize just hours and minutes, not seconds.
 */
uint32_t serialize(Time t)
{
	return t.hours*60 + t.minutes;
}

Time deserialize(uint32_t t)
{
	return Time{t/60, t%60};
}

} // namespace Time
