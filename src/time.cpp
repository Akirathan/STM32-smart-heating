/*
 * time.cpp
 *
 *  Created on: Jul 14, 2017
 *      Author: mayfa
 */

#include "time.hpp"

namespace Time {

Time::Time(uint32_t hours, uint32_t minutes)
	: hours(hours), minutes(minutes)
{ }

uint32_t serialize(Time t)
{
	return t.hours*60 + t.minutes;
}

Time deserialize(uint32_t t)
{
	return Time{t/60, t%60};
}

} // namespace Time
