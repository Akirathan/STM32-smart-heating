/*
 * intervalframe_data.cpp
 *
 *  Created on: Jul 26, 2017
 *      Author: mayfa
 */

#include "interval_frame_data.hpp"

IntervalFrameData::IntervalFrameData()
	: from(0),
	  to(0),
	  temp(0)
{ }

/**
 * @param from ... serialized time (hours*60 + minutes)
 * @param to   ... serialized time (hours*60 + minutes)
 * @param temp ... tempreature in this interval.
 */
IntervalFrameData::IntervalFrameData(uint32_t from, uint32_t to, uint32_t temp)
	: from(from),
	  to(to),
	  temp(temp)
{ }

bool IntervalFrameData::isSet() const
{
	return (from != 0) || (to != 0) || (temp != 0);
}
