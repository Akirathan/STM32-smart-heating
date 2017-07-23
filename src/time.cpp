/*
 * time.cpp
 *
 *  Created on: Jul 14, 2017
 *      Author: mayfa
 */

#include "time.hpp"

time::time() : hours{0}, minutes{0}
{ }

time::time(uint32_t hours, uint32_t minutes) : hours{hours}, minutes{minutes}
{ }

uint32_t serialize_time(time t)
{
	return t.hours*60 + t.minutes;
}

time deserialize_time(uint32_t t)
{
	return time{t/60, t%60};
}
